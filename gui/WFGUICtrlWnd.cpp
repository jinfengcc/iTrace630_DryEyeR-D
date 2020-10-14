//WF capture controller window

//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "Settings.h"

#include "WFGUICtrlWnd.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CWFGUICtrlWnd, CMemWnd)

	ON_MESSAGE(WM_SELBTN_POS_CHANGED, OnOnOffChangedPos)

	ON_BN_CLICKED(IDC_SHOWSCAN_BTN, OnShowScanBtnClicked)

	ON_BN_CLICKED(IDC_CAPTURE_BTN, OnCaptureBtnClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CWFGUICtrlWnd::CWFGUICtrlWnd() :CMemWnd()
{
}

//***************************************************************************************

void CWFGUICtrlWnd::CreateThisWnd(RECT& Rect, CWnd* pWnd, int CapWidth)
{
	m_TargetOn = !::Settings.m_AccommTargetLightOff;
	m_AutoMode = 1;
	m_PupilCen = !::Settings.m_AlignmentMethod;
	::PresbiaMode = 1;//Presbia
	::PresbiaModeOrder = 1;//Presbia

	m_ShowScanRing = TRUE;

	::WFScanModeChanged = FALSE;

	m_Type = FALSE;

	m_WFORCT = 0;

	CreateWnd(Rect, pWnd);

	m_w = Rect.right - Rect.left - CapWidth;
	m_h = Rect.bottom - Rect.top;

	//Acq Btn
	{
		m_Acquire = FALSE;

		int Left_w = intRound(real(CapWidth) / 5);
		int BtnW = Left_w;
		int top_h = intRound(real(m_h) / 10);

		int W = ::GetSystemMetrics(SM_CXSCREEN);
		int q = W == 1920 ? 2 : (W == 1600 ? 1 : 0);

		RECT tempRect;
		::SetRect(&tempRect, m_w + Left_w, top_h, m_w + Left_w + BtnW, top_h + BtnW);
		m_CapBtn.Create(_T(""), _T(""), 0, tempRect, this, IDC_CAPTURE_BTN);

		m_CapBtn.SetImage(q == 2 ? IDR_ACQUISITION_3 : q == 1 ? IDR_ACQUISITION_2 : IDR_ACQUISITION_2);
		m_CapBtn.SetBk(FALSE);
		m_CapBtn.SetBkColor(BLACK);
	}
	//Acq Btn

	if (m_isPresbia)
	{
		int mid_w = intRound(real(m_w) / 2);

		int OnoffWidth = intRound(0.07    * m_w);
		int Half_OnoffHeight = intRound(0.5 * 0.45 * OnoffWidth);
		OnoffWidth = intRound(2.0 * OnoffWidth);//From 2.3 to 2.0

		int mid_h1 = 4 * Half_OnoffHeight + 40; //Presbia
		int mid_h2 = 2 * Half_OnoffHeight + 20;//Presbia

		RECT tempRect;
		m_FontSize = intRound(1.4 * Half_OnoffHeight);

		CMFont Font(m_FontSize, 400, "Arial");
		m_MemDC.MeasureRect("   Target Light:", tempRect, Font);
		int LableaW = tempRect.right - tempRect.left;

		m_MemDC.MeasureRect("   Mode:", tempRect, Font);
		int LablebW = tempRect.right - tempRect.left;

		m_MemDC.MeasureRect("    Centration:", tempRect, Font);
		int LablecW = tempRect.right - tempRect.left;

		int top = mid_h1 - Half_OnoffHeight;
		int btm = mid_h1 + Half_OnoffHeight;

		int BtnW = intRound(0.5 * real(OnoffWidth));

		int space = intRound(real(m_w - (LableaW + LablebW + LablecW + BtnW + 3 * OnoffWidth)) / 2.4);

		//Bk Rect (if it WF and CT exam togther, do not allow to change pupil/dot center,thus need change the components position)
		int Bk_Left = intRound(real(m_w) / 4);

		::SetRect(&m_Bk_LabelRect[0], Bk_Left - LableaW, top, Bk_Left, btm);
		::SetRect(&m_Bk_OnoffRect[0], Bk_Left, top, Bk_Left + OnoffWidth, btm);

		Bk_Left = mid_w; //+ intRound( real(m_w) / 5);
		::SetRect(&m_Bk_LabelRect[1], Bk_Left, top, Bk_Left + LablebW, btm);
		::SetRect(&m_Bk_OnoffRect[1], Bk_Left + LablebW, top, Bk_Left + LablebW + OnoffWidth, btm);

		::SetRect(&m_Bk_BtnRect, Bk_Left + LablebW + OnoffWidth, top, Bk_Left + LablebW + OnoffWidth + BtnW, btm);
		//Bk Rect Done

		//If it is CT Acquire, Set the RECT for CT Caputre ONOFF AND LABLE
		::SetRect(&m_CT_LabelRect, mid_w - LablebW - LablebW, top, mid_w - LablebW, btm);
		::SetRect(&m_CT_OnoffRect, mid_w - LablebW, top, mid_w - LablebW + OnoffWidth, btm);
		//Done

		int Left, Right;

		//Label "Fixation Light:"
		Left = 0;
		Right = Left + LableaW;
		::SetRect(&m_LabelRect[0], Left, top, Right, btm);
		//Label "Fixation Light:"

		//Onoff 0
		Left = Right;
		Right = Left + OnoffWidth;
		::SetRect(&m_OnoffRect[0], Left, top, Right, btm);
		m_OnOff[0].Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_OnoffRect[0], this, IDC_ONOFF0_BTN);
		m_OnOff[0].m_id = 0;
		m_OnOff[0].m_OnOff = m_TargetOn;
		m_OnOff[0].m_Labels[0] = "On";
		m_OnOff[0].m_Labels[1] = "Off";
		//Onoff 0

		//Label "Mode:" RECT
		Left = Right + space;
		Right = Left + LablebW;
		::SetRect(&m_LabelRect[1], Left, top, Right, btm);
		//Label "Mode:" RECT Done

		//Onoff 1 Auto and Manual
		Left = Right;
		Right = Left + intRound(OnoffWidth*1.5);
		::SetRect(&m_OnoffRect[1], Left, top, Right, btm);

		m_AutoManual.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_OnoffRect[1], this, IDC_ONOFF1_BTN);
		m_AutoManual.m_id = 1;
		m_AutoManual.m_OnOff = 1;
		m_AutoManual.m_Labels[0] = "Auto";
		m_AutoManual.m_Labels[1] = "Manual Full Scan";
		m_AutoManual.m_Labels[2] = "Manual 64 Scan";
		m_AutoManual.m_SmallFont[0] = FALSE;
		m_AutoManual.m_SmallFont[1] = TRUE;
		m_AutoManual.m_SmallFont[2] = TRUE;
		m_AutoManual.SetTip("Auto", "Manual Full Scan", "Manual 64 Scan");
		//Onoff 1

		//Btn	
		Left = Right;
		Right = Left + BtnW;
		::SetRect(&tempRect, Left, top, Right, btm);
		m_ShowScanBtn.Create(_T(""), _T(""), 0, tempRect, this, IDC_SHOWSCAN_BTN);
		m_ShowScanBtn.SetText("Show Scan Pattern");//601
												   //Btn

												   //Label "Centration:"
		Left = Right + space;
		Right = Left + LablecW;
		::SetRect(&m_LabelRect[2], Left, top, Right, btm);
		//Label "Centration:"

		//Onoff 2
		Left = Right;
		Right = Left + OnoffWidth;
		::SetRect(&m_OnoffRect[2], Left, top, Right, btm);

		//Change from m_OnOff[2] to m_OnOff[1]
		m_OnOff[1].Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_OnoffRect[2], this, IDC_ONOFF2_BTN);
		m_OnOff[1].m_id = 2;
		m_OnOff[1].m_OnOff = m_PupilCen;
		m_OnOff[1].m_Labels[0] = "Pupil";
		m_OnOff[1].m_Labels[1] = "IR Dot";
		//Onoff 2

		//Presbia OnOff
		int Half_OnoffWidth = intRound(0.5 * OnoffWidth);
		top = mid_h2 - Half_OnoffHeight;
		btm = mid_h2 + Half_OnoffHeight;
		Left = mid_w - Half_OnoffWidth;
		Right = mid_w + 2 * Half_OnoffWidth;

		m_MemDC.MeasureRect("   Presbia Mode:", tempRect, Font);
		int LablePresbiaW = tempRect.right - tempRect.left;

		::SetRect(&m_Presbia_Onoff_LabelRect, Left - LablePresbiaW, top, Left, btm);

		::SetRect(&m_Presbia_OnoffRect, Left, top, Right, btm);
		m_PresbiaOnOff.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_Presbia_OnoffRect, this, IDC_PRESBIA_ONOFF_BTN);
		m_PresbiaOnOff.m_id = 3;
		m_PresbiaOnOff.m_OnOff = 1;
		m_PresbiaOnOff.m_Labels[0] = "Normal";
		m_PresbiaOnOff.m_Labels[1] = "Presbia64";
		m_PresbiaOnOff.m_Labels[2] = "Presbia256";
		//Done		
	}
	else
	{
		int mid_w = intRound(real(m_w) / 2);
		int mid_h = intRound(real(m_h) / 3);

		int OnoffWidth = intRound(0.07    * m_w);
		int Half_OnoffHeight = intRound(0.5 * 0.45 * OnoffWidth);
		OnoffWidth = intRound(2.0 * OnoffWidth);// From 2.3 to 2.0

		RECT tempRect;
		m_FontSize = intRound(1.4 * Half_OnoffHeight);

		CMFont Font(m_FontSize, 400, "Arial");
		m_MemDC.MeasureRect("   Target Light:", tempRect, Font);
		int LableaW = tempRect.right - tempRect.left;

		m_MemDC.MeasureRect("   Mode:", tempRect, Font);
		int LablebW = tempRect.right - tempRect.left;

		m_MemDC.MeasureRect("    Centration:", tempRect, Font);
		int LablecW = tempRect.right - tempRect.left;

		int top = mid_h - Half_OnoffHeight;
		int btm = mid_h + Half_OnoffHeight;

		int BtnW = intRound(0.5 * real(OnoffWidth));

		int space = intRound(real(m_w - (LableaW + LablebW + LablecW + BtnW + 3 * OnoffWidth)) / 2.4); // From 2.0 to 2.4

																									   //Bk Rect (if it WF and CT exam togther, do not allow to change pupil/dot center,thus need change the components position)
		int Bk_Left = intRound(real(m_w) / 4);

		::SetRect(&m_Bk_LabelRect[0], Bk_Left - LableaW, top, Bk_Left, btm);
		::SetRect(&m_Bk_OnoffRect[0], Bk_Left, top, Bk_Left + OnoffWidth, btm);

		Bk_Left = mid_w; //+ intRound( real(m_w) / 5);
		::SetRect(&m_Bk_LabelRect[1], Bk_Left, top, Bk_Left + LablebW, btm);
		::SetRect(&m_Bk_OnoffRect[1], Bk_Left + LablebW, top, Bk_Left + LablebW + OnoffWidth, btm);

		::SetRect(&m_Bk_BtnRect, Bk_Left + LablebW + OnoffWidth, top, Bk_Left + LablebW + OnoffWidth + BtnW, btm);
		//Bk Rect Done

		//If it is CT Acquire, Set the RECT for CT Caputre ONOFF AND LABLE		
		::SetRect(&m_CT_LabelRect, mid_w - LablebW - LablebW, top, mid_w - LablebW, btm);
		::SetRect(&m_CT_OnoffRect, mid_w - LablebW, top, mid_w - LablebW + OnoffWidth, btm);
		//Done

		int Left, Right;

		//Label "Fixation Light:"
		Left = 0;
		Right = Left + LableaW;
		::SetRect(&m_LabelRect[0], Left, top, Right, btm);
		//Label "Fixation Light:"

		//Onoff 0
		Left = Right;
		Right = Left + OnoffWidth;
		::SetRect(&m_OnoffRect[0], Left, top, Right, btm);
		m_OnOff[0].Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_OnoffRect[0], this, IDC_ONOFF0_BTN);
		m_OnOff[0].m_id = 0;
		m_OnOff[0].m_OnOff = m_TargetOn;
		m_OnOff[0].m_Labels[0] = "On";
		m_OnOff[0].m_Labels[1] = "Off";
		//Onoff 0

		//Label "Mode:"
		Left = Right + space;
		Right = Left + LablebW;
		::SetRect(&m_LabelRect[1], Left, top, Right, btm);
		//Label "Mode:"

		//Onoff 1
		Left = Right;
		Right = Left + intRound(OnoffWidth*1.5);
		::SetRect(&m_OnoffRect[1], Left, top, Right, btm);

		m_AutoManual.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_OnoffRect[1], this, IDC_ONOFF1_BTN);
		m_AutoManual.m_id = 1;
		m_AutoManual.m_OnOff = 1;
		m_AutoManual.m_Labels[0] = "Auto";
		m_AutoManual.m_Labels[1] = "Manual Full Scan";
		m_AutoManual.m_Labels[2] = "Manual 64 Scan";
		m_AutoManual.m_SmallFont[0] = FALSE;
		m_AutoManual.m_SmallFont[1] = TRUE;
		m_AutoManual.m_SmallFont[2] = TRUE;
		m_AutoManual.SetTip("Auto", "Manual Full Scan", "Manual 64 Scan");
		//Onoff 1

		//Btn	
		Left = Right;
		Right = Left + BtnW;
		::SetRect(&tempRect, Left, top, Right, btm);
		m_ShowScanBtn.Create(_T(""), _T(""), 0, tempRect, this, IDC_SHOWSCAN_BTN);
		m_ShowScanBtn.SetText("Show Scan Pattern");//601
												   //Btn

												   //Label "Centration:"
		Left = Right + space;
		Right = Left + LablecW;
		::SetRect(&m_LabelRect[2], Left, top, Right, btm);
		//Label "Centration:"

		//Onoff 2
		Left = Right;
		Right = Left + OnoffWidth;
		::SetRect(&m_OnoffRect[2], Left, top, Right, btm);

		// Change from m_OnOff[2] to m_OnOff[1]
		m_OnOff[1].Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_OnoffRect[2], this, IDC_ONOFF2_BTN);
		m_OnOff[1].m_id = 2;
		m_OnOff[1].m_OnOff = m_PupilCen;
		m_OnOff[1].m_Labels[0] = "Pupil";
		m_OnOff[1].m_Labels[1] = "IR Dot";
		//Onoff 2
		//Done		
	}
}

