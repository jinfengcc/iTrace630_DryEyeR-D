//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "PreopDataDlg.h"
#include "ICLDataDlg.h"
#include "Settings.h"//[cjf***04192012]

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

int angular_distance_counterclockwise1(int a1, int a2)
{
	int da = (a2 - a1) % 360;
	if (da < 0) da += 360;
	return da;
}

// distance between a1 and a2 ª [0, 180)
int angular_distance1(int a1, int a2)
{
	int da = angular_distance_counterclockwise1(a1, a2);
	if (da > 180) da = 360 - da;
	return da;
}
//***************************************************************************************

BEGIN_MESSAGE_MAP(CPreopDataDlg, CDialog)

	ON_WM_PAINT()

	ON_BN_CLICKED(IDC_OP1_RADIO, OnOp1RadioClicked)
	ON_BN_CLICKED(IDC_OP2_RADIO, OnOp2RadioClicked)
	ON_BN_CLICKED(IDC_OP3_RADIO, OnOp3RadioClicked)
	// PRE-OP
	ON_EN_CHANGE(IDC_PREOP_CORNEA_CYL_EDIT, OnPreopCorneaCylEditChanged)
	ON_EN_CHANGE(IDC_PREOP_CORNEA_AXIS_EDIT, OnPreopCorneaAxisEditChanged)
	ON_EN_CHANGE(IDC_PREOP_ENTIRE_EYE_AXIS_EDIT, OnPreopEntireEyeAxisEditChanged)
	ON_EN_CHANGE(IDC_PREOP_ENTIRE_EYE_CYL_EDIT, OnPreopEntireEyeCylEditChanged)
	ON_BN_CLICKED(IDC_PREOP_CORNEA_WF_CHECK, OnPreopCorneaWFCheckClicked)
	ON_BN_CLICKED(IDC_PREOP_CORNEA_SIMK_CHECK, OnPreopCorneaSimKCheckClicked)
	ON_BN_CLICKED(IDC_PREOP_ENTIRE_EYE_WF_CHECK, OnPreopEntireEyeWFCheckClicked)
	// IOL
	ON_EN_CHANGE(IDC_INCISION_INDUCED_CYL_EDIT, OnIncisionInducedCylEditChanged)
	ON_EN_CHANGE(IDC_INCISION_AXIS_EDIT, OnIncisionAxisEditChanged)
	ON_EN_CHANGE(IDC_IOL_PLACEMENT_AXIS_EDIT, OnIOLAxisEditChanged)
	ON_BN_CLICKED(IDC_IOL1_RADIO, OnIOL1RadioClicked)
	ON_BN_CLICKED(IDC_IOL2_RADIO, OnIOL2RadioClicked)
	ON_BN_CLICKED(IDC_IOL3_RADIO, OnIOL3RadioClicked)
	ON_BN_CLICKED(IDC_IOL4_RADIO, OnIOL4RadioClicked)
	ON_BN_CLICKED(IDC_IOL5_RADIO, OnIOL5RadioClicked)
	ON_BN_CLICKED(IDC_IOL6_RADIO, OnIOL6RadioClicked)
	ON_BN_CLICKED(IDC_IOL7_RADIO, OnIOL7RadioClicked)
	ON_BN_CLICKED(IDC_IOL8_RADIO, OnIOL8RadioClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CPreopDataDlg::CPreopDataDlg(CWnd* pParentWnd, COpData* pOpData, int OD) :
	CDialog(IDD_PREOP_DATA_DLG, pParentWnd)
{
	m_OD = OD;//[Cjf***04192012] 

	m_pOpData = pOpData;
	m_OpData = *pOpData;

	m_CanChange = FALSE;

	m_PreopCorneaCyl_DontChange = FALSE;
	m_PreopCorneaAxis_DontChange = FALSE;
	m_PreopEntireEyeCyl_DontChange = FALSE;
	m_PreopEntireEyeAxis_DontChange = FALSE;
	m_IncisionAxis_DontChange = FALSE;
	m_IncisionInducedCyl_DontChange = FALSE;
	m_IOLAxis_DontChange = FALSE;
}

//***************************************************************************************

void CPreopDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_OP1_RADIO, m_OpRadio[0]);
	DDX_Control(pDX, IDC_OP2_RADIO, m_OpRadio[1]);
	DDX_Control(pDX, IDC_OP3_RADIO, m_OpRadio[2]);

	// Pre-op

	DDX_Control(pDX, IDC_PREOP_CORNEA_CYL_EDIT, m_PreopCorneaCylEdit);
	DDX_Control(pDX, IDC_PREOP_CORNEA_AXIS_EDIT, m_PreopCorneaAxisEdit);
	DDX_Control(pDX, IDC_PREOP_CORNEA_WF_CHECK, m_PreopCorneaWFCheck);
	DDX_Control(pDX, IDC_PREOP_CORNEA_SIMK_CHECK, m_PreopCorneaSimKCheck);

	DDX_Control(pDX, IDC_PREOP_INTERNAL_CYL_EDIT, m_PreopInternalCylEdit);
	DDX_Control(pDX, IDC_PREOP_INTERNAL_AXIS_EDIT, m_PreopInternalAxisEdit);

	DDX_Control(pDX, IDC_PREOP_ENTIRE_EYE_CYL_EDIT, m_PreopEntireEyeCylEdit);
	DDX_Control(pDX, IDC_PREOP_ENTIRE_EYE_AXIS_EDIT, m_PreopEntireEyeAxisEdit);
	DDX_Control(pDX, IDC_PREOP_ENTIRE_EYE_WF_CHECK, m_PreopEntireEyeWFCheck);

	// IOL

	DDX_Control(pDX, IDC_INCISION_AXIS_EDIT, m_IncisionAxisEdit);
	DDX_Control(pDX, IDC_INCISION_INDUCED_CYL_EDIT, m_IncisionInducedCylEdit);
	DDX_Control(pDX, IDC_INCISION_INDUCED_AXIS_EDIT, m_IncisionInducedAxisEdit);

	DDX_Control(pDX, IDC_IOL_SPHERE_COMBO, m_IOLSphereComboBox);
	DDX_Control(pDX, IDC_IOL_PLACEMENT_AXIS_EDIT, m_IOLPlacementAxisEdit);

	DDX_Control(pDX, IDC_IOL1_RADIO, m_IOLRadio[0]);
	DDX_Control(pDX, IDC_IOL2_RADIO, m_IOLRadio[1]);
	DDX_Control(pDX, IDC_IOL3_RADIO, m_IOLRadio[2]);
	DDX_Control(pDX, IDC_IOL4_RADIO, m_IOLRadio[3]);
	DDX_Control(pDX, IDC_IOL5_RADIO, m_IOLRadio[4]);
	DDX_Control(pDX, IDC_IOL6_RADIO, m_IOLRadio[5]);
	DDX_Control(pDX, IDC_IOL7_RADIO, m_IOLRadio[6]);
	DDX_Control(pDX, IDC_IOL8_RADIO, m_IOLRadio[7]);

	DDX_Control(pDX, IDC_IOL1_CYL1_EDIT, m_IOLCylEdit[0][0]);
	DDX_Control(pDX, IDC_IOL2_CYL1_EDIT, m_IOLCylEdit[1][0]);
	DDX_Control(pDX, IDC_IOL3_CYL1_EDIT, m_IOLCylEdit[2][0]);
	DDX_Control(pDX, IDC_IOL4_CYL1_EDIT, m_IOLCylEdit[3][0]);
	DDX_Control(pDX, IDC_IOL5_CYL1_EDIT, m_IOLCylEdit[4][0]);
	DDX_Control(pDX, IDC_IOL6_CYL1_EDIT, m_IOLCylEdit[5][0]);
	DDX_Control(pDX, IDC_IOL7_CYL1_EDIT, m_IOLCylEdit[6][0]);
	DDX_Control(pDX, IDC_IOL8_CYL1_EDIT, m_IOLCylEdit[7][0]);

	DDX_Control(pDX, IDC_IOL1_CYL2_EDIT, m_IOLCylEdit[0][1]);
	DDX_Control(pDX, IDC_IOL2_CYL2_EDIT, m_IOLCylEdit[1][1]);
	DDX_Control(pDX, IDC_IOL3_CYL2_EDIT, m_IOLCylEdit[2][1]);
	DDX_Control(pDX, IDC_IOL4_CYL2_EDIT, m_IOLCylEdit[3][1]);
	DDX_Control(pDX, IDC_IOL5_CYL2_EDIT, m_IOLCylEdit[4][1]);
	DDX_Control(pDX, IDC_IOL6_CYL2_EDIT, m_IOLCylEdit[5][1]);
	DDX_Control(pDX, IDC_IOL7_CYL2_EDIT, m_IOLCylEdit[6][1]);
	DDX_Control(pDX, IDC_IOL8_CYL2_EDIT, m_IOLCylEdit[7][1]);

	DDX_Control(pDX, IDC_IOL1_AXIS_EDIT, m_IOLAxisEdit[0]);
	DDX_Control(pDX, IDC_IOL2_AXIS_EDIT, m_IOLAxisEdit[1]);
	DDX_Control(pDX, IDC_IOL3_AXIS_EDIT, m_IOLAxisEdit[2]);
	DDX_Control(pDX, IDC_IOL4_AXIS_EDIT, m_IOLAxisEdit[3]);
	DDX_Control(pDX, IDC_IOL5_AXIS_EDIT, m_IOLAxisEdit[4]);
	DDX_Control(pDX, IDC_IOL6_AXIS_EDIT, m_IOLAxisEdit[5]);
	DDX_Control(pDX, IDC_IOL7_AXIS_EDIT, m_IOLAxisEdit[6]);
	DDX_Control(pDX, IDC_IOL8_AXIS_EDIT, m_IOLAxisEdit[7]);

	DDX_Control(pDX, IDC_IOL1_POSTOP_EYE_CYL_EDIT, m_IOLPostopEyeCylEdit[0]);
	DDX_Control(pDX, IDC_IOL2_POSTOP_EYE_CYL_EDIT, m_IOLPostopEyeCylEdit[1]);
	DDX_Control(pDX, IDC_IOL3_POSTOP_EYE_CYL_EDIT, m_IOLPostopEyeCylEdit[2]);
	DDX_Control(pDX, IDC_IOL4_POSTOP_EYE_CYL_EDIT, m_IOLPostopEyeCylEdit[3]);
	DDX_Control(pDX, IDC_IOL5_POSTOP_EYE_CYL_EDIT, m_IOLPostopEyeCylEdit[4]);
	DDX_Control(pDX, IDC_IOL6_POSTOP_EYE_CYL_EDIT, m_IOLPostopEyeCylEdit[5]);
	DDX_Control(pDX, IDC_IOL7_POSTOP_EYE_CYL_EDIT, m_IOLPostopEyeCylEdit[6]);
	DDX_Control(pDX, IDC_IOL8_POSTOP_EYE_CYL_EDIT, m_IOLPostopEyeCylEdit[7]);

	DDX_Control(pDX, IDC_IOL1_POSTOP_EYE_AXIS_EDIT, m_IOLPostopEyeAxisEdit[0]);
	DDX_Control(pDX, IDC_IOL2_POSTOP_EYE_AXIS_EDIT, m_IOLPostopEyeAxisEdit[1]);
	DDX_Control(pDX, IDC_IOL3_POSTOP_EYE_AXIS_EDIT, m_IOLPostopEyeAxisEdit[2]);
	DDX_Control(pDX, IDC_IOL4_POSTOP_EYE_AXIS_EDIT, m_IOLPostopEyeAxisEdit[3]);
	DDX_Control(pDX, IDC_IOL5_POSTOP_EYE_AXIS_EDIT, m_IOLPostopEyeAxisEdit[4]);
	DDX_Control(pDX, IDC_IOL6_POSTOP_EYE_AXIS_EDIT, m_IOLPostopEyeAxisEdit[5]);
	DDX_Control(pDX, IDC_IOL7_POSTOP_EYE_AXIS_EDIT, m_IOLPostopEyeAxisEdit[6]);
	DDX_Control(pDX, IDC_IOL8_POSTOP_EYE_AXIS_EDIT, m_IOLPostopEyeAxisEdit[7]);

	// Post-op

	DDX_Control(pDX, IDC_POSTOP_CORNEA_CYL_EDIT, m_PostopCorneaCylEdit);
	DDX_Control(pDX, IDC_POSTOP_CORNEA_AXIS_EDIT, m_PostopCorneaAxisEdit);

	DDX_Control(pDX, IDC_POSTOP_INTERNAL_CYL_EDIT, m_PostopInternalCylEdit);
	DDX_Control(pDX, IDC_POSTOP_INTERNAL_AXIS_EDIT, m_PostopInternalAxisEdit);

	DDX_Control(pDX, IDC_POSTOP_ENTIRE_EYE_CYL_EDIT, m_PostopEntireEyeCylEdit);
	DDX_Control(pDX, IDC_POSTOP_ENTIRE_EYE_AXIS_EDIT, m_PostopEntireEyeAxisEdit);

	//520
	DDX_Control(pDX, IDC_STATIC_INFO1, m_TraceyIOLInfo1);
	DDX_Control(pDX, IDC_STATIC_INFO2, m_TraceyIOLInfo2);
	//520
}

