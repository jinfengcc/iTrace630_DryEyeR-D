//***************************************************************************************

#include "StdAfx.h"

#include "TipWnd.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CTipWnd, CWnd)

	ON_WM_CREATE()
	ON_WM_PAINT()

END_MESSAGE_MAP()

//***************************************************************************************

CTipWnd::CTipWnd(RECT& Rect, CWnd* pWnd, const CString& Tip)
{
	m_Tip = " " + Tip + " ";
	CWnd::CreateEx(WS_EX_TOPMOST, ::WndClassName, _T(""), WS_POPUP, Rect, pWnd, 0);
}

//***************************************************************************************

int CTipWnd::OnCreate(CREATESTRUCT* cs)
{
	if (CWnd::OnCreate(cs) == -1) return -1;

	int W = ::GetSystemMetrics(SM_CXSCREEN);
	int H = ::GetSystemMetrics(SM_CYSCREEN);

	CMFont Font(intRound(0.02 * H), 400, "Arial");

	CMDC DC;
	DC.CreateCompatibleDC(NULL);

	RECT Rect;
	DC.MeasureRect(m_Tip, Rect, Font);

	int w = Rect.right - Rect.left + 2;
	int h = Rect.bottom - Rect.top + 2;

	int l = cs->x + (cs->cx >> 1) - (w >> 1);

	if (l < 0) l = 0;
	else if (l + w > W) l = W - w;

	int t = cs->y - 2 - h;

	if (t < 0) t = 0;

	SetWindowPos(NULL, l, t, w, h, 0);

	ShowWindow(SW_SHOW);

	return 0;
}

//***************************************************************************************

void CTipWnd::OnPaint()
{
	CPaintDC PaintDC(this);

	CMDC DC;
	DC.Attach(PaintDC.Detach());

	CMFont Font(intRound(0.02 * ::GetSystemMetrics(SM_CYSCREEN)), 400, "Arial");

	RECT Rect;
	GetClientRect(&Rect);

	DC.DrawRectangle(Rect, 0x004fefff, 0x004fefff);
	DC.WriteText(m_Tip, Rect, Font, BLACK, 1);

	PaintDC.Attach(DC.Detach());
}

//***************************************************************************************

