//***************************************************************************************

#pragma once

//***************************************************************************************

class complex
{
public:

	complex() { re = 0.0; im = 0.0; }
	complex(const complex& c) { re = c.re; im = c.im; }
	complex(const real_t& r, const real_t& i) { re = r; im = i; }

	real_t re;
	real_t im;

	const complex& operator=(const complex& a);
	const complex& operator+=(const complex& a);
	const complex& operator-=(const complex& a);
	const complex& operator*=(const complex& a);
	const complex& operator*=(const real_t& k);
	const complex& operator/=(const complex& a);
	const complex& operator/=(const real_t& k);
};

//***************************************************************************************

complex operator~(const complex& a);
real_t mod(const complex& a);
real_t arg(const complex& a);
real_t sqr_mod(const complex& a);

//***************************************************************************************

BOOL operator==(const complex& a, const complex& b);
BOOL operator!=(const complex& a, const complex& b);
complex operator+(const complex& a, const complex& b);
complex operator-(const complex& a, const complex& b);
complex operator*(const complex& a, const complex& b);
complex operator*(const complex& a, const real_t& k);
complex operator/(const complex& a, const complex& b);
complex operator/(const complex& a, const real_t& k);

//***************************************************************************************

void FourierTransform(Matrix<complex>& Data, BOOL Forward);

//***************************************************************************************
