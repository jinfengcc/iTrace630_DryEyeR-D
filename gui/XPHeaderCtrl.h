//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MGdi.h"

//***************************************************************************************

class CXPHeaderCtrl : public CHeaderCtrl
{
public:

	CXPHeaderCtrl();

	int m_num_cols;
	BOOL m_sortable;
	int  m_sort_col;
	BOOL m_sort_asc;

	void SetFont(CMFont* pFont) { m_pFont = pFont; }

private:

	CMFont* m_pFont;

	BOOL m_HasMouse;
	int m_hot_col;

	void OnMouseMove(uint nFlags, CPoint Point);
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	void DrawItem(DRAWITEMSTRUCT* pDIS);

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
