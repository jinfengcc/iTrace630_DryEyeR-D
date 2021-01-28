//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Data.h"

//***************************************************************************************

class CDatabaseDlg : public CDialog
{
public:

	CDatabaseDlg(CWnd* pParentWnd);
	~CDatabaseDlg();// for destory m_hAccel

	int m_DatabaseType;
	CString m_FolderName;
	CString m_FileName;

private:

	HACCEL  m_hAccel;// for ctrl+e merge database

	CButton m_MSJetRadio;
	CButton m_MySQLRadio;
	CButton m_MSJetOldRadio;
	CButton m_FolderBrowseButton;
	CButton m_FileBrowseButton;
	CButton m_ReinitButton;
	CButton m_ReadOnlyCheck;
	CButton m_DatabaseMergeButton;//

	CEdit m_FolderNameEdit;
	CEdit m_FileNameEdit;

	virtual BOOL PreTranslateMessage(MSG* pMsg) override;//

	afx_msg void OnMSJetRadioClicked();
	afx_msg void OnMySQLRadioClicked();
	afx_msg void OnMSJetOldRadioClicked();
	afx_msg void OnFolderBrowseButtonClicked();
	afx_msg void OnFileBrowseButtonClicked();
	afx_msg void OnReinitButtonClicked();
	afx_msg void OnMegrgeButtonClicked();//
	afx_msg void OnCtrlE();//

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;

	DECLARE_MESSAGE_MAP()

};

//***************************************************************************************
