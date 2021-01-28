//***************************************************************************************
//530 for a new quick wf capture dlg for technician
#include "StdAfx.h"
#include "Resource.h"
#include "QuickWFDlg.h"
#include "WFAcquisition.h"//520
#include "WFAcquisitionDlg.h"//520
#include "BusyCursor.h"//

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CQuickWFDlg, CDialog)

	ON_BN_CLICKED(IDQUICKWF, OnQuickWFBtnClicked)
	ON_BN_CLICKED(IDSAVE, OnSaveBtnClicked)
	ON_BN_CLICKED(IDDELETION, OnDeleteBtnClicked)
	ON_BN_CLICKED(IDPRINT, OnPrintBtnClicked)
	ON_BN_CLICKED(IDC_OPTOMETER_CHECK, OnOptometerCheckClicked)

	ON_BN_CLICKED(IDC_OPTOMETER_CHECK, OnPrintBtnClicked)
	ON_NOTIFY(NM_DBLCLK, IDC_WFRES_LIST, OnListDblClicked)

	ON_BN_CLICKED(IDC_CAL_SAVE, OnCalSaveBtnClicked)//

END_MESSAGE_MAP()

//***************************************************************************************

CQuickWFDlg::CQuickWFDlg(CHW* pHW, CALIBRATION* pCalibration, CWnd* pParentWnd) : CDialog(IDD_QUICKWF_DLG, pParentWnd)
{
	m_pHW = pHW;
	m_pCalibration = pCalibration;
}

//***************************************************************************************

void CQuickWFDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDQUICKWF, m_QuickWFBtn);
	DDX_Control(pDX, IDSAVE, m_SaveWFBtn);
	DDX_Control(pDX, IDDELETION, m_DeleteWFBtn);
	DDX_Control(pDX, IDPRINT, m_PrintBtn);

	DDX_Control(pDX, IDC_WF_VERTEX_COMBOBOX, m_VertexComboBox);
	DDX_Control(pDX, IDC_WF_ZONE_0_COMBOBOX, m_ZonesComboBox);
	DDX_Control(pDX, IDC_ROUND_1_8_RADIO, m_Round8Radio);
	DDX_Control(pDX, IDC_ROUND_1_100_RADIO, m_Round100Radio);
	DDX_Control(pDX, IDC_WF_SCAN_D_EDIT, m_ScanDiameterEdit);

	DDX_Control(pDX, IDC_ZERNIKE_C3_EDIT, m_C3Edit);
	DDX_Control(pDX, IDC_ZERNIKE_C4_EDIT, m_C4Edit);
	DDX_Control(pDX, IDC_ZERNIKE_C5_EDIT, m_C5Edit);

	DDX_Control(pDX, IDC_CAL_EXP_TIME_EDIT, m_LADExpTimeEdit);
	DDX_Control(pDX, IDC_CAL_SWAP_LADS_CHECK, m_SwapLADsCheck);
	DDX_Control(pDX, IDC_CAL_FLIP_LADX_CHECK, m_FlipLADXCheck);
	DDX_Control(pDX, IDC_CAL_F_X_EDIT, m_LADXFocalDistanceEdit);
	DDX_Control(pDX, IDC_CAL_FLIP_LADY_CHECK, m_FlipLADYCheck);
	DDX_Control(pDX, IDC_CAL_F_Y_EDIT, m_LADYFocalDistanceEdit);

	DDX_Control(pDX, IDC_OPTOMETER_CHECK, m_OptometerCheck);

	DDX_Control(pDX, IDC_RES_TEXT, m_ResText);
	DDX_Control(pDX, IDC_OPT_ENABLE_TEXT, m_Opt_Enable_Text);

	DDX_Control(pDX, IDC_CAL_SAVE, m_CalSaveButton);//

}

//***************************************************************************************

