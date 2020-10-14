//***************************************************************************************

#include "StdAfx.h"
#include "Settings.h"
#include "Resource.h"
#include "DispWnd.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

const PIXELFORMATDESCRIPTOR CDispWnd::m_PixelFormatDescriptor =
{
	sizeof(PIXELFORMATDESCRIPTOR),
	1,
	PFD_SUPPORT_OPENGL | PFD_DRAW_TO_BITMAP,
	PFD_TYPE_RGBA,
	24,
	0, 0, 0, 0, 0, 0,
	0, 0,
	0, 0, 0, 0, 0,
	32,
	0,
	0,
	PFD_MAIN_PLANE,
	0, 0, 0, 0
};

//***************************************************************************************

BEGIN_MESSAGE_MAP(CDispWnd, CMemWnd)

	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_BN_CLICKED(IDC_INC_ZONE, OnIncZoneButtonClicked)
	ON_BN_CLICKED(IDC_DEC_ZONE, OnDecZoneButtonClicked)
	ON_BN_CLICKED(IDC_SOLO_MASK_ITEM, OnSoloMaskButtonClicked)  

END_MESSAGE_MAP()

//***************************************************************************************

CDispWnd::CDispWnd(RECT& Rect, CWnd* pWnd)
{
	CreateWnd(Rect, pWnd);

	m_Type = 0;
	m_Method3D = FALSE;
	m_SizeSmall = TRUE;
	m_CaptionVertical = TRUE;

	m_LTLabelFontSizePrc = 0.050;
	m_LTLabelColor = WHITE;
	m_LTLabelBkColor = NOCOLOR;

	m_RTLabelFontSizePrc = 0.045;
	m_RTLabelColor = WHITE;
	m_RTLabelBkColor = NOCOLOR;

	m_RBLabelFontSizePrc = 0.045;
	m_RBLabelColor[0] = WHITE;
	m_RBLabelColor[1] = WHITE;
	m_RBLabelColor[2] = WHITE;
	m_RBLabelColor[3] = WHITE;
	m_RBLabelColor[4] = WHITE;
	m_RBLabelColor[5] = WHITE;
	m_RBLabelBkColor = NOCOLOR;

	m_l = 0;

	m_mx = INVALID_VALUE;
	m_my = INVALID_VALUE;

	m_SIMLTLabel[0] = "";

	m_ax = 0;
	m_ay = 0;

	m_LTBigFont = FALSE;//530
}

//***************************************************************************************

// This button used to change the RMS to full or simplified gui
void CDispWnd::CreateSimpButtons(BOOL ShowDetails)
{
	RECT Rect;
	CMRgn Rgn;
	int w = intRound(0.058 * m_w); if (w < 25) w = 25;

	int W = ::GetSystemMetrics(SM_CXSCREEN);

	int r = intRound(real(m_w) / 2.0 - 2);
	int b = m_h - 2;
	::SetRect(&Rect, r - w, b - w, r, b);

	CString s = (!ShowDetails ? "Show Details" : "Hide Details");
	m_ShowMoreButton.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_SHOW_FULL);

	if (m_SizeSmall)
	{
		m_ShowMoreButton.SetImage(!ShowDetails ?
			(W == 1920 ? IDR_SHOW_DETAIL_3 : W == 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
			(W == 1920 ? IDR_HIDE_DETAIL_3 : W == 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
	}
	else
	{
		m_ShowMoreButton.SetImage(!ShowDetails ?
			(W == 1920 ? IDR_SHOW_DETAIL_6 : W == 1600 ? IDR_SHOW_DETAIL_5 : IDR_SHOW_DETAIL_4) :
			(W == 1920 ? IDR_HIDE_DETAIL_6 : W == 1600 ? IDR_HIDE_DETAIL_5 : IDR_HIDE_DETAIL_4));
	}

	m_ShowMoreButton.SetBk(FALSE);
	m_ShowMoreButton.SetBkColor(BLACK);
	m_ShowMoreButton.SetTip(s);
	Rgn.SetRectRgn(&Rect);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	m_ShowMoreButton.ShowWindow(SW_SHOW);
}


//***************************************************************************************
void CDispWnd::CreateZoneButtons()
{
	RECT Rect;
	CMRgn Rgn;
	int w = intRound(0.058 * m_w); if (w < 25) w = 25;

	int W = ::GetSystemMetrics(SM_CXSCREEN);

	int r = m_w - 2;
	int b = m_h - 2;
	::SetRect(&Rect, r - w, b - w, r, b);
	m_IncZoneButton.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_INC_ZONE);
	m_IncZoneButton.SetImage(m_SizeSmall ?
		(W == 1920 ? IDR_INC_ZONE_3 : W == 1600 ? IDR_INC_ZONE_2 : IDR_INC_ZONE_1) :
		(W == 1920 ? IDR_INC_ZONE_6 : W == 1600 ? IDR_INC_ZONE_5 : IDR_INC_ZONE_4));
	m_IncZoneButton.SetBk(FALSE);
	m_IncZoneButton.SetBkColor(BLACK);
	m_IncZoneButton.SetTip("Increase zone size");
	Rgn.SetRectRgn(&Rect);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	r -= w;
	::SetRect(&Rect, r - w, b - w, r, b);
	m_DecZoneButton.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_DEC_ZONE);
	m_DecZoneButton.SetImage(m_SizeSmall ?
		(W == 1920 ? IDR_DEC_ZONE_3 : W == 1600 ? IDR_DEC_ZONE_2 : IDR_DEC_ZONE_1) :
		(W == 1920 ? IDR_DEC_ZONE_6 : W == 1600 ? IDR_DEC_ZONE_5 : IDR_DEC_ZONE_4));
	m_DecZoneButton.SetBk(FALSE);
	m_DecZoneButton.SetBkColor(BLACK);
	m_DecZoneButton.SetTip("Decrease zone size");
	Rgn.SetRectRgn(&Rect);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	m_DecZoneButton.ShowWindow(SW_SHOW);
	m_IncZoneButton.ShowWindow(SW_SHOW);
}
//***************************************************************************************