//***************************************************************************************

void CPreopDataDlg::SetDlgData()
{
	m_CanChange = FALSE;

	CString s;

	m_TraceyIOLInfo1.ShowWindow(SW_HIDE);//520
	m_TraceyIOLInfo2.ShowWindow(SW_HIDE);//520
	switch (m_OpData.m_OpType) {
	case 1: m_OpRadio[1].SetCheck(1); break;
	case 2: m_OpRadio[2].SetCheck(1); break;
	default: m_OpData.m_OpType = 0; m_OpRadio[0].SetCheck(1);
	}

	// Pre-op corneal astigmatism

	if (!m_PreopCorneaCyl_DontChange) {
		s.Empty();
		if (m_OpData.m_CorneaPreopCyl != INVALID_VALUE) s.Format(_T("%.2f"), m_OpData.m_CorneaPreopCyl);
		m_PreopCorneaCylEdit.SetWindowText(s);
	}

	if (!m_PreopCorneaAxis_DontChange) {
		s.Empty();
		if (m_OpData.m_CorneaPreopAxis != INVALID_VALUE) s.Format(_T("%i"), m_OpData.m_CorneaPreopAxis);
		m_PreopCorneaAxisEdit.SetWindowText(s);
	}

	m_PreopCorneaWFCheck.SetCheck(
		m_OpData.m_CorneaPreopCyl != INVALID_VALUE && fabs(m_OpData.m_CorneaPreopCyl - m_OpData.m_CorneaPreopCylWF) < 0.001 &&
		m_OpData.m_CorneaPreopAxis != INVALID_VALUE && m_OpData.m_CorneaPreopAxis == m_OpData.m_CorneaPreopAxisWF);

	m_PreopCorneaSimKCheck.SetCheck(
		m_OpData.m_CorneaPreopCyl != INVALID_VALUE && fabs(m_OpData.m_CorneaPreopCyl - m_OpData.m_CorneaPreopCylSimK) < 0.01 &&
		m_OpData.m_CorneaPreopAxis != INVALID_VALUE && m_OpData.m_CorneaPreopAxis == m_OpData.m_CorneaPreopAxisSimK);//[5.1.1 0.001 -> 0.01

																													 // Pre-op internal optics astigmatism

	s.Empty();
	if (m_OpData.m_InternalPreopCyl != INVALID_VALUE) s.Format(_T("%.2f"), m_OpData.m_InternalPreopCyl);
	m_PreopInternalCylEdit.SetWindowText(s);

	s.Empty();
	if (m_OpData.m_InternalPreopAxis != INVALID_VALUE) s.Format(_T("%i"), m_OpData.m_InternalPreopAxis);
	m_PreopInternalAxisEdit.SetWindowText(s);

	// Pre-op entire eye astigmatism

	if (!m_PreopEntireEyeCyl_DontChange) {
		s.Empty();
		if (m_OpData.m_EyePreopCyl != INVALID_VALUE) s.Format(_T("%.2f"), m_OpData.m_EyePreopCyl);
		m_PreopEntireEyeCylEdit.SetWindowText(s);
	}

	if (!m_PreopEntireEyeAxis_DontChange) {
		s.Empty();
		if (m_OpData.m_EyePreopAxis != INVALID_VALUE) s.Format(_T("%i"), m_OpData.m_EyePreopAxis);
		m_PreopEntireEyeAxisEdit.SetWindowText(s);
	}

	m_PreopEntireEyeWFCheck.SetCheck(
		m_OpData.m_EyePreopCyl != INVALID_VALUE && fabs(m_OpData.m_EyePreopCyl - m_OpData.m_EyePreopCylWF) < 0.001 &&
		m_OpData.m_EyePreopAxis != INVALID_VALUE && m_OpData.m_EyePreopAxis == m_OpData.m_EyePreopAxisWF);

	// Surgery incision axis

	m_IncisionAxisEdit.SetReadOnly(m_OpData.m_OpType != 0 && m_OpData.m_OpType != 2);
	if (!m_IncisionAxis_DontChange) {
		s.Empty();
		if (m_OpData.m_CorneaIncisionAxis != INVALID_VALUE) s.Format(_T("%i"), m_OpData.m_CorneaIncisionAxis);
		m_IncisionAxisEdit.SetWindowText(s);
	}

	// Surgically induced corneal astigmatism

	m_IncisionInducedCylEdit.SetReadOnly(m_OpData.m_OpType != 0 && m_OpData.m_OpType != 2);
	if (!m_IncisionInducedCyl_DontChange) {
		s.Empty();
		if (m_OpData.m_CorneaInducedCyl != INVALID_VALUE) s.Format(_T("%.2f"), m_OpData.m_CorneaInducedCyl);
		m_IncisionInducedCylEdit.SetWindowText(s);
	}

	s.Empty();
	if (m_OpData.m_CorneaInducedAxis != INVALID_VALUE) s.Format(_T("%i"), m_OpData.m_CorneaInducedAxis);
	m_IncisionInducedAxisEdit.SetWindowText(s);

	// IOL sphere

	m_IOLSphereComboBox.ResetContent();
	m_IOLSphereComboBox.EnableWindow(m_OpData.m_OpType == 0);
	if (m_OpData.m_OpType == 0) {
		int CurSel = -1;
		for (int i = 0; i <= 40; i++) {
			real_t IOLSph = 10.0 + 0.5 * i;
			if (i & 1) s.Format(_T("%.1f"), IOLSph); else s.Format(_T("%i"), intRound(IOLSph));
			m_IOLSphereComboBox.AddString(s);
			if (m_OpData.m_IOLSph != INVALID_VALUE) {
				if (fabs(m_OpData.m_IOLSph - IOLSph) < 0.001) {
					CurSel = i;
				}
			}
		}
		if (CurSel != -1) m_IOLSphereComboBox.SetCurSel(CurSel);
	}

	// IOL placement axis

	m_IOLPlacementAxisEdit.SetReadOnly(m_OpData.m_OpType != 0);
	if (m_OpData.m_OpType == 0) {
		if (!m_IOLAxis_DontChange) {
			s.Empty();
			if (m_OpData.m_IOLPlacementAxis != INVALID_VALUE) s.Format(_T("%i"), m_OpData.m_IOLPlacementAxis);
			m_IOLPlacementAxisEdit.SetWindowText(s);
		}
	}
	else {
		m_IOLPlacementAxisEdit.SetWindowText(_T(""));
	}

	// IOLs

	const int ID[8] = {
		IDC_IOL1_STATIC, IDC_IOL2_STATIC, IDC_IOL3_STATIC, IDC_IOL4_STATIC,
		IDC_IOL5_STATIC, IDC_IOL6_STATIC, IDC_IOL7_STATIC, IDC_IOL8_STATIC
	};

	for (int i = 0; i < 8; i++) {

		CIOL* pIOL = &m_OpData.m_IOLs[i];

		s = " " + pIOL->m_Name;
		m_IOLRadio[i].SetWindowText(s);

		s.Format(_T("%.2f"), pIOL->m_Cyl);
		m_IOLCylEdit[i][0].SetWindowText(s);

		s.Format(_T("%.2f"), pIOL->m_InternalPostopCyl);
		m_IOLCylEdit[i][1].SetWindowText(s);

		if (m_OpData.m_OpType == 0) {

			m_IOLRadio[i].EnableWindow(TRUE);

			m_IOLRadio[i].SetCheck(m_OpData.m_SelectedIOL == i);

			s.Empty();
			if (pIOL->m_InternalPostopAxis != INVALID_VALUE && fabs(pIOL->m_Cyl) > 0.001) {
				s.Format(_T("%i"), pIOL->m_InternalPostopAxis);
			}
			m_IOLAxisEdit[i].SetWindowText(s);

			s.Empty();
			if (pIOL->m_EyePostopCyl != INVALID_VALUE) {
				s.Format(_T("%.2f"), pIOL->m_EyePostopCyl);
			}
			m_IOLPostopEyeCylEdit[i].SetWindowText(s);

			s.Empty();
			if (pIOL->m_EyePostopAxis != INVALID_VALUE) {
				s.Format(_T("%i°"), pIOL->m_EyePostopAxis);
			}
			m_IOLPostopEyeAxisEdit[i].SetWindowText(s);

			s.Empty();
			if (m_OpData.m_BestIOL == i) s = "min";
			((CWnd*)GetDlgItem(ID[i]))->SetWindowText(s);
		}

		else {
			m_IOLRadio[i].EnableWindow(FALSE);
			m_IOLRadio[i].SetCheck(0);
			m_IOLAxisEdit[i].SetWindowText(_T(""));
			m_IOLPostopEyeCylEdit[i].SetWindowText(_T(""));
			m_IOLPostopEyeAxisEdit[i].SetWindowText(_T(""));
			((CWnd*)GetDlgItem(ID[i]))->SetWindowText(_T(""));
		}
	}

	// Post-op corneal astigmatism

	s.Empty();
	if (m_OpData.m_CorneaPostopCyl != INVALID_VALUE) s.Format(_T("%.2f"), m_OpData.m_CorneaPostopCyl);
	m_PostopCorneaCylEdit.SetWindowText(s);

	s.Empty();
	if (m_OpData.m_CorneaPostopAxis != INVALID_VALUE) s.Format(_T("%i"), m_OpData.m_CorneaPostopAxis);
	m_PostopCorneaAxisEdit.SetWindowText(s);

	// Post-op internal astigmatism

	s.Empty();
	if (m_OpData.m_InternalPostopCyl != INVALID_VALUE) s.Format(_T("%.2f"), m_OpData.m_InternalPostopCyl);
	m_PostopInternalCylEdit.SetWindowText(s);

	s.Empty();
	if (m_OpData.m_InternalPostopAxis != INVALID_VALUE) s.Format(_T("%i"), m_OpData.m_InternalPostopAxis);
	m_PostopInternalAxisEdit.SetWindowText(s);

	// Post-op entire eye astigmatism

	s.Empty();
	if (m_OpData.m_EyePostopCyl != INVALID_VALUE) s.Format(_T("%.2f"), m_OpData.m_EyePostopCyl);
	m_PostopEntireEyeCylEdit.SetWindowText(s);

	s.Empty();
	if (m_OpData.m_EyePostopAxis != INVALID_VALUE) s.Format(_T("%i"), m_OpData.m_EyePostopAxis);
	m_PostopEntireEyeAxisEdit.SetWindowText(s);

	m_CanChange = TRUE;

	//520, show info of IOL enhancement
	if (m_OpData.m_OpType == 1)
	{
		CString s1, s2;
		s1 = "";
		s2 = "";

		//info 1
		if ((m_OpData.m_InternalPreopAxis != INVALID_VALUE) && (m_OpData.m_IOLPlacementAxis != INVALID_VALUE))
		{
			int a[2][2];
			a[0][0] = (m_OpData.m_InternalPreopAxis + 90) % 180;
			a[0][1] = a[0][0] + 180;
			a[1][0] = m_OpData.m_IOLPlacementAxis;
			a[1][1] = a[1][0] + 180;

			if (angular_distance1(a[0][0], a[1][0]) > angular_distance1(a[0][0], a[1][1])) Swap(a[1][0], a[1][1]);

			int a1, a2;
			BOOL m_Clock_wise;
			if (angular_distance_counterclockwise1(a[0][0], a[1][0]) < 180)
			{
				a1 = a[0][0];
				a2 = a[1][0];
				m_Clock_wise = 1;
			}
			else
			{
				a1 = a[1][0];
				a2 = a[0][0];
				m_Clock_wise = 0;
			}

			CString angelStr;
			int Angel = angular_distance1(a1, a2);

			if (m_Clock_wise)
			{
				Angel = 180 - Angel;
			}

			if (Angel == 180) Angel = 0;

			angelStr.Format(_T("%i°"), Angel);

			s1 = angelStr + " rotation clockwise = ";
		}
		//Done

		//info 2
		if ((m_OpData.m_EyePreopCyl != INVALID_VALUE) && (m_OpData.m_EyePostopCyl != INVALID_VALUE))
		{
			CString DisStr;
			real_t disCylPower = (m_OpData.m_EyePreopCyl - m_OpData.m_EyePostopCyl);
			DisStr.Format(_T("%.2f"), disCylPower);

			s2 = DisStr + " D change in cylinder power.";
		}
		//Done

		m_TraceyIOLInfo1.SetWindowText(s1);
		m_TraceyIOLInfo2.SetWindowText(s2);


		m_TraceyIOLInfo1.ShowWindow(SW_SHOW);
		m_TraceyIOLInfo2.ShowWindow(SW_SHOW);
	}
	//520
}

