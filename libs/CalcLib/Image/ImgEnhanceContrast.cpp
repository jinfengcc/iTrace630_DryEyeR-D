#include "pch.h"
#include "ImgEnhanceContrast.h"
#include "Histogram.h"
#include <libs/CommonLib/AppSettings.h>

namespace {
  using MapData = std::array<std::uint8_t, 256>;

  void GetMapData(const cv::Mat &img, MapData &md);

  const double K = 0.0;
} // namespace

void img::EnhanceContrast(cv::Mat img)
{
  if (img.empty())
    return;

  MapData md;
  GetMapData(img, md);

  Expects(img.type() == CV_8UC1);
  cv::Mat1b(img).forEach([&](uchar &v, const int *) { v = static_cast<uchar>(md[v]); });

  auto size  = AppSettings::Instance()->Get("hrcam.filter.size", 0);
  auto sigma = AppSettings::Instance()->Get("hrcam.filter.sigma", 0);

  if (size > 0 && sigma > 0)
    cv::GaussianBlur(img, img, {size, size}, sigma, sigma);
}

namespace {
  using Pair = std::pair<int, int>;

  Pair GetEdgePoints(const cv::Mat_<float> &hist, int numPixels);
  void GetMapData(Pair x, MapData &md);

  void GetMapData(const cv::Mat &img, MapData &md)
  {
    auto hist = Histogram(img, 256);
    auto edgp = GetEdgePoints(hist, img.total());

    GetMapData(edgp, md);
  }

  void GetMapData(Pair p, MapData &md)
  {
    if (K > 0) {
      for (int i = 0; i < 256; ++i) {
        auto x = 255 / (1 + std::exp(-K * (i - p.second)));
        md[i]  = static_cast<int>(std::round(x));
      }
      return;
    }
    const int    x0 = p.first;
    const int    x1 = p.second;
    const double y0 = x0 / 2.0;
    const double y1 = 256 - y0;

    const double slope1 = y0 / x0;
    const double slope2 = (y1 - y0) / (x1 - x0 - 1);
    const double slope3 = (255 - y1) / (255 - x1);

    for (int i = 0; i < x0; ++i)
      md[i] = static_cast<std::uint8_t>(i * slope1);

    for (int i = x0; i < x1; ++i)
      md[i] = static_cast<std::uint8_t>(y0 + (i - x0) * slope2);

    for (int i = x1; i < 256; ++i)
      md[i] = static_cast<std::uint8_t>(y1 + (i - x1) * slope3);
  }

  Pair GetEdgePoints(const cv::Mat_<float> &hist, int numPixels)
  {
    Expects(hist.cols == 1);

    const float x0_lim = 0.65f * numPixels;
    const float x1_lim = 0.95f * numPixels;

    int   x0  = 0;
    int   x1  = 0;
    float sum = 0.0f;

    for (int i = 0; i < hist.rows && x1 == 0; ++i) {
      sum += hist(i, 0);

      if (x0 == 0 && sum > x0_lim)
        x0 = i;

      else if (sum > x1_lim)
        x1 = i;
    }
    return {x0, x1};
  }
} // namespace
