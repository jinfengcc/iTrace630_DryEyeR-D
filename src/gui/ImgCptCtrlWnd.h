//530 manual color image capture controller window

//***************************************************************************************

#pragma once

//***************************************************************************************
#include "MemWnd.h"
#include "Data.h"
#include "Btn.h"
#include "Rad.h"
#include "XPSliderCtrl.h"

//***************************************************************************************

class CImgCptCtrlWnd : public CMemWnd
{
public:
	CImgCptCtrlWnd();
	~CImgCptCtrlWnd();

	void CreateThisWnd(RECT& Rect, CWnd* pWnd, int ScreenTop, int CapHeight);

	void  ShowGUI();
	void  HideGUI();
	BOOL  m_ShowWindow;

	int   GetWhiteLEDsPower() { return m_WhiteLEDsPower; };
	int   GetBrightness() { return m_Brightness; };
	int   GetContrast() { return m_Contrast; };
	int   GetHue() { return m_Hue; };
	int   GetSaturation() { return m_Saturation; };

	BOOL  m_ShowAcqBtn;//Acqurie btn
	BOOL  m_Acquire;   //Acqurie btn

	void  ShowAcqBtnGUI();//Acqurie btn
	void  HideAcqBtnGUI();//Acqurie btn

private:

	int  m_ScreenTop;

	int  m_w;
	int  m_h;
	int  m_eH;
	int  m_FontSize;

	int  m_WhiteLEDsPower;
	int  m_Brightness;
	int  m_Contrast;
	int  m_Hue;
	int  m_Saturation;

	CBtn m_CapBtn; //Acqurie btn

	RECT m_sRect[10];
	RECT m_lRect[5];
	RECT m_rRect[5];

	CRad          m_VeryLightEyeRad;
	CRad          m_LightEyeRad;
	CRad          m_MediumEyeRad;
	CRad          m_DarkEyeRad;
	CRad          m_VeryDarkEyeRad;

	CXPSliderCtrl m_WhiteLEDsPowerLevelSlider;
	CXPSliderCtrl m_BrightnessSlider;
	CXPSliderCtrl m_ContrastSlider;
	CXPSliderCtrl m_HueSlider;
	CXPSliderCtrl m_SaturationSlider;

	void Label0();
	void Label1();

	virtual void RepaintMemDC() override;

	afx_msg void OnVLightEyeRadioClicked();
	afx_msg void OnLightEyeRadioClicked();
	afx_msg void OnMediumEyeRadioClicked();
	afx_msg void OnDarkEyeRadioClicked();
	afx_msg void OnVeryDarkEyeRadioClicked();

	afx_msg void OnCaptureBtnClicked();//Acqurie btn

	afx_msg LRESULT OnSliderChangedPos(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
