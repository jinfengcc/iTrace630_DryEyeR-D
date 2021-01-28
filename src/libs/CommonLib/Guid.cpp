#include "pch.h"
#include "Guid.h"

#pragma comment(lib, "Rpcrt4.lib")

Guid::Guid(std::string_view s, bool throwOnError)
  : Guid(utf8_convert(s), throwOnError)
{
}

Guid::Guid(std::wstring_view s, bool throwOnError)
{
  std::wstring s1(s);

  // skip '{', '}' if they exist
  if (s1[0] == '{' && s1.size() > 2)
    s1 = s1.substr(1, s1.size() - 2);

  if (UuidFromStringW(reinterpret_cast<RPC_WSTR>(s1.data()), &m_guid) != RPC_S_OK) {
    if (throwOnError)
      TRACEY_THROW("Invalid GUID string");
    else
      m_guid = GUID_NULL;
  }
}

std::string Guid::string(bool withBraces) const
{
  auto &g = m_guid;

  fmt::memory_buffer buf;

  if (withBraces)
    fmt::format_to(buf, "{}", '{');

  fmt::format_to(buf, "{:08X}-", g.Data1);
  fmt::format_to(buf, "{:04X}-", g.Data2);
  fmt::format_to(buf, "{:04X}-", g.Data3);
  fmt::format_to(buf, "{:02X}{:02X}-", g.Data4[0], g.Data4[1]);
  fmt::format_to(buf, "{:02X}{:02X}{:02X}{:02X}{:02X}{:02X}", g.Data4[2], g.Data4[3], g.Data4[4], g.Data4[5], g.Data4[6], g.Data4[7]);

  if (withBraces)
    fmt::format_to(buf, "{}", '}');

  return fmt::to_string(buf);
}

std::wstring Guid::wstring(bool withBraces) const
{
  return utf8_convert(string(withBraces));
}
