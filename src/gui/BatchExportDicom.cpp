#include "stdafx.h"
#include "BatchExportDicom.h"
#include "BatchDicomHelper.h"

BatchExportDicom::BatchExportDicom()
{
}

bool BatchExportDicom::Export()
{
  return ConfigDicom() ? BatchExport::Export() : false;
}

bool BatchExportDicom::OnExport(const ViewServices &vs, ViewType vt, const CPatient *patient, std::span<const CExamHeader *> exams)
{
  BatchDicomHelper bdh(patient, exams);
  bdh.SendToServer(vs, vt);

  return true;
}

BatchSettings BatchExportDicom::Settings() const
{
  return BatchSettings(BatchSettings(m_selection));
}

bool BatchExportDicom::ConfigDicom()
{
  auto dcc = CreateObj<ITraceyDicomConfig>();

  if (!dcc->IsValid()) {
    if (!dcc->Change(AfxGetMainWnd()->GetSafeHwnd()))
      return false;
  }

  m_selection = dcc->Get().selectionFlags;

  return true;
}