void CDispWnd::CreateZoneButtons2()
{
	RECT Rect;
	CMRgn Rgn;
	int w = intRound(0.058 * m_w); if (w > 15) w = 15;

	int W = ::GetSystemMetrics(SM_CXSCREEN);

	int r = m_w - 7;
	::SetRect(&Rect, r - w, 3, r, 3 + w);
	m_IncZoneButton.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_INC_ZONE);
	m_IncZoneButton.SetImage(m_SizeSmall ?
		(W == 1920 ? IDR_INC_ZONE_3 : W == 1600 ? IDR_INC_ZONE_2 : IDR_INC_ZONE_1) :
		(W == 1920 ? IDR_INC_ZONE_6 : W == 1600 ? IDR_INC_ZONE_5 : IDR_INC_ZONE_4));
	m_IncZoneButton.SetBk(FALSE);
	m_IncZoneButton.SetBkColor(BLACK);
	m_IncZoneButton.SetTip("Increase zone size");
	Rgn.SetRectRgn(&Rect);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	r -= w;
	r -= 5;
	::SetRect(&Rect, r - w, 3, r, 3 + w);
	m_DecZoneButton.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_DEC_ZONE);
	m_DecZoneButton.SetImage(m_SizeSmall ?
		(W == 1920 ? IDR_DEC_ZONE_3 : W == 1600 ? IDR_DEC_ZONE_2 : IDR_DEC_ZONE_1) :
		(W == 1920 ? IDR_DEC_ZONE_6 : W == 1600 ? IDR_DEC_ZONE_5 : IDR_DEC_ZONE_4));
	m_DecZoneButton.SetBk(FALSE);
	m_DecZoneButton.SetBkColor(BLACK);
	m_DecZoneButton.SetTip("Decrease zone size");
	Rgn.SetRectRgn(&Rect);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	m_DecZoneButton.ShowWindow(SW_SHOW);
	m_IncZoneButton.ShowWindow(SW_SHOW);
}

//***************************************************************************************

