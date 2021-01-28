/***************************************************************************************

Autor:         Jinfeng Chen
E-mail:        jinfengcc@gmail.com
Date:          11-14-2016
Description:   Create a dialog for the source of import patient selection.
Since customer has their MWL system for DICOM import
verison:       6.2.0

***************************************************************************************/

//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "DicomSelTypeDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CDicomSelTypeDlg, CDialog)

	ON_BN_CLICKED(IDC_IMPORT_FILE_RADIO, OnFielRadioClicked)
	ON_BN_CLICKED(IDC_IMPORT_MWL_RADIO, OnMWLRadioClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CDicomSelTypeDlg::CDicomSelTypeDlg(CWnd* pParentWnd) : CDialog(IDD_DICOMSELTYPEDLG, pParentWnd)
{

}

//***************************************************************************************

void CDicomSelTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_IMPORT_FILE_RADIO, m_FileRadio);
	DDX_Control(pDX, IDC_IMPORT_MWL_RADIO, m_MWLRadio);
}

//***************************************************************************************

BOOL CDicomSelTypeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CenterWindow();

	m_FileRadioSel = TRUE;

	m_FileRadio.SetCheck(m_FileRadioSel);
	m_MWLRadio.SetCheck(!m_FileRadioSel);

	return FALSE;
}

//***************************************************************************************

void CDicomSelTypeDlg::OnFielRadioClicked()
{
	if (!m_FileRadio.GetCheck())
		m_FileRadioSel = FALSE;
	else m_FileRadioSel = TRUE;
}

//***************************************************************************************

void CDicomSelTypeDlg::OnMWLRadioClicked()
{
	if (!m_MWLRadio.GetCheck())
		m_FileRadioSel = TRUE;
	else m_FileRadioSel = FALSE;
}

//***************************************************************************************
