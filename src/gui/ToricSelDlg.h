//***************************************************************************************

#pragma once

//***************************************************************************************


//***************************************************************************************

class CToricSelDlg : public CDialog
{
public:

	CToricSelDlg(CWnd* pParentWnd);

	BOOL   m_pre;//True: The customer chose the data as the pre-op data

private:

	CButton m_Pre_Radio;//IDC_PREOP_RADIO
	CButton m_Post_Radio;//IDC_POSTOP_RADIO
	CButton m_OKButton;
	CButton m_CancelButton;


	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;

	afx_msg void OnPreRadioClicked();
	afx_msg void OnPostRadioClicked();

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
