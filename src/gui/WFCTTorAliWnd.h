/*******************************************************************
Autor:         Jinfeng Chen
E-mail:        jinfengcc@gmail.com
Date:          04-19-2016
Description:   Create a new wfct gui used for showing toric planner alignment
*****************************************************************************************/


//***************************************************************************************

#pragma once

//***************************************************************************************

#include "SumWnd.h"
#include "PlannerWnd.h"//6.2.0
#include "Data.h"

//***************************************************************************************

class CWFCTTorAliWnd : public CSumWnd
{
public:

	CWFCTTorAliWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CWFExam* pWFExam, CCTExam* pCTExam, int show = SW_SHOW);

private:

	BOOL     m_CheckWnd;//6.2.0 IF it is true, shows the toric check gui, otherwise shows the planner

	BOOL     m_ShowGrayEye;//6.2.0 IF it is planner, show the gray eye or not

	BOOL     m_ShowColorEye;//6.2.0 IF it is planner, show the color eye or not


	CWFExam* m_pWFExam;
	CCTExam* m_pCTExam;

	CBtn     m_PlannerBtn;//6.2.0 Show the planner button
	
	void     PreopData(RECT Rect, int NumRows, BOOL* Rows);//6.2.0 Show the planner text info

	virtual CString GetName() override { return "Toric Alignment Check"; }
	virtual CWndSettings* GetWndSettings() override { return m_pWndSettings + m_d; }

	virtual void CreateMenu() override;//6.2.0

	virtual void RepaintMemDC() override;
	virtual void CreateChildWnd() override;

	afx_msg void OnPlannerBtnClicked();
	afx_msg void OnShowColorEyeClicked();
	afx_msg void OnShowGrayEyeClicked();

	afx_msg LRESULT OnBigPlanWndShown(WPARAM wParam, LPARAM lParam);//6.2.0 Show osher alignment 

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
