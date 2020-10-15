//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "FolderDlg.h"
#include "Settings.h"
#include "Registry.h"
#include "SettingsDefaultDlg.h"
#include "INIParser.h"//6.2.0 ini registration

#include "SettingsSoloDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CSettingsSoloDlg, CDialog)

	ON_BN_CLICKED(IDC_WF_ENABLE_PROBING_CHECK, OnProbingButtonClicked)

	ON_BN_CLICKED(IDDEFAULT, OnDefaultButtonClicked)
	ON_BN_CLICKED(IDSAVE, OnSaveButtonClicked)
	ON_BN_CLICKED(IDLOAD, OnLoadButtonClicked)

	ON_BN_CLICKED(IDC_ACCOMM_TARGET_LIGHT_ON_CHECK2, OnTargetLightBtnClicked)//530

	ON_BN_CLICKED(IDC_WFSUPEREXAM_CHK, OnSuperExamBtnClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CSettingsSoloDlg::CSettingsSoloDlg(CWnd* pParentWnd) : CDialog(IDD_SETTINGS_SOLO_DLG, pParentWnd)
{

}

//***************************************************************************************

void CSettingsSoloDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_WF_POS_CYL_CHECK2, m_PosCylCheck);//1
	DDX_Control(pDX, IDC_WF_VERTEX_COMBOBOX2, m_VertexComboBox);//1
	DDX_Control(pDX, IDC_WF_ZONE_0_COMBOBOX2, m_ZonesComboBox[0]);//1
	DDX_Control(pDX, IDC_WF_ZONE_1_COMBOBOX2, m_ZonesComboBox[1]);//1
	DDX_Control(pDX, IDC_WF_ZONE_2_COMBOBOX2, m_ZonesComboBox[2]);//1
	DDX_Control(pDX, IDC_WF_ENABLE_PROBING_CHECK2, m_ProbingEnabledCheck);//1
	DDX_Control(pDX, IDC_WF_ENABLE_FOGGING_CHECK2, m_FoggingEnabledCheck);//1
	DDX_Control(pDX, IDC_WF_MANUAL_SCAN_D_EDIT2, m_ManualScanDiameterEdit);//1
	DDX_Control(pDX, IDC_WF_AUTO_SCAN_D_EDIT2, m_AutoScanDiameterEdit);//1

	DDX_Control(pDX, IDC_WF_SCAN_AFTER_NEW_EXAM_CHECK2, m_AnimateAfterNewExamCheck);//1
	DDX_Control(pDX, IDC_DATE_FORMAT_MMDDYYYY_RADIO2, m_MMDDYYYYRadio);//1
	DDX_Control(pDX, IDC_DATE_FORMAT_DDMMYYYY_RADIO2, m_DDMMYYYYRadio);//1
	DDX_Control(pDX, IDC_DATE_FORMAT_YYYYMMDD_RADIO2, m_YYYYMMDDRadio);//1
	DDX_Control(pDX, IDC_WAVETOUCH_CHECK2, m_WavetouchCheck);//1
	DDX_Control(pDX, IDC_ACCOMM_TARGET_LIGHT_ON_CHECK2, m_AccommTargetLightOnCheck);//1
	DDX_Control(pDX, IDC_SHOW_SEND_ARKR_CHECK2, m_ShowSendARKRCheck);//1

	DDX_Control(pDX, IDC_WFVRF_RADIO2, m_WFVrfRadio);//1
	DDX_Control(pDX, IDC_WFVFA_RADIO2, m_WFRfaRadio);//1
	DDX_Control(pDX, IDC_WFSUM1_RADIO2, m_WFAbaRadio);//1
	DDX_Control(pDX, IDC_WFSUM2_RADIO2, m_WFVfaRadio);//1

	DDX_Control(pDX, IDC_ALIGNMENT_PUPIL_RADIO2, m_AlignmentPupilRadio);//1
	DDX_Control(pDX, IDC_ALIGNMENT_4IRDOTS_RADIO2, m_Alignment4IrdotsRadio);//1
	DDX_Control(pDX, IDC_ROUND_1_8_RADIO2, m_Round8Radio);//1
	DDX_Control(pDX, IDC_ROUND_1_100_RADIO2, m_Round100Radio);//1
	DDX_Control(pDX, IDCANCEL, m_CancelButton);//1
	DDX_Control(pDX, IDOK, m_OKButton);//1
	DDX_Control(pDX, IDDEFAULT, m_DefaultButton);//1
	DDX_Control(pDX, IDSAVE, m_SaveButton);//1
	DDX_Control(pDX, IDLOAD, m_LoadButton);//1

	DDX_Control(pDX, IDC_TARGETTIME_EDIT, m_TargetTimeEdit);//530
	DDX_Control(pDX, IDC_TT_STATIC, m_TargetTimeLabel);//530

	DDX_Control(pDX, IDC_WFSUPEREXAM_CHK, m_Super_ExamCheck);
}

//***************************************************************************************

BOOL CSettingsSoloDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_BackSomeDefaults = FALSE;//520

	CString s;

	m_PosCylCheck.SetCheck(::Settings.m_PositiveCylinder);

	for (real_t v = 0.0; v < 20.001; v += 0.5)
	{
		s.Format(_T("%.1f"), v);
		m_VertexComboBox.AddString(s);
	}
	s.Format(_T("%.1f"), ::Settings.m_VertexDistanceUm * 0.001);
	m_VertexComboBox.SelectString(-1, s);

	for (real_t d = 2.0; d < 8.001; d += 0.1)
	{
		s.Format(_T("%.1f"), d);
		m_ZonesComboBox[0].AddString(s);
		m_ZonesComboBox[1].AddString(s);
		m_ZonesComboBox[2].AddString(s);
	}
	for (int i = 0; i < 3; i++)
	{
		s.Format(_T("%.1f"), ::Settings.m_ZoneDiameterUm[i] * 0.001);
		m_ZonesComboBox[i].SelectString(-1, s);
	}

	m_ProbingEnabledCheck.SetCheck(::Settings.m_ProbingEnabled);

	m_FoggingEnabledCheck.SetCheck(::Settings.m_ProbingEnabled && ::Settings.m_FoggingEnabled);
	m_FoggingEnabledCheck.EnableWindow(::Settings.m_ProbingEnabled);

	s.Format(_T("%.1f"), ::Settings.m_ManualScanDiameterUm * 0.001);
	m_ManualScanDiameterEdit.SetWindowText(s);


	s.Format(_T("%.1f"), ::Settings.m_AutoScanDiameterUm * 0.001);
	m_AutoScanDiameterEdit.SetWindowText(s);
	//Done

	if (::Settings.m_RefractionRounding == 0) {
		m_Round8Radio.SetCheck(1);
	}
	else {
		m_Round100Radio.SetCheck(1);
	}

	m_AnimateAfterNewExamCheck.SetCheck(::Settings.m_AnimateAfterNewExam);

	if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY) {
		m_DDMMYYYYRadio.SetCheck(1);
	}
	else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD) {
		m_YYYYMMDDRadio.SetCheck(1);
	}
	else {
		m_MMDDYYYYRadio.SetCheck(1);
	}

	if (::Settings.m_AlignmentMethod == ALIGNMENT_PUPIL_CENTER) {
		m_AlignmentPupilRadio.SetCheck(1);
	}
	else {
		m_Alignment4IrdotsRadio.SetCheck(1);
	}

	switch (::NewSettings.m_DefaultWFSolo)
	{
	case 0: m_WFVrfRadio.SetCheck(1); break; //Point
	case 1: m_WFRfaRadio.SetCheck(1); break; //Refraction analysis
	case 2: m_WFAbaRadio.SetCheck(1); break; //Refraction
	case 3: m_WFVfaRadio.SetCheck(1); break;	//VFA
	}


	m_WavetouchCheck.SetCheck(::Settings.m_ShowWavetouch);
	m_AccommTargetLightOnCheck.SetCheck(!::Settings.m_AccommTargetLightOff);

	//530
	if (::Settings.m_AccommTargetLightOff)
	{
		m_TargetTimeEdit.ShowWindow(SW_HIDE);
		m_TargetTimeLabel.ShowWindow(SW_HIDE);
	}
	else
	{
		m_TargetTimeEdit.ShowWindow(SW_SHOW);
		m_TargetTimeLabel.ShowWindow(SW_SHOW);

		CString s;
		s.Format(_T("%i"), ::Settings.m_AccommTargetLightTime);
		m_TargetTimeEdit.SetWindowText(s);
	}
	//530

	m_ShowSendARKRCheck.SetCheck(::Settings.m_ShowSendARKRButton);

	RECT Rect;
	GetWindowRect(&Rect);
	int ww = Rect.right - Rect.left;
	int wh = Rect.bottom - Rect.top;

	CenterWindow();

	GetClientRect(&Rect);
	int cw = Rect.right - Rect.left;
	int ch = Rect.bottom - Rect.top;

	int W = ::GetSystemMetrics(SM_CXSCREEN);
	int H = ::GetSystemMetrics(SM_CYSCREEN);

	int d = intRound(0.005 * W);

	int bw = intRound(0.065 * W);
	int bh = intRound(0.033 * H);
	int bl = intRound(0.5 * cw - (0.5 * d + bw));
	int bt = ch - d - bh;

	m_CancelButton.SetWindowPos(NULL, bl, bt, bw, bh, SWP_NOZORDER);
	bl = intRound(0.5 * cw + 0.5 * d);
	m_OKButton.SetWindowPos(NULL, bl, bt, bw, bh, SWP_NOZORDER);
	bl = intRound(0.8 * cw + 0.5 * d);
	m_DefaultButton.SetWindowPos(NULL, bl, bt, bw - 30, bh - 5, SWP_NOZORDER);
	bl = intRound(0.8 * cw + 0.5 * d);
	bt = intRound(ch - d - bh*1.9);
	m_LoadButton.SetWindowPos(NULL, bl, bt, bw - 30, bh - 5, SWP_NOZORDER);
	bl = intRound(0.8 * cw + 0.5 * d);
	bt = intRound(ch - d - bh*2.8);
	m_SaveButton.SetWindowPos(NULL, bl, bt, bw - 30, bh - 5, SWP_NOZORDER);

	//531 Super WF Exam
	m_OriAliMethod = ::Settings.m_AlignmentMethod;
	m_Super_ExamCheck.SetCheck(::NewSettings.m_Super_Exam);
	if (::NewSettings.m_Super_Exam)
	{
		m_Alignment4IrdotsRadio.SetCheck(TRUE);
		m_AlignmentPupilRadio.SetCheck(FALSE);

		m_Alignment4IrdotsRadio.EnableWindow(FALSE);
		m_AlignmentPupilRadio.EnableWindow(FALSE);

		::Settings.m_AlignmentMethod = ALIGNMENT_4_IR_DOTS;
	}
	//Done

	return TRUE;
}

//***************************************************************************************

