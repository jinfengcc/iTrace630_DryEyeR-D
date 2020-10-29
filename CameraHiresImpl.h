#pragma once

#include <libs/CommonLib/TraceyConfig.h>
#include <libs/CommonLib/Singleton.h>

class CameraHires;

class CameraHiResImpl final // : public Singleton<CameraHiResImpl>
{
  friend class CameraHires;

public:
  CameraHiResImpl() = default;
  ~CameraHiResImpl()
  {
    Close();
  }

private:
  using CapProperties = std::array<int, cv::CV__CAP_PROP_LATEST>;

  mutable cv::VideoCapture m_videoCap;
  mutable std::mutex       m_mutex;
  CapProperties            m_capProps;
  std::string              m_devName;
  sig::Signal<cv::Mat>     m_signal;
  std::jthread             m_thread;
  cv::Mat                  m_threadImgs[2];
  int                      m_curThreadImg{};
  double                   m_fps{};
  mutable std::uint64_t    m_tickCount{};

  bool          Open(std::string_view devName);
  void          Close();
  bool          Connected(double *fps = nullptr) const;
  bool          Connect(bool yes);
  bool          Settings(ITraceyConfig *tc);
  sig::SignalId StartCapture(std::function<void(cv::Mat)> notify);
  void          StopCapture(sig::SignalId sigId);
  bool          GetImage(cv::Mat &img) const;
  static double TranslateProp(const GUID &id, double v);
  void          LogProperties() const;
  void          DefaultSettings();
  void          ThreadFunc(std::stop_token token);

  void SetCapProperty(int property, double value)
  {
    m_capProps[property] = static_cast<int>(value);
    m_videoCap.set(property, value);
  }
};

template <typename... Args>
inline void CAMERA_DILASCIA(std::string_view data, Args... args)
{
  DILASCIA_TRACE_EX("HRCAM", data, args...);
}
