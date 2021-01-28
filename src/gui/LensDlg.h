//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Image.h"
#include "MGDI.h"

//***************************************************************************************

class CLensDlg : public CDialog
{
public:

	CLensDlg(CWnd* pParentWnd, CEyeImage* pImage);

private:

	CEyeImage* m_pImage;

	real_t m_le_x_um[3];
	real_t m_le_y_um[3];

	int m_w;
	int m_h;
	int m_l;
	int m_t;
	real_t m_w_um;
	real_t m_h_um;
	real_t m_x_px_um;
	real_t m_y_px_um;
	real_t m_cx;
	real_t m_cy;

	CMDC m_MemDC;
	CRgn m_Rgn;

	void DrawDots();

	int m_d;
	CButton m_Dot1Radio;
	CButton m_Dot2Radio;
	CButton m_Dot3Radio;
	afx_msg void OnDotRadioClicked();

	CButton m_OKButton;
	CButton m_CancelButton;

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;

	afx_msg void OnPaint();

	afx_msg void OnMouseMove(uint nFlags, CPoint Point);
	afx_msg void OnLButtonDown(uint nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
