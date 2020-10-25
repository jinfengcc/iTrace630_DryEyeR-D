// MainViewDlg.h : interface of the CMainViewDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "resource.h"
#include "DialogWithColor.h"
#include <IView.h>
#include <interfaces/hal/IFlash.h>

class CFlashView
  : public CDialogWithColor<CFlashView>
  , public CDynamicDialogLayout<CFlashView>
  , public IView
{
public:
  CFlashView() = default;

  enum { IDD = IDD_VIEW_FLASH };

  // IView
  HWND GetHWnd() const override
  {
    return m_hWnd;
  }

private:
  using IFlash = hal::IFlash;

  BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
  void OnReadClicked(UINT uNotifyCode, int nID, CWindow wndCtl);
  void OnWriteClicked(UINT uNotifyCode, int nID, CWindow wndCtl);

  void LoadFlashSize(IFlash *flash);
  void LoadFlashContents(IFlash *flash);
  void LoadDbCalibration();

  BEGIN_MSG_MAP_EX(CFlashView)
    MSG_WM_INITDIALOG(OnInitDialog)
    COMMAND_ID_HANDLER_EX(IDC_READ, OnReadClicked)
    COMMAND_ID_HANDLER_EX(IDC_WRITE, OnWriteClicked)
    CHAIN_MSG_MAP(CDialogWithColor<CFlashView>)
    CHAIN_MSG_MAP(CDynamicDialogLayout<CFlashView>)
  END_MSG_MAP()
};
