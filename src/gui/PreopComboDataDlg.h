//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MGDI.h"
#include "Data.h"

//***************************************************************************************

class CPreopComboDataDlg : public CDialog
{
public:

	CPreopComboDataDlg(CWnd* pParentWnd, COpData* pOpData, int OD, CString PatientName, CString PatientAge);

private:

	int m_Last_BestIOL;
	int m_Last_BestIOLPos;//7.0.0

	int m_OD;//IS the checking eyes belongs to OD or OS?

	CString m_PatientName;
	CString m_PatientAge;

	COpData* m_pOpData;
	COpData m_OpData;

	void GetDlgData();
	void SetDlgData();

	void SetIncLoC_SurInAst();//For the the 'incision location' and 'surgically induced astigmatism'  

	BOOL ValidateData();

	BOOL m_CanChange;

	// Pre-op  
	CEdit m_PreopEntireEyeCylEdit;
	BOOL m_PreopEntireEyeCyl_DontChange;
	afx_msg void OnPreopEntireEyeCylEditChanged();

	CEdit m_PreopEntireEyeAxisEdit;
	BOOL m_PreopEntireEyeAxis_DontChange;
	afx_msg void OnPreopEntireEyeAxisEditChanged();

	CButton m_PreopEntireEyeWFCheck;
	afx_msg void OnPreopEntireEyeWFCheckClicked();

	CButton m_PreopCyclinderAdjCheck;// Cyclinder adjustment Branch
	afx_msg void OnPreopCyclinderAdjCheckClicked();// Cyclinder adjustment Branch

	CEdit m_PreopInternalCylEdit;

	CEdit m_PreopInternalAxisEdit;

	CStatic m_PreopAdjStatic;// IDC_ADJ_TEXT;

	CStatic m_PatientNameText;//7.0.0
	CStatic m_PatientAgeText;//7.0.0

	BOOL  m_PreopCorneaCyl_DontChange;
	CEdit m_PreopCorneaCylEdit;

	afx_msg void OnPreopCorneaCylEditChanged();

	BOOL m_PreopCorneaAxis_DontChange;
	CEdit m_PreopCorneaAxisEdit;
	afx_msg void OnPreopCorneaAxisEditChanged();

	CButton m_PreopCorneaWFCheck;
	afx_msg void OnPreopCorneaWFCheckClicked();

	CButton m_PreopCorneaSimKCheck;
	afx_msg void OnPreopCorneaSimKCheckClicked();

	// IOL

	CEdit m_IncisionAxisEdit;
	BOOL m_IncisionAxis_DontChange;
	afx_msg void OnIncisionAxisEditChanged();

	CEdit m_IncisionInducedCylEdit;
	BOOL m_IncisionInducedCyl_DontChange;
	afx_msg void OnIncisionInducedCylEditChanged();

	CEdit m_IncisionInducedAxisEdit;

	CComboBox m_IOLSphereComboBox;

	BOOL m_IOLAxis_DontChange;
	CEdit m_IOLPlacementAxisEdit;
	afx_msg void OnIOLAxisEditChanged();

	CButton m_NewIOLRadios[7];//7.0.0
	CEdit m_NewIOLCylEdit[7][2];//7.0.0
	CEdit m_NewIOLAxisEdit[7];//7.0.0
	CEdit m_NewIOLPostopEyeCylEdit[7];//7.0.0
	CEdit m_NewIOLPostopEyeAxisEdit[7];//7.0.0

	afx_msg void OnIOL1RadioClicked() { OnIOLRadioClicked(0); }
	afx_msg void OnIOL2RadioClicked() { OnIOLRadioClicked(1); }
	afx_msg void OnIOL3RadioClicked() { OnIOLRadioClicked(2); }
	afx_msg void OnIOL4RadioClicked() { OnIOLRadioClicked(3); }
	afx_msg void OnIOL5RadioClicked() { OnIOLRadioClicked(4); }
	afx_msg void OnIOL6RadioClicked() { OnIOLRadioClicked(5); }
	afx_msg void OnIOL7RadioClicked() { OnIOLRadioClicked(6); }

	void OnIOLRadioClicked(const int i);

	// Post-op

	CEdit m_PostopCorneaCylEdit;
	CEdit m_PostopCorneaAxisEdit;

	CEdit m_PostopInternalCylEdit;
	CEdit m_PostopInternalAxisEdit;

	CEdit m_PostopEntireEyeCylEdit;
	CEdit m_PostopEntireEyeAxisEdit;

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;

	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
