1. New Dicom source import(did something)  //Done

2. OnNomogramCheckClicked(Toric planner) //Done

3. Aod's tools-- can not adjust laser power (From Chris) 

4. wf cal-- takes both shots with everything disabled(From Chris)

5. New Dicom import for MWL(did something)// Need test

6. refraction zone(in settings)  does not show real number in the excel sheet if it set to
(From customer Adrian, check Email) //Done

7. Lens opacity not printing ( From customer Alan Wee, check Email ) //Done

8. Read - only can not connect to database? ( no sure about this bug )

9. Change the registry functionalilty to ini file,//Done

10. Sergy said one bug and he fixed it, check the email

11. The printer dispear issue check email 'FW : Printer window disappearing SN / 3652'//Done

12. Add Chinese//Done

13. After add chinese the win style changed

14. Change all 6.2.0 to 6.2.0 (do not forget setting files too)

















5-8-2004  

1.working on task button, till //m_SettedTask.m_SubButton[t] = 0; 

2.Victor and Chris testing 530 Presbia
(one problem: Since Presbia is based on COMBO, the unfinished 530 include show toric and chang for presbis too)

 They found the software crashed randomly during click new exam, do not solve this yet.


5-9-2004 Fri

1. Change the control wnd pointers of acquiitiondlg, waitting for crash test.
Crash problem has been solved.

2. Andrea sent a mail talking about the iol selection which the docotr want to define lense too

5-12-2014 Mon

1. Andrea ask for remmbering the presbia zone as a functionality
Done

2. Remove Toric/Chang... and so on from Presbia and Combo version
Done

3. Disable task advanced setting for Presbia version right now

4.

5. Andrea ask turn to PVS for adding key/(gui) for one degree rotation.


