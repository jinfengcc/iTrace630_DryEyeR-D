#pragma once

#include <stdexcept>
#include <libs/CommonLib/Implements.h>
#include <libs/CommonLib/Guid.h>
#include "Legacy/DB.h"
#include "dbconfig.h"

class DatabaseJet : public Implements<IDatabaseJet>
{
public:
  DatabaseJet()
    : Implements<IDatabaseJet>("IDatabaseJet")
  {
  }
  ~DatabaseJet()
  {
    Close();
  }

  // Inherited via IDatabaseJet
  void Open() override;
  void Open(const char *json) override;
  bool IsReadOnly() const override;
  void Close() override;

  auto GetGroups() const -> std::vector<PATGROUP> override;
  auto GetGroup(const GUID &id) const -> std::optional<PATGROUP> override;
  bool SaveGroup(const PATGROUP &group) override;
  bool DeleteGoup(const GUID &groupID) override;

  auto GetClinics() const -> std::vector<CLINIC> override;
  bool SaveClinic(const CLINIC &group) override;
  bool DeleteClinic(const GUID &clinicID) override;

  auto GetPhysicians() const -> std::vector<PHYSICIAN> override;
  bool SavePhysician(const PHYSICIAN &physician) override;
  bool DeletePhysician(const GUID &physicianID) override;

  auto GetOperators() const -> std::vector<OPERATOR> override;
  bool SaveOperator(const OPERATOR &oper) override;
  bool DeleteOperator(const GUID &operID) override;

  auto GetPatients() const -> std::vector<PATIENT> override;
  auto GetPatient(const GUID &id) const -> std::optional<PATIENT> override;
  bool SavePatient(const PATIENT &physician) override;
  bool DeletePatient(const GUID &physicianID) override;

  auto GetExamHeaders(const GUID &patientID) const -> std::vector<EXAMHEADER> override;
  bool SaveExamHeader(const EXAMHEADER &exam) override;
  bool DeleteExam(const GUID &examID) override;
  auto GetExamBlob(const GUID &examId) const -> std::vector<BYTE> override;
  auto GetExamBlob(const fs::path &examFile) const -> std::vector<BYTE> override;
  bool SaveExamBlob(const GUID &examId, const void *data, unsigned size) override;

  auto GetEyes(const GUID &patientID) const -> std::vector<EYEDATA> override;
  bool SaveEye(const EYEDATA &exam) override;
  bool DeleteEyeData(const GUID &eyeID) override;

  // Calibration
  auto GetCalibrationBlob(const GUID &calibrationId) const -> std::vector<BYTE> override;
  auto GetCalibrationBlob(const fs::path &calibrationFile) const -> std::vector<BYTE> override;
  bool SaveCalibrationBlob(const GUID &calibrationId, const void *data, unsigned size) override;

private:
  CDB      m_legacyDb;
  DBCONFIG m_dbConfig;

  void OpenImpl(const json &js);
  auto GetPatientsImpl(const wchar_t *filter) const -> std::vector<PATIENT>;

  static void Check(bool ok, const char *msg)
  {
    if (!ok)
      throw std::runtime_error(msg);
  }

  fs::path GetExamBlobFile(const GUID &examId) const
  {
    auto path = fs::path(m_dbConfig.examFolder) / Guid(examId).string();
    path.replace_extension(".exam");
    return path;
  }
  fs::path GetCalibrationBlobFile(const GUID &calibrationId) const
  {
    auto path = fs::path(m_dbConfig.calsFolder) / Guid(calibrationId).string();
    path.replace_extension(".cal");
    return path;
  }
};