BOOL CQuickWFDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_WFClick = FALSE;

	RECT Rect;
	GetClientRect(&Rect);
	m_w = Rect.right - Rect.left;
	m_h = Rect.bottom - Rect.top;

	CString s;

	//Vertex
	for (real_t v = 0.0; v < 20.001; v += 0.5) {
		s.Format(_T("%.1f"), v);
		m_VertexComboBox.AddString(s);
	}
	s.Format(_T("%.1f"), 0.0);
	m_VertexComboBox.SelectString(-1, s);
	//Done

	//Zone
	for (real_t d = 2.0; d < 8.001; d += 0.1) {
		s.Format(_T("%.1f"), d);
		m_ZonesComboBox.AddString(s);
	}
	s.Format(_T("%.1f"), 4.0);//
	m_ZonesComboBox.SelectString(-1, s);
	//Done

	m_Round100Radio.SetCheck(TRUE);

	m_ScanDiameterEdit.SetWindowText(_T("6.0"));

	m_ScanDiameterEdit.GetWindowText(s);

	// Zernike C3
	s.Format(_T("%.3f"), m_pCalibration->Zernike.C3);
	m_C3Edit.SetWindowText(s);


	// Zernike C4
	s.Format(_T("%.3f"), m_pCalibration->Zernike.C4);
	m_C4Edit.SetWindowText(s);

	// Zernike C5
	s.Format(_T("%.3f"), m_pCalibration->Zernike.C5);
	m_C5Edit.SetWindowText(s);

	// LADs exposure time
	s.Format(_T("%i"), m_pCalibration->LADExposureTime);
	m_LADExpTimeEdit.SetWindowText(s);

	// LADs swap
	m_SwapLADsCheck.SetCheck(m_pCalibration->LADSwapXY);

	// LADX flip
	m_FlipLADXCheck.SetCheck(!m_pCalibration->LADXNoFlip);

	// LADX focal distance
	s.Format(_T("%.0f"), m_pCalibration->LADXFocalDistance);
	m_LADXFocalDistanceEdit.SetWindowText(s);

	// LADY flip
	m_FlipLADYCheck.SetCheck(!m_pCalibration->LADYNoFlip);

	// LADY focal distance
	s.Format(_T("%.0f"), m_pCalibration->LADYFocalDistance);
	m_LADYFocalDistanceEdit.SetWindowText(s);

	//Optometer enabled or not
	m_OptometerCheck.SetCheck(m_pCalibration->OptometerEnabled == 1 ? 1 : 0);

	//Result Text
	m_ResText.SetWindowText(_T(""));

	//Wf result list
	int cw[2];

	CMFont*   pFont = new CMFont();
	pFont->Create(intRound(0.8* (20)), 400, "Arial");

	::SetRect(&Rect, 20, intRound(0.2*m_h), intRound(0.65*m_w), intRound(0.9*m_h));
	cw[0] = intRound((0.65*m_w - 30)*0.05);
	cw[1] = intRound((0.65*m_w - 30)*0.95);

	m_WFResultList.SetFont(pFont);
	m_WFResultList.CreateWnd(WS_VISIBLE | LVS_SINGLESEL | LVS_NOSORTHEADER, Rect, this, IDC_WFRES_LIST);

	m_WFResultList.InsertColumn(0, _T("#"), intRound(cw[0]));
	m_WFResultList.InsertColumn(1, _T("Res"), intRound(cw[1]));

	m_count = 0;
	//Down

	//Backup releated Setting values
	m_Bk_VertexDistanceUm = ::Settings.m_VertexDistanceUm;
	for (int i = 0; i < 3; i++) {
		m_Bk_ZoneDiameterUm[i] = ::Settings.m_ZoneDiameterUm[i];
	}
	m_Bk_RefractionRounding = ::Settings.m_RefractionRounding;
	m_Bk_AutoScanDiameterUm = ::Settings.m_AutoScanDiameterUm;
	m_Bk_ManualScanDiameterUm = ::Settings.m_ManualScanDiameterUm;
	//Done

	//Backup releated Calibration values
	m_backC3 = m_pCalibration->Zernike.C3;
	m_backC4 = m_pCalibration->Zernike.C4;
	m_backC5 = m_pCalibration->Zernike.C5;
	m_backExposureTime = m_pCalibration->LADExposureTime;
	m_backLADSwapXY = m_pCalibration->LADSwapXY;
	m_backADXNoFlip = !m_pCalibration->LADXNoFlip;
	m_backLADXFocalDistance = m_pCalibration->LADXFocalDistance;
	m_backLADYNoFlip = !m_pCalibration->LADYNoFlip;
	m_backLADYFocalDistance = m_pCalibration->LADYFocalDistance;
	m_backOptometerEnabled = m_pCalibration->OptometerEnabled;
	//Done

	delete pFont;

	return FALSE;
}

