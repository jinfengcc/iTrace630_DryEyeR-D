#include "pch.h"
#include "CameraHiresImplIDS.h"
#include "ids/uEye.h"
#include "ids/IDSVideoCamera.h"

CameraHiresImplIDS::CameraHiresImplIDS()
{
  m_camera = std::make_unique<IDSVideoCamera>([this](const cv::Mat &img) { img.copyTo(m_recentImg); });
}

CameraHiresImplIDS::~CameraHiresImplIDS()
{
  Close();
}

bool CameraHiresImplIDS::Open()
{
  return m_camera->Open();
}

void CameraHiresImplIDS::Close()
{
  m_camera->Close();
}

bool CameraHiresImplIDS::Connected(double *fps) const
{
  return m_camera->IsOpened();
}

bool CameraHiresImplIDS::Connect(bool yes)
{
  throw std::logic_error("The method or operation is not implemented.");
}

bool CameraHiresImplIDS::Settings(ITraceyConfig *tc)
{
  throw std::logic_error("The method or operation is not implemented.");
}

sig::SignalId CameraHiresImplIDS::StartCapture(std::function<void(unsigned)> notify)
{
  throw std::logic_error("The method or operation is not implemented.");
}

void CameraHiresImplIDS::StopCapture(sig::SignalId sigId)
{
  throw std::logic_error("The method or operation is not implemented.");
}

void CameraHiresImplIDS::StartFrameTransfer()
{
  throw std::logic_error("The method or operation is not implemented.");
}

void CameraHiresImplIDS::StopFrameTransfer()
{
  throw std::logic_error("The method or operation is not implemented.");
}

bool CameraHiresImplIDS::GetImage(Mode mode, cv::Mat &img)
{
  throw std::logic_error("The method or operation is not implemented.");
}
