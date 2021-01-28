//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "WFSingleCmpWnd.h"
#include "EyeWnd.h"
#include "PSFWnd.h"
#include "MTFWnd.h"
#include "EEFWnd.h"
#include "LtrWnd.h"
#include "RMSWnd.h"
#include "ChartWnd.h"
#include "BusyCursor.h"
#include "SelectImgDlg.h"//521
#include "NearLtrWnd.h"//530

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CWFSingleCmpWnd, CSumWnd)

	ON_MESSAGE(WM_ROTATE, OnRotateMsg)
	ON_MESSAGE(WM_CHANGE_CENT, OnChangeCentMsg)
	ON_MESSAGE(WM_CHANGE_STEP, OnChangeStepMsg)
	ON_MESSAGE(WM_CHANGE_MAX, OnChangeMaxMsg)

	ON_COMMAND(IDC_DEFAULT_GUI_ITEM, OnShowDefaultItemClicked)//521

	ON_COMMAND(IDC_SELECT_IMAGE_ITEM, OnSelEyeImgItemClicked)//521

	ON_MESSAGE(WM_SHOW_SIMPRMS, OnChangeRmsGUI)//530 Change the rms gui between simplify and details

	ON_BN_CLICKED(IDC_SHOW_FULL_INFO, OnShowWFTextInfoBtnClicked)//530
	ON_BN_CLICKED(IDC_SHOW_FULL_INFO2, OnShowWFTextInfoBtn2Clicked)//530
	ON_BN_CLICKED(IDC_SHOW_FULL_INFO3, OnShowWFTextInfoBtn3Clicked)//530

END_MESSAGE_MAP()

//***************************************************************************************

