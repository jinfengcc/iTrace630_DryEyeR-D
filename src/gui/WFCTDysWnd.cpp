//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "WFCTDysWnd.h"
#include "EyeWnd.h"
#include "PSFWnd.h"
#include "MTFWnd.h"
#include "LTRWnd.h"
#include "EEFWnd.h"
#include "RMSWnd.h"
#include "BusyCursor.h"
#include "GlobalFunctions.h"//

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CWFCTDysWnd, CSumWnd)

	ON_MESSAGE(WM_ROTATE, OnRotateMsg)
	ON_MESSAGE(WM_CHANGE_CENT, OnChangeCentMsg)
	ON_MESSAGE(WM_CHANGE_STEP, OnChangeStepMsg)
	ON_MESSAGE(WM_CHANGE_MAX, OnChangeMaxMsg)
	ON_MESSAGE(WM_SHOW_SIMPRMS, OnChangeRmsGUI)//Change the rms gui between simplify and details

	ON_COMMAND(IDC_DEFAULT_GUI_ITEM, OnShowDefaultItemClicked)

	ON_BN_CLICKED(IDC_SHOW_FULL_INFO, OnShowWFTextInfoBtnClicked)
	ON_BN_CLICKED(IDC_SHOW_FULL_CTINFO, OnShowCTTextInfoBtnClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CWFCTDysWnd::CWFCTDysWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CWFExam* pWFExam, CCTExam* pCTExam, CWndSettings* pWndSettings, int i, int show) :
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

																												 //530 OutLine Rect
																												 //m_OutLineRect[0]
	::SetRect(&m_OutLineRect[0], intRound(m_g - 2), intRound(t - 2), intRound(m_g + w + 2), intRound(t));
	::SetRect(&m_OutLineRect[1], intRound(m_g - 2), intRound(t - 2), intRound(m_g), intRound(t + h + m_g + h + 2));
	::SetRect(&m_OutLineRect[2], intRound(m_g + w), intRound(t - 2), intRound(m_g + w + 2), intRound(t + h + m_g + h + 2));
	::SetRect(&m_OutLineRect[3], intRound(m_g + (real_t(w)*0.38)), intRound(t + h + m_g + h), intRound(m_g + w + 2), intRound(t + h + m_g + h + 2));

	::SetRect(&m_OutLineRect[4], intRound(m_g + w + m_g - 2), intRound(t - 2), intRound(m_g + w + m_g + w + m_g + w + 2), intRound(t));
	::SetRect(&m_OutLineRect[5], intRound(m_g + w + m_g - 2), intRound(t - 2), intRound(m_g + w + m_g), intRound(t + h + 2));
	::SetRect(&m_OutLineRect[6], intRound(m_g + w + m_g + w + m_g + w), intRound(t - 2), intRound(m_g + w + m_g + w + m_g + w + 2), intRound(t + h + 2));
	::SetRect(&m_OutLineRect[7], intRound(m_g + w + m_g - 2), intRound(t + h), intRound(m_g + w + m_g + w + m_g + w + 2), intRound(t + h + 2));

	::SetRect(&m_OutLineRect[8], intRound(m_g + w + m_g - 2), intRound(t + h + m_g - 2), intRound(m_g + w + m_g + w + m_g + w + 2), intRound(t + h + m_g));
	::SetRect(&m_OutLineRect[9], intRound(m_g + w + m_g - 2), intRound(t + h + m_g - 2), intRound(m_g + w + m_g), intRound(t + h + m_g + h + 2));
	::SetRect(&m_OutLineRect[10], intRound(m_g + w + m_g + w + m_g + w), intRound(t + h + m_g - 2), intRound(m_g + w + m_g + w + m_g + w + 2), intRound(t + h + m_g + h + 2));
	::SetRect(&m_OutLineRect[11], intRound(m_g + w + m_g - 2), intRound(t + h + m_g + h), intRound(m_g + w + m_g + w + m_g + w + 2), intRound(t + h + m_g + h + 2));
	//Done

	m_pWFExam = pWFExam;
	m_pCTExam = pCTExam;

	// WFCT Angle k/a adjust wf eye limbus by ct info
	LimbusAdjust(m_pWFExam, m_pCTExam);
	m_pWFExam->CalcAngles();

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

	m_WFCTDYSWnd = TRUE;//600

						//*007*[cjf***05052012],record the increase and decrease range
	::TempSettings.Com_r_max_um = ((m_pWFExam->m_WfSurface.m_r_max_um) < (m_pCTExam->m_WfSurface.m_r_max_um) ? m_pWFExam->m_WfSurface.m_r_max_um : m_pCTExam->m_WfSurface.m_r_max_um);
	//*007*[cjf***05052012]

	if (::TempSettings.Com_r_max_um > 2500) ::TempSettings.Com_r_max_um = 2500;//06022015 for DLI Branch

	for (int d = 0; d < 6; d++)   m_pWndSettings[d].m_ZoneRadiusUm = intRound(m_pWFExam->m_WfSurface.m_r_max_um); //600

	m_i = i;

	m_rms[0] = 0;
	m_rms[1] = 0;
	m_rms[2] = 0;

	//
	m_HoTotalRms[0] = 0;
	m_HoTotalRms[1] = 0;
	m_HoTotalRms[2] = 0;

	CZernikeMask Mask;
	Mask.SetType(MASK_HO_TOTAL);

	// corneal wavefront surface
	m_Surfaces[1] = m_pCTExam->m_WfSurface;
	// total wavefront surface
	m_Surfaces[2] = m_pWFExam->m_WfSurface;

	real_t r_max_um0 = 2500.0;//06022015 For DLI Branch
	r_max_um0 = __min(r_max_um0, m_Surfaces[1].m_r_max_um);
	r_max_um0 = __min(r_max_um0, m_Surfaces[2].m_r_max_um);
	r_max_um0 = __min(r_max_um0, m_pWndSettings[m_d].m_ZoneRadiusUm);

	m_Surfaces[1].ChangeRMaxUm(r_max_um0);
	m_Surfaces[1] &= Mask;
	m_Surfaces[1].ComputePiston(m_pWndSettings[m_d].m_Piston);
	m_Surfaces[1].ComputeTilt(m_pWndSettings[m_d].m_Tilt);

	m_Surfaces[2].ChangeRMaxUm(r_max_um0);
	m_Surfaces[2] &= Mask;
	m_Surfaces[2].ComputePiston(m_pWndSettings[m_d].m_Piston);
	m_Surfaces[2].ComputeTilt(m_pWndSettings[m_d].m_Tilt);

	// internal optics wavefront surface
	m_Surfaces[0] = m_Surfaces[2];
	m_Surfaces[0] -= m_Surfaces[1];


	m_HoTotalRms[0] = m_Surfaces[2].ComputeRms();//Entire eye rms
	m_HoTotalRms[1] = m_Surfaces[1].ComputeRms();//corneal eye rms
	m_HoTotalRms[2] = m_Surfaces[0].ComputeRms();//internal eye rms
												 //


												 //531 Calculate m_ReMTF_Surfaces
	CZernikeMask DysMask;
	DysMask.SetType(MASK_HO_TOTAL);

	// corneal wavefront surface
	m_DysSuface[1] = m_pCTExam->m_WfSurface;
	// total wavefront surface
	m_DysSuface[2] = m_pWFExam->m_WfSurface;

	real_t r_max_um = 2500.0;//06022015 for DLI Branch
	r_max_um = __min(r_max_um, m_DysSuface[1].m_r_max_um);
	r_max_um = __min(r_max_um, m_DysSuface[2].m_r_max_um);
	//r_max_um = __min(r_max_um, m_pWndSettings[m_d].m_ZoneRadiusUm);

	m_DysSuface[1].ChangeRMaxUm(r_max_um);
	m_DysSuface[1] &= DysMask;

	m_DysSuface[2].ChangeRMaxUm(r_max_um);
	m_DysSuface[2] &= DysMask;

	// internal optics wavefront surface
	m_DysSuface[0] = m_DysSuface[2];
	m_DysSuface[0] -= m_DysSuface[1];


	//surface for correct dysfunctional function;
	real_t um1, um2, um;

	m_ReMTF_Surfaces = m_DysSuface[0];
	for (int mode = 6; mode <= 27; mode++)
	{
		if (mode != 7 && mode != 8)
		{
			um1 = m_DysSuface[0].GetCUm(mode);//Total
			um2 = m_DysSuface[2].GetCUm(mode);//Internal

			um = (fabs(um1) <= fabs(um2) ? um1 : um2);

			if (mode == 24)
			{
				um = (um > 0.2 ? um - 0.2 : 0);
			}

			m_ReMTF_Surfaces.SetCUm(mode, um);
		}
	}

	//05292015 for DLI Branch	 
	real_t newum1 = 0;
	real_t newum2 = 0;
	um1 = m_DysSuface[1].GetCUm(7);//Corneal
	um2 = m_DysSuface[0].GetCUm(7);//Internal (Lens)
	real_t um3 = m_DysSuface[1].GetCUm(8);//Corneal
	real_t um4 = m_DysSuface[0].GetCUm(8);//Internal (Lens)
	real_t um5 = m_DysSuface[2].GetCUm(7);//Total
	real_t um6 = m_DysSuface[2].GetCUm(8);//Total
	CreateAdjustedUm(um1, um2, um3, um4, um5, um6, newum1, newum2);
	m_ReMTF_Surfaces.SetCUm(7, newum1);
	m_ReMTF_Surfaces.SetCUm(8, newum2);
	//05292015 for DLI Branch	
	//531 Calculate m_ReMTF_Surfaces Done

	m_StaticDysVal = CalDysVal(0, r_max_um);

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
	m_WFShowFullInfoButton.ShowWindow(SW_HIDE);

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

