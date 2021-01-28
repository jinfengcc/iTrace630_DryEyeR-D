//530 wf controller window

//***************************************************************************************

#pragma once

//***************************************************************************************
#include "MemWnd.h"
#include "Btn.h"

//***************************************************************************************

class CExamCaptureWnd : public CMemWnd
{
public:
	CExamCaptureWnd();
	~CExamCaptureWnd();

	void CreateThisWnd(RECT& Rect, CWnd* pWnd);

	void  ShowGUI();
	void  HideGUI();

	BOOL  m_ShowWindow;

	BOOL  m_Acquire;

private:

	int     m_w;
	int     m_h;
	int     m_FontSize;

	CBtn    m_CapBtn;// Capture Btun

	virtual void RepaintMemDC() override;

	afx_msg void OnCaptureBtnClicked();

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
