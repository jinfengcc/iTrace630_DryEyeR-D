//***************************************************************************************

#pragma once

//***************************************************************************************

#include "SumWnd.h"
#include "Onoff.h"
#include "ThroughFocusVSOTFa.h"//531
#include "DofChartDlg.h"//531

//***************************************************************************************

class CWFSingleSumWnd : public CSumWnd
{
public:

	CWFSingleSumWnd(CWnd *pWnd, RECT &WndRect, CPatient *pPatient, CWFExam *pWFExam, CWndSettings *pWndSettings, int k, int show = SW_SHOW);

private:

	int m_k;//520,only show the Radial Ruler under the Angle K/A

	Matrix<real_t> m_X, m_F;//cjf

	BOOL m_MapShowRRuler;
	BOOL m_MapShowLRuler;
	BOOL m_MapShowInlay;

	CWFExam* m_pWFExam;

	int     m_AverageExam;
	GUID    m_SelectExamsID[1024];

								  
	BOOL    SameViAc0();
	BOOL    SameViAc1();

	BOOL    SameWFRMS();
	BOOL    SameAngleKA();
	BOOL    SameDepthFocus();
							 

	virtual CString GetNameImpl() const override
	{
		// WFCT Angle k/a
		if (m_k == 2)//Angle K/A
		{
			return "Angle Kappa/Alpha";
		}
		else// WFCT Angle k/a
		{
			return "WF Summary Display";
		}
	}
	virtual CWndSettings* GetWndSettings() override { return m_pWndSettings + m_d; }

	virtual void OnSizeLargeItemClicked() override;

	virtual void CreateMenu() override;

	virtual void RepaintMemDC() override;
	virtual void CreateChildWnd() override;

	afx_msg void OnShowDefaultItemClicked();
	afx_msg void OnSelEyeImgItemClicked();

	afx_msg void OnEditLimbusItemClicked();//520
	afx_msg void OnShowRRulerItemClicked();//520
	afx_msg void OnShowLRulerItemClicked();//520
	afx_msg void OnShowInalyItemClicked();//520
	afx_msg LRESULT OnChangeRmsGUI(WPARAM wParam, LPARAM lParam);//530

																 //afx_msg void OnDofBtnClicked();//531 

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