//05292015 for DLI Branch
//um1: Corneal coma at mode 7 (vertiacl coma)
//um2: Internal(lens) coma at mode 7  (vertiacl coma)
//um3: Corneal coma at mode 8   (horizontal coma)
//um4: Internal(lens) coma at mode 8 (horizontal coma)

//um5: Total coma at mode 7  
//um6: Total coma at mode 8 
void CWFCTDysWnd::CreateAdjustedUm(real_t um1, real_t um2, real_t um3, real_t um4, real_t um5, real_t um6, real_t& newum1, real_t& newum2)
{
	real_t ZC = sqrt(um1*um1 + um3*um3);
	real_t ZL = sqrt(um2*um2 + um4*um4);

	if ((ZL <= ZC) && (ZL > 0.1*ZC))
	{
		real_t CornealDeg = ::GetDegree(um3, um1);
		real_t LensDeg = ::GetDegree(um4, um2);

		real_t adj_CornealDeg = CornealDeg;
		real_t adj_LensDeg = LensDeg;

		if (CornealDeg > LensDeg)
			adj_LensDeg += 180;
		else
			adj_CornealDeg += 180;

		real_t DegreeDis = fabs(adj_CornealDeg - adj_LensDeg);


		real_t ratio = 1.0;

		//Joe's coefficents
		//if (DegreeDis <= 5) ratio = 1.0;
		//else if (DegreeDis <= 15) ratio = 0.85;
		//else if (DegreeDis <= 25) ratio = 0.70;
		//else if (DegreeDis <= 35) ratio = 0.55;
		//else if (DegreeDis <= 45) ratio = 0.40;

		if (DegreeDis <= 5) ratio = 1.0;
		else if (DegreeDis <= 15) ratio = 0.97;
		else if (DegreeDis <= 25) ratio = 0.91;
		else if (DegreeDis <= 35) ratio = 0.82;
		else if (DegreeDis <= 45) ratio = 0.71;

		if (DegreeDis <= 45)
		{
			/*newum1 = ZC*(cos(CornealDeg * _Pi_180) + ratio * cos(LensDeg * _Pi_180));
			newum2 = ZC*(sin(CornealDeg * _Pi_180) + ratio * sin(LensDeg * _Pi_180));

			real a = cos(CornealDeg * _Pi_180);
			real b = cos((CornealDeg + 180) * _Pi_180);

			real c = sin(CornealDeg * _Pi_180);
			real d = sin((CornealDeg + 180) * _Pi_180);

			newum1 = ZC*cos(CornealDeg * _Pi_180) + ZL*ratio * cos((CornealDeg + 180) * _Pi_180);
			newum2 = ZC*sin(CornealDeg * _Pi_180) + ZL*ratio * sin((CornealDeg + 180) * _Pi_180);*/

			// (7) = Len of Internal * ( weight) * cos(CornealDeg);
			// (8) = Len of Internal * ( weight) * sin(CornealDeg);
			newum1 = ZC * (1 - ratio) * cos(CornealDeg * _Pi_180);
			newum2 = ZC * (1 - ratio) * sin(CornealDeg * _Pi_180);
		}
		else
		{
			newum1 = (fabs(um2) <= fabs(um5) ? um2 : um5);
			newum2 = (fabs(um4) <= fabs(um6) ? um4 : um6);
		}
	}
	else
	{
		newum1 = (fabs(um2) <= fabs(um5) ? um2 : um5);
		newum2 = (fabs(um4) <= fabs(um6) ? um4 : um6);
	}
}
//05292015 for DLI Branch Done

