#pragma once

class CWindowPaintImpl
  : public CWindowImpl<CWindowPaintImpl>
  , public CBufferedPaintImpl<CWindowPaintImpl>
{
public:
  DECLARE_WND_CLASS(L"MapWindow")

  CWindowPaintImpl() = default;

  BOOL SubclassWindow(HWND hWnd, std::function<void(CDCHandle, const RECT &)> &&paintFunc)
  {
    m_paintFunc = paintFunc;
    return CWindowImpl<CWindowPaintImpl>::SubclassWindow(hWnd);
  }

  void DoPaint(CDCHandle dc, RECT &rect)
  {
    GetClientRect(&rect);
    m_paintFunc(dc, rect);
  }

private:
  std::function<void(CDCHandle, const RECT &)> m_paintFunc;

  BEGIN_MSG_MAP(CWindowPaintImpl)
    CHAIN_MSG_MAP(CBufferedPaintImpl<CWindowPaintImpl>)
    FORWARD_NOTIFICATIONS();
  END_MSG_MAP()
};
