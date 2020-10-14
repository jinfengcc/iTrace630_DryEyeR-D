//***************************************************************************************

#include "StdAfx.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

void Transpose(const Matrix<real>& M, Matrix<real>& MT)
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

BOOL Multiply(const Matrix<real>& M1, const Matrix<real>& M2, Matrix<real>& M)
{
	if (M1.GetNumCols() != M2.GetNumRows()) return FALSE;

	M.Create(M1.GetNumRows(), M2.GetNumCols());

	for (int r = 0; r < M1.GetNumRows(); r++)
	{
		for (int c = 0; c < M2.GetNumCols(); c++)
		{
			real v = 0.0;
			for (int k = 0; k < M1.GetNumCols(); k++) v += M1(r, k) * M2(k, c);
			M(r, c) = v;
		}
	}

	return TRUE;
}

//***************************************************************************************

BOOL Invert(const Matrix<real>& M, Matrix<real>& MI)
{
	int NumRows = M.GetNumRows();
	int NumCols = M.GetNumCols();

	if (NumRows != NumCols) return FALSE;

	Matrix<real> ME(NumRows, 2 * NumCols);

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

		real t = ME(r, r);
		for (c = r; c < 2 * NumCols; c++)
		{
			ME(r, c) /= t;
		}

		for (k = 0; k < NumRows; k++)
		{
			if (k == r) continue; // skip r row
			real t = ME(k, r);
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

BOOL LSM(const Matrix<real>& A, Matrix<real>& X, const Matrix<real>& F)
{
	Matrix<real> AT;

	Transpose(A, AT);

	Matrix<real> AT_A;

	Multiply(AT, A, AT_A);

	Matrix<real> AT_A_I;

	if (!Invert(AT_A, AT_A_I)) return FALSE;

	Matrix<real> AT_A_I_AT;

	Multiply(AT_A_I, AT, AT_A_I_AT);

	Multiply(AT_A_I_AT, F, X);

	return TRUE;
}

//***************************************************************************************

BOOL LES(const Matrix<real>& A, Matrix<real>& X, const Matrix<real>& F)
{
	Matrix<real> AI;

	if (!Invert(A, AI)) return FALSE;

	Multiply(AI, F, X);

	return TRUE;
}

//***************************************************************************************

void Zoom(const Matrix<real>& A, Matrix<real>& B, const real k)
{
	int NumRows = A.GetNumRows();
	int NumCols = A.GetNumCols();
	int cx = NumCols >> 1;
	int cy = NumRows >> 1;

	B.Create(NumRows, NumCols);

	for (int by = 0; by < NumRows; by++)
	{
		real ay = cy + (by - cy) / k;
		for (int bx = 0; bx < NumCols; bx++)
		{
			real ax = cx + (bx - cx) / k;
			int ax0 = (int)ax;
			int ay0 = (int)ay;
			int ax1 = ((real)ax0 == ax) ? ax0 : ax0 + 1;
			int ay1 = ((real)ay0 == ay) ? ay0 : ay0 + 1;
			if (ax0 < 0 || ax1 >= NumCols || ay0 < 0 || ay1 >= NumRows)
			{
				B(by, bx) = 0.0;
			}
			else
			{
				real v00 = A(ay0, ax0);
				real v01 = A(ay0, ax1);
				real v11 = A(ay1, ax1);
				real v10 = A(ay1, ax0);
				real vl = v00 + (v10 - v00) * (ay - ay0);
				real vr = v01 + (v11 - v01) * (ay - ay0);
				B(by, bx) = vl + (vr - vl) * (ax - ax0);
			}
		}
	}
}

//***************************************************************************************

void Normalize(Matrix<real>& A, const real Min, const real Max)
{
	real mn = DBL_MAX;
	real mx = -DBL_MAX;

	int n = A.GetNumCols() * A.GetNumRows();

	for (int i = 0; i < n; i++)
	{
		real v = A[i];
		if (mn > v) mn = v;
		if (mx < v) mx = v;
	}

	real d = mx - mn;
	real k = d == 0.0 ? 0.0 : (Max - Min) / d;

	for (int i = 0; i < n; i++)
	{
		A[i] = Min + k * (A[i] - mn);
	}
}

//***************************************************************************************

void Angles(const Matrix<real>& A, Matrix<real>& B)
{
	Matrix<real> AT;

	Transpose(A, AT);

	Matrix<real> AT_A;

	Multiply(AT, A, AT_A);

	int num_unknowns = A.GetNumCols();

	B.Create(num_unknowns, num_unknowns);
	B.Fill(INVALID_VALUE);

	for (int j = 0; j < num_unknowns; j++)
	{
		for (int i = 0; i < num_unknowns; i++)
		{
			real z = sqrt(AT_A(i, i) * AT_A(j, j));

			if (z != 0.0)
			{
				real b = _180_Pi * acos(AT_A(j, i) / z);
				B(j, i) = b > 90.0 ? 180.0 - b : b;
			}
		}
	}
}

//***************************************************************************************
