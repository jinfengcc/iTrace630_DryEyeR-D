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

	afx_msg void OnEditLensDotsItemClicked();
	afx_msg void OnEditLimbusItemClicked();

	void LensInfo(RECT Rect, int NumRows);

	virtual CString GetName() override { return "WaveTouch Lens Order Display"; }
	virtual CWndSettings* GetWndSettings() override { return m_pWndSettings + m_d; }

	virtual void CreateMenu() override;

	virtual void RepaintMemDC() override;
	virtual void CreateChildWnd() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
