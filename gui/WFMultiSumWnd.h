//***************************************************************************************

#pragma once

//***************************************************************************************

#include "SumWnd.h"

//***************************************************************************************

class CWFMultiSumWnd : public CSumWnd
{
public:

	CWFMultiSumWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, int NumExams, CWFExam** ppWFExams, CWndSettings* pWndSettings);

private:

	int m_NumExams;
	CWFExam** m_ppWFExams;

	BOOL m_diff;
	real m_r_um;
	int m_series;

	virtual CString GetName() override { return "Multi-Exam Summary Display"; }
	virtual CWndSettings* GetWndSettings() override { return m_pWndSettings; }

	virtual void CreateMenu() override;

	void OnSeriesNumber(int exam_number);
	afx_msg void OnSeriesNumber00ItemClicked() { OnSeriesNumber(0); }
	afx_msg void OnSeriesNumber01ItemClicked() { OnSeriesNumber(1); }
	afx_msg void OnSeriesNumber02ItemClicked() { OnSeriesNumber(2); }
	afx_msg void OnSeriesNumber03ItemClicked() { OnSeriesNumber(3); }
	afx_msg void OnSeriesNumber04ItemClicked() { OnSeriesNumber(4); }
	afx_msg void OnSeriesNumber05ItemClicked() { OnSeriesNumber(5); }
	afx_msg void OnSeriesNumber06ItemClicked() { OnSeriesNumber(6); }
	afx_msg void OnSeriesNumber07ItemClicked() { OnSeriesNumber(7); }
	afx_msg void OnSeriesNumber08ItemClicked() { OnSeriesNumber(8); }
	afx_msg void OnSeriesNumber09ItemClicked() { OnSeriesNumber(9); }

	afx_msg void OnDiffItemClicked();
	afx_msg void OnAvgZoneItemClicked();

	virtual void CreateChildWnd() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
