#include "pch.h"
#include "SignalUI.h"

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

#define GUID _T("531AF056-B8BE-4c4c-B786-AC608DF0DF12")

namespace {
  // Hidden window implementation
  DWORD mainThreadId = GetCurrentThreadId();
  HWND  hiddenWnd;
  UINT  WM_PRIVATE = ::RegisterWindowMessage(_T("WM_") GUID);

  LRESULT CALLBACK HiddenWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    if (uMsg == WM_PRIVATE) {
      using sig::details::SignalUiBase;
      auto obj = reinterpret_cast<SignalUiBase *>(lParam);
      obj->OnHiddenWndMessage();
      return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }

  HWND CreateHiddenWindow()
  {
    static const TCHAR *class_name = _T("WindowClass") GUID;

    WNDCLASSEX wx    = {sizeof(WNDCLASSEX)};
    wx.lpfnWndProc   = &HiddenWndProc;
    wx.hInstance     = HINST_THISCOMPONENT;
    wx.lpszClassName = class_name;
    if (!RegisterClassEx(&wx))
      return nullptr;

    return CreateWindowEx(0, class_name, class_name, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);
  }

  void PumpMessages()
  {
    MSG msg;
    while (PeekMessage(&msg, hiddenWnd, 0, 0, TRUE)) {
      DispatchMessage(&msg);
    }
  }

} // namespace

namespace sig {
  bool UiThread()
  {
    return mainThreadId == GetCurrentThreadId();
  }

  namespace details {
    SignalUiBase::SignalUiBase()
    {
      if (hiddenWnd == nullptr)
        hiddenWnd = CreateHiddenWindow();
    }

    // NOTE: We assume that obj will be alive long enough to finish processing in the WndProc
    void SignalUiBase::Post(SignalUiBase *obj)
    {
      //static BOOL hasMsgQueue = PostThreadMessage(UiThread(), WM_APP, 0, 0);
      if (hiddenWnd)
        PostMessage(hiddenWnd, WM_PRIVATE, 0, reinterpret_cast<LPARAM>(obj));
    }
  } // namespace details
} // namespace sig
