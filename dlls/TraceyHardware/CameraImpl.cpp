#include "pch.h"
#include "CameraImpl.h"
//#include "VideoDecode.h"
//#include <libs/CommonLib/TestSupport/TestDataFolder.h>

using namespace std::chrono_literals;

namespace {
  const int VIDEO_ROWS = thw::VIDEO_FRAME::ROWS;
  const int VIDEO_COLS = thw::VIDEO_FRAME::COLS;

  using SETTINGS = hal::ICamera::SETTINGS;
  static const SETTINGS startupSettings{false, {}, -1, -1, -1, -1};
} // namespace

CameraImpl::CameraImpl(THW *thw)
  : m_empty(cv::Mat::zeros(VIDEO_ROWS, VIDEO_COLS, CV_8UC3))
{
  m_image = m_empty.clone();

  // clang-format off
  thw->GetFunc( "Lynx_IsConnected"            , &m_connectedFunc         );
  thw->GetFunc( "Lynx_SetVideoBrightness"     , &m_setBrightnessFunc     );
  thw->GetFunc( "Lynx_SetVideoContrast"       , &m_setContrastFunc       );
  thw->GetFunc( "Lynx_SetVideoHue"            , &m_setHueFunc            );
  thw->GetFunc( "Lynx_SetVideoSaturation"     , &m_setSaturationFunc     );
  thw->GetFunc( "Lynx_GetMostRecentVideoFrame", &m_getMostRecentFrameFunc);
  // clang-format on

  ShutdownSignal.Connect([this]() { Settings({}); });
}

bool CameraImpl::Connected() const
{
  return Invoke(m_connectedFunc);
}

bool CameraImpl::LoadImage()
{
  if (TransferFrame()) {
    cv::resize(m_frame.image, m_image, {IMG_COLS, IMG_ROWS});
  }

//#ifdef _DEBUG
//  else if (LoadFromFile("tracey_image_1.bin")) {
//    m_image = VideoDecode(m_frame.vframe, false);
//  }
//  else {
//    m_image = m_empty.clone();
//  }
//#endif // _DEBUG

  return !m_image.empty();
}

bool CameraImpl::GetImage(cv::Mat &mat) const
{
  if (m_image.empty())
    return false;

  m_image.copyTo(mat);
  return true;
}

cv::Mat CameraImpl::GetEmptyImage() const
{
  return m_empty;
}

SETTINGS CameraImpl::Settings() const
{
  return m_settings;
}

void CameraImpl::Settings(const SETTINGS &settings)
{
  if (settings.brightness != m_settings.brightness) {
    Invoke(m_setBrightnessFunc, settings.brightness);
    LOG_Diag("Setting Brightness = {}", settings.brightness);
  }

  if (settings.contrast != m_settings.contrast) {
    Invoke(m_setContrastFunc, settings.contrast);
    LOG_Diag("Setting Contrast = {}", settings.contrast);
  }

  if (settings.hue != m_settings.hue) {
    Invoke(m_setHueFunc, settings.hue);
    LOG_Diag("Setting Hue = {}", settings.hue);
  }

  if (settings.saturation != m_settings.saturation) {
    Invoke(m_setSaturationFunc, settings.saturation);
    LOG_Diag("Setting Saturation = {}", settings.saturation);
  }

  m_settings = settings;
}

bool CameraImpl::TransferFrame()
{
  auto color = m_settings.color;

  BOOL available = FALSE;
  if (Connected() && m_getMostRecentFrameFunc != nullptr) {
    ulong StartTime = clock();
    while (!available && clock() - StartTime < 50) {
      if (!Invoke(m_getMostRecentFrameFunc, color, 0, 0, &available, &m_frame.vframe)) {
        LOG_Err("CAMERA::GetMostRecentFunction failed");
        break;
      }
    }
  }
  //else {
  //  color     = TRUE;
  //  available = LoadFromFile("tracey_image_1.bin") ? TRUE : FALSE;
  //}

  m_frame.color     = color;
  m_frame.available = available;
  if (available) {
    m_frame.image = color ? ConvertColor() : ConvertGray();
  }

  return available;
}

//bool CameraImpl::LoadFromFile(const char *fileName)
//{
//  auto     tdf  = GetTestDataFolder();
//  auto     binf = tdf / "camera" / fileName;
//  CAtlFile file;
//  if (file.Create(binf.wstring().c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING) == S_OK) {
//    file.Read(&m_frame.vframe, sizeof(m_frame.vframe));
//    m_frame.color     = TRUE;
//    m_frame.available = TRUE;
//    return true;
//  }
//  return false;
//}

