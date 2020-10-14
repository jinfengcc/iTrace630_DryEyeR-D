//***************************************************************************************

#pragma once

//***************************************************************************************

class complex
{
public:

	complex() { re = 0.0; im = 0.0; }
	complex(const complex& c) { re = c.re; im = c.im; }
	complex(const real& r, const real& i) { re = r; im = i; }

	real re;
	real im;

	const complex& operator=(const complex& a);
	const complex& operator+=(const complex& a);
	const complex& operator-=(const complex& a);
	const complex& operator*=(const complex& a);
	const complex& operator*=(const real& k);
	const complex& operator/=(const complex& a);
	const complex& operator/=(const real& k);
};

//***************************************************************************************

complex operator~(const complex& a);
real mod(const complex& a);
real arg(const complex& a);
real sqr_mod(const complex& a);

//***************************************************************************************

BOOL operator==(const complex& a, const complex& b);
BOOL operator!=(const complex& a, const complex& b);
complex operator+(const complex& a, const complex& b);
complex operator-(const complex& a, const complex& b);
complex operator*(const complex& a, const complex& b);
complex operator*(const complex& a, const real& k);
complex operator/(const complex& a, const complex& b);
complex operator/(const complex& a, const real& k);

//***************************************************************************************

void FourierTransform(Matrix<complex>& Data, BOOL Forward);

//***************************************************************************************
