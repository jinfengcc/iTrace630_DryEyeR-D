//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Surface.h"

//***************************************************************************************

class CMaskDlg : public CDialog
{
public:

	CMaskDlg(CWnd* pParentWnd);

	BOOL m_m[ZERNIKE_NUM_MODES];

private:

	afx_msg void OnClearAllButtonClicked();
	afx_msg void OnSelectAllButtonClicked();

	virtual BOOL OnInitDialog() override;
	virtual void OnOK() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
