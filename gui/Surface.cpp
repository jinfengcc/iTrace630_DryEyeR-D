//***************************************************************************************

#include "StdAfx.h"
#include "Surface.h"
#include "MaskDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BOOL CSurface::GetAxUmAt(const real r_um, const real a_rd, real& ax_um) const
{
	real dz_dr;

	if (!GetAt(r_um, a_rd, NULL, NULL, NULL, &dz_dr, NULL, NULL)) return FALSE;
	if (dz_dr == 0.0) return FALSE;

	ax_um = r_um * sqrt(1.0 + sqr(dz_dr)) / dz_dr;

	return TRUE;
}

//***************************************************************************************

BOOL CSurface::GetTnUmAt(const real r_um, const real a_rd, real& tn_um) const
{
	if (!GetAt(r_um, a_rd, NULL, NULL, NULL, NULL, NULL, &tn_um)) return FALSE;

	if (tn_um == DBL_MAX) return FALSE;

	return TRUE;
}

//***************************************************************************************

BOOL CSurface::GetRfUmAt(const real r_um, const real a_rd, real& rf_um) const
{
	real z_um, dz_dr;
	if (!GetAt(r_um, a_rd, &z_um, NULL, NULL, &dz_dr, NULL, NULL)) return FALSE;

	if (dz_dr == 0.0) return FALSE;

	real tan_a = dz_dr;
	real sin_a = tan_a / sqrt(1.0 + sqr(tan_a));
	real cos_a = sqrt(1.0 - sqr(sin_a));

	real sin_g = sin_a / 1.3375; // Snell's law //cjf note : is 1.3375 be the corneal refractive index?  
	real cos_g = sqrt(1.0 - sqr(sin_g));

	real sin_b = sin_a * cos_g - cos_a * sin_g; // b = a - g
	real cos_b = sqrt(1.0 - sqr(sin_b));
	real cot_b = cos_b / sin_b;

	rf_um = z_um + r_um * cot_b;

	return TRUE;
}

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

CConicSurface::CConicSurface()
{
	Reset();
}

//***************************************************************************************

void CConicSurface::Reset()
{
	CSurface::Reset();

	m_ax0_um = 0.0;
	m_q = 0.0;

	m_a_um = 0.0;
	m_b_um = 0.0;
	m_p_um = 0.0;
}

//***************************************************************************************

void CConicSurface::Create(const real r_max_um, const real ax0_um, const real q)
{
	m_r_max_um = r_max_um;

	m_ax0_um = ax0_um;
	m_q = q;

	// hyperbola
	if (m_q < -1.0)
	{
		m_a_um = m_ax0_um / (-1.0 - m_q);
		m_b_um = m_ax0_um / sqrt(-1.0 - m_q);
		m_p_um = 0.0;
	}
	// parabola
	else if (m_q == -1.0)
	{
		m_a_um = 0.0;
		m_b_um = 0.0;
		m_p_um = m_ax0_um;
	}
	// prolate ellipse, circle, oblate ellipse
	else
	{
		m_a_um = m_ax0_um / (1.0 + m_q);
		m_b_um = m_ax0_um / sqrt(1.0 + m_q);
		m_p_um = 0.0;

		if (m_r_max_um > m_b_um) m_r_max_um = m_b_um;
	}
}

//***************************************************************************************

