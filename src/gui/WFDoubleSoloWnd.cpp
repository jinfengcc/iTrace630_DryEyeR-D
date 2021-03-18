// Solo Refraction analysis display

//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "WFDoubleSoloWnd.h"
#include "EyeWnd.h"
#include "PSFWnd.h"
#include "MTFWnd.h"
#include "EEFWnd.h"
#include "LtrWnd.h"
#include "RMSWnd.h"
#include "ChartWnd.h"
#include "BusyCursor.h"
#include "SelectImgDlg.h"
#include "NearLtrWnd.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CWFDoubleSoloWnd, CSumWnd)

	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()

	ON_MESSAGE(WM_CHANGE_CENT, OnChangeCentMsg)//No used now
	ON_MESSAGE(WM_CHANGE_STEP, OnChangeStepMsg)//No used now

	ON_MESSAGE(WM_SLIDER_LCLICK, OnSliderClick)
	ON_MESSAGE(WM_SLIDER_POS_CHANGED, OnSliderChangedPos)

	ON_BN_CLICKED(IDC_SOLO_SUN_ITEM1, OnSunButton1Clicked)
	ON_BN_CLICKED(IDC_SOLO_MOON_ITEM1, OnMoonButton1Clicked)
	ON_BN_CLICKED(IDC_SOLO_TOGGLE_ITEM1, OnToggleButton1Clicked)

	ON_BN_CLICKED(IDC_SOLO_SUN_ITEM2, OnSunButton2Clicked)
	ON_BN_CLICKED(IDC_SOLO_MOON_ITEM2, OnMoonButton2Clicked)
	ON_BN_CLICKED(IDC_SOLO_TOGGLE_ITEM2, OnToggleButton2Clicked)

	ON_COMMAND(IDC_DEFAULT_GUI_ITEM, OnShowDefaultItemClicked)//new
	ON_COMMAND(IDC_SELECT_IMAGE_ITEM, OnSelEyeImgItemClicked)//new

END_MESSAGE_MAP()

//***************************************************************************************

