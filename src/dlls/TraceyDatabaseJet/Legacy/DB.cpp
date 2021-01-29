//***************************************************************************************

#include "StdAfx.h"
#include "DB.h"
#include "Chinese.h"
#include <Shlwapi.h> // PathFileExists
#include <libs/LegacyLib/Utils.h>

//***************************************************************************************

void Error(const char *s)
{
  ::MessageBoxA(NULL, s, "Error", MB_TOPMOST | MB_TASKMODAL | MB_ICONERROR | MB_OK);
}

EYE Type2Eye(int type)
{
  auto eye = static_cast<EYE>(type);
  if (!(eye == EYE::OD || eye == EYE::OS))
    eye = EYE::UNKNOWN;
  return eye;
};


//***************************************************************************************

// void GUIDToStr(const GUID& guid, CString& s)
//{
//  s.Format("{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
//    guid.Data1, guid.Data2, guid.Data3,
//    guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
//    guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
//  s.MakeUpper();
//}
//
////***************************************************************************************
//
// void StrToGUID(const char* s, GUID& guid)
//{
//  char t[17];
//
//  uint u;
//
//  memset(t, 0, sizeof(t));
//  strncpy(t, s + 1, 8);
//  sscanf(t, "%08x", &guid.Data1);
//
//  memset(t, 0, sizeof(t));
//  strncpy(t, s + 10, 4);
//  sscanf(t, "%04x", &u);
//  guid.Data2 = (ushort)u;
//
//  memset(t, 0, sizeof(t));
//  strncpy(t, s + 15, 4);
//  sscanf(t, "%04x", &u);
//  guid.Data3 = (ushort)u;
//
//  memset(t, 0, sizeof(t));
//  strncpy(t, s + 20, 4);
//  strncat(t, s + 25, 12);
//  for (int i = 0; i < 8; i++) {
//    sscanf(t + (i << 1), "%02x", &u);
//    guid.Data4[i] = (uchar)u;
//  }
//}

//***************************************************************************************

CField::CField()
{
  m_Exists  = FALSE;
  m_pData   = NULL;
  m_MaxSize = 0;
  m_Indexed = FALSE;
  m_Primary = FALSE;
  m_Unique  = FALSE;
}

//***************************************************************************************

void CField::Init(void *pData, const char *Name, short Type, int MaxSize, BOOL AutoInc, BOOL Indexed, BOOL Primary, BOOL Unique)
{
  m_pData   = pData;
  m_MaxSize = MaxSize;
  m_Indexed = Indexed;
  m_Primary = Primary;
  m_Unique  = Unique;

  m_Info.m_strName = Name;
  m_Info.m_nType   = Type;
  switch (Type) {
  case dbInteger:
    m_Info.m_lSize = 2;
    break;
  case dbLong:
    m_Info.m_lSize = 4;
    break;
  case dbBoolean:
    m_Info.m_lSize = 1;
    break;
  case dbSingle:
    m_Info.m_lSize = 4;
    break;
  case dbDouble:
    m_Info.m_lSize = 8;
    break;
  case dbText:
    m_Info.m_lSize = MaxSize;
    break;
  case dbLongBinary:
    m_Info.m_lSize = 0;
    break;
  case dbGUID:
    m_Info.m_lSize = 16;
    break;
  default:
    m_Info.m_lSize = 0;
    break;
  }
  m_Info.m_lAttributes = Type == dbText ? dbVariableField : dbFixedField;
  if (AutoInc)
    m_Info.m_lAttributes |= dbAutoIncrField;
  m_Info.m_nOrdinalPosition  = 0;
  m_Info.m_bRequired         = AutoInc;
  m_Info.m_bAllowZeroLength  = Type == dbText ? TRUE : FALSE;
  m_Info.m_lCollatingOrder   = 0;
  m_Info.m_strForeignName    = L"";
  m_Info.m_strSourceField    = L"";
  m_Info.m_strSourceTable    = L"";
  m_Info.m_strValidationRule = L"";
  m_Info.m_strValidationText = L"";
  m_Info.m_strDefaultValue   = L"";
}

//***************************************************************************************

CSet::CSet(CDaoDatabase *pDB)
  : CDaoRecordset(pDB)
{
  m_TableExists = FALSE;
  m_NumFields   = 0;
  m_Fields      = NULL;

  m_strFilter = "";
  m_strSort   = "ID ASC";
  m_nFields   = 0;
}

//***************************************************************************************

CSet::~CSet()
{
  if (m_Fields)
    delete[] m_Fields;
}
//***************************************************************************************
void CSet::CreateFields()
{
  CDaoTableDef Table(m_pDatabase);
  try {
    Table.Open(GetName());
    m_TableExists = TRUE;
  }
  catch (CDaoException *pDaoException) {
    pDaoException->Delete();
    m_TableExists = FALSE;
  }
  BOOL New;
  if (m_TableExists) {
    New = FALSE;
  }
  else if (m_pDatabase->CanUpdate()) {
    Table.Create(GetName());
    m_TableExists = TRUE;
    New           = TRUE;
  }
  else
    return;

  for (int i = 0; i < m_NumFields; i++) {
    try {
      CDaoFieldInfo Info;
      Table.GetFieldInfo(m_Fields[i].m_Info.m_strName, Info, AFX_DAO_PRIMARY_INFO);
      m_Fields[i].m_Exists = TRUE;
    }
    catch (CDaoException *pDaoException) {
      pDaoException->Delete();
      if (m_pDatabase->CanUpdate()) {
        Table.CreateField(m_Fields[i].m_Info);
        m_Fields[i].m_Exists = TRUE;
      }
      else {
        m_Fields[i].m_Exists = FALSE;
      }
    }
  }

  for (int i = 0; i < m_NumFields; i++) {
    if (m_Fields[i].m_Exists && m_Fields[i].m_Indexed) {
      try {
        CDaoIndexFieldInfo IndexField;
        IndexField.m_strName     = m_Fields[i].m_Info.m_strName;
        IndexField.m_bDescending = FALSE;
        CDaoIndexInfo Index;
        Index.m_strName        = m_Fields[i].m_Info.m_strName;
        Index.m_pFieldInfos    = &IndexField;
        Index.m_nFields        = 1;
        Index.m_bPrimary       = m_Fields[i].m_Primary;
        Index.m_bUnique        = m_Fields[i].m_Unique;
        Index.m_bClustered     = TRUE;
        Index.m_bIgnoreNulls   = FALSE;
        Index.m_bRequired      = TRUE;
        Index.m_bForeign       = FALSE;
        Index.m_lDistinctCount = 0;
        Table.CreateIndex(Index);
      }
      catch (CDaoException *pDaoException) {
        pDaoException->Delete();
      }
    }
  }

  if (New)
    Table.Append();

  Table.Close();

  m_nFields = 0;
  for (int i = 0; i < m_NumFields; i++) {
    if (m_Fields[i].m_Exists)
      m_nFields++;
  }
}

//***************************************************************************************

void CSet::DoFieldExchange(CDaoFieldExchange *pFX)
{
  pFX->SetFieldType(CDaoFieldExchange::outputColumn);

  for (int i = 0; i < m_NumFields; i++) {
    if (m_Fields[i].m_Exists) {
      switch (m_Fields[i].m_Info.m_nType) {
      case dbInteger:
        DFX_Short(pFX, m_Fields[i].m_Info.m_strName, *((short *)(m_Fields[i].m_pData)));
        break;
      case dbLong:
        DFX_Long(pFX, m_Fields[i].m_Info.m_strName, *((long *)(m_Fields[i].m_pData)));
        break;
      case dbBoolean:
        DFX_Bool(pFX, m_Fields[i].m_Info.m_strName, *((BOOL *)(m_Fields[i].m_pData)));
        break;
      case dbSingle:
        DFX_Single(pFX, m_Fields[i].m_Info.m_strName, *((float *)(m_Fields[i].m_pData)));
        break;
      case dbDouble:
        DFX_Double(pFX, m_Fields[i].m_Info.m_strName, *((double *)(m_Fields[i].m_pData)));
        break;
      case dbText:
        DFX_Text(pFX, m_Fields[i].m_Info.m_strName, *((CString *)(m_Fields[i].m_pData)));
        break;
      case dbLongBinary:
        DFX_Binary(pFX, m_Fields[i].m_Info.m_strName, *((CByteArray *)(m_Fields[i].m_pData)), m_Fields[i].m_MaxSize,
                   AFX_DAO_DISABLE_FIELD_CACHE);
        break;
      }
    }
  }
}

