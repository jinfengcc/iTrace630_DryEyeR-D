//***************************************************************************************

#pragma once

//***************************************************************************************

class CStatusDlg : public CDialog
{
public:

	CStatusDlg(CWnd* pParentWnd);

	CStatic m_LicenseTypeStatic;

private:

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;

	afx_msg void OnActivateButtonClicked();

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
