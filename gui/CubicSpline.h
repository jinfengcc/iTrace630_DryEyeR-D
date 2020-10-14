//***************************************************************************************

#pragma once

//***************************************************************************************

class CCubicSpline
{
public:

	CCubicSpline() { m_n = 0; }

	BOOL Create(Matrix<real>& x, Matrix<real>& f);

	BOOL GetAt(real x, real& f, BOOL CanExtrapolate = FALSE);

	int m_n;
	Matrix<real> m_x;
	Matrix<real> m_f;
	Matrix<real> m_k;

};

//***************************************************************************************
