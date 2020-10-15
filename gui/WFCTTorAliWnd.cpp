/*******************************************************************
Autor:         Jinfeng Chen
E-mail:        jinfengcc@gmail.com
Date:          04-19-2016
Description:   Create a new wfct gui used for showing toric planner alignment
*****************************************************************************************/

#include "StdAfx.h"
#include "Resource.h"
#include "WFCTTorAliWnd.h"
#include "EyeWnd.h"

#include "BusyCursor.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CWFCTTorAliWnd, CSumWnd)

	ON_BN_CLICKED(IDC_PLANNER_BUTTON, OnPlannerBtnClicked)//6.2.0 Planner button click

	ON_BN_CLICKED(IDC_SHOW_MAP_COLOR_EYE_ITEM, OnShowColorEyeClicked)//6.2.0 

	ON_BN_CLICKED(IDC_SHOW_MAP_GRAY_EYE_ITEM, OnShowGrayEyeClicked)//6.2.0 

	ON_MESSAGE(WM_NOTIFY_LBTN_DBCLK, OnBigPlanWndShown)    //6.2.0 for showing a big planer wnd

END_MESSAGE_MAP()

//***************************************************************************************

CWFCTTorAliWnd::CWFCTTorAliWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CWFExam* pWFExam, CCTExam* pCTExam) :
	CSumWnd(pWnd, WndRect, pPatient, NULL)
{
	CBusyCursor Cursor;


	m_CheckWnd = TRUE;//6.2.0

	//----------------------------------------------------
	RECT Rect;
	GetWindowRect(&Rect);
	real_t t = 3.0 * (0.03 * m_h) + m_g;
	real_t w = ((Rect.right - Rect.left) - 4.0 * m_g) / 3.0;
	real_t h = ((Rect.bottom - Rect.top - t) - 2.0 * m_g) / 2.0;
	::SetRect(&m_Rect[0], intRound(m_g), intRound(t), intRound(m_g + w), intRound(t + h)); // internal	wavefront Map
	::SetRect(&m_Rect[1], intRound(m_g + w + m_g), intRound(t), intRound(m_g + w + m_g + w), intRound(t + h)); // Cornea wavefront map	
	::SetRect(&m_Rect[2], intRound(m_g + w + m_g + w + m_g), intRound(t), intRound(m_g + w + m_g + w + m_g + w), intRound(t + h)); // Cornea axial map	

	::SetRect(&m_Rect[3], intRound(m_g), intRound(t + h + m_g), intRound(m_g + w), intRound(t + h + m_g + h));
	::SetRect(&m_Rect[4], intRound(m_g + w + m_g), intRound(t + h + m_g), intRound(m_g + w + m_g + w + m_g + w), intRound(t + h + m_g + h));

	//The rect of the planner gui
	::SetRect(&m_Rect[5], intRound(m_g + w + m_g + w + m_g), intRound(t), intRound(m_g + w + m_g + w + m_g + w), intRound(t + h));
	::SetRect(&m_Rect[6], intRound(m_g + w + m_g + w + m_g), intRound(t + h + m_g), intRound(m_g + w + m_g + w + m_g + w), intRound(t + h + m_g + h));

	::SetRect(&m_LargeRect, intRound(m_g), intRound(t), intRound(m_g + w + m_g + w), intRound(t + h + m_g + h));//6.2.0 Used for toric planner


	//6.2.0 The 'Planner button' For showing planner
	t = 0.02 * ::GetSystemMetrics(SM_CXSCREEN);
	int hBtn = intRound(t);
	int wBtn = intRound(3.5 * t);

	int fs = intRound(0.02 * ::GetSystemMetrics(SM_CYSCREEN));

	::SetRect(&Rect, m_Rect[4].right - 5 - wBtn, m_Rect[4].bottom - hBtn - 5, m_Rect[4].right - 5, m_Rect[4].bottom - 5);
	m_PlannerBtn.SetBkColor(NOCOLOR);
	m_PlannerBtn.SetText("Planner");
	m_PlannerBtn.SetTip("Show the toric planner");
	m_PlannerBtn.SetFont(fs);
	m_PlannerBtn.Create(_T(""), _T(""), WS_VISIBLE, Rect, this, IDC_PLANNER_BUTTON);

	m_PlannerBtn.ShowWindow(SW_SHOW);

	//----------------------------------------------------

	m_pWFExam = pWFExam;
	m_pCTExam = pCTExam;

	//----------------------------------------------------

	//6.2.0 The settings of planner Gui
	m_pWndSettings = &m_ThisWndSetting;

	CWndSettings* pWndSettings = GetWndSettings();

	pWndSettings->m_MapShowEye = TRUE;
	pWndSettings->m_MapShowMap = TRUE;
	pWndSettings->m_MapShowOsherRing = TRUE;
	
	if (m_pCTExam->m_ColorImage.m_RGBData.GetMem() != NULL)
	{
		m_ShowGrayEye = 0;
		m_ShowColorEye = 1;
	}
	else
	{
		m_ShowGrayEye = 1;
		m_ShowColorEye = 0;		
	}

	::TempSettings.NewPlannerSettings.m_MapShowIncision = 0;
	::TempSettings.NewPlannerSettings.m_MapShowPreopCorneal = 0;
	::TempSettings.NewPlannerSettings.m_MapShowPreopInternal = 0;
	::TempSettings.NewPlannerSettings.m_MapShowPreopEntireEye = 0;
	::TempSettings.NewPlannerSettings.m_MapPostopCorneal = 0;
	::TempSettings.NewPlannerSettings.m_MapPostopEntireEye = 1;
	::TempSettings.NewPlannerSettings.m_MapShowICLIndSteep = 0;
	::TempSettings.NewPlannerSettings.m_MapShowICLIndFlat = 0;

	::TempSettings.NewPlannerSettings.m_NewPlaner = 1;
	::Settings.m_PlannerSettings.m_MapShowZaldivarCaliper = 0;

	//----------------------------------------------------

	m_WFCTTORREFSumWnd = TRUE;



	::TempSettings.Com_r_max_um = ((m_pWFExam->m_WfSurface.m_r_max_um) < (m_pCTExam->m_WfSurface.m_r_max_um) ? m_pWFExam->m_WfSurface.m_r_max_um : m_pCTExam->m_WfSurface.m_r_max_um);

	m_ThisWndSetting.m_ZoneRadiusUm = intRound(m_pWFExam->m_WfSurface.m_r_max_um);

	m_ThisWndSetting.m_Mask.SetType(MASK_TOTAL_NO_DEFOCUS);
	m_ThisWndSetting.m_Piston = TRUE;
	m_ThisWndSetting.m_Tilt = TRUE;
	m_ThisWndSetting.SetDefaultScale();
	m_ThisWndSetting.SetDefaultColors(1);

	CreateChildWnd();
	Repaint();

	ShowWindow(SW_SHOW);
}

