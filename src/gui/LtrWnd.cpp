//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "LtrWnd.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CLtrWnd, CDispWnd)

	ON_BN_CLICKED(IDC_ZOOM_IN, OnZoomInButtonClicked)
	ON_BN_CLICKED(IDC_ZOOM_OUT, OnZoomOutButtonClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CLtrWnd::CLtrWnd(RECT& Rect, CWnd* pWnd) : CDispWnd(Rect, pWnd)
{
	m_Zoom = 400;
}

//***************************************************************************************

void CLtrWnd::CreateZoomButtons()
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

void CLtrWnd::OnZoomInButtonClicked()
{
	switch (m_Zoom) {
	case 100: m_Zoom = 200; break;
	case 200: m_Zoom = 400; break;
	default: break;
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

void CLtrWnd::OnZoomOutButtonClicked()
{
	switch (m_Zoom) {
	case 400: m_Zoom = 200; break;
	case 200: m_Zoom = 100; break;
	default: break;
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

void CLtrWnd::RepaintMemDC()
{
	ClearMemDC();
	Letter();
	Labels();
	Border();
}

//***************************************************************************************

void CLtrWnd::Letter()
{
	real_t _0 = m_Printing ? 1.0 : 0.0;
	real_t _1 = m_Printing ? 0.0 : 1.0;

	int n = m_Ltr.m_Array.GetNumRows();

	HGLRC hRenderingContext = NULL;

	int PixelFormat = ::ChoosePixelFormat(m_MemDC.m_hDC, &m_PixelFormatDescriptor);
	if (PixelFormat != 0) 
	{
		if (::SetPixelFormat(m_MemDC.m_hDC, PixelFormat, &m_PixelFormatDescriptor))
		{
			hRenderingContext = ::wglCreateContext(m_MemDC.m_hDC);
		}
	}

	if (hRenderingContext == NULL) return;

	::wglMakeCurrent(m_MemDC.m_hDC, hRenderingContext);

	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();

	real_t p = (real_t)m_w / m_h;

	::glOrtho(-p, p, -1.0, 1.0, -5.0, 5.0);

	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();

	if (m_Method3D)
	{
		::glRotated(-45 - m_ay, 1.0, 0.0, 0.0);
		::glRotated(-45 - m_ax, 0.0, 0.0, 1.0);
		::glTranslated(0.0, 0.0, -0.2);
	}

	real_t q = 1.0;
	q *= 0.01 * m_Zoom;
	q *= n / 1024.0;
	q *= 2.0 / n;
	q *= 0.825;
	::glScaled(q, q, 1.0);

	::glViewport(0, 0, m_w, m_h);

	::glClearColor((GLclampf)_0, (GLclampf)_0, (GLclampf)_0, (GLclampf)1.0);
	::glClearDepth(1.0);
	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	::glDepthFunc(GL_LEQUAL);
	::glEnable(GL_DEPTH_TEST);
	::glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// letter
	int o = n >> 1;
	real_t thr = 0.01;
	for (int j = 0; j < n - 1; j++)
	{
		int y[4] = { j, j + 1, j + 1, j };
		for (int i = 0; i < n - 1; i++)
		{
			int x[4] = { i, i, i + 1, i + 1 };
			real_t z[4];

			for (int k = 0; k < 4; k++)
			{
				z[k] = m_Ltr.m_Array(y[k], x[k]);
			}

			if (z[0] > thr || z[1] > thr || z[2] > thr || z[3] > thr)
			{
				::glBegin(GL_POLYGON);
				for (int k = 0; k < 4; k++)
				{
					real_t c = m_Printing ? 1.0 - z[k] : z[k];
					::glColor3d(c, c, c);
					::glVertex3d(x[k] - o, y[k] - o, z[k]);
				}
				::glEnd();
			}
		}
	}

	// grid
	::glColor3d(m_Printing ? 0.8 : 0.2, m_Printing ? 0.8 : 0.2, m_Printing ? 0.8 : 0.2);
	real_t g_mn = 10.0;
	int m;

	switch (m_Zoom) 
	{
	case 100: m = 12; break;
	case 200: m = 6; break;
	case 400: m = 3; break;
	}

	real_t a = m * g_mn * m_Ltr.m_px_mn;
	::glBegin(GL_LINES);

	for (int i = -m; i <= m; i++)
	{
		real_t b = i * g_mn * m_Ltr.m_px_mn;
		::glVertex3d(-a, b, 0.0); ::glVertex3d(a, b, 0.0);
		::glVertex3d(b, -a, 0.0); ::glVertex3d(b, a, 0.0);
	}
	::glEnd();

	// axes
	real_t u = 1.02 * a;
	::glColor3d(_1, _1, _1);
	::glBegin(GL_LINES);
	::glVertex3d(-u, 0.0, 0.0); ::glVertex3d(-a, 0.0, 0.0); // Ox
	::glVertex3d(u, 0.0, 0.0); ::glVertex3d(a, 0.0, 0.0); // Ox
	::glVertex3d(0.0, -u, 0.0); ::glVertex3d(0.0, -a, 0.0); // Oy
	::glVertex3d(0.0, u, 0.0); ::glVertex3d(0.0, a, 0.0); // Oy
	::glEnd();

	if (m_Method3D)
	{
		// X arrow
		::glBlendFunc(GL_ONE, GL_ZERO);
		::glColor3d(_1, _1, _1);
		::glBegin(GL_TRIANGLES);
		::glVertex3d(1.15*a, 0.0, 0.0); ::glVertex3d(1.13*a, -0.02*a, 0.0); ::glVertex3d(1.21*a, 0.0, 0.0);
		::glVertex3d(1.15*a, 0.0, 0.0); ::glVertex3d(1.13*a, 0.02*a, 0.0); ::glVertex3d(1.21*a, 0.0, 0.0);
		::glEnd();
		// Y arrow
		::glBlendFunc(GL_ONE, GL_ZERO);
		::glBegin(GL_TRIANGLES);
		::glVertex3d(0.0, 1.15*a, 0.0); ::glVertex3d(-0.02*a, 1.13*a, 0.0); ::glVertex3d(0.0, 1.21*a, 0.0);
		::glVertex3d(0.0, 1.15*a, 0.0); ::glVertex3d(0.02*a, 1.13*a, 0.0); ::glVertex3d(0.0, 1.21*a, 0.0);
		::glEnd();
	}

	::glFlush();

	::wglMakeCurrent(NULL, NULL);

	::wglDeleteContext(hRenderingContext);

	//---------------------------------------------------

	int FontSize = intRound(0.045 * m_h);
	CMFont Font(FontSize, 400, "Arial");

	COLORREF white = m_Printing ? BLACK : WHITE;
	CString s;
	RECT Rect;

	// units
	s.Format(_T("Spacing:  %i arcmin"), (int)g_mn);
	::SetRect(&Rect, 0, m_h - 3, m_w, m_h - 3 - FontSize);
	m_MemDC.WriteText(s, Rect, Font, white, 1);

	// Dray Dysfunctional index  
	if (m_DysVal >= 0 && m_DysVal <= 10)
	{
		int FontSize = intRound(0.045 * m_h);
		CMFont Font(FontSize, 400, "Arial");

		int ValFontSize = intRound(0.085 * m_h);//601	  
		CMFont ValFont(ValFontSize, 400, "Arial");//601

		COLORREF CLColor[10];
		CLColor[9] = RGB(250, 0, 0);
		CLColor[8] = RGB(220, 46, 0);
		CLColor[7] = RGB(227, 97, 0);
		CLColor[6] = RGB(227, 137, 0);
		CLColor[5] = RGB(227, 176, 0);
		CLColor[4] = RGB(200, 200, 0);
		CLColor[3] = RGB(176, 227, 0);
		CLColor[2] = RGB(137, 227, 0);
		CLColor[1] = RGB(66, 207, 0);
		CLColor[0] = RGB(17, 247, 100);

		real_t d_w = 0.05*m_w;
		int  L = m_w - 10 - intRound(d_w);
		int  R = m_w - 10;

		int d_per_h = intRound(0.08*m_h);
		real_t Top = intRound(0.1*m_h);
		int  Bottom = intRound(0.9*m_h);

		for (int i = 0; i < 10; i++)
		{
			int NextPos = intRound(Top + d_per_h);
			if (i == 9) NextPos = Bottom;
			::SetRect(&Rect, L, intRound(Top), R, NextPos);
			m_MemDC.DrawRectangle(Rect, CLColor[i], CLColor[i]);


			if (i == 0 || i == 5)
			{
				::SetRect(&Rect, L - FontSize - 2, intRound(Top) - FontSize, L - 2, intRound(Top) + FontSize);
				if (i == 0) m_MemDC.WriteText("10", Rect, Font, CLColor[i], 1);
				else       m_MemDC.WriteText("5", Rect, Font, CLColor[i], 1);
			}

			Top = NextPos;

			if (i == 9)
			{
				::SetRect(&Rect, L - FontSize - 2, intRound(Top) - FontSize, L - 2, intRound(Top) + FontSize);
				m_MemDC.WriteText("0", Rect, Font, CLColor[i], 1);
			}
		}

		//Arrow and value
		int x0 = L - FontSize - 4;
		int y0 = intRound(0.1*m_h + (10 - m_DysVal)*0.08*m_h);

		int x1 = x0 - 8;
		int y1 = y0 - 6;

		int x2 = x1;
		int y2 = y0 + 6;

		int cn = intRound(10 - m_DysVal);
		if (cn == 10) cn = 9;

		if (!m_SizeSmall)
		{
			x2 = x1 = x1 - 8;
			y1 -= 6;
			y2 += 6;
		}

		m_MemDC.DrawTriangle(x0, y0, x1, y1, x2, y2, 1, CLColor[cn], CLColor[cn]);

		x1 = x0 - 25;
		y1 = y0 - 2;
		y2 = y0 + 2;
		
		if (!m_SizeSmall)
		{
			x1 = x1 - 20;
			y1 -= 3;
			y2 += 3;
		}

		::SetRect(&Rect, x1, y1, x2, y2);
		m_MemDC.DrawRectangle(Rect, CLColor[cn], CLColor[cn]);

		if (m_DysVal == 10.00)
		{
			::SetRect(&Rect, x1 - ValFontSize - ValFontSize, y0 + 8, x0, y0 + 8 + ValFontSize);
		}																												  
		else
		{
			if (m_DysVal <= 0.8)
			{
				if (!m_SizeSmall) ::SetRect(&Rect, x1 - intRound(1.8*ValFontSize), y0 - 8 - ValFontSize, x0, y0 - 8);
				else ::SetRect(&Rect, x1 - ValFontSize, y0 - 8 - ValFontSize, x0, y0 - 8);
			}
			else
			{
				if (!m_SizeSmall) ::SetRect(&Rect, x1 - intRound(1.8*ValFontSize), y0 + 8, x0, y0 + 8 + ValFontSize);
				else ::SetRect(&Rect, x1 - ValFontSize, y0 + 8, x0, y0 + 8 + ValFontSize);
			}
		}

		CString s;
		s.Format(_T("%.2f"), m_DysVal);
		m_MemDC.WriteText(s, Rect, ValFont, CLColor[cn], 0);
	}
	// Dysfunctional index  Done

}

//***************************************************************************************
