#pragma once

#include <libs/CommonLib/Singleton.h>

class CSplashScreenImpl;

class CSplashScreen : public Singleton<CSplashScreen>
{
  friend class Singleton<CSplashScreen>;
  CSplashScreen(int resId);

public:
  ~CSplashScreen();

  void ShowSplash(bool show = true);
  void SetTextMessage(const TCHAR *pszMessage);
  bool IsVisible() const;

private:
  std::unique_ptr<CSplashScreenImpl> m_pimpl;

  void DrawOther(Gdiplus::Graphics &, Gdiplus::RectF rc);
};
