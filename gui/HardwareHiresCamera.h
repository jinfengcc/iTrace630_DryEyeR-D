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
  cv::Mat                m_imgColor;
  cv::Mat3b              m_imgGray;
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
    catch(...)
    {
      return FALSE;
    }
  }

  int GetWidth() const;
  int GetHeight() const;

  void StartTransferringVideoFrame()
  {
    m_getNewImage = true;
  }
  void FinishTransferringVideoFrame()
  {
    m_getNewImage = false;
  }

  uchar *GetRGBData(bool color);

  // clang-format off
  void SetBrightness(int value)  { m_config.Set(Camera::BRIGHTNESS, value); }
  void SetContrast  (int value)  { m_config.Set(Camera::CONTRAST  , value); }
  void SetHue       (int value)  { m_config.Set(Camera::HUE       , value); }
  void SetSaturation(int value)  { m_config.Set(Camera::SATURATION, value); }
  void SetGain      (int value)  { m_config.Set(Camera::GAIN      , value); }
  void SetExposure  (int value)  { m_config.Set(Camera::EXPOSURE  , value); }   // 0 = Auto, -1..-13
  // clang-format on
};

