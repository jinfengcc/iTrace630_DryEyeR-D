#include "pch.h"
#include "DicomQuery.h"
#include "DicomQuery_.h"
#include "DicomDateTime.h"

DicomQuery::DicomQuery(const Config &cfg)
  : DicomBase(LogLevel::Warn)
  , m_cfg(cfg.mwl)
{
}

DicomQuery::DicomQuery(const std::filesystem::path &path)
  : DicomBase(LogLevel::Warn)
  , m_path(path)
{
}

auto DicomQuery::Query(const Filter *filter) const -> WorkList
{
  bool loadPath = fs::is_directory(m_path) || fs::is_regular_file(m_path);
  auto pats     = loadPath ? LoadFromPath() : LoadFromServer(filter);

  // Filter and sort patients;
  FilterPatients(pats, filter);
  SortPatients(pats);

  // Erase patients without work items
  std::erase_if(pats, [](const WorkItems &p) { return p.items.empty(); });

  DCMLOG_Info("Query: Items received = {}", pats.size());

  return pats;
}

auto DicomQuery::LoadFromServer(const Filter *filter) const -> WorkList
{
  auto obj = PrepareQuery(filter);
  return RunQuery(obj.get());
}

auto DicomQuery::PrepareQuery(const Filter *qf) const -> wil::com_ptr<IDCXOBJ>
{
  // clang-format off
  std::array requestTags = {
    rzdcxLib::patientName      ,
    rzdcxLib::patientID        ,
    rzdcxLib::OtherPatientIDs  ,
    rzdcxLib::PatientBirthDate ,
    rzdcxLib::PatientSex       ,

    rzdcxLib::SeriesDate                    ,
    rzdcxLib::SeriesTime                    ,
    rzdcxLib::RequestingPhysician           ,
    rzdcxLib::ReferringPhysicianName        ,
    rzdcxLib::AccessionNumber               ,
    rzdcxLib::RequestedProcedureDescription ,
    rzdcxLib::ScheduledStationAETitle       ,
    rzdcxLib::studyInstanceUID              ,
    rzdcxLib::StudyDate                     ,
    rzdcxLib::StudyTime                     ,
    rzdcxLib::StudyDescription              ,
    rzdcxLib::Modality                      ,

    rzdcxLib::ScheduledStationAETitle,
    rzdcxLib::PerformedStationAETitle,
    rzdcxLib::ReceivingAE,
    rzdcxLib::RequestingAE,
    rzdcxLib::DestinationAE,

  };
  // clang-format on

  auto rzdcx = RZDCX::Instance();
  auto obj   = rzdcx->CreateInstance<IDCXOBJ>();

  for (auto t : requestTags)
    InsertElement(obj.get(), t);

  // Special handling for Modality, etc
  if (1) {
    auto temp = rzdcx->CreateInstance<IDCXOBJ>();

    InsertElement(temp.get(), rzdcxLib::Modality);
    InsertElement(temp.get(), rzdcxLib::ScheduledStationAETitle);
    InsertElement(temp.get(), rzdcxLib::ScheduledProcedureStepStartDate);
    InsertElement(temp.get(), rzdcxLib::ScheduledProcedureStepStartTime);
    InsertElement(temp.get(), rzdcxLib::ScheduledPerformingPhysiciansName);
    InsertElement(temp.get(), rzdcxLib::ScheduledProcedureStepDescription);

    auto iter = rzdcx->CreateInstance<IDCXOBJIterator>();

    iter->Insert(temp.get());
    InsertUnknown(obj.get(), rzdcxLib::ScheduledProcedureStepSequence, iter.get());
  }

  if (qf)
    AddFilter(obj.get(), *qf);

  return obj;
}

