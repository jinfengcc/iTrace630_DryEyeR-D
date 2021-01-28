//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "Video3ToolDlg.h"
#include "BusyCursor.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CVideo3ToolDlg, CDialog)

	ON_WM_HSCROLL()

	ON_MESSAGE(WM_THREAD_UPDATE, OnUpdate)
	ON_MESSAGE(WM_THREAD_FINISH, OnFinish)

	ON_BN_CLICKED(IDC_VERY_LIGHT_EYE_RADIO, OnVeryLightEyeRadioClicked)
	ON_BN_CLICKED(IDC_LIGHT_EYE_RADIO, OnLightEyeRadioClicked)
	ON_BN_CLICKED(IDC_MEDIUM_EYE_RADIO, OnMediumEyeRadioClicked)
	ON_BN_CLICKED(IDC_DARK_EYE_RADIO, OnDarkEyeRadioClicked)
	ON_BN_CLICKED(IDC_VERY_DARK_EYE_RADIO, OnVeryDarkEyeRadioClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CVideo3ToolDlg::CVideo3ToolDlg(CHW* pHW, CWnd* pParentWnd) :
	CDialog(IDD_VIDEO3_TOOL_DLG, pParentWnd)
{
	m_pHW = pHW;
	m_pHW->m_pCurrentVideoSettings = &m_pHW->m_Calibration.WFVideo2Settings;
	m_pLiveVideo = new CLiveVideo(m_pHW);
	m_pLiveVideo->m_pVideoWnd = this;
}

//***************************************************************************************

CVideo3ToolDlg::~CVideo3ToolDlg()
{
	delete m_pLiveVideo;
}

//***************************************************************************************

void CVideo3ToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDOK, m_OKButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);

	DDX_Control(pDX, IDC_CAL_VIDEO_WHITE_LEDS_POWER_LEVEL_EDIT, m_WhiteLEDsPowerLevelEdit);
	DDX_Control(pDX, IDC_CAL_VIDEO_BRIGHTNESS_EDIT, m_BrightnessEdit);
	DDX_Control(pDX, IDC_CAL_VIDEO_CONTRAST_EDIT, m_ContrastEdit);
	DDX_Control(pDX, IDC_CAL_VIDEO_HUE_EDIT, m_HueEdit);
	DDX_Control(pDX, IDC_CAL_VIDEO_SATURATION_EDIT, m_SaturationEdit);

	DDX_Control(pDX, IDC_CAL_VIDEO_WHITE_LEDS_POWER_LEVEL_SLIDER, m_WhiteLEDsPowerLevelSlider);
	DDX_Control(pDX, IDC_CAL_VIDEO_BRIGHTNESS_SLIDER, m_BrightnessSlider);
	DDX_Control(pDX, IDC_CAL_VIDEO_CONTRAST_SLIDER, m_ContrastSlider);
	DDX_Control(pDX, IDC_CAL_VIDEO_HUE_SLIDER, m_HueSlider);
	DDX_Control(pDX, IDC_CAL_VIDEO_SATURATION_SLIDER, m_SaturationSlider);


	DDX_Control(pDX, IDC_VERY_LIGHT_EYE_RADIO, m_VeryLightEyeRad);
	DDX_Control(pDX, IDC_LIGHT_EYE_RADIO, m_LightEyeRad);
	DDX_Control(pDX, IDC_MEDIUM_EYE_RADIO, m_MediumEyeRad);
	DDX_Control(pDX, IDC_DARK_EYE_RADIO, m_DarkEyeRad);
	DDX_Control(pDX, IDC_VERY_DARK_EYE_RADIO, m_VeryDarkEyeRad);
}

//***************************************************************************************

