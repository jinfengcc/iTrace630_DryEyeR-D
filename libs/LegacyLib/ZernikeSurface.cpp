//***************************************************************************************

#include "stdafx.h"
#include "ZernikeSurface.h"


int CZernikeSurface::m_ord[ZERNIKE_NUM_MODES];
int CZernikeSurface::m_frq[ZERNIKE_NUM_MODES];
real_t CZernikeSurface::m_nrm[ZERNIKE_NUM_MODES];
real_t CZernikeSurface::m_rc[ZERNIKE_NUM_MODES][ZERNIKE_NUM_ORDERS];
//Matrix<real_t> CZernikeSurface::m_cs;
//Matrix<real_t>        CZernikeSurface::m_sc;
static Matrix<real_t> m_cs;
static Matrix<real_t> m_sc;
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
					real_t c = factorial(o - k) /
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

void CZernikeSurface::SaveTxtFile(std::wstring_view fileName)
{
	FILE* pFile = _wfopen(fileName.data(), L"w");
	if (pFile == NULL) return;

	fprintf(pFile, "Normalized Zernike coefficients in microns @ %.2lf mm\n\n", m_r_max_um * 0.002);

	for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
	{
		fprintf(pFile, "%2i\t%3.10lf\n", z, GetCUm(z));
	}

	fclose(pFile);

  ::OpenTxtFile(utf8_convert(fileName).c_str());
}

//***************************************************************************************