void CSettingsSoloDlg::OnOK()
{
	if (!CheckSetting()) return;//530 To check if there are some dis-allowed inputs
	SetSetting();
	CDialog::OnOK();
}

//***************************************************************************************

void CSettingsSoloDlg::OnProbingButtonClicked()
{
	m_FoggingEnabledCheck.SetCheck(m_ProbingEnabledCheck.GetCheck() && m_FoggingEnabledCheck.GetCheck());
	m_FoggingEnabledCheck.EnableWindow(m_ProbingEnabledCheck.GetCheck());
}

//***************************************************************************************

// set the default values of setting
void CSettingsSoloDlg::OnDefaultButtonClicked()
{

	CSettingsDefaultDlg* pDlg = new CSettingsDefaultDlg(this);

	if (pDlg->DoModal() == IDOK)
	{
		m_CtlDef = pDlg->m_CtlDef;
		m_GUIDef = pDlg->m_GUIDef;
	}
	else
	{
		m_CtlDef = FALSE;
		m_GUIDef = FALSE;
	}
	delete pDlg;


	if (m_CtlDef)
	{

		CString s;

		m_PosCylCheck.SetCheck(0);

		s.Format(_T("%.1f"), 12.0);
		m_VertexComboBox.SelectString(-1, s);

		s.Format(_T("%.1f"), 2.0);
		m_ZonesComboBox[0].SelectString(-1, s);

		s.Format(_T("%.1f"), 3.0);
		m_ZonesComboBox[1].SelectString(-1, s);

		s.Format(_T("%.1f"), 5.0);
		m_ZonesComboBox[2].SelectString(-1, s);

		m_Round8Radio.SetCheck(1);
		m_Round100Radio.SetCheck(0);

		m_ProbingEnabledCheck.SetCheck(1);///////
		m_FoggingEnabledCheck.SetCheck(1);
		m_FoggingEnabledCheck.EnableWindow(1);////////

		s.Format(_T("%.1f"), 3.0);
		m_ManualScanDiameterEdit.SetWindowText(s);

		s.Format(_T("%.1f"), 6.5);
		m_AutoScanDiameterEdit.SetWindowText(s);

		m_AnimateAfterNewExamCheck.SetCheck(0);

		m_DDMMYYYYRadio.SetCheck(0);
		m_YYYYMMDDRadio.SetCheck(0);
		m_MMDDYYYYRadio.SetCheck(1);

		m_AlignmentPupilRadio.SetCheck(1);
		m_Alignment4IrdotsRadio.SetCheck(0);

		m_WavetouchCheck.SetCheck(0);
		m_AccommTargetLightOnCheck.SetCheck(1);

		//530
		m_TargetTimeEdit.ShowWindow(SW_SHOW);
		m_TargetTimeLabel.ShowWindow(SW_SHOW);

		m_TargetTimeEdit.SetWindowText(_T("60"));
		//530

		m_ShowSendARKRCheck.SetCheck(0);

		//For other default settings
		m_WFRfaRadio.SetCheck(1);
		m_WFVrfRadio.SetCheck(0);
		m_WFAbaRadio.SetCheck(0);
		m_WFVfaRadio.SetCheck(0);
	}

	//Set other default settings
	if (m_GUIDef) m_BackSomeDefaults = TRUE;
	//Done

	m_Super_ExamCheck.SetCheck(0);//531
	m_Alignment4IrdotsRadio.EnableWindow(TRUE);//531
	m_AlignmentPupilRadio.EnableWindow(TRUE);//531
}


//***************************************************************************************

