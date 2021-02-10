//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "Video2ToolDlg.h"
#include "BusyCursor.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CVideo2ToolDlg, CDialog)

	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CAL_START_VIDEO, OnStartVideoButtonClicked)
	ON_BN_CLICKED(IDC_CAL_ABORT_VIDEO, OnAbortVideoButtonClicked)
	ON_BN_CLICKED(IDC_CAL_START, OnStartButtonClicked)
	ON_MESSAGE(WM_THREAD_UPDATE, OnUpdate)
	ON_MESSAGE(WM_THREAD_FINISH, OnFinish)

	ON_BN_CLICKED(IDC_PLACIDO_CHK, OnPlacdioCheckClicked) // test cjf 10292020

END_MESSAGE_MAP()

//***************************************************************************************

CVideo2ToolDlg::CVideo2ToolDlg(CHW* pHW, CWnd* pParentWnd) :
	CDialog(IDD_VIDEO2_TOOL_DLG, pParentWnd)
{
	m_pHW = pHW;
	m_pHW->m_pCurrentVideoSettings = &m_pHW->m_Calibration.WFVideo2Settings;
	m_pLiveVideo = new CLiveVideo(m_pHW);
	m_pLiveVideo->m_pVideoWnd = this;
}

//***************************************************************************************

CVideo2ToolDlg::~CVideo2ToolDlg()
{
	delete m_pLiveVideo;
}

//***************************************************************************************

void CVideo2ToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDOK, m_OKButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_CAL_START_VIDEO, m_StartVideoButton);
	DDX_Control(pDX, IDC_CAL_ABORT_VIDEO, m_AbortVideoButton);
	DDX_Control(pDX, IDC_CAL_START, m_StartButton);
	DDX_Control(pDX, IDC_CAL_VIDEO_INFRARED_LEDS_POWER_LEVEL_EDIT, m_InfraredLEDsPowerLevelEdit);
	DDX_Control(pDX, IDC_CAL_VIDEO_WHITE_LEDS_POWER_LEVEL_EDIT, m_WhiteLEDsPowerLevelEdit);
	DDX_Control(pDX, IDC_CAL_VIDEO_BRIGHTNESS_EDIT, m_BrightnessEdit);
	DDX_Control(pDX, IDC_CAL_VIDEO_CONTRAST_EDIT, m_ContrastEdit);
	DDX_Control(pDX, IDC_CAL_VIDEO_HUE_EDIT, m_HueEdit);
	DDX_Control(pDX, IDC_CAL_VIDEO_SATURATION_EDIT, m_SaturationEdit);
	DDX_Control(pDX, IDC_CAL_VIDEO_DELAY_EDIT, m_DelayEdit);
	DDX_Control(pDX, IDC_CAL_VIDEO_INFRARED_LEDS_POWER_LEVEL_SLIDER, m_InfraredLEDsPowerLevelSlider);
	DDX_Control(pDX, IDC_CAL_VIDEO_WHITE_LEDS_POWER_LEVEL_SLIDER, m_WhiteLEDsPowerLevelSlider);
	DDX_Control(pDX, IDC_CAL_VIDEO_BRIGHTNESS_SLIDER, m_BrightnessSlider);
	DDX_Control(pDX, IDC_CAL_VIDEO_CONTRAST_SLIDER, m_ContrastSlider);
	DDX_Control(pDX, IDC_CAL_VIDEO_HUE_SLIDER, m_HueSlider);
	DDX_Control(pDX, IDC_CAL_VIDEO_SATURATION_SLIDER, m_SaturationSlider);

	DDX_Control(pDX, IDC_PLACIDO_CHK, m_PlacdioCheck);//test cjf 10292020
	
}

//***************************************************************************************

