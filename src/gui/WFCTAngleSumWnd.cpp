/*******************************************************************
Autor:         Jinfeng Chen
E-mail:        jinfengcc@gmail.com
Date:          04-26-2016
Description:   Transfomed angle k/a window from WF windows, add the functionality that
the limbus edge and length should be decided by wf and ct exam togther
*****************************************************************************************/


//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "EyeWnd.h"
#include "PSFWnd.h"
#include "MTFWnd.h"
#include "EEFWnd.h"
#include "LtrWnd.h"
#include "RMSWnd.h"
#include "ChartWnd.h"
#include "PolyWnd.h"
#include "BusyCursor.h"
#include "ImageDlg3.h"//520
#include "SelectImgDlg.h"//521
#include "NearLtrWnd.h"//530
#include "DOFWnd.h"//531

#include "GlobalFunctions.h"//
#include "WFCTAngleSumWnd.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CWFCTAngleSumWnd, CSumWnd)
	ON_COMMAND(IDC_EDIT_LIMBUS_ITEM, OnEditLimbusItemClicked)//520
	ON_COMMAND(IDC_SHOW_MAP_RRULER_ITEM, OnShowRRulerItemClicked)//520
	ON_COMMAND(IDC_SHOW_MAP_LRULER_ITEM, OnShowLRulerItemClicked)//520
	ON_COMMAND(IDC_SHOW_MAP_INLAY_ITEM, OnShowInalyItemClicked)//520

	ON_COMMAND(IDC_DEFAULT_GUI_ITEM, OnShowDefaultItemClicked)//521
	ON_COMMAND(IDC_SELECT_IMAGE_ITEM, OnSelEyeImgItemClicked)//521

	ON_MESSAGE(WM_SHOW_SIMPRMS, OnChangeRmsGUI)//530 Change the rms gui between simplify and details	
END_MESSAGE_MAP()

//***************************************************************************************

CWFCTAngleSumWnd::CWFCTAngleSumWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CWFExam* pWFExam, CCTExam* pCTExam,
	CWndSettings* pWndSettings, int k, int show)
	:
	CSumWnd(pWnd, WndRect, pPatient, pWndSettings)
{
	CBusyCursor Cursor;

	m_k = k;// = 0 : Visual Acuity; = 1 : WF/RMS ; 2 :Angle K/A ; // 3: Custom; 4: Depth of focus;

	m_MapShowRRuler = TRUE;//520
	m_MapShowLRuler = TRUE;//520
	m_MapShowInlay = TRUE;//520

						  //530
	if (m_isPresbia)
	{
		if (m_k == 0)
		{
			for (int i = 0; i < 4; i++) m_InsideZone[i] = ::NewSettings.m_ShowPreZone_WF_VA_Wnd[i];
		}
		if (m_k == 2)
		{
			for (int i = 0; i < 4; i++) m_InsideZone[i] = ::NewSettings.m_ShowPreZone_WF_AKA_Wnd[i];
		}
	}
	//530

	//----------------------------------------------------
	RECT Rect;
	GetWindowRect(&Rect);

	real_t t = 3.0 * (0.03 * m_h) + m_g;
	real_t w = ((Rect.right - Rect.left) - 4.0 * m_g) / 3.0;
	real_t h = ((Rect.bottom - Rect.top - t) - 2.0 * m_g) / 2.0;
	::SetRect(&m_Rect[0], intRound(m_g), intRound(t), intRound(m_g + w), intRound(t + h)); // map 1
	::SetRect(&m_Rect[1], intRound(m_g), intRound(t + h + m_g), intRound(m_g + w), intRound(t + h + m_g + h)); // map 2
	::SetRect(&m_Rect[2], intRound(m_g + w + m_g), intRound(t), intRound(m_g + w + m_g + w), intRound(t + h)); // map 3
	::SetRect(&m_Rect[3], intRound(m_g + w + m_g), intRound(t + h + m_g), intRound(m_g + w + m_g + w), intRound(t + h + m_g + h)); // map 4
	::SetRect(&m_Rect[4], intRound(m_g + w + m_g + w + m_g), intRound(t), intRound(m_g + w + m_g + w + m_g + w), intRound(t + h + m_g + h)); // info
	::SetRect(&m_LargeRect, intRound(m_g), intRound(t), intRound(m_g + w + m_g + w), intRound(t + h + m_g + h)); // large map

	m_pWFExam = pWFExam;
	m_pCTExam = pCTExam;

	// WFCT Angle k/a adjust wf eye limbus by ct info
	LimbusAdjust(m_pWFExam, m_pCTExam);
	// WFCT Angle k/a adjust wf eye limbus by ct info Done

	m_AverageExam = (m_pWFExam->m_Header.IsOD() ? ::OD_AverageExam : ::OS_AverageExam);

	for (int i = 0; i < m_AverageExam; i++)
	{
		if (pWFExam->m_Header.IsOD())
			m_SelectExamsID[i] = ::OD_SelectExamsID[i];
		else
			m_SelectExamsID[i] = ::OS_SelectExamsID[i];
	}

	//*007*[cjf***05052012],record the increase and decrease range
	::TempSettings.Com_r_max_um = m_pWFExam->m_WfSurface.m_r_max_um;
	//*007*[cjf***05052012]

	m_WFSumWnd = TRUE;//521

					  //m_scale = int((::TempSettings.Com_r_max_um + 1000)/1000) * 1000;//521 Test

	m_d = -1;
	for (int i = 0; i < 4; i++) {
		m_pWndSettings[i].m_ZoneRadiusUm = intRound(m_pWFExam->m_WfSurface.m_r_max_um);//530
		if (!m_pWndSettings[i].m_SizeSmall) {
			m_d = i;
			CreateChildWnd();
			break;
		}
	}
	if (m_d == -1) {
		for (m_d = 0; m_d < 4; m_d++) {
			CreateChildWnd();
		}
	}

	Repaint();

	ShowWindow(show);
}

//***************************************************************************************

void CWFCTAngleSumWnd::OnSizeLargeItemClicked()
{
	CBusyCursor Cursor;

	for (int i = 0; i < 4; i++) SAFE_DELETE(m_pDispWnd[i]);

	CWndSettings* pWndSettings = GetWndSettings();

	pWndSettings->m_SizeSmall = !pWndSettings->m_SizeSmall;

	if (pWndSettings->m_SizeSmall) {
		for (m_d = 0; m_d < 4; m_d++)
		{
			m_pWndSettings[m_d].m_SizeSmall = 1;
			CreateChildWnd();
		}
	}
	else {
		CreateChildWnd();
	}

	Repaint();
}

//***************************************************************************************

void CWFCTAngleSumWnd::RepaintMemDC()
{
	CSumWnd::RepaintMemDC();

	//BOOL Rows[44] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

	if (m_InsideZone[m_d] == TRUE)
	{
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
	}
	WFCTExamLargeInfo(m_pWFExam, m_Rect[4]);//530  Original is WFExamInfo

											// Draw the glass icon for print out  
	if (m_Printing || m_Saving)
	{
		int this_d;
		BOOL SmallSize = TRUE;

		for (int i = 0; i < 4; i++)
		{
			if (m_pWndSettings[i].m_SizeSmall == 0)
			{
				this_d = i;
				SmallSize = FALSE;
				break;
			}
		}

		if (SmallSize)
		{
			for (int i = 0; i < 4; i++)
			{
				int GUIType = m_pWndSettings[i].m_Type;

				if (GUIType != TYPE_TEYE && GUIType != TYPE_DOFF)
				{
					int MaskType = m_pWndSettings[i].m_Mask.GetType();
					DrawGlassIcon(MaskType, 1, i);
				}
			}
		}
		else
		{
			int GUIType = m_pWndSettings[this_d].m_Type;

			if (GUIType != TYPE_TEYE && GUIType != TYPE_DOFF)
			{
				int MaskType = m_pWndSettings[this_d].m_Mask.GetType();
				DrawGlassIcon(MaskType, 0, this_d);
			}
		}
	}
	//  Draw the glass icon for print out  Done
}

//***************************************************************************************

