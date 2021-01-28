#pragma once

#include "TkTraceWin.h"
#include <string>

// ID sent as COPYDATASRUCT::dwData to identify the WM_COPYDATA message
// as coming from an app using TRACEWIN.
#define ID_COPYDATA_TRACEMSGA MAKELONG(MAKEWORD('t', 'w'), MAKEWORD('i', 'n'))
#define ID_COPYDATA_TRACEMSGW (ID_COPYDATA_TRACEMSGA + 1)
#define ID_COPYDATA_TRACE_ID MAKEWORD('a', 't')
#define ID_COPYDATA_TRACE_IMG MAKEWORD('i', 'm')
#define COPYDATA_TRACE_ID_PRE "TraceWinId{60442CBF-074D-4394-A9C4-F6CD66120836}:"

#pragma pack(push)
#pragma pack(4)
struct IMGDATA
{
  DWORD             dwVersion;
  char              szTitle[512];
  std::uint16_t     cx, cy;
  dilascia::ImgType pixelFormat;
  std::uint8_t      data[1];
};
#pragma pack(pop)

inline WORD GetTraceId(const char *id)
{
  char sz[sizeof(COPYDATA_TRACE_ID_PRE) + 64];
  strcpy_s(sz, COPYDATA_TRACE_ID_PRE);
  strcat_s(sz, id);
  return static_cast<WORD>(GlobalAddAtomA(sz));
}

inline std::string GetTraceId(WORD atom)
{
  char sz[sizeof(COPYDATA_TRACE_ID_PRE) + 64];
  if (GlobalGetAtomNameA(static_cast<ATOM>(atom), sz, sizeof(sz)) > sizeof(COPYDATA_TRACE_ID_PRE))
    return sz + sizeof(COPYDATA_TRACE_ID_PRE) - 1;

  return "TraceId(????)";
}