//***************************************************************************************
void CWFCTDysWnd::OnSizeLargeItemClicked()
{
	CWndSettings* pWndSettings = GetWndSettings();

	pWndSettings->m_SizeSmall = !pWndSettings->m_SizeSmall;

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
	//::TempSettings.Temp_WFCTCA = FALSE;
}

//***************************************************************************************

void CWFCTDysWnd::RepaintMemDC()
{
	CSumWnd::RepaintMemDC();

	//530 To show simplied wf info for Chang Analysis
	BOOL Rows[22] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	//530

	DysInfo(m_pWFExam, m_Rect[4], m_StaticDysVal, m_HoTotalRms, 1);	//531

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

		m_MemDC.DrawRectangle(m_OutLineRect[8], white, NOCOLOR);
		m_MemDC.DrawRectangle(m_OutLineRect[9], white, NOCOLOR);
		m_MemDC.DrawRectangle(m_OutLineRect[10], white, NOCOLOR);
		m_MemDC.DrawRectangle(m_OutLineRect[11], white, NOCOLOR);
	}
	//530 Done

	// Draw the glass icon for print out  
	if (m_Printing || m_Saving)
	{
		int MaskType = m_pWndSettings[0].m_Mask.GetType();
		DrawGlassIcon(MaskType, 1, 0);
		DrawGlassIcon(MaskType, 1, 1);
	}
	//  Draw the glass icon for print out  Done
}