BOOL CZernikeSurface::LoadTxtFile(std::wstring_view fileName)
{
  //auto  G_As  = utf8_convert(FileName.GetString());
  FILE *pFile = _wfopen(fileName.data(), L"r");
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

BOOL CZernikeSurface::GetAt(const real_t r_um, const real_t a_rd,
	real_t* w_um, real_t* dw_dx, real_t* dw_dy, real_t* dw_dr, real_t* dw_da, real_t* rc_um) const
{
	real_t r = r_um / m_r_max_um;
	real_t P[ZERNIKE_NUM_ORDERS];
	P[0] = 1.0;

	for (int o = 1; o < ZERNIKE_NUM_ORDERS; o++)
	{
		P[o] = r * P[o - 1];
	}

	real_t S[ZERNIKE_NUM_FREQUENCIES];
	real_t C[ZERNIKE_NUM_FREQUENCIES];

	for (int f = 0; f < ZERNIKE_NUM_FREQUENCIES; f++)
	{
		S[f] = sin(f * a_rd);
		C[f] = cos(f * a_rd);
	}

	real_t w_ = 0.0;
	real_t dw_dr_ = 0.0;
	real_t dw_da_ = 0.0;
	real_t d2w_dr2_ = 0.0;

	for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
	{
		real_t R = 0.0;
		real_t dR_dr = 0.0;
		real_t d2R_dr2 = 0.0;

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

		real_t c = m_c[z];
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

void CZernikeSurface::GetTiltAt(const real_t a_rd, real_t* tilt, real_t* tilt_x, real_t* tilt_y) const
{
	real_t tx = 0.0, ty = 0.0;

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

BOOL CZernikeSurface::CreateXY(const int max_order, const real_t r_max_um, const int num_points, const real_t *r_um, const real_t *a_rd,
                               const real_t *dw_dx, const real_t *dw_dy, real_t *rmse)
{
	Reset();

	int num_modes = (max_order + 1) * (max_order + 2) / 2;

	if (num_modes > ZERNIKE_NUM_MODES) return FALSE;

	m_r_max_um = r_max_um;

	int num_equations = 2 * num_points;
	int num_unknowns = num_modes - 1;

	if (num_points < num_unknowns) return FALSE;

	Matrix<real_t> A(num_equations, num_unknowns);
	A.Fill(0.0);

	Vector<real_t> COS(max_order + 1);
	Vector<real_t> SIN(max_order + 1);
	Vector<real_t> POW(max_order + 1);

	for (int p = 0; p < num_points; p++)
	{
		for (int f = 0; f <= max_order; f++)
		{
			COS[f] = cos(f * a_rd[p]);
			SIN[f] = sin(f * a_rd[p]);
		}

		real_t r = r_um[p] / m_r_max_um;
		POW[0] = 1.0;

		for (int o = 1; o <= max_order; o++)
		{
			POW[o] = r * POW[o - 1];
		}

		for (int z = 1; z < num_modes; z++)
		{
			if (r != 0.0)
			{
				real_t R = 0.0;
				real_t dR_dr = 0.0;

				for (int o = abs(m_frq[z]); o <= m_ord[z]; o += 2)
				{
					R += m_rc[z][o] * POW[o];

					if (o > 0)
					{
						dR_dr += o * m_rc[z][o] * POW[o - 1];
					}
				}

				real_t dZ_dr, dZ_da;
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

	Matrix<real_t> F(num_equations, 1);
	Matrix<real_t> C;

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
			real_t w_um, _dw_dx, _dw_dy;
			GetAt(r_um[p], a_rd[p], &w_um, &_dw_dx, &_dw_dy, NULL, NULL, NULL);
			*rmse += sqr(_dw_dx - dw_dx[p]) + sqr(_dw_dy - dw_dy[p]);
		}

		*rmse = sqrt(*rmse);
	}

	return TRUE;
}

//***************************************************************************************

BOOL CZernikeSurface::CreateR(const int max_order, const real_t r_max_um, const int num_points, const real_t *r_um, const real_t *a_rd,
                              const real_t *dw_dr, real_t *rmse)
{
	Reset();

	int num_modes = (max_order + 1) * (max_order + 2) / 2;

	if (num_modes > ZERNIKE_NUM_MODES) return FALSE;

	m_r_max_um = r_max_um;

	int num_equations = num_points;
	int num_unknowns = num_modes - 1;

	if (num_points < num_unknowns) return FALSE;

	Matrix<real_t> A(num_equations, num_unknowns);
	A.Fill(0.0);

	Vector<real_t> COS(max_order + 1);
	Vector<real_t> SIN(max_order + 1);
	Vector<real_t> POW(max_order + 1);

	for (int p = 0; p < num_points; p++)
	{
		for (int f = 0; f <= max_order; f++)
		{
			COS[f] = cos(f * a_rd[p]);
			SIN[f] = sin(f * a_rd[p]);
		}

		real_t r = r_um[p] / m_r_max_um;
		POW[0] = 1.0;

		for (int o = 1; o <= max_order; o++)
		{
			POW[o] = r * POW[o - 1];
		}

		for (int z = 1; z < num_modes; z++)
		{
			real_t dR_dr = 0.0;
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

	Matrix<real_t> F(num_equations, 1, (real_t*)dw_dr);
	Matrix<real_t> C;

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
			real_t w_um, _dw_dr;
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

void CZernikeSurface::SetCUm(const int z, const real_t c_um)
{
	if (z < 0 || z >= ZERNIKE_NUM_MODES) return;

	m_c[z] = m_nrm[z] * c_um / m_r_max_um;
}

//***************************************************************************************

real_t CZernikeSurface::GetCUm(const int z) const
{
	if (z < 0 || z >= ZERNIKE_NUM_MODES) return 0.0;
	return m_c[z] * m_r_max_um / m_nrm[z];
}

//***************************************************************************************

void CZernikeSurface::ChangeRMaxUm(const real_t r_max_um)
{
	if (r_max_um >= m_r_max_um) return;

	if (r_max_um <= 0.0)
	{
		m_r_max_um = 0.0;
		for (int z = 0; z < ZERNIKE_NUM_MODES; z++) m_c[z] = 0.0;
		return;
	}

	Matrix<real_t> c(ZERNIKE_NUM_MODES, 1, m_c);
	Matrix<real_t> s;

	Multiply(m_cs, c, s);

	real_t r = r_max_um / m_r_max_um;
	Vector<real_t> p(ZERNIKE_NUM_MODES);
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

real_t CZernikeSurface::ComputeRms() const
{
	real_t v = 0.0;

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

void CZernikeSurface::GetCombinedRms(const int mode, real_t& value, int& axis) const
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

void CZernikeSurface::AddSphEq(const real_t SphEq)
{
	m_c[4] -= SphEq * (m_r_max_um * 0.000001) / 4.0;
}

//***************************************************************************************

const char *CZernikeSurface::m_rms_name[ZERNIKE_NUM_MODES] =
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

int CZernikeSurface::EvaluateMode(const int mode1, const int mode2)
{
	real_t rms1 = 0.0;
	if (0 <= mode1 && mode1 < ZERNIKE_NUM_MODES)
	{
		rms1 = GetCUm(mode1);
	}

	real_t rms2 = 0.0;
	if (0 <= mode2 && mode2 < ZERNIKE_NUM_MODES)
	{
		rms2 = GetCUm(mode2);
	}

	real_t rms = hyp(rms1, rms2);

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

void CZernikeSurface::SetSphEq(const real_t SphEq)
{
	m_c[4] = 0.0;

	AddSphEq(SphEq);
}

//***************************************************************************************
