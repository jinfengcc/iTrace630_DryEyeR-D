//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Data.h"

//***************************************************************************************

class CGroupDlg : public CDialog
{
public:

	CGroupDlg(CWnd* pParentWnd, CGroup* pGroup);

	CGroup* m_pGroup;

private:

	CEdit m_NameEdit;

	afx_msg HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);//

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
