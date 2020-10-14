/***************************************************************************************
Autor:          Jinfeng Chen
E-mail:         jinfengcc@gmail.com
Date:           03-02-2016
Description:    The databases merge dialog.
Version:        

Modification 1
Modifier:
E-mail:
Time:
Description:
***************************************************************************************/

#include "StdAfx.h"
#include "Resource.h"
#include "Data.h"
#include "Settings.h"
#include "DatabaseDestDlg.h"

#include "DatabaseMerge.h"

#define BUF_SIZE    256

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CDatabaseMergeDlg, CDialog)

	ON_BN_CLICKED(IDC_FOLDER_BROWSE1, OnFileBrowseButton1Clicked)
	ON_BN_CLICKED(IDC_FOLDER_BROWSE2, OnFileBrowseButton2Clicked)
	ON_BN_CLICKED(IDC_FOLDER_BROWSE3, OnFileBrowseButton3Clicked)
	ON_BN_CLICKED(IDC_FOLDER_BROWSE4, OnFileBrowseButton4Clicked)
	ON_BN_CLICKED(IDC_FOLDER_BROWSE5, OnFileBrowseButton5Clicked)

END_MESSAGE_MAP()

//***************************************************************************************

CDatabaseMergeDlg::CDatabaseMergeDlg(CWnd* pParentWnd, CString DefaultDabaseFile) : CDialog(IDD_DATA_MERAGE_DLG, pParentWnd)
{
	m_FileNames[0] = DefaultDabaseFile;
}

//***************************************************************************************

void CDatabaseMergeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_FILENAME_EDIT1, m_FileNameEdit[0]);
	DDX_Control(pDX, IDC_FILENAME_EDIT2, m_FileNameEdit[1]);
	DDX_Control(pDX, IDC_FILENAME_EDIT3, m_FileNameEdit[2]);
	DDX_Control(pDX, IDC_FILENAME_EDIT4, m_FileNameEdit[3]);
	DDX_Control(pDX, IDC_FILENAME_EDIT5, m_FileNameEdit[4]);

	DDX_Control(pDX, IDC_FOLDER_BROWSE1, m_FileBrowseButton1);
	DDX_Control(pDX, IDC_FOLDER_BROWSE2, m_FileBrowseButton2);
	DDX_Control(pDX, IDC_FOLDER_BROWSE3, m_FileBrowseButton3);
	DDX_Control(pDX, IDC_FOLDER_BROWSE4, m_FileBrowseButton4);
	DDX_Control(pDX, IDC_FOLDER_BROWSE5, m_FileBrowseButton5);
}

//***************************************************************************************

BOOL CDatabaseMergeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_FileNameEdit[0].SetWindowText(m_FileNames[0]);

	m_FileNameEdit[0].SetFocus();

	return TRUE;
}

//***************************************************************************************

void CDatabaseMergeDlg::OnOK()
{
	CString FolderName[5], FileName;

	int t = 0;
	for (int i = 0; i < 5; i++)
	{
		m_FileNameEdit[i].GetWindowText(m_FileNames[i]);

		if (m_FileNames[i] != "" && ::PathFileExists(m_FileNames[i]))
		{
			t++;
		}

		GetFolderandFileName(m_FileNames[i], FolderName[i], FileName);
		{
			if (FileName == "tdb_be.mdb")
			{
				::Info("The 'tdb_be.mdb' file can not be merged, sorry for that.");
				m_FileNameEdit[i].SetWindowText(_T(""));
				m_FileNameEdit[i].SetFocus();
				return;
			}

			for (int j = 0; j < i; j++)
			{
				if (FolderName[i] == FolderName[j] && FolderName[i] != "")
				{
					::Info("The merged database file (*.mdb) can not be under same database folder, please create a new folder for it.(Except tdb_be.mdb)");
					m_FileNameEdit[i].SetWindowText(_T(""));
					m_FileNameEdit[i].SetFocus();
					return;
				}
			}
		}
	}

	if (t <= 1)
	{
		::Info("Please select at least two databases.");
		return;
	}

	if (!MergeDatabases()) return;

	CDialog::OnOK();
}

//***************************************************************************************

