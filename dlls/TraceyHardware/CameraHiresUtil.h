#pragma once

namespace hrc {
  void DumpSettings(const char *title, cv::VideoCapture &vc);
  int GetDeviceId(std::string_view name);

}


