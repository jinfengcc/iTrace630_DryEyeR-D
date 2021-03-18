//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "WFCTIolWnd.h"
#include "EyeWnd.h"
#include "NewEyeWnd.h"
#include "PSFWnd.h"
#include "MTFWnd.h"
#include "EEFWnd.h"
#include "LtrWnd.h"
#include "RMSWnd.h"
#include "ChartWnd.h"
#include "BusyCursor.h"
#include "SettingsIOLSelDlg.h"
#include "GlobalFunctions.h"//

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CWFCTIolWnd, CSumWnd)

	ON_MESSAGE(WM_CHANGE_CENT, OnChangeCentMsg)
	ON_MESSAGE(WM_CHANGE_STEP, OnChangeStepMsg)

	ON_COMMAND(IDC_DEFAULT_GUI_ITEM, OnShowDefaultItemClicked)

	ON_BN_CLICKED(IDC_TRACEY_RECYES_ITEM, OnTraceYesClicked)
	ON_BN_CLICKED(IDC_TRACEY_RECNO_ITEM, OnTraceNoClicked)

	ON_BN_CLICKED(IDC_HOA_YES_ITEM, OnHoaYesClicked)
	ON_BN_CLICKED(IDC_HOA_NO_ITEM, OnHoaNoClicked)

	ON_BN_CLICKED(IDC_EYECOR_YES_ITEM, OnEyeCorYesClicked)
	ON_BN_CLICKED(IDC_EYECOR_NO_ITEM, OnEyeCorNoClicked)

	ON_BN_CLICKED(IDC_IOLS_BUTTON, OnSettingClicked)

	ON_MESSAGE(WM_CHANGE_MAX, OnChangeMaxMsg) //531

END_MESSAGE_MAP()

//***************************************************************************************

CWFCTIolWnd::CWFCTIolWnd(CWnd *pWnd, RECT &WndRect, CPatient *pPatient, CWFExam *pWFExam, CCTExam *pCTExam, CWndSettings *pWndSettings,
                         int show)
  : CSumWnd(pWnd, WndRect, pPatient, pWndSettings)
{
	CBusyCursor Cursor;

	//----------------------------------------------------
	RECT Rect;
	GetWindowRect(&Rect);
	real_t t = 3.0 * (0.03 * m_h) + m_g;
	real_t w2 = ((Rect.right - Rect.left) - 5.0 * m_g) / 6.0;
	real_t h2 = ((Rect.bottom - Rect.top - t) - 4.0 * m_g) / 4.0;
	real_t moveH = 3.0*m_g;

	real_t t1 = t + moveH;
	real_t w = w2 + w2;
	real_t h = h2 + m_g + h2;

	real_t h3 = 0.4 * h;

	int  Btm = intRound(t1 + h + m_g + h - moveH);
	int  mid_h = intRound((h - m_g) / 2);
	int  mid_Left = intRound(m_g + w + m_g + w2 + m_g);
	int  mid_Right = mid_Left + intRound((m_w - mid_Left - 2 * m_g)*0.57);


	::SetRect(&m_TitleRect[0], intRound(m_g), intRound(t - 2 * m_g), intRound(m_g + w + m_g + w2), intRound(t1 - m_g)); // Title "OD"

	::SetRect(&m_Rect[0], intRound(m_g + w2 + m_g), intRound(t1), intRound(m_g + w + m_g + w2), intRound(t1 + h)); // exam 1 map
	::SetRect(&m_Rect[1], intRound(m_g), intRound(t1), intRound(m_g + w2), intRound(t1 + h2)); // exam 1 psf
	::SetRect(&m_Rect[2], intRound(m_g), intRound(t1 + h2 + m_g), intRound(m_g + w2), intRound(t1 + h)); // exam 1 letter

	::SetRect(&m_Rect[3], intRound(m_g), intRound(t1 + h + m_g), intRound(m_g + w), intRound(t1 + h + h3));// WF exam 1 info
	::SetRect(&m_InfoRect[0], intRound(m_g), intRound(t1 + h + h3 + m_g), intRound(m_g + w), Btm); // CT exam 1 info

	::SetRect(&m_Rect[4], intRound(m_g + w + m_g), intRound(t1 + h + m_g), intRound(m_g + w + m_g + w2), intRound(t1 + h + h3)); // exam 1 WF complaints
	::SetRect(&m_ComplaintRect[0], intRound(m_g + w + m_g), intRound(t1 + h + h3 + m_g), intRound(m_g + w + m_g + w2), Btm); // exam 1 CT complaints

																															 //Middle Collum 
	int this_top = intRound(t1);
	int this_btm = intRound(t1 + mid_h);
	::SetRect(&m_Rect[5], mid_Left, this_top, mid_Right, this_btm); // RMS Wnd 1

	this_top = intRound(this_btm + m_g);
	this_btm = intRound(this_top + mid_h);
	::SetRect(&m_Rect[6], mid_Left, this_top, mid_Right, this_btm); // RMS Wnd 2

	this_top = intRound(this_btm + m_g);
	this_btm = intRound(this_top + mid_h);
	::SetRect(&m_Rect[7], mid_Left, this_top, mid_Right, this_btm); // RMS Wnd 3

	this_top = intRound(this_btm + m_g);
	::SetRect(&m_Rect[8], mid_Left, this_top, mid_Right, Btm); //Corneal Spherical Aberration Function RECT
															   //Done

															   //Right Panel
	int  right_Left = intRound(mid_Right + m_g);
	int  right_Right = intRound(m_w - m_g);
	::SetRect(&m_Rect[9], right_Left, intRound(t1), right_Right, this_btm);

	::SetRect(&m_Rect[10], right_Left, this_top, right_Right, Btm);
	//Done

	//----------------------------------------------------


	m_pWFExam = pWFExam;
	m_pCTExam = pCTExam;

	// WFCT Angle k/a adjust wf eye limbus by ct info
	LimbusAdjust(m_pWFExam, m_pCTExam);
	m_pWFExam->CalcAngles();
	// WFCT Angle k/a adjust wf eye limbus by ct info Done

	m_WFIolWnd = TRUE;

	//*007*[cjf***05052012],record the increase and decrease range

	real_t m_r;
	m_r = m_pWFExam->m_WfSurface.m_r_max_um;

	::TempSettings.Com_r_max_um = m_r;

	//521, to find the minimum border for refraction/wafefront border
	m_Scale = GetGridScale(m_r);
	//521

	//The Right panel checkbox and consideration res
	RECT TempRect;

	int L = m_Rect[9].left + 5, T = m_Rect[9].top + 3, R = m_Rect[9].right - 3, B = m_Rect[9].bottom;

	int h1 = intRound((m_Rect[9].bottom - m_Rect[9].top - 5) / 30);

	int top = T;

	int btm = intRound(top + h1);

	//Tracey refrection
	top += h1;
	btm = (top + h1);

	top += h1;
	btm = (top + h1);

	m_TraceySet = FALSE;
	if (m_pWFExam->m_Complaints.m_NightMiopia == COMP_SEVERE ||
		m_pWFExam->m_Complaints.m_NightHyperopia == COMP_SEVERE ||
		m_pWFExam->m_Complaints.m_Astigmatism == COMP_SEVERE ||
		m_pWFExam->m_Complaints.m_BlurDoubleVision == COMP_SEVERE ||
		m_pWFExam->m_Complaints.m_GlareHalo == COMP_SEVERE ||
		m_pWFExam->m_Complaints.m_MixedFocus == COMP_SEVERE)
	{
		m_TraceySet = FALSE;
	}
	else if (m_pWFExam->m_Complaints.m_NightMiopia == COMP_MODERATE ||
		m_pWFExam->m_Complaints.m_NightHyperopia == COMP_MODERATE ||
		m_pWFExam->m_Complaints.m_Astigmatism == COMP_MODERATE ||
		m_pWFExam->m_Complaints.m_BlurDoubleVision == COMP_MODERATE ||
		m_pWFExam->m_Complaints.m_GlareHalo == COMP_MODERATE ||
		m_pWFExam->m_Complaints.m_MixedFocus == COMP_MODERATE) {
		m_TraceySet = FALSE;
	}
	else {
		m_TraceySet = TRUE;
	}

	::SetRect(&TempRect, R - 60, top, R - 47, top + 13);
	m_TraceyRecYes.Create(_T(""), WS_CHILD | BS_CHECKBOX, TempRect, this, IDC_TRACEY_RECYES_ITEM);
	m_MemDC.FillSolidRect(&TempRect, BLACK);
	m_TraceyRecYes.ShowWindow(SW_SHOW);
	m_TraceyRecYes.SetCheck(m_TraceySet);

	::SetRect(&TempRect, R - 25, top, R - 12, top + 13);
	m_TraceyRecNo.Create(_T(""), WS_CHILD | BS_CHECKBOX, TempRect, this, IDC_TRACEY_RECNO_ITEM);
	m_MemDC.FillSolidRect(&TempRect, BLACK);
	m_TraceyRecNo.ShowWindow(SW_SHOW);
	m_TraceyRecNo.SetCheck(!m_TraceySet);

	::SetRect(&TempRect, R - 60, m_Rect[9].bottom - 30, R - 10, m_Rect[9].bottom - 10);
	m_IOLSetBtn.SetBkColor(NOCOLOR);
	m_IOLSetBtn.SetText("Setting");
	m_IOLSetBtn.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, TempRect, this, IDC_IOLS_BUTTON);
	//Done

	//Create surfaces and mask
	CZernikeMask Mask;
	Mask.SetType(MASK_HO_TOTAL);

	real_t r_um = 4000;

	CZernikeSurface Ct_WfSurface = m_pCTExam->m_WfSurface;//corneal
	r_um = __min(4000, Ct_WfSurface.m_r_max_um);

	CZernikeSurface WfSurface = m_pWFExam->m_WfSurface;//Entire
	r_um = __min(r_um, WfSurface.m_r_max_um);

	Ct_WfSurface.ChangeRMaxUm(r_um);
	WfSurface.ChangeRMaxUm(r_um);

	CZernikeSurface Int_WfSurface = WfSurface;//Ct_WfSurface; //internal
	Int_WfSurface -= Ct_WfSurface;
	//Done

	//Corneal HOA  
	Ct_WfSurface &= Mask;
	real_t rms = Ct_WfSurface.ComputeRms();
	m_vals[1] = rms;

	m_CorHoa = (rms >= ::NewSettings.m_IOLSel_CHOA ? TRUE : FALSE);//??

	top += h1;
	btm = (top + h1);

	top += h1;
	btm = (top + h1);

	::SetRect(&TempRect, R - 60, top, R - 47, top + 13);
	m_HOAYes.Create(_T(""), WS_CHILD | BS_CHECKBOX, TempRect, this, IDC_HOA_YES_ITEM);
	m_MemDC.FillSolidRect(&TempRect, BLACK);
	m_HOAYes.ShowWindow(SW_SHOW);
	m_HOAYes.SetCheck(m_CorHoa);

	::SetRect(&TempRect, R - 25, top, R - 12, top + 13);
	m_HOANo.Create(_T(""), WS_CHILD | BS_CHECKBOX, TempRect, this, IDC_HOA_NO_ITEM);
	m_MemDC.FillSolidRect(&TempRect, BLACK);
	m_HOANo.ShowWindow(SW_SHOW);
	m_HOANo.SetCheck(!m_CorHoa);
	//Done


	//Entire/Internal HOA /Potential visual complain
	Int_WfSurface &= Mask;
	rms = Int_WfSurface.ComputeRms();

	m_Inter = (rms >= ::NewSettings.m_IOLSel_IHOA ? TRUE : FALSE);

	top += h1;
	btm = (top + h1);

	top += h1;
	btm = (top + h1);

	::SetRect(&TempRect, R - 60, top, R - 47, top + 13);
	m_EyeCorYes.Create(_T(""), WS_CHILD | BS_CHECKBOX, TempRect, this, IDC_EYECOR_YES_ITEM);
	m_MemDC.FillSolidRect(&TempRect, BLACK);
	m_EyeCorYes.ShowWindow(SW_SHOW);
	m_EyeCorYes.SetCheck(m_Inter);

	::SetRect(&TempRect, R - 25, top, R - 12, top + 13);
	m_EyeCorNo.Create(_T(""), WS_CHILD | BS_CHECKBOX, TempRect, this, IDC_EYECOR_NO_ITEM);
	m_MemDC.FillSolidRect(&TempRect, BLACK);
	m_EyeCorNo.ShowWindow(SW_SHOW);
	m_EyeCorNo.SetCheck(!m_Inter);
	//Done

	//
	real_t AlphaRum = m_pWFExam->m_Header.m_AngleAlphaRUm*0.001;
	// 0: < 0.5; 1: 0.5~0.65; 2 >0.65.
	m_Alpha_Dec = (AlphaRum >= ::NewSettings.m_IOLSel_Alpha[0] ? 0 : AlphaRum >= ::NewSettings.m_IOLSel_Alpha[1] ? 1 : 2);
	m_vals[0] = AlphaRum;

	//BOOL CorHoa = (rms >= 0.2 ? TRUE : FALSE);
	m_TCHoa_Dec = m_TCHoa_Dec1 = (m_CorHoa ? 1 : 0);

	//> 1D ture  < 1D false;
	m_Cor_Ast_Dec = (m_pCTExam->m_rf_df_dp < ::NewSettings.m_IOLSel_Astig ? 0 : 1);
	if (m_pCTExam->m_rf_df_dp >0) m_vals[2] = m_pCTExam->m_rf_df_dp;
	else m_vals[2] = 0;

	CZernikeSurface Surface = m_pCTExam->m_WfSurface;//corneal
	Surface.ChangeRMaxUm(3000);
	real_t csa = Surface.GetCUm(12);

	// 0.14 ~ 0.20 ~ 0.25 ~0.30
	m_Cor_SphAb_Dec = (csa <= ::NewSettings.m_IOLSel_Sph_Abbe[0] ? 0 : csa <= ::NewSettings.m_IOLSel_Sph_Abbe[1] ? 1 : csa <= ::NewSettings.m_IOLSel_Sph_Abbe[2] ? 2 : csa <= ::NewSettings.m_IOLSel_Sph_Abbe[3] ? 3 : 4);
	m_vals[3] = csa;

	GetConRes();
	//

	CreateChildWnd();

	Repaint();

	ShowWindow(show);
}

