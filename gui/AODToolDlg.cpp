//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "AODToolDlg.h"

#include "BusyCursor.h"//

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CAODToolDlg, CDialog)

	ON_MESSAGE(WM_THREAD_UPDATE, OnThreadUpdate)
	ON_MESSAGE(WM_THREAD_FINISH, OnThreadFinish)

	ON_BN_CLICKED(IDC_CAL_V_LINE_RADIO, OnRadioClicked)
	ON_BN_CLICKED(IDC_CAL_H_LINE_RADIO, OnRadioClicked)
	ON_BN_CLICKED(IDC_CAL_LTRB_LINE_RADIO, OnRadioClicked)
	ON_BN_CLICKED(IDC_CAL_LBRT_LINE_RADIO, OnRadioClicked)
	ON_BN_CLICKED(IDC_CAL_CIRCLE_RADIO, OnRadioClicked)
	ON_BN_CLICKED(IDC_CAL_CROSS_RADIO, OnRadioClicked)
	ON_BN_CLICKED(IDC_CAL_LETTER_RADIO, OnRadioClicked)
	ON_BN_CLICKED(IDC_CAL_SQUARE_RADIO, OnRadioClicked)
	ON_BN_CLICKED(IDC_CAL_PROBE_RADIO, OnRadioClicked)
	ON_BN_CLICKED(IDC_CAL_SCAN_RADIO, OnRadioClicked)

	ON_BN_CLICKED(IDC_CAL_FIRST_POINT, OnFirstButtonClicked)
	ON_BN_CLICKED(IDC_CAL_NEXT_POINT, OnNextButtonClicked)
	ON_BN_CLICKED(IDC_CAL_PREV_POINT, OnPrevButtonClicked)
	ON_BN_CLICKED(IDC_CAL_LOOP_SCANNING, OnLoopButtonClicked)
	ON_BN_CLICKED(IDC_CAL_STOP_SCANNING, OnStopButtonClicked)

	ON_BN_CLICKED(IDC_CAL_MOVE_LEFT, OnMoveLtButtonClicked)
	ON_BN_CLICKED(IDC_CAL_MOVE_RIGHT, OnMoveRtButtonClicked)
	ON_BN_CLICKED(IDC_CAL_MOVE_UP, OnMoveUpButtonClicked)
	ON_BN_CLICKED(IDC_CAL_MOVE_DOWN, OnMoveDnButtonClicked)
	ON_BN_CLICKED(IDC_CAL_INC_X, OnIncXButtonClicked)
	ON_BN_CLICKED(IDC_CAL_DEC_X, OnDecXButtonClicked)
	ON_BN_CLICKED(IDC_CAL_INC_Y, OnIncYButtonClicked)
	ON_BN_CLICKED(IDC_CAL_DEC_Y, OnDecYButtonClicked)

	ON_BN_CLICKED(IDC_CAL_DEC_LAD_ZOOM, OnDecLADZoomButtonClicked)
	ON_BN_CLICKED(IDC_CAL_INC_LAD_ZOOM, OnIncLADZoomButtonClicked)

	ON_BN_CLICKED(IDC_CAL_SAVE3, OnCalSaveBtnClicked)//

END_MESSAGE_MAP()

//***************************************************************************************

CAODToolDlg::CAODToolDlg(CHW* pHW, CWnd* pParentWnd) :
	CDialog(IDD_AOD_TOOL_DLG, pParentWnd)
{
	m_pHW = pHW;
	m_Thread = NULL;
}

