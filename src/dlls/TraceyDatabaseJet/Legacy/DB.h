//***************************************************************************************

#pragma once

#include "dbconfig.h"

using TDB_ADDRESS     = ADDRESS;
using TDB_CLINIC      = CLINIC;
using TDB_GROUP       = PATGROUP;
using TDB_PATIENT     = PATIENT;
using TDB_PHYSICIAN   = PHYSICIAN;
using TDB_OPERATOR    = OPERATOR;
using TDB_EXAM_HEADER = EXAMHEADER;
using TDB_EYE         = EYEDATA;


//***************************************************************************************

#if 0
//#pragma pack(push)
//#pragma pack(1)

typedef struct _TDB_ADDRESS {

  char            Line1[51];
  char            Line2[51];
  char            City[51];
  char            State[51]; // Region/province for non-US addresses is stored here
  char            ZIP[21];
  char            Country[51];

} TDB_ADDRESS;

//typedef struct _TDB_GROUP {
//
//  GUID            GroupID;
//  char            Name[51];
//
//} TDB_GROUP__;


typedef struct _TDB_CLINIC {

  GUID            ClinicID;
  char            Name[51];
  TDB_ADDRESS     Address;

} TDB_CLINIC__;

typedef struct _TDB_PHYSICIAN {

  GUID            PhysicianID;
  char            LastName[51];
  char            FirstName[51];

} TDB_PHYSICIAN__;


typedef struct _TDB_OPERATOR {

  GUID            OperatorID;
  char            LastName[51];
  char            FirstName[51];

} TDB_OPERATOR__;


typedef struct _TDB_PATIENT {

  GUID            PatientID;
  GUID            GroupID;
  char            CustomID[51];
  char            LastName[51];
  char            FirstName[51];
  char            MiddleName[51];
  int             BirthYear;
  int             BirthMonth;
  int             BirthDay;
  int             Sex;
  TDB_ADDRESS     Address;
  char            HomePhone[21];
  char            WorkPhone[21];
  char            EMail[51];
  char            Note[101];

  ////[cjf***04302012*003*] 1
  int             RegYear;
  int             RegMonth;
  int             RegDay;
  int             RegHour;
  int             RegMinute;
  ////[cjf***0432012*003*]

} TDB_PATIENT__;

#if 0
typedef struct _TDB_EXAM_HEADER {

  GUID            ExamID;
  GUID            PatientID;
  GUID            CalibrationID;
  GUID            ClinicID;
  GUID            PhysicianID;
  GUID            OperatorID;
  int             Type;
  int             Year;
  int             Month;
  int             Day;
  int             Hour;
  int             Minute;
  int             Second;
  int             Eye;
  int             Preop;
  int             SeriesNumber;
  int             Reserved1;
  int             Reserved2;
  int             Reserved3;
  int             Reserved4;
  char            Note[101];

} TDB_EXAM_HEADER__;
#endif
//#pragma pack(pop)

typedef struct _TDB_EYE
{

  GUID   EyeID;
  GUID   PatientID;
  int    Type;
  double ManifestVertex;
  double ManifestSphere;
  double ManifestCylinder;
  int    ManifestAxis;
  double K1;
  double K2;
  int    K2Axis;

} TDB_EYE;

#endif

//***************************************************************************************

class CField
{
public:
  CField();

  void Init(void *pData, const char *Name, short Type, int MaxSize = 0, BOOL AutoInc = FALSE, BOOL Indexed = FALSE, BOOL Primary = FALSE,
            BOOL Unique = FALSE);

  BOOL  m_Exists;
  void *m_pData;
  int   m_MaxSize;
  BOOL  m_Indexed;
  BOOL  m_Primary;
  BOOL  m_Unique;

  CDaoFieldInfo m_Info;
};

//***************************************************************************************

class CSet : public CDaoRecordset
{
public:
  CSet(CDaoDatabase *pDB);
  ~CSet();

  BOOL m_TableExists;

  CString              GetDefaultSQL() { return CString(GetName()); }
  virtual const TCHAR *GetName() const = 0; // { return _T(""); }

protected:
  long m_ID;

  int     m_NumFields;
  CField *m_Fields;

  void CreateFields();

private:
  void DoFieldExchange(CDaoFieldExchange *pFX);
};

//***************************************************************************************

class CGroupsSet : public CSet
{
public:
  CGroupsSet(CDaoDatabase *pDB);

  CString m_GroupID;
  CString m_Name;

  const TCHAR *GetName() const override { return _T("Groups"); }
};

//***************************************************************************************

class CClinicsSet : public CSet
{
public:
  CClinicsSet(CDaoDatabase *pDB);

  CString m_ClinicID;
  CString m_Name;
  CString m_Line1;
  CString m_Line2;
  CString m_City;
  CString m_State;
  CString m_ZIP;
  CString m_Country;

  const TCHAR *GetName() const override { return _T("Clinics"); }
};

//***************************************************************************************

class CPhysiciansSet : public CSet
{
public:
  CPhysiciansSet(CDaoDatabase *pDB);

  CString m_PhysicianID;
  CString m_LastName;
  CString m_FirstName;

  const TCHAR *GetName() const override {return _T("Physicians"); }
};

//***************************************************************************************

class COperatorsSet : public CSet
{
public:
  COperatorsSet(CDaoDatabase *pDB);

  CString m_OperatorID;
  CString m_LastName;
  CString m_FirstName;

  const TCHAR *GetName() const override { return _T("Operators"); }
};

//***************************************************************************************

