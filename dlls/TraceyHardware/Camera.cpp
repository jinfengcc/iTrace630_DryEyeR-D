#include "pch.h"
#include "Camera.h"
#include "CameraImpl.h"

CameraImpl *Camera::m_pimpl;

Camera::Camera(THW *thw)
  : Implements<hal::ICamera>("ICamera")
  , m_thw(thw)
{
  if (!m_pimpl)
    m_pimpl = new CameraImpl(thw);
}

bool Camera::LoadImage()
{
  return Power() && m_pimpl->LoadImage();
}

bool Camera::GetImage(cv::Mat &mat) const
{
  return Power() && m_pimpl->GetImage(mat);
}

bool Camera::Connected() const
{
  return m_pimpl->Connected();
}

bool Camera::Power() const
{
  return m_thw->Power(thw::POWER::VIDEO_CAMERA);
}

void Camera::Power(bool power)
{
  m_thw->Power(thw::POWER::VIDEO_CAMERA, power);
}

auto Camera::Settings() const -> SETTINGS
{
  return m_pimpl->Settings();
}

void Camera::Settings(const SETTINGS &settings)
{
  m_pimpl->Settings(settings);
}