//***************************************************************************************
//[cjf***40092012] used to handle 'f/l/s' 3 shortkeys
BOOL CAODToolDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		if (pMsg->wParam == 70)
		{
			if (m_LoopButton.IsWindowEnabled())
			{
				CAODToolDlg::OnFirstButtonClicked();
				return true;
			}
		}
		else if (pMsg->wParam == 76)
		{
			if (m_FirstButton.IsWindowEnabled())
			{
				CAODToolDlg::OnLoopButtonClicked();
				return true;
			}
		}
		else if (pMsg->wParam == 83)
		{
			CAODToolDlg::OnStopButtonClicked();
			return true;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
//[cjf***40092012]
//***************************************************************************************

void CAODToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CAL_V_LINE_RADIO, m_VLineRadio);
	DDX_Control(pDX, IDC_CAL_H_LINE_RADIO, m_HLineRadio);
	DDX_Control(pDX, IDC_CAL_LTRB_LINE_RADIO, m_LTRBLineRadio);
	DDX_Control(pDX, IDC_CAL_LBRT_LINE_RADIO, m_LBRTLineRadio);
	DDX_Control(pDX, IDC_CAL_CIRCLE_RADIO, m_CircleRadio);
	DDX_Control(pDX, IDC_CAL_CROSS_RADIO, m_CrossRadio);
	DDX_Control(pDX, IDC_CAL_LETTER_RADIO, m_LetterRadio);
	DDX_Control(pDX, IDC_CAL_SQUARE_RADIO, m_SquareRadio);
	DDX_Control(pDX, IDC_CAL_PROBE_RADIO, m_ProbeRadio);
	DDX_Control(pDX, IDC_CAL_SCAN_RADIO, m_ScanRadio);

	DDX_Control(pDX, IDC_CAL_RADIUS_EDIT, m_RadiusEdit);

	DDX_Control(pDX, IDC_CAL_FIRST_POINT, m_FirstButton);
	DDX_Control(pDX, IDC_CAL_NEXT_POINT, m_NextButton);
	DDX_Control(pDX, IDC_CAL_PREV_POINT, m_PrevButton);
	DDX_Control(pDX, IDC_CAL_LOOP_SCANNING, m_LoopButton);
	DDX_Control(pDX, IDC_CAL_STOP_SCANNING, m_StopButton);

	DDX_Control(pDX, IDC_CAL_AODX_FREQ0_EDIT, m_CentFreqXEdit);
	DDX_Control(pDX, IDC_CAL_AODX_HZ_UM_EDIT, m_HzUmXEdit);
	DDX_Control(pDX, IDC_CAL_AODY_FREQ0_EDIT, m_CentFreqYEdit);
	DDX_Control(pDX, IDC_CAL_AODY_HZ_UM_EDIT, m_HzUmYEdit);

	DDX_Control(pDX, IDC_CAL_MOVE_RIGHT, m_MoveRtButton);
	DDX_Control(pDX, IDC_CAL_MOVE_LEFT, m_MoveLtButton);
	DDX_Control(pDX, IDC_CAL_MOVE_UP, m_MoveUpButton);
	DDX_Control(pDX, IDC_CAL_MOVE_DOWN, m_MoveDnButton);
	DDX_Control(pDX, IDC_CAL_INC_X, m_IncXButton);
	DDX_Control(pDX, IDC_CAL_DEC_X, m_DecXButton);
	DDX_Control(pDX, IDC_CAL_INC_Y, m_IncYButton);
	DDX_Control(pDX, IDC_CAL_DEC_Y, m_DecYButton);

	DDX_Control(pDX, IDC_CAL_LASER_ON_CHECK, m_LaserOnCheck);
	DDX_Control(pDX, IDC_CAL_AODS_ON_CHECK, m_AODsOnCheck);
	DDX_Control(pDX, IDC_CAL_LADS_ON_CHECK, m_LADsOnCheck);

	DDX_Control(pDX, IDC_CAL_DEC_LAD_ZOOM, m_DecLADZoomButton);
	DDX_Control(pDX, IDC_CAL_INC_LAD_ZOOM, m_IncLADZoomButton);

	DDX_Control(pDX, IDC_CAL_AODX_RMS_EDIT, m_AODXRMSEdit);//
	DDX_Control(pDX, IDC_CAL_AODY_RMS_EDIT, m_AODYRMSEdit);//

	DDX_Control(pDX, IDC_CAL_SAVE3, m_CalSaveButton);//
}

//***************************************************************************************

