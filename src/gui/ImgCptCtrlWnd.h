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
#define VERYLIGHTEYE 1
#define LIGHTEYE     2
#define MEDIUMEYE    3
#define DARKEYE      4
#define VERYDARKEYE  5

//***************************************************************************************

class CImgCptCtrlWnd : public CMemWnd
{
public:
	CImgCptCtrlWnd();
	~CImgCptCtrlWnd();

	void CreateThisWnd(RECT &Rect, CWnd *pWnd, int ScreenTop, int CapHeight, BOOL IsHRCamera);

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

  const int S_HRVeryLightVals[6]      = {15, 140, 200, 80, 80, 80}; // High resolution camera
  const int S_NormalVeryLightVals[6]  = {9, 65, 247, 22, 255, };        // Previous normal camera

  const int S_HRLightVals[6]          = {11, 125, 200, 90, 90, 90}; 
  const int S_NormalLightVals[6]      = {11, 80, 245, 22, 255, };       

  const int S_HRMediumVals[6]         = {30, 120, 100, 100, 100, 100}; 
  const int S_NormalMediumVals[6]     = {10, 95, 255, 22, 255, };          

  const int S_HRDarkVals[6]           = {30, 140, 200, 100, 100, 100}; 
  const int S_NormalDarkVals[6]       = {10, 110, 255, 22, 255, };         

  const int S_HRVeryDarkVals[6]       = {30, 140, 200, 100, 100, 100}; 
  const int S_NormalVeryDarkVals[6]   = {9, 126, 255, 22, 255, };          

  const CStringA S_HRVeryLight[6]     = {"15/50", "140/255", "200/255", "80/255", "80/255", "80/255"}; 
  const CStringA S_NormalVeryLight[6] = {"9/50", "65/255", "247/255", "22/255", "255/255", ""};           

  const CStringA S_HRLight[6]         = {"11/50", "125/255", "200/255", "90/255", "90/255", "90/255"}; 
  const CStringA S_NormalLight[6]     = {"11/50", "80/255", "245/255", "22/255", "255/255", ""};          

  const CStringA S_HRMedium[6]        = {"30/50", "120/255", "100/255", "100/255", "100/255", "100/255"}; 
  const CStringA S_NormalMedium[6]    = {"10/50", "95/255", "255/255", "22/255", "255/255", ""};          

  const CStringA S_HRDark[6]          = {"30/50", "140/255", "200/255", "100/255", "100/255", "100/255"}; 
  const CStringA S_NormalDark[6]      = {"10/50", "110/255", "255/255", "22/255", "255/255", ""};         

  const CStringA S_HRVeryDark[6]      = {"30/50", "140/255", "200/255", "100/255", "100/255", "100/255"}; 
  const CStringA S_NormalVeryDark[6]  = {"9/50", "126/255", "255/255", "22/255", "255/255", ""};          

  BOOL m_VeryLightStatus[5] = {TRUE, FALSE, FALSE, FALSE, FALSE};
  BOOL m_LightStatus[5] = {FALSE, TRUE, FALSE, FALSE, FALSE};	
  BOOL m_MediumStatus[5] = {FALSE, FALSE, TRUE, FALSE, FALSE};	  
  BOOL m_DarkStatus[5]     = {FALSE, FALSE, FALSE, TRUE, FALSE};	  
  BOOL m_VeryDarkStatus[5] = {FALSE, FALSE, FALSE, FALSE, TRUE};

	int  m_ScreenTop;

	BOOL m_IsHRCamera;

	int  m_w;
	int  m_h;
	int  m_eH;
	int  m_FontSize;

	int  m_WhiteLEDsPower;
	int  m_Brightness;
	int  m_Contrast;
	int  m_Hue;
	int  m_Saturation;

	int m_Red;
    int m_Green;
    int m_Blue;

	CString m_Titles[6];

	CBtn m_CapBtn; //Acqurie btn

	RECT m_sRect[11];
	RECT m_lRect[6];
	RECT m_rRect[6];

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

	// For HR Camera
	CXPSliderCtrl m_RedSlider;
    CXPSliderCtrl m_GreenSlider;
    CXPSliderCtrl m_BlueSlider;
    // For HR Camera

	void Label();

	void SetTitle(int EyeType);
    void SetVideoAttributes(int EyeType);
    void SetEyeChecks(BOOL CheckStatus[5]);

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
