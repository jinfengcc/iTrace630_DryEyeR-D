#include "stdafx.h"
#include "CustomMenu.h"

namespace {
  const int MARGIN = 5;
  const int BASEID = 1000;

  inline auto Color(DWORD c)
  {
    return Gdiplus::Color(c | 0xFF000000);
  }

  struct Assets
  {
    Gdiplus::Font font{L"Arial", 9};

    // clang-format off
    Gdiplus::SolidBrush textBrush       {Color(BLACK    )};
    Gdiplus::SolidBrush frameBush       {Color(WHITE    )};
    Gdiplus::SolidBrush bkBrush         {Color(0xE2E2E2 )};
    Gdiplus::SolidBrush hiliteBrush {Color(0xFEC27F )};
    Gdiplus::SolidBrush selBrush    {Color(0xFCF1E6 )};

    //Gdiplus::Pen        frameNormalPen  {Color(BTN_EDGE ), 1.0F};
    Gdiplus::Pen        frameSelectedPen{Color(0xFEC27F), 1.0F};
    // clang-format on
  };

  static Assets *assets;
} // namespace

//////////////////////////////////////////////////////////////////////

CCustomMenu::CCustomMenu(int width, int height)
  : m_size(width + MARGIN, height + MARGIN)
{
  if (assets == nullptr)
    assets = new Assets;

  CreatePopupMenu();
}

CCustomMenu::~CCustomMenu()
{
}

BOOL CCustomMenu::LoadMenu(LPCTSTR lpszResourceName)
{
  ASSERT(FALSE);
  return 0;
}

BOOL CCustomMenu::LoadMenu(UINT nIDResource)
{
  ASSERT(FALSE);
  return 0;
}

void CCustomMenu::AddMenuItem(const TCHAR *sz)
{
  AppendMenu(MF_OWNERDRAW, BASEID + m_menuData.size(), (LPCTSTR)m_menuData.size());
  m_menuData.push_back({CString(sz)});
}

int CCustomMenu::TrackPopupMenu(int x, int y, CWnd *pWnd)
{
  UINT uFlags = TPM_NONOTIFY | TPM_RETURNCMD | TPM_TOPALIGN | TPM_RIGHTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON;

  auto res = CMenu::TrackPopupMenu(uFlags, x - MARGIN, y - MARGIN, pWnd);
  return res >= BASEID ? (res - BASEID) : -1;
}

void CCustomMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
  lpMeasureItemStruct->itemWidth  = m_size.cx;
  lpMeasureItemStruct->itemHeight = m_size.cy;
}

void CCustomMenu::DrawItem(LPDRAWITEMSTRUCT lpds)
{
  Gdiplus::Graphics g(lpds->hDC);
  CRect             t(lpds->rcItem);
  Gdiplus::RectF    rc((float)t.left, (float)t.top, (float)t.Width(), (float)t.Height());

  DrawFrame(g, rc, lpds);
  DrawText(g, rc, (unsigned)lpds->itemData);
}

void CCustomMenu::DrawFrame(Gdiplus::Graphics &g, Gdiplus::RectF &rc, LPDRAWITEMSTRUCT lpds)
{
  using namespace Gdiplus;

  g.FillRectangle(&assets->frameBush, rc);
  rc.Inflate(-MARGIN, -MARGIN);

  if ((lpds->itemState & ODS_SELECTED) && (lpds->itemAction & (ODA_SELECT | ODA_DRAWENTIRE))) {
    g.FillRectangle(&assets->hiliteBrush, rc);
  }
  else if (auto ndx = (unsigned)lpds->itemData; ndx == m_selected) {
    g.FillRectangle(&assets->selBrush, rc);
    g.DrawRectangle(&assets->frameSelectedPen, rc);
  }
  else {
    g.FillRectangle(&assets->bkBrush, rc);
  }
}

void CCustomMenu::DrawText(Gdiplus::Graphics &g, const Gdiplus::RectF &rc, unsigned n)
{
  using namespace Gdiplus;

  StringFormat format;
  format.SetLineAlignment(StringAlignmentCenter);
  format.SetAlignment(StringAlignmentCenter);

  auto s = CStringW(m_menuData[n].strCaption);
  g.DrawString(s.GetString(), -1, &assets->font, rc, &format, &assets->textBrush);
}
