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
	//int a = 0;
}
//***************************************************************************************
void CImgCptCtrlWnd::CreateThisWnd(RECT& Rect, CWnd* pWnd, int ScreenTop, int CapHeight)
{
	CreateWnd(Rect, pWnd);

	m_ScreenTop = ScreenTop;

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
		int q = W == 1920 ? 2 : (W == 1600 ? 1 : 0);

		RECT tempRect;
		::SetRect(&tempRect, 2 * Left_w, m_h + top_h, 2 * Left_w + BtnW, m_h + top_h + BtnW);
		m_CapBtn.Create(_T(""), _T(""), 0, tempRect, this, IDC_CAPTURE_BTN);

		m_CapBtn.SetImage(q == 2 ? IDR_ACQUISITION_3 : q == 1 ? IDR_ACQUISITION_2 : IDR_ACQUISITION_2);
		m_CapBtn.SetBk(FALSE);
		m_CapBtn.SetBkColor(BLACK);
		//530 New Remmber add into Presbia
	}
	//Acq Btn

	int sh = 10;
	m_eH = intRound(real_t(m_h - 130) / 10);

	m_FontSize = intRound(0.5*m_eH);

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

	m_WhiteLEDsPowerLevelSlider.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_sRect[5], this, IDC_MTF_ANGLE_SLIDER);
	m_BrightnessSlider.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_sRect[6], this, IDC_MTF_ANGLE_SLIDER);
	m_ContrastSlider.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_sRect[7], this, IDC_MTF_ANGLE_SLIDER);
	m_HueSlider.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_sRect[8], this, IDC_MTF_ANGLE_SLIDER);
	m_SaturationSlider.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_sRect[9], this, IDC_MTF_ANGLE_SLIDER);

	//m_WhiteLEDsPowerLevelSlider.m_Pos = 0.1;
	//m_BrightnessSlider.m_Pos = 95.0 / 255.0;
	//m_ContrastSlider.m_Pos = 1;
	//m_HueSlider.m_Pos = 22.0 / 255.0;
	//m_SaturationSlider.m_Pos = 1;

	//m_WhiteLEDsPower = 10;
	//m_Brightness = 95;
	//m_Contrast = 255;
	//m_Hue = 22;
	//m_Saturation = 255;

	// 7.0.0 HireCamera
  m_WhiteLEDsPowerLevelSlider.m_Pos = 30.0 / 50.0;
  m_BrightnessSlider.m_Pos          = 140.0 / 255.0;
  m_ContrastSlider.m_Pos            = 200.0 / 255.0;
  m_HueSlider.m_Pos                 = 145.0 / 255.0;
  m_SaturationSlider.m_Pos          = 90.0 / 255.0;

  m_WhiteLEDsPower = 30;
  m_Brightness     = 140;
  m_Contrast       = 200;
  m_Hue            = 145;
  m_Saturation     = 90;
  // 7.0.0 HireCamera
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
	m_ContrastSlider.ShowWindow(SW_SHOW);
	m_HueSlider.ShowWindow(SW_SHOW);
	m_SaturationSlider.ShowWindow(SW_SHOW);

	Label0();
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
	m_ContrastSlider.ShowWindow(SW_HIDE);
	m_HueSlider.ShowWindow(SW_HIDE);
	m_SaturationSlider.ShowWindow(SW_HIDE);
}

//***************************************************************************************

