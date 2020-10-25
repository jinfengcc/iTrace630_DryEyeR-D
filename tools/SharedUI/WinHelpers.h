
#pragma once

#include <vector>
#include <optional>

CRect                            GetSameAspectRect(CRect rc, const CRect &refRect);
std::vector<HWND>                WindowAreaControls(HWND hWnd, int nIDMarker);
std::unique_ptr<Gdiplus::Bitmap> Mat2Bitmap(const cv::Mat &img);
void                             DrawImage(CDCHandle dc, const RECT &rc, const cv::Mat &img);
void                             DrawImage(Gdiplus::Graphics &g, const RECT &rc, const cv::Mat &img);

inline CSize GetTextSize(HFONT font, const TCHAR *text)
{
  CDCHandle dc = GetDC(nullptr);
  HFONT     hf = dc.SelectFont(font);
  CSize     size;
  dc.GetTextExtent(text, _tcslen(text), &size);
  ReleaseDC(nullptr, dc);

  return size;
}

inline int GetTextHeight(HFONT font)
{
  return GetTextSize(font, _T("A")).cy;
}

inline BOOL IsShiftKeyPressed()
{
  return ::GetAsyncKeyState(VK_SHIFT) < 0;
}

inline BOOL IsCtrlKeyPressed()
{
  return ::GetAsyncKeyState(VK_CONTROL) < 0;
}

#ifdef BEGIN_MSG_MAP_EX
#undef BEGIN_MSG_MAP_EX

template <class Func>
inline BOOL InvokeAndHandleException(HWND w, Func &&func)
{
  try {
    return func();
  }
  catch (const std::exception &e) {
    MessageBoxA(w, e.what(), "Error", MB_OK);
    return FALSE;
  }
}

inline CWindow GetFieldLabel(HWND field)
{
  auto label = GetNextWindow(field, GW_HWNDPREV);
  if (IsWindow(label)) {
    TCHAR szClassName[256];
    ::GetClassName(label, szClassName, 255);
    if (_tcsicmp(szClassName, _T("Static")) == 0)
      return label;
  }
  return {};
}

inline CWindow GetFieldLabel(HWND parent, int idc)
{
  return GetFieldLabel(::GetDlgItem(parent, idc));
}

template<class T>
std::optional<T> GetValue(HWND hWnd)
{
  TCHAR sz[101];
  if (::GetWindowText(hWnd, sz, 100) > 0) {
    TCHAR *p;
    auto val = _tcstod(sz, &p);
    if (*p == 0)
      return {static_cast<T>(val)};
  }
  return {};
}


// clang-format off
#define BEGIN_MSG_MAP_EX(theClass) \
public: \
	BOOL m_bMsgHandled{}; \
	/* "handled" management for cracked handlers */ \
	BOOL IsMsgHandled() const \
	{ \
		return m_bMsgHandled; \
	} \
	void SetMsgHandled(BOOL bHandled) \
	{ \
		m_bMsgHandled = bHandled; \
	} \
	BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID = 0) override \
	{ \
		BOOL bOldMsgHandled = m_bMsgHandled; \
		BOOL bRet = FALSE; \
    InvokeAndHandleException(hWnd, [&]() { return _ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult, dwMsgMapID); }); \
		m_bMsgHandled = bOldMsgHandled; \
		return bRet; \
	} \
	BOOL _ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID) \
	{ \
		BOOL bHandled = TRUE; \
		(hWnd); \
		(uMsg); \
		(wParam); \
		(lParam); \
		(lResult); \
		(bHandled); \
		switch(dwMsgMapID) \
		{ \
		case 0:
// clang-format on
#endif
