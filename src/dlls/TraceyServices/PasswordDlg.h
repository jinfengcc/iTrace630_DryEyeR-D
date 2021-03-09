#pragma once

#include "resource.h"

class CPasswordDlg
  : public CDialogImpl<CPasswordDlg>
  , public CWinDataExchange<CPasswordDlg>
{
public:
  enum { IDD = IDD_PASSWORD};

  CPasswordDlg(const char *expected)
    : m_expected(expected)
  {
  }

private:
  CString m_expected;
  CString m_password;

  BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
  void OnOK(UINT uNotifyCode, int nID, CWindow wndCtl);
  void OnCancel(UINT uNotifyCode, int nID, CWindow wndCtl);

  BEGIN_MSG_MAP_EX(CPasswordDlg)
    MSG_WM_INITDIALOG(OnInitDialog)
    COMMAND_ID_HANDLER_EX(IDOK, OnOK)
    COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
  END_MSG_MAP()

  BEGIN_DDX_MAP(CPasswordDlg)
    DDX_TEXT(IDC_PASSWORD, m_password)
  END_DDX_MAP()
};
