//530 manual color image capture controller window

//***************************************************************************************

//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "ImgCptCtrlWnd.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CImgCptCtrlWnd, CMemWnd)

	ON_BN_CLICKED(IDC_VERY_LIGHT_EYE_RADIO, OnVLightEyeRadioClicked)
	ON_BN_CLICKED(IDC_LIGHT_EYE_RADIO, OnLightEyeRadioClicked)
	ON_BN_CLICKED(IDC_MEDIUM_EYE_RADIO, OnMediumEyeRadioClicked)
	ON_BN_CLICKED(IDC_DARK_EYE_RADIO, OnDarkEyeRadioClicked)
	ON_BN_CLICKED(IDC_VERY_DARK_EYE_RADIO, OnVeryDarkEyeRadioClicked)

	ON_MESSAGE(WM_SLIDER_POS_CHANGED, OnSliderChangedPos)

	ON_BN_CLICKED(IDC_CAPTURE_BTN, OnCaptureBtnClicked)

END_MESSAGE_MAP()

//***************************************************************************************
CImgCptCtrlWnd::CImgCptCtrlWnd() :CMemWnd()
{

}

//***************************************************************************************

CImgCptCtrlWnd::~CImgCptCtrlWnd()
{
}
//***************************************************************************************
void CImgCptCtrlWnd::CreateThisWnd(RECT &Rect, CWnd *pWnd, int ScreenTop, int CapHeight, BOOL IsHRCamera)
{
	CreateWnd(Rect, pWnd);

	m_ScreenTop = ScreenTop;

	m_IsHRCamera = IsHRCamera;

	m_w = Rect.right - Rect.left;
	m_h = Rect.bottom - Rect.top - CapHeight;

	//Acq Btn
	{
		m_Acquire = FALSE;

		int Left_w = intRound(real_t(CapHeight) / 5);
		int BtnW = Left_w;
		int top_h = intRound(real_t(m_w) / 10);

		//530 New Remmber add into Presbia
		int W = ::GetSystemMetrics(SM_CXSCREEN);
		int q = W >= 1920 ? 2 : (W >= 1600 ? 1 : 0);

		RECT tempRect;
		::SetRect(&tempRect, 2 * Left_w, m_h + top_h, 2 * Left_w + BtnW, m_h + top_h + BtnW);
		m_CapBtn.Create(_T(""), _T(""), 0, tempRect, this, IDC_CAPTURE_BTN);

		m_CapBtn.SetImage(q == 2 ? IDR_ACQUISITION_3 : q == 1 ? IDR_ACQUISITION_2 : IDR_ACQUISITION_2);
		m_CapBtn.SetBk(FALSE);
		m_CapBtn.SetBkColor(BLACK);
		//530 New Remmber add into Presbia
	}
	//Acq Btn

	int    sh    = 10;
	real_t StepH = m_IsHRCamera ? 12 : 10;
    real_t FontSizeRatio = m_IsHRCamera ? 0.7 : 0.5;

	m_eH  = intRound(real_t(m_h-130)/StepH);

	m_FontSize = intRound(FontSizeRatio * m_eH);

	::SetRect(&m_sRect[0], 50, sh, m_w - 10, sh + m_eH);
	sh += (m_eH + 10);
	::SetRect(&m_sRect[1], 50, sh, m_w - 10, sh + m_eH);

	sh += (m_eH + 10);
	::SetRect(&m_sRect[2], 50, sh, m_w - 10, sh + m_eH);

	sh += (m_eH + 10);
	::SetRect(&m_sRect[3], 50, sh, m_w - 10, sh + m_eH);

	sh += (m_eH + 10);
	::SetRect(&m_sRect[4], 50, sh, m_w - 10, sh + m_eH);

	sh += (m_eH + 25);
	::SetRect(&m_sRect[5], intRound(0.35*m_w), sh, intRound(0.8*m_w), sh + m_eH);
	::SetRect(&m_lRect[0], 50, sh, intRound(0.35*m_w), sh + m_eH);
	::SetRect(&m_rRect[0], intRound(0.8*m_w), sh, m_w - 10, sh + m_eH);

	sh += (m_eH + 10);
	::SetRect(&m_sRect[6], intRound(0.35*m_w), sh, intRound(0.8*m_w), sh + m_eH);
	::SetRect(&m_lRect[1], 50, sh, intRound(0.35*m_w), sh + m_eH);
	::SetRect(&m_rRect[1], intRound(0.8*m_w), sh, m_w - 10, sh + m_eH);

	sh += (m_eH + 10);
	::SetRect(&m_sRect[7], intRound(0.35*m_w), sh, intRound(0.8*m_w), sh + m_eH);
	::SetRect(&m_lRect[2], 50, sh, intRound(0.35*m_w), sh + m_eH);
	::SetRect(&m_rRect[2], intRound(0.8*m_w), sh, m_w - 10, sh + m_eH);

	sh += (m_eH + 10);
	::SetRect(&m_sRect[8], intRound(0.35*m_w), sh, intRound(0.8*m_w), sh + m_eH);
	::SetRect(&m_lRect[3], 50, sh, intRound(0.35*m_w), sh + m_eH);
	::SetRect(&m_rRect[3], intRound(0.8*m_w), sh, m_w - 10, sh + m_eH);

	sh += (m_eH + 10);
	::SetRect(&m_sRect[9], intRound(0.35*m_w), sh, intRound(0.8*m_w), sh + m_eH);
	::SetRect(&m_lRect[4], 50, sh, intRound(0.35*m_w), sh + m_eH);
	::SetRect(&m_rRect[4], intRound(0.8*m_w), sh, m_w - 10, sh + m_eH);
		
	//For HR Camera
    sh += (m_eH + 10);
    ::SetRect(&m_sRect[10], intRound(0.35 * m_w), sh, intRound(0.8 * m_w), sh + m_eH);
    ::SetRect(&m_lRect[5], 50, sh, intRound(0.35 * m_w), sh + m_eH);
    ::SetRect(&m_rRect[5], intRound(0.8 * m_w), sh, m_w - 10, sh + m_eH);
    // For HR Camera

	m_VeryLightEyeRad.Create(_T(""), _T(""), 0, m_sRect[0], this, IDC_VERY_LIGHT_EYE_RADIO);
	m_LightEyeRad.Create(_T(""), _T(""), 0, m_sRect[1], this, IDC_LIGHT_EYE_RADIO);
	m_MediumEyeRad.Create(_T(""), _T(""), 0, m_sRect[2], this, IDC_MEDIUM_EYE_RADIO);
	m_DarkEyeRad.Create(_T(""), _T(""), 0, m_sRect[3], this, IDC_DARK_EYE_RADIO);
	m_VeryDarkEyeRad.Create(_T(""), _T(""), 0, m_sRect[4], this, IDC_VERY_DARK_EYE_RADIO);

	m_VeryLightEyeRad.SetEnabled(TRUE);
	m_VeryLightEyeRad.SetFontSize(m_FontSize);
	m_VeryLightEyeRad.SetText("Very Light Eye");
	m_VeryLightEyeRad.SetBkColor(BLACK);

	m_LightEyeRad.SetFontSize(m_FontSize);
	m_LightEyeRad.SetText("Light Eye");
	m_LightEyeRad.SetBkColor(BLACK);

	m_MediumEyeRad.SetFontSize(m_FontSize);
	m_MediumEyeRad.SetText("Medium Eye");
	m_MediumEyeRad.SetBkColor(BLACK);

	m_DarkEyeRad.SetFontSize(m_FontSize);
	m_DarkEyeRad.SetText("Dark Eye");
	m_DarkEyeRad.SetBkColor(BLACK);

	m_VeryDarkEyeRad.SetFontSize(m_FontSize);
	m_VeryDarkEyeRad.SetText("Very Dark Eye");
	m_VeryDarkEyeRad.SetBkColor(BLACK);

	m_MediumEyeRad.SetCheck(TRUE);

	m_WhiteLEDsPowerLevelSlider.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_sRect[5], this, IDC_IMGCTRL_SLIDER);
    m_BrightnessSlider.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_sRect[6], this, IDC_IMGCTRL_SLIDER);
    m_ContrastSlider.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_sRect[7], this, IDC_IMGCTRL_SLIDER);     
    
	// For HR Camera
    if (m_IsHRCamera) {
      m_RedSlider.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_sRect[8], this, IDC_IMGCTRL_SLIDER);
      m_GreenSlider.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_sRect[9], this, IDC_IMGCTRL_SLIDER);
      m_BlueSlider.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_sRect[10], this, IDC_IMGCTRL_SLIDER);
    }
    else {
      m_HueSlider.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_sRect[8], this, IDC_IMGCTRL_SLIDER);
      m_SaturationSlider.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_sRect[9], this, IDC_IMGCTRL_SLIDER);
	}
    // For HR Camera

	SetVideoAttributes(MEDIUMEYE);
}

