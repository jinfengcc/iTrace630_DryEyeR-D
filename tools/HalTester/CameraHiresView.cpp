#include "pch.h"
#include "CameraHiresView.h"
#include "WinHelpers.h"
#include "libs/CommonLib/AppSettings.h"

using hal::ICameraHires;

bool CCameraHiresView::OnHide(bool)
{
  if (m_camera)
    m_camera->StopCapture();
  return true;
}

void CCameraHiresView::OnShow()
{
  if (m_camera)
    m_camera->StartCapture([this](unsigned) { m_cameraWnd.Invalidate(); });
}

void CCameraHiresView::OnTerminating()
{
  AppSettings::Instance()->RemoveNotify(m_notificationId);
  m_camera->StopCapture();
}

BOOL CCameraHiresView::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
  InitializeSpinners();
  InitializeCombo();

  // clang-format off
  SetDlgItemInt( IDC_BRIGHTNESS  ,  30);
  SetDlgItemInt( IDC_CONTRAST    ,   0);
  SetDlgItemInt( IDC_HUE         , 130);
  SetDlgItemInt( IDC_SATURATION  ,  70);
  SetDlgItemInt( IDC_GAIN        , 100);
  // clang-format on

  CreateObjects();
  InitDynamicLayout(false, false);
  return TRUE;
}

void CCameraHiresView::OnPopupMore(UINT uNotifyCode, int nID, CWindow wndCtl)
{
  RECT rc;

  ::GetWindowRect(wndCtl, &rc);

  CMenu menu  = LoadMenu(_Module.m_hInst, MAKEINTRESOURCE(IDR_POPUP_MENU));
  auto  popup = menu.GetSubMenu(0);

  auto check  = [&popup](int id, bool yes) { popup.CheckMenuItem(id, MF_BYCOMMAND | (yes ? MF_CHECKED : MF_UNCHECKED)); };
  auto enable = [&popup](int id, bool yes) { popup.EnableMenuItem(id, MF_BYCOMMAND | (yes ? MF_ENABLED : MF_DISABLED)); };

  auto connected = m_camera->Connected();

  check(ID_HIRESCAMEARA_CONNECT, connected);
  check(ID_HIRESCAMEARA_COLORIMAGE, m_colorImg);
  check(ID_HIRESCAMEARA_ROTATE, m_rotate);
  check(ID_FLIP_NONE, m_flip == Flip::none);
  check(ID_FLIP_HORIZONTAL, m_flip == Flip::hor);
  check(ID_FLIP_VERTICAL, m_flip == Flip::ver);
  check(ID_HIRESCAMEARA_SHOWINFO, m_showInfo);
  check(ID_ENHANCE_NONE, m_colorImg || m_enhance == Enhance::none);
  if (m_colorImg) {
    enable(ID_ENHANCE_NORMALIZE, false);
    enable(ID_ENHANCE_GAMMA, false);
  }
  else {
    check(ID_ENHANCE_NORMALIZE, m_enhance == Enhance::normal);
    check(ID_ENHANCE_GAMMA, m_enhance == Enhance::gamma);
  }

  auto flags = TPM_RIGHTBUTTON | TPM_RETURNCMD;
  auto cmd   = TrackPopupMenuEx(popup, flags, rc.left, rc.bottom, m_hWnd, nullptr);

  switch (cmd) {
  case ID_HIRESCAMEARA_CONNECT:
    m_camera->Connect(!connected);
    break;
  case ID_HIRESCAMEARA_COLORIMAGE:
    m_colorImg = !m_colorImg;
    break;
  case ID_HIRESCAMEARA_ROTATE:
    m_rotate = !m_rotate;
    break;
  case ID_FLIP_NONE:
    m_flip = Flip::none;
    break;
  case ID_FLIP_HORIZONTAL:
    m_flip = Flip::hor;
    break;
  case ID_FLIP_VERTICAL:
    m_flip = Flip::ver;
    break;
  case ID_HIRESCAMEARA_SHOWINFO:
    m_showInfo = !m_showInfo;
    break;
  case ID_ENHANCE_NONE:
    m_enhance = Enhance::none;
    break;
  case ID_ENHANCE_NORMALIZE:
    m_enhance = Enhance::normal;
    break;
  case ID_ENHANCE_GAMMA:
    m_enhance = Enhance::gamma;
    break;

  default:
    break;
  }
}

void CCameraHiresView::OnEditChanged(UINT uNotifyCode, int nID, CWindow wndCtl)
{
  static std::map<int, Guid> map{
    // clang-format off
    {IDC_BRIGHTNESS  , ICameraHires::BRIGHTNESS},
    {IDC_CONTRAST    , ICameraHires::CONTRAST  },
    {IDC_HUE         , ICameraHires::HUE       },
    {IDC_SATURATION  , ICameraHires::SATURATION},
    {IDC_GAIN        , ICameraHires::GAIN      },
    // clang-format on
  };

  if (map.contains(nID)) {
    if (auto i = GetValue<int>(GetDlgItem(nID)); i.has_value())
      m_config.Set(map[nID], i.value());
  }
  else {
    SetMsgHandled(FALSE);
  }
}

void CCameraHiresView::OnExposureChanged(UINT uNotifyCode, int nID, CWindow wndCtl)
{
  if (auto cb = CComboBox(GetDlgItem(nID))) {
    int sel = cb.GetCurSel();
    m_config.Set(ICameraHires::EXPOSURE, -sel);
  }
}

