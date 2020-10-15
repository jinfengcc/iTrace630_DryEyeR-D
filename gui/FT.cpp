//***************************************************************************************

#include "StdAfx.h"
#include "FT.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

const complex& complex::operator=(const complex& a)
{
	re = a.re;
	im = a.im;
	return *this;
}

const complex& complex::operator+=(const complex& a)
{
	re += a.re;
	im += a.im;
	return *this;
}

const complex& complex::operator-=(const complex& a)
{
	re -= a.re;
	im -= a.im;
	return *this;
}

const complex& complex::operator*=(const complex& a)
{
	real_t _re = re * a.re - im * a.im;
	real_t _im = re * a.im + im * a.re;
	re = _re;
	im = _im;
	return *this;
}

const complex& complex::operator/=(const complex& a)
{
	real_t d = a.re * a.re + a.im * a.im;
	real_t _re = (re * a.re + im * a.im) / d;
	real_t _im = (im * a.re - re * a.im) / d;
	re = _re;
	im = _im;
	return *this;
}

//***************************************************************************************

const complex& complex::operator*=(const real_t& k)
{
	re *= k;
	im *= k;
	return *this;
}

//***************************************************************************************

const complex& complex::operator/=(const real_t& k)
{
	re /= k;
	im /= k;
	return *this;
}

//***************************************************************************************

complex operator~(const complex& a)
{
	return complex(a.re, -a.im);
}

//***************************************************************************************

real_t mod(const complex& a)
{
	return sqrt(a.re * a.re + a.im * a.im);
}

//***************************************************************************************

real_t arg(const complex& a)
{
	return angle(a.im, a.re);
}

//***************************************************************************************

real_t sqr_mod(const complex& a)
{
	return a.re * a.re + a.im * a.im;
}

//***************************************************************************************

BOOL operator==(const complex& a, const complex& b)
{
	return a.re == b.re && a.im == b.im;
}

//***************************************************************************************

BOOL operator!=(const complex& a, const complex& b)
{
	return a.re != b.re || a.im != b.im;
}

//***************************************************************************************

complex operator+(const complex& a, const complex& b)
{
	return complex(a.re + b.re, a.im + b.im);
}

//***************************************************************************************

complex operator-(const complex& a, const complex& b)
{
	return complex(a.re - b.re, a.im - b.im);
}

//***************************************************************************************

complex operator*(const complex& a, const complex& b)
{
	return complex(a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re);
}

//***************************************************************************************

complex operator/(const complex& a, const complex& b)
{
	real_t d = b.re * b.re + b.im * b.im;
	return complex((a.re * b.re + a.im * b.im) / d, (a.im * b.re - a.re * b.im) / d);
}

//***************************************************************************************

complex operator*(const complex& a, const real_t& k)
{
	return complex(a.re * k, a.im * k);
}

//***************************************************************************************

complex operator/(const complex& a, const real_t& k)
{
	return complex(a.re / k, a.im / k);
}

//***************************************************************************************

void FourierTransform1D(Matrix<complex>& Data, BOOL Forward)
{
	int N = Data.GetSize();

	int logN = log2((uint)N);

	if (logN == -1)
	{
		Matrix<complex> _Data(N);

		complex d(cos(_2_Pi / N), sin(_2_Pi / N));

		complex a(1.0, 0.0);

		for (int i = 0; i < N; i++)
		{
			complex na(1.0, 0.0);

			for (int n = 0; n < N; n++)
			{
				_Data[i] += Data[n] * (Forward ? ~na : na);

				na *= a;
			}

			a *= d;
		}

		if (Forward)
		{
			for (int i = 0; i < N; i++) Data[i] = _Data[i] / (real_t)N;
		}
		else
		{
			for (int i = 0; i < N; i++) Data[i] = _Data[i];
		}
	}

	else
	{
		// bit reversal sorting
		int i, i1, i2;

		for (i1 = 1; i1 < N - 1; i1++)
		{
			i2 = 0;
			for (i = 0; i < logN; i++)
			{
				i2 |= (((i1 & (1 << i)) >> i) << (logN - 1 - i));
			}
			if (i1 < i2)
			{
				Swap(Data[i1], Data[i2]);
			}
		}

		complex v(-1.0, 0.0);

		for (int l = 1; l <= logN; l++)
		{
			int M = 1 << l;
			int M2 = M >> 1;

			complex w(1.0, 0.0);

			for (int sh = 0; sh < M2; sh++)
			{
				for (i1 = sh; i1 < N; i1 += M)
				{
					i2 = i1 + M2;

					complex t(Data[i2] * w);
					Data[i2] = Data[i1] - t;
					Data[i1] = Data[i1] + t;
				}

				w *= v;
			}

			v.im = sqrt((1.0 - v.re) * 0.5);
			v.re = sqrt((1.0 + v.re) * 0.5);
			if (Forward) v.im = -v.im;
		}

		if (Forward)
		{
			for (i = 0; i < N; i++) Data[i] /= (real_t)N;
		}
	}
}

//***************************************************************************************

void FourierTransform(Matrix<complex>& Data, BOOL Forward)
{
	int NumRows = Data.GetNumRows();
	int NumCols = Data.GetNumCols();

	if (NumCols < 1 || NumRows < 1) return;

	if (NumCols == 1)
	{
		FourierTransform1D(Data, Forward);
		return;
	}

	Matrix<complex> DataX(NumCols);
	for (int row = 0; row < NumRows; row++)
	{
		for (int col = 0; col < NumCols; col++)
		{
			DataX[col] = Data(row, col);
		}

		FourierTransform1D(DataX, Forward);

		for (int col = 0; col < NumCols; col++)
		{
			Data(row, col) = DataX[col];
		}
	}

	if (NumRows == 1) return;

	Matrix<complex> DataY(NumRows);
	for (int col = 0; col < NumCols; col++)
	{
		for (int row = 0; row < NumRows; row++)
		{
			DataY[row] = Data(row, col);
		}

		FourierTransform1D(DataY, Forward);

		for (int row = 0; row < NumRows; row++)
		{
			Data(row, col) = DataY[row];
		}
	}
}

//***************************************************************************************
