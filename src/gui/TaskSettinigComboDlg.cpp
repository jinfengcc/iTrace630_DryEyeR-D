//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "TaskSettinigComboDlg.h"
#include "Data.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CTaskSettinigComboDlg, CDialog)

	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()

	ON_BN_CLICKED(ID_DEFAULT, OnDefault)

	ON_BN_CLICKED(IDC_TASK_EN_RADIO, OnEnableClicked)
	ON_BN_CLICKED(IDC_TASK_DIS_RADIO, OnDisableClicked)

	ON_BN_CLICKED(IDC_WFRA_CHECK, OnWfraClicked)
	ON_BN_CLICKED(IDC_WFAA_CHECK, OnWfaaClicked)
	ON_BN_CLICKED(IDC_VFA_CHECK, OnVfaClicked)
	ON_BN_CLICKED(IDC_WFVA_CHECK, OnWfvaClicked)
	ON_BN_CLICKED(IDC_WFRMS_CHECK, OnWfrmsClicked)
	ON_BN_CLICKED(IDC_WFKA_CHECK, OnWfkaClicked)
	ON_BN_CLICKED(IDC_WFAC_CHECK, OnWfacClicked)
	ON_BN_CLICKED(IDC_NEARVISION_CHECK, OnWfnvClicked)//6.2.0 Near Vision

	ON_BN_CLICKED(IDC_CTDS_CHECK, OnCtdsClicked)
	ON_BN_CLICKED(IDC_CTSU_CHECK, OnCtsuClicked)
	ON_BN_CLICKED(IDC_CTKE_CHECK, OnCtkeClicked)
	ON_BN_CLICKED(IDC_CT3D_CHECK, OnCt3DClicked)
	ON_BN_CLICKED(IDC_CTCM_CHECK, OnCtcmClicked)

	ON_BN_CLICKED(IDC_WCCH_CHECK, OnWcchClicked)
	ON_BN_CLICKED(IDC_WCSU_CHECK, OnWcsuClicked)
	ON_BN_CLICKED(IDC_WCVA_CHECK, OnWcvaClicked)
	ON_BN_CLICKED(IDC_WCOU_CHECK, OnWcouClicked)
	ON_BN_CLICKED(IDC_WCIOL_CHECK, OnWciolClicked)
	ON_BN_CLICKED(IDC_DYSFUN_CHECK, OnDysfunClicked)
	ON_BN_CLICKED(IDC_ASTISOU_CHECK, OnAstiSoufunClicked)
	ON_BN_CLICKED(IDC_DYSPTFUN_CHECK, OnDysPtfunClicked)
	ON_BN_CLICKED(IDC_DEPTHFOCUS_CHECK, OnDepthFocusClicked)
	ON_BN_CLICKED(IDC_TORLI_CHECK, OnTorAliClicked)

	ON_BN_CLICKED(IDC_TOR_CHECK, OnTorClicked)

	ON_BN_CLICKED(IDC_DEFINE1_CHECK, OnDef1ChekClicked)
	ON_BN_CLICKED(IDC_DEFINE2_CHECK, OnDef2ChekClicked)
	ON_BN_CLICKED(IDC_DEFINE3_CHECK, OnDef3ChekClicked)
	ON_BN_CLICKED(IDC_DEFINE4_CHECK, OnDef4ChekClicked)
	ON_BN_CLICKED(IDC_DEFINE5_CHECK, OnDef5ChekClicked)
	ON_BN_CLICKED(IDC_DEFINE6_CHECK, OnDef6ChekClicked)
	ON_BN_CLICKED(IDC_DEFINE7_CHECK, OnDef7ChekClicked)
	ON_BN_CLICKED(IDC_DEFINE8_CHECK, OnDef8ChekClicked)

END_MESSAGE_MAP()
//***************************************************************************************


CTaskSettinigComboDlg::CTaskSettinigComboDlg(CWnd* pParentWnd, CTaskSetting OriTask, int WhichTask) :
	CDialog(IDD_TASK_COMBO_SETTING_DLG, pParentWnd)
{
	m_OriTask = OriTask;
	m_WhichTask = WhichTask;
}

//***************************************************************************************

void CTaskSettinigComboDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TASK_NAME_EDIT, m_TaskName_Edit);

	//Enable radio
	DDX_Control(pDX, IDC_TASK_EN_RADIO, m_TaskEn_Radio);
	DDX_Control(pDX, IDC_TASK_DIS_RADIO, m_TaskDis_Radio);
	//Done

	//WF display
	DDX_Control(pDX, IDC_WFRA_CHECK, m_Wfra_Check); //Refraction Analysis 
	DDX_Control(pDX, IDC_WFRA_TEXT, m_Wfra_Text); //Refraction Analysis   

	DDX_Control(pDX, IDC_WFAA_CHECK, m_Wfaa_Check); //Aberration Analysis 
	DDX_Control(pDX, IDC_WFAA_TEXT, m_Wfaa_Text);  //Aberration Analysis

	DDX_Control(pDX, IDC_VFA_CHECK, m_Vfa_Check);  //VFA
	DDX_Control(pDX, IDC_VFA_TEXT, m_Vfa_Text); //VFA

	DDX_Control(pDX, IDC_WFVA_CHECK, m_Wfva_Check); //WF Visual Acuity
	DDX_Control(pDX, IDC_WFVA_TEXT, m_Wfva_Text); //WF Visual Acuity

	DDX_Control(pDX, IDC_WFRMS_CHECK, m_Wfrms_Check);//WF/RMS
	DDX_Control(pDX, IDC_WFRMS_TEXT, m_Wfrms_Text);//WF/RMS

	DDX_Control(pDX, IDC_WFKA_CHECK, m_Wfka_Check); //Angle K/A
	DDX_Control(pDX, IDC_WFKA_TEXT, m_Wfka_Text); //Angle K/A

	DDX_Control(pDX, IDC_WFAC_CHECK, m_Wfac_Check); //Accomm and DOF Analysis
	DDX_Control(pDX, IDC_WFAC_TEXT, m_Wfac_Text); //Accomm and DOF Analysis

	DDX_Control(pDX, IDC_NEARVISION_CHECK, m_Wfnv_Check); //6.2.0 Near vision
	DDX_Control(pDX, IDC_NEARVISION_TEXT, m_Wfnv_Text); //6.2.0 Near vision  
														//Done

														//CT display
	DDX_Control(pDX, IDC_CTDS_CHECK, m_Ctds_Check); //CT OD/OS
	DDX_Control(pDX, IDC_CTDS_TEXT, m_Ctds_Text); //CT OD/OS

	DDX_Control(pDX, IDC_CTSU_CHECK, m_Ctsu_Check); //CT Summary
	DDX_Control(pDX, IDC_CTSU_TEXT, m_Ctsu_Text); //CT Summary

	DDX_Control(pDX, IDC_CTKE_CHECK, m_Ctke_Check); //Keratometry
	DDX_Control(pDX, IDC_CTKE_TEXT, m_Ctke_Text); //Keratometry

	DDX_Control(pDX, IDC_CT3D_CHECK, m_Ct3d_Check); //3D Z-Elevation
	DDX_Control(pDX, IDC_CT3D_TEXT, m_Ct3d_Text); //3D Z-Elevation

	DDX_Control(pDX, IDC_CTCM_CHECK, m_Ctcm_Check); //CT Compare
	DDX_Control(pDX, IDC_CTCM_TEXT, m_Ctcm_Text); //CT Compare
												  //Done

												  //WF/CT display
	DDX_Control(pDX, IDC_WCCH_CHECK, m_Wcch_Check); //Chang Analysis
	DDX_Control(pDX, IDC_WCSU_CHECK, m_Wcsu_Check); //WF/CT Summary
	DDX_Control(pDX, IDC_WCVA_CHECK, m_Wcva_Check); //WF/CT Visual Acuity
	DDX_Control(pDX, IDC_WCOU_CHECK, m_Wcou_Check); //OU Overview
	DDX_Control(pDX, IDC_WCIOL_CHECK, m_Wciol_Check); //IOL Selection Assistant
	DDX_Control(pDX, IDC_DYSFUN_CHECK, m_Dysfun_Check); // Dysfunctional Analysis for surgeon
	DDX_Control(pDX, IDC_DYSPTFUN_CHECK, m_Dysfun_Pt_Check); // Dysfunctional Analysis for patient  
	DDX_Control(pDX, IDC_ASTISOU_CHECK, m_AstiSou_Check); // Astigmastim Source
	DDX_Control(pDX, IDC_DEPTHFOCUS_CHECK, m_DepthFocus_Check); // Astigmastim Source
	DDX_Control(pDX, IDC_TORLI_CHECK, m_ToricAli_Check); // Toric Alignment

	DDX_Control(pDX, IDC_WCCH_TEXT, m_Wcch_Text); //Chang Analysis
	DDX_Control(pDX, IDC_WCSU_TEXT, m_Wcsu_Text); //WF/CT Summary
	DDX_Control(pDX, IDC_WCVA_TEXT, m_Wcva_Text); //WF/CT Visual Acuity
	DDX_Control(pDX, IDC_WCOU_TEXT, m_Wcou_Text); //OU Overview
	DDX_Control(pDX, IDC_WCIOL_TEXT, m_Wciol_Text); //IOL Selection Assistant
	DDX_Control(pDX, IDC_DYSFUN_TEXT, m_DysFun_Text); // Dysfunctional Analysis for surgeon
	DDX_Control(pDX, IDC_DYSPTFUN_TEXT, m_DysFun_Pt_Text); // Dysfunctional Analysis for patient  
	DDX_Control(pDX, IDC_ASTISOU_TEXT, m_Astisou_Text); // Astigmastim Source
	DDX_Control(pDX, IDC_DEPTHFOCUS_TEXT, m_DepthFocus_Text); // Astigmastim Source
	DDX_Control(pDX, IDC_TORLI_TEXT, m_ToricAli_Text); //Toric Alignment
													   //Done

	DDX_Control(pDX, IDC_TOR_CHECK, m_Tor_Check); //Toric Planner
	DDX_Control(pDX, IDC_TOR_TEXT, m_Tor_Text); //Toric Planner

	DDX_Control(pDX, IDC_OTH_STATIC, m_Other_Text);

	DDX_Control(pDX, IDC_OTH_STATIC2, m_UserDef_Text);

	DDX_Control(pDX, IDC_SN_STATIC, m_sn_Text); //Current selection number

												//User Define
	DDX_Control(pDX, IDC_DEFINE1_CHECK, m_Def_Check[0]);
	DDX_Control(pDX, IDC_DEFINE2_CHECK, m_Def_Check[1]);
	DDX_Control(pDX, IDC_DEFINE3_CHECK, m_Def_Check[2]);
	DDX_Control(pDX, IDC_DEFINE4_CHECK, m_Def_Check[3]);
	DDX_Control(pDX, IDC_DEFINE5_CHECK, m_Def_Check[4]);
	DDX_Control(pDX, IDC_DEFINE6_CHECK, m_Def_Check[5]);
	DDX_Control(pDX, IDC_DEFINE7_CHECK, m_Def_Check[6]);
	DDX_Control(pDX, IDC_DEFINE8_CHECK, m_Def_Check[7]);

	DDX_Control(pDX, IDC_DEFINE1_EDIT, m_Def_Edit[0]);
	DDX_Control(pDX, IDC_DEFINE2_EDIT, m_Def_Edit[1]);
	DDX_Control(pDX, IDC_DEFINE3_EDIT, m_Def_Edit[2]);
	DDX_Control(pDX, IDC_DEFINE4_EDIT, m_Def_Edit[3]);
	DDX_Control(pDX, IDC_DEFINE5_EDIT, m_Def_Edit[4]);
	DDX_Control(pDX, IDC_DEFINE6_EDIT, m_Def_Edit[5]);
	DDX_Control(pDX, IDC_DEFINE7_EDIT, m_Def_Edit[6]);
	DDX_Control(pDX, IDC_DEFINE8_EDIT, m_Def_Edit[7]);
	//Done

	//
	DDX_Control(pDX, IDC_ORDER_TEXT1, m_Order_Text[0]);
	DDX_Control(pDX, IDC_ORDER_TEXT2, m_Order_Text[1]);
	DDX_Control(pDX, IDC_ORDER_TEXT3, m_Order_Text[2]);
	DDX_Control(pDX, IDC_ORDER_TEXT4, m_Order_Text[3]);
	DDX_Control(pDX, IDC_ORDER_TEXT5, m_Order_Text[4]);
	DDX_Control(pDX, IDC_ORDER_TEXT6, m_Order_Text[5]);
	DDX_Control(pDX, IDC_ORDER_TEXT7, m_Order_Text[6]);
	DDX_Control(pDX, IDC_ORDER_TEXT8, m_Order_Text[7]);
	DDX_Control(pDX, IDC_ORDER_TEXT9, m_Order_Text[8]);
	DDX_Control(pDX, IDC_ORDER_TEXT10, m_Order_Text[9]);
	DDX_Control(pDX, IDC_ORDER_TEXT11, m_Order_Text[10]);
	DDX_Control(pDX, IDC_ORDER_TEXT12, m_Order_Text[11]);
	DDX_Control(pDX, IDC_ORDER_TEXT13, m_Order_Text[12]);
	DDX_Control(pDX, IDC_ORDER_TEXT14, m_Order_Text[13]);
	DDX_Control(pDX, IDC_ORDER_TEXT15, m_Order_Text[14]);
	DDX_Control(pDX, IDC_ORDER_TEXT16, m_Order_Text[15]);
	DDX_Control(pDX, IDC_ORDER_TEXT17, m_Order_Text[16]);
	DDX_Control(pDX, IDC_ORDER_TEXT18, m_Order_Text[17]);
	DDX_Control(pDX, IDC_ORDER_TEXT19, m_Order_Text[18]);
	DDX_Control(pDX, IDC_ORDER_TEXT20, m_Order_Text[19]);
	DDX_Control(pDX, IDC_ORDER_TEXT21, m_Order_Text[20]);
	DDX_Control(pDX, IDC_ORDER_TEXT22, m_Order_Text[21]);
	DDX_Control(pDX, IDC_ORDER_TEXT23, m_Order_Text[22]);
	DDX_Control(pDX, IDC_ORDER_TEXT24, m_Order_Text[23]);
	DDX_Control(pDX, IDC_ORDER_TEXT25, m_Order_Text[24]);
	DDX_Control(pDX, IDC_ORDER_TEXT26, m_Order_Text[25]);
	DDX_Control(pDX, IDC_ORDER_TEXT27, m_Order_Text[26]);
	DDX_Control(pDX, IDC_ORDER_TEXT28, m_Order_Text[27]);
	DDX_Control(pDX, IDC_ORDER_TEXT29, m_Order_Text[28]);
	DDX_Control(pDX, IDC_ORDER_TEXT30, m_Order_Text[29]); // Depth of focus
	DDX_Control(pDX, IDC_ORDER_TEXT33, m_Order_Text[30]);
	DDX_Control(pDX, IDC_ORDER_TEXT34, m_Order_Text[31]); //6.2.0 Near Vision 
}
//***************************************************************************************