void CImgCptCtrlWnd::Label0()
{
	CMFont Font(m_FontSize, 400, "Arial");

	RECT Rect;
	::SetRect(&Rect, 20, 1, m_w - 1, m_h - 1);
	m_MemDC.DrawRectangle(Rect, WHITE, NOCOLOR);
	InvalidateRect(&Rect, FALSE);

	m_MemDC.WriteText("White LEDS", m_lRect[0], Font, WHITE, 0, BLACK);
	m_MemDC.WriteText("Brightness", m_lRect[1], Font, WHITE, 0, BLACK);
	m_MemDC.WriteText("Contrast", m_lRect[2], Font, WHITE, 0, BLACK);
	m_MemDC.WriteText("Hue", m_lRect[3], Font, WHITE, 0, BLACK);
	m_MemDC.WriteText("Saturation", m_lRect[4], Font, WHITE, 0, BLACK);

	CString s[5];
	if (m_VeryLightEyeRad.GetCheck())
	{
		//s[0] = "9/100";
		//s[1] = "65/255";
		//s[2] = "247/255";
		//s[3] = "22/255";
		//s[4] = "255/255";

		// 7.0.0 HireCamera
		s[0] = "30/50";
		s[1] = "140/255";
		s[2] = "200/255";
		s[3] = "145/255";
		s[4] = "90/255";
		// 7.0.0 HireCamera
	}
	else if (m_LightEyeRad.GetCheck())
	{
		//s[0] = "11/100";
		//s[1] = "80/255";
		//s[2] = "245/255";
		//s[3] = "22/255";
		//s[4] = "255/255";

		// 7.0.0 HireCamera
    s[0] = "11/50";
    s[1] = "125/255";
    s[2] = "200/255";
    s[3] = "42/255";
    s[4] = "130/255";
    // 7.0.0 HireCamera
	}
	else if (m_MediumEyeRad.GetCheck())
	{
		//s[0] = "10/100";
		//s[1] = "95/255";
		//s[2] = "255/255";
		//s[3] = "22/255";
		//s[4] = "255/255";

    // 7.0.0 HireCamera
    s[0] = "30/50";
    s[1] = "140/255";
    s[2] = "200/255";
    s[3] = "145/255";
    s[4] = "90/255";
    // 7.0.0 HireCamera
	}
	else if (m_DarkEyeRad.GetCheck())
	{
		//s[0] = "10/100";
		//s[1] = "110/255";
		//s[2] = "255/255";
		//s[3] = "22/255";
		//s[4] = "255/255";


		// 7.0.0 HireCamera
    s[0] = "30/50";
    s[1] = "140/255";
    s[2] = "200/255";
    s[3] = "145/255";
    s[4] = "90/255";
    // 7.0.0 HireCamera
	}
	else if (m_VeryDarkEyeRad.GetCheck())
	{
		//s[0] = "9/100";
		//s[1] = "126/255";
		//s[2] = "255/255";
		//s[3] = "22/255";
		//s[4] = "255/255";


		// 7.0.0 HireCamera
    s[0] = "30/50";
    s[1] = "140/255";
    s[2] = "200/255";
    s[3] = "145/255";
    s[4] = "90/255";
    // 7.0.0 HireCamera
	}

	m_MemDC.WriteText(s[0], m_rRect[0], Font, WHITE, 2, BLACK);
	m_MemDC.WriteText(s[1], m_rRect[1], Font, WHITE, 2, BLACK);
	m_MemDC.WriteText(s[2], m_rRect[2], Font, WHITE, 2, BLACK);
	m_MemDC.WriteText(s[3], m_rRect[3], Font, WHITE, 2, BLACK);
	m_MemDC.WriteText(s[4], m_rRect[4], Font, WHITE, 2, BLACK);

	for (int i = 0; i < 5; i++)
	{
		InvalidateRect(&m_lRect[i], FALSE);
		InvalidateRect(&m_rRect[i], FALSE);
	}
}


//***************************************************************************************

