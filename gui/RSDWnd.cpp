//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "RSDWnd.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CRSDWnd, CDispWnd)

	ON_BN_CLICKED(IDC_ZOOM_IN, OnZoomInButtonClicked)
	ON_BN_CLICKED(IDC_ZOOM_OUT, OnZoomOutButtonClicked)
	ON_MESSAGE(WM_CHANGE_POS, OnChangePosMsg)

END_MESSAGE_MAP()

//***************************************************************************************

CRSDWnd::CRSDWnd(RECT& Rect, CWnd* pWnd) : CDispWnd(Rect, pWnd)
{
	m_AddedSphEq = 0.0;
	m_Zoom = 400;
}

//***************************************************************************************

void CRSDWnd::CreateZoomButtons()
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
	if (m_Zoom == 400) m_ZoomInButton.SetEnabled(FALSE);
	if (m_Zoom == 100) m_ZoomOutButton.SetEnabled(FALSE);

	m_ZoomInButton.Repaint();
	m_ZoomOutButton.Repaint();
	//521

	m_ZoomOutButton.ShowWindow(SW_SHOW);
	m_ZoomInButton.ShowWindow(SW_SHOW);
}

//***************************************************************************************

void CRSDWnd::OnZoomInButtonClicked()
{
	switch (m_Zoom) {
	case  50: m_Zoom = 100; break;
	case 100: m_Zoom = 200; break;
	case 200: m_Zoom = 400; break;
	default: return;
	}

	//521
	m_ZoomInButton.SetEnabled(TRUE);
	m_ZoomOutButton.SetEnabled(TRUE);
	if (m_Zoom == 400) m_ZoomInButton.SetEnabled(FALSE);
	if (m_Zoom == 100) m_ZoomOutButton.SetEnabled(FALSE);

	m_ZoomInButton.Repaint();
	m_ZoomOutButton.Repaint();
	//521

	Repaint();
}

//***************************************************************************************

void CRSDWnd::OnZoomOutButtonClicked()
{
	switch (m_Zoom) {
	case 400: m_Zoom = 200; break;
	case 200: m_Zoom = 100; break;
	case 100: m_Zoom = 50; break;
	default: return;
	}

	//521
	m_ZoomInButton.SetEnabled(TRUE);
	m_ZoomOutButton.SetEnabled(TRUE);
	if (m_Zoom == 400) m_ZoomInButton.SetEnabled(FALSE);
	if (m_Zoom == 100) m_ZoomOutButton.SetEnabled(FALSE);

	m_ZoomInButton.Repaint();
	m_ZoomOutButton.Repaint();
	//521

	Repaint();
}

//***************************************************************************************

void CRSDWnd::RepaintMemDC()
{
	ClearMemDC();
	Grid();
	Points();
	Labels();
	Border();

	BackupMemDC();

	Cursor();
}

//***************************************************************************************

void CRSDWnd::Grid()
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	real_t px_mn = ((0.8 * m_h)/*pix*/ / 240.0/*min*/) * (m_Zoom * 0.01);
	int g_mn = 10;
	real_t g_px = g_mn * px_mn;

	int m;
	switch (m_Zoom) {
	case  50: m = 24; break;
	case 100: m = 12; break;
	case 200: m = 6; break;
	case 400: m = 3; break;
	default: return;
	}

	real_t cx = 0.5 * m_w;
	real_t cy = 0.5 * m_h;

	// vertical lines
	int y1 = intRound(cy - g_px * m);
	int y2 = intRound(cy + g_px * m);
	for (int i = -m; i <= m; i++) {
		int x = intRound(cx + g_px * i);
		m_MemDC.DrawLine(x, y1, x, y2 + 1, 1, gray);
	}

	// horizontal lines
	int x1 = intRound(cx - g_px * m);
	int x2 = intRound(cx + g_px * m);
	for (int i = -m; i <= m; i++) {
		int y = intRound(cy + g_px * i);
		m_MemDC.DrawLine(x1, y, x2 + 1, y, 1, gray);
	}

	//---------------------------------------------------

	int FontSize = intRound(0.045 * m_h);
	CMFont Font(FontSize, 400, "Arial");

	CString s;
	RECT Rect;

	s.Format(_T("Spacing:  %i arcmin"), g_mn);
	::SetRect(&Rect, 0, m_h - 3, m_w, m_h - 3 - FontSize);
	m_MemDC.WriteText(s, Rect, Font, white, 1);
}

//***************************************************************************************

