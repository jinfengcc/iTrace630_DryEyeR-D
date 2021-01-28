#pragma once

#include "resource.h"
#include "interfaces/IProgress.h"

class CProgressDlg : public CDialogImpl<CProgressDlg>
{
public:
  enum { IDD = IDD_PROGRESS };

  using Options = IProgress::Options;

  CProgressDlg(HWND hWndParent, const Options &options);
  ~CProgressDlg();

  void SetTitle(const char *msg);
  void SetStatus(const char *msg);
  void SetTitle(const wchar_t *msg);
  void SetStatus(const wchar_t *msg);
  void SetRange(int low, int hi);
  void SetProgressPos(int nPos);
  bool CheckCancelButton();

private:
  CWindow          m_parent{};
  CProgressBarCtrl m_Progress;
  bool             m_cancel{};

  BOOL OnInitDialog(HWND hWnd, LPARAM lParam);
  void OnCancel(UINT uNotifyCode, int nID, CWindow wndCtl);

  void PumpMessages();

  BEGIN_MSG_MAP_EX(CProgressDlg)
    MSG_WM_INITDIALOG(OnInitDialog)
    COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
  END_MSG_MAP()
};
