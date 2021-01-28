//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Data.h"

//***************************************************************************************

class CEyeDlg : public CDialog
{
public:

	CEyeDlg(CWnd* pParentWnd, CEye* pEye);

private:

	CEye* m_pEye;

	CEdit m_VertexEdit;
	CEdit m_SphEdit;
	CEdit m_CylEdit;
	CEdit m_AxisEdit;
	CEdit m_K1Edit;
	CEdit m_K2Edit;
	CEdit m_K2AxisEdit;

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;

};

//***************************************************************************************
