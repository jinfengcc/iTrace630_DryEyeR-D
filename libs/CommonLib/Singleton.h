#pragma once
// Copied from: https://vorbrodt.blog/2020/07/10/singleton-pattern/
// renamed abstract_singleton to Singleton

#include <mutex>
#include <memory>
#include <utility>
#include <stdexcept>
#include <gsl/gsl>

/*
Usage:
class X : public Singleton<X> {
  friend class Singleton<X>; // required only if X has a private/protected constructor
};

*/

template <typename T>
class Singleton
{
  using storage_t                    = std::unique_ptr<T>;
  inline static storage_t m_instance = nullptr;

protected:
  Singleton()          = default;
  virtual ~Singleton() = default;

  Singleton(const Singleton &) = delete;
  Singleton(Singleton &&)      = delete;
  Singleton &operator=(const Singleton &) = delete;
  Singleton &operator=(Singleton &&) = delete;

public:
  template <typename... Args>
  static T *Create(Args &&... args)
  {
    static std::mutex s_lock;
    std::scoped_lock  lock(s_lock);
    if (m_instance)
      throw std::logic_error("This abstract singleton has already been created!");

    m_instance.reset(new T(std::forward<Args>(args)...));
    return m_instance.get();
  }

  static T *Instance() noexcept
  {
    if constexpr (std::is_default_constructible_v<T>) {
      return m_instance ? m_instance.get() : Create();
    }
    else {
      Expects(m_instance);
      return m_instance.get();
    }
  }
};
