#pragma once

#include <wil/com.h>
#include <interfaces/data/Dicom.h>
#include "rzdcx/rzdcx.h"
#include "DicomLog.h"

class DicomBase
{
public:
  using LogLevel = dicom::LogLevel;
  static void SetLogging(LogLevel logLevel);

protected:
  DicomBase(LogLevel logLevel)
  {
    SetLogging(logLevel);
  }

private:
  static fs::path LogFile();
};

[[noreturn]] inline  void ThrowDicomComError(const char *msg, const _com_error &e)
{
  CString err((const wchar_t *)e.Description());
  TRACEY_THROW("{}: {}", msg, CStringA(err).GetString());
}
