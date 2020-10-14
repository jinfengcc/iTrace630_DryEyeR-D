//***************************************************************************************

#include "StdAfx.h"

#include "OnOff.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(COnOff, CWnd)

	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()

	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)

END_MESSAGE_MAP()

//***************************************************************************************

COnOff::COnOff()
{
	m_HasMouse = FALSE;
}

//***************************************************************************************

COnOff::~COnOff()
{
}

//***************************************************************************************

BOOL COnOff::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = ::AfxRegisterWndClass(CS_DBLCLKS, ::LoadCursor(NULL, IDC_HAND), NULL, NULL);
	cs.style = cs.style | WS_CHILD;
	cs.dwExStyle = 0;

	m_MemDC.CreateTrueColorDC(cs.cx, cs.cy);

	return CWnd::PreCreateWindow(cs);
}

//***************************************************************************************

void COnOff::OnShowWindow(BOOL bShow, uint nStatus)
{
	if (bShow) Repaint();

	CWnd::OnShowWindow(bShow, nStatus);
}

//***************************************************************************************

void COnOff::OnPaint()
{
	CPaintDC PaintDC(this);

	CMDC DC;
	DC.Attach(PaintDC.Detach());

	RECT Rect, OutLineRect, InBtnRect;
	GetClientRect(&Rect);

	if (m_HasMouse)
	{
		DC.DrawRectangle(Rect, YELLOW, YELLOW);
		::SetRect(&OutLineRect, Rect.left + 1, Rect.top + 1, Rect.right - 1, Rect.bottom - 1);
		DC.DrawRectangle(OutLineRect, WHITE, WHITE);
	}
	else DC.DrawRectangle(Rect, WHITE, WHITE);

	int w = Rect.right - Rect.left + 1;
	int h = Rect.bottom - Rect.top + 1;
	int e = (w - 8) / 2;
	int d = h / 2;

	//out  
	::SetRect(&OutLineRect, Rect.left + 2, Rect.top + 2, Rect.right - 2, Rect.bottom - 2);
	DC.DrawRectangle(OutLineRect, GRAY, GRAY);

	::SetRect(&OutLineRect, Rect.left + 4, Rect.top + 4, Rect.right - 4, Rect.bottom - 4);
	DC.DrawRectangle(OutLineRect, CYAN, CYAN);

	::SetRect(&OutLineRect, Rect.left + 2, Rect.top + 2, Rect.right - 2, Rect.bottom - 2);
	DC.DrawRectangle(OutLineRect, GRAY, GRAY);

	::SetRect(&OutLineRect, Rect.left + 4, Rect.top + 4, Rect.right - 4, Rect.bottom - 4);
	DC.DrawRectangle(OutLineRect, CYAN, CYAN);
	//

	//In Button
	int x0 = intRound(Rect.left + 4 + (w - 8)* m_Pos);
	if (m_Pos == 0) x0 += 1;
	if (m_Pos == 0.5) x0 -= 1;

	int x1 = x0 + e;

	int se1 = x0 + intRound(e / 2) - 1;
	int se0 = se1 - intRound(e / 5);
	int se2 = se1 + intRound(e / 5);

	::SetRect(&InBtnRect, x0, Rect.top + 5, x1, Rect.bottom - 5);
	DC.DrawRectangle(InBtnRect, MEDIUM_GRAY, MEDIUM_GRAY);

	DC.DrawLine(x0, Rect.top + 5, x0, Rect.bottom - 5, 2, DARK_GRAY);//left ver-line  
	DC.DrawLine(x0, Rect.top + 5, x1, Rect.top + 5, 3, WHITE);//up hor-line
	DC.DrawLine(x1 - 3, Rect.top + 5, x1 - 3, Rect.bottom - 8, 3, WHITE);//right ver-line
	DC.DrawLine(x0, Rect.bottom - 5, x1 - 2, Rect.bottom - 5, 4, DARK_GRAY);//down hor-line

	if (m_Pos == 0.5)
		DC.DrawLine(x1, Rect.top + 4, x1, Rect.bottom - 3, 1, DARK_GRAY);//right ver- Dark_line
	else DC.DrawLine(x1, Rect.top + 4, x1, Rect.bottom - 3, 1, GRAY);//right ver- Dark_line

	DC.DrawLine(se0, Rect.top + d - 2, se0, Rect.top + d + 2, 1, BLUE);
	DC.DrawLine(se1, Rect.top + d - 2, se1, Rect.top + d + 2, 1, BLUE);
	DC.DrawLine(se2, Rect.top + d - 2, se2, Rect.top + d + 2, 1, BLUE);
	//In Button Done

	PaintDC.Attach(DC.Detach());
}

//***************************************************************************************

LRESULT COnOff::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_HasMouse = FALSE;

	if (m_Pos != 0 && m_Pos != 0.5)
	{
		if (m_Pos < 0.25)
		{
			m_OnOff = TRUE;
			m_Pos = 0;
		}
		else
		{
			m_OnOff = FALSE;
			m_Pos = 0.5;
		}

		Repaint();

		GetParent()->PostMessage(WM_ONOFF_POS_CHANGED, (WPARAM)this, m_id);
	}
	else Invalidate(FALSE);

	return 0;
}

//***************************************************************************************

LRESULT COnOff::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

//***************************************************************************************

void COnOff::Repaint()
{
	Invalidate(FALSE);
}

//***************************************************************************************

void COnOff::OnLButtonDown(uint nFlags, CPoint Point)
{
	OnMouseMove(nFlags, Point);
}

//***************************************************************************************

void COnOff::OnLButtonUp(uint nFlags, CPoint Point)
{
	if (MK_LBUTTON)
	{
		m_OnOff = !m_OnOff;

		if (m_OnOff)
		{
			m_Pos = 0;
		}
		else
		{
			m_Pos = 0.5;
		}

		Repaint();

		GetParent()->PostMessage(WM_ONOFF_POS_CHANGED, (WPARAM)this, m_id);
	}
}

//***************************************************************************************

void COnOff::OnMouseMove(uint nFlags, CPoint Point)
{
	m_HasMouse = TRUE;

	TRACKMOUSEEVENT Event = { sizeof(TRACKMOUSEEVENT), TME_LEAVE | TME_HOVER, m_hWnd, 500/*ms*/ };
	::TrackMouseEvent(&Event);

	if (nFlags & MK_LBUTTON)
	{
		RECT Rect;
		GetClientRect(&Rect);

		int w = Rect.right - Rect.left + 1;
		int h = Rect.bottom - Rect.top + 1;
		int d = h / 2;

		int x1 = Rect.left + d;
		int x2 = Rect.right - d;

		if (x1 <= Point.x && Point.x <= x2)
		{
			m_Pos = (real)(Point.x - x1) / (2 * (w - d - d));
		}
	}

	Repaint();
}

//***************************************************************************************
