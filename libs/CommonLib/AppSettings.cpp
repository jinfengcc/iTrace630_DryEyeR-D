#include "pch.h"
#include "AppSettings.h"
#include <filesystem>
#include <fstream>
#include "AppSettingsImpl.h"

////////////////////////////////////////////////////////////////////

AppSettings::AppSettings(std::function<void()> func)
  : m_json(AppSettingsImpl::Instance()->GetJson())
{
  if (func) {
    m_sigId = AppSettingsImpl::Instance()->m_notify.Connect([func, this](const json *p) {
      m_json = *p;
      func();
    });
  }
}

AppSettings::AppSettings(const fs::path &jsonfile)
{
  m_json = AppSettingsImpl::LoadJson(std::ifstream(jsonfile));
}

AppSettings::AppSettings(std::string_view jsonStr)
{
  m_json = AppSettingsImpl::LoadJson(std::stringstream(jsonStr.data()));
}

AppSettings::AppSettings(const AppSettings &aps, std::string_view addr)
{
  if (auto j = aps.GetJsonData(std::string(addr)))
    m_json = *j;
}

AppSettings::~AppSettings()
{
  if (m_sigId)
    AppSettingsImpl::Instance()->m_notify.Disconnect(m_sigId);
}

AppSettings::Data AppSettings::GetData(std::string_view addr) const
{
  if (auto js = GetJsonData(addr.data())) {
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

const json *AppSettings::GetJsonData(std::string addr) const
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

auto AppSettings::SplitAddr(std::string &addr) const -> std::vector<std::string>
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

fs::path AppSettings::GetStdSettingsFile()
{
  return AppSettingsImpl::Instance()->m_file;
}
