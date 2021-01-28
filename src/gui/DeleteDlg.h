//***************************************************************************************

#pragma once

//***************************************************************************************

//#include "Data.h"

//***************************************************************************************

class CDeleteDlg : public CDialog
{
public:

	CDeleteDlg(CWnd* pParentWnd, CString name, int type);// type=1 :Clinic; type=2 :Physician; type=3 :Operator

private:
	int m_type;
	CString m_caption;

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;
};

//***************************************************************************************
