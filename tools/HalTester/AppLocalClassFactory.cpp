#include "pch.h"
#include <libs/CommonLib/ClassFactoryImpl.h>
#include "SettingsStorage.h"


namespace {

  struct AppLocalClassFactory : public ClassFactoryImpl
  {
    AppLocalClassFactory()
    {
      AddCreator(__uuidof(ISettingsStorage), &AppLocalClassFactory::CreateObj);
    }

    static bool CreateObj(REFIID riid, void **ppv)
    {
      if (riid == __uuidof(ISettingsStorage)) {
        return CreateSettings(ppv);
      }
      else {
        return false;
      }
    }

    static bool CreateSettings(void **ppv)
    {
      static SettingsStorage appSettings;
      return appSettings.QueryInterface(__uuidof(ISettingsStorage), ppv) == S_OK;
    }
  };
} // namespace
#define API __declspec(dllexport)

extern "C" IClassFactory API *CreateClassFactory()
{
  static AppLocalClassFactory cf;
  return cf.Factory();
}
