//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "GroupDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CGroupDlg, CDialog)

	ON_WM_CTLCOLOR()//

END_MESSAGE_MAP()

//***************************************************************************************

CGroupDlg::CGroupDlg(CWnd* pParentWnd, CGroup* pGroup) :
	CDialog(IDD_GROUP_DLG, pParentWnd)
{
	m_pGroup = pGroup;
}

//***************************************************************************************

void CGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_NAME_EDIT, m_NameEdit);
}

//***************************************************************************************

BOOL CGroupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_NameEdit.SetWindowText(m_pGroup->m_Name);

	m_NameEdit.SetFocus();

	return FALSE;
}

//***************************************************************************************

void CGroupDlg::OnOK()
{
	CString Name;

	m_NameEdit.GetWindowText(Name);
	if (Name.IsEmpty()) {
		::Error("Please enter a name.");
		m_NameEdit.SetFocus();
		return;
	}

	if (Name.FindOneOf(_T("\\/:*?\"<>|,")) != -1)
	{
		::Error("A name can't contain any of the following characters: \\/:*?\"<>|,");
		m_NameEdit.SetFocus();
		return;
	}

	//here detect whether the Database already has the same group_name 
	::DB.LoadGroups();
	CString lower_Name = Name;
	lower_Name.MakeLower();
	for (int i = 0; i < ::DB.m_Groups.GetSize(); i++)
	{
		if (lower_Name == ::DB.m_Groups[i].m_Name.MakeLower())
		{
			::Error("Group '" + Name + "' already exists.");
			m_NameEdit.Clear();
			m_NameEdit.SetFocus();
			return;
		}
	}

	m_pGroup->m_Name = Name;

	CDialog::OnOK();
}

//***************************************************************************************
//
HBRUSH CGroupDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_EDIT)
	{
		pDC->SetTextColor(GREEN);
	}

	return hbr;
}
//

//***************************************************************************************