//***************************************************************************************
//[cjf***04192012],GET the 'incision location' and 'surgically induced astigmatism'  
void CPreopDataDlg::SetIncLoC_SurInAst()
{
	/*m_IncisionAxisEdit.SetWindowText(_T("0.0");
	m_IncisionInducedCylEdit.SetWindowText(_T("0,0");*/
	CString s;
	s.Empty();
	if (m_OD == 1)
	{
		s.Format(_T("%i"), ::NewSettings.m_IncLoC_OD);
		m_IncisionAxisEdit.SetWindowText(s);
		s.Empty();
		s.Format(_T("%.2f"), ::NewSettings.m_SurInAst_OD);
		m_IncisionInducedCylEdit.SetWindowText(s);
	}
	else if (m_OD == 0)
	{
		s.Format(_T("%i"), ::NewSettings.m_IncLoC_OS);
		m_IncisionAxisEdit.SetWindowText(s);
		s.Empty();
		s.Format(_T("%.2f"), ::NewSettings.m_SurInAst_OS);
		m_IncisionInducedCylEdit.SetWindowText(s);
	}
}
//[cjf***04192012]

//***************************************************************************************

void CPreopDataDlg::GetDlgData()
{
	CString s;

	// Pre-op corneal astigmatism
	m_PreopCorneaCylEdit.GetWindowText(s);

	if (s.IsEmpty())
	{
		m_OpData.m_CorneaPreopCyl = INVALID_VALUE;
	}
	else
	{
		G_As = s;
		m_OpData.m_CorneaPreopCyl = atof(G_As);
	}

	m_PreopCorneaAxisEdit.GetWindowText(s);

	if (s.IsEmpty())
	{
		m_OpData.m_CorneaPreopAxis = INVALID_VALUE;
	}
	else
	{
		G_As = s;
		m_OpData.m_CorneaPreopAxis = atoi(G_As);
	}

	// Pre-op entire eye astigmatism
	m_PreopEntireEyeCylEdit.GetWindowText(s);

	if (s.IsEmpty())
	{
		m_OpData.m_EyePreopCyl = INVALID_VALUE;
	}
	else
	{
		G_As = s;
		m_OpData.m_EyePreopCyl = atof(G_As);
	}

	m_PreopEntireEyeAxisEdit.GetWindowText(s);

	if (s.IsEmpty())
	{
		m_OpData.m_EyePreopAxis = INVALID_VALUE;
	}
	else
	{
		G_As = s;
		m_OpData.m_EyePreopAxis = atoi(G_As);
	}

	// Surgical incision axis
	m_IncisionAxisEdit.GetWindowText(s);

	if (s.IsEmpty())
	{
		m_OpData.m_CorneaIncisionAxis = INVALID_VALUE;
	}
	else
	{
		G_As = s;
		m_OpData.m_CorneaIncisionAxis = atoi(G_As);
	}

	// Surgically induced corneal astigmatism
	m_IncisionInducedCylEdit.GetWindowText(s);

	if (s.IsEmpty())
	{
		m_OpData.m_CorneaInducedCyl = INVALID_VALUE;
	}
	else
	{
		G_As = s;
		m_OpData.m_CorneaInducedCyl = atof(G_As);
	}

	if (m_OpData.m_OpType == 0)
	{
		// IOL sphere
		m_IOLSphereComboBox.GetWindowText(s);

		if (s.IsEmpty())
		{
			m_OpData.m_IOLSph = INVALID_VALUE;
		}
		else
		{
			G_As = s;
			m_OpData.m_IOLSph = atof(G_As);
		}

		// IOL placement axis
		m_IOLPlacementAxisEdit.GetWindowText(s);

		if (s.IsEmpty())
		{
			m_OpData.m_IOLPlacementAxis = INVALID_VALUE;
		}
		else
		{
			G_As = s;
			m_OpData.m_IOLPlacementAxis = atoi(G_As);
		}

		// Selected IOL
		m_OpData.m_SelectedIOL = -1;

		for (int i = 0; i < 8; i++)
		{
			if (m_IOLRadio[i].GetCheck())
			{
				m_OpData.m_SelectedIOL = i;
				break;
			}
		}
	}
}