BOOL CDatabaseMergeDlg::MergeDatabases()
{
	CString s, DesFolder, DesFileName, sel_Folder[5], FileName;

	BOOL DisDataName;
	BOOL NoExamFolder[5];

	CDatabaseDestDlg* pDlg = new CDatabaseDestDlg(this, m_FileNames[0]);

	if (pDlg->DoModal() == IDOK)
	{
		//warning the replace danger if there already own a database 
		m_DestOriFileName = pDlg->GetDestFile();
		if (::PathFileExists(m_DestOriFileName))
		{
			s = "The merged database will replace the current database '" + m_DestOriFileName + "'. Do you want to replace it ?";
			if (!::Question(s, 1, 0))
			{
				return FALSE;
			}
		}
		//warning the replace danger if there already own a database Done

		m_tClinics = 0;
		m_tExamHeaders = 0;
		m_tEyes = 0;
		m_tGroups = 0;
		m_tOperators = 0;
		m_tPatients = 0;
		m_tPhysicians = 0;

		::DB.Disconnect();
		//Read the selected database
		for (int i = 0; i < 5; i++)
		{
			DisDataName = FALSE;
			NoExamFolder[i] = FALSE;

			m_FileNameEdit[i].GetWindowText(m_FileNames[i]);

			if (m_FileNames[i] != "")
			{
				GetFolderandFileName(m_FileNames[i], sel_Folder[i], FileName);

				//Check the database exams and cal folders have data inside
				if (!::PathFileExists(sel_Folder[i] + "\\Exams") || !::PathFileExists(sel_Folder[i] + "\\Cals"))
				{
					NoExamFolder[i] = TRUE;

					s = "Database folder '" + sel_Folder[i] + "\\'";
					s = "There are not corresponding exam folder and data under " + s;
					s += ", do you still want to merge it ? ";

					if (!::Question(s, 1, 0))
					{
						::DB.Connect(::Settings.m_OpenDBAsReadOnly);
						delete pDlg;

						if (i == 0) m_FileNameEdit[0].SetFocus();
						else if (i == 1) m_FileNameEdit[1].SetFocus();
						else if (i == 2) m_FileNameEdit[2].SetFocus();
						else if (i == 3) m_FileNameEdit[3].SetFocus();
						else if (i == 4) m_FileNameEdit[4].SetFocus();

						return FALSE;
					}
				}
				//Check the database exams and cal folders have data inside Done

				if (FileName != "tdb.mdb")
				{
					DisDataName = TRUE;

					//Copy this data file to tdb.mdb	
					if (!CreateTdbFile(sel_Folder[i] + "\\tdb.mdb", m_FileNames[i]))
					{
						::Info("Please change the database file name to tdb.mdb.");
						::DB.Connect(::Settings.m_OpenDBAsReadOnly);
						delete pDlg;
						return FALSE;
					}
					//Copy this data file to tdb.mdb done
				}

				m_ThisDB.ConnectOtherMsjet(sel_Folder[i]);

				//Save the clinics
				if (!SaveClinics(i))
				{
					::Error("Soory, the clinics number can not be greater than 1000.");
					m_ThisDB.Disconnect();
					::DB.Connect(::Settings.m_OpenDBAsReadOnly);
					delete pDlg;
					return FALSE;
				}
				//Save the clinics Done

				//Save the groups
				if (!SaveGroups(i))
				{
					::Error("Soory, the groups number can not be greater than 1000.");
					m_ThisDB.Disconnect();
					::DB.Connect(::Settings.m_OpenDBAsReadOnly);
					delete pDlg;
					return FALSE;
				}
				//Save the groups Done

				//Save the operators
				if (!SaveOperators(i))
				{
					::Error("Soory, the operators number can not be greater than 1000.");
					m_ThisDB.Disconnect();
					::DB.Connect(::Settings.m_OpenDBAsReadOnly);
					delete pDlg;
					return FALSE;
				}
				//Save the groups Done

				//Save the operators
				if (!SavePhysicians(i))
				{
					::Error("Soory, the physicians number can not be greater than 1000.");
					m_ThisDB.Disconnect();
					::DB.Connect(::Settings.m_OpenDBAsReadOnly);
					delete pDlg;
					return FALSE;
				}
				//Save the groups Done

				//Save the Eyes
				if (!SaveEyes(i))
				{
					::Error("Soory, the eyes number can not be greater than 50000.");
					m_ThisDB.Disconnect();
					::DB.Connect(::Settings.m_OpenDBAsReadOnly);
					delete pDlg;
					return FALSE;
				}
				//Save the Eyes Done

				//Save the ExamHeaders
				if (!SaveExamHeaders(i))
				{
					::Error("Soory, the exams number can not be greater than 50000.");
					m_ThisDB.Disconnect();
					::DB.Connect(::Settings.m_OpenDBAsReadOnly);
					delete pDlg;
					return FALSE;
				}
				//Save the ExamHeaders Done

				//Save the Patients
				if (!SavePatients(i))
				{
					::Error("Soory, the Patients number can not be greater than 5000.");
					m_ThisDB.Disconnect();
					::DB.Connect(::Settings.m_OpenDBAsReadOnly);
					delete pDlg;
					return FALSE;
				}
				//Save the Patients Done				

				m_ThisDB.Disconnect();

				//If the database name is not 'tdb.mdb', delete the copied 'tdb.mdb'
				if (DisDataName)
				{
					DeleteFile(sel_Folder[i] + "\\tdb.mdb");
				}
				//If the database name is not 'tdb.mdb', delete the copied 'tdb.mdb' Done		
			}
		}

		//Create the merged data set		
		BOOL  DifDesDataName = FALSE;

		GetFolderandFileName(m_DestOriFileName, DesFolder, DesFileName);

		if (DesFileName != "tdb.mdb")
		{
			DifDesDataName = TRUE;

			//Copy this data file to tdb.mdb	
			if (::PathFileExists(m_DestOriFileName))
			{
				if (!CreateTdbFile(DesFolder + "\\tdb.mdb", m_DestOriFileName))
				{
					::Info("Please change the destination database file name '" + m_DestOriFileName + "'to tdb.mdb.");
					delete pDlg;
					return FALSE;
				}
			}
			//Copy this data file to tdb.mdb done			
		}
		m_MergeDB.ConnectOtherMsjet(DesFolder);
		SaveMergedData();//Save the merged data sets				
		m_MergeDB.Disconnect();

		//Copy the files in exams and cal folders to merged folder
		CString OriExamFolder;
		CString OriCalFolder;
		CString DesExamFolder = DesFolder + "\\Exams";
		CString DesCalFolder = DesFolder + "\\Cals";

		if (!::PathFileExists(DesExamFolder)) ::CreateDirectory(DesExamFolder, NULL);
		if (!::PathFileExists(DesCalFolder)) ::CreateDirectory(DesCalFolder, NULL);

		for (int i = 0; i < 5; i++)
		{
			//copy all the files from selected folders to merged folder
			if (!NoExamFolder[i] && m_FileNames[i] != "")
			{
				OriExamFolder = sel_Folder[i] + "\\Exams";
				OriCalFolder = sel_Folder[i] + "\\Cals";

				CopyFolder(OriExamFolder, DesFolder);
				CopyFolder(OriCalFolder, DesFolder);
			}
		}
		//Copy the exams and cal folders to merged folder Done

		//Show the sucessful info
		::Info("The selected databases have been merged to '" + m_DestOriFileName + "' sucessfully.");
		//Show the sucessful info Done

		//If the database name is not 'tdb.mdb', copy the merged file to delete the copied 'tdb.mdb'
		if (DifDesDataName)
		{
			DeleteFile(m_DestOriFileName);
			CreateTdbFile(m_DestOriFileName, DesFolder + "\\tdb.mdb");
			DeleteFile(DesFolder + "\\tdb.mdb");

			s = "For using merged database correctly, please change the data name '" + DesFileName + "'to 'tdb.mdb'";
			::Info(s);
		}
		//If the database name is not 'tdb.mdb', delete the copied 'tdb.mdb' Done	

		::DB.Connect(::Settings.m_OpenDBAsReadOnly);
	}

	delete pDlg;

	return TRUE;
}
//***************************************************************************************
void CDatabaseMergeDlg::SaveMergedData()
{
	//Clinics
	m_MergeDB.LoadClinics();
	int count = m_MergeDB.m_Clinics.GetSize();

	for (int i = 0; i < count; i++)
	{
		m_MergeDB.DeleteClinic(m_MergeDB.m_Clinics[i].m_ClinicID);
	}


	for (int i = 0; i < m_tClinics; i++)
	{
		m_MergeDB.SaveClinic(m_pClinics[i]);
	}
	//Clinics Done

	//Groups
	for (int i = 0; i < m_tGroups; i++)
	{
		m_MergeDB.SaveGroup(m_pGroups[i]);
	}
	//Groups Done

	//Operators
	for (int i = 0; i < m_tOperators; i++)
	{
		m_MergeDB.SaveOperator(m_pOperators[i]);
	}
	//Operators Done

	//Physicians
	for (int i = 0; i < m_tPhysicians; i++)
	{
		m_MergeDB.SavePhysician(m_pPhysicians[i]);
	}
	//Physicians Done

	//Patients
	m_MergeDB.LoadPatients();
	count = m_MergeDB.m_Patients.GetSize();

	for (int i = 0; i < count; i++)
	{
		m_MergeDB.DeletePatient(m_MergeDB.m_Patients[i].m_PatientID);
	}


	for (int i = 0; i < m_tPatients; i++)
	{
		m_MergeDB.SavePatient(m_pPatients[i]);
	}
	//Patients Done

	//Eyes
	for (int i = 0; i < m_tEyes; i++)
	{
		m_MergeDB.SaveEye(m_pEyes[i]);
	}
	//Eyes Done

	//Eyes
	for (int i = 0; i < m_tExamHeaders; i++)
	{
		m_MergeDB.SaveExamHeaders(m_pExamHeaders[i]);
	}
	//Eyes Done
}
//***************************************************************************************
BOOL CDatabaseMergeDlg::SaveExamHeaders(int i)
{
	CPatient     This_Patient;
	m_ThisDB.LoadPatients();
	int count = m_ThisDB.m_Patients.GetSize();

	for (int j = 0; j < count; j++)
	{
		This_Patient = m_ThisDB.m_Patients[j];
		m_ThisDB.LoadExamHeaders(m_ThisDB.m_Patients[j].m_PatientID);//Big Database Research since 03062019
		//m_ThisDB.LoadTxtExamHeaders(m_ThisDB.m_Patients[j].m_PatientID);//Big Database Research since 03062019		

		if (m_ThisDB.m_NumExams > 0)
		{
			for (int k = 0; k < m_ThisDB.m_NumExams; k++)
			{
				if (NotInsideDataSet(m_ThisDB.m_ExamHeaders[k].m_ExamID, m_tExamHeaders, TYPE_EXAMHEADER))
				{
					m_pExamHeaders[m_tExamHeaders] = m_ThisDB.m_ExamHeaders[k];
					m_tExamHeaders++;
				}
			}
		}

		if (m_tExamHeaders > 50000)
		{
			return FALSE;
		}
	}

	return TRUE;
}
//***************************************************************************************
BOOL CDatabaseMergeDlg::SaveEyes(int i)
{
	CPatient     This_Patient;
	m_ThisDB.LoadPatients();
	int count = m_ThisDB.m_Patients.GetSize();

	for (int j = 0; j < count; j++)
	{
		This_Patient = m_ThisDB.m_Patients[j];
		m_ThisDB.LoadEyes(m_ThisDB.m_Patients[j].m_PatientID);

		if (m_ThisDB.m_EyeNum >= 1 && m_ThisDB.m_EyeNum <= 2)
		{
			for (int k = 0; k < m_ThisDB.m_EyeNum; k++)
			{
				if (NotInsideDataSet(m_ThisDB.m_Eyes[k].m_EyeID, m_tEyes, TYPE_EYE))
				{
					m_pEyes[m_tEyes] = m_ThisDB.m_Eyes[k];
					m_tEyes++;
				}
			}
		}

		if (m_tEyes > 1000)
		{
			return FALSE;
		}
	}

	return TRUE;
}
//***************************************************************************************
BOOL CDatabaseMergeDlg::SaveClinics(int i)
{
	m_ThisDB.LoadClinics();
	int count = m_ThisDB.m_Clinics.GetSize();

	for (int j = 0; j < count; j++)
	{
		if (i != 0)
		{
			if (NotInsideDataSet(m_ThisDB.m_Clinics[j].m_ClinicID, m_tClinics, TYPE_CLINIC))
			{
				m_pClinics[m_tClinics] = m_ThisDB.m_Clinics[j];
				m_tClinics++;
			}
		}
		else
		{
			m_pClinics[m_tClinics] = m_ThisDB.m_Clinics[j];
			m_tClinics++;
		}
		if (m_tClinics > 1000)
		{
			return FALSE;
		}
	}

	return TRUE;
}
//***************************************************************************************
BOOL CDatabaseMergeDlg::SaveGroups(int i)
{
	m_ThisDB.LoadGroups();
	int count = m_ThisDB.m_Groups.GetSize();

	for (int j = 0; j < count; j++)
	{
		if (i != 0)
		{
			if (NotInsideDataSet(m_ThisDB.m_Groups[j].m_GroupID, m_tGroups, TYPE_GROUP))
			{
				m_pGroups[m_tGroups] = m_ThisDB.m_Groups[j];
				m_tGroups++;
			}
		}
		else
		{
			m_pGroups[m_tGroups] = m_ThisDB.m_Groups[j];
			m_tGroups++;
		}
		if (m_tGroups > 1000)
		{
			return FALSE;
		}
	}

	return TRUE;
}
//***************************************************************************************
BOOL CDatabaseMergeDlg::SaveOperators(int i)
{
	m_ThisDB.LoadOperators();
	int count = m_ThisDB.m_Operators.GetSize();

	for (int j = 0; j < count; j++)
	{
		if (i != 0)
		{
			if (NotInsideDataSet(m_ThisDB.m_Operators[j].m_OperatorID, m_tOperators, TYPE_OPERATOR))
			{
				m_pOperators[m_tOperators] = m_ThisDB.m_Operators[j];
				m_tOperators++;
			}
		}
		else
		{
			m_pOperators[m_tOperators] = m_ThisDB.m_Operators[j];
			m_tOperators++;
		}
		if (m_tOperators > 1000)
		{
			return FALSE;
		}
	}

	return TRUE;
}
//***************************************************************************************
BOOL CDatabaseMergeDlg::SavePatients(int i)
{
	m_ThisDB.LoadPatients();
	int count = m_ThisDB.m_Patients.GetSize();

	for (int j = 0; j < count; j++)
	{
		if (i != 0)
		{
			if (NotInsideDataSet(m_ThisDB.m_Patients[j].m_PatientID, m_tPatients, TYPE_PATIENT))
			{
				m_pPatients[m_tPatients] = m_ThisDB.m_Patients[j];
				m_tPatients++;
			}
		}
		else
		{
			m_pPatients[m_tPatients] = m_ThisDB.m_Patients[j];
			m_tPatients++;
		}
		if (m_tPatients > 5000)
		{
			return FALSE;
		}
	}

	return TRUE;
}
//***************************************************************************************
BOOL CDatabaseMergeDlg::SavePhysicians(int i)
{
	m_ThisDB.LoadPhysicians();
	int count = m_ThisDB.m_Physicians.GetSize();

	for (int j = 0; j < count; j++)
	{
		if (i != 0)
		{
			if (NotInsideDataSet(m_ThisDB.m_Physicians[j].m_PhysicianID, m_tPhysicians, TYPE_PHYSICIAN))
			{
				m_pPhysicians[m_tPhysicians] = m_ThisDB.m_Physicians[j];
				m_tPhysicians++;
			}
		}
		else
		{
			m_pPhysicians[m_tPhysicians] = m_ThisDB.m_Physicians[j];
			m_tPhysicians++;
		}
		if (m_tPhysicians > 1000)
		{
			return FALSE;
		}
	}

	return TRUE;
}
//***************************************************************************************