//***************************************************************************************

CGroupsSet::CGroupsSet(CDaoDatabase *pDB)
  : CSet(pDB)
{
  m_NumFields = 3;
  m_Fields    = new CField[m_NumFields];
  m_Fields[0].Init(&m_ID, "ID", dbLong, 0, TRUE, TRUE, TRUE, TRUE);
  m_Fields[1].Init(&m_GroupID, "GroupID", dbText, 38);
  m_Fields[2].Init(&m_Name, "Name", dbText, 50);
  CreateFields();
}

//***************************************************************************************

CClinicsSet::CClinicsSet(CDaoDatabase *pDB)
  : CSet(pDB)
{
  m_NumFields = 9;
  m_Fields    = new CField[m_NumFields];
  m_Fields[0].Init(&m_ID, "ID", dbLong, 0, TRUE, TRUE, TRUE, TRUE);
  m_Fields[1].Init(&m_ClinicID, "ClinicID", dbText, 38);
  m_Fields[2].Init(&m_Name, "Name", dbText, 50);
  m_Fields[3].Init(&m_Line1, "Address1", dbText, 50);
  m_Fields[4].Init(&m_Line2, "Address2", dbText, 50);
  m_Fields[5].Init(&m_City, "City", dbText, 50);
  m_Fields[6].Init(&m_State, "State", dbText, 50);
  m_Fields[7].Init(&m_ZIP, "Zip", dbText, 20);
  m_Fields[8].Init(&m_Country, "Country", dbText, 50);
  CreateFields();
}

//***************************************************************************************

CPhysiciansSet::CPhysiciansSet(CDaoDatabase *pDB)
  : CSet(pDB)
{
  m_NumFields = 4;
  m_Fields    = new CField[m_NumFields];
  m_Fields[0].Init(&m_ID, "ID", dbLong, 0, TRUE, TRUE, TRUE, TRUE);
  m_Fields[1].Init(&m_PhysicianID, "PhysicianID", dbText, 38);
  m_Fields[2].Init(&m_LastName, "LastName", dbText, 50);
  m_Fields[3].Init(&m_FirstName, "FirstName", dbText, 50);
  CreateFields();
}

//***************************************************************************************

COperatorsSet::COperatorsSet(CDaoDatabase *pDB)
  : CSet(pDB)
{
  m_NumFields = 4;
  m_Fields    = new CField[m_NumFields];
  m_Fields[0].Init(&m_ID, "ID", dbLong, 0, TRUE, TRUE, TRUE, TRUE);
  m_Fields[1].Init(&m_OperatorID, "OperatorID", dbText, 38);
  m_Fields[2].Init(&m_LastName, "LastName", dbText, 50);
  m_Fields[3].Init(&m_FirstName, "FirstName", dbText, 50);
  CreateFields();
}

//***************************************************************************************

CPatientsSet::CPatientsSet(CDaoDatabase *pDB)
  : CSet(pDB)
{
  // m_NumFields = 21;//
  m_NumFields = 26; //[cjf***04302012*003*]
  m_Fields    = new CField[m_NumFields];
  m_Fields[0].Init(&m_ID, "ID", dbLong, 0, TRUE, TRUE, TRUE, TRUE);
  m_Fields[1].Init(&m_PatientID, "PatientID", dbText, 38, FALSE, TRUE, FALSE, TRUE);
  m_Fields[2].Init(&m_GroupID, "GroupID", dbText, 38);
  m_Fields[3].Init(&m_CustomID, "CustomID", dbText, 50);
  m_Fields[4].Init(&m_LastName, "LastName", dbText, 50);
  m_Fields[5].Init(&m_FirstName, "FirstName", dbText, 50);
  m_Fields[6].Init(&m_MiddleName, "MiddleName", dbText, 50);
  m_Fields[7].Init(&m_Sex, "Sex", dbLong);
  m_Fields[8].Init(&m_BirthYear, "BirthYear", dbLong);
  m_Fields[9].Init(&m_BirthMonth, "BirthMonth", dbLong);
  m_Fields[10].Init(&m_BirthDay, "BirthDay", dbLong);
  m_Fields[11].Init(&m_Line1, "Address1", dbText, 50);
  m_Fields[12].Init(&m_Line2, "Address2", dbText, 50);
  m_Fields[13].Init(&m_City, "City", dbText, 50);
  m_Fields[14].Init(&m_State, "State", dbText, 50);
  m_Fields[15].Init(&m_ZIP, "Zip", dbText, 20);
  m_Fields[16].Init(&m_Country, "Country", dbText, 50);
  m_Fields[17].Init(&m_HomePhone, "HomePhone", dbText, 20);
  m_Fields[18].Init(&m_WorkPhone, "WorkPhone", dbText, 20);
  m_Fields[19].Init(&m_EMail, "EMail", dbText, 50);
  m_Fields[20].Init(&m_Note, "Note", dbText, 100);

  //[cjf***04302012*003*] 1
  m_Fields[21].Init(&m_RegYear, "RegYear", dbLong);
  m_Fields[22].Init(&m_RegMonth, "RegMonth", dbLong);
  m_Fields[23].Init(&m_RegDay, "RegDay", dbLong);
  m_Fields[24].Init(&m_RegHour, "RegHour", dbLong);
  m_Fields[25].Init(&m_RegMinute, "RegMinute", dbLong);
  //
  CreateFields();
}

//***************************************************************************************

CEyesSet::CEyesSet(CDaoDatabase *pDB)
  : CSet(pDB)
{
  m_NumFields = 11;
  m_Fields    = new CField[m_NumFields];
  m_Fields[0].Init(&m_ID, "ID", dbLong, 0, TRUE, TRUE, TRUE);
  m_Fields[1].Init(&m_EyeID, "EyeID", dbText, 38);
  m_Fields[2].Init(&m_PatientID, "PatientID", dbText, 38);
  m_Fields[3].Init(&m_Type, "Type", dbLong);
  m_Fields[4].Init(&m_ManifestVertex, "ManifestVertex", dbDouble);
  m_Fields[5].Init(&m_ManifestSphere, "ManifestSphere", dbDouble);
  m_Fields[6].Init(&m_ManifestCylinder, "ManifestCylinder", dbDouble);
  m_Fields[7].Init(&m_ManifestAxis, "ManifestAxis", dbLong);
  m_Fields[8].Init(&m_K1, "K1", dbDouble);
  m_Fields[9].Init(&m_K2, "K2", dbDouble);
  m_Fields[10].Init(&m_K2Axis, "K2Axis", dbLong);
  CreateFields();
}

//***************************************************************************************

CExamHeadersSet::CExamHeadersSet(CDaoDatabase *pDB)
  : CSet(pDB)
{
  m_NumFields = 22;
  m_Fields    = new CField[m_NumFields];
  m_Fields[0].Init(&m_ID, "ID", dbLong, 0, TRUE, TRUE, TRUE, TRUE);
  m_Fields[1].Init(&m_ExamID, "ExamID", dbText, 38, FALSE, TRUE, FALSE, TRUE);
  m_Fields[2].Init(&m_PatientID, "PatientID", dbText, 38);
  m_Fields[3].Init(&m_ClinicID, "ClinicID", dbText, 38);
  m_Fields[4].Init(&m_PhysicianID, "PhysicianID", dbText, 38);
  m_Fields[5].Init(&m_OperatorID, "OperatorID", dbText, 38);
  m_Fields[6].Init(&m_CalibrationID, "CalibrationID", dbText, 38);
  m_Fields[7].Init(&m_Type, "Type", dbLong);
  m_Fields[8].Init(&m_Year, "ExamYear", dbLong);
  m_Fields[9].Init(&m_Month, "ExamMonth", dbLong);
  m_Fields[10].Init(&m_Day, "ExamDay", dbLong);
  m_Fields[11].Init(&m_Hour, "ExamHour", dbLong);
  m_Fields[12].Init(&m_Minute, "ExamMinute", dbLong);
  m_Fields[13].Init(&m_Second, "ExamSecond", dbLong);
  m_Fields[14].Init(&m_Eye, "Eye", dbLong);
  m_Fields[15].Init(&m_Preop, "Preop", dbLong);
  m_Fields[16].Init(&m_SeriesNumber, "SeriesNumber", dbLong);
  m_Fields[17].Init(&m_Reserved1, "Reserved1", dbLong);
  m_Fields[18].Init(&m_Reserved2, "Reserved2", dbLong);
  m_Fields[19].Init(&m_Reserved3, "Reserved3", dbLong);
  m_Fields[20].Init(&m_Reserved4, "Reserved4", dbLong);
  m_Fields[21].Init(&m_Note, "Note", dbText, 100);
  CreateFields();
}

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

