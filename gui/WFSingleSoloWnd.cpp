// Solo Aberration analysis display
//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "WFSingleSoloWnd.h"
#include "EyeWnd.h"
#include "LtrWnd.h"
#include "RMSWnd.h"
#include "BusyCursor.h"
#include "SelectImgDlg.h"
#include "NearLtrWnd.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CWFSingleSoloWnd, CSumWnd)

	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()

	ON_MESSAGE(WM_SLIDER_LCLICK, OnSliderClick)
	ON_MESSAGE(WM_SLIDER_POS_CHANGED, OnSliderChangedPos)

	ON_BN_CLICKED(IDC_SOLO_SUN_ITEM, OnSunButtonClicked)
	ON_BN_CLICKED(IDC_SOLO_MOON_ITEM, OnMoonButtonClicked)
	ON_BN_CLICKED(IDC_SOLO_TOGGLE_ITEM, OnToggleButtonClicked)

	ON_COMMAND(IDC_DEFAULT_GUI_ITEM, OnShowDefaultItemClicked)//new
	ON_COMMAND(IDC_SELECT_IMAGE_ITEM, OnSelEyeImgItemClicked)//new

END_MESSAGE_MAP()

//***************************************************************************************

CWFSingleSoloWnd::CWFSingleSoloWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CWFExam* pWFExam, CWndSettings* pWndSettings) :
	CSumWnd(pWnd, WndRect, pPatient, pWndSettings)
{
	CBusyCursor Cursor;

	//----------------------------------------------------
	RECT Rect;
	GetWindowRect(&Rect);
	real_t t = 3.0 * (0.03 * m_h) + m_g;
	real_t w = ((Rect.right - Rect.left) - 4.0 * m_g) / 3.0;
	real_t h = ((Rect.bottom - Rect.top - t) - 2.0 * m_g) / 2.0;

	real_t Space = 5 * m_g;

	::SetRect(&m_Rect[0], intRound(m_g), intRound(t), intRound(m_g + w), intRound(t + h)); // map 1
	::SetRect(&m_Rect[1], intRound(m_g), intRound(t + h + m_g), intRound(m_g + w), intRound(t + h + m_g + h)); // map 2
	::SetRect(&m_Rect[2], intRound(m_g + w + m_g), intRound(t), intRound(m_g + w + m_g + w), intRound(t + h)); // map 3
	::SetRect(&m_Rect[3], intRound(m_g + w + m_g), intRound(t + h + m_g), intRound(m_g + w + m_g + w), intRound(t + h + m_g + h)); // map 4


	::SetRect(&m_Rect[4], intRound(m_g + w + m_g + w + Space), intRound(t), intRound(m_g + w + m_g + w + m_g + w), intRound(t + h + m_g + h)); // info
	::SetRect(&m_LargeRect, intRound(m_g), intRound(t), intRound(m_g + w + m_g + w), intRound(t + h + m_g + h)); // large map 

	//Cotrol components
	m_SliderTop = t + h / 2.0;
	real_t SliderBottom = 2 * h - h / 2.0;
	::SetRect(&m_Rect[5], intRound(m_g + w + m_g + w + m_g), intRound(m_SliderTop), intRound(m_g + w + m_g + w + 4 * m_g), intRound(SliderBottom + m_g)); // Slider
																																						  //Done

	int  IconWide = 9;

	//scan size info
	//small Size
	::SetRect(&m_Rect[6], intRound(m_g + w + m_g + w), intRound(m_SliderTop - 5.5*IconWide), intRound(m_g + w + m_g + w + Space), intRound(m_SliderTop - 3 * IconWide));
	//Large Window

	//Refraction Info rectangle pos
	int infoPh = (m_Rect[4].bottom - m_Rect[4].top) / 30;//per line h inside the info window
	::SetRect(&m_Rect[7], intRound(m_g + w + m_g + w + Space), intRound(t + 9 * infoPh), intRound(m_g + w + m_g + w + m_g + w), intRound(t + 10 * infoPh));

	//Three Control buttons  
	::SetRect(&m_ButtonsRect[0], intRound(m_g + w + m_g + w + 1.5*IconWide), intRound(m_SliderTop - 2.5*IconWide), intRound(m_g + w + m_g + w + 3.5*IconWide), intRound(m_SliderTop - 0.5*IconWide)); // map 1
	m_SunButton.Create(_T(""), _T(""), 0, m_ButtonsRect[0], this, IDC_SOLO_SUN_ITEM);
	m_SunButton.SetImage(IDR_SOLO_MOON);
	m_SunButton.SetBk(FALSE);
	m_SunButton.SetBkColor(BLACK);
	m_SunButton.ShowWindow(SW_SHOW);

	::SetRect(&m_ButtonsRect[1], intRound(m_g + w + m_g + w + 1.5*IconWide), intRound(SliderBottom + 1.5*IconWide), intRound(m_g + w + m_g + w + 3.5*IconWide), intRound(SliderBottom + 3.5*IconWide)); // map 1
	m_MoonButton.Create(_T(""), _T(""), 0, m_ButtonsRect[1], this, IDC_SOLO_MOON_ITEM);
	m_MoonButton.SetImage(IDR_SOLO_SUN);
	m_MoonButton.SetBk(FALSE);
	m_MoonButton.SetBkColor(BLACK);
	m_MoonButton.ShowWindow(SW_SHOW);

	::SetRect(&m_ButtonsRect[2], intRound(m_g + w + m_g + w + 1.2*IconWide), intRound(SliderBottom + 5.5*IconWide), intRound(m_g + w + m_g + w + 3.7*IconWide), intRound(SliderBottom + 11.5*IconWide)); // map 1
	m_ToggleButton.Create(_T(""), _T(""), 0, m_ButtonsRect[2], this, IDC_SOLO_TOGGLE_ITEM);

	if (m_pWndSettings[0].m_Type == TYPE_TLTR && m_pWndSettings[1].m_Type == TYPE_TLTR)
	{
		m_ToggleButton.SetImage(IDR_SOLO_E_MAP);
		m_ToggleButton.SetTip("Switch to map");
	}
	else
	{
		m_ToggleButton.SetImage(IDR_SOLO_E_MAP);
		m_ToggleButton.SetTip("Switch to Visual Acuity");
	}
	m_ToggleButton.SetBk(FALSE);
	m_ToggleButton.SetBkColor(BLACK);
	m_ToggleButton.ShowWindow(SW_SHOW);

	//----------------------------------------------------

	m_pWFExam = pWFExam;

	m_AverageExam = (m_pWFExam->m_Header.IsOD() ? ::OD_AverageExam : ::OS_AverageExam);

	for (int i = 0; i < m_AverageExam; i++)
	{
		if (pWFExam->m_Header.IsOD())
			m_SelectExamsID[i] = ::OD_SelectExamsID[i];
		else
			m_SelectExamsID[i] = ::OS_SelectExamsID[i];
	}

	//record the increase and decrease range
	::TempSettings.Com_r_max_um = m_pWFExam->m_WfSurface.m_r_max_um;
	m_scan.Format(_T("%.2f mm"), ::TempSettings.Com_r_max_um*0.002);

	for (m_d = 0; m_d < 4; m_d++)
	{
		CWndSettings* pWndSettings = GetWndSettings();
		pWndSettings->m_ZoneRadiusUm = 5000;
	}

	m_d = -1;

	for (int i = 0; i < 4; i++)
	{
		if (!m_pWndSettings[i].m_SizeSmall)
		{
			m_d = i;
			CreateChildWnd();
			break;
		}
	}

	if (m_d == -1)
	{
		for (m_d = 0; m_d < 4; m_d++)
		{
			CreateChildWnd();
		}
	}

	//Slider
	m_Slider.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_Rect[5], this, IDC_SOLO_SLIDER);
	m_Slider.m_Pos = 1;
	m_Slider.m_scaleNum = (int)(::TempSettings.Com_r_max_um*0.002);

	real_t rest = ::TempSettings.Com_r_max_um - intRound(::TempSettings.Com_r_max_um) % 500;
	m_Slider.m_FirstPos = 1 - (::TempSettings.Com_r_max_um - rest) / (::TempSettings.Com_r_max_um - 500);

	m_HighLight = FALSE;

	Repaint();

	//The 'transform funtionality' info ("Link to Aberration analysis")
	m_HighLight = FALSE;

	RECT InfoRect;
	::SetRect(&InfoRect, 0, 0, 160, 20);
	m_NoteInfo.CreateWnd(InfoRect, this);
	m_NoteInfo.ClearMemDC();

	CMFont Font(15, 400, "Arial");
	m_NoteInfo.m_MemDC.WriteText("  Link to Refraction analysis", InfoRect, Font, BLACK, 0, NOCOLOR);
	m_NoteInfo.ShowWindow(FALSE);

	ShowWindow(SW_SHOW);
}

