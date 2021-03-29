#pragma once

#include <windows.h>

class SingleInstance
{
protected:
  DWORD        m_dwLastError;
  HANDLE       m_hMutex;
  const TCHAR *m_pszMutexName;

public:
  SingleInstance(const TCHAR *strMutexName);
  ~SingleInstance();

  BOOL IsAnotherInstanceRunning()
  {
    return (ERROR_ALREADY_EXISTS == m_dwLastError);
  }
};
