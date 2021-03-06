//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "WFCTSumWnd.h"
#include "EyeWnd.h"
#include "PSFWnd.h"
#include "MTFWnd.h"
#include "LTRWnd.h"
#include "EEFWnd.h"
#include "RMSWnd.h"
#include "BusyCursor.h"
#include "GlobalFunctions.h"

//***************************************************************************************

BEGIN_MESSAGE_MAP(CWFCTSumWnd, CSumWnd)

	ON_MESSAGE(WM_ROTATE, OnRotateMsg)
	ON_MESSAGE(WM_CHANGE_CENT, OnChangeCentMsg)
	ON_MESSAGE(WM_CHANGE_STEP, OnChangeStepMsg)
	ON_MESSAGE(WM_CHANGE_MAX, OnChangeMaxMsg)
	ON_MESSAGE(WM_SHOW_SIMPRMS, OnChangeRmsGUI)//530 Change the rms gui between simplify and details

	ON_COMMAND(IDC_DEFAULT_GUI_ITEM, OnShowDefaultItemClicked)

	ON_BN_CLICKED(IDC_SHOW_FULL_INFO, OnShowWFTextInfoBtnClicked)
	ON_BN_CLICKED(IDC_SHOW_FULL_CTINFO, OnShowCTTextInfoBtnClicked)

END_MESSAGE_MAP()

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