//***************************************************************************************

BOOL CPreopDataDlg::ValidateData()
{
	if (m_OpData.m_OpType == 0 || m_OpData.m_OpType == 2) {

		if (m_OpData.m_CorneaPreopCyl == INVALID_VALUE) {
			::Error("Please enter pre-op corneal cylinder power.");
			return FALSE;
		}
		if (m_OpData.m_CorneaPreopCyl < 0.0 || m_OpData.m_CorneaPreopCyl > 10.0) {
			::Error("Pre-op corneal cylinder power must be between 0.0 and 10.0 D.");
			return FALSE;
		}
		if (m_OpData.m_CorneaPreopAxis == INVALID_VALUE) {
			::Error("Please enter pre-op corneal cylinder axis.");
			return FALSE;
		}
		if (m_OpData.m_CorneaPreopAxis < 0 || m_OpData.m_CorneaPreopAxis >= 180) {
			::Error("Pre-op corneal cylinder axis must be between 0 and 179.");
			return FALSE;
		}
	}

	if (m_OpData.m_OpType == 0) {

		if (m_OpData.m_CorneaIncisionAxis == INVALID_VALUE) {
			::Error("Please enter surgical incision location.");
			return FALSE;
		}
		if (m_OpData.m_CorneaIncisionAxis < 0 || m_OpData.m_CorneaIncisionAxis >= 360) {
			::Error("Surgical incision location must be between 0 and 359.");
			return FALSE;
		}

		if (m_OpData.m_CorneaInducedCyl == INVALID_VALUE) {
			::Error("Please enter surgically induced cylinder power.");
			return FALSE;
		}
		if (m_OpData.m_CorneaInducedCyl < 0.0 || m_OpData.m_CorneaInducedCyl > 2.0) {
			::Error("Surgically induced cylinder power must be between 0.0 and 2.0 D.");
			return FALSE;
		}

		if (m_OpData.m_IOLSph == INVALID_VALUE) {
			::Error("Please select IOL's spherical power.");
			return FALSE;
		}

		if (m_OpData.m_IOLPlacementAxis == INVALID_VALUE) {
			::Error("Please enter IOL's placement axis.");
			return FALSE;
		}
		if (m_OpData.m_IOLPlacementAxis < 0 || m_OpData.m_IOLPlacementAxis >= 180) {
			::Error("IOL's placement axis must be between 0 and 179.");
			return FALSE;
		}

		if (m_OpData.m_SelectedIOL < 0 || m_OpData.m_SelectedIOL > 7) {
			::Error("Please select an IOL.");
			return FALSE;
		}
	}

	if (m_OpData.m_OpType == 2) {

		if (m_OpData.m_ICL.m_Axis == INVALID_VALUE) {
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
	}

	return TRUE;
}

//***************************************************************************************

BOOL CPreopDataDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	RECT Rect;
	GetClientRect(&Rect);
	CHoyaLogo Logo;
	m_LogoDC.CreateTrueColorDC(Logo.m_w, Logo.m_h);
	m_LogoDC.FillSolidRect(&Rect, ::GetSysColor(COLOR_BTNFACE));
	m_LogoDC.DrawImage(Logo, 0, 0, 0, 0, TRUE);

	m_OpData.Recalculate();

	SetDlgData();

	SetIncLoC_SurInAst();//[cjf***04192012]

	return FALSE;
}

