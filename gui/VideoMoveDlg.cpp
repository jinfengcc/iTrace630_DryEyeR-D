//[5.1.1]
//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "VideoMoveDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CVideoMoveDlg, CDialog)

	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_VIDEO_START_BTN, OnStartVideoButtonClicked)
	ON_BN_CLICKED(IDC_VIDEO_ABORT_BTN, OnAbortVideoButtonClicked)
	ON_BN_CLICKED(IDC_VIDEO_LEFT_BTN, OnVideoLeftButtonClicked)
	ON_BN_CLICKED(IDC_VIDEO_RIGHT_BTN, OnVideoRightButtonClicked)
	ON_BN_CLICKED(IDC_VIDEO_UP_BTN, OnVideoUpButtonClicked)
	ON_BN_CLICKED(IDC_VIDEO_DOWN_BTN, OnVideoDownButtonClicked)
	ON_MESSAGE(WM_THREAD_UPDATE, OnUpdate)
	ON_MESSAGE(WM_THREAD_FINISH, OnFinish)

END_MESSAGE_MAP()

//***************************************************************************************

CVideoMoveDlg::CVideoMoveDlg(CHW* pHW, CWnd* pParentWnd) :
	CDialog(IDD_VIDEO_MOVE_DLG, pParentWnd)
{
	m_pHW = pHW;
	m_pHW->m_pCurrentVideoSettings = &m_pHW->m_Calibration.WFVideoSettings;
	m_pLiveVideo = new CLiveVideo(m_pHW);
	m_pLiveVideo->m_pVideoWnd = this;
}

//***************************************************************************************

CVideoMoveDlg::~CVideoMoveDlg()
{
	delete m_pLiveVideo;
}

//***************************************************************************************

void CVideoMoveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDOK, m_OKButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_VIDEO_START_BTN, m_StartVideoButton);
	DDX_Control(pDX, IDC_VIDEO_ABORT_BTN, m_AbortVideoButton);
	DDX_Control(pDX, IDC_LEFT_EDIT, m_LeftEdit);
	DDX_Control(pDX, IDC_RIGHT_EDIT, m_RightEdit);
	DDX_Control(pDX, IDC_UP_EDIT, m_UpEdit);
	DDX_Control(pDX, IDC_DOWN_EDIT, m_DownEdit);
	//DDX_Control(pDX, IDC_VIDEO_LEFT_BTN                            , m_LeftButton                  );

}

//***************************************************************************************

