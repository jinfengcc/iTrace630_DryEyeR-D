#include "pch.h"
#include "ListCtrlColumns.h"

void AddListColumns(CListViewCtrl &listCtrl, gsl::span<LV_COLUMN> lva, bool bSizeIsPercentage, bool bAddMargin)
{
  CRect rc;
  listCtrl.GetClientRect(&rc);

  if (bAddMargin) {
    rc.right -= GetSystemMetrics(SM_CXVSCROLL) + 1;
  }

  int cxTotal = 0;
  if (bSizeIsPercentage) {
    for (auto &i : lva)
      cxTotal += i.cx;
  }

  int cx = 0;
  for (unsigned i = 0; i < lva.size(); i++) {
    auto lv = lva[i];
    if (bSizeIsPercentage) {
      if (i == lva.size() - 1)
        lv.cx = rc.Width() - cx;
      else
        lv.cx = MulDiv(lv.cx, rc.Width(), cxTotal);
      cx += lv.cx;
    }

    listCtrl.InsertColumn(i, &lv);
  }
}

void SetColumnSize(CListViewCtrl &listCtrl, gsl::span<LV_COLUMN> lva, int cx)
{
  if (cx == 0) {
    CRect rc;
    listCtrl.GetClientRect(rc);
    cx = rc.Width();
  }


  {
    NONCLIENTMETRICS s;
    memset(&s, 0, sizeof(s));
    s.cbSize = sizeof(s);
    ::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &s, 0);

    cx -= s.iScrollWidth;
  }

  int total = 0;
  for (const auto &c : lva)
    total += c.cx;

  for (unsigned i = 0; i < lva.size(); i++) {
    LVCOLUMN col;

    col.mask = LVCF_WIDTH;

    if (listCtrl.GetColumn(i, &col)) {
      if (auto x = lva[i].cx * cx / total; x != col.cx) {
        col.cx = x;
        listCtrl.SetColumn(i, &col);
      }
    }
  }
}
