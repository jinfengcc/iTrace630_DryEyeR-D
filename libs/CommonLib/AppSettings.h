#pragma once

#include <variant>
#include <functional>
#include "Singleton.h"

namespace fs = std::filesystem;

class AppSettings : public Singleton<AppSettings>
{
public:
  ~AppSettings();

  int  AddNotify(std::function<void()> func);
  void RemoveNotify(int id);

  template <class T>
  std::optional<T> Get(std::string_view section, std::string_view name)
  {
    auto v = GetData(section, name);

    if (std::holds_alternative<T>(v))
      return std::get<T>(v);

    if constexpr (std::is_floating_point_v<T>) {
      if (std::holds_alternative<int>(v))
        return static_cast<T>(std::get<int>(v));
    }
    return {};
  }

  std::wstring GetConfigFile() const;

private:
  struct Impl;
  std::unique_ptr<Impl> m_pimpl;

  friend class Singleton<AppSettings>;

  AppSettings();
  AppSettings(std::wstring_view jsonfile);

  using Data = std::variant<std::monostate, bool, int, double, std::string>;
  Data GetData(std::string_view section, std::string_view name) const;

  static fs::path GetJsonFilename();
};