CWFSingleCmpWnd::CWFSingleCmpWnd(CWnd *pWnd, RECT &WndRect, CPatient *pPatient, CWFExam *pWFExam1, CWFExam *pWFExam2,
                                 CWndSettings *pWndSettings, int show)
  :
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
	::SetRect(&m_Rect[5], intRound(m_g + w + m_g + w + m_g), intRound(t + h + m_g), intRound(m_g + w + m_g + w + m_g + w), intRound(t + h + h + m_g)); // diff info
																																						   //530
	for (int i = 0; i < 3; i++)
	{
		m_InsideZone[i] = ::NewSettings.m_ShowPreZone_WF_CMPWnd;
	}

	m_pWFExam1 = pWFExam1;
	m_pWFExam2 = pWFExam2;

	m_OD = m_pWFExam1->m_Header.IsOD();//521

	//record the increase and decrease range
	::TempSettings.Com_r_max_um = ((m_pWFExam1->m_WfSurface.m_r_max_um) < (m_pWFExam2->m_WfSurface.m_r_max_um) ? m_pWFExam1->m_WfSurface.m_r_max_um : m_pWFExam2->m_WfSurface.m_r_max_um);

	CreateChildWnd();
	Repaint();

	// Draw the WF Text Info button
	CString s = (m_pWndSettings[0].m_SimplifiedWFPanel ? "Show Details" : "Hide Details");
	::SetRect(&Rect, m_Rect[3].right - 25, m_Rect[3].top + 5, m_Rect[3].right - 5, m_Rect[3].top + 20);
	m_WFShowFullInfoButton.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_SHOW_FULL_INFO);
	m_WFShowFullInfoButton.SetImage(m_pWndSettings[0].m_SimplifiedWFPanel ?
		(m_SW == 1920 ? IDR_SHOW_DETAIL_3 : m_SW == 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
		(m_SW == 1920 ? IDR_HIDE_DETAIL_3 : m_SW == 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
	m_WFShowFullInfoButton.SetBk(FALSE);
	m_WFShowFullInfoButton.SetBkColor(BLACK);
	m_WFShowFullInfoButton.SetTip(s);
	m_WFShowFullInfoButton.ShowWindow(SW_SHOW);

	::SetRect(&Rect, m_Rect[4].right - 25, m_Rect[4].top + 5, m_Rect[4].right - 5, m_Rect[4].top + 20);
	m_WFShowFullInfoButton2.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_SHOW_FULL_INFO2);
	m_WFShowFullInfoButton2.SetImage(m_pWndSettings[0].m_SimplifiedWFPanel ?
		(m_SW == 1920 ? IDR_SHOW_DETAIL_3 : m_SW == 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
		(m_SW == 1920 ? IDR_HIDE_DETAIL_3 : m_SW == 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
	m_WFShowFullInfoButton2.SetBk(FALSE);
	m_WFShowFullInfoButton2.SetBkColor(BLACK);
	m_WFShowFullInfoButton2.SetTip(s);
	m_WFShowFullInfoButton2.ShowWindow(SW_SHOW);

	::SetRect(&Rect, m_Rect[5].right - 25, m_Rect[5].top + 5, m_Rect[5].right - 5, m_Rect[5].top + 20);
	m_WFShowFullInfoButton3.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_SHOW_FULL_INFO3);
	m_WFShowFullInfoButton3.SetImage(m_pWndSettings[0].m_SimplifiedWFPanel ?
		(m_SW == 1920 ? IDR_SHOW_DETAIL_3 : m_SW == 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
		(m_SW == 1920 ? IDR_HIDE_DETAIL_3 : m_SW == 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
	m_WFShowFullInfoButton3.SetBk(FALSE);
	m_WFShowFullInfoButton3.SetBkColor(BLACK);
	m_WFShowFullInfoButton3.SetTip(s);
	m_WFShowFullInfoButton3.ShowWindow(SW_SHOW);
	// Done

	ShowWindow(show);
}

//***************************************************************************************

void CWFSingleCmpWnd::RepaintMemDC()
{
	CSumWnd::RepaintMemDC();

	BOOL Rows[22] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

	if (m_pWFExam1) 
	{
		if (m_InsideZone[m_d] == TRUE)
		{
			m_pWFExam1->Process();//530

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

		if (m_pWndSettings[0].m_SimplifiedWFPanel == 1)
		{
			ChangWFExamInfo(m_pWFExam1, m_Rect[3], 1, FALSE);
		}
		else
		{
			WFExamInfo(m_pWFExam1, m_Rect[3], 22, Rows);
		}
	}

	if (m_pWFExam2) 
	{
		if (m_InsideZone[m_d] == TRUE)
		{
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

		if (m_pWndSettings[0].m_SimplifiedWFPanel == 1)
		{
			ChangWFExamInfo(m_pWFExam2, m_Rect[4], 1, FALSE);
		}
		else
		{
			WFExamInfo(m_pWFExam2, m_Rect[4], 22, Rows);
		}
	}

	CWFExam* pWFExam3 = new CWFExam;
	pWFExam3->m_WfSurface = m_Surfaces[3];
	Rows[0] = Rows[1] = Rows[2] = Rows[3] = Rows[4] = Rows[5] = 0;

	if (m_pWndSettings[0].m_SimplifiedWFPanel == 1)
	{
		ChangWFExamInfo(pWFExam3, m_Rect[5], 0, FALSE);
	}
	else
	{
		WFExamInfo(pWFExam3, m_Rect[5], 22, Rows);
	}

	delete pWFExam3;

	// Draw the glass icon for print out  
	if (m_Printing || m_Saving)
	{

		int GUIType = m_pWndSettings[0].m_Type;
		int MaskType = m_pWndSettings[0].m_Mask.GetType();

		if (GUIType != TYPE_TEYE && GUIType != TYPE_DOFF && m_pWFExam1)
		{
			DrawGlassIcon(MaskType, 1, 0);
		}

		if (GUIType != TYPE_TEYE && GUIType != TYPE_DOFF && m_pWFExam2)
		{
			DrawGlassIcon(MaskType, 1, 1);
		}

		if (GUIType != TYPE_TEYE && GUIType != TYPE_DOFF && m_pWFExam1 && m_pWFExam2)
		{
			DrawGlassIcon(MaskType, 1, 2);
		}
	}
	//  Draw the glass icon for print out  Done
}

//***************************************************************************************

void CWFSingleCmpWnd::CreateChildWnd()
{
	for (int d = 0; d < 3; d++) SAFE_DELETE(m_pDispWnd[d]);

	CWndSettings* pWndSettings = GetWndSettings();
		
	// type
	if (pWndSettings->m_Type != TYPE_TWFM &&
		pWndSettings->m_Type != TYPE_TRFM &&
		pWndSettings->m_Type != TYPE_TPSF &&
		pWndSettings->m_Type != TYPE_TMTF &&
		pWndSettings->m_Type != TYPE_TLTR &&
		pWndSettings->m_Type != TYPE_TEEF &&
		pWndSettings->m_Type != TYPE_TRMS &&
		pWndSettings->m_Type != TYPE_TAZR)
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

	if (m_InsideZone[m_d] == TRUE)
	{
		m_pWFExam1->Presbia24Process();

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

		m_pWFExam2->Presbia24Process();

		// for average exam 
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
	else
	{
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

		m_pWFExam2->Process();

		// for average exam 
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
	for (int i = 0; i < 3; i++) m_InsideZone[i] = m_InsideZone[m_d];
	::NewSettings.m_ShowPreZone_WF_CMPWnd = m_InsideZone[m_d];

	// copy
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

	CString PresbisRBs[3];
	if (m_InsideZone[m_d] == TRUE)
	{
		real_t SphEq, Sph, Cyl; int Axis;
		for (int i = 0; i < 3; i++)
		{
			CWFExam::GetSpheqSphCylAxis(m_Surfaces[i], ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);

			if (::Settings.m_RefractionRounding == 0)
			{
				Sph = round8(Sph);
				Cyl = round8(Cyl);
			}
			PresbisRBs[i].Format(_T(" %+.2f D %+.2f D x %i° "), Sph, Cyl, Axis);
		}
	}
	
	CZernikeSurface Surfaces[2];
	Surfaces[0] = m_pWFExam1->m_WfSurface;
	Surfaces[1] = m_pWFExam2->m_WfSurface;

	// change diameter
	real_t n_r_max_um = 5000.0;
	n_r_max_um = __min(n_r_max_um, Surfaces[0].m_r_max_um);
	n_r_max_um = __min(n_r_max_um, Surfaces[1].m_r_max_um);

	Surfaces[0].ChangeRMaxUm(n_r_max_um);
	Surfaces[1].ChangeRMaxUm(n_r_max_um);

	// mask
	Surfaces[0] &= pWndSettings->m_Mask;
	Surfaces[0].ComputePiston(pWndSettings->m_Piston);
	Surfaces[0].ComputeTilt(pWndSettings->m_Tilt);
	Surfaces[1] &= pWndSettings->m_Mask;
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

			if (m_InsideZone[m_d] == TRUE)
			{
				pEyeWnd->m_LTLabel[2] = "Presbia 2.4mm";
			}

			pEyeWnd->m_MapShowEye = pWndSettings->m_MapShowEye;
			pEyeWnd->m_MapShowMap = pWndSettings->m_MapShowMap;
			pEyeWnd->m_MapShowNumbers = pWndSettings->m_MapShowNumbers;
			pEyeWnd->m_MapShowPupil = pWndSettings->m_MapShowPupil;
			pEyeWnd->m_MapShowCornealVertex = pWndSettings->m_MapShowCornealVertex;
			pEyeWnd->m_MapShowLimbus = FALSE;
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
				if (m_InsideZone[d] == TRUE)
				{
					CWFExam::CreatePresbiaWfm3D(pEyeWnd->m_Map3D, m_Surfaces[d]);
				}
				else
				{
					CWFExam::CreateWfm3D(pEyeWnd->m_Map3D, m_Surfaces[d]);
				}
			}
			else
			{
				if (m_InsideZone[d] == TRUE) CWFExam::CreatePresbiaWfm2D(pEyeWnd->m_Map2D, m_Surfaces[d], 1.0 / pEyeWnd->m_y_px_um);
				else                        CWFExam::CreateWfm2D(pEyeWnd->m_Map2D, m_Surfaces[d], 1.0 / pEyeWnd->m_y_px_um);
			}
			pEyeWnd->CreateCentButtons();
			pEyeWnd->CreateZoomButtons();

			if (m_InsideZone[d] == TRUE) pEyeWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 2);
			else                        pEyeWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 1);


			if (m_InsideZone[m_d] != TRUE)
			{
				pEyeWnd->CreateZoneButtons();
			}
			else
			{
				pEyeWnd->m_RBLabel[5] = PresbisRBs[d];
				pEyeWnd->m_RBLabelColor[5] = m_Printing ? BLACK : WHITE;
			}

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

			if (m_InsideZone[m_d] == TRUE)
			{
				pEyeWnd->m_LTLabel[2] = "Presbia 2.4mm";
			}

			pEyeWnd->m_MapShowEye = pWndSettings->m_MapShowEye;
			pEyeWnd->m_MapShowMap = pWndSettings->m_MapShowMap;
			pEyeWnd->m_MapShowNumbers = pWndSettings->m_MapShowNumbers;
			pEyeWnd->m_MapShowPupil = pWndSettings->m_MapShowPupil;
			pEyeWnd->m_MapShowCornealVertex = pWndSettings->m_MapShowCornealVertex;
			pEyeWnd->m_MapShowLimbus = FALSE;
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
				if (m_InsideZone[d] == TRUE) CWFExam::CreatePresbiaRfm3D(pEyeWnd->m_Map3D, m_Surfaces[d]);
				else                        CWFExam::CreateRfm3D(pEyeWnd->m_Map3D, m_Surfaces[d]);
			}
			else
			{
				if (m_InsideZone[d] == TRUE)  CWFExam::CreatePresbiaRfm2D(pEyeWnd->m_Map2D, m_Surfaces[d], 1.0 / pEyeWnd->m_y_px_um);
				else                         CWFExam::CreateRfm2D(pEyeWnd->m_Map2D, m_Surfaces[d], 1.0 / pEyeWnd->m_y_px_um);
			}

			pEyeWnd->CreateCentButtons();
			pEyeWnd->CreateZoomButtons();

			if (m_InsideZone[d] == TRUE) pEyeWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 2);
			else                        pEyeWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 1);																										  

			if (m_InsideZone[m_d] != TRUE) pEyeWnd->CreateZoneButtons();
			else
			{
				pEyeWnd->m_RBLabel[5] = PresbisRBs[d];
				pEyeWnd->m_RBLabelColor[5] = m_Printing ? BLACK : WHITE;
			}

			m_pDispWnd[d] = pEyeWnd;
		}
		break;

		case TYPE_TPSF:
		{
			CPSFWnd* pPSFWnd = new CPSFWnd(Rect, this);
			CWFExam::CreatePsf(m_Surfaces[d], pPSFWnd->m_Psf, pWndSettings->m_EEFType);
			pPSFWnd->m_Type = pWndSettings->m_Type;
			pPSFWnd->m_SizeSmall = TRUE;
			pPSFWnd->m_Method3D = pWndSettings->m_Method3D;
			pPSFWnd->m_LTLabel[0] = "Point Spread Function";
			pWndSettings->m_Mask.GetName(pPSFWnd->m_LTLabel[1]);
			pPSFWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);

			if (m_InsideZone[m_d] == TRUE) pPSFWnd->m_LTLabel[2] = "Presbia 2.4mm";

			pPSFWnd->m_ShowEEF = pWndSettings->m_ShowEEF;
			pPSFWnd->CreateZoomButtons();

			if (m_InsideZone[m_d] == TRUE) pPSFWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 2);//531
			else                          pPSFWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 1);//531

			if (m_InsideZone[m_d] != TRUE) pPSFWnd->CreateZoneButtons();
			else
			{
				pPSFWnd->m_RBLabel[5] = PresbisRBs[d];
				pPSFWnd->m_RBLabelColor[5] = m_Printing ? BLACK : WHITE;
			}

			m_pDispWnd[d] = pPSFWnd;
		}
		break;

		case TYPE_TLTR:
		{
			CLtrWnd* pLtrWnd = new CLtrWnd(Rect, this);
			pLtrWnd->m_DysVal = -1;
			CWFExam::CreateLtr(m_Surfaces[d], pLtrWnd->m_Ltr, pWndSettings->m_Ltr, pWndSettings->m_LtrLine, pWndSettings->m_LtrOrientation);
			pLtrWnd->m_Type = pWndSettings->m_Type;
			pLtrWnd->m_SizeSmall = TRUE;
			pLtrWnd->m_Method3D = pWndSettings->m_Method3D;

			pLtrWnd->m_LTLabel[0] = "Letter";
			pWndSettings->m_Mask.GetName(pLtrWnd->m_LTLabel[1]);
			pLtrWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);

			if (m_InsideZone[m_d] == TRUE) pLtrWnd->m_LTLabel[2] = "Presbia 2.4mm";

			pLtrWnd->CreateZoomButtons();
		
			if (m_InsideZone[m_d] != TRUE) pLtrWnd->CreateZoneButtons();
			else
			{
				pLtrWnd->m_RBLabel[5] = PresbisRBs[d];
				pLtrWnd->m_RBLabelColor[5] = m_Printing ? BLACK : WHITE;
			}


			if (m_InsideZone[m_d] == TRUE) pLtrWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 2);
			else                          pLtrWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 1);

			m_pDispWnd[d] = pLtrWnd;
		}
		break;

		case TYPE_TMTF:
		{
			CMTFWnd* pMTFWnd = new CMTFWnd(Rect, this);
			CWFExam::CreateMtf(m_Surfaces[d], pMTFWnd->m_Mtf);
			pMTFWnd->m_Type = pWndSettings->m_Type;
			pMTFWnd->m_SizeSmall = TRUE;
			pMTFWnd->m_Method3D = pWndSettings->m_Method3D;

			pMTFWnd->m_ShowDys = FALSE;

			pMTFWnd->m_LTLabel[0] = "Modulation Transfer Function";
			pWndSettings->m_Mask.GetName(pMTFWnd->m_LTLabel[1]);
			pMTFWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);

			if (m_InsideZone[m_d] == TRUE) pMTFWnd->m_LTLabel[2] = "Presbia 2.4mm";

			pMTFWnd->m_Average = pWndSettings->m_MTFAverage;
			pMTFWnd->m_Angle = pWndSettings->m_MTFAngle;
			if (!pWndSettings->m_Method3D && !pWndSettings->m_MTFAverage)
			{
				pMTFWnd->CreateSlider();
			}
			
			if (m_InsideZone[m_d] != TRUE) pMTFWnd->CreateZoneButtons();
			else
			{
				pMTFWnd->m_RBLabel[5] = PresbisRBs[d];
				pMTFWnd->m_RBLabelColor[5] = m_Printing ? BLACK : WHITE;
			}

			if (m_InsideZone[m_d] == TRUE) pMTFWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 2);
			else                          pMTFWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 1);

			m_pDispWnd[d] = pMTFWnd;
		}
		break;

		case TYPE_TEEF:
		{
			CEEFWnd* pEEFWnd = new CEEFWnd(Rect, this);
			CWFExam::CreatePsf(m_Surfaces[d], pEEFWnd->m_Psf, pWndSettings->m_EEFType);
			pEEFWnd->m_Type = pWndSettings->m_Type;
			pEEFWnd->m_SizeSmall = TRUE;
			pEEFWnd->m_Method3D = pWndSettings->m_Method3D;
			pEEFWnd->m_LTLabel[0] = "Encircled Energy Function";
			pWndSettings->m_Mask.GetName(pEEFWnd->m_LTLabel[1]);
			pEEFWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);

			if (m_InsideZone[m_d] == TRUE) pEEFWnd->m_LTLabel[2] = "Presbia 2.4mm";

			//pEEFWnd->CreateZoneButtons();

			if (m_InsideZone[m_d] != TRUE)
			{
				pEEFWnd->CreateZoneButtons();
			}
			else
			{
				pEEFWnd->m_RBLabel[5] = PresbisRBs[d];
				pEEFWnd->m_RBLabelColor[5] = m_Printing ? BLACK : WHITE;
			}

			if (m_InsideZone[m_d] == TRUE) pEEFWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 2);
			else                          pEEFWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 1);

			m_pDispWnd[d] = pEEFWnd;
		}
		break;

		case TYPE_TRMS:
		{
			CRMSWnd* pRMSWnd = new CRMSWnd(Rect, this);
			pRMSWnd->m_SimpliedRMS = pWndSettings->m_SimplifiedRMS;
			pRMSWnd->m_Type = pWndSettings->m_Type;
			pRMSWnd->m_SizeSmall = TRUE;
			pRMSWnd->m_Method3D = pWndSettings->m_Method3D;
			pRMSWnd->m_LTLabel[0] = "Root Mean Square";
			pWndSettings->m_Mask.GetName(pRMSWnd->m_LTLabel[1]);
			pRMSWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);

			if (m_InsideZone[m_d] == TRUE) pRMSWnd->m_LTLabel[2] = "Presbia 2.4mm";

			pRMSWnd->m_Surface = m_Surfaces[d];
			pRMSWnd->m_Individual = pWndSettings->m_RMSIndividual;
			if (!pRMSWnd->m_Method3D && pRMSWnd->m_Individual) pRMSWnd->CreateSimpButtons(!pWndSettings->m_SimplifiedRMS);//530
			pRMSWnd->CreateZoomButtons();

			if (m_InsideZone[m_d] != TRUE)
			{
				pRMSWnd->CreateZoneButtons();
			}
			else
			{
				pRMSWnd->m_RBLabel[5] = PresbisRBs[d];
				pRMSWnd->m_RBLabelColor[5] = m_Printing ? BLACK : WHITE;
			}

			if (m_InsideZone[m_d] == TRUE)
			{
				pRMSWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 2);
			}
			else
			{
				pRMSWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 1);
			}

			m_pDispWnd[d] = pRMSWnd;
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

			if (m_InsideZone[m_d] == TRUE) pAZRWnd->m_LTLabel[2] = "Presbia 2.4mm";
		
			real_t dr_um = 100.0;
			int n = (int)(r_max_um / dr_um) + 1;

			//
			pAZRWnd->m_MainLabels.Create(2);
			pAZRWnd->m_MainLabels[0] = "Annular Refraction";
			pAZRWnd->m_MainLabels[1] = "Zonal Refraction";
			//
			pAZRWnd->m_Colors.Create(2);
			pAZRWnd->m_Colors[0] = MAGENTA;
			pAZRWnd->m_Colors[1] = m_Printing ? BLUE : CYAN;
			//
			pAZRWnd->m_YUnitLabel = "diopters";
			//
			pAZRWnd->m_XUnitLabel = "millimeters";
			//
			pAZRWnd->m_BLabels.Create(n);

			for (int i = 0; i < n; i++) 
			{
				real_t r_mm = 0.002 * i * dr_um;
				if (fabs(r_mm - (int)r_mm) < 0.001) 
				{
					pAZRWnd->m_BLabels[i].Format(_T("%i"), (int)r_mm);
				}
			}

			pAZRWnd->m_Values.Create(n, 2);
			for (int i = 0; i < n; i++)
			{
				real_t r_um = i == 0 ? 1.0 : i * dr_um;
				pAZRWnd->m_Values(i, 0) = CWFExam::GetAnnularRfAt(m_Surfaces[d], r_um);
				pAZRWnd->m_Values(i, 1) = CWFExam::GetAverageRfAt(m_Surfaces[d], r_um);
			}		

			if (m_InsideZone[m_d] != TRUE)
			{
				pAZRWnd->CreateZoneButtons();
			}
			else
			{
				pAZRWnd->m_RBLabel[5] = PresbisRBs[d];
				pAZRWnd->m_RBLabelColor[5] = m_Printing ? BLACK : WHITE;
			}


			if (m_InsideZone[m_d] == TRUE)
			{
				pAZRWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 2);
			}
			else
			{
				pAZRWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 1);
			}

			m_pDispWnd[d] = pAZRWnd;
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
			for (int d = 0; d < 3; d++) {
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
		//
		for (int d = 0; d < 3; d++)
		{
			CEyeWnd* pEyeWnd = (CEyeWnd*)m_pDispWnd[d];
			pEyeWnd->m_Cent = Cent;
			pEyeWnd->m_Step = Step;
		}
	}

	else if (pWndSettings->m_Type == TYPE_TRMS)
	{
		for (int d = 0; d < 3; d++)
		{
			((CRMSWnd*)m_pDispWnd[d])->m_Max = 1;
			((CRMSWnd*)m_pDispWnd[d])->m_ZoomInButton.SetEnabled(FALSE);
			((CRMSWnd*)m_pDispWnd[d])->m_ZoomInButton.Repaint();
		}
	}

	else if (pWndSettings->m_Type == TYPE_TAZR)
	{
		Matrix<real_t> max_arr(3);
		Matrix<real_t> min_arr(3);

		for (int d = 0; d < 3; d++)
		{
			CChartWnd* pAZRWnd = (CChartWnd*)m_pDispWnd[d];
			pAZRWnd->m_Values.GetMax(max_arr[d]);
			pAZRWnd->m_Values.GetMin(min_arr[d]);
		}

		real_t rmax; max_arr.GetMax(rmax);
		real_t rmin; min_arr.GetMin(rmin);
		real_t t = __max(fabs(rmin), fabs(rmax));
		int imax = (int)t;
		if (t > imax + 0.001) imax++;
		int imin = -imax;

		for (int d = 0; d < 3; d++)
		{
			CChartWnd* pAZRWnd = (CChartWnd*)m_pDispWnd[d];
			pAZRWnd->m_max = imax;
			pAZRWnd->m_min = imin;
			pAZRWnd->m_LLabels.Create(imax - imin + 1);

			for (int i = imin; i <= imax; i++)
			{
				pAZRWnd->m_LLabels[i - imin].Format(_T("%i"), i);
			}
		}
	}

	for (int d = 0; d < 3; d++)
	{
		if (m_pWndSettings[0].m_ZoneRadiusUm >= ::TempSettings.Com_r_max_um && m_InsideZone[m_d] == FALSE)
		{
			m_pDispWnd[d]->m_IncZoneButton.SetEnabled(FALSE);
			m_pDispWnd[d]->m_IncZoneButton.Repaint();
		}

		if (m_pWndSettings[0].m_ZoneRadiusUm == 500 && m_InsideZone[m_d] == FALSE)
		{
			m_pDispWnd[d]->m_DecZoneButton.SetEnabled(FALSE);
			m_pDispWnd[d]->m_DecZoneButton.Repaint();
		}
	}
}

