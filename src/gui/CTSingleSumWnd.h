//***************************************************************************************

#pragma once

//***************************************************************************************

#include "SumWnd.h"

//***************************************************************************************

class CCTSingleSumWnd : public CSumWnd
{
public:

	CCTSingleSumWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CCTExam* pCTExam, CWndSettings* pWndSettings, int i, int show = SW_SHOW);

private:

	//521
	BOOL     SameCTSum();
	BOOL     SameCTKera();
	BOOL     SameCT3DZ();
	//521

	CCTExam* m_pCTExam;

	virtual CString GetNameImpl() const override { return "CT Summary Display"; }
	virtual CWndSettings* GetWndSettings() override { return m_pWndSettings + m_d; }

	virtual void OnSizeLargeItemClicked() override;

	virtual void CreateMenu() override;

	virtual void RepaintMemDC() override;
	virtual void CreateChildWnd() override;

	afx_msg void OnShowDefaultItemClicked();//521
	afx_msg LRESULT OnOkulix(WPARAM wParam, LPARAM lParam);//521

														   /* afx_msg LRESULT OnShowSurgPlanMsg(WPARAM wParam, LPARAM lParam);
														   afx_msg LRESULT OnShowAxialMapMsg(WPARAM wParam, LPARAM lParam);*/

	afx_msg LRESULT OnChangeRmsGUI(WPARAM wParam, LPARAM lParam);//530

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
