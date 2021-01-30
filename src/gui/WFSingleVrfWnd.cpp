//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "WFSingleVrfWnd.h"
#include "EyeWnd.h"
#include "RSDWnd.h"
#include "SignalWnd.h"
#include "BusyCursor.h"
#include "LensDlg.h"
#include "ImageDlg.h"
#include "SelectImgDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CWFSingleVrfWnd, CSumWnd)

	ON_BN_CLICKED(IDC_SHOT_1, OnShot1ButtonClicked)
	ON_BN_CLICKED(IDC_SHOT_2, OnShot2ButtonClicked)
	ON_BN_CLICKED(IDC_NEXT, OnNextButtonClicked)
	ON_BN_CLICKED(IDC_PREV, OnPrevButtonClicked)
	ON_BN_CLICKED(IDC_ACCEPT, OnAcceptButtonClicked)
	ON_BN_CLICKED(IDC_REJECT, OnRejectButtonClicked)

	ON_COMMAND(IDK_F1, OnF1)
	ON_COMMAND(IDK_F2, OnF2)
	ON_COMMAND(IDK_F3, OnF3)
	ON_COMMAND(IDK_F4, OnF4)
	ON_COMMAND(IDK_F7, OnF7)

	ON_COMMAND(IDK_LEFT, OnPrevButtonClicked)
	ON_COMMAND(IDK_RIGHT, OnNextButtonClicked)
	ON_COMMAND(IDC_EDIT_LENS_DOTS_ITEM, OnEditLensDotsItemClicked)
	ON_COMMAND(IDC_EDIT_LIMBUS_ITEM, OnEditLimbusItemClicked)

	ON_COMMAND(IDC_DEFAULT_GUI_ITEM, OnShowDefaultItemClicked)

	ON_COMMAND(IDC_SELECT_IMAGE_ITEM, OnSelEyeImgItemClicked)

	ON_WM_TIMER()

END_MESSAGE_MAP()

//***************************************************************************************

