//***************************************************************************************

#pragma once

//***************************************************************************************

#include "SumWnd.h"
#include "Data.h"

//***************************************************************************************

class CCTSingleCmpWnd : public CSumWnd
{
public:

	CCTSingleCmpWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CCTExam* pCTExam1, CCTExam* pCTExam2, CWndSettings* pWndSettings);

private:

	//521
	BOOL    SameCTCmp();
	//521

	CCTExam* m_pCTExam1;
	CCTExam* m_pCTExam2;

	virtual CString GetNameImpl() const override { return "CT Comparison Display"; }
	virtual CWndSettings* GetWndSettings() override { return m_pWndSettings; }

	virtual void CreateMenu() override;

	virtual void RepaintMemDC() override;
	virtual void CreateChildWnd() override;

	afx_msg void OnShowDefaultItemClicked();//521

	afx_msg LRESULT OnRotateMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeCentMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeStepMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeMaxMsg(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnChangeRmsGUI(WPARAM wParam, LPARAM lParam);//530

	afx_msg void OnShowCTTextInfoBtnClicked();//530
	afx_msg void OnShowCTTextInfoBtn2Clicked();//530

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