//***************************************************************************************

void CWFCTTorAliWnd::RepaintMemDC()
{
	CSumWnd::RepaintMemDC();

	if (m_CheckWnd)
	{
		WFCTTorDataInfo(m_pWFExam, m_pCTExam, m_Rect[3]);
		WFCTTorRefInfo(m_pWFExam, m_pCTExam, m_Rect[4]);

		// Draw the glass icon for print out  
		if (m_Printing || m_Saving)
		{
			for (int i = 0; i < 2; i++)
			{
				DrawGlassIcon(MASK_TOTAL_NO_DEFOCUS, 1, i);
			}
		}		
	}

	//6.2.0 Toric planner
	else
	{
		BOOL Rows[22] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
		CTExamInfo(m_pCTExam, m_Rect[5], 22, Rows);

		PreopData(m_Rect[6], 22, Rows);
	}
}

//***************************************************************************************

void CWFCTTorAliWnd::CreateChildWnd()
{
	if (m_CheckWnd)
	{

		for (int d = 0; d < 3; d++) SAFE_DELETE(m_pDispWnd[d]);

		int k = (m_d < 0 || m_d > 2) ? 0 : m_d;

		// corneal wavefront surface
		m_Surfaces[1] = m_pCTExam->m_WfSurface;

		// total wavefront surface
		m_Surfaces[2] = m_pWFExam->m_WfSurface;

		real_t r_max_um = 5000.0;
		r_max_um = __min(r_max_um, m_Surfaces[1].m_r_max_um);
		r_max_um = __min(r_max_um, m_Surfaces[2].m_r_max_um);
		r_max_um = __min(r_max_um, m_ThisWndSetting.m_ZoneRadiusUm);

		m_Surfaces[1].ChangeRMaxUm(r_max_um);
		m_Surfaces[1] &= m_ThisWndSetting.m_Mask;
		m_Surfaces[1].ComputePiston(m_ThisWndSetting.m_Piston);
		m_Surfaces[1].ComputeTilt(m_ThisWndSetting.m_Tilt);

		m_Surfaces[2].ChangeRMaxUm(r_max_um);
		m_Surfaces[2] &= m_ThisWndSetting.m_Mask;
		m_Surfaces[2].ComputePiston(m_ThisWndSetting.m_Piston);
		m_Surfaces[2].ComputeTilt(m_ThisWndSetting.m_Tilt);

		// internal optics wavefront surface
		m_Surfaces[0] = m_Surfaces[2];
		m_Surfaces[0] -= m_Surfaces[1];

		//0: Internal; 1:Corneal; 2:Total
		real_t SphEq[3], Sph[3], Cyl[3];
		int Axes[3];
		for (int i = 0; i < 3; i++) {
			CWFExam::GetSpheqSphCylAxis(m_Surfaces[i], 0, TRUE, SphEq[i], Sph[i], Cyl[i], Axes[i]);

			m_Axes[i] = Axes[i];//
		}

		//m_Rect[0] interrnal wavefront Map
		RECT Rect = m_Rect[0];

		CEyeWnd* pEyeWnd0 = new CEyeWnd(Rect, this);
		pEyeWnd0->m_Type = TYPE_IWFM;
		pEyeWnd0->m_SizeSmall = TRUE;
		pEyeWnd0->m_Method3D = FALSE;
		pEyeWnd0->m_LTLabel[0] = "INTERNAL - Wavefront Map";
		m_ThisWndSetting.m_Mask.GetName(pEyeWnd0->m_LTLabel[1]);
		pEyeWnd0->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
		pEyeWnd0->m_MapShowEye = FALSE;
		pEyeWnd0->m_MapShowMap = TRUE;
		pEyeWnd0->m_MapShowNumbers = FALSE;
		pEyeWnd0->m_MapShowPupil = FALSE;
		pEyeWnd0->m_MapShowCornealVertex = FALSE;
		pEyeWnd0->m_MapShowLimbus = FALSE;
		pEyeWnd0->m_MapShowSolidSurface = FALSE;
		pEyeWnd0->m_MapShowWireMesh = FALSE;
		pEyeWnd0->m_MapShowAstigmatismAxes = FALSE;
		pEyeWnd0->m_MapShowBLUEAstigmatismAxes = TRUE;
		pEyeWnd0->m_MapShowREDAstigmatismAxes = FALSE;
		pEyeWnd0->m_MapTranslucent = FALSE;
		pEyeWnd0->m_DonotShow90Degree = TRUE;
		pEyeWnd0->m_astigm_axis = Axes[0];
		pEyeWnd0->m_Unit = "µ";
		pEyeWnd0->m_Inc = 0.10;
		CScale* pScale0 = m_ThisWndSetting.GetScale();
		pEyeWnd0->m_NumColors = pScale0->m_NumColors;

		for (int i = 0; i < pScale0->m_NumColors; i++)
		{
			pEyeWnd0->m_Colors[i] = pScale0->m_Colors[i];
		}
		CWFExam::CreateWfm2D(pEyeWnd0->m_Map2D, m_Surfaces[0], 1.0 / pEyeWnd0->m_y_px_um);

		pEyeWnd0->CreateCentButtons();
		pEyeWnd0->CreateZoomButtons();
		pEyeWnd0->CreateZoneButtons();
		pEyeWnd0->CreateSoloMaskButtons(m_ThisWndSetting.m_Mask.GetType(), 1);
		m_pDispWnd[0] = pEyeWnd0;
		//interrnal wavefront Map Done


		//m_Rect[1] interrnal wavefront Map
		Rect = m_Rect[1];

		CEyeWnd* pEyeWnd1 = new CEyeWnd(Rect, this);
		pEyeWnd1->m_Type = TYPE_IWFM;
		pEyeWnd1->m_SizeSmall = TRUE;
		pEyeWnd1->m_Method3D = FALSE;
		pEyeWnd1->m_LTLabel[0] = "CORNEA - Wavefront Map";
		m_ThisWndSetting.m_Mask.GetName(pEyeWnd1->m_LTLabel[1]);
		pEyeWnd1->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
		pEyeWnd1->m_MapShowEye = FALSE;
		pEyeWnd1->m_MapShowMap = TRUE;
		pEyeWnd1->m_MapShowNumbers = FALSE;
		pEyeWnd1->m_MapShowPupil = FALSE;
		pEyeWnd1->m_MapShowCornealVertex = FALSE;
		pEyeWnd1->m_MapShowLimbus = FALSE;
		pEyeWnd1->m_MapShowSolidSurface = FALSE;
		pEyeWnd1->m_MapShowWireMesh = FALSE;
		pEyeWnd1->m_MapShowAstigmatismAxes = FALSE;
		pEyeWnd1->m_MapShowBLUEAstigmatismAxes = FALSE;
		pEyeWnd1->m_MapShowREDAstigmatismAxes = TRUE;
		pEyeWnd1->m_MapTranslucent = FALSE;
		pEyeWnd1->m_DonotShow90Degree = TRUE;
		pEyeWnd1->m_astigm_axis = Axes[1];
		pEyeWnd1->m_Unit = "µ";
		pEyeWnd1->m_Inc = 0.10;
		CScale* pScale1 = m_ThisWndSetting.GetScale();
		pEyeWnd1->m_NumColors = pScale1->m_NumColors;

		for (int i = 0; i < pScale1->m_NumColors; i++)
		{
			pEyeWnd1->m_Colors[i] = pScale1->m_Colors[i];
		}
		pEyeWnd1->m_pCTExam = m_pCTExam;
		m_pCTExam->CreateWfm2D(pEyeWnd1->m_Map2D, m_Surfaces[1], 1.0 / pEyeWnd1->m_y_px_um);

		pEyeWnd1->CreateCentButtons();
		pEyeWnd1->CreateZoomButtons();
		pEyeWnd1->CreateZoneButtons();
		pEyeWnd1->CreateSoloMaskButtons(m_ThisWndSetting.m_Mask.GetType(), 1);
		m_pDispWnd[1] = pEyeWnd1;
		//interrnal wavefront Map Done



		//m_Rect[2] cornea axial Map
		Rect = m_Rect[2];

		CEyeWnd* pEyeWnd2 = new CEyeWnd(Rect, this);
		pEyeWnd2->m_Type = TYPE_CAXM;
		pEyeWnd2->m_SizeSmall = TRUE;
		pEyeWnd2->m_Method3D = FALSE;
		pEyeWnd2->m_LTLabel[0] = "CORNEA - Axial Map";
		pEyeWnd2->m_MapShowEye = TRUE;
		pEyeWnd2->m_MapShowMap = TRUE;
		pEyeWnd2->m_MapShowNumbers = FALSE;
		pEyeWnd2->m_MapShowPupil = FALSE;
		pEyeWnd2->m_MapShowCornealVertex = FALSE;
		pEyeWnd2->m_MapShowLimbus = FALSE;
		pEyeWnd2->m_MapShowSolidSurface = FALSE;
		pEyeWnd2->m_MapShowWireMesh = FALSE;
		pEyeWnd2->m_MapShowKeratometry = TRUE;
		pEyeWnd2->m_MapTranslucent = FALSE;
		pEyeWnd2->m_astigm_axis = Axes[0];
		pEyeWnd2->m_Unit = "D";
		pEyeWnd2->m_Inc = 0.25;
		CScale* pScale2 = m_ThisWndSetting.GetScale();
		pEyeWnd2->m_NumColors = pScale2->m_NumColors;

		for (int i = 0; i < pScale2->m_NumColors; i++)
		{
			pEyeWnd2->m_Colors[i] = pScale2->m_Colors[i];
		}
		pEyeWnd2->m_pCTExam = m_pCTExam;
		m_pCTExam->CreateDpm2D(pEyeWnd2->m_Map2D, TYPE_CAXM, DIOPTERS, 1.0 / pEyeWnd2->m_y_px_um);
		pEyeWnd2->m_Circles.Create(1);
		pEyeWnd2->m_Circles[0].m_x_um = 0.0;
		pEyeWnd2->m_Circles[0].m_y_um = 0.0;
		pEyeWnd2->m_Circles[0].m_r_um = r_max_um;
		pEyeWnd2->m_Circles[0].m_color = BLACK;

		pEyeWnd2->CreateCentButtons();
		pEyeWnd2->CreateZoomButtons();
		m_pDispWnd[2] = pEyeWnd2;
		//interrnal axial Map Done



		//middle value
		real_t Cent = 0.0;
		real_t Inc = 0.10;
		real_t Step;	// step value

					//Wavefront map
		if (pScale0->m_StepAuto)
		{
			real_t Span = 0.0;

			for (int d = 0; d < 2; d++)
			{
				CEyeWnd* pEyeWnd = (CEyeWnd*)m_pDispWnd[d];
				real_t Min = pEyeWnd->m_Map2D.m_min;
				real_t Max = pEyeWnd->m_Map2D.m_max;
				Span = __max(Span, 2.0 * __max(fabs(Min), fabs(Max)));
			}
			real_t t = Span / pScale0->m_NumColors;
			Step = (int)(t / Inc) * Inc;
			if (t > Step + 0.001) Step += Inc;
		}
		else
		{
			Step = RealRound(pScale0->m_Step, Inc);
		}
		if (Step < Inc) Step = Inc;
		real_t StepMax = 10;
		if (Step > StepMax) Step = StepMax;
		//
		for (int d = 0; d < 3; d++)
		{
			CEyeWnd* pEyeWnd = (CEyeWnd*)m_pDispWnd[d];
			pEyeWnd->m_Cent = Cent;
			pEyeWnd->m_Step = Step;
		}


		//Axial map
		m_ThisWndSetting.SetDefaultColors(0);
		Inc = 0.25;
		CEyeWnd* pEyeWnd = (CEyeWnd*)m_pDispWnd[2];
		// middle value
		int MapUnit = DIOPTERS;
		if (pScale2->m_CentAuto)
		{
			Cent = m_pCTExam->m_ax0_dp;
		}
		else {
			Cent = pScale2->m_Cent;
		}

		Cent = RealRound(Cent, Inc);
		// step value
		Step;
		Step = RealRound(pScale2->m_Step, Inc);

		if (Step < Inc) Step = Inc;
		StepMax = 2.0;

		if (Step > StepMax) Step = StepMax;
		pEyeWnd->m_Cent = Cent;
		pEyeWnd->m_Step = Step;
		//middle value Done

		//Scan size
		for (int d = 0; d < 2; d++)
		{
			if (m_ThisWndSetting.m_ZoneRadiusUm >= ::TempSettings.Com_r_max_um)
			{
				m_pDispWnd[d]->m_IncZoneButton.SetEnabled(FALSE);
				m_pDispWnd[d]->m_IncZoneButton.Repaint();
			}

			if (m_ThisWndSetting.m_ZoneRadiusUm == 500)
			{
				m_pDispWnd[d]->m_DecZoneButton.SetEnabled(FALSE);
				m_pDispWnd[d]->m_DecZoneButton.Repaint();
			}
		}
		//Scan size Done
	}

	else//6.2.0 Shows the planner gui
	{
		for (int d = 0; d < 3; d++) SAFE_DELETE(m_pDispWnd[d]);

		CWndSettings* pWndSettings = GetWndSettings();

		CPlanWnd* pPlanWnd = new CPlanWnd(m_LargeRect, this);

		pPlanWnd->m_pCTExam = m_pCTExam;

		pPlanWnd->m_ShowGrayEye =  m_ShowGrayEye;
		pPlanWnd->m_ShowColorEye = m_ShowColorEye;

		pPlanWnd->m_ShowEye =  pWndSettings->m_MapShowEye;
		pPlanWnd->m_ShowMap =  pWndSettings->m_MapShowMap;
	
		pPlanWnd->m_ShowOsherRing = TRUE;//pWndSettings->m_MapShowOsherRing;

		pPlanWnd->m_ShowIncision = ::TempSettings.NewPlannerSettings.m_MapShowIncision;
		pPlanWnd->m_ShowPreopCorneal = ::TempSettings.NewPlannerSettings.m_MapShowPreopCorneal;
		pPlanWnd->m_ShowPreopInternal = ::TempSettings.NewPlannerSettings.m_MapShowPreopInternal;
		pPlanWnd->m_ShowPreopEntireEye = ::TempSettings.NewPlannerSettings.m_MapShowPreopEntireEye;
		pPlanWnd->m_PostopCorneal = ::TempSettings.NewPlannerSettings.m_MapPostopCorneal;
		pPlanWnd->m_PostopInternal = ::TempSettings.NewPlannerSettings.m_MapPostopInternal;
		pPlanWnd->m_PostopEntireEye = ::TempSettings.NewPlannerSettings.m_MapPostopEntireEye;
		pPlanWnd->m_ShowICLIndSteep = ::TempSettings.NewPlannerSettings.m_MapShowICLIndSteep;
		pPlanWnd->m_ShowICLIndFlat = ::TempSettings.NewPlannerSettings.m_MapShowICLIndFlat;

		m_pCTExam->CreateDpm2D(pPlanWnd->m_Map2D, TYPE_CAXM, DIOPTERS, 1.0 / pPlanWnd->m_y_px_um);

		m_pDispWnd[0] = pPlanWnd;
	}
}

