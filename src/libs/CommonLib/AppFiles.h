#pragma once

#include <filesystem>

namespace fs = std::filesystem;

namespace app {
  enum class Type {
    exe, exeFolder, json, dicomSqlite, logFolder, dicomLogFile, mainDataFolder, tempFolder
  };

  const fs::path &GetAppPath(Type type);
} // namespace app
inline fs::path GetExeFilePath()
{
  char sz[_MAX_PATH];
  GetModuleFileNameA(nullptr, sz, _MAX_PATH);
  return fs::path(sz);
}

inline fs::path GetExeFileFolder()
{
  return GetExeFilePath().parent_path();
}
