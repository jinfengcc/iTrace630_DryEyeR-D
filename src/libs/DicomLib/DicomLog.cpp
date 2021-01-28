#include "pch.h"
#include "DicomLog.h"
#include "libs/CommonLib/ShrinkFIle.h"

namespace {
  const DWORD MAX_LOG_FILE = 5'000'000;
};

DicomLog::DicomLog()
  : m_logFile(app::GetAppPath(app::Type::dicomLogFile))
{
}

DicomLog::DicomLog(const fs::path &logFile)
  : m_logFile(logFile)
{
}

void DicomLog::Log(LogLevel logLevel, const char *txt, const _com_error &e)
{
  if (IsWindow(dilascia::Window()) || IsEnabled(logLevel)) {
    CStringA s((const char *)e.Description());

    auto parts = StringSplit(s.GetString(), "\n");

    if (parts.size() > 1) {
      auto span = std::span(&parts[1], parts.size() - 1);
      auto jj   = fmt::format("{} [{}]", parts[0], fmt::join(span, ","));
      Log(logLevel, "{}: {}", txt, jj);
    }
    else {
      Log(logLevel, "{}: {}", txt, s.GetString());
    }
  }
}

void DicomLog::Preample(fmt::memory_buffer &buf, LogLevel ll)
{
  auto now = COleDateTime::GetCurrentTime();

  fmt::format_to(buf, "{}-{:02d}-{:02d} ", now.GetYear(), now.GetMonth(), now.GetDay());
  fmt::format_to(buf, "{:02d}:{:02d}:{:02d} ", now.GetHour(), now.GetMinute(), now.GetSecond());

  std::array ls = {"[????]", "[Info]", "[Warn]", "[Err ]", "[    ]"};
  auto       n  = static_cast<unsigned>(ll);

  fmt::format_to(buf, "{}: ", n < ls.size() ? ls[n] : ls.back());
}

void DicomLog::SendLog(LogLevel logLevel, std::string_view sv)
{
  TkTraceWinPrintEx("dicom", "{}\n", sv);

  if (IsEnabled(logLevel)) {
    ShrinkFile(m_logFile, MAX_LOG_FILE);

    auto fs = std::ofstream(m_logFile.string(), std::ofstream::out | std::ofstream::app);
    fs << sv.data() << "\n";
  }
}
