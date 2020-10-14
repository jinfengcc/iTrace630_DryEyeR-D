//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "RMSWnd.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CRMSWnd, CDispWnd)

	ON_BN_CLICKED(IDC_ZOOM_IN, OnZoomInButtonClicked)
	ON_BN_CLICKED(IDC_ZOOM_OUT, OnZoomOutButtonClicked)
	ON_BN_CLICKED(IDC_SHOW_FULL, OnShowSimpRmsButtonClicked) //530

END_MESSAGE_MAP()

//***************************************************************************************

CRMSWnd::CRMSWnd(RECT& Rect, CWnd* pWnd) : CDispWnd(Rect, pWnd)
{
	m_Max = 3;
}

//***************************************************************************************

void CRMSWnd::CreateZoomButtons()
{
	RECT Rect;
	CMRgn Rgn;
	int w = intRound(0.058 * m_w); if (w < 25) w = 25;

	int W = ::GetSystemMetrics(SM_CXSCREEN);

	int l = 2;
	int b = m_h - 2;
	SetRect(&Rect, l, b - w, l + w, b);
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
	SetRect(&Rect, l, b - w, l + w, b);
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
	if (m_Max == 1) m_ZoomInButton.SetEnabled(FALSE);
	m_ZoomInButton.Repaint();
	//521

	m_ZoomInButton.ShowWindow(SW_SHOW);
	m_ZoomOutButton.ShowWindow(SW_SHOW);
}

//***************************************************************************************
//530
void CRMSWnd::CreateZoomButtons2()
{
	RECT Rect;
	CMRgn Rgn;
	int w = intRound(0.058 * m_w); if (w > 15) w = 15;

	int W = ::GetSystemMetrics(SM_CXSCREEN);

	int l = intRound(0.7 * m_w);
	SetRect(&Rect, l, 3, l + w, 3 + w);
	m_ZoomOutButton.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_ZOOM_OUT);
	m_ZoomOutButton.SetImage(m_SizeSmall ?
		(W == 1920 ? IDR_ZOOM_OUT_3 : W == 1600 ? IDR_ZOOM_OUT_2 : IDR_ZOOM_OUT_1) :
		(W == 1920 ? IDR_ZOOM_OUT_6 : W == 1600 ? IDR_ZOOM_OUT_5 : IDR_ZOOM_OUT_4));
	m_ZoomOutButton.SetBk(FALSE);
	m_ZoomOutButton.SetBkColor(BLACK);
	m_ZoomOutButton.SetTip("Zoom out");
	Rgn.SetRectRgn(&Rect);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	l += w + 5;
	SetRect(&Rect, l, 3, l + w, 3 + w);
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
	if (m_Max == 1) m_ZoomInButton.SetEnabled(FALSE);
	m_ZoomInButton.Repaint();
	//521

	m_ZoomInButton.ShowWindow(SW_SHOW);
	m_ZoomOutButton.ShowWindow(SW_SHOW);
}

//***************************************************************************************

void CRMSWnd::OnShowSimpRmsButtonClicked()
{
	GetParent()->PostMessage(WM_SHOW_SIMPRMS, (WPARAM)this, (LPARAM)m_Max);
}

//***************************************************************************************

void CRMSWnd::OnZoomOutButtonClicked()
{
	m_Max++;

	//521
	m_ZoomInButton.SetEnabled(TRUE);
	m_ZoomInButton.Repaint();
	//521

	Repaint();
	GetParent()->PostMessage(WM_CHANGE_MAX, (WPARAM)this, (LPARAM)m_Max);
}

//***************************************************************************************

void CRMSWnd::OnZoomInButtonClicked()
{
	//521
	if (m_Max == 1) m_ZoomInButton.SetEnabled(FALSE);
	m_ZoomInButton.Repaint();
	//521

	if (m_Max == 1) return;
	m_Max--;

	//521
	m_ZoomInButton.SetEnabled(TRUE);
	if (m_Max == 1) m_ZoomInButton.SetEnabled(FALSE);
	m_ZoomInButton.Repaint();
	//521

	Repaint();
	GetParent()->PostMessage(WM_CHANGE_MAX, (WPARAM)this, (LPARAM)m_Max);
}

