#include "stdafx.h"
#include "ViewServices.h"
#include "PatientServices.h"
#include "WFSingleSumWnd.h"
#include "WFSingleVrfWnd.h"
#include "libs/CommonLib/TempFile.h"
#include "WFCTSumWnd.h"
#include "WFSingleSoloWnd.h"
#include "CTSingleVrfWnd.h"
#include "CTSingleSumWnd.h"
#include "CTOsherAliWnd.h"
//#include "WFCTSumAnaWnd.h"
#include "WFCTIolWnd.h"
#include "WFCTAngleSumWnd.h"
#include "WFCTDysWnd.h"
#include "WFCTDysPtWnd.h"
#include "WFCTTorAliWnd.h"
//#include "WFCTCorSphWnd.h"
#include "BatchSettings.h"
#include "WFCTOUSumWnd.h"
//#include "WFCTAcqSumWnd.h"
#include "WFNearVisionSumWnd.h"

ViewServices::ViewServices(CWFExam *wfExam, CWFExam *wfExam2, CCTExam *ctExam, CCTExam *ctExam2, const CPatient *patient)
  : m_wfExam{wfExam}
  , m_ctExam{ctExam}
  , m_wfExam2{wfExam2}
  , m_ctExam2{ctExam2}
  , m_patient(const_cast<CPatient *>(patient))
{
  if (m_patient == nullptr) {
    auto patGuids = PatientServices::Instance()->GetSelectedPatientIDs();

    TRACEY_THROW_IF(patGuids.empty(), "No patient selected.");
    TRACEY_THROW_IF(patGuids.size() > 1, "More than one patient selected.");

    m_patient = CDB::Instance()->GetPatient(patGuids[0]);
  }

  TRACEY_THROW_IF(m_wfExam == nullptr && m_ctExam == nullptr, "No exam provided.");

  for (auto i : {m_wfExam, m_wfExam2}) {
    if (i)
      i->Process();
  }

  for (auto i : {m_ctExam, m_ctExam2}) {
    if (i)
      i->Process();
  }
}

cv::Mat ViewServices::Save(ViewType type) const
{
  if (auto w = CreateView(type))
    return SaveImpl(w.get());

  return {};
}

//***************************************************************************************

cv::Mat ViewServices::SaveImpl(CMemWnd *w)
{
  const int W = 1464;
  const int H = w->m_MemDC.m_h * W / w->m_MemDC.m_w;

  auto img = cv::Mat(w->m_MemDC.m_h, w->m_MemDC.m_w, CV_8UC3, w->m_MemDC.m_RGBData).clone();

  // Shrink if big
  if (img.cols > W)
    cv::resize(img, img, {W, H});

  cv::flip(img, img, 0);
  return img;
}

