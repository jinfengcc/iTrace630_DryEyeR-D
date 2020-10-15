#include "pch.h"
#include "TkTraceWin.h"
#include "TkTraceWinParam.h"

namespace Gdiplus {
  using std::max;
  using std::min;
} // namespace Gdiplus

#include <gsl/gsl>
#include <gdiplus.h>

namespace {
}

namespace dilascia {
  namespace details {
    HWND g_TraceWinHandle;

    HWND FindTraceWindow()
    {
      const char *classNames[] = {
        "MfxTraceWindow"
        // "TraceWin TRACE Window",
      };

      for (auto cn : classNames) {
        g_TraceWinHandle = ::FindWindowA(cn, nullptr);
        if (g_TraceWinHandle)
          break;
      }

      return g_TraceWinHandle;
    }

    void WriteImpl(DWORD id, const void *data, int size)
    {
      // const DWORD ID_COPYDATA_TRACEMSG = MAKELONG(MAKEWORD('t', 'w'), MAKEWORD('i', 'n'));
      if (auto hTraceWnd = Window(); IsWindow(hTraceWnd)) {
        COPYDATASTRUCT cds;
        cds.dwData = id;
        cds.cbData = gsl::narrow<UINT>(size);
        cds.lpData = const_cast<void *>(data);
        ::SendMessage(hTraceWnd, WM_COPYDATA, (WPARAM)0, (LPARAM)&cds);
      }
    }
  } // namespace details

  void Write(const char *ids, std::string_view s)
  {
    static_assert(sizeof(ATOM) == sizeof(WORD));

    if (ids == nullptr) {
      details::WriteImpl(ID_COPYDATA_TRACEMSGA, s.data(), s.size());
    }
    else {
      auto id = MAKELONG(GetTraceId(ids), ID_COPYDATA_TRACE_ID);
      details::WriteImpl(id, s.data(), s.size());
    }
  }

  int PixelSize(ImgType type)
  {
    switch (type) {
    case ImgType::FLOAT:
      return sizeof(float);
    case ImgType::DOUBLE:
      return sizeof(double);
    case ImgType::BYTE:
      return sizeof(BYTE);
    case ImgType::RGB:
      return 3;
    // case ImgType::RGBA:
    //  return 4;
    default:
      return 1;
    }
  }

  void ImgWrite(const char *ids, ImgType pixelFormat, SIZE size, const void *data)
  {
    using namespace dilascia;

    if (!IsWindow(Window()))
      return;

    int               datalen = size.cx * size.cy * PixelSize(pixelFormat);
    std::vector<BYTE> buf(sizeof(IMGDATA) + datalen);
    auto *            img = reinterpret_cast<IMGDATA *>(&buf[0]);

    img->dwVersion   = 0x100;
    img->cx          = gsl::narrow<std::uint16_t>(size.cx);
    img->cy          = gsl::narrow<std::uint16_t>(size.cy);
    img->pixelFormat = pixelFormat;

    memcpy(img->data, data, datalen);

    static_assert(sizeof(ATOM) == sizeof(WORD));

    auto id = MAKELONG(GetTraceId(ids), ID_COPYDATA_TRACE_IMG);
    details::WriteImpl(id, &buf[0], buf.size());
  }
} // namespace dilascia
