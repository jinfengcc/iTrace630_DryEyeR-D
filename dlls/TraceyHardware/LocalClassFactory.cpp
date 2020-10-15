#include "pch.h"
#include <libs/CommonLib/ClassFactoryImpl.h>

#include "CameraHiRes.h"

#define API __declspec(dllexport)

namespace {
  struct LocalClassFactory : ClassFactoryImpl
  {
    LocalClassFactory()
    {
      // clang-format off
      AddCreator(__uuidof(hal::ICameraHires       ), &LocalClassFactory::Create<CameraHires>);
      // clang-format on
    }

  private:
    // #todo Handle simulated hardware

    template <class T>
    static bool Create(REFIID riid, void **ppv)
    {
      auto obj = new T();
      return obj->QueryInterface(riid, ppv) == S_OK;
    }

  };
} // namespace

extern "C" IClassFactory API *CreateClassFactory()
{
  static LocalClassFactory cf;
  return cf.Factory();
}
