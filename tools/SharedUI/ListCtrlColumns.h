#pragma once

#include <gsl/gsl>

void AddListColumns(CListViewCtrl &listCtrl, gsl::span<LV_COLUMN> lva, bool bSizeIsPercentage = false, bool bAddMargin = true);
void SetColumnSize(CListViewCtrl &listCtrl, gsl::span<LV_COLUMN> lva, int cx = 0);
