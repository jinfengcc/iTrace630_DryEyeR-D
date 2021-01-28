#pragma once

#include <filesystem>
#include <fstream>
#include <string_view>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;

//! This class encapsulates the application settings
//! The settings are stored in a json format - here is an example
//!
//!      {
//!        "DatabaseJet":
//!        {
//!          "database"   : "C:\\tracey\\data\\tdb.mdb",
//!          "examFolder" : "C:\\tracey\\data\\data\\Exams",
//!          "calibFolder": "C:\\tracey\\data\\data\\Calib",
//!          "readOnly"   : true
//!        }
//!      }
//!

struct JsonSettings final // : ImplementsStatic<IAppSettings>
{
  using json = nlohmann::json;

  JsonSettings();
  explicit JsonSettings(const char *str);
  explicit JsonSettings(const fs::path &p);

  json        Get(std::string_view section) const;
  void        Set(std::string_view section, const json &js);
  void        Save(std::ostream &fs);
  bool        Save();
  std::string ToString(bool formatted = true) const;

private:
  fs::path m_file;
  json     m_json;
  bool     m_modified{};

  void Load(std::istream &fs);

  static std::string GetMainJsonFile();
};
