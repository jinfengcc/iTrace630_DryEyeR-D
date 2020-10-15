////////////////////////////////////////////////////////////////
// CWindowPlacement Copyright 1996 Microsoft Systems Journal.
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.

#include "pch.h"
#include <tchar.h>
#include "WindowPlacement.h"
// #include "WindowPlacement.h"

namespace {
  const TCHAR g_fmt[] = _T("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d");

  bool LoadImpl(const TCHAR *psz, WINDOWPLACEMENT &wp)
  {
    const int NFIELDS = 10;

    wp.length = sizeof(WINDOWPLACEMENT);
    return _stscanf_s(psz, g_fmt, &wp.showCmd, &wp.flags, &wp.ptMinPosition.x, &wp.ptMinPosition.y, &wp.ptMaxPosition.x,
                      &wp.ptMaxPosition.y, &wp.rcNormalPosition.left, &wp.rcNormalPosition.right, &wp.rcNormalPosition.top,
                      &wp.rcNormalPosition.bottom) == NFIELDS;
  }

  BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
  {
    auto mrcl = reinterpret_cast<std::vector<RECT> *>(dwData);
    mrcl->push_back(*lprcMonitor);
    return TRUE;
  }

  bool IsPlacementValid(const WINDOWPLACEMENT &wp)
  {
    std::vector<RECT> mrcl;
    if (!EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProc, (LPARAM)&mrcl))
      return false;

    for (auto &r : mrcl) {
      RECT rTemp;
      if (::IntersectRect(&rTemp, &wp.rcNormalPosition, &r))
        return true;
    }

    return false;
  }

} // namespace

namespace WinPlacement {
  bool Load(const TCHAR *psz, WINDOWPLACEMENT &wp)
  {
    if (LoadImpl(psz, wp) && IsPlacementValid(wp))
      return true;

    wp = WINDOWPLACEMENT{};
    return false;
  }

  bool Load(const TCHAR *psz, HWND hWnd)
  {
    WINDOWPLACEMENT wp{sizeof(WINDOWPLACEMENT)};
    if (!Load(psz, wp))
      return false;

    return ::SetWindowPlacement(hWnd, &wp) == TRUE;
  }

  CString Save(WINDOWPLACEMENT wp)
  {
    TCHAR m_szBuf[256];
    _stprintf_s(m_szBuf, g_fmt, wp.showCmd, wp.flags, wp.ptMinPosition.x, wp.ptMinPosition.y, wp.ptMaxPosition.x,
                wp.ptMaxPosition.y, wp.rcNormalPosition.left, wp.rcNormalPosition.right, wp.rcNormalPosition.top,
                wp.rcNormalPosition.bottom);
    return m_szBuf;
  }

  CString Save(HWND hWnd)
  {
    WINDOWPLACEMENT wp{sizeof(WINDOWPLACEMENT)};
    if (::GetWindowPlacement(hWnd, &wp))
      return Save(wp);
    else
      return L"";
  }

} // namespace WinPlacement
