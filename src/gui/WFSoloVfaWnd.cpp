// Solo Vfa display
//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "WFSoloVfaWnd.h"
#include "EyeWnd.h"
#include "PSFWnd.h"
#include "MTFWnd.h"
#include "EEFWnd.h"
#include "LtrWnd.h"
#include "RMSWnd.h"
#include "ChartWnd.h"
#include "BusyCursor.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CWFSoloVfaWnd, CSumWnd)

	ON_WM_TIMER() //for solo hyperlinker
	ON_WM_MOUSEMOVE()//for solo hyperlinker
	ON_WM_LBUTTONDOWN()//for solo hyperlinker

	ON_MESSAGE(WM_CHANGE_CENT, OnChangeCentMsg)
	ON_MESSAGE(WM_CHANGE_STEP, OnChangeStepMsg)

	ON_MESSAGE(WM_SLIDER_LCLICK, OnSliderClick)
	ON_MESSAGE(WM_SLIDER_POS_CHANGED, OnSliderChangedPos)

	ON_BN_CLICKED(IDC_SOLO_SUN_ITEM1, OnSunButton1Clicked)
	ON_BN_CLICKED(IDC_SOLO_MOON_ITEM1, OnMoonButton1Clicked)

	ON_BN_CLICKED(IDC_SOLO_SUN_ITEM2, OnSunButton2Clicked)
	ON_BN_CLICKED(IDC_SOLO_MOON_ITEM2, OnMoonButton2Clicked)

	ON_COMMAND(IDC_DEFAULT_GUI_ITEM, OnShowDefaultItemClicked)

	ON_BN_CLICKED(IDC_SHOW_FULL_INFO, OnShowWFTextInfoBtnClicked)
	ON_BN_CLICKED(IDC_SHOW_FULL_INFO2, OnShowWFTextInfoBtn2Clicked)

END_MESSAGE_MAP()

//***************************************************************************************