BOOL CConicSurface::GetAt(const real r_um, const real a_rd, real* z_um, real* dz_dx, real* dz_dy,
	real* dz_dr, real* dz_da, real* rc_um) const
{
	if (r_um > m_r_max_um) return FALSE;

	if (z_um)
	{
		*z_um = 0.0;
		// hyperbola
		if (m_q < -1.0)
		{
			*z_um = m_a_um * (sqrt(1.0 + sqr(r_um / m_b_um)) - 1.0);
		}
		// parabola
		else if (m_q == -1.0)
		{
			*z_um = sqr(r_um) / (2.0 * m_p_um);
		}
		// prolate ellipse, circle, oblate ellipse
		else
		{
			*z_um = m_a_um * (1.0 - sqrt(1.0 - sqr(r_um / m_b_um)));
		}
	}

	//
	real _dz_dr = 0.0;
	if (dz_dr || dz_dx || dz_dy)
	{
		// hyperbola
		if (m_q < -1.0)
		{
			_dz_dr = (m_a_um * r_um) / (m_b_um * sqrt(sqr(m_b_um) + sqr(r_um)));
		}
		// parabola
		else if (m_q == -1.0)
		{
			_dz_dr = r_um / m_p_um;
		}
		// prolate ellipse, circle, oblate ellipse
		else
		{
			_dz_dr = (m_a_um * r_um) / (m_b_um * sqrt(sqr(m_b_um) - sqr(r_um)));
		}
	}

	if (dz_dr) *dz_dr = _dz_dr;
	if (dz_da) *dz_da = 0.0;
	if (dz_dx) *dz_dx = _dz_dr * cos(a_rd);
	if (dz_dy) *dz_dy = _dz_dr * sin(a_rd);

	//
	if (rc_um)
	{
		// hyperbola
		if (m_q < -1.0)
		{
			real b2 = sqr(m_b_um);
			real b4 = sqr(b2);
			real a2 = sqr(m_a_um);
			real r2 = sqr(r_um);
			*rc_um = sqrt(cube(b4 + r2 * (a2 + b2))) / (m_a_um * b4);
		}
		// parabola
		else if (m_q == -1.0)
		{
			real p2 = sqr(m_p_um);
			real r2 = sqr(r_um);
			*rc_um = sqrt(cube(p2 + r2)) / p2;
		}
		// prolate ellipse, circle, oblate ellipse
		else
		{
			real b2 = sqr(m_b_um);
			real b4 = sqr(b2);
			real a2 = sqr(m_a_um);
			real r2 = sqr(r_um);
			*rc_um = sqrt(cube(b4 + r2 * (a2 - b2))) / (m_a_um * b4);
		}
	}

	return TRUE;
}

//***************************************************************************************

real CConicSurface::GetEccentricity()
{
	// hyperbola, parabola, prolate ellipse, circle
	if (m_q <= 0) return sqrt(-m_q);
	// oblate ellipse
	return sqrt(m_q / (1.0 + m_q));
}

//***************************************************************************************

BOOL CConicSurface::GetAxUmAt(const real r_um, const real a_rd, real& ax_um) const
{
	if (r_um > m_r_max_um) return FALSE;

	ax_um = sqrt(sqr(m_ax0_um) - m_q * sqr(r_um));

	return TRUE;
}

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

int CZernikeSurface::m_ord[ZERNIKE_NUM_MODES];
int CZernikeSurface::m_frq[ZERNIKE_NUM_MODES];
real CZernikeSurface::m_nrm[ZERNIKE_NUM_MODES];
real CZernikeSurface::m_rc[ZERNIKE_NUM_MODES][ZERNIKE_NUM_ORDERS];
Matrix<real> CZernikeSurface::m_cs;
Matrix<real> CZernikeSurface::m_sc;

//***************************************************************************************

CZernikeSurface::CZernikeSurface()
{
	static BOOL Init = FALSE;
	if (!Init)
	{
		memset(m_rc, 0, sizeof(m_rc));
		int z = 0;

		for (int o = 0; o < ZERNIKE_NUM_ORDERS; o++)
		{
			for (int f = -o; f <= o; f += 2)
			{
				m_ord[z] = o; //test
				m_frq[z] = f;
				m_nrm[z] = f != 0 ? sqrt(2.0 * (o + 1.0)) : sqrt(o + 1.0);

				for (int k = 0; k <= ((o - abs(f)) >> 1); k++)
				{
					int i = o - 2 * k;
					real c = factorial(o - k) /
						(factorial(k) * factorial(((o + abs(f)) >> 1) - k) * factorial(((o - abs(f)) >> 1) - k));
					m_rc[z][i] = (k & 1) ? -c : c;
				}
				z++;
			}
		}

		m_cs.Create(ZERNIKE_NUM_MODES, ZERNIKE_NUM_MODES);
		m_sc.Create(ZERNIKE_NUM_MODES, ZERNIKE_NUM_MODES);
		m_cs.Fill(0.0);

		for (int i = 0; i < ZERNIKE_NUM_MODES; i++)
		{
			for (int k = 0; k < ZERNIKE_NUM_MODES; k++)
			{
				if (m_frq[i] == m_frq[k]) m_cs(i, k) = m_rc[k][m_ord[i]];
			}
		}

		Invert(m_cs, m_sc);

		Init = TRUE;
	}

	Reset();
}

