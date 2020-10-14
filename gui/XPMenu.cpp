//***************************************************************************************

#include "StdAfx.h"
#include "XPMenu.h"
#include "MGdi.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

CXPMenuItem::CXPMenuItem()
{
	m_pSubmenu = NULL;
}

CXPMenuItem::~CXPMenuItem()
{
	if (m_pSubmenu) delete m_pSubmenu;
}

//***************************************************************************************

CXPMenu::CXPMenu()
{
	int h = ::GetSystemMetrics(SM_CYSCREEN);
	m_item_w = intRound(0.280 * h);
	m_item_h = intRound(0.028 * h);

	CreatePopupMenu();
}

//***************************************************************************************

CXPMenu::~CXPMenu()
{
	DestroyMenu();
}

//***************************************************************************************

void CXPMenu::AppendSeparator()
{
	AppendMenu(MF_SEPARATOR);
}

//***************************************************************************************

void CXPMenu::AppendItem(const uint id, const BOOL radio, const BOOL check, const CString& ltext, const CString& rtext)
{
	CXPMenuItem *pItem = m_Items.Append();

	pItem->m_id = id;
	pItem->m_radio = radio;
	pItem->m_check = check;
	pItem->m_ltext = ltext;
	pItem->m_rtext = rtext;
	pItem->m_pSubmenu = NULL;

	AppendMenuW(MF_OWNERDRAW, (uint)pItem->m_id, (LPCTSTR)pItem);
}

//***************************************************************************************

CXPMenu* CXPMenu::AppendSubmenu(const CString& ltext, const CString& rtext)
{
	CXPMenuItem* pItem = m_Items.Append();

	pItem->m_id = 0;
	pItem->m_radio = FALSE;
	pItem->m_check = FALSE;
	pItem->m_ltext = ltext;
	pItem->m_rtext = rtext;
	pItem->m_pSubmenu = new CXPMenu();

	AppendMenuW(MF_OWNERDRAW | MF_POPUP, (uint)pItem->m_pSubmenu->m_hMenu, (LPCTSTR)pItem);

	return pItem->m_pSubmenu;
}

//***************************************************************************************

void CXPMenu::MeasureItem(MEASUREITEMSTRUCT* pMeasureItemStruct)
{
	pMeasureItemStruct->itemWidth = m_item_w;
	pMeasureItemStruct->itemHeight = m_item_h;
}

//***************************************************************************************

void CXPMenu::DrawItem(DRAWITEMSTRUCT* pDrawItemStruct)
{
	CXPMenuItem* pItem = (CXPMenuItem*)pDrawItemStruct->itemData;
	if (!pItem) return;

	CMDC DC;
	DC.Attach(pDrawItemStruct->hDC);

	RECT Rect = pDrawItemStruct->rcItem;
	int W = Rect.right - Rect.left;
	int H = Rect.bottom - Rect.top;

	BOOL hot = pDrawItemStruct->itemState & ODS_SELECTED;

	// rectangle
	COLORREF col = ::GetSysColor(COLOR_MENU);

	if (hot)
	{
		uchar r1 = (uchar)(0x000000db * (ulong)GetRValue(col) / 0x000000ff);
		uchar g1 = (uchar)(0x000000df * (ulong)GetGValue(col) / 0x000000ff);
		uchar b1 = (uchar)(0x000000cf * (ulong)GetBValue(col) / 0x000000ff);
		uchar r2 = (uchar)(0x0000009f * (ulong)GetRValue(col) / 0x000000ff);
		uchar g2 = (uchar)(0x0000009c * (ulong)GetGValue(col) / 0x000000ff);
		uchar b2 = (uchar)(0x0000009a * (ulong)GetBValue(col) / 0x000000ff);
		DC.DrawRectangle(Rect, RGB(r2, g2, b2), RGB(r1, g1, b1));
	}
	else
	{
		DC.FillSolidRect(&Rect, col);
	}

	// dot
	if (pItem->m_radio)
	{
		int x = Rect.left + 12;
		int y = (Rect.top + Rect.bottom) / 2;
		col = hot ? 0x000000af : 0x000000af;
		DC.DrawCircle(x, y, 2, 1, col, col);
	}

	// check
	else if (pItem->m_check)
	{
		int x = Rect.left + 9;
		int y = Rect.top + 9;
		col = hot ? 0x00009f00 : 0x00009f00;
		DC.DrawLine(x, y, x, y + 3, 1, col);
		++x; ++y; DC.DrawLine(x, y, x, y + 3, 1, col);
		++x; ++y; DC.DrawLine(x, y, x, y + 3, 1, col);
		++x; --y; DC.DrawLine(x, y, x, y + 3, 1, col);
		++x; --y; DC.DrawLine(x, y, x, y + 3, 1, col);
		++x; --y; DC.DrawLine(x, y, x, y + 3, 1, col);
		++x; --y; DC.DrawLine(x, y, x, y + 3, 1, col);
	}

	CMFont Font(intRound(0.7 * m_item_h), 400, "Arial");
	Rect.left += 27;
	Rect.right -= 5;
	DC.WriteText(pItem->m_ltext, Rect, Font, BLACK, 0);
	DC.WriteText(pItem->m_rtext, Rect, Font, GRAY, 2);

	DC.Detach();
}

//***************************************************************************************
