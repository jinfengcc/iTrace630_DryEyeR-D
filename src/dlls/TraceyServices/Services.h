#pragma once

#include <interfaces/IServices.h>
#include <libs/CommonLib/Implements.h>

class Services : public Implements<IServices>
{
public:
  Services()
    : Implements<IServices>("IServices")
  {
  }

  bool CheckPassword(const char *expected, HWND parent) override;
};