//void CDB::Connect(const BOOL ReadOnly)
//{
//	int Type;
//	CString FolderName;
//	CString FileName;
//	GetType(Type);
//
//	//Type = 0;
//	GetFolderName(FolderName);
//	GetFileName(FileName);
//	
//	BOOL FirstRun = FALSE;//
//	BOOL NeedChangeLinker = FALSE;//
//	BOOL NeedChangeServerLinker = FALSE;//
//	CString LocalServerFolderName = "";// If the database is from server, here save the front-end database
//
//	 //	
//	if (Type == DATABASE_TYPE_MSJET)
//	{
//		//
//		//LinkerCheck(FolderName);
//		// 
//
//		int pos;
//		BOOL ServerDataBase = FindSubString("\\TraceyLinkerServer\\", FolderName, pos);
//		BOOL MdbDataExist = TRUE;
//
//		if (ServerDataBase)
//		{
//			MdbDataExist = ::PathFileExists(FolderName + "\\tdb.mdb");	
//		}
//
//		if (!(ServerDataBase && MdbDataExist))
//			LocalServerFolderName = SplitDatabaseProcess(FolderName, NeedChangeLinker, NeedChangeServerLinker, FirstRun);
//	}
//	//
//
//	//530
//	if (::Licensing.IsDemo())
//	{
//		Type = 0;
//		FolderName = MAIN_DIR + "\\Data\\MSJetEx";
//		FileName = "";
//
//		CRegKey1 Key0(HKEY_CURRENT_USER, MAIN_REG_KEY);
//		CRegKey1 Key(HKEY_LOCAL_MACHINE, MAIN_REG_KEY);//
//
//		int Type, Type1;
//		Key0.GetIntValue("OMED", Type);
//		if (Type != 1 && Type != 2)
//		{
//			Key.GetIntValue("OMED", Type1);
//			Type = Type1;
//		}
//
//		if (!::PathFileExists(FolderName))
//		{
//			if (Type == 1 || Type == 2)
//			{
//				Info("Database has been deleted. Please contact Tracey technology.");
//				return;
//			}
//
//			::CreateDirectory(FolderName, NULL);
//		}
//
//		CString DemoFileName = MAIN_DIR + "\\Data\\MSJetEx\\exc.zip";
//		CString DbFileName = MAIN_DIR + "\\Data\\MSJetEx\\tdb.mdb";
//		if (!::PathFileExists(DemoFileName) && ::PathFileExists(DbFileName))
//		{
//			//delete all files in the folder
//			DeleteDirectory((LPSTR)(LPCTSTR)FolderName);
//		}
//
//		//Create the demo correction file
//		if (!::PathFileExists(DemoFileName))
//		{
//			if (Type == 1 || Type == 2)
//			{
//				Info("Database has been deleted. please contact Tracey technology.");
//				return;
//			}
//
//			CDEMOID DemoData;
//			for (int i = 0; i < 30; i++)
//			{
//				DemoData.PatientID[i] = GUID_NULL;
//			}
//			DemoData.SaveIntoDemoFile(DemoFileName);
//
//			Key.SetIntValue("OMED", 2);
//			Key0.SetIntValue("OMED", 2);
//		}
//	}
//	//530
//
//	m_hDLL = LoadLibrary(Type);
//
//	if (m_hDLL == NULL) goto _ERROR_;
//
//	if (Type == DATABASE_TYPE_MSJET || Type == DATABASE_TYPE_MSJET_OLD)
//	{
//		DllConnect = (TDllConnect)::GetProcAddress(m_hDLL, "Connect");
//		if (DllConnect == NULL) goto _ERROR_;
//		DllDisconnect = (TDllDisconnect)::GetProcAddress(m_hDLL, "Disconnect");
//		if (DllDisconnect == NULL) goto _ERROR_;
//		DllIsReadOnly = (TDllIsReadOnly)::GetProcAddress(m_hDLL, "IsReadOnly");
//		if (DllIsReadOnly == NULL) goto _ERROR_;
//	}
//	else {
//		DllConnect = NULL;
//		DllDisconnect = NULL;
//		DllIsReadOnly = NULL;
//	}
//
//	//
//	if (Type == DATABASE_TYPE_MSJET)
//	{
//		DllChangeLiner = (TDllChangeLinker)::GetProcAddress(m_hDLL, "ChangeLinker");
//		if (DllChangeLiner == NULL) goto _ERROR_;
//	}
//	//
//
//	DllGetGroupsCount = (TDllGetGroupsCount)::GetProcAddress(m_hDLL, "GetGroupsCount");
//	if (DllGetGroupsCount == NULL) goto _ERROR_;
//
//	DllLoadGroups = (TDllLoadGroups)::GetProcAddress(m_hDLL, "LoadGroups");
//	if (DllLoadGroups == NULL) goto _ERROR_;
//
//	DllSaveGroup = (TDllSaveGroup)::GetProcAddress(m_hDLL, "SaveGroup");
//	if (DllSaveGroup == NULL) goto _ERROR_;
//
//	DllGetClinicsCount = (TDllGetClinicsCount)::GetProcAddress(m_hDLL, "GetClinicsCount");
//	if (DllGetClinicsCount == NULL) goto _ERROR_;
//
//	DllLoadClinics = (TDllLoadClinics)::GetProcAddress(m_hDLL, "LoadClinics");
//	if (DllLoadClinics == NULL) goto _ERROR_;
//
//	DllSaveClinic = (TDllSaveClinic)::GetProcAddress(m_hDLL, "SaveClinic");
//	if (DllSaveClinic == NULL) goto _ERROR_;
//
//	//[cjf***05082012]
//	if (Type == DATABASE_TYPE_MSJET)
//	{
//		DllDeleteClinic = (TDllDeleteClinic)::GetProcAddress(m_hDLL, "DeleteClinic");
//		if (DllDeleteClinic == NULL) goto _ERROR_;
//	}
//	//[cjf***05082012]
//
//	DllGetPhysiciansCount = (TDllGetPhysiciansCount)::GetProcAddress(m_hDLL, "GetPhysiciansCount");
//	if (DllGetPhysiciansCount == NULL) goto _ERROR_;
//
//	DllLoadPhysicians = (TDllLoadPhysicians)::GetProcAddress(m_hDLL, "LoadPhysicians");
//	if (DllLoadPhysicians == NULL) goto _ERROR_;
//
//	DllSavePhysician = (TDllSavePhysician)::GetProcAddress(m_hDLL, "SavePhysician");
//	if (DllSavePhysician == NULL) goto _ERROR_;
//
//	DllGetOperatorsCount = (TDllGetOperatorsCount)::GetProcAddress(m_hDLL, "GetOperatorsCount");
//	if (DllGetOperatorsCount == NULL) goto _ERROR_;
//
//	DllLoadOperators = (TDllLoadOperators)::GetProcAddress(m_hDLL, "LoadOperators");
//	if (DllLoadOperators == NULL) goto _ERROR_;
//
//	DllSaveOperator = (TDllSaveOperator)::GetProcAddress(m_hDLL, "SaveOperator");
//	if (DllSaveOperator == NULL) goto _ERROR_;
//
//	DllGetPatientsCount = (TDllGetPatientsCount)::GetProcAddress(m_hDLL, "GetPatientsCount");
//	if (DllGetPatientsCount == NULL) goto _ERROR_;
//
//	DllLoadPatients = (TDllLoadPatients)::GetProcAddress(m_hDLL, "LoadPatients");
//	if (DllLoadPatients == NULL) goto _ERROR_;
//
//	DllSavePatient = (TDllSavePatient)::GetProcAddress(m_hDLL, "SavePatient");
//	if (DllSavePatient == NULL) goto _ERROR_;
//
//	DllDeletePatient = (TDllDeletePatient)::GetProcAddress(m_hDLL, "DeletePatient");
//	if (DllDeletePatient == NULL) goto _ERROR_;
//
//	DllGetEyesCount = (TDllGetEyesCount)::GetProcAddress(m_hDLL, "GetEyesCount");
//	if (DllGetEyesCount == NULL) goto _ERROR_;
//
//	DllLoadEyes = (TDllLoadEyes)::GetProcAddress(m_hDLL, "LoadEyes");
//	if (DllLoadEyes == NULL) goto _ERROR_;
//
//	DllSaveEye = (TDllSaveEye)::GetProcAddress(m_hDLL, "SaveEye");
//	if (DllSaveEye == NULL) goto _ERROR_;
//
//	DllGetExamsCount = (TDllGetExamsCount)::GetProcAddress(m_hDLL, "GetExamsCount");
//	if (DllGetExamsCount == NULL) goto _ERROR_;
//
//	DllLoadExamHeaders = (TDllLoadExamHeaders)::GetProcAddress(m_hDLL, "LoadExamHeaders");
//	if (DllLoadExamHeaders == NULL) goto _ERROR_;
//
//	DllSaveExamHeader = (TDllSaveExamHeader)::GetProcAddress(m_hDLL, "SaveExamHeader");
//	if (DllSaveExamHeader == NULL) goto _ERROR_;
//
//	DllGetExamSize = (TDllGetExamSize)::GetProcAddress(m_hDLL, "GetExamSize");
//	if (DllGetExamSize == NULL) goto _ERROR_;
//
//	DllLoadExam = (TDllLoadExam)::GetProcAddress(m_hDLL, "LoadExam");
//	if (DllLoadExam == NULL) goto _ERROR_;
//
//	DllSaveExam = (TDllSaveExam)::GetProcAddress(m_hDLL, "SaveExam");
//	if (DllSaveExam == NULL) goto _ERROR_;
//
//	DllDeleteExam = (TDllDeleteExam)::GetProcAddress(m_hDLL, "DeleteExam");
//	if (DllDeleteExam == NULL) goto _ERROR_;
//
//	DllGetCalibrationSize = (TDllGetCalibrationSize)::GetProcAddress(m_hDLL, "GetCalibrationSize");
//	if (DllGetCalibrationSize == NULL) goto _ERROR_;
//
//	DllLoadCalibration = (TDllLoadCalibration)::GetProcAddress(m_hDLL, "LoadCalibration");
//	if (DllLoadCalibration == NULL) goto _ERROR_;
//
//	DllSaveCalibration = (TDllSaveCalibration)::GetProcAddress(m_hDLL, "SaveCalibration");
//	if (DllSaveCalibration == NULL) goto _ERROR_;
//
//	if (Type == DATABASE_TYPE_MSJET_OLD) {
//		if (::PathFileExists(FileName)) {
//			BOOL Res = FALSE;
//			if (DllConnect(FileName, "", "", TRUE, FALSE)) {
//				Res = TRUE;
//			}
//			else if (DllConnect(FileName, "please", "", TRUE, FALSE)) {
//				Res = TRUE;
//			}
//			else {
//				CPasswordDlg* pDlg = new CPasswordDlg(NULL);
//				if (pDlg->DoModal() == IDOK) {
//					if (DllConnect(FileName, pDlg->m_Password, "", TRUE, FALSE), FALSE) {
//						Res = TRUE;
//					}
//					else {
//						CString Password("please" + pDlg->m_Password);
//						if (DllConnect(FileName, Password, "", TRUE, FALSE)) {
//							Res = TRUE;
//						}
//					}
//				}
//				delete pDlg;
//			}
//			if (!Res) {
//				::Error("Failed to open database.");
//			}
//		}
//		else {
//			CString s;
//			s.Format(_T("Database file \"%s\" does not exist.", FileName);
//			::Error(s);
//		}
//	}
//
//	else if (Type == DATABASE_TYPE_MSJET) {	
//		if (LocalServerFolderName == "")
//		{
//			if (!DllConnect(FolderName, "", "", ReadOnly, FALSE))
//			{
//				::Error("Failed to open database.");
//			}
//			//
//			else
//			{
//				if (NeedChangeLinker)
//				{
//					if (FirstRun)
//					{
//						DllDisconnect();
//
//						if (!DllConnect(FolderName, "", "", ReadOnly, FALSE))
//						{
//							::Error("Failed to open database.");
//						}
//					}
//
//					if (!DllChangeLiner(FolderName))
//					{
//						::Error("Failed to change Database linker.");
//					}
//				}
//			}
//		}
//		else //Connect to server
//		{
//			if (NeedChangeLinker)
//			{
//				if (!DllConnect(FolderName, "", "", ReadOnly, FALSE))
//				{
//					::Error("Failed to open database.");
//				}
//				if (FirstRun)
//				{
//					DllDisconnect();
//
//					if (!DllConnect(FolderName, "", "", ReadOnly, FALSE))
//					{
//						::Error("Failed to open database.");
//					}
//				}
//
//				CString SelfFolderName = GetSelfFolderName(FolderName);//The path on the server view(local computer)
//aaa				if (!DllChangeLiner(SelfFolderName))
//				{
//					::Error("Failed to change Database linker.");
//				}
//
//				DllDisconnect();
//			}
//
//			if (NeedChangeServerLinker)
//			{				
//				if (!DllConnect(FolderName, "", LocalServerFolderName, ReadOnly, FALSE))
//				{
//					::Error("Failed to open database.");
//				}
//
//				if (!DllChangeLiner(FolderName))
//				{
//					::Error("Failed to change Database linker.");
//				}
//			}
//			else
//			{
//				if (!DllConnect(FolderName, "", LocalServerFolderName, ReadOnly, FALSE))
//				{
//					::Error("Failed to open database.");
//				}
//			}
//		}
//		//
//	}
//
//
//	//530
//	if (::Licensing.IsDemo())
//	{
//		BOOL Wrong = FALSE;
//		CString DemoFileName = "C:\\Tracey\\Data\\MSJetEx\\exc.zip";
//		LoadPatients();
//		int count = m_Patients.GetSize();
//		if (count> 30)
//		{
//			Wrong = TRUE;
//		}
//		else
//		{
//			CDEMOID DemoData;
//			DemoData.LoadFromDemoFile(DemoFileName);
//
//			int DemoCount = 0;
//			for (int i = 0; i < 30; i++)
//			{
//				if (DemoData.PatientID[i] != GUID_NULL) DemoCount++;
//				else break;
//			}
//
//			if (count != DemoCount)
//			{
//				Wrong = TRUE;
//			}
//			else
//			{
//				for (int i = 0; i < count; i++)
//				{
//					if (m_Patients[i].m_PatientID != DemoData.PatientID[i])
//					{
//						Wrong = TRUE;
//						break;
//					}
//				}
//			}
//		}
//
//		if (Wrong)
//		{
//			Disconnect();
//
//			//Delete all files inside the folder
//			DeleteDirectory((LPSTR)(LPCTSTR)FolderName);
//
//			Connect(::Settings.m_OpenDBAsReadOnly);
//		}
//	}
//	//530
//
//	return;
//
//_ERROR_:
//
//	if (m_hDLL != NULL) {
//		::FreeLibrary(m_hDLL);
//		m_hDLL = NULL;
//	}
//
//	::Error("Failed to load database DLL file.");
//}



