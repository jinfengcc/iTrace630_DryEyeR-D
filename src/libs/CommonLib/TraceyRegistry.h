#pragma once

#include <string>
#include <string_view>
#include <fmt/format.h>
#include <wtl/atlapp.h>
#include <wtl/atlmisc.h>

inline const wchar_t *TraceyRegistryKey()
{
  const static wchar_t root[] = LR"(Software\Tracey Technologies\iTrace)";
  return root;
}

inline std::wstring TraceyRegistryKey(std::wstring_view section)
{
  if (section.empty())
    return TraceyRegistryKey();

  return fmt::format(LR"({}\{})", TraceyRegistryKey(), section);
}

template <class Base>
class TraceyRegistryImpl : public CRegPropertyImpl<Base>
{
public:
  TraceyRegistryImpl(std::wstring_view section = {})
    : m_regKey(TraceyRegistryKey(section))
  {
  }

  void Load()
  {
    this->Read(m_regKey.c_str());
  }

  void Save()
  {
    this->Write(m_regKey.c_str());
  }

  template <class TProp>
  void ReadWriteProp(LPCTSTR lpstrRegValue, TProp &prop, bool bWrite)
  {
    if constexpr (std::is_enum_v<TProp>) {
      auto v = static_cast<int>(prop);
      CRegPropertyImpl<Base>::ReadWriteProp(lpstrRegValue, v, bWrite);
      prop = static_cast<TProp>(v);
    }
    else {
      CRegPropertyImpl<Base>::ReadWriteProp(lpstrRegValue, prop, bWrite);
    }
  }

private:
  std::wstring m_regKey;
};


