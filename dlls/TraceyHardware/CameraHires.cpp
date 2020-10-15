#include "pch.h"
#include "CameraHires.h"
#include "CameraHiresImpl.h"
#include <interfaces/ITraceyConfig.h>

namespace {
  const char defCameraName[] = "HD USB Camera";
}

CameraHires::CameraHires()
  : Implements<hal::ICameraHires>("ICameraHires")
{
  static std::weak_ptr<CameraHiResImpl> camImpl;
  static std::mutex                     mutex;

  std::lock_guard lock(mutex);
  if (m_pimpl = camImpl.lock(); !m_pimpl) {
    m_pimpl = std::make_shared<CameraHiResImpl>();
    m_pimpl->Open(defCameraName);
    camImpl = m_pimpl;
  }
}

void CameraHires::Initialize(IUnknown *unk)
{
  m_config.reset();
  if (unk) {
    if (m_config = wil::com_query<ITraceyConfig>(unk))
      m_configSignalId = m_config->OnDataChanged([this]() { m_pimpl->Settings(m_config.get()); });
  }

  m_pimpl->Open(GetCameraName());
  m_pimpl->Settings(m_config.get());

  // Prime the camera
  cv::Mat img;
  for (int i = 0; i < 5; ++i)
    GetImage(img, Mode::HIRES);
}

bool CameraHires::StartCapture(std::function<void(cv::Mat)> notify)
{
  m_configSignalId = m_pimpl->StartCapture(std::move(notify));
  return m_configSignalId != 0;
}

void CameraHires::StopCapture()
{
  m_pimpl->StopCapture(m_configSignalId);
  m_configSignalId = 0;
}

bool CameraHires::GetImage(cv::Mat &img, Mode mode) const
{
  if (!m_pimpl->GetImage(img))
    return false;

#ifdef _DEBUG
  if (fs::exists(R"(C:\1\thanos)"))
    cv::imwrite(R"(C:\1\thanos\hires_ct.png)", img);
#endif

  if (mode == ICameraHires::Mode::LORES) {
    enum { IMG_ROWS = 468, IMG_COLS = 624 };
    cv::resize(img, img, {IMG_COLS, IMG_ROWS}, cv::INTER_AREA);
  }

  return true;
}

bool CameraHires::Connected() const
{
  return m_pimpl->Connected();
}

std::string CameraHires::GetCameraName()
{
  TraceyConfig tc(m_config.get());
  return tc.Get<std::string>(CFG::CAMERA::NAME, defCameraName);
}