CWFCTSumWnd::CWFCTSumWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CWFExam* pWFExam, CCTExam* pCTExam, CWndSettings* pWndSettings, int i, int show) :
	CSumWnd(pWnd, WndRect, pPatient, pWndSettings)
{
	CBusyCursor Cursor;

	//----------------------------------------------------
	RECT Rect;
	GetWindowRect(&Rect);
	real_t t = 3.0 * (0.03 * m_h) + m_g;
	real_t w = ((Rect.right - Rect.left) - 4.0 * m_g) / 3.0;
	real_t h = ((Rect.bottom - Rect.top - t) - 2.0 * m_g) / 2.0;
	::SetRect(&m_Rect[0], intRound(m_g), intRound(t), intRound(m_g + w), intRound(t + h)); // internal
	::SetRect(&m_Rect[1], intRound(m_g), intRound(t + h + m_g), intRound(m_g + w), intRound(t + h + m_g + h)); // corneal
	::SetRect(&m_Rect[2], intRound(m_g + w + m_g), intRound(t), intRound(m_g + w + m_g + w), intRound(t + h)); // total
	::SetRect(&m_Rect[3], intRound(m_g + w + m_g), intRound(t + h + m_g), intRound(m_g + w + m_g + w), intRound(t + h + m_g + h)); // axial
	::SetRect(&m_Rect[4], intRound(m_g + w + m_g + w + m_g), intRound(t), intRound(m_g + w + m_g + w + m_g + w), intRound(t + h)); // wf exam info
	::SetRect(&m_Rect[5], intRound(m_g + w + m_g + w + m_g), intRound(t + h + m_g), intRound(m_g + w + m_g + w + m_g + w), intRound(t + h + m_g + h)); // ct exam info
	::SetRect(&m_LargeRect, intRound(m_g), intRound(t), intRound(m_g + w + m_g + w), intRound(t + h + m_g + h)); // large map
	//----------------------------------------------------
																											 //m_OutLineRect[0]
	::SetRect(&m_OutLineRect[0], intRound(m_g + w + m_g - 2), intRound(t - 2), intRound(m_g + w + m_g), intRound(t + h + 2));
	::SetRect(&m_OutLineRect[1], intRound(m_g + w + m_g - 2), intRound(t - 2), intRound(m_g + w + m_g + w + m_g + w + 2), intRound(t));
	::SetRect(&m_OutLineRect[2], intRound(m_g + w + m_g - 2), intRound(t + h), intRound(m_g + w + m_g + w + m_g + w + 2), intRound(t + h + 2));
	::SetRect(&m_OutLineRect[3], intRound(m_g + w + m_g + w + m_g + w), intRound(t - 2), intRound(m_g + w + m_g + w + m_g + w + 2), intRound(t + h + 2));

	::SetRect(&m_OutLineRect[4], intRound(m_g - 2), intRound(t + h + m_g - 2), intRound(m_g), intRound(t + h + m_g + h));
	::SetRect(&m_OutLineRect[5], intRound(m_g - 2), intRound(t + h + m_g - 2), intRound(m_g + w + m_g + w + m_g + w + 2), intRound(t + h + m_g));
	::SetRect(&m_OutLineRect[6], intRound(m_g + real_t(w)*0.38), intRound(t + h + m_g + h), intRound(m_g + w + m_g + w + m_g + w + 2), intRound(t + h + m_g + h + 2));
	::SetRect(&m_OutLineRect[7], intRound(m_g + w + m_g + w + m_g + w), intRound(t + h + m_g - 2), intRound(m_g + w + m_g + w + m_g + w + 2), intRound(t + h + m_g + h + 2));
	//Done

	m_pWFExam = pWFExam;
	m_pCTExam = pCTExam;

	// WFCT Angle k/a adjust wf eye limbus by ct info
	//if (pWFExam->m_AlignmentMethod == ALIGNMENT_4_IR_DOTS)
	{
		LimbusAdjust(m_pWFExam, m_pCTExam);
		m_pWFExam->CalcAngles();
	}

	if (pWFExam->m_AlignmentMethod == ALIGNMENT_PUPIL_CENTER)
	{
		m_pCTExam->CalcAngles();
		if (m_pCTExam->m_Header.m_AngleAlphaOK)
		{
			m_pWFExam->m_Header.m_AngleAlphaOK = TRUE;
			m_pWFExam->m_Header.m_AngleAlphaRUm = m_pCTExam->m_Header.m_AngleAlphaRUm;
			m_pWFExam->m_Header.m_AngleAlphaADg = m_pCTExam->m_Header.m_AngleAlphaADg;
		}
	}
	// WFCT Angle k/a adjust wf eye limbus by ct info Done

	//*007*[cjf***05052012],record the increase and decrease range
	::TempSettings.Com_r_max_um = ((m_pWFExam->m_WfSurface.m_r_max_um) < (m_pCTExam->m_WfSurface.m_r_max_um) ? m_pWFExam->m_WfSurface.m_r_max_um : m_pCTExam->m_WfSurface.m_r_max_um);
	//*007*[cjf***05052012]

	m_i = i;

	int Stop = 6;
	if (m_i == 6)	  Stop = 4;
	for (int d = 0; d < Stop; d++)   m_pWndSettings[d].m_ZoneRadiusUm = intRound(m_pWFExam->m_WfSurface.m_r_max_um); //600

	CreateChildWnd();
	Repaint();
	//::TempSettings.Temp_WFCTCA = FALSE;

	//530 Draw the WF and CT Text Info button
	CString s = (m_pWndSettings[0].m_SimplifiedWFPanel ? "Show Details" : "Hide Details");
	::SetRect(&Rect, m_Rect[4].right - 25, m_Rect[4].top + 5, m_Rect[4].right - 5, m_Rect[4].top + 25);
	m_WFShowFullInfoButton.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_SHOW_FULL_INFO);
	m_WFShowFullInfoButton.SetImage(m_pWndSettings[0].m_SimplifiedWFPanel ?
		(m_SW >= 1920 ? IDR_SHOW_DETAIL_3 : m_SW >= 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
		(m_SW >= 1920 ? IDR_HIDE_DETAIL_3 : m_SW >= 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
	m_WFShowFullInfoButton.SetBk(FALSE);
	m_WFShowFullInfoButton.SetBkColor(BLACK);
	m_WFShowFullInfoButton.SetTip(s);
	m_WFShowFullInfoButton.ShowWindow(SW_SHOW);

	::SetRect(&Rect, m_Rect[5].right - 25, m_Rect[5].top + 5, m_Rect[5].right - 5, m_Rect[5].top + 25);
	m_CTShowFullInfoButton.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_SHOW_FULL_CTINFO);
	m_CTShowFullInfoButton.SetImage(m_pWndSettings[0].m_SimplifiedWFPanel ?
		(m_SW >= 1920 ? IDR_SHOW_DETAIL_3 : m_SW >= 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
		(m_SW >= 1920 ? IDR_HIDE_DETAIL_3 : m_SW >= 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
	m_CTShowFullInfoButton.SetBk(FALSE);
	m_CTShowFullInfoButton.SetBkColor(BLACK);
	m_CTShowFullInfoButton.SetTip(s);
	m_CTShowFullInfoButton.ShowWindow(SW_SHOW);
	//Done

	ShowWindow(show);
}

//***************************************************************************************

CString CWFCTSumWnd::GetNameImpl() const
{
  if (m_i == 0) {
    return "WF&&CT Chang Analysis Display";
  }
  else if (m_i == 1) {
    return "WF&&CT Summary Display";
  }
  else if (m_i == 2) {
    return "WF&&CT MTF Display";
  }
  else if (m_i == 3) {
    return "WF&&CT Custom Display";
  }
  else if (m_i == 6) {
    return "WF&&CT Astigmatism Source Display";
  }
  else {
    return "WF&&CT Summary Display";
  }
}

void CWFCTSumWnd::OnSizeLargeItemClicked()
{
	CWndSettings* pWndSettings = GetWndSettings();

	pWndSettings->m_SizeSmall = !pWndSettings->m_SizeSmall;

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
	//::TempSettings.Temp_WFCTCA = FALSE;
}

//***************************************************************************************

void CWFCTSumWnd::RepaintMemDC()
{
	CSumWnd::RepaintMemDC();

	//530 To show simplied wf info for Chang Analysis
	BOOL Rows[22] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	if (m_pWndSettings[0].m_SimplifiedWFPanel == 1)
		ChangWFExamInfo(m_pWFExam, m_Rect[4], 1, TRUE);
	else
		WFExamInfo(m_pWFExam, m_Rect[4], 22, Rows);

	if (m_pWndSettings[0].m_SimplifiedCTPanel == 1)
		ChangCTExamInfo(m_pCTExam, m_Rect[5]);
	else
		CTExamInfo(m_pCTExam, m_Rect[5], 22, Rows);
	//530

	CMFont Font(intRound(1.1 * (2.0 * m_g)), 400, "Arial");

	//cjfaaa
	if (!m_isCombo)//520 + 530
	{
		if (m_pWFExam->m_Header.m_FellowExamID != m_pCTExam->m_Header.m_ExamID) {
			RECT Rect = { intRound(0.010 * m_w), intRound(0.060 * m_h), m_w, intRound(0.090 * m_h) };
			m_MemDC.WriteText("Exams not taken at the same time", Rect, Font, m_Printing ? ORANGE : YELLOW, 0);
		}
	}

	//520
	else
	{
		if (m_pWndSettings->m_MapShowAstigmatismAxes)
		{
			CString s, disStr;
			//Calculated where X = Internal flat axis – corneal steep axis.

			m_Axes[0] = (m_Axes[0] + 90) % 180;
			int dis = m_Axes[0] - m_Axes[1];
			disStr.Format(_T("%i"), dis);
			//
			if (dis > 0) s = "Internal flat is +" + disStr + "° from corneal steep";
			else if (dis < 0) s = "Internal flat is " + disStr + "° from corneal steep";
			else s = "Internal flat is 0° from corneal steep";
			CMFont Font1(intRound(1.0 * (2.0 * m_g)), 400, "Arial");//520
			CMFont Font2(intRound(0.8 * (2.0 * m_g)), 400, "Arial");//520

			if (m_pWFExam->m_Header.m_FellowExamID != m_pCTExam->m_Header.m_ExamID)
			{
				RECT Rect = { intRound(0.010 * m_w), intRound(0.060 * m_h), intRound(0.19*m_w), intRound(0.090 * m_h) };
				m_MemDC.WriteText("Exams not taken at the same time", Rect, Font1, m_Printing ? ORANGE : YELLOW, 0);

				RECT Rect2 = { Rect.right, intRound(0.060 * m_h), m_w, intRound(0.090 * m_h) };
				m_MemDC.WriteText(s, Rect2, Font1, m_Printing ? ORANGE : WHITE, 0);
			}
			else
			{
				RECT Rect = { intRound(0.010 * m_w), intRound(0.060 * m_h), m_w, intRound(0.090 * m_h) };
				m_MemDC.WriteText(s, Rect, Font, m_Printing ? ORANGE : WHITE, 0);
			}
		}
		else
		{
			if (m_pWFExam->m_Header.m_FellowExamID != m_pCTExam->m_Header.m_ExamID) {
				RECT Rect = { intRound(0.010 * m_w), intRound(0.060 * m_h), m_w, intRound(0.090 * m_h) };
				m_MemDC.WriteText("Exams not taken at the same time", Rect, Font, m_Printing ? ORANGE : YELLOW, 0);
			}
		}
	}
	//

	//530 show the highlight outline of WF and CT Summary（Chang Analysis)
	if (!m_ShowLargeMap)  //Change Analysis
	{
		COLORREF white = m_Printing ? BLACK : WHITE;

		m_MemDC.DrawRectangle(m_OutLineRect[0], white, NOCOLOR);
		m_MemDC.DrawRectangle(m_OutLineRect[1], white, NOCOLOR);
		m_MemDC.DrawRectangle(m_OutLineRect[2], white, NOCOLOR);
		m_MemDC.DrawRectangle(m_OutLineRect[3], white, NOCOLOR);

		m_MemDC.DrawRectangle(m_OutLineRect[4], white, NOCOLOR);
		m_MemDC.DrawRectangle(m_OutLineRect[5], white, NOCOLOR);
		m_MemDC.DrawRectangle(m_OutLineRect[6], white, NOCOLOR);
		m_MemDC.DrawRectangle(m_OutLineRect[7], white, NOCOLOR);
	}
	//530 Done

	// Draw the glass icon for print out
	if (m_Printing || m_Saving)
	{
		int this_d;
		BOOL SmallSize = TRUE;

		for (int i = 0; i < 3; i++)
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
			for (int i = 0; i < 3; i++)
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

void CWFCTSumWnd::CreateChildWnd()
{
	for (int d = 0; d < 4; d++) SAFE_DELETE(m_pDispWnd[d]);

	// Îïðåäåëèòü êàêèå îêíà ñîçäàþòñÿ, à êàêèå - íåò
	m_ShowLargeMap = FALSE;//530
	BOOL b[4] = { FALSE, FALSE, FALSE, FALSE };
	int d;
	for (d = 0; d < 4; d++) {
		if (m_pWndSettings[d].m_SizeSmall == FALSE) {
			b[d] = TRUE;
			m_ShowLargeMap = TRUE;//530
			break;
		}
	}
	if (d == 4) {
		for (int d = 0; d < 4; d++) b[d] = TRUE;
	}

	// Ïðîâåðèòü òèï ïåðâûõ òð¸õ îêîí
	int type[3][6] = {
		{ TYPE_IWFM, TYPE_IRMS, TYPE_IPSF, TYPE_IMTF, TYPE_ILTR, TYPE_IEEF },
		{ TYPE_CWFM, TYPE_CRMS, TYPE_CPSF, TYPE_CMTF, TYPE_CLTR, TYPE_CEEF },
		{ TYPE_TWFM, TYPE_TRMS, TYPE_TPSF, TYPE_TMTF, TYPE_TLTR, TYPE_TEEF }
	};
	int k = 0;
	for (; k < 6; k++) {
		if (m_pWndSettings[0].m_Type == type[0][k]) break;
	}
	if (k == 6) {
		m_pWndSettings[0].m_Type = TYPE_IWFM;
		m_pWndSettings[1].m_Type = TYPE_CWFM;
		m_pWndSettings[2].m_Type = TYPE_TWFM;
	}

	if (0 <= m_d && m_d < 3) {
		CWndSettings *pWndSettings = &m_pWndSettings[m_d];
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

	// Ñèíõðîíèçèðîâàòü
	k = (m_d < 0 || m_d > 2) ? 0 : m_d;

	if (m_pWndSettings[k].m_Type != TYPE_TOPM)//531
	{
		for (int d = 0; d < 3; d++)
		{
			if (d != k && m_pWndSettings[d].m_Type != TYPE_TOPM)
			{
				if (m_pWndSettings[k].m_Type == type[k][1]) m_pWndSettings[d].m_Type = type[d][1];
				else if (m_pWndSettings[k].m_Type == type[k][2]) m_pWndSettings[d].m_Type = type[d][2];
				else if (m_pWndSettings[k].m_Type == type[k][3]) m_pWndSettings[d].m_Type = type[d][3];
				else if (m_pWndSettings[k].m_Type == type[k][4]) m_pWndSettings[d].m_Type = type[d][4];
				else if (m_pWndSettings[k].m_Type == type[k][5]) m_pWndSettings[d].m_Type = type[d][5];
				else                                             m_pWndSettings[d].m_Type = type[d][0];

				m_pWndSettings[d].m_Method3D = m_pWndSettings[k].m_Method3D;
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
				m_pWndSettings[d].m_RMSIndividual = m_pWndSettings[k].m_RMSIndividual;
				m_pWndSettings[d].m_ShowEEF = m_pWndSettings[k].m_ShowEEF;
				m_pWndSettings[d].m_EEFType = m_pWndSettings[k].m_EEFType;
				m_pWndSettings[d].m_MTFAverage = m_pWndSettings[k].m_MTFAverage;
				m_pWndSettings[d].m_MTFAngle = m_pWndSettings[k].m_MTFAngle;
				m_pWndSettings[d].m_LtrLine = m_pWndSettings[k].m_LtrLine;
				//
				m_pWndSettings[d].m_Ltr = m_pWndSettings[k].m_Ltr;
				m_pWndSettings[d].m_LtrOrientation = m_pWndSettings[k].m_LtrOrientation;
				//
			}
		}
	}

	// corneal wavefront surface
	m_Surfaces[1] = m_pCTExam->m_WfSurface;
	// total wavefront surface
	m_Surfaces[2] = m_pWFExam->m_WfSurface;

	real_t r_max_um = 5000.0;
	r_max_um = __min(r_max_um, m_Surfaces[1].m_r_max_um);
	r_max_um = __min(r_max_um, m_Surfaces[2].m_r_max_um);
	r_max_um = __min(r_max_um, m_pWndSettings[0].m_ZoneRadiusUm);

	m_Surfaces[1].ChangeRMaxUm(r_max_um);
	m_Surfaces[1] &= m_pWndSettings[0].m_Mask;
	m_Surfaces[1].ComputePiston(m_pWndSettings[0].m_Piston);
	m_Surfaces[1].ComputeTilt(m_pWndSettings[0].m_Tilt);

	m_Surfaces[2].ChangeRMaxUm(r_max_um);
	m_Surfaces[2] &= m_pWndSettings[0].m_Mask;
	m_Surfaces[2].ComputePiston(m_pWndSettings[0].m_Piston);
	m_Surfaces[2].ComputeTilt(m_pWndSettings[0].m_Tilt);

	// internal optics wavefront surface
	m_Surfaces[0] = m_Surfaces[2];
	m_Surfaces[0] -= m_Surfaces[1];

	//531 surface for correct dysfunctional function;
	m_ReMTF_Surfaces = m_Surfaces[0];
	for (int mode = 6; mode <= 27; mode++)
	{
		real_t um1 = m_Surfaces[0].GetCUm(mode);
		real_t um2 = m_Surfaces[2].GetCUm(mode);

		real_t um = (fabs(um1) <= fabs(um2) ? um1 : um2);

		if (mode == 24)
		{
			um = (um > 0.2 ? um - 0.2 : 0);
		}

		m_ReMTF_Surfaces.SetCUm(mode, um);
	}
	//531 Done

	real_t SphEq[3], Sph[3], Cyl[3];
	int Axes[3];
	for (int i = 0; i < 3; i++) {
		CWFExam::GetSpheqSphCylAxis(m_Surfaces[i], 0, TRUE, SphEq[i], Sph[i], Cyl[i], Axes[i]);

		//520
		m_Axes[i] = Axes[i];
		//520
	}

	CString s[3] = { "Internal Optics", "Cornea", "Total Eye" };
	CString s2[3] = { "INTERNAL", "CORNEA", "TOTAL EYE" };//530

	for (int d = 0; d < 3; d++) {
		if (b[d]) {
			CWndSettings* pWndSettings = m_pWndSettings + d;
			RECT Rect = pWndSettings->m_SizeSmall ? m_Rect[d] : m_LargeRect;
			switch (pWndSettings->m_Type) {

				//531
			case TYPE_TOPM:
			{
				CEyeWnd* pEyeWnd = new CEyeWnd(Rect, this);
				pEyeWnd->m_Type = pWndSettings->m_Type;
				pEyeWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
				pEyeWnd->m_Method3D = pWndSettings->m_Method3D;
				pEyeWnd->m_LTLabel[0] = "TOTAL EYE - Opacity Map";
				//pEyeWnd->m_LTLabel[0] += (m_d==2?" X":(m_d==3?" Y":""));
				pEyeWnd->m_MapShowEye = pWndSettings->m_MapShowEye;
				pEyeWnd->m_MapShowMap = pWndSettings->m_MapShowMap;
				pEyeWnd->m_MapShowNumbers = pWndSettings->m_MapShowNumbers;
				pEyeWnd->m_MapShowPupil = pWndSettings->m_MapShowPupil;
				pEyeWnd->m_MapShowCornealVertex = pWndSettings->m_MapShowCornealVertex;
				pEyeWnd->m_MapShowLimbus = pWndSettings->m_MapShowLimbus;
				pEyeWnd->m_MapTranslucent = pWndSettings->m_MapTranslucent;
				pEyeWnd->m_Unit = Unit::none;
				pEyeWnd->m_Inc = pWndSettings->GetIncrement();
				CScale* pScale = ::NewSettings.m_DysfuncSettings[2].GetScale();//The color is different
				pEyeWnd->m_Cent = 0.5;
				pEyeWnd->m_Step = 1.0 / (pScale->m_NumColors - 1);
				pEyeWnd->m_NumColors = pScale->m_NumColors;
				for (int i = 0; i < pScale->m_NumColors; i++) {
					pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
				}
				//531abc
				pEyeWnd->m_pWFExam = m_pWFExam;

				//531
				if (m_pWFExam->m_DetectorType == DT_LINEAR_ARRAYS)
				{
					m_pWFExam->CreateOpm2D(pEyeWnd->m_Map2D, 1.0 / pEyeWnd->m_y_px_um, m_pWFExam->m_CurShot, 2);//m_d==1?2:m_d==2?0:1);

					real_t mean, std_dev;
					pEyeWnd->m_Map2D.ComputeMeanStdDev(mean, std_dev);

					CString Grade = "";

					if (mean >= 0.8) Grade = "0";
					else if (mean >= 0.7 && mean < 0.8) Grade = "1";
					else if (mean >= 0.6 && mean < 0.7) Grade = "2";
					else if (mean >= 0.5 && mean < 0.6) Grade = "3";
					else if (mean >= 0.3 && mean < 0.5) Grade = "4";
					else if (mean < 0.3)               Grade = "5";

					CString ss;
					real_t val = (1 - mean) * 2;// 531.8
					ss.Format(_T("Index = %.2f"), val);// 531.8
													   //ss.Format(_T("Index = %.2f",  mean);
					pEyeWnd->m_LTLabel[1] = ss;
					ss = "Grade = " + Grade;
					pEyeWnd->m_LTLabel[2] = ss;
				}
				else
				{
					pEyeWnd->m_LTLabel[1] = "Grade is unavailable";
				}
				//531

				m_pDispWnd[d] = pEyeWnd;
			}
			break;
			//531 Done

			case TYPE_IWFM:
			case TYPE_CWFM:
			case TYPE_TWFM:
			{
				CEyeWnd* pEyeWnd = new CEyeWnd(Rect, this);
				pEyeWnd->m_Type = pWndSettings->m_Type;
				pEyeWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
				pEyeWnd->m_Method3D = pWndSettings->m_Method3D;
				//pEyeWnd->m_LTLabel[0] = "Wavefront Map - " + s[d];
				pEyeWnd->m_LTLabel[0] = s2[d] + " - Wavefront Map";
				pWndSettings->m_Mask.GetName(pEyeWnd->m_LTLabel[1]);
				pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
				pEyeWnd->m_MapShowEye = pWndSettings->m_MapShowEye;
				pEyeWnd->m_MapShowMap = pWndSettings->m_MapShowMap;
				pEyeWnd->m_MapShowNumbers = pWndSettings->m_MapShowNumbers;
				pEyeWnd->m_MapShowPupil = pWndSettings->m_MapShowPupil;
				pEyeWnd->m_MapShowCornealVertex = pWndSettings->m_MapShowCornealVertex;
				pEyeWnd->m_MapShowLimbus = pWndSettings->m_MapShowLimbus;
				pEyeWnd->m_MapShowSolidSurface = pWndSettings->m_MapShowSolidSurface;
				pEyeWnd->m_MapShowWireMesh = pWndSettings->m_MapShowWireMesh;
				pEyeWnd->m_MapShowAstigmatismAxes = pWndSettings->m_MapShowAstigmatismAxes;
				pEyeWnd->m_MapTranslucent = pWndSettings->m_MapTranslucent;
				pEyeWnd->m_MapShowBLUEAstigmatismAxes = FALSE;//
				pEyeWnd->m_MapShowREDAstigmatismAxes = FALSE;//
				pEyeWnd->m_DonotShow90Degree = TRUE;//
				pEyeWnd->m_astigm_axis = Axes[d];
				pEyeWnd->m_Unit = Unit::micron;
				pEyeWnd->m_Inc = pWndSettings->GetIncrement();
				CScale* pScale = pWndSettings->GetScale();
				pEyeWnd->m_NumColors = pScale->m_NumColors;

				for (int i = 0; i < pScale->m_NumColors; i++) {
					pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
				}
				if (d == 1) {
					pEyeWnd->m_pCTExam = m_pCTExam;
				}
				else if (d == 2) {
					pEyeWnd->m_pWFExam = m_pWFExam;
				}
				if (d == 1) {
					if (pWndSettings->m_Method3D) {
						m_pCTExam->CreateWfm3D(pEyeWnd->m_Map3D, m_Surfaces[d]);
					}
					else {
						m_pCTExam->CreateWfm2D(pEyeWnd->m_Map2D, m_Surfaces[d], 1.0 / pEyeWnd->m_y_px_um);
					}
				}
				else if (d == 0 || d == 2) {
					if (pWndSettings->m_Method3D) {
						CWFExam::CreateWfm3D(pEyeWnd->m_Map3D, m_Surfaces[d]);
					}
					else {
						CWFExam::CreateWfm2D(pEyeWnd->m_Map2D, m_Surfaces[d], 1.0 / pEyeWnd->m_y_px_um);
					}
				}
				pEyeWnd->CreateCentButtons();
				pEyeWnd->CreateZoomButtons();
				pEyeWnd->CreateZoneButtons();
				pEyeWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 1);//531
				m_pDispWnd[d] = pEyeWnd;
			}
			break;
			case TYPE_IPSF:
			case TYPE_CPSF:
			case TYPE_TPSF:
			{
				CPSFWnd* pPSFWnd = new CPSFWnd(Rect, this);
				CWFExam::CreatePsf(m_Surfaces[d], pPSFWnd->m_Psf, pWndSettings->m_EEFType);
				pPSFWnd->m_Type = pWndSettings->m_Type;
				pPSFWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
				pPSFWnd->m_Method3D = pWndSettings->m_Method3D;
				//pPSFWnd->m_LTLabel[0] = "Point Spread Function - " + s[d];
				pPSFWnd->m_LTLabel[0] = s2[d] + " - Point Spread Function";
				pWndSettings->m_Mask.GetName(pPSFWnd->m_LTLabel[1]);
				pPSFWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
				pPSFWnd->m_ShowEEF = pWndSettings->m_ShowEEF;
				pPSFWnd->CreateZoomButtons();
				pPSFWnd->CreateZoneButtons();
				pPSFWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 1);//531
				m_pDispWnd[d] = pPSFWnd;
			}
			break;
			case TYPE_IMTF:
			case TYPE_CMTF:
			case TYPE_TMTF:
			{
				CMTFWnd* pMTFWnd = new CMTFWnd(Rect, this);

				//531
				if (pWndSettings->m_Type == TYPE_IMTF)
				{
					CWFExam::CreateMtf(m_ReMTF_Surfaces, pMTFWnd->m_ReMtf);
				}
				//531 Done

				pMTFWnd->m_ShowDys = FALSE;//531

				CWFExam::CreateMtf(m_Surfaces[d], pMTFWnd->m_Mtf);
				pMTFWnd->m_Type = pWndSettings->m_Type;
				pMTFWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
				pMTFWnd->m_Method3D = pWndSettings->m_Method3D;
				//pMTFWnd->m_LTLabel[0] = "Modulation Transfer Function - " + s[d];
				/*pMTFWnd->m_LTLabel[0] = s2[d] + " - Modulation Transfer Function";*/
				pMTFWnd->m_LTLabel[0] = s2[d] + " - MTF";//531
				pWndSettings->m_Mask.GetName(pMTFWnd->m_LTLabel[1]);
				pMTFWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
				pMTFWnd->m_Average = pWndSettings->m_MTFAverage;
				pMTFWnd->m_Angle = pWndSettings->m_MTFAngle;
				if (!pWndSettings->m_Method3D && !pWndSettings->m_MTFAverage) {
					pMTFWnd->CreateSlider();
				}
				pMTFWnd->CreateZoneButtons();

				pMTFWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 1);//531
				m_pDispWnd[d] = pMTFWnd;
			}
			break;
			case TYPE_IRMS:
			case TYPE_CRMS:
			case TYPE_TRMS:
			{
				//530
				CRMSWnd* pRMSWnd = new CRMSWnd(Rect, this);
				pRMSWnd->m_SimpliedRMS = pWndSettings->m_SimplifiedRMS;//530

				if (pWndSettings->m_SimplifiedRMS == 1)
				{
					pRMSWnd->m_SIMLTLabel[0] = s2[d];
					pWndSettings->m_Mask.GetName(pRMSWnd->m_SIMLTLabel[1]);
					pRMSWnd->m_SIMLTLabel[1] = " - RMS " + pRMSWnd->m_SIMLTLabel[1];
					pRMSWnd->m_SIMLTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
				}
				//530

				pRMSWnd->m_Type = pWndSettings->m_Type;
				pRMSWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
				pRMSWnd->m_Method3D = pWndSettings->m_Method3D;

				//pRMSWnd->m_LTLabel[0] = "Root Mean Square - " + s[d];
				pRMSWnd->m_LTLabel[0] = s2[d] + " - Root Mean Square";

				pWndSettings->m_Mask.GetName(pRMSWnd->m_LTLabel[1]);
				pRMSWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
				pRMSWnd->m_Surface = m_Surfaces[d];
				pRMSWnd->m_Individual = pWndSettings->m_RMSIndividual;
				if (!pRMSWnd->m_Method3D && pRMSWnd->m_Individual) pRMSWnd->CreateSimpButtons(!pWndSettings->m_SimplifiedRMS);//530
				pRMSWnd->CreateZoomButtons();
				pRMSWnd->CreateZoneButtons();
				pRMSWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 1);//531
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
				//pLtrWnd->m_LTLabel[0].Format(_T("Letter 20 / %i - %s"), pWndSettings->m_LtrLine, s[d]);
				//pLtrWnd->m_LTLabel[0].Format(_T("%s - Letter 20 / %i", s2[d], pWndSettings->m_LtrLine);
				pLtrWnd->m_LTLabel[0].Format(_T("%s - Letter"), s2[d]);//531
				pWndSettings->m_Mask.GetName(pLtrWnd->m_LTLabel[1]);
				pLtrWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
				pLtrWnd->CreateZoomButtons();
				pLtrWnd->CreateZoneButtons();
				pLtrWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 1);//531
				pLtrWnd->m_DysVal = -1;//531

				m_pDispWnd[d] = pLtrWnd;
			}
			break;
			case TYPE_IEEF:
			case TYPE_CEEF:
			case TYPE_TEEF:
			{
				CEEFWnd* pEEFWnd = new CEEFWnd(Rect, this);
				CWFExam::CreatePsf(m_Surfaces[d], pEEFWnd->m_Psf, pWndSettings->m_EEFType);
				pEEFWnd->m_Type = pWndSettings->m_Type;
				pEEFWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
				pEEFWnd->m_Method3D = pWndSettings->m_Method3D;
				//pEEFWnd->m_LTLabel[0] = "Encircled Energy Function - " + s[d];
				pEEFWnd->m_LTLabel[0] = s2[d] + " - Encircled Energy Function";
				pWndSettings->m_Mask.GetName(pEEFWnd->m_LTLabel[1]);
				pEEFWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
				pEEFWnd->CreateZoneButtons();
				pEEFWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 1);//531


				m_pDispWnd[d] = pEEFWnd;
			}
			break;
			}
		}
	}

	// ñèíõðîíèçèðîâàòü map scales
	{
		CWndSettings* pWndSettings = m_pWndSettings + 0;
		if (pWndSettings->m_Type == TYPE_IWFM)
		{
			CScale* pScale = pWndSettings->GetScale();
			real_t Inc = pWndSettings->GetIncrement();
			// middle value
			real_t Cent = 0.0;
			// step value
			real_t Step;
			if (pScale->m_StepAuto) {
				real_t Span = 0.0;
				for (d = 0; d < 3; d++)
				{
					if (b[d] && m_pWndSettings[d].m_Type != TYPE_TOPM)
					{
						CEyeWnd* pEyeWnd = (CEyeWnd*)m_pDispWnd[d];
						real_t Min = pWndSettings->m_Method3D ? pEyeWnd->m_Map3D.m_min : pEyeWnd->m_Map2D.m_min;
						real_t Max = pWndSettings->m_Method3D ? pEyeWnd->m_Map3D.m_max : pEyeWnd->m_Map2D.m_max;
						Span = __max(Span, 2.0 * __max(fabs(Min), fabs(Max)));
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
			for (d = 0; d < 3; d++) {
				if (b[d] && m_pWndSettings[d].m_Type != TYPE_TOPM) {
					CEyeWnd* pEyeWnd = (CEyeWnd*)m_pDispWnd[d];
					pEyeWnd->m_Cent = Cent;
					pEyeWnd->m_Step = Step;
				}
			}
		}
		else if (pWndSettings->m_Type == TYPE_IRMS)
		{
			//531 RMS Set scale to 1
			for (d = 0; d < 3; d++)
			{
				if (b[d])
				{
					((CRMSWnd*)m_pDispWnd[d])->m_Max = 1;

					((CRMSWnd*)m_pDispWnd[d])->m_ZoomInButton.SetEnabled(FALSE);
					((CRMSWnd*)m_pDispWnd[d])->m_ZoomInButton.Repaint();
				}
			}
			//531 RMS Set scale to 1 Done

			//    int MaxMax = 1;
			//    for (d = 0; d < 3; d++)
			// {
			//      if (b[d] && m_pWndSettings[d].m_Type != TYPE_TOPM)
			//{
			//        CRMSWnd* pRMSWnd = (CRMSWnd*)m_pDispWnd[d];
			//        real t = pRMSWnd->m_Surface.ComputeRms();
			//        int Max = (int)t;
			//        if (t > Max + 0.001) Max++;
			//        MaxMax = __max(MaxMax, Max);
			//      }
			//    }
			//    for (d = 0; d < 3; d++)
			// {
			//  if (b[d] && m_pWndSettings[d].m_Type != TYPE_TOPM)
			//  {
			//	  ((CRMSWnd*)m_pDispWnd[d])->m_Max = 1;

			//	  //531
			//	  if(MaxMax == 1)
			//	  {
			//	   ((CRMSWnd*)m_pDispWnd[d])->m_ZoomInButton.SetEnabled(FALSE);
			//	   ((CRMSWnd*)m_pDispWnd[d])->m_ZoomInButton.Repaint();
			//	  }
			//      //531 Done
			//  }
			//    }
		}
	}

	// quadrant 3
	if (b[3]) {
		CWndSettings* pWndSettings = m_pWndSettings + 3;
		RECT Rect = pWndSettings->m_SizeSmall ? m_Rect[3] : m_LargeRect;
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
			m_pDispWnd[3] = pEyeWnd;
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
			if (pWndSettings->m_Type == TYPE_CAXM) {
				pEyeWnd->m_LTLabel[0] = "Axial Map";
				if (m_i == 0) { // Chang Analysis
					pEyeWnd->m_RBLabelColor[0] = m_Printing ? BLACK : WHITE;
					pEyeWnd->m_RBLabelColor[1] = pEyeWnd->m_RBLabelColor[0];
					pEyeWnd->m_RBLabelColor[2] = pEyeWnd->m_RBLabelColor[0];
					pEyeWnd->m_RBLabelColor[3] = pEyeWnd->m_RBLabelColor[0];
					pEyeWnd->m_RBLabelColor[4] = pEyeWnd->m_RBLabelColor[0];
					pEyeWnd->m_RBLabelColor[5] = pEyeWnd->m_RBLabelColor[0];
					pEyeWnd->m_RBLabelBkColor = m_Printing ? WHITE : BLACK;
					pEyeWnd->m_RBLabel[1] = "Avg K";
					for (int k = 0; k <= 3; k++) {
						if (m_pCTExam->m_zk_ok[k]) {
							pEyeWnd->m_RBLabel[2 + k].Format(_T("%.2f D @ D = %i mm"), m_pCTExam->m_zk_av_dp[k], k);
						}
					}
				}
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
			pEyeWnd->m_MapShowKeratometry = pWndSettings->m_MapShowKeratometry;
			pEyeWnd->m_MapShowSimKAxes = pWndSettings->m_MapShowSimKAxes;
			pEyeWnd->m_Unit = MapUnit == DIOPTERS ? Unit::D : Unit::mm;
			pEyeWnd->m_Inc = pWndSettings->GetIncrement();
			CScale* pScale = pWndSettings->GetScale();
			pEyeWnd->m_NumColors = pScale->m_NumColors;

			for (int i = 0; i < pScale->m_NumColors; i++)
			{
				pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
			}

			pEyeWnd->m_pCTExam = m_pCTExam;

			if (pWndSettings->m_Method3D)
			{
				m_pCTExam->CreateDpm3D(pEyeWnd->m_Map3D, pWndSettings->m_Type, MapUnit);
			}
			else
			{
				m_pCTExam->CreateDpm2D(pEyeWnd->m_Map2D, pWndSettings->m_Type, MapUnit, 1.0 / pEyeWnd->m_y_px_um);
				pEyeWnd->m_Circles.Create(1);
				pEyeWnd->m_Circles[0].m_x_um = 0.0;
				pEyeWnd->m_Circles[0].m_y_um = 0.0;
				pEyeWnd->m_Circles[0].m_r_um = r_max_um;
				pEyeWnd->m_Circles[0].m_color = BLACK;
			}
			pEyeWnd->CreateCentButtons();
			pEyeWnd->CreateZoomButtons();
			m_pDispWnd[3] = pEyeWnd;
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
			pEyeWnd->m_MapShowLimbus = pWndSettings->m_MapShowLimbus;
			pEyeWnd->m_MapShowPupil = pWndSettings->m_MapShowPupil;
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
			pEyeWnd->m_pCTExam = m_pCTExam;
			if (pWndSettings->m_Method3D) {
				m_pCTExam->CreateElm3D(pEyeWnd->m_Map3D, RefSurface);
			}
			else {
				m_pCTExam->CreateElm2D(pEyeWnd->m_Map2D, RefSurface, 1.0 / pEyeWnd->m_y_px_um);
				pEyeWnd->m_Circles.Create(1);
				pEyeWnd->m_Circles[0].m_x_um = 0.0;
				pEyeWnd->m_Circles[0].m_y_um = 0.0;
				pEyeWnd->m_Circles[0].m_r_um = r_max_um;
				pEyeWnd->m_Circles[0].m_color = BLACK;
			}
			pEyeWnd->CreateCentButtons();
			pEyeWnd->CreateZoomButtons();
			m_pDispWnd[3] = pEyeWnd;
		}
		break;
		}

		if (pWndSettings->m_Type == TYPE_CAXM || pWndSettings->m_Type == TYPE_CTNM || pWndSettings->m_Type == TYPE_CRFM || pWndSettings->m_Type == TYPE_CELM) {
			CScale* pScale = pWndSettings->GetScale();
			real_t Inc = pWndSettings->GetIncrement();
			CEyeWnd* pEyeWnd = (CEyeWnd*)m_pDispWnd[3];
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
	}


	//521
	if (m_d < 3 && m_pWndSettings[m_d].m_Type != TYPE_TOPM)
	{
		if (!m_pWndSettings[m_d].m_SizeSmall)
		{
			if (m_pWndSettings[m_d].m_ZoneRadiusUm >= ::TempSettings.Com_r_max_um)
			{
				m_pDispWnd[m_d]->m_IncZoneButton.SetEnabled(FALSE);
				m_pDispWnd[m_d]->m_IncZoneButton.Repaint();
			}

			if (m_pWndSettings[m_d].m_ZoneRadiusUm == 500)
			{
				m_pDispWnd[m_d]->m_DecZoneButton.SetEnabled(FALSE);
				m_pDispWnd[m_d]->m_DecZoneButton.Repaint();
			}
		}
		else
		{
			for (int d = 0; d < 3; d++)
			{
				if (b[d])
				{
					if (m_pWndSettings[d].m_Type != TYPE_TOPM)
					{
						if (m_pWndSettings[d].m_ZoneRadiusUm >= ::TempSettings.Com_r_max_um)
						{
							m_pDispWnd[d]->m_IncZoneButton.SetEnabled(FALSE);
							m_pDispWnd[d]->m_IncZoneButton.Repaint();
						}

						if (m_pWndSettings[d].m_ZoneRadiusUm == 500)
						{
							m_pDispWnd[d]->m_DecZoneButton.SetEnabled(FALSE);
							m_pDispWnd[d]->m_DecZoneButton.Repaint();
						}
					}
				}
			}
		}
	}
	//521
}

//***************************************************************************************

LRESULT CWFCTSumWnd::OnRotateMsg(WPARAM wParam, LPARAM lParam)
{
	int ax, ay, d;

	for (d = 0; d < 3; d++) {
		if ((WPARAM)m_pDispWnd[d] == wParam && m_pWndSettings[d].m_Type != TYPE_TOPM) {
			ax = m_pDispWnd[d]->m_ax;
			ay = m_pDispWnd[d]->m_ay;
			break;
		}
	}
	if (d == 3) return 0;

	for (d = 0; d < 3; d++) {
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] != wParam && m_pWndSettings[d].m_Type != TYPE_TOPM) {
			m_pDispWnd[d]->m_ax = ax;
			m_pDispWnd[d]->m_ay = ay;
			m_pDispWnd[d]->Repaint();
		}
	}

	return 0;
}

//***************************************************************************************

LRESULT CWFCTSumWnd::OnChangeCentMsg(WPARAM wParam, LPARAM lParam)
{
	real_t Cent = INVALID_VALUE;

	for (int d = 0; d < 3; d++) {
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] == wParam && m_pWndSettings[d].m_Type != TYPE_TOPM) {
			Cent = ((CEyeWnd*)m_pDispWnd[d])->m_Cent;
			break;
		}
	}

	if (Cent == INVALID_VALUE) return 0;

	for (int d = 0; d < 3; d++) {
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] != wParam && m_pWndSettings[d].m_Type != TYPE_TOPM) {
			((CEyeWnd*)m_pDispWnd[d])->m_Cent = Cent;
			m_pDispWnd[d]->Repaint();
		}
	}

	return 0;
}

