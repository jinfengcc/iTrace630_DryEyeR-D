#pragma once

#include <wtl/atlapp.h>
#include <wtl/atlmisc.h>

namespace WinPlacement {
  bool    Load(const TCHAR *psz, WINDOWPLACEMENT &wp);
  bool    Load(const TCHAR *psz, HWND hWnd);
  CString Save(WINDOWPLACEMENT wp);
  CString Save(HWND hWnd);

} // namespace WinPlacement
