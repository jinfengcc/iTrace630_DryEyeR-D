//***************************************************************************************

#pragma once

//***************************************************************************************

class CSettingsDefaultDlg : public CDialog
{
public:

	CSettingsDefaultDlg(CWnd* pParentWnd);

	BOOL  m_CtlDef;
	BOOL  m_GUIDef;
	BOOL  m_ExcDef;

private:

	CButton   m_CtlDefCheck;
	CButton   m_GUIDefCheck;
	CButton   m_ExcDefCheck;

	void DoDataExchange(CDataExchange* pDX);
	BOOL OnInitDialog();

	afx_msg void OnCtlDefCheckClicked();
	afx_msg void OnGUIDefCheckClicked();
	afx_msg void OnExcDefCheckClicked();

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
