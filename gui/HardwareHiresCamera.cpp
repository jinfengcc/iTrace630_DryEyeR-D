#include "stdafx.h"
#include "HardwareHiresCamera.h"

HardwareHiresCamera::HardwareHiresCamera()
{
  // DO NOT change the order of the next 2 statements
  InitializeConfig();
  m_cameraImpl = CreateObj<Camera>(m_config);
}

int HardwareHiresCamera::GetWidth() const
{
  return 624;
}

int HardwareHiresCamera::GetHeight() const
{
  return 468;
}

void HardwareHiresCamera::StartTransferringVideoFrame()
{
  m_getNewImage = true;
  m_cameraImpl->StartCapture(m_imgColor);
}

void HardwareHiresCamera::FinishTransferringVideoFrame()
{
  m_cameraImpl->StopCapture();
  m_getNewImage = false;
}

uchar *HardwareHiresCamera::GetRGBData(bool color)
{
  if (Connected()) {
    if (m_getNewImage) {
      m_cameraImpl->GetImage(m_imgColor, Camera::Mode::LORES);
      ConvertToGray();
    }

    return color ? m_imgColor.ptr<uchar>() : m_imgGray.ptr<uchar>();
  }
  else {
    return nullptr;
  }
}

void HardwareHiresCamera::InitializeConfig()
{
  // clang-format off
  SetBrightness( 20);
  SetContrast  ( 10);
  SetHue       (128);
  SetSaturation( 64);
  SetExposure  ( Camera::Exposure::_40ms);
  // clang-format on
}

void HardwareHiresCamera::ConvertToGray()
{
  m_imgColor.copyTo(m_imgGray);
  m_imgGray.forEach([](cv::Vec3b &value, const int *) {
    auto gray = 0.299 * value[2] + 0.587 * value[1] + 0.114 * value[0];
    value[0] = value[1] = value[2] = cv::saturate_cast<std::uint8_t>(gray);
  });
}