//***************************************************************************************

BOOL CQuickWFDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == 0x51) OnQuickWFBtnClicked();
		if (pMsg->wParam == 0x53) OnSaveBtnClicked();
		if (pMsg->wParam == 0x44) OnDeleteBtnClicked();
		if (pMsg->wParam == 0x50) OnPrintBtnClicked();
	}

	if (pMsg->message == WM_KEYUP)
	{
		if ((GetFocus() == &m_ScanDiameterEdit))
		{
			CString DOB;
			m_ScanDiameterEdit.GetWindowText(DOB);

			BOOL IsNum = ::InputIsRealNum(DOB);

			if (!IsNum)
			{
				int len = DOB.GetLength();
				DOB = DOB.Left(len - 1);
				m_ScanDiameterEdit.SetWindowText(DOB);
				m_ScanDiameterEdit.SetSel(-1);
			}
		}

		if ((GetFocus() == &m_C3Edit))
		{
			CString DOB;
			m_C3Edit.GetWindowText(DOB);

			BOOL IsNum = ::InputIsRealNum(DOB);

			if (!IsNum)
			{
				int len = DOB.GetLength();
				DOB = DOB.Left(len - 1);
				m_C3Edit.SetWindowText(DOB);
				m_C3Edit.SetSel(-1);
			}
		}

		if ((GetFocus() == &m_C4Edit))
		{
			CString DOB;

			m_C4Edit.GetWindowText(DOB);
			BOOL IsNum = ::InputIsRealNum(DOB);

			if (!IsNum)
			{
				int len = DOB.GetLength();
				DOB = DOB.Left(len - 1);
				m_C4Edit.SetWindowText(DOB);
				m_C4Edit.SetSel(-1);
			}
		}

		if ((GetFocus() == &m_C5Edit))
		{
			CString DOB;
			m_C5Edit.GetWindowText(DOB);
			BOOL IsNum = ::InputIsRealNum(DOB);

			if (!IsNum)
			{
				int len = DOB.GetLength();
				DOB = DOB.Left(len - 1);
				m_C5Edit.SetWindowText(DOB);
				m_C5Edit.SetSel(-1);
			}
		}
	}

	return FALSE;
}

//***************************************************************************************

void CQuickWFDlg::OnOptometerCheckClicked()
{
	BOOL Checked = m_OptometerCheck.GetCheck();
	m_OptometerCheck.SetCheck(Checked);

	if (m_OptometerCheck.GetCheck()) m_Opt_Enable_Text.SetWindowText(_T("Enabled"));
	else m_Opt_Enable_Text.SetWindowText(_T("Disabled"));
}

//***************************************************************************************

