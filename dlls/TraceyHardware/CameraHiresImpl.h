#pragma once

#include <interfaces/hal/ICamera.h>
#include <libs/CommonLib/TraceyConfig.h>
#include <libs/CommonLib/Singleton.h>

class CameraHiResImpl final // : public Singleton<CameraHiResImpl>
{
  using Mode = ICameraHires::Mode;

public:
  CameraHiResImpl();
  ~CameraHiResImpl();

  bool          Open();
  void          Close();
  bool          Connected(double *fps = nullptr) const;
  bool          Connect(bool yes);
  bool          Settings(ITraceyConfig *tc);
  sig::SignalId StartCapture(std::function<void(unsigned)> notify);
  void          StopCapture(sig::SignalId sigId);
  void          StartFrameTransfer();
  void          StopFrameTransfer();
  bool          GetImage(Mode mode, cv::Mat &img);

private:
  using CapProperties = std::array<int, cv::CV__CAP_PROP_LATEST>;
  using VideoCapturePtr = std::unique_ptr<cv::VideoCapture>;

  struct Scratch;
  std::unique_ptr<Scratch> m_scratch;
  mutable VideoCapturePtr  m_videoCap;
  mutable std::mutex       m_mutex;
  std::jthread             m_thread;
  sig::Signal<unsigned>    m_signal;
  CapProperties            m_capProps{};
  std::string              m_devName;
  cv::Mat                  m_images[4]; // Hires-color, lores-color, hires-gray, lores-gray
  double                   m_fps{};

  void        DefaultSettings();
  bool        SetCapProperty(int propid, int value);
  void        ThreadFunc(std::stop_token token);
  bool        ReadImage();
  void        LogProperties(int onepropid = -1) const;
  static void DumpSettings(const char *title, cv::VideoCapture &vc);
};

template <typename... Args>
inline void CAMERA_DILASCIA(std::string_view fmt, Args... args)
{
  DILASCIA_TRACE_EX("HRCAM", fmt, args...);
}
