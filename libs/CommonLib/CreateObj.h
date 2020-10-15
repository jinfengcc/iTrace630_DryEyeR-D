#pragma once

#include <concepts>
#include <wil/com.h>

IClassFactory *GetSysClassFactory();
void           SetSysClassFactory(IClassFactory *f);

namespace crob {
  namespace details {
    template <class T>
    concept has_initialize0 = requires(T &t)
    {
      t.Initialize();
    };

    template <class T, class U>
    concept has_initialize1 = requires(T &t, U unk)
    {
      t.Initialize(unk);
    };

    template <class T>
    inline void Initialize(wil::com_ptr_t<T> &obj, IUnknown *unk)
    {
      try {
        if constexpr (has_initialize0<T>)
          obj->Initialize();
        else if constexpr (has_initialize1<T, decltype(unk)>)
          obj->Initialize(unk);
      }
      catch (...) 
      {
        //...
      }
    }

  } // namespace details

  template <typename Obj>
  bool CreateObj(IClassFactory *factory, wil::com_ptr_t<Obj> &obj, IUnknown *unk = nullptr)
  {
    if (factory == nullptr || factory->CreateInstance(nullptr, __uuidof(Obj), obj.put_void()) != S_OK)
      return false;

    details::Initialize(obj, unk);

    return true;
  }

  template <typename Obj>
  auto CreateObj(IClassFactory *factory, IUnknown *unk)
  {
    wil::com_ptr_t<Obj> obj;
    CreateObj<Obj>(factory, obj, unk);
    return obj;
  }

  template <typename Obj>
  wil::com_ptr_t<Obj> CreateObj(IUnknown *factory, IUnknown *unk)
  {
    auto f = factory.query<IClassFactory>();
    return CreateObj<Obj>(f.get(), unk);
  }

  template <typename Obj>
  bool CreateObj(wil::com_ptr_t<Obj> &obj, IUnknown *unk = nullptr)
  {
    return CreateObj<Obj>(GetSysClassFactory(), obj, unk);
  }

  template <typename Obj>
  auto CreateObj(IUnknown *unk = nullptr)
  {
    return CreateObj<Obj>(GetSysClassFactory(), unk);
  }
} // namespace crob

using crob::CreateObj;
