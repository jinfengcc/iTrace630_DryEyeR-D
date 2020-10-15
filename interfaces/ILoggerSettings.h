#pragma once

#include "ILogger.h"

TRACEYOBJ("56DEC98A-14E0-4899-B0FA-95B45EE3C3B8") ILoggerSettings : public IUnknown
{
  virtual void Enable(ILogger::Type type, bool enable = true)           = 0;
  virtual void SetOutFile(std::string_view fileName, DWORD maxFileSize) = 0;
};