CWFSoloVfaWnd::CWFSoloVfaWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CWFExam* pWFExam1, CWFExam* pWFExam2, CWndSettings* pWndSettings) :
	CSumWnd(pWnd, WndRect, pPatient, pWndSettings)
{
	CBusyCursor Cursor;

	//----------------------------------------------------
	RECT Rect;
	GetWindowRect(&Rect);
	real_t t = 3.0 * (0.03 * m_h) + m_g;
	real_t w2 = ((Rect.right - Rect.left) - 5.0 * m_g) / 6.0;
	real_t h2 = ((Rect.bottom - Rect.top - t) - 4.0 * m_g) / 4.0;
	real_t w = w2 + w2;
	real_t h = h2 + m_g + h2;
	real_t w3 = w*0.85;
	real_t w4 = w*0.15;

	m_Top = intRound(t);
	m_Bootom = intRound(t + h + m_g + h);
	m_MideLine = intRound(m_w / 2);

	::SetRect(&m_Rect[0], intRound(m_g), intRound(t), intRound(m_g + w3), intRound(t + h)); // exam 1 map
	::SetRect(&m_Rect[1], intRound(m_g + w3 + m_g), intRound(t), intRound(m_g + w3 + m_g + w2), intRound(t + h2)); // exam 1 psf
	::SetRect(&m_Rect[2], intRound(m_g + w3 + m_g), intRound(t + h2 + m_g), intRound(m_g + w3 + m_g + w2), intRound(t + h)); // exam 1 letter

	::SetRect(&m_Rect[3], intRound(m_g + w + m_g + w2 + m_g), intRound(t), intRound(m_g + w + m_g + w2 + m_g + w3), intRound(t + h)); // exam 2 map

	::SetRect(&m_Rect[4], intRound(m_g + w + m_g + w2 + m_g + w3 + m_g), intRound(t), intRound(m_g + w + m_g + w2 + m_g + w3 + m_g + w2), intRound(t + h2)); // exam 2 psf
	::SetRect(&m_Rect[5], intRound(m_g + w + m_g + w2 + m_g + w3 + m_g), intRound(t + h2 + m_g), intRound(m_g + w + m_g + w2 + m_g + w3 + m_g + w2), intRound(t + h)); // exam 2 letter

	::SetRect(&m_Rect[6], intRound(m_g), intRound(t + h + m_g), intRound(m_g + w), intRound(t + h + m_g + h)); // exam 1 info
	::SetRect(&m_Rect[7], intRound(m_g + w + m_g + w2 + m_g), intRound(t + h + m_g), intRound(m_g + w + m_g + w2 + m_g + w), intRound(t + h + m_g + h)); // exam 2 info
	::SetRect(&m_Rect[8], intRound(m_g + w + m_g), intRound(t + h + m_g), intRound(m_g + w + m_g + w2), intRound(t + h + m_g + h)); // exam 1 complaints
	::SetRect(&m_Rect[9], intRound(m_g + w + m_g + w2 + m_g + w + m_g), intRound(t + h + m_g), intRound(m_g + w + m_g + w2 + m_g + w + m_g + w2), intRound(t + h + m_g + h)); // exam 2 complaints

	::SetRect(&m_Rect[10], intRound(m_g + w3 + m_g + w2), intRound(t), intRound(m_g + w + m_g + w2), intRound(t + h)); // OD slider rect
	::SetRect(&m_Rect[11], intRound(m_g + w + m_g + w2 + m_g + w3 + m_g + w2), intRound(t), intRound(m_g + w + m_g + w2 + m_g + w + m_g + w2), intRound(t + h)); // OS slider rect
	//----------------------------------------------------

	m_pWFExam1 = NULL;
	m_pWFExam2 = NULL;
	if (pWFExam1) {
		if (pWFExam1->m_Header.IsOD()) m_pWFExam1 = pWFExam1;
		else m_pWFExam2 = pWFExam1;
	}
	if (pWFExam2) {
		if (pWFExam2->m_Header.IsOD()) m_pWFExam1 = pWFExam2;
		else m_pWFExam2 = pWFExam2;
	}

	//record the increase and decrease range
	m_VFASumWnd = TRUE;//520

	int invalidValue = 0;
	real_t m_r1, m_r2;

	if (m_pWFExam1 == NULL) invalidValue = 1;
	else m_r1 = m_pWFExam1->m_WfSurface.m_r_max_um;

	if (m_pWFExam2 == NULL) invalidValue = (invalidValue == 0 ? 2 : 3);
	else m_r2 = m_pWFExam2->m_WfSurface.m_r_max_um;

	switch (invalidValue)
	{
	case 0:
		::TempSettings.Com_r_max_um = (m_r1< m_r2 ? m_r1 : m_r2);
		m_maxRum = (m_r1 > m_r2 ? m_r1 : m_r2);
		break;
	case 1:
		::TempSettings.Com_r_max_um = m_r2;
		m_maxRum = m_r2;
		break;
	case 2:
		::TempSettings.Com_r_max_um = m_r1;
		m_maxRum = m_r1;
		break;
	case 3:
		::TempSettings.Com_r_max_um = 5000; break;
	}


	if (m_pWFExam1 != NULL)
	{
		::TempSettings.Com_r_max_um1 = m_r1;
		m_scan1.Format(_T("%.2f mm"), m_r1*0.002);
	}
	else
	{
		::TempSettings.Com_r_max_um1 = 0;
	}

	if (m_pWFExam2 != NULL)
	{
		::TempSettings.Com_r_max_um2 = m_r2;
		m_scan2.Format(_T("%.2f mm"), m_r2*0.002);
	}
	else ::TempSettings.Com_r_max_um2 = 0;


	//Sliders and some dispaly windows controls
	int  Slider_Left1 = intRound(m_g + w3 + m_g + w2 + 0.25*w4);
	int  Slider_Right1 = intRound(m_g + w3 + m_g + w2 + 0.75*w4);
	int  Slider_Left2 = intRound(m_g + w + m_g + w2 + m_g + w3 + m_g + w2 + 0.25*w4);
	int  Slider_Right2 = intRound(m_g + w + m_g + w2 + m_g + w3 + m_g + w2 + 0.75*w4);

	m_Slider_Top = intRound(t + 0.2*h);
	int  Slider_Bot = intRound(t + 0.8*h);
	int  IconWide = 9;

	::SetRect(&m_SliderRects[0], Slider_Left1, m_Slider_Top, Slider_Right1, Slider_Bot);
	::SetRect(&m_SliderRects[1], Slider_Left2, m_Slider_Top, Slider_Right2, Slider_Bot);

	::SetRect(&m_SliderRects[2], Slider_Left1, intRound(m_Slider_Top - 2.5*IconWide), Slider_Right1, intRound(m_Slider_Top - 0.5*IconWide));
	::SetRect(&m_SliderRects[3], Slider_Left1, intRound(Slider_Bot + 0.8*IconWide), Slider_Right1, intRound(Slider_Bot + 2.8*IconWide));

	::SetRect(&m_SliderRects[4], Slider_Left2, intRound(m_Slider_Top - 2.5*IconWide), Slider_Right2, intRound(m_Slider_Top - 0.5*IconWide));
	::SetRect(&m_SliderRects[5], Slider_Left2, intRound(Slider_Bot + 0.8*IconWide), Slider_Right2, intRound(Slider_Bot + 2.8*IconWide));

	::SetRect(&m_SliderRects[6], m_Rect[10].left, intRound(m_Slider_Top - 5.5*IconWide), m_Rect[10].right, intRound(m_Slider_Top - 3 * IconWide));
	::SetRect(&m_SliderRects[7], m_Rect[11].left, intRound(m_Slider_Top - 5.5*IconWide), m_Rect[11].right, intRound(m_Slider_Top - 3 * IconWide));


	if (m_pWFExam1)
	{
		m_ODSlider.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_SliderRects[0], this, IDC_SOLO_OD_SLIDER);

		if (::TempSettings.Com_r_max_um1 < m_maxRum)
		{
			m_ODSlider.m_Pos = 1 - (m_maxRum - ::TempSettings.Com_r_max_um1) / (::TempSettings.Com_r_max_um2 - 500);
		}
		else m_ODSlider.m_Pos = 1;

		m_ODSliderMaxPos = m_ODSlider.m_Pos;
		m_ODSlider.OnPaint();

		m_SunButton1.Create(_T(""), _T(""), 0, m_SliderRects[2], this, IDC_SOLO_SUN_ITEM1);
		m_SunButton1.SetImage(IDR_SOLO_MOON);
		m_SunButton1.SetBk(FALSE);
		m_SunButton1.SetBkColor(BLACK);
		m_SunButton1.ShowWindow(SW_SHOW);

		m_MoonButton1.Create(_T(""), _T(""), 0, m_SliderRects[3], this, IDC_SOLO_MOON_ITEM1);
		m_MoonButton1.SetImage(IDR_SOLO_SUN);
		m_MoonButton1.SetBk(FALSE);
		m_MoonButton1.SetBkColor(BLACK);
		m_MoonButton1.ShowWindow(SW_SHOW);
	}

	if (m_pWFExam2)
	{
		m_OSSlider.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_SliderRects[1], this, IDC_SOLO_OS_SLIDER);
		if (::TempSettings.Com_r_max_um2 < m_maxRum)
		{
			m_OSSlider.m_Pos = 1 - (m_maxRum - ::TempSettings.Com_r_max_um2) / (::TempSettings.Com_r_max_um1 - 500);
		}
		else m_OSSlider.m_Pos = 1;

		m_OSSliderMaxPos = m_OSSlider.m_Pos;
		m_OSSlider.OnPaint();

		m_SunButton2.Create(_T(""), _T(""), 0, m_SliderRects[4], this, IDC_SOLO_SUN_ITEM2);
		m_SunButton2.SetImage(IDR_SOLO_MOON);
		m_SunButton2.SetBk(FALSE);
		m_SunButton2.SetBkColor(BLACK);
		m_SunButton2.ShowWindow(SW_SHOW);

		m_MoonButton2.Create(_T(""), _T(""), 0, m_SliderRects[5], this, IDC_SOLO_MOON_ITEM2);
		m_MoonButton2.SetImage(IDR_SOLO_SUN);
		m_MoonButton2.SetBk(FALSE);
		m_MoonButton2.SetBkColor(BLACK);
		m_MoonButton2.ShowWindow(SW_SHOW);
	}
	//Done

	CreateChildWnd();

	int  SNum = (int)(m_maxRum*0.002);
	real_t rest = m_maxRum - intRound(m_maxRum) % 500;
	real_t pos = 1 - (m_maxRum - rest) / (m_maxRum - 500);
	if (m_pWFExam1)
	{
		m_ODSlider.m_scaleNum = SNum;
		m_ODSlider.m_FirstPos = pos;
		m_ODSlider.OnPaint();
	}

	if (m_pWFExam2)
	{
		m_OSSlider.m_scaleNum = SNum;
		m_OSSlider.m_FirstPos = pos;
		m_OSSlider.OnPaint();
	}

	Repaint();

	//530 Draw the WF Text Info button
	if (m_pWFExam1)
	{
		CString s = (m_pWndSettings[0].m_SimplifiedWFPanel ? "Show Details" : "Hide Details");
		::SetRect(&Rect, m_Rect[6].right - 25, m_Rect[6].top + 5, m_Rect[6].right - 5, m_Rect[6].top + 20);
		m_WFShowFullInfoButton.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_SHOW_FULL_INFO);
		m_WFShowFullInfoButton.SetImage(m_pWndSettings[0].m_SimplifiedWFPanel ?
			(m_SW == 1920 ? IDR_SHOW_DETAIL_3 : m_SW == 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
			(m_SW == 1920 ? IDR_HIDE_DETAIL_3 : m_SW == 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
		m_WFShowFullInfoButton.SetBk(FALSE);
		m_WFShowFullInfoButton.SetBkColor(BLACK);
		m_WFShowFullInfoButton.SetTip(s);
		m_WFShowFullInfoButton.ShowWindow(SW_SHOW);
	}

	if (m_pWFExam2)
	{
		CString s = (m_pWndSettings[0].m_SimplifiedWFPanel ? "Show Details" : "Hide Details");
		::SetRect(&Rect, m_Rect[7].right - 25, m_Rect[7].top + 5, m_Rect[7].right - 5, m_Rect[7].top + 20);
		m_WFShowFullInfoButton2.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_SHOW_FULL_INFO2);
		m_WFShowFullInfoButton2.SetImage(m_pWndSettings[0].m_SimplifiedWFPanel ?
			(m_SW == 1920 ? IDR_SHOW_DETAIL_3 : m_SW == 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
			(m_SW == 1920 ? IDR_HIDE_DETAIL_3 : m_SW == 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
		m_WFShowFullInfoButton2.SetBk(FALSE);
		m_WFShowFullInfoButton2.SetBkColor(BLACK);
		m_WFShowFullInfoButton2.SetTip(s);
		m_WFShowFullInfoButton2.ShowWindow(SW_SHOW);
	}
	//Done

	ShowWindow(SW_SHOW);

	// The 'transform funtionality' info ("Link to aberration analysis" or"Link to refraction analysis" )
	m_HighLight1 = 0;
	m_HighLight2 = 0;

	RECT InfoRect;
	::SetRect(&InfoRect, 0, 0, 160, 20);
	m_NoteInfo.CreateWnd(InfoRect, this);
	m_NoteInfo.ShowWindow(FALSE);
	//Done  
}

//***************************************************************************************

void CWFSoloVfaWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default 
	switch (nIDEvent)
	{
	case SOLOTIMER:
	{
		m_NoteInfo.ShowWindow(FALSE);
		KillTimer(SOLOTIMER);
		break;
	}
	}
}

//***************************************************************************************
void CWFSoloVfaWnd::RepaintMemDC()
{
	CSumWnd::RepaintMemDC();

	BOOL Rows[22] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	BOOL HighLight1, HighLight2;
	CMFont FontScanSize(intRound(1.8*m_g), 400, "Arial");

	if (m_pWFExam1)
	{
		m_MemDC.DrawRectangle(m_Rect[10], gray, NOCOLOR);
		m_MemDC.WriteText(m_scan1, m_SliderRects[6], FontScanSize, RED, 1, NOCOLOR);

		HighLight1 = TRUE ? m_HighLight1 == 1 : FALSE;
		HighLight2 = TRUE ? m_HighLight2 == 1 : FALSE;

		if (m_pWndSettings[0].m_SimplifiedWFPanel == 1)
			ChangWFExamInfo(m_pWFExam1, m_Rect[6], 1, TRUE);
		else SoloVfaExamInfo(m_pWFExam1, m_Rect[6], 22, Rows, HighLight1);

		SoloVisualComplaints(m_pWFExam1, m_Rect[8], 22, HighLight2);
	}

	if (m_pWFExam2)
	{
		m_MemDC.DrawRectangle(m_Rect[11], gray, NOCOLOR);
		m_MemDC.WriteText(m_scan2, m_SliderRects[7], FontScanSize, RED, 1, NOCOLOR);

		HighLight1 = TRUE ? m_HighLight1 == 2 : FALSE;
		HighLight2 = TRUE ? m_HighLight2 == 2 : FALSE;

		if (m_pWndSettings[0].m_SimplifiedWFPanel == 1)
			ChangWFExamInfo(m_pWFExam2, m_Rect[7], 1, TRUE);
		else SoloVfaExamInfo(m_pWFExam2, m_Rect[7], 22, Rows, HighLight1);

		SoloVisualComplaints(m_pWFExam2, m_Rect[9], 22, HighLight2);
	}

	if (m_pWFExam1 && m_pWFExam2)
	{
		int mideLine = intRound(m_w / (real_t)2);

		m_MemDC.DrawLine(mideLine, m_Top, mideLine, m_Bootom, 2, white);
	}
}

//***************************************************************************************

void CWFSoloVfaWnd::CreateChildWnd()
{
	for (int d = 0; d < 6; d++) SAFE_DELETE(m_pDispWnd[d]);

	CWndSettings* pWndSettings = GetWndSettings();
	int newMaskType = pWndSettings->m_Mask.GetType();
	m_pWndSettings[m_d].m_Mask.SetType(newMaskType);

	{
		// mask
		int MaskType = m_pWndSettings[0].m_Mask.GetType();
		if (MaskType != MASK_TOTAL && MaskType != MASK_TOTAL_NO_DEFOCUS && MaskType != MASK_LO_TOTAL && MaskType != MASK_HO_TOTAL) {
			m_pWndSettings[0].m_Mask.SetType(MASK_TOTAL);
			m_pWndSettings[0].m_Piston = TRUE;
			m_pWndSettings[0].m_Tilt = TRUE;
			m_pWndSettings[1].m_Mask.SetType(MASK_TOTAL);
			m_pWndSettings[1].m_Piston = TRUE;
			m_pWndSettings[1].m_Tilt = TRUE;
			m_pWndSettings[2].m_Mask.SetType(MASK_TOTAL);
			m_pWndSettings[2].m_Piston = TRUE;
			m_pWndSettings[2].m_Tilt = TRUE;
		}
		// colors
		if (!m_pWndSettings[0].AreDefaultColors()) {
			m_pWndSettings[0].SetDefaultColors(1);
		}
	}

	// type
	if (m_pWndSettings[0].m_Type != TYPE_TWFM) m_pWndSettings[0].m_Type = TYPE_TRFM;
	m_pWndSettings[1].m_Type = TYPE_TPSF;
	m_pWndSettings[2].m_Type = TYPE_TLTR;

	{
		CWndSettings *pWndSettings = &m_pWndSettings[2];
		if (!(pWndSettings->m_Ltr == 0 || pWndSettings->m_Ltr == 1)) {
			pWndSettings->m_Ltr = 0;
		}
		if (!(pWndSettings->m_LtrLine == 20 || pWndSettings->m_LtrLine == 25 || pWndSettings->m_LtrLine == 30 ||
			pWndSettings->m_LtrLine == 40 || pWndSettings->m_LtrLine == 50 || pWndSettings->m_LtrLine == 70 ||
			pWndSettings->m_LtrLine == 100 || pWndSettings->m_LtrLine == 200)) {
			pWndSettings->m_LtrLine = 40;
		}
		if (!(pWndSettings->m_LtrOrientation == 0 || pWndSettings->m_LtrOrientation == 90 ||
			pWndSettings->m_LtrOrientation == 180 || pWndSettings->m_LtrOrientation == 270)) {
			pWndSettings->m_LtrOrientation = 0;
		}
	}

	//new
	int  Greater;
	int  r_min_max;

	if (m_pWFExam1 && m_pWFExam2)
	{
		if (m_pWFExam1->m_WfSurface.m_r_max_um > m_pWFExam2->m_WfSurface.m_r_max_um)
		{
			Greater = 1;
			r_min_max = intRound(m_pWFExam2->m_WfSurface.m_r_max_um);
		}
		else if (m_pWFExam1->m_WfSurface.m_r_max_um < m_pWFExam2->m_WfSurface.m_r_max_um)
		{
			Greater = 2;
			r_min_max = intRound(m_pWFExam1->m_WfSurface.m_r_max_um);
		}
		else
		{
			Greater = 0;
			r_min_max = intRound(m_pWFExam1->m_WfSurface.m_r_max_um);
		}
	}
	else if (m_pWFExam1 && !m_pWFExam2)
	{
		Greater = 0;
		r_min_max = intRound(m_pWFExam1->m_WfSurface.m_r_max_um);
	}
	else if (m_pWFExam2 && !m_pWFExam1)
	{
		Greater = 0;
		r_min_max = intRound(m_pWFExam2->m_WfSurface.m_r_max_um);
	}
	//Done

	if (m_Togther != TRUE && m_Togther != FALSE)
	{
		if (Greater == 2) m_d = 3;
		else m_d = 0;
		for (int d = 0; d < 3; d++)
		{
			if (m_pWFExam1) m_pWndSettings[d].m_ZoneRadiusUm = intRound(m_pWFExam1->m_WfSurface.m_r_max_um);
		}
		for (int d = 3; d < 6; d++)
		{
			if (m_pWFExam2) m_pWndSettings[d].m_ZoneRadiusUm = intRound(m_pWFExam2->m_WfSurface.m_r_max_um);
		}
	}

	m_Togther = TRUE;// Increase or decrease togther

	if (Greater != 0)
	{
		if (m_pWndSettings[3].m_ZoneRadiusUm != 0)
		{
			if (m_pWndSettings[m_d].m_ZoneRadiusUm < r_min_max)
				m_Togther = TRUE;
			else
			{
				m_Togther = FALSE;

				for (int d = 0; d < 3; d++)
				{
					// size
					m_pWndSettings[d].m_SizeSmall = TRUE;
					// method
					m_pWndSettings[d].m_Method3D = FALSE;
					if (d != m_d)
					{
						// zone
						/*if(Greater == 1)
						{
						m_pWndSettings[d].m_ZoneRadiusUm = m_pWndSettings[m_d].m_ZoneRadiusUm;
						}*/

						// mask
						m_pWndSettings[d].m_Mask = m_pWndSettings[m_d].m_Mask;
						m_pWndSettings[d].m_Piston = m_pWndSettings[m_d].m_Piston;
						m_pWndSettings[d].m_Tilt = m_pWndSettings[m_d].m_Tilt;
					}
				}

				for (int d = 3; d < 6; d++)
				{
					// size
					m_pWndSettings[d].m_SizeSmall = TRUE;
					// method
					m_pWndSettings[d].m_Method3D = FALSE;
					if (d != m_d)
					{
						// zone
						/*if(Greater == 2)
						{
						if(m_Togther) m_pWndSettings[d].m_ZoneRadiusUm = m_pWndSettings[m_d].m_ZoneRadiusUm;
						}*/

						// mask
						m_pWndSettings[d].m_Mask = m_pWndSettings[m_d].m_Mask;
						m_pWndSettings[d].m_Piston = m_pWndSettings[m_d].m_Piston;
						m_pWndSettings[d].m_Tilt = m_pWndSettings[m_d].m_Tilt;
					}
				}
			}
		}

		else
		{
			for (int d = 0; d < 6; d++)
			{
				m_pWndSettings[d].m_ZoneRadiusUm = r_min_max;

				// size
				m_pWndSettings[d].m_SizeSmall = TRUE;
				// method
				m_pWndSettings[d].m_Method3D = FALSE;

				if (d != 0)
				{
					m_pWndSettings[d].m_Mask = m_pWndSettings[0].m_Mask;
					m_pWndSettings[d].m_Piston = m_pWndSettings[0].m_Piston;
					m_pWndSettings[d].m_Tilt = m_pWndSettings[0].m_Tilt;
				}
			}
		}
	}


	if (m_Togther)
	{
		//int k = m_d % 3;

		for (int d = 0; d < 6; d++)
		{
			// size
			m_pWndSettings[d].m_SizeSmall = TRUE;
			// method
			m_pWndSettings[d].m_Method3D = FALSE;
			if (d != m_d) {
				// zone
				m_pWndSettings[d].m_ZoneRadiusUm = m_pWndSettings[m_d].m_ZoneRadiusUm;

				// mask
				m_pWndSettings[d].m_Mask = m_pWndSettings[m_d].m_Mask;
				m_pWndSettings[d].m_Piston = m_pWndSettings[m_d].m_Piston;
				m_pWndSettings[d].m_Tilt = m_pWndSettings[m_d].m_Tilt;
			}
		}
	}


	real_t r_max_um = 5000;
	real_t r_max_um1, r_max_um2;

	if (m_pWFExam1) r_max_um1 = m_pWFExam1->m_WfSurface.m_r_max_um;
	if (m_pWFExam2) r_max_um2 = m_pWFExam2->m_WfSurface.m_r_max_um;

	if (m_pWFExam1) r_max_um1 = __min(r_max_um1, m_pWndSettings[0].m_ZoneRadiusUm);
	if (m_pWFExam2) r_max_um2 = __min(r_max_um2, m_pWndSettings[3].m_ZoneRadiusUm);

	if (m_pWFExam1) r_max_um = __min(r_max_um, m_pWFExam1->m_WfSurface.m_r_max_um);
	if (m_pWFExam2) r_max_um = __min(r_max_um, m_pWFExam2->m_WfSurface.m_r_max_um);
	r_max_um = __min(r_max_um, m_pWndSettings[0].m_ZoneRadiusUm);

	for (int e = 0; e < 2; e++) {

		CWFExam* pWFExam = e == 0 ? m_pWFExam1 : m_pWFExam2;

		if (pWFExam == NULL) continue;

		CZernikeSurface Surface;
		Surface = pWFExam->m_WfSurface;

		//New
		if (m_Togther)
			Surface.ChangeRMaxUm(r_max_um);
		else
		{
			if (e == 0) Surface.ChangeRMaxUm(r_max_um1);
			else     Surface.ChangeRMaxUm(r_max_um2);
		}
		//Done

		//if(e==0) Surface.ChangeRMaxUm(r_max_um);
		//else     Surface.ChangeRMaxUm(r_max_um);

		Surface &= m_pWndSettings[0].m_Mask;
		Surface.ComputePiston(m_pWndSettings[0].m_Piston);
		Surface.ComputeTilt(m_pWndSettings[0].m_Tilt);

		for (int d = e * 3; d < e * 3 + 3; d++) {

			m_Surfaces[d] = Surface;

			CWndSettings* pWndSettings = m_pWndSettings + (d % 3);

			switch (pWndSettings->m_Type) {

			case TYPE_TWFM:
			{
				CEyeWnd* pEyeWnd = new CEyeWnd(m_Rect[d], this);
				pEyeWnd->m_Type = TYPE_TWFM;
				pEyeWnd->m_SizeSmall = TRUE;
				pEyeWnd->m_Method3D = FALSE;
				pEyeWnd->m_LTLabel[0] = "Wavefront Map";
				pWndSettings->m_Mask.GetName(pEyeWnd->m_LTLabel[1]);

				//New
				if (m_Togther)
					pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
				else
				{
					if (e == 0) pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um1 * 0.002);
					else     pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um2 * 0.002);
				}
				//Done

				//if(e==0) pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);//aaa
				//else     pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);//aaa

				pEyeWnd->m_MapShowMap = TRUE;
				pEyeWnd->m_MapShowNumbers = FALSE;
				pEyeWnd->m_MapShowEye = FALSE;
				pEyeWnd->m_MapShowPupil = FALSE;
				pEyeWnd->m_MapShowLimbus = FALSE;
				pEyeWnd->m_MapShowPoints = FALSE;
				pEyeWnd->m_MapTranslucent = FALSE;
				pEyeWnd->m_Unit = "µ";
				pEyeWnd->m_Inc = pWndSettings->GetIncrement();
				CScale* pScale = pWndSettings->GetScale();
				pEyeWnd->m_NumColors = pScale->m_NumColors;
				for (int i = 0; i < pScale->m_NumColors; i++) {
					pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
				}
				pEyeWnd->m_pWFExam = pWFExam;
				CWFExam::CreateWfm2D(pEyeWnd->m_Map2D, Surface, 1.0 / pEyeWnd->m_y_px_um);
				pEyeWnd->CreateCentButtons();
				pEyeWnd->CreateZoomButtons();
				pEyeWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 0);
				//pEyeWnd->CreateZoneButtons();
				m_pDispWnd[d] = pEyeWnd;
			}
			break;

			case TYPE_TRFM:
			{
				CEyeWnd* pEyeWnd = new CEyeWnd(m_Rect[d], this);
				pEyeWnd->m_Type = TYPE_TRFM;
				pEyeWnd->m_SizeSmall = TRUE;
				pEyeWnd->m_Method3D = FALSE;
				pEyeWnd->m_LTLabel[0] = "Refraction Map";
				pWndSettings->m_Mask.GetName(pEyeWnd->m_LTLabel[1]);

				//New
				if (m_Togther)
					pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
				else
				{
					if (e == 0) pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um1 * 0.002);
					else     pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um2 * 0.002);
				}
				//Done

				//      if(e==0) pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
				//else     pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);

				pEyeWnd->m_MapShowMap = TRUE;
				pEyeWnd->m_MapShowNumbers = FALSE;
				pEyeWnd->m_MapShowEye = FALSE;
				pEyeWnd->m_MapShowPupil = FALSE;
				pEyeWnd->m_MapShowLimbus = FALSE;
				pEyeWnd->m_MapShowPoints = FALSE;
				pEyeWnd->m_MapTranslucent = FALSE;
				pEyeWnd->m_Unit = "D";
				pEyeWnd->m_Inc = pWndSettings->GetIncrement();
				CScale* pScale = pWndSettings->GetScale();
				pEyeWnd->m_NumColors = pScale->m_NumColors;
				for (int i = 0; i < pScale->m_NumColors; i++) {
					pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
				}
				pEyeWnd->m_pWFExam = pWFExam;
				CWFExam::CreateRfm2D(pEyeWnd->m_Map2D, Surface, 1.0 / pEyeWnd->m_y_px_um);
				pEyeWnd->CreateCentButtons();
				pEyeWnd->CreateZoomButtons();
				pEyeWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 0);
				//pEyeWnd->CreateZoneButtons();
				m_pDispWnd[d] = pEyeWnd;
			}
			break;

			case TYPE_TPSF:
			{
				CPSFWnd* pPSFWnd = new CPSFWnd(m_Rect[d], this);
				CWFExam::CreatePsf(Surface, pPSFWnd->m_Psf, pWndSettings->m_EEFType);
				pPSFWnd->m_Type = TYPE_TPSF;
				pPSFWnd->m_SizeSmall = TRUE;
				pPSFWnd->m_Method3D = FALSE;
				pPSFWnd->m_LTLabel[0] = "Point Spread Function";
				pWndSettings->m_Mask.GetName(pPSFWnd->m_LTLabel[1]);

				//New
				if (m_Togther)
					pPSFWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
				else
				{
					if (e == 0) pPSFWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um1 * 0.002);
					else     pPSFWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um2 * 0.002);
				}
				//Done

				//      if(e == 0) pPSFWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
				//else       pPSFWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
				pPSFWnd->m_ShowEEF = FALSE;
				pPSFWnd->CreateZoomButtons();
				//pPSFWnd->CreateZoneButtons();
				pPSFWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 0);
				m_pDispWnd[d] = pPSFWnd;
			}
			break;

			case TYPE_TLTR:
			{
				CLtrWnd* pLtrWnd = new CLtrWnd(m_Rect[d], this);
				pLtrWnd->m_DysVal = -1;//531
				CWFExam::CreateLtr(Surface, pLtrWnd->m_Ltr, pWndSettings->m_Ltr, pWndSettings->m_LtrLine, pWndSettings->m_LtrOrientation);
				pLtrWnd->m_Type = TYPE_TLTR;
				pLtrWnd->m_SizeSmall = TRUE;
				pLtrWnd->m_Method3D = FALSE;
				//pLtrWnd->m_LTLabel[0].Format(_T("Letter 20 / %i", pWndSettings->m_LtrLine);
				pLtrWnd->m_LTLabel[0] = "Letter";//531
				pWndSettings->m_Mask.GetName(pLtrWnd->m_LTLabel[1]);

				//New
				if (m_Togther)
					pLtrWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
				else
				{
					if (e == 0) pLtrWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um1 * 0.002);
					else     pLtrWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um2 * 0.002);
				}
				//Done

				pLtrWnd->CreateZoomButtons();
				//pLtrWnd->CreateZoneButtons();
				pLtrWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 0);
				m_pDispWnd[d] = pLtrWnd;
			}
			break;

			}

		}
	}

	{
		CWndSettings* pWndSettings = m_pWndSettings;
		CScale* pScale = pWndSettings->GetScale();
		real_t Inc = pWndSettings->GetIncrement();
		real_t Cent = 0.0;
		real_t Step;
		if (pScale->m_StepAuto) {
			real_t Span = 0.0;
			for (int e = 0; e < 2; e++) {
				CEyeWnd* pEyeWnd = (CEyeWnd*)m_pDispWnd[e * 3];
				if (pEyeWnd) {
					real_t Min = pWndSettings->m_Method3D ? pEyeWnd->m_Map3D.m_min : pEyeWnd->m_Map2D.m_min;
					real_t Max = pWndSettings->m_Method3D ? pEyeWnd->m_Map3D.m_max : pEyeWnd->m_Map2D.m_max;
					Span = __max(Span, 2.0 * __max(fabs(Min - Cent), fabs(Max - Cent)));
				}
			}
			real_t t = Span / pScale->m_NumColors;
			Step = (int)(t / Inc) * Inc;
			if (t > Step + 0.001) Step += Inc;
		}
		else {
			Step = RealRound(pScale->m_Step, Inc);
		}
		if (Step < Inc) Step = Inc;
		real_t StepMax = pWndSettings->GetMaxStep();
		if (Step > StepMax) Step = StepMax;
		//
		for (int e = 0; e < 2; e++) {
			CEyeWnd* pEyeWnd = (CEyeWnd*)m_pDispWnd[e * 3];
			if (pEyeWnd) {
				pEyeWnd->m_Cent = Cent;
				pEyeWnd->m_Step = Step;
			}
		}
	}
}

