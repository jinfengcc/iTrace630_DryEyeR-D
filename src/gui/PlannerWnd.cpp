//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "PlannerWnd.h"
#include "PreopDataDlg.h"
#include "PreopComboDataDlg.h"//
#include "BusyCursor.h"
#include "ToricSelDlg.h"
#include "ToricSelDlg2.h"//
#include "FolderDlg.h"//
#include "GlobalFunctions.h"
#include "zip.h"//

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CPlannerWnd, CSumWnd)

	ON_WM_MOUSEMOVE()//7.0.0 onmousemove

	ON_MESSAGE(WM_UPDATE_INFO, OnUpdateInfoMsg)
	ON_BN_CLICKED(IDC_PREOP_DATA_BUTTON, OnDataButtonClicked)

	ON_MESSAGE(WM_NOTIFY_LBTN_DBCLK, OnBigWndShown)//[520]

	ON_BN_CLICKED(IDC_EXPORT_TORIC_DATA_BUTTON, OnESDButtonClicked) //[520] export surgeon data for iTraceTPS

	ON_BN_CLICKED(IDC_SHOW_MAP_COLOR_EYE_ITEM, OnShowColorEyeClicked)

	ON_BN_CLICKED(IDC_SHOW_MAP_GRAY_EYE_ITEM, OnShowGrayEyeClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CPlannerWnd::CPlannerWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CCTExam* pCTExam, CWndSettings* pWndSettings, int OD) :
	CSumWnd(pWnd, WndRect, pPatient, pWndSettings)
{
	CBusyCursor Cursor;

	m_Patient = pPatient;//7.0.0

	m_OD = OD;
	m_ESDBtnCreated = 0;

	m_PatientIOL.PatientID = pPatient->m_PatientID;

	CGroup* pGroup = ::DB.GetGroup(pPatient->m_GroupID);
	if (pGroup)
	{
		CStringA AGroupName(pGroup->m_Name);
		strncpy(m_PatientIOL.GroupName, AGroupName, 50);
	}
	else
	{
		strncpy(m_PatientIOL.GroupName, "", 50);
	}

	CStringA AID(pPatient->m_CustomID);
	CStringA ALN(pPatient->m_LastName);
	CStringA AFN(pPatient->m_FirstName);
	CStringA AMN(pPatient->m_MiddleName);

	strncpy(m_PatientIOL.CustomID, AID, 50);
	strncpy(m_PatientIOL.LastName, ALN, 50);
	strncpy(m_PatientIOL.FirstName, AFN, 50);
	strncpy(m_PatientIOL.MiddleName, AMN, 50);

	m_PatientIOL.Sex = pPatient->m_Sex;

	m_PatientIOL.BirthYear = pPatient->m_BirthYear;
	m_PatientIOL.BirthMonth = pPatient->m_BirthMonth;
	m_PatientIOL.BirthDay = pPatient->m_BirthDay;

	CStringA AInfo(pPatient->m_Address.m_Line1);
	strncpy(m_PatientIOL.Address.Line1, AInfo, 50);
	CStringA AInfo1(pPatient->m_Address.m_Line2);
	strncpy(m_PatientIOL.Address.Line2, AInfo1, 50);
	CStringA AInfo2(pPatient->m_Address.m_City);
	strncpy(m_PatientIOL.Address.City, AInfo2, 50);
	CStringA AInfo3(pPatient->m_Address.m_State);
	strncpy(m_PatientIOL.Address.State, AInfo3, 50);
	CStringA AInfo4(pPatient->m_Address.m_ZIP);
	strncpy(m_PatientIOL.Address.ZIP, AInfo4, 20);
	CStringA AInfo5(pPatient->m_Address.m_Country);
	strncpy(m_PatientIOL.Address.Country, AInfo5, 50);
	CStringA AInfo6(pPatient->m_HomePhone);
	strncpy(m_PatientIOL.HomePhone, AInfo6, 20);
	CStringA AInfo7(pPatient->m_WorkPhone);
	strncpy(m_PatientIOL.WorkPhone, AInfo7, 20);
	CStringA AInfo8(pPatient->m_EMail);
	strncpy(m_PatientIOL.EMail, AInfo8, 50);
	CStringA AInfo9(pPatient->m_Note);
	strncpy(m_PatientIOL.Note, AInfo9, 100);

	if (pPatient->m_RegYear < 3010 && pPatient->m_RegYear > 0)
	{
		m_PatientIOL.RegYear = pPatient->m_RegYear;
		m_PatientIOL.RegMonth = pPatient->m_RegMonth;
		m_PatientIOL.RegDay = pPatient->m_RegDay;
		m_PatientIOL.RegHour = pPatient->m_RegHour;
		m_PatientIOL.RegMinute = pPatient->m_RegMinute;
	}
	else
	{
		m_PatientIOL.RegYear = 0;
		m_PatientIOL.RegMonth = 0;
		m_PatientIOL.RegDay = 0;
		m_PatientIOL.RegHour = 0;
		m_PatientIOL.RegMinute = 0;
	}

	RECT Rect;
	GetWindowRect(&Rect);
	real_t t = 3.0 * (0.03 * m_h) + m_g;
	real_t w = ((Rect.right - Rect.left) - 4.0 * m_g) / 3.0;
	real_t h = ((Rect.bottom - Rect.top - t) - 2.0 * m_g) / 2.0;
	::SetRect(&m_Rect[0], intRound(m_g + w + m_g + w + m_g), intRound(t), intRound(m_g + w + m_g + w + m_g + w), intRound(t + h));
	::SetRect(&m_Rect[1], intRound(m_g + w + m_g + w + m_g), intRound(t + h + m_g), intRound(m_g + w + m_g + w + m_g + w), intRound(t + h + m_g + h));
	::SetRect(&m_LargeRect, intRound(m_g), intRound(t), intRound(m_g + w + m_g + w), intRound(t + h + m_g + h));

	m_pCTExam = pCTExam;

	if (m_pCTExam->m_ColorImage.m_RGBData.GetMem() != NULL)
	{
		if (pWndSettings->m_MapShowEye)
		{
			m_ShowGrayEye = 0;
			m_ShowColorEye = 1;
		}
		else
		{
			m_ShowGrayEye = 0;
			m_ShowColorEye = 0;
		}
	}
	else
	{
		if (pWndSettings->m_MapShowEye)
		{
			m_ShowGrayEye = 1;
			m_ShowColorEye = 0;
		}
		else
		{
			m_ShowGrayEye = 0;
			m_ShowColorEye = 0;
		}
	}

	CreateChildWnd();

	Repaint();

	ShowWindow(SW_SHOW);


	CMRgn Rgn;

	t = 0.02 * ::GetSystemMetrics(SM_CXSCREEN);
	int h1 = intRound(t);
	int w1 = intRound(5.5 * t);
	::SetRect(&Rect, m_Rect[1].left + 2, m_Rect[1].bottom - h1 - 2, m_Rect[1].left + 2 + w1, m_Rect[1].bottom - 2);

	int fs = intRound(0.02 * ::GetSystemMetrics(SM_CYSCREEN));

	m_DataButton.SetBkColor(NOCOLOR);
	m_DataButton.SetText("Toric Calculator");
	m_DataButton.SetFont(fs);
	m_DataButton.Create(_T(""), _T(""), WS_VISIBLE, Rect, this, IDC_PREOP_DATA_BUTTON);
	Rgn.SetRectRgn(&Rect);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	::SetRect(&Rect, m_Rect[1].right - 2 - w1, m_Rect[1].bottom - h1 - 2, m_Rect[1].right - 2, m_Rect[1].bottom - 2);
	m_ExportSurDateButton.SetBkColor(NOCOLOR);
	m_ExportSurDateButton.SetText("Export PVS Data");
	m_ExportSurDateButton.SetTip("Export PVS Data for Surgery");
	m_ExportSurDateButton.SetFont(fs);
	m_ExportSurDateButton.Create(_T(""), _T(""), WS_VISIBLE, Rect, this, IDC_EXPORT_TORIC_DATA_BUTTON);
	Rgn.SetRectRgn(&Rect);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	m_ExportSurDateButton.ShowWindow(SW_HIDE);

	//if (m_pCTExam->m_OpData.m_OpType == 2 ) m_ExportSurDateButton.ShowWindow(SW_HIDE);
	//else m_ExportSurDateButton.ShowWindow(SW_SHOW);

	m_ESDBtnCreated = 1;
}

