#include "pch.h"
#include "CameraHiresImpl.h"
#include "CameraHiresProps.h"

// using namespace std::literals;

namespace {
  enum { IMG_ROWS = 468, IMG_COLS = 624 };
  const int HIRES_COLOR_ = static_cast<int>(ICameraHires::Mode::HIRES_COLOR);
  const int LORES_COLOR_ = static_cast<int>(ICameraHires::Mode::LORES_COLOR);
  const int HIRES_GRAY3_ = static_cast<int>(ICameraHires::Mode::HIRES_GRAY);
  const int LORES_GRAY3_ = static_cast<int>(ICameraHires::Mode::LORES_GRAY);
  const int HIRES_GRAY1_ = 4;

} // namespace

struct CameraHiResImpl::Scratch
{
  static_assert(HIRES_GRAY1_ == 4);
  cv::Mat          images[5];
  bool             emulation{false};
  CameraHiResProps camProps;
};

CameraHiResImpl::CameraHiResImpl()
{
  m_scratch = std::make_unique<Scratch>();
}

CameraHiResImpl::~CameraHiResImpl()
{
  Close();
}

bool CameraHiResImpl::Open()
{
  StopCapture(0);
  std::fill(m_capProps.begin(), m_capProps.end(), -10000);

  // m_devName = devName;
  if (Connect(true)) {
    m_videoCap.grab();
  }

  return Connected();
}

void CameraHiResImpl::Close()
{
  m_signal.DisconnectAll(sig::yes_i_am_sure());
  if (m_thread.joinable()) {
    m_thread.request_stop();
    m_thread.join();
  }
  m_videoCap.release();
}

bool CameraHiResImpl::Connected(double *fps) const
{
  if (fps)
    *fps = m_fps;

  return m_videoCap.isOpened();
}

bool CameraHiResImpl::Connect(bool yes)
{
  if (auto prev = m_videoCap.isOpened(); prev != yes) {
    if (yes) {
      m_scratch->camProps.Initialize();
      auto devid = m_scratch->camProps.GetDeviceId();
      if (devid < 0)
        return false;

      m_videoCap.open(devid);
      DefaultSettings();
    }
    else {
      m_videoCap.release();
    }
  }

  return true;
}

bool CameraHiResImpl::Settings(ITraceyConfig *pc)
{
  if (!Connected())
    return false;

  std::array ps = {
    // clang-format off
    std::make_pair(ICameraHires::BRIGHTNESS, cv::CAP_PROP_BRIGHTNESS),
    std::make_pair(ICameraHires::CONTRAST  , cv::CAP_PROP_CONTRAST  ),
    std::make_pair(ICameraHires::SATURATION, cv::CAP_PROP_SATURATION),
    std::make_pair(ICameraHires::HUE       , cv::CAP_PROP_HUE       ),
    std::make_pair(ICameraHires::GAIN      , cv::CAP_PROP_GAIN      ),
    //std::make_pair(ICameraHires::EXPOSURE  , cv::CAP_PROP_EXPOSURE  ),
    // clang-format on
  };

  TraceyConfig c(pc);
  for (auto &p : ps) {
    if (auto v = c.Get<int>(p.first); v.has_value()) {
      SetCapProperty(p.second, v.value());
    }
  }

  // LogProperties();
  return true;
}

sig::SignalId CameraHiResImpl::StartCapture(std::function<void(unsigned)> notify)
{
  if (m_thread.joinable()) {
    return {};
  }

  auto id  = notify ? m_signal.Connect(std::move(notify)) : 0;
  m_thread = std::jthread([this](std::stop_token token) { ThreadFunc(token); });

  return id;
}

void CameraHiResImpl::StopCapture(sig::SignalId sigId)
{
  if (sigId)
    m_signal.Disconnect(sigId);

  if (m_thread.joinable()) {
    m_thread.request_stop();
    m_thread.join();
  }
}

void CameraHiResImpl::StartFrameTransfer()
{
  m_scratch->emulation = true;
  ReadImage();
}

void CameraHiResImpl::StopFrameTransfer()
{
  // do nothing
}

bool CameraHiResImpl::GetImage(Mode mode, cv::Mat &img)
{
  auto &i = m_images[static_cast<int>(mode)];
  if (m_scratch->emulation || m_thread.joinable()) {
    std::lock_guard lock(m_mutex);
    if (i.empty())
      return false;

    i.copyTo(img);
    return true;
  }
  else {
    if (!ReadImage() || i.empty())
      return false;

    i.copyTo(img);
    return true;
  }
}

