//***************************************************************************************

#include "StdAfx.h"
#include <libs/CommonLib/Compression/Compression.h>
#include "Registry.h"
#include "zip.h"
#include "unzip.h"
#include "PasswordDlg.h"
#include "Settings.h"
#include "Licensing.h"
#include "INIParser.h"//6.2.0 ini registration
#include "GlobalFunctions.h"
#include <sys/stat.h>// get file info

#include "Data.h"
#include "AppVersion.h"
#include "AcquisitionParameters.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

#pragma comment(lib, "mpr.lib")//

//***************************************************************************************

CScanPoint::CScanPoint()
{
	m_shot = 0;
	m_r_um = 0.0;
	m_a_rd = 0.0;
	m_dx_um = 0.0;
	m_dy_um = 0.0;
	m_OpacityX = 0.0;
	m_OpacityY = 0.0;
	m_Opacity = 0.0;
	m_AutoBad = FALSE;
	m_Bad = FALSE;
}

//***************************************************************************************

CAddress::CAddress()
{
	Reset();
}

//***************************************************************************************

void CAddress::Reset()
{
	m_Line1.Empty();
	m_Line2.Empty();
	m_City.Empty();
	m_State.Empty();
	m_ZIP.Empty();
	m_Country.Empty();
}

//***************************************************************************************

CGroup::CGroup()
{
	Reset();
}

//***************************************************************************************

void CGroup::Reset()
{
	m_GroupID = GUID_NULL;
	m_Name.Empty();
}

//***************************************************************************************

CClinic::CClinic()
{
	Reset();
}

//***************************************************************************************

void CClinic::Reset()
{
	m_ClinicID = GUID_NULL;
	m_Name.Empty();
	m_Address.Reset();
}

//***************************************************************************************

CPhysician::CPhysician()
{
	Reset();
}

//***************************************************************************************

void CPhysician::Reset()
{
	m_PhysicianID = GUID_NULL;
	m_LastName.Empty();
	m_FirstName.Empty();
}

//***************************************************************************************

CString CPhysician::FullName()
{
	CString Name(m_FirstName);
	if (m_LastName != "") {
		if (Name != "") Name += " ";
		Name += m_LastName;
	}
	return Name;
}

//***************************************************************************************

COperator::COperator()
{
	Reset();
}

//***************************************************************************************

void COperator::Reset()
{
	m_OperatorID = GUID_NULL;
	m_LastName.Empty();
	m_FirstName.Empty();
}

//***************************************************************************************

CString COperator::FullName()
{
	CString Name(m_FirstName);
	if (m_LastName != "") {
		if (Name != "") Name += " ";
		Name += m_LastName;
	}
	return Name;
}

//***************************************************************************************

CEye::CEye()
{
	Reset();
}

//***************************************************************************************

void CEye::Reset()
{
	m_EyeID = GUID_NULL;
	m_PatientID = GUID_NULL;
	m_Type = EYE_OD;
	m_ManifestVertex = 0.0;
	m_ManifestSphere = 0.0;
	m_ManifestCylinder = 0.0;
	m_ManifestAxis = 0;
	m_K1 = 0.0;
	m_K2 = 0.0;
	m_K2Axis = 0;
}

//***************************************************************************************

CPatient::CPatient()
{
	Reset();
}

//***************************************************************************************

void CPatient::Reset()
{
	m_PatientID = GUID_NULL;
	m_GroupID = GUID_NULL;
	m_CustomID.Empty();
	m_LastName.Empty();
	m_FirstName.Empty();
	m_MiddleName.Empty();
	m_BirthYear = 0;
	m_BirthMonth = 0;
	m_BirthDay = 0;
	m_Sex = SEX_UNKNOWN;
	m_Address.Reset();
	m_HomePhone.Empty();
	m_WorkPhone.Empty();
	m_EMail.Empty();
	m_Note.Empty();
}

//***************************************************************************************

//6.2.0 Chinese add function G_ReadChinese()
CString CPatient::FullName()
{
	CString Name(G_ReadChinese(m_FirstName));

	if (m_MiddleName != "")
	{
		if (Name != "") Name += " ";
		Name += G_ReadChinese(m_MiddleName);
	}

	if (m_LastName != "")
	{
		if (Name != "") Name += " ";
		Name += G_ReadChinese(m_LastName);
	}

	return Name;
}

//***************************************************************************************

CExamHeader::CExamHeader()
{
	Reset();
}

//***************************************************************************************

void CExamHeader::Reset()
{
	m_ExamID = GUID_NULL;
	m_FellowExamID = GUID_NULL;
	m_PatientID = GUID_NULL;
	m_CalibrationID = GUID_NULL;
	m_ClinicID = GUID_NULL;
	m_PhysicianID = GUID_NULL;
	m_OperatorID = GUID_NULL;
	m_Type = EXAM_TYPE_UNKNOWN;
	m_Mode = EXAM_MODE_UNKNOWN;
	m_Year = 0;
	m_Month = 0;
	m_Day = 0;
	m_Hour = 0;
	m_Minute = 0;
	m_Second = 0;
	m_Eye = EYE_OD;
	m_Preop = PREOP_UNKNOWN;
	m_SeriesNumber = 0;
	m_Note.Empty();
	m_SaveYear = 0;
	m_SaveMonth = 0;
	m_SaveDay = 0;
	m_SaveHour = 0;
	m_SaveMinute = 0;
	m_SaveSecond = 0;
	m_Saved = FALSE;

	m_AutoNote.Empty();
	m_AngleKappaOK = FALSE;
	m_AngleAlphaOK = FALSE;
}

//***************************************************************************************

const char* CExamHeader::Preop()
{
	switch (m_Preop) {
	case PREOP_YES: return "Pre";
	case PREOP_NO: return "Post";
	default: return "";
	}
}

//***************************************************************************************

const char* CExamHeader::SoftwareVersion()
{
	uint v1 = (m_SoftwareVersion & 0x0f000000) >> 24;
	uint v2 = (m_SoftwareVersion & 0x00f00000) >> 20;
	uint v3 = (m_SoftwareVersion & 0x000f0000) >> 16;
	static char s[8];
	sprintf(s, "%u.%u.%u", v1, v2, v3);
	return s;
}

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

CDB::CDB()
{
	m_hDLL = NULL;
}

//***************************************************************************************

CDB::~CDB()
{
	Disconnect();
}

//***************************************************************************************
//530 delete all files inside a specified folder
void   CDB::DeleteDirectory(char* sDirName)
{
	CFileFind tempFind;
	char sTempFileFind[200];

	sprintf(sTempFileFind, "%s\\*.*", sDirName);
	BOOL IsFinded = tempFind.FindFile(LPCTSTR(sTempFileFind));
	while (IsFinded)
	{
		IsFinded = tempFind.FindNextFile();

		if (!tempFind.IsDots())
		{
			char sFoundFileName[200];
			CStringA Astr(tempFind.GetFileName().GetBuffer(200));
			strcpy(sFoundFileName, Astr);

			if (tempFind.IsDirectory())
			{
				char sTempDir[200];
				sprintf(sTempDir, "%s\\%s", sDirName, sFoundFileName);
				DeleteDirectory(sTempDir);
			}
			else
			{
				char sTempFileName[200];
				sprintf(sTempFileName, "%s\\%s", sDirName, sFoundFileName);
				DeleteFile(LPWSTR(sTempFileName));
			}
		}
	}
	tempFind.Close();

	/*if(!RemoveDirectory(sDirName))
	{
	return FALSE;
	}
	return TRUE; */
}
//530

//***************************************************************************************

void CDB::GetType(int& Type)
{
	//6.2.0 ini registration
	CINIParser IniPar;
	Type = IniPar.GetIntKey(IniSection, "Database Type", IniPath);
	//6.2.0 ini registration
}

//***************************************************************************************

void CDB::SetType(const int Type)
{
	CINIParser IniPar;
	IniPar.SetIntKey(IniSection, "Database Type", Type, IniPath);
}

//***************************************************************************************

void CDB::GetFolderName(CString& FolderName)
{
	FolderName.Empty();

	if (::Licensing.IsDemo())//530
	{
		FolderName = "C:\\Tracey\\Data\\MSJetEx";
	}
	else
	{
		CINIParser IniPar;
		FolderName = IniPar.GetStrKey(IniSection, "Database Folder", IniPath);
	}
}

//***************************************************************************************
// if the database server is from network, using the local front-end database
void CDB::GetLocalFolderName(CString& LocalFolderName, CString ServerFolderName)
{
	ServerFolderName.Replace(_T(":"), _T(""));
	ServerFolderName.Replace(_T("\\"), _T("_"));
	ServerFolderName.Replace(_T("/"), _T("_"));
	ServerFolderName.Replace(_T(":"), _T("_"));
	ServerFolderName.Replace(_T("*"), _T("_"));
	ServerFolderName.Replace(_T("?"), _T("_"));
	ServerFolderName.Replace(_T("\""), _T("_"));
	ServerFolderName.Replace(_T("<"), _T("_"));
	ServerFolderName.Replace(_T(">"), _T("_"));
	ServerFolderName.Replace(_T("|"), _T("_"));
	ServerFolderName.Replace(_T(","), _T("_"));

	//Check the server name is root name or not (ex: D_)
	CString lastS = ServerFolderName.Right(1);
	if (lastS == "_")
	{
		ServerFolderName.Replace(_T("_"), _T(""));
	}
	//Done

	LocalFolderName = MAIN_DIR + "\\Data\\TraceyLinkerServer\\";

	if (!::PathFileExists(LocalFolderName))
	{
		::CreateDirectory(LocalFolderName, NULL);
	}

	LocalFolderName += ServerFolderName;
}
//

//***************************************************************************************

//
CString CDB::GetSelfFolderName(CString FolderName)
{
	CString SelfFolderName = FolderName;
	CString LeftStr = FolderName.Left(2);

	SelfFolderName.Replace(LeftStr, _T("C:"));//ABC Maynot be c:

	return SelfFolderName;
}
//
//***************************************************************************************

void CDB::SetFolderName(const CString& FolderName)
{
	CINIParser IniPar;
	IniPar.SetStrKey(IniSection, "Database Folder", FolderName, IniPath);
}

//***************************************************************************************

void CDB::GetFileName(CString& FileName)
{
	//6.2.0 ini registration
	CINIParser IniPar;
	FileName = IniPar.GetStrKey(IniSection, "Database Filename", IniPath);
	//6.2.0 ini registration
}

//***************************************************************************************

void CDB::SetFileName(const CString& FileName)
{
	CINIParser IniPar;
	IniPar.SetStrKey(IniSection, "Database Filename", FileName, IniPath);
}

//***************************************************************************************

BOOL CDB::IsReadOnly()
{
	int Type;
	GetType(Type);

	if (Type == DATABASE_TYPE_MSJET) {
		if (m_hDLL == NULL) return TRUE;
		return DllIsReadOnly();
	}

	if (Type == DATABASE_TYPE_MYSQL) {
		return FALSE;
	}

	return TRUE;
}

//***************************************************************************************

HMODULE CDB::LoadLibrary(const int Type)
{
	if (Type == DATABASE_TYPE_MSJET_OLD) {
#ifdef _DEBUG
		return ::LoadLibrary(_T("C:\\Tracey\\tdbmsjet31.dll"));
#else
		return ::LoadLibrary(_T("tdbmsjet31.dll"));
#endif
	}

	if (Type == DATABASE_TYPE_MSJET) {
#ifdef _DEBUG
		// return ::LoadLibrary("D:\\Tracey\\TDBMSJet61\\Release\\tdbmsjet5.dll");//
		//return ::LoadLibrary("C:\\Tracey\\tdbmsjet61.dll");//
		return ::LoadLibrary(_T("C:\\Tracey\\tdbmsjet62.dll"));//6.2.0 For Dicom
#else
		//return ::LoadLibrary("tdbmsjet61.dll");//
		return ::LoadLibrary(_T("tdbmsjet62.dll"));//6.2.0 For Dicom
#endif
	}

	if (Type == DATABASE_TYPE_MYSQL) {
#ifdef _DEBUG
		return ::LoadLibrary(_T("C:\\Tracey\\tdbmysqldebug.dll"));
#else
		return ::LoadLibrary(_T("tdbmysql.dll"));
#endif
	}

	return NULL;
}

//***************************************************************************************

void CDB::Reinit()
{
	if (!::Question("This will delete the entire MySQL database. Are you sure?", 0, 1)) return;

	CPasswordDlg* pDlg = new CPasswordDlg(NULL);
	if (pDlg->DoModal() == IDOK)
	{

		Disconnect();

		HMODULE hDLL = LoadLibrary(DATABASE_TYPE_MYSQL);

		if (hDLL != NULL)
		{
			DllInitDatabase = (TDllInitDatabase)::GetProcAddress(hDLL, "InitDatabase");
			if (DllInitDatabase != NULL)
			{
				CStringA Apass(pDlg->m_Password);
				uint Res = (uint)DllInitDatabase(Apass);
				//::Info((int)Res);
			}
			else
			{
				::Error("Failed to load init function.");
			}
			::FreeLibrary(hDLL);
		}
		else
		{
			::Error("Failed to load database DLL file.");
		}

		Connect(FALSE);
	}

	delete pDlg;
}