//***************************************************************************************

LRESULT CWFSingleCmpWnd::OnRotateMsg(WPARAM wParam, LPARAM lParam)
{
	int ax, ay;

	for (int d = 0; d < 3; d++)
	{
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] == wParam)
		{
			ax = m_pDispWnd[d]->m_ax;
			ay = m_pDispWnd[d]->m_ay;
			break;
		}
	}

	for (int d = 0; d < 3; d++)
	{
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] != wParam)
		{
			m_pDispWnd[d]->m_ax = ax;
			m_pDispWnd[d]->m_ay = ay;
			m_pDispWnd[d]->Repaint();
		}
	}

	return 0;
}

//***************************************************************************************

LRESULT CWFSingleCmpWnd::OnChangeCentMsg(WPARAM wParam, LPARAM lParam)
{
	real_t Cent = INVALID_VALUE;

	for (int d = 0; d < 2; d++)
	{
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] == wParam)
		{
			Cent = ((CEyeWnd*)m_pDispWnd[d])->m_Cent;
			break;
		}
	}

	if (Cent == INVALID_VALUE) return 0;

	for (int d = 0; d < 2; d++)
	{
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] != wParam)
		{
			((CEyeWnd*)m_pDispWnd[d])->m_Cent = Cent;
			m_pDispWnd[d]->Repaint();
			break;
		}
	}

	return 0;
}

