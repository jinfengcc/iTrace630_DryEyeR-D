#pragma once

#include <span>
#include "Data.h"
#include <interfaces/ITraceyDicom.h>
#include "ViewServices.h"

class BatchDicomHelper
{
public:
  BatchDicomHelper(const CPatient *pat, std::span<const CExamHeader *> exams);

  bool SendToServer(const ViewServices &vs, ViewType vt) const;

  bool SendToFile(const fs::path &file, const ViewServices &vs, ViewType vt) const;

private:
  const CPatient *               m_patient;
  std::span<const CExamHeader *> m_exams;

  dicom::Patient DcmPatient() const;
  dicom::Work    DcmWork(ViewType vt) const;
};

