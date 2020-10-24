#include "stdafx.h"
#include "AppVersion.h"

namespace {
  constexpr unsigned GetVVVVYMDD(unsigned MAJOR, unsigned MINOR, unsigned YEAR, unsigned DAY)
  {
    unsigned VVVV = MAKEWORD(MINOR, MAJOR);
    unsigned Y    = YEAR - 2015;
    unsigned M    = DAY / 100;
    unsigned DD   = DAY % 100;
    unsigned YM   = (Y << 4) | M;
    unsigned YMDD = MAKEWORD(DD, YM);

    return MAKELONG(YMDD, VVVV);
  }

  constexpr unsigned GetVVVVYMDD()
  {
    return GetVVVVYMDD(VERSION_MAJOR, VERSION_MINOR, VERSION_YEAR, VERSION_DAY);
  }
} // namespace

const unsigned SOFTWARE_VERSION_3 = 0x03000000;
const unsigned SOFTWARE_VERSION_4 = 0x04000000;
const unsigned SOFTWARE_VERSION   = GetVVVVYMDD(); // V V V V Y M D D

CString FullSoftwareVersion()
{
  uint v1 = VERSION_MAJOR;
  uint v2 = VERSION_MINOR / 10;
  uint v3 = VERSION_MINOR % 10;
  uint y  = VERSION_YEAR;
  uint m  = VERSION_DAY / 100;
  uint d  = VERSION_DAY % 100;

  CString s;
  s.Format(_T("%u.%u.%u %04u-%02u-%02u"), v1, v2, v3, y, m, d);

  return s;
}

// Sanity checks
namespace {
  constexpr unsigned Str2Uint(const char *s)
  {
    unsigned x = 0;
    for (; *s != 0; ++s)
      x = (x * 10) + (*s - '0');
    return x;
  }

  // clang-format off
  static_assert(0x07005a02 == GetVVVVYMDD(7,  0, 2020, 1002));
  static_assert(0x070a5911 == GetVVVVYMDD(7, 10, 2020,  917));

  static_assert(Str2Uint(VERSION_MAJOR_STR) == VERSION_MAJOR);
  static_assert(Str2Uint(VERSION_MINOR_STR) == VERSION_MINOR);
  static_assert(Str2Uint(VERSION_YEAR_STR ) == VERSION_YEAR );
  static_assert(Str2Uint(VERSION_DAY_STR  ) == VERSION_DAY  );
  // clang-format on

} // namespace
