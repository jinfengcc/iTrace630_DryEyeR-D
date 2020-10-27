#pragma once

#include <gdiplus.h>
#include <atlstr.h>
#include <atlwin.h>
#include <wtl/atlapp.h>
//#include <wtl/atlmisc.h>
#include <wtl/atlcrack.h>

#include "atlgdix.h"

class CSplashScreenImpl
  : public CWindowImpl<CSplashScreenImpl, ATL::CWindow, CNullTraits>
  , public COffscreenDraw<CSplashScreenImpl>
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

  void DoPaint(CDCHandle dc);
private:
  Gdiplus::Bitmap     m_bitmap;
  Callback            m_callback;
  SIZE                m_size{};

  void Initialize();
  void PumpMessages();

  BEGIN_MSG_MAP_EX(CSplashScreen)
    CHAIN_MSG_MAP(COffscreenDraw<CSplashScreenImpl>)
  END_MSG_MAP()
};