void CImgCptCtrlWnd::Label1()
{
	CMFont Font(m_FontSize, 400, "Arial");

	CString s[5];
	if (m_VeryLightEyeRad.GetCheck())
	{
		//s[0] = "9/100";
		//s[1] = "65/255";
		//s[2] = "247/255";
		//s[3] = "22/255";
		//s[4] = "255/255";


		// 7.0.0 HireCamera
    s[0] = "30/50";
    s[1] = "140/255";
    s[2] = "200/255";
    s[3] = "145/255";
    s[4] = "90/255";
    // 7.0.0 HireCamera
	}
	else if (m_LightEyeRad.GetCheck())
	{
		//s[0] = "11/100";
		//s[1] = "80/255";
		//s[2] = "245/255";
		//s[3] = "22/255";
		//s[4] = "255/255";


		// 7.0.0 HireCamera
    s[0] = "30/50";
    s[1] = "140/255";
    s[2] = "200/255";
    s[3] = "145/255";
    s[4] = "90/255";
    // 7.0.0 HireCamera
	}
	else if (m_MediumEyeRad.GetCheck())
	{
	//	s[0] = "50/100";
	//	s[1] = "95/255";
	//	s[2] = "255/255";
	//	s[3] = "22/255";
	//	s[4] = "255/255";


		// 7.0.0 HireCamera
  s[0] = "30/50";
  s[1] = "140/255";
  s[2] = "200/255";
  s[3] = "145/255";
  s[4] = "90/255";
  // 7.0.0 HireCamera
	}
	else if (m_DarkEyeRad.GetCheck())
	{
		//s[0] = "10/100";
		//s[1] = "110/255";
		//s[2] = "255/255";
		//s[3] = "22/255";
		//s[4] = "255/255";


		// 7.0.0 HireCamera
    s[0] = "30/50";
    s[1] = "140/255";
    s[2] = "200/255";
    s[3] = "145/255";
    s[4] = "90/255";
    // 7.0.0 HireCamera
	}
	else if (m_VeryDarkEyeRad.GetCheck())
	{
		//s[0] = "9/100";
		//s[1] = "126/255";
		//s[2] = "255/255";
		//s[3] = "22/255";
		//s[4] = "255/255";


		// 7.0.0 HireCamera
    s[0] = "30/50";
    s[1] = "140/255";
    s[2] = "200/255";
    s[3] = "145/255";
    s[4] = "90/255";
    // 7.0.0 HireCamera
	}

	ClearMemDC();
	m_MemDC.WriteText(s[0], m_rRect[0], Font, WHITE, 2, BLACK);
	m_MemDC.WriteText(s[1], m_rRect[1], Font, WHITE, 2, BLACK);
	m_MemDC.WriteText(s[2], m_rRect[2], Font, WHITE, 2, BLACK);
	m_MemDC.WriteText(s[3], m_rRect[3], Font, WHITE, 2, BLACK);
	m_MemDC.WriteText(s[4], m_rRect[4], Font, WHITE, 2, BLACK);

	for (int i = 0; i < 5; i++)
	{
		InvalidateRect(&m_rRect[i], FALSE);
	}
}

//***************************************************************************************

void CImgCptCtrlWnd::RepaintMemDC()
{
	// ClearMemDC();
}

//***************************************************************************************
void CImgCptCtrlWnd::OnVLightEyeRadioClicked()
{
	if (!m_VeryLightEyeRad.GetCheck())
	{
		//m_WhiteLEDsPower = 9;
		//m_Brightness = 65;
		//m_Contrast = 247;
		//m_Hue = 22;
		//m_Saturation = 255;

		//m_WhiteLEDsPowerLevelSlider.m_Pos = 9.0 / 100.0;
		//m_BrightnessSlider.m_Pos = 65.0 / 255.0;
		//m_ContrastSlider.m_Pos = 247.0 / 255.0;
		//m_HueSlider.m_Pos = 22.0 / 255.0;
		//m_SaturationSlider.m_Pos = 1;

		//7.0.0 Hirecamera
		m_WhiteLEDsPower = 30;
		m_Brightness     = 140;
		m_Contrast       = 200;
		m_Hue            = 145;
		m_Saturation     = 90;

		m_WhiteLEDsPowerLevelSlider.m_Pos = 30 / 50.0;
		m_BrightnessSlider.m_Pos          = 140.0 / 255.0;
		m_ContrastSlider.m_Pos            = 200.0 / 255.0;
		m_HueSlider.m_Pos                 = 145.0 / 255.0;
    m_SaturationSlider.m_Pos			  = 90.0 / 255.0;
		// 7.0.0 Hirecamera

		m_WhiteLEDsPowerLevelSlider.RePaint();
		m_BrightnessSlider.RePaint();
		m_ContrastSlider.RePaint();
		m_HueSlider.RePaint();
		m_SaturationSlider.RePaint();

		m_VeryLightEyeRad.SetCheck(TRUE);
		m_VeryLightEyeRad.Repaint();

		if (m_LightEyeRad.GetCheck())
		{
			m_LightEyeRad.SetCheck(FALSE);
			m_LightEyeRad.Repaint();
		}

		if (m_MediumEyeRad.GetCheck())
		{
			m_MediumEyeRad.SetCheck(FALSE);
			m_MediumEyeRad.Repaint();
		}

		if (m_DarkEyeRad.GetCheck())
		{
			m_DarkEyeRad.SetCheck(FALSE);
			m_DarkEyeRad.Repaint();
		}

		if (m_VeryDarkEyeRad.GetCheck())
		{
			m_VeryDarkEyeRad.SetCheck(FALSE);
			m_VeryDarkEyeRad.Repaint();
		}

		Label1();
	}
}

