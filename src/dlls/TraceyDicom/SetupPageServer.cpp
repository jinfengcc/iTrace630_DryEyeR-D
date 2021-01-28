#include "pch.h"
#include "SetupPageServer.h"

int CSetupPageServer::OnApply()
{
  return DoDataExchange(TRUE) ? PSNRET_NOERROR : PSNRET_INVALID_NOCHANGEPAGE;
}

int CSetupPageServer::OnSetActive()
{
  DoDataExchange(DDX_LOAD, IDC_RADIO1);
  return CPropertyPageImpl<CSetupPageServer>::OnSetActive();
}

BOOL CSetupPageServer::OnKillActive()
{
  DoDataExchange(DDX_SAVE, IDC_RADIO1);
  return CPropertyPageImpl<CSetupPageServer>::OnKillActive();
}

BOOL CSetupPageServer::DoDataExchange(BOOL bSaveAndValidate, UINT nCtlID)
{
  static_assert(0 == static_cast<int>(dicom::LogLevel::None));
  static_assert(1 == static_cast<int>(dicom::LogLevel::Info));
  static_assert(2 == static_cast<int>(dicom::LogLevel::Warn));
  static_assert(3 == static_cast<int>(dicom::LogLevel::Error));

  // clang-format off
  DDX_TEXT(IDC_IPADDR , m_cfg.host)
  DDX_UINT(IDC_PORT   , m_cfg.port)
  DDX_TEXT(IDC_CALL   , m_cfg.remoteAETitle)
  DDX_TEXT(IDC_CALLING, m_cfg.localAETitle)
  DDX_RADIO(IDC_RADIO1, m_logLevel)
  // clang-format on

  auto check = [&, this](int idc, bool ok, const char *err) {
    if (ok || !bSaveAndValidate || (nCtlID > 0 && nCtlID != idc))
      return;

    GetDlgItem(idc).SetFocus();
    throw std::runtime_error(err);
  };

  try {
    check(IDC_IPADDR, !m_cfg.host.IsEmpty(), "Host may not be empty");

    return TRUE;
  }
  catch (std::exception &e) {
    MessageBoxA(*this, e.what(), "iTrace", MB_ICONSTOP | MB_OK);
    return FALSE;
  }
}

BOOL CSetupPageServer::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
  InitDynamicLayout(false, false);
  CPropertySheetWindow sheet = this->GetPropertySheet();
  sheet.CenterWindow(this->GetPropertySheet().GetParent());

  DoDataExchange(DDX_LOAD);

  return TRUE; // Let the system set the focus
}

void CSetupPageServer::OnVerify(UINT uNotifyCode, int nID, CWindow wndCtl)
{
  if (!DoDataExchange(DDX_SAVE))
    return;

  try {
    DicomBase::SetLogging(m_logLevel);
    DicomEcho();
    ::MessageBoxA(*this, "Server connected successfully", "DICOM", MB_OK);
  }
  catch (_com_error &e) {
    DCMLOG_Error("Server Connection", e);

    auto err = fmt::format(L"Unable to connect to server\n{}", (const wchar_t *)e.Description());
    ::MessageBoxW(*this, err.c_str(), L"DICOM", MB_OK | MB_ICONSTOP);
  }
  catch (...) {
    DCMLOG_Error("Server Connection Failed : {}", "Unknown Error");
    ::MessageBoxA(*this, "Unable to connect to server\nUnknown Error", "DICOM", MB_OK | MB_ICONSTOP);
  }
}

void CSetupPageServer::DicomEcho()
{
  CWaitCursor cursor;

  auto req = RZDCX::Instance()->CreateInstance<rzdcxLib::IDCXREQ>();
  req->Echo(m_cfg.localAETitle.GetString(), m_cfg.remoteAETitle.GetString(), m_cfg.host.GetString(), m_cfg.port);
}

void CSetupPageServer::OnContextMenu(CWindow wnd, CPoint point)
{
  CMenu menu;
  menu.CreatePopupMenu();

  enum { local = 1001, remote };
  menu.AppendMenu(MF_STRING, local, L"localhost");
  menu.AppendMenu(MF_STRING, remote, L"www.dicomserver.co.uk");

  UINT uFlags = TPM_NONOTIFY | TPM_RETURNCMD | TPM_TOPALIGN | TPM_RIGHTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON;
  auto res    = menu.TrackPopupMenu(uFlags, point.x, point.y, *this);
  if (res == 0)
    return;

  DoDataExchange(DDX_SAVE, IDC_RADIO1);

  m_cfg.host          = res == local ? "localhost" : "www.dicomserver.co.uk";
  m_cfg.port          = 104u;
  m_cfg.remoteAETitle = res == local ? "DSRSVC" : "DCMSERVER";
  m_cfg.localAETitle  = "YYYY";

  DoDataExchange(DDX_LOAD);
}