//***************************************************************************************

void CImgCptCtrlWnd::ShowGUI()
{
 	m_ShowWindow = TRUE;

	m_VeryLightEyeRad.ShowWindow(SW_SHOW);
	m_LightEyeRad.ShowWindow(SW_SHOW);
	m_MediumEyeRad.ShowWindow(SW_SHOW);
	m_DarkEyeRad.ShowWindow(SW_SHOW);
	m_VeryDarkEyeRad.ShowWindow(SW_SHOW);

	m_WhiteLEDsPowerLevelSlider.ShowWindow(SW_SHOW);
	m_BrightnessSlider.ShowWindow(SW_SHOW);

	if (m_IsHRCamera) {
		m_RedSlider.ShowWindow(SW_SHOW);
		m_GreenSlider.ShowWindow(SW_SHOW);
		m_BlueSlider.ShowWindow(SW_SHOW);
	}
    else {
		m_ContrastSlider.ShowWindow(SW_SHOW);
		m_HueSlider.ShowWindow(SW_SHOW);
		m_SaturationSlider.ShowWindow(SW_SHOW);
    }

	Label();
}

//***************************************************************************************

void CImgCptCtrlWnd::HideGUI()
{
	m_ShowWindow = FALSE;

	m_VeryLightEyeRad.ShowWindow(SW_HIDE);
	m_LightEyeRad.ShowWindow(SW_HIDE);
	m_MediumEyeRad.ShowWindow(SW_HIDE);
	m_DarkEyeRad.ShowWindow(SW_HIDE);
	m_VeryDarkEyeRad.ShowWindow(SW_HIDE);

	m_WhiteLEDsPowerLevelSlider.ShowWindow(SW_HIDE);
	m_BrightnessSlider.ShowWindow(SW_HIDE);

	if (m_IsHRCamera) {
      m_RedSlider.ShowWindow(SW_HIDE);
      m_GreenSlider.ShowWindow(SW_HIDE);
      m_BlueSlider.ShowWindow(SW_HIDE);
	 }
	else {
      m_ContrastSlider.ShowWindow(SW_HIDE);
      m_HueSlider.ShowWindow(SW_HIDE);
      m_SaturationSlider.ShowWindow(SW_HIDE);
	}
}

