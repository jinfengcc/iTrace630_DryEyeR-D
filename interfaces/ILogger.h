#pragma once

#include <string_view>
#include <libs/CommonLib/CreateObj.h>
#include <libs/LoggingLib/TkTraceWin.h>
#include "TraceyObj.h"

//////////////////////////////////////////////////////////////////////////

TRACEYOBJ("13C6EDCD-7D6F-4A67-BD95-63B588487BDF") ILogger : public IUnknown
{
  enum class Type { Diag, Info, Warn, Err, THROW_, BUG_, ASSRT_, FATAL_, All__, Default__ };

  virtual bool Enabled(Type type) const               = 0;
  virtual void Write(Type type, std::string_view msg) = 0;
};

//////////////////////////////////////////////////////////////////////////

inline void LogWrite(ILogger *logger, ILogger::Type type, std::string_view msg)
{
  if (logger && logger->Enabled(type))
    logger->Write(type, msg);
}

#ifdef FMT_FORMAT_H_
template <typename... Args>
void LogWrite(ILogger *logger, ILogger::Type type, std::string_view msg, Args... args)
{
  if (logger && logger->Enabled(type))
    logger->Write(type, fmt::format(msg, args...));
}
#endif

//////////////////////////////////////////////////////////////////////////

inline ILogger *DefaultLogger()
{
  static auto obj = CreateObj<ILogger>();
  return obj.get();
}

//////////////////////////////////////////////////////////////////////////

// clang-format off
#define LOG_Info(...)   LogWrite(DefaultLogger(), ILogger::Type::Info  , __VA_ARGS__)
#define LOG_Diag(...)   LogWrite(DefaultLogger(), ILogger::Type::Diag  , __VA_ARGS__)
#define LOG_Warn(...)   LogWrite(DefaultLogger(), ILogger::Type::Warn  , __VA_ARGS__)
#define LOG_Err(...)    LogWrite(DefaultLogger(), ILogger::Type::Err   , __VA_ARGS__)
#define LOG_Throw(...)  LogWrite(DefaultLogger(), ILogger::Type::THROW_, __VA_ARGS__)
#define LOG_Bug(...)    LogWrite(DefaultLogger(), ILogger::Type::BUG_  , __VA_ARGS__)
#define LOG_Fatal(...)  LogWrite(DefaultLogger(), ILogger::Type::FATAL_, __VA_ARGS__)
#define LOG_ASSERT(...) LogWrite(DefaultLogger(), ILogger::Type::ASSRT_, __VA_ARGS__)
// clang-format on
