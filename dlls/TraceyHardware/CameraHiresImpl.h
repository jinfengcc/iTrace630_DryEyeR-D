#pragma once

#include <interfaces/hal/ICamera.h>
#include <libs/CommonLib/TraceyConfig.h>
#include <libs/CommonLib/Singleton.h>

class CameraHires;

class CameraHiResImpl final // : public Singleton<CameraHiResImpl>
{
  //friend class CameraHires;

  using Mode = ICameraHires::Mode;
public:
  CameraHiResImpl() = default;
  ~CameraHiResImpl()
  {
    Close();
  }

  bool          Open(std::string_view devName);
  void          Close();
  bool          Connected(double *fps = nullptr) const;
  bool          Connect(bool yes);
  bool          Settings(ITraceyConfig *tc);
  sig::SignalId StartCapture(std::function<void(unsigned)> notify);
  void          StopCapture(sig::SignalId sigId);
  bool          GetImage(Mode mode, cv::Mat &img) const;

private:
  using CapProperties = std::array<int, cv::CV__CAP_PROP_LATEST>;

  mutable cv::VideoCapture m_videoCap;
  mutable std::mutex       m_mutex;
  std::jthread             m_thread;
  sig::Signal<unsigned>    m_signal;
  CapProperties            m_capProps{};
  std::string              m_devName;
  cv::Mat                  m_images[4];     // Hires-color, lores-color, hires-gray, lores-gray
  //cv::Mat                  m_colorImage[2];
  //cv::Mat                  m_grayImage;
  // cv::Mat                  m_threadImgs[2];
  //int                      m_curThreadImg{};
  double                   m_fps{};
  mutable std::uint64_t    m_tickCount{};

  static double TranslateProp(int propid, int v);
  bool          SetCapProperty(int propid, int value);
  void LogProperties(int onepropid = -1) const;
  void          DefaultSettings();
  void          ThreadFunc(std::stop_token token);

};

template <typename... Args>
inline void CAMERA_DILASCIA(std::string_view fmt, Args... args)
{
  DILASCIA_TRACE_EX("HRCAM", fmt, args...);
}
