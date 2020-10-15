#include "pch.h"
#include "Logger.h"
#include "TkTraceWin.h"


bool Logger::Enabled(Type type) const
{
  if (DILASCIA_VISIBLE())
    return true;

  return EnabledImpl(type);
}

void Logger::Write(Type type, std::string_view msg)
{
  auto str = fmt::format("[{}]: {}\n", GetTypeName(type), msg);
  if (type == ILogger::Type::Diag) {
    DILASCIA_TRACE_EX("Diag", str);
  }
  else {
    DILASCIA_TRACE(str);
  }

  if (EnabledImpl(type))
    WriteToFile(str);
}

void Logger::Enable(Type type, bool enable)
{
  if (type == ILogger::Type::All__) {
    m_enabled = 0xFFFF & ~Mask(ILogger::Type::Diag);
  }
  else if (type == ILogger::Type::Default__) {
    m_enabled = 0xFFFF & ~(Mask(ILogger::Type::Info) | Mask(ILogger::Type::Diag));
  }
  else {
    if (enable)
      m_enabled |= Mask(type);
    else
      m_enabled &= ~Mask(type);
  }
}

void Logger::SetOutFile(std::string_view fileName, DWORD maxFile)
{
  auto path = fs::path(fileName);
  if (fs::exists(path.parent_path())) {
    std::lock_guard lock(m_mutex);
    m_outFile     = std::move(path);
    m_maxFileSize = maxFile;
  }
}

const char *Logger::GetTypeName(Type type)
{
  // clang-format off
  static std::array names = {
    "Diag ",
    "Info ",
    "Warn ",
    "Error",
    "THROW",
    "BUG  ",
    "ASSRT",
    "FATAL"
  };
  // clang-format on

  auto n = static_cast<unsigned>(type);
  return n < names.size() ? names[n] : "?????";
}

void Logger::WriteToFile(std::string_view str)
{
  std::lock_guard lock(m_mutex);
  { // BLOCK
    auto fs = std::ofstream(m_outFile.string(), std::ofstream::out | std::ofstream::app);
    fs << str.data();
  }
  ShrinkFile();
}

void Logger::ShrinkFile(const char *pDelim)
{
  if (m_maxFileSize == 0 || !fs::exists(m_outFile))
    return;

  auto dwFileSize = fs::file_size(m_outFile);
  if (dwFileSize < m_maxFileSize)
    return;

  CAtlFile file;
  if (file.Create(m_outFile.wstring().c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, OPEN_EXISTING) != S_OK)
    return;

  CAtlFileMapping map;
  if (map.MapFile(file, static_cast<SIZE_T>(dwFileSize), 0, PAGE_READWRITE | SEC_COMMIT, FILE_MAP_ALL_ACCESS) != S_OK)
    return;

  // 10% hysteresis
  auto hyst = m_maxFileSize - m_maxFileSize * 10 / 100;

  auto *begin = static_cast<char *>(map);
  auto *end   = strstr(begin + hyst, pDelim);
  if (end == nullptr)
    return;

  end += strlen(pDelim);
  auto skiplen = std::distance(begin, end);
  memcpy(begin, end, gsl::narrow<size_t>(dwFileSize - skiplen));

  map.Unmap();

  file.SetSize(dwFileSize - skiplen);
}