//***************************************************************************************

CWFGUICtrlWnd::~CWFGUICtrlWnd()
{
}

//***************************************************************************************

void CWFGUICtrlWnd::ShowGUI()
{
	m_ShowWindow = TRUE;
	m_AutoManual.ShowWindow(SW_SHOW);

	if (!m_WFORCT)
	{
		m_OnOff[0].ShowWindow(SW_SHOW);
		m_OnOff[1].ShowWindow(SW_SHOW);
		if (m_isPresbia) m_PresbiaOnOff.ShowWindow(SW_SHOW);//Presbia	

		m_ShowScanBtn.ShowWindow(SW_SHOW);
		Label();
	}
	else
	{
		if (m_isPresbia) m_PresbiaOnOff.ShowWindow(SW_HIDE);//Presbia
		m_AutoManual.MoveWindow(&m_CT_OnoffRect, 1);//Change from m_OnOff[1]  to m_AutoManual
		CTLabel();
	}
}

//***************************************************************************************

void CWFGUICtrlWnd::HideGUI()
{
	m_ShowWindow = FALSE;

	m_OnOff[0].ShowWindow(SW_HIDE);
	m_OnOff[1].ShowWindow(SW_HIDE);
	m_AutoManual.ShowWindow(SW_HIDE);	//Change from m_OnOff[2]  to m_AutoManual
	if (m_isPresbia) m_PresbiaOnOff.ShowWindow(SW_HIDE);//Presbia	

	m_ShowScanBtn.ShowWindow(SW_HIDE);

	Label();
}