BOOL CVideo2ToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_w = CHW::m_VideoWidth;
	m_h = CHW::m_VideoHeight;
	m_w_um = m_pHW->m_Calibration.VideoWidthMicrons;
	m_h_um = m_pHW->m_Calibration.VideoHeightMicrons;
	m_x_px_um = m_w / m_w_um;
	m_y_px_um = m_h / m_h_um;
	m_cx = 0.5 * m_w;
	m_cy = 0.5 * m_h - 1;

	VIDEO_SETTINGS* pVideoSettings = m_pHW->m_pCurrentVideoSettings;

	m_BackupInfraredLEDsPowerLevel = pVideoSettings->InfraredLEDsPowerLevel;
	m_BackupWhiteLEDsPowerLevel = pVideoSettings->WhiteLEDsPowerLevel;
	m_BackupBrightness = pVideoSettings->Brightness;
	m_BackupContrast = pVideoSettings->Contrast;
	m_BackupHue = pVideoSettings->Hue;
	m_BackupSaturation = pVideoSettings->Saturation;

	m_BackupDelay = m_pHW->m_Calibration.ColorImageDelay;

	m_InfraredLEDsPowerLevelSlider.SetRange(0, 100, FALSE);
	m_WhiteLEDsPowerLevelSlider.SetRange(0, 100, FALSE);
	m_BrightnessSlider.SetRange(0, 255, FALSE);
	m_ContrastSlider.SetRange(0, 255, FALSE);
	m_HueSlider.SetRange(0, 255, FALSE);
	m_SaturationSlider.SetRange(0, 255, FALSE);

	m_InfraredLEDsPowerLevelSlider.SetPos(pVideoSettings->InfraredLEDsPowerLevel);
	m_WhiteLEDsPowerLevelSlider.SetPos(pVideoSettings->WhiteLEDsPowerLevel);
	m_BrightnessSlider.SetPos(pVideoSettings->Brightness);
	m_ContrastSlider.SetPos(pVideoSettings->Contrast);
	m_HueSlider.SetPos(pVideoSettings->Hue);
	m_SaturationSlider.SetPos(pVideoSettings->Saturation);

	CString s;

	s.Format(_T("%i"), pVideoSettings->InfraredLEDsPowerLevel);
	m_InfraredLEDsPowerLevelEdit.SetWindowText(s);

	s.Format(_T("%i"), pVideoSettings->WhiteLEDsPowerLevel);
	m_WhiteLEDsPowerLevelEdit.SetWindowText(s);

	s.Format(_T("%i"), pVideoSettings->Brightness);
	m_BrightnessEdit.SetWindowText(s);

	s.Format(_T("%i"), pVideoSettings->Contrast);
	m_ContrastEdit.SetWindowText(s);

	s.Format(_T("%i"), pVideoSettings->Hue);
	m_HueEdit.SetWindowText(s);

	s.Format(_T("%i"), pVideoSettings->Saturation);
	m_SaturationEdit.SetWindowText(s);

	s.Format(_T("%i"), m_pHW->m_Calibration.ColorImageDelay);
	m_DelayEdit.SetWindowText(s);

	m_PlacdioCheck.SetCheck(FALSE);//test cjf 10292020

	RECT Rect = { 10, 10, 10 + CHW::m_VideoWidth, 10 + CHW::m_VideoHeight };
	m_VideoWnd.CreateWnd(Rect, this);

	return TRUE;
}

//***************************************************************************************

void CVideo2ToolDlg::OnCancel()
{
	if (!m_CancelButton.IsWindowEnabled()) return;

	VIDEO_SETTINGS* pVideoSettings = m_pHW->m_pCurrentVideoSettings;

	pVideoSettings->InfraredLEDsPowerLevel = m_BackupInfraredLEDsPowerLevel;
	pVideoSettings->WhiteLEDsPowerLevel = m_BackupWhiteLEDsPowerLevel;
	pVideoSettings->Brightness = m_BackupBrightness;
	pVideoSettings->Contrast = m_BackupContrast;
	pVideoSettings->Hue = m_BackupHue;
	pVideoSettings->Saturation = m_BackupSaturation;

	m_pHW->m_Calibration.ColorImageDelay = m_BackupDelay;

	CDialog::OnCancel();
}

//***************************************************************************************

