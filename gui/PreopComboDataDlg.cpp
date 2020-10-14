//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "PreopComboDataDlg.h"
#include "ICLDataDlg.h"
#include "Settings.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

int angular_distance_counterclockwise2(int a1, int a2)
{
	int da = (a2 - a1) % 360;
	if (da < 0) da += 360;
	return da;
}

//***************************************************************************************

// distance between a1 and a2 ?[0, 180)
int angular_distance2(int a1, int a2)
{
	int da = angular_distance_counterclockwise2(a1, a2);
	if (da > 180) da = 360 - da;
	return da;
}

//***************************************************************************************

BEGIN_MESSAGE_MAP(CPreopComboDataDlg, CDialog)

	ON_WM_PAINT()

	ON_WM_CTLCOLOR()

	// PRE-OP
	ON_EN_CHANGE(IDC_PREOP_CORNEA_CYL_EDIT, OnPreopCorneaCylEditChanged)
	ON_EN_CHANGE(IDC_PREOP_CORNEA_AXIS_EDIT, OnPreopCorneaAxisEditChanged)
	ON_EN_CHANGE(IDC_PREOP_ENTIRE_EYE_AXIS_EDIT, OnPreopEntireEyeAxisEditChanged)
	ON_EN_CHANGE(IDC_PREOP_ENTIRE_EYE_CYL_EDIT, OnPreopEntireEyeCylEditChanged)
	ON_BN_CLICKED(IDC_PREOP_CORNEA_WF_CHECK, OnPreopCorneaWFCheckClicked)
	ON_BN_CLICKED(IDC_PREOP_CORNEA_SIMK_CHECK, OnPreopCorneaSimKCheckClicked)
	ON_BN_CLICKED(IDC_PREOP_ENTIRE_EYE_WF_CHECK, OnPreopEntireEyeWFCheckClicked)
	ON_BN_CLICKED(IDC_PREOP_CYCLINDER_CHECK, OnPreopCyclinderAdjCheckClicked)//6.2.0 Cyclinder adjustment Branch

	 // IOL
	ON_EN_CHANGE(IDC_INCISION_INDUCED_CYL_EDIT, OnIncisionInducedCylEditChanged)
	ON_EN_CHANGE(IDC_INCISION_AXIS_EDIT, OnIncisionAxisEditChanged)
	ON_EN_CHANGE(IDC_IOL_PLACEMENT_AXIS_EDIT, OnIOLAxisEditChanged)
	ON_BN_CLICKED(IDC_IOL1_RADIO, OnIOL1RadioClicked)
	ON_BN_CLICKED(IDC_IOL2_RADIO, OnIOL2RadioClicked)
	ON_BN_CLICKED(IDC_IOL3_RADIO, OnIOL3RadioClicked)
	ON_BN_CLICKED(IDC_IOL4_RADIO, OnIOL4RadioClicked)
	ON_BN_CLICKED(IDC_IOL5_RADIO, OnIOL5RadioClicked)
	ON_BN_CLICKED(IDC_IOL5a_RADIO, OnIOL5aRadioClicked)
	ON_BN_CLICKED(IDC_IOL6_RADIO, OnIOL6RadioClicked)
	ON_BN_CLICKED(IDC_IOL7_RADIO, OnIOL7RadioClicked)
	ON_BN_CLICKED(IDC_IOL8_RADIO, OnIOL8RadioClicked)

	ON_BN_CLICKED(IDC_IOL1_RADIO2, OnIOL9RadioClicked)
	ON_BN_CLICKED(IDC_IOL3_RADIO2, OnIOL10RadioClicked)
	ON_BN_CLICKED(IDC_IOL4_RADIO2, OnIOL11RadioClicked)

END_MESSAGE_MAP()


//***************************************************************************************

CPreopComboDataDlg::CPreopComboDataDlg(CWnd* pParentWnd, COpData* pOpData, int OD) :
	CDialog(IDD_PREOP_COMBO_DATA_DLG, pParentWnd)
{
	m_OD = OD;

	m_pOpData = pOpData;
	m_OpData = *pOpData;

	if (m_OpData.m_OpType == 1)
	{
		m_OpData.m_OpType = 0;
	}

	//6.2.0 Cyclinder adjustment Branch
	if (m_OpData.m_CorneaCycliderAdj != TRUE && m_OpData.m_CorneaCycliderAdj != FALSE)
	{
		m_OpData.m_CorneaCycliderAdj = FALSE;		
	}
	else
	{
		m_OpData.m_CorneaCycliderAdj = m_OpData.m_CorneaCycliderAdj;	
	}
	//Cyclinder adjustment Branch

	m_CanChange = FALSE;

	m_PreopCorneaCyl_DontChange = FALSE;
	m_PreopCorneaAxis_DontChange = FALSE;
	m_PreopEntireEyeCyl_DontChange = FALSE;
	m_PreopEntireEyeAxis_DontChange = FALSE;
	m_IncisionAxis_DontChange = FALSE;
	m_IncisionInducedCyl_DontChange = FALSE;
	m_IOLAxis_DontChange = FALSE;
	
	m_Last_BestIOL = -1;//
}
//***************************************************************************************

BOOL CPreopComboDataDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_OpData.Recalculate();

	SetDlgData();

	SetIncLoC_SurInAst();
	return FALSE;
}
//***************************************************************************************

void CPreopComboDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

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

	DDX_Control(pDX, IDC_PREOP_CYCLINDER_CHECK, m_PreopCyclinderAdjCheck);//6.2.0 Cyclinder adjustment Branch

	// IOL
	DDX_Control(pDX, IDC_INCISION_AXIS_EDIT, m_IncisionAxisEdit);
	DDX_Control(pDX, IDC_INCISION_INDUCED_CYL_EDIT, m_IncisionInducedCylEdit);
	DDX_Control(pDX, IDC_INCISION_INDUCED_AXIS_EDIT, m_IncisionInducedAxisEdit);

	DDX_Control(pDX, IDC_IOL_SPHERE_COMBO, m_IOLSphereComboBox);
	DDX_Control(pDX, IDC_IOL_PLACEMENT_AXIS_EDIT, m_IOLPlacementAxisEdit);

	DDX_Control(pDX, IDC_IOL1_RADIO, m_IOLRadio[0]);
	DDX_Control(pDX, IDC_IOL1_RADIO2, m_IOLRadio[9]);
	DDX_Control(pDX, IDC_IOL2_RADIO, m_IOLRadio[1]);
	DDX_Control(pDX, IDC_IOL3_RADIO, m_IOLRadio[2]);
	DDX_Control(pDX, IDC_IOL3_RADIO2, m_IOLRadio[10]);
	DDX_Control(pDX, IDC_IOL4_RADIO, m_IOLRadio[3]);
	DDX_Control(pDX, IDC_IOL4_RADIO2, m_IOLRadio[11]);
	DDX_Control(pDX, IDC_IOL5_RADIO, m_IOLRadio[4]);
	DDX_Control(pDX, IDC_IOL5a_RADIO, m_IOLRadio[8]);
	DDX_Control(pDX, IDC_IOL6_RADIO, m_IOLRadio[5]);
	DDX_Control(pDX, IDC_IOL7_RADIO, m_IOLRadio[6]);
	DDX_Control(pDX, IDC_IOL8_RADIO, m_IOLRadio[7]);

	DDX_Control(pDX, IDC_IOL1_CYL1_EDIT, m_IOLCylEdit[0][0]);
	DDX_Control(pDX, IDC_IOL1_CYL1_EDIT2, m_IOLCylEdit[9][0]);
	DDX_Control(pDX, IDC_IOL2_CYL1_EDIT, m_IOLCylEdit[1][0]);
	DDX_Control(pDX, IDC_IOL3_CYL1_EDIT, m_IOLCylEdit[2][0]);
	DDX_Control(pDX, IDC_IOL3_CYL1_EDIT2, m_IOLCylEdit[10][0]);
	DDX_Control(pDX, IDC_IOL4_CYL1_EDIT, m_IOLCylEdit[3][0]);
	DDX_Control(pDX, IDC_IOL4_CYL1_EDIT2, m_IOLCylEdit[11][0]);
	DDX_Control(pDX, IDC_IOL5_CYL1_EDIT, m_IOLCylEdit[4][0]);
	DDX_Control(pDX, IDC_IOL5a_CYL1_EDIT, m_IOLCylEdit[8][0]);
	DDX_Control(pDX, IDC_IOL6_CYL1_EDIT, m_IOLCylEdit[5][0]);
	DDX_Control(pDX, IDC_IOL7_CYL1_EDIT, m_IOLCylEdit[6][0]);
	DDX_Control(pDX, IDC_IOL8_CYL1_EDIT, m_IOLCylEdit[7][0]);

	DDX_Control(pDX, IDC_IOL1_CYL2_EDIT, m_IOLCylEdit[0][1]);
	DDX_Control(pDX, IDC_IOL1_CYL2_EDIT2, m_IOLCylEdit[9][1]);
	DDX_Control(pDX, IDC_IOL2_CYL2_EDIT, m_IOLCylEdit[1][1]);
	DDX_Control(pDX, IDC_IOL3_CYL2_EDIT, m_IOLCylEdit[2][1]);
	DDX_Control(pDX, IDC_IOL3_CYL2_EDIT2, m_IOLCylEdit[10][1]);
	DDX_Control(pDX, IDC_IOL4_CYL2_EDIT, m_IOLCylEdit[3][1]);
	DDX_Control(pDX, IDC_IOL4_CYL2_EDIT2, m_IOLCylEdit[11][1]);
	DDX_Control(pDX, IDC_IOL5_CYL2_EDIT, m_IOLCylEdit[4][1]);
	DDX_Control(pDX, IDC_IOL5a_CYL2_EDIT, m_IOLCylEdit[8][1]);
	DDX_Control(pDX, IDC_IOL6_CYL2_EDIT, m_IOLCylEdit[5][1]);
	DDX_Control(pDX, IDC_IOL7_CYL2_EDIT, m_IOLCylEdit[6][1]);
	DDX_Control(pDX, IDC_IOL8_CYL2_EDIT, m_IOLCylEdit[7][1]);

	DDX_Control(pDX, IDC_IOL1_AXIS_EDIT, m_IOLAxisEdit[0]);
	DDX_Control(pDX, IDC_IOL1_AXIS_EDIT2, m_IOLAxisEdit[9]);
	DDX_Control(pDX, IDC_IOL2_AXIS_EDIT, m_IOLAxisEdit[1]);
	DDX_Control(pDX, IDC_IOL3_AXIS_EDIT, m_IOLAxisEdit[2]);
	DDX_Control(pDX, IDC_IOL3_AXIS_EDIT2, m_IOLAxisEdit[10]);
	DDX_Control(pDX, IDC_IOL4_AXIS_EDIT, m_IOLAxisEdit[3]);
	DDX_Control(pDX, IDC_IOL4_AXIS_EDIT2, m_IOLAxisEdit[11]);
	DDX_Control(pDX, IDC_IOL5_AXIS_EDIT, m_IOLAxisEdit[4]);
	DDX_Control(pDX, IDC_IOL5a_AXIS_EDIT, m_IOLAxisEdit[8]);
	DDX_Control(pDX, IDC_IOL6_AXIS_EDIT, m_IOLAxisEdit[5]);
	DDX_Control(pDX, IDC_IOL7_AXIS_EDIT, m_IOLAxisEdit[6]);
	DDX_Control(pDX, IDC_IOL8_AXIS_EDIT, m_IOLAxisEdit[7]);

	DDX_Control(pDX, IDC_IOL1_POSTOP_EYE_CYL_EDIT, m_IOLPostopEyeCylEdit[0]);
	DDX_Control(pDX, IDC_IOL1_POSTOP_EYE_CYL_EDIT2, m_IOLPostopEyeCylEdit[9]);
	DDX_Control(pDX, IDC_IOL2_POSTOP_EYE_CYL_EDIT, m_IOLPostopEyeCylEdit[1]);
	DDX_Control(pDX, IDC_IOL3_POSTOP_EYE_CYL_EDIT, m_IOLPostopEyeCylEdit[2]);
	DDX_Control(pDX, IDC_IOL3_POSTOP_EYE_CYL_EDIT2, m_IOLPostopEyeCylEdit[10]);
	DDX_Control(pDX, IDC_IOL4_POSTOP_EYE_CYL_EDIT, m_IOLPostopEyeCylEdit[3]);
	DDX_Control(pDX, IDC_IOL4_POSTOP_EYE_CYL_EDIT2, m_IOLPostopEyeCylEdit[11]);
	DDX_Control(pDX, IDC_IOL5_POSTOP_EYE_CYL_EDIT, m_IOLPostopEyeCylEdit[4]);
	DDX_Control(pDX, IDC_IOL5a_POSTOP_EYE_CYL_EDIT, m_IOLPostopEyeCylEdit[8]);
	DDX_Control(pDX, IDC_IOL6_POSTOP_EYE_CYL_EDIT, m_IOLPostopEyeCylEdit[5]);
	DDX_Control(pDX, IDC_IOL7_POSTOP_EYE_CYL_EDIT, m_IOLPostopEyeCylEdit[6]);
	DDX_Control(pDX, IDC_IOL8_POSTOP_EYE_CYL_EDIT, m_IOLPostopEyeCylEdit[7]);

	DDX_Control(pDX, IDC_IOL1_POSTOP_EYE_AXIS_EDIT, m_IOLPostopEyeAxisEdit[0]);
	DDX_Control(pDX, IDC_IOL1_POSTOP_EYE_AXIS_EDIT2, m_IOLPostopEyeAxisEdit[9]);
	DDX_Control(pDX, IDC_IOL2_POSTOP_EYE_AXIS_EDIT, m_IOLPostopEyeAxisEdit[1]);
	DDX_Control(pDX, IDC_IOL3_POSTOP_EYE_AXIS_EDIT, m_IOLPostopEyeAxisEdit[2]);
	DDX_Control(pDX, IDC_IOL3_POSTOP_EYE_AXIS_EDIT2, m_IOLPostopEyeAxisEdit[10]);
	DDX_Control(pDX, IDC_IOL4_POSTOP_EYE_AXIS_EDIT, m_IOLPostopEyeAxisEdit[3]);
	DDX_Control(pDX, IDC_IOL4_POSTOP_EYE_AXIS_EDIT2, m_IOLPostopEyeAxisEdit[11]);
	DDX_Control(pDX, IDC_IOL5_POSTOP_EYE_AXIS_EDIT, m_IOLPostopEyeAxisEdit[4]);
	DDX_Control(pDX, IDC_IOL5a_POSTOP_EYE_AXIS_EDIT, m_IOLPostopEyeAxisEdit[8]);
	DDX_Control(pDX, IDC_IOL6_POSTOP_EYE_AXIS_EDIT, m_IOLPostopEyeAxisEdit[5]);
	DDX_Control(pDX, IDC_IOL7_POSTOP_EYE_AXIS_EDIT, m_IOLPostopEyeAxisEdit[6]);
	DDX_Control(pDX, IDC_IOL8_POSTOP_EYE_AXIS_EDIT, m_IOLPostopEyeAxisEdit[7]);

	DDX_Control(pDX, IDC_ADJ_TEXT, m_PreopAdjStatic);//6.2.0 New
}