//Check the import patient is already in the dataset or not
BOOL CDatabaseMergeDlg::NotInsideDataSet(GUID ImportID, int setNumber, int type)
{
	GUID id;
	for (int i = 0; i < setNumber; i++)
	{
		switch (type)
		{
		case TYPE_CLINIC:
			id = m_pClinics[i].m_ClinicID;
			break;

		case TYPE_EXAMHEADER:
			id = m_pExamHeaders[i].m_ExamID;
			break;

		case TYPE_EYE:
			id = m_pEyes[i].m_EyeID;
			break;

		case TYPE_GROUP:
			id = m_pGroups[i].m_GroupID;
			break;

		case TYPE_OPERATOR:
			id = m_pOperators[i].m_OperatorID;
			break;

		case TYPE_PATIENT:
			id = m_pPatients[i].m_PatientID;
			break;

		case TYPE_PHYSICIAN:
			id = m_pPhysicians[i].m_PhysicianID;
			break;

		}
		if (ImportID == id) return FALSE;
	}
	return TRUE;
}

//***************************************************************************************

//Copy this data file to tdb.mdb	
BOOL CDatabaseMergeDlg::CreateTdbFile(CString DestTdbFileName, CString DestOriFileName)
{
	BOOL Res = TRUE;
	char data[BUF_SIZE];
	size_t bytes_in, bytes_out;

	CStringA ADestTdbFileName(DestTdbFileName);
	CStringA ADestOriFileName(DestOriFileName);
	FILE* pFileIn = fopen(ADestTdbFileName, "rb");
	FILE* pFileOut = fopen(ADestOriFileName, "wb");
	//while (bytes_in = fgets(s, sizeof(s), pFileIn))
	while ((bytes_in = fread(data, 1, BUF_SIZE, pFileIn)) > 0)
	{
		bytes_out = fwrite(data, 1, bytes_in, pFileOut);
		if (bytes_in != bytes_out)
		{
			Res = FALSE;
		}
	}
	fclose(pFileIn);
	fclose(pFileOut);

	return Res;
}
//Copy this data file to tdb.mdb done