//***************************************************************************************
void CWFCTDysWnd::CreateChildWnd()
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

	if (m_d == 0) m_pWndSettings[1].m_Mask = m_pWndSettings[0].m_Mask;		//531
	else if (m_d == 1) m_pWndSettings[0].m_Mask = m_pWndSettings[1].m_Mask;		//531
	else if (m_d == 2 || m_d == 3) m_pWndSettings[m_d].m_Mask = m_pWndSettings[0].m_Mask;		// bug fix

																								// corneal wavefront surface
	m_Surfaces[1] = m_pCTExam->m_WfSurface;
	// total wavefront surface
	m_Surfaces[2] = m_pWFExam->m_WfSurface;

	real_t r_max_um = 2500.0;//06022015 For DLI Branch
	r_max_um = __min(r_max_um, m_Surfaces[1].m_r_max_um);
	r_max_um = __min(r_max_um, m_Surfaces[2].m_r_max_um);
	r_max_um = __min(r_max_um, m_pWndSettings[m_d].m_ZoneRadiusUm);

	m_Surfaces[1].ChangeRMaxUm(r_max_um);
	m_Surfaces[1] &= m_pWndSettings[m_d].m_Mask;
	m_Surfaces[1].ComputePiston(m_pWndSettings[m_d].m_Piston);
	m_Surfaces[1].ComputeTilt(m_pWndSettings[m_d].m_Tilt);

	m_Surfaces[2].ChangeRMaxUm(r_max_um);
	m_Surfaces[2] &= m_pWndSettings[m_d].m_Mask;
	m_Surfaces[2].ComputePiston(m_pWndSettings[m_d].m_Piston);
	m_Surfaces[2].ComputeTilt(m_pWndSettings[m_d].m_Tilt);

	// internal optics wavefront surface
	m_Surfaces[0] = m_Surfaces[2];
	m_Surfaces[0] -= m_Surfaces[1];

	if (m_rms[0] == 0)
	{
		m_rms[0] = m_Surfaces[2].ComputeRms();//Entire eye rms
		m_rms[1] = m_Surfaces[1].ComputeRms();//corneal eye rms
		m_rms[2] = m_Surfaces[0].ComputeRms();//internal eye rms
	}

	real_t SphEq[3], Sph[3], Cyl[3];
	int Axes[3];
	for (int i = 0; i < 3; i++) {
		CWFExam::GetSpheqSphCylAxis(m_Surfaces[i], 0, TRUE, SphEq[i], Sph[i], Cyl[i], Axes[i]);

		//520 
		m_Axes[i] = Axes[i];
		//520
	}

	CString s[3] = { "Internal Optics", "Cornea", "Total Eye" };
	CString s2[3] = { "INTERNAL", "INTERNAL", "CORNEA" };//530

	for (int d = 0; d < 3; d++)
	{
		if (b[d]) {
			CWndSettings* pWndSettings = m_pWndSettings + d;
			RECT Rect = pWndSettings->m_SizeSmall ? m_Rect[d] : m_LargeRect;

			if (d == 1 && pWndSettings->m_Type != TYPE_ILTR) pWndSettings->m_Type = TYPE_ILTR;

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
				CScale* pScale = pWndSettings->GetScale();
				pEyeWnd->m_Cent = 0.5;
				pEyeWnd->m_Step = 1.0 / (pScale->m_NumColors - 1);
				pEyeWnd->m_NumColors = pScale->m_NumColors;
				for (int i = 0; i < pScale->m_NumColors; i++) {
					pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
				}
				//531abc			 
				pEyeWnd->m_pWFExam = m_pWFExam;

				if (m_pWFExam->m_DetectorType == DT_LINEAR_ARRAYS)
				{
					m_pWFExam->CreateOpm2D(pEyeWnd->m_Map2D, 1.0 / pEyeWnd->m_y_px_um, m_pWFExam->m_CurShot, 2);//m_d==1?2:m_d==2?0:1);

																												//531
					real_t mean, std_dev;
					pEyeWnd->m_Map2D.ComputeMeanStdDev(mean, std_dev);

					std_dev = fabs(std_dev);

					int  GradeMean, GradeSd;
					real_t Grade;

					CString GradeStr = "";

					if (mean <  0.69)               GradeMean = 4;
					else if (mean >= 0.69 && mean < 0.76) GradeMean = 3;
					else if (mean >= 0.76 && mean < 0.85) GradeMean = 2;
					else if (mean >= 0.85 && mean < 0.91) GradeMean = 1;
					else                                  GradeMean = 0;

					if (std_dev <= 0.03)                  GradeSd = 0;
					else if (std_dev > 0.03 && std_dev <= 0.06) GradeSd = 1;
					else if (std_dev > 0.06 && std_dev <= 0.09) GradeSd = 2;
					else if (std_dev > 0.09 && std_dev <= 0.12) GradeSd = 3;
					else                                        GradeSd = 4;

					Grade = real_t(GradeMean + GradeSd) / 2.0;

					GradeStr.Format(_T("%.1f"), Grade);

					Grade = real_t(GradeMean + GradeSd) / 2.0;

					GradeStr.Format(_T("%.1f"), Grade);

					pEyeWnd->m_RBLabel[0] = "Grade = ";
					pEyeWnd->m_RBLabel[1] = GradeStr;
				}
				else
				{
					pEyeWnd->m_RBLabel[0] = "Grade ";
					pEyeWnd->m_RBLabel[1] = "is unavailable";
				}
				//531		

				m_pDispWnd[d] = pEyeWnd;
			}
			break;


			case TYPE_IMTF:
			{
				CMTFWnd* pMTFWnd = new CMTFWnd(Rect, this);
				CWFExam::CreateMtf(m_Surfaces[0], pMTFWnd->m_Mtf);

				//531 Calculate a correct surface for corrected MTF         
				CWFExam::CreateMtf(m_ReMTF_Surfaces, pMTFWnd->m_ReMtf);
				//531 Done

				pMTFWnd->m_ShowDys = TRUE;//531

				pMTFWnd->m_Dysval = m_StaticDysVal;//CalDysVal(0,Rect,r_max_um);//600

				pMTFWnd->m_Type = pWndSettings->m_Type;
				pMTFWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
				pMTFWnd->m_Method3D = pWndSettings->m_Method3D;

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

			case TYPE_ILTR:
			{
				if (!(pWndSettings->m_LtrLine == 20 || pWndSettings->m_LtrLine == 25 || pWndSettings->m_LtrLine == 30 ||
					pWndSettings->m_LtrLine == 40 || pWndSettings->m_LtrLine == 50 || pWndSettings->m_LtrLine == 70 ||
					pWndSettings->m_LtrLine == 100 || pWndSettings->m_LtrLine == 200))
				{
					pWndSettings->m_LtrLine = 40;
				}

				CLtrWnd* pLtrWnd = new CLtrWnd(Rect, this);

				CWFExam::CreateLtr(m_Surfaces[0], pLtrWnd->m_Ltr, pWndSettings->m_Ltr, pWndSettings->m_LtrLine, pWndSettings->m_LtrOrientation);
				pLtrWnd->m_Type = pWndSettings->m_Type;
				pLtrWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
				pLtrWnd->m_Method3D = pWndSettings->m_Method3D;
				//pLtrWnd->m_LTLabel[0].Format(_T("%s - Letter 20 / %i", s2[0], pWndSettings->m_LtrLine);
				pLtrWnd->m_LTLabel[0].Format(_T("%s - Letter"), s2[0]);//531
				pWndSettings->m_Mask.GetName(pLtrWnd->m_LTLabel[1]);
				pLtrWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
				pLtrWnd->CreateZoomButtons();
				pLtrWnd->CreateZoneButtons();

				//531
				//Calculate the Dysfunctional value		  
				pLtrWnd->m_DysVal = m_StaticDysVal;
				pLtrWnd->m_RTLabel[0] = "Dysfunctional Lens Index";
				//531 Done

				//601 05202015	
				pLtrWnd->m_TMLabel = "";
				if (m_rms[1] >= 0.280)
				{
					pLtrWnd->m_TMLabel = "HIGH CORNEAL ABERRATIONS";
					pLtrWnd->m_TMLabelColor = (m_rms[1] < 0.450 ? (m_Printing ? ORANGE : YELLOW) : RED);
				}
				//601 05202015 Done    

				pLtrWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 1);//531

				m_pDispWnd[d] = pLtrWnd;
			}
			break;
			}
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
				pEyeWnd->m_LTLabel[0] = s2[2] + "  Axial Map";
				{ // Chang Analysis	
					pEyeWnd->m_RBLabelColor[0] = m_Printing ? BLACK : WHITE;
					pEyeWnd->m_RBLabelColor[1] = pEyeWnd->m_RBLabelColor[0];
					pEyeWnd->m_RBLabelColor[2] = pEyeWnd->m_RBLabelColor[0];
					pEyeWnd->m_RBLabelColor[3] = pEyeWnd->m_RBLabelColor[0];
					pEyeWnd->m_RBLabelColor[4] = pEyeWnd->m_RBLabelColor[0];
					pEyeWnd->m_RBLabelColor[5] = pEyeWnd->m_RBLabelColor[0];
					pEyeWnd->m_RBLabelBkColor = m_Printing ? WHITE : BLACK;
					pEyeWnd->m_RBLabel[1] = "Avg K";
					for (int k = 0; k <= 3; k++)
					{
						if (m_pCTExam->m_zk_ok[k])
						{
							pEyeWnd->m_RBLabel[2 + k].Format(_T("%.2f D @ D = %i mm"), m_pCTExam->m_zk_av_dp[k], k);
						}
					}
				}
			}
			else if (pWndSettings->m_Type == TYPE_CTNM) {
				pEyeWnd->m_LTLabel[0] = s2[2] + "  Local ROC Map";
			}
			else {
				pEyeWnd->m_LTLabel[0] = s2[2] + "  Refractive Map";
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
			for (int i = 0; i < pScale->m_NumColors; i++) {
				pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
			}
			pEyeWnd->m_pCTExam = m_pCTExam;
			if (pWndSettings->m_Method3D) {
				m_pCTExam->CreateDpm3D(pEyeWnd->m_Map3D, pWndSettings->m_Type, MapUnit);
			}
			else {
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


	//600
	if (m_d == 0 || m_d == 1)
	{
		int a_d = (m_d == 0 ? 1 : 0);

		if (m_pWndSettings[a_d].m_ZoneRadiusUm != m_pWndSettings[m_d].m_ZoneRadiusUm)
			m_pWndSettings[a_d].m_ZoneRadiusUm = m_pWndSettings[m_d].m_ZoneRadiusUm;
	}
	//600 Done

	for (int d = 0; d < 2; d++)
	{
		if (b[d])
		{
			if (m_pWndSettings[d].m_ZoneRadiusUm >= ::TempSettings.Com_r_max_um)
			{
				m_pDispWnd[d]->m_IncZoneButton.SetEnabled(FALSE);
				m_pDispWnd[d]->m_IncZoneButton.Repaint();
			}
			if (m_pWndSettings[d].m_ZoneRadiusUm == 1250)
			{
				m_pDispWnd[d]->m_DecZoneButton.SetEnabled(FALSE);
				m_pDispWnd[d]->m_DecZoneButton.Repaint();
			}
		}
	}
}


//***************************************************************************************

LRESULT CWFCTDysWnd::OnRotateMsg(WPARAM wParam, LPARAM lParam)
{
	int ax, ay, d;

	for (d = 0; d < 3; d++) {
		if ((WPARAM)m_pDispWnd[d] == wParam) {
			ax = m_pDispWnd[d]->m_ax;
			ay = m_pDispWnd[d]->m_ay;
			break;
		}
	}
	if (d == 3) return 0;

	for (d = 0; d < 3; d++) {
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] != wParam) {
			m_pDispWnd[d]->m_ax = ax;
			m_pDispWnd[d]->m_ay = ay;
			m_pDispWnd[d]->Repaint();
		}
	}

	return 0;
}

