//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "CTSingleSumWnd.h"
#include "EyeWnd.h"
#include "RMSWnd.h"
#include "LtrWnd.h"//
#include "BusyCursor.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CCTSingleSumWnd, CSumWnd)

	ON_COMMAND(IDC_DEFAULT_GUI_ITEM, OnShowDefaultItemClicked)//521

	ON_MESSAGE(WM_OKULIX_LCLICK, OnOkulix)//521

	ON_COMMAND(IDC_DEFAULT_GUI_ITEM, OnShowDefaultItemClicked)//521

	ON_MESSAGE(WM_SHOW_SIMPRMS, OnChangeRmsGUI)//530 Change the rms gui between simplify and details

END_MESSAGE_MAP()
//***************************************************************************************

CCTSingleSumWnd::CCTSingleSumWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CCTExam* pCTExam, CWndSettings* pWndSettings, int i) :
	CSumWnd(pWnd, WndRect, pPatient, pWndSettings)
{
	CBusyCursor Cursor;

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
																												 //----------------------------------------------------

	m_pCTExam = pCTExam;

	//*007*[cjf***05052012],record the increase and decrease range
	::TempSettings.Com_r_max_um = m_pCTExam->m_WfSurface.m_r_max_um;
	//*007*[cjf***05052012]

	m_i = i;//0: CT Summary; 1:Keratometry; 2:3-D Z elevation

	m_d = -1;
	for (int d = 0; d < 4; d++) {
		if (m_pWndSettings[d].m_SizeSmall == FALSE) {
			m_d = d;
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

	ShowWindow(SW_SHOW);
}

//***************************************************************************************

void CCTSingleSumWnd::OnSizeLargeItemClicked()
{
	CWndSettings* pWndSettings = GetWndSettings();

	pWndSettings->m_SizeSmall = !pWndSettings->m_SizeSmall;

	for (int i = 0; i < 4; i++) SAFE_DELETE(m_pDispWnd[i]);

	CBusyCursor Cursor;

	if (pWndSettings->m_SizeSmall) {
		for (m_d = 0; m_d < 4; m_d++) CreateChildWnd();
	}
	else {
		CreateChildWnd();
	}

	Repaint();
}

//***************************************************************************************

void CCTSingleSumWnd::RepaintMemDC()
{
	CSumWnd::RepaintMemDC();

	BOOL Rows[44] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

	CTExamLargeInfo(m_pCTExam, m_Rect[4], 44, Rows);//530  Original is WFExamInfo
}

//***************************************************************************************

void CCTSingleSumWnd::CreateChildWnd()//[520], add 'int Type'
{
	SAFE_DELETE(m_pDispWnd[m_d]);

	CWndSettings* pWndSettings = GetWndSettings();

	if (pWndSettings->m_Type != TYPE_CEYE &&
		pWndSettings->m_Type != TYPE_CAXM &&
		pWndSettings->m_Type != TYPE_CTNM &&
		pWndSettings->m_Type != TYPE_CRFM &&
		pWndSettings->m_Type != TYPE_CELM &&
		pWndSettings->m_Type != TYPE_CWFM &&
		pWndSettings->m_Type != TYPE_CRMS &&
		pWndSettings->m_Type != TYPE_CSKM &&
		pWndSettings->m_Type != TYPE_CLTR &&//6.2.0
		pWndSettings->m_Type != TYPE_TLTR) // Add TYPE_TLTR
	{
		pWndSettings->m_Type = TYPE_CAXM;
	}

	if (pWndSettings->m_Type == TYPE_CRMS) {
		if (pWndSettings->m_Method3D) {
			pWndSettings->m_RMSIndividual = TRUE;
		}
	}

	if (pWndSettings->m_Type == TYPE_CSKM) {
		pWndSettings->m_Method3D = FALSE;
	}

	// if (!::Licensing.IsWorkstation() && !::Licensing.IsViewer() && !::Licensing.IsCombo() && !::Licensing.IsPresbia() && !::Licensing.IsDemo()) 
	// {
	//// method
	//if (m_i == 2) { // 3D Z-Elev
	//  pWndSettings->m_Method3D = TRUE;
	//}
	//else {
	//  pWndSettings->m_Method3D = FALSE;
	//}
	//// mask
	//if (pWndSettings->m_Type == TYPE_CWFM ||
	//	pWndSettings->m_Type == TYPE_CRMS) {
	//  int MaskType = pWndSettings->m_Mask.GetType();
	//  if (MaskType != MASK_TOTAL &&
	//	  MaskType != MASK_TOTAL_NO_DEFOCUS &&
	//	  MaskType != MASK_LO_TOTAL &&
	//	  MaskType != MASK_HO_TOTAL) {
	//	pWndSettings->m_Mask.SetType(MASK_TOTAL);
	//	pWndSettings->m_Piston = TRUE;
	//	pWndSettings->m_Tilt = TRUE;
	//  }
	//}
	//// colors
	//if (pWndSettings->m_Type == TYPE_CAXM ||
	//	pWndSettings->m_Type == TYPE_CTNM ||
	//	pWndSettings->m_Type == TYPE_CRFM ||
	//	pWndSettings->m_Type == TYPE_CELM ||
	//	pWndSettings->m_Type == TYPE_CWFM) {
	//  if (!pWndSettings->AreDefaultColors()) {
	//	pWndSettings->SetDefaultColors(1);
	//  }
	//}
	// }

	m_Surfaces[m_d] = m_pCTExam->m_WfSurface;
	real_t r_max_um = 5000.0;
	r_max_um = __min(r_max_um, m_Surfaces[m_d].m_r_max_um);
	r_max_um = __min(r_max_um, pWndSettings->m_ZoneRadiusUm);
	m_Surfaces[m_d].ChangeRMaxUm(r_max_um);
	m_Surfaces[m_d] &= pWndSettings->m_Mask;
	m_Surfaces[m_d].ComputePiston(pWndSettings->m_Piston);
	m_Surfaces[m_d].ComputeTilt(pWndSettings->m_Tilt);

	RECT Rect = pWndSettings->m_SizeSmall ? m_Rect[m_d] : m_LargeRect;

	switch (pWndSettings->m_Type) {

	case TYPE_CEYE:
	{
		CEyeWnd* pEyeWnd = new CEyeWnd(Rect, this);
		pEyeWnd->m_Type = pWndSettings->m_Type;
		pEyeWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
		pEyeWnd->m_Method3D = FALSE;
		pEyeWnd->m_MapShowEye = TRUE;
		pEyeWnd->m_MapShowRings = pWndSettings->m_MapShowRings;
		pEyeWnd->m_MapShowLimbus = pWndSettings->m_MapShowLimbus;
		pEyeWnd->m_MapShowPupil = pWndSettings->m_MapShowPupil;
		pEyeWnd->m_pCTExam = m_pCTExam;
		m_pDispWnd[m_d] = pEyeWnd;
	}
	break;

	case TYPE_CAXM:
	case TYPE_CTNM:
	case TYPE_CRFM:
	{
		int MapUnit = (pWndSettings->m_Type == TYPE_CRFM) || (pWndSettings->m_MapUnit == DIOPTERS) ? DIOPTERS : MILLIMETERS;
		CEyeWnd* pEyeWnd = new CEyeWnd(Rect, this);
		pEyeWnd->m_Type = pWndSettings->m_Type;
		pEyeWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
		pEyeWnd->m_Method3D = pWndSettings->m_Method3D;
		if (pWndSettings->m_Type == TYPE_CAXM) pEyeWnd->m_LTLabel[0] = "Axial Map";
		else if (pWndSettings->m_Type == TYPE_CTNM) pEyeWnd->m_LTLabel[0] = "Local ROC Map";
		else pEyeWnd->m_LTLabel[0] = "Refractive Map";
		pEyeWnd->m_MapShowEye = pWndSettings->m_MapShowEye;
		pEyeWnd->m_MapShowMap = pWndSettings->m_MapShowMap;
		pEyeWnd->m_MapShowNumbers = pWndSettings->m_MapShowNumbers;
		pEyeWnd->m_MapShowPupil = pWndSettings->m_MapShowPupil;
		pEyeWnd->m_MapShowLimbus = pWndSettings->m_MapShowLimbus;
		pEyeWnd->m_MapShowSolidSurface = pWndSettings->m_MapShowSolidSurface;
		pEyeWnd->m_MapShowWireMesh = pWndSettings->m_MapShowWireMesh;
		pEyeWnd->m_MapTranslucent = pWndSettings->m_MapTranslucent;
		pEyeWnd->m_MapShowKeratometry = pWndSettings->m_MapShowKeratometry;
		pEyeWnd->m_MapShowSimKAxes = pWndSettings->m_MapShowSimKAxes;
		pEyeWnd->m_Unit = MapUnit == DIOPTERS ? "D" : "mm";
		pEyeWnd->m_Inc = pWndSettings->GetIncrement();
		CScale* pScale = pWndSettings->GetScale();
		pEyeWnd->m_NumColors = pScale->m_NumColors;
		for (int i = 0; i < pScale->m_NumColors; i++) {
			pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
		}
		pEyeWnd->m_pCTExam = m_pCTExam;
		if (pWndSettings->m_Method3D) {
			m_pCTExam->CreateDpm3D(pEyeWnd->m_Map3D, pWndSettings->m_Type, MapUnit);
		}
		else {
			m_pCTExam->CreateDpm2D(pEyeWnd->m_Map2D, pWndSettings->m_Type, MapUnit, 1.0 / pEyeWnd->m_y_px_um);
		}
		pEyeWnd->CreateCentButtons();
		pEyeWnd->CreateZoomButtons();

		//521 for Okulix
		if (pWndSettings->m_Type == TYPE_CTNM)
		{
			pEyeWnd->CreateOKulixButtons();
		}
		//521 for Okulix

		m_pDispWnd[m_d] = pEyeWnd;
	}
	break;

	case TYPE_CELM:
	{
		real_t ax0_um, q;
		if (pWndSettings->m_RefSurfaceType == REF_SURFACE_BEST_SPHERE) {
			ax0_um = m_pCTExam->m_HtSpSurface.m_ax0_um;
			q = 0.0;
		}
		else if (pWndSettings->m_RefSurfaceType == REF_SURFACE_BEST_CONIC) {
			ax0_um = m_pCTExam->m_HtCnSurface.m_ax0_um;
			q = m_pCTExam->m_HtCnSurface.m_q;
		}
		else {
			ax0_um = pWndSettings->m_RefSurfaceAx0Um;
			q = pWndSettings->m_RefSurfaceQ;
		}
		CConicSurface RefSurface;
		RefSurface.Create(CT_R_MAX_UM, ax0_um, q);

		CEyeWnd* pEyeWnd = new CEyeWnd(Rect, this);
		pEyeWnd->m_Type = pWndSettings->m_Type;
		pEyeWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
		pEyeWnd->m_Method3D = pWndSettings->m_Method3D;
		pEyeWnd->m_LTLabel[0] = "Z Elevation Map";
		pEyeWnd->m_RBLabel[4] = "Ref Surface";
		pEyeWnd->m_RBLabel[5].Format(_T("R0 = %.2f mm, Q = %.2f"), 0.001 * RefSurface.m_ax0_um, RefSurface.m_q);
		pEyeWnd->m_RBLabelColor[0] = m_Printing ? BLACK : WHITE;
		pEyeWnd->m_RBLabelColor[1] = pEyeWnd->m_RBLabelColor[0];
		pEyeWnd->m_RBLabelColor[2] = pEyeWnd->m_RBLabelColor[0];
		pEyeWnd->m_RBLabelColor[3] = pEyeWnd->m_RBLabelColor[0];
		pEyeWnd->m_RBLabelColor[4] = pEyeWnd->m_RBLabelColor[0];
		pEyeWnd->m_RBLabelColor[5] = pEyeWnd->m_RBLabelColor[0];
		pEyeWnd->m_RBLabelBkColor = NOCOLOR;
		pEyeWnd->m_MapShowEye = pWndSettings->m_MapShowEye;
		pEyeWnd->m_MapShowMap = pWndSettings->m_MapShowMap;
		pEyeWnd->m_MapShowNumbers = pWndSettings->m_MapShowNumbers;
		pEyeWnd->m_MapShowPupil = pWndSettings->m_MapShowPupil;
		pEyeWnd->m_MapShowLimbus = pWndSettings->m_MapShowLimbus;
		pEyeWnd->m_MapShowSolidSurface = pWndSettings->m_MapShowSolidSurface;
		pEyeWnd->m_MapShowWireMesh = pWndSettings->m_MapShowWireMesh;
		pEyeWnd->m_MapTranslucent = pWndSettings->m_MapTranslucent;
		pEyeWnd->m_Unit = "�";
		pEyeWnd->m_Inc = pWndSettings->GetIncrement();
		CScale* pScale = pWndSettings->GetScale();
		pEyeWnd->m_NumColors = pScale->m_NumColors;
		for (int i = 0; i < pScale->m_NumColors; i++) {
			pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
		}
		pEyeWnd->m_pCTExam = m_pCTExam;
		if (pWndSettings->m_Method3D) {
			m_pCTExam->CreateElm3D(pEyeWnd->m_Map3D, RefSurface);
		}
		else {
			m_pCTExam->CreateElm2D(pEyeWnd->m_Map2D, RefSurface, 1.0 / pEyeWnd->m_y_px_um);
		}
		pEyeWnd->CreateCentButtons();
		pEyeWnd->CreateZoomButtons();
		m_pDispWnd[m_d] = pEyeWnd;
	}
	break;

	case TYPE_CWFM:
	{
		CEyeWnd* pEyeWnd = new CEyeWnd(Rect, this);
		pEyeWnd->m_Type = pWndSettings->m_Type;
		pEyeWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
		pEyeWnd->m_Method3D = pWndSettings->m_Method3D;
		pEyeWnd->m_LTLabel[0] = "Wavefront Map";
		pWndSettings->m_Mask.GetName(pEyeWnd->m_LTLabel[1]);
		pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
		pEyeWnd->m_MapShowEye = pWndSettings->m_MapShowEye;
		pEyeWnd->m_MapShowMap = pWndSettings->m_MapShowMap;
		pEyeWnd->m_MapShowNumbers = pWndSettings->m_MapShowNumbers;
		pEyeWnd->m_MapShowLimbus = pWndSettings->m_MapShowLimbus;
		pEyeWnd->m_MapShowPupil = pWndSettings->m_MapShowPupil;
		pEyeWnd->m_MapShowSolidSurface = pWndSettings->m_MapShowSolidSurface;
		pEyeWnd->m_MapShowWireMesh = pWndSettings->m_MapShowWireMesh;
		pEyeWnd->m_MapTranslucent = pWndSettings->m_MapTranslucent;
		pEyeWnd->m_Unit = "�";
		pEyeWnd->m_Inc = pWndSettings->GetIncrement();
		CScale* pScale = pWndSettings->GetScale();
		pEyeWnd->m_NumColors = pScale->m_NumColors;
		for (int i = 0; i < pScale->m_NumColors; i++) {
			pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
		}
		pEyeWnd->m_pCTExam = m_pCTExam;
		if (pWndSettings->m_Method3D) {
			m_pCTExam->CreateWfm3D(pEyeWnd->m_Map3D, m_Surfaces[m_d]);
		}
		else {
			m_pCTExam->CreateWfm2D(pEyeWnd->m_Map2D, m_Surfaces[m_d], 1.0 / pEyeWnd->m_y_px_um);
		}
		pEyeWnd->CreateCentButtons();
		pEyeWnd->CreateZoomButtons();
		pEyeWnd->CreateZoneButtons();
		m_pDispWnd[m_d] = pEyeWnd;
	}
	break;

	case TYPE_CRMS:
	{
		CRMSWnd* pRMSWnd = new CRMSWnd(Rect, this);
		pRMSWnd->m_SimpliedRMS = pWndSettings->m_SimplifiedRMS;//530
		pRMSWnd->m_Type = pWndSettings->m_Type;
		pRMSWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
		pRMSWnd->m_Method3D = pWndSettings->m_Method3D;
		pRMSWnd->m_LTLabel[0] = "Root Mean Square";
		pWndSettings->m_Mask.GetName(pRMSWnd->m_LTLabel[1]);
		pRMSWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
		pRMSWnd->m_Surface = m_Surfaces[m_d];
		pRMSWnd->m_Individual = pWndSettings->m_RMSIndividual;
		if (!pRMSWnd->m_Method3D && pRMSWnd->m_Individual) pRMSWnd->CreateSimpButtons(!pWndSettings->m_SimplifiedRMS);//530
		pRMSWnd->CreateZoomButtons();
		pRMSWnd->CreateZoneButtons();
		m_pDispWnd[m_d] = pRMSWnd;
	}
	break;

	case TYPE_CSKM:
	{
		CEyeWnd* pEyeWnd = new CEyeWnd(Rect, this);
		pEyeWnd->m_Type = pWndSettings->m_Type;
		pEyeWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
		pEyeWnd->m_Method3D = FALSE;//pWndSettings->m_Method3D;
		pEyeWnd->m_LTLabel[0] = "Keratometry Map";
		pEyeWnd->m_MapShowEye = pWndSettings->m_MapShowEye;
		pEyeWnd->m_MapShowRings = FALSE;
		pEyeWnd->m_MapShowLimbus = pWndSettings->m_MapShowLimbus;
		pEyeWnd->m_MapShowPupil = pWndSettings->m_MapShowPupil;
		pEyeWnd->m_pCTExam = m_pCTExam;
		m_pDispWnd[m_d] = pEyeWnd;
	}
	break;

	// Add snellen E
	case TYPE_TLTR:
	{
		if (!(pWndSettings->m_Ltr == 0 || pWndSettings->m_Ltr == 1)) {
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

		CLtrWnd* pLtrWnd = new CLtrWnd(Rect, this);
		pLtrWnd->m_DysVal = -1;//531

		CWFExam::CreateLtr(m_Surfaces[m_d], pLtrWnd->m_Ltr, pWndSettings->m_Ltr, pWndSettings->m_LtrLine, pWndSettings->m_LtrOrientation);
		pLtrWnd->m_Type = pWndSettings->m_Type;
		pLtrWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
		pLtrWnd->m_Method3D = pWndSettings->m_Method3D;
		//pLtrWnd->m_LTLabel[0].Format(_T("Letter 20 / %i", pWndSettings->m_LtrLine);
		pLtrWnd->m_LTLabel[0] = "Letter";
		pWndSettings->m_Mask.GetName(pLtrWnd->m_LTLabel[1]);
		pLtrWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
		pLtrWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 1);//531		  
		pLtrWnd->CreateZoneButtons();

		m_pDispWnd[m_d] = pLtrWnd;
	}
	break;
	// Add snellen E Done

	}

	if (pWndSettings->m_Type == TYPE_CAXM ||
		pWndSettings->m_Type == TYPE_CTNM ||
		pWndSettings->m_Type == TYPE_CRFM ||
		pWndSettings->m_Type == TYPE_CELM ||
		pWndSettings->m_Type == TYPE_CWFM) {

		CEyeWnd* pEyeWnd = (CEyeWnd*)m_pDispWnd[m_d];
		CScale* pScale = pWndSettings->GetScale();
		real_t Inc = pWndSettings->GetIncrement();

		// middle value
		real_t Cent;
		if (pWndSettings->m_Type == TYPE_CAXM || pWndSettings->m_Type == TYPE_CTNM || pWndSettings->m_Type == TYPE_CRFM) {
			int MapUnit = (pWndSettings->m_Type == TYPE_CRFM) || (pWndSettings->m_MapUnit == DIOPTERS) ? DIOPTERS : MILLIMETERS;
			if (pScale->m_CentAuto) {
				if (MapUnit == DIOPTERS) {
					Cent = m_pCTExam->m_ax0_dp;
				}
				else {
					Cent = 0.001 * m_pCTExam->m_ax0_um;
				}
			}
			else {
				Cent = pScale->m_Cent;
			}
		}
		else {
			Cent = 0.0;
		}
		Cent = RealRound(Cent, Inc);

		// step value
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

	else if (pWndSettings->m_Type == TYPE_CRMS)
	{
		/*CRMSWnd* pRMSWnd = (CRMSWnd*)m_pDispWnd[m_d];
		real t = pRMSWnd->m_Surface.ComputeRms();
		int Max = (int)t;
		if (t > Max + 0.001) Max++;
		if (Max < 1) Max = 1;
		pRMSWnd->m_Max = Max;*/

		//531 RMS Set scale to 1		
		CRMSWnd* pRMSWnd = (CRMSWnd*)m_pDispWnd[m_d];
		pRMSWnd->m_Max = 1;

		pRMSWnd->m_ZoomInButton.SetEnabled(FALSE);
		pRMSWnd->m_ZoomInButton.Repaint();
		//531 Done
	}

	//521 
	if (pWndSettings->m_Type == TYPE_CWFM || pWndSettings->m_Type == TYPE_CRMS)
	{
		m_pDispWnd[m_d]->m_IncZoneButton.SetEnabled(TRUE);
		m_pDispWnd[m_d]->m_IncZoneButton.Repaint();

		m_pDispWnd[m_d]->m_DecZoneButton.SetEnabled(TRUE);
		m_pDispWnd[m_d]->m_DecZoneButton.Repaint();


		if (pWndSettings->m_ZoneRadiusUm >= (int)::TempSettings.Com_r_max_um)
		{
			m_pDispWnd[m_d]->m_IncZoneButton.SetEnabled(FALSE);
			m_pDispWnd[m_d]->m_IncZoneButton.Repaint();
		}

		if (pWndSettings->m_ZoneRadiusUm == 500)
		{
			m_pDispWnd[m_d]->m_DecZoneButton.SetEnabled(FALSE);
			m_pDispWnd[m_d]->m_DecZoneButton.Repaint();
		}
	}
	//521
}

//***************************************************************************************

void CCTSingleSumWnd::CreateMenu()
{
	CWndSettings* pWndSettings = GetWndSettings();
	CScale* pScale = pWndSettings->GetScale();
	CXPMenu* pSubMenu;

	m_pMenu->AppendItem(IDC_TYPE_CEYE_ITEM, pWndSettings->m_Type == TYPE_CEYE, FALSE, "Eye Image", "");
	m_pMenu->AppendItem(IDC_TYPE_CAXM_ITEM, pWndSettings->m_Type == TYPE_CAXM, FALSE, "Axial Map", "");
	m_pMenu->AppendItem(IDC_TYPE_CTNM_ITEM, pWndSettings->m_Type == TYPE_CTNM, FALSE, "Local ROC Map", "");
	m_pMenu->AppendItem(IDC_TYPE_CRFM_ITEM, pWndSettings->m_Type == TYPE_CRFM, FALSE, "Refractive Map", "");
	m_pMenu->AppendItem(IDC_TYPE_CELM_ITEM, pWndSettings->m_Type == TYPE_CELM, FALSE, "Z Elevation Map", "");
	m_pMenu->AppendItem(IDC_TYPE_CWFM_ITEM, pWndSettings->m_Type == TYPE_CWFM, FALSE, "Wavefront Map", "");
	m_pMenu->AppendItem(IDC_TYPE_CRMS_ITEM, pWndSettings->m_Type == TYPE_CRMS, FALSE, "Root Mean Square", "");
	m_pMenu->AppendItem(IDC_TYPE_CSKM_ITEM, pWndSettings->m_Type == TYPE_CSKM, FALSE, "Keratometry Map", "");
	m_pMenu->AppendItem(IDC_TYPE_TLTR_ITEM, pWndSettings->m_Type == TYPE_TLTR, FALSE, "Visual Acuity Chart", "");//

																												 //[520] 'OneOsherAlignmentClicked'
																												 //if(IsWorkstation)
	{
		m_pMenu->AppendItem(IDC_TYPE_OSHER_ALIGNMENT, FALSE, FALSE, "Osher Iris Imaging", "");
	}

	m_pMenu->AppendSeparator();

	if (pWndSettings->m_Type == TYPE_CAXM ||
		pWndSettings->m_Type == TYPE_CTNM ||
		pWndSettings->m_Type == TYPE_CRFM ||
		pWndSettings->m_Type == TYPE_CELM ||
		pWndSettings->m_Type == TYPE_CWFM ||
		pWndSettings->m_Type == TYPE_CRMS && pWndSettings->m_RMSIndividual)
	{
		m_pMenu->AppendItem(IDC_METHOD_3D_ITEM, FALSE, pWndSettings->m_Method3D, "3D", "");
	}

	//
	if (pWndSettings->m_Type == TYPE_TLTR)
	{
		int type = 0;
		if (m_pCTExam->m_WfSurface.m_r_max_um <= 700) type = 1;

		CreateSubmenuZone(type);
		CreateSubmenuMask();
		m_pMenu->AppendSeparator();
		CreateSubmenuLtr();
		m_pMenu->AppendSeparator();
	}
	//

	m_pMenu->AppendItem(IDC_SIZE_LARGE_ITEM, FALSE, !pWndSettings->m_SizeSmall, "Large window", "");
	if (pWndSettings->m_Type != TYPE_CSKM && pWndSettings->m_Type != TYPE_CPLN && pWndSettings->m_Type != TYPE_TLTR) {
		m_pMenu->AppendSeparator();
	}

	if (pWndSettings->m_Type == TYPE_CAXM ||
		pWndSettings->m_Type == TYPE_CTNM) {
		m_pMenu->AppendItem(IDC_MAP_DIOPTERS_ITEM, pWndSettings->m_MapUnit == DIOPTERS, FALSE, "Diopters", "");
		m_pMenu->AppendItem(IDC_MAP_MILLIMETERS_ITEM, pWndSettings->m_MapUnit == MILLIMETERS, FALSE, "Millimeters", "");
		m_pMenu->AppendSeparator();
	}

	if (pWndSettings->m_Type == TYPE_CWFM ||
		pWndSettings->m_Type == TYPE_CRMS) {
		CreateSubmenuZone(1);
		CreateSubmenuMask();
		m_pMenu->AppendSeparator();
	}

	if (pWndSettings->m_Type == TYPE_CEYE) {
		m_pMenu->AppendItem(IDC_SHOW_MAP_RINGS_ITEM, FALSE, pWndSettings->m_MapShowRings, "Show rings", "");
		m_pMenu->AppendItem(IDC_SHOW_MAP_PUPIL_ITEM, FALSE, pWndSettings->m_MapShowPupil, "Show pupil", "");
		m_pMenu->AppendItem(IDC_SHOW_MAP_LIMBUS_ITEM, FALSE, pWndSettings->m_MapShowLimbus, "Show limbus", "");
	}
	else if (pWndSettings->m_Type == TYPE_CAXM ||
		pWndSettings->m_Type == TYPE_CTNM ||
		pWndSettings->m_Type == TYPE_CRFM ||
		pWndSettings->m_Type == TYPE_CELM ||
		pWndSettings->m_Type == TYPE_CWFM) {
		if (!pWndSettings->m_Method3D) {
			m_pMenu->AppendItem(IDC_SHOW_MAP_MAP_ITEM, FALSE, pWndSettings->m_MapShowMap, "Show map", "");
			m_pMenu->AppendItem(IDC_SHOW_MAP_NUMBERS_ITEM, FALSE, pWndSettings->m_MapShowNumbers, "Show numbers", "");
			m_pMenu->AppendItem(IDC_SHOW_MAP_EYE_ITEM, FALSE, pWndSettings->m_MapShowEye, "Show eye image", "");
			m_pMenu->AppendItem(IDC_SHOW_MAP_PUPIL_ITEM, FALSE, pWndSettings->m_MapShowPupil, "Show pupil", "");
			m_pMenu->AppendItem(IDC_SHOW_MAP_LIMBUS_ITEM, FALSE, pWndSettings->m_MapShowLimbus, "Show limbus", "");
			if (pWndSettings->m_Type == TYPE_CAXM) {
				m_pMenu->AppendItem(IDC_SHOW_MAP_KERATOMETRY_ITEM, FALSE, pWndSettings->m_MapShowKeratometry, "Show keratometry", "");
			}
			m_pMenu->AppendItem(IDC_MAP_TRANSLUCENT_ITEM, FALSE, pWndSettings->m_MapTranslucent, "Translucent map", "");
		}
		else {
			m_pMenu->AppendItem(IDC_SHOW_MAP_SURFACE_ITEM, FALSE, pWndSettings->m_MapShowSolidSurface, "Show solid surface", "");
			m_pMenu->AppendItem(IDC_SHOW_MAP_MESH_ITEM, FALSE, pWndSettings->m_MapShowWireMesh, "Show wire mesh", "");
		}
		if (pWndSettings->m_Type == TYPE_CAXM ||
			pWndSettings->m_Type == TYPE_CTNM ||
			pWndSettings->m_Type == TYPE_CRFM) {
			pSubMenu = m_pMenu->AppendSubmenu("Map middle value", "");
			pSubMenu->AppendItem(IDC_MAP_CENT_USER_ITEM, !pScale->m_CentAuto, FALSE, "Fixed...", "");
			pSubMenu->AppendItem(IDC_MAP_CENT_AUTO_ITEM, pScale->m_CentAuto, FALSE, "Auto", "");
		}
		CreateSubmenuMapSteps();
		CreateSubmenuMapColors();
		if (pWndSettings->m_Type == TYPE_CELM) {
			m_pMenu->AppendSeparator();
			pSubMenu = m_pMenu->AppendSubmenu("Reference surface", "");
			pSubMenu->AppendItem(IDC_REF_BEST_SPHERE_ITEM, pWndSettings->m_RefSurfaceType == REF_SURFACE_BEST_SPHERE, FALSE, "Best fit sphere", "");
			pSubMenu->AppendItem(IDC_REF_BEST_CONICOID_ITEM, pWndSettings->m_RefSurfaceType == REF_SURFACE_BEST_CONIC, FALSE, "Best fit conicoid", "");
			pSubMenu->AppendItem(IDC_REF_USER_CONICOID_ITEM, pWndSettings->m_RefSurfaceType == REF_SURFACE_USER_CONIC, FALSE, "User defined conicoid...", "");
		}
	}
	else if (pWndSettings->m_Type == TYPE_CRMS) {
		if (!pWndSettings->m_Method3D) {
			m_pMenu->AppendItem(IDC_RMS_CMB_ITEM, FALSE, !pWndSettings->m_RMSIndividual, "Combined", "");
		}
	}

	if (pWndSettings->m_Type == TYPE_CWFM ||
		pWndSettings->m_Type == TYPE_CRMS) {
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_SAVE_TXT_ITEM, FALSE, FALSE, "Save Zernike coefficients into file", "");
	}

	//521
	if (m_i == 0 && !SameCTSum())
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
	}

	if (m_i == 1 && !SameCTKera())
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
	}

	if (m_i == 2 && !SameCT3DZ())
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
	}
	//521

	//601
	if (m_i == 3)
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
	}
	//601
}

