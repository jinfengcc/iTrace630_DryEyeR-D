//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MGdi.h"
#include "Image.h"
#include "MGdi.h"

//***************************************************************************************

class CSelBtn : public CWnd
{
public:

	CSelBtn();
	~CSelBtn();

	int  m_id;

	BOOL m_OnOff;

	CString m_Labels[2];

	void Repaint();

private:

	int   m_w;
	int   m_h;

	int   m_Half_w;

	CMDC  m_MemDC;

	BOOL  m_HasMouse1;
	BOOL  m_HasMouse2;

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs) override;

	afx_msg void OnPaint();
	afx_msg void OnMouseMove(uint nFlags, CPoint Point);
	afx_msg void OnLButtonDown(uint nFlags, CPoint Point);
	afx_msg void OnLButtonUp(uint nFlags, CPoint Point);

	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	afx_msg void OnShowWindow(BOOL bShow, uint nStatus);

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
