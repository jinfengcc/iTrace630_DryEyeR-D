//***************************************************************************************

#pragma once

//***************************************************************************************

#include "SumWnd.h"
#include "Data.h"

//***************************************************************************************

class CCTSingleVrfWnd : public CSumWnd
{
public:

	CCTSingleVrfWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CCTExam* pCTExam, CWndSettings* pWndSettings, int show = SW_SHOW);

private:

	CCTExam* m_pCTExam;

	virtual CString GetNameImpl() const override { return "CT Verification Display"; }
	virtual CWndSettings* GetWndSettings() override { return m_pWndSettings; }

	virtual void CreateMenu() override;

	afx_msg void OnEditLimbusItemClicked();

	virtual void RepaintMemDC() override;
	virtual void CreateChildWnd() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