//***************************************************************************************
//521
void CCTSingleSumWnd::OnShowDefaultItemClicked()
{
	CBusyCursor Cursor;

	CWndSettings* pWndSettings = GetWndSettings();

	pWndSettings->m_LtrDistanceType = 0;//530

	if (m_i == 0)
	{
		if (m_d == 0)
		{
			pWndSettings->m_Type = 31;
			pWndSettings->m_MapUnit = DIOPTERS;

			pWndSettings->m_MapShowKeratometry = TRUE;//6.2.0
		}
		if (m_d == 1)
		{
			pWndSettings->m_Type = TYPE_TLTR;
			pWndSettings->m_RefSurfaceType = REF_SURFACE_BEST_SPHERE;

			pWndSettings->m_LtrLine = 100;

			pWndSettings->m_ZoneRadiusUm = 2000;//6.2.0

			pWndSettings->m_Mask.SetType(MASK_HO_TOTAL);//6.2.0
		}
		if (m_d == 2)
		{
			pWndSettings->m_Type = TYPE_CRMS;
			pWndSettings->m_MapUnit = MILLIMETERS;

			pWndSettings->m_ZoneRadiusUm = 2000;//6.2.0
		}
		if (m_d == 3)
		{
			pWndSettings->m_Type = TYPE_CEYE;
			pWndSettings->m_MapShowRings = FALSE;
		}

		pWndSettings->m_Method3D = 0;
		pWndSettings->m_SizeSmall = TRUE;
		pWndSettings->m_MapShowSolidSurface = TRUE;
		pWndSettings->m_MapShowWireMesh = TRUE;

		pWndSettings->m_MapShowMap = 1;
		pWndSettings->m_MapShowEye = 1;//521 change from 0 to 1
		pWndSettings->m_MapShowNumbers = 0;
		pWndSettings->m_MapShowPupil = 0;
		pWndSettings->m_MapShowLimbus = 0;
		pWndSettings->m_MapShowCornealVertex = 0;
		pWndSettings->m_MapTranslucent = 0;	

		CScale* pScale = pWndSettings->GetScale();
		pScale->m_Step = 0.5;
		pWndSettings->SetDefaultColors(1);

		for (m_d = 0; m_d < 4; m_d++)
		{
			CreateChildWnd();
		}
	}
	else if (m_i == 1)
	{
		for (int i = 0; i < 4; i++) SAFE_DELETE(m_pDispWnd[i]);

		m_pWndSettings[m_d].m_Type = 51;
		m_pWndSettings[m_d].m_SizeSmall = FALSE;

		CreateChildWnd();
	}
	else if (m_i == 2)
	{
		//if (m_isCombo)//530
		//{
		//	OneOsherAlignmentClicked();
		//}
		//else 
		{
			for (int i = 0; i < 4; i++) SAFE_DELETE(m_pDispWnd[i]);
			m_pWndSettings[m_d].m_Type = 34;
			m_pWndSettings[m_d].m_SizeSmall = 0;
			m_pWndSettings[m_d].m_Method3D = 1;
			m_pWndSettings[m_d].m_MapShowSolidSurface = 1;
			m_pWndSettings[m_d].m_MapShowWireMesh = 1;

			CScale* pScale = m_pWndSettings[m_d].GetScale();
			pScale->m_Step = 0.5;

			m_pWndSettings[m_d].SetDefaultColors(1);

			m_pWndSettings[m_d].m_RefSurfaceType = REF_SURFACE_BEST_SPHERE;

			CreateChildWnd();
		}
	}
	//601
	else if (m_i == 3)
	{
		OneOsherAlignmentClicked();
	}
	//601

	Repaint();
}
//521
//***************************************************************************************
//521
BOOL CCTSingleSumWnd::SameCTSum()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_d == 0)
	{
		if (pWndSettings->m_Type != 31)        return FALSE;
		if (pWndSettings->m_MapUnit != DIOPTERS)  return FALSE;

		if (pWndSettings->m_MapShowKeratometry != TRUE)  return FALSE;
	}
	if (m_d == 1)
	{
		if (pWndSettings->m_Type != TYPE_TLTR)          return FALSE;
		if (pWndSettings->m_ZoneRadiusUm != 2000)       return FALSE;
		if (pWndSettings->m_LtrLine != 100)             return FALSE;
	}
	if (m_d == 2)
	{
		if (pWndSettings->m_Type != TYPE_CRMS)          return FALSE;

		if (pWndSettings->m_ZoneRadiusUm != 2000)       return FALSE;
	}
	if (m_d == 3)
	{
		if (pWndSettings->m_Type != TYPE_CEYE)          return FALSE;
		if (pWndSettings->m_MapShowRings != FALSE)      return FALSE;
	}

	if (pWndSettings->m_Method3D != 0)        return FALSE;
	if (pWndSettings->m_SizeSmall != 1)        return FALSE;
	if (pWndSettings->m_MapShowMap != 1)        return FALSE;
	if (pWndSettings->m_MapShowEye != 1)        return FALSE;
	if (pWndSettings->m_MapShowNumbers != 0)        return FALSE;
	if (pWndSettings->m_MapShowPupil != 0)        return FALSE;
	if (pWndSettings->m_MapShowLimbus != 0)        return FALSE;
	if (pWndSettings->m_MapShowCornealVertex != 0)        return FALSE;
	if (pWndSettings->m_MapTranslucent != 0)        return FALSE;

	CScale* pScale = pWndSettings->GetScale();
	if (pScale->m_Step != 0.5) return FALSE;
	if (!pWndSettings->AreDefaultColors(1))            return FALSE;

	return TRUE;
}
//521
//***************************************************************************************
//521
BOOL CCTSingleSumWnd::SameCTKera()
{
	if (m_pWndSettings[m_d].m_Type != 51) return FALSE;
	if (m_pWndSettings[m_d].m_SizeSmall == 1)  return FALSE;

	return TRUE;
}
//521
//***************************************************************************************
//521
BOOL CCTSingleSumWnd::SameCT3DZ()
{
	if (m_pWndSettings[m_d].m_Type != 34) return FALSE;
	if (m_pWndSettings[m_d].m_SizeSmall == 1)  return FALSE;
	if (m_pWndSettings[m_d].m_Method3D == 0)  return FALSE;
	if (m_pWndSettings[m_d].m_MapShowSolidSurface == 0)  return FALSE;
	if (m_pWndSettings[m_d].m_MapShowWireMesh == 0)  return FALSE;

	CScale* pScale = m_pWndSettings[m_d].GetScale();
	if (pScale->m_Step != 0.5)  return FALSE;

	if (!m_pWndSettings[m_d].AreDefaultColors(1))         return FALSE;

	if (!m_pWndSettings[m_d].m_RefSurfaceType == REF_SURFACE_BEST_SPHERE) return FALSE;

	return TRUE;
}
//521
//***************************************************************************************

