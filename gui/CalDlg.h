//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Hardware.h"
#include "MGDI.h"

//***************************************************************************************

class CCalDlg : public CDialog
{
public:

	CCalDlg(CHW* pHW, CALIBRATION* pCalibration, CWnd* pParentWnd);

	CHW*         m_pHW;
	CALIBRATION* m_pCalibration;

	CButton      m_LoadButton;
	CButton      m_SaveButton;
	CButton      m_RestoreButton;
	CButton      m_BackupButton;
	CButton      m_OKButton;
	CButton      m_VideoMovementButton;//[5.1.1]for allow video frame move a little bit
	CButton      m_VideoQkWFExamButton;//520

	CEdit        m_GUIDEdit;
	CEdit        m_SerialEdit;
	CEdit        m_DateEdit;
	CEdit        m_TimeEdit;
	CButton      m_FirmwareToolButton;

	CEdit        m_WavelengthEdit;

	CEdit        m_NumPointsEdit;

	CButton      m_FlipAODXCheck;
	CEdit        m_AODXFreq0Edit;
	CEdit        m_AODXHzUmEdit;
	CEdit        m_AODXRMSEdit;
	CButton      m_FlipAODYCheck;
	CEdit        m_AODYFreq0Edit;
	CEdit        m_AODYHzUmEdit;
	CEdit        m_AODYRMSEdit;
	CButton      m_AODToolButton;

	CEdit        m_LADExpTimeEdit;
	CButton      m_SwapLADsCheck;
	CButton      m_FlipLADXCheck;
	CEdit        m_LADXFocalDistanceEdit;
	CButton      m_FlipLADYCheck;
	CEdit        m_LADYFocalDistanceEdit;

	CButton      m_VideoFlipXCheck;
	CButton      m_VideoFlipYCheck;
	CEdit        m_VideoWidthEdit;
	CEdit        m_VideoHeightEdit;
	CEdit        m_VideoWidthUmEdit;
	CEdit        m_VideoHeightUmEdit;

	CEdit        m_WFVideoInfraredLEDsPowerLevelEdit;
	CEdit        m_WFVideoBrightnessEdit;
	CEdit        m_WFVideoContrastEdit;

	CEdit        m_WFVideo2InfraredLEDsPowerLevelEdit;
	CEdit        m_WFVideo2WhiteLEDsPowerLevelEdit;
	CEdit        m_WFVideo2BrightnessEdit;
	CEdit        m_WFVideo2ContrastEdit;
	CEdit        m_WFVideo2HueEdit;
	CEdit        m_WFVideo2SaturationEdit;
	CEdit        m_WFVideo2DelayEdit;

	CEdit        m_CTVideoBrightnessEdit;
	CEdit        m_CTVideoContrastEdit;

	CButton      m_VideoToolButton;
	CButton      m_Video2ToolButton;

	CEdit        m_OptP10Edit;
	CEdit        m_OptP09Edit;
	CEdit        m_OptP08Edit;
	CEdit        m_OptP07Edit;
	CEdit        m_OptP06Edit;
	CEdit        m_OptP05Edit;
	CEdit        m_OptP04Edit;
	CEdit        m_OptP03Edit;
	CEdit        m_OptP02Edit;
	CEdit        m_OptP01Edit;
	CEdit        m_OptN00Edit;
	CEdit        m_OptM01Edit;
	CEdit        m_OptM02Edit;
	CEdit        m_OptM03Edit;
	CEdit        m_OptM04Edit;
	CEdit        m_OptM05Edit;
	CEdit        m_OptM06Edit;
	CEdit        m_OptM07Edit;
	CEdit        m_OptM08Edit;
	CEdit        m_OptM09Edit;
	CEdit        m_OptM10Edit;
	CEdit        m_OptGapEdit;
	CButton      m_OptTableButton;
	CButton      m_OptEnabledCheck;

	CEdit        m_C3Edit;
	CEdit        m_C4Edit;
	CEdit        m_C5Edit;

	CEdit        m_AccommodationTargetPowerLevelEdit;
	CEdit        m_FarTargetPowerLevelEdit;
	CEdit        m_NearTargetPowerLevelEdit;

	CEdit        m_NumConsecFramesEdit;
	CEdit        m_AllowedDecentErrorEdit;
	CEdit        m_AllowedMovementEdit;

	CButton      m_ZAlignmentMethod1Radio;
	CButton      m_ZAlignmentMethod2Radio;
	CEdit        m_LaserThresholdEdit;
	CEdit        m_SensorThresholdEdit;
	CEdit        m_LaserDurationEdit;

	CButton      m_Ball1Radio;
	CButton      m_Ball2Radio;
	CButton      m_Ball3Radio;
	CButton      m_Ball4Radio;
	CEdit        m_Ball1Edit;
	CEdit        m_Ball2Edit;
	CEdit        m_Ball3Edit;
	CEdit        m_Ball4Edit;
	CEdit        m_NumRingsEdit;
	CButton      m_RingsToolButton;

	int          m_l;
	int          m_t;
	int          m_w;
	int          m_h;
	CMDC         m_RingsDC;

	void SetDlgData();
	void GetDlgData();

	int GetSelectedBall();

	afx_msg void OnFirmwareToolButtonClicked();
	afx_msg void OnLoadButtonClicked();
	afx_msg void OnSaveButtonClicked();
	afx_msg void OnRestoreButtonClicked();
	afx_msg void OnBackupButtonClicked();
	afx_msg void OnAODToolButtonClicked();
	afx_msg void OnVideoToolButtonClicked();
	afx_msg void OnVideo2ToolButtonClicked();
	afx_msg void OnBallRadioClicked();
	afx_msg void OnRingsToolButtonClicked();
	afx_msg void OnOptTableButtonClicked();
	afx_msg void OnVideoMoveButtonClicked();//[5.1.1]
	afx_msg void OnVideoQkWFExamBtnClicked();//520


	afx_msg void OnPaint();

	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;
	virtual void OnCancel() override;
	virtual void OnOK() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