//***************************************************************************************

LRESULT CWFCTSumWnd::OnChangeStepMsg(WPARAM wParam, LPARAM lParam)
{
	real_t Step = INVALID_VALUE;

	for (int d = 0; d < 3; d++) {
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] == wParam && m_pWndSettings[d].m_Type != TYPE_TOPM) {
			Step = ((CEyeWnd*)m_pDispWnd[d])->m_Step;
			break;
		}
	}

	if (Step == INVALID_VALUE) return 0;

	for (int d = 0; d < 3; d++) {
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] != wParam&& m_pWndSettings[d].m_Type != TYPE_TOPM) {
			((CEyeWnd*)m_pDispWnd[d])->m_Step = Step;
			m_pDispWnd[d]->Repaint();
		}
	}

	return 0;
}

//***************************************************************************************
//530
LRESULT CWFCTSumWnd::OnChangeRmsGUI(WPARAM wParam, LPARAM lParam)
{
	BOOL Simple = FALSE;
	int  Sel_d = 0;

	for (int d = 0; d < 3; d++) {
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] == wParam && m_pWndSettings[d].m_Type != TYPE_TOPM) //531
		{
			Simple = !m_pWndSettings[d].m_SimplifiedRMS;
			Sel_d = d;
			break;
		}
	}

	int W = ::GetSystemMetrics(SM_CXSCREEN);
	if (m_pWndSettings[Sel_d].m_SizeSmall)
	{
		for (int s_d = 0; s_d < 3; s_d++)
		{
			if (m_pWndSettings[s_d].m_Type != TYPE_TOPM) //531
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
		}
	}
	else
	{
		for (int s_d = 0; s_d < 3; s_d++)
		{
			m_pWndSettings[s_d].m_SimplifiedRMS = Simple;
		}

		((CRMSWnd*)m_pDispWnd[Sel_d])->m_SimpliedRMS = Simple;

		CString s = (Simple ? "Show Details" : "Hide Details");
		m_pDispWnd[Sel_d]->m_ShowMoreButton.SetTip(s);
		m_pDispWnd[Sel_d]->m_ShowMoreButton.SetImage(Simple ?
			(W >= 1920 ? IDR_SHOW_DETAIL_6 : W >= 1600 ? IDR_SHOW_DETAIL_5 : IDR_SHOW_DETAIL_4) :
			(W >= 1920 ? IDR_HIDE_DETAIL_6 : W >= 1600 ? IDR_HIDE_DETAIL_5 : IDR_HIDE_DETAIL_4));
		m_pDispWnd[Sel_d]->m_ShowMoreButton.Repaint();
		m_pDispWnd[Sel_d]->Repaint();
	}
	return 0;
}
//530
//***************************************************************************************
LRESULT CWFCTSumWnd::OnChangeMaxMsg(WPARAM wParam, LPARAM lParam)
{
	int Max = INVALID_VALUE;

	for (int d = 0; d < 3; d++) {
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] == wParam && m_pWndSettings[d].m_Type != TYPE_TOPM) {
			Max = ((CRMSWnd*)m_pDispWnd[d])->m_Max;
			break;
		}
	}

	if (Max == INVALID_VALUE) return 0;

	for (int d = 0; d < 3; d++) {
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] != wParam && m_pWndSettings[d].m_Type != TYPE_TOPM) {
			((CRMSWnd*)m_pDispWnd[d])->m_Max = Max;

			if (Max == 1) ((CRMSWnd*)m_pDispWnd[d])->m_ZoomInButton.SetEnabled(FALSE);
			else         ((CRMSWnd*)m_pDispWnd[d])->m_ZoomInButton.SetEnabled(TRUE);
			((CRMSWnd*)m_pDispWnd[d])->m_ZoomInButton.Repaint();

			m_pDispWnd[d]->Repaint();
		}
	}

	return 0;
}

