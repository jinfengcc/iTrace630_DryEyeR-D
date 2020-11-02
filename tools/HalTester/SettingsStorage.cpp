#include "pch.h"
#include "SettingsStorage.h"

SettingsStorage::SettingsStorage()
{
  fs::path path;

  if (__argc == 2) {
    path = __targv[1];
  }
  else {
    char exe[_MAX_PATH];
    GetModuleFileNameA(nullptr, exe, _MAX_PATH);

    auto path = fs::path(exe);
    path.replace_extension(".json");

  }

  m_sysFile = path.string();
}
