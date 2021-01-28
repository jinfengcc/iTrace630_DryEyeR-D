#pragma once

#include "resource.h"

class CSetupPageServer
  : public CPropertyPageImpl<CSetupPageServer>
  , public CWinDataExchange<CSetupPageServer>
  , public CDynamicDialogLayout<CSetupPageServer>
{
public:
  enum { IDD = IDD_SETUP_SERVER };

  CSetupPageServer(dicom::Server &cfg, dicom::LogLevel &logLevel, const TCHAR *title)
    : CPropertyPageImpl<CSetupPageServer>(title)
    , m_cfg(cfg)
    , m_logLevel(logLevel)
  {
  }

private:
  friend class CPropertyPageImpl<CSetupPageServer>;

  dicom::Server &  m_cfg;
  dicom::LogLevel &m_logLevel;

  int OnApply();
  int OnSetActive();
  BOOL OnKillActive();

  BOOL DoDataExchange(BOOL bSaveAndValidate = FALSE, UINT nCtlID = (UINT)-1);
  BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
  void OnVerify(UINT uNotifyCode, int nID, CWindow wndCtl);
  void DicomEcho();

  void OnContextMenu(CWindow wnd, CPoint point);

  using CWinDataExchange::DDX_Radio;
	void DDX_Radio(UINT nID, dicom::LogLevel &nValue, BOOL bSave)
  {
    auto n = static_cast<int>(nValue);
    DDX_Radio(nID, n, bSave);
    nValue = static_cast<dicom::LogLevel>(n);
  }


  BEGIN_MSG_MAP_EX(CSetupPageServer)
    MSG_WM_INITDIALOG(OnInitDialog)
    MSG_WM_CONTEXTMENU(OnContextMenu)
    COMMAND_ID_HANDLER_EX(IDC_VERIFY, OnVerify)
    CHAIN_MSG_MAP(CPropertyPageImpl<CSetupPageServer>)
    CHAIN_MSG_MAP(CDynamicDialogLayout<CSetupPageServer>)
  END_MSG_MAP()
};