//***************************************************************************************

void CRMSWnd::RepaintMemDC()
{
	ClearMemDC();
	if (m_Method3D) {
		RmsInd3D();
	}
	else {
		if (m_Individual) {
			RmsInd2D();
		}
		else {
			RmsCmb2D();
		}
	}
	m_CaptionVertical = m_Method3D;
	Labels();
	Border();
}

//***************************************************************************************

void CRMSWnd::RmsInd2D()
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	CString s;
	RECT Rect;

	int FontSize;
	//521
	if (m_isSolo || m_SimpliedRMS)
	{
		FontSize = intRound(0.055 * m_h);
		CMFont Font(FontSize, 400, "Arial");

		real L = m_w * 0.015;
		real T = m_w * 0.050;
		//real h = m_w * 0.0545;
		real h = real(m_h) / 14.5; //0.0545;
		real g = m_w * 0.008;

		real wz = m_w * 0.054;
		real ws = m_w * 0.280;
		real wu = m_w * 0.150;
		real wb = m_w * 0.470;

		real px_um = (real)wb / m_Max;

		// z
		real l = L;
		real r = l + wz + 1;
		real t = T;
		real b = t + h + 1;
		::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b));
		m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);
		m_MemDC.WriteText("z ", Rect, Font, white, 2);

		// name
		l = r - 1; r += ws;
		::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b));
		m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);
		m_MemDC.WriteText("  Name", Rect, Font, white, 0);
		// value
		l = r - 1; r += wu;
		::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b));
		m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);
		m_MemDC.WriteText("µ ", Rect, Font, white, 2);
		// bar
		l = r - 1; r += wb;
		::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b));
		m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);
		// scale
		int k = m_Max / 10 + 1;

		for (int i = 1; i <= m_Max; i++)
		{
			if (i % k > 0) continue;
			real x = l + i * px_um;
			::SetRect(&Rect, intRound(l), intRound(t), intRound(x), intRound(b));
			s.Format(_T("%i "), i);
			m_MemDC.WriteText(s, Rect, Font, white, 2);
			m_MemDC.DrawVertDottedLine(intRound(x), intRound(t + 3), intRound(b - 3), gray, NOCOLOR);
		}

		for (int mode = 3; mode < 15; mode++)
		{
			if (mode != 4)
			{
				real um = m_Surface.GetCUm(mode);
				COLORREF bar_color = um >= 0.0 ? RED : BLUE;
				um = fabs(um);

				int sev = COMP_NORMAL;
				COLORREF lbl_color = white;

				if (mode == 7 || mode == 8) {
					sev = m_Surface.EvaluateMode(7, 8);
				}
				else if (mode == 12) {
					sev = m_Surface.EvaluateMode(12, -1);
				}
				else if (mode == 11 || mode == 13) {
					sev = m_Surface.EvaluateMode(11, 13);
				}
				else if (mode == 6 || mode == 9) {
					sev = m_Surface.EvaluateMode(6, 9);
				}
				else if (mode == 10 || mode == 14) {
					sev = m_Surface.EvaluateMode(10, 14);
				}
				if (sev == COMP_SEVERE) lbl_color = RED;
				else if (sev == COMP_MODERATE) lbl_color = m_Printing ? ORANGE : YELLOW;//520 FROM lbl_color = YELLOW;

				t += h;
				if (CZernikeSurface::m_ord[mode] != CZernikeSurface::m_ord[mode - 1]) t += g;

				b = t + h + 1;

				// z
				l = L;
				r = l + wz + 1;
				::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b));
				m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);
				s.Format(_T("%i "), mode);
				m_MemDC.WriteText(s, Rect, Font, lbl_color, 2);

				// name
				l = L + wz;
				r = l + ws + 1;
				::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b));
				m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);
				//
				s = "  " + CZernikeSurface::m_rms_name[mode];
				m_MemDC.WriteText(s, Rect, Font, lbl_color, 0);
				// value
				l = L + wz + ws;
				r = l + wu + 1;
				::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b));
				m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);
				s.Format(_T("%6.3f "), um);
				m_MemDC.WriteText(s, Rect, Font, lbl_color, 2);
				// bar and grid
				l = L + wz + ws + wu;
				r = l + wb + 1;
				::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b));
				m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);
				//
				r = l + intRound(__min(um, m_Max) * px_um) + 1;

				//531
				if (um > m_Max)
				{
					real midh = (b - t - 4) / 2.0;
					m_MemDC.DrawTriangle(intRound(r), intRound(t + 2), intRound(r), intRound(b - 2), intRound(r + 5), intRound(t + 2 + midh), 1, bar_color, bar_color);
				}
				//531 Done
				::SetRect(&Rect, intRound(l), intRound(t + 2), intRound(r), intRound(b - 2));

				m_MemDC.DrawRectangle(Rect, bar_color, bar_color);
				// grid
				for (int i = 0; i <= m_Max; i++)
				{
					real x = l + i * px_um;
					if (0 < i && i < m_Max)
					{
						m_MemDC.DrawVertDottedLine(intRound(x), intRound(t + 3), intRound(b - 3), gray, NOCOLOR);
					}
					else
					{
						m_MemDC.DrawLine(intRound(x), intRound(t), intRound(x), intRound(b), 1, gray);
					}
				}
			}
		}
	}
	//521 Done
	else
	{
		FontSize = intRound(0.040 * m_h);
		CMFont Font(FontSize, 400, "Arial");

		real L = m_w * 0.015;
		real T = m_w * 0.050;
		real h = m_w * 0.0245;
		real g = m_w * 0.008;

		real wz = m_w * 0.044;
		real wn = m_w * 0.039;
		real wm = m_w * 0.039;
		real ws = m_w * 0.175;
		real wu = m_w * 0.097;
		real wb = m_w * 0.570;

		real px_um = (real)wb / m_Max;

		// z
		real l = L;
		real r = l + wz + 1;
		real t = T;
		real b = t + h + 1;
		::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b));
		m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);
		m_MemDC.WriteText("z ", Rect, Font, white, 2);
		// n
		l = r - 1; r += wn;
		::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b));
		m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);
		m_MemDC.WriteText("n ", Rect, Font, white, 2);
		// m
		l = r - 1; r += wm;
		::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b));
		m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);
		m_MemDC.WriteText("m ", Rect, Font, white, 2);
		// name
		l = r - 1; r += ws;
		::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b));
		m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);
		m_MemDC.WriteText("  Name", Rect, Font, white, 0);
		// value
		l = r - 1; r += wu;
		::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b));
		m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);
		m_MemDC.WriteText("µ ", Rect, Font, white, 2);
		// bar
		l = r - 1; r += wb;
		::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b));
		m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);
		// scale
		int k = m_Max / 10 + 1;

		for (int i = 1; i <= m_Max; i++)
		{
			if (i % k > 0) continue;
			real x = l + i * px_um;
			::SetRect(&Rect, intRound(l), intRound(t), intRound(x), intRound(b));
			s.Format(_T("%i "), i);
			m_MemDC.WriteText(s, Rect, Font, white, 2);
			m_MemDC.DrawVertDottedLine(intRound(x), intRound(t + 3), intRound(b - 3), gray, NOCOLOR);
		}

		for (int mode = 3; mode < 28; mode++)
		{

			real um = m_Surface.GetCUm(mode);
			COLORREF bar_color = um >= 0.0 ? RED : BLUE;
			um = fabs(um);

			int sev = COMP_NORMAL;
			COLORREF lbl_color = white;

			if (mode == 7 || mode == 8) {
				sev = m_Surface.EvaluateMode(7, 8);
			}
			else if (mode == 12) {
				sev = m_Surface.EvaluateMode(12, -1);
			}
			else if (mode == 11 || mode == 13) {
				sev = m_Surface.EvaluateMode(11, 13);
			}
			else if (mode == 6 || mode == 9) {
				sev = m_Surface.EvaluateMode(6, 9);
			}
			else if (mode == 10 || mode == 14) {
				sev = m_Surface.EvaluateMode(10, 14);
			}
			else if (mode == 15 || mode == 20) {
				sev = m_Surface.EvaluateMode(15, 20);
			}
			else if (mode == 21 || mode == 27) {
				sev = m_Surface.EvaluateMode(21, 27);
			}
			if (sev == COMP_SEVERE) lbl_color = RED;
			else if (sev == COMP_MODERATE) lbl_color = m_Printing ? ORANGE : YELLOW;//520 FROM lbl_color = YELLOW;

			t += h;
			if (CZernikeSurface::m_ord[mode] != CZernikeSurface::m_ord[mode - 1]) t += g;

			b = t + h + 1;

			// z
			l = L;
			r = l + wz + 1;
			::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b));
			m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);
			s.Format(_T("%i "), mode);
			m_MemDC.WriteText(s, Rect, Font, lbl_color, 2);
			// n
			l = L + wz;
			r = l + wn + 1;
			::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b));
			m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);
			s.Format(_T("%i "), CZernikeSurface::m_ord[mode]);
			m_MemDC.WriteText(s, Rect, Font, lbl_color, 2);
			// m
			l = L + wz + wn;
			r = l + wm + 1;
			::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b));
			m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);
			s.Format(_T("%i "), CZernikeSurface::m_frq[mode]);
			m_MemDC.WriteText(s, Rect, Font, lbl_color, 2);
			// name
			l = L + wz + wn + wm;
			r = l + ws + 1;
			::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b));
			m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);
			//
			s = "  " + CZernikeSurface::m_rms_name[mode];
			m_MemDC.WriteText(s, Rect, Font, lbl_color, 0);
			// value
			l = L + wz + wn + wm + ws;
			r = l + wu + 1;
			::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b));
			m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);
			s.Format(_T("%6.3f "), um);
			m_MemDC.WriteText(s, Rect, Font, lbl_color, 2);
			// bar and grid
			l = L + wz + wn + wm + ws + wu;
			r = l + wb + 1;
			::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b));
			m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);
			//
			r = l + intRound(__min(um, m_Max) * px_um) + 1;

			//531
			if (um > m_Max)
			{
				real midh = (b - t - 4) / 2.0;
				m_MemDC.DrawTriangle(intRound(r), intRound(t + 2), intRound(r), intRound(b - 2), intRound(r + 5), intRound(t + 2 + midh), 1, bar_color, bar_color);
			}
			//531 Done

			::SetRect(&Rect, intRound(l), intRound(t + 2), intRound(r), intRound(b - 2));
			m_MemDC.DrawRectangle(Rect, bar_color, bar_color);
			// grid
			for (int i = 0; i <= m_Max; i++) {
				real x = l + i * px_um;
				if (0 < i && i < m_Max) {
					m_MemDC.DrawVertDottedLine(intRound(x), intRound(t + 3), intRound(b - 3), gray, NOCOLOR);
				}
				else {
					m_MemDC.DrawLine(intRound(x), intRound(t), intRound(x), intRound(b), 1, gray);
				}
			}
		}
	}
}

