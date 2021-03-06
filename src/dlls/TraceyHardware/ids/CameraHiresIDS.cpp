#include "pch.h"
#include "CameraHiresIDS.h"
#include <interfaces/ITraceyConfig.h>

namespace {
  enum { IMG_ROWS = 468, IMG_COLS = 624 };

#if CAM_HIRES_PRECALC
  const int HIRES_COLOR_ = static_cast<int>(ICameraHires::Mode::HIRES_COLOR);
  const int LORES_COLOR_ = static_cast<int>(ICameraHires::Mode::LORES_COLOR);
  const int HIRES_GRAY3_ = static_cast<int>(ICameraHires::Mode::HIRES_GRAY);
  const int LORES_GRAY3_ = static_cast<int>(ICameraHires::Mode::LORES_GRAY);
  const int HIRES_GRAY1_ = 4;
#endif
} // namespace

CameraHiresIDS::CameraHiresIDS()
  : Implements<hal::ICameraHires>("ICameraHires")
{
  static std::weak_ptr<IDSVideoCamera> camImpl;
  static std::mutex                    mutex;

  std::lock_guard lock(mutex);
  if (m_pimpl = camImpl.lock(); !m_pimpl) {
    m_pimpl = std::make_shared<IDSVideoCamera>([this](const cv::Mat &img) {
      bool copyImage = false;
      if (copyImage) {
        static int ndx    = 0;
        const char file[] = R"(C:\1\_ids\ids_image_{}.png)";
        cv::imwrite(fmt::format(file, ++ndx), img);
      }

      UpdateFPS();
      if (m_transferImg) {
        UpdateImages(img);
        if (m_callback)
          m_callback(0);

        {
          std::unique_lock lock(m_transferMutex);
          ++m_captureCount;
        }
        m_transferCondition.notify_one();
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
  if (m_transferImg) {
    std::unique_lock locker(m_transferMutex);
    auto             capture = m_captureCount;
    m_transferCondition.wait(locker, [&, this]() { return m_captureCount != capture; });

    m_transferImg = false;
#ifdef _DEBUG
    // We need to slow down for miguel's machine
    Sleep(500);
#endif
  }
}

bool CameraHiresIDS::GetImage(cv::Mat &img, Mode mode) const
{
#if CAM_HIRES_PRECALC
  if (auto &i = m_images[static_cast<int>(mode)]; i.empty())
    return false;
  else {
    i.copyTo(img);
    return true;
  }
#else
  if (m_image.empty())
    return false;

  switch (mode) {
  case Mode::HIRES_COLOR:
    m_image.copyTo(img);
    break;
  case Mode::LORES_COLOR:
    cv::resize(m_image, img, {IMG_COLS, IMG_ROWS});
    break;
  case Mode::HIRES_GRAY:
    cv::cvtColor(m_image, img, cv::COLOR_BGR2GRAY);
    break;
  case Mode::LORES_GRAY:
    GetLowResGrayImage(img);
    break;

  default:
    return false;
  }

  return !img.empty();
#endif
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
  using Prop = IDSVideoCamera::Prop;

  std::array ps = {
    // clang-format off
    std::make_pair(ICameraHires::EXPOSURE  , Prop::exposure),
    std::make_pair(ICameraHires::GAIN      , Prop::gain    ),
    std::make_pair(ICameraHires::GAIN_RED  , Prop::red     ),
    std::make_pair(ICameraHires::GAIN_GREEN, Prop::green   ),
    std::make_pair(ICameraHires::GAIN_BLUE , Prop::blue    ),
    // clang-format on
  };

  TraceyConfig c(tc);
  for (auto &p : ps) {
    if (auto v = c.Get<int>(p.first); v.has_value()) {
      m_pimpl->SetProperty(p.second, v.value());
    }
  }

  return true;
}

void CameraHiresIDS::UpdateImages(const cv::Mat &orig)
{
#if CAM_HIRES_PRECALC
  auto &colorImg = m_images[HIRES_COLOR_];
  auto &grayImg3 = m_images[HIRES_GRAY3_];
  auto &grayImg1 = m_images[HIRES_GRAY1_];

  orig.copyTo(colorImg);

  cv::cvtColor(colorImg, grayImg1, cv::COLOR_BGR2GRAY);
  cv::cvtColor(grayImg1, grayImg3, cv::COLOR_GRAY2BGR);

  cv::resize(colorImg, m_images[LORES_COLOR_], {IMG_COLS, IMG_ROWS});
  cv::resize(grayImg3, m_images[LORES_GRAY3_], {IMG_COLS, IMG_ROWS});
#else
  orig.copyTo(m_image);
#endif
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

/////////////////////////////////////////////////////////////////////////////////////////

#define GRAY_MODE_BLUE  0
#define GRAY_MODE_GREEN 1
#define GRAY_MODE_RED   2
#define GRAY_MODE_STD   3

#define GRAY_MODE       GRAY_MODE_STD

void CameraHiresIDS::GetLowResGrayImage(cv::Mat &img) const
{
  cv::resize(m_image, img, {IMG_COLS, IMG_ROWS});
  ATLASSERT(img.type() == CV_8UC3);

#if GRAY_MODE == GRAY_MODE_STD
  cv::Mat3b(img).forEach([](cv::Vec3b &pixel, const int *) {
    auto B = pixel[0];
    auto G = pixel[1];
    auto R = pixel[2];

    auto gray = 0.299 * R + 0.587 * G + 0.114 * B;

    pixel[0] = pixel[1] = pixel[2] = cv::saturate_cast<BYTE>(gray);
  });
#else
  cv::Mat3b(img).forEach([](cv::Vec3b &pixel, const int *) {
    auto gray = pixel[GRAY_MODE];
    pixel[0] = pixel[1] = pixel[2] = gray;
  });
#endif

}

// std::string CameraHires::GetCameraName()
//{
//  TraceyConfig tc(m_config.get());
//  return tc.Get<std::string>(CFG::CAMERA::NAME, defCameraName);
//}
