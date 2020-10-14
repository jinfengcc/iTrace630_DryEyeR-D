//A vertical slider control by cjf

//***************************************************************************************

#include "StdAfx.h"
#include "VerSliderCtrl.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CVerSliderCtrl, CWnd)

	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()

END_MESSAGE_MAP()

//***************************************************************************************

CVerSliderCtrl::CVerSliderCtrl()
{
	m_Pos = 1.0;
	m_scaleNum = 0;
	m_FirstPos = 0;
}

//***************************************************************************************

BOOL CVerSliderCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = ::AfxRegisterWndClass(0, ::LoadCursor(NULL, IDC_HAND), 0, 0);
	cs.style = cs.style | WS_CHILD;
	cs.dwExStyle = 0;
	return CWnd::PreCreateWindow(cs);
}

//***************************************************************************************

void CVerSliderCtrl::OnPaint()
{
	/*COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF black = m_Printing ? WHITE : BLACK;*/

	CPaintDC PaintDC(this);

	CMDC DC;
	DC.Attach(PaintDC.Detach());

	RECT Rect;
	GetClientRect(&Rect);

	DC.DrawRectangle(Rect, BLACK, BLACK);

	int w = Rect.right - Rect.left;
	int d = w / 2;

	int x0 = (Rect.left + Rect.right) / 2;
	int y1 = Rect.top + d;
	int y2 = Rect.bottom - d;

	int h = y2 - y1;

	DC.DrawLine(x0, y1, x0, y2, 2, WHITE);

	int y0 = y1 + intRound((1 - m_Pos) * h);

	if (y0 > y2) y0 = y2;

	DC.DrawCircle(x0, y0, intRound(0.3*d), 1, YELLOW, YELLOW);

	//Draw the scales 
	if (m_scaleNum > 1)
	{
		if (!(m_scaleNum == 2 && m_FirstPos == 1))
		{
			real PosSpace = m_FirstPos / (m_scaleNum - 1);
			for (int i = m_scaleNum; i >= 2; i--)
			{
				real pos = m_FirstPos - (m_scaleNum - i)*PosSpace;
				int yPos = y1 + intRound((1 - pos) * h);
				DC.DrawLine(x0 - 8, yPos, x0, yPos, 2, LIGHT_GRAY);
			}
		}
	}
	//Done

	PaintDC.Attach(DC.Detach());
}

//***************************************************************************************

void CVerSliderCtrl::OnLButtonDown(uint nFlags, CPoint Point)
{
	m_DownX = Point.x;
	m_DownY = Point.y;
	OnMouseMove(nFlags, Point);
}

//***************************************************************************************
int  CVerSliderCtrl::returnY()
{
	RECT Rect;
	GetClientRect(&Rect);

	int w = Rect.right - Rect.left;
	int d = w / 2;

	int x0 = (Rect.left + Rect.right) / 2;
	int y1 = Rect.top + d;
	int y2 = Rect.bottom - d;

	int h = y2 - y1;

	return intRound((1 - m_Pos)*h + Rect.top) + 10;
}

//***************************************************************************************
void CVerSliderCtrl::OnLButtonUp(uint nFlags, CPoint Point)
{
	real xPower = (real)Point.x - (real)m_DownX;
	xPower *= xPower;

	real yPower = (real)Point.y - (real)m_DownY;
	yPower *= yPower;
	real dis = sqrt(xPower + yPower);

	if (dis != 0) return;

	RECT Rect;
	GetClientRect(&Rect);

	int w = Rect.right - Rect.left;
	int d = w / 2;

	int x0 = (Rect.left + Rect.right) / 2;
	int y1 = Rect.top + d;
	int y2 = Rect.bottom - d;

	int h = y2 - y1;

	Point.y -= 10;

	if (Rect.top <= Point.y && Point.y <= Rect.bottom)
	{
		m_Pos = 1 - (real)(Point.y - Rect.top) / h;

		if (m_Pos < 0) m_Pos = 0;

		Invalidate(FALSE);

		GetParent()->PostMessage(WM_SLIDER_LCLICK, (WPARAM)this, 0);
	}
}

//***************************************************************************************

void CVerSliderCtrl::OnMouseMove(uint nFlags, CPoint Point)
{
	if (nFlags & MK_LBUTTON)
	{
		RECT Rect;
		GetClientRect(&Rect);

		int w = Rect.right - Rect.left;
		int d = w / 2;

		int x0 = (Rect.left + Rect.right) / 2;
		int y1 = Rect.top + d;
		int y2 = Rect.bottom - d;

		int h = y2 - y1;

		Point.y -= 10;

		if (Rect.top <= Point.y && Point.y <= Rect.bottom)
		{

			m_Pos = 1 - (real)(Point.y - Rect.top) / h;

			if (m_Pos < 0) m_Pos = 0;

			Invalidate(FALSE);

			GetParent()->PostMessage(WM_SLIDER_POS_CHANGED, (WPARAM)this, 0);
		}
	}
}

//***************************************************************************************
