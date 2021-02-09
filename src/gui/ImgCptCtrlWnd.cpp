// 530 manual color image capture controller window

//***************************************************************************************

//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "ImgCptCtrlWnd.h"

namespace {
  enum EyeColor {
    VERYLIGHTEYE,
    LIGHTEYE,
    MEDIUMEYE,
    DARKEYE,
    VERYDARKEYE,
  };

  std::array eyeColorNames = {"Very Light Eye", "Light Eye", "Medium Eye", "Dark Eye", "Very Dark Eye"};

  using Settings = CImgCptCtrlWnd::Settings;

  // clang-format off
  static Settings stdSettings[] = {
  // led, brightness, contrast, hue, saturation, red, green, blue
    { 9,  65,         247,      22,  255,         80,  80,    80},  // VERYLIGHTEYE
    {11,  80,         245,      22,  255,         90,  90,    90},  // LIGHTEYE
    {10,  95,         255,      22,  255,        100, 100,   100},  // MEDIUMEYE
    {10, 110,         255,      22,  255,        100, 100,   100},  // DARKEYE
    { 9, 126,         255,      22,  255,        100, 100,   100},  // VERYDARKEYE
  };
  // clang-format on
} // namespace

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
CImgCptCtrlWnd::CImgCptCtrlWnd()
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

  // Acq Btn
  {
    m_Acquire = FALSE;

    int Left_w = intRound(real_t(CapHeight) / 5);
    int BtnW   = Left_w;
    int top_h  = intRound(real_t(m_w) / 10);

    // 530 New Remmber add into Presbia
    int W = ::GetSystemMetrics(SM_CXSCREEN);
    int q = W >= 1920 ? 2 : (W >= 1600 ? 1 : 0);

    RECT tempRect;
    ::SetRect(&tempRect, 2 * Left_w, m_h + top_h, 2 * Left_w + BtnW, m_h + top_h + BtnW);
    m_CapBtn.Create(_T(""), _T(""), 0, tempRect, this, IDC_CAPTURE_BTN);

    m_CapBtn.SetImage(q == 2 ? IDR_ACQUISITION_3 : q == 1 ? IDR_ACQUISITION_2 : IDR_ACQUISITION_2);
    m_CapBtn.SetBk(FALSE);
    m_CapBtn.SetBkColor(BLACK);
    // 530 New Remmber add into Presbia
  }
  // Acq Btn

  int    sh            = 10;
  real_t StepH         = m_IsHRCamera ? 12 : 10;
  real_t FontSizeRatio = m_IsHRCamera ? 0.7 : 0.5;

  m_eH = intRound(real_t(m_h - 130) / StepH);

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
  ::SetRect(&m_sRect[5], intRound(0.35 * m_w), sh, intRound(0.8 * m_w), sh + m_eH);
  ::SetRect(&m_lRect[0], 50, sh, intRound(0.35 * m_w), sh + m_eH);
  ::SetRect(&m_rRect[0], intRound(0.8 * m_w), sh, m_w - 10, sh + m_eH);

  sh += (m_eH + 10);
  ::SetRect(&m_sRect[6], intRound(0.35 * m_w), sh, intRound(0.8 * m_w), sh + m_eH);
  ::SetRect(&m_lRect[1], 50, sh, intRound(0.35 * m_w), sh + m_eH);
  ::SetRect(&m_rRect[1], intRound(0.8 * m_w), sh, m_w - 10, sh + m_eH);

  sh += (m_eH + 10);
  ::SetRect(&m_sRect[7], intRound(0.35 * m_w), sh, intRound(0.8 * m_w), sh + m_eH);
  ::SetRect(&m_lRect[2], 50, sh, intRound(0.35 * m_w), sh + m_eH);
  ::SetRect(&m_rRect[2], intRound(0.8 * m_w), sh, m_w - 10, sh + m_eH);

  sh += (m_eH + 10);
  ::SetRect(&m_sRect[8], intRound(0.35 * m_w), sh, intRound(0.8 * m_w), sh + m_eH);
  ::SetRect(&m_lRect[3], 50, sh, intRound(0.35 * m_w), sh + m_eH);
  ::SetRect(&m_rRect[3], intRound(0.8 * m_w), sh, m_w - 10, sh + m_eH);

  sh += (m_eH + 10);
  ::SetRect(&m_sRect[9], intRound(0.35 * m_w), sh, intRound(0.8 * m_w), sh + m_eH);
  ::SetRect(&m_lRect[4], 50, sh, intRound(0.35 * m_w), sh + m_eH);
  ::SetRect(&m_rRect[4], intRound(0.8 * m_w), sh, m_w - 10, sh + m_eH);

  // For HR Camera
  sh += (m_eH + 10);
  ::SetRect(&m_sRect[10], intRound(0.35 * m_w), sh, intRound(0.8 * m_w), sh + m_eH);
  ::SetRect(&m_lRect[5], 50, sh, intRound(0.35 * m_w), sh + m_eH);
  ::SetRect(&m_rRect[5], intRound(0.8 * m_w), sh, m_w - 10, sh + m_eH);
  // For HR Camera

  std::array radioIdc = {IDC_VERY_LIGHT_EYE_RADIO, IDC_LIGHT_EYE_RADIO, IDC_MEDIUM_EYE_RADIO, IDC_DARK_EYE_RADIO, IDC_VERY_DARK_EYE_RADIO};

  static_assert(eyeColorNames.size() == _countof(m_radioBtns));
  static_assert(radioIdc.size() == _countof(m_radioBtns));

  for (unsigned i = 0; i < eyeColorNames.size(); ++i) {
    m_radioBtns[i].Create(_T(""), _T(""), 0, m_sRect[i], this, radioIdc[i]);
    m_radioBtns[i].SetEnabled(TRUE);
    m_radioBtns[i].SetFontSize(m_FontSize);
    m_radioBtns[i].SetText(eyeColorNames[i]);
    m_radioBtns[i].SetBkColor(BLACK);
  }

  m_radioBtns[MEDIUMEYE].SetCheck(TRUE);

  m_WhiteLEDsPowerLevelSlider.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_sRect[5], this, IDC_IMGCTRL_SLIDER);
  m_BrightnessSlider.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_sRect[6], this, IDC_IMGCTRL_SLIDER);
  m_ContrastSlider.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_sRect[7], this, IDC_IMGCTRL_SLIDER);
  m_HueOrRedSlider.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_sRect[8], this, IDC_IMGCTRL_SLIDER);
  m_SaturationOrGreenSlider.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_sRect[9], this, IDC_IMGCTRL_SLIDER);

  if (m_IsHRCamera) {
    m_BlueSlider.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_sRect[10], this, IDC_IMGCTRL_SLIDER);
  }

  SetVideoAttributes(MEDIUMEYE);
}

