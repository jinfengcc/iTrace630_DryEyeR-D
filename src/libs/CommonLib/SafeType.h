#pragma once

template <typename T, typename Tag>
class SafeType
{
public:
  using Type = T;

  template <class U>
  explicit SafeType(U const &value)
    : value_(static_cast<T>(value))
  {
  }

  explicit SafeType(T &&value)
    : value_(std::move(value))
  {
  }

  // clang-format off
  T       &get()        { return value_; }
  T        get() const  { return value_; }
  operator T  () const  { return value_; }
  // clang-format on

private:
  T value_{};
};
