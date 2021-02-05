//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MemWnd.h"
#include "Acquisition.h"
#include "ImgCptCtrlWnd.h"
#include "WFGUICtrlWnd.h"
#include "WFGUICtrl0Wnd.h"
#include "ExamCaptureWnd.h"
#include "HRImgCptCtrlWnd.h"//high resolution camera image settings


//***************************************************************************************

class CAcquisitionDlg : public CDialog
{
public:

	CAcquisitionDlg(CWnd* pParentWnd, CAcquisition* pAcquisition);
	~CAcquisitionDlg();

	BOOL  m_LaserSafe; //6.2.0 For laser safty check

protected:

	BOOL m_CT;

	CAcquisition* m_pAcquisition;

	int m_w;
	int m_h;
	real_t m_w_um;
	real_t m_h_um;
	real_t m_x_px_um;
	real_t m_y_px_um;
	real_t m_cx;
	real_t m_cy;

	BOOL m_ShowTechInfo;

	int  m_top;

	HACCEL             m_hAccel;

	CMemWnd            m_VideoWnd;

	//530
	CMemWnd			   m_unUseWnd;

	CImgCptCtrlWnd	   m_ControlWnd;      //CT color image setting
	CHRImgCptCtrlWnd   m_HRControlWnd;    //high resolution camera image capture settings
	CWFGUICtrl0Wnd     m_WFGUICtrl0Wnd;   //WF Slider of scan size

	CWFGUICtrlWnd      m_WFGUICtrlWnd;   //WF Based panel
		
	 //afx_msg void OnCtrlS();//520, while the user capture the color image, let user adjust camera setting

	afx_msg void OnEscape();
	afx_msg void OnSpace();
	afx_msg void OnCtrlD();
	afx_msg void OnEnter();
	afx_msg void OnShiftEnter();
	afx_msg void OnCtrlEnter();
	virtual afx_msg LRESULT OnUpdate(WPARAM wParam, LPARAM lParam) { return 0; }
	afx_msg LRESULT OnFinish(WPARAM wParam, LPARAM lParam);

	virtual BOOL PreTranslateMessage(MSG* pMsg) override;
	virtual BOOL OnInitDialog() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
