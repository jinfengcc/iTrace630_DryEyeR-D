#pragma once


#define USE_JSON_SETTINGS   1

#if USE_JSON_SETTINGS

#include "libs/CommonLib/AppSettings.h"

struct RzdcxSettings
{
  RzdcxSettings()
  {
    Load();
  }

  void Load()
  {
    AppSettings aps(AppSettings(), "dicom.rzdcx");

    // clang-format off
    aps.Get("email"             , &m_email             );
    aps.Get("serialNo"          , &m_serialNo          );
    aps.Get("maxWaitSeconds"    , &m_maxWaitSeconds    );
    aps.Get("minExpectedVersion", &m_minExpectedVersion);
    // clang-format on

    if (m_minExpectedVersion < 3)
      m_minExpectedVersion = 3;
    if (m_maxWaitSeconds < 1)
      m_maxWaitSeconds = 1;
  }

  std::string m_email             {"agatz@traceytech.com"};
  std::string m_serialNo          {"TRACYTECH-TEST-20210630"};
  int         m_maxWaitSeconds    {10};
  int         m_minExpectedVersion{3};
  std::string m_request;
  std::string m_license;

};

#else

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
    if (m_minExpectedVersion < 3)
       m_minExpectedVersion = 3;
    if (m_maxWaitSeconds < 1)
      m_maxWaitSeconds = 1;
  }
  std::string m_email{"agatz@traceytech.com"};
  std::string m_serialNo{"TRACYTECH-TEST-20210630"};
  int         m_maxWaitSeconds{10};
  int         m_minExpectedVersion{3};
  std::string m_request;
  std::string m_license;

  // clang-format off
  BEGIN_REGPROP_MAP(RzdcxSettings)
    REG_PROPERTY(L"RZDCX.email"     , m_email)
    REG_PROPERTY(L"RZDCX.serialNo"  , m_serialNo)
    REG_PROPERTY(L"RZDCX.maxWaitSeconds", m_maxWaitSeconds)
    REG_PROPERTY(L"RZDCX.minExpectedVersion", m_minExpectedVersion)
    REG_PROPERTY(L"RZDCX.request", m_request)
    REG_PROPERTY(L"RZDCX.license", m_license)
  END_REGPROP_MAP()
  // clang-format on
};

#endif
