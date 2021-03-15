#include "stdafx.h"
#include "AcquisitionParameters.h"

namespace {
  const int laserAlignmentLimit[] = {5, 10};
}

AcquisitionParameters::AcquisitionParameters(CameraType cameraType)
  : m_cameraType(cameraType)
  , m_laserAlignmentLimit(laserAlignmentLimit[static_cast<int>(cameraType)])
{
}
