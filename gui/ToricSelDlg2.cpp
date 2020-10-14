//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "ToricSelDlg2.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CToricSelDlg2, CDialog)

	ON_BN_CLICKED(IDC_RADIO1, OnFileRadioClicked)
	ON_BN_CLICKED(IDC_RADIO2, OnZipRadioClicked)
	ON_BN_CLICKED(IDC_RADIO3, OnBothRadioClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CToricSelDlg2::CToricSelDlg2(CWnd* pParentWnd) : CDialog(IDD_TORIC_DLG2, pParentWnd)//
{
}

//***************************************************************************************

void CToricSelDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_RADIO1, m_SaveToFile_Radio);
	DDX_Control(pDX, IDC_RADIO2, m_SaveToZip_Radio);
	DDX_Control(pDX, IDC_RADIO3, m_SaveToBoth_Radio);
	DDX_Control(pDX, IDOK, m_OKButton);

}

//***************************************************************************************

BOOL CToricSelDlg2::OnInitDialog()
{
	CDialog::OnInitDialog();

	CenterWindow();

	m_SaveToFile_Radio.SetCheck(1);
	m_SaveToFile = TRUE;
	m_SaveToZip = FALSE;
	m_SaveToBoth = FALSE;

	return FALSE;
}

//***************************************************************************************
void CToricSelDlg2::OnFileRadioClicked()
{
	m_SaveToFile_Radio.SetCheck(1);
	//m_SaveToZip_Radio.SetCheck(0);
	//m_SaveToBoth_Radio.SetCheck(0);
	m_SaveToFile = TRUE;
	m_SaveToZip = FALSE;
	m_SaveToBoth = FALSE;
}

//***************************************************************************************
void CToricSelDlg2::OnZipRadioClicked()
{
	m_SaveToZip_Radio.SetCheck(1);
	m_SaveToZip = TRUE;
	m_SaveToFile = FALSE;
	m_SaveToBoth = FALSE;
}

//***************************************************************************************
void CToricSelDlg2::OnBothRadioClicked()
{
	m_SaveToBoth_Radio.SetCheck(1);
	m_SaveToBoth = TRUE;
	m_SaveToZip = FALSE;
	m_SaveToFile = FALSE;
}


