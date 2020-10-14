//***************************************************************************************

#include "StdAfx.h"
#include "Data.h"
#include "CubicSpline.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

void CCTExam::Clear()
{
	m_OK = FALSE;

	m_r_max_um = 0.0;
	for (int a = 0; a < 360; a++)
	{
		m_ra_max_um[a] = 0.0;
	}

	for (int r = 0; r < MAX_NUM_RINGS; r++)
	{
		for (int a = 0; a < 360; a++)
		{
			m_ax_um[r][a] = INVALID_VALUE;
			m_dh_dr[r][a] = INVALID_VALUE;
		}
	}

	m_ax0_um = 0.0;
	m_ax0_dp = 0.0;
	m_rf0_um = 0.0;
	m_HtZrSurface.Reset();
	m_HtSpSurface.Reset();
	m_HtCnSurface.Reset();
	m_WfSurface.Reset();

	m_sk_ok = FALSE;

	for (int k = 0; k < 3; k++)
	{
		m_kr_ok[k] = FALSE;
	}

	for (int k = 0; k < 11; k++)
	{
		m_zk_ok[k] = FALSE;
	}

	m_rf_ok = FALSE;
	m_is_ok = FALSE;

	m_sph_6mm = 0.0;
}

//***************************************************************************************

BOOL CCTExam::Process()
{
	Clear();

	m_Image.m_w_um = m_Calibration.VideoWidthMicrons;
	m_Image.m_h_um = m_Calibration.VideoHeightMicrons;
	m_Image.m_NumRings = m_Calibration.NumRings;

	if (!m_Header.m_Saved)
	{
		if (!m_Image.m_li_ok)
		{
			m_Image.FindLimbus();
		}
	}

	if (!m_Image.m_ri_ok)
	{
		m_Image.FindVertexAndRings();

		if (m_Header.m_Saved && m_Image.m_ri_ok) ::DB.SaveCTExam(this);
	}


	if (!m_Image.m_ri_ok) return FALSE;

	CopyRiFromImage();

	FindAx();

	if (!CreateHeightSurface())
	{
		Clear();
		return FALSE;
	}

	if (!FindCentralPower())
	{
		Clear();
		return FALSE;
	}

	m_OK = TRUE;

	FindKeratometry();

	FindRefractivePower();

	FindIS();

	CreateBestFitConicoidHeightSurfaces();

	CreateWavefrontSurface();

	return TRUE;
}

//***************************************************************************************

void CCTExam::CopyRiFromImage()
{
	for (int a = 0; a < 360; a++)
	{
		for (int r = m_Image.m_NumRings - 1; r >= 0; r--)
		{
			if (m_Image.m_ri_r_um[r][a] != INVALID_VALUE)
			{
				m_ra_max_um[a] = m_Image.m_ri_r_um[r][a];
				break;
			}
		}

		m_ra_max_um[a] = __min(m_ra_max_um[a], CT_R_MAX_UM);

		if (m_r_max_um < m_ra_max_um[a])
		{
			m_r_max_um = m_ra_max_um[a];
		}
	}
}

//***************************************************************************************

void CCTExam::FindAx()
{
	CCubicSpline Spline;
	Matrix<real> r_um(4);
	Matrix<real> ax_um(4, 1, m_Calibration.BallsAx);

	for (int r = 0; r < m_Image.m_NumRings; r++)
	{
		for (int a = 0; a < 360; a++)
		{
			if (m_Image.m_ri_r_um[r][a] != INVALID_VALUE)
			{
				for (int b = 0; b < 4; b++)
				{
					r_um[b] = m_Calibration.BallsRi[b][r][a];
				}
				if (Spline.Create(r_um, ax_um))
				{
					Spline.GetAt(m_Image.m_ri_r_um[r][a], m_ax_um[r][a], TRUE);
				}
			}
		}
	}
}

//***************************************************************************************