//***************************************************************************************

LRESULT CWFCTDysWnd::OnChangeCentMsg(WPARAM wParam, LPARAM lParam)
{
	real_t Cent = INVALID_VALUE;

	for (int d = 0; d < 3; d++) {
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] == wParam) {
			Cent = ((CEyeWnd*)m_pDispWnd[d])->m_Cent;
			break;
		}
	}

	if (Cent == INVALID_VALUE) return 0;

	for (int d = 0; d < 3; d++) {
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] != wParam) {
			((CEyeWnd*)m_pDispWnd[d])->m_Cent = Cent;
			m_pDispWnd[d]->Repaint();
		}
	}

	return 0;
}

//***************************************************************************************

LRESULT CWFCTDysWnd::OnChangeStepMsg(WPARAM wParam, LPARAM lParam)
{
	real_t Step = INVALID_VALUE;

	for (int d = 0; d < 3; d++) {
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] == wParam) {
			Step = ((CEyeWnd*)m_pDispWnd[d])->m_Step;
			break;
		}
	}

	if (Step == INVALID_VALUE) return 0;

	for (int d = 0; d < 3; d++) {
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] != wParam) {
			((CEyeWnd*)m_pDispWnd[d])->m_Step = Step;
			m_pDispWnd[d]->Repaint();
		}
	}

	return 0;
}

