//***************************************************************************************

#pragma once

//***************************************************************************************

#include "SumWnd.h"

//***************************************************************************************

class CWFCTOUSumWnd : public CSumWnd
{
public:

	CWFCTOUSumWnd(CWnd *pWnd, RECT &WndRect, CPatient *pPatient, CWFExam *pWFExam1, CWFExam *pWFExam2, CCTExam *pCTExam1, CCTExam *pCTExam2,
                CWndSettings *pWndSettings, int show = SW_SHOW);

private:

	int  m_Scale;//521 
	BOOL SameOU();//521

	int m_cen;//530
	int m_right;//530
	int m_btm;//530

	RECT m_TitleRect[2];
	RECT m_InfoRect[2];
	RECT m_ComplaintRect[2];

	CWFExam* m_pWFExam1;
	CWFExam* m_pWFExam2;

	CCTExam* m_pCTExam1;
	CCTExam* m_pCTExam2;

	virtual CString GetNameImpl() const override { return "OU WF and CT Overview"; }
	virtual CWndSettings* GetWndSettings() override { return m_pWndSettings + (m_d % 3); }

	virtual void CreateMenu() override;

	virtual void RepaintMemDC() override;
	virtual void CreateChildWnd() override;

	int     GetGridScale(real_t r_um1, real_t r_um2);//521
	afx_msg void OnShowDefaultItemClicked();//521

	afx_msg void OnCornealItemClicked();//530  

	afx_msg LRESULT OnChangeCentMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeStepMsg(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
