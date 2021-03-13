//***************************************************************************************

#pragma once

//***************************************************************************************

#include "SumWnd.h"
#include "Data.h"

//***************************************************************************************

class CWFCTSumWnd : public CSumWnd
{
public:
  CWFCTSumWnd(CWnd *pWnd, RECT &WndRect, CPatient *pPatient, CWFExam *pWFExam, CCTExam *pCTExam, CWndSettings *pWndSettings, int i,
              CString Name, BOOL ShowWin)
    : CWFCTSumWnd(pWnd, WndRect, pPatient, pWFExam, pCTExam, pWndSettings, i, ShowWin)
  {
  }


	CWFCTSumWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CWFExam* pWFExam, CCTExam* pCTExam, CWndSettings* pWndSettings, int i, int show = SW_SHOW);

private:

	BOOL     SameSum();//521  Change Analysis,Summary,Visual Acuity

	CWFExam* m_pWFExam;
	CCTExam* m_pCTExam;

	BOOL m_ShowLargeMap;//530 show large map?
	RECT m_OutLineRect[8];//530 For outline of Chang Analysis

	virtual CString GetNameImpl() const override { return "WF and CT Summary Display"; }
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