//***************************************************************************************

void CWFSoloVfaWnd::CreateMenu()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_d == 0 || m_d == 3) {
		m_pMenu->AppendItem(IDC_TYPE_TWFM_ITEM, pWndSettings->m_Type == TYPE_TWFM, FALSE, "Wavefront Map", "");
		m_pMenu->AppendItem(IDC_TYPE_TRFM_ITEM, pWndSettings->m_Type == TYPE_TRFM, FALSE, "Refraction Map", "");
		m_pMenu->AppendSeparator();
	}

	//CreateSubmenuZone();
	CreateSubmenuMask();

	if (m_d == 0 || m_d == 3) {

		m_pMenu->AppendSeparator();

		CreateSubmenuMapSteps();
		CreateSubmenuMapColors();
	}

	else if (m_d == 2 || m_d == 5) {

		m_pMenu->AppendSeparator();

		CreateSubmenuLtr();
	}

	if (pWndSettings->m_Type == TYPE_TWFM ||
		pWndSettings->m_Type == TYPE_TRFM ||
		pWndSettings->m_Type == TYPE_TLTR ||
		pWndSettings->m_Type == TYPE_TRMS)
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_SAVE_TXT_ITEM, FALSE, FALSE, "Save Zernike coefficients into file", "");
	}

	
	if (!SameSoloVfa())
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
	}
	
}