BOOL CCTExam::CreateHeightSurface()
{
	BOOL Res = FALSE;

	real* _r_um = new real[360 * m_Image.m_NumRings];
	real* _a_rd = new real[360 * m_Image.m_NumRings];
	real* _dh_dr = new real[360 * m_Image.m_NumRings];
	int n = 0;

	for (int r = 0; r < m_Image.m_NumRings; r++)
	{
		for (int a = 0; a < 360; a++)
		{
			real r_um = m_Image.m_ri_r_um[r][a];
			if (r_um != INVALID_VALUE && r_um <= CT_R_MAX_UM)
			{
				real t = sqr(m_ax_um[r][a]) - sqr(r_um);
				if (t > 0.0)
				{
					_r_um[n] = r_um;
					_a_rd[n] = a * _Pi_180;
					m_dh_dr[r][a] = (_dh_dr[n] = r_um / sqrt(t));
					n++;
				}
			}
		}
	}

	if (m_HtZrSurface.CreateR(ZERNIKE_ORDER_MAX, m_r_max_um, n, _r_um, _a_rd, _dh_dr, NULL))
	{
		m_HtZrSurface.ComputePiston(TRUE);
		m_HtZrSurface.ComputeTilt(TRUE);
		Res = TRUE;
	}

	delete[] _r_um;
	delete[] _a_rd;
	delete[] _dh_dr;

	return Res;
}

//***************************************************************************************

BOOL CCTExam::FindCentralPower()
{
	m_ax0_um = 0.0;
	m_ax0_dp = 0.0;
	m_rf0_um = 0.0;

	for (int a = 0; a < 360; a++)
	{
		real a_rd = a * _Pi_180;
		real ax0 = GetAxUmAt(1.0, a_rd);
		real rf0 = GetRfUmAt(1.0, a_rd);
		if (ax0 == INVALID_VALUE || rf0 == INVALID_VALUE) return FALSE;
		m_ax0_um += ax0;
		m_rf0_um += rf0;
	}

	m_ax0_um /= 360.0;
	m_rf0_um /= 360.0;
	m_ax0_dp = SKI_UM / m_ax0_um;

	return TRUE;
}

//***************************************************************************************

