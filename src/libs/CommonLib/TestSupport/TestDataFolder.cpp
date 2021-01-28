#include "pch.h"
#include "TestDataFolder.h"
#include <iostream>

using std::filesystem::exists;
using std::filesystem::path;

namespace {
  const char inp_env_name[] = "iTraceTestData";
  const char out_env_name[] = "iTraceTestResults";

  path input_test_folder;
  path output_test_folder;

  fs::path GetEnvFolder(const char *env)
  {
    char sz[_MAX_PATH]{};
    auto in = fmt::format("%{}%", env);

    ExpandEnvironmentStringsA(in.c_str(), sz, _MAX_PATH);
    return (in != sz) ? path(sz) : path();
  }

  void ReportErrors(const path &tf);
} // namespace

path GetTestDataFolder(bool reportErrors)
{
  if (input_test_folder.empty()) {
    input_test_folder = GetEnvFolder(inp_env_name);
  }

  if (reportErrors) {
    if (input_test_folder.empty() || !exists(input_test_folder))
      ReportErrors(input_test_folder);
  }

  return exists(input_test_folder) ? input_test_folder : path();
}

void SetTestDataFolder(const std::filesystem::path &p)
{
  input_test_folder = p;
}

path GetTestResultsFolder()
{
  if (output_test_folder.empty()) {
    output_test_folder = GetEnvFolder(out_env_name);
  }

  if (!fs::exists(output_test_folder))
    return {};

  if (output_test_folder == GetTestDataFolder())
    throw std::runtime_error("TestResultFolder == TestDataFolder");

  return output_test_folder;
}

void SetTestResultsFolder(const std::filesystem::path &p)
{
  output_test_folder = p;
}

namespace {
  void ReportErrors(const path &tf)
  {
    if (!tf.empty() && exists(tf))
      return;

    std::cout << "\n"
              << "**************************************************************************\n";

    if (tf.empty())
      std::cout << "Environment variable '" << inp_env_name << "' does not exist";
    else
      std::cout << "Folder pointed by environment variable '" << inp_env_name << "' does not exist";

    std::cout << "\n"
              << "**************************************************************************\n";
  }
} // namespace