void CVideo2ToolDlg::OnOK()
{
	CString s;
	m_DelayEdit.GetWindowText(s);
	G_As = s;
	m_pHW->m_Calibration.ColorImageDelay = atoi(G_As);

	CDialog::OnOK();
}

//***************************************************************************************

void CVideo2ToolDlg::OnHScroll(uint nSBCode, uint nPos, CScrollBar* pScrollBar)
{
	VIDEO_SETTINGS* pVideoSettings = m_pHW->m_pCurrentVideoSettings;
	CString s;

	switch (pScrollBar->GetDlgCtrlID()) {
	case IDC_CAL_VIDEO_INFRARED_LEDS_POWER_LEVEL_SLIDER:
		pVideoSettings->InfraredLEDsPowerLevel = m_InfraredLEDsPowerLevelSlider.GetPos();
		s.Format(_T("%i"), pVideoSettings->InfraredLEDsPowerLevel);
		m_InfraredLEDsPowerLevelEdit.SetWindowText(s);
		m_pLiveVideo->ChangeInfraredLEDsPowerLevel();
		break;
	case IDC_CAL_VIDEO_WHITE_LEDS_POWER_LEVEL_SLIDER:
		pVideoSettings->WhiteLEDsPowerLevel = m_WhiteLEDsPowerLevelSlider.GetPos();
		s.Format(_T("%i"), pVideoSettings->WhiteLEDsPowerLevel);
		m_WhiteLEDsPowerLevelEdit.SetWindowText(s);
		m_pLiveVideo->ChangeWhiteLEDsPowerLevel();
		break;
	case IDC_CAL_VIDEO_BRIGHTNESS_SLIDER:
		pVideoSettings->Brightness = m_BrightnessSlider.GetPos();
		s.Format(_T("%i"), pVideoSettings->Brightness);
		m_BrightnessEdit.SetWindowText(s);
		m_pLiveVideo->ChangeBrightness();
		break;
	case IDC_CAL_VIDEO_CONTRAST_SLIDER:
		pVideoSettings->Contrast = m_ContrastSlider.GetPos();
		s.Format(_T("%i"), pVideoSettings->Contrast);
		m_ContrastEdit.SetWindowText(s);
		m_pLiveVideo->ChangeContrast();
		break;
	case IDC_CAL_VIDEO_HUE_SLIDER:
		pVideoSettings->Hue = m_HueSlider.GetPos();
		s.Format(_T("%i"), pVideoSettings->Hue);
		m_HueEdit.SetWindowText(s);
		m_pLiveVideo->ChangeHue();
		break;
	case IDC_CAL_VIDEO_SATURATION_SLIDER:
		pVideoSettings->Saturation = m_SaturationSlider.GetPos();
		s.Format(_T("%i"), pVideoSettings->Saturation);
		m_SaturationEdit.SetWindowText(s);
		m_pLiveVideo->ChangeSaturation();
		break;
	}
}

//***************************************************************************************

void CVideo2ToolDlg::OnStartVideoButtonClicked()
{
	if (!m_pLiveVideo->Start()) return;

	m_OKButton.EnableWindow(FALSE);
	m_CancelButton.EnableWindow(FALSE);
	m_StartVideoButton.EnableWindow(FALSE);
	m_AbortVideoButton.EnableWindow(TRUE);
	m_StartButton.EnableWindow(FALSE);
}

//***************************************************************************************

void CVideo2ToolDlg::OnAbortVideoButtonClicked()
{
	m_pLiveVideo->Abort();
}

//***************************************************************************************

LRESULT CVideo2ToolDlg::OnFinish(WPARAM wParam, LPARAM lParam)
{
	m_pLiveVideo->Finish();

	m_OKButton.EnableWindow(TRUE);
	m_CancelButton.EnableWindow(TRUE);
	m_StartVideoButton.EnableWindow(TRUE);
	m_AbortVideoButton.EnableWindow(FALSE);
	m_StartButton.EnableWindow(TRUE);

	return 0;
}

//***************************************************************************************

