//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "RegtimeDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

CRegtimeDlg::CRegtimeDlg(CWnd* pParentWnd, CString* RegFromTime, CString* RegToTime) : CDialog(IDD_REGISTRATION_TIME_DLG, pParentWnd)
{
	m_RegFromTime = RegFromTime;
	m_RegToTime = RegToTime;
}

//***************************************************************************************

void CRegtimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_DATETIMEFROM, m_FromEdit);
	DDX_Control(pDX, IDC_DATETIMETO, m_ToEdit);
	DDX_Control(pDX, IDOK, m_OKButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

//***************************************************************************************

BOOL CRegtimeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;
}

//***************************************************************************************


//***************************************************************************************

void CRegtimeDlg::OnOK()
{
	CString from, to;

	m_FromEdit.GetWindowText(*m_RegFromTime);
	m_ToEdit.GetWindowText(*m_RegToTime);
	//m_RegFromTime = &from;
	//m_RegToTime = &to;
	CDialog::OnOK();
}

//***************************************************************************************

