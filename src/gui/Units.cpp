#include "stdafx.h"
#include "Units.h"

const wchar_t *GetUnitText(Unit unit)
{
  static std::array txt = {L"", L"µ", L"mm", L"D", L"°"};
  return txt[static_cast<int>(unit)];
}
