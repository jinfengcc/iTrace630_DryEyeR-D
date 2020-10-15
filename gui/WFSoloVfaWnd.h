//Solo Vfa display
//***************************************************************************************

#pragma once

//***************************************************************************************

#include "SumWnd.h"
#include "ImgInfoWnd.h"
#include "VerSliderCtrl.h"

//***************************************************************************************

class CWFSoloVfaWnd : public CSumWnd
{
public:

	CWFSoloVfaWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CWFExam* pWFExam1, CWFExam* pWFExam2, CWndSettings* pWndSettings);

private:

#define SOLOTIMER  1 

	CWFExam* m_pWFExam1;
	CWFExam* m_pWFExam2;

	real_t     m_maxRum;
	CString  m_scan1;
	CString  m_scan2;

	int      m_Top;
	int      m_Bootom;
	int      m_MideLine;

	int      m_Slider_Top;
	real_t     m_ODSliderMaxPos;
	real_t     m_OSSliderMaxPos;

	RECT     m_SliderRects[8];

	CBtn     m_SunButton1;
	CBtn     m_MoonButton1;

	CBtn     m_SunButton2;
	CBtn     m_MoonButton2;

	CVerSliderCtrl m_ODSlider;
	CVerSliderCtrl m_OSSlider;

	BOOL    SameSoloVfa();//new

	virtual CString GetName() override { return "Visual Function Analysis Display"; }
	virtual CWndSettings* GetWndSettings() override
	{
		int a = (m_d % 3);
		return m_pWndSettings + (m_d % 3);
	}

	virtual void CreateMenu() override;

	virtual void RepaintMemDC() override;
	virtual void CreateChildWnd() override;

	afx_msg LRESULT OnChangeCentMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeStepMsg(WPARAM wParam, LPARAM lParam);

	afx_msg void OnSunButton1Clicked();
	afx_msg void OnMoonButton1Clicked();

	afx_msg void OnSunButton2Clicked();
	afx_msg void OnMoonButton2Clicked();

	void OnSunButtonClicked();
	void OnMoonButtonClicked();

	//for solo hyperlinker
	int            m_HighLight1;
	int            m_HighLight2;
	CImgInfoWnd    m_NoteInfo;

	void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(uint nFlags, CPoint Point);
	afx_msg void OnLButtonDown(uint nFlags, CPoint Point);
	//Done

	afx_msg void OnShowDefaultItemClicked();//new

	afx_msg LRESULT OnSliderClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSliderChangedPos(WPARAM wParam, LPARAM lParam);

	afx_msg void OnShowWFTextInfoBtnClicked();//530
	afx_msg void OnShowWFTextInfoBtn2Clicked();//530

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
