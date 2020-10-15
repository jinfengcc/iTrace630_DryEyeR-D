//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MemWnd.h"
#include "CTAcquisition.h"

//***************************************************************************************

class CRingsToolDlg : public CDialog
{
public:

	CRingsToolDlg(CCTAcquisition* pCTAcquisition, int b, CWnd* pParentWnd);

private:

	CCTAcquisition* m_pCTAcquisition;

	BOOL m_Start;//[5.1.1]
	int m_w;
	int m_h;
	real_t m_w_um;
	real_t m_h_um;
	real_t m_x_px_um;
	real_t m_y_px_um;
	real_t m_cx;
	real_t m_cy;

	int         m_b;
	CCTExam     m_CTExam[4];
	CMemWnd     m_Video1Wnd;
	CMemWnd     m_Video2Wnd;
	CButton     m_Exam1Radio;
	CButton     m_Exam2Radio;
	CButton     m_Exam3Radio;
	CButton     m_Exam4Radio;
	CButton     m_StartExamButton;
	CButton     m_AcquireExamButton;
	CButton     m_AbortExamButton;
	CEdit       m_VideoBrightnessEdit;
	CEdit       m_VideoContrastEdit;
	CSliderCtrl m_VideoBrightnessSlider;
	CSliderCtrl m_VideoContrastSlider;
	CButton     m_OKButton;
	CButton     m_CancelButton;

	//
	CButton     m_Ball1Radio;
	CButton     m_Ball2Radio;
	CButton     m_Ball3Radio;
	CButton     m_Ball4Radio;
	CEdit       m_Ball1Edit;
	CEdit       m_Ball2Edit;
	CEdit       m_Ball3Edit;
	CEdit       m_Ball4Edit;

	CButton     m_CalSaveButton;
	// 

	int m_BackupVideoBrightness;
	int m_BackupVideoContrast;

	int GetSelectedRadio();

	afx_msg void OnRadioClicked();
	afx_msg void OnBallRadioClicked();//
	afx_msg void OnStartExamButtonClicked();
	afx_msg void OnAcquireExamButtonClicked();
	afx_msg void OnAbortExamButtonClicked();
	afx_msg void OnHScroll(uint nSBCode, uint nPos, CScrollBar* pScrollBar);
	afx_msg void OnCalSaveBtnClicked();//  

	afx_msg LRESULT OnUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFinish(WPARAM wParam, LPARAM lParam);

	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;
	virtual void OnCancel() override;
	virtual void OnOK() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