//***************************************************************************************
//530
LRESULT CWFCTDysWnd::OnChangeRmsGUI(WPARAM wParam, LPARAM lParam)
{
	BOOL Simple = FALSE;
	int  Sel_d = 1;

	Simple = !m_pWndSettings[Sel_d].m_SimplifiedRMS;

	int W = ::GetSystemMetrics(SM_CXSCREEN);

	m_pWndSettings[Sel_d].m_SimplifiedRMS = Simple;
	((CRMSWnd*)m_pDispWnd[Sel_d])->m_SimpliedRMS = Simple;

	CString s = (Simple ? "Show Details" : "Hide Details");
	m_pDispWnd[Sel_d]->m_ShowMoreButton.SetTip(s);

	m_pDispWnd[Sel_d]->m_ShowMoreButton.SetImage(Simple ?
		(W >= 1920 ? IDR_SHOW_DETAIL_3 : W >= 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
		(W >= 1920 ? IDR_HIDE_DETAIL_3 : W >= 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));

	m_pDispWnd[Sel_d]->m_ShowMoreButton.Repaint();
	m_pDispWnd[Sel_d]->Repaint();

	return 0;
}
//530
//***************************************************************************************
LRESULT CWFCTDysWnd::OnChangeMaxMsg(WPARAM wParam, LPARAM lParam)
{
	int Max = INVALID_VALUE;

	for (int d = 0; d < 3; d++) {
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] == wParam) {
			Max = ((CRMSWnd*)m_pDispWnd[d])->m_Max;
			break;
		}
	}

	if (Max == INVALID_VALUE) return 0;

	for (int d = 0; d < 3; d++) {
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] != wParam) {
			((CRMSWnd*)m_pDispWnd[d])->m_Max = Max;
			m_pDispWnd[d]->Repaint();
		}
	}

	return 0;
}