void CWFCTAngleSumWnd::CreateChildWnd()
{
	CBusyCursor Cursor;

	//530
	if (m_InsideZone[m_d] == TRUE)  m_pWFExam->Presbia24Process();
	else                           m_pWFExam->Process();
	//530

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

	//530
	if (m_isPresbia)
	{
		if (m_k == 0)
			::NewSettings.m_ShowPreZone_WF_VA_Wnd[m_d] = m_InsideZone[m_d];
		else if (m_k == 2)
			::NewSettings.m_ShowPreZone_WF_AKA_Wnd[m_d] = m_InsideZone[m_d];
	}
	//530

	SAFE_DELETE(m_pDispWnd[m_d]);

	CWndSettings* pWndSettings = GetWndSettings();
	pWndSettings->m_ZoneRadiusUm = pWndSettings->m_ZoneRadiusUm;

	//530
	if (m_InsideZone[m_d] == TRUE && pWndSettings->m_ZoneRadiusUm > 1700)
	{
		pWndSettings->m_ZoneRadiusUm = 1700;
	}
	//530

	if (pWndSettings->m_Type != TYPE_TEYE &&
		pWndSettings->m_Type != TYPE_TWFM &&
		pWndSettings->m_Type != TYPE_TRFM &&
		pWndSettings->m_Type != TYPE_TPSF &&
		pWndSettings->m_Type != TYPE_TMTF &&
		pWndSettings->m_Type != TYPE_TLTR &&
		pWndSettings->m_Type != TYPE_TEEF &&
		pWndSettings->m_Type != TYPE_TRMS &&
		pWndSettings->m_Type != TYPE_TAZR &&
		pWndSettings->m_Type != TYPE_DOFF //531
		)
	{
		pWndSettings->m_Type = TYPE_TWFM;
	}

	if (pWndSettings->m_Type == TYPE_TRMS) {
		if (pWndSettings->m_Method3D) {
			pWndSettings->m_RMSIndividual = TRUE;
		}
	}
	else if (pWndSettings->m_Type == TYPE_TEYE) {
		pWndSettings->m_Method3D = FALSE;
		pWndSettings->m_MapShowEye = TRUE;
		//pWndSettings->m_MapShowMap = FALSE;
		//pWndSettings->m_MapShowKeratometry = FALSE;
		//pWndSettings->m_MapShowNumbers = FALSE;
		pWndSettings->m_MapShowPoints = FALSE;
		pWndSettings->m_MapShowRings = FALSE;
		pWndSettings->m_MapShowWavetouchLens = FALSE;
	}
	else if (pWndSettings->m_Type == TYPE_TLTR) {
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

	m_Surfaces[m_d] = m_pWFExam->m_WfSurface;
	real_t r_max_um = 5000.0;
	r_max_um = __min(r_max_um, m_Surfaces[m_d].m_r_max_um);
	r_max_um = __min(r_max_um, pWndSettings->m_ZoneRadiusUm);

	m_Surfaces[m_d].ChangeRMaxUm(r_max_um);

	//530
	CString PresbisRBs;
	if (m_InsideZone[m_d] == TRUE)
	{
		real_t SphEq, Sph, Cyl; int Axis;

		CWFExam::GetSpheqSphCylAxis(m_Surfaces[m_d], ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
		//
		if (::Settings.m_RefractionRounding == 0) {
			Sph = round8(Sph);
			Cyl = round8(Cyl);
		}
		PresbisRBs.Format(_T(" %+.2f D %+.2f D x %i° "), Sph, Cyl, Axis);
	}
	//530

	m_Surfaces[m_d] &= pWndSettings->m_Mask;
	m_Surfaces[m_d].ComputePiston(pWndSettings->m_Piston);
	m_Surfaces[m_d].ComputeTilt(pWndSettings->m_Tilt);

	RECT Rect = pWndSettings->m_SizeSmall ? m_Rect[m_d] : m_LargeRect;

	switch (pWndSettings->m_Type) {

	case TYPE_TEYE:
	{
		CEyeWnd* pEyeWnd = new CEyeWnd(Rect, this);
		pEyeWnd->m_Type = pWndSettings->m_Type;
		pEyeWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
		pEyeWnd->m_Method3D = pWndSettings->m_Method3D;

		pEyeWnd->m_RBLabelFontSizePrc = 0.030;

		pEyeWnd->m_RBLabelBkColor = m_Printing ? WHITE : BLACK;
		pEyeWnd->m_RBLabelColor[0] = GREEN;
		pEyeWnd->m_RBLabelColor[1] = GREEN;
		pEyeWnd->m_RBLabelColor[2] = GREEN;

		if (m_k == 2 && m_pWFExam->m_Image.m_RGBData)//520, 521
		{
			if (m_pWFExam->m_Header.m_AngleKappaOK) {
				pEyeWnd->m_RBLabel[0] = "Angle Kappa";
				pEyeWnd->m_RBLabel[1].Format(_T("(X, Y) = (%.3f, %.3f)"),//3
					m_pWFExam->m_Header.m_AngleKappaXUm * 0.001,
					m_pWFExam->m_Header.m_AngleKappaYUm * 0.001);
				pEyeWnd->m_RBLabel[2].Format(_T("(R, A) = (%.3f, %i)"),
					m_pWFExam->m_Header.m_AngleKappaRUm * 0.001,
					m_pWFExam->m_Header.m_AngleKappaADg);
			}
			pEyeWnd->m_RBLabelColor[3] = BLUE;
			pEyeWnd->m_RBLabelColor[4] = BLUE;
			pEyeWnd->m_RBLabelColor[5] = BLUE;

			if (m_pWFExam->m_Header.m_AngleAlphaOK) {
				pEyeWnd->m_RBLabel[3] = "Angle Alpha";
				pEyeWnd->m_RBLabel[4].Format(_T("(X, Y) = (%.3f, %.3f)"),//4
					m_pWFExam->m_Header.m_AngleAlphaXUm * 0.001,
					m_pWFExam->m_Header.m_AngleAlphaYUm * 0.001);
				pEyeWnd->m_RBLabel[5].Format(_T("(R, A) = (%.3f, %i)"),
					m_pWFExam->m_Header.m_AngleAlphaRUm * 0.001,
					m_pWFExam->m_Header.m_AngleAlphaADg);
			}


			//520	
			pEyeWnd->m_LBLabelColor[0] = RGB(255, 128, 0);//aaa
			pEyeWnd->m_LBLabelColor[1] = RGB(255, 128, 0);

			pEyeWnd->m_LBLabelColor[2] = YELLOW;
			pEyeWnd->m_LBLabelColor[3] = YELLOW;
			pEyeWnd->m_LBLabelColor[4] = YELLOW;
			pEyeWnd->m_LBLabelColor[5] = YELLOW;
			pEyeWnd->m_LBLabelColor[6] = WHITE;
			pEyeWnd->m_LBLabelColor[7] = WHITE;
			pEyeWnd->m_LBLabelColor[8] = WHITE;
			pEyeWnd->m_LBLabelColor[9] = WHITE;
			pEyeWnd->m_LBLabelColor[10] = WHITE;

			if (m_pWFExam->m_Header.m_LinearRulerOK)
			{
				pEyeWnd->m_LBLabel[0] = "Linear Ruler";
				real_t dis = hyp(m_pWFExam->m_Image.m_LRa_x0_um - m_pWFExam->m_Image.m_LRa_x1_um, m_pWFExam->m_Image.m_LRa_y0_um - m_pWFExam->m_Image.m_LRa_y1_um) * 0.001;
				pEyeWnd->m_LBLabel[1].Format(_T("(D) = (%.3f)"), dis);
			}

			if (m_pWFExam->m_Header.m_AngleRulerOK)
			{
				pEyeWnd->m_LBLabel[2] = "Radial Ruler";
				pEyeWnd->m_LBLabel[3].Format(_T("(D) = (%.3f)"), m_pWFExam->m_Image.m_Ra_r_um * 0.002);

				pEyeWnd->m_LBLabel[4].Format(_T("(X, Y) = (%.3f, %.3f)"),
					m_pWFExam->m_Header.m_AngleRulerXUm * 0.001,
					m_pWFExam->m_Header.m_AngleRulerYUm * 0.001);

				pEyeWnd->m_LBLabel[5].Format(_T("(R, A) = (%.3f, %i)"),
					m_pWFExam->m_Header.m_AngleRulerRUm * 0.001,
					m_pWFExam->m_Header.m_AngleRulerADg);
			}
			//520

			// disk 520
			if (m_pWFExam->m_Header.m_InlayOK && m_pWFExam->m_Image.m_In_r_um == 0)
			{
				m_pWFExam->m_Header.m_InlayOK = FALSE;
			}
			if (m_pWFExam->m_Header.m_InlayOK)
			{
				pEyeWnd->m_LBLabel[6] = "Disk";

				pEyeWnd->m_LBLabel[7].Format(_T("(D) = (%.3f)"), m_pWFExam->m_Image.m_In_r_um * 0.002);
				if (m_pWFExam->m_Image.m_In_In_r_um < 0) m_pWFExam->m_Image.m_In_In_r_um = 0;
				pEyeWnd->m_LBLabel[8].Format(_T("(Inner_D) = (%.3f)"), m_pWFExam->m_Image.m_In_In_r_um * 0.002);

				pEyeWnd->m_LBLabel[9].Format(_T("(X, Y) = (%.3f, %.3f)"),
					m_pWFExam->m_Header.m_InlayXUm * 0.001,
					m_pWFExam->m_Header.m_InlayYUm * 0.001);

				pEyeWnd->m_LBLabel[10].Format(_T("(R, A) = (%.3f, %i)"),
					m_pWFExam->m_Header.m_InlayAngleRUm * 0.001,
					m_pWFExam->m_Header.m_InlayAngleADg);
			}
			//Done 520
		}

		pEyeWnd->m_MapShowEye = pWndSettings->m_MapShowEye;
		pEyeWnd->m_MapShowPupil = pWndSettings->m_MapShowPupil;
		pEyeWnd->m_MapShowCornealVertex = pWndSettings->m_MapShowCornealVertex;
		pEyeWnd->m_MapShowLimbus = pWndSettings->m_MapShowLimbus;
		pEyeWnd->m_MapShowPoints = pWndSettings->m_MapShowPoints;
		pEyeWnd->m_MapShowWavetouchLens = pWndSettings->m_MapShowWavetouchLens;

		if (m_k == 2)
		{
			pEyeWnd->m_ShowRadialRuler = m_MapShowRRuler;//520
			pEyeWnd->m_ShowLinearRuler = m_MapShowLRuler;//520
			pEyeWnd->m_ShowInlay = m_MapShowInlay;//520
		}
		else
		{
			pEyeWnd->m_ShowRadialRuler = FALSE;//520
			pEyeWnd->m_ShowLinearRuler = FALSE;//520
			pEyeWnd->m_ShowInlay = FALSE;//520
		}

		pEyeWnd->m_pWFExam = m_pWFExam;
		m_pDispWnd[m_d] = pEyeWnd;
	}
	break;

	case TYPE_TWFM:
	{
		CEyeWnd* pEyeWnd = new CEyeWnd(Rect, this);

		//CNewEyeWnd* pEyeWnd = new CNewEyeWnd(Rect, this, m_scale);//521 Test

		pEyeWnd->m_Type = pWndSettings->m_Type;
		pEyeWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
		pEyeWnd->m_Method3D = pWndSettings->m_Method3D;
		pEyeWnd->m_LTLabel[0] = "Wavefront Map";
		pWndSettings->m_Mask.GetName(pEyeWnd->m_LTLabel[1]);
		pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
		//530
		if (m_InsideZone[m_d] == TRUE) pEyeWnd->m_LTLabel[2] = "Presbia 2.4mm";
		//530
		pEyeWnd->m_MapShowEye = pWndSettings->m_MapShowEye;
		pEyeWnd->m_MapShowMap = pWndSettings->m_MapShowMap;
		pEyeWnd->m_MapShowNumbers = pWndSettings->m_MapShowNumbers;
		pEyeWnd->m_MapShowPupil = pWndSettings->m_MapShowPupil;
		pEyeWnd->m_MapShowCornealVertex = pWndSettings->m_MapShowCornealVertex;
		pEyeWnd->m_MapShowLimbus = pWndSettings->m_MapShowLimbus;
		pEyeWnd->m_MapShowSolidSurface = pWndSettings->m_MapShowSolidSurface;
		pEyeWnd->m_MapShowWireMesh = pWndSettings->m_MapShowWireMesh;
		pEyeWnd->m_MapTranslucent = pWndSettings->m_MapTranslucent;
		pEyeWnd->m_Unit = "µ";
		pEyeWnd->m_Inc = pWndSettings->GetIncrement();
		CScale* pScale = pWndSettings->GetScale();
		pEyeWnd->m_NumColors = pScale->m_NumColors;
		for (int i = 0; i < pScale->m_NumColors; i++) {
			pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
		}
		pEyeWnd->m_pWFExam = m_pWFExam;

		if (pWndSettings->m_Method3D) {
			if (m_InsideZone[m_d] == TRUE) CWFExam::CreatePresbiaWfm3D(pEyeWnd->m_Map3D, m_Surfaces[m_d]);//530
			else                          CWFExam::CreateWfm3D(pEyeWnd->m_Map3D, m_Surfaces[m_d]);
		}
		else {
			if (m_InsideZone[m_d] == TRUE) CWFExam::CreatePresbiaWfm2D(pEyeWnd->m_Map2D, m_Surfaces[m_d], 1.0 / pEyeWnd->m_y_px_um);//530
			else                          CWFExam::CreateWfm2D(pEyeWnd->m_Map2D, m_Surfaces[m_d], 1.0 / pEyeWnd->m_y_px_um);
		}

		pEyeWnd->CreateCentButtons();
		pEyeWnd->CreateZoomButtons();

		if (m_InsideZone[m_d] == TRUE) pEyeWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 2);//531
		else                          pEyeWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 1);//531

																										//530
																										//pEyeWnd->CreateZoneButtons(); 
		if (m_InsideZone[m_d] != TRUE)
		{
			pEyeWnd->CreateZoneButtons();
		}
		else
		{
			pEyeWnd->m_RBLabel[5] = PresbisRBs;
		}
		//530

		m_pDispWnd[m_d] = pEyeWnd;
	}
	break;

	case TYPE_TRFM:
	{
		CEyeWnd* pEyeWnd = new CEyeWnd(Rect, this);
		pEyeWnd->m_Type = pWndSettings->m_Type;
		pEyeWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
		pEyeWnd->m_Method3D = pWndSettings->m_Method3D;
		pEyeWnd->m_LTLabel[0] = "Refraction Map";
		pWndSettings->m_Mask.GetName(pEyeWnd->m_LTLabel[1]);
		pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
		//530
		if (m_InsideZone[m_d] == TRUE) pEyeWnd->m_LTLabel[2] = "Presbia 2.4mm";
		//530
		pEyeWnd->m_MapShowEye = pWndSettings->m_MapShowEye;
		pEyeWnd->m_MapShowMap = pWndSettings->m_MapShowMap;
		pEyeWnd->m_MapShowNumbers = pWndSettings->m_MapShowNumbers;
		pEyeWnd->m_MapShowPupil = pWndSettings->m_MapShowPupil;
		pEyeWnd->m_MapShowCornealVertex = pWndSettings->m_MapShowCornealVertex;
		pEyeWnd->m_MapShowLimbus = pWndSettings->m_MapShowLimbus;
		pEyeWnd->m_MapShowSolidSurface = pWndSettings->m_MapShowSolidSurface;
		pEyeWnd->m_MapShowWireMesh = pWndSettings->m_MapShowWireMesh;
		pEyeWnd->m_MapTranslucent = pWndSettings->m_MapTranslucent;
		pEyeWnd->m_Unit = "D";
		pEyeWnd->m_Inc = pWndSettings->GetIncrement();
		CScale* pScale = pWndSettings->GetScale();
		pEyeWnd->m_NumColors = pScale->m_NumColors;
		for (int i = 0; i < pScale->m_NumColors; i++) {
			pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
		}
		pEyeWnd->m_pWFExam = m_pWFExam;
		if (pWndSettings->m_Method3D) {
			if (m_InsideZone[m_d] == TRUE) CWFExam::CreatePresbiaRfm3D(pEyeWnd->m_Map3D, m_Surfaces[m_d]);//530
			else                          CWFExam::CreateRfm3D(pEyeWnd->m_Map3D, m_Surfaces[m_d]);
		}
		else {
			if (m_InsideZone[m_d] == TRUE) CWFExam::CreatePresbiaRfm2D(pEyeWnd->m_Map2D, m_Surfaces[m_d], 1.0 / pEyeWnd->m_y_px_um);//530
			else                          CWFExam::CreateRfm2D(pEyeWnd->m_Map2D, m_Surfaces[m_d], 1.0 / pEyeWnd->m_y_px_um);
		}
		pEyeWnd->CreateCentButtons();
		pEyeWnd->CreateZoomButtons();
		if (m_InsideZone[m_d] == TRUE) pEyeWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 2);//531
		else                       	  pEyeWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 1);//531

																										//530
																										//pEyeWnd->CreateZoneButtons(); 
		if (m_InsideZone[m_d] != TRUE)
		{
			pEyeWnd->CreateZoneButtons();
		}
		else
		{
			pEyeWnd->m_RBLabel[5] = PresbisRBs;
		}
		//530

		m_pDispWnd[m_d] = pEyeWnd;
	}
	break;

	case TYPE_TPSF:
	{
		CPSFWnd* pPSFWnd = new CPSFWnd(Rect, this);
		CWFExam::CreatePsf(m_Surfaces[m_d], pPSFWnd->m_Psf, pWndSettings->m_EEFType);
		pPSFWnd->m_Type = pWndSettings->m_Type;
		pPSFWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
		pPSFWnd->m_Method3D = pWndSettings->m_Method3D;
		pPSFWnd->m_LTLabel[0] = "Point Spread Function";
		pWndSettings->m_Mask.GetName(pPSFWnd->m_LTLabel[1]);
		pPSFWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
		//530
		if (m_InsideZone[m_d] == TRUE) pPSFWnd->m_LTLabel[2] = "Presbia 2.4mm";
		//530
		pPSFWnd->m_ShowEEF = pWndSettings->m_ShowEEF;
		pPSFWnd->CreateZoomButtons();
		if (m_InsideZone[m_d] == TRUE) pPSFWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 2);//531
		else                       	  pPSFWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 1);//531

																										//530
																										//pPSFWnd->CreateZoneButtons(); 
		if (m_InsideZone[m_d] != TRUE) pPSFWnd->CreateZoneButtons();
		else
		{
			pPSFWnd->m_RBLabel[5] = PresbisRBs;
		}
		//530

		m_pDispWnd[m_d] = pPSFWnd;
	}
	break;

	case TYPE_TLTR:
	{
		CLtrWnd* pLtrWnd = new CLtrWnd(Rect, this);
		pLtrWnd->m_DysVal = -1;//531

		CWFExam::CreateLtr(m_Surfaces[m_d], pLtrWnd->m_Ltr, pWndSettings->m_Ltr, pWndSettings->m_LtrLine, pWndSettings->m_LtrOrientation);
		pLtrWnd->m_Type = pWndSettings->m_Type;
		pLtrWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
		pLtrWnd->m_Method3D = pWndSettings->m_Method3D;
		//pLtrWnd->m_LTLabel[0].Format(_T("Letter 20 / %i", pWndSettings->m_LtrLine);
		pLtrWnd->m_LTLabel[0] = "Letter";//531
		pWndSettings->m_Mask.GetName(pLtrWnd->m_LTLabel[1]);
		pLtrWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
		//530
		if (m_InsideZone[m_d] == TRUE) pLtrWnd->m_LTLabel[2] = "Presbia 2.4mm";
		//530
		pLtrWnd->CreateZoomButtons();
		if (m_InsideZone[m_d] == TRUE) pLtrWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 2);//531
		else                       	  pLtrWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 1);//531

																										//530
																										//pLtrWnd->CreateZoneButtons(); 
		if (m_InsideZone[m_d] != TRUE)
		{
			pLtrWnd->CreateZoneButtons();
		}
		else
		{
			pLtrWnd->m_RBLabel[5] = PresbisRBs;
		}
		//530

		m_pDispWnd[m_d] = pLtrWnd;
	}
	break;

	case TYPE_TMTF:
	{
		CMTFWnd* pMTFWnd = new CMTFWnd(Rect, this);
		CWFExam::CreateMtf(m_Surfaces[m_d], pMTFWnd->m_Mtf);
		pMTFWnd->m_Type = pWndSettings->m_Type;
		pMTFWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
		pMTFWnd->m_Method3D = pWndSettings->m_Method3D;

		pMTFWnd->m_ShowDys = FALSE;//531

		pMTFWnd->m_LTLabel[0] = "Modulation Transfer Function";
		pWndSettings->m_Mask.GetName(pMTFWnd->m_LTLabel[1]);
		pMTFWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
		//530
		if (m_InsideZone[m_d] == TRUE) pMTFWnd->m_LTLabel[2] = "Presbia 2.4mm";
		//530
		pMTFWnd->m_Average = pWndSettings->m_MTFAverage;
		pMTFWnd->m_Angle = pWndSettings->m_MTFAngle;
		if (!pWndSettings->m_Method3D && !pWndSettings->m_MTFAverage)
		{
			pMTFWnd->CreateSlider();
		}

		//530
		//pMTFWnd->CreateZoneButtons(); 
		if (m_InsideZone[m_d] != TRUE)
		{
			pMTFWnd->CreateZoneButtons();
		}
		else
		{
			pMTFWnd->m_RBLabel[5] = PresbisRBs;
		}
		if (m_InsideZone[m_d] == TRUE) pMTFWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 2);//531
		else                       	  pMTFWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 1);//531
																										//530

		m_pDispWnd[m_d] = pMTFWnd;
	}
	break;

	case TYPE_TEEF:
	{
		CEEFWnd* pEEFWnd = new CEEFWnd(Rect, this);
		CWFExam::CreatePsf(m_Surfaces[m_d], pEEFWnd->m_Psf, pWndSettings->m_EEFType);
		pEEFWnd->m_Type = pWndSettings->m_Type;
		pEEFWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
		pEEFWnd->m_Method3D = pWndSettings->m_Method3D;
		pEEFWnd->m_LTLabel[0] = "Encircled Energy Function";
		pWndSettings->m_Mask.GetName(pEEFWnd->m_LTLabel[1]);
		pEEFWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
		//530
		if (m_InsideZone[m_d] == TRUE) pEEFWnd->m_LTLabel[2] = "Presbia 2.4mm";
		//530

		//530
		//pEEFWnd->CreateZoneButtons(); 
		if (m_InsideZone[m_d] != TRUE) pEEFWnd->CreateZoneButtons();
		else
		{
			pEEFWnd->m_RBLabel[5] = PresbisRBs;
		}
		//530

		if (m_InsideZone[m_d] == TRUE) pEEFWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 2);//531
		else                          pEEFWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 1);//531

		m_pDispWnd[m_d] = pEEFWnd;
	}
	break;

	case TYPE_TRMS:
	{
		CRMSWnd* pRMSWnd = new CRMSWnd(Rect, this);
		pRMSWnd->m_SimpliedRMS = pWndSettings->m_SimplifiedRMS;//530
		pRMSWnd->m_Type = pWndSettings->m_Type;
		pRMSWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
		pRMSWnd->m_Method3D = pWndSettings->m_Method3D;
		pRMSWnd->m_LTLabel[0] = "Root Mean Square";
		pWndSettings->m_Mask.GetName(pRMSWnd->m_LTLabel[1]);
		pRMSWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
		//530
		if (m_InsideZone[m_d] == TRUE) pRMSWnd->m_LTLabel[2] = "Presbia 2.4mm";
		//530
		pRMSWnd->m_Surface = m_Surfaces[m_d];
		pRMSWnd->m_Individual = pWndSettings->m_RMSIndividual;
		if (!pRMSWnd->m_Method3D && pRMSWnd->m_Individual) pRMSWnd->CreateSimpButtons(!pWndSettings->m_SimplifiedRMS);//530
		pRMSWnd->CreateZoomButtons();

		//530
		//pRMSWnd->CreateZoneButtons(); 
		if (m_InsideZone[m_d] != TRUE) pRMSWnd->CreateZoneButtons();
		else
		{
			pRMSWnd->m_RBLabel[5] = PresbisRBs;
		}
		//530

		if (m_InsideZone[m_d] == TRUE) pRMSWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 2);//531
		else                          pRMSWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 1);//531

		m_pDispWnd[m_d] = pRMSWnd;
	}
	break;

	case TYPE_TAZR:
	{
		CChartWnd* pAZRWnd = new CChartWnd(Rect, this);
		pAZRWnd->m_Type = pWndSettings->m_Type;
		pAZRWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
		pAZRWnd->m_Method3D = pWndSettings->m_Method3D;
		pWndSettings->m_Mask.GetName(pAZRWnd->m_LTLabel[1]);
		pAZRWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);

		if (m_InsideZone[m_d] == TRUE)
		{
			pAZRWnd->m_LTLabel[2] = "Presbia 2.4mm";
		}

		real_t dr_um = 100.0;
		int n = (int)(r_max_um / dr_um) + 1;

		pAZRWnd->m_MainLabels.Create(2);
		pAZRWnd->m_MainLabels[0] = "Annular Refraction";
		pAZRWnd->m_MainLabels[1] = "Zonal Refraction";

		pAZRWnd->m_Colors.Create(2);
		pAZRWnd->m_Colors[0] = MAGENTA;
		pAZRWnd->m_Colors[1] = m_Printing ? BLUE : CYAN;

		pAZRWnd->m_YUnitLabel = "diopters";

		pAZRWnd->m_XUnitLabel = "millimeters";

		pAZRWnd->m_BLabels.Create(n);

		for (int i = 0; i < n; i++)
		{
			real_t d_mm = 0.002 * i * dr_um;
			if (fabs(d_mm - (int)d_mm) < 0.001)
			{
				pAZRWnd->m_BLabels[i].Format(_T("%i"), (int)d_mm);
			}
		}

		pAZRWnd->m_Values.Create(n, 2);

		for (int i = 0; i < n; i++)
		{
			real_t r_um = i * dr_um;
			pAZRWnd->m_Values(i, 0) = CWFExam::GetAnnularRfAt(m_Surfaces[m_d], r_um);
			pAZRWnd->m_Values(i, 1) = CWFExam::GetAverageRfAt(m_Surfaces[m_d], r_um);
		}

		if (m_InsideZone[m_d] != TRUE)
		{
			pAZRWnd->CreateZoneButtons();
		}
		else
		{
			pAZRWnd->m_RBLabel[5] = PresbisRBs;
		}

		if (m_InsideZone[m_d] == TRUE)
		{
			pAZRWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 2);
		}
		else
		{
			pAZRWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 1);
		}

		m_pDispWnd[m_d] = pAZRWnd;
	}
	break;

	case TYPE_DOFF:
	{
		if (m_X.GetSize() == 0)
		{
			::CreateThroughFocusVSOTFA(m_pWFExam->m_WfSurface, m_X, m_F);
		}

		CDOFWnd* pDOFWnd = new CDOFWnd(Rect, m_X, m_F, this);
		pDOFWnd->m_Type = pWndSettings->m_Type;
		pDOFWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
		pDOFWnd->m_LTLabel[0] = "Through-focus VSOTFa";
		pDOFWnd->CreateSwitchBtn();

		m_pDispWnd[m_d] = pDOFWnd;
	}
	break;
	//531


	//迷雾汀芳，暗夜黄昏，浮动花影，适彼中秋

	//寂静昭华 君子瑟琴  随云起舞  素手点星  

	}

	if (pWndSettings->m_Type == TYPE_TWFM ||
		pWndSettings->m_Type == TYPE_TRFM) {

		CEyeWnd* pEyeWnd = (CEyeWnd*)m_pDispWnd[m_d];
		CScale* pScale = pWndSettings->GetScale();

		real_t Inc = pWndSettings->GetIncrement();

		real_t Cent = 0.0;

		real_t Step;
		if (pScale->m_StepAuto) {
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

		/* CRMSWnd* pRMSWnd = (CRMSWnd*)m_pDispWnd[m_d];
		real t = pRMSWnd->m_Surface.ComputeRms();
		int Max = (int)t;
		if (t > Max + 0.001) Max++;
		if (Max < 1) Max = 1;
		pRMSWnd->m_Max = Max;*/
	}

	else if (pWndSettings->m_Type == TYPE_TAZR) {
		CChartWnd* pAZRWnd = (CChartWnd*)m_pDispWnd[m_d];
		real_t rmin; pAZRWnd->m_Values.GetMin(rmin);
		real_t rmax; pAZRWnd->m_Values.GetMax(rmax);
		real_t t = __max(fabs(rmin), fabs(rmax));
		int imax = (int)t;
		if (t > imax + 0.001) imax++;
		int imin = -imax;
		pAZRWnd->m_max = imax;
		pAZRWnd->m_min = imin;
		pAZRWnd->m_LLabels.Create(imax - imin + 1);
		for (int i = imin; i <= imax; i++) {
			pAZRWnd->m_LLabels[i - imin].Format(_T("%i"), i);
		}
	}

	//531
	if (pWndSettings->m_Type == TYPE_DOFF) return;
	//531

	//521
	if (m_pWndSettings[m_d].m_ZoneRadiusUm >= m_pWFExam->m_WfSurface.m_r_max_um
		&& m_pWndSettings[m_d].m_Type != TYPE_TEYE && m_InsideZone[m_d] == FALSE)
	{
		m_pDispWnd[m_d]->m_IncZoneButton.SetEnabled(FALSE);
		m_pDispWnd[m_d]->m_IncZoneButton.Repaint();
	}

	if (m_pWndSettings[m_d].m_ZoneRadiusUm == 500
		&& m_pWndSettings[m_d].m_Type != TYPE_TEYE && m_InsideZone[m_d] == FALSE)
	{
		m_pDispWnd[m_d]->m_DecZoneButton.SetEnabled(FALSE);
		m_pDispWnd[m_d]->m_DecZoneButton.Repaint();
	}
	//521

}