BOOL CTaskSettinigComboDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	////6.2.0
	//m_Wfnv_Check.ShowWindow(FALSE);
	//m_Wfnv_Text.ShowWindow(FALSE);
	//m_Order_Text[31].ShowWindow(FALSE);
	////6.2.0

	m_t = 0;

	SetDlg(m_OriTask);

	m_SetDefault = FALSE;
	m_Changed = FALSE;

	for (int i = 0; i < 8; i++)
	{
		m_Def_Edit[i].EnableWindow(FALSE);
		m_Def_Check[i].EnableWindow(FALSE);
		m_Def_Edit[i].ShowWindow(FALSE);//for the future
		m_Def_Check[i].ShowWindow(FALSE);//for the future
	}

	if (m_OriTask.m_Enable)
	{
		m_Enable = TRUE;
		m_TaskEn_Radio.SetCheck(TRUE);
		m_TaskDis_Radio.SetCheck(FALSE);
	}
	else
	{
		m_Enable = FALSE;
		m_TaskEn_Radio.SetCheck(FALSE);
		m_TaskDis_Radio.SetCheck(TRUE);
	}

	m_TaskEn_Radio.ShowWindow(FALSE);
	m_TaskDis_Radio.ShowWindow(FALSE);

	m_UserDef_Text.EnableWindow(FALSE);
	m_UserDef_Text.ShowWindow(FALSE);//for the future

	return FALSE;
}

//***************************************************************************************

void CTaskSettinigComboDlg::OnOK()
{
	CString s;
	m_TaskName_Edit.GetWindowText(s);
	if (s == "")
	{
		::Info("Please input the task name.");
		m_TaskName_Edit.SetFocus();
		return;
	}

	s.Trim();

	if (!CheckDefEdit()) return;

	BOOL Enable = m_TaskEn_Radio.GetCheck();

	if (Enable)
	{
		if (m_t == 0)
		{
			::Info("Please select or custom at least one display.");
			return;
		}

		if (m_SetDefault)
		{
			switch (m_WhichTask)
			{
			case 0:
				m_SettedTask = SetDefTask0();
				break;
			case 1:
				m_SettedTask = SetDefTask1();
				break;
			case 2:
				m_SettedTask = SetDefTask2();
				break;
			case 3:
				m_SettedTask = SetDefTask3();
				break;
			case 4:
				m_SettedTask = SetDefTask4();
				break;
			}
		}
		else
		{
			if (m_Changed)
			{
				GetDlg();
			}
			else
			{
				m_SettedTask = m_OriTask;

				CString TaskName = m_SettedTask.ReadBtnName();//6.2.0 For Chinese

				if (s != TaskName)
				{
					m_SettedTask.SaveBtnName(s);		//6.2.0 For Chinese			
				}
			}
		}
	}
	else
	{
		GetDlg();
	}

	m_SettedTask.m_Enable = Enable;

	CDialog::OnOK();
}

//***************************************************************************************

void CTaskSettinigComboDlg::OnDefault()
{
	m_t = 0;

	CTaskSetting DefaultTask;

	if (m_WhichTask == 0) DefaultTask = SetDefTask0();
	else if (m_WhichTask == 1) DefaultTask = SetDefTask1();
	else if (m_WhichTask == 2) DefaultTask = SetDefTask2();
	else if (m_WhichTask == 3) DefaultTask = SetDefTask3();
	else if (m_WhichTask == 4) DefaultTask = SetDefTask4();

	SetDlg(DefaultTask);

	m_SetDefault = TRUE;
	m_Changed = TRUE;
}

//***************************************************************************************