BOOL CVideoMoveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_w = CHW::m_VideoWidth;
	m_h = CHW::m_VideoHeight;
	m_w_um = m_pHW->m_Calibration.VideoWidthMicrons;
	m_h_um = m_pHW->m_Calibration.VideoHeightMicrons;
	m_x_px_um = m_w / m_w_um;
	m_y_px_um = m_h / m_h_um;
	//m_cx                           = 0.5 * m_w;
	//m_cy                           = 0.5 * m_h - 1;


	RECT Rect = { 10, 10, 10 + CHW::m_VideoWidth, 10 + CHW::m_VideoHeight };
	m_VideoWnd.CreateWnd(Rect, this);

	//
	m_StepLeft = m_pHW->m_Calibration.StepLeft - 8;
	m_StepRight = m_pHW->m_Calibration.StepRight - 8;
	m_StepUp = m_pHW->m_Calibration.StepUp - 5;
	m_StepDown = m_pHW->m_Calibration.StepDown - 5;

	if ((m_StepLeft == -8) && (m_StepRight == -8) && (m_StepUp == -5) && (m_StepDown == -5))
	{
		m_StepLeft = 0;
		m_StepRight = 0;
		m_StepUp = 0;
		m_StepDown = 0;
	}

	///Left 
	CString s;
	if (m_StepLeft < -8 || m_StepLeft > 8) m_StepLeft = 0;

	real EditC = m_StepLeft / m_x_px_um;
	s.Format(_T("%.2f"), EditC);
	m_LeftEdit.SetWindowText(s);
	//

	//Right  
	if (m_StepRight < -8 || m_StepRight > 8) m_StepRight = 0;

	EditC = m_StepRight / m_x_px_um;
	s.Format(_T("%.2f"), EditC);
	m_RightEdit.SetWindowText(s);
	//

	//Up  
	if (m_StepUp < -5 || m_StepUp > 5) m_StepUp = 0;

	EditC = m_StepUp / m_y_px_um;
	s.Format(_T("%.2f"), EditC);
	m_UpEdit.SetWindowText(s);
	//

	//Done  
	if (m_StepDown < -5 || m_StepDown > 5) m_StepDown = 0;

	EditC = m_StepDown / m_y_px_um;
	s.Format(_T("%.2f"), EditC);
	m_DownEdit.SetWindowText(s);
	//


	this->GetWindowRect(&Rect);
	int W = Rect.right - Rect.left;
	int H = Rect.bottom - Rect.top;
	::SetRect(&Rect, intRound(0.825*W), intRound(0.39*H), intRound(0.875*W), intRound(0.46*H));
	m_LeftButton.Create(_T(""), _T(""), 0, Rect, this, IDC_VIDEO_LEFT_BTN);
	m_LeftButton.SetImage(W == 1920 ? IDR_LEFT_ARROW_3 : W == 1600 ? IDR_LEFT_ARROW_2 : IDR_LEFT_ARROW_1);
	m_LeftButton.SetBk(FALSE);
	m_LeftButton.SetEnabled(FALSE);
	m_LeftButton.SetTip("Video range move to left");
	m_LeftButton.ShowWindow(SW_SHOW);

	::SetRect(&Rect, intRound(0.905*W), intRound(0.39*H), intRound(0.955*W), intRound(0.46*H));
	m_RightButton.Create(_T(""), _T(""), 0, Rect, this, IDC_VIDEO_RIGHT_BTN);
	m_RightButton.SetImage(W == 1920 ? IDR_RIGHT_ARROW_3 : W == 1600 ? IDR_RIGHT_ARROW_2 : IDR_RIGHT_ARROW_1);
	m_RightButton.SetBk(FALSE);
	m_RightButton.SetEnabled(FALSE);
	m_RightButton.SetTip("Video range move to right");
	m_RightButton.ShowWindow(SW_SHOW);

	::SetRect(&Rect, intRound(0.87*W), intRound(0.32*H), intRound(0.91*W), intRound(0.40*H));
	m_UpButton.Create(_T(""), _T(""), 0, Rect, this, IDC_VIDEO_UP_BTN);
	m_UpButton.SetImage(IDR_UP_ARROW_4);
	m_UpButton.SetBk(FALSE);
	m_UpButton.SetEnabled(FALSE);
	m_UpButton.SetTip("Video range up");
	m_UpButton.ShowWindow(SW_SHOW);

	::SetRect(&Rect, intRound(0.87*W), intRound(0.45*H), intRound(0.91*W), intRound(0.53*H));
	m_DownButton.Create(_T(""), _T(""), 0, Rect, this, IDC_VIDEO_DOWN_BTN);
	m_DownButton.SetImage(IDR_DOWN_ARROW_4);
	m_DownButton.SetBk(FALSE);
	m_DownButton.SetEnabled(FALSE);
	m_DownButton.SetTip("Video range down");
	m_DownButton.ShowWindow(SW_SHOW);

	return TRUE;
}

//***************************************************************************************

void CVideoMoveDlg::OnCancel()
{
	CDialog::OnCancel();
}

//***************************************************************************************

void CVideoMoveDlg::OnOK()
{
	m_pHW->m_Calibration.StepLeft = m_StepLeft + 8;
	m_pHW->m_Calibration.StepRight = m_StepRight + 8;
	m_pHW->m_Calibration.StepUp = m_StepUp + 5;
	m_pHW->m_Calibration.StepDown = m_StepDown + 5;

	CDialog::OnOK();
}

//***************************************************************************************

void CVideoMoveDlg::OnStartVideoButtonClicked()
{
	if (!m_pLiveVideo->Start()) return;

	m_OKButton.EnableWindow(FALSE);
	m_CancelButton.EnableWindow(FALSE);
	m_StartVideoButton.EnableWindow(FALSE);
	m_AbortVideoButton.EnableWindow(TRUE);

	if (m_StepLeft != 8) { m_LeftButton.SetEnabled(TRUE); m_LeftButton.Repaint(); }
	if (m_StepRight != 8) { m_RightButton.SetEnabled(TRUE); m_RightButton.Repaint(); }
	if (m_StepUp != 5) { m_UpButton.SetEnabled(TRUE); m_UpButton.Repaint(); }
	if (m_StepDown != 5) { m_DownButton.SetEnabled(TRUE); m_DownButton.Repaint(); }
}

//***************************************************************************************

void CVideoMoveDlg::OnAbortVideoButtonClicked()
{
	m_pLiveVideo->Abort();
}

//***************************************************************************************