//CString CDB::SplitDatabaseProcess(CString FolderName, BOOL& NeedChangeLinker, BOOL& NeedChangeServerLinker, BOOL& FirstRun)
//{
//	NeedChangeLinker = FALSE;
//	NeedChangeServerLinker = FALSE;
//
//	CString LocalServerFolderName = "";
//		
//	CString JetFileName = FolderName + "\\tdb.mdb";
//
//	FirstRun = FALSE;
//	//The code is first runing, construct the local default database
//	if (!::PathFileExists(FolderName))
//	{
//		if (!::CreateDirectory(FolderName, NULL)) {
//			Error("Failed to create database folder.");
//			return LocalServerFolderName;
//		}
//
//		CString DefaultFolder = MAIN_DIR + "\\Data\\MSJet";
//		if (FolderName == DefaultFolder)
//		{
//			FirstRun = TRUE;
//
//			//Copy the linker tdb.mdb to local database place
//			CString StartDatabaseName = MAIN_DIR + "\\tdb.mdb";
//			CopyDatabaseFrontEndFile(JetFileName, StartDatabaseName);
//			//Done
//		}		
//	}
//	//the code is first runing, construct the local default database Done
//
//	if (!FirstRun)
//	{		
//		if (!IsLocalDatabase(JetFileName))
//		{
//			if (!::PathFileExists(JetFileName))
//			{
//				CString StartDatabaseName = MAIN_DIR + "\\tdb.mdb";
//				CopyDatabaseFrontEndFile(JetFileName, StartDatabaseName);
//
//				//Change the front-end linker to current location
//				NeedChangeLinker = TRUE;
//				FirstRun = TRUE;
//				//Change the front-end linker Done
//			}
//			else
//			{
//				CString BackEndFileName = FolderName + "\\tdb_be.mdb";
//				if (!::PathFileExists(BackEndFileName)) //Means it is an old database folder
//				{
//					CString StartDatabaseName = MAIN_DIR + "\\tdb.mdb";
//
//					//Rename the tdb.mdb to tdb_be.mdb
//					rename(JetFileName, BackEndFileName);
//					remove(JetFileName);
//					//Rename the tdb.mdb to tdb_be.mdb Done
//
//					//copy the front-end database
//					CopyDatabaseFrontEndFile(JetFileName, StartDatabaseName);
//					//copy the front-end database Done
//
//					//Change the front-end linker to current location
//					NeedChangeLinker = TRUE;
//					//Change the front-end linker Done
//				}
//			}
//		}
//		else
//		{
//			GetLocalFolderName(LocalServerFolderName, FolderName);
//
//			if (!::PathFileExists(JetFileName))
//			{
//				CString StartDatabaseName = MAIN_DIR + "\\tdb.mdb";
//				CopyDatabaseFrontEndFile(JetFileName, StartDatabaseName);
//
//				//Change the front-end linker to current location
//				NeedChangeLinker = TRUE;				
//				FirstRun = TRUE;
//				//Change the front-end linker Done
//			}
//			else
//			{
//				CString BackEndFileName = FolderName + "\\tdb_be.mdb";
//				if (!::PathFileExists(BackEndFileName)) //Means it is an old database folder
//				{
//					CString StartDatabaseName = MAIN_DIR + "\\tdb.mdb";
//
//					//Rename the tdb.mdb to tdb_be.mdb
//					rename(JetFileName, BackEndFileName);
//					remove(JetFileName);
//					//Rename the tdb.mdb to tdb_be.mdb Done
//
//					//copy the front-end database
//					CopyDatabaseFrontEndFile(JetFileName, StartDatabaseName);
//					//copy the front-end database Done
//
//					//Change the front-end linker to current location
//					NeedChangeLinker = TRUE;					
//					//Change the front-end linker Done
//				}
//			}								
//
//			//If the loacl folder does not exist, create it and then copy the front-end database form server
//			if (!::PathFileExists(LocalServerFolderName))
//			{		
//				::CreateDirectory(LocalServerFolderName, NULL);
//
//				//Copy the front-end database to here
//				CopyDatabaseFrontEndFile(LocalServerFolderName + "\\tdb.mdb", JetFileName);
//				//Done
//
//				NeedChangeServerLinker = TRUE;
//			}
//			else
//			{
//				//If the loacl tdb file does not exist, copy the front-end database form server and change the linker
//				if (!::PathFileExists(LocalServerFolderName + "\\tdb.mdb"))
//				{	
//					//Copy the front-end database to here
//					CopyDatabaseFrontEndFile(LocalServerFolderName + "\\tdb.mdb", JetFileName);
//					//Done
//
//					NeedChangeServerLinker = TRUE;
//				}
//			}
//			//Get the local front_end database, and set it the value 'FolderName';	Done				
//			
//		}
//	}
//
//	return LocalServerFolderName;
//}
//
//***************************************************************************************
////
//BOOL  CDB::IsLocalDatabase(CString JetFileName)
//{
//	//Call WNetGetUniversalName with the REMOTE_NAME_INFO_LEVEL option
//	DWORD cbBuff = 1000;    // Size of Buffer
//	TCHAR szBuff[1000];    // Buffer to receive information
//	REMOTE_NAME_INFO  * prni = (REMOTE_NAME_INFO *)&szBuff;
//	UNIVERSAL_NAME_INFO * puni = (UNIVERSAL_NAME_INFO *)&szBuff;
//	
//	if ((WNetGetUniversalName(JetFileName, REMOTE_NAME_INFO_LEVEL, (LPVOID)&szBuff, &cbBuff)) == NO_ERROR) //It is from network,
//		return FALSE;
//	
//	return TRUE;
//}
////
//***************************************************************************************