#pragma once

#include <deque>
#include <mutex>
#include <condition_variable>

template<class T>
class notification_queue {
  std::deque<T> _q;
  bool                    _done{false};
  std::mutex                   _mutex;
  std::condition_variable      _ready;

public:
  bool try_pop(T &x)
  {
    std::lock_t lock{_mutex, try_to_lock};
    if (!lock || _q.empty())
      return false;
    x = move(_q.front());
    _q.pop_front();
    return true;
  }

  bool try_push(T &&f)
  {
    {
      std::lock_t lock{_mutex, try_to_lock};
      if (!lock)
        return false;
      _q.emplace_back(forward<F>(f));
    }
    _ready.notify_one();
    return true;
  }

  void done()
  {
    {
      std::unique_lock<mutex> lock{_mutex};
      _done = true;
    }
    _ready.notify_all();
  }

  bool pop(T &x)
  {
    std::lock_t lock{_mutex};
    while (_q.empty() && !_done)
      _ready.wait(lock);
    if (_q.empty())
      return false;
    x = move(_q.front());
    _q.pop_front();
    return true;
  }

  void push(T &&f)
  {
    {
      lock_t lock{_mutex};
      _q.emplace_back(forward<F>(f));
    }
    _ready.notify_one();
  }
};