//***************************************************************************************

void CZernikeSurface::Reset()
{
	CSurface::Reset();

	for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
	{
		m_c[z] = 0.0;
	}
}

//***************************************************************************************

void CZernikeSurface::Add(const CZernikeSurface& Surface)
{
	CZernikeSurface Surface1;
	Surface1 = Surface;
	if (m_r_max_um < Surface1.m_r_max_um)
	{
		Surface1.ChangeRMaxUm(m_r_max_um);
	}
	else if (m_r_max_um > Surface1.m_r_max_um)
	{
		ChangeRMaxUm(Surface1.m_r_max_um);
	}

	for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
	{
		m_c[z] += Surface1.m_c[z];
	}
}

//***************************************************************************************

void CZernikeSurface::Subtract(const CZernikeSurface& Surface)
{
	CZernikeSurface Surface1;

	Surface1 = Surface;

	if (m_r_max_um < Surface1.m_r_max_um)
	{
		Surface1.ChangeRMaxUm(m_r_max_um);
	}
	else if (m_r_max_um > Surface1.m_r_max_um)
	{
		ChangeRMaxUm(Surface1.m_r_max_um);
	}

	for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
	{
		m_c[z] -= Surface1.m_c[z];
	}
}

//***************************************************************************************

void CZernikeSurface::SaveTxtFile(const CString& FileName)
{
	G_As = FileName;
	FILE* pFile = fopen(G_As, "w");
	if (pFile == NULL) return;

	fprintf(pFile, "Normalized Zernike coefficients in microns @ %.2lf mm\n\n", m_r_max_um * 0.002);

	for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
	{
		fprintf(pFile, "%2i\t%3.10lf\n", z, GetCUm(z));
	}

	fclose(pFile);

	G_As = FileName;
	::OpenTxtFile(G_As);
}

//***************************************************************************************

BOOL CZernikeSurface::LoadTxtFile(const CString& FileName)
{
	G_As = FileName;
	FILE* pFile = fopen(G_As, "r");
	if (pFile == NULL) return FALSE;

	char s[1024];
	double r;
	if (fgets(s, 1024, pFile) != NULL)
	{
		uint k = 0;

		while (k < strlen(s) && !(s[k] >= '0' && s[k] <= '9')) k++;

		sscanf(s + k, "%lf", &r);
	}

	if (r < 2.0 || r > 8.0)
	{
		fclose(pFile);
		Error("Scan size is inappropriate");
		return FALSE;
	}

	m_r_max_um = r * 500.0;

	while (fgets(s, 1024, pFile) != NULL)
	{
		int z = -1;
		double v = 0.0;
		sscanf(s, "%i%lf", &z, &v);
		SetCUm(z, v);
	}

	fclose(pFile);
	return TRUE;
}

//***************************************************************************************

