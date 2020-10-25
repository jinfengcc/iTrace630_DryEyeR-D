//***************************************************************************************

#pragma once

#include "Structures.h"

//***************************************************************************************

class CCubicSpline
{
public:

	CCubicSpline() { m_n = 0; }

	BOOL Create(Matrix<real_t>& x, Matrix<real_t>& f);

	BOOL GetAt(real_t x, real_t& f, BOOL CanExtrapolate = FALSE);

	int m_n;
	Matrix<real_t> m_x;
	Matrix<real_t> m_f;
	Matrix<real_t> m_k;

};

//***************************************************************************************
