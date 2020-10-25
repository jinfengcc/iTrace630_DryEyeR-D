#pragma once

#include <libs/CommonLib/SignalUI.h>

class Globals
{
public:
  static Globals &Instance()
  {
    static Globals gd;
    return gd;
  }

  void CreateFonts(double scale);

  struct TAB
  {
    COLORREF fgNormal;
    COLORREF fgSelected;
    COLORREF fgDisabled;
    COLORREF bkColor;
    COLORREF bkColorHover;
    CFont    fontNormal;
    CFont    fontSelected;
    CBrush   bkBrush;
  };

  struct Separator
  {
    COLORREF textColor;
    CFont    font;
    CPen     pen;
  };

  double   animationDuration;
  COLORREF bkColor;
  CBrush   bkBrush;

  TAB       tab;
  Separator sep;

  struct Signals
  {
    sig::Signal<>         idle_;
    sig::Signal<unsigned> changeView;
    sig::Signal<unsigned> viewChanged;
    sig::Signal<>         terminating;
    sig::Signal<>         commandFired;
  };

  Signals signals;

private:
  Globals(void);

  void        Initialize();
  static void Initialize(TAB &tab);
  static void Initialize(Separator &s);
};

#define SIGNALS Globals::Instance().signals