//***************************************************************************************

LRESULT CWFSoloVfaWnd::OnChangeCentMsg(WPARAM wParam, LPARAM lParam)
{
	if (!m_pDispWnd[0] || !m_pDispWnd[3]) return 0;

	if (wParam == (WPARAM)m_pDispWnd[0]) {
		((CEyeWnd*)m_pDispWnd[3])->m_Cent = ((CEyeWnd*)m_pDispWnd[0])->m_Cent;
		m_pDispWnd[3]->Repaint();
	}
	else if (wParam == (WPARAM)m_pDispWnd[3]) {
		((CEyeWnd*)m_pDispWnd[0])->m_Cent = ((CEyeWnd*)m_pDispWnd[3])->m_Cent;
		m_pDispWnd[0]->Repaint();
	}

	return 0;
}

//***************************************************************************************

LRESULT CWFSoloVfaWnd::OnChangeStepMsg(WPARAM wParam, LPARAM lParam)
{
	if (!m_pDispWnd[0] || !m_pDispWnd[3]) return 0;

	if (wParam == (WPARAM)m_pDispWnd[0]) {
		((CEyeWnd*)m_pDispWnd[3])->m_Step = ((CEyeWnd*)m_pDispWnd[0])->m_Step;
		m_pDispWnd[3]->Repaint();
	}
	else if (wParam == (WPARAM)m_pDispWnd[3]) {
		((CEyeWnd*)m_pDispWnd[0])->m_Step = ((CEyeWnd*)m_pDispWnd[3])->m_Step;
		m_pDispWnd[0]->Repaint();
	}

	return 0;
}

