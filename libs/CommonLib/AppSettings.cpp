#include "pch.h"
#include "AppSettings.h"
#include <filesystem>
#include <fstream>
#include <wil/filesystem.h>
#include <interfaces/ISettingsStorage.h>
#include "SignalUI.h"

struct AppSettings::Impl
{
  fs::path                         m_file;
  json                             m_json;
  sig::SignalUI<>                  m_notify;
  wil::unique_folder_change_reader m_dirWatcher;

  Impl(const fs::path &file);
  const json *GetData(std::string addr) const;

private:
  void Load();
  void OnFileChanged(wil::FolderChangeEvent, const wchar_t *fileName);

  std::vector<std::string> SplitAddr(std::string &addr) const;
};

////////////////////////////////////////////////////////////////////

AppSettings::AppSettings()
{
  auto ss = CreateObj<ISettingsStorage>();
  m_pimpl = std::make_unique<Impl>(ss->GetSysFile());
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

std::string AppSettings::GetSection(std::string_view addr) const
{
  return std::string();
}

AppSettings::Data AppSettings::GetData(std::string_view addr) const
{
  if (auto js = m_pimpl->GetData(addr.data())) {
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

AppSettings::Impl::Impl(const fs::path &file)
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
  if (!fs::exists(m_file))
    return;

  for (int i = 0; i++ < 3; ) {
    try {
      auto fs = std::ifstream(m_file);
      m_json  = json::parse(fs, nullptr, true, true);
      break;
    }
    catch (std::exception &e) {
      DILASCIA_TRACE("Error: {}\n", e.what());
      Sleep(i*100);
    }
  }
}

void AppSettings::Impl::OnFileChanged(wil::FolderChangeEvent, const wchar_t *fileName)
{
  if (_wcsicmp(fileName, m_file.filename().wstring().c_str()) == 0) {
    Load();
    m_notify();
  }
}

const json *AppSettings::Impl::GetData(std::string addr) const
{
  if (!m_json.is_object())
    return nullptr;

  auto kv = SplitAddr(addr);
  auto js = &m_json;

  for (auto &k : kv) {
    if (auto i = js->find(k); i != js->end()) {
      js = &*i;
    }
    else {
      js = nullptr;
      break;
    }
  }

  return js;
}

std::vector<std::string> AppSettings::Impl::SplitAddr(std::string &addr) const
{
  auto kv = std::vector<std::string>();

  for (auto n = addr.find('.'); !addr.empty() && n != std::string::npos;) {
    kv.push_back(addr.substr(0, n));
    addr = addr.substr(n + 1);
    n    = addr.find('.');
  }
  if (!addr.empty())
    kv.push_back(addr);

  return kv;
}
