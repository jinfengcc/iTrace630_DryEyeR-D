#pragma once

enum class ViewType {
  WF_Point            = 0,
  WF_NearVision       = 1,
  WF_VisualAcuity     = 2,
  WF_DepthFocus       = 3,
  WF_RMS              = 4,
  _MISSING5           = 5,
  WF_Aberation        = 6,
  WF_Custom           = 7,
  CT_Rings            = 8,
  CT_Summary          = 9,
  CT_Keratometry      = 10,
  CT_3DElevation      = 11,
  CT_OsherIris        = 12,
  CT_Custom           = 13,
  WFCT_Change         = 14,
  WFCT_Summary        = 15,
  WFCT_MTF            = 16,
  WFCT_OUOverview     = 17,
  WFCT_SumAna         = 18, // Not in 6.x
  WFCT_Iol            = 19,
  WFCT_AngleKA        = 20, // Not in 6.x
  WFCT_DysfunMD       = 21,
  WFCT_DysfunPT       = 22,
  WFCT_AstigmSrc      = 23,
  WFCT_ToricCheck     = 24,
  WFCT_Custom         = 25,
  _MISSING26          = 26,
  WFCT_ExamResult     = 27, // Not in 6.x
  WFCT_CornealSphAber = 28,
  _MAX_VIEW_TYPES,
};