void CSettingsSoloDlg::SetSetting()
{
	CString s;

	::Settings.m_PositiveCylinder = m_PosCylCheck.GetCheck();

	m_VertexComboBox.GetLBText(m_VertexComboBox.GetCurSel(), s);
	G_As = s;
	::Settings.m_VertexDistanceUm = (int)(atof(G_As) * 1000.0);

	for (int i = 0; i < 3; i++) {
		m_ZonesComboBox[i].GetLBText(m_ZonesComboBox[i].GetCurSel(), s);
		G_As = s;
		::Settings.m_ZoneDiameterUm[i] = (int)(atof(G_As) * 1000.0);
	}

	::Settings.m_Cap_ColorImg = 1;
	::Settings.m_Cap_ColorAutoOrManu = 1;//???????

	::Settings.m_ProbingEnabled = m_ProbingEnabledCheck.GetCheck();

	::Settings.m_FoggingEnabled = ::Settings.m_ProbingEnabled && m_FoggingEnabledCheck.GetCheck();

	m_ManualScanDiameterEdit.GetWindowText(s);
	G_As = s;
	::Settings.m_ManualScanDiameterUm = (int)RealRound(atof(G_As) * 1000.0, 100.0);
	if (::Settings.m_ManualScanDiameterUm < 2000) ::Settings.m_ManualScanDiameterUm = 2000;
	else if (::Settings.m_ManualScanDiameterUm > 8000) ::Settings.m_ManualScanDiameterUm = 8000;

	m_AutoScanDiameterEdit.GetWindowText(s);
	G_As = s;
	::Settings.m_AutoScanDiameterUm = (int)RealRound(atof(G_As) * 1000.0, 100.0);
	if (::Settings.m_AutoScanDiameterUm < 2000) ::Settings.m_AutoScanDiameterUm = 2000;
	else if (::Settings.m_AutoScanDiameterUm > 8000) ::Settings.m_AutoScanDiameterUm = 8000;

	::Settings.m_AnimateAfterNewExam = m_AnimateAfterNewExamCheck.GetCheck();

	if (m_DDMMYYYYRadio.GetCheck()) {
		::Settings.m_DateFormat = DATE_FORMAT_DD_MM_YYYY;
	}
	else if (m_YYYYMMDDRadio.GetCheck()) {
		::Settings.m_DateFormat = DATE_FORMAT_YYYY_MM_DD;
	}
	else {
		::Settings.m_DateFormat = DATE_FORMAT_MM_DD_YYYY;
	}

	if (m_AlignmentPupilRadio.GetCheck()) {
		::Settings.m_AlignmentMethod = ALIGNMENT_PUPIL_CENTER;
	}
	else {
		::Settings.m_AlignmentMethod = ALIGNMENT_4_IR_DOTS;
	}


	::Settings.m_ShowWavetouch = m_WavetouchCheck.GetCheck();
	::Settings.m_AccommTargetLightOff = !m_AccommTargetLightOnCheck.GetCheck();

	//530
	m_TargetTimeEdit.GetWindowText(s);
	G_As = s;
	::Settings.m_AccommTargetLightTime = atoi(G_As);
	//530

	::Settings.m_ShowSendARKRButton = m_ShowSendARKRCheck.GetCheck();

	::NewSettings.m_Super_Exam = m_Super_ExamCheck.GetCheck();//531

	if (m_Round8Radio.GetCheck()) {
		::Settings.m_RefractionRounding = 0;
	}
	else {
		::Settings.m_RefractionRounding = 1;
	}

	// solo default screen setting
	{

		if (m_WFVrfRadio.GetCheck())
			::NewSettings.m_DefaultWFSolo = 0;
		else if (m_WFRfaRadio.GetCheck())
			::NewSettings.m_DefaultWFSolo = 1;
		else if (m_WFAbaRadio.GetCheck())
			::NewSettings.m_DefaultWFSolo = 2;
		else if (m_WFVfaRadio.GetCheck())
			::NewSettings.m_DefaultWFSolo = 3;
	}


	//520 Set display default settings
	if (m_BackSomeDefaults)
	{
		::Settings.m_CSsmSettings[1][0].m_Type = 51;

		//WF Point display
		::Settings.m_TSvrSettings[0].m_MapShowCornealVertex = 1;
		::Settings.m_TSvrSettings[0].m_MapShowPoints = 1;
		::Settings.m_TSvrSettings[0].m_MapShowPupil = 1;
		::Settings.m_TSvrSettings[0].m_MapShowLimbus = 1;
		::Settings.m_TSvrSettings[0].m_SizeSmall = TRUE;
		//Done


		//*****************
		//::Settings.m_TSsmSettings[2][0].m_SizeSmall            = FALSE;
		//*****************

		//WF Vfa display
		::Settings.m_TDsmSettings[0].m_Type = 11;
		::Settings.m_TDsmSettings[3].m_Type = 11;

		for (int i = 0; i < 6; i++)
		{
			::Settings.m_TDsmSettings[i].m_Mask.SetType(MASK_HO_TOTAL);
			::Settings.m_TDsmSettings[i].m_Piston = FALSE;
			::Settings.m_TDsmSettings[i].m_Tilt = FALSE;

			if (i != 0 && i != 3)
			{
				::Settings.m_TDsmSettings[i].m_Ltr = 0;
				::Settings.m_TDsmSettings[i].m_LtrLine = 70;
				::Settings.m_TDsmSettings[i].m_LtrOrientation = 0;
			}

			if (i == 0 || i == 3)
			{
				CScale* pScale = ::Settings.m_TDsmSettings[i].GetScale();
				pScale->m_Step = 0.5;

				::Settings.m_TDsmSettings[i].SetDefaultColors(1);
			}
		}
		//Done



		// WF Compare Display
		::Settings.m_TScmSettings.m_Type = 11;

		::Settings.m_TScmSettings.m_Method3D = 0;
		::Settings.m_TScmSettings.m_MapShowSolidSurface = TRUE;
		::Settings.m_TScmSettings.m_MapShowWireMesh = TRUE;

		::Settings.m_TScmSettings.m_Mask.SetType(MASK_TOTAL);
		::Settings.m_TScmSettings.m_Piston = FALSE;
		::Settings.m_TScmSettings.m_Tilt = FALSE;

		::Settings.m_TScmSettings.m_MapShowMap = 1;
		::Settings.m_TScmSettings.m_MapShowNumbers = 0;
		::Settings.m_TScmSettings.m_MapShowEye = 0;
		::Settings.m_TScmSettings.m_MapShowPupil = 0;
		::Settings.m_TScmSettings.m_MapShowLimbus = 0;
		::Settings.m_TScmSettings.m_MapShowCornealVertex = 0;
		::Settings.m_TScmSettings.m_MapTranslucent = 0;

		::Settings.m_TScmSettings.m_ZoneRadiusUm = 5000;

		::Settings.m_TScmSettings.m_Ltr = 0;
		::Settings.m_TScmSettings.m_LtrLine = 70;
		::Settings.m_TScmSettings.m_LtrOrientation = 0;

		CScale* pScale = ::Settings.m_TScmSettings.GetScale();
		pScale->m_Step = 0.5;
		::Settings.m_TScmSettings.SetDefaultColors(1);
		//Done



		//WF visual acuity display
		::Settings.m_TSsmSettings[0][0].m_Mask.SetType(MASK_TOTAL);
		::Settings.m_TSsmSettings[0][0].m_Piston = TRUE;
		::Settings.m_TSsmSettings[0][0].m_Tilt = TRUE;

		::Settings.m_TSsmSettings[0][2].m_Mask.SetType(MASK_TOTAL);
		::Settings.m_TSsmSettings[0][2].m_Piston = TRUE;
		::Settings.m_TSsmSettings[0][2].m_Tilt = TRUE;

		::Settings.m_TSsmSettings[0][1].m_Mask.SetType(MASK_HO_TOTAL);
		::Settings.m_TSsmSettings[0][1].m_Piston = FALSE;
		::Settings.m_TSsmSettings[0][1].m_Tilt = FALSE;

		::Settings.m_TSsmSettings[0][3].m_Mask.SetType(MASK_HO_TOTAL);
		::Settings.m_TSsmSettings[0][3].m_Piston = FALSE;
		::Settings.m_TSsmSettings[0][3].m_Tilt = FALSE;

		for (int i = 0; i< 2; i++)
		{
			::Settings.m_TSsmSettings[0][i].m_Type = 11;

			::Settings.m_TSsmSettings[0][i].m_MapShowMap = 1;
			::Settings.m_TSsmSettings[0][i].m_MapShowEye = 0;
			::Settings.m_TSsmSettings[0][i].m_MapShowNumbers = 0;
			::Settings.m_TSsmSettings[0][i].m_MapShowPupil = 0;
			::Settings.m_TSsmSettings[0][i].m_MapShowLimbus = 0;
			::Settings.m_TSsmSettings[0][i].m_MapShowCornealVertex = 0;
			::Settings.m_TSsmSettings[0][i].m_MapTranslucent = 0;

			::Settings.m_TSsmSettings[0][i].m_Method3D = 0;
			::Settings.m_TSsmSettings[0][i].m_MapShowSolidSurface = TRUE;
			::Settings.m_TSsmSettings[0][i].m_MapShowWireMesh = TRUE;
			::Settings.m_TSsmSettings[0][i].m_ZoneRadiusUm = 5000;
			::Settings.m_TSsmSettings[0][i].m_SizeSmall = TRUE;

			CScale* pScale = ::Settings.m_TSsmSettings[0][i].GetScale();
			pScale->m_Step = 0.5;
			::Settings.m_TSsmSettings[0][i].SetDefaultColors(1);
		}

		for (int i = 2; i< 4; i++)
		{
			::Settings.m_TSsmSettings[0][i].m_Type = 14;
			::Settings.m_TSsmSettings[0][i].m_Ltr = 0;
			::Settings.m_TSsmSettings[0][i].m_LtrLine = 70;
			::Settings.m_TSsmSettings[0][i].m_LtrOrientation = 0;
			::Settings.m_TSsmSettings[0][i].m_Method3D = 0;
			::Settings.m_TSsmSettings[0][i].m_MapShowSolidSurface = TRUE;
			::Settings.m_TSsmSettings[0][i].m_MapShowWireMesh = TRUE;
			::Settings.m_TSsmSettings[0][i].m_ZoneRadiusUm = 5000;
			::Settings.m_TSsmSettings[0][i].m_SizeSmall = TRUE;
		}
		//Done



		//WF/RMS
		::Settings.m_TSsmSettings[1][0].m_Mask.SetType(MASK_TOTAL);
		::Settings.m_TSsmSettings[1][0].m_Piston = TRUE;
		::Settings.m_TSsmSettings[1][0].m_Tilt = TRUE;

		::Settings.m_TSsmSettings[1][1].m_Mask.SetType(MASK_HO_TOTAL);
		::Settings.m_TSsmSettings[1][1].m_Piston = FALSE;
		::Settings.m_TSsmSettings[1][1].m_Tilt = FALSE;

		::Settings.m_TSsmSettings[1][2].m_Mask.SetType(MASK_LO_TOTAL);
		::Settings.m_TSsmSettings[1][2].m_Piston = FALSE;
		::Settings.m_TSsmSettings[1][2].m_Tilt = FALSE;

		::Settings.m_TSsmSettings[1][3].m_Mask.SetType(MASK_TOTAL);
		::Settings.m_TSsmSettings[1][3].m_Piston = TRUE;
		::Settings.m_TSsmSettings[1][3].m_Tilt = TRUE;

		for (int i = 0; i< 3; i++)
		{
			::Settings.m_TSsmSettings[1][i].m_Type = 10;

			::Settings.m_TSsmSettings[1][i].m_Method3D = 0;
			::Settings.m_TSsmSettings[1][i].m_MapShowSolidSurface = TRUE;
			::Settings.m_TSsmSettings[1][i].m_MapShowWireMesh = TRUE;
			::Settings.m_TSsmSettings[1][i].m_ZoneRadiusUm = 5000;

			::Settings.m_TSsmSettings[1][i].m_SizeSmall = TRUE;

			::Settings.m_TSsmSettings[1][i].m_MapShowMap = 1;
			::Settings.m_TSsmSettings[1][i].m_MapShowEye = 0;
			::Settings.m_TSsmSettings[1][i].m_MapShowNumbers = 0;
			::Settings.m_TSsmSettings[1][i].m_MapShowPupil = 0;
			::Settings.m_TSsmSettings[1][i].m_MapShowLimbus = 0;
			::Settings.m_TSsmSettings[1][i].m_MapShowCornealVertex = 0;
			::Settings.m_TSsmSettings[1][i].m_MapTranslucent = 0;

			CScale* pScale = ::Settings.m_TSsmSettings[1][i].GetScale();
			pScale->m_Step = 0.5;
			::Settings.m_TSsmSettings[1][i].SetDefaultColors(1);
		}

		::Settings.m_TSsmSettings[1][3].m_Type = 16;
		::Settings.m_TSsmSettings[1][3].m_ZoneRadiusUm = 5000;
		::Settings.m_TSsmSettings[1][3].m_RMSIndividual = FALSE;
		::Settings.m_TSsmSettings[1][3].m_SizeSmall = TRUE;
		//Done



		//WF Angle K/A Display
		//Problem the showruler and show inlay can not be set here
		//Can not be remmber in the display too
		::Settings.m_TSsmSettings[2][0].m_Type = 1;
		::Settings.m_TSsmSettings[2][0].m_MapShowPupil = 1;
		::Settings.m_TSsmSettings[2][0].m_MapShowLimbus = 1;
		::Settings.m_TSsmSettings[2][0].m_MapShowCornealVertex = 1;
		::Settings.m_TSsmSettings[2][0].m_SizeSmall = FALSE;
		//Done



		//WF Custom
		::Settings.m_TSsmSettings[3][0].m_Mask.SetType(MASK_TOTAL);
		::Settings.m_TSsmSettings[3][0].m_Piston = TRUE;
		::Settings.m_TSsmSettings[3][0].m_Tilt = TRUE;

		::Settings.m_TSsmSettings[3][1].m_Mask.SetType(MASK_HO_TOTAL);
		::Settings.m_TSsmSettings[3][1].m_Piston = FALSE;
		::Settings.m_TSsmSettings[3][1].m_Tilt = FALSE;

		::Settings.m_TSsmSettings[3][2].m_Mask.SetType(MASK_TOTAL);
		::Settings.m_TSsmSettings[3][2].m_Piston = TRUE;
		::Settings.m_TSsmSettings[3][2].m_Tilt = TRUE;

		::Settings.m_TSsmSettings[3][3].m_Mask.SetType(MASK_HO_TOTAL);
		::Settings.m_TSsmSettings[3][3].m_Piston = FALSE;
		::Settings.m_TSsmSettings[3][3].m_Tilt = FALSE;

		for (int i = 0; i < 4; i++)
		{
			::Settings.m_TSsmSettings[3][i].m_ZoneRadiusUm = 5000;
			::Settings.m_TSsmSettings[3][i].m_Method3D = 0;
			::Settings.m_TSsmSettings[3][i].m_MapShowSolidSurface = TRUE;
			::Settings.m_TSsmSettings[3][i].m_MapShowWireMesh = TRUE;
			::Settings.m_TSsmSettings[3][i].m_SizeSmall = TRUE;

			if (i == 0 || i == 1)
			{
				::Settings.m_TSsmSettings[3][i].m_Type = 15;//TYPE_TMTF
				::Settings.m_TSsmSettings[3][i].m_MTFAverage = 1;
				::Settings.m_TSsmSettings[3][i].m_MTFAngle = 0;
			}

			if (i == 2 || i == 3)
			{
				::Settings.m_TSsmSettings[3][i].m_Type = 12;//TYPE_TPSF
				::Settings.m_TSsmSettings[3][i].m_ShowEEF = 0;
			}
		}
		//Done


		//WF Toric planner
		::Settings.m_PlannerSettings.m_MapShowEye = 1;
		//Done


		//CT Rings
		::Settings.m_CSvrSettings.m_MapShowRings = 1;
		::Settings.m_CSvrSettings.m_MapShowPupil = 0;
		::Settings.m_CSvrSettings.m_MapShowLimbus = 0;
		//Done


		//CT OD/OS
		::Settings.m_CDsmSettings[0].m_Type = 31;//Axial map       TYPE_CAXM
		::Settings.m_CDsmSettings[1].m_Type = 32;//local Roc  map  TYPE_CTNM

		for (int i = 0; i < 2; i++)
		{
			::Settings.m_CDsmSettings[i].m_Method3D = 0;
			::Settings.m_CDsmSettings[i].m_MapShowSolidSurface = TRUE;
			::Settings.m_CDsmSettings[i].m_MapShowWireMesh = TRUE;
			::Settings.m_CDsmSettings[i].m_MapUnit = DIOPTERS;

			::Settings.m_CDsmSettings[i].m_MapShowMap = 1;
			::Settings.m_CDsmSettings[i].m_MapShowEye = 1;
			::Settings.m_CDsmSettings[i].m_MapShowNumbers = 0;
			::Settings.m_CDsmSettings[i].m_MapShowPupil = 0;
			::Settings.m_CDsmSettings[i].m_MapShowLimbus = 0;
			::Settings.m_CDsmSettings[i].m_MapShowCornealVertex = 0;
			::Settings.m_CDsmSettings[i].m_MapTranslucent = 0;

			CScale* pScale = ::Settings.m_CDsmSettings[i].GetScale();
			pScale->m_Step = 0.5;
			::Settings.m_CDsmSettings[i].SetDefaultColors(1);
		}
		//Done


		//CT Compare
		::Settings.m_CScmSettings.m_Type = 31;//Axial map       TYPE_CAXM
		::Settings.m_CScmSettings.m_Method3D = 0;
		::Settings.m_CScmSettings.m_MapShowSolidSurface = TRUE;
		::Settings.m_CScmSettings.m_MapShowWireMesh = TRUE;
		::Settings.m_CScmSettings.m_MapUnit = DIOPTERS;

		::Settings.m_CScmSettings.m_MapShowMap = 1;
		::Settings.m_CScmSettings.m_MapShowEye = 1;
		::Settings.m_CScmSettings.m_MapShowNumbers = 0;
		::Settings.m_CScmSettings.m_MapShowPupil = 0;
		::Settings.m_CScmSettings.m_MapShowLimbus = 0;
		::Settings.m_CScmSettings.m_MapShowCornealVertex = 0;
		::Settings.m_CScmSettings.m_MapTranslucent = 0;

		CScale* pScale2 = ::Settings.m_CScmSettings.GetScale();
		pScale2->m_Step = 0.5;
		::Settings.m_CScmSettings.SetDefaultColors(1);
		//Done


		//CT Summary
		::Settings.m_CSsmSettings[0][0].m_Type = 31; //Axial map       TYPE_CAXM
		::Settings.m_CSsmSettings[0][1].m_Type = 34; //z elevation     TYPE_CELM
		::Settings.m_CSsmSettings[0][2].m_Type = 32; //local Roc  map  TYPE_CTNM
		::Settings.m_CSsmSettings[0][3].m_Type = 33; //refrective map  TYPE_CRFM

		::Settings.m_CSsmSettings[0][0].m_MapUnit = DIOPTERS;
		::Settings.m_CSsmSettings[0][2].m_MapUnit = MILLIMETERS;

		::Settings.m_CSsmSettings[0][1].m_RefSurfaceType = REF_SURFACE_BEST_SPHERE;
		for (int i = 0; i < 4; i++)
		{
			::Settings.m_CSsmSettings[0][i].m_Method3D = 0;
			::Settings.m_CSsmSettings[0][i].m_MapShowSolidSurface = TRUE;
			::Settings.m_CSsmSettings[0][i].m_MapShowWireMesh = TRUE;
			::Settings.m_CSsmSettings[0][i].m_SizeSmall = TRUE;

			::Settings.m_CSsmSettings[0][i].m_MapShowMap = 1;
			::Settings.m_CSsmSettings[0][i].m_MapShowEye = 1;
			::Settings.m_CSsmSettings[0][i].m_MapShowNumbers = 0;
			::Settings.m_CSsmSettings[0][i].m_MapShowPupil = 0;
			::Settings.m_CSsmSettings[0][i].m_MapShowLimbus = 0;
			::Settings.m_CSsmSettings[0][i].m_MapShowCornealVertex = 0;
			::Settings.m_CSsmSettings[0][i].m_MapTranslucent = 0;

			CScale* pScale = ::Settings.m_CSsmSettings[0][i].GetScale();
			pScale->m_Step = 0.5;
			::Settings.m_CSsmSettings[0][i].SetDefaultColors(1);
		}
		//Done


		//CT Kerotametry Display
		::Settings.m_CSsmSettings[1][0].m_Type = 51; //Kerotametry map       TYPE_CSKM
		::Settings.m_CSsmSettings[1][0].m_SizeSmall = FALSE;
		//Done


		//CT 3D Z Elevation Display
		::Settings.m_CSsmSettings[2][0].m_Type = 34; //z elevation     TYPE_CELM
		::Settings.m_CSsmSettings[2][0].m_SizeSmall = FALSE;
		::Settings.m_CSsmSettings[2][0].m_Method3D = 1;
		::Settings.m_CSsmSettings[2][0].m_MapShowSolidSurface = TRUE;
		::Settings.m_CSsmSettings[2][0].m_MapShowWireMesh = TRUE;
		CScale* pScale3 = ::Settings.m_CSsmSettings[2][0].GetScale();
		pScale3->m_Step = 0.5;
		::Settings.m_CSsmSettings[2][0].SetDefaultColors(1);
		::Settings.m_CSsmSettings[2][0].m_RefSurfaceType = REF_SURFACE_BEST_SPHERE;
		//Done


		//CT Custom Display
		::Settings.m_CSsmSettings[3][0].m_Type = 31; //Axial map       TYPE_CAXM
		::Settings.m_CSsmSettings[3][1].m_Type = 34; //z elevation     TYPE_CELM
		::Settings.m_CSsmSettings[3][2].m_Type = 32; //local Roc  map  TYPE_CTNM
		::Settings.m_CSsmSettings[3][3].m_Type = 33; //refrective map  TYPE_CRFM

		::Settings.m_CSsmSettings[3][0].m_MapUnit = DIOPTERS;
		::Settings.m_CSsmSettings[3][2].m_MapUnit = MILLIMETERS;

		::Settings.m_CSsmSettings[3][1].m_RefSurfaceType = REF_SURFACE_BEST_SPHERE;

		for (int i = 0; i < 4; i++)
		{
			::Settings.m_CSsmSettings[3][i].m_Method3D = 0;
			::Settings.m_CSsmSettings[3][i].m_MapShowSolidSurface = TRUE;
			::Settings.m_CSsmSettings[3][i].m_MapShowWireMesh = TRUE;
			::Settings.m_CSsmSettings[3][i].m_SizeSmall = TRUE;

			::Settings.m_CSsmSettings[3][i].m_MapShowMap = 1;
			::Settings.m_CSsmSettings[3][i].m_MapShowEye = 1;
			::Settings.m_CSsmSettings[3][i].m_MapShowNumbers = 0;
			::Settings.m_CSsmSettings[3][i].m_MapShowPupil = 0;
			::Settings.m_CSsmSettings[3][i].m_MapShowLimbus = 0;
			::Settings.m_CSsmSettings[3][i].m_MapShowCornealVertex = 0;
			::Settings.m_CSsmSettings[3][i].m_MapTranslucent = 0;

			CScale* pScale = ::Settings.m_CSsmSettings[3][i].GetScale();
			pScale->m_Step = 0.5;
			::Settings.m_CSsmSettings[3][i].SetDefaultColors(1);
		}
		//Done


		//WF&CT Change Analysis Display
		::Settings.m_IDsmSettings[0][0].m_Type = 41; //RMS map        TYPE_IRMS
		::Settings.m_IDsmSettings[0][0].m_Method3D = 0;
		::Settings.m_IDsmSettings[0][0].m_MapShowSolidSurface = TRUE;
		::Settings.m_IDsmSettings[0][0].m_MapShowWireMesh = TRUE;
		::Settings.m_IDsmSettings[0][0].m_SizeSmall = TRUE;
		::Settings.m_IDsmSettings[0][0].m_ZoneRadiusUm = 5000;
		::Settings.m_IDsmSettings[0][0].m_Mask.SetType(MASK_TOTAL_NO_DEFOCUS);
		::Settings.m_IDsmSettings[0][0].m_Piston = TRUE;
		::Settings.m_IDsmSettings[0][0].m_Tilt = TRUE;
		::Settings.m_IDsmSettings[0][0].m_RMSIndividual = TRUE;

		::Settings.m_IDsmSettings[0][3].m_Type = 31; //Axial map       TYPE_CAXM
		::Settings.m_IDsmSettings[0][3].m_Method3D = 0;
		::Settings.m_IDsmSettings[0][3].m_MapShowSolidSurface = TRUE;
		::Settings.m_IDsmSettings[0][3].m_MapShowWireMesh = TRUE;
		::Settings.m_IDsmSettings[0][3].m_SizeSmall = TRUE;
		::Settings.m_IDsmSettings[0][3].m_MapUnit = DIOPTERS;
		::Settings.m_IDsmSettings[0][3].m_MapShowMap = 1;
		::Settings.m_IDsmSettings[0][3].m_MapShowEye = 1;
		::Settings.m_IDsmSettings[0][3].m_MapShowNumbers = 0;
		::Settings.m_IDsmSettings[0][3].m_MapShowPupil = 0;
		::Settings.m_IDsmSettings[0][3].m_MapShowLimbus = 0;
		::Settings.m_IDsmSettings[0][3].m_MapShowCornealVertex = 0;
		::Settings.m_IDsmSettings[0][3].m_MapTranslucent = 0;
		::Settings.m_IDsmSettings[0][3].m_MapShowKeratometry = 1;

		CScale* pScale4 = ::Settings.m_IDsmSettings[0][3].GetScale();
		pScale4->m_Step = 0.5;
		::Settings.m_IDsmSettings[0][3].SetDefaultColors(1);
		//Done


		//WF&CT Summary and Custom Displays
		for (int i = 1; i < 4; i++)
		{
			if (i != 2)
			{
				::Settings.m_IDsmSettings[i][0].m_Type = 40; //Wavefront map
				::Settings.m_IDsmSettings[i][1].m_Type = 36; //Wavefront map
				::Settings.m_IDsmSettings[i][2].m_Type = 16; //Wavefront map

				::Settings.m_IDsmSettings[i][0].m_Method3D = 0;
				::Settings.m_IDsmSettings[i][0].m_MapShowSolidSurface = TRUE;
				::Settings.m_IDsmSettings[i][0].m_MapShowWireMesh = TRUE;
				::Settings.m_IDsmSettings[i][0].m_SizeSmall = TRUE;
				::Settings.m_IDsmSettings[i][0].m_Mask.SetType(MASK_TOTAL_NO_DEFOCUS);
				::Settings.m_IDsmSettings[i][0].m_Piston = TRUE;
				::Settings.m_IDsmSettings[i][0].m_Tilt = TRUE;
				::Settings.m_IDsmSettings[i][0].m_MapShowMap = 1;
				::Settings.m_IDsmSettings[i][0].m_ZoneRadiusUm = 5000;
				::Settings.m_IDsmSettings[i][0].m_MapShowEye = 0;
				::Settings.m_IDsmSettings[i][0].m_MapShowNumbers = 0;
				::Settings.m_IDsmSettings[i][0].m_MapShowPupil = 0;
				::Settings.m_IDsmSettings[i][0].m_MapShowLimbus = 0;
				::Settings.m_IDsmSettings[i][0].m_MapShowCornealVertex = 0;
				::Settings.m_IDsmSettings[i][0].m_MapTranslucent = 0;
				::Settings.m_IDsmSettings[i][0].m_MapShowKeratometry = 0;
				::Settings.m_IDsmSettings[i][0].m_MapShowKeratometry = 0;
				::Settings.m_IDsmSettings[i][0].m_MapShowAstigmatismAxes = 0;
				::Settings.m_IDsmSettings[i][0].SetDefaultColors(1);


				::Settings.m_IDsmSettings[i][3].m_Type = 31; //Axial map       TYPE_CAXM
				::Settings.m_IDsmSettings[i][3].m_Method3D = 0;
				::Settings.m_IDsmSettings[i][3].m_MapShowSolidSurface = TRUE;
				::Settings.m_IDsmSettings[i][3].m_MapShowWireMesh = TRUE;
				::Settings.m_IDsmSettings[i][3].m_SizeSmall = TRUE;
				::Settings.m_IDsmSettings[i][3].m_MapUnit = DIOPTERS;
				::Settings.m_IDsmSettings[i][3].m_MapShowMap = 1;
				::Settings.m_IDsmSettings[i][3].m_MapShowEye = 1;
				::Settings.m_IDsmSettings[i][3].m_MapShowNumbers = 0;
				::Settings.m_IDsmSettings[i][3].m_MapShowPupil = 0;
				::Settings.m_IDsmSettings[i][3].m_MapShowLimbus = 0;
				::Settings.m_IDsmSettings[i][3].m_MapShowCornealVertex = 0;
				::Settings.m_IDsmSettings[i][3].m_MapTranslucent = 0;
				::Settings.m_IDsmSettings[i][3].m_MapShowKeratometry = 1;

				CScale* pScale = ::Settings.m_IDsmSettings[i][3].GetScale();
				pScale->m_Step = 0.5;
				::Settings.m_IDsmSettings[i][3].SetDefaultColors(1);
			}
		}
		//Done


		//WF&CT Visual acuity Display
		::Settings.m_IDsmSettings[2][0].m_Type = TYPE_ILTR;
		::Settings.m_IDsmSettings[2][0].m_Method3D = FALSE;
		::Settings.m_IDsmSettings[2][0].m_MapShowSolidSurface = TRUE;
		::Settings.m_IDsmSettings[2][0].m_MapShowWireMesh = TRUE;
		::Settings.m_IDsmSettings[2][0].m_SizeSmall = TRUE;
		::Settings.m_IDsmSettings[2][0].m_ZoneRadiusUm = 5000;
		::Settings.m_IDsmSettings[2][0].m_Mask.SetType(MASK_TOTAL_NO_DEFOCUS);
		::Settings.m_IDsmSettings[2][0].m_Piston = TRUE;
		::Settings.m_IDsmSettings[2][0].m_Tilt = TRUE;
		for (int i = 0; i < 3; i++) ::Settings.m_IDsmSettings[2][i].m_Ltr = 0;
		::Settings.m_IDsmSettings[2][0].m_LtrLine = 70;
		::Settings.m_IDsmSettings[2][0].m_LtrOrientation = 0;

		::Settings.m_IDsmSettings[2][3].m_Type = 31; //Axial map    TYPE_CAXM
		::Settings.m_IDsmSettings[2][3].m_Method3D = 0;
		::Settings.m_IDsmSettings[2][3].m_MapShowSolidSurface = TRUE;
		::Settings.m_IDsmSettings[2][3].m_MapShowWireMesh = TRUE;
		::Settings.m_IDsmSettings[2][3].m_SizeSmall = TRUE;
		::Settings.m_IDsmSettings[2][3].m_MapUnit = DIOPTERS;
		::Settings.m_IDsmSettings[2][3].m_MapShowMap = 1;
		::Settings.m_IDsmSettings[2][3].m_MapShowEye = 1;
		::Settings.m_IDsmSettings[2][3].m_MapShowNumbers = 0;
		::Settings.m_IDsmSettings[2][3].m_MapShowPupil = 0;
		::Settings.m_IDsmSettings[2][3].m_MapShowLimbus = 0;
		::Settings.m_IDsmSettings[2][3].m_MapShowCornealVertex = 0;
		::Settings.m_IDsmSettings[2][3].m_MapTranslucent = 0;
		::Settings.m_IDsmSettings[2][3].m_MapShowKeratometry = 1;

		CScale* pScale5 = ::Settings.m_IDsmSettings[2][3].GetScale();
		pScale5->m_Step = 0.5;
		::Settings.m_IDsmSettings[2][3].SetDefaultColors(1);

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				::Settings.m_IDsmSettings[i][j].m_MapShowEye = 0;
			}
		}

		//WF&CT  OU Overview
		::NewSettings.m_OWFCTSettings[0].m_Type = 31;//Axial map       TYPE_CAXM
		::NewSettings.m_OWFCTSettings[0].m_Mask.SetType(MASK_HO_TOTAL);
		::NewSettings.m_OWFCTSettings[0].m_Piston = FALSE;
		::NewSettings.m_OWFCTSettings[0].m_Tilt = FALSE;
		::NewSettings.m_OWFCTSettings[0].m_Method3D = 0;
		::NewSettings.m_OWFCTSettings[0].m_MapShowSolidSurface = TRUE;
		::NewSettings.m_OWFCTSettings[0].m_MapShowWireMesh = TRUE;
		::NewSettings.m_OWFCTSettings[0].m_MapUnit = DIOPTERS;
		::NewSettings.m_OWFCTSettings[0].m_MapShowMap = 1;
		::NewSettings.m_OWFCTSettings[0].m_MapShowEye = 1;
		::NewSettings.m_OWFCTSettings[0].m_MapShowNumbers = 0;
		::NewSettings.m_OWFCTSettings[0].m_MapShowPupil = 0;
		::NewSettings.m_OWFCTSettings[0].m_MapShowLimbus = 0;
		::NewSettings.m_OWFCTSettings[0].m_MapShowCornealVertex = 0;
		::NewSettings.m_OWFCTSettings[0].m_MapTranslucent = 0;
		::NewSettings.m_OWFCTSettings[0].m_MapShowKeratometry = 0;

		CScale* pScale6 = ::NewSettings.m_OWFCTSettings[0].GetScale();
		pScale6->m_Step = 0.5;
		::NewSettings.m_OWFCTSettings[0].SetDefaultColors(1);


		::NewSettings.m_OWFCTSettings[1].m_Type = 11;//Refraction Map  TYPE_TRFM
		::NewSettings.m_OWFCTSettings[1].m_ZoneRadiusUm = 5000;
		::NewSettings.m_OWFCTSettings[1].m_Mask.SetType(MASK_HO_TOTAL);
		::NewSettings.m_OWFCTSettings[1].m_Piston = FALSE;
		::NewSettings.m_OWFCTSettings[1].m_Tilt = FALSE;
		::NewSettings.m_OWFCTSettings[1].SetDefaultColors(1);

		::NewSettings.m_OWFCTSettings[2].m_Ltr = 0;
		::NewSettings.m_OWFCTSettings[2].m_LtrLine = 70;
		::NewSettings.m_OWFCTSettings[2].m_LtrOrientation = 0;
		//Done


		//IOL Selection Overview 530
		::NewSettings.m_IOLSelSettings[0].m_Type = 31;//Axial map       TYPE_CAXM
		::NewSettings.m_IOLSelSettings[0].m_Mask.SetType(MASK_HO_TOTAL);//601
		::NewSettings.m_IOLSelSettings[0].m_Piston = FALSE;
		::NewSettings.m_IOLSelSettings[0].m_Tilt = FALSE;
		::NewSettings.m_IOLSelSettings[0].m_Method3D = 0;
		::NewSettings.m_IOLSelSettings[0].m_MapShowSolidSurface = TRUE;
		::NewSettings.m_IOLSelSettings[0].m_MapShowWireMesh = TRUE;
		::NewSettings.m_IOLSelSettings[0].m_MapUnit = DIOPTERS;
		::NewSettings.m_IOLSelSettings[0].m_MapShowMap = 1;
		::NewSettings.m_IOLSelSettings[0].m_MapShowEye = 1;
		::NewSettings.m_IOLSelSettings[0].m_MapShowNumbers = 0;
		::NewSettings.m_IOLSelSettings[0].m_MapShowPupil = 0;
		::NewSettings.m_IOLSelSettings[0].m_MapShowLimbus = 0;
		::NewSettings.m_IOLSelSettings[0].m_MapShowCornealVertex = 0;
		::NewSettings.m_IOLSelSettings[0].m_MapTranslucent = 0;
		::NewSettings.m_IOLSelSettings[0].m_MapShowKeratometry = 0;

		pScale6 = ::NewSettings.m_IOLSelSettings[0].GetScale();
		pScale6->m_Step = 0.5;
		::NewSettings.m_IOLSelSettings[0].SetDefaultColors(1);


		::NewSettings.m_IOLSelSettings[1].m_Type = 11;//Refraction Map  TYPE_TRFM
		::NewSettings.m_IOLSelSettings[1].m_ZoneRadiusUm = 5000;
		::NewSettings.m_IOLSelSettings[1].m_Mask.SetType(MASK_HO_TOTAL);//601
		::NewSettings.m_IOLSelSettings[1].m_Piston = FALSE;
		::NewSettings.m_IOLSelSettings[1].m_Tilt = FALSE;
		::NewSettings.m_IOLSelSettings[1].SetDefaultColors(1);

		::NewSettings.m_IOLSelSettings[2].m_Ltr = 0;
		::NewSettings.m_IOLSelSettings[2].m_LtrLine = 70;
		::NewSettings.m_IOLSelSettings[2].m_LtrOrientation = 0;

		for (int i = 3; i < 6; i++)
		{
			::NewSettings.m_IOLSelSettings[i].m_Type = 41; //RMS map        TYPE_IRMS
			::NewSettings.m_IOLSelSettings[i].m_Method3D = 0;
			::NewSettings.m_IOLSelSettings[i].m_MapShowSolidSurface = TRUE;
			::NewSettings.m_IOLSelSettings[i].m_MapShowWireMesh = TRUE;
			::NewSettings.m_IOLSelSettings[i].m_SizeSmall = TRUE;
			::NewSettings.m_IOLSelSettings[i].m_ZoneRadiusUm = 5000;
			::NewSettings.m_IOLSelSettings[i].m_Mask.SetType(MASK_HO_TOTAL);//601
			::NewSettings.m_IOLSelSettings[i].m_Piston = TRUE;
			::NewSettings.m_IOLSelSettings[i].m_Tilt = TRUE;
			::NewSettings.m_IOLSelSettings[i].m_RMSIndividual = TRUE;
		}

		//Done 530

		//Astiagmatism source 530
		::NewSettings.m_InAstigSettings[0] = ::Settings.m_IDsmSettings[2][0];
		::NewSettings.m_InAstigSettings[0] = ::Settings.m_IDsmSettings[2][1];
		::NewSettings.m_InAstigSettings[0] = ::Settings.m_IDsmSettings[2][2];
		::NewSettings.m_InAstigSettings[3] = ::Settings.m_IDsmSettings[2][3];
		for (int i = 0; i < 4; i++)
		{
			::NewSettings.m_InAstigSettings[i].m_SizeSmall = TRUE;
			::NewSettings.m_InAstigSettings[i].m_SimplifiedRMS = TRUE;
			if (i != 3)::NewSettings.m_InAstigSettings[i].m_Mask.SetType(MASK_ASTIGMATISM);
		}
		//Astiagmatism source 530  done

		//Dysfunctional Lens Analysis 531
		::NewSettings.m_DysfuncSettings[0].m_Type = TYPE_IMTF;
		::NewSettings.m_DysfuncSettings[0].m_MTFAverage = TRUE;
		::NewSettings.m_DysfuncSettings[0].m_SimplifiedWFPanel = TRUE;
		::NewSettings.m_DysfuncSettings[0].m_SimplifiedCTPanel = TRUE;
		::NewSettings.m_DysfuncSettings[1].m_Type = TYPE_ILTR;
		::NewSettings.m_DysfuncSettings[1].m_RMSIndividual = FALSE;
		::NewSettings.m_DysfuncSettings[1].m_SimplifiedRMS = TRUE;
		::NewSettings.m_DysfuncSettings[2].m_Type = TYPE_TOPM;
		::NewSettings.m_DysfuncSettings[3].m_Type = TYPE_CAXM;

		for (int i = 0; i < 4; i++)
		{
			::NewSettings.m_DysfuncSettings[i].m_Method3D = 0;
			::NewSettings.m_DysfuncSettings[i].m_MapShowSolidSurface = TRUE;
			::NewSettings.m_DysfuncSettings[i].m_MapShowWireMesh = TRUE;
			::NewSettings.m_DysfuncSettings[i].m_SizeSmall = TRUE;
			if (i == 0 || i == 1) ::NewSettings.m_DysfuncSettings[i].m_Mask.SetType(MASK_HO_TOTAL);
			else       ::NewSettings.m_DysfuncSettings[i].m_Mask.SetType(MASK_TOTAL_NO_DEFOCUS);
			::NewSettings.m_DysfuncSettings[i].m_Piston = TRUE;
			::NewSettings.m_DysfuncSettings[i].m_Tilt = TRUE;
			::NewSettings.m_DysfuncSettings[i].m_MapShowMap = 1;
			::NewSettings.m_DysfuncSettings[i].m_ZoneRadiusUm = 5000;
			::NewSettings.m_DysfuncSettings[i].m_MapShowEye = 0;
			::NewSettings.m_DysfuncSettings[i].m_MapShowNumbers = 0;
			::NewSettings.m_DysfuncSettings[i].m_MapShowPupil = 0;
			::NewSettings.m_DysfuncSettings[i].m_MapShowLimbus = 0;
			::NewSettings.m_DysfuncSettings[i].m_MapShowCornealVertex = 0;
			::NewSettings.m_DysfuncSettings[i].m_MapTranslucent = 0;
			::NewSettings.m_DysfuncSettings[i].m_MapShowKeratometry = 0;
			::NewSettings.m_DysfuncSettings[i].m_MapShowAstigmatismAxes = 0;
			//::NewSettings.m_DysfuncSettings[i].SetDefaultColors(1);

			::NewSettings.m_DysfuncSettings[i].SetDefaultScale();
			CScale* pScale = ::NewSettings.m_DysfuncSettings[i].GetScale();
			pScale->m_Step = 0.5;
			::NewSettings.m_DysfuncSettings[i].SetDefaultColors(1);

			if (i == 3)
			{
				::NewSettings.m_DysfuncSettings[3].m_Type = TYPE_CTNM;
				CScale* pScale1 = ::NewSettings.m_DysfuncSettings[i].GetScale();
				pScale1->m_Step = 0.5;
				::NewSettings.m_DysfuncSettings[i].SetDefaultColors(1);

				::NewSettings.m_DysfuncSettings[3].m_Type = TYPE_CRFM;
				CScale* pScale2 = ::NewSettings.m_DysfuncSettings[i].GetScale();
				pScale2->m_Step = 0.5;
				::NewSettings.m_DysfuncSettings[i].SetDefaultColors(1);

				::NewSettings.m_DysfuncSettings[3].m_Type = TYPE_CELM;
				::NewSettings.m_DysfuncSettings[i].SetDefaultColors(1);

				::NewSettings.m_DysfuncSettings[3].m_Type = TYPE_CAXM;
			}

			::NewSettings.m_DysfuncSettings[i].m_Ltr = 0;
			::NewSettings.m_DysfuncSettings[i].m_LtrLine = 70;
			::NewSettings.m_DysfuncSettings[i].m_LtrOrientation = 0;
		}
		::NewSettings.m_DysfuncSettings[3].m_MapShowAstigmatismAxes = 1;
		::NewSettings.m_DysfuncSettings[3].m_MapShowKeratometry = 1;
		::NewSettings.m_DysfuncSettings[3].m_MapShowEye = 1;
		::NewSettings.m_DysfuncSettings[3].m_MapShowSimKAxes = 1;
		//Dysfunctional Lens Analysis 531  done

		//531 For Dysfunctional lens patient
		::NewSettings.m_DysPtSettings[0] = ::NewSettings.m_DysfuncSettings[1];
		::NewSettings.m_DysPtSettings[0] = ::NewSettings.m_DysfuncSettings[1];
		::NewSettings.m_DysPtSettings[0].m_SimplifiedWFPanel = 1;
		::NewSettings.m_DysPtSettings[0].m_Type = TYPE_CLTR;

		::NewSettings.m_DysPtSettings[1] = ::NewSettings.m_DysfuncSettings[1];

		::NewSettings.m_DysPtSettings[2] = ::NewSettings.m_DysfuncSettings[1];
		::NewSettings.m_DysPtSettings[2].m_Type = TYPE_TLTR;

		::NewSettings.m_DysPtSettings[3] = ::NewSettings.m_DysfuncSettings[3];
		::NewSettings.m_DysPtSettings[4] = ::NewSettings.m_DysfuncSettings[2];
		//Done

		//531 For Depth focus display
		for (int i = 0; i < 4; i++)
		{
			::NewSettings.m_TSsm5Settings[i] = ::Settings.m_TSsmSettings[0][i];
			::NewSettings.m_TSsm5Settings[i].m_SizeSmall = FALSE;
			::NewSettings.m_TSsm5Settings[i].m_Type = TYPE_DOFF;
		}
		//531 Done

		// For Depth focus Comparison display
		for (int i = 0; i < 4; i++)
		{
			::NewSettings.m_TSsm6Settings[i] = ::Settings.m_TSsmSettings[0][i];
			::NewSettings.m_TSsm6Settings[i].m_SizeSmall = FALSE;
			::NewSettings.m_TSsm6Settings[i].m_Type = TYPE_DOFC;
		}
		// Done

		for (int i = 0; i < 4; i++)
			::NewSettings.m_OSHER_ALIG_CUS[i] = 0;


		//530 Presbia scan size zone
		::NewSettings.m_ShowPreZone_WF_VFAWnd = FALSE;
		::NewSettings.m_ShowPreZone_WF_CMPWnd = FALSE;

		for (int i = 0; i < 4; i++)
		{
			::NewSettings.m_ShowPreZone_WF_VA_Wnd[i] = FALSE;
			::NewSettings.m_ShowPreZone_WF_AKA_Wnd[i] = FALSE;
		}
		//Done

		//Excel Export
		for (int i = 0; i < 89; i++)
		{
			if (i < 53) ::Settings.m_EXCEL_WF_SETS[i] = '0';
			if (i < 68) ::Settings.m_EXCEL_CT_SETS[i] = '0';
			if (i < 89) ::Settings.m_EXCEL_WFCT_SETS[i] = '0';
		}
		::Settings.m_EXCEL_WFCT_SETS[0] = '3';

		//
		::NewSettings.m_NEW_EXCEL_WF_SETS[0] = '0';//WFOpacity Grade
		::NewSettings.m_NEW_EXCEL_WFCT_SETS[0] = '0';//WFCT DLI Value
		::NewSettings.m_NEW_EXCEL_WFCT_SETS[1] = '0';//WFCT Opacity Grade
													 //
													 //Done

													 //Others in newsettings
		::NewSettings.m_IncLoC_OD = 0;
		::NewSettings.m_IncLoC_OS = 0;
		::NewSettings.m_SurInAst_OD = 0.0;
		::NewSettings.m_SurInAst_OS = 0.0;
		if (m_isCombo) ::NewSettings.m_OSHER_ALIG_CUS[2] = 1;

		SetDef_ExList_Width();//Set the default widths of the exam list

							  //for screen shot saving
		::NewSettings.m_ScreenShotFormat = 0; //0: save 'jpg' format while the users save the screen
											  //Done

											  //For solo Aberration and Refraction analysis settings
		::NewSettings.m_DefaultWFSolo = 1;//0:Point; 1:Refraction; 2:Aberration; 3:VFA
										  //Refraction Analysis Settings
		for (int i = 0; i<4; i++)
		{
			::NewSettings.m_WFSoloSettings[0][i].m_Type = TYPE_TLTR;//visual acuity
			switch (i)
			{
			case 0: ::NewSettings.m_WFSoloSettings[0][i].m_Mask.SetType(MASK_TOTAL);
				break;
			case 1: ::NewSettings.m_WFSoloSettings[0][i].m_Mask.SetType(MASK_HO_TOTAL);
				break;
			case 2: ::NewSettings.m_WFSoloSettings[0][i].m_Mask.SetType(MASK_TOTAL);
				break;
			case 3: ::NewSettings.m_WFSoloSettings[0][i].m_Mask.SetType(MASK_HO_TOTAL);
				break;
			}
			::NewSettings.m_WFSoloSettings[0][i].m_SizeSmall = TRUE;
			::NewSettings.m_WFSoloSettings[0][i].m_Piston = FALSE;
			::NewSettings.m_WFSoloSettings[0][i].m_Tilt = FALSE;
			::NewSettings.m_WFSoloSettings[0][i].m_Method3D = FALSE;
			::NewSettings.m_WFSoloSettings[0][i].m_MapShowSolidSurface = TRUE;
			::NewSettings.m_WFSoloSettings[0][i].m_MapShowWireMesh = TRUE;
			::NewSettings.m_WFSoloSettings[0][i].m_MapUnit = DIOPTERS;
			::NewSettings.m_WFSoloSettings[0][i].m_MapShowMap = 1;
			::NewSettings.m_WFSoloSettings[0][i].m_MapShowEye = 0;
			::NewSettings.m_WFSoloSettings[0][i].m_MapShowNumbers = 0;
			::NewSettings.m_WFSoloSettings[0][i].m_MapShowPupil = 0;
			::NewSettings.m_WFSoloSettings[0][i].m_MapShowLimbus = 0;
			::NewSettings.m_WFSoloSettings[0][i].m_MapShowCornealVertex = 0;
			::NewSettings.m_WFSoloSettings[0][i].m_MapTranslucent = 0;
			::NewSettings.m_WFSoloSettings[0][i].m_MapShowKeratometry = 0;
			::NewSettings.m_WFSoloSettings[0][i].m_ZoneRadiusUm = 5000;
			CScale* pScale = ::NewSettings.m_WFSoloSettings[0][i].GetScale();
			pScale->m_Step = 0.5;
			::NewSettings.m_WFSoloSettings[0][i].SetDefaultColors(1);
			::NewSettings.m_WFSoloSettings[0][i].m_Ltr = 0;
			::NewSettings.m_WFSoloSettings[0][i].m_LtrLine = 70;
			::NewSettings.m_WFSoloSettings[0][i].m_LtrOrientation = 0;
		}
		//Done


		//Aberration Analysis Settings
		::NewSettings.m_WFSoloSettings[1][0].m_Type = 10;//Wavefront map
		::NewSettings.m_WFSoloSettings[1][0].m_Mask.SetType(MASK_TOTAL);
		::NewSettings.m_WFSoloSettings[1][0].m_SizeSmall = TRUE;
		::NewSettings.m_WFSoloSettings[1][0].m_Piston = FALSE;
		::NewSettings.m_WFSoloSettings[1][0].m_Tilt = FALSE;
		::NewSettings.m_WFSoloSettings[1][0].m_Method3D = 0;
		::NewSettings.m_WFSoloSettings[1][0].m_MapShowSolidSurface = TRUE;
		::NewSettings.m_WFSoloSettings[1][0].m_MapShowWireMesh = TRUE;
		::NewSettings.m_WFSoloSettings[1][0].m_MapUnit = DIOPTERS;
		::NewSettings.m_WFSoloSettings[1][0].m_MapShowMap = 1;
		::NewSettings.m_WFSoloSettings[1][0].m_MapShowEye = 0;
		::NewSettings.m_WFSoloSettings[1][0].m_MapShowNumbers = 0;
		::NewSettings.m_WFSoloSettings[1][0].m_MapShowPupil = 0;
		::NewSettings.m_WFSoloSettings[1][0].m_MapShowLimbus = 0;
		::NewSettings.m_WFSoloSettings[1][0].m_MapShowCornealVertex = 0;
		::NewSettings.m_WFSoloSettings[1][0].m_MapTranslucent = 0;
		::NewSettings.m_WFSoloSettings[1][0].m_MapShowKeratometry = 0;
		::NewSettings.m_WFSoloSettings[1][0].m_ZoneRadiusUm = 5000;
		pScale = ::NewSettings.m_WFSoloSettings[1][0].GetScale();
		pScale->m_Step = 0.5;
		::NewSettings.m_WFSoloSettings[1][0].SetDefaultColors(1);
		::NewSettings.m_WFSoloSettings[1][0].m_Ltr = 0;
		::NewSettings.m_WFSoloSettings[1][0].m_LtrLine = 70;
		::NewSettings.m_WFSoloSettings[1][0].m_LtrOrientation = 0;



		::NewSettings.m_WFSoloSettings[1][1].m_Type = 10;//Wavefront map
		::NewSettings.m_WFSoloSettings[1][1].m_Mask.SetType(MASK_HO_TOTAL);
		::NewSettings.m_WFSoloSettings[1][1].m_SizeSmall = TRUE;
		::NewSettings.m_WFSoloSettings[1][1].m_Piston = FALSE;
		::NewSettings.m_WFSoloSettings[1][1].m_Tilt = FALSE;
		::NewSettings.m_WFSoloSettings[1][1].m_Method3D = 0;
		::NewSettings.m_WFSoloSettings[1][1].m_MapShowSolidSurface = TRUE;
		::NewSettings.m_WFSoloSettings[1][1].m_MapShowWireMesh = TRUE;
		::NewSettings.m_WFSoloSettings[1][1].m_MapUnit = DIOPTERS;
		::NewSettings.m_WFSoloSettings[1][1].m_MapShowMap = 1;
		::NewSettings.m_WFSoloSettings[1][1].m_MapShowEye = 0;
		::NewSettings.m_WFSoloSettings[1][1].m_MapShowNumbers = 0;
		::NewSettings.m_WFSoloSettings[1][1].m_MapShowPupil = 0;
		::NewSettings.m_WFSoloSettings[1][1].m_MapShowLimbus = 0;
		::NewSettings.m_WFSoloSettings[1][1].m_MapShowCornealVertex = 0;
		::NewSettings.m_WFSoloSettings[1][1].m_MapTranslucent = 0;
		::NewSettings.m_WFSoloSettings[1][1].m_MapShowKeratometry = 0;
		::NewSettings.m_WFSoloSettings[1][1].m_ZoneRadiusUm = 5000;
		pScale = ::NewSettings.m_WFSoloSettings[1][1].GetScale();
		pScale->m_Step = 0.5;
		::NewSettings.m_WFSoloSettings[1][1].SetDefaultColors(1);
		::NewSettings.m_WFSoloSettings[1][1].m_Ltr = 0;
		::NewSettings.m_WFSoloSettings[1][1].m_LtrLine = 70;
		::NewSettings.m_WFSoloSettings[1][1].m_LtrOrientation = 0;



		::NewSettings.m_WFSoloSettings[1][2].m_Type = TYPE_TRMS;//TYPE_TMTF;
		::NewSettings.m_WFSoloSettings[1][2].m_Mask.SetType(MASK_TOTAL);
		::NewSettings.m_WFSoloSettings[1][2].m_RMSIndividual = FALSE;//Combined
		::NewSettings.m_WFSoloSettings[1][2].m_SizeSmall = TRUE;
		::NewSettings.m_WFSoloSettings[1][2].m_Piston = FALSE;
		::NewSettings.m_WFSoloSettings[1][2].m_Tilt = FALSE;
		::NewSettings.m_WFSoloSettings[1][2].m_Method3D = 0;
		::NewSettings.m_WFSoloSettings[1][2].m_MapShowSolidSurface = TRUE;
		::NewSettings.m_WFSoloSettings[1][2].m_MapShowWireMesh = TRUE;
		::NewSettings.m_WFSoloSettings[1][2].m_MapUnit = DIOPTERS;
		::NewSettings.m_WFSoloSettings[1][2].m_MapShowMap = 1;
		::NewSettings.m_WFSoloSettings[1][2].m_MapShowEye = 0;
		::NewSettings.m_WFSoloSettings[1][2].m_MapShowNumbers = 0;
		::NewSettings.m_WFSoloSettings[1][2].m_MapShowPupil = 0;
		::NewSettings.m_WFSoloSettings[1][2].m_MapShowLimbus = 0;
		::NewSettings.m_WFSoloSettings[1][2].m_MapShowCornealVertex = 0;
		::NewSettings.m_WFSoloSettings[1][2].m_MapTranslucent = 0;
		::NewSettings.m_WFSoloSettings[1][2].m_MapShowKeratometry = 0;
		::NewSettings.m_WFSoloSettings[1][2].m_ZoneRadiusUm = 5000;



		::NewSettings.m_WFSoloSettings[1][3].m_Type = TYPE_TRMS;
		::NewSettings.m_WFSoloSettings[1][3].m_Mask.SetType(MASK_HO_TOTAL);
		::NewSettings.m_WFSoloSettings[1][3].m_RMSIndividual = TRUE;
		::NewSettings.m_WFSoloSettings[1][3].m_SizeSmall = TRUE;
		::NewSettings.m_WFSoloSettings[1][3].m_Piston = FALSE;
		::NewSettings.m_WFSoloSettings[1][3].m_Tilt = FALSE;
		::NewSettings.m_WFSoloSettings[1][3].m_Method3D = 0;
		::NewSettings.m_WFSoloSettings[1][3].m_MapShowSolidSurface = TRUE;
		::NewSettings.m_WFSoloSettings[1][3].m_MapShowWireMesh = TRUE;
		::NewSettings.m_WFSoloSettings[1][3].m_MapUnit = DIOPTERS;
		::NewSettings.m_WFSoloSettings[1][3].m_MapShowMap = 1;
		::NewSettings.m_WFSoloSettings[1][3].m_MapShowEye = 0;
		::NewSettings.m_WFSoloSettings[1][3].m_MapShowNumbers = 0;
		::NewSettings.m_WFSoloSettings[1][3].m_MapShowPupil = 0;
		::NewSettings.m_WFSoloSettings[1][3].m_MapShowLimbus = 0;
		::NewSettings.m_WFSoloSettings[1][3].m_MapShowCornealVertex = 0;
		::NewSettings.m_WFSoloSettings[1][3].m_MapTranslucent = 0;
		::NewSettings.m_WFSoloSettings[1][3].m_MapShowKeratometry = 0;
		::NewSettings.m_WFSoloSettings[1][3].m_ZoneRadiusUm = 5000;
		//Done

		//531 For Showing wf or ct buttons setting
		for (int i = 0; i < 8; i++)
		{
			::NewSettings.m_WFSumShow[i] = TRUE;
			::NewSettings.m_CTSumShow[i] = TRUE;
			::NewSettings.m_WFCTSumShow[i] = TRUE;
		}
		//531 Done
	}
	//Done
}
//***************************************************************************************

