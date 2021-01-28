//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Data.h"

//***************************************************************************************

class CSameDicomPatientDlg : public CDialog
{
public:

	CSameDicomPatientDlg(CWnd* pParentWnd, CPatient* NewPatient, CPatient* OldPatient);

private:
	BOOL      m_SelFN;
	BOOL      m_SelMN;
	BOOL      m_SelLN;
	BOOL      m_SelDOB;
	BOOL      m_SelSex;

	CPatient* m_NewPatient;
	CPatient* m_OldPatient;

	CComboBox m_LastNameEdit;
	CComboBox m_FirstNameEdit;
	CComboBox m_MiddleNameEdit;
	CComboBox m_DOBEdit;
	CComboBox m_SexEdit;

	CStatic   m_LastNameText;
	CStatic   m_FirstNameText;
	CStatic   m_MiddleNameText;
	CStatic   m_DOBText;
	CStatic   m_SexText;

	CEdit     m_NoteEdit;

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************