void DicomQuery::AddFilter(IDCXOBJ *obj, const Filter &qf) const
{
  auto rzdcx = RZDCX::Instance();

  auto ffunc = [](IDCXOBJ *obj, DICOM_TAGS_ENUM tag, const CString &s) {
    if (!s.IsEmpty())
      InsertElement(obj, tag, s + "*");
  };

  {
    auto temp = rzdcx->CreateInstance<IDCXOBJ>();

    InsertElement(temp.get(), rzdcxLib::ScheduledProcedureStepStartTime);
    InsertElement(temp.get(), rzdcxLib::ScheduledProcedureStepDescription); // Ask for SPS description
    InsertElement(temp.get(), rzdcxLib::ScheduledProcedureStepID);          // Ask for SPS ID
    InsertElement(temp.get(), rzdcxLib::Modality, qf.modality);

    if (Valid(qf.dateFrom)) {
      auto dateTo = Valid(qf.dateTo) ? qf.dateTo : NextDay(qf.dateFrom);

      fmt::memory_buffer buf;
      fmt::format_to(buf, "{:04d}{:02d}{:02d}", qf.dateFrom.year, qf.dateFrom.month, qf.dateFrom.day);
      fmt::format_to(buf, "-{:04d}{:02d}{:02d}", dateTo.year, dateTo.month, dateTo.day);
      auto str = fmt::to_string(buf);

      InsertElement(temp.get(), rzdcxLib::ScheduledProcedureStepStartDate, str.c_str());
    }
    else {
      InsertElement(temp.get(), rzdcxLib::ScheduledProcedureStepStartDate);
    }

    // InsertElement(temp.get(), rzdcxLib::AccessionNumber, qf.accession);
    // ffunc(temp.get(), rzdcxLib::patientID, qf.patientID);

    auto iter = rzdcx->CreateInstance<IDCXOBJIterator>();
    iter->Insert(temp.get());

    InsertUnknown(obj, rzdcxLib::ScheduledProcedureStepSequence, iter.get());
  }

  ffunc(obj, rzdcxLib::PatientsName, qf.lastName);
  // ffunc(obj, rzdcxLib::patientID, qf.patientID);
  // ffunc(obj, rzdcxLib::AccessionNumber, qf.accession);
  InsertElement(obj, rzdcxLib::patientID, qf.patientID);

  InsertElement(obj, rzdcxLib::Modality);
  InsertElement(obj, rzdcxLib::ScheduledProcedureStepStartTime);
  InsertElement(obj, rzdcxLib::ScheduledProcedureStepDescription);
  InsertElement(obj, rzdcxLib::ScheduledProcedureStepID);
}

auto DicomQuery::RunQuery(IDCXOBJ *rp) const -> WorkList
{
  try {
    using rzdcxLib::IDCXREQ;

    auto rzdcx = RZDCX::Instance();
    auto req   = rzdcx->CreateInstance<IDCXREQ>();

    {
      auto h = CStringA(m_cfg.host);
      auto a = CStringA(m_cfg.remoteAETitle);
      auto c = CStringA(m_cfg.localAETitle);

      DCMLOG_Info("Query: host = {}, port = {}, AE-Title = {}, local-AE-Title = {}", h.GetString(), m_cfg.port, a.GetString(), c.GetString());
    }

    auto iter = req->Query(m_cfg.localAETitle.GetString(), m_cfg.remoteAETitle.GetString(), m_cfg.host.GetString(), m_cfg.port, MWL_SOP_CLASS_UID, rp);

    WorkList pats;
    for (; !iter->AtEnd(); iter->Next()) {
      auto item = iter->Get();
      LoadFromObj(item, pats);
    }

    return pats;
  }
  catch (const _com_error &e) {
    DCMLOG_Error("Query", e);
    ThrowDicomComError("DICOM error", e);
  }
  catch (const std::exception &e) {
    DCMLOG_Error("Query: {}", e.what());
    throw;
  }
}

auto DicomQuery::LoadFromPath() const -> WorkList
{
  WorkList workList;

  if (fs::is_regular_file(m_path)) {
    auto obj = RZDCX::Instance()->CreateInstance<IDCXOBJ>();
    obj->openFile(m_path.wstring().c_str());
    LoadFromObj(obj.get(), workList);
  }
  else if (fs::is_directory(m_path)) {
    for (fs::recursive_directory_iterator b(m_path), e; b != e; ++b) {
      auto p = b->path();
      if (p.string().ends_with(".dcm")) {
        auto obj = RZDCX::Instance()->CreateInstance<IDCXOBJ>();
        obj->openFile(p.wstring().c_str());
        LoadFromObj(obj.get(), workList);
      }
    }
  }

  return workList;
}

auto DicomQuery::LoadFromFile(const fs::path &file) const -> WorkItems
{
  auto obj = RZDCX::Instance()->CreateInstance<IDCXOBJ>();
  obj->openFile(file.wstring().c_str());

  auto patient = GetPatient(obj.get());
  auto work    = GetWork(obj.get());

  return {patient, {work}};
}

