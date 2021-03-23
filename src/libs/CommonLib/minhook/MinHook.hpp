#pragma once

#include "MinHook.h"

class MinHook
{
public:
  MinHook()
  {
    m_status = MH_Initialize();
  }

  ~MinHook()
  {
    if (m_status == MH_OK)
      MH_Uninitialize();
  }

  MinHook(const MinHook &) = delete;
  MinHook &operator = (const MinHook &) = delete;

  template<class T>
  bool CreateHook( T *target, T *replacement, T **original = nullptr)
  {
    return MH_CreateHook(target, replacement, reinterpret_cast<LPVOID *>(original)) == MH_OK;
  }

  template <class T>
  bool EnableHook(T *target, bool enable = true)
  {
    return (enable ? MH_EnableHook(target) : MH_DisableHook(target)) == MH_OK;
  }

  bool EnableHooks(bool enable = true)
  {
    return (enable ? MH_EnableHook(MH_ALL_HOOKS) : MH_DisableHook(MH_ALL_HOOKS)) == MH_OK;
  }

private:
  MH_STATUS m_status;
};
