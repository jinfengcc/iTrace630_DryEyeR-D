#include "pch.h"
#include "Histogram.h"

cv::Mat_<float> Histogram(const cv::Mat &img, int bins)
{
  cv::Mat_<float> hist;

  int          channels[] = {0};
  int          histSize[] = {bins};
  float        sranges[]  = {0, 256};
  const float *ranges[]   = {sranges};

  cv::calcHist(&img, 1, channels, cv::Mat(), // do not use mask
               hist, 1, histSize, ranges,
               true, // the histogram is uniform
               false);

  return hist;
}
