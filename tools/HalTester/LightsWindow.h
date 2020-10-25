#pragma once

#include "resource.h"

class CLightsWindow
  : public CWindow
  , public CWinDataExchange<CLightsWindow>
  , public CMessageMap
{
public:
  CLightsWindow(HWND hWnd);

private:
  CContainedWindowT<CStatic>   m_imgCtrl;
  wil::com_ptr_t<hal::IDevice> m_lights;
  std::array<int, 5>           m_lightIntensity{};

  void OnEditChanged(UINT uNotifyCode, int nID, CWindow wndCtl);

  BEGIN_MSG_MAP_EX(CLightsWindow)
    COMMAND_CODE_HANDLER_EX(EN_CHANGE, OnEditChanged)
  END_MSG_MAP()

  BEGIN_DDX_MAP(CLightsWindow)
    DDX_INT_RANGE(IDC_EDIT5, m_lightIntensity[0], 0, 100)
    DDX_INT_RANGE(IDC_EDIT6, m_lightIntensity[1], 0, 100)
    DDX_INT_RANGE(IDC_EDIT7, m_lightIntensity[2], 0, 100)
    DDX_INT_RANGE(IDC_EDIT8, m_lightIntensity[3], 0, 100)
    DDX_INT_RANGE(IDC_EDIT9, m_lightIntensity[4], 0, 100)
  END_DDX_MAP()
};