//***************************************************************************************

void CPreopComboDataDlg::SetDlgData()
{
	m_CanChange = FALSE;

	CString s;

	m_OpData.m_OpType = 0;

	//6.2.0 Cyclinder adjustment Branch
	if (m_OpData.m_CorneaCycliderAdj != TRUE)
	{
		m_PreopAdjStatic.SetWindowText(_T(""));
	}
	else
	{
		CString s;
		s.Format(_T("%.2fD x %i° [Effective Corneal Power]"), m_OpData.m_Pri_CorneaPreopCyl, m_OpData.m_Pri_CorneaPreopAxis);
		m_PreopAdjStatic.SetWindowText(s);
	}
	//6.2.0 Cyclinder adjustment Branch

	// Pre-op corneal astigmatism
	if (!m_PreopCorneaCyl_DontChange)
	{
		s.Empty();
			
		s.Format(_T("%.2f"), m_OpData.m_CorneaPreopCyl);

		m_PreopCorneaCylEdit.SetWindowText(s);
	}

	if (!m_PreopCorneaAxis_DontChange)
	{
		s.Empty();
		
		s.Format(_T("%i"), m_OpData.m_CorneaPreopAxis);

		m_PreopCorneaAxisEdit.SetWindowText(s);
	}

	m_PreopCorneaWFCheck.SetCheck(
		m_OpData.m_CorneaPreopCyl != INVALID_VALUE && fabs(m_OpData.m_CorneaPreopCyl - m_OpData.m_CorneaPreopCylWF) < 0.001 &&
		m_OpData.m_CorneaPreopAxis != INVALID_VALUE && m_OpData.m_CorneaPreopAxis == m_OpData.m_CorneaPreopAxisWF);

	m_PreopCorneaSimKCheck.SetCheck(
		m_OpData.m_CorneaPreopCyl != INVALID_VALUE && fabs(m_OpData.m_CorneaPreopCyl - m_OpData.m_CorneaPreopCylSimK) < 0.01 &&
		m_OpData.m_CorneaPreopAxis != INVALID_VALUE && m_OpData.m_CorneaPreopAxis == m_OpData.m_CorneaPreopAxisSimK);
	// Pre-op internal optics astigmatism
	
	s.Empty();
	if (m_OpData.m_InternalPreopCyl != INVALID_VALUE) s.Format(_T("%.2f"), m_OpData.m_InternalPreopCyl);
	m_PreopInternalCylEdit.SetWindowText(s);

	s.Empty();
	if (m_OpData.m_InternalPreopAxis != INVALID_VALUE) s.Format(_T("%i"), m_OpData.m_InternalPreopAxis);
	m_PreopInternalAxisEdit.SetWindowText(s);

	// Pre-op entire eye astigmatism

	if (!m_PreopEntireEyeCyl_DontChange)
	{
		s.Empty();

		if (m_OpData.m_EyePreopCyl != INVALID_VALUE)
		{
			s.Format(_T("%.2f"), m_OpData.m_EyePreopCyl);
		}

		m_PreopEntireEyeCylEdit.SetWindowText(s);
	}

	if (!m_PreopEntireEyeAxis_DontChange)
	{
		s.Empty();
		if (m_OpData.m_EyePreopAxis != INVALID_VALUE)
		{
			s.Format(_T("%i"), m_OpData.m_EyePreopAxis);
		}
		m_PreopEntireEyeAxisEdit.SetWindowText(s);
	}

	m_PreopEntireEyeWFCheck.SetCheck(
		m_OpData.m_EyePreopCyl != INVALID_VALUE && fabs(m_OpData.m_EyePreopCyl - m_OpData.m_EyePreopCylWF) < 0.001 &&
		m_OpData.m_EyePreopAxis != INVALID_VALUE && m_OpData.m_EyePreopAxis == m_OpData.m_EyePreopAxisWF);

	// Surgery incision axis

	m_IncisionAxisEdit.SetReadOnly(m_OpData.m_OpType != 0 && m_OpData.m_OpType != 2);

	if (!m_IncisionAxis_DontChange)
	{
		s.Empty();

		if (m_OpData.m_CorneaIncisionAxis != INVALID_VALUE)
		{
			s.Format(_T("%i"), m_OpData.m_CorneaIncisionAxis);
		}

		m_IncisionAxisEdit.SetWindowText(s);
	}


	// Surgically induced corneal astigmatism

	m_IncisionInducedCylEdit.SetReadOnly(m_OpData.m_OpType != 0 && m_OpData.m_OpType != 2);

	if (!m_IncisionInducedCyl_DontChange)
	{
		s.Empty();

		if (m_OpData.m_CorneaInducedCyl != INVALID_VALUE)
		{
			s.Format(_T("%.2f"), m_OpData.m_CorneaInducedCyl);
		}

		m_IncisionInducedCylEdit.SetWindowText(s);
	}

	s.Empty();

	if (m_OpData.m_CorneaInducedAxis != INVALID_VALUE)
	{
		s.Format(_T("%i"), m_OpData.m_CorneaInducedAxis);
	}

	m_IncisionInducedAxisEdit.SetWindowText(s);

	// IOL sphere

	m_IOLSphereComboBox.ResetContent();
	m_IOLSphereComboBox.EnableWindow(TRUE);

	if (m_OpData.m_OpType == 0)
	{
		int CurSel = -1;
		for (int i = 0; i <= 40; i++)
		{
			real IOLSph = 10.0 + 0.5 * i;
			if (i & 1) s.Format(_T("%.1f"), IOLSph); else s.Format(_T("%i"), intRound(IOLSph));
			m_IOLSphereComboBox.AddString(s);
			if (m_OpData.m_IOLSph != INVALID_VALUE)
			{
				if (fabs(m_OpData.m_IOLSph - IOLSph) < 0.001)
				{
					CurSel = i;
				}
			}
		}
		if (CurSel != -1) m_IOLSphereComboBox.SetCurSel(CurSel);
		else
		{
			if (m_OpData.m_IOLSph != INVALID_VALUE)
			{
				CString s;
				s.Format(_T("%.2f"), m_OpData.m_IOLSph);
				m_IOLSphereComboBox.SetWindowText(s);
			}
		}
	}

	// IOL placement axis

	m_IOLPlacementAxisEdit.SetReadOnly(m_OpData.m_OpType != 0);
	if (m_OpData.m_OpType == 0)
	{
		if (!m_IOLAxis_DontChange)
		{
			s.Empty();

			if (m_OpData.m_IOLPlacementAxis != INVALID_VALUE)
			{
				s.Format(_T("%i"), m_OpData.m_IOLPlacementAxis);
			}
			m_IOLPlacementAxisEdit.SetWindowText(s);
		}
	}
	else
	{
		m_IOLPlacementAxisEdit.SetWindowText(_T(""));
	}


	// IOLs
	const int ID[12] =
	{
		IDC_IOL1_STATIC, IDC_IOL2_STATIC, IDC_IOL3_STATIC, IDC_IOL4_STATIC,
		IDC_IOL5_STATIC, IDC_IOL6_STATIC, IDC_IOL7_STATIC, IDC_IOL8_STATIC,
		IDC_IOL5a_STATIC, IDC_IOL1_STATIC2, IDC_IOL3_STATIC2, IDC_IOL4_STATIC2
	};

	for (int i = 0; i < 12; i++)
	{
		CIOL* pIOL;
		
		if (m_OpData.m_CorneaCycliderAdj != TRUE)
		{
			pIOL = &m_OpData.m_IOLs[i];
		}
		else
		{
			pIOL = &m_OpData.m_Pri_IOLs[i];
		}

		s = " " + pIOL->m_Name;
		m_IOLRadio[i].SetWindowText(s);

		s.Format(_T("%.2f"), pIOL->m_Cyl);
		m_IOLCylEdit[i][0].SetWindowText(s);

		s.Format(_T("%.2f"), pIOL->m_InternalPostopCyl);
		m_IOLCylEdit[i][1].SetWindowText(s);

		if (m_OpData.m_OpType == 0)
		{

			m_IOLRadio[i].EnableWindow(TRUE);

			m_IOLRadio[i].SetCheck(m_OpData.m_SelectedIOL == i);

			s.Empty();
			if (pIOL->m_InternalPostopAxis != INVALID_VALUE && fabs(pIOL->m_Cyl) > 0.001)
			{
				s.Format(_T("%i"), pIOL->m_InternalPostopAxis);
			}
			m_IOLAxisEdit[i].SetWindowText(s);

			s.Empty();
			if (pIOL->m_EyePostopCyl != INVALID_VALUE)
			{
				s.Format(_T("%.2f"), pIOL->m_EyePostopCyl);
			}
			m_IOLPostopEyeCylEdit[i].SetWindowText(s);

			s.Empty();
			if (pIOL->m_EyePostopAxis != INVALID_VALUE)
			{
				s.Format(_T("%i"), pIOL->m_EyePostopAxis);
			}
			m_IOLPostopEyeAxisEdit[i].SetWindowText(s);

			s.Empty();
			
			if (m_OpData.m_CorneaCycliderAdj != TRUE)
			{
				if (m_OpData.m_BestIOL == i) s = "min";
			}
			else
			{
				if (m_OpData.m_Pri_BestIOL == i) s = "min";
			}

			((CWnd*)GetDlgItem(ID[i]))->SetWindowText(s);
		}

		else
		{
			m_IOLRadio[i].EnableWindow(FALSE);
			m_IOLRadio[i].SetCheck(0);
			m_IOLAxisEdit[i].SetWindowText(_T(""));
			m_IOLPostopEyeCylEdit[i].SetWindowText(_T(""));
			m_IOLPostopEyeAxisEdit[i].SetWindowText(_T(""));
			((CWnd*)GetDlgItem(ID[i]))->SetWindowText(_T(""));
		}
	}

	// Post-op corneal astigmatism
	//s.Empty();

	//if (m_OpData.m_CorneaPostopCyl != INVALID_VALUE)
	//{
	//	s.Format(_T("%.2f"), m_OpData.m_CorneaPostopCyl);
	//}
	//m_PostopCorneaCylEdit.SetWindowText(s);

	//s.Empty();
	//if (m_OpData.m_CorneaPostopAxis != INVALID_VALUE)
	//{
	//	s.Format(_T("%i"), m_OpData.m_CorneaPostopAxis);
	//}
	//m_PostopCorneaAxisEdit.SetWindowText(s);

	//// Post-op internal astigmatism

	//s.Empty();
	//if (m_OpData.m_InternalPostopCyl != INVALID_VALUE)
	//{
	//	s.Format(_T("%.2f"), m_OpData.m_InternalPostopCyl);
	//}
	//m_PostopInternalCylEdit.SetWindowText(s);

	//s.Empty();
	//if (m_OpData.m_InternalPostopAxis != INVALID_VALUE) 
	//{
	//	s.Format(_T("%i"), m_OpData.m_InternalPostopAxis);
	//}
	//m_PostopInternalAxisEdit.SetWindowText(s);

	//// Post-op entire eye astigmatism

	//s.Empty();
	//if (m_OpData.m_EyePostopCyl != INVALID_VALUE) 
	//{
	//	s.Format(_T("%.2f"), m_OpData.m_EyePostopCyl);
	//}
	//m_PostopEntireEyeCylEdit.SetWindowText(s);

	//s.Empty();
	//if (m_OpData.m_EyePostopAxis != INVALID_VALUE) 
	//{
	//	s.Format(_T("%i"), m_OpData.m_EyePostopAxis);
	//}
	//m_PostopEntireEyeAxisEdit.SetWindowText(s);

	m_CanChange = TRUE;

	m_PreopCyclinderAdjCheck.SetCheck(m_OpData.m_CorneaCycliderAdj);//6.2.0 Cyclinder adjustment Branch

	Invalidate(FALSE);//
}