//***************************************************************************************

void CWFCTSumWnd::CreateMenu()
{
	CWndSettings* pWndSettings = m_pWndSettings + m_d;
	CScale* pScale = pWndSettings->GetScale();
	CXPMenu* pSubMenu;

	if (pWndSettings->m_Method3D) pWndSettings->m_RMSIndividual = TRUE;

	switch (m_d) {

	case 0:
		// type
		m_pMenu->AppendItem(IDC_TYPE_IWFM_ITEM, pWndSettings->m_Type == TYPE_IWFM, FALSE, "Wavefront Map", "");
		{
			m_pMenu->AppendItem(IDC_TYPE_IPSF_ITEM, pWndSettings->m_Type == TYPE_IPSF, FALSE, "Point Spread Function", "");
			m_pMenu->AppendItem(IDC_TYPE_IMTF_ITEM, pWndSettings->m_Type == TYPE_IMTF, FALSE, "Modulation Transfer Function", "");
			m_pMenu->AppendItem(IDC_TYPE_ILTR_ITEM, pWndSettings->m_Type == TYPE_ILTR, FALSE, "Visual Acuity Chart", "");
			m_pMenu->AppendItem(IDC_TYPE_IEEF_ITEM, pWndSettings->m_Type == TYPE_IEEF, FALSE, "Encircled Energy Function", "");
		}
		m_pMenu->AppendItem(IDC_TYPE_IRMS_ITEM, pWndSettings->m_Type == TYPE_IRMS, FALSE, "Root Mean Square", "");
		m_pMenu->AppendSeparator();
		// method// || IsCombo
		//if (IsWorkstation)
		{
			if (pWndSettings->m_Type == TYPE_IWFM ||
				pWndSettings->m_Type == TYPE_IRFM ||
				pWndSettings->m_Type == TYPE_IPSF ||
				pWndSettings->m_Type == TYPE_IMTF ||
				pWndSettings->m_Type == TYPE_ILTR ||
				pWndSettings->m_Type == TYPE_IRMS && pWndSettings->m_RMSIndividual) {
				m_pMenu->AppendItem(IDC_METHOD_3D_ITEM, FALSE, pWndSettings->m_Method3D, "3D", "");
			}
		}
		// size
		m_pMenu->AppendItem(IDC_SIZE_LARGE_ITEM, FALSE, !pWndSettings->m_SizeSmall, "Large window", "");
		m_pMenu->AppendSeparator();
		// zone & mask
		CreateSubmenuZone(2);
		CreateSubmenuMask();
		//
		if (pWndSettings->m_Type == TYPE_IWFM || pWndSettings->m_Type == TYPE_IRFM) {
			m_pMenu->AppendSeparator();
			if (pWndSettings->m_Method3D) {
				m_pMenu->AppendItem(IDC_SHOW_MAP_SURFACE_ITEM, FALSE, pWndSettings->m_MapShowSolidSurface, "Show solid surface", "");
				m_pMenu->AppendItem(IDC_SHOW_MAP_MESH_ITEM, FALSE, pWndSettings->m_MapShowWireMesh, "Show wire mesh", "");
			}
			else {
				m_pMenu->AppendItem(IDC_SHOW_MAP_MAP_ITEM, FALSE, pWndSettings->m_MapShowMap, "Show map", "");
				m_pMenu->AppendItem(IDC_SHOW_MAP_NUMBERS_ITEM, FALSE, pWndSettings->m_MapShowNumbers, "Show numbers", "");
				m_pMenu->AppendItem(IDC_SHOW_MAP_ASTIGM_AXES_ITEM, FALSE, pWndSettings->m_MapShowAstigmatismAxes, "Show astigmatism axes", "");
			}
			m_pMenu->AppendSeparator();//521
			CreateSubmenuMapSteps();
			CreateSubmenuMapColors();
		}
		else if (pWndSettings->m_Type == TYPE_IPSF) {
			m_pMenu->AppendSeparator();
			m_pMenu->AppendItem(IDC_SHOW_PSF_EEF_ITEM, FALSE, pWndSettings->m_ShowEEF, "Show 50% energy circle", "");
			if (pWndSettings->m_ShowEEF) {
				if (pWndSettings->m_EEFType != EEF_CNT) pWndSettings->m_EEFType = EEF_MAX;
				m_pMenu->AppendItem(IDC_EEF_TYPE1_ITEM, pWndSettings->m_EEFType == EEF_MAX, FALSE, "Circle around maximum", "");
				m_pMenu->AppendItem(IDC_EEF_TYPE2_ITEM, pWndSettings->m_EEFType == EEF_CNT, FALSE, "Circle around centroid", "");
			}
		}
		else if (pWndSettings->m_Type == TYPE_IMTF) {
			if (!pWndSettings->m_Method3D) {
				m_pMenu->AppendSeparator();
				m_pMenu->AppendItem(IDC_MTF_AVG_ITEM, FALSE, pWndSettings->m_MTFAverage, "Radial average curve", "");
			}
		}
		else if (pWndSettings->m_Type == TYPE_ILTR) {
			m_pMenu->AppendSeparator();
			CreateSubmenuLtr();
		}
		else if (pWndSettings->m_Type == TYPE_IEEF) {
			m_pMenu->AppendSeparator();
			if (pWndSettings->m_EEFType != EEF_CNT) pWndSettings->m_EEFType = EEF_MAX;
			m_pMenu->AppendItem(IDC_EEF_TYPE1_ITEM, pWndSettings->m_EEFType == EEF_MAX, FALSE, "Circle round maximum", "");
			m_pMenu->AppendItem(IDC_EEF_TYPE2_ITEM, pWndSettings->m_EEFType == EEF_CNT, FALSE, "Circle round centroid", "");
		}
		else if (pWndSettings->m_Type == TYPE_IRMS) {
			if (!pWndSettings->m_Method3D) {
				m_pMenu->AppendSeparator();
				m_pMenu->AppendItem(IDC_RMS_CMB_ITEM, FALSE, !pWndSettings->m_RMSIndividual, "Combined", "");
			}
		}

		break;

	case 1:
		// type
		m_pMenu->AppendItem(IDC_TYPE_CWFM_ITEM, pWndSettings->m_Type == TYPE_CWFM, FALSE, "Wavefront Map", "");
		{
			m_pMenu->AppendItem(IDC_TYPE_CPSF_ITEM, pWndSettings->m_Type == TYPE_CPSF, FALSE, "Point Spread Function", "");
			m_pMenu->AppendItem(IDC_TYPE_CMTF_ITEM, pWndSettings->m_Type == TYPE_CMTF, FALSE, "Modulation Transfer Function", "");
			m_pMenu->AppendItem(IDC_TYPE_CLTR_ITEM, pWndSettings->m_Type == TYPE_CLTR, FALSE, "Visual Acuity Chart", "");
			m_pMenu->AppendItem(IDC_TYPE_CEEF_ITEM, pWndSettings->m_Type == TYPE_CEEF, FALSE, "Encircled Energy Function", "");
		}
		m_pMenu->AppendItem(IDC_TYPE_CRMS_ITEM, pWndSettings->m_Type == TYPE_CRMS, FALSE, "Root Mean Square", "");
		m_pMenu->AppendSeparator();
		// method
		{
			if (pWndSettings->m_Type == TYPE_CWFM ||
				pWndSettings->m_Type == TYPE_CPSF ||
				pWndSettings->m_Type == TYPE_CMTF ||
				pWndSettings->m_Type == TYPE_CLTR ||
				pWndSettings->m_Type == TYPE_CRMS && pWndSettings->m_RMSIndividual) {
				m_pMenu->AppendItem(IDC_METHOD_3D_ITEM, FALSE, pWndSettings->m_Method3D, "3D", "");
			}
		}
		// size
		m_pMenu->AppendItem(IDC_SIZE_LARGE_ITEM, FALSE, !pWndSettings->m_SizeSmall, "Large window", "");
		m_pMenu->AppendSeparator();
		// zone & mask
		CreateSubmenuZone(2);
		CreateSubmenuMask();
		//
		if (pWndSettings->m_Type == TYPE_CWFM) {
			m_pMenu->AppendSeparator();
			if (pWndSettings->m_Method3D) {
				m_pMenu->AppendItem(IDC_SHOW_MAP_SURFACE_ITEM, FALSE, pWndSettings->m_MapShowSolidSurface, "Show solid surface", "");
				m_pMenu->AppendItem(IDC_SHOW_MAP_MESH_ITEM, FALSE, pWndSettings->m_MapShowWireMesh, "Show wire mesh", "");
			}
			else {
				m_pMenu->AppendItem(IDC_SHOW_MAP_MAP_ITEM, FALSE, pWndSettings->m_MapShowMap, "Show map", "");
				m_pMenu->AppendItem(IDC_SHOW_MAP_NUMBERS_ITEM, FALSE, pWndSettings->m_MapShowNumbers, "Show numbers", "");
				m_pMenu->AppendItem(IDC_SHOW_MAP_EYE_ITEM, FALSE, pWndSettings->m_MapShowEye, "Show eye image", "");
				m_pMenu->AppendItem(IDC_SHOW_MAP_PUPIL_ITEM, FALSE, pWndSettings->m_MapShowPupil, "Show pupil", "");
				m_pMenu->AppendItem(IDC_SHOW_MAP_LIMBUS_ITEM, FALSE, pWndSettings->m_MapShowLimbus, "Show limbus", "");
				m_pMenu->AppendItem(IDC_SHOW_MAP_ASTIGM_AXES_ITEM, FALSE, pWndSettings->m_MapShowAstigmatismAxes, "Show astigmatism axes", "");
				m_pMenu->AppendItem(IDC_MAP_TRANSLUCENT_ITEM, FALSE, pWndSettings->m_MapTranslucent, "Translucent map", "");
			}
			m_pMenu->AppendSeparator();//521
			CreateSubmenuMapSteps();
			CreateSubmenuMapColors();
		}
		else if (pWndSettings->m_Type == TYPE_CPSF) {
			m_pMenu->AppendSeparator();
			m_pMenu->AppendItem(IDC_SHOW_PSF_EEF_ITEM, FALSE, pWndSettings->m_ShowEEF, "Show 50% energy circle", "");
			if (pWndSettings->m_ShowEEF) {
				if (pWndSettings->m_EEFType != EEF_CNT) pWndSettings->m_EEFType = EEF_MAX;
				m_pMenu->AppendItem(IDC_EEF_TYPE1_ITEM, pWndSettings->m_EEFType == EEF_MAX, FALSE, "Circle around maximum", "");
				m_pMenu->AppendItem(IDC_EEF_TYPE2_ITEM, pWndSettings->m_EEFType == EEF_CNT, FALSE, "Circle around centroid", "");
			}
		}
		else if (pWndSettings->m_Type == TYPE_CMTF) {
			if (!pWndSettings->m_Method3D) {
				m_pMenu->AppendSeparator();
				m_pMenu->AppendItem(IDC_MTF_AVG_ITEM, FALSE, pWndSettings->m_MTFAverage, "Radial average curve", "");
			}
		}
		else if (pWndSettings->m_Type == TYPE_CLTR) {
			m_pMenu->AppendSeparator();
			CreateSubmenuLtr();
		}
		else if (pWndSettings->m_Type == TYPE_CEEF) {
			m_pMenu->AppendSeparator();
			if (pWndSettings->m_EEFType != EEF_CNT) pWndSettings->m_EEFType = EEF_MAX;
			m_pMenu->AppendItem(IDC_EEF_TYPE1_ITEM, pWndSettings->m_EEFType == EEF_MAX, FALSE, "Circle round maximum", "");
			m_pMenu->AppendItem(IDC_EEF_TYPE2_ITEM, pWndSettings->m_EEFType == EEF_CNT, FALSE, "Circle round centroid", "");
		}
		else if (pWndSettings->m_Type == TYPE_CRMS) {
			if (!pWndSettings->m_Method3D) {
				m_pMenu->AppendSeparator();
				m_pMenu->AppendItem(IDC_RMS_CMB_ITEM, FALSE, !pWndSettings->m_RMSIndividual, "Combined", "");
			}
		}

		break;

	case 2:
		// type
		m_pMenu->AppendItem(IDC_TYPE_TWFM_ITEM, pWndSettings->m_Type == TYPE_TWFM, FALSE, "Wavefront Map", "");
		{
			m_pMenu->AppendItem(IDC_TYPE_TPSF_ITEM, pWndSettings->m_Type == TYPE_TPSF, FALSE, "Point Spread Function", "");
			m_pMenu->AppendItem(IDC_TYPE_TMTF_ITEM, pWndSettings->m_Type == TYPE_TMTF, FALSE, "Modulation Transfer Function", "");
			m_pMenu->AppendItem(IDC_TYPE_TLTR_ITEM, pWndSettings->m_Type == TYPE_TLTR, FALSE, "Visual Acuity Chart", "");
			m_pMenu->AppendItem(IDC_TYPE_TEEF_ITEM, pWndSettings->m_Type == TYPE_TEEF, FALSE, "Encircled Energy Function", "");
		}
		m_pMenu->AppendItem(IDC_TYPE_TRMS_ITEM, pWndSettings->m_Type == TYPE_TRMS, FALSE, "Root Mean Square", "");
		//m_pMenu->AppendItem(IDC_TYPE_TOPM_ITEM, pWndSettings->m_Type == TYPE_TOPM, FALSE, "Opacity Map", "");//531
		m_pMenu->AppendSeparator();
		// method
		{
			if (pWndSettings->m_Type == TYPE_TWFM ||
				pWndSettings->m_Type == TYPE_TRFM ||
				pWndSettings->m_Type == TYPE_TPSF ||
				pWndSettings->m_Type == TYPE_TMTF ||
				pWndSettings->m_Type == TYPE_TLTR ||
				pWndSettings->m_Type == TYPE_TRMS && pWndSettings->m_RMSIndividual) {
				m_pMenu->AppendItem(IDC_METHOD_3D_ITEM, FALSE, pWndSettings->m_Method3D, "3D", "");
			}
		}
		// size
		m_pMenu->AppendItem(IDC_SIZE_LARGE_ITEM, FALSE, !pWndSettings->m_SizeSmall, "Large window", "");
		m_pMenu->AppendSeparator();
		// zone & mask
		CreateSubmenuZone(2);
		CreateSubmenuMask();
		//
		if (pWndSettings->m_Type == TYPE_TWFM || pWndSettings->m_Type == TYPE_TRFM) {
			m_pMenu->AppendSeparator();
			if (pWndSettings->m_Method3D) {
				m_pMenu->AppendItem(IDC_SHOW_MAP_SURFACE_ITEM, FALSE, pWndSettings->m_MapShowSolidSurface, "Show solid surface", "");
				m_pMenu->AppendItem(IDC_SHOW_MAP_MESH_ITEM, FALSE, pWndSettings->m_MapShowWireMesh, "Show wire mesh", "");
			}
			else {
				m_pMenu->AppendItem(IDC_SHOW_MAP_MAP_ITEM, FALSE, pWndSettings->m_MapShowMap, "Show map", "");
				m_pMenu->AppendItem(IDC_SHOW_MAP_NUMBERS_ITEM, FALSE, pWndSettings->m_MapShowNumbers, "Show numbers", "");
				m_pMenu->AppendItem(IDC_SHOW_MAP_EYE_ITEM, FALSE, pWndSettings->m_MapShowEye, "Show eye image", "");
				m_pMenu->AppendItem(IDC_SHOW_MAP_PUPIL_ITEM, FALSE, pWndSettings->m_MapShowPupil, "Show pupil", "");
				m_pMenu->AppendItem(IDC_SHOW_MAP_VERTEX_ITEM, FALSE, pWndSettings->m_MapShowCornealVertex, "Show corneal vertex", "");
				m_pMenu->AppendItem(IDC_SHOW_MAP_LIMBUS_ITEM, FALSE, pWndSettings->m_MapShowLimbus, "Show limbus", "");
				m_pMenu->AppendItem(IDC_SHOW_MAP_ASTIGM_AXES_ITEM, FALSE, pWndSettings->m_MapShowAstigmatismAxes, "Show astigmatism axes", "");
				m_pMenu->AppendItem(IDC_MAP_TRANSLUCENT_ITEM, FALSE, pWndSettings->m_MapTranslucent, "Translucent map", "");
			}
			m_pMenu->AppendSeparator();//521
			CreateSubmenuMapSteps();
			CreateSubmenuMapColors();
		}
		else if (pWndSettings->m_Type == TYPE_TPSF) {
			m_pMenu->AppendSeparator();
			m_pMenu->AppendItem(IDC_SHOW_PSF_EEF_ITEM, FALSE, pWndSettings->m_ShowEEF, "Show 50% energy circle", "");
			if (pWndSettings->m_ShowEEF) {
				if (pWndSettings->m_EEFType != EEF_CNT) pWndSettings->m_EEFType = EEF_MAX;
				m_pMenu->AppendItem(IDC_EEF_TYPE1_ITEM, pWndSettings->m_EEFType == EEF_MAX, FALSE, "Circle around maximum", "");
				m_pMenu->AppendItem(IDC_EEF_TYPE2_ITEM, pWndSettings->m_EEFType == EEF_CNT, FALSE, "Circle around centroid", "");
			}
		}
		else if (pWndSettings->m_Type == TYPE_TMTF) {
			if (!pWndSettings->m_Method3D) {
				m_pMenu->AppendSeparator();
				m_pMenu->AppendItem(IDC_MTF_AVG_ITEM, FALSE, pWndSettings->m_MTFAverage, "Radial average curve", "");
			}
		}
		else if (pWndSettings->m_Type == TYPE_TLTR) {
			m_pMenu->AppendSeparator();
			CreateSubmenuLtr();
		}
		else if (pWndSettings->m_Type == TYPE_TEEF) {
			m_pMenu->AppendSeparator();
			if (pWndSettings->m_EEFType != EEF_CNT) pWndSettings->m_EEFType = EEF_MAX;
			m_pMenu->AppendItem(IDC_EEF_TYPE1_ITEM, pWndSettings->m_EEFType == EEF_MAX, FALSE, "Circle round maximum", "");
			m_pMenu->AppendItem(IDC_EEF_TYPE2_ITEM, pWndSettings->m_EEFType == EEF_CNT, FALSE, "Circle round centroid", "");
		}
		else if (pWndSettings->m_Type == TYPE_TRMS) {
			if (!pWndSettings->m_Method3D) {
				m_pMenu->AppendSeparator();
				m_pMenu->AppendItem(IDC_RMS_CMB_ITEM, FALSE, !pWndSettings->m_RMSIndividual, "Combined", "");
			}
		}

		break;

	case 3:
		// type
		m_pMenu->AppendItem(IDC_TYPE_CEYE_ITEM, pWndSettings->m_Type == TYPE_CEYE, FALSE, "Eye Image", "");
		m_pMenu->AppendItem(IDC_TYPE_CAXM_ITEM, pWndSettings->m_Type == TYPE_CAXM, FALSE, "Axial Map", "");
		m_pMenu->AppendItem(IDC_TYPE_CTNM_ITEM, pWndSettings->m_Type == TYPE_CTNM, FALSE, "Local ROC Map", "");
		m_pMenu->AppendItem(IDC_TYPE_CRFM_ITEM, pWndSettings->m_Type == TYPE_CRFM, FALSE, "Refractive Map", "");
		m_pMenu->AppendItem(IDC_TYPE_CELM_ITEM, pWndSettings->m_Type == TYPE_CELM, FALSE, "Z Elevation Map", "");
		m_pMenu->AppendSeparator();
		// method
		{
			if (pWndSettings->m_Type == TYPE_CAXM ||
				pWndSettings->m_Type == TYPE_CTNM ||
				pWndSettings->m_Type == TYPE_CRFM ||
				pWndSettings->m_Type == TYPE_CELM) {
				m_pMenu->AppendItem(IDC_METHOD_3D_ITEM, FALSE, pWndSettings->m_Method3D, "3D", "");
			}
		}
		// size
		m_pMenu->AppendItem(IDC_SIZE_LARGE_ITEM, FALSE, !pWndSettings->m_SizeSmall, "Large window", "");
		m_pMenu->AppendSeparator();
		// units
		if (pWndSettings->m_Type == TYPE_CAXM || pWndSettings->m_Type == TYPE_CTNM) {
			m_pMenu->AppendItem(IDC_MAP_DIOPTERS_ITEM, pWndSettings->m_MapUnit == DIOPTERS, FALSE, "Diopters", "");
			m_pMenu->AppendItem(IDC_MAP_MILLIMETERS_ITEM, pWndSettings->m_MapUnit == MILLIMETERS, FALSE, "Millimeters", "");
			m_pMenu->AppendSeparator();
		}
		// elements
		if (pWndSettings->m_Type == TYPE_CEYE) {
			m_pMenu->AppendItem(IDC_SHOW_MAP_RINGS_ITEM, FALSE, pWndSettings->m_MapShowRings, "Show rings", "");
			m_pMenu->AppendItem(IDC_SHOW_MAP_PUPIL_ITEM, FALSE, pWndSettings->m_MapShowPupil, "Show pupil", "");
			m_pMenu->AppendItem(IDC_SHOW_MAP_LIMBUS_ITEM, FALSE, pWndSettings->m_MapShowLimbus, "Show limbus", "");
		}
		else if (pWndSettings->m_Type == TYPE_CAXM || pWndSettings->m_Type == TYPE_CTNM || pWndSettings->m_Type == TYPE_CRFM || pWndSettings->m_Type == TYPE_CELM) {
			if (pWndSettings->m_Method3D) {
				m_pMenu->AppendItem(IDC_SHOW_MAP_SURFACE_ITEM, FALSE, pWndSettings->m_MapShowSolidSurface, "Show solid surface", "");
				m_pMenu->AppendItem(IDC_SHOW_MAP_MESH_ITEM, FALSE, pWndSettings->m_MapShowWireMesh, "Show wire mesh", "");
			}
			else {
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
		}
		if (pWndSettings->m_Type == TYPE_CAXM || pWndSettings->m_Type == TYPE_CTNM || pWndSettings->m_Type == TYPE_CRFM) {
			pSubMenu = m_pMenu->AppendSubmenu("Map middle value", "");
			pSubMenu->AppendItem(IDC_MAP_CENT_USER_ITEM, !pScale->m_CentAuto, FALSE, "Fixed...", "");
			pSubMenu->AppendItem(IDC_MAP_CENT_AUTO_ITEM, pScale->m_CentAuto, FALSE, "Auto", "");
		}
		if (pWndSettings->m_Type == TYPE_CAXM || pWndSettings->m_Type == TYPE_CTNM || pWndSettings->m_Type == TYPE_CRFM || pWndSettings->m_Type == TYPE_CELM) {
			CreateSubmenuMapSteps();
		}
		if (pWndSettings->m_Type == TYPE_CAXM || pWndSettings->m_Type == TYPE_CTNM || pWndSettings->m_Type == TYPE_CRFM || pWndSettings->m_Type == TYPE_CELM) {
			CreateSubmenuMapColors();
		}
		if (pWndSettings->m_Type == TYPE_CELM) {
			m_pMenu->AppendSeparator();
			pSubMenu = m_pMenu->AppendSubmenu("Reference surface", "");
			pSubMenu->AppendItem(IDC_REF_BEST_SPHERE_ITEM, pWndSettings->m_RefSurfaceType == REF_SURFACE_BEST_SPHERE, FALSE, "Best fit sphere", "");
			pSubMenu->AppendItem(IDC_REF_BEST_CONICOID_ITEM, pWndSettings->m_RefSurfaceType == REF_SURFACE_BEST_CONIC, FALSE, "Best fit conicoid", "");
			pSubMenu->AppendItem(IDC_REF_USER_CONICOID_ITEM, pWndSettings->m_RefSurfaceType == REF_SURFACE_USER_CONIC, FALSE, "User defined conicoid...", "");
		}

		break;
	}

	if (m_d != 3) {
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_SAVE_TXT_ITEM, FALSE, FALSE, "Save Zernike coefficients into file", "");
	}

	//521
	if (m_i != 3 && !SameSum())//custom
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
	}
	//521
}

