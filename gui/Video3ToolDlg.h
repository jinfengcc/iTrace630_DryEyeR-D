//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MemWnd.h"
#include "Hardware.h"
#include "LiveVideo.h"
#include "Image.h"

//***************************************************************************************

class CVideo3ToolDlg : public CDialog
{
public:

	CVideo3ToolDlg(CHW* pHW, CWnd* pParentWnd);
	~CVideo3ToolDlg();

private:

	CHW* m_pHW;
	CLiveVideo* m_pLiveVideo;
	CEyeImage m_Image;

	int         m_w;
	int         m_h;
	real_t        m_w_um;
	real_t        m_h_um;
	real_t        m_x_px_um;
	real_t        m_y_px_um;
	real_t        m_cx;
	real_t        m_cy;

	int         m_BackupWhiteLEDsPowerLevel;
	int         m_BackupBrightness;
	int         m_BackupContrast;
	int         m_BackupHue;
	int         m_BackupSaturation;

	CMemWnd     m_VideoWnd;
	CButton     m_OKButton;
	CButton     m_CancelButton;

	CButton     m_VeryLightEyeRad;
	CButton     m_LightEyeRad;
	CButton     m_MediumEyeRad;
	CButton     m_DarkEyeRad;
	CButton     m_VeryDarkEyeRad;

	CEdit       m_WhiteLEDsPowerLevelEdit;
	CEdit       m_BrightnessEdit;
	CEdit       m_ContrastEdit;
	CEdit       m_HueEdit;
	CEdit       m_SaturationEdit;

	CSliderCtrl m_WhiteLEDsPowerLevelSlider;
	CSliderCtrl m_BrightnessSlider;
	CSliderCtrl m_ContrastSlider;
	CSliderCtrl m_HueSlider;
	CSliderCtrl m_SaturationSlider;

	afx_msg void OnVeryLightEyeRadioClicked();
	afx_msg void OnLightEyeRadioClicked();
	afx_msg void OnMediumEyeRadioClicked();
	afx_msg void OnDarkEyeRadioClicked();
	afx_msg void OnVeryDarkEyeRadioClicked();

	afx_msg void OnHScroll(uint nSBCode, uint nPos, CScrollBar* pScrollBar);
	afx_msg LRESULT OnUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFinish(WPARAM wParam, LPARAM lParam);

	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;
	virtual void OnCancel() override;
	virtual void OnOK() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
