#include "pch.h"
#include "Image.h"

bool GetPixel(const cv::Mat &img, const cv::Point2d &pt, double &value)
{
  auto xi = (int)pt.x;
  auto yi = (int)pt.y;

  if (xi < 0 || xi >= img.cols - 1)
    return false;

  if (yi < 0 || yi >= img.rows - 1)
    return false;

  double y0 = 0, y1 = 0;

  switch (img.type()) {
  case CV_8UC1:
    y0 = std::lerp(img.at<BYTE>({xi + 0, yi + 0}), img.at<BYTE>({xi + 0, yi + 1}), pt.y - yi);
    y1 = std::lerp(img.at<BYTE>({xi + 1, yi + 0}), img.at<BYTE>({xi + 1, yi + 1}), pt.y - yi);
    break;

  case CV_64FC1:
    y0 = std::lerp(img.at<double>({xi + 0, yi + 0}), img.at<double>({xi + 0, yi + 1}), pt.y - yi);
    y1 = std::lerp(img.at<double>({xi + 1, yi + 0}), img.at<double>({xi + 1, yi + 1}), pt.y - yi);
    break;

  default:
    Expects(false);
    break;
  }

  value = std::lerp(y0, y1, pt.x - xi);

  return true;
}
