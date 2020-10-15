#pragma once
#include "Unit.h"

template <int R>
struct Dimension : public Unit<double, enum class DimTag, R>
{
  static constexpr int UNITS = R;

  using Base = Unit<double, enum class DimTag, R>;

  explicit constexpr Dimension(double x)
    : Base(x)
  {
  }

  constexpr Dimension(const Dimension &x)
    : Base(x.data)
  {
  }
  // constexpr Dimension &operator=(const Dimension &x)
  //{
  //  data = x.data;
  //  return *this;
  //}

  template <int U>
  constexpr Dimension(Dimension<U> x)
    : Dimension(x.value() * (x.ratio / this->ratio))
  {
  }

  // template <int U>
  // constexpr Dimension &operator=(const Dimension<U> &x)
  //{
  //  this->data = x.value() * x.ratio / this->ratio;
  //  return *this;
  //}
};

using Microns     = Dimension<1>;
using Millimeters = Dimension<1'000>;
using Centimeters = Dimension<100'000>;
using Meters      = Dimension<1'000'000>;

inline Microns operator""_um(long double x)
{
  return Microns(x);
}

inline Millimeters operator""_mm(long double x)
{
  return Millimeters(x);
}

// Multiplication
template <int R, class U>
Dimension<R> operator*(Dimension<R> a, U b)
{
  return Dimension<R>(a.value() * b);
}

template <int R, class U>
Dimension<R> operator*(U b, Dimension<R> a)
{
  return Dimension<R>(a.value() * b);
}

// Division
template <int R, class U>
Dimension<R> operator/(Dimension<R> a, U b)
{
  Expects(std::abs(b) > 1.e-9);
  return Dimension<R>(a.value() / b);
}

template <int R, int U>
double operator/(Dimension<R> a, Dimension<U> b)
{
  Expects(std::abs(b.value()) > 1.e-9);

  Dimension<R> bb(b);
  return a.value() / bb.value();
}

// Addition
template <int R, class U>
Dimension<R> operator+(Dimension<R> a, U b)
{
  return Dimension<R>(a.value() + b);
}

template <int R, class U>
Dimension<R> operator+(U b, Dimension<R> a)
{
  return a + b;
}

template <int R, int U>
Dimension<R> operator+(Dimension<R> a, Dimension<U> b)
{
  return a + Dimension<R>(b).value();
}

// Substraction
template <int R, int U>
Dimension<R> operator-(Dimension<R> a, Dimension<U> b)
{
  Dimension<R> bb(b);
  return Dimension<R>(a.value() - bb.value());
}
