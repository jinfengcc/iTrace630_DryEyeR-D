#include "stdafx.h"
#include "BatchSettings.h"
#include "Settings.h"
#include "GlobalFunctions.h"

bool BatchSettings::BatchEnabled(ViewType vt) const
{
  auto n = static_cast<unsigned>(vt);
  if (m_flags.has_value()) {
    return (m_flags.value() & (1 << n)) != 0;
  }
  else {
    ATLASSERT(false);
    return false;
  }
}

auto BatchSettings::BatchEnabled() const -> std::vector<ViewType>
{
  std::vector<ViewType> res;

  for (unsigned i = 0; i < static_cast<unsigned>(ViewType::_MAX_VIEW_TYPES); ++i) {
    if (auto t = static_cast<ViewType>(i); BatchEnabled(t))
      res.push_back(t);
  }

  return res;
}

const wchar_t *BatchSettings::BatchLabel(ViewType vt, bool abbrvName)
{
  struct Names
  {
    const wchar_t *abrv;
    const wchar_t *full;
  };

  // clang-format off
  static std::array names{
    Names{L"WFPoint"       , L"WF Points"                         },   // WF_Point            = 0,
    Names{L"WFNearVision"  , L"WF Simulated Near Vision"          },   // WF_NearVision       = 1,
    Names{L"WFVisualAcuity", L"WF Visual Acuity"                  },   // WF_VisualAcuity     = 2,
    Names{L"WFDepthFocus"  , L"WF Depth of Focus"                 },   // WF_DepthFocus       = 3,
    Names{L"WFRMS"         , L"WF RMS"                            },   // WF_RMS              = 4,
    Names{L"?????"         , L""                                  },   // _MISSING5           = 5,
    Names{L"WFAberration"  , L"WF Aberration Analysis"            },   // WF_Aberation        = 6,
    Names{L"WFCustom"      , L"WF Custom"                         },   // WF_Custom           = 7,
    Names{L"CTRings"       , L"CT Rings"                          },   // CT_Rings            = 8,
    Names{L"CTSummary"     , L"CT Summary"                        },   // CT_Summary          = 9,
    Names{L"CTKeratometry" , L"CT Keratometry"                    },   // CT_Keratometry      = 10,
    Names{L"CTZElevation"  , L"CT 3D-Elevation"                   },   // CT_3DElevation      = 11,
    Names{L"CTOsher"       , L"CT Osher Iris Imaging"             },   // CT_OsherIris        = 12,
    Names{L"CTCustom"      , L"CT Custom"                         },   // CT_Custom           = 13,
    Names{L"WFCTChang"     , L"WF/CT Chang Analysis"              },   // WFCT_Change         = 14,
    Names{L"WFCTSummary"   , L"WF/CT Summary"                     },   // WFCT_Summary        = 15,
    Names{L"WFCTMTF"       , L"WF/CT MTF"                         },   // WFCT_MTF            = 16,
    Names{L"WFCOUOverview" , L"WF/CT OU Overview"                 },   // WFCT_OUOverview     = 17,
    Names{L"WFCTSumAna"    , L"WF/CT Summary and Analysis"        },   // WFCT_SumAna         = 18,
    Names{L"WFCTIOL"       , L"WF/CT IOL Selection"               },   // WFCT_Iol            = 19,
    Names{L"WFCTAngleKA"   , L"WF/CT Angle K/A"                   },   // WFCT_AngleKA        = 20,
    Names{L"WFCTDysfunMD"  , L"WF/CT Dysfunctional Lens - MD"     },   // WFCT_DysfunMD       = 21,
    Names{L"WFCTDysfunPT"  , L"WF/CT Dysfunctional Lens - PT"     },   // WFCT_DysfunPT       = 22,
    Names{L"WFCTAstigmSrc" , L"WF/CT Astigmatism Source"          },   // WFCT_AstigmSrc      = 23,
    Names{L"WFCTToricCheck", L"WF/CT Toric Check"                 },   // WFCT_ToricCheck     = 24,
    Names{L"WFCTCustom"    , L"WF/CT Custom"                      },   // WFCT_Custom         = 25,
    Names{L"?????"         , L""                                  },   // _MISSING26          = 26,
    Names{L"WFCTExamResult", L"WF/CT Exam Results"                },   // WFCT_ExamResult     = 27,
    Names{L"WFCTCorSph"    , L"WF/CT Corneal Spherical Aberration"},   // WFCT_CornealSphAber = 28,
  };
  // clang-format on

  if (auto n = static_cast<unsigned>(vt); n < names.size())
    return abbrvName ? names[n].abrv : names[n].full;
  else
    return L"????????????";
}