//***************************************************************************************

LRESULT CWFSingleCmpWnd::OnChangeStepMsg(WPARAM wParam, LPARAM lParam)
{
	real_t Step = INVALID_VALUE;

	for (int d = 0; d < 2; d++)
	{
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] == wParam)
		{
			Step = ((CEyeWnd*)m_pDispWnd[d])->m_Step;
			break;
		}
	}

	if (Step == INVALID_VALUE) return 0;

	for (int d = 0; d < 2; d++)
	{
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] != wParam)
		{
			((CEyeWnd*)m_pDispWnd[d])->m_Step = Step;
			m_pDispWnd[d]->Repaint();
			break;
		}
	}

	return 0;
}

//***************************************************************************************

LRESULT CWFSingleCmpWnd::OnChangeMaxMsg(WPARAM wParam, LPARAM lParam)
{
	int Max = INVALID_VALUE;

	for (int d = 0; d < 2; d++)
	{
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] == wParam)
		{
			Max = ((CRMSWnd*)m_pDispWnd[d])->m_Max;
			break;
		}
	}

	if (Max == INVALID_VALUE) return 0;

	for (int d = 0; d < 2; d++)
	{
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] != wParam)
		{
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

void CWFSingleCmpWnd::CreateMenu()
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
	m_pMenu->AppendItem(IDC_TYPE_TPSF_ITEM, pWndSettings->m_Type == TYPE_TPSF, FALSE, "Point Spread Function", "");
	m_pMenu->AppendItem(IDC_TYPE_TLTR_ITEM, pWndSettings->m_Type == TYPE_TLTR, FALSE, "Visual Acuity Chart", "");
	
	m_pMenu->AppendItem(IDC_TYPE_TMTF_ITEM, pWndSettings->m_Type == TYPE_TMTF, FALSE, "Modulation Transfer Function", "");
	m_pMenu->AppendItem(IDC_TYPE_TEEF_ITEM, pWndSettings->m_Type == TYPE_TEEF, FALSE, "Encircled Energy Function", "");
	m_pMenu->AppendItem(IDC_TYPE_TRMS_ITEM, pWndSettings->m_Type == TYPE_TRMS, FALSE, "Root Mean Square", "");

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

	int type = 0;
	if (::TempSettings.Com_r_max_um <= 700) type = 1;
	CreateSubmenuZone(type);

	CreateSubmenuMask();
	m_pMenu->AppendSeparator();

	if (pWndSettings->m_Type == TYPE_TWFM || pWndSettings->m_Type == TYPE_TRFM)
	{
		if (pWndSettings->m_Method3D)
		{
			m_pMenu->AppendItem(IDC_SHOW_MAP_SURFACE_ITEM, FALSE, pWndSettings->m_MapShowSolidSurface, "Show solid surface", "");
			m_pMenu->AppendItem(IDC_SHOW_MAP_MESH_ITEM, FALSE, pWndSettings->m_MapShowWireMesh, "Show wire mesh", "");
		}
		else
		{
			m_pMenu->AppendItem(IDC_SHOW_MAP_MAP_ITEM, FALSE, pWndSettings->m_MapShowMap, "Show map", "");
			m_pMenu->AppendItem(IDC_SHOW_MAP_NUMBERS_ITEM, FALSE, pWndSettings->m_MapShowNumbers, "Show numbers", "");

			if ((m_d == 0 && m_pWFExam1->m_Image.m_RGBData) ||
				(m_d == 1 && m_pWFExam2->m_Image.m_RGBData)
				)
			{
				m_pMenu->AppendItem(IDC_SHOW_MAP_EYE_ITEM, FALSE, pWndSettings->m_MapShowEye, "Show eye image", "");
				m_pMenu->AppendItem(IDC_SHOW_MAP_PUPIL_ITEM, FALSE, pWndSettings->m_MapShowPupil, "Show pupil", "");
				m_pMenu->AppendItem(IDC_SHOW_MAP_VERTEX_ITEM, FALSE, pWndSettings->m_MapShowCornealVertex, "Show corneal vertex", "");
				// m_pMenu->AppendItem(IDC_SHOW_MAP_LIMBUS_ITEM, FALSE, pWndSettings->m_MapShowLimbus, "Show limbus", "");//
			}

			m_pMenu->AppendItem(IDC_MAP_TRANSLUCENT_ITEM, FALSE, pWndSettings->m_MapTranslucent, "Translucent map", "");
		}
		m_pMenu->AppendSeparator();//521
		CreateSubmenuMapSteps();
		CreateSubmenuMapColors();
		m_pMenu->AppendSeparator();
	}

	else if (pWndSettings->m_Type == TYPE_TPSF)
	{
		m_pMenu->AppendItem(IDC_SHOW_PSF_EEF_ITEM, FALSE, pWndSettings->m_ShowEEF, "Show 50% energy circle", "");
		if (pWndSettings->m_ShowEEF)
		{
			if (pWndSettings->m_EEFType != EEF_CNT) pWndSettings->m_EEFType = EEF_MAX;
			m_pMenu->AppendItem(IDC_EEF_TYPE1_ITEM, pWndSettings->m_EEFType == EEF_MAX, FALSE, "Circle around maximum", "");
			m_pMenu->AppendItem(IDC_EEF_TYPE2_ITEM, pWndSettings->m_EEFType == EEF_CNT, FALSE, "Circle around centroid", "");
		}
		m_pMenu->AppendSeparator();
	}

	else if (pWndSettings->m_Type == TYPE_TMTF)
	{
		if (!pWndSettings->m_Method3D)
		{
			m_pMenu->AppendItem(IDC_MTF_AVG_ITEM, FALSE, pWndSettings->m_MTFAverage, "Radial average curve", "");
			m_pMenu->AppendSeparator();
		}
	}

	else if (pWndSettings->m_Type == TYPE_TLTR)
	{
		CreateSubmenuLtr();
		m_pMenu->AppendSeparator();
	}

	else if (pWndSettings->m_Type == TYPE_TPSF || pWndSettings->m_Type == TYPE_TEEF)
	{
		if (pWndSettings->m_EEFType != EEF_CNT) pWndSettings->m_EEFType = EEF_MAX;
		m_pMenu->AppendItem(IDC_EEF_TYPE1_ITEM, pWndSettings->m_EEFType == EEF_MAX, FALSE, "Circle round maximum", "");
		m_pMenu->AppendItem(IDC_EEF_TYPE2_ITEM, pWndSettings->m_EEFType == EEF_CNT, FALSE, "Circle round centroid", "");
		m_pMenu->AppendSeparator();
	}

	else if (pWndSettings->m_Type == TYPE_TRMS)
	{
		if (!pWndSettings->m_Method3D)
		{
			m_pMenu->AppendItem(IDC_RMS_CMB_ITEM, FALSE, !pWndSettings->m_RMSIndividual, "Combined", "");
			m_pMenu->AppendSeparator();
		}
	}

	m_pMenu->AppendItem(IDC_SAVE_TXT_ITEM, FALSE, FALSE, "Save Zernike coefficients into file", "");

	if (!SameCmp())
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
	}
}

