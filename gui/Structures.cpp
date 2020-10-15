//***************************************************************************************

#include "StdAfx.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

void Transpose(const Matrix<real_t>& M, Matrix<real_t>& MT)
{
	MT.Create(M.GetNumCols(), M.GetNumRows());

	for (int r = 0; r < M.GetNumRows(); r++)
	{
		for (int c = 0; c < M.GetNumCols(); c++)
		{
			MT(c, r) = M(r, c);
		}
	}
}

//***************************************************************************************

BOOL Multiply(const Matrix<real_t>& M1, const Matrix<real_t>& M2, Matrix<real_t>& M)
{
	if (M1.GetNumCols() != M2.GetNumRows()) return FALSE;

	M.Create(M1.GetNumRows(), M2.GetNumCols());

	for (int r = 0; r < M1.GetNumRows(); r++)
	{
		for (int c = 0; c < M2.GetNumCols(); c++)
		{
			real_t v = 0.0;
			for (int k = 0; k < M1.GetNumCols(); k++) v += M1(r, k) * M2(k, c);
			M(r, c) = v;
		}
	}

	return TRUE;
}

//***************************************************************************************

BOOL Invert(const Matrix<real_t>& M, Matrix<real_t>& MI)
{
	int NumRows = M.GetNumRows();
	int NumCols = M.GetNumCols();

	if (NumRows != NumCols) return FALSE;

	Matrix<real_t> ME(NumRows, 2 * NumCols);

	int r, c, k;

	for (r = 0; r < NumRows; r++)
	{
		for (c = 0; c < NumCols; c++) ME(r, c) = M(r, c);
		for (; c < 2 * NumCols; c++) ME(r, c) = 0.0;
		ME(r, r + NumCols) = 1.0;
	}

	for (r = 0; r < NumRows; r++)
	{
		if (ME(r, r) == 0.0)
		{
			// find row k with r element != 0
			for (k = r + 1; k < NumRows; k++)
			{
				if (ME(k, r) != 0.0) break;
			}

			if (k == NumRows)
			{
				return FALSE;
			}

			// swap NumRows r and k
			for (c = r; c < 2 * NumCols; c++)
			{
				Swap(ME(r, c), ME(k, c));
			}
		}

		real_t t = ME(r, r);
		for (c = r; c < 2 * NumCols; c++)
		{
			ME(r, c) /= t;
		}

		for (k = 0; k < NumRows; k++)
		{
			if (k == r) continue; // skip r row
			real_t t = ME(k, r);
			for (c = r; c < 2 * NumCols; c++) {
				ME(k, c) -= t * ME(r, c);
			}
		}
	}

	MI.Create(NumRows, NumCols);

	for (r = 0; r < NumRows; r++)
	{
		for (c = 0; c < NumCols; c++)
		{
			MI(r, c) = ME(r, c + NumCols);
		}
	}

	return TRUE;
}

//***************************************************************************************

BOOL LSM(const Matrix<real_t>& A, Matrix<real_t>& X, const Matrix<real_t>& F)
{
	Matrix<real_t> AT;

	Transpose(A, AT);

	Matrix<real_t> AT_A;

	Multiply(AT, A, AT_A);

	Matrix<real_t> AT_A_I;

	if (!Invert(AT_A, AT_A_I)) return FALSE;

	Matrix<real_t> AT_A_I_AT;

	Multiply(AT_A_I, AT, AT_A_I_AT);

	Multiply(AT_A_I_AT, F, X);

	return TRUE;
}

//***************************************************************************************

BOOL LES(const Matrix<real_t>& A, Matrix<real_t>& X, const Matrix<real_t>& F)
{
	Matrix<real_t> AI;

	if (!Invert(A, AI)) return FALSE;

	Multiply(AI, F, X);

	return TRUE;
}

//***************************************************************************************

void Zoom(const Matrix<real_t>& A, Matrix<real_t>& B, const real_t k)
{
	int NumRows = A.GetNumRows();
	int NumCols = A.GetNumCols();
	int cx = NumCols >> 1;
	int cy = NumRows >> 1;

	B.Create(NumRows, NumCols);

	for (int by = 0; by < NumRows; by++)
	{
		real_t ay = cy + (by - cy) / k;
		for (int bx = 0; bx < NumCols; bx++)
		{
			real_t ax = cx + (bx - cx) / k;
			int ax0 = (int)ax;
			int ay0 = (int)ay;
			int ax1 = ((real_t)ax0 == ax) ? ax0 : ax0 + 1;
			int ay1 = ((real_t)ay0 == ay) ? ay0 : ay0 + 1;
			if (ax0 < 0 || ax1 >= NumCols || ay0 < 0 || ay1 >= NumRows)
			{
				B(by, bx) = 0.0;
			}
			else
			{
				real_t v00 = A(ay0, ax0);
				real_t v01 = A(ay0, ax1);
				real_t v11 = A(ay1, ax1);
				real_t v10 = A(ay1, ax0);
				real_t vl = v00 + (v10 - v00) * (ay - ay0);
				real_t vr = v01 + (v11 - v01) * (ay - ay0);
				B(by, bx) = vl + (vr - vl) * (ax - ax0);
			}
		}
	}
}

//***************************************************************************************

void Normalize(Matrix<real_t>& A, const real_t Min, const real_t Max)
{
	real_t mn = DBL_MAX;
	real_t mx = -DBL_MAX;

	int n = A.GetNumCols() * A.GetNumRows();

	for (int i = 0; i < n; i++)
	{
		real_t v = A[i];
		if (mn > v) mn = v;
		if (mx < v) mx = v;
	}

	real_t d = mx - mn;
	real_t k = d == 0.0 ? 0.0 : (Max - Min) / d;

	for (int i = 0; i < n; i++)
	{
		A[i] = Min + k * (A[i] - mn);
	}
}

//***************************************************************************************

void Angles(const Matrix<real_t>& A, Matrix<real_t>& B)
{
	Matrix<real_t> AT;

	Transpose(A, AT);

	Matrix<real_t> AT_A;

	Multiply(AT, A, AT_A);

	int num_unknowns = A.GetNumCols();

	B.Create(num_unknowns, num_unknowns);
	B.Fill(INVALID_VALUE);

	for (int j = 0; j < num_unknowns; j++)
	{
		for (int i = 0; i < num_unknowns; i++)
		{
			real_t z = sqrt(AT_A(i, i) * AT_A(j, j));

			if (z != 0.0)
			{
				real_t b = _180_Pi * acos(AT_A(j, i) / z);
				B(j, i) = b > 90.0 ? 180.0 - b : b;
			}
		}
	}
}

//***************************************************************************************
