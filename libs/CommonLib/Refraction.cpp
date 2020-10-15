#include "pch.h"
#include "Refraction.h"

REFRACTION TranslateVertex(const REFRACTION &rx, double vtx)
{
  if (std::abs(vtx - rx.vtx) < 0.001)
    return rx;

  double dv = (vtx - rx.vtx) * 1.0e-3;
  double dt = (rx.sph + rx.cyl) / (1.0 + (rx.sph + rx.cyl) * dv);

  return {rx.sph / (1.0 + rx.sph * dv), dt - rx.sph, vtx, rx.pupil};
}
