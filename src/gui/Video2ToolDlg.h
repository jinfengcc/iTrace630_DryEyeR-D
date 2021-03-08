//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MemWnd.h"
#include "Hardware.h"
#include "LiveVideo.h"
#include "Image.h"

//***************************************************************************************

class CVideo2ToolDlg : public CDialog
{
public:

	CVideo2ToolDlg(CHW* pHW, CWnd* pParentWnd);
	~CVideo2ToolDlg();

private:

	CHW* m_pHW;
	CLiveVideo* m_pLiveVideo;
	CEyeImage m_Image;

	int         m_w;
	int         m_h;
	real_t      m_w_um;
	real_t      m_h_um;
	real_t      m_x_px_um;
	real_t      m_y_px_um;
	real_t      m_cx;
	real_t      m_cy;

	BOOL        m_IsHRCamera;

	int         m_BackupInfraredLEDsPowerLevel;
	int         m_BackupWhiteLEDsPowerLevel;
	int         m_BackupBrightness;
	int         m_BackupContrast;
	int         m_BackupHueOrRed;
	int         m_BackupSatOrGreen;
    int         m_BackupBlue;// For high resolution camera
	int         m_BackupDelay;

	CMemWnd     m_VideoWnd;
	CButton     m_StartVideoButton;
	CButton     m_AbortVideoButton;
	CButton     m_StartButton;
	CButton     m_OKButton;
	CButton     m_CancelButton;
	CEdit       m_InfraredLEDsPowerLevelEdit;
	CEdit       m_WhiteLEDsPowerLevelEdit;
	CEdit       m_BrightnessEdit;
	CEdit       m_ContrastEdit;
	CEdit       m_HueOrRedEdit;
	CEdit       m_SatOrGreenEdit;
    CEdit       m_BlueEdit;// For high resolution camera
	CEdit       m_DelayEdit;
	CSliderCtrl m_InfraredLEDsPowerLevelSlider;
	CSliderCtrl m_WhiteLEDsPowerLevelSlider;
	CSliderCtrl m_BrightnessSlider;
	CSliderCtrl m_ContrastSlider;
    CSliderCtrl m_HueOrRedSlider;
    CSliderCtrl m_SatOrGreenSlider;// For high resolution camera
    CSliderCtrl m_BlueSlider;// For high resolution camera

	CStatic     m_HueOrRedSText; // For high resolution camera
    CStatic     m_SatOrGreenText; // For high resolution camera
	CStatic     m_BlueText;// For high resolution camera

	CButton     m_PlacdioCheck;//test cjf 10292020

	afx_msg void OnStartVideoButtonClicked();
	afx_msg void OnAbortVideoButtonClicked();
	afx_msg void OnStartButtonClicked();
	afx_msg void OnHScroll(uint nSBCode, uint nPos, CScrollBar* pScrollBar);
	afx_msg LRESULT OnUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFinish(WPARAM wParam, LPARAM lParam);

	afx_msg void OnPlacdioCheckClicked();// test cjf 10292020

	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;
	virtual void OnCancel() override;
	virtual void OnOK() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
