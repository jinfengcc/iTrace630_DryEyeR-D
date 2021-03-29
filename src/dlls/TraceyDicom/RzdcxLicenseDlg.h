#pragma once

#include "resource.h"

class CRzdcxLicenseDlg
  : public CDialogImpl<CRzdcxLicenseDlg>
  , public CWinDataExchange<CRzdcxLicenseDlg>

{
public:
  enum
  {
    IDD = IDD_LICENSE
  };

  CRzdcxLicenseDlg();

private:
  CString m_request;
  CString m_status;
  CString m_license;

  BOOL    OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
  void    OnOK(UINT uNotifyCode, int nID, CWindow wndCtl);
  void    OnCancel(UINT uNotifyCode, int nID, CWindow wndCtl);

  BEGIN_MSG_MAP_EX(CRzdcxLicenseDlg)
    MSG_WM_INITDIALOG(OnInitDialog)
    COMMAND_ID_HANDLER_EX(IDOK, OnOK)
    COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
  END_MSG_MAP()

  BEGIN_DDX_MAP(CRzdcxLicenseDlg)
    DDX_TEXT(IDC_REQUEST, m_request)
    DDX_TEXT(IDC_STATUS, m_status)
    DDX_TEXT(IDC_LICENSE, m_license)
  END_DDX_MAP()
};
