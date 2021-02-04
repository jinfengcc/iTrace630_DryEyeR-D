#pragma once

#include <interfaces/hal/ICamera.h>
#include <libs/CommonLib/TraceyConfig.h>
#include "ids/IDSVideoCamera.h"

#define CAM_HIRES_PRECALC   0

class CameraHiresIDS : public Implements<hal::ICameraHires>
{
public:
  CameraHiresIDS();

  ~CameraHiresIDS()
  {
    StopCapture();
  }

  // Inherited via ICameraHires
  void Initialize(IUnknown *unk) override;
  bool Connect(bool yes) override;

  void StartCapture(Callback notify) override;
  void StopCapture() override;

  void StartFrameTransfer() override;
  void StopFrameTransfer() override;

  bool GetImage(cv::Mat &mat, Mode mode) const override;
  bool Connected(double *fps) const override;

private:
#if USE_OLD_PATIENT_GROUP
  cv::Mat                           m_images[5]; // Hires-color, lores-color, hires-gray, lores-gray, temp
#else
  cv::Mat m_image;
#endif

  std::shared_ptr<IDSVideoCamera>   m_pimpl;
  wil::com_ptr<ITraceyConfig>       m_config;
  Callback                          m_callback;
  sig::SignalId                     m_configSignalId{};
  bool                              m_transferImg{true};
  double                            m_framesPerSecond{};
  int                               m_frameCount{};
  std::int64_t                      m_frameTimer{};

  bool Settings(ITraceyConfig *tc);
  void UpdateImages(const cv::Mat &orig);
  void UpdateFPS();
  void GetLowResGrayImage(cv::Mat &img) const;
};
