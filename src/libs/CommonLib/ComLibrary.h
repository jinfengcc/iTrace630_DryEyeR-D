#pragma once
///////////////////////////////////////////////////////////
// ComLibrary
// Allows in-proc server loading without the server being
// registered.
// It basically wraps DllGetClassObject(),
// IClassFactory::CreateInstance() and DllCanUnloadNow().
//
// Usage:
// Put a member somwhere:
//   ComLibrary mMyLib;
//
// Before you want to use it call:
//   HRESULT hr = mMyLib.load(L"C:\\absolute\\path\\to\\MyLib.dll");
//
// Note that the path has to be absolute and can't be a network path to
// prevent security issues with loading DLLs from somewhere unexpected.
//
// And when you finished:
//   mMyLib.unload();
//
// If you use the library from within another library it's
// probably a good idea to modify your DllCanUnloadNow()
// and call there also mMyLib.CanUnloadNow().

#include <wil/com.h>

class ComLibrary final
{
public:
  ComLibrary(const wchar_t *szDllName = nullptr);
  ~ComLibrary();

  HRESULT Load(const wchar_t *szDllName);
  HRESULT Unload(BOOL bForce = FALSE);

  // these two wrap the functions exported from the COM module
  HRESULT GetClassObject(REFCLSID aClsid, REFIID aIid, LPVOID *ppv) const
  {
    return m_DllGetClassObject ? m_DllGetClassObject(aClsid, aIid, ppv) : E_UNEXPECTED;
  }

  HRESULT CanUnloadNow() const
  {
    return m_DllCanUnloadNow ? m_DllCanUnloadNow() : S_OK; // default: yes, can unload
  }

  // wraps DllGetClassObject and IClassFactory::CreateInstance
  template <class Iface>
  HRESULT CreateInstance(REFCLSID aClsid, Iface **aRetPtrPtr, IUnknown *aUnknownOuter = nullptr) const
  {
    wil::com_ptr<IClassFactory> factory;
    HRESULT hr = this->GetClassObject(aClsid, IID_IClassFactory, factory.put_void());
    if (FAILED(hr)) {
      return hr;
    }
    hr = factory->CreateInstance(aUnknownOuter, __uuidof(Iface), (void **)aRetPtrPtr);
    return hr;
  }

  template <class Iface>
  wil::com_ptr<Iface> CreateInstance(REFCLSID aClsid, IUnknown *aUnknownOuter = nullptr) const
  {
    wil::com_ptr<Iface> ptr;
    if (auto hr = CreateInstance<Iface>(aClsid, ptr.put(), aUnknownOuter); hr != S_OK)
      ptr.reset();

    return ptr;
  }

private:
  // prevent copying
  ComLibrary(const ComLibrary &) = delete;
  ComLibrary &operator=(const ComLibrary &) = delete;

  using FnDllGetClassObject = HRESULT(__stdcall *)(IN REFCLSID rclsid, IN REFIID riid, OUT LPVOID FAR *ppv);
  using FnDllCanUnloadNow   = HRESULT(__stdcall *)(void);

  HMODULE             m_hDll{};
  FnDllGetClassObject m_DllGetClassObject{};
  FnDllCanUnloadNow   m_DllCanUnloadNow{};
};
