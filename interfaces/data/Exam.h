#pragma once

#include <cstdint>
#include <Unknwn.h>
#include <opencv2/opencv.hpp>

#pragma pack(push)
#pragma pack(1)
namespace exam {
  using real_t = double;
  using uchar  = unsigned char;
  using Bool   = int32_t;
  using Int    = int32_t;
  using Uint   = uint32_t;

  enum class Type : Int {
    None               = 0,
    WF                 = 1,
    CT                 = 2,
    CT_DRYEYE_First    = 3,
    CT_DRYEYE_OTHERS   = 4,
    CT_DRYEYE_MidBlink = 5,
  };

  struct HEADER
  {
    enum class MODE : Int { None, Auto, Manual, Presbia64, Presbia256 };

    GUID ExamID;
    GUID PatientID;
    GUID CalibrationID;
    GUID ClinicID;
    GUID PhysicianID;
    GUID OperatorID;
    Type Type;
    Int  Year;
    Int  Month;
    Int  Day;
    Int  Hour;
    Int  Minute;
    Int  Second;
    Int  Eye;
    Int  Preop;
    Int  SeriesNumber;
    MODE Mode;
    Uint SoftwareVersion;
    Int  Reserved3;
    Int  Reserved4;
    char Note[101];
  };
} // namespace exam
#pragma pack(pop)