//***************************************************************************************
void CImgCptCtrlWnd::OnLightEyeRadioClicked()
{
	if (!m_LightEyeRad.GetCheck())
	{
		/*m_WhiteLEDsPower = 11;
		m_Brightness = 80;
		m_Contrast = 245;
		m_Hue = 22;
		m_Saturation = 255;

		m_WhiteLEDsPowerLevelSlider.m_Pos = 11.0 / 100.0;
		m_BrightnessSlider.m_Pos = 80.0 / 255.0;
		m_ContrastSlider.m_Pos = 245.0 / 255.0;
		m_HueSlider.m_Pos = 22.0 / 255.0;
		m_SaturationSlider.m_Pos = 1;*/

		// 7.0.0 Hirecamera
    m_WhiteLEDsPower = 30;
    m_Brightness     = 140;
    m_Contrast       = 200;
    m_Hue            = 145;
    m_Saturation     = 90;

    m_WhiteLEDsPowerLevelSlider.m_Pos = 30 / 50.0;
    m_BrightnessSlider.m_Pos          = 140.0 / 255.0;
    m_ContrastSlider.m_Pos            = 200.0 / 255.0;
    m_HueSlider.m_Pos                 = 145.0 / 255.0;
    m_SaturationSlider.m_Pos          = 90.0 / 255.0;
    // 7.0.0 Hirecamera

		m_WhiteLEDsPowerLevelSlider.RePaint();
		m_BrightnessSlider.RePaint();
		m_ContrastSlider.RePaint();
		m_HueSlider.RePaint();
		m_SaturationSlider.RePaint();

		m_LightEyeRad.SetCheck(TRUE);
		m_LightEyeRad.Repaint();

		if (m_VeryLightEyeRad.GetCheck())
		{
			m_VeryLightEyeRad.SetCheck(FALSE);
			m_VeryLightEyeRad.Repaint();
		}

		if (m_MediumEyeRad.GetCheck())
		{
			m_MediumEyeRad.SetCheck(FALSE);
			m_MediumEyeRad.Repaint();
		}

		if (m_DarkEyeRad.GetCheck())
		{
			m_DarkEyeRad.SetCheck(FALSE);
			m_DarkEyeRad.Repaint();
		}

		if (m_VeryDarkEyeRad.GetCheck())
		{
			m_VeryDarkEyeRad.SetCheck(FALSE);
			m_VeryDarkEyeRad.Repaint();
		}

		Label1();
	}
}

