#include "pch.h"
#include "CameraView.h"

BOOL CCameraView::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
  m_cameraWindow = std::make_unique<CCameraWindow>(*this);
  m_lightsWindow = std::make_unique<CLightsWindow>(*this);

  InitDynamicLayout(false, false);
  return TRUE;
}
