#pragma once

#include <Gdiplus.h>

#define ASSERT ATLASSERT
using real_t = double;

#include "Utils.h"
#include "Sort.h"
#include "Structures.h"

template<class T>
using Vector = Matrix<T>;

// 7.0.0 For acq res wnd res icon selection, used in 'AcqResWnd.cpp'
#define ACQRES_CG    0
#define ACQRES_CG_IG 1
#define ACQRES_CG_IR 2
#define ACQRES_CG_IY 3
#define ACQRES_CR    4
#define ACQRES_CR_IG 5
#define ACQRES_CR_IR 6
#define ACQRES_CR_IY 7
#define ACQRES_CY    8
#define ACQRES_CY_IG 9
#define ACQRES_CY_IR 10
#define ACQRES_CY_IY 11
#define ACQRES_GRAY  12
// 7.0.0

#ifndef DEG2RAD_DEFINED
#define DEG2RAD_DEFINED

inline double Deg2Rad(double x)
{
  return x * _Pi_180;
}
inline double Rad2Deg(double x)
{
  return x * _180_Pi;
}
inline cv::Point2d Polar2Cart(double r, double th_deg)
{
  return {r * cos(Deg2Rad(th_deg)), r * sin(Deg2Rad(th_deg))};
}
#endif

// #HACK: Remove the defines below
#define Error(x)

// #define LINE_SIZE(w) (((w) + (w) + (w)) & 3 ? (((w) + (w) + (w)) | 3) + 1 : (w) + (w) + (w))
#define LINE_SIZE(w) (w)
#define IMAGE_SIZE(w, h) ((h)*LINE_SIZE(w))

//***************************************************************************************

//template <typename T>
//void sort_asc(T *m, const int n, int k = INT_MAX)
//{
//  k = __min(k, n - 1);
//
//  for (int i = 0; i < k; i++) {
//    for (int j = n - 1; j > i; j--) {
//      if (m[j - 1] > m[j])
//        Swap(m[j], m[j - 1]);
//    }
//  }
//}
//
////***************************************************************************************
//
//template <typename T>
//void sort_des(T *m, const int n, int k = INT_MAX)
//{
//  k = __min(k, n - 1);
//
//  for (int i = 0; i < k; i++) {
//    for (int j = n - 1; j > i; j--) {
//      if (m[j - 1] < m[j])
//        Swap(m[j], m[j - 1]);
//    }
//  }
//}

extern real_t COS[360];
extern real_t SIN[360];
extern int    MutiCOS[800][360];
extern int    MutiSIN[800][360];

//////////////////////////////////////////////////////////////////////////////////////
