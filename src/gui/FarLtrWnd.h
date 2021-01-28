/*******************************************************************
Autor:         Jinfeng Chen
E-mail:        jinfengcc@gmail.com
Version:       6.2.0
Date:          05-01-2017
Description:   Generate a new WF LTR Display which shows the Far vision for wf Near vision GUI
*****************************************************************************************/

#pragma once

//***************************************************************************************

#include "DispWnd.h"
#include "Btn.h"
#include "Data.h"

//***************************************************************************************

class CFarLtrWnd : public CDispWnd
{
public:

	CFarLtrWnd(RECT& Rect, CWnd* pWnd);

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

