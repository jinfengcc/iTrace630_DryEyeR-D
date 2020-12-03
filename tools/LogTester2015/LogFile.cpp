#include "LogFile.h"

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdarg.h>

const int         BUF_SIZE  = 512;
static const char logFile[] = "C:\\Tracey\\LogFile.txt";

static void LogFileImpl(const char *file, int lineNo, const char *str)
{
  std::ofstream fs(logFile, std::ios::app);
  static bool   init = false;
  if (!init) {
    init = true;
    fs << "\n-----------------------------------\n";
  }

  char buf[_MAX_PATH + 10 + BUF_SIZE] = {};
  _snprintf(buf, sizeof(buf)-1, "%s(%d): %s\n", file, lineNo, str);

  fs << buf;
}

static std::string Wchar2Char(const wchar_t *str)
{
  int len  = wcslen(str);
  int size = WideCharToMultiByte(CP_UTF8, 0, str, len, nullptr, 0, nullptr, nullptr);

  std::string strTo(size, 0);
  WideCharToMultiByte(CP_UTF8, 0, str, size, &strTo[0], size, nullptr, nullptr);

  return strTo;
}

void LogToFile(const char *file, int lineNo, const char *str, ...)
{
  char buf[BUF_SIZE] = {};

  va_list args;
  va_start(args, str);
  _vsnprintf(buf, sizeof(buf)-1, str, args);
  va_end(args);

  LogFileImpl(file, lineNo, buf);
}

void LogToFile(const char *file, int lineNo, const wchar_t *str, ...)
{
  wchar_t buf[BUF_SIZE] = {};

  va_list args;
  va_start(args, str);
  _vsnwprintf(buf, sizeof(buf)-1, str, args);
  va_end(args);

  std::string cs = Wchar2Char(buf);
  LogFileImpl(file, lineNo, &cs[0]);
}
