#pragma once

#include <interfaces/IProgress.h>
#include "ViewServices.h"
#include "BatchSettings.h"

namespace batch {
  using WFExamPtr = std::unique_ptr<CWFExam>;
  using CTExamPtr = std::unique_ptr<CCTExam>;
  //using ExamPair  = std::pair<WFExamPtr, CTExamPtr>;

  struct ExamPair
  {
    WFExamPtr wfe;
    CTExamPtr cte;
  };

  auto GetSelectedPatient() -> CPatient *;
  auto GetSelectedExams() -> std::vector<ExamPair>;
  auto LoadFellowExam(const WFExamPtr &e) -> CTExamPtr;
  auto LoadFellowExam(const CTExamPtr &e) -> WFExamPtr;
}; // namespace batch

#if 0
class BatchHelper
{
public:
  BatchHelper();

  auto GetSelectedPatient() -> CPatient *;
  auto GetEnabledTypes() const -> std::vector<ViewType>;

  auto GetViewService(ViewType vt) -> ViewServices;

private:
  using WFExamPtr = std::unique_ptr<CWFExam>;
  using CTExamPtr = std::unique_ptr<CCTExam>;

  std::vector<WFExamPtr> m_wfe;
  std::vector<CTExamPtr> m_cte;

  std::vector<WFExamPtr> m_wfeFellow;
  std::vector<CTExamPtr> m_cteFellow;

  static auto LoadWFExam(const GUID &id) -> WFExamPtr;
  static auto LoadCTExam(const GUID &id) -> CTExamPtr;

  bool WFCTOUEnabled() const
  {
    return m_cteFellow.size() == 2 || m_wfeFellow.size() == 2;
  }
};
#endif
