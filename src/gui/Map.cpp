//***************************************************************************************

#include "StdAfx.h"
#include "Data.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

CMap2D::CMap2D()
{
	m_r_max_um = 0.0;

	m_min = 0.0;
	m_max = 0.0;

	m_dr_um = 100.0;

	m_nr = 0;
	m_data.Create(1, 1);
	m_data.Fill(INVALID_VALUE);
}

//***************************************************************************************

void CMap2D::Create(const real_t r_max_um, const real_t dr_um)
{
	m_r_max_um = __min(r_max_um, 5000.0);

	m_min = DBL_MAX;
	m_max = -DBL_MAX;

	m_dr_um = dr_um;

	m_nr = (int)(m_r_max_um / m_dr_um);
	m_data.Create(m_nr + 1 + m_nr, m_nr + 1 + m_nr);
	m_data.Fill(INVALID_VALUE);
}

//***************************************************************************************

void CMap2D::SetAt(const int y, const int x, const real_t v)
{
	if (y < -m_nr || y > m_nr || x < -m_nr || x > m_nr) return;

	m_data(m_nr + y, m_nr + x) = v;

	if (v != INVALID_VALUE)
	{
		if (m_min > v) m_min = v;
		if (m_max < v) m_max = v;
	}
}

//***************************************************************************************

real_t CMap2D::GetAt(const int y, const int x) const
{
	if (y < -m_nr || y > m_nr || x < -m_nr || x > m_nr) return INVALID_VALUE;

	return m_data(m_nr + y, m_nr + x);
}

//***************************************************************************************

void CMap2D::Subtract(const CMap2D& Map1, const CMap2D& Map2)
{
	if (fabs(Map1.m_dr_um - Map2.m_dr_um) > 0.000000001) return;

	Create(__min(Map1.m_r_max_um, Map2.m_r_max_um), Map1.m_dr_um);

	for (int y = -m_nr; y <= m_nr; y++)
	{
		for (int x = -m_nr; x <= m_nr; x++)
		{
			real_t v1 = Map1.GetAt(y, x);
			real_t v2 = Map2.GetAt(y, x);

			if (v1 != INVALID_VALUE && v2 != INVALID_VALUE)
			{
				real_t v = v1 - v2;
				SetAt(y, x, v);
				if (m_min > v) m_min = v;
				if (m_max < v) m_max = v;
			}
		}
	}
}

//***************************************************************************************

void CMap2D::ComputeMeanStdDev(real_t& mean, real_t& std_dev) const
{
	mean = 0.0;
	std_dev = 0.0;

	int k = 0;
	for (int y = -m_nr; y <= m_nr; y++)
	{
		for (int x = -m_nr; x <= m_nr; x++)
		{
			real_t v = GetAt(y, x);
			if (v != INVALID_VALUE)
			{
				mean += v;
				k++;
			}
		}
	}

	if (k == 0) return;

	mean /= k;

	for (int y = -m_nr; y <= m_nr; y++)
	{
		for (int x = -m_nr; x <= m_nr; x++)
		{
			real_t v = GetAt(y, x);
			if (v != INVALID_VALUE)
			{
				std_dev += (v - mean) * (v - mean);
			}
		}
	}

	std_dev = sqrt(std_dev / k);
}

//***************************************************************************************

CMap3D::CMap3D()
{
	m_r_max_um = 0.0;

	m_min = 0.0;
	m_max = 0.0;

	m_dr_um = 100.0;

	m_nr = 0;
	m_data.Create(1, 360);
	m_data.Fill(INVALID_VALUE);
}

//***************************************************************************************

void CMap3D::Create(const real_t r_max_um, const real_t dr_um)
{
	m_r_max_um = __min(r_max_um, 5000.0);

	m_min = DBL_MAX;
	m_max = -DBL_MAX;

	m_dr_um = dr_um;

	m_nr = (int)(m_r_max_um / m_dr_um);
	m_data.Create(m_nr + 1, 360);
	m_data.Fill(INVALID_VALUE);
}

//***************************************************************************************

void CMap3D::SetAt(const int r, const int a, const real_t v)
{
	ASSERT(r >= 0 && r <= m_nr && a >= 0 && a <= 359);
	if (r < 0 || r > m_nr || a < 0 || a > 359) return;

	m_data(r, a) = v;

	if (v != INVALID_VALUE)
	{
		if (m_min > v) m_min = v;
		if (m_max < v) m_max = v;
	}
}

//***************************************************************************************

real_t CMap3D::GetAt(const int r, const int a) const
{
	ASSERT(r >= 0 && r <= m_nr && a >= 0 && a <= 359);
	if (r < 0 || r > m_nr || a < 0 || a > 359) return INVALID_VALUE;

	return m_data(r, a);
}

//***************************************************************************************

void CMap3D::Subtract(const CMap3D& Map1, const CMap3D& Map2)
{
	if (fabs(Map1.m_dr_um - Map2.m_dr_um) > 0.000000001) return;

	Create(__min(Map1.m_r_max_um, Map2.m_r_max_um), Map1.m_dr_um);

	for (int r = 0; r <= m_nr; r++)
	{
		for (int a = 0; a < 360; a++)
		{
			real_t v1 = Map1.GetAt(r, a);
			real_t v2 = Map2.GetAt(r, a);

			if (v1 != INVALID_VALUE && v2 != INVALID_VALUE)
			{
				real_t v = v1 - v2;
				SetAt(r, a, v);
				if (m_min > v) m_min = v;
				if (m_max < v) m_max = v;
			}
		}
	}
}

//***************************************************************************************
