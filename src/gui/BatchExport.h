#pragma once

#include "BatchHelper.h"
#include <span>

class BatchExport
{
public:
  BatchExport();

  bool Export();

protected:
private:
  virtual bool          OnExport(const ViewServices &vs, ViewType vt, const CPatient *patient, std::span<const CExamHeader *> exams) = 0;
  virtual BatchSettings Settings() const                                                                                             = 0;

private:
  using ExamPair = batch::ExamPair;

  CPatient *            patient{};
  std::vector<ExamPair> m_exams;
};