//***************************************************************************************

void CWFCTAngleSumWnd::CreateMenu()
{
	m_d = m_d;
	CWndSettings* pWndSettings = GetWndSettings();

	//521
	if (m_AverageExam != 0)
	{
		m_pMenu->AppendItem(IDC_SELECT_IMAGE_ITEM, FALSE, FALSE, "Select eye image", "");
		m_pMenu->AppendSeparator();

		if (m_pWFExam->m_Image.m_RGBData)
		{
			m_pMenu->AppendItem(IDC_TYPE_TEYE_ITEM, pWndSettings->m_Type == TYPE_TEYE, FALSE, "Eye Image", "");
		}
	}
	else //521
		m_pMenu->AppendItem(IDC_TYPE_TEYE_ITEM, pWndSettings->m_Type == TYPE_TEYE, FALSE, "Eye Image", "");

	m_pMenu->AppendItem(IDC_TYPE_TWFM_ITEM, pWndSettings->m_Type == TYPE_TWFM, FALSE, "Wavefront Map", "");
	m_pMenu->AppendItem(IDC_TYPE_TRFM_ITEM, pWndSettings->m_Type == TYPE_TRFM, FALSE, "Refraction Map", "");
	m_pMenu->AppendItem(IDC_TYPE_TPSF_ITEM, pWndSettings->m_Type == TYPE_TPSF, FALSE, "Point Spread Function", "");
	m_pMenu->AppendItem(IDC_TYPE_TMTF_ITEM, pWndSettings->m_Type == TYPE_TMTF, FALSE, "Modulation Transfer Function", "");
	m_pMenu->AppendItem(IDC_TYPE_TLTR_ITEM, pWndSettings->m_Type == TYPE_TLTR, FALSE, "Visual Acuity Chart", "");

	//CreateSubmenuNLtr();

	m_pMenu->AppendItem(IDC_TYPE_TEEF_ITEM, pWndSettings->m_Type == TYPE_TEEF, FALSE, "Encircled Energy Function", "");
	m_pMenu->AppendItem(IDC_TYPE_TRMS_ITEM, pWndSettings->m_Type == TYPE_TRMS, FALSE, "Root Mean Square", "");
	m_pMenu->AppendItem(IDC_TYPE_TAZR_ITEM, pWndSettings->m_Type == TYPE_TAZR, FALSE, "Annular / Zonal Refraction", "");

	if (m_isCombo) m_pMenu->AppendItem(IDC_TYPE_DOFF_ITEM, pWndSettings->m_Type == TYPE_DOFF, FALSE, "Depth of Focus Function", "");//531
	m_pMenu->AppendSeparator();

	//531 Dof
	if (pWndSettings->m_Type == TYPE_DOFF && m_k != 4)
	{
		m_pMenu->AppendItem(IDC_SIZE_LARGE_ITEM, FALSE, !pWndSettings->m_SizeSmall, "Large window", "");
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
		return;
	}
	//531;

	if (pWndSettings->m_Type == TYPE_TWFM ||
		pWndSettings->m_Type == TYPE_TRFM ||
		pWndSettings->m_Type == TYPE_TPSF ||
		pWndSettings->m_Type == TYPE_TMTF ||
		pWndSettings->m_Type == TYPE_TLTR ||
		(pWndSettings->m_Type == TYPE_TRMS && pWndSettings->m_RMSIndividual)) {
		m_pMenu->AppendItem(IDC_METHOD_3D_ITEM, FALSE, pWndSettings->m_Method3D, "3D", "");
	}
	m_pMenu->AppendItem(IDC_SIZE_LARGE_ITEM, FALSE, !pWndSettings->m_SizeSmall, "Large window", "");

	if (!(m_k == 2 && pWndSettings->m_Type == TYPE_TEYE && !m_pWFExam->m_Image.m_RGBData))
	{
		if (m_k != 4) m_pMenu->AppendSeparator();
	}

	if (pWndSettings->m_Type == TYPE_TWFM ||
		pWndSettings->m_Type == TYPE_TRFM ||
		pWndSettings->m_Type == TYPE_TPSF ||
		pWndSettings->m_Type == TYPE_TMTF ||
		pWndSettings->m_Type == TYPE_TLTR ||
		pWndSettings->m_Type == TYPE_TEEF ||
		pWndSettings->m_Type == TYPE_TRMS ||
		pWndSettings->m_Type == TYPE_TAZR) {

		//530
		int type = 0;
		if (m_pWFExam->m_WfSurface.m_r_max_um <= 700) type = 1;
		//
		CreateSubmenuZone(type);
		CreateSubmenuMask();
		m_pMenu->AppendSeparator();
	}

	if (pWndSettings->m_Type == TYPE_TEYE)
	{
		if (m_pWFExam->m_Image.m_RGBData)//521
		{
			m_pMenu->AppendItem(IDC_SHOW_MAP_PUPIL_ITEM, FALSE, pWndSettings->m_MapShowPupil, "Show pupil", "");
			m_pMenu->AppendItem(IDC_SHOW_MAP_VERTEX_ITEM, FALSE, pWndSettings->m_MapShowCornealVertex, "Show corneal vertex", "");
			m_pMenu->AppendItem(IDC_SHOW_MAP_LIMBUS_ITEM, FALSE, pWndSettings->m_MapShowLimbus, "Show limbus", "");

			//520
			if (m_k == 2)
			{
				m_pMenu->AppendItem(IDC_SHOW_MAP_RRULER_ITEM, FALSE, m_MapShowRRuler, "Show Radial Ruler", "");
				m_pMenu->AppendItem(IDC_SHOW_MAP_LRULER_ITEM, FALSE, m_MapShowLRuler, "Show Linear Ruler", "");
				m_pMenu->AppendItem(IDC_SHOW_MAP_INLAY_ITEM, FALSE, m_MapShowInlay, "Show Disk", "");
			}
			//520
		}
	}

	else if (pWndSettings->m_Type == TYPE_TWFM || pWndSettings->m_Type == TYPE_TRFM) {
		if (pWndSettings->m_Method3D) {
			m_pMenu->AppendItem(IDC_SHOW_MAP_SURFACE_ITEM, FALSE, pWndSettings->m_MapShowSolidSurface, "Show solid surface", "");
			m_pMenu->AppendItem(IDC_SHOW_MAP_MESH_ITEM, FALSE, pWndSettings->m_MapShowWireMesh, "Show wire mesh", "");
		}
		else {
			m_pMenu->AppendItem(IDC_SHOW_MAP_MAP_ITEM, FALSE, pWndSettings->m_MapShowMap, "Show map", "");
			m_pMenu->AppendItem(IDC_SHOW_MAP_NUMBERS_ITEM, FALSE, pWndSettings->m_MapShowNumbers, "Show numbers", "");

			if (m_pWFExam->m_Image.m_RGBData)//521
			{
				m_pMenu->AppendItem(IDC_SHOW_MAP_EYE_ITEM, FALSE, pWndSettings->m_MapShowEye, "Show eye image", "");
				m_pMenu->AppendItem(IDC_SHOW_MAP_PUPIL_ITEM, FALSE, pWndSettings->m_MapShowPupil, "Show pupil", "");
				m_pMenu->AppendItem(IDC_SHOW_MAP_VERTEX_ITEM, FALSE, pWndSettings->m_MapShowCornealVertex, "Show corneal vertex", "");
				m_pMenu->AppendItem(IDC_SHOW_MAP_LIMBUS_ITEM, FALSE, pWndSettings->m_MapShowLimbus, "Show limbus", "");
			}
			m_pMenu->AppendItem(IDC_MAP_TRANSLUCENT_ITEM, FALSE, pWndSettings->m_MapTranslucent, "Translucent map", "");
		}
		m_pMenu->AppendSeparator();//521
		CreateSubmenuMapSteps();
		CreateSubmenuMapColors();
		m_pMenu->AppendSeparator();
	}

	else if (pWndSettings->m_Type == TYPE_TPSF) {
		m_pMenu->AppendItem(IDC_SHOW_PSF_EEF_ITEM, FALSE, pWndSettings->m_ShowEEF, "Show 50% energy circle", "");
		if (pWndSettings->m_ShowEEF) {
			if (pWndSettings->m_EEFType != EEF_CNT) pWndSettings->m_EEFType = EEF_MAX;
			m_pMenu->AppendItem(IDC_EEF_TYPE1_ITEM, pWndSettings->m_EEFType == EEF_MAX, FALSE, "Circle around maximum", "");
			m_pMenu->AppendItem(IDC_EEF_TYPE2_ITEM, pWndSettings->m_EEFType == EEF_CNT, FALSE, "Circle around centroid", "");
		}
		m_pMenu->AppendSeparator();
	}

	else if (pWndSettings->m_Type == TYPE_TMTF) {
		if (!pWndSettings->m_Method3D) {
			m_pMenu->AppendItem(IDC_MTF_AVG_ITEM, FALSE, pWndSettings->m_MTFAverage, "Radial average curve", "");
			m_pMenu->AppendSeparator();
		}
	}

	else if (pWndSettings->m_Type == TYPE_TLTR)
	{
		CreateSubmenuLtr();
		m_pMenu->AppendSeparator();
	}

	else if (pWndSettings->m_Type == TYPE_TEEF) {
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

	if (pWndSettings->m_Type == TYPE_TWFM ||
		pWndSettings->m_Type == TYPE_TRFM ||
		pWndSettings->m_Type == TYPE_TPSF ||
		pWndSettings->m_Type == TYPE_TMTF ||
		pWndSettings->m_Type == TYPE_TLTR ||
		pWndSettings->m_Type == TYPE_TEEF ||
		pWndSettings->m_Type == TYPE_TRMS ||
		pWndSettings->m_Type == TYPE_TAZR) {
		m_pMenu->AppendItem(IDC_SAVE_TXT_ITEM, FALSE, FALSE, "Save Zernike coefficients into file", "");
	}

	//520, only let the users re-draw the pupil/limbus... under the 'Angle k/a' GUI
	if (m_k == 2 && pWndSettings->m_Type == TYPE_TEYE && m_pWFExam->m_Image.m_RGBData)
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_EDIT_LIMBUS_ITEM, FALSE, FALSE, "Edit pupil,limbus,vertex,ruler and disk", "");
	}
	//520

	//521
	if (m_k == 0)//Visual Acuity
	{
		if ((m_d == 0 || m_d == 1) && !SameViAc0())
		{
			m_pMenu->AppendSeparator();
			m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
		}
		if ((m_d == 2 || m_d == 3) && !SameViAc1())
		{
			m_pMenu->AppendSeparator();
			m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
		}
	}
	else if (m_k == 1 && !SameWFRMS())//WF/RMS
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
	}
	else if (m_k == 2 && !SameAngleKA())
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
	}
	else if (m_k == 4 && !SameDepthFocus())
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
	}
	//521
}

