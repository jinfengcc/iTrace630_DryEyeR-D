#pragma once

#include <filesystem>

namespace fs = std::filesystem;

namespace app {
  enum class Type {
    exe, exeFolder, json, dicomSqlite, logFolder, dicomLogFile, mainDataFolder, tempFolder
  };

  const fs::path &GetAppPath(Type type);
} // namespace app
