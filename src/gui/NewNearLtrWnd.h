/*******************************************************************
Autor:         Jinfeng Chen
E-mail:        jinfengcc@gmail.com
Version:       6.2.0
Date:          05-01-2017
Description:   Generate a new WF Near LTR Display which shows the Near vision for wf Near vision GUI
*****************************************************************************************/

#pragma once

//***************************************************************************************

#include "DispWnd.h"
#include "Btn.h"
#include "Data.h"

//***************************************************************************************

class CNewNearLtrWnd : public CDispWnd
{
public:

	CNewNearLtrWnd(RECT& Rect, real um_mn, CWnd* pWnd);

	CNearPSF m_NearLtr;

	void CreateZoomButtons();

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

