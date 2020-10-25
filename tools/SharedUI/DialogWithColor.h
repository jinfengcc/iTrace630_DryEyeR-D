#pragma once

#include "Globals.h"

template <typename Base>
class CDialogWithColor : public CDialogImpl<Base>
{
public:
  CDialogWithColor()
  {
  }

private:
  HBRUSH OnCtlColorStatic(CDCHandle dc, HWND hWnd)
  {
    const Globals &bd = Globals::Instance();
    dc.SetBkColor(bd.bkColor);
    return static_cast<HBRUSH>(bd.bkBrush);
  }

  BEGIN_MSG_MAP_EX(CDialogWithColor)
    //MSG_WM_CTLCOLOREDIT(OnCtlColorStatic)
    //MSG_WM_CTLCOLORSTATIC(OnCtlColorStatic)
    //MSG_WM_CTLCOLORBTN(OnCtlColorStatic)
    //MSG_WM_CTLCOLORDLG(OnCtlColorStatic)
  END_MSG_MAP()
};
