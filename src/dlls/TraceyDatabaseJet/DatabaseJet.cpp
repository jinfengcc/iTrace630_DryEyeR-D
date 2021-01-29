#include "stdafx.h"
#include "DatabaseJet.h"
#include "JetCasts.h"
#include "dbconfig.h"

#include <memory>
#include <algorithm>
#include <libs/CommonLib/AppClassFactory.h>
#include <libs/CommonLib/Compression/Blob.h>
#include <libs/CommonLib/JsonSettings.h>

namespace {
  const char json_name[] = "DatabaseJet";
}

void DatabaseJet::Open()
{
  JsonSettings js;
  OpenImpl(js.Get(json_name));
}

void DatabaseJet::Open(const char *json)
{
  auto js  = json::parse(json).at(json_name);
  OpenImpl(js);
}

void DatabaseJet::OpenImpl(const json &js)
{
  m_dbConfig = js.get<DBCONFIG>();

  ::AfxDaoInit();

  auto b = m_legacyDb.Open(m_dbConfig);
  Check(b, "Unable to open database");
}

void DatabaseJet::Close()
{
  m_legacyDb.Close();
  ::AfxDaoTerm();
}

bool DatabaseJet::IsReadOnly() const
{
  return m_legacyDb.IsReadOnly() == TRUE;
}

///////////////////////////////////////////////////////////////////////////////

auto DatabaseJet::GetGroups() const -> std::vector<PATGROUP>
{
  if (auto n = m_legacyDb.GetGroupsCount(); n > 0) {
    std::vector<PATGROUP> data(static_cast<unsigned>(n));
    m_legacyDb.LoadGroups(&data[0]);
    std::sort(data.begin(), data.end());
    return data;
  }
  return std::vector<PATGROUP>();
}

auto DatabaseJet::GetGroup(const GUID &id) const -> std::optional<PATGROUP>
{
  auto x = GetGroups();
  auto i = std::find_if(x.begin(), x.end(), [&](const auto &g) { return id == g.GroupID; });
  if (i == x.end())
    return {};
  else
    return {*i};
}

bool DatabaseJet::SaveGroup(const PATGROUP &group)
{
  if (IsReadOnly())
    TRACEY_THROW("Database is read-only. Cannot Save Group.");

  return m_legacyDb.SaveGroup(&group) == TRUE;
}

bool DatabaseJet::DeleteGoup(const GUID &groupID)
{
  UNREFERENCED_PARAMETER(groupID);
  TRACEY_THROW_BUG("DeleteGoup is not implemented");
}

///////////////////////////////////////////////////////////////////////////////

auto DatabaseJet::GetClinics() const -> std::vector<CLINIC>
{
  if (auto n = m_legacyDb.GetClinicsCount(); n > 0) {
    std::vector<TDB_CLINIC> data(static_cast<unsigned>(n));
    m_legacyDb.LoadClinics(&data[0]);
    std::sort(data.begin(), data.end());
    return data;
  }
  return {};
}

bool DatabaseJet::SaveClinic(const CLINIC &clinic)
{
  if (IsReadOnly())
    TRACEY_THROW("Database is read-only. Cannot Save Clinic.");

  return m_legacyDb.SaveClinic(&clinic) == TRUE;
}

bool DatabaseJet::DeleteClinic(const GUID &clinicID)
{
  if (IsReadOnly())
    TRACEY_THROW("Database is read-only. Cannot Delete Clinic.");

  return m_legacyDb.DeleteClinic(clinicID) == TRUE;
}

///////////////////////////////////////////////////////////////////////////////

auto DatabaseJet::GetPhysicians() const -> std::vector<PHYSICIAN>
{
  if (auto n = m_legacyDb.GetPhysiciansCount(); n > 0) {
    std::vector<TDB_PHYSICIAN> data(static_cast<unsigned>(n));
    m_legacyDb.LoadPhysicians(&data[0]);
    std::sort(data.begin(), data.end());
    return data;
  }
  return {};
}

bool DatabaseJet::SavePhysician(const PHYSICIAN &physician)
{
  if (IsReadOnly())
    TRACEY_THROW("Database is read-only. Cannot save physician.");

  return m_legacyDb.SavePhysician(&physician) == TRUE;
}

bool DatabaseJet::DeletePhysician(const GUID &physicianID)
{
  UNREFERENCED_PARAMETER(physicianID);
  TRACEY_THROW_BUG("DeletePhysician is not implemented");
}

///////////////////////////////////////////////////////////////////////////////

auto DatabaseJet::GetOperators() const -> std::vector<OPERATOR>
{
  if (auto n = m_legacyDb.GetOperatorsCount(); n > 0) {
    std::vector<TDB_OPERATOR> data(static_cast<unsigned>(n));
    m_legacyDb.LoadOperators(&data[0]);
    std::sort(data.begin(), data.end());
    return data;
  }
  return {};
}

