#pragma once

namespace thw {
  // clang-format off
  enum class POWER {
    ACCOMMODATION_MOTOR  = 0,
    COMPENSATION_MOTOR   = 1,
    AODS                 = 2,
    VIDEO_CAMERA         = 3,
    VISTA_HAND_UNIT      = 4,
    TRIANGULATION_LASER  = 5,
    PLACIDO              = 6,
    LADS                 = 7,
    SCANNING_LASER       = 8,
    _MAX_POWER           = 9,
  };

  enum class LEDS {
    INFRARED_LEDS        = 0,
    WHITE_LEDS           = 1,
    ACCOMMODATION_TARGET = 2,
    FAR_FIXATION_TARGET  = 3,
    NEAR_FIXATION_TARGET = 4,
    _MAX_LED             = 5,
  };
  // clang-format on
} // namespace thw

template <class T>
void GetFunc(HMODULE dll, const char *name, T *func)
{
  *(FARPROC *)func = GetProcAddress(dll, name);
  Expects(func != nullptr);
}

// Copied (and modified) from cppreference.com std::invoke
template <class F, class... Args>
constexpr decltype(auto) Invoke(F &&f, Args &&... args)
{
  if (!f)
    TRACEY_THROW("Invalid function pointer");

  if constexpr (std::is_same_v<decltype(std::forward<F>(f)(std::forward<Args>(args)...)), void>)
    std::forward<F>(f)(std::forward<Args>(args)...);
  else
    return std::forward<F>(f)(std::forward<Args>(args)...);
}
