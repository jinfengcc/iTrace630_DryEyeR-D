//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "CalDlg.h"
#include "FirmwareToolDlg.h"
#include "AODToolDlg.h"
#include "VideoToolDlg.h"
#include "Video2ToolDlg.h"
#include "RingsToolDlg.h"
#include "BusyCursor.h"
#include "ChartDlg.h"
#include "FolderDlg.h"
#include "VideoMoveDlg.h"//[5.1.1]
#include "WFAcquisition.h"//520
#include "WFAcquisitionDlg.h"//520
#include "Settings.h"

#include "QuickWFDlg.h"//530

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CCalDlg, CDialog)

	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CAL_FIRMWARE_TOOL, OnFirmwareToolButtonClicked)
	ON_BN_CLICKED(IDC_CAL_LOAD, OnLoadButtonClicked)
	ON_BN_CLICKED(IDC_CAL_SAVE, OnSaveButtonClicked)
	ON_BN_CLICKED(IDC_CAL_RESTORE, OnRestoreButtonClicked)
	ON_BN_CLICKED(IDC_CAL_BACKUP, OnBackupButtonClicked)
	ON_BN_CLICKED(IDC_CAL_AOD_TOOL, OnAODToolButtonClicked)
	ON_BN_CLICKED(IDC_CAL_VIDEO_TOOL, OnVideoToolButtonClicked)
	ON_BN_CLICKED(IDC_CAL_VIDEO2_TOOL, OnVideo2ToolButtonClicked)
	ON_BN_CLICKED(IDC_CAL_RINGS_TOOL, OnRingsToolButtonClicked)
	ON_BN_CLICKED(IDC_CAL_BALL1_RADIO, OnBallRadioClicked)
	ON_BN_CLICKED(IDC_CAL_BALL2_RADIO, OnBallRadioClicked)
	ON_BN_CLICKED(IDC_CAL_BALL3_RADIO, OnBallRadioClicked)
	ON_BN_CLICKED(IDC_CAL_BALL4_RADIO, OnBallRadioClicked)
	ON_BN_CLICKED(IDC_CAL_OPT_TABLE, OnOptTableButtonClicked)
	ON_BN_CLICKED(IDC_CAL_VIDEO_MOVE, OnVideoMoveButtonClicked)//[5.1.1]
	ON_BN_CLICKED(IDC_CAL_QUICK_WFEXAM, OnVideoQkWFExamBtnClicked)//520 for a quick wf exam trigger


END_MESSAGE_MAP()

//***************************************************************************************

CCalDlg::CCalDlg(CHW* pHW, CALIBRATION* pCalibration, CWnd* pParentWnd) : CDialog(IDD_CAL_DLG, pParentWnd)
{
	m_pHW = pHW;
	m_pCalibration = pCalibration;
}

//***************************************************************************************

void CCalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDOK, m_OKButton);

	DDX_Control(pDX, IDC_CAL_LOAD, m_LoadButton);
	DDX_Control(pDX, IDC_CAL_SAVE, m_SaveButton);
	DDX_Control(pDX, IDC_CAL_RESTORE, m_RestoreButton);
	DDX_Control(pDX, IDC_CAL_BACKUP, m_BackupButton);

	DDX_Control(pDX, IDC_CAL_GUID_EDIT, m_GUIDEdit);
	DDX_Control(pDX, IDC_CAL_SERIAL_EDIT, m_SerialEdit);
	DDX_Control(pDX, IDC_CAL_DATE_EDIT, m_DateEdit);
	DDX_Control(pDX, IDC_CAL_TIME_EDIT, m_TimeEdit);
	DDX_Control(pDX, IDC_CAL_FIRMWARE_TOOL, m_FirmwareToolButton);

	DDX_Control(pDX, IDC_CAL_WAVELENGTH_EDIT, m_WavelengthEdit);

	DDX_Control(pDX, IDC_CAL_NUM_POINTS_EDIT, m_NumPointsEdit);

	DDX_Control(pDX, IDC_CAL_FLIP_AODX_CHECK, m_FlipAODXCheck);
	DDX_Control(pDX, IDC_CAL_AODX_FREQ0_EDIT, m_AODXFreq0Edit);
	DDX_Control(pDX, IDC_CAL_AODX_HZ_UM_EDIT, m_AODXHzUmEdit);
	DDX_Control(pDX, IDC_CAL_AODX_RMS_EDIT, m_AODXRMSEdit);
	DDX_Control(pDX, IDC_CAL_FLIP_AODY_CHECK, m_FlipAODYCheck);
	DDX_Control(pDX, IDC_CAL_AODY_FREQ0_EDIT, m_AODYFreq0Edit);
	DDX_Control(pDX, IDC_CAL_AODY_HZ_UM_EDIT, m_AODYHzUmEdit);
	DDX_Control(pDX, IDC_CAL_AODY_RMS_EDIT, m_AODYRMSEdit);
	DDX_Control(pDX, IDC_CAL_AOD_TOOL, m_AODToolButton);

	DDX_Control(pDX, IDC_CAL_EXP_TIME_EDIT, m_LADExpTimeEdit);
	DDX_Control(pDX, IDC_CAL_SWAP_LADS_CHECK, m_SwapLADsCheck);
	DDX_Control(pDX, IDC_CAL_FLIP_LADX_CHECK, m_FlipLADXCheck);
	DDX_Control(pDX, IDC_CAL_F_X_EDIT, m_LADXFocalDistanceEdit);
	DDX_Control(pDX, IDC_CAL_FLIP_LADY_CHECK, m_FlipLADYCheck);
	DDX_Control(pDX, IDC_CAL_F_Y_EDIT, m_LADYFocalDistanceEdit);

	DDX_Control(pDX, IDC_CAL_VIDEO_FLIPX_CHECK, m_VideoFlipXCheck);
	DDX_Control(pDX, IDC_CAL_VIDEO_FLIPY_CHECK, m_VideoFlipYCheck);
	DDX_Control(pDX, IDC_CAL_VIDEO_WIDTH_EDIT, m_VideoWidthEdit);
	DDX_Control(pDX, IDC_CAL_VIDEO_HEIGHT_EDIT, m_VideoHeightEdit);
	DDX_Control(pDX, IDC_CAL_VIDEO_WIDTH_UM_EDIT, m_VideoWidthUmEdit);
	DDX_Control(pDX, IDC_CAL_VIDEO_HEIGHT_UM_EDIT, m_VideoHeightUmEdit);
	DDX_Control(pDX, IDC_CAL_WFVIDEO_INFRARED_LEDS_POWER_LEVEL_EDIT, m_WFVideoInfraredLEDsPowerLevelEdit);
	DDX_Control(pDX, IDC_CAL_WFVIDEO_BRIGHTNESS_EDIT, m_WFVideoBrightnessEdit);
	DDX_Control(pDX, IDC_CAL_WFVIDEO_CONTRAST_EDIT, m_WFVideoContrastEdit);
	DDX_Control(pDX, IDC_CAL_WFVIDEO2_INFRARED_LEDS_POWER_LEVEL_EDIT, m_WFVideo2InfraredLEDsPowerLevelEdit);
	DDX_Control(pDX, IDC_CAL_WFVIDEO2_WHITE_LEDS_POWER_LEVEL_EDIT, m_WFVideo2WhiteLEDsPowerLevelEdit);
	DDX_Control(pDX, IDC_CAL_WFVIDEO2_BRIGHTNESS_EDIT, m_WFVideo2BrightnessEdit);
	DDX_Control(pDX, IDC_CAL_WFVIDEO2_CONTRAST_EDIT, m_WFVideo2ContrastEdit);
	DDX_Control(pDX, IDC_CAL_WFVIDEO2_HUE_EDIT, m_WFVideo2HueEdit);
	DDX_Control(pDX, IDC_CAL_WFVIDEO2_SATURATION_EDIT, m_WFVideo2SaturationEdit);
	DDX_Control(pDX, IDC_CAL_WFVIDEO2_DELAY_EDIT, m_WFVideo2DelayEdit);
	DDX_Control(pDX, IDC_CAL_CTVIDEO_BRIGHTNESS_EDIT, m_CTVideoBrightnessEdit);
	DDX_Control(pDX, IDC_CAL_CTVIDEO_CONTRAST_EDIT, m_CTVideoContrastEdit);
	DDX_Control(pDX, IDC_CAL_VIDEO_TOOL, m_VideoToolButton);
	DDX_Control(pDX, IDC_CAL_VIDEO2_TOOL, m_Video2ToolButton);

	DDX_Control(pDX, IDC_CAL_ZALIGN_METHOD_1_RADIO, m_ZAlignmentMethod1Radio);
	DDX_Control(pDX, IDC_CAL_ZALIGN_METHOD_2_RADIO, m_ZAlignmentMethod2Radio);
	DDX_Control(pDX, IDC_CAL_LASER_THRESHOLD_EDIT, m_LaserThresholdEdit);
	DDX_Control(pDX, IDC_CAL_SENSOR_THRESHOLD_EDIT, m_SensorThresholdEdit);
	DDX_Control(pDX, IDC_CAL_LASER_DURATION_EDIT, m_LaserDurationEdit);

	DDX_Control(pDX, IDC_CAL_C3_EDIT, m_C3Edit);
	DDX_Control(pDX, IDC_CAL_C4_EDIT, m_C4Edit);
	DDX_Control(pDX, IDC_CAL_C5_EDIT, m_C5Edit);

	DDX_Control(pDX, IDC_CAL_ACCOMM_TARGET_POWER_LEVEL_EDIT, m_AccommodationTargetPowerLevelEdit);
	DDX_Control(pDX, IDC_CAL_FAR_TARGET_POWER_LEVEL_EDIT, m_FarTargetPowerLevelEdit);
	DDX_Control(pDX, IDC_CAL_NEAR_TARGET_POWER_LEVEL_EDIT, m_NearTargetPowerLevelEdit);

	DDX_Control(pDX, IDC_CAL_BALL1_RADIO, m_Ball1Radio);
	DDX_Control(pDX, IDC_CAL_BALL2_RADIO, m_Ball2Radio);
	DDX_Control(pDX, IDC_CAL_BALL3_RADIO, m_Ball3Radio);
	DDX_Control(pDX, IDC_CAL_BALL4_RADIO, m_Ball4Radio);
	DDX_Control(pDX, IDC_CAL_BALL1_EDIT, m_Ball1Edit);
	DDX_Control(pDX, IDC_CAL_BALL2_EDIT, m_Ball2Edit);
	DDX_Control(pDX, IDC_CAL_BALL3_EDIT, m_Ball3Edit);
	DDX_Control(pDX, IDC_CAL_BALL4_EDIT, m_Ball4Edit);
	DDX_Control(pDX, IDC_CAL_NUM_RINGS_EDIT, m_NumRingsEdit);
	DDX_Control(pDX, IDC_CAL_RINGS_TOOL, m_RingsToolButton);

	DDX_Control(pDX, IDC_CAL_OPT_M10_EDIT, m_OptM10Edit);
	DDX_Control(pDX, IDC_CAL_OPT_M09_EDIT, m_OptM09Edit);
	DDX_Control(pDX, IDC_CAL_OPT_M08_EDIT, m_OptM08Edit);
	DDX_Control(pDX, IDC_CAL_OPT_M07_EDIT, m_OptM07Edit);
	DDX_Control(pDX, IDC_CAL_OPT_M06_EDIT, m_OptM06Edit);
	DDX_Control(pDX, IDC_CAL_OPT_M05_EDIT, m_OptM05Edit);
	DDX_Control(pDX, IDC_CAL_OPT_M04_EDIT, m_OptM04Edit);
	DDX_Control(pDX, IDC_CAL_OPT_M03_EDIT, m_OptM03Edit);
	DDX_Control(pDX, IDC_CAL_OPT_M02_EDIT, m_OptM02Edit);
	DDX_Control(pDX, IDC_CAL_OPT_M01_EDIT, m_OptM01Edit);
	DDX_Control(pDX, IDC_CAL_OPT_N00_EDIT, m_OptN00Edit);
	DDX_Control(pDX, IDC_CAL_OPT_P01_EDIT, m_OptP01Edit);
	DDX_Control(pDX, IDC_CAL_OPT_P02_EDIT, m_OptP02Edit);
	DDX_Control(pDX, IDC_CAL_OPT_P03_EDIT, m_OptP03Edit);
	DDX_Control(pDX, IDC_CAL_OPT_P04_EDIT, m_OptP04Edit);
	DDX_Control(pDX, IDC_CAL_OPT_P05_EDIT, m_OptP05Edit);
	DDX_Control(pDX, IDC_CAL_OPT_P06_EDIT, m_OptP06Edit);
	DDX_Control(pDX, IDC_CAL_OPT_P07_EDIT, m_OptP07Edit);
	DDX_Control(pDX, IDC_CAL_OPT_P08_EDIT, m_OptP08Edit);
	DDX_Control(pDX, IDC_CAL_OPT_P09_EDIT, m_OptP09Edit);
	DDX_Control(pDX, IDC_CAL_OPT_P10_EDIT, m_OptP10Edit);
	DDX_Control(pDX, IDC_CAL_OPT_GAP_EDIT, m_OptGapEdit);
	DDX_Control(pDX, IDC_CAL_OPT_ENABLED_CHECK, m_OptEnabledCheck);
	DDX_Control(pDX, IDC_CAL_OPT_TABLE, m_OptTableButton);

	DDX_Control(pDX, IDC_CAL_NUM_CONSEC_FRAMES_EDIT, m_NumConsecFramesEdit);
	DDX_Control(pDX, IDC_CAL_ALLOWED_DECENTRATION_EDIT, m_AllowedDecentErrorEdit);
	DDX_Control(pDX, IDC_CAL_ALLOWED_MOVEMENT_EDIT, m_AllowedMovementEdit);
	DDX_Control(pDX, IDC_CAL_VIDEO_MOVE, m_VideoMovementButton);//[5.1.1]
	DDX_Control(pDX, IDC_CAL_QUICK_WFEXAM, m_VideoQkWFExamButton);//520    
}

//***************************************************************************************