CWFDoubleSoloWnd::CWFDoubleSoloWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CWFExam* pWFExam1, CWFExam* pWFExam2, CWndSettings* pWndSettings) :
	CSumWnd(pWnd, WndRect, pPatient, pWndSettings)
{
	CBusyCursor Cursor;

	//----------------------------------------------------
	real_t Top = 0.1*m_h;
	real_t MapTop = 0.16*m_h;
	m_MideLine = m_w / 2.0;

	m_w1 = 0.5*m_MideLine;
	real_t w2 = 0.08*m_MideLine;

	real_t h1 = 0.43*m_h - m_g;

	if (h1 < 0.8*m_w1)  m_w1 = h1 / 0.8;

	m_StopX1 = m_g + m_w1 + w2;
	m_StopX2 = m_g + m_MideLine + m_w1 + w2;

	::SetRect(&m_Rect[0], intRound(m_g), intRound(MapTop), intRound(m_g + m_w1), intRound(MapTop + h1)); // exam 1 letter
	::SetRect(&m_Rect[1], intRound(m_g), intRound(MapTop + h1 + m_g), intRound(m_g + m_w1), intRound(m_h - m_g)); // exam 1 letter
	::SetRect(&m_Rect[2], intRound(m_g + m_MideLine), intRound(MapTop), intRound(m_g + m_MideLine + m_w1), intRound(MapTop + h1)); // exam 2 letter
	::SetRect(&m_Rect[3], intRound(m_g + m_MideLine), intRound(MapTop + h1 + m_g), intRound(m_g + m_MideLine + m_w1), intRound(m_h - m_g)); // exam 2 letter

																																			//Exam Info Rect
	int h3 = intRound(0.6*(MapTop - Top));
	int w3 = intRound(0.8*m_MideLine);

	int h4 = intRound((m_h - m_g - MapTop) / (real_t)3);

	int t = intRound(MapTop + h4);
	::SetRect(&m_Rect[4], intRound(m_MideLine - w3), intRound(Top), intRound(m_MideLine - m_g), intRound(Top + h3));
	::SetRect(&m_Rect[5], intRound(m_w - w3), intRound(Top), intRound(m_w - m_g), intRound(Top + h3));

	::SetRect(&m_Rect[8], intRound(m_StopX1 + 5), intRound(MapTop), intRound(m_MideLine - 5), t);
	::SetRect(&m_Rect[9], intRound(m_StopX2 + 5), intRound(MapTop), intRound(m_w - 5), t);

	::SetRect(&m_Rect[10], intRound(m_StopX1 + m_g), t, intRound(m_MideLine - m_g), t + h4);
	::SetRect(&m_Rect[11], intRound(m_StopX2 + m_g), t, intRound(m_w - m_g), t + h4);

	::SetRect(&m_Rect[6], intRound(m_StopX1 + 5), t + h4, intRound(m_MideLine - 5), t + h4 + h4);
	::SetRect(&m_Rect[7], intRound(m_StopX2 + 5), t + h4, intRound(m_w - 5), t + h4 + h4);

	//Done

	m_pWFExam1 = NULL;
	m_pWFExam2 = NULL;

	if (pWFExam1)
	{
		if (pWFExam1->m_Header.IsOD()) m_pWFExam1 = pWFExam1;
		else m_pWFExam2 = pWFExam1;
	}

	if (pWFExam2)
	{
		if (pWFExam2->m_Header.IsOD()) m_pWFExam1 = pWFExam2;
		else m_pWFExam2 = pWFExam2;
	}

	m_invalidValue = 0;
	real_t m_r1, m_r2;
	if (m_pWFExam1 == NULL)
	{
		m_invalidValue = 1;
	}
	else m_r1 = m_pWFExam1->m_WfSurface.m_r_max_um;

	if (m_pWFExam2 == NULL) m_invalidValue = (m_invalidValue == 0 ? 2 : 3);
	else m_r2 = m_pWFExam2->m_WfSurface.m_r_max_um;

	switch (m_invalidValue)
	{
	case 0:
		::TempSettings.Com_r_max_um = (m_r1 < m_r2 ? m_r1 : m_r2);
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
		::TempSettings.Com_r_max_um = 5000;
		break;
	}
	//*007*[cjf***05052012]

	//
	if (m_pWFExam1 != NULL)
	{
		::TempSettings.Com_r_max_um1 = m_r1;
		m_scan1.Format(_T("%.2f mm"), m_r1*0.002);
	}
	if (m_pWFExam2 != NULL)
	{
		::TempSettings.Com_r_max_um2 = m_r2;
		m_scan2.Format(_T("%.2f mm"), m_r2*0.002);
	}

	//

	//Sliders and some dispaly windows controls
	int  Slider_Left1 = intRound(m_g + m_w1 + 0.25*w2);
	int  Slider_Right1 = intRound(m_g + m_w1 + 0.75*w2);
	int  Slider_Left2 = intRound(m_g + m_MideLine + m_w1 + 0.25*w2);
	int  Slider_Right2 = intRound(m_g + m_MideLine + m_w1 + 0.75*w2);

	real_t Info_Total_H = m_h - m_g - MapTop;

	m_Slider_Top = intRound(MapTop + 0.25*Info_Total_H);
	int  Slider_Bot = intRound(MapTop + 0.75*Info_Total_H);
	int  IconWide = 9;

	::SetRect(&m_SliderRects[0], Slider_Left1, m_Slider_Top, Slider_Right1, Slider_Bot);
	::SetRect(&m_SliderRects[1], Slider_Left2, m_Slider_Top, Slider_Right2, Slider_Bot);

	::SetRect(&m_SliderRects[2], Slider_Left1, intRound(m_Slider_Top - 2.5*IconWide), Slider_Right1, intRound(m_Slider_Top - 0.5*IconWide));
	::SetRect(&m_SliderRects[3], Slider_Left1, intRound(Slider_Bot + 0.8*IconWide), Slider_Right1, intRound(Slider_Bot + 2.8*IconWide));
	::SetRect(&m_SliderRects[4], Slider_Left1, intRound(Slider_Bot + 4.5*IconWide), Slider_Right1, intRound(Slider_Bot + 10.5*IconWide));

	::SetRect(&m_SliderRects[5], Slider_Left2, intRound(m_Slider_Top - 2.5*IconWide), Slider_Right2, intRound(m_Slider_Top - 0.5*IconWide));
	::SetRect(&m_SliderRects[6], Slider_Left2, intRound(Slider_Bot + 0.8*IconWide), Slider_Right2, intRound(Slider_Bot + 2.8*IconWide));
	::SetRect(&m_SliderRects[7], Slider_Left2, intRound(Slider_Bot + 4.5*IconWide), Slider_Right2, intRound(Slider_Bot + 10.5*IconWide));

	::SetRect(&m_SliderRects[8], intRound(m_g + m_w1), intRound(m_Slider_Top - 5.5*IconWide), intRound(m_StopX1), intRound(m_Slider_Top - 3 * IconWide));
	::SetRect(&m_SliderRects[9], intRound(m_g + m_w1 + m_MideLine), intRound(m_Slider_Top - 5.5*IconWide), intRound(m_StopX2), intRound(m_Slider_Top - 3 * IconWide));


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

		m_ToggleButton1.Create(_T(""), _T(""), 0, m_SliderRects[4], this, IDC_SOLO_TOGGLE_ITEM1);
		if (m_pWndSettings[0].m_Type == TYPE_TLTR && m_pWndSettings[1].m_Type == TYPE_TLTR)
		{
			m_ToggleButton1.SetImage(IDR_SOLO_E_MAP);
			m_ToggleButton1.SetTip("Switch to map");
		}
		else
		{
			m_ToggleButton1.SetImage(IDR_SOLO_E_MAP);
			m_ToggleButton1.SetTip("Switch to Visual Acuity");
		}
		m_ToggleButton1.SetBk(FALSE);
		m_ToggleButton1.SetBkColor(BLACK);
		m_ToggleButton1.ShowWindow(SW_SHOW);
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

		m_SunButton2.Create(_T(""), _T(""), 0, m_SliderRects[5], this, IDC_SOLO_SUN_ITEM2);
		m_SunButton2.SetImage(IDR_SOLO_MOON);
		m_SunButton2.SetBk(FALSE);
		m_SunButton2.SetBkColor(BLACK);
		m_SunButton2.ShowWindow(SW_SHOW);

		m_MoonButton2.Create(_T(""), _T(""), 0, m_SliderRects[6], this, IDC_SOLO_MOON_ITEM2);
		m_MoonButton2.SetImage(IDR_SOLO_SUN);
		m_MoonButton2.SetBk(FALSE);
		m_MoonButton2.SetBkColor(BLACK);
		m_MoonButton2.ShowWindow(SW_SHOW);

		m_ToggleButton2.Create(_T(""), _T(""), 0, m_SliderRects[7], this, IDC_SOLO_TOGGLE_ITEM2);
		if (m_pWndSettings[2].m_Type == TYPE_TLTR && m_pWndSettings[3].m_Type == TYPE_TLTR)
		{
			m_ToggleButton2.SetImage(IDR_SOLO_E_MAP);
			m_ToggleButton2.SetTip("Switch to map");
		}
		else
		{
			m_ToggleButton2.SetImage(IDR_SOLO_E_MAP);
			m_ToggleButton2.SetTip("Switch to Visual Acuity");
		}
		m_ToggleButton2.SetBk(FALSE);
		m_ToggleButton2.SetBkColor(BLACK);
		m_ToggleButton2.ShowWindow(SW_SHOW);
	}
	//Done

	for (m_d = 0; m_d < 4; m_d++)
	{
		CWndSettings* pWndSettings = GetWndSettings();
		pWndSettings->m_ZoneRadiusUm = 5000;

		if (!m_pWndSettings[m_d].m_SizeSmall)
		{
			m_pWndSettings[m_d].m_SizeSmall = TRUE;
		}
		if (m_pWFExam1 && (m_d == 0 || m_d == 1))
			CreateChildWnd();
		if (m_pWFExam2 && (m_d == 2 || m_d == 3))
			CreateChildWnd();
	}

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

	//The 'transform funtionality' info ("Link to Aberration analysis")
	m_HighLight = FALSE;

	RECT InfoRect;
	::SetRect(&InfoRect, 0, 0, 160, 20);
	m_NoteInfo.CreateWnd(InfoRect, this);
	m_NoteInfo.ClearMemDC();

	CMFont Font(15, 400, "Arial");
	m_NoteInfo.m_MemDC.WriteText("  Link to Aberration analysis", InfoRect, Font, BLACK, 0, NOCOLOR);
	m_NoteInfo.ShowWindow(FALSE);
	//Done

	ShowWindow(SW_SHOW);
}

