#include "pch.h"
#include "GdiplusInitializer.h"

GdiplusInitializer::GdiplusInitializer()
{
  HRESULT hRes = ::CoInitialize(NULL);
  ATLASSERT(SUCCEEDED(hRes));

  Gdiplus::GdiplusStartupInput gdiplusStartupInput;
  Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}

GdiplusInitializer::~GdiplusInitializer()
{
  Gdiplus::GdiplusShutdown(m_gdiplusToken);
  ::CoUninitialize();
}
