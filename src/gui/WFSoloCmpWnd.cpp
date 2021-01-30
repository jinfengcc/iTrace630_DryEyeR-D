//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "WFSoloCmpWnd.h"
#include "EyeWnd.h"
#include "PSFWnd.h"
#include "MTFWnd.h"
#include "EEFWnd.h"
#include "LtrWnd.h"
#include "RMSWnd.h"
#include "ChartWnd.h"
#include "BusyCursor.h"
#include "SelectImgDlg.h"//new
#include "NearLtrWnd.h"//530

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CWFSoloCmpWnd, CSumWnd)

	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()

	ON_MESSAGE(WM_ROTATE, OnRotateMsg)
	ON_MESSAGE(WM_CHANGE_CENT, OnChangeCentMsg)
	ON_MESSAGE(WM_CHANGE_STEP, OnChangeStepMsg)
	ON_MESSAGE(WM_CHANGE_MAX, OnChangeMaxMsg)

	ON_MESSAGE(WM_SLIDER_LCLICK, OnSliderClick)
	ON_MESSAGE(WM_SLIDER_POS_CHANGED, OnSliderChangedPos)


	ON_BN_CLICKED(IDC_SOLO_SUN_ITEM1, OnSunButton1Clicked)
	ON_BN_CLICKED(IDC_SOLO_MOON_ITEM1, OnMoonButton1Clicked)
	ON_BN_CLICKED(IDC_SOLO_TOGGLE_ITEM1, OnToggleButtonClicked)

	ON_BN_CLICKED(IDC_SOLO_SUN_ITEM2, OnSunButton2Clicked)
	ON_BN_CLICKED(IDC_SOLO_MOON_ITEM2, OnMoonButton2Clicked)
	ON_BN_CLICKED(IDC_SOLO_TOGGLE_ITEM2, OnToggleButtonClicked)

	ON_BN_CLICKED(IDC_SOLO_SUN_ITEM3, OnSunButton3Clicked)
	ON_BN_CLICKED(IDC_SOLO_MOON_ITEM3, OnMoonButton3Clicked)
	ON_BN_CLICKED(IDC_SOLO_TOGGLE_ITEM3, OnToggleButtonClicked)

	ON_COMMAND(IDC_DEFAULT_GUI_ITEM, OnShowDefaultItemClicked)//new
	ON_COMMAND(IDC_SELECT_IMAGE_ITEM, OnSelEyeImgItemClicked)//new

	ON_BN_CLICKED(IDC_SHOW_FULL_INFO, OnShowWFTextInfoBtnClicked)//530
	ON_BN_CLICKED(IDC_SHOW_FULL_INFO2, OnShowWFTextInfoBtn2Clicked)//530
	ON_BN_CLICKED(IDC_SHOW_FULL_INFO3, OnShowWFTextInfoBtn3Clicked)//530

END_MESSAGE_MAP()

//***************************************************************************************