void CameraHiResImpl::DefaultSettings()
{
  if (Connected()) {
    DumpSettings("before setup", m_videoCap);

    auto fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');

    // clang-format off
    SetCapProperty(cv::CAP_PROP_FOURCC       , fourcc);
    SetCapProperty(cv::CAP_PROP_FRAME_WIDTH  , m_scratch->camProps.GetResolution().cx);
    SetCapProperty(cv::CAP_PROP_FRAME_HEIGHT , m_scratch->camProps.GetResolution().cy);
    SetCapProperty(cv::CAP_PROP_EXPOSURE     , 0);
    SetCapProperty(cv::CAP_PROP_FPS          , 30);

    //SetCapProperty( cv::CAP_PROP_BRIGHTNESS, 100);
    //SetCapProperty( cv::CAP_PROP_CONTRAST  , 10);
    //SetCapProperty( cv::CAP_PROP_SATURATION, 70);
    //SetCapProperty( cv::CAP_PROP_HUE       , 128);

    //SetCapProperty(cv::CAP_PROP_GAIN      , 100);
    //SetCapProperty(cv::CAP_PROP_ISO_SPEED , 400);
    //SetCapProperty(cv::CAP_PROP_BACKLIGHT , 0);
    //SetCapProperty(cv::CAP_PROP_BUFFERSIZE, 2);
    // clang-format on

    DumpSettings("after setup", m_videoCap);
  }
}

bool CameraHiResImpl::SetCapProperty(int propid, int value)
{
  if (propid == cv::CAP_PROP_EXPOSURE) {
    if (value) {
      m_videoCap.set(cv::CAP_PROP_EXPOSURE, value);
    }
    else {
      // Somehow going to auto does not work if the manual was set to long exposure.
      // So, step manually to a low setting and then set auto.
      const auto stepForAuto = static_cast<int>(ICameraHires::Exposure::_5ms);
      m_videoCap.set(cv::CAP_PROP_EXPOSURE, stepForAuto);
      m_videoCap.set(cv::CAP_PROP_AUTO_EXPOSURE, 1);
    }
  }
  else if (m_capProps[propid] != value) {
    m_videoCap.set(propid, m_scratch->camProps.TranslatePropery(propid, value));
  }
  else {
    return false;
  }

  m_capProps[propid] = value;
  LogProperties(propid);
  return true;
}

void CameraHiResImpl::ThreadFunc(std::stop_token token)
{
  auto fpc = 0;
  auto now = GetTickCount64();

  for (unsigned n = 0; !token.stop_requested(); ++n) {
    if (!m_videoCap.isOpened()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      continue;
    }

    if (ReadImage())
      m_signal(n);

    // Frames per second
    ++fpc;
    if (auto elapsed = GetTickCount64() - now; elapsed >= 2000) {
      m_fps = fpc / (elapsed / 1000.0);

      DILASCIA_TRACE_EX("CAM_FPS", "FPS = {:.2f} ({})\n", m_fps, fpc);
      now = GetTickCount64();
      fpc = 0;
    }
  }
}

bool CameraHiResImpl::ReadImage()
{
  auto &colorImg = m_scratch->images[HIRES_COLOR_];
  auto &grayImg3 = m_scratch->images[HIRES_GRAY3_];
  auto &grayImg1 = m_scratch->images[HIRES_GRAY1_];

  bool ok = m_videoCap.retrieve(colorImg);
  m_videoCap.grab();

  if (ok && !colorImg.empty()) {
    cv::cvtColor(colorImg, grayImg1, cv::COLOR_BGR2GRAY);
    cv::cvtColor(grayImg1, grayImg3, cv::COLOR_GRAY2BGR);

    cv::resize(colorImg, m_scratch->images[LORES_COLOR_], {IMG_COLS, IMG_ROWS});
    cv::resize(grayImg3, m_scratch->images[LORES_GRAY3_], {IMG_COLS, IMG_ROWS});

    {
      std::lock_guard lock(m_mutex);
      for (unsigned i = 0; i < _countof(m_images); ++i) {
        //m_images[i] = m_scratch->images[i].clone();
        m_scratch->images[i].copyTo(m_images[i]);
      }
    }

    return true;
  }
  else {
    CAMERA_DILASCIA("Unable to read hi-res image\n");
    return false;
  }
}

