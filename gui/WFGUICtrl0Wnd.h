//wf controller window

//***************************************************************************************

#pragma once

//***************************************************************************************
#include "MemWnd.h"
#include "VerSliderCtrl2.h"

//***************************************************************************************

class CWFGUICtrl0Wnd : public CMemWnd
{
public:
	CWFGUICtrl0Wnd();
	~CWFGUICtrl0Wnd();

	void  CreateThisWnd(RECT& Rect, CWnd* pWnd);
	void  ShowGUI();
	void  HideGUI();

	BOOL  m_ShowWindow;

	void  SetPos(real Pos_Val);

	void  Clear() { m_Slider.Clear(); };	//Presbia

	real  GetDiameter() { return m_ScanDiameter; };

	void  CrSliderInfo();//Create the slider for Presbia 256 or  Normal wf capture

private:

	int     m_w;
	int     m_h;
	int     m_FontSize;

	real    m_ScanDiameter;

	RECT    m_SliderRect;

	RECT    m_SliderLabelRect[7];
	RECT    m_Slider64LabelRect[8];

	CVerSliderCtrl2 m_Slider;

	void    Label();
	virtual void RepaintMemDC() override;

	afx_msg LRESULT OnSliderClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSliderChangedPos(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