//***************************************************************************************
//520
void CWFCTAngleSumWnd::OnEditLimbusItemClicked()
{
	CEyeImage* pImage = &m_pWFExam->m_Image;

	if (pImage->m_RGBData.GetMem() == NULL) return;

	CImageDlg3* pDlg = new CImageDlg3(this, pImage, &m_pCTExam->m_Image, TRUE, TRUE);//1

	if (pDlg->DoModal() == IDOK)
	{

		CBusyCursor Cursor;

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


		//To re-draw the txt in the left-bottom corner of image
		if (m_pWFExam->m_Header.m_AngleKappaOK)
		{
			m_pDispWnd[m_d]->m_RBLabel[0] = "Angle Kappa";
			m_pDispWnd[m_d]->m_RBLabel[1].Format(_T("(X, Y) = (%.3f, %.3f)"),//1
				m_pWFExam->m_Header.m_AngleKappaXUm * 0.001,
				m_pWFExam->m_Header.m_AngleKappaYUm * 0.001);

			m_pDispWnd[m_d]->m_RBLabel[2].Format(_T("(R, A) = (%.3f, %i)"),
				m_pWFExam->m_Header.m_AngleKappaRUm * 0.001,
				m_pWFExam->m_Header.m_AngleKappaADg);
		}
		else
		{
			m_pDispWnd[m_d]->m_RBLabel[0] = "";
			m_pDispWnd[m_d]->m_RBLabel[1] = "";
			m_pDispWnd[m_d]->m_RBLabel[2] = "";
		}

		m_pDispWnd[m_d]->m_RBLabelColor[3] = BLUE;
		m_pDispWnd[m_d]->m_RBLabelColor[4] = BLUE;
		m_pDispWnd[m_d]->m_RBLabelColor[5] = BLUE;

		if (m_pWFExam->m_Header.m_AngleAlphaOK)
		{
			m_pDispWnd[m_d]->m_RBLabel[3] = "Angle Alpha";
			m_pDispWnd[m_d]->m_RBLabel[4].Format(_T("(X, Y) = (%.3f, %.3f)"),//2
				m_pWFExam->m_Header.m_AngleAlphaXUm * 0.001,
				m_pWFExam->m_Header.m_AngleAlphaYUm * 0.001);

			m_pDispWnd[m_d]->m_RBLabel[5].Format(_T("(R, A) = (%.3f, %i)"),
				m_pWFExam->m_Header.m_AngleAlphaRUm * 0.001,
				m_pWFExam->m_Header.m_AngleAlphaADg);
		}
		else
		{
			m_pDispWnd[m_d]->m_RBLabel[3] = "";
			m_pDispWnd[m_d]->m_RBLabel[4] = "";
			m_pDispWnd[m_d]->m_RBLabel[5] = "";
		}

		m_pDispWnd[m_d]->m_LBLabelColor[0] = RGB(255, 128, 0);//Linear Ruler
		m_pDispWnd[m_d]->m_LBLabelColor[1] = RGB(255, 128, 0);//Linear Ruler

		m_pDispWnd[m_d]->m_LBLabelColor[2] = YELLOW;
		m_pDispWnd[m_d]->m_LBLabelColor[3] = YELLOW;
		m_pDispWnd[m_d]->m_LBLabelColor[4] = YELLOW;
		m_pDispWnd[m_d]->m_LBLabelColor[5] = YELLOW;
		m_pDispWnd[m_d]->m_LBLabelColor[6] = WHITE;//Inlay
		m_pDispWnd[m_d]->m_LBLabelColor[7] = WHITE;//Inlay
		m_pDispWnd[m_d]->m_LBLabelColor[8] = WHITE;//Inlay
		m_pDispWnd[m_d]->m_LBLabelColor[9] = WHITE;//Inlay
		m_pDispWnd[m_d]->m_LBLabelColor[10] = WHITE;//Inlay

		if (m_pWFExam->m_Header.m_LinearRulerOK)
		{
			m_pDispWnd[m_d]->m_LBLabel[0] = "Linear Ruler";
			real_t dis = hyp(m_pWFExam->m_Image.m_LRa_x0_um - m_pWFExam->m_Image.m_LRa_x1_um, m_pWFExam->m_Image.m_LRa_y0_um - m_pWFExam->m_Image.m_LRa_y1_um) * 0.001;
			m_pDispWnd[m_d]->m_LBLabel[1].Format(_T("(D) = (%.3f)"), dis);
		}
		else
		{
			m_pDispWnd[m_d]->m_LBLabel[0] = "";
			m_pDispWnd[m_d]->m_LBLabel[1] = "";
		}

		if (m_pWFExam->m_Header.m_AngleRulerOK)
		{
			m_pDispWnd[m_d]->m_LBLabel[2] = "Radial Ruler";
			m_pDispWnd[m_d]->m_LBLabel[3].Format(_T("(D) = (%.3f)"), m_pWFExam->m_Image.m_Ra_r_um * 0.002);
			m_pDispWnd[m_d]->m_LBLabel[4].Format(_T("(X, Y) = (%.3f, %.3f)"),
				m_pWFExam->m_Header.m_AngleRulerXUm * 0.001,
				m_pWFExam->m_Header.m_AngleRulerYUm * 0.001);

			m_pDispWnd[m_d]->m_LBLabel[5].Format(_T("(R, A) = (%.3f, %i)"),
				m_pWFExam->m_Header.m_AngleRulerRUm * 0.001,
				m_pWFExam->m_Header.m_AngleRulerADg);
		}
		else
		{
			m_pDispWnd[m_d]->m_LBLabel[2] = "";
			m_pDispWnd[m_d]->m_LBLabel[3] = "";
			m_pDispWnd[m_d]->m_LBLabel[4] = "";
			m_pDispWnd[m_d]->m_LBLabel[5] = "";
		}

		//Inlay
		if (m_pWFExam->m_Header.m_InlayOK)
		{
			m_pDispWnd[m_d]->m_LBLabel[6] = "Disk";
			m_pDispWnd[m_d]->m_LBLabel[7].Format(_T("(D) = (%.3f)"), m_pWFExam->m_Image.m_In_r_um * 0.002);
			m_pDispWnd[m_d]->m_LBLabel[8].Format(_T("(Inner_D) = (%.3f)"), m_pWFExam->m_Image.m_In_In_r_um * 0.002);

			m_pDispWnd[m_d]->m_LBLabel[9].Format(_T("(X, Y) = (%.3f, %.3f)"),
				m_pWFExam->m_Header.m_InlayXUm * 0.001,
				m_pWFExam->m_Header.m_InlayYUm * 0.001);

			m_pDispWnd[m_d]->m_LBLabel[10].Format(_T("(R, A) = (%.3f, %i)"),
				m_pWFExam->m_Header.m_InlayAngleRUm * 0.001,
				m_pWFExam->m_Header.m_InlayAngleADg);
		}
		else
		{
			m_pDispWnd[m_d]->m_LBLabel[6] = "";
			m_pDispWnd[m_d]->m_LBLabel[7] = "";
			m_pDispWnd[m_d]->m_LBLabel[8] = "";
			m_pDispWnd[m_d]->m_LBLabel[9] = "";
			m_pDispWnd[m_d]->m_LBLabel[10] = "";
		}
		//Done

		Repaint();

		if (m_pWFExam->m_Header.m_Saved)
			::DB.SaveWFExam(m_pWFExam);
	}

	delete pDlg;
}


