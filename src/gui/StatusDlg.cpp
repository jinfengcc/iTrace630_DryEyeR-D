//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "StatusDlg.h"
#include "ActivationDlg.h"
#include "Licensing.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CStatusDlg, CDialog)

	ON_BN_CLICKED(IDC_ACTIVATE, OnActivateButtonClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CStatusDlg::CStatusDlg(CWnd* pParentWnd) : CDialog(IDD_STATUS_DLG, pParentWnd)
{
}

//***************************************************************************************

void CStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LICENSE_TYPE_STATIC, m_LicenseTypeStatic);
  DDX_Control(pDX, IDCANCEL, m_closeButton);

}

void CStatusDlg::OnCancel()
{
  if (!Licensing.IsValid()) {
    AfxMessageBox(L"Invalid License. Program will terminate");
    _exit(1);
  }
  CDialog::OnCancel();
}

//***************************************************************************************

BOOL CStatusDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString s;

	     if (::Licensing.IsPresbia())     s = "Presbia";
	else if (::Licensing.IsDemo())        s = "Demo";
	else if (::Licensing.IsSolo())        s = "Solo";
	else if (::Licensing.IsCombo())       s = "Combo";
	else if (::Licensing.IsWorkstation()) s = "Workstation";
	else if (::Licensing.IsViewer())      s = "Workstation Viewer";
	else if (::Licensing.IsComboViewer()) s = "Combo Viewer";
	else if (::Licensing.IsComboDICOM())  s = "Combo Dicom";//620
	else                                  s = "None";

	m_LicenseTypeStatic.SetWindowText(s);

  UpdateCloseButton();
	GetDlgItem(IDCANCEL)->SetFocus();

	return TRUE;
}

//***************************************************************************************

void CStatusDlg::OnActivateButtonClicked()
{
	auto pDlg = std::make_unique<CActivationDlg>(this);

	if (pDlg->DoModal() == IDOK) {

		CString s;

		if (::Licensing.IsPresbia())     s = "Presbia";
		else if (::Licensing.IsDemo())        s = "Demo";
		else if (::Licensing.IsSolo())        s = "Solo";
		else if (::Licensing.IsCombo())       s = "Combo";
		else if (::Licensing.IsWorkstation()) s = "Workstation";
		else if (::Licensing.IsViewer())      s = "Workstation Viewer";
		else if (::Licensing.IsComboViewer()) s = "Combo Viewer";
		else if (::Licensing.IsComboDICOM())  s = "Combo Dicom";//620
		else                                  s = "None";

		m_LicenseTypeStatic.SetWindowText(s);
	}

  UpdateCloseButton();
}

void CStatusDlg::UpdateCloseButton()
{
  m_closeButton.SetWindowText(Licensing.IsValid() ? L"Close" : L"Exit");
}

//***************************************************************************************