void CCTExam::FindKeratometry()
{
	// average keratometry at 11 circles
	for (int z = 0; z <= 10; z++)
	{
		real sa = 0.0;
		int na = 0;

		for (int a = 0; a < 360; a++)
		{
			real v = GetAxUmAt(z * 500.0, a * _Pi_180);
			if (v != INVALID_VALUE)
			{
				sa += v;
				na++;
			}
		}

		if (na == 360)
		{
			m_zk_ok[z] = TRUE;
			m_zk_av_dp[z] = SKI_UM * na / sa;
		}
		else
		{
			m_zk_ok[z] = FALSE;
			m_zk_av_dp[z] = INVALID_VALUE;
		}
	}

	// keratometry at 3 zones
	int r1_um[3] = { 0, 1500, 2500 };
	int r2_um[3] = { 1500, 2500, 3500 };

	for (int z = 0; z < 3; z++)
	{
		Matrix<real> FA(360);
		int na = 0;
		for (int a = 0; a < 360; a++)
		{
			int nr = 0;
			real sr = 0.0;

			for (int r_um = r1_um[z]; r_um <= r2_um[z]; r_um += 100)
			{
				real v = GetAxUmAt(r_um, a * _Pi_180);
				if (v != INVALID_VALUE)
				{
					nr++;
					sr += v;
				}
			}

			if (nr == (r2_um[z] - r1_um[z]) / 100 + 1)
			{
				na++;
				FA[a] = sr / nr;
			}
		}

		if (na == 360)
		{
			real min = DBL_MAX;
			int amin;
			real max = -DBL_MAX;
			int amax;

			for (int a = 0; a < 360; a++)
			{
				if (min > FA[a])
				{
					min = FA[a];
					amin = a;
				}
				if (max < FA[a])
				{
					max = FA[a];
					amax = a;
				}
			}

			m_kr_ok[z] = TRUE;
			m_kr_st_um[z][0] = min;
			m_kr_st_dp[z][0] = SKI_UM / m_kr_st_um[z][0];
			m_kr_st_dg[z][0] = amin;
			m_kr_fl_um[z][0] = max;
			m_kr_fl_dp[z][0] = SKI_UM / m_kr_fl_um[z][0];
			m_kr_fl_dg[z][0] = amax;

			int b = amin + 180;
			if (b > 359) b -= 360;

			min = DBL_MAX;

			for (int a = b - 90; a <= b + 90; a++)
			{
				int c = a < 0 ? a + 360 : (a > 359 ? a - 360 : a);
				if (min > FA[c])
				{
					min = FA[c];
					amin = c;
				}
			}

			m_kr_st_um[z][1] = min;
			m_kr_st_dp[z][1] = SKI_UM / m_kr_st_um[z][1];
			m_kr_st_dg[z][1] = amin;

			b = amax + 180; if (b > 359) b -= 360;
			max = -DBL_MAX;

			for (int a = b - 90; a <= b + 90; a++)
			{
				int c = a < 0 ? a + 360 : (a > 359 ? a - 360 : a);
				if (max < FA[c])
				{
					max = FA[c];
					amax = c;
				}
			}

			m_kr_fl_um[z][1] = max;
			m_kr_fl_dp[z][1] = SKI_UM / m_kr_fl_um[z][1];
			m_kr_fl_dg[z][1] = amax;
		}
	}


	// sim k
	real X1[360];
	real F1[360];
	int n = 0;

	for (int a = 0; a < 360; a++)
	{
		real v = GetAxUmAt(1500.0, a * _Pi_180);
		if (v != INVALID_VALUE)
		{
			X1[n] = a;
			F1[n] = v;
			n++;
		}
	}

	if (n >= 270)  // let it be at least three quarters
	{
		Matrix<real> X2(n + 4);
		Matrix<real> F2(n + 4);

		X2[0] = X1[n - 2] - 360;
		F2[0] = F1[n - 2];
		X2[1] = X1[n - 1] - 360;
		F2[1] = F1[n - 1];

		for (int i = 2; i < n + 2; i++)
		{
			X2[i] = X1[i - 2];
			F2[i] = F1[i - 2];
		}

		X2[n + 2] = X1[0] + 360;
		F2[n + 2] = F1[0];
		X2[n + 3] = X1[1] + 360;
		F2[n + 3] = F1[1];

		CCubicSpline Spline;
		Spline.Create(X2, F2);

		Matrix<real> F3(360);

		for (int a = 0; a < 360; a++)
		{
			Spline.GetAt(a, F3[a]);
		}

		Matrix<real> A(360, 2);
		for (int a = 0; a < 360; a++)
		{
			int b = (a + a) % 360;
			A(a, 0) = SIN[b];
			A(a, 1) = COS[b];
		}

		Matrix<real> C;
		if (LSM(A, C, F3))
		{
			int amax = intRound(0.5 * _180_Pi * angle(C[0], C[1])); if (amax >= 180) amax -= 180;
			int amin = amax + 90; if (amin >= 180) amin -= 180;
			m_sk_ok = TRUE;
			m_sk_st_um = 0.5 * (F3[amin] + F3[amin + 180]);
			m_sk_st_dp = SKI_UM / m_sk_st_um;
			m_sk_st_dg = amin;
			m_sk_fl_um = 0.5 * (F3[amax] + F3[amax + 180]);
			m_sk_fl_dp = SKI_UM / m_sk_fl_um;
			m_sk_fl_dg = amax;
			m_sk_av_um = 0.5 * (m_sk_fl_um + m_sk_st_um);
			m_sk_av_dp = SKI_UM / m_sk_av_um;
			m_sk_df_dp = m_sk_st_dp - m_sk_fl_dp;
		}
	}
}

//***************************************************************************************

