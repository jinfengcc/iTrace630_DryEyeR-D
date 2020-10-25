#include "stdafx.h"
#include "interfaces/data/Calibration.h"
#include "findax.h"

cv::Mat FindAx(cv::Mat_<double> rings, CALIBRATION_V1 &cal)
{
  Expects(rings.rows == MAX_NUM_RINGS);
  Expects(rings.cols == 360);

  CCubicSpline Spline;
  Matrix<real_t> r_um(4);
  Matrix<real_t> ax_um(4, 1, cal.BallsAx);

  cv::Mat_<double> m_ax_um(MAX_NUM_RINGS, 360);
  m_ax_um.setTo(INVALID_VALUE);

  for (int r = 0; r < rings.rows; r++) {
    for (int a = 0; a < rings.cols; a++) {
      double res = INVALID_VALUE;
      if (rings(r, a) != INVALID_VALUE) {
        for (int b = 0; b < 4; b++) {
          r_um[b] = cal.BallsRi[b][r][a];
        }
        if (Spline.Create(r_um, ax_um)) {
          Spline.GetAt(rings(r, a), res, TRUE);
        }
      }
      m_ax_um(r, a) = res;
    }
  }

  return m_ax_um;
}

cv::Mat FindAx2(cv::Mat_<double> rings, CALIBRATION_V1 &cal)
{
  Matrix<real_t> ax_um(4, 1, cal.BallsAx);

  auto m_ax_um = rings.clone();

  m_ax_um.forEach([&](double &val, const int *position) {
    if (!IsValidValue(val))
      return;

    int r = position[0];
    int a = position[1];

    Matrix<real_t> r_um(4);
    for (int b = 0; b < 4; b++)
      r_um[b] = cal.BallsRi[b][r][a];

    CCubicSpline Spline;
    if (Spline.Create(r_um, ax_um))
      Spline.GetAt(val, val, TRUE);
  });

  return m_ax_um;
}

CALIBRATION_EX::CachedData CreateCachedData(CALIBRATION_EX &cal)
{
  auto cachedData = std::make_unique<CALIBRATION_CACHED_DATA>();

  Matrix<real_t> ax_um(4, 1, cal.cal->BallsAx);
  Matrix<real_t> r_um(4);

  for (int r = 0; r < MAX_NUM_RINGS; r++) {
    for (int a = 0; a < 360; a++) {
      for (int b = 0; b < 4; b++)
        r_um[b] = cal.cal->BallsRi[b][r][a];
      auto &spline = cachedData->calSplines[r][a];
      spline.Create(r_um, ax_um);
    }
  }

  return cachedData;
}

cv::Mat FindAx3(cv::Mat_<double> rings, CALIBRATION_EX &cal)
{
  if (cal.cachedData == nullptr)
    cal.cachedData = CreateCachedData(cal);

  auto m_ax_um = rings.clone();
  m_ax_um.forEach([&](double &val, const int *position) {
    if (!IsValidValue(val))
      return;

    int r = position[0];
    int a = position[1];

    auto &spline = cal.cachedData->calSplines[r][a];

    spline.GetAt(val, val, TRUE);
  });

  return m_ax_um;
}

// !!!!! Wrong function do not use
cv::Mat FindAx_wrong_calc(cv::Mat_<double> rings, CALIBRATION_V1 &cal)
{
  Matrix<real_t> ax_um(4, 1, cal.BallsAx);

  auto         m_ax_um = rings.clone();
  Matrix<real_t> r_um(4);
  for (int b = 0; b < 4; b++)
    r_um[b] = cal.BallsRi[b][0][0];

  CCubicSpline Spline;
  Spline.Create(r_um, ax_um);

  m_ax_um.forEach([&](double &val, const int *position) {
    if (!IsValidValue(val))
      return;

    int r = position[0];
    int a = position[1];

    Spline.GetAt(val, val, TRUE);
  });

  return m_ax_um;
}
