#include "pch.h"
#include "JsonSettings.h"
#include "AppFiles.h"

JsonSettings::JsonSettings()
  : JsonSettings(app::GetAppPath(app::Type::json))
{
}

JsonSettings::JsonSettings(const char *str)
{
  if (str && *str) {
    m_json = json::parse(str);
  }
}

JsonSettings::JsonSettings(const fs::path &p)
{
  if (fs::exists(p)) {
    m_file = p;
    std::ifstream fs(p);
    Load(fs);
  }
}

void JsonSettings::Load(std::istream &fs)
{
  m_json     = json::parse(fs, nullptr, true, true);
  m_modified = false;
}

void JsonSettings::Save(std::ostream &fs)
{
  fs << ToString();
}

bool JsonSettings::Save()
{
  if (!m_modified)
    return true;

  if (fs::is_regular_file(m_file)) {
    std::ofstream(m_file.string()) << ToString();
    m_modified = false;
    return true;
  }
  else {
    return false;
  }
}

std::string JsonSettings::ToString(bool formatted) const
{
  return m_json.is_null() ? std::string() : m_json.dump(formatted ? 2 : -1);
}

auto JsonSettings::Get(std::string_view section) const -> json
{
  if (m_json.is_null() || section.empty())
    return m_json;

  if (auto i = m_json.find(section.data()); i != m_json.end())
    return *i;
  else
    return {};
}

void JsonSettings::Set(std::string_view section, const json &js)
{
  m_modified             = true;
  m_json[section.data()] = js;
}

std::string JsonSettings::GetMainJsonFile()
{
  char sz[_MAX_PATH];
  GetModuleFileNameA(nullptr, sz, _MAX_PATH);

  auto p = fs::path(sz);
  p.replace_extension(".json");

  return p.string();
}