BOOL CZernikeSurface::GetAt(const real r_um, const real a_rd,
	real* w_um, real* dw_dx, real* dw_dy, real* dw_dr, real* dw_da, real* rc_um) const
{
	real r = r_um / m_r_max_um;
	real P[ZERNIKE_NUM_ORDERS];
	P[0] = 1.0;

	for (int o = 1; o < ZERNIKE_NUM_ORDERS; o++)
	{
		P[o] = r * P[o - 1];
	}

	real S[ZERNIKE_NUM_FREQUENCIES];
	real C[ZERNIKE_NUM_FREQUENCIES];

	for (int f = 0; f < ZERNIKE_NUM_FREQUENCIES; f++)
	{
		S[f] = sin(f * a_rd);
		C[f] = cos(f * a_rd);
	}

	real w_ = 0.0;
	real dw_dr_ = 0.0;
	real dw_da_ = 0.0;
	real d2w_dr2_ = 0.0;

	for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
	{
		real R = 0.0;
		real dR_dr = 0.0;
		real d2R_dr2 = 0.0;

		for (int o = abs(m_frq[z]); o <= m_ord[z]; o += 2)
		{
			R += m_rc[z][o] * P[o];
			if (o > 0)
			{
				dR_dr += o * m_rc[z][o] * P[o - 1];
				if (o > 1)
				{
					d2R_dr2 += o * (o - 1) * m_rc[z][o] * P[o - 2];
				}
			}
		}

		real c = m_c[z];
		int f = m_frq[z];

		if (f >= 0)
		{
			w_ += c * R         *  C[f];
			dw_dr_ += c * dR_dr     *  C[f];
			dw_da_ += c * R     * f * -S[f];
			d2w_dr2_ += c * d2R_dr2   *  C[f];
		}
		else
		{
			f = -f;
			w_ += c * R         *  S[f];
			dw_dr_ += c * dR_dr     *  S[f];
			dw_da_ += c * R     * f *  C[f];
			d2w_dr2_ += c * d2R_dr2   *  S[f];
		}
	}

	if (w_um) *w_um = m_r_max_um * w_;
	if (dw_dr) *dw_dr = dw_dr_;
	if (dw_da) *dw_da = dw_da_;

	if (r == 0.0)
	{
		if (dw_dx || dw_dy) GetTiltAt(a_rd, NULL, dw_dx, dw_dy);
	}
	else
	{
		if (dw_dx) *dw_dx = dw_dr_ * C[1] - dw_da_ * S[1] / r;
		if (dw_dy) *dw_dy = dw_dr_ * S[1] + dw_da_ * C[1] / r;
	}

	if (rc_um)
	{
		if (d2w_dr2_ == 0.0) *rc_um = DBL_MAX;
		else *rc_um = m_r_max_um * sqrt(cube(1.0 + sqr(dw_dr_))) / d2w_dr2_;
	}

	return TRUE;
}

//***************************************************************************************

void CZernikeSurface::GetTiltAt(const real a_rd, real* tilt, real* tilt_x, real* tilt_y) const
{
	real tx = 0.0, ty = 0.0;

	for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
	{
		switch (m_frq[z])
		{
		case -1: ty += m_c[z] * m_rc[z][1]; break;
		case  1: tx += m_c[z] * m_rc[z][1]; break;
		}
	}

	if (tilt) *tilt = ty * sin(a_rd) + tx * cos(a_rd);
	if (tilt_y) *tilt_y = ty;
	if (tilt_x) *tilt_x = tx;
}

//***************************************************************************************