void CCTExam::FindRefractivePower()
{
	real rf_um_min = DBL_MAX;
	real rf_um_max = -DBL_MAX;
	real rf_um_avg = 0.0;
	int a_min = 0;
	int a_max = 0;
	int n = 0;

	for (int a = 0; a < 180; a++)
	{
		real rf_um_a_avg = 0.0;
		real sa = 0.0;
		int na = 0;

		for (int i = 1; i <= 15; i++)
		{
			real r_um = i * 100.0;
			real r1_um = r_um - 50.0;
			real r2_um = r_um + 50.0;
			real s = r2_um * r2_um - r1_um * r1_um;
			real rf1_um = GetRfUmAt(r_um, a        * _Pi_180);
			real rf2_um = GetRfUmAt(r_um, (a + 180) * _Pi_180);

			if (rf1_um != INVALID_VALUE && rf2_um != INVALID_VALUE)
			{
				rf_um_a_avg += (rf1_um + rf2_um) * s;
				sa += (s + s);
				na++;
			}
		}

		if (na == 15)
		{
			rf_um_a_avg /= sa;

			if (rf_um_a_avg > rf_um_max)
			{
				rf_um_max = rf_um_a_avg;
				a_max = a;
			}

			if (rf_um_a_avg < rf_um_min)
			{
				rf_um_min = rf_um_a_avg;
				a_min = a;
			}

			rf_um_avg += rf_um_a_avg;
			n++;
		}
	}

	if (n == 180)
	{
		rf_um_avg /= n;
		m_rf_st_dp = RFI_UM / rf_um_min;
		m_rf_st_dg = a_min;
		m_rf_fl_dp = RFI_UM / rf_um_max;
		m_rf_fl_dg = a_max;
		m_rf_av_dp = RFI_UM / rf_um_avg;
		m_rf_df_dp = m_rf_st_dp - m_rf_fl_dp;
		m_rf_ok = TRUE;
	}
}

//***************************************************************************************

void CCTExam::FindIS()
{
	real ax_sup_um = 0.0;
	real ax_inf_um = 0.0;
	int ns = 0;
	int ni = 0;

	for (int a = 30; a <= 150; a += 30)
	{
		real ax_um = GetAxUmAt(2200.0, a * _Pi_180);

		if (ax_um != INVALID_VALUE)
		{
			ax_sup_um += ax_um;
			ns++;
		}
	}

	for (int a = 210; a <= 330; a += 30)
	{
		real ax_um = GetAxUmAt(2200.0, a * _Pi_180);

		if (ax_um != INVALID_VALUE)
		{
			ax_inf_um += ax_um;
			ni++;
		}
	}

	if (ns == 5 && ni == 5)
	{
		ax_sup_um /= ns;
		ax_inf_um /= ni;
		real ax_sup_dp = SKI_UM / ax_sup_um;
		real ax_inf_dp = SKI_UM / ax_inf_um;
		m_is_df_dp = ax_inf_dp - ax_sup_dp;
		m_is_ok = TRUE;
	}
}

//***************************************************************************************

void CCTExam::CreateBestFitConicoidHeightSurfaces()
{
	CZernikeSurface HtZrSurface;
	HtZrSurface = m_HtZrSurface;

	for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
	{
		if (CZernikeSurface::m_frq[z] != 0) HtZrSurface.SetCUm(z, 0.0);
	}

	real d_um = 10.0;
	real r_min_um = 1500.0;
	real r_max_um = m_HtZrSurface.m_r_max_um;

	int n = (int)((r_max_um - r_min_um) / d_um) + 1;

	//6.2.0 0601 for bug of blink capture
	if (r_max_um < r_min_um)
	{
		n = 0;
	}
	//6.2.0

	Matrix<real> r_um(n);
	Matrix<real> ax_um_zernike(n);

	for (int k = 0; k < n; k++)
	{
		r_um[k] = r_min_um + k * d_um;
		HtZrSurface.GetAxUmAt(r_um[k], 0.0, ax_um_zernike[k]);
	}

	real ax0_um_rounded = RealRound(m_ax0_um, 10.0);

	// best fit conicoid
	m_HtCnSurface.Create(r_max_um, m_ax0_um, 0.0);
	real s_best = DBL_MAX;
	CConicSurface HtCnSurface;

	for (int i = -200; i <= 200; i++)
	{
		real q = 0.01 * i;

		for (int j = -50; j <= 50; j++)
		{
			real ax0_um_j = ax0_um_rounded + j * 10;
			real s = 0.0;
			HtCnSurface.Create(r_max_um, ax0_um_j, q);

			for (int k = 0; k < n; k++)
			{
				real ax_um_conic;
				HtCnSurface.GetAxUmAt(r_um[k], 0.0, ax_um_conic);
				s += fabs(ax_um_conic - ax_um_zernike[k]);
			}

			s /= n;

			if (s < s_best)
			{
				s_best = s;
				m_HtCnSurface = HtCnSurface;
			}
		}
	}

	// best fit sphere
	m_HtSpSurface.Create(r_max_um, m_ax0_um, 0.0);
	s_best = DBL_MAX;
	for (int j = -50; j <= 50; j++)
	{
		real ax0_um_j = ax0_um_rounded + j * 10;
		real s = 0.0;
		HtCnSurface.Create(r_max_um, ax0_um_j, 0.0);

		for (int k = 0; k < n; k++)
		{
			real ax_um_conic = ax0_um_j;
			s += fabs(ax_um_conic - ax_um_zernike[k]);
		}

		s /= n;

		if (s < s_best)
		{
			s_best = s;
			m_HtSpSurface = HtCnSurface;
		}
	}
}

