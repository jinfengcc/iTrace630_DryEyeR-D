#include "pch.h"
#include "SetupPageSelection.h"
#include "libs/CommonLib/ViewTypes.h"

int CSetupPageSelection::OnApply()
{
  return DoDataExchange(TRUE) ? PSNRET_NOERROR : PSNRET_INVALID_NOCHANGEPAGE;
}

BOOL CSetupPageSelection::DoDataExchange(BOOL bSaveAndValidate, UINT nCtlID)
{
  auto CheckBit = [&](int nID, ViewType vt) {
    auto wMask = 1u << static_cast<int>(vt);
    int  x     = (m_cfg.selectionFlags & wMask) ? 1 : 0;
    DDX_Check(nID, x, bSaveAndValidate);
    if (bSaveAndValidate) {
      if (x)
        m_cfg.selectionFlags |= wMask;
      else
        m_cfg.selectionFlags &= ~wMask;
    }
  };

  // clang-format off
  CheckBit(IDC_WFPOINTS_CHECK     , ViewType::WF_Point           );   // Points
  CheckBit(IDC_NEARVISION_CHECK   , ViewType::WF_NearVision      );   // Near Vision
  CheckBit(IDC_WFVA_CHECK         , ViewType::WF_VisualAcuity    );   // WF Visual Acuity
  CheckBit(IDC_DEPTHFOCUS_CHECK   , ViewType::WF_DepthFocus      );   // WF Depth of Focus
  CheckBit(IDC_WFRMS_CHECK        , ViewType::WF_RMS             );   // WF/RMS
  CheckBit(IDC_WCCUSTOM_CHECK     , ViewType::WF_Custom          );   // WF Custom

  CheckBit(IDC_CTRINGS_CHECK      , ViewType::CT_Rings           );   // CT Summary
  CheckBit(IDC_CTSU_CHECK         , ViewType::CT_Summary         );   // CT Summary
  CheckBit(IDC_CTKE_CHECK         , ViewType::CT_Keratometry     );   // Keratometry
  CheckBit(IDC_CT3D_CHECK         , ViewType::CT_3DElevation     );   // 3D Z-Elevation
  CheckBit(IDC_CTOSHER_CHECK      , ViewType::CT_OsherIris       );   // CT Osher
  CheckBit(IDC_CTCUSTOM_CHECK     , ViewType::CT_Custom          );   // CT Custom

  CheckBit(IDC_WCCH_CHECK         , ViewType::WFCT_Change        );   // Chang Analysis
  CheckBit(IDC_WCSU_CHECK         , ViewType::WFCT_Summary       );   // WF/CT Summary
  CheckBit(IDC_WCVA_CHECK         , ViewType::WFCT_MTF           );   // WF/CT MTF
  CheckBit(IDC_WCIOL_CHECK        , ViewType::WFCT_Iol           );   // IOL Selection Assistant
  CheckBit(IDC_DYSFUN_CHECK       , ViewType::WFCT_DysfunMD      );   // Dysfunctional Analysis MD
  CheckBit(IDC_DYSPTFUN_CHECK     , ViewType::WFCT_DysfunPT      );   // Dysfunctional Analysis PT
  CheckBit(IDC_ASTISOU_CHECK      , ViewType::WFCT_AstigmSrc     );   // Astigmatism Source
  CheckBit(IDC_TORLI_CHECK        , ViewType::WFCT_ToricCheck    );   // Toric Check
  CheckBit(IDC_WCCUSTOM_CHECK     , ViewType::WFCT_Custom        );
  // clang-format on

  if (bSaveAndValidate && m_cfg.selectionFlags == 0) {
    AtlMessageBox(*this, _T("You must select at least one view"));
    return FALSE;
  }

  return TRUE;
}

BOOL CSetupPageSelection::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
  DoDataExchange(DDX_LOAD);
  return TRUE; // Let the system set the focus
}

void CSetupPageSelection::OnContextMenu(CWindow wnd, CPoint point)
{
  CMenu menu;
  menu.CreatePopupMenu();

  enum
  {
    all_ct = 1001,
    no_ct,
    all_wf,
    no_wf,
    all_ctwf,
    no_ctwf,
    all,
    none
  };

  menu.AppendMenu(MF_STRING, all_ct, L"CT (All)");
  menu.AppendMenu(MF_STRING, no_ct, L"CT (None)");
  menu.AppendMenu(MF_SEPARATOR);
  menu.AppendMenu(MF_STRING, all_wf, L"WF (All)");
  menu.AppendMenu(MF_STRING, no_wf, L"WF (None)");
  menu.AppendMenu(MF_SEPARATOR);
  menu.AppendMenu(MF_STRING, all_ctwf, L"CTWF (All)");
  menu.AppendMenu(MF_STRING, no_ctwf, L"CTWF (None)");
  menu.AppendMenu(MF_SEPARATOR);
  menu.AppendMenu(MF_STRING, all, L"All");
  menu.AppendMenu(MF_STRING, none, L"None");

  UINT uFlags = TPM_NONOTIFY | TPM_RETURNCMD | TPM_TOPALIGN | TPM_RIGHTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON;
  auto res    = menu.TrackPopupMenu(uFlags, point.x, point.y, *this);

  // clang-format off
  constexpr auto CT = CombineEnumFlags(
    ViewType::CT_Rings,
    ViewType::CT_Summary,
    ViewType::CT_Keratometry,
    ViewType::CT_3DElevation,
    ViewType::CT_OsherIris,
    ViewType::CT_Custom
  );
  constexpr auto WF = CombineEnumFlags(
    ViewType::WF_Point,
    ViewType::WF_NearVision,
    ViewType::WF_VisualAcuity,
    ViewType::WF_DepthFocus,
    ViewType::WF_RMS,
    ViewType::WF_Custom
  );
  constexpr auto WFCT = CombineEnumFlags(
    ViewType::WFCT_Change,
    ViewType::WFCT_Summary,
    ViewType::WFCT_MTF,
    ViewType::WFCT_Iol,
    ViewType::WFCT_DysfunMD,
    ViewType::WFCT_DysfunPT,
    ViewType::WFCT_AstigmSrc,
    ViewType::WFCT_ToricCheck,
    ViewType::WFCT_Custom
  );
  // clang-format on

  switch (res) {
  case all_ct:
    m_cfg.selectionFlags |= CT;
    break;
  case no_ct:
    m_cfg.selectionFlags &= ~CT;
    break;
  case all_wf:
    m_cfg.selectionFlags |= WF;
    break;
  case no_wf:
    m_cfg.selectionFlags &= ~WF;
    break;
  case all_ctwf:
    m_cfg.selectionFlags |= WFCT;
    break;
  case no_ctwf:
    m_cfg.selectionFlags &= ~WFCT;
    break;
  case all:
    m_cfg.selectionFlags = std::uint32_t(-1);
    break;
  case none:
    m_cfg.selectionFlags = 0;
    break;
  default:
    return;
  }

  DoDataExchange(DDX_LOAD);
}
