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
  std::fill(m_capProps.begin(), m_capProps.end(), 0.0);

  Connect(true);
  m_devName = devName.empty() ? defCameraName : devName;
  m_videoCap.release();
  if (auto devid = hrc::GetDeviceId(m_devName); devid >= 0)
    m_videoCap.open(devid);

  DefaultSettings();

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
  if (m_devName.empty())
    return false;

  auto devid = hrc::GetDeviceId(m_devName);
  if (devid < 0)
    return false;

  auto prev = m_videoCap.isOpened();
  if (yes != prev) {
    if (yes)
      m_videoCap.open(devid);
    else
      m_videoCap.release();
  }

  return prev;
}

bool CameraHiResImpl::Settings(ITraceyConfig *pc)
{
  if (!Connected())
    return false;

  std::array ps = {
    // clang-format off
    std::make_pair(ICameraHires::BRIGHTNESS, cv::CAP_PROP_BRIGHTNESS),
    std::make_pair(ICameraHires::CONTRAST  , cv::CAP_PROP_CONTRAST  ),
    std::make_pair(ICameraHires::HUE       , cv::CAP_PROP_HUE       ),
    std::make_pair(ICameraHires::SATURATION, cv::CAP_PROP_SATURATION),
    std::make_pair(ICameraHires::GAIN      , cv::CAP_PROP_GAIN      ),
    std::make_pair(ICameraHires::EXPOSURE  , cv::CAP_PROP_EXPOSURE  ),
    // clang-format on
  };

  TraceyConfig c(pc);
  for (auto &p : ps) {
    if (auto v = c.Get<int>(p.first); v.has_value()) {
      SetCapProperty(p.second, TranslateProp(p.first, v.value()));
      if (p.first == ICameraHires::EXPOSURE) {
        if (v.value() == 0)
          SetCapProperty(cv::CAP_PROP_AUTO_EXPOSURE, 1);
      }
    }
  }

  LogProperties();
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
    if (auto &i = m_threadImgs[1 - m_curThreadImg]; !i.empty()) {
      i.copyTo(img);
      return true;
    }
    else {
      return false;
    }
  }
  else {
    LogProperties();
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
    SetCapProperty(cv::CAP_PROP_GAIN         , 100);
    // SetCapProperty(cv::CAP_PROP_ISO_SPEED , 400);
    // SetCapProperty(cv::CAP_PROP_BACKLIGHT , 0);
    // SetCapProperty(cv::CAP_PROP_BUFFERSIZE, 2);
    // clang-format on

    hrc::DumpSettings("after setup", m_videoCap);
  }
}

void CameraHiResImpl::ThreadFunc(std::stop_token token)
{
  auto ndx = [this]() -> int {
    std::lock_guard lock(m_mutex);
    m_curThreadImg = ++m_curThreadImg & 1;
    return m_curThreadImg;
  };

  auto now = std::chrono::steady_clock::now();

  for (unsigned n = 0; !token.stop_requested(); ++n) {
    if (!m_videoCap.isOpened()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      continue;
    }

    auto &img = m_threadImgs[ndx()];
    if (!m_videoCap.read(img)) {
      CAMERA_DILASCIA("Unable to read hi-res image\n");
      continue;
    }

    m_signal(img);

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
double CameraHiResImpl::TranslateProp(const GUID &id, double v)
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
    GUID   id;
    double minValue;
    double maxValue;
  };

  static PropRange propRange[] = {
    // clang-format off
      { ICameraHires::BRIGHTNESS,   -64,   64},  // Range [  -64,   64]
      { ICameraHires::CONTRAST  ,     0,   95},  // Range [    0,   95]
      { ICameraHires::HUE       ,  -128,  128},  // Range [-2000, 2000]
      { ICameraHires::SATURATION,     0,  128},  // Range [    0,  128]
      { ICameraHires::GAIN      ,     0,  100},  // Range [    0,  100]
    // clang-format on
  };

  auto i = std::find_if(std::begin(propRange), std::end(propRange), [&](const auto &x) { return x.id == id; });
  if (i != std::end(propRange)) {
    v = i->minValue + (v / 256.0) * (i->maxValue - i->minValue);
  }
  return std::round(v);
}

void CameraHiResImpl::LogProperties() const
{
  std::array ps = {
    // clang-format off
    std::make_pair( cv::CAP_PROP_BRIGHTNESS, "BRIGHTNESS"),
    std::make_pair( cv::CAP_PROP_CONTRAST  , "CONTRAST"),
    std::make_pair( cv::CAP_PROP_HUE       , "HUE"),
    std::make_pair( cv::CAP_PROP_SATURATION, "SATURATION"),
    std::make_pair( cv::CAP_PROP_GAIN      , "GAIN"),
    std::make_pair( cv::CAP_PROP_EXPOSURE  , "EXPOSURE"),
    // clang-format on
  };

  for (auto &p : ps)
    CAMERA_DILASCIA("{:<10} = {:4.1},{:4.1}\n", p.second, m_videoCap.get(p.first), m_capProps[p.first]);
}
