#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <optional>
#include "TraceyObj.h"

#include <libs/DatabaseLib/DbData.h>

TRACEYOBJ("85BA1187-BBE6-461C-98F0-4A5D853D03D3") IDatabaseJet : public IUnknown
{
  virtual void Open()                 = 0;
  virtual void Open(const char *json) = 0;
  virtual void Close()                = 0;
  virtual bool IsReadOnly() const     = 0;

  // Groups
  virtual std::vector<PATGROUP>   GetGroups() const                = 0;
  virtual std::optional<PATGROUP> GetGroup(const GUID &id) const   = 0;
  virtual bool                    SaveGroup(const PATGROUP &group) = 0;

  // Clinics
  virtual std::vector<CLINIC> GetClinics() const                 = 0;
  virtual bool                SaveClinic(const CLINIC &clinic)   = 0;
  virtual bool                DeleteClinic(const GUID &clinicID) = 0;

  // Physician
  virtual std::vector<PHYSICIAN> GetPhysicians() const                     = 0;
  virtual bool                   SavePhysician(const PHYSICIAN &physician) = 0;

  // Operator
  virtual std::vector<OPERATOR> GetOperators() const               = 0;
  virtual bool                  SaveOperator(const OPERATOR &oper) = 0;

  // Physician
  virtual std::vector<PATIENT>   GetPatients() const                    = 0;
  virtual std::optional<PATIENT> GetPatient(const GUID &id) const       = 0;
  virtual bool                   SavePatient(const PATIENT &physician)  = 0;
  virtual bool                   DeletePatient(const GUID &physicianID) = 0;

  // Exam
  virtual std::vector<EXAMHEADER> GetExamHeaders(const GUID &patientID) const = 0;
  virtual bool                    SaveExamHeader(const EXAMHEADER &exam)      = 0;
  virtual bool                    DeleteExam(const GUID &examID)              = 0;

  // Exam BLOB
  virtual std::vector<BYTE> GetExamBlob(const GUID &examId) const                             = 0;
  virtual std::vector<BYTE> GetExamBlob(const fs::path &examFile) const                       = 0;
  virtual bool              SaveExamBlob(const GUID &examId, const void *data, unsigned size) = 0;

  // Eye
  virtual std::vector<EYEDATA> GetEyes(const GUID &patientID) const = 0;
  virtual bool                 SaveEye(const EYEDATA &exam)         = 0;

  // Calibration BLOB (#todo Return a json string, eventually)
  virtual std::vector<BYTE> GetCalibrationBlob(const GUID &calibrationId) const                             = 0;
  virtual std::vector<BYTE> GetCalibrationBlob(const fs::path &calibrationFile) const                       = 0;
  virtual bool              SaveCalibrationBlob(const GUID &calibrationId, const void *data, unsigned size) = 0;

private:
  virtual bool DeletePhysician(const GUID &physicianID) = 0;
  virtual bool DeleteOperator(const GUID &operID)       = 0;
  virtual bool DeleteGoup(const GUID &groupID)          = 0;
  virtual bool DeleteEyeData(const GUID &eyeID)         = 0;
};

using IDatabase = IDatabaseJet;

// todo: for backward compatibility
using TDB_ADDRESS     = ADDRESS;
using TDB_CLINIC      = CLINIC;
using TDB_GROUP       = PATGROUP;
using TDB_PATIENT     = PATIENT;
using TDB_PHYSICIAN   = PHYSICIAN;
using TDB_OPERATOR    = OPERATOR;
using TDB_EXAM_HEADER = EXAMHEADER;
using TDB_EYE         = EYEDATA;
