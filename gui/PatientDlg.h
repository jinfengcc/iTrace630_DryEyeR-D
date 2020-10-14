//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Data.h"

//***************************************************************************************

class CPatientDlg : public CDialog
{
public:

	CPatientDlg(CWnd* pParentWnd, CPatient* pPatient, CEye* pOD, CEye* pOS);

	BOOL m_SaveOD;
	BOOL m_SaveOS;

private:

	CPatient* m_pPatient;
	CEye* m_pOD;
	CEye* m_pOS;

	int        m_CurYear;

	CEdit      m_LastNameEdit;
	CEdit      m_FirstNameEdit;
	CEdit      m_MiddleNameEdit;
	//CEdit      m_YearEdit;//[cjf***032012]
	CComboBox  m_MonthComboBox;
	CComboBox  m_DaysComboBox;//[cjf***032012]
	CComboBox  m_YearComboBox;//[cjf***032012]
	CStatic    m_YearsLabel;//530
	CStatic    m_MonthLabel;//530
	CStatic    m_DaysLabel;//530
						   /* CEdit      m_DayEdit;*///[cjf***032012]
	CButton    m_MaleCheck;
	CButton    m_FemaleCheck;

	CEdit      m_CustomIdEdit;
	CComboBox  m_GroupsComboBox;
	CEdit      m_NoteEdit;

	CEdit      m_Line1Edit;
	CEdit      m_Line2Edit;
	CEdit      m_CityEdit;
	CComboBox  m_StateComboBox;
	CEdit      m_ZipEdit;
	CEdit      m_CountryEdit;
	CEdit      m_HomePhoneEdit;
	CEdit      m_WorkPhoneEdit;
	CEdit      m_EMailEdit;

	afx_msg void OnNewGroupButtonClicked();
	afx_msg void OnODButtonClicked();
	afx_msg void OnOSButtonClicked();
	afx_msg void OnMaleCheckClicked();
	afx_msg void OnFemaleCheckClicked();

	afx_msg HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);//

	int GetCurYear();
	void CreateGroupsList(const GUID& GroupID);

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