//***************************************************************************************

//GET the 'incision location' and 'surgically induced astigmatism'  
void CPreopComboDataDlg::SetIncLoC_SurInAst()
{
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

void CPreopComboDataDlg::GetDlgData()
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
		for (int i = 0; i < 12; i++)
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

BOOL CPreopComboDataDlg::ValidateData()
{
	if (m_OpData.m_OpType == 0 || m_OpData.m_OpType == 2)
	{
		if (m_OpData.m_CorneaPreopCyl == INVALID_VALUE)
		{
			::Error("Please enter pre-op corneal cylinder power.");
			return FALSE;
		}
		if (m_OpData.m_CorneaPreopCyl < 0.0 || m_OpData.m_CorneaPreopCyl > 10.0)
		{
			::Error("Pre-op corneal cylinder power must be between 0.0 and 10.0 D.");
			return FALSE;
		}
		if (m_OpData.m_CorneaPreopAxis == INVALID_VALUE)
		{
			::Error("Please enter pre-op corneal cylinder axis.");
			return FALSE;
		}
		if (m_OpData.m_CorneaPreopAxis < 0 || m_OpData.m_CorneaPreopAxis >= 180)
		{
			::Error("Pre-op corneal cylinder axis must be between 0 and 179.");
			return FALSE;
		}
	}

	if (m_OpData.m_OpType == 0)
	{
		if (m_OpData.m_CorneaIncisionAxis == INVALID_VALUE)
		{
			::Error("Please enter surgical incision location.");
			return FALSE;
		}
		if (m_OpData.m_CorneaIncisionAxis < 0 || m_OpData.m_CorneaIncisionAxis >= 360)
		{
			::Error("Surgical incision location must be between 0 and 359.");
			return FALSE;
		}

		if (m_OpData.m_CorneaInducedCyl == INVALID_VALUE) {
			::Error("Please enter surgically induced cylinder power.");
			return FALSE;
		}
		if (m_OpData.m_CorneaInducedCyl < 0.0 || m_OpData.m_CorneaInducedCyl > 2.0)
		{
			::Error("Surgically induced cylinder power must be between 0.0 and 2.0 D.");
			return FALSE;
		}

		if (m_OpData.m_IOLSph == INVALID_VALUE)
		{
			::Error("Please select or input IOL's spherical power.");
			return FALSE;
		}

		if (m_OpData.m_IOLPlacementAxis == INVALID_VALUE)
		{
			::Error("Please enter IOL's placement axis.");
			return FALSE;
		}
		if (m_OpData.m_IOLPlacementAxis < 0 || m_OpData.m_IOLPlacementAxis >= 180)
		{
			::Error("IOL's placement axis must be between 0 and 179.");
			return FALSE;
		}

		if (m_OpData.m_SelectedIOL < 0 || m_OpData.m_SelectedIOL > 11)
		{
			::Error("Please select an IOL.");
			return FALSE;
		}
	}

	return TRUE;
}

//***************************************************************************************

HBRUSH CPreopComboDataDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		const int ID[12] =
		{
			IDC_IOL1_STATIC, IDC_IOL2_STATIC, IDC_IOL3_STATIC, IDC_IOL4_STATIC,
			IDC_IOL5_STATIC, IDC_IOL6_STATIC, IDC_IOL7_STATIC, IDC_IOL8_STATIC,
			IDC_IOL5a_STATIC, IDC_IOL1_STATIC2, IDC_IOL3_STATIC2, IDC_IOL4_STATIC2
		};

		CString s;

		for (int i = 0; i < 12; i++)
		{
			if (pWnd->GetDlgCtrlID() == ID[i])
			{
				((CWnd*)GetDlgItem(ID[i]))->GetWindowText(s);
				if (s == "min")
				{
					pDC->SetTextColor(BLUE);
					break;
				}
			}
		}
	}

	return hbr;
}

