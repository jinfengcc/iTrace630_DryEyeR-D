//***************************************************************************************

#include "StdAfx.h"
#include "CubicSpline.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BOOL CCubicSpline::Create(Matrix<real>& x, Matrix<real>& f)
{
	m_n = x.GetSize();

	if (m_n < 4)
	{
		m_n = 0;
		return FALSE;
	}

	for (int i = 0; i < m_n - 1; i++)
	{
		for (int j = m_n - 1; j > i; j--)
		{
			if (x[j - 1] > x[j])
			{
				Swap(x[j], x[j - 1]);
				Swap(f[j], f[j - 1]);
			}
		}
	}

	for (int i = 0; i < m_n - 1; i++)
	{
		if (x[i] == x[i + 1])
		{
			m_n = 0;
			return FALSE;
		}
	}

	m_x = x;
	m_f = f;
	m_k.Create(m_n);

	Matrix<real> a(m_n, m_n);
	a.Fill(0.0);
	a(0, 0) = 1.0;

	for (int i = 1; i < m_n - 1; i++)
	{
		a(i, i - 1) = m_x[i + 1] - m_x[i];
		a(i, i) = 2 * (m_x[i + 1] - m_x[i - 1]);
		a(i, i + 1) = m_x[i] - m_x[i - 1];
	}

	a(m_n - 1, m_n - 1) = 1.0;

	Matrix<real> h(m_n);

	h[0] = (-11 * m_f[0] + 18 * m_f[1] - 9 * m_f[2] + 2 * m_f[3]) /
		(2 * (m_x[3] - m_x[0]));

	for (int i = 1; i < m_n - 1; i++)
	{
		h[i] = 3 * (m_f[i + 1] - m_f[i]) * (m_x[i] - m_x[i - 1]) / (m_x[i + 1] - m_x[i]) +
			3 * (m_f[i] - m_f[i - 1]) * (m_x[i + 1] - m_x[i]) / (m_x[i] - m_x[i - 1]);
	}

	h[m_n - 1] = (11 * m_f[m_n - 1] - 18 * m_f[m_n - 2] + 9 * m_f[m_n - 3] - 2 * m_f[m_n - 4]) /
		(2 * (m_x[m_n - 1] - m_x[m_n - 4]));

	if (!LES(a, m_k, h))
	{
		m_n = 0;
		return FALSE;
	}

	return TRUE;
}

//***************************************************************************************

BOOL CCubicSpline::GetAt(real x, real& f, BOOL CanExtrapolate)
{
	if (m_n < 4) return FALSE;

	int i;

	for (i = 0; i < m_n - 1; i++)
	{
		if (m_x[i] <= x && x <= m_x[i + 1]) break;
	}

	if (i == m_n - 1)
	{
		if (CanExtrapolate)
		{
			if (x < m_x[0]) i = 0;
			else i = m_n - 2;
		}
		else
		{
			return FALSE;
		}
	}

	real h = m_x[i + 1] - m_x[i];
	real h2 = h * h;
	real h3 = h2 * h;
	real d1 = m_x[i + 1] - x;
	real d2 = x - m_x[i];

	f = (d1 * d1 * (2.0 * d2 + h) * m_f[i] + d2 * d2 * (2.0 * d1 + h) * m_f[i + 1]) / h3 +
		(d1 * d1 * d2 * m_k[i] - d2 * d2 * d1 * m_k[i + 1]) / h2;

	return TRUE;
}

//***************************************************************************************