CWFSingleVrfWnd::CWFSingleVrfWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CWFExam* pWFExam, CWndSettings* pWndSettings, int whichExam, BOOL ShowExamNum, int show) :
	CSumWnd(pWnd, WndRect, pPatient, pWndSettings)
{
	CBusyCursor Cursor;

	//531
	m_whichExam = whichExam;
	m_ShowExamNum = ShowExamNum;
	//531 Done

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
	CMFont Font(intRound(1.1 * (2.0 * m_g)), 400, "Arial");
	//
	m_MemDC.MeasureRect(" Point 000", Rect, Font);
	int l = intRound(0.010 * m_w);
	int r = l + Rect.right - Rect.left;
	::SetRect(&m_PointRect[0], l, intRound(0.060 * m_h), r, intRound(0.090 * m_h));
	//
	l += Rect.right - Rect.left;
	m_MemDC.MeasureRect("          000 Points Rejected", Rect, Font);
	r += Rect.right - Rect.left;
	::SetRect(&m_PointRect[1], l, intRound(0.060 * m_h), r, intRound(0.090 * m_h));
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

	if (m_AverageExam != 0)
	{
		for (m_d = 0; m_d < 2; m_d++)
		{
			CWndSettings* pWndSettings = GetWndSettings();
			pWndSettings->m_MapShowPupil = FALSE;
			pWndSettings->m_MapShowCornealVertex = FALSE;
			pWndSettings->m_MapShowLimbus = FALSE;
			pWndSettings->m_MapShowPoints = TRUE;
			pWndSettings->m_MapTranslucent = FALSE;
		}
	}


	m_pWFExam->m_CurShot = 0;
	m_pWFExam->m_CurPoint = 0;

	CScanPoint* pPoint = m_pWFExam->m_Points.MoveFirst();

	m_inc = TRUE;
	m_cnt = 0;
	m_TimerF4 = 0;
	m_TimerF7 = 0;

	m_d = -1;
	for (int i = 0; i < 4; i++) {
		if (m_pWndSettings[i].m_SizeSmall == FALSE) {
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

	//----------------------------------------------------

	int W = ::GetSystemMetrics(SM_CXSCREEN);

	CMRgn Rgn;

	int s = intRound(0.035 * ::GetSystemMetrics(SM_CXSCREEN));
	r = (m_Rect[4].left + m_Rect[4].right) / 2 + 3 * s;
	::SetRect(&Rect, r - s, m_Rect[4].bottom - s - 2, r, m_Rect[4].bottom - 2);

	BOOL DBIsReadOnly = ::DB.IsReadOnly();

	m_RejectButton.Create(_T(""), _T(""), 0, Rect, this, IDC_REJECT);
	m_RejectButton.SetImage(W >= 1920 ? IDR_REJECT_3 : W >= 1600 ? IDR_REJECT_2 : IDR_REJECT_1);
	m_RejectButton.SetBk(FALSE);
	m_RejectButton.SetBkColor(BLACK);
	m_RejectButton.SetTip("Reject point");
	m_RejectButton.SetEnabled(!DBIsReadOnly && !pPoint->m_Bad);
	Rgn.SetRectRgn(&Rect);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	Rect.left -= s; Rect.right -= s;
	m_AcceptButton.Create(_T(""), _T(""), 0, Rect, this, IDC_ACCEPT);
	m_AcceptButton.SetImage(W >= 1920 ? IDR_ACCEPT_3 : W >= 1600 ? IDR_ACCEPT_2 : IDR_ACCEPT_1);
	m_AcceptButton.SetBk(FALSE);
	m_AcceptButton.SetBkColor(BLACK);
	m_AcceptButton.SetTip("Accept point");
	m_AcceptButton.SetEnabled(!DBIsReadOnly && pPoint->m_Bad && !pPoint->m_AutoBad);
	Rgn.SetRectRgn(&Rect);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	Rect.left -= s; Rect.right -= s;
	m_NextButton.Create(_T(""), _T(""), 0, Rect, this, IDC_NEXT);
	m_NextButton.SetImage(W >= 1920 ? IDR_RIGHT_ARROW_3 : W >= 1600 ? IDR_RIGHT_ARROW_2 : IDR_RIGHT_ARROW_1);
	m_NextButton.SetBk(FALSE);
	m_NextButton.SetBkColor(BLACK);
	m_NextButton.SetTip("Next point");
	Rgn.SetRectRgn(&Rect);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	Rect.left -= s; Rect.right -= s;
	m_PrevButton.Create(_T(""), _T(""), 0, Rect, this, IDC_PREV);
	m_PrevButton.SetImage(W >= 1920 ? IDR_LEFT_ARROW_3 : W >= 1600 ? IDR_LEFT_ARROW_2 : IDR_LEFT_ARROW_1);
	m_PrevButton.SetBk(FALSE);
	m_PrevButton.SetBkColor(BLACK);
	m_PrevButton.SetTip("Previous point");
	Rgn.SetRectRgn(&Rect);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	if (m_pWFExam->GetNumShots() > 1) {
		Rect.left -= s; Rect.right -= s;
		m_Shot2Button.Create(_T(""), _T(""), 0, Rect, this, IDC_SHOT_2);
		m_Shot2Button.SetImage(W >= 1920 ? IDR_SHOT2_3 : W >= 1600 ? IDR_SHOT2_2 : IDR_SHOT2_1);
		m_Shot2Button.SetBk(FALSE);
		m_Shot2Button.SetBkColor(BLACK);
		m_Shot2Button.SetTip("2nd shot");
		m_Shot2Button.SetEnabled(m_pWFExam->m_CurShot != 1);
		Rgn.SetRectRgn(&Rect);
		m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

		Rect.left -= s; Rect.right -= s;
		m_Shot1Button.Create(_T(""), _T(""), 0, Rect, this, IDC_SHOT_1);
		m_Shot1Button.SetImage(W >= 1920 ? IDR_SHOT1_3 : W >= 1600 ? IDR_SHOT1_2 : IDR_SHOT1_1);
		m_Shot1Button.SetBk(FALSE);
		m_Shot1Button.SetBkColor(BLACK);
		m_Shot1Button.SetTip("1st shot");
		m_Shot1Button.SetEnabled(m_pWFExam->m_CurShot != 0);
		Rgn.SetRectRgn(&Rect);
		m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

		m_Shot1Button.ShowWindow(SW_SHOW);//Presbia
		m_Shot2Button.ShowWindow(SW_SHOW);//Presbia
	}

	m_PrevButton.ShowWindow(SW_SHOW);
	m_NextButton.ShowWindow(SW_SHOW);
	m_AcceptButton.ShowWindow(SW_SHOW);
	m_RejectButton.ShowWindow(SW_SHOW);
}

//***************************************************************************************

CWFSingleVrfWnd::~CWFSingleVrfWnd()
{
	if (m_TimerF4 != 0) KillTimer(m_TimerF4);
	if (m_TimerF7 != 0) KillTimer(m_TimerF7);
}

//***************************************************************************************

void CWFSingleVrfWnd::OnSizeLargeItemClicked()
{
	CWndSettings* pWndSettings = GetWndSettings();

	pWndSettings->m_SizeSmall = !pWndSettings->m_SizeSmall;

	for (int d = 0; d < 4; d++) SAFE_DELETE(m_pDispWnd[d]);

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

void CWFSingleVrfWnd::PointInfo()
{
	m_MemDC.FillSolidRect(&m_PointRect[0], m_Printing ? WHITE : BLACK);

	COLORREF Color = m_pWFExam->m_Points.Get(m_pWFExam->m_CurPoint)->m_Bad ? RED : (m_Printing ? BLACK : WHITE);

	CString s;
	s.Format(_T("Point %i"), m_pWFExam->m_CurPoint + 1);

	CMFont Font(intRound(1.1 * (2.0 * m_g)), 400, "Arial");

	m_MemDC.WriteText(s, m_PointRect[0], Font, Color, 0);

	// bad points info
	int NumBadPoints = m_pWFExam->m_Points.GetSize() - m_pWFExam->m_NumGoodPoints;
	if (NumBadPoints > 0) {

		COLORREF Color = (NumBadPoints < 10) ? (m_Printing ? ORANGE : YELLOW) : RED;

		s.Format(_T("%i Points Rejected"), NumBadPoints);

		m_MemDC.WriteText(s, m_PointRect[1], Font, Color, 0);
	}
}

//***************************************************************************************

void CWFSingleVrfWnd::RepaintMemDC()
{
	CSumWnd::RepaintMemDC();

	//531
	CMFont Font(intRound(1.1 * (2.0 * m_g)), 400, "Arial");
	if (m_ShowExamNum)
	{
		CString s;
		RECT Rect;
		::SetRect(&Rect, m_Rect[4].left, intRound(0.060 * m_h), m_Rect[4].right, intRound(0.090 * m_h));
		s.Format(_T(" Multi-WF Exam %i"), m_whichExam + 1);
		m_MemDC.WriteText(s, Rect, Font, MAGENTA, 0);
	}
	//531

	//BOOL Rows[44] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

	WFExamLargeInfo(m_pWFExam, m_Rect[4], 1);//530  Original is WFExamInfo

	PointInfo();
}

//***************************************************************************************

void CWFSingleVrfWnd::CreateChildWnd()
{
	SAFE_DELETE(m_pDispWnd[m_d]);

	CWndSettings* pWndSettings = GetWndSettings();

	switch (m_d) {
	case 0:
		pWndSettings->m_Type = TYPE_TEYE;
		break;
	case 1:
		/*if (!::Licensing.IsWorkstation() && !::Licensing.IsCombo())
		{
		pWndSettings->m_Type = TYPE_TRSD;
		}
		else
		{*///remove by 521
		if (pWndSettings->m_Type != TYPE_TOPM &&
			pWndSettings->m_Type != TYPE_TEYC) {
			pWndSettings->m_Type = TYPE_TRSD;
		}
		//}
		//pWndSettings->m_Type = TYPE_TOPM;
		break;
	case 2:
		//pWndSettings->m_Type = TYPE_TOPM;
		pWndSettings->m_Type = TYPE_TSGN;
		break;
	case 3:
		//pWndSettings->m_Type = TYPE_TOPM;
		pWndSettings->m_Type = TYPE_TSGN;
		break;
	}

	if (!::Settings.m_ShowWavetouch) {
		if (m_d == 0) {
			pWndSettings->m_MapShowWavetouchLens = FALSE;
		}
	}

	pWndSettings->m_Method3D = FALSE;

	if (pWndSettings->m_Type == TYPE_TEYE ||
		pWndSettings->m_Type == TYPE_TEYC) {
		pWndSettings->m_MapShowEye = TRUE;
	}
	if (pWndSettings->m_Type == TYPE_TEYC) {
		pWndSettings->m_MapShowPupil = FALSE;
		pWndSettings->m_MapShowCornealVertex = FALSE;
		pWndSettings->m_MapShowLimbus = FALSE;
		pWndSettings->m_MapShowPoints = FALSE;
		pWndSettings->m_MapShowWavetouchLens = FALSE;
	}
	if (pWndSettings->m_Type == TYPE_TOPM) {
		pWndSettings->m_MapShowMap = TRUE;
		pWndSettings->m_MapShowNumbers = FALSE;
	}

	RECT Rect = pWndSettings->m_SizeSmall ? m_Rect[m_d] : m_LargeRect;

	int SignalZoom = 8;
	for (; SignalZoom >= 2; SignalZoom--) {
		if ((1 << (16 - SignalZoom)) > m_pWFExam->m_SignalMax) break;
	}

	switch (pWndSettings->m_Type) {

	case TYPE_TEYE:
	{
		CEyeWnd* pEyeWnd = new CEyeWnd(Rect, this);
		pEyeWnd->m_Type = pWndSettings->m_Type;
		pEyeWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
		pEyeWnd->m_Method3D = pWndSettings->m_Method3D;
		pEyeWnd->m_MapShowEye = pWndSettings->m_MapShowEye;
		pEyeWnd->m_MapShowPupil = pWndSettings->m_MapShowPupil;
		pEyeWnd->m_MapShowCornealVertex = pWndSettings->m_MapShowCornealVertex;
		pEyeWnd->m_MapShowLimbus = FALSE;// pWndSettings->m_MapShowLimbus;//
		pEyeWnd->m_MapShowPoints = pWndSettings->m_MapShowPoints;
		pEyeWnd->m_MapShowWavetouchLens = pWndSettings->m_MapShowWavetouchLens;
		pEyeWnd->m_pWFExam = m_pWFExam;
		m_pDispWnd[m_d] = pEyeWnd;
	}
	break;
	case TYPE_TEYC:
	{
		CEyeWnd* pEyeWnd = new CEyeWnd(Rect, this);
		pEyeWnd->m_Type = pWndSettings->m_Type;
		pEyeWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
		pEyeWnd->m_Method3D = pWndSettings->m_Method3D;
		pEyeWnd->m_LTLabel[0] = "Color Image";
		pEyeWnd->m_MapShowEye = pWndSettings->m_MapShowEye;
		pEyeWnd->m_MapShowPupil = pWndSettings->m_MapShowPupil;
		pEyeWnd->m_MapShowCornealVertex = pWndSettings->m_MapShowCornealVertex;
		pEyeWnd->m_MapShowLimbus = FALSE;// pWndSettings->m_MapShowLimbus;//
		pEyeWnd->m_MapShowPoints = pWndSettings->m_MapShowPoints;
		pEyeWnd->m_MapShowWavetouchLens = pWndSettings->m_MapShowWavetouchLens;
		pEyeWnd->m_pWFExam = m_pWFExam;
		m_pDispWnd[m_d] = pEyeWnd;
	}
	break;

	case TYPE_TOPM:
	{
		//pWndSettings->SetDefaultColors(0);
		CEyeWnd* pEyeWnd = new CEyeWnd(Rect, this);
		pEyeWnd->m_Type = pWndSettings->m_Type;
		pEyeWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
		pEyeWnd->m_Method3D = pWndSettings->m_Method3D;
		pEyeWnd->m_LTLabel[0] = "Opacity Map";
		//pEyeWnd->m_LTLabel[0] += (m_d==2?" X":(m_d==3?" Y":""));
		pEyeWnd->m_MapShowEye = pWndSettings->m_MapShowEye;
		pEyeWnd->m_MapShowMap = pWndSettings->m_MapShowMap;
		pEyeWnd->m_MapShowNumbers = pWndSettings->m_MapShowNumbers;
		pEyeWnd->m_MapShowPupil = pWndSettings->m_MapShowPupil;
		pEyeWnd->m_MapShowCornealVertex = pWndSettings->m_MapShowCornealVertex;
		pEyeWnd->m_MapShowLimbus = FALSE;// pWndSettings->m_MapShowLimbus;//
		pEyeWnd->m_MapTranslucent = pWndSettings->m_MapTranslucent;
		pEyeWnd->m_Unit = "";
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

			pEyeWnd->m_LTLabel[1] = "Grade = " + GradeStr;
		}
		else
		{
			pEyeWnd->m_LTLabel[1] = "Grade is unavailable";
		}
		//531


		m_pDispWnd[m_d] = pEyeWnd;
	}
	break;

	case TYPE_TRSD:
	{
		CRSDWnd* pRSDWnd = new CRSDWnd(Rect, this);
		pRSDWnd->m_Type = pWndSettings->m_Type;
		pRSDWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
		pRSDWnd->m_Method3D = pWndSettings->m_Method3D;
		pRSDWnd->m_LTLabel[0] = "Retinal Spot Diagram";
		pRSDWnd->m_pWFExam = m_pWFExam;
		pRSDWnd->m_Surface = m_pWFExam->m_WfSurface;
		pRSDWnd->CreateZoomButtons();
		m_pDispWnd[m_d] = pRSDWnd;
	}
	break;

	case TYPE_TSGN:
	{
		CSignalWnd* pSignalWnd = new CSignalWnd(Rect, this);
		pSignalWnd->m_Type = pWndSettings->m_Type;
		pSignalWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
		pSignalWnd->m_Method3D = pWndSettings->m_Method3D;
		pSignalWnd->m_LTLabel[0] = m_d == 2 ? "Horizontal Point Profile" : "Vertical Point Profile";
		pSignalWnd->m_pWFExam = m_pWFExam;
		pSignalWnd->m_Horz = m_d == 2;
		pSignalWnd->m_Zoom = SignalZoom;
		pSignalWnd->CreateZoomButtons();
		m_pDispWnd[m_d] = pSignalWnd;
	}
	break;

	}

}

//***************************************************************************************

void CWFSingleVrfWnd::OnShot1ButtonClicked()
{
	int NumShots = m_pWFExam->GetNumShots();
	m_Shot1Button.SetEnabled(FALSE);
	m_Shot2Button.SetEnabled(TRUE);
	m_Shot1Button.Repaint();
	m_Shot2Button.Repaint();
	ChangeShot(0);
}

void CWFSingleVrfWnd::OnShot2ButtonClicked()
{
	int NumShots = m_pWFExam->GetNumShots();
	m_Shot1Button.SetEnabled(TRUE);
	m_Shot2Button.SetEnabled(FALSE);
	m_Shot1Button.Repaint();
	m_Shot2Button.Repaint();
	ChangeShot(1);
}

void CWFSingleVrfWnd::ChangeShot(int shot)
{
	m_pWFExam->m_CurShot = shot;
	m_pWFExam->FstPoint();
	ChangePoint(TRUE);
}

//***************************************************************************************

void CWFSingleVrfWnd::OnPrevButtonClicked()
{
	m_pWFExam->PrvPoint();
	ChangePoint(FALSE);
}

void CWFSingleVrfWnd::OnNextButtonClicked()
{
	m_pWFExam->NxtPoint();
	ChangePoint(FALSE);
}

void CWFSingleVrfWnd::ChangePoint(BOOL ChangeShot)
{
	CScanPoint* pPoint = m_pWFExam->m_Points.Get(m_pWFExam->m_CurPoint);

	if (!::DB.IsReadOnly()) {
		m_AcceptButton.SetEnabled(pPoint->m_Bad && !pPoint->m_AutoBad);
		m_RejectButton.SetEnabled(!pPoint->m_Bad);
		m_AcceptButton.Repaint();
		m_RejectButton.Repaint();
	}

	if (m_pDispWnd[0]) {
		if (ChangeShot) {
			m_pDispWnd[0]->Repaint();
		}
		else {
			((CEyeWnd*)m_pDispWnd[0])->ChangePoint();
		}
	}
	if (m_pDispWnd[1]) {
		if (m_pWndSettings[1].m_Type == TYPE_TRSD) {
			m_pDispWnd[1]->Repaint();
		}
		else if (m_pWndSettings[1].m_Type == TYPE_TOPM) {
			if (ChangeShot) {
				CEyeWnd* pEyeWnd = ((CEyeWnd*)m_pDispWnd[1]);
				m_pWFExam->CreateOpm2D(pEyeWnd->m_Map2D, 1.0 / pEyeWnd->m_y_px_um, m_pWFExam->m_CurShot, 2);
				m_pDispWnd[1]->Repaint();
			}
		}
	}
	if (m_pDispWnd[2]) {
		m_pDispWnd[2]->Repaint();
	}
	if (m_pDispWnd[3]) {
		m_pDispWnd[3]->Repaint();
	}

	PointInfo();
	InvalidateRect(&m_PointRect[0], FALSE);
	InvalidateRect(&m_PointRect[1], FALSE);
}

//***************************************************************************************

void CWFSingleVrfWnd::OnAccept(BOOL accept)
{
	CScanPoint* pPoint = m_pWFExam->m_Points.Get(m_pWFExam->m_CurPoint);
	pPoint->m_Bad = !accept;

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

	m_AcceptButton.SetEnabled(pPoint->m_Bad && !pPoint->m_AutoBad);
	m_RejectButton.SetEnabled(!pPoint->m_Bad);
	m_AcceptButton.Repaint();
	m_RejectButton.Repaint();

	if ((m_pWndSettings + 1)->m_Type == TYPE_TRSD) {
		CRSDWnd* pWnd = (CRSDWnd*)m_pDispWnd[1];
		if (pWnd) {
			pWnd->m_Surface = m_pWFExam->m_WfSurface;
		}
	}

	Repaint();

	if (m_pWFExam->m_Header.m_Saved) ::DB.SaveWFExam(m_pWFExam);
}

//***************************************************************************************

void CWFSingleVrfWnd::AddDefocus(real_t SphEq)
{
	if ((m_pWndSettings + 1)->m_Type == TYPE_TRSD) {
		CRSDWnd* pWnd = (CRSDWnd*)m_pDispWnd[1];
		if (pWnd) {
			pWnd->m_AddedSphEq += SphEq;
			pWnd->m_Surface.AddSphEq(SphEq);
			pWnd->Repaint();
		}
	}
}

void CWFSingleVrfWnd::OnF1()
{
	AddDefocus(-0.1);
}

void CWFSingleVrfWnd::OnF2()
{
	AddDefocus(0.1);
}

void CWFSingleVrfWnd::OnF3()
{
	if ((m_pWndSettings + 1)->m_Type == TYPE_TRSD) {
		CRSDWnd* pWnd = (CRSDWnd*)m_pDispWnd[1];
		if (pWnd) {
			pWnd->m_AddedSphEq = 0.0;
			pWnd->m_Surface.m_c[4] = m_pWFExam->m_WfSurface.m_c[4];
			pWnd->Repaint();
		}
	}
	m_cnt = 0;
	m_inc = TRUE;
}

//***************************************************************************************

void CWFSingleVrfWnd::OnF4()
{
	if (m_TimerF4 == 0) {
		if (m_TimerF7 != 0) {
			KillTimer(m_TimerF7);
			m_TimerF7 = 0;
		}
		m_TimerF4 = SetTimer(1, 20, NULL);
	}
	else {
		KillTimer(m_TimerF4);
		m_TimerF4 = 0;
	}
}

void CWFSingleVrfWnd::OnF7()
{
	if (m_TimerF7 == 0) {
		if (m_TimerF4 != 0) {
			KillTimer(m_TimerF4);
			m_TimerF4 = 0;
		}
		m_cur_shot_point = 0;
		OnShot1ButtonClicked();
		m_TimerF7 = SetTimer(1, 20, NULL);
	}
	else {
		KillTimer(m_TimerF7);
		m_TimerF7 = 0;
	}
}

void CWFSingleVrfWnd::OnTimer(UINT_PTR Event)
{
	if (Event == m_TimerF4) {
		if (m_inc) {
			if (m_cnt < 100) {
				m_cnt++;
				OnF2();
			}
			else {
				m_cnt--;
				m_inc = FALSE;
				OnF1();
			}
		}
		else {
			if (m_cnt > -100) {
				m_cnt--;
				OnF1();
			}
			else {
				m_cnt++;
				m_inc = TRUE;
				OnF2();
			}
		}
	}

	else if (Event == m_TimerF7) {
		if (m_cur_shot_point == m_pWFExam->GetNumShotPoints()) {
			m_cur_shot_point = 0;
			if (m_pWFExam->m_CurShot == 0 && m_pWFExam->GetNumShots() > 1) {
				OnShot2ButtonClicked();
			}
			else {
				OnShot1ButtonClicked();
				KillTimer(m_TimerF7);
				m_TimerF7 = 0;
			}
		}
		else {
			m_cur_shot_point++;
			OnNextButtonClicked();
		}
	}
}

//***************************************************************************************

void CWFSingleVrfWnd::CreateMenu()
{
	//BOOL IsCustomvis = ::Settings.m_IsCustomvis;
	BOOL IsWavetouch = ::Settings.m_ShowWavetouch;

	CWndSettings* pWndSettings = GetWndSettings();

	if (m_d == 0)
	{

		if (m_AverageExam != 0)
		{
			m_pMenu->AppendItem(IDC_SELECT_IMAGE_ITEM, FALSE, FALSE, "Select eye image", "");
			m_pMenu->AppendSeparator();
		}


		m_pMenu->AppendItem(IDC_SIZE_LARGE_ITEM, FALSE, !pWndSettings->m_SizeSmall, "Large window", "");
		m_pMenu->AppendSeparator();

		if (m_pWFExam->m_Image.m_RGBData)
		{
			m_pMenu->AppendItem(IDC_SHOW_MAP_PUPIL_ITEM, FALSE, pWndSettings->m_MapShowPupil, "Show pupil", "");
			m_pMenu->AppendItem(IDC_SHOW_MAP_VERTEX_ITEM, FALSE, pWndSettings->m_MapShowCornealVertex, "Show corneal vertex", "");
			//m_pMenu->AppendItem(IDC_SHOW_MAP_LIMBUS_ITEM, FALSE, pWndSettings->m_MapShowLimbus, "Show limbus", "");//
		}


		m_pMenu->AppendItem(IDC_SHOW_MAP_POINTS_ITEM, FALSE, pWndSettings->m_MapShowPoints, "Show scan points", "");
		if (IsWavetouch) {
			m_pMenu->AppendItem(IDC_SHOW_MAP_LENS_ITEM, FALSE, pWndSettings->m_MapShowWavetouchLens, "Show lens dots", "");
		}

		if (m_pWFExam->m_Image.m_RGBData)
		{
			m_pMenu->AppendSeparator();
			m_pMenu->AppendItem(IDC_EDIT_LIMBUS_ITEM, FALSE, FALSE, "Edit pupil and vertex", "");
		}

		if (IsWavetouch) {
			m_pMenu->AppendItem(IDC_EDIT_LENS_DOTS_ITEM, FALSE, FALSE, "Edit lens dots", "");
		}
	}

	else if (m_d == 1)
	{

		if (m_AverageExam != 0)
		{
			m_pMenu->AppendItem(IDC_SELECT_IMAGE_ITEM, FALSE, FALSE, "Select eye image", "");
			m_pMenu->AppendSeparator();
		}


		{
			//if (IsCustomvis) {
			//  m_pMenu->AppendItem(IDC_TYPE_TCEY_ITEM, pWndSettings->m_Type == TYPE_TEYC, FALSE, "Color eye image", "");
			//}
			m_pMenu->AppendItem(IDC_TYPE_TRSD_ITEM, pWndSettings->m_Type == TYPE_TRSD, FALSE, "Retinal Spot Diagram", "");
			m_pMenu->AppendItem(IDC_TYPE_TOPM_ITEM, pWndSettings->m_Type == TYPE_TOPM, FALSE, "Opacity Map", "");
			m_pMenu->AppendSeparator();
		}
		m_pMenu->AppendItem(IDC_SIZE_LARGE_ITEM, FALSE, !pWndSettings->m_SizeSmall, "Large window", "");
		if (pWndSettings->m_Type == TYPE_TOPM) {
			m_pMenu->AppendSeparator();


			if (m_pWFExam->m_Image.m_RGBData)
			{
				m_pMenu->AppendItem(IDC_SHOW_MAP_EYE_ITEM, FALSE, pWndSettings->m_MapShowEye, "Show eye image", "");
				m_pMenu->AppendItem(IDC_SHOW_MAP_PUPIL_ITEM, FALSE, pWndSettings->m_MapShowPupil, "Show pupil", "");
				m_pMenu->AppendItem(IDC_SHOW_MAP_VERTEX_ITEM, FALSE, pWndSettings->m_MapShowCornealVertex, "Show corneal vertex", "");
				//m_pMenu->AppendItem(IDC_SHOW_MAP_LIMBUS_ITEM, FALSE, pWndSettings->m_MapShowLimbus, "Show limbus", "");//
			}

			m_pMenu->AppendItem(IDC_MAP_TRANSLUCENT_ITEM, FALSE, pWndSettings->m_MapTranslucent, "Translucent map", "");
			CXPMenu* pSubMenu = m_pMenu->AppendSubmenu("Map colors", "");
			BOOL DefaultColors = pWndSettings->AreDefaultColors(0);
			pSubMenu->AppendItem(IDC_MAP_COLORS1_ITEM, DefaultColors, FALSE, "Default", "");
			pSubMenu->AppendItem(IDC_MAP_COLORS_USER_ITEM, !DefaultColors, FALSE, "Custom...", "");
		}
	}


	if (m_d == 0 && !SameVrf0())
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
	}
	if (m_d == 1 && !SameVrf1())
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
	}

}