BOOL CAODToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_BackupAODXFreq0 = m_pHW->m_Calibration.AODX.Freq0;
	m_BackupAODYFreq0 = m_pHW->m_Calibration.AODY.Freq0;
	m_BackupAODXHzUm = m_pHW->m_Calibration.AODX.HzUm;
	m_BackupAODYHzUm = m_pHW->m_Calibration.AODY.HzUm;
	m_BackupAODXRms = m_pHW->m_Calibration.AODX.RMS;//
	m_BackupAODYRms = m_pHW->m_Calibration.AODY.RMS;//

	m_LADZoom = 2;

	RECT Rect;

	::SetRect(&Rect, 90, 10, 90 + 181, 10 + 181);
	m_GridWnd.CreateWnd(Rect, this);

	::SetRect(&Rect, 280, 10, 280 + 565, 10 + 280);
	m_LADXWnd.CreateWnd(Rect, this);

	::SetRect(&Rect, 280, 300, 280 + 565, 300 + 280);
	m_LADYWnd.CreateWnd(Rect, this);

	//m_GridWnd.Repaint(); will be repainted in OnRadioClicked()
	m_LADXWnd.Repaint();
	m_LADYWnd.Repaint();

	m_RadiusEdit.SetWindowText(_T("3000"));

	CString s;
	s.Format(_T("%i"), m_pHW->m_Calibration.AODX.Freq0);
	m_CentFreqXEdit.SetWindowText(s);

	s.Format(_T("%i"), m_pHW->m_Calibration.AODY.Freq0);
	m_CentFreqYEdit.SetWindowText(s);

	s.Format(_T("%i"), m_pHW->m_Calibration.AODX.HzUm);
	m_HzUmXEdit.SetWindowText(s);

	s.Format(_T("%i"), m_pHW->m_Calibration.AODY.HzUm);
	m_HzUmYEdit.SetWindowText(s);

	//
	s.Format(_T("%i"), m_pHW->m_Calibration.AODX.RMS);
	m_AODXRMSEdit.SetWindowText(s);

	s.Format(_T("%i"), m_pHW->m_Calibration.AODY.RMS);
	m_AODYRMSEdit.SetWindowText(s);
	//

	m_LaserOnCheck.SetCheck(1);
	m_AODsOnCheck.SetCheck(1);
	m_LADsOnCheck.SetCheck(1);

	m_ScanRadio.SetCheck(1);
	OnRadioClicked();

	GetDlgItem(IDC_X_HZ_STATIC)->SetWindowText(_T(""));
	GetDlgItem(IDC_Y_HZ_STATIC)->SetWindowText(_T(""));

	return TRUE;
}

//***************************************************************************************

void CAODToolDlg::OnStopButtonClicked()
{
	if (m_Thread != NULL) {
		AbortThread();
	}
	else {
		m_pHW->StopContinuousScanning();

		m_GridWnd.m_PointsColor = GREEN;
		m_GridWnd.Repaint();

		EnableRadios(TRUE);
		m_FirstButton.EnableWindow(TRUE);
		m_LoopButton.EnableWindow(TRUE);
		m_StopButton.EnableWindow(FALSE);
		EnableFreqButtons(FALSE);
		m_LaserOnCheck.EnableWindow(TRUE);
		m_AODsOnCheck.EnableWindow(TRUE);
		m_LADsOnCheck.EnableWindow(TRUE);
	}
}

//***************************************************************************************

void CAODToolDlg::OnFirstButtonClicked()
{
	m_GridWnd.m_CurPoint = 0;
	//m_GridWnd.Repaint(); will be repainted in OnRadioClicked()

	OnRadioClicked(); // in case radius changed

	EnableRadios(FALSE);
	m_FirstButton.EnableWindow(FALSE);
	m_NextButton.EnableWindow(TRUE);
	m_PrevButton.EnableWindow(TRUE);
	m_LoopButton.EnableWindow(FALSE);
	m_StopButton.EnableWindow(TRUE);

	StartThread();
}

//***************************************************************************************

void CAODToolDlg::OnPrevButtonClicked()
{
	if (--m_GridWnd.m_CurPoint == -1) m_GridWnd.m_CurPoint = m_GridWnd.m_XUm.GetSize() - 1;
	m_GridWnd.Repaint();
}

//***************************************************************************************

void CAODToolDlg::OnNextButtonClicked()
{
	if (++m_GridWnd.m_CurPoint == m_GridWnd.m_XUm.GetSize()) m_GridWnd.m_CurPoint = 0;
	m_GridWnd.Repaint();
}

//***************************************************************************************

