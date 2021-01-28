#include "pch.h"
#include "ExamHelper.h"

cv::Mat NextImageFromBlob(const BYTE *&pb)
{
  if (auto jpgSize = *reinterpret_cast<const int32_t *>(pb); jpgSize > 0) {
    auto img = cv::imdecode({pb + sizeof(int32_t), jpgSize}, cv::IMREAD_UNCHANGED);
    pb += jpgSize + sizeof(int32_t);
    return img;
  }
  else {
    return {};
  }
}
