#pragma once

#include <variant>
#include <functional>
#include <optional>
#include "Singleton.h"

namespace fs = std::filesystem;

class AppSettings : public Singleton<AppSettings>
{
public:
  AppSettings();
  AppSettings(const fs::path &jsonfile);
  AppSettings(std::string_view jsonfile);

  ~AppSettings();

  AppSettings GetSection(std::string_view addr) const;

  int  AddNotify(std::function<void()> func);
  void RemoveNotify(int id);

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

  std::wstring GetConfigFile() const;

private:
  struct Impl;
  std::unique_ptr<Impl> m_pimpl;

  using Data = std::variant<std::monostate, bool, int, double, std::string>;
  Data GetData(std::string_view addr) const;

  static fs::path GetJsonFilename();
};
