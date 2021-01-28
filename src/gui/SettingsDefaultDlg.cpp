//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "SettingsDefaultDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CSettingsDefaultDlg, CDialog)

	ON_BN_CLICKED(IDC_CONTROL_DEF_CHECK, OnCtlDefCheckClicked)
	ON_BN_CLICKED(IDC_DISPLAY_DEF_CHECK, OnGUIDefCheckClicked)
	ON_BN_CLICKED(IDC_EXCEL_DEF_CHECK, OnExcDefCheckClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CSettingsDefaultDlg::CSettingsDefaultDlg(CWnd* pParentWnd) : CDialog(IDD_SETTINGS_DEF_DLG, pParentWnd)
{

}

//***************************************************************************************

void CSettingsDefaultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CONTROL_DEF_CHECK, m_CtlDefCheck);
	DDX_Control(pDX, IDC_DISPLAY_DEF_CHECK, m_GUIDefCheck);
	DDX_Control(pDX, IDC_EXCEL_DEF_CHECK, m_ExcDefCheck);
}

//***************************************************************************************

BOOL CSettingsDefaultDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CenterWindow();

	m_CtlDef = TRUE;
	m_GUIDef = FALSE;
	m_ExcDef = FALSE;

	m_CtlDefCheck.SetCheck(m_CtlDef);
	m_GUIDefCheck.SetCheck(m_GUIDef);
	m_ExcDefCheck.SetCheck(m_ExcDef);

	if (m_isSolo) m_ExcDefCheck.ShowWindow(SW_HIDE);

	return FALSE;
}

//***************************************************************************************

void CSettingsDefaultDlg::OnCtlDefCheckClicked()
{
	if (!m_CtlDefCheck.GetCheck())
		m_CtlDef = FALSE;
	else m_CtlDef = TRUE;

	m_CtlDefCheck.SetCheck(m_CtlDef);
}

//***************************************************************************************

void CSettingsDefaultDlg::OnGUIDefCheckClicked()
{
	if (!m_GUIDefCheck.GetCheck())
		m_GUIDef = FALSE;
	else m_GUIDef = TRUE;

	m_GUIDefCheck.SetCheck(m_GUIDef);
}

//***************************************************************************************

void CSettingsDefaultDlg::OnExcDefCheckClicked()
{
	if (!m_ExcDefCheck.GetCheck())
		m_ExcDef = FALSE;
	else m_ExcDef = TRUE;

	m_ExcDefCheck.SetCheck(m_ExcDef);
}

//***************************************************************************************
