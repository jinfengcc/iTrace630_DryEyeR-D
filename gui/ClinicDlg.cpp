// ***********************************************************************
// Assembly         : 
// Author           : iTrace
// Created          : 09-07-2016
//
// Last Modified By : iTrace
// Last Modified On : 03-28-2016
// ***********************************************************************


#include "StdAfx.h"
#include "Resource.h"
#include "ClinicDlg.h"
#include "Settings.h"
#include "DeleteDlg.h"

#include "GlobalFunctions.h"//6.2.0 For adding read chinese function

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CClinicDlg, CDialog)

	//ON_BN_CLICKED(IDDELETE, OnDeleteButtonClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CClinicDlg::CClinicDlg(CWnd* pParentWnd, CClinic* pClinic) :
	CDialog(IDD_CLINIC_DLG, pParentWnd)
{
	m_pClinic = pClinic;
}

//***************************************************************************************

void CClinicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_NAME_EDIT, m_NameEdit);
	DDX_Control(pDX, IDC_LINE1_EDIT, m_Line1Edit);
	DDX_Control(pDX, IDC_LINE2_EDIT, m_Line2Edit);
	DDX_Control(pDX, IDC_CITY_EDIT, m_CityEdit);
	DDX_Control(pDX, IDC_STATE_COMBOBOX, m_StateComboBox);
	DDX_Control(pDX, IDC_ZIP_EDIT, m_ZipEdit);
	DDX_Control(pDX, IDC_COUNTRY_EDIT, m_CountryEdit);
}

//***************************************************************************************

BOOL CClinicDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString STATE[57] = {
		{ "AA" },{ "AE" },{ "AK" },{ "AL" },{ "AP" },{ "AR" },{ "AZ" },{ "CA" },{ "CO" },{ "CT" },
		{ "DC" },{ "DE" },{ "FL" },{ "GA" },{ "GU" },{ "HI" },{ "IA" },{ "ID" },{ "IL" },{ "IN" },
		{ "KS" },{ "KY" },{ "LA" },{ "MA" },{ "MD" },{ "ME" },{ "MI" },{ "MN" },{ "MO" },{ "MS" },
		{ "MT" },{ "NC" },{ "ND" },{ "NE" },{ "NH" },{ "NJ" },{ "NM" },{ "NV" },{ "NY" },{ "OH" },
		{ "OK" },{ "OR" },{ "PA" },{ "PR" },{ "RI" },{ "SC" },{ "SD" },{ "TN" },{ "TX" },{ "UT" },
		{ "VA" },{ "VI" },{ "VT" },{ "WA" },{ "WI" },{ "WV" },{ "WY" }
	};
	for (int i = 0; i < 57; i++) m_StateComboBox.AddString(LPCTSTR(STATE[i]));
	
	m_NameEdit.SetWindowText(G_ReadChinese(m_pClinic->m_Name));
	m_Line1Edit.SetWindowText(G_ReadChinese(m_pClinic->m_Address.m_Line1));
	m_Line2Edit.SetWindowText(G_ReadChinese(m_pClinic->m_Address.m_Line2));
	m_CityEdit.SetWindowText(G_ReadChinese(m_pClinic->m_Address.m_City));
	m_StateComboBox.SetWindowText(G_ReadChinese(m_pClinic->m_Address.m_State));
	m_ZipEdit.SetWindowText(m_pClinic->m_Address.m_ZIP);
	m_CountryEdit.SetWindowText(G_ReadChinese(m_pClinic->m_Address.m_Country));

	m_NameEdit.SetFocus();

	GetDlgItem(IDOK)->EnableWindow(!::DB.IsReadOnly());


	return FALSE;
}

//***************************************************************************************

void CClinicDlg::OnOK()
{
	CString Name;
	m_NameEdit.GetWindowText(Name);
	if (Name.IsEmpty()) {
		::Error("Please enter a name.");
		m_NameEdit.SetFocus();
		return;
	}
	if (Name.FindOneOf(LPWSTR("\\/:*?\"<>|,")) != -1) {
		::Error("A name can't contain any of the following characters: \\/:*?\"<>|,");
		m_NameEdit.SetFocus();
		return;
	}

	//[cjf***04182012], do not allow the same name input (only for new function
	CString ori_lower_Name = m_pClinic->m_Name;
	ori_lower_Name.MakeLower();
	CString lower_Name = Name;
	lower_Name.MakeLower();
	for (int i = 0; i < ::DB.m_Clinics.GetSize(); i++) 
	{
		if (ori_lower_Name != ::DB.m_Clinics[i].m_Name.MakeLower()) 
		{
			if (lower_Name == ::DB.m_Clinics[i].m_Name.MakeLower()) 
			{
				::Error("Clinic '" + Name + "' already exists.");
				m_NameEdit.Clear();
				m_NameEdit.SetFocus();
				return;
			}
		}
	}

	m_pClinic->m_Name = G_ConvertChineseStr(Name);

	CString Line1, Line2, City, State, Country;

	m_Line1Edit.GetWindowText(Line1);
	m_Line2Edit.GetWindowText(Line2);
	m_CityEdit.GetWindowText(City);
	m_StateComboBox.GetWindowText(State);
	m_CountryEdit.GetWindowText(Country);
	m_ZipEdit.GetWindowText(m_pClinic->m_Address.m_ZIP);

	m_pClinic->m_Address.m_Line1 = G_ConvertChineseStr(Line1);
	m_pClinic->m_Address.m_Line2 = G_ConvertChineseStr(Line2);
	m_pClinic->m_Address.m_City = G_ConvertChineseStr(City);
	m_pClinic->m_Address.m_State = G_ConvertChineseStr(State);
	m_pClinic->m_Address.m_Country = G_ConvertChineseStr(Country);

	CDialog::OnOK();
}

////***************************************************************************************
////[cjf***05082012], delete the clinic 
//void CClinicDlg::OnDeleteButtonClicked()
//{
//  // under the clinic name, check whether there are exams exists
//  CString Name;
//  m_NameEdit.GetWindowText(Name);
//
//  CString lower_Name = Name;	
//  lower_Name.MakeLower();
//
//  if (!::Question("You are about to remove selected clinic '"+Name +"',\n and their records in Exam database.\n Are you sure?\n (This operation does not delete any exams)", 0, 1)) {
//	goto End;
//  }
// 
//  //delete this clinic from clinics table
//  for (int i = 0; i < ::DB.m_Clinics.GetSize(); i++) {		    
//	if (lower_Name == ::DB.m_Clinics[i].m_Name.MakeLower()) {
//		::DB.DeleteClinic(::DB.m_Clinics[i].m_ClinicID);
//		::TempSettings.Temp_Deleted = 1;
//		break;
//	}		
//  }
//
//  if(::TempSettings.Temp_Deleted)  ::Info("Clinic '"+Name +"' has been removed." );
//  CDialog::OnCancel();
//  
//  End:  
//	int  a =0;
//}
////***************************************************************************************