//***************************************************************************************

void CDatabaseMergeDlg::GetFolderandFileName(CString PathName, CString &Folder, CString &FileName)
{
	int strLength = PathName.GetLength();

	char *pValue = new char[strLength];

	CStringA APathName(PathName);
	strncpy(pValue, APathName, strLength);

	int FindPos = 0;
	for (int j = 0; j < strLength - 1; j++)
	{
		char c = *(pValue + j);
		if (c == '\\')
		{
			FindPos = j;
		}
	}

	Folder = PathName;
	Folder.Truncate(FindPos);

	FileName = PathName;
	FileName.Replace(Folder + _T("\\"), _T(""));
}

//***************************************************************************************

void CDatabaseMergeDlg::OnFileBrowseButton1Clicked()
{
	CString FolderName = "C:\\";
	CFileDialog* pDlg = new CFileDialog(TRUE, _T("mdb"), _T(""), OFN_HIDEREADONLY, _T("MDB files (*.mdb)|*.mdb||"), NULL);
	pDlg->m_ofn.lpstrTitle = _T("Select a file");
	pDlg->m_ofn.lpstrInitialDir = FolderName;
	if (pDlg->DoModal() == IDOK) {
		m_FileNames[0] = pDlg->GetPathName();
		m_FileNameEdit[0].SetWindowText(m_FileNames[0]);
	}
}
//***************************************************************************************

