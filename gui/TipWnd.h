//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MGdi.h"
#include "Image.h"

//***************************************************************************************

class CTipWnd : public CWnd
{
public:

	CTipWnd(RECT& Rect, CWnd* pWnd, const CString& Tip);

private:

	CString m_Tip;

	afx_msg int OnCreate(CREATESTRUCT* cs);
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
