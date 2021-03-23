#include "pch.h"
#include "TraceyDicomQuery.h"
#include "WorklistDialog.h"

dicom::Patient  g_mostRecentPatient;
dicom::Work     g_mostRecentWork;

TraceyDicomQuery::TraceyDicomQuery()
  : Implements<ITraceyDicomQuery>("ITraceyDicomQuery")
{
}

bool TraceyDicomQuery::Query(HWND hWndParent)
{
  auto cfg = CreateObj<ITraceyDicomConfig>();
  return Query(cfg.get(), hWndParent);
}

bool TraceyDicomQuery::Query(ITraceyDicomConfig *cfg, HWND hWndParent)
{
  m_patient = nullptr;
  m_work    = nullptr;

  if (!cfg->IsValid()) {
    if (!cfg->ChangeConfig(hWndParent))
      return false;
  }

  auto patient = std::make_unique<dicom::Patient>();
  auto work    = std::make_unique<dicom::Work>();

  CWorklistDialog dlg(cfg, *patient, *work);
  if (DoModal(dlg, hWndParent) != IDOK)
    return false;

  m_patient = std::move(patient);
  m_work    = std::move(work);

  g_mostRecentPatient = *m_patient;
  g_mostRecentWork    = *m_work;

  return true;
}