//***************************************************************************************

void CCTExam::CreateWavefrontSurface()
{
	real* _r_um = new real[720];
	real* _a_rd = new real[720];
	real* _dw_dx = new real[720];
	real* _dw_dy = new real[720];

	int n = 0;

	for (int a = 0; a < 360; a += 10)
	{
		real a_rd = a * _Pi_180;
		for (int r = 1; r <= 20; r++)
		{
			real r_um = r * CT_R_MAX_UM / 20.0;
			if (r_um > m_ra_max_um[a]) break;

			real h_um, dh_dx, dh_dy;
			if (!m_HtZrSurface.GetAt(r_um, a_rd, &h_um, &dh_dx, &dh_dy, NULL, NULL, NULL)) continue;

			real tan_ax = dh_dx;
			real tan_ay = dh_dy;
			real tan_a = hyp(tan_ax, tan_ay); if (tan_a == 0.0) continue;
			real sin_a = tan_a / sqrt(1.0 + sqr(tan_a));

			real sin_g = sin_a / 1.3375; // Snell's law
			real tan_g = sin_g / sqrt(1.0 - sqr(sin_g));

			real tan_b = (tan_a - tan_g) / (1.0 + tan_a * tan_g);
			real k = tan_b / tan_a;
			real tan_bx = k * tan_ax;
			real tan_by = k * tan_ay;

			real dx_um = r_um * COS[a] - (m_rf0_um - h_um) * tan_bx;
			real dy_um = r_um * SIN[a] - (m_rf0_um - h_um) * tan_by;

			_r_um[n] = r_um;
			_a_rd[n] = a_rd;
			_dw_dx[n] = -1.3375 * dx_um / m_rf0_um;
			_dw_dy[n] = -1.3375 * dy_um / m_rf0_um;

			n++;
		}
	}

	m_WfSurface.CreateXY(ZERNIKE_ORDER_MAX, m_r_max_um, n, _r_um, _a_rd, _dw_dx, _dw_dy, NULL);
	m_WfSurface.ComputePiston(TRUE);
	m_WfSurface.ComputeTilt(TRUE);

	delete[] _r_um;
	delete[] _a_rd;
	delete[] _dw_dx;
	delete[] _dw_dy;

	// spherical aberration at D = 6 mm
	CZernikeSurface WfSurface;
	WfSurface = m_WfSurface;
	WfSurface.ChangeRMaxUm(3000);
	m_sph_6mm = WfSurface.GetCUm(12);
}

//***************************************************************************************

