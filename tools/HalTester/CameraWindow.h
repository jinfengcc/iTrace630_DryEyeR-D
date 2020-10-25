#pragma once

#include "resource.h"

class CCameraWindow
  : public CWindow
  , public CWinDataExchange<CCameraWindow>
  , public CMessageMap
{
public:
  CCameraWindow(HWND hWnd);

  void Start();
  void Stop();

private:
  using ICamera = hal::ICamera;

  CContainedWindowT<CStatic> m_imgCtrl;
  wil::com_ptr_t<ICamera>    m_camera;
  cv::Mat                    m_image;
  BOOL                       m_freeze{};
  BOOL                       m_cameraPower{};
  ICamera::SETTINGS          m_settings{};
  BOOL                       m_imgUnwrap;

  void OnTimer(UINT_PTR nIDEvent);
  void OnButtonClicked(UINT uNotifyCode, int nID, CWindow wndCtl);
  void OnEditChanged(UINT uNotifyCode, int nID, CWindow wndCtl);
  BOOL OnCameraEraseBkgnd(CDCHandle dc);
  void OnCameraPaint(CDCHandle dc);
  void DrawImage(Gdiplus::Graphics &gdg, const cv::Mat &img);
  void UpdatePropVisuals();

  // clang-format off
  BEGIN_MSG_MAP_EX(CCameraWindow)
    MSG_WM_TIMER(OnTimer)
    COMMAND_CODE_HANDLER_EX(BN_CLICKED, OnButtonClicked)
    COMMAND_CODE_HANDLER_EX(EN_CHANGE, OnEditChanged)
    ALT_MSG_MAP(1)
      MSG_WM_ERASEBKGND(OnCameraEraseBkgnd)
      MSG_WM_PAINT(OnCameraPaint)
  END_MSG_MAP()

  BEGIN_DDX_MAP(CCameraWindow)
    DDX_CHECK(IDC_POWER , m_cameraPower);
    DDX_CHECK(IDC_FREEZE, m_freeze);
    DDX_CHECK(IDC_UNWRAP, m_imgUnwrap)
    DDX_INT  (IDC_EDIT1 , m_settings.brightness);
    DDX_INT  (IDC_EDIT2 , m_settings.contrast);
    DDX_INT  (IDC_EDIT3 , m_settings.hue);
    DDX_INT  (IDC_EDIT4 , m_settings.saturation);
  END_DDX_MAP()
  // clang-format on
};
