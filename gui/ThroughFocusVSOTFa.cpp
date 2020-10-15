//***************************************************************************************

#include "StdAfx.h"
#include "ThroughFocusVSOTFa.h"
#include "FT.h"//531

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

const real_t l_um = 0.555;

//***************************************************************************************

void CreateNeuralCSF(Matrix<real_t>& CSF_Array, const int n, const real_t cd_px)
{
	static BOOL Init = FALSE;

	const int p = 6;

	static Matrix<real_t> C;

	if (!Init)
	{
		const int m = 53;

		real_t x[m] =
		{
			3.9355,  4.9677,  6.0000,  6.9677,  8.0000,  8.9677, 10.0000, 10.9032, 12.0000, 12.9677,
			14.9677, 16.0000, 16.9677, 18.0000, 18.9677, 20.0000, 20.9677, 22.0000, 22.9677, 24.0000,
			24.9677, 26.0000, 28.0000, 28.9677, 30.0000, 30.9677, 31.9355, 32.9677, 34.0000, 34.9677,
			36.0000, 36.9677, 38.0000, 40.0000, 40.9677, 42.0000, 42.9677, 44.0000, 44.9677, 46.0000,
			46.9677, 48.0000, 48.9677, 50.0000, 50.9677, 52.9032, 53.9355, 54.9677, 56.0000, 56.9032,
			57.9355, 58.9677, 60.0000
		};

		real_t f[m] =
		{
			2.1999, 2.2476, 2.2988, 2.3261, 2.3431, 2.3431, 2.3261, 2.3090, 2.2885, 2.2544,
			2.2101, 2.1658, 2.1282, 2.0907, 2.0566, 2.0123, 1.9645, 1.9304, 1.8929, 1.8588,
			1.8213, 1.7769, 1.7428, 1.6951, 1.6610, 1.6269, 1.5859, 1.5518, 1.5075, 1.4598,
			1.4291, 1.3915, 1.3472, 1.2995, 1.2551, 1.2074, 1.1630, 1.1187, 1.0744, 1.0095,
			0.9652, 0.9106, 0.8492, 0.7947, 0.7231, 0.6685, 0.5969, 0.5423, 0.4707, 0.3990,
			0.3445, 0.2660, 0.1842
		};

		Matrix<real_t> A(m, p);
		Matrix<real_t> F(m, 1);

		for (int j = 0; j < m; j++)
		{
			A(j, 0) = 1;
			for (int i = 1; i < p; i++)
			{
				A(j, i) = A(j, i - 1) * x[j];
			}
			F(j, 0) = f[j];
		}

		::LSM(A, C, F);

		Init = TRUE;
	}

	ASSERT((n & 1) == 0);

	CSF_Array.Create(n, n);
	CSF_Array.Fill(0.0);

	int o = n >> 1;
	for (int j = 0; j < o; j++)
	{
		for (int i = 0; i < o; i++)
		{
			real_t r_px = hyp(j, i);
			real_t r_cd = r_px * cd_px;

			if (r_cd <= 60.0)
			{
				real_t u = 1.0;
				real_t v = C[0];

				for (int k = 1; k < p; k++)
				{
					u *= r_cd;
					v += C[k] * u;
				}

				v = pow(10, v);
				CSF_Array(j, i) = v;

				if (j > 0)
				{
					CSF_Array(n - j, i) = v;
				}

				if (i > 0)
				{
					CSF_Array(j, n - i) = v;
				}

				if (j > 0 && i > 0)
				{
					CSF_Array(n - j, n - i) = v;
				}
			}
		}
	}
}

//***************************************************************************************

void CreateOTF(const CZernikeSurface& Surface, Matrix<complex>& Array, const int n)
{
	Array.Create(n, n);

	// Pupil function
	const real_t t_um = 4.0 * Surface.m_r_max_um;
	const real_t pf_um_px = t_um / n;
	const int o = n >> 1;

	for (int y = 0; y < n; y++)
	{
		real_t y_um = (y - o) * pf_um_px;

		for (int x = 0; x < n; x++)
		{
			real_t x_um = (x - o) * pf_um_px;
			real_t r_um = hyp(y_um, x_um);
			if (r_um <= Surface.m_r_max_um)
			{
				real_t a_rd = angle(y_um, x_um);
				real_t w_um = 0.0;
				Surface.GetAt(r_um, a_rd, &w_um, NULL, NULL, NULL, NULL, NULL);
				real_t w = _2_Pi * w_um / l_um;
				real_t a = exp(-0.120 * sqr(0.001 * r_um));
				Array(y, x) = complex(a * cos(w), -a * sin(w));
			}
		}
	}

	// PSF
	FourierTransform(Array, TRUE);

	for (int y = 0; y < n; y++)
	{
		for (int x = 0; x < n; x++)
		{
			Array(y, x).re = sqr_mod(Array(y, x));
			Array(y, x).im = 0.0;
		}
	}

	// OTF
	FourierTransform(Array, TRUE);
}

//***************************************************************************************

real_t CalcVSOTFA(const CZernikeSurface& Surface, const int n)
{
	const real_t t_um = 4.0 * Surface.m_r_max_um;
	const real_t psf_px_mn = t_um / (60.0 * _180_Pi * l_um);
	const real_t otf_cd_px = psf_px_mn * 60.0 / n;

	// No aberrations OTF
	CZernikeSurface Surface_DL;
	Surface_DL.m_r_max_um = Surface.m_r_max_um;
	Matrix<complex> OTF_DL_Array;
	CreateOTF(Surface_DL, OTF_DL_Array, n);

	// OTF
	Matrix<complex> OTF_Array;
	CreateOTF(Surface, OTF_Array, n);

	// Neural CSF
	Matrix<real_t> CSF_Array;
	CreateNeuralCSF(CSF_Array, n, otf_cd_px);

	// Integration
	real_t S1 = 0.0;
	real_t S2 = 0.0;
	for (int y = 0; y < n; y++)
	{
		for (int x = 0; x < n; x++)
		{
			S1 += CSF_Array(y, x) * fabs(OTF_Array(y, x).re);
			S2 += CSF_Array(y, x) * fabs(OTF_DL_Array(y, x).re);
		}
	}

	return S2 != 0.0 ? S1 / S2 : 1.0;
}

//***************************************************************************************

void CreateThroughFocusVSOTFA(const CZernikeSurface& Surface, Matrix<real_t>& X, Matrix<real_t>& F)
{
	const int n = 512;

	CZernikeSurface Surface1;

	Surface1 = Surface;

	Surface1.SetCUm(0, 0.0);
	Surface1.SetCUm(1, 0.0);
	Surface1.SetCUm(2, 0.0);
	Surface1.SetCUm(3, 0.0);
	Surface1.SetCUm(4, 0.0);
	Surface1.SetCUm(5, 0.0);

	const int m = 12;
	X.Create(m + 1 + m, 1);
	F.Create(m + 1 + m, 1);
	for (int i = -m; i <= m; i++)
	{
		real_t SphEqDpt = i * 0.25;
		Surface1.SetSphEq(SphEqDpt);
		X[i + m] = SphEqDpt;
		F[i + m] = CalcVSOTFA(Surface1, n);
	}
}

//***************************************************************************************
