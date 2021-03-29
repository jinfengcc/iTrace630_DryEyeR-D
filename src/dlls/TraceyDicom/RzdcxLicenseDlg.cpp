#include "pch.h"
#include "RzdcxLicenseDlg.h"
#include "libs/DicomLib/rzdcx/rzdcxActivate.h"

CRzdcxLicenseDlg::CRzdcxLicenseDlg()
  : m_request(rzdcxGetLicenseRequest().c_str())
  , m_status(rzdcxIsActivated() ? L"Activated" : L"NOT Activated")
  , m_license(rzdcxGetLicense().c_str())
{
}

BOOL CRzdcxLicenseDlg::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
  DoDataExchange(DDX_LOAD);
  CenterWindow();

  return TRUE; // Let the system set the focus
}

void CRzdcxLicenseDlg::OnOK(UINT uNotifyCode, int nID, CWindow wndCtl)
{
  if (!DoDataExchange(DDX_SAVE))
    return;

  if (m_license.IsEmpty()) {
    AtlMessageBox(*this, L"Please enter a valid license", L"Error", MB_ICONSTOP | MB_OK);
    return;
  }

  if (!rzdcxSetLicense(CStringA(m_license).GetString())) {
    AtlMessageBox(*this, L"The license entered is not valid. Please enter a valid license", L"Error", MB_ICONSTOP | MB_OK);
    return;
  }

  EndDialog(nID);
}

void CRzdcxLicenseDlg::OnCancel(UINT uNotifyCode, int nID, CWindow wndCtl)
{
  EndDialog(nID);
}
