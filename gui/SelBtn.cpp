//***************************************************************************************

#include "StdAfx.h"

#include "SelBtn.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CSelBtn, CWnd)

	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()

	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)

END_MESSAGE_MAP()

//***************************************************************************************

CSelBtn::CSelBtn()
{
	m_HasMouse1 = FALSE;
	m_HasMouse2 = FALSE;
}

//***************************************************************************************

CSelBtn::~CSelBtn()
{
}

//***************************************************************************************

BOOL CSelBtn::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = ::AfxRegisterWndClass(CS_DBLCLKS, ::LoadCursor(NULL, IDC_HAND), NULL, NULL);
	cs.style = cs.style | WS_CHILD;
	cs.dwExStyle = 0;

	m_MemDC.CreateTrueColorDC(cs.cx, cs.cy);

	return CWnd::PreCreateWindow(cs);
}

//***************************************************************************************

void CSelBtn::OnShowWindow(BOOL bShow, uint nStatus)
{
	if (bShow) Repaint();

	CWnd::OnShowWindow(bShow, nStatus);
}

//***************************************************************************************

void CSelBtn::OnPaint()
{
	CPaintDC PaintDC(this);

	CMDC DC;
	DC.Attach(PaintDC.Detach());

	RECT Rect, LeftRect, RightRect, LeftOutLineRect, RightOutLineRect;
	GetClientRect(&Rect);

	m_w = Rect.right - Rect.left;
	m_h = Rect.bottom - Rect.top;

	m_Half_w = intRound(real_t(m_w) / 2);

	::SetRect(&LeftRect, 0, Rect.top, m_Half_w, Rect.bottom);
	::SetRect(&RightRect, m_Half_w, Rect.top, m_w, Rect.bottom);

	if (m_HasMouse1 || m_HasMouse2)
	{
		if (m_HasMouse1)
		{
			DC.DrawRectangle(RightRect, BLACK, BLACK);
			DC.DrawRectangle(LeftRect, YELLOW, YELLOW);
		}
		else
		{
			DC.DrawRectangle(LeftRect, BLACK, BLACK);
			DC.DrawRectangle(RightRect, YELLOW, YELLOW);
		}
	}
	else
	{
		DC.DrawRectangle(Rect, BLACK, BLACK);
		DC.DrawLine(m_Half_w, Rect.top, m_Half_w, Rect.bottom, 1, BLACK);
	}

	int w = Rect.right - Rect.left + 1;
	int h = Rect.bottom - Rect.top + 1;
	int e = (w - 8) / 2;
	int d = h / 2;

	//out  
	::SetRect(&LeftOutLineRect, LeftRect.left + 1, LeftRect.top + 1, LeftRect.right - 1, LeftRect.bottom - 1);
	DC.DrawRectangle(LeftOutLineRect, DARK_GRAY, DARK_GRAY);

	::SetRect(&RightOutLineRect, RightRect.left + 1, RightRect.top + 1, RightRect.right - 1, RightRect.bottom - 1);
	DC.DrawRectangle(RightOutLineRect, DARK_GRAY, DARK_GRAY);

	CMFont Font(LeftOutLineRect.bottom - LeftOutLineRect.top - 10, 400, "Arial");

	//In Button 
	if (m_OnOff)
	{
		DC.DrawRectangle(LeftOutLineRect, WHITE, CYAN);
		DC.DrawLine(LeftOutLineRect.left + 1, LeftOutLineRect.top + 1, LeftOutLineRect.right - 2, LeftOutLineRect.top + 1, 3, WHITE);
		DC.DrawLine(LeftOutLineRect.right - 3, LeftOutLineRect.top + 3, LeftOutLineRect.right - 3, LeftOutLineRect.bottom - 4, 3, BLACK);
		DC.DrawLine(LeftOutLineRect.left + 1, LeftOutLineRect.bottom - 4, LeftOutLineRect.right - 3, LeftOutLineRect.bottom - 4, 3, BLACK);
		DC.DrawLine(LeftOutLineRect.left + 1, LeftOutLineRect.top + 1, LeftOutLineRect.left + 1, LeftOutLineRect.bottom - 2, 3, WHITE);

		DC.DrawLine(LeftOutLineRect.left + 2, LeftOutLineRect.top + 2, LeftOutLineRect.right - 3, LeftOutLineRect.top + 2, 1, DARK_GRAY);
		DC.DrawLine(LeftOutLineRect.left + 2, LeftOutLineRect.top + 3, LeftOutLineRect.left + 2, LeftOutLineRect.bottom - 3, 1, DARK_GRAY);


		DC.WriteText(m_Labels[0], LeftOutLineRect, Font, BLACK, 1, CYAN);
		DC.WriteText(m_Labels[1], RightOutLineRect, Font, GRAY, 1, DARK_GRAY);
	}
	else
	{
		DC.DrawRectangle(RightOutLineRect, WHITE, CYAN);
		DC.DrawLine(RightOutLineRect.left + 1, RightOutLineRect.top + 1, RightOutLineRect.right - 2, RightOutLineRect.top + 1, 3, WHITE);
		DC.DrawLine(RightOutLineRect.right - 3, RightOutLineRect.top + 3, RightOutLineRect.right - 3, RightOutLineRect.bottom - 4, 3, BLACK);
		DC.DrawLine(RightOutLineRect.left + 1, RightOutLineRect.bottom - 4, RightOutLineRect.right - 3, RightOutLineRect.bottom - 4, 3, BLACK);
		DC.DrawLine(RightOutLineRect.left + 1, LeftOutLineRect.top + 1, RightOutLineRect.left + 1, RightOutLineRect.bottom - 2, 3, WHITE);

		DC.DrawLine(RightOutLineRect.left + 2, RightOutLineRect.top + 2, RightOutLineRect.right - 3, RightOutLineRect.top + 2, 1, DARK_GRAY);
		DC.DrawLine(RightOutLineRect.left + 2, RightOutLineRect.top + 3, RightOutLineRect.left + 2, RightOutLineRect.bottom - 3, 1, DARK_GRAY);


		DC.WriteText(m_Labels[0], LeftOutLineRect, Font, GRAY, 1, DARK_GRAY);
		DC.WriteText(m_Labels[1], RightOutLineRect, Font, BLACK, 1, CYAN);
	}

	PaintDC.Attach(DC.Detach());
}