BOOL CZernikeSurface::CreateXY(const int max_order, const real r_max_um, const int num_points,
	real* r_um, real* a_rd, real* dw_dx, real* dw_dy, real* rmse)
{
	Reset();

	int num_modes = (max_order + 1) * (max_order + 2) / 2;

	if (num_modes > ZERNIKE_NUM_MODES) return FALSE;

	m_r_max_um = r_max_um;

	int num_equations = 2 * num_points;
	int num_unknowns = num_modes - 1;

	if (num_points < num_unknowns) return FALSE;

	Matrix<real> A(num_equations, num_unknowns);
	A.Fill(0.0);

	Matrix<real> COS(max_order + 1);
	Matrix<real> SIN(max_order + 1);
	Matrix<real> POW(max_order + 1);

	for (int p = 0; p < num_points; p++)
	{
		for (int f = 0; f <= max_order; f++)
		{
			COS[f] = cos(f * a_rd[p]);
			SIN[f] = sin(f * a_rd[p]);
		}

		real r = r_um[p] / m_r_max_um;
		POW[0] = 1.0;

		for (int o = 1; o <= max_order; o++)
		{
			POW[o] = r * POW[o - 1];
		}

		for (int z = 1; z < num_modes; z++)
		{
			if (r != 0.0)
			{
				real R = 0.0;
				real dR_dr = 0.0;

				for (int o = abs(m_frq[z]); o <= m_ord[z]; o += 2)
				{
					R += m_rc[z][o] * POW[o];

					if (o > 0)
					{
						dR_dr += o * m_rc[z][o] * POW[o - 1];
					}
				}

				real dZ_dr, dZ_da;
				int f = m_frq[z];

				if (f >= 0)
				{
					dZ_dr = dR_dr *      COS[f];
					dZ_da = R     * f * -SIN[f];
				}
				else
				{
					f = -f;
					dZ_dr = dR_dr *      SIN[f];
					dZ_da = R     * f *  COS[f];
				}

				A(p, z - 1) = dZ_dr * COS[1] - dZ_da * SIN[1] / r;
				A(p + num_points, z - 1) = dZ_dr * SIN[1] + dZ_da * COS[1] / r;
			}
			else
			{
				if (m_frq[z] == -1)
				{
					A(p, z - 1) = 0.0;
					A(p + num_points, z - 1) = m_rc[z][1];
				}
				else if (m_frq[z] == 1)
				{
					A(p, z - 1) = m_rc[z][1];
					A(p + num_points, z - 1) = 0.0;
				}
				else
				{
					A(p, z - 1) = 0.0;
					A(p + num_points, z - 1) = 0.0;
				}
			}
		}
	}

	Matrix<real> F(num_equations);
	Matrix<real> C;

	for (int p = 0; p < num_points; p++)
	{
		F[p] = dw_dx[p];
		F[p + num_points] = dw_dy[p];
	}

	if (!LSM(A, C, F)) return FALSE;

	for (int z = 1; z < num_modes; z++)
	{
		m_c[z] = C[z - 1];
	}

	if (rmse)
	{
		*rmse = 0.0;

		for (int p = 0; p < num_points; p++)
		{
			real w_um, _dw_dx, _dw_dy;
			GetAt(r_um[p], a_rd[p], &w_um, &_dw_dx, &_dw_dy, NULL, NULL, NULL);
			*rmse += sqr(_dw_dx - dw_dx[p]) + sqr(_dw_dy - dw_dy[p]);
		}

		*rmse = sqrt(*rmse);
	}

	return TRUE;
}

//***************************************************************************************

BOOL CZernikeSurface::CreateR(const int max_order, const real r_max_um, const int num_points,
	real* r_um, real* a_rd, real* dw_dr, real* rmse)
{
	Reset();

	int num_modes = (max_order + 1) * (max_order + 2) / 2;

	if (num_modes > ZERNIKE_NUM_MODES) return FALSE;

	m_r_max_um = r_max_um;

	int num_equations = num_points;
	int num_unknowns = num_modes - 1;

	if (num_points < num_unknowns) return FALSE;

	Matrix<real> A(num_equations, num_unknowns);
	A.Fill(0.0);

	Matrix<real> COS(max_order + 1);
	Matrix<real> SIN(max_order + 1);
	Matrix<real> POW(max_order + 1);

	for (int p = 0; p < num_points; p++)
	{
		for (int f = 0; f <= max_order; f++)
		{
			COS[f] = cos(f * a_rd[p]);
			SIN[f] = sin(f * a_rd[p]);
		}

		real r = r_um[p] / m_r_max_um;
		POW[0] = 1.0;

		for (int o = 1; o <= max_order; o++)
		{
			POW[o] = r * POW[o - 1];
		}

		for (int z = 1; z < num_modes; z++)
		{
			real dR_dr = 0.0;
			for (int o = abs(m_frq[z]); o <= m_ord[z]; o += 2)
			{
				if (o > 0)
				{
					dR_dr += o * m_rc[z][o] * POW[o - 1];
				}
			}

			if (m_frq[z] >= 0) A(p, z - 1) = dR_dr * COS[m_frq[z]];
			else               A(p, z - 1) = dR_dr * SIN[-m_frq[z]];
		}
	}

	Matrix<real> F(num_equations, 1, (real*)dw_dr);
	Matrix<real> C;

	if (!LSM(A, C, F)) return FALSE;

	for (int z = 1; z < num_modes; z++)
	{
		m_c[z] = C[z - 1];
	}

	if (rmse)
	{
		*rmse = 0.0;

		for (int p = 0; p < num_points; p++)
		{
			real w_um, _dw_dr;
			GetAt(r_um[p], a_rd[p], &w_um, NULL, NULL, &_dw_dr, NULL, NULL);
			*rmse += sqr(_dw_dr - dw_dr[p]);
		}

		*rmse = sqrt(*rmse);
	}

	return TRUE;
}