CWFSoloCmpWnd::CWFSoloCmpWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CWFExam* pWFExam1, CWFExam* pWFExam2, CWndSettings* pWndSettings) :
	CSumWnd(pWnd, WndRect, pPatient, pWndSettings)
{
	CBusyCursor Cursor;

	//----------------------------------------------------
	RECT Rect;
	GetWindowRect(&Rect);
	real_t t = 3.0 * (0.03 * m_h) + m_g;
	real_t w = ((Rect.right - Rect.left) - 4.0 * m_g) / 3.0;
	real_t h = ((Rect.bottom - Rect.top - t) - 2.0 * m_g) / 2.0;
	real_t w1 = 0.85*w;

	m_mapTop = intRound(t);
	m_mapBtm = intRound(t + h + m_g + h);
	m_SplX = intRound(m_g + w + m_g + w + m_g / 2.0);

	::SetRect(&m_Rect[0], intRound(m_g), intRound(t), intRound(m_g + w1), intRound(t + h)); // exam 1 map
	::SetRect(&m_Rect[1], intRound(m_g + w + m_g), intRound(t), intRound(m_g + w + m_g + w1), intRound(t + h)); // exam 2 map
	::SetRect(&m_Rect[2], intRound(m_g + w + m_g + w + m_g), intRound(t), intRound(m_g + w + m_g + w + m_g + w1), intRound(t + h)); // diff map
	::SetRect(&m_Rect[3], intRound(m_g), intRound(t + h + m_g), intRound(m_g + w), intRound(t + h + m_g + h)); // exam 1 info
	::SetRect(&m_Rect[4], intRound(m_g + w + m_g), intRound(t + h + m_g), intRound(m_g + w + m_g + w), intRound(t + h + m_g + h)); // exam 2 info
	::SetRect(&m_Rect[5], intRound(m_g + w + m_g + w + m_g), intRound(t + h + m_g), intRound(m_g + w + m_g + w + m_g + w), intRound(t + h + h + m_g)); // diff info

	::SetRect(&m_Rect[6], intRound(m_g + w1), intRound(t), intRound(m_g + w), intRound(t + h)); //slider1 rect
	::SetRect(&m_Rect[7], intRound(m_g + w + m_g + w1), intRound(t), intRound(m_g + w + m_g + w), intRound(t + h)); //slider2 rect
	::SetRect(&m_Rect[8], intRound(m_g + w + m_g + w + m_g + w1), intRound(t), intRound(m_g + w + m_g + w + m_g + w), intRound(t + h)); //slider3 rect
																																		//----------------------------------------------------

	m_pWFExam1 = pWFExam1;
	m_pWFExam2 = pWFExam2;

	m_OD = m_pWFExam1->m_Header.IsOD();//new

									   //*007*[cjf***05052012],record the increase and decrease range
	::TempSettings.Com_r_max_um = ((m_pWFExam1->m_WfSurface.m_r_max_um) < (m_pWFExam2->m_WfSurface.m_r_max_um) ? m_pWFExam1->m_WfSurface.m_r_max_um : m_pWFExam2->m_WfSurface.m_r_max_um);
	pWndSettings->m_ZoneRadiusUm = 5000;
	//*007*[cjf***05052012]

	//Sliders
	m_scan.Format(_T("%.2f mm"), ::TempSettings.Com_r_max_um*0.002);

	int  IconWide = 9;
	real_t w2 = 0.15*w;
	int sliderTop = intRound(t + 0.2*h);
	int sliderBtm = intRound(t + 0.65*h);

	int slider1Left = intRound(m_g + w1 + 0.25*w2);
	int slider1Right = intRound(m_g + w1 + 0.75*w2);

	int slider2Left = slider1Left + intRound(w + m_g);
	int slider2Right = slider1Right + intRound(w + m_g);

	int slider3Left = slider2Left + intRound(w + m_g);
	int slider3Right = slider2Right + intRound(w + m_g);

	m_Slider_Top = sliderTop;
	m_slider1Right = slider1Right;
	m_slider3Left = slider3Left;

	::SetRect(&m_SliderRects[0], slider1Left, sliderTop, slider1Right, sliderBtm); //slider[0] rect
	::SetRect(&m_SliderRects[1], slider1Left, sliderTop - intRound(2.5*IconWide), slider1Right, sliderTop - intRound(0.5*IconWide)); //Sun
	::SetRect(&m_SliderRects[2], slider1Left, sliderBtm + intRound(0.8*IconWide), slider1Right, sliderBtm + intRound(2.8*IconWide)); //Moon
	::SetRect(&m_SliderRects[3], intRound(m_g + w1), sliderTop - intRound(5.5*IconWide), intRound(m_g + w), sliderTop - intRound(3.0*IconWide)); //Scan val

	::SetRect(&m_SliderRects[4], slider2Left, sliderTop, slider2Right, sliderBtm); //slider[1] rect
	::SetRect(&m_SliderRects[5], slider2Left, sliderTop - intRound(2.5*IconWide), slider2Right, sliderTop - intRound(0.5*IconWide)); //Sun
	::SetRect(&m_SliderRects[6], slider2Left, sliderBtm + intRound(0.8*IconWide), slider2Right, sliderBtm + intRound(2.8*IconWide)); //Moon
	::SetRect(&m_SliderRects[7], intRound(m_g + w + m_g + w1), sliderTop - intRound(5.5*IconWide), intRound(m_g + w + m_g + w), sliderTop - intRound(3.0*IconWide)); //Scan val

	::SetRect(&m_SliderRects[8], slider3Left, sliderTop, slider3Right, sliderBtm); //slider[2] rect
	::SetRect(&m_SliderRects[9], slider3Left, sliderTop - intRound(2.5*IconWide), slider3Right, sliderTop - intRound(0.5*IconWide)); //Sun
	::SetRect(&m_SliderRects[10], slider3Left, sliderBtm + intRound(0.8*IconWide), slider3Right, sliderBtm + intRound(2.8*IconWide)); //Moon
	::SetRect(&m_SliderRects[11], intRound(m_g + w + m_g + w + m_g + w1), sliderTop - intRound(5.5*IconWide), intRound(m_g + w + m_g + w + m_g + w), sliderTop - intRound(3.0*IconWide)); //Scan val

	::SetRect(&m_SliderRects[12], slider1Left, sliderBtm + intRound(4 * IconWide), slider1Right, sliderBtm + intRound(10 * IconWide)); //Toggle button
	::SetRect(&m_SliderRects[13], slider2Left, sliderBtm + intRound(4 * IconWide), slider2Right, sliderBtm + intRound(10 * IconWide)); //
	::SetRect(&m_SliderRects[14], slider3Left, sliderBtm + intRound(4 * IconWide), slider3Right, sliderBtm + intRound(10 * IconWide)); //

	if (m_pWFExam1)
	{
		m_Slider[0].Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_SliderRects[0], this, IDC_SOLO_SLIDER0);

		m_Slider[0].m_Pos = 1;

		m_Slider[0].OnPaint();

		m_SunButton1.Create(_T(""), _T(""), 0, m_SliderRects[1], this, IDC_SOLO_SUN_ITEM1);
		m_SunButton1.SetImage(IDR_SOLO_MOON);
		m_SunButton1.SetBk(FALSE);
		m_SunButton1.SetBkColor(BLACK);
		m_SunButton1.ShowWindow(SW_SHOW);

		m_MoonButton1.Create(_T(""), _T(""), 0, m_SliderRects[2], this, IDC_SOLO_MOON_ITEM1);
		m_MoonButton1.SetImage(IDR_SOLO_SUN);
		m_MoonButton1.SetBk(FALSE);
		m_MoonButton1.SetBkColor(BLACK);
		m_MoonButton1.ShowWindow(SW_SHOW);

		m_ToggleButton1.Create(_T(""), _T(""), 0, m_SliderRects[12], this, IDC_SOLO_TOGGLE_ITEM1);
		if (pWndSettings->m_Type == TYPE_TLTR)
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
		m_Slider[1].Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_SliderRects[4], this, IDC_SOLO_SLIDER1);
		m_Slider[1].m_Pos = 1;
		m_Slider[1].OnPaint();

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

		m_ToggleButton2.Create(_T(""), _T(""), 0, m_SliderRects[13], this, IDC_SOLO_TOGGLE_ITEM2);
		if (pWndSettings->m_Type == TYPE_TLTR)
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

	if (m_pWFExam1 && m_pWFExam2)
	{
		m_Slider[2].Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_SliderRects[8], this, IDC_SOLO_SLIDER2);
		m_Slider[2].m_Pos = 1;
		m_Slider[2].OnPaint();

		m_SunButton3.Create(_T(""), _T(""), 0, m_SliderRects[9], this, IDC_SOLO_SUN_ITEM3);
		m_SunButton3.SetImage(IDR_SOLO_MOON);
		m_SunButton3.SetBk(FALSE);
		m_SunButton3.SetBkColor(BLACK);
		m_SunButton3.ShowWindow(SW_SHOW);

		m_MoonButton3.Create(_T(""), _T(""), 0, m_SliderRects[10], this, IDC_SOLO_MOON_ITEM3);
		m_MoonButton3.SetImage(IDR_SOLO_SUN);
		m_MoonButton3.SetBk(FALSE);
		m_MoonButton3.SetBkColor(BLACK);
		m_MoonButton3.ShowWindow(SW_SHOW);

		m_ToggleButton3.Create(_T(""), _T(""), 0, m_SliderRects[14], this, IDC_SOLO_TOGGLE_ITEM3);
		if (pWndSettings->m_Type == TYPE_TLTR)
		{
			m_ToggleButton3.SetImage(IDR_SOLO_E_MAP);
			m_ToggleButton3.SetTip("Switch to map");
		}
		else
		{
			m_ToggleButton3.SetImage(IDR_SOLO_E_MAP);
			m_ToggleButton3.SetTip("Switch to Visual Acuity");
		}
		m_ToggleButton3.SetBk(FALSE);
		m_ToggleButton3.SetBkColor(BLACK);
		m_ToggleButton3.ShowWindow(SW_SHOW);
	}

	int  SNum = (int)(::TempSettings.Com_r_max_um*0.002);
	real_t rest = ::TempSettings.Com_r_max_um - intRound(::TempSettings.Com_r_max_um) % 500;
	real_t pos = 1 - (::TempSettings.Com_r_max_um - rest) / (::TempSettings.Com_r_max_um - 500);
	if (m_pWFExam1)
	{
		m_Slider[0].m_scaleNum = SNum;
		m_Slider[0].m_FirstPos = pos;
		m_Slider[0].OnPaint();
	}

	if (m_pWFExam2)
	{
		m_Slider[1].m_scaleNum = SNum;
		m_Slider[1].m_FirstPos = pos;
		m_Slider[1].OnPaint();
	}

	if (m_pWFExam1 && m_pWFExam2)
	{
		m_Slider[2].m_scaleNum = SNum;
		m_Slider[2].m_FirstPos = pos;
		m_Slider[2].OnPaint();
	}
	//Done

	CreateChildWnd();
	Repaint();

	//The 'transform funtionality' info ("Link to Aberration analysis")
	m_HighLight = 0;

	RECT InfoRect;
	::SetRect(&InfoRect, 0, 0, 160, 20);
	m_NoteInfo.CreateWnd(InfoRect, this);
	m_NoteInfo.ClearMemDC();

	CMFont Font(15, 400, "Arial");
	m_NoteInfo.m_MemDC.WriteText("  Link to Refraction analysis", InfoRect, Font, BLACK, 0, NOCOLOR);
	m_NoteInfo.ShowWindow(FALSE);
	//Done

	//530 Draw the WF Text Info button
	if (m_pWFExam1)
	{
		CString s = (m_pWndSettings[0].m_SimplifiedWFPanel ? "Show Details" : "Hide Details");
		::SetRect(&Rect, m_Rect[3].right - 25, m_Rect[3].top + 5, m_Rect[3].right - 5, m_Rect[3].top + 20);
		m_WFShowFullInfoButton.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_SHOW_FULL_INFO);
		m_WFShowFullInfoButton.SetImage(m_pWndSettings[0].m_SimplifiedWFPanel ?
			(m_SW >= 1920 ? IDR_SHOW_DETAIL_3 : m_SW >= 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
			(m_SW >= 1920 ? IDR_HIDE_DETAIL_3 : m_SW >= 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
		m_WFShowFullInfoButton.SetBk(FALSE);
		m_WFShowFullInfoButton.SetBkColor(BLACK);
		m_WFShowFullInfoButton.SetTip(s);
		m_WFShowFullInfoButton.ShowWindow(SW_SHOW);
	}

	if (m_pWFExam2)
	{
		CString s = (m_pWndSettings[0].m_SimplifiedWFPanel ? "Show Details" : "Hide Details");
		::SetRect(&Rect, m_Rect[4].right - 25, m_Rect[4].top + 5, m_Rect[4].right - 5, m_Rect[4].top + 20);
		m_WFShowFullInfoButton2.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_SHOW_FULL_INFO2);
		m_WFShowFullInfoButton2.SetImage(m_pWndSettings[0].m_SimplifiedWFPanel ?
			(m_SW >= 1920 ? IDR_SHOW_DETAIL_3 : m_SW >= 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
			(m_SW >= 1920 ? IDR_HIDE_DETAIL_3 : m_SW >= 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
		m_WFShowFullInfoButton2.SetBk(FALSE);
		m_WFShowFullInfoButton2.SetBkColor(BLACK);
		m_WFShowFullInfoButton2.SetTip(s);
		m_WFShowFullInfoButton2.ShowWindow(SW_SHOW);
	}

	if (m_pWFExam1 && m_pWFExam2)
	{
		CString s = (m_pWndSettings[0].m_SimplifiedWFPanel ? "Show Details" : "Hide Details");
		::SetRect(&Rect, m_Rect[5].right - 25, m_Rect[5].top + 5, m_Rect[5].right - 5, m_Rect[5].top + 20);
		m_WFShowFullInfoButton3.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_SHOW_FULL_INFO2);
		m_WFShowFullInfoButton3.SetImage(m_pWndSettings[0].m_SimplifiedWFPanel ?
			(m_SW >= 1920 ? IDR_SHOW_DETAIL_3 : m_SW >= 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
			(m_SW >= 1920 ? IDR_HIDE_DETAIL_3 : m_SW >= 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
		m_WFShowFullInfoButton3.SetBk(FALSE);
		m_WFShowFullInfoButton3.SetBkColor(BLACK);
		m_WFShowFullInfoButton3.SetTip(s);
		m_WFShowFullInfoButton3.ShowWindow(SW_SHOW);
	}
	//530 Done

	ShowWindow(SW_SHOW);
}

//***************************************************************************************

void CWFSoloCmpWnd::RepaintMemDC()
{
	CSumWnd::RepaintMemDC();

	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	BOOL Rows[22] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

	BOOL HighLight = FALSE;

	CMFont FontScanSize(intRound(1.8*m_g), 400, "Arial");

	if (m_pWFExam1) {
		HighLight = TRUE ? m_HighLight == 1 : FALSE;

		if (m_pWndSettings[0].m_SimplifiedWFPanel == 1)
			ChangWFExamInfo(m_pWFExam1, m_Rect[3], 1, TRUE);
		else SoloCmpWFExamInfo(m_pWFExam1, m_Rect[3], 22, Rows, HighLight);

		m_MemDC.DrawRectangle(m_Rect[6], gray, NOCOLOR);
		m_MemDC.WriteText(m_scan, m_SliderRects[3], FontScanSize, RED, 1, NOCOLOR);
	}

	if (m_pWFExam2) {
		HighLight = TRUE ? m_HighLight == 2 : FALSE;
		if (m_pWndSettings[0].m_SimplifiedWFPanel == 1)
			ChangWFExamInfo(m_pWFExam2, m_Rect[4], 1, TRUE);
		else SoloCmpWFExamInfo(m_pWFExam2, m_Rect[4], 22, Rows, HighLight);

		m_MemDC.DrawRectangle(m_Rect[7], gray, NOCOLOR);
		m_MemDC.WriteText(m_scan, m_SliderRects[7], FontScanSize, RED, 1, NOCOLOR);
	}

	if (m_pWFExam1 && m_pWFExam2)
	{
		m_MemDC.DrawRectangle(m_Rect[8], gray, NOCOLOR);
		m_MemDC.WriteText(m_scan, m_SliderRects[11], FontScanSize, RED, 1, NOCOLOR);
		m_MemDC.DrawLine(m_SplX, m_mapTop, m_SplX, m_mapBtm, 2, white);
	}

	CWFExam* pWFExam3 = new CWFExam;
	pWFExam3->m_WfSurface = m_Surfaces[3];
	Rows[0] = Rows[1] = Rows[2] = Rows[3] = Rows[4] = Rows[5] = 0;
	if (m_pWndSettings[0].m_SimplifiedWFPanel == 1)
		ChangWFExamInfo(pWFExam3, m_Rect[5], 0, TRUE);
	else   WFExamInfo(pWFExam3, m_Rect[5], 22, Rows);
	delete pWFExam3;
}

//***************************************************************************************

void CWFSoloCmpWnd::CreateChildWnd()
{
	for (int d = 0; d < 3; d++) SAFE_DELETE(m_pDispWnd[d]);

	CWndSettings* pWndSettings = GetWndSettings();


	{
		// type
		if (pWndSettings->m_Type != TYPE_TWFM &&
			pWndSettings->m_Type != TYPE_TRFM &&
			pWndSettings->m_Type != TYPE_TLTR )
		{
			pWndSettings->m_Type = TYPE_TWFM;
		}

		// mask
		int MaskType = pWndSettings->m_Mask.GetType();

		if (MaskType != MASK_TOTAL &&
			MaskType != MASK_HO_TOTAL) 
		{
			pWndSettings->m_Mask.SetType(MASK_TOTAL);
			pWndSettings->m_Piston = TRUE;
			pWndSettings->m_Tilt = TRUE;
		}

		// colors
		if (pWndSettings->m_Type == TYPE_TWFM ||
			pWndSettings->m_Type == TYPE_TRFM) 
		{
			if (!pWndSettings->AreDefaultColors()) 
			{
				pWndSettings->SetDefaultColors(1);
			}
		}

		// psf
		if (pWndSettings->m_Type == TYPE_TPSF) 
		{
			pWndSettings->m_ShowEEF = FALSE;
		}
	}

	// type
	if (pWndSettings->m_Type != TYPE_TWFM &&
		pWndSettings->m_Type != TYPE_TRFM &&
		pWndSettings->m_Type != TYPE_TPSF &&
		pWndSettings->m_Type != TYPE_TMTF &&
		pWndSettings->m_Type != TYPE_TLTR &&
		pWndSettings->m_Type != TYPE_TEEF &&
		pWndSettings->m_Type != TYPE_TRMS &&
		pWndSettings->m_Type != TYPE_TAZR ) 
	{
		pWndSettings->m_Type = TYPE_TWFM;
	}

	if (pWndSettings->m_Type == TYPE_TRMS) 
	{
		if (pWndSettings->m_Method3D)
		{
			pWndSettings->m_RMSIndividual = TRUE;
		}
	}
	else if (pWndSettings->m_Type == TYPE_TLTR)
	{
		if (!(pWndSettings->m_Ltr == 0 || pWndSettings->m_Ltr == 1)) 
		{
			pWndSettings->m_Ltr = 0;
		}
		if (!(pWndSettings->m_LtrLine == 20 || pWndSettings->m_LtrLine == 25 || pWndSettings->m_LtrLine == 30 ||
			pWndSettings->m_LtrLine == 40 || pWndSettings->m_LtrLine == 50 || pWndSettings->m_LtrLine == 70 ||
			pWndSettings->m_LtrLine == 100 || pWndSettings->m_LtrLine == 200)) 
		{
			pWndSettings->m_LtrLine = 40;
		}
		if (!(pWndSettings->m_LtrOrientation == 0 || pWndSettings->m_LtrOrientation == 90 ||
			pWndSettings->m_LtrOrientation == 180 || pWndSettings->m_LtrOrientation == 270)) 
		{
			pWndSettings->m_LtrOrientation = 0;
		}
	}
	

	CWFExam* pWFExam[3] = { m_pWFExam1, m_pWFExam2, NULL };

	m_Surfaces[0] = pWFExam[0]->m_WfSurface;
	m_Surfaces[1] = pWFExam[1]->m_WfSurface;

	// change diameter
	real_t r_max_um = 5000.0;
	r_max_um = __min(r_max_um, m_Surfaces[0].m_r_max_um);
	r_max_um = __min(r_max_um, m_Surfaces[1].m_r_max_um);
	r_max_um = __min(r_max_um, pWndSettings->m_ZoneRadiusUm);
	m_Surfaces[0].ChangeRMaxUm(r_max_um);
	m_Surfaces[1].ChangeRMaxUm(r_max_um);

	// mask
	m_Surfaces[0] &= pWndSettings->m_Mask;
	m_Surfaces[0].ComputePiston(pWndSettings->m_Piston);
	m_Surfaces[0].ComputeTilt(pWndSettings->m_Tilt);
	m_Surfaces[1] &= pWndSettings->m_Mask;
	m_Surfaces[1].ComputePiston(pWndSettings->m_Piston);
	m_Surfaces[1].ComputeTilt(pWndSettings->m_Tilt);

	// subtract
	m_Surfaces[2] = m_Surfaces[0];
	m_Surfaces[2] -= m_Surfaces[1];

	
	CZernikeSurface Surfaces[2];
	Surfaces[0] = m_pWFExam1->m_WfSurface;
	Surfaces[1] = m_pWFExam2->m_WfSurface;

	// change diameter
	Surfaces[0].ChangeRMaxUm(r_max_um);
	Surfaces[1].ChangeRMaxUm(r_max_um);

	// mask
	Surfaces[0].ComputePiston(pWndSettings->m_Piston);
	Surfaces[0].ComputeTilt(pWndSettings->m_Tilt);
	Surfaces[1].ComputePiston(pWndSettings->m_Piston);
	Surfaces[1].ComputeTilt(pWndSettings->m_Tilt);

	// subtract
	m_Surfaces[3] = Surfaces[0];
	m_Surfaces[3] -= Surfaces[1];
	

	for (int d = 0; d < 3; d++) 
	{
		RECT Rect = m_Rect[d];

		switch (pWndSettings->m_Type)
		{
			case TYPE_TWFM:
			{
				CEyeWnd* pEyeWnd = new CEyeWnd(Rect, this);
				pEyeWnd->m_Type = pWndSettings->m_Type;
				pEyeWnd->m_SizeSmall = TRUE;
				pEyeWnd->m_Method3D = pWndSettings->m_Method3D;
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
				pEyeWnd->m_Unit = "µ";
				pEyeWnd->m_Inc = pWndSettings->GetIncrement();
				CScale* pScale = pWndSettings->GetScale();
				pEyeWnd->m_NumColors = pScale->m_NumColors;

				for (int i = 0; i < pScale->m_NumColors; i++)
				{
					pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
				}

				pEyeWnd->m_pWFExam = pWFExam[d];

				if (pWndSettings->m_Method3D)
				{
					CWFExam::CreateWfm3D(pEyeWnd->m_Map3D, m_Surfaces[d]);
				}
				else 
				{
					CWFExam::CreateWfm2D(pEyeWnd->m_Map2D, m_Surfaces[d], 1.0 / pEyeWnd->m_y_px_um);
				}

				pEyeWnd->CreateCentButtons();
				pEyeWnd->CreateZoomButtons();
				pEyeWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 0);
				//pEyeWnd->CreateZoneButtons();
				m_pDispWnd[d] = pEyeWnd;
			}
			break;

			case TYPE_TRFM:
			{
				CEyeWnd* pEyeWnd = new CEyeWnd(Rect, this);
				pEyeWnd->m_Type = pWndSettings->m_Type;
				pEyeWnd->m_SizeSmall = TRUE;
				pEyeWnd->m_Method3D = pWndSettings->m_Method3D;
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
				pEyeWnd->m_Unit = "D";
				pEyeWnd->m_Inc = pWndSettings->GetIncrement();
				CScale* pScale = pWndSettings->GetScale();
				pEyeWnd->m_NumColors = pScale->m_NumColors;

				for (int i = 0; i < pScale->m_NumColors; i++) 
				{
					pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
				}

				pEyeWnd->m_pWFExam = pWFExam[d];

				if (pWndSettings->m_Method3D)
				{
					CWFExam::CreateRfm3D(pEyeWnd->m_Map3D, m_Surfaces[d]);
				}
				else
				{
					CWFExam::CreateRfm2D(pEyeWnd->m_Map2D, m_Surfaces[d], 1.0 / pEyeWnd->m_y_px_um);
				}

				pEyeWnd->CreateCentButtons();
				pEyeWnd->CreateZoomButtons();
				pEyeWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 0);

				m_pDispWnd[d] = pEyeWnd;
			}
			break;
		
			case TYPE_TLTR:
			{
				CLtrWnd* pLtrWnd = new CLtrWnd(Rect, this);
				pLtrWnd->m_DysVal = -1;//531
				CWFExam::CreateLtr(m_Surfaces[d], pLtrWnd->m_Ltr, pWndSettings->m_Ltr, pWndSettings->m_LtrLine, pWndSettings->m_LtrOrientation);
				pLtrWnd->m_Type = pWndSettings->m_Type;
				pLtrWnd->m_SizeSmall = TRUE;
				pLtrWnd->m_Method3D = pWndSettings->m_Method3D;
				//pLtrWnd->m_LTLabel[0].Format(_T("Letter 20 / %i", pWndSettings->m_LtrLine);
				pLtrWnd->m_LTLabel[0] = "Letter";//531
				pWndSettings->m_Mask.GetName(pLtrWnd->m_LTLabel[1]);
				pLtrWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
				pLtrWnd->CreateZoomButtons();
				//pLtrWnd->CreateZoneButtons();
				pLtrWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 0);
				m_pDispWnd[d] = pLtrWnd;
			}
			break;
		}
	}

	if (pWndSettings->m_Type == TYPE_TWFM || pWndSettings->m_Type == TYPE_TRFM) 
	{
		CScale* pScale = pWndSettings->GetScale();
		real_t Inc = pWndSettings->GetIncrement();
		real_t Cent = 0.0;
		real_t Step;

		if (pScale->m_StepAuto)
		{
			real_t Span = 0.0;

			for (int d = 0; d < 3; d++) 
			{
				CEyeWnd* pEyeWnd = (CEyeWnd*)m_pDispWnd[d];
				real_t Min = pWndSettings->m_Method3D ? pEyeWnd->m_Map3D.m_min : pEyeWnd->m_Map2D.m_min;
				real_t Max = pWndSettings->m_Method3D ? pEyeWnd->m_Map3D.m_max : pEyeWnd->m_Map2D.m_max;
				Span = __max(Span, 2.0 * __max(fabs(Min - Cent), fabs(Max - Cent)));
			}

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
		
		for (int d = 0; d < 3; d++)
		{
			CEyeWnd* pEyeWnd = (CEyeWnd*)m_pDispWnd[d];
			pEyeWnd->m_Cent = Cent;
			pEyeWnd->m_Step = Step;
		}
	}

	//
	if (pWndSettings->m_Type == TYPE_TLTR)
	{
		if (m_pWFExam1)
		{
			m_ToggleButton1.SetImage(IDR_SOLO_E_MAP);
			m_ToggleButton1.SetTip("Switch to map");
			m_ToggleButton1.Repaint();
		}
		if (m_pWFExam2)
		{
			m_ToggleButton2.SetImage(IDR_SOLO_E_MAP);
			m_ToggleButton2.SetTip("Switch to map");
			m_ToggleButton2.Repaint();
		}

		if (m_pWFExam1 && m_pWFExam2)
		{
			m_ToggleButton3.SetImage(IDR_SOLO_E_MAP);
			m_ToggleButton3.SetTip("Switch to map");
			m_ToggleButton3.Repaint();
		}
	}
	else
	{
		if (m_pWFExam1)
		{
			m_ToggleButton1.SetImage(IDR_SOLO_E_MAP);
			m_ToggleButton1.SetTip("Switch to Visual Acuity");
			m_ToggleButton1.Repaint();
		}

		if (m_pWFExam2)
		{
			m_ToggleButton2.SetImage(IDR_SOLO_E_MAP);
			m_ToggleButton2.SetTip("Switch to Visual Acuity");
			m_ToggleButton2.Repaint();
		}

		if (m_pWFExam1 && m_pWFExam2)
		{
			m_ToggleButton3.SetImage(IDR_SOLO_E_MAP);
			m_ToggleButton3.SetTip("Switch to Visual Acuity");
			m_ToggleButton3.Repaint();
		}
	}
}

//***************************************************************************************

LRESULT CWFSoloCmpWnd::OnRotateMsg(WPARAM wParam, LPARAM lParam)
{
	int ax, ay;

	for (int d = 0; d < 3; d++) {
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] == wParam) {
			ax = m_pDispWnd[d]->m_ax;
			ay = m_pDispWnd[d]->m_ay;
			break;
		}
	}

	for (int d = 0; d < 3; d++) {
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] != wParam) {
			m_pDispWnd[d]->m_ax = ax;
			m_pDispWnd[d]->m_ay = ay;
			m_pDispWnd[d]->Repaint();
		}
	}

	return 0;
}