LRESULT CVideo2ToolDlg::OnUpdate(WPARAM wParam, LPARAM lParam)
{
	CString s;
	RECT Rect;
	int fs = 16;
	CMFont Font(fs, 600, "Arial");

	uchar* pRGBData = m_pHW->GetRGBData();
	memcpy(m_VideoWnd.m_MemDC.m_RGBData, pRGBData, CHW::m_VideoSize);

	::SetRect(&Rect, 0, m_h - fs - fs, m_w - 5, m_h - fs);
	s.Format(_T("%u"), m_pLiveVideo->m_NumFramesReceived);
	m_VideoWnd.m_MemDC.WriteText(s, Rect, Font, YELLOW, 2);

	::SetRect(&Rect, 0, m_h - fs, m_w - 5, m_h);
	s.Format(_T("%.1f ms"), m_pLiveVideo->m_AverageCycleTime);
	m_VideoWnd.m_MemDC.WriteText(s, Rect, Font, YELLOW, 2);

	m_VideoWnd.Invalidate(FALSE);
	m_VideoWnd.UpdateWindow();

	return 0;
}

//***************************************************************************************

void CVideo2ToolDlg::OnStartButtonClicked()
{
	CBusyCursor Cursor;

	CString s;
	m_DelayEdit.GetWindowText(s);
	G_As = s;
	int Delay = atoi(G_As);

	m_pHW->ClearFrames();

	m_pHW->ApplyCurrentVideoSettings();
	m_pHW->TurnInfraredLEDsOn();
	m_pHW->TurnWhiteLEDsOn();
	//m_pHW->TurnVideoCameraOn();comment out by [cjf***032112]
	//[cjf***032112]  
	if (!::HW.ISVideoCameraOn)
	{
		m_pHW->TurnVideoCameraOn();
		::HW.ISVideoCameraOn = true;
	}
	//[cjf***032112]

	//BOOL on = FALSE;

	ulong StartTime = clock();
	ulong Time;
	int fs = 16;
	CMFont Font(fs, 600, "Arial");
	RECT Rect = { 0, m_h - fs, m_w - 5, m_h };
	while ((Time = (clock() - StartTime)) < (ulong)Delay/* + 2000*/) {

		/*
		if (Time > 2000 && !on) {
		m_pHW->TurnScanningLaserOn();
		m_pHW->TurnAODsOn();
		m_pHW->SetAODsFrequencies(0.0, 1000.0);
		on = TRUE;
		}
		*/

		m_pHW->StartTransferringVideoFrame();
		
		memcpy(m_VideoWnd.m_MemDC.m_RGBData, m_pHW->GetRGBData(), CHW::m_VideoSize);
		s.Format(_T("%i ms"), Time);
		m_VideoWnd.m_MemDC.WriteText(s, Rect, Font, YELLOW, 2);
		m_VideoWnd.Invalidate(FALSE);
		m_VideoWnd.UpdateWindow();

		m_pHW->FinishTransferringVideoFrame();
	}

	memcpy(m_VideoWnd.m_MemDC.m_RGBData, m_pHW->GetRGBData(), CHW::m_VideoSize);
	s.Format(_T("%i ms"), Time);
	m_VideoWnd.m_MemDC.WriteText(s, Rect, Font, YELLOW, 2);
	m_VideoWnd.Invalidate(FALSE);
	m_VideoWnd.UpdateWindow();

	/*
	m_pHW->TurnAODsOff();
	m_pHW->TurnScanningLaserOff();
	*/

	//m_pHW->TurnVideoCameraOff();//comment out by [cjf***032112]
	m_pHW->TurnInfraredLEDsOff();
	m_pHW->TurnWhiteLEDsOff();
}

//***************************************************************************************

// test cjf 10292020
void CVideo2ToolDlg::OnPlacdioCheckClicked()
{
  BOOL Checked = m_PlacdioCheck.GetCheck();

  if (Checked) {
    m_pHW->TurnPlacidoOn();
  }
  else {
    m_pHW->TurnPlacidoOff();
  }
}

//***************************************************************************************