//***************************************************************************************

void CWFCTDysWnd::CreateMenu()
{
	CWndSettings* pWndSettings = m_pWndSettings + m_d;
	CScale* pScale = pWndSettings->GetScale();
	CXPMenu* pSubMenu;

	if (pWndSettings->m_Method3D) pWndSettings->m_RMSIndividual = TRUE;

	if (m_d == 0)//MTF
	{
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

		if (!pWndSettings->m_Method3D) {
			m_pMenu->AppendSeparator();
			m_pMenu->AppendItem(IDC_MTF_AVG_ITEM, FALSE, pWndSettings->m_MTFAverage, "Radial average curve", "");
		}
	}

	else if (m_d == 1)//LTR
	{
		m_pMenu->AppendItem(IDC_METHOD_3D_ITEM, FALSE, pWndSettings->m_Method3D, "3D", "");
		// size
		m_pMenu->AppendItem(IDC_SIZE_LARGE_ITEM, FALSE, !pWndSettings->m_SizeSmall, "Large window", "");
		m_pMenu->AppendSeparator();
		// zone & mask
		CreateSubmenuZone(2);
		CreateSubmenuMask();

		m_pMenu->AppendSeparator();
		CreateSubmenuLtr();
		//   
		//  if (!pWndSettings->m_Method3D) {
		//    m_pMenu->AppendSeparator();
		//    m_pMenu->AppendItem(IDC_RMS_CMB_ITEM, FALSE, !pWndSettings->m_RMSIndividual, "Combined", "");
		//  }    
	}

	else if (m_d == 2)//OPT
	{
		m_pMenu->AppendItem(IDC_SIZE_LARGE_ITEM, FALSE, !pWndSettings->m_SizeSmall, "Large window", "");
		m_pMenu->AppendSeparator();

		m_pMenu->AppendItem(IDC_SHOW_MAP_EYE_ITEM, FALSE, pWndSettings->m_MapShowEye, "Show eye image", "");
		m_pMenu->AppendItem(IDC_SHOW_MAP_PUPIL_ITEM, FALSE, pWndSettings->m_MapShowPupil, "Show pupil", "");
		m_pMenu->AppendItem(IDC_SHOW_MAP_VERTEX_ITEM, FALSE, pWndSettings->m_MapShowCornealVertex, "Show corneal vertex", "");
		m_pMenu->AppendItem(IDC_SHOW_MAP_LIMBUS_ITEM, FALSE, pWndSettings->m_MapShowLimbus, "Show limbus", "");

		m_pMenu->AppendItem(IDC_MAP_TRANSLUCENT_ITEM, FALSE, pWndSettings->m_MapTranslucent, "Translucent map", "");
		CXPMenu* pSubMenu = m_pMenu->AppendSubmenu("Map colors", "");
		BOOL DefaultColors = pWndSettings->AreDefaultColors(0);
		pSubMenu->AppendItem(IDC_MAP_COLORS1_ITEM, DefaultColors, FALSE, "Default", "");
		pSubMenu->AppendItem(IDC_MAP_COLORS_USER_ITEM, !DefaultColors, FALSE, "Custom...", "");
	}

	else if (m_d == 3)
	{
		// type
		m_pMenu->AppendItem(IDC_TYPE_CEYE_ITEM, pWndSettings->m_Type == TYPE_CEYE, FALSE, "Eye Image", "");
		m_pMenu->AppendItem(IDC_TYPE_CAXM_ITEM, pWndSettings->m_Type == TYPE_CAXM, FALSE, "Axial Map", "");
		m_pMenu->AppendItem(IDC_TYPE_CTNM_ITEM, pWndSettings->m_Type == TYPE_CTNM, FALSE, "Local ROC Map", "");
		m_pMenu->AppendItem(IDC_TYPE_CRFM_ITEM, pWndSettings->m_Type == TYPE_CRFM, FALSE, "Refractive Map", "");
		m_pMenu->AppendItem(IDC_TYPE_CELM_ITEM, pWndSettings->m_Type == TYPE_CELM, FALSE, "Z Elevation Map", "");
		m_pMenu->AppendSeparator();
		// method

		if (pWndSettings->m_Type == TYPE_CAXM ||
			pWndSettings->m_Type == TYPE_CTNM ||
			pWndSettings->m_Type == TYPE_CRFM ||
			pWndSettings->m_Type == TYPE_CELM)
		{
			m_pMenu->AppendItem(IDC_METHOD_3D_ITEM, FALSE, pWndSettings->m_Method3D, "3D", "");
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
	}

	if (m_d != 3) {
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_SAVE_TXT_ITEM, FALSE, FALSE, "Save Zernike coefficients into file", "");
	}

	//521
	if (!SameSum())//custom
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
	}
	//521
}

