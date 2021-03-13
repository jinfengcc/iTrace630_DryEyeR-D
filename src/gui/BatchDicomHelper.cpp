#include "stdafx.h"
#include "BatchDicomHelper.h"
#include "libs/DicomLib/DicomQuery.h"
#include "BatchSettings.h"
#include "AppVersion.h"

BatchDicomHelper::BatchDicomHelper(const CPatient *pat, std::span<const CExamHeader *> exams)
  : m_patient(pat)
  , m_exams(exams)
{
  if (!(exams.size() == 1 || exams.size() == 2 || exams.size() == 4)) {
    TRACEY_THROW("An export file may contain 1, 2 or 4 exams");
  }
}

bool BatchDicomHelper::SendToServer(const ViewServices &vs, ViewType vt) const
{
  try {
    if (auto img = vs.Save(vt); !img.empty()) {
      auto patient = DcmPatient();
      auto work    = DcmWork(vt);
      auto dcm     = CreateObj<ITraceyDicomSend>();

      if (!dcm->SetPatient(patient))
        TRACEY_THROW("");

      return dcm->SendToServer(work, img);
    }
    else {
      return true; // Success, for not configured outputs
    }
  }
  catch (const _com_error &e) {
    ThrowDicomComError("DICOM server error", e);
  }
}

bool BatchDicomHelper::SendToFile(const fs::path &file, const ViewServices &vs, ViewType vt) const
{
  try {
    if (auto img = vs.Save(vt); !img.empty()) {
      auto patient = DcmPatient();
      auto work    = DcmWork(vt);
      auto dcm     = CreateObj<ITraceyDicomSend>();

      return dcm->SendToFile(file, patient, work, img);
    }
    else {
      return true; // Success, for not configured outputs
    }
  }
  catch (const _com_error &e) {
    ThrowDicomComError("DICOM error: {}", e);
  }
}

dicom::Patient BatchDicomHelper::DcmPatient() const
{
  ATLASSERT(m_patient != nullptr);
  static_assert(dicom::Sex::Other == static_cast<dicom::Sex>(SEX_UNKNOWN));
  static_assert(dicom::Sex::Male == static_cast<dicom::Sex>(SEX_MALE));
  static_assert(dicom::Sex::Female == static_cast<dicom::Sex>(SEX_FEMALE));

  auto name = dicom::Name{.last = m_patient->m_LastName, .first = m_patient->m_FirstName};
  auto bdy  = dicom::Date{.year = m_patient->m_BirthYear, .month = m_patient->m_BirthMonth, .day = m_patient->m_BirthDay};

  return dicom::Patient{
    .name = name, .patientID = m_patient->m_CustomID, .birthDate = bdy, .sex = static_cast<dicom::Sex>(m_patient->m_Sex)};
}

dicom::Work BatchDicomHelper::DcmWork(ViewType vt) const
{
  dicom::Work work{};

  ATLASSERT(!m_exams.empty());
  if (auto exam = m_exams.front()) {
    work.seriesTime.year   = exam->m_Year;
    work.seriesTime.month  = exam->m_Month;
    work.seriesTime.day    = exam->m_Day;
    work.seriesTime.hour   = exam->m_Hour;
    work.seriesTime.minute = exam->m_Minute;
    work.seriesTime.second = exam->m_Second;
    work.imageLaterality   = exam->IsOD() ? "R" : "L";
    work.softwareVersion  = fmt::format("{}.{}.{}.{}", VERSION_MAJOR_STR, VERSION_MINOR_STR, VERSION_YEAR_STR, VERSION_DAY_STR).c_str();

    if (vt == ViewType::_WFCT_OUOverview)
      work.imageLaterality = "B";

    work.modality             = "OPM";
    work.assentionNumber      = "1234";
    work.procedureDescription = "Test procedure";
    work.seriesInstanceUID    = "1.2.3.4.5.6.7";

    work.seriesDescription = BatchSettings::BatchLabel(vt, false);
  }

  return work;
}
