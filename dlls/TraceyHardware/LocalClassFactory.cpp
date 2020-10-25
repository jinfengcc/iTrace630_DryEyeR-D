#include "pch.h"
#include <libs/CommonLib/ClassFactoryImpl.h>

#include "Device.h"
#include "Flash.h"
#include "Camera.h"
#include "CameraHiRes.h"

#define API __declspec(dllexport)

namespace {
  struct LocalClassFactory : ClassFactoryImpl
  {
    LocalClassFactory()
    {
      // clang-format off
      AddCreator(__uuidof(hal::IDevice            ), &LocalClassFactory::Create<Device            >);
      AddCreator(__uuidof(hal::IFlash             ), &LocalClassFactory::Create<Flash             >);
      AddCreator(__uuidof(hal::ICamera            ), &LocalClassFactory::Create<Camera            >);
      AddCreator(__uuidof(hal::ICameraHires       ), &LocalClassFactory::Create<CameraHires>);
      // clang-format on
    }

  private:
    // #todo Handle simulated hardware

    template <class T>
    static bool Create(REFIID riid, void **ppv)
    {
      auto obj = new T(GetDllStuff());
      return obj->QueryInterface(riid, ppv) == S_OK;
    }
    template <>
    static bool Create<CameraHires>(REFIID riid, void **ppv)
    {
      auto obj = new CameraHires;
      return obj->QueryInterface(riid, ppv) == S_OK;
    }

    static THW *GetDllStuff()
    {
      static THW dll;
      return &dll;
    }
  };
} // namespace

extern "C" IClassFactory API *CreateClassFactory()
{
  static LocalClassFactory cf;
  return cf.Factory();
}
