//***************************************************************************************

#pragma once

//***************************************************************************************

class CPasswordDlg : public CDialog
{
public:

	CPasswordDlg(CWnd* pParentWnd);

	CString m_Password;

private:

	void DoDataExchange(CDataExchange* pDX);
	BOOL OnInitDialog();
};

//***************************************************************************************