void CQuickWFDlg::OnSaveBtnClicked()
{
	m_ResText.SetWindowText(_T(""));

	if (m_res[m_count] != "")
	{
		if (m_count == 300)
		{
			::Info("The saved wf tests are 300.");
			OnCancel();
		}

		CString s;
		s.Format(_T("%i"), m_count + 1);

		m_WFResultList.InsertItem(m_count, s);
		m_WFResultList.SetItemText(m_count, 1, m_res[m_count]);

		//Save the value of right panel
		m_Saved_VertexDistanceUm[m_count] = ::Settings.m_VertexDistanceUm;
		m_Saved_ZoneDiameterUm[m_count] = ::Settings.m_ZoneDiameterUm[0];
		m_Saved_RefractionRounding[m_count] = ::Settings.m_RefractionRounding;
		m_Saved_ScanDiameterUm[m_count] = ::Settings.m_AutoScanDiameterUm;

		m_Saved_C3[m_count] = m_pCalibration->Zernike.C3;
		m_Saved_C4[m_count] = m_pCalibration->Zernike.C4;
		m_Saved_C5[m_count] = m_pCalibration->Zernike.C5;
		m_Saved_ExposureTime[m_count] = m_pCalibration->LADExposureTime;
		m_Saved_LADSwapXY[m_count] = m_pCalibration->LADSwapXY;
		m_Saved_ADXNoFlip[m_count] = m_pCalibration->LADXNoFlip;
		m_Saved_LADXFocalDistance[m_count] = m_pCalibration->LADXFocalDistance;
		m_Saved_LADYNoFlip[m_count] = m_pCalibration->LADYNoFlip;
		m_Saved_LADYFocalDistance[m_count] = m_pCalibration->LADYFocalDistance;
		m_Saved_OptometerEnabled[m_count] = m_pCalibration->OptometerEnabled;
		//Done

		m_count++;
	}
}

//***************************************************************************************

