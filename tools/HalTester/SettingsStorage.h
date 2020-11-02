#pragma once

#include <interfaces/ISettingsStorage.h>
#include <libs/CommonLib/Implements.h>

class SettingsStorage : public ImplementsStatic<ISettingsStorage>
{
public:

  SettingsStorage();

private:
  std::string m_sysFile;

  const char *GetSysFile() const override
  {
    return m_sysFile.c_str();
  }
  const char *GetUserFile() const override
  {
    return nullptr;
  }
};