BOOL CCalDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ctrl + w
	if (m_pCalibration == NULL) {
		m_pCalibration = &m_pHW->m_Calibration;
	}
	// exam
	else {
		m_LoadButton.EnableWindow(FALSE);
		m_SaveButton.EnableWindow(FALSE);
		m_RestoreButton.EnableWindow(FALSE);
		m_BackupButton.EnableWindow(FALSE);
		m_OKButton.EnableWindow(FALSE);
		m_GUIDEdit.EnableWindow(FALSE);
		m_SerialEdit.EnableWindow(FALSE);
		m_DateEdit.EnableWindow(FALSE);
		m_TimeEdit.EnableWindow(FALSE);
		m_FirmwareToolButton.EnableWindow(FALSE);
		m_WavelengthEdit.EnableWindow(FALSE);
		m_NumPointsEdit.EnableWindow(FALSE);
		m_FlipAODXCheck.EnableWindow(FALSE);
		m_AODXFreq0Edit.EnableWindow(FALSE);
		m_AODXHzUmEdit.EnableWindow(FALSE);
		m_AODXRMSEdit.EnableWindow(FALSE);
		m_FlipAODYCheck.EnableWindow(FALSE);
		m_AODYFreq0Edit.EnableWindow(FALSE);
		m_AODYHzUmEdit.EnableWindow(FALSE);
		m_AODYRMSEdit.EnableWindow(FALSE);
		m_AODToolButton.EnableWindow(FALSE);
		m_LADExpTimeEdit.EnableWindow(FALSE);
		m_SwapLADsCheck.EnableWindow(FALSE);
		m_FlipLADXCheck.EnableWindow(FALSE);
		m_LADXFocalDistanceEdit.EnableWindow(FALSE);
		m_FlipLADYCheck.EnableWindow(FALSE);
		m_LADYFocalDistanceEdit.EnableWindow(FALSE);
		m_VideoFlipXCheck.EnableWindow(FALSE);
		m_VideoFlipYCheck.EnableWindow(FALSE);
		m_VideoWidthEdit.EnableWindow(FALSE);
		m_VideoHeightEdit.EnableWindow(FALSE);
		m_VideoWidthUmEdit.EnableWindow(FALSE);
		m_VideoHeightUmEdit.EnableWindow(FALSE);
		m_WFVideoInfraredLEDsPowerLevelEdit.EnableWindow(FALSE);
		m_WFVideoBrightnessEdit.EnableWindow(FALSE);
		m_WFVideoContrastEdit.EnableWindow(FALSE);
		m_WFVideo2InfraredLEDsPowerLevelEdit.EnableWindow(FALSE);
		m_WFVideo2WhiteLEDsPowerLevelEdit.EnableWindow(FALSE);
		m_WFVideo2BrightnessEdit.EnableWindow(FALSE);
		m_WFVideo2ContrastEdit.EnableWindow(FALSE);
		m_WFVideo2HueEdit.EnableWindow(FALSE);
		m_WFVideo2SaturationEdit.EnableWindow(FALSE);
		m_WFVideo2DelayEdit.EnableWindow(FALSE);
		m_CTVideoBrightnessEdit.EnableWindow(FALSE);
		m_CTVideoContrastEdit.EnableWindow(FALSE);
		m_VideoToolButton.EnableWindow(FALSE);
		m_Video2ToolButton.EnableWindow(FALSE);
		m_OptP10Edit.EnableWindow(FALSE);
		m_OptP09Edit.EnableWindow(FALSE);
		m_OptP08Edit.EnableWindow(FALSE);
		m_OptP07Edit.EnableWindow(FALSE);
		m_OptP06Edit.EnableWindow(FALSE);
		m_OptP05Edit.EnableWindow(FALSE);
		m_OptP04Edit.EnableWindow(FALSE);
		m_OptP03Edit.EnableWindow(FALSE);
		m_OptP02Edit.EnableWindow(FALSE);
		m_OptP01Edit.EnableWindow(FALSE);
		m_OptN00Edit.EnableWindow(FALSE);
		m_OptM01Edit.EnableWindow(FALSE);
		m_OptM02Edit.EnableWindow(FALSE);
		m_OptM03Edit.EnableWindow(FALSE);
		m_OptM04Edit.EnableWindow(FALSE);
		m_OptM05Edit.EnableWindow(FALSE);
		m_OptM06Edit.EnableWindow(FALSE);
		m_OptM07Edit.EnableWindow(FALSE);
		m_OptM08Edit.EnableWindow(FALSE);
		m_OptM09Edit.EnableWindow(FALSE);
		m_OptM10Edit.EnableWindow(FALSE);
		m_OptGapEdit.EnableWindow(FALSE);
		m_OptEnabledCheck.EnableWindow(FALSE);
		m_OptTableButton.EnableWindow(FALSE);
		m_C3Edit.EnableWindow(FALSE);
		m_C4Edit.EnableWindow(FALSE);
		m_C5Edit.EnableWindow(FALSE);
		m_AccommodationTargetPowerLevelEdit.EnableWindow(FALSE);
		m_FarTargetPowerLevelEdit.EnableWindow(FALSE);
		m_NearTargetPowerLevelEdit.EnableWindow(FALSE);
		m_ZAlignmentMethod1Radio.EnableWindow(FALSE);
		m_ZAlignmentMethod2Radio.EnableWindow(FALSE);
		m_LaserThresholdEdit.EnableWindow(FALSE);
		m_SensorThresholdEdit.EnableWindow(FALSE);
		m_LaserDurationEdit.EnableWindow(FALSE);
		m_Ball1Edit.EnableWindow(FALSE);
		m_Ball2Edit.EnableWindow(FALSE);
		m_Ball3Edit.EnableWindow(FALSE);
		m_Ball4Edit.EnableWindow(FALSE);
		m_NumRingsEdit.EnableWindow(FALSE);
		m_RingsToolButton.EnableWindow(FALSE);
		m_NumConsecFramesEdit.EnableWindow(FALSE);
		m_AllowedDecentErrorEdit.EnableWindow(FALSE);
		m_AllowedMovementEdit.EnableWindow(FALSE);
	}

	RECT Rect;
	GetClientRect(&Rect);
	int w = Rect.right - Rect.left;
	int h = Rect.bottom - Rect.top;
	m_l = intRound(0.600 * w);
	m_t = intRound(0.460 * h);
	m_w = intRound(0.360 * w);
	m_h = m_w;
	m_RingsDC.CreateTrueColorDC(m_w, m_h);

	m_Ball1Radio.SetCheck(1);

	SetDlgData();

	m_OKButton.SetFocus();

	return FALSE;
}

//***************************************************************************************

void CCalDlg::OnBallRadioClicked()
{
	m_RingsDC.FillSolidRect(0, 0, m_w, m_h, BLACK);

	real h_um = 12000.0;
	real w_um = h_um * m_w / m_h;
	real x_px_um = m_w / w_um;
	real y_px_um = m_h / h_um;
	real cx = m_w * 0.5;
	real cy = m_h * 0.5;

	real m = 1000.0 * ((int)(0.5 * __max(w_um, h_um) * 0.001) + 1.0);
	for (real um = -m; um < m + 0.1; um += 1000.0) {
		int y = intRound(cy - um * y_px_um);
		m_RingsDC.DrawHorzDottedLine(0, m_w - 1, y, 0x00606060, NOCOLOR);
		int x = intRound(cx + um * x_px_um);
		m_RingsDC.DrawVertDottedLine(x, 0, m_h - 1, 0x00606060, NOCOLOR);
	}

	int b = GetSelectedBall();

	for (int r = 0; r < m_pCalibration->NumRings; r++) {
		for (int a1 = 0; a1 < 360; a1++) {
			int a2 = CheckAngle(a1 + 1);
			real ri1_um = m_pCalibration->BallsRi[b][r][a1]; if (ri1_um == INVALID_VALUE) continue;
			real ri2_um = m_pCalibration->BallsRi[b][r][a2]; if (ri2_um == INVALID_VALUE) continue;
			int ya1 = intRound(cy - ri1_um * SIN[a1] * y_px_um);
			int xa1 = intRound(cx + ri1_um * COS[a1] * x_px_um);
			int ya2 = intRound(cy - ri2_um * SIN[a2] * y_px_um);
			int xa2 = intRound(cx + ri2_um * COS[a2] * x_px_um);
			m_RingsDC.DrawLine(xa1, ya1, xa2, ya2, 1, r % 2 ? YELLOW : RED);
		}
	}

	Invalidate(FALSE);
}