BOOL CVideo3ToolDlg::OnInitDialog()
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

	m_BackupWhiteLEDsPowerLevel = pVideoSettings->WhiteLEDsPowerLevel;
	m_BackupBrightness = pVideoSettings->Brightness;
	m_BackupContrast = pVideoSettings->Contrast;
	m_BackupHue = pVideoSettings->Hue;
	m_BackupSaturation = pVideoSettings->Saturation;

	m_WhiteLEDsPowerLevelSlider.SetRange(0, 100, FALSE);
	m_BrightnessSlider.SetRange(0, 255, FALSE);
	m_ContrastSlider.SetRange(0, 255, FALSE);
	m_HueSlider.SetRange(0, 255, FALSE);
	m_SaturationSlider.SetRange(0, 255, FALSE);

	m_WhiteLEDsPowerLevelSlider.SetPos(pVideoSettings->WhiteLEDsPowerLevel);
	m_BrightnessSlider.SetPos(pVideoSettings->Brightness);
	m_ContrastSlider.SetPos(pVideoSettings->Contrast);
	m_HueSlider.SetPos(pVideoSettings->Hue);
	m_SaturationSlider.SetPos(pVideoSettings->Saturation);

	CString s;

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

	m_MediumEyeRad.SetCheck(TRUE);

	RECT Rect = { 10, 10, 10 + CHW::m_VideoWidth, 10 + CHW::m_VideoHeight };
	m_VideoWnd.CreateWnd(Rect, this);

	if (!m_pLiveVideo->Start())  return FALSE;

	return TRUE;
}

//***************************************************************************************

void CVideo3ToolDlg::OnCancel()
{
	m_pLiveVideo->Abort();

	VIDEO_SETTINGS* pVideoSettings = m_pHW->m_pCurrentVideoSettings;

	pVideoSettings->WhiteLEDsPowerLevel = m_BackupWhiteLEDsPowerLevel;
	pVideoSettings->Brightness = m_BackupBrightness;
	pVideoSettings->Contrast = m_BackupContrast;
	pVideoSettings->Hue = m_BackupHue;
	pVideoSettings->Saturation = m_BackupSaturation;
}

//***************************************************************************************

void CVideo3ToolDlg::OnOK()
{
	m_pLiveVideo->Abort();
	//CDialog::OnOK();
}

//***************************************************************************************

