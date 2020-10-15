#pragma once

#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

#include <Interfaces/ISettings.h>
#include <libs/CommonLib/Implements.h>

struct AppSettings final: ImplementsStatic<ISettings>
{
  explicit AppSettings(std::string_view str_or_filename);
  ~AppSettings();

  // Inherited via ISettings
  json        Get(std::string_view section) const override;
  void        Set(std::string_view section, const json &js) override;
  std::string ToString(bool formatted = true) const override
  {
    return m_json.dump(formatted ? 2 : -1);
  }

private:
  fs::path m_file;
  json     m_json;
  bool     m_modified{};

  void Load(std::istream &fs);
  void Save(std::ostream &fs);
  void Flush();
};
