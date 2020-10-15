#pragma once

#include <atlbase.h>
#include <cstdint>
#include <memory>

#pragma pack(push)
#pragma pack(1)

struct CALIBRATION_V1
{
  using real_t = double;
  using uchar  = unsigned char;
  using Bool   = int32_t;
  using Int    = int32_t;

  struct VIDEO_SETTINGS
  {
    Bool Color;
    Int  InfraredLEDsPowerLevel;
    Int  WhiteLEDsPowerLevel;
    Int  Brightness;
    Int  Contrast;
    Int  Hue;
    Int  Saturation;
  };

  struct AOD_SETTINGS
  {
    Int Freq0;
    Int HzUm;
    Int RMS;
  };

  struct ZERNIKE_SETTINGS
  {
    real_t C3;
    real_t C4;
    real_t C5;
  };

  GUID             CalibrationID;
  char             HardwareSerial[28];
  SYSTEMTIME       Time;
  real_t           LaserWavelength;
  Int              NumPoints;
  Int              OptometerEnabled;
  AOD_SETTINGS     AODX;
  AOD_SETTINGS     AODY;
  Int              LADExposureTime; // µs
  Bool             LADSwapXY;
  Bool             LADXNoFlip;
  real_t           LADXFocalDistance;
  Bool             LADYNoFlip;
  real_t           LADYFocalDistance;
  Int              OptometerTable[21]; // D -> µ
  Int              OptometerGap;
  Int              LaserSpotRadiusX;
  Int              LaserSpotRadiusY;
  Int              IntensityThreshold1;
  Int              LaserSpotRingRadius;
  Int              IntensityThreshold2;
  Bool             AODXNoFlip;
  Bool             AODYNoFlip;
  Int              LaserDuration;
  Int              LaserIntensityThreshold;
  Int              ZAlignmentMethod;
  Int              SensorThreshold;
  Int              NumConsecFrames;
  Int              AllowedDecentError;
  Int              AllowedMovement;
  uchar            __Reserved1[340];
  Bool             VideoFlipX;
  Bool             VideoFlipY;
  Int              VideoWidth;
  Int              VideoHeight;
  real_t           VideoWidthMicrons;
  real_t           VideoHeightMicrons;
  uchar            StepLeft;  // for camers frame move left (step 1pixel)
  uchar            StepRight; // for camers frame move right (step 1pixel)
  uchar            StepUp;    // for camers frame move up (step 1pixel)
  uchar            StepDown;  // for camers frame move down (step 1pixel)
  uchar            __Reserved2[24];
  VIDEO_SETTINGS   WFVideoSettings;
  VIDEO_SETTINGS   WFVideo2Settings;
  VIDEO_SETTINGS   CTVideoSettings;
  ZERNIKE_SETTINGS Zernike;
  Int              ColorImageDelay;
  Int              AccommodationTargetPowerLevel;
  Int              FarTargetPowerLevel;
  Int              NearTargetPowerLevel;
  uchar            __Reserved3[4];
  Int              NumRings;
  real_t           BallsAx[4];
  real_t           BallsRi[4][26][360];
};

#pragma pack(pop)

struct CALIBRATION_CACHED_DATA;

struct CALIBRATION_EX
{
  using CalData    = std::unique_ptr<CALIBRATION_V1>;
  using CachedData = std::unique_ptr<CALIBRATION_CACHED_DATA>;

  CalData    cal;
  CachedData cachedData;
};
