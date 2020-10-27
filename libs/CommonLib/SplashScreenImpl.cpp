#include "pch.h"
#include "SplashScreenImpl.h"
#include <wtl/atlmisc.h>


CSplashScreenImpl::CSplashScreenImpl(int resId, Callback cb)
  : m_bitmap(GetModuleHandle(nullptr), MAKEINTRESOURCE(resId))
  , m_callback(cb)
{
  m_hWnd = Create(nullptr);

  Initialize();
  ShowWindow(SW_SHOW);
  PumpMessages();
}

CSplashScreenImpl::~CSplashScreenImpl()
{
  DestroyWindow();
}

void CSplashScreenImpl::Initialize()
{
  m_size.cx = m_bitmap.GetWidth();
  m_size.cy = m_bitmap.GetHeight();

  // Max width ~ 90% of screen
  int maxw = 90 * GetSystemMetrics(SM_CXSCREEN) / 100;

  if (m_size.cx > maxw) {
    m_size.cy = m_size.cx * maxw / m_size.cx;
    m_size.cx = maxw;
  }

  SetWindowPos(HWND_TOPMOST, 0, 0, m_size.cx, m_size.cy, SWP_NOMOVE);
  CenterWindow();
}

void CSplashScreenImpl::DoPaint(CDCHandle dc)
{
  CRect rc;
  GetClientRect(&rc);

  using namespace Gdiplus;

  Graphics g(dc);
  g.DrawImage(&m_bitmap, rc.left, rc.top, rc.Width(), rc.Height());

  auto rcf = RectF(REAL(rc.left), REAL(rc.top), REAL(rc.Width()), REAL(rc.Height()));
  if (m_callback)
    m_callback(g, rcf);
}

void CSplashScreenImpl::PumpMessages()
{
  MSG msg;
  while (::PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE)) {
    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
  }
}
