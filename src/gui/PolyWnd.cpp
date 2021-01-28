//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "PolyWnd.h"

//***************************************************************************************

CPolyWnd::CPolyWnd(RECT& Rect, CWnd* pWnd) : CDispWnd(Rect, pWnd)
{
}

//***************************************************************************************

void CPolyWnd::RepaintMemDC()
{
  ClearMemDC();
  Angles();
  Labels();
  Border();
}

//***************************************************************************************

void CPolyWnd::Angles()
{
  RECT Rect;
  GetClientRect(&Rect);

  real dw = (real)(Rect.right - Rect.left) / (ZERNIKE_NUM_MODES + 1);
  real dh = (real)(Rect.bottom - Rect.top) / (ZERNIKE_NUM_MODES + 1);
  CString s;

  int FontSize = round(0.020 * m_h);
  CMFont Font(FontSize, 400, "Arial");

  for (int y = -1; y < ZERNIKE_NUM_MODES; y++) {

    Rect.top = round((y + 1) * dh);
    Rect.bottom = round((y + 1) * dh + dh + 1);

    for (int x = -1; x < ZERNIKE_NUM_MODES; x++) {

      Rect.left = round((x + 1) * dw);
      Rect.right = round((x + 1) * dw + dw + 1);

      if (x == -1 && y >= 0) {
        s.Format("%i", y);
        m_MemDC.WriteText(s, Rect, Font, m_Printing ? BLACK : WHITE, 1);
      }
      else if (x >= 0 && y == -1) {
        s.Format("%i", x);
        m_MemDC.WriteText(s, Rect, Font, m_Printing ? BLACK : WHITE, 1);
      }
      else if (x >= 0 && y >= 0) {
        real a = 0.0;//m_Surface.m_a[y][x];
        if (a != INVALID_VALUE) {
          COLORREF col;
          if (a > 89.9) col = 0x0000af00;
          else if (a > 89.0) col = YELLOW;
          else if (a > 80.0) col = ORANGE;
          else col = RED;
          s.Format("%i", round(a));
          m_MemDC.WriteText(s, Rect, Font, col, 1);
        }
      }
    }
  }

}

//***************************************************************************************
