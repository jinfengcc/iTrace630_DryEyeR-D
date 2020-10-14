//***************************************************************************************

#include "StdAfx.h"
#include "XPHeaderCtrl.h"
#include "XPControl.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CXPHeaderCtrl, CHeaderCtrl)

	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)

END_MESSAGE_MAP()

//***************************************************************************************

CXPHeaderCtrl::CXPHeaderCtrl()
{
	m_sortable = FALSE;
	m_sort_col = 0;
	m_sort_asc = TRUE;
	m_num_cols = 0;

	m_HasMouse = FALSE;
	m_hot_col = -1;
}

//***************************************************************************************

void CXPHeaderCtrl::OnMouseMove(uint nFlags, CPoint Point)
{
	if (!m_HasMouse) {

		m_HasMouse = TRUE;

		TRACKMOUSEEVENT Event = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, 0 };
		::TrackMouseEvent(&Event);
	}

	int hot_col = -1;
	for (int col = 0; col < m_num_cols; col++) {
		CRect Rect;
		GetItemRect(col, &Rect);
		if (Rect.PtInRect(Point)) {
			hot_col = col;
			break;
		}
	}

	if (m_hot_col != hot_col) {
		m_hot_col = hot_col;
		Invalidate(FALSE);
	}

	CHeaderCtrl::OnMouseMove(nFlags, Point);
}

//***************************************************************************************

LRESULT CXPHeaderCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_HasMouse = FALSE;
	m_hot_col = -1;

	Invalidate(FALSE);

	return 0;
}

//***************************************************************************************

void CXPHeaderCtrl::DrawItem(DRAWITEMSTRUCT* pDIS)
{
	CMDC DC;
	DC.Attach(pDIS->hDC);

	int num_cols = GetItemCount();

	int col = (int)pDIS->itemID;

	BOOL sort = m_sortable && col == m_sort_col;

	COLORREF color_text = sort ? 0x00af9f5f : 0x00000000;
	COLORREF color_arrow = 0x00af9f5f;
	COLORREF color_bk;

	RECT Rect = pDIS->rcItem;

	int state;
	if (pDIS->itemState == ODS_DISABLED) {
		state = HIS_NORMAL;
		color_bk = ::GetSysColor(COLOR_BTNFACE);
	}
	else if (pDIS->itemState == ODS_SELECTED) {
		state = HIS_PRESSED;
		color_bk = 0x00dadfde;
	}
	else if (m_HasMouse && col == m_hot_col) {
		state = HIS_HOT;
		color_bk = 0x00ffffff;
	}
	else {
		state = HIS_NORMAL;
		color_bk = ::GetSysColor(COLOR_BTNFACE);
	}

	// background
	if (!::DrawBackgintRound(XP_LIST_HEADER, m_hWnd, DC.m_hDC, HP_HEADERITEM, state, Rect)) {

		DC.FillSolidRect(&Rect, color_bk);

		if (state == HIS_PRESSED) {
			// horz top
			DC.DrawLine(Rect.left + 1, Rect.top + 2, Rect.right - 1, Rect.top + 2, 1, 0x00c9d1d0);
			DC.DrawLine(Rect.left + 1, Rect.top + 1, Rect.right - 1, Rect.top + 1, 1, 0x00b8c2c1);
			DC.DrawLine(Rect.left, Rect.top, Rect.right, Rect.top, 1, 0x0097a5a5);
			// horz bottom
			DC.DrawLine(Rect.left, Rect.bottom - 1, Rect.right, Rect.bottom - 1, 1, 0x0097a5a5);
			// vert left
			DC.DrawLine(Rect.left + 2, Rect.top + 1, Rect.left + 2, Rect.bottom - 1, 1, 0x00c9d1d0);
			DC.DrawLine(Rect.left + 1, Rect.top + 1, Rect.left + 1, Rect.bottom - 1, 1, 0x00b8c2c1);
			DC.DrawLine(Rect.left, Rect.top, Rect.left, Rect.bottom, 1, 0x0097a5a5);
			// vert right
			DC.DrawLine(Rect.right - 1, Rect.top, Rect.right - 1, Rect.bottom, 1, 0x0097a5a5);
		}
		else if (state == HIS_HOT) {
			// horz bottom
			DC.DrawLine(Rect.left + 3, Rect.bottom - 1, Rect.right - 3, Rect.bottom - 1, 1, 0x005896e3);
			DC.DrawLine(Rect.left + 2, Rect.bottom - 2, Rect.right - 2, Rect.bottom - 2, 1, 0x005896e3);
			DC.DrawLine(Rect.left + 1, Rect.bottom - 3, Rect.right - 1, Rect.bottom - 3, 1, 0x004f91e3);
		}
		else {
			// vert right
			DC.DrawLine(Rect.right - 2, Rect.top + 3, Rect.right - 2, Rect.bottom - 4, 1, 0x00b8c7cb);
			DC.DrawLine(Rect.right - 1, Rect.top + 3, Rect.right - 1, Rect.bottom - 4, 1, 0x00ffffff);
			// horz bottom
			DC.DrawLine(Rect.left, Rect.bottom - 1, Rect.right, Rect.bottom - 1, 1, 0x00b8c7cb);
			DC.DrawLine(Rect.left, Rect.bottom - 2, Rect.right, Rect.bottom - 2, 1, 0x00c2d2d6);
			DC.DrawLine(Rect.left, Rect.bottom - 3, Rect.right, Rect.bottom - 3, 1, 0x00cddee2);
		}
	}

	// text
	Rect = pDIS->rcItem;
	Rect.left += 8;
	if (sort) Rect.right -= 23;
	Rect.bottom -= 2;
	if (state == HIS_PRESSED) { Rect.left += 2; Rect.top += 2; }
	char buf[100];
	HDITEM hditem;
	hditem.mask = HDI_TEXT | HDI_FORMAT;
	hditem.pszText = (LPWSTR)buf;
	hditem.cchTextMax = 99;
	GetItem(col, &hditem);
	DC.WriteText(hditem.pszText, Rect, *m_pFont, color_text, 0);

	// sort arrow
	if (sort) {
		Rect = pDIS->rcItem;
		int r = Rect.right - 8; if (state == HIS_PRESSED) r += 2;
		int l = r - 9;
		int d = ((Rect.bottom - Rect.top) - 5) / 2;
		int t = Rect.top + d - 1; if (state == HIS_PRESSED) t += 2;
		int b = t + 5;
		if (m_sort_asc) {
			DC.DrawLine(l, b, r, b, 1, color_arrow);
			DC.DrawLine(l + 1, b - 1, r - 1, b - 1, 1, color_arrow);
			DC.DrawLine(l + 2, b - 2, r - 2, b - 2, 1, color_arrow);
			DC.DrawLine(l + 3, b - 3, r - 3, b - 3, 1, color_arrow);
			DC.DrawLine(l + 4, b - 4, r - 4, b - 4, 1, color_arrow);
		}
		else {
			DC.DrawLine(l, t + 1, r, t + 1, 1, color_arrow);
			DC.DrawLine(l + 1, t + 2, r - 1, t + 2, 1, color_arrow);
			DC.DrawLine(l + 2, t + 3, r - 2, t + 3, 1, color_arrow);
			DC.DrawLine(l + 3, t + 4, r - 3, t + 4, 1, color_arrow);
			DC.DrawLine(l + 4, t + 5, r - 4, t + 5, 1, color_arrow);
		}
	}

	DC.Detach();
}

//***************************************************************************************