//***************************************************************************************

void CImgCptCtrlWnd::Label()
{
	CMFont Font(m_FontSize, 400, "Arial");

	RECT Rect;
	::SetRect(&Rect, 20, 1, m_w - 1, m_h - 1);
	m_MemDC.DrawRectangle(Rect, WHITE, NOCOLOR);
	InvalidateRect(&Rect, FALSE);

	m_MemDC.WriteText("White LEDS", m_lRect[0], Font, WHITE, 0, BLACK);
	m_MemDC.WriteText("Brightness", m_lRect[1], Font, WHITE, 0, BLACK);
	m_MemDC.WriteText("Contrast", m_lRect[2], Font, WHITE, 0, BLACK);

	if (m_IsHRCamera) {
      m_MemDC.WriteText("Red", m_lRect[3], Font, WHITE, 0, BLACK);
      m_MemDC.WriteText("Green", m_lRect[4], Font, WHITE, 0, BLACK);
      m_MemDC.WriteText("Blue", m_lRect[5], Font, WHITE, 0, BLACK);
    }
    else {
      m_MemDC.WriteText("Hue", m_lRect[3], Font, WHITE, 0, BLACK);
      m_MemDC.WriteText("Saturation", m_lRect[4], Font, WHITE, 0, BLACK);
	}

	if (m_VeryLightEyeRad.GetCheck())
	{
		 SetTitle(VERYLIGHTEYE);		
	}
	else if (m_LightEyeRad.GetCheck())
	{
		 SetTitle(LIGHTEYE);		
	}
	else if (m_MediumEyeRad.GetCheck())
	{
		 SetTitle(MEDIUMEYE);		
	}
	else if (m_DarkEyeRad.GetCheck())
	{
		 SetTitle(DARKEYE);		
	}
	else if (m_VeryDarkEyeRad.GetCheck())
	{
		 SetTitle(VERYDARKEYE);		
	}

    for (int i = 0; i < 6; i++) 
    {
        m_MemDC.FillSolidRect(m_rRect[i].left, m_rRect[i].top, m_rRect[i].right - m_rRect[i].left, m_rRect[i].bottom - m_rRect[i].top, BLACK);

        if (i != 5)      m_MemDC.WriteText(m_Titles[i], m_rRect[i], Font, WHITE, 2, BLACK);
        else 
        {
          if (m_IsHRCamera)  m_MemDC.WriteText(m_Titles[5], m_rRect[5], Font, WHITE, 2, BLACK);
        }

        InvalidateRect(&m_lRect[i], FALSE);
        InvalidateRect(&m_rRect[i], FALSE);
    }
}

