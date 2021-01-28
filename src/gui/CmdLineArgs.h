#pragma once

#include <optional>

class CmdLineArgs
{
public:
  CmdLineArgs();

  // clang-format off
  auto ScreenWidth() const  { return m_screenWidth;  }
  auto FullScreen () const  { return m_fullScreen;  }
  // clang-format on

private:
  bool               m_fullScreen{};
  std::optional<int> m_screenWidth;
};
