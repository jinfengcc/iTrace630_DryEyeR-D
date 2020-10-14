//***************************************************************************************

#pragma once

//***************************************************************************************


//***************************************************************************************

class CRegtimeDlg : public CDialog
{
public:

	CRegtimeDlg(CWnd* pParentWnd, CString* RegFromTime, CString*  RegToTime);

private:
	CString* m_RegFromTime;
	CString* m_RegToTime;

	CButton m_OKButton;
	CButton m_CancelButton;
	CEdit   m_FromEdit;
	CEdit   m_ToEdit;

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;
};

//***************************************************************************************