void CCTExam::CreateDpm2D(CMap2D& Map, const int Type, const int Unit, const real dr_um)
{
	if (!m_OK) return;

	Map.Create(m_r_max_um, dr_um);

	for (int y = -Map.m_nr; y <= Map.m_nr; y++)
	{
		real y_um = y * dr_um;

		for (int x = -Map.m_nr; x <= Map.m_nr; x++)
		{
			real x_um = x * dr_um;
			real r_um = hyp(y_um, x_um);

			if (r_um <= m_r_max_um)
			{
				real a_rd = angle(y_um, x_um);
				real v = INVALID_VALUE;

				if (Unit == DIOPTERS)
				{
					switch (Type)
					{
					case TYPE_CAXM: v = GetAxDpAt(r_um, a_rd); break;
					case TYPE_CTNM: v = GetTnDpAt(r_um, a_rd); break;
					case TYPE_CRFM: v = GetRfDpAt(r_um, a_rd); break;
					}
				}
				else
				{
					switch (Type)
					{
					case TYPE_CAXM: v = GetAxUmAt(r_um, a_rd); break;
					case TYPE_CTNM: v = GetTnUmAt(r_um, a_rd); break;
					case TYPE_CRFM: v = GetRfUmAt(r_um, a_rd); break;
					}

					if (v != INVALID_VALUE)
					{
						v *= 0.001;
					}
				}

				Map.SetAt(y, x, v);
			}
		}
	}
}

//***************************************************************************************

void CCTExam::CreateDpm3D(CMap3D& Map, const int Type, const int Unit)
{
	if (!m_OK) return;

	Map.Create(m_r_max_um);

	for (int r = 0; r <= Map.m_nr; r++)
	{
		real r_um = r * Map.m_dr_um;

		if (r_um == 0.0)
		{
			r_um = 1.0;
		}

		for (int a = 0; a < 360; a++)
		{
			real a_rd = _Pi_180 * a;
			real v = INVALID_VALUE;
			if (Unit == DIOPTERS)
			{
				switch (Type)
				{
				case TYPE_CAXM: v = GetAxDpAt(r_um, a_rd); break;
				case TYPE_CTNM: v = GetTnDpAt(r_um, a_rd); break;
				case TYPE_CRFM: v = GetRfDpAt(r_um, a_rd); break;
				}
			}
			else
			{
				switch (Type)
				{
				case TYPE_CAXM: v = GetAxUmAt(r_um, a_rd); break;
				case TYPE_CTNM: v = GetTnUmAt(r_um, a_rd); break;
				case TYPE_CRFM: v = GetRfUmAt(r_um, a_rd); break;
				}

				if (v != INVALID_VALUE)
				{
					v *= 0.001;
				}
			}
			Map.SetAt(r, a, v);
		}
	}
}

//***************************************************************************************

real CCTExam::GetAxUmAt(const real r_um, const real a_rd)
{
	if (!CheckAt(r_um, a_rd)) return INVALID_VALUE;

	if (r_um == 0.0) return m_ax0_um;

	real ax_um;

	if (!m_HtZrSurface.GetAxUmAt(r_um, a_rd, ax_um)) return INVALID_VALUE;

	return ax_um;
}

//***************************************************************************************

real CCTExam::GetAxDpAt(const real r_um, const real a_rd)
{
	real ax_um = GetAxUmAt(r_um, a_rd);

	if (ax_um == INVALID_VALUE) return INVALID_VALUE;

	return SKI_UM / ax_um;
}

//***************************************************************************************

real CCTExam::GetTnUmAt(const real r_um, const real a_rd)
{
	if (!CheckAt(r_um, a_rd)) return INVALID_VALUE;

	if (r_um == 0.0) return m_ax0_um;

	real tn_um;

	if (!m_HtZrSurface.GetTnUmAt(r_um, a_rd, tn_um)) return INVALID_VALUE;

	return tn_um;
}

//***************************************************************************************

real CCTExam::GetTnDpAt(const real r_um, const real a_rd)
{
	real tn_um = GetTnUmAt(r_um, a_rd);

	if (tn_um == INVALID_VALUE) return INVALID_VALUE;

	return SKI_UM / tn_um;//337500
}

//***************************************************************************************

real CCTExam::GetRfUmAt(const real r_um, const real a_rd)
{
	if (!CheckAt(r_um, a_rd)) return INVALID_VALUE;

	if (r_um == 0.0) return m_rf0_um;

	real rf_um;

	if (!m_HtZrSurface.GetRfUmAt(r_um, a_rd, rf_um)) return INVALID_VALUE;

	return rf_um;
}

//***************************************************************************************

