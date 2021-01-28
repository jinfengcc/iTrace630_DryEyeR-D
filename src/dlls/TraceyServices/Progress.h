#pragma once

#include <interfaces/IProgress.h>
#include "ProgressDlg.h"

class Progress : public Implements<IProgress>
{
public:
  Progress()
    : Implements<IProgress>("IProgress")
  {
  }

  bool Initialize(HWND parent, Options options) override;
  void SetTitle(const char *msg) override;
  void SetStatus(const char *msg) override;
  void SetTitle(const wchar_t *msg) override;
  void SetStatus(const wchar_t *msg) override;
  void SetProgressRange(int low, int hi) override;
  void SetProgress(int perc = 0) override;
  bool CancelRequested() const override;

private:
  std::unique_ptr<CProgressDlg> m_dlg;
};