// pos 0 : right bottom corner; 1 : right bottom corrner, but move left for zone butoon space, 2: right bottom corrner, but move up for Presbia text space,
void CDispWnd::CreateSoloMaskButtons(int makeType, int pos)
{
	RECT Rect;
	CMRgn Rgn;
	int w = intRound(0.058 * m_w); if (w < 25) w = 25;

	int W = ::GetSystemMetrics(SM_CXSCREEN);

	int r = m_w - 2;
	int b = m_h - 2;

	if (pos == 1) r = r - 2 * w - 2;
	else if (pos == 2) b = b - w - 2;

	::SetRect(&Rect, intRound(r - 1.5*w), b - w, r, b);
	m_SoloMaskButton.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_SOLO_MASK_ITEM);

	if (makeType == MASK_TOTAL)
	{
		// m_SoloMaskButton.SetImage(m_SizeSmall ?
		//(W == 1920 ? IDR_SOLO_NOCOR3 : W == 1600 ? IDR_SOLO_NOCOR2 : IDR_SOLO_NOCOR1) :
		//(W == 1920 ? IDR_SOLO_NOCOR6 : W == 1600 ? IDR_SOLO_NOCOR5 : IDR_SOLO_NOCOR4));
		m_SoloMaskButton.SetImage(m_SizeSmall ?
			(W == 1920 ? IDR_SOLO_NOCOR3 : W == 1600 ? IDR_SOLO_NOCOR2 : IDR_SOLO_NOCOR1) :
			(W == 1920 ? IDR_SOLO_NOCOR6 : W == 1600 ? IDR_SOLO_NOCOR5 : IDR_SOLO_NOCOR4));
		m_SoloMaskButton.SetTip("Displayed without spectacle correction");
	}
	else if (makeType == MASK_HO_TOTAL)
	{
		m_SoloMaskButton.SetImage(m_SizeSmall ?
			(W == 1920 ? IDR_SOLO_COR3 : W == 1600 ? IDR_SOLO_COR2 : IDR_SOLO_COR1) :
			(W == 1920 ? IDR_SOLO_COR6 : W == 1600 ? IDR_SOLO_COR5 : IDR_SOLO_COR4));
		m_SoloMaskButton.SetTip("Displayed with spectacle correction");
	}
	else if (makeType == MASK_TOTAL_NO_DEFOCUS)
	{
		m_SoloMaskButton.SetImage(m_SizeSmall ?
			(W == 1920 ? IDR_SOLO_CORSP3 : W == 1600 ? IDR_SOLO_CORSP2 : IDR_SOLO_CORSP1) :
			(W == 1920 ? IDR_SOLO_CORSP6 : W == 1600 ? IDR_SOLO_CORSP5 : IDR_SOLO_CORSP4));
		m_SoloMaskButton.SetTip("Displayed with spherical correction only");
	}

	m_SoloMaskButton.SetBk(FALSE);
	m_SoloMaskButton.SetBkColor(BLACK);
	Rgn.SetRectRgn(&Rect);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	m_SoloMaskButton.ShowWindow(SW_SHOW);
}

//***************************************************************************************

void CDispWnd::OnSoloMaskButtonClicked()
{
	GetParent()->PostMessage(WM_SOLO_MASK, (WPARAM)this, 0);
}

//***************************************************************************************
void CDispWnd::OnDecZoneButtonClicked()
{
	GetParent()->PostMessage(WM_DEC_ZONE, (WPARAM)this, 0);
}

//***************************************************************************************

void CDispWnd::OnIncZoneButtonClicked()
{
	GetParent()->PostMessage(WM_INC_ZONE, (WPARAM)this, 0);
}

//***************************************************************************************

void CDispWnd::OnMouseMove(uint nFlags, CPoint Point)
{
	if (m_mx == INVALID_VALUE) {

		TRACKMOUSEEVENT Event = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, 0 };
		::TrackMouseEvent(&Event);
	}

	else {

		if ((nFlags & MK_LBUTTON) && m_Method3D) {

			m_ax += m_mx - Point.x;
			m_ax %= 360; if (m_ax < 0) m_ax += 360;

			m_ay += m_my - Point.y;
			m_ay %= 360; if (m_ay < 0) m_ay += 360;

			Repaint();

			GetParent()->PostMessage(WM_ROTATE, (WPARAM)this, 0);
		}
	}

	m_mx = Point.x;
	m_my = Point.y;
}

//***************************************************************************************

LRESULT CDispWnd::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_mx = INVALID_VALUE;
	m_my = INVALID_VALUE;

	return 0;
}

//***************************************************************************************