void CQuickWFDlg::OnDeleteBtnClicked()
{
	if (m_count == 0) return;

	int selC = m_WFResultList.GetSelectedCount();
	if (selC != 0)
	{
		int p = m_WFResultList.GetSelectionMark();

		for (int i = p; i < m_count; i++)
		{
			m_res[i] = m_res[i + 1];

			m_Saved_VertexDistanceUm[i] = m_Saved_VertexDistanceUm[i + 1];
			m_Saved_ZoneDiameterUm[i] = m_Saved_ZoneDiameterUm[i + 1];
			m_Saved_RefractionRounding[i] = m_Saved_RefractionRounding[i + 1];
			m_Saved_ScanDiameterUm[i] = m_Saved_ScanDiameterUm[i + 1];

			m_Saved_C3[i] = m_Saved_C3[i + 1];
			m_Saved_C4[i] = m_Saved_C4[i + 1];
			m_Saved_C5[i] = m_Saved_C5[i + 1];
			m_Saved_ExposureTime[i] = m_Saved_ExposureTime[i + 1];
			m_Saved_LADSwapXY[i] = m_Saved_LADSwapXY[i + 1];
			m_Saved_ADXNoFlip[i] = m_Saved_ADXNoFlip[i + 1];
			m_Saved_LADXFocalDistance[i] = m_Saved_LADXFocalDistance[i + 1];
			m_Saved_LADYNoFlip[i] = m_Saved_LADYNoFlip[i + 1];
			m_Saved_LADYFocalDistance[i] = m_Saved_LADYFocalDistance[i + 1];
			m_Saved_OptometerEnabled[i] = m_Saved_OptometerEnabled[i + 1];
		}

		m_count--;

		m_WFResultList.DeleteAllItems();

		for (int i = 0; i < m_count; i++)
		{
			CString s;
			s.Format(_T("%i"), i + 1);
			m_WFResultList.InsertItem(i, s);
			m_WFResultList.SetItemText(i, 1, m_res[i]);
		}
	}



	//m_ResText.SetWindowText(_T(""));

	//if(m_res[m_count] != "")
	//{
	//	if(m_count == 300)
	//	{
	//		::Info("The saved wf tests are 300.");
	//           OnCancel();
	//	}

	//	CString s;
	//	s.Format(_T("%i"), m_count);

	//	//m_WFExamHeadersList.InsertItem(k1, s);
	//	m_WFResultList.InsertItem(m_count, s);
	//	m_WFResultList.SetItemText(m_count, 1, m_res[m_count]);

	//	//Save the value of right panel
	//	m_Saved_VertexDistanceUm[m_count] = ::Settings.m_VertexDistanceUm;
	//	m_Saved_ZoneDiameterUm[m_count] = ::Settings.m_ZoneDiameterUm[0];
	//	m_Saved_RefractionRounding[m_count] = ::Settings.m_RefractionRounding;
	//	m_Saved_ScanDiameterUm[m_count] = ::Settings.m_AutoScanDiameterUm;

	//	m_Saved_C3[m_count] = m_pCalibration->Zernike.C3;
	//	m_Saved_C4[m_count] = m_pCalibration->Zernike.C4;
	//	m_Saved_C5[m_count] = m_pCalibration->Zernike.C5;
	//	m_Saved_ExposureTime[m_count] = m_pCalibration->LADExposureTime;
	//	m_Saved_LADSwapXY[m_count] = m_pCalibration->LADSwapXY;
	//	m_Saved_ADXNoFlip[m_count] = m_pCalibration->LADXNoFlip;
	//	m_Saved_LADXFocalDistance[m_count] = m_pCalibration->LADXFocalDistance;
	//	m_Saved_LADYNoFlip[m_count] = m_pCalibration->LADYNoFlip; 
	//	m_Saved_LADYFocalDistance[m_count] = m_pCalibration->LADYFocalDistance ;
	//	m_Saved_OptometerEnabled[m_count] = m_pCalibration->OptometerEnabled;
	//	//Done

	//	m_count++;		
	//}
}
//***************************************************************************************
void CQuickWFDlg::OnQuickWFBtnClicked()
{
	m_WFClick = TRUE;

	CString s;

	//Settings
	//Vertex and zone
	m_VertexComboBox.GetLBText(m_VertexComboBox.GetCurSel(), s);
	G_As = s;
	::Settings.m_VertexDistanceUm = (int)(atof(G_As) * 1000.0);

	m_ZonesComboBox.GetLBText(m_ZonesComboBox.GetCurSel(), s);
	for (int i = 0; i < 3; i++)
	{
		G_As = s;
		::Settings.m_ZoneDiameterUm[i] = (int)(atof(G_As) * 1000.0);
	}
	//Done

	//Rounding
	if (m_Round8Radio.GetCheck())
	{
		::Settings.m_RefractionRounding = 0;
	}
	else
	{
		::Settings.m_RefractionRounding = 1;
	}
	//Done

	//Scan diameter
	m_ScanDiameterEdit.GetWindowText(s);
	G_As = s;
	::Settings.m_AutoScanDiameterUm = (int)RealRound(atof(G_As) * 1000.0, 100.0);

	if (::Settings.m_AutoScanDiameterUm < 2000)      ::Settings.m_AutoScanDiameterUm = 2000;
	else if (::Settings.m_AutoScanDiameterUm > 8000) ::Settings.m_AutoScanDiameterUm = 8000;

	::Settings.m_ManualScanDiameterUm = ::Settings.m_AutoScanDiameterUm;
	//Done
	//Setting Done

	ChangeCal();

	//
	BOOL BackUpSuperExamSetting = ::NewSettings.m_Super_Exam;
	::NewSettings.m_Super_Exam = FALSE;
	//

	CWFAcquisition* pWFAcquisition = new CWFAcquisition(&::HW, &::Settings, FALSE);

	CWFAcquisitionDlg* pWFAcquisitionDlg = new CWFAcquisitionDlg(this, pWFAcquisition, TRUE);

	//pWFAcquisitionDlg->m_NormalUsed = 0;
	//pWFAcquisitionDlg->ShowWindow(SW_HIDE);

	BOOL Res = pWFAcquisitionDlg->DoModal() == IDOK;

	//m_res = "abc";
	if (Res)
	{
		CWFExam WFExam = pWFAcquisition->m_WFExam;
		WFExam.Process();
		CZernikeSurface Surface = WFExam.m_WfSurface;

		real_t um3 = Surface.GetCUm(3);
		real_t um4 = Surface.GetCUm(4);
		real_t um5 = Surface.GetCUm(5);

		CString sC3, sC4, sC5, sSph;
		sC3.Format(_T("%6.10f"), um3);
		sC4.Format(_T("%6.10f"), um4);
		sC5.Format(_T("%6.10f"), um5);

		sC3 = "C3:  " + sC3;
		sC4 = "C4:  " + sC4;
		sC5 = "C5:  " + sC5;

		m_res[m_count] = sC3 + "    " + sC4 + "    " + sC5 + "\n\n";

		real_t SphEq, Sph, Cyl; int Axis;
		Surface.ChangeRMaxUm(2000);//????????

		CWFExam::GetSpheqSphCylAxis(Surface, 0, ::Settings.m_PositiveCylinder,
			SphEq, Sph, Cyl, Axis);

		sSph.Format(_T("Value of sphere @ %.1f mm zone:  %.2f    "), ::Settings.m_ZoneDiameterUm[0] * 0.001, Sph);
		m_res[m_count] = sSph + m_res[m_count];

		m_ResText.SetWindowText(m_res[m_count]);
	}

	int a = 0;
	//delete pWFAcquisition;
	//delete pWFAcquisitionDlg;

	::NewSettings.m_Super_Exam = BackUpSuperExamSetting;//
}

