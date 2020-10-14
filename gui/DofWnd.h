//***************************************************************************************

#pragma once

//***************************************************************************************

#include "DispWnd.h"
#include "Btn.h"
#include "Surface.h"
#include "CubicSpline.h"

//***************************************************************************************

class CDOFWnd : public CDispWnd
{
public:

	CDOFWnd(RECT& Rect, Matrix<real>& X, Matrix<real>& F, CWnd* pWnd);

	CCubicSpline m_Spline;

	void CreateSwitchBtn();

private:

	CMRgn m_Rgn;

	real m_F[3001];
	int m_imax;
	real m_xmax;
	real m_fmax;
	real m_fmin;

	BOOL m_abs;
	real m_thr;

	int m_pos;

	int m_w_px;
	int m_h_px;
	int m_l_px;
	int m_r_px;
	int m_t_px;
	int m_b_px;

	int m_mx_px;
	int m_my_px;

	CBtn m_SwitchButton;

	virtual void RepaintMemDC() override;

	afx_msg void OnSwitchBtnClicked();

	afx_msg void OnLButtonDown(uint nFlags, CPoint Point);
	afx_msg virtual void OnMouseMove(uint nFlags, CPoint Point) override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
