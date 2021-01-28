#include "pch.h"
#include "DicomSqlite.h"
#include <array>
#include "DicomLog.h"

namespace {
  struct Column
  {
    const char *name;
    const char *type;
  };

  const char   tableName[] = "Patient";
  const Column columns[]   = {
    // clang-format off
    {"PatientGuid"         , "TEXT PRIMARY KEY"},
    {"PatientID"           , "TEXT"            },
    {"Patient"             , "TEXT"            },
    {"RequestingPhysician" , "TEXT"            },
    {"ReferringPhysician"  , "TEXT"            },
    {"Accession"           , "TEXT"            },
    {"Modality"            , "TEXT"            },
    {"StudyInstanceUID"    , "TEXT"            },
    {"StudyTime"           , "DATETIME"        },
    {"Other"               , "TEXT"            },
    // clang-format on
  };

  enum class Col { guid, id, patName, reqPhysician, refPhysician, accession, modality, studyUID, studyTime, other };

  inline auto CurrentDateTime()
  {
    auto now = COleDateTime::GetCurrentTime();
    return dicom::DateTime{now.GetYear(), now.GetMonth(), now.GetDay(), now.GetHour(), now.GetMinute(), now.GetSecond()};
  }

  inline auto Date2Str(const dicom::DateTime &t)
  {
    return fmt::format("{}-{}-{} {}:{}:{}", t.year, t.month, t.day, t.hour, t.minute, t.second);
  }
  inline auto Str2Date(std::string_view s)
  {
    dicom::DateTime t{};
    if (sscanf_s(s.data(), "%d-%d-%d %d:%d:%d", &t.year, &t.month, &t.day, &t.hour, &t.minute, &t.second) != 6)
      t = {};

    return t;
  }
} // namespace

DicomSqlite::DicomSqlite()
  : DicomSqlite(app::GetAppPath(app::Type::dicomSqlite))
{
}