//***************************************************************************************

void CQuickWFDlg::OnCancel()
{
	GetBkSetVal();

	m_pHW->CancelCalibrationChanges();

	CDialog::OnCancel();
}

//***************************************************************************************

void CQuickWFDlg::OnOK()
{
	GetBkSetVal();

	if (CalChanged() && !m_WFClick)
	{
		if (!::Question("Calibration settings have been changed.\nYou did not take any wf test. Do you want to change these values?", 1, 0))
		{
			m_pHW->CancelCalibrationChanges();
		}
		else ChangeCal();

		CDialog::OnOK();
	}

	ChangeCal();

	CDialog::OnOK();
}

//***************************************************************************************

void CQuickWFDlg::GetBkSetVal()
{
	::Settings.m_VertexDistanceUm = m_Bk_VertexDistanceUm;

	for (int i = 0; i < 3; i++)
	{
		::Settings.m_ZoneDiameterUm[i] = m_Bk_ZoneDiameterUm[i];
	}

	::Settings.m_RefractionRounding = m_Bk_RefractionRounding;
	::Settings.m_AutoScanDiameterUm = m_Bk_AutoScanDiameterUm;
	::Settings.m_ManualScanDiameterUm = m_Bk_ManualScanDiameterUm;
}

//***************************************************************************************

BOOL  CQuickWFDlg::CalChanged()
{
	CString s;

	// Zernike C3
	m_C3Edit.GetWindowText(s);
	G_As = s;
	if (m_backC3 != atof(G_As)) return TRUE;

	// Zernike C4
	m_C4Edit.GetWindowText(s);
	G_As = s;
	if (m_backC4 != atof(G_As)) return TRUE;

	// Zernike C5
	m_C5Edit.GetWindowText(s);
	G_As = s;
	if (m_backC5 != atof(G_As)) return TRUE;

	// LADs exposure time
	m_LADExpTimeEdit.GetWindowText(s);
	G_As = s;
	if (m_backExposureTime != atoi(G_As)) return TRUE;

	// LADs swap
	if (m_backLADSwapXY != m_SwapLADsCheck.GetCheck()) return TRUE;

	// LADX flip
	if (m_backADXNoFlip != m_FlipLADXCheck.GetCheck()) return TRUE;

	// LADX focal distance
	m_LADXFocalDistanceEdit.GetWindowText(s);
	G_As = s;
	if (m_backLADXFocalDistance != atof(G_As)) return TRUE;

	// LADY flip
	if (m_backLADYNoFlip != m_FlipLADYCheck.GetCheck()) return TRUE;

	// LADY focal distance
	m_LADYFocalDistanceEdit.GetWindowText(s);
	G_As = s;
	if (m_backLADYFocalDistance != atof(G_As)) return TRUE;
	//Done

	//Optometer enabled
	m_pCalibration->OptometerEnabled = m_OptometerCheck.GetCheck() ? 1 : 0;
	if (m_backOptometerEnabled != m_OptometerCheck.GetCheck()) return TRUE;
	//Done

	return FALSE;
}

