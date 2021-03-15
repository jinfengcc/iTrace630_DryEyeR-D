#pragma once

#include "libs/CommonLib/Singleton.h"

class AcquisitionParameters : public Singleton<AcquisitionParameters>
{
public:
  enum class CameraType
  {
    analog,
    ids
  };

  const CameraType m_cameraType;
  const int        m_laserAlignmentLimit;

  static bool GermanCamera()
  {
    return Instance()->m_cameraType == CameraType::ids;
  }
  static bool AnalogCamera()
  {
    return Instance()->m_cameraType == CameraType::analog;
  }

private:
  friend class Singleton<AcquisitionParameters>;

  AcquisitionParameters(CameraType cameraType);
};