//***************************************************************************************
// solo hyperlinker
void CWFSoloVfaWnd::OnMouseMove(uint nFlags, CPoint Point)
{
	CPoint point;
	GetCursorPos(&point);

	CMFont Font(15, 400, "Arial");
	int Infox, Infoy;
	RECT InfoRect;
	::SetRect(&InfoRect, 0, 0, 160, 20);

	if (point.x > m_Rect[6].left && point.x < m_Rect[6].right
		&& point.y > m_Rect[6].top && point.y < m_Rect[6].bottom)
	{
		::SetCursor(::LoadCursor(NULL, IDC_HAND));
		Infox = point.x + 20;
		Infoy = point.y - 20;
		m_NoteInfo.MoveWindow(Infox, Infoy, 160, 20, 1);

		if (m_HighLight1 != 1)
		{
			m_NoteInfo.ShowWindow(TRUE);
			m_NoteInfo.ClearMemDC();
			m_NoteInfo.m_MemDC.WriteText("  Link to Refraction analysis", InfoRect, Font, BLACK, 0, NOCOLOR);
			m_HighLight1 = 1;
			m_HighLight2 = 0;
			Repaint();
			SetTimer(SOLOTIMER, 1300, 0);
		}
		m_NoteInfo.Repaint();///	
	}
	else if (point.x > m_Rect[7].left && point.x < m_Rect[7].right
		&& point.y > m_Rect[7].top && point.y < m_Rect[7].bottom)
	{
		::SetCursor(::LoadCursor(NULL, IDC_HAND));
		Infox = point.x + 20;
		Infoy = point.y - 20;
		m_NoteInfo.MoveWindow(Infox, Infoy, 160, 20, 1);

		if (m_HighLight1 != 2)
		{
			m_NoteInfo.ShowWindow(TRUE);

			m_NoteInfo.ClearMemDC();
			m_NoteInfo.m_MemDC.WriteText("  Link to Refraction analysis", InfoRect, Font, BLACK, 0, NOCOLOR);
			m_HighLight1 = 2;
			m_HighLight2 = 0;
			Repaint();
			SetTimer(SOLOTIMER, 1300, 0);
		}
		m_NoteInfo.Repaint();
	}

	else if (point.x > m_Rect[8].left && point.x < m_Rect[8].right
		&& point.y > m_Rect[8].top && point.y < m_Rect[8].bottom)
	{
		::SetCursor(::LoadCursor(NULL, IDC_HAND));
		Infox = point.x + 20;
		Infoy = point.y - 20;
		m_NoteInfo.MoveWindow(Infox, Infoy, 160, 20, 1);

		if (m_HighLight2 != 1)
		{
			m_NoteInfo.ShowWindow(TRUE);

			m_NoteInfo.ClearMemDC();
			m_NoteInfo.m_MemDC.WriteText("  Link to Aberration analysis", InfoRect, Font, BLACK, 0, NOCOLOR);
			m_HighLight2 = 1;
			m_HighLight1 = 0;
			Repaint();
			SetTimer(SOLOTIMER, 1300, 0);
		}
		m_NoteInfo.Repaint();
	}

	else if (point.x > m_Rect[9].left && point.x < m_Rect[9].right
		&& point.y > m_Rect[9].top && point.y < m_Rect[9].bottom)
	{
		::SetCursor(::LoadCursor(NULL, IDC_HAND));
		Infox = point.x - 170;
		Infoy = point.y - 20;
		m_NoteInfo.MoveWindow(Infox, Infoy, 160, 20, 1);

		if (m_HighLight2 != 2)
		{
			m_NoteInfo.ShowWindow(TRUE);
			m_NoteInfo.ClearMemDC();
			m_NoteInfo.m_MemDC.WriteText("  Link to Aberration analysis", InfoRect, Font, BLACK, 0, NOCOLOR);
			m_HighLight2 = 2;
			m_HighLight1 = 0;
			Repaint();
			SetTimer(SOLOTIMER, 1300, 0);
		}
		m_NoteInfo.Repaint();
	}
	else
	{
		m_NoteInfo.ShowWindow(FALSE);

		if (m_HighLight1 != 0 || m_HighLight2 != 0)
		{
			m_HighLight1 = 0;
			m_HighLight2 = 0;
			Repaint();
		}
	}
}

