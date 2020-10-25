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
  bool Open(std::string_view devName);
  void Close();

  bool Connected(double *fps = nullptr) const
  {
    if (fps)
      *fps = m_fps;
    return m_videoCap.isOpened();
  }

  bool          Settings(ITraceyConfig *tc);
  sig::SignalId StartCapture(std::function<void(cv::Mat)> notify);
  void          StopCapture(sig::SignalId sigId);
  bool          GetImage(cv::Mat &img) const;

private:
  mutable cv::VideoCapture m_videoCap;
  mutable std::mutex       m_mutex;
  std::string              m_devName;
  sig::Signal<cv::Mat>     m_signal;
  std::jthread             m_thread;
  cv::Mat                  m_threadImgs[2];
  int                      m_curThreadImg{};
  double                   m_fps{};

  void DefaultSettings();
  void ThreadFunc(std::stop_token token);
};
