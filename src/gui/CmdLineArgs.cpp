#include "stdafx.h"
#include "CmdLineArgs.h"

namespace {
  static const wchar_t screenSize[] = L"ScreenSize=";
  static const wchar_t fullScreen[] = L"FullScreen=";

  const wchar_t *MatchArg(const std::wstring &arg, const wchar_t *x)
  {
    if (arg.starts_with(x))
      return &arg[wcslen(x)];
    else
      return nullptr;
  }
} // namespace

CmdLineArgs::CmdLineArgs()
{

  for (int i = 1; i < __argc; ++i) {
    std::wstring  arg(__wargv[i]);

    if (auto v = MatchArg(arg, screenSize)) {
      int cx = 0;
      if (swscanf_s(v, L"%d", &cx) == 1 && cx >= 640)
        m_screenWidth = cx;
    }

    else if (auto v = MatchArg(arg, fullScreen)) {
      if (_wcsicmp(v, L"false") == 0 || _wcsicmp(v, L"no") == 0 || _wcsicmp(v, L"0") == 0)
        m_fullScreen = false;
    }
  }
}
