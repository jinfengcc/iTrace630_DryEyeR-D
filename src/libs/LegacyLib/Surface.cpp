//***************************************************************************************

#include "stdafx.h"
#include "Surface.h"

// #include "MaskDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

// GetAxialCurvatureInMicrons - refractive map
BOOL CSurface::GetAxUmAt(const real_t r_um, const real_t a_rd, real_t& ax_um) const
{
	real_t dz_dr;

	if (!GetAt(r_um, a_rd, NULL, NULL, NULL, &dz_dr, NULL, NULL)) return FALSE;
	if (dz_dr == 0.0) return FALSE;

	ax_um = r_um * sqrt(1.0 + sqr(dz_dr)) / dz_dr;

	return TRUE;
}

//***************************************************************************************

// 
BOOL CSurface::GetTnUmAt(const real_t r_um, const real_t a_rd, real_t& tn_um) const
{
	if (!GetAt(r_um, a_rd, NULL, NULL, NULL, NULL, NULL, &tn_um)) return FALSE;

	if (tn_um == DBL_MAX) return FALSE;

	return TRUE;
}

//***************************************************************************************

BOOL CSurface::GetRfUmAt(const real_t r_um, const real_t a_rd, real_t& rf_um) const
{
	real_t z_um, dz_dr;
	if (!GetAt(r_um, a_rd, &z_um, NULL, NULL, &dz_dr, NULL, NULL)) return FALSE;

	if (dz_dr == 0.0) return FALSE;

	real_t tan_a = dz_dr;
	real_t sin_a = tan_a / sqrt(1.0 + sqr(tan_a));
	real_t cos_a = sqrt(1.0 - sqr(sin_a));

	real_t sin_g = sin_a / 1.3375; // Snell's law //cjf note : is 1.3375 be the corneal refractive index?  
	real_t cos_g = sqrt(1.0 - sqr(sin_g));

	real_t sin_b = sin_a * cos_g - cos_a * sin_g; // b = a - g
	real_t cos_b = sqrt(1.0 - sqr(sin_b));
	real_t cot_b = cos_b / sin_b;

	rf_um = z_um + r_um * cot_b;

	return TRUE;
}

