#pragma once

#include <string>
#include <map>
#include <variant>
#include <optional>
#include "Guid.h"
#include "NonCopyable.h"

template<class Key, class ...Args>
class ConfigMap : public NonCopyable
{
public:
  template<class T>
  void Set(Key key, const T &data) noexcept
  {
    m_data[key] = data;
  }

  template <class T>
  bool TryGet(Key key, T &data) const noexcept
  {
    if (auto i = m_data.find(key); i != m_data.end()) {
      if (auto p = std::get_if<T>(&i->second)) {
        data = *p;
        return true;
      }
    }
    return false;
  }
  bool TryGet(Key key, double &data) const noexcept
  {
    if (!TryGet<double>(key, data)) {
      int x;
      if (!TryGet<int>(key, x))
        return false;
      data = x;
    }

    return true;
  }

  template <class T>
  std::optional<T> Get(Key key) const noexcept
  {
    T data{};
    if (TryGet<T>(key, data))
      return {data};
    else
      return {};
  }

  template <class T>
  T Get(Key key, T defValue) const noexcept
  {
    T v;
    return TryGet(key, v) ? v : defValue;
  }


private:
  using Data = std::variant<Args...>;
  using Map  = std::map<Key, Data>;

  Map m_data;
};

