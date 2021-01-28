#pragma once

#include <cmath>
#include <opencv2/opencv.hpp>
#include <libs/CommonLib/constants.h>

#ifndef DEG2RAD_DEFINED
#define DEG2RAD_DEFINED
inline double Deg2Rad(double x)
{
  return x * _Pi_180;
}
inline double Rad2Deg(double x)
{
  return x * _180_Pi;
}
inline cv::Point2d Polar2Cart(double r, double th_deg)
{
  return {r * cos(Deg2Rad(th_deg)), r * sin(Deg2Rad(th_deg))};
}
#endif

inline cv::Size2d ImgSizeMM2PixelsPerMM(cv::Size2d imgSizeInMM, cv::Size2i imgSizeInPixels)
{
  auto cx = imgSizeInPixels.width / imgSizeInMM.width;
  auto cy = imgSizeInPixels.height / imgSizeInMM.height;
  return {cx, cy};
}

namespace Math {
  extern std::array<double, 360> SIN, COS;
}
