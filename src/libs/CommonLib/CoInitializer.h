#pragma once

#include <atlbase.h>

struct CoInitializer
{
  CoInitializer()
  {
    CoInitialize(nullptr);
  }
  ~CoInitializer()
  {
    CoUninitialize();
  }
};
