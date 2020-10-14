//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "MaskDlg.h"
#include "Data.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CMaskDlg, CDialog)

	ON_BN_CLICKED(IDC_CLEAR_ALL, OnClearAllButtonClicked)
	ON_BN_CLICKED(IDC_SELECT_ALL, OnSelectAllButtonClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CMaskDlg::CMaskDlg(CWnd* pParentWnd) : CDialog(IDD_MASK_DLG, pParentWnd)
{
}

//***************************************************************************************

BOOL CMaskDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	for (int i = 3; i < 45; i++) {
		((CButton*)GetDlgItem(IDC_00 + i))->SetCheck(m_m[i]);
	}

	return TRUE;
}

//***************************************************************************************

void CMaskDlg::OnClearAllButtonClicked()
{
	m_m[0] = FALSE;
	m_m[1] = FALSE;
	m_m[2] = FALSE;
	for (int i = 3; i < 45; i++) {
		m_m[i] = FALSE;
		((CButton*)GetDlgItem(IDC_00 + i))->SetCheck(m_m[i]);
	}
}

//***************************************************************************************

void CMaskDlg::OnSelectAllButtonClicked()
{
	m_m[0] = FALSE;
	m_m[1] = FALSE;
	m_m[2] = FALSE;
	for (int i = 3; i < 45; i++) {
		m_m[i] = TRUE;
		((CButton*)GetDlgItem(IDC_00 + i))->SetCheck(m_m[i]);
	}
}

//***************************************************************************************

void CMaskDlg::OnOK()
{
	CDialog::OnOK();

	m_m[0] = FALSE;
	m_m[1] = FALSE;
	m_m[2] = FALSE;
	for (int i = 3; i < 45; i++) {
		m_m[i] = ((CButton*)GetDlgItem(IDC_00 + i))->GetCheck();
	}
}

//***************************************************************************************
