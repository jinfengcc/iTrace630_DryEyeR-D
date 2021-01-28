//***************************************************************************************

#pragma once

//***************************************************************************************

#include "DispWnd.h"
#include "Btn.h"
#include "Data.h"

//***************************************************************************************

class CLtrWnd : public CDispWnd
{
public:

	CLtrWnd(RECT& Rect, CWnd* pWnd);

	CPSF m_Ltr;

	void CreateZoomButtons();

	real_t m_DysVal;//531

private:

	int  m_Zoom;

	virtual void RepaintMemDC() override;
	void Letter();

	CBtn m_ZoomInButton;
	CBtn m_ZoomOutButton;
	afx_msg void OnZoomInButtonClicked();
	afx_msg void OnZoomOutButtonClicked();

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