//***************************************************************************************
void CImgCptCtrlWnd::OnMediumEyeRadioClicked()
{
	if (!m_MediumEyeRad.GetCheck())
	{
		/*m_WhiteLEDsPower = 10;
		m_Brightness = 95;
		m_Contrast = 255;
		m_Hue = 22;
		m_Saturation = 255;

		m_WhiteLEDsPowerLevelSlider.m_Pos = 0.1;
		m_BrightnessSlider.m_Pos = 95.0 / 255.0;
		m_ContrastSlider.m_Pos = 1;
		m_HueSlider.m_Pos = 22.0 / 255.0;
		m_SaturationSlider.m_Pos = 1;*/

		// 7.0.0 Hirecamera
    m_WhiteLEDsPower = 30;
    m_Brightness     = 140;
    m_Contrast       = 200;
    m_Hue            = 145;
    m_Saturation     = 90;

    m_WhiteLEDsPowerLevelSlider.m_Pos = 30 / 50.0;
    m_BrightnessSlider.m_Pos          = 140.0 / 255.0;
    m_ContrastSlider.m_Pos            = 200.0 / 255.0;
    m_HueSlider.m_Pos                 = 145.0 / 255.0;
    m_SaturationSlider.m_Pos          = 90.0 / 255.0;
    // 7.0.0 Hirecamera

		m_WhiteLEDsPowerLevelSlider.RePaint();
		m_BrightnessSlider.RePaint();
		m_ContrastSlider.RePaint();
		m_HueSlider.RePaint();
		m_SaturationSlider.RePaint();

		m_MediumEyeRad.SetCheck(TRUE);
		m_MediumEyeRad.Repaint();

		if (m_VeryLightEyeRad.GetCheck())
		{
			m_VeryLightEyeRad.SetCheck(FALSE);
			m_VeryLightEyeRad.Repaint();
		}

		if (m_LightEyeRad.GetCheck())
		{
			m_LightEyeRad.SetCheck(FALSE);
			m_LightEyeRad.Repaint();
		}

		if (m_DarkEyeRad.GetCheck())
		{
			m_DarkEyeRad.SetCheck(FALSE);
			m_DarkEyeRad.Repaint();
		}

		if (m_VeryDarkEyeRad.GetCheck())
		{
			m_VeryDarkEyeRad.SetCheck(FALSE);
			m_VeryDarkEyeRad.Repaint();
		}

		Label1();
	}
}

//***************************************************************************************
void CImgCptCtrlWnd::OnDarkEyeRadioClicked()
{
	if (!m_DarkEyeRad.GetCheck())
	{
		/*m_WhiteLEDsPower = 10;
		m_Brightness = 110;
		m_Contrast = 255;
		m_Hue = 2;
		m_Saturation = 255;

		m_WhiteLEDsPowerLevelSlider.m_Pos = 0.1;
		m_BrightnessSlider.m_Pos = 110.0 / 255.0;
		m_ContrastSlider.m_Pos = 1;
		m_HueSlider.m_Pos = 22.0 / 255.0;
		m_SaturationSlider.m_Pos = 1;*/

		// 7.0.0 Hirecamera
    m_WhiteLEDsPower = 30;
    m_Brightness     = 140;
    m_Contrast       = 200;
    m_Hue            = 145;
    m_Saturation     = 90;

    m_WhiteLEDsPowerLevelSlider.m_Pos = 30.0 / 50.0;
    m_BrightnessSlider.m_Pos          = 140.0 / 255.0;
    m_ContrastSlider.m_Pos            = 200.0 / 255.0;
    m_HueSlider.m_Pos                 = 145.0 / 255.0;
    m_SaturationSlider.m_Pos          = 90.0 / 255.0;
    // 7.0.0 Hirecamera

		m_WhiteLEDsPowerLevelSlider.RePaint();
		m_BrightnessSlider.RePaint();
		m_ContrastSlider.RePaint();
		m_HueSlider.RePaint();
		m_SaturationSlider.RePaint();

		m_DarkEyeRad.SetCheck(TRUE);
		m_DarkEyeRad.Repaint();

		if (m_VeryLightEyeRad.GetCheck())
		{
			m_VeryLightEyeRad.SetCheck(FALSE);
			m_VeryLightEyeRad.Repaint();
		}

		if (m_LightEyeRad.GetCheck())
		{
			m_LightEyeRad.SetCheck(FALSE);
			m_LightEyeRad.Repaint();
		}

		if (m_MediumEyeRad.GetCheck())
		{
			m_MediumEyeRad.SetCheck(FALSE);
			m_MediumEyeRad.Repaint();
		}

		if (m_VeryDarkEyeRad.GetCheck())
		{
			m_VeryDarkEyeRad.SetCheck(FALSE);
			m_VeryDarkEyeRad.Repaint();
		}

		Label1();
	}
}