DicomSqlite::DicomSqlite(const fs::path &dbName)
{
  try {
    if (!fs::exists(dbName))
      DCMLOG_Warn("Dicom: Creating Database {}", dbName.string());

    m_db = std::make_unique<SQLite::Database>(dbName.string(), SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    if (!TableExists()) {
      fmt::memory_buffer buf;
      fmt::format_to(buf, "CREATE TABLE {} (", tableName);
      for (unsigned i = 0; i < _countof(columns); ++i) {
        fmt::format_to(buf, "{} {}", columns[i].name, columns[i].type);
        fmt::format_to(buf, "{}", i == _countof(columns) - 1 ? ")" : ",");
      }

      DCMLOG_Warn("Dicom: Creating Database Table {}", tableName);
      m_db->exec(fmt::to_string(buf).c_str());
    }
  }
  catch (const std::exception &e) {
    DCMLOG_Error("Dicom SQLite: {}", e.what());
    throw;
  }
}

void DicomSqlite::SaveWorkItem(Data data)
{
  // Do not store records with no accession number
  if (data.accessionNumber.empty())
    return;

  // do not save duplicate patiendID, accession number
  auto previous = Query(data.patientID.c_str());
  for (auto &p : previous) {
    if (p.accessionNumber == data.accessionNumber) {
      return;
    }
  }

  data.studyTime = CurrentDateTime();
  data.guid      = Guid::NewGuid();

  Insert(data);
}

void DicomSqlite::SaveWorkItem(const dicom::Patient &patient, const dicom::Work &work)
{
  auto Str = [](const CString &s) {
    CStringA sa(s);
    return std::string(sa.GetString());
  };

  Data data{};
  data.patientID           = Str(patient.patientID);
  data.patientName         = Str(patient.name.last + L"^" + patient.name.first);
  data.accessionNumber     = Str(work.accession);
  data.modality            = Str(work.modality);
  data.studyInstanceUID    = Str(work.studyInstanceUID);
  data.requestingPhysician = Str(work.requestingPhysician);
  data.referringPhysician  = Str(work.referringPhysician);

  SaveWorkItem(data);
}

void DicomSqlite::Insert(const Data &data)
{
  try {
    auto guid = data.guid ? data.guid.string() : Guid::NewGuid().string();

    auto _C = [](Col c) -> int { return static_cast<int>(c) + 1; };

    SQLite::Statement query(*m_db, "INSERT INTO Patient VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

    // clang-format off
    query.bind( _C(Col::guid        ), data.guid.string()      );
    query.bind( _C(Col::id          ), data.patientID          );
    query.bind( _C(Col::patName     ), data.patientName        );
    query.bind( _C(Col::reqPhysician), data.requestingPhysician);
    query.bind( _C(Col::refPhysician), data.referringPhysician );
    query.bind( _C(Col::accession   ), data.accessionNumber    );
    query.bind( _C(Col::modality    ), data.modality           );
    query.bind( _C(Col::studyUID    ), data.studyInstanceUID   );
    query.bind( _C(Col::studyTime   ), Date2Str(data.studyTime));
    query.bind( _C(Col::other       ), ""                      );
    // clang-format on

    query.exec();
  }
  catch (const std::exception &e) {
    DCMLOG_Error("Dicom SQLite (Insert): {}", e.what());
    throw;
  }
}

auto DicomSqlite::Query(const char *patID) -> std::vector<Data>
{
  try {
    std::vector<Data> data;

    fmt::memory_buffer buf;
    fmt::format_to(buf, "SELECT * FROM Patient ");
    if (patID) {
      fmt::format_to(buf, "WHERE PatientID = '{}' ", patID);
      fmt::format_to(buf, "ORDER BY StudyTime DESC");
    }

    SQLite::Statement query(*m_db, fmt::to_string(buf));
    while (query.executeStep()) {
      data.push_back(QueryImpl(query));
    }

    return data;
  }
  catch (const std::exception &e) {
    DCMLOG_Error("Dicom SQLite (Insert): {}", e.what());
    throw;
  }
}

auto DicomSqlite::Query(const Guid &guid) -> std::optional<Data>
{
  try {
    SQLite::Statement query(*m_db, "SELECT * FROM Patient WHERE PatientGuid = ?");
    SQLite::bind(query, guid.string());

    if (query.executeStep()) {
      auto data = QueryImpl(query);
      ATLASSERT(!query.executeStep());
      return data;
    }

    return {};
  }
  catch (const std::exception &e) {
    DCMLOG_Error("Dicom SQLite (Insert): {}", e.what());
    throw;
  }
}

auto DicomSqlite::QueryMostRecent(const char *patID) -> std::optional<Data>
{
  Expects(patID != nullptr);

  try {
    SQLite::Statement query(*m_db, "SELECT * FROM Patient WHERE PatientID = ?");
    SQLite::bind(query, patID);

    if (query.executeStep()) {
      auto data = QueryImpl(query);
      return data;
    }

    return {};
  }
  catch (const std::exception &e) {
    DCMLOG_Error("Dicom SQLite (Insert): {}", e.what());

    return {};
  }
}

auto DicomSqlite::TableExists() -> bool
{
  if (!m_db->tableExists(tableName))
    return false;

  try {
    SQLite::Statement query(*m_db, "SELECT * FROM Patient");
    int               n = 0;
    for (auto &c : columns) {
      auto col = query.getColumnName(n++);
      if (strcmp(col, c.name) != 0)
        throw std::runtime_error("Wrong col name");
    }
    return true;
  }
  catch (const std::exception &) {
  }

  m_db->exec("DROP TABLE IF EXISTS Patient");
  return false;
}

auto DicomSqlite::QueryImpl(SQLite::Statement &query) const -> Data
{
  auto Str = [&](Col n) {
    auto c = query.getColumn(static_cast<int>(n));
    return c.getString();
  };

  Data data;

  data.guid                = Guid(Str(Col::guid));
  data.patientID           = Str(Col::id);
  data.patientName         = Str(Col::patName);
  data.accessionNumber     = Str(Col::accession);
  data.modality            = Str(Col::modality);
  data.studyInstanceUID    = Str(Col::studyUID);
  data.requestingPhysician = Str(Col::reqPhysician);
  data.referringPhysician  = Str(Col::refPhysician);
  data.studyTime           = Str2Date(Str(Col::studyTime));

  return data;
}
