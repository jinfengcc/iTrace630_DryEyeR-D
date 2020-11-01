#include "pch.h"
#include "AppSettings.h"
#include <filesystem>
#include <fstream>
#include <wil/filesystem.h>
#include "SignalUI.h"

struct AppSettings::Impl
{
  fs::path                         m_file;
  json                             m_json;
  sig::SignalUI<>                  m_notify;
  wil::unique_folder_change_reader m_dirWatcher;

  Impl(const std::wstring &file);
  const json *GetData(std::string_view section, std::string_view name) const;

private:
  void Load();
  void OnFileChanged(wil::FolderChangeEvent, const wchar_t *fileName);
};

////////////////////////////////////////////////////////////////////

AppSettings::AppSettings()
  : m_pimpl(std::make_unique<Impl>(GetJsonFilename()))
{
}

AppSettings::AppSettings(std::wstring_view jsonfile)
  : m_pimpl(std::make_unique<Impl>(std::wstring(jsonfile)))
{
}

AppSettings::~AppSettings()
{
}

int AppSettings::AddNotify(std::function<void()> func)
{
  return m_pimpl->m_notify.Connect(std::move(func));
}

void AppSettings::RemoveNotify(int id)
{
  m_pimpl->m_notify.Disconnect(id);
}

AppSettings::Data AppSettings::GetData(std::string_view section, std::string_view name) const
{
  if (auto js = m_pimpl->GetData(section, name)) {
    if (js->is_boolean())
      return js->get<bool>();
    else if (js->is_number_float())
      return {js->get<double>()};
    else if (js->is_number())
      return {js->get<int>()};
    else if (js->is_string())
      return {js->get<std::string>()};
  }

  return {};
}

std::wstring AppSettings::GetConfigFile() const
{
  return m_pimpl->m_file.wstring();
}

fs::path AppSettings::GetJsonFilename()
{
  TCHAR exe[_MAX_PATH];
  GetModuleFileName(nullptr, exe, _MAX_PATH);

  auto path = fs::path(exe);
  path.replace_extension(".json");

  return path;
}

////////////////////////////////////////////////////////////////////

AppSettings::Impl::Impl(const std::wstring &file)
  : m_file(file)
{
  Load();

  wchar_t sz[_MAX_PATH * 2];
  wcscpy_s(sz, m_file.parent_path().wstring().c_str());

  if (sz[wcslen(sz) - 1] != '\\')
    wcscat_s(sz, L"\\");

  auto callback = [this](wil::FolderChangeEvent event, PCWSTR fileName) { OnFileChanged(event, fileName); };
  m_dirWatcher  = wil::make_folder_change_reader(sz, false, wil::FolderChangeEvents::All, callback);
}

void AppSettings::Impl::Load()
{
  m_json.clear();
  if (fs::exists(m_file)) {
    auto fs = std::ifstream(m_file);
    m_json  = json::parse(fs, nullptr, true, true);
  }
}

void AppSettings::Impl::OnFileChanged(wil::FolderChangeEvent, const wchar_t *fileName)
{
  if (_wcsicmp(fileName, m_file.filename().wstring().c_str()) == 0) {
    Load();
    m_notify();
  }
}

const json *AppSettings::Impl::GetData(std::string_view section, std::string_view name) const
{
  if (!m_json.is_object())
    return nullptr;

  if (auto &j1 = m_json.at(section.data()); j1.is_object()) {
    if (auto &j2 = j1.at(name.data()); !j2.is_null())
      return &j2;
  }

  return nullptr;
}
