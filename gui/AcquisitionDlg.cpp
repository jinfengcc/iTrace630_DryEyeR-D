//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "Settings.h"

#include "AcquisitionDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CAcquisitionDlg, CDialog)

	ON_COMMAND(IDK_CTRL_D, OnCtrlD)
	ON_COMMAND(IDK_ESCAPE, OnEscape)
	ON_COMMAND(IDK_SPACE, OnSpace)
	ON_COMMAND(IDK_ENTER, OnEnter)
	ON_COMMAND(IDK_SHIFT_ENTER, OnShiftEnter)
	ON_COMMAND(IDK_CTRL_ENTER, OnCtrlEnter)
	ON_MESSAGE(WM_THREAD_UPDATE, OnUpdate)
	ON_MESSAGE(WM_THREAD_FINISH, OnFinish)

	//ON_COMMAND(IDK_CTRL_S      , OnCtrlS     )//520//cancel by 530

END_MESSAGE_MAP()

//***************************************************************************************

CAcquisitionDlg::CAcquisitionDlg(CWnd* pParentWnd, CAcquisition* pAcquisition) :
	CDialog(IDD_VIDEO_DLG, pParentWnd)
{
	m_pAcquisition = pAcquisition;
	m_hAccel = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR));

	m_LaserSafe = TRUE;//6.2.0 For laser safty check
}

//***************************************************************************************

CAcquisitionDlg::~CAcquisitionDlg()
{
	DestroyAcceleratorTable(m_hAccel);
}
//***************************************************************************************

BOOL CAcquisitionDlg::PreTranslateMessage(MSG* pMsg)
{
	if (TranslateAccelerator(m_hWnd, m_hAccel, pMsg)) return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

//***************************************************************************************

BOOL CAcquisitionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ShowTechInfo = FALSE;

	m_w = CHW::m_VideoWidth;
	m_h = CHW::m_VideoHeight;
	m_w_um = m_pAcquisition->m_pHW->m_Calibration.VideoWidthMicrons;
	m_h_um = m_pAcquisition->m_pHW->m_Calibration.VideoHeightMicrons;
	m_x_px_um = m_w / m_w_um;
	m_y_px_um = m_h / m_h_um;
	m_cx = 0.5 * m_w;
	m_cy = 0.5 * m_h - 1;

	//530  
	{
		int W = ::GetSystemMetrics(SM_CXSCREEN);
		int H = ::GetSystemMetrics(SM_CYSCREEN);

		int left = intRound(real(W) / 2 - real(m_w) / 2);
		int top = intRound(real(H) / 2 - real(m_h) / 2);
		int hight = H - top;

		//Set the video wnd
		SetWindowPos(NULL, 0, top, W, hight, 0);
		m_pAcquisition->m_pVideoWnd = this;
		//Done

		RECT Rect = { left, 0, left + m_w, m_h };
		m_VideoWnd.CreateWnd(Rect, this);

		::SetRect(&Rect, left + m_w, 0, W, m_h);

		int ALeft = left - intRound(real(m_w) / 5);
		if (ALeft < 0)  ALeft = 0;

		int ARight = left + intRound(real(m_w) / 5) + m_w;
		if (ARight > W) ARight = W;

		if (m_CT)
		{
			if (::Settings.m_Cap_ColorImg && !::Settings.m_Cap_ColorAutoOrManu)
			{
				::SetRect(&Rect, left + m_w, 0, W, H);

				m_top = top;

				int CapHeight = H - m_h;

				//Manual color capture GUI 
				m_ControlWnd.CreateThisWnd(Rect, this, m_top, CapHeight);
				m_ControlWnd.m_ShowWindow = FALSE;
				m_ControlWnd.HideGUI();

				m_ControlWnd.m_ShowAcqBtn = FALSE;
				m_ControlWnd.HideAcqBtnGUI();
				//Done
			}

			::SetRect(&Rect, 0, 0, W, H);
			m_unUseWnd.CreateWnd(Rect, this);
		}
		else
		{
			::Normal64Mode = FALSE;// WF 64 Scan

								   //Slider of wf scan size
			m_WFGUICtrl0Wnd.CreateThisWnd(Rect, this);
			m_WFGUICtrl0Wnd.m_ShowWindow = FALSE;
			m_WFGUICtrl0Wnd.HideGUI();
			//Done	

			//Based panel
			int CapWidth = W - ARight;
			::SetRect(&Rect, ALeft, m_h, W, hight);
			m_WFGUICtrlWnd.CreateThisWnd(Rect, this, CapWidth);
			m_WFGUICtrlWnd.m_ShowWindow = FALSE;
			m_WFGUICtrlWnd.HideGUI();

			m_WFGUICtrlWnd.m_ShowAcqBtn = FALSE;
			m_WFGUICtrlWnd.HideAcqBtnGUI();
			//Done

			::SetRect(&Rect, 0, 0, left, H);
			m_unUseWnd.CreateWnd(Rect, this);
		}
	}
	//530


	/* else if( m_NormalUsed == 0)
	{
	RECT Rect = {0, 0, m_w, m_h};
	m_VideoWnd.CreateWnd(Rect, this);

	m_pAcquisition->m_pVideoWnd = this;

	SetWindowPos(NULL, 0, 0, m_w, m_h, 0);
	CenterWindow();
	}*/

	return TRUE;
}

//***************************************************************************************

void CAcquisitionDlg::OnEscape()
{
	m_pAcquisition->Abort();
}

//***************************************************************************************

void CAcquisitionDlg::OnSpace()
{
	m_pAcquisition->Acquire();
}

//***************************************************************************************

LRESULT CAcquisitionDlg::OnFinish(WPARAM wParam, LPARAM lParam)
{
	m_pAcquisition->Finish();

	//6.2.0 For laser safty
	if (lParam == 1) m_LaserSafe = FALSE;
	//6.2.0 For laser safty

	if (wParam == 1) CDialog::OnOK();
	else CDialog::OnCancel();

	return 0;
}

//***************************************************************************************

void CAcquisitionDlg::OnEnter()
{
	m_pAcquisition->ChangeMode();
}

void CAcquisitionDlg::OnShiftEnter()
{
	m_pAcquisition->ChangeMode();
}

void CAcquisitionDlg::OnCtrlEnter()
{
	m_pAcquisition->ChangeMode();
}

//***************************************************************************************

void CAcquisitionDlg::OnCtrlD()
{
	m_ShowTechInfo = !m_ShowTechInfo;
}

//***************************************************************************************

//***************************************************************************************

////520
////while the user capturing the color image, let user can adjust the camera setting
//void CAcquisitionDlg::OnCtrlS()
//{
//	if(::NewSettings.m_Adjusting_CT)
//	{
//		m_pAcquisition->SetColorCamera();
//	}
//}
////Done


//***************************************************************************************
