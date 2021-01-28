#pragma once
#include <stdexcept>

/********************
@brief All the calculator object functions return the results
in a 'Result' container, that returns either the value or an error string.

Usage pattern 1:
  Result<X> x = obj->CalculateX(...);
  if (x) {
     // Use x.value()
     auto a = x.value()
  }

Usage pattern 2:
  Result<X> x = obj->CalculateX(...);
  try {
     // Use x.value()
     auto a = x.value()
  }
  catch( const std::exception &e) {
     // handle exception
  }
*************************************************/

template <class T>
class [[nodiscard]] Result
{
public:
  Result(const T &t)
    : m_value(t)
  {
  }
  Result(const char *err, bool)
    : m_error(err)
  {
  }
  explicit operator bool() const
  {
    return m_error.empty();
  }
  const T &value() const
  {
    return *get();
  }
  const T *operator->() const
  {
    return get();
  }
  const char *error() const
  {
    return m_error.c_str();
  }

private:
  T           m_value{};
  std::string m_error;

  const T *get() const
  {
    if (!m_error.empty())
      throw std::runtime_error(m_error);
    return &m_value;
  }
};
