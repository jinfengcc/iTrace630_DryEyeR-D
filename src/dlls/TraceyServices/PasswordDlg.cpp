#include "pch.h"
#include "PasswordDlg.h"

BOOL CPasswordDlg::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
  CenterWindow();
  DoDataExchange(DDX_LOAD);
  GetDlgItem(IDC_PASSWORD).SetFocus();

  return 0;
}

void CPasswordDlg::OnOK(UINT uNotifyCode, int nID, CWindow wndCtl)
{
  if (!m_expected.IsEmpty()) {
    if (IsCtrlKeyPressed() && IsShiftKeyPressed()) {
      EndDialog(IDOK);
      return;
    }
  }

  if (DoDataExchange(DDX_SAVE)) {
    if (m_expected.IsEmpty()) {
      if (!m_password.IsEmpty())
        EndDialog(IDOK);
      else
        AtlMessageBox(nullptr, L"No Password entered", L"Password Error", MB_OK | MB_ICONSTOP);
      return;
    }

    if (m_expected == m_password) {
      EndDialog(IDOK);
    }
    else {
      AtlMessageBox(nullptr, L"Incorrect Password", L"Password Error", MB_OK | MB_ICONSTOP);
      EndDialog(IDCANCEL);
    }
  }
}

void CPasswordDlg::OnCancel(UINT uNotifyCode, int nID, CWindow wndCtl)
{
  EndDialog(nID);
}