//***************************************************************************************

void CWFSingleVrfWnd::OnEditLimbusItemClicked()
{
	CEyeImage* pImage = &m_pWFExam->m_Image;

	if (pImage->m_RGBData.GetMem() == NULL) return;

	CImageDlg* pDlg = new CImageDlg(this, pImage, TRUE, FALSE);//2

	if (pDlg->DoModal() == IDOK) {

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

		Repaint();

		if (m_pWFExam->m_Header.m_Saved) ::DB.SaveWFExam(m_pWFExam);
	}

	delete pDlg;
}

//***************************************************************************************

void CWFSingleVrfWnd::OnEditLensDotsItemClicked()
{
	CEyeImage* pImage = &m_pWFExam->m_Image;

	if (pImage->m_RGBData.GetMem() == NULL) return;

	CLensDlg* pDlg = new CLensDlg(this, pImage);

	if (pDlg->DoModal() == IDOK) {

		if (m_pWFExam->m_Header.m_Saved) ::DB.SaveWFExam(m_pWFExam);

		Repaint();
	}

	delete pDlg;
}


//***************************************************************************************

void CWFSingleVrfWnd::OnSelEyeImgItemClicked()
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
		pWndSettings->m_MapShowPupil = TRUE;
		pWndSettings->m_MapShowCornealVertex = TRUE;
		pWndSettings->m_MapShowLimbus = FALSE;// From TRUE to FALSE
		pWndSettings->m_MapShowPoints = TRUE;
		pWndSettings->m_MapShowEye = TRUE;

		pWndSettings->m_MapTranslucent = FALSE;

		m_pWFExam->m_CurShot = 0;
		m_pWFExam->m_CurPoint = 0;

		CScanPoint* pPoint = m_pWFExam->m_Points.MoveFirst();

		m_inc = TRUE;
		m_cnt = 0;
		m_TimerF4 = 0;
		m_TimerF7 = 0;

		m_d = -1;
		for (int i = 0; i < 4; i++) {
			if (m_pWndSettings[i].m_SizeSmall == FALSE) {
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
	}
	else delete pDlg;
}

