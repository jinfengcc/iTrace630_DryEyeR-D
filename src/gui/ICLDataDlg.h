//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Data.h"

//***************************************************************************************

class CICLDataDlg : public CDialog
{
public:

	CICLDataDlg(CWnd* pParentWnd, COpData* pOpData);

private:

	COpData* m_pOpData;
	COpData m_OpData;

	void GetDlgData();
	void SetDlgData();

	BOOL ValidateData();

	BOOL m_CanChange;

	BOOL m_ICLCyl_DontChange;
	CEdit m_ICLCylEdit;
	afx_msg void OnICLCylEditChanged();

	BOOL m_ICLAxis_DontChange;
	CEdit m_ICLAxisEdit;
	afx_msg void OnICLAxisEditChanged();

	BOOL m_ICLPlacementAxis_DontChange;
	CEdit m_ICLPlacementAxisEdit;
	afx_msg void OnICLPlacementAxisEditChanged();

	CEdit m_ICLInducedCylEdit;

	CEdit m_ICLInducedAxisEdit;

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
