//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MGdi.h"

//***************************************************************************************

class CXPSliderCtrl : public CWnd
{
public:

	CXPSliderCtrl();

	real_t m_Pos;

	void RePaint();

private:

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs) override;
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(uint nFlags, CPoint Point);
	afx_msg void OnLButtonDown(uint nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
