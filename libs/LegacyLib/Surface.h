//***************************************************************************************

#pragma once

#include <atlbase.h>

#include <libs/CommonLib/types.h>
#include <libs/LegacyLib/Utils.h>

class CSurface
{
public:
  // ASKQ: Hide (max radius in um)
  real_t m_r_max_um;

  virtual void Reset()
  {
    m_r_max_um = 0.0;
  }

  struct PointPolar
  {
    real_t r;
    real_t theta;
  };

  struct SurfData
  {
    real_t z_um;  // elevation z
    real_t dz_dx; // delta(z)/delta(x)
    real_t dz_dy; // delta(z)/delta(x)
    real_t dz_dr; // delta(z)/delta(x)
    real_t dz_da;
    real_t rc_um; // ?
  };

  //virtual SurfData GetAt(struct PointPolar pp) const = 0;

  virtual BOOL GetAt(const real_t r_um, const real_t a_rd, real_t *z_um, real_t *dz_dx, real_t *dz_dy, real_t *dz_dr, real_t *dz_da,
                     real_t *rc_um) const = 0;
  //{
  //  return FALSE;
  //}

  // ASKQ: Use more meaningful names
  virtual BOOL GetAxUmAt(const real_t r_um, const real_t a_rd, real_t &ax_um) const;
  virtual BOOL GetTnUmAt(const real_t r_um, const real_t a_rd, real_t &tn_um) const;
  virtual BOOL GetRfUmAt(const real_t r_um, const real_t a_rd, real_t &rf_um) const;
};