//***************************************************************************************

void CPreopComboDataDlg::OnOK()
{
	//6.2.0
	CString s;
	m_IOLSphereComboBox.GetWindowText(s);

	BOOL IsRealNum;

	IsRealNum = ::InputIsRealNum(s);

	if (!IsRealNum && s != "")
	{
		::Info("The spherical power is not a vaild number.");
		m_IOLSphereComboBox.SetWindowText(_T(""));
		m_IOLSphereComboBox.SetFocus();
	}
	//6.2.0

	GetDlgData();

	if (!ValidateData()) return;

	*m_pOpData = m_OpData;

	//record the 'incision location' and 'surgically induced astigmatism'  
	// Toric IOC enhancement will automatic set the values to 0, do not save these values  
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


	// set the GUI axis display
	::TempSettings.NewPlannerSettings.m_MapShowIncision = 0;
	::TempSettings.NewPlannerSettings.m_MapShowPreopCorneal = 0;
	::TempSettings.NewPlannerSettings.m_MapShowPreopInternal = 0;
	::TempSettings.NewPlannerSettings.m_MapShowPreopEntireEye = 0;
	::TempSettings.NewPlannerSettings.m_MapPostopCorneal = 0;
	::TempSettings.NewPlannerSettings.m_MapPostopEntireEye = 1;
	::TempSettings.NewPlannerSettings.m_MapShowICLIndSteep = 0;
	::TempSettings.NewPlannerSettings.m_MapShowICLIndFlat = 0;
	::TempSettings.NewPlannerSettings.m_MapShowIncision = 1;
	::TempSettings.NewPlannerSettings.m_NewPlaner = 1;
	::Settings.m_PlannerSettings.m_MapShowZaldivarCaliper = 0;

	CDialog::OnOK();
}

