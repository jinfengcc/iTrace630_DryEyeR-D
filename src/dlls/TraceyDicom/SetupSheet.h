#pragma once

#include "SetupPageServer.h"
#include "SetupPageSelection.h"

class CDicomSetup : public CPropertySheetImpl<CDicomSetup>
{
public:
  enum { SP_PROGRAM = 0, SP_LENS, SP_TORIC, SP_PIGGY, SP_ADVANCED };

  CDicomSetup(ITraceyDicomConfig *cfg);

  int DoModal(HWND hWndParent = ::GetActiveWindow());

private:
  ITraceyDicomConfig *m_obj;
  dicom::Config       m_cfg;
  CSetupPageServer    m_serverMWL;
  CSetupPageServer    m_serverStore;
  CSetupPageSelection m_selection;

  BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);

  BEGIN_MSG_MAP(CDicomSetup)
    MSG_WM_INITDIALOG(OnInitDialog)
    CHAIN_MSG_MAP(CPropertySheetImpl<CDicomSetup>)
  END_MSG_MAP()
};
