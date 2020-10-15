#include "stdafx.h"
#include "AppVersion.h"
#include <fmt/format.h>
#include <gsl/gsl>

namespace {
  unsigned GetVVVVYMDD();
}

const unsigned SOFTWARE_VERSION_3 = 0x03000000;
const unsigned SOFTWARE_VERSION_4 = 0x04000000;
const unsigned SOFTWARE_VERSION   = GetVVVVYMDD(); // V V V V Y M D D

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

  void VerifyStrings()
  {
    // clang-format off
    static_assert(0x07005a02 == GetVVVVYMDD(7,  0, 2020, 1002));
    static_assert(0x070a5911 == GetVVVVYMDD(7, 10, 2020,  917));

    Expects(fmt::format("{}"   , VERSION_MAJOR) == VERSION_MAJOR_STR);
    Expects(fmt::format("{}"   , VERSION_MINOR) == VERSION_MINOR_STR);
    Expects(fmt::format("{}"   , VERSION_YEAR ) == VERSION_YEAR_STR );
    Expects(fmt::format("{:04}", VERSION_DAY  ) == VERSION_DAY_STR  );

    Expects(fmt::format("{:04}", 901  ) == "0901");
    // clang-format on
  }

  unsigned GetVVVVYMDD()
  {
    VerifyStrings();
    return GetVVVVYMDD(VERSION_MAJOR, VERSION_MINOR, VERSION_YEAR, VERSION_DAY);
  }
} // namespace
