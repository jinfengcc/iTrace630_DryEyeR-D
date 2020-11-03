#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

template <class T, class U>
std::vector<T> Mat2Vector(const cv::Mat_<U> &mat)
{
  if constexpr (std::is_same_v<T, U>) {
    if (mat.isContinuous()) {
      return {mat.ptr<T>(0), mat.ptr<T>(0) + mat.total()};
    }
    else {
      std::vector<T> array;
      array.reserve(mat.total());
      for (int i = 0; i < mat.rows; ++i)
        array.insert(array.end(), mat.ptr<T>(i), mat.ptr<T>(i) + mat.cols);
      return array;
    }
  }
  else {
    std::vector<T> array;
    array.reserve(mat.total());
    for (auto x : mat)
      array.push_back(static_cast<T>(x));
  }
}
