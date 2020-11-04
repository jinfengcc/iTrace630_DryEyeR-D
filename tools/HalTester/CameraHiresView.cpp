#include "pch.h"
#include "CameraHiresView.h"
#include "WinHelpers.h"
#include "libs/CommonLib/AppSettings.h"
#include <nlohmann/json.hpp>
#include "Spline.h"
#include "libs/CalcLib/Image/ImgEnhanceContrast.h"

using hal::ICameraHires;

enum class Orient { none, rotate, flipx, flipy };

// map TaskState values to JSON as strings
NLOHMANN_JSON_SERIALIZE_ENUM(Orient, {
                                       {Orient::none, nullptr},
                                       {Orient::rotate, "rotate"},
                                       {Orient::flipx, "flipx"},
                                       {Orient::flipy, "flipy"},
                                     })

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
  check(ID_HIRESCAMEARA_COLORIMAGE, m_showColor);
  check(ID_HIRESCAMEARA_ENHANCE, m_enhance && !m_showColor);
  check(ID_HIRESCAMEARA_SHOWINFO, m_showInfo);

  enable(ID_HIRESCAMEARA_ENHANCE, !m_showColor);

  auto flags = TPM_RIGHTBUTTON | TPM_RETURNCMD;
  auto cmd   = TrackPopupMenuEx(popup, flags, rc.left, rc.bottom, m_hWnd, nullptr);

  switch (cmd) {
  case ID_HIRESCAMEARA_CONNECT:
    m_camera->Connect(!connected);
    break;
  case ID_HIRESCAMEARA_COLORIMAGE:
    m_showColor = !m_showColor;
    break;
  case ID_HIRESCAMEARA_ENHANCE:
    m_enhance = !m_enhance;
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
    DrawImage(dc, rc, m_showColor ? m_colorImage : m_grayImage);
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

void CCameraHiresView::LoadImage()
{
  if (m_camera->GetImage(m_colorImage)) {
    ProcessImage();
  }
}

void CCameraHiresView::ProcessImage()
{
  AppSettings apps;

  if (apps.Get("HalTester.hrcam.rotate", false)) {
    cv::rotate(m_colorImage, m_colorImage, cv::ROTATE_180);
  }

  if (apps.Get("HalTester.hrcam.flipv", false)) {
    cv::flip(m_colorImage, m_colorImage, 0);
  }

  if (apps.Get("HalTester.hrcam.fliph", false)) {
    cv::flip(m_colorImage, m_colorImage, 1);
  }

  cv::cvtColor(m_colorImage, m_grayImage, cv::COLOR_BGR2GRAY);

#ifdef _DEBUG
  auto tmp = m_grayImage.clone();
#endif

  if (m_enhance)
    img::EnhanceContrast(m_grayImage);

#if 0
  if (apps.Get("HalTester.hrcam.enhance.histogram", false)) {
    cv::equalizeHist(m_grayImage, m_grayImage);
  }

  if (apps.Get("HalTester.hrcam.enhance.sharpen", false)) {
    m_grayImage = SharpenImage(m_grayImage);
  }

  if (auto gamma = apps.Get("HalTester.hrcam.enhance.gamma", 0.0); gamma > 0.0) {
    cv::Mat lookUpTable(1, 256, CV_8U);
    uchar * p = lookUpTable.ptr();
    for (int i = 0; i < 256; ++i)
      p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);

    cv::Mat tmp1 = m_grayImage;
    cv::Mat tmp2 = tmp1.clone();
    cv::LUT(tmp1, lookUpTable, tmp2);

    tmp2.copyTo(m_grayImage);
  }
#endif
}

void CCameraHiresView::ProcessImageMore()
{
  #ifdef _DEBUG
  #endif

  /// Establish the number of bins

  float        range[]   = {0, 256};
  const float *histRange = {range};
  int          histSize  = 256;

  bool uniform    = true;
  bool accumulate = false;

  cv::Mat_<float> hist;
  cv::calcHist(&m_grayImage, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

  std::vector<int> hv;
  for (auto v : hist)
    hv.push_back(static_cast<int>(v));

  const int N = m_grayImage.rows * m_grayImage.cols;
  int       xx0 = 0, xx1 = 0;
  int       sum = 0;
  for (unsigned i = 0; i < hv.size() && xx1 == 0; ++i) {
    sum += hv[i];
    if (xx0 == 0 && sum > 2* N / 3)
      xx0 = i;

    if (xx1 == 0 && sum > 95 * N / 100)
      xx1 = i;
  }

  // const int    x0 = 30, x1 = 100;
  const int    x0 = 50, x1 = xx1;
  const double y0 = 15, y1 = 240;

  std::vector<double> tv(256);
  for (int i = 0; i < x0; ++i) {
    const double slope = y0 / x0;
    tv[i]              = i * slope;
  }
  for (int i = x0; i < x1; ++i) {
    const double slope = (y1 - y0) / (x1 - x0 - 1);
    tv[i]              = y0 + (i - x0) * slope;
  }
  for (int i = x1; i < 256; ++i) {
    const double slope = (255 - y1) / (255 - x1);
    tv[i]              = y1 + (i - x1) * slope;
  }

  cv::Mat1b tmp = m_grayImage.clone();
  tmp.forEach([&](uchar &v, const int *) { v = static_cast<uchar>(tv[v]); });

  // for (int )

  std::vector<double> X{0, 20, 100, 255};
  std::vector<double> Y{0, 0, 255, 255};

  tk::spline s;
  s.set_points(X, Y);

  std::vector<int> kxs;
  for (int i = 0; i < 256; ++i)
    kxs.push_back(s(i));
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
