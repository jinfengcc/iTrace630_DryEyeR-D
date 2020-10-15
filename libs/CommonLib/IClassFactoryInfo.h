#pragma once

#include <vector>

struct __declspec(uuid("F3D5F956-1D03-4278-AFE7-41AB05CA7791")) IClassFactoryInfo : public IUnknown
{
  virtual std::vector<GUID> Implements() const = 0;
};