//***************************************************************************************

void CWFCTIolWnd::RepaintMemDC()
{
	CSumWnd::RepaintMemDC();

	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	CMFont Font(intRound(4.5 * m_g), 400, "Arial");
	CString s = (m_pWFExam->m_Header.IsOD() ? "OD" : "OS");
	m_MemDC.WriteText(s, m_TitleRect[0], Font, white, 1);

	real_t psSize = m_pWFExam->m_ScanDiameter * 0.001;

	if (m_pWFExam->m_Image.m_pu_ok)
		psSize = m_pWFExam->m_Image.m_pu_r_mean_um * 0.002;

	if (m_pWFExam)
	{
		OUWFExamInfo(m_pWFExam, m_Rect[3]);
		OUCTExamInfo(m_pCTExam, m_InfoRect[0]);
		VisualComplaints(m_pWFExam, m_Rect[4], 8);
		VisualCTComplaints(m_pWFExam, m_ComplaintRect[0], 10);
	}

	//Corneal Spherical Aberration Function Display 

	CSADisplay(psSize, m_pCTExam, m_Rect[8]);

	//Control Panel  
	WFCTIolPan(m_Rect[9], m_TraceySet, m_CorHoa, m_Inter, m_Alpha_Dec, m_TCHoa_Dec, m_Cor_Ast_Dec, m_Cor_SphAb_Dec, m_vals);

	WFCTIolCon(m_Rect[10], m_Con_Res1, m_Con_Res2, m_Con_Res3, m_Con_Res4);
}

//***************************************************************************************

