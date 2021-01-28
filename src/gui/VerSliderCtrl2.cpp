//A vertical slider control by cjf

//***************************************************************************************

#include "StdAfx.h"

#include "VerSliderCtrl2.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CVerSliderCtrl2, CWnd)

	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()

END_MESSAGE_MAP()

//***************************************************************************************

CVerSliderCtrl2::CVerSliderCtrl2()
{
	m_Pos = 1.0;
	m_scaleNum = 0;
}

//***************************************************************************************

BOOL CVerSliderCtrl2::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = ::AfxRegisterWndClass(0, ::LoadCursor(NULL, IDC_HAND), 0, 0);
	cs.style = cs.style | WS_CHILD;
	cs.dwExStyle = 0;
	return CWnd::PreCreateWindow(cs);
}

//***************************************************************************************

void CVerSliderCtrl2::Refresh()
{
	::Sleep(50);
	Invalidate(FALSE);
}

//***************************************************************************************

//Presbia
void CVerSliderCtrl2::Clear()
{
	Invalidate(FALSE);
}

//***************************************************************************************

void CVerSliderCtrl2::OnPaint()
{
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

	int y0 = y1 + intRound((1 - m_Pos) * h);

	if (y0 > y2) y0 = y2;

	//Draw the scales
	for (int i = 0; i <= m_scaleNum; i++)
	{
		int  yPos = intRound(y1 + real_t(i) * real_t(y2 - y1) / m_scaleNum);

		if (i == m_scaleNum)  yPos = y2;
		DC.DrawLine(x0, yPos, x0 + 8, yPos, 2, RED);
	}
	//Done

	DC.DrawLine(x0, y1, x0, y2, 2, WHITE);

	DC.DrawCircle(x0, y0, intRound(0.3*d), 1, YELLOW, YELLOW);

	int xLeft = x0 - intRound(0.4*d);
	int xRight = x0 + intRound(0.4*d);
	int yTop = y0 - intRound(0.9*d);
	int yBtm = y0 + intRound(0.9*d);

	::SetRect(&m_PointReginon, xLeft, yTop, xRight, yBtm);

	PaintDC.Attach(DC.Detach());
}

//***************************************************************************************

void CVerSliderCtrl2::OnLButtonDown(uint nFlags, CPoint Point)
{
	//m_LBtnDownTime = (int)clock();
	m_DownX = Point.x;
	m_DownY = Point.y;
	OnMouseMove(nFlags, Point);
}

//***************************************************************************************
int  CVerSliderCtrl2::returnY()
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
void CVerSliderCtrl2::OnLButtonUp(uint nFlags, CPoint Point)
{
	real_t xPower = (real_t)Point.x - (real_t)m_DownX;
	xPower *= xPower;

	real_t yPower = (real_t)Point.y - (real_t)m_DownY;
	yPower *= yPower;
	real_t dis = sqrt(xPower + yPower);

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
		m_Pos = 1 - (real_t)(Point.y - Rect.top) / h;

		if (m_Pos < LowerstPos) m_Pos = LowerstPos;

		Invalidate(FALSE);

		GetParent()->PostMessage(WM_SLIDER_LCLICK, (WPARAM)this, 0);
	}
}

//***************************************************************************************

void CVerSliderCtrl2::OnMouseMove(uint nFlags, CPoint Point)
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

			m_Pos = 1 - (real_t)(Point.y - Rect.top) / h;

			if (m_Pos < LowerstPos) m_Pos = LowerstPos;

			Invalidate(FALSE);

			GetParent()->PostMessage(WM_SLIDER_POS_CHANGED, (WPARAM)this, 0);
		}
	}
}

//***************************************************************************************