//***************************************************************************************
//530
BOOL CDEMOID::LoadFromDemoFile(const CString& FileName)
{
	BOOL Res = FALSE;

	HANDLE hFile = ::CreateFile(FileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{

		int ZipSize = ::GetFileSize(hFile, NULL);
		void* pZipDataNew = malloc(ZipSize);

		ulong bt;
		::ReadFile(hFile, pZipDataNew, ZipSize, &bt, NULL);

		if (::GetDecompressedSize(pZipDataNew, ZipSize) == sizeof(CDEMOID)) {
			if (::Decompress(pZipDataNew, ZipSize, this)) {
				Res = TRUE;
			}
		}

		free(pZipDataNew);
		::CloseHandle(hFile);
	}
	return Res;
}
//530


//***************************************************************************************
//530
void CDEMOID::SaveIntoDemoFile(const CString& FileName)
{
	HANDLE hFile = ::CreateFile(FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{

		void* pZipData = malloc(sizeof(CDEMOID));
		int ZipSize;

		if (::Compress(this, sizeof(CDEMOID), pZipData, &ZipSize))
		{
			ulong bt;
			::WriteFile(hFile, pZipData, ZipSize, &bt, NULL);
		}

		free(pZipData);

		::CloseHandle(hFile);
	}
}
//530
//***************************************************************************************

// Load Other msjet database for database merge
void CDB::ConnectOtherMsjet(CString FolderName)
{
	CStringA AFolderName(FolderName);

	m_hDLL = LoadLibrary(DATABASE_TYPE_MSJET);

	DllConnect = (TDllConnect)::GetProcAddress(m_hDLL, "Connect");
	if (DllConnect == NULL) goto _ERROR_;

	DllDisconnect = (TDllDisconnect)::GetProcAddress(m_hDLL, "Disconnect");
	if (DllDisconnect == NULL) goto _ERROR_;

	DllIsReadOnly = (TDllIsReadOnly)::GetProcAddress(m_hDLL, "IsReadOnly");
	if (DllIsReadOnly == NULL) goto _ERROR_;

	DllChangeLinker = (TDllChangeLinker)::GetProcAddress(m_hDLL, "ChangeLinker");
	if (DllChangeLinker == NULL) goto _ERROR_;

	DllGetGroupsCount = (TDllGetGroupsCount)::GetProcAddress(m_hDLL, "GetGroupsCount");
	if (DllGetGroupsCount == NULL) goto _ERROR_;

	DllLoadGroups = (TDllLoadGroups)::GetProcAddress(m_hDLL, "LoadGroups");
	if (DllLoadGroups == NULL) goto _ERROR_;

	DllSaveGroup = (TDllSaveGroup)::GetProcAddress(m_hDLL, "SaveGroup");
	if (DllSaveGroup == NULL) goto _ERROR_;

	DllGetClinicsCount = (TDllGetClinicsCount)::GetProcAddress(m_hDLL, "GetClinicsCount");
	if (DllGetClinicsCount == NULL) goto _ERROR_;

	DllLoadClinics = (TDllLoadClinics)::GetProcAddress(m_hDLL, "LoadClinics");
	if (DllLoadClinics == NULL) goto _ERROR_;

	DllSaveClinic = (TDllSaveClinic)::GetProcAddress(m_hDLL, "SaveClinic");
	if (DllSaveClinic == NULL) goto _ERROR_;

	DllDeleteClinic = (TDllDeleteClinic)::GetProcAddress(m_hDLL, "DeleteClinic");
	if (DllDeleteClinic == NULL) goto _ERROR_;

	DllGetPhysiciansCount = (TDllGetPhysiciansCount)::GetProcAddress(m_hDLL, "GetPhysiciansCount");
	if (DllGetPhysiciansCount == NULL) goto _ERROR_;

	DllLoadPhysicians = (TDllLoadPhysicians)::GetProcAddress(m_hDLL, "LoadPhysicians");
	if (DllLoadPhysicians == NULL) goto _ERROR_;

	DllSavePhysician = (TDllSavePhysician)::GetProcAddress(m_hDLL, "SavePhysician");
	if (DllSavePhysician == NULL) goto _ERROR_;

	DllGetOperatorsCount = (TDllGetOperatorsCount)::GetProcAddress(m_hDLL, "GetOperatorsCount");
	if (DllGetOperatorsCount == NULL) goto _ERROR_;

	DllLoadOperators = (TDllLoadOperators)::GetProcAddress(m_hDLL, "LoadOperators");
	if (DllLoadOperators == NULL) goto _ERROR_;

	DllSaveOperator = (TDllSaveOperator)::GetProcAddress(m_hDLL, "SaveOperator");
	if (DllSaveOperator == NULL) goto _ERROR_;

	DllGetPatientsCount = (TDllGetPatientsCount)::GetProcAddress(m_hDLL, "GetPatientsCount");
	if (DllGetPatientsCount == NULL) goto _ERROR_;

	DllLoadPatients = (TDllLoadPatients)::GetProcAddress(m_hDLL, "LoadPatients");
	if (DllLoadPatients == NULL) goto _ERROR_;

	DllSavePatient = (TDllSavePatient)::GetProcAddress(m_hDLL, "SavePatient");
	if (DllSavePatient == NULL) goto _ERROR_;

	DllDeletePatient = (TDllDeletePatient)::GetProcAddress(m_hDLL, "DeletePatient");
	if (DllDeletePatient == NULL) goto _ERROR_;

	DllGetEyesCount = (TDllGetEyesCount)::GetProcAddress(m_hDLL, "GetEyesCount");
	if (DllGetEyesCount == NULL) goto _ERROR_;

	DllLoadEyes = (TDllLoadEyes)::GetProcAddress(m_hDLL, "LoadEyes");
	if (DllLoadEyes == NULL) goto _ERROR_;

	DllSaveEye = (TDllSaveEye)::GetProcAddress(m_hDLL, "SaveEye");
	if (DllSaveEye == NULL) goto _ERROR_;

	DllGetExamsCount = (TDllGetExamsCount)::GetProcAddress(m_hDLL, "GetExamsCount");
	if (DllGetExamsCount == NULL) goto _ERROR_;

	DllLoadExamHeaders = (TDllLoadExamHeaders)::GetProcAddress(m_hDLL, "LoadExamHeaders");
	if (DllLoadExamHeaders == NULL) goto _ERROR_;

	DllSaveExamHeader = (TDllSaveExamHeader)::GetProcAddress(m_hDLL, "SaveExamHeader");
	if (DllSaveExamHeader == NULL) goto _ERROR_;

	DllGetExamSize = (TDllGetExamSize)::GetProcAddress(m_hDLL, "GetExamSize");
	if (DllGetExamSize == NULL) goto _ERROR_;

	DllLoadExam = (TDllLoadExam)::GetProcAddress(m_hDLL, "LoadExam");
	if (DllLoadExam == NULL) goto _ERROR_;

	DllSaveExam = (TDllSaveExam)::GetProcAddress(m_hDLL, "SaveExam");
	if (DllSaveExam == NULL) goto _ERROR_;

	DllDeleteExam = (TDllDeleteExam)::GetProcAddress(m_hDLL, "DeleteExam");
	if (DllDeleteExam == NULL) goto _ERROR_;

	DllGetCalibrationSize = (TDllGetCalibrationSize)::GetProcAddress(m_hDLL, "GetCalibrationSize");
	if (DllGetCalibrationSize == NULL) goto _ERROR_;

	DllLoadCalibration = (TDllLoadCalibration)::GetProcAddress(m_hDLL, "LoadCalibration");
	if (DllLoadCalibration == NULL) goto _ERROR_;

	DllSaveCalibration = (TDllSaveCalibration)::GetProcAddress(m_hDLL, "SaveCalibration");
	if (DllSaveCalibration == NULL) goto _ERROR_;

	if (!DllConnect(AFolderName, "", "", FALSE)) {
		::Error("Failed to open database.");
	}

	return;

_ERROR_:

	if (m_hDLL != NULL) {
		::FreeLibrary(m_hDLL);
		m_hDLL = NULL;
	}

	::Error("Failed to load database DLL file.");
}
//***************************************************************************************

CString CDB::SplitDatabaseProcess(CString FolderName, BOOL& NeedChangeLinker, BOOL& FirstRun)
{
	NeedChangeLinker = FALSE;

	CString LocalServerFolderName = "";

	CString JetFileName = FolderName + "\\tdb.mdb";

	JetFileName.Replace(_T("\\\\"), _T("\\"));

	//Construct the default database
	if (!::PathFileExists(FolderName))
	{
		if (!::CreateDirectory(FolderName, NULL)) {
			Error("Failed to create database folder.");
			return LocalServerFolderName;
		}
		FirstRun = TRUE;
	}

	if (!::PathFileExists(JetFileName))
	{
		FirstRun = TRUE;
		//::Info("First run");//test
	}
	//Construct the default database Done

	GetLocalFolderName(LocalServerFolderName, FolderName);

	//If the linker database folder does not exist, create it
	if (!::PathFileExists(LocalServerFolderName))
	{
		::CreateDirectory(LocalServerFolderName, NULL);
	}


	if (!::PathFileExists(LocalServerFolderName + "\\tdb_linker.mdb"))
	{
		NeedChangeLinker = TRUE;
	}
	else
	{
		//Check the date, if it is less than installation then delete this file and generate a new one for 6.2.0 again
		CString linkerFile = LocalServerFolderName + "\\tdb_linker.mdb";

		WIN32_FILE_ATTRIBUTE_DATA attr;
		GetFileAttributesEx(linkerFile, GetFileExInfoStandard, &attr);

		FILETIME LastModTime= attr.ftLastWriteTime;//ftCreationTime;

		SYSTEMTIME time;
		FileTimeToSystemTime(&LastModTime, &time);

		//Read the installation day and compare to file generated date
		BOOL ReGen = FALSE;

		CINIParser IniPar;
		CString InstDate = IniPar.GetStrKey(IniSection, "Inst Data", IniPath);

		//InstDate.Replace(_T("-"),)
		CStringA YearA (InstDate.Right(4));
		CString Temp   (InstDate.Left(4) );
		CStringA Month (Temp.Left(2)     );
		CStringA Day   (Temp.Right(2)    );

		int YearI = atoi(YearA);
		int MonthI = atoi(Month);
		int DayI = atoi(Day);

		if (time.wYear < YearI)
		{
			ReGen = TRUE;
		}
		else if (time.wYear == YearI)
		{
			if (time.wMonth < MonthI)
			{
				ReGen = TRUE;
			}
			else if (time.wMonth == MonthI)
			{
				if (time.wDay < DayI)
				{
					ReGen = TRUE;
				}
			}
		}

		if (ReGen)
		{
			DeleteFile(linkerFile);
			NeedChangeLinker = TRUE;
		}
		//Done
	}
	//If the linker database folder does not exist, create it

	return LocalServerFolderName;
}


//**************************************************************************************************
//6.2.0 For Dicom
void CDB::LocalConnect(const BOOL ReadOnly)
{
	int Type;
	CString FolderName;
	CString FileName;
	GetType(Type);

	//Type = 0;
	GetFolderName(FolderName);
	GetFileName(FileName);

	CStringA AFileName(FileName);
	CStringA AFolderName(FolderName);

	BOOL FirstRun = FALSE;
	BOOL NeedChangeLinker = FALSE;
	CString LocalServerFolderName = "";//If the database is from server, here save the front-end database

	CStringA AstrLocal(LocalServerFolderName);

	if (::Licensing.IsDemo())
	{
		Type = 0;
		FolderName = MAIN_DIR + "\\Data\\MSJetEx";
		FileName = "";

		CINIParser IniPar;
		int Type = IniPar.GetIntKey(IniSection, "OMED", IniPath);//6.2.0 ini registration

		if (!::PathFileExists(FolderName))
		{
			if (Type == 1 || Type == 2)
			{
				Info("Database has been deleted. Please contact Tracey technology.");
				return;
			}

			::CreateDirectory(FolderName, NULL);
		}

		CString DemoFileName = MAIN_DIR + "\\Data\\MSJetEx\\exc.zip";
		CString DbFileName = MAIN_DIR + "\\Data\\MSJetEx\\tdb.mdb";
		if (!::PathFileExists(DemoFileName) && ::PathFileExists(DbFileName))
		{
			//delete all files in the folder
			DeleteDirectory((LPSTR)(LPCTSTR)FolderName);
		}

		//Create the demo correction file
		if (!::PathFileExists(DemoFileName))
		{
			if (Type == 1 || Type == 2)
			{
				Info("Database has been deleted. please contact Tracey technology.");
				return;
			}

			CDEMOID DemoData;
			for (int i = 0; i < 30; i++)
			{
				DemoData.PatientID[i] = GUID_NULL;
			}
			DemoData.SaveIntoDemoFile(DemoFileName);

			IniPar.SetIntKey(IniSection, "OMED", 2, IniPath);//6.2.0 ini registration
		}
	}
	//530

	m_hDLL = LoadLibrary(Type);

	if (m_hDLL == NULL) goto _ERROR_;

	if (Type == DATABASE_TYPE_MSJET || Type == DATABASE_TYPE_MSJET_OLD)
	{
		DllConnect = (TDllConnect)::GetProcAddress(m_hDLL, "Connect");
		if (DllConnect == NULL) goto _ERROR_;
		DllDisconnect = (TDllDisconnect)::GetProcAddress(m_hDLL, "Disconnect");
		if (DllDisconnect == NULL) goto _ERROR_;
		DllIsReadOnly = (TDllIsReadOnly)::GetProcAddress(m_hDLL, "IsReadOnly");
		if (DllIsReadOnly == NULL) goto _ERROR_;
	}
	else {
		DllConnect = NULL;
		DllDisconnect = NULL;
		DllIsReadOnly = NULL;
	}

	if (Type == DATABASE_TYPE_MSJET)
	{
		DllChangeLinker = (TDllChangeLinker)::GetProcAddress(m_hDLL, "ChangeLinker");
		if (DllChangeLinker == NULL) goto _ERROR_;

		DllCreateLinker = (TDllCreateLinker)::GetProcAddress(m_hDLL, "CreateLinker");
		if (DllCreateLinker == NULL) goto _ERROR_;
	}

	DllGetGroupsCount = (TDllGetGroupsCount)::GetProcAddress(m_hDLL, "GetGroupsCount");
	if (DllGetGroupsCount == NULL) goto _ERROR_;

	DllLoadGroups = (TDllLoadGroups)::GetProcAddress(m_hDLL, "LoadGroups");
	if (DllLoadGroups == NULL) goto _ERROR_;

	DllSaveGroup = (TDllSaveGroup)::GetProcAddress(m_hDLL, "SaveGroup");
	if (DllSaveGroup == NULL) goto _ERROR_;

	DllGetClinicsCount = (TDllGetClinicsCount)::GetProcAddress(m_hDLL, "GetClinicsCount");
	if (DllGetClinicsCount == NULL) goto _ERROR_;

	DllLoadClinics = (TDllLoadClinics)::GetProcAddress(m_hDLL, "LoadClinics");
	if (DllLoadClinics == NULL) goto _ERROR_;

	DllSaveClinic = (TDllSaveClinic)::GetProcAddress(m_hDLL, "SaveClinic");
	if (DllSaveClinic == NULL) goto _ERROR_;

	if (Type == DATABASE_TYPE_MSJET)
	{
		DllDeleteClinic = (TDllDeleteClinic)::GetProcAddress(m_hDLL, "DeleteClinic");
		if (DllDeleteClinic == NULL) goto _ERROR_;
	}

	DllGetPhysiciansCount = (TDllGetPhysiciansCount)::GetProcAddress(m_hDLL, "GetPhysiciansCount");
	if (DllGetPhysiciansCount == NULL) goto _ERROR_;

	DllLoadPhysicians = (TDllLoadPhysicians)::GetProcAddress(m_hDLL, "LoadPhysicians");
	if (DllLoadPhysicians == NULL) goto _ERROR_;

	DllSavePhysician = (TDllSavePhysician)::GetProcAddress(m_hDLL, "SavePhysician");
	if (DllSavePhysician == NULL) goto _ERROR_;

	DllGetOperatorsCount = (TDllGetOperatorsCount)::GetProcAddress(m_hDLL, "GetOperatorsCount");
	if (DllGetOperatorsCount == NULL) goto _ERROR_;

	DllLoadOperators = (TDllLoadOperators)::GetProcAddress(m_hDLL, "LoadOperators");
	if (DllLoadOperators == NULL) goto _ERROR_;

	DllSaveOperator = (TDllSaveOperator)::GetProcAddress(m_hDLL, "SaveOperator");
	if (DllSaveOperator == NULL) goto _ERROR_;

	DllGetPatientsCount = (TDllGetPatientsCount)::GetProcAddress(m_hDLL, "GetPatientsCount");
	if (DllGetPatientsCount == NULL) goto _ERROR_;

	DllLoadPatients = (TDllLoadPatients)::GetProcAddress(m_hDLL, "LoadPatients");
	if (DllLoadPatients == NULL) goto _ERROR_;

	DllSavePatient = (TDllSavePatient)::GetProcAddress(m_hDLL, "SavePatient");
	if (DllSavePatient == NULL) goto _ERROR_;

	DllDeletePatient = (TDllDeletePatient)::GetProcAddress(m_hDLL, "DeletePatient");
	if (DllDeletePatient == NULL) goto _ERROR_;

	DllGetEyesCount = (TDllGetEyesCount)::GetProcAddress(m_hDLL, "GetEyesCount");
	if (DllGetEyesCount == NULL) goto _ERROR_;

	DllLoadEyes = (TDllLoadEyes)::GetProcAddress(m_hDLL, "LoadEyes");
	if (DllLoadEyes == NULL) goto _ERROR_;

	DllSaveEye = (TDllSaveEye)::GetProcAddress(m_hDLL, "SaveEye");
	if (DllSaveEye == NULL) goto _ERROR_;

	DllGetExamsCount = (TDllGetExamsCount)::GetProcAddress(m_hDLL, "GetExamsCount");
	if (DllGetExamsCount == NULL) goto _ERROR_;

	DllLoadExamHeaders = (TDllLoadExamHeaders)::GetProcAddress(m_hDLL, "LoadExamHeaders");
	if (DllLoadExamHeaders == NULL) goto _ERROR_;

	DllSaveExamHeader = (TDllSaveExamHeader)::GetProcAddress(m_hDLL, "SaveExamHeader");
	if (DllSaveExamHeader == NULL) goto _ERROR_;

	DllGetExamSize = (TDllGetExamSize)::GetProcAddress(m_hDLL, "GetExamSize");
	if (DllGetExamSize == NULL) goto _ERROR_;

	DllLoadExam = (TDllLoadExam)::GetProcAddress(m_hDLL, "LoadExam");
	if (DllLoadExam == NULL) goto _ERROR_;

	DllSaveExam = (TDllSaveExam)::GetProcAddress(m_hDLL, "SaveExam");
	if (DllSaveExam == NULL) goto _ERROR_;

	DllDeleteExam = (TDllDeleteExam)::GetProcAddress(m_hDLL, "DeleteExam");
	if (DllDeleteExam == NULL) goto _ERROR_;

	DllGetCalibrationSize = (TDllGetCalibrationSize)::GetProcAddress(m_hDLL, "GetCalibrationSize");
	if (DllGetCalibrationSize == NULL) goto _ERROR_;

	DllLoadCalibration = (TDllLoadCalibration)::GetProcAddress(m_hDLL, "LoadCalibration");
	if (DllLoadCalibration == NULL) goto _ERROR_;

	DllSaveCalibration = (TDllSaveCalibration)::GetProcAddress(m_hDLL, "SaveCalibration");
	if (DllSaveCalibration == NULL) goto _ERROR_;

	if (Type == DATABASE_TYPE_MSJET_OLD)
	{
		if (::PathFileExists(FileName))
		{
			BOOL Res = FALSE;
			if (DllConnect(AFileName, "", "", TRUE))
			{
				Res = TRUE;
			}
			else if (DllConnect(AFileName, "please", "", TRUE))
			{
				Res = TRUE;
			}
			else {
				CPasswordDlg* pDlg = new CPasswordDlg(NULL);
				if (pDlg->DoModal() == IDOK)
				{
					CStringA Apsss(pDlg->m_Password);
					if (DllConnect(AFileName, Apsss, "", TRUE), FALSE)
					{
						Res = TRUE;
					}
					else
					{
						CString Password("please" + pDlg->m_Password);
						CStringA Apsss(Password);
						if (DllConnect(AFileName, Apsss, "", TRUE))
						{
							Res = TRUE;
						}
					}
				}
				delete pDlg;
			}
			if (!Res) {
				::Error("Failed to open database.");
			}
		}
		else
		{
			CString s;
			s.Format(_T("Database file \"%s\" does not exist."), AFileName);
			::Error(s);
		}
	}

	else if (Type == DATABASE_TYPE_MSJET)
	{
		//
		if (FirstRun)
		{
			if (!DllConnect(AFolderName, "", "", FALSE))
			{
				::Error("Failed to open database.");
			}

			DllDisconnect();
		}

		int a = 0;

		if (NeedChangeLinker)
		{
			if (!FirstRun)
			{
				if (!DllConnect(AFolderName, "", "", FALSE))
				{
					::Error("Failed to open database.");
				}

				DllDisconnect();
			}

			if (!DllCreateLinker(AstrLocal + "\\tdb_linker.mdb", AFolderName))
			{
				::Error("Failed to create linked Database.");
			}
		}

		if (!DllConnect(AFolderName, "", AstrLocal, ReadOnly))
		{
			::Error("Failed to open linked database.");
		}
	}


	if (::Licensing.IsDemo())
	{
		BOOL Wrong = FALSE;
		CString DemoFileName = "C:\\Tracey\\Data\\MSJetEx\\exc.zip";
		LoadPatients();
		int count = m_Patients.GetSize();
		if (count> 30)
		{
			Wrong = TRUE;
		}
		else
		{
			CDEMOID DemoData;
			DemoData.LoadFromDemoFile(DemoFileName);

			int DemoCount = 0;
			for (int i = 0; i < 30; i++)
			{
				if (DemoData.PatientID[i] != GUID_NULL) DemoCount++;
				else break;
			}

			if (count != DemoCount)
			{
				Wrong = TRUE;
			}
			else
			{
				for (int i = 0; i < count; i++)
				{
					if (m_Patients[i].m_PatientID != DemoData.PatientID[i])
					{
						Wrong = TRUE;
						break;
					}
				}
			}
		}

		if (Wrong)
		{
			Disconnect();
			DeleteDirectory((LPSTR)(LPCTSTR)FolderName);

			Connect(::Settings.m_OpenDBAsReadOnly);
		}
	}
	//530

	return;

_ERROR_:

	if (m_hDLL != NULL) {
		::FreeLibrary(m_hDLL);
		m_hDLL = NULL;
	}

	::Error("Failed to load database DLL file.");
}
//6.2.0 For Dicom
//**************************************************************************************************


void CDB::Connect(const BOOL ReadOnly)
{
	int Type;
	CString FolderName;
	CString FileName;
	GetType(Type);
	GetFolderName(FolderName);
	GetFileName(FileName);

	BOOL FirstRun = FALSE;
	BOOL NeedChangeLinker = FALSE;
	CString LocalServerFolderName = "";//If the database is from server, here save the front-end database

	if (Type == DATABASE_TYPE_MSJET)
	{
		LocalServerFolderName = SplitDatabaseProcess(FolderName, NeedChangeLinker, FirstRun);
	}

	CStringA AstrFolder(FolderName);
	CStringA AstrLocal(LocalServerFolderName);

	//530
	if (::Licensing.IsDemo())
	{
		Type = 0;
		FolderName = MAIN_DIR + "\\Data\\MSJetEx";
		FileName = "";

		CINIParser IniPar;
		int Type = IniPar.GetIntKey(IniSection, "OMED", IniPath);//6.2.0 ini registration

		if (!::PathFileExists(FolderName))
		{
			if (Type == 1 || Type == 2)
			{
				Info("Database has been deleted. Please contact Tracey technology.");
				return;
			}

			::CreateDirectory(FolderName, NULL);
		}

		CString DemoFileName = MAIN_DIR + "\\Data\\MSJetEx\\exc.zip";
		CString DbFileName = MAIN_DIR + "\\Data\\MSJetEx\\tdb.mdb";
		if (!::PathFileExists(DemoFileName) && ::PathFileExists(DbFileName))
		{
			//delete all files in the folder
			DeleteDirectory((LPSTR)(LPCTSTR)FolderName);
		}

		//Create the demo correction file
		if (!::PathFileExists(DemoFileName))
		{
			if (Type == 1 || Type == 2)
			{
				Info("Database has been deleted. please contact Tracey technology.");
				return;
			}

			CDEMOID DemoData;
			for (int i = 0; i < 30; i++)
			{
				DemoData.PatientID[i] = GUID_NULL;
			}
			DemoData.SaveIntoDemoFile(DemoFileName);

			IniPar.SetIntKey(IniSection, "OMED", 2, IniPath);//6.2.0 ini registration
		}
	}
	//530

	m_hDLL = LoadLibrary(Type);

	if (m_hDLL == NULL) goto _ERROR_;

	if (Type == DATABASE_TYPE_MSJET || Type == DATABASE_TYPE_MSJET_OLD)
	{
		DllConnect = (TDllConnect)::GetProcAddress(m_hDLL, "Connect");
		if (DllConnect == NULL) goto _ERROR_;
		DllDisconnect = (TDllDisconnect)::GetProcAddress(m_hDLL, "Disconnect");
		if (DllDisconnect == NULL) goto _ERROR_;
		DllIsReadOnly = (TDllIsReadOnly)::GetProcAddress(m_hDLL, "IsReadOnly");
		if (DllIsReadOnly == NULL) goto _ERROR_;
	}
	else {
		DllConnect = NULL;
		DllDisconnect = NULL;
		DllIsReadOnly = NULL;
	}

	if (Type == DATABASE_TYPE_MSJET)
	{
		DllChangeLinker = (TDllChangeLinker)::GetProcAddress(m_hDLL, "ChangeLinker");
		if (DllChangeLinker == NULL) goto _ERROR_;

		DllCreateLinker = (TDllCreateLinker)::GetProcAddress(m_hDLL, "CreateLinker");
		if (DllCreateLinker == NULL) goto _ERROR_;
	}

	DllGetGroupsCount = (TDllGetGroupsCount)::GetProcAddress(m_hDLL, "GetGroupsCount");
	if (DllGetGroupsCount == NULL) goto _ERROR_;

	DllLoadGroups = (TDllLoadGroups)::GetProcAddress(m_hDLL, "LoadGroups");
	if (DllLoadGroups == NULL) goto _ERROR_;

	DllSaveGroup = (TDllSaveGroup)::GetProcAddress(m_hDLL, "SaveGroup");
	if (DllSaveGroup == NULL) goto _ERROR_;

	DllGetClinicsCount = (TDllGetClinicsCount)::GetProcAddress(m_hDLL, "GetClinicsCount");
	if (DllGetClinicsCount == NULL) goto _ERROR_;

	DllLoadClinics = (TDllLoadClinics)::GetProcAddress(m_hDLL, "LoadClinics");
	if (DllLoadClinics == NULL) goto _ERROR_;

	DllSaveClinic = (TDllSaveClinic)::GetProcAddress(m_hDLL, "SaveClinic");
	if (DllSaveClinic == NULL) goto _ERROR_;

	//[cjf***05082012]
	if (Type == DATABASE_TYPE_MSJET)
	{
		DllDeleteClinic = (TDllDeleteClinic)::GetProcAddress(m_hDLL, "DeleteClinic");
		if (DllDeleteClinic == NULL) goto _ERROR_;
	}
	//[cjf***05082012]

	DllGetPhysiciansCount = (TDllGetPhysiciansCount)::GetProcAddress(m_hDLL, "GetPhysiciansCount");
	if (DllGetPhysiciansCount == NULL) goto _ERROR_;

	DllLoadPhysicians = (TDllLoadPhysicians)::GetProcAddress(m_hDLL, "LoadPhysicians");
	if (DllLoadPhysicians == NULL) goto _ERROR_;

	DllSavePhysician = (TDllSavePhysician)::GetProcAddress(m_hDLL, "SavePhysician");
	if (DllSavePhysician == NULL) goto _ERROR_;

	DllGetOperatorsCount = (TDllGetOperatorsCount)::GetProcAddress(m_hDLL, "GetOperatorsCount");
	if (DllGetOperatorsCount == NULL) goto _ERROR_;

	DllLoadOperators = (TDllLoadOperators)::GetProcAddress(m_hDLL, "LoadOperators");
	if (DllLoadOperators == NULL) goto _ERROR_;

	DllSaveOperator = (TDllSaveOperator)::GetProcAddress(m_hDLL, "SaveOperator");
	if (DllSaveOperator == NULL) goto _ERROR_;

	DllGetPatientsCount = (TDllGetPatientsCount)::GetProcAddress(m_hDLL, "GetPatientsCount");
	if (DllGetPatientsCount == NULL) goto _ERROR_;

	DllLoadPatients = (TDllLoadPatients)::GetProcAddress(m_hDLL, "LoadPatients");
	if (DllLoadPatients == NULL) goto _ERROR_;

	DllSavePatient = (TDllSavePatient)::GetProcAddress(m_hDLL, "SavePatient");
	if (DllSavePatient == NULL) goto _ERROR_;

	DllDeletePatient = (TDllDeletePatient)::GetProcAddress(m_hDLL, "DeletePatient");
	if (DllDeletePatient == NULL) goto _ERROR_;

	DllGetEyesCount = (TDllGetEyesCount)::GetProcAddress(m_hDLL, "GetEyesCount");
	if (DllGetEyesCount == NULL) goto _ERROR_;

	DllLoadEyes = (TDllLoadEyes)::GetProcAddress(m_hDLL, "LoadEyes");
	if (DllLoadEyes == NULL) goto _ERROR_;

	DllSaveEye = (TDllSaveEye)::GetProcAddress(m_hDLL, "SaveEye");
	if (DllSaveEye == NULL) goto _ERROR_;

	DllGetExamsCount = (TDllGetExamsCount)::GetProcAddress(m_hDLL, "GetExamsCount");
	if (DllGetExamsCount == NULL) goto _ERROR_;

	DllLoadExamHeaders = (TDllLoadExamHeaders)::GetProcAddress(m_hDLL, "LoadExamHeaders");
	if (DllLoadExamHeaders == NULL) goto _ERROR_;

	DllSaveExamHeader = (TDllSaveExamHeader)::GetProcAddress(m_hDLL, "SaveExamHeader");
	if (DllSaveExamHeader == NULL) goto _ERROR_;

	DllGetExamSize = (TDllGetExamSize)::GetProcAddress(m_hDLL, "GetExamSize");
	if (DllGetExamSize == NULL) goto _ERROR_;

	DllLoadExam = (TDllLoadExam)::GetProcAddress(m_hDLL, "LoadExam");
	if (DllLoadExam == NULL) goto _ERROR_;

	DllSaveExam = (TDllSaveExam)::GetProcAddress(m_hDLL, "SaveExam");
	if (DllSaveExam == NULL) goto _ERROR_;

	DllDeleteExam = (TDllDeleteExam)::GetProcAddress(m_hDLL, "DeleteExam");
	if (DllDeleteExam == NULL) goto _ERROR_;

	DllGetCalibrationSize = (TDllGetCalibrationSize)::GetProcAddress(m_hDLL, "GetCalibrationSize");
	if (DllGetCalibrationSize == NULL) goto _ERROR_;

	DllLoadCalibration = (TDllLoadCalibration)::GetProcAddress(m_hDLL, "LoadCalibration");
	if (DllLoadCalibration == NULL) goto _ERROR_;

	DllSaveCalibration = (TDllSaveCalibration)::GetProcAddress(m_hDLL, "SaveCalibration");
	if (DllSaveCalibration == NULL) goto _ERROR_;

	if (Type == DATABASE_TYPE_MSJET_OLD)
	{
		CStringA Astr(FileName);
		if (::PathFileExists(FileName))
		{
			BOOL Res = FALSE;
			if (DllConnect(Astr, "", "", TRUE))
			{
				Res = TRUE;
			}
			else if (DllConnect(Astr, "please", "", TRUE))
			{
				Res = TRUE;
			}
			else
			{
				CPasswordDlg* pDlg = new CPasswordDlg(NULL);
				if (pDlg->DoModal() == IDOK)
				{
					CStringA Apass(pDlg->m_Password);
					if (DllConnect(Astr, Apass, "", TRUE), FALSE)
					{
						Res = TRUE;
					}
					else
					{
						CString Password("please" + pDlg->m_Password);

						CStringA Ap(Password);
						if (DllConnect(Astr, Ap, "", TRUE))
						{
							Res = TRUE;
						}
					}
				}
				delete pDlg;
			}

			if (!Res)
			{
				::Error("Failed to open database.");
			}
		}
		else
		{
			CString s;
			s.Format(_T("Database file \"%s\" does not exist."), FileName);
			::Error(s);
		}
	}

	else if (Type == DATABASE_TYPE_MSJET)
	{
		if (FirstRun)
		{
			CStringA ansiStr(FolderName);

			if (!DllConnect(ansiStr, "", "", FALSE))//6.2.0
			{
				::Error("Failed to open database 1.");
			}

			DllDisconnect();
		}

		int a = 0;

		if (NeedChangeLinker)
		{
			if (!FirstRun)
			{
				if (!DllConnect(AstrFolder, "", "", FALSE))
				{
					::Error("Failed to open database 2.");
				}

				DllDisconnect();
			}

			if (!DllCreateLinker(AstrLocal + "\\tdb_linker.mdb", AstrFolder))
			{
				::Error("Failed to create linked Database.");
			}
		}

		if (!DllConnect(AstrFolder, "", AstrLocal, ReadOnly))
		{
			::Error("Failed to open linked database.");
		}
	}


	if (::Licensing.IsDemo())
	{
		BOOL Wrong = FALSE;
		CString DemoFileName = "C:\\Tracey\\Data\\MSJetEx\\exc.zip";
		LoadPatients();
		int count = m_Patients.GetSize();
		if (count> 30)
		{
			Wrong = TRUE;
		}
		else
		{
			CDEMOID DemoData;
			DemoData.LoadFromDemoFile(DemoFileName);

			int DemoCount = 0;
			for (int i = 0; i < 30; i++)
			{
				if (DemoData.PatientID[i] != GUID_NULL) DemoCount++;
				else break;
			}

			if (count != DemoCount)
			{
				Wrong = TRUE;
			}
			else
			{
				for (int i = 0; i < count; i++)
				{
					if (m_Patients[i].m_PatientID != DemoData.PatientID[i])
					{
						Wrong = TRUE;
						break;
					}
				}
			}
		}

		if (Wrong)
		{
			Disconnect();

			//Delete all files inside the folder
			DeleteDirectory((LPSTR)(LPCTSTR)FolderName);

			Connect(::Settings.m_OpenDBAsReadOnly);
		}
	}

	return;

_ERROR_:

	if (m_hDLL != NULL) {
		::FreeLibrary(m_hDLL);
		m_hDLL = NULL;
	}

	::Error("Failed to load database DLL file.");
}
//***************************************************************************************

void CDB::Disconnect()
{
	if (m_hDLL != NULL) {

		if (DllDisconnect) DllDisconnect();

		::FreeLibrary(m_hDLL);
		m_hDLL = NULL;
	}
}

//***************************************************************************************

void CDB::Change(const int NewType, const CString& NewFolderName, const CString& NewFileName, const BOOL NewReadOnly)
{
	if (NewType == DATABASE_TYPE_MSJET || NewType == DATABASE_TYPE_MYSQL || NewType == DATABASE_TYPE_MSJET_OLD) {

		int CurType;
		CString CurFolderName;
		CString CurFileName;
		GetType(CurType);
		GetFolderName(CurFolderName);
		GetFileName(CurFileName);

		if (NewType != CurType || ((NewType == DATABASE_TYPE_MSJET) && ((NewFolderName != CurFolderName) ||
			(NewReadOnly != DllIsReadOnly()))) || ((NewType == DATABASE_TYPE_MSJET_OLD) && (NewFileName != CurFileName)))
		{

			Disconnect();

			// ��������??������?����?��? ��� ����?��?����?
			SetType(NewType);
			if (NewType == DATABASE_TYPE_MSJET) {
				SetFolderName(NewFolderName);
			}
			else if (NewType == DATABASE_TYPE_MSJET_OLD) {
				SetFileName(NewFileName);
			}

			Connect(NewReadOnly);
		}
	}
}

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

void CDB::LoadGroups()
{
	m_Groups.Destroy();

	if (m_hDLL == NULL) return;

	int NumGroups = DllGetGroupsCount(); if (NumGroups <= 0) return;

	TDB_GROUP* pGroups = new TDB_GROUP[NumGroups];

	if (DllLoadGroups(pGroups)) {
		m_Groups.Create(NumGroups);
		for (int i = 0; i < NumGroups; i++) {
			m_Groups[i].m_GroupID = pGroups[i].GroupID;
			m_Groups[i].m_Name = pGroups[i].Name;
		}
	}

	delete[] pGroups;
}

//***************************************************************************************

BOOL CDB::SaveGroup(CGroup& Group)
{
	if (m_hDLL == NULL) return FALSE;

	if (Group.m_GroupID == GUID_NULL) ::CoCreateGuid(&Group.m_GroupID);

	TDB_GROUP* pGroup = new TDB_GROUP;

	memset(pGroup, 0, sizeof(TDB_GROUP));

	CStringA AName(Group.m_Name);
	pGroup->GroupID = Group.m_GroupID;

	strncpy(pGroup->Name, AName, 50);

	BOOL Res = DllSaveGroup(pGroup);

	delete pGroup;

	return Res;
}

//***************************************************************************************

CGroup* CDB::GetGroup(const GUID& GroupID)
{
	for (int i = 0; i < m_Groups.GetSize(); i++) {
		if (m_Groups[i].m_GroupID == GroupID) return &m_Groups[i];
	}
	return NULL;
}

//***************************************************************************************
//***************************************************************************************

void CDB::LoadClinics()
{
	m_Clinics.Destroy();

	if (m_hDLL == NULL) return;

	int NumClinics = DllGetClinicsCount(); if (NumClinics == 0) return;

	TDB_CLINIC* pClinics = new TDB_CLINIC[NumClinics];

	if (DllLoadClinics(pClinics)) {
		m_Clinics.Create(NumClinics);
		for (int i = 0; i < NumClinics; i++) {
			m_Clinics[i].m_ClinicID = pClinics[i].ClinicID;
			m_Clinics[i].m_Name = pClinics[i].Name;
			m_Clinics[i].m_Address.m_Line1 = pClinics[i].Address.Line1;
			m_Clinics[i].m_Address.m_Line2 = pClinics[i].Address.Line2;
			m_Clinics[i].m_Address.m_City = pClinics[i].Address.City;
			m_Clinics[i].m_Address.m_State = pClinics[i].Address.State;
			m_Clinics[i].m_Address.m_ZIP = pClinics[i].Address.ZIP;
			m_Clinics[i].m_Address.m_Country = pClinics[i].Address.Country;
		}
	}

	delete[] pClinics;
}

//***************************************************************************************

BOOL CDB::SaveClinic(CClinic& Clinic)
{
	if (m_hDLL == NULL) return FALSE;

	if (Clinic.m_ClinicID == GUID_NULL) ::CoCreateGuid(&Clinic.m_ClinicID);

	TDB_CLINIC* pClinic = new TDB_CLINIC;

	memset(pClinic, 0, sizeof(TDB_CLINIC));

	pClinic->ClinicID = Clinic.m_ClinicID;

	CStringA AName(Clinic.m_Name);
	CStringA AAd1(Clinic.m_Address.m_Line1);
	CStringA AAd2(Clinic.m_Address.m_Line2);
	CStringA ACity(Clinic.m_Address.m_City);
	CStringA AState(Clinic.m_Address.m_State);
	CStringA AZIP(Clinic.m_Address.m_ZIP);
	CStringA ACountry(Clinic.m_Address.m_Country);

	strncpy(pClinic->Name, AName, 50);
	strncpy(pClinic->Address.Line1, AAd1, 50);
	strncpy(pClinic->Address.Line2, AAd2, 50);
	strncpy(pClinic->Address.City, ACity, 50);
	strncpy(pClinic->Address.State, AState, 50);
	strncpy(pClinic->Address.ZIP, AZIP, 20);
	strncpy(pClinic->Address.Country, ACountry, 50);

	BOOL Res = DllSaveClinic(pClinic);

	delete pClinic;

	return Res;
}

//***************************************************************************************

CClinic* CDB::GetClinic(const GUID& ClinicID)
{
	for (int i = 0; i < m_Clinics.GetSize(); i++) {
		if (m_Clinics[i].m_ClinicID == ClinicID) return &m_Clinics[i];
	}
	return NULL;
}

//***************************************************************************************
//[cjf***05082012]
void CDB::DeleteClinic(const GUID& ClinicID)
{
	if (m_hDLL == NULL) return;

	DllDeleteClinic(ClinicID);
}
//[cjf***05082012]
//***************************************************************************************

void CDB::LoadPhysicians()
{
	m_Physicians.Destroy();

	if (m_hDLL == NULL) return;

	int NumPhysicians = DllGetPhysiciansCount(); if (NumPhysicians == 0) return;

	TDB_PHYSICIAN* pPhysicians = new TDB_PHYSICIAN[NumPhysicians];

	if (DllLoadPhysicians(pPhysicians))
	{
		m_Physicians.Create(NumPhysicians);

		for (int i = 0; i < NumPhysicians; i++)
		{
			m_Physicians[i].m_PhysicianID = pPhysicians[i].PhysicianID;
			m_Physicians[i].m_LastName = pPhysicians[i].LastName;
			m_Physicians[i].m_FirstName = pPhysicians[i].FirstName;
		}
	}

	delete[] pPhysicians;
}

//***************************************************************************************

BOOL CDB::SavePhysician(CPhysician& Physician)
{
	if (m_hDLL == NULL) return FALSE;

	if (Physician.m_PhysicianID == GUID_NULL) ::CoCreateGuid(&Physician.m_PhysicianID);

	TDB_PHYSICIAN* pPhysician = new TDB_PHYSICIAN;

	memset(pPhysician, 0, sizeof(TDB_PHYSICIAN));

	pPhysician->PhysicianID = Physician.m_PhysicianID;

	CStringA ALastName(Physician.m_LastName);
	CStringA AFirstName(Physician.m_FirstName);
	strncpy(pPhysician->LastName, ALastName, 50);
	strncpy(pPhysician->FirstName, AFirstName, 50);

	BOOL Res = DllSavePhysician(pPhysician);

	delete pPhysician;

	return Res;
}

//***************************************************************************************

CPhysician* CDB::GetPhysician(const GUID& PhysicianID)
{
	for (int i = 0; i < m_Physicians.GetSize(); i++) {
		if (m_Physicians[i].m_PhysicianID == PhysicianID) return &m_Physicians[i];
	}
	return NULL;
}

//***************************************************************************************
//***************************************************************************************

void CDB::LoadOperators()
{
	m_Operators.Destroy();

	if (m_hDLL == NULL) return;

	int NumOperators = DllGetOperatorsCount(); if (NumOperators == 0) return;

	TDB_OPERATOR* pOperators = new TDB_OPERATOR[NumOperators];

	if (DllLoadOperators(pOperators)) {
		m_Operators.Create(NumOperators);
		for (int i = 0; i < NumOperators; i++) {
			m_Operators[i].m_OperatorID = pOperators[i].OperatorID;
			m_Operators[i].m_LastName = pOperators[i].LastName;
			m_Operators[i].m_FirstName = pOperators[i].FirstName;
		}
	}

	delete[] pOperators;
}

//***************************************************************************************

BOOL CDB::SaveOperator(COperator& Operator)
{
	if (m_hDLL == NULL) return FALSE;

	if (Operator.m_OperatorID == GUID_NULL) ::CoCreateGuid(&Operator.m_OperatorID);

	TDB_OPERATOR* pOperator = new TDB_OPERATOR;

	memset(pOperator, 0, sizeof(TDB_OPERATOR));

	pOperator->OperatorID = Operator.m_OperatorID;

	CStringA ALastName(Operator.m_LastName);
	CStringA AFirstName(Operator.m_FirstName);
	strncpy(pOperator->LastName, ALastName, 50);
	strncpy(pOperator->FirstName, AFirstName, 50);

	BOOL Res = DllSaveOperator(pOperator);

	delete pOperator;

	return Res;
}

//***************************************************************************************

COperator* CDB::GetOperator(const GUID& OperatorID)
{
	for (int i = 0; i < m_Operators.GetSize(); i++) {
		if (m_Operators[i].m_OperatorID == OperatorID) return &m_Operators[i];
	}
	return NULL;
}

//***************************************************************************************
//***************************************************************************************

void CDB::LoadEyes(const GUID& PatientID)
{
	m_Eyes.Destroy();

	if (m_hDLL == NULL) return;

	//int NumEyes = DllGetEyesCount(PatientID);
	//if (NumEyes < 1 || NumEyes > 2) return;

	//TDB_EYE* pEyes = new TDB_EYE[NumEyes];

	m_EyeNum = DllGetEyesCount(PatientID);
	if (m_EyeNum < 1 || m_EyeNum > 2) return;

	TDB_EYE* pEyes = new TDB_EYE[m_EyeNum];

	if (DllLoadEyes(PatientID, pEyes)) {
		m_Eyes.Create(m_EyeNum);
		for (int i = 0; i < m_EyeNum; i++) {
			m_Eyes[i].m_EyeID = pEyes[i].EyeID;
			m_Eyes[i].m_PatientID = pEyes[i].PatientID;
			m_Eyes[i].m_Type = pEyes[i].Type;
			m_Eyes[i].m_ManifestVertex = pEyes[i].ManifestVertex;
			m_Eyes[i].m_ManifestSphere = pEyes[i].ManifestSphere;
			m_Eyes[i].m_ManifestCylinder = pEyes[i].ManifestCylinder;
			m_Eyes[i].m_ManifestAxis = pEyes[i].ManifestAxis;
			m_Eyes[i].m_K1 = pEyes[i].K1;
			m_Eyes[i].m_K2 = pEyes[i].K2;
			m_Eyes[i].m_K2Axis = pEyes[i].K2Axis;
		}
	}

	delete[] pEyes;
}

//***************************************************************************************

BOOL CDB::SaveEye(CEye& Eye)
{
	if (m_hDLL == NULL) return FALSE;

	TDB_EYE* pEye = new TDB_EYE;

	if (Eye.m_EyeID == GUID_NULL) ::CoCreateGuid(&Eye.m_EyeID);

	pEye->EyeID = Eye.m_EyeID;
	pEye->PatientID = Eye.m_PatientID;
	pEye->Type = Eye.m_Type;
	pEye->ManifestVertex = Eye.m_ManifestVertex;
	pEye->ManifestSphere = Eye.m_ManifestSphere;
	pEye->ManifestCylinder = Eye.m_ManifestCylinder;
	pEye->ManifestAxis = Eye.m_ManifestAxis;
	pEye->K1 = Eye.m_K1;
	pEye->K2 = Eye.m_K2;
	pEye->K2Axis = Eye.m_K2Axis;

	BOOL Res = DllSaveEye(pEye);

	delete pEye;

	return Res;
}

//***************************************************************************************
//***************************************************************************************

void CDB::PatientClassToStruct(const CPatient& Patient, TDB_PATIENT* pPatient)
{
	memset(pPatient, 0, sizeof(TDB_PATIENT));

	pPatient->PatientID = Patient.m_PatientID;
	pPatient->GroupID = Patient.m_GroupID;

	CStringA AID(Patient.m_CustomID);
	CStringA ALastName(Patient.m_LastName);
	CStringA AFirstName(Patient.m_FirstName);
	CStringA AMiddleName(Patient.m_MiddleName);

	strncpy(pPatient->CustomID, AID, 50);
	strncpy(pPatient->LastName, ALastName, 50);
	strncpy(pPatient->FirstName, AFirstName, 50);
	strncpy(pPatient->MiddleName, AMiddleName, 50);

	pPatient->Sex = Patient.m_Sex;

	pPatient->BirthYear = Patient.m_BirthYear;
	pPatient->BirthMonth = Patient.m_BirthMonth;
	pPatient->BirthDay = Patient.m_BirthDay;

	CStringA Aadd1(Patient.m_Address.m_Line1);
	CStringA Aadd2(Patient.m_Address.m_Line2);
	CStringA Acity(Patient.m_Address.m_City);
	CStringA AState(Patient.m_Address.m_State);
	CStringA AZIP(Patient.m_Address.m_ZIP);
	CStringA ACountry(Patient.m_Address.m_Country);
	CStringA AHomePhone(Patient.m_HomePhone);
	CStringA AWorkPhone(Patient.m_WorkPhone);
	CStringA AEMail(Patient.m_EMail);
	CStringA ANote(Patient.m_Note);

	strncpy(pPatient->Address.Line1, Aadd1, 50);
	strncpy(pPatient->Address.Line2, Aadd2, 50);
	strncpy(pPatient->Address.City, Acity, 50);
	strncpy(pPatient->Address.State, AState, 50);
	strncpy(pPatient->Address.ZIP, AZIP, 20);
	strncpy(pPatient->Address.Country, ACountry, 50);
	strncpy(pPatient->HomePhone, AHomePhone, 20);
	strncpy(pPatient->WorkPhone, AWorkPhone, 20);
	strncpy(pPatient->EMail, AEMail, 50);
	strncpy(pPatient->Note, ANote, 100);

	pPatient->RegYear = Patient.m_RegYear;
	pPatient->RegMonth = Patient.m_RegMonth;
	pPatient->RegDay = Patient.m_RegDay;
	pPatient->RegHour = Patient.m_RegHour;
	pPatient->RegMinute = Patient.m_RegMinute;

	pPatient->IsDicom = Patient.m_IsDicom; //6.2.0 For Dicom
}

//***************************************************************************************

void CDB::PatientStructToClass(const TDB_PATIENT* pPatient, CPatient& Patient)
{
	Patient.m_PatientID = pPatient->PatientID;
	Patient.m_GroupID = pPatient->GroupID;
	Patient.m_CustomID = pPatient->CustomID;
	Patient.m_LastName = pPatient->LastName;
	Patient.m_FirstName = pPatient->FirstName;
	Patient.m_MiddleName = pPatient->MiddleName;
	Patient.m_BirthYear = pPatient->BirthYear;
	Patient.m_BirthMonth = pPatient->BirthMonth;
	Patient.m_BirthDay = pPatient->BirthDay;
	Patient.m_Sex = pPatient->Sex;
	Patient.m_Address.m_Line1 = pPatient->Address.Line1;
	Patient.m_Address.m_Line2 = pPatient->Address.Line2;
	Patient.m_Address.m_City = pPatient->Address.City;
	Patient.m_Address.m_State = pPatient->Address.State;
	Patient.m_Address.m_ZIP = pPatient->Address.ZIP;
	Patient.m_Address.m_Country = pPatient->Address.Country;
	Patient.m_HomePhone = pPatient->HomePhone;
	Patient.m_WorkPhone = pPatient->WorkPhone;
	Patient.m_EMail = pPatient->EMail;
	Patient.m_Note = pPatient->Note;

	//[cjf***04302012*003*]
	Patient.m_RegYear = pPatient->RegYear;
	Patient.m_RegMonth = pPatient->RegMonth;
	Patient.m_RegDay = pPatient->RegDay;
	Patient.m_RegHour = pPatient->RegHour;
	Patient.m_RegMinute = pPatient->RegMinute;
	//[cjf***04302012*003*]

	Patient.m_IsDicom = pPatient->IsDicom;//6.2.0 For Dicom
}

//***************************************************************************************

void CDB::LoadPatients()
{
	m_Patients.Destroy();

	if (m_hDLL == NULL) return;

	int NumPatients = DllGetPatientsCount();

	if (NumPatients <= 0) return;

	TDB_PATIENT* pPatients = new TDB_PATIENT[NumPatients];

	if (DllLoadPatients(pPatients)) {
		m_Patients.Create(NumPatients);
		for (int i = 0; i < NumPatients; i++) {
			PatientStructToClass(pPatients + i, m_Patients[i]);
		}
	}

	delete[] pPatients;
}

//***************************************************************************************

BOOL CDB::SavePatient(CPatient& Patient)
{
	if (m_hDLL == NULL) return FALSE;

	if (Patient.m_PatientID == GUID_NULL) ::CoCreateGuid(&Patient.m_PatientID);

	TDB_PATIENT* pPatient = new TDB_PATIENT;

	PatientClassToStruct(Patient, pPatient);

	BOOL Res = DllSavePatient(pPatient);

	delete pPatient;

	return Res;
}

//***************************************************************************************

void CDB::DeletePatient(const GUID& PatientID)
{
	if (m_hDLL == NULL) return;

	DllDeletePatient(PatientID);

	//GenerateExamAmount();
}

//***************************************************************************************

CPatient* CDB::GetPatient(const GUID& PatientID)
{
	for (int i = 0; i < m_Patients.GetSize(); i++) {
		if (m_Patients[i].m_PatientID == PatientID) return &m_Patients[i];
	}
	return NULL;
}

//***************************************************************************************

void CDB::LoadExamHeaders(const GUID& PatientID)
{
	int Type;
	GetType(Type);

	m_ExamHeaders.Destroy();

	if (m_hDLL == NULL) return;

	m_NumExams = DllGetExamsCount(PatientID); if (m_NumExams == 0) return;

	TDB_EXAM_HEADER* pExamHeaders = new TDB_EXAM_HEADER[m_NumExams];

	memset(pExamHeaders, 0, m_NumExams * sizeof(TDB_EXAM_HEADER));

	if (DllLoadExamHeaders(PatientID, pExamHeaders)) {
		m_ExamHeaders.Create(m_NumExams);
		for (int i = 0; i < m_NumExams; i++) {
			m_ExamHeaders[i].m_ExamID = pExamHeaders[i].ExamID;
			m_ExamHeaders[i].m_PatientID = pExamHeaders[i].PatientID;
			m_ExamHeaders[i].m_CalibrationID = pExamHeaders[i].CalibrationID;
			m_ExamHeaders[i].m_ClinicID = pExamHeaders[i].ClinicID;
			m_ExamHeaders[i].m_PhysicianID = pExamHeaders[i].PhysicianID;
			m_ExamHeaders[i].m_OperatorID = pExamHeaders[i].OperatorID;
			m_ExamHeaders[i].m_Type = pExamHeaders[i].Type;
			m_ExamHeaders[i].m_Year = pExamHeaders[i].Year;
			m_ExamHeaders[i].m_Month = pExamHeaders[i].Month;
			m_ExamHeaders[i].m_Day = pExamHeaders[i].Day;
			m_ExamHeaders[i].m_Hour = pExamHeaders[i].Hour;
			m_ExamHeaders[i].m_Minute = pExamHeaders[i].Minute;
			m_ExamHeaders[i].m_Second = pExamHeaders[i].Second;
			m_ExamHeaders[i].m_Eye = pExamHeaders[i].Eye;
			m_ExamHeaders[i].m_Preop = pExamHeaders[i].Preop;
			m_ExamHeaders[i].m_SeriesNumber = pExamHeaders[i].SeriesNumber;
			m_ExamHeaders[i].m_Mode = pExamHeaders[i].Mode;
			if (Type == DATABASE_TYPE_MSJET_OLD) {
				m_ExamHeaders[i].m_SoftwareVersion = SOFTWARE_VERSION_3;
			}
			else if (pExamHeaders[i].SoftwareVersion == 0xffffffff) {
				m_ExamHeaders[i].m_SoftwareVersion = SOFTWARE_VERSION_4;
			}
			else {
				m_ExamHeaders[i].m_SoftwareVersion = pExamHeaders[i].SoftwareVersion;
			}
			m_ExamHeaders[i].m_Note = pExamHeaders[i].Note;
			//m_ExamHeaders[i].m_FellowExamID    = pExamHeaders[i].FellowExamID;
			m_ExamHeaders[i].m_Saved = TRUE;
		}
	}
	delete[] pExamHeaders;

	for (int i = 0; i < m_NumExams - 1; i++) {
		for (int j = m_NumExams - 1; j > i; j--) {
			BOOL swap = FALSE;
			if (m_ExamHeaders[j - 1].m_Year > m_ExamHeaders[j].m_Year) swap = TRUE;
			else if (m_ExamHeaders[j - 1].m_Year == m_ExamHeaders[j].m_Year) {
				if (m_ExamHeaders[j - 1].m_Month > m_ExamHeaders[j].m_Month) swap = TRUE;
				else if (m_ExamHeaders[j - 1].m_Month == m_ExamHeaders[j].m_Month) {
					if (m_ExamHeaders[j - 1].m_Day > m_ExamHeaders[j].m_Day) swap = TRUE;
					else if (m_ExamHeaders[j - 1].m_Day == m_ExamHeaders[j].m_Day) {
						if (m_ExamHeaders[j - 1].m_Hour > m_ExamHeaders[j].m_Hour) swap = TRUE;
						else if (m_ExamHeaders[j - 1].m_Hour == m_ExamHeaders[j].m_Hour) {
							if (m_ExamHeaders[j - 1].m_Minute > m_ExamHeaders[j].m_Minute) swap = TRUE;
							else if (m_ExamHeaders[j - 1].m_Minute == m_ExamHeaders[j].m_Minute) {
								if (m_ExamHeaders[j - 1].m_Second > m_ExamHeaders[j].m_Second) swap = TRUE;
							}
						}
					}
				}
			}
			if (swap) {
				Swap(m_ExamHeaders[j - 1], m_ExamHeaders[j]);
			}
		}
	}

	for (int i = 0; i < m_NumExams; i++) {
		m_ExamHeaders[i].m_Number = i + 1;
	}

}

//***************************************************************************************
//

BOOL CDB::SaveExamHeaders(CExamHeader& ExamHeader)
{
	if (m_hDLL == NULL) return FALSE;

	TDB_EXAM_HEADER *pExamHeader = new TDB_EXAM_HEADER;

  pExamHeader->ExamID          = ExamHeader.m_ExamID;
  pExamHeader->PatientID       = ExamHeader.m_PatientID;
  pExamHeader->CalibrationID   = ExamHeader.m_CalibrationID;
  pExamHeader->ClinicID        = ExamHeader.m_ClinicID;
  pExamHeader->PhysicianID     = ExamHeader.m_PhysicianID;
  pExamHeader->OperatorID      = ExamHeader.m_OperatorID;
  pExamHeader->Type            = ExamHeader.m_Type;
  pExamHeader->Mode            = ExamHeader.m_Mode;
  pExamHeader->Year            = ExamHeader.m_Year;
  pExamHeader->Month           = ExamHeader.m_Month;
  pExamHeader->Day             = ExamHeader.m_Day;
  pExamHeader->Hour            = ExamHeader.m_Hour;
  pExamHeader->Minute          = ExamHeader.m_Minute;
  pExamHeader->Second          = ExamHeader.m_Second;
  pExamHeader->Eye             = ExamHeader.m_Eye;
  pExamHeader->Preop           = ExamHeader.m_Preop;
  pExamHeader->SeriesNumber    = ExamHeader.m_SeriesNumber;
  pExamHeader->SoftwareVersion = ExamHeader.m_SoftwareVersion;
  pExamHeader->legacy          = 0;
  pExamHeader->Reserved4       = 0;

  CString s = ExamHeader.m_Note;
  if (s.GetLength() == 100) {
    s = s.Left(99);
  }
  CStringA Astr(s);
  strncpy(pExamHeader->Note, Astr, 100);

  BOOL Res = DllSaveExamHeader(pExamHeader);

  return Res;
}

//***************************************************************************************

CExamHeader* CDB::GetExamHeader(const GUID& ExamID)
{
	for (int i = 0; i < m_ExamHeaders.GetSize(); i++) {
		if (m_ExamHeaders[i].m_ExamID == ExamID) return &m_ExamHeaders[i];
	}
	return NULL;
}

//***************************************************************************************

////Big Database Research since 03062019
//void CDB::LoadTxtExamHeaders(const GUID& PatientID)
//{
//	int Type;
//	GetType(Type);
//
//	m_ExamHeaders.Destroy();
//
//	CString PatientIDStr;
//
//	GUIDToStr(PatientID, PatientIDStr);
//
//	CString FolderName;
//	GetFolderName(FolderName);
//
//	CString FileName = FolderName + "\\ExamHeader\\" + PatientIDStr + ".csv";
//
//	if (!::PathFileExists(FileName))
//	{
//		m_NumExams = 0;
//		return;
//	}
//
//	CStringA FileNameA = FileName;
//	FILE* pFileR = fopen(FileNameA, "r");
//
//	CStringA Date;
//	char buf[1024];
//
//	int i = 0;
//	while (fgets(buf, 100, pFileR) != NULL)
//	{
//		i++;
//	}
//
//	m_NumExams = (int)((real)i / 18.0);
//
//	fclose(pFileR);
//
//
//	pFileR = fopen(FileNameA, "r");
//
//	i = 0;
//	int count = 0;
//
//	m_ExamHeaders.Create(m_NumExams);
//
//	while (fgets(buf, 100, pFileR) != NULL)
//	{
//		Date = buf;
//		Date.Replace("\n", "");
//
//		if (count == 0)
//		{
//			StrToGUID(Date, m_ExamHeaders[i].m_ExamID);
//			count = 1;
//		}
//		else if (count == 1)
//		{
//			StrToGUID(Date, m_ExamHeaders[i].m_CalibrationID);
//			count = 2;
//		}
//		else if (count == 2)
//		{
//			StrToGUID(Date, m_ExamHeaders[i].m_ClinicID);
//			count = 3;
//		}
//		else if (count == 3)
//		{
//			StrToGUID(Date, m_ExamHeaders[i].m_PhysicianID);
//			count = 4;
//		}
//		else if (count == 4)
//		{
//			StrToGUID(Date, m_ExamHeaders[i].m_OperatorID);
//			count = 5;
//		}
//		else if (count == 5)
//		{
//			m_ExamHeaders[i].m_Type = atoi(Date);
//			count = 6;
//		}
//		else if (count == 6)
//		{
//			m_ExamHeaders[i].m_Mode = atoi(Date);
//			count = 7;
//		}
//		else if (count == 7)
//		{
//			m_ExamHeaders[i].m_Year = atoi(Date);
//			count = 8;
//		}
//		else if (count == 8)
//		{
//			m_ExamHeaders[i].m_Month = atoi(Date);
//			count = 9;
//		}
//		else if (count == 9)
//		{
//			m_ExamHeaders[i].m_Day = atoi(Date);
//			count = 10;
//		}
//		else if (count == 10)
//		{
//			m_ExamHeaders[i].m_Hour = atoi(Date);
//			count = 11;
//		}
//		else if (count == 11)
//		{
//			m_ExamHeaders[i].m_Minute = atoi(Date);
//			count = 12;
//		}
//		else if (count == 12)
//		{
//			m_ExamHeaders[i].m_Second = atoi(Date);
//			count = 13;
//		}
//		else if (count == 13)
//		{
//			m_ExamHeaders[i].m_Eye = atoi(Date);
//			count = 14;
//		}
//		else if (count == 14)
//		{
//			m_ExamHeaders[i].m_Preop = atoi(Date);
//			count = 15;
//		}
//		else if (count == 15)
//		{
//			m_ExamHeaders[i].m_SeriesNumber = atoi(Date);
//			count = 16;
//		}
//		else if (count == 16)
//		{
//			m_ExamHeaders[i].m_Note = Date;
//			count = 17;
//		}
//		else if (count == 17)
//		{
//			m_ExamHeaders[i].m_SoftwareVersion = atoi(Date);
//			m_ExamHeaders[i].m_Saved = TRUE;
//
//			count = 0;
//			i++;
//		}
//	}
//
//	fclose(pFileR);
//
//
//	for (int i = 0; i < m_NumExams - 1; i++)
//	{
//		for (int j = m_NumExams - 1; j > i; j--)
//		{
//			BOOL swap = FALSE;
//
//			if (m_ExamHeaders[j - 1].m_Year > m_ExamHeaders[j].m_Year)
//			{
//				swap = TRUE;
//			}
//			else if (m_ExamHeaders[j - 1].m_Year == m_ExamHeaders[j].m_Year)
//			{
//				if (m_ExamHeaders[j - 1].m_Month > m_ExamHeaders[j].m_Month)
//				{
//					swap = TRUE;
//				}
//				else if (m_ExamHeaders[j - 1].m_Month == m_ExamHeaders[j].m_Month)
//				{
//					if (m_ExamHeaders[j - 1].m_Day > m_ExamHeaders[j].m_Day) swap = TRUE;
//					else if (m_ExamHeaders[j - 1].m_Day == m_ExamHeaders[j].m_Day)
//					{
//						if (m_ExamHeaders[j - 1].m_Hour > m_ExamHeaders[j].m_Hour) swap = TRUE;
//						else if (m_ExamHeaders[j - 1].m_Hour == m_ExamHeaders[j].m_Hour)
//						{
//							if (m_ExamHeaders[j - 1].m_Minute > m_ExamHeaders[j].m_Minute) swap = TRUE;
//							else if (m_ExamHeaders[j - 1].m_Minute == m_ExamHeaders[j].m_Minute)
//							{
//								if (m_ExamHeaders[j - 1].m_Second > m_ExamHeaders[j].m_Second) swap = TRUE;
//							}
//						}
//					}
//				}
//			}
//			if (swap)
//			{
//				Swap(m_ExamHeaders[j - 1], m_ExamHeaders[j]);
//			}
//		}
//	}
//
//	for (int i = 0; i < m_NumExams; i++)
//	{
//		m_ExamHeaders[i].m_Number = i + 1;
//	}
//}
////Big Database Research since 03062019
//
////***************************************************************************************
//
////Big Database Research since 03062019
//BOOL CDB::SaveTxTExamHeader(const GUID& PatientID, ExamHeader_STR ThisExamStr)
//{
//	CString PatientIDStr;
//
//	GUIDToStr(PatientID, PatientIDStr);
//
//	CString FolderName;
//	GetFolderName(FolderName);
//
//	CStringA FileName = FolderName + "\\ExamHeader\\" + PatientIDStr + ".csv";
//
//	FILE* pFileW = fopen(FileName, "a");
//
//	fprintf(pFileW, "%s\n", ThisExamStr.ExamID_Str);
//	fprintf(pFileW, "%s\n", ThisExamStr.CalibrationID_Str);
//	fprintf(pFileW, "%s\n", ThisExamStr.ClinicIDStr);
//	fprintf(pFileW, "%s\n", ThisExamStr.PhysicianID_Str);
//	fprintf(pFileW, "%s\n", ThisExamStr.OperatorID_Str);
//	fprintf(pFileW, "%s\n", ThisExamStr.Type_Str);
//	fprintf(pFileW, "%s\n", ThisExamStr.Mode_Str);
//	fprintf(pFileW, "%s\n", ThisExamStr.Year_Str);
//	fprintf(pFileW, "%s\n", ThisExamStr.Month_Str);
//	fprintf(pFileW, "%s\n", ThisExamStr.Day_Str);
//	fprintf(pFileW, "%s\n", ThisExamStr.Hour_Str);
//	fprintf(pFileW, "%s\n", ThisExamStr.Minute_Str);
//	fprintf(pFileW, "%s\n", ThisExamStr.Second_Str);
//	fprintf(pFileW, "%s\n", ThisExamStr.Eye_Str);
//	fprintf(pFileW, "%s\n", ThisExamStr.Preop_Str);
//	fprintf(pFileW, "%s\n", ThisExamStr.SeriesNumber_Str);
//	fprintf(pFileW, "%s\n", ThisExamStr.Note_Str);
//	fprintf(pFileW, "%s\n", ThisExamStr.SoftwareVersion_Str);
//
//	fclose(pFileW);
//
//	return TRUE;
//}
////Big Database Research since 03062019
//
////***************************************************************************************
//
////Big Database Research since 03062019
//BOOL CDB::SaveTxTExamHeaders(CExamHeader* In_ExamHeader)
//{
//	LoadTxtExamHeaders(In_ExamHeader->m_PatientID);
//
//	int ExamSize = ::DB.m_ExamHeaders.GetSize();
//
//	CString IDStr;
//	CStringA StrA;
//	ExamHeader_STR ThisExamStr;
//
//	CString PatientIDStr;
//
//	GUIDToStr(In_ExamHeader->m_PatientID, PatientIDStr);
//
//	CString FolderName;
//	GetFolderName(FolderName);
//
//	CStringA FileName = FolderName + "\\ExamHeader\\" + PatientIDStr + ".csv";
//
//	BOOL Found = FALSE;
//
//	FILE* pFileW = fopen(FileName, "w");
//
//	for (int e = 0; e < ExamSize; e++)
//	{
//		CExamHeader* eExamHeader = &::DB.m_ExamHeaders[e];
//
//		if (eExamHeader->m_ExamID == In_ExamHeader->m_ExamID)
//		{
//			eExamHeader = In_ExamHeader;
//			Found = TRUE;
//		}
//
//		GUIDToStr(eExamHeader->m_ExamID, IDStr);
//		StrA = IDStr;
//		fprintf(pFileW, "%s\n", StrA);
//
//		GUIDToStr(eExamHeader->m_CalibrationID, IDStr);
//		StrA = IDStr;
//		fprintf(pFileW, "%s\n", StrA);
//
//		GUIDToStr(eExamHeader->m_ClinicID, IDStr);
//		StrA = IDStr;
//		fprintf(pFileW, "%s\n", StrA);
//
//		GUIDToStr(eExamHeader->m_PhysicianID, IDStr);
//		StrA = IDStr;
//		fprintf(pFileW, "%s\n", StrA);
//
//		GUIDToStr(eExamHeader->m_OperatorID, IDStr);
//		StrA = IDStr;
//		fprintf(pFileW, "%s\n", StrA);
//
//		StrA.Format("%i", eExamHeader->m_Type);
//		fprintf(pFileW, "%s\n", StrA);
//
//		StrA.Format("%i", eExamHeader->m_Mode);
//		fprintf(pFileW, "%s\n", StrA);
//
//		StrA.Format("%i", eExamHeader->m_Year);
//		fprintf(pFileW, "%s\n", StrA);
//
//		StrA.Format("%i", eExamHeader->m_Month);
//		fprintf(pFileW, "%s\n", StrA);
//
//		StrA.Format("%i", eExamHeader->m_Day);
//		fprintf(pFileW, "%s\n", StrA);
//
//		StrA.Format("%i", eExamHeader->m_Hour);
//		fprintf(pFileW, "%s\n", StrA);
//
//		StrA.Format("%i", eExamHeader->m_Minute);
//		fprintf(pFileW, "%s\n", StrA);
//
//		StrA.Format("%i", eExamHeader->m_Second);
//		fprintf(pFileW, "%s\n", StrA);
//
//		StrA.Format("%i", eExamHeader->m_Eye);
//		fprintf(pFileW, "%s\n", StrA);
//
//		StrA.Format("%i", eExamHeader->m_Preop);
//		fprintf(pFileW, "%s\n", StrA);
//
//		StrA.Format("%i", eExamHeader->m_SeriesNumber);
//		fprintf(pFileW, "%s\n", StrA);
//
//		StrA = eExamHeader->m_Note;
//		fprintf(pFileW, "%s\n", StrA);
//
//		StrA.Format("%i", eExamHeader->m_SoftwareVersion);
//		fprintf(pFileW, "%s\n", StrA);
//	}
//
//	if (!Found)
//	{
//		GUIDToStr(In_ExamHeader->m_ExamID, IDStr);
//		StrA = IDStr;
//		fprintf(pFileW, "%s\n", StrA);
//
//		GUIDToStr(In_ExamHeader->m_CalibrationID, IDStr);
//		StrA = IDStr;
//		fprintf(pFileW, "%s\n", StrA);
//
//		GUIDToStr(In_ExamHeader->m_ClinicID, IDStr);
//		StrA = IDStr;
//		fprintf(pFileW, "%s\n", StrA);
//
//		GUIDToStr(In_ExamHeader->m_PhysicianID, IDStr);
//		StrA = IDStr;
//		fprintf(pFileW, "%s\n", StrA);
//
//		GUIDToStr(In_ExamHeader->m_OperatorID, IDStr);
//		StrA = IDStr;
//		fprintf(pFileW, "%s\n", StrA);
//
//		StrA.Format("%i", In_ExamHeader->m_Type);
//		fprintf(pFileW, "%s\n", StrA);
//
//		StrA.Format("%i", In_ExamHeader->m_Mode);
//		fprintf(pFileW, "%s\n", StrA);
//
//		StrA.Format("%i", In_ExamHeader->m_Year);
//		fprintf(pFileW, "%s\n", StrA);
//
//		StrA.Format("%i", In_ExamHeader->m_Month);
//		fprintf(pFileW, "%s\n", StrA);
//
//		StrA.Format("%i", In_ExamHeader->m_Day);
//		fprintf(pFileW, "%s\n", StrA);
//
//		StrA.Format("%i", In_ExamHeader->m_Hour);
//		fprintf(pFileW, "%s\n", StrA);
//
//		StrA.Format("%i", In_ExamHeader->m_Minute);
//		fprintf(pFileW, "%s\n", StrA);
//
//		StrA.Format("%i", In_ExamHeader->m_Second);
//		fprintf(pFileW, "%s\n", StrA);
//
//		StrA.Format("%i", In_ExamHeader->m_Eye);
//		fprintf(pFileW, "%s\n", StrA);
//
//		StrA.Format("%i", In_ExamHeader->m_Preop);
//		fprintf(pFileW, "%s\n", StrA);
//
//		StrA.Format("%i", In_ExamHeader->m_SeriesNumber);
//		fprintf(pFileW, "%s\n", StrA);
//
//		StrA = In_ExamHeader->m_Note;
//		fprintf(pFileW, "%s\n", StrA);
//
//		StrA.Format("%i", In_ExamHeader->m_SoftwareVersion);
//		fprintf(pFileW, "%s\n", StrA);
//	}
//
//	fclose(pFileW);
//
//	return TRUE;
//}
////Big Database Research since 03062019
//***************************************************************************************

//***************************************************************************************
//
////Big Database Research since 03062019
//BOOL CDB::DelTxTExamHeader(const GUID& ExamID)
//{
//	//ABC
//}

 //***************************************************************************************
////Big Database Research since 03062019
//BOOL CDB::DelTxTExamHeader(const GUID& PatientID, const GUID& ExamID)
//{
//	LoadTxtExamHeaders(PatientID);//Big Database Research since 03062019
//
//	int ExamSize = ::DB.m_ExamHeaders.GetSize();
//
//	CString IDStr;
//	CStringA StrA;
//	ExamHeader_STR ThisExamStr;
//
//	CString PatientIDStr;
//
//	GUIDToStr(PatientID, PatientIDStr);
//
//	CString FolderName;
//	GetFolderName(FolderName);
//
//	CStringA FileName = FolderName + "\\ExamHeader\\" + PatientIDStr + ".csv";
//
//	FILE* pFileW = fopen(FileName, "w");
//
//	for (int e = 0; e < ExamSize; e++)
//	{
//		CExamHeader* eExamHeader = &::DB.m_ExamHeaders[e];
//
//		if (eExamHeader->m_ExamID == ExamID)
//		{
//			int a = 0;
//		}
//		else
//		{
//			GUIDToStr(eExamHeader->m_ExamID, IDStr);
//			StrA = IDStr;
//			fprintf(pFileW, "%s\n", StrA);
//
//			GUIDToStr(eExamHeader->m_CalibrationID, IDStr);
//			StrA = IDStr;
//			fprintf(pFileW, "%s\n", StrA);
//
//			GUIDToStr(eExamHeader->m_ClinicID, IDStr);
//			StrA = IDStr;
//			fprintf(pFileW, "%s\n", StrA);
//
//			GUIDToStr(eExamHeader->m_PhysicianID, IDStr);
//			StrA = IDStr;
//			fprintf(pFileW, "%s\n", StrA);
//
//			GUIDToStr(eExamHeader->m_OperatorID, IDStr);
//			StrA = IDStr;
//			fprintf(pFileW, "%s\n", StrA);
//
//			StrA.Format("%i", eExamHeader->m_Type);
//			fprintf(pFileW, "%s\n", StrA);
//
//			StrA.Format("%i", eExamHeader->m_Mode);
//			fprintf(pFileW, "%s\n", StrA);
//
//			StrA.Format("%i", eExamHeader->m_Year);
//			fprintf(pFileW, "%s\n", StrA);
//
//			StrA.Format("%i", eExamHeader->m_Month);
//			fprintf(pFileW, "%s\n", StrA);
//
//			StrA.Format("%i", eExamHeader->m_Day);
//			fprintf(pFileW, "%s\n", StrA);
//
//			StrA.Format("%i", eExamHeader->m_Hour);
//			fprintf(pFileW, "%s\n", StrA);
//
//			StrA.Format("%i", eExamHeader->m_Minute);
//			fprintf(pFileW, "%s\n", StrA);
//
//			StrA.Format("%i", eExamHeader->m_Second);
//			fprintf(pFileW, "%s\n", StrA);
//
//			StrA.Format("%i", eExamHeader->m_Eye);
//			fprintf(pFileW, "%s\n", StrA);
//
//			StrA.Format("%i", eExamHeader->m_Preop);
//			fprintf(pFileW, "%s\n", StrA);
//
//			StrA.Format("%i", eExamHeader->m_SeriesNumber);
//			fprintf(pFileW, "%s\n", StrA);
//
//			StrA = eExamHeader->m_Note;
//			fprintf(pFileW, "%s\n", StrA);
//
//			StrA.Format("%i", eExamHeader->m_SoftwareVersion);
//			fprintf(pFileW, "%s\n", StrA);
//		}
//	}
//
//	fclose(pFileW);
//
//	return TRUE;
//}
////Big Database Research since 03062019

//***************************************************************************************

BOOL CDB::LoadCalibration(const GUID& CalibrationID, CALIBRATION* pCalibration)
{
	int Type;
	GetType(Type);

	if (Type == DATABASE_TYPE_MSJET_OLD) {
		if (DllGetCalibrationSize(CalibrationID) != sizeof(CALIBRATION)) return FALSE;
		if (!DllLoadCalibration(CalibrationID, pCalibration, sizeof(CALIBRATION))) return FALSE;
	}

	else {
		int CompressedSize = DllGetCalibrationSize(CalibrationID);
		if (CompressedSize <= 0) return FALSE;
		void* pCompressedData = malloc(CompressedSize);
		if (!DllLoadCalibration(CalibrationID, pCompressedData, CompressedSize)) {
			free(pCompressedData);
			return FALSE;
		}
		if (::GetDecompressedSize(pCompressedData, CompressedSize) != sizeof(CALIBRATION)) {
			free(pCompressedData);
			return FALSE;
		}
		if (!::Decompress(pCompressedData, CompressedSize, pCalibration)) {
			free(pCompressedData);
			return FALSE;
		}
		free(pCompressedData);
	}

	return TRUE;
}

//***************************************************************************************

BOOL CDB::SaveCalibration(const GUID& CalibrationID, CALIBRATION* pCalibration)
{
	void* pZipData = malloc(sizeof(CALIBRATION));
	int ZipSize;
	BOOL Res = FALSE;
	if (::Compress(pCalibration, sizeof(CALIBRATION), pZipData, &ZipSize)) {
		if (DllSaveCalibration(CalibrationID, pZipData, ZipSize)) {
			Res = TRUE;
		}
	}
	free(pZipData);
	return Res;
}

//***************************************************************************************

BOOL CDB::SaveExam(CExam* pExam)
{
	if (pExam->m_Header.m_Type == EXAM_TYPE_WF) return SaveWFExam((CWFExam*)pExam);
	if (pExam->m_Header.m_Type == EXAM_TYPE_CT) return SaveCTExam((CCTExam*)pExam);
	return FALSE;
}

//***************************************************************************************

//[5.1.1]
BOOL CDB::ExamExist(const GUID& ExamID)
{
	int Type;
	GetType(Type);

	int DecompressedSize;

	if (Type == DATABASE_TYPE_MSJET_OLD) {
		DecompressedSize = DllGetExamSize(ExamID);
		if (DecompressedSize <= 0) return FALSE;
	}

	else {
		// Get compressed size
		int CompressedSize = DllGetExamSize(ExamID);
		if (CompressedSize <= 0) return FALSE;
	}

	return TRUE;
}
//[5.1.1]

//***************************************************************************************

//[5.1.1]
GUID CDB::LoadFellowID(const GUID& ExamID, real_t& scanSize, int& NumImages, int exam_type)
{
	scanSize = 0;
	GUID FellowExamID = GUID_NULL;

	int Type;
	GetType(Type);

	int DecompressedSize;
	uchar* pDecompressedData;

	if (Type == DATABASE_TYPE_MSJET_OLD)
	{
		DecompressedSize = DllGetExamSize(ExamID);

		if (DecompressedSize <= 0)
		{
			//free(pDecompressedData);
			return FellowExamID;
		}

		pDecompressedData = (uchar*)malloc(DecompressedSize);

		if (!DllLoadExam(ExamID, pDecompressedData, DecompressedSize))
		{
			//free(pDecompressedData);
			return FellowExamID;
		}
	}

	else
	{
		// Get compressed size
		int CompressedSize = DllGetExamSize(ExamID);

		if (CompressedSize <= 0)
		{
			//free(pDecompressedData);
			return FellowExamID;
		}

		// Allocate memory
		void* pCompressedData = malloc(CompressedSize);

		// Load compressed exam data
		if (!DllLoadExam(ExamID, pCompressedData, CompressedSize))
		{
			//free(pDecompressedData);
			free(pCompressedData);
			return FellowExamID;
		}

		// Decompress
		DecompressedSize = ::GetDecompressedSize(pCompressedData, CompressedSize);
		if (DecompressedSize <= 0)
		{
			//free(pDecompressedData);
			free(pCompressedData);
			return FellowExamID;
		}

		pDecompressedData = (uchar*)malloc(DecompressedSize);
		if (!::Decompress(pCompressedData, CompressedSize, pDecompressedData))
		{
			free(pDecompressedData);
			free(pCompressedData);
			return FellowExamID;
		}
		// Free memory
		free(pCompressedData);
	}


	uchar* p = pDecompressedData;
	TDB_EXAM_HEADER* pExamHeader = (TDB_EXAM_HEADER*)p;

	p += sizeof(TDB_EXAM_HEADER);

	if (exam_type == EXAM_TYPE_WF)
	{
		TDB_WF_EXAM_DATA* pExamData = (TDB_WF_EXAM_DATA*)p;
		if (!::Licensing.IsSolo())
		{
			if (pExamData->StructSize >= 6168) {
				FellowExamID = pExamData->FellowExamID;
			}
		}
		scanSize = pExamData->ScanDiameter;
		NumImages = pExamData->NumImages;//[5.2]
										 //free(pExamData);
	}
	else
	{
		if (!::Licensing.IsSolo())
		{
			TDB_CT_EXAM_DATA* pExamData = (TDB_CT_EXAM_DATA*)p;

			if (pExamData->StructSize >= 80788) {
				FellowExamID = pExamData->FellowExamID;
			}
			NumImages = pExamData->NumImages;//[520]
											 //free(pExamData);
		}
	}

	//free(p);
	free(pDecompressedData);
	return FellowExamID;
}
//[5.1.1]
//***************************************************************************************
//// Recover the exam patients;05202016 for a customer lost tdb data but has exam files.
////Big Database Research since 03062019
//BOOL CDB::SaveExamHeader(CExamHeader m_Header, int Type)
//{
//	//Big Database Research since 03062019
//	ExamHeader_STR ThisExamStr;
//	CString IDStr;
//
//	GUIDToStr(m_Header.m_ExamID, IDStr);
//
//	ThisExamStr.ExamID_Str = IDStr;
//
//	GUIDToStr(m_Header.m_CalibrationID, IDStr);
//	ThisExamStr.CalibrationID_Str = IDStr;
//
//	GUIDToStr(m_Header.m_ClinicID, IDStr);
//	ThisExamStr.ClinicIDStr = IDStr;
//
//	GUIDToStr(m_Header.m_PhysicianID, IDStr);
//	ThisExamStr.PhysicianID_Str = IDStr;
//
//	GUIDToStr(m_Header.m_OperatorID, IDStr);
//	ThisExamStr.OperatorID_Str = IDStr;
//
//	ThisExamStr.Type_Str.Format("%i", m_Header.m_Type);
//	ThisExamStr.Mode_Str.Format("%i", m_Header.m_Mode);
//	ThisExamStr.Year_Str.Format("%i", m_Header.m_Year);
//	ThisExamStr.Month_Str.Format("%i", m_Header.m_Month);
//	ThisExamStr.Day_Str.Format("%i", m_Header.m_Day);
//	ThisExamStr.Hour_Str.Format("%i", m_Header.m_Hour);
//	ThisExamStr.Minute_Str.Format("%i", m_Header.m_Minute);
//	ThisExamStr.Second_Str.Format("%i", m_Header.m_Second);
//	ThisExamStr.Eye_Str.Format("%i", m_Header.m_Eye);
//	ThisExamStr.Preop_Str.Format("%i", m_Header.m_Preop);
//	ThisExamStr.SeriesNumber_Str.Format("%i", m_Header.m_SeriesNumber);
//
//	ThisExamStr.Note_Str = m_Header.m_Note;
//	ThisExamStr.SoftwareVersion_Str.Format("%i", m_Header.m_SoftwareVersion);
//
//	ThisExamStr.Reserved3_Str = "-1";
//	ThisExamStr.Reserved4_Str = "-1";
//
//	return  TRUE;
//	//Big Database Research since 03062019
//
//
//	//Original Code
//	//int Size = sizeof(TDB_EXAM_HEADER) + sizeof(TDB_WF_EXAM_DATA);
//
//	//if (!Type) Size = sizeof(TDB_EXAM_HEADER) + sizeof(TDB_CT_EXAM_DATA);
//
//	//uchar* pData = (uchar*)malloc(Size);
//	//memset(pData, 0, Size);
//	//uchar* p = pData;
//
//	//// Fill out exam header
//	//TDB_EXAM_HEADER* pExamHeader = (TDB_EXAM_HEADER*)p;
//
//	//pExamHeader->ExamID = m_Header.m_ExamID;
//	//pExamHeader->PatientID = m_Header.m_PatientID;
//	//pExamHeader->CalibrationID = m_Header.m_CalibrationID;
//	//pExamHeader->ClinicID = m_Header.m_ClinicID;
//	//pExamHeader->PhysicianID = m_Header.m_PhysicianID;
//	//pExamHeader->OperatorID = m_Header.m_OperatorID;
//	//pExamHeader->Type = m_Header.m_Type;
//	//pExamHeader->Mode = m_Header.m_Mode;
//	//pExamHeader->Year = m_Header.m_Year;
//	//pExamHeader->Month = m_Header.m_Month;
//	//pExamHeader->Day = m_Header.m_Day;
//	//pExamHeader->Hour = m_Header.m_Hour;
//	//pExamHeader->Minute = m_Header.m_Minute;
//	//pExamHeader->Second = m_Header.m_Second;
//	//pExamHeader->Eye = m_Header.m_Eye;
//	//pExamHeader->Preop = m_Header.m_Preop;
//	//pExamHeader->SeriesNumber = m_Header.m_SeriesNumber;
//	//pExamHeader->SoftwareVersion = m_Header.m_SoftwareVersion;
//	//pExamHeader->Reserved3 = -1;
//	//pExamHeader->Reserved4 = -1;
//
//	//CStringA  Astr(m_Header.m_Note);
//	//strncpy(pExamHeader->Note, Astr, 100);
//
//	//if (DllSaveExamHeader(pExamHeader))
//	//	return TRUE;
//	//else
//	//	return FALSE;
//}

//***************************************************************************************

////Big Database Research since 03062019
//BOOL CDB::CheckExamsNum(CString DataFolderName)
//{
//	CString FolderName;
//	CFileFind finder;
//
//	CStringA FileName = DataFolderName + "\\ExamHeader\\ExamIdNum.txt";
//
//	//Read the Exams Numum
//	int ExamNum621, ExamNumPreV;
//
//	FILE* pFileR = fopen(FileName, "r");
//
//	CStringA Data;
//	char buf1[20];
//
//	fgets(buf1, 20, pFileR);
//
//	fclose(pFileR);
//
//	ExamNum621 = atoi(buf1);
//
//	//--------------------------------
//
//	FolderName = DataFolderName + "\\Exams";
//
//	BOOL working = finder.FindFile(FolderName + "\\*.exam");
//
//	ExamNumPreV = 0;
//	while (working)
//	{
//		working = finder.FindNextFile();
//		if (finder.IsDots())    continue;
//		if (finder.IsDirectory())
//		{
//		}
//		else
//		{
//			ExamNumPreV++;
//		}
//	}
//	//Read the ExamsID done
//
//	//If there are differents
//	if (ExamNum621 != ExamNumPreV)
//	{
//		return FALSE;
//	}
//	else
//	{
//		return TRUE;
//	}
//}
//
////Big Database Research since 03062019
//
////***************************************************************************************
//
////Big Database Research since 03062019
//void CDB::CheckPreNewExams()
//{
//	CString DataFolderName;
//	GetFolderName(DataFolderName);
//
//	//1. Check Patients
//	int TotPatCount = m_Patients.GetSize();
//	CStringA PatientIDs;
//	CString PatientID;
//
//	int St = (int)clock();
//
//	for (int p = 0; p < TotPatCount; p++)
//	{
//		CPatient* pPatient = &m_Patients[p];
//
//		GUIDToStr(pPatient->m_PatientID, PatientID);
//
//		PatientIDs += PatientID + "*";
//	}
//	//Read Current PatientIDs from
//
//	CFileFind finder;
//
//	int FoundID;
//
//	CString FolderName = DataFolderName + "\\ExamHeader";
//
//	BOOL working = finder.FindFile(FolderName + "\\*.csv");
//
//	while (working)
//	{
//		working = finder.FindNextFile();
//		if (finder.IsDots())    continue;
//		if (finder.IsDirectory())
//		{
//		}
//		else
//		{
//			CStringA ClassFileName = finder.GetFileName();
//
//			ClassFileName.Replace(".csv", "");
//
//			FoundID = PatientIDs.Find(ClassFileName, 0);
//
//			if (FoundID != -1)
//			{
//				PatientIDs.Replace(ClassFileName + "*", "");
//			}
//			else
//			{
//				//pre deletion or add by 6.2.1?
//			}
//		}
//	}
//
//	if (PatientIDs != "")
//	{
//		int Length;
//		GUID NewPatientID;
//		CString  IDStr;
//		CStringA pIDStr;
//		int ExamSize;
//		ExamHeader_STR ThisExamStr;
//
//		while (TRUE)
//		{
//			FoundID = PatientIDs.Find("*", 0);
//
//			pIDStr = PatientIDs.Left(FoundID );
//
//			StrToGUID(pIDStr, NewPatientID);
//
//			LoadExamHeaders(NewPatientID);
//
//			ExamSize = ::DB.m_ExamHeaders.GetSize();
//
//			for (int e = 0; e < ExamSize; e++)
//			{
//				CExamHeader* pExamHeader = &::DB.m_ExamHeaders[e];
//
//				GUIDToStr(pExamHeader->m_ExamID, IDStr);
//				ThisExamStr.ExamID_Str = IDStr;
//
//				GUIDToStr(pExamHeader->m_CalibrationID, IDStr);
//				ThisExamStr.CalibrationID_Str = IDStr;
//
//				GUIDToStr(pExamHeader->m_ClinicID, IDStr);
//				ThisExamStr.ClinicIDStr = IDStr;
//
//				GUIDToStr(pExamHeader->m_PhysicianID, IDStr);
//				ThisExamStr.PhysicianID_Str = IDStr;
//
//				GUIDToStr(pExamHeader->m_OperatorID, IDStr);
//				ThisExamStr.OperatorID_Str = IDStr;
//
//				ThisExamStr.Type_Str.Format("%i", pExamHeader->m_Type);
//				ThisExamStr.Mode_Str.Format("%i", pExamHeader->m_Mode);
//				ThisExamStr.Year_Str.Format("%i", pExamHeader->m_Year);
//				ThisExamStr.Month_Str.Format("%i", pExamHeader->m_Month);
//				ThisExamStr.Day_Str.Format("%i", pExamHeader->m_Day);
//				ThisExamStr.Hour_Str.Format("%i", pExamHeader->m_Hour);
//				ThisExamStr.Minute_Str.Format("%i", pExamHeader->m_Minute);
//				ThisExamStr.Second_Str.Format("%i", pExamHeader->m_Second);
//				ThisExamStr.Eye_Str.Format("%i", pExamHeader->m_Eye);
//				ThisExamStr.Preop_Str.Format("%i", pExamHeader->m_Preop);
//				ThisExamStr.SeriesNumber_Str.Format("%i", pExamHeader->m_SeriesNumber);
//
//				ThisExamStr.Note_Str = pExamHeader->m_Note;
//				ThisExamStr.SoftwareVersion_Str.Format("%i", pExamHeader->m_SoftwareVersion);
//
//				ThisExamStr.Reserved3_Str = "-1";
//				ThisExamStr.Reserved4_Str = "-1";
//
//				SaveTxTExamHeader(NewPatientID, ThisExamStr);
//			}
//
//
//			PatientIDs.Replace(pIDStr + "*", "");
//
//			Length = PatientIDs.GetLength();
//			if (Length == 0)
//			{
//				break;
//			}
//		}
//	}
//
//	//int Ut = (int)clock() - St;
//
//	//int a = 0;
//}
////Big Database Research since 03062019

//***************************************************************************************
BOOL CDB::LoadWFExam(const GUID& ExamID, CWFExam* pWFExam)
{
	int Type;
	GetType(Type);

	pWFExam->m_Points.Destroy();
	pWFExam->m_Image.Destroy();
	pWFExam->m_ColorImage.Destroy();

	if (m_hDLL == NULL) return FALSE;

	int DecompressedSize;
	uchar* pDecompressedData;

	if (Type == DATABASE_TYPE_MSJET_OLD)
	{
		DecompressedSize = DllGetExamSize(ExamID);
		if (DecompressedSize <= 0) return FALSE;
		pDecompressedData = (uchar*)malloc(DecompressedSize);
		if (!DllLoadExam(ExamID, pDecompressedData, DecompressedSize))
		{
			free(pDecompressedData);
			return FALSE;
		}
	}

	else
	{
		 //Get compressed size
		int CompressedSize = DllGetExamSize(ExamID);
		if (CompressedSize <= 0) return FALSE;
		// Allocate memory
		void* pCompressedData = malloc(CompressedSize);
		// Load compressed exam data
		if (!DllLoadExam(ExamID, pCompressedData, CompressedSize))
		{
			free(pCompressedData);
			return FALSE;
		}
		// Decompress
		DecompressedSize = ::GetDecompressedSize(pCompressedData, CompressedSize);
		if (DecompressedSize <= 0) {
			free(pCompressedData);
			return FALSE;
		}
		pDecompressedData = (uchar*)malloc(DecompressedSize);
		if (!::Decompress(pCompressedData, CompressedSize, pDecompressedData))
		{
			free(pDecompressedData);
			free(pCompressedData);
			return FALSE;
		}
		// Free memory
		free(pCompressedData);
	}

	uchar* p = pDecompressedData;
	TDB_EXAM_HEADER* pExamHeader = (TDB_EXAM_HEADER*)p;

	pWFExam->m_Header.m_ExamID = pExamHeader->ExamID;
	pWFExam->m_Header.m_PatientID = pExamHeader->PatientID;
	pWFExam->m_Header.m_CalibrationID = pExamHeader->CalibrationID;
	pWFExam->m_Header.m_ClinicID = pExamHeader->ClinicID;
	pWFExam->m_Header.m_PhysicianID = pExamHeader->PhysicianID;
	pWFExam->m_Header.m_OperatorID = pExamHeader->OperatorID;
	pWFExam->m_Header.m_Type = pExamHeader->Type;
	pWFExam->m_Header.m_Year = pExamHeader->Year;
	pWFExam->m_Header.m_Month = pExamHeader->Month;
	pWFExam->m_Header.m_Day = pExamHeader->Day;
	pWFExam->m_Header.m_Hour = pExamHeader->Hour;
	pWFExam->m_Header.m_Minute = pExamHeader->Minute;
	pWFExam->m_Header.m_Second = pExamHeader->Second;
	pWFExam->m_Header.m_Eye = pExamHeader->Eye;
	pWFExam->m_Header.m_Preop = pExamHeader->Preop;
	pWFExam->m_Header.m_SeriesNumber = pExamHeader->SeriesNumber;
	pWFExam->m_Header.m_Mode = pExamHeader->Mode;

	if (Type == DATABASE_TYPE_MSJET_OLD)
	{
		pWFExam->m_Header.m_SoftwareVersion = SOFTWARE_VERSION_3;
	}
	else if (pExamHeader->SoftwareVersion == 0xffffffff)
	{
		pWFExam->m_Header.m_SoftwareVersion = SOFTWARE_VERSION_4;
	}
	else
	{
		pWFExam->m_Header.m_SoftwareVersion = pExamHeader->SoftwareVersion;
	}
	pWFExam->m_Header.m_Note = pExamHeader->Note;
	pWFExam->m_Header.m_Saved = TRUE;

	p += sizeof(TDB_EXAM_HEADER);

	TDB_WF_EXAM_DATA* pExamData = (TDB_WF_EXAM_DATA*)p;

	pWFExam->m_ScanDiameter = pExamData->ScanDiameter;
	pWFExam->m_AccommTargetPosition = pExamData->AccommTargetPosition;
	pWFExam->m_PrecalcSphere = pExamData->PrecalcSphere;
	pWFExam->m_PrecalcCylinder = pExamData->PrecalcCylinder;
	pWFExam->m_PrecalcAxis = pExamData->PrecalcAxis;
	pWFExam->m_WavetouchLensPower = pExamData->WavetouchLensPower;
	pWFExam->m_WavetouchLensBaseCurve = pExamData->WavetouchLensBaseCurve;
	pWFExam->m_DetectorType = pExamData->DetectorType;

	// Wavetouch lens dots
	CEyeImage* pImage = &pWFExam->m_Image;
	BOOL OK = pExamData->StructSize >= 117 && pExamData->WavetouchLensDotsOK == 1;
	pImage->m_le_ok = OK;
	for (int i = 0; i < 3; i++) {
		pImage->m_le_x_um[i] = OK ? pExamData->WavetouchLensDotX[i] : 0.0;
		pImage->m_le_y_um[i] = OK ? pExamData->WavetouchLensDotY[i] : 0.0;
	}
	// Corneal vertex
	OK = pExamData->StructSize >= 134 && pExamData->VertexOK == 1;
	pImage->m_ve_ok = OK;
	pImage->m_ve_x_um = OK ? pExamData->VertexX : 0.0;
	pImage->m_ve_y_um = OK ? pExamData->VertexY : 0.0;
	// Pupil
	OK = pExamData->StructSize >= 3055 && pExamData->PupilOK == 1;
	pImage->m_pu_ok = OK;
	pImage->m_pu_x0_um = OK ? pExamData->PupilX0 : 0.0;
	pImage->m_pu_y0_um = OK ? pExamData->PupilY0 : 0.0;
	for (int i = 0; i < 360; i++) {
		pImage->m_pu_r_um[i] = OK ? pExamData->PupilR[i] : 0.0;
	}
	pImage->m_pu_r_min_um = OK ? pExamData->PupilRMin : 0.0;
	pImage->m_pu_r_max_um = OK ? pExamData->PupilRMax : 0.0;
	pImage->m_pu_r_mean_um = OK ? pExamData->PupilRMean : 0.0;
	// AOD fix
	pWFExam->m_AODsOK = pExamData->StructSize >= 3056 && pExamData->AODsOK == 1;
	// Limbus
	OK = pExamData->StructSize >= 5977 && pExamData->LimbusOK == 1;
	pImage->m_li_ok = OK;
	pImage->m_li_x0_um = OK ? pExamData->LimbusX0 : 0.0;
	pImage->m_li_y0_um = OK ? pExamData->LimbusY0 : 0.0;
	for (int i = 0; i < 360; i++)
	{
		pImage->m_li_r_um[i] = OK ? pExamData->LimbusR[i] : 0.0;
	}
	pImage->m_li_r_min_um = OK ? pExamData->LimbusRMin : 0.0;
	pImage->m_li_r_max_um = OK ? pExamData->LimbusRMax : 0.0;
	pImage->m_li_r_mean_um = OK ? pExamData->LimbusRMean : 0.0;


	// Save date & time
	OK = pExamData->StructSize >= 6001;
	pWFExam->m_Header.m_SaveYear = OK ? pExamData->SaveYear : pWFExam->m_Header.m_Year;
	pWFExam->m_Header.m_SaveMonth = OK ? pExamData->SaveMonth : pWFExam->m_Header.m_Month;
	pWFExam->m_Header.m_SaveDay = OK ? pExamData->SaveDay : pWFExam->m_Header.m_Day;
	pWFExam->m_Header.m_SaveHour = OK ? pExamData->SaveHour : pWFExam->m_Header.m_Hour;
	pWFExam->m_Header.m_SaveMinute = OK ? pExamData->SaveMinute : pWFExam->m_Header.m_Minute;
	pWFExam->m_Header.m_SaveSecond = OK ? pExamData->SaveSecond : pWFExam->m_Header.m_Second;
	// Wavetouch extra information
	OK = pExamData->StructSize >= 6151;
	pWFExam->m_WavetouchOther = OK ? pExamData->WavetouchOther : "";
	pWFExam->m_WavetouchNote = OK ? pExamData->WavetouchNote : "";
	pWFExam->m_WavetouchVertex = OK ? pExamData->WavetouchVertex : 0.0;
	pWFExam->m_WavetouchSph = OK ? pExamData->WavetouchSph : INVALID_VALUE;
	pWFExam->m_WavetouchCyl = OK ? pExamData->WavetouchCyl : INVALID_VALUE;
	pWFExam->m_WavetouchAxis = OK ? pExamData->WavetouchAxis : INVALID_VALUE;
	// Alignment method
	if (pExamData->StructSize >= 6152)
	{
		pWFExam->m_AlignmentMethod = pExamData->AlignmentMethod;
	}
	else {
		pWFExam->m_AlignmentMethod = ALIGNMENT_PUPIL_CENTER;
	}

	//Fellow exam ID
	if (pExamData->StructSize >= 6168)
	{
		pWFExam->m_Header.m_FellowExamID = pExamData->FellowExamID;
	}
	else {
		pWFExam->m_Header.m_FellowExamID = GUID_NULL;
	}

	// Scan decentration
	if (pExamData->StructSize >= 6184)
	{
		pWFExam->m_sc_x0_um = pExamData->ScanX0;
		pWFExam->m_sc_y0_um = pExamData->ScanY0;
	}
	else {
		pWFExam->m_sc_x0_um = 0.0;
		pWFExam->m_sc_y0_um = 0.0;
	}

	//520
	//Radial Ruler and Inlay
	//6.2.0 New Bug
	BOOL RaOk = (pExamData->RadialRulerR > 0);
	OK = ( pExamData->StructSize >= 6209 && pExamData->RadialRulerOk == 1 && RaOk);
	pImage->m_Ra_ok = OK;
	pImage->m_Ra_x_um = OK ? pExamData->RadialRulerX0 : 0.0;
	pImage->m_Ra_y_um = OK ? pExamData->RadialRulerY0 : 0.0;
	pImage->m_Ra_r_um = OK ? pExamData->RadialRulerR : 0.0;

	BOOL InlayOk = (pExamData->InlayR0 > 0);//6.2.0 New Bug
	OK = (pExamData->StructSize >= 6242 && pExamData->InlayOk == 1 && InlayOk);
	pImage->m_In_ok = OK;
	pImage->m_In_x_um = OK ? pExamData->InlayX0 : 0.0;
	pImage->m_In_y_um = OK ? pExamData->InlayY0 : 0.0;
	pImage->m_In_r_um = OK ? pExamData->InlayR0 : 0.0;
	pImage->m_In_In_r_um = OK ? pExamData->InlayInR0 : 0.0;

	real_t Dis = sqrt((pExamData->LinearRulerX1 - pExamData->LinearRulerX0)*(pExamData->LinearRulerX1 - pExamData->LinearRulerX0) + (pExamData->LinearRulerY1 - pExamData->LinearRulerY0)*(pExamData->LinearRulerY1 - pExamData->LinearRulerY0));
	BOOL RulerOk = (Dis > 0 );//6.2.0 New Bug
	OK = (pExamData->StructSize >= 6275 && pExamData->LinearRulerOk == 1 && RulerOk);
	pImage->m_LRa_ok = OK;
	pImage->m_LRa_x0_um = OK ? pExamData->LinearRulerX0 : 0.0;
	pImage->m_LRa_y0_um = OK ? pExamData->LinearRulerY0 : 0.0;
	pImage->m_LRa_x1_um = OK ? pExamData->LinearRulerX1 : 0.0;
	pImage->m_LRa_y1_um = OK ? pExamData->LinearRulerY1 : 0.0;
	//520

	p += pExamData->StructSize;

	// Points
	for (int i = 0; i < pExamData->NumPoints; i++) {
		switch (pExamData->DetectorType) {
		case DT_QUADRANT:
		{
			CScanPoint* pScanPoint = pWFExam->m_Points.Append();
			TDB_POINT_0* pPoint = (TDB_POINT_0*)p;
			pScanPoint->m_shot = pPoint->ShotNumber;
			pScanPoint->m_Bad = pPoint->Bad;
			pScanPoint->m_r_um = pPoint->Radius;
			pScanPoint->m_a_rd = pPoint->Angle;
			pScanPoint->m_dx_um = pPoint->DX;
			pScanPoint->m_dy_um = pPoint->DY;
		}
		break;
		case DT_LINEAR_ARRAYS:
		{
			CScanPoint* pScanPoint = pWFExam->m_Points.Append();
			TDB_POINT_1* pPoint = (TDB_POINT_1*)p;
			pScanPoint->m_shot = pPoint->ShotNumber;
			pScanPoint->m_Bad = pPoint->Bad;
			pScanPoint->m_r_um = pPoint->Radius;
			pScanPoint->m_a_rd = pPoint->Angle;

			ushort Signal[512];
			memcpy(&Signal, pPoint->DataX, 1024);

			short Signal2[512];
			memcpy(&Signal2, pPoint->DataX, 1024);

			memcpy(&pScanPoint->m_LADX.m_Signal, pPoint->DataX, 1024);
			memcpy(&pScanPoint->m_LADY.m_Signal, pPoint->DataY, 1024);
			/*
			if (!pWFExam->m_AODsOK) {
			pScanPoint->m_a_rd -= _Pi;
			if (pScanPoint->m_a_rd < 0.0) pScanPoint->m_a_rd += _2_Pi;
			for (int i = 0; i < 256; i++) {
			Swap(pScanPoint->m_LADX.m_Signal[i], pScanPoint->m_LADX.m_Signal[511 - i]);
			Swap(pScanPoint->m_LADY.m_Signal[i], pScanPoint->m_LADY.m_Signal[511 - i]);
			}
			}
			*/

		}
		break;
		}
		p += pExamData->PointSize;
	}

	if (pExamData->NumImages > 3) return FALSE;//

	// Image
	if (pExamData->NumImages > 0)
	{
		int JPGSize = *((int*)p);
		p += sizeof(int);
		pWFExam->m_Image.LoadFromMem(JPGSize, p);
		p += JPGSize;
	}
	// Color image
	if (pExamData->NumImages > 1)
	{
		int JPGSize = *((int*)p);
		p += sizeof(int);
		pWFExam->m_ColorImage.LoadFromMem(JPGSize, p);
		p += JPGSize;
	}

	// Free memory
	free(pDecompressedData);

	// Calibration
	if (!LoadCalibration(pWFExam->m_Header.m_CalibrationID, &pWFExam->m_Calibration))
		return FALSE;

	////6.2.0
	//pExamData->RadialRulerOk = pImage->m_Ra_ok;
	//pExamData->InlayOk = pImage->m_In_ok;
	//pExamData->LinearRulerOk = pImage->m_LRa_ok;
	////6.2.0

	return TRUE;
}

//***************************************************************************************

BOOL CDB::SaveWFExam(CWFExam* pWFExam)
{
	if (m_hDLL == NULL) return FALSE;
	if (IsReadOnly()) return FALSE;

	// Calibration
	if (!SaveCalibration(pWFExam->m_Calibration.CalibrationID, &pWFExam->m_Calibration)) {
		::Error("Failed to save exam. Error code: 1.");
		return FALSE;
	}

	// Calculate exam size
	int Size = sizeof(TDB_EXAM_HEADER) + sizeof(TDB_WF_EXAM_DATA);

	int NumPoints = pWFExam->m_Points.GetSize();
	int PointSize;
	switch (pWFExam->m_DetectorType) {
	case DT_QUADRANT:
		PointSize = sizeof(TDB_POINT_0);
		break;
	case DT_LINEAR_ARRAYS:
		PointSize = sizeof(TDB_POINT_1);
		break;
	default:
		PointSize = 0;
	}
	Size += NumPoints * PointSize;

	int NumImages = 0;
	int JPGSize[2] = { 0, 0 };
	uchar* pJPGData[2] = { NULL, NULL };
	if (pWFExam->m_Image.m_RGBData.GetMem()) {
		pWFExam->m_Image.SaveIntoMem(&JPGSize[0], &pJPGData[0], COMPRESSION_JPG);
		Size += sizeof(int) + JPGSize[0];
		NumImages++;
	}
	if (pWFExam->m_ColorImage.m_RGBData.GetMem()) {
		pWFExam->m_ColorImage.SaveIntoMem(&JPGSize[1], &pJPGData[1], COMPRESSION_JPG);
		Size += sizeof(int) + JPGSize[1];
		NumImages++;
	}

	// Allocate memory
	uchar* pData = (uchar*)malloc(Size);
	memset(pData, 0, Size);

	uchar* p = pData;

	// Fill out exam header
  auto pExamHeader = reinterpret_cast<TDB_EXAM_HEADER *>(p);

  pExamHeader->ExamID          = pWFExam->m_Header.m_ExamID;
  pExamHeader->PatientID       = pWFExam->m_Header.m_PatientID;
  pExamHeader->CalibrationID   = pWFExam->m_Header.m_CalibrationID;
  pExamHeader->ClinicID        = pWFExam->m_Header.m_ClinicID;
  pExamHeader->PhysicianID     = pWFExam->m_Header.m_PhysicianID;
  pExamHeader->OperatorID      = pWFExam->m_Header.m_OperatorID;
  pExamHeader->Type            = pWFExam->m_Header.m_Type;
  pExamHeader->Mode            = pWFExam->m_Header.m_Mode;
  pExamHeader->Year            = pWFExam->m_Header.m_Year;
  pExamHeader->Month           = pWFExam->m_Header.m_Month;
  pExamHeader->Day             = pWFExam->m_Header.m_Day;
  pExamHeader->Hour            = pWFExam->m_Header.m_Hour;
  pExamHeader->Minute          = pWFExam->m_Header.m_Minute;
  pExamHeader->Second          = pWFExam->m_Header.m_Second;
  pExamHeader->Eye             = pWFExam->m_Header.m_Eye;
  pExamHeader->Preop           = pWFExam->m_Header.m_Preop;
  pExamHeader->SeriesNumber    = pWFExam->m_Header.m_SeriesNumber;
  pExamHeader->SoftwareVersion = pWFExam->m_Header.m_SoftwareVersion;
  pExamHeader->analogCamera    = AcquisitionParameters::AnalogCamera();

	G_As = pWFExam->m_Header.m_Note;
	strncpy(pExamHeader->Note, G_As, 100);
	//Big Database Research since 03062019

	p += sizeof(TDB_EXAM_HEADER);

	// Fill out exam data header
	TDB_WF_EXAM_DATA* pExamData = (TDB_WF_EXAM_DATA*)p;
	pExamData->StructSize = sizeof(TDB_WF_EXAM_DATA);
	pExamData->ScanDiameter = pWFExam->m_ScanDiameter;
	pExamData->AccommTargetPosition = pWFExam->m_AccommTargetPosition;
	pExamData->PrecalcSphere = pWFExam->m_PrecalcSphere;
	pExamData->PrecalcCylinder = pWFExam->m_PrecalcCylinder;
	pExamData->PrecalcAxis = pWFExam->m_PrecalcAxis;
	pExamData->WavetouchLensPower = pWFExam->m_WavetouchLensPower;
	pExamData->WavetouchLensBaseCurve = pWFExam->m_WavetouchLensBaseCurve;
	pExamData->DetectorType = pWFExam->m_DetectorType;
	pExamData->NumPoints = NumPoints;
	pExamData->PointSize = PointSize;
	pExamData->NumImages = NumImages;

	// Wavetouch lens dots
	CEyeImage* pImage = &pWFExam->m_Image;
	BOOL OK = pImage->m_le_ok;
	pExamData->WavetouchLensDotsOK = OK ? 1 : 0;
	for (int i = 0; i < 3; i++) {
		pExamData->WavetouchLensDotX[i] = OK ? pImage->m_le_x_um[i] : 0.0;
		pExamData->WavetouchLensDotY[i] = OK ? pImage->m_le_y_um[i] : 0.0;
	}
	// Corneal vertex
	OK = pImage->m_ve_ok;
	pExamData->VertexOK = OK ? 1 : 0;
	pExamData->VertexX = OK ? pImage->m_ve_x_um : 0.0;
	pExamData->VertexY = OK ? pImage->m_ve_y_um : 0.0;
	// Pupil
	OK = pImage->m_pu_ok;
	pExamData->PupilOK = OK ? 1 : 0;
	pExamData->PupilX0 = OK ? pImage->m_pu_x0_um : 0.0;
	pExamData->PupilY0 = OK ? pImage->m_pu_y0_um : 0.0;
	for (int i = 0; i < 360; i++) {
		pExamData->PupilR[i] = OK ? pImage->m_pu_r_um[i] : 0.0;
	}
	pExamData->PupilRMin = OK ? pImage->m_pu_r_min_um : 0.0;
	pExamData->PupilRMax = OK ? pImage->m_pu_r_max_um : 0.0;
	pExamData->PupilRMean = OK ? pImage->m_pu_r_mean_um : 0.0;
	// AOD fix
	pExamData->AODsOK == pWFExam->m_AODsOK ? 1 : 0;
	// Limbus
	OK = pImage->m_li_ok;
	pExamData->LimbusOK = OK ? 1 : 0;
	pExamData->LimbusX0 = OK ? pImage->m_li_x0_um : 0.0;
	pExamData->LimbusY0 = OK ? pImage->m_li_y0_um : 0.0;
	for (int i = 0; i < 360; i++) {
		pExamData->LimbusR[i] = OK ? pImage->m_li_r_um[i] : 0.0;
	}
	pExamData->LimbusRMin = OK ? pImage->m_li_r_min_um : 0.0;
	pExamData->LimbusRMax = OK ? pImage->m_li_r_max_um : 0.0;
	pExamData->LimbusRMean = OK ? pImage->m_li_r_mean_um : 0.0;

	// Save date & time
	SYSTEMTIME Time;
	::GetLocalTime(&Time);
	pExamData->SaveYear = Time.wYear;
	pExamData->SaveMonth = Time.wMonth;
	pExamData->SaveDay = Time.wDay;
	pExamData->SaveHour = Time.wHour;
	pExamData->SaveMinute = Time.wMinute;
	pExamData->SaveSecond = Time.wSecond;
	// Wavetouch extra information
	G_As = pWFExam->m_WavetouchOther;
	strncpy(pExamData->WavetouchOther, G_As, 20);
	G_As = pWFExam->m_WavetouchNote;
	strncpy(pExamData->WavetouchNote, G_As, 100);
	pExamData->WavetouchVertex = pWFExam->m_WavetouchVertex;
	pExamData->WavetouchSph = pWFExam->m_WavetouchSph;
	pExamData->WavetouchCyl = pWFExam->m_WavetouchCyl;
	pExamData->WavetouchAxis = pWFExam->m_WavetouchAxis;
	// Alignment method
	pExamData->AlignmentMethod = pWFExam->m_AlignmentMethod;
	// Fellow exam ID
	pExamData->FellowExamID = pWFExam->m_Header.m_FellowExamID;
	// Scan decentration
	pExamData->ScanX0 = pWFExam->m_sc_x0_um;
	pExamData->ScanY0 = pWFExam->m_sc_y0_um;

	//520
	//Ruler and InLay
	OK = pImage->m_Ra_ok;
	pExamData->RadialRulerOk = OK ? 1 : 0;
	pExamData->RadialRulerX0 = OK ? pImage->m_Ra_x_um : 0.0;
	pExamData->RadialRulerY0 = OK ? pImage->m_Ra_y_um : 0.0;
	pExamData->RadialRulerR = OK ? pImage->m_Ra_r_um : 0.0;

	OK = pImage->m_In_ok;
	pExamData->InlayOk = OK ? 1 : 0;
	pExamData->InlayX0 = OK ? pImage->m_In_x_um : 0.0;
	pExamData->InlayY0 = OK ? pImage->m_In_y_um : 0.0;
	pExamData->InlayR0 = OK ? pImage->m_In_r_um : 0.0;
	pExamData->InlayInR0 = OK ? pImage->m_In_In_r_um : 0.0;

	OK = pImage->m_LRa_ok;
	pExamData->LinearRulerOk = OK ? 1 : 0;
	pExamData->LinearRulerX0 = OK ? pImage->m_LRa_x0_um : 0.0;
	pExamData->LinearRulerY0 = OK ? pImage->m_LRa_y0_um : 0.0;
	pExamData->LinearRulerX1 = OK ? pImage->m_LRa_x1_um : 0.0;
	pExamData->LinearRulerY1 = OK ? pImage->m_LRa_y1_um : 0.0;
	//520

	p += sizeof(TDB_WF_EXAM_DATA);

	// Points
	CScanPoint* pScanPoint = pWFExam->m_Points.MoveFirst();
	while (pScanPoint) {
		switch (pWFExam->m_DetectorType) {
		case DT_QUADRANT:
		{
			TDB_POINT_0* pPoint = (TDB_POINT_0*)p;
			pPoint->ShotNumber = pScanPoint->m_shot;
			pPoint->Bad = pScanPoint->m_Bad;
			pPoint->Radius = pScanPoint->m_r_um;
			pPoint->Angle = pScanPoint->m_a_rd;
			pPoint->DX = pScanPoint->m_dx_um;
			pPoint->DY = pScanPoint->m_dy_um;
		}
		break;
		case DT_LINEAR_ARRAYS:
		{
			TDB_POINT_1* pPoint = (TDB_POINT_1*)p;
			pPoint->ShotNumber = pScanPoint->m_shot;
			pPoint->Bad = pScanPoint->m_Bad;
			pPoint->Radius = pScanPoint->m_r_um;
			pPoint->Angle = pScanPoint->m_a_rd;
			memcpy(pPoint->DataX, &pScanPoint->m_LADX.m_Signal, 1024);
			memcpy(pPoint->DataY, &pScanPoint->m_LADY.m_Signal, 1024);
		}
		break;
		}
		p += PointSize;
		pScanPoint = pWFExam->m_Points.MoveNext();
	}

	// Image
	if (pJPGData[0]) {
		*((int*)p) = JPGSize[0];
		p += sizeof(int);
		memcpy(p, pJPGData[0], JPGSize[0]);
		p += JPGSize[0];
		free(pJPGData[0]);
	}
	// Color image
	if (pJPGData[1]) {
		*((int*)p) = JPGSize[1];
		p += sizeof(int);
		memcpy(p, pJPGData[1], JPGSize[1]);
		p += JPGSize[1];
		free(pJPGData[1]);
	}

	// Compress
	void* pCompressedData = malloc(Size);
	int CompressedSize;
	::Compress(pData, Size, pCompressedData, &CompressedSize);

	// Save exam
	if (DllSaveExamHeader(pExamHeader))//Big Database Research since 03062019
	//if (SaveTxTExamHeaders(&pWFExam->m_Header))//Big Database Research since 03062019
	{
		if (DllSaveExam(pWFExam->m_Header.m_ExamID, pCompressedData, CompressedSize)) {
			pWFExam->m_Header.m_Saved = TRUE;
		}
		else {
			::Error("Failed to save exam. Error code: 4.");
		}
	}
	else {
		::Error("Failed to save exam. Error code: 3.");
	}

	// Free memory
	free(pCompressedData);
	free(pData);

	//GenerateExamAmount();//Big Database Research since 03062019

	return pWFExam->m_Header.m_Saved;
}

//***************************************************************************************

BOOL CDB::LoadCTExam(const GUID& ExamID, CCTExam* pCTExam)
{
	int Type;
	GetType(Type);

	pCTExam->m_Image.Destroy();

	if (m_hDLL == NULL) return FALSE;

	int DecompressedSize;
	uchar* pDecompressedData;

	if (Type == DATABASE_TYPE_MSJET_OLD) {
		DecompressedSize = DllGetExamSize(ExamID);
		if (DecompressedSize <= 0) return FALSE;
		pDecompressedData = (uchar*)malloc(DecompressedSize);
		if (!DllLoadExam(ExamID, pDecompressedData, DecompressedSize)) {
			free(pDecompressedData);
			return FALSE;
		}
	}

	else {
		// Get compressed size
		int CompressedSize = DllGetExamSize(ExamID);
		if (CompressedSize <= 0) return FALSE;
		// Allocate memory
		void* pCompressedData = malloc(CompressedSize);
		// Load compressed exam data
		if (!DllLoadExam(ExamID, pCompressedData, CompressedSize)) {
			free(pCompressedData);
			return FALSE;
		}
		// Decompress
		DecompressedSize = ::GetDecompressedSize(pCompressedData, CompressedSize);
		if (DecompressedSize <= 0) {
			free(pCompressedData);
			return FALSE;
		}
		pDecompressedData = (uchar*)malloc(DecompressedSize);
		if (!::Decompress(pCompressedData, CompressedSize, pDecompressedData)) {
			free(pDecompressedData);
			free(pCompressedData);
			return FALSE;
		}
		// Free memory
		free(pCompressedData);
	}

	uchar* p = pDecompressedData;
	TDB_EXAM_HEADER* pExamHeader = (TDB_EXAM_HEADER*)p;

	pCTExam->m_Header.m_ExamID = pExamHeader->ExamID;
	pCTExam->m_Header.m_PatientID = pExamHeader->PatientID;
	pCTExam->m_Header.m_CalibrationID = pExamHeader->CalibrationID;
	pCTExam->m_Header.m_ClinicID = pExamHeader->ClinicID;
	pCTExam->m_Header.m_PhysicianID = pExamHeader->PhysicianID;
	pCTExam->m_Header.m_OperatorID = pExamHeader->OperatorID;
	pCTExam->m_Header.m_Type = pExamHeader->Type;
	pCTExam->m_Header.m_Year = pExamHeader->Year;
	pCTExam->m_Header.m_Month = pExamHeader->Month;
	pCTExam->m_Header.m_Day = pExamHeader->Day;
	pCTExam->m_Header.m_Hour = pExamHeader->Hour;
	pCTExam->m_Header.m_Minute = pExamHeader->Minute;
	pCTExam->m_Header.m_Second = pExamHeader->Second;
	pCTExam->m_Header.m_Eye = pExamHeader->Eye;
	pCTExam->m_Header.m_Preop = pExamHeader->Preop;
	pCTExam->m_Header.m_SeriesNumber = pExamHeader->SeriesNumber;
	pCTExam->m_Header.m_Mode = pExamHeader->Mode;
	if (Type == DATABASE_TYPE_MSJET_OLD) {
		pCTExam->m_Header.m_SoftwareVersion = SOFTWARE_VERSION_3;
	}
	else if (pExamHeader->SoftwareVersion == 0xffffffff) {
		pCTExam->m_Header.m_SoftwareVersion = SOFTWARE_VERSION_4;
	}
	else {
		pCTExam->m_Header.m_SoftwareVersion = pExamHeader->SoftwareVersion;
	}
	pCTExam->m_Header.m_Note = pExamHeader->Note;
	pCTExam->m_Header.m_Saved = TRUE;

	p += sizeof(TDB_EXAM_HEADER);

	TDB_CT_EXAM_DATA* pExamData = (TDB_CT_EXAM_DATA*)p;

	//520
	int numberImgs = 1;
	if (pExamData->StructSize >= 8)
	{
		numberImgs = pExamData->NumImages;
	}
	//520

	// Limbus
	CEyeImage* pImage = &pCTExam->m_Image;
	BOOL OK = pExamData->StructSize >= 2929 && pExamData->LimbusOK == 1;
	pImage->m_li_ok = OK;
	pImage->m_li_x0_um = OK ? pExamData->LimbusX0 : 0.0;
	pImage->m_li_y0_um = OK ? pExamData->LimbusY0 : 0.0;
	for (int i = 0; i < 360; i++) {
		pImage->m_li_r_um[i] = OK ? pExamData->LimbusR[i] : 0.0;
	}
	pImage->m_li_r_min_um = OK ? pExamData->LimbusRMin : 0.0;
	pImage->m_li_r_max_um = OK ? pExamData->LimbusRMax : 0.0;
	pImage->m_li_r_mean_um = OK ? pExamData->LimbusRMean : 0.0;
	// Pupil
	OK = pExamData->StructSize >= 5850 && pExamData->PupilOK == 1;
	pImage->m_pu_ok = OK;
	pImage->m_pu_x0_um = OK ? pExamData->PupilX0 : 0.0;
	pImage->m_pu_y0_um = OK ? pExamData->PupilY0 : 0.0;
	for (int i = 0; i < 360; i++) {
		pImage->m_pu_r_um[i] = OK ? pExamData->PupilR[i] : 0.0;
	}
	pImage->m_pu_r_min_um = OK ? pExamData->PupilRMin : 0.0;
	pImage->m_pu_r_max_um = OK ? pExamData->PupilRMax : 0.0;
	pImage->m_pu_r_mean_um = OK ? pExamData->PupilRMean : 0.0;
	// Vertex
	OK = pExamData->StructSize >= 80748 && pExamData->VertexOK == 1;
	pImage->m_ve_ok = OK;
	pImage->m_ve_x_um = OK ? pExamData->VertexX : 0.0;
	pImage->m_ve_y_um = OK ? pExamData->VertexY : 0.0;
	// Rings
	OK = pExamData->StructSize >= 80748 && pExamData->RingsOK == 1;
	pImage->m_ri_ok = OK;
	for (int r = 0; r < 26; r++) {
		for (int a = 0; a < 360; a++) {
			pImage->m_ri_r_um[r][a] = OK ? pExamData->RingsR[r][a] : INVALID_VALUE;
		}
	}
	// Save date & time
	OK = pExamData->StructSize >= 80772;
	pCTExam->m_Header.m_SaveYear = OK ? pExamData->SaveYear : pCTExam->m_Header.m_Year;
	pCTExam->m_Header.m_SaveMonth = OK ? pExamData->SaveMonth : pCTExam->m_Header.m_Month;
	pCTExam->m_Header.m_SaveDay = OK ? pExamData->SaveDay : pCTExam->m_Header.m_Day;
	pCTExam->m_Header.m_SaveHour = OK ? pExamData->SaveHour : pCTExam->m_Header.m_Hour;
	pCTExam->m_Header.m_SaveMinute = OK ? pExamData->SaveMinute : pCTExam->m_Header.m_Minute;
	pCTExam->m_Header.m_SaveSecond = OK ? pExamData->SaveSecond : pCTExam->m_Header.m_Second;
	// Fellow exam ID
	if (pExamData->StructSize >= 80788) {
		pCTExam->m_Header.m_FellowExamID = pExamData->FellowExamID;
	}
	else {
		pCTExam->m_Header.m_FellowExamID = GUID_NULL;
	}

	//520
	// Radial ruler
	OK = pExamData->StructSize >= 80813 && pExamData->RadialRulerOk == 1;
	pImage->m_Ra_ok = OK;
	pImage->m_Ra_x_um = OK ? pExamData->RadialRulerX0 : 0.0;
	pImage->m_Ra_y_um = OK ? pExamData->RadialRulerY0 : 0.0;
	pImage->m_Ra_r_um = OK ? pExamData->RadialRulerR : 0.0;

	//Inlay
	OK = pExamData->StructSize >= 80846 && pExamData->InlayOk == 1;
	pImage->m_In_ok = OK;
	pImage->m_In_x_um = OK ? pExamData->InlayX0 : 0.0;
	pImage->m_In_y_um = OK ? pExamData->InlayY0 : 0.0;
	pImage->m_In_r_um = OK ? pExamData->InlayR0 : 0.0;//530
	pImage->m_In_In_r_um = OK ? pExamData->InlayInR0 : 0.0;//530

														   //Linear Ruler
	OK = pExamData->StructSize >= 80879 && pExamData->LinearRulerOk == 1;
	pImage->m_LRa_ok = OK;
	pImage->m_LRa_x0_um = OK ? pExamData->LinearRulerX0 : 0.0;
	pImage->m_LRa_y0_um = OK ? pExamData->LinearRulerY0 : 0.0;
	pImage->m_LRa_x1_um = OK ? pExamData->LinearRulerX1 : 0.0;
	pImage->m_LRa_y1_um = OK ? pExamData->LinearRulerY1 : 0.0;
	//Done


	//520 Load the color image info
	if (numberImgs == 2 && pExamData->StructSize >= 83, 800)//83699
	{
		CEyeImage* pColorImage = &pCTExam->m_ColorImage;

		// Limbus
		BOOL OK = pExamData->StructSize >= 83800 && pExamData->ColorLimbusOK == 1;
		pColorImage->m_li_ok = OK;
		pColorImage->m_li_x0_um = OK ? pExamData->ColorLimbusX0 : 0.0;
		pColorImage->m_li_y0_um = OK ? pExamData->ColorLimbusY0 : 0.0;
		for (int i = 0; i < 360; i++) {
			pColorImage->m_li_r_um[i] = OK ? pExamData->ColorLimbusR[i] : 0.0;
		}
		pColorImage->m_li_r_min_um = OK ? pExamData->ColorLimbusRMin : 0.0;
		pColorImage->m_li_r_max_um = OK ? pExamData->ColorLimbusRMax : 0.0;
		pColorImage->m_li_r_mean_um = OK ? pExamData->ColorLimbusRMean : 0.0;

		// Pupil
		OK = pExamData->StructSize >= 86721 && pExamData->ColorPupilOK == 1;
		pColorImage->m_pu_ok = OK;
		pColorImage->m_pu_x0_um = OK ? pExamData->ColorPupilX0 : 0.0;
		pColorImage->m_pu_y0_um = OK ? pExamData->ColorPupilY0 : 0.0;
		for (int i = 0; i < 360; i++) {
			pColorImage->m_pu_r_um[i] = OK ? pExamData->ColorPupilR[i] : 0.0;
		}
		pColorImage->m_pu_r_min_um = OK ? pExamData->ColorPupilRMin : 0.0;
		pColorImage->m_pu_r_max_um = OK ? pExamData->ColorPupilRMax : 0.0;
		pColorImage->m_pu_r_mean_um = OK ? pExamData->ColorPupilRMean : 0.0;

		// Vertex
		OK = pExamData->StructSize >= 86738 && pExamData->ColorVertexOK == 1;
		pColorImage->m_ve_ok = OK;
		pColorImage->m_ve_x_um = OK ? pExamData->ColorVertexX : 0.0;
		pColorImage->m_ve_y_um = OK ? pExamData->ColorVertexY : 0.0;

		//Radial Ruler
		OK = pExamData->StructSize >= 86763 && pExamData->ColorRadialRulerOk == 1;
		pColorImage->m_Ra_ok = OK;
		pColorImage->m_Ra_x_um = OK ? pExamData->ColorRadialRulerX0 : 0.0;
		pColorImage->m_Ra_y_um = OK ? pExamData->ColorRadialRulerY0 : 0.0;
		pColorImage->m_Ra_r_um = OK ? pExamData->ColorRadialRulerR : 0.0;

		//Inlay
		OK = pExamData->StructSize >= 86788 && pExamData->ColorInlayOk == 1;
		pColorImage->m_In_ok = OK;
		pColorImage->m_In_x_um = OK ? pExamData->ColorInlayX0 : 0.0;
		pColorImage->m_In_y_um = OK ? pExamData->ColorInlayY0 : 0.0;
		pColorImage->m_In_r_um = OK ? pExamData->ColorInlayR0 : 0.0;//530
		pColorImage->m_In_In_r_um = OK ? pExamData->ColorInlayInR0 : 0.0;//530

																		 //Linear Ruler
		OK = pExamData->StructSize >= 86829 && pExamData->ColorLinearRulerOk == 1;
		pColorImage->m_LRa_ok = OK;
		pColorImage->m_LRa_x0_um = OK ? pExamData->ColorLinearRulerX0 : 0.0;
		pColorImage->m_LRa_y0_um = OK ? pExamData->ColorLinearRulerY0 : 0.0;
		pColorImage->m_LRa_x1_um = OK ? pExamData->ColorLinearRulerX1 : 0.0;
		pColorImage->m_LRa_y1_um = OK ? pExamData->ColorLinearRulerY1 : 0.0;
	}
	//Done

	p += pExamData->StructSize;

	// Image
	int JPGSize = *((int*)p);
	p += sizeof(int);
	pCTExam->m_Image.LoadFromMem(JPGSize, p);
	p += JPGSize;

	////[520]
	// Color image
	if (numberImgs == 2)
	{
		JPGSize = *((int*)p);
		if (JPGSize > 0)//which means the color image exist
		{
			p += sizeof(int);
			pCTExam->m_ColorImage.LoadFromMem(JPGSize, p);
			p += JPGSize;
		}
	}
	//[520]

	// Free memory
	free(pDecompressedData);

	// Calibration
	if (!LoadCalibration(pCTExam->m_Header.m_CalibrationID, &pCTExam->m_Calibration)) return FALSE;

	return TRUE;
}

//***************************************************************************************

BOOL CDB::SaveCTExam(CCTExam* pCTExam)
{
	if (m_hDLL == NULL) return FALSE;
	if (IsReadOnly()) return FALSE;

	// Calibration
	if (!SaveCalibration(pCTExam->m_Calibration.CalibrationID, &pCTExam->m_Calibration)) {
		::Error("Failed to save exam. Error code: 1.");
		return FALSE;
	}

	// Calculate exam size
	int Size = sizeof(TDB_EXAM_HEADER) + sizeof(TDB_CT_EXAM_DATA);

	if (pCTExam->m_Image.m_RGBData.GetMem() == NULL) {
		::Error("Failed to save exam. Error code: 2.");
		return FALSE;
	}

	//[520]
	//int JPGSize = 0;
	//uchar* pJPGData = NULL;
	//pCTExam->m_Image.SaveIntoMem(&JPGSize, &pJPGData, COMPRESSION_JPG);
	//Size += sizeof(int) + JPGSize;

	int ImgNums = 1;//[520]
	int JPGSize[2] = { 0, 0 };
	uchar* pJPGData[2] = { NULL, NULL };
	if (pCTExam->m_Image.m_RGBData.GetMem()) {
		pCTExam->m_Image.SaveIntoMem(&JPGSize[0], &pJPGData[0], COMPRESSION_JPG);
		Size += sizeof(int) + JPGSize[0];
	}
	if (pCTExam->m_ColorImage.m_RGBData.GetMem()) {
		pCTExam->m_ColorImage.SaveIntoMem(&JPGSize[1], &pJPGData[1], COMPRESSION_JPG);
		Size += sizeof(int) + JPGSize[1];
		ImgNums = 2;
	}
	//[520]

	// Allocate memory
	uchar* pData = (uchar*)malloc(Size);
	memset(pData, 0, Size);

	uchar* p = pData;

	// Fill out exam header
	TDB_EXAM_HEADER* pExamHeader = (TDB_EXAM_HEADER*)p;
  pExamHeader->ExamID          = pCTExam->m_Header.m_ExamID;
  pExamHeader->PatientID       = pCTExam->m_Header.m_PatientID;
  pExamHeader->CalibrationID   = pCTExam->m_Header.m_CalibrationID;
  pExamHeader->ClinicID        = pCTExam->m_Header.m_ClinicID;
  pExamHeader->PhysicianID     = pCTExam->m_Header.m_PhysicianID;
  pExamHeader->OperatorID      = pCTExam->m_Header.m_OperatorID;
  pExamHeader->Type            = pCTExam->m_Header.m_Type;
  pExamHeader->Mode            = pCTExam->m_Header.m_Mode;
  pExamHeader->Year            = pCTExam->m_Header.m_Year;
  pExamHeader->Month           = pCTExam->m_Header.m_Month;
  pExamHeader->Day             = pCTExam->m_Header.m_Day;
  pExamHeader->Hour            = pCTExam->m_Header.m_Hour;
  pExamHeader->Minute          = pCTExam->m_Header.m_Minute;
  pExamHeader->Second          = pCTExam->m_Header.m_Second;
  pExamHeader->Eye             = pCTExam->m_Header.m_Eye;
  pExamHeader->Preop           = pCTExam->m_Header.m_Preop;
  pExamHeader->SeriesNumber    = pCTExam->m_Header.m_SeriesNumber;
  pExamHeader->SoftwareVersion = pCTExam->m_Header.m_SoftwareVersion;
  pExamHeader->analogCamera    = AcquisitionParameters::AnalogCamera();

	G_As = pCTExam->m_Header.m_Note;
	strncpy(pExamHeader->Note, G_As, 100);

	p += sizeof(TDB_EXAM_HEADER);

	// Exam data header
	TDB_CT_EXAM_DATA* pExamData = (TDB_CT_EXAM_DATA*)p;
	pExamData->StructSize = sizeof(TDB_CT_EXAM_DATA);
	pExamData->NumImages = ImgNums;//[520]

								   // Limbus
	CEyeImage* pImage = &pCTExam->m_Image;
	BOOL OK = pImage->m_li_ok;
	pExamData->LimbusOK = OK ? 1 : 0;
	pExamData->LimbusX0 = OK ? pImage->m_li_x0_um : 0.0;
	pExamData->LimbusY0 = OK ? pImage->m_li_y0_um : 0.0;
	for (int i = 0; i < 360; i++) {
		pExamData->LimbusR[i] = OK ? pImage->m_li_r_um[i] : 0.0;
	}
	pExamData->LimbusRMin = OK ? pImage->m_li_r_min_um : 0.0;
	pExamData->LimbusRMax = OK ? pImage->m_li_r_max_um : 0.0;
	pExamData->LimbusRMean = OK ? pImage->m_li_r_mean_um : 0.0;
	// Pupil
	OK = pImage->m_pu_ok;
	pExamData->PupilOK = OK ? 1 : 0;
	pExamData->PupilX0 = OK ? pImage->m_pu_x0_um : 0.0;
	pExamData->PupilY0 = OK ? pImage->m_pu_y0_um : 0.0;
	for (int i = 0; i < 360; i++) {
		pExamData->PupilR[i] = OK ? pImage->m_pu_r_um[i] : 0.0;
	}
	pExamData->PupilRMin = OK ? pImage->m_pu_r_min_um : 0.0;
	pExamData->PupilRMax = OK ? pImage->m_pu_r_max_um : 0.0;
	pExamData->PupilRMean = OK ? pImage->m_pu_r_mean_um : 0.0;
	// Vertex
	OK = pImage->m_ve_ok;
	pExamData->VertexOK = OK ? 1 : 0;
	pExamData->VertexX = OK ? pImage->m_ve_x_um : 0.0;
	pExamData->VertexY = OK ? pImage->m_ve_y_um : 0.0;
	// Rings
	OK = pImage->m_ri_ok;
	pExamData->RingsOK = OK ? 1 : 0;
	for (int r = 0; r < 26; r++) {
		for (int a = 0; a < 360; a++) {
			pExamData->RingsR[r][a] = OK ? pImage->m_ri_r_um[r][a] : INVALID_VALUE;
		}
	}
	// Save date & time
	SYSTEMTIME Time;
	::GetLocalTime(&Time);
	pExamData->SaveYear = Time.wYear;
	pExamData->SaveMonth = Time.wMonth;
	pExamData->SaveDay = Time.wDay;
	pExamData->SaveHour = Time.wHour;
	pExamData->SaveMinute = Time.wMinute;
	pExamData->SaveSecond = Time.wSecond;
	// Fellow exam ID
	pExamData->FellowExamID = pCTExam->m_Header.m_FellowExamID;

	// Radial Ruler
	OK = pImage->m_Ra_ok;
	pExamData->RadialRulerOk = OK ? 1 : 0;
	pExamData->RadialRulerX0 = OK ? pImage->m_Ra_x_um : 0.0;
	pExamData->RadialRulerY0 = OK ? pImage->m_Ra_y_um : 0.0;
	pExamData->RadialRulerR = OK ? pImage->m_Ra_r_um : 0.0;

	//Inlay
	OK = pImage->m_In_ok;
	pExamData->InlayOk = OK ? 1 : 0;
	pExamData->InlayX0 = OK ? pImage->m_In_x_um : 0.0;
	pExamData->InlayY0 = OK ? pImage->m_In_y_um : 0.0;
	pExamData->InlayR0 = OK ? pImage->m_In_r_um : 0.0;
	pExamData->InlayInR0 = OK ? pImage->m_In_In_r_um : 0.0;//530

														   //Linear Ruler
	OK = pImage->m_LRa_ok;
	pExamData->LinearRulerOk = OK ? 1 : 0;
	pExamData->LinearRulerX0 = OK ? pImage->m_LRa_x0_um : 0.0;
	pExamData->LinearRulerY0 = OK ? pImage->m_LRa_y0_um : 0.0;
	pExamData->LinearRulerX1 = OK ? pImage->m_LRa_x1_um : 0.0;
	pExamData->LinearRulerY1 = OK ? pImage->m_LRa_y1_um : 0.0;

	//520 save the color image info
	if (ImgNums == 2)
	{
		CEyeImage* pColorImage = &pCTExam->m_ColorImage;

		//Limbus
		BOOL OK = pColorImage->m_li_ok;
		pExamData->ColorLimbusOK = OK ? 1 : 0;
		pExamData->ColorLimbusX0 = OK ? pColorImage->m_li_x0_um : 0.0;
		pExamData->ColorLimbusY0 = OK ? pColorImage->m_li_y0_um : 0.0;
		for (int i = 0; i < 360; i++) {
			pExamData->ColorLimbusR[i] = OK ? pColorImage->m_li_r_um[i] : 0.0;
		}
		pExamData->ColorLimbusRMin = OK ? pColorImage->m_li_r_min_um : 0.0;
		pExamData->ColorLimbusRMax = OK ? pColorImage->m_li_r_max_um : 0.0;
		pExamData->ColorLimbusRMean = OK ? pColorImage->m_li_r_mean_um : 0.0;

		// Pupil
		OK = pColorImage->m_pu_ok;
		pExamData->ColorPupilOK = OK ? 1 : 0;
		pExamData->ColorPupilX0 = OK ? pColorImage->m_pu_x0_um : 0.0;
		pExamData->ColorPupilY0 = OK ? pColorImage->m_pu_y0_um : 0.0;
		for (int i = 0; i < 360; i++) {
			pExamData->ColorPupilR[i] = OK ? pColorImage->m_pu_r_um[i] : 0.0;
		}
		pExamData->ColorPupilRMin = OK ? pColorImage->m_pu_r_min_um : 0.0;
		pExamData->ColorPupilRMax = OK ? pColorImage->m_pu_r_max_um : 0.0;
		pExamData->ColorPupilRMean = OK ? pColorImage->m_pu_r_mean_um : 0.0;

		// Vertex
		OK = pColorImage->m_ve_ok;
		pExamData->ColorVertexOK = OK ? 1 : 0;
		pExamData->ColorVertexX = OK ? pColorImage->m_ve_x_um : 0.0;
		pExamData->ColorVertexY = OK ? pColorImage->m_ve_y_um : 0.0;

		//Radial Ruler
		OK = pColorImage->m_Ra_ok;
		pExamData->ColorRadialRulerOk = OK ? 1 : 0;
		pExamData->ColorRadialRulerX0 = OK ? pColorImage->m_Ra_x_um : 0.0;
		pExamData->ColorRadialRulerY0 = OK ? pColorImage->m_Ra_y_um : 0.0;
		pExamData->ColorRadialRulerR = OK ? pColorImage->m_Ra_r_um : 0.0;

		//Inlay
		OK = pColorImage->m_In_ok;
		pExamData->ColorInlayOk = OK ? 1 : 0;
		pExamData->ColorInlayX0 = OK ? pColorImage->m_In_x_um : 0.0;
		pExamData->ColorInlayY0 = OK ? pColorImage->m_In_y_um : 0.0;
		pExamData->ColorInlayR0 = OK ? pColorImage->m_In_r_um : 0.0;
		pExamData->ColorInlayInR0 = OK ? pColorImage->m_In_In_r_um : 0.0;//530

																		 //Linear Ruler
		OK = pColorImage->m_LRa_ok;
		pExamData->ColorLinearRulerOk = OK ? 1 : 0;
		pExamData->ColorLinearRulerX0 = OK ? pColorImage->m_LRa_x0_um : 0.0;
		pExamData->ColorLinearRulerY0 = OK ? pColorImage->m_LRa_y0_um : 0.0;
		pExamData->ColorLinearRulerX1 = OK ? pColorImage->m_LRa_x1_um : 0.0;
		pExamData->ColorLinearRulerY1 = OK ? pColorImage->m_LRa_y1_um : 0.0;
	}
	//Done

	p += sizeof(TDB_CT_EXAM_DATA);

	// Image
	//[520]
	//*((int*)p) = JPGSize;
	//p += sizeof(int);
	//memcpy(p, pJPGData, JPGSize);
	//free(pJPGData);

	if (pJPGData[0]) {
		*((int*)p) = JPGSize[0];
		p += sizeof(int);
		memcpy(p, pJPGData[0], JPGSize[0]);
		p += JPGSize[0];
		free(pJPGData[0]);
	}
	// Color image
	if (pJPGData[1]) {
		*((int*)p) = JPGSize[1];
		p += sizeof(int);
		memcpy(p, pJPGData[1], JPGSize[1]);
		p += JPGSize[1];
		free(pJPGData[1]);
	}
	//[520]

	// Compress
	void* pCompressedData = malloc(Size);
	int CompressedSize;
	::Compress(pData, Size, pCompressedData, &CompressedSize);

	// Save exam
	if (DllSaveExamHeader(pExamHeader))//Big Database Research since 03062019
	//if (SaveTxTExamHeaders(&pCTExam->m_Header))//Big Database Research since 03062019
	{
		if (DllSaveExam(pCTExam->m_Header.m_ExamID, pCompressedData, CompressedSize)) {
			pCTExam->m_Header.m_Saved = TRUE;
		}
		else {
			::Error("Failed to save exam. Error code: 4.");
		}
	}
	else {
		::Error("Failed to save exam. Error code: 3.");
	}

	//GenerateExamAmount();//Big Database Research since 03062019

	// Free memory
	free(pCompressedData);
	free(pData);

	return pCTExam->m_Header.m_Saved;
}

//***************************************************************************************

void CDB::DeleteExam(const GUID& ExamID)
{
	if (m_hDLL == NULL) return;

	DllDeleteExam(ExamID);

//	GenerateExamAmount();//Big Database Research since 03062019
}

//***************************************************************************************

BOOL CDB::Import(const CString& FileName)
{
	if (m_hDLL == NULL) return FALSE;

	HZIP hZip = ::OpenZip(FileName, "");
	if (hZip == 0) return FALSE;

	CString Extensions[9] = { "group", "patient", "eye", "cal", "clinic", "physician", "operator", "exam", "ave" };//601 06092015 for averaged exam add "ave" here

	ZIPENTRY ZipEntry;
	::GetZipItem(hZip, -1, &ZipEntry);
	int NumEntries = ZipEntry.index;

	for (int k = 0; k < 9; k++)
	{
		for (int i = 0; i < NumEntries; i++)
		{
			::GetZipItem(hZip, i, &ZipEntry);
			CString FileName(ZipEntry.name);
			CStringA AFileName(FileName.Left(FileName.GetLength() - 1 - Extensions[k].GetLength()));

			int FileSize = ZipEntry.unc_size;
			if (Extensions[k] == FileName.Right(Extensions[k].GetLength()))
			{
				void* pData = malloc(FileSize);
				::UnzipItem(hZip, i, pData, FileSize);

				GUID ID;
				::StrToGUID(AFileName, ID);

				if (k == 0)
				{
					DllSaveGroup((TDB_GROUP*)pData);
				}
				else if (k == 1)
				{
					DllSavePatient((TDB_PATIENT*)pData);
				}
				else if (k == 2)
				{
					DllSaveEye((TDB_EYE*)pData);
				}
				else if (k == 3)
				{
					DllSaveCalibration(ID, pData, FileSize);
				}
				else if (k == 4)
				{
					DllSaveClinic((TDB_CLINIC*)pData);
				}
				else if (k == 5)
				{
					DllSavePhysician((TDB_PHYSICIAN*)pData);
				}
				else if (k == 6)
				{
					DllSaveOperator((TDB_OPERATOR*)pData);
				}
				else if (k == 7)
				{
					int DecompressedSize = ::GetDecompressedSize(pData, ZipEntry.unc_size);
					void* pDecompressedData = malloc(DecompressedSize);
					if (::Decompress(pData, FileSize, pDecompressedData))
					{
						////Big Database Research since 03062019
						//TDB_EXAM_HEADER* pExamHeader = (TDB_EXAM_HEADER*)pDecompressedData;
						//ExamHeader_STR ThisExamStr;
						//CString IDStr;

						//GUIDToStr(pExamHeader->ExamID, IDStr);
						//ThisExamStr.ExamID_Str = IDStr;

						//GUIDToStr(pExamHeader->CalibrationID, IDStr);
						//ThisExamStr.CalibrationID_Str = IDStr;

						//GUIDToStr(pExamHeader->ClinicID, IDStr);
						//ThisExamStr.ClinicIDStr = IDStr;

						//GUIDToStr(pExamHeader->PhysicianID, IDStr);
						//ThisExamStr.PhysicianID_Str = IDStr;

						//GUIDToStr(pExamHeader->OperatorID, IDStr);
						//ThisExamStr.OperatorID_Str = IDStr;

						//ThisExamStr.Type_Str.Format("%i", pExamHeader->Type);
						//ThisExamStr.Mode_Str.Format("%i", pExamHeader->Mode);
						//ThisExamStr.Year_Str.Format("%i", pExamHeader->Year);
						//ThisExamStr.Month_Str.Format("%i", pExamHeader->Month);
						//ThisExamStr.Day_Str.Format("%i", pExamHeader->Day);
						//ThisExamStr.Hour_Str.Format("%i", pExamHeader->Hour);
						//ThisExamStr.Minute_Str.Format("%i", pExamHeader->Minute);
						//ThisExamStr.Second_Str.Format("%i", pExamHeader->Second);
						//ThisExamStr.Eye_Str.Format("%i", pExamHeader->Eye);
						//ThisExamStr.Preop_Str.Format("%i", pExamHeader->Preop);
						//ThisExamStr.SeriesNumber_Str.Format("%i", pExamHeader->SeriesNumber);

						//ThisExamStr.Note_Str = pExamHeader->Note;
						//ThisExamStr.SoftwareVersion_Str.Format("%i", pExamHeader->SoftwareVersion);

						//SaveTxTExamHeader(pExamHeader->PatientID, ThisExamStr);
						DllSaveExamHeader((TDB_EXAM_HEADER*)pDecompressedData);
						//Big Database Research since 03062019

						DllSaveExam(ID, pData, FileSize);
					}
					free(pDecompressedData);
				}
				//601 06092015 for loading averaged exam
				else if (k == 8)
				{
					CAveExamInfo* AveExamInfo = (CAveExamInfo*)pData;
					CString ExamId, SaveFileName;
					GUIDToStr(ID, ExamId);
					SaveFileName = MAIN_DIR + "\\Data\\" + ExamId;
					AveExamInfo->SaveIntoFile(SaveFileName);
				}
				//601 06092015 for averaged exam Done

				free(pData);
			}
		}
	}

	::CloseZipU(hZip);

	//GenerateExamAmount();//Big Database Research since 03062019

	return TRUE;
}

//***************************************************************************************

BOOL CDB::ExportPatient(const GUID& PatientID, const CString& FolderName)
{
	int Type;
	GetType(Type);

	if (Type == DATABASE_TYPE_MSJET_OLD) {
		::Error("Can't export old style database data.");
		return FALSE;
	}

	if (m_hDLL == NULL) return FALSE;

	BOOL Res = FALSE;

	uint64 BytesAvail, BytesTotal, BytesFree;
	::GetDiskFreeSpaceEx(FolderName, (PULARGE_INTEGER)&BytesAvail, (PULARGE_INTEGER)&BytesTotal, (PULARGE_INTEGER)&BytesFree);

	TDB_PATIENT* pPatients = NULL;
	int NumPatients = DllGetPatientsCount();
	if (NumPatients > 0) {
		pPatients = new TDB_PATIENT[NumPatients];
		DllLoadPatients(pPatients);
	}

	TDB_PATIENT* pPatient = NULL;
	for (int i = 0; i < NumPatients; i++) {
		if (pPatients[i].PatientID == PatientID) {
			pPatient = &pPatients[i];
			break;
		}
	}

	if (pPatient == NULL) {
		if (pPatients) delete[] pPatients;
		return FALSE;
	}

	TDB_GROUP* pGroups = NULL;
	int NumGroups = DllGetGroupsCount();
	if (NumGroups > 0) {
		pGroups = new TDB_GROUP[NumGroups];
		DllLoadGroups(pGroups);
	}

	TDB_CLINIC* pClinics = NULL;
	int NumClinics = DllGetClinicsCount();
	if (NumClinics > 0) {
		pClinics = new TDB_CLINIC[NumClinics];
		DllLoadClinics(pClinics);
	}

	TDB_PHYSICIAN* pPhysicians = NULL;
	int NumPhysicians = DllGetPhysiciansCount();
	if (NumPhysicians > 0) {
		pPhysicians = new TDB_PHYSICIAN[NumPhysicians];
		DllLoadPhysicians(pPhysicians);
	}

	TDB_OPERATOR* pOperators = NULL;
	int NumOperators = DllGetOperatorsCount();
	if (NumOperators > 0) {
		pOperators = new TDB_OPERATOR[NumOperators];
		DllLoadOperators(pOperators);
	}

	TDB_EYE* pEyes = NULL;
	int NumEyes = DllGetEyesCount(PatientID);
	if (NumEyes > 0) {
		pEyes = new TDB_EYE[NumEyes];
		DllLoadEyes(PatientID, pEyes);
	}

	TDB_EXAM_HEADER* pExamHeaders = NULL;
	int NumExams = DllGetExamsCount(PatientID);
	if (NumExams > 0) {
		pExamHeaders = new TDB_EXAM_HEADER[NumExams];
		DllLoadExamHeaders(PatientID, pExamHeaders);
	}

	::CreateDirectory(FolderName, NULL);

	List<GUID> ClinicIDs;
	List<GUID> PhysicianIDs;
	List<GUID> OperatorIDs;
	List<GUID> CalIDs;

	CString s;
	::GUIDToStr(PatientID, s);

	CString FileName(G_ReadChinese(pPatient->FirstName));
	FileName += " ";
	FileName += G_ReadChinese(pPatient->LastName);
	FileName += " ";
	FileName += s;
	FileName += ".zip";

	FileName.Replace(_T("\\"), _T("_"));
	FileName.Replace(_T("/"), _T("_"));
	FileName.Replace(_T(":"), _T("_"));
	FileName.Replace(_T("*"), _T("_"));
	FileName.Replace(_T("?"), _T("_"));
	FileName.Replace(_T("\""), _T("_"));
	FileName.Replace(_T("<"), _T("_"));
	FileName.Replace(_T(">"), _T("_"));
	FileName.Replace(_T("|"), _T("_"));
	FileName.Replace(_T(","), _T("_"));

	FileName = FolderName + "\\" + FileName;

	HZIP hZip = ::CreateZip(FileName, "");
	TDB_GROUP* pGroup = NULL;

	// patient
	if ((BytesAvail -= sizeof(TDB_PATIENT)) < 0) {
		::Error("Not enough space.");
		goto END;
	}
	FileName = s + ".patient";
	::ZipAdd(hZip, FileName, pPatient, sizeof(TDB_PATIENT));

	// group
	for (int i = 0; i < NumGroups; i++) {
		if (pGroups[i].GroupID == pPatient->GroupID) {
			pGroup = &pGroups[i];
			break;
		}
	}
	if (pGroup) {
		::GUIDToStr(pGroup->GroupID, s);
		FileName = s + ".group";
		if ((BytesAvail -= sizeof(TDB_GROUP)) < 0) {
			::Error("Not enough space.");
			goto END;
		}
		::ZipAdd(hZip, FileName, pGroup, sizeof(TDB_GROUP));
	}

	// eyes
	for (int i = 0; i < NumEyes; i++) {
		::GUIDToStr(pEyes[i].EyeID, s);
		FileName = s + ".eye";
		if ((BytesAvail -= sizeof(TDB_EYE)) < 0) {
			::Error("Not enough space.");
			goto END;
		}
		::ZipAdd(hZip, FileName, pEyes + i, sizeof(TDB_EYE));
	}

	for (int i = 0; i < NumExams; i++) {

		// clinic
		if (!ClinicIDs.Find(pExamHeaders[i].ClinicID)) {
			for (int j = 0; j < NumClinics; j++) {
				if (pClinics[j].ClinicID == pExamHeaders[i].ClinicID) {
					ClinicIDs.Append(&pExamHeaders[i].ClinicID);
					::GUIDToStr(pExamHeaders[i].ClinicID, FileName);
					FileName += ".clinic";
					if ((BytesAvail -= sizeof(TDB_CLINIC)) < 0) {
						::Error("Not enough space.");
						goto END;
					}
					::ZipAdd(hZip, FileName, pClinics + j, sizeof(TDB_CLINIC));
					break;
				}
			}
		}

		// physician
		if (!PhysicianIDs.Find(pExamHeaders[i].PhysicianID)) {
			for (int j = 0; j < NumPhysicians; j++) {
				if (pPhysicians[j].PhysicianID == pExamHeaders[i].PhysicianID) {
					PhysicianIDs.Append(&pExamHeaders[i].PhysicianID);
					::GUIDToStr(pExamHeaders[i].PhysicianID, FileName);
					FileName += ".physician";
					if ((BytesAvail -= sizeof(TDB_PHYSICIAN)) < 0) {
						::Error("Not enough space.");
						goto END;
					}
					::ZipAdd(hZip, FileName, pPhysicians + j, sizeof(TDB_PHYSICIAN));
					break;
				}
			}
		}

		// operator
		if (!OperatorIDs.Find(pExamHeaders[i].OperatorID)) {
			for (int j = 0; j < NumOperators; j++) {
				if (pOperators[j].OperatorID == pExamHeaders[i].OperatorID) {
					OperatorIDs.Append(&pExamHeaders[i].OperatorID);
					::GUIDToStr(pExamHeaders[i].OperatorID, FileName);
					FileName += ".operator";
					if ((BytesAvail -= sizeof(TDB_OPERATOR)) < 0) {
						::Error("Not enough space.");
						goto END;
					}
					::ZipAdd(hZip, FileName, pOperators + j, sizeof(TDB_OPERATOR));
					break;
				}
			}
		}

		// calibration
		if (!CalIDs.Find(pExamHeaders[i].CalibrationID)) {
			//if (m_Type == DATABASE_TYPE_MSJET_OLD) {
			//  int DecompressedSize = DllGetCalibrationSize(pExamHeaders[i].CalibrationID);
			//  if (DecompressedSize > 0) {
			//    uchar* pDecompressedData = (uchar*)malloc(DecompressedSize);
			//    if (DllLoadCalibration(pExamHeaders[i].CalibrationID, pDecompressedData, DecompressedSize)) {
			//      uchar* pCompressedData = (uchar*)malloc(DecompressedSize);
			//      int CompressedSize;
			//      ::Compress(pDecompressedData, DecompressedSize, pCompressedData, &CompressedSize);
			//      ::GUIDToStr(pExamHeaders[i].CalibrationID, FileName);
			//      FileName += ".cal";
			//      ::ZipAdd(hZip, FileName, pCompressedData, CompressedSize);
			//      free(pCompressedData);
			//    }
			//    free(pDecompressedData);
			//  }
			//}
			//else {
			int CompressedSize = DllGetCalibrationSize(pExamHeaders[i].CalibrationID);
			void* pCompressedData = malloc(CompressedSize);
			if (DllLoadCalibration(pExamHeaders[i].CalibrationID, pCompressedData, CompressedSize)) {
				CalIDs.Append(&pExamHeaders[i].CalibrationID);
				::GUIDToStr(pExamHeaders[i].CalibrationID, FileName);
				FileName += ".cal";
				if ((BytesAvail -= CompressedSize) < 0) {
					::Error("Not enough space.");
					goto END;
				}
				::ZipAdd(hZip, FileName, pCompressedData, CompressedSize);
			}
			free(pCompressedData);
			//}
		}

		// exam
		{
			//if (m_Type == DATABASE_TYPE_MSJET_OLD) {
			//  int DecompressedSize = DllGetExamSize(pExamHeaders[i].ExamID);
			//  if (DecompressedSize > 0) {
			//    uchar* pDecompressedData = (uchar*)malloc(DecompressedSize);
			//    if (DllLoadExam(pExamHeaders[i].ExamID, pDecompressedData, DecompressedSize)) {
			//      uchar* pCompressedData = (uchar*)malloc(DecompressedSize);
			//      int CompressedSize;
			//      ::Compress(pDecompressedData, DecompressedSize, pCompressedData, &CompressedSize);
			//      ::GUIDToStr(pExamHeaders[i].ExamID, FileName);
			//      FileName += ".exam";
			//      ::ZipAdd(hZip, FileName, pCompressedData, CompressedSize);
			//      free(pCompressedData);
			//    }
			//    free(pDecompressedData);
			//  }
			//}
			//else {
			int CompressedSize = DllGetExamSize(pExamHeaders[i].ExamID);
			void* pCompressedData = malloc(CompressedSize);
			if (DllLoadExam(pExamHeaders[i].ExamID, pCompressedData, CompressedSize)) {
				::GUIDToStr(pExamHeaders[i].ExamID, FileName);
				FileName += ".exam";
				if ((BytesAvail -= CompressedSize) < 0) {
					::Error("Not enough space.");
					goto END;
				}
				::ZipAdd(hZip, FileName, pCompressedData, CompressedSize);
			}
			free(pCompressedData);
			//}

			//601 06092015 for saving average exam m_c
			if (pExamHeaders[i].Type == EXAM_TYPE_WF)
			{
				CString ExamId, LoadFileName;
				::GUIDToStr(pExamHeaders[i].ExamID, FileName);
				LoadFileName = MAIN_DIR + "\\Data\\" + FileName;

				if (::PathFileExists(LoadFileName))
				{
					CAveExamInfo* AveExamInfo = new CAveExamInfo;
					AveExamInfo->LoadFromFile(LoadFileName);

					void* AvepCompressedData = malloc(sizeof(CAveExamInfo));
					AvepCompressedData = (CAveExamInfo*)AveExamInfo;

					FileName += ".ave";

					if ((BytesAvail -= sizeof(CAveExamInfo)) < 0)
					{
						::Error("Not enough space.");
						goto END;
					}
					::ZipAdd(hZip, FileName, AvepCompressedData, sizeof(CAveExamInfo));
				}
			}
			//601 06092015 for saving average exam m_c
		} // exam

	} // loop

	Res = TRUE;

END:

	::CloseZipZ(hZip);

	if (pGroups) delete[] pGroups;
	if (pClinics) delete[] pClinics;
	if (pPhysicians) delete[] pPhysicians;
	if (pOperators) delete[] pOperators;
	if (pPatients) delete[] pPatients;
	if (pEyes) delete[] pEyes;
	if (pExamHeaders) delete[] pExamHeaders;

	return Res;
}

//***************************************************************************************

