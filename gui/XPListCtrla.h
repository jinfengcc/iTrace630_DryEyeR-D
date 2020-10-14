//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MGdi.h"
#include "XPHeaderCtrl.h"

//***************************************************************************************

class CXPListCtrla : public CListCtrl
{
public:

	CXPListCtrla();

	CXPHeaderCtrl m_HeaderCtrl;

	void CreateWnd(ulong Style, const RECT& Rect, CWnd* pParentWnd, uint ID);
	void InsertColumn(const int n, LPWSTR Name, const int Width);
	void SetSortItem(const int sort_col);

	void SetFont(CMFont* pFont) { m_pFont = pFont; m_HeaderCtrl.SetFont(pFont); }

	int m_FelRow;

	COLORREF m_SelRowColor;
	COLORREF m_FelRowColor;
	COLORREF m_HotRowColor;

private:

	CMFont* m_pFont;

	BOOL m_HasMouse;
	int m_HotRow;

	void OnLButtonDblClk(uint nFlags, CPoint Point);
	void OnLButtonDown(uint nFlags, CPoint Point);
	void OnRButtonDblClk(uint nFlags, CPoint Point);
	void OnRButtonDown(uint nFlags, CPoint Point);
	void OnMouseMove(uint nFlags, CPoint Point);
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	void MeasureItem(MEASUREITEMSTRUCT* pMIS);
	void DrawItem(DRAWITEMSTRUCT* pDIS);

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