//***************************************************************************************

// solo hyperlinker
void CWFSoloVfaWnd::OnLButtonDown(uint nFlags, CPoint Point)
{
	CPoint point;
	GetCursorPos(&point);

	if (point.x > m_Rect[6].left && point.x < m_Rect[6].right
		&& point.y > m_Rect[6].top && point.y < m_Rect[6].bottom)
	{
		GetParent()->PostMessage(WM_SOLO_HYPELINK, 1, 0);
	}
	else if (point.x > m_Rect[7].left && point.x < m_Rect[7].right
		&& point.y > m_Rect[7].top && point.y < m_Rect[7].bottom)
	{
		GetParent()->PostMessage(WM_SOLO_HYPELINK, 1, 1);
	}

	else if (point.x > m_Rect[8].left && point.x < m_Rect[8].right
		&& point.y > m_Rect[8].top && point.y < m_Rect[8].bottom)
	{
		GetParent()->PostMessage(WM_SOLO_HYPELINK, 0, 0);
	}

	else if (point.x > m_Rect[9].left && point.x < m_Rect[9].right
		&& point.y > m_Rect[9].top && point.y < m_Rect[9].bottom)
	{
		GetParent()->PostMessage(WM_SOLO_HYPELINK, 0, 1);
	}
}
//Done
//***************************************************************************************

//***************************************************************************************

void CWFSoloVfaWnd::OnSunButton1Clicked()
{
	OnSunButtonClicked();
}

//***************************************************************************************

void CWFSoloVfaWnd::OnMoonButton1Clicked()
{
	OnMoonButtonClicked();
}

//***************************************************************************************

void CWFSoloVfaWnd::OnSunButton2Clicked()
{
	OnSunButtonClicked();
}

//***************************************************************************************

void CWFSoloVfaWnd::OnMoonButton2Clicked()
{
	OnMoonButtonClicked();
}

//***************************************************************************************

