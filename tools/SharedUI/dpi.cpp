#include "pch.h"
#include "dpi.h"

// clang-format off
enum MONITOR_DPI_TYPE {
   MDT_EFFECTIVE_DPI = 0,
   MDT_ANGULAR_DPI   = 1,
   MDT_RAW_DPI       = 2,
   MDT_DEFAULT       = MDT_EFFECTIVE_DPI
};
// clang-format on

struct CDPI::ShLibFuncs {
  ShLibFuncs()
  {
    if (HINSTANCE h = LoadLibraryA("shcore.dll"); h != nullptr) {
      (FARPROC &)m_spi = GetProcAddress(h, "SetProcessDpiAwareness");
      (FARPROC &)m_gpi = GetProcAddress(h, "GetProcessDpiAwareness");
      (FARPROC &)m_gdm = GetProcAddress(h, "GetDpiForMonitor");
    }
  }

  using SetProcessDpiAwareness = HRESULT(__stdcall *)(PROCESS_DPI_AWARENESS value);
  using GetProcessDpiAwareness = HRESULT(__stdcall *)(HANDLE hprocess, PROCESS_DPI_AWARENESS *value);
  using GetDpiForMonitor = HRESULT(__stdcall *)(_In_ HMONITOR hmonitor, _In_ MONITOR_DPI_TYPE dpiType, _Out_ UINT *dpiX, _Out_ UINT *dpiY);

  SetProcessDpiAwareness m_spi = nullptr;
  GetProcessDpiAwareness m_gpi = nullptr;
  GetDpiForMonitor       m_gdm = nullptr;
};

CDPI::CDPI()
{
  m_nScaleFactor    = 0;
  m_nScaleFactorSDA = 0;
  m_Awareness       = PROCESS_DPI_UNAWARE;

  SetScale(96);
  if (!LoadShCoreLib())
    return;

  // Get the DPI for the main monitor, and set the scaling factor
  SetAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
  auto hMonitor = MonitorFromPoint({1, 1}, MONITOR_DEFAULTTONEAREST);
  UINT dpix = 0, dpiy = 0;
  if (auto hr = m_funcs->m_gdm(hMonitor, MDT_EFFECTIVE_DPI, &dpix, &dpiy); hr == S_OK)
    SetScale(dpix);
}

CDPI::~CDPI() {}

PROCESS_DPI_AWARENESS CDPI::GetAwareness()
{
  if (m_funcs) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId());
    m_funcs->m_gpi(hProcess, &m_Awareness);
  }
  return m_Awareness;
}

void CDPI::SetAwareness(PROCESS_DPI_AWARENESS awareness)
{
  if (!m_funcs)
    return;

  if (HRESULT hr = m_funcs->m_spi(awareness); hr == S_OK)
    m_Awareness = awareness;
  else
    throw std::runtime_error("Unable to set DpiAwarness");
}

bool CDPI::LoadShCoreLib()
{
  auto f = std::make_unique<ShLibFuncs>();
  if (f->m_spi != nullptr)
    m_funcs = std::move(f);
  return m_funcs != nullptr;
}
