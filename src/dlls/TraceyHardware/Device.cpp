#include "pch.h"
#include "Device.h"

Device::Device(THW *thw)
  : Implements<hal::IDevice>("IDevice")
  , m_thw(thw)
{
  // clang-format off
  thw->GetFunc("Lynx_GetFirmwareBuildNumber", &m_firmVersionFunc);
  thw->GetFunc("Lynx_CompareFirmware"       , &m_compareFirmwareFunc);
  thw->GetFunc("Lynx_UpdateFirmware"        , &m_updateFirmwareFunc);
  thw->GetFunc("Lynx_GetButtonState"        , &m_buttonStatefunc);
  // clang-format on
}

bool Device::Connected() const
{
  return m_thw->Connected();
}

void Device::Shutdown()
{
  m_thw->Shutdown();
}

bool Device::Power(POWER p) const
{
  // clang-format off
  static_assert(thw::POWER::ACCOMMODATION_MOTOR == static_cast<thw::POWER>(POWER::ACCOMMODATION_MOTOR));
  static_assert(thw::POWER::COMPENSATION_MOTOR  == static_cast<thw::POWER>(POWER::COMPENSATION_MOTOR ));
  static_assert(thw::POWER::AODS                == static_cast<thw::POWER>(POWER::AODS               ));
  static_assert(thw::POWER::VIDEO_CAMERA        == static_cast<thw::POWER>(POWER::VIDEO_CAMERA       ));
  static_assert(thw::POWER::VISTA_HAND_UNIT     == static_cast<thw::POWER>(POWER::VISTA_HAND_UNIT    ));
  static_assert(thw::POWER::TRIANGULATION_LASER == static_cast<thw::POWER>(POWER::TRIANGULATION_LASER));
  static_assert(thw::POWER::PLACIDO             == static_cast<thw::POWER>(POWER::PLACIDO            ));
  static_assert(thw::POWER::LADS                == static_cast<thw::POWER>(POWER::LADS               ));
  static_assert(thw::POWER::SCANNING_LASER      == static_cast<thw::POWER>(POWER::SCANNING_LASER     ));
  // clang-format on

  return m_thw->Power(static_cast<thw::POWER>(p));
}

void Device::Power(POWER p, bool power)
{
  m_thw->Power(static_cast<thw::POWER>(p), power);
}

int Device::Power(LED led) const
{
  return m_thw->LedPower(static_cast<thw::LEDS>(led));
}

void Device::Power(LED led, int powerLevel)
{
  // clang-format off
  static_assert(LED::INFRARED             == static_cast<LED>(thw::LEDS::INFRARED_LEDS       ));
  static_assert(LED::WHITE                == static_cast<LED>(thw::LEDS::WHITE_LEDS          ));
  static_assert(LED::ACCOMMODATION_TARGET == static_cast<LED>(thw::LEDS::ACCOMMODATION_TARGET));
  static_assert(LED::FAR_FIXATION_TARGET  == static_cast<LED>(thw::LEDS::FAR_FIXATION_TARGET ));
  static_assert(LED::NEAR_FIXATION_TARGET == static_cast<LED>(thw::LEDS::NEAR_FIXATION_TARGET));
  // clang-format on

  m_thw->LedPower(static_cast<thw::LEDS>(led), powerLevel);
}

EYE Device::CurrentEye() const
{
  return m_thw->CurrentEye();
}

bool Device::ButtonIsPressed() const
{
  int  Count{};
  BOOL State{FALSE};

  if (!m_buttonStatefunc || !m_buttonStatefunc(&Count, &State))
    return false;

  return State == TRUE;
}
