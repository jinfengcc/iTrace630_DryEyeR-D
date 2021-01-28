#include "pch.h"
#include "TempFile.h"

namespace {
  struct TempHelper
  {
    TempHelper()
    {
      InitTempFolder();
      EmptyTempFolder();
    }

    ~TempHelper()
    {
      EmptyTempFolder();
    }

    fs::path m_tempFolder;

  private:
    void InitTempFolder()
    {
      char szTemp[_MAX_PATH];
      auto s = GetTempPathA(_MAX_PATH, szTemp);
      TRACEY_THROW_IF(s < 1 || s >= _MAX_PATH, "Bug: GetTempPath Failed");
      m_tempFolder = std::filesystem::path(szTemp) / "_tracey";
      fs::create_directories(m_tempFolder);
    }

    void EmptyTempFolder()
    {
      for (fs::recursive_directory_iterator i(m_tempFolder), e; i != e; ++i) {
        try {
          fs::remove(i->path());
        }
        catch (...) {
        }
      }
    }
  };
} // namespace

namespace util {

  std::filesystem::path GetTempFolder()
  {
    static TempHelper th;
    return th.m_tempFolder;
  }

  std::filesystem::path GetTempFile(const char *pszExt)
  {
    auto guid = Guid::NewGuid();
    auto path = GetTempFolder() / guid.string(false);

    path.replace_extension(pszExt);

    return path;
  }
} // namespace util
