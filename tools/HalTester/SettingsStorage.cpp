#include "pch.h"
#include "SettingsStorage.h"
#include "libs/CommonLib/AppFiles.h"

SettingsStorage::SettingsStorage()
{
  fs::path path;

  if (__argc == 2) {
    path = __targv[1];
  }
  else {
    path = app::GetAppPath(app::Type::exeFolder) / "iTrace.json";
    if (!fs::exists(path)) {
      path = app::GetAppPath(app::Type::exe);
      path.replace_extension(".json");
    }
  }

  m_sysFile = path.string();
}