CDB::CDB()
{
  m_pGroupsSet      = NULL;
  m_pClinicsSet     = NULL;
  m_pPhysiciansSet  = NULL;
  m_pOperatorsSet   = NULL;
  m_pPatientsSet    = NULL;
  m_pEyesSet        = NULL;
  m_pExamHeadersSet = NULL;
}

//***************************************************************************************

CDB::~CDB()
{
  Close();
}

//***************************************************************************************
// 6.1.0  LocalFolderName is used to find the local front-end database(link database) location if the back-en is in the server

// 6.1.0 add LocalFolderName and Merge
BOOL CDB::Open(const DBCONFIG &dbc)
{
  if (m_Database.IsOpen())
    return FALSE;

#ifndef NEVER
  if (!dbc.readOnly) {
    static_assert(_ASSERT_LEVEL > 1);
    // todo make sure the file and the folders are read-write

    fs::create_directories(dbc.examFolder);
  }
  m_FileName   = dbc.database.c_str();
  m_FolderName = dbc.examFolder.c_str();
  m_ReadOnly   = dbc.readOnly;

  if (!fs::exists(dbc.database)) {
    try {
      CDaoDatabase dao;
      dao.Create(m_FileName, dbLangGeneral, dbVersion40);
    }
    catch (CDaoException *pDaoException) {
      pDaoException->Delete();
      TRACEY_THROW("Failed to create database.");
    }
  }
#else
  if (!::PathFileExists(FolderName)) {
    if (!::CreateDirectoryFolderName, NULL))
      {
        Error("Failed to create database folder.");
        return FALSE;
      }
  }

  m_FolderName = FolderName;
  CString FileName(m_FolderName + "\\tdb.mdb");

  // 6.1.0
  if (!::PathFileExists(FileName)) {
    try {
      m_Database.Create(FileName, dbLangGeneral, dbVersion40);
      m_Database.Close();
    }
    catch (CDaoException *pDaoException) {
      pDaoException->Delete();
      Error("Failed to create database ccc.");
      return FALSE;
    }
  }
  // 6.1.0

  CString ThisLocalFolderName = LocalFolderName;
  if (ThisLocalFolderName != "") {
    FileName = ThisLocalFolderName + "\\tdb_linker.mdb"; // 6.1.0 change from 'm_FolderName' to  ThisLocalFolderName
  }
  else {
    FileName = m_FolderName + "\\tdb.mdb";
  }

  m_ReadOnly = ReadOnly || (::GetFileAttributes(FileName) & FILE_ATTRIBUTE_READONLY);