void CVideoMoveDlg::OnVideoLeftButtonClicked()
{
	m_StepLeft++;
	m_StepRight = -1 * m_StepLeft;

	::HW.m_Calibration.StepLeft = m_StepLeft + 8;
	::HW.m_Calibration.StepRight = m_StepRight + 8;

	m_RightButton.SetEnabled(TRUE);  m_RightButton.Repaint();

	CString s;
	real EditC = m_StepLeft / m_x_px_um;
	s.Format(_T("%.2f"), EditC);
	m_LeftEdit.SetWindowText(s);

	EditC = -1 * EditC;
	s.Format(_T("%.2f"), EditC);
	m_RightEdit.SetWindowText(s);

	if (m_StepLeft == 8) { m_LeftButton.SetEnabled(FALSE);  m_LeftButton.Repaint(); }
}

//***************************************************************************************

void CVideoMoveDlg::OnVideoRightButtonClicked()
{
	m_StepRight++;
	m_StepLeft = -1 * m_StepRight;

	::HW.m_Calibration.StepLeft = m_StepLeft + 8;
	::HW.m_Calibration.StepRight = m_StepRight + 8;

	m_LeftButton.SetEnabled(TRUE);  m_LeftButton.Repaint();

	CString s;
	real EditC = m_StepLeft / m_x_px_um;
	s.Format(_T("%.2f"), EditC);
	m_LeftEdit.SetWindowText(s);

	EditC = -1 * EditC;
	s.Format(_T("%.2f"), EditC);
	m_RightEdit.SetWindowText(s);

	if (m_StepRight == 8) { m_RightButton.SetEnabled(FALSE);  m_RightButton.Repaint(); }
}

//***************************************************************************************

void CVideoMoveDlg::OnVideoUpButtonClicked()
{
	m_StepUp++;
	m_StepDown = -1 * m_StepUp;

	::HW.m_Calibration.StepUp = m_StepUp + 5;
	::HW.m_Calibration.StepDown = m_StepDown + 5;

	m_DownButton.SetEnabled(TRUE);  m_DownButton.Repaint();

	CString s;
	real EditC = m_StepUp / m_y_px_um;
	s.Format(_T("%.2f"), EditC);
	m_UpEdit.SetWindowText(s);

	EditC = -1 * EditC;
	s.Format(_T("%.2f"), EditC);
	m_DownEdit.SetWindowText(s);

	if (m_StepUp == 5) { m_UpButton.SetEnabled(FALSE);  m_UpButton.Repaint(); }
}

//***************************************************************************************

void CVideoMoveDlg::OnVideoDownButtonClicked()
{
	m_StepDown++;
	m_StepUp = -1 * m_StepDown;

	::HW.m_Calibration.StepUp = m_StepUp + 5;
	::HW.m_Calibration.StepDown = m_StepDown + 5;

	m_UpButton.SetEnabled(TRUE);  m_UpButton.Repaint();

	CString s;
	real EditC = m_StepUp / m_y_px_um;
	s.Format(_T("%.2f"), EditC);
	m_UpEdit.SetWindowText(s);

	EditC = -1 * EditC;
	s.Format(_T("%.2f"), EditC);
	m_DownEdit.SetWindowText(s);

	if (m_StepDown == 5) { m_DownButton.SetEnabled(FALSE);  m_DownButton.Repaint(); }
}

//***************************************************************************************

LRESULT CVideoMoveDlg::OnFinish(WPARAM wParam, LPARAM lParam)
{
	m_pLiveVideo->Finish();

	m_OKButton.EnableWindow(TRUE);
	m_CancelButton.EnableWindow(TRUE);
	m_StartVideoButton.EnableWindow(TRUE);
	m_AbortVideoButton.EnableWindow(FALSE);
	//
	m_LeftButton.SetEnabled(FALSE);
	m_RightButton.SetEnabled(FALSE);
	m_UpButton.SetEnabled(FALSE);
	m_DownButton.SetEnabled(FALSE);

	m_LeftButton.Repaint();
	m_RightButton.Repaint();
	m_UpButton.Repaint();
	m_DownButton.Repaint();

	return 0;
}

//***************************************************************************************

LRESULT CVideoMoveDlg::OnUpdate(WPARAM wParam, LPARAM lParam)
{
	CString s;
	int fs = 16;
	CMFont Font(fs, 600, "Arial");

	uchar* pRGBData = m_pHW->GetRGBData();
	memcpy(m_VideoWnd.m_MemDC.m_RGBData, pRGBData, CHW::m_VideoSize);



	m_VideoWnd.Invalidate(FALSE);
	m_VideoWnd.UpdateWindow();

	return 0;
}

//***************************************************************************************
