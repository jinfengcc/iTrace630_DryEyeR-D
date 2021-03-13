//***************************************************************************************

#pragma once

//***************************************************************************************

#include "SumWnd.h"
#include "VerSliderCtrl.h"
#include "ImgInfoWnd.h"

//***************************************************************************************
#define TYPE_MAP    0 
#define TYPE_VISACU 1 

class CWFSingleSoloWnd : public CSumWnd
{
public:

	CWFSingleSoloWnd(CWnd *pWnd, RECT &WndRect, CPatient *pPatient, CWFExam *pWFExam, CWndSettings *pWndSettings, int show = SW_SHOW);

private:

#define SOLOTIMER  1 

	int      m_AverageExam;//new
	GUID     m_SelectExamsID[1024];//new

	real_t     m_SliderTop;

	int            m_HighLight;
	CImgInfoWnd    m_NoteInfo;

	CString  m_scan;
	CWFExam* m_pWFExam;

	CRect          m_ButtonsRect[3];
	CBtn           m_SunButton;
	CBtn           m_MoonButton;
	CBtn           m_ToggleButton;

	CVerSliderCtrl m_Slider;

	BOOL    SameAbeSet();//new

	virtual CString GetNameImpl() const override { return "Aberration Analysis Display"; }
	virtual CWndSettings* GetWndSettings() override { return m_pWndSettings + m_d; }

	virtual void OnSizeLargeItemClicked() override;

	virtual void CreateMenu() override;

	virtual void RepaintMemDC() override;
	virtual void CreateChildWnd() override;

	afx_msg void OnShowDefaultItemClicked();//new
	afx_msg void OnSelEyeImgItemClicked();//new

	afx_msg void OnSunButtonClicked();
	afx_msg void OnMoonButtonClicked();
	afx_msg void OnToggleButtonClicked();

	afx_msg LRESULT OnSliderClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSliderChangedPos(WPARAM wParam, LPARAM lParam);

	void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(uint nFlags, CPoint Point);
	afx_msg void OnLButtonDown(uint nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
