#include "pch.h"
#include "rzdcxActivate.h"
#include "rzdcxSettings.h"
#include "rzdcxCheckActivation.h"
#include "libs/CommonLib/VersionInfo.h"
#include "../CommonLib/RunConsoleApp.h"
#include "rzdcx.h"

namespace {
  const auto MIN_EXPECTED_VERSION = 3;
  const auto MAX_SERVER_WAIT      = 10; // seconds
  const auto EMAIL                = "agatz@traceytech.com";
  const auto SERNO                = "TRACYTECH-TEST-20210630";
  const auto TEST_REQUEST         = "   3         604bd33a             EDC6F84DD34B992AE291 t";

  void        Activate(const fs::path &dll);
  std::string GetLicenseRequestString(const fs::path &dll);
  std::string GetLicenseString(const fs::path &dll);
  bool        SaveLicense(const fs::path &dll, const char *license);

} // namespace

void rzdcxActivate()
{
  auto dll = rzdcxGetDllName();

  if (!rzdcxCheckActivation(dll)) {
    Activate(dll);
  }
}

bool rzdcxIsActivated()
{
  return rzdcxCheckActivation(rzdcxGetDllName());
}

fs::path rzdcxGetDllName()
{
  auto dll = GetExeFileFolder() / "RZDCX.dll";

  VersionInfo vi(dll);
  if (auto i = vi.get(); HIWORD(i->dwFileVersionMS) < MIN_EXPECTED_VERSION) {
    auto v0 = HIWORD(i->dwFileVersionMS);
    auto v1 = LOWORD(i->dwFileVersionMS);
    auto v2 = HIWORD(i->dwFileVersionLS);
    auto v3 = LOWORD(i->dwFileVersionLS);

    TRACEY_THROW("Wrong RZDCX.DLL Version {}.{}.{}.{} (Expected {}.0.0.0 or higher)", v0, v1, v2, v3, MIN_EXPECTED_VERSION);
  }

  return dll;
}

std::string rzdcxGetLicenseRequest()
{
  return GetLicenseRequestString(rzdcxGetDllName());
}

std::string rzdcxGetLicense()
{
  return GetLicenseString(rzdcxGetDllName());
}

bool rzdcxSetLicense(const char *license)
{
  return SaveLicense(rzdcxGetDllName(), license);
}

namespace {

  std::string GetRZDXActivationProgram()
  {
    auto prog = "TraceyRzdcxActivation.exe";
    auto exe  = GetExeFileFolder() / prog;

    TRACEY_THROW_IF(!fs::exists(exe), "Missing RZDCX Activation software '{}'", prog);

    return exe.string();
  }

  fs::path GetLicenseRequestFile(const fs::path &dll)
  {
    return dll.parent_path() / "hrzsdk_license_request.txt";
  }

  fs::path GetLicenseFile(const fs::path &dll)
  {
    return dll.parent_path() / "HRZSDK.license";
  }

  std::string GetFileString( const fs::path &txt)
  {
    std::string str;
    if (fs::exists(txt))
    {
      std::ifstream ss(txt);
      while (std::getline(ss, str)) {
        while (!str.empty() && str[str.size() - 1] == '\n' || str[str.size() - 1] == '\r')
          str.resize(str.size() - 1);

        if (auto n = str.find_first_not_of(' '); n != std::string::npos)
          str = str.substr(n);

        if (!str.empty())
          break;
      }
    }

    return str;
  }

  std::string GetLicenseRequestString(const fs::path &dll)
  {
    auto requestFile = GetLicenseRequestFile(dll);
    if (!fs::exists(requestFile)) {
      // Run once to create it
      RZDCX rzdcx(dll);
      auto  obj = rzdcx.CreateInstance<IDCXOBJ>();
    }

    return GetFileString(requestFile);
  }

  std::string RetrieveLicense(const fs::path &dll);

  std::string GetLicenseString(const fs::path &dll)
  {
    try
    {
      auto str = GetFileString(GetLicenseFile(dll));
      if (str.empty())
        str = RetrieveLicense(dll);
      return str;
    }
    catch (const std::exception &e)
    {
      return "";
    }
  }

  bool SaveLicense(const fs::path &dll, const char *license)
  {
    auto licenseFile = GetLicenseFile(dll);

    std::ofstream(licenseFile) << license;

    if (rzdcxCheckActivation(dll)) {
      return true;
    }
    else {
      fs::remove(licenseFile);
      fs::remove(GetLicenseRequestFile(dll));
      return false;
    }
  }

  std::string RetrieveLicense(const fs::path &dll)
  {
    auto exe_path = GetRZDXActivationProgram();
    auto request  = GetLicenseRequestString(dll);
    auto cmd      = fmt::format(R"("{}" {} {} "{}")", exe_path, EMAIL, SERNO, request);

    LOG_Info("RZDCX (Request ): {}", request);

    auto runner  = RunConsoleApp();
    auto license = runner.RunCmdAndGetOutput(cmd.c_str(), MAX_SERVER_WAIT * 1'000);

    LOG_Info("RZDCX (Response): {}", license);

    TRACEY_THROW_IF(license.empty(), "Unable to obtain RZDCX license. Make sure you are connected to the internet and try again");

    const char error[] = "Error:";
    if (license.starts_with(error))
      TRACEY_THROW(&license[strlen(error)]);

    const char success[] = "Success:";
    if (!license.starts_with(success))
      TRACEY_THROW("Unexpected license '{}'", license);

    return license.substr(strlen(success));
  }


  void Activate(const fs::path &dll)
  {
    auto payload = RetrieveLicense(dll);
    if (!SaveLicense(dll, payload.c_str()))
      TRACEY_THROW("Unable to obtain RZDCX license (Unknown Error)");
  }
} // namespace