//***************************************************************************************

void CWFSingleSoloWnd::OnSizeLargeItemClicked()
{
	CBusyCursor Cursor;

	for (int i = 0; i < 4; i++)
	{
		SAFE_DELETE(m_pDispWnd[i]);
	}

	CWndSettings* pWndSettings = GetWndSettings();

	pWndSettings->m_SizeSmall = !pWndSettings->m_SizeSmall;

	if (pWndSettings->m_SizeSmall)
	{
		for (m_d = 0; m_d < 4; m_d++) CreateChildWnd();
	}
	else 
	{
		CreateChildWnd();
	}

	Repaint();
}

//***************************************************************************************

void CWFSingleSoloWnd::RepaintMemDC()
{
	CSumWnd::RepaintMemDC();

	CMFont FontScanSize(intRound(1.5*m_g), 400, "Arial");

	m_MemDC.WriteText(m_scan, m_Rect[6], FontScanSize, RED, 1, NOCOLOR);

	BOOL Rows[30] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	WFSoloExamInfo(m_pWFExam, m_Rect[4], 30, Rows, m_HighLight);
}

//***************************************************************************************

void CWFSingleSoloWnd::CreateChildWnd()
{
	SAFE_DELETE(m_pDispWnd[m_d]);

	CWndSettings* pWndSettings = GetWndSettings();

	m_Surfaces[m_d] = m_pWFExam->m_WfSurface;

	real_t r_max_um = 5000.0;

	r_max_um = __min(r_max_um, m_Surfaces[m_d].m_r_max_um);
	r_max_um = __min(r_max_um, pWndSettings->m_ZoneRadiusUm);
	m_Surfaces[m_d].ChangeRMaxUm(r_max_um);
	m_Surfaces[m_d] &= pWndSettings->m_Mask;
	m_Surfaces[m_d].ComputePiston(pWndSettings->m_Piston);
	m_Surfaces[m_d].ComputeTilt(pWndSettings->m_Tilt);

	RECT Rect = pWndSettings->m_SizeSmall ? m_Rect[m_d] : m_LargeRect;
		
	switch (pWndSettings->m_Type)
	{
	case TYPE_TWFM: //Wafefront map
	{
		CEyeWnd* pEyeWnd = new CEyeWnd(Rect, this);
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
		pEyeWnd->m_Unit = "µ";
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

		pEyeWnd->m_pWFExam = m_pWFExam;

		CWFExam::CreateWfm2D(pEyeWnd->m_Map2D, m_Surfaces[m_d], 1.0 / pEyeWnd->m_y_px_um);

		pEyeWnd->CreateCentButtons();
		pEyeWnd->CreateZoomButtons();
		//pEyeWnd->CreateZoneButtons();
		pEyeWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 0);

		m_pDispWnd[m_d] = pEyeWnd;
	}
	break;

	case TYPE_TRFM:  //Refraction Map 11
	{
		CEyeWnd* pEyeWnd = new CEyeWnd(Rect, this);
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
		pEyeWnd->m_Unit = "D";
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
		pEyeWnd->m_pWFExam = m_pWFExam;

		CWFExam::CreateRfm2D(pEyeWnd->m_Map2D, m_Surfaces[m_d], 1.0 / pEyeWnd->m_y_px_um);

		pEyeWnd->CreateCentButtons();
		pEyeWnd->CreateZoomButtons();
		//pEyeWnd->CreateZoneButtons();
		pEyeWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 0);

		m_pDispWnd[m_d] = pEyeWnd;
	}
	break;

	case TYPE_TLTR:
	{
		CLtrWnd* pLtrWnd = new CLtrWnd(Rect, this);
		pLtrWnd->m_DysVal = -1;
		CWFExam::CreateLtr(m_Surfaces[m_d], pLtrWnd->m_Ltr, pWndSettings->m_Ltr, pWndSettings->m_LtrLine, pWndSettings->m_LtrOrientation);
		pLtrWnd->m_Type = pWndSettings->m_Type;
		pLtrWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
		pLtrWnd->m_Method3D = FALSE;

		pLtrWnd->m_LTLabel[0] = "Letter";
		pWndSettings->m_Mask.GetName(pLtrWnd->m_LTLabel[1]);
		pLtrWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
		pLtrWnd->CreateZoomButtons();

		pLtrWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 0);
		m_pDispWnd[m_d] = pLtrWnd;
	}
	break;

	case TYPE_TRMS:
	{
		CRMSWnd* pRMSWnd = new CRMSWnd(Rect, this);
		pRMSWnd->m_Type = pWndSettings->m_Type;
		pRMSWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
		pRMSWnd->m_Method3D = FALSE;
		pRMSWnd->m_LTLabel[0] = "Root Mean Square";
		pWndSettings->m_Mask.GetName(pRMSWnd->m_LTLabel[1]);
		pRMSWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
		pRMSWnd->m_Surface = m_Surfaces[m_d];
		pRMSWnd->m_Individual = pWndSettings->m_RMSIndividual;
		pRMSWnd->CreateZoomButtons();

		pRMSWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 0);

		m_pDispWnd[m_d] = pRMSWnd;
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
		else {
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
		pRMSWnd->m_Max = 1;
		pRMSWnd->m_ZoomInButton.SetEnabled(FALSE);
		pRMSWnd->m_ZoomInButton.Repaint();
	}
	
	//Change the toggle button
	if (m_pWndSettings[0].m_Type == TYPE_TLTR && m_pWndSettings[1].m_Type == TYPE_TLTR)
	{
		m_ToggleButton.SetImage(IDR_SOLO_E_MAP);
		m_ToggleButton.SetTip("Switch to map");
	}
	else
	{
		m_ToggleButton.SetImage(IDR_SOLO_E_MAP);
		m_ToggleButton.SetTip("Switch to Visual Acuity");
	}

	m_ToggleButton.Repaint();
	//Done
}