#endif

  // [cjf***05032012*006*], add the regtime automaticly it they are not exist and the dabase is readonly
  if (m_ReadOnly) {
    try {
      m_Database.Open(m_FileName, FALSE, 0, L"");
    }
    catch (CDaoException *pDaoException) {
      pDaoException->Delete();
      Error("Failed to open database.");
      return FALSE;
    }
    CPatientsSet *m_pPatientsSetTest;
    m_pPatientsSetTest = new CPatientsSet(&m_Database);
    CDaoTableDef Table(&m_Database);
    Table.Open(L"Patients");

    // BOOL exist = 1;
    try {
      CDaoFieldInfo Info;
      Table.GetFieldInfo(L"RegYear", Info, AFX_DAO_PRIMARY_INFO);
    }
    catch (CDaoException *pDaoException) {
      pDaoException->Delete();

      CField m_RegYearFiled;
      int    m_RegYear;
      m_RegYearFiled.Init(&m_RegYear, "RegYear", dbLong);
      Table.CreateField(m_RegYearFiled.m_Info);
      //::Error("ok");

      CField m_RegMonthFiled;
      int    m_RegMonth;
      m_RegMonthFiled.Init(&m_RegMonth, "RegMonth", dbLong);
      Table.CreateField(m_RegMonthFiled.m_Info);

      CField m_RegDayFiled;
      int    m_RegDay;
      m_RegDayFiled.Init(&m_RegDay, "RegDay", dbLong);
      Table.CreateField(m_RegDayFiled.m_Info);

      CField m_RegHourFiled;
      int    m_RegHour;
      m_RegHourFiled.Init(&m_RegHour, "RegHour", dbLong);
      Table.CreateField(m_RegHourFiled.m_Info);

      CField m_RegMinuteFiled;
      int    m_RegMinute;
      m_RegMinuteFiled.Init(&m_RegMinute, "RegMinute", dbLong);
      Table.CreateField(m_RegMinuteFiled.m_Info);
    }

    if (m_Database.IsOpen()) {
      delete m_pPatientsSetTest;
    }
    m_Database.Close();
  }
  // [cjf***05032012*006*], add the regtime automatic it they are not exist done

  HANDLE hFile;

  hFile = ::CreateFile(m_FileName, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);

  if (hFile == INVALID_HANDLE_VALUE) {
    Error("Failed to open main database file.");
    return FALSE;
  }
  ulong FileSize = ::GetFileSize(hFile, NULL);
  ::CloseHandle(hFile);

  if (FileSize > 1000 * 1024 * 1024)
    m_ReadOnly = TRUE;

  // CString LdbFileName(FileName);
  // LdbFileName = LdbFileName.Left(LdbFileName.GetLength() - 3) + "ldb";
  // hFile = ::CreateFile(LdbFileName, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
  // if (hFile != INVALID_HANDLE_VALUE) {
  //  if (!::DeleteFile(LdbFileName)) m_ReadOnly = TRUE;
  //  ::CloseHandle(hFile);
  //}

  try {
    m_Database.Open(m_FileName, FALSE, m_ReadOnly, L"");
  }
  catch (CDaoException *pDaoException) {
    pDaoException->Delete();
    TRACEY_THROW("Failed to open database.");
  }

  m_pGroupsSet      = new CGroupsSet(&m_Database);
  m_pClinicsSet     = new CClinicsSet(&m_Database);
  m_pPhysiciansSet  = new CPhysiciansSet(&m_Database);
  m_pOperatorsSet   = new COperatorsSet(&m_Database);
  m_pPatientsSet    = new CPatientsSet(&m_Database);
  m_pEyesSet        = new CEyesSet(&m_Database);
  m_pExamHeadersSet = new CExamHeadersSet(&m_Database);

  CreateRelation(L"PatientsEyes", L"Patients", L"Eyes", L"PatientID", L"PatientID");
  CreateRelation(L"PatientsExamHeaders", L"Patients", L"ExamHeaders", L"PatientID", L"PatientID");

  return TRUE;
}
//***************************************************************************************
// 6.1.0 Create the front-end table linker
// CDaoDatabase
BOOL CDB::CreateLinker(CString FileName, CString newLinker)
{
  newLinker = L";DATABASE=" + newLinker + L"\\tdb.mdb";

  CDaoDatabase Linker_Database;

  try {
    Linker_Database.Create(FileName, dbLangGeneral, dbVersion40);
    Linker_Database.Close();
  }
  catch (CDaoException *pDaoException) {
    pDaoException->Delete();
    Error("Failed to create database aaa.");
    return FALSE;
  }

  try {
    Linker_Database.Open(FileName, FALSE, m_ReadOnly, L"");
  }
  catch (CDaoException *pDaoException) {
    pDaoException->Delete();
    Error("Failed to open database bbb.");
    return FALSE;
  }

  // Error(newLinker);

  /*CDaoDatabase OriDataBase;
  try {
    Error("-1");
    CString FileName = newLinker + "\\tdb.mdb";
    OriDataBase.Open(newLinker, FALSE, m_ReadOnly, "");
  }
  catch (CDaoException* pDaoException) {
    Error("-2");
    pDaoException->Delete();
    Error("Failed to open database.");
    return FALSE;
  }

  CDaoTableDef Table = db.CreateTableDef(accTable)

  Linker_Database*/
  /*Error("-3");
  CGroupsSet*      pGroupsSet = new CGroupsSet(&Linker_Database);
  CClinicsSet*     pClinicsSet = new CClinicsSet(&Linker_Database);
  CPhysiciansSet*  pPhysiciansSet = new CPhysiciansSet(&Linker_Database);
  COperatorsSet*   pOperatorsSet = new COperatorsSet(&Linker_Database);
  CPatientsSet*    pPatientsSet = new CPatientsSet(&Linker_Database);
  CEyesSet*        pEyesSet = new CEyesSet(&Linker_Database);
  CExamHeadersSet* pExamHeadersSet = new CExamHeadersSet(&Linker_Database);*/

  // Error("-5");

  CDaoTableDef Table(&Linker_Database);

  try {
    Table.Create(L"Groups");
    Table.SetSourceTableName(L"Groups");
    Table.SetConnect(newLinker);
    Table.Append();

    Table.Create(L"Clinics");
    Table.SetSourceTableName(L"Clinics");
    Table.SetConnect(newLinker);
    Table.Append();

    Table.Create(L"Physicians");
    Table.SetSourceTableName(L"Physicians");
    Table.SetConnect(newLinker);
    Table.Append();

    Table.Create(L"Operators");
    Table.SetSourceTableName(L"Operators");
    Table.SetConnect(newLinker);
    Table.Append();

    Table.Create(L"Patients");
    Table.SetSourceTableName(L"Patients");
    Table.SetConnect(newLinker);
    Table.Append();

    Table.Create(L"Eyes");
    Table.SetSourceTableName(L"Eyes");
    Table.SetConnect(newLinker);
    Table.Append();

    Table.Create(L"ExamHeaders");
    Table.SetSourceTableName(L"ExamHeaders");
    Table.SetConnect(newLinker);
    Table.Append();
  }
  catch (CDaoException *pDaoException) {
    pDaoException->Delete();
    return FALSE;
  }

  Table.Close();

  return TRUE;
}
// 6.1.0 Create the front-end table linker
//***************************************************************************************
// 6.1.0 Change the front-end table linker
BOOL CDB::ChangeLinker(CString newLinker)
{
  newLinker = L";DATABASE=" + newLinker + L"\\tdb.mdb";
  // Error(newLinker);
  CDaoTableDef Table(&m_Database);

  // Groups Table
  try {
    Table.Open(L"Groups");
  }
  catch (CDaoException *pDaoException) {
    pDaoException->Delete();
  }

  Table.SetConnect(newLinker);

  Table.RefreshLink();

  Table.Close();
  // Groups Done

  // Clinics Table
  try {
    Table.Open(L"Clinics");
  }
  catch (CDaoException *pDaoException) {
    pDaoException->Delete();
  }

  Table.SetConnect(newLinker);
  Table.RefreshLink();
  Table.Close();
  // Clinics Done

  // Physicians Table
  try {
    Table.Open(L"Physicians");
  }
  catch (CDaoException *pDaoException) {
    pDaoException->Delete();
  }

  Table.SetConnect(newLinker);
  Table.RefreshLink();
  Table.Close();
  // Physicians Done

  // Operators Table
  try {
    Table.Open(L"Operators");
  }
  catch (CDaoException *pDaoException) {
    pDaoException->Delete();
  }

  Table.SetConnect(newLinker);
  Table.RefreshLink();
  Table.Close();
  // Operators Done

  // Patient Table
  try {
    Table.Open(L"Patients");
  }
  catch (CDaoException *pDaoException) {
    pDaoException->Delete();
  }

  Table.SetConnect(newLinker);
  Table.RefreshLink();
  Table.Close();
  // Patient Done

  // Eyes Table
  try {
    Table.Open(L"Eyes");
  }
  catch (CDaoException *pDaoException) {
    pDaoException->Delete();
  }

  Table.SetConnect(newLinker);
  Table.RefreshLink();
  Table.Close();
  // Eyes Done

  // ExamHeaders Table
  try {
    Table.Open(L"ExamHeaders");
  }
  catch (CDaoException *pDaoException) {
    pDaoException->Delete();
  }

  Table.SetConnect(newLinker);
  Table.RefreshLink();
  Table.Close();
  // ExamHeaders Done

  return TRUE;
}
// 6.1.0
//***************************************************************************************

void CDB::Close()
{
  if (m_Database.IsOpen()) {
    delete m_pGroupsSet;
    delete m_pClinicsSet;
    delete m_pPhysiciansSet;
    delete m_pOperatorsSet;
    delete m_pPatientsSet;
    delete m_pEyesSet;
    delete m_pExamHeadersSet;
    m_Database.Close();
  }
}

//***************************************************************************************

void CDB::CreateRelation(const CString &Name, const CString &TableName, const CString &ForeignTableName, const CString &FieldName,
                         const CString &ForeignFieldName)
{
  if (m_Database.CanUpdate()) {
    try {
      CDaoRelationFieldInfo RelationFieldInfo;
      CDaoRelationInfo      RelationInfo;
      RelationInfo.m_strName             = Name;
      RelationInfo.m_strTable            = TableName;
      RelationInfo.m_strForeignTable     = ForeignTableName;
      RelationFieldInfo.m_strName        = FieldName;
      RelationFieldInfo.m_strForeignName = ForeignFieldName;
      RelationInfo.m_lAttributes         = dbRelationUpdateCascade | dbRelationDeleteCascade;
      RelationInfo.m_pFieldInfos         = &RelationFieldInfo;
      RelationInfo.m_nFields             = 1;
      m_Database.CreateRelation(RelationInfo);
    }
    catch (CDaoException *pDaoException) {
      pDaoException->Delete();
    }
  }
}

//***************************************************************************************
//***************************************************************************************

int CDB::GetGroupsCount() const
{
  if (!m_Database.IsOpen())
    return 0;
  if (!m_pGroupsSet->m_TableExists)
    return 0;

  m_pGroupsSet->m_strFilter = "";
  m_pGroupsSet->Open();

  int NumGroups = 0;
  while (!m_pGroupsSet->IsEOF()) {
    m_pGroupsSet->MoveNext();
    NumGroups++;
  }

  m_pGroupsSet->Close();

  return NumGroups;
}

//***************************************************************************************

BOOL CDB::LoadGroups(TDB_GROUP *pGroups) const
{
  if (!m_Database.IsOpen())
    return FALSE;
  if (!m_pGroupsSet->m_TableExists)
    return FALSE;

  m_pGroupsSet->m_strFilter = "";
  m_pGroupsSet->Open();

  int k = 0;
  while (!m_pGroupsSet->IsEOF()) {

    TDB_GROUP *pGroup = &pGroups[k++];

    StrToGUID(m_pGroupsSet->m_GroupID, pGroup->GroupID);
    pGroup->Name = m_pGroupsSet->m_Name.GetString();

    m_pGroupsSet->MoveNext();
  }

  m_pGroupsSet->Close();

  return TRUE;
}

//***************************************************************************************

BOOL CDB::SaveGroup(const TDB_GROUP *pGroup)
{
  if (!m_Database.IsOpen())
    return FALSE;
  if (!m_Database.CanUpdate())
    return FALSE;
  if (!m_pGroupsSet->m_TableExists)
    return FALSE;

  CString strGroupID;
  GUIDToStr(pGroup->GroupID, strGroupID);

  m_pGroupsSet->m_strFilter.Format(L"GroupID = %s", strGroupID);
  m_pGroupsSet->Open();

  if (m_pGroupsSet->IsEOF())
    m_pGroupsSet->AddNew();
  else
    m_pGroupsSet->Edit();

  GUIDToStr(pGroup->GroupID, m_pGroupsSet->m_GroupID);
  m_pGroupsSet->m_Name = pGroup->Name.c_str();

  m_pGroupsSet->Update();

  m_pGroupsSet->Close();

  return TRUE;
}

