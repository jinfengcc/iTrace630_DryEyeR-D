#pragma once

#include "Signal.h"

  // template <typename... Args>
// class Slot
//{
// public:
//  using Signal = Signal<Args...>;

//  Slot(const Signal &e) = delete;

//  template <typename Func>
//  Slot(Signal &e, Func func)
//    : m_signal(e)
//  {
//    Connect(func);
//  }

//  ~Slot()
//  {
//    Disconnect();
//  }

//  template <typename Func>
//  void Connect(Func func)
//  {
//    Disconnect();
//    m_id = m_signal.connect(func);
//  }
//  void Disconnect()
//  {
//    SignalId nullId{};
//    if (m_id != nullId) {
//      m_signal.disconnect(m_id);
//      m_id = nullId;
//    }
//  }

// private:
//  Signal & m_signal;
//  SignalId m_id{};
//};