//***************************************************************************************

void CPreopComboDataDlg::OnPreopCorneaCylEditChanged()
{
	if (!m_CanChange) return;

	GetDlgData();

	m_OpData.m_IOLPlacementAxis = INVALID_VALUE;

	m_OpData.Recalculate();

	m_OpData.m_CorneaCycliderAdjCaled = FALSE;
	m_OpData.m_Pri_IOLPlacementAxis = INVALID_VALUE;
	m_OpData.Pri_Recalculate();//6.2.0	

	m_PreopCorneaCyl_DontChange = TRUE;

	SetDlgData();

	m_PreopCorneaCyl_DontChange = FALSE;
}

//***************************************************************************************

void CPreopComboDataDlg::OnPreopCorneaAxisEditChanged()
{
	if (!m_CanChange) return;

	GetDlgData();
	
	m_OpData.m_IOLPlacementAxis = INVALID_VALUE;

	m_OpData.Recalculate();

	m_OpData.m_CorneaCycliderAdjCaled = FALSE;

	m_OpData.m_Pri_IOLPlacementAxis = INVALID_VALUE;
	m_OpData.Pri_Recalculate();//6.2.0	

	m_PreopCorneaAxis_DontChange = TRUE;

	SetDlgData();

	m_PreopCorneaAxis_DontChange = FALSE;
}

