#pragma once

#include "TraceyObj.h"

TRACEYOBJ("d023ab1e-07a6-4e8f-b283-da0eacbcb312") IServices : public IUnknown
{
  virtual bool CheckPassword(const char *expected = nullptr, HWND parent = nullptr) = 0;
};
