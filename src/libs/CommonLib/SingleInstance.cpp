#include "pch.h"
#include "SingleInstance.h"

// this code is from Q243953 in case you lose the article and wonder
// where this code came from...
SingleInstance::SingleInstance(const TCHAR *strMutexName)
  : m_pszMutexName(strMutexName)
{
  // be sure to use a name that is unique for this application otherwise
  // two apps may think they are the same if they are using same name for
  // 3rd parm to CreateMutex
  m_hMutex      = CreateMutex(nullptr, FALSE, strMutexName); // do early
  m_dwLastError = GetLastError();                         // save for use later...
}

SingleInstance::~SingleInstance()
{
  m_dwLastError = 0;
  if (m_hMutex) // don't forget to close handles...
  {
    CloseHandle(m_hMutex); // do as late as possible
    m_hMutex = nullptr;       // good habit to be in
  }
}
