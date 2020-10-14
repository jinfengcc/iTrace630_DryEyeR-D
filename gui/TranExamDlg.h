/*******************************************************************
Autor:          Jinfeng Chen
E-mail:         jinfengcc@gmail.com
Date:           10-13-2015
Software Ver:   
Description:    This dialog is used to transferr exam

*****************************************************************************************/


//***************************************************************************************

#pragma once

//***************************************************************************************

#include "XPListCtrl.h"

//***************************************************************************************

class CTranExamDlg : public CDialog
{
public:

	CTranExamDlg(CWnd* pParentWnd);

	int  m_Res;//0: Override OD/OS 1: Transform exam; -1 : cancel
	int  m_P;//Patient position in the database

private:

	BOOL       m_OdOsIsSelected;

	CButton    m_OdOsBtn;
	CButton    m_TransBtn;

	CXPListCtrl m_PatientsList;

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;

	afx_msg void UpdatePatientsList();
	afx_msg void OnOdOsRadioClicked();
	afx_msg void OnTransRadioClicked();

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
