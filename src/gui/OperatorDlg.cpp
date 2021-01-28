//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "OperatorDlg.h"

#include "GlobalFunctions.h"//6.2.0 For adding read chinese function

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

COperatorDlg::COperatorDlg(CWnd* pParentWnd, COperator* pOperator) :
	CDialog(IDD_OPERATOR_DLG, pParentWnd)
{
	m_pOperator = pOperator;
}

//***************************************************************************************

void COperatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LAST_NAME_EDIT, m_LastNameEdit);
	DDX_Control(pDX, IDC_FIRST_NAME_EDIT, m_FirstNameEdit);
}

//***************************************************************************************

BOOL COperatorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_LastNameEdit.SetWindowText(G_ReadChinese(m_pOperator->m_LastName));
	m_FirstNameEdit.SetWindowText(G_ReadChinese(m_pOperator->m_FirstName));

	m_LastNameEdit.SetFocus();

	GetDlgItem(IDOK)->EnableWindow(!::DB.IsReadOnly());

	return FALSE;
}

//***************************************************************************************

void COperatorDlg::OnOK()
{
	CString LastName;
	m_LastNameEdit.GetWindowText(LastName);
	if (LastName.IsEmpty()) {
		::Error("Please enter last name.");
		m_LastNameEdit.SetFocus();
		return;
	}
	if (LastName.FindOneOf(_T("\\/:*?\"<>|,")) != -1) {
		::Error("Last name can't contain any of the following characters: \\/:*?\"<>|,");
		m_LastNameEdit.SetFocus();
		return;
	}

	CString FirstName;
	m_FirstNameEdit.GetWindowText(FirstName);
	if (FirstName.IsEmpty()) {
		::Error("Please enter first name.");
		m_FirstNameEdit.SetFocus();
		return;
	}
	if (FirstName.FindOneOf(_T("\\/:*?\"<>|,")) != -1) {
		::Error("First name can't contain any of the following characters: \\/:*?\"<>|,");
		m_FirstNameEdit.SetFocus();
		return;
	}

	//[cjf***04182012], do not allow the same name input
	CString ori_lower_Name = m_pOperator->m_FirstName + " " + m_pOperator->m_LastName;
	ori_lower_Name.MakeLower();

	CString Name = FirstName + " " + LastName;
	CString lower_Name = Name;
	lower_Name.MakeLower();
	for (int i = 0; i < ::DB.m_Operators.GetSize(); i++) {
		if (ori_lower_Name != ::DB.m_Operators[i].FullName().MakeLower()) {
			if (lower_Name == ::DB.m_Operators[i].FullName().MakeLower()) {
				::Error("Operator '" + Name + "' already exists.");
				m_FirstNameEdit.Clear();
				m_LastNameEdit.Clear();
				m_LastNameEdit.SetFocus();
				return;
			}
		}
	}
	//[cjf***04182012], do not allow the same name input

	m_pOperator->m_LastName = G_ConvertChineseStr(LastName);
	m_pOperator->m_FirstName = G_ConvertChineseStr(FirstName);

	CDialog::OnOK();
}

//***************************************************************************************
