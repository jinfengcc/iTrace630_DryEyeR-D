#pragma once

#include <interfaces/ICalibration.h>
#include <interfaces/IDatabase.h>

class Calibration : public Implements<ICalibration>
{
public:
  Calibration()
    : Implements<ICalibration>("ICalibration")
  {
  }

  // ICalibration
  void    Initialize(CALIBRATION_V1 &cal) const override;
  bool    Read(const Storage &storage, CALIBRATION_V1 &cal) const override;
  DataPtr Read(const Storage &storage) const override;
  bool    Write(const Storage &storage, const CALIBRATION_V1 &cal) override;
  bool    Verify(const Storage &storage, const CALIBRATION_V1 &cal) const override;

private:
  using IFlash = hal::IFlash;
  bool ReadV1(const IFlash *storage, CALIBRATION_V1 &cal) const;
  bool WriteV1(IFlash *storage, const CALIBRATION_V1 &cal);

  bool ReadV1(const IDatabase *db, const GUID &id, CALIBRATION_V1 &cal) const;
  bool WriteV1(IDatabase *db, const GUID &id, const CALIBRATION_V1 &cal);

  bool ReadV1(const fs::path &file, CALIBRATION_V1 &cal) const;

  static bool Validate(CALIBRATION_V1 &cal);
  static void ResetCalibration(CALIBRATION_V1 &cal);
};
