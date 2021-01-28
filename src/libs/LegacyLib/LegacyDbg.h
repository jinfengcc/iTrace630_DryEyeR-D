#pragma once

#include "Image.h"

namespace dbg {

  constexpr int  NUM_STEPS = 512;
  constexpr auto STEP_SIZE = 5200.0 / (NUM_STEPS - 1);  // Microns

  cv::Mat DrawRingsUnwrapped(const Matrix<real_t> &old, const CEyeImage *eyeImage);
  void    DrawRing(cv::Mat &dbgImg, const CEyeImage *eyeImage, int r, cv::Vec3b color);
  cv::Mat DrawRings(const Matrix<real_t> &old, const CEyeImage *eyeImage);
  cv::Mat DrawRings(cv::Mat img, const CEyeImage *eyeImage);

} // namespace dbg