//***************************************************************************************
//521
void CWFCTSumWnd::OnShowDefaultItemClicked()
{
	CBusyCursor Cursor;

	m_pWndSettings[m_d].m_LtrDistanceType = 0;//530

	if (m_d != 3)
	{
		if (m_i == 0)//Change Analysis
		{
			m_pWndSettings[0].m_Type = 41;
			m_pWndSettings[0].m_RMSIndividual = 1;
		}
		else if (m_i == 1)//Summary
		{
			m_pWndSettings[0].m_Type = 40;
			m_pWndSettings[0].m_MapShowAstigmatismAxes = 0;
			m_pWndSettings[0].m_MapShowMap = 1;
			m_pWndSettings[0].m_MapShowEye = 0;
			m_pWndSettings[0].m_MapShowNumbers = 0;

			m_pWndSettings[0].m_MapShowPupil = 0;
			m_pWndSettings[0].m_MapShowLimbus = 0;
			m_pWndSettings[0].m_MapShowCornealVertex = 0;
			m_pWndSettings[0].m_MapTranslucent = 0;
		}
		else if (m_i == 2) //6.2.0 Change from Visual Acuity to MTF
		{
			m_pWndSettings[0].m_Type = TYPE_IMTF;

			m_pWndSettings[1].m_Type = TYPE_CMTF;
			m_pWndSettings[2].m_Type = TYPE_TMTF;
		}
		else if (m_i == 6)// 530 Astigmatism source display
		{
			m_pWndSettings[0].m_Type = TYPE_ILTR;
			m_pWndSettings[0].m_Ltr = 0;
			m_pWndSettings[0].m_LtrLine = 70;
			m_pWndSettings[0].m_LtrOrientation = 0;
		}


		m_pWndSettings[0].m_Method3D = 0;
		m_pWndSettings[m_d].m_SizeSmall = 1;
		if (m_i != 6 && m_i != 2)
		{
			m_pWndSettings[0].m_Mask.SetType(MASK_TOTAL_NO_DEFOCUS);
		}
		else
		{
			if (m_i == 2)
			{
				m_pWndSettings[0].m_Mask.SetType(MASK_HO_TOTAL);//6.2.0
			}
			else
			{
				m_pWndSettings[0].m_Mask.SetType(MASK_ASTIGMATISM);
			}
		}

		m_pWndSettings[0].m_Piston = 1;
		m_pWndSettings[0].m_Tilt = 1;
		m_pWndSettings[0].m_ZoneRadiusUm = 5000;

		m_d = 0;
	}
	else
	{
		CWndSettings* pWndSettings = GetWndSettings();

		pWndSettings->m_Type = 31;
		pWndSettings->m_Method3D = 0;
		pWndSettings->m_SizeSmall = 1;
		pWndSettings->m_MapUnit = DIOPTERS;
		pWndSettings->m_MapShowMap = 1;
		pWndSettings->m_MapShowEye = 1;
		pWndSettings->m_MapShowNumbers = 0;
		pWndSettings->m_MapShowPupil = 0;
		pWndSettings->m_MapShowLimbus = 0;
		pWndSettings->m_MapShowCornealVertex = 0;
		pWndSettings->m_MapTranslucent = 0;
		pWndSettings->m_MapShowKeratometry = 1;

		CScale* pScale = pWndSettings->GetScale();
		pScale->m_Step = 0.5;
		pWndSettings->SetDefaultColors(1);
	}

	CreateChildWnd();

	Repaint();
}
//521
//***************************************************************************************
//521
BOOL CWFCTSumWnd::SameSum()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_i == 0 && m_d != 3)  //Change Analysis
	{
		if (m_pWndSettings[0].m_Type != 41)        return FALSE;
		if (m_pWndSettings[0].m_RMSIndividual != 1)         return FALSE;
	}

	if (m_i == 1 && m_d != 3)  //Summary
	{
		if (m_pWndSettings[0].m_Type != 40)        return FALSE;
		if (m_pWndSettings[0].m_MapShowAstigmatismAxes != 0)   return FALSE;
		if (m_pWndSettings[0].m_MapShowMap != 1)         return FALSE;
		if (m_pWndSettings[m_d].m_MapShowEye != 0)         return FALSE;
		if (m_pWndSettings[0].m_MapShowNumbers != 0)         return FALSE;

		if (m_pWndSettings[m_d].m_MapShowPupil != 0)         return FALSE;
		if (m_pWndSettings[m_d].m_MapShowLimbus != 0)         return FALSE;
		if (m_pWndSettings[m_d].m_MapShowCornealVertex != 0)   return FALSE;
		if (m_pWndSettings[m_d].m_MapTranslucent != 0)         return FALSE;
	}

	if (m_i == 2)//6.2.0 MTF
	{
		if (m_pWndSettings[0].m_Type != TYPE_IMTF) return FALSE;
		if (m_pWndSettings[1].m_Type != TYPE_CMTF) return FALSE;
		if (m_pWndSettings[2].m_Type != TYPE_TMTF) return FALSE;

		if (m_pWndSettings[0].m_Mask.GetType() != MASK_HO_TOTAL)   return FALSE;
	}

	if (m_i == 6)  //Astigmatism Source
	{
		if (m_pWndSettings[0].m_Type != TYPE_ILTR) return FALSE;
		if (m_pWndSettings[0].m_Ltr != 0)   return FALSE;
		if (m_pWndSettings[0].m_LtrLine != 70)  return FALSE;
		if (m_pWndSettings[0].m_LtrOrientation != 0)   return FALSE;
	}

	if (m_d != 3)
	{
		if (m_pWndSettings[0].m_Method3D != 0)         return FALSE;
		if (m_pWndSettings[m_d].m_SizeSmall != 1)         return FALSE;

		if (m_i != 2)
		{
			if (m_i != 6)//530
			{
				if (m_pWndSettings[0].m_Mask.GetType() != MASK_TOTAL_NO_DEFOCUS)   return FALSE;
			}
			else//530
			{
				if (m_pWndSettings[0].m_Mask.GetType() != MASK_ASTIGMATISM)   return FALSE;
			}
		}

		if (m_pWndSettings[0].m_Piston != 1)         return FALSE;
		if (m_pWndSettings[0].m_Tilt != 1)         return FALSE;
	}

	if (m_d == 3)
	{
		if (pWndSettings->m_Type != 31)        return FALSE;
		if (pWndSettings->m_Method3D != 0)         return FALSE;
		if (pWndSettings->m_SizeSmall != 1)         return FALSE;
		if (pWndSettings->m_MapUnit != DIOPTERS)  return FALSE;
		if (pWndSettings->m_MapShowMap != 1)         return FALSE;
		if (pWndSettings->m_MapShowEye != 1)         return FALSE;
		if (pWndSettings->m_MapShowNumbers != 0)         return FALSE;
		if (pWndSettings->m_MapShowPupil != 0)         return FALSE;
		if (pWndSettings->m_MapShowLimbus != 0)         return FALSE;
		if (pWndSettings->m_MapShowCornealVertex != 0)         return FALSE;
		if (pWndSettings->m_MapTranslucent != 0)         return FALSE;
		if (pWndSettings->m_MapShowKeratometry != 1)         return FALSE;

		CScale* pScale = pWndSettings->GetScale();
		if (pScale->m_Step != 0.5)                              return FALSE;
		if (!pWndSettings->AreDefaultColors(1))                 return FALSE;
	}

	return TRUE;
}
//521
//***************************************************************************************
//530
void CWFCTSumWnd::OnShowWFTextInfoBtnClicked()
{
	m_pWndSettings[0].m_SimplifiedWFPanel = !m_pWndSettings[0].m_SimplifiedWFPanel;
	Repaint();

	CString s = (m_pWndSettings[0].m_SimplifiedWFPanel ? "Show Details" : "Hide Details");
	m_WFShowFullInfoButton.SetTip(s);
	m_WFShowFullInfoButton.SetImage(m_pWndSettings[0].m_SimplifiedWFPanel ?
		(m_SW >= 1920 ? IDR_SHOW_DETAIL_3 : m_SW >= 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
		(m_SW >= 1920 ? IDR_HIDE_DETAIL_3 : m_SW >= 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
	m_WFShowFullInfoButton.Repaint();
}
//530
//***************************************************************************************
//530
void CWFCTSumWnd::OnShowCTTextInfoBtnClicked()
{
	m_pWndSettings[0].m_SimplifiedCTPanel = !m_pWndSettings[0].m_SimplifiedCTPanel;
	Repaint();

	CString s = (m_pWndSettings[0].m_SimplifiedWFPanel ? "Show Details" : "Hide Details");
	m_CTShowFullInfoButton.SetTip(s);
	m_CTShowFullInfoButton.SetImage(m_pWndSettings[0].m_SimplifiedCTPanel ?
		(m_SW >= 1920 ? IDR_SHOW_DETAIL_3 : m_SW >= 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
		(m_SW >= 1920 ? IDR_HIDE_DETAIL_3 : m_SW >= 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
	m_CTShowFullInfoButton.Repaint();
}
//530
//****************************************************************************************