//***************************************************************************************
//***************************************************************************************

int CDB::GetClinicsCount() const
{
  if (!m_Database.IsOpen())
    return 0;
  if (!m_pClinicsSet->m_TableExists)
    return 0;

  m_pClinicsSet->m_strFilter = "";
  m_pClinicsSet->Open();

  int NumClinics = 0;
  while (!m_pClinicsSet->IsEOF()) {
    m_pClinicsSet->MoveNext();
    NumClinics++;
  }

  m_pClinicsSet->Close();

  return NumClinics;
}

//***************************************************************************************

BOOL CDB::LoadClinics(TDB_CLINIC *pClinics) const
{
  if (!m_Database.IsOpen())
    return FALSE;
  if (!m_pClinicsSet->m_TableExists)
    return FALSE;

  m_pClinicsSet->m_strFilter = "";
  m_pClinicsSet->Open();

  int k = 0;
  while (!m_pClinicsSet->IsEOF()) {

    TDB_CLINIC *pClinic = &pClinics[k++];

    StrToGUID(m_pClinicsSet->m_ClinicID, pClinic->ClinicID);
    pClinic->Name            = Str2Str(m_pClinicsSet->m_Name);
    pClinic->Address.Line1   = Str2Str(m_pClinicsSet->m_Line1);
    pClinic->Address.Line2   = Str2Str(m_pClinicsSet->m_Line2);
    pClinic->Address.City    = Str2Str(m_pClinicsSet->m_City);
    pClinic->Address.State   = Str2Str(m_pClinicsSet->m_State);
    pClinic->Address.ZIP     = Str2Str(m_pClinicsSet->m_ZIP);
    pClinic->Address.Country = Str2Str(m_pClinicsSet->m_Country);

    m_pClinicsSet->MoveNext();
  }

  m_pClinicsSet->Close();

  return TRUE;
}

//***************************************************************************************

BOOL CDB::SaveClinic(const TDB_CLINIC *pClinic)
{
  if (!m_Database.IsOpen())
    return FALSE;
  if (!m_Database.CanUpdate())
    return FALSE;
  if (!m_pClinicsSet->m_TableExists)
    return FALSE;

  CString strClinicID;
  GUIDToStr(pClinic->ClinicID, strClinicID);

  m_pClinicsSet->m_strFilter.Format(L"ClinicID = %s", strClinicID);
  m_pClinicsSet->Open();

  if (m_pClinicsSet->IsEOF())
    m_pClinicsSet->AddNew();
  else
    m_pClinicsSet->Edit();

  GUIDToStr(pClinic->ClinicID, m_pClinicsSet->m_ClinicID);
  m_pClinicsSet->m_Name    = pClinic->Name.c_str();
  m_pClinicsSet->m_Line1   = pClinic->Address.Line1.c_str();
  m_pClinicsSet->m_Line2   = pClinic->Address.Line2.c_str();
  m_pClinicsSet->m_City    = pClinic->Address.City.c_str();
  m_pClinicsSet->m_State   = pClinic->Address.State.c_str();
  m_pClinicsSet->m_ZIP     = pClinic->Address.ZIP.c_str();
  m_pClinicsSet->m_Country = pClinic->Address.Country.c_str();

  m_pClinicsSet->Update();

  m_pClinicsSet->Close();

  return TRUE;
}

//[cjf***05082012]
BOOL CDB::DeleteClinic(const GUID &ClinicID)
{
  if (!m_Database.IsOpen())
    return FALSE;
  if (!m_Database.CanUpdate())
    return FALSE;

  CString strClinicID;
  GUIDToStr(ClinicID, strClinicID);

  if (m_pExamHeadersSet->m_TableExists) {
    m_pExamHeadersSet->m_strFilter.Format(L"ClinicID = %s", strClinicID);
    m_pExamHeadersSet->Open();
    while (!m_pExamHeadersSet->IsEOF()) {
      // Delete exam header record
      m_pExamHeadersSet->m_ClinicID = L"";
      m_pExamHeadersSet->MoveNext();
    }
    m_pExamHeadersSet->Close();
  }

  if (m_pClinicsSet->m_TableExists) {
    m_pClinicsSet->Open();
    while (!m_pClinicsSet->IsEOF()) {
      if (m_pClinicsSet->m_ClinicID == strClinicID) {
        m_pClinicsSet->Delete();
        break;
      }
      m_pClinicsSet->MoveNext();
    }
    m_pClinicsSet->Close();
  }

  return TRUE;
}
//***************************************************************************************
//***************************************************************************************

int CDB::GetPhysiciansCount() const
{
  if (!m_Database.IsOpen())
    return 0;
  if (!m_pPhysiciansSet->m_TableExists)
    return 0;

  m_pPhysiciansSet->m_strFilter = L"";
  m_pPhysiciansSet->Open();

  int NumPhysicians = 0;
  while (!m_pPhysiciansSet->IsEOF()) {
    m_pPhysiciansSet->MoveNext();
    NumPhysicians++;
  }

  m_pPhysiciansSet->Close();

  return NumPhysicians;
}

//***************************************************************************************

BOOL CDB::LoadPhysicians(TDB_PHYSICIAN *pPhysicians) const
{
  if (!m_Database.IsOpen())
    return FALSE;
  if (!m_pPhysiciansSet->m_TableExists)
    return FALSE;

  m_pPhysiciansSet->m_strFilter = L"";
  m_pPhysiciansSet->Open();

  int k = 0;
  while (!m_pPhysiciansSet->IsEOF()) {

    TDB_PHYSICIAN *pPhysician = &pPhysicians[k++];

    StrToGUID(m_pPhysiciansSet->m_PhysicianID, pPhysician->PhysicianID);
    pPhysician->Name.Last   = Str2Str(m_pPhysiciansSet->m_LastName);
    pPhysician->Name.First  = Str2Str(m_pPhysiciansSet->m_FirstName);
    pPhysician->Name.Middle = L"";

    m_pPhysiciansSet->MoveNext();
  }

  m_pPhysiciansSet->Close();

  return TRUE;
}

//***************************************************************************************

BOOL CDB::SavePhysician(const TDB_PHYSICIAN *pPhysician)
{
  if (!m_Database.IsOpen())
    return FALSE;
  if (!m_Database.CanUpdate())
    return FALSE;
  if (!m_pPhysiciansSet->m_TableExists)
    return FALSE;

  CString strPhysicianID;
  GUIDToStr(pPhysician->PhysicianID, strPhysicianID);

  m_pPhysiciansSet->m_strFilter.Format(L"PhysicianID = %s", strPhysicianID);
  m_pPhysiciansSet->Open();

  if (m_pPhysiciansSet->IsEOF())
    m_pPhysiciansSet->AddNew();
  else
    m_pPhysiciansSet->Edit();

  GUIDToStr(pPhysician->PhysicianID, m_pPhysiciansSet->m_PhysicianID);
  m_pPhysiciansSet->m_LastName  = pPhysician->Name.Last.c_str();
  m_pPhysiciansSet->m_FirstName = pPhysician->Name.First.c_str();

  m_pPhysiciansSet->Update();

  m_pPhysiciansSet->Close();

  return TRUE;
}

//***************************************************************************************
//***************************************************************************************

int CDB::GetOperatorsCount() const
{
  if (!m_Database.IsOpen())
    return 0;
  if (!m_pOperatorsSet->m_TableExists)
    return 0;

  m_pOperatorsSet->m_strFilter = "";
  m_pOperatorsSet->Open();

  int NumOperators = 0;
  while (!m_pOperatorsSet->IsEOF()) {
    m_pOperatorsSet->MoveNext();
    NumOperators++;
  }

  m_pOperatorsSet->Close();

  return NumOperators;
}

//***************************************************************************************

