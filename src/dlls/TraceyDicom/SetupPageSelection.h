#pragma once

#include "resource.h"
#include "libs/CommonLib/ViewTypes.h"

class CSetupPageSelection
  : public CPropertyPageImpl<CSetupPageSelection>
  , public CWinDataExchange<CSetupPageSelection>
{
public:
  enum { IDD = IDD_SETUP_SELECT };

  CSetupPageSelection(dicom::Config &cfg)
    : m_cfg(cfg)
  {
  }

  int OnApply();

private:
  dicom::Config &m_cfg;

  BOOL DoDataExchange(BOOL bSaveAndValidate = FALSE, UINT nCtlID = (UINT)-1);
  BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
  void OnContextMenu(CWindow wnd, CPoint point);

  BEGIN_MSG_MAP_EX(CSetupPageSelection)
    MSG_WM_INITDIALOG(OnInitDialog)
    MSG_WM_CONTEXTMENU(OnContextMenu)
    CHAIN_MSG_MAP(CPropertyPageImpl<CSetupPageSelection>)
  END_MSG_MAP()
};