//***************************************************************************************

void CWFDoubleSoloWnd::RepaintMemDC()
{
	CSumWnd::RepaintMemDC();

	COLORREF white = m_Printing ? BLACK : WHITE;
	CMFont FontScanSize(intRound(1.8*m_g), 400, "Arial");

	int y0 = intRound(0.1*m_h);
	int x0 = intRound(m_w / 2.0);

	m_MemDC.DrawLine(0, y0, m_w, y0, 2, white);
	m_MemDC.DrawLine(x0, y0, x0, m_h, 2, white);

	int MapTop = intRound(0.16*m_h);

	if (m_pWFExam1)
	{
		m_MemDC.DrawLine(intRound(m_g + m_w1), MapTop, intRound(m_StopX1), intRound(MapTop), 1, DARK_GRAY);
		m_MemDC.DrawLine(intRound(m_StopX1), MapTop, intRound(m_StopX1), intRound(m_h - m_g - 1), 1, DARK_GRAY);
		m_MemDC.DrawLine(intRound(m_g + m_w1), intRound(m_h - m_g - 1), intRound(m_StopX1), intRound(m_h - m_g - 1), 1, DARK_GRAY);

		m_MemDC.WriteText(m_scan1, m_SliderRects[8], FontScanSize, RED, 1, NOCOLOR);
	}

	if (m_pWFExam2)
	{
		m_MemDC.DrawLine(intRound(m_MideLine + m_g + m_w1), intRound(MapTop), intRound(m_StopX2), intRound(MapTop), 1, DARK_GRAY);
		m_MemDC.DrawLine(intRound(m_StopX2), intRound(MapTop), intRound(m_StopX2), intRound(m_h - m_g - 1), 1, DARK_GRAY);
		m_MemDC.DrawLine(intRound(m_MideLine + m_g + m_w1), intRound(m_h - m_g - 1), intRound(m_StopX2), intRound(m_h - m_g - 1), 1, DARK_GRAY);
		m_MemDC.WriteText(m_scan2, m_SliderRects[9], FontScanSize, RED, 1, NOCOLOR);
	}

	//OD OS text Info
	CMFont Font(intRound(0.7*(MapTop - y0)), 400, "Arial");

	RECT Rect1, Rect2;
	::SetRect(&Rect1, intRound(m_g), intRound(y0 + m_g), intRound(0.05 * m_w), intRound(MapTop - m_g));
	::SetRect(&Rect2, intRound(m_g + m_MideLine), intRound(y0 + m_g), intRound(m_MideLine + 0.05 * m_w), intRound(MapTop - m_g));
	if (m_pWFExam1) m_MemDC.WriteText("OD", Rect1, Font, white, 0);
	if (m_pWFExam2) m_MemDC.WriteText("OS", Rect2, Font, white, 0);
	//Done

	//Other Info  
	BOOL HighLight = FALSE;
	if (m_pWFExam1) {
		HighLight = TRUE ? m_HighLight == 1 : FALSE;
		WFSoloDoubleExamInfo(m_pWFExam1, m_Rect[4], m_Rect[6], m_Rect[10], m_Rect[8], HighLight);
	}

	if (m_pWFExam2) {
		HighLight = TRUE ? m_HighLight == 2 : FALSE;
		WFSoloDoubleExamInfo(m_pWFExam2, m_Rect[5], m_Rect[7], m_Rect[11], m_Rect[9], HighLight);
	}
	//Done
}

//***************************************************************************************

