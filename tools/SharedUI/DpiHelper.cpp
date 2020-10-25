#include "pch.h"
#include "DpiHelper.h"

DpiHelper::DpiHelper()
{
   HINSTANCE h = LoadLibraryA("shcore.dll");
   if (h == nullptr)
      return;

   (FARPROC &)m_spi = GetProcAddress(h, "SetProcessDpiAwareness");
   (FARPROC &)m_gpi = GetProcAddress(h, "GetProcessDpiAwareness");
}


DpiHelper::~DpiHelper()
{
}

bool DpiHelper::IsDpiAware() const
{
   if (m_gpi == nullptr)
      return false;

   PROCESS_DPI_AWARENESS a;
   if (m_gpi(NULL, &a) != S_OK)
      return false;

   return a != PROCESS_DPI_UNAWARE;
}

void DpiHelper::SetDpiAware(bool yes)
{
   if (m_spi == nullptr)
      return;

   if (IsDpiAware())
      return;

   m_spi(yes ? PROCESS_PER_MONITOR_DPI_AWARE : PROCESS_DPI_UNAWARE);
}
