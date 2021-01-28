//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "NumDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

CNumDlg::CNumDlg(CWnd* pParentWnd, const CString& title) :
	CDialog(IDD_NUMBER_DLG, pParentWnd)
{
	m_title = title;
}

//***************************************************************************************

void CNumDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NUMBER_EDIT, m_NumberEdit);
}

//***************************************************************************************

BOOL CNumDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(m_title);
	CenterWindow();
	m_NumberEdit.SetFocus();
	return FALSE;
}

//***************************************************************************************

void CNumDlg::OnOK()
{
	CString s;
	m_NumberEdit.GetWindowText(s);
	if (s == "") return;

	G_As = s;
	m_number = atof(G_As);

	CDialog::OnOK();
}

//***************************************************************************************

BOOL EnterNumber(real_t& number, CWnd* parent, const CString& title)
{
	CNumDlg dlg(parent, title);
	if (dlg.DoModal() != IDOK) return FALSE;
	number = dlg.m_number;
	return TRUE;
}

//***************************************************************************************