void CWFSoloVfaWnd::OnSunButtonClicked()
{
	CBusyCursor Cursor;

	if (m_pWFExam1)
	{
		m_scan1.Format(_T("%.2f mm"), ::TempSettings.Com_r_max_um1*0.002);
		for (int i = 0; i < 3; i++) m_pWndSettings[i].m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um1);

		m_ODSlider.m_Pos = m_ODSliderMaxPos;
		m_ODSlider.OnPaint();
	}


	if (m_pWFExam2)
	{
		m_scan2.Format(_T("%.2f mm"), ::TempSettings.Com_r_max_um2*0.002);
		for (int i = 3; i < 6; i++) m_pWndSettings[i].m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um2);

		m_OSSlider.m_Pos = m_OSSliderMaxPos;
		m_OSSlider.OnPaint();
	}

	CreateChildWnd();

	Repaint();
}

//***************************************************************************************

void CWFSoloVfaWnd::OnMoonButtonClicked()
{
	CBusyCursor Cursor;

	m_scan1 = "1.00 mm";
	m_scan2 = "1.00 mm";

	if (m_pWFExam1)
	{
		for (int i = 0; i < 3; i++) m_pWndSettings[i].m_ZoneRadiusUm = 500;
		m_ODSlider.m_Pos = 0;
		m_ODSlider.OnPaint();
	}

	if (m_pWFExam2)
	{
		for (int i = 3; i < 6; i++) m_pWndSettings[i].m_ZoneRadiusUm = 500;
		m_OSSlider.m_Pos = 0;
		m_OSSlider.OnPaint();
	}

	CreateChildWnd();

	Repaint();
}

//***************************************************************************************

//***************************************************************************************
LRESULT CWFSoloVfaWnd::OnSliderClick(WPARAM wParam, LPARAM lParam)
{
	CBusyCursor Cursor;

	CPoint point;
	GetCursorPos(&point);

	if (point.x < m_MideLine && m_pWFExam1)
	{
		BOOL limitOS = TRUE;
		if (m_ODSliderMaxPos != 1)
		{
			if (m_ODSlider.m_Pos > m_ODSliderMaxPos)
			{
				m_ODSlider.m_Pos = m_ODSliderMaxPos;
				m_ODSlider.OnPaint();

				if (m_pWFExam2)
				{
					m_OSSlider.m_Pos = m_ODSlider.m_Pos;
					m_OSSlider.OnPaint();
				}

				for (int i = 0; i < 6; i++)
				{
					m_pWndSettings[i].m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um1);

				}

				m_scan1.Format(_T("%.2f mm"), ::TempSettings.Com_r_max_um1*0.002);
				m_scan2 = m_scan1;
				Repaint();

				return 0;
			}
			limitOS = FALSE;
		}
		else
		{
			if (m_ODSlider.m_Pos > 0.95) m_ODSlider.m_Pos = 1;
		}

		int newZoneRadiusUm = intRound(m_maxRum - (m_maxRum - 500)* (1 - m_ODSlider.m_Pos));

		//****************************
		if (newZoneRadiusUm > ::TempSettings.Com_r_max_um1)
		{
			newZoneRadiusUm = intRound(::TempSettings.Com_r_max_um1);
		}
		else
		{
			newZoneRadiusUm = GetClosePos(newZoneRadiusUm, intRound(::TempSettings.Com_r_max_um1));
		}

		m_ODSlider.m_Pos = 1 - (m_maxRum - newZoneRadiusUm) / (m_maxRum - 500);
		SetCursorPos(point.x + 7, m_Slider_Top + m_ODSlider.returnY());
		//****************************

		for (int i = 0; i < 6; i++)
		{
			m_pWndSettings[i].m_ZoneRadiusUm = newZoneRadiusUm;
		}

		CreateChildWnd();
		m_scan1.Format(_T("%.2f mm"), newZoneRadiusUm*0.002);
		m_scan2 = m_scan1;

		if (m_pWFExam2)
		{
			if (limitOS && (newZoneRadiusUm > ::TempSettings.Com_r_max_um2))
			{
				m_OSSlider.m_Pos = m_OSSliderMaxPos;
				m_scan2.Format(_T("%.2f mm"), ::TempSettings.Com_r_max_um2*0.002);
			}
			else m_OSSlider.m_Pos = m_ODSlider.m_Pos;
			m_OSSlider.OnPaint();
		}

		m_ODSlider.OnPaint();
	}

	//OS
	else if (point.x > m_MideLine && m_pWFExam2)
	{
		BOOL limitOD = TRUE;
		if (m_OSSliderMaxPos != 1)
		{
			if (m_OSSlider.m_Pos > m_OSSliderMaxPos)
			{
				m_OSSlider.m_Pos = m_OSSliderMaxPos;
				m_OSSlider.OnPaint();

				if (m_pWFExam1)
				{
					m_ODSlider.m_Pos = m_OSSlider.m_Pos;
					m_ODSlider.OnPaint();
				}

				for (int i = 0; i < 6; i++)
				{
					m_pWndSettings[i].m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um2);

				}

				m_scan1.Format(_T("%.2f mm"), ::TempSettings.Com_r_max_um2*0.002);
				m_scan2 = m_scan1;
				Repaint();

				return 0;
			}
			limitOD = FALSE;
		}
		else
		{
			if (m_OSSlider.m_Pos > 0.95) m_OSSlider.m_Pos = 1;
		}
		int newZoneRadiusUm = intRound(m_maxRum - (m_maxRum - 500)* (1 - m_OSSlider.m_Pos));

		//****************************
		if (newZoneRadiusUm > ::TempSettings.Com_r_max_um2)
		{
			newZoneRadiusUm = intRound(::TempSettings.Com_r_max_um2);
		}
		else
		{
			newZoneRadiusUm = GetClosePos(newZoneRadiusUm, intRound(::TempSettings.Com_r_max_um2));
		}

		m_OSSlider.m_Pos = 1 - (m_maxRum - newZoneRadiusUm) / (m_maxRum - 500);
		SetCursorPos(point.x + 7, m_Slider_Top + m_OSSlider.returnY());
		//SetCursorPos(point.x + 20, point.y);
		//****************************

		m_scan2.Format(_T("%.2f mm"), newZoneRadiusUm*0.002);
		m_scan1 = m_scan2;

		for (int i = 0; i < 6; i++)
		{
			m_pWndSettings[i].m_ZoneRadiusUm = newZoneRadiusUm;
		}

		CreateChildWnd();

		if (m_pWFExam1)
		{
			if (limitOD && (newZoneRadiusUm > ::TempSettings.Com_r_max_um1))
			{
				m_ODSlider.m_Pos = m_ODSliderMaxPos;
				m_scan1.Format(_T("%.2f mm"), ::TempSettings.Com_r_max_um1*0.002);
			}
			else m_ODSlider.m_Pos = m_OSSlider.m_Pos;

			m_ODSlider.OnPaint();
		}

		m_OSSlider.OnPaint();
	}

	Repaint();

	return 0;
}

