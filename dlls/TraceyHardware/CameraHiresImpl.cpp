#include "pch.h"
#include "CameraHiresImpl.h"
#include "CameraHiresUtil.h"

using namespace std::literals;

namespace {
  const char defCameraName[] = "HD USB Camera";

} // namespace

bool CameraHiResImpl::Open(std::string_view devName)
{
  StopCapture(0);
  std::fill(m_capProps.begin(), m_capProps.end(), -10000);

  m_devName = devName.empty() ? defCameraName : devName;
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
    if (!yes) {
      m_videoCap.release();
    }
    else if (auto devid = hrc::GetDeviceId(m_devName); devid >= 0) {
      m_videoCap.open(devid);
      DefaultSettings();
    }
    else {
      return false;
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

sig::SignalId CameraHiResImpl::StartCapture(std::function<void(cv::Mat)> notify)
{
  auto id  = m_signal.Connect(std::move(notify));
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

bool CameraHiResImpl::GetImage(cv::Mat &img) const
{
  if (m_thread.joinable()) {
    std::lock_guard lock(m_mutex);
    m_image.copyTo(img);
    return !m_image.empty();
  }
  else {
    if (GetTickCount64() - m_tickCount > 5000) {
      m_tickCount = GetTickCount64();
      LogProperties();
    }

    return m_videoCap.read(img);
  }
}

void CameraHiResImpl::DefaultSettings()
{
  if (Connected()) {
    hrc::DumpSettings("before setup", m_videoCap);

    auto fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');

    // clang-format off
    SetCapProperty(cv::CAP_PROP_FOURCC       , fourcc);
    SetCapProperty(cv::CAP_PROP_FRAME_WIDTH  , 1280);
    SetCapProperty(cv::CAP_PROP_FRAME_HEIGHT , 960);
    SetCapProperty(cv::CAP_PROP_AUTO_EXPOSURE, 1);
    // SetCapProperty(cv::CAP_PROP_EXPOSURE     , 0);
    // SetCapProperty(cv::CAP_PROP_GAIN         , 100);
    // SetCapProperty(cv::CAP_PROP_ISO_SPEED , 400);
    // SetCapProperty(cv::CAP_PROP_BACKLIGHT , 0);
    // SetCapProperty(cv::CAP_PROP_BUFFERSIZE, 2);
    // clang-format on

    hrc::DumpSettings("after setup", m_videoCap);
  }
}

void CameraHiResImpl::ThreadFunc(std::stop_token token)
{
  //auto ndx = [this]() -> int {
  //  std::lock_guard lock(m_mutex);
  //  m_curThreadImg = ++m_curThreadImg & 1;
  //  return m_curThreadImg;
  //};

  auto now = std::chrono::steady_clock::now();

  cv::Mat img;
  for (unsigned n = 0; !token.stop_requested(); ++n) {
    if (!m_videoCap.isOpened()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      continue;
    }

    if (m_videoCap.read(img)) {
      //m_signal(img);
    }
    else {
      CAMERA_DILASCIA("Unable to read hi-res image\n");
    }

    if (!img.empty()) {
      std::lock_guard lock(m_mutex);
      img.copyTo(m_image);
    }

    // Frames per second
    auto t = std::chrono::steady_clock::now();
    if (auto d = std::chrono::duration_cast<std::chrono::milliseconds>(t - now); d >= 2000ms) {
      m_fps = n / (d.count() / 1000.0);

      now = t;
      n   = 0;
    }
  }
}

// v is 0...255
double CameraHiResImpl::TranslateProp(int propid, int v)
{
  /*****************

  Control                             Reg.            Current         Min.          Max.
  Control Undefined                   0x00               0             0             0
  Back light Compensation             0x01               3             0             3
  Brightness                          0x02              64           -64            64
  Contrast                            0x03              95             0            95
  Gain                                0x04             100             0           100
  Power Line Frequency                0x05               2             0             2
  Hue                                 0x06            2000         -2000          2000
  Saturation                          0x07             128             0           128
  Gamma                               0x08               7             1             7
  White Balance Temperature           0x09             300           100           300
  White Balance Temperature (Auto)    0x0a            6500          2800          6500
                                      0x0b               0             0             0
  ******************/

  struct PropRange
  {
    int    id;
    double minValue;
    double maxValue;
  };

  static PropRange propRange[] = {
    // clang-format off
    { cv::CAP_PROP_BRIGHTNESS,   -64,   64},  // Range [  -64,   64]
    { cv::CAP_PROP_CONTRAST  ,     0,   95},  // Range [    0,   95]
    { cv::CAP_PROP_HUE       ,  -128,  128},  // Range [-2000, 2000]
    { cv::CAP_PROP_SATURATION,     0,  128},  // Range [    0,  128]
    { cv::CAP_PROP_GAIN      ,     0,  100},  // Range [    0,  100]
    // clang-format on
  };

  double x = v;

  auto i = std::find_if(std::begin(propRange), std::end(propRange), [propid](const auto &x) { return x.id == propid; });
  if (i != std::end(propRange)) {
    x = i->minValue + (v / 256.0) * (i->maxValue - i->minValue);
  }
  return std::round(x);
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
    m_videoCap.set(propid, TranslateProp(propid, value));
  }
  else {
    return false;
  }

  m_capProps[propid] = value;
  LogProperties(propid);
  return true;
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