void CDatabaseMergeDlg::OnFileBrowseButton2Clicked()
{
	CString FolderName = "C:\\";
	CFileDialog* pDlg = new CFileDialog(TRUE, _T("mdb"), _T(""), OFN_HIDEREADONLY, _T("MDB files (*.mdb)|*.mdb||"), NULL);
	pDlg->m_ofn.lpstrTitle = _T("Select a file");
	pDlg->m_ofn.lpstrInitialDir = FolderName;
	if (pDlg->DoModal() == IDOK) {
		m_FileNames[1] = pDlg->GetPathName();
		m_FileNameEdit[1].SetWindowText(m_FileNames[1]);
	}
}
//***************************************************************************************

void CDatabaseMergeDlg::OnFileBrowseButton3Clicked()
{
	CString FolderName = "C:\\";
	CFileDialog* pDlg = new CFileDialog(TRUE, _T("mdb"), _T(""), OFN_HIDEREADONLY, _T("MDB files (*.mdb)|*.mdb||"), NULL);
	pDlg->m_ofn.lpstrTitle = _T("Select a file");
	pDlg->m_ofn.lpstrInitialDir = FolderName;
	if (pDlg->DoModal() == IDOK) {
		m_FileNames[2] = pDlg->GetPathName();
		m_FileNameEdit[2].SetWindowText(m_FileNames[2]);
	}
}
//***************************************************************************************

