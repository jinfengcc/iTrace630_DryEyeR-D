//***************************************************************************************

#include "StdAfx.h"

#include "XPSliderCtrl.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CXPSliderCtrl, CWnd)

	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()

END_MESSAGE_MAP()

//***************************************************************************************

CXPSliderCtrl::CXPSliderCtrl()
{
	m_Pos = 0.0;
}

//***************************************************************************************

BOOL CXPSliderCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = ::AfxRegisterWndClass(0, ::LoadCursor(NULL, IDC_HAND), 0, 0);
	cs.style = cs.style | WS_CHILD;
	cs.dwExStyle = 0;
	return CWnd::PreCreateWindow(cs);
}

//***************************************************************************************

void CXPSliderCtrl::OnPaint()
{
	CPaintDC PaintDC(this);

	CMDC DC;
	DC.Attach(PaintDC.Detach());

	RECT Rect;
	GetClientRect(&Rect);

	DC.DrawRectangle(Rect, GRAY, DARK_GRAY);

	int w = Rect.right - Rect.left + 1;
	int h = Rect.bottom - Rect.top + 1;
	int d = h / 2;

	int y0 = (Rect.top + Rect.bottom) / 2;
	int x1 = Rect.left + d;
	int x2 = Rect.right - d;

	DC.DrawLine(x1, y0, x2 + 1, y0, 1, WHITE);

	int x0 = x1 + intRound(m_Pos * (w - d - d));

	DC.DrawCircle(x0, y0, intRound(0.7 * d), 1, WHITE, LIGHT_GRAY);

	PaintDC.Attach(DC.Detach());
}


//***************************************************************************************

void CXPSliderCtrl::RePaint()
{
	CPaintDC PaintDC(this);

	CMDC DC;
	DC.Attach(PaintDC.Detach());

	RECT Rect;
	GetClientRect(&Rect);

	DC.DrawRectangle(Rect, GRAY, DARK_GRAY);

	int w = Rect.right - Rect.left + 1;
	int h = Rect.bottom - Rect.top + 1;
	int d = h / 2;

	int y0 = (Rect.top + Rect.bottom) / 2;
	int x1 = Rect.left + d;
	int x2 = Rect.right - d;

	DC.DrawLine(x1, y0, x2 + 1, y0, 1, WHITE);

	int x0 = x1 + intRound(m_Pos * (w - d - d));

	DC.DrawCircle(x0, y0, intRound(0.7 * d), 1, WHITE, LIGHT_GRAY);

	PaintDC.Attach(DC.Detach());

	Invalidate(FALSE);
}

//***************************************************************************************

void CXPSliderCtrl::OnLButtonDown(uint nFlags, CPoint Point)
{
	OnMouseMove(nFlags, Point);
}

//***************************************************************************************

void CXPSliderCtrl::OnMouseMove(uint nFlags, CPoint Point)
{
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
			m_Pos = (real_t)(Point.x - x1) / (w - d - d);

			Invalidate(FALSE);

			GetParent()->PostMessage(WM_SLIDER_POS_CHANGED, (WPARAM)this, 0);
		}
	}
}

//***************************************************************************************
