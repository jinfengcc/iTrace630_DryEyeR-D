#pragma once

namespace nlohmann {

  template<class T>
  void get(const json& j, const char* name, T& value)
  {
    if (auto i = j.find(name); i != j.end())
      value = i->get<T>();
    else
      value = T{};
  }

  template <>
  struct adl_serializer<SYSTEMTIME>
  {
    static void to_json(json &j, const SYSTEMTIME &x)
    {
      // clang-format off
      j = json{
        {"year"  , x.wYear     },
        {"month" , x.wMonth    },
        {"day"   , x.wDay      },
        {"hour"  , x.wHour     },
        {"minute", x.wMinute   },
        {"second", x.wSecond   }
      };
      // clang-format on
    }

    static void from_json(const json &j, SYSTEMTIME &x)
    {
      get(j, "year"  , x.wYear  );
      get(j, "month" , x.wMonth );
      get(j, "day"   , x.wDay   );
      get(j, "hour"  , x.wHour  );
      get(j, "minute", x.wMinute);
      get(j, "second", x.wSecond);
    }
  };

  template <>
  struct adl_serializer<CALIBRATION_V1::VIDEO_SETTINGS>
  {
    static void to_json(json &j, const CALIBRATION_V1::VIDEO_SETTINGS &x)
    {
      // clang-format off
      j = json{
        {"color"            , x.Color                 },
        {"infraredLEDsPower", x.InfraredLEDsPowerLevel},
        {"whiteLEDsPower"   , x.WhiteLEDsPowerLevel   },
        {"brightness"       , x.Brightness            },
        {"contrast"         , x.Contrast              },
        {"hue"              , x.Hue                   },
        {"saturation"       , x.Saturation            },
      };
      // clang-format on
    }

    static void from_json(const json &j, CALIBRATION_V1::VIDEO_SETTINGS &x)
    {
      // clang-format off
      get(j, "color"            , x.Color                 );
      get(j, "infraredLEDsPower", x.InfraredLEDsPowerLevel);
      get(j, "whiteLEDsPower"   , x.WhiteLEDsPowerLevel   );
      get(j, "brightness"       , x.Brightness            );
      get(j, "contrast"         , x.Contrast              );
      get(j, "hue"              , x.Hue                   );
      get(j, "saturation"       , x.Saturation            );
      // clang-format on
    }
  };

  template <>
  struct adl_serializer<CALIBRATION_V1::AOD_SETTINGS>
  {
    static void to_json(json &j, const CALIBRATION_V1::AOD_SETTINGS &x)
    {
      // clang-format off
      j = json{
        {"Freq0", x.Freq0},
        {"HzUm" , x.HzUm},
        {"RMS"  , x.RMS},
      };
      // clang-format on
    }

    static void from_json(const json &j, CALIBRATION_V1::AOD_SETTINGS &x)
    {
      get(j, "Freq0", x.Freq0);
      get(j, "HzUm" , x.HzUm );
      get(j, "RMS"  , x.RMS  );
    }
  };

  template <>
  struct adl_serializer<CALIBRATION_V1::ZERNIKE_SETTINGS>
  {
    static void to_json(json &j, const CALIBRATION_V1::ZERNIKE_SETTINGS &x)
    {
      // clang-format off
      j = json{
        {"c3" , x.C3},
        {"c4" , x.C4},
        {"c5" , x.C5},
      };
      // clang-format on
    }

    static void from_json(const json &j, CALIBRATION_V1::ZERNIKE_SETTINGS &x)
    {
      get(j, "c3", x.C3);
      get(j, "c4", x.C4);
      get(j, "c5", x.C5);
    }
  };

  template <>
  struct adl_serializer<CALIBRATION_V1>
  {
    static void to_json(json &j, const CALIBRATION_V1 &x)
    {
      // clang-format off
      j = json{
        {"WFVideoSettings" , x.WFVideoSettings},
      };
      // clang-format on
    }

    static void from_json(const json &j, CALIBRATION_V1 &x)
    {
      get(j, "WFVideoSettings", x.WFVideoSettings);
    }
  };

} // namespace nlohmann