//***************************************************************************************

void CWFSingleSoloWnd::CreateMenu()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_AverageExam != 0 && m_d < 2)
	{
		m_pMenu->AppendItem(IDC_SELECT_IMAGE_ITEM, FALSE, FALSE, "Select eye image", "");

		m_pMenu->AppendSeparator();
	}

	if (pWndSettings->m_Type == TYPE_TWFM ||
		pWndSettings->m_Type == TYPE_TRFM ||
		pWndSettings->m_Type == TYPE_TLTR
		)
	{
		m_pMenu->AppendItem(IDC_TYPE_TWFM_ITEM, pWndSettings->m_Type == TYPE_TWFM, FALSE, "Wavefront Map", "");
		m_pMenu->AppendItem(IDC_TYPE_TRFM_ITEM, pWndSettings->m_Type == TYPE_TRFM, FALSE, "Refraction Map", "");
		m_pMenu->AppendItem(IDC_TYPE_TLTR_ITEM, pWndSettings->m_Type == TYPE_TLTR, FALSE, "Visual Acuity Chart", "");
	
		m_pMenu->AppendSeparator();
	}

	m_pMenu->AppendItem(IDC_SIZE_LARGE_ITEM, FALSE, !pWndSettings->m_SizeSmall, "Large window", "");
	m_pMenu->AppendSeparator();

	if (pWndSettings->m_Type == TYPE_TWFM ||
		pWndSettings->m_Type == TYPE_TRFM ||
		pWndSettings->m_Type == TYPE_TLTR ||
		pWndSettings->m_Type == TYPE_TRMS)
	{
		CreateSubmenuMask();
		m_pMenu->AppendSeparator();
	}

	if (pWndSettings->m_Type == TYPE_TWFM || pWndSettings->m_Type == TYPE_TRFM)
	{
		m_pMenu->AppendItem(IDC_SHOW_MAP_MAP_ITEM, FALSE, pWndSettings->m_MapShowMap, "Show map", "");

		if (m_d < 2 && m_pWFExam->m_Image.m_RGBData)
		{
			m_pMenu->AppendItem(IDC_SHOW_MAP_EYE_ITEM, FALSE, pWndSettings->m_MapShowEye, "Show eye image", "");
			m_pMenu->AppendItem(IDC_SHOW_MAP_PUPIL_ITEM, FALSE, pWndSettings->m_MapShowPupil, "Show pupil", "");
			m_pMenu->AppendItem(IDC_SHOW_MAP_VERTEX_ITEM, FALSE, pWndSettings->m_MapShowCornealVertex, "Show corneal vertex", "");			
		}

		m_pMenu->AppendItem(IDC_MAP_TRANSLUCENT_ITEM, FALSE, pWndSettings->m_MapTranslucent, "Translucent map", "");

		m_pMenu->AppendSeparator();
		CreateSubmenuMapSteps();
		CreateSubmenuMapColors();
		m_pMenu->AppendSeparator();
	}
	else if (pWndSettings->m_Type == TYPE_TLTR)
	{
		CreateSubmenuLtr();
		m_pMenu->AppendSeparator();
	}
	else if (pWndSettings->m_Type == TYPE_TRMS)
	{
		m_pMenu->AppendItem(IDC_RMS_CMB_ITEM, FALSE, !pWndSettings->m_RMSIndividual, "Combined", "");
		m_pMenu->AppendSeparator();
	}

	if (pWndSettings->m_Type == TYPE_TWFM ||
		pWndSettings->m_Type == TYPE_TRFM ||
		pWndSettings->m_Type == TYPE_TLTR ||
		pWndSettings->m_Type == TYPE_TRMS)
	{
		m_pMenu->AppendItem(IDC_SAVE_TXT_ITEM, FALSE, FALSE, "Save Zernike coefficients into file", "");
	}

	if (!SameAbeSet())
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
	}
}