//***************************************************************************************
//***************************************************************************************
LRESULT CWFSoloVfaWnd::OnSliderChangedPos(WPARAM wParam, LPARAM lParam)
{
	CPoint point;
	GetCursorPos(&point);

	if (point.x < m_MideLine && m_pWFExam1)
	{
		BOOL limitOS = TRUE;
		if (m_ODSliderMaxPos != 1)
		{
			if (m_ODSlider.m_Pos > m_ODSliderMaxPos)
			{
				m_ODSlider.m_Pos = m_ODSliderMaxPos;
				m_ODSlider.OnPaint();

				if (m_pWFExam2)
				{
					m_OSSlider.m_Pos = m_ODSlider.m_Pos;
					m_OSSlider.OnPaint();
				}

				for (int i = 0; i < 6; i++)
				{
					m_pWndSettings[i].m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um1);
				}

				m_scan1.Format(_T("%.2f mm"), ::TempSettings.Com_r_max_um1*0.002);
				m_scan2 = m_scan1;
				Repaint();

				return 0;
			}
			limitOS = FALSE;
		}
		else
		{
			if (m_ODSlider.m_Pos > 0.95) m_ODSlider.m_Pos = 1;
		}

		int newZoneRadiusUm = intRound(m_maxRum - (m_maxRum - 500)* (1 - m_ODSlider.m_Pos));

		if (newZoneRadiusUm > ::TempSettings.Com_r_max_um1)
		{
			newZoneRadiusUm = intRound(::TempSettings.Com_r_max_um1);
		}

		for (int i = 0; i < 6; i++)
		{
			m_pWndSettings[i].m_ZoneRadiusUm = newZoneRadiusUm;
		}

		CreateChildWnd();
		m_scan1.Format(_T("%.2f mm"), newZoneRadiusUm*0.002);
		m_scan2 = m_scan1;

		if (m_pWFExam2)
		{
			if (limitOS && (newZoneRadiusUm > ::TempSettings.Com_r_max_um2))
			{
				m_OSSlider.m_Pos = m_OSSliderMaxPos;
				m_scan2.Format(_T("%.2f mm"), ::TempSettings.Com_r_max_um2*0.002);
			}
			else m_OSSlider.m_Pos = m_ODSlider.m_Pos;

			m_OSSlider.OnPaint();
		}
	}



	//OS
	else if (point.x > m_MideLine && m_pWFExam2)
	{
		BOOL limitOD = TRUE;
		if (m_OSSliderMaxPos != 1)
		{
			if (m_OSSlider.m_Pos > m_OSSliderMaxPos)
			{
				m_OSSlider.m_Pos = m_OSSliderMaxPos;
				m_OSSlider.OnPaint();

				if (m_pWFExam1)
				{
					m_ODSlider.m_Pos = m_OSSlider.m_Pos;
					m_ODSlider.OnPaint();
				}

				for (int i = 0; i < 6; i++)
				{
					m_pWndSettings[i].m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um2);

				}

				m_scan1.Format(_T("%.2f mm"), ::TempSettings.Com_r_max_um2*0.002);
				m_scan2 = m_scan1;
				Repaint();

				return 0;
			}
			limitOD = FALSE;
		}
		else
		{
			if (m_OSSlider.m_Pos > 0.95) m_OSSlider.m_Pos = 1;
		}
		int newZoneRadiusUm = intRound(m_maxRum - (m_maxRum - 500)* (1 - m_OSSlider.m_Pos));

		if (newZoneRadiusUm > ::TempSettings.Com_r_max_um2)
		{
			newZoneRadiusUm = intRound(::TempSettings.Com_r_max_um2);
		}

		m_scan2.Format(_T("%.2f mm"), newZoneRadiusUm*0.002);
		m_scan1 = m_scan2;

		for (int i = 0; i < 6; i++)
		{
			m_pWndSettings[i].m_ZoneRadiusUm = newZoneRadiusUm;
		}

		CreateChildWnd();
		if (m_pWFExam1)
		{
			if (limitOD && (newZoneRadiusUm > ::TempSettings.Com_r_max_um1))
			{
				m_ODSlider.m_Pos = m_ODSliderMaxPos;
				m_scan1.Format(_T("%.2f mm"), ::TempSettings.Com_r_max_um1*0.002);
			}
			else m_ODSlider.m_Pos = m_OSSlider.m_Pos;

			m_ODSlider.OnPaint();
		}
	}

	Repaint();

	return 0;
}

//***************************************************************************************

//***************************************************************************************
//new
void CWFSoloVfaWnd::OnShowDefaultItemClicked()
{
	CBusyCursor Cursor;

	m_pWndSettings[m_d].m_LtrDistanceType = 0;//530
											  //WF Vfa display

	if (m_d == 0 || m_d == 3)
	{
		m_pWndSettings[0].m_Type = 11;
		m_pWndSettings[3].m_Type = 11;
	}

	for (int i = 0; i < 6; i++)
	{
		m_pWndSettings[i].m_Mask.SetType(MASK_HO_TOTAL);
		m_pWndSettings[i].m_Piston = FALSE;
		m_pWndSettings[i].m_Tilt = FALSE;

		if (m_d == 2 || m_d == 5)
		{
			if (i != 0 && i != 3)
			{
				m_pWndSettings[i].m_Ltr = 0;
				m_pWndSettings[i].m_LtrLine = 70;
				m_pWndSettings[i].m_LtrOrientation = 0;
			}
		}

		if (m_d == 0 || m_d == 3)
		{
			if (i == 0 || i == 3)
			{
				CScale* pScale = m_pWndSettings[i].GetScale();
				pScale->m_Step = 0.5;

				m_pWndSettings[i].SetDefaultColors(1);
			}
		}
	}
	//Done  

	CreateChildWnd();

	Repaint();
}
//new
//***************************************************************************************
//new
BOOL CWFSoloVfaWnd::SameSoloVfa()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_d == 0 || m_d == 3)
	{
		if (pWndSettings->m_Type != 11)    return FALSE;

		CScale* pScale = pWndSettings->GetScale();
		if (pScale->m_Step != 0.5)                         return FALSE;
		if (!pWndSettings->AreDefaultColors(1))            return FALSE;
	}

	if (pWndSettings->m_Mask.GetType() != MASK_HO_TOTAL)   return FALSE;

	if (m_d == 2 || m_d == 5)
	{
		if (pWndSettings->m_Ltr != 0)         return FALSE;
		if (pWndSettings->m_LtrLine != 70)        return FALSE;
		if (pWndSettings->m_LtrOrientation != 0)         return FALSE;
	}

	return TRUE;
}
//new
//***************************************************************************************
//530
void CWFSoloVfaWnd::OnShowWFTextInfoBtnClicked()
{
	m_pWndSettings[0].m_SimplifiedWFPanel = !m_pWndSettings[0].m_SimplifiedWFPanel;
	Repaint();

	CString s = (m_pWndSettings[0].m_SimplifiedWFPanel ? "Show Details" : "Hide Details");
	m_WFShowFullInfoButton.SetTip(s);
	m_WFShowFullInfoButton.SetImage(m_pWndSettings[0].m_SimplifiedWFPanel ?
		(m_SW == 1920 ? IDR_SHOW_DETAIL_3 : m_SW == 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
		(m_SW == 1920 ? IDR_HIDE_DETAIL_3 : m_SW == 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
	m_WFShowFullInfoButton.Repaint();

	if (m_pWFExam2)
	{
		m_WFShowFullInfoButton2.SetImage(m_pWndSettings[0].m_SimplifiedWFPanel ?
			(m_SW == 1920 ? IDR_SHOW_DETAIL_3 : m_SW == 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
			(m_SW == 1920 ? IDR_HIDE_DETAIL_3 : m_SW == 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
		m_WFShowFullInfoButton2.SetTip(s);
		m_WFShowFullInfoButton2.Repaint();
	}
}
//530
//***************************************************************************************
//530
void CWFSoloVfaWnd::OnShowWFTextInfoBtn2Clicked()
{
	OnShowWFTextInfoBtnClicked();
}
//530
//***************************************************************************************