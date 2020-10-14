//***************************************************************************************

#pragma once

//***************************************************************************************

#include "DispWnd.h"
#include "Btn.h"
#include "Data.h"

//***************************************************************************************

class CSignalWnd : public CDispWnd
{
public:

	CSignalWnd(RECT& Rect, CWnd* pWnd);

	CWFExam* m_pWFExam;
	BOOL     m_Horz;
	int      m_Zoom;

	void CreateZoomButtons();

private:

	virtual void RepaintMemDC() override;
	void Signal();

	CBtn m_ZoomInButton;
	CBtn m_ZoomOutButton;
	afx_msg void OnZoomInButtonClicked();
	afx_msg void OnZoomOutButtonClicked();

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