void CAODToolDlg::OnRadioClicked()
{
	CString s;
	m_RadiusEdit.GetWindowText(s);
	CStringA ansiStr(s);
	int r_um = atoi(ansiStr);
	if (r_um < 500) {
		r_um = 500;
		m_RadiusEdit.SetWindowText(_T("500"));
	}
	else if (r_um > 4000) {
		r_um = 4000;
		m_RadiusEdit.SetWindowText(_T("4000"));
	}

	if (m_VLineRadio.GetCheck()) m_GridWnd.CreateVLinePattern(r_um);
	else if (m_HLineRadio.GetCheck()) m_GridWnd.CreateHLinePattern(r_um);
	else if (m_LTRBLineRadio.GetCheck()) m_GridWnd.CreateLTRBLinePattern(r_um);
	else if (m_LBRTLineRadio.GetCheck()) m_GridWnd.CreateLBRTLinePattern(r_um);
	else if (m_CircleRadio.GetCheck()) m_GridWnd.CreateCirclePattern(r_um);
	else if (m_CrossRadio.GetCheck()) m_GridWnd.CreateCrossPattern(r_um);
	else if (m_LetterRadio.GetCheck()) m_GridWnd.CreateLetterPattern(r_um);
	else if (m_SquareRadio.GetCheck()) m_GridWnd.CreateSquarePattern(r_um);
	else if (m_ProbeRadio.GetCheck()) m_GridWnd.CreateProbePattern(r_um);
	else if (m_ScanRadio.GetCheck()) m_GridWnd.CreateScanPattern(r_um);

	m_GridWnd.Repaint();
}

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

LRESULT CAODToolDlg::OnThreadUpdate(WPARAM wParam, LPARAM lParam)
{
	m_LADXWnd.m_Zoom = m_LADZoom;
	m_LADYWnd.m_Zoom = m_LADZoom;
	m_LADXWnd.Repaint();
	m_LADYWnd.Repaint();
	return 0;
}

//***************************************************************************************

LRESULT CAODToolDlg::OnThreadFinish(WPARAM wParam, LPARAM lParam)
{
	::WaitForSingleObject(m_Thread, INFINITE);
	::CloseHandle(m_Thread);
	m_Thread = NULL;

	EnableRadios(TRUE);
	m_FirstButton.EnableWindow(TRUE);
	m_NextButton.EnableWindow(FALSE);
	m_PrevButton.EnableWindow(FALSE);
	m_LoopButton.EnableWindow(TRUE);
	m_StopButton.EnableWindow(FALSE);

	m_GridWnd.m_CurPoint = -1;
	m_GridWnd.Repaint();

	return 0;
}

//***************************************************************************************

void CAODToolDlg::Main()
{
	int CurPoint = -1;

	BOOL LaserOn = FALSE;
	BOOL AODsOn = FALSE;
	BOOL LADsOn = FALSE;

	//
	CString s;
	m_AODXRMSEdit.GetWindowText(s);
	G_As = s;
	m_pHW->m_Calibration.AODX.RMS = atoi(G_As);
	m_AODYRMSEdit.GetWindowText(s);
	G_As = s;
	m_pHW->m_Calibration.AODY.RMS = atoi(G_As);
	//

	while (m_ThreadAborted == FALSE) {

		BOOL LaserShouldBeOn = m_LaserOnCheck.GetCheck();
		if (LaserOn && !LaserShouldBeOn) m_pHW->TurnScanningLaserOff();
		else if (!LaserOn && LaserShouldBeOn) m_pHW->TurnScanningLaserOn();
		LaserOn = LaserShouldBeOn;

		BOOL AODsShouldBeOn = m_AODsOnCheck.GetCheck();
		if (AODsOn && !AODsShouldBeOn) m_pHW->TurnAODsOff();
		else if (!AODsOn && AODsShouldBeOn) m_pHW->TurnAODsOn();
		AODsOn = AODsShouldBeOn;

		BOOL LADsShouldBeOn = m_LADsOnCheck.GetCheck();
		if (LADsOn && !LADsShouldBeOn) m_pHW->TurnLADsOff();
		else if (!LADsOn && LADsShouldBeOn) m_pHW->TurnLADsOn();
		LADsOn = LADsShouldBeOn;

		if (CurPoint != m_GridWnd.m_CurPoint) {
			CurPoint = m_GridWnd.m_CurPoint;
			m_pHW->SetAODsFrequencies(m_GridWnd.m_XUm[CurPoint], m_GridWnd.m_YUm[CurPoint]);
			CString s;
			s.Format(_T("X = %i Hz"), m_pHW->m_XHz);
			GetDlgItem(IDC_X_HZ_STATIC)->SetWindowText(s);
			s.Format(_T("Y = %i Hz"), m_pHW->m_YHz);
			GetDlgItem(IDC_Y_HZ_STATIC)->SetWindowText(s);
		}

		m_pHW->ReadLADs(m_LADXWnd.m_LAD.m_Signal, m_LADYWnd.m_LAD.m_Signal);
		m_LADXWnd.m_LAD.Process();
		m_LADYWnd.m_LAD.Process();

		SendMessage(WM_THREAD_UPDATE, 0, 0);

		::Sleep(10);
	}

	m_pHW->TurnScanningLaserOff();
	m_pHW->TurnAODsOff();
	m_pHW->TurnLADsOff();

	GetDlgItem(IDC_X_HZ_STATIC)->SetWindowText(_T(""));
	GetDlgItem(IDC_Y_HZ_STATIC)->SetWindowText(_T(""));

	PostMessage(WM_THREAD_FINISH, 0, 0);
}

