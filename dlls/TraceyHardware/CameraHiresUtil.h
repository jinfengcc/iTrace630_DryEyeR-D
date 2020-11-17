#pragma once

namespace hrc {
  void DumpSettings(const char *title, cv::VideoCapture &vc);
  int GetDeviceId(std::span<const char *> names);

}