//***************************************************************************************

void CPreopComboDataDlg::OnPreopEntireEyeCylEditChanged()
{
	if (!m_CanChange) return;

	GetDlgData();

	m_OpData.Recalculate();

	m_PreopEntireEyeCyl_DontChange = TRUE;

	SetDlgData();

	m_PreopEntireEyeCyl_DontChange = FALSE;
}

//***************************************************************************************

void CPreopComboDataDlg::OnPreopEntireEyeAxisEditChanged()
{
	if (!m_CanChange) return;

	GetDlgData();

	m_OpData.Recalculate();

	m_PreopEntireEyeAxis_DontChange = TRUE;

	SetDlgData();

	m_PreopEntireEyeAxis_DontChange = FALSE;
}

//***************************************************************************************

void CPreopComboDataDlg::OnPreopCorneaWFCheckClicked()
{
	GetDlgData();

	m_OpData.m_CorneaPreopCyl = m_OpData.m_CorneaPreopCylWF;
	m_OpData.m_CorneaPreopAxis = m_OpData.m_CorneaPreopAxisWF;

	m_OpData.m_CorneaCycliderAdjCaled = FALSE;//6.2.0

	m_OpData.Recalculate();

	m_OpData.m_Pri_IOLPlacementAxis = INVALID_VALUE;
	m_OpData.Pri_Recalculate();

	SetDlgData();	
	//6.2.0
}

//***************************************************************************************

void CPreopComboDataDlg::OnPreopCorneaSimKCheckClicked()
{
	GetDlgData();

	m_OpData.m_CorneaPreopCyl = m_OpData.m_CorneaPreopCylSimK;
	m_OpData.m_CorneaPreopAxis = m_OpData.m_CorneaPreopAxisSimK;

	m_OpData.m_CorneaCycliderAdjCaled = FALSE;//6.2.0

	m_OpData.Recalculate();

	m_OpData.m_Pri_IOLPlacementAxis = INVALID_VALUE;
	m_OpData.Pri_Recalculate();

	SetDlgData();
}

//***************************************************************************************

void CPreopComboDataDlg::OnPreopEntireEyeWFCheckClicked()
{
	GetDlgData();

	m_OpData.m_EyePreopCyl = m_OpData.m_EyePreopCylWF;
	m_OpData.m_EyePreopAxis = m_OpData.m_EyePreopAxisWF;

	m_OpData.Recalculate();

	SetDlgData();
}

//***************************************************************************************

//6.2.0 Cyclinder adjustment Branch
void CPreopComboDataDlg::OnPreopCyclinderAdjCheckClicked()
{
	m_OpData.m_CorneaCycliderAdj = !m_OpData.m_CorneaCycliderAdj;
	m_PreopCyclinderAdjCheck.SetCheck(m_OpData.m_CorneaCycliderAdj);

	m_OpData.Pri_Recalculate();

	SetDlgData();
}

//***************************************************************************************