void CRSDWnd::Points()
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF black = m_Printing ? WHITE : BLACK;

	real_t cx = 0.5 * m_w;
	real_t cy = 0.5 * m_h;

	real_t px_mn = ((0.8 * m_h) / 240.0) * (m_Zoom * 0.01);

	real_t f = CWFExam::m_f_eye_um * CPSF::m_mn_um;

	COLORREF colors[7] = { MAGENTA, RED, ORANGE, YELLOW, GREEN, CYAN, BLUE };
	CPen Pen[7], *pPen;
	for (int i = 0; i < 7; i++) {
		Pen[i].CreatePen(PS_SOLID, 1, colors[i]);
	}

	int circle = -1;
	real_t r_um_prev = -1.0;
	real_t a_rd_prev;

	int CurShot = m_pWFExam->m_CurShot;
	int CurPoint = m_pWFExam->m_CurPoint;

	CScanPoint* pPoint = m_pWFExam->m_Points.MoveFirst();

	while (pPoint)
	{
		if (pPoint->m_shot == CurShot)
		{
			real_t r_um = pPoint->m_r_um - 0.00001;
			real_t a_rd = pPoint->m_a_rd;

			if (r_um <= m_Surface.m_r_max_um)
			{
				if (fabs(r_um - r_um_prev) > 0.001)
				{
					circle++;
				}

				real_t dw_dx, dw_dy;
				m_Surface.GetAt(r_um, a_rd, NULL, &dw_dx, &dw_dy, NULL, NULL, NULL);
				int x = intRound(cx + (-f * dw_dx) * px_mn);
				int y = intRound(cy - (-f * dw_dy) * px_mn);

				if (r_um != r_um_prev)
				{
					m_MemDC.MoveTo(x, y);
				}
				else
				{
					pPen = m_MemDC.SelectObject(&Pen[circle % 7]);
					real_t da_rd = (a_rd - a_rd_prev) / 20.0;

					for (int l = 0; l <= 20; l++)
					{
						m_Surface.GetAt(r_um, a_rd_prev + l * da_rd, NULL, &dw_dx, &dw_dy, NULL, NULL, NULL);
						x = intRound(cx + (-f * dw_dx) * px_mn);
						y = intRound(cy - (-f * dw_dy) * px_mn);
						m_MemDC.LineTo(x, y);
					}

					m_MemDC.SelectObject(pPen);
				}

				COLORREF color = pPoint->m_Bad ? white : colors[circle % 7];
				m_MemDC.DrawCircle(x, y, 3, 1, color, color);

				r_um_prev = r_um;
				a_rd_prev = a_rd;
			}			
		}

		pPoint = m_pWFExam->m_Points.MoveNext();
	}
	

	if (pPoint = m_pWFExam->m_Points.Get(CurPoint))
	{
		real_t dw_dx, dw_dy;
		m_Surface.GetAt(pPoint->m_r_um, pPoint->m_a_rd, NULL, &dw_dx, &dw_dy, NULL, NULL, NULL);
		int x = intRound(cx + (-f * dw_dx) * px_mn);
		int y = intRound(cy - (-f * dw_dy) * px_mn);
		m_MemDC.DrawCircle(x, y, 3, 1, white, NOCOLOR);
		m_MemDC.DrawCircle(x, y, 4, 1, black, NOCOLOR);
		m_MemDC.DrawCircle(x, y, 5, 1, white, NOCOLOR);
	}

	if (fabs(m_AddedSphEq) < 0.000001)
	{
		m_RTLabel[0].Empty();
	}
	else
	{
		m_RTLabel[0].Format(_T("Added defocus = %.1f dpt "), m_AddedSphEq);
	}
}

//***************************************************************************************

void CRSDWnd::Cursor()
{
	if (m_cursor_r_um > m_Surface.m_r_max_um) return;

	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF black = m_Printing ? WHITE : BLACK;

	real_t cx = 0.5 * m_w;
	real_t cy = 0.5 * m_h;

	real_t dw_dx, dw_dy;
	m_Surface.GetAt(m_cursor_r_um, m_cursor_a_rd, NULL, &dw_dx, &dw_dy, NULL, NULL, NULL);

	real_t px_mn = ((0.8 * m_h) / 240.0) * (m_Zoom * 0.01);

	real_t f = CWFExam::m_f_eye_um * CPSF::m_mn_um;

	int x = intRound(cx + (-f * dw_dx) * px_mn);
	int y = intRound(cy - (-f * dw_dy) * px_mn);

	m_MemDC.DrawLine(x - 5, y - 1, x + 6, y - 1, 1, white);
	m_MemDC.DrawLine(x - 5, y, x + 6, y, 1, white);
	m_MemDC.DrawLine(x - 5, y + 1, x + 6, y + 1, 1, white);
	m_MemDC.DrawLine(x - 1, y - 5, x - 1, y + 6, 1, white);
	m_MemDC.DrawLine(x, y - 5, x, y + 6, 1, white);
	m_MemDC.DrawLine(x + 1, y - 5, x + 1, y + 6, 1, white);
	m_MemDC.DrawLine(x - 4, y, x + 5, y, 1, black);
	m_MemDC.DrawLine(x, y - 4, x, y + 5, 1, black);
}

//***************************************************************************************

LRESULT CRSDWnd::OnChangePosMsg(WPARAM wParam, LPARAM lParam)
{
	RestoreMemDC();

	m_cursor_r_um = ((real_t*)lParam)[0];
	m_cursor_a_rd = ((real_t*)lParam)[1];
	Cursor();

	InvalidateRgn(&m_Rgn, FALSE);
	UpdateWindow();

	return 0;
}

//***************************************************************************************
