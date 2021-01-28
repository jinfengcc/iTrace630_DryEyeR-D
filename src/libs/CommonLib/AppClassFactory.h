#pragma once

#include <atlbase.h>
#include <string>
#include <list>
#include <mutex>
#include <filesystem>
#include <wil/com.h>

#include "Implements.h"
#include "Singleton.h"

class AppClassFactory : public Implements<IClassFactory>
{
public:
  AppClassFactory()
    : Implements<IClassFactory>("AppClassFactory")
  {
    if (Globals::Instance()->_list.empty())
      CreateDataList();
  }

  template <class U>
  wil::com_ptr_t<U> Create()
  {
    wil::com_ptr_t<U> ptr;
    CreateInstance(nullptr, __uuidof(U), ptr.put_void());
    return ptr;
  }

  // IClassFactory
  HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObject) override;
  HRESULT STDMETHODCALLTYPE LockServer(BOOL /*fLock*/) override
  {
    return E_NOTIMPL;
  }

private:
  struct Data
  {
    std::filesystem::path         path;
    wil::com_ptr_t<IClassFactory> factory;
    std::list<GUID>               guids;
  };

  using DataList = std::vector<Data>;

  struct Globals : Singleton<Globals>
  {
    std::mutex _mutex;
    DataList   _list;
  };

  static void CreateDataList();
  static Data CreateData(std::filesystem::path &&p);
  static void VerifyUnique(const DataList &dl);

  static DataList GetDllList();
};
