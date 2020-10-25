#include "pch.h"
#include "thw.h"
#include "Device.h"

THW::THW()
{
  std::fill(m_power.begin(), m_power.end(), false);
  std::fill(m_led.begin(), m_led.end(), -1);

  m_dll = LoadLibraryA("thw.dll");
  Expects(m_dll != nullptr);

  // clang-format off
  GetFunc("Lynx_InitializeDeviceStatus", &m_initDeviceStatusFunc);
  GetFunc("Lynx_IsConnected"           , &m_connectedFunc);
  GetFunc("Lynx_GetODOSState"          , &m_odosFunc);
  GetFunc("Lynx_SetPowerState"         , &m_setPowerStateFunc);
  GetFunc("Lynx_SetIlluminationLevels" , &m_setIlluminationLevelsFunc);
  // clang-format on

  Invoke(m_initDeviceStatusFunc);
  if (!Invoke(m_connectedFunc))
    LOG_Err("HAL: Device is not connected");
}

bool THW::Connected() const
{
  return Invoke(m_connectedFunc) == TRUE;
}

bool THW::Power(POWER id) const
{
  return m_power[static_cast<int>(id)];
}

void THW::Power(POWER id, bool pwr)
{
  if (pwr != Power(id))
    PowerImpl(id, m_power[static_cast<int>(id)] = pwr);
}

int THW::LedPower(LED led) const
{
  return m_led[static_cast<int>(led)];
}

void THW::LedPower(LED led, int powerLevel)
{
  powerLevel = std::clamp(powerLevel, 0, 100);
  if (powerLevel != LedPower(led)) {
    m_led[static_cast<int>(led)] = powerLevel;

    union {
      int32_t                       raw[8];
      thw::LIGHTSOURCES_POWERLEVELS power;
    } pl;

    static_assert(sizeof(thw::LIGHTSOURCES_POWERLEVELS) == sizeof(pl.raw));

    // This implies that if a value is -1, it is ignored by thw.dll (TODO: Confirm)
    std::fill(std::begin(pl.raw), std::end(pl.raw), -1);
    pl.raw[static_cast<int>(led)] = powerLevel;

    if (Invoke(m_setIlluminationLevelsFunc, &pl.power)) {
      LOG_Diag("Setting Illumination({})={}", static_cast<int>(led), powerLevel);
    }
    else {
      LOG_Err("Unable to set Illumination({})={}", static_cast<int>(led), powerLevel);
      LOG_Diag("**** Error: Setting Illumination({})={}", static_cast<int>(led), powerLevel);
    }
  }
}

EYE THW::CurrentEye() const
{
  BOOL OD{TRUE};
  if (m_odosFunc)
    m_odosFunc(&OD);

  return OD ? EYE::OD : EYE::OS;
}

void THW::PowerImpl(POWER id, bool pwr)
{
  if (Invoke(m_setPowerStateFunc, pwr ? 0xFFFFFFFF : 0, 1 << static_cast<int>(id))) {
    LOG_Diag("Setting Power({})={}", static_cast<int>(id), pwr ? "ON" : "OFF");
  }
  else {
    LOG_Err("Unable to set Power({})={}", static_cast<int>(id), pwr ? "ON" : "OFF");
    LOG_Diag("**** Error: Setting Power({})={}", static_cast<int>(id), pwr ? "ON" : "OFF");
  }
}

void THW::Shutdown()
{
  ShutdownSignal();

  // Turn lights off
  for (int i = 0; i < static_cast<int>(thw::LEDS::_MAX_LED); ++i)
    LedPower(static_cast<thw::LEDS>(i), 0);

  // Turn power off
  for (int id = 0; id < static_cast<int>(thw::POWER::_MAX_POWER); ++id)
    PowerImpl(static_cast<POWER>(id), false);

  // Don't free DLL, let the system do it
}
