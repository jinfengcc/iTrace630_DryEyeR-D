#include "pch.h"
#include "Progress.h"

bool Progress::Initialize(HWND parent, Options options)
{
  m_dlg = std::make_unique<CProgressDlg>(parent, options);
  return m_dlg->IsWindow();
}

void Progress::SetTitle(const char *msg)
{
  m_dlg->SetTitle(msg);
}

void Progress::SetStatus(const char *msg)
{
  m_dlg->SetStatus(msg);
}

void Progress::SetTitle(const wchar_t *msg)
{
  m_dlg->SetTitle(msg);
}

void Progress::SetStatus(const wchar_t *msg)
{
  m_dlg->SetStatus(msg);
}

void Progress::SetProgressRange(int low, int hi)
{
  m_dlg->SetRange(low, hi);
}

void Progress::SetProgress(int perc)
{
  m_dlg->SetProgressPos(perc);
}

bool Progress::CancelRequested() const
{
  return m_dlg->CheckCancelButton();
}