BOOL CDB::LoadOperators(TDB_OPERATOR *pOperators) const
{
  if (!m_Database.IsOpen())
    return FALSE;
  if (!m_pOperatorsSet->m_TableExists)
    return FALSE;

  m_pOperatorsSet->m_strFilter = L"";
  m_pOperatorsSet->Open();

  int k = 0;
  while (!m_pOperatorsSet->IsEOF()) {

    TDB_OPERATOR *pOperator = &pOperators[k++];

    StrToGUID(m_pOperatorsSet->m_OperatorID, pOperator->OperatorID);
    pOperator->Name.Last   = Str2Str(m_pOperatorsSet->m_LastName);
    pOperator->Name.First  = Str2Str(m_pOperatorsSet->m_FirstName);
    pOperator->Name.Middle = L"";

    m_pOperatorsSet->MoveNext();
  }

  m_pOperatorsSet->Close();

  return TRUE;
}

//***************************************************************************************

BOOL CDB::SaveOperator(const TDB_OPERATOR *pOperator)
{
  if (!m_Database.IsOpen())
    return FALSE;
  if (!m_Database.CanUpdate())
    return FALSE;
  if (!m_pOperatorsSet->m_TableExists)
    return FALSE;

  CString strOperatorID;
  GUIDToStr(pOperator->OperatorID, strOperatorID);

  m_pOperatorsSet->m_strFilter.Format(L"OperatorID = %s", strOperatorID);
  m_pOperatorsSet->Open();

  if (m_pOperatorsSet->IsEOF())
    m_pOperatorsSet->AddNew();
  else
    m_pOperatorsSet->Edit();

  GUIDToStr(pOperator->OperatorID, m_pOperatorsSet->m_OperatorID);
  m_pOperatorsSet->m_LastName  = pOperator->Name.Last.c_str();
  m_pOperatorsSet->m_FirstName = pOperator->Name.First.c_str();

  m_pOperatorsSet->Update();

  m_pOperatorsSet->Close();

  return TRUE;
}

//***************************************************************************************
//***************************************************************************************

int CDB::GetPatientsCount(const wchar_t *filter) const
{
  if (!m_Database.IsOpen())
    return 0;
  if (!m_pPatientsSet->m_TableExists)
    return 0;

  m_pPatientsSet->m_strFilter = filter;
  m_pPatientsSet->Open();

  int NumPatients = 0;
  while (!m_pPatientsSet->IsEOF()) {
    m_pPatientsSet->MoveNext();
    NumPatients++;
  }

  m_pPatientsSet->Close();

  return NumPatients;
}

//***************************************************************************************

BOOL CDB::LoadPatients(TDB_PATIENT *pPatients, const wchar_t *filter) const
{
  if (!m_Database.IsOpen())
    return FALSE;
  if (!m_pPatientsSet->m_TableExists)
    return FALSE;

  m_pPatientsSet->m_strFilter = filter;
  m_pPatientsSet->Open();

  int NumPatients = 0;
  while (!m_pPatientsSet->IsEOF()) {

    TDB_PATIENT *pPatient = &pPatients[NumPatients++];

    StrToGUID(m_pPatientsSet->m_PatientID, pPatient->PatientID);
    StrToGUID(m_pPatientsSet->m_GroupID, pPatient->GroupID);

    pPatient->CustomID        = Str2Str(m_pPatientsSet->m_CustomID);
    pPatient->Name.Last       = Str2Str(m_pPatientsSet->m_LastName);
    pPatient->Name.First      = Str2Str(m_pPatientsSet->m_FirstName);
    pPatient->Name.Middle     = Str2Str(m_pPatientsSet->m_MiddleName);
    pPatient->Sex             = static_cast<PATIENT::SEX>(m_pPatientsSet->m_Sex);
    pPatient->Birthday.Year   = m_pPatientsSet->m_BirthYear;
    pPatient->Birthday.Month  = m_pPatientsSet->m_BirthMonth;
    pPatient->Birthday.Day    = m_pPatientsSet->m_BirthDay;
    pPatient->Address.Line1   = Str2Str(m_pPatientsSet->m_Line1);
    pPatient->Address.Line2   = Str2Str(m_pPatientsSet->m_Line2);
    pPatient->Address.City    = Str2Str(m_pPatientsSet->m_City);
    pPatient->Address.State   = Str2Str(m_pPatientsSet->m_State);
    pPatient->Address.ZIP     = Str2Str(m_pPatientsSet->m_ZIP);
    pPatient->Address.Country = Str2Str(m_pPatientsSet->m_Country);
    pPatient->HomePhone       = Str2Str(m_pPatientsSet->m_HomePhone);
    pPatient->WorkPhone       = Str2Str(m_pPatientsSet->m_WorkPhone);
    pPatient->EMail           = Str2Str(m_pPatientsSet->m_EMail);
    pPatient->Note            = Str2Str(m_pPatientsSet->m_Note);

    ////[cjf***04302012*003*]	1
    pPatient->Registration.Year   = m_pPatientsSet->m_RegYear;
    pPatient->Registration.Month  = m_pPatientsSet->m_RegMonth;
    pPatient->Registration.Day    = m_pPatientsSet->m_RegDay;
    pPatient->Registration.Hour   = m_pPatientsSet->m_RegHour;
    pPatient->Registration.Minute = m_pPatientsSet->m_RegMinute;
    pPatient->Registration.Second = 0;
    ////[cjf***0432012*003*]

    m_pPatientsSet->MoveNext();
  }

  m_pPatientsSet->Close();

  return TRUE;
}

//***************************************************************************************

BOOL CDB::SavePatient(const TDB_PATIENT *pPatient)
{
  if (!m_Database.IsOpen())
    return FALSE;
  if (!m_Database.CanUpdate())
    return FALSE;
  if (!m_pPatientsSet->m_TableExists)
    return FALSE;

  CString strPatientID;
  GUIDToStr(pPatient->PatientID, strPatientID);

  m_pPatientsSet->m_strFilter.Format(L"PatientID = %s", strPatientID);
  m_pPatientsSet->Open();

  if (m_pPatientsSet->IsEOF())
    m_pPatientsSet->AddNew();
  else
    m_pPatientsSet->Edit();

  m_pPatientsSet->m_PatientID = strPatientID;
  GUIDToStr(pPatient->GroupID, m_pPatientsSet->m_GroupID);
  m_pPatientsSet->m_CustomID   = pPatient->CustomID.c_str();
  m_pPatientsSet->m_LastName   = pPatient->Name.Last.c_str();
  m_pPatientsSet->m_FirstName  = pPatient->Name.First.c_str();
  m_pPatientsSet->m_MiddleName = pPatient->Name.Middle.c_str();
  m_pPatientsSet->m_Sex        = static_cast<int>(pPatient->Sex);
  m_pPatientsSet->m_BirthYear  = pPatient->Birthday.Year;
  m_pPatientsSet->m_BirthMonth = pPatient->Birthday.Month;
  m_pPatientsSet->m_BirthDay   = pPatient->Birthday.Day;
  m_pPatientsSet->m_Line1      = pPatient->Address.Line1.c_str();
  m_pPatientsSet->m_Line2      = pPatient->Address.Line2.c_str();
  m_pPatientsSet->m_City       = pPatient->Address.City.c_str();
  m_pPatientsSet->m_State      = pPatient->Address.State.c_str();
  m_pPatientsSet->m_ZIP        = pPatient->Address.ZIP.c_str();
  m_pPatientsSet->m_Country    = pPatient->Address.Country.c_str();
  m_pPatientsSet->m_HomePhone  = pPatient->HomePhone.c_str();
  m_pPatientsSet->m_WorkPhone  = pPatient->WorkPhone.c_str();
  m_pPatientsSet->m_EMail      = pPatient->EMail.c_str();
  m_pPatientsSet->m_Note       = pPatient->Note.c_str();

  ////[cjf***04302012*003*] 1
  m_pPatientsSet->m_RegYear   = pPatient->Registration.Year;
  m_pPatientsSet->m_RegMonth  = pPatient->Registration.Month;
  m_pPatientsSet->m_RegDay    = pPatient->Registration.Day;
  m_pPatientsSet->m_RegHour   = pPatient->Registration.Hour;
  m_pPatientsSet->m_RegMinute = pPatient->Registration.Minute;
  ////[cjf***04302012*003*]

  m_pPatientsSet->Update();

  m_pPatientsSet->Close();

  return TRUE;
}

