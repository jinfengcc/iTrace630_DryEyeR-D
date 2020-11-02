#pragma once

#include <string_view>
#include "TraceyObj.h"

TRACEYOBJ("8bd0252a-f453-47c2-8139-15016deb5aef") ISettingsStorage : public IUnknown
{
  virtual const char *GetSysFile() const = 0;
  virtual const char *GetUserFile() const = 0;
};

