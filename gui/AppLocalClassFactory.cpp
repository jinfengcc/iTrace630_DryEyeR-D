#include "stdafx.h"
#include <libs/CommonLib/ClassFactoryImpl.h>
#include <libs/CommonLib/AppSettings.h>

namespace {

  struct AppLocalClassFactory : public ClassFactoryImpl
  {
    AppLocalClassFactory()
      : ClassFactoryImpl({{__uuidof(ISettings), &AppLocalClassFactory::CreateObj}})
    {
      AddCreator(__uuidof(ISettings), &AppLocalClassFactory::CreateObj);
    }

    static bool CreateObj(REFIID riid, void **ppv)
    {
      if (riid == __uuidof(ISettings)) {
        return CreateSettings(ppv);
      }
      else {
        return false;
      }
    }

    static bool CreateSettings(void **ppv)
    {
      static AppSettings *appSettings;
      if (appSettings == nullptr) {
        ASSERT(FALSE);
      }
      return appSettings->QueryInterface(__uuidof(ISettings), ppv) == S_OK;
    }
  };
} // namespace
#define API __declspec(dllexport)

extern "C" IClassFactory API *CreateClassFactory()
{
  static AppLocalClassFactory cf;
  return cf.Factory();
}