//***************************************************************************************

BOOL CDB::DeletePatient(const GUID &PatientID)
{
  if (!m_Database.IsOpen())
    return FALSE;
  if (!m_Database.CanUpdate())
    return FALSE;

  CString strPatientID;
  GUIDToStr(PatientID, strPatientID);

  if (m_pExamHeadersSet->m_TableExists) {
    m_pExamHeadersSet->m_strFilter.Format(L"PatientID = %s", strPatientID);
    m_pExamHeadersSet->Open();
    while (!m_pExamHeadersSet->IsEOF()) {
      // Delete exam file
      ::DeleteFile(m_FolderName + L"\\Exams\\" + m_pExamHeadersSet->m_ExamID + L".exam");
      // Delete exam header record
      m_pExamHeadersSet->Delete();
      m_pExamHeadersSet->MoveNext();
    }
    m_pExamHeadersSet->Close();
  }

  if (m_pPatientsSet->m_TableExists) {
    m_pPatientsSet->m_strFilter.Format(L"PatientID = %s", strPatientID);
    m_pPatientsSet->Open();
    if (!m_pPatientsSet->IsEOF()) {
      m_pPatientsSet->Delete();
    }
    m_pPatientsSet->Close();
  }

  return TRUE;
}

//***************************************************************************************
//***************************************************************************************

int CDB::GetEyesCount(const GUID &PatientID) const
{
  if (!m_Database.IsOpen())
    return 0;
  if (!m_pEyesSet->m_TableExists)
    return 0;

  CString strPatientID;
  GUIDToStr(PatientID, strPatientID);

  m_pEyesSet->m_strFilter.Format(L"PatientID = %s", strPatientID);
  m_pEyesSet->Open();

  int NumEyes = 0;
  while (!m_pEyesSet->IsEOF()) {
    m_pEyesSet->MoveNext();
    NumEyes++;
  }

  m_pEyesSet->Close();

  return NumEyes;
}

//***************************************************************************************

BOOL CDB::LoadEyes(const GUID &PatientID, TDB_EYE *pEyes) const
{
  if (!m_Database.IsOpen())
    return FALSE;
  if (!m_pEyesSet->m_TableExists)
    return FALSE;

  CString strPatientID;
  GUIDToStr(PatientID, strPatientID);

  m_pEyesSet->m_strFilter.Format(L"PatientID = %s", strPatientID);
  m_pEyesSet->Open();

  int NumEyes = 0;
  while (!m_pEyesSet->IsEOF()) {

    TDB_EYE *pEye = &pEyes[NumEyes++];

    auto eyeFunc = [](int type) -> EYE {
      auto eye = static_cast<EYE>(type);
      if (!(eye == EYE::OD || eye == EYE::OS))
        eye = EYE::UNKNOWN;
      return eye;
    };

    StrToGUID(m_pEyesSet->m_EyeID, pEye->EyeID);
    pEye->PatientID        = PatientID;
    pEye->eye              = Type2Eye(m_pEyesSet->m_Type);
    pEye->ManifestVertex   = m_pEyesSet->m_ManifestVertex;
    pEye->ManifestSphere   = m_pEyesSet->m_ManifestSphere;
    pEye->ManifestCylinder = m_pEyesSet->m_ManifestCylinder;
    pEye->ManifestAxis     = m_pEyesSet->m_ManifestAxis;
    pEye->K1               = m_pEyesSet->m_K1;
    pEye->K2               = m_pEyesSet->m_K2;
    pEye->K2Axis           = m_pEyesSet->m_K2Axis;

    m_pEyesSet->MoveNext();
  }

  m_pEyesSet->Close();

  return TRUE;
}

//***************************************************************************************

BOOL CDB::SaveEye(const TDB_EYE *pEye)
{
  if (!m_Database.IsOpen())
    return FALSE;
  if (!m_Database.CanUpdate())
    return FALSE;
  if (!m_pEyesSet->m_TableExists)
    return FALSE;

  CString strEyeID;
  GUIDToStr(pEye->EyeID, strEyeID);

  m_pEyesSet->m_strFilter.Format(L"EyeID = %s", strEyeID);
  m_pEyesSet->Open();

  if (m_pEyesSet->IsEOF())
    m_pEyesSet->AddNew();
  else
    m_pEyesSet->Edit();

  m_pEyesSet->m_EyeID = strEyeID;
  GUIDToStr(pEye->PatientID, m_pEyesSet->m_PatientID);
  m_pEyesSet->m_Type             = static_cast<int>(pEye->eye);
  m_pEyesSet->m_ManifestVertex   = pEye->ManifestVertex;
  m_pEyesSet->m_ManifestSphere   = pEye->ManifestSphere;
  m_pEyesSet->m_ManifestCylinder = pEye->ManifestCylinder;
  m_pEyesSet->m_ManifestAxis     = pEye->ManifestAxis;
  m_pEyesSet->m_K1               = pEye->K1;
  m_pEyesSet->m_K2               = pEye->K2;
  m_pEyesSet->m_K2Axis           = pEye->K2Axis;

  m_pEyesSet->Update();

  m_pEyesSet->Close();

  return TRUE;
}

//***************************************************************************************
//***************************************************************************************

int CDB::GetExamsCount(const GUID &PatientID) const
{
  if (!m_Database.IsOpen())
    return 0;
  if (!m_pExamHeadersSet->m_TableExists)
    return 0;

  CString strPatientID;
  GUIDToStr(PatientID, strPatientID);

  m_pExamHeadersSet->m_strFilter.Format(L"PatientID = %s", strPatientID);
  m_pExamHeadersSet->Open();

  int NumExams = 0;
  while (!m_pExamHeadersSet->IsEOF()) {
    m_pExamHeadersSet->MoveNext();
    NumExams++;
  }

  m_pExamHeadersSet->Close();

  return NumExams;
}

//***************************************************************************************

BOOL CDB::LoadExamHeaders(const GUID &PatientID, TDB_EXAM_HEADER *pExamHeaders) const
{
  if (!m_Database.IsOpen())
    return FALSE;
  if (!m_pExamHeadersSet->m_TableExists)
    return FALSE;

  CString strPatientID;
  GUIDToStr(PatientID, strPatientID);

  m_pExamHeadersSet->m_strFilter.Format(L"PatientID = %s", strPatientID);
  m_pExamHeadersSet->Open();

  int k = 0;
  while (!m_pExamHeadersSet->IsEOF()) {

    TDB_EXAM_HEADER *pExamHeader = &pExamHeaders[k++];

    StrToGUID(m_pExamHeadersSet->m_ExamID, pExamHeader->ExamID);
    pExamHeader->PatientID = PatientID;
    StrToGUID(m_pExamHeadersSet->m_CalibrationID, pExamHeader->CalibrationID);
    StrToGUID(m_pExamHeadersSet->m_ClinicID, pExamHeader->ClinicID);
    StrToGUID(m_pExamHeadersSet->m_PhysicianID, pExamHeader->PhysicianID);
    StrToGUID(m_pExamHeadersSet->m_OperatorID, pExamHeader->OperatorID);
    pExamHeader->Type         = static_cast<EXAMHEADER::TYPE>(m_pExamHeadersSet->m_Type);
    pExamHeader->Date.Year    = m_pExamHeadersSet->m_Year;
    pExamHeader->Date.Month   = m_pExamHeadersSet->m_Month;
    pExamHeader->Date.Day     = m_pExamHeadersSet->m_Day;
    pExamHeader->Date.Hour    = m_pExamHeadersSet->m_Hour;
    pExamHeader->Date.Minute  = m_pExamHeadersSet->m_Minute;
    pExamHeader->Date.Second  = m_pExamHeadersSet->m_Second;
    pExamHeader->Eye          = Type2Eye(m_pExamHeadersSet->m_Eye);
    pExamHeader->Preop        = m_pExamHeadersSet->m_Preop;
    pExamHeader->SeriesNumber = m_pExamHeadersSet->m_SeriesNumber;
    pExamHeader->Reserved1    = m_pExamHeadersSet->m_Reserved1;
    pExamHeader->Reserved2    = m_pExamHeadersSet->m_Reserved2;
    pExamHeader->Reserved3    = m_pExamHeadersSet->m_Reserved3;
    pExamHeader->Reserved4    = m_pExamHeadersSet->m_Reserved4;
    pExamHeader->Note         = Str2Str(m_pExamHeadersSet->m_Note);

    m_pExamHeadersSet->MoveNext();
  }

  m_pExamHeadersSet->Close();

  return TRUE;
}

