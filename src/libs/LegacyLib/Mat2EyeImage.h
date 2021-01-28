#pragma once

#include "Image.h"

inline std::unique_ptr<CEyeImage> Mat2EyeImage(const cv::Size_<double> &size, const cv::Mat &img, int numRings)
{
  if (img.channels() != 3)
    throw std::runtime_error("Invalid RGB Eye image");

  auto eimg        = std::make_unique<CEyeImage>();
  eimg->m_NumRings = numRings;
  eimg->m_w        = img.cols;
  eimg->m_h        = img.rows;
  eimg->m_w_um     = size.width;
  eimg->m_h_um     = size.height;
  eimg->m_RGBData.Attach(img.rows, img.cols * 3, img.data);

  return eimg;
}

inline cv::Mat EyeImage2Mat(const CEyeImage &bi)
{
  return cv::Mat(bi.m_h, bi.m_w, CV_8UC3, bi.m_RGBData.GetMem());
}

inline cv::Mat OldMat2Mat(const Matrix<real_t> &img)
{
  auto x = cv::Mat(img.GetNumRows(), img.GetNumCols(), CV_64F, img.GetMem());

  cv::Mat dbgImg;
  double  min, max;
  cv::minMaxIdx(x, &min, &max);
  if (min != max)
    x.convertTo(dbgImg, CV_8U, 255.0 / (max - min), -255.0 * min / (max - min));

  cv::cvtColor(dbgImg, dbgImg, cv::COLOR_GRAY2BGR);

  return dbgImg;
};
