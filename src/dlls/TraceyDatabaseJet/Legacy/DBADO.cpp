//***************************************************************************************

#include "StdAfx.h"
#include "DB.h"

//***************************************************************************************

CDB     DB;
CWinApp App;

//***************************************************************************************

#define TDB_API extern "C" __declspec(dllexport)

//***************************************************************************************

//TDB_API BOOL __cdecl Connect(const char *FolderName, const char *Password, const char *LocalFolderName,
//                             const BOOL ReadOnly) // 6.1.0 add LocalFolderName and Merge
//{
//  UNREFERENCED_PARAMETER(Password);
//
//  ::AfxDaoInit();
//
//  return DB.Open(nullptr, FolderName, LocalFolderName, ReadOnly); // 6.1.0 add LocalFolderName and Merge
//}

TDB_API void __cdecl Disconnect()
{
  DB.Close();

  ::AfxDaoTerm();
}

//***************************************************************************************
// 6.1.0
TDB_API BOOL __cdecl ChangeLinker(const char *FolderName)
{
  return DB.ChangeLinker(FolderName);
}
// 6.1.0

//***************************************************************************************
// 6.1.0
TDB_API BOOL __cdecl CreateLinker(const char *FileName, const char *LinkerFolderName)
{
  return DB.CreateLinker(FileName, LinkerFolderName);
}
// 6.1.0
//***************************************************************************************

TDB_API BOOL __cdecl IsReadOnly()
{
  return DB.IsReadOnly();
}

//***************************************************************************************

TDB_API int __cdecl GetGroupsCount()
{
  return DB.GetGroupsCount();
}

TDB_API BOOL __cdecl LoadGroups(TDB_GROUP *pGroups)
{
  return DB.LoadGroups(pGroups);
}

TDB_API BOOL __cdecl SaveGroup(TDB_GROUP *pGroup)
{
  return DB.SaveGroup(pGroup);
}

//***************************************************************************************

TDB_API int __cdecl GetClinicsCount()
{
  return DB.GetClinicsCount();
}

TDB_API BOOL __cdecl LoadClinics(TDB_CLINIC *pClinics)
{
  return DB.LoadClinics(pClinics);
}

TDB_API BOOL __cdecl SaveClinic(TDB_CLINIC *pClinic)
{
  return DB.SaveClinic(pClinic);
}

//[cjf***05082012]
TDB_API BOOL __cdecl DeleteClinic(const GUID &ClinicID)
{
  return DB.DeleteClinic(ClinicID);
}
//[cjf***05082012]
//***************************************************************************************

TDB_API int __cdecl GetPhysiciansCount()
{
  return DB.GetPhysiciansCount();
}

TDB_API BOOL __cdecl LoadPhysicians(TDB_PHYSICIAN *pPhysicians)
{
  return DB.LoadPhysicians(pPhysicians);
}

TDB_API BOOL __cdecl SavePhysician(TDB_PHYSICIAN *pPhysician)
{
  return DB.SavePhysician(pPhysician);
}

//***************************************************************************************

TDB_API int __cdecl GetOperatorsCount()
{
  return DB.GetOperatorsCount();
}

TDB_API BOOL __cdecl LoadOperators(TDB_OPERATOR *pOperators)
{
  return DB.LoadOperators(pOperators);
}

TDB_API BOOL __cdecl SaveOperator(TDB_OPERATOR *pOperator)
{
  return DB.SaveOperator(pOperator);
}

//***************************************************************************************

TDB_API int __cdecl GetPatientsCount()
{
  return DB.GetPatientsCount();
}

TDB_API BOOL __cdecl LoadPatients(TDB_PATIENT *pPatients)
{
  return DB.LoadPatients(pPatients);
}

TDB_API BOOL __cdecl SavePatient(TDB_PATIENT *pPatient)
{
  return DB.SavePatient(pPatient);
}

TDB_API BOOL __cdecl DeletePatient(const GUID &PatientID)
{
  return DB.DeletePatient(PatientID);
}

//***************************************************************************************

TDB_API int __cdecl GetEyesCount(const GUID &PatientID)
{
  return DB.GetEyesCount(PatientID);
}

TDB_API BOOL __cdecl LoadEyes(const GUID &PatientID, TDB_EYE *pEyes)
{
  return DB.LoadEyes(PatientID, pEyes);
}

TDB_API BOOL __cdecl SaveEye(TDB_EYE *pEye)
{
  return DB.SaveEye(pEye);
}

//***************************************************************************************

TDB_API int __cdecl GetExamsCount(const GUID &PatientID)
{
  return DB.GetExamsCount(PatientID);
}

TDB_API BOOL __cdecl LoadExamHeaders(const GUID &PatientID, TDB_EXAM_HEADER *pExamHeaders)
{
  return DB.LoadExamHeaders(PatientID, pExamHeaders);
}

TDB_API BOOL __cdecl SaveExamHeader(TDB_EXAM_HEADER *pExamHeader)
{
  return DB.SaveExamHeader(pExamHeader);
}

TDB_API int __cdecl GetExamSize(const GUID &ExamID)
{
  return DB.GetExamSize(ExamID);
}

TDB_API BOOL __cdecl LoadExam(const GUID &ExamID, void *pExam)
{
  return DB.LoadExam(ExamID, pExam);
}

TDB_API BOOL __cdecl SaveExam(const GUID &ExamID, void *pExam, const int ExamSize)
{
  return DB.SaveExam(ExamID, pExam, ExamSize);
}

TDB_API BOOL __cdecl DeleteExam(const GUID &ExamID)
{
  return DB.DeleteExam(ExamID);
}

//***************************************************************************************

TDB_API int __cdecl GetCalibrationSize(const GUID &CalibrationID)
{
  return DB.GetCalibrationSize(CalibrationID);
}

TDB_API BOOL __cdecl LoadCalibration(const GUID &CalibrationID, void *pCalibration)
{
  return DB.LoadCalibration(CalibrationID, pCalibration);
}

TDB_API BOOL __cdecl SaveCalibration(const GUID &CalibrationID, void *pCalibration, const int CalibrationSize)
{
  return DB.SaveCalibration(CalibrationID, pCalibration, CalibrationSize);
}

//***************************************************************************************