//***************************************************************************************
//Change CALIBRATION
void  CQuickWFDlg::ChangeCal()
{
	CString s;

	// Zernike C3
	m_C3Edit.GetWindowText(s);
	G_As = s;
	m_pCalibration->Zernike.C3 = atof(G_As);

	// Zernike C4
	m_C4Edit.GetWindowText(s);
	G_As = s;
	m_pCalibration->Zernike.C4 = atof(G_As);

	// Zernike C5
	m_C5Edit.GetWindowText(s);
	G_As = s;
	m_pCalibration->Zernike.C5 = atof(G_As);

	// LADs exposure time
	m_LADExpTimeEdit.GetWindowText(s);
	G_As = s;
	m_pCalibration->LADExposureTime = atoi(G_As);

	// LADs swap
	m_pCalibration->LADSwapXY = m_SwapLADsCheck.GetCheck();

	// LADX flip
	m_pCalibration->LADXNoFlip = !m_FlipLADXCheck.GetCheck();//cjf

															 // LADX focal distance
	m_LADXFocalDistanceEdit.GetWindowText(s);
	G_As = s;
	m_pCalibration->LADXFocalDistance = atof(G_As);

	// LADY flip
	m_pCalibration->LADYNoFlip = !m_FlipLADYCheck.GetCheck();//cjf

															 // LADY focal distance
	m_LADYFocalDistanceEdit.GetWindowText(s);
	G_As = s;
	m_pCalibration->LADYFocalDistance = atof(G_As);
	//Done

	//Optometer enabled
	m_pCalibration->OptometerEnabled = m_OptometerCheck.GetCheck() ? 1 : 0;
	//Done
}

//***************************************************************************************

void CQuickWFDlg::OnListDblClicked(NMHDR* pNMHDR, LRESULT* pRes)
{
	*pRes = 0;

	CString s;
	int m = m_WFResultList.GetSelectionMark();

	//Vertex
	s.Format(_T("%.1f"), m_Saved_VertexDistanceUm[m] * 0.001);
	m_VertexComboBox.SelectString(-1, s);
	//Done

	//Zone
	s.Format(_T("%.1f"), m_Saved_ZoneDiameterUm[m] * 0.001);
	m_ZonesComboBox.SelectString(-1, s);
	//Done

	m_Round8Radio.SetCheck(!m_Saved_RefractionRounding[m]);
	m_Round100Radio.SetCheck(m_Saved_RefractionRounding[m]);

	s.Format(_T("%.1f"), m_Saved_ScanDiameterUm[m] * 0.001);
	m_ScanDiameterEdit.SetWindowText(s);

	// Zernike C3
	s.Format(_T("%.3f"), m_Saved_C3[m]);
	m_C3Edit.SetWindowText(s);

	// Zernike C4
	s.Format(_T("%.3f"), m_Saved_C4[m]);
	m_C4Edit.SetWindowText(s);

	// Zernike C5
	s.Format(_T("%.3f"), m_Saved_C5[m]);
	m_C5Edit.SetWindowText(s);

	// LADs exposure time
	s.Format(_T("%i"), m_Saved_ExposureTime[m]);
	m_LADExpTimeEdit.SetWindowText(s);

	// LADs swap
	m_SwapLADsCheck.SetCheck(m_Saved_LADSwapXY[m]);

	// LADX flip
	m_FlipLADXCheck.SetCheck(!m_Saved_ADXNoFlip[m]);

	// LADX focal distance
	s.Format(_T("%.0f"), m_Saved_LADXFocalDistance[m]);
	m_LADXFocalDistanceEdit.SetWindowText(s);

	// LADY flip
	m_FlipLADYCheck.SetCheck(!m_Saved_LADYNoFlip[m]);

	// LADY focal distance
	s.Format(_T("%.0f"), m_Saved_LADYFocalDistance[m]);
	m_LADYFocalDistanceEdit.SetWindowText(s);

	//Optometer enabled or not
	m_OptometerCheck.SetCheck(m_Saved_OptometerEnabled[m] == 1 ? 1 : 0);
}