//***************************************************************************************

void CImgCptCtrlWnd::SetTitle(int EyeType)
{
  for (int i = 0; i < 6; i++) {
    switch (EyeType) {
    case VERYLIGHTEYE:
      m_Titles[i] = m_IsHRCamera ? S_HRVeryLight[i] : S_NormalVeryLight[i];
      break;
    case LIGHTEYE:
      m_Titles[i] = m_IsHRCamera ? S_HRLight[i] : S_NormalLight[i];
      break;
    case MEDIUMEYE:
      m_Titles[i] = m_IsHRCamera ? S_HRMedium[i] : S_NormalMedium[i];
      break;
    case DARKEYE:
      m_Titles[i] = m_IsHRCamera ? S_HRDark[i] : S_NormalDark[i];
      break;
    case VERYDARKEYE:
      m_Titles[i] = m_IsHRCamera ? S_HRVeryDark[i] : S_NormalVeryDark[i];
      break;
    }
  }
}

//***************************************************************************************

void CImgCptCtrlWnd::RepaintMemDC()
{
	// ClearMemDC();
}

//***************************************************************************************

void CImgCptCtrlWnd::SetVideoAttributes(int EyeType)
{
  if (EyeType == VERYLIGHTEYE) {
    if (m_IsHRCamera) {
      m_WhiteLEDsPower = S_HRVeryLightVals[0];
      m_Brightness     = S_HRVeryLightVals[1];
      m_Contrast       = S_HRVeryLightVals[2];
      m_Red            = S_HRVeryLightVals[3];
      m_Green          = S_HRVeryLightVals[4];
      m_Blue           = S_HRVeryLightVals[5];
    }
    else {
      m_WhiteLEDsPower = S_NormalVeryLightVals[0];
      m_Brightness     = S_NormalVeryLightVals[1];
      m_Contrast       = S_NormalVeryLightVals[2];
      m_Hue            = S_NormalVeryLightVals[3];
      m_Saturation     = S_NormalVeryLightVals[4];
    }
  }
  else if (EyeType == LIGHTEYE) {
    if (m_IsHRCamera) {
      m_WhiteLEDsPower = S_HRLightVals[0];
      m_Brightness     = S_HRLightVals[1];
      m_Contrast       = S_HRLightVals[2];
      m_Red            = S_HRLightVals[3];
      m_Green          = S_HRLightVals[4];
      m_Blue           = S_HRLightVals[5];
    }
    else {
      m_WhiteLEDsPower = S_NormalLightVals[0];
      m_Brightness     = S_NormalLightVals[1];
      m_Contrast       = S_NormalLightVals[2];
      m_Hue            = S_NormalLightVals[3];
      m_Saturation     = S_NormalLightVals[4];
    }
  }
  else if (EyeType == MEDIUMEYE) {
    if (m_IsHRCamera) {
      m_WhiteLEDsPower = S_HRMediumVals[0];
      m_Brightness     = S_HRMediumVals[1];
      m_Contrast       = S_HRMediumVals[2];
      m_Red            = S_HRMediumVals[3];
      m_Green          = S_HRMediumVals[4];
      m_Blue           = S_HRMediumVals[5];
    }
    else {
      m_WhiteLEDsPower = S_NormalMediumVals[0];
      m_Brightness     = S_NormalMediumVals[1];
      m_Contrast       = S_NormalMediumVals[2];
      m_Hue            = S_NormalMediumVals[3];
      m_Saturation     = S_NormalMediumVals[4];
    }
  }
  else if (EyeType == DARKEYE) {
    if (m_IsHRCamera) {
      m_WhiteLEDsPower = S_HRDarkVals[0];
      m_Brightness     = S_HRDarkVals[1];
      m_Contrast       = S_HRDarkVals[2];
      m_Red            = S_HRDarkVals[3];
      m_Green          = S_HRDarkVals[4];
      m_Blue           = S_HRDarkVals[5];
    }
    else {
      m_WhiteLEDsPower = S_NormalDarkVals[0];
      m_Brightness     = S_NormalDarkVals[1];
      m_Contrast       = S_NormalDarkVals[2];
      m_Hue            = S_NormalDarkVals[3];
      m_Saturation     = S_NormalDarkVals[4];
    }
  }
  else if (EyeType == VERYDARKEYE) {
    if (m_IsHRCamera) {
      m_WhiteLEDsPower = S_HRVeryDarkVals[0];
      m_Brightness     = S_HRVeryLightVals[1];
      m_Contrast       = S_HRVeryLightVals[2];
      m_Red            = S_HRVeryLightVals[3];
      m_Green          = S_HRVeryLightVals[4];
      m_Blue           = S_HRVeryLightVals[5];
    }
    else {
      m_WhiteLEDsPower = S_NormalVeryDarkVals[0];
      m_Brightness     = S_NormalVeryDarkVals[1];
      m_Contrast       = S_NormalVeryDarkVals[2];
      m_Hue            = S_NormalVeryDarkVals[3];
      m_Saturation     = S_NormalVeryDarkVals[4];
    }
  }

  m_WhiteLEDsPowerLevelSlider.m_Pos = real_t(m_WhiteLEDsPower) / 50.0;
  m_BrightnessSlider.m_Pos          = real_t(m_Brightness) / 255.0;
  m_ContrastSlider.m_Pos            = real_t(m_Contrast) / 255.0;

  m_WhiteLEDsPowerLevelSlider.RePaint();
  m_BrightnessSlider.RePaint();
  m_ContrastSlider.RePaint();

  if (m_IsHRCamera) {
    m_RedSlider.m_Pos   = real_t(m_Red) / 255.0;
    m_GreenSlider.m_Pos = real_t(m_Green) / 255.0;
    m_BlueSlider.m_Pos  = real_t(m_Blue) / 255.0;

	m_RedSlider.RePaint();
    m_GreenSlider.RePaint();
    m_BlueSlider.RePaint();
  }
  else {
    m_HueSlider.m_Pos        = real_t(m_Hue) / 255.0;
    m_SaturationSlider.m_Pos = real_t(m_Saturation) / 255.0;

	m_HueSlider.RePaint();
    m_SaturationSlider.RePaint();
  }
}