static ulong __stdcall ThreadFunction(void* pParam)
{
	((CAODToolDlg*)pParam)->Main();
	return 0;
}

void CAODToolDlg::StartThread()
{
	m_ThreadAborted = FALSE;
	ulong ID;
	m_Thread = ::CreateThread(NULL, 0, ThreadFunction, this, 0, &ID);
}

void CAODToolDlg::AbortThread()
{
	m_ThreadAborted = TRUE;
}

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

void CAODToolDlg::OnLoopButtonClicked()
{
	m_GridWnd.m_PointsColor = RED;
	m_GridWnd.Repaint();

	EnableRadios(FALSE);
	m_FirstButton.EnableWindow(FALSE);
	m_LoopButton.EnableWindow(FALSE);
	m_StopButton.EnableWindow(TRUE);
	EnableFreqButtons(TRUE);
	m_LaserOnCheck.EnableWindow(FALSE);
	m_AODsOnCheck.EnableWindow(FALSE);
	m_LADsOnCheck.EnableWindow(FALSE);

	//
	CString s;
	m_AODXRMSEdit.GetWindowText(s);
	G_As = s;
	m_pHW->m_Calibration.AODX.RMS = atoi(G_As);
	m_AODYRMSEdit.GetWindowText(s);
	G_As = s;
	m_pHW->m_Calibration.AODY.RMS = atoi(G_As);
	//

	m_pHW->StartContinuousScanning(m_GridWnd.m_XUm.GetSize(), m_GridWnd.m_XUm.GetMem(), m_GridWnd.m_YUm.GetMem());
}

//---------------------------------------------------------------------------------------

void CAODToolDlg::OnMoveRtButtonClicked()
{
	m_pHW->m_Calibration.AODX.Freq0 -= 50000;
	CString s;
	s.Format(_T("%i"), m_pHW->m_Calibration.AODX.Freq0);
	m_CentFreqXEdit.SetWindowText(s);

	//
	m_AODXRMSEdit.GetWindowText(s);
	G_As = s;
	m_pHW->m_Calibration.AODX.RMS = atoi(G_As);
	m_AODYRMSEdit.GetWindowText(s);
	G_As = s;
	m_pHW->m_Calibration.AODY.RMS = atoi(G_As);
	//

	m_pHW->StopContinuousScanning();
	m_pHW->StartContinuousScanning(m_GridWnd.m_XUm.GetSize(), m_GridWnd.m_XUm.GetMem(), m_GridWnd.m_YUm.GetMem());
}

void CAODToolDlg::OnMoveLtButtonClicked()
{
	m_pHW->m_Calibration.AODX.Freq0 += 50000;
	CString s;
	s.Format(_T("%i"), m_pHW->m_Calibration.AODX.Freq0);
	m_CentFreqXEdit.SetWindowText(s);

	//
	m_AODXRMSEdit.GetWindowText(s);
	G_As = s;
	m_pHW->m_Calibration.AODX.RMS = atoi(G_As);
	m_AODYRMSEdit.GetWindowText(s);
	G_As = s;
	m_pHW->m_Calibration.AODY.RMS = atoi(G_As);
	//

	m_pHW->StopContinuousScanning();
	m_pHW->StartContinuousScanning(m_GridWnd.m_XUm.GetSize(), m_GridWnd.m_XUm.GetMem(), m_GridWnd.m_YUm.GetMem());
}

