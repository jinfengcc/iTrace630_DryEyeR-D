//***************************************************************************************

#pragma once

//***************************************************************************************

#include "SumWnd.h"
#include "Data.h"

//***************************************************************************************

class CWFSingleCmpWnd : public CSumWnd
{
public:

	CWFSingleCmpWnd(CWnd *pWnd, RECT &WndRect, CPatient *pPatient, CWFExam *pWFExam1, CWFExam *pWFExam2, CWndSettings *pWndSettings,
                  int show = SW_SHOW);

private:

	BOOL    m_OD;//521

	BOOL    SameCmp();//521

	CWFExam* m_pWFExam1;
	CWFExam* m_pWFExam2;

	virtual CString GetNameImpl() const override { return "WF Comparison Display"; }
	virtual CWndSettings* GetWndSettings() override { return m_pWndSettings; }

	virtual void CreateMenu() override;

	virtual void RepaintMemDC() override;
	virtual void CreateChildWnd() override;

	void DiffRefractionInfo();

	afx_msg void OnShowDefaultItemClicked();//521
	afx_msg void OnSelEyeImgItemClicked();//521

	afx_msg LRESULT OnRotateMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeCentMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeStepMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeMaxMsg(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnChangeRmsGUI(WPARAM wParam, LPARAM lParam);//530

	afx_msg void OnShowWFTextInfoBtnClicked();//530
	afx_msg void OnShowWFTextInfoBtn2Clicked();//530
	afx_msg void OnShowWFTextInfoBtn3Clicked();//530

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
