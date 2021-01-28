#include "pch.h"
#include "AccessionDialog.h"

BOOL CAccessionDialog::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
  CenterWindow();
  DoDataExchange(DDX_LOAD);
  return TRUE; // Let the system set the focus
}

void CAccessionDialog::OnOK(UINT uNotifyCode, int nID, CWindow wndCtl)
{
  if (DoDataExchange(DDX_SAVE)) {
    EndDialog(IDOK);
  }
}

void CAccessionDialog::OnCancel(UINT uNotifyCode, int nID, CWindow wndCtl)
{
  EndDialog(IDCANCEL);
}