void CDispWnd::Labels()
{
	int L = m_l + intRound(0.012 * m_w);
	int R = m_w - intRound(0.012 * m_w);
	int T = intRound(0.012 * m_h);
	int B = m_h - intRound(0.012 * m_h);

	RECT Rect;
	CString s;
	int FontSize, h, t;

	FontSize = intRound(m_RTLabelFontSizePrc * m_h);

	// left top
	{
		m_LTLabelColor = m_Printing ? BLACK : WHITE;

		if (m_TMLabel != "")
		{
			CMFont SmallFont(intRound(1.0 * FontSize), 400, "Arial");
			m_TMLabelBkColor = NOCOLOR;
			int h = intRound(1.0 * FontSize);
			::SetRect(&Rect, L, T + h, R, T + 2 * h);
			if (m_TMLabelColor == YELLOW && m_Printing)
				m_TMLabelColor = ORANGE;
			if (m_TMLabelColor == ORANGE && !m_Printing)
				m_TMLabelColor = YELLOW;
			m_MemDC.WriteText(m_TMLabel, Rect, SmallFont, m_TMLabelColor, 1, m_TMLabelBkColor);
		}

		if (m_SIMLTLabel[0] != "")
		{
			int LargeFontSize = intRound(m_w * 0.050);

			if (m_SIMLTLabel[3] == "iol sel")
			{
				LargeFontSize = intRound(1.3 * FontSize);
			}

			CMFont LargeFont(LargeFontSize, 400, "Arial");

			int h = intRound(0.900 * FontSize);
			int t = T;
			::SetRect(&Rect, L, t, R, t + h);
			m_MemDC.WriteText(m_SIMLTLabel[0], Rect, LargeFont, m_LTLabelColor, 0, m_LTLabelBkColor);
			m_MemDC.WriteText(m_SIMLTLabel[2], Rect, LargeFont, m_LTLabelColor, 2, m_LTLabelBkColor);

			m_MemDC.MeasureRect(m_SIMLTLabel[0] + "  ", Rect, LargeFont);
			int w = Rect.right - Rect.left;

			::SetRect(&Rect, L + w, t, R, t + h);
			m_MemDC.WriteText(m_SIMLTLabel[1], Rect, LargeFont, m_LTLabelColor, 0, m_LTLabelBkColor);
		}
		else
		{
			CMFont Font(FontSize, 400, "Arial");
			CMFont Font2(intRound(1.5 * FontSize), 400, "Arial");
			CMFont SmallFont(intRound(0.8 * FontSize), 400, "Arial");//530

			int h = intRound(0.900 * FontSize);

			if (m_LTBigFont) h = intRound(1.4 * FontSize);//530

			int t = T;
			if (m_CaptionVertical)
			{
				for (int i = 0; i < 5; i++)
				{
					if (!m_LTLabel[i].IsEmpty())
					{
						//
						if ((m_LTLabel[i].Find(_T("INTERNAL"), 0) != -1
							|| m_LTLabel[i].Find(_T("CORNEA"), 0) != -1
							|| m_LTLabel[i].Find(_T("TOTAL"), 0) != -1)
							&& m_LTLabel[i].Find(_T("Map"), 0) != -1)
						{
							::SetRect(&Rect, L, t, R, t + intRound(m_w * 0.050));
						}
						else  ::SetRect(&Rect, L, t, R, t + h);

						if (m_LTLabel[i] == "Presbia 2.4mm" && i == 2)
						{
							m_MemDC.WriteText(m_LTLabel[i], Rect, SmallFont, m_LTLabelColor, 0, m_LTLabelBkColor);
						}
						else
						{
							if (m_LTBigFont) m_MemDC.WriteText(m_LTLabel[i], Rect, Font2, m_LTLabelColor, 0, m_LTLabelBkColor);
							else
							{
								if (i == 0 && (m_LTLabel[i].Find(_T("INTERNAL"), 0) != -1 || m_LTLabel[i].Find(_T("CORNEA"), 0) != -1 || m_LTLabel[i].Find(_T("TOTAL"), 0) != -1))
								{
									int LargeFontSize = intRound(m_w * 0.050);

									if (m_SIMLTLabel[3] == "iol sel")
									{
										LargeFontSize = intRound(1.3 * FontSize);
									}
									CMFont LargeFont(LargeFontSize, 400, "Arial");
									m_MemDC.WriteText(m_LTLabel[i], Rect, LargeFont, m_LTLabelColor, 0, m_LTLabelBkColor);
								}

								else
								{
									m_MemDC.WriteText(m_LTLabel[i], Rect, Font, m_LTLabelColor, 0, m_LTLabelBkColor);
								}
							}
						}
					}

					if ((m_LTLabel[i].Find(_T("INTERNAL"), 0) != -1
						|| m_LTLabel[i].Find(_T("CORNEA"), 0) != -1
						|| m_LTLabel[i].Find(_T("TOTAL"), 0) != -1)
						&& m_LTLabel[i].Find(_T("Map"), 0) != -1)
					{
						t += intRound(m_w * 0.050);
					}
					else	t += h;
				}
			}
			else
			{
				s = m_LTLabel[0];

				if (!m_LTLabel[1].IsEmpty())
				{
					s += "   " + m_LTLabel[1];
				}

				if (!m_LTLabel[2].IsEmpty())
				{
					s += "   " + m_LTLabel[2];
				}

				::SetRect(&Rect, L, t, R, t + h);

				if (m_LTBigFont) m_MemDC.WriteText(s, Rect, Font2, m_LTLabelColor, 0, m_LTLabelBkColor);
				else            m_MemDC.WriteText(s, Rect, Font, m_LTLabelColor, 0, m_LTLabelBkColor);
			}
		}
	}

	FontSize = intRound(m_RTLabelFontSizePrc * m_h);

	// right top
	{
		m_RTLabelColor = m_Printing ? BLACK : WHITE;
		CMFont Font(FontSize, 400, "Arial");
		h = intRound(0.900 * FontSize);
		t = T;
		for (int i = 0; i < 2; i++)
		{
			if (!m_RTLabel[i].IsEmpty())
			{
				if (i == 0 && m_RTLabel[i] == "Dysfunctional Lens Index")
				{
					::SetRect(&Rect, L, t, R - 20, t + h);
					m_MemDC.WriteText(m_RTLabel[i], Rect, Font, m_RTLabelColor, 2, m_RTLabelBkColor);

					int SmallFontSize = intRound(FontSize * 0.6);
					CMFont SmallFont(SmallFontSize, 400, "Arial");
					::SetRect(&Rect, R - 18, t, R, t + intRound(0.5 * FontSize));
					m_MemDC.WriteText("TM", Rect, SmallFont, m_RTLabelColor, 0, m_RTLabelBkColor);
				}
				//6.2.0 NEAR VISION
				else if (i == 0 && (m_RTLabel[i] == "FAR" || m_RTLabel[i] == "NEAR"))
				{
					int BigFontSize = intRound(1.5 * FontSize);
					h = intRound(0.900 * BigFontSize);
					CMFont BigFont(BigFontSize, 400, "Arial");
					::SetRect(&Rect, L, t, R, t + h);
					m_MemDC.WriteText(m_RTLabel[i], Rect, BigFont, m_RTLabelColor, 2, m_RTLabelBkColor);
				}
				//6.2.0 NEAR VISION
				else
				{
					::SetRect(&Rect, L, t, R, t + h);
					m_MemDC.WriteText(m_RTLabel[i], Rect, Font, m_RTLabelColor, 2, m_RTLabelBkColor);
				}
			}
			t += h;
		}
	}

	// right bottom
	{
		if (::TempSettings.Temp_WFAngel)
			FontSize = intRound(0.5 *m_RTLabelFontSizePrc * m_h);
		else FontSize = intRound(0.9 *m_RTLabelFontSizePrc * m_h);

		if (m_RBLabel[0] == "Grade = " || m_RBLabel[0] == "Grade ")//
		{
			int FontSize1 = intRound(1.5 *m_RTLabelFontSizePrc * m_h);
			CMFont Font(FontSize1, 400, "Arial");
			h = intRound(0.900 * FontSize1);

			t = B - 2 * h;
			for (int i = 0; i < 2; i++)
			{
				::SetRect(&Rect, L, t, R, t + h);

				//if (m_Printing && m_RBLabelColor[i] == WHITE) m_RBLabelColor[i] = BLACK; //0826

				m_MemDC.WriteText(m_RBLabel[i], Rect, Font, WHITE, 2, m_RBLabelBkColor);//6.2.0
				t += h;
			}
		}
		// For opacity map Done
		else
		{
			CMFont Font(FontSize, 400, "Arial");
			h = intRound(0.900 * FontSize);

			t = B - 6 * h;
			for (int i = 0; i < 6; i++) {
				if (!m_RBLabel[i].IsEmpty()) {
					::SetRect(&Rect, L, t, R, t + h);

					if (m_Printing && m_RBLabelColor[i] == WHITE && m_isPresbia) m_RBLabelColor[i] = BLACK; //0826

					m_MemDC.WriteText(m_RBLabel[i], Rect, Font, m_RBLabelColor[i], 2, m_RBLabelBkColor);
				}
				t += h;
			}
		}
	}

	// Left bottom 520
	{
		FontSize = intRound(0.5 *m_RTLabelFontSizePrc * m_h);
		CMFont Font(FontSize, 400, "Arial");
		h = intRound(0.900 * FontSize);
		t = B - 11 * h;
		for (int i = 0; i < 11; i++) {
			if (!m_LBLabel[i].IsEmpty()) {
				::SetRect(&Rect, L, t, R, t + h);
				m_MemDC.WriteText(m_LBLabel[i], Rect, Font, m_LBLabelColor[i], 0, m_RBLabelBkColor);
			}
			t += h;
		}
	}
	//DONE
}

//***************************************************************************************
