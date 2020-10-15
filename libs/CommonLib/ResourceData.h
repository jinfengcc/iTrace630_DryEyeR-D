#pragma once

class ResourceData
{
public:
  ResourceData(const TCHAR *pszRsrcType, const TCHAR *pszRsrcId, HINSTANCE hInstance = nullptr)
    : m_pszRsrcType(pszRsrcType)
    , m_pszRsrcId(pszRsrcId)
    , m_hInstance(hInstance)
  {
  }

  ResourceData(const TCHAR *pszRsrcType, int iRsrcId, HINSTANCE hInstance = nullptr)
    : ResourceData(pszRsrcType, MAKEINTRESOURCE(iRsrcId), hInstance)
  {
  }

  explicit operator bool() const;

  // Read functionality
  bool LoadData(IStream **pStream);
  auto GetRsrcData() -> std::span<const BYTE>
  {
    auto [com, size] = GetRsrcDataImpl();
    return {static_cast<const BYTE *>(com), size};
  }

private:
  HINSTANCE    m_hInstance;
  const TCHAR *m_pszRsrcType;
  const TCHAR *m_pszRsrcId;

  HINSTANCE GetInstanceHandle() const
  {
    return m_hInstance ? m_hInstance : GetModuleHandle(nullptr);
  }

  auto GetRsrcDataImpl() -> std::pair<const void *, size_t>;
};
