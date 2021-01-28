#pragma once

#include "Exam.h"
#include "Calibration.h"

#pragma pack(push)
#pragma pack(1)

namespace exam::wf {
  enum class Alignment : uchar { PUPIL_CENTER, IR_4DOTS };
  enum class Detector : Int { QUADRANT, LINEAR };

  namespace ver1 {

    struct BLOB
    {
      Int StructSize;

      real_t   ScanDiameter;
      real_t   AccommTargetPosition;
      real_t   PrecalcSphere;
      real_t   PrecalcCylinder;
      Int      PrecalcAxis;
      real_t   WavetouchLensPower;
      Int      WavetouchLensBaseCurve;
      Detector DetectorType;
      Int      NumPoints;
      Int      PointSize;
      Int      NumImages; //     68 bytes

      uchar  WavetouchLensDotsOK;
      real_t WavetouchLensDotX[3];
      real_t WavetouchLensDotY[3]; //    117 bytes

      uchar  VertexOK;
      real_t VertexX;
      real_t VertexY; //    134 bytes

      uchar  PupilOK;
      real_t PupilX0;
      real_t PupilY0;
      real_t PupilR[360];
      real_t PupilRMin;
      real_t PupilRMax;
      real_t PupilRMean; //  3,055 bytes

      uchar AODsOK; //  3,056 bytes

      uchar  LimbusOK;
      real_t LimbusX0;
      real_t LimbusY0;
      real_t LimbusR[360];
      real_t LimbusRMin;
      real_t LimbusRMax;
      real_t LimbusRMean; //  5,977 bytes

      Int SaveYear;
      Int SaveMonth;
      Int SaveDay;
      Int SaveHour;
      Int SaveMinute;
      Int SaveSecond; //  6,001 bytes

      char   WavetouchOther[21];
      char   WavetouchNote[101];
      real_t WavetouchVertex;
      real_t WavetouchSph;
      real_t WavetouchCyl;
      Int    WavetouchAxis; //  6,151 bytes

      Alignment AlignmentMethod; //  6,152 bytes

      GUID FellowExamID; //  6,168 bytes

      real_t ScanX0;
      real_t ScanY0; //  6,184 bytes

      // Radial Ruler, used in the Angle K/A
      uchar  RadialRulerOk; //   1 byte
      real_t RadialRulerX0; //   8 bytes
      real_t RadialRulerY0; //   8 bytes
      real_t RadialRulerR;  //   8 + 8 + 8 +1 + 6184 = 6209 bytes

      // Inlay, used in the Angle K/A
      uchar  InlayOk;   //   1 byte
      real_t InlayX0;   //   8 bytes
      real_t InlayY0;   //   8 bytes
      real_t InlayR0;   //   8 bytes + 8 + 8 + 1 + 6209 = 6234 bytes
      real_t InlayInR0; //   8 bytes + 6234 = 6242 bytes  ***cjf***

      // Linear Ruler, used in the Angle K/A
      uchar  LinearRulerOk; //   1 byte
      real_t LinearRulerX0; //   8 bytes
      real_t LinearRulerY0; //   8 bytes
      real_t LinearRulerX1; //   8 bytes
      real_t LinearRulerY1; //   8 + 8 + 8 + 8 + 1 + 6242 = 6275 bytes
    };

  } // namespace ver1
} // namespace exam::wf

#pragma pack(pop)

namespace exam::wf::ver1 {
  struct LAD
  {
    using Array = std::array<std::uint16_t, 512>;

    Array    Signal;
    real_t   CentroidPos;
    real_t   Baseline;
    uint16_t Max;
    unsigned MaxPos;
    real_t   Area;
    Bool     Bad;
  };

  struct TDB_POINT
  {
    Int         ShotNumber;
    Bool        Bad;
    real_t      Radius;
    real_t      Angle;
    cv::Point2d point;
    LAD         ladx;
    LAD         lady;
  };

  struct DATA
  {
    HEADER                 header;
    BLOB                   data;
    CALIBRATION_V1         calib;
    std::vector<TDB_POINT> points;
    cv::Mat                img;
    cv::Mat                colorImg;
  };
} // namespace exam::wf::ver1

using WF_EXAM_LAD  = exam::wf::ver1::LAD;
using WF_EXAM_DATA = exam::wf::ver1::DATA;