void CSettingsSoloDlg::OnLoadButtonClicked()
{
	CFileDialog* pDlg = new CFileDialog(TRUE, _T("set"), _T(""), OFN_HIDEREADONLY, _T("setting files (*.set)|*.set||"), this);

	CString Dir;

	CINIParser IniPar;
	Dir = IniPar.GetStrKey(IniSection, "Settings Folder", IniPath);//6.2.0 ini registration

	pDlg->m_ofn.lpstrInitialDir = Dir;
	pDlg->m_ofn.lpstrTitle = _T("Load Setting file");

	BOOL Res = pDlg->DoModal() == IDOK;
	CString FileName(pDlg->GetPathName());
	CString sFileName(pDlg->GetFileName());
	CString FileName2 = FileName + "n";
	delete pDlg;

	if (!Res) return;

	//
	if (!::Settings.LoadFromFile(FileName))
	{
		::Info("Loaded Setting file failed.");
		//::Settings.LoadFromResource();
	}
	else
	{
		Dir = FileName;
		Dir.Replace(sFileName, _T(""));

		IniPar.SetStrKey(IniSection, "Settings Folder", Dir, IniPath);//6.2.0 ini registration
	}

	m_BackSomeDefaults = FALSE; //520, If the user use load button to load backuped settings, set the default settings off
	SetSelection();
}

