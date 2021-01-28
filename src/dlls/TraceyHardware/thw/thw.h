#pragma once

class THW
{
public:
  using POWER = thw::POWER;
  using LED   = thw::LEDS;

  THW();
  void Shutdown();

  explicit operator bool() const
  {
    return m_dll != nullptr;
  }

  bool Connected() const;
  bool Power(POWER id) const;
  void Power(POWER id, bool pwr);
  int  LedPower(LED led) const;
  void LedPower(LED led, int powerLevel);
  EYE  CurrentEye() const;

  template <class T>
  void GetFunc(const char *name, T *func)
  {
    *(FARPROC *)func = GetProcAddress(m_dll, name);
    Expects(func != nullptr);
  }

private:
  std::array<bool, 32> m_power{};
  std::array<int, 32>  m_led{};

  void PowerImpl(POWER id, bool pwr);

private:
  typedef void(_cdecl *TDllInitializeDeviceStatus)(void);
  typedef BOOL(_cdecl *TDllIsConnected)(void);
  typedef BOOL(_cdecl *TDllGetODOSState)(BOOL *);
  typedef BOOL(_cdecl *TDllSetPowerState)(ulong, ulong);
  typedef BOOL(_cdecl *TDllSetIlluminationLevelsFunc)(thw::LIGHTSOURCES_POWERLEVELS *);

  HMODULE m_dll;

  TDllInitializeDeviceStatus    m_initDeviceStatusFunc;
  TDllIsConnected               m_connectedFunc;
  TDllGetODOSState              m_odosFunc;
  TDllSetPowerState             m_setPowerStateFunc;
  TDllSetIlluminationLevelsFunc m_setIlluminationLevelsFunc;
};