auto ViewServices::CreateView(ViewType type, const RECT *rc) const -> ViewPtr
{
  extern CRect GetSummaryRect();

  CRect rct = rc ? CRect(*rc) : GetSummaryRect();

  ViewPtr vp;
  switch (type) {
  case ViewType::WF_Point:
    vp = CreateImpl<CWFSingleVrfWnd>(rct, Settings.m_TSvrSettings[0]);
    break;

  case ViewType::WF_NearVision:
    vp = CreateImpl<CWFNearVisionSumWnd>(rct, nullptr);
    break;

  //case ViewType::WF_VisualAcuity:
  //  vp = CreateImpl<CWFSingleSumWnd, WF_VISUALACUITY_GUI>(rct, Settings.m_TSsmSettings[0][0]);
  //  break;

  //case ViewType::WF_DepthFocus:
  //  vp = CreateImpl<CWFSingleSumWnd, WF_DEPFOCUS_GUI>(rct, NewSettings.m_TSsm5Settings[0]);
  //  break;

  //case ViewType::WF_RMS:
  //  vp = CreateImpl<CWFSingleSumWnd, WF_WFRMS_GUI>(rct, Settings.m_TSsmSettings[1][0]);
  //  break;

  case ViewType::_MISSING5:
    break;

  case ViewType::WF_Aberation:
    vp = CreateImpl<CWFSingleSoloWnd>(rct, Settings.m_TSsmSettings[1][0]);
    break;

  //case ViewType::WF_Custom:
  //  vp = CreateImpl<CWFSingleSumWnd, WF_WFCUSTOM_GUI>(rct, Settings.m_TSsmSettings[3][0]);
  //  break;

  case ViewType::CT_Rings:
    vp = CreateImpl<CCTSingleVrfWnd>(rct, Settings.m_CSvrSettings);
    break;

  //case ViewType::CT_Summary:
  //  vp = CreateImpl<CCTSingleSumWnd, CT_SUMMARY_GUI>(rct, Settings.m_CSsmSettings[0][0]);
  //  break;

  //case ViewType::CT_Keratometry:
  //  vp = CreateImpl<CCTSingleSumWnd, CT_KERATOMETRY_GUI>(rct, Settings.m_CSsmSettings[1][0]);
  //  break;

  //case ViewType::CT_3DElevation:
  //  vp = CreateImpl<CCTSingleSumWnd, CT_3DELEVATION_GUI>(rct, Settings.m_CSsmSettings[2][0]);
  //  break;

  case ViewType::CT_OsherIris:
    vp = CreateImpl<CCTOsherAliWnd>(rct, Settings.m_CSsmSettings[2][0]);
    break;

  //case ViewType::CT_Custom:
  //  vp = CreateImpl<CCTSingleSumWnd, CT_CUSTOM_GUI>(rct, Settings.m_CSsmSettings[1][0]);
  //  break;

  case ViewType::WFCT_Change:
    if (m_wfExam != nullptr && m_ctExam != nullptr)
      vp = CreateImpl<CWFCTSumWnd, 0>(rct, Settings.m_IDsmSettings[0][0]);
    break;

  case ViewType::WFCT_Summary:
    vp = CreateImpl<CWFCTSumWnd, 1>(rct, Settings.m_IDsmSettings[1][0]);
    break;

  case ViewType::WFCT_MTF:
    vp = CreateImpl<CWFCTSumWnd, 2>(rct, Settings.m_IDsmSettings[2][0]);
    break;

  case ViewType::WFCT_OUOverview:
    vp = CreateImpl<CWFCTOUSumWnd>(rct, NewSettings.m_OWFCTSettings[0]);
    break;

  //case ViewType::WFCT_SumAna:
  //  vp = CreateImpl<CWFCTSumAnaWnd, 0>(rct, NewSettings.m_WFCTSumAnaSettings[0]);
  //  break;

  case ViewType::WFCT_Iol:
    vp = CreateImpl<CWFCTIolWnd>(rct, NewSettings.m_IOLSelSettings[0]);
    break;

  case ViewType::WFCT_AngleKA:
    vp = CreateImpl<CWFCTAngleSumWnd, 2>(rct, Settings.m_TSsmSettings[2][0]);
    break;

  case ViewType::WFCT_DysfunMD:
    vp = CreateImpl<CWFCTDysWnd, 6>(rct, NewSettings.m_DysfuncSettings[0]);
    break;

  case ViewType::WFCT_DysfunPT:
    vp = CreateImpl<CWFCTDysPtWnd, 0>(rct, NewSettings.m_DysPtSettings[0]);
    break;

  case ViewType::WFCT_AstigmSrc:
    vp = CreateImpl<CWFCTSumWnd, 6>(rct, NewSettings.m_InAstigSettings[0]);
    break;

  case ViewType::WFCT_ToricCheck:
    vp = CreateWFCTToricCheck(rct);
    break;

  case ViewType::WFCT_Custom:
    vp = CreateImpl<CWFCTSumWnd, 3>(rct, Settings.m_IDsmSettings[3][0]);
    break;

  case ViewType::_MISSING26:
    break;

  //case ViewType::WFCT_ExamResult:
  //  vp = CreateImpl<CWFCTAcqSumWnd>(rct, Settings.m_TSvrSettings[0]);
  //  break;

  //case ViewType::WFCT_CornealSphAber:
  //  vp = CreateImpl<CWFCTCorSphWnd>(rct, nullptr);
  //  break;

  default:
    break;
  }

  return vp;
}

template <class T, class... Ts>
struct is_any : std::disjunction<std::is_same<T, Ts>...>
{
};

template <class T, class... Ts>
constexpr bool is_any_v = is_any<T, Ts...>::value;

