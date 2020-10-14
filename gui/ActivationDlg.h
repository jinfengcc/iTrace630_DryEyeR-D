//***************************************************************************************

#pragma once

//***************************************************************************************

class CActivationDlg : public CDialog
{
public:

	CActivationDlg(CWnd* pParentWnd);

	CEdit m_HardwareIDEdit;
	CEdit m_SoftwareAKEdit;

private:

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;

};

//***************************************************************************************
