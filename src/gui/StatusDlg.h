//***************************************************************************************

#pragma once

//***************************************************************************************

class CStatusDlg : public CDialog
{
public:

	CStatusDlg(CWnd* pParentWnd);

	CStatic m_LicenseTypeStatic;

private:
  CButton m_closeButton;

	BOOL OnInitDialog() override;
	void DoDataExchange(CDataExchange* pDX) override;
  void OnCancel() override;

	afx_msg void OnActivateButtonClicked();

	DECLARE_MESSAGE_MAP()

  void UpdateCloseButton();
};

//***************************************************************************************
