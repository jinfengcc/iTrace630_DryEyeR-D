#pragma once

#include <optional>
#include <opencv2/opencv.hpp>
#include "Dimension.h"
#include "libs/CalcLib/CalcUtils.h"

struct IMAGE
{
  // (0,0) is at the center of the image - x left to right, y top to bottom
  using Point = cv::Point_<Microns>;
  using Size  = cv::Size_<Microns>;

  cv::Mat pixels;
  Size    size{};
};


template <int N = 1>
inline cv::Point2d DP2Pixel(const IMAGE &img, const cv::Point_<Dimension<N>> &p)
{
  auto x = img.pixels.cols * (p.x / img.size.width);
  auto y = img.pixels.rows * (p.y / img.size.height);

  return {x, y};
}

template <class T, int N = 1>
inline cv::Point_<T> DPolar2Pixel(const IMAGE &img, const Dimension<N> r, double theta)
{
  auto scale = img.pixels.cols / Microns(img.size.width).value();

  auto x = img.pixels.cols / 2.0 + scale * Microns(r).value() * cos(Deg2Rad(theta));
  auto y = img.pixels.rows / 2.0 + scale * Microns(r).value() * sin(Deg2Rad(theta));

  if constexpr (std::is_integral_v<T>) {
    x = std::round(x);
    y = std::round(y);
  }

  return {static_cast<T>(x), static_cast<T>(y)};
}

template <int N = 1>
inline cv::Point_<Dimension<N>> Pixel2DP(const IMAGE &img, const cv::Point2d &p)
{
  auto x = img.size.width  * p.x / img.pixels.cols;
  auto y = img.size.height * p.y / img.pixels.rows;

  return {x, y};
}

inline bool PtInImage(const cv::Mat &img, cv::Point pt)
{
  return pt.x >= 0 && pt.x < img.cols && pt.y >= 0 && pt.y < img.rows;
}

bool GetPixel(const cv::Mat &img, const cv::Point2d &pt, double &value);

inline bool GetPixel(const IMAGE &img, const IMAGE::Point &p, double &value)
{
  auto pt = DP2Pixel(img, p);
  return GetPixel(img.pixels, pt, value);
}

inline std::optional<double> GetPixel(const cv::Mat &img, const cv::Point2d &p)
{
  double value;
  if (GetPixel(img, p, value))
    return {value};
  else
    return {};
}

inline std::optional<double> GetPixel(const IMAGE &img, const IMAGE::Point &p)
{
  return GetPixel(img.pixels, DP2Pixel(img, p));
}