void CameraHiResImpl::LogProperties(int onepropid /*= -1*/) const
{
  std::array ps = {
    // clang-format off
    std::make_pair( cv::CAP_PROP_FRAME_WIDTH  , "WIDTH"),
    std::make_pair( cv::CAP_PROP_FRAME_HEIGHT , "HEIGHT"),
    std::make_pair( cv::CAP_PROP_BRIGHTNESS   , "BRIGHTNESS"),
    std::make_pair( cv::CAP_PROP_CONTRAST     , "CONTRAST"),
    std::make_pair( cv::CAP_PROP_HUE          , "HUE"),
    std::make_pair( cv::CAP_PROP_SATURATION   , "SATURATION"),
    std::make_pair( cv::CAP_PROP_GAIN         , "GAIN"),
    std::make_pair( cv::CAP_PROP_EXPOSURE     , "EXPOSURE"),
    std::make_pair( cv::CAP_PROP_AUTO_EXPOSURE, "AUTO_EXP."),
    // clang-format on
  };

  for (auto &p : ps) {
    if (onepropid >= 0 && onepropid != p.first)
      continue;

    if (p.first == cv::CAP_PROP_EXPOSURE) {
      std::array exposure = {
        "Auto",  "640 ms", "320 ms",  "160 ms",  "80 ms",  "40 ms",  "20 ms",
        "10 ms", "5 ms",   "2.50 ms", "1.25 ms", "650 us", "312 us", "150 us",
      };

      unsigned ndx = -m_capProps[cv::CAP_PROP_EXPOSURE];
      auto     txt = ndx < exposure.size() ? exposure[ndx] : "???";

      CAMERA_DILASCIA("{:<10} = {}\n", p.second, txt);
    }
    else {
      CAMERA_DILASCIA("{:<10} = {:4} ({:5.0f})\n", p.second, m_capProps[p.first], m_videoCap.get(p.first));
    }
  }
}

void CameraHiResImpl::DumpSettings(const char *title, cv::VideoCapture &vc)
{
  static std::array names = {
    "CAP_PROP_POS_MSEC            ", "CAP_PROP_POS_FRAMES          ", "CAP_PROP_POS_AVI_RATIO       ", "CAP_PROP_FRAME_WIDTH         ",
    "CAP_PROP_FRAME_HEIGHT        ", "CAP_PROP_FPS                 ", "CAP_PROP_FOURCC              ", "CAP_PROP_FRAME_COUNT         ",
    "CAP_PROP_FORMAT              ", "CAP_PROP_MODE                ", "CAP_PROP_BRIGHTNESS          ", "CAP_PROP_CONTRAST            ",
    "CAP_PROP_SATURATION          ", "CAP_PROP_HUE                 ", "CAP_PROP_GAIN                ", "CAP_PROP_EXPOSURE            ",
    "CAP_PROP_CONVERT_RGB         ", "CAP_PROP_WHITE_BALANCE_BLUE_U", "CAP_PROP_RECTIFICATION       ", "CAP_PROP_MONOCHROME          ",
    "CAP_PROP_SHARPNESS           ", "CAP_PROP_AUTO_EXPOSURE       ", "CAP_PROP_GAMMA               ", "CAP_PROP_TEMPERATURE         ",
    "CAP_PROP_TRIGGER             ", "CAP_PROP_TRIGGER_DELAY       ", "CAP_PROP_WHITE_BALANCE_RED_V ", "CAP_PROP_ZOOM                ",
    "CAP_PROP_FOCUS               ", "CAP_PROP_GUID                ", "CAP_PROP_ISO_SPEED           ", "CAP_PROP_BACKLIGHT           ",
    "CAP_PROP_PAN                 ", "CAP_PROP_TILT                ", "CAP_PROP_ROLL                ", "CAP_PROP_IRIS                ",
    "CAP_PROP_SETTINGS            ", "CAP_PROP_BUFFERSIZE          ", "CAP_PROP_AUTOFOCUS           ", "CAP_PROP_SAR_NUM             ",
    "CAP_PROP_SAR_DEN             ", "CAP_PROP_BACKEND             ", "CAP_PROP_CHANNEL             ", "CAP_PROP_AUTO_WB             ",
    "CAP_PROP_WB_TEMPERATURE      ", "CAP_PROP_CODEC_PIXEL_FORMAT  ", "CAP_PROP_BITRATE             ",
  };

  DILASCIA_TRACE_EX("CAMERA", fmt::format("------- {} -----------\n", title).c_str());
  for (int i = 0; i < 47; ++i) {
    auto x = vc.get(i);
    DILASCIA_TRACE_EX("CAMERA", fmt::format("{} = {}\n", names[i], (int)x).c_str());
  }
}
