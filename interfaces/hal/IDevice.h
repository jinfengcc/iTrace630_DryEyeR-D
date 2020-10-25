#pragma once

#include <vector>
#include <interfaces/TraceyObj.h>
#include <libs/CommonLib/types.h>

namespace hal {
  // clang-format off
  enum class POWER {
    ACCOMMODATION_MOTOR  = 0,
    COMPENSATION_MOTOR   = 1,
    AODS                 = 2,
    VIDEO_CAMERA         = 3,
    VISTA_HAND_UNIT      = 4,
    TRIANGULATION_LASER  = 5,
    PLACIDO              = 6,
    LADS                 = 7,
    SCANNING_LASER       = 8,
  };
  enum class LED {
    INFRARED             = 0,
    WHITE                = 1,
    ACCOMMODATION_TARGET = 2,
    FAR_FIXATION_TARGET  = 3,
    NEAR_FIXATION_TARGET = 4,
  };
  // clang-format on

  TRACEYOBJ("074A18B7-35A5-4F2F-95B4-A5C6927C9DF2") IDevice : public IUnknown
  {
    virtual bool Connected() const              = 0;
    virtual void Shutdown()                     = 0;
    virtual bool Power(POWER p) const           = 0;
    virtual void Power(POWER p, bool power)     = 0;
    virtual int  Power(LED led) const           = 0;
    virtual void Power(LED led, int powerLevel) = 0;
    virtual EYE  CurrentEye() const             = 0;
    virtual bool ButtonIsPressed() const        = 0;
  };

} // namespace hal
