//***************************************************************************************

#include "StdAfx.h"
#include "XPListCtrla.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CXPListCtrla, CListCtrl)

	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_MEASUREITEM_REFLECT()

END_MESSAGE_MAP()

//***************************************************************************************

CXPListCtrla::CXPListCtrla()
{
	m_HasMouse = FALSE;
	m_HotRow = -1;
	m_FelRow = -1;

	m_SelRowColor = 0x008f7f6f;//0x009f8f4f;
	m_FelRowColor = 0x00efdfcf;
	m_HotRowColor = 0x00efffdf;//0x00ffefcf;
}

//***************************************************************************************

void CXPListCtrla::CreateWnd(ulong Style, const RECT& Rect, CWnd* pParentWnd, uint ID)
{
	Style |= WS_CHILD | WS_BORDER | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED;

	Create(Style, Rect, pParentWnd, ID);

	SetExtendedStyle(LVS_EX_GRIDLINES); // vertical lines separating columns

	SetHoverTime(400000);

	SetBkColor(0x00f0f6f6);

	m_HeaderCtrl.SubclassWindow(GetHeaderCtrl()->GetSafeHwnd());
}

//***************************************************************************************

void CXPListCtrla::InsertColumn(const int n, LPWSTR Name, const int Width)
{
	LVCOLUMN Column;

	Column.mask = LVCF_ORDER | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	Column.fmt = 0;
	Column.cx = Width;
	Column.pszText = Name;
	Column.cchTextMax = lstrlenW(Name);
	Column.iSubItem = -1;
	Column.iOrder = n;
	Column.iImage = 0;

	CListCtrl::InsertColumn(n, &Column);

	// set owner draw bit
	HDITEM hditem;
	hditem.mask = HDI_FORMAT;
	m_HeaderCtrl.GetItem(n, &hditem);
	hditem.fmt |= HDF_OWNERDRAW;
	m_HeaderCtrl.SetItem(n, &hditem);

	m_HeaderCtrl.m_num_cols++;
}

//***************************************************************************************

void CXPListCtrla::SetSortItem(const int sort_col)
{
	if (m_HeaderCtrl.m_sort_col == sort_col)
	{
		m_HeaderCtrl.m_sort_asc = !m_HeaderCtrl.m_sort_asc;
	}
	else
	{
		m_HeaderCtrl.m_sort_col = sort_col;
	}

	m_HeaderCtrl.Invalidate(FALSE);
}

//***************************************************************************************

void CXPListCtrla::OnLButtonDown(uint nFlags, CPoint Point)
{
	int row = HitTest(Point);
	if (row == -1) return;

	CRect Rect;
	GetItemRect(row, &Rect, LVIR_BOUNDS);
	if (!Rect.PtInRect(Point)) return;

	CListCtrl::OnLButtonDown(nFlags, Point);
}

//***************************************************************************************

void CXPListCtrla::OnRButtonDown(uint nFlags, CPoint Point)
{
	int row = HitTest(Point);
	if (row == -1) return;

	CRect Rect;
	GetItemRect(row, &Rect, LVIR_BOUNDS);
	if (!Rect.PtInRect(Point)) return;

	CListCtrl::SetItemState(row, 0, LVIS_SELECTED | LVIS_FOCUSED);
}

//***************************************************************************************

void CXPListCtrla::OnLButtonDblClk(uint nFlags, CPoint Point)
{
	int row = HitTest(Point);
	if (row == -1) return;

	CRect Rect;
	GetItemRect(row, &Rect, LVIR_BOUNDS);
	if (!Rect.PtInRect(Point)) return;

	CListCtrl::OnLButtonDblClk(nFlags, Point);
}

//***************************************************************************************

void CXPListCtrla::OnRButtonDblClk(uint nFlags, CPoint Point)
{
	int row = HitTest(Point);
	if (row == -1) return;

	CRect Rect;
	GetItemRect(row, &Rect, LVIR_BOUNDS);
	if (!Rect.PtInRect(Point)) return;

	CListCtrl::OnRButtonDblClk(nFlags, Point);
}

//***************************************************************************************

void CXPListCtrla::OnMouseMove(uint nFlags, CPoint Point)
{
	if (!m_HasMouse)
	{
		m_HasMouse = TRUE;

		TRACKMOUSEEVENT event = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, 0 };
		::TrackMouseEvent(&event);
	}

	CRect Rect;
	int row = HitTest(Point);

	if (row != -1)
	{
		GetItemRect(row, &Rect, LVIR_BOUNDS);
		if (!Rect.PtInRect(Point)) row = -1;
	}

	if (m_HotRow != row)
	{
		if (row != -1)
		{
			GetItemRect(row, &Rect, LVIR_BOUNDS);
			InvalidateRect(&Rect, FALSE);
		}
		if (m_HotRow != -1)
		{
			GetItemRect(m_HotRow, &Rect, LVIR_BOUNDS);
			InvalidateRect(&Rect, FALSE);
		}
		m_HotRow = row;
	}

	CListCtrl::OnMouseMove(nFlags, Point);
}

//***************************************************************************************

LRESULT CXPListCtrla::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if (m_HotRow != -1)
	{
		RECT Rect;
		GetItemRect(m_HotRow, &Rect, LVIR_BOUNDS);
		InvalidateRect(&Rect, FALSE);
		m_HotRow = -1;
	}

	m_HasMouse = FALSE;

	return 0;
}

//***************************************************************************************

void CXPListCtrla::MeasureItem(MEASUREITEMSTRUCT* pMIS)
{
	pMIS->itemHeight = intRound(1.1 * m_pFont->m_Height);
}

//***************************************************************************************

void CXPListCtrla::DrawItem(DRAWITEMSTRUCT* pDIS)
{
	CMDC DC;
	DC.Attach(pDIS->hDC);

	//pDIS->itemState == ODS_SELECTED - doesn't work
	BOOL Selected = FALSE;
	POSITION pos = GetFirstSelectedItemPosition();
	while (pos)
	{
		if (pDIS->itemID == GetNextSelectedItem(pos))
		{
			Selected = TRUE;
			break;
		}
	}

	COLORREF BkColor = 0x00ffffff;
	COLORREF TextColor = 0x00000000;
	if (Selected)
	{
		BkColor = m_SelRowColor;
		TextColor = 0x00ffffff;
	}
	else if (pDIS->itemID == m_HotRow)
	{
		BkColor = m_HotRowColor;
		TextColor = 0x00000000;
	}
	else if (pDIS->itemID == m_FelRow)
	{
		BkColor = m_FelRowColor;
		TextColor = 0x00000000;
	}

	CRect Rect = pDIS->rcItem;
	Rect.right--;
	Rect.bottom--;
	DC.DrawRectangle(Rect, BkColor, BkColor);

	for (int col = 0; col < m_HeaderCtrl.m_num_cols; col++)
	{
		LVCOLUMN Column;
		memset(&Column, 0, sizeof(Column));
		Column.mask = LVCF_WIDTH;
		GetColumn(col, &Column);

		// GetSubItemRect возвращает для col==0 rectangle всей строки, а не нулевой ячейки
		GetSubItemRect(pDIS->itemID, col, LVIR_BOUNDS, Rect);
		Rect.right = Rect.left + Column.cx - 1;
		Rect.left += 3;
		DC.WriteText(GetItemText(pDIS->itemID, col), Rect, *m_pFont, TextColor, 0);
	}

	DC.Detach();
}

//***************************************************************************************
