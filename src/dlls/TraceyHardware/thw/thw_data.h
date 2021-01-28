#pragma once

#pragma pack(push)
#pragma pack(8)

namespace thw {
  struct VIDEO_FRAME
  {
    enum { ROWS = 487, COLS = 720 };
    int   StructSize;
    ulong Timestamp;
    uchar Data[2 * ROWS * COLS];
  };

  struct AOD_CONFIG
  {
    int XHz;
    int YHz;
    int XPower;
    int YPower;
  };

  struct LIGHTSOURCES_POWERLEVELS
  {
    std::int32_t InfraredLEDs;
    std::int32_t WhiteLEDs;
    std::int32_t AccommodationTarget;
    std::int32_t FarFixationTarget;
    std::int32_t NearFixationTarget;
    std::int32_t Reserved3;
    std::int32_t Reserved2;
    std::int32_t Reserved1;
  };

} // namespace thw

#pragma pack(pop)
