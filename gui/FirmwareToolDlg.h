//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Hardware.h"

//***************************************************************************************

class CFirmwareToolDlg : public CDialog
{
public:

	CFirmwareToolDlg(CHW* pHW, CWnd* pParentWnd);

	CHW* m_pHW;

	CEdit m_FirmwareBuildEdit;
	CEdit m_FirmwareFileEdit;

	CButton m_BrowseButton;
	CButton m_CompareButton;
	CButton m_UpdateButton;

	afx_msg void OnBrowseButtonClicked();
	afx_msg void OnCompareButtonClicked();
	afx_msg void OnUpdateButtonClicked();

	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;
	virtual void OnCancel() override;
	virtual void OnOK() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