//***************************************************************************************
void CImgCptCtrlWnd::OnVeryDarkEyeRadioClicked()
{
	if (!m_VeryDarkEyeRad.GetCheck())
	{
		/*m_WhiteLEDsPower = 9;
		m_Brightness = 126;
		m_Contrast = 255;
		m_Hue = 22;
		m_Saturation = 255;

		m_WhiteLEDsPowerLevelSlider.m_Pos = 9.0 / 100.0;
		m_BrightnessSlider.m_Pos = 126.0 / 255.0;
		m_ContrastSlider.m_Pos = 1;
		m_HueSlider.m_Pos = 22.0 / 255.0;
		m_SaturationSlider.m_Pos = 1;*/

		// 7.0.0 Hirecamera
    m_WhiteLEDsPower = 30;
    m_Brightness     = 140;
    m_Contrast       = 200;
    m_Hue            = 145;
    m_Saturation     = 90;

    m_WhiteLEDsPowerLevelSlider.m_Pos = 30.0 / 50.0;
    m_BrightnessSlider.m_Pos          = 140.0 / 255.0;
    m_ContrastSlider.m_Pos            = 200.0 / 255.0;
    m_HueSlider.m_Pos                 = 145.0 / 255.0;
    m_SaturationSlider.m_Pos          = 90.0 / 255.0;
    // 7.0.0 Hirecamera

		m_WhiteLEDsPowerLevelSlider.RePaint();
		m_BrightnessSlider.RePaint();
		m_ContrastSlider.RePaint();
		m_HueSlider.RePaint();
		m_SaturationSlider.RePaint();

		m_VeryDarkEyeRad.SetCheck(TRUE);
		m_VeryDarkEyeRad.Repaint();

		if (m_VeryLightEyeRad.GetCheck())
		{
			m_VeryLightEyeRad.SetCheck(FALSE);
			m_VeryLightEyeRad.Repaint();
		}

		if (m_LightEyeRad.GetCheck())
		{
			m_LightEyeRad.SetCheck(FALSE);
			m_LightEyeRad.Repaint();
		}

		if (m_MediumEyeRad.GetCheck())
		{
			m_MediumEyeRad.SetCheck(FALSE);
			m_MediumEyeRad.Repaint();
		}

		if (m_DarkEyeRad.GetCheck())
		{
			m_DarkEyeRad.SetCheck(FALSE);
			m_DarkEyeRad.Repaint();
		}

		Label1();
	}
}
//***************************************************************************************