//***************************************************************************************

//set the default values of setting
void CSettingsSoloDlg::OnSaveButtonClicked()
{
	SetSetting();
A:
	SYSTEMTIME CurTime;
	::GetLocalTime(&CurTime);
	CString Def_FileName;
	Def_FileName.Format(_T("%02i-%02i-%04i-%02i-%02i"), CurTime.wMonth, CurTime.wDay, CurTime.wYear, CurTime.wHour, CurTime.wMinute);

	CINIParser IniPar;
	CString Dir = IniPar.GetStrKey(IniSection, "Settings Folder", IniPath);//6.2.0 ini registration

	CFileDialog* pDlg = new CFileDialog(FALSE, _T("set"), Def_FileName, OFN_OVERWRITEPROMPT, _T("setting files (*.set)|*.set||"), this);
	pDlg->m_ofn.lpstrInitialDir = Dir;
	pDlg->m_ofn.lpstrTitle = _T("Save Setting file");
	BOOL Res = pDlg->DoModal() == IDOK;
	CString FileName(pDlg->GetPathName());
	CString sFileName(pDlg->GetFileName());
	CString FileName2 = FileName + "n";
	delete pDlg;

	if (!Res) return;

	CString leftStr = FileName.Right(3);
	if (leftStr != "set")
	{
		::Info("Please use the '.set' as file format.");
		goto A;
	}

	Dir = FileName;
	Dir.Replace(sFileName, _T(""));

	IniPar.SetStrKey(IniSection, "Settings Folder", Dir, IniPath);//6.2.0 ini registration

	Settings.SaveIntoFile(FileName);
	::NewSettings.SaveIntoFile(FileName2);
	::Info("Setting file '" + FileName + "' saved successfully.");
}
//***************************************************************************************