void CWFDoubleSoloWnd::CreateChildWnd()
{
	SAFE_DELETE(m_pDispWnd[m_d]);
	// type

	CWndSettings *pWndSettings = &m_pWndSettings[m_d];
	if (!(pWndSettings->m_Ltr == 0 || pWndSettings->m_Ltr == 1))
	{
		pWndSettings->m_Ltr = 0;
	}
	if (!(pWndSettings->m_LtrLine == 20 || pWndSettings->m_LtrLine == 25 || pWndSettings->m_LtrLine == 30 ||
		pWndSettings->m_LtrLine == 40 || pWndSettings->m_LtrLine == 50 || pWndSettings->m_LtrLine == 70 ||
		pWndSettings->m_LtrLine == 100 || pWndSettings->m_LtrLine == 200)) {
		pWndSettings->m_LtrLine = 70;
	}

	if (!(pWndSettings->m_LtrOrientation == 0 || pWndSettings->m_LtrOrientation == 90 ||
		pWndSettings->m_LtrOrientation == 180 || pWndSettings->m_LtrOrientation == 270)) {
		pWndSettings->m_LtrOrientation = 0;
	}
		
	if (m_d == 0 || m_d == 1)
	{
		if (m_pWFExam1)
			m_Surfaces[m_d] = m_pWFExam1->m_WfSurface;
	}
	else
	{
		if (m_pWFExam2)
			m_Surfaces[m_d] = m_pWFExam2->m_WfSurface;
	}

	//
	real_t r_max_um = 5000.0;
	r_max_um = __min(r_max_um, m_Surfaces[m_d].m_r_max_um);

	r_max_um = __min(r_max_um, pWndSettings->m_ZoneRadiusUm);
	m_Surfaces[m_d].ChangeRMaxUm(r_max_um);
	m_Surfaces[m_d] &= pWndSettings->m_Mask;
	m_Surfaces[m_d].ComputePiston(pWndSettings->m_Piston);
	m_Surfaces[m_d].ComputeTilt(pWndSettings->m_Tilt);

	switch (pWndSettings->m_Type)
	{
		case TYPE_TWFM:
		{
			CEyeWnd* pEyeWnd = new CEyeWnd(m_Rect[m_d], this);
			pEyeWnd->m_Type = pWndSettings->m_Type;
			pEyeWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
			pEyeWnd->m_Method3D = FALSE;
			pEyeWnd->m_LTLabel[0] = "Wavefront Map";
			pWndSettings->m_Mask.GetName(pEyeWnd->m_LTLabel[1]);
			pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
			pEyeWnd->m_MapShowEye = pWndSettings->m_MapShowEye;
			pEyeWnd->m_MapShowMap = pWndSettings->m_MapShowMap;
			pEyeWnd->m_MapShowNumbers = pWndSettings->m_MapShowNumbers;
			pEyeWnd->m_MapShowPupil = pWndSettings->m_MapShowPupil;
			pEyeWnd->m_MapShowCornealVertex = pWndSettings->m_MapShowCornealVertex;
			pEyeWnd->m_MapShowLimbus = FALSE;// pWndSettings->m_MapShowLimbus;//
			pEyeWnd->m_MapShowSolidSurface = pWndSettings->m_MapShowSolidSurface;
			pEyeWnd->m_MapShowWireMesh = pWndSettings->m_MapShowWireMesh;
			pEyeWnd->m_MapTranslucent = pWndSettings->m_MapTranslucent;
			pEyeWnd->m_Unit = Unit::micron;
			pEyeWnd->m_Inc = pWndSettings->GetIncrement();

			if (!pWndSettings->AreDefaultColors(0) &&
				!pWndSettings->AreDefaultColors(1)
				)
			{
				pWndSettings->SetDefaultColors(1);
			}

			CScale* pScale = pWndSettings->GetScale();
			pEyeWnd->m_NumColors = pScale->m_NumColors;

			for (int i = 0; i < pScale->m_NumColors; i++)
			{
				pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
			}


			pEyeWnd->m_pWFExam = (m_d == 0 || m_d == 1 ? m_pWFExam1 : m_pWFExam2);

			CWFExam::CreateWfm2D(pEyeWnd->m_Map2D, m_Surfaces[m_d], 1.0 / pEyeWnd->m_y_px_um);

			pEyeWnd->CreateCentButtons();
			pEyeWnd->CreateZoomButtons();
			pEyeWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 0);
			//pEyeWnd->CreateZoneButtons();

			m_pDispWnd[m_d] = pEyeWnd;
		}
		break;

		case TYPE_TRFM:  //Refraction Map 11
		{
			CEyeWnd* pEyeWnd = new CEyeWnd(m_Rect[m_d], this);
			pEyeWnd->m_Type = pWndSettings->m_Type;
			pEyeWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
			pEyeWnd->m_Method3D = FALSE;
			pEyeWnd->m_LTLabel[0] = "Refraction Map";
			pWndSettings->m_Mask.GetName(pEyeWnd->m_LTLabel[1]);
			pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
			pEyeWnd->m_MapShowEye = pWndSettings->m_MapShowEye;
			pEyeWnd->m_MapShowMap = pWndSettings->m_MapShowMap;
			pEyeWnd->m_MapShowNumbers = pWndSettings->m_MapShowNumbers;
			pEyeWnd->m_MapShowPupil = pWndSettings->m_MapShowPupil;
			pEyeWnd->m_MapShowCornealVertex = pWndSettings->m_MapShowCornealVertex;
			pEyeWnd->m_MapShowLimbus = FALSE;// pWndSettings->m_MapShowLimbus;//
			pEyeWnd->m_MapShowSolidSurface = pWndSettings->m_MapShowSolidSurface;
			pEyeWnd->m_MapShowWireMesh = pWndSettings->m_MapShowWireMesh;
			pEyeWnd->m_MapTranslucent = pWndSettings->m_MapTranslucent;
			pEyeWnd->m_Unit = Unit::D;
			pEyeWnd->m_Inc = pWndSettings->GetIncrement();

			if (!pWndSettings->AreDefaultColors(0) &&
				!pWndSettings->AreDefaultColors(1)
				)
			{
				pWndSettings->SetDefaultColors(1);
			}

			CScale* pScale = pWndSettings->GetScale();
			pEyeWnd->m_NumColors = pScale->m_NumColors;
			for (int i = 0; i < pScale->m_NumColors; i++) {
				pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
			}

			pEyeWnd->m_pWFExam = (m_d == 0 || m_d == 1 ? m_pWFExam1 : m_pWFExam2);

			CWFExam::CreateRfm2D(pEyeWnd->m_Map2D, m_Surfaces[m_d], 1.0 / pEyeWnd->m_y_px_um);

			pEyeWnd->CreateCentButtons();
			pEyeWnd->CreateZoomButtons();
			pEyeWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 0);
			//pEyeWnd->CreateZoneButtons();

			m_pDispWnd[m_d] = pEyeWnd;
		}
		break;

		case TYPE_TLTR:
		{
			CLtrWnd* pLtrWnd = new CLtrWnd(m_Rect[m_d], this);
			pLtrWnd->m_DysVal = -1;//531
			CWFExam::CreateLtr(m_Surfaces[m_d], pLtrWnd->m_Ltr, pWndSettings->m_Ltr, pWndSettings->m_LtrLine, pWndSettings->m_LtrOrientation);
			pLtrWnd->m_Type = pWndSettings->m_Type;
			pLtrWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
			pLtrWnd->m_Method3D = FALSE;
			//pLtrWnd->m_LTLabel[0].Format(_T("Letter 20 / %i", pWndSettings->m_LtrLine);
			pLtrWnd->m_LTLabel[0] = "Letter";//531

			pWndSettings->m_Mask.GetName(pLtrWnd->m_LTLabel[1]);
			pLtrWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
			pLtrWnd->CreateZoomButtons();
			//pLtrWnd->CreateZoneButtons();
			pLtrWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 0);
			m_pDispWnd[m_d] = pLtrWnd;
		}
		break;
	}



	if (pWndSettings->m_Type == TYPE_TWFM || pWndSettings->m_Type == TYPE_TRFM)
	{
		CEyeWnd* pEyeWnd = (CEyeWnd*)m_pDispWnd[m_d];
		CScale* pScale = pWndSettings->GetScale();

		real_t Inc = pWndSettings->GetIncrement();

		real_t Cent = 0.0;

		real_t Step;
		if (pScale->m_StepAuto)
		{
			real_t Min = pWndSettings->m_Method3D ? pEyeWnd->m_Map3D.m_min : pEyeWnd->m_Map2D.m_min;
			real_t Max = pWndSettings->m_Method3D ? pEyeWnd->m_Map3D.m_max : pEyeWnd->m_Map2D.m_max;
			real_t Span = 2.0 * __max(fabs(Min - Cent), fabs(Max - Cent));
			real_t t = Span / pScale->m_NumColors;
			Step = (int)(t / Inc) * Inc;
			if (t > Step + 0.001) Step += Inc;
		}
		else
		{
			Step = RealRound(pScale->m_Step, Inc);
		}
		if (Step < Inc) Step = Inc;
		real_t StepMax = pWndSettings->GetMaxStep();
		if (Step > StepMax) Step = StepMax;

		pEyeWnd->m_Cent = Cent;
		pEyeWnd->m_Step = Step;
	}

	else if (pWndSettings->m_Type == TYPE_TRMS)
	{
		CRMSWnd* pRMSWnd = (CRMSWnd*)m_pDispWnd[m_d];
		/*real t = pRMSWnd->m_Surface.ComputeRms();
		int Max = (int)t;
		if (t > Max + 0.001) Max++;
		if (Max < 1) Max = 1;*/
		pRMSWnd->m_Max = 1;
		pRMSWnd->m_ZoomInButton.SetEnabled(FALSE);
		pRMSWnd->m_ZoomInButton.Repaint();
	}

	//Set the 
	if (m_pWFExam1)
	{
		if (m_pWndSettings[0].m_Type == TYPE_TLTR && m_pWndSettings[1].m_Type == TYPE_TLTR)
		{

			m_ToggleButton1.SetImage(IDR_SOLO_E_MAP);
			m_ToggleButton1.SetTip("Switch to map");
		}
		else
		{
			m_ToggleButton1.SetImage(IDR_SOLO_E_MAP);
			m_ToggleButton1.SetTip("Switch to Visual Acuity");
		}
		m_ToggleButton1.Repaint();
	}

	if (m_pWFExam2)
	{
		if (m_pWndSettings[2].m_Type == TYPE_TLTR && m_pWndSettings[3].m_Type == TYPE_TLTR)
		{
			m_ToggleButton2.SetImage(IDR_SOLO_E_MAP);
			m_ToggleButton2.SetTip("Switch to map");
		}
		else
		{
			m_ToggleButton2.SetImage(IDR_SOLO_E_MAP);
			m_ToggleButton2.SetTip("Switch to Visual Acuity");
		}
		m_ToggleButton2.Repaint();
	}
}