//***************************************************************************************

void CQuickWFDlg::OnPrintBtnClicked()
{
	if (m_count == 0)
	{
		::Info("No saved exam result in the list.");
		return;
	}

	SYSTEMTIME CurTime;
	::GetLocalTime(&CurTime);
	CString Def_FileName;
	Def_FileName.Format(_T("QuickWF-%02i-%02i-%04i-%02i-%02i-%02i"), CurTime.wMonth, CurTime.wDay, CurTime.wYear, CurTime.wHour, CurTime.wMinute, CurTime.wSecond);

	CString Dir = "C:\\";

	CFileDialog* pDlg = new CFileDialog(FALSE, _T("txt"), Def_FileName, OFN_OVERWRITEPROMPT, _T("TXT (*.txt)|*.txt||"), this);
	pDlg->m_ofn.lpstrInitialDir = Dir;

	BOOL Res = pDlg->DoModal() == IDOK;
	if (!Res) return;
	CString FileName = pDlg->GetPathName();
	delete pDlg;

	CString sTitle;
	sTitle.Format(_T("Generation time:  %02i-%02i-%04i   %02i:%02i:%02i"), CurTime.wMonth, CurTime.wDay, CurTime.wYear, CurTime.wHour, CurTime.wMinute, CurTime.wSecond);
	if (CurTime.wHour < 12) sTitle += " AM";
	else  sTitle += " PM";

	CString Serial;
	CStringA ASerial(Serial);
	strncpy(m_pCalibration->HardwareSerial, ASerial, sizeof(m_pCalibration->HardwareSerial));

	CStringA AFileName(FileName);
	FILE* pFilew = fopen(AFileName, "w");
	fprintf(pFilew, "%s\n\n", sTitle);
	fprintf(pFilew, "Machine Serial:  %s\n\n", Serial);

	CString s;

	for (int i = 0; i < m_count; i++)
	{
		s.Format(_T("%i.   "), i + 1);
		s += m_res[i];
		fprintf(pFilew, "%s\n", s);
	}
	fclose(pFilew);

	::Info("The exam results have been printed to " + FileName);
}

//*******************************************************************************************
// Save the cal values to Exporm
void CQuickWFDlg::OnCalSaveBtnClicked()
{
	CBusyCursor Cursor;

	CString s;

	// Zernike C3
	m_C3Edit.GetWindowText(s);
	G_As = s;
	m_pCalibration->Zernike.C3 = atof(G_As);

	// Zernike C4
	m_C4Edit.GetWindowText(s);
	G_As = s;
	m_pCalibration->Zernike.C4 = atof(G_As);

	// Zernike C5
	m_C5Edit.GetWindowText(s);
	G_As = s;
	m_pCalibration->Zernike.C5 = atof(G_As);

	m_LADExpTimeEdit.GetWindowText(s);
	G_As = s;
	m_pCalibration->LADExposureTime = atoi(G_As);

	// LADs swap
	m_pCalibration->LADSwapXY = m_SwapLADsCheck.GetCheck();

	// LADX flip
	m_pCalibration->LADXNoFlip = !m_FlipLADXCheck.GetCheck();

	// LADX focal distance
	m_LADXFocalDistanceEdit.GetWindowText(s);
	G_As = s;
	m_pCalibration->LADXFocalDistance = atof(G_As);

	// LADY flip
	m_pCalibration->LADYNoFlip = !m_FlipLADYCheck.GetCheck();

	// LADY focal distance
	m_LADYFocalDistanceEdit.GetWindowText(s);
	G_As = s;
	m_pCalibration->LADYFocalDistance = atof(G_As);

	// Optometer enabled
	m_pCalibration->OptometerEnabled = m_OptometerCheck.GetCheck() ? 1 : 0;

	m_pHW->SaveCalibrationIntoFlash();
}
