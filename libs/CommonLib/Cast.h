#pragma once

#include <concepts>
#include "Guid.h"

template <class To, class From>
inline To Cast(const From &from)
{
  if constexpr (std::is_arithmetic_v<From> && std::is_integral_v<To>) {
    return static_cast<To>(std::round(from));
  }
  else if constexpr (std::is_convertible_v<From, std::string_view> && std::is_arithmetic_v<To>) {
    char * p{};
    double v = strtod(std::string_view(from).data(), &p);
    if (*p != 0)
      throw std::runtime_error("String is not float");

    return Cast<To, double>(v);
  }
  else if constexpr (std::is_convertible_v<GUID, To>) {
    return static_cast<To>(Guid(from));
  }
  else
  {
    return static_cast<To>(from);
  }
}

template <>
inline std::string Cast<std::string, GUID>(const GUID &guid)
{
  return Guid(guid).string();
}

template <>
inline std::wstring Cast<std::wstring, GUID>(const GUID &guid)
{
  return Guid(guid).wstring();
}
