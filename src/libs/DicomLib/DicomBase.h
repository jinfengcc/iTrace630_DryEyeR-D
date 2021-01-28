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