//***************************************************************************************

void CRMSWnd::RmsCmb2D()
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	COLORREF Colors[15];
	Colors[14] = 0x000000aa;
	Colors[13] = 0x000000dc;
	Colors[12] = 0x000064e6;
	Colors[11] = 0x000096f0;
	Colors[10] = 0x0000d7ff;
	Colors[9] = 0x0000ffff;
	Colors[8] = 0x0000dcb4;
	Colors[7] = 0x0000be00;
	Colors[6] = 0x006ed200;
	Colors[5] = 0x00dcdc00;
	Colors[4] = 0x00c88000;
	Colors[3] = 0x00c80000;
	Colors[2] = 0x00c8004b;
	Colors[1] = 0x00c80082;
	Colors[0] = 0x007d0052;
	real step = 2.0 * m_Max / (15 - 1);
	step = ((int)(step / 0.1) + 1) * 0.1;
	real min = -step * (15 - 1) / 2;

	int FontSize = intRound(0.036 * m_h);
	CMFont Font(FontSize, 400, "Arial");

	real L = m_w * 0.014;
	real T = m_w * 0.050;

	real wr = m_w * 0.235;
	real hr = m_w * 0.225;
	real g = m_w * 0.008;

	real wb = m_w * 0.028;
	real hb = m_w * 0.169;

	int r_max_px = intRound(m_w * 0.070);
	real r_max_um = m_Surface.m_r_max_um;
	real map_px_um = r_max_px / r_max_um;

	real bar_px_um = (real)hb / m_Max;

	int MaskType[3][4] =
	{
		{ MASK_TOTAL, MASK_LO_TOTAL   , MASK_HO_TOTAL, MASK_NONE },
		{ MASK_NONE , MASK_DEFOCUS    , MASK_COMA    , MASK_SPHERICAL },
		{ MASK_NONE , MASK_ASTIGMATISM, MASK_TREFOIL , MASK_4ORD_ASTIGMATISM }
	};

	RECT Rect[3][4];
	for (int j = 0; j < 3; j++) 
	{
		for (int i = 0; i < 4; i++) 
		{
			real l = L + i * (wr + g);
			real t = T + j * (hr + g);
			::SetRect(&Rect[j][i], intRound(l), intRound(t), intRound(l + wr + 1), intRound(t + hr + 1));
		}
	}

	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 4; i++)
		{

			if (MaskType[j][i] == MASK_NONE) continue;

			CZernikeMask Mask;
			Mask.SetType(MaskType[j][i]);

			CZernikeSurface Surface;
			Surface = m_Surface;
			Surface &= Mask;
			real rms = Surface.ComputeRms();

			int dg = -1;
			switch (MaskType[j][i]) {
			case MASK_ASTIGMATISM:
				dg = intRound(angle(Surface.m_c[3], Surface.m_c[5]) * _180_Pi / 2.0) % 180;
				break;
			case MASK_COMA:
				dg = intRound(angle(Surface.m_c[7], Surface.m_c[8]) * _180_Pi) % 360;
				break;
			case MASK_TREFOIL:
				dg = intRound(angle(Surface.m_c[6], Surface.m_c[9]) * _180_Pi / 3.0) % 120;
				break;
			case MASK_4ORD_ASTIGMATISM:
				dg = intRound(angle(Surface.m_c[11], Surface.m_c[13]) * _180_Pi / 2.0) % 180;
				break;
			}

			// frame
			RECT rct = Rect[j][i];
			m_MemDC.DrawRectangle(rct, gray, NOCOLOR);

			// name
			rct.bottom = rct.top + FontSize;
			CString s;
			Mask.GetName(s);
			m_MemDC.WriteText(" " + s.Left(16), rct, Font, white, 0);

			// value
			rct.top += FontSize; rct.bottom += FontSize;
			s.Format(_T(" %.3f µ"), rms);
			if (MaskType[j][i] == MASK_DEFOCUS) {
				if (Surface.m_c[4] > 0.0) s = "+" + s;
				else if (Surface.m_c[4] < 0.0) s = "-" + s;
			}
			else if (MaskType[j][i] == MASK_SPHERICAL) {
				if (Surface.m_c[12] > 0.0) s = "+" + s;
				else if (Surface.m_c[12] < 0.0) s = "-" + s;
			}
			// angle
			if (dg != -1) {
				CString s2;
				s2.Format(_T(" x %i°"), dg);
				s += s2;
			}
			m_MemDC.WriteText(s, rct, Font, white, 0);

			// bar
			rct = Rect[j][i];
			rct.left = rct.right - intRound(0.15 * wr);
			rct.top = rct.bottom - intRound(__min(rms, m_Max) * bar_px_um) - 1;

			//531
			if (rms > m_Max)
			{
				real midW = real(rct.right - rct.left) / 2.0;
				m_MemDC.DrawTriangle(intRound(rct.left), intRound(rct.top), intRound(rct.right - 1), intRound(rct.top), intRound(rct.left + midW), intRound(rct.top - 5), 1, white, white);
			}
			//531 Done

			m_MemDC.DrawRectangle(rct, white, white);

			// bar grid
			rct.top = rct.bottom - intRound(hb) - 1;
			m_MemDC.DrawRectangle(rct, gray, NOCOLOR);
			for (int k = 1; k < m_Max; k++) {
				m_MemDC.DrawHorzDottedLine(rct.left, rct.right - 1, rct.bottom - intRound(k * bar_px_um) - 1, gray, NOCOLOR);
			}

			// map
			rct = Rect[j][i];
			int ox = intRound((rct.left + (rct.right - wb - g)) / 2);
			int oy = ((rct.top + FontSize + FontSize) + rct.bottom) / 2;
			int l = ox - r_max_px;
			int r = ox + r_max_px;
			int t = oy - r_max_px;
			int b = oy + r_max_px;
			for (int y = t; y <= b; y++) 
			{
				real y_um = (oy - y) / map_px_um;
				for (int x = l; x <= r; x++) 
				{
					real x_um = (x - ox) / map_px_um;
					real r_um = hyp(x_um, y_um);
					if (r_um < r_max_um) 
					{
						real a_rd = angle(y_um, x_um);
						real w_um = 0.0;
						if (Surface.GetAt(r_um, a_rd, &w_um, NULL, NULL, NULL, NULL, NULL)) 
						{
							int c = intRound((w_um - min) / step);
							if (c < 0) c = 0; else if (c >= 15) c = 15 - 1;
							COLORREF C = Colors[c];
							m_MemDC.SetPixel(x, y, GetRValue(C), GetGValue(C), GetBValue(C));
						}
					}
				}
			}

		}
	}

}

