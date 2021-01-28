#pragma once

#include "libs/CommonLib/Singleton.h"
#include "libs/LoggingLib/TkTraceWin.h"
#include <comdef.h>

class DicomLog : public Singleton<DicomLog>
{
public:
  DicomLog();
  DicomLog(const fs::path &logFile);

  using LogLevel = dicom::LogLevel;

  void SetLevel(LogLevel ll)
  {
    m_logLevel = ll;
  }

  template <class... Args>
  void Log(LogLevel logLevel, const char *txt, const Args &... args)
  {
    if (IsWindow(dilascia::Window()) || IsEnabled(logLevel)) {
      fmt::memory_buffer buf;
      Preample(buf, logLevel);
      fmt::format_to(buf, txt, args...);

      SendLog(logLevel, fmt::to_string(buf));
    }
  }

  void Log(LogLevel logLevel, const char *txt, const _com_error &e);

  auto LogFile() const
  {
    return m_logFile;
  }

private:
  fs::path m_logFile;
  LogLevel m_logLevel = LogLevel::Warn;

  static void Preample(fmt::memory_buffer &buf, LogLevel ll);
  void        SendLog(LogLevel logLevel, std::string_view sv);

  bool IsEnabled(LogLevel logLevel) const
  {
    if (m_logLevel == LogLevel::None)
      return false;

    if (static_cast<int>(logLevel) < static_cast<int>(m_logLevel))
      return false;

    return true;
  }
};

#ifndef DMLOG_DISABLE
#  define DCMLOG_Info(...)  DicomLog::Instance()->Log(dicom::LogLevel::Info, __VA_ARGS__)
#  define DCMLOG_Warn(...)  DicomLog::Instance()->Log(dicom::LogLevel::Warn, __VA_ARGS__)
#  define DCMLOG_Error(...) DicomLog::Instance()->Log(dicom::LogLevel::Error, __VA_ARGS__)
#else
#  define DCMLOG_Info(...)
#  define DCMLOG_Warn(...)
#  define DCMLOG_Error(...)
#endif