void CWFCTIolWnd::CreateChildWnd()
{
	for (int d = 0; d < 6; d++)
		SAFE_DELETE(m_pDispWnd[d]);

	//Set the setting of disp wnd
	if (m_pWndSettings[0].m_Type != TYPE_CTNM)
	{
		m_pWndSettings[0].m_Type = TYPE_CAXM;//Axial Map
	}

	if (m_pWndSettings[1].m_Type != TYPE_TWFM) m_pWndSettings[1].m_Type = TYPE_TRFM;

	m_pWndSettings[2].m_Type = TYPE_TLTR;

	for (int i = 0; i < 6; i++)
	{
		CWndSettings *pWndSettings = &m_pWndSettings[i];
		if (!(pWndSettings->m_Ltr == 0 || pWndSettings->m_Ltr == 1))
		{
			pWndSettings->m_Ltr = 0;
		}
		if (!(pWndSettings->m_LtrLine == 20 || pWndSettings->m_LtrLine == 25 || pWndSettings->m_LtrLine == 30 ||
			pWndSettings->m_LtrLine == 40 || pWndSettings->m_LtrLine == 50 || pWndSettings->m_LtrLine == 70 ||
			pWndSettings->m_LtrLine == 100 || pWndSettings->m_LtrLine == 200))
		{
			pWndSettings->m_LtrLine = 70;
		}
		if (!(pWndSettings->m_LtrOrientation == 0 || pWndSettings->m_LtrOrientation == 90 ||
			pWndSettings->m_LtrOrientation == 180 || pWndSettings->m_LtrOrientation == 270))
		{
			pWndSettings->m_LtrOrientation = 0;
		}
		m_pWndSettings[i].m_RMSIndividual = TRUE; //531 AAA
	}
	//done

	int  r_min_max = intRound(m_pWFExam->m_WfSurface.m_r_max_um);


	for (int d = 1; d < 6; d++)
	{
		// size
		m_pWndSettings[d].m_SizeSmall = TRUE;

		if (d != m_d)
		{
			// zone
			m_pWndSettings[d].m_ZoneRadiusUm = m_pWndSettings[m_d].m_ZoneRadiusUm;

			// mask
			m_pWndSettings[d].m_Mask = m_pWndSettings[m_d].m_Mask;
			m_pWndSettings[d].m_Piston = m_pWndSettings[m_d].m_Piston;
			m_pWndSettings[d].m_Tilt = m_pWndSettings[m_d].m_Tilt;
		}
	}
	//Done

	real_t ar_max_um = 5000;

	if (m_pWFExam) ar_max_um = m_pWFExam->m_WfSurface.m_r_max_um;
	ar_max_um = __min(ar_max_um, m_pWndSettings[1].m_ZoneRadiusUm);

	CZernikeSurface Surface;
	Surface = m_pWFExam->m_WfSurface;

	Surface &= m_pWndSettings[1].m_Mask;
	Surface.ComputePiston(m_pWndSettings[1].m_Piston);
	Surface.ComputeTilt(m_pWndSettings[1].m_Tilt);

	for (int d = 1; d < 3; d++)
	{
		m_Surfaces[d] = Surface;

		Surface.ChangeRMaxUm(ar_max_um);

		CWndSettings* pWndSettings = m_pWndSettings + d;

		switch (pWndSettings->m_Type)
		{
		case TYPE_TWFM:
		{
			//521
			//CEyeWnd* pEyeWnd = new CEyeWnd(m_Rect[t], this);
			CNewEyeWnd* pEyeWnd = new CNewEyeWnd(m_Rect[d], this, m_Scale);
			//521

			pEyeWnd->m_Type = TYPE_TWFM;
			pEyeWnd->m_SizeSmall = TRUE;
			pEyeWnd->m_Method3D = FALSE;
			pEyeWnd->m_LTLabel[0] = "Wavefront Map";
			pWndSettings->m_Mask.GetName(pEyeWnd->m_LTLabel[1]);

			pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), ar_max_um * 0.002);
			//Done
			//pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);

			pEyeWnd->m_MapShowMap = TRUE;
			pEyeWnd->m_MapShowNumbers = FALSE;
			pEyeWnd->m_MapShowEye = FALSE;
			pEyeWnd->m_MapShowPupil = FALSE;
			pEyeWnd->m_MapShowLimbus = FALSE;
			pEyeWnd->m_MapShowPoints = FALSE;
			pEyeWnd->m_MapTranslucent = FALSE;
			pEyeWnd->m_Unit = Unit::micron;
			pEyeWnd->m_Inc = pWndSettings->GetIncrement();
			CScale* pScale = pWndSettings->GetScale();
			pEyeWnd->m_NumColors = pScale->m_NumColors;
			for (int i = 0; i < pScale->m_NumColors; i++) {
				pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
			}
			pEyeWnd->m_pWFExam = m_pWFExam;
			CWFExam::CreateWfm2D(pEyeWnd->m_Map2D, Surface, 1.0 / pEyeWnd->m_y_px_um);
			pEyeWnd->CreateCentButtons();
			pEyeWnd->CreateZoomButtons();
			pEyeWnd->CreateZoneButtons();
			m_pDispWnd[d] = pEyeWnd;
		}
		break;

		case TYPE_TRFM:
		{
			//521
			//CEyeWnd* pEyeWnd = new CEyeWnd(m_Rect[t], this);
			CNewEyeWnd* pEyeWnd = new CNewEyeWnd(m_Rect[d], this, m_Scale);
			//521

			pEyeWnd->m_Type = TYPE_TRFM;
			pEyeWnd->m_SizeSmall = TRUE;
			pEyeWnd->m_Method3D = FALSE;
			pEyeWnd->m_LTLabel[0] = "Refraction Map";
			pWndSettings->m_Mask.GetName(pEyeWnd->m_LTLabel[1]);


			pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), ar_max_um * 0.002);
			//pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);

			pEyeWnd->m_MapShowMap = TRUE;
			pEyeWnd->m_MapShowNumbers = FALSE;
			pEyeWnd->m_MapShowEye = FALSE;
			pEyeWnd->m_MapShowPupil = FALSE;
			pEyeWnd->m_MapShowLimbus = FALSE;
			pEyeWnd->m_MapShowPoints = FALSE;
			pEyeWnd->m_MapTranslucent = FALSE;
			pEyeWnd->m_Unit = Unit::D;
			pEyeWnd->m_Inc = pWndSettings->GetIncrement();
			CScale* pScale = pWndSettings->GetScale();
			pEyeWnd->m_NumColors = pScale->m_NumColors;
			for (int i = 0; i < pScale->m_NumColors; i++) {
				pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
			}
			pEyeWnd->m_pWFExam = m_pWFExam;
			CWFExam::CreateRfm2D(pEyeWnd->m_Map2D, Surface, 1.0 / pEyeWnd->m_y_px_um);
			pEyeWnd->CreateCentButtons();
			pEyeWnd->CreateZoomButtons();
			pEyeWnd->CreateZoneButtons();
			m_pDispWnd[d] = pEyeWnd;
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

			pLtrWnd->m_LTLabel[2].Format(_T("%.2f mm"), ar_max_um * 0.002);

			pLtrWnd->CreateZoomButtons();
			pLtrWnd->CreateZoneButtons();
			m_pDispWnd[d] = pLtrWnd;
		}
		break;
		}
	}

	//Show the m_pDispWnd[0] and m_pDispWnd[3]  
	real_t r_max_um = 5000;

	m_Surfaces[0] = m_pCTExam->m_WfSurface;
	CZernikeSurface WFSurface = m_pWFExam->m_WfSurface;

	r_max_um = __min(r_max_um, m_Surfaces[0].m_r_max_um);

	if (m_pWndSettings[1].m_ZoneRadiusUm != 0)
		r_max_um = __min(r_max_um, m_pWndSettings[1].m_ZoneRadiusUm);

	if (m_d == 1 || m_d == 2)
	{
		r_max_um = __min(r_max_um, m_pWndSettings[m_d].m_ZoneRadiusUm);
	}

	m_Surfaces[0].ChangeRMaxUm(r_max_um);
	m_Surfaces[0] &= m_pWndSettings[1].m_Mask;
	m_Surfaces[0].ComputePiston(m_pWndSettings[0].m_Piston);
	m_Surfaces[0].ComputeTilt(m_pWndSettings[0].m_Tilt);

	//Dis 0 
	CCTExam* pCTExam = m_pCTExam;

	switch (m_pWndSettings[0].m_Type)
	{
	case TYPE_CAXM:
	case TYPE_CTNM:
	{
		int MapUnit = (m_pWndSettings[0].m_Type == TYPE_CRFM) || (m_pWndSettings[0].m_MapUnit == DIOPTERS) ? DIOPTERS : MILLIMETERS;
		CEyeWnd* pEyeWnd = new CEyeWnd(m_Rect[0], this);
		pEyeWnd->m_Type = m_pWndSettings[0].m_Type;
		pEyeWnd->m_SizeSmall = m_pWndSettings[0].m_SizeSmall;
		pEyeWnd->m_Method3D = m_pWndSettings[0].m_Method3D;

		if (m_pWndSettings[0].m_Type == TYPE_CAXM) pEyeWnd->m_LTLabel[0] = "Axial Map";
		else if (m_pWndSettings[0].m_Type == TYPE_CTNM) pEyeWnd->m_LTLabel[0] = "Local ROC Map";
		else pEyeWnd->m_LTLabel[0] = "Refractive Map";

		pEyeWnd->m_MapShowEye = m_pWndSettings[0].m_MapShowEye;
		pEyeWnd->m_MapShowMap = TRUE;//m_pWndSettings[0].m_MapShowMap;
		pEyeWnd->m_MapShowNumbers = m_pWndSettings[0].m_MapShowNumbers;
		pEyeWnd->m_MapShowPupil = m_pWndSettings[0].m_MapShowPupil;
		pEyeWnd->m_MapShowLimbus = m_pWndSettings[0].m_MapShowLimbus;
		pEyeWnd->m_MapShowSolidSurface = m_pWndSettings[0].m_MapShowSolidSurface;
		pEyeWnd->m_MapShowWireMesh = m_pWndSettings[0].m_MapShowWireMesh;
		pEyeWnd->m_MapTranslucent = m_pWndSettings[0].m_MapTranslucent;
		pEyeWnd->m_MapShowKeratometry = TRUE;// m_pWndSettings[0].m_MapShowKeratometry; // 6.3.0 bug fix
		pEyeWnd->m_MapShowSimKAxes = m_pWndSettings[0].m_MapShowSimKAxes;
		pEyeWnd->m_Unit = MapUnit == DIOPTERS ? Unit::D : Unit::mm;
		pEyeWnd->m_Inc = m_pWndSettings[0].GetIncrement();
		CScale* pScale = m_pWndSettings[0].GetScale();
		pEyeWnd->m_NumColors = pScale->m_NumColors;
		for (int i = 0; i < pScale->m_NumColors; i++) {
			pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
		}
		pEyeWnd->m_pCTExam = pCTExam;
		if (m_pWndSettings[0].m_Method3D)
		{
			pCTExam->CreateDpm3D(pEyeWnd->m_Map3D, m_pWndSettings[0].m_Type, MapUnit);
		}
		else
		{
			pCTExam->CreateDpm2D(pEyeWnd->m_Map2D, m_pWndSettings[0].m_Type, MapUnit, 1.0 / pEyeWnd->m_y_px_um);
			pEyeWnd->m_Circles.Create(1);
			pEyeWnd->m_Circles[0].m_x_um = 0.0;
			pEyeWnd->m_Circles[0].m_y_um = 0.0;
			pEyeWnd->m_Circles[0].m_r_um = ar_max_um;
			pEyeWnd->m_Circles[0].m_color = BLACK;
		}
		pEyeWnd->CreateCentButtons();
		pEyeWnd->CreateZoomButtons();
		m_pDispWnd[0] = pEyeWnd;
	}
	break;
	}
	//Done

	//Scale range of windows 0 and 3
	{
		CWndSettings* pWndSettings = m_pWndSettings;
		CScale* pScale = pWndSettings->GetScale();
		real_t Inc = pWndSettings->GetIncrement();
		real_t Cent = 0.0;
		real_t Step;

		// middle value
		if (m_pWndSettings[0].m_Type == TYPE_CAXM || m_pWndSettings[0].m_Type == TYPE_CTNM || m_pWndSettings[0].m_Type == TYPE_CRFM)
		{
			int MapUnit = (m_pWndSettings[0].m_Type == TYPE_CRFM) || (m_pWndSettings[0].m_MapUnit == DIOPTERS) ? DIOPTERS : MILLIMETERS;
			if (pScale->m_CentAuto)
			{
				if (MapUnit == DIOPTERS)
				{
					Cent = m_pCTExam->m_ax0_dp;
				}
				else
				{
					Cent = 0.001 * m_pCTExam->m_ax0_um;
				}
			}
			else
			{
				Cent = pScale->m_Cent;
			}
		}
		else
		{
			Cent = 0.0;
		}
		Cent = RealRound(Cent, Inc);

		// step value
		if (pScale->m_StepAuto)
		{
			real_t Span = 0.0;
			for (int e = 0; e < 1; e++)
			{
				CEyeWnd* pEyeWnd = (CEyeWnd*)m_pDispWnd[e * 3];

				if (pEyeWnd)
				{
					real_t Min = m_pWndSettings[0].m_Method3D ? pEyeWnd->m_Map3D.m_min : pEyeWnd->m_Map2D.m_min;
					real_t Max = m_pWndSettings[0].m_Method3D ? pEyeWnd->m_Map3D.m_max : pEyeWnd->m_Map2D.m_max;
					real_t Span = 2.0 * __max(fabs(Min - Cent), fabs(Max - Cent));
				}
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
		real_t StepMax = m_pWndSettings[0].GetMaxStep();
		if (Step > StepMax) Step = StepMax;

		for (int e = 0; e < 1; e++)
		{
			CEyeWnd* pEyeWnd = (CEyeWnd*)m_pDispWnd[e * 3];
			if (pEyeWnd)
			{
				pEyeWnd->m_Cent = Cent;
				pEyeWnd->m_Step = Step;
			}
		}
	}
	//Done

	// corneal wavefront surface
	m_Surfaces[4] = m_pCTExam->m_WfSurface;
	// entire wavefront surface
	m_Surfaces[5] = m_pWFExam->m_WfSurface;

	m_Surfaces[4].ChangeRMaxUm(ar_max_um);
	m_Surfaces[4] &= m_pWndSettings[1].m_Mask;
	m_Surfaces[4].ComputePiston(m_pWndSettings[0].m_Piston);
	m_Surfaces[4].ComputeTilt(m_pWndSettings[0].m_Tilt);

	m_Surfaces[5].ChangeRMaxUm(ar_max_um);
	m_Surfaces[5] &= m_pWndSettings[1].m_Mask;
	m_Surfaces[5].ComputePiston(m_pWndSettings[0].m_Piston);
	m_Surfaces[5].ComputeTilt(m_pWndSettings[0].m_Tilt);

	// internal optics wavefront surface
	m_Surfaces[3] = m_Surfaces[5];
	m_Surfaces[3] -= m_Surfaces[4];

	CString s[3] = { "Internal Optics", "Cornea", "Total Eye" };
	CString s2[3] = { "INTERNAL", "CORNEA", "TOTAL EYE" };//530


	if (m_pWndSettings[3].m_Type != TYPE_IRMS
		&& m_pWndSettings[3].m_Type != TYPE_IMTF
		&& m_pWndSettings[3].m_Type != TYPE_ILTR)
		m_pWndSettings[3].m_Type = TYPE_IRMS;

	if (m_pWndSettings[4].m_Type == TYPE_IRMS) m_pWndSettings[4].m_Type = TYPE_CRMS;
	if (m_pWndSettings[4].m_Type == TYPE_IMTF) m_pWndSettings[4].m_Type = TYPE_CMTF;
	if (m_pWndSettings[4].m_Type == TYPE_ILTR) m_pWndSettings[4].m_Type = TYPE_CLTR;

	if (m_pWndSettings[5].m_Type == TYPE_IRMS) m_pWndSettings[5].m_Type = TYPE_TRMS;
	if (m_pWndSettings[5].m_Type == TYPE_IMTF) m_pWndSettings[5].m_Type = TYPE_TMTF;
	if (m_pWndSettings[5].m_Type == TYPE_ILTR) m_pWndSettings[5].m_Type = TYPE_TLTR;

	if (m_pWndSettings[4].m_Type != TYPE_CRMS
		&& m_pWndSettings[4].m_Type != TYPE_CMTF
		&& m_pWndSettings[4].m_Type != TYPE_CLTR)
		m_pWndSettings[4].m_Type = TYPE_CRMS;

	if (m_pWndSettings[5].m_Type != TYPE_TRMS
		&& m_pWndSettings[5].m_Type != TYPE_TMTF
		&& m_pWndSettings[5].m_Type != TYPE_TLTR)
		m_pWndSettings[5].m_Type = TYPE_TRMS;

	//531
	if (m_d == 3)
	{
		if (m_pWndSettings[3].m_Type == TYPE_IRMS)
		{
			m_pWndSettings[4].m_Type = TYPE_CRMS;
			m_pWndSettings[5].m_Type = TYPE_TRMS;
		}
		else if (m_pWndSettings[3].m_Type == TYPE_IMTF)
		{
			m_pWndSettings[4].m_Type = TYPE_CMTF;
			m_pWndSettings[5].m_Type = TYPE_TMTF;
		}
		else if (m_pWndSettings[3].m_Type == TYPE_ILTR)
		{
			m_pWndSettings[4].m_Type = TYPE_CLTR;
			m_pWndSettings[5].m_Type = TYPE_TLTR;
		}
	}
	else if (m_d == 4)
	{
		if (m_pWndSettings[4].m_Type == TYPE_CRMS)
		{
			m_pWndSettings[3].m_Type = TYPE_IRMS;
			m_pWndSettings[5].m_Type = TYPE_TRMS;
		}
		else if (m_pWndSettings[4].m_Type == TYPE_CMTF)
		{
			m_pWndSettings[3].m_Type = TYPE_IMTF;
			m_pWndSettings[5].m_Type = TYPE_TMTF;
		}
		else if (m_pWndSettings[4].m_Type == TYPE_CLTR)
		{
			m_pWndSettings[3].m_Type = TYPE_ILTR;
			m_pWndSettings[5].m_Type = TYPE_TLTR;
		}
	}
	else if (m_d == 5)
	{
		if (m_pWndSettings[5].m_Type == TYPE_TRMS)
		{
			m_pWndSettings[3].m_Type = TYPE_IRMS;
			m_pWndSettings[4].m_Type = TYPE_CRMS;
		}
		else if (m_pWndSettings[5].m_Type == TYPE_TMTF)
		{
			m_pWndSettings[3].m_Type = TYPE_IMTF;
			m_pWndSettings[4].m_Type = TYPE_CMTF;
		}
		else if (m_pWndSettings[5].m_Type == TYPE_TLTR)
		{
			m_pWndSettings[3].m_Type = TYPE_ILTR;
			m_pWndSettings[4].m_Type = TYPE_CLTR;
		}
	}
	//531 Done

	//531
	int k = (m_d < 3 || m_d > 5) ? 0 : m_d;
	for (int d = 3; d < 6; d++)
	{
		if (d != k)
		{
			m_pWndSettings[d].m_ZoneRadiusUm = m_pWndSettings[k].m_ZoneRadiusUm;
			m_pWndSettings[d].m_Mask = m_pWndSettings[k].m_Mask;
			m_pWndSettings[d].m_Piston = m_pWndSettings[k].m_Piston;
			m_pWndSettings[d].m_Tilt = m_pWndSettings[k].m_Tilt;
			m_pWndSettings[d].m_MapShowEye = m_pWndSettings[k].m_MapShowEye;
			m_pWndSettings[d].m_MapShowMap = m_pWndSettings[k].m_MapShowMap;
			m_pWndSettings[d].m_MapShowNumbers = m_pWndSettings[k].m_MapShowNumbers;
			m_pWndSettings[d].m_MapShowPupil = m_pWndSettings[k].m_MapShowPupil;
			m_pWndSettings[d].m_MapShowLimbus = m_pWndSettings[k].m_MapShowLimbus;
			m_pWndSettings[d].m_MapShowSolidSurface = m_pWndSettings[k].m_MapShowSolidSurface;
			m_pWndSettings[d].m_MapShowWireMesh = m_pWndSettings[k].m_MapShowWireMesh;
			m_pWndSettings[d].m_MapShowAstigmatismAxes = m_pWndSettings[k].m_MapShowAstigmatismAxes;
			m_pWndSettings[d].m_MapTranslucent = m_pWndSettings[k].m_MapTranslucent;
			*(m_pWndSettings[d].GetScale()) = *(m_pWndSettings[k].GetScale());
			m_pWndSettings[d].m_ShowEEF = m_pWndSettings[k].m_ShowEEF;
			m_pWndSettings[d].m_EEFType = m_pWndSettings[k].m_EEFType;
			m_pWndSettings[d].m_MTFAverage = m_pWndSettings[k].m_MTFAverage;
			m_pWndSettings[d].m_MTFAngle = m_pWndSettings[k].m_MTFAngle;
			m_pWndSettings[d].m_LtrLine = m_pWndSettings[k].m_LtrLine;
		}
	}
	//531

	for (int d = 3; d < 6; d++)
	{
		CWndSettings* pWndSettings = m_pWndSettings + d;
		RECT Rect = (d == 3 ? m_Rect[6] : d == 4 ? m_Rect[7] : m_Rect[5]);
		switch (pWndSettings->m_Type) {
		case TYPE_IMTF:
		case TYPE_CMTF:
		case TYPE_TMTF:
		{
			CMTFWnd* pMTFWnd = new CMTFWnd(Rect, this);
			CWFExam::CreateMtf(m_Surfaces[d], pMTFWnd->m_Mtf);
			pMTFWnd->m_Type = pWndSettings->m_Type;
			pMTFWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
			pMTFWnd->m_Method3D = pWndSettings->m_Method3D;

			pMTFWnd->m_ShowDys = FALSE;//531

			pMTFWnd->m_LTLabel[0] = s2[d - 3] + "- Modulation Transfer Function";
			pWndSettings->m_Mask.GetName(pMTFWnd->m_LTLabel[1]);
			pMTFWnd->m_LTLabel[2].Format(_T("%.2f mm"), ar_max_um * 0.002);

			pMTFWnd->m_SIMLTLabel[3] = "iol sel";//531

			pMTFWnd->m_Average = pWndSettings->m_MTFAverage;
			pMTFWnd->m_Angle = pWndSettings->m_MTFAngle;
			if (!pWndSettings->m_Method3D && !pWndSettings->m_MTFAverage) {
				pMTFWnd->CreateSlider();
			}
			pMTFWnd->CreateZoneButtons();
			m_pDispWnd[d] = pMTFWnd;
		}
		break;

		case TYPE_IRMS:
		case TYPE_CRMS:
		case TYPE_TRMS:
		{
			//530
			CRMSWnd* pRMSWnd = new CRMSWnd(Rect, this);
			pRMSWnd->m_SimpliedRMS = TRUE;

			pRMSWnd->m_SIMLTLabel[0] = s2[d - 3];
			pWndSettings->m_Mask.GetName(pRMSWnd->m_SIMLTLabel[1]);

			CString s;
			s.Format(_T("%.2f mm"), ar_max_um * 0.002);

			pRMSWnd->m_SIMLTLabel[1] = " - RMS " + pRMSWnd->m_SIMLTLabel[1] + "   " + s;
			//pRMSWnd->m_SIMLTLabel[2].Format(_T("%.2f mm"), ar_max_um * 0.002);		
			pRMSWnd->m_SIMLTLabel[3] = "iol sel";

			pRMSWnd->m_Type = pWndSettings->m_Type;
			pRMSWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
			pRMSWnd->m_Method3D = pWndSettings->m_Method3D;

			pRMSWnd->m_LTLabel[0] = "Root Mean Square - " + s[d - 3];
			pWndSettings->m_Mask.GetName(pRMSWnd->m_LTLabel[1]);
			pRMSWnd->m_LTLabel[2].Format(_T("%.2f mm"), ar_max_um * 0.002);
			pRMSWnd->m_Surface = m_Surfaces[d];
			pRMSWnd->m_Individual = pWndSettings->m_RMSIndividual;
			pRMSWnd->CreateZoomButtons2();
			pRMSWnd->CreateZoneButtons2();
			m_pDispWnd[d] = pRMSWnd;
		}
		break;

		case TYPE_ILTR:
		case TYPE_CLTR:
		case TYPE_TLTR:
		{
			CLtrWnd* pLtrWnd = new CLtrWnd(Rect, this);
			pLtrWnd->m_DysVal = -1;//531
			CWFExam::CreateLtr(m_Surfaces[d], pLtrWnd->m_Ltr, pWndSettings->m_Ltr, pWndSettings->m_LtrLine, pWndSettings->m_LtrOrientation);
			pLtrWnd->m_Type = pWndSettings->m_Type;
			pLtrWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
			pLtrWnd->m_Method3D = pWndSettings->m_Method3D;
			//pLtrWnd->m_LTLabel[0].Format(_T("%s - Letter 20 / %i", s2[d-3], pWndSettings->m_LtrLine);
			pLtrWnd->m_LTLabel[0].Format(_T("%s - Letter"), s2[d - 3]);//531
			pWndSettings->m_Mask.GetName(pLtrWnd->m_LTLabel[1]);
			pLtrWnd->m_LTLabel[2].Format(_T("%.2f mm"), ar_max_um * 0.002);

			pLtrWnd->m_SIMLTLabel[3] = "iol sel";//531

			pLtrWnd->CreateZoomButtons();
			pLtrWnd->CreateZoneButtons();
			m_pDispWnd[d] = pLtrWnd;
		}
		break;
		}
	}
	//Done

	//Scale range of windows 1 and 4
	{
		CWndSettings* pWndSettings = m_pWndSettings + 1;
		CScale* pScale = pWndSettings->GetScale();
		real_t Inc = pWndSettings->GetIncrement();
		real_t Cent = 0.0;
		real_t Step;
		if (pScale->m_StepAuto)
		{
			real_t Span = 0.0;
			for (int e = 0; e < 1; e++)
			{
				CNewEyeWnd* pEyeWnd = (CNewEyeWnd*)m_pDispWnd[e * 3 + 1];
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
		for (int e = 0; e < 1; e++) {
			CNewEyeWnd* pEyeWnd = (CNewEyeWnd*)m_pDispWnd[e * 3 + 1];
			if (pEyeWnd) {
				pEyeWnd->m_Cent = Cent;
				pEyeWnd->m_Step = Step;
			}
		}
	}
	//Done

	if (m_pWndSettings[3].m_Type == TYPE_IRMS)
	{
		for (int d = 3; d < 6; d++)
		{
			((CRMSWnd*)m_pDispWnd[d])->m_Max = 1;
			((CRMSWnd*)m_pDispWnd[d])->m_ZoomInButton.SetEnabled(FALSE);
			((CRMSWnd*)m_pDispWnd[d])->m_ZoomInButton.Repaint();
		}

		/*int MaxMax = 1;
		for(int d = 3; d < 6; d++)
		{
		CRMSWnd* pRMSWnd = (CRMSWnd*)m_pDispWnd[d];
		real t = pRMSWnd->m_Surface.ComputeRms();
		int Max = (int)t;
		if (t > Max + 0.001) Max++;
		MaxMax = __max(MaxMax, Max);
		}

		for(int d = 3; d < 6; d++)
		{
		((CRMSWnd*)m_pDispWnd[d])->m_Max = MaxMax;
		if(MaxMax == 1)
		{
		((CRMSWnd*)m_pDispWnd[d])->m_ZoomInButton.SetEnabled(FALSE);
		((CRMSWnd*)m_pDispWnd[d])->m_ZoomInButton.Repaint();
		}
		}*/
	}//531



	 //521 Let the zoom in/out button to be gray if minimum or maximum scan size
	real_t maxR = TempSettings.Com_r_max_um;
	for (int i = 1; i < 6; i++)
	{
		BOOL Check = TRUE;
		/*  if(m_pWndSettings[3].m_Type == TYPE_IRMS && i == 3) Check = FALSE;
		if(m_pWndSettings[4].m_Type == TYPE_CRMS && i == 4) Check = FALSE;
		if(m_pWndSettings[5].m_Type == TYPE_TRMS && i == 5) Check = FALSE;*/

		if (Check)
		{
			m_pDispWnd[i]->m_IncZoneButton.SetEnabled(TRUE);
			m_pDispWnd[i]->m_IncZoneButton.Repaint();

			m_pDispWnd[i]->m_DecZoneButton.SetEnabled(TRUE);
			m_pDispWnd[i]->m_DecZoneButton.Repaint();

			if (m_pWndSettings[i].m_ZoneRadiusUm >= (int)maxR)
			{
				m_pDispWnd[i]->m_IncZoneButton.SetEnabled(FALSE);
				m_pDispWnd[i]->m_IncZoneButton.Repaint();
			}

			if (m_pWndSettings[i].m_ZoneRadiusUm == 500)
			{
				m_pDispWnd[i]->m_DecZoneButton.SetEnabled(FALSE);
				m_pDispWnd[i]->m_DecZoneButton.Repaint();
			}
		}
	}
	//521  
}

//***************************************************************************************

void CWFCTIolWnd::CreateMenu()
{
	if (m_d == 0)
	{
		CScale* pScale = m_pWndSettings[m_d].GetScale();
		CXPMenu* pSubMenu;

		//m_pMenu->AppendItem(IDC_TYPE_CEYE_ITEM, pWndSettings->m_Type == TYPE_CEYE, FALSE, "Eye Image", "");
		m_pMenu->AppendItem(IDC_TYPE_CAXM_ITEM, m_pWndSettings[m_d].m_Type == TYPE_CAXM, FALSE, "Axial Map", "");
		m_pMenu->AppendItem(IDC_TYPE_CTNM_ITEM, m_pWndSettings[m_d].m_Type == TYPE_CTNM, FALSE, "Local ROC Map", "");

		m_pMenu->AppendSeparator();

		m_pMenu->AppendItem(IDC_SHOW_MAP_EYE_ITEM, FALSE, m_pWndSettings[m_d].m_MapShowEye, "Show eye image", "");
		m_pMenu->AppendItem(IDC_SHOW_MAP_PUPIL_ITEM, FALSE, m_pWndSettings[m_d].m_MapShowPupil, "Show pupil", "");
		m_pMenu->AppendItem(IDC_MAP_TRANSLUCENT_ITEM, FALSE, m_pWndSettings[m_d].m_MapTranslucent, "Translucent map", "");

		m_pMenu->AppendSeparator();

		pSubMenu = m_pMenu->AppendSubmenu("Map middle value", "");
		pSubMenu->AppendItem(IDC_MAP_CENT_USER_ITEM, !pScale->m_CentAuto, FALSE, "Fixed...", "");
		pSubMenu->AppendItem(IDC_MAP_CENT_AUTO_ITEM, pScale->m_CentAuto, FALSE, "Auto", "");

		CreateSubmenuMapSteps();
		CreateSubmenuMapColors();
	}
	else if (m_d == 1 || m_d == 2)
	{
		if (m_d == 1) {
			m_pMenu->AppendItem(IDC_TYPE_TWFM_ITEM, m_pWndSettings[m_d].m_Type == TYPE_TWFM, FALSE, "Wavefront Map", "");
			m_pMenu->AppendItem(IDC_TYPE_TRFM_ITEM, m_pWndSettings[m_d].m_Type == TYPE_TRFM, FALSE, "Refraction Map", "");
			m_pMenu->AppendSeparator();
		}

		CreateSubmenuZone(2);
		CreateSubmenuMask();

		if (m_d == 1) {

			m_pMenu->AppendSeparator();

			CreateSubmenuMapSteps();
			CreateSubmenuMapColors();
		}

		else if (m_d == 2) {

			m_pMenu->AppendSeparator();

			CreateSubmenuLtr();
		}
	}
	else
	{
		m_pMenu->AppendItem(IDC_TYPE_IMTF_ITEM, m_pWndSettings[m_d].m_Type == TYPE_IMTF || m_pWndSettings[m_d].m_Type == TYPE_CMTF || m_pWndSettings[m_d].m_Type == TYPE_TMTF, FALSE, "Modulation Transfer Function", "");
		m_pMenu->AppendItem(IDC_TYPE_ILTR_ITEM, m_pWndSettings[m_d].m_Type == TYPE_ILTR || m_pWndSettings[m_d].m_Type == TYPE_CLTR || m_pWndSettings[m_d].m_Type == TYPE_TLTR, FALSE, "Visual Acuity Chart", "");
		m_pMenu->AppendItem(IDC_TYPE_IRMS_ITEM, m_pWndSettings[m_d].m_Type == TYPE_IRMS || m_pWndSettings[m_d].m_Type == TYPE_CRMS || m_pWndSettings[m_d].m_Type == TYPE_TRMS, FALSE, "Root Mean Square", "");
		m_pMenu->AppendSeparator();


		if (
			m_pWndSettings[m_d].m_Type == TYPE_IMTF || m_pWndSettings[m_d].m_Type == TYPE_CMTF || m_pWndSettings[m_d].m_Type == TYPE_TMTF ||
			m_pWndSettings[m_d].m_Type == TYPE_ILTR || m_pWndSettings[m_d].m_Type == TYPE_CLTR || m_pWndSettings[m_d].m_Type == TYPE_TLTR ||
			m_pWndSettings[m_d].m_Type == TYPE_IRMS || m_pWndSettings[m_d].m_Type == TYPE_CRMS || m_pWndSettings[m_d].m_Type == TYPE_TRMS
			&& m_pWndSettings[m_d].m_RMSIndividual)
		{
			m_pMenu->AppendItem(IDC_METHOD_3D_ITEM, FALSE, m_pWndSettings[m_d].m_Method3D, "3D", "");
			m_pMenu->AppendSeparator();
		}

		// zone & mask
		CreateSubmenuZone(2);
		CreateSubmenuMask();
		//
		if (m_pWndSettings[m_d].m_Type == TYPE_IMTF || m_pWndSettings[m_d].m_Type == TYPE_CMTF || m_pWndSettings[m_d].m_Type == TYPE_TMTF)
		{
			if (!m_pWndSettings[m_d].m_Method3D)
			{
				m_pMenu->AppendSeparator();
				m_pMenu->AppendItem(IDC_MTF_AVG_ITEM, FALSE, m_pWndSettings[m_d].m_MTFAverage, "Radial average curve", "");
			}
		}
		else if (m_pWndSettings[m_d].m_Type == TYPE_ILTR || m_pWndSettings[m_d].m_Type == TYPE_CLTR || m_pWndSettings[m_d].m_Type == TYPE_TLTR) {
			m_pMenu->AppendSeparator();
			CreateSubmenuLtr();
		}
		else if (m_pWndSettings[m_d].m_Type == TYPE_IRMS || m_pWndSettings[m_d].m_Type == TYPE_CRMS || m_pWndSettings[m_d].m_Type == TYPE_TRMS) {
			// if (!m_pWndSettings[m_d].m_Method3D) {
			//m_pMenu->AppendSeparator();
			//m_pMenu->AppendItem(IDC_RMS_CMB_ITEM, FALSE, !m_pWndSettings[m_d].m_RMSIndividual, "Combined", "");
			// }//531
		}

		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_SAVE_TXT_ITEM, FALSE, FALSE, "Save Zernike coefficients into file", "");
	}

	//521
	if (!SameOU())
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
	}
	//521
}

//***************************************************************************************

LRESULT CWFCTIolWnd::OnChangeCentMsg(WPARAM wParam, LPARAM lParam)
{
	//if (!m_pDispWnd[1] || !m_pDispWnd[4]) return 0;

	if (wParam == (WPARAM)m_pDispWnd[0]) {
		((CEyeWnd*)m_pDispWnd[3])->m_Cent = ((CEyeWnd*)m_pDispWnd[0])->m_Cent;
		m_pDispWnd[3]->Repaint();
	}
	else if (wParam == (WPARAM)m_pDispWnd[3]) {
		((CEyeWnd*)m_pDispWnd[0])->m_Cent = ((CEyeWnd*)m_pDispWnd[3])->m_Cent;
		m_pDispWnd[0]->Repaint();
	}

	if (wParam == (WPARAM)m_pDispWnd[1]) {
		((CEyeWnd*)m_pDispWnd[4])->m_Cent = ((CEyeWnd*)m_pDispWnd[1])->m_Cent;
		m_pDispWnd[4]->Repaint();
	}
	else if (wParam == (WPARAM)m_pDispWnd[4]) {
		((CEyeWnd*)m_pDispWnd[1])->m_Cent = ((CEyeWnd*)m_pDispWnd[4])->m_Cent;
		m_pDispWnd[1]->Repaint();
	}

	return 0;
}

//***************************************************************************************

LRESULT CWFCTIolWnd::OnChangeStepMsg(WPARAM wParam, LPARAM lParam)
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

//521
int CWFCTIolWnd::GetGridScale(real_t r_um)
{
	r_um = r_um + 1000;

	return int(r_um / 1000) * 1000;
}
//521

//***************************************************************************************
//521
void CWFCTIolWnd::OnShowDefaultItemClicked()
{
	CBusyCursor Cursor;

	m_pWndSettings[m_d].m_LtrDistanceType = 0;//530

	if (m_d == 0)
	{
		m_pWndSettings[0].m_Type = 31;//Axial map       TYPE_CAXM
		m_pWndSettings[0].m_Method3D = 0;
		m_pWndSettings[0].m_MapUnit = DIOPTERS;
		m_pWndSettings[0].m_MapShowMap = 1;
		m_pWndSettings[0].m_MapShowEye = 1;
		m_pWndSettings[0].m_MapShowNumbers = 0;
		m_pWndSettings[0].m_MapShowPupil = 0;
		m_pWndSettings[0].m_MapShowLimbus = 0;
		m_pWndSettings[0].m_MapShowCornealVertex = 0;
		m_pWndSettings[0].m_MapTranslucent = 0;
		m_pWndSettings[0].m_MapShowKeratometry = 0;

		CScale* pScale6 = m_pWndSettings[0].GetScale();
		pScale6->m_Step = 0.5;
		m_pWndSettings[0].SetDefaultColors(1);

		m_d = 0;
	}

	else if (m_d == 1)
	{
		m_pWndSettings[1].m_Type = 11;//Refraction Map  TYPE_TRFM
		m_pWndSettings[1].m_ZoneRadiusUm = 5000;

		for (int i = 0; i <= 5; i++)		m_pWndSettings[i].m_Mask.SetType(MASK_HO_TOTAL);//601

		m_pWndSettings[1].m_Piston = FALSE;
		m_pWndSettings[1].m_Tilt = FALSE;
		m_pWndSettings[1].SetDefaultColors(1);

		m_d = 1;
	}

	else if (m_d == 2)
	{
		for (int i = 0; i <= 5; i++)		m_pWndSettings[i].m_Mask.SetType(MASK_HO_TOTAL);//601
		m_pWndSettings[2].m_Ltr = 0;
		m_pWndSettings[2].m_LtrLine = 70;
		m_pWndSettings[2].m_LtrOrientation = 0;

		m_d = 2;
	}

	else
	{
		if (m_d == 3) m_pWndSettings[3].m_Type = TYPE_IRMS;
		if (m_d == 4) m_pWndSettings[4].m_Type = TYPE_CRMS;
		if (m_d == 5) m_pWndSettings[5].m_Type = TYPE_TRMS;

		m_pWndSettings[m_d].m_RMSIndividual = TRUE;

		m_pWndSettings[m_d].m_Method3D = FALSE;
		m_pWndSettings[m_d].m_SizeSmall = TRUE;
		m_pWndSettings[m_d].m_Mask.SetType(MASK_HO_TOTAL);	//601
		m_pWndSettings[m_d].m_Piston = FALSE;
		m_pWndSettings[m_d].m_Tilt = FALSE;
	}

	CreateChildWnd();

	Repaint();
}

//***************************************************************************************

BOOL CWFCTIolWnd::SameOU()
{
	//CWndSettings* pWndSettings = GetWndSettings();

	if (m_d == 0)
	{
		if (m_pWndSettings[m_d].m_Type != 31)    return FALSE;
		if (m_pWndSettings[m_d].m_Method3D != 0)     return FALSE;
		if (m_pWndSettings[m_d].m_MapUnit != DIOPTERS) return FALSE;

		if (m_pWndSettings[m_d].m_MapShowMap != 1)     return FALSE;
		if (m_pWndSettings[m_d].m_MapShowEye != 1)     return FALSE;
		if (m_pWndSettings[m_d].m_MapShowPupil != 0)     return FALSE;
		if (m_pWndSettings[m_d].m_MapTranslucent != 0)     return FALSE;

		CScale* pScale = m_pWndSettings[m_d].GetScale();
		if (pScale->m_Step != 0.5)                         return FALSE;
		if (!m_pWndSettings[m_d].AreDefaultColors(1))            return FALSE;
	}

	else if (m_d == 1)
	{
		if (m_pWndSettings[m_d].m_Type != 11)        return FALSE;
		if (m_pWndSettings[m_d].m_Mask.GetType() != MASK_HO_TOTAL)   return FALSE;	//601

		if (!m_pWndSettings[m_d].AreDefaultColors(1))            return FALSE;
	}

	else if (m_d == 2)
	{
		if (m_pWndSettings[m_d].m_Mask.GetType() != MASK_HO_TOTAL)   return FALSE;	//601
		if (m_pWndSettings[m_d].m_Ltr != 0)         return FALSE;
		if (m_pWndSettings[m_d].m_LtrLine != 70)        return FALSE;
		if (m_pWndSettings[m_d].m_LtrOrientation != 0)         return FALSE;
	}
	else
	{
		if (m_pWndSettings[3].m_Type != TYPE_IRMS && m_d == 3) return FALSE;
		if (m_pWndSettings[4].m_Type != TYPE_CRMS && m_d == 4) return FALSE;
		if (m_pWndSettings[5].m_Type != TYPE_TRMS && m_d == 5) return FALSE;

		// if (m_pWndSettings[m_d].m_RMSIndividual     != TRUE)      return FALSE;	//531 AAA

		if (m_pWndSettings[m_d].m_Method3D != 0)         return FALSE;
		if (m_pWndSettings[m_d].m_SizeSmall != 1)         return FALSE;
		if (m_pWndSettings[m_d].m_Mask.GetType() != MASK_HO_TOTAL)   return FALSE;	//601
	}

	return TRUE;
}
//***************************************************************************************

void CWFCTIolWnd::OnTraceYesClicked()
{
	BOOL Yes = m_TraceyRecYes.GetCheck();
	if (!Yes)
	{
		m_TraceyRecYes.SetCheck(TRUE);
		m_TraceyRecNo.SetCheck(FALSE);

		GetConRes();
		Repaint();
	}
}
//***************************************************************************************

void CWFCTIolWnd::OnTraceNoClicked()
{
	BOOL No = m_TraceyRecNo.GetCheck();
	if (!No)
	{
		m_TraceyRecNo.SetCheck(TRUE);
		m_TraceyRecYes.SetCheck(FALSE);

		GetConRes();
		Repaint();
	}
}

//***************************************************************************************


void CWFCTIolWnd::OnHoaYesClicked()
{
	BOOL Yes = m_HOAYes.GetCheck();
	if (!Yes)
	{
		m_HOAYes.SetCheck(TRUE);
		m_HOANo.SetCheck(FALSE);

		m_TCHoa_Dec1 = TRUE;
		GetConRes();
		Repaint();
	}
}
//***************************************************************************************

void CWFCTIolWnd::OnHoaNoClicked()
{
	BOOL No = m_HOANo.GetCheck();
	if (!No)
	{
		m_HOANo.SetCheck(TRUE);
		m_HOAYes.SetCheck(FALSE);

		m_TCHoa_Dec1 = FALSE;
		GetConRes();
		Repaint();
	}
}

//***************************************************************************************


void CWFCTIolWnd::OnEyeCorYesClicked()
{
	BOOL Yes = m_EyeCorYes.GetCheck();
	if (!Yes)
	{
		m_EyeCorYes.SetCheck(TRUE);
		m_EyeCorNo.SetCheck(FALSE);

		GetConRes();
		Repaint();
	}
}
//***************************************************************************************

void CWFCTIolWnd::OnEyeCorNoClicked()
{
	BOOL No = m_EyeCorNo.GetCheck();
	if (!No)
	{
		m_EyeCorNo.SetCheck(TRUE);
		m_EyeCorYes.SetCheck(FALSE);

		GetConRes();
		Repaint();
	}
}

//***************************************************************************************

void CWFCTIolWnd::OnSettingClicked()
{
	BOOL Changed = FALSE;
	CSettingsIOLSelDlg* pDlg = new CSettingsIOLSelDlg(this, FALSE, ::NewSettings.m_IOLSel_Alpha, ::NewSettings.m_IOLSel_CHOA, ::NewSettings.m_IOLSel_IHOA, ::NewSettings.m_IOLSel_Astig, ::NewSettings.m_IOLSel_Sph_Abbe);

	if (pDlg->DoModal() == IDOK)
	{
		::NewSettings.m_IOLSel_Alpha[0] = pDlg->m_IOLSel_Alpha[0];
		::NewSettings.m_IOLSel_Alpha[1] = pDlg->m_IOLSel_Alpha[1];

		::NewSettings.m_IOLSel_CHOA = pDlg->m_IOLSel_CHOA;
		::NewSettings.m_IOLSel_IHOA = pDlg->m_IOLSel_IHOA;

		::NewSettings.m_IOLSel_Astig = pDlg->m_IOLSel_Astig;

		::NewSettings.m_IOLSel_Sph_Abbe[0] = pDlg->m_IOLSel_Sph_Abbe[0];
		::NewSettings.m_IOLSel_Sph_Abbe[1] = pDlg->m_IOLSel_Sph_Abbe[1];
		::NewSettings.m_IOLSel_Sph_Abbe[2] = pDlg->m_IOLSel_Sph_Abbe[2];
		::NewSettings.m_IOLSel_Sph_Abbe[3] = pDlg->m_IOLSel_Sph_Abbe[3];

		Changed = TRUE;
	}

	delete pDlg;

	if (Changed)
	{
		GetVals();
		Repaint();
	}
}

//***************************************************************************************

void CWFCTIolWnd::GetConRes()
{
	BOOL TraceyYes = m_TraceyRecYes.GetCheck();

	//#1
	if (TraceyYes)
	{
		//m_Con_Res1 = "   Minimal HOAs present";
		m_Con_Res1 = "   None";//531
		m_Con_Res2 = "";
		m_Con_Res3 = "";//new
		m_Con_Res4 = "";//new
		return;
	}

	BOOL EyeCorYes = m_EyeCorYes.GetCheck();

	//#3
	if (!EyeCorYes) //internal yes or not
	{
		//m_Con_Res1 = "   Minimal Internal HOAs present";
		m_Con_Res1 = "   None";//531
		m_Con_Res2 = "";
		m_Con_Res3 = "";//new
		m_Con_Res4 = "";//new
		return;
	}

	//#5-#8
	if (m_Alpha_Dec == 0) // Angle aplha >= 0.65
	{
		/*m_Con_Res1 = "Spherical IOL";
		m_Con_Res2 = "";*/

		//m_Con_Res1 = "1. Rayner Aspheric / MI 60";//new
		//m_Con_Res2 = "2. Spherical IOL";//new
		m_Con_Res1 = "   Spherical IOL";//531
		m_Con_Res2 = "";//531
		m_Con_Res3 = "";//new
		m_Con_Res4 = "";//new
		return;
	}

	if (m_Alpha_Dec == 1) // 0.5 <= Angle aplha < 0.65
	{
		/*m_Con_Res1 = "HOYA ABC or other Spherical IOL";
		m_Con_Res2 = "";*/

		//m_Con_Res1 = "1. HOYA ABC";//new
		//m_Con_Res2 = "2. Rayner Aspheric / MI 60";//new
		//m_Con_Res3 = "3. Spherical IOL";//new

		m_Con_Res1 = "1. Advanced Balance Curve IOL";//new 531
		m_Con_Res2 = "2. Spherical IOL";//new 531
		m_Con_Res3 = "";//new 531
		m_Con_Res4 = "";//new
		return;
	}

	//Following Angle aplha < 0.5
	if (m_TCHoa_Dec == 1 || m_TCHoa_Dec1 == 1) //Total corneal hoa >= 0.2 
	{
		m_Con_Res1 = "1. MonoFocal Non-Toric IOL";

		if (m_Cor_SphAb_Dec == 0) //Spherical Aberration (SA) at 4mm  <= 0.14
								  //m_Con_Res2 = "   ( Rayner Aspheric / MI 60 / Spherical IOL )";
			m_Con_Res2 = "2. Spherical IOL";//531
		else if (m_Cor_SphAb_Dec == 1) //Spherical Aberration (SA) at 4mm  0.14 ~ 0.20
									   // m_Con_Res2 = "   ( Hoya ABC )";
			m_Con_Res2 = "2. Advanced Balance Curve IOL";//531				   
		else if (m_Cor_SphAb_Dec == 2) //Spherical Aberration (SA) at 4mm  0.20 ~ 0.25
									   //m_Con_Res2 = "   ( Hoya ABC / Acrysof IQ )";
			m_Con_Res2 = "2. Advanced Balance Curve IOL or Aspheric IOL";//531
		else if (m_Cor_SphAb_Dec == 3) //Spherical Aberration (SA) at 4mm  0.25 ~ 0.30
			m_Con_Res2 = "2. Aspheric IOL";
		else                          //Spherical Aberration (SA) at 4mm  > 0.30
			m_Con_Res2 = "2. Aspheric IOL";
		m_Con_Res3 = "";//new
		m_Con_Res4 = "";//new
	}
	else //Total corneal hoa < 0.2 
	{
		if (m_Cor_Ast_Dec == 0) //Astigmatism < 1D
		{
			m_Con_Res1 = "1. Premium IOL";//new
			m_Con_Res2 = "2. MonoFocal Non-Toric IOL";//new

			if (m_Cor_SphAb_Dec == 0) m_Con_Res3 = "3. Spherical IOL";
			else if (m_Cor_SphAb_Dec == 1) m_Con_Res3 = "3. Advanced Balance Curve IOL";
			else if (m_Cor_SphAb_Dec == 2) m_Con_Res3 = "3. Advanced Balance Curve IOL or Aspheric IOL";
			else if (m_Cor_SphAb_Dec == 3) m_Con_Res3 = "3. Aspheric IOL";
			else                          m_Con_Res3 = "3. Aspheric IOL";

			m_Con_Res4 = "";
		}
		else
		{
			m_Con_Res1 = "1. Toric Premium IOL";//new
			m_Con_Res2 = "2. Toric IOL";//new

			if (m_Cor_SphAb_Dec == 0) m_Con_Res3 = "4. Spherical IOL";
			else if (m_Cor_SphAb_Dec == 1) m_Con_Res3 = "4. Advanced Balance Curve IOL";
			else if (m_Cor_SphAb_Dec == 2) m_Con_Res3 = "4. Advanced Balance Curve IOL or Aspheric IOL";
			else if (m_Cor_SphAb_Dec == 3) m_Con_Res3 = "4. Aspheric IOL";
			else                          m_Con_Res3 = "4. Aspheric IOL";
		}

		// if(m_Cor_Ast_Dec == 0) //Astigmatism < 1D
		// {
		//  //m_Con_Res1 = "Multifocal IOL, Mono Focal Non Toric IOL";

		//  m_Con_Res1 = "1. Multifocal IOL";//new
		//  m_Con_Res2 = "2. Mono Focal Non Toric IOL";//new

		//	 if(m_Cor_SphAb_Dec == 0) m_Con_Res3 = "   ( Rayner Aspheric / MI 60 / Spherical IOL )";
		//else if(m_Cor_SphAb_Dec == 1) m_Con_Res3 = "   ( Hoya ABC )";
		//else if(m_Cor_SphAb_Dec == 2) m_Con_Res3 = "   ( Hoya ABC / Acrysof IQ )";
		//else if(m_Cor_SphAb_Dec == 3) m_Con_Res3 = "   ( Acrysof IQ )";
		//else                          m_Con_Res3 = "   ( Tecnis IOL )";

		//m_Con_Res4 = "";
		// }
		// else
		// { 
		//  //m_Con_Res1 = "Toric MF, Toric IOL, Mono Focal Non Toric IOL";

		//  m_Con_Res1 = "1. Toric MFL";//new
		//  m_Con_Res2 = "2. Toric IOL";//new
		//  m_Con_Res3 = "3. Mono Focal Non Toric IOL";//new

		//	 if(m_Cor_SphAb_Dec == 0) m_Con_Res4 = "   ( Rayner Aspheric / MI 60 / Spherical IOL )";
		//else if(m_Cor_SphAb_Dec == 1) m_Con_Res4 = "   ( Hoya ABC )";
		//else if(m_Cor_SphAb_Dec == 2) m_Con_Res4 = "   ( Hoya ABC / Acrysof IQ )";
		//else if(m_Cor_SphAb_Dec == 3) m_Con_Res4 = "   ( Acrysof IQ )";
		//else                          m_Con_Res4 = "   ( Tecnis IOL )";
		// }
	}
}

//***************************************************************************************

void CWFCTIolWnd::GetVals()
{
	//Create surfaces and mask
	CZernikeMask Mask;
	Mask.SetType(MASK_HO_TOTAL);

	real_t r_um = 4000;

	CZernikeSurface Ct_WfSurface = m_pCTExam->m_WfSurface;//corneal
	r_um = __min(4000, Ct_WfSurface.m_r_max_um);

	CZernikeSurface WfSurface = m_pWFExam->m_WfSurface;//Entire
	r_um = __min(r_um, WfSurface.m_r_max_um);

	Ct_WfSurface.ChangeRMaxUm(r_um);
	WfSurface.ChangeRMaxUm(r_um);

	CZernikeSurface Int_WfSurface = Ct_WfSurface; //internal
	Int_WfSurface -= WfSurface;
	//Done

	Ct_WfSurface &= Mask;
	real_t rms = Ct_WfSurface.ComputeRms();
	m_vals[1] = rms;

	m_CorHoa = (rms >= ::NewSettings.m_IOLSel_CHOA ? TRUE : FALSE);//??

	Int_WfSurface &= Mask;
	rms = Int_WfSurface.ComputeRms();

	m_Inter = (rms >= ::NewSettings.m_IOLSel_IHOA ? TRUE : FALSE);
	//

	real_t AlphaRum = m_pWFExam->m_Header.m_AngleAlphaRUm*0.001;
	// 0: < 0.5; 1: 0.5~0.65; 2 >0.65.
	m_Alpha_Dec = (AlphaRum >= ::NewSettings.m_IOLSel_Alpha[0] ? 0 : AlphaRum >= ::NewSettings.m_IOLSel_Alpha[1] ? 1 : 2);
	m_vals[0] = AlphaRum;

	//BOOL CorHoa = (rms >= 0.2 ? TRUE : FALSE);
	m_TCHoa_Dec = m_TCHoa_Dec1 = (m_CorHoa ? 1 : 0);

	//> 1D ture  < 1D false;
	m_Cor_Ast_Dec = (m_pCTExam->m_rf_df_dp < ::NewSettings.m_IOLSel_Astig ? 0 : 1);
	if (m_pCTExam->m_rf_df_dp >0) m_vals[2] = m_pCTExam->m_rf_df_dp;
	else m_vals[2] = 0;

	CZernikeSurface Surface = m_pCTExam->m_WfSurface;//corneal
	Surface.ChangeRMaxUm(3000);
	real_t csa = Surface.GetCUm(12);

	// 0.14 ~ 0.20 ~ 0.25 ~0.30
	m_Cor_SphAb_Dec = (csa <= ::NewSettings.m_IOLSel_Sph_Abbe[0] ? 0 : csa <= ::NewSettings.m_IOLSel_Sph_Abbe[1] ? 1 : csa <= ::NewSettings.m_IOLSel_Sph_Abbe[2] ? 2 : csa <= ::NewSettings.m_IOLSel_Sph_Abbe[3] ? 3 : 4);
	m_vals[3] = csa;

	GetConRes();
	//
}

//************************************************************************************************************************

LRESULT CWFCTIolWnd::OnChangeMaxMsg(WPARAM wParam, LPARAM lParam)
{
	int Max = INVALID_VALUE;

	for (int d = 3; d < 6; d++) {
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] == wParam) {
			Max = ((CRMSWnd*)m_pDispWnd[d])->m_Max;

			break;
		}
	}

	if (Max == INVALID_VALUE) return 0;

	for (int d = 3; d < 6; d++) {
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] != wParam) {
			((CRMSWnd*)m_pDispWnd[d])->m_Max = Max;

			if (Max == 1) ((CRMSWnd*)m_pDispWnd[d])->m_ZoomInButton.SetEnabled(FALSE);
			else         ((CRMSWnd*)m_pDispWnd[d])->m_ZoomInButton.SetEnabled(TRUE);
			((CRMSWnd*)m_pDispWnd[d])->m_ZoomInButton.Repaint();

			m_pDispWnd[d]->Repaint();
		}
	}

	return 0;
}
