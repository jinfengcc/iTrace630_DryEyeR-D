#include "stdafx.h"
#include "BatchExport.h"
#include "BatchSettings.h"

namespace {
  template <class... Arg>
  auto MakeVector(const Arg &... args)
  {
    auto f = [](const auto &a) { return a ? &a->m_Header : nullptr; };
    auto v = std::vector<const CExamHeader *>{f(args)...};

    std::erase_if(v, [](const auto *h) { return h == nullptr; });
    return v;
  }
} // namespace

BatchExport::BatchExport()
  : patient(batch::GetSelectedPatient())
  , m_exams(batch::GetSelectedExams())
{
}

bool BatchExport::Export()
{
  if (m_exams.empty()) {
    ::Warning("No exams were selected for export");
    return false;
  }

  int nExams = 0;

  try {
    auto progress = CreateObj<IProgress>();

    progress->Initialize(AfxGetMainWnd()->GetSafeHwnd(), {.showCancel = true, .progType = IProgress::ProgType::marquee});

    for (auto &e : m_exams) {
      nExams += (e.wfe == nullptr ? 0 : 1);
      nExams += (e.cte == nullptr ? 0 : 1);
    }

    SetTitle(progress, "Exporting {} exam{}", nExams, nExams == 1 ? "" : "s");
    SetStatus(progress, "Preparing...");

    auto settings = Settings();

    auto types = settings.BatchEnabled();
    progress->SetProgressRange(0, m_exams.size() * types.size() - 1);

    int n = 0;
    if (settings.BatchEnabled(ViewType::WFCT_OUOverview)) {
      // Special handling of WF/CT OU
      HandleWFCTOU(ViewType::WFCT_OUOverview);
      std::erase(types, ViewType::WFCT_OUOverview);
      progress->SetProgress(++n);
    }

    for (auto &e : m_exams) {
      ViewServices vs(e.wfe.get(), e.cte.get(), patient);

      for (auto vt : types) {
        SetStatus(progress, L"Exporting {}...", BatchSettings::BatchLabel(vt, false));
        progress->SetProgress(++n);

        auto headers = MakeVector(e.wfe, e.cte);
        OnExport(vs, vt, patient, {headers});

        if (progress->CancelRequested())
          return false;
      }
    }
  }
  catch (const std::exception &e) {
    if (*e.what())
      Error(e.what());
    return false;
  }

  Info(fmt::format("{} exam{} successfully exported", nExams, nExams == 1 ? "" : "s").c_str());
  return true;
}

void BatchExport::HandleWFCTOU(ViewType type)
{
  // Must have exactly 2 exams
  if (m_exams.size() != 2)
    return;

  // and only WF or CT but not both
  for (auto &e : m_exams) {
    if (e.wfe && e.cte || (e.wfe == nullptr && e.cte == nullptr))
      return;
  }

  if (m_exams[0].wfe && m_exams[1].wfe) {
    auto &wf0 = m_exams[0].wfe;
    auto &wf1 = m_exams[1].wfe;

    if (wf0->m_Header.IsOD() == wf1->m_Header.IsOD()) {
      auto ct0 = batch::LoadFellowExam(wf0);
      auto ct1 = batch::LoadFellowExam(wf1);

      if (ct0 && ct1) {
        ViewServices vs(wf0.get(), wf1.get(), ct0.get(), ct1.get(), patient);

        auto headers = MakeVector(wf0, wf1, ct0, ct1);
        OnExport(vs, type, patient, {headers});
      }
    }
  }

  if (m_exams[0].cte && m_exams[1].cte) {
    auto &ct0 = m_exams[0].cte;
    auto &ct1 = m_exams[1].cte;

    if (ct0->m_Header.IsOD() == ct1->m_Header.IsOD()) {
      auto wf0 = batch::LoadFellowExam(m_exams[0].cte);
      auto wf1 = batch::LoadFellowExam(m_exams[1].cte);

      if (wf0 && wf1) {
        ViewServices vs(wf0.get(), wf1.get(), ct0.get(), ct1.get(), patient);

        auto headers = MakeVector(wf0, wf1, ct0, ct1);
        OnExport(vs, type, patient, {headers});
      }
    }
  }
}
