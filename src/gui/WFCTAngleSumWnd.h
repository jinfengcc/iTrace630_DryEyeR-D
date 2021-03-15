/*******************************************************************
Autor:         Jinfeng Chen
E-mail:        jinfengcc@gmail.com
Date:          04-26-2016
Description:   Transfomed angle k/a window from WF windows, add the functionality that
the limbus edge and length should be decided by wf and ct exam togther
*****************************************************************************************/


//***************************************************************************************

#pragma once

//***************************************************************************************

#include "SumWnd.h"
#include "Onoff.h"
#include "ThroughFocusVSOTFa.h"//531
#include "DofChartDlg.h"//531

//***************************************************************************************

class CWFCTAngleSumWnd : public CSumWnd
{
public:
  CWFCTAngleSumWnd(CWnd *pWnd, RECT &WndRect, CPatient *pPatient, CWFExam *pWFExam, CCTExam *pCTExam, CWndSettings *pWndSettings, int k,
                   CString, BOOL ShowWin)
    : CWFCTAngleSumWnd(pWnd, WndRect, pPatient, pWFExam, pCTExam, pWndSettings, k, ShowWin)
  {
  }


	CWFCTAngleSumWnd(CWnd *pWnd, RECT &WndRect, CPatient *pPatient, CWFExam *pWFExam, CCTExam *pCTExam, CWndSettings *pWndSettings, int k,
                   int show = SW_SHOW);

private:

	int m_k;//520,only show the Radial Ruler under the Angle K/A

	Matrix<real_t> m_X, m_F;//cjf

	BOOL m_MapShowRRuler;
	BOOL m_MapShowLRuler;
	BOOL m_MapShowInlay;

	CWFExam* m_pWFExam;
	CCTExam* m_pCTExam;

	int     m_AverageExam;//521
	GUID    m_SelectExamsID[1024];//521

								  //521
	BOOL    SameViAc0();
	BOOL    SameViAc1();

	BOOL    SameWFRMS();
	BOOL    SameAngleKA();
	BOOL    SameDepthFocus();
							 //521


	virtual CString GetNameImpl() const override
	{
		// WFCT Angle k/a
		if (m_k == 2)//Angle K/A
		{
			return "WF&&CT Angle Kappa/Alpha";
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

	afx_msg void OnShowDefaultItemClicked();//521
	afx_msg void OnSelEyeImgItemClicked();//521

	afx_msg void OnEditLimbusItemClicked();//520
	afx_msg void OnShowRRulerItemClicked();//520
	afx_msg void OnShowLRulerItemClicked();//520
	afx_msg void OnShowInalyItemClicked();//520
	afx_msg LRESULT OnChangeRmsGUI(WPARAM wParam, LPARAM lParam);//530

																 //afx_msg void OnDofBtnClicked();//531

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************

