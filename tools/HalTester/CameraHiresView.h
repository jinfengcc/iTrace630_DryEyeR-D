#pragma once

#include "resource.h"
#include "DialogWithColor.h"
#include "CameraWindow.h"
#include "LightsWindow.h"
#include "WindowPaintImpl.h"
#include <interfaces/hal/ICamera.h>
#include <libs/CommonLib/TraceyConfigImpl.h>

class CCameraHiresView
  : public CDialogWithColor<CCameraHiresView>
  , public CDynamicDialogLayout<CCameraHiresView>
  , public CWinDataExchange<CCameraHiresView>
  , public IView
{
public:
  CCameraHiresView() = default;

  enum { IDD = IDD_VIEW_CAMEARA_HIRES };

private:
  // IView
  HWND GetHWnd() const override
  {
    return m_hWnd;
  }
  bool OnHide(bool) override;
  void OnShow() override;
  void OnTerminating() override;

private:
  CWindowPaintImpl                  m_cameraWnd;
  TraceyConfigImpl                  m_config;
  wil::com_ptr_t<hal::ICameraHires> m_camera;
  bool                              m_colorImg{true};
  std::mutex                        m_mutex;
  //cv::Mat                           m_image;
  std::unique_ptr<CLightsWindow>    m_lightsWindow;

  BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
  void OnPopupMore(UINT uNotifyCode, int nID, CWindow wndCtl);
  void OnEditChanged(UINT uNotifyCode, int nID, CWindow wndCtl);
  void OnExposureChanged(UINT uNotifyCode, int nID, CWindow wndCtl);

  void    InitializeSpinners();
  void    InitializeCombo();
  void    CreateObjects();
  void    UpdateFPS();
  cv::Mat GetImage();

  BEGIN_MSG_MAP_EX(CCameraHiresView)
    MSG_WM_INITDIALOG(OnInitDialog)
    COMMAND_ID_HANDLER_EX(IDC_MORE, OnPopupMore)
    COMMAND_CODE_HANDLER_EX(EN_CHANGE, OnEditChanged)
    COMMAND_HANDLER_EX(IDC_EXPOSURE, CBN_SELCHANGE, OnExposureChanged)
    CHAIN_MSG_MAP(CDialogWithColor<CCameraHiresView>)
    CHAIN_MSG_MAP(CDynamicDialogLayout<CCameraHiresView>)
    CHAIN_MSG_MAP_MEMBER_PTR(m_lightsWindow);
  END_MSG_MAP()

  BEGIN_DDX_MAP(CCameraWindow)
  END_DDX_MAP()
};
