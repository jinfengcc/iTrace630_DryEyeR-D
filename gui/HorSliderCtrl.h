/*******************************************************************
Autor:         Jinfeng Chen
E-mail:        jinfengcc@gmail.com
Version:       6.2.0
Date:          04-25-2017
Description:   Generate a new Horizontal slider control component which used for the WF Near and Far vision control
*****************************************************************************************/

#pragma once

//***************************************************************************************

#include "MGdi.h"

//***************************************************************************************

typedef struct _HSliderParameters
{
	real            Pos;
	int             scaleNum;//Max scaleNum is 100
	real            WideRatio;//The wide of outline rectengle
	int             ScaleType;//0: Scale in the up of splider line  1: Scale in the down of line 2: Scale in the up and down of line
	int             ScaleLength[100];//Max scaleNum is 100

	RECT            SliderRect;

	CString         Title;
	CString         UpLabels[100];
	CString         DownLabels[100];

	BOOL            OutlineHighlight[4];// highlight the outline of rectenge, 0: left line 1: top line 2:right line 3:bottom line

} HSliderParameters;


//***************************************************************************************

class CHorSliderCtrl : public CWnd
{
public:

	CHorSliderCtrl();
	~CHorSliderCtrl();

	BOOL    m_Printing;
	HSliderParameters m_Pars;

	void SetParameters(HSliderParameters Pars);

	afx_msg void OnPaint();

private:

	int     m_DownX;
	int     m_DownY;

	int     m_LastDownX;
	int     m_LastDownY;

	int     m_w;
	int     m_h;

	int     m_Left;
	int     m_Right;

	real    m_SliderWidth;
	real    m_halfScale;
	int     m_HalfWidth;

	long    m_RGBSize;
	BOOL    m_Draw;
	uchar*  m_MemBackup;

	BOOL    m_Mousemove;

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs) override;

	afx_msg void OnMouseMove(uint nFlags, CPoint Point);
	afx_msg void OnLButtonDown(uint nFlags, CPoint Point);
	afx_msg void OnLButtonUp(uint nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
