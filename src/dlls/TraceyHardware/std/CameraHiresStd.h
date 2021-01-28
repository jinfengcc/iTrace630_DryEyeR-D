#pragma once

#include <interfaces/hal/ICamera.h>
#include "CameraHiresImplStd.h"

class CameraHiresStd : public Implements<hal::ICameraHires>
{
public:
  CameraHiresStd();

  ~CameraHiresStd()
  {
    StopCapture();
  }

  // Inherited via ICameraHires
  void Initialize(IUnknown *unk) override;
  bool Connect(bool yes) override;

  void StartCapture(std::function<void(unsigned)> notify) override;
  void StopCapture() override;

  void StartFrameTransfer() override;
  void StopFrameTransfer() override;

  bool GetImage(cv::Mat &mat, Mode mode) const override;
  bool Connected(double *fps) const override;

private:
  std::shared_ptr<CameraHiresImplStd> m_pimpl;
  wil::com_ptr<ITraceyConfig>         m_config;
  sig::SignalId                       m_configSignalId{};
  bool                                m_transferImg{true};

  // std::string GetCameraName();
};