//***************************************************************************************

void CWFDoubleSoloWnd::CreateMenu()
{
	CWndSettings* pWndSettings = &m_pWndSettings[m_d];

	//521
	if ((::OD_AverageExam != 0 && m_d < 2) ||
		(::OS_AverageExam != 0 && m_d >= 2)
		)
	{
		m_pMenu->AppendItem(IDC_SELECT_IMAGE_ITEM, FALSE, FALSE, "Select eye image", "");

		m_pMenu->AppendSeparator();
	}
	//521	 

	m_pMenu->AppendItem(IDC_TYPE_TWFM_ITEM, pWndSettings->m_Type == TYPE_TWFM, FALSE, "Wavefront Map", "");
	m_pMenu->AppendItem(IDC_TYPE_TRFM_ITEM, pWndSettings->m_Type == TYPE_TRFM, FALSE, "Refraction Map", "");
	m_pMenu->AppendItem(IDC_TYPE_TLTR_ITEM, pWndSettings->m_Type == TYPE_TLTR, FALSE, "Visual Acuity Chart", "");

	//CreateSubmenuNLtr();

	m_pMenu->AppendSeparator();

	//CreateSubmenuZone();
	CreateSubmenuMask();

	m_pMenu->AppendSeparator();

	if (pWndSettings->m_Type == TYPE_TWFM || pWndSettings->m_Type == TYPE_TRFM)
	{
		m_pMenu->AppendItem(IDC_SHOW_MAP_MAP_ITEM, FALSE, pWndSettings->m_MapShowMap, "Show map", "");

		if ((m_d < 2 && m_pWFExam1->m_Image.m_RGBData) ||
			(m_d >= 2 && m_pWFExam2->m_Image.m_RGBData)
			)
		{
			m_pMenu->AppendItem(IDC_SHOW_MAP_EYE_ITEM, FALSE, pWndSettings->m_MapShowEye, "Show eye image", "");
			m_pMenu->AppendItem(IDC_SHOW_MAP_PUPIL_ITEM, FALSE, pWndSettings->m_MapShowPupil, "Show pupil", "");
			m_pMenu->AppendItem(IDC_SHOW_MAP_VERTEX_ITEM, FALSE, pWndSettings->m_MapShowCornealVertex, "Show corneal vertex", "");
			//m_pMenu->AppendItem(IDC_SHOW_MAP_LIMBUS_ITEM, FALSE, pWndSettings->m_MapShowLimbus, "Show limbus", "");//
		}
		m_pMenu->AppendItem(IDC_MAP_TRANSLUCENT_ITEM, FALSE, pWndSettings->m_MapTranslucent, "Translucent map", "");

		m_pMenu->AppendSeparator();//521
		CreateSubmenuMapSteps();
		CreateSubmenuMapColors();
		m_pMenu->AppendSeparator();
	}
	else if (pWndSettings->m_Type == TYPE_TLTR)
	{
		CreateSubmenuLtr();
		m_pMenu->AppendSeparator();
	}

	m_pMenu->AppendItem(IDC_SAVE_TXT_ITEM, FALSE, FALSE, "Save Zernike coefficients into file", "");

	if (!SameRefSet())
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
	}
}