//***************************************************************************************

void CPreopDataDlg::OnOK()
{
	GetDlgData();

	if (!ValidateData()) return;

	*m_pOpData = m_OpData;

	//record the 'incision location' and 'surgically induced astigmatism'  
	if (m_pOpData->m_OpType != 1)// Toric IOC enhancement will automatic set the values to 0, do not save these values
	{
		CString s;
		if (m_OD == 1)
		{
			m_IncisionAxisEdit.GetWindowText(s);
			G_As = s;
			::NewSettings.m_IncLoC_OD = atoi(G_As);
			s.Empty();
			m_IncisionInducedCylEdit.GetWindowText(s);
			G_As = s;
			::NewSettings.m_SurInAst_OD = atof(G_As);
		}
		else if (m_OD == 0)
		{
			m_IncisionAxisEdit.GetWindowText(s);
			G_As = s;
			::NewSettings.m_IncLoC_OS = atoi(G_As);
			s.Empty();
			m_IncisionInducedCylEdit.GetWindowText(s);
			G_As = s;
			::NewSettings.m_SurInAst_OS = atof(G_As);
		}
	}

	//set the GUI axis display
	::TempSettings.NewPlannerSettings.m_MapShowIncision = 0;
	::TempSettings.NewPlannerSettings.m_MapShowPreopCorneal = 0;
	::TempSettings.NewPlannerSettings.m_MapShowPreopInternal = 0;
	::TempSettings.NewPlannerSettings.m_MapShowPreopEntireEye = 0;
	::TempSettings.NewPlannerSettings.m_MapPostopCorneal = 0;
	::TempSettings.NewPlannerSettings.m_MapPostopInternal = 0;
	::TempSettings.NewPlannerSettings.m_MapPostopEntireEye = 1;
	::TempSettings.NewPlannerSettings.m_MapShowICLIndSteep = 0;
	::TempSettings.NewPlannerSettings.m_MapShowICLIndFlat = 0;

	if (m_pOpData->m_OpType == 0)//IOL
	{
		::TempSettings.NewPlannerSettings.m_MapShowIncision = 1;
	}
	else if (m_pOpData->m_OpType == 2)//ICL
	{
		::TempSettings.NewPlannerSettings.m_MapShowPreopEntireEye = 1;
		::TempSettings.NewPlannerSettings.m_MapShowICLIndSteep = 1;
		::TempSettings.NewPlannerSettings.m_MapPostopEntireEye = 1;
	}
	::TempSettings.NewPlannerSettings.m_NewPlaner = 1;
	::Settings.m_PlannerSettings.m_MapShowZaldivarCaliper = 0;
	//[cjf***05022012*004*]   

	CDialog::OnOK();
}

