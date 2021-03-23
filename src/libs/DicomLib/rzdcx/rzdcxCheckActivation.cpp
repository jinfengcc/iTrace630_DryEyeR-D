#include "pch.h"
#include "rzdcxCheckActivation.h"

#include "rzdcx.h"
#include "libs/CommonLib/minhook/MinHook.hpp"

// typedef int(WINAPI *MESSAGEBOXA)(HWND, LPCSTR, LPCSTR, UINT);
using MESSAGEBOXA = decltype(&MessageBoxA);

static MESSAGEBOXA fpMessageBoxA = nullptr;

static int WINAPI DetourMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
  throw 1;
}

#pragma optimize("", off)
bool rzdcxCheckActivation(const fs::path &dllFile)
{
  MinHook minHook;

  minHook.CreateHook(&MessageBoxA, &DetourMessageBoxA);
  minHook.EnableHooks();

  RZDCX rzdcx(dllFile);
  for (int i = 0; i < 200; ++i) {
    try {
      auto obj = rzdcx.CreateInstance<IDCXOBJ>();
      TRACEY_THROW_IF(!obj, "Unable to create RZDCX object");
    }
    catch (int) {
      return false;
    }
  }

  return true;
}

