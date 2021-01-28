/*******************************************************************
Autor:          Jinfeng Chen
E-mail:         jinfengcc@gmail.com
Date:           10-07-2015
Software Ver:   
Description:    This dialog is used to merge two or more patients to be one patient.

*****************************************************************************************/


//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Data.h"

//***************************************************************************************

class CPatientsMergeDlg : public CDialog
{
public:

	CPatientsMergeDlg(CWnd* pParentWnd, CPatient pPatient[100], int nPatients, CEye* pOD, CEye* pOS, CPatient* resPatient);

	BOOL m_SaveOD;
	BOOL m_SaveOS;

private:

	int  m_nPatients;
	CPatient m_pPatient[100];
	CPatient* m_resPatient;
	CEye* m_pOD;
	CEye* m_pOS;

	CComboBox  m_LastNameComboBox;
	CComboBox  m_FirstNameComboBox;
	CComboBox  m_MiddleComboBox;

	CComboBox  m_MonthComboBox;
	CComboBox  m_DaysComboBox;//[cjf***032012]
	CComboBox  m_YearComboBox;//[cjf***032012]
	CStatic    m_YearsLabel;//530
	CStatic    m_MonthLabel;//530
	CStatic    m_DaysLabel;//530
						   /* CEdit      m_DayEdit;*///[cjf***032012]
	CButton    m_MaleCheck;
	CButton    m_FemaleCheck;

	CComboBox  m_CustomIdComboBox;
	CComboBox  m_GroupsComboBox;
	CEdit      m_NoteEdit;

	CComboBox  m_Line1ComboBox;
	CComboBox  m_Line2ComboBox;
	CComboBox  m_CityComboBox;

	CComboBox  m_StateComboBox;

	CComboBox  m_ZipComboBox;

	CComboBox  m_CountryComboBox;

	CComboBox  m_HomePhoneComboBox;
	CComboBox  m_WorkPhoneComboBox;
	CComboBox  m_EMailComboBox;

	afx_msg void OnNewGroupButtonClicked();
	afx_msg void OnODButtonClicked();
	afx_msg void OnOSButtonClicked();
	afx_msg void OnMaleCheckClicked();
	afx_msg void OnFemaleCheckClicked();

	void CreateGroupsList(const GUID& GroupID);
	void CreateIDList(const CString& GroupID);
	void CreateAdLine1List(const CString& AdLine1);
	void CreateAdLine2List(const CString& AdLine2);

	void CreateCityList(const CString& City);
	void CreateZipList(const CString& Zip);
	void CreateCountryList(const CString& Country);
	void CreateHomePhoneList(const CString& HomePhone);
	void CreateWorkPhoneList(const CString& WorkPhone);
	void CreateEmailList(const CString& Email);

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
