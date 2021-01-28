#include "stdafx.h"
#include "BatchHelper.h"
#include "BatchSettings.h"
#include "PatientServices.h"
#include "GlobalFunctions.h"

namespace batch {

  auto LoadWFExam(const GUID &id) -> WFExamPtr
  {
    if (id == GUID_NULL)
      return {};

    auto ex = std::make_unique<CWFExam>();
    if (!CDB::Instance()->LoadWFExam(id, ex.get()))
      ex.reset();

    return ex;
  }

  auto LoadCTExam(const GUID &id) -> CTExamPtr
  {
    if (id == GUID_NULL)
      return {};

    auto ex = std::make_unique<CCTExam>();
    if (!CDB::Instance()->LoadCTExam(id, ex.get()))
      ex.reset();

    return ex;
  }

  auto GetSelectedPatient() -> CPatient *
  {
    auto patGuids = PatientServices::Instance()->GetSelectedPatientIDs();

    TRACEY_THROW_IF(patGuids.empty(), "No patient selected.");
    TRACEY_THROW_IF(patGuids.size() > 1, "More than one patient selected.");

    return CDB::Instance()->GetPatient(patGuids[0]);
  }

  auto GetSelectedExams() -> std::vector<ExamPair>
  {
    auto wf = PatientServices::Instance()->GetSelectedExamIDs(PatientServices::Type::WF);
    auto ct = PatientServices::Instance()->GetSelectedExamIDs(PatientServices::Type::CT);

    std::vector<ExamPair> pd;

    if (wf.size() == 1 && ct.size() == 1) {
      auto wfe = LoadWFExam(wf[0]);
      auto cte = LoadCTExam(ct[0]);
      pd.emplace_back(std::move(wfe), std::move(cte));
    }
    else if (ct.size() == 0) {
      for (auto &id : wf) {
        if (auto wfe = LoadWFExam(id)) {
          //auto cte = LoadCTExam(wfe->m_Header.m_FellowExamID);
          pd.emplace_back(std::move(wfe), nullptr);
        }
      }
    }
    else if (wf.size() == 0) {
      for (auto &id : ct) {
        if (auto cte = LoadCTExam(id)) {
          //auto wfe = LoadWFExam(cte->m_Header.m_FellowExamID);
          pd.emplace_back(nullptr, std::move(cte));
        }
      }
    }
    else {
      TRACEY_THROW("More than one WF/CT exams selected");
    }

    // sanity check
    for (auto &p : pd) {
      if (p.wfe && p.cte && p.wfe->m_Header.IsOD() != p.cte->m_Header.IsOD())
        TRACEY_THROW("Invalid exam selection (WF/CT for different eyes)");
    }

    TRACEY_THROW_IF(pd.empty(), "No exams selected");
    return pd;
  }

  auto LoadFellowExam(const WFExamPtr &e) -> CTExamPtr
  {
    return LoadCTExam(e->m_Header.m_FellowExamID);
  }

  auto LoadFellowExam(const CTExamPtr &e) -> WFExamPtr
  {
    return LoadWFExam(e->m_Header.m_FellowExamID);
  }

} // namespace batch

#if 0
BatchHelper::BatchHelper()
{
  auto wf = PatientServices::Instance()->GetSelectedExamIDs(PatientServices::Type::WF);
  for (auto &id : wf) {
    if (auto x = LoadWFExam(id))
      m_wfe.emplace_back(std::move(x));
  }

  auto ct = PatientServices::Instance()->GetSelectedExamIDs(PatientServices::Type::CT);
  for (auto &id : ct) {
    if (auto x = LoadCTExam(id))
      m_cte.emplace_back(std::move(x));
  }

  if (m_wfe.size() == 2 && m_wfe[0]->m_Header.IsOD() != m_wfe[1]->m_Header.IsOD()) {
    for (auto &e : m_wfe) {
      if (auto f = LoadCTExam(e->m_Header.m_FellowExamID))
        m_cteFellow.emplace_back(std::move(f));
    }
  }

  if (m_cte.size() == 2 && m_cte[0]->m_Header.IsOD() != m_cte[1]->m_Header.IsOD()) {
    for (auto &e : m_cte) {
      if (auto f = LoadWFExam(e->m_Header.m_FellowExamID))
        m_wfeFellow.emplace_back(std::move(f));
    }
  }
}

auto BatchHelper::GetSelectedPatient() -> CPatient *
{
  auto patGuids = PatientServices::Instance()->GetSelectedPatientIDs();

  TRACEY_THROW_IF(patGuids.empty(), "No patient selected.");
  TRACEY_THROW_IF(patGuids.size() > 1, "More than one patient selected.");

  return CDB::Instance()->GetPatient(patGuids[0]);
}

auto BatchHelper::GetEnabledTypes() const -> std::vector<ViewType>
{
  auto types = BatchSettings().BatchEnabled();
  if (!WFCTOUEnabled()) {
    std::erase(types, ViewType::WFCT_OUOverview);
  }
  return types;
}

auto BatchHelper::LoadWFExam(const GUID &id) -> WFExamPtr
{
  if (id == GUID_NULL)
    return {};

  auto ex = std::make_unique<CWFExam>();
  if (!CDB::Instance()->LoadWFExam(id, ex.get()))
    ex.reset();

  return ex;
}

auto BatchHelper::LoadCTExam(const GUID &id) -> CTExamPtr
{
  if (id == GUID_NULL)
    return {};

  auto ex = std::make_unique<CCTExam>();
  if (!CDB::Instance()->LoadCTExam(id, ex.get()))
    ex.reset();

  return ex;
}
#endif
