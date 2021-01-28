#pragma once

#include <string>
#include <filesystem>
#include "DicomBase.h"

class DicomQuery : DicomBase
{
public:
  using WorkList = dicom::WorkList;
  using Config   = dicom::Config;

  DicomQuery(const Config &cfg);
  DicomQuery(const std::filesystem::path &path);

  struct Filter
  {
    dicom::Date dateFrom;
    dicom::Date dateTo;
    CString     patientID;
    CString     firstName;
    CString     lastName;
    CString     accession;
    CString     modality;
  };

  WorkList Query(const Filter *qf = nullptr) const;

private:
  using DateTime        = dicom::DateTime;
  using Patient         = dicom::Patient;
  using WorkItems       = dicom::WorkItems;
  using Work            = dicom::Work;
  using IDCXOBJ         = rzdcxLib::IDCXOBJ;
  using IDCXOBJIterator = rzdcxLib::IDCXOBJIterator;

  dicom::Server         m_cfg;
  LogLevel              m_logLevel;
  std::filesystem::path m_path;

  // Server support
  auto LoadFromServer(const Filter *filter) const -> WorkList;
  auto PrepareQuery(const Filter *qf) const -> wil::com_ptr<IDCXOBJ>;
  void AddFilter(IDCXOBJ *obj, const Filter &qf) const;
  auto RunQuery(IDCXOBJ *rp) const -> WorkList;

  // File/Folder support
  auto LoadFromPath() const -> WorkList;
  auto LoadFromFile(const fs::path &file) const -> WorkItems;

  void LoadFromObj(IDCXOBJ *obj, WorkList &pl) const;
  void FilterPatients(WorkList &pl, const Filter *qf) const;
  void SortPatients(WorkList &pl) const;
  auto GetPatient(IDCXOBJ *item) const -> Patient;
  auto GetWork(IDCXOBJ *item) const -> Work;
  auto GetWorkDate(IDCXOBJ *item) const -> DateTime;
};