//***************************************************************************************

int CCalDlg::GetSelectedBall()
{
	if (m_Ball1Radio.GetCheck()) return 0;
	if (m_Ball2Radio.GetCheck()) return 1;
	if (m_Ball3Radio.GetCheck()) return 2;
	if (m_Ball4Radio.GetCheck()) return 3;
	return 0;
}

//***************************************************************************************

void CCalDlg::SetDlgData()
{
	CString s;

	// GUID
	CString guid;
	::GUIDToStr(m_pCalibration->CalibrationID, guid);
	m_GUIDEdit.SetWindowText(guid);
	// Serial number
	s = m_pCalibration->HardwareSerial;
	m_SerialEdit.SetWindowText(s);
	// Date
	s.Format(_T("%04i-%02i-%02i"), m_pCalibration->Time.wYear, m_pCalibration->Time.wMonth, m_pCalibration->Time.wDay);
	m_DateEdit.SetWindowText(s);
	// Time
	s.Format(_T("%02i:%02i"), m_pCalibration->Time.wHour, m_pCalibration->Time.wMinute);
	m_TimeEdit.SetWindowText(s);

	// Laser wavelength
	s.Format(_T("%.0f"), m_pCalibration->LaserWavelength);
	m_WavelengthEdit.SetWindowText(s);

	// Number of scan points
	s.Format(_T("%i"), m_pCalibration->NumPoints);
	m_NumPointsEdit.SetWindowText(s);

	// AODX flip
	m_FlipAODXCheck.SetCheck(!m_pCalibration->AODXNoFlip);
	// AODX frequency 0
	s.Format(_T("%i"), m_pCalibration->AODX.Freq0);
	m_AODXFreq0Edit.SetWindowText(s);
	// AODX Hz per Um
	s.Format(_T("%i"), m_pCalibration->AODX.HzUm);
	m_AODXHzUmEdit.SetWindowText(s);
	// AODX RMS
	s.Format(_T("%i"), m_pCalibration->AODX.RMS);
	m_AODXRMSEdit.SetWindowText(s);
	// AODY flip
	m_FlipAODYCheck.SetCheck(!m_pCalibration->AODYNoFlip);
	// AODY frequency 0
	s.Format(_T("%i"), m_pCalibration->AODY.Freq0);
	m_AODYFreq0Edit.SetWindowText(s);
	// AODY Hz per Um
	s.Format(_T("%i"), m_pCalibration->AODY.HzUm);
	m_AODYHzUmEdit.SetWindowText(s);
	// AODY RMS
	s.Format(_T("%i"), m_pCalibration->AODY.RMS);
	m_AODYRMSEdit.SetWindowText(s);

	// LADs exposure time
	s.Format(_T("%i"), m_pCalibration->LADExposureTime);
	m_LADExpTimeEdit.SetWindowText(s);
	// LADs swap
	m_SwapLADsCheck.SetCheck(m_pCalibration->LADSwapXY);
	// LADX flip
	m_FlipLADXCheck.SetCheck(!m_pCalibration->LADXNoFlip);
	// LADX focal distance
	s.Format(_T("%.0f"), m_pCalibration->LADXFocalDistance);
	m_LADXFocalDistanceEdit.SetWindowText(s);
	// LADY flip
	m_FlipLADYCheck.SetCheck(!m_pCalibration->LADYNoFlip);
	// LADY focal distance
	s.Format(_T("%.0f"), m_pCalibration->LADYFocalDistance);
	m_LADYFocalDistanceEdit.SetWindowText(s);

	// Video flip X
	m_VideoFlipXCheck.SetCheck(m_pCalibration->VideoFlipX);
	// Video flip Y
	m_VideoFlipYCheck.SetCheck(m_pCalibration->VideoFlipY);
	// Video width
	s.Format(_T("%i"), m_pCalibration->VideoWidth);
	m_VideoWidthEdit.SetWindowText(s);
	// Video height
	s.Format(_T("%i"), m_pCalibration->VideoHeight);
	m_VideoHeightEdit.SetWindowText(s);
	// Video width in microns
	s.Format(_T("%.0f"), m_pCalibration->VideoWidthMicrons);
	m_VideoWidthUmEdit.SetWindowText(s);
	// Video height in microns
	s.Format(_T("%.0f"), m_pCalibration->VideoHeightMicrons);
	m_VideoHeightUmEdit.SetWindowText(s);

	// WF video infrared illumination level
	s.Format(_T("%i"), m_pCalibration->WFVideoSettings.InfraredLEDsPowerLevel);
	m_WFVideoInfraredLEDsPowerLevelEdit.SetWindowText(s);
	// WF video brightness
	s.Format(_T("%i"), m_pCalibration->WFVideoSettings.Brightness);
	m_WFVideoBrightnessEdit.SetWindowText(s);
	// WF video contrast
	s.Format(_T("%i"), m_pCalibration->WFVideoSettings.Contrast);
	m_WFVideoContrastEdit.SetWindowText(s);

	// WF color video infrared illumination level
	s.Format(_T("%i"), m_pCalibration->WFVideo2Settings.InfraredLEDsPowerLevel);
	m_WFVideo2InfraredLEDsPowerLevelEdit.SetWindowText(s);
	// WF color video white illumination level
	s.Format(_T("%i"), m_pCalibration->WFVideo2Settings.WhiteLEDsPowerLevel);
	m_WFVideo2WhiteLEDsPowerLevelEdit.SetWindowText(s);
	// WF color video brightness
	s.Format(_T("%i"), m_pCalibration->WFVideo2Settings.Brightness);
	m_WFVideo2BrightnessEdit.SetWindowText(s);
	// WF color video contrast
	s.Format(_T("%i"), m_pCalibration->WFVideo2Settings.Contrast);
	m_WFVideo2ContrastEdit.SetWindowText(s);
	// WF color video hue
	s.Format(_T("%i"), m_pCalibration->WFVideo2Settings.Hue);
	m_WFVideo2HueEdit.SetWindowText(s);
	// WF color video saturation
	s.Format(_T("%i"), m_pCalibration->WFVideo2Settings.Saturation);
	m_WFVideo2SaturationEdit.SetWindowText(s);
	// WF color video delay
	s.Format(_T("%i"), m_pCalibration->ColorImageDelay);
	m_WFVideo2DelayEdit.SetWindowText(s);

	// CT video brightness
	s.Format(_T("%i"), m_pCalibration->CTVideoSettings.Brightness);
	m_CTVideoBrightnessEdit.SetWindowText(s);
	// CT video contrast
	s.Format(_T("%i"), m_pCalibration->CTVideoSettings.Contrast);
	m_CTVideoContrastEdit.SetWindowText(s);

	// Zernike C3
	s.Format(_T("%.3f"), m_pCalibration->Zernike.C3);
	m_C3Edit.SetWindowText(s);
	// Zernike C4
	s.Format(_T("%.3f"), m_pCalibration->Zernike.C4);
	m_C4Edit.SetWindowText(s);
	// Zernike C5
	s.Format(_T("%.3f"), m_pCalibration->Zernike.C5);
	m_C5Edit.SetWindowText(s);

	// Accommodation target illumination level
	s.Format(_T("%i"), m_pCalibration->AccommodationTargetPowerLevel);
	m_AccommodationTargetPowerLevelEdit.SetWindowText(s);
	// Far target illumination level
	s.Format(_T("%i"), m_pCalibration->FarTargetPowerLevel);
	m_FarTargetPowerLevelEdit.SetWindowText(s);
	// Near target illumination level
	s.Format(_T("%i"), m_pCalibration->NearTargetPowerLevel);
	m_NearTargetPowerLevelEdit.SetWindowText(s);

	//
	m_ZAlignmentMethod1Radio.SetCheck(m_pCalibration->ZAlignmentMethod == 0);
	m_ZAlignmentMethod2Radio.SetCheck(m_pCalibration->ZAlignmentMethod != 0);
	// Laser threshold
	s.Format(_T("%i"), m_pCalibration->LaserIntensityThreshold);
	m_LaserThresholdEdit.SetWindowText(s);
	// Sensor threshold
	s.Format(_T("%i"), m_pCalibration->SensorThreshold);
	m_SensorThresholdEdit.SetWindowText(s);
	// Laser duration
	s.Format(_T("%i"), m_pCalibration->LaserDuration);
	m_LaserDurationEdit.SetWindowText(s);

	//
	s.Format(_T("%i"), m_pCalibration->NumConsecFrames);
	m_NumConsecFramesEdit.SetWindowText(s);
	//
	s.Format(_T("%i"), m_pCalibration->AllowedDecentError);
	m_AllowedDecentErrorEdit.SetWindowText(s);
	//
	s.Format(_T("%i"), m_pCalibration->AllowedMovement);
	m_AllowedMovementEdit.SetWindowText(s);

	// Ball 1 radius
	s.Format(_T("%.0f"), m_pCalibration->BallsAx[0]);
	m_Ball1Edit.SetWindowText(s);
	// Ball 2 radius
	s.Format(_T("%.0f"), m_pCalibration->BallsAx[1]);
	m_Ball2Edit.SetWindowText(s);
	// Ball 3 radius
	s.Format(_T("%.0f"), m_pCalibration->BallsAx[2]);
	m_Ball3Edit.SetWindowText(s);
	// Ball 4 radius
	s.Format(_T("%.0f"), m_pCalibration->BallsAx[3]);
	m_Ball4Edit.SetWindowText(s);

	// Number of rings
	s.Format(_T("%i"), m_pCalibration->NumRings);
	m_NumRingsEdit.SetWindowText(s);

	// Optometer table
	s.Format(_T("%i"), m_pCalibration->OptometerTable[0]);
	m_OptM10Edit.SetWindowText(s);
	s.Format(_T("%i"), m_pCalibration->OptometerTable[1]);
	m_OptM09Edit.SetWindowText(s);
	s.Format(_T("%i"), m_pCalibration->OptometerTable[2]);
	m_OptM08Edit.SetWindowText(s);
	s.Format(_T("%i"), m_pCalibration->OptometerTable[3]);
	m_OptM07Edit.SetWindowText(s);
	s.Format(_T("%i"), m_pCalibration->OptometerTable[4]);
	m_OptM06Edit.SetWindowText(s);
	s.Format(_T("%i"), m_pCalibration->OptometerTable[5]);
	m_OptM05Edit.SetWindowText(s);
	s.Format(_T("%i"), m_pCalibration->OptometerTable[6]);
	m_OptM04Edit.SetWindowText(s);
	s.Format(_T("%i"), m_pCalibration->OptometerTable[7]);
	m_OptM03Edit.SetWindowText(s);
	s.Format(_T("%i"), m_pCalibration->OptometerTable[8]);
	m_OptM02Edit.SetWindowText(s);
	s.Format(_T("%i"), m_pCalibration->OptometerTable[9]);
	m_OptM01Edit.SetWindowText(s);
	s.Format(_T("%i"), m_pCalibration->OptometerTable[10]);
	m_OptN00Edit.SetWindowText(s);
	s.Format(_T("%i"), m_pCalibration->OptometerTable[11]);
	m_OptP01Edit.SetWindowText(s);
	s.Format(_T("%i"), m_pCalibration->OptometerTable[12]);
	m_OptP02Edit.SetWindowText(s);
	s.Format(_T("%i"), m_pCalibration->OptometerTable[13]);
	m_OptP03Edit.SetWindowText(s);
	s.Format(_T("%i"), m_pCalibration->OptometerTable[14]);
	m_OptP04Edit.SetWindowText(s);
	s.Format(_T("%i"), m_pCalibration->OptometerTable[15]);
	m_OptP05Edit.SetWindowText(s);
	s.Format(_T("%i"), m_pCalibration->OptometerTable[16]);
	m_OptP06Edit.SetWindowText(s);
	s.Format(_T("%i"), m_pCalibration->OptometerTable[17]);
	m_OptP07Edit.SetWindowText(s);
	s.Format(_T("%i"), m_pCalibration->OptometerTable[18]);
	m_OptP08Edit.SetWindowText(s);
	s.Format(_T("%i"), m_pCalibration->OptometerTable[19]);
	m_OptP09Edit.SetWindowText(s);
	s.Format(_T("%i"), m_pCalibration->OptometerTable[20]);
	m_OptP10Edit.SetWindowText(s);
	// Optometer gap
	s.Format(_T("%i"), m_pCalibration->OptometerGap);
	m_OptGapEdit.SetWindowText(s);
	// Optometer enabled
	m_OptEnabledCheck.SetCheck(m_pCalibration->OptometerEnabled == 1 ? 1 : 0);

	// Draw rings
	OnBallRadioClicked();
}

