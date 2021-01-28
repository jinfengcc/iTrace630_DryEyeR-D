#include "pch.h"
#include "ProgressDlg.h"

CProgressDlg::CProgressDlg(HWND hParent, const Options &options)
{
  if (!Create(hParent))
    return;

  if (hParent != nullptr && ::IsWindowEnabled(hParent)) {
    ::EnableWindow(hParent, FALSE);
    m_parent = hParent;
  }
  GetDlgItem(IDCANCEL).ShowWindow(options.showCancel ? SW_SHOW : SW_HIDE);
  GetDlgItem(IDC_PROGRESS).ShowWindow(options.showProgress ? SW_SHOW : SW_HIDE);
  SetTitle(options.title);
  SetStatus(options.status);

  CenterWindow(hParent);
  ShowWindow(SW_SHOW);
  UpdateWindow();
}

CProgressDlg::~CProgressDlg()
{
  if (m_parent.IsWindow()) {
    m_parent.EnableWindow(TRUE);
    DestroyWindow();
  }
}

void CProgressDlg::SetTitle(const char *msg)
{
  ::SetDlgItemTextA(m_hWnd, IDC_TITLE, msg ? msg : "");
  PumpMessages();
}

void CProgressDlg::SetStatus(const char *msg)
{
  ::SetDlgItemTextA(m_hWnd, IDC_STATUS, msg ? msg : "");
  PumpMessages();
}

void CProgressDlg::SetTitle(const wchar_t *msg)
{
  ::SetDlgItemTextW(m_hWnd, IDC_TITLE, msg ? msg : L"");
  PumpMessages();
}

void CProgressDlg::SetStatus(const wchar_t *msg)
{
  ::SetDlgItemTextW(m_hWnd, IDC_STATUS, msg ? msg : L"");
  PumpMessages();
}

void CProgressDlg::SetRange(int low, int hi)
{
  m_Progress.SetRange32(low, hi);
}

void CProgressDlg::SetProgressPos(int nPos)
{
  if (!m_Progress.IsWindowVisible())
    m_Progress.ShowWindow(SW_SHOW);

  m_Progress.SetPos(nPos);
  PumpMessages();
}

bool CProgressDlg::CheckCancelButton()
{
  PumpMessages();
  auto res = m_cancel;
  m_cancel = false;
  return res;
}

BOOL CProgressDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
  m_Progress = GetDlgItem(IDC_PROGRESS);
  m_Progress.SetRange(0, 100);

  SetTitle("iTrace");
  SetStatus("");

  return TRUE;
}

void CProgressDlg::OnCancel(UINT uNotifyCode, int nID, CWindow wndCtl)
{
  m_cancel = true;
}

void CProgressDlg::PumpMessages()
{
  // Must call Create() before using the dialog
  ATLASSERT(m_hWnd != nullptr);

  MSG msg;
  // Handle dialog messages
  while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
    if (!IsDialogMessage(&msg)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
}
