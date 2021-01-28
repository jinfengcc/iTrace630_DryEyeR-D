#pragma once

#include <windows.h>
#include <chrono>
#include <stdexcept>
#include <memory>
#include <concurrent_queue.h>
#include <functional>

class ThreadPool final
{
  ThreadPool(const ThreadPool &) = delete;
  ThreadPool &operator=(const ThreadPool &) = delete;

public:
  ThreadPool(bool privatePool = false);
  ThreadPool(unsigned minThreads, unsigned maxThreads);

  ~ThreadPool();

  static ThreadPool &Instance()
  {
    static ThreadPool tp;
    return tp;
  }

  template <typename F>
  void Submit(F &&f)
  {
    m_queue.push(std::move(f));
    SubmitThreadpoolWork(m_work);
  }

  void WaitForWorkersToFinish(bool terminateNotStartedWorkers)
  {
    BOOL b = terminateNotStartedWorkers ? TRUE : FALSE;
    WaitForThreadpoolWorkCallbacks(m_work, b);
  }

private:
  using Func  = std::function<void(void)>;
  using Queue = Concurrency::concurrent_queue<Func>;
  struct Impl;

  std::unique_ptr<Impl> m_pimpl;
  TP_WORK *             m_work{};
  Queue                 m_queue;

  static void CALLBACK callback(PTP_CALLBACK_INSTANCE /*instance*/, PVOID parameter, PTP_WORK /*Work*/)
  {
    if (auto queue = static_cast<Queue *>(parameter)) {
      Func func;
      if (queue->try_pop(func))
        func();
    }
  }
};
