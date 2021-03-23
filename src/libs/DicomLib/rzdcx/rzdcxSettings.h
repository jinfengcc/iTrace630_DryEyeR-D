#pragma once

#include "libs/CommonLib/TraceyRegistry.h"

struct RzdcxSettings : public TraceyRegistryImpl<RzdcxSettings>
{
  RzdcxSettings()
    : TraceyRegistryImpl<RzdcxSettings>(L"License")
  {
    Load();
  }

  void Load()
  {
    TraceyRegistryImpl<RzdcxSettings>::Load();
  }
  std::string m_request;
  std::string m_license;

  BEGIN_REGPROP_MAP(RzdcxSettings)
    REG_PROPERTY(L"RZDCX.request", m_request)
    REG_PROPERTY(L"RZDCX.license", m_license)
  END_REGPROP_MAP()
};
