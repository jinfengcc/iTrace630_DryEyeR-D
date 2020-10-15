//***************************************************************************************

#include "StdAfx.h"
#include "SignalWnd.h"
#include "Resource.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CSignalWnd, CDispWnd)

	ON_BN_CLICKED(IDC_ZOOM_IN, OnZoomInButtonClicked)
	ON_BN_CLICKED(IDC_ZOOM_OUT, OnZoomOutButtonClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CSignalWnd::CSignalWnd(RECT& Rect, CWnd* pWnd) : CDispWnd(Rect, pWnd)
{
	m_Zoom = 2;
}

//***************************************************************************************

void CSignalWnd::CreateZoomButtons()
{
	RECT Rect;
	CMRgn Rgn;
	int w = intRound(0.058 * m_w); if (w < 25) w = 25;

	int W = ::GetSystemMetrics(SM_CXSCREEN);

	int l = 2;
	int b = m_h - 2;
	::SetRect(&Rect, l, b - w, l + w, b);
	m_ZoomOutButton.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_ZOOM_OUT);
	m_ZoomOutButton.SetImage(m_SizeSmall ?
		(W == 1920 ? IDR_ZOOM_OUT_3 : W == 1600 ? IDR_ZOOM_OUT_2 : IDR_ZOOM_OUT_1) :
		(W == 1920 ? IDR_ZOOM_OUT_6 : W == 1600 ? IDR_ZOOM_OUT_5 : IDR_ZOOM_OUT_4));
	m_ZoomOutButton.SetBk(FALSE);
	m_ZoomOutButton.SetBkColor(BLACK);
	m_ZoomOutButton.SetTip("Zoom out");
	Rgn.SetRectRgn(&Rect);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	l += w;
	::SetRect(&Rect, l, b - w, l + w, b);
	m_ZoomInButton.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_ZOOM_IN);
	m_ZoomInButton.SetImage(m_SizeSmall ?
		(W == 1920 ? IDR_ZOOM_IN_3 : W == 1600 ? IDR_ZOOM_IN_2 : IDR_ZOOM_IN_1) :
		(W == 1920 ? IDR_ZOOM_IN_6 : W == 1600 ? IDR_ZOOM_IN_5 : IDR_ZOOM_IN_4));
	m_ZoomInButton.SetBk(FALSE);
	m_ZoomInButton.SetBkColor(BLACK);
	m_ZoomInButton.SetTip("Zoom in");
	Rgn.SetRectRgn(&Rect);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	//521
	m_ZoomInButton.SetEnabled(TRUE);
	m_ZoomOutButton.SetEnabled(TRUE);
	if (m_Zoom == 8) m_ZoomInButton.SetEnabled(FALSE);
	if (m_Zoom == 2) m_ZoomOutButton.SetEnabled(FALSE);

	m_ZoomInButton.Repaint();
	m_ZoomOutButton.Repaint();
	//521

	m_ZoomOutButton.ShowWindow(SW_SHOW);
	m_ZoomInButton.ShowWindow(SW_SHOW);
}

//***************************************************************************************

void CSignalWnd::OnZoomInButtonClicked()
{
	if (m_Zoom == 8) return;
	m_Zoom++;

	//521
	m_ZoomInButton.SetEnabled(TRUE);
	m_ZoomOutButton.SetEnabled(TRUE);
	if (m_Zoom == 8) m_ZoomInButton.SetEnabled(FALSE);
	if (m_Zoom == 2) m_ZoomOutButton.SetEnabled(FALSE);

	m_ZoomInButton.Repaint();
	m_ZoomOutButton.Repaint();
	//521

	Repaint();
}

void CSignalWnd::OnZoomOutButtonClicked()
{
	if (m_Zoom == 2) return;
	m_Zoom--;

	//521
	m_ZoomInButton.SetEnabled(TRUE);
	m_ZoomOutButton.SetEnabled(TRUE);
	if (m_Zoom == 8) m_ZoomInButton.SetEnabled(FALSE);
	if (m_Zoom == 2) m_ZoomOutButton.SetEnabled(FALSE);

	m_ZoomInButton.Repaint();
	m_ZoomOutButton.Repaint();
	//521

	Repaint();
}

//***************************************************************************************

void CSignalWnd::RepaintMemDC()
{
	ClearMemDC();
	Signal();
	Labels();
	Border();
}

//***************************************************************************************

void CSignalWnd::Signal()
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;
	COLORREF yellow = m_Printing ? ORANGE : YELLOW;

	if (m_pWFExam->m_DetectorType != DT_LINEAR_ARRAYS) return;

	int p = m_pWFExam->m_CurPoint;
	if (p < 0 || p >= m_pWFExam->m_Points.GetSize()) return;

	CScanPoint* pPoint = m_pWFExam->m_Points.Get(p);

	CLAD* pData = m_Horz ? &pPoint->m_LADX : &pPoint->m_LADY;

	int FontSize = intRound(0.045 * m_h);
	CMFont Font(FontSize, 400, "Arial");

	CString s;
	RECT Rect;

	real_t L = 0.10 * m_w;
	real_t R = 0.90 * m_w;
	real_t T = 0.10 * m_h;
	real_t B = 0.90 * m_h;

	// horizontal lines
	int max = 1 << (16 - m_Zoom);
	int d = max >> 4;
	for (int j = 0; j <= max; j += d) {
		real_t y = B - (j * (B - T) / max);
		m_MemDC.DrawLine(intRound(L), intRound(y), intRound(R + 1), intRound(y), 1, gray);
		::SetRect(&Rect, 0, intRound(y - 100.0), intRound(L), intRound(y + 100.0));
		s.Format(_T("%i "), j);
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	// vertical lines
	for (int i = 0; i <= 512; i += 32)
	{
		real_t x = L + (i * (R - L) / 512.0);
		m_MemDC.DrawLine(intRound(x), intRound(T), intRound(x), intRound(B + 1), 1, gray);
	}

	// signal
	CPen Pen(PS_SOLID, 2, yellow);
	CPen* pPen = m_MemDC.SelectObject(&Pen);
	for (int i = 0; i <= 511; i++)
	{
		int x = intRound(L + (i * (R - L) / 512.0));
		int y = intRound(B - (pData->m_Signal[i] * (B - T) / max));
		if (i == 0)
		{
			m_MemDC.MoveTo(x, y);
		}
		else 
		{
			m_MemDC.LineTo(x, y);
		}
	}
	m_MemDC.SelectObject(pPen);

	// max signal
	real_t y = B - (pData->m_Max * (B - T) / max);
	m_MemDC.DrawHorzDottedLine(intRound(L), intRound(R + 1.0), intRound(y), BLUE, NOCOLOR);
	::SetRect(&Rect, intRound(R), intRound(y - 100.0), intRound(R + 1000.0), intRound(y + 100.0));
	s.Format(_T(" %i"), pData->m_Max);
	m_MemDC.WriteText(s, Rect, Font, BLUE, 0);

	// centroid
	if (!pData->m_Bad) {
		real_t x = L + (pData->m_CentroidPos * (R - L) / 512.0);
		m_MemDC.DrawLine(intRound(x), intRound(T), intRound(x), intRound(B + 1), 1, RED);
		::SetRect(&Rect, intRound(x - 1000.0), intRound(B + 3.0), intRound(x + 1000.0), intRound(B + 3.0 + FontSize));
		s.Format(_T("%i"), intRound(pData->m_CentroidPos));
		m_MemDC.WriteText(s, Rect, Font, RED, 1);
	}
}

//***************************************************************************************