void CWFCTAngleSumWnd::OnShowRRulerItemClicked()
{
	m_MapShowRRuler = !m_MapShowRRuler;
	CreateChildWnd();
	Repaint();
}

void CWFCTAngleSumWnd::OnShowLRulerItemClicked()
{
	m_MapShowLRuler = !m_MapShowLRuler;
	CreateChildWnd();
	Repaint();
}

void CWFCTAngleSumWnd::OnShowInalyItemClicked()
{
	m_MapShowInlay = !m_MapShowInlay;
	CreateChildWnd();
	Repaint();
}
//520
//***************************************************************************************
//521
void CWFCTAngleSumWnd::OnShowDefaultItemClicked()
{
	CBusyCursor Cursor;

	CWndSettings* pWndSettings = GetWndSettings();

	pWndSettings->m_LtrDistanceType = 0;//530

	m_InsideZone[m_d] = FALSE; //530

							   //530
	if (m_k == 0)
		::NewSettings.m_ShowPreZone_WF_VA_Wnd[m_d] = FALSE;
	else if (m_k == 2)
		::NewSettings.m_ShowPreZone_WF_AKA_Wnd[m_d] = FALSE;
	//530

	if (m_k == 0)// Visual Acuity
	{
		if (m_d == 0 || m_d == 1)
		{
			if (m_d == 0)
			{
				pWndSettings->m_Mask.SetType(MASK_TOTAL);
				pWndSettings->m_Piston = TRUE;
				pWndSettings->m_Tilt = TRUE;
			}
			if (m_d == 1)
			{
				pWndSettings->m_Mask.SetType(MASK_HO_TOTAL);
				pWndSettings->m_Piston = FALSE;
				pWndSettings->m_Tilt = FALSE;
			}

			pWndSettings->m_Type = 11;

			pWndSettings->m_MapShowMap = 1;
			pWndSettings->m_MapShowEye = 0;
			pWndSettings->m_MapShowNumbers = 0;
			pWndSettings->m_MapShowPupil = 0;
			pWndSettings->m_MapShowLimbus = 0;
			pWndSettings->m_MapShowCornealVertex = 0;
			pWndSettings->m_MapTranslucent = 0;

			pWndSettings->m_Method3D = 0;
			pWndSettings->m_MapShowSolidSurface = TRUE;
			pWndSettings->m_MapShowWireMesh = TRUE;
			pWndSettings->m_SizeSmall = TRUE;

			CScale* pScale = pWndSettings->GetScale();
			pScale->m_Step = 0.5;
			pWndSettings->SetDefaultColors(1);
		}

		if (m_d == 2 || m_d == 3)
		{
			if (m_d == 2)
			{
				pWndSettings->m_Mask.SetType(MASK_TOTAL);
				pWndSettings->m_Piston = TRUE;
				pWndSettings->m_Tilt = TRUE;
			}
			if (m_d == 3)
			{
				pWndSettings->m_Mask.SetType(MASK_HO_TOTAL);
				pWndSettings->m_Piston = FALSE;
				pWndSettings->m_Tilt = FALSE;
			}

			pWndSettings->m_Type = 14;
			pWndSettings->m_Ltr = 0;
			pWndSettings->m_LtrLine = 70;
			pWndSettings->m_LtrOrientation = 0;
			pWndSettings->m_Method3D = 0;
			pWndSettings->m_MapShowSolidSurface = TRUE;
			pWndSettings->m_MapShowWireMesh = TRUE;
			pWndSettings->m_SizeSmall = TRUE;
		}
	}
	else if (m_k == 1) //WF/RMS
	{
		if (m_d == 0)
		{
			pWndSettings->m_Mask.SetType(MASK_TOTAL);
			pWndSettings->m_Piston = TRUE;
			pWndSettings->m_Tilt = TRUE;
		}
		if (m_d == 1)
		{
			pWndSettings->m_Mask.SetType(MASK_HO_TOTAL);
			pWndSettings->m_Piston = FALSE;
			pWndSettings->m_Tilt = FALSE;
		}
		if (m_d == 2)
		{
			pWndSettings->m_Mask.SetType(MASK_LO_TOTAL);
			pWndSettings->m_Piston = FALSE;
			pWndSettings->m_Tilt = FALSE;


		}
		if (m_d == 3)
		{
			pWndSettings->m_Mask.SetType(MASK_TOTAL);
			pWndSettings->m_Piston = TRUE;
			pWndSettings->m_Tilt = TRUE;
		}

		if (m_d != 3)
		{
			pWndSettings->m_Type = 10;

			pWndSettings->m_Method3D = 0;
			pWndSettings->m_SizeSmall = 1;

			pWndSettings->m_MapShowMap = 1;
			pWndSettings->m_MapShowEye = 0;
			pWndSettings->m_MapShowNumbers = 0;
			pWndSettings->m_MapShowPupil = 0;
			pWndSettings->m_MapShowLimbus = 0;
			pWndSettings->m_MapShowCornealVertex = 0;
			pWndSettings->m_MapTranslucent = 0;

			CScale* pScale = pWndSettings->GetScale();
			pScale->m_Step = 0.5;
			pWndSettings->SetDefaultColors(1);
		}
		else
		{
			pWndSettings->m_Type = 16;
			pWndSettings->m_SizeSmall = 1;
			pWndSettings->m_RMSIndividual = FALSE;
		}
	}
	else if (m_k == 2)//Angle K/A
	{
		for (int i = 0; i < 4; i++) SAFE_DELETE(m_pDispWnd[i]);
		m_pWndSettings[m_d].m_Type = 1;
		m_pWndSettings[m_d].m_MapShowPupil = 1;
		m_pWndSettings[m_d].m_MapShowLimbus = 1;
		m_pWndSettings[m_d].m_MapShowCornealVertex = 1;
		m_pWndSettings[m_d].m_SizeSmall = FALSE;
	}
	//531
	else if (m_k == 4)//Depth of focus
	{
		for (int i = 0; i < 4; i++) SAFE_DELETE(m_pDispWnd[i]);
		m_pWndSettings[m_d].m_Type = 61;
		m_pWndSettings[m_d].m_SizeSmall = FALSE;
	}
	//531 Done

	if (m_k == 0 || m_k == 1)
	{
		for (m_d = 0; m_d < 4; m_d++)
		{
			CreateChildWnd();
		}
	}
	else if (m_k == 2 || m_k == 4) CreateChildWnd();

	Repaint();
}
//521
//***************************************************************************************
//521
BOOL CWFCTAngleSumWnd::SameViAc0()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_d == 0)
	{
		if (pWndSettings->m_Mask.GetType() != MASK_TOTAL) return FALSE;
		if (pWndSettings->m_Piston != TRUE)       return FALSE;
		if (pWndSettings->m_Tilt != TRUE)       return FALSE;
	}
	if (m_d == 1)
	{
		if (pWndSettings->m_Mask.GetType() != MASK_HO_TOTAL) return FALSE;
		if (pWndSettings->m_Piston == TRUE)       return FALSE;
		if (pWndSettings->m_Tilt == TRUE)       return FALSE;
	}

	if (pWndSettings->m_Type != 11) return FALSE;

	if (pWndSettings->m_MapShowMap != 1) return FALSE;
	if (m_pWFExam->m_Image.m_RGBData && pWndSettings->m_MapShowEye != 0) return FALSE;
	if (pWndSettings->m_MapShowNumbers != 0) return FALSE;
	if (m_pWFExam->m_Image.m_RGBData && pWndSettings->m_MapShowPupil != 0) return FALSE;
	if (m_pWFExam->m_Image.m_RGBData && pWndSettings->m_MapShowLimbus != 0) return FALSE;
	if (m_pWFExam->m_Image.m_RGBData && pWndSettings->m_MapShowCornealVertex != 0) return FALSE;
	if (pWndSettings->m_MapTranslucent != 0) return FALSE;

	if (pWndSettings->m_Method3D != 0) return FALSE;
	if (pWndSettings->m_SizeSmall != 1) return FALSE;

	CScale* pScale = pWndSettings->GetScale();
	if (pScale->m_Step != 0.5) return FALSE;
	if (!pWndSettings->AreDefaultColors(1))            return FALSE;

	return TRUE;
}
//521
//***************************************************************************************
//521
BOOL CWFCTAngleSumWnd::SameViAc1()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_d == 2)
	{
		if (pWndSettings->m_Mask.GetType() != MASK_TOTAL) return FALSE;
		if (pWndSettings->m_Piston != TRUE)       return FALSE;
		if (pWndSettings->m_Tilt != TRUE)       return FALSE;
	}
	if (m_d == 3)
	{
		if (pWndSettings->m_Mask.GetType() != MASK_HO_TOTAL) return FALSE;
		if (pWndSettings->m_Piston == TRUE)       return FALSE;
		if (pWndSettings->m_Tilt == TRUE)       return FALSE;
	}

	if (pWndSettings->m_Type != 14)   return FALSE;
	if (pWndSettings->m_Ltr != 0)    return FALSE;
	if (pWndSettings->m_LtrLine != 70)   return FALSE;
	if (pWndSettings->m_LtrOrientation != 0)    return FALSE;
	if (pWndSettings->m_Method3D != 0)    return FALSE;
	if (pWndSettings->m_SizeSmall != TRUE) return FALSE;

	return TRUE;
}
//521
//***************************************************************************************
//521
BOOL CWFCTAngleSumWnd::SameWFRMS()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_d == 0)
	{
		if (pWndSettings->m_Mask.GetType() != MASK_TOTAL) return FALSE;
		if (pWndSettings->m_Piston != TRUE)       return FALSE;
		if (pWndSettings->m_Tilt != TRUE)       return FALSE;

		if (pWndSettings->m_Type != 10) return FALSE;
	}
	if (m_d == 1)
	{
		if (pWndSettings->m_Mask.GetType() != MASK_HO_TOTAL) return FALSE;
		if (pWndSettings->m_Piston == TRUE)       return FALSE;
		if (pWndSettings->m_Tilt == TRUE)       return FALSE;

		if (pWndSettings->m_Type != 10) return FALSE;
	}
	if (m_d == 2)
	{
		if (pWndSettings->m_Mask.GetType() != MASK_LO_TOTAL) return FALSE;
		if (pWndSettings->m_Piston == TRUE)       return FALSE;
		if (pWndSettings->m_Tilt == TRUE)       return FALSE;


	}
	if (m_d == 3)
	{
		if (pWndSettings->m_Mask.GetType() != MASK_TOTAL) return FALSE;
		if (pWndSettings->m_Piston != TRUE)       return FALSE;
		if (pWndSettings->m_Tilt != TRUE)       return FALSE;
	}

	if (m_d != 3)
	{
		if (pWndSettings->m_Type != 10) return FALSE;

		if (pWndSettings->m_Method3D != 0) return FALSE;
		if (pWndSettings->m_SizeSmall != 1) return FALSE;

		if (pWndSettings->m_MapShowMap != 1) return FALSE;
		if (m_pWFExam->m_Image.m_RGBData && pWndSettings->m_MapShowEye != 0) return FALSE;
		if (pWndSettings->m_MapShowNumbers != 0) return FALSE;
		if (m_pWFExam->m_Image.m_RGBData && pWndSettings->m_MapShowPupil != 0) return FALSE;
		if (m_pWFExam->m_Image.m_RGBData && pWndSettings->m_MapShowLimbus != 0) return FALSE;
		if (m_pWFExam->m_Image.m_RGBData && pWndSettings->m_MapShowCornealVertex != 0) return FALSE;
		if (pWndSettings->m_MapTranslucent != 0) return FALSE;

		CScale* pScale = pWndSettings->GetScale();
		if (pScale->m_Step != 0.5) return FALSE;
		if (!pWndSettings->AreDefaultColors(1))            return FALSE;
	}
	else
	{
		if (pWndSettings->m_Type != 16) return FALSE;
		if (pWndSettings->m_RMSIndividual != FALSE) return FALSE;
		if (pWndSettings->m_SizeSmall != 1)  return FALSE;
	}

	return TRUE;
}
//521
//***************************************************************************************
//521
BOOL CWFCTAngleSumWnd::SameAngleKA()
{
	if (m_pWndSettings[m_d].m_Type != 1) return FALSE;
	if (m_pWFExam->m_Image.m_RGBData && m_pWndSettings[m_d].m_MapShowPupil != 1) return FALSE;
	if (m_pWFExam->m_Image.m_RGBData && m_pWndSettings[m_d].m_MapShowLimbus != 1) return FALSE;
	if (m_pWFExam->m_Image.m_RGBData && m_pWndSettings[m_d].m_MapShowCornealVertex != 1) return FALSE;
	if (m_pWndSettings[m_d].m_SizeSmall == 1) return FALSE;

	return TRUE;
}
//521
//***************************************************************************************
//521
BOOL CWFCTAngleSumWnd::SameDepthFocus()
{
	if (m_pWndSettings[m_d].m_Type != TYPE_DOFF) return FALSE;
	if (m_pWndSettings[m_d].m_SizeSmall == TRUE)      return FALSE;

	return TRUE;
}
//521
//***************************************************************************************
//521
void CWFCTAngleSumWnd::OnSelEyeImgItemClicked()
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
//530
LRESULT CWFCTAngleSumWnd::OnChangeRmsGUI(WPARAM wParam, LPARAM lParam)
{
	int W = ::GetSystemMetrics(SM_CXSCREEN);

	for (int d = 0; d < 4; d++)
	{
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] == wParam)
		{
			m_pWndSettings[d].m_SimplifiedRMS = !m_pWndSettings[d].m_SimplifiedRMS;
			((CRMSWnd*)m_pDispWnd[d])->m_SimpliedRMS = m_pWndSettings[d].m_SimplifiedRMS;

			CString s = (m_pWndSettings[d].m_SimplifiedRMS ? "Show Details" : "Hide Details");
			m_pDispWnd[d]->m_ShowMoreButton.SetTip(s);

			if (m_pWndSettings[d].m_SizeSmall)
			{
				m_pDispWnd[d]->m_ShowMoreButton.SetImage(m_pWndSettings[d].m_SimplifiedRMS ?
					(W >= 1920 ? IDR_SHOW_DETAIL_3 : W >= 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
					(W >= 1920 ? IDR_HIDE_DETAIL_3 : W >= 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
			}
			else
			{
				m_pDispWnd[d]->m_ShowMoreButton.SetImage(m_pWndSettings[d].m_SimplifiedRMS ?
					(W >= 1920 ? IDR_SHOW_DETAIL_6 : W >= 1600 ? IDR_SHOW_DETAIL_5 : IDR_SHOW_DETAIL_4) :
					(W >= 1920 ? IDR_HIDE_DETAIL_6 : W >= 1600 ? IDR_HIDE_DETAIL_5 : IDR_HIDE_DETAIL_4));
			}

			m_pDispWnd[d]->m_ShowMoreButton.Repaint();

			m_pDispWnd[d]->Repaint();
		}
	}

	return 0;
}
//530
//***************************************************************************************
