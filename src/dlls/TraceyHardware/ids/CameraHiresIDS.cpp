#include "pch.h"
#include "CameraHiresIDS.h"
#include "CameraHiresImplIDS.h"
#include <interfaces/ITraceyConfig.h>

namespace {
  enum { IMG_ROWS = 468, IMG_COLS = 624 };
  const int HIRES_COLOR_ = static_cast<int>(ICameraHires::Mode::HIRES_COLOR);
  const int LORES_COLOR_ = static_cast<int>(ICameraHires::Mode::LORES_COLOR);
  const int HIRES_GRAY3_ = static_cast<int>(ICameraHires::Mode::HIRES_GRAY);
  const int LORES_GRAY3_ = static_cast<int>(ICameraHires::Mode::LORES_GRAY);
  const int HIRES_GRAY1_ = 4;

} // namespace

CameraHiresIDS::CameraHiresIDS()
  : Implements<hal::ICameraHires>("ICameraHires")
{
  static std::weak_ptr<IDSVideoCamera> camImpl;
  static std::mutex                    mutex;

  std::lock_guard lock(mutex);
  if (m_pimpl = camImpl.lock(); !m_pimpl) {
    m_pimpl = std::make_shared<IDSVideoCamera>([this](const cv::Mat &img) {
      UpdateFPS();
      if (m_transferImg) {
        UpdateImages(img);
        if (m_callback)
          m_callback(0);
      }
    });
    camImpl = m_pimpl;
  }
}

void CameraHiresIDS::Initialize(IUnknown *unk)
{
  m_config.reset();
  if (unk) {
    if (m_config = wil::com_query<ITraceyConfig>(unk))
      m_configSignalId = m_config->OnDataChanged([this]() { Settings(m_config.get()); });
  }

  m_pimpl->Open();
  Settings(m_config.get());
}

bool CameraHiresIDS::Connect(bool yes)
{
  bool connected = m_pimpl->IsOpened();
  if (yes) {
    m_pimpl->Open();
    Settings(m_config.get());
  }
  else {
    m_pimpl->Close();
  }

  return connected;
}

void CameraHiresIDS::StartCapture(std::function<void(unsigned)> notify)
{
  m_callback = std::move(notify);
}

void CameraHiresIDS::StopCapture()
{
  m_callback = nullptr;
}

void CameraHiresIDS::StartFrameTransfer()
{
  m_transferImg = true;
}

void CameraHiresIDS::StopFrameTransfer()
{
  m_transferImg = false;
}

bool CameraHiresIDS::GetImage(cv::Mat &img, Mode mode) const
{
  if (auto &i = m_images[static_cast<int>(mode)]; i.empty())
    return false;
  else {
    i.copyTo(img);
    return true;
  }
}

bool CameraHiresIDS::Connected(double *fps) const
{
  bool b = m_pimpl->IsOpened();

  if (fps)
    *fps = b ? m_framesPerSecond : 0;

  return b;
}

bool CameraHiresIDS::Settings(ITraceyConfig *tc)
{
  return false;
}

void CameraHiresIDS::UpdateImages(const cv::Mat &orig)
{
  auto &colorImg = m_images[HIRES_COLOR_];
  auto &grayImg3 = m_images[HIRES_GRAY3_];
  auto &grayImg1 = m_images[HIRES_GRAY1_];

  orig.copyTo(colorImg);

  cv::cvtColor(colorImg, grayImg1, cv::COLOR_BGR2GRAY);
  cv::cvtColor(grayImg1, grayImg3, cv::COLOR_GRAY2BGR);

  cv::resize(colorImg, m_images[LORES_COLOR_], {IMG_COLS, IMG_ROWS});
  cv::resize(grayImg3, m_images[LORES_GRAY3_], {IMG_COLS, IMG_ROWS});
}

void CameraHiresIDS::UpdateFPS()
{
  ++m_frameCount;
  if (auto elapsed = GetTickCount64() - m_frameTimer; elapsed >= 2000) {
    m_framesPerSecond = m_frameCount / (elapsed / 1000.0);

    m_frameTimer = GetTickCount64();
    m_frameCount = 0;
  }
}

// std::string CameraHires::GetCameraName()
//{
//  TraceyConfig tc(m_config.get());
//  return tc.Get<std::string>(CFG::CAMERA::NAME, defCameraName);
//}