//***************************************************************************************

LRESULT CWFSingleSoloWnd::OnSliderChangedPos(WPARAM wParam, LPARAM lParam)
{
	if (m_Slider.m_Pos > 0.95) m_Slider.m_Pos = 1;
	int newZoneRadiusUm = intRound(::TempSettings.Com_r_max_um - (::TempSettings.Com_r_max_um - 500)* (1 - m_Slider.m_Pos));

	CWndSettings* pWndSettings = GetWndSettings();

	if (pWndSettings->m_SizeSmall)
	{
		for (m_d = 0; m_d < 4; m_d++)
		{
			pWndSettings = GetWndSettings();

			pWndSettings->m_ZoneRadiusUm = newZoneRadiusUm;

			CreateChildWnd();
		}
	}
	else
	{
		int saveID = m_d;

		for (m_d = 0; m_d < 4; m_d++)
		{
			pWndSettings = GetWndSettings();

			pWndSettings->m_ZoneRadiusUm = newZoneRadiusUm;
		}

		m_d = saveID;

		CreateChildWnd();
	}

	m_scan.Format(_T("%.2f mm"), newZoneRadiusUm*0.002);

	Repaint();

	return 0;
}

//***************************************************************************************
LRESULT CWFSingleSoloWnd::OnSliderClick(WPARAM wParam, LPARAM lParam)
{
	CBusyCursor Cursor;

	CPoint point;
	GetCursorPos(&point);

	if (m_Slider.m_Pos > 0.95) m_Slider.m_Pos = 1;
	int newZoneRadiusUm = intRound(::TempSettings.Com_r_max_um - (::TempSettings.Com_r_max_um - 500)* (1 - m_Slider.m_Pos));

	//****************************
	if (newZoneRadiusUm >= ::TempSettings.Com_r_max_um)
	{
		newZoneRadiusUm = intRound(::TempSettings.Com_r_max_um);
	}
	else
	{
		newZoneRadiusUm = GetClosePos(newZoneRadiusUm, intRound(::TempSettings.Com_r_max_um));
	}

	m_Slider.m_Pos = 1 - (::TempSettings.Com_r_max_um - newZoneRadiusUm) / (::TempSettings.Com_r_max_um - 500);
	SetCursorPos(point.x + 7, intRound(m_SliderTop) + m_Slider.returnY());
	//****************************


	CWndSettings* pWndSettings = GetWndSettings();

	if (pWndSettings->m_SizeSmall)
	{
		for (m_d = 0; m_d < 4; m_d++)
		{
			pWndSettings = GetWndSettings();

			pWndSettings->m_ZoneRadiusUm = newZoneRadiusUm;

			CreateChildWnd();
		}
	}
	else
	{
		int saveID = m_d;

		for (m_d = 0; m_d < 4; m_d++)
		{
			pWndSettings = GetWndSettings();

			pWndSettings->m_ZoneRadiusUm = newZoneRadiusUm;
		}

		m_d = saveID;

		CreateChildWnd();
	}

	m_scan.Format(_T("%.2f mm"), newZoneRadiusUm*0.002);

	Repaint();

	return 0;
}

