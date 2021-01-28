#include "stdafx.h"
#include "ConicSurface.h"

CConicSurface::CConicSurface()
{
  Reset();
}

//***************************************************************************************

void CConicSurface::Reset()
{
  CSurface::Reset();

  m_ax0_um = 0.0;
  m_q      = 0.0;

  m_a_um = 0.0;
  m_b_um = 0.0;
  m_p_um = 0.0;
}

//***************************************************************************************

void CConicSurface::Create(const real_t r_max_um, const real_t ax0_um, const real_t q)
{
  m_r_max_um = r_max_um;

  m_ax0_um = ax0_um;
  m_q      = q;

  // hyperbola
  if (m_q < -1.0) {
    m_a_um = m_ax0_um / (-1.0 - m_q);
    m_b_um = m_ax0_um / sqrt(-1.0 - m_q);
    m_p_um = 0.0;
  }
  // parabola
  else if (m_q == -1.0) {
    m_a_um = 0.0;
    m_b_um = 0.0;
    m_p_um = m_ax0_um;
  }
  // prolate ellipse, circle, oblate ellipse
  else {
    m_a_um = m_ax0_um / (1.0 + m_q);
    m_b_um = m_ax0_um / sqrt(1.0 + m_q);
    m_p_um = 0.0;

    if (m_r_max_um > m_b_um)
      m_r_max_um = m_b_um;
  }
}

//***************************************************************************************

BOOL CConicSurface::GetAt(const real_t r_um, const real_t a_rd, real_t *z_um, real_t *dz_dx, real_t *dz_dy, real_t *dz_dr, real_t *dz_da,
                          real_t *rc_um) const
{
  if (r_um > m_r_max_um)
    return FALSE;

  if (z_um) {
    *z_um = 0.0;
    // hyperbola
    if (m_q < -1.0) {
      *z_um = m_a_um * (sqrt(1.0 + sqr(r_um / m_b_um)) - 1.0);
    }
    // parabola
    else if (m_q == -1.0) {
      *z_um = sqr(r_um) / (2.0 * m_p_um);
    }
    // prolate ellipse, circle, oblate ellipse
    else {
      *z_um = m_a_um * (1.0 - sqrt(1.0 - sqr(r_um / m_b_um)));
    }
  }

  //
  real_t _dz_dr = 0.0;
  if (dz_dr || dz_dx || dz_dy) {
    // hyperbola
    if (m_q < -1.0) {
      _dz_dr = (m_a_um * r_um) / (m_b_um * sqrt(sqr(m_b_um) + sqr(r_um)));
    }
    // parabola
    else if (m_q == -1.0) {
      _dz_dr = r_um / m_p_um;
    }
    // prolate ellipse, circle, oblate ellipse
    else {
      _dz_dr = (m_a_um * r_um) / (m_b_um * sqrt(sqr(m_b_um) - sqr(r_um)));
    }
  }

  if (dz_dr)
    *dz_dr = _dz_dr;
  if (dz_da)
    *dz_da = 0.0;
  if (dz_dx)
    *dz_dx = _dz_dr * cos(a_rd);
  if (dz_dy)
    *dz_dy = _dz_dr * sin(a_rd);

  //
  if (rc_um) {
    // hyperbola
    if (m_q < -1.0) {
      real_t b2 = sqr(m_b_um);
      real_t b4 = sqr(b2);
      real_t a2 = sqr(m_a_um);
      real_t r2 = sqr(r_um);
      *rc_um  = sqrt(cube(b4 + r2 * (a2 + b2))) / (m_a_um * b4);
    }
    // parabola
    else if (m_q == -1.0) {
      real_t p2 = sqr(m_p_um);
      real_t r2 = sqr(r_um);
      *rc_um  = sqrt(cube(p2 + r2)) / p2;
    }
    // prolate ellipse, circle, oblate ellipse
    else {
      real_t b2 = sqr(m_b_um);
      real_t b4 = sqr(b2);
      real_t a2 = sqr(m_a_um);
      real_t r2 = sqr(r_um);
      *rc_um  = sqrt(cube(b4 + r2 * (a2 - b2))) / (m_a_um * b4);
    }
  }

  return TRUE;
}

//***************************************************************************************

real_t CConicSurface::GetEccentricity()
{
  // hyperbola, parabola, prolate ellipse, circle
  if (m_q <= 0)
    return sqrt(-m_q);
  // oblate ellipse
  return sqrt(m_q / (1.0 + m_q));
}

//***************************************************************************************

BOOL CConicSurface::GetAxUmAt(const real_t r_um, const real_t a_rd, real_t &ax_um) const
{
  if (r_um > m_r_max_um)
    return FALSE;

  ax_um = sqrt(sqr(m_ax0_um) - m_q * sqr(r_um));

  return TRUE;
}
