#pragma once

// NOTE: All Dimensions(Vertex, Pupil) are in mm

struct REFRACTION
{
  double sph, cyl, axis, vtx;
  double pupil;
};

REFRACTION TranslateVertex(const REFRACTION &rx, double vtx);
