#pragma once

#include <variant>
#include <functional>
#include <optional>
#include "AppSettingsImpl.h"

class AppSettings // : public Singleton<AppSettings>
{
public:
  //template <class Func>
  //AppSettings(Func &&func)
  //  : AppSettings()
  //{
  //  SetCallback(func);
  //}

  AppSettings();
  AppSettings(const fs::path &jsonfile);
  AppSettings(std::string_view jsonfile);
  AppSettings(const AppSettings &aps, std::string_view addr);

  ~AppSettings();

  template <class Func>
  void SetCallback(Func &&func)
  {
    if (m_sigId) {
      AppSettingsImpl::Instance()->m_notify.Disconnect(m_sigId);
      m_sigId = {};
    }

    if (func) {
      m_sigId = AppSettingsImpl::Instance()->m_notify.Connect([func, this](const json *p) {
        m_json = *p;
        func();
      });
    }
  }

  /// 'addr' is in a.b.c format
  template <class T>
  std::optional<T> Get(std::string_view addr)
  {
    auto v = GetData(addr);

    if (std::holds_alternative<T>(v))
      return std::get<T>(v);

    if constexpr (std::is_floating_point_v<T>) {
      if (std::holds_alternative<int>(v))
        return static_cast<T>(std::get<int>(v));
    }
    return {};
  }

  template <class T>
  T Get(std::string_view addr, T defaultValue)
  {
    auto v = Get<T>(addr);
    return v.has_value() ? v.value() : defaultValue;
  }

  static fs::path GetStdSettingsFile();

private:
  using json = nlohmann::json;

  json m_json;
  int  m_sigId{};

  using Data = std::variant<std::monostate, bool, int, double, std::string>;

  Data        GetData(std::string_view addr) const;
  const json *GetJsonData(std::string addr) const;
  auto        SplitAddr(std::string &addr) const -> std::vector<std::string>;
};

