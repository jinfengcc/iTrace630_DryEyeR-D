#pragma once

// Inspired from: https://vorbrodt.blog/2020/07/10/singleton-pattern/
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
  inline static std::unique_ptr<T> m_instance = nullptr;

  static void ThrowIf(bool b, const char *txt)
  {
    if (b)
      throw std::logic_error(txt);
  }

  static constexpr bool has_default_constructor()
  {
    // We need to derive from Q to check for default constructor
    // std::is_default_constructible_v<T> returns false,
    // even if Singleton<T> is friend of T
    struct Q : public T
    {
      using T::T;
    };
    return std::is_default_constructible_v<Q>;
  }

protected:
  Singleton()          = default;
  virtual ~Singleton() = default;

  Singleton(const Singleton &) = delete;
  Singleton(Singleton &&)      = delete;
  Singleton &operator=(const Singleton &) = delete;
  Singleton &operator=(Singleton &&) = delete;

public:
  template <typename... Args>
  static T *CreateInstance(Args &&... args)
  {
    static std::mutex s_lock;
    std::scoped_lock  lock(s_lock);

    ThrowIf(m_instance.get() != nullptr, "This abstract singleton has already been created!");
    m_instance.reset(new T(std::forward<Args>(args)...));
    return m_instance.get();
  }

  static T *Instance()
  {
    if constexpr (has_default_constructor()) {
      return m_instance ? m_instance.get() : CreateInstance();
    }
    else {
      ThrowIf(m_instance.get() == nullptr, "This abstract singleton has not been created!");
      return m_instance.get();
    }
  }
};
