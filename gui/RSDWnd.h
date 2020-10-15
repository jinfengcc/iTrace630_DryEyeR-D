//***************************************************************************************

#pragma once

//***************************************************************************************

#include "DispWnd.h"
#include "Btn.h"
#include "Data.h"

//***************************************************************************************

class CRSDWnd : public CDispWnd
{
public:

	CRSDWnd(RECT& Rect, CWnd* pWnd);

	CWFExam*        m_pWFExam;
	CZernikeSurface m_Surface;
	int             m_Zoom;
	real_t            m_AddedSphEq;

	void CreateZoomButtons();

private:

	real_t m_cursor_r_um;
	real_t m_cursor_a_rd;

	virtual void RepaintMemDC() override;
	void Grid();
	void Points();
	void Cursor();

	CBtn m_ZoomInButton;
	CBtn m_ZoomOutButton;
	afx_msg void OnZoomInButtonClicked();
	afx_msg void OnZoomOutButtonClicked();

	afx_msg LRESULT OnChangePosMsg(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
