#include "pch.h"
#include "ResourceData.h"
#include <atlfile.h>

ResourceData::operator bool() const
{
  return FindResource(GetInstanceHandle(), m_pszRsrcId, m_pszRsrcType) != nullptr;
}

bool  ResourceData::LoadData(IStream **ppStream)
{
  auto data = GetRsrcData();
  if (data.empty())
    return false;

  CComPtr<IStream> pStream;
  HRESULT hr = CreateStreamOnHGlobal(nullptr, TRUE, &pStream);
  if (hr != S_OK)
    return hr;

  ULONG ul;
  hr = pStream->Write(data.data(), data.size(), &ul);
  if (hr != S_OK)
    return hr;

  if (static_cast<ptrdiff_t>(ul) != data.size())
    return false;

  LARGE_INTEGER u1 = { 0 };
  pStream->Seek(u1, STREAM_SEEK_SET, nullptr);
  *ppStream = pStream.Detach();
  return true;
}

auto ResourceData::GetRsrcDataImpl() -> std::pair<const void *, size_t>
{
  HINSTANCE hInst = GetInstanceHandle();

  HRSRC  hRsrc = ::FindResource(hInst, m_pszRsrcId, m_pszRsrcType);
  if (hRsrc == nullptr)
    return {};

  HGLOBAL hResource = ::LoadResource(hInst, hRsrc);
  if (hResource == nullptr)
    return {};

  return {
    ::LockResource(hResource),
    static_cast<size_t>(::SizeofResource(hInst, hRsrc))
  };
}