real CCTExam::GetRfDpAt(const real r_um, const real a_rd)
{
	real rf_um = GetRfUmAt(r_um, a_rd);

	if (rf_um == INVALID_VALUE) return INVALID_VALUE;

	return RFI_UM / rf_um;
}

//***************************************************************************************

real CCTExam::GetElUmAt(const real r_um, const real a_rd, const CSurface& RefSurface)
{
	if (!CheckAt(r_um, a_rd)) return INVALID_VALUE;

	real ht_ref_um;
	if (!RefSurface.GetAt(r_um, a_rd, &ht_ref_um, NULL, NULL, NULL, NULL, NULL)) return INVALID_VALUE;

	real ht_um;
	if (!m_HtZrSurface.GetAt(r_um, a_rd, &ht_um, NULL, NULL, NULL, NULL, NULL)) return INVALID_VALUE;

	return ht_ref_um - ht_um;
}

//***************************************************************************************

void CCTExam::CreateElm2D(CMap2D& Map, const CSurface& RefSurface, const real dr_um)
{
	if (!m_OK) return;

	Map.Create(m_r_max_um, dr_um);

	for (int y = -Map.m_nr; y <= Map.m_nr; y++)
	{
		real y_um = y * dr_um;
		for (int x = -Map.m_nr; x <= Map.m_nr; x++)
		{
			real x_um = x * dr_um;
			real r_um = hyp(y_um, x_um);

			if (r_um <= m_r_max_um)
			{
				real a_rd = angle(y_um, x_um);
				real v = GetElUmAt(r_um, a_rd, RefSurface);
				Map.SetAt(y, x, v);
			}
		}
	}
}

//***************************************************************************************

void CCTExam::CreateElm3D(CMap3D& Map, const CSurface& RefSurface)
{
	if (!m_OK) return;

	Map.Create(m_r_max_um);

	for (int r = 0; r <= Map.m_nr; r++)
	{
		real r_um = r * Map.m_dr_um;
		for (int a = 0; a < 360; a++)
		{
			real a_rd = _Pi_180 * a;
			real v = GetElUmAt(r_um, a_rd, RefSurface);
			Map.SetAt(r, a, v);
		}
	}
}

//***************************************************************************************

real CCTExam::GetWfUmAt(const real r_um, const real a_rd, const CSurface& WfSurface) const
{
	if (!CheckAt(r_um, a_rd)) return INVALID_VALUE;

	real wf_um;
	WfSurface.GetAt(r_um, a_rd, &wf_um, NULL, NULL, NULL, NULL, NULL);

	return wf_um;
}

//***************************************************************************************

void CCTExam::CreateWfm2D(CMap2D& Map, const CSurface& WfSurface, const real dr_um)
{
	if (!m_OK) return;

	Map.Create(WfSurface.m_r_max_um, dr_um);

	for (int y = -Map.m_nr; y <= Map.m_nr; y++)
	{
		real y_um = y * dr_um;
		for (int x = -Map.m_nr; x <= Map.m_nr; x++)
		{
			real x_um = x * dr_um;
			real r_um = hyp(y_um, x_um);
			if (r_um <= WfSurface.m_r_max_um) {
				real a_rd = angle(y_um, x_um);
				real v = GetWfUmAt(r_um, a_rd, WfSurface);
				Map.SetAt(y, x, v);
			}
		}
	}
}

//***************************************************************************************

void CCTExam::CreateWfm3D(CMap3D& Map, const CSurface& WfSurface)
{
	if (!m_OK) return;

	Map.Create(WfSurface.m_r_max_um);

	for (int r = 0; r <= Map.m_nr; r++)
	{
		real r_um = r * Map.m_dr_um;
		for (int a = 0; a < 360; a++)
		{
			real a_rd = _Pi_180 * a;
			real v = GetWfUmAt(r_um, a_rd, WfSurface);
			Map.SetAt(r, a, v);
		}
	}
}

//***************************************************************************************

BOOL CCTExam::CheckAt(const real r_um, const real a_rd) const
{
	return r_um <= m_ra_max_um[CheckAngle(intRound(a_rd * _180_Pi))];
}

//***************************************************************************************