//***************************************************************************************

void CSettingsSoloDlg::SetSelection()
{
	m_PosCylCheck.SetCheck(0);
	m_ProbingEnabledCheck.SetCheck(0);
	m_FoggingEnabledCheck.SetCheck(0);
	m_FoggingEnabledCheck.EnableWindow(0);
	m_AnimateAfterNewExamCheck.SetCheck(0);
	m_DDMMYYYYRadio.SetCheck(0);
	m_YYYYMMDDRadio.SetCheck(0);
	m_MMDDYYYYRadio.SetCheck(0);
	m_AlignmentPupilRadio.SetCheck(0);
	m_Alignment4IrdotsRadio.SetCheck(0);


	m_WavetouchCheck.SetCheck(0);
	m_AccommTargetLightOnCheck.SetCheck(0);////
	m_ShowSendARKRCheck.SetCheck(0);
	m_Round8Radio.SetCheck(0);
	m_Round100Radio.SetCheck(0);
	//

	CString s;

	m_PosCylCheck.SetCheck(::Settings.m_PositiveCylinder);

	s.Format(_T("%.1f"), ::Settings.m_VertexDistanceUm * 0.001);
	m_VertexComboBox.SelectString(-1, s);

	for (int i = 0; i < 3; i++) {
		s.Format(_T("%.1f"), ::Settings.m_ZoneDiameterUm[i] * 0.001);
		m_ZonesComboBox[i].SelectString(-1, s);
	}



	m_ProbingEnabledCheck.SetCheck(::Settings.m_ProbingEnabled);

	m_FoggingEnabledCheck.SetCheck(::Settings.m_ProbingEnabled && ::Settings.m_FoggingEnabled);
	m_FoggingEnabledCheck.EnableWindow(::Settings.m_ProbingEnabled);

	s.Format(_T("%.1f"), ::Settings.m_ManualScanDiameterUm * 0.001);
	m_ManualScanDiameterEdit.SetWindowText(s);

	s.Format(_T("%.1f"), ::Settings.m_AutoScanDiameterUm * 0.001);
	m_AutoScanDiameterEdit.SetWindowText(s);

	m_AnimateAfterNewExamCheck.SetCheck(::Settings.m_AnimateAfterNewExam);

	if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY) {
		m_DDMMYYYYRadio.SetCheck(1);
	}
	else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD) {
		m_YYYYMMDDRadio.SetCheck(1);
	}
	else {
		m_MMDDYYYYRadio.SetCheck(1);
	}

	if (::Settings.m_AlignmentMethod == ALIGNMENT_PUPIL_CENTER) {
		m_AlignmentPupilRadio.SetCheck(1);
	}
	else {
		m_Alignment4IrdotsRadio.SetCheck(1);
	}



	m_WavetouchCheck.SetCheck(::Settings.m_ShowWavetouch);
	m_AccommTargetLightOnCheck.SetCheck(!::Settings.m_AccommTargetLightOff);
	m_ShowSendARKRCheck.SetCheck(::Settings.m_ShowSendARKRButton);



	if (::Settings.m_RefractionRounding == 0) {
		m_Round8Radio.SetCheck(1);
	}
	else {
		m_Round100Radio.SetCheck(1);
	}

	//531
	m_Super_ExamCheck.SetCheck(::NewSettings.m_Super_Exam);
	if (::NewSettings.m_Super_Exam)
	{
		m_Alignment4IrdotsRadio.SetCheck(TRUE);
		m_AlignmentPupilRadio.SetCheck(FALSE);

		m_Alignment4IrdotsRadio.EnableWindow(FALSE);
		m_AlignmentPupilRadio.EnableWindow(FALSE);

		::Settings.m_AlignmentMethod = ALIGNMENT_4_IR_DOTS;
	}
	else
	{
		m_Alignment4IrdotsRadio.EnableWindow(TRUE);
		m_AlignmentPupilRadio.EnableWindow(TRUE);
	}
	//531
}



