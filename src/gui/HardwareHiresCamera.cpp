#include "stdafx.h"
#include "HardwareHiresCamera.h"
#include <libs/CalcLib/Image/ImgEnhanceContrast.h>

HardwareHiresCamera::HardwareHiresCamera()
{
  // DO NOT change the order of the next 2 statements
  InitializeConfig();
  m_cameraImpl = CreateObj<Camera>(m_config);
  m_cameraImpl->StartCapture(nullptr);
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
  //m_cameraImpl->StartCapture();
  m_getNewImage = true;
  m_cameraImpl->StartFrameTransfer();
}

void HardwareHiresCamera::FinishTransferringVideoFrame()
{
  //m_cameraImpl->StopCapture();
  //m_getNewImage = false;
  //Sleep(30);
  m_cameraImpl->StopFrameTransfer();
}

uchar *HardwareHiresCamera::GetRGBData(bool color)
{
  if (Connected()) {
    if (m_getNewImage) {
      auto mode = color ? Camera::Mode::LORES_COLOR : Camera::Mode::LORES_GRAY;
      if (!m_cameraImpl->GetImage(m_image, mode))
        return nullptr;

      // ConvertToGray();
    }

    return m_image.ptr<uchar>();
  }
  else {
    return nullptr;
  }
}

void HardwareHiresCamera::SetConfig(const Guid &id, int value)
{
  m_config.Set(id, value);
  //img::EnhanceContrast(cv::Mat());
}

void HardwareHiresCamera::InitializeConfig()
{
  // clang-format off
  SetBrightness( 20);
  SetContrast  ( 10);
  SetHue       (128);
  SetSaturation( 64);
  SetGain      (200);
  // clang-format on
}

//void HardwareHiresCamera::ConvertToGray()
//{
//  cv::cvtColor(m_imgColor, m_imgGray1, cv::COLOR_BGR2GRAY);
//  img::EnhanceContrast(m_imgGray1);
//  cv::cvtColor(m_imgGray1, m_imgGray3, cv::COLOR_GRAY2BGR);
//
//  //m_imgColor.copyTo(m_imgGray);
//  //m_imgGray.forEach([](cv::Vec3b &value, const int *) {
//  //  auto gray = 0.299 * value[2] + 0.587 * value[1] + 0.114 * value[0];
//  //  value[0] = value[1] = value[2] = cv::saturate_cast<std::uint8_t>(gray);
//  //});
//}