//***************************************************************************************

LRESULT CWFDoubleSoloWnd::OnChangeCentMsg(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

//***************************************************************************************

LRESULT CWFDoubleSoloWnd::OnChangeStepMsg(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
//***************************************************************************************
LRESULT CWFDoubleSoloWnd::OnSliderClick(WPARAM wParam, LPARAM lParam)
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

				for (m_d = 0; m_d < 4; m_d++)
				{
					CWndSettings* pWndSettings = GetWndSettings();

					pWndSettings->m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um1);

					if ((m_d == 2 || m_d == 3))
					{
						if (m_pWFExam2) CreateChildWnd();
					}
					else CreateChildWnd();
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

		for (m_d = 0; m_d < 4; m_d++)
		{
			CWndSettings* pWndSettings = GetWndSettings();

			pWndSettings->m_ZoneRadiusUm = newZoneRadiusUm;

			if ((m_d == 2 || m_d == 3))
			{
				if (m_pWFExam2) CreateChildWnd();
			}
			else
				CreateChildWnd();
		}


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

				for (m_d = 0; m_d < 4; m_d++)
				{
					CWndSettings* pWndSettings = GetWndSettings();

					pWndSettings->m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um2);

					if ((m_d == 0 || m_d == 1))
					{
						if (m_pWFExam1) CreateChildWnd();
					}
					else CreateChildWnd();
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

		for (m_d = 0; m_d < 4; m_d++)
		{
			CWndSettings* pWndSettings = GetWndSettings();

			pWndSettings->m_ZoneRadiusUm = newZoneRadiusUm;

			if ((m_d == 0 || m_d == 1))
			{
				if (m_pWFExam1) CreateChildWnd();
			}
			else CreateChildWnd();
		}

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
LRESULT CWFDoubleSoloWnd::OnSliderChangedPos(WPARAM wParam, LPARAM lParam)
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

				for (m_d = 0; m_d < 4; m_d++)
				{
					CWndSettings* pWndSettings = GetWndSettings();

					pWndSettings->m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um1);

					if ((m_d == 2 || m_d == 3))
					{
						if (m_pWFExam2) CreateChildWnd();
					}
					else CreateChildWnd();
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

		for (m_d = 0; m_d < 4; m_d++)
		{
			CWndSettings* pWndSettings = GetWndSettings();

			pWndSettings->m_ZoneRadiusUm = newZoneRadiusUm;

			if ((m_d == 2 || m_d == 3))
			{
				if (m_pWFExam2)
					CreateChildWnd();
			}
			else
				CreateChildWnd();
		}

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

				for (m_d = 0; m_d < 4; m_d++)
				{
					CWndSettings* pWndSettings = GetWndSettings();

					pWndSettings->m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um2);

					if ((m_d == 0 || m_d == 1))
					{
						if (m_pWFExam1) CreateChildWnd();
					}
					else CreateChildWnd();
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

		for (m_d = 0; m_d < 4; m_d++)
		{
			CWndSettings* pWndSettings = GetWndSettings();

			pWndSettings->m_ZoneRadiusUm = newZoneRadiusUm;

			if ((m_d == 0 || m_d == 1))
			{
				if (m_pWFExam1) CreateChildWnd();
			}
			else CreateChildWnd();
		}

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
void CWFDoubleSoloWnd::OnSunButton1Clicked()
{
	OnSunButtonClicked();
}

//***************************************************************************************
void CWFDoubleSoloWnd::OnMoonButton1Clicked()
{
	OnMoonButtonClicked();
}

//***************************************************************************************

void CWFDoubleSoloWnd::OnToggleButton1Clicked()
{
	CBusyCursor Cursor;

	CWndSettings* pWndSettings = GetWndSettings();

	if (m_pWndSettings[0].m_Type == TYPE_TLTR && m_pWndSettings[1].m_Type == TYPE_TLTR)
	{
		m_d = 0;
		pWndSettings = GetWndSettings();
		pWndSettings->m_Type = TYPE_TRFM;
		CreateChildWnd();

		m_d = 1;
		pWndSettings = GetWndSettings();
		pWndSettings->m_Type = TYPE_TRFM;
		CreateChildWnd();

		m_ToggleButton1.SetImage(IDR_SOLO_E_MAP);
		m_ToggleButton1.SetTip("Switch to Visual Acuity");
	}
	else
	{
		m_d = 0;
		pWndSettings = GetWndSettings();
		pWndSettings->m_Type = TYPE_TLTR;
		CreateChildWnd();

		m_d = 1;
		pWndSettings = GetWndSettings();
		pWndSettings->m_Type = TYPE_TLTR;
		CreateChildWnd();

		m_ToggleButton1.SetImage(IDR_SOLO_E_MAP);
		m_ToggleButton1.SetTip("Switch to map");
	}

	Repaint();
}

//***************************************************************************************


void CWFDoubleSoloWnd::OnSunButton2Clicked()
{
	OnSunButtonClicked();
}

//***************************************************************************************

void CWFDoubleSoloWnd::OnMoonButton2Clicked()
{
	OnMoonButtonClicked();
}

//***************************************************************************************

void CWFDoubleSoloWnd::OnToggleButton2Clicked()
{
	CBusyCursor Cursor;

	CWndSettings* pWndSettings = GetWndSettings();

	if (m_pWndSettings[2].m_Type == TYPE_TLTR && m_pWndSettings[3].m_Type == TYPE_TLTR)
	{
		m_d = 2;
		pWndSettings = GetWndSettings();
		pWndSettings->m_Type = TYPE_TRFM;
		CreateChildWnd();

		m_d = 3;
		pWndSettings = GetWndSettings();
		pWndSettings->m_Type = TYPE_TRFM;
		CreateChildWnd();

		m_ToggleButton2.SetImage(IDR_SOLO_E_MAP);
		m_ToggleButton2.SetTip("Switch to Visual Acuity");
	}
	else
	{
		m_d = 2;
		pWndSettings = GetWndSettings();
		pWndSettings->m_Type = TYPE_TLTR;
		CreateChildWnd();

		m_d = 3;
		pWndSettings = GetWndSettings();
		pWndSettings->m_Type = TYPE_TLTR;
		CreateChildWnd();

		m_ToggleButton2.SetImage(IDR_SOLO_E_MAP);
		m_ToggleButton2.SetTip("Switch to map");
	}

	Repaint();
}


//***************************************************************************************

void CWFDoubleSoloWnd::OnSunButtonClicked()
{
	CBusyCursor Cursor;

	if (m_pWFExam1)
	{
		m_scan1.Format(_T("%.2f mm"), ::TempSettings.Com_r_max_um1*0.002);
		for (m_d = 0; m_d < 2; m_d++)
		{
			CWndSettings* pWndSettings = GetWndSettings();

			pWndSettings->m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um1);

			CreateChildWnd();
		}
		m_ODSlider.m_Pos = m_ODSliderMaxPos;
		m_ODSlider.OnPaint();
	}


	if (m_pWFExam2)
	{
		m_scan2.Format(_T("%.2f mm"), ::TempSettings.Com_r_max_um2*0.002);
		for (m_d = 2; m_d < 4; m_d++)
		{
			CWndSettings* pWndSettings = GetWndSettings();

			pWndSettings->m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um2);

			CreateChildWnd();
		}
		m_OSSlider.m_Pos = m_OSSliderMaxPos;
		m_OSSlider.OnPaint();
	}

	Repaint();
}