//***************************************************************************************

//6.2.0 Switch between the default and planner gui
void CWFCTTorAliWnd::OnPlannerBtnClicked()
{
	m_CheckWnd = !m_CheckWnd;

	if (m_CheckWnd)
	{
		m_PlannerBtn.SetText("Planner");
		m_PlannerBtn.SetTip("Show the toric planner");

		m_PlannerBtn.Repaint();
	}
	else
	{
		m_pCTExam->m_OpData.m_OpType = 1;
		m_pCTExam->m_OpData.m_CorneaCycliderAdj = FALSE;
		m_pCTExam->m_OpData.Recalculate();		

		m_PlannerBtn.SetText("Return");
		m_PlannerBtn.SetTip("Show the toric check");

		m_PlannerBtn.Repaint();
	}

	CreateChildWnd();
	Repaint();
}



//***************************************************************************************

//6.2.0
void CWFCTTorAliWnd::PreopData(RECT Rect, int NumRows, BOOL* Rows)
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF black = m_Printing ? WHITE : BLACK;
	COLORREF light_gray = m_Printing ? DARK_GRAY : LIGHT_GRAY;
	COLORREF dark_gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;
	COLORREF light_yellow = m_Printing ? 0x0000f8f8 : 0x008fffff;

	m_MemDC.DrawRectangle(Rect, dark_gray, black);

	CString s;
	int num = NumRows;

	if (m_isCombo) num = num + 4;
	real_t h = (Rect.bottom - Rect.top) / (real_t)num;

	CMFont Font(intRound(1.1 * h), 400, "Arial");

	RECT Rect1;
	m_MemDC.MeasureRect("000° ", Rect1, Font);
	int w1 = Rect1.right - Rect1.left;

	m_MemDC.MeasureRect("   Post-Op Ent Eye Ast ", Rect1, Font);
	int w2 = Rect1.right - Rect1.left;

	m_MemDC.MeasureRect("       Post-Op IO Ast ", Rect1, Font);
	int w3 = Rect1.right - Rect1.left;

	int L = Rect.left;
	int T = Rect.top;
	int R = Rect.right;
	int B = Rect.bottom;
	int R4 = R - w1;
	int R3 = R - w2;
	int R2 = R3 - w1;
	int R1 = R3 - w3;

	real_t t = T;
	::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
	m_MemDC.WriteText(" Pre-Op Corneal Astigmatism", Rect, Font, white, 0);

	if (m_pCTExam->m_OpData.m_CorneaPreopCyl != INVALID_VALUE)
	{
		::SetRect(&Rect, L, intRound(t), R4, intRound(t + h + 1));
		s.Format(_T("%.2f D x "), m_pCTExam->m_OpData.m_CorneaPreopCyl);
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	if (m_pCTExam->m_OpData.m_CorneaPreopAxis != INVALID_VALUE)
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		s.Format(_T("%i° "), m_pCTExam->m_OpData.m_CorneaPreopAxis);
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	t += h;
	::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
	m_MemDC.WriteText(" Pre-Op Internal Optics Astigmatism", Rect, Font, white, 0);

	if (m_pCTExam->m_OpData.m_InternalPreopCyl != INVALID_VALUE)
	{
		s.Format(_T("%.2f D x "), m_pCTExam->m_OpData.m_InternalPreopCyl);
		::SetRect(&Rect, L, intRound(t), R4, intRound(t + h + 1));
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	if (m_pCTExam->m_OpData.m_InternalPreopAxis != INVALID_VALUE)
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		s.Format(_T("%i° "), m_pCTExam->m_OpData.m_InternalPreopAxis);
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	t += h;
	::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
	m_MemDC.WriteText(" Pre-Op Entire Eye Astigmatism", Rect, Font, white, 0);//cjfaaa do we need change here? 'to Entire eye

	if (m_pCTExam->m_OpData.m_EyePreopCyl != INVALID_VALUE)
	{
		s.Format(_T("%.2f D x "), m_pCTExam->m_OpData.m_EyePreopCyl);
		::SetRect(&Rect, L, intRound(t), R4, intRound(t + h + 1));
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	if (m_pCTExam->m_OpData.m_EyePreopAxis != INVALID_VALUE)
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		s.Format(_T("%i° "), m_pCTExam->m_OpData.m_EyePreopAxis);
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	t += h;

	if (m_pCTExam->m_OpData.m_OpType == 0 || m_pCTExam->m_OpData.m_OpType == 2)
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Surgical Incision Location", Rect, Font, white, 0);
		s.Format(_T("%i° "), m_pCTExam->m_OpData.m_CorneaIncisionAxis);
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	t += h;
	if (m_pCTExam->m_OpData.m_OpType == 0 || m_pCTExam->m_OpData.m_OpType == 2)
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Surgery Induced Corneal Astigmatism", Rect, Font, white, 0);
		::SetRect(&Rect, L, intRound(t), R4, intRound(t + h + 1));
		s.Format(_T("%.2f D x "), m_pCTExam->m_OpData.m_CorneaInducedCyl);
		m_MemDC.WriteText(s, Rect, Font, white, 2);
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		s.Format(_T("%i° "), m_pCTExam->m_OpData.m_CorneaInducedAxis);
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	t += h;
	if (m_pCTExam->m_OpData.m_OpType == 0)
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" IOL's Spherical Power", Rect, Font, white, 0);
		s.Format(_T("%.2f D "), m_pCTExam->m_OpData.m_IOLSph);
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	t += h;
	if (m_pCTExam->m_OpData.m_OpType == 0)
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" IOL's Placement Axis", Rect, Font, white, 0);
		s.Format(_T("%i° "), m_pCTExam->m_OpData.m_IOLPlacementAxis);
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}
	else if (m_pCTExam->m_OpData.m_OpType == 2)
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" ICL's Placement Axis", Rect, Font, white, 0);
		s.Format(_T("%i° "), m_pCTExam->m_OpData.m_ICL.m_PlacementAxis);
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	t += h;
	
	t -= 4 * h;	

	t += h;
	::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));

	m_MemDC.WriteText(" Post-Op Corneal Astigmatism", Rect, Font, white, 0);

	if (m_pCTExam->m_OpData.m_CorneaPostopCyl != INVALID_VALUE)
	{
		::SetRect(&Rect, L, intRound(t), R4, intRound(t + h + 1));
		s.Format(_T("%.2f D x "), m_pCTExam->m_OpData.m_CorneaPostopCyl);
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	if (m_pCTExam->m_OpData.m_CorneaPostopAxis != INVALID_VALUE)
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		s.Format(_T("%i° "), m_pCTExam->m_OpData.m_CorneaPostopAxis);
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	t += h;
	::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
	m_MemDC.WriteText(" Post-Op Internal Optics Astigmatism", Rect, Font, white, 0);

	if (m_pCTExam->m_OpData.m_InternalPostopCyl != INVALID_VALUE)
	{
		::SetRect(&Rect, L, intRound(t), R4, intRound(t + h + 1));
		s.Format(_T("%.2f D x "), m_pCTExam->m_OpData.m_InternalPostopCyl);
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	if (m_pCTExam->m_OpData.m_InternalPostopAxis != INVALID_VALUE)
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		s.Format(_T("%i° "), m_pCTExam->m_OpData.m_InternalPostopAxis);
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	t += h;
	::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
	m_MemDC.WriteText(" Post-Op Entire Eye Astigmatism", Rect, Font, white, 0);

	if (m_pCTExam->m_OpData.m_EyePostopCyl != INVALID_VALUE)
	{
		::SetRect(&Rect, L, intRound(t), R4, intRound(t + h + 1));
		s.Format(_T("%.2f D x "), m_pCTExam->m_OpData.m_EyePostopCyl);
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	if (m_pCTExam->m_OpData.m_EyePostopAxis != INVALID_VALUE)
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		s.Format(_T("%i° "), m_pCTExam->m_OpData.m_EyePostopAxis);
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}
}

//***************************************************************************************

//6.2.0 Right click menu for planner gui
void CWFCTTorAliWnd::CreateMenu()
{
	if (m_CheckWnd) return;

	CWndSettings* pWndSettings = GetWndSettings();

	if (m_pCTExam->m_ColorImage.m_RGBData.GetMem() != NULL)
	{
		m_pMenu->AppendItem(IDC_SHOW_MAP_GRAY_EYE_ITEM, FALSE, m_ShowGrayEye, "Show eye image", "");

		m_pMenu->AppendItem(IDC_SHOW_MAP_COLOR_EYE_ITEM, FALSE, m_ShowColorEye, "Show color eye image", "");

	}
	else
	{
		m_pMenu->AppendItem(IDC_SHOW_MAP_EYE_ITEM, FALSE, pWndSettings->m_MapShowEye, "Show eye image", "");
	}


	m_pMenu->AppendItem(IDC_SHOW_MAP_MAP_ITEM, FALSE, pWndSettings->m_MapShowMap, "Show axial map", "");
	//m_pMenu->AppendItem(IDC_SHOW_MAP_ZALDIVAR_ITEM, FALSE, pWndSettings->m_MapShowZaldivarCaliper, "Show Zaldivar caliper", "");
	m_pMenu->AppendItem(IDC_SHOW_MAP_OSHER_RING_ITEM, FALSE, pWndSettings->m_MapShowOsherRing, "Show Dynamic Angle scale", "");//Show Osher ring

	m_pMenu->AppendSeparator();

	if (m_pCTExam->m_OpData.m_OpType == 0)//IOL
	{
		m_pMenu->AppendItem(IDC_SHOW_MAP_INCISION_ITEM, FALSE, ::TempSettings.NewPlannerSettings.m_MapShowIncision, "Show surgical incision", "");
		m_pMenu->AppendItem(IDC_SHOW_MAP_PREOP_CORNEAL_ITEM, FALSE, ::TempSettings.NewPlannerSettings.m_MapShowPreopCorneal, "Show pre-op corneal", "");
		m_pMenu->AppendItem(IDC_SHOW_MAP_PREOP_INTERNAL_ITEM, FALSE, ::TempSettings.NewPlannerSettings.m_MapShowPreopInternal, "Show pre-op internal", "");
		m_pMenu->AppendItem(IDC_SHOW_MAP_PREOP_ENTIRE_ITEM, FALSE, ::TempSettings.NewPlannerSettings.m_MapShowPreopEntireEye, "Show pre-op entire eye", "");
		m_pMenu->AppendItem(IDC_SHOW_MAP_POSTOP_CORNEAL_ITEM, FALSE, ::TempSettings.NewPlannerSettings.m_MapPostopCorneal, "Show post-op corneal", "");
		m_pMenu->AppendItem(IDC_SHOW_MAP_POSTOP_INTERNAL_ITEM, FALSE, ::TempSettings.NewPlannerSettings.m_MapPostopInternal, "Show post-op internal", "");
		m_pMenu->AppendItem(IDC_SHOW_MAP_POSTOP_ENTIRE_ITEM, FALSE, ::TempSettings.NewPlannerSettings.m_MapPostopEntireEye, "Show post-op entire eye", "");
	}
	else if (m_pCTExam->m_OpData.m_OpType == 1)//Toric IOL enhancement
	{
		m_pMenu->AppendItem(IDC_SHOW_MAP_PREOP_CORNEAL_ITEM, FALSE, ::TempSettings.NewPlannerSettings.m_MapShowPreopCorneal, "Show pre-op corneal", "");
		m_pMenu->AppendItem(IDC_SHOW_MAP_PREOP_INTERNAL_ITEM, FALSE, ::TempSettings.NewPlannerSettings.m_MapShowPreopInternal, "Show pre-op internal", "");
		m_pMenu->AppendItem(IDC_SHOW_MAP_PREOP_ENTIRE_ITEM, FALSE, ::TempSettings.NewPlannerSettings.m_MapShowPreopEntireEye, "Show pre-op entire eye", "");
		m_pMenu->AppendItem(IDC_SHOW_MAP_POSTOP_CORNEAL_ITEM, FALSE, ::TempSettings.NewPlannerSettings.m_MapPostopCorneal, "Show post-op corneal", "");
		m_pMenu->AppendItem(IDC_SHOW_MAP_POSTOP_INTERNAL_ITEM, FALSE, ::TempSettings.NewPlannerSettings.m_MapPostopInternal, "Show post-op internal", "");
		m_pMenu->AppendItem(IDC_SHOW_MAP_POSTOP_ENTIRE_ITEM, FALSE, ::TempSettings.NewPlannerSettings.m_MapPostopEntireEye, "Show post-op entire eye", "");
	}
	else if (m_pCTExam->m_OpData.m_OpType == 2)//ICL
	{
		m_pMenu->AppendItem(IDC_SHOW_MAP_PREOP_CORNEAL_ITEM, FALSE, ::TempSettings.NewPlannerSettings.m_MapShowPreopCorneal, "Show pre-op corneal", "");
		m_pMenu->AppendItem(IDC_SHOW_MAP_PREOP_ENTIRE_ITEM, FALSE, ::TempSettings.NewPlannerSettings.m_MapShowPreopEntireEye, "Show pre-op entire eye", "");
		// m_pMenu->AppendItem(IDC_SHOW_MAP_POSTOP_ENTIRE_ITEM, FALSE, ::TempSettings.NewPlannerSettings.m_MapPostopEntireEye, "Show post-op entire eye", "");  
		m_pMenu->AppendItem(IDC_SHOW_MAP_ICL_STEEP_ITEM, FALSE, ::TempSettings.NewPlannerSettings.m_MapShowICLIndSteep, "Show ICL induced Steep", "");
		m_pMenu->AppendItem(IDC_SHOW_MAP_ICL_FLAT_ITEM, FALSE, ::TempSettings.NewPlannerSettings.m_MapShowICLIndFlat, "Show ICL induced Flat", "");
	}
}

//***************************************************************************************

void CWFCTTorAliWnd::OnShowColorEyeClicked()
{
	m_ShowColorEye = !m_ShowColorEye;

	if (m_ShowColorEye) m_ShowGrayEye = FALSE;

	CWndSettings* pWndSettings = GetWndSettings();
	pWndSettings->m_MapShowEye = m_ShowGrayEye || m_ShowColorEye;

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
}

//***************************************************************************************

void CWFCTTorAliWnd::OnShowGrayEyeClicked()
{
	m_ShowGrayEye = !m_ShowGrayEye;

	if (m_ShowGrayEye) m_ShowColorEye = FALSE;

	CWndSettings* pWndSettings = GetWndSettings();
	pWndSettings->m_MapShowEye = m_ShowGrayEye || m_ShowColorEye;

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
}


//***************************************************************************************

LRESULT CWFCTTorAliWnd::OnBigPlanWndShown(WPARAM wParam, LPARAM lParam)
{
	GetParent()->PostMessage(WM_NOTIFY_LBTN_DBCLK, (WPARAM)0, (LPARAM)0);

	//if (m_Screen == SCREEN_BIG_TORIC_PLANNER)
	//{
	//	m_Screen = SCREEN_TORIC_PLANNER;
	//	m_BigPlanWnd->ShowWindow(SW_HIDE);
	//	m_pSumWnd->Repaint();
	//	m_pSumWnd->ShowWindow(SW_SHOW);

	//	ShowCommonSummaryLevelButtons();

	//	if (m_SummaryType == 1) {
	//		ShowCTSummaryLevelButtons();
	//	}

	//	else if (m_SummaryType == 2) {
	//		ShowINSummaryLevelButtons();
	//	}

	//	else if (m_SummaryType == 0) {
	//		ShowWFSummaryLevelButtons();
	//	}
	//	else if (m_SummaryType == 3) {
	//		ShowTaskButtons(m_WhichTaskBtn);
	//	}
	//}
	//else if (m_Screen == SCREEN_TORIC_PLANNER)
	//{
	//	m_BigPlanWnd = new CPlanBigWnd(this, m_BigRect, m_DicomInfo, m_pCTExams[0]);
	//	m_Screen = SCREEN_BIG_TORIC_PLANNER;

	//	//531
	//	if (m_WhichTaskBtn != -1)
	//	{
	//		m_SummaryType = 3;
	//	}//531
	//	else
	//	{
	//		if (m_WFVfaButton.IsWindowVisible()) {
	//			m_SummaryType = 0;
	//		}

	//		else if (m_CTDblButton.IsWindowVisible()) {
	//			m_SummaryType = 1;
	//		}

	//		else if (m_INSumButton[1].IsWindowVisible()) {
	//			m_SummaryType = 2;
	//		}
	//	}

	//	HideAllSummaryLevelButtons();
	//	m_pSumWnd->ShowWindow(SW_HIDE);
	//	m_BigPlanWnd->Repaint();
	//	m_BigPlanWnd->ShowWindow(SW_SHOW);
	//}

	return 0;
}
//[520]

//***************************************************************************************