//***************************************************************************************

void CPreopDataDlg::OnOp1RadioClicked()
{
	GetDlgData();

	m_OpData.m_OpType = 0;

	m_OpData.Recalculate();

	SetDlgData();

	SetIncLoC_SurInAst();//[cjf***04192012]
}

void CPreopDataDlg::OnOp2RadioClicked()
{
	GetDlgData();

	m_OpData.m_OpType = 1;

	m_OpData.Recalculate();

	SetDlgData();
}

void CPreopDataDlg::OnOp3RadioClicked()
{
	GetDlgData();

	m_OpData.m_OpType = 2;

	m_OpData.Recalculate();

	SetDlgData();

	SetIncLoC_SurInAst();//[cjf***04192012]

	CICLDataDlg *pDlg = new CICLDataDlg(this, &m_OpData);
	if (pDlg->DoModal() == IDOK) {
		m_OpData.Recalculate();
		SetDlgData();
	}
	delete pDlg;
}

//***************************************************************************************

void CPreopDataDlg::OnPreopCorneaCylEditChanged()
{
	if (!m_CanChange) return;

	GetDlgData();

	m_OpData.m_IOLPlacementAxis = INVALID_VALUE;

	m_OpData.Recalculate();

	m_PreopCorneaCyl_DontChange = TRUE;

	SetDlgData();

	m_PreopCorneaCyl_DontChange = FALSE;
}