//***************************************************************************************

void CCalDlg::GetDlgData()
{
	CString s;

	// Serial number
	m_SerialEdit.GetWindowText(s);
	CStringA ansiStr(s);
	strncpy(m_pCalibration->HardwareSerial, ansiStr, sizeof(m_pCalibration->HardwareSerial));
	m_pCalibration->HardwareSerial[sizeof(m_pCalibration->HardwareSerial) - 1] = 0;

	// Laser wavelength
	m_WavelengthEdit.GetWindowText(s);
	CStringA ansiStr2(s);
	m_pCalibration->LaserWavelength = atof(ansiStr2);

	// Number of scan points
	m_NumPointsEdit.GetWindowText(s);
	CStringA ansiStr3(s);
	m_pCalibration->NumPoints = atoi(ansiStr3);

	// AODX flip
	m_pCalibration->AODXNoFlip = !m_FlipAODXCheck.GetCheck();
	// AODX frequency 0
	m_AODXFreq0Edit.GetWindowText(s);
	CStringA ansiStr4(s);
	m_pCalibration->AODX.Freq0 = atoi(ansiStr4);
	// AODX Hz per Um
	m_AODXHzUmEdit.GetWindowText(s);
	CStringA ansiStr5(s);
	m_pCalibration->AODX.HzUm = atoi(ansiStr5);
	// AODX RMS
	m_AODXRMSEdit.GetWindowText(s);
	CStringA ansiStr6(s);
	m_pCalibration->AODX.RMS = atoi(ansiStr6);
	// AODY flip
	m_pCalibration->AODYNoFlip = !m_FlipAODYCheck.GetCheck();
	// AODY frequency 0
	m_AODYFreq0Edit.GetWindowText(s);
	CStringA ansiStr7(s);
	m_pCalibration->AODY.Freq0 = atoi(ansiStr7);
	// AODY Hz per Um
	m_AODYHzUmEdit.GetWindowText(s);
	CStringA ansiStr7a(s);//6.2.0 New (another new is the ruler in download wf exem)
	m_pCalibration->AODY.HzUm = atoi(ansiStr7a);
	// AODY RMS
	m_AODYRMSEdit.GetWindowText(s);
	CStringA ansiStr8(s);
	m_pCalibration->AODY.RMS = atoi(ansiStr8);

	// LADs exposure time
	m_LADExpTimeEdit.GetWindowText(s);
	CStringA ansiStr9(s);
	m_pCalibration->LADExposureTime = atoi(ansiStr9);
	// LADs swap
	m_pCalibration->LADSwapXY = m_SwapLADsCheck.GetCheck();
	// LADX flip
	m_pCalibration->LADXNoFlip = !m_FlipLADXCheck.GetCheck();
	// LADX focal distance
	m_LADXFocalDistanceEdit.GetWindowText(s);
	CStringA ansiStra(s);
	m_pCalibration->LADXFocalDistance = atof(ansiStra);
	// LADY flip
	m_pCalibration->LADYNoFlip = !m_FlipLADYCheck.GetCheck();
	// LADY focal distance
	m_LADYFocalDistanceEdit.GetWindowText(s);
	CStringA ansiStrb(s);
	m_pCalibration->LADYFocalDistance = atof(ansiStrb);

	// Video flip X
	m_pCalibration->VideoFlipX = m_VideoFlipXCheck.GetCheck();
	// Video flip Y
	m_pCalibration->VideoFlipY = m_VideoFlipYCheck.GetCheck();
	// Video width in microns
	m_VideoWidthUmEdit.GetWindowText(s);
	CStringA ansiStrc(s);
	m_pCalibration->VideoWidthMicrons = atof(ansiStrc);
	// Video height in microns
	m_VideoHeightUmEdit.GetWindowText(s);
	CStringA ansiStrd(s);
	m_pCalibration->VideoHeightMicrons = atof(ansiStrd);

	// WF video infrared illumination level
	m_WFVideoInfraredLEDsPowerLevelEdit.GetWindowText(s);
	CStringA ansiStre(s);
	m_pCalibration->WFVideoSettings.InfraredLEDsPowerLevel = atoi(ansiStre);
	// WF video brightness
	m_WFVideoBrightnessEdit.GetWindowText(s);
	CStringA ansiStrf(s);
	m_pCalibration->WFVideoSettings.Brightness = atoi(ansiStrf);
	// WF video contrast
	m_WFVideoContrastEdit.GetWindowText(s);
	CStringA ansiStrg(s);
	m_pCalibration->WFVideoSettings.Contrast = atoi(ansiStrg);

	// WF color video infrared illumination level
	m_WFVideo2InfraredLEDsPowerLevelEdit.GetWindowText(s);
	CStringA ansiStra0(s);
	m_pCalibration->WFVideo2Settings.InfraredLEDsPowerLevel = atoi(ansiStra0);
	// WF color video white illumination level
	m_WFVideo2WhiteLEDsPowerLevelEdit.GetWindowText(s);
	CStringA ansiStra1(s);
	m_pCalibration->WFVideo2Settings.WhiteLEDsPowerLevel = atoi(ansiStra1);
	// WF color video brightness
	m_WFVideo2BrightnessEdit.GetWindowText(s);
	CStringA ansiStra2(s);
	m_pCalibration->WFVideo2Settings.Brightness = atoi(ansiStra2);
	// WF color video contrast
	m_WFVideo2ContrastEdit.GetWindowText(s);
	CStringA ansiStra3(s);
	m_pCalibration->WFVideo2Settings.Contrast = atoi(ansiStra3);
	// WF color video hue
	m_WFVideo2HueEdit.GetWindowText(s);
	CStringA ansiStra4(s);
	m_pCalibration->WFVideo2Settings.Hue = atoi(ansiStra4);
	// WF color video saturation
	m_WFVideo2SaturationEdit.GetWindowText(s);
	CStringA ansiStra5(s);
	m_pCalibration->WFVideo2Settings.Saturation = atoi(ansiStra5);
	// WF color video delay
	m_WFVideo2DelayEdit.GetWindowText(s);
	CStringA ansiStrb0(s);
	m_pCalibration->ColorImageDelay = atoi(ansiStrb0);

	// CT video brightness
	m_CTVideoBrightnessEdit.GetWindowText(s);
	CStringA ansiStrb1(s);
	m_pCalibration->CTVideoSettings.Brightness = atoi(ansiStrb1);
	// CT video contrast
	m_CTVideoContrastEdit.GetWindowText(s);
	CStringA ansiStrb2(s);
	m_pCalibration->CTVideoSettings.Contrast = atoi(ansiStrb2);

	// Zernike C3
	m_C3Edit.GetWindowText(s);
	CStringA ansiStrb3(s);
	m_pCalibration->Zernike.C3 = atof(ansiStrb3);
	// Zernike C4
	m_C4Edit.GetWindowText(s);
	CStringA ansiStrb4(s);
	m_pCalibration->Zernike.C4 = atof(ansiStrb4);
	// Zernike C5
	m_C5Edit.GetWindowText(s);
	CStringA ansiStrb5(s);
	m_pCalibration->Zernike.C5 = atof(ansiStrb5);

	// Accommodation target illumination level
	m_AccommodationTargetPowerLevelEdit.GetWindowText(s);
	CStringA ansiStrc0(s);
	m_pCalibration->AccommodationTargetPowerLevel = atoi(ansiStrc0);
	// Far target illumination level
	m_FarTargetPowerLevelEdit.GetWindowText(s);
	CStringA ansiStrc1(s);
	m_pCalibration->FarTargetPowerLevel = atoi(ansiStrc1);
	// Near target illumination level
	m_NearTargetPowerLevelEdit.GetWindowText(s);
	CStringA ansiStraaa(s);
	m_pCalibration->NearTargetPowerLevel = atoi(ansiStraaa);

	//
	if (m_ZAlignmentMethod1Radio.GetCheck()) m_pCalibration->ZAlignmentMethod = 0;
	else m_pCalibration->ZAlignmentMethod = 1;
	// Laser threshold
	m_LaserThresholdEdit.GetWindowText(s);
	CStringA ansiStrc2(s);
	m_pCalibration->LaserIntensityThreshold = atoi(ansiStrc2);
	// Sensor threshold
	m_SensorThresholdEdit.GetWindowText(s);
	CStringA ansiStrc3(s);
	m_pCalibration->SensorThreshold = atoi(ansiStrc3);
	// Laser duration
	m_LaserDurationEdit.GetWindowText(s);
	CStringA ansiStrc4(s);
	m_pCalibration->LaserDuration = atoi(ansiStrc4);

	//
	m_NumConsecFramesEdit.GetWindowText(s);
	CStringA ansiStrc5(s);
	m_pCalibration->NumConsecFrames = atoi(ansiStrc5);
	//
	m_AllowedDecentErrorEdit.GetWindowText(s);
	CStringA ansiStrc6(s);
	m_pCalibration->AllowedDecentError = atoi(ansiStrc6);
	//
	m_AllowedMovementEdit.GetWindowText(s);
	CStringA ansiStrd1(s);
	m_pCalibration->AllowedMovement = atoi(ansiStrd1);

	// Ball 1 radius
	m_Ball1Edit.GetWindowText(s);
	CStringA ansiStrd2(s);
	m_pCalibration->BallsAx[0] = atof(ansiStrd2);
	// Ball 2 radius
	m_Ball2Edit.GetWindowText(s);
	CStringA ansiStrd3(s);
	m_pCalibration->BallsAx[1] = atof(ansiStrd3);
	// Ball 3 radius
	m_Ball3Edit.GetWindowText(s);
	CStringA ansiStrd4(s);
	m_pCalibration->BallsAx[2] = atof(ansiStrd4);
	// Ball 4 radius
	m_Ball4Edit.GetWindowText(s);
	CStringA ansiStrd5(s);
	m_pCalibration->BallsAx[3] = atof(ansiStrd5);

	// Number of rings
	m_NumRingsEdit.GetWindowText(s);
	CStringA ansiStre1(s);
	m_pCalibration->NumRings = atoi(ansiStre1);

	// Optometer table
	m_OptM10Edit.GetWindowText(s);
	CStringA ansiStre2(s);
	m_pCalibration->OptometerTable[0] = atoi(ansiStre2);
	m_OptM09Edit.GetWindowText(s);
	CStringA ansiStre3(s);
	m_pCalibration->OptometerTable[1] = atoi(ansiStre3);
	m_OptM08Edit.GetWindowText(s);
	CStringA ansiStre4(s);
	m_pCalibration->OptometerTable[2] = atoi(ansiStre4);
	m_OptM07Edit.GetWindowText(s);
	CStringA ansiStre5(s);
	m_pCalibration->OptometerTable[3] = atoi(ansiStre5);
	m_OptM06Edit.GetWindowText(s);
	CStringA ansiStrf1(s);
	m_pCalibration->OptometerTable[4] = atoi(ansiStrf1);
	m_OptM05Edit.GetWindowText(s);
	CStringA ansiStrf2(s);
	m_pCalibration->OptometerTable[5] = atoi(ansiStrf2);
	m_OptM04Edit.GetWindowText(s);
	CStringA ansiStrf3(s);
	m_pCalibration->OptometerTable[6] = atoi(ansiStrf3);
	m_OptM03Edit.GetWindowText(s);
	CStringA ansiStrf4(s);
	m_pCalibration->OptometerTable[7] = atoi(ansiStrf4);
	m_OptM02Edit.GetWindowText(s);
	CStringA ansiStrf5(s);
	m_pCalibration->OptometerTable[8] = atoi(ansiStrf5);
	m_OptM01Edit.GetWindowText(s);
	CStringA ansiStrg1(s);
	m_pCalibration->OptometerTable[9] = atoi(ansiStrg1);
	m_OptN00Edit.GetWindowText(s);
	CStringA ansiStrg2(s);
	m_pCalibration->OptometerTable[10] = atoi(ansiStrg2);
	m_OptP01Edit.GetWindowText(s);
	CStringA ansiStrg3(s);
	m_pCalibration->OptometerTable[11] = atoi(ansiStrg3);
	m_OptP02Edit.GetWindowText(s);
	CStringA ansiStrg4(s);
	m_pCalibration->OptometerTable[12] = atoi(ansiStrg4);
	m_OptP03Edit.GetWindowText(s);
	CStringA ansiStrg5(s);
	m_pCalibration->OptometerTable[13] = atoi(ansiStrg5);
	m_OptP04Edit.GetWindowText(s);
	CStringA ansiStr00(s);
	m_pCalibration->OptometerTable[14] = atoi(ansiStr00);
	m_OptP05Edit.GetWindowText(s);
	CStringA ansiStraa(s);
	m_pCalibration->OptometerTable[15] = atoi(ansiStraa);
	m_OptP06Edit.GetWindowText(s);
	CStringA ansiStrab(s);
	m_pCalibration->OptometerTable[16] = atoi(ansiStrab);
	m_OptP07Edit.GetWindowText(s);
	CStringA ansiStrac(s);
	m_pCalibration->OptometerTable[17] = atoi(ansiStrac);
	m_OptP08Edit.GetWindowText(s);
	CStringA ansiStrad(s);
	m_pCalibration->OptometerTable[18] = atoi(ansiStrad);
	m_OptP09Edit.GetWindowText(s);
	CStringA ansiStrae(s);
	m_pCalibration->OptometerTable[19] = atoi(ansiStrae);
	m_OptP10Edit.GetWindowText(s);
	CStringA ansiStrba(s);
	m_pCalibration->OptometerTable[20] = atoi(ansiStrba);
	// Optometer gap
	m_OptGapEdit.GetWindowText(s);
	CStringA ansiStrbb(s);
	m_pCalibration->OptometerGap = atoi(ansiStrbb);
	// Optometer enabled
	m_pCalibration->OptometerEnabled = m_OptEnabledCheck.GetCheck() ? 1 : 0;
}

