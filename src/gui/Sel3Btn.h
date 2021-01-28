//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MGdi.h"
#include "TipWnd.h"

//***************************************************************************************

class CSel3Btn : public CWnd
{
public:

	CSel3Btn();
	~CSel3Btn();

	int  m_id;

	int  m_OnOff;

	CString m_Labels[3];

	BOOL m_SmallFont[3];

	void SetTip(const CString& Tip0, const CString& Tip1, const CString& Tip2) { m_Tip[0] = Tip0; m_Tip[1] = Tip1; m_Tip[2] = Tip2; }//
	void Repaint();

private:

	int   m_w;
	int   m_h;

	int   m_Third_w;

	int   m_CurrentTip;
	RECT  m_LeftRect;
	RECT  m_MidRect;
	RECT  m_RightRect;

	CString m_Tip[3];

	CMDC  m_MemDC;

	BOOL  m_HasMouse1;
	BOOL  m_HasMouse2;
	BOOL  m_HasMouse3;

	CTipWnd* m_pTipWnd;
	void ShowTip(int num);
	void HideTip();

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs) override;

	afx_msg void OnPaint();
	afx_msg void OnMouseMove(uint nFlags, CPoint Point);
	afx_msg void OnLButtonDown(uint nFlags, CPoint Point);
	afx_msg void OnLButtonUp(uint nFlags, CPoint Point);

	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);

	afx_msg void OnShowWindow(BOOL bShow, uint nStatus);

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
