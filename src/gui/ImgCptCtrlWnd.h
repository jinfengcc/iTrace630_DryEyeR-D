// 530 manual color image capture controller window

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

  void CreateThisWnd(RECT &Rect, CWnd *pWnd, int ScreenTop, int CapHeight, BOOL IsHRCamera);

  void ShowGUI();
  void HideGUI();
  BOOL m_ShowWindow;

  struct Settings
  {
    int whiteLEDsPower{};
    int brightness{};
    int contrast{};
    int hue{};
    int saturation{};
    int red{};
    int green{};
    int blue{};
  };

  // clang-format off
  int GetWhiteLEDsPower() const  {    return m_settings.whiteLEDsPower;  }
  int GetBrightness    () const  {    return m_settings.brightness;      }
  int GetContrast      () const  {    return m_settings.contrast;        }
  int GetHue           () const  {    return m_settings.hue;             }
  int GetSaturation    () const  {    return m_settings.saturation;      }
  int GetRed           () const  {    return m_settings.red;             }
  int GetGreen         () const  {    return m_settings.green;           }
  int GetBlue          () const  {    return m_settings.blue;            }
  // clang-format on

  BOOL m_Acquire;    // Acquire btn

  void ShowAcqBtnGUI(); // Acquire btn
  void HideAcqBtnGUI(); // Acquire btn

private:
  BOOL m_ShowAcqBtn; // Acquire btn

  int m_ScreenTop;

  BOOL m_IsHRCamera;

  int m_w;
  int m_h;
  int m_eH;
  int m_FontSize;

  Settings m_settings;

  CString m_Titles[6];

  CBtn m_CapBtn; // Acquire btn

  RECT m_sRect[11];
  RECT m_lRect[6];
  RECT m_rRect[6];

  CRad m_radioBtns[5];

  CXPSliderCtrl m_WhiteLEDsPowerLevelSlider;
  CXPSliderCtrl m_BrightnessSlider;
  CXPSliderCtrl m_ContrastSlider;
  CXPSliderCtrl m_HueOrRedSlider;
  CXPSliderCtrl m_SaturationOrGreenSlider;
  CXPSliderCtrl m_BlueSlider;

  void Label();

  void SetTitle(int EyeType, CMFont &font);
  void SetVideoAttributes(int EyeType);
  void SetEyeChecks(int EyeType);

  void RepaintMemDC() override;

  afx_msg void OnVLightEyeRadioClicked();
  afx_msg void OnLightEyeRadioClicked();
  afx_msg void OnMediumEyeRadioClicked();
  afx_msg void OnDarkEyeRadioClicked();
  afx_msg void OnVeryDarkEyeRadioClicked();

  afx_msg void OnCaptureBtnClicked(); // Acquire btn

  afx_msg LRESULT OnSliderChangedPos(WPARAM wParam, LPARAM lParam);
  DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
