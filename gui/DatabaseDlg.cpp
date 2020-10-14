//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "Registry.h"
#include "Settings.h"
#include "FolderDlg.h"
#include "PasswordDlg.h"
#include "BusyCursor.h"
#include "DatabaseMerge.h"
#include "INIParser.h"//6.2.0 ini registration

#include "DatabaseDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CDatabaseDlg, CDialog)

	ON_BN_CLICKED(IDC_MSJET_RADIO, OnMSJetRadioClicked)
	ON_BN_CLICKED(IDC_MYSQL_RADIO, OnMySQLRadioClicked)
	ON_BN_CLICKED(IDC_MSJETOLD_RADIO, OnMSJetOldRadioClicked)
	ON_BN_CLICKED(IDC_FOLDER_BROWSE, OnFolderBrowseButtonClicked)
	ON_BN_CLICKED(IDC_FILE_BROWSE, OnFileBrowseButtonClicked)
	ON_BN_CLICKED(IDC_REINIT, OnReinitButtonClicked)

	ON_COMMAND(IDK_CTRL_E, OnCtrlE)//
	ON_BN_CLICKED(IDC_DATABASE_MERGE, OnMegrgeButtonClicked)// For database merge

END_MESSAGE_MAP()

//***************************************************************************************

CDatabaseDlg::CDatabaseDlg(CWnd* pParentWnd) : CDialog(IDD_DATABASE_DLG, pParentWnd)
{
	m_hAccel = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR));//

	::DB.GetType(m_DatabaseType);
	::DB.GetFolderName(m_FolderName);
	::DB.GetFileName(m_FileName);
}

//***************************************************************************************
//
CDatabaseDlg::~CDatabaseDlg()
{
	DestroyAcceleratorTable(m_hAccel);
}
//
//***************************************************************************************

void CDatabaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_MSJET_RADIO, m_MSJetRadio);
	DDX_Control(pDX, IDC_MYSQL_RADIO, m_MySQLRadio);
	DDX_Control(pDX, IDC_MSJETOLD_RADIO, m_MSJetOldRadio);
	DDX_Control(pDX, IDC_FOLDER_BROWSE, m_FolderBrowseButton);
	DDX_Control(pDX, IDC_FILE_BROWSE, m_FileBrowseButton);
	DDX_Control(pDX, IDC_REINIT, m_ReinitButton);
	DDX_Control(pDX, IDC_FOLDERNAME_EDIT, m_FolderNameEdit);
	DDX_Control(pDX, IDC_FILENAME_EDIT, m_FileNameEdit);
	DDX_Control(pDX, IDC_READ_ONLY_CHECK, m_ReadOnlyCheck);

	DDX_Control(pDX, IDC_DATABASE_MERGE, m_DatabaseMergeButton);// For database merge

}
//***************************************************************************************
//
BOOL CDatabaseDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == 0x45 && GetKeyState(VK_CONTROL) < 0) {
		if (TranslateAccelerator(m_hWnd, m_hAccel, pMsg)) return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
//
//***************************************************************************************

BOOL CDatabaseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	switch (m_DatabaseType) {
	case DATABASE_TYPE_MSJET:
		m_MSJetRadio.SetCheck(TRUE);
		OnMSJetRadioClicked();
		break;
	case DATABASE_TYPE_MYSQL:
		m_MySQLRadio.SetCheck(TRUE);
		OnMySQLRadioClicked();
		break;
	case DATABASE_TYPE_MSJET_OLD:
		m_MSJetOldRadio.SetCheck(TRUE);
		OnMSJetOldRadioClicked();
		break;
	}

	return TRUE;
}

//***************************************************************************************

void CDatabaseDlg::OnOK()
{
	if (m_DatabaseType == DATABASE_TYPE_MSJET) {

		::Settings.m_OpenDBAsReadOnly = m_ReadOnlyCheck.GetCheck();

		m_FolderNameEdit.GetWindowText(m_FolderName);
		if (!::PathFileExists(m_FolderName))
		{
			CString s;
			s.Format(_T("Database folder \"%s\" does not exist."), m_FolderName);
			::Error(s);
			return;
		}
	}

	else if (m_DatabaseType == DATABASE_TYPE_MSJET_OLD) {
		m_FileNameEdit.GetWindowText(m_FileName);
		if (!::PathFileExists(m_FileName)) {
			CString s;
			s.Format(_T("Database file \"%s\" does not exist."), m_FileName);
			::Error(s);
			return;
		}
	}

	CDialog::OnOK();
}

//***************************************************************************************

