//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "PhysicianDlg.h"

#include "GlobalFunctions.h"//6.2.0 For adding read chinese function

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

CPhysicianDlg::CPhysicianDlg(CWnd* pParentWnd, CPhysician* pPhysician) :
	CDialog(IDD_PHYSICIAN_DLG, pParentWnd)
{
	m_pPhysician = pPhysician;
}

//***************************************************************************************

void CPhysicianDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LAST_NAME_EDIT, m_LastNameEdit);
	DDX_Control(pDX, IDC_FIRST_NAME_EDIT, m_FirstNameEdit);
}

//***************************************************************************************

BOOL CPhysicianDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_LastNameEdit.SetWindowText(G_ReadChinese(m_pPhysician->m_LastName));
	m_FirstNameEdit.SetWindowText(G_ReadChinese(m_pPhysician->m_FirstName));

	m_LastNameEdit.SetFocus();

	GetDlgItem(IDOK)->EnableWindow(!::DB.IsReadOnly());

	return FALSE;
}

//***************************************************************************************

void CPhysicianDlg::OnOK()
{
	CString LastName;
	m_LastNameEdit.GetWindowText(LastName);

	if (LastName.IsEmpty())
	{
		::Error("Please enter last name.");
		m_LastNameEdit.SetFocus();
		return;
	}

	if (LastName.FindOneOf(_T("\\/:*?\"<>|,")) != -1)
	{
		::Error("Last name can't contain any of the following characters: \\/:*?\"<>|,");
		m_LastNameEdit.SetFocus();
		return;
	}

	CString FirstName;
	m_FirstNameEdit.GetWindowText(FirstName);

	if (FirstName.IsEmpty())
	{
		::Error("Please enter first name.");
		m_FirstNameEdit.SetFocus();
		return;
	}

	if (FirstName.FindOneOf(_T("\\/:*?\"<>|,")) != -1)
	{
		::Error("First name can't contain any of the following characters: \\/:*?\"<>|,");
		m_FirstNameEdit.SetFocus();
		return;
	}

	//do not allow the same name input
	CString ori_lower_Name = m_pPhysician->m_FirstName + " " + m_pPhysician->m_LastName;
	ori_lower_Name.MakeLower();

	CString Name = FirstName + " " + LastName;
	CString lower_Name = Name;
	lower_Name.MakeLower();
	for (int i = 0; i < ::DB.m_Physicians.GetSize(); i++)
	{
		if (ori_lower_Name != ::DB.m_Physicians[i].FullName().MakeLower())
		{
			if (lower_Name == ::DB.m_Physicians[i].FullName().MakeLower())
			{
				::Error("Physician '" + Name + "' already exists.");
				m_FirstNameEdit.Clear();
				m_LastNameEdit.Clear();
				m_LastNameEdit.SetFocus();
				return;
			}
		}
	}
	// do not allow the same name input done

	m_pPhysician->m_LastName = G_ConvertChineseStr(LastName);
	m_pPhysician->m_FirstName = G_ConvertChineseStr(FirstName);

	CDialog::OnOK();
}

//***************************************************************************************
