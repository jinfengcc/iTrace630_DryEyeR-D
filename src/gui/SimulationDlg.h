//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Surface.h"

//***************************************************************************************

class CSimulationDlg : public CDialog
{
public:

	CSimulationDlg(CWnd* pParentWnd);

	CZernikeSurface m_Surface;
	int m_Eye;

private:

	CButton m_ODRadio;
	CButton m_OSRadio;

	afx_msg void OnLoadButtonClicked();
	afx_msg void OnPaint();

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
