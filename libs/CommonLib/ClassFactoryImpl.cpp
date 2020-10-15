#include "pch.h"
#include "ClassFactoryImpl.h"

HRESULT __stdcall ClassFactoryImpl::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObject)
{
  UNREFERENCED_PARAMETER(pUnkOuter);
  for (auto &f : m_funcs) {
    if (f.id == riid) {
      if (f.func(riid, ppvObject))
        return S_OK;
    }
  }

  *ppvObject = nullptr;
  return E_NOINTERFACE;
}

HRESULT __stdcall ClassFactoryImpl::LockServer(BOOL)
{
  return E_NOTIMPL;
}

std::vector<GUID> ClassFactoryImpl::Implements() const
{
  auto v = std::vector<GUID>();
  std::transform(m_funcs.begin(), m_funcs.end(), std::back_inserter(v), [](const auto &x) { return x.id; });
  return v;
}
