#include "pch.h"
#include "AppSettings.h"

AppSettings::AppSettings(std::string_view str_or_filename)
{
  if (fs::is_regular_file(str_or_filename)) {
    m_file = str_or_filename;
    std::ifstream fs(str_or_filename);
    Load(fs);
  }
  else {
    m_json = json::parse(str_or_filename.data());
  }
}

AppSettings::~AppSettings()
{
  Flush();
}

void AppSettings::Load(std::istream &fs)
{
  m_json     = json::parse(fs);
  m_modified = false;
}

void AppSettings::Save(std::ostream &fs)
{
  fs << ToString();
}

auto AppSettings::Get(std::string_view section) const -> json
{
  if (section.empty())
    return m_json;

  return m_json.at(section.data());
}

void AppSettings::Set(std::string_view section, const json &js)
{
  m_modified             = true;
  m_json[section.data()] = js;
}

void AppSettings::Flush()
{
  if (m_modified && fs::is_regular_file(m_file)) {
    std::ofstream(m_file.string()) << m_json.dump(2);
    m_modified = false;
  }
}
