#pragma once

#include <libs/CommonLib/CreateObj.h>
#include <interfaces/hal/ICamera.h>
#include <libs/CommonLib/TraceyConfigImpl.h>

class HardwareHiresCamera
{
private:
  using Camera = hal::ICameraHires;

  wil::com_ptr_t<Camera> m_cameraImpl;
  bool                   m_getNewImage{false};
  cv::Mat                m_image;
  TraceyConfigImpl       m_config;

  void InitializeConfig();
  void ConvertToGray();

public:
  HardwareHiresCamera();

  bool Connected() const
  {
    try {
      return const_cast<HardwareHiresCamera *>(this)->m_cameraImpl->Connected();
    }
    catch (...) {
      return false;
    }
  }
  bool Connect(bool yes = true)
  {
    try {
      return m_cameraImpl->Connect(yes);
    }
    catch (...) {
      return false;
    }
  }
  bool Disconnect()
  {
    return Connect(false);
  }

  int    GetWidth() const;
  int    GetHeight() const;
  void   StartTransferringVideoFrame();
  void   FinishTransferringVideoFrame();
  uchar *GetRGBData(bool color);

  // clang-format off
  void SetBrightness(int value)  { SetConfig(Camera::BRIGHTNESS, value); }
  void SetContrast  (int value)  { SetConfig(Camera::CONTRAST  , value); }
  void SetGain      (int value)  { SetConfig(Camera::GAIN      , value); }
  void SetRedGain   (int value)  { SetConfig(Camera::GAIN_RED  , value); }
  void SetGreenGain (int value)  { SetConfig(Camera::GAIN_GREEN, value); }
  void SetBlueGain  (int value)  { SetConfig(Camera::GAIN_BLUE , value); }
  // clang-format on

  void SetHue       (int)  { }
  void SetSaturation(int)  { }

private:
  void SetConfig(const Guid &id, int value);
};
