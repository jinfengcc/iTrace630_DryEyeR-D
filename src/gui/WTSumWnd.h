//***************************************************************************************

#pragma once

//***************************************************************************************

#include "SumWnd.h"
#include "Data.h"

//***************************************************************************************

class CWTSumWnd : public CSumWnd
{
public:

	CWTSumWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CWFExam* pWFExam, CCTExam* pCTExam, CWndSettings* pWndSettings);

private:

	CWFExam* m_pWFExam;
	CCTExam* m_pCTExam;

	CString m_labels[5];//6.3.0

	BOOL    m_Pupil;//6.3.0
	int     m_Ang;//6.3.0

	afx_msg void OnEditLensDotsItemClicked();
	afx_msg void OnEditLimbusItemClicked();
	afx_msg LRESULT OnSwitch(WPARAM wParam, LPARAM lParam);//20190118 for this branch

	void LensInfo(RECT Rect, int NumRows);

	virtual CString GetNameImpl() const override { return "WaveTouch Lens Order Display"; }
	virtual CWndSettings* GetWndSettings() override { return m_pWndSettings + m_d; }

	virtual void CreateMenu() override;

	virtual void RepaintMemDC() override;
	virtual void CreateChildWnd() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
