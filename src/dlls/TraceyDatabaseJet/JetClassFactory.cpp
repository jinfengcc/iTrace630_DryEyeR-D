#include "stdafx.h"
#include <libs/CommonLib/ClassFactoryImpl.h>

#include "DatabaseJet.h"
#include "DatabaseServices.h"

#define API __declspec(dllexport)

struct JetClassFactory : ClassFactoryImpl
{
  JetClassFactory()
  {
    // clang-format off
    AddCreator(__uuidof(IDatabaseJet)     , &JetClassFactory::CreateObj<DatabaseJet>);
    AddCreator(__uuidof(IDatabaseServices), &JetClassFactory::CreateObj<DatabaseServices>);
    // clang-format on
  }

private:
  template <class T>
  static bool CreateObj(REFIID riid, void **ppv)
  {
    auto obj = new T();
    return obj->QueryInterface(riid, ppv) == S_OK;
  }
};

extern "C" IClassFactory API *CreateClassFactory()
{
  static JetClassFactory cf;
  return cf.Factory();
}
