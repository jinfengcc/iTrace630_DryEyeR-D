#pragma once

class DpiHelper
{
public:
   DpiHelper();
   ~DpiHelper();

   bool IsDpiAware() const;
   void SetDpiAware(bool yes);

private:
   enum PROCESS_DPI_AWARENESS {
      PROCESS_DPI_UNAWARE = 0,
      PROCESS_SYSTEM_DPI_AWARE = 1,
      PROCESS_PER_MONITOR_DPI_AWARE = 2
   };

   using SetProcessDpiAwareness = HRESULT(__stdcall *)(PROCESS_DPI_AWARENESS value);
   using GetProcessDpiAwareness = HRESULT(__stdcall *)(HANDLE hprocess, PROCESS_DPI_AWARENESS *value);

   SetProcessDpiAwareness m_spi = nullptr;
   GetProcessDpiAwareness m_gpi = nullptr;
};

