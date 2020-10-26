#pragma once

#include <gdiplus.h>
#include <atlstr.h>
#include <atlwin.h>
#include <wtl/atlapp.h>
//#include <wtl/atlmisc.h>
#include <wtl/atlcrack.h>

class CSplashScreenImpl : public CWindowImpl<CSplashScreenImpl, ATL::CWindow, CNullTraits>
{
public:
  using Callback = std::function<void(Gdiplus::Graphics &, Gdiplus::RectF)>;

  CSplashScreenImpl(int resId, Callback cb);
  ~CSplashScreenImpl();

  int GetWidth() const
  {
    return m_size.cx;
  }
  int GetHeight() const
  {
    return m_size.cy;
  }

private:
  Gdiplus::Bitmap     m_bitmap;
  Callback            m_callback;
  SIZE                m_size{};

  BOOL OnEraseBkgnd(CDCHandle dc);
  void OnPaint(CDCHandle);

  void Initialize();
  void DoPaint(CDCHandle dc, RECT &rcClip);
  void PumpMessages();

  BEGIN_MSG_MAP_EX(CSplashScreen)
    MSG_WM_ERASEBKGND(OnEraseBkgnd)
    MSG_WM_PAINT(OnPaint)
  END_MSG_MAP()
};