//***************************************************************************************


LRESULT CPlannerWnd::OnUpdateInfoMsg(WPARAM wParam, LPARAM lParam)
{
	BOOL Rows[22] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

	PreopData(m_Rect[1], 22, Rows);

	InvalidateRect(&m_Rect[1], FALSE);
	UpdateWindow();

	return 0;
}

//***************************************************************************************

LRESULT CPlannerWnd::OnBigWndShown(WPARAM wParam, LPARAM lParam)
{
	GetParent()->PostMessage(WM_NOTIFY_LBTN_DBCLK, (WPARAM)0, (LPARAM)0);

	return 0;
}

//***************************************************************************************

void CPlannerWnd::RepaintMemDC()
{
	CSumWnd::RepaintMemDC();

	/* if(m_ESDBtnCreated)
	{
	if (m_pCTExam->m_OpData.m_OpType == 2 ) m_ExportSurDateButton.ShowWindow(SW_HIDE);
	else m_ExportSurDateButton.ShowWindow(SW_SHOW);
	}*/

	BOOL Rows[22] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	CTExamInfo(m_pCTExam, m_Rect[0], 22, Rows);

	PreopData(m_Rect[1], 22, Rows);
}

//***************************************************************************************

void CPlannerWnd::CreateChildWnd()
{
	for (int d = 0; d < 3; d++) SAFE_DELETE(m_pDispWnd[d]);

	CWndSettings* pWndSettings = GetWndSettings();

	CPlanWnd* pPlanWnd = new CPlanWnd(m_LargeRect, this);

	pPlanWnd->m_pCTExam = m_pCTExam;

	pPlanWnd->m_ShowGrayEye = m_ShowGrayEye;
	pPlanWnd->m_ShowColorEye = m_ShowColorEye;

	pPlanWnd->m_ShowEye = pWndSettings->m_MapShowEye;
	pPlanWnd->m_ShowMap = pWndSettings->m_MapShowMap;
	if (!m_isCombo) pPlanWnd->m_ShowCaliper = pWndSettings->m_MapShowZaldivarCaliper;

	pPlanWnd->m_ShowOsherRing = pWndSettings->m_MapShowOsherRing;

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

//***************************************************************************************

void CPlannerWnd::PreopData(RECT Rect, int NumRows, BOOL* Rows)
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF black = m_Printing ? WHITE : BLACK;
	COLORREF light_gray = m_Printing ? DARK_GRAY : LIGHT_GRAY;
	COLORREF dark_gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;
	COLORREF light_yellow = m_Printing ? 0x0000f8f8 : 0x008fffff;

	m_MemDC.DrawRectangle(Rect, dark_gray, black);

	CString s;
	int num = NumRows;

	real_t h = (Rect.bottom - Rect.top) / ((real_t)num + 1);

	CMFont Font(intRound(1 * h), 400, "Arial");

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
	if (m_pCTExam->m_OpData.m_CorneaCycliderAdj)// asdfa
	{
		m_MemDC.WriteText(" Pre-Op Corneal Astigmatism", Rect, Font, white, 0);

		m_MemDC.MeasureRect(" Pre-Op Corneal Astigmatism ", Rect1, Font);
		w1 = Rect1.right - Rect1.left;


		m_MemDC.MeasureRect(" (Adjustment Factor Used) ", Rect1, Font);
		w2 = Rect1.right - Rect1.left;

		int NewL = L + w1;
		int NewR = NewL + w2;

		::SetRect(&Rect, NewL, intRound(t), NewR, intRound(t + h + 1));
		m_MemDC.WriteText(" (Adjustment Factor Used) ", Rect, Font, ORANGE, 0);
	}
	else
	{
		m_MemDC.WriteText(" Pre-Op Corneal Astigmatism", Rect, Font, white, 0);
	}

	if (m_pCTExam->m_OpData.m_CorneaCycliderAdj)
	{
		if (m_pCTExam->m_OpData.m_Pri_CorneaPreopCyl != INVALID_VALUE)
		{
			::SetRect(&Rect, L, intRound(t), R4, intRound(t + h + 1));
			s.Format(_T("%.2f D x "), m_pCTExam->m_OpData.m_Pri_CorneaPreopCyl);
			m_MemDC.WriteText(s, Rect, Font, white, 2);
		}

		if (m_pCTExam->m_OpData.m_Pri_CorneaPreopAxis != INVALID_VALUE)
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));

			s.Format(_T("%i° "), m_pCTExam->m_OpData.m_Pri_CorneaPreopAxis);

			m_MemDC.WriteText(s, Rect, Font, white, 2);
		}
	}
	else
	{
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
	t += h;

	if (m_pCTExam->m_OpData.m_OpType == 0)
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Model", Rect, Font, white, 0);

		::SetRect(&Rect, L, intRound(t), R1, intRound(t + h + 1));
		m_MemDC.WriteText("Cyl Power ", Rect, Font, white, 2);

		::SetRect(&Rect, L, intRound(t), R3, intRound(t + h + 1));
		m_MemDC.WriteText("Post-Op IO Ast ", Rect, Font, white, 2);

		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText("Post-Op Ent Eye Ast ", Rect, Font, white, 2);
	}

	else if (m_pCTExam->m_OpData.m_OpType == 2)
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" ICL's Astigmatism", Rect, Font, white, 0);
		if (m_pCTExam->m_OpData.m_ICL.m_Cyl != INVALID_VALUE)
		{
			::SetRect(&Rect, L, intRound(t), R4, intRound(t + h + 1));
			s.Format(_T("%.2f D x "), m_pCTExam->m_OpData.m_ICL.m_Cyl);
			m_MemDC.WriteText(s, Rect, Font, white, 2);
		}
		if (m_pCTExam->m_OpData.m_ICL.m_Axis != INVALID_VALUE)
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			s.Format(_T("%i° "), m_pCTExam->m_OpData.m_ICL.m_Axis);
			m_MemDC.WriteText(s, Rect, Font, white, 2);
		}
	}

	if (m_pCTExam->m_OpData.m_OpType == 0)
	{
		//7.0.0
		int BestIOL = m_pCTExam->m_OpData.m_CorneaCycliderAdj != TRUE ? m_pCTExam->m_OpData.m_NewBestIOL : m_pCTExam->m_OpData.m_NewPri_BestIOL;

		int FirstIOL = BestIOL - 3;
		int LastIOL = BestIOL + 3;

		if (FirstIOL < 0)
		{
			LastIOL -= FirstIOL;
			FirstIOL = 0;
		}
		else if (LastIOL > 30)
		{
			FirstIOL -= (LastIOL - 30);
			LastIOL = 30;
		}
		//7.0.0

		int Order = 0;
		for (int i = FirstIOL; i <= LastIOL; i++)
		{
			t += h;

			CIOL* pIOL;

			//
			if (m_pCTExam->m_OpData.m_CorneaCycliderAdj)
			{
				pIOL = &m_pCTExam->m_OpData.m_NewPri_IOLs[i];
			}
			else
			{
				pIOL = &m_pCTExam->m_OpData.m_NewIOLs[i];
			}
			//

			BOOL IOLCalculated = pIOL->m_EyePostopCyl != INVALID_VALUE;

			COLORREF Color = GRAY;

			if (Order == m_pCTExam->m_OpData.m_SelectedIOL) Color = white;

			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			s = " " + pIOL->m_Name;
			m_MemDC.WriteText(s, Rect, Font, Color, 0);

			::SetRect(&Rect, L, intRound(t), R1, intRound(t + h + 1));
			s.Format(_T("%.2f D "), pIOL->m_Cyl);
			m_MemDC.WriteText(s, Rect, Font, Color, 2);

			if (fabs(pIOL->m_Cyl) > 0.001)
			{
				::SetRect(&Rect, L, intRound(t), R2, intRound(t + h + 1));
				s.Format(_T("%.2f D x "), pIOL->m_InternalPostopCyl);
				m_MemDC.WriteText(s, Rect, Font, Color, 2);

				::SetRect(&Rect, L, intRound(t), R3, intRound(t + h + 1));
				s.Format(_T("%i° "), pIOL->m_InternalPostopAxis);
				m_MemDC.WriteText(s, Rect, Font, Color, 2);
			}

			if (IOLCalculated)
			{
				::SetRect(&Rect, L, intRound(t), R4, intRound(t + h + 1));
				s.Format(_T("%.2f D x "), pIOL->m_EyePostopCyl);
				m_MemDC.WriteText(s, Rect, Font, Color, 2);

				::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
				s.Format(_T("%i° "), pIOL->m_EyePostopAxis);
				m_MemDC.WriteText(s, Rect, Font, Color, 2);
			}

			Order++;
		}
	}

	else if (m_pCTExam->m_OpData.m_OpType == 1)
	{
		t -= 4 * h;
	}
	else if (m_pCTExam->m_OpData.m_OpType == 2)
	{
		t += h;

		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" ICL Induced Astigmatism", Rect, Font, white, 0);
		if (m_pCTExam->m_OpData.m_ICL.m_InducedCyl != INVALID_VALUE)
		{
			::SetRect(&Rect, L, intRound(t), R4, intRound(t + h + 1));
			s.Format(_T("%.2f D x "), m_pCTExam->m_OpData.m_ICL.m_InducedCyl);
			m_MemDC.WriteText(s, Rect, Font, white, 2);
		}
		if (m_pCTExam->m_OpData.m_ICL.m_InducedAxis != INVALID_VALUE)
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			s.Format(_T("%i° "), m_pCTExam->m_OpData.m_ICL.m_InducedAxis);
			m_MemDC.WriteText(s, Rect, Font, white, 2);
		}

		t += 7 * h;
	}

	t += h;
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

