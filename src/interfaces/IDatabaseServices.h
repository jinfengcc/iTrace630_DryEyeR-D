#pragma once

#include <filesystem>
#include "TraceyObj.h"

TRACEYOBJ("ee581803-02dd-426f-91f0-1e2833788158") IDatabaseServices : public IUnknown
{
  using path = std::filesystem::path;

  virtual path GetExamFile(const path &dbFolder, const GUID examId) const = 0;
  virtual path GetCalibrationFile(const path &dbFolder, const GUID calId) const = 0;
};
