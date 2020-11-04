#pragma once

#include <variant>
#include <functional>
#include <optional>
#include <filesystem>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;

class AppSettings // : public Singleton<AppSettings>
{
public:
  AppSettings(std::function<void()> func = nullptr);
  AppSettings(const fs::path &jsonfile);
  AppSettings(std::string_view jsonfile);
  AppSettings(const AppSettings &aps, std::string_view addr);

  ~AppSettings();

  /// 'addr' is in a.b.c format
  template <class T>
  std::optional<T> Get(std::string_view addr);

  template <class T>
  T Get(std::string_view addr, T defaultValue);

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

template <class T>
inline std::optional<T> AppSettings::Get(std::string_view addr)
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
inline T AppSettings::Get(std::string_view addr, T defaultValue)
{
  auto v = Get<T>(addr);
  return v.has_value() ? v.value() : defaultValue;
}
