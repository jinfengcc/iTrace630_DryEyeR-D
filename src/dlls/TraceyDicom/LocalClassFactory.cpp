#include "pch.h"

#define API __declspec(dllexport)

#include "TraceyDicomQuery.h"
#include "TraceyDicomSend.h"
#include "TraceyDicomConfig.h"

namespace {
  struct LocalClassFactory : ClassFactoryImpl
  {
    LocalClassFactory()
    {
      // clang-format off
      AddCreator(__uuidof(ITraceyDicomQuery      ), &LocalClassFactory::Create<TraceyDicomQuery >);
      AddCreator(__uuidof(ITraceyDicomSend       ), &LocalClassFactory::Create<TraceyDicomSend  >);
      AddCreator(__uuidof(ITraceyDicomConfig     ), &LocalClassFactory::Create<TraceyDicomConfig>);
      AddCreator(__uuidof(ITraceyDicomConfigRZDCX), &LocalClassFactory::Create<TraceyDicomConfig>);
      // clang-format on
    }

  private:
    template <class T>
    static bool Create(REFIID riid, void **ppv)
    {
      auto obj = new T;
      return obj->QueryInterface(riid, ppv) == S_OK;
    }
  };
} // namespace

extern "C" IClassFactory API *CreateClassFactory()
{
  static LocalClassFactory cf;
  return cf.Factory();
}