//***************************************************************************************

void CCalDlg::OnLoadButtonClicked()
{
	CBusyCursor Cursor;

	if (m_pHW->LoadCalibrationFromFlash()) {
		SetDlgData();
	}
}

//***************************************************************************************

void CCalDlg::OnSaveButtonClicked()
{
	CBusyCursor Cursor;

	GetDlgData();

	if (m_pHW->SaveCalibrationIntoFlash()) {
		SetDlgData(); // to show new GUID, date & time
	}
}

//***************************************************************************************

void CCalDlg::OnRestoreButtonClicked()
{
	CString FolderName(MAIN_DIR + "\\Cal");
	::CreateDirectory(FolderName, NULL);

	CFileDialog* pDlg = new CFileDialog(TRUE, _T("cal"), _T(""), OFN_HIDEREADONLY, _T("Calibration files (*.cal)|*.cal||"), this);
	pDlg->m_ofn.lpstrTitle = _T("Load settings from a file");
	pDlg->m_ofn.lpstrInitialDir = FolderName;
	if (pDlg->DoModal() == IDOK) {
		if (m_pHW->LoadCalibrationFromFile(pDlg->GetPathName())) {
			SetDlgData();
		}
	}
	delete pDlg;
}

//***************************************************************************************

void CCalDlg::OnBackupButtonClicked()
{
	CString FolderName = MAIN_DIR + "\\Cal";
	::CreateDirectory(FolderName, NULL);

	CString s;
	::GUIDToStr(m_pCalibration->CalibrationID, s);
	CString FileName = FolderName + "\\" + s + ".cal";

	CFileDialog* pDlg = new CFileDialog(FALSE, _T("cal"), FileName, OFN_OVERWRITEPROMPT, _T("Calibration files (*.cal)|*.cal||"), this);
	pDlg->m_ofn.lpstrTitle = _T("Save settings into a file");
	pDlg->m_ofn.lpstrInitialDir = FolderName;
	if (pDlg->DoModal() == IDOK) {
		GetDlgData();
		m_pHW->SaveCalibrationIntoFile(pDlg->GetPathName());
	}
	delete pDlg;
}