//***************************************************************************************

void CWFSingleSoloWnd::OnSunButtonClicked()
{
	CBusyCursor Cursor;

	CWndSettings* pWndSettings = GetWndSettings();

	if (pWndSettings->m_SizeSmall)
	{
		for (m_d = 0; m_d < 4; m_d++)
		{
			pWndSettings = GetWndSettings();

			pWndSettings->m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um);

			CreateChildWnd();
		}
	}
	else
	{
		pWndSettings->m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um);

		CreateChildWnd();
	}

	m_Slider.m_Pos = 1;
	m_Slider.OnPaint();

	m_scan.Format(_T("%.2f mm"), ::TempSettings.Com_r_max_um*0.002);

	Repaint();
}

//***************************************************************************************

void CWFSingleSoloWnd::OnMoonButtonClicked()
{
	CBusyCursor Cursor;

	CWndSettings* pWndSettings = GetWndSettings();

	if (pWndSettings->m_SizeSmall)
	{
		for (m_d = 0; m_d < 4; m_d++)
		{
			pWndSettings = GetWndSettings();

			pWndSettings->m_ZoneRadiusUm = 500;

			CreateChildWnd();
		}
	}
	else
	{
		pWndSettings->m_ZoneRadiusUm = 500;

		CreateChildWnd();
	}

	m_Slider.m_Pos = 0;
	m_Slider.OnPaint();

	m_scan = "1.00 mm";
	Repaint();
}

