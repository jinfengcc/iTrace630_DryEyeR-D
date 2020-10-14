//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "ICLDataDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CICLDataDlg, CDialog)

	ON_EN_CHANGE(IDC_ICL_CYL_EDIT, OnICLCylEditChanged)
	ON_EN_CHANGE(IDC_ICL_AXIS_EDIT, OnICLAxisEditChanged)
	ON_EN_CHANGE(IDC_ICL_PLACEMENT_AXIS_EDIT, OnICLPlacementAxisEditChanged)

END_MESSAGE_MAP()

//***************************************************************************************

CICLDataDlg::CICLDataDlg(CWnd* pParentWnd, COpData* pOpData) :
	CDialog(IDD_ICL_DATA_DLG, pParentWnd)
{
	m_pOpData = pOpData;
	m_OpData = *pOpData;

	m_CanChange = FALSE;

	m_ICLCyl_DontChange = FALSE;
	m_ICLAxis_DontChange = FALSE;
	m_ICLPlacementAxis_DontChange = FALSE;
}

//***************************************************************************************

void CICLDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ICL_CYL_EDIT, m_ICLCylEdit);
	DDX_Control(pDX, IDC_ICL_AXIS_EDIT, m_ICLAxisEdit);

	DDX_Control(pDX, IDC_ICL_PLACEMENT_AXIS_EDIT, m_ICLPlacementAxisEdit);

	DDX_Control(pDX, IDC_ICL_INDUCED_CYL_EDIT, m_ICLInducedCylEdit);
	DDX_Control(pDX, IDC_ICL_INDUCED_AXIS_EDIT, m_ICLInducedAxisEdit);
}

//***************************************************************************************

void CICLDataDlg::SetDlgData()
{
	m_CanChange = FALSE;

	CString s;

	if (!m_ICLCyl_DontChange) {
		s.Empty();
		if (m_OpData.m_ICL.m_Cyl != INVALID_VALUE) s.Format(_T("%.2f"), m_OpData.m_ICL.m_Cyl);
		m_ICLCylEdit.SetWindowText(s);
	}

	if (!m_ICLAxis_DontChange) {
		s.Empty();
		if (m_OpData.m_ICL.m_Axis != INVALID_VALUE) s.Format(_T("%i"), m_OpData.m_ICL.m_Axis);
		m_ICLAxisEdit.SetWindowText(s);
	}

	if (!m_ICLPlacementAxis_DontChange) {
		s.Empty();
		if (m_OpData.m_ICL.m_PlacementAxis != INVALID_VALUE) s.Format(_T("%i"), m_OpData.m_ICL.m_PlacementAxis);
		m_ICLPlacementAxisEdit.SetWindowText(s);
	}

	s.Empty();
	if (m_OpData.m_ICL.m_InducedCyl != INVALID_VALUE) s.Format(_T("%.2f"), m_OpData.m_ICL.m_InducedCyl);
	m_ICLInducedCylEdit.SetWindowText(s);

	s.Empty();
	if (m_OpData.m_ICL.m_InducedAxis != INVALID_VALUE) s.Format(_T("%i"), m_OpData.m_ICL.m_InducedAxis);
	m_ICLInducedAxisEdit.SetWindowText(s);

	m_CanChange = TRUE;
}

//***************************************************************************************

void CICLDataDlg::GetDlgData()
{
	CString s;

	m_ICLCylEdit.GetWindowText(s);
	if (s.IsEmpty())
	{
		m_OpData.m_ICL.m_Cyl = INVALID_VALUE;
	}
	else
	{
		G_As = s;
		m_OpData.m_ICL.m_Cyl = atof(G_As);
	}


	m_ICLAxisEdit.GetWindowText(s);
	if (s.IsEmpty())
	{
		m_OpData.m_ICL.m_Axis = INVALID_VALUE;
	}
	else
	{
		G_As = s;
		m_OpData.m_ICL.m_Axis = atoi(G_As);
	}

	m_ICLPlacementAxisEdit.GetWindowText(s);
	if (s.IsEmpty())
	{
		m_OpData.m_ICL.m_PlacementAxis = INVALID_VALUE;
	}
	else
	{
		G_As = s;
		m_OpData.m_ICL.m_PlacementAxis = atoi(G_As);
	}
}

//***************************************************************************************

BOOL CICLDataDlg::ValidateData()
{
	if (m_OpData.m_ICL.m_Axis == INVALID_VALUE)
	{
		::Error("Please enter ICL's cylinder axis.");
		return FALSE;
	}

	if (m_OpData.m_ICL.m_Axis < 0 || m_OpData.m_ICL.m_Axis >= 180) {
		::Error("ICL's cylinder axis must be between 0 and 179 degrees.");
		return FALSE;
	}

	if (m_OpData.m_ICL.m_PlacementAxis == INVALID_VALUE) {
		::Error("Please enter ICL's placement axis.");
		return FALSE;
	}

	if (m_OpData.m_ICL.m_PlacementAxis < 0 || m_OpData.m_ICL.m_PlacementAxis >= 180) {
		::Error("ICL's placement axis must be between 0 and 179 degrees.");
		return FALSE;
	}

	return TRUE;
}

//***************************************************************************************

BOOL CICLDataDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetDlgData();

	return FALSE;
}

//***************************************************************************************

void CICLDataDlg::OnOK()
{
	GetDlgData();

	if (!ValidateData()) return;

	*m_pOpData = m_OpData;

	CDialog::OnOK();
}

//***************************************************************************************

void CICLDataDlg::OnICLCylEditChanged()
{
	if (!m_CanChange) return;

	GetDlgData();

	m_OpData.Recalculate();

	m_ICLCyl_DontChange = TRUE;

	SetDlgData();

	m_ICLCyl_DontChange = FALSE;
}

//***************************************************************************************

void CICLDataDlg::OnICLAxisEditChanged()
{
	if (!m_CanChange) return;

	GetDlgData();

	m_OpData.m_ICL.m_PlacementAxis = INVALID_VALUE;

	m_OpData.Recalculate();

	m_ICLAxis_DontChange = TRUE;

	SetDlgData();

	m_ICLAxis_DontChange = FALSE;
}

//***************************************************************************************

void CICLDataDlg::OnICLPlacementAxisEditChanged()
{
	if (!m_CanChange) return;

	GetDlgData();

	m_OpData.Recalculate();

	m_ICLPlacementAxis_DontChange = TRUE;

	SetDlgData();

	m_ICLPlacementAxis_DontChange = FALSE;
}

//***************************************************************************************