//***************************************************************************************


//***************************************************************************************

void CWFSingleVrfWnd::OnShowDefaultItemClicked()
{
	CBusyCursor Cursor;

	m_pWndSettings[m_d].m_LtrDistanceType = 0;//530

	if (m_d == 0)//WF Point display
	{
		m_pWndSettings[0].m_MapShowCornealVertex = 1;
		m_pWndSettings[0].m_MapShowPoints = 1;
		m_pWndSettings[0].m_MapShowPupil = 1;
		m_pWndSettings[0].m_MapShowLimbus = FALSE;// From TRUE to FALSE
		m_pWndSettings[0].m_SizeSmall = TRUE;
	}

	if (m_d == 1)
	{
		m_pWndSettings[1].m_Type = TYPE_TRSD;
		m_pWndSettings[1].m_SizeSmall = TRUE;
	}

	for (m_d = 0; m_d < 4; m_d++)
	{
		CreateChildWnd();
	}

	Repaint();
}

//***************************************************************************************

BOOL CWFSingleVrfWnd::SameVrf0()
{
	if (m_pWFExam->m_Image.m_RGBData && m_pWndSettings[0].m_MapShowCornealVertex != 1) return FALSE;
	if (m_pWndSettings[0].m_MapShowPoints != 1)                                 return FALSE;
	if (m_pWFExam->m_Image.m_RGBData && m_pWndSettings[0].m_MapShowPupil != 1) return FALSE;
	//if(m_pWFExam->m_Image.m_RGBData && m_pWndSettings[0].m_MapShowLimbus        != 1) return FALSE;
	if (m_pWndSettings[0].m_SizeSmall != 1) return FALSE;

	return TRUE;
}

//***************************************************************************************

BOOL CWFSingleVrfWnd::SameVrf1()
{
	if (m_pWndSettings[1].m_Type != TYPE_TRSD) return FALSE;
	if (m_pWndSettings[1].m_SizeSmall != 1) return FALSE;

	return TRUE;
}

//***************************************************************************************
