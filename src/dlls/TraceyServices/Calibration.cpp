#include "pch.h"
#include "Calibration.h"
#include "CalibrationJson.h"

void Calibration::Initialize(CALIBRATION_V1 &cal) const
{
  ResetCalibration(cal);
}

bool Calibration::Read(const Storage &storage, CALIBRATION_V1 &cal) const
{
  memset(&cal, 0, sizeof(cal));

  bool ok = false;
  if (std::holds_alternative<IFlash *>(storage)) {
    ok = ReadV1(std::get<IFlash *>(storage), cal);
  }
  else if (std::holds_alternative<DBID>(storage)) {
    auto [unk, id] = std::get<DBID>(storage);
    if (auto db = wil::com_query<IDatabase>(unk))
      ok = ReadV1(db.get(), id, cal);
  }
  else if (std::holds_alternative<fs::path>(storage)) {
    auto file = std::get<fs::path>(storage);
    ok        = ReadV1(file, cal);
  }

  return ok && Validate(cal);
}

auto Calibration::Read(const Storage &storage) const -> DataPtr
{
  auto cd = std::make_unique<CALIBRATION_V1>();
  if (Read(storage, *cd))
    return cd;
  else
    return {};
}

bool Calibration::Write(const Storage &storage, const CALIBRATION_V1 &cal_)
{
  bool ok{};

  // Copy and validate before write
  auto cal = std::make_unique<CALIBRATION_V1>(cal_);
  Validate(*cal);

  if (std::holds_alternative<IFlash *>(storage)) {
    ok = WriteV1(std::get<IFlash *>(storage), *cal);
  }
  else if (std::holds_alternative<DBID>(storage)) {
    auto [unk, id] = std::get<DBID>(storage);
    if (auto db = wil::com_query<IDatabase>(unk))
      ok = WriteV1(db.get(), id, *cal);
  }

  // Always verify writes
  return ok && Verify(storage, *cal);
}

bool Calibration::Verify(const Storage &storage, const CALIBRATION_V1 &cal) const
{
  auto cd = std::make_unique<CALIBRATION_V1>();
  Read(storage, *cd);

  return memcmp(cd.get(), &cal, sizeof(cal)) == 0;
}

/// From flash /////////////////////////////////////////////////////////////////////////

bool Calibration::ReadV1(const IFlash *storage, CALIBRATION_V1 &cal) const
{
  std::uint32_t size;
  storage->Read(0, &size, sizeof(size));

  if (size < 1000 || size > storage->Size() - sizeof(size))
    TRACEY_THROW("Invalid Calibration Flash Data Size: {}", size);

  std::vector<BYTE> buf(size);
  storage->Read(sizeof(size), &buf[0], buf.size());

  if (auto size = DecompressedSize(buf); size != sizeof(CALIBRATION_V1))
    TRACEY_THROW("Invalid Calibration Decompressed Data Size {} (Expected: {})", size, sizeof(CALIBRATION_V1));

  auto decom = Decompress(buf);
  memcpy(&cal, &decom[0], sizeof(cal));

  return true;
}

bool Calibration::WriteV1(IFlash *storage, const CALIBRATION_V1 &cal)
{
  auto cd = Compress(&cal, sizeof(cal));

  std::uint32_t size = cd.size();
  if (size < 1000 || size > storage->Size() - sizeof(size))
    TRACEY_THROW("Compressed calibration size ({}) exceeds flash Data Size ({})", size, storage->Size());

  storage->Write(0, &size, sizeof(size));
  storage->Write(sizeof(size), &cd[0], cd.size());

  return true;
}

/// From database //////////////////////////////////////////////////////////////////////

bool Calibration::ReadV1(const IDatabase *db, const GUID &id, CALIBRATION_V1 &cal) const
{
  auto blob = db->GetCalibrationBlob(id);
  if (blob.empty())
    TRACEY_THROW("Unable to find requested calibration record in the database");

  if (blob.size() != sizeof(CALIBRATION_V1))
    TRACEY_THROW("Invalid Db Calibration Data Size {} (Expected: {})", blob.size(), sizeof(CALIBRATION_V1));

  memcpy(&cal, &blob[0], sizeof(cal));
  return true;
}

bool Calibration::WriteV1(IDatabase *db, const GUID &id, const CALIBRATION_V1 &cal)
{
  return db->SaveCalibrationBlob(id, &cal, sizeof(cal));
}

/// From file //////////////////////////////////////////////////////////////////////

bool Calibration::ReadV1(const fs::path &file, CALIBRATION_V1 &cal) const
{
  auto blob = Blob::Load(file);
  if (blob.empty())
    TRACEY_THROW("Unable to load calibration from file '{}'", file.string());

  if (blob.size() != sizeof(CALIBRATION_V1))
    TRACEY_THROW("Invalid Db Calibration Data Size {} (Expected: {})", blob.size(), sizeof(CALIBRATION_V1));

  memcpy(&cal, &blob[0], sizeof(cal));
  return true;
}


