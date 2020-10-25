#pragma once

#include "Surface.h"

class CConicSurface : public CSurface
{

public:
  CConicSurface();

  virtual void Reset() override;

  void Create(const real_t r_max_um, const real_t ax0_um, const real_t q);

  real_t GetEccentricity();

  virtual BOOL GetAt(const real_t r_um, const real_t a_rd, real_t *z_um, real_t *dz_dx, real_t *dz_dy, real_t *dz_dr, real_t *dz_da,
                     real_t *rc_um) const override;

  virtual BOOL GetAxUmAt(const real_t r_um, const real_t a_rd, real_t &ax_um) const override;

  real_t m_ax0_um;
  real_t m_q;

private:
  real_t m_a_um;
  real_t m_b_um;
  real_t m_p_um;
};
