//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MemWnd.h"
#include "Hardware.h"
#include "LiveVideo.h"
#include "Image.h"

//***************************************************************************************

class CVideoToolDlg : public CDialog
{
public:

	CVideoToolDlg(CHW* pHW, CWnd* pParentWnd);
	~CVideoToolDlg();

private:

	CHW* m_pHW;
	CLiveVideo* m_pLiveVideo;
	CEyeImage m_Image;

	int         m_w;
	int         m_h;
	real        m_w_um;
	real        m_h_um;
	real        m_x_px_um;
	real        m_y_px_um;
	real        m_cx;
	real        m_cy;

	int         m_BackupInfraredLEDsPowerLevel;
	int         m_BackupBrightness;
	int         m_BackupContrast;

	CMemWnd     m_VideoWnd;
	CButton     m_StartVideoButton;
	CButton     m_AbortVideoButton;
	CButton     m_OKButton;
	CButton     m_CancelButton;
	CEdit       m_InfraredLEDsPowerLevelEdit;
	CEdit       m_BrightnessEdit;
	CEdit       m_ContrastEdit;
	CSliderCtrl m_InfraredLEDsPowerLevelSlider;
	CSliderCtrl m_BrightnessSlider;
	CSliderCtrl m_ContrastSlider;
	CButton     m_PupilCheck;
	CButton     m_TargetCheck;//[cjf***03282012]

	afx_msg void OnStartVideoButtonClicked();
	afx_msg void OnAbortVideoButtonClicked();
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