void CPreopComboDataDlg::OnIncisionAxisEditChanged()
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

void CPreopComboDataDlg::OnIncisionInducedCylEditChanged()
{
	//if (!m_CanChange) return;

	GetDlgData();

	m_OpData.m_IOLPlacementAxis = INVALID_VALUE;

	m_OpData.Recalculate();

	m_IncisionInducedCyl_DontChange = TRUE;

	SetDlgData();

	m_IncisionInducedCyl_DontChange = FALSE;
}

//***************************************************************************************

void CPreopComboDataDlg::OnIOLAxisEditChanged()
{
	if (!m_CanChange) return;

	GetDlgData();

	m_OpData.Recalculate();

	m_IOLAxis_DontChange = TRUE;

	SetDlgData();

	m_IOLAxis_DontChange = FALSE;
}

//***************************************************************************************

void CPreopComboDataDlg::OnIOLRadioClicked(const int i)
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

void CPreopComboDataDlg::OnPaint()
{
	CPaintDC PaintDC(this);

	//Highlight the min row
	const int Title[12] = {
		IDC_IOL1_RADIO, IDC_IOL2_RADIO, IDC_IOL3_RADIO, IDC_IOL4_RADIO,
		IDC_IOL5_RADIO, IDC_IOL6_RADIO, IDC_IOL7_RADIO, IDC_IOL8_RADIO,
		IDC_IOL5a_RADIO, IDC_IOL1_RADIO2, IDC_IOL3_RADIO2, IDC_IOL4_RADIO2
	};

	const int ID[12] = {
		IDC_IOL1_STATIC, IDC_IOL2_STATIC, IDC_IOL3_STATIC, IDC_IOL4_STATIC,
		IDC_IOL5_STATIC, IDC_IOL6_STATIC, IDC_IOL7_STATIC, IDC_IOL8_STATIC,
		IDC_IOL5a_STATIC, IDC_IOL1_STATIC2, IDC_IOL3_STATIC2, IDC_IOL4_STATIC2
	};

	RECT LastHighlightRect, HihglightRect, ComponentRect, LastComRect;

	int BestIOL = m_OpData.m_BestIOL;

	if (m_OpData.m_CorneaCycliderAdj)
	{
		BestIOL = m_OpData.m_Pri_BestIOL;
	}

	if (m_Last_BestIOL == -1)
	{
		GetDlgItem(Title[BestIOL])->GetWindowRect(&ComponentRect);
		GetDlgItem(ID[BestIOL])->GetWindowRect(&LastComRect);

		ScreenToClient(&ComponentRect);
		ScreenToClient(&LastComRect);

		HihglightRect.left = ComponentRect.left - 10;
		HihglightRect.right = LastComRect.right + 10;
		HihglightRect.top = ComponentRect.top - 5;
		HihglightRect.bottom = ComponentRect.bottom + 5;

		PaintDC.DrawFocusRect(&HihglightRect);

		m_Last_BestIOL = BestIOL;
	}
	else
	{
		if (BestIOL != -1 && m_Last_BestIOL != BestIOL)
		{
			//Remove Last Rect	 
			GetDlgItem(Title[m_Last_BestIOL])->GetWindowRect(&ComponentRect);
			GetDlgItem(ID[m_Last_BestIOL])->GetWindowRect(&LastComRect);

			ScreenToClient(&ComponentRect);
			ScreenToClient(&LastComRect);

			LastHighlightRect.left = ComponentRect.left - 10;
			LastHighlightRect.right = LastComRect.right + 10;
			LastHighlightRect.top = ComponentRect.top - 5;
			LastHighlightRect.bottom = ComponentRect.bottom + 5;

			COLORREF BkColor = ::GetSysColor(COLOR_BTNFACE);
			PaintDC.FillSolidRect(&LastHighlightRect, BkColor);
			//Remove Last Rect	 

			GetDlgItem(Title[BestIOL])->GetWindowRect(&ComponentRect);
			GetDlgItem(ID[BestIOL])->GetWindowRect(&LastComRect);

			ScreenToClient(&ComponentRect);
			ScreenToClient(&LastComRect);

			HihglightRect.left = ComponentRect.left - 10;
			HihglightRect.right = LastComRect.right + 10;
			HihglightRect.top = ComponentRect.top - 5;
			HihglightRect.bottom = ComponentRect.bottom + 5;

			PaintDC.DrawFocusRect(&HihglightRect);

			m_Last_BestIOL = BestIOL;
		}

		if (BestIOL == -1)
		{
			//Remove Last Rect	 
			GetDlgItem(Title[m_Last_BestIOL])->GetWindowRect(&ComponentRect);
			GetDlgItem(ID[m_Last_BestIOL])->GetWindowRect(&LastComRect);

			ScreenToClient(&ComponentRect);
			ScreenToClient(&LastComRect);

			LastHighlightRect.left = ComponentRect.left - 10;
			LastHighlightRect.right = LastComRect.right + 10;
			LastHighlightRect.top = ComponentRect.top - 5;
			LastHighlightRect.bottom = ComponentRect.bottom + 5;

			COLORREF BkColor = ::GetSysColor(COLOR_BTNFACE);
			PaintDC.FillSolidRect(&LastHighlightRect, BkColor);
			//Remove Last Rect	 
		}
	}
	//Highlight the min row Done
}

//***************************************************************************************
