//530 wf capture controller window

//***************************************************************************************

//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "ExamCaptureWnd.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CExamCaptureWnd, CMemWnd)

	ON_BN_CLICKED(IDC_CAPTURE_BTN, OnCaptureBtnClicked)

END_MESSAGE_MAP()

//***************************************************************************************
CExamCaptureWnd::CExamCaptureWnd() :CMemWnd()
{
}
//***************************************************************************************

void CExamCaptureWnd::CreateThisWnd(RECT& Rect, CWnd* pWnd)
{
	CreateWnd(Rect, pWnd);

	m_w = Rect.right - Rect.left;
	m_h = Rect.bottom - Rect.top;

	m_Acquire = FALSE;

	int Left_w = intRound(real_t(m_w) / 5);
	int BtnW = Left_w;
	int top_h = intRound(real_t(m_h) / 10);

	//530 New Remmber add into Presbia
	int W = ::GetSystemMetrics(SM_CXSCREEN);
	int q = W >= 1920 ? 2 : (W >= 1600 ? 1 : 0);

	RECT tempRect;
	::SetRect(&tempRect, Left_w, top_h, Left_w + BtnW, top_h + BtnW);
	m_CapBtn.Create(_T(""), _T(""), 0, tempRect, this, IDC_CAPTURE_BTN);

	m_CapBtn.SetImage(q == 2 ? IDR_ACQUISITION_3 : q == 1 ? IDR_ACQUISITION_2 : IDR_ACQUISITION_2);
	m_CapBtn.SetBk(FALSE);
	m_CapBtn.SetBkColor(BLACK);
	//530 New Remmber add into Presbia
}

//***************************************************************************************

CExamCaptureWnd::~CExamCaptureWnd()
{
}

//***************************************************************************************
void CExamCaptureWnd::ShowGUI()
{
	m_ShowWindow = TRUE;

	m_CapBtn.ShowWindow(SW_SHOW);
}

//***************************************************************************************

void CExamCaptureWnd::HideGUI()
{
	m_ShowWindow = FALSE;

	m_CapBtn.ShowWindow(SW_HIDE);
}

//***************************************************************************************

void CExamCaptureWnd::RepaintMemDC()
{
	// ClearMemDC();
}
//***************************************************************************************

//***************************************************************************************
void CExamCaptureWnd::OnCaptureBtnClicked()
{
	m_Acquire = TRUE;
}