//***************************************************************************************

void CPreopDataDlg::OnPreopCorneaAxisEditChanged()
{
	if (!m_CanChange) return;

	GetDlgData();

	m_OpData.m_IOLPlacementAxis = INVALID_VALUE;

	m_OpData.Recalculate();

	m_PreopCorneaAxis_DontChange = TRUE;

	SetDlgData();

	m_PreopCorneaAxis_DontChange = FALSE;
}

//***************************************************************************************

void CPreopDataDlg::OnPreopEntireEyeCylEditChanged()
{
	if (!m_CanChange) return;

	GetDlgData();

	m_OpData.Recalculate();

	m_PreopEntireEyeCyl_DontChange = TRUE;

	SetDlgData();

	m_PreopEntireEyeCyl_DontChange = FALSE;
}

//***************************************************************************************

void CPreopDataDlg::OnPreopEntireEyeAxisEditChanged()
{
	if (!m_CanChange) return;

	GetDlgData();

	m_OpData.Recalculate();

	m_PreopEntireEyeAxis_DontChange = TRUE;

	SetDlgData();

	m_PreopEntireEyeAxis_DontChange = FALSE;
}

//***************************************************************************************

void CPreopDataDlg::OnPreopCorneaWFCheckClicked()
{
	GetDlgData();

	m_OpData.m_CorneaPreopCyl = m_OpData.m_CorneaPreopCylWF;
	m_OpData.m_CorneaPreopAxis = m_OpData.m_CorneaPreopAxisWF;

	m_OpData.Recalculate();

	SetDlgData();
}

