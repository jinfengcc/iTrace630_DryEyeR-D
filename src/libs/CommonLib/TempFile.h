#pragma once

#include <string>
#include <filesystem>
#include "TraceyException.h"
#include "Guid.h"

namespace util {
  std::filesystem::path GetTempFolder();
  std::filesystem::path GetTempFile(const char *pszExt = ".tmp");
} // namespace util
