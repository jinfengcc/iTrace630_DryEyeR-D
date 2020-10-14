//***************************************************************************************

#pragma once

//***************************************************************************************

#include "DispWnd.h"
#include "Btn.h"
#include "Surface.h"

//***************************************************************************************

class CRMSWnd : public CDispWnd
{
public:

	CRMSWnd(RECT& Rect, CWnd* pWnd);

	CZernikeSurface m_Surface;
	BOOL            m_Individual;
	int             m_Max;

	void CreateZoomButtons();
	void CreateZoomButtons2();//530

	BOOL  m_SimpliedRMS;//531, Is this wnd are sub-win of WFCT Summary(Chang Analysis)

	CBtn m_ZoomInButton;
	CBtn m_ZoomOutButton;

private:

	virtual void RepaintMemDC() override;
	void RmsInd2D();
	void RmsCmb2D();
	void RmsInd3D();


	afx_msg void OnShowSimpRmsButtonClicked();//530
	afx_msg void OnZoomInButtonClicked();
	afx_msg void OnZoomOutButtonClicked();

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
