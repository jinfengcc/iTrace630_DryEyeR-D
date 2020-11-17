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
  //cv::Mat img;
  //for (int i = 0; i < 5; ++i)
  //  GetImage(img, Mode::HIRES);
}

bool CameraHires::Connect(bool yes)
{
  if (m_pimpl->Connect(yes)) {
    m_pimpl->Settings(m_config.get());
    return true;
  }
  else {
    return false;
  }
}

void CameraHires::StartCapture(std::function<void(unsigned)> notify)
{
  m_configSignalId = m_pimpl->StartCapture(std::move(notify));
}

void CameraHires::StopCapture()
{
  m_pimpl->StopCapture(m_configSignalId);
  m_configSignalId = 0;
}

bool CameraHires::GetImage(cv::Mat &img, Mode mode) const
{
  const auto color = mode == ICameraHires::Mode::LORES_COLOR || mode == ICameraHires::Mode::HIRES_COLOR;
  if (!m_pimpl->GetImage(mode, img)) {
    CAMERA_DILASCIA( "Unable to get image\n");
    return false;
  }

#if defined(_DEBUG) && 0
  if (fs::exists(R"(C:\1\thanos)"))
    cv::imwrite(R"(C:\1\thanos\hires_ct.png)", img);
#endif

#if defined(_DEBUG) && 0
  cv::Mat tmp;
  cv::cvtColor(img, tmp, cv::COLOR_BGR2GRAY);
  cv::equalizeHist(tmp, tmp);
#endif // _DEBUG


  //if (mode == ICameraHires::Mode::LORES_COLOR || mode == ICameraHires::Mode::LORES_GRAY) {
  //  enum { IMG_ROWS = 468, IMG_COLS = 624 };
  //  cv::resize(img, img, {IMG_COLS, IMG_ROWS}, cv::INTER_AREA);
  //}

  return true;
}

bool CameraHires::Connected(double *fps) const
{
  auto f = 0.0;
  auto c = m_pimpl->Connected(&f);
  if (fps)
    *fps = f;

  return c;
}

std::string CameraHires::GetCameraName()
{
  TraceyConfig tc(m_config.get());
  return tc.Get<std::string>(CFG::CAMERA::NAME, defCameraName);
}