bool DatabaseJet::SaveOperator(const OPERATOR &oper)
{
  if (IsReadOnly())
    TRACEY_THROW("Database is read-only. Cannot save operator.");

  return m_legacyDb.SaveOperator(&oper) == TRUE;
}

bool DatabaseJet::DeleteOperator(const GUID &operID)
{
  UNREFERENCED_PARAMETER(operID);
  TRACEY_THROW_BUG("DeleteOperator is not implemented");
}

///////////////////////////////////////////////////////////////////////////////

auto DatabaseJet::GetPatients() const -> std::vector<PATIENT>
{
  return GetPatientsImpl(L"");
}

auto DatabaseJet::GetPatient(const GUID &id) const -> std::optional<PATIENT>
{
  auto filter = fmt::format(L"PatientID = {}", Guid(id).wstring());
  if (auto pats = GetPatientsImpl(filter.c_str()); pats.size() == 1)
    return {pats[0]};
  else
    return {};
}

auto DatabaseJet::GetPatientsImpl(const wchar_t *filter) const -> std::vector<PATIENT>
{
  if (auto n = m_legacyDb.GetPatientsCount(filter); n > 0) {
    std::vector<TDB_PATIENT> data(static_cast<unsigned>(n));
    m_legacyDb.LoadPatients(&data[0], filter);
    std::sort(data.begin(), data.end());
    return data;
  }
  return {};
}

bool DatabaseJet::SavePatient(const PATIENT &patient)
{
  if (IsReadOnly())
    TRACEY_THROW("Database is read-only. Cannot save patient.");

  return m_legacyDb.SavePatient(&patient) == TRUE;
}

bool DatabaseJet::DeletePatient(const GUID &patientID)
{
  if (IsReadOnly())
    TRACEY_THROW("Database is read-only. Cannot delete patient.");

  return m_legacyDb.DeletePatient(patientID) == TRUE;
}

///////////////////////////////////////////////////////////////////////////////

auto DatabaseJet::GetExamHeaders(const GUID &patientID) const -> std::vector<EXAMHEADER>
{
  if (auto count = m_legacyDb.GetExamsCount(patientID); count > 0) {
    std::vector<EXAMHEADER> data(count);
    m_legacyDb.LoadExamHeaders(patientID, &data[0]);
    std::sort(data.begin(), data.end());
    return data;
  }

  return {};
}

bool DatabaseJet::SaveExamHeader(const EXAMHEADER &exam)
{
  if (IsReadOnly())
    TRACEY_THROW("Database is read-only. Cannot save exam.");

  return m_legacyDb.SaveExamHeader(&exam) == TRUE;
}

auto DatabaseJet::GetExamBlob(const GUID &examId) const -> std::vector<BYTE>
{
  return GetExamBlob(GetExamBlobFile(examId));
}

auto DatabaseJet::GetExamBlob(const fs::path &examFile) const -> std::vector<BYTE>
{
  return Blob::Load(examFile);
}

bool DatabaseJet::DeleteExam(const GUID &examID)
{
  if (IsReadOnly())
    TRACEY_THROW("Database is read-only. Cannot delete exam.");

  return m_legacyDb.DeleteExam(examID) == TRUE;
}

bool DatabaseJet::SaveExamBlob(const GUID &examId, const void *data, unsigned size)
{
  if (IsReadOnly())
    TRACEY_THROW("Database is read-only. Cannot save exam blob.");

  return m_legacyDb.SaveExam(examId, data, size) == TRUE;
}

///////////////////////////////////////////////////////////////////////////////

auto DatabaseJet::GetEyes(const GUID &patientID) const -> std::vector<EYEDATA>
{
  if (auto count = m_legacyDb.GetEyesCount(patientID); count > 0) {
    std::vector<EYEDATA> data(count);
    m_legacyDb.LoadEyes(patientID, &data[0]);
    return data;
  }

  return {};
}

bool DatabaseJet::SaveEye(const EYEDATA &exam)
{
  if (IsReadOnly())
    TRACEY_THROW("Database is read-only. Cannot save eye.");

  return m_legacyDb.SaveEye(&exam) == TRUE;
}

bool DatabaseJet::DeleteEyeData(const GUID &eyeID)
{
  UNREFERENCED_PARAMETER(eyeID);
  TRACEY_THROW_BUG("DeleteEyeData is not implemented");
}

auto DatabaseJet::GetCalibrationBlob(const GUID &calibrationId) const -> std::vector<BYTE>
{
  return GetCalibrationBlob(GetCalibrationBlobFile(calibrationId));
}

auto DatabaseJet::GetCalibrationBlob(const fs::path &calibrationFile) const -> std::vector<BYTE>
{
  return Blob::Load(calibrationFile);
}

bool DatabaseJet::SaveCalibrationBlob(const GUID &calibrationId, const void *data, unsigned size)
{
  if (IsReadOnly())
    TRACEY_THROW("Database is read-only. Cannot save exam blob.");

  return m_legacyDb.SaveCalibration(calibrationId, data, size);
}
