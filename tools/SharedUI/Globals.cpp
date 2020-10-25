#include "pch.h"
#include "Globals.h"

#include <fmt/color.h>

const int FONT_HEIGHT = 100;

DWORD ToRGB( fmt::color c)
{
  union {
    DWORD dw;
    BYTE  c[4];
  } x;

  x.dw = static_cast<DWORD>(c);
  std::swap(x.c[0], x.c[2]);
  return x.dw;
}

Globals::Globals(void)
{
  Initialize();
  Initialize(tab);
  Initialize(sep);

  CreateFonts(1.0);
}

void Globals::CreateFonts(double scale)
{
  if (tab.fontNormal)
    return;

  if (tab.fontNormal)
    tab.fontNormal.DeleteObject();
  if (tab.fontSelected)
    tab.fontSelected.DeleteObject();
  if (sep.font)
    sep.font.DeleteObject();

  LOGFONT lf = {static_cast<int>(FONT_HEIGHT * scale), 0, 0, 0, 400, 255, 0, 0, 0, 3, 2, 1, 34, _T("Arial")};
  tab.fontNormal.CreatePointFontIndirect(&lf, nullptr);

  lf.lfWeight = FW_BOLD;
  lf.lfItalic = 0;
  tab.fontSelected.CreatePointFontIndirect(&lf, nullptr);

  lf.lfHeight = FONT_HEIGHT - 20;
  sep.font.CreatePointFont(static_cast<int>((FONT_HEIGHT - 20) * scale), _T("Calibri"));
}

void Globals::Initialize()
{
  animationDuration = 0.2;

  bkColor = RGB(251, 251, 252); // RGB(111, 255, 255);
  // bkColor = ToRGB(fmt::color::white_smoke);
  bkColor = ToRGB(fmt::color::red);
  bkColor = GetSysColor(COLOR_BTNFACE);

  bkBrush.CreateSolidBrush(bkColor);
}

void Globals::Initialize(TAB &tb)
{
  tb.fgNormal     = RGB(255, 255, 255);
  tb.fgSelected   = RGB(0, 0, 0);
  tb.fgDisabled   = RGB(121, 148, 176); // RGB(209, 212, 222);
  tb.bkColor      = RGB(101, 128, 146); // RGB(229, 243, 251);
  tb.bkColorHover = RGB(121, 148, 176); // RGB(203, 232, 246);


  //tab.fgNormal = static_cast<DWORD>(fmt::color::dark_blue);
  // tab.bkColor = static_cast<DWORD>(fmt::color::dark_blue);
  tb.fgNormal = static_cast<DWORD>(fmt::color::dark_blue);

  tb.bkBrush.CreateSolidBrush(tb.bkColor);
}

void Globals::Initialize(Separator &s)
{
  s.textColor = RGB(181, 181, 182);
  s.pen.CreatePen(PS_SOLID, 1, RGB(211, 211, 212));
}
