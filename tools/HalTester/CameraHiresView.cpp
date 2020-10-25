#include "pch.h"
#include "CameraHiresView.h"
#include "WinHelpers.h"

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
  m_camera->StartCapture([this](cv::Mat) { m_cameraWnd.Invalidate(); });
}

void CCameraHiresView::OnTerminating()
{
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
    dc.FillSolidRect(&rc, RGB(0, 0, 0));
    m_camera->GetImage(m_image);
    UpdateFPS();
    DrawImage(dc, rc, m_image);
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
  double fps{};
  if (m_camera && m_camera->Connected(&fps)) {
    SetDlgItemText(IDC_IMG_SIZE, fmt::format(L"{}x{}", m_image.cols, m_image.rows).c_str());
    SetDlgItemText(IDC_FPS, fmt::format(L"{:.2}fps", fps).c_str());
  }
  else {
    SetDlgItemText(IDC_IMG_SIZE, L"");
    SetDlgItemText(IDC_FPS, L"");
  }
}
