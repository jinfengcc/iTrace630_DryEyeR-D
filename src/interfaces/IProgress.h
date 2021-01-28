#pragma once

#include "TraceyObj.h"

TRACEYOBJ("7e1772da-3719-46e9-8b35-30abd11deb47") IProgress : public IUnknown
{
  struct Options
  {
    const char *title;
    bool        showCancel;
    bool        showProgress;
    const char *status;
  };

  virtual bool Initialize(HWND parent, Options options) = 0;
  virtual void SetTitle(const char *msg)                = 0;
  virtual void SetTitle(const wchar_t *msg)             = 0;
  virtual void SetStatus(const char *msg)               = 0;
  virtual void SetStatus(const wchar_t *msg)            = 0;
  virtual void SetProgressRange(int low, int hi)        = 0;
  virtual void SetProgress(int perc)                    = 0;
  virtual bool CancelRequested() const                  = 0;

private:
};

template <class P, class S, class... Args>
inline void SetTitle(P &p, const S *s, Args... args)
{
  auto text = fmt::format(s, args...);
  p->SetTitle(text.c_str());
}

template <class P, class S, class... Args>
inline void SetStatus(P &p, const S *s, Args... args)
{
  auto text = fmt::format(s, args...);
  p->SetStatus(text.c_str());
}
