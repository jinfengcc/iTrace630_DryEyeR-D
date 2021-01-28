#pragma once

class Device : public Implements<hal::IDevice>
{
public:
  Device(THW *thw);

  int FirmwareBuildNumber() const;

private:
  bool Connected() const override;
  void Shutdown() override;
  bool Power(POWER p) const override;
  void Power(POWER p, bool power) override;
  int  Power(LED led) const override;
  void Power(LED led, int powerLevel) override;
  EYE  CurrentEye() const override;
  bool ButtonIsPressed() const override;

private:
  typedef BOOL(_cdecl *TDllGetFirmwareBuildNumber)(ushort *);
  typedef BOOL(_cdecl *TDllCompareFirmware)(const wchar_t *, int *, int *, ushort *, ushort *);
  typedef BOOL(_cdecl *TDllUpdateFirmware)(const wchar_t *, int *);
  typedef BOOL(_cdecl *TDllGetButtonState)(int *, BOOL *);

  THW *                      m_thw;
  TDllGetFirmwareBuildNumber m_firmVersionFunc;
  TDllCompareFirmware        m_compareFirmwareFunc;
  TDllUpdateFirmware         m_updateFirmwareFunc;
  TDllGetButtonState         m_buttonStatefunc;
};