void CCameraHiresView::CreateObjects()
{
  m_lightsWindow = std::make_unique<CLightsWindow>(*this);

  m_camera = CreateObj<ICameraHires>(m_config);
  m_cameraWnd.SubclassWindow(GetDlgItem(IDC_CAMERA), [this](CDCHandle dc, const RECT &rc) {
    LoadImage();

    dc.FillSolidRect(&rc, RGB(0, 0, 0));
    DrawImage(dc, rc, m_colorImg ? m_colorImage : m_grayImage);
    UpdateFPS();
    DrawInfo(dc, rc);
  });

  m_notificationId = AppSettings::Instance()->AddNotify([this]() {

  });
}

void CCameraHiresView::InitializeSpinners()
{
  int cameraSpinners[] = {
    IDC_SPIN1, IDC_SPIN2, IDC_SPIN3, IDC_SPIN4, IDC_SPIN10,
  };

  for (auto id : cameraSpinners) {
    CUpDownCtrl w(GetDlgItem(id));
    w.SetRange(0, 255);
  }
}

void CCameraHiresView::InitializeCombo()
{
  const wchar_t *exposure[] = {
    L"Auto",  L"640 ms", L"320 ms",  L"160 ms",  L"80 ms",  L"40 ms",  L"20 ms",
    L"10 ms", L"5 ms",   L"2.50 ms", L"1.25 ms", L"650 µs", L"312 µs", L"150 µs",
  };

  if (auto cb = CComboBox(GetDlgItem(IDC_EXPOSURE))) {
    for (auto e : exposure)
      cb.AddString(e);
    cb.SetCurSel(0);
  }
}

void CCameraHiresView::UpdateFPS()
{
  //double fps{};
  //if (m_camera && m_camera->Connected(&fps)) {
  //  SetDlgItemText(IDC_FPS, fmt::format(L"{:.2}fps", fps).c_str());
  //  SetDlgItemText(IDC_IMG_SIZE, fmt::format(L"{}x{}", m_colorImage.cols, m_colorImage.rows).c_str());
  //}
  //else {
  //  SetDlgItemText(IDC_FPS, L"");
  //  SetDlgItemText(IDC_IMG_SIZE, L"");
  //}
}

void CCameraHiresView::LoadImage()
{
  if (m_camera->GetImage(m_colorImage)) {
    cv::cvtColor(m_colorImage, m_grayImage, cv::COLOR_BGR2GRAY);
    if (m_rotate)
      cv::rotate(m_colorImage, m_colorImage, cv::ROTATE_180);

    if (m_flip == Flip::hor)
      cv::flip(m_colorImage, m_colorImage, 1);
    else if (m_flip == Flip::ver)
      cv::flip(m_colorImage, m_colorImage, 0);

    ProcessImage();
  }
}

void CCameraHiresView::ProcessImage()
{
  if (m_enhance == Enhance::none)
    return;

  double minVal, maxVal;
  cv::minMaxIdx(m_grayImage, &minVal, &maxVal);

  auto a = 255 / (maxVal - minVal);
  auto b = -minVal;

  m_grayImage.convertTo(m_grayImage, -1, a, b);

  m_grayImage = SharpenImage(m_grayImage);

  if (m_enhance == Enhance::gamma) {
    auto    gamma_ = 1.5;
    cv::Mat lookUpTable(1, 256, CV_8U);
    uchar * p = lookUpTable.ptr();
    for (int i = 0; i < 256; ++i)
      p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, gamma_) * 255.0);

    cv::Mat tmp1 = m_grayImage;
    cv::Mat tmp2 = tmp1.clone();
    cv::LUT(tmp1, lookUpTable, tmp2);

    tmp2.copyTo(m_grayImage);
  }
}

void CCameraHiresView::DrawInfo(CDCHandle dc, const RECT &rc)
{
  if (m_colorImage.empty() || !m_showInfo)
    return;

  CBrush brush;
  brush.CreateSolidBrush(RGB(255, 255, 255));

  CFont font;
  font.CreatePointFont(80, _T("Arial"), dc);

  dc.SaveDC();

  dc.SelectBrush(brush);
  dc.SelectFont(font);
  dc.SetTextAlign(TA_BOTTOM);

  double maxValue;
  cv::minMaxIdx(m_grayImage, nullptr, &maxValue);

  const int margin = 1;

  auto txt = fmt::format(L"max = {}", maxValue);
  dc.TextOut(rc.left + margin, rc.bottom - (margin - 1), txt.c_str(), txt.size());

  double fps = 0;
  if (m_camera->Connected(&fps)) {
    txt = fmt::format(L"{}x{} FPS={:.1f}fps", m_colorImage.cols, m_colorImage.rows, fps);
    SIZE size;
    dc.GetTextExtent(txt.c_str(), txt.size(), &size);
    dc.TextOut(rc.right + (margin - 1) - size.cx, rc.bottom - (margin - 1), txt.c_str(), txt.size());
  }

  dc.RestoreDC(-1);
}

cv::Mat CCameraHiresView::SharpenImage(const cv::Mat &lena)
{
  using namespace cv;
  Mat sharpenedLena;
  Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
  cv::filter2D(lena, sharpenedLena, lena.depth(), kernel);

  return sharpenedLena;
}
