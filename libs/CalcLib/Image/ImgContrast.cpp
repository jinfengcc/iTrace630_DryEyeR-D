#include "pch.h"
#include "ImgContrast.h"
#include "Histogram.h"

namespace {
  using MapData = std::array<std::uint8_t, 256>;
  void GetMapData(const cv::Mat &img, MapData &md);
} // namespace

void img::EnhanceContrast(cv::Mat &img)
{
  MapData md;
  GetMapData(img, md);

  Expects(img.type() == CV_8UC1);
  cv::Mat1b(img).forEach([&](uchar &v, const int *) { v = static_cast<uchar>(md[v]); });
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

  void GetMapData(Pair x, MapData &md)
  {
    const int    x0 = x.first;
    const int    x1 = x.second;
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