void CDatabaseDlg::OnMSJetRadioClicked()
{
	m_DatabaseType = DATABASE_TYPE_MSJET;

	m_FolderNameEdit.EnableWindow(TRUE);
	m_FolderNameEdit.SetWindowText(m_FolderName);
	m_FolderBrowseButton.EnableWindow(TRUE);
	m_ReadOnlyCheck.EnableWindow(TRUE);
	m_ReadOnlyCheck.SetCheck(::Settings.m_OpenDBAsReadOnly);

	m_FileBrowseButton.EnableWindow(FALSE);
	m_FileNameEdit.EnableWindow(FALSE);
	m_FileNameEdit.SetWindowText(_T(""));

	m_ReinitButton.EnableWindow(FALSE);
}

//***************************************************************************************

void CDatabaseDlg::OnMySQLRadioClicked()
{
	m_DatabaseType = DATABASE_TYPE_MYSQL;

	m_FolderNameEdit.EnableWindow(FALSE);
	m_FolderNameEdit.SetWindowText(_T(""));
	m_FolderBrowseButton.EnableWindow(FALSE);
	m_ReadOnlyCheck.EnableWindow(FALSE);
	m_ReadOnlyCheck.SetCheck(FALSE);

	m_FileBrowseButton.EnableWindow(FALSE);
	m_FileNameEdit.EnableWindow(FALSE);
	m_FileNameEdit.SetWindowText(_T(""));

	m_ReinitButton.EnableWindow(TRUE);
}

//***************************************************************************************

void CDatabaseDlg::OnMSJetOldRadioClicked()
{
	m_DatabaseType = DATABASE_TYPE_MSJET_OLD;

	m_FolderNameEdit.EnableWindow(FALSE);
	m_FolderNameEdit.SetWindowText(_T(""));
	m_FolderBrowseButton.EnableWindow(FALSE);
	m_ReadOnlyCheck.EnableWindow(FALSE);
	m_ReadOnlyCheck.SetCheck(FALSE);

	m_FileBrowseButton.EnableWindow(TRUE);
	m_FileNameEdit.EnableWindow(TRUE);
	m_FileNameEdit.SetWindowText(m_FileName);

	m_ReinitButton.EnableWindow(FALSE);
}

//***************************************************************************************

void CDatabaseDlg::OnFolderBrowseButtonClicked()
{
	CINIParser IniPar;
	CString DatabaseFolder = IniPar.GetStrKey(IniSection, "Database Folder", IniPath);//6.2.0 ini registration

	CFolderDlg* pDlg = new CFolderDlg(this, "");
	pDlg->m_InitialFolderName = DatabaseFolder;

	if (pDlg->Show())
	{
		m_FolderName = pDlg->m_FolderName;
		m_FolderNameEdit.SetWindowText(m_FolderName);
	}

	delete pDlg;
}

//***************************************************************************************

//Merge the database
void CDatabaseDlg::OnMegrgeButtonClicked()
{
	CBusyCursor Cursor;
	CString DatabaseFileName = m_FolderName + "\\tdb.mdb";
	CDatabaseMergeDlg* pDlg = new CDatabaseMergeDlg(this, DatabaseFileName);

	if (pDlg->DoModal() == IDOK)
	{
		int a = 0;
	}

	delete pDlg;
}

//***************************************************************************************

void CDatabaseDlg::OnFileBrowseButtonClicked()
{
	CINIParser IniPar;
	CString DatabaseFolder = IniPar.GetStrKey(IniSection, "Database Folder", IniPath);//6.2.0 ini registration

	if (DatabaseFolder != "")
	{
		CFileDialog* pDlg = new CFileDialog(TRUE, _T("mdb"), _T(""), OFN_HIDEREADONLY, _T("MDB files (*.mdb)|*.mdb||"), NULL);
		pDlg->m_ofn.lpstrTitle = _T("Select a file");
		pDlg->m_ofn.lpstrInitialDir = DatabaseFolder;

		if (pDlg->DoModal() == IDOK)
		{
			m_FileName = pDlg->GetPathName();
			m_FileNameEdit.SetWindowText(m_FileName);
		}
	}
}

//***************************************************************************************

void CDatabaseDlg::OnReinitButtonClicked()
{
	::DB.Reinit();
}

//***************************************************************************************
// Database merge
void CDatabaseDlg::OnCtrlE()
{
	/*CPasswordDlg* pDlg = new CPasswordDlg(this);
	int Res = pDlg->DoModal();
	CString Password = pDlg->m_Password;
	delete pDlg;

	if (Res == IDOK && Password == "tracey")
	{
	OnMegrgeButtonClicked();
	}	*/
}
//
//***************************************************************************************
