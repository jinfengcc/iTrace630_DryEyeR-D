//***************************************************************************************

#include "StdAfx.h"

#include "Rad.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CRad, CWnd)

	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)

END_MESSAGE_MAP()

//***************************************************************************************

CRad::CRad()
{
	m_Enabled = TRUE;
	m_Pushed = FALSE;
	m_HasMouse = FALSE;
	m_Highlighted = FALSE;
	m_Checked = FALSE;

	m_BkColor = ::GetSysColor(COLOR_BTNFACE);

	m_Text = "";
	m_TextColor = WHITE;

	m_FontSize = 20;
	m_FontWidth = 400;
	m_FontFace = "Arial";
}

//***************************************************************************************

CRad::~CRad()
{
}

//***************************************************************************************

BOOL CRad::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = ::AfxRegisterWndClass(CS_DBLCLKS, ::LoadCursor(NULL, IDC_HAND), NULL, NULL);
	cs.style = cs.style | WS_CHILD;
	cs.dwExStyle = 0;

	m_MemDC.CreateTrueColorDC(cs.cx, cs.cy);

	return CWnd::PreCreateWindow(cs);
}

//***************************************************************************************

void CRad::OnShowWindow(BOOL bShow, uint nStatus)
{
	if (bShow) Repaint();

	CWnd::OnShowWindow(bShow, nStatus);
}

//***************************************************************************************

void CRad::OnPaint()
{
	CPaintDC PaintDC(this);

	RECT Rect;
	GetClientRect(&Rect);

	PaintDC.BitBlt(0, 0, Rect.right, Rect.bottom, &m_MemDC, 0, 0, SRCCOPY);
}

//***************************************************************************************

void CRad::OnLButtonDown(uint nFlags, CPoint Point)
{
	if (m_Enabled)
	{
		m_Pushed = TRUE;
	}
}

//***************************************************************************************

void CRad::OnLButtonUp(uint nFlags, CPoint Point)
{
	if (m_Pushed)
	{
		ulong wParam = ((ulong)BN_CLICKED << 16) | (ulong)GetDlgCtrlID();
		ulong lParam = (ulong)m_hWnd;
		GetParent()->PostMessage(WM_COMMAND, (WPARAM)wParam, (LPARAM)lParam);
	}
}

//***************************************************************************************

void CRad::OnMouseMove(uint nFlags, CPoint Point)
{
	if (!m_HasMouse)
	{
		m_HasMouse = TRUE;

		TRACKMOUSEEVENT Event = { sizeof(TRACKMOUSEEVENT), TME_LEAVE | TME_HOVER, m_hWnd, 500/*ms*/ };
		::TrackMouseEvent(&Event);

		Repaint();
	}
}

//***************************************************************************************

LRESULT CRad::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_HasMouse = FALSE;

	Repaint();

	return 0;
}

//***************************************************************************************

LRESULT CRad::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

//***************************************************************************************

void CRad::Repaint()
{
	RECT Rect;
	GetClientRect(&Rect);

	if (m_BkColor != NOCOLOR) m_MemDC.FillSolidRect(&Rect, m_BkColor);

	int r = intRound(real_t(Rect.bottom - Rect.top) / 2);
	int y = Rect.top + r;
	r = intRound(real_t(r) / 2);
	int x = Rect.left + r;
	int r2 = (r - 3 < 1 ? 1 : r - 3);

	if (m_HasMouse) m_MemDC.DrawCircle(x, y, r, 2, m_BkColor, CYAN);
	else m_MemDC.DrawCircle(x, y, r, 2, m_BkColor, WHITE);

	if (m_Checked)
	{
		m_MemDC.DrawCircle(x, y, r2, 2, LIGHT_GRAY, CYAN);
		m_MemDC.DrawCircle(x, y, r2 - 2, 2, BLUE, CYAN);
	}
	else
	{
		m_MemDC.DrawCircle(x, y, r2, 2, LIGHT_GRAY, WHITE);
	}

	if (!m_Text.IsEmpty())
	{
		COLORREF TextColor = m_Enabled ? m_TextColor : 0x00a0a0a0;
		CStringA AFontFace(m_FontFace);
		CMFont Font(m_FontSize, m_FontWidth, AFontFace);

		RECT Rect1;
		::SetRect(&Rect1, Rect.left + 2 * r + 10, Rect.top, Rect.right, Rect.bottom);


		if (m_Checked) m_MemDC.WriteText(m_Text, Rect1, Font, RED, 0);
		else
		{
			if (m_HasMouse) m_MemDC.WriteText(m_Text, Rect1, Font, CYAN, 0);
			else           m_MemDC.WriteText(m_Text, Rect1, Font, TextColor, 0);
		}
	}

	Invalidate(FALSE);
}

//***************************************************************************************