// set the default exam list width
void  CSettingsSoloDlg::SetDef_ExList_Width()
{
	for (int i = 0; i< 11; i++)
	{
		switch (i)
		{
		case 0:
			::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] = ::NewSettings.CTOS_COL_Width[i] = 0.030;
			break;
		case 1:
			::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] = ::NewSettings.CTOS_COL_Width[i] = 0.058;
			break;
		case 2:
			::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] = ::NewSettings.CTOS_COL_Width[i] = 0.046;
			break;
		case 3:
			::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] = ::NewSettings.CTOS_COL_Width[i] = 0.002;
			break;
		case 4:
			::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] = ::NewSettings.CTOS_COL_Width[i] = 0.002;
			break;
		case 5:
			::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] = ::NewSettings.CTOS_COL_Width[i] = 0.002;
			break;
		case 6:
			::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] = ::NewSettings.CTOS_COL_Width[i] = 0.002;
			break;
		case 7:
			::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] = ::NewSettings.CTOS_COL_Width[i] = 0.002;
			break;
		case 8:
			::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] = ::NewSettings.CTOS_COL_Width[i] = 0.002;
			break;
		case 9:
			::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] = ::NewSettings.CTOS_COL_Width[i] = 0.002;
			break;
		case 10:
			::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] = ::NewSettings.CTOS_COL_Width[i] = 0.3;
			break;
		}
	}
}

