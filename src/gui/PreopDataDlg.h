//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MGDI.h"
#include "Data.h"

//***************************************************************************************

class CPreopDataDlg : public CDialog
{
public:

	CPreopDataDlg(CWnd* pParentWnd, COpData* pOpData, int OD);////[Cjf***04122012]
															  //CPreopDataDlg(CWnd* pParentWnd, COpData* pOpData);////[Cjf***04122012], ORIGINAL CODE

private:

	CMDC m_LogoDC;

	int m_OD;//[Cjf***04192012], IS the checking eyes belongs to OD or OS?

	COpData* m_pOpData;
	COpData m_OpData;

	void GetDlgData();
	void SetDlgData();
	void SetIncLoC_SurInAst();//[cjf***40192012],For the the 'incision location' and 'surgically induced astigmatism'  

	BOOL ValidateData();

	CButton m_OpRadio[3];
	afx_msg void OnOp1RadioClicked();
	afx_msg void OnOp2RadioClicked();
	afx_msg void OnOp3RadioClicked();

	BOOL m_CanChange;

	// Pre-op



	CStatic m_TraceyIOLInfo1;//520;IDC_STATIC
	CStatic m_TraceyIOLInfo2;//520;IDC_STATIC

	CEdit m_PreopEntireEyeCylEdit;
	BOOL m_PreopEntireEyeCyl_DontChange;
	afx_msg void OnPreopEntireEyeCylEditChanged();

	CEdit m_PreopEntireEyeAxisEdit;
	BOOL m_PreopEntireEyeAxis_DontChange;
	afx_msg void OnPreopEntireEyeAxisEditChanged();

	CButton m_PreopEntireEyeWFCheck;
	afx_msg void OnPreopEntireEyeWFCheckClicked();

	CEdit m_PreopInternalCylEdit;

	CEdit m_PreopInternalAxisEdit;

	BOOL m_PreopCorneaCyl_DontChange;
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

	CButton m_IOLRadio[8];
	afx_msg void OnIOL1RadioClicked() { OnIOLRadioClicked(0); }
	afx_msg void OnIOL2RadioClicked() { OnIOLRadioClicked(1); }
	afx_msg void OnIOL3RadioClicked() { OnIOLRadioClicked(2); }
	afx_msg void OnIOL4RadioClicked() { OnIOLRadioClicked(3); }
	afx_msg void OnIOL5RadioClicked() { OnIOLRadioClicked(4); }
	afx_msg void OnIOL6RadioClicked() { OnIOLRadioClicked(5); }
	afx_msg void OnIOL7RadioClicked() { OnIOLRadioClicked(6); }
	afx_msg void OnIOL8RadioClicked() { OnIOLRadioClicked(7); }
	void OnIOLRadioClicked(const int i);

	CEdit m_IOLCylEdit[8][2];
	CEdit m_IOLAxisEdit[8];

	CEdit m_IOLPostopEyeCylEdit[8];
	CEdit m_IOLPostopEyeAxisEdit[8];

	// Post-op

	CEdit m_PostopCorneaCylEdit;
	CEdit m_PostopCorneaAxisEdit;

	CEdit m_PostopInternalCylEdit;
	CEdit m_PostopInternalAxisEdit;

	CEdit m_PostopEntireEyeCylEdit;
	CEdit m_PostopEntireEyeAxisEdit;

	//

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;

	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