//***************************************************************************************

void CZernikeSurface::ComputePiston(const BOOL balance)
{
	m_c[0] = 0.0;

	if (balance)
	{
		for (int z = 3; z < ZERNIKE_NUM_MODES; z++)
		{
			if (m_frq[z] == 0) m_c[0] -= m_c[z] * m_rc[z][0];
		}
	}
}

void CZernikeSurface::ComputeTilt(const BOOL balance)
{
	m_c[1] = 0.0;
	m_c[2] = 0.0;

	if (balance)
	{
		for (int z = 3; z < ZERNIKE_NUM_MODES; z++)
		{
			switch (m_frq[z])
			{
			case -1: m_c[1] -= m_c[z] * m_rc[z][1]; break;
			case   1: m_c[2] -= m_c[z] * m_rc[z][1]; break;
			}
		}
	}
}

//***************************************************************************************

void CZernikeSurface::SetCUm(const int z, const real c_um)
{
	if (z < 0 || z >= ZERNIKE_NUM_MODES) return;

	m_c[z] = m_nrm[z] * c_um / m_r_max_um;
}

//***************************************************************************************

real CZernikeSurface::GetCUm(const int z) const
{
	if (z < 0 || z >= ZERNIKE_NUM_MODES) return 0.0;
	return m_c[z] * m_r_max_um / m_nrm[z];
}

//***************************************************************************************

void CZernikeSurface::ChangeRMaxUm(const real r_max_um)
{
	if (r_max_um >= m_r_max_um) return;

	if (r_max_um <= 0.0)
	{
		m_r_max_um = 0.0;
		for (int z = 0; z < ZERNIKE_NUM_MODES; z++) m_c[z] = 0.0;
		return;
	}

	Matrix<real> c(ZERNIKE_NUM_MODES, 1, m_c);
	Matrix<real> s;

	Multiply(m_cs, c, s);

	real r = r_max_um / m_r_max_um;
	Matrix<real> p(ZERNIKE_NUM_MODES);
	p[0] = 1.0 / r;

	for (int o = 1; o < ZERNIKE_NUM_ORDERS; o++)
	{
		p[o] = p[o - 1] * r;
	}

	for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
	{
		s[z] *= p[m_ord[z]];
	}

	Multiply(m_sc, s, c);

	m_r_max_um = r_max_um;

	for (int z = 0; z < ZERNIKE_NUM_MODES; z++) m_c[z] = c[z];
}

//***************************************************************************************

real CZernikeSurface::ComputeRms() const
{
	real v = 0.0;

	for (int z = 3; z < ZERNIKE_NUM_MODES; z++)
	{
		v += sqr(GetCUm(z));
	}

	return sqrt(v);
}

//***************************************************************************************

void CZernikeSurface::ApplyMask(const CZernikeMask& Mask)
{
	m_c[0] = 0.0;
	m_c[1] = 0.0;
	m_c[2] = 0.0;

	for (int z = 3; z < ZERNIKE_NUM_MODES; z++)
	{
		if (Mask.Get(z) == FALSE) m_c[z] = 0.0;
	}
}

//***************************************************************************************

void CZernikeSurface::GetCombinedRms(const int mode, real& value, int& axis) const
{
	int mode1 = -1, mode2 = -1;
	int z = 0;

	for (int o = 0; o <= m_ord[mode]; o++)
	{
		for (int f = -o; f <= o; f += 2)
		{
			if (o == m_ord[mode])
			{
				if (f == m_frq[mode]) mode1 = z;
				if (f == -m_frq[mode]) mode2 = z;
			}
			z++;
		}
	}

	if (mode1 > mode2) Swap(mode1, mode2);

	if (mode1 != mode2)
	{
		axis = intRound(angle(m_c[mode1], m_c[mode2]) * _180_Pi / m_frq[mode2]) % (360 / m_frq[mode2]);
		value = hyp(GetCUm(mode1), GetCUm(mode2));
	}
	else
	{
		axis = -1;
		value = GetCUm(mode1);
	}
}

