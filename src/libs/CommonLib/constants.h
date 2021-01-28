#pragma once

#include <algorithm>
#include <type_traits>

constexpr auto _E1     = 2.71828182845904523536028747135; // change from _E to _E1. 0528 for vs2013
constexpr auto _Pi     = 3.14159265358979323846264338328;
constexpr auto _2_Pi   = 6.28318530717958647692528676656;
constexpr auto _Pi_2   = 1.57079632679489661923132169164;
constexpr auto _Pi_180 = 0.01745329251994329576923690768;
constexpr auto _180_Pi = 57.29577951308232087679815481411;

#define INVALID_VALUE 1'000'000'000
#define TOPRING_VALUE 20'000'000
#define LID_VALUE     30'000'000

template<class T>
inline bool IsValidValue(T value)
{
  const DWORD BAD_VALUE_MIN = 10'000'000;

  static_assert(INVALID_VALUE > BAD_VALUE_MIN);
  static_assert(TOPRING_VALUE > BAD_VALUE_MIN);
  static_assert(LID_VALUE > BAD_VALUE_MIN);

  return value < static_cast<DWORD>(BAD_VALUE_MIN);
}

template <class T>
inline bool AllValuesValid(std::initializer_list<T> v)
{
  return std::all_of(v.begin(), v.end(), [](auto i) { return IsValidValue<T>(i); });
}