template <class T, int type>
auto ViewServices::CreateImpl(RECT rc, CWndSettings *ws) const -> ViewPtr
{
  constexpr bool WFSUM    = std::is_same_v<T, CWFSingleSumWnd>;
  constexpr bool WFOTHER  = is_any_v<T, CWFSingleVrfWnd, CWFSingleSoloWnd>;
  constexpr bool CTSUM    = std::is_same_v<T, CCTSingleSumWnd>;
  constexpr bool CTOTHER  = std::is_same_v<T, CCTSingleVrfWnd>;
  constexpr bool WFCTSUM1 = std::is_same_v<T, CWFCTSumWnd> || std::is_same_v<T, CWFCTAngleSumWnd>;
  constexpr bool WFCTSUM2 = std::is_same_v<T, CWFCTDysWnd> || std::is_same_v<T, CWFCTDysPtWnd>;

  if constexpr (WFSUM || CTSUM)
    static_assert(type != 0);
  else if constexpr (WFCTSUM1 || WFCTSUM2)
    static_assert(type >= 0);
  else
    static_assert(type == -1);

  constexpr bool CT   = CTSUM || CTOTHER || std::is_same_v<T, CCTOsherAliWnd>;
  constexpr bool WF   = WFSUM || WFOTHER || std::is_same_v<T, CWFNearVisionSumWnd>;
  constexpr bool WFCT = WFCTSUM1 || WFCTSUM2 || is_any_v<T, CWFCTIolWnd>;

  if constexpr (CT) {
    if (m_ctExam == nullptr)
      return {};
  }
  else if constexpr (WF) {
    if (m_wfExam == nullptr)
      return {};
  }
  else if constexpr (WFCT) {
    if (m_wfExam == nullptr || m_ctExam == nullptr)
      return {};
  }

  if constexpr (std::is_same_v<T, CWFCTOUSumWnd>) {
    if (m_wfExam == nullptr || m_wfExam2 == nullptr)
      return {};
    if (m_ctExam == nullptr || m_ctExam2 == nullptr)
      return {};

    return std::make_unique<T>(AfxGetMainWnd(), rc, m_patient, m_wfExam, m_wfExam, m_ctExam2, m_ctExam2, ws, SW_HIDE);
  }

  if constexpr (WFSUM)
    return std::make_unique<T>(AfxGetMainWnd(), rc, m_patient, m_wfExam, ws, type, "", SW_HIDE);

  if constexpr (WFOTHER)
    return std::make_unique<T>(AfxGetMainWnd(), rc, m_patient, m_wfExam, ws, SW_HIDE);

  if constexpr (CTSUM)
    return std::make_unique<T>(AfxGetMainWnd(), rc, m_patient, m_ctExam, ws, 2, type, "", SW_HIDE);

  if constexpr (CTOTHER)
    return std::make_unique<T>(AfxGetMainWnd(), rc, m_patient, m_ctExam, ws, SW_HIDE);

  if constexpr (std::is_same_v<T, CCTOsherAliWnd>)
    return std::make_unique<T>(AfxGetMainWnd(), rc, m_ctExam, TRUE, SW_HIDE, FALSE);

  if constexpr (std::is_same_v<T, CWFNearVisionSumWnd>) {
    // #todo fix this (it needs 2 copies of the wf exam)
    m_duplicateWfExam = CDB::Instance()->LoadWFExam(m_wfExam->m_Header.m_ExamID);
    return std::make_unique<T>(AfxGetMainWnd(), rc, m_patient, m_wfExam, m_duplicateWfExam.get(), 1, 0, SW_HIDE);
  }

  if constexpr (WFCTSUM1)
    return std::make_unique<T>(AfxGetMainWnd(), rc, m_patient, m_wfExam, m_ctExam, ws, type, "", SW_HIDE);

  if constexpr (WFCTSUM2)
    return std::make_unique<T>(AfxGetMainWnd(), rc, m_patient, m_wfExam, m_ctExam, ws, type, SW_HIDE);

  if constexpr (std::is_same_v<T, CWFCTIolWnd>)
    return std::make_unique<T>(AfxGetMainWnd(), rc, m_patient, m_wfExam, m_ctExam, ws, SW_HIDE);

  //if constexpr (std::is_same_v<T, CWFCTCorSphWnd>)
  //  return std::make_unique<T>(AfxGetMainWnd(), rc, m_patient, m_wfExam, m_ctExam, "", SW_HIDE);

  //if constexpr (std::is_same_v<T, CWFCTAcqSumWnd>)
  //  return std::make_unique<T>(AfxGetMainWnd(), rc, m_patient, m_wfExam, m_ctExam, ws, SW_HIDE, FALSE, 0, 0, FALSE, FALSE, 0);
}

auto ViewServices::CreateWFCTToricCheck(RECT rc) const -> ViewPtr
{
  if (!m_ctExam || !m_wfExam)
    return {};

  if (!m_ctExam->m_OpData.m_OK) {
    if (m_ctExam->m_sk_ok) {
      m_ctExam->m_OpData.m_CorneaPreopCylSimK  = m_ctExam->m_sk_df_dp;
      m_ctExam->m_OpData.m_CorneaPreopAxisSimK = m_ctExam->m_sk_st_dg;
    }

    CZernikeSurface Surface = m_ctExam->m_WfSurface;

    Surface.ChangeRMaxUm(2000.0);

    real_t SphEq, Sph, Cyl;
    int    Axis;
    CWFExam::GetSpheqSphCylAxis(Surface, 0, TRUE, SphEq, Sph, Cyl, Axis);
    Cyl = 0.01 * intRound(100.0 * Cyl);

    m_ctExam->m_OpData.m_CorneaPreopCyl = m_ctExam->m_OpData.m_CorneaPreopCylWF = Cyl;
    m_ctExam->m_OpData.m_CorneaPreopAxis = m_ctExam->m_OpData.m_CorneaPreopAxisWF = Axis;

    if (m_wfExam) {
      Surface = m_wfExam->m_WfSurface;
      Surface.ChangeRMaxUm(__min(2000.0, Surface.m_r_max_um));
      CWFExam::GetSpheqSphCylAxis(Surface, 0, TRUE, SphEq, Sph, Cyl, Axis);
      Cyl                              = 0.01 * intRound(100.0 * Cyl);
      m_ctExam->m_OpData.m_EyePreopCyl = m_ctExam->m_OpData.m_EyePreopCylWF = Cyl;
      m_ctExam->m_OpData.m_EyePreopAxis = m_ctExam->m_OpData.m_EyePreopAxisWF = Axis;
    }
  }

  auto p = const_cast<CPatient *>(m_patient);
  return std::make_unique<CWFCTTorAliWnd>(AfxGetMainWnd(), rc, p, m_wfExam, m_ctExam, SW_HIDE);
}
