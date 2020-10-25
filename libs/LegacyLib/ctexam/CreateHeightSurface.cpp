#include "stdafx.h"
#if 1
#include "ZernikeSurface.h"

#define TWEAKABLE

TWEAKABLE const double CT_R_MAX_UM = 5000.0;

#define ZERNIKE_ORDER_MAX       8
#define ZERNIKE_NUM_ORDERS      9
#define ZERNIKE_NUM_FREQUENCIES 9
#define ZERNIKE_NUM_MODES       45

inline auto sqr(double x)
{
  return x * x;
}

bool CreateHeightSurface(const cv::Mat_<double> &rings, cv::Mat_<double> m_ax_um, CZernikeSurface &m_HtZrSurface)
{
  BOOL Res = FALSE;

  real_t *_r_um  = new real_t[rings.rows * rings.cols];
  real_t *_a_rd  = new real_t[rings.rows * rings.cols];
  real_t *_dh_dr = new real_t[rings.rows * rings.cols];
  int   n      = 0;

  Expects(rings.cols == 360);

  for (int r = 0; r < rings.rows; r++) {
    for (int a = 0; a < 360; a++) {
      real_t r_um = rings(r, a);
      if (r_um != INVALID_VALUE && r_um <= CT_R_MAX_UM) {
        real_t t = sqr(m_ax_um[r][a]) - sqr(r_um);
        if (t > 0.0) {
          _r_um[n]      = r_um;
          _a_rd[n]      = a * _Pi_180;
          //m_dh_dr[r][a] = (_dh_dr[n] = r_um / sqrt(t));
          n++;
        }
      }
    }
  }

  const auto m_r_max_um = CT_R_MAX_UM;

  if (m_HtZrSurface.CreateR(ZERNIKE_ORDER_MAX, m_r_max_um, n, _r_um, _a_rd, _dh_dr, nullptr)) {
    m_HtZrSurface.ComputePiston(TRUE);
    m_HtZrSurface.ComputeTilt(TRUE);
    Res = TRUE;
  }

  delete[] _r_um;
  delete[] _a_rd;
  delete[] _dh_dr;

  return Res;
}
#endif