//521, Invoke Okulix function
LRESULT CCTSingleSumWnd::OnOkulix(WPARAM wParam, LPARAM lParam)
{
	//1. Create Folder for Okulix
	CString FolderName = "C:\\Tracey";

	if (!::PathFileExists(FolderName))
		::CreateDirectory(FolderName, NULL);

	FolderName += "\\OKULIX";

	if (!::PathFileExists(FolderName))
		::CreateDirectory(FolderName, NULL);
	//Done


	//2. Write the exam data to file CORNEA.ACT
	CString FlieName = FolderName + "\\CORNEA.ACT";
	CStringA AstrFileName(FlieName);
	FILE* pFile = fopen(AstrFileName, "w");

	CString Line0, Line1, Line2, Line3, Line4, Line5;

	CString ZeroLine = "";

	for (int i = 0; i < 29; i++)
	{
		ZeroLine += "0.000 ";
	}
	ZeroLine += "0.000";

	//Get Line0
	int strLength;

	CString LastName = m_pPatient->m_LastName;
	if (LastName == "") LastName = "None     ";
	else
	{
		strLength = LastName.GetLength();

		if (strLength > 9)
			LastName.Truncate(9);
		else
		{
			int lessLen = 9 - strLength;
			for (int i = 0; i < lessLen; i++)
			{
				LastName += " ";
			}
		}
	}
	Line0 = LastName + " ";

	CString FirstName = m_pPatient->m_FirstName;
	if (FirstName == "") LastName = "None ";
	else
	{
		strLength = FirstName.GetLength();

		if (strLength > 5)
			FirstName.Truncate(5);
		else
		{
			int lessLen = 5 - strLength;
			for (int i = 0; i < lessLen; i++)
			{
				FirstName += " ";
			}
		}
	}
	Line0 += FirstName + " ";

	CString Dob;
	Dob.Format(_T("%02i.%02i.%04i"), m_pPatient->m_BirthMonth, m_pPatient->m_BirthDay, m_pPatient->m_BirthYear);
	Line0 += Dob + " ";

	CString OD = (m_pCTExam->m_Header.IsOD() ? "OD" : "OS");
	Line0 += OD + " ";

	CString ExamDate;
	ExamDate.Format(_T("%02i.%02i.%04i"), m_pCTExam->m_Header.m_Month, m_pCTExam->m_Header.m_Day, m_pCTExam->m_Header.m_Year);
	Line0 += ExamDate + " ";

	CString  X_pupil_offset = "     ";//5
	Line0 += X_pupil_offset + " ";

	CString  Y_pupil_offset = "     ";//5
	Line0 += Y_pupil_offset + " ";

	CString  Axial_eye_length = "     ";//5
	Line0 += Axial_eye_length + " ";

	CString  Len_measur_device = "  ";//2
	Line0 += Len_measur_device + " ";

	CString  Anterior_chamber_depth = "     ";//5
	Line0 += Anterior_chamber_depth + " ";

	CString Lens_thickness = "     ";//5
	Line0 += Lens_thickness + " ";

	CString Topographic_device = "ITRAC";
	Line0 += Topographic_device;

	fprintf(pFile, "%s\n", Line0);
	//Get Line0

	//Get Line1-Line6
	CEyeImage* pImage = &m_pCTExam->m_Image;
	CString Radial_Dis, Tangential;
	real_t r_um, tangential_um;

	if (pImage->m_ri_ok)
	{
		for (int a = 1; a <= 360; a++)
		{
			Line1 = "";
			Line2 = "";
			Line3 = "";
			Line4 = "";
			Line5 = "";

			r_um = 0;
			tangential_um = 0;

			for (int i = 0; i < 30; i++)
			{
				if (i < pImage->m_NumRings)
					r_um = pImage->m_ri_r_um[i][a];
				else r_um = 0;

				if (r_um == 0 || r_um == INVALID_VALUE) Radial_Dis = "0.000";
				else
				{
					if (r_um >= 5000)
						Radial_Dis = "0.000";
					else
						Radial_Dis.Format(_T("%.3f"), r_um / 1000);
				}
				if (i < 29) Line1 += Radial_Dis + " ";
				else       Line1 += Radial_Dis;


				//Line2 
				real_t a_rd = _2_Pi * a / 360;

				if (i < pImage->m_NumRings)
					tangential_um = m_pCTExam->GetTnUmAt(r_um, a_rd);
				else tangential_um = 0;

				if (tangential_um == 0 || tangential_um == INVALID_VALUE) Tangential = "0.000";
				else
				{
					if (tangential_um >= 10000)
						Tangential.Format(_T("%.2f"), tangential_um / 1000);
					else
						Tangential.Format(_T("%.3f"), tangential_um / 1000);
				}

				if (i < 29) Line2 += Tangential + " ";
				else       Line2 += Tangential;
			}
			fprintf(pFile, "%s\n", Line1);
			fprintf(pFile, "%s\n", Line2); //test for Line2	    	
										   //Get Line1

										   //Get Line3 / reserved for Okulix	
			fprintf(pFile, "%s\n", ZeroLine); //Line3
											  //Line3

											  //Get Line4 	
			fprintf(pFile, "%s\n", ZeroLine); //Line4
											  //Line4

											  //Get Line5 	
			fprintf(pFile, "%s\n", ZeroLine); //Line5			
											  //Line5
		}
	}

	fclose(pFile);
	//Done


	//3. Invoke Okulix 
	char buf[256];
	GetCurrentDirectory(256, LPWSTR(buf));
	SetCurrentDirectory(LPWSTR("C:\\Tracey\\OKULIX"));
	WinExec("C:\\Tracey\\OKULIX\\augt.exe", SW_SHOW);
	SetCurrentDirectory(LPWSTR(buf));

	//HINSTANCE hDllInst =::LoadLibrary("C:\\Tracey\\korax.dll");
	//if(hDllInst)
	//{
	//	 ::Info("hERE");
	//	 typedef void (WINAPI *TDLLaugtom)(short, short, short, float); 

	//	 TDLLaugtom   fun   =   NULL;   
	//	 fun = (TDLLaugtom)GetProcAddress(hDllInst,"augtom"); 
	//  
	//	 if(fun)
	//	 {
	//	   short isp;      // language, 1 for German, 2 for English
	//	   short ixa;      // x-coordinate of upper left OKULIX window
	//	   short iya;      // y-coordinate of upper left OKULIX window
	//	   float fag;      // OKULIX window size factor: 1, 1.25, 1.6, 2.0
	//		isp=2;          // English
	//		ixa=0;
	//		iya=0;
	//		fag=1.25;       // window is 800X600
	//		fun(isp, ixa, iya, fag);
	//	 }
	//}
	//FreeLibrary(hDllInst);  

	//Done
	return S_OK;
}
//521, Invoke Okulix function

//***************************************************************************************
//530
LRESULT CCTSingleSumWnd::OnChangeRmsGUI(WPARAM wParam, LPARAM lParam)
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
					(W == 1920 ? IDR_SHOW_DETAIL_3 : W == 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
					(W == 1920 ? IDR_HIDE_DETAIL_3 : W == 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
			}
			else
			{
				m_pDispWnd[d]->m_ShowMoreButton.SetImage(m_pWndSettings[d].m_SimplifiedRMS ?
					(W == 1920 ? IDR_SHOW_DETAIL_6 : W == 1600 ? IDR_SHOW_DETAIL_5 : IDR_SHOW_DETAIL_4) :
					(W == 1920 ? IDR_HIDE_DETAIL_6 : W == 1600 ? IDR_HIDE_DETAIL_5 : IDR_HIDE_DETAIL_4));
			}

			m_pDispWnd[d]->m_ShowMoreButton.Repaint();

			m_pDispWnd[d]->Repaint();
		}
	}

	return 0;
}
//530
//***************************************************************************************