//***************************************************************************************

BOOL CDB::SaveExamHeader(const TDB_EXAM_HEADER *pExamHeader)
{
  if (!m_Database.IsOpen())
    return FALSE;
  if (!m_Database.CanUpdate())
    return FALSE;
  if (!m_pExamHeadersSet->m_TableExists)
    return FALSE;

  CString strExamID;
  GUIDToStr(pExamHeader->ExamID, strExamID);

  m_pExamHeadersSet->m_strFilter.Format(L"ExamID = %s", strExamID);
  m_pExamHeadersSet->Open();

  if (m_pExamHeadersSet->IsEOF())
    m_pExamHeadersSet->AddNew();
  else
    m_pExamHeadersSet->Edit();

  m_pExamHeadersSet->m_ExamID = strExamID;
  GUIDToStr(pExamHeader->PatientID, m_pExamHeadersSet->m_PatientID);
  GUIDToStr(pExamHeader->CalibrationID, m_pExamHeadersSet->m_CalibrationID);
  GUIDToStr(pExamHeader->ClinicID, m_pExamHeadersSet->m_ClinicID);
  GUIDToStr(pExamHeader->PhysicianID, m_pExamHeadersSet->m_PhysicianID);
  GUIDToStr(pExamHeader->OperatorID, m_pExamHeadersSet->m_OperatorID);
  m_pExamHeadersSet->m_Type         = static_cast<int>(pExamHeader->Type);
  m_pExamHeadersSet->m_Year         = pExamHeader->Date.Year;
  m_pExamHeadersSet->m_Month        = pExamHeader->Date.Month;
  m_pExamHeadersSet->m_Day          = pExamHeader->Date.Day;
  m_pExamHeadersSet->m_Hour         = pExamHeader->Date.Hour;
  m_pExamHeadersSet->m_Minute       = pExamHeader->Date.Minute;
  m_pExamHeadersSet->m_Second       = pExamHeader->Date.Second;
  m_pExamHeadersSet->m_Eye          = static_cast<int>(pExamHeader->Eye);
  m_pExamHeadersSet->m_Preop        = pExamHeader->Preop;
  m_pExamHeadersSet->m_SeriesNumber = pExamHeader->SeriesNumber;
  m_pExamHeadersSet->m_Reserved1    = pExamHeader->Reserved1;
  m_pExamHeadersSet->m_Reserved2    = pExamHeader->Reserved2;
  m_pExamHeadersSet->m_Reserved3    = pExamHeader->Reserved3;
  m_pExamHeadersSet->m_Reserved4    = pExamHeader->Reserved4;
  m_pExamHeadersSet->m_Note         = pExamHeader->Note.c_str();

  m_pExamHeadersSet->Update();

  m_pExamHeadersSet->Close();

  return TRUE;
}

//***************************************************************************************

int CDB::GetExamSize(const GUID &ExamID) const
{
  CString strExamID;
  GUIDToStr(ExamID, strExamID);

  ulong ExamSize = 0;

  CString FileName;
  FileName.Format(L"%s\\Exams\\%s.exam", m_FolderName, strExamID);

  HANDLE hFile = ::CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {
    ExamSize = ::GetFileSize(hFile, NULL);
    ::CloseHandle(hFile);
  }

  return (int)ExamSize;
}

//***************************************************************************************

BOOL CDB::LoadExam(const GUID &ExamID, void *pExam) const
{
  CString strExamID;
  GUIDToStr(ExamID, strExamID);

  BOOL res = FALSE;

  CString FileName;
  FileName.Format(L"%s\\Exams\\%s.exam", m_FolderName, strExamID);

  HANDLE hFile = ::CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {
    ulong ExamSize = ::GetFileSize(hFile, NULL);
    ulong bt;
    res = ::ReadFile(hFile, pExam, ExamSize, &bt, NULL);
    ::CloseHandle(hFile);
  }

  return res;
}

//***************************************************************************************

BOOL CDB::SaveExam(const GUID &ExamID, const void *pExam, const int ExamSize)
{
  CString strExamID;
  GUIDToStr(ExamID, strExamID);

  BOOL res = FALSE;

  CString ExamsFolderName(m_FolderName + L"\\Exams");
  ::CreateDirectory(ExamsFolderName, NULL);

  CString FileName;
  FileName.Format(L"%s\\%s.exam", ExamsFolderName, strExamID);

  HANDLE hFile = ::CreateFile(FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {
    ulong bt;
    res = ::WriteFile(hFile, pExam, (ulong)ExamSize, &bt, NULL);
    ::CloseHandle(hFile);
  }

  return res;
}

//***************************************************************************************

BOOL CDB::DeleteExam(const GUID &ExamID)
{
  if (!m_Database.IsOpen())
    return FALSE;
  if (!m_Database.CanUpdate())
    return FALSE;
  if (!m_pExamHeadersSet->m_TableExists)
    return FALSE;

  CString strExamID;
  GUIDToStr(ExamID, strExamID);

  m_pExamHeadersSet->m_strFilter.Format(L"ExamID = %s", strExamID);
  m_pExamHeadersSet->Open();
  if (!m_pExamHeadersSet->IsEOF()) {
    m_pExamHeadersSet->Delete();
  }
  m_pExamHeadersSet->Close();

  CString FileName;
  FileName.Format(L"%s\\Exams\\%s.exam", m_FolderName, strExamID);

  ::DeleteFile(FileName);

  return TRUE;
}

//***************************************************************************************
//***************************************************************************************

int CDB::GetCalibrationSize(const GUID &CalibrationID)
{
  CString strCalibrationID;
  GUIDToStr(CalibrationID, strCalibrationID);

  ulong CalibrationSize = 0;

  CString FileName;
  FileName.Format(L"%s\\Cals\\%s.cal", m_FolderName, strCalibrationID);

  HANDLE hFile = ::CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {
    CalibrationSize = ::GetFileSize(hFile, NULL);
    ::CloseHandle(hFile);
  }

  return (int)CalibrationSize;
}

//***************************************************************************************

BOOL CDB::LoadCalibration(const GUID &CalibrationID, void *pCalibration)
{
  CString strCalibrationID;
  GUIDToStr(CalibrationID, strCalibrationID);

  BOOL res = FALSE;

  CString FileName;
  FileName.Format(L"%s\\Cals\\%s.cal", m_FolderName, strCalibrationID);

  HANDLE hFile = ::CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {
    ulong CalibrationSize = ::GetFileSize(hFile, NULL);
    ulong bt;
    res = ::ReadFile(hFile, pCalibration, CalibrationSize, &bt, NULL);
    ::CloseHandle(hFile);
  }

  return res;
}

//***************************************************************************************

BOOL CDB::SaveCalibration(const GUID &CalibrationID, const void *pCalibration, const int CalibrationSize)
{
  CString strCalibrationID;
  GUIDToStr(CalibrationID, strCalibrationID);

  BOOL res = FALSE;

  CString CalFolderName(m_FolderName + L"\\Cals");
  ::CreateDirectory(CalFolderName, NULL);

  CString FileName;
  FileName.Format(L"%s\\%s.cal", CalFolderName, strCalibrationID);

  if (!::PathFileExists(FileName)) {
    HANDLE hFile = ::CreateFile(FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
      ulong bt;
      res = ::WriteFile(hFile, pCalibration, (ulong)CalibrationSize, &bt, NULL);
      ::CloseHandle(hFile);
    }
  }
  else {
    res = TRUE;
  }

  return res;
}

//***************************************************************************************
