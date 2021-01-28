//521 Solo Refraction analysis display
//***************************************************************************************

#pragma once

//***************************************************************************************

#include "SumWnd.h"
#include "VerSliderCtrl.h"
#include "ImgInfoWnd.h"

//***************************************************************************************

class CWFDoubleSoloWnd : public CSumWnd
{
public:

	CWFDoubleSoloWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CWFExam* pWFExam1, CWFExam* pWFExam2, CWndSettings* pWndSettings);

private:

#define SOLOTIMER  1 //521

	real_t     m_w1;
	real_t     m_MideLine;
	real_t     m_StopX1;
	real_t     m_StopX2;

	real_t     m_maxRum;
	CString  m_scan1;
	CString  m_scan2;

	int      m_invalidValue;

	CWFExam* m_pWFExam1;
	CWFExam* m_pWFExam2;

	int      m_Slider_Top;
	real_t     m_ODSliderMaxPos;
	real_t     m_OSSliderMaxPos;

	RECT     m_SliderRects[10];

	CBtn           m_SunButton1;
	CBtn           m_MoonButton1;
	CBtn           m_ToggleButton1;

	CBtn           m_SunButton2;
	CBtn           m_MoonButton2;
	CBtn           m_ToggleButton2;

	CVerSliderCtrl m_ODSlider;
	CVerSliderCtrl m_OSSlider;

	int            m_HighLight;
	CImgInfoWnd    m_NoteInfo;

	BOOL    SameRefSet();//new

	virtual CString GetName() override { return "Refraction Analysis Display"; }
	virtual CWndSettings* GetWndSettings() override
	{
		return m_pWndSettings + m_d;
	}

	virtual void CreateMenu() override;

	virtual void RepaintMemDC() override;
	virtual void CreateChildWnd() override;

	afx_msg void OnShowDefaultItemClicked();//new
	afx_msg void OnSelEyeImgItemClicked();//new

	afx_msg LRESULT OnChangeCentMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeStepMsg(WPARAM wParam, LPARAM lParam);

	afx_msg void OnSunButton1Clicked();
	afx_msg void OnMoonButton1Clicked();
	afx_msg void OnToggleButton1Clicked();

	afx_msg void OnSunButton2Clicked();
	afx_msg void OnMoonButton2Clicked();
	afx_msg void OnToggleButton2Clicked();

	void OnSunButtonClicked();
	void OnMoonButtonClicked();

	afx_msg LRESULT OnSliderClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSliderChangedPos(WPARAM wParam, LPARAM lParam);

	void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(uint nFlags, CPoint Point);
	afx_msg void OnLButtonDown(uint nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
