#include "pch.h"
#include "ComLibrary.h"
#include <comutil.h>

ComLibrary::ComLibrary(const wchar_t *szDllName)
{
  if (szDllName)
    Load(szDllName);
}

ComLibrary::~ComLibrary()
{
  Unload(TRUE);
}

HRESULT ComLibrary::Load(const wchar_t *szDllName)
{
  if (m_hDll)
    return S_FALSE; // is already loaded

  // Add a bit of securty:
  // 1) deny network paths
  if (PathIsNetworkPathW(szDllName))
    return E_INVALIDARG;

  // load library
  _bstr_t dllName(szDllName);
  m_hDll = ::CoLoadLibrary(dllName, FALSE);
  if (!m_hDll)
    return HRESULT_FROM_WIN32(::GetLastError());

  // get DllGetClassObject and DllCanUnloadNow
  m_DllGetClassObject = (FnDllGetClassObject)::GetProcAddress(m_hDll, "DllGetClassObject");
  if (!m_DllGetClassObject)
    return HRESULT_FROM_WIN32(::GetLastError());

  // DllCanUnloadNow is optional
  m_DllCanUnloadNow = (FnDllCanUnloadNow)::GetProcAddress(m_hDll, "DllCanUnloadNow");

  return S_OK;
}

//----------------------------------------------------------------------------

HRESULT ComLibrary::Unload(BOOL bForce)
{
  if (!m_hDll)
    return S_OK; // not loaded or already unloaded

  // call DllCanUnloadNow
  HRESULT hr = this->CanUnloadNow();

  // if can NOT unload AND NOT force...
  if ((S_OK != hr) && !bForce) {
    return hr;
  } // ... don't close

  // close: free and clean up
  m_DllGetClassObject = nullptr;
  m_DllCanUnloadNow   = nullptr;

  ::CoFreeLibrary(m_hDll);
  m_hDll = nullptr;

  return S_OK;
}
