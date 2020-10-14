//***************************************************************************************

#pragma once

//***************************************************************************************

class CDatabaseDestDlg : public CDialog
{
public:

	CDatabaseDestDlg(CWnd* pParentWnd, CString DefaultDabaseFile);
	CString GetDestFile() { return m_FileName; }
private:

	CString m_FileName;
	CEdit m_FileNameEdit;

	CButton m_FileBrowseButton;

	afx_msg void OnFileBrowseButtonClicked();

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************

