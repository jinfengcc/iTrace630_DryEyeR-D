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

