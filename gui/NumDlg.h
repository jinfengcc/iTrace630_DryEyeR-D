//***************************************************************************************

#pragma once

//***************************************************************************************

class CNumDlg : public CDialog
{
public:

	CNumDlg(CWnd* pParentWnd, const CString& title);

	real_t m_number;
	CString m_title;

private:

	CEdit m_NumberEdit;

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;
};

//***************************************************************************************

BOOL EnterNumber(real_t& number, CWnd* parent, const CString& title);

//***************************************************************************************
