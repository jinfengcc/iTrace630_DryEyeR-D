/***************************************************************************************
Autor:          Jinfeng Chen
E-mail:         jinfengcc@gmail.com
Date:           03-02-2016
Description:    The databases merge destination dialog.
Version:        

Modification 1
Modifier:
E-mail:
Time:
Description:
***************************************************************************************/

#include "StdAfx.h"
#include "Resource.h"
#include "DatabaseDestDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CDatabaseDestDlg, CDialog)

	ON_BN_CLICKED(IDC_FOLDER_BROWSE, OnFileBrowseButtonClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CDatabaseDestDlg::CDatabaseDestDlg(CWnd* pParentWnd, CString DefaultDabaseFile) : CDialog(IDD_DATA_MERAGE_DEST_DLG, pParentWnd)
{
	m_FileName = DefaultDabaseFile;
}

//***************************************************************************************

void CDatabaseDestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_FILENAME_EDIT, m_FileNameEdit);
	DDX_Control(pDX, IDC_FOLDER_BROWSE, m_FileBrowseButton);
}

//***************************************************************************************

BOOL CDatabaseDestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_FileNameEdit.SetWindowText(m_FileName);

	m_FileNameEdit.SetFocus();

	return TRUE;
}

//***************************************************************************************

void CDatabaseDestDlg::OnOK()
{
	m_FileNameEdit.GetWindowText(m_FileName);

	if (m_FileName == "")
	{
		::Info("Please select a destination database file.");
		return;
	}

	CDialog::OnOK();
}


//***************************************************************************************

void CDatabaseDestDlg::OnFileBrowseButtonClicked()
{
	CString FolderName = "C:\\";
	CFileDialog* pDlg = new CFileDialog(TRUE, _T("mdb"), _T(""), OFN_HIDEREADONLY, _T("MDB files (*.mdb)|*.mdb||"), NULL);
	pDlg->m_ofn.lpstrTitle = _T("Select a file");

	pDlg->m_ofn.lpstrInitialDir = FolderName;

	if (pDlg->DoModal() == IDOK)
	{
		m_FileName = pDlg->GetPathName();
		m_FileNameEdit.SetWindowText(m_FileName);
	}
}

//***************************************************************************************