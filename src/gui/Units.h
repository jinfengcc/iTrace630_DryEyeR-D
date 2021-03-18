#pragma once

enum class Unit
{
  none, micron, mm, D, deg
};

const wchar_t *GetUnitText(Unit unit);