//***************************************************************************************

void CRMSWnd::RmsInd3D()
{
	real black = m_Printing ? 1.0 : 0.0;
	real white = m_Printing ? 0.0 : 1.0;
	real gray = m_Printing ? 0.75 : 0.25;

	HGLRC hRenderingContext = NULL;
	int PixelFormat = ::ChoosePixelFormat(m_MemDC.m_hDC, &m_PixelFormatDescriptor);
	if (PixelFormat != 0) {
		if (::SetPixelFormat(m_MemDC.m_hDC, PixelFormat, &m_PixelFormatDescriptor)) {
			hRenderingContext = ::wglCreateContext(m_MemDC.m_hDC);
		}
	}
	if (hRenderingContext == NULL) return;

	::wglMakeCurrent(m_MemDC.m_hDC, hRenderingContext);

	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();
	real p = (real)m_w / m_h;
	::glOrtho(-p, p, -1.0, 1.0, -5.0, 5.0);

	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();
	::glRotated(-60 - m_ay, 1.0, 0.0, 0.0);
	::glRotated(135 - m_ax, 0.0, 0.0, 1.0);
	::glTranslated(0.0, 0.0, -0.5);
	::glScaled(0.9, 0.9, 1.0);

	::glViewport(0, 0, m_w, m_h);

	::glClearColor((GLclampf)black, (GLclampf)black, (GLclampf)black, 1.0f);
	::glClearDepth(1.0);
	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	::glDepthFunc(GL_LEQUAL);
	::glEnable(GL_DEPTH_TEST);
	::glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	::glColor3d(gray, gray, gray);
	::glBegin(GL_LINE_LOOP);
	for (int a = 0; a < 360; a++) ::glVertex3d(SIN[a], COS[a], 0.0);
	::glEnd();

	int a_bg = -m_ax + 45;
	int a_fn = -m_ax + 225;
	for (int i = 1; i <= m_Max; i++) {
		real h = (real)i / m_Max;
		::glBegin(GL_LINE_STRIP);
		for (int a = a_bg; a <= a_fn; a++) {
			int b = CheckAngle(a);
			::glVertex3d(SIN[b], COS[b], h);
		}
		::glEnd();
	}

	::glBegin(GL_LINES);
	int b = CheckAngle(a_bg);
	::glVertex3d(SIN[b], COS[b], 0.0);
	::glVertex3d(SIN[b], COS[b], 1.0);
	b = CheckAngle(a_fn);
	::glVertex3d(SIN[b], COS[b], 0.0);
	::glVertex3d(SIN[b], COS[b], 1.0);
	::glEnd();


	::glEnable(GL_POLYGON_OFFSET_FILL);
	::glPolygonOffset(0.0, 10000.0);

	real ymin = -0.4;
	real ymax = 0.8;
	real xmin = -0.7;
	real xmax = 0.7;

	real dy = (ymax - ymin) / (ZERNIKE_NUM_ORDERS - 3);
	real dx = (xmax - xmin) / (ZERNIKE_NUM_ORDERS - 1);
	real ay = 0.37 * dy;
	real ax = 0.46 * dx;
	real y = ymax + dy;
	real x;
	for (int mode = 3; mode < ZERNIKE_NUM_MODES; mode++) {

		if (CZernikeSurface::m_ord[mode] != CZernikeSurface::m_ord[mode - 1]) {
			y -= dy;
			x = -dx * 0.5 * CZernikeSurface::m_ord[mode];
		}

		real um = m_Surface.GetCUm(mode);

		real r, g, b;
		if (um < -0.001) { r = 0.0; g = 0.0; b = 1.0; }
		else if (um > 0.001) { r = 1.0; g = 0.0; b = 0.0; }
		else {
			real c = m_Printing ? 0.8 : 0.2;
			r = g = b = c;
		}

		real z = __min(fabs(um) / m_Max, 1.0);

		// боковые четырёхугольники
		real bx = ax * (1.0 - COS[60]);
		real by = ay * (1.0 - COS[60]);
		real s[6][4][3] = {
			{ { x - bx,y - ay,0 },{ x - ax,y   ,0 },{ x - ax,y   ,z },{ x - bx,y - ay,z } },
			{ { x - ax,y   ,0 },{ x - bx,y + ay,0 },{ x - bx,y + ay,z },{ x - ax,y   ,z } },
			{ { x - bx,y + ay,0 },{ x + bx,y + ay,0 },{ x + bx,y + ay,z },{ x - bx,y + ay,z } },
			{ { x + bx,y + ay,0 },{ x + ax,y   ,0 },{ x + ax,y   ,z },{ x + bx,y + ay,z } },
			{ { x + ax,y   ,0 },{ x + bx,y - ay,0 },{ x + bx,y - ay,z },{ x + ax,y   ,z } },
			{ { x + bx,y - ay,0 },{ x - bx,y - ay,0 },{ x - bx,y - ay,z },{ x + bx,y - ay,z } }
		};
		for (int p = 0; p < 6; p++) {
			::glBegin(GL_POLYGON);
			::glColor3d(r, g, b);
			for (int v = 0; v < 4; v++) ::glVertex3d(s[p][v][1], s[p][v][0], s[p][v][2]);
			::glEnd();

			::glBegin(GL_LINE_LOOP);
			::glColor3d(0.5*r, 0.5*g, 0.5*b);
			for (int v = 0; v < 4; v++) ::glVertex3d(s[p][v][1], s[p][v][0], s[p][v][2]);
			::glEnd();
		}

		// нижний и верхний шестиугольники
		real u[2][6][3] = {
			{ { x - bx,y - ay,0 },{ x - ax,y,0 },{ x - bx,y + ay,0 },{ x + bx,y + ay,0 },{ x + ax,y,0 },{ x + bx,y - ay,0 } },
			{ { x - bx,y - ay,z },{ x - ax,y,z },{ x - bx,y + ay,z },{ x + bx,y + ay,z },{ x + ax,y,z },{ x + bx,y - ay,z } }
		};
		for (int p = 0; p < 2; p++) {
			::glBegin(GL_POLYGON);
			::glColor3d(r, g, b);
			for (int v = 0; v < 6; v++) ::glVertex3d(u[p][v][1], u[p][v][0], u[p][v][2]);
			::glEnd();
		}

		x += dx;
	}

	::glDisable(GL_POLYGON_OFFSET_FILL);

	::glFlush();

	::wglMakeCurrent(NULL, NULL);

	::wglDeleteContext(hRenderingContext);
}

//***************************************************************************************
