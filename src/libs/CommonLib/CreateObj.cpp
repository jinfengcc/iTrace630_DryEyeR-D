#include "pch.h"
#include "CreateObj.h"
#include "AppClassFactory.h"

namespace {
  HMODULE GetCurrentModule()
  {
    DWORD   flags = GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS;
    HMODULE hm    = nullptr;
    GetModuleHandleEx(flags, reinterpret_cast<LPCTSTR>(&GetCurrentModule), &hm);
    return hm;
  }

  IClassFactory *g_classFactory = nullptr;
} // namespace

#define API __declspec(dllexport)

extern "C" IClassFactory API *MainSysClassFactory()
{
  static AppClassFactory cf;
  return &cf;
}

IClassFactory *GetSysClassFactory()
{
  if (g_classFactory == nullptr) {
    static std::mutex mutex;
    std::lock_guard   lock(mutex);
    if (g_classFactory == nullptr) {
      HMODULE cur = GetCurrentModule();
      HMODULE exe = GetModuleHandle(nullptr);

      if (cur != exe) {
        using FUNC = IClassFactory *(*)();
        if (auto func = (FUNC)GetProcAddress(exe, "MainSysClassFactory"))
          g_classFactory = func();
      }

      if (g_classFactory == nullptr)
        g_classFactory = MainSysClassFactory();
    }
  }

  return g_classFactory;
}

void SetSysClassFactory(IClassFactory *f)
{
  g_classFactory = f;
}
