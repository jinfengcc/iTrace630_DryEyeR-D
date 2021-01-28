//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "ChartWnd.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

CChartWnd::CChartWnd(RECT& Rect, CWnd* pWnd) : CDispWnd(Rect, pWnd)
{
}

//***************************************************************************************

void CChartWnd::RepaintMemDC()
{
	ClearMemDC();
	Chart();
	Labels();
	Border();
}

//***************************************************************************************

void CChartWnd::Chart()
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	int FontSize1 = intRound(0.050 * m_h);
	CMFont Font1(FontSize1, 400, "Arial");

	int FontSize2 = intRound(0.045 * m_h);
	CMFont Font2(FontSize2, 400, "Arial");

	CString s;
	RECT Rect;

	// top labels
	int n = m_MainLabels.GetSize();

	if (n > 0) 
	{
		int L = m_l + intRound(0.012 * m_w);
		int T = intRound(0.012 * m_h);
		int l = L;
		int h = intRound(0.900 * FontSize1);

		for (int i = 0; i < n; i++)
		{
			::SetRect(&Rect, l, T, m_w, T + h);
			COLORREF color = m_Colors[i % 6];
			m_MemDC.WriteText(m_MainLabels[i], Rect, Font1, color, 0);
			m_MemDC.MeasureRect(m_MainLabels[i], Rect, Font1);
			l += intRound(1.1 * (Rect.right - Rect.left));
		}
	}

	int nx = m_Values.GetNumRows();
	int ny = m_max - m_min + 1;

	if (ny == 1)
	{
		m_max = m_min + 1; 
		ny = 2; 
	}

	real_t L = 0.100 * m_w;
	real_t T = 0.180 * m_h;
	real_t R = 0.900 * m_w;
	real_t B = 0.860 * m_h;
	real_t dw_px = (R - L) / (nx - 1.0);
	real_t dh_px = (B - T) / (ny - 1.0);

	// vertical lines
	int nt = m_TLabels.GetSize();
	int nb = m_BLabels.GetSize();
	for (int i = 0; i < nx; i++)
	{
		real_t x = L + i * dw_px;
		m_MemDC.DrawLine(intRound(x), intRound(T), intRound(x), intRound(B + 1), 1, gray);
		if (i < nt)
		{
			::SetRect(&Rect, intRound(x - 1000.0), intRound(T - 3.0 - FontSize2), intRound(x + 1000.0), intRound(T - 3.0));
			m_MemDC.WriteText(m_TLabels[i], Rect, Font2, white, 1);
		}
		if (i < nb) 
		{
			::SetRect(&Rect, intRound(x - 1000.0), intRound(B + 3.0), intRound(x + 1000.0), intRound(B + 3.0 + FontSize2));
			m_MemDC.WriteText(m_BLabels[i], Rect, Font2, white, 1);
		}
	}

	// horizontal lines
	int nl = m_LLabels.GetSize();
	for (int i = m_min; i <= m_max; i++)
	{
		real_t y = B - (i - m_min) * dh_px;
		m_MemDC.DrawLine(intRound(L), intRound(y), intRound(R + 1.0), intRound(y), 1, i == 0 ? white : gray);
		if (i < nl) 
		{
			::SetRect(&Rect, 0, intRound(y - 100.0), intRound(L), intRound(y + 100.0));
			s = m_LLabels[i - m_min] + " ";
			m_MemDC.WriteText(s, Rect, Font2, white, 2);
		}
	}

	// lines
	for (int col = 0; col < m_Values.GetNumCols(); col++)
	{
		COLORREF color = m_Colors[col % 6];
		int xp, yp;
		for (int row = 0; row < m_Values.GetNumRows(); row++)
		{
			int x = intRound(L + row * dw_px);
			int y = intRound(B - (m_Values(row, col) - m_min) * dh_px);
			if (row > 0) m_MemDC.DrawLine(xp, yp, x, y, 3, color);
			xp = x;
			yp = y;
		}
	}

	// Y unit label
	real_t l = 0.02 * m_w;
	::SetRect(&Rect, intRound(l), intRound(T), intRound(l + FontSize2), intRound(B));
	m_MemDC.WriteRotatedText(m_YUnitLabel, intRound(0.4 * L), intRound(0.5 * (T + B)), 90, FontSize2, 400, "Arial", white, NOCOLOR);

	// X unit label
	::SetRect(&Rect, 0, intRound(B + 3.0 + FontSize2), m_w, intRound(B + 3.0 + 2.0 * FontSize2));
	m_MemDC.WriteText(m_XUnitLabel, Rect, Font2, white, 1);
}

//***************************************************************************************
