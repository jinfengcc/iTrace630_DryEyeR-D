//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "RefSurfaceDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

CRefSurfaceDlg::CRefSurfaceDlg(CWnd* pParentWnd, real ax0_um, real q) :
	CDialog(IDD_REF_SURFACE_DLG, pParentWnd)
{
	m_ax0_um = ax0_um;
	m_q = q;
}

//***************************************************************************************

void CRefSurfaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CENTRAL_RADIUS_EDIT, m_CentralRadiusEdit);
	DDX_Control(pDX, IDC_ASPHERICITY_EDIT, m_AsphericityEdit);
}

//***************************************************************************************

BOOL CRefSurfaceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString s;
	s.Format(_T("%.2f"), 0.001 * m_ax0_um);
	m_CentralRadiusEdit.SetWindowText(s);

	s.Format(_T("%.2f"), m_q);
	m_AsphericityEdit.SetWindowText(s);

	m_CentralRadiusEdit.SetFocus();

	return FALSE;
}

//***************************************************************************************

void CRefSurfaceDlg::OnOK()
{
	CString s;
	m_CentralRadiusEdit.GetWindowText(s);
	if (s == "")
	{
		::Warning("Enter a value for central radius.");
		m_CentralRadiusEdit.SetFocus();
		return;
	}

	G_As = s;
	real ax0_mm = atof(G_As);

	if (ax0_mm < 5.0 || ax0_mm > 10.0)
	{
		::Warning("Enter a reasonable value for central radius.");
		m_CentralRadiusEdit.SetFocus();
		return;
	}

	m_AsphericityEdit.GetWindowText(s);
	if (s == "")
	{
		::Warning("Enter a value for asphericity.");
		m_AsphericityEdit.SetFocus();
		return;
	}

	G_As = s;
	real q = atof(G_As);
	if (q < -2.0 || q > 2.0)
	{
		::Warning("Enter a reasonable value for asphericity.");
		m_AsphericityEdit.SetFocus();
		return;
	}

	m_ax0_um = 1000.0 * ax0_mm;
	m_q = q;

	CDialog::OnOK();
}

//***************************************************************************************
