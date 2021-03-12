//***************************************************************************************

#pragma once

//***************************************************************************************

#include "SumWnd.h"
#include "Data.h"

//***************************************************************************************

class CWFCTDysWnd : public CSumWnd
{
public:

	CWFCTDysWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CWFExam* pWFExam, CCTExam* pCTExam, CWndSettings* pWndSettings, int i, int show = SW_SHOW);

private:

	real_t     m_StaticDysVal; //531
	real_t     m_rms[3]; //531
	real_t     m_HoTotalRms[3]; //

	BOOL     SameSum();//521  Change Analysis,Summary,Visual Acuity

	CWFExam* m_pWFExam;
	CCTExam* m_pCTExam;

	BOOL m_ShowLargeMap;//530 show large map?
	RECT m_OutLineRect[12];//530 For outline of Chang Analysis

	void CreateAdjustedUm(real_t um1, real_t um2, real_t um3, real_t um4, real_t um5, real_t um6, real_t& newum1, real_t& newum2);
	real_t CalDysVal(int d, real_t pupilSize);

	virtual CString GetName() override { return "WF&&CT Dysfunctional Lens Analysis"; }
	virtual CWndSettings* GetWndSettings() override { return m_pWndSettings + m_d; }

	virtual void OnSizeLargeItemClicked() override;

	virtual void CreateMenu() override;

	virtual void RepaintMemDC() override;
	virtual void CreateChildWnd() override;

	afx_msg void OnShowDefaultItemClicked();//521

	afx_msg LRESULT OnRotateMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeCentMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeStepMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeMaxMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeRmsGUI(WPARAM wParam, LPARAM lParam);//530

	afx_msg void OnShowWFTextInfoBtnClicked();//530
	afx_msg void OnShowCTTextInfoBtnClicked();//530

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