/// Validation /////////////////////////////////////////////////////////////

bool Calibration::Validate(CALIBRATION_V1 &cal)
{
  if (cal.LaserIntensityThreshold <= 0 || cal.LaserIntensityThreshold > 255)
    cal.LaserIntensityThreshold = 160;

  if (cal.SensorThreshold < 0 || cal.SensorThreshold > 65535)
    cal.SensorThreshold = 30000;

  if (cal.LaserDuration <= 0 || cal.LaserDuration > 600)
    cal.LaserDuration = 30;

  return true;
}

void Calibration::ResetCalibration(CALIBRATION_V1 &cal)
{
  memset(&cal, 0, sizeof(cal));

  cal.CalibrationID = GUID_NULL;

  cal.LaserWavelength = 785.0;

  cal.NumPoints = 256;

  cal.AODX.Freq0 = 60000000;
  cal.AODX.HzUm  = 2000;
  cal.AODX.RMS   = 10;
  cal.AODY.Freq0 = 60000000;
  cal.AODY.HzUm  = 2000;
  cal.AODY.RMS   = 100;

  cal.LADExposureTime   = 1024;
  cal.LADSwapXY         = FALSE;
  cal.LADXNoFlip        = FALSE;
  cal.LADXFocalDistance = 75000.0;
  cal.LADYNoFlip        = FALSE;
  cal.LADYFocalDistance = 75000.0;

  cal.AODXNoFlip = FALSE;
  cal.AODYNoFlip = FALSE;

  cal.VideoFlipX         = FALSE;
  cal.VideoFlipY         = FALSE;
  cal.VideoWidth         = 624;
  cal.VideoHeight        = 468;
  cal.VideoWidthMicrons  = 16000.0;
  cal.VideoHeightMicrons = 12000.0;

  // cal.VideoSettings.Color                     = FALSE;
  // cal.VideoSettings.InfraredLEDsPowerLevel    =  50;
  // cal.VideoSettings.WhiteLEDsPowerLevel       =  50;
  // cal.VideoSettings.Brightness                = 150;
  // cal.VideoSettings.Contrast                  = 255;
  // cal.VideoSettings.Hue                       =   0;
  // cal.VideoSettings.Saturation                = 255;

  cal.WFVideoSettings.Color                  = FALSE;
  cal.WFVideoSettings.InfraredLEDsPowerLevel = 50;
  cal.WFVideoSettings.WhiteLEDsPowerLevel    = 0;
  cal.WFVideoSettings.Brightness             = 150;
  cal.WFVideoSettings.Contrast               = 255;
  cal.WFVideoSettings.Hue                    = 0;
  cal.WFVideoSettings.Saturation             = 255;

  cal.WFVideo2Settings.Color                  = TRUE;
  cal.WFVideo2Settings.InfraredLEDsPowerLevel = 0;
  cal.WFVideo2Settings.WhiteLEDsPowerLevel    = 50;
  cal.WFVideo2Settings.Brightness             = 150;
  cal.WFVideo2Settings.Contrast               = 255;
  cal.WFVideo2Settings.Hue                    = 0;
  cal.WFVideo2Settings.Saturation             = 255;

  cal.CTVideoSettings.Color                  = FALSE;
  cal.CTVideoSettings.InfraredLEDsPowerLevel = 0;
  cal.CTVideoSettings.WhiteLEDsPowerLevel    = 0;
  cal.CTVideoSettings.Brightness             = 150;
  cal.CTVideoSettings.Contrast               = 255;
  cal.CTVideoSettings.Hue                    = 0;
  cal.CTVideoSettings.Saturation             = 255;

  cal.OptometerEnabled = 1;

  for (int i = 0; i <= 20; i++)
    cal.OptometerTable[i] = 0;

  cal.OptometerGap = 0;

  cal.Zernike.C3 = 0.0;
  cal.Zernike.C4 = 0.0;
  cal.Zernike.C5 = 0.0;

  cal.ColorImageDelay = 280;

  cal.AccommodationTargetPowerLevel = 50;
  cal.FarTargetPowerLevel           = 50;
  cal.NearTargetPowerLevel          = 50;

  cal.NumRings = 24;

  cal.LaserSpotRadiusX    = 12;
  cal.LaserSpotRadiusY    = 7;
  cal.IntensityThreshold1 = 180;
  cal.LaserSpotRingRadius = 18;
  cal.IntensityThreshold2 = 40;

  cal.LaserIntensityThreshold = 160;
  cal.LaserDuration           = 30;

  cal.BallsAx[0] = 6129.0;
  cal.BallsAx[1] = 7099.0;
  cal.BallsAx[2] = 7939.0;
  cal.BallsAx[3] = 9000.0;
}
