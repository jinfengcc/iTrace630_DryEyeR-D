//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "CTSingleCmpWnd.h"
#include "EyeWnd.h"
#include "RMSWnd.h"
#include "LtrWnd.h"//
#include "BusyCursor.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CCTSingleCmpWnd, CSumWnd)

	ON_MESSAGE(WM_ROTATE, OnRotateMsg)
	ON_MESSAGE(WM_CHANGE_CENT, OnChangeCentMsg)
	ON_MESSAGE(WM_CHANGE_STEP, OnChangeStepMsg)
	ON_MESSAGE(WM_CHANGE_MAX, OnChangeMaxMsg)

	ON_COMMAND(IDC_DEFAULT_GUI_ITEM, OnShowDefaultItemClicked)//521

	ON_MESSAGE(WM_SHOW_SIMPRMS, OnChangeRmsGUI)//530 Change the rms gui between simplify and details

	ON_BN_CLICKED(IDC_SHOW_FULL_CTINFO, OnShowCTTextInfoBtnClicked)//530
	ON_BN_CLICKED(IDC_SHOW_FULL_CTINFO2, OnShowCTTextInfoBtn2Clicked)//530

END_MESSAGE_MAP()

//***************************************************************************************

CCTSingleCmpWnd::CCTSingleCmpWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CCTExam* pCTExam1, CCTExam* pCTExam2, CWndSettings* pWndSettings) :
	CSumWnd(pWnd, WndRect, pPatient, pWndSettings)
{
	CBusyCursor Cursor;

	//----------------------------------------------------
	RECT Rect;
	GetWindowRect(&Rect);
	real_t t = 3.0 * (0.03 * m_h) + m_g;
	real_t w = ((Rect.right - Rect.left) - 4.0 * m_g) / 3.0;
	real_t h = ((Rect.bottom - Rect.top - t) - 2.0 * m_g) / 2.0;
	::SetRect(&m_Rect[0], intRound(m_g), intRound(t), intRound(m_g + w), intRound(t + h)); // exam 1 map
	::SetRect(&m_Rect[1], intRound(m_g + w + m_g), intRound(t), intRound(m_g + w + m_g + w), intRound(t + h)); // exam 2 map
	::SetRect(&m_Rect[2], intRound(m_g + w + m_g + w + m_g), intRound(t), intRound(m_g + w + m_g + w + m_g + w), intRound(t + h)); // diff map
	::SetRect(&m_Rect[3], intRound(m_g), intRound(t + h + m_g), intRound(m_g + w), intRound(t + h + m_g + h)); // exam 1 info
	::SetRect(&m_Rect[4], intRound(m_g + w + m_g), intRound(t + h + m_g), intRound(m_g + w + m_g + w), intRound(t + h + m_g + h)); // exam 2 info
																																   //----------------------------------------------------

	m_pCTExam1 = pCTExam1;
	m_pCTExam2 = pCTExam2;

	//*007*[cjf***05052012],record the increase and decrease range
	::TempSettings.Com_r_max_um = ((m_pCTExam1->m_WfSurface.m_r_max_um) < (m_pCTExam2->m_WfSurface.m_r_max_um) ? m_pCTExam1->m_WfSurface.m_r_max_um : m_pCTExam2->m_WfSurface.m_r_max_um);
	//*007*[cjf***05052012]

	CreateChildWnd();
	Repaint();

	//530 Draw the CT Text Info button
	CString s = (m_pWndSettings[0].m_SimplifiedWFPanel ? "Show Details" : "Hide Details");
	::SetRect(&Rect, m_Rect[3].right - 25, m_Rect[3].top + 5, m_Rect[3].right - 5, m_Rect[3].top + 25);
	m_CTShowFullInfoButton.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_SHOW_FULL_CTINFO);
	m_CTShowFullInfoButton.SetImage(m_pWndSettings[0].m_SimplifiedWFPanel ?
		(m_SW >= 1920 ? IDR_SHOW_DETAIL_3 : m_SW >= 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
		(m_SW >= 1920 ? IDR_HIDE_DETAIL_3 : m_SW >= 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
	m_CTShowFullInfoButton.SetBk(FALSE);
	m_CTShowFullInfoButton.SetBkColor(BLACK);
	m_CTShowFullInfoButton.SetTip(s);
	m_CTShowFullInfoButton.ShowWindow(SW_SHOW);

	::SetRect(&Rect, m_Rect[4].right - 25, m_Rect[4].top + 5, m_Rect[4].right - 5, m_Rect[4].top + 25);
	m_CTShowFullInfoButton2.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_SHOW_FULL_CTINFO);
	m_CTShowFullInfoButton2.SetImage(m_pWndSettings[0].m_SimplifiedWFPanel ?
		(m_SW >= 1920 ? IDR_SHOW_DETAIL_3 : m_SW >= 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
		(m_SW >= 1920 ? IDR_HIDE_DETAIL_3 : m_SW >= 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
	m_CTShowFullInfoButton2.SetBk(FALSE);
	m_CTShowFullInfoButton2.SetBkColor(BLACK);
	m_CTShowFullInfoButton2.SetTip(s);
	m_CTShowFullInfoButton2.ShowWindow(SW_SHOW);
	//Done

	ShowWindow(SW_SHOW);
}

//***************************************************************************************

void CCTSingleCmpWnd::RepaintMemDC()
{
	CSumWnd::RepaintMemDC();

	BOOL Rows[22] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

	if (m_pCTExam1) {
		if (m_pWndSettings[0].m_SimplifiedCTPanel == 1)
			ChangCTExamInfo(m_pCTExam1, m_Rect[3]);
		else CTExamInfo(m_pCTExam1, m_Rect[3], 22, Rows);
	}

	if (m_pCTExam2) {
		if (m_pWndSettings[0].m_SimplifiedCTPanel == 1)
			ChangCTExamInfo(m_pCTExam2, m_Rect[4]);
		else CTExamInfo(m_pCTExam2, m_Rect[4], 22, Rows);
	}
}

//***************************************************************************************

void CCTSingleCmpWnd::CreateChildWnd()
{
	for (int d = 0; d < 3; d++) SAFE_DELETE(m_pDispWnd[d]);

	CWndSettings* pWndSettings = GetWndSettings();

	// type
	if (pWndSettings->m_Type != TYPE_CEYE &&
		pWndSettings->m_Type != TYPE_CAXM &&
		pWndSettings->m_Type != TYPE_CTNM &&
		pWndSettings->m_Type != TYPE_CRFM &&
		pWndSettings->m_Type != TYPE_CELM &&
		pWndSettings->m_Type != TYPE_CWFM &&
		pWndSettings->m_Type != TYPE_CRMS &&
		pWndSettings->m_Type != TYPE_TLTR) // Add TYPE_TLTR
	{
		pWndSettings->m_Type = TYPE_CAXM;
	}

	//if (!::Licensing.IsWorkstation() && !::Licensing.IsViewer() && !::Licensing.IsCombo()  && !::Licensing.IsDemo() && !::Licensing.IsPresbia() ) 
	//{
	//  // method
	//  pWndSettings->m_Method3D = FALSE;
	//  // mask
	//  if (pWndSettings->m_Type == TYPE_CWFM ||
	//      pWndSettings->m_Type == TYPE_CRMS) {
	//    int MaskType = pWndSettings->m_Mask.GetType();
	//    if (MaskType != MASK_TOTAL &&
	//        MaskType != MASK_TOTAL_NO_DEFOCUS &&
	//        MaskType != MASK_LO_TOTAL &&
	//        MaskType != MASK_HO_TOTAL) {
	//      pWndSettings->m_Mask.SetType(MASK_TOTAL);
	//      pWndSettings->m_Piston = TRUE;
	//      pWndSettings->m_Tilt = TRUE;
	//    }
	//  }
	//  // colors
	//  if (pWndSettings->m_Type == TYPE_CAXM ||
	//      pWndSettings->m_Type == TYPE_CTNM ||
	//      pWndSettings->m_Type == TYPE_CRFM ||
	//      pWndSettings->m_Type == TYPE_CELM ||
	//      pWndSettings->m_Type == TYPE_CWFM) {
	//    if (!pWndSettings->AreDefaultColors()) {
	//      pWndSettings->SetDefaultColors(1);
	//    }
	//  }
	//}

	if (pWndSettings->m_Type == TYPE_CRMS) {
		if (pWndSettings->m_Method3D) {
			pWndSettings->m_RMSIndividual = TRUE;
		}
	}

	CCTExam* pCTExam[3] = { m_pCTExam1, m_pCTExam2, NULL };
	// copy
	m_Surfaces[0] = pCTExam[0]->m_WfSurface;
	m_Surfaces[1] = pCTExam[1]->m_WfSurface;
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

	for (int d = 0; d < 3; d++) {

		switch (pWndSettings->m_Type) {

		case TYPE_CEYE:
		{
			if (d < 2) {
				CEyeWnd* pEyeWnd = new CEyeWnd(m_Rect[d], this);
				pEyeWnd->m_Type = pWndSettings->m_Type;
				pEyeWnd->m_SizeSmall = TRUE;
				pEyeWnd->m_Method3D = FALSE;
				pEyeWnd->m_MapShowEye = TRUE;
				pEyeWnd->m_MapShowLimbus = pWndSettings->m_MapShowLimbus;
				pEyeWnd->m_MapShowPupil = pWndSettings->m_MapShowPupil;
				pEyeWnd->m_MapShowRings = pWndSettings->m_MapShowRings;
				pEyeWnd->m_pCTExam = pCTExam[d];
				m_pDispWnd[d] = pEyeWnd;
			}
		}
		break;

		case TYPE_CAXM:
		case TYPE_CTNM:
		case TYPE_CRFM:
		{
			int MapUnit = (pWndSettings->m_Type == TYPE_CRFM) || (pWndSettings->m_MapUnit == DIOPTERS) ? DIOPTERS : MILLIMETERS;
			CEyeWnd* pEyeWnd = new CEyeWnd(m_Rect[d], this);
			pEyeWnd->m_Type = pWndSettings->m_Type;
			pEyeWnd->m_SizeSmall = TRUE;
			pEyeWnd->m_Method3D = pWndSettings->m_Method3D;
			if (pWndSettings->m_Type == TYPE_CAXM) {
				pEyeWnd->m_LTLabel[0] = "Axial Map";
			}
			else if (pWndSettings->m_Type == TYPE_CTNM) {
				pEyeWnd->m_LTLabel[0] = "Local ROC Map";
			}
			else {
				pEyeWnd->m_LTLabel[0] = "Refractive Map";
			}
			pEyeWnd->m_MapShowEye = pWndSettings->m_MapShowEye;
			pEyeWnd->m_MapShowLimbus = pWndSettings->m_MapShowLimbus;
			pEyeWnd->m_MapShowPupil = pWndSettings->m_MapShowPupil;
			pEyeWnd->m_MapShowMap = pWndSettings->m_MapShowMap;
			pEyeWnd->m_MapShowNumbers = pWndSettings->m_MapShowNumbers;
			pEyeWnd->m_MapShowSolidSurface = pWndSettings->m_MapShowSolidSurface;
			pEyeWnd->m_MapShowWireMesh = pWndSettings->m_MapShowWireMesh;
			pEyeWnd->m_MapTranslucent = pWndSettings->m_MapTranslucent;
			pEyeWnd->m_MapShowKeratometry = d == 2 ? FALSE : pWndSettings->m_MapShowKeratometry;
			pEyeWnd->m_MapShowSimKAxes = d == 2 ? FALSE : pWndSettings->m_MapShowSimKAxes;
			pEyeWnd->m_Unit = MapUnit == DIOPTERS ? Unit::D : Unit::mm;
			pEyeWnd->m_Inc = pWndSettings->GetIncrement();
			CScale* pScale = pWndSettings->GetScale();
			pEyeWnd->m_NumColors = pScale->m_NumColors;
			for (int i = 0; i < pScale->m_NumColors; i++) {
				pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
			}
			pEyeWnd->m_pCTExam = pCTExam[d];
			if (d < 2) {
				if (pWndSettings->m_Method3D) {
					pCTExam[d]->CreateDpm3D(pEyeWnd->m_Map3D, pWndSettings->m_Type, MapUnit);
				}
				else {
					pCTExam[d]->CreateDpm2D(pEyeWnd->m_Map2D, pWndSettings->m_Type, MapUnit, 1.0 / pEyeWnd->m_y_px_um);
				}
			}
			else {
				CEyeWnd* pEyeWnd1 = (CEyeWnd*)m_pDispWnd[0];
				CEyeWnd* pEyeWnd2 = (CEyeWnd*)m_pDispWnd[1];
				if (pWndSettings->m_Method3D) {
					pEyeWnd->m_Map3D.Subtract(pEyeWnd1->m_Map3D, pEyeWnd2->m_Map3D);
				}
				else {
					pEyeWnd->m_Map2D.Subtract(pEyeWnd1->m_Map2D, pEyeWnd2->m_Map2D);
				}
			}
			pEyeWnd->CreateCentButtons();
			pEyeWnd->CreateZoomButtons();
			m_pDispWnd[d] = pEyeWnd;
		}
		break;

		case TYPE_CELM:
		{
			CConicSurface RefSurface;
			RefSurface.Create(CT_R_MAX_UM, pWndSettings->m_RefSurfaceAx0Um, pWndSettings->m_RefSurfaceQ);

			CEyeWnd* pEyeWnd = new CEyeWnd(m_Rect[d], this);
			pEyeWnd->m_Type = pWndSettings->m_Type;
			pEyeWnd->m_SizeSmall = TRUE;
			pEyeWnd->m_Method3D = pWndSettings->m_Method3D;
			pEyeWnd->m_LTLabel[0] = "Z Elevation Map";
			if (d < 2) {
				pEyeWnd->m_RBLabel[4] = "Ref Surface";
				pEyeWnd->m_RBLabel[5].Format(_T("R0 = %.2f mm, Q = %.2f"), 0.001 * RefSurface.m_ax0_um, RefSurface.m_q);
				pEyeWnd->m_RBLabelColor[0] = m_Printing ? BLACK : WHITE;
				pEyeWnd->m_RBLabelColor[1] = pEyeWnd->m_RBLabelColor[0];
				pEyeWnd->m_RBLabelColor[2] = pEyeWnd->m_RBLabelColor[0];
				pEyeWnd->m_RBLabelColor[3] = pEyeWnd->m_RBLabelColor[0];
				pEyeWnd->m_RBLabelColor[4] = pEyeWnd->m_RBLabelColor[0];
				pEyeWnd->m_RBLabelColor[5] = pEyeWnd->m_RBLabelColor[0];
				pEyeWnd->m_RBLabelBkColor = NOCOLOR;
			}
			pEyeWnd->m_MapShowEye = pWndSettings->m_MapShowEye;
			pEyeWnd->m_MapShowLimbus = pWndSettings->m_MapShowLimbus;
			pEyeWnd->m_MapShowPupil = pWndSettings->m_MapShowPupil;
			pEyeWnd->m_MapShowMap = pWndSettings->m_MapShowMap;
			pEyeWnd->m_MapShowNumbers = pWndSettings->m_MapShowNumbers;
			pEyeWnd->m_MapShowSolidSurface = pWndSettings->m_MapShowSolidSurface;
			pEyeWnd->m_MapShowWireMesh = pWndSettings->m_MapShowWireMesh;
			pEyeWnd->m_MapTranslucent = pWndSettings->m_MapTranslucent;
			pEyeWnd->m_Unit = Unit::micron;
			pEyeWnd->m_Inc = pWndSettings->GetIncrement();
			CScale* pScale = pWndSettings->GetScale();
			pEyeWnd->m_NumColors = pScale->m_NumColors;
			for (int i = 0; i < pScale->m_NumColors; i++) {
				pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
			}
			pEyeWnd->m_pCTExam = pCTExam[d];
			if (d < 2) {
				if (pWndSettings->m_Method3D) {
					pCTExam[d]->CreateElm3D(pEyeWnd->m_Map3D, RefSurface);
				}
				else {
					pCTExam[d]->CreateElm2D(pEyeWnd->m_Map2D, RefSurface, 1.0 / pEyeWnd->m_y_px_um);
				}
			}
			else {
				CEyeWnd* pEyeWnd1 = (CEyeWnd*)m_pDispWnd[0];
				CEyeWnd* pEyeWnd2 = (CEyeWnd*)m_pDispWnd[1];
				if (pWndSettings->m_Method3D) {
					pEyeWnd->m_Map3D.Subtract(pEyeWnd1->m_Map3D, pEyeWnd2->m_Map3D);
				}
				else {
					pEyeWnd->m_Map2D.Subtract(pEyeWnd1->m_Map2D, pEyeWnd2->m_Map2D);
				}
			}
			pEyeWnd->CreateCentButtons();
			pEyeWnd->CreateZoomButtons();
			m_pDispWnd[d] = pEyeWnd;
		}
		break;

		case TYPE_CWFM:
		{
			CEyeWnd* pEyeWnd = new CEyeWnd(m_Rect[d], this);
			pEyeWnd->m_Type = pWndSettings->m_Type;
			pEyeWnd->m_SizeSmall = TRUE;
			pEyeWnd->m_Method3D = pWndSettings->m_Method3D;
			pEyeWnd->m_LTLabel[0] = "Wavefront Map";
			pWndSettings->m_Mask.GetName(pEyeWnd->m_LTLabel[1]);
			pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
			pEyeWnd->m_MapShowEye = pWndSettings->m_MapShowEye;
			pEyeWnd->m_MapShowLimbus = pWndSettings->m_MapShowLimbus;
			pEyeWnd->m_MapShowPupil = pWndSettings->m_MapShowPupil;
			pEyeWnd->m_MapShowMap = pWndSettings->m_MapShowMap;
			pEyeWnd->m_MapShowNumbers = pWndSettings->m_MapShowNumbers;
			pEyeWnd->m_MapShowSolidSurface = pWndSettings->m_MapShowSolidSurface;
			pEyeWnd->m_MapShowWireMesh = pWndSettings->m_MapShowWireMesh;
			pEyeWnd->m_MapTranslucent = pWndSettings->m_MapTranslucent;
			pEyeWnd->m_Unit = Unit::micron;
			pEyeWnd->m_Inc = pWndSettings->GetIncrement();
			CScale* pScale = pWndSettings->GetScale();
			pEyeWnd->m_NumColors = pScale->m_NumColors;
			for (int i = 0; i < pScale->m_NumColors; i++) {
				pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
			}
			pEyeWnd->m_pCTExam = pCTExam[d];
			if (d < 2) {
				if (pWndSettings->m_Method3D) {
					pCTExam[d]->CreateWfm3D(pEyeWnd->m_Map3D, m_Surfaces[d]);
				}
				else {
					pCTExam[d]->CreateWfm2D(pEyeWnd->m_Map2D, m_Surfaces[d], 1.0 / pEyeWnd->m_y_px_um);
				}
			}
			else {
				CEyeWnd* pEyeWnd1 = (CEyeWnd*)m_pDispWnd[0];
				CEyeWnd* pEyeWnd2 = (CEyeWnd*)m_pDispWnd[1];
				if (pWndSettings->m_Method3D) {
					pEyeWnd->m_Map3D.Subtract(pEyeWnd1->m_Map3D, pEyeWnd2->m_Map3D);
				}
				else {
					pEyeWnd->m_Map2D.Subtract(pEyeWnd1->m_Map2D, pEyeWnd2->m_Map2D);
				}
			}
			pEyeWnd->CreateCentButtons();
			pEyeWnd->CreateZoomButtons();
			pEyeWnd->CreateZoneButtons();
			m_pDispWnd[d] = pEyeWnd;
		}
		break;

		case TYPE_CRMS:
		{
			CRMSWnd* pRMSWnd = new CRMSWnd(m_Rect[d], this);
			pRMSWnd->m_SimpliedRMS = pWndSettings->m_SimplifiedRMS;//530
			pRMSWnd->m_Type = pWndSettings->m_Type;
			pRMSWnd->m_SizeSmall = TRUE;
			pRMSWnd->m_Method3D = pWndSettings->m_Method3D;
			pRMSWnd->m_LTLabel[0] = "Root Mean Square";
			pWndSettings->m_Mask.GetName(pRMSWnd->m_LTLabel[1]);
			pRMSWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
			pRMSWnd->m_Surface = m_Surfaces[d];
			pRMSWnd->m_Individual = pWndSettings->m_RMSIndividual;
			if (!pRMSWnd->m_Method3D && pRMSWnd->m_Individual) pRMSWnd->CreateSimpButtons(!pWndSettings->m_SimplifiedRMS);//530
			pRMSWnd->CreateZoomButtons();
			pRMSWnd->CreateZoneButtons();
			m_pDispWnd[d] = pRMSWnd;
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

			CLtrWnd* pLtrWnd = new CLtrWnd(m_Rect[d], this);
			pLtrWnd->m_DysVal = -1;//531

			CWFExam::CreateLtr(m_Surfaces[d], pLtrWnd->m_Ltr, pWndSettings->m_Ltr, pWndSettings->m_LtrLine, pWndSettings->m_LtrOrientation);
			pLtrWnd->m_Type = pWndSettings->m_Type;
			pLtrWnd->m_SizeSmall = TRUE;
			pLtrWnd->m_Method3D = pWndSettings->m_Method3D;
			//pLtrWnd->m_LTLabel[0].Format(_T("Letter 20 / %i", pWndSettings->m_LtrLine);
			pLtrWnd->m_LTLabel[0] = "Letter";
			pWndSettings->m_Mask.GetName(pLtrWnd->m_LTLabel[1]);
			pLtrWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
			pLtrWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 1);//531		  
			pLtrWnd->CreateZoneButtons();

			m_pDispWnd[d] = pLtrWnd;
		}
		break;
		// Add snellen E Done

		}
	}

	if (pWndSettings->m_Type == TYPE_CAXM || pWndSettings->m_Type == TYPE_CTNM || pWndSettings->m_Type == TYPE_CRFM ||
		pWndSettings->m_Type == TYPE_CELM || pWndSettings->m_Type == TYPE_CWFM) {

		CScale* pScale = pWndSettings->GetScale();
		real_t Inc = pWndSettings->GetIncrement();

		// middle value
		real_t Cent;
		if (pWndSettings->m_Type == TYPE_CAXM || pWndSettings->m_Type == TYPE_CTNM || pWndSettings->m_Type == TYPE_CRFM) {
			int MapUnit = (pWndSettings->m_Type == TYPE_CRFM) || (pWndSettings->m_MapUnit == DIOPTERS) ? DIOPTERS : MILLIMETERS;
			if (pScale->m_CentAuto) {
				if (MapUnit == DIOPTERS) {
					Cent = 0.5 * (m_pCTExam1->m_ax0_dp + m_pCTExam2->m_ax0_dp);
				}
				else {
					Cent = 0.001 * 0.5 * (m_pCTExam1->m_ax0_um + m_pCTExam2->m_ax0_um);
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
			real_t Span = 0.0;
			for (int d = 0; d < 3; d++) {
				CEyeWnd* pEyeWnd = (CEyeWnd*)m_pDispWnd[d];
				real_t Min = pWndSettings->m_Method3D ? pEyeWnd->m_Map3D.m_min : pEyeWnd->m_Map2D.m_min;
				real_t Max = pWndSettings->m_Method3D ? pEyeWnd->m_Map3D.m_max : pEyeWnd->m_Map2D.m_max;
				real_t Cnt = d == 2 ? 0.0 : Cent;
				Span = __max(Span, 2.0 * __max(fabs(Min - Cnt), fabs(Max - Cnt)));
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

		for (int d = 0; d < 3; d++) {
			CEyeWnd* pEyeWnd = (CEyeWnd*)m_pDispWnd[d];
			pEyeWnd->m_Cent = d == 2 ? 0.0 : Cent;
			pEyeWnd->m_Step = Step;
		}
	}

	else if (pWndSettings->m_Type == TYPE_CRMS)
	{
		//531 RMS Set scale to 1
		for (int d = 0; d < 3; d++)
		{
			((CRMSWnd*)m_pDispWnd[d])->m_Max = 1;

			((CRMSWnd*)m_pDispWnd[d])->m_ZoomInButton.SetEnabled(FALSE);
			((CRMSWnd*)m_pDispWnd[d])->m_ZoomInButton.Repaint();
		}
		//531 Done

		//int MaxMax = 1;
		//for (int d = 0; d < 3; d++) {
		//  CRMSWnd* pRMSWnd = (CRMSWnd*)m_pDispWnd[d];
		//  real t = pRMSWnd->m_Surface.ComputeRms();
		//  int Max = (int)t;
		//  if (t > Max + 0.001) Max++;
		//  MaxMax = __max(MaxMax, Max);
		//}
		//for (int d = 0; d < 3; d++) {
		//  ((CRMSWnd*)m_pDispWnd[d])->m_Max = MaxMax;

		//        //531
		// if(MaxMax == 1)
		// {
		//  ((CRMSWnd*)m_pDispWnd[d])->m_ZoomInButton.SetEnabled(FALSE);
		//  ((CRMSWnd*)m_pDispWnd[d])->m_ZoomInButton.Repaint();
		// }
		//    //531 Done
		//}
	}

	//521 
	if (pWndSettings->m_Type == TYPE_CWFM || pWndSettings->m_Type == TYPE_CRMS)
	{
		m_pDispWnd[m_d]->m_IncZoneButton.SetEnabled(TRUE);
		m_pDispWnd[m_d]->m_IncZoneButton.Repaint();

		m_pDispWnd[m_d]->m_DecZoneButton.SetEnabled(TRUE);
		m_pDispWnd[m_d]->m_DecZoneButton.Repaint();

		int PairWin1, PairWin2;

		if (m_d == 0)
		{
			PairWin1 = 1;
			PairWin2 = 2;
		}
		if (m_d == 1)
		{
			PairWin1 = 0;
			PairWin2 = 2;
		}
		if (m_d == 2)
		{
			PairWin1 = 0;
			PairWin2 = 1;
		}

		m_pDispWnd[PairWin1]->m_IncZoneButton.SetEnabled(TRUE);
		m_pDispWnd[PairWin1]->m_IncZoneButton.Repaint();

		m_pDispWnd[PairWin1]->m_DecZoneButton.SetEnabled(TRUE);
		m_pDispWnd[PairWin1]->m_DecZoneButton.Repaint();

		m_pDispWnd[PairWin2]->m_IncZoneButton.SetEnabled(TRUE);
		m_pDispWnd[PairWin2]->m_IncZoneButton.Repaint();

		m_pDispWnd[PairWin2]->m_DecZoneButton.SetEnabled(TRUE);
		m_pDispWnd[PairWin2]->m_DecZoneButton.Repaint();

		if (pWndSettings->m_ZoneRadiusUm >= (int)::TempSettings.Com_r_max_um)
		{
			m_pDispWnd[m_d]->m_IncZoneButton.SetEnabled(FALSE);
			m_pDispWnd[m_d]->m_IncZoneButton.Repaint();

			m_pDispWnd[PairWin1]->m_IncZoneButton.SetEnabled(FALSE);
			m_pDispWnd[PairWin1]->m_IncZoneButton.Repaint();

			m_pDispWnd[PairWin2]->m_IncZoneButton.SetEnabled(FALSE);
			m_pDispWnd[PairWin2]->m_IncZoneButton.Repaint();
		}

		if (pWndSettings->m_ZoneRadiusUm == 500)
		{
			m_pDispWnd[m_d]->m_DecZoneButton.SetEnabled(FALSE);
			m_pDispWnd[m_d]->m_DecZoneButton.Repaint();

			m_pDispWnd[PairWin1]->m_DecZoneButton.SetEnabled(FALSE);
			m_pDispWnd[PairWin1]->m_DecZoneButton.Repaint();

			m_pDispWnd[PairWin2]->m_DecZoneButton.SetEnabled(FALSE);
			m_pDispWnd[PairWin2]->m_DecZoneButton.Repaint();
		}
	}
	//521
}

//***************************************************************************************

LRESULT CCTSingleCmpWnd::OnRotateMsg(WPARAM wParam, LPARAM lParam)
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

LRESULT CCTSingleCmpWnd::OnChangeCentMsg(WPARAM wParam, LPARAM lParam)
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

LRESULT CCTSingleCmpWnd::OnChangeStepMsg(WPARAM wParam, LPARAM lParam)
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

LRESULT CCTSingleCmpWnd::OnChangeMaxMsg(WPARAM wParam, LPARAM lParam)
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
			else         ((CRMSWnd*)m_pDispWnd[d])->m_ZoomInButton.SetEnabled(TRUE);
			((CRMSWnd*)m_pDispWnd[d])->m_ZoomInButton.Repaint();

			m_pDispWnd[d]->Repaint();
			break;
		}
	}

	return 0;
}

//***************************************************************************************

void CCTSingleCmpWnd::CreateMenu()
{
	CWndSettings* pWndSettings = GetWndSettings();
	CScale* pScale = pWndSettings->GetScale();
	CXPMenu* pSubMenu;
	//BOOL IsWorkstation = ::Licensing.IsWorkstation();
	//BOOL IsViewer      = ::Licensing.IsViewer();
	//BOOL IsCombo       = ::Licensing.IsCombo();
	//BOOL IsDemo        = ::Licensing.IsDemo();//530
	//BOOL IsPresbia     = ::Licensing.IsPresbia();//530

	m_pMenu->AppendItem(IDC_TYPE_CEYE_ITEM, pWndSettings->m_Type == TYPE_CEYE, FALSE, "Eye Image", "");
	m_pMenu->AppendItem(IDC_TYPE_CAXM_ITEM, pWndSettings->m_Type == TYPE_CAXM, FALSE, "Axial Map", "");
	m_pMenu->AppendItem(IDC_TYPE_CTNM_ITEM, pWndSettings->m_Type == TYPE_CTNM, FALSE, "Local ROC Map", "");
	m_pMenu->AppendItem(IDC_TYPE_CRFM_ITEM, pWndSettings->m_Type == TYPE_CRFM, FALSE, "Refractive Map", "");
	m_pMenu->AppendItem(IDC_TYPE_CELM_ITEM, pWndSettings->m_Type == TYPE_CELM, FALSE, "Z Elevation Map", "");
	m_pMenu->AppendItem(IDC_TYPE_CWFM_ITEM, pWndSettings->m_Type == TYPE_CWFM, FALSE, "Wavefront Map", "");
	m_pMenu->AppendItem(IDC_TYPE_CRMS_ITEM, pWndSettings->m_Type == TYPE_CRMS, FALSE, "Root Mean Square", "");
	m_pMenu->AppendItem(IDC_TYPE_TLTR_ITEM, pWndSettings->m_Type == TYPE_TLTR, FALSE, "Visual Acuity Chart", "");//
	m_pMenu->AppendSeparator();


	if (pWndSettings->m_Type == TYPE_CAXM ||
		pWndSettings->m_Type == TYPE_CTNM ||
		pWndSettings->m_Type == TYPE_CRFM ||
		pWndSettings->m_Type == TYPE_CELM ||
		pWndSettings->m_Type == TYPE_CWFM ||
		pWndSettings->m_Type == TYPE_CRMS && pWndSettings->m_RMSIndividual) {
		m_pMenu->AppendItem(IDC_METHOD_3D_ITEM, FALSE, pWndSettings->m_Method3D, "3D", "");
		m_pMenu->AppendSeparator();
	}

	//
	if (pWndSettings->m_Type == TYPE_TLTR)
	{
		int type = 0;
		if (::TempSettings.Com_r_max_um <= 700) type = 1;

		CreateSubmenuZone(type);
		CreateSubmenuMask();
		m_pMenu->AppendSeparator();
		CreateSubmenuLtr();
	}
	//

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
			pSubMenu->AppendItem(IDC_REF_USER_CONICOID_ITEM, TRUE, FALSE, "User defined conicoid...", "");
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
	if (!SameCTCmp())
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
	}
	//521
}

//***************************************************************************************
//521
void CCTSingleCmpWnd::OnShowDefaultItemClicked()
{
	CBusyCursor Cursor;

	CWndSettings* pWndSettings = GetWndSettings();

	pWndSettings->m_LtrDistanceType = 0;//530

	pWndSettings->m_Type = 31;
	pWndSettings->m_Method3D = 0;
	pWndSettings->m_MapShowSolidSurface = TRUE;
	pWndSettings->m_MapShowWireMesh = TRUE;
	pWndSettings->m_MapUnit = DIOPTERS;

	pWndSettings->m_MapShowMap = 1;
	pWndSettings->m_MapShowEye = 1;
	pWndSettings->m_MapShowNumbers = 0;
	pWndSettings->m_MapShowPupil = 0;
	pWndSettings->m_MapShowLimbus = 0;
	pWndSettings->m_MapShowCornealVertex = 0;
	pWndSettings->m_MapTranslucent = 0;
	pWndSettings->m_MapShowKeratometry = 0;//530

	CScale* pScale = pWndSettings->GetScale();
	pScale->m_Step = 0.5;
	pWndSettings->SetDefaultColors(1);

	CreateChildWnd();

	Repaint();
}
//521
//***************************************************************************************
//521
BOOL CCTSingleCmpWnd::SameCTCmp()
{
	CWndSettings* pWndSettings = GetWndSettings();


	if (pWndSettings->m_Type != 31)       return FALSE;
	if (pWndSettings->m_Method3D != 0)        return FALSE;
	if (pWndSettings->m_MapUnit != DIOPTERS) return FALSE;

	if (pWndSettings->m_MapShowMap != 1)        return FALSE;
	if (pWndSettings->m_MapShowEye != 1)        return FALSE;
	if (pWndSettings->m_MapShowNumbers != 0)        return FALSE;
	if (pWndSettings->m_MapShowPupil != 0)        return FALSE;
	if (pWndSettings->m_MapShowLimbus != 0)        return FALSE;
	if (pWndSettings->m_MapShowCornealVertex != 0)        return FALSE;
	if (pWndSettings->m_MapTranslucent != 0)        return FALSE;
	if (pWndSettings->m_MapShowKeratometry != 0)        return FALSE;//530

	CScale* pScale = pWndSettings->GetScale();
	if (pScale->m_Step != 0.5)      return FALSE;

	if (!pWndSettings->AreDefaultColors(1))               return FALSE;

	return TRUE;
}
//521
//***************************************************************************************
//530
LRESULT CCTSingleCmpWnd::OnChangeRmsGUI(WPARAM wParam, LPARAM lParam)
{
	BOOL Simple = FALSE;

	for (int d = 0; d < 3; d++) {
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] == wParam)
		{
			Simple = !m_pWndSettings[d].m_SimplifiedRMS;
			break;
		}
	}

	int W = ::GetSystemMetrics(SM_CXSCREEN);
	for (int s_d = 0; s_d < 3; s_d++)
	{
		m_pWndSettings[s_d].m_SimplifiedRMS = Simple;
		((CRMSWnd*)m_pDispWnd[s_d])->m_SimpliedRMS = Simple;

		CString s = (Simple ? "Show Details" : "Hide Details");
		m_pDispWnd[s_d]->m_ShowMoreButton.SetTip(s);

		m_pDispWnd[s_d]->m_ShowMoreButton.SetImage(Simple ?
			(W >= 1920 ? IDR_SHOW_DETAIL_3 : W >= 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
			(W >= 1920 ? IDR_HIDE_DETAIL_3 : W >= 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));

		m_pDispWnd[s_d]->m_ShowMoreButton.Repaint();
		m_pDispWnd[s_d]->Repaint();
	}
	return 0;
}
//530
//***************************************************************************************
//530
void CCTSingleCmpWnd::OnShowCTTextInfoBtnClicked()
{
	m_pWndSettings[0].m_SimplifiedCTPanel = !m_pWndSettings[0].m_SimplifiedCTPanel;
	Repaint();

	CString s = (m_pWndSettings[0].m_SimplifiedWFPanel ? "Show Details" : "Hide Details");
	m_CTShowFullInfoButton.SetTip(s);
	m_CTShowFullInfoButton.SetImage(m_pWndSettings[0].m_SimplifiedCTPanel ?
		(m_SW >= 1920 ? IDR_SHOW_DETAIL_3 : m_SW >= 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
		(m_SW >= 1920 ? IDR_HIDE_DETAIL_3 : m_SW >= 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
	m_CTShowFullInfoButton.Repaint();

	m_CTShowFullInfoButton2.SetTip(s);
	m_CTShowFullInfoButton2.SetImage(m_pWndSettings[0].m_SimplifiedCTPanel ?
		(m_SW >= 1920 ? IDR_SHOW_DETAIL_3 : m_SW >= 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
		(m_SW >= 1920 ? IDR_HIDE_DETAIL_3 : m_SW >= 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
	m_CTShowFullInfoButton2.Repaint();
}

//***************************************************************************************

void CCTSingleCmpWnd::OnShowCTTextInfoBtn2Clicked()
{
	OnShowCTTextInfoBtnClicked();
}

//***************************************************************************************
