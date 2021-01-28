#pragma once

#include <interfaces/hal/ICamera.h>
#include <libs/CommonLib/TraceyConfig.h>

class IDSVideoCamera;

class CameraHiresImplIDS final
{
public:
  using Mode = ICameraHires::Mode;
  using Callback = std::function<void(const cv::Mat &)>;

  CameraHiresImplIDS();
  ~CameraHiresImplIDS();

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
  std::unique_ptr<IDSVideoCamera> m_camera;
  cv::Mat                         m_recentImg;
  bool                            m_capture{true};
};
