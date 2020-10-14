//***************************************************************************************

#pragma once

//***************************************************************************************

#include "SumWnd.h"
#include "Data.h"
#include "ImgInfoWnd.h"
#include "VerSliderCtrl.h"

//***************************************************************************************

class CWFSoloCmpWnd : public CSumWnd
{
public:

	CWFSoloCmpWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CWFExam* pWFExam1, CWFExam* pWFExam2, CWndSettings* pWndSettings);

private:

	BOOL    m_OD;//new

#define SOLOTIMER  1 

	CWFExam*       m_pWFExam1;
	CWFExam*       m_pWFExam2;

	int            m_HighLight;
	CImgInfoWnd    m_NoteInfo;

	int            m_mapTop;
	int            m_mapBtm;
	int            m_SplX;
	int            m_Slider_Top;
	int            m_slider1Right;
	int            m_slider3Left;

	CString        m_scan;

	RECT           m_SliderRects[15];

	CBtn           m_SunButton1;
	CBtn           m_MoonButton1;
	CBtn           m_ToggleButton1;

	CBtn           m_SunButton2;
	CBtn           m_MoonButton2;
	CBtn           m_ToggleButton2;

	CBtn           m_SunButton3;
	CBtn           m_MoonButton3;
	CBtn           m_ToggleButton3;

	CVerSliderCtrl m_Slider[3];

	BOOL    SameSoloCmp();//new

	virtual CString GetName() override { return "WF Comparison Display"; }
	virtual CWndSettings* GetWndSettings() override { return m_pWndSettings; }

	virtual void CreateMenu() override;

	virtual void RepaintMemDC() override;
	virtual void CreateChildWnd() override;

	void DiffRefractionInfo();

	afx_msg void OnShowDefaultItemClicked();//new
	afx_msg void OnSelEyeImgItemClicked();//new

	afx_msg LRESULT OnRotateMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeCentMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeStepMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeMaxMsg(WPARAM wParam, LPARAM lParam);

	void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(uint nFlags, CPoint Point);
	afx_msg void OnLButtonDown(uint nFlags, CPoint Point);

	afx_msg void OnSunButton1Clicked();
	afx_msg void OnMoonButton1Clicked();

	afx_msg void OnSunButton2Clicked();
	afx_msg void OnMoonButton2Clicked();

	afx_msg void OnSunButton3Clicked();
	afx_msg void OnMoonButton3Clicked();

	afx_msg void OnToggleButtonClicked();

	void OnSunButtonClicked();
	void OnMoonButtonClicked();

	afx_msg LRESULT OnSliderClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSliderChangedPos(WPARAM wParam, LPARAM lParam);

	afx_msg void OnShowWFTextInfoBtnClicked();//530
	afx_msg void OnShowWFTextInfoBtn2Clicked();//530
	afx_msg void OnShowWFTextInfoBtn3Clicked();//530

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
