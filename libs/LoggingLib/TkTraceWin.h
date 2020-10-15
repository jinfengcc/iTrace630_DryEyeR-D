#pragma once

#include <fmt/format.h>

namespace dilascia {
  namespace details {
    extern HWND g_TraceWinHandle;
    HWND        FindTraceWindow();
  } // namespace details

  inline HWND Window()
  {
    using namespace details;
    return IsWindow(g_TraceWinHandle) ? g_TraceWinHandle : FindTraceWindow();
  }

  enum class ImgType : std::uint16_t {
    FLOAT,
    DOUBLE,
    BYTE,
    RGB,
    // RGBA,
  };

  void Write(const char *ids, std::string_view s);
  void ImgWrite(const char *ids, ImgType pixelFormat, SIZE size, const void *data);
} // namespace dilascia

template <typename... Args>
void TkTraceWinPrintEx(const char *id, std::string_view data, Args... args)
{
  if (IsWindow(dilascia::Window())) {
    auto s = fmt::format(data, args...);
    dilascia::Write(id, s);
  }
}

template <typename... Args>
void TkTraceWinPrint(std::string_view data, Args... args)
{
  TkTraceWinPrintEx(nullptr, data, args...);
}

#define DILASCIA_VISIBLE() IsWindow(dilascia::Window())
#define DILASCIA_TRACE(fmt, ...) TkTraceWinPrint(fmt, __VA_ARGS__)
#define DILASCIA_TRACE_EX(id, fmt, ...) TkTraceWinPrintEx(id, fmt, __VA_ARGS__)

inline void DILASCIA_TRACEIMG_RGB(const char *id, SIZE size, const void *data)
{
  using namespace dilascia;
  ImgWrite(id, ImgType::RGB, size, data);
}
