#include "pch.h"
#include "DicomBase.h"

void DicomBase::SetLogging(LogLevel logLevel)
{
  RZDCX::LogLevel rzll;
  switch (logLevel) {
  case LogLevel::None:
    rzll = RZDCX::LogLevel::None;
    break;
  case LogLevel::Info:
    rzll = RZDCX::LogLevel::Info;
    break;
  case LogLevel::Warn:
    rzll = RZDCX::LogLevel::Warning;
    break;
  default:
    rzll = RZDCX::LogLevel::Error;
    break;
  }

  RZDCX::Instance()->SetLogging(rzll, LogFile());
}

fs::path DicomBase::LogFile()
{
  auto file = DicomLog::Instance()->LogFile();
  file.replace_extension(".rzdcx.log");
  return file;
}
