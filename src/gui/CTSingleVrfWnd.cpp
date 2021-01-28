//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "CTSingleVrfWnd.h"
#include "EyeWnd.h"
#include "BusyCursor.h"
#include "ImageDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CCTSingleVrfWnd, CSumWnd)

	ON_COMMAND(IDC_EDIT_LIMBUS_ITEM, OnEditLimbusItemClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CCTSingleVrfWnd::CCTSingleVrfWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CCTExam* pCTExam, CWndSettings* pWndSettings, int show) :
	CSumWnd(pWnd, WndRect, pPatient, pWndSettings)
{
	CBusyCursor Cursor;

	//----------------------------------------------------
	RECT Rect;
	GetWindowRect(&Rect);
	real_t t = 3.0 * (0.03 * m_h) + m_g;
	real_t w = ((Rect.right - Rect.left) - 4.0 * m_g) / 3.0;
	real_t h = ((Rect.bottom - Rect.top - t) - 2.0 * m_g) / 2.0;
	::SetRect(&m_Rect[0], intRound(m_g + w + m_g + w + m_g), intRound(t), intRound(m_g + w + m_g + w + m_g + w), intRound(t + h + m_g + h)); // info
	::SetRect(&m_LargeRect, intRound(m_g), intRound(t), intRound(m_g + w + m_g + w), intRound(t + h + m_g + h)); // large map
																												 //----------------------------------------------------

	m_pCTExam = pCTExam;

	CreateChildWnd();
	Repaint();

	ShowWindow(show);
}

//***************************************************************************************

void CCTSingleVrfWnd::RepaintMemDC()
{
	CSumWnd::RepaintMemDC();

	BOOL Rows[44] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

	CTExamLargeInfo(m_pCTExam, m_Rect[0], 44, Rows);
}

//***************************************************************************************

void CCTSingleVrfWnd::CreateChildWnd()
{
	SAFE_DELETE(m_pDispWnd[0]);

	CWndSettings* pWndSettings = GetWndSettings();

	pWndSettings->m_Type = TYPE_CEYE;
	pWndSettings->m_SizeSmall = FALSE;
	pWndSettings->m_Method3D = FALSE;
	pWndSettings->m_MapShowEye = TRUE;

	CEyeWnd* pEyeWnd = new CEyeWnd(m_LargeRect, this);
	pEyeWnd->m_Type = pWndSettings->m_Type;
	pEyeWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
	pEyeWnd->m_Method3D = pWndSettings->m_Method3D;
	pEyeWnd->m_MapShowEye = pWndSettings->m_MapShowEye;
	pEyeWnd->m_MapShowRings = pWndSettings->m_MapShowRings;
	pEyeWnd->m_MapShowPupil = pWndSettings->m_MapShowPupil;
	pEyeWnd->m_MapShowLimbus = pWndSettings->m_MapShowLimbus;
	pEyeWnd->m_pCTExam = m_pCTExam;
	m_pDispWnd[0] = pEyeWnd;
}

//***************************************************************************************

void CCTSingleVrfWnd::CreateMenu()
{
	CWndSettings* pWndSettings = GetWndSettings();

	m_pMenu->AppendItem(IDC_SHOW_MAP_RINGS_ITEM, FALSE, pWndSettings->m_MapShowRings, "Show rings", "");
	m_pMenu->AppendItem(IDC_SHOW_MAP_PUPIL_ITEM, FALSE, pWndSettings->m_MapShowPupil, "Show pupil", "");
	m_pMenu->AppendItem(IDC_SHOW_MAP_LIMBUS_ITEM, FALSE, pWndSettings->m_MapShowLimbus, "Show limbus", "");
	m_pMenu->AppendSeparator();
	m_pMenu->AppendItem(IDC_EDIT_LIMBUS_ITEM, FALSE, FALSE, "Edit pupil, limbus and rings", "");
}

//***************************************************************************************

void CCTSingleVrfWnd::OnEditLimbusItemClicked()
{
	CEyeImage* pImage = &m_pCTExam->m_Image;

	if (pImage->m_RGBData.GetMem() == NULL) return;

	CImageDlg* pDlg = new CImageDlg(this, pImage, FALSE, FALSE);//3

	if (pDlg->DoModal() == IDOK) {

		CBusyCursor Cursor;

		m_pCTExam->Process();

		Repaint();

		if (m_pCTExam->m_Header.m_Saved) ::DB.SaveCTExam(m_pCTExam);
	}

	delete pDlg;
}

//***************************************************************************************