//***************************************************************************************

void CImgCptCtrlWnd::ShowGUI()
{
  m_ShowWindow = TRUE;

  for (auto &r : m_radioBtns)
    r.ShowWindow(SW_SHOW);

  m_WhiteLEDsPowerLevelSlider.ShowWindow(SW_SHOW);
  m_BrightnessSlider.ShowWindow(SW_SHOW);
  m_ContrastSlider.ShowWindow(SW_SHOW);
  m_HueOrRedSlider.ShowWindow(SW_SHOW);
  m_SaturationOrGreenSlider.ShowWindow(SW_SHOW);

  if (m_IsHRCamera) {
    m_BlueSlider.ShowWindow(SW_SHOW);
  }

  Label();
}

//***************************************************************************************

void CImgCptCtrlWnd::HideGUI()
{
  m_ShowWindow = FALSE;

  for (auto &r : m_radioBtns)
    r.ShowWindow(SW_HIDE);

  m_WhiteLEDsPowerLevelSlider.ShowWindow(SW_HIDE);
  m_BrightnessSlider.ShowWindow(SW_HIDE);
  m_HueOrRedSlider.ShowWindow(SW_HIDE);
  m_SaturationOrGreenSlider.ShowWindow(SW_HIDE);

  if (m_IsHRCamera) {
    m_BlueSlider.ShowWindow(SW_HIDE);
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

  for (auto &r : m_radioBtns)
    if (r.GetCheck()) {
      SetTitle(std::distance(&m_radioBtns[0], &r), Font);
    }

  for (auto &i : m_lRect) {
    InvalidateRect(&i, FALSE);
  }
}

//***************************************************************************************

void CImgCptCtrlWnd::SetTitle(int EyeType, CMFont &font)
{
  auto draw = [&, this](RECT rc, int value, int maxValue) {
    m_MemDC.FillSolidRect(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, BLACK);
    m_MemDC.WriteText(fmt::format(L"{}/{}", value, maxValue).c_str(), rc, font, WHITE, 2, BLACK);
    InvalidateRect(&rc, FALSE);
  };

  auto s = stdSettings[EyeType];

  int n = 0;
  draw(m_rRect[n++], s.whiteLEDsPower, 50);
  draw(m_rRect[n++], s.brightness, 255);
  draw(m_rRect[n++], s.contrast, 255);

  if (m_IsHRCamera) {
    draw(m_rRect[n++], s.red, 255);
    draw(m_rRect[n++], s.green, 255);
    draw(m_rRect[n++], s.blue, 255);
  }
  else {
    draw(m_rRect[n++], s.hue, 255);
    draw(m_rRect[n++], s.saturation, 255);
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
  m_settings = stdSettings[EyeType];

  m_WhiteLEDsPowerLevelSlider.m_Pos = real_t(m_settings.whiteLEDsPower) / 50.0;
  m_BrightnessSlider.m_Pos          = real_t(m_settings.brightness) / 255.0;
  m_ContrastSlider.m_Pos            = real_t(m_settings.contrast) / 255.0;

  m_WhiteLEDsPowerLevelSlider.RePaint();
  m_BrightnessSlider.RePaint();
  m_ContrastSlider.RePaint();

  if (m_IsHRCamera) {
    m_HueOrRedSlider.m_Pos          = real_t(m_settings.red) / 255.0;
    m_SaturationOrGreenSlider.m_Pos = real_t(m_settings.green) / 255.0;
    m_BlueSlider.m_Pos              = real_t(m_settings.blue) / 255.0;

    m_BlueSlider.RePaint();
  }
  else {
    m_HueOrRedSlider.m_Pos          = real_t(m_settings.hue) / 255.0;
    m_SaturationOrGreenSlider.m_Pos = real_t(m_settings.saturation) / 255.0;
  }

  m_HueOrRedSlider.RePaint();
  m_SaturationOrGreenSlider.RePaint();
}

//***************************************************************************************

void CImgCptCtrlWnd::SetEyeChecks(int EyeType)
{
  if (!m_radioBtns[EyeType].GetCheck())
    return;

  for (unsigned i = 0; i < _countof(m_radioBtns); ++i) {
    m_radioBtns[i].SetCheck(EyeType == i);
    m_radioBtns[i].Repaint();
  }

  SetVideoAttributes(EyeType);
  Label();
}

//***************************************************************************************

void CImgCptCtrlWnd::OnVLightEyeRadioClicked()
{
  SetEyeChecks(VERYLIGHTEYE);
}

//***************************************************************************************

void CImgCptCtrlWnd::OnLightEyeRadioClicked()
{
  SetEyeChecks(LIGHTEYE);
}

//***************************************************************************************

void CImgCptCtrlWnd::OnMediumEyeRadioClicked()
{
  SetEyeChecks(MEDIUMEYE);
}

//***************************************************************************************

void CImgCptCtrlWnd::OnDarkEyeRadioClicked()
{
  SetEyeChecks(DARKEYE);
}

//***************************************************************************************

void CImgCptCtrlWnd::OnVeryDarkEyeRadioClicked()
{
  SetEyeChecks(VERYDARKEYE);
}

//***************************************************************************************

LRESULT CImgCptCtrlWnd::OnSliderChangedPos(WPARAM wParam, LPARAM lParam)
{
  for (auto &r : m_radioBtns) {
    if (r.GetCheck()) {
      r.SetCheck(FALSE);
      r.Repaint();
    }
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

    m_settings.whiteLEDsPower = val;
  }
  else if (y >= m_sRect[6].top && y <= m_sRect[6].bottom) {
    if (m_BrightnessSlider.m_Pos > 0.993)
      m_BrightnessSlider.m_Pos = 1;
    int val = intRound(m_BrightnessSlider.m_Pos * 255.0);
    s.Format(_T("%i/255"), val);

    m_MemDC.FillSolidRect(m_rRect[1].left, m_rRect[1].top, m_rRect[1].right - m_rRect[1].left, m_rRect[1].bottom - m_rRect[1].top, BLACK);
    m_MemDC.WriteText(s, m_rRect[1], Font, WHITE, 2, BLACK);
    InvalidateRect(&m_rRect[1], FALSE);

    m_settings.brightness = val;
  }
  else if (y >= m_sRect[7].top && y <= m_sRect[7].bottom) {
    if (m_ContrastSlider.m_Pos > 0.993)
      m_ContrastSlider.m_Pos = 1;
    int val = intRound(m_ContrastSlider.m_Pos * 255.0);
    s.Format(_T("%i/255"), val);

    m_MemDC.FillSolidRect(m_rRect[2].left, m_rRect[2].top, m_rRect[2].right - m_rRect[2].left, m_rRect[2].bottom - m_rRect[2].top, BLACK);
    m_MemDC.WriteText(s, m_rRect[2], Font, WHITE, 2, BLACK);
    InvalidateRect(&m_rRect[2], FALSE);

    m_settings.contrast = val;
  }
  else if (y >= m_sRect[8].top && y <= m_sRect[8].bottom) {
    if (m_HueOrRedSlider.m_Pos > 0.993)
      m_HueOrRedSlider.m_Pos = 1;
    int val = intRound(m_HueOrRedSlider.m_Pos * 255.0);
    s.Format(_T("%i/255"), val);

    m_MemDC.FillSolidRect(m_rRect[3].left, m_rRect[3].top, m_rRect[3].right - m_rRect[3].left, m_rRect[3].bottom - m_rRect[3].top, BLACK);
    m_MemDC.WriteText(s, m_rRect[3], Font, WHITE, 2, BLACK);
    InvalidateRect(&m_rRect[3], FALSE);

    m_settings.red = val;
    m_settings.hue = val;
  }
  else if (y >= m_sRect[9].top && y <= m_sRect[9].bottom) {
    if (m_SaturationOrGreenSlider.m_Pos > 0.993)
      m_SaturationOrGreenSlider.m_Pos = 1;
    int val = intRound(m_SaturationOrGreenSlider.m_Pos * 255.0);
    s.Format(_T("%i/255"), val);

    m_MemDC.FillSolidRect(m_rRect[4].left, m_rRect[4].top, m_rRect[4].right - m_rRect[4].left, m_rRect[4].bottom - m_rRect[4].top, BLACK);
    m_MemDC.WriteText(s, m_rRect[4], Font, WHITE, 2, BLACK);
    InvalidateRect(&m_rRect[4], FALSE);

    m_settings.green      = val;
    m_settings.saturation = val;
  }
  else if (y >= m_sRect[10].top && y <= m_sRect[10].bottom) {
    if (m_BlueSlider.m_Pos > 0.993)
      m_BlueSlider.m_Pos = 1;
    int val = intRound(m_BlueSlider.m_Pos * 255.0);
    s.Format(_T("%i/255"), val);

    m_MemDC.FillSolidRect(m_rRect[5].left, m_rRect[5].top, m_rRect[5].right - m_rRect[5].left, m_rRect[5].bottom - m_rRect[5].top, BLACK);
    m_MemDC.WriteText(s, m_rRect[5], Font, WHITE, 2, BLACK);
    InvalidateRect(&m_rRect[5], FALSE);

    m_settings.blue = val;
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
