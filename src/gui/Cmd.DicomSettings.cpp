#include "stdafx.h"
#include "Cmd.Target.h"
#include "Resource.h"
#include "CustomMenu.h"
#include "interfaces/ITraceyDicom.h"

class CmdDicomSettings : public CmdTargetBase
{
  DECLARE_MESSAGE_MAP()
  afx_msg void OnSettings();
  afx_msg void OnLicense();
};

static CmdDicomSettings cmd;

BEGIN_MESSAGE_MAP(CmdDicomSettings, CCmdTarget)
  ON_COMMAND(IDC_SETTINGS_BUTTON, OnSettings)
  ON_COMMAND(IDC_LICENSE_BUTTON, OnLicense)
END_MESSAGE_MAP()

void CmdDicomSettings::OnSettings()
{
  if (!m_isComboDicom) {
    SetMsgHandled(FALSE);
    return;
  }

  CCustomButtonMenu menu;
  menu.AddMenuItems({_T("Standard"), _T("DICOM")});

  if (auto res = menu.TrackPopupMenu(); res == 0) {
    SetMsgHandled(FALSE);
  }
  else if (res == 1) {
    auto obj = CreateObj<ITraceyDicomConfig>();
    obj->ChangeConfig(AfxGetMainWnd()->GetSafeHwnd());
  }
}

void CmdDicomSettings::OnLicense()
{
  auto obj = CreateObj<ITraceyDicomConfigRZDCX>();
  if (IsShiftKeyPressed()) {
    obj->ChangeActivation(AfxGetMainWnd()->GetSafeHwnd());
    return;
  }

  if (!m_isComboDicom || obj->IsActivated()) {
    SetMsgHandled(FALSE);
    return;
  }

  CCustomButtonMenu menu;
  menu.AddMenuItems({_T("Standard"), _T("DICOM")});

  if (auto res = menu.TrackPopupMenu(); res == 0) {
    SetMsgHandled(FALSE);
  }
  else if (res == 1) {
    auto old = obj->IsActivated();
    obj->ChangeActivation(AfxGetMainWnd()->GetSafeHwnd());
    if (old != obj->IsActivated()) {
      if (obj->IsActivated())
        Info("DICOM Library Successfully activated");
      else
        Error("Unable to activate the DICOM Library");
    }
  }
}
