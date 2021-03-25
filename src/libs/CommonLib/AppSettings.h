#pragma once

#include <variant>
#include <functional>
#include <optional>
#include "AppSettingsImpl.h"

class AppSettings
{
public:
  AppSettings();
  AppSettings(const fs::path &jsonfile);
  AppSettings(std::string_view jsonfile);
  AppSettings(const AppSettings &aps, std::string_view addr);

  ~AppSettings();

  template <class Func>
  void SetCallback(Func &&func)
  {
    ClearCallback();
    m_sigId = AppSettingsImpl::Instance()->m_notify.Connect([func, this](const json *p) {
      m_json = *p;
      func();
    });
  }

  /// 'addr' is in a.b.c format

  using json = nlohmann::json;
  const json *GetJsonData(std::string addr) const;

  template <class T>
  std::optional<T> Get(std::string_view addr)
  {
    auto v = GetData(addr);

    if (std::holds_alternative<T>(v))
      return std::get<T>(v);

    if constexpr (std::is_floating_point_v<T> || std::is_unsigned_v<T>) {
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

  template <class T>
  void Get(std::string_view addr, T *value)
  {
    if (auto v = Get<T>(addr); v.has_value())
      *value = v.value();
  }

  static fs::path GetStdSettingsFile();

private:

  json m_json;
  int  m_sigId{};

  using Data = std::variant<std::monostate, bool, int, double, std::string>;

  void        ClearCallback();
  Data        GetData(std::string_view addr) const;
  auto        SplitAddr(std::string &addr) const -> std::vector<std::string>;
};
