#include "pch.h"
#include "IniParser.h"
#include "Cast.h"

namespace {
  const char iniPath[]    = R"(C:\Tracey\Tracey700sys.ini)";
  const char iniSection[] = R"(Tracey 7.0.0)";

  static std::array Keys = {
    "Main Folder",
    //"Export Excel Folder",
    "Database Type",
    "Database Folder",
    //"Database Filename",
    //"Database BackupFolder",
    //"S1",
    //"S2",
    //"S3",
    //"S4",
    //"Software Activation Key 7.0.0",
    //"OMED",
    //"Export Folder",
    //"WaveTouch Folder",
    //"Settings Folder",
    //"Import Format",
    //"Screenshot Folder",
    //"Dicom MWL Local AE",
    //"Dicom MWL Target AE",
    //"Dicom MWL IP Address",
    //"Dicom MWL IP Port",
    //"RZDCX ACT",
    //"Dicom Modality",
    "Inst Data",
  };

  auto GetTypeStr( ini::Type type)
  {
    return Keys[static_cast<int>(type)];
  }

} // namespace

namespace ini {
  std::optional<int> GetInt(Type type)
  {
    if (auto s = GetStr(type); !s.empty())
      return Cast<int>(s);
    else
      return std::nullopt;
  }

  int GetIntOr(Type type, int defaultValue)
  {
    return GetInt(type).value_or(defaultValue);
  }

  void SetInt(Type type, int value)
  {
    SetStr(type, std::to_string(value));
  }

  std::string GetStr(Type type)
  {
    char sz[_MAX_PATH];
    GetPrivateProfileStringA(iniSection, GetTypeStr(type), "", sz, sizeof(sz), iniPath);

    return sz;
  }

  void SetStr(Type type, std::string_view value)
  {
    WritePrivateProfileStringA(iniSection, GetTypeStr(type), value.data(), iniPath);
  }
} // namespace ini
