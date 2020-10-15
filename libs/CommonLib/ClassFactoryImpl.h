#pragma once

#include <Unknwn.h>
#include <vector>
#include <functional>
#include <libs/CommonLib/SafeType.h>
#include <libs/CommonLib/Implements.h>
#include <libs/CommonLib/IClassFactoryInfo.h>

struct ClassFactoryImpl : ImplementsStatic<IClassFactory, IClassFactoryInfo>
{
  using Func = std::function<bool(REFIID, void **)>;

  struct Pair
  {
    GUID id;
    Func func;
  };

  ClassFactoryImpl() = default;
  ClassFactoryImpl(std::initializer_list<Pair> il)
    : m_funcs(il.begin(), il.end())
  {
  }
  template <class Func>
  void AddCreator(REFIID riid, Func &&func)
  {
    m_funcs.push_back({riid, std::move(func)});
  }

  IClassFactory *Factory()
  {
    return this;
  }

  // IClassFactory
  HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObject) override;
  HRESULT STDMETHODCALLTYPE LockServer(BOOL /*fLock*/) override;

  // IClassFactoryInfo
  std::vector<GUID> Implements() const override;

private:
  std::vector<Pair> m_funcs;
};
