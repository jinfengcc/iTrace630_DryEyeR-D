//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "EyeDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

CEyeDlg::CEyeDlg(CWnd* pParentWnd, CEye* pEye) : CDialog(IDD_EYE_DLG, pParentWnd)
{
	m_pEye = pEye;
}

//***************************************************************************************

void CEyeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_VERTEX_EDIT, m_VertexEdit);
	DDX_Control(pDX, IDC_SPH_EDIT, m_SphEdit);
	DDX_Control(pDX, IDC_CYL_EDIT, m_CylEdit);
	DDX_Control(pDX, IDC_AXIS_EDIT, m_AxisEdit);
	DDX_Control(pDX, IDC_K1_EDIT, m_K1Edit);
	DDX_Control(pDX, IDC_K2_EDIT, m_K2Edit);
	DDX_Control(pDX, IDC_K2_AXIS_EDIT, m_K2AxisEdit);
}

//***************************************************************************************

BOOL CEyeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString s;

	s.Format(_T("%.2f"), m_pEye->m_ManifestVertex);
	m_VertexEdit.SetWindowText(s);

	s.Format(_T("%.2f"), m_pEye->m_ManifestSphere);
	m_SphEdit.SetWindowText(s);

	s.Format(_T("%.2f"), m_pEye->m_ManifestCylinder);
	m_CylEdit.SetWindowText(s);

	s.Format(_T("%i"), m_pEye->m_ManifestAxis);
	m_AxisEdit.SetWindowText(s);

	s.Format(_T("%.2f"), m_pEye->m_K1);
	m_K1Edit.SetWindowText(s);

	s.Format(_T("%.2f"), m_pEye->m_K2);
	m_K2Edit.SetWindowText(s);

	s.Format(_T("%i"), m_pEye->m_K2Axis);
	m_K2AxisEdit.SetWindowText(s);

	SetWindowText(ISOD(m_pEye->m_Type) ? _T("OD") : _T("OS"));

	GetDlgItem(IDOK)->EnableWindow(::DB.IsReadOnly() ? FALSE : TRUE);

	return TRUE;
}

//***************************************************************************************

void CEyeDlg::OnOK()
{
	CString s;

	m_VertexEdit.GetWindowText(s);
	G_As = s;
	m_pEye->m_ManifestVertex = atof(G_As);

	m_SphEdit.GetWindowText(s);
	G_As = s;
	m_pEye->m_ManifestSphere = atof(G_As);

	m_CylEdit.GetWindowText(s);
	G_As = s;
	m_pEye->m_ManifestCylinder = atof(G_As);

	m_AxisEdit.GetWindowText(s);
	G_As = s;
	m_pEye->m_ManifestAxis = atoi(G_As);

	m_K1Edit.GetWindowText(s);
	G_As = s;
	m_pEye->m_K1 = atof(G_As);

	m_K2Edit.GetWindowText(s);
	G_As = s;
	m_pEye->m_K2 = atof(G_As);

	m_K2AxisEdit.GetWindowText(s);
	G_As = s;
	m_pEye->m_K2Axis = atoi(G_As);

	CDialog::OnOK();
}

//***************************************************************************************
