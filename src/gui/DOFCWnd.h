/// The display for Depth of Focus Comparison

//迷雾汀芳，暗夜黄昏，浮动花影，适彼中秋

//寂静昭华，君子瑟琴，随云起舞，素手点星

//作者： 陈劲风

//***************************************************************************************

#pragma once

//***************************************************************************************

#include "DispWnd.h"
#include "Btn.h"
#include "Surface.h"
#include "CubicSpline.h"

//***************************************************************************************

class CDOFCWnd : public CDispWnd
{
public:

	CDOFCWnd(RECT& Rect, Matrix<real_t>& X1, Matrix<real_t>& F1, Matrix<real_t>& X2, Matrix<real_t>& F2, real_t fraction1, real_t fraction2, CWnd* pWnd);

	CCubicSpline m_Spline1;
	CCubicSpline m_Spline2;

private:

	CMRgn m_Rgn;

	real_t m_fraction1;
	real_t m_fraction2;

	real_t m_F1[3001];
	int  m_imax1;
	real_t m_xmax1;
	real_t m_fmax1;
	real_t m_fmin1;

	real_t m_F2[3001];
	int  m_imax2;
	real_t m_xmax2;
	real_t m_fmax2;
	real_t m_fmin2;

	real_t m_fmax;

	real_t m_thr1;
	real_t m_thr2;

	int  m_pos1;
	int  m_pos2;

	int  m_w_px;
	int  m_h_px;
	int  m_l_px;
	int  m_r_px;
	int  m_t_px;
	int  m_b_px;

	int  m_mx_px;
	int  m_my_px;

	CBtn m_SwitchButton;

	int    GetMaxHorValue(real_t fmax);
	virtual void RepaintMemDC() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