//***************************************************************************************

void CCalDlg::OnFirmwareToolButtonClicked()
{
	CFirmwareToolDlg* pDlg = new CFirmwareToolDlg(m_pHW, this);
	if (pDlg->DoModal() == IDOK) {
	}
	delete pDlg;
}

//***************************************************************************************

void CCalDlg::OnAODToolButtonClicked()
{
	GetDlgData();

	CAODToolDlg* pDlg = new CAODToolDlg(m_pHW, this);
	if (pDlg->DoModal() == IDOK) {
		SetDlgData();
	}
	delete pDlg;
}

//***************************************************************************************

void CCalDlg::OnVideoToolButtonClicked()
{
	GetDlgData();

	CVideoToolDlg* pDlg = new CVideoToolDlg(m_pHW, this);
	if (pDlg->DoModal() == IDOK) {
		SetDlgData();
	}
	delete pDlg;
}

//***************************************************************************************

void CCalDlg::OnVideo2ToolButtonClicked()
{
	GetDlgData();

	CVideo2ToolDlg* pDlg = new CVideo2ToolDlg(m_pHW, this);
	if (pDlg->DoModal() == IDOK) {
		SetDlgData();
	}
	delete pDlg;
}

//***************************************************************************************

void CCalDlg::OnRingsToolButtonClicked()
{
	::NewSettings.m_Adjust_CT = TRUE;//[5.1.1]

	GetDlgData();

	CCTAcquisition* pCTAcquisition = new CCTAcquisition(m_pHW, 1);//[cjf***04032012]

	CRingsToolDlg* pDlg = new CRingsToolDlg(pCTAcquisition, GetSelectedBall(), this);
	if (pDlg->DoModal() == IDOK) {
		SetDlgData();
	}
	delete pDlg;

	delete pCTAcquisition;

	::NewSettings.m_Adjust_CT = FALSE;
}

