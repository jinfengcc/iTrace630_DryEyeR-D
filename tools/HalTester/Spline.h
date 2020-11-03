#pragma once

/*
 * spline.h
 *
 * simple cubic spline interpolation library without external
 * dependencies
 *
 * ---------------------------------------------------------------------
 * Copyright (C) 2011, 2014 Tino Kluge (ttk448 at gmail.com)
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * ---------------------------------------------------------------------
 *
 */

#ifndef TK_SPLINE_H
#define TK_SPLINE_H

#include <cstdio>
#include <cassert>
#include <vector>
#include <algorithm>

// unnamed namespace only because the implementation is in this
// header file and we don't want to export symbols to the obj files
namespace tk {


  // spline interpolation
  class spline
  {
  public:
    enum bd_type { first_deriv = 1, second_deriv = 2 };

  private:
    std::vector<double> m_x, m_y; // x,y coordinates of points
    // interpolation parameters
    // f(x) = a*(x-x_i)^3 + b*(x-x_i)^2 + c*(x-x_i) + y_i
    std::vector<double> m_a, m_b, m_c; // spline coefficients
    double              m_b0, m_c0;    // for left extrapol
    bd_type             m_left, m_right;
    double              m_left_value, m_right_value;
    bool                m_force_linear_extrapolation;

  public:
    // set default boundary condition to be zero curvature at both ends
    spline()
      : m_left(second_deriv)
      , m_right(second_deriv)
      , m_left_value(0.0)
      , m_right_value(0.0)
      , m_force_linear_extrapolation(false)
    {
      ;
    }

    // optional, but if called it has to come be before set_points()
    void   set_boundary(bd_type left, double left_value, bd_type right, double right_value, bool force_linear_extrapolation = false);
    void   set_points(const std::vector<double> &x, const std::vector<double> &y, bool cubic_spline = true);
    double operator()(double x) const;
    double deriv(int order, double x) const;
  };

} // namespace tk

#endif /* TK_SPLINE_H */
