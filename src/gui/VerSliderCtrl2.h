
//A vertical slider control by cjf

//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MGdi.h"

//***************************************************************************************

class CVerSliderCtrl2 : public CWnd
{
public:

	CVerSliderCtrl2();

	int  m_scaleNum;//nn 

	real_t m_Pos;

	real_t LowerstPos;//Presbia, the allowed lowest pos

	afx_msg void OnPaint();

	int   returnY();

	void  Refresh();

	void  Clear();//Presbia

private:

	int     m_DownX;
	int     m_DownY;

	RECT    m_PointReginon;
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs) override;

	afx_msg void OnMouseMove(uint nFlags, CPoint Point);
	afx_msg void OnLButtonDown(uint nFlags, CPoint Point);
	afx_msg void OnLButtonUp(uint nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
