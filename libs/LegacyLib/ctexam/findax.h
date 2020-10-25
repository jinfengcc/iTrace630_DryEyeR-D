#pragma once

#include "../CubicSpline.h"

struct CALIBRATION_CACHED_DATA
{
  CCubicSpline calSplines[MAX_NUM_RINGS][360];
};

cv::Mat FindAx(cv::Mat_<double> rings, CALIBRATION_V1 &cal);
cv::Mat FindAx2(cv::Mat_<double> rings, CALIBRATION_V1 &cal);
cv::Mat FindAx3(cv::Mat_<double> rings, CALIBRATION_EX &cal);

cv::Mat FindAx_wrong_calc(cv::Mat_<double> rings, CALIBRATION_V1 &cal);