void DicomQuery::LoadFromObj(IDCXOBJ *obj, WorkList &pl) const
{
  if (auto pat = GetPatient(obj); Valid(pat)) {
    auto f = std::find_if(pl.begin(), pl.end(), [&pat](const WorkItems &x) { return x.patient == pat; });
    if (f == pl.end()) {
      pl.push_back({std::move(pat)});
      f = std::prev(pl.end());
    }

    if (auto work = GetWork(obj); Valid(work))
      f->items.push_back(std::move(work));
  }
}

void DicomQuery::FilterPatients(WorkList &res, const Filter *qf) const
{
  if (qf == nullptr)
    return;

  auto match = [](CString value, CString f) -> bool {
    value.MakeLower();
    f.MakeLower();
    return value.Find(f.GetString()) >= 0;
  };

  // if (!qf->modality.IsEmpty()) {
  //  for (auto &p : res)
  //    std::erase_if(p.items, [qf](const Work &w) { return w.modality != qf->modality; });
  //}

  // if (!qf->lastName.IsEmpty())
  //  std::erase_if(res, [&](const WorkItems &w) { return !match(w.patient.name.last, qf->lastName); });

  // if (!qf->firstName.IsEmpty())
  //  std::erase_if(res, [&](const WorkItems &w) { return !match(w.patient.name.first, qf->firstName); });

  if (!qf->patientID.IsEmpty())
    std::erase_if(res, [&](const WorkItems &w) { return !match(w.patient.patientID, qf->patientID); });

  for (auto &p : res) {
    if (!qf->accession.IsEmpty())
      std::erase_if(p.items, [&](const auto &w) { return !match(w.accession, qf->accession); });
  }
}

void DicomQuery::SortPatients(WorkList &pl) const
{
  std::ranges::sort(pl,
                    [](const auto &a, const auto &b) { return a.patient.name.last.CompareNoCase(b.patient.name.last.GetString()) < 0; });

  for (auto &v : pl) {
    std::ranges::sort(v.items, [](const auto &a, const auto &b) { return a.studyTime < b.studyTime; });
  }
}

auto DicomQuery::GetPatient(IDCXOBJ *item) const -> Patient
{
  Patient pat{};

  // clang-format off
  GetElementValue(item, rzdcxLib::patientName      , pat.name           );
  GetElementValue(item, rzdcxLib::patientID        , pat.patientID      );
  GetElementValue(item, rzdcxLib::OtherPatientIDs  , pat.uniquePatientID);
  GetElementValue(item, rzdcxLib::PatientBirthDate , pat.birthDate      );
  GetElementValue(item, rzdcxLib::PatientSex       , pat.sex            );
  // clang-format on

  return pat;
}

auto DicomQuery::GetWork(IDCXOBJ *item) const -> Work
{
  Work work;

  work.studyTime = GetWorkDate(item);

  // clang-format off
  GetElementValue(item, rzdcxLib::ReferringPhysicianName         , work.referringPhysician);
  GetElementValue(item, rzdcxLib::RequestingPhysician            , work.requestingPhysician);
  GetElementValue(item, rzdcxLib::AccessionNumber                , work.assentionNumber);
  GetElementValue(item, rzdcxLib::RequestedProcedureDescription  , work.procedureDescription);
  GetElementValue(item, rzdcxLib::studyInstanceUID               , work.studyInstanceUID);
  //GetElementValue(item, rzdcxLib::StudyDescription               , studyDescription);
  GetElementValue(item, rzdcxLib::AccessionNumber                , work.accession);
  GetElementValue(item, rzdcxLib::Modality                       , work.modality);
  // clang-format on

  DICOM_TAGS_ENUM tags[] = {
    rzdcxLib::ScheduledStationAETitle,
    rzdcxLib::PerformedStationAETitle,
    rzdcxLib::ReceivingAE,
    rzdcxLib::RequestingAE,
    rzdcxLib::DestinationAE,
  };

  CString ss;
  for (auto t : tags)
    GetElementValue(item, t, ss);

  return work;
}

auto DicomQuery::GetWorkDate(IDCXOBJ *item) const -> DateTime
{
  using namespace rzdcxLib;

  // clang-format off
  std::array tagpairs = {
    std::make_pair(StudyDate                      , StudyTime),
    std::make_pair(ScheduledProcedureStepStartDate, ScheduledProcedureStepStartTime),
  };
  // clang-format on

  for (auto &tp : tagpairs) {
    std::wstring str;
    if (!GetElementValue(item, tp.first, str))
      continue;

    DateTime dt{};
    if (!ParseDate(str, dt))
      continue;

    if (GetElementValue(item, tp.second, str))
      ParseTime(str, dt);

    return dt;
  }

  return {};
}
