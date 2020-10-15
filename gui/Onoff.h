//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MGdi.h"
#include "Image.h"

//***************************************************************************************

class COnOff : public CWnd
{
public:

	COnOff();
	~COnOff();

	real_t m_Pos;

	int  m_id;

	BOOL m_OnOff;

	void Repaint();

private:

	CMDC  m_MemDC;

	BOOL  m_HasMouse;

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs) override;

	afx_msg void OnPaint();
	afx_msg void OnMouseMove(uint nFlags, CPoint Point);
	afx_msg void OnLButtonDown(uint nFlags, CPoint Point);
	afx_msg void OnLButtonUp(uint nFlags, CPoint Point);

	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	afx_msg void OnShowWindow(BOOL bShow, uint nStatus);

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