//***************************************************************************************

void CPreopDataDlg::OnPreopCorneaSimKCheckClicked()
{
	GetDlgData();

	m_OpData.m_CorneaPreopCyl = m_OpData.m_CorneaPreopCylSimK;
	m_OpData.m_CorneaPreopAxis = m_OpData.m_CorneaPreopAxisSimK;

	m_OpData.Recalculate();

	SetDlgData();
}

//***************************************************************************************

void CPreopDataDlg::OnPreopEntireEyeWFCheckClicked()
{
	GetDlgData();

	m_OpData.m_EyePreopCyl = m_OpData.m_EyePreopCylWF;
	m_OpData.m_EyePreopAxis = m_OpData.m_EyePreopAxisWF;

	m_OpData.Recalculate();

	SetDlgData();
}

//***************************************************************************************

void CPreopDataDlg::OnIncisionAxisEditChanged()
{
	if (!m_CanChange) return;

	GetDlgData();

	m_OpData.m_IOLPlacementAxis = INVALID_VALUE;

	m_OpData.Recalculate();

	m_IncisionAxis_DontChange = TRUE;

	SetDlgData();

	m_IncisionAxis_DontChange = FALSE;
}

//***************************************************************************************

void CPreopDataDlg::OnIncisionInducedCylEditChanged()
{
	if (!m_CanChange) return;

	GetDlgData();

	m_OpData.m_IOLPlacementAxis = INVALID_VALUE;

	m_OpData.Recalculate();

	m_IncisionInducedCyl_DontChange = TRUE;

	SetDlgData();

	m_IncisionInducedCyl_DontChange = FALSE;
}

//***************************************************************************************

void CPreopDataDlg::OnIOLAxisEditChanged()
{
	if (!m_CanChange) return;

	GetDlgData();

	m_OpData.Recalculate();

	m_IOLAxis_DontChange = TRUE;

	SetDlgData();

	m_IOLAxis_DontChange = FALSE;
}

//***************************************************************************************

void CPreopDataDlg::OnIOLRadioClicked(const int i)
{
	GetDlgData();

	m_OpData.m_SelectedIOL = i;

	m_OpData.m_InternalPostopCyl = m_OpData.m_IOLs[i].m_InternalPostopCyl;
	m_OpData.m_InternalPostopAxis = m_OpData.m_IOLs[i].m_InternalPostopAxis;
	m_OpData.m_EyePostopCyl = m_OpData.m_IOLs[i].m_EyePostopCyl;
	m_OpData.m_EyePostopAxis = m_OpData.m_IOLs[i].m_EyePostopAxis;

	SetDlgData();
}

//***************************************************************************************

void CPreopDataDlg::OnPaint()
{
	CPaintDC PaintDC(this);

	RECT Rect;
	GetClientRect(&Rect);

	PaintDC.BitBlt(intRound(0.970 * Rect.right) - m_LogoDC.m_w, intRound(0.030 * Rect.bottom),
		m_LogoDC.m_w, m_LogoDC.m_h, &m_LogoDC, 0, 0, SRCCOPY);

	//CMDC m_MemDC;
}

//***************************************************************************************
