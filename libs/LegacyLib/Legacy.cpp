#include "stdafx.h"
#include "Legacy.h"

real_t COS[360];
real_t SIN[360];
int    MutiCOS[800][360];
int    MutiSIN[800][360];

struct CalcSinCos
{
  CalcSinCos()
  {
    for (int a = 0; a < 360; a++) {
      ::COS[a] = cos(a * _Pi_180);
      ::SIN[a] = sin(a * _Pi_180);
    }

    for (int i = 0; i < 800; i++) {
      for (int a = 0; a < 360; a++) {
        ::MutiCOS[i][a] = intRound(i * ::COS[a]);
        ::MutiSIN[i][a] = intRound(i * ::SIN[a]);
      }
    }
  }
};

static CalcSinCos calcSinCos;
