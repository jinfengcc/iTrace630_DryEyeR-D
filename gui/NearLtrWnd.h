//530 copy from LtrWnd.cpp but change the CPSF m_Ltr to CPSF2 m_Ltr

//***************************************************************************************

#pragma once

//***************************************************************************************

#include "DispWnd.h"
#include "Btn.h"
#include "Data.h"

//***************************************************************************************

class CNearLtrWnd : public CDispWnd
{
public:

	CNearLtrWnd(RECT& Rect, int NearLtrDisType, CWnd* pWnd);

	CPSF1 m_Ltr1;
	CPSF2 m_Ltr2;
	CPSF3 m_Ltr3;
	CPSF4 m_Ltr4;
	CPSF5 m_Ltr5;

	void CreateZoomButtons();

private:

	int  m_Zoom;

	int  m_NearLtrDisType;

	virtual void RepaintMemDC() override;
	void Letter();

	CBtn m_ZoomInButton;
	CBtn m_ZoomOutButton;
	afx_msg void OnZoomInButtonClicked();
	afx_msg void OnZoomOutButtonClicked();

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