//***************************************************************************************

void CWFDoubleSoloWnd::OnMoonButtonClicked()
{
	CBusyCursor Cursor;

	if (m_pWFExam1)
	{
		m_scan1 = "1.00 mm";
		for (m_d = 0; m_d < 2; m_d++)
		{
			CWndSettings* pWndSettings = GetWndSettings();

			pWndSettings->m_ZoneRadiusUm = 500;

			CreateChildWnd();
		}
		m_ODSlider.m_Pos = 0;
		m_ODSlider.OnPaint();
	}

	if (m_pWFExam2)
	{
		m_scan2 = "1.00 mm";
		for (m_d = 2; m_d < 4; m_d++)
		{
			CWndSettings* pWndSettings = GetWndSettings();

			pWndSettings->m_ZoneRadiusUm = 500;

			CreateChildWnd();
		}

		m_OSSlider.m_Pos = 0;
		m_OSSlider.OnPaint();
	}

	Repaint();
}

//***************************************************************************************
//************ For highlight the hyperlinker box ****************************************
//***************************************************************************************

void CWFDoubleSoloWnd::OnTimer(UINT_PTR nIDEvent)
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

void CWFDoubleSoloWnd::OnMouseMove(uint nFlags, CPoint Point)
{
	CPoint point;
	GetCursorPos(&point);

	if (point.x > m_Rect[10].left && point.x < m_Rect[10].right
		&& point.y > m_Rect[10].top  && point.y < m_Rect[10].bottom
		&& m_pWFExam1)
	{
		::SetCursor(::LoadCursor(NULL, IDC_HAND));

		int Infox, Infoy;
		Infox = point.x + 20;
		Infoy = point.y - 20;
		m_NoteInfo.MoveWindow(Infox, Infoy, 160, 20, 1);

		if (m_HighLight != 1)
		{
			m_NoteInfo.ShowWindow(TRUE);
			m_NoteInfo.Invalidate(FALSE);
			m_HighLight = 1;
			Repaint();
			SetTimer(SOLOTIMER, 1300, 0);
		}
		m_NoteInfo.Repaint();
	}
	else if (point.x > m_Rect[11].left && point.x < m_Rect[11].right
		&& point.y > m_Rect[11].top  && point.y < m_Rect[11].bottom
		&& m_pWFExam2)
	{
		::SetCursor(::LoadCursor(NULL, IDC_HAND));
		int Infox, Infoy;
		Infox = point.x - 170;
		Infoy = point.y - 20;
		m_NoteInfo.MoveWindow(Infox, Infoy, 160, 20, 1);

		if (m_HighLight != 2)
		{
			m_NoteInfo.ShowWindow(TRUE);
			m_NoteInfo.Invalidate(FALSE);
			m_HighLight = 2;
			Repaint();
			SetTimer(SOLOTIMER, 1300, 0);
		}
		m_NoteInfo.Repaint();
	}
	else
	{
		m_NoteInfo.ShowWindow(FALSE);

		if (m_HighLight != 0)
		{
			m_HighLight = 0;
			Repaint();
		}
	}
}