//***************************************************************************************

void CCalDlg::OnOptTableButtonClicked()
{
	real x[21];
	real y[21];
	for (int i = 0; i < 21; i++) {
		x[i] = -10 + i;
		y[i] = m_pCalibration->OptometerTable[i];
	}
	ShowFunction(x, y, 21);
}

//***************************************************************************************

//[5.1.1]
void CCalDlg::OnVideoMoveButtonClicked()
{
	CVideoMoveDlg* pDlg = new CVideoMoveDlg(m_pHW, this);
	if (pDlg->DoModal() == IDOK) {
		SetDlgData();
	}
	delete pDlg;
	//m_pCalibration
}
//[5.1.1]

//***************************************************************************************

//530 trigger a quick wf exam
void CCalDlg::OnVideoQkWFExamBtnClicked()
{
	real backC3 = m_pCalibration->Zernike.C3;
	real backC4 = m_pCalibration->Zernike.C4;
	real backC5 = m_pCalibration->Zernike.C5;
	int  backExposureTime = m_pCalibration->LADExposureTime;
	BOOL backLADSwapXY = m_pCalibration->LADSwapXY;
	BOOL backADXNoFlip = m_pCalibration->LADXNoFlip;
	real backLADXFocalDistance = m_pCalibration->LADXFocalDistance;
	BOOL backLADYNoFlip = m_pCalibration->LADYNoFlip;
	real backLADYFocalDistance = m_pCalibration->LADYFocalDistance;
	int  backOptometerEnabled = m_pCalibration->OptometerEnabled;

	GetDlgData();//

	CQuickWFDlg* pQuickWFDlg = new CQuickWFDlg(&::HW, m_pCalibration, this);
	BOOL Res = pQuickWFDlg->DoModal() == IDOK;


	if (Res)
	{
		CString s;

		// Zernike C3
		s.Format(_T("%.3f"), m_pCalibration->Zernike.C3);
		m_C3Edit.SetWindowText(s);
		// Zernike C4
		s.Format(_T("%.3f"), m_pCalibration->Zernike.C4);
		m_C4Edit.SetWindowText(s);
		// Zernike C5
		s.Format(_T("%.3f"), m_pCalibration->Zernike.C5);
		m_C5Edit.SetWindowText(s);

		// LADs exposure time
		s.Format(_T("%i"), m_pCalibration->LADExposureTime);
		m_LADExpTimeEdit.SetWindowText(s);

		// LADs swap
		m_SwapLADsCheck.SetCheck(m_pCalibration->LADSwapXY);
		// LADX flip
		m_FlipLADXCheck.SetCheck(!m_pCalibration->LADXNoFlip);
		// LADX focal distance
		s.Format(_T("%.0f"), m_pCalibration->LADXFocalDistance);
		m_LADXFocalDistanceEdit.SetWindowText(s);
		// LADY flip
		m_FlipLADYCheck.SetCheck(!m_pCalibration->LADYNoFlip);
		// LADY focal distance
		s.Format(_T("%.0f"), m_pCalibration->LADYFocalDistance);
		m_LADYFocalDistanceEdit.SetWindowText(s);

		m_OptEnabledCheck.SetCheck(m_pCalibration->OptometerEnabled == 1 ? 1 : 0);
	}

	m_pCalibration->Zernike.C3 = backC3;
	m_pCalibration->Zernike.C4 = backC4;
	m_pCalibration->Zernike.C5 = backC5;
	m_pCalibration->LADExposureTime = backExposureTime;
	m_pCalibration->LADSwapXY = backLADSwapXY;
	m_pCalibration->LADXNoFlip = backADXNoFlip;
	m_pCalibration->LADXFocalDistance = backLADXFocalDistance;
	m_pCalibration->LADYNoFlip = backLADYNoFlip;
	m_pCalibration->LADYFocalDistance = backLADYFocalDistance;
	m_pCalibration->OptometerEnabled = backOptometerEnabled;

	int a = 0;
	//Done
}
//530

//***************************************************************************************

void CCalDlg::OnPaint()
{
	CPaintDC PaintDC(this);
	PaintDC.BitBlt(m_l, m_t, m_w, m_h, &m_RingsDC, 0, 0, SRCCOPY);
}

//***************************************************************************************

void CCalDlg::OnCancel()
{
	m_pHW->CancelCalibrationChanges();

	CDialog::OnCancel();
}

void CCalDlg::OnOK()
{
	GetDlgData();

	if (m_pHW->HasCalibrationChanged()) {
		::Warning("Calibration settings have been changed.\nYou need to save them into EEPROM first.");
		return;
	}

	CDialog::OnOK();
}

//***************************************************************************************

/*CWFAcquisition* pWFAcquisition = new CWFAcquisition(&::HW, &::Settings, FALSE);

CWFAcquisitionDlg* pWFAcquisitionDlg = new CWFAcquisitionDlg(this, pWFAcquisition);

BOOL Res = pWFAcquisitionDlg->DoModal() == IDOK;

if (Res) {
CWFExam WFExam = pWFAcquisition->m_WFExam;
WFExam.Process();
CZernikeSurface Surface= WFExam.m_WfSurface;

real um3 = Surface.GetCUm(3);
real um4 = Surface.GetCUm(4);
real um5 = Surface.GetCUm(5);

CString s, sC3, sC4, sC5,sSph;
sC3.Format(_T("%6.10f"),um3);
sC4.Format(_T("%6.10f"),um4);
sC5.Format(_T("%6.10f"),um5);

sC3 = "C3:  " + sC3;
sC4 = "C4:  " + sC4;
sC5 = "C5:  " + sC5;

s = sC3 + "    " + sC4 + "    " + sC5 + "\n\n";

real SphEq, Sph, Cyl; int Axis;
Surface.ChangeRMaxUm(2000);

CWFExam::GetSpheqSphCylAxis(Surface, 0, ::Settings.m_PositiveCylinder,
SphEq, Sph, Cyl, Axis);

sSph.Format(_T("Value of sphere @ 4mm zone:  %.2f", Sph);
s += sSph;

::Info(s);
}*/