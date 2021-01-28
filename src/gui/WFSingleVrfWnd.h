//***************************************************************************************

#pragma once

//***************************************************************************************

#include "SumWnd.h"
#include "Data.h"

//***************************************************************************************

class CWFSingleVrfWnd : public CSumWnd
{
private:

	BOOL    SameVrf0();
	BOOL    SameVrf1();

	int     m_AverageExam;
	GUID    m_SelectExamsID[1024];

public:

	CWFSingleVrfWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CWFExam* pWFExam, CWndSettings* pWndSettings, int show)
    : CWFSingleVrfWnd(pWnd, WndRect, pPatient, pWFExam, pWndSettings, 0, FALSE, show)
  {
  }

  CWFSingleVrfWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CWFExam* pWFExam, CWndSettings* pWndSettings, int whichExam, BOOL ShowExamNum, int show = SW_SHOW);//531
	virtual ~CWFSingleVrfWnd();

	CWFExam* m_pWFExam;

	virtual CString GetName() override { return "WF Verification Display"; }
	virtual CWndSettings* GetWndSettings() override { return m_pWndSettings + m_d; }

	virtual void OnSizeLargeItemClicked() override;

	virtual void CreateMenu() override;

	afx_msg void OnEditLensDotsItemClicked();
	afx_msg void OnEditLimbusItemClicked();
	afx_msg void OnShowDefaultItemClicked();
	afx_msg void OnSelEyeImgItemClicked();

	CBtn m_Shot1Button;
	CBtn m_Shot2Button;
	afx_msg void OnShot1ButtonClicked();
	afx_msg void OnShot2ButtonClicked();

	CBtn m_PrevButton;
	CBtn m_NextButton;
	afx_msg void OnPrevButtonClicked();
	afx_msg void OnNextButtonClicked();

	CBtn m_AcceptButton;
	CBtn m_RejectButton;
	void OnAccept(BOOL accept);
	afx_msg void OnAcceptButtonClicked() { OnAccept(TRUE); }
	afx_msg void OnRejectButtonClicked() { OnAccept(FALSE); }

	RECT m_PointRect[2];
	void PointInfo();

	void ChangeShot(int Shot);
	void ChangePoint(BOOL ChangeShot);

	virtual void RepaintMemDC() override;
	virtual void CreateChildWnd() override;

	void AddDefocus(real_t SphEq);
	afx_msg void OnF1();
	afx_msg void OnF2();
	afx_msg void OnF3();
	afx_msg void OnF4();
	afx_msg void OnF7();

	BOOL m_ShowExamNum;//531
	int  m_whichExam;//531
	BOOL m_inc;
	int m_cnt;
	UINT_PTR m_TimerF4;
	int m_cur_shot_point;
	UINT_PTR m_TimerF7;
	afx_msg void OnTimer(UINT_PTR Event);

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
