#pragma once

#include <chrono>
#include <functional>
#include <atomic>

class ThreadPoolTimer final
{
  ThreadPoolTimer(const ThreadPoolTimer &) = delete;
  ThreadPoolTimer &operator=(const ThreadPoolTimer &) = delete;

public:
  ThreadPoolTimer();

  template<class Func>
  ThreadPoolTimer(Func &&f)
    : ThreadPoolTimer()
  {
    SetFunc<FUNC>(f);
  }

  ThreadPoolTimer(ThreadPoolTimer &&other) noexcept;
  ~ThreadPoolTimer();

  void Abort();

  template <class Func>
  void SetFunc(Func &&f)
  {
    m_func = std::move(f);
  }

  template <class TP, class P>
  void Schedule(TP when, P period)
  {
    auto file_time = When2Filetime<TP>(when);
    auto period_ms = std::chrono::milliseconds(period).count();

    SetThreadpoolTimer(m_timer, &file_time, static_cast<DWORD>(period_ms), 10);
  }

  template <class D, class R>
  void Schedule(std::chrono::duration<D, R> period)
  {
    auto when = std::chrono::system_clock::now() + period;
    Schedule(when, period);
  }

  template <class TP>
  void ScheduleOnce(TP when)
  {
    auto file_time = When2Filetime<TP>(when);
    SetThreadpoolTimer(m_timer, &file_time, 0, 10);
  }

private:
  using FUNC = std::function<void(void)>;

  TP_CALLBACK_ENVIRON *m_callbackEnv{};
  TP_TIMER *           m_timer{};
  FUNC                 m_func;
  std::atomic_bool     m_guard{false};  // guard against recursive running (when m_func takes too long)

  static void CALLBACK timer_callback(PTP_CALLBACK_INSTANCE /*Instance*/, PVOID parameter, PTP_TIMER /*timer*/)
  {
    auto pThis = static_cast<ThreadPoolTimer *>(parameter);
    if (!pThis->m_guard.exchange(true)) {
      if (pThis->m_func)
        pThis->m_func();
      pThis->m_guard = false;
    }
  }

  template <class TP>
  FILETIME When2Filetime(TP when)
  {
    constexpr bool is_tp = std::is_same_v<TP, std::chrono::system_clock::time_point>;
    if constexpr (is_tp)
      return time_point_to_FILETIME(when);
    else
      return time_point_to_FILETIME(std::chrono::system_clock::now() + when);
  }

  static FILETIME time_point_to_FILETIME(const std::chrono::system_clock::time_point &when);
};
