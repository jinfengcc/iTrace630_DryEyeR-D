#include "pch.h"
#include "TraceyDicomSend.h"
#include "libs/DicomLib/DicomFileWrite.h"
#include "libs/DicomLib/DicomLog.h"
#include "AccessionDialog.h"

TraceyDicomSend::TraceyDicomSend()
  : Implements<ITraceyDicomSend>("ITraceyDicomSend")
{
}

bool TraceyDicomSend::SetPatient(const Patient &pt)
{
  TRACEY_THROW_IF(pt.patientID.IsEmpty(), "The patient does not have an ID (Chart #) and may not be exported");

  m_patient = std::make_unique<Patient>(pt);
  return GetAccessionEtc();
}

bool TraceyDicomSend::SendToServer(const Work &work, const cv::Mat &image)
{
  auto cfg = CreateObj<ITraceyDicomConfig>();
  return SendToServer(cfg.get(), work, image);
}

bool TraceyDicomSend::SendToServer(ITraceyDicomConfig *cfg, const Work &work_, const cv::Mat &image)
{
  TRACEY_THROW_IF(m_patient == nullptr, "Need to call SetPatient first");

  if (!cfg->IsValid()) {
    return false;
  }

  try {
    DicomSend ds(cfg->Get());

    Work work = work_;

    UpdateAccessionEtc(work);

    ds.Insert(*m_patient);
    ds.Insert(work);
    ds.Insert(image);

    return ds.SendToServer();
  }
  catch (const std::exception &e) {
    DCMLOG_Error("Dicom SQLite (Insert): {}", e.what());
    throw;
  }
}

bool TraceyDicomSend::SendToFile(const Path &path, const Patient &pt, const Work &work, const cv::Mat &image)
{
  DicomFileWriter dfw;

  dfw.Insert(pt);
  dfw.Insert(work);
  dfw.Insert(image);

  dfw.Save(path);

  return true;
}

bool TraceyDicomSend::GetAccessionEtc()
{
  DicomSqlite sql;

  auto patID = CStringA(m_patient->patientID);

  auto pat = sql.QueryMostRecent(patID.GetString());
  if (pat.has_value() && !pat->accessionNumber.empty()) {
    m_accessionNumber     = pat->accessionNumber.c_str();
    m_studyInstanceUID    = pat->studyInstanceUID.c_str();
    m_modality            = pat->modality.c_str();
    m_requestingPhysician = pat->requestingPhysician.c_str();
    m_referringPhysician  = pat->referringPhysician.c_str();
    m_studyTime           = pat->studyTime;

    return true;
  }
  else {
    m_accessionNumber.Empty();
    m_studyInstanceUID.Empty();
    m_requestingPhysician.Empty();
    m_studyTime = {};
  }

  CAccessionDialog dlg(*m_patient);
  if (dlg.DoModal() != IDOK)
    return false;

  m_accessionNumber = dlg.AccessionNumber();

  auto name = CStringA(m_patient->name.last + L", " + m_patient->name.first);
  DCMLOG_Warn("DicomSend: User entered AccessionNumber '{}' for patient: {}", m_accessionNumber.GetString(), name.GetString());

  auto dsd = DicomSqlite::Data{.patientID = patID.GetString(), .accessionNumber = m_accessionNumber.GetString()};

  sql.SaveWorkItem(dsd);
  return true;
}

void TraceyDicomSend::UpdateAccessionEtc(dicom::Work &work)
{
  work.accession           = m_accessionNumber;
  work.studyInstanceUID    = m_studyInstanceUID;
  work.studyTime           = m_studyTime;
  work.requestingPhysician = m_requestingPhysician;
  work.referringPhysician  = m_referringPhysician;

  // modality is always OPM
#if 1
  work.modality = "OPM";
#else
  if (!m_modality.IsEmpty())
    work.modality = m_modality;

  // Modality may not be empty
  if (work.modality.IsEmpty())
    work.modality = "OPM";
#endif
}
