#include "pch.h"
#include "MainDlg.h"
#include <dpi.h>
#include <interfaces/hal/IDevice.h>
#include <libs/CommonLib/WindowPlacement.h>
#include "CameraHiresView.h"
#include "CameraView.h"

CMainDlg::CMainDlg()
{
  m_appSettings.SetCallback([]() {
    OutputDebugStringA("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
  });

  CreateFonts();
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
  DestroyWindow();
  ::PostQuitMessage(wID);

  return 0;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL & /*bHandled*/)
{
  m_viewContainer.Shutdown();
  CreateObj<hal::IDevice>()->Shutdown();

  CMessageLoop *pLoop = _Module.GetMessageLoop();
  ATLASSERT(pLoop != nullptr);
  pLoop->RemoveMessageFilter(this);
  pLoop->RemoveIdleHandler(this);

  return 0;
}

void CMainDlg::OnDpiChanged(UINT nDpiX, UINT nDpiY, PRECT pRect)
{
  auto &dpi = CDPI::Instance();
  if (dpi.GetAwareness() == PROCESS_DPI_UNAWARE)
    return;

  dpi.SetScale(nDpiX);
  CreateFonts();
  MoveWindow(pRect);
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL & /*bHandled*/)
{
  SetIcons();
  SetVersionInfo();

  // register object for message filtering and idle updates
  CMessageLoop *pLoop = _Module.GetMessageLoop();
  ATLASSERT(pLoop != nullptr);
  pLoop->AddMessageFilter(this);
  pLoop->AddIdleHandler(this);

  InitializeViews();
  CenterWindow();
  InitDynamicLayout();

  return TRUE;
}

void CMainDlg::InitializeViews()
{
  m_btnViewGroup = CSimpleTabCtrl::CreateIntance(*this, IDC_TABMENU, [this](unsigned ndx) -> bool {
    m_viewContainer.ShowViewNdx(ndx);
    return true;
  });

  m_viewContainer.CreateContainer(*this, IDC_CONTENTS);
  //SIGNALS.commandFired.Connect([this]() {
  //  if (this->SendDlgItemMessage(IDC_AUTO_CLOSE, BM_GETCHECK))
  //    this->PostMessage(WM_COMMAND, IDCANCEL);
  //});

  CreateView<CCameraHiresView>(_T("Camera (Hi Res)"));

  if (VerifyInstrument()) {
    CreateView<CCameraView>(_T("Camera"));
    CreateView<CFlashView>(_T("Flash"));
  }
}

void CMainDlg::SetVersionInfo()
{
  VersionInfo vi;
  if (auto ffi = static_cast<const VS_FIXEDFILEINFO *>(vi)) {
    auto title = fmt::format(L"HAL Tester Ver {:04}.{:04}", HIWORD(ffi->dwFileVersionLS), LOWORD(ffi->dwFileVersionLS));
    SetWindowText(title.c_str());
  }
}

void CMainDlg::SetIcons()
{
  HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
  SetIcon(hIcon, TRUE);
  HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
  SetIcon(hIconSmall, FALSE);
}

BOOL CMainDlg::OnIdle()
{
  UIUpdateChildWindows();
  return FALSE;
}

BOOL CMainDlg::PreTranslateMessage(MSG *pMsg)
{
  return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnEraseBackground(CDCHandle dc)
{
  auto &g = Globals::Instance();

  dc.FillSolidRect(GetSectRect(Sect::Tab), g.tab.bkColor);

  CRect rcc = GetSectRect(Sect::Contents);
  dc.FillSolidRect(rcc, g.bkColor);

  DrawVersion(dc, rcc);

  return 1;
}

CRect CMainDlg::GetSectRect(Sect s) const
{
  CRect rc;
  GetClientRect(rc);

  CRect   r;
  CWindow w = GetDlgItem(IDC_TABMENU);
  w.GetWindowRect(r);
  ScreenToClient(r);

  switch (s) {
  case Sect::Tab:
    rc.right = r.right;
    break;
  case Sect::Contents:
    rc.left = r.right;
    break;
  default:
    break;
  }

  return rc;
}

void CMainDlg::DrawVersion(CDCHandle &dc, CRect rcc)
{
  // if (auto ver = GetVerInfo(); !ver.empty()) {
  //  HFONT hf = dc.SelectFont(m_font);
  //  auto  c  = dc.SetBkColor(Globals::Instance().bkColor);
  //  --rcc.bottom;
  //  dc.DrawText(ver.c_str(), -1, rcc, DT_RIGHT | DT_BOTTOM | DT_SINGLELINE);
  //  dc.SetBkColor(c);
  //  dc.SelectFont(hf);
  //}
}

void CMainDlg::CreateFonts()
{
  auto &dpi = CDPI::Instance();

  if (m_font)
    m_font.DeleteObject();

  int fontSize = 50 * dpi.GetScale() / 100;
  m_font.CreatePointFont(std::min(fontSize, 70), L"Arial");

  Globals::Instance().CreateFonts(dpi.GetScale() / 100.0);
}

HBRUSH CMainDlg::OnCtlColorButton(CDCHandle dc, HWND hWnd)
{
  const Globals &bd = Globals::Instance();
  if (::GetDlgCtrlID(hWnd) == IDCANCEL) {
    dc.SetBkColor(bd.tab.bkColor);
    return bd.tab.bkBrush;
  }
  else {
    dc.SetTextColor(bd.tab.fgNormal);
    dc.SetBkColor(bd.tab.bkColor);
    return bd.tab.bkBrush;
  }
}

bool CMainDlg::VerifyInstrument(bool quiet /*= true*/)
{
  auto d = CreateObj<hal::IDevice>();
  if (d && d->Connected()) {
    return true;
  }
  else {
    if (!quiet) {
      const wchar_t msg[] = L"Instrument not connected or powered down.\nPlease power up and try again.";
      AtlMessageBox(nullptr, msg, L"HalTester Error", MB_ICONSTOP);
    }

    return false;
  }
}
