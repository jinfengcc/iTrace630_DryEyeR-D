#pragma once

#include <interfaces/ILogger.h>
#include <interfaces/ILoggerSettings.h>

#include <thread>
#include <mutex>

class Logger : public ImplementsStatic<ILogger, ILoggerSettings>
{
public:
  Logger()
  {
    Enable(Type::Default__, true);
  }

  // ILogger
  bool Enabled(Type type) const override;
  void Write(Type type, std::string_view msg) override;

  // ILoggerSettings
  void Enable(Type type, bool enable = true) override;
  void SetOutFile(std::string_view fileName, DWORD maxFileSize = 0) override;

private:
  DWORD      m_enabled{0xFFFF};
  std::mutex m_mutex;
  fs::path   m_outFile;
  DWORD      m_maxFileSize{};

  static DWORD Mask(Type type)
  {
    return DWORD(1) << static_cast<int>(type);
  }
  bool EnabledImpl(Type type) const
  {
    return (m_enabled & Mask(type)) ? true : false;
  }

  static const char *GetTypeName(Type type);
  void               WriteToFile(std::string_view str);
  void               ShrinkFile(const char *pDelim = "\r\n");
};