#if 1
cv::Mat CameraImpl::ConvertColor()
{
  // auto &result = m_imageThread;

  // auto result = m_imageThread.clone();

#if 0
    auto func = [&](const auto &pixel, int x, int y) {
      int y1 = pixel[0];
      int cb = pixel[1];
      int y2 = pixel[2];
      int cr = pixel[3];

      result(y, x)     = cv::Vec3b(cb, cr, y1);
      result(y, x + 1) = cv::Vec3b(cb, cr, y2);
    };

    auto *odata = reinterpret_cast<cv::Vec4b *>(&m_vframe.Data[0]);
    auto  odd   = cv::Mat4b(ROWS / 2, COLS / 2, odata);
    odd.forEach([&](const auto &pixel, const int *position) { func(pixel, position[1], 2 * position[0]); });

    auto *edata = reinterpret_cast<cv::Vec4b *>(&m_vframe.Data[2 * (ROWS / 2) * COLS]);
    auto  even  = cv::Mat4b(ROWS / 2, COLS / 2, edata);
    even.forEach([&](const auto &pixel, const int *position) { func(pixel, position[1], 2 * position[0] + 1); });
#else

  cv::Mat result;
  {
    auto *yy = reinterpret_cast<std::uint8_t *>(&m_frame.vframe.Data[0]);
    auto *uv = reinterpret_cast<std::uint8_t *>(&m_frame.vframe.Data[2 * (VIDEO_ROWS / 2) * VIDEO_COLS]);

    cv::Mat xx3;
    auto    xx1 = cv::Mat(VIDEO_ROWS - 1, VIDEO_COLS, CV_8U, yy);
    auto    xx2 = cv::Mat(VIDEO_ROWS / 2, VIDEO_COLS / 2, CV_8UC2, uv);

    cv::cvtColorTwoPlane(xx1, xx2, xx3, cv::COLOR_YUV2BGR_NV21);
    cv::cvtColorTwoPlane(xx1, xx2, xx3, cv::COLOR_YUV2BGR_NV12);

    cv::Mat xy3;
    cv::Mat xy2(VIDEO_ROWS - 1, VIDEO_COLS, CV_8UC2);
    auto    xy1 = cv::Mat(VIDEO_ROWS - 1, VIDEO_COLS, CV_8UC2, yy);

    std::vector<BYTE> buf(VIDEO_COLS * 2);
    for (int row = 0; row < xy1.rows / 2; ++row) {
      auto q = xy1.ptr(row);
      auto p = xy2.ptr(2 * row);

      memcpy(p, q, 2 * VIDEO_COLS);

      q = xy1.ptr(row + xy1.rows / 2);
      p = xy2.ptr(2 * row + 1);

      memcpy(p, q, 2 * VIDEO_COLS);
    }

    cv::cvtColor(xy2, xy3, cv::COLOR_YUV2BGR_YUY2);
    return xy3;

    auto *odata = reinterpret_cast<std::uint8_t *>(&m_frame.vframe.Data[0]);

    // clang-format off
      struct _XXX
      {
        int conv;
        int mtype;
        int rowdiv;
        int coldiv;
      } xxx[] = {
        {cv::COLOR_YUV2BGR_YUY2, CV_8UC2, 1, 1},
        //{cv::COLOR_YUV2BGR_NV21, CV_8UC2, 2, 2},
        //{cv::COLOR_YUV2BGR     , CV_8UC2, 2, 2},
        //{cv::COLOR_YUV2BGR_YV12, CV_8UC2, 2, 2},
        //{cv::COLOR_YUV2BGR_NV12, CV_8UC2, 2, 2},
        {cv::COLOR_YUV2RGB_YUYV, CV_8UC2, 2, 2},
        {cv::COLOR_YUV2RGB_UYVY, CV_8UC2, 2, 2},
        {cv::COLOR_YUV2BGR_Y422, CV_8UC2, 2, 2},
      };
    // clang-format on

    // auto bw = cv::Mat(ROWS, COLS, CV_8U, m_vframe.Data);

    cv::Mat mat;
    for (auto x : xxx) {
      mat = cv::Mat((VIDEO_ROWS - 1) / x.rowdiv, VIDEO_COLS / x.coldiv, x.mtype, odata);
      cv::cvtColor(mat, result, x.conv);
    }

    return result;
  }

  // auto *odata = reinterpret_cast<cv::Vec4b *>(&m_vframe.Data[0]);
  // for (int r = 0; r < result.rows; r += 2) {
  //  for (int c = 0; c < result.rows; c += 2) {
  //    auto pixel = *odata++;
  //    int  y1    = pixel[0];
  //    int  cb    = pixel[1];
  //    int  y2    = pixel[2];
  //    int  cr    = pixel[3];

  //    result(r, c)     = cv::Vec3b(y1, cb, cr);
  //    result(r, c + 1) = cv::Vec3b(y2, cb, cr);
  //  }
  //}
#endif

  cv::flip(result, result, 1);

  cv::cvtColor(result, result, cv::COLOR_YCrCb2BGR);
  return result;
}

cv::Mat CameraImpl::ConvertGray()
{
  // This image is interlaced
  cv::Mat src(VIDEO_ROWS, VIDEO_COLS, CV_8U, m_frame.vframe.Data);
  cv::Mat dst(VIDEO_ROWS, VIDEO_COLS, CV_8U);

  // Copy and de-interlace
  for (int row = 0; row < src.rows / 2; ++row) {
    auto p = src.ptr<uchar>(row);
    auto q = dst.ptr<uchar>(2 * row);
    memcpy(q, p, src.cols);

    p = src.ptr<uchar>(row + src.rows / 2);
    q = dst.ptr<uchar>(2 * row + 1);
    memcpy(q, p, src.cols);
  }

  cv::cvtColor(dst, dst, cv::COLOR_GRAY2BGR);
  return dst;
}
#endif