//***************************************************************************************

void CZernikeSurface::AddSphEq(const real SphEq)
{
	m_c[4] -= SphEq * (m_r_max_um * 0.000001) / 4.0;
}

//***************************************************************************************

CString CZernikeSurface::m_rms_name[ZERNIKE_NUM_MODES] =
{
	"Piston",
	"Tilt", "Tilt",
	"Astigmatism", "Defocus", "Astigmatism",
	"Trefoil", "Coma", "Coma", "Trefoil",
	"Tetrafoil", "Astigmatism", "Spherical", "Astigmatism", "Tetrafoil",
	"Pentafoil", "Trefoil", "Coma", "Coma", "Trefoil", "Pentafoil",
	"Hexafoil", "Tetrafoil", "Astigmatism", "Spherical", "Astigmatism", "Tetrafoil", "Hexafoil",
	"Heptafoil", "Pentafoil", "Trefoil", "Coma", "Coma", "Trefoil", "Pentafoil", "Heptafoil",
	"Octafoil", "Hexafoil", "Tetrafoil", "Astigmatism", "Spherical", "Astigmatism", "Tetrafoil", "Hexafoil", "Octafoil"
};

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

CZernikeMask::CZernikeMask()
{
	for (int z = 0; z < ZERNIKE_NUM_MODES; z++) m_m[z] = FALSE;
}

//***************************************************************************************

BOOL CZernikeMask::SetType(const int type)
{
	m_m[0] = FALSE;
	m_m[1] = FALSE;
	m_m[2] = FALSE;

	switch (type)
	{
	case MASK_TOTAL:
	{
		for (int z = 3; z < ZERNIKE_NUM_MODES; z++) m_m[z] = TRUE;
	}
	return TRUE;

	case MASK_LO_TOTAL:
	{
		m_m[3] = TRUE;
		m_m[4] = TRUE;
		m_m[5] = TRUE;
		for (int z = 6; z < ZERNIKE_NUM_MODES; z++) m_m[z] = FALSE;
	}
	return TRUE;

	case MASK_DEFOCUS:
	{
		m_m[3] = FALSE;
		m_m[4] = TRUE;
		m_m[5] = FALSE;
		for (int z = 6; z < ZERNIKE_NUM_MODES; z++) m_m[z] = FALSE;
	}
	return TRUE;

	case MASK_ASTIGMATISM:
	{
		m_m[3] = TRUE;
		m_m[4] = FALSE;
		m_m[5] = TRUE;
		for (int z = 6; z < ZERNIKE_NUM_MODES; z++) m_m[z] = FALSE;
	}
	return TRUE;

	case MASK_HO_TOTAL:
	{
		m_m[3] = FALSE;
		m_m[4] = FALSE;
		m_m[5] = FALSE;
		for (int z = 6; z < ZERNIKE_NUM_MODES; z++) m_m[z] = TRUE;
	}
	return TRUE;

	case MASK_COMA:
	{
		for (int z = 3; z < ZERNIKE_NUM_MODES; z++) m_m[z] = FALSE;
		m_m[7] = TRUE;
		m_m[8] = TRUE;
	}
	return TRUE;

	case MASK_TREFOIL:
	{
		for (int z = 3; z < ZERNIKE_NUM_MODES; z++) m_m[z] = FALSE;
		m_m[6] = TRUE;
		m_m[9] = TRUE;
	}
	return TRUE;

	case MASK_SPHERICAL:
	{
		for (int z = 3; z < ZERNIKE_NUM_MODES; z++) m_m[z] = FALSE;
		m_m[12] = TRUE;
	}
	return TRUE;

	case MASK_4ORD_ASTIGMATISM:
	{
		for (int z = 3; z < ZERNIKE_NUM_MODES; z++) m_m[z] = FALSE;
		m_m[11] = TRUE;
		m_m[13] = TRUE;
	}
	return TRUE;

	case MASK_TOTAL_NO_DEFOCUS:
	{
		m_m[3] = TRUE;
		m_m[4] = FALSE;
		m_m[5] = TRUE;
		for (int z = 6; z < ZERNIKE_NUM_MODES; z++) m_m[z] = TRUE;
	}
	return TRUE;

	case MASK_NONE:
	{
		for (int z = 3; z < ZERNIKE_NUM_MODES; z++) m_m[z] = FALSE;
	}
	return TRUE;
	}

	CMaskDlg* pDlg = new CMaskDlg(NULL);

	for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
	{
		pDlg->m_m[z] = m_m[z];
	}

	BOOL Res = pDlg->DoModal() == IDOK;

	if (Res)
	{
		m_m[0] = FALSE;
		m_m[1] = FALSE;
		m_m[2] = FALSE;

		for (int z = 3; z < ZERNIKE_NUM_MODES; z++)
		{
			m_m[z] = pDlg->m_m[z];
		}
	}

	delete pDlg;

	return Res;
}

