#pragma once

#include <vector>
#include <list>
#include <tuple>
#include <utility>
#include <functional>
#include <type_traits>
#include <cassert>
#include <mutex>
#include <algorithm>
#include <wil/result_macros.h>

namespace wil {
  inline void exe_terminating()
  {
    if (!details::g_processShutdownInProgress) {
      details::g_processShutdownInProgress = true;
    }
  }
} // namespace wil

namespace sig {
  enum class yes_i_am_sure {};
  using SignalId = long;

  struct Slot
  {
    template <class Func>
    Slot(Func &&func)
      : m_cleanFunc(func)
    {
    }
    ~Slot()
    {
      m_cleanFunc();
    }

  private:
    std::function<void()> m_cleanFunc;
  };

  template <typename... Args>
  class Signal
  {
  public:
    using Func = std::function<void(Args...)>;

    bool Enable(bool enable)
    {
      std::swap(enable, m_bEnable);
      return enable;
    }

    void operator()(Args... args)
    {
      execute(std::move(args)...);
    }
    void execute(Args... args)
    {
      auto s = this->GetFuncs();
      for (auto &f : s) {
        if (!wil::ProcessShutdownInProgress())
          f.func(args...);
      }
    }

    [[nodiscard]] Slot ConnectEx(Func f)
    {
      auto id = Connect(f);
      return Slot([id, this] { Disconnect(id); });
    }

    SignalId Connect(Func f);
    void     Disconnect(SignalId id);
    void     DisconnectAll(yes_i_am_sure);

  protected:
    struct Data
    {
      Func     func;
      SignalId id{};
    };

    using DataList = std::vector<Data>;
    using mutex_t  = std::mutex;
    using lock_t   = std::lock_guard<mutex_t>;

    mutex_t  m_mutex;
    DataList m_callbacks;

    DataList GetFuncs()
    {
      lock_t lock(m_mutex);
      return m_bEnable ? m_callbacks : DataList{};
    }

  private:
    enum { m_idHiWord = 0xCD10000 };
    bool m_bEnable = true;

    static bool ValidId(SignalId id)
    {
      return (id & 0xFFFF0000) == m_idHiWord;
    }
    static SignalId GetNextId()
    {
      static SignalId id = m_idHiWord;
      return id++;
    }
  };

  template <typename... Args>
  SignalId Signal<Args...>::Connect(Func f)
  {
    std::lock_guard<decltype(m_mutex)> lock(m_mutex);
    m_callbacks.push_back(Data{f, GetNextId()});
    return m_callbacks.back().id;
  }
  template <typename... Args>
  void Signal<Args...>::Disconnect(SignalId id)
  {
    if (id) {
      assert(ValidId(id));
      std::lock_guard<decltype(m_mutex)> lock(m_mutex);
      auto i = std::remove_if(m_callbacks.begin(), m_callbacks.end(), [id](const Data &d) { return d.id == id; });

      m_callbacks.erase(i, m_callbacks.end());
    }
  }
  template <typename... Args>
  void Signal<Args...>::DisconnectAll(yes_i_am_sure)
  {
    std::lock_guard<decltype(m_mutex)> lock(m_mutex);
    m_callbacks.clear();
  }

  template <class Func, typename... Args>
  SignalId Connect(Signal<Args...> &s, Func f)
  {
    return s.Connect(f);
  }
  template <class Func, typename... Args>
  [[nodiscard]] auto ConnectEx(Signal<Args...> &s, Func f)
  {
    // clang-format off
    struct Pair  {
      Signal<Args...> *s;
      SignalId         id;
      Pair(Signal<Args...> *s, SignalId id) : s(s), id(id) {}
      ~Pair() { s->Disconnect(id); }
    };
    // clang-format on
    return std::make_unique<Pair>(&s, s.Connect(f));
  }
  template <typename... Args>
  void Disconnect(Signal<Args...> &s, SignalId id)
  {
    s.Disconnect(id);
  }
  template <typename... Args>
  void DisconnectAll(Signal<Args...> &s, yes_i_am_sure yims)
  {
    s.DisconnectAll(yims);
  }

} // namespace sig

using sig::Connect;
using sig::ConnectEx;
using sig::Disconnect;
using sig::DisconnectAll;
using sig::Signal;