//***************************************************************************************

LRESULT CWFSoloCmpWnd::OnChangeCentMsg(WPARAM wParam, LPARAM lParam)
{
	real_t Cent = INVALID_VALUE;

	for (int d = 0; d < 2; d++) {
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] == wParam) {
			Cent = ((CEyeWnd*)m_pDispWnd[d])->m_Cent;
			break;
		}
	}

	if (Cent == INVALID_VALUE) return 0;

	for (int d = 0; d < 2; d++) {
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] != wParam) {
			((CEyeWnd*)m_pDispWnd[d])->m_Cent = Cent;
			m_pDispWnd[d]->Repaint();
			break;
		}
	}

	return 0;
}

//***************************************************************************************

LRESULT CWFSoloCmpWnd::OnChangeStepMsg(WPARAM wParam, LPARAM lParam)
{
	real_t Step = INVALID_VALUE;

	for (int d = 0; d < 2; d++) {
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] == wParam) {
			Step = ((CEyeWnd*)m_pDispWnd[d])->m_Step;
			break;
		}
	}

	if (Step == INVALID_VALUE) return 0;

	for (int d = 0; d < 2; d++) {
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] != wParam) {
			((CEyeWnd*)m_pDispWnd[d])->m_Step = Step;
			m_pDispWnd[d]->Repaint();
			break;
		}
	}

	return 0;
}

