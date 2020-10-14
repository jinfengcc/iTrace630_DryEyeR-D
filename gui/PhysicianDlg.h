//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Data.h"

//***************************************************************************************

class CPhysicianDlg : public CDialog
{
public:

	CPhysicianDlg(CWnd* pParentWnd, CPhysician* pPhysician);

	CPhysician* m_pPhysician;

private:

	CEdit m_LastNameEdit;
	CEdit m_FirstNameEdit;

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;
};

//***************************************************************************************
