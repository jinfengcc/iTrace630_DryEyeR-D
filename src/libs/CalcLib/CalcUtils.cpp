#include "pch.h"
#include "CalcUtils.h"

namespace {
  using namespace Math;

  struct Init
  {
    Init()
    {
      for (unsigned i = 0; i < SIN.size(); ++i) {
        SIN[i] = sin(Deg2Rad(i));
        COS[i] = cos(Deg2Rad(i));
      }
    }
  };

  static Init _init;
}

namespace Math {
  std::array<double, 360> SIN, COS;
}