void CPlannerWnd::OnDataButtonClicked()
{
	BOOL OK;
	if (m_isCombo)
	{
		CString PatientName = G_ReadChinese(m_Patient->m_FirstName) + "." + G_ReadChinese(m_Patient->m_LastName);
		CString PatientAge = GetPatientAge(m_Patient);

		CPreopComboDataDlg* pDlg = new CPreopComboDataDlg(this, &m_pCTExam->m_OpData, m_OD, PatientName, PatientAge);
		OK = pDlg->DoModal() == IDOK;
		delete pDlg;
	}
	else
	{
		CPreopDataDlg* pDlg = new CPreopDataDlg(this, &m_pCTExam->m_OpData, m_OD);
		OK = pDlg->DoModal() == IDOK;
		delete pDlg;
	}

	if (OK)
	{
		CBusyCursor Cursor;

		Repaint();
	}
}


//***************************************************************************************

//7.0.0
CString CPlannerWnd::GetPatientAge(CPatient* m_Patient)
{
	CString res;

	SYSTEMTIME CurTime;
	::GetLocalTime(&CurTime);
	int age = CurTime.wYear - m_Patient->m_BirthYear;

	if (CurTime.wMonth < m_Patient->m_BirthMonth)
	{
		age--;
	}
	else if (CurTime.wMonth == m_Patient->m_BirthMonth)
	{
		if (CurTime.wDay < m_Patient->m_BirthDay)
		{
			age--;
		}
	}

	res.Format(_T("%i"), age);

	return res;
}

