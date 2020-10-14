//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Data.h"

//***************************************************************************************

class COperatorDlg : public CDialog
{
public:

	COperatorDlg(CWnd* pParentWnd, COperator* pOperator);

	COperator* m_pOperator;

private:

	CEdit m_LastNameEdit;
	CEdit m_FirstNameEdit;

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;
};

//***************************************************************************************