void CDatabaseMergeDlg::OnFileBrowseButton4Clicked()
{
	CString FolderName = "C:\\";
	CFileDialog* pDlg = new CFileDialog(TRUE, _T("mdb"), _T(""), OFN_HIDEREADONLY, _T("MDB files (*.mdb)|*.mdb||"), NULL);
	pDlg->m_ofn.lpstrTitle = _T("Select a file");
	pDlg->m_ofn.lpstrInitialDir = FolderName;
	if (pDlg->DoModal() == IDOK) {
		m_FileNames[3] = pDlg->GetPathName();
		m_FileNameEdit[3].SetWindowText(m_FileNames[3]);
	}
}
//***************************************************************************************

void CDatabaseMergeDlg::OnFileBrowseButton5Clicked()
{
	CString FolderName = "C:\\";
	CFileDialog* pDlg = new CFileDialog(TRUE, _T("mdb"), _T(""), OFN_HIDEREADONLY, _T("MDB files (*.mdb)|*.mdb||"), NULL);
	pDlg->m_ofn.lpstrTitle = _T("Select a file");
	pDlg->m_ofn.lpstrInitialDir = FolderName;
	if (pDlg->DoModal() == IDOK) {
		m_FileNames[4] = pDlg->GetPathName();
		m_FileNameEdit[4].SetWindowText(m_FileNames[4]);
	}
}
//***************************************************************************************