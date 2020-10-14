//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MGdi.h"
#include "Data.h"//520 for import the patient's info for dicom file saveing

//***************************************************************************************


class CImgInfoWnd : public CWnd
{
public:

	CImgInfoWnd();
	virtual ~CImgInfoWnd();

	int m_w;
	int m_h;
	CMDC m_MemDC;

	void CreateWnd(RECT& Rect, CWnd* pParentWnd);
	void ClearMemDC();
	void Border();
	virtual void RepaintMemDC() {}
	void Repaint();


protected:

	static BOOL m_Printing;

	CMRgn m_Rgn;

	afx_msg void OnPaint();

private:

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
