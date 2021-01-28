#pragma once

#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>
#include <string>
#include <filesystem>
#include <gsl/gsl_assert>

namespace fs = std::filesystem;

class DicomSqlite
{
public:
  struct Data
  {
    using DateTime = dicom::DateTime;

    std::string patientID;
    std::string patientName;
    std::string accessionNumber;
    std::string modality;
    std::string requestingPhysician;
    std::string referringPhysician;
    std::string studyInstanceUID;
    DateTime    studyTime;
    Guid        guid;
  };

  DicomSqlite();
  DicomSqlite(const fs::path &dbName);

  void SaveWorkItem(Data data);
  void SaveWorkItem(const dicom::Patient &patient, const dicom::Work &work);

  void Insert(const Data &data);
  auto Query(const char *patID) -> std::vector<Data>;
  auto Query(const Guid &guid) -> std::optional<Data>;

  auto QueryMostRecent(const char *patID) -> std::optional<Data>;

private:
  std::unique_ptr<SQLite::Database> m_db;

  auto TableExists() -> bool;
  auto QueryImpl(SQLite::Statement &query) const -> Data;
};
