//***************************************************************************************

#pragma once

//***************************************************************************************

#include "DispWnd.h"
#include "Btn.h"
#include "Data.h"

//***************************************************************************************

class CPSFWnd : public CDispWnd
{
public:

	CPSFWnd(RECT& Rect, CWnd* pWnd);

	CPSF m_Psf;
	BOOL m_ShowEEF;

	void CreateZoomButtons();

private:

	int m_Zoom;

	virtual void RepaintMemDC() override;
	void Psf();

	CBtn m_ZoomInButton;
	CBtn m_ZoomOutButton;
	afx_msg void OnZoomInButtonClicked();
	afx_msg void OnZoomOutButtonClicked();

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