//***************************************************************************************
void CImgCptCtrlWnd::OnVLightEyeRadioClicked()
{
	if (!m_VeryLightEyeRad.GetCheck())
	{
		SetVideoAttributes(VERYLIGHTEYE);
		SetEyeChecks(m_VeryLightStatus);
		Label();
	}
}

//***************************************************************************************

void CImgCptCtrlWnd::SetEyeChecks(BOOL CheckStatus[5])
{
  m_VeryLightEyeRad.SetCheck(CheckStatus[0]);
  m_VeryLightEyeRad.Repaint();

  m_LightEyeRad.SetCheck(CheckStatus[1]);
  m_LightEyeRad.Repaint();

  m_MediumEyeRad.SetCheck(CheckStatus[2]);
  m_MediumEyeRad.Repaint();

  m_DarkEyeRad.SetCheck(CheckStatus[3]);
  m_DarkEyeRad.Repaint();

  m_VeryDarkEyeRad.SetCheck(CheckStatus[4]);
  m_VeryDarkEyeRad.Repaint();
}

//***************************************************************************************
void CImgCptCtrlWnd::OnLightEyeRadioClicked()
{
	if (!m_LightEyeRad.GetCheck())
	{ 
		SetVideoAttributes(LIGHTEYE);		
		SetEyeChecks(m_LightStatus);

		Label();
	}
}

