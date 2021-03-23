#include "pch.h"
#include "SetupSheet.h"
#include "interfaces/IServices.h"

CDicomSetup::CDicomSetup(ITraceyDicomConfig *obj)
  : CPropertySheetImpl<CDicomSetup>(_T("DICOM Setup"))
  , m_obj(obj)
  , m_cfg(obj->Get())
  , m_serverMWL(m_cfg.mwl, m_cfg.loggingLevel, L"Worklist Server")
  , m_serverStore(m_cfg.store, m_cfg.loggingLevel, L"Store Server")
  , m_selection(m_cfg)
{
  m_psh.dwFlags |= PSH_NOAPPLYNOW;
  m_psh.dwFlags &= ~PSH_HASHELP;

  AddPage(m_serverMWL);
  AddPage(m_serverStore);
  AddPage(m_selection);
}

int CDicomSetup::DoModal(HWND hWndParent /*= ::GetActiveWindow()*/)
{
  if (m_obj->IsValid()) {
    auto pwd = CreateObj<IServices>();
    if (!pwd->CheckPassword())
      return IDCANCEL;
  }

  if (!hWndParent)
    hWndParent = ::GetActiveWindow();

  if (int res = CPropertySheetImpl<CDicomSetup>::DoModal(hWndParent); res != IDOK)
    return res;

  m_obj->Set(m_cfg);
  return IDOK;
}

BOOL CDicomSetup::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
  CenterWindow(::GetActiveWindow());

  return TRUE;
}
