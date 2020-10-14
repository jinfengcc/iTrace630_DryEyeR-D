//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "EEFWnd.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

CEEFWnd::CEEFWnd(RECT& Rect, CWnd* pWnd) : CDispWnd(Rect, pWnd)
{
}

//***************************************************************************************

void CEEFWnd::RepaintMemDC()
{
	ClearMemDC();
	Eef();
	Labels();
	Border();
}

//***************************************************************************************

void CEEFWnd::Eef()
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	real L = 0.100 * m_w;
	real T = 0.180 * m_h;
	real R = 0.900 * m_w;
	real B = 0.860 * m_h;
	real w_px = R - L;
	real h_px = B - T;
	int w_mn = 50;
	int g_mn = 10;
	real px_mn = w_px / w_mn;

	CString s;
	RECT Rect;

	int FontSize = intRound(0.045 * m_h);
	CMFont Font(FontSize, 400, "Arial");

	// вертикальные линии
	for (int i_mn = 0; i_mn <= w_mn; i_mn += g_mn) {
		real x = L + px_mn * i_mn;
		m_MemDC.DrawLine(intRound(x), intRound(T), intRound(x), intRound(B + 1), 1, gray);
		s.Format(_T("%i"), i_mn);
		SetRect(&Rect, intRound(x - 1000.0), intRound(B + 3.0), intRound(x + 1000.0), intRound(B + 3.0 + FontSize));
		m_MemDC.WriteText(s, Rect, Font, white, 1);
	}
	// units
	::SetRect(&Rect, 0, intRound(B + 3.0 + FontSize), m_w, intRound(B + 3.0 + 2.0 * FontSize));
	m_MemDC.WriteText("arcmin", Rect, Font, white, 1);

	// горизонтальные линии
	for (int i_prc = 0; i_prc <= 100; i_prc += 10) {
		real y = B - 0.01 * i_prc * h_px;
		m_MemDC.DrawLine(intRound(L), intRound(y), intRound(R + 1.0), intRound(y), 1, gray);
		s.Format(_T("%i "), i_prc);
		SetRect(&Rect, intRound(L - 1000.0), intRound(y - 100.0), intRound(L), intRound(y + 100.0));
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	Matrix<real> x(w_mn + 1);
	Matrix<real> y(w_mn + 1);
	Matrix<real> p(w_mn + 1);
	for (int i_mn = 0; i_mn <= w_mn; i_mn++) {
		p[i_mn] = m_Psf.m_EncircledEnergy[i_mn];
		x[i_mn] = L + i_mn * px_mn;
		y[i_mn] = B - p[i_mn] * 0.01 * h_px;
	}
	for (int i_mn = 1; i_mn <= w_mn; i_mn++) {
		// участок кривой
		m_MemDC.DrawLine(intRound(x[i_mn - 1]), intRound(y[i_mn - 1]), intRound(x[i_mn]), intRound(y[i_mn]), 3, m_Printing ? ORANGE : YELLOW);
		// линии идущие от точек пересечения кривой с 10, 20, 30 и т.д. минутами
		if (i_mn % 10 == 0) {
			if (p[i_mn] <= 99.0) {
				COLORREF color = m_Printing ? 0x0000afef : 0x005f9fbf;
				m_MemDC.DrawLine(intRound(x[i_mn]), intRound(y[i_mn]), intRound(R + 1.0), intRound(y[i_mn]), 1, color);
				m_MemDC.DrawLine(intRound(x[i_mn]), intRound(y[i_mn]), intRound(x[i_mn]), intRound(B + 1.0), 1, color);
				if (p[i_mn] - p[i_mn - 10] > 5.0) {
					s.Format(_T(" %i"), intRound(p[i_mn]));
					SetRect(&Rect, intRound(R), intRound(y[i_mn] - 100.0), intRound(R + 1000.0), intRound(y[i_mn] + 100.0));
					m_MemDC.WriteText(s, Rect, Font, color, 0);
				}
			}
		}
	}
	// линии идущие от точки пересечения кривой с уровнем 50%
	{
		int i_mn = m_Psf.m_R50mn;
		// вертикальная
		m_MemDC.DrawLine(intRound(x[i_mn]), intRound(T), intRound(x[i_mn]), intRound(y[i_mn] - 3.0), 1, RED);
		// горизонтальная
		real y = B - 0.5 * h_px;
		m_MemDC.DrawLine(intRound(L), intRound(y), intRound(x[i_mn] - 3.0), intRound(y), 1, RED);
		s.Format(_T(" %i"), i_mn);
		::SetRect(&Rect, intRound(x[i_mn] - 1000.0), intRound(T - 3.0 - FontSize), intRound(x[i_mn] + 1000.0), intRound(T - 3));
		m_MemDC.WriteText(s, Rect, Font, RED, 1);
	}

}

//***************************************************************************************
