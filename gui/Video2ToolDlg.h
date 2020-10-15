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
	real_t        m_w_um;
	real_t        m_h_um;
	real_t        m_x_px_um;
	real_t        m_y_px_um;
	real_t        m_cx;
	real_t        m_cy;

	int         m_BackupInfraredLEDsPowerLevel;
	int         m_BackupWhiteLEDsPowerLevel;
	int         m_BackupBrightness;
	int         m_BackupContrast;
	int         m_BackupHue;
	int         m_BackupSaturation;
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
	CEdit       m_HueEdit;
	CEdit       m_SaturationEdit;
	CEdit       m_DelayEdit;
	CSliderCtrl m_InfraredLEDsPowerLevelSlider;
	CSliderCtrl m_WhiteLEDsPowerLevelSlider;
	CSliderCtrl m_BrightnessSlider;
	CSliderCtrl m_ContrastSlider;
	CSliderCtrl m_HueSlider;
	CSliderCtrl m_SaturationSlider;

	afx_msg void OnStartVideoButtonClicked();
	afx_msg void OnAbortVideoButtonClicked();
	afx_msg void OnStartButtonClicked();
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
