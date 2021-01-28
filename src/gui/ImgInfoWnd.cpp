//***************************************************************************************

#include "StdAfx.h"
#include "ImgInfoWnd.h"
#include "Image.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BOOL CImgInfoWnd::m_Printing = FALSE;

//***************************************************************************************

BEGIN_MESSAGE_MAP(CImgInfoWnd, CWnd)

	ON_WM_PAINT()

END_MESSAGE_MAP()

//***************************************************************************************

CImgInfoWnd::CImgInfoWnd()
{

}

//***************************************************************************************

CImgInfoWnd::~CImgInfoWnd()
{
}

//***************************************************************************************

void CImgInfoWnd::CreateWnd(RECT& Rect, CWnd* pParentWnd)
{
	CWnd::CreateEx(0, ::WndClassName, _T(""), WS_CHILD, Rect, pParentWnd, 0);

	m_w = Rect.right - Rect.left;
	m_h = Rect.bottom - Rect.top;

	//Border();

	m_MemDC.CreateTrueColorDC(m_w, m_h);

	m_Rgn.SetRectRgn(0, 0, m_w, m_h);

	ShowWindow(SW_SHOW);
}

//***************************************************************************************

void CImgInfoWnd::OnPaint()
{
	CPaintDC PaintDC(this);
	PaintDC.BitBlt(0, 0, m_w, m_h, &m_MemDC, 0, 0, SRCCOPY);
}

//***************************************************************************************

void CImgInfoWnd::ClearMemDC()
{
	m_MemDC.FillSolidRect(0, 0, m_w, m_h, m_Printing ? WHITE : WHITE);
}

//***************************************************************************************

void CImgInfoWnd::Border()
{
	RECT Rect = { 0, 0, m_w, m_h };
	m_MemDC.DrawRectangle(Rect, m_Printing ? LIGHT_GRAY : DARK_GRAY/*(m_HasFocus ? WHITE : DARK_GRAY)*/, NOCOLOR);
}

//***************************************************************************************

void CImgInfoWnd::Repaint()
{
	RepaintMemDC();

	InvalidateRgn(&m_Rgn, FALSE);

	UpdateWindow();
}



