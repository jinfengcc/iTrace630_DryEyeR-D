//[5.1.1]
//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MemWnd.h"
#include "Hardware.h"
#include "LiveVideo.h"
#include "Image.h"
#include "Btn.h"

//***************************************************************************************

class CVideoMoveDlg : public CDialog
{
public:

	CVideoMoveDlg(CHW* pHW, CWnd* pParentWnd);
	~CVideoMoveDlg();

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

	int         m_StepLeft;
	int         m_StepRight;
	int         m_StepUp;
	int         m_StepDown;


	CMemWnd     m_VideoWnd;
	CButton     m_StartVideoButton;
	CButton     m_AbortVideoButton;
	CButton     m_OKButton;
	CButton     m_CancelButton;
	CEdit       m_LeftEdit;
	CEdit       m_RightEdit;
	CEdit       m_UpEdit;
	CEdit       m_DownEdit;
	CBtn        m_LeftButton;
	CBtn        m_RightButton;
	CBtn        m_UpButton;
	CBtn        m_DownButton;


	afx_msg void OnStartVideoButtonClicked();
	afx_msg void OnAbortVideoButtonClicked();
	afx_msg void OnVideoLeftButtonClicked();
	afx_msg void OnVideoRightButtonClicked();
	afx_msg void OnVideoUpButtonClicked();
	afx_msg void OnVideoDownButtonClicked();
	afx_msg LRESULT OnUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFinish(WPARAM wParam, LPARAM lParam);

	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;
	virtual void OnCancel() override;
	virtual void OnOK() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