//***************************************************************************************

void CPlannerWnd::CreateMenu()
{
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
	//[cjf***05042012*005*]
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
	//[cjf***05042012*005*]
}

//***************************************************************************************

//[520] ,export the IOL DATA fOr The Surgeon program iTraceTPS
void CPlannerWnd::OnESDButtonClicked()
{
	if (m_pCTExam->m_ColorImage.m_RGBData.GetMem() == NULL)
	{
		::Info("Sorry, only the exam whose color eye image was captured can be exported.");
		return;
	}

	//Let the user select it is pre-op or post-op data
	CToricSelDlg* pDlg = new CToricSelDlg(this);
	if (pDlg->DoModal() == IDOK)
	{
		CToricSelDlg2* pDlg2 = new CToricSelDlg2(this);

		int ExportType = 1;

		if (pDlg2->DoModal() == IDOK)
		{
			ExportType = pDlg2->GetSelection();
		}
		delete pDlg2;

		CString SavedFileName, FileName1, FileName2, FolderName, basicFolderName;
		CString ExportYear, ExportMonth, ExportDay, ExportHour, ExportMinute;
		CString s;
		//
		GUIDToStr(m_PatientIOL.PatientID, s);

		SYSTEMTIME Time;
		::GetLocalTime(&Time);

		m_PatientIOL.ExportYear = Time.wYear;
		m_PatientIOL.ExportMonth = Time.wMonth;
		m_PatientIOL.ExportDay = Time.wDay;
		m_PatientIOL.ExportHour = Time.wHour;
		m_PatientIOL.ExportMinute = Time.wMinute;

		ExportYear.Format(_T("%i"), Time.wYear);
		ExportMonth.Format(_T("%02i"), Time.wMonth);
		ExportDay.Format(_T("%02i"), Time.wDay);
		ExportHour.Format(_T("%02i"), Time.wHour);
		ExportMinute.Format(_T("%02i"), Time.wMinute);

		SavedFileName = ExportMonth + "-" + ExportDay + "-" + ExportYear + " " + ExportHour + "-" + ExportMinute;

		FolderName = MAIN_DIR + "\\TPSData";
		if (!::PathFileExists(FolderName)) ::CreateDirectory(FolderName, NULL);
		FolderName = FolderName + "\\" + m_PatientIOL.LastName + "." + m_PatientIOL.FirstName + " - " + s;
		if (!::PathFileExists(FolderName)) ::CreateDirectory(FolderName, NULL);

		basicFolderName = FolderName;

		if (pDlg->m_pre)
		{
			FolderName = FolderName + "\\##";
			m_PatientIOL.pCTExam.m_Header.m_Preop = 1;
		}
		else
		{
			FolderName = FolderName + "\\__";
			m_PatientIOL.pCTExam.m_Header.m_Preop = 2;
		}
		if (!::PathFileExists(FolderName)) ::CreateDirectory(FolderName, NULL);

		FileName1 = FolderName + "\\" + SavedFileName + ".iol";
		FileName2 = FolderName + "\\" + SavedFileName + ".jpg";
		CString FileName3 = FolderName + "\\" + SavedFileName + "Gray.jpg";

		G_As = FileName2;
		strncpy(m_PatientIOL.FileName, G_As, 300);

		//Calculate the eye vertex, pupil, limbus of color eye image		
		if (!m_pCTExam->m_ColorImage.m_ve_ok)
		{
			m_pCTExam->m_ColorImage.FindWFVertex();
		}

		if (!m_pCTExam->m_ColorImage.m_pu_ok)
		{
			m_pCTExam->m_ColorImage.FindPupil();
		}

		if (!m_pCTExam->m_ColorImage.m_li_ok)
		{
			m_pCTExam->m_ColorImage.FindLimbus();
		}

		//Done
		m_PatientIOL.pCTExam = *m_pCTExam;

		//
		m_PatientIOL.pCTExam.m_ColorImage.m_LRa_ok = FALSE;
		m_PatientIOL.pCTExam.m_ColorImage.m_LRa_x0_um = -1;
		m_PatientIOL.pCTExam.m_ColorImage.m_LRa_y0_um = -1;
		m_PatientIOL.pCTExam.m_ColorImage.m_LRa_x1_um = -1;
		m_PatientIOL.pCTExam.m_ColorImage.m_LRa_y1_um = -1;

		m_PatientIOL.SaveIntoFile(FileName1);
		m_PatientIOL.pCTExam.m_ColorImage.SaveIntoFile(FileName2);
		m_PatientIOL.pCTExam.m_Image.SaveIntoFile(FileName3);

		//Save the zip file
		CString ExportFolderName; //1, export to pvsdata; 2. export to a zip file 3. export to pvsdata and zip file 
		CString ZipFileName;
		if (ExportType != 1)
		{
			ExportFolderName = MAIN_DIR + "\\Export PVS Data";

			if (!::PathFileExists(ExportFolderName)) ::CreateDirectory(ExportFolderName, NULL);

			CFolderDlg::m_InitialFolderName = ExportFolderName;

			CFolderDlg* pDlg = new CFolderDlg(this, "");
			if (pDlg->Show()) {
				ExportFolderName = pDlg->m_FolderName;
			}
			else {
				ExportFolderName.Empty();
			}
			delete pDlg;


			{
				CString s;
				::GUIDToStr(m_PatientIOL.PatientID, s);

				ZipFileName = "_PVS0_";
				ZipFileName += m_PatientIOL.LastName;
				ZipFileName += ".";
				ZipFileName += m_PatientIOL.FirstName;
				ZipFileName += " - ";
				ZipFileName += s;

				ZipFileName += "_PVS1_.zip";

				ZipFileName.Replace(_T("\\"), _T("_"));
				ZipFileName.Replace(_T("/"), _T("_"));
				ZipFileName.Replace(_T(":"), _T("_"));
				ZipFileName.Replace(_T("*"), _T("_"));
				ZipFileName.Replace(_T("?"), _T("_"));
				ZipFileName.Replace(_T("\""), _T("_"));
				ZipFileName.Replace(_T("<"), _T("_"));
				ZipFileName.Replace(_T(">"), _T("_"));
				ZipFileName.Replace(_T("|"), _T("_"));
				ZipFileName.Replace(_T(","), _T("_"));

				ZipFileName = ExportFolderName + "\\" + ZipFileName;

				HZIP hZip = ::CreateZip(ZipFileName, "");

				//	
				GetFileNamesFromFolder(basicFolderName);
				//
				for (int i = 0; i < 3; i++)
				{
					CString zipName = m_FileNamesInFolder[i];
					zipName.Replace(basicFolderName + _T("\\"), _T(""));
					zipName.Replace(_T("\\"), _T("_pvs_"));

					::ZipAdd(hZip, zipName, m_FileNamesInFolder[i]);
				}

				::CloseZipZ(hZip);
			}

			if (ExportType == 2)
			{
				DeleteDirectory((LPSTR)(LPCTSTR)basicFolderName);
			}
		}
		//Done

		if (ExportType != 2)	::Info("IOL Data file : \n<" + SavedFileName + ">\n saved successfully.");

		if (ExportType != 1)	::Info("IOL Data zip file : \n<" + ZipFileName + ">\n saved successfully.");
	}
	else
	{
	}
	delete pDlg;
}
//[520] 

