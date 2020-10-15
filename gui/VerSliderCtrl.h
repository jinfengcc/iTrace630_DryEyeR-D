//A vertical slider control? by cjf

//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MGdi.h"

//***************************************************************************************

class CVerSliderCtrl : public CWnd
{
public:

	CVerSliderCtrl();

	int   m_scaleNum;//nn
	real_t  m_FirstPos;//mm
	real_t  m_Pos;

	int   returnY();

	afx_msg void OnPaint();

private:

	int     m_DownX;
	int     m_DownY;

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs) override;

	afx_msg void OnMouseMove(uint nFlags, CPoint Point);
	afx_msg void OnLButtonDown(uint nFlags, CPoint Point);
	afx_msg void OnLButtonUp(uint nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