//***************************************************************************************

void  CWFGUICtrlWnd::HideAliMethod()
{
	m_OnOff[1].ShowWindow(SW_HIDE);	//Change from m_OnOff[2]  to m_OnOff[1]

	m_OnOff[0].MoveWindow(&m_Bk_OnoffRect[0], 1);
	m_AutoManual.MoveWindow(&m_Bk_OnoffRect[1], 1);	//Change from m_OnOff[1]  to m_AutoManual
	m_ShowScanBtn.MoveWindow(&m_Bk_BtnRect, 1);

	m_Type = 1;

	Label();
}

//***************************************************************************************

LRESULT CWFGUICtrlWnd::OnOnOffChangedPos(WPARAM wParam, LPARAM lParam)
{
	if (lParam == 0)
	{
		m_TargetOn = m_OnOff[0].m_OnOff;
	}
	else if (lParam == 1)
	{
		m_AutoMode = m_AutoManual.m_OnOff;

		if (m_AutoMode == 3) ::Normal64Mode = TRUE;
		else ::Normal64Mode = FALSE;

		::WFScanModeChanged = TRUE;
	}
	else if (lParam == 2)
	{
		m_PupilCen = m_OnOff[1].m_OnOff; //Change from m_OnOff[2]  to m_OnOff[1]
	}
	//Presbia
	else if (lParam == 3)
	{
		::PresbiaModeOrder = ::PresbiaMode;
		::PresbiaMode = m_PresbiaOnOff.m_OnOff;

		if (::PresbiaMode == 2 || ::PresbiaMode == 3)
		{
			m_AutoMode = m_OnOff[1].m_OnOff = 2;
			m_OnOff[1].ShowWindow(SW_HIDE);
			m_ShowScanBtn.ShowWindow(SW_HIDE);

			m_AutoManual.ShowWindow(SW_HIDE); //Change from m_OnOff[2]  to m_AutoManual
		}
		else
		{
			m_AutoMode = m_AutoManual.m_OnOff = 1;
			m_AutoManual.ShowWindow(SW_SHOW);
			m_ShowScanBtn.ShowWindow(SW_SHOW);

			if (!m_Type) m_OnOff[1].ShowWindow(SW_SHOW);
		}
	}
	//Presbia

	if (!m_WFORCT)   Label();
	else           	CTLabel();

	return 0;
}