//***************************************************************************************

void CPlannerWnd::OnShowColorEyeClicked()
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

void CPlannerWnd::OnShowGrayEyeClicked()
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

void CPlannerWnd::GetFileNamesFromFolder(CString FolderName)
{
	CFileFind finder;
	BOOL Working = finder.FindFile(FolderName + "\\*.*");
	int t = 0;

	while (Working)
	{
		Working = finder.FindNextFile();
		if (finder.IsDots())	   continue;
		else if (finder.IsDirectory())
		{
			CString DirName = FolderName + "\\" + finder.GetFileName();
			GetFileNamesFromFolder(DirName);
		}
		else
		{
			CString FileName = FolderName + "\\" + finder.GetFileName();
			m_FileNamesInFolder[t] = FileName;
			t++;
		}
	}
}

//*********************************************************************************************

BOOL CPlannerWnd::DeleteDirectory(char* sDirName)
{
	CFileFind tempFind;
	char sTempFileFind[200];

	sprintf(sTempFileFind, "%s\\*.*", sDirName);
	BOOL IsFinded = tempFind.FindFile((LPCTSTR)sTempFileFind);
	while (IsFinded)
	{
		IsFinded = tempFind.FindNextFile();

		if (!tempFind.IsDots())
		{
			char sFoundFileName[200];
			CStringA Afilename(tempFind.GetFileName().GetBuffer(200));
			strcpy(sFoundFileName, Afilename);

			if (tempFind.IsDirectory())
			{
				char sTempDir[200];
				sprintf(sTempDir, "%s\\%s", sDirName, sFoundFileName);
				DeleteDirectory(sTempDir);
			}
			else
			{
				char sTempFileName[200];
				sprintf(sTempFileName, "%s\\%s", sDirName, sFoundFileName);

				DeleteFile((LPCWSTR)sTempFileName);
			}
		}
	}
	tempFind.Close();
	if (!RemoveDirectory((LPCWSTR)sDirName))
	{
		return FALSE;
	}
	return TRUE;
}

//*********************************************************************************************

//7.0.0
void CPlannerWnd::OnMouseMove(uint nFlags, CPoint Point)
{
	this->GetParent()->SendMessage(WM_BTN_ISHOVER, 0, 0);
}
//7.0.0

//***************************************************************************************