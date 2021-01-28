#include "pch.h"
#include "AppInitializer.h"

#pragma comment(lib, "gdiplus.lib")

AppInitializer::AppInitializer()
{
  HRESULT hRes = ::CoInitialize(nullptr);
  ATLASSERT(SUCCEEDED(hRes));

  Gdiplus::GdiplusStartupInput gdiplusStartupInput;
  Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}

AppInitializer::~AppInitializer()
{
  Gdiplus::GdiplusShutdown(m_gdiplusToken);
  ::CoUninitialize();
}
