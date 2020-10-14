//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "FirmwareToolDlg.h"
#include "BusyCursor.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CFirmwareToolDlg, CDialog)

	ON_BN_CLICKED(IDC_CAL_BROWSE, OnBrowseButtonClicked)
	ON_BN_CLICKED(IDC_CAL_FIRMWARE_COMPARE, OnCompareButtonClicked)
	ON_BN_CLICKED(IDC_CAL_FIRMWARE_UPDATE, OnUpdateButtonClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CFirmwareToolDlg::CFirmwareToolDlg(CHW* pHW, CWnd* pParentWnd) :
	CDialog(IDD_FIRMWARE_TOOL_DLG, pParentWnd)
{
	m_pHW = pHW;
}

//***************************************************************************************

void CFirmwareToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CAL_FIRMWARE_BUILD_EDIT, m_FirmwareBuildEdit);
	DDX_Control(pDX, IDC_CAL_FIRMWARE_FILE_EDIT, m_FirmwareFileEdit);
	DDX_Control(pDX, IDC_CAL_BROWSE, m_BrowseButton);
	DDX_Control(pDX, IDC_CAL_FIRMWARE_COMPARE, m_CompareButton);
	DDX_Control(pDX, IDC_CAL_FIRMWARE_UPDATE, m_UpdateButton);
}

//***************************************************************************************

BOOL CFirmwareToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString s;
	s.Format(_T("%i"), m_pHW->GetFirmwareBuild());
	m_FirmwareBuildEdit.SetWindowText(s);

	return TRUE;
}

//***************************************************************************************

void CFirmwareToolDlg::OnBrowseButtonClicked()
{
	CFileDialog* pDlg = new CFileDialog(TRUE, _T("ldr"), NULL, OFN_HIDEREADONLY, _T("DSP loader files (*.ldr)|*.ldr||"), this);
	pDlg->m_ofn.lpstrInitialDir = MAIN_DIR;
	if (pDlg->DoModal() == IDOK)
	{
		m_FirmwareFileEdit.SetWindowText(pDlg->GetPathName());
	}
	delete pDlg;
}

//***************************************************************************************

void CFirmwareToolDlg::OnCompareButtonClicked()
{
	CString FileName;
	m_FirmwareFileEdit.GetWindowText(FileName);
	if (FileName.IsEmpty())
	{
		::Warning("Please select a file first.");
		return;
	}
	m_pHW->CompareFirmware(FileName);
}

//***************************************************************************************

void CFirmwareToolDlg::OnUpdateButtonClicked()
{
	CString FileName;
	m_FirmwareFileEdit.GetWindowText(FileName);
	if (FileName.IsEmpty())
	{
		::Warning("Please select a file first.");
		return;
	}

	CBusyCursor Cursor;

	m_pHW->UpdateFirmware(FileName);
}

//***************************************************************************************

void CFirmwareToolDlg::OnCancel()
{
	CDialog::OnCancel();
}

//***************************************************************************************

void CFirmwareToolDlg::OnOK()
{
	CDialog::OnOK();
}

//***************************************************************************************
