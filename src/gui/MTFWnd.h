//***************************************************************************************

#pragma once

//***************************************************************************************

#include "DispWnd.h"
#include "XPSliderCtrl.h"
#include "Data.h"

//***************************************************************************************

class CMTFWnd : public CDispWnd
{
public:

	CMTFWnd(RECT& Rect, CWnd* pWnd);

	CMTF m_Mtf;
	CMTF m_ReMtf;//531 corrected MTF
	BOOL m_Average;
	int  m_Angle;
	BOOL m_ShowDys;//531 Show Dysfunctional lens bar or not
	real_t m_Dysval;//600 for Dysfunctional lens value

	void CreateSlider();

private:

	virtual void RepaintMemDC() override;
	void Mtf2D();
	void Re_Mtf2D();//corrected MTF 2D
	void Mtf2DDys();//corrected MTF 2D

	void Mtf3D();

	CXPSliderCtrl m_AngleSlider;
	afx_msg LRESULT OnSliderChangedPos(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
