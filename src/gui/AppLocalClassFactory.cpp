#include "stdafx.h"
#include <libs/CommonLib/ClassFactoryImpl.h>
#include <libs/CommonLib/AppSettings.h>

namespace {

  struct AppLocalClassFactory : public ClassFactoryImpl
  {
    AppLocalClassFactory()
    {
    }

    static bool CreateObj(REFIID riid, void **ppv)
    {
      return false;
    }
  };
} // namespace
#define API __declspec(dllexport)

extern "C" IClassFactory API *CreateClassFactory()
{
  static AppLocalClassFactory cf;
  return cf.Factory();
}
