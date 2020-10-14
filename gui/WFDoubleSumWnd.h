//***************************************************************************************

#pragma once

//***************************************************************************************

#include "SumWnd.h"

//***************************************************************************************

class CWFDoubleSumWnd : public CSumWnd
{
public:

	CWFDoubleSumWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CWFExam* pWFExam1, CWFExam* pWFExam2, CWndSettings* pWndSettings);

private:

	CWFExam* m_pWFExam1;
	CWFExam* m_pWFExam2;

	virtual CString GetName() override { return "Visual Function Analysis Display"; }
	virtual CWndSettings* GetWndSettings() override
	{
		int a = (m_d % 3);
		return m_pWndSettings + (m_d % 3);
	}

	virtual void CreateMenu() override;

	virtual void RepaintMemDC() override;
	virtual void CreateChildWnd() override;

	//521
	BOOL  SameVfa0();
	BOOL  SameVfa1();
	BOOL  SameVfa2();
	BOOL  SameVfa3();
	BOOL  SameVfa4();
	BOOL  SameVfa5();
	afx_msg void OnShowDefaultItemClicked();
	//521

	afx_msg LRESULT OnChangeCentMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeStepMsg(WPARAM wParam, LPARAM lParam);

	afx_msg void OnShowWFTextInfoBtnClicked();//530
	afx_msg void OnShowWFTextInfoBtn2Clicked();//530

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
