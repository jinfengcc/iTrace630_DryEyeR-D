#pragma once

#include <string>

class RunConsoleApp
{
  HANDLE m_hReadPipe{};
  HANDLE m_hWritePipe{};

public:
  RunConsoleApp();
  ~RunConsoleApp();

  explicit operator bool() const
  {
    return m_hReadPipe != nullptr;
  }

  std::string RunCmdAndGetOutput(const char *cmd, DWORD maxWait = 5000);
  bool        RunAndWait(const char *cmd, DWORD maxWait);
  std::string GetProgramOutput() const;
};