HBRUSH CTaskSettinigComboDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		if (pWnd->GetDlgCtrlID() == IDC_SN_STATIC)
		{
			if (m_Enable) pDC->SetTextColor(RED);
			else         pDC->SetTextColor(GRAY);
		}

		if (pWnd->GetDlgCtrlID() == IDC_WFRA_TEXT)
		{
			if (m_Wfra_Check.GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_WFAA_TEXT)
		{
			if (m_Wfaa_Check.GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_VFA_TEXT)
		{
			if (m_Vfa_Check.GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}


		if (pWnd->GetDlgCtrlID() == IDC_WFVA_TEXT)
		{
			if (m_Wfva_Check.GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}


		if (pWnd->GetDlgCtrlID() == IDC_WFRMS_TEXT)
		{
			if (m_Wfrms_Check.GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}


		if (pWnd->GetDlgCtrlID() == IDC_WFKA_TEXT)
		{
			if (m_Wfka_Check.GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}


		if (pWnd->GetDlgCtrlID() == IDC_WFAC_TEXT)
		{
			if (m_Wfac_Check.GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}

		//6.2.0 Near Vision
		if (pWnd->GetDlgCtrlID() == IDC_NEARVISION_TEXT)
		{
			if (m_Wfnv_Check.GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}
		//6.2.0 Near Vision		

		if (pWnd->GetDlgCtrlID() == IDC_CTDS_TEXT)
		{
			if (m_Ctds_Check.GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_CTSU_TEXT)
		{
			if (m_Ctsu_Check.GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}


		if (pWnd->GetDlgCtrlID() == IDC_CTKE_TEXT)
		{
			if (m_Ctke_Check.GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}


		if (pWnd->GetDlgCtrlID() == IDC_CT3D_TEXT)
		{
			if (m_Ct3d_Check.GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}

		//n
		if (pWnd->GetDlgCtrlID() == IDC_CTCM_TEXT)
		{
			if (m_Ctcm_Check.GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}
		//n

		if (pWnd->GetDlgCtrlID() == IDC_WCCH_TEXT)
		{
			if (m_Wcch_Check.GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}


		if (pWnd->GetDlgCtrlID() == IDC_WCSU_TEXT)
		{
			if (m_Wcsu_Check.GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_WCVA_TEXT)
		{
			if (m_Wcva_Check.GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}


		if (pWnd->GetDlgCtrlID() == IDC_WCOU_TEXT)
		{
			if (m_Wcou_Check.GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}


		if (pWnd->GetDlgCtrlID() == IDC_WCIOL_TEXT)
		{
			if (m_Wciol_Check.GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_DYSFUN_TEXT)
		{
			if (m_Dysfun_Check.GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_DYSPTFUN_TEXT)
		{
			if (m_Dysfun_Pt_Check.GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ASTISOU_TEXT)
		{
			if (m_AstiSou_Check.GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_DEPTHFOCUS_TEXT)
		{
			if (m_DepthFocus_Check.GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_TORLI_TEXT)
		{
			if (m_ToricAli_Check.GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_TOR_TEXT)
		{
			if (m_Tor_Check.GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}


		//ORDER
		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT1)
		{
			if (m_Wfra_Check.GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT2)
		{
			if (m_Wfaa_Check.GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT3)
		{
			if (m_Vfa_Check.GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT4)
		{
			if (m_Wfva_Check.GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT5)
		{
			if (m_Wfrms_Check.GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT6)
		{
			if (m_Wfka_Check.GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT7)
		{
			if (m_Wfac_Check.GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT8)
		{
			if (m_Ctds_Check.GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT9)
		{
			if (m_Ctsu_Check.GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT10)
		{
			if (m_Ctke_Check.GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT11)
		{
			if (m_Ct3d_Check.GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		//n
		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT26)
		{
			if (m_Ctcm_Check.GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}
		//

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT12)
		{
			if (m_Wcch_Check.GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT13)
		{
			if (m_Wcsu_Check.GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT14)
		{
			if (m_Wcva_Check.GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT15)
		{
			if (m_Wcou_Check.GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT16)
		{
			if (m_Wciol_Check.GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT17)
		{
			if (m_Tor_Check.GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT18)
		{
			if (m_Def_Check[0].GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT19)
		{
			if (m_Def_Check[1].GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT20)
		{
			if (m_Def_Check[2].GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT21)
		{
			if (m_Def_Check[3].GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT22)
		{
			if (m_Def_Check[4].GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT23)
		{
			if (m_Def_Check[5].GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT24)
		{
			if (m_Def_Check[6].GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT25)
		{
			if (m_Def_Check[7].GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT27)
		{
			if (m_Dysfun_Check.GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT29)
		{
			if (m_Dysfun_Pt_Check.GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT28)
		{
			if (m_AstiSou_Check.GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT30)
		{
			if (m_DepthFocus_Check.GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT33)
		{
			if (m_Enable)
			{
				pDC->SetTextColor(WHITE);
				pDC->SetBkColor(BLACK);
			}
			else
			{
				pDC->SetTextColor(GRAY);
				pDC->SetBkColor(LIGHT_GRAY);
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_ORDER_TEXT34)//6.2.0 Near Vision
		{
			if (m_Wfnv_Check.GetCheck())
			{
				if (m_Enable)
				{
					pDC->SetTextColor(WHITE);
					pDC->SetBkColor(BLACK);
				}
				else
				{
					pDC->SetTextColor(GRAY);
					pDC->SetBkColor(LIGHT_GRAY);
				}
			}
		}
		//ORDER DONE
	}

	if (nCtlColor == CTLCOLOR_EDIT)
	{
		if (pWnd->GetDlgCtrlID() == IDC_DEFINE1_EDIT)
		{
			if (m_Def_Check[0].GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_DEFINE2_EDIT)
		{
			if (m_Def_Check[1].GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_DEFINE3_EDIT)
		{
			if (m_Def_Check[2].GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_DEFINE4_EDIT)
		{
			if (m_Def_Check[3].GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_DEFINE5_EDIT)
		{
			if (m_Def_Check[4].GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_DEFINE6_EDIT)
		{
			if (m_Def_Check[5].GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_DEFINE7_EDIT)
		{
			if (m_Def_Check[6].GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}

		if (pWnd->GetDlgCtrlID() == IDC_DEFINE8_EDIT)
		{
			if (m_Def_Check[7].GetCheck())
			{
				if (m_Enable) pDC->SetTextColor(BLUE);
				else         pDC->SetTextColor(GRAY);
			}
			else
			{
				if (m_Enable) pDC->SetTextColor(BLACK);
				else         pDC->SetTextColor(GRAY);
			}
		}
	}

	return hbr;
}

//***************************************************************************************

void CTaskSettinigComboDlg::OnCheckBoxClick(int whichone)
{
	m_SetDefault = FALSE;
	m_Changed = TRUE;

	BOOL Select;

	switch (whichone)
	{
	case 0:
		Select = m_Wfra_Check.GetCheck();
		break;
	case 1:
		Select = m_Wfaa_Check.GetCheck();
		break;
	case 2:
		Select = m_Vfa_Check.GetCheck();
		break;
	case 3:
		Select = m_Wfva_Check.GetCheck();
		break;
	case 4:
		Select = m_Wfrms_Check.GetCheck();
		break;
	case 5:
		Select = m_Wfka_Check.GetCheck();
		break;
	case 6:
		Select = m_Wfac_Check.GetCheck();
		break;
	case 7:
		Select = m_Ctds_Check.GetCheck();
		break;
	case 8:
		Select = m_Ctsu_Check.GetCheck();
		break;
	case 9:
		Select = m_Ctke_Check.GetCheck();
		break;
	case 10:
		Select = m_Ct3d_Check.GetCheck();
		break;
	case 11:
		Select = m_Wcch_Check.GetCheck();
		break;
	case 12:
		Select = m_Wcsu_Check.GetCheck();
		break;
	case 13:
		Select = m_Wcva_Check.GetCheck();
		break;
	case 14:
		Select = m_Wcou_Check.GetCheck();
		break;
	case 15:
		Select = m_Wciol_Check.GetCheck();
		break;
	case 16:
		Select = m_Tor_Check.GetCheck();
		break;
	case 17:
		Select = m_Def_Check[0].GetCheck();
		break;
	case 18:
		Select = m_Def_Check[1].GetCheck();
		break;
	case 19:
		Select = m_Def_Check[2].GetCheck();
		break;
	case 20:
		Select = m_Def_Check[3].GetCheck();
		break;
	case 21:
		Select = m_Def_Check[4].GetCheck();
		break;
	case 22:
		Select = m_Def_Check[5].GetCheck();
		break;
	case 23:
		Select = m_Def_Check[6].GetCheck();
		break;
	case 24:
		Select = m_Def_Check[7].GetCheck();
		break;
	case 25:
		Select = m_Ctcm_Check.GetCheck();
		break;
	case 26:
		Select = m_Dysfun_Check.GetCheck();
		break;
	case 27:
		Select = m_AstiSou_Check.GetCheck();
		break;
	case 28:
		Select = m_Dysfun_Pt_Check.GetCheck();
		break;
	case 29:
		Select = m_DepthFocus_Check.GetCheck();
		break;
	case 30:
		Select = m_ToricAli_Check.GetCheck();
		break;
	case 31:
		Select = m_Wfnv_Check.GetCheck();//6.2.0 Near Vision
		break;
	}

	if (Select) 	m_t++;
	else        m_t--;

	if (m_t > 8)
	{
		m_t = 8;
		::Info("Sorry, the maximum selection is 8.");

		switch (whichone)
		{
		case 0:
			m_Wfra_Check.SetCheck(!Select);
			break;
		case 1:
			m_Wfaa_Check.SetCheck(!Select);
			break;
		case 2:
			m_Vfa_Check.SetCheck(!Select);
			break;
		case 3:
			m_Wfva_Check.SetCheck(!Select);
			break;
		case 4:
			m_Wfrms_Check.SetCheck(!Select);
			break;
		case 5:
			m_Wfka_Check.SetCheck(!Select);
			break;
		case 6:
			m_Wfac_Check.SetCheck(!Select);
			break;
		case 7:
			m_Ctds_Check.SetCheck(!Select);
			break;
		case 8:
			m_Ctsu_Check.SetCheck(!Select);
			break;
		case 9:
			m_Ctke_Check.SetCheck(!Select);
			break;
		case 10:
			m_Ct3d_Check.SetCheck(!Select);
			break;
		case 11:
			m_Wcch_Check.SetCheck(!Select);
			break;
		case 12:
			m_Wcsu_Check.SetCheck(!Select);
			break;
		case 13:
			m_Wcva_Check.SetCheck(!Select);
			break;
		case 14:
			m_Wcou_Check.SetCheck(!Select);
			break;
		case 15:
			m_Wciol_Check.SetCheck(!Select);
			break;
		case 16:
			m_Tor_Check.SetCheck(!Select);
			break;
		case 17:
			m_Def_Check[0].SetCheck(!Select);
			break;
		case 18:
			m_Def_Check[1].SetCheck(!Select);
			break;
		case 19:
			m_Def_Check[2].SetCheck(!Select);
			break;
		case 20:
			m_Def_Check[3].SetCheck(!Select);
			break;
		case 21:
			m_Def_Check[4].SetCheck(!Select);
			break;
		case 22:
			m_Def_Check[5].SetCheck(!Select);
			break;
		case 23:
			m_Def_Check[6].SetCheck(!Select);
			break;
		case 24:
			m_Def_Check[7].SetCheck(!Select);
			break;
		case 25:
			m_Ctcm_Check.SetCheck(!Select);
			break;
		case 26:
			m_Dysfun_Check.SetCheck(!Select);
			break;
		case 27:
			m_AstiSou_Check.SetCheck(!Select);
			break;
		case 28:
			m_Dysfun_Pt_Check.SetCheck(!Select);
			break;
		case 29:
			m_DepthFocus_Check.SetCheck(!Select);
			break;
		case 30:
			m_ToricAli_Check.SetCheck(!Select);
			break;
		case 31:
			m_Wfnv_Check.SetCheck(!Select);//6.2.0 Near Vision		
			break;
		}

		return;
	}

	CString s;
	s.Format(_T("%i"), m_t);
	m_sn_Text.SetWindowText(s);

	//Order
	int tt = 0;
	if (m_Wfra_Check.GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[0].ShowWindow(TRUE);
		m_Order_Text[0].SetWindowText(s);
		m_Order_Text_Show[0] = TRUE;
		m_Order_OriInt[0] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[0].ShowWindow(FALSE);
		m_Order_Text_Show[0] = FALSE;
	}

	if (m_Wfaa_Check.GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[1].ShowWindow(TRUE);
		m_Order_Text[1].SetWindowText(s);
		m_Order_Text_Show[1] = TRUE;
		m_Order_OriInt[1] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[1].ShowWindow(FALSE);
		m_Order_Text_Show[1] = FALSE;
	}

	if (m_Vfa_Check.GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[2].ShowWindow(TRUE);
		m_Order_Text[2].SetWindowText(s);
		m_Order_Text_Show[2] = TRUE;
		m_Order_OriInt[2] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[2].ShowWindow(FALSE);
		m_Order_Text_Show[2] = FALSE;
	}

	if (m_Wfva_Check.GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[3].ShowWindow(TRUE);
		m_Order_Text[3].SetWindowText(s);
		m_Order_Text_Show[3] = TRUE;
		m_Order_OriInt[3] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[3].ShowWindow(FALSE);
		m_Order_Text_Show[3] = FALSE;
	}

	if (m_Wfrms_Check.GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[4].ShowWindow(TRUE);
		m_Order_Text[4].SetWindowText(s);
		m_Order_Text_Show[4] = TRUE;
		m_Order_OriInt[4] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[4].ShowWindow(FALSE);
		m_Order_Text_Show[4] = FALSE;
	}

	if (m_Wfka_Check.GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[5].ShowWindow(TRUE);
		m_Order_Text[5].SetWindowText(s);
		m_Order_Text_Show[5] = TRUE;
		m_Order_OriInt[5] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[5].ShowWindow(FALSE);
		m_Order_Text_Show[5] = FALSE;
	}

	if (m_Wfac_Check.GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[6].ShowWindow(TRUE);
		m_Order_Text[6].SetWindowText(s);
		m_Order_Text_Show[6] = TRUE;
		m_Order_OriInt[6] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[6].ShowWindow(FALSE);
		m_Order_Text_Show[6] = FALSE;
	}

	//6.2.0 Near Vision
	if (m_Wfnv_Check.GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[31].ShowWindow(TRUE);
		m_Order_Text[31].SetWindowText(s);
		m_Order_Text_Show[31] = TRUE;
		m_Order_OriInt[31] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[31].ShowWindow(FALSE);
		m_Order_Text_Show[31] = FALSE;
	}
	//6.2.0 Near Vision

	if (m_Ctds_Check.GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[7].ShowWindow(TRUE);
		m_Order_Text[7].SetWindowText(s);
		m_Order_Text_Show[7] = TRUE;
		m_Order_OriInt[7] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[7].ShowWindow(FALSE);
		m_Order_Text_Show[7] = FALSE;
	}

	if (m_Ctsu_Check.GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[8].ShowWindow(TRUE);
		m_Order_Text[8].SetWindowText(s);
		m_Order_Text_Show[8] = TRUE;
		m_Order_OriInt[8] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[8].ShowWindow(FALSE);
		m_Order_Text_Show[8] = FALSE;
	}

	if (m_Ctke_Check.GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[9].ShowWindow(TRUE);
		m_Order_Text[9].SetWindowText(s);
		m_Order_Text_Show[9] = TRUE;
		m_Order_OriInt[9] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[9].ShowWindow(FALSE);
		m_Order_Text_Show[9] = FALSE;
	}

	if (m_Ct3d_Check.GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[10].ShowWindow(TRUE);
		m_Order_Text[10].SetWindowText(s);
		m_Order_Text_Show[10] = TRUE;
		m_Order_OriInt[10] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[10].ShowWindow(FALSE);
		m_Order_Text_Show[10] = FALSE;
	}

	//n
	if (m_Ctcm_Check.GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[25].ShowWindow(TRUE);
		m_Order_Text[25].SetWindowText(s);
		m_Order_Text_Show[25] = TRUE;
		m_Order_OriInt[25] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[25].ShowWindow(FALSE);
		m_Order_Text_Show[25] = FALSE;
	}
	//Done

	if (m_Wcch_Check.GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[11].ShowWindow(TRUE);
		m_Order_Text[11].SetWindowText(s);
		m_Order_Text_Show[11] = TRUE;
		m_Order_OriInt[11] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[11].ShowWindow(FALSE);
		m_Order_Text_Show[11] = FALSE;
	}

	if (m_Wcsu_Check.GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[12].ShowWindow(TRUE);
		m_Order_Text[12].SetWindowText(s);
		m_Order_Text_Show[12] = TRUE;
		m_Order_OriInt[12] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[12].ShowWindow(FALSE);
		m_Order_Text_Show[12] = FALSE;
	}

	if (m_Wcva_Check.GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[13].ShowWindow(TRUE);
		m_Order_Text[13].SetWindowText(s);
		m_Order_Text_Show[13] = TRUE;
		m_Order_OriInt[13] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[13].ShowWindow(FALSE);
		m_Order_Text_Show[13] = FALSE;
	}

	if (m_Wcou_Check.GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[14].ShowWindow(TRUE);
		m_Order_Text[14].SetWindowText(s);
		m_Order_Text_Show[14] = TRUE;
		m_Order_OriInt[14] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[14].ShowWindow(FALSE);
		m_Order_Text_Show[14] = FALSE;
	}

	if (m_Wciol_Check.GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[15].ShowWindow(TRUE);
		m_Order_Text[15].SetWindowText(s);
		m_Order_Text_Show[15] = TRUE;
		m_Order_OriInt[15] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[15].ShowWindow(FALSE);
		m_Order_Text_Show[15] = FALSE;
	}

	if (m_Dysfun_Check.GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[26].ShowWindow(TRUE);
		m_Order_Text[26].SetWindowText(s);
		m_Order_Text_Show[26] = TRUE;
		m_Order_OriInt[26] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[26].ShowWindow(FALSE);
		m_Order_Text_Show[26] = FALSE;
	}

	if (m_AstiSou_Check.GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[27].ShowWindow(TRUE);
		m_Order_Text[27].SetWindowText(s);
		m_Order_Text_Show[27] = TRUE;
		m_Order_OriInt[27] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[27].ShowWindow(FALSE);
		m_Order_Text_Show[27] = FALSE;
	}

	if (m_Dysfun_Pt_Check.GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[28].ShowWindow(TRUE);
		m_Order_Text[28].SetWindowText(s);
		m_Order_Text_Show[28] = TRUE;
		m_Order_OriInt[28] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[28].ShowWindow(FALSE);
		m_Order_Text_Show[28] = FALSE;
	}

	if (m_DepthFocus_Check.GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[29].ShowWindow(TRUE);
		m_Order_Text[29].SetWindowText(s);
		m_Order_Text_Show[29] = TRUE;
		m_Order_OriInt[29] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[29].ShowWindow(FALSE);
		m_Order_Text_Show[29] = FALSE;
	}

	if (m_ToricAli_Check.GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[30].ShowWindow(TRUE);
		m_Order_Text[30].SetWindowText(s);
		m_Order_Text_Show[30] = TRUE;
		m_Order_OriInt[30] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[30].ShowWindow(FALSE);
		m_Order_Text_Show[30] = FALSE;
	}

	if (m_Tor_Check.GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[16].ShowWindow(TRUE);
		m_Order_Text[16].SetWindowText(s);
		m_Order_Text_Show[16] = TRUE;
		m_Order_OriInt[16] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[16].ShowWindow(FALSE);
		m_Order_Text_Show[16] = FALSE;
	}

	if (m_Def_Check[0].GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[17].ShowWindow(TRUE);
		m_Order_Text[17].SetWindowText(s);
		m_Order_Text_Show[17] = TRUE;
		m_Order_OriInt[17] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[17].ShowWindow(FALSE);
		m_Order_Text_Show[17] = FALSE;
	}

	if (m_Def_Check[1].GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[18].ShowWindow(TRUE);
		m_Order_Text[18].SetWindowText(s);
		m_Order_Text_Show[18] = TRUE;
		m_Order_OriInt[18] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[18].ShowWindow(FALSE);
		m_Order_Text_Show[18] = FALSE;
	}

	if (m_Def_Check[2].GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[19].ShowWindow(TRUE);
		m_Order_Text[19].SetWindowText(s);
		m_Order_Text_Show[19] = TRUE;
		m_Order_OriInt[19] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[19].ShowWindow(FALSE);
		m_Order_Text_Show[19] = FALSE;
	}

	if (m_Def_Check[3].GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[20].ShowWindow(TRUE);
		m_Order_Text[20].SetWindowText(s);
		m_Order_Text_Show[20] = TRUE;
		m_Order_OriInt[20] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[20].ShowWindow(FALSE);
		m_Order_Text_Show[20] = FALSE;
	}

	if (m_Def_Check[4].GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[21].ShowWindow(TRUE);
		m_Order_Text[21].SetWindowText(s);
		m_Order_Text_Show[21] = TRUE;
		m_Order_OriInt[21] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[21].ShowWindow(FALSE);
		m_Order_Text_Show[21] = FALSE;
	}

	if (m_Def_Check[5].GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[22].ShowWindow(TRUE);
		m_Order_Text[22].SetWindowText(s);
		m_Order_Text_Show[22] = TRUE;
		m_Order_OriInt[22] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[22].ShowWindow(FALSE);
		m_Order_Text_Show[22] = FALSE;
	}

	if (m_Def_Check[6].GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[23].ShowWindow(TRUE);
		m_Order_Text[23].SetWindowText(s);
		m_Order_Text_Show[23] = TRUE;
		m_Order_OriInt[23] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[23].ShowWindow(FALSE);
		m_Order_Text_Show[23] = FALSE;
	}

	if (m_Def_Check[7].GetCheck())
	{
		CString s = GetStrN(tt);
		m_Order_Text[24].ShowWindow(TRUE);
		m_Order_Text[24].SetWindowText(s);
		m_Order_Text_Show[24] = TRUE;
		m_Order_OriInt[24] = tt + 1;
		tt++;
	}
	else
	{
		m_Order_Text[24].ShowWindow(FALSE);
		m_Order_Text_Show[24] = FALSE;
	}

	Invalidate(FALSE);
}

//***************************************************************************************

void CTaskSettinigComboDlg::SetDlg(CTaskSetting Task)
{
	CString s = Task.ReadBtnName();
	m_TaskName_Edit.SetWindowText(s);

	if (Task.m_Enable)
	{
		m_Enable = TRUE;
		m_TaskEn_Radio.SetCheck(TRUE);
		m_TaskDis_Radio.SetCheck(FALSE);
	}
	else
	{
		m_Enable = FALSE;
		m_TaskEn_Radio.SetCheck(TRUE);
		m_TaskDis_Radio.SetCheck(FALSE);
	}

	m_Wfra_Check.SetCheck(FALSE);
	m_Wfaa_Check.SetCheck(FALSE);
	m_Vfa_Check.SetCheck(FALSE);
	m_Wfva_Check.SetCheck(FALSE);
	m_Wfrms_Check.SetCheck(FALSE);
	m_Wfka_Check.SetCheck(FALSE);
	m_Wfac_Check.SetCheck(FALSE);
	m_Wfnv_Check.SetCheck(FALSE);//6.2.0 Near Vision

	m_Ctds_Check.SetCheck(FALSE);
	m_Ctsu_Check.SetCheck(FALSE);
	m_Ctke_Check.SetCheck(FALSE);
	m_Ct3d_Check.SetCheck(FALSE);
	m_Ctcm_Check.SetCheck(FALSE);

	m_Wcch_Check.SetCheck(FALSE);
	m_Wcsu_Check.SetCheck(FALSE);
	m_Wcva_Check.SetCheck(FALSE);
	m_Wcou_Check.SetCheck(FALSE);
	m_Wciol_Check.SetCheck(FALSE);
	m_Dysfun_Check.SetCheck(FALSE);
	m_Dysfun_Pt_Check.SetCheck(FALSE);
	m_AstiSou_Check.SetCheck(FALSE);
	m_DepthFocus_Check.SetCheck(FALSE);
	m_ToricAli_Check.SetCheck(FALSE);

	m_Tor_Check.SetCheck(FALSE);

	for (int i = 0; i < 8; i++)
	{
		m_Def_Check[i].SetCheck(FALSE);
		m_Def_Edit[i].SetWindowText(_T(""));
	}

	m_LastSort = -1;

	for (int i = 0; i < 32; i++)
	{
		m_Order_Text[i].ShowWindow(FALSE);
		m_Order_Text_Show[i] = FALSE;
		m_Order_Int[i] = 0;
		m_Order_OriInt[i] = 0;
	}

	for (int i = 0; i < Task.m_SubBtnNum; i++)
	{
		int Type = Task.m_SubButton[i];

		CString s = GetStrN(i);
		if (Type != TYPE_NONA)
		{
			m_t++;
		}

		switch (Type)
		{
		case TYPE_NONA:
			break;

		case TYPE_DEFI1:
			m_Def_Check[0].SetCheck(TRUE);
			m_Order_Text[17].ShowWindow(TRUE);
			m_Order_Text[17].SetWindowText(s);
			m_Order_Text_Show[17] = TRUE;
			m_Order_OriInt[17] = i + 1;
			break;

		case TYPE_DEFI2:
			m_Def_Check[1].SetCheck(TRUE);
			m_Order_Text[18].ShowWindow(TRUE);
			m_Order_Text[18].SetWindowText(s);
			m_Order_Text_Show[18] = TRUE;
			m_Order_OriInt[18] = i + 1;
			break;

		case TYPE_DEFI3:
			m_Def_Check[2].SetCheck(TRUE);
			m_Order_Text[19].ShowWindow(TRUE);
			m_Order_Text[19].SetWindowText(s);
			m_Order_Text_Show[19] = TRUE;
			m_Order_OriInt[19] = i + 1;
			break;

		case TYPE_DEFI4:
			m_Def_Check[3].SetCheck(TRUE);
			m_Order_Text[20].ShowWindow(TRUE);
			m_Order_Text[20].SetWindowText(s);
			m_Order_Text_Show[20] = TRUE;
			m_Order_OriInt[20] = i + 1;
			break;

		case TYPE_DEFI5:
			m_Def_Check[4].SetCheck(TRUE);
			m_Order_Text[21].ShowWindow(TRUE);
			m_Order_Text[21].SetWindowText(s);
			m_Order_Text_Show[21] = TRUE;
			m_Order_OriInt[21] = i + 1;
			break;

		case TYPE_DEFI6:
			m_Def_Check[5].SetCheck(TRUE);
			m_Order_Text[22].ShowWindow(TRUE);
			m_Order_Text[22].SetWindowText(s);
			m_Order_Text_Show[22] = TRUE;
			m_Order_OriInt[22] = i + 1;
			break;

		case TYPE_DEFI7:
			m_Def_Check[6].SetCheck(TRUE);
			m_Order_Text[23].ShowWindow(TRUE);
			m_Order_Text[23].SetWindowText(s);
			m_Order_Text_Show[23] = TRUE;
			m_Order_OriInt[23] = i + 1;
			break;

		case TYPE_DEFI8:
			m_Def_Check[7].SetCheck(TRUE);
			m_Order_Text[24].ShowWindow(TRUE);
			m_Order_Text[24].SetWindowText(s);
			m_Order_Text_Show[24] = TRUE;
			m_Order_OriInt[24] = i + 1;
			break;

		case TYPE_WCOU:
			m_Wcou_Check.SetCheck(TRUE);
			m_Order_Text[14].ShowWindow(TRUE);
			m_Order_Text[14].SetWindowText(s);
			m_Order_Text_Show[14] = TRUE;
			m_Order_OriInt[14] = i + 1;
			break;

		case TYPE_REFA:
			m_Wfra_Check.SetCheck(TRUE);
			m_Order_Text[0].ShowWindow(TRUE);
			m_Order_Text[0].SetWindowText(s);
			m_Order_Text_Show[0] = TRUE;
			m_Order_OriInt[0] = i + 1;
			break;

		case TYPE_ABEA:
			m_Wfaa_Check.SetCheck(TRUE);
			m_Order_Text[1].ShowWindow(TRUE);
			m_Order_Text[1].SetWindowText(s);
			m_Order_Text_Show[1] = TRUE;
			m_Order_OriInt[1] = i + 1;
			break;

		case TYPE_WCVA:
			m_Wcva_Check.SetCheck(TRUE);
			m_Order_Text[13].ShowWindow(TRUE);
			m_Order_Text[13].SetWindowText(s);
			m_Order_Text_Show[13] = TRUE;
			m_Order_OriInt[13] = i + 1;
			break;

		case TYPE_CTSU:
			m_Ctsu_Check.SetCheck(TRUE);
			m_Order_Text[8].ShowWindow(TRUE);
			m_Order_Text[8].SetWindowText(s);
			m_Order_Text_Show[8] = TRUE;
			m_Order_OriInt[8] = i + 1;
			break;

		case TYPE_VFA:
			m_Vfa_Check.SetCheck(TRUE);
			m_Order_Text[2].ShowWindow(TRUE);
			m_Order_Text[2].SetWindowText(s);
			m_Order_Text_Show[2] = TRUE;
			m_Order_OriInt[2] = i + 1;
			break;

		case TYPE_KERA:
			m_Ctke_Check.SetCheck(TRUE);
			m_Order_Text[9].ShowWindow(TRUE);
			m_Order_Text[9].SetWindowText(s);
			m_Order_Text_Show[9] = TRUE;
			m_Order_OriInt[9] = i + 1;
			break;

		case TYPE_CHAN:
			m_Wcch_Check.SetCheck(TRUE);
			m_Order_Text[11].ShowWindow(TRUE);
			m_Order_Text[11].SetWindowText(s);
			m_Order_Text_Show[11] = TRUE;
			m_Order_OriInt[11] = i + 1;
			break;

		case TYPE_IOLS:
			m_Wciol_Check.SetCheck(TRUE);
			m_Order_Text[15].ShowWindow(TRUE);
			m_Order_Text[15].SetWindowText(s);
			m_Order_Text_Show[15] = TRUE;
			m_Order_OriInt[15] = i + 1;
			break;

		case TYPE_ANGK://Angle K/A
			m_Wfka_Check.SetCheck(TRUE);
			m_Order_Text[5].ShowWindow(TRUE);
			m_Order_Text[5].SetWindowText(s);
			m_Order_Text_Show[5] = TRUE;
			m_Order_OriInt[5] = i + 1;
			break;

		case TYPE_TORI:
			m_Tor_Check.SetCheck(TRUE);
			m_Order_Text[16].ShowWindow(TRUE);
			m_Order_Text[16].SetWindowText(s);
			m_Order_Text_Show[16] = TRUE;
			m_Order_OriInt[16] = i + 1;

			break;

		case TYPE_ACCO:
			m_Wfac_Check.SetCheck(TRUE);
			m_Order_Text[6].ShowWindow(TRUE);
			m_Order_Text[6].SetWindowText(s);
			m_Order_Text_Show[6] = TRUE;
			m_Order_OriInt[6] = i + 1;
			break;

			//6.2.0 Near Vision
		case TYPE_NEVI:
			m_Wfnv_Check.SetCheck(TRUE);
			m_Order_Text[31].ShowWindow(TRUE);
			m_Order_Text[31].SetWindowText(s);
			m_Order_Text_Show[31] = TRUE;
			m_Order_OriInt[31] = i + 1;
			break;
			//6.2.0 Near Vision

		case TYPE_WFVA:
			m_Wfva_Check.SetCheck(TRUE);
			m_Order_Text[3].ShowWindow(TRUE);
			m_Order_Text[3].SetWindowText(s);
			m_Order_Text_Show[3] = TRUE;
			m_Order_OriInt[3] = i + 1;
			break;

		case TYPE_WFRM:
			m_Wfrms_Check.SetCheck(TRUE);
			m_Order_Text[4].ShowWindow(TRUE);
			m_Order_Text[4].SetWindowText(s);
			m_Order_Text_Show[4] = TRUE;
			m_Order_OriInt[4] = i + 1;
			break;

		case TYPE_CTDS:
			m_Ctds_Check.SetCheck(TRUE);
			m_Order_Text[7].ShowWindow(TRUE);
			m_Order_Text[7].SetWindowText(s);
			m_Order_Text_Show[7] = TRUE;
			m_Order_OriInt[7] = i + 1;
			break;

		case TYPE_CT3D:
			m_Ct3d_Check.SetCheck(TRUE);
			m_Order_Text[10].ShowWindow(TRUE);
			m_Order_Text[10].SetWindowText(s);
			m_Order_Text_Show[10] = TRUE;
			m_Order_OriInt[10] = i + 1;
			break;

		case TYPE_WCSU:
			m_Wcsu_Check.SetCheck(TRUE);
			m_Order_Text[12].ShowWindow(TRUE);
			m_Order_Text[12].SetWindowText(s);
			m_Order_Text_Show[12] = TRUE;
			m_Order_OriInt[12] = i + 1;
			break;

			//n
		case TYPE_CTCM:
			m_Ctcm_Check.SetCheck(TRUE);
			m_Order_Text[25].ShowWindow(TRUE);
			m_Order_Text[25].SetWindowText(s);
			m_Order_Text_Show[25] = TRUE;
			m_Order_OriInt[25] = i + 1;
			break;

		case TYPE_DYSF:
			m_Dysfun_Check.SetCheck(TRUE);
			m_Order_Text[26].ShowWindow(TRUE);
			m_Order_Text[26].SetWindowText(s);
			m_Order_Text_Show[26] = TRUE;
			m_Order_OriInt[26] = i + 1;
			break;

		case TYPE_ASTI:
			m_AstiSou_Check.SetCheck(TRUE);
			m_Order_Text[27].ShowWindow(TRUE);
			m_Order_Text[27].SetWindowText(s);
			m_Order_Text_Show[27] = TRUE;
			m_Order_OriInt[27] = i + 1;
			break;

			// Dysfunctional lens analysis patient
		case TYPE_DYSP:
			m_Dysfun_Pt_Check.SetCheck(TRUE);
			m_Order_Text[28].ShowWindow(TRUE);
			m_Order_Text[28].SetWindowText(s);
			m_Order_Text_Show[28] = TRUE;
			m_Order_OriInt[28] = i + 1;
			break;

			// Depth of focus
		case TYPE_DEPF:
			m_DepthFocus_Check.SetCheck(TRUE);
			m_Order_Text[29].ShowWindow(TRUE);
			m_Order_Text[29].SetWindowText(s);
			m_Order_Text_Show[29] = TRUE;
			m_Order_OriInt[29] = i + 1;
			break;

			//Toric alignment
		case TYPE_TOAL:
			m_ToricAli_Check.SetCheck(TRUE);
			m_Order_Text[30].ShowWindow(TRUE);
			m_Order_Text[30].SetWindowText(s);
			m_Order_Text_Show[30] = TRUE;
			m_Order_OriInt[30] = i + 1;
			break;
		}
	}

	s.Format(_T("%i"), m_t);
	m_sn_Text.SetWindowText(s);

	SetDlgAble();
}


//***************************************************************************************

int CTaskSettinigComboDlg::GetNStr(int n)
{
	CString s;
	m_Order_Text[n].GetWindowText(s);
	G_As = s;
	int num = atoi(G_As);

	return num;
}

//***************************************************************************************
CString CTaskSettinigComboDlg::GetStrN(int n)
{
	CString s;

	if (n == 0) s = " 01 ";
	else if (n == 1) s = " 02 ";
	else if (n == 2) s = " 03 ";
	else if (n == 3) s = " 04 ";
	else if (n == 4) s = " 05 ";
	else if (n == 5) s = " 06 ";
	else if (n == 6) s = " 07 ";
	else if (n == 7) s = " 08 ";

	return s;
}

//***************************************************************************************

void CTaskSettinigComboDlg::OnEnableClicked()
{
	if (m_Enable) return;
	else
	{
		m_Enable = TRUE;
		SetDlgAble();
	}
}

//***************************************************************************************

void CTaskSettinigComboDlg::OnDisableClicked()
{
	if (!m_Enable) return;
	else
	{
		m_Enable = FALSE;
		SetDlgAble();
	}
}

//***************************************************************************************

void CTaskSettinigComboDlg::SetDlgAble()
{
	m_TaskName_Edit.EnableWindow(m_Enable);

	m_Wfra_Check.EnableWindow(m_Enable);
	m_Wfaa_Check.EnableWindow(m_Enable);
	m_Vfa_Check.EnableWindow(m_Enable);
	m_Wfva_Check.EnableWindow(m_Enable);
	m_Wfrms_Check.EnableWindow(m_Enable);
	m_Wfka_Check.EnableWindow(m_Enable);
	m_Wfac_Check.EnableWindow(m_Enable);
	m_Wfnv_Check.EnableWindow(m_Enable);//6.2.0 Near vision

	m_Ctds_Check.EnableWindow(m_Enable);
	m_Ctsu_Check.EnableWindow(m_Enable);
	m_Ctke_Check.EnableWindow(m_Enable);
	m_Ct3d_Check.EnableWindow(m_Enable);
	m_Ctcm_Check.EnableWindow(m_Enable);//n

	m_Wcch_Check.EnableWindow(m_Enable);
	m_Wcsu_Check.EnableWindow(m_Enable);
	m_Wcva_Check.EnableWindow(m_Enable);
	m_Wcou_Check.EnableWindow(m_Enable);
	m_Wciol_Check.EnableWindow(m_Enable);
	m_Tor_Check.EnableWindow(m_Enable);

	m_Dysfun_Check.EnableWindow(m_Enable);
	m_Dysfun_Pt_Check.EnableWindow(m_Enable);
	m_AstiSou_Check.EnableWindow(m_Enable);
	m_DepthFocus_Check.EnableWindow(m_Enable);
	m_ToricAli_Check.EnableWindow(m_Enable);

	Invalidate(FALSE);
}

//***************************************************************************************

void CTaskSettinigComboDlg::OnLButtonDown(uint nFlags, CPoint Point)
{
	if (!m_Enable) return;

	int whichCheck = -1;
	CRect rect;
	CPoint point, OriPoint;

	GetCursorPos(&point);

	//
	OriPoint = point;
	m_Wfra_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_Wfra_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_Wfra_Check.SetCheck(!m_Wfra_Check.GetCheck());
		OnWfraClicked();
		return;
	}
	//
	OriPoint = point;
	m_Wfaa_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_Wfaa_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_Wfaa_Check.SetCheck(!m_Wfaa_Check.GetCheck());
		OnWfaaClicked();
		return;
	}
	//
	OriPoint = point;
	m_Vfa_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_Vfa_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_Vfa_Check.SetCheck(!m_Vfa_Check.GetCheck());
		OnVfaClicked();
		return;
	}
	//
	OriPoint = point;
	m_Wfva_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_Wfva_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_Wfva_Check.SetCheck(!m_Wfva_Check.GetCheck());
		OnWfvaClicked();
		return;
	}
	//
	OriPoint = point;
	m_Wfrms_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_Wfrms_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_Wfrms_Check.SetCheck(!m_Wfrms_Check.GetCheck());
		OnWfrmsClicked();
		return;
	}
	//
	OriPoint = point;
	m_Wfka_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_Wfka_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_Wfka_Check.SetCheck(!m_Wfka_Check.GetCheck());
		OnWfkaClicked();
		return;
	}
	//
	OriPoint = point;
	m_Wfac_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_Wfac_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_Wfac_Check.SetCheck(!m_Wfac_Check.GetCheck());
		OnWfacClicked();
		return;
	}

	//6.2.0 Near Vision
	OriPoint = point;
	m_Wfnv_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_Wfnv_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_Wfnv_Check.SetCheck(!m_Wfnv_Check.GetCheck());
		OnWfnvClicked();
		return;
	}
	//6.2.0 Near Vision

	OriPoint = point;
	m_Ctds_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_Ctds_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_Ctds_Check.SetCheck(!m_Ctds_Check.GetCheck());
		OnCtdsClicked();
		return;
	}
	//
	OriPoint = point;
	m_Ctsu_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_Ctsu_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_Ctsu_Check.SetCheck(!m_Ctsu_Check.GetCheck());
		OnCtsuClicked();
		return;
	}
	//
	OriPoint = point;
	m_Ctke_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_Ctke_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_Ctke_Check.SetCheck(!m_Ctke_Check.GetCheck());
		OnCtkeClicked();
		return;
	}
	//
	OriPoint = point;
	m_Ct3d_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_Ct3d_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_Ct3d_Check.SetCheck(!m_Ct3d_Check.GetCheck());
		OnCt3DClicked();
		return;
	}
	//
	OriPoint = point;
	m_Ctcm_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_Ctcm_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_Ctcm_Check.SetCheck(!m_Ctcm_Check.GetCheck());
		OnCtcmClicked();
		return;
	}
	//
	OriPoint = point;
	m_Wcch_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_Wcch_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_Wcch_Check.SetCheck(!m_Wcch_Check.GetCheck());
		OnWcchClicked();
		return;
	}
	//
	OriPoint = point;
	m_Wcsu_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_Wcsu_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_Wcsu_Check.SetCheck(!m_Wcsu_Check.GetCheck());
		OnWcsuClicked();
		return;
	}
	//
	OriPoint = point;
	m_Wcva_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_Wcva_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_Wcva_Check.SetCheck(!m_Wcva_Check.GetCheck());
		OnWcvaClicked();
		return;
	}
	//
	OriPoint = point;
	m_Wcou_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_Wcou_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_Wcou_Check.SetCheck(!m_Wcou_Check.GetCheck());
		OnWcouClicked();
		return;
	}
	//
	OriPoint = point;
	m_Wciol_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_Wciol_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_Wciol_Check.SetCheck(!m_Wciol_Check.GetCheck());
		OnWciolClicked();
		return;
	}

	OriPoint = point;
	m_DysFun_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_DysFun_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_Dysfun_Check.SetCheck(!m_Dysfun_Check.GetCheck());
		OnDysfunClicked();
		return;
	}

	OriPoint = point;
	m_DysFun_Pt_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_DysFun_Pt_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_Dysfun_Pt_Check.SetCheck(!m_Dysfun_Pt_Check.GetCheck());
		OnDysPtfunClicked();
		return;
	}

	OriPoint = point;
	m_ToricAli_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_ToricAli_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_ToricAli_Check.SetCheck(!m_ToricAli_Check.GetCheck());
		OnTorAliClicked();
		return;
	}

	OriPoint = point;
	m_Astisou_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_Astisou_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_AstiSou_Check.SetCheck(!m_AstiSou_Check.GetCheck());
		OnAstiSoufunClicked();
		return;
	}

	OriPoint = point;
	m_DepthFocus_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_DepthFocus_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_DepthFocus_Check.SetCheck(!m_DepthFocus_Check.GetCheck());
		OnDepthFocusClicked();
		return;
	}

	OriPoint = point;
	m_Tor_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_Tor_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_Tor_Check.SetCheck(!m_Tor_Check.GetCheck());
		OnTorClicked();
		return;
	}

	//Following click order text
	for (int i = 0; i < 32; i++)
	{
		if (m_Order_Text_Show[i])
		{
			OriPoint = point;
			m_Order_Text[i].ScreenToClient(&OriPoint);
			::GetClientRect(m_Order_Text[i].GetSafeHwnd(), &rect);
			rect.left -= 5;

			if (rect.PtInRect(OriPoint))
			{
				m_Changed = TRUE;

				if (m_LastSort != -1 && i != m_LastSort)
				{
					CreateOriOrder();
				}

				m_Order_Int[i] += 1;
				if (m_Order_Int[i] > m_t) m_Order_Int[i] = 1;
				m_Order_Text[i].SetWindowText(GetStrN(m_Order_Int[i] - 1));

				for (int j = 0; j < 32; j++)
				{
					if (m_Order_Text_Show[j])
					{
						if (j != i)
						{
							int otherN = GetNStr(j);
							int oriN = m_Order_OriInt[i];

							if (otherN == m_Order_Int[i] && otherN != m_Order_OriInt[i])
							{
								CString s = GetStrN(m_Order_OriInt[i] - 1);
								m_Order_Text[j].SetWindowText(s);
							}
							else
							{
								CString s = GetStrN(m_Order_OriInt[j] - 1);
								m_Order_Text[j].SetWindowText(s);
							}
						}
					}
				}

				m_LastSort = i;
				return;
			}
		}
	}
	//Done
}

//********************************************************************************

void CTaskSettinigComboDlg::CreateOriOrder()
{
	for (int i = 0; i < 32; i++)
	{
		m_Order_Int[i] = 0;
		m_Order_OriInt[i] = 0;
		if (m_Order_Text_Show[i])
		{
			m_Order_OriInt[i] = GetNStr(i);
		}
	}
}
//********************************************************************************

void CTaskSettinigComboDlg::GetDlg()
{
	CString s;

	m_TaskName_Edit.GetWindowText(s);
	m_SettedTask.SaveBtnName(s);// For Chinese

	m_SettedTask.m_DefNum = 0;
	m_SettedTask.m_SubBtnNum = m_t;

	m_SettedTask.m_Enable = m_TaskEn_Radio.GetCheck();

	for (int i = 0; i < 8; i++)
	{
		m_SettedTask.m_SelfSettings[i] = FALSE;
		m_SettedTask.m_SubButton[i] = TYPE_NONA;
	}

	int t = 0;

	//WF
	if (m_Wfra_Check.GetCheck()) //Refraction Analysis
	{
		t = GetNStr(0) - 1;

		m_SettedTask.m_SubButton[t] = TYPE_REFA;
		m_SettedTask.m_SelfSettings[t] = FALSE;
	}

	if (m_Wfaa_Check.GetCheck()) //Aberration Analysis
	{
		t = GetNStr(1) - 1;

		m_SettedTask.m_SubButton[t] = TYPE_ABEA;
		m_SettedTask.m_SelfSettings[t] = FALSE;
	}

	if (m_Vfa_Check.GetCheck()) //VFA
	{
		t = GetNStr(2) - 1;

		m_SettedTask.m_SubButton[t] = TYPE_VFA;
		m_SettedTask.m_SelfSettings[t] = FALSE;
	}

	if (m_Wfva_Check.GetCheck()) //WF Visual Acuity
	{
		t = GetNStr(3) - 1;

		m_SettedTask.m_SubButton[t] = TYPE_WFVA;
		m_SettedTask.m_SelfSettings[t] = FALSE;
	}

	if (m_Wfrms_Check.GetCheck()) //WF/RMS
	{
		t = GetNStr(4) - 1;

		m_SettedTask.m_SubButton[t] = TYPE_WFRM;
		m_SettedTask.m_SelfSettings[t] = FALSE;
	}

	if (m_Wfka_Check.GetCheck()) //Angle K/A
	{
		t = GetNStr(5) - 1;

		m_SettedTask.m_SubButton[t] = TYPE_ANGK;
		m_SettedTask.m_SelfSettings[t] = FALSE;
	}

	if (m_Wfac_Check.GetCheck()) //Accomm and DOF Analysis?
	{
		t = GetNStr(6) - 1;

		m_SettedTask.m_SubButton[t] = TYPE_ACCO;
		m_SettedTask.m_SelfSettings[t] = FALSE;
	}

	if (m_Wfnv_Check.GetCheck()) //6.2.0 Near Vision
	{
		t = GetNStr(31) - 1;

		m_SettedTask.m_SubButton[t] = TYPE_NEVI;
		m_SettedTask.m_SelfSettings[t] = FALSE;
	}
	//WF Done

	//CT
	if (m_Ctds_Check.GetCheck()) //CT OD/OS
	{
		t = GetNStr(7) - 1;

		m_SettedTask.m_SubButton[t] = TYPE_CTDS;
		m_SettedTask.m_SelfSettings[t] = FALSE;
	}

	if (m_Ctsu_Check.GetCheck()) //CT Summary
	{
		t = GetNStr(8) - 1;

		m_SettedTask.m_SubButton[t] = TYPE_CTSU;
		m_SettedTask.m_SelfSettings[t] = FALSE;
	}

	if (m_Ctke_Check.GetCheck()) //Keratometry
	{
		t = GetNStr(9) - 1;

		m_SettedTask.m_SubButton[t] = TYPE_KERA;
		m_SettedTask.m_SelfSettings[t] = FALSE;
	}

	if (m_Ct3d_Check.GetCheck()) //3D Z-Elevation
	{
		t = GetNStr(10) - 1;

		m_SettedTask.m_SubButton[t] = TYPE_CT3D;
		m_SettedTask.m_SelfSettings[t] = FALSE;
	}

	//n
	if (m_Ctcm_Check.GetCheck()) //CT Compare
	{
		t = GetNStr(25) - 1;

		m_SettedTask.m_SubButton[t] = TYPE_CTCM;
		m_SettedTask.m_SelfSettings[t] = FALSE;
	}
	//CT Done

	//WF/CT
	if (m_Wcch_Check.GetCheck()) //Chang Analysis
	{
		t = GetNStr(11) - 1;

		m_SettedTask.m_SubButton[t] = TYPE_CHAN;
		m_SettedTask.m_SelfSettings[t] = FALSE;
	}

	if (m_Wcsu_Check.GetCheck()) //WF/CT Summary
	{
		t = GetNStr(12) - 1;

		m_SettedTask.m_SubButton[t] = TYPE_WCSU;
		m_SettedTask.m_SelfSettings[t] = FALSE;
	}

	if (m_Wcva_Check.GetCheck()) //WF/CT Visual Acuity
	{
		t = GetNStr(13) - 1;

		m_SettedTask.m_SubButton[t] = TYPE_WCVA;
		m_SettedTask.m_SelfSettings[t] = FALSE;
	}

	if (m_Wcou_Check.GetCheck()) //OU Overview
	{
		t = GetNStr(14) - 1;

		m_SettedTask.m_SubButton[t] = TYPE_WCOU;
		m_SettedTask.m_SelfSettings[t] = FALSE;
	}

	if (m_Wciol_Check.GetCheck()) //IOL Selection Assistant
	{
		t = GetNStr(15) - 1;

		m_SettedTask.m_SubButton[t] = TYPE_IOLS;
		m_SettedTask.m_SelfSettings[t] = FALSE;
	}

	if (m_Dysfun_Check.GetCheck()) //Dysfunctional Lens Analysis
	{
		t = GetNStr(26) - 1;

		m_SettedTask.m_SubButton[t] = TYPE_DYSF;
		m_SettedTask.m_SelfSettings[t] = FALSE;
	}

	if (m_Dysfun_Pt_Check.GetCheck()) //Dysfunctional Lens Analysis
	{
		t = GetNStr(28) - 1;

		m_SettedTask.m_SubButton[t] = TYPE_DYSP;
		m_SettedTask.m_SelfSettings[t] = FALSE;
	}

	if (m_AstiSou_Check.GetCheck()) //Astigmatism source
	{
		t = GetNStr(27) - 1;

		m_SettedTask.m_SubButton[t] = TYPE_ASTI;
		m_SettedTask.m_SelfSettings[t] = FALSE;
	}

	if (m_DepthFocus_Check.GetCheck()) //Depth of focus
	{
		t = GetNStr(29) - 1;

		m_SettedTask.m_SubButton[t] = TYPE_DEPF;
		m_SettedTask.m_SelfSettings[t] = FALSE;
	}

	if (m_ToricAli_Check.GetCheck()) //Toric Alignment
	{
		t = GetNStr(30) - 1;

		m_SettedTask.m_SubButton[t] = TYPE_TOAL;
		m_SettedTask.m_SelfSettings[t] = FALSE;
	}
	//WF/CT Done

	//Toric
	if (m_Tor_Check.GetCheck()) //Toric Planner
	{
		t = GetNStr(16) - 1;

		m_SettedTask.m_SubButton[t] = TYPE_TORI;
		m_SettedTask.m_SelfSettings[t] = FALSE;
	}
	//Toric Done

	//User Define
	for (int i = 0; i < 8; i++)
	{
		if (m_Def_Check[i].GetCheck()) //IOL Selection Assistant
		{
			t = GetNStr(i + 17) - 1;

			m_SettedTask.m_SubButton[t] = TYPE_NONA;
			m_SettedTask.m_SelfSettings[t] = FALSE;
		}
	}
	//Done	
}

//********************************************************************************

BOOL CTaskSettinigComboDlg::CheckDefEdit()
{
	CString s;
	for (int i = 0; i < 8; i++)
	{
		if (m_Def_Check[i].GetCheck())
		{
			m_Def_Edit[i].GetWindowText(s);
			if (s == "")
			{
				::Info("The selected definition display name can not be empty.");
				m_Def_Edit[i].SetFocus();
				return FALSE;
			}
		}
	}
	return TRUE;
}

//********************************************************************************
CTaskSetting CTaskSettinigComboDlg::SetDefTask1()
{
	CTaskSetting DefTask;

	DefTask.m_Enable = TRUE;
	DefTask.m_DefNum = 0;

	for (int j = 0; j < 10; j++) DefTask.m_SelfSettings[j] = FALSE;

	CString Def_Name = "Vision Diagnostics";
	int     Def_Num = 6;

	DefTask.m_SubButton[0] = TYPE_WCOU;
	DefTask.m_SubButton[1] = TYPE_REFA;
	DefTask.m_SubButton[2] = TYPE_ABEA;
	DefTask.m_SubButton[3] = TYPE_WCVA;
	DefTask.m_SubButton[4] = TYPE_CTSU;
	DefTask.m_SubButton[5] = TYPE_VFA;
	DefTask.m_SubButton[6] = TYPE_NONA;
	DefTask.m_SubButton[7] = TYPE_NONA;
	DefTask.m_SubButton[8] = TYPE_NONA;
	DefTask.m_SubButton[9] = TYPE_NONA;

	G_As = Def_Name;
	strncpy(&DefTask.m_ButtonName[0], G_As, 25);

	DefTask.m_SubBtnNum = Def_Num;

	return DefTask;
}
//********************************************************************************************
CTaskSetting CTaskSettinigComboDlg::SetDefTask2()
{
	CTaskSetting DefTask;

	DefTask.m_Enable = TRUE;
	DefTask.m_DefNum = 0;

	for (int j = 0; j < 10; j++) DefTask.m_SelfSettings[j] = FALSE;

	CString Def_Name = "Corneal Diagnostics";
	int     Def_Num = 4;

	DefTask.m_SubButton[0] = TYPE_CTSU;
	DefTask.m_SubButton[1] = TYPE_KERA;
	DefTask.m_SubButton[2] = TYPE_WCVA;
	DefTask.m_SubButton[3] = TYPE_CHAN;
	DefTask.m_SubButton[4] = TYPE_NONA;
	DefTask.m_SubButton[5] = TYPE_NONA;
	DefTask.m_SubButton[6] = TYPE_NONA;
	DefTask.m_SubButton[7] = TYPE_NONA;
	DefTask.m_SubButton[8] = TYPE_NONA;
	DefTask.m_SubButton[9] = TYPE_NONA;

	G_As = Def_Name;
	strncpy(&DefTask.m_ButtonName[0], G_As, 25);

	DefTask.m_SubBtnNum = Def_Num;

	return DefTask;
}
//********************************************************************************
CTaskSetting CTaskSettinigComboDlg::SetDefTask0()
{
	CTaskSetting DefTask;

	DefTask.m_Enable = TRUE;
	DefTask.m_DefNum = 0;

	for (int j = 0; j < 10; j++) DefTask.m_SelfSettings[j] = FALSE;

	CString Def_Name = "Cataract Evaluation";

	int     Def_Num = 5;//

						// Change the default settings
	DefTask.m_SubButton[0] = TYPE_DYSP;
	DefTask.m_SubButton[1] = TYPE_CHAN;
	DefTask.m_SubButton[2] = TYPE_ANGK;
	DefTask.m_SubButton[3] = TYPE_TOAL;
	DefTask.m_SubButton[4] = TYPE_TORI;
	DefTask.m_SubButton[5] = TYPE_NONA;
	DefTask.m_SubButton[6] = TYPE_NONA;
	DefTask.m_SubButton[7] = TYPE_NONA;
	DefTask.m_SubButton[8] = TYPE_NONA;
	DefTask.m_SubButton[9] = TYPE_NONA;

	G_As = Def_Name;
	strncpy(&DefTask.m_ButtonName[0], G_As, 25);

	DefTask.m_SubBtnNum = Def_Num;

	return DefTask;
}
//********************************************************************************
CTaskSetting CTaskSettinigComboDlg::SetDefTask3()
{
	CTaskSetting DefTask;

	DefTask.m_Enable = TRUE;
	DefTask.m_DefNum = 0;

	for (int j = 0; j < 10; j++) DefTask.m_SelfSettings[j] = FALSE;

	CString Def_Name = "Toric Planner";
	int     Def_Num = 1;

	DefTask.m_SubButton[0] = TYPE_TORI;
	for (int j = 1; j < 10; j++) DefTask.m_SubButton[j] = TYPE_NONA;

	G_As = Def_Name;
	strncpy(&DefTask.m_ButtonName[0], G_As, 25);
	DefTask.m_SubBtnNum = Def_Num;

	return DefTask;
}
//********************************************************************************
CTaskSetting CTaskSettinigComboDlg::SetDefTask4()
{
	CTaskSetting DefTask;

	DefTask.m_Enable = TRUE;
	DefTask.m_DefNum = 0;

	for (int j = 0; j < 10; j++) DefTask.m_SelfSettings[j] = FALSE;

	CString Def_Name = "Accomm and DOF Analysis";
	int     Def_Num = 2;

	DefTask.m_SubButton[0] = TYPE_ACCO;
	DefTask.m_SubButton[1] = TYPE_DEPF;
	for (int j = 2; j < 10; j++) DefTask.m_SubButton[j] = TYPE_NONA;

	G_As = Def_Name;
	strncpy(&DefTask.m_ButtonName[0], G_As, 25);

	DefTask.m_SubBtnNum = Def_Num;

	return DefTask;
}

//********************************************************************************


