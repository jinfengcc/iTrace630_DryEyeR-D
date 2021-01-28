#pragma once

#include <atlbase.h>
#include <string_view>
#include <stdexcept>

class Guid
{
public:
  Guid() = default;

  Guid(const GUID &g)
    : m_guid(g)
  {
  }
  explicit Guid(std::string_view s, bool throwOnError = true);
  explicit Guid(std::wstring_view s, bool throwOnError = true);

  static Guid NullGuid()
  {
    return Guid{GUID_NULL};
  }
  static Guid NewGuid()
  {
    auto g = Guid{GUID_NULL};
    if (UuidCreate(&g.m_guid) != RPC_S_OK)
      throw std::runtime_error("Unable to create a new GUID");
    return g;
  }

  explicit operator bool() const
  {
    return m_guid != GUID_NULL;
  }

  operator GUID() const
  {
    return m_guid;
  }
  const GUID &get() const
  {
    return m_guid;
  }

  std::string  string(bool withBraces = true) const;
  std::wstring wstring(bool withBraces = true) const;

  friend bool operator<(const Guid &a, const Guid &b)
  {
    return memcmp(&a.m_guid, &b.m_guid, sizeof(GUID)) < 0;
  }

private:
  GUID m_guid{GUID_NULL};
};

// Specialize std::hash
namespace std {
  template <>
  struct hash<Guid>
  {
    size_t operator()(const Guid &guid) const noexcept
    {
      RPC_STATUS status{};
      unsigned   hash = UuidHash(const_cast<UUID *>(&guid.get()), &status);
      return status == RPC_S_OK ? hash : 0;
    }
  };
} // namespace std
