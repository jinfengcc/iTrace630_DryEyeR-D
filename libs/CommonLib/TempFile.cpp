#include "pch.h"
#include "TempFile.h"

namespace {
  struct TempCleanup
  {
    TempCleanup()
    {
      std::lock_guard lock(_mutex);
      fs::remove_all(util::GetTempFolder());
    }
    std::mutex _mutex;
  };
} // namespace

namespace util {

  std::filesystem::path GetTempFolder()
  {
    char szTemp[_MAX_PATH];
    auto s = GetTempPathA(_MAX_PATH, szTemp);
    TRACEY_THROW_IF(s < 1 || s >= _MAX_PATH, "Bug: GetTempPath Failed");

    return std::filesystem::path(szTemp) / "TRACEY";
  }

  std::filesystem::path GetTempFile(const char *pszExt)
  {
    static TempCleanup _cleanup;

    auto guid = Guid::NewGuid();
    auto path = GetTempFolder() / guid.string(false);

    path.replace_extension(pszExt);
    std::filesystem::create_directories(path.parent_path());

    return path;
  }
} // namespace util