//***************************************************************************************

void CWFGUICtrlWnd::CTLabel()
{
	m_MemDC.FillSolidRect(&m_Presbia_Onoff_LabelRect, BLACK);//Presbia
	InvalidateRect(&m_Presbia_Onoff_LabelRect, FALSE);//Presbia

	if (m_ShowWindow)
	{
		CMFont Font(m_FontSize, 400, "Arial");

		m_MemDC.WriteText("Mode: ", m_CT_LabelRect, Font, WHITE, 2, BLACK);

		for (int i = 0; i < 3; i++)
			InvalidateRect(&m_LabelRect[i], FALSE);
	}
	else
	{
		m_MemDC.FillSolidRect(0, 0, m_w, m_h, BLACK);
		for (int i = 0; i < 3; i++)
		{
			InvalidateRect(&m_LabelRect[i], FALSE);
		}
	}
}

//***************************************************************************************

void CWFGUICtrlWnd::Label()
{
	if (m_ShowWindow)
	{
		CMFont Font(m_FontSize, 400, "Arial");

		if (m_isPresbia) m_MemDC.WriteText("Presbia Mode: ", m_Presbia_Onoff_LabelRect, Font, WHITE, 2, BLACK);//Presbia		

		InvalidateRect(&m_Presbia_Onoff_LabelRect, FALSE);

		if (m_Type == FALSE)
		{
			m_MemDC.WriteText("Fixation Light: ", m_LabelRect[0], Font, WHITE, 2, BLACK);
			if (::PresbiaMode == 1)//Presbia
			{
				m_MemDC.WriteText("Mode: ", m_LabelRect[1], Font, WHITE, 2, BLACK);
				m_MemDC.WriteText("Centration: ", m_LabelRect[2], Font, WHITE, 2, BLACK);
			}
			//Presbia
			else
			{
				m_MemDC.FillSolidRect(&m_LabelRect[1], BLACK);
				m_MemDC.FillSolidRect(&m_LabelRect[2], BLACK);
			}
			//Presbia				

			for (int i = 0; i < 3; i++)
				InvalidateRect(&m_LabelRect[i], FALSE);
		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
				m_MemDC.FillSolidRect(&m_LabelRect[i], BLACK);
				InvalidateRect(&m_LabelRect[i], FALSE);
			}

			m_MemDC.WriteText("Fixation Light: ", m_Bk_LabelRect[0], Font, WHITE, 2, BLACK);

			if (::PresbiaMode == 1)//Presbia
				m_MemDC.WriteText("Mode: ", m_Bk_LabelRect[1], Font, WHITE, 2, BLACK);

			//Presbia
			else   m_MemDC.FillSolidRect(&m_Bk_LabelRect[1], BLACK);
			//Presbia


			for (int i = 0; i < 2; i++)
			{
				InvalidateRect(&m_Bk_LabelRect[i], FALSE);
			}
		}

		if (m_AutoMode == 1)
		{
			m_ShowScanBtn.SetEnabled(FALSE);
		}
		else
		{
			m_ShowScanBtn.SetEnabled(TRUE);
		}

		m_ShowScanBtn.Repaint();
	}
	else
	{
		m_MemDC.FillSolidRect(0, 0, m_w, m_h, BLACK);

		for (int i = 0; i < 3; i++)
		{
			InvalidateRect(&m_LabelRect[i], FALSE);
		}
		InvalidateRect(&m_CT_LabelRect, FALSE);
	}
}

