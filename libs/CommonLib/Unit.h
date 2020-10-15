#pragma once

#include <gsl/gsl_assert>

template <class T, class Tag, double R>
struct Unit
{
  using DataType = T;

  DataType                data{};
  static constexpr double ratio = R;

  explicit constexpr Unit(double x)
    : data(x)
  {
  }

  constexpr Unit(const Unit &x)
    : data(x.data)
  {
  }
  constexpr Unit &operator=(const Unit &x)
  {
    data = x.data;
    return *this;
  }

  double value() const
  {
    return data;
  }
};

//template <class T, class Tag, int R, class U>
//Unit<T, Tag, R> operator*(Unit<T, Tag, R> a, U b)
//{
//  return Unit<T, Tag, R>(a.value() * b);
//}
//
//template <class T, class Tag, int R, class U>
//Unit<T, Tag, R> operator*(U b, Unit<T, Tag, R> a)
//{
//  return Unit<T, Tag, R>(a.value() * b);
//}
//
//template <class T, class Tag, int R, class U>
//Unit<T, Tag, R> operator/(Unit<T, Tag, R> a, U b)
//{
//  Expects(std::abs(b) > 1.e-9);
//  return Unit<T, Tag, R>(a.value() / b);
//}
//
//template <class T, class Tag, int R, int R2>
//double operator/(Unit<T, Tag, R> a, Unit<T, Tag, R2> b)
//{
//  Expects(std::abs(b.value()) > 1.e-9);
//
//  Unit<T, Tag, R> bb(b);
//  return a.value() / bb.value();
//}
//
//template <class T, class Tag, int R, int R2>
//Unit<T, Tag, R> operator+(Unit<T, Tag, R> a, Unit<T, Tag, R2> b)
//{
//  Unit<T, Tag, R> bb(b);
//  return Unit<T, Tag, R>(a.value() + bb.value());
//}
//
//template <class T, class Tag, int R, int R2>
//Unit<T, Tag, R> operator-(Unit<T, Tag, R> a, Unit<T, Tag, R2> b)
//{
//  Unit<T, Tag, R> bb(b);
//  return Unit<T, Tag, R>(a.value() - bb.value());
//}