//***************************************************************************************

LRESULT CSelBtn::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_HasMouse1 = FALSE;
	m_HasMouse2 = FALSE;

	Invalidate(FALSE);

	return 0;
}

//***************************************************************************************

void CSelBtn::Repaint()
{
	Invalidate(FALSE);
}

//***************************************************************************************

void CSelBtn::OnLButtonDown(uint nFlags, CPoint Point)
{
}

//***************************************************************************************

void CSelBtn::OnLButtonUp(uint nFlags, CPoint Point)
{
	if (MK_LBUTTON)
	{
		if (Point.x < m_Half_w)
		{
			m_OnOff = TRUE;
		}
		else
		{
			m_OnOff = FALSE;
		}

		Repaint();

		GetParent()->PostMessage(WM_SELBTN_POS_CHANGED, (WPARAM)this, m_id);
	}
}

//***************************************************************************************

void CSelBtn::OnMouseMove(uint nFlags, CPoint Point)
{
	if (Point.x < m_Half_w)
	{
		m_HasMouse1 = TRUE;
		m_HasMouse2 = FALSE;
	}
	else
	{
		m_HasMouse2 = TRUE;
		m_HasMouse1 = FALSE;
	}

	TRACKMOUSEEVENT Event = { sizeof(TRACKMOUSEEVENT), TME_LEAVE | TME_HOVER, m_hWnd, 500/*ms*/ };
	::TrackMouseEvent(&Event);

	Repaint();
}

//***************************************************************************************
