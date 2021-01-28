#pragma once

// #include <concrt.h>
// #include <agents.h>
#include <mutex>
#include <list>
#include <queue>
#include <atlbase.h>
#include "Signal.h"
#include <atomic>

namespace sig {
  namespace details {
    class SignalUiBase
    {
    public:
      virtual void OnHiddenWndMessage() = 0;

    protected:
      SignalUiBase();
      static void Post(SignalUiBase *obj);
    };
  } // namespace details

  bool UiThread();

  template <typename... Args>
  class SignalUI
    : public Signal<Args...>
    , private details::SignalUiBase
  {
  public:
    SignalUI(bool alwaysPostInHiddenWindow = true)
      : m_alwaysPostInHiddenWindow(alwaysPostInHiddenWindow)
    {
    }

    void operator()(Args... args)
    {
      if (m_alwaysPostInHiddenWindow || !UiThread()) {
        if (auto s = this->GetFuncs(); !s.empty()) {
          for (auto &f : s)
            Push(std::move(f), std::forward<Args>(args)...);
          Post(this);
        }
      }
      else {
        Signal<Args...>::execute(std::move(args)...);
      }
    }

    void Disconnect(SignalId id)
    {
      Signal<Args...>::Disconnect(id);
      RemoveFromQueue(id);
      WaitForInFlight(id);
    }

    void DisconnectAll(yes_i_am_sure y)
    {
      Signal<Args...>::DisconnectAll(y);
      RemoveFromQueue(0);
      WaitForInFlight(0);
    }

  private:
    using Data     = Signal<Args...>::Data;
    using Func     = std::function<void(Args...)>;
    using FuncData = std::pair<Data, std::tuple<Args...>>;

    std::mutex            m_mutex;
    std::deque<FuncData>  m_queueCont;
    std::queue<FuncData>  m_queue{m_queueCont};
    bool                  m_alwaysPostInHiddenWindow;
    std::atomic<SignalId> m_inFlight;

    void Push(Data func, const Args &... args)
    {
      std::lock_guard lock(m_mutex);
      m_queue.push(std::make_pair(func, std::make_tuple(args...)));
    }

    bool Pop(FuncData &fd)
    {
      std::lock_guard lock(m_mutex);
      if (m_queue.empty()) {
        m_inFlight = 0;
        return false;
      }

      fd         = m_queue.front();
      m_inFlight = fd.first.id;
      m_queue.pop();
      return true;
    }

    void RemoveFromQueue(SignalId id)
    {
      std::lock_guard lock(m_mutex);
      for (auto &i : m_queueCont) {
        if (i.first.id == id || id == 0)
          i.first.func = nullptr;
      }
    }

    void WaitForInFlight(SignalId id)
    {
      if (id == 0) {
        for (int i = 0; i < 10 && m_inFlight != 0; ++i)
          Sleep(50);
      }
      else {
        for (int i = 0; i < 10 && m_inFlight == id; ++i)
          Sleep(50);
      }
    }

    void OnHiddenWndMessage() override
    {
      FuncData fd;
      while (Pop(fd)) {
        if (fd.first.func && !wil::ProcessShutdownInProgress())
          std::apply(fd.first.func, fd.second);

        m_inFlight = 0;
      }
    }
  };
} // namespace sig

using sig::SignalUI;
