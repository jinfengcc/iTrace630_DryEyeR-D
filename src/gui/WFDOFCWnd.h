// The display for Depth of Focus Comparison

//***************************************************************************************

#pragma once

//***************************************************************************************

#include "SumWnd.h"
#include "Onoff.h"
#include "ThroughFocusVSOTFa.h"//531
#include "DofChartDlg.h"//531

//***************************************************************************************

class CWFDOFCWnd : public CSumWnd
{
public:

	CWFDOFCWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CWFExam* pWFExam1, CWFExam* pWFExam2, CWndSettings* pWndSettings);

private:

	Matrix<real_t> m_X1, m_F1;
	Matrix<real_t> m_X2, m_F2;

	CWFExam* m_pWFExam1;
	CWFExam* m_pWFExam2;

	BOOL    SameDepthFocus();

	real_t    m_fraction1;
	real_t    m_fraction2;

	virtual CString GetNameImpl() const override { return "WF Summary Display"; }
	virtual CWndSettings* GetWndSettings() override { return m_pWndSettings + m_d; }

	virtual void CreateMenu() override;

	virtual void RepaintMemDC() override;
	virtual void CreateChildWnd() override;

	afx_msg void OnShowDefaultItemClicked();//521

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