void CAODToolDlg::OnMoveUpButtonClicked()
{
	m_pHW->m_Calibration.AODY.Freq0 += 50000;
	CString s;
	s.Format(_T("%i"), m_pHW->m_Calibration.AODY.Freq0);
	m_CentFreqYEdit.SetWindowText(s);

	//
	m_AODXRMSEdit.GetWindowText(s);
	G_As = s;
	m_pHW->m_Calibration.AODX.RMS = atoi(G_As);
	m_AODYRMSEdit.GetWindowText(s);
	G_As = s;
	m_pHW->m_Calibration.AODY.RMS = atoi(G_As);
	//

	m_pHW->StopContinuousScanning();
	m_pHW->StartContinuousScanning(m_GridWnd.m_XUm.GetSize(), m_GridWnd.m_XUm.GetMem(), m_GridWnd.m_YUm.GetMem());
}

void CAODToolDlg::OnMoveDnButtonClicked()
{
	m_pHW->m_Calibration.AODY.Freq0 -= 50000;
	CString s;
	s.Format(_T("%i"), m_pHW->m_Calibration.AODY.Freq0);
	m_CentFreqYEdit.SetWindowText(s);

	//
	m_AODXRMSEdit.GetWindowText(s);
	G_As = s;
	m_pHW->m_Calibration.AODX.RMS = atoi(G_As);
	m_AODYRMSEdit.GetWindowText(s);
	G_As = s;
	m_pHW->m_Calibration.AODY.RMS = atoi(G_As);
	//

	m_pHW->StopContinuousScanning();
	m_pHW->StartContinuousScanning(m_GridWnd.m_XUm.GetSize(), m_GridWnd.m_XUm.GetMem(), m_GridWnd.m_YUm.GetMem());
}

//---------------------------------------------------------------------------------------

void CAODToolDlg::OnIncXButtonClicked()
{
	m_pHW->m_Calibration.AODX.HzUm += 5;
	CString s;
	s.Format(_T("%i"), m_pHW->m_Calibration.AODX.HzUm);
	m_HzUmXEdit.SetWindowText(s);

	//
	m_AODXRMSEdit.GetWindowText(s);
	G_As = s;
	m_pHW->m_Calibration.AODX.RMS = atoi(G_As);
	m_AODYRMSEdit.GetWindowText(s);
	G_As = s;
	m_pHW->m_Calibration.AODY.RMS = atoi(G_As);
	//

	m_pHW->StopContinuousScanning();
	m_pHW->StartContinuousScanning(m_GridWnd.m_XUm.GetSize(), m_GridWnd.m_XUm.GetMem(), m_GridWnd.m_YUm.GetMem());
}

void CAODToolDlg::OnDecXButtonClicked()
{
	m_pHW->m_Calibration.AODX.HzUm -= 5;
	CString s;
	s.Format(_T("%i"), m_pHW->m_Calibration.AODX.HzUm);
	m_HzUmXEdit.SetWindowText(s);

	//
	m_AODXRMSEdit.GetWindowText(s);
	G_As = s;
	m_pHW->m_Calibration.AODX.RMS = atoi(G_As);
	m_AODYRMSEdit.GetWindowText(s);
	G_As = s;
	m_pHW->m_Calibration.AODY.RMS = atoi(G_As);
	//

	m_pHW->StopContinuousScanning();
	m_pHW->StartContinuousScanning(m_GridWnd.m_XUm.GetSize(), m_GridWnd.m_XUm.GetMem(), m_GridWnd.m_YUm.GetMem());
}

void CAODToolDlg::OnIncYButtonClicked()
{
	m_pHW->m_Calibration.AODY.HzUm += 5;
	CString s;
	s.Format(_T("%i"), m_pHW->m_Calibration.AODY.HzUm);
	m_HzUmYEdit.SetWindowText(s);

	//
	m_AODXRMSEdit.GetWindowText(s);
	G_As = s;
	m_pHW->m_Calibration.AODX.RMS = atoi(G_As);
	m_AODYRMSEdit.GetWindowText(s);
	G_As = s;
	m_pHW->m_Calibration.AODY.RMS = atoi(G_As);
	//

	m_pHW->StopContinuousScanning();
	m_pHW->StartContinuousScanning(m_GridWnd.m_XUm.GetSize(), m_GridWnd.m_XUm.GetMem(), m_GridWnd.m_YUm.GetMem());
}

