#pragma once

#include <numeric>

namespace Gdiplus {
  using std::max;
  using std::min;
}

#include <gdiplus.h>

struct AppInitializer
{
  AppInitializer();
  ~AppInitializer();

private:
  ULONG_PTR m_gdiplusToken{0};
};
