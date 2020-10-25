#include "pch.h"
#include "LightsWindow.h"

CLightsWindow::CLightsWindow(HWND hWnd)
  : CWindow(hWnd)
{
  CreateObj(m_lights);
  if (m_lights->Connected()) {

    auto ws = WindowAreaControls(*this, IDC_LIGHT_SEC);
    for (auto w : ws) {
      char szClassName[256];
      ::GetClassNameA(w, szClassName, _countof(szClassName));
      if (_stricmp(szClassName, "msctls_updown32") == 0)
        CUpDownCtrl(w).SetRange(0, 100);
    }

    DoDataExchange(DDX_LOAD);
  }
  else {
    auto ws = WindowAreaControls(*this, IDC_LIGHT_SEC);
    for (CWindow w : ws)
      w.EnableWindow(FALSE);
    SetDlgItemText(IDC_LIGHT_SEC, L"Lights not available");
  }

}

void CLightsWindow::OnEditChanged(UINT uNotifyCode, int nID, CWindow wndCtl)
{
  SetMsgHandled(FALSE); // Let siblings have a chance on a button click

  if (m_lights && DoDataExchange(DDX_SAVE)) {
    for (unsigned i = 0; i < m_lightIntensity.size(); ++i)
      m_lights->Power(static_cast<hal::LED>(i), m_lightIntensity[i]);
  }
}