//***************************************************************************************

int CZernikeMask::GetType() const
{
	int Type[11] =
	{
		MASK_TOTAL,
		MASK_LO_TOTAL,
		MASK_DEFOCUS,
		MASK_ASTIGMATISM,
		MASK_HO_TOTAL,
		MASK_COMA,
		MASK_TREFOIL,
		MASK_SPHERICAL,
		MASK_4ORD_ASTIGMATISM,
		MASK_TOTAL_NO_DEFOCUS,
		MASK_NONE
	};

	CZernikeMask Mask;

	for (int i = 0; i < 11; i++)
	{
		Mask.SetType(Type[i]);
		if (memcmp(m_m, Mask.m_m, ZERNIKE_NUM_MODES * sizeof(BOOL)) == 0) return Type[i];
	}

	return MASK_OTHER;
}

//***************************************************************************************

void CZernikeMask::GetName(const int Type, CString& Name)
{
	switch (Type)
	{
	case MASK_TOTAL: Name = "Total"; return;
	case MASK_LO_TOTAL: Name = "LO Total"; return;
	case MASK_DEFOCUS: Name = "Defocus"; return;
	case MASK_ASTIGMATISM: Name = "Astigmatism"; return;
	case MASK_HO_TOTAL: Name = "HO Total"; return;
	case MASK_COMA: Name = "Coma"; return;
	case MASK_TREFOIL: Name = "Trefoil"; return;
	case MASK_SPHERICAL: Name = "Spherical"; return;
	case MASK_4ORD_ASTIGMATISM: Name = "Secondary Astigmatism"; return;
	case MASK_TOTAL_NO_DEFOCUS: Name = "Total, no Defocus"; return;
	case MASK_NONE: Name = "None"; return;
	}

	Name = "Other";
}

//***************************************************************************************

void CZernikeMask::GetName(CString& Name) const
{
	CZernikeMask::GetName(GetType(), Name);
}

//***************************************************************************************

int CZernikeSurface::EvaluateMode(const int mode1, const int mode2)
{
	real rms1 = 0.0;
	if (0 <= mode1 && mode1 < ZERNIKE_NUM_MODES)
	{
		rms1 = GetCUm(mode1);
	}

	real rms2 = 0.0;
	if (0 <= mode2 && mode2 < ZERNIKE_NUM_MODES)
	{
		rms2 = GetCUm(mode2);
	}

	real rms = hyp(rms1, rms2);

	rms += 0.0001;//

	if (m_r_max_um >= 3000.0)
	{
		if (rms >= 0.400) return COMP_SEVERE;
		if (rms >= 0.325) return COMP_MODERATE;
		if (rms >= 0.250) return COMP_LIGHT;
		return COMP_NORMAL;
	}

	if (m_r_max_um >= 2250.0)
	{
		if (rms >= 0.325) return COMP_SEVERE;
		if (rms >= 0.250) return COMP_MODERATE;
		if (rms >= 0.175) return COMP_LIGHT;
		return COMP_NORMAL;
	}

	if (rms >= 0.250) return COMP_SEVERE;
	if (rms >= 0.175) return COMP_MODERATE;
	if (rms >= 0.100) return COMP_LIGHT;
	return COMP_NORMAL;
}

//***************************************************************************************

void CZernikeSurface::SetSphEq(const real SphEq)
{
	m_c[4] = 0.0;

	AddSphEq(SphEq);
}

//***************************************************************************************