//***************************************************************************************

void CWFSingleCmpWnd::OnShowDefaultItemClicked()
{
	CBusyCursor Cursor;

	CWndSettings* pWndSettings = GetWndSettings();

	pWndSettings->m_LtrDistanceType = 0;

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

//***************************************************************************************

BOOL CWFSingleCmpWnd::SameCmp()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (pWndSettings->m_Type != 11)
	{
		return FALSE;
	}

	if (pWndSettings->m_Method3D != 0)
	{
		return FALSE;
	}

	if (pWndSettings->m_Mask.GetType() != MASK_TOTAL)
	{
		return FALSE;
	}

	if (pWndSettings->m_Piston != FALSE)
	{
		return FALSE;
	}

	if (pWndSettings->m_Tilt != FALSE)
	{
		return FALSE;
	}

	if (pWndSettings->m_MapShowMap != 1)
	{
		return FALSE;
	}

	if (pWndSettings->m_MapShowNumbers != 0)
	{
		return FALSE;
	}

	if (((m_d == 0 && m_pWFExam1->m_Image.m_RGBData) ||
		(m_d == 1 && m_pWFExam2->m_Image.m_RGBData)) &&
		pWndSettings->m_MapShowEye != 0)
	{
		return FALSE;
	}

	if (((m_d == 0 && m_pWFExam1->m_Image.m_RGBData) ||
		(m_d == 1 && m_pWFExam2->m_Image.m_RGBData)) &&
		pWndSettings->m_MapShowPupil != 0)
	{
		return FALSE;
	}

	if (pWndSettings->m_MapShowCornealVertex != 0)
	{
		return FALSE;
		if (pWndSettings->m_MapTranslucent != 0)
		{
			return FALSE;
		}

		CScale* pScale = pWndSettings->GetScale();

		if (pScale->m_Step != 0.5)
		{
			return FALSE;
		}

		if (!pWndSettings->AreDefaultColors(1))
		{
			return FALSE;
		}

		return TRUE;
	}
}

