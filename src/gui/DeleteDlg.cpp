//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "DeleteDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

CDeleteDlg::CDeleteDlg(CWnd* pParentWnd, CString name, int type) :
	CDialog(IDD_DELETE_DLG, pParentWnd)
{
	m_type = type;
	m_caption = name;
}

//***************************************************************************************

void CDeleteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

//***************************************************************************************

BOOL CDeleteDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString Type_Name;
	switch (m_type)
	{
	case 1:
		Type_Name = "Clinic";
		break;
	case 2:
		Type_Name = "Physician";
		break;
	case 3:
		Type_Name = "Operator";
		break;
	};

	CString caption = "Delete " + Type_Name + " '" + m_caption + "'";
	return FALSE;
}

//***************************************************************************************

void CDeleteDlg::OnOK()
{
	CDialog::OnOK();
}

//***************************************************************************************