//***************************************************************************************


void CWFSingleSoloWnd::OnToggleButtonClicked()
{
	CBusyCursor Cursor;

	CWndSettings* pWndSettings = GetWndSettings();

	if (pWndSettings->m_SizeSmall)
	{
		if (m_pWndSettings[0].m_Type == TYPE_TLTR && m_pWndSettings[1].m_Type == TYPE_TLTR)
		{
			m_d = 0;
			pWndSettings = GetWndSettings();
			pWndSettings->m_Type = TYPE_TWFM;
			CreateChildWnd();

			m_d = 1;
			pWndSettings = GetWndSettings();
			pWndSettings->m_Type = TYPE_TWFM;
			CreateChildWnd();

			m_ToggleButton.SetImage(IDR_SOLO_E_MAP);
			m_ToggleButton.SetTip("Switch to Visual Acuity");
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

			m_ToggleButton.SetImage(IDR_SOLO_E_MAP);
			m_ToggleButton.SetTip("Switch to map");
		}
	}
	else
	{
		if (m_d == 0 || m_d == 1)
		{
			pWndSettings = GetWndSettings();
			if (m_pWndSettings[0].m_Type == TYPE_TLTR && m_pWndSettings[1].m_Type == TYPE_TLTR)
			{
				pWndSettings->m_Type = TYPE_TWFM;
				CreateChildWnd();

				m_ToggleButton.SetImage(IDR_SOLO_E_MAP);
				m_ToggleButton.SetTip("Switch to Visual Acuity");
			}
			else
			{
				pWndSettings->m_Type = TYPE_TLTR;
				CreateChildWnd();

				m_ToggleButton.SetImage(IDR_SOLO_E_MAP);
				m_ToggleButton.SetTip("Switch to map");
			}
		}
	}

	Repaint();
}