class CPatientsSet : public CSet
{
public:
  CPatientsSet(CDaoDatabase *pDB);

  CString m_PatientID;
  CString m_GroupID;
  CString m_CustomID;
  CString m_LastName;
  CString m_FirstName;
  CString m_MiddleName;
  int     m_Sex;
  int     m_BirthYear;
  int     m_BirthMonth;
  int     m_BirthDay;
  CString m_Line1;
  CString m_Line2;
  CString m_City;
  CString m_State;
  CString m_ZIP;
  CString m_Country;
  CString m_HomePhone;
  CString m_WorkPhone;
  CString m_EMail;
  CString m_Note;
  ////[cjf***04302012*003*] 1
  int m_RegYear;
  int m_RegMonth;
  int m_RegDay;
  int m_RegHour;
  int m_RegMinute;
  ////[cjf***04302012*003*]

  const TCHAR *GetName() const override {return _T("Patients"); }
};

//***************************************************************************************

class CEyesSet : public CSet
{
public:
  CEyesSet(CDaoDatabase *pDB);

  CString m_EyeID;
  CString m_PatientID;
  int     m_Type;
  double  m_ManifestVertex;
  double  m_ManifestSphere;
  double  m_ManifestCylinder;
  int     m_ManifestAxis;
  double  m_K1;
  double  m_K2;
  int     m_K2Axis;

  const TCHAR *GetName() const override {return _T("Eyes"); }
};

//***************************************************************************************

class CExamHeadersSet : public CSet
{
public:
  CExamHeadersSet(CDaoDatabase *pDB);

  CString m_ExamID;
  CString m_PatientID;
  CString m_ClinicID;
  CString m_PhysicianID;
  CString m_OperatorID;
  CString m_CalibrationID;
  int     m_Type;
  int     m_Year;
  int     m_Month;
  int     m_Day;
  int     m_Hour;
  int     m_Minute;
  int     m_Second;
  int     m_Eye;
  int     m_Preop;
  int     m_SeriesNumber;
  int     m_Reserved1;
  int     m_Reserved2;
  int     m_Reserved3;
  int     m_Reserved4;
  CString m_Note;

  const TCHAR *GetName() const override { return _T("ExamHeaders"); }
};

//***************************************************************************************

class CDB
{
public:
  CDB();
  ~CDB();

  BOOL Open(const DBCONFIG &dbc);
  void Close();
  BOOL IsReadOnly() const { return m_ReadOnly; }

  BOOL ChangeLinker(CString newLinker);                   // 6.1.0
  BOOL CreateLinker(CString FileName, CString newLinker); // 6.1.0

  int  GetGroupsCount() const;
  BOOL LoadGroups(TDB_GROUP *pGroups) const;
  BOOL SaveGroup(const TDB_GROUP *pGroup);

  int  GetClinicsCount() const;
  BOOL LoadClinics(TDB_CLINIC *pClinics) const;
  BOOL SaveClinic(const TDB_CLINIC *pClinic);
  BOOL DeleteClinic(const GUID &ClinicID); //[cjf***05082012]

  int  GetPhysiciansCount() const;
  BOOL LoadPhysicians(TDB_PHYSICIAN *pPhysicians) const;
  BOOL SavePhysician(const TDB_PHYSICIAN *pPhysician);

  int  GetOperatorsCount() const;
  BOOL LoadOperators(TDB_OPERATOR *pOperators) const;
  BOOL SaveOperator(const TDB_OPERATOR *pOperator);

  int  GetPatientsCount(const wchar_t *filter) const;
  BOOL LoadPatients(TDB_PATIENT *pPatients, const wchar_t *filter) const;
  BOOL SavePatient(const TDB_PATIENT *pPatient);
  BOOL DeletePatient(const GUID &PatientID);

  int  GetEyesCount(const GUID &PatientID) const;
  BOOL LoadEyes(const GUID &PatientID, TDB_EYE *pEyes) const;
  BOOL SaveEye(const TDB_EYE *pEye);

  int  GetExamsCount(const GUID &PatientID) const;
  BOOL LoadExamHeaders(const GUID &PatientID, TDB_EXAM_HEADER *pExamHeaders) const;
  BOOL SaveExamHeader(const TDB_EXAM_HEADER *pExamHeader);
  int  GetExamSize(const GUID &ExamID) const;
  BOOL LoadExam(const GUID &ExamID, void *pExam) const;
  BOOL SaveExam(const GUID &ExamID, const void *pExam, const int ExamSize);
  BOOL DeleteExam(const GUID &ExamID);

  int  GetCalibrationSize(const GUID &CalibrationID);
  BOOL LoadCalibration(const GUID &CalibrationID, void *pCalibration);
  BOOL SaveCalibration(const GUID &CalibrationID, const void *pCalibration, const int CalibrationSize);

private:
  CString m_FileName;
  CString m_FolderName;
  BOOL    m_ReadOnly;

  CDaoDatabase m_Database;

  CGroupsSet *     m_pGroupsSet;
  CClinicsSet *    m_pClinicsSet;
  CPhysiciansSet * m_pPhysiciansSet;
  COperatorsSet *  m_pOperatorsSet;
  CPatientsSet *   m_pPatientsSet;
  CEyesSet *       m_pEyesSet;
  CExamHeadersSet *m_pExamHeadersSet;

  void CreateRelation(const CString &Name, const CString &TableName, const CString &ForeignTableName, const CString &FieldName,
                      const CString &ForeignFieldName);
};

//***************************************************************************************
