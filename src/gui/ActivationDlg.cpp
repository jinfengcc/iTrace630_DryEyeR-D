//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "ActivationDlg.h"
#include "Licensing.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

CActivationDlg::CActivationDlg(CWnd* pParentWnd) : CDialog(IDD_ACTIVATION_DLG, pParentWnd)
{
}

//***************************************************************************************

void CActivationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_HID_EDIT, m_HardwareIDEdit);
	DDX_Control(pDX, IDC_SAK_EDIT, m_SoftwareAKEdit);
}

//***************************************************************************************

BOOL CActivationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString s;
	for (int i = 0; i < ::Licensing.m_HardwareID.GetLength(); i++) {
		s += ::Licensing.m_HardwareID[i];
		if ((i + 1) % 5 == 0 && i != ::Licensing.m_HardwareID.GetLength() - 1) s += " - ";
	}
	m_HardwareIDEdit.SetWindowText(s);

	s.Empty();
	for (int i = 0; i < ::Licensing.m_SoftwareAK.GetLength(); i++) {
		s += ::Licensing.m_SoftwareAK[i];
		if ((i + 1) % 5 == 0 && i != ::Licensing.m_SoftwareAK.GetLength() - 1) s += " - ";
	}
	m_SoftwareAKEdit.SetWindowText(s);

	GetDlgItem(IDCANCEL)->SetFocus();

	return TRUE;
}

//***************************************************************************************

void CActivationDlg::OnOK()
{
	CDialog::OnOK();

	CString SoftwareAK;

	m_SoftwareAKEdit.GetWindowText(SoftwareAK);

	SoftwareAK.Remove(' ');
	SoftwareAK.Remove('-');
	SoftwareAK.MakeUpper();

	::Licensing.DetermineLicenseType(SoftwareAK);
}

//***************************************************************************************
