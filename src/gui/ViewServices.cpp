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
  TWEAKABLE const RECT rc{0, 0, 1462, 858};
  if (auto w = CreateView(type, &rc))
    return SaveImpl(w.get());

  return {};
}

//***************************************************************************************

cv::Mat ViewServices::SaveImpl(CMemWnd *w)
{
  const int W = 1462;
  const int H = w->m_MemDC.m_h * W / w->m_MemDC.m_w;

  auto img = cv::Mat(w->m_MemDC.m_h, w->m_MemDC.m_w, CV_8UC3, w->m_MemDC.m_RGBData).clone();

  // Shrink if big
  if (img.cols > W)
    cv::resize(img, img, {W, H});

  cv::flip(img, img, 0);
  return img;
}

auto ViewServices::CreateView(ViewType type, const RECT *rc1, int show) const -> ViewPtr
{
  extern CRect GetSummaryRect();

  CRect rct = rc1 ? CRect(*rc1) : GetSummaryRect();

  enum WF
  {
    WF_VisualAcuity = 0,
    WF_RMS          = 1,
    WF_DepthOfFocus = 4,
  };

  enum CT
  {
    CT_SUMMARY       = 0,
    CT_KERATOMETRY   = 1,
    CT_3D_ZELEVATION = 2,
    CT_OSHER_IRIS    = 3,
    CT_CUSTOM        = 4,
  };

  enum WFCT
  {
    WFCT_Change  = 0,
    WFCT_Summary = 1,
    WFCT_MTF     = 2,
  };

  ViewPtr vp;
  switch (type) {
  case ViewType::WF_Point:
    if (m_wfExam)
      vp = std::make_unique<CWFSingleVrfWnd>(AfxGetMainWnd(), rct, m_patient, m_wfExam, &Settings.m_TSvrSettings[0], show);
    break;

  case ViewType::WF_NearVision:
    if (m_wfExam) {
      // #todo fix this (it needs 2 copies of the wf exam)
      m_duplicateWfExam = CDB::Instance()->LoadWFExam(m_wfExam->m_Header.m_ExamID);
      m_duplicateWfExam->Process();
      vp = std::make_unique<CWFNearVisionSumWnd>(AfxGetMainWnd(), rct, m_patient, m_wfExam, m_duplicateWfExam.get(), 2, 0, show);
    }
    break;

  case ViewType::WF_VisualAcuity:
    if (m_wfExam)
      vp = std::make_unique<CWFSingleSumWnd>(AfxGetMainWnd(), rct, m_patient, m_wfExam, &Settings.m_TSsmSettings[0][0], WF_VisualAcuity, show);
    break;

  case ViewType::WF_DepthFocus:
    if (m_wfExam)
      vp = std::make_unique<CWFSingleSumWnd>(AfxGetMainWnd(), rct, m_patient, m_wfExam, &NewSettings.m_TSsm5Settings[0], WF_DepthOfFocus, show);
    break;

  case ViewType::WF_RMS:
    if (m_wfExam)
      vp = std::make_unique<CWFSingleSumWnd>(AfxGetMainWnd(), rct, m_patient, m_wfExam, &Settings.m_TSsmSettings[1][0], WF_RMS, show);
    break;

  case ViewType::_MISSING5:
    break;

  case ViewType::_WF_Aberation:
    break;

  case ViewType::_WF_Custom:
    break;

  case ViewType::CT_Rings:
    if (m_ctExam)
      vp = std::make_unique<CCTSingleVrfWnd>(AfxGetMainWnd(), rct, m_patient, m_ctExam, &Settings.m_CSvrSettings, show);
    break;

  case ViewType::CT_Summary:
    if (m_ctExam)
      vp = std::make_unique<CCTSingleSumWnd>(AfxGetMainWnd(), rct, m_patient, m_ctExam, &Settings.m_CSsmSettings[0][0], CT_SUMMARY, show);
    break;

  case ViewType::CT_Keratometry:
    if (m_ctExam)
      vp = std::make_unique<CCTSingleSumWnd>(AfxGetMainWnd(), rct, m_patient, m_ctExam, &Settings.m_CSsmSettings[1][0], CT_KERATOMETRY, show);
    break;

  case ViewType::CT_3DElevation:
    if (m_ctExam)
      vp = std::make_unique<CCTSingleSumWnd>(AfxGetMainWnd(), rct, m_patient, m_ctExam, &Settings.m_CSsmSettings[2][0], CT_3D_ZELEVATION, show);
    break;

  case ViewType::CT_OsherIris:
    if (m_ctExam)
      vp = std::make_unique<CCTOsherAliWnd>(AfxGetMainWnd(), rct, m_ctExam, TRUE, show);
    break;

  case ViewType::_CT_Custom:
    break;

  case ViewType::WFCT_Change:
    if (m_wfExam != nullptr && m_ctExam != nullptr)
      return std::make_unique<CWFCTSumWnd>(AfxGetMainWnd(), rct, m_patient, m_wfExam, m_ctExam, &Settings.m_IDsmSettings[0][0], WFCT_Change, "", show);
    break;

  case ViewType::WFCT_Summary:
    if (m_wfExam != nullptr && m_ctExam != nullptr)
      vp = std::make_unique<CWFCTSumWnd>(AfxGetMainWnd(), rct, m_patient, m_wfExam, m_ctExam, &Settings.m_IDsmSettings[1][0], WFCT_Summary, "", show);
    break;

  case ViewType::WFCT_MTF:
    if (m_wfExam != nullptr && m_ctExam != nullptr)
      vp = std::make_unique<CWFCTSumWnd>(AfxGetMainWnd(), rct, m_patient, m_wfExam, m_ctExam, &Settings.m_IDsmSettings[2][0], WFCT_MTF, "", show);
    break;

  case ViewType::_WFCT_OUOverview:
    //if (m_wfExam && m_wfExam2 && m_ctExam && m_ctExam2)
    //  vp = std::make_unique<CWFCTOUSumWnd>(AfxGetMainWnd(), rct, m_patient, m_wfExam, m_wfExam, m_ctExam2, m_ctExam2, &NewSettings.m_OWFCTSettings[0], show);
    break;

  case ViewType::_WFCT_SumAna:
    break;

  case ViewType::WFCT_Iol:
    if (m_wfExam != nullptr && m_ctExam != nullptr)
      vp = std::make_unique<CWFCTIolWnd>(AfxGetMainWnd(), rct, m_patient, m_wfExam, m_ctExam, &NewSettings.m_IOLSelSettings[0], show);
    break;

  case ViewType::_WFCT_AngleKA:
    break;

  case ViewType::WFCT_DysfunMD:
    if (m_wfExam != nullptr && m_ctExam != nullptr)
      return std::make_unique<CWFCTDysWnd>(AfxGetMainWnd(), rct, m_patient, m_wfExam, m_ctExam, &NewSettings.m_DysfuncSettings[0], 6, show);
    break;

  case ViewType::WFCT_DysfunPT:
    if (m_wfExam != nullptr && m_ctExam != nullptr)
      return std::make_unique<CWFCTDysPtWnd>(AfxGetMainWnd(), rct, m_patient, m_wfExam, m_ctExam, &NewSettings.m_DysPtSettings[0], 0, show);
    break;

  case ViewType::WFCT_AstigmSrc:
    if (m_wfExam != nullptr && m_ctExam != nullptr)
      vp = std::make_unique<CWFCTSumWnd>(AfxGetMainWnd(), rct, m_patient, m_wfExam, m_ctExam, &NewSettings.m_InAstigSettings[0], 6, "", show);
    break;

  case ViewType::WFCT_ToricCheck:
    vp = CreateWFCTToricCheck(rct);
    break;

  case ViewType::WFCT_Custom:
    if (m_wfExam != nullptr && m_ctExam != nullptr)
      vp = std::make_unique<CWFCTSumWnd>(AfxGetMainWnd(), rct, m_patient, m_wfExam, m_ctExam, &Settings.m_IDsmSettings[3][0], 3, "", show);
    break;

  case ViewType::_MISSING26:
    break;

  case ViewType::_WFCT_ExamResult:
    break;

  case ViewType::_WFCT_CornealSphAber:
    break;

  default:
    break;
  }

  return vp;
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