//***************************************************************************************

LRESULT CWFSoloCmpWnd::OnChangeMaxMsg(WPARAM wParam, LPARAM lParam)
{
	int Max = INVALID_VALUE;

	for (int d = 0; d < 2; d++) {
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] == wParam) {
			Max = ((CRMSWnd*)m_pDispWnd[d])->m_Max;
			break;
		}
	}

	if (Max == INVALID_VALUE) return 0;

	for (int d = 0; d < 2; d++) {
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] != wParam) {
			((CRMSWnd*)m_pDispWnd[d])->m_Max = Max;

			if (Max == 1) ((CRMSWnd*)m_pDispWnd[d])->m_ZoomInButton.SetEnabled(FALSE);
			else          ((CRMSWnd*)m_pDispWnd[d])->m_ZoomInButton.SetEnabled(TRUE);
			((CRMSWnd*)m_pDispWnd[d])->m_ZoomInButton.Repaint();

			m_pDispWnd[d]->Repaint();
			break;
		}
	}

	return 0;
}

//***************************************************************************************

void CWFSoloCmpWnd::CreateMenu()
{
	CWndSettings* pWndSettings = GetWndSettings();

	
	if (m_OD && ((::OD_AverageExam != 0 && m_d == 0) ||
		(::OD_AverageExam2 != 0 && m_d == 1))
		)
	{
		m_pMenu->AppendItem(IDC_SELECT_IMAGE_ITEM, FALSE, FALSE, "Select eye image", "");

		m_pMenu->AppendSeparator();
	}

	if (!m_OD && ((::OS_AverageExam != 0 && m_d == 0) ||
		(::OS_AverageExam2 != 0 && m_d == 1))
		)
	{
		m_pMenu->AppendItem(IDC_SELECT_IMAGE_ITEM, FALSE, FALSE, "Select eye image", "");

		m_pMenu->AppendSeparator();
	}
		 


	m_pMenu->AppendItem(IDC_TYPE_TWFM_ITEM, pWndSettings->m_Type == TYPE_TWFM, FALSE, "Wavefront Map", "");
	m_pMenu->AppendItem(IDC_TYPE_TRFM_ITEM, pWndSettings->m_Type == TYPE_TRFM, FALSE, "Refraction Map", "");
	if (!m_isSolo)
	{
		m_pMenu->AppendItem(IDC_TYPE_TPSF_ITEM, pWndSettings->m_Type == TYPE_TPSF, FALSE, "Point Spread Function", "");
		m_pMenu->AppendItem(IDC_TYPE_TLTR_ITEM, pWndSettings->m_Type == TYPE_TLTR, FALSE, "Visual Acuity Chart", "");
		m_pMenu->AppendItem(IDC_TYPE_TMTF_ITEM, pWndSettings->m_Type == TYPE_TMTF, FALSE, "Modulation Transfer Function", "");
		m_pMenu->AppendItem(IDC_TYPE_TEEF_ITEM, pWndSettings->m_Type == TYPE_TEEF, FALSE, "Encircled Energy Function", "");
		m_pMenu->AppendItem(IDC_TYPE_TRMS_ITEM, pWndSettings->m_Type == TYPE_TRMS, FALSE, "Root Mean Square", "");
	}
	else
	{
		//m_pMenu->AppendItem(IDC_TYPE_TLTR_ITEM, pWndSettings->m_Type == TYPE_TLTR, FALSE, "Visual Acuity Chart", "");
		//
		//CreateSubmenuNLtr();		
	}

	if (!m_isSolo)
	{
		m_pMenu->AppendItem(IDC_TYPE_TAZR_ITEM, pWndSettings->m_Type == TYPE_TAZR, FALSE, "Annular / Zonal Refraction", "");
		m_pMenu->AppendSeparator();

		if (pWndSettings->m_Type == TYPE_TWFM ||
			pWndSettings->m_Type == TYPE_TRFM ||
			pWndSettings->m_Type == TYPE_TPSF ||
			pWndSettings->m_Type == TYPE_TMTF ||
			pWndSettings->m_Type == TYPE_TLTR ||
			(pWndSettings->m_Type == TYPE_TRMS && pWndSettings->m_RMSIndividual))
		{
			m_pMenu->AppendItem(IDC_METHOD_3D_ITEM, FALSE, pWndSettings->m_Method3D, "3D", "");
			m_pMenu->AppendSeparator();
		}
	}

	else m_pMenu->AppendSeparator();

	//CreateSubmenuZone();
	CreateSubmenuMask();
	m_pMenu->AppendSeparator();

	if (pWndSettings->m_Type == TYPE_TWFM || pWndSettings->m_Type == TYPE_TRFM)
	{
		if (pWndSettings->m_Method3D)
		{
			if (!m_isSolo)
			{
				m_pMenu->AppendItem(IDC_SHOW_MAP_SURFACE_ITEM, FALSE, pWndSettings->m_MapShowSolidSurface, "Show solid surface", "");
				m_pMenu->AppendItem(IDC_SHOW_MAP_MESH_ITEM, FALSE, pWndSettings->m_MapShowWireMesh, "Show wire mesh", "");
			}
		}
		else {
			m_pMenu->AppendItem(IDC_SHOW_MAP_MAP_ITEM, FALSE, pWndSettings->m_MapShowMap, "Show map", "");
			if (!m_isSolo) m_pMenu->AppendItem(IDC_SHOW_MAP_NUMBERS_ITEM, FALSE, pWndSettings->m_MapShowNumbers, "Show numbers", "");
			m_pMenu->AppendItem(IDC_SHOW_MAP_EYE_ITEM, FALSE, pWndSettings->m_MapShowEye, "Show eye image", "");
			m_pMenu->AppendItem(IDC_SHOW_MAP_PUPIL_ITEM, FALSE, pWndSettings->m_MapShowPupil, "Show pupil", "");
			m_pMenu->AppendItem(IDC_SHOW_MAP_VERTEX_ITEM, FALSE, pWndSettings->m_MapShowCornealVertex, "Show corneal vertex", "");
			//m_pMenu->AppendItem(IDC_SHOW_MAP_LIMBUS_ITEM, FALSE, pWndSettings->m_MapShowLimbus, "Show limbus", "");//
			m_pMenu->AppendItem(IDC_MAP_TRANSLUCENT_ITEM, FALSE, pWndSettings->m_MapTranslucent, "Translucent map", "");
		}
		m_pMenu->AppendSeparator();
		CreateSubmenuMapSteps();
		CreateSubmenuMapColors();
		m_pMenu->AppendSeparator();
	}

	else if (pWndSettings->m_Type == TYPE_TPSF) {
		if (!m_isSolo)
		{
			m_pMenu->AppendItem(IDC_SHOW_PSF_EEF_ITEM, FALSE, pWndSettings->m_ShowEEF, "Show 50% energy circle", "");
			if (pWndSettings->m_ShowEEF) {
				if (pWndSettings->m_EEFType != EEF_CNT) pWndSettings->m_EEFType = EEF_MAX;
				m_pMenu->AppendItem(IDC_EEF_TYPE1_ITEM, pWndSettings->m_EEFType == EEF_MAX, FALSE, "Circle around maximum", "");
				m_pMenu->AppendItem(IDC_EEF_TYPE2_ITEM, pWndSettings->m_EEFType == EEF_CNT, FALSE, "Circle around centroid", "");
			}
			m_pMenu->AppendSeparator();
		}
	}

	else if (pWndSettings->m_Type == TYPE_TMTF) {
		if (!pWndSettings->m_Method3D) {
			m_pMenu->AppendItem(IDC_MTF_AVG_ITEM, FALSE, pWndSettings->m_MTFAverage, "Radial average curve", "");
			m_pMenu->AppendSeparator();
		}
	}

	else if (pWndSettings->m_Type == TYPE_TLTR) {
		CreateSubmenuLtr();
		m_pMenu->AppendSeparator();
	}

	else if (pWndSettings->m_Type == TYPE_TPSF || pWndSettings->m_Type == TYPE_TEEF) {
		if (pWndSettings->m_EEFType != EEF_CNT) pWndSettings->m_EEFType = EEF_MAX;
		m_pMenu->AppendItem(IDC_EEF_TYPE1_ITEM, pWndSettings->m_EEFType == EEF_MAX, FALSE, "Circle round maximum", "");
		m_pMenu->AppendItem(IDC_EEF_TYPE2_ITEM, pWndSettings->m_EEFType == EEF_CNT, FALSE, "Circle round centroid", "");
		m_pMenu->AppendSeparator();
	}

	else if (pWndSettings->m_Type == TYPE_TRMS) {
		if (!pWndSettings->m_Method3D) {
			m_pMenu->AppendItem(IDC_RMS_CMB_ITEM, FALSE, !pWndSettings->m_RMSIndividual, "Combined", "");
			m_pMenu->AppendSeparator();
		}
	}

	m_pMenu->AppendItem(IDC_SAVE_TXT_ITEM, FALSE, FALSE, "Save Zernike coefficients into file", "");

	
	if (!SameSoloCmp())
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
	}
	
}