//***************************************************************************************

//***************************************************************************************
//************ For highlight the hyperlinker box ****************************************
//***************************************************************************************

void CWFSingleSoloWnd::OnTimer(UINT_PTR nIDEvent)
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
void CWFSingleSoloWnd::OnMouseMove(uint nFlags, CPoint Point)
{
	CPoint point;
	GetCursorPos(&point);

	if (point.x > m_Rect[7].left && point.x < m_Rect[7].right
		&& point.y > m_Rect[7].top  && point.y < m_Rect[7].bottom
		&& m_pWFExam)
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

void CWFSingleSoloWnd::OnLButtonDown(uint nFlags, CPoint Point)
{
	CPoint point;
	GetCursorPos(&point);

	if (point.x > m_Rect[7].left && point.x < m_Rect[7].right
		&& point.y > m_Rect[7].top  && point.y < m_Rect[7].bottom
		&& m_pWFExam)
	{
		GetParent()->PostMessage(WM_SOLO_HYPELINK, 2, 0);
	}
}

//***************************************************************************************
//************ For highlight the hyperlinker box  Done***********************************
//***************************************************************************************

void CWFSingleSoloWnd::OnShowDefaultItemClicked()
{
	CBusyCursor Cursor;

	m_pWndSettings[m_d].m_LtrDistanceType = 0;

	if (m_d == 0 || m_d == 1)
	{
		m_pWndSettings[m_d].m_Type = 10;

		m_pWndSettings[m_d].m_SizeSmall = TRUE;

		m_pWndSettings[m_d].m_MapUnit = DIOPTERS;

		m_pWndSettings[m_d].m_MapShowMap = 1;
		m_pWndSettings[m_d].m_MapShowEye = 0;
		m_pWndSettings[m_d].m_MapShowPupil = 0;
		m_pWndSettings[m_d].m_MapShowLimbus = 0;
		m_pWndSettings[m_d].m_MapShowCornealVertex = 0;
		m_pWndSettings[m_d].m_MapTranslucent = 0;

		m_pWndSettings[m_d].m_ZoneRadiusUm = 5000;

		CScale* pScale = m_pWndSettings[m_d].GetScale();
		pScale->m_Step = 0.5;

		m_pWndSettings[m_d].SetDefaultColors(1);
	}

	if (m_d == 2 || m_d == 3)
	{
		m_pWndSettings[m_d].m_Type = TYPE_TRMS;

		m_pWndSettings[m_d].m_RMSIndividual = FALSE;
		m_pWndSettings[m_d].m_SizeSmall = TRUE;
	}

	if (m_d == 0 || m_d == 2)
	{
		m_pWndSettings[m_d].m_Mask.SetType(MASK_TOTAL);
	}

	if (m_d == 1 || m_d == 3)
	{
		m_pWndSettings[m_d].m_Mask.SetType(MASK_HO_TOTAL);
	}

	for (m_d = 0; m_d < 4; m_d++)
	{
		CreateChildWnd();
	}

	Repaint();
}

//***************************************************************************************

BOOL CWFSingleSoloWnd::SameAbeSet()
{
	if (m_d == 0 || m_d == 1)
	{
		if (m_pWndSettings[m_d].m_Type != 10)
		{
			return FALSE;
		}

		if (m_pWndSettings[m_d].m_SizeSmall != TRUE)
		{
			return FALSE;
		}

		if (m_pWndSettings[m_d].m_MapUnit != DIOPTERS)
		{
			return FALSE;
		}

		if (m_pWndSettings[m_d].m_MapShowMap != 1)
		{
			return FALSE;
		}

		if (m_pWndSettings[m_d].m_MapShowEye != 0)
		{
			return FALSE;
		}

		if (m_pWndSettings[m_d].m_MapShowPupil != 0)
		{
			return FALSE;
		}

		if (m_pWndSettings[m_d].m_MapShowCornealVertex != 0)
		{
			return FALSE;
		}

		if (m_pWndSettings[m_d].m_MapTranslucent != 0)
		{
			return FALSE;
		}
		
		CScale* pScale = m_pWndSettings[m_d].GetScale();

		if (pScale->m_Step != 0.5) 
		{
			return FALSE;
		}

		if (!m_pWndSettings[m_d].AreDefaultColors(1)) 
		{
			return FALSE;
		}
	}
	if (m_d == 2 || m_d == 3)
	{
		if (m_pWndSettings[m_d].m_Type != TYPE_TRMS)
		{
			return FALSE;
		}

		if (m_pWndSettings[m_d].m_RMSIndividual != FALSE)
		{
			return FALSE;
		}

		if (m_pWndSettings[m_d].m_SizeSmall != TRUE)
		{
			return FALSE;
		}
	}

	if (m_d == 0 || m_d == 2)
	{
		if (m_pWndSettings[m_d].m_Mask.GetType() != MASK_TOTAL)
		{
			return FALSE;
		}
	}
	if (m_d == 1 || m_d == 3)
	{
		if (m_pWndSettings[m_d].m_Mask.GetType() != MASK_HO_TOTAL)
		{
			return FALSE;
		}
	}

	return TRUE;
}

//***************************************************************************************

void CWFSingleSoloWnd::OnSelEyeImgItemClicked()
{
	int type = (m_pWFExam->m_Header.IsOD() ? 0 : 1);

	CSelectImgDlg* pDlg = new CSelectImgDlg(this, type);

	if (pDlg->DoModal() == IDOK)
	{
		CWFExam* SelectWFExam = new CWFExam;
		::DB.LoadWFExam(pDlg->m_Sel_ID, SelectWFExam);

		m_pWFExam->m_Header.m_Note = "Averaged Exam";
		m_pWFExam->m_Image = SelectWFExam->m_Image;
		m_pWFExam->Process();

		// for average exam 
		CString ExamId, LoadFileName;
		GUIDToStr(m_pWFExam->m_Header.m_ExamID, ExamId);
		LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

		if (::PathFileExists(LoadFileName))
		{
			CAveExamInfo AveExamInfo;

			AveExamInfo.LoadFromFile(LoadFileName);
			for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
			{
				m_pWFExam->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
			}
		}
		// for average exam Done

		SAFE_DELETE(SelectWFExam);

		delete pDlg;

		CWndSettings* pWndSettings = GetWndSettings();
		pWndSettings->m_MapShowEye = TRUE;

		m_d = -1;

		for (int i = 0; i < 4; i++)
		{
			if (!m_pWndSettings[i].m_SizeSmall)
			{
				m_d = i;
				CreateChildWnd();
				break;
			}
		}

		if (m_d == -1) 
		{
			for (m_d = 0; m_d < 4; m_d++)
			{
				CreateChildWnd();
			}
		}

		Repaint();
	}
	else delete pDlg;
}

//***************************************************************************************