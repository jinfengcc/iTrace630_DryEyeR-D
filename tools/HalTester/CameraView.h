#pragma once

#include "resource.h"
#include "DialogWithColor.h"
#include "CameraWindow.h"
#include "LightsWindow.h"

class CCameraView
  : public CDialogWithColor<CCameraView>
  , public CDynamicDialogLayout<CCameraView>
  , public IView
{
public:
  CCameraView()
  {
  }

  enum { IDD = IDD_VIEW_CAMEARA };

private:
  // IView
  HWND GetHWnd() const override
  {
    return m_hWnd;
  }
  bool OnHide(bool) override
  {
    m_cameraWindow->Stop();
    return TRUE;
  }
  void OnShow() override
  {
    m_cameraWindow->Start();
  }

private:
  std::unique_ptr<CCameraWindow> m_cameraWindow;
  std::unique_ptr<CLightsWindow> m_lightsWindow;

  BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);

  BEGIN_MSG_MAP_EX(CCameraView)
    MSG_WM_INITDIALOG(OnInitDialog)
    CHAIN_MSG_MAP(CDialogWithColor<CCameraView>)
    CHAIN_MSG_MAP(CDynamicDialogLayout<CCameraView>)
    CHAIN_MSG_MAP_MEMBER_PTR(m_cameraWindow);
    CHAIN_MSG_MAP_MEMBER_PTR(m_lightsWindow);
  END_MSG_MAP()
};
