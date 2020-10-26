// HicSplashScreen.cpp : Implementation of CSplashScreen

#include "stdafx.h"
#include "SplashScreen.h"
#include "AppVersion.h"

namespace ATL {
  using CString = ::CString;
}

#include <libs/CommonLib/SplashScreenImpl.h>
#include "Licensing.h"

CSplashScreen::CSplashScreen(int resId)
{
  auto callback = [this](Gdiplus::Graphics &g, Gdiplus::RectF rc) { DrawOther(g, rc); };
  m_pimpl       = std::make_unique<CSplashScreenImpl>(resId, callback);
}

CSplashScreen::~CSplashScreen()
{
  // No need to delete object
  m_pimpl.release();
}

void CSplashScreen::ShowSplash(bool show)
{
  m_pimpl->ShowWindow(show ? SW_SHOW : SW_HIDE);
}

void CSplashScreen::SetTextMessage(const TCHAR *pszMessage)
{
  // m_sTextMessage = pszMessage ? pszMessage : _T("");
  // Invalidate();
  // PumpMessages();
}

bool CSplashScreen::IsVisible() const
{
  return m_pimpl->IsWindowVisible();
}

void CSplashScreen::DrawOther(Gdiplus::Graphics &g, Gdiplus::RectF rc)
{
  using namespace Gdiplus;

  StringFormat format;
  format.SetAlignment(StringAlignmentNear);
  format.SetLineAlignment(StringAlignmentFar);

  SolidBrush brush(Gdiplus::Color(135, 120, 180));

  rc.Inflate(-31, -31);
  {
    Font font(L"Arial", 18, FontStyleBoldItalic, UnitPixel);
    auto ver = fmt::format(L"Version {} {}", FullSoftwareVersion().GetString(), Licensing.LicenseName().GetString());
    g.DrawString(ver.c_str(), ver.size(), &font, rc, &format, &brush);
  }

  rc.Inflate(0, 21);
  if (false)
  {
    Font font(L"Arial", 12, FontStyleItalic, UnitPixel);
    auto ver = fmt::format(L"{}", L"1234567890123");
    g.DrawString(ver.c_str(), ver.size(), &font, rc, &format, &brush);
  }
}