//***************************************************************************************

void CWFSingleCmpWnd::OnSelEyeImgItemClicked()
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

LRESULT CWFSingleCmpWnd::OnChangeRmsGUI(WPARAM wParam, LPARAM lParam)
{
	BOOL Simple = FALSE;

	for (int d = 0; d < 3; d++)
	{
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
			(W == 1920 ? IDR_SHOW_DETAIL_3 : W == 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
			(W == 1920 ? IDR_HIDE_DETAIL_3 : W == 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));

		m_pDispWnd[s_d]->m_ShowMoreButton.Repaint();
		m_pDispWnd[s_d]->Repaint();
	}
	return 0;
}

//***************************************************************************************

void CWFSingleCmpWnd::OnShowWFTextInfoBtnClicked()
{
	m_pWndSettings[0].m_SimplifiedWFPanel = !m_pWndSettings[0].m_SimplifiedWFPanel;
	Repaint();

	CString s = (m_pWndSettings[0].m_SimplifiedWFPanel ? "Show Details" : "Hide Details");
	m_WFShowFullInfoButton.SetTip(s);
	m_WFShowFullInfoButton.SetImage(m_pWndSettings[0].m_SimplifiedWFPanel ?
		(m_SW == 1920 ? IDR_SHOW_DETAIL_3 : m_SW == 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
		(m_SW == 1920 ? IDR_HIDE_DETAIL_3 : m_SW == 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
	m_WFShowFullInfoButton.Repaint();

	m_WFShowFullInfoButton2.SetTip(s);
	m_WFShowFullInfoButton2.SetImage(m_pWndSettings[0].m_SimplifiedWFPanel ?
		(m_SW == 1920 ? IDR_SHOW_DETAIL_3 : m_SW == 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
		(m_SW == 1920 ? IDR_HIDE_DETAIL_3 : m_SW == 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
	m_WFShowFullInfoButton2.Repaint();

	m_WFShowFullInfoButton3.SetTip(s);
	m_WFShowFullInfoButton3.SetImage(m_pWndSettings[0].m_SimplifiedWFPanel ?
		(m_SW == 1920 ? IDR_SHOW_DETAIL_3 : m_SW == 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
		(m_SW == 1920 ? IDR_HIDE_DETAIL_3 : m_SW == 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
	m_WFShowFullInfoButton3.Repaint();
}

//***************************************************************************************

void CWFSingleCmpWnd::OnShowWFTextInfoBtn2Clicked()
{
	OnShowWFTextInfoBtnClicked();
}

//***************************************************************************************

void CWFSingleCmpWnd::OnShowWFTextInfoBtn3Clicked()
{
	OnShowWFTextInfoBtnClicked();
}

//***************************************************************************************
