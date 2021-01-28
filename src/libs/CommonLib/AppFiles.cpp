#include "pch.h"
#include "AppFiles.h"
#include "TraceyRegistry.h"

namespace {
  struct RegistrySettings : public TraceyRegistryImpl<RegistrySettings>
  {
    RegistrySettings()
    {
      Load();
    }

    CString m_dataFolder{LR"(C:\Tracey)"};
    CString m_logFolder{LR"(C:\Tracey\Log)"};
    CString m_dicomSqlFile{LR"(C:\Tracey\Data\dicom.cfg.db3)"};

    BEGIN_REGPROP_MAP(RegistrySettings)
      REG_PROPERTY(L"DataFolder", m_dataFolder)
      REG_PROPERTY(L"DicomSqlFile", m_dicomSqlFile)
      REG_PROPERTY(L"LogFolder", m_logFolder)
    END_REGPROP_MAP()
  };

} // namespace

namespace app {

  fs::path GetAppPathImpl(Type type)
  {
    switch (type) {
    case Type::exe: {
      char sz[_MAX_PATH];
      GetModuleFileNameA(nullptr, sz, _MAX_PATH);
      return fs::path(sz);
    }

    case Type::exeFolder:
      return GetAppPath(Type::exe).parent_path();

    case Type::json: {
      auto p = GetAppPath(Type::exe);
      p.replace_extension(".json");
      return p;
    }

    case Type::mainDataFolder: {
      auto p = fs::path(RegistrySettings().m_dataFolder.GetString());
      fs::create_directories(p);
      return p;
    }

    case Type::dicomSqlite: {
      auto p = fs::path(RegistrySettings().m_dicomSqlFile.GetString());
      fs::create_directories(p.parent_path());
      return p;
    }

    case Type::logFolder: {
      auto p = fs::path(RegistrySettings().m_logFolder.GetString());
      fs::create_directories(p);
      return p;
    }

    case Type::dicomLogFile:
      return GetAppPath(Type::logFolder) / "dicom.log";

    default:
      return {};
    }
  }


  const fs::path &GetAppPath(Type type)
  {
    static std::array<fs::path, 8> paths;

    auto n = static_cast<unsigned>(type);
    ATLENSURE(n < paths.size());

    if (paths[n].empty()) {
      paths[n] = GetAppPathImpl(type);
    }

    return paths[n];
  }
} // namespace app
