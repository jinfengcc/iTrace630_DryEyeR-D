//***************************************************************************************
#include "pch.h"
#include "windows.h"
#include "RunConsoleApp.h"

//***************************************************************************************

RunConsoleApp::RunConsoleApp()
{
  SECURITY_ATTRIBUTES sa{.nLength = sizeof(SECURITY_ATTRIBUTES), .bInheritHandle = TRUE};
  CreatePipe(&m_hReadPipe, &m_hWritePipe, &sa, 0);
}

RunConsoleApp::~RunConsoleApp()
{
  if (m_hReadPipe)
    CloseHandle(m_hReadPipe);

  if (m_hWritePipe)
    CloseHandle(m_hWritePipe);
}

std::string RunConsoleApp::RunCmdAndGetOutput(const char *cmd, DWORD maxWait)
{
  return RunAndWait(cmd, maxWait) ? GetProgramOutput() : std::string();
}

bool RunConsoleApp::RunAndWait(const char *cmd, DWORD maxWait)
{
  STARTUPINFOA si{.cb          = sizeof(STARTUPINFO),
                  .dwFlags     = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES,
                  .wShowWindow = SW_HIDE,
                  .hStdOutput  = m_hWritePipe,
                  .hStdError   = m_hWritePipe};

  SECURITY_ATTRIBUTES sa{.nLength = sizeof(SECURITY_ATTRIBUTES), .bInheritHandle = TRUE};

  PROCESS_INFORMATION m_processInfo{};
  if (!CreateProcessA(nullptr, (char *)cmd, &sa, &sa, TRUE, 0, nullptr, nullptr, &si, &m_processInfo))
    return false;

  auto gotid = WaitForSingleObject(m_processInfo.hProcess, maxWait) == WAIT_OBJECT_0;

  CloseHandle(m_processInfo.hThread);
  CloseHandle(m_processInfo.hProcess);

  return gotid;
}

std::string RunConsoleApp::GetProgramOutput() const
{
  std::string str;
  if (auto fileSize = GetFileSize(m_hReadPipe, nullptr)) {
    str.resize(fileSize);

    unsigned long count{};
    if (!ReadFile(m_hReadPipe, &str[0], str.size(), &count, nullptr))
      str.clear();
  }

  return str;
}

#ifdef _DEBUG
void RunConsoleAppTest()
{
  auto p = RunConsoleApp();
  auto s = p.RunCmdAndGetOutput("IpConfig.exe");
}
#endif // _DEBUG