//***************************************************************************************

void  CWFGUICtrlWnd::RepaintAuto()
{
	m_OnOff[1].m_OnOff = m_AutoMode;
	m_OnOff[1].Repaint();

	if (!m_WFORCT)  Label();
	else           CTLabel();
}

//***************************************************************************************

void  CWFGUICtrlWnd::RepaintPupilCen()
{
	m_OnOff[1].m_OnOff = m_PupilCen;
	m_OnOff[1].Repaint();

	Label();
}

//***************************************************************************************

void  CWFGUICtrlWnd::RepaintTarget()
{
	m_OnOff[0].m_OnOff = m_TargetOn;
	m_OnOff[0].Repaint();
	Label();
}

//***************************************************************************************

void CWFGUICtrlWnd::OnShowScanBtnClicked()
{
	m_ShowScanRing = !m_ShowScanRing;

	if (m_ShowScanRing) m_ShowScanBtn.SetText("Show Scan Pattern");
	else               m_ShowScanBtn.SetText("Show Scan Ring");

	m_ShowScanBtn.Repaint();
}

//***************************************************************************************
void CWFGUICtrlWnd::ShowAcqBtnGUI()
{
	m_ShowAcqBtn = TRUE;

	m_CapBtn.ShowWindow(SW_SHOW);
}

//***************************************************************************************

void CWFGUICtrlWnd::HideAcqBtnGUI()
{
	m_ShowAcqBtn = FALSE;

	m_CapBtn.ShowWindow(SW_HIDE);
}

//***************************************************************************************
void CWFGUICtrlWnd::OnCaptureBtnClicked()
{
	m_Acquire = TRUE;
}

//***************************************************************************************

void CWFGUICtrlWnd::SetScanRing(BOOL ScanRing)
{
	if (m_ShowScanRing)
	{
		OnShowScanBtnClicked();
	}
}

//***************************************************************************************