//***************************************************************************************

void CWFDoubleSoloWnd::OnLButtonDown(uint nFlags, CPoint Point)
{
	CPoint point;
	GetCursorPos(&point);

	if (point.x > m_Rect[10].left && point.x < m_Rect[10].right
		&& point.y > m_Rect[10].top  && point.y < m_Rect[10].bottom
		&& m_pWFExam1)
	{
		GetParent()->PostMessage(WM_SOLO_HYPELINK, 0, 0);
	}

	if (point.x > m_Rect[11].left && point.x < m_Rect[11].right
		&& point.y > m_Rect[11].top  && point.y < m_Rect[11].bottom
		&& m_pWFExam2)
	{
		GetParent()->PostMessage(WM_SOLO_HYPELINK, 0, 1);
	}
}

//***************************************************************************************
//************ For highlight the hyperlinker box  Done***********************************
//***************************************************************************************


//***************************************************************************************
//new
void CWFDoubleSoloWnd::OnShowDefaultItemClicked()
{
	CBusyCursor Cursor;

	m_pWndSettings[m_d].m_Type = TYPE_TLTR;

	m_pWndSettings[m_d].m_LtrDistanceType = 0;//530 

	if (m_d == 0 || m_d == 2)
	{
		m_pWndSettings[m_d].m_Mask.SetType(MASK_TOTAL);
	}
	if (m_d == 1 || m_d == 3)
	{
		m_pWndSettings[m_d].m_Mask.SetType(MASK_HO_TOTAL);
	}

	m_pWndSettings[m_d].m_Ltr = 0;

	m_pWndSettings[m_d].m_LtrLine = 70;

	m_pWndSettings[m_d].m_LtrOrientation = 0;

	CreateChildWnd();

	Repaint();
}
//new
//***************************************************************************************
//new
BOOL CWFDoubleSoloWnd::SameRefSet()
{
	if (m_pWndSettings[m_d].m_Type != TYPE_TLTR)        return FALSE;

	if (m_d == 0 || m_d == 2)
	{
		if (m_pWndSettings[m_d].m_Mask.GetType() != MASK_TOTAL)      return FALSE;
	}
	if (m_d == 1 || m_d == 3)
	{
		if (m_pWndSettings[m_d].m_Mask.GetType() != MASK_HO_TOTAL)   return FALSE;
	}

	if (m_pWndSettings[m_d].m_Ltr != 0)               return FALSE;

	if (m_pWndSettings[m_d].m_LtrLine != 70)              return FALSE;

	if (m_pWndSettings[m_d].m_LtrOrientation != 0)               return FALSE;

	return TRUE;
}
//new
//***************************************************************************************

//***************************************************************************************
//521
void CWFDoubleSoloWnd::OnSelEyeImgItemClicked()
{
	int type = (m_d == 0 ? 0 : 1);

	CSelectImgDlg* pDlg = new CSelectImgDlg(this, type);

	if (pDlg->DoModal() == IDOK)
	{
		CWFExam* SelectWFExam = new CWFExam;
		::DB.LoadWFExam(pDlg->m_Sel_ID, SelectWFExam);

		if (type == 0)
		{
			m_pWFExam1->m_Header.m_Note = "Averaged Exam";
			m_pWFExam1->m_Image = SelectWFExam->m_Image;
			m_pWFExam1->Process();

			// for average exam 
			CString ExamId, LoadFileName;
			GUIDToStr(m_pWFExam1->m_Header.m_ExamID, ExamId);
			LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

			if (::PathFileExists(LoadFileName))
			{
				CAveExamInfo AveExamInfo;

				AveExamInfo.LoadFromFile(LoadFileName);
				for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
				{
					m_pWFExam1->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
				}
			}
			// for average exam Done
		}
		else
		{
			m_pWFExam2->m_Header.m_Note = "Averaged Exam";
			m_pWFExam2->m_Image = SelectWFExam->m_Image;
			m_pWFExam2->Process();

			// for average exam 
			CString ExamId, LoadFileName;
			GUIDToStr(m_pWFExam2->m_Header.m_ExamID, ExamId);
			LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

			if (::PathFileExists(LoadFileName))
			{
				CAveExamInfo AveExamInfo;

				AveExamInfo.LoadFromFile(LoadFileName);
				for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
				{
					m_pWFExam2->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
				}
			}
			// for average exam Done
		}

		SAFE_DELETE(SelectWFExam);

		delete pDlg;

		m_d = 0;

		CWndSettings* pWndSettings = GetWndSettings();
		pWndSettings->m_MapShowEye = TRUE;

		CreateChildWnd();

		Repaint();
	}
	else delete pDlg;
}
//521
//***************************************************************************************
