#include "pch.h"
#include "ThreadPool.h"
#include "ThreadPoolTimer.h"

#include <atlbase.h>

ThreadPoolTimer::ThreadPoolTimer()
{
  m_timer = CreateThreadpoolTimer(&timer_callback, this, m_callbackEnv);
  if (m_timer == nullptr)
    throw std::bad_alloc();
}

ThreadPoolTimer::ThreadPoolTimer(ThreadPoolTimer &&other) noexcept
{
  m_timer       = other.m_timer;
  m_callbackEnv = other.m_callbackEnv;
  m_func        = std::move(other.m_func);

  other.m_callbackEnv = {};
  other.m_timer       = {};
}

ThreadPoolTimer::~ThreadPoolTimer()
{
  Abort();
}

void ThreadPoolTimer::Abort()
{
  if (m_timer != nullptr) {
    SetThreadpoolTimer(m_timer, nullptr, 0, 0);
    CloseThreadpoolTimer(m_timer);
  }
}

FILETIME ThreadPoolTimer::time_point_to_FILETIME(const std::chrono::system_clock::time_point &when)
{
  FILETIME   ft = {0, 0};
  SYSTEMTIME st = {0};
  time_t     t  = std::chrono::system_clock::to_time_t(when);
  tm         utc_tm;
  if (!gmtime_s(&utc_tm, &t)) {
    st.wSecond       = static_cast<WORD>(utc_tm.tm_sec);
    st.wMinute       = static_cast<WORD>(utc_tm.tm_min);
    st.wHour         = static_cast<WORD>(utc_tm.tm_hour);
    st.wDay          = static_cast<WORD>(utc_tm.tm_mday);
    st.wMonth        = static_cast<WORD>(utc_tm.tm_mon + 1);
    st.wYear         = static_cast<WORD>(utc_tm.tm_year + 1900);
    st.wMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(when.time_since_epoch()).count() % 1000;
    SystemTimeToFileTime(&st, &ft);
  }
  return ft;
}
