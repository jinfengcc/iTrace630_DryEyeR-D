#pragma once

#include <filesystem>

void                  rzdcxActivate();
bool                  rzdcxIsActivated();
std::filesystem::path rzdcxGetDllName();
std::string           rzdcxGetLicenseRequest();
std::string           rzdcxGetLicense();
bool                  rzdcxSetLicense(const char *license);
