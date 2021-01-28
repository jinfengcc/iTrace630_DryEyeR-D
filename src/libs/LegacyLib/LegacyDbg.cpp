#include "stdafx.h"
#include "LegacyDbg.h"
#include "Mat2EyeImage.h"

namespace {
  bool PtInImage(const cv::Mat &img, cv::Point pt)
  {
    return pt.x >= 0 && pt.x < img.cols && pt.y >= 0 && pt.y < img.rows;
  }

  void DrawCircle(cv::Mat dbgImg, cv::Point2i p, cv::Vec3b color, int radius)
  {
    if (p.x < 0 || p.x > dbgImg.cols - radius || p.y < 0 || p.y > dbgImg.rows - radius)
      return;
    cv::ellipse(dbgImg, p, {radius, radius}, 0, 0, 360, color);
  }

  void DrawPoint(cv::Mat dbgImg, cv::Point2i p, cv::Vec3b color)
  {
    constexpr int DRAW_ELLIPSE = 0;

    if constexpr (DRAW_ELLIPSE) {
      DrawCircle(dbgImg, p, color, 2);
    }
    else {
      if (PtInImage(dbgImg, p)) {
        auto *ptr = dbgImg.ptr<cv::Vec3b>(p.y);
        ptr[p.x]  = color;
      }
    }
  }
} // namespace

namespace dbg {
  cv::Mat DrawRingsUnwrapped(const Matrix<real_t> &old, const CEyeImage *eyeImage)
  {
    auto img = OldMat2Mat(old);

    if (eyeImage) {
      const auto pixPerUM = 1.0 / STEP_SIZE;

      for (int i = 0; i < MAX_NUM_RINGS; ++i) {
        const auto color = (i & 1) ? cv::Vec3b(0, 0, 255) : cv::Vec3b(0, 255, 0);

        for (auto a = 0; a < 360; ++a) {
          if (auto v = eyeImage->m_ri_r_um[i][a]; v != INVALID_VALUE)
            DrawPoint(img, {intRound(v * pixPerUM), a}, color);
        }
      }
    }

    return img;
  }
  void DrawRing(cv::Mat &dbgImg, const CEyeImage *eyeImage, int r, cv::Vec3b color)
  {
    auto center = cv::Point2d(16000.0 / 2, 12000.0 / 2);

    for (auto a = 0; a < 360; ++a) {
      if (auto v = eyeImage->m_ri_r_um[r][a]; v != INVALID_VALUE) {
        auto x = intRound((center.x + v * cos(Deg2Rad(a))) * eyeImage->m_w / eyeImage->m_w_um);
        auto y = intRound((center.y + v * sin(Deg2Rad(a))) * eyeImage->m_h / eyeImage->m_h_um);
        DrawPoint(dbgImg, {x, y}, color);
      }
    }
  }
  cv::Mat DrawRings(const Matrix<real_t> &old, const CEyeImage *eyeImage)
  {
    auto img = OldMat2Mat(old);
    return DrawRings(img, eyeImage);
  }

  cv::Mat DrawRings(cv::Mat img, const CEyeImage *eyeImage)
  {
    if (eyeImage) {
      const auto pixPerUM = 1.0 / STEP_SIZE;

      for (int i = 0; i < MAX_NUM_RINGS; ++i) {
        const auto color = (i & 1) ? cv::Vec3b(0, 0, 255) : cv::Vec3b(0, 255, 0);
        DrawRing(img, eyeImage, i, color);
      }
    }

    return img;
  }
} // namespace dbg