//***************************************************************************************

//***************************************************************************************
//************ For highlight the hyperlinker box ****************************************
//***************************************************************************************

void CWFSoloCmpWnd::OnTimer(UINT_PTR nIDEvent)
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
void CWFSoloCmpWnd::OnMouseMove(uint nFlags, CPoint Point)
{
	CPoint point;
	GetCursorPos(&point);

	if (point.x > m_Rect[3].left && point.x < m_Rect[3].right
		&& point.y > m_Rect[3].top  && point.y < m_Rect[3].bottom
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
	else if (point.x > m_Rect[4].left && point.x < m_Rect[4].right
		&& point.y > m_Rect[4].top  && point.y < m_Rect[4].bottom
		&& m_pWFExam1)
	{
		::SetCursor(::LoadCursor(NULL, IDC_HAND));

		int Infox, Infoy;
		Infox = point.x + 20;
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

void CWFSoloCmpWnd::OnLButtonDown(uint nFlags, CPoint Point)
{
	CPoint point;
	GetCursorPos(&point);

	if (point.x > m_Rect[3].left && point.x < m_Rect[3].right
		&& point.y > m_Rect[3].top  && point.y < m_Rect[3].bottom
		&& m_pWFExam1)
	{
		GetParent()->PostMessage(WM_SOLO_HYPELINK, 3, 0);//Left OD, m_pWFExam1
	}
	if (point.x > m_Rect[4].left && point.x < m_Rect[4].right
		&& point.y > m_Rect[4].top  && point.y < m_Rect[4].bottom
		&& m_pWFExam2)
	{
		GetParent()->PostMessage(WM_SOLO_HYPELINK, 3, 1);//Right OD, m_pWFExam2
	}
}

//***************************************************************************************
//************ For highlight the hyperlinker box  Done***********************************
//***************************************************************************************

//***************************************************************************************

void CWFSoloCmpWnd::OnToggleButtonClicked()
{
	CBusyCursor Cursor;

	CWndSettings* pWndSettings = GetWndSettings();

	if (pWndSettings->m_Type == TYPE_TLTR)
	{
		pWndSettings->m_Type = TYPE_TRFM;
	}
	else
	{
		pWndSettings->m_Type = TYPE_TLTR;
	}

	CreateChildWnd();
	Repaint();
}


//***************************************************************************************

void CWFSoloCmpWnd::OnSunButton1Clicked()
{
	OnSunButtonClicked();
}

//***************************************************************************************

void CWFSoloCmpWnd::OnMoonButton1Clicked()
{
	OnMoonButtonClicked();
}

//***************************************************************************************

void CWFSoloCmpWnd::OnSunButton2Clicked()
{
	OnSunButtonClicked();
}

//***************************************************************************************

void CWFSoloCmpWnd::OnMoonButton2Clicked()
{
	OnMoonButtonClicked();
}

//***************************************************************************************

void CWFSoloCmpWnd::OnSunButton3Clicked()
{
	OnSunButtonClicked();
}

//***************************************************************************************

void CWFSoloCmpWnd::OnMoonButton3Clicked()
{
	OnMoonButtonClicked();
}

//***************************************************************************************

void CWFSoloCmpWnd::OnSunButtonClicked()
{
	CBusyCursor Cursor;

	m_scan.Format(_T("%.2f mm"), ::TempSettings.Com_r_max_um*0.002);
	CWndSettings* pWndSettings = GetWndSettings();
	pWndSettings->m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um1);
	CreateChildWnd();

	for (int i = 0; i < 3; i++)
	{
		m_Slider[i].m_Pos = 1;
		m_Slider[i].OnPaint();
	}

	Repaint();
}

//***************************************************************************************

void CWFSoloCmpWnd::OnMoonButtonClicked()
{
	CBusyCursor Cursor;

	m_scan = "1.00 mm";
	CWndSettings* pWndSettings = GetWndSettings();
	pWndSettings->m_ZoneRadiusUm = 500;
	CreateChildWnd();

	for (int i = 0; i < 3; i++)
	{
		m_Slider[i].m_Pos = 0;
		m_Slider[i].OnPaint();
	}

	Repaint();
}

//***************************************************************************************


//***************************************************************************************
LRESULT CWFSoloCmpWnd::OnSliderClick(WPARAM wParam, LPARAM lParam)
{
	CBusyCursor Cursor;
	CPoint point;
	GetCursorPos(&point);

	int newZoneRadiusUm;
	real_t pos = 1;

	if (point.x <= m_slider1Right)
	{
		if (m_Slider[0].m_Pos > 0.95) m_Slider[0].m_Pos = 1;
		newZoneRadiusUm = intRound(::TempSettings.Com_r_max_um - (::TempSettings.Com_r_max_um - 500)* (1 - m_Slider[0].m_Pos));
	}
	else if (point.x >= m_slider3Left)
	{
		if (m_Slider[2].m_Pos > 0.95) m_Slider[2].m_Pos = 1;
		newZoneRadiusUm = intRound(::TempSettings.Com_r_max_um - (::TempSettings.Com_r_max_um - 500)* (1 - m_Slider[2].m_Pos));
	}
	else
	{
		if (m_Slider[1].m_Pos > 0.95) m_Slider[1].m_Pos = 1;
		newZoneRadiusUm = intRound(::TempSettings.Com_r_max_um - (::TempSettings.Com_r_max_um - 500)* (1 - m_Slider[1].m_Pos));
	}

	//****************************
	newZoneRadiusUm = GetClosePos(newZoneRadiusUm, intRound(::TempSettings.Com_r_max_um1));

	pos = 1 - (::TempSettings.Com_r_max_um - newZoneRadiusUm) / (::TempSettings.Com_r_max_um - 500);
	SetCursorPos(point.x + 7, m_Slider_Top + m_Slider[0].returnY());
	//****************************

	CWndSettings* pWndSettings = GetWndSettings();

	pWndSettings->m_ZoneRadiusUm = newZoneRadiusUm;

	CreateChildWnd();

	m_scan.Format(_T("%.2f mm"), newZoneRadiusUm*0.002);

	for (int i = 0; i < 3; i++)
	{
		m_Slider[i].m_Pos = pos;
		m_Slider[i].OnPaint();
	}

	Repaint();

	return 0;
}

//***************************************************************************************
//***************************************************************************************
LRESULT CWFSoloCmpWnd::OnSliderChangedPos(WPARAM wParam, LPARAM lParam)
{
	CPoint point;
	GetCursorPos(&point);

	int newZoneRadiusUm;
	real_t pos = 1;

	if (point.x <= m_slider1Right)
	{
		if (m_Slider[0].m_Pos > 0.95) m_Slider[0].m_Pos = 1;
		newZoneRadiusUm = intRound(::TempSettings.Com_r_max_um - (::TempSettings.Com_r_max_um - 500)* (1 - m_Slider[0].m_Pos));
		pos = m_Slider[0].m_Pos;
	}
	else if (point.x >= m_slider3Left)
	{
		if (m_Slider[2].m_Pos > 0.95) m_Slider[2].m_Pos = 1;
		newZoneRadiusUm = intRound(::TempSettings.Com_r_max_um - (::TempSettings.Com_r_max_um - 500)* (1 - m_Slider[2].m_Pos));
		pos = m_Slider[2].m_Pos;
	}
	else
	{
		if (m_Slider[1].m_Pos > 0.95) m_Slider[1].m_Pos = 1;
		newZoneRadiusUm = intRound(::TempSettings.Com_r_max_um - (::TempSettings.Com_r_max_um - 500)* (1 - m_Slider[1].m_Pos));
		pos = m_Slider[1].m_Pos;
	}

	CWndSettings* pWndSettings = GetWndSettings();

	pWndSettings->m_ZoneRadiusUm = newZoneRadiusUm;

	CreateChildWnd();

	m_scan.Format(_T("%.2f mm"), newZoneRadiusUm*0.002);

	for (int i = 0; i < 3; i++)
	{
		m_Slider[i].m_Pos = pos;
		m_Slider[i].OnPaint();
	}

	Repaint();

	return 0;
}

//***************************************************************************************




//***************************************************************************************
//new
void CWFSoloCmpWnd::OnShowDefaultItemClicked()
{
	CBusyCursor Cursor;

	CWndSettings* pWndSettings = GetWndSettings();

	pWndSettings->m_LtrDistanceType = 0;//530

	pWndSettings->m_Type = 11;

	pWndSettings->m_Method3D = 0;
	pWndSettings->m_MapShowSolidSurface = TRUE;
	pWndSettings->m_MapShowWireMesh = TRUE;

	pWndSettings->m_Mask.SetType(MASK_TOTAL);
	pWndSettings->m_Piston = FALSE;
	pWndSettings->m_Tilt = FALSE;

	pWndSettings->m_MapShowMap = 1;
	pWndSettings->m_MapShowNumbers = 0;
	pWndSettings->m_MapShowEye = 0;
	pWndSettings->m_MapShowPupil = 0;
	pWndSettings->m_MapShowLimbus = 0;
	pWndSettings->m_MapShowCornealVertex = 0;
	pWndSettings->m_MapTranslucent = 0;

	pWndSettings->m_ZoneRadiusUm = 5000;

	CScale* pScale = pWndSettings->GetScale();
	pScale->m_Step = 0.5;
	pWndSettings->SetDefaultColors(1);

	CreateChildWnd();

	Repaint();
}
//new
//***************************************************************************************
//new
BOOL CWFSoloCmpWnd::SameSoloCmp()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (pWndSettings->m_Type != 11)    return FALSE;

	if (pWndSettings->m_Method3D != 0)     return FALSE;

	if (pWndSettings->m_Mask.GetType() != MASK_TOTAL)   return FALSE;
	if (pWndSettings->m_Piston != FALSE) return FALSE;
	if (pWndSettings->m_Tilt != FALSE) return FALSE;

	if (pWndSettings->m_MapShowMap != 1)     return FALSE;
	if (pWndSettings->m_MapShowNumbers != 0)     return FALSE;
	if (pWndSettings->m_MapShowEye != 0)     return FALSE;
	if (pWndSettings->m_MapShowPupil != 0)     return FALSE;
	//if (pWndSettings->m_MapShowLimbus        != 0)     return FALSE; 	
	if (pWndSettings->m_MapShowCornealVertex != 0)     return FALSE;
	if (pWndSettings->m_MapTranslucent != 0)     return FALSE;

	CScale* pScale = pWndSettings->GetScale();
	if (pScale->m_Step != 0.5)                         return FALSE;
	if (!pWndSettings->AreDefaultColors(1))            return FALSE;

	return TRUE;
}
//new
//***************************************************************************************

//***************************************************************************************

void CWFSoloCmpWnd::OnSelEyeImgItemClicked()
{

	int type = (m_d == 0 ? 3 : 4);

	if (!m_OD)
	{
		type = (m_d == 0 ? 5 : 6);
	}

	CSelectImgDlg* pDlg = new CSelectImgDlg(this, type);

	if (pDlg->DoModal() == IDOK)
	{
		CWFExam* SelectWFExam = new CWFExam;
		::DB.LoadWFExam(pDlg->m_Sel_ID, SelectWFExam);

		if (type == 0 || type == 3 || type == 5)
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

//***************************************************************************************
//530
void CWFSoloCmpWnd::OnShowWFTextInfoBtnClicked()
{
	m_pWndSettings[0].m_SimplifiedWFPanel = !m_pWndSettings[0].m_SimplifiedWFPanel;
	Repaint();

	CString s = (m_pWndSettings[0].m_SimplifiedWFPanel ? "Show Details" : "Hide Details");
	m_WFShowFullInfoButton.SetTip(s);
	m_WFShowFullInfoButton.SetImage(m_pWndSettings[0].m_SimplifiedWFPanel ?
		(m_SW >= 1920 ? IDR_SHOW_DETAIL_3 : m_SW >= 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
		(m_SW >= 1920 ? IDR_HIDE_DETAIL_3 : m_SW >= 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
	m_WFShowFullInfoButton.Repaint();

	m_WFShowFullInfoButton2.SetTip(s);
	m_WFShowFullInfoButton2.SetImage(m_pWndSettings[0].m_SimplifiedWFPanel ?
		(m_SW >= 1920 ? IDR_SHOW_DETAIL_3 : m_SW >= 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
		(m_SW >= 1920 ? IDR_HIDE_DETAIL_3 : m_SW >= 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
	m_WFShowFullInfoButton2.Repaint();

	m_WFShowFullInfoButton3.SetTip(s);
	m_WFShowFullInfoButton3.SetImage(m_pWndSettings[0].m_SimplifiedWFPanel ?
		(m_SW >= 1920 ? IDR_SHOW_DETAIL_3 : m_SW >= 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
		(m_SW >= 1920 ? IDR_HIDE_DETAIL_3 : m_SW >= 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
	m_WFShowFullInfoButton3.Repaint();
}
//530
//***************************************************************************************
//530
void CWFSoloCmpWnd::OnShowWFTextInfoBtn2Clicked()
{
	OnShowWFTextInfoBtnClicked();
}
//530
//***************************************************************************************
//530
void CWFSoloCmpWnd::OnShowWFTextInfoBtn3Clicked()
{
	OnShowWFTextInfoBtnClicked();
}
//530
//***************************************************************************************