//***************************************************************************************
void CImgCptCtrlWnd::OnMediumEyeRadioClicked()
{
	if (!m_MediumEyeRad.GetCheck())
	{
		SetVideoAttributes(MEDIUMEYE);
		SetEyeChecks(m_MediumStatus);

		Label();
	}
}

//***************************************************************************************
void CImgCptCtrlWnd::OnDarkEyeRadioClicked()
{
	if (!m_DarkEyeRad.GetCheck())
	{
		SetVideoAttributes(DARKEYE);
		SetEyeChecks(m_DarkStatus);

		Label();
	}
}

//***************************************************************************************
void CImgCptCtrlWnd::OnVeryDarkEyeRadioClicked()
{
	if (!m_VeryDarkEyeRad.GetCheck())
	{
		SetVideoAttributes(VERYDARKEYE);
		SetEyeChecks(m_VeryDarkStatus);

		Label();
	}
}
//***************************************************************************************

LRESULT CImgCptCtrlWnd::OnSliderChangedPos(WPARAM wParam, LPARAM lParam)
{
  if (m_VeryLightEyeRad.GetCheck()) {
    m_VeryLightEyeRad.SetCheck(FALSE);
    m_VeryLightEyeRad.Repaint();
  }

  if (m_LightEyeRad.GetCheck()) {
    m_LightEyeRad.SetCheck(FALSE);
    m_LightEyeRad.Repaint();
  }

  if (m_MediumEyeRad.GetCheck()) {
    m_MediumEyeRad.SetCheck(FALSE);
    m_MediumEyeRad.Repaint();
  }

  if (m_DarkEyeRad.GetCheck()) {
    m_DarkEyeRad.SetCheck(FALSE);
    m_DarkEyeRad.Repaint();
  }

  if (m_VeryDarkEyeRad.GetCheck()) {
    m_VeryDarkEyeRad.SetCheck(FALSE);
    m_VeryDarkEyeRad.Repaint();
  }

  //
  CPoint point;
  GetCursorPos(&point);

  CString s;
  CMFont  Font(m_FontSize, 400, "Arial");
  int     y = point.y - m_ScreenTop;

  if (y >= m_sRect[5].top && y <= m_sRect[5].bottom) {
    if (m_WhiteLEDsPowerLevelSlider.m_Pos > 0.99)
      m_WhiteLEDsPowerLevelSlider.m_Pos = 1;
    int val = intRound(m_WhiteLEDsPowerLevelSlider.m_Pos * 50.0);
    s.Format(_T("%i/50"), val);

    m_MemDC.FillSolidRect(m_rRect[0].left, m_rRect[0].top, m_rRect[0].right - m_rRect[0].left, m_rRect[0].bottom - m_rRect[0].top, BLACK);
    m_MemDC.WriteText(s, m_rRect[0], Font, WHITE, 2, BLACK);
    InvalidateRect(&m_rRect[0], FALSE);

    m_WhiteLEDsPower = val;
  }
  else if (y >= m_sRect[6].top && y <= m_sRect[6].bottom) {
    if (m_BrightnessSlider.m_Pos > 0.993)
      m_BrightnessSlider.m_Pos = 1;
    int val = intRound(m_BrightnessSlider.m_Pos * 255.0);
    s.Format(_T("%i/255"), val);

    m_MemDC.FillSolidRect(m_rRect[1].left, m_rRect[1].top, m_rRect[1].right - m_rRect[1].left, m_rRect[1].bottom - m_rRect[1].top, BLACK);
    m_MemDC.WriteText(s, m_rRect[1], Font, WHITE, 2, BLACK);
    InvalidateRect(&m_rRect[1], FALSE);

    m_Brightness = val;
  }
  else if (y >= m_sRect[7].top && y <= m_sRect[7].bottom) {
    if (m_ContrastSlider.m_Pos > 0.993)
      m_ContrastSlider.m_Pos = 1;
    int val = intRound(m_ContrastSlider.m_Pos * 255.0);
    s.Format(_T("%i/255"), val);

    m_MemDC.FillSolidRect(m_rRect[2].left, m_rRect[2].top, m_rRect[2].right - m_rRect[2].left, m_rRect[2].bottom - m_rRect[2].top, BLACK);
    m_MemDC.WriteText(s, m_rRect[2], Font, WHITE, 2, BLACK);
    InvalidateRect(&m_rRect[2], FALSE);

    m_Contrast = val;
  }
  else if (y >= m_sRect[8].top && y <= m_sRect[8].bottom) {
    if (m_IsHRCamera) {
      if (m_RedSlider.m_Pos > 0.993)
        m_RedSlider.m_Pos = 1;
      int val = intRound(m_RedSlider.m_Pos * 255.0);
      s.Format(_T("%i/255"), val);

      m_MemDC.FillSolidRect(m_rRect[3].left, m_rRect[3].top, m_rRect[3].right - m_rRect[3].left, m_rRect[3].bottom - m_rRect[3].top, BLACK);
      m_MemDC.WriteText(s, m_rRect[3], Font, WHITE, 2, BLACK);
      InvalidateRect(&m_rRect[3], FALSE);

      m_Red = val;
    }
    else {
      if (m_HueSlider.m_Pos > 0.993)
        m_HueSlider.m_Pos = 1;
      int val = intRound(m_HueSlider.m_Pos * 255.0);
      s.Format(_T("%i/255"), val);

      m_MemDC.FillSolidRect(m_rRect[3].left, m_rRect[3].top, m_rRect[3].right - m_rRect[3].left, m_rRect[3].bottom - m_rRect[3].top, BLACK);
      m_MemDC.WriteText(s, m_rRect[3], Font, WHITE, 2, BLACK);
      InvalidateRect(&m_rRect[3], FALSE);

      m_Hue = val;
    }
  }
  else if (y >= m_sRect[9].top && y <= m_sRect[9].bottom) {
    if (m_IsHRCamera) {
      if (m_GreenSlider.m_Pos > 0.993)
        m_GreenSlider.m_Pos = 1;
      int val = intRound(m_GreenSlider.m_Pos * 255.0);
      s.Format(_T("%i/255"), val);

      m_MemDC.FillSolidRect(m_rRect[4].left, m_rRect[4].top, m_rRect[4].right - m_rRect[4].left, m_rRect[4].bottom - m_rRect[4].top, BLACK);
      m_MemDC.WriteText(s, m_rRect[4], Font, WHITE, 2, BLACK);
      InvalidateRect(&m_rRect[4], FALSE);

      m_Green = val;
    }
    else {
      if (m_SaturationSlider.m_Pos > 0.993)
        m_SaturationSlider.m_Pos = 1;

      int val = intRound(m_SaturationSlider.m_Pos * 255.0);
      s.Format(_T("%i/255"), val);

      m_MemDC.FillSolidRect(m_rRect[4].left, m_rRect[4].top, m_rRect[4].right - m_rRect[4].left, m_rRect[4].bottom - m_rRect[4].top, BLACK);
      m_MemDC.WriteText(s, m_rRect[4], Font, WHITE, 2, BLACK);
      InvalidateRect(&m_rRect[4], FALSE);

      m_Saturation = val;
    }
  }
  else if (y >= m_sRect[10].top && y <= m_sRect[10].bottom) {
      if (m_BlueSlider.m_Pos > 0.993)
      m_BlueSlider.m_Pos = 1;
      int val = intRound(m_BlueSlider.m_Pos * 255.0);
      s.Format(_T("%i/255"), val);

      m_MemDC.FillSolidRect(m_rRect[5].left, m_rRect[5].top, m_rRect[5].right - m_rRect[5].left, m_rRect[5].bottom - m_rRect[5].top, BLACK);
      m_MemDC.WriteText(s, m_rRect[5], Font, WHITE, 2, BLACK);
      InvalidateRect(&m_rRect[5], FALSE);

      m_Blue = val;
  }

  return 0;
}

//***************************************************************************************
void CImgCptCtrlWnd::ShowAcqBtnGUI()
{
	m_ShowAcqBtn = TRUE;

	m_CapBtn.ShowWindow(SW_SHOW);
}

//***************************************************************************************

void CImgCptCtrlWnd::HideAcqBtnGUI()
{
	m_ShowAcqBtn = FALSE;

	m_CapBtn.ShowWindow(SW_HIDE);
}

//***************************************************************************************
void CImgCptCtrlWnd::OnCaptureBtnClicked()
{
	m_Acquire = TRUE;
}