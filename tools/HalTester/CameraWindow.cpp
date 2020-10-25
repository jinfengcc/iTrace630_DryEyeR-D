#include "pch.h"
#include "CameraWindow.h"

CCameraWindow::CCameraWindow(HWND hWnd)
  : CWindow(hWnd)
  , m_imgCtrl(this, 1)
{
  SetTimer(2, 50);
  m_imgCtrl.SubclassWindow(GetDlgItem(IDC_CAMERA));

  auto ws = WindowAreaControls(*this, IDC_CAMERA_SEC);
  for (auto w : ws) {
    char szClassName[256];
    ::GetClassNameA(w, szClassName, _countof(szClassName));
    if (_stricmp(szClassName, "msctls_updown32") == 0)
      CUpDownCtrl(w).SetRange(0, 255);
  }

  DoDataExchange(DDX_LOAD);
  UpdatePropVisuals();
}

void CCameraWindow::Start()
{
  CreateObj(m_camera);
  DoDataExchange(DDX_SAVE);
  m_camera->Settings(m_settings);
}

void CCameraWindow::Stop()
{
  m_camera = nullptr;
}

void CCameraWindow::OnTimer(UINT_PTR nIDEvent)
{
  if (!m_freeze && m_camera) {
    m_camera->LoadImage();
    m_camera->GetImage(m_image);
    m_imgCtrl.Invalidate();
  }
}

void CCameraWindow::OnButtonClicked(UINT uNotifyCode, int nID, CWindow wndCtl)
{
  // Let siblings have a chance on a button click
  SetMsgHandled(FALSE);

  DoDataExchange(DDX_SAVE);
  UpdatePropVisuals();

  m_camera->Power(m_cameraPower);
  if (m_cameraPower) {
    m_settings = m_camera->Settings();
    DoDataExchange(DDX_LOAD);
  }

  m_imgCtrl.Invalidate();
}

void CCameraWindow::OnEditChanged(UINT uNotifyCode, int nID, CWindow wndCtl)
{
  // Let siblings have a chance on this event
  SetMsgHandled(FALSE);

  if (m_camera) {
    DoDataExchange(DDX_SAVE);
    m_camera->Settings(m_settings);
  }
}

BOOL CCameraWindow::OnCameraEraseBkgnd(CDCHandle)
{
  return TRUE;
}

void CCameraWindow::OnCameraPaint(CDCHandle ddc)
{
  WTL::CPaintDC  dc(m_imgCtrl);
  WTL::CMemoryDC memDC(dc, dc.m_ps.rcPaint);
  memDC.FillSolidRect(&dc.m_ps.rcPaint, RGB(0, 24, 40));
  if (m_image.empty())
    return;

  Gdiplus::Graphics gdg(memDC);
  if (m_imgUnwrap) {
    auto ctr = cv::Point2i(m_image.size().width / 2, m_image.size().height / 2);
    auto cp  = m_image(cv::Range(ctr.y - 101, ctr.y + 100), cv::Range(ctr.x - 101, ctr.x + 100));

    cp = m_image;
    cv::Mat img;
    cv::warpPolar(cp, img, {}, {float(cp.size().width / 2.0), float(cp.size().height / 2.0)}, 111, cv::WARP_POLAR_LINEAR);
    cv::rotate(img, img, cv::ROTATE_90_COUNTERCLOCKWISE);

    if (IsShiftKeyPressed())
      cv::GaussianBlur(img, img, {9, 9}, 0);

    DrawImage(gdg, img);
  }
  else {
    DrawImage(gdg, m_image);
  }
}

void CCameraWindow::DrawImage(Gdiplus::Graphics &gdg, const cv::Mat &img)
{
  CRect rc;
  GetDlgItem(IDC_CAMERA).GetClientRect(rc);

  auto bmp = Mat2Bitmap(img);
  auto rc1 = GetSameAspectRect(rc, CRect(0, 0, bmp->GetWidth(), bmp->GetHeight()));
  gdg.DrawImage(bmp.get(), rc1.left, rc1.top, rc1.Width(), rc1.Height());
}


void CCameraWindow::UpdatePropVisuals()
{
  auto ws = WindowAreaControls(*this, IDC_CAMERA_PROPS);
  for (auto w : ws)
    ::EnableWindow(w, m_cameraPower);
}