//***************************************************************************************

void CSettingsSoloDlg::OnTargetLightBtnClicked()
{
	if (m_AccommTargetLightOnCheck.GetCheck())
	{
		m_TargetTimeEdit.ShowWindow(SW_SHOW);
		m_TargetTimeLabel.ShowWindow(SW_SHOW);

		CString s;
		s.Format(_T("%i"), ::Settings.m_AccommTargetLightTime);
		m_TargetTimeEdit.SetWindowText(s);
	}
	else
	{
		m_TargetTimeEdit.ShowWindow(SW_HIDE);
		m_TargetTimeLabel.ShowWindow(SW_HIDE);
	}
}

//***************************************************************************************

BOOL CSettingsSoloDlg::CheckSetting()
{
	CString s_TargetTime, s_MSD, s_ASD;

	//Target Light time
	if (m_AccommTargetLightOnCheck.GetCheck())
	{
		m_TargetTimeEdit.GetWindowText(s_TargetTime);
	}
	else
	{
		s_TargetTime = "60";
	}

	if (s_TargetTime == "") goto A;
	else
	{
		if (::NotIntNum(s_TargetTime)) goto A1;
	}

	G_As = s_TargetTime;
	int TargetTime = atoi(G_As);
	if (TargetTime > 60) goto A2;
	//Done

	//Man Scan D
	m_ManualScanDiameterEdit.GetWindowText(s_MSD);

	if (s_MSD == "") goto B;
	else
	{
		if (::NotRealNum(s_MSD)) goto B1;
	}

	G_As = s_MSD;
	real_t MSD = atof(G_As);
	if (MSD < 2 || MSD > 8) goto B2;
	//Done


	//Auto Scan D
	m_AutoScanDiameterEdit.GetWindowText(s_ASD);

	if (s_ASD == "") goto C;
	else
	{
		if (::NotRealNum(s_ASD)) goto C1;
	}

	G_As = s_ASD;
	real_t ASD = atof(G_As);
	if (ASD < 2 || ASD > 8) goto C2;
	//Done

	return TRUE;

A:  ::Info("Fixation light last time can not be empty.");
	m_TargetTimeEdit.SetFocus();
	return FALSE;

A1: ::Info("Fixation light last time must be integer.");
	m_TargetTimeEdit.SetFocus();
	return FALSE;

A2: ::Info("Fixation light last time must be less than 60s.");
	m_TargetTimeEdit.SetFocus();
	return FALSE;

B:  ::Info("Manual scan diameter can not be empty.");
	m_ManualScanDiameterEdit.SetFocus();
	return FALSE;

B1: ::Info("Manual scan diameter must be real.");
	m_ManualScanDiameterEdit.SetFocus();
	return FALSE;

B2: ::Info("Manual scan diameter range is 2mm - 8mm.");
	m_ManualScanDiameterEdit.SetFocus();
	return FALSE;

C:  ::Info("Auto scan diameter can not be empty.");
	m_AutoScanDiameterEdit.SetFocus();
	return FALSE;

C1: ::Info("Auto scan diameter must be real.");
	m_AutoScanDiameterEdit.SetFocus();
	return FALSE;

C2: ::Info("Auto scan diameter range is 2mm - 8mm.");
	m_AutoScanDiameterEdit.SetFocus();
	return FALSE;
}

//530
//***************************************************************************************
//530
BOOL CSettingsSoloDlg::PreTranslateMessage(MSG* pMsg)
{
	//Number check
	if (pMsg->message == WM_KEYUP)
	{
		int Edit = 0;

		if (GetFocus() == &m_TargetTimeEdit)         Edit = 1;
		else if (GetFocus() == &m_ManualScanDiameterEdit) Edit = 2;
		else if (GetFocus() == &m_AutoScanDiameterEdit)   Edit = 3;

		if (Edit != 0)
		{
			CString DOB;
			GetFocus()->GetWindowText(DOB);

			BOOL IsNum = TRUE;
			if (Edit == 1)   IsNum = ::InputIsIntNum(DOB);
			else            IsNum = ::InputIsRealNum(DOB);

			if (!IsNum)
			{
				int len = DOB.GetLength();
				DOB = DOB.Left(len - 1);
				GetFocus()->SetWindowText(DOB);

				if (Edit == 1)  m_TargetTimeEdit.SetSel(-1);
				if (Edit == 2)  m_ManualScanDiameterEdit.SetSel(-1);
				if (Edit == 3)  m_AutoScanDiameterEdit.SetSel(-1);
			}
		}
	}
	//Done

	return CDialog::PreTranslateMessage(pMsg);
}
//530
//*********************************************************************************
//531
void CSettingsSoloDlg::OnSuperExamBtnClicked()
{
	BOOL Checked = m_Super_ExamCheck.GetCheck();

	if (Checked)
	{
		m_Alignment4IrdotsRadio.SetCheck(TRUE);
		m_AlignmentPupilRadio.SetCheck(FALSE);

		m_Alignment4IrdotsRadio.EnableWindow(FALSE);
		m_AlignmentPupilRadio.EnableWindow(FALSE);
	}
	else
	{
		m_Alignment4IrdotsRadio.EnableWindow(TRUE);
		m_AlignmentPupilRadio.EnableWindow(TRUE);

		if (m_OriAliMethod == ALIGNMENT_4_IR_DOTS)
		{
			m_Alignment4IrdotsRadio.SetCheck(TRUE);
			m_AlignmentPupilRadio.SetCheck(FALSE);
		}
		else
		{
			m_AlignmentPupilRadio.SetCheck(TRUE);
			m_Alignment4IrdotsRadio.SetCheck(FALSE);
		}
	}
}
//531
