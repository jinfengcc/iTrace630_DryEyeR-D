#include "pch.h"
#include "ThreadPool.h"

#include <atlbase.h>

struct ThreadPool::Impl
{
  TP_CALLBACK_ENVIRON m_callBackEnv;
  TP_POOL *           m_pool{};

  Impl()
  {
    m_pool = CreateThreadpool(nullptr);
    InitializeThreadpoolEnvironment(&m_callBackEnv);
    SetThreadpoolCallbackPool(&m_callBackEnv, m_pool);
    SetThreadpoolCallbackLibrary(&m_callBackEnv, GetCurrentModule());
  }

  ~Impl()
  {
    CloseThreadpool(m_pool);
    DestroyThreadpoolEnvironment(&m_callBackEnv);
  }

  static HMODULE GetCurrentModule()
  {
    DWORD   flags = GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS;
    HMODULE hm    = nullptr;
    GetModuleHandleEx(flags, reinterpret_cast<LPCTSTR>(&ThreadPool::Impl::GetCurrentModule), &hm);
    return hm;
  }
};

ThreadPool::ThreadPool(bool privatePool)
{
  if (privatePool)
    m_pimpl = std::make_unique<Impl>();

  m_work = CreateThreadpoolWork(callback, &m_queue, m_pimpl ? &m_pimpl->m_callBackEnv : nullptr);
}

ThreadPool::ThreadPool(unsigned minThreads, unsigned maxThreads)
  : ThreadPool(true)
{
  SetThreadpoolThreadMaximum(m_pimpl->m_pool, maxThreads);
  SetThreadpoolThreadMinimum(m_pimpl->m_pool, minThreads);
}

ThreadPool::~ThreadPool()
{
  CloseThreadpoolWork(m_work);
}

