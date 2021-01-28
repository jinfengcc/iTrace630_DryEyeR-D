#include "stdafx.h"
#include "Cmd.Target.h"
#include "Resource.h"
#include "CustomMenu.h"
#include "interfaces/ITraceyDicom.h"

class CmdDicomSettings : public CmdTargetBase
{
  DECLARE_MESSAGE_MAP()
  afx_msg void OnSettings();
};

static CmdDicomSettings cmd;

BEGIN_MESSAGE_MAP(CmdDicomSettings, CCmdTarget)
  ON_COMMAND(IDC_SETTINGS_BUTTON, OnSettings)
END_MESSAGE_MAP()

void CmdDicomSettings::OnSettings()
{
  CRect rc = GetButtonRect();
  CCustomMenu menu(rc.Width(), rc.Height());

  menu.AddMenuItems({_T("Standard"), _T("DICOM")});
  if (auto res = menu.TrackPopupMenu(rc.left, rc.top); res == 0) {
    SetMsgHandled(FALSE);
  }
  else if (res == 1) {
    auto obj = CreateObj<ITraceyDicomConfig>();
    obj->Change(AfxGetMainWnd()->GetSafeHwnd());
  }
}
