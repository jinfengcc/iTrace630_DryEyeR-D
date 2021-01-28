#pragma once

#include "BatchExport.h"

class BatchExportDicom : public BatchExport
{
public:
  BatchExportDicom();

  bool Export();

private:
  std::uint32_t m_selection{};

  bool          OnExport(const ViewServices &vs, ViewType vt, const CPatient *patient, std::span<const CExamHeader *> exams) override;
  BatchSettings Settings() const override;

  bool        ConfigDicom();
};
