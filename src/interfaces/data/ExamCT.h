#pragma once

#include "Exam.h"
#include "Calibration.h"

#pragma pack(push)
#pragma pack(1)

namespace exam::ct::ver1 {

  struct BLOB
  {
    Int StructSize;
    Int NumImages; //      8 bytes

    uchar  LimbusOK;
    real_t LimbusX0;
    real_t LimbusY0;
    real_t LimbusR[360];
    real_t LimbusRMin;
    real_t LimbusRMax;
    real_t LimbusRMean; //  2,929 bytes

    uchar  PupilOK;
    real_t PupilX0;
    real_t PupilY0;
    real_t PupilR[360];
    real_t PupilRMin;
    real_t PupilRMax;
    real_t PupilRMean; //  5,850 bytes

    uchar  VertexOK;
    real_t VertexX;
    real_t VertexY;
    uchar  RingsOK;
    real_t RingsR[26][360]; // 80,748 bytes

    Int SaveYear;
    Int SaveMonth;
    Int SaveDay;
    Int SaveHour;
    Int SaveMinute;
    Int SaveSecond; // 80,772 bytes

    GUID FellowExamID; // 80,788 bytes

    // for gray image disk and rulers
    // Radial ruler
    uchar  RadialRulerOk; //   1 bytes
    real_t RadialRulerX0; //   8 bytes
    real_t RadialRulerY0; //   8 bytes
    real_t RadialRulerR;  //   8 + 8 + 8 +1 + 80,788  = 80,813 bytes

    // Inlay, used in the Angle K/A
    uchar  InlayOk;   //   1 byte
    real_t InlayX0;   //   8 bytes
    real_t InlayY0;   //   8 bytes
    real_t InlayR0;   //   8 + 8 + 8 + 1 + 80,813 = 80,838 bytes
    real_t InlayInR0; //   530 8 bytes + 80,838 = 80,846 bytes  ***cjf***

    // Linear Ruler, used in the Angle K/A
    uchar  LinearRulerOk; //   1 byte
    real_t LinearRulerX0; //   8 bytes
    real_t LinearRulerY0; //   8 bytes
    real_t LinearRulerX1; //   8 bytes
    real_t LinearRulerY1; //   8 + 8 + 8 + 8 + 1 + 80,846 = 80,879 bytes

    // for color image info
    uchar  ColorLimbusOK;     // 1 bytes
    real_t ColorLimbusX0;     // 8 bytes
    real_t ColorLimbusY0;     // 8 bytes
    real_t ColorLimbusR[360]; // 8 bytes * 360 = 2880 bytes
    real_t ColorLimbusRMin;   // 8 bytes
    real_t ColorLimbusRMax;   // 8 bytes
    real_t ColorLimbusRMean;  // 80,879 bytes + 1 + 8 + 8 + 2880 + 8 + 8 + 8 = 83,800 bytes

    uchar  ColorPupilOK;     // 1 bytes
    real_t ColorPupilX0;     // 8 bytes
    real_t ColorPupilY0;     // 8 bytes
    real_t ColorPupilR[360]; // 8 bytes * 360 = 2880 bytes
    real_t ColorPupilRMin;   // 8 bytes
    real_t ColorPupilRMax;   // 8 bytes
    real_t ColorPupilRMean;  // 83,800 bytes + 2921 bytes = 86,721 bytes

    uchar  ColorVertexOK; // 1 bytes
    real_t ColorVertexX;  // 8 bytes
    real_t ColorVertexY;  // 86,721 bytes + 1 + 8 + 8 = 86,738 bytes

    // Radial Ruler, used in the Angle K/A
    uchar  ColorRadialRulerOk; //   1 bytes
    real_t ColorRadialRulerX0; //   8 bytes
    real_t ColorRadialRulerY0; //   8 bytes
    real_t ColorRadialRulerR;  //   8 + 8 + 8 +1 + 86,738  = 86,763 bytes

    // Inlay, used in the Angle K/A
    uchar  ColorInlayOk;   //   1 byte
    real_t ColorInlayX0;   //   8 bytes
    real_t ColorInlayY0;   //   8 bytes
    real_t ColorInlayR0;   //   8 + 8 + 8 + 1 + 86,763 = 86,788 bytes
    real_t ColorInlayInR0; //   530 8 bytes + 86,788 = 86,796 bytes  ***cjf***

    // Linear Ruler, used in the Angle K/A
    uchar  ColorLinearRulerOk; //   1 byte
    real_t ColorLinearRulerX0; //   8 bytes
    real_t ColorLinearRulerY0; //   8 bytes
    real_t ColorLinearRulerX1; //   8 bytes
    real_t ColorLinearRulerY1; //   8 + 8 + 8 + 8 + 1 + 86,796 = 86,829 bytes
  };

  struct DATA
  {
    HEADER         header;
    BLOB           data;
    CALIBRATION_V1 calib;
    cv::Mat        img;
    cv::Mat        colorImg;
  };

  using RING = std::vector<double>;

  struct RINGS
  {
    cv::Point2d       center;
    std::vector<RING> rings;
  };
} // namespace exam::ct::ver1

#pragma pack(pop)

using CT_EXAM_DATA = exam::ct::ver1::DATA;
using CT_RING      = exam::ct::ver1::RING;
using CT_RINGS     = exam::ct::ver1::RINGS;
