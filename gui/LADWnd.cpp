//***************************************************************************************

#include "StdAfx.h"
#include "LADWnd.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

CLADWnd::CLADWnd()
{
	m_Zoom = 2;
}

//***************************************************************************************

void CLADWnd::RepaintMemDC()
{
	ClearMemDC();

	CString s;
	RECT Rect;
	CMFont Font12(12, 400, "Arial");
	CMFont Font16(16, 600, "Arial");

	int l = 40, b = m_h - 12;

	// Horizontal lines
	Rect.left = 0;
	Rect.right = l - 5;
	for (int i = 0; i <= 16; i++) {
		int y = i << 4; /* 0 <= y <= 256 */
		m_MemDC.DrawLine(l, b - y, l + 513, b - y, 1, DARK_GRAY);
		Rect.top = b - y - 6;
		Rect.bottom = b - y + 6;
		s.Format(_T("%i"), y << (8 - m_Zoom));
		m_MemDC.WriteText(s, Rect, Font12, WHITE, 2);
	}

	// Vertical lines
	for (int i = 0; i <= 512; i += 32) {
		m_MemDC.DrawLine(l + i, b - 256, l + i, b + 1, 1, DARK_GRAY);
	}

	// Signal
	CPen Pen(PS_SOLID, 1, YELLOW);
	CPen* pPen = m_MemDC.SelectObject(&Pen);
	for (int x = 0; x < 512; x++) {
		int y = m_LAD.m_Signal[x] >> (8 - m_Zoom);
		if (x == 0) m_MemDC.MoveTo(l + x, b - y);
		else m_MemDC.LineTo(l + x, b - y);
	}
	m_MemDC.SelectObject(pPen);

	// Centroid
	if (!m_LAD.m_Bad) {
		int x = intRound(m_LAD.m_CentroidPos);
		m_MemDC.DrawLine(l + x, b - 256, l + x, b + 1, 1, RED);
		s.Format(_T("%i"), x);
		Rect.left = l + x + 3;
		Rect.top = b - 15;
		Rect.right = Rect.left + 50;
		Rect.bottom = Rect.top + 16;
		m_MemDC.WriteText(s, Rect, Font16, RED, 0);
	}
}


//***************************************************************************************