void CAODToolDlg::OnDecYButtonClicked()
{
	m_pHW->m_Calibration.AODY.HzUm -= 5;
	CString s;
	s.Format(_T("%i"), m_pHW->m_Calibration.AODY.HzUm);
	m_HzUmYEdit.SetWindowText(s);

	//
	m_AODXRMSEdit.GetWindowText(s);
	G_As = s;
	m_pHW->m_Calibration.AODX.RMS = atoi(G_As);
	m_AODYRMSEdit.GetWindowText(s);
	G_As = s;
	m_pHW->m_Calibration.AODY.RMS = atoi(G_As);
	//

	m_pHW->StopContinuousScanning();
	m_pHW->StartContinuousScanning(m_GridWnd.m_XUm.GetSize(), m_GridWnd.m_XUm.GetMem(), m_GridWnd.m_YUm.GetMem());
}

//***************************************************************************************

void CAODToolDlg::OnDecLADZoomButtonClicked()
{
	if (--m_LADZoom < 2) m_LADZoom = 2;
}

void CAODToolDlg::OnIncLADZoomButtonClicked()
{
	if (++m_LADZoom > 8) m_LADZoom = 8;
}

//***************************************************************************************

void CAODToolDlg::OnCancel()
{
	if (m_StopButton.IsWindowEnabled()) return;

	m_pHW->m_Calibration.AODX.Freq0 = m_BackupAODXFreq0;
	m_pHW->m_Calibration.AODY.Freq0 = m_BackupAODYFreq0;
	m_pHW->m_Calibration.AODX.HzUm = m_BackupAODXHzUm;
	m_pHW->m_Calibration.AODY.HzUm = m_BackupAODYHzUm;

	m_pHW->m_Calibration.AODX.RMS = m_BackupAODXRms;//
	m_pHW->m_Calibration.AODY.RMS = m_BackupAODYRms;//

	CDialog::OnCancel();
}

//***************************************************************************************

void CAODToolDlg::OnOK()
{
	if (m_StopButton.IsWindowEnabled()) return;
	CDialog::OnOK();
}

//***************************************************************************************

void CAODToolDlg::EnableRadios(const BOOL enable)
{
	m_RadiusEdit.EnableWindow(enable);
	m_VLineRadio.EnableWindow(enable);
	m_HLineRadio.EnableWindow(enable);
	m_LTRBLineRadio.EnableWindow(enable);
	m_LBRTLineRadio.EnableWindow(enable);
	m_CircleRadio.EnableWindow(enable);
	m_CrossRadio.EnableWindow(enable);
	m_LetterRadio.EnableWindow(enable);
	m_SquareRadio.EnableWindow(enable);
	m_ProbeRadio.EnableWindow(enable);
	m_ScanRadio.EnableWindow(enable);
}

//***************************************************************************************

void CAODToolDlg::EnableFreqButtons(const BOOL enable)
{
	m_MoveRtButton.EnableWindow(enable);
	m_MoveLtButton.EnableWindow(enable);
	m_MoveUpButton.EnableWindow(enable);
	m_MoveDnButton.EnableWindow(enable);
	m_IncXButton.EnableWindow(enable);
	m_DecXButton.EnableWindow(enable);
	m_IncYButton.EnableWindow(enable);
	m_DecYButton.EnableWindow(enable);
}

//**************************************************************************************

//610 Save the cal values to Exporm
void CAODToolDlg::OnCalSaveBtnClicked()
{
	CBusyCursor Cursor;

	CString s;

	//610
	m_AODXRMSEdit.GetWindowText(s);
	G_As = s;
	m_pHW->m_Calibration.AODX.RMS = atoi(G_As);
	m_AODYRMSEdit.GetWindowText(s);
	G_As = s;
	m_pHW->m_Calibration.AODY.RMS = atoi(G_As);
	//610

	m_pHW->SaveCalibrationIntoFlash();
}

//***************************************************************************************