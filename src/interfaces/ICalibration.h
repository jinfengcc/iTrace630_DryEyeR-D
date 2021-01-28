#pragma once

#include <variant>
#include <filesystem>
#include <interfaces/TraceyObj.h>
#include <interfaces/hal/IFlash.h>
#include "data/Calibration.h"

TRACEYOBJ("D597DDB8-1FD6-4F03-AA0B-EF62B65F8C35") ICalibration : public IUnknown
{
  using DBID    = std::pair<IUnknown *, GUID>;
  using Storage = std::variant<hal::IFlash *, DBID, std::filesystem::path>;

  using DataPtr = std::unique_ptr<CALIBRATION_V1>;

  virtual void    Initialize(CALIBRATION_V1 & cal) const                          = 0;
  virtual bool    Read(const Storage &storage, CALIBRATION_V1 &cal) const         = 0;
  virtual DataPtr Read(const Storage &storage) const                              = 0;
  virtual bool    Write(const Storage &storage, const CALIBRATION_V1 &cal)        = 0;
  virtual bool    Verify(const Storage &storage, const CALIBRATION_V1 &cal) const = 0;
};