LRESULT CImgCptCtrlWnd::OnSliderChangedPos(WPARAM wParam, LPARAM lParam)
{
	if (m_VeryLightEyeRad.GetCheck())
	{
		m_VeryLightEyeRad.SetCheck(FALSE);
		m_VeryLightEyeRad.Repaint();
	}

	if (m_LightEyeRad.GetCheck())
	{
		m_LightEyeRad.SetCheck(FALSE);
		m_LightEyeRad.Repaint();
	}

	if (m_MediumEyeRad.GetCheck())
	{
		m_MediumEyeRad.SetCheck(FALSE);
		m_MediumEyeRad.Repaint();
	}

	if (m_DarkEyeRad.GetCheck())
	{
		m_DarkEyeRad.SetCheck(FALSE);
		m_DarkEyeRad.Repaint();
	}

	if (m_VeryDarkEyeRad.GetCheck())
	{
		m_VeryDarkEyeRad.SetCheck(FALSE);
		m_VeryDarkEyeRad.Repaint();
	}

	//
	CPoint point;
	GetCursorPos(&point);

	CString s;
	CMFont Font(m_FontSize, 400, "Arial");
	int y = point.y - m_ScreenTop;

	if (y >= m_sRect[5].top && y <= m_sRect[5].bottom)
	{
		if (m_WhiteLEDsPowerLevelSlider.m_Pos > 0.99) m_WhiteLEDsPowerLevelSlider.m_Pos = 1;
		int val = intRound(m_WhiteLEDsPowerLevelSlider.m_Pos * 100.0);
		s.Format(_T("%i/100"), val);

		m_MemDC.FillSolidRect(m_rRect[0].left, m_rRect[0].top, m_rRect[0].right - m_rRect[0].left, m_rRect[0].bottom - m_rRect[0].top, BLACK);
		m_MemDC.WriteText(s, m_rRect[0], Font, WHITE, 2, BLACK);
		InvalidateRect(&m_rRect[0], FALSE);

		m_WhiteLEDsPower = val;
	}
	else if (y >= m_sRect[6].top && y <= m_sRect[6].bottom)
	{
		if (m_BrightnessSlider.m_Pos > 0.993) m_BrightnessSlider.m_Pos = 1;
		int val = intRound(m_BrightnessSlider.m_Pos * 255.0);
		s.Format(_T("%i/255"), val);

		m_MemDC.FillSolidRect(m_rRect[1].left, m_rRect[1].top, m_rRect[1].right - m_rRect[1].left, m_rRect[1].bottom - m_rRect[1].top, BLACK);
		m_MemDC.WriteText(s, m_rRect[1], Font, WHITE, 2, BLACK);
		InvalidateRect(&m_rRect[1], FALSE);

		m_Brightness = val;
	}
	else if (y >= m_sRect[7].top && y <= m_sRect[7].bottom)
	{
		if (m_ContrastSlider.m_Pos > 0.993) m_ContrastSlider.m_Pos = 1;
		int val = intRound(m_ContrastSlider.m_Pos * 255.0);
		s.Format(_T("%i/255"), val);

		m_MemDC.FillSolidRect(m_rRect[2].left, m_rRect[2].top, m_rRect[2].right - m_rRect[2].left, m_rRect[2].bottom - m_rRect[2].top, BLACK);
		m_MemDC.WriteText(s, m_rRect[2], Font, WHITE, 2, BLACK);
		InvalidateRect(&m_rRect[2], FALSE);

		m_Contrast = val;
	}
	else if (y >= m_sRect[8].top && y <= m_sRect[8].bottom)
	{
		if (m_HueSlider.m_Pos > 0.993) m_HueSlider.m_Pos = 1;
		int val = intRound(m_HueSlider.m_Pos * 255.0);
		s.Format(_T("%i/255"), val);

		m_MemDC.FillSolidRect(m_rRect[3].left, m_rRect[3].top, m_rRect[3].right - m_rRect[3].left, m_rRect[3].bottom - m_rRect[3].top, BLACK);
		m_MemDC.WriteText(s, m_rRect[3], Font, WHITE, 2, BLACK);
		InvalidateRect(&m_rRect[3], FALSE);

		m_Hue = val;
	}
	else if (y >= m_sRect[9].top && y <= m_sRect[9].bottom)
	{
		if (m_SaturationSlider.m_Pos > 0.993) m_SaturationSlider.m_Pos = 1;

		int val = intRound(m_SaturationSlider.m_Pos * 255.0);
		s.Format(_T("%i/255"), val);

		m_MemDC.FillSolidRect(m_rRect[4].left, m_rRect[4].top, m_rRect[4].right - m_rRect[4].left, m_rRect[4].bottom - m_rRect[4].top, BLACK);
		m_MemDC.WriteText(s, m_rRect[4], Font, WHITE, 2, BLACK);
		InvalidateRect(&m_rRect[4], FALSE);

		m_Saturation = val;
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