//***************************************************************************************
//521
void CWFCTDysWnd::OnShowDefaultItemClicked()
{
	CBusyCursor Cursor;
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_d == 0)//MTF
	{
		pWndSettings->m_Type = TYPE_IMTF;
		pWndSettings->m_Method3D = 0;
		pWndSettings->m_SizeSmall = TRUE;
		pWndSettings->m_Mask.SetType(MASK_HO_TOTAL);
		pWndSettings->m_MTFAverage = TRUE;
	}
	else if (m_d == 1)//ACU
	{
		pWndSettings->m_Type = TYPE_ILTR;
		pWndSettings->m_Method3D = 0;
		pWndSettings->m_SizeSmall = TRUE;
		//pWndSettings->m_RMSIndividual = FALSE;
		//pWndSettings->m_SimplifiedRMS = TRUE;
		pWndSettings->m_Mask.SetType(MASK_HO_TOTAL);

		pWndSettings->m_Ltr = 0;
		pWndSettings->m_LtrLine = 70;
		pWndSettings->m_LtrOrientation = 0;
	}
	else if (m_d == 2)//OPM
	{
		pWndSettings->m_Type = TYPE_TOPM;
		pWndSettings->m_SizeSmall = TRUE;
	}
	else
	{
		CWndSettings* pWndSettings = GetWndSettings();

		pWndSettings->m_Type = TYPE_CAXM;
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
BOOL CWFCTDysWnd::SameSum()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (pWndSettings->m_SizeSmall != 1)         return FALSE;

	if (m_d == 0)//MTF
	{
		if (pWndSettings->m_Method3D != 0)         return FALSE;
		if (pWndSettings->m_Mask.GetType() != MASK_HO_TOTAL)   return FALSE;
		if (pWndSettings->m_MTFAverage != 1)         return FALSE;
	}

	else if (m_d == 1)// LTR
	{
		if (pWndSettings->m_Mask.GetType() != MASK_HO_TOTAL) return FALSE;
		if (pWndSettings->m_Method3D != 0)             return FALSE;
		if (pWndSettings->m_Ltr != 0)   return FALSE;
		if (pWndSettings->m_LtrLine != 70)  return FALSE;
		if (pWndSettings->m_LtrOrientation != 0)   return FALSE;
	}

	else if (m_d == 3)
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

//***************************************************************************************

void CWFCTDysWnd::OnShowWFTextInfoBtnClicked()
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

//***************************************************************************************
//530
void CWFCTDysWnd::OnShowCTTextInfoBtnClicked()
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
//***************************************************************************************
//***************************************************************************************
//531
real_t CWFCTDysWnd::CalDysVal(int d, real_t pupilSize)
{
	real_t  index, valNumX;

	pupilSize = pupilSize * 2 / 1000;

	CMTF Mtf;

	if (d != 0) CWFExam::CreateMtf(m_Surfaces[d], Mtf);
	else CWFExam::CreateMtf(m_ReMTF_Surfaces, Mtf);

	int w = 427;//Rect.right - Rect.left;
	int h = 375;//Rect.bottom - Rect.top;

	real_t L = 0.100 * w;
	real_t T = 0.180 * h;
	real_t R = 0.900 * w;
	real_t B = 0.860 * h;

	real_t w_px = R - L;
	real_t h_px = B - T;
	int  w_cd = 30;
	real_t px_cd = w_px / w_cd;
	real_t valNumY = T + (B - T) * 3 / 5;

	real_t fa;
	real_t minpos = 1000;//531
	int  k = 0;
	int  t = 0;

	real_t areaW = 0;//601
	real_t areaB = 0;//601
	real_t areaC = 0;//601
	real_t lastxa = L;//601

	for (real_t i = 0; i <= 30; i = i + 0.1)
	{
		real_t xa = L + i * px_cd;
		if (!Mtf.GetAtCdR(i, fa)) break;

		real_t ya = B - fa * h_px;

		areaB += (B - ya)*(xa - lastxa);//601	
		areaW += h_px*(xa - lastxa);//601	

		real_t disPos = fabs(ya - valNumY);
		if (disPos < minpos)
		{
			valNumX = xa;
			minpos = disPos;
		}

		if (t == 0 || t == 10)
		{
			//avgH += fa;
			k++;
			t = 0;

			if (k == 5) areaC = areaB;//601
		}
		t++;

		lastxa = xa;//601
	}

	areaC = areaB - areaC;//601
	areaC /= areaW;//601

	valNumX = (valNumX - L) / px_cd;

	//calculate the value 601
	index = (sqrt(valNumX) * 2 + (areaC - 0.05) * 10 / 0.35) / (2 * ((5 - (pupilSize + 0.6))*0.03 + 1));
	index = (index > 10 ? 10 : index);
	//Done 601

	return index;
}
//531 Done
//***************************************************************************************