void CVideo3ToolDlg::OnHScroll(uint nSBCode, uint nPos, CScrollBar* pScrollBar)
{
	VIDEO_SETTINGS* pVideoSettings = m_pHW->m_pCurrentVideoSettings;
	CString s;

	switch (pScrollBar->GetDlgCtrlID())
	{
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

LRESULT CVideo3ToolDlg::OnUpdate(WPARAM wParam, LPARAM lParam)
{
	uchar* pRGBData = m_pHW->GetRGBData();
	memcpy(m_VideoWnd.m_MemDC.m_RGBData, pRGBData, CHW::m_VideoSize);

	m_VideoWnd.Invalidate(FALSE);
	m_VideoWnd.UpdateWindow();

	return 0;
}

//***************************************************************************************

LRESULT CVideo3ToolDlg::OnFinish(WPARAM wParam, LPARAM lParam)
{
	m_pLiveVideo->Finish();
	CDialog::OnOK();

	return 0;
}

//***************************************************************************************

void CVideo3ToolDlg::OnVeryLightEyeRadioClicked()
{
	VIDEO_SETTINGS* pVideoSettings = m_pHW->m_pCurrentVideoSettings;

	pVideoSettings->WhiteLEDsPowerLevel = 9;
	pVideoSettings->Brightness = 106;
	pVideoSettings->Contrast = 247;

	m_WhiteLEDsPowerLevelSlider.SetRange(0, 100, FALSE);
	m_BrightnessSlider.SetRange(0, 255, FALSE);
	m_ContrastSlider.SetRange(0, 255, FALSE);
	m_HueSlider.SetRange(0, 255, FALSE);
	m_SaturationSlider.SetRange(0, 255, FALSE);

	m_WhiteLEDsPowerLevelSlider.SetPos(pVideoSettings->WhiteLEDsPowerLevel);
	m_BrightnessSlider.SetPos(pVideoSettings->Brightness);
	m_ContrastSlider.SetPos(pVideoSettings->Contrast);
	m_HueSlider.SetPos(pVideoSettings->Hue);
	m_SaturationSlider.SetPos(pVideoSettings->Saturation);

	CString s;

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
}


void CVideo3ToolDlg::OnLightEyeRadioClicked()
{
	VIDEO_SETTINGS* pVideoSettings = m_pHW->m_pCurrentVideoSettings;

	pVideoSettings->WhiteLEDsPowerLevel = 11;
	pVideoSettings->Brightness = 53;
	pVideoSettings->Contrast = 245;

	m_WhiteLEDsPowerLevelSlider.SetRange(0, 100, FALSE);
	m_BrightnessSlider.SetRange(0, 255, FALSE);
	m_ContrastSlider.SetRange(0, 255, FALSE);
	m_HueSlider.SetRange(0, 255, FALSE);
	m_SaturationSlider.SetRange(0, 255, FALSE);

	m_WhiteLEDsPowerLevelSlider.SetPos(pVideoSettings->WhiteLEDsPowerLevel);
	m_BrightnessSlider.SetPos(pVideoSettings->Brightness);
	m_ContrastSlider.SetPos(pVideoSettings->Contrast);
	m_HueSlider.SetPos(pVideoSettings->Hue);
	m_SaturationSlider.SetPos(pVideoSettings->Saturation);

	CString s;

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
}

//*****************************************************************************************

void CVideo3ToolDlg::OnMediumEyeRadioClicked()
{
	VIDEO_SETTINGS* pVideoSettings = m_pHW->m_pCurrentVideoSettings;

	pVideoSettings->WhiteLEDsPowerLevel = m_BackupWhiteLEDsPowerLevel;
	pVideoSettings->Brightness = m_BackupBrightness;
	pVideoSettings->Contrast = m_BackupContrast;
	pVideoSettings->Hue = m_BackupHue;
	pVideoSettings->Saturation = m_BackupSaturation;

	m_WhiteLEDsPowerLevelSlider.SetRange(0, 100, FALSE);
	m_BrightnessSlider.SetRange(0, 255, FALSE);
	m_ContrastSlider.SetRange(0, 255, FALSE);
	m_HueSlider.SetRange(0, 255, FALSE);
	m_SaturationSlider.SetRange(0, 255, FALSE);

	m_WhiteLEDsPowerLevelSlider.SetPos(pVideoSettings->WhiteLEDsPowerLevel);
	m_BrightnessSlider.SetPos(pVideoSettings->Brightness);
	m_ContrastSlider.SetPos(pVideoSettings->Contrast);
	m_HueSlider.SetPos(pVideoSettings->Hue);
	m_SaturationSlider.SetPos(pVideoSettings->Saturation);

	CString s;

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
}


//***************************************************************************************

void CVideo3ToolDlg::OnDarkEyeRadioClicked()
{
	VIDEO_SETTINGS* pVideoSettings = m_pHW->m_pCurrentVideoSettings;

	pVideoSettings->WhiteLEDsPowerLevel = 10;
	pVideoSettings->Brightness = 96;
	pVideoSettings->Contrast = 255;

	m_WhiteLEDsPowerLevelSlider.SetRange(0, 100, FALSE);
	m_BrightnessSlider.SetRange(0, 255, FALSE);
	m_ContrastSlider.SetRange(0, 255, FALSE);
	m_HueSlider.SetRange(0, 255, FALSE);
	m_SaturationSlider.SetRange(0, 255, FALSE);

	m_WhiteLEDsPowerLevelSlider.SetPos(pVideoSettings->WhiteLEDsPowerLevel);
	m_BrightnessSlider.SetPos(pVideoSettings->Brightness);
	m_ContrastSlider.SetPos(pVideoSettings->Contrast);
	m_HueSlider.SetPos(pVideoSettings->Hue);
	m_SaturationSlider.SetPos(pVideoSettings->Saturation);

	CString s;

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
}

//***************************************************************************************

void CVideo3ToolDlg::OnVeryDarkEyeRadioClicked()
{
	VIDEO_SETTINGS* pVideoSettings = m_pHW->m_pCurrentVideoSettings;

	pVideoSettings->WhiteLEDsPowerLevel = 9;
	pVideoSettings->Brightness = 126;
	pVideoSettings->Contrast = 255;

	m_WhiteLEDsPowerLevelSlider.SetRange(0, 100, FALSE);
	m_BrightnessSlider.SetRange(0, 255, FALSE);
	m_ContrastSlider.SetRange(0, 255, FALSE);
	m_HueSlider.SetRange(0, 255, FALSE);
	m_SaturationSlider.SetRange(0, 255, FALSE);

	m_WhiteLEDsPowerLevelSlider.SetPos(pVideoSettings->WhiteLEDsPowerLevel);
	m_BrightnessSlider.SetPos(pVideoSettings->Brightness);
	m_ContrastSlider.SetPos(pVideoSettings->Contrast);
	m_HueSlider.SetPos(pVideoSettings->Hue);
	m_SaturationSlider.SetPos(pVideoSettings->Saturation);

	CString s;

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
}