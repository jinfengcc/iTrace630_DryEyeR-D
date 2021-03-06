//***************************************************************************************

#include "StdAfx.h"
#include "Data.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BOOL CMTF::GetAtCdXY(const real_t x_cd, const real_t y_cd, real_t& v)
{
	int nx = m_Array.GetNumCols();
	int ny = m_Array.GetNumRows();
	real_t x = (nx >> 1) + x_cd * m_px_cd;
	real_t y = (ny >> 1) + y_cd * m_px_cd;

	int x0 = (int)x;
	int y0 = (int)y;
	int x1 = ((real_t)x0 == x) ? x0 : x0 + 1;
	int y1 = ((real_t)y0 == y) ? y0 : y0 + 1;

	if (x0 < 0 || x1 >= nx || y0 < 0 || y1 >= ny) return FALSE;

	real_t v00 = m_Array(y0, x0);
	real_t v01 = m_Array(y0, x1);
	real_t v11 = m_Array(y1, x1);
	real_t v10 = m_Array(y1, x0);

	real_t vl = v00 + (v10 - v00) * (y - y0);
	real_t vr = v01 + (v11 - v01) * (y - y0);

	v = vl + (vr - vl) * (x - x0);

	return TRUE;
}

//***************************************************************************************

BOOL CMTF::GetAtCdRA(const real_t r_cd, int a_dg, real_t& v)
{
	return GetAtCdXY(r_cd * COS[a_dg], r_cd * SIN[a_dg], v);
}

//***************************************************************************************

BOOL CMTF::GetAtCdR(const real_t r_cd, real_t& v)
{
	real_t w;
	v = 0.0;

	for (int a_dg = 0; a_dg < 180; a_dg++)
	{
		if (!GetAtCdRA(r_cd, a_dg, w))
		{
			v = 0.0;
			return FALSE;
		}
		v += w;
	}

	v /= 180.0;

	return TRUE;
}

//***************************************************************************************
