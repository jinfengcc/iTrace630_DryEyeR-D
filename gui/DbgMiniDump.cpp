#include "stdafx.h"
#include "DbgMiniDump.h"
#include "AppVersion.h"
#include "SplashScreen.h"
#include "libs/LoggingLib/TkTraceWin.h"

#include <dbghelp.h>

class DbgMiniDump
{
public:
  static void Init()
  {
    if (!g_miniDump) {
      g_miniDump = new DbgMiniDump;
      SetUnhandledExceptionFilter(&DbgMiniDump::TopLevelFilter);
    }
  }

private:
  static inline DbgMiniDump *g_miniDump{};

  std::wstring m_dumpFile;
  std::wstring m_error;

  DbgMiniDump();

  DWORD Save(LPEXCEPTION_POINTERS exceptionPointers);
  bool  SaveImpl(LPEXCEPTION_POINTERS exceptionPointers, MINIDUMP_TYPE type);

  static LONG WINAPI TopLevelFilter(struct _EXCEPTION_POINTERS *pExceptionInfo);
};

void DbgMiniDumpInit()
{
  DbgMiniDump::Init();
}

DbgMiniDump::DbgMiniDump()
{
  const wchar_t folderName[] = LR"(C:\Tracey\MiniDump)";
  fs::create_directories(folderName);

  auto now    = COleDateTime::GetCurrentTime();
  uint y      = VERSION_YEAR;
  uint m      = VERSION_DAY / 100;
  uint d      = VERSION_DAY % 100;

  fmt::wmemory_buffer buf;

  fmt::format_to(buf, LR"({}\TDMP)", folderName);
  fmt::format_to(buf, LR"(_{:4}{:02}{:02})", y, m, d);
  fmt::format_to(buf, LR"(_{:02}{:02}{:02})", now.GetHour(), now.GetMinute(), now.GetSecond());
  fmt::format_to(buf, LR"(.mdmp)");

  m_dumpFile = fmt::to_string(buf);

  const wchar_t g_msg[] =
    L"iTracey has experienced an unexpected problem and is going to close."
    "A diagnostic dump file has been saved at this location:"
    "\n\n'{}'\n\n"
    "Please send this file to the technical support team "
    " so that they can try to fix the problem that you have experienced.";
  m_error = fmt::format(g_msg, m_dumpFile);
}

DWORD DbgMiniDump::Save(LPEXCEPTION_POINTERS exceptionPointers)
{
  DILASCIA_TRACE_EX("DMP", "Saving minidump file\n");
  const auto type = MINIDUMP_TYPE(MiniDumpWithCodeSegs | MiniDumpWithDataSegs);
  if (!SaveImpl(exceptionPointers, type)) {
    DILASCIA_TRACE_EX("DMP", "Failed saving\n");
    return EXCEPTION_CONTINUE_SEARCH;
  }

  DILASCIA_TRACE_EX("DMP", "Showing message\n");

  CSplashScreen::Instance()->HideSplash();
  MessageBoxW(nullptr, m_error.c_str(), L"iTrace", MB_OK | MB_ICONSTOP);

  return EXCEPTION_EXECUTE_HANDLER;
}

bool DbgMiniDump::SaveImpl(LPEXCEPTION_POINTERS exceptionPointers, MINIDUMP_TYPE type)
{
  using MINIDUMPWRITEDUMP = BOOL(WINAPI *)(HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, CONST PMINIDUMP_EXCEPTION_INFORMATION,
                                           CONST PMINIDUMP_USER_STREAM_INFORMATION, CONST PMINIDUMP_CALLBACK_INFORMATION);

  auto func = (MINIDUMPWRITEDUMP)GetProcAddress(LoadLibraryA("DBGHELP.DLL"), "MiniDumpWriteDump");

  if (!func)
    return false;

  HANDLE hFile = ::CreateFile(m_dumpFile.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile == INVALID_HANDLE_VALUE) {
    return false;
  }

  _MINIDUMP_EXCEPTION_INFORMATION ExInfo;

  ExInfo.ThreadId          = GetCurrentThreadId();
  ExInfo.ExceptionPointers = exceptionPointers;
  ExInfo.ClientPointers    = NULL;

  LONG retval = EXCEPTION_CONTINUE_SEARCH;

  // write the dump
  BOOL bOK = func(GetCurrentProcess(), GetCurrentProcessId(), hFile, type, &ExInfo, NULL, NULL);
  CloseHandle(hFile);
  return bOK ? true : false;
}

LONG __stdcall DbgMiniDump::TopLevelFilter(_EXCEPTION_POINTERS *pExceptionInfo)
{
  return g_miniDump->Save(pExceptionInfo);
}
