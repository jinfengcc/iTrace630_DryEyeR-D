//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "FolderDlg.h"
#include "Registry.h"
#include "SettingsIOLSelDlg.h"
#include "TaskSettinigComboDlg.h"
#include "ExcelSettingsDlg2.h"
#include "SettingsDefaultDlg.h"
#include "INIParser.h"//6.2.0 ini registration
#include "GlobalFunctions.h"//6.2.0 Chinese Character

#include "SettingsComboDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CSettingsComboDlg, CDialog)

	ON_WM_CTLCOLOR()

	ON_WM_LBUTTONDOWN()

	ON_BN_CLICKED(IDC_WF_ENABLE_PROBING_CHECK, OnProbingButtonClicked)
	ON_BN_CLICKED(IDC_FOLDER_BROWSE, OnFolderBrowseButtonClicked)
	ON_BN_CLICKED(IDDEFAULT, OnDefaultButtonClicked)
	ON_BN_CLICKED(IDSAVE, OnSaveButtonClicked)
	ON_BN_CLICKED(IDLOAD, OnLoadButtonClicked)

	ON_BN_CLICKED(IDC_CAP_COLORIMG_CHECK, OnColImgButtonClicked)
	ON_BN_CLICKED(IDC_COLOR_IMG_RADIO1, OnColImgAutoManRadio1Clicked)
	ON_BN_CLICKED(IDC_COLOR_IMG_RADIO2, OnColImgAutoManRadio2Clicked)

	ON_BN_CLICKED(IDC_EXCEL_EXPORT_CHECK, OnExcelCheckClicked)// for excel export
	ON_BN_CLICKED(IDC_EXCEL_SETTING, OnExcelSettingBtnClicked)//  for excel export
	ON_BN_CLICKED(IDC_FOLDER_BROWSE2, OnFolderBrowse2ButtonClicked)//  for excel export

	ON_BN_CLICKED(IDIOLSELECTION2, OnIOLSelectionBtnClicked)

	ON_BN_CLICKED(IDC_ACCOMM_TARGET_LIGHT_ON_CHECK, OnTargetLightBtnClicked)

	ON_BN_CLICKED(IDC_TASKADVANCE, OnTaskAdBtnClicked)
	ON_BN_CLICKED(IDC_TASKSHORTCUT_CHECK, OnTaskShowBtnClicked)

	ON_BN_CLICKED(IDC_WFSUPEREXAM_CHK, OnSuperExamBtnClicked)

	ON_BN_CLICKED(IDC_SUBTASK1_CHECK, OnSubTasck1ChkClicked)
	ON_BN_CLICKED(IDC_SUBTASK2_CHECK, OnSubTasck2ChkClicked)
	ON_BN_CLICKED(IDC_SUBTASK3_CHECK, OnSubTasck3ChkClicked)
	ON_BN_CLICKED(IDC_SUBTASK4_CHECK, OnSubTasck4ChkClicked)
	ON_BN_CLICKED(IDC_SUBTASK5_CHECK, OnSubTasck5ChkClicked)

	ON_BN_CLICKED(IDC_TASK1_RADIO, OnSubTasck1RioClicked)
	ON_BN_CLICKED(IDC_TASK2_RADIO, OnSubTasck2RioClicked)
	ON_BN_CLICKED(IDC_TASK3_RADIO, OnSubTasck3RioClicked)
	ON_BN_CLICKED(IDC_TASK4_RADIO, OnSubTasck4RioClicked)
	ON_BN_CLICKED(IDC_TASK5_RADIO, OnSubTasck5RioClicked)

	ON_BN_CLICKED(IDC_WFSUM2_CHECK, OnWFChk2Clicked)
	ON_BN_CLICKED(IDC_WFSUM3_CHECK, OnWFChk3Clicked)
	ON_BN_CLICKED(IDC_WFSUM4_CHECK, OnWFChk4Clicked)
	ON_BN_CLICKED(IDC_WFSUM5_CHECK, OnWFChk5Clicked)
	ON_BN_CLICKED(IDC_WFSUM6_CHECK, OnWFChk6Clicked)
	ON_BN_CLICKED(IDC_WFSUM7_CHECK, OnWFChk7Clicked)
	ON_BN_CLICKED(IDC_WFSUM8_CHECK, OnWFChk8Clicked)
	ON_BN_CLICKED(IDC_WFSUM9_CHECK, OnWFChk9Clicked)//Depth of Focus Comparison'
	ON_BN_CLICKED(IDC_WF_NEARVISION_CHECK, OnWFChk10Clicked)//6.2.0 'Wf near vision'

	ON_BN_CLICKED(IDC_CTSUM1_CHECK, OnCTChk1Clicked)
	ON_BN_CLICKED(IDC_CTSUM2_CHECK, OnCTChk2Clicked)
	ON_BN_CLICKED(IDC_CTSUM3_CHECK, OnCTChk3Clicked)
	ON_BN_CLICKED(IDC_CTSUM4_CHECK, OnCTChk4Clicked)
	ON_BN_CLICKED(IDC_CTSUM5_CHECK, OnCTChk5Clicked)
	ON_BN_CLICKED(IDC_CTSUM6_CHECK, OnCTChk6Clicked)
	ON_BN_CLICKED(IDC_CTSUM7_CHECK, OnCTChk7Clicked)

	ON_BN_CLICKED(IDC_INSUM1_CHECK, OnINChk0Clicked)
	ON_BN_CLICKED(IDC_INSUM2_CHECK, OnINChk1Clicked)
	ON_BN_CLICKED(IDC_INSUM3_CHECK, OnINChk2Clicked)
	ON_BN_CLICKED(IDC_INSUM4_CHECK, OnINChk3Clicked)
	ON_BN_CLICKED(IDC_INSUM5_CHECK, OnINChk4Clicked)
	ON_BN_CLICKED(IDC_INSUM6_CHECK, OnINChk5Clicked)
	ON_BN_CLICKED(IDC_INSUM7_CHECK, OnINChk6Clicked)
	ON_BN_CLICKED(IDC_INSUM8_CHECK, OnINChk7Clicked)
	ON_BN_CLICKED(IDC_INSUM9_CHECK, OnINChk8Clicked)
	ON_BN_CLICKED(IDC_INSUM10_CHECK, OnINChk9Clicked)//WFCT Angle ka 
	ON_BN_CLICKED(IDC_INSUM12_CHECK, OnINChk10Clicked)//Toric Alignment

	ON_BN_CLICKED(IDC_WF_POS_CYL_CHECK, OnInvalidClicked)
	ON_BN_CLICKED(IDC_ROUND_1_8_RADIO, OnInvalidClicked)
	ON_BN_CLICKED(IDC_ROUND_1_100_RADIO, OnInvalidClicked)

	ON_BN_CLICKED(IDC_WF_ENABLE_FOGGING_CHECK, OnInvalidClicked)
	ON_BN_CLICKED(IDC_WF_SCAN_AFTER_NEW_EXAM_CHECK, OnInvalidClicked)
	ON_BN_CLICKED(IDC_ALIGNMENT_PUPIL_RADIO, OnInvalidClicked)
	ON_BN_CLICKED(IDC_ALIGNMENT_4IRDOTS_RADIO, OnInvalidClicked)

	ON_BN_CLICKED(IDC_DATE_FORMAT_MMDDYYYY_RADIO, OnInvalidClicked)
	ON_BN_CLICKED(IDC_DATE_FORMAT_DDMMYYYY_RADIO, OnInvalidClicked)
	ON_BN_CLICKED(IDC_DATE_FORMAT_YYYYMMDD_RADIO, OnInvalidClicked)

	ON_BN_CLICKED(IDC_SHOW_SIM_RADIO, OnInvalidClicked)
	ON_BN_CLICKED(IDC_SHOW_REF_RADIO, OnInvalidClicked)

	ON_BN_CLICKED(IDC_WAVETOUCH_CHECK, OnInvalidClicked)
	ON_BN_CLICKED(IDC_SHOW_SEND_ARKR_CHECK, OnInvalidClicked)

	ON_BN_CLICKED(IDC_JPG_RADIO, OnInvalidClicked)
	ON_BN_CLICKED(IDC_PNG_RADIO, OnInvalidClicked)
	ON_BN_CLICKED(IDC_INCLUDE_LADS_CHECK, OnInvalidClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CSettingsComboDlg::CSettingsComboDlg(CWnd* pParentWnd) : CDialog(IDD_SETTINGS_COMBO_DLG, pParentWnd)
{

}

//***************************************************************************************

void CSettingsComboDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_WF_POS_CYL_CHECK, m_PosCylCheck);
	DDX_Control(pDX, IDC_WF_POS_CYL_TEXT, m_PosCylText);
	DDX_Control(pDX, IDC_WF_VERTEX_COMBOBOX, m_VertexComboBox);
	DDX_Control(pDX, IDC_WF_ZONE_0_COMBOBOX, m_ZonesComboBox[0]);
	DDX_Control(pDX, IDC_WF_ZONE_1_COMBOBOX, m_ZonesComboBox[1]);
	DDX_Control(pDX, IDC_WF_ZONE_2_COMBOBOX, m_ZonesComboBox[2]);
	DDX_Control(pDX, IDC_WF_ENABLE_PROBING_CHECK, m_ProbingEnabledCheck);
	DDX_Control(pDX, IDC_WF_ENABLE_PROBING_TEXT, m_ProbingEnabledText);
	DDX_Control(pDX, IDC_WF_ENABLE_FOGGING_CHECK, m_FoggingEnabledCheck);
	DDX_Control(pDX, IDC_WF_ENABLE_FOGGING_TEXT, m_FoggingEnabledText);
	DDX_Control(pDX, IDC_WF_MANUAL_SCAN_D_EDIT, m_ManualScanDiameterEdit);
	DDX_Control(pDX, IDC_WF_AUTO_SCAN_D_EDIT, m_AutoScanDiameterEdit);

	DDX_Control(pDX, IDC_WF_SCAN_AFTER_NEW_EXAM_CHECK, m_AnimateAfterNewExamCheck);
	DDX_Control(pDX, IDC_WF_SCAN_AFTER_NEW_EXAM_TEXT, m_AnimateAfterNewExamText);
	DDX_Control(pDX, IDC_DATE_FORMAT_MMDDYYYY_RADIO, m_MMDDYYYYRadio);
	DDX_Control(pDX, IDC_DATE_FORMAT_DDMMYYYY_RADIO, m_DDMMYYYYRadio);
	DDX_Control(pDX, IDC_DATE_FORMAT_YYYYMMDD_RADIO, m_YYYYMMDDRadio);
	DDX_Control(pDX, IDC_DATE_FORMAT_MMDDYYYY_TEXT, m_MMDDYYYYText);
	DDX_Control(pDX, IDC_DATE_FORMAT_DDMMYYYY_TEXT, m_DDMMYYYYText);
	DDX_Control(pDX, IDC_DATE_FORMAT_YYYYMMDD_TEXT, m_YYYYMMDDText);
	DDX_Control(pDX, IDC_NAME1_EDIT, m_Button1Edit);
	DDX_Control(pDX, IDC_NAME2_EDIT, m_Button2Edit);
	DDX_Control(pDX, IDC_NAME3_EDIT, m_Button3Edit);
	DDX_Control(pDX, IDC_NAME4_EDIT, m_Button4Edit);
	DDX_Control(pDX, IDC_NAME4_EDIT2, m_Button4Edit2);
	DDX_Control(pDX, IDC_NAME5_EDIT, m_Button5Edit);
	DDX_Control(pDX, IDC_NAME6_EDIT, m_Button6Edit);
	DDX_Control(pDX, IDC_NAME7_EDIT, m_Button7Edit);
	DDX_Control(pDX, IDC_NAME8_EDIT, m_Button8Edit);
	DDX_Control(pDX, IDC_NAME8_EDIT2, m_Button8Edit2);
	DDX_Control(pDX, IDC_NAME9_EDIT, m_Button9Edit);
	DDX_Control(pDX, IDC_NAME10_EDIT, m_Button10Edit);
	DDX_Control(pDX, IDC_NAME11_EDIT, m_Button11Edit);
	DDX_Control(pDX, IDC_NAME12_EDIT, m_Button12Edit);
	DDX_Control(pDX, IDC_NAME12_EDIT3, m_Button13Edit);
	DDX_Control(pDX, IDC_NAME4_EDIT3, m_Button14Edit);//WF Depth focus comparison
	DDX_Control(pDX, IDC_NAME11_EDIT2, m_Button15Edit);//WFCT Angle K/A

	DDX_Control(pDX, IDC_WAVETOUCH_CHECK, m_WavetouchCheck);
	DDX_Control(pDX, IDC_WAVETOUCH_TEXT, m_WavetouchText);
	DDX_Control(pDX, IDC_ACCOMM_TARGET_LIGHT_ON_CHECK, m_AccommTargetLightOnCheck);
	DDX_Control(pDX, IDC_ACCOMM_TARGET_LIGHT_ON_TEXT, m_AccommTargetLightOnText);
	DDX_Control(pDX, IDC_SHOW_SEND_ARKR_CHECK, m_ShowSendARKRCheck);
	DDX_Control(pDX, IDC_SHOW_SEND_ARKR_TEXT, m_ShowSendARKRText);
	DDX_Control(pDX, IDC_JPG_RADIO, m_UseJPGFormatRadio);
	DDX_Control(pDX, IDC_JPG_TEXT, m_UseJPGFormatText);
	DDX_Control(pDX, IDC_PNG_RADIO, m_UsePNGFormatRadio);
	DDX_Control(pDX, IDC_PNG_TEXT, m_UsePNGFormatText);
	DDX_Control(pDX, IDC_INCLUDE_LADS_CHECK, m_IncludeLADsCheck);
	DDX_Control(pDX, IDC_INCLUDE_LADS_TEXT, m_IncludeLADsText);
	DDX_Control(pDX, IDC_FOLDERNAME_EDIT, m_FolderNameEdit);
	DDX_Control(pDX, IDC_FOLDER_BROWSE, m_FolderBrowseButton);
	DDX_Control(pDX, IDC_WFVRF_RADIO, m_WFVrfRadio);
	DDX_Control(pDX, IDC_STATIC_WF_POINTS, m_WFVrfText);
	DDX_Control(pDX, IDC_WFVFA_RADIO, m_WFVfaRadio);
	DDX_Control(pDX, IDC_STATIC_WF_VFA, m_WFVfaText);
	DDX_Control(pDX, IDC_WFCMP_RADIO, m_WFCmpRadio);
	DDX_Control(pDX, IDC_WFNEARVISON_RADIO, m_WFNearVisionRadio);//6.2.0
	DDX_Control(pDX, IDC_STATIC_WF_COMPARE, m_WFCmpText);
	DDX_Control(pDX, IDC_STATIC_WF_NEARVISION, m_WFNearVisonText);//6.2.0
	DDX_Control(pDX, IDC_WFSUM1_RADIO, m_WFSum1Radio);
	DDX_Control(pDX, IDC_WFSUM2_RADIO, m_WFSum2Radio);
	DDX_Control(pDX, IDC_WFSUM3_RADIO, m_WFSum3Radio);// Angle K/A
	DDX_Control(pDX, IDC_WFSUM4_RADIO, m_WFSum4Radio);
	DDX_Control(pDX, IDC_WFSUM5_RADIO, m_WFSum5Radio);
	DDX_Control(pDX, IDC_WFSUM6_RADIO, m_WFSum6Radio);

	DDX_Control(pDX, IDC_CTVRF_RADIO, m_CTVrfRadio);
	DDX_Control(pDX, IDC_STATIC_CT_VRF, m_CTVrfText);
	DDX_Control(pDX, IDC_CTDBL_RADIO, m_CTDblRadio);
	DDX_Control(pDX, IDC_STATIC_CT_ODOS, m_CTDblText);
	DDX_Control(pDX, IDC_CTCMP_RADIO, m_CTCmpRadio);
	DDX_Control(pDX, IDC_STATIC_CT_COMPARE, m_CTCmpText);
	DDX_Control(pDX, IDC_CTSUM1_RADIO, m_CTSum1Radio);
	DDX_Control(pDX, IDC_CTSUM2_RADIO, m_CTSum2Radio);
	DDX_Control(pDX, IDC_CTSUM3_RADIO, m_CTSum3Radio);
	DDX_Control(pDX, IDC_CTSUM4_RADIO, m_CTSum4Radio);
	DDX_Control(pDX, IDC_CTSUM5_RADIO, m_CTSum5Radio);

	DDX_Control(pDX, IDC_INSUM1_RADIO, m_INSum1Radio);
	DDX_Control(pDX, IDC_INSUM2_RADIO, m_INSum2Radio);
	DDX_Control(pDX, IDC_INSUM3_RADIO, m_INSum3Radio);
	DDX_Control(pDX, IDC_INSUM4_RADIO, m_INSum4Radio);
	DDX_Control(pDX, IDC_STATIC_WFCT_OU, m_INSum4Text);
	DDX_Control(pDX, IDC_INSUM7_RADIO, m_INSum5Radio);//530 531 Change to OU Overview
	DDX_Control(pDX, IDC_STATIC_WFCT_IOL, m_INSum5Text);
	DDX_Control(pDX, IDC_INSUM5_RADIO, m_INSum6Radio);  //530
	DDX_Control(pDX, IDC_STATIC_WFCT_DYSMD, m_INSum6Text);//
	DDX_Control(pDX, IDC_INSUM6_RADIO, m_INSum7Radio);
	DDX_Control(pDX, IDC_STATIC_WFCT_DYSPT, m_INSum7Text);//
	DDX_Control(pDX, IDC_INSUM8_RADIO, m_INSum8Radio);
	DDX_Control(pDX, IDC_INSUM9_RADIO, m_INSum9Radio);
	DDX_Control(pDX, IDC_INSUM10_RADIO, m_INSum10Radio);  // WFCT Angle K/A
	DDX_Control(pDX, IDC_INSUM12_RADIO, m_INSum11Radio);  // Toric Alignment
	DDX_Control(pDX, IDC_STATIC_WFCT_TORALI, m_INSum11Text);//

	DDX_Control(pDX, IDC_ALIGNMENT_PUPIL_RADIO, m_AlignmentPupilRadio);
	DDX_Control(pDX, IDC_ALIGNMENT_PUPIL_TEXT, m_AlignmentPupilText);//
	DDX_Control(pDX, IDC_ALIGNMENT_4IRDOTS_RADIO, m_Alignment4IrdotsRadio);
	DDX_Control(pDX, IDC_ALIGNMENT_4IRDOTS_TEXT, m_Alignment4IrdotsText);//
	DDX_Control(pDX, IDC_ROUND_1_8_RADIO, m_Round8Radio);
	DDX_Control(pDX, IDC_ROUND_1_8_TEXT, m_Round8Text);//
	DDX_Control(pDX, IDC_ROUND_1_100_RADIO, m_Round100Radio);
	DDX_Control(pDX, IDC_ROUND_1_100_TEXT, m_Round100Text);//
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDOK, m_OKButton);
	DDX_Control(pDX, IDDEFAULT, m_DefaultButton);
	DDX_Control(pDX, IDSAVE, m_SaveButton);
	DDX_Control(pDX, IDLOAD, m_LoadButton);
	DDX_Control(pDX, IDC_CAP_COLORIMG_CHECK, m_CapColorImgCheck);//[520]
	DDX_Control(pDX, IDC_CAP_COLORIMG_TEXT, m_CapColorImgText);//
	DDX_Control(pDX, IDC_COLOR_IMG_RADIO1, m_CapColorImgAutoRadio);//[520]
	DDX_Control(pDX, IDC_COLOR_IMG_RADIO2, m_CapColorImgManuRadio);//[520]  
	DDX_Control(pDX, IDC_COLOR_IMG_TEXT1, m_CapColorImgAutoText);//
	DDX_Control(pDX, IDC_COLOR_IMG_TEXT2, m_CapColorImgManuText);//

	DDX_Control(pDX, IDC_EXCEL_EXPORT_CHECK, m_ExcelExportCheck);//[520]
	DDX_Control(pDX, IDC_EXCEL_EXPORT_TEXT, m_ExcelExportText);//
	DDX_Control(pDX, IDC_EXCEL_SETTING, m_ExcelSettingButton);//[520]   
	DDX_Control(pDX, IDC_FOLDERNAME_EDIT2, m_FolderNameEdit2);//[520] 
	DDX_Control(pDX, IDC_FOLDER_BROWSE2, m_FolderBrowseButton2);//[520] 

	DDX_Control(pDX, IDIOLSELECTION2, m_IolSelButton);//530

	DDX_Control(pDX, IDC_TARGETTIME_EDIT, m_TargetTimeEdit);//530
	DDX_Control(pDX, IDC_TT_STATIC, m_TargetTimeLabel);//530

	DDX_Control(pDX, IDC_SHOW_SIM_RADIO, m_SimkRadio);//530
	DDX_Control(pDX, IDC_SHOW_REF_RADIO, m_RefkRadio);//530
	DDX_Control(pDX, IDC_SHOW_SIM_TEXT, m_SimkText);//
	DDX_Control(pDX, IDC_SHOW_REF_TEXT, m_RefkText);//

	DDX_Control(pDX, IDC_TASK1_RADIO, m_TaskRadio[0]);//530
	DDX_Control(pDX, IDC_TASK2_RADIO, m_TaskRadio[1]);//530
	DDX_Control(pDX, IDC_TASK3_RADIO, m_TaskRadio[2]);//530
	DDX_Control(pDX, IDC_TASK4_RADIO, m_TaskRadio[3]);//530
	DDX_Control(pDX, IDC_TASK5_RADIO, m_TaskRadio[4]);//530

	DDX_Control(pDX, IDC_TASK1_EDIT, m_TaskEdit[0]);//530
	DDX_Control(pDX, IDC_TASK2_EDIT, m_TaskEdit[1]);//530
	DDX_Control(pDX, IDC_TASK3_EDIT, m_TaskEdit[2]);//530
	DDX_Control(pDX, IDC_TASK4_EDIT, m_TaskEdit[3]);//530
	DDX_Control(pDX, IDC_TASK5_EDIT, m_TaskEdit[4]);//530

	DDX_Control(pDX, IDC_TASKADVANCE, m_TaskAdBtn);//530

	DDX_Control(pDX, IDC_TASKSHORTCUT_CHECK, m_Task_ShortCutCheck);//530
	DDX_Control(pDX, IDC_TASKSHORTCUT_TEXT, m_Task_ShortCutText);//

	DDX_Control(pDX, IDC_STATIC1, m_Static[0]);//530
	DDX_Control(pDX, IDC_STATIC2, m_Static[1]);//530
	DDX_Control(pDX, IDC_STATIC3, m_Static[2]);//530
	DDX_Control(pDX, IDC_STATIC4, m_Static[3]);//530  


	DDX_Control(pDX, IDC_WFSUPEREXAM_CHK, m_Super_ExamCheck);
	DDX_Control(pDX, IDC_WFSUPEREXAM_TEXT, m_Super_ExamText);

	DDX_Control(pDX, IDC_SUBTASK1_CHECK, m_SubTask_Check[0]);
	DDX_Control(pDX, IDC_SUBTASK2_CHECK, m_SubTask_Check[1]);
	DDX_Control(pDX, IDC_SUBTASK3_CHECK, m_SubTask_Check[2]);
	DDX_Control(pDX, IDC_SUBTASK4_CHECK, m_SubTask_Check[3]);
	DDX_Control(pDX, IDC_SUBTASK5_CHECK, m_SubTask_Check[4]);

	DDX_Control(pDX, IDC_WFSUM2_CHECK, m_WFSum_Check[1]);// WF vfa
	DDX_Control(pDX, IDC_WFSUM3_CHECK, m_WFSum_Check[2]);
	DDX_Control(pDX, IDC_WF_NEARVISION_CHECK, m_WFSum_Check[9]);// 6.2.0 WF Near Vision
	DDX_Control(pDX, IDC_WFSUM4_CHECK, m_WFSum_Check[3]);
	DDX_Control(pDX, IDC_WFSUM5_CHECK, m_WFSum_Check[4]);
	DDX_Control(pDX, IDC_WFSUM6_CHECK, m_WFSum_Check[5]);
	DDX_Control(pDX, IDC_WFSUM7_CHECK, m_WFSum_Check[6]);
	DDX_Control(pDX, IDC_WFSUM8_CHECK, m_WFSum_Check[7]);
	DDX_Control(pDX, IDC_WFSUM9_CHECK, m_WFSum_Check[8]);

	DDX_Control(pDX, IDC_CTSUM1_CHECK, m_CTSum_Check[1]);
	DDX_Control(pDX, IDC_CTSUM2_CHECK, m_CTSum_Check[2]);
	DDX_Control(pDX, IDC_CTSUM3_CHECK, m_CTSum_Check[3]);
	DDX_Control(pDX, IDC_CTSUM4_CHECK, m_CTSum_Check[4]);
	DDX_Control(pDX, IDC_CTSUM5_CHECK, m_CTSum_Check[5]);
	DDX_Control(pDX, IDC_CTSUM6_CHECK, m_CTSum_Check[6]);
	DDX_Control(pDX, IDC_CTSUM7_CHECK, m_CTSum_Check[7]);

	DDX_Control(pDX, IDC_INSUM1_CHECK, m_INSum_Check[0]);
	DDX_Control(pDX, IDC_INSUM2_CHECK, m_INSum_Check[1]);
	DDX_Control(pDX, IDC_INSUM3_CHECK, m_INSum_Check[2]);
	DDX_Control(pDX, IDC_INSUM4_CHECK, m_INSum_Check[3]);
	DDX_Control(pDX, IDC_INSUM5_CHECK, m_INSum_Check[4]);
	DDX_Control(pDX, IDC_INSUM6_CHECK, m_INSum_Check[5]);
	DDX_Control(pDX, IDC_INSUM7_CHECK, m_INSum_Check[6]);
	DDX_Control(pDX, IDC_INSUM8_CHECK, m_INSum_Check[7]);
	DDX_Control(pDX, IDC_INSUM9_CHECK, m_INSum_Check[8]);
	DDX_Control(pDX, IDC_INSUM10_CHECK, m_INSum_Check[9]);// WFCT Angle K/A
	DDX_Control(pDX, IDC_INSUM12_CHECK, m_INSum_Check[10]);// Toric Alignment

	DDX_Control(pDX, IDC_SELWFNUM_TEXT, m_SelNumText[0]);//
	DDX_Control(pDX, IDC_SELCTNUM_TEXT, m_SelNumText[1]);//
	DDX_Control(pDX, IDC_SELWFCTNUM_TEXT, m_SelNumText[2]);
}

//***************************************************************************************

BOOL CSettingsComboDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	for (int i = 0; i < 5; i++)
	{
		m_TaskChanged[i] = FALSE;
	}

	m_BackSomeDefaults = FALSE;
	m_ExcDef = FALSE;

	CRect rect;
	GetWindowRect(&rect);

	if (::Settings.m_Cap_ColorImg != TRUE && ::Settings.m_Cap_ColorImg != FALSE)
	{
		::Settings.m_Cap_ColorImg = TRUE;
	}
	m_CapColorImgCheck.SetCheck(::Settings.m_Cap_ColorImg);

	if (::Settings.m_Cap_ColorImg == TRUE)
	{
		m_CapColorImgAutoRadio.ShowWindow(SW_SHOW);
		m_CapColorImgAutoText.ShowWindow(SW_SHOW);
		m_CapColorImgManuRadio.ShowWindow(SW_SHOW);
		m_CapColorImgManuText.ShowWindow(SW_SHOW);

		m_CapColorImgAutoRadio.SetCheck(::Settings.m_Cap_ColorAutoOrManu);
		m_CapColorImgManuRadio.SetCheck(!::Settings.m_Cap_ColorAutoOrManu);
	}
	else
	{
		m_CapColorImgAutoRadio.ShowWindow(SW_HIDE);
		m_CapColorImgAutoText.ShowWindow(SW_HIDE);
		m_CapColorImgManuRadio.ShowWindow(SW_HIDE);
		m_CapColorImgManuText.ShowWindow(SW_HIDE);
	}

	m_Task_ShortCutCheck.SetCheck(::NewSettings.m_ShowTask);

	if (!::NewSettings.m_ShowTask)
	{
		HideTaskSettings();
	}

	for (int i = 0; i < 5; i++)
	{
		m_TaskRadio[i].SetCheck(FALSE);
		m_DlgTask[i].m_Enable = ::NewSettings.m_Tasks[i].m_Enable;
	}

	BOOL HasTask = FALSE;

	for (int i = 0; i < 5; i++)
	{
		if (::NewSettings.m_Tasks[i].m_Enable)
		{
			m_TaskRadio[i].SetCheck(TRUE);
			m_SelTaskRadio = i;
			HasTask = TRUE;
			break;
		}
	}

	if (!HasTask) m_TaskAdBtn.EnableWindow(FALSE);

	m_CtlDef = FALSE;
	m_GUIDef = FALSE;
	m_ExcDef = FALSE;

	// Show WF, CT, WF/CTButtons
	BOOL ShowWFBtn = FALSE;
	for (int i = 0; i <= 8; i++) //6.2.0 from i <= 7 to i <= 8, for adding new 'WF near vision'
	{
		//'Depth of focus comparison'
		if (i == 0)
		{
			ShowWFBtn = ::NewSettings.m_WFNewSumShow[i];
			m_WFSum_Check[8].SetCheck(ShowWFBtn);
		
			if (ShowWFBtn)
			{
				m_WFSum6Radio.EnableWindow(TRUE);
				m_Button14Edit.EnableWindow(TRUE);
			}
			else
			{
				m_WFSum6Radio.EnableWindow(FALSE);
				m_Button14Edit.EnableWindow(FALSE);
			}
		}

		//6.2.0 'WF near vision'
		if (i == 7)
		{
			ShowWFBtn = ::NewSettings.m_WFNewSumShow[1];
			m_WFSum_Check[9].SetCheck(ShowWFBtn);

			if (ShowWFBtn)
			{
				m_WFNearVisionRadio.EnableWindow(TRUE);
			}
			else
			{
				m_WFNearVisionRadio.EnableWindow(FALSE);
			}

			//
			ShowWFBtn = ::NewSettings.m_WFSumShow[7];
			m_WFSum_Check[7].SetCheck(ShowWFBtn);

			if (ShowWFBtn)
			{
				m_WFSum5Radio.EnableWindow(TRUE);
				m_Button4Edit.EnableWindow(TRUE);
			}
			else
			{
				m_WFSum5Radio.EnableWindow(FALSE);
				m_Button4Edit.EnableWindow(FALSE);
			}
		}
		//6.2.0 'WF near vision'
		
		if (i != 0 && i != 7 && i != 8)
		{
			//WFCT Angle k/a
			if (i == 5)
			{
				::NewSettings.m_WFSumShow[i] = FALSE;
				m_WFSum_Check[5].ShowWindow(SW_HIDE);
			}
			//WFCT Angle k/a

			ShowWFBtn = ::NewSettings.m_WFSumShow[i];
			m_WFSum_Check[i].SetCheck(ShowWFBtn);

			if (ShowWFBtn)
			{
				if (i == 1)
				{
					m_WFVfaRadio.EnableWindow(TRUE);
				}
				else if (i == 2)
				{
					m_WFCmpRadio.EnableWindow(TRUE);
				}
				else if (i == 3)
				{
					m_WFSum1Radio.EnableWindow(TRUE);
					m_Button1Edit.EnableWindow(TRUE);
				}
				else if (i == 4)
				{
					m_WFSum2Radio.EnableWindow(TRUE);
					m_Button2Edit.EnableWindow(TRUE);
				}
				else if (i == 5)
				{
					m_WFSum3Radio.EnableWindow(FALSE);
					m_Button3Edit.EnableWindow(FALSE);
				}
				else if (i == 6)
				{
					m_WFSum4Radio.EnableWindow(TRUE);
					m_Button4Edit2.EnableWindow(TRUE);
				}	
			}
			else
			{
				if (i == 1)
				{
					m_WFVfaRadio.EnableWindow(FALSE);
				}
				else if (i == 2)
				{
					m_WFCmpRadio.EnableWindow(FALSE);
				}
				else if (i == 3)
				{
					m_WFSum1Radio.EnableWindow(FALSE);
					m_Button1Edit.EnableWindow(FALSE);
				}
				else if (i == 4)
				{
					m_WFSum2Radio.EnableWindow(FALSE);
					m_Button2Edit.EnableWindow(FALSE);
				}
				else if (i == 5)
				{
					m_WFSum3Radio.EnableWindow(FALSE);
					m_Button3Edit.EnableWindow(FALSE);
				}
				else if (i == 6)
				{
					m_WFSum4Radio.EnableWindow(FALSE);
					m_Button4Edit2.EnableWindow(FALSE);
				}			
			}

			//CT
			BOOL ShowCTBtn = ::NewSettings.m_CTSumShow[i];
			m_CTSum_Check[i].SetCheck(ShowCTBtn);

			if (ShowCTBtn)
			{
				if (i == 1)
				{
					m_CTDblRadio.EnableWindow(TRUE);
				}
				else if (i == 2)
				{
					m_CTCmpRadio.EnableWindow(TRUE);
				}
				else if (i == 3)
				{
					m_CTSum1Radio.EnableWindow(TRUE);
					m_Button5Edit.EnableWindow(TRUE);
				}
				else if (i == 4)
				{
					m_CTSum2Radio.EnableWindow(TRUE);
					m_Button6Edit.EnableWindow(TRUE);
				}
				else if (i == 5)
				{
					m_CTSum3Radio.EnableWindow(TRUE);
					m_Button7Edit.EnableWindow(TRUE);
				}
				else if (i == 6)
				{
					m_CTSum4Radio.EnableWindow(TRUE);
					m_Button8Edit.EnableWindow(TRUE);
				}		
			}
			else
			{
				if (i == 1)
				{
					m_CTDblRadio.EnableWindow(FALSE);
				}
				else if (i == 2)
				{
					m_CTCmpRadio.EnableWindow(FALSE);
				}
				else if (i == 3)
				{
					m_CTSum1Radio.EnableWindow(FALSE);
					m_Button5Edit.EnableWindow(FALSE);
				}
				else if (i == 4)
				{
					m_CTSum2Radio.EnableWindow(FALSE);
					m_Button6Edit.EnableWindow(FALSE);
				}
				else if (i == 5)
				{
					m_CTSum3Radio.EnableWindow(FALSE);
					m_Button7Edit.EnableWindow(FALSE);
				}
				else if (i == 6)
				{
					m_CTSum4Radio.EnableWindow(FALSE);
					m_Button8Edit.EnableWindow(FALSE);
				}				
			}
			//CT Done
		}
		else if (i == 7)
		{
			BOOL ShowCTBtn = ::NewSettings.m_CTSumShow[i];
			m_CTSum_Check[i].SetCheck(ShowCTBtn);

			if (ShowCTBtn)
			{
				m_CTSum5Radio.EnableWindow(TRUE);
				m_Button8Edit2.EnableWindow(TRUE);
			}
			else
			{
				m_CTSum5Radio.EnableWindow(FALSE);
				m_Button8Edit2.EnableWindow(FALSE);
			}
		}

		//WF/CT
		BOOL ShowWFCTBtn = ::NewSettings.m_WFCTSumShow[i];
		m_INSum_Check[i].SetCheck(ShowWFCTBtn);

		if (ShowWFCTBtn)
		{
			if (i == 0)
			{
				m_INSum1Radio.EnableWindow(TRUE);
				m_Button9Edit.EnableWindow(TRUE);
			}
			else if (i == 1)
			{
				m_INSum2Radio.EnableWindow(TRUE);
				m_Button10Edit.EnableWindow(TRUE);
			}
			else if (i == 2)
			{
				m_INSum3Radio.EnableWindow(TRUE);
				m_Button11Edit.EnableWindow(TRUE);
			}
			else if (i == 3)
			{
				m_INSum4Radio.EnableWindow(TRUE);
			}
			else if (i == 4)
			{
				m_INSum5Radio.EnableWindow(TRUE);
			}
			else if (i == 5)
			{
				m_INSum6Radio.EnableWindow(TRUE);
			}
			else if (i == 6)
			{
				m_INSum7Radio.EnableWindow(TRUE);
			}
			else if (i == 7)
			{
				m_INSum8Radio.EnableWindow(TRUE);
				m_Button12Edit.EnableWindow(TRUE);
			}
		}
		else
		{
			if (i == 0)
			{
				m_INSum1Radio.EnableWindow(FALSE);
				m_Button9Edit.EnableWindow(FALSE);
			}
			else if (i == 1)
			{
				m_INSum2Radio.EnableWindow(FALSE);
				m_Button10Edit.EnableWindow(FALSE);
			}
			else if (i == 2)
			{
				m_INSum3Radio.EnableWindow(FALSE);
				m_Button11Edit.EnableWindow(FALSE);
			}
			else if (i == 3)
			{
				m_INSum4Radio.EnableWindow(FALSE);
			}
			else if (i == 4)
			{
				m_INSum5Radio.EnableWindow(FALSE);
			}
			else if (i == 5)
			{
				m_INSum6Radio.EnableWindow(FALSE);
			}
			else if (i == 6)
			{
				m_INSum7Radio.EnableWindow(FALSE);
			}
			else if (i == 7)
			{
				m_INSum8Radio.EnableWindow(FALSE);
				m_Button12Edit.EnableWindow(FALSE);
			}
		}
		//WF/CT Done
	}

	if (::NewSettings.m_WFCTSumShow[8])
	{
		m_INSum9Radio.EnableWindow(TRUE);
		m_Button13Edit.EnableWindow(TRUE);
		m_INSum_Check[8].SetCheck(TRUE);
	}
	else
	{
		m_INSum9Radio.EnableWindow(FALSE);
		m_Button13Edit.EnableWindow(FALSE);
		m_INSum_Check[8].SetCheck(FALSE);
	}

	// Add WFCT Angle K/A (Transform from WF Screen)
	if (::NewSettings.m_WFCTNewSumShow[0])
	{
		m_INSum10Radio.EnableWindow(TRUE);
		m_Button15Edit.EnableWindow(TRUE);
		m_INSum_Check[9].SetCheck(TRUE);
	}
	else
	{
		m_INSum10Radio.EnableWindow(FALSE);
		m_Button15Edit.EnableWindow(FALSE);
		m_INSum_Check[9].SetCheck(FALSE);
	}
	// Add WFCT Angle K/A (Transform from WF Screen) Done

	// Add WFCT Toric Alignment
	if (::NewSettings.m_WFCTNewSumShow[1])
	{
		m_INSum11Radio.EnableWindow(TRUE);
		m_INSum_Check[10].SetCheck(TRUE);
	}
	else
	{
		m_INSum11Radio.EnableWindow(FALSE);
		m_INSum_Check[10].SetCheck(FALSE);
	}
	// Add WFCT Toric Alignment Done

	CString s;

	m_PosCylCheck.SetCheck(::Settings.m_PositiveCylinder);

	for (real_t v = 0.0; v < 20.001; v += 0.5)
	{
		s.Format(_T("%.1f"), v);
		m_VertexComboBox.AddString(s);
	}
	s.Format(_T("%.1f"), ::Settings.m_VertexDistanceUm * 0.001);
	m_VertexComboBox.SelectString(-1, s);

	real_t Zone1Val = 2.0;
	if (m_isPresbia) Zone1Val = 1.3;

	for (real_t d = Zone1Val; d < 8.001; d += 0.1)
	{
		s.Format(_T("%.1f"), d);
		m_ZonesComboBox[0].AddString(s);
		m_ZonesComboBox[1].AddString(s);
		m_ZonesComboBox[2].AddString(s);
	}

	for (int i = 0; i < 3; i++)
	{
		if (!m_isPresbia && ::Settings.m_ZoneDiameterUm[i] < 2)
		{
			if (i == 0) ::Settings.m_ZoneDiameterUm[i] = 2000;
			if (i == 1) ::Settings.m_ZoneDiameterUm[i] = 3000;
			if (i == 2) ::Settings.m_ZoneDiameterUm[i] = 5000;
		}

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

	if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
	{
		m_DDMMYYYYRadio.SetCheck(1);
	}
	else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
	{
		m_YYYYMMDDRadio.SetCheck(1);
	}
	else
	{
		m_MMDDYYYYRadio.SetCheck(1);
	}

	if (::Settings.m_AlignmentMethod == ALIGNMENT_PUPIL_CENTER)
	{
		m_AlignmentPupilRadio.SetCheck(1);
	}
	else
	{
		m_Alignment4IrdotsRadio.SetCheck(1);
	}

	CString ButtonName;
	::Settings.GetButtonName(0, 0, ButtonName);
	m_Button1Edit.SetWindowText(ButtonName);

	::Settings.GetButtonName(0, 1, ButtonName);
	m_Button2Edit.SetWindowText(ButtonName);

	CString sNext;
	::Settings.GetButtonName(0, 2, sNext);

	if (ButtonName == "Angle K/A") m_Button2Edit.SetWindowText(_T("WF/RMS"));

	::Settings.GetButtonName(0, 2, ButtonName);
	m_Button3Edit.SetWindowText(ButtonName);

	::Settings.GetButtonName(0, 3, ButtonName);
	m_Button4Edit.SetWindowText(ButtonName);

	::Settings.GetButtonName(0, 4, ButtonName);
	m_Button4Edit2.SetWindowText(ButtonName);

	::NewSettings.GetNewWFBtnName(0, 0, ButtonName);
	m_Button14Edit.SetWindowText(ButtonName);

	::Settings.GetButtonName(1, 0, ButtonName);
	m_Button5Edit.SetWindowText(ButtonName);

	::Settings.GetButtonName(1, 1, ButtonName);
	m_Button6Edit.SetWindowText(ButtonName);

	::Settings.GetButtonName(1, 2, ButtonName);//3D Z-Elevation
	m_Button7Edit.SetWindowText(ButtonName);

	::Settings.GetButtonName(1, 4, ButtonName);//Osher Iris Imaging
	m_Button8Edit.SetWindowText(ButtonName);

	::Settings.GetButtonName(1, 3, ButtonName);//Custom
	m_Button8Edit2.SetWindowText(ButtonName);

	::Settings.GetButtonName(2, 0, ButtonName);//Chang Analysis
	m_Button9Edit.SetWindowText(ButtonName);

	::Settings.GetButtonName(2, 1, ButtonName);//WFCT Summary
	m_Button10Edit.SetWindowText(ButtonName);

	::Settings.GetButtonName(2, 2, ButtonName);//WFCT Visual Acuity
	m_Button11Edit.SetWindowText(ButtonName);

	::Settings.GetButtonName(2, 4, ButtonName);//Astigmatism source
	m_Button12Edit.SetWindowText(ButtonName);

	::Settings.GetButtonName(2, 3, ButtonName);
	m_Button13Edit.SetWindowText(ButtonName);

	// WFCT Angle k/a
	::Settings.GetButtonName(2, 7, ButtonName);// WFCT Angle K/A
	m_Button15Edit.SetWindowText(ButtonName);
	// WFCT Angle k/a

	switch (::Settings.m_DefaultWFSum)
	{
	case 10: m_WFVfaRadio.SetCheck(1); break;
	case 11: m_WFVrfRadio.SetCheck(1); break;
	case 12: m_WFCmpRadio.SetCheck(1); break;
	case 13: m_WFSum6Radio.SetCheck(1); break;
	case  0: m_WFSum1Radio.SetCheck(1); break;
	case  1: m_WFSum2Radio.SetCheck(1); break;
		//case  2: m_WFSum3Radio.SetCheck(1); break;// WFCT Angle k/a
	case  2: m_WFVfaRadio.SetCheck(1); break;// WFCT Angle k/a
	case  4: m_WFSum4Radio.SetCheck(1); break;//depth of focus
	case  3: m_WFSum5Radio.SetCheck(1); break;//4 : Custom
	case 14: m_WFNearVisionRadio.SetCheck(1); break;//6.2.0
	}
	switch (::Settings.m_DefaultCTSum)
	{
	case 10: m_CTDblRadio.SetCheck(1); break;
	case 11: m_CTVrfRadio.SetCheck(1); break;
	case 12: m_CTCmpRadio.SetCheck(1); break;
	case  0: m_CTSum1Radio.SetCheck(1); break;
	case  1: m_CTSum2Radio.SetCheck(1); break;
	case  2: m_CTSum3Radio.SetCheck(1); break;
	case  3: m_CTSum4Radio.SetCheck(1); break;
	case  4: m_CTSum5Radio.SetCheck(1); break;
	}
	switch (::Settings.m_DefaultINSum)
	{
	case  0: m_INSum1Radio.SetCheck(1); break;
	case  1: m_INSum2Radio.SetCheck(1); break;
	case  2: m_INSum3Radio.SetCheck(1); break;
	case  3: m_INSum4Radio.SetCheck(1); break;
	case  6: m_INSum7Radio.SetCheck(1); break;
	case  10: m_INSum6Radio.SetCheck(1); break;
	case  7: m_INSum5Radio.SetCheck(1); break;
	case  8: m_INSum8Radio.SetCheck(1); break;
	case  9: m_INSum9Radio.SetCheck(1); break;
	case  4: m_INSum10Radio.SetCheck(1); break;// Add WFCT Angle K/A
	case  11: m_INSum11Radio.SetCheck(1); break;// Toric Alignment abc
	}


	m_Button1Edit.ShowWindow(SW_SHOW);
	m_Button2Edit.ShowWindow(SW_SHOW);

	m_Button5Edit.ShowWindow(SW_SHOW);
	m_Button6Edit.ShowWindow(SW_SHOW);

	m_Button9Edit.ShowWindow(SW_SHOW);
	m_Button10Edit.ShowWindow(SW_SHOW);

	((CWnd*)GetDlgItem(IDC_NAMES1_STATIC))->ShowWindow(SW_SHOW);
	((CWnd*)GetDlgItem(IDC_NAMES2_STATIC))->ShowWindow(SW_SHOW);
	((CWnd*)GetDlgItem(IDC_NAMES3_STATIC))->ShowWindow(SW_SHOW);
	m_WFVrfRadio.ShowWindow(SW_SHOW);
	m_WFVfaRadio.ShowWindow(SW_SHOW);
	m_WFCmpRadio.ShowWindow(SW_SHOW);
	m_WFSum1Radio.ShowWindow(SW_SHOW);
	m_WFSum2Radio.ShowWindow(SW_SHOW);

	//WF
	m_Button3Edit.ShowWindow(SW_HIDE);// WFCT Angle k/a
	m_Button4Edit.ShowWindow(SW_SHOW);
	m_Button4Edit2.ShowWindow(SW_SHOW);
	m_WFSum3Radio.ShowWindow(SW_HIDE);// WFCT Angle k/a
	m_WFSum4Radio.ShowWindow(SW_SHOW);
	m_WFSum5Radio.ShowWindow(SW_SHOW);
	m_Button14Edit.ShowWindow(SW_SHOW);
	m_WFNearVisionRadio.ShowWindow(SW_SHOW);

	//CT
	m_Button7Edit.ShowWindow(SW_SHOW);
	m_Button8Edit.ShowWindow(SW_SHOW);
	m_Button8Edit2.ShowWindow(SW_SHOW);
	m_CTSum3Radio.ShowWindow(SW_SHOW);
	m_CTSum4Radio.ShowWindow(SW_SHOW);
	m_CTSum5Radio.ShowWindow(SW_SHOW);

	//WF and CT
	m_Button11Edit.ShowWindow(SW_SHOW);
	m_Button12Edit.ShowWindow(SW_SHOW);
	m_Button13Edit.ShowWindow(SW_SHOW);
	m_Button15Edit.ShowWindow(SW_SHOW);// WFCT Angle k/a

	m_INSum1Radio.ShowWindow(SW_SHOW);
	m_INSum3Radio.ShowWindow(SW_SHOW);
	m_INSum4Radio.ShowWindow(SW_SHOW);
	m_INSum5Radio.ShowWindow(SW_SHOW);
	m_INSum6Radio.ShowWindow(SW_SHOW);
	m_INSum7Radio.ShowWindow(SW_SHOW);
	m_INSum8Radio.ShowWindow(SW_SHOW);
	m_INSum9Radio.ShowWindow(SW_SHOW);
	m_INSum10Radio.ShowWindow(SW_SHOW);// WFCT 'Angle k/a'
	m_INSum11Radio.ShowWindow(SW_SHOW);// Toric Alignment  

	m_CTVrfRadio.ShowWindow(SW_SHOW);
	m_CTDblRadio.ShowWindow(SW_SHOW);
	m_CTCmpRadio.ShowWindow(SW_SHOW);
	m_CTSum1Radio.ShowWindow(SW_SHOW);
	m_CTSum2Radio.ShowWindow(SW_SHOW);

	m_INSum2Radio.ShowWindow(SW_SHOW);


	m_WavetouchCheck.SetCheck(::Settings.m_ShowWavetouch);
	m_AccommTargetLightOnCheck.SetCheck(!::Settings.m_AccommTargetLightOff);

	if (::Settings.m_AccommTargetLightOff)
	{
		m_TargetTimeEdit.ShowWindow(SW_HIDE);
		m_TargetTimeLabel.ShowWindow(SW_HIDE);
	}
	else
	{
		m_TargetTimeEdit.ShowWindow(SW_SHOW);
		m_TargetTimeLabel.ShowWindow(SW_SHOW);

		s.Format(_T("%i"), ::Settings.m_AccommTargetLightTime);
		m_TargetTimeEdit.SetWindowText(s);
	}

	if (::Settings.m_ShowSimK == TRUE)
	{
		m_SimkRadio.SetCheck(TRUE);
		m_RefkRadio.SetCheck(FALSE);
	}
	else
	{
		m_SimkRadio.SetCheck(FALSE);
		m_RefkRadio.SetCheck(TRUE);
	}

	CString taskName = ::NewSettings.ReadTaskName(0);//6.2.0 Chinese
	m_TaskEdit[0].SetWindowText(taskName);

	if (!::NewSettings.m_Tasks[0].m_Enable)
	{
		m_TaskEdit[0].EnableWindow(FALSE);
		m_TaskRadio[0].EnableWindow(FALSE);
		m_SubTask_Check[0].SetCheck(FALSE);
	}
	else m_SubTask_Check[0].SetCheck(TRUE);

	taskName = ::NewSettings.ReadTaskName(1);//6.2.0 Chinese
	m_TaskEdit[1].SetWindowText(taskName);
	if (!::NewSettings.m_Tasks[1].m_Enable)
	{
		m_TaskEdit[1].EnableWindow(FALSE);
		m_TaskRadio[1].EnableWindow(FALSE);
		m_SubTask_Check[1].SetCheck(FALSE);
	}
	else m_SubTask_Check[1].SetCheck(TRUE);

	taskName = ::NewSettings.ReadTaskName(2);//6.2.0 Chinese
	m_TaskEdit[2].SetWindowText(taskName);
	if (!::NewSettings.m_Tasks[2].m_Enable)
	{
		m_TaskEdit[2].EnableWindow(FALSE);
		m_TaskRadio[2].EnableWindow(FALSE);
		m_SubTask_Check[2].SetCheck(FALSE);
	}
	else m_SubTask_Check[2].SetCheck(TRUE);

	taskName = ::NewSettings.ReadTaskName(3);//6.2.0 Chinese  
	{
		m_TaskEdit[3].SetWindowText(taskName);
		if (!::NewSettings.m_Tasks[3].m_Enable)
		{
			m_TaskEdit[3].EnableWindow(FALSE);
			m_TaskRadio[3].EnableWindow(FALSE);
			m_SubTask_Check[3].SetCheck(FALSE);
		}
		else m_SubTask_Check[3].SetCheck(TRUE);
	}

	taskName = ::NewSettings.ReadTaskName(4);//6.2.0 Chinese  
	if (taskName == "Accommdation Analysis")
	{
		taskName = "Accomm and DOF Analysis";
	}

	m_TaskEdit[4].SetWindowText(taskName);
	if (!::NewSettings.m_Tasks[4].m_Enable)
	{
		m_TaskEdit[4].EnableWindow(FALSE);
		m_TaskRadio[4].EnableWindow(FALSE);
		m_SubTask_Check[4].SetCheck(FALSE);
	}
	else m_SubTask_Check[4].SetCheck(TRUE);

	m_TaskEdit[0].SetLimitText(23);
	m_TaskEdit[1].SetLimitText(23);
	m_TaskEdit[2].SetLimitText(23);
	m_TaskEdit[3].SetLimitText(23);
	m_TaskEdit[4].SetLimitText(23);
	//530

	m_ShowSendARKRCheck.SetCheck(::Settings.m_ShowSendARKRButton);

	// Export  
	m_UseJPGFormatRadio.SetCheck(::Settings.m_UseJPGFormat);
	m_UsePNGFormatRadio.SetCheck(!::Settings.m_UseJPGFormat);
	m_IncludeLADsCheck.SetCheck(::Settings.m_IncludeLADs);

	CINIParser IniPar;
	CString FolderName = IniPar.GetStrKey(IniSection, "Export Folder", IniPath);//6.2.0 ini registration;

	if (!::PathFileExists(FolderName))
	{
		FolderName = MAIN_DIR + "\\Export Data";
		IniPar.SetStrKey(IniSection, "Export Folder", FolderName, IniPath);//6.2.0 ini registration

		::CreateDirectory(FolderName, NULL);
	}
	m_FolderNameEdit.SetWindowText(FolderName);

	((CWnd*)GetDlgItem(IDC_EXPORT_GROUPBOX))->ShowWindow(SW_SHOW);
	m_FolderNameEdit.ShowWindow(SW_SHOW);
	m_FolderBrowseButton.ShowWindow(SW_SHOW);
	m_UseJPGFormatRadio.ShowWindow(SW_SHOW);
	m_UsePNGFormatRadio.ShowWindow(SW_SHOW);
	m_IncludeLADsCheck.ShowWindow(SW_SHOW);

	if (::Settings.m_RefractionRounding == 0)
	{
		m_Round8Radio.SetCheck(1);
	}
	else
	{
		m_Round100Radio.SetCheck(1);
	}

	//Excel export check
	m_ExcelExportCheck.SetCheck(::Settings.m_Export_Excel);

	FolderName = IniPar.GetStrKey(IniSection, "Export Excel Folder", IniPath);//6.2.0 ini registration

	if (!::PathFileExists(FolderName))
	{
		FolderName = MAIN_DIR + "\\Export Excel Data";
		IniPar.SetStrKey(IniSection, "Export Excel Folder", FolderName, IniPath);//6.2.0 ini registration
		::CreateDirectory(FolderName, NULL);
	}
	m_FolderNameEdit2.SetWindowText(FolderName);
	//Excel export check Done

	RECT Rect;
	GetWindowRect(&Rect);
	int ww = Rect.right - Rect.left;
	int wh = Rect.bottom - Rect.top;

	CenterWindow();

	//530 IOL Selection data set
	m_IOLSel_Alpha[0] = ::NewSettings.m_IOLSel_Alpha[0];
	m_IOLSel_Alpha[1] = ::NewSettings.m_IOLSel_Alpha[1];

	m_IOLSel_CHOA = ::NewSettings.m_IOLSel_CHOA;
	m_IOLSel_IHOA = ::NewSettings.m_IOLSel_IHOA;

	m_IOLSel_Astig = ::NewSettings.m_IOLSel_Astig;

	m_IOLSel_Sph_Abbe[0] = ::NewSettings.m_IOLSel_Sph_Abbe[0];
	m_IOLSel_Sph_Abbe[1] = ::NewSettings.m_IOLSel_Sph_Abbe[1];
	m_IOLSel_Sph_Abbe[2] = ::NewSettings.m_IOLSel_Sph_Abbe[2];
	m_IOLSel_Sph_Abbe[3] = ::NewSettings.m_IOLSel_Sph_Abbe[3];
	//Done

	//530
	if (m_isComboViewer)
	{
		m_ProbingEnabledCheck.EnableWindow(FALSE);
		m_ProbingEnabledText.EnableWindow(FALSE);

		m_FoggingEnabledCheck.EnableWindow(FALSE);
		m_FoggingEnabledText.EnableWindow(FALSE);

		m_ManualScanDiameterEdit.EnableWindow(FALSE);
		m_AutoScanDiameterEdit.EnableWindow(FALSE);


		m_AccommTargetLightOnCheck.EnableWindow(FALSE);
		m_AccommTargetLightOnText.EnableWindow(FALSE);


		m_AlignmentPupilRadio.EnableWindow(FALSE);
		m_AlignmentPupilText.EnableWindow(FALSE);

		m_Alignment4IrdotsRadio.EnableWindow(FALSE);
		m_Alignment4IrdotsText.EnableWindow(FALSE);

		m_TargetTimeEdit.EnableWindow(FALSE);
		m_TargetTimeLabel.EnableWindow(FALSE);

		m_AnimateAfterNewExamCheck.EnableWindow(FALSE);
		m_AnimateAfterNewExamText.EnableWindow(FALSE);

		m_CapColorImgCheck.EnableWindow(FALSE);
		m_CapColorImgAutoRadio.EnableWindow(FALSE);
		m_CapColorImgManuRadio.EnableWindow(FALSE);

		m_CapColorImgText.EnableWindow(FALSE);
		m_CapColorImgAutoText.EnableWindow(FALSE);
		m_CapColorImgManuText.EnableWindow(FALSE);

		m_Super_ExamCheck.EnableWindow(FALSE);
		m_Super_ExamText.EnableWindow(FALSE);

		for (int i = 0; i < 4; i++)
		{
			m_Static[i].EnableWindow(FALSE);
		}
	}
	//530

	//Super WF Exam
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

	int NumChk = 0;
	for (int j = 1; j <= 8; j++)
	{
		if (m_WFSum_Check[j].GetCheck())
			NumChk++;
	}
	s.Format(_T("%i"), NumChk + 1);
	m_SelNumText[0].SetWindowText(s);

	NumChk = 0;
	for (int j = 1; j < 8; j++)
	{
		if (m_CTSum_Check[j].GetCheck())
			NumChk++;
	}
	s.Format(_T("%i"), NumChk + 1);
	m_SelNumText[1].SetWindowText(s);

	NumChk = 0;
	for (int j = 10; j >= 0; j--)
	{
		if (m_INSum_Check[j].GetCheck())
		{
			NumChk++;
		}
	}
	s.Format(_T("%i"), NumChk);
	m_SelNumText[2].SetWindowText(s);

	return TRUE;
}

//***************************************************************************************

BOOL CSettingsComboDlg::PreTranslateMessage(MSG* pMsg)
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

	return CDialog::PreTranslateMessage(pMsg);
}

//***************************************************************************************

void CSettingsComboDlg::OnOK()
{
	if (!CheckSetting()) return;// check if there are some dis-allowed inputs 

	SetSetting();//change the original code here to function 'SetSetting'

	CDialog::OnOK();
}

//***************************************************************************************

void CSettingsComboDlg::OnProbingButtonClicked()
{
	m_FoggingEnabledCheck.SetCheck(m_ProbingEnabledCheck.GetCheck() && m_FoggingEnabledCheck.GetCheck());
	m_FoggingEnabledCheck.EnableWindow(m_ProbingEnabledCheck.GetCheck());

	Invalidate(FALSE);
}

//***************************************************************************************

// set the default values of setting
void CSettingsComboDlg::OnDefaultButtonClicked()
{
	
	CSettingsDefaultDlg* pDlg = new CSettingsDefaultDlg(this);

	if (pDlg->DoModal() == IDOK)
	{
		m_CtlDef = pDlg->m_CtlDef;
		m_GUIDef = pDlg->m_GUIDef;
		m_ExcDef = pDlg->m_ExcDef;
	}
	else
	{
		m_CtlDef = FALSE;
		m_GUIDef = FALSE;
		m_ExcDef = FALSE;
	}
	delete pDlg;
	

	CString s;

	if (m_CtlDef)
	{
		//530
		m_Task_ShortCutCheck.SetCheck(1);

		ShowTaskSettings();

		m_TaskRadio[0].SetCheck(1);
		m_TaskRadio[1].SetCheck(0);
		m_TaskRadio[2].SetCheck(0);
		m_TaskRadio[3].SetCheck(0);
		m_TaskRadio[4].SetCheck(0);

		//
		m_SubTask_Check[0].SetCheck(TRUE);
		//

		m_SelTaskRadio = 0;
		//530

		for (int i = 1; i < 5; i++) m_SubTask_Check[i].SetCheck(FALSE); //

		m_Super_ExamCheck.SetCheck(0);

		if (!m_isComboViewer)//
		{
			m_Alignment4IrdotsRadio.EnableWindow(TRUE);
			m_AlignmentPupilRadio.EnableWindow(TRUE);
		}

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

		m_ProbingEnabledCheck.SetCheck(1);
		m_FoggingEnabledCheck.SetCheck(1);
		if (!m_isComboViewer) m_FoggingEnabledCheck.EnableWindow(1);

		s.Format(_T("%.1f"), 3.0);
		m_ManualScanDiameterEdit.SetWindowText(s);

		s.Format(_T("%.1f"), 6.5);
		m_AutoScanDiameterEdit.SetWindowText(s);

		m_AnimateAfterNewExamCheck.SetCheck(0);

		m_DDMMYYYYRadio.SetCheck(0);
		m_YYYYMMDDRadio.SetCheck(0);
		m_MMDDYYYYRadio.SetCheck(1);

		m_AlignmentPupilRadio.SetCheck(FALSE);
		m_Alignment4IrdotsRadio.SetCheck(TRUE);

		{
			m_WFVfaRadio.SetCheck(FALSE);
			m_WFVrfRadio.SetCheck(1);
			m_WFCmpRadio.SetCheck(FALSE);
			m_WFSum1Radio.SetCheck(FALSE);
			m_WFSum2Radio.SetCheck(FALSE);
			m_WFSum3Radio.SetCheck(FALSE);
			m_WFSum4Radio.SetCheck(FALSE);
			m_WFSum6Radio.SetCheck(FALSE);
			m_WFSum5Radio.SetCheck(FALSE);
			m_WFNearVisionRadio.SetCheck(FALSE);//6.2.0

			m_CTDblRadio.SetCheck(0);
			m_CTVrfRadio.SetCheck(1);
			m_CTCmpRadio.SetCheck(0);
			m_CTSum1Radio.SetCheck(0);
			m_CTSum2Radio.SetCheck(0);
			m_CTSum3Radio.SetCheck(0);
			m_CTSum4Radio.SetCheck(0);
			m_CTSum5Radio.EnableWindow(FALSE);

			m_INSum1Radio.SetCheck(0);
			m_INSum2Radio.SetCheck(0);
			m_INSum3Radio.SetCheck(0);
			m_INSum4Radio.SetCheck(0);
			m_INSum5Radio.SetCheck(0);//530 
			m_INSum6Radio.SetCheck(0);//530 
			m_INSum7Radio.SetCheck(1);
			m_INSum8Radio.SetCheck(0);
			m_INSum9Radio.SetCheck(0);
			m_INSum10Radio.SetCheck(0); // WFCT 'Angle k/a'
			m_INSum11Radio.SetCheck(0); // Toric Alignment
			m_INSum8Radio.EnableWindow(FALSE);
			m_INSum9Radio.EnableWindow(FALSE);
		}

		m_WavetouchCheck.SetCheck(0);
		m_AccommTargetLightOnCheck.SetCheck(1);

		//530
		m_TargetTimeEdit.ShowWindow(SW_SHOW);
		m_TargetTimeLabel.ShowWindow(SW_SHOW);

		m_TargetTimeEdit.SetWindowText(_T("60"));

		m_SimkRadio.SetCheck(FALSE);
		m_RefkRadio.SetCheck(TRUE);

		m_TaskEdit[0].SetWindowText(_T("Cataract Evaluation"));
		m_TaskEdit[0].EnableWindow(TRUE);
		m_TaskRadio[0].EnableWindow(TRUE);
		m_TaskEdit[1].SetWindowText(_T("Vision Diagnostics"));
		m_TaskEdit[1].EnableWindow(FALSE);
		m_TaskRadio[1].EnableWindow(FALSE);
		m_TaskEdit[2].SetWindowText(_T("Corneal Diagnostics"));
		m_TaskEdit[2].EnableWindow(FALSE);
		m_TaskRadio[2].EnableWindow(FALSE);
		m_TaskEdit[3].SetWindowText(_T("Toric Planner"));
		m_TaskEdit[3].EnableWindow(FALSE);
		m_TaskRadio[3].EnableWindow(FALSE);
		m_TaskEdit[4].SetWindowText(_T("Accomm and DOF Analysis"));
		m_TaskEdit[4].EnableWindow(FALSE);
		m_TaskRadio[4].EnableWindow(FALSE);

		m_TaskAdBtn.EnableWindow(TRUE);

		for (int i = 0; i < 5; i++) m_DlgTask[i].m_Enable = TRUE;
		//530

		m_ShowSendARKRCheck.SetCheck(0);


		m_Button1Edit.EnableWindow(FALSE);//WF Visual Acuity
		m_Button2Edit.EnableWindow(FALSE);//WF WF/RMS
		m_Button3Edit.EnableWindow(FALSE);//WF Angle K/A
		m_Button4Edit.EnableWindow(FALSE);//WF Custom
		m_Button4Edit2.EnableWindow(FALSE);//WF Depth of Focus
		m_Button14Edit.EnableWindow(FALSE);//WF  'Depth of Focus Comparison'

		m_Button5Edit.EnableWindow(TRUE);//CT Summary
		m_Button6Edit.EnableWindow(TRUE);//CT Keratometry 
		m_Button7Edit.EnableWindow(FALSE);//CT 3D Z-Elevation
		m_Button8Edit.EnableWindow(FALSE);//CT Osher Iris Imaging 6.2.0 Change to false
		m_Button8Edit2.EnableWindow(FALSE);//CT Custom

		m_Button9Edit.EnableWindow(TRUE);//WF&CT Chang Analysis
		m_Button10Edit.EnableWindow(TRUE);//WF&CT Summary
		m_Button11Edit.EnableWindow(TRUE);//WF&CT Visual Acuity
		m_Button12Edit.EnableWindow(FALSE);//WF&CT Astigmatism Source
		m_Button13Edit.EnableWindow(FALSE);//WF&CT Custom
		m_Button15Edit.EnableWindow(TRUE);// WFCT Angle ka

		m_WFVrfRadio.EnableWindow(TRUE);
		m_WFVfaRadio.EnableWindow(FALSE);
		m_WFCmpRadio.EnableWindow(TRUE);
		m_WFSum1Radio.EnableWindow(FALSE);
		m_WFSum2Radio.EnableWindow(FALSE);// Let the WF/RMS be disable	  
		m_WFSum3Radio.EnableWindow(FALSE);// WFCT Angle ka
		m_WFSum4Radio.EnableWindow(FALSE);
		m_WFSum6Radio.EnableWindow(FALSE);
		m_WFSum5Radio.EnableWindow(FALSE);
		m_WFNearVisionRadio.EnableWindow(FALSE);//6.2.0

		m_CTVrfRadio.EnableWindow(TRUE);
		m_CTDblRadio.EnableWindow(TRUE);//CT OD/OS //6.2.0
		m_CTCmpRadio.EnableWindow(TRUE);
		m_CTSum1Radio.EnableWindow(TRUE);
		m_CTSum2Radio.EnableWindow(TRUE);//6.2.0
		m_CTSum3Radio.EnableWindow(FALSE);//CT 3D Z-elevation
		m_CTSum4Radio.EnableWindow(FALSE);
		m_CTSum5Radio.EnableWindow(FALSE);

		m_INSum1Radio.EnableWindow(TRUE);
		m_INSum2Radio.EnableWindow(TRUE);
		m_INSum3Radio.EnableWindow(FALSE);
		m_INSum4Radio.EnableWindow(TRUE);
		m_INSum5Radio.EnableWindow(FALSE);//530
		m_INSum6Radio.EnableWindow(FALSE);//530
		m_INSum7Radio.EnableWindow(TRUE);
		m_INSum8Radio.EnableWindow(FALSE);
		m_INSum9Radio.EnableWindow(FALSE);
		m_INSum10Radio.EnableWindow(TRUE);// WFCT 'Angle k/a'


		m_INSum_Check[0].SetCheck(TRUE);
		for (int i = 1; i < 7; i++)
		{
			m_WFSum_Check[i].SetCheck(TRUE);
			m_CTSum_Check[i].SetCheck(TRUE);
			m_INSum_Check[i].SetCheck(TRUE);
		}

		m_WFSum_Check[1].SetCheck(FALSE);// 6.2.0 WF vrf disable
		m_WFSum_Check[4].SetCheck(FALSE);// WF/RMS disable
		m_WFSum_Check[5].SetCheck(FALSE);// WFCT Angle k/a
		m_WFSum_Check[6].SetCheck(FALSE);// Depth of focus disable
		m_WFSum_Check[8].SetCheck(FALSE);//
		m_WFSum_Check[9].SetCheck(FALSE);//6.2.0

		m_WFSum_Check[3].SetCheck(FALSE);
		m_WFSum_Check[7].SetCheck(FALSE);

		//m_CTSum_Check[1].SetCheck(FALSE);//6.2.0
		m_CTSum_Check[5].SetCheck(FALSE);
		m_CTSum_Check[6].SetCheck(FALSE);//6.2.0
		m_CTSum_Check[7].SetCheck(FALSE);
		
		m_INSum_Check[3].SetCheck(FALSE);//6.2.0
		m_INSum_Check[4].SetCheck(FALSE);//
		m_INSum_Check[5].SetCheck(FALSE);//
		m_INSum_Check[7].SetCheck(FALSE);
		m_INSum_Check[8].SetCheck(FALSE);
		m_INSum_Check[9].SetCheck(TRUE);// WFCT 'Angle k/a'	  
		m_INSum_Check[10].SetCheck(TRUE);// Toric Alignment

										 //[5.1.1] 
		{
			CString ButtonName;
			m_Button1Edit.SetWindowText(_T("Visual Acuity"));

			m_Button2Edit.SetWindowText(_T("WF/RMS"));
			m_Button3Edit.SetWindowText(_T("Angle K/A"));

			m_Button4Edit.SetWindowText(_T("Custom"));
			m_Button4Edit2.SetWindowText(_T("Depth of Focus"));
			m_Button5Edit.SetWindowText(_T("CT Summary"));
			m_Button6Edit.SetWindowText(_T("Keratometry"));
			m_Button14Edit.SetWindowText(_T("Depth of Focus Comparison"));//

																		  //[520]Osher alignment		
			m_Button7Edit.SetWindowText(_T("3D Z-Elevation"));
			m_Button8Edit.SetWindowText(_T("Osher Iris Imaging"));
			m_Button8Edit2.SetWindowText(_T("Custom"));
			//[520]

			m_Button9Edit.SetWindowText(_T("Chang Analysis"));
			m_Button10Edit.SetWindowText(_T("Summary"));
			m_Button11Edit.SetWindowText(_T("M T F"));
			m_Button12Edit.SetWindowText(_T("Astigmatism Source"));
			m_Button13Edit.SetWindowText(_T("Custom"));
			m_Button15Edit.SetWindowText(_T("Angle K/A"));// WFCT Angle ka
		}
		//[5.1.1]

		// Export
		m_UseJPGFormatRadio.SetCheck(0);
		m_UsePNGFormatRadio.SetCheck(1);
		m_IncludeLADsCheck.SetCheck(0);
		//
		CString FolderName = "C:\\Tracey\\Export Data";
		m_FolderNameEdit.SetWindowText(FolderName);

		FolderName = "C:\\Tracey\\Export Excel Data";
		m_FolderNameEdit2.SetWindowText(FolderName);

		//520 Excel export
		m_ExcelExportCheck.SetCheck(0);
		//520

		//520
		m_CapColorImgCheck.SetCheck(FALSE);
		m_CapColorImgAutoRadio.ShowWindow(SW_HIDE);
		m_CapColorImgAutoText.ShowWindow(SW_HIDE);
		m_CapColorImgManuRadio.ShowWindow(SW_HIDE);
		m_CapColorImgManuText.ShowWindow(SW_HIDE);

		// 
		m_SelNumText[0].SetWindowText(_T("3"));
		m_SelNumText[1].SetWindowText(_T("5"));
		m_SelNumText[2].SetWindowText(_T("5"));
		//

		Invalidate(FALSE);//
	}

	//For other default settings

	//[520]
	//Set other default settings    
	if (m_GUIDef || m_ExcDef) m_BackSomeDefaults = TRUE; //530
														 //Done

														 //530 Set default task
	for (int i = 0; i < 5; i++)	  m_TaskChanged[i] = TRUE;

	m_DlgTask[0] = SetDefTask0();
	m_DlgTask[1] = SetDefTask1();
	m_DlgTask[2] = SetDefTask2();
	m_DlgTask[3] = SetDefTask3();
	m_DlgTask[4] = SetDefTask4();
}


//***************************************************************************************

BOOL CSettingsComboDlg::CheckSetting()
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

//***************************************************************************************

void CSettingsComboDlg::SetSetting()
{
	CString s;

	::Settings.m_PositiveCylinder = m_PosCylCheck.GetCheck();

	m_VertexComboBox.GetLBText(m_VertexComboBox.GetCurSel(), s);
	G_As = s;
	::Settings.m_VertexDistanceUm = (int)(atof(G_As) * 1000.0);

	for (int i = 0; i < 3; i++)
	{
		m_ZonesComboBox[i].GetLBText(m_ZonesComboBox[i].GetCurSel(), s);
		G_As = s;
		::Settings.m_ZoneDiameterUm[i] = (int)(atof(G_As) * 1000.0);
	}

	::Settings.m_Cap_ColorImg = m_CapColorImgCheck.GetCheck();//520
	::Settings.m_Cap_ColorAutoOrManu = m_CapColorImgAutoRadio.GetCheck();//520

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

	if (m_DDMMYYYYRadio.GetCheck())
	{
		::Settings.m_DateFormat = DATE_FORMAT_DD_MM_YYYY;
	}
	else if (m_YYYYMMDDRadio.GetCheck())
	{
		::Settings.m_DateFormat = DATE_FORMAT_YYYY_MM_DD;
	}
	else {
		::Settings.m_DateFormat = DATE_FORMAT_MM_DD_YYYY;
	}

	if (m_AlignmentPupilRadio.GetCheck())
	{
		::Settings.m_AlignmentMethod = ALIGNMENT_PUPIL_CENTER;
	}
	else
	{
		::Settings.m_AlignmentMethod = ALIGNMENT_4_IR_DOTS;
	}

	CString ButtonName;

	//WF disply names
	m_Button1Edit.GetWindowText(ButtonName);
	::Settings.SetButtonName(0, 0, ButtonName);

	m_Button2Edit.GetWindowText(ButtonName);
	::Settings.SetButtonName(0, 1, ButtonName);

	m_Button3Edit.GetWindowText(ButtonName);
	::Settings.SetButtonName(0, 2, ButtonName);

	m_Button4Edit.GetWindowText(ButtonName);
	::Settings.SetButtonName(0, 3, ButtonName);

	m_Button4Edit2.GetWindowText(ButtonName);
	::Settings.SetButtonName(0, 4, ButtonName);

	m_Button14Edit.GetWindowText(ButtonName);//
	::NewSettings.SetNewWFBtnName(0, 0, ButtonName);//
													//WF disply names Done

													//CT disply names
	m_Button5Edit.GetWindowText(ButtonName);
	::Settings.SetButtonName(1, 0, ButtonName);

	m_Button6Edit.GetWindowText(ButtonName);
	::Settings.SetButtonName(1, 1, ButtonName);

	m_Button7Edit.GetWindowText(ButtonName);
	::Settings.SetButtonName(1, 2, ButtonName);

	m_Button8Edit.GetWindowText(ButtonName);
	::Settings.SetButtonName(1, 4, ButtonName);

	m_Button8Edit2.GetWindowText(ButtonName);
	::Settings.SetButtonName(1, 3, ButtonName);
	//CT disply names Done

	m_Button9Edit.GetWindowText(ButtonName);//Chang Analysis
	::Settings.SetButtonName(2, 0, ButtonName);

	m_Button10Edit.GetWindowText(ButtonName);//
	::Settings.SetButtonName(2, 1, ButtonName);

	m_Button11Edit.GetWindowText(ButtonName);
	::Settings.SetButtonName(2, 2, ButtonName);

	m_Button12Edit.GetWindowText(ButtonName);
	::Settings.SetButtonName(2, 4, ButtonName);

	m_Button13Edit.GetWindowText(ButtonName);
	::Settings.SetButtonName(2, 3, ButtonName);

	// WFCT Angle ka
	m_Button15Edit.GetWindowText(ButtonName);
	::Settings.SetButtonName(2, 7, ButtonName);
	// WFCT Angle ka

	if (m_WFSum1Radio.GetCheck()) ::Settings.m_DefaultWFSum = 0;
	else if (m_WFSum2Radio.GetCheck()) ::Settings.m_DefaultWFSum = 1;
	//else if (m_WFSum3Radio.GetCheck()) ::Settings.m_DefaultWFSum =  2;// WFCT Angle k/a
	else if (m_WFSum4Radio.GetCheck()) ::Settings.m_DefaultWFSum = 4;//Depth of focus
	else if (m_WFSum5Radio.GetCheck()) ::Settings.m_DefaultWFSum = 3;// Custom
	else if (m_WFVfaRadio.GetCheck())  ::Settings.m_DefaultWFSum = 10;
	else if (m_WFCmpRadio.GetCheck())  ::Settings.m_DefaultWFSum = 12;
	else if (m_WFSum6Radio.GetCheck())  ::Settings.m_DefaultWFSum = 13;
	else if (m_WFNearVisionRadio.GetCheck())  ::Settings.m_DefaultWFSum = 14;//6.2.0
	else                               ::Settings.m_DefaultWFSum = 11;

	if (m_CTSum1Radio.GetCheck()) ::Settings.m_DefaultCTSum = 0;
	else if (m_CTSum2Radio.GetCheck()) ::Settings.m_DefaultCTSum = 1;
	else if (m_CTSum3Radio.GetCheck()) ::Settings.m_DefaultCTSum = 2;
	else if (m_CTSum4Radio.GetCheck()) ::Settings.m_DefaultCTSum = 3;
	else if (m_CTSum5Radio.GetCheck()) ::Settings.m_DefaultCTSum = 4;
	else if (m_CTDblRadio.GetCheck())  ::Settings.m_DefaultCTSum = 10;
	else if (m_CTCmpRadio.GetCheck())  ::Settings.m_DefaultCTSum = 12;
	else                               ::Settings.m_DefaultCTSum = 11;

	if (::Settings.m_DefaultCTSum == 3) ::NewSettings.m_OSHER_ALIG_CUS[3] = 1;

	if (m_INSum1Radio.GetCheck()) ::Settings.m_DefaultINSum = 0; //SetSetting
	else if (m_INSum2Radio.GetCheck()) ::Settings.m_DefaultINSum = 1;
	else if (m_INSum3Radio.GetCheck()) ::Settings.m_DefaultINSum = 2;
	else if (m_INSum4Radio.GetCheck()) ::Settings.m_DefaultINSum = 3;
	else if (m_INSum5Radio.GetCheck()) ::Settings.m_DefaultINSum = 7;
	else if (m_INSum6Radio.GetCheck()) ::Settings.m_DefaultINSum = 10;
	else if (m_INSum7Radio.GetCheck()) ::Settings.m_DefaultINSum = 6;//530
	else if (m_INSum8Radio.GetCheck()) ::Settings.m_DefaultINSum = 8;//530	
	else if (m_INSum9Radio.GetCheck()) ::Settings.m_DefaultINSum = 9;//530	
	else if (m_INSum10Radio.GetCheck()) ::Settings.m_DefaultINSum = 4;// WFCT 'Angle k/a'
	else if (m_INSum11Radio.GetCheck()) ::Settings.m_DefaultINSum = 11;// Toric Alignment

	::Settings.m_ShowWavetouch = m_WavetouchCheck.GetCheck();
	::Settings.m_AccommTargetLightOff = !m_AccommTargetLightOnCheck.GetCheck();

	m_TargetTimeEdit.GetWindowText(s);
	G_As = s;
	::Settings.m_AccommTargetLightTime = atoi(G_As);

	::Settings.m_ShowSimK = m_SimkRadio.GetCheck();

	::NewSettings.m_ShowTask = m_Task_ShortCutCheck.GetCheck();

	::NewSettings.m_Super_Exam = m_Super_ExamCheck.GetCheck();

	CString Def_Name;

	for (int i = 0; i < 5; i++)
	{
		m_TaskEdit[i].GetWindowText(Def_Name);
		Def_Name.Trim();//  
						//strncpy(&::NewSettings.m_Tasks[i].m_ButtonName[0], (const char*)Def_Name, 25);

		::NewSettings.SaveTaskName(i, Def_Name);//6.2.0 Chinese
		m_DlgTask[i].SaveBtnName(Def_Name);//6.2.0 Chinese
	}
	//530

	::Settings.m_ShowSendARKRButton = m_ShowSendARKRCheck.GetCheck();


	::Settings.m_UseJPGFormat = m_UseJPGFormatRadio.GetCheck();
	::Settings.m_IncludeLADs = m_IncludeLADsCheck.GetCheck();

	CString FolderName;
	m_FolderNameEdit.GetWindowText(FolderName);

	CINIParser IniPar;
	IniPar.SetStrKey(IniSection, "Export Folder", FolderName, IniPath);//6.2.0 ini registration

	if (m_Round8Radio.GetCheck())
	{
		::Settings.m_RefractionRounding = 0;
	}
	else
	{
		::Settings.m_RefractionRounding = 1;
	}


	::Settings.m_Export_Excel = m_ExcelExportCheck.GetCheck();
	m_FolderNameEdit2.GetWindowText(FolderName);
	IniPar.SetStrKey(IniSection, "Export Excel Folder", FolderName, IniPath);//6.2.0 ini registration

																			 //solo default screen setting
	::NewSettings.m_DefaultWFSolo = 1;
	//Done

	//IOL Selection data set
	if (m_CtlDef)
	{
		//IOL Selection data set
		::NewSettings.m_IOLSel_Alpha[0] = 0.65;
		::NewSettings.m_IOLSel_Alpha[1] = 0.5;

		::NewSettings.m_IOLSel_CHOA = 0.2;
		::NewSettings.m_IOLSel_IHOA = 0.2;

		::NewSettings.m_IOLSel_Astig = 1.0;

		::NewSettings.m_IOLSel_Sph_Abbe[0] = 0.14;
		::NewSettings.m_IOLSel_Sph_Abbe[1] = 0.20;
		::NewSettings.m_IOLSel_Sph_Abbe[2] = 0.25;
		::NewSettings.m_IOLSel_Sph_Abbe[3] = 0.30;
		//Done
	}
	else
	{
		::NewSettings.m_IOLSel_Alpha[0] = m_IOLSel_Alpha[0];
		::NewSettings.m_IOLSel_Alpha[1] = m_IOLSel_Alpha[1];

		::NewSettings.m_IOLSel_CHOA = m_IOLSel_CHOA;
		::NewSettings.m_IOLSel_IHOA = m_IOLSel_IHOA;

		::NewSettings.m_IOLSel_Astig = m_IOLSel_Astig;

		::NewSettings.m_IOLSel_Sph_Abbe[0] = m_IOLSel_Sph_Abbe[0];
		::NewSettings.m_IOLSel_Sph_Abbe[1] = m_IOLSel_Sph_Abbe[1];
		::NewSettings.m_IOLSel_Sph_Abbe[2] = m_IOLSel_Sph_Abbe[2];
		::NewSettings.m_IOLSel_Sph_Abbe[3] = m_IOLSel_Sph_Abbe[3];
	}
	//Done


	//520 Set display default settings
	if (m_BackSomeDefaults)
	{
		SetSettingsDefault();
	}

	//530 Set task settings
	for (int i = 0; i < 5; i++)
	{
		if (m_TaskChanged[i])  ::NewSettings.m_Tasks[i] = m_DlgTask[i];
		::NewSettings.m_Tasks[i].m_Enable = m_SubTask_Check[i].GetCheck();
	}

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			int type = ::NewSettings.m_Tasks[i].m_SubButton[j];

			switch (type)
			{
			case TYPE_WCVA:
				::Settings.GetButtonName(2, 2, s);
				if (s != "M T F")
				{
					SetDefTask_WCVA(::NewSettings.m_Tasks[i].m_WndSettings[j]);
					::NewSettings.m_Tasks[i].m_SelfSettings[j] = TRUE;
				}
				break;
			case TYPE_CTSU:
				::Settings.GetButtonName(1, 0, s);
				if (s != "CT Summary")
				{
					SetDefTask_CTSU(::NewSettings.m_Tasks[i].m_WndSettings[j]);
					::NewSettings.m_Tasks[i].m_SelfSettings[j] = TRUE;
				}
				break;
			case TYPE_KERA:
				::Settings.GetButtonName(1, 1, s);
				if (s != "Keratometry")
				{
					SetDefTask_CTKE(::NewSettings.m_Tasks[i].m_WndSettings[j]);
					::NewSettings.m_Tasks[i].m_SelfSettings[j] = TRUE;
				}
				break;
			case TYPE_CHAN:
				::Settings.GetButtonName(2, 0, s);
				if (s != "Chang Analysis")
				{
					SetDefTask_CHAN(::NewSettings.m_Tasks[i].m_WndSettings[j]);
					::NewSettings.m_Tasks[i].m_SelfSettings[j] = TRUE;
				}
				break;
			case TYPE_ANGK:
				::Settings.GetButtonName(0, 2, s);
				if (s != "Angle K/A")
				{
					SetDefTask_WFAN(::NewSettings.m_Tasks[i].m_WndSettings[j]);
					::NewSettings.m_Tasks[i].m_SelfSettings[j] = TRUE;
				}
				break;

				//not handle yet
			case TYPE_WFVA:
				::Settings.GetButtonName(0, 0, s);
				if (s != "M T F")//"Visual Acuity"
				{
					SetDefTask_WFVA(::NewSettings.m_Tasks[i].m_WndSettings[j]);
					::NewSettings.m_Tasks[i].m_SelfSettings[j] = TRUE;
				}
				break;
			case TYPE_WFRM:
				::Settings.GetButtonName(0, 1, s);
				if (s != "WF/RMS")
				{
					SetDefTask_WFRM(::NewSettings.m_Tasks[i].m_WndSettings[j]);
					::NewSettings.m_Tasks[i].m_SelfSettings[j] = TRUE;
				}
				break;
			case TYPE_CT3D:
				::Settings.GetButtonName(1, 2, s);
				if (s != "3D Z-Elevation")
				{
					SetDefTask_CT3D(::NewSettings.m_Tasks[i].m_WndSettings[j]);
					::NewSettings.m_Tasks[i].m_SelfSettings[j] = TRUE;
				}
				break;
			case TYPE_WCSU:
				::Settings.GetButtonName(2, 1, s);
				if (s != "Summary")
				{
					SetDefTask_WCSU(::NewSettings.m_Tasks[i].m_WndSettings[j]);
					::NewSettings.m_Tasks[i].m_SelfSettings[j] = TRUE;
				}
				break;

			case TYPE_ASTI:
				::Settings.GetButtonName(2, 4, s);
				if (s != "Astigmatism Source")
				{
					SetDefTask_WCAT(::NewSettings.m_Tasks[i].m_WndSettings[j]);
					::NewSettings.m_Tasks[i].m_SelfSettings[j] = TRUE;
				}
				break;
			case TYPE_DEPF:
				::Settings.GetButtonName(0, 4, s);
				if (s != "Depth of Focus")
				{
					SetDefTask_WCDP(::NewSettings.m_Tasks[i].m_WndSettings[j]);
					::NewSettings.m_Tasks[i].m_SelfSettings[j] = TRUE;
				}
				break;
			}
		}
	}
	//530 set task setting Done

	// Set wf button showing  
	::NewSettings.m_WFCTSumShow[0] = m_INSum_Check[0].GetCheck();
	for (int i = 1; i <= 7; i++)
	{
		::NewSettings.m_WFSumShow[i] = m_WFSum_Check[i].GetCheck();
		::NewSettings.m_CTSumShow[i] = m_CTSum_Check[i].GetCheck();
		::NewSettings.m_WFCTSumShow[i] = m_INSum_Check[i].GetCheck();
	}
	::NewSettings.m_WFNewSumShow[0] = m_WFSum_Check[8].GetCheck();
	::NewSettings.m_WFNewSumShow[1] = m_WFSum_Check[9].GetCheck();//6.2.0

	::NewSettings.m_WFCTSumShow[8] = m_INSum_Check[8].GetCheck();
	//Set wf button showing Done

	::NewSettings.m_WFCTNewSumShow[0] = m_INSum_Check[9].GetCheck(); // transformed WFCT 'Angle K/A'

	::NewSettings.m_WFCTNewSumShow[1] = m_INSum_Check[10].GetCheck(); // Toric Alignment
}

//***************************************************************************************

//set the default values of setting
void CSettingsComboDlg::OnLoadButtonClicked()
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
	}
	else
	{
		if (!::NewSettings.LoadFromFile(FileName2))
		{
			::Info("Loaded Setting file failed.");
		}
		else
		{
			Dir = FileName;
			Dir.Replace(sFileName, _T(""));

			IniPar.SetStrKey(IniSection, "Settings Folder", Dir, IniPath);//6.2.0 ini registration
		}
	}

	m_BackSomeDefaults = FALSE; //If the user use load button to load backuped settings, set the default settings off
	SetSelection();
}

//***************************************************************************************

// set the default values of setting
void CSettingsComboDlg::OnSaveButtonClicked()
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
//520
void CSettingsComboDlg::OnColImgButtonClicked()
{
	if (m_CapColorImgCheck.GetCheck())
	{
		m_CapColorImgAutoRadio.ShowWindow(SW_SHOW);
		m_CapColorImgAutoText.ShowWindow(SW_SHOW);
		m_CapColorImgManuRadio.ShowWindow(SW_SHOW);
		m_CapColorImgManuText.ShowWindow(SW_SHOW);

		m_CapColorImgAutoRadio.SetCheck(::Settings.m_Cap_ColorAutoOrManu);
		m_CapColorImgManuRadio.SetCheck(!::Settings.m_Cap_ColorAutoOrManu);
	}
	else
	{
		m_CapColorImgAutoRadio.ShowWindow(SW_HIDE);
		m_CapColorImgAutoText.ShowWindow(SW_HIDE);
		m_CapColorImgManuRadio.ShowWindow(SW_HIDE);
		m_CapColorImgManuText.ShowWindow(SW_HIDE);
	}

	Invalidate(FALSE);
}

//***************************************************************************************

void CSettingsComboDlg::OnColImgAutoManRadio1Clicked()
{
	m_CapColorImgManuRadio.SetCheck(!m_CapColorImgAutoRadio.GetCheck());

	Invalidate(FALSE);
}

//***************************************************************************************

void CSettingsComboDlg::OnColImgAutoManRadio2Clicked()
{
	m_CapColorImgAutoRadio.SetCheck(!m_CapColorImgManuRadio.GetCheck());
	Invalidate(FALSE);
}

//***************************************************************************************

void CSettingsComboDlg::OnTargetLightBtnClicked()
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

	Invalidate(FALSE);
}

//***************************************************************************************

void CSettingsComboDlg::OnTaskAdBtnClicked()
{
	CTaskSetting This_Tasks;

	CString TaskName;
	int TaskN;

	if (m_TaskRadio[0].GetCheck()) { TaskN = 0;  m_TaskEdit[0].GetWindowText(TaskName); }
	else if (m_TaskRadio[1].GetCheck()) { TaskN = 1;  m_TaskEdit[1].GetWindowText(TaskName); }
	else if (m_TaskRadio[2].GetCheck()) { TaskN = 2;  m_TaskEdit[2].GetWindowText(TaskName); }
	else if (m_TaskRadio[3].GetCheck()) { TaskN = 3;  m_TaskEdit[3].GetWindowText(TaskName); }
	else if (m_TaskRadio[4].GetCheck()) { TaskN = 4;  m_TaskEdit[4].GetWindowText(TaskName); }

	TaskName.Trim();//

	if (m_TaskChanged[TaskN]) This_Tasks = m_DlgTask[TaskN];
	else                     This_Tasks = ::NewSettings.m_Tasks[TaskN];

	This_Tasks.SaveBtnName(TaskName);//6.2.0 Chinese

	CTaskSettinigComboDlg* pDlg = new CTaskSettinigComboDlg(this, This_Tasks, TaskN);

	if (pDlg->DoModal() == IDOK)
	{
		char sa[26];
		memset(sa, 0, sizeof(sa));
		strncpy(sa, (const char*)pDlg->m_SettedTask.m_ButtonName, 25);
		TaskName = sa;

		//6.2.0 Chinese
		int count = TaskName.Find(_T("Chinese"), 0);
		if (count != -1) //chinese
		{
			char s2[201];
			memset(s2, 0, sizeof(s2));
			strncpy(s2, (const char*)pDlg->m_SettedTask.m_ChButtonName, 200);
			TaskName = s2;

			CString Str = TaskName;

			TaskName = "";

			int Len = Str.GetLength();
			int NewLen = Len;
			CStringA StrId = "";

			for (int k = 0; k < Len; k++)
			{
				CString Namestr = Str.Left(1);

				if (Namestr == "*")
				{
					BOOL Chinese = FALSE;

					TCHAR ch = StrId.GetAt(0);
					int Len = StrId.GetLength();

					if (Len > 1)
					{
						Chinese = TRUE;
						StrId.Replace("C", "");
					}

					if (Chinese)
					{
						int id = atoi(StrId);
						TaskName += G_GetChinese(id);
					}
					else
					{
						TaskName += StrId;
					}
					StrId = "";
				}
				else
				{
					StrId += Namestr;
				}

				Str = Str.Right(NewLen - 1);
				NewLen = Str.GetLength();
			}
		}
		//6.2.0 Chinese		

		for (int i = 0; i < 5; i++)
		{
			if (m_TaskRadio[i].GetCheck()) { m_TaskEdit[i].SetWindowText(TaskName); break; }
		}

		m_TaskChanged[TaskN] = TRUE;
		m_DlgTask[TaskN] = pDlg->m_SettedTask;

		for (int i = 0; i < 5; i++)
		{
			if (m_TaskRadio[i].GetCheck() && !m_DlgTask[TaskN].m_Enable) { m_TaskEdit[i].EnableWindow(FALSE); m_TaskRadio[i].EnableWindow(FALSE); }
			if (m_TaskRadio[i].GetCheck() && m_DlgTask[TaskN].m_Enable) { m_TaskEdit[i].EnableWindow(TRUE);  m_TaskRadio[i].EnableWindow(TRUE); }
		}
	}
}

//***************************************************************************************

//for excel setting export
void CSettingsComboDlg::OnExcelCheckClicked()
{
	Invalidate(FALSE);
}


//***************************************************************************************

//for excel setting export
void CSettingsComboDlg::OnExcelSettingBtnClicked()
{
	CExcelSettingsDlg2* pDlg = new CExcelSettingsDlg2(this, m_ExcDef);

	if (pDlg->DoModal() == IDOK)
	{
	}
	delete pDlg;
}

//***************************************************************************************

void CSettingsComboDlg::SetSelection()
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

	{
		m_WFVfaRadio.SetCheck(0);
		m_WFCmpRadio.SetCheck(0);
		m_WFVrfRadio.SetCheck(0);
		m_WFSum1Radio.SetCheck(0);
		m_WFSum2Radio.SetCheck(0);
		m_WFSum3Radio.SetCheck(0);
		m_WFSum4Radio.SetCheck(0);
		m_WFSum5Radio.SetCheck(0);
		m_WFNearVisionRadio.SetCheck(0);//6.2.0

		m_CTDblRadio.SetCheck(0);
		m_CTVrfRadio.SetCheck(0);
		m_CTCmpRadio.SetCheck(0);
		m_CTSum1Radio.SetCheck(0);
		m_CTSum2Radio.SetCheck(0);
		m_CTSum3Radio.SetCheck(0);
		m_CTSum4Radio.SetCheck(0);
		m_CTSum5Radio.SetCheck(0);


		m_INSum1Radio.SetCheck(0);
		m_INSum2Radio.SetCheck(0);
		m_INSum3Radio.SetCheck(0);
		m_INSum4Radio.SetCheck(0);
		m_INSum5Radio.SetCheck(0);//530
		m_INSum6Radio.SetCheck(0);//530
		m_INSum7Radio.SetCheck(0);
		m_INSum8Radio.SetCheck(0);
		m_INSum9Radio.SetCheck(0);
		m_INSum10Radio.SetCheck(0);// WFCT 'Angle k/a'
		m_INSum11Radio.SetCheck(0);// Toric Alignment

		m_UseJPGFormatRadio.SetCheck(0);
		m_UsePNGFormatRadio.SetCheck(0);
		m_IncludeLADsCheck.SetCheck(0);
	}

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

	//520
	{
		m_CapColorImgCheck.SetCheck(::Settings.m_Cap_ColorImg);
		if (::Settings.m_Cap_ColorImg == TRUE)
		{
			m_CapColorImgAutoRadio.ShowWindow(SW_SHOW);
			m_CapColorImgAutoText.ShowWindow(SW_SHOW);
			m_CapColorImgManuRadio.ShowWindow(SW_SHOW);
			m_CapColorImgManuText.ShowWindow(SW_SHOW);

			m_CapColorImgAutoRadio.SetCheck(::Settings.m_Cap_ColorAutoOrManu);
			m_CapColorImgManuRadio.SetCheck(!::Settings.m_Cap_ColorAutoOrManu);
		}
		else
		{
			m_CapColorImgAutoRadio.ShowWindow(SW_HIDE);
			m_CapColorImgAutoText.ShowWindow(SW_HIDE);
			m_CapColorImgManuRadio.ShowWindow(SW_HIDE);
			m_CapColorImgManuText.ShowWindow(SW_HIDE);
		}
	}
	//520

	m_ProbingEnabledCheck.SetCheck(::Settings.m_ProbingEnabled);

	m_FoggingEnabledCheck.SetCheck(::Settings.m_ProbingEnabled && ::Settings.m_FoggingEnabled);
	if (!m_isComboViewer) m_FoggingEnabledCheck.EnableWindow(::Settings.m_ProbingEnabled);

	s.Format(_T("%.1f"), ::Settings.m_ManualScanDiameterUm * 0.001);
	m_ManualScanDiameterEdit.SetWindowText(s);

	
	s.Format(_T("%.1f"), ::Settings.m_AutoScanDiameterUm * 0.001);
	m_AutoScanDiameterEdit.SetWindowText(s);
	//Done

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

	{
		switch (::Settings.m_DefaultWFSum) {
		case 10: m_WFVfaRadio.SetCheck(1); break;
		case 11: m_WFVrfRadio.SetCheck(1); break;
		case 12: m_WFCmpRadio.SetCheck(1); break;
		case 13: m_WFSum6Radio.SetCheck(1); break;//
		case  0: m_WFSum1Radio.SetCheck(1); break;
		case  1: m_WFSum2Radio.SetCheck(1); break;
			//case  2: m_WFSum3Radio.SetCheck(1); break;// WFCT Angle k/a
		case  2: m_WFVfaRadio.SetCheck(1); break;// WFCT Angle k/a
		case  4: m_WFSum4Radio.SetCheck(1); break;
		case  3: m_WFSum5Radio.SetCheck(1); break;
		case 14: m_WFNearVisionRadio.SetCheck(1); break; //6.2.0
		}
		switch (::Settings.m_DefaultCTSum) {
		case 10: m_CTDblRadio.SetCheck(1); break;
		case 11: m_CTVrfRadio.SetCheck(1); break;
		case 12: m_CTCmpRadio.SetCheck(1); break;
		case  0: m_CTSum1Radio.SetCheck(1); break;
		case  1: m_CTSum2Radio.SetCheck(1); break;
		case  2: m_CTSum3Radio.SetCheck(1); break;
		case  3: m_CTSum4Radio.SetCheck(1); break;
		case  4: m_CTSum5Radio.SetCheck(1); break;
		}
		switch (::Settings.m_DefaultINSum) { //bbb SetSelection
		case  0: m_INSum1Radio.SetCheck(1); break;
		case  1: m_INSum2Radio.SetCheck(1); break;
		case  2: m_INSum3Radio.SetCheck(1); break;
		case  3: m_INSum4Radio.SetCheck(1); break;
		case  6: m_INSum7Radio.SetCheck(1); break;//530
		case  10: m_INSum6Radio.SetCheck(1); break;//530	
		case  7: m_INSum5Radio.SetCheck(1); break;
		case  8: m_INSum8Radio.SetCheck(1); break;
		case  9: m_INSum9Radio.SetCheck(1); break;
		case  4: m_INSum10Radio.SetCheck(1); break;// WFCT 'Angle k/a'
		case  11: m_INSum11Radio.SetCheck(1); break;// Toric Alignment
		}
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

	//530 2
	if (::Settings.m_ShowSimK == TRUE)
	{
		m_SimkRadio.SetCheck(TRUE);
		m_RefkRadio.SetCheck(FALSE);
	}
	else
	{
		m_SimkRadio.SetCheck(FALSE);
		m_RefkRadio.SetCheck(TRUE);
	}
	//530

	m_ShowSendARKRCheck.SetCheck(::Settings.m_ShowSendARKRButton);

	//Export  
	m_UseJPGFormatRadio.SetCheck(::Settings.m_UseJPGFormat);
	m_UsePNGFormatRadio.SetCheck(!::Settings.m_UseJPGFormat);
	m_IncludeLADsCheck.SetCheck(::Settings.m_IncludeLADs);

	CINIParser IniPar;
	CString FolderName = IniPar.GetStrKey(IniSection, "Export Folder", IniPath);	//6.2.0 ini registration

	m_FolderNameEdit.SetWindowText(FolderName);
	//Export  Done


	//520 Excel Export      	  
	FolderName = IniPar.GetStrKey(IniSection, "Export Excel Folder", IniPath);	//6.2.0 ini registration	
	m_FolderNameEdit2.SetWindowText(FolderName);
	//Done

	if (::Settings.m_RefractionRounding == 0) {
		m_Round8Radio.SetCheck(1);
	}
	else {
		m_Round100Radio.SetCheck(1);
	}

	{
		CString ButtonName;
		::Settings.GetButtonName(0, 0, ButtonName);
		m_Button1Edit.SetWindowText(ButtonName);

		::Settings.GetButtonName(0, 1, ButtonName);
		m_Button2Edit.SetWindowText(ButtonName);

		::Settings.GetButtonName(0, 2, ButtonName);
		m_Button3Edit.SetWindowText(ButtonName);

		::Settings.GetButtonName(0, 3, ButtonName);
		m_Button4Edit.SetWindowText(ButtonName);

		::Settings.GetButtonName(0, 4, ButtonName);
		m_Button4Edit2.SetWindowText(ButtonName);

		//
		::NewSettings.GetNewWFBtnName(0, 0, ButtonName);
		m_Button14Edit.SetWindowText(ButtonName);
		//

		::Settings.GetButtonName(1, 0, ButtonName);
		m_Button5Edit.SetWindowText(ButtonName);

		::Settings.GetButtonName(1, 1, ButtonName);
		m_Button6Edit.SetWindowText(ButtonName);

		::Settings.GetButtonName(1, 2, ButtonName);
		m_Button7Edit.SetWindowText(ButtonName);

		::Settings.GetButtonName(1, 4, ButtonName);
		m_Button8Edit.SetWindowText(ButtonName);

		::Settings.GetButtonName(1, 3, ButtonName);
		m_Button8Edit2.SetWindowText(ButtonName);

		::Settings.GetButtonName(2, 0, ButtonName);
		m_Button9Edit.SetWindowText(ButtonName);
		::Settings.GetButtonName(2, 1, ButtonName);
		m_Button10Edit.SetWindowText(ButtonName);
		::Settings.GetButtonName(2, 2, ButtonName);
		m_Button11Edit.SetWindowText(ButtonName);
		::Settings.GetButtonName(2, 4, ButtonName);
		m_Button12Edit.SetWindowText(ButtonName);

		::Settings.GetButtonName(2, 3, ButtonName);
		m_Button13Edit.SetWindowText(ButtonName);

		// WFCT Angle ka
		::Settings.GetButtonName(2, 7, ButtonName);
		m_Button15Edit.SetWindowText(ButtonName);
		// WFCT Angle ka
	}

	//530 to fix bugs (some saved settings do not be shown)
	m_ExcelExportCheck.SetCheck(::Settings.m_Export_Excel);

	//IOL Selection data set
	m_IOLSel_Alpha[0] = ::NewSettings.m_IOLSel_Alpha[0];
	m_IOLSel_Alpha[1] = ::NewSettings.m_IOLSel_Alpha[1];

	m_IOLSel_CHOA = ::NewSettings.m_IOLSel_CHOA;
	m_IOLSel_IHOA = ::NewSettings.m_IOLSel_IHOA;

	m_IOLSel_Astig = ::NewSettings.m_IOLSel_Astig;

	m_IOLSel_Sph_Abbe[0] = ::NewSettings.m_IOLSel_Sph_Abbe[0];
	m_IOLSel_Sph_Abbe[1] = ::NewSettings.m_IOLSel_Sph_Abbe[1];
	m_IOLSel_Sph_Abbe[2] = ::NewSettings.m_IOLSel_Sph_Abbe[2];
	m_IOLSel_Sph_Abbe[3] = ::NewSettings.m_IOLSel_Sph_Abbe[3];

	//Task
	m_Task_ShortCutCheck.SetCheck(::NewSettings.m_ShowTask);

	if (!::NewSettings.m_ShowTask)
	{
		HideTaskSettings();
	}

	//Edit
	for (int i = 0; i < 5; i++) { m_TaskEdit[i].EnableWindow(TRUE); m_TaskRadio[i].EnableWindow(TRUE); }

	CString taskName = ::NewSettings.ReadTaskName(0);//6.2.0 Chinese cjf

	m_TaskEdit[0].SetWindowText(taskName);
	if (!::NewSettings.m_Tasks[0].m_Enable)
	{
		m_TaskEdit[0].EnableWindow(FALSE);
		m_TaskRadio[0].EnableWindow(FALSE);
		m_SubTask_Check[0].SetCheck(FALSE);
	}
	else m_SubTask_Check[0].SetCheck(TRUE);

	taskName = ::NewSettings.ReadTaskName(1);//6.2.0 Chinese cjf
	m_TaskEdit[1].SetWindowText(taskName);
	if (!::NewSettings.m_Tasks[1].m_Enable)
	{
		m_TaskEdit[1].EnableWindow(FALSE);
		m_TaskRadio[1].EnableWindow(FALSE);
		m_SubTask_Check[1].SetCheck(FALSE);
	}
	else m_SubTask_Check[1].SetCheck(TRUE);

	taskName = ::NewSettings.ReadTaskName(2);//6.2.0 Chinese cjf
	m_TaskEdit[2].SetWindowText(taskName);
	if (!::NewSettings.m_Tasks[2].m_Enable)
	{
		m_TaskEdit[2].EnableWindow(FALSE);
		m_TaskRadio[2].EnableWindow(FALSE);
		m_SubTask_Check[2].SetCheck(FALSE);
	}
	else m_SubTask_Check[2].SetCheck(TRUE);

	taskName = ::NewSettings.ReadTaskName(3);//6.2.0 Chinese cjf
	m_TaskEdit[3].SetWindowText(taskName);
	if (!::NewSettings.m_Tasks[3].m_Enable)
	{
		m_TaskEdit[3].EnableWindow(FALSE);
		m_TaskRadio[3].EnableWindow(FALSE);
		m_SubTask_Check[3].SetCheck(FALSE);
	}
	else m_SubTask_Check[3].SetCheck(TRUE);

	taskName = ::NewSettings.ReadTaskName(4);//6.2.0 Chinese cjf
	if (taskName == "Accommdation Analysis")
		taskName = "Accomm and DOF Analysis";
	m_TaskEdit[4].SetWindowText(taskName);
	if (!::NewSettings.m_Tasks[4].m_Enable)
	{
		m_TaskEdit[4].EnableWindow(FALSE);
		m_TaskRadio[4].EnableWindow(FALSE);
		m_SubTask_Check[4].SetCheck(FALSE);
	}
	else m_SubTask_Check[4].SetCheck(TRUE);


	BOOL HasTask = FALSE;
	for (int i = 0; i < 5; i++)
	{
		m_DlgTask[i].m_Enable = ::NewSettings.m_Tasks[i].m_Enable;
		m_TaskRadio[i].SetCheck(FALSE);
	}

	for (int i = 0; i < 5; i++)
	{
		if (::NewSettings.m_Tasks[i].m_Enable)
		{
			HasTask = TRUE;
			m_TaskRadio[i].SetCheck(TRUE);
			m_SelTaskRadio = i;
			break;
		}
	}
	if (!HasTask)
	{
		m_TaskAdBtn.EnableWindow(FALSE);
		for (int i = 0; i < 5; i++) m_TaskRadio[i].SetCheck(FALSE);
	}
	else m_TaskAdBtn.EnableWindow(TRUE);
	//



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


	// Show WF Buttons
	BOOL ShowWFBtn = FALSE;
	for (int i = 0; i <= 8; i++)
	{
		// 'Depth of focus comparison'
		if (i == 0)
		{
			ShowWFBtn = ::NewSettings.m_WFNewSumShow[i];
			m_WFSum_Check[8].SetCheck(ShowWFBtn);

			if (ShowWFBtn)
			{
				m_WFSum6Radio.EnableWindow(TRUE);
				m_Button14Edit.EnableWindow(TRUE);
			}
			else
			{
				m_WFSum6Radio.EnableWindow(FALSE);
				m_Button14Edit.EnableWindow(FALSE);
			}
		}
		// 'Depth of focus comparison'

		//6.2.0 'WF near vision'
		if (i == 7)
		{
			ShowWFBtn = ::NewSettings.m_WFNewSumShow[1];
			m_WFSum_Check[9].SetCheck(ShowWFBtn);

			if (ShowWFBtn)
			{
				m_WFNearVisionRadio.EnableWindow(TRUE);
			}
			else
			{
				m_WFNearVisionRadio.EnableWindow(FALSE);

			}

			//
			ShowWFBtn = ::NewSettings.m_WFSumShow[7];
			m_WFSum_Check[7].SetCheck(ShowWFBtn);

			if (ShowWFBtn)
			{
				m_WFSum5Radio.EnableWindow(TRUE);
				m_Button4Edit.EnableWindow(TRUE);
			}
			else
			{
				m_WFSum5Radio.EnableWindow(FALSE);
				m_Button4Edit.EnableWindow(FALSE);
			}	
		}
		//6.2.0 'WF near vision'

		if (i != 0 && i != 7)
		{
			ShowWFBtn = ::NewSettings.m_WFSumShow[i];
			m_WFSum_Check[i].SetCheck(ShowWFBtn);

			if (ShowWFBtn)
			{
				if (i == 1)
				{
					m_WFVfaRadio.EnableWindow(TRUE);
				}
				else if (i == 2)
				{
					m_WFCmpRadio.EnableWindow(TRUE);
				}
				else if (i == 3)
				{
					m_WFSum1Radio.EnableWindow(TRUE);
					m_Button1Edit.EnableWindow(TRUE);
				}
				else if (i == 4)
				{
					m_WFSum2Radio.EnableWindow(TRUE);
					m_Button2Edit.EnableWindow(TRUE);
				}
				else if (i == 5)
				{
					m_WFSum3Radio.EnableWindow(FALSE);// WFCT Angle k/a
					m_Button3Edit.EnableWindow(FALSE);// WFCT Angle k/a
				}
				else if (i == 6)
				{
					m_WFSum4Radio.EnableWindow(TRUE);
					m_Button4Edit2.EnableWindow(TRUE);
				}
			}
			else
			{
				if (i == 1)
				{
					m_WFVfaRadio.EnableWindow(FALSE);
				}
				else if (i == 2)
				{
					m_WFCmpRadio.EnableWindow(FALSE);
				}
				else if (i == 3)
				{
					m_WFSum1Radio.EnableWindow(FALSE);
					m_Button1Edit.EnableWindow(FALSE);
				}
				else if (i == 4)
				{
					m_WFSum2Radio.EnableWindow(FALSE);
					m_Button2Edit.EnableWindow(FALSE);
				}
				else if (i == 5)
				{
					m_WFSum3Radio.EnableWindow(FALSE);
					m_Button3Edit.EnableWindow(FALSE);
				}
				else if (i == 6)
				{
					m_WFSum4Radio.EnableWindow(FALSE);
					m_Button4Edit2.EnableWindow(FALSE);
				}
			}

			//
			BOOL ShowCTBtn = ::NewSettings.m_CTSumShow[i];
			m_CTSum_Check[i].SetCheck(ShowCTBtn);

			if (ShowCTBtn)
			{
				if (i == 1)
				{
					m_CTDblRadio.EnableWindow(TRUE);
				}
				else if (i == 2)
				{
					m_CTCmpRadio.EnableWindow(TRUE);
				}
				else if (i == 3)
				{
					m_CTSum1Radio.EnableWindow(TRUE);
					m_Button5Edit.EnableWindow(TRUE);
				}
				else if (i == 4)
				{
					m_CTSum2Radio.EnableWindow(TRUE);
					m_Button6Edit.EnableWindow(TRUE);
				}
				else if (i == 5)
				{
					m_CTSum3Radio.EnableWindow(TRUE);
					m_Button7Edit.EnableWindow(TRUE);
				}
				else if (i == 6)
				{
					m_CTSum4Radio.EnableWindow(TRUE);
					m_Button8Edit.EnableWindow(TRUE);
				}		
			}
			else
			{
				if (i == 1)
				{
					m_CTDblRadio.EnableWindow(FALSE);
				}
				else if (i == 2)
				{
					m_CTCmpRadio.EnableWindow(FALSE);
				}
				else if (i == 3)
				{
					m_CTSum1Radio.EnableWindow(FALSE);
					m_Button5Edit.EnableWindow(FALSE);
				}
				else if (i == 4)
				{
					m_CTSum2Radio.EnableWindow(FALSE);
					m_Button6Edit.EnableWindow(FALSE);
				}
				else if (i == 5)
				{
					m_CTSum3Radio.EnableWindow(FALSE);
					m_Button7Edit.EnableWindow(FALSE);
				}
				else if (i == 6)
				{
					m_CTSum4Radio.EnableWindow(FALSE);
					m_Button8Edit.EnableWindow(FALSE);
				}		
			}
		}
		else if (i == 7)
		{
			BOOL ShowCTBtn = ::NewSettings.m_CTSumShow[i];
			m_CTSum_Check[i].SetCheck(ShowCTBtn);

			if (ShowCTBtn)
			{
				m_CTSum5Radio.EnableWindow(TRUE);
				m_Button8Edit2.EnableWindow(TRUE);
			}
			else
			{
				m_CTSum5Radio.EnableWindow(FALSE);
				m_Button8Edit2.EnableWindow(FALSE);
			}
		}

		//WF/CT
		BOOL ShowWFCTBtn = ::NewSettings.m_WFCTSumShow[i];
		m_INSum_Check[i].SetCheck(ShowWFCTBtn);

		if (ShowWFCTBtn)
		{
			if (i == 0)
			{
				m_INSum1Radio.EnableWindow(TRUE);
				m_Button9Edit.EnableWindow(TRUE);
			}
			else if (i == 1)
			{
				m_INSum2Radio.EnableWindow(TRUE);
				m_Button10Edit.EnableWindow(TRUE);
			}
			else if (i == 2)
			{
				m_INSum3Radio.EnableWindow(TRUE);
				m_Button11Edit.EnableWindow(TRUE);
			}
			else if (i == 3)
			{
				m_INSum4Radio.EnableWindow(TRUE);
			}
			else if (i == 4)
			{
				m_INSum5Radio.EnableWindow(TRUE);
			}
			else if (i == 5)
			{
				m_INSum6Radio.EnableWindow(TRUE);
			}
			else if (i == 6)
			{
				m_INSum7Radio.EnableWindow(TRUE);
			}
			else if (i == 7)
			{
				m_INSum8Radio.EnableWindow(TRUE);
				m_Button12Edit.EnableWindow(TRUE);
			}
		}
		else
		{
			if (i == 0)
			{
				m_INSum1Radio.EnableWindow(FALSE);
				m_Button9Edit.EnableWindow(FALSE);
			}
			else if (i == 1)
			{
				m_INSum2Radio.EnableWindow(FALSE);
				m_Button10Edit.EnableWindow(FALSE);
			}
			else if (i == 2)
			{
				m_INSum3Radio.EnableWindow(FALSE);
				m_Button11Edit.EnableWindow(FALSE);
			}
			else if (i == 3)
			{
				m_INSum4Radio.EnableWindow(FALSE);
			}
			else if (i == 4)
			{
				m_INSum5Radio.EnableWindow(FALSE);
			}
			else if (i == 5)
			{
				m_INSum6Radio.EnableWindow(FALSE);
			}
			else if (i == 6)
			{
				m_INSum7Radio.EnableWindow(FALSE);
			}
			else if (i == 7)
			{
				m_INSum8Radio.EnableWindow(FALSE);
				m_Button12Edit.EnableWindow(FALSE);
			}
		}
		//WF/CT Done
	}

	if (::NewSettings.m_WFCTSumShow[8])
	{
		m_INSum9Radio.EnableWindow(TRUE);
		m_Button13Edit.EnableWindow(TRUE);
		m_INSum_Check[8].SetCheck(TRUE);
	}
	else
	{
		m_INSum9Radio.EnableWindow(FALSE);
		m_Button13Edit.EnableWindow(FALSE);
		m_INSum_Check[8].SetCheck(FALSE);
	}

	// Add WFCT Angle K/A (Transform from WF Screen)
	if (::NewSettings.m_WFCTNewSumShow[0])
	{
		m_INSum10Radio.EnableWindow(TRUE);
		m_Button15Edit.EnableWindow(TRUE);
		m_INSum_Check[9].SetCheck(TRUE);
	}
	else
	{
		m_INSum10Radio.EnableWindow(FALSE);
		m_Button15Edit.EnableWindow(FALSE);
		m_INSum_Check[9].SetCheck(FALSE);
	}
	//

	// Toric Alignment
	if (::NewSettings.m_WFCTNewSumShow[1])
	{
		m_INSum11Radio.EnableWindow(TRUE);
		m_INSum_Check[10].SetCheck(TRUE);
	}
	else
	{
		m_INSum11Radio.EnableWindow(FALSE);
		m_INSum_Check[10].SetCheck(FALSE);
	}
	//  
}

//***************************************************************************************

void CSettingsComboDlg::OnFolderBrowseButtonClicked()
{
	m_FolderNameEdit.GetWindowText(CFolderDlg::m_InitialFolderName);

	CFolderDlg* pDlg = new CFolderDlg(this, "Select an existing or create a new folder for data export");

	if (pDlg->Show())
	{
		m_FolderNameEdit.SetWindowText(pDlg->m_FolderName);
	}

	delete pDlg;
}

//***************************************************************************************
//520
void CSettingsComboDlg::OnFolderBrowse2ButtonClicked()
{
	m_FolderNameEdit2.GetWindowText(CFolderDlg::m_InitialFolderName);

	CFolderDlg* pDlg = new CFolderDlg(this, "Select an existing or create a new folder for data export");
	if (pDlg->Show()) {
		m_FolderNameEdit2.SetWindowText(pDlg->m_FolderName);
	}
	delete pDlg;
}
//520
//***************************************************************************************

// set the default exam list width
void  CSettingsComboDlg::SetDef_ExList_Width()
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
		case 4:
			::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = 0.05;
			::NewSettings.CTOD_COL_Width[i] = ::NewSettings.CTOS_COL_Width[i] = 0.002;
			break;
		case 3:
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
			::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = 0.25;
			::NewSettings.CTOD_COL_Width[i] = ::NewSettings.CTOS_COL_Width[i] = 0.3;
			break;
		}
	}
}

//********************************************************************************************

void CSettingsComboDlg::OnIOLSelectionBtnClicked()
{
	CSettingsIOLSelDlg* pDlg = new CSettingsIOLSelDlg(this, m_CtlDef, m_IOLSel_Alpha, m_IOLSel_CHOA, m_IOLSel_IHOA, m_IOLSel_Astig, m_IOLSel_Sph_Abbe);

	if (pDlg->DoModal() == IDOK)
	{
		m_IOLSel_Alpha[0] = pDlg->m_IOLSel_Alpha[0];
		m_IOLSel_Alpha[1] = pDlg->m_IOLSel_Alpha[1];

		m_IOLSel_CHOA = pDlg->m_IOLSel_CHOA;
		m_IOLSel_IHOA = pDlg->m_IOLSel_IHOA;

		m_IOLSel_Astig = pDlg->m_IOLSel_Astig;

		m_IOLSel_Sph_Abbe[0] = pDlg->m_IOLSel_Sph_Abbe[0];
		m_IOLSel_Sph_Abbe[1] = pDlg->m_IOLSel_Sph_Abbe[1];
		m_IOLSel_Sph_Abbe[2] = pDlg->m_IOLSel_Sph_Abbe[2];
		m_IOLSel_Sph_Abbe[3] = pDlg->m_IOLSel_Sph_Abbe[3];

		m_CtlDef = FALSE;
	}

	delete pDlg;
}

//********************************************************************************************
//530
void CSettingsComboDlg::SetDefTask_WCVA(CWndSettings InWndSetting[4])
{
	InWndSetting[0] = ::Settings.m_IDsmSettings[2][0];
	InWndSetting[1] = ::Settings.m_IDsmSettings[2][1];
	InWndSetting[2] = ::Settings.m_IDsmSettings[2][2];
	InWndSetting[3] = ::Settings.m_IDsmSettings[2][3];

	InWndSetting[0].m_Type = TYPE_ILTR;
	InWndSetting[0].m_Method3D = FALSE;
	InWndSetting[0].m_MapShowSolidSurface = TRUE;
	InWndSetting[0].m_MapShowWireMesh = TRUE;
	InWndSetting[0].m_SizeSmall = TRUE;
	InWndSetting[0].m_ZoneRadiusUm = 5000;
	InWndSetting[0].m_Mask.SetType(MASK_TOTAL_NO_DEFOCUS);
	InWndSetting[0].m_Piston = TRUE;
	InWndSetting[0].m_Tilt = TRUE;

	for (int i = 0; i < 3; i++)
	{
		InWndSetting[i].m_Ltr = 0;
		InWndSetting[i].m_SimplifiedRMS = TRUE;
		InWndSetting[i].m_SimplifiedWFPanel = TRUE;
		InWndSetting[i].m_SimplifiedCTPanel = TRUE;

		InWndSetting[i].m_SizeSmall = TRUE;
	}

	InWndSetting[0].m_LtrLine = 70;
	InWndSetting[0].m_LtrOrientation = 0;

	InWndSetting[3].m_Type = 31; //Axial map       TYPE_CAXM
	InWndSetting[3].m_Method3D = 0;
	InWndSetting[3].m_MapShowSolidSurface = TRUE;
	InWndSetting[3].m_MapShowWireMesh = TRUE;
	InWndSetting[3].m_SizeSmall = TRUE;
	InWndSetting[3].m_MapUnit = DIOPTERS;
	InWndSetting[3].m_MapShowMap = 1;
	InWndSetting[3].m_MapShowEye = 1;
	InWndSetting[3].m_MapShowNumbers = 0;
	InWndSetting[3].m_MapShowPupil = 0;
	InWndSetting[3].m_MapShowLimbus = 0;
	InWndSetting[3].m_MapShowCornealVertex = 0;
	InWndSetting[3].m_MapTranslucent = 0;
	InWndSetting[3].m_MapShowKeratometry = 1;

	InWndSetting[3].SetDefaultScale();

	for (int i = 0; i < 3; i++)
	{
		InWndSetting[i].m_MapShowEye = 0;
		InWndSetting[i].SetDefaultColors(1);
	}
	//Done
}
//530

//********************************************************************************************
//530
void CSettingsComboDlg::SetDefTask_CTSU(CWndSettings InWndSetting[4])
{
	InWndSetting[0] = ::Settings.m_CSsmSettings[0][0];
	InWndSetting[1] = ::Settings.m_CSsmSettings[0][1];
	InWndSetting[2] = ::Settings.m_CSsmSettings[0][2];
	InWndSetting[3] = ::Settings.m_CSsmSettings[0][3];

	InWndSetting[0].m_Type = 31; //Axial map       TYPE_CAXM
	InWndSetting[1].m_Type = 34; //z elevation     TYPE_CELM 
	InWndSetting[2].m_Type = 32; //local Roc  map  TYPE_CTNM
	InWndSetting[3].m_Type = 33; //refrective map  TYPE_CRFM

	InWndSetting[0].m_MapUnit = DIOPTERS;
	InWndSetting[2].m_MapUnit = MILLIMETERS;

	InWndSetting[1].m_RefSurfaceType = REF_SURFACE_BEST_SPHERE;

	for (int i = 0; i < 4; i++)
	{
		InWndSetting[i].m_Method3D = 0;
		InWndSetting[i].m_MapShowSolidSurface = TRUE;
		InWndSetting[i].m_MapShowWireMesh = TRUE;
		InWndSetting[i].m_SizeSmall = TRUE;

		InWndSetting[i].m_MapShowMap = 1;
		InWndSetting[i].m_MapShowEye = 1; 
		InWndSetting[i].m_MapShowNumbers = 0;
		InWndSetting[i].m_MapShowPupil = 0;
		InWndSetting[i].m_MapShowLimbus = 0;
		InWndSetting[i].m_MapShowCornealVertex = 0;
		InWndSetting[i].m_MapTranslucent = 0;
		InWndSetting[i].m_MapShowKeratometry = 0;// 530 Keratometry

		InWndSetting[i].SetDefaultScale();
		InWndSetting[i].SetDefaultColors(1);
	}
}
//530
//********************************************************************************************
//530
void CSettingsComboDlg::SetDefTask_CTKE(CWndSettings InWndSetting[4])
{
	InWndSetting[0] = ::Settings.m_CSsmSettings[1][0];
	InWndSetting[1] = ::Settings.m_CSsmSettings[1][1];
	InWndSetting[2] = ::Settings.m_CSsmSettings[1][2];
	InWndSetting[3] = ::Settings.m_CSsmSettings[1][3];

	InWndSetting[0].m_Type = 51; //Axial map       TYPE_CAXM
	InWndSetting[1].m_Type = 34; //z elevation     TYPE_CELM 
	InWndSetting[2].m_Type = 32; //local Roc  map  TYPE_CTNM
	InWndSetting[3].m_Type = 33; //refrective map  TYPE_CRFM

	InWndSetting[0].m_MapUnit = DIOPTERS;
	InWndSetting[2].m_MapUnit = MILLIMETERS;

	InWndSetting[1].m_RefSurfaceType = REF_SURFACE_BEST_SPHERE;

	for (int i = 0; i < 4; i++)
	{
		InWndSetting[i].m_Method3D = 0;
		InWndSetting[i].m_MapShowSolidSurface = TRUE;
		InWndSetting[i].m_MapShowWireMesh = TRUE;
		InWndSetting[i].m_SizeSmall = TRUE;

		InWndSetting[i].m_MapShowMap = 1;
		InWndSetting[i].m_MapShowEye = 1; 
		InWndSetting[i].m_MapShowNumbers = 0;
		InWndSetting[i].m_MapShowPupil = 0;
		InWndSetting[i].m_MapShowLimbus = 0;
		InWndSetting[i].m_MapShowCornealVertex = 0;
		InWndSetting[i].m_MapTranslucent = 0;

		InWndSetting[i].SetDefaultScale();
		InWndSetting[i].SetDefaultColors(1);
	}

	InWndSetting[0].m_SizeSmall = FALSE;
}
//530
//********************************************************************************************
//530
void CSettingsComboDlg::SetDefTask_CHAN(CWndSettings InWndSetting[4])
{
	InWndSetting[0] = ::Settings.m_IDsmSettings[0][0];
	InWndSetting[1] = ::Settings.m_IDsmSettings[0][1];
	InWndSetting[2] = ::Settings.m_IDsmSettings[0][2];
	InWndSetting[3] = ::Settings.m_IDsmSettings[0][3];

	InWndSetting[0].m_Type = 41; //RMS map        TYPE_IRMS
	InWndSetting[0].m_Method3D = 0;
	InWndSetting[0].m_MapShowSolidSurface = TRUE;
	InWndSetting[0].m_MapShowWireMesh = TRUE;
	InWndSetting[0].m_SizeSmall = TRUE;
	InWndSetting[0].m_ZoneRadiusUm = 5000;
	InWndSetting[0].m_Mask.SetType(MASK_TOTAL_NO_DEFOCUS);
	InWndSetting[0].m_Piston = TRUE;
	InWndSetting[0].m_Tilt = TRUE;
	InWndSetting[0].m_RMSIndividual = TRUE;

	InWndSetting[3].m_Type = 31; //Axial map       TYPE_CAXM
	InWndSetting[3].m_Method3D = 0;
	InWndSetting[3].m_MapShowSolidSurface = TRUE;
	InWndSetting[3].m_MapShowWireMesh = TRUE;
	InWndSetting[3].m_SizeSmall = TRUE;
	InWndSetting[3].m_MapUnit = DIOPTERS;
	InWndSetting[3].m_MapShowMap = 1;
	InWndSetting[3].m_MapShowEye = 1;
	InWndSetting[3].m_MapShowNumbers = 0;
	InWndSetting[3].m_MapShowPupil = 0;
	InWndSetting[3].m_MapShowLimbus = 0;
	InWndSetting[3].m_MapShowCornealVertex = 0;
	InWndSetting[3].m_MapTranslucent = 0;
	InWndSetting[3].m_MapShowKeratometry = 1;

	for (int i = 0; i < 4; i++)
	{
		InWndSetting[i].m_SizeSmall = TRUE;
		InWndSetting[i].m_SimplifiedRMS = TRUE;
		InWndSetting[i].m_SimplifiedWFPanel = TRUE;
		InWndSetting[i].m_SimplifiedCTPanel = TRUE;
	}

	InWndSetting[0].SetDefaultScale();
	InWndSetting[0].SetDefaultColors(1);

	InWndSetting[3].SetDefaultScale();
	InWndSetting[3].SetDefaultColors(1);
}
//530
//********************************************************************************************
//530
void CSettingsComboDlg::SetDefTask_WFAN(CWndSettings InWndSetting[4])
{
	InWndSetting[0] = ::Settings.m_TSsmSettings[2][0];
	InWndSetting[1] = ::Settings.m_TSsmSettings[2][1];
	InWndSetting[2] = ::Settings.m_TSsmSettings[2][2];
	InWndSetting[3] = ::Settings.m_TSsmSettings[2][3];

	for (int i = 0; i < 4; i++)
	{
		InWndSetting[i].m_Type = 1;
		InWndSetting[i].m_MapShowPupil = 1;
		InWndSetting[i].m_MapShowLimbus = 1;
		InWndSetting[i].m_MapShowCornealVertex = 1;
		InWndSetting[i].m_SizeSmall = 1;
	}

	InWndSetting[0].m_SizeSmall = FALSE;
}
//530
//********************************************************************************************
//530
void CSettingsComboDlg::SetDefTask_WFVA(CWndSettings InWndSetting[4])
{
	InWndSetting[0] = ::Settings.m_TSsmSettings[0][0];
	InWndSetting[1] = ::Settings.m_TSsmSettings[0][1];
	InWndSetting[2] = ::Settings.m_TSsmSettings[0][2];
	InWndSetting[3] = ::Settings.m_TSsmSettings[0][3];

	InWndSetting[0].m_Mask.SetType(MASK_TOTAL);
	InWndSetting[0].m_Piston = TRUE;
	InWndSetting[0].m_Tilt = TRUE;
	InWndSetting[0].m_LtrDistanceType = 0;//530

	InWndSetting[2].m_Mask.SetType(MASK_TOTAL);
	InWndSetting[2].m_Piston = TRUE;
	InWndSetting[2].m_Tilt = TRUE;
	InWndSetting[2].m_LtrDistanceType = 0;//530

	InWndSetting[1].m_Mask.SetType(MASK_HO_TOTAL);
	InWndSetting[1].m_Piston = FALSE;
	InWndSetting[1].m_Tilt = FALSE;
	InWndSetting[1].m_LtrDistanceType = 0;//530

	InWndSetting[3].m_Mask.SetType(MASK_HO_TOTAL);
	InWndSetting[3].m_Piston = FALSE;
	InWndSetting[3].m_Tilt = FALSE;
	InWndSetting[3].m_LtrDistanceType = 0;//530


	for (int i = 0; i< 2; i++)
	{
		InWndSetting[i].m_Type = 11;

		InWndSetting[i].m_MapShowMap = 1;
		InWndSetting[i].m_MapShowEye = 0;
		InWndSetting[i].m_MapShowNumbers = 0;
		InWndSetting[i].m_MapShowPupil = 0;
		InWndSetting[i].m_MapShowLimbus = 0;
		InWndSetting[i].m_MapShowCornealVertex = 0;
		InWndSetting[i].m_MapTranslucent = 0;

		InWndSetting[i].m_Method3D = 0;
		InWndSetting[i].m_MapShowSolidSurface = TRUE;
		InWndSetting[i].m_MapShowWireMesh = TRUE;
		InWndSetting[i].m_ZoneRadiusUm = 5000;
		InWndSetting[i].m_SizeSmall = TRUE;

		InWndSetting[i].SetDefaultScale();
		InWndSetting[i].SetDefaultColors(1);
	}

	for (int i = 2; i< 4; i++)
	{
		InWndSetting[i].m_Type = 14;
		InWndSetting[i].m_Ltr = 0;
		InWndSetting[i].m_LtrLine = 70;
		InWndSetting[i].m_LtrOrientation = 0;
		InWndSetting[i].m_Method3D = 0;
		InWndSetting[i].m_MapShowSolidSurface = TRUE;
		InWndSetting[i].m_MapShowWireMesh = TRUE;
		InWndSetting[i].m_ZoneRadiusUm = 5000;
		InWndSetting[i].m_SizeSmall = TRUE;

		InWndSetting[i].SetDefaultScale();
		InWndSetting[i].SetDefaultColors(1);
	}
}
//530
//********************************************************************************************
//530
void CSettingsComboDlg::SetDefTask_WFRM(CWndSettings InWndSetting[4])
{
	InWndSetting[0] = ::Settings.m_TSsmSettings[1][0];
	InWndSetting[1] = ::Settings.m_TSsmSettings[1][1];
	InWndSetting[2] = ::Settings.m_TSsmSettings[1][2];
	InWndSetting[3] = ::Settings.m_TSsmSettings[1][3];

	InWndSetting[0].m_Mask.SetType(MASK_TOTAL);
	InWndSetting[0].m_Piston = TRUE;
	InWndSetting[0].m_Tilt = TRUE;

	InWndSetting[1].m_Mask.SetType(MASK_HO_TOTAL);
	InWndSetting[1].m_Piston = FALSE;
	InWndSetting[1].m_Tilt = FALSE;

	InWndSetting[2].m_Mask.SetType(MASK_LO_TOTAL);
	InWndSetting[2].m_Piston = FALSE;
	InWndSetting[2].m_Tilt = FALSE;

	InWndSetting[3].m_Mask.SetType(MASK_TOTAL);
	InWndSetting[3].m_Piston = TRUE;
	InWndSetting[3].m_Tilt = TRUE;

	for (int i = 0; i< 3; i++)
	{
		InWndSetting[i].m_Type = 10;

		InWndSetting[i].m_Method3D = 0;
		InWndSetting[i].m_MapShowSolidSurface = TRUE;
		InWndSetting[i].m_MapShowWireMesh = TRUE;
		InWndSetting[i].m_ZoneRadiusUm = 5000;

		InWndSetting[i].m_SizeSmall = TRUE;

		InWndSetting[i].m_MapShowMap = 1;
		InWndSetting[i].m_MapShowEye = 0;
		InWndSetting[i].m_MapShowNumbers = 0;
		InWndSetting[i].m_MapShowPupil = 0;
		InWndSetting[i].m_MapShowLimbus = 0;
		InWndSetting[i].m_MapShowCornealVertex = 0;
		InWndSetting[i].m_MapTranslucent = 0;

		InWndSetting[i].SetDefaultScale();
		InWndSetting[i].SetDefaultColors(1);
	}

	InWndSetting[3].m_Type = 16;
	InWndSetting[3].m_ZoneRadiusUm = 5000;
	InWndSetting[3].m_RMSIndividual = FALSE;
	InWndSetting[3].m_SizeSmall = TRUE;
}
//530
//********************************************************************************************
//530
void CSettingsComboDlg::SetDefTask_CT3D(CWndSettings InWndSetting[4])
{
	InWndSetting[0] = ::Settings.m_CSsmSettings[2][0];
	InWndSetting[1] = ::Settings.m_CSsmSettings[2][1];
	InWndSetting[2] = ::Settings.m_CSsmSettings[2][2];
	InWndSetting[3] = ::Settings.m_CSsmSettings[2][3];

	InWndSetting[0].m_Type = 34; //z elevation     TYPE_CELM 
	InWndSetting[0].m_SizeSmall = FALSE;
	InWndSetting[0].m_Method3D = 1;
	InWndSetting[0].m_MapShowSolidSurface = TRUE;
	InWndSetting[0].m_MapShowWireMesh = TRUE;
	InWndSetting[0].SetDefaultScale();
	InWndSetting[0].SetDefaultColors(1);
	InWndSetting[0].m_RefSurfaceType = REF_SURFACE_BEST_SPHERE;

}
//530
//********************************************************************************************
//530
void CSettingsComboDlg::SetDefTask_WCSU(CWndSettings InWndSetting[4])
{
	InWndSetting[0] = ::Settings.m_IDsmSettings[0][0];
	InWndSetting[1] = ::Settings.m_IDsmSettings[0][1];
	InWndSetting[2] = ::Settings.m_IDsmSettings[0][2];
	InWndSetting[3] = ::Settings.m_IDsmSettings[0][3];

	InWndSetting[0].m_Type = 40; //Wavefront map     
	InWndSetting[1].m_Type = 36; //Wavefront map     
	InWndSetting[2].m_Type = 16; //Wavefront map  

	InWndSetting[0].m_Method3D = 0;
	InWndSetting[0].m_MapShowSolidSurface = TRUE;
	InWndSetting[0].m_MapShowWireMesh = TRUE;
	InWndSetting[0].m_SizeSmall = TRUE;
	InWndSetting[0].m_Mask.SetType(MASK_TOTAL_NO_DEFOCUS);
	InWndSetting[0].m_Piston = TRUE;
	InWndSetting[0].m_Tilt = TRUE;
	InWndSetting[0].m_MapShowMap = 1;
	InWndSetting[0].m_ZoneRadiusUm = 5000;
	InWndSetting[0].m_MapShowEye = 0;
	InWndSetting[0].m_MapShowNumbers = 0;
	InWndSetting[0].m_MapShowPupil = 0;
	InWndSetting[0].m_MapShowLimbus = 0;
	InWndSetting[0].m_MapShowCornealVertex = 0;
	InWndSetting[0].m_MapTranslucent = 0;
	InWndSetting[0].m_MapShowKeratometry = 0;
	InWndSetting[0].m_MapShowKeratometry = 0;
	InWndSetting[0].m_MapShowAstigmatismAxes = 0;
	InWndSetting[0].SetDefaultColors(1);


	InWndSetting[3].m_Type = 31; //Axial map       TYPE_CAXM
	InWndSetting[3].m_Method3D = 0;
	InWndSetting[3].m_MapShowSolidSurface = TRUE;
	InWndSetting[3].m_MapShowWireMesh = TRUE;
	InWndSetting[3].m_SizeSmall = TRUE;
	InWndSetting[3].m_MapUnit = DIOPTERS;
	InWndSetting[3].m_MapShowMap = 1;
	InWndSetting[3].m_MapShowEye = 1;
	InWndSetting[3].m_MapShowNumbers = 0;
	InWndSetting[3].m_MapShowPupil = 0;
	InWndSetting[3].m_MapShowLimbus = 0;
	InWndSetting[3].m_MapShowCornealVertex = 0;
	InWndSetting[3].m_MapTranslucent = 0;
	InWndSetting[3].m_MapShowKeratometry = 1;

	InWndSetting[0].SetDefaultScale();
	InWndSetting[0].SetDefaultColors(1);
}

//********************************************************************************************
//Default setting of Astigmastism Source inside the task
void CSettingsComboDlg::SetDefTask_WCAT(CWndSettings InWndSetting[4])
{
	InWndSetting[0] = ::Settings.m_IDsmSettings[0][0];
	InWndSetting[1] = ::Settings.m_IDsmSettings[0][1];
	InWndSetting[2] = ::Settings.m_IDsmSettings[0][2];
	InWndSetting[3] = ::Settings.m_IDsmSettings[0][3];

	//Astiagmatism source 530 
	InWndSetting[0] = ::Settings.m_IDsmSettings[2][0];
	InWndSetting[1] = ::Settings.m_IDsmSettings[2][1];
	InWndSetting[2] = ::Settings.m_IDsmSettings[2][2];
	InWndSetting[3] = ::Settings.m_IDsmSettings[2][3];
	for (int i = 0; i < 4; i++)
	{
		InWndSetting[i].m_SizeSmall = TRUE;
		InWndSetting[i].m_SimplifiedRMS = TRUE;

		if (i != 3) InWndSetting[i].m_Mask.SetType(MASK_ASTIGMATISM);
	}
	//Astiagmatism source 530  done
}

//********************************************************************************************

// Default setting of Depth of focus inside the task
void CSettingsComboDlg::SetDefTask_WCDP(CWndSettings InWndSetting[4])
{
	for (int i = 0; i < 4; i++)
	{
		InWndSetting[i] = ::Settings.m_TSsmSettings[0][i];
		InWndSetting[i].m_SizeSmall = FALSE;
		InWndSetting[i].m_Type = TYPE_DOFF;
	}
}

//********************************************************************************

CTaskSetting CSettingsComboDlg::SetDefTask1()
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

CTaskSetting CSettingsComboDlg::SetDefTask2()
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

CTaskSetting CSettingsComboDlg::SetDefTask0()
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

CTaskSetting CSettingsComboDlg::SetDefTask3()
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

CTaskSetting CSettingsComboDlg::SetDefTask4()
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

void CSettingsComboDlg::HideTaskSettings()
{
	for (int i = 0; i<5; i++)
	{
		m_TaskRadio[i].ShowWindow(FALSE);
		m_TaskEdit[i].ShowWindow(FALSE);
		m_SubTask_Check[i].ShowWindow(FALSE);
	}
	m_TaskAdBtn.ShowWindow(FALSE);
}

//********************************************************************************

void CSettingsComboDlg::ShowTaskSettings()
{
	for (int i = 0; i<5; i++)
	{
		m_TaskRadio[i].ShowWindow(TRUE);
		m_TaskEdit[i].ShowWindow(TRUE);
		m_SubTask_Check[i].ShowWindow(TRUE);
	}

	m_TaskAdBtn.ShowWindow(TRUE);
}

//********************************************************************************

void CSettingsComboDlg::OnTaskShowBtnClicked()
{
	BOOL Checked = m_Task_ShortCutCheck.GetCheck();

	if (Checked) ShowTaskSettings();
	else        HideTaskSettings();

	Invalidate(FALSE);
}

//*********************************************************************************

void CSettingsComboDlg::OnSuperExamBtnClicked()
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

	Invalidate(FALSE);
}

//***********************************************************************************************

void CSettingsComboDlg::OnSubTasckRioClicked(int i)
{
	BOOL Chk = m_SubTask_Check[i].GetCheck();

	if (!Chk)
	{
		m_TaskRadio[i].SetCheck(FALSE);

		if (m_SubTask_Check[m_SelTaskRadio].GetCheck())
			m_TaskRadio[m_SelTaskRadio].SetCheck(TRUE);
		else
		{
			for (int j = 0; j < 5; j++)
			{
				if (m_SubTask_Check[j].GetCheck())
				{
					m_TaskRadio[j].SetCheck(TRUE);
					m_SelTaskRadio = j;
					break;
				}
			}
		}
	}
	else m_SelTaskRadio = i;
}

//***********************************************************************************************


void CSettingsComboDlg::OnSubTasckChkClicked(int i)
{
	BOOL Chk = m_SubTask_Check[i].GetCheck();

	m_TaskEdit[i].EnableWindow(Chk);
	m_TaskRadio[i].EnableWindow(Chk);
	if (!m_CtlDef)//
	{
		m_DlgTask[i] = ::NewSettings.m_Tasks[i];
	}
	m_TaskChanged[i] = TRUE;
	m_DlgTask[i].m_Enable = Chk;
	::NewSettings.m_Tasks[i].m_Enable = Chk;

	BOOL RadioChk = FALSE;

	for (int i = 0; i < 5; i++)
	{
		if (m_TaskRadio[i].GetCheck() && !m_DlgTask[i].m_Enable)
		{
			RadioChk = TRUE;
			break;
		}
	}


	if (RadioChk)
	{
		for (int i = 0; i < 5; i++) m_TaskRadio[i].SetCheck(FALSE);

		for (int i = 0; i < 5; i++)
		{
			if (m_DlgTask[i].m_Enable)
			{
				m_TaskRadio[i].SetCheck(TRUE);
				break;
			}
		}
	}

	BOOL HasChk = FALSE;
	for (int i = 0; i < 5; i++)
	{
		if (m_TaskRadio[i].GetCheck())
		{
			HasChk = TRUE;
			m_SelTaskRadio = i;
			break;
		}
	}

	if (!HasChk)
	{
		for (int i = 0; i < 5; i++)
		{
			if (m_DlgTask[i].m_Enable)
			{
				m_TaskRadio[i].SetCheck(TRUE);
				HasChk = TRUE;
				m_SelTaskRadio = i;
				break;
			}
		}
	}

	if (!HasChk) m_TaskAdBtn.EnableWindow(FALSE);
	else        m_TaskAdBtn.EnableWindow(TRUE);
}

//***********************************************************************************************

void CSettingsComboDlg::OnWFChkClicked(int i)
{
	BOOL Chk = m_WFSum_Check[i].GetCheck();

	int NumChk;

	if (Chk)
	{
		NumChk = 0;

		// add the new functionality 'Depth of Focus Comparison'
		//6.2.0 Add 'WF Near vision'
		for (int j = 1; j <= 9; j++)
		{
			if (m_WFSum_Check[j].GetCheck())
				NumChk++;
		}

		if (NumChk >= 7)
		{
			m_WFSum_Check[i].SetCheck(FALSE);
			::Info("The maxmum WF displays is 7.");
			return;
		}
	}

	//
	NumChk = 0;

	for (int j = 1; j <= 9; j++)
	{
		if (m_WFSum_Check[j].GetCheck())
			NumChk++;
	}

	CString s;
	s.Format(_T("%i"), NumChk + 1);
	m_SelNumText[0].SetWindowText(s);
	//

	if (i == 1)
	{
		if (!Chk && m_WFVfaRadio.GetCheck())
		{
			m_WFVfaRadio.SetCheck(FALSE);
			m_WFVrfRadio.SetCheck(TRUE);
		}
		m_WFVfaRadio.EnableWindow(Chk);
	}
	else if (i == 2)
	{
		if (!Chk && m_WFCmpRadio.GetCheck())
		{
			m_WFCmpRadio.SetCheck(FALSE);
			m_WFVrfRadio.SetCheck(TRUE);
		}
		m_WFCmpRadio.EnableWindow(Chk);
	}
	else if (i == 3)
	{
		if (!Chk && m_WFSum1Radio.GetCheck())
		{
			m_WFSum1Radio.SetCheck(FALSE);
			m_WFVrfRadio.SetCheck(TRUE);
		}
		m_WFSum1Radio.EnableWindow(Chk);
		m_Button1Edit.EnableWindow(Chk);
	}
	else if (i == 4)
	{
		if (!Chk && m_WFSum2Radio.GetCheck())
		{
			m_WFSum2Radio.SetCheck(FALSE);
			m_WFVrfRadio.SetCheck(TRUE);
		}
		m_WFSum2Radio.EnableWindow(Chk);
		m_Button2Edit.EnableWindow(Chk);
	}
	else if (i == 5)
	{
		if (!Chk && m_WFSum3Radio.GetCheck())
		{
			m_WFSum3Radio.SetCheck(FALSE);
			m_WFVrfRadio.SetCheck(TRUE);
		}
		m_WFSum3Radio.EnableWindow(Chk);
		m_Button3Edit.EnableWindow(Chk);
	}
	else if (i == 6)
	{
		if (!Chk && m_WFSum4Radio.GetCheck())
		{
			m_WFSum4Radio.SetCheck(FALSE);
			m_WFVrfRadio.SetCheck(TRUE);
		}
		m_WFSum4Radio.EnableWindow(Chk);
		m_Button4Edit2.EnableWindow(Chk);
	}
	else if (i == 7)
	{
		if (!Chk && m_WFSum5Radio.GetCheck())
		{
			m_WFSum5Radio.SetCheck(FALSE);
			m_WFVrfRadio.SetCheck(TRUE);
		}
		m_WFSum5Radio.EnableWindow(Chk);
		m_Button4Edit.EnableWindow(Chk);
	}
	else if (i == 8)
	{
		if (!Chk && m_WFSum6Radio.GetCheck())
		{
			m_WFSum6Radio.SetCheck(FALSE);
			m_WFVrfRadio.SetCheck(TRUE);
		}
		m_WFSum6Radio.EnableWindow(Chk);
		m_Button14Edit.EnableWindow(Chk);
	}
	//6.2.0
	else if (i == 9)
	{
		if (!Chk && m_WFNearVisionRadio.GetCheck())
		{
			m_WFNearVisionRadio.SetCheck(FALSE);
			m_WFVrfRadio.SetCheck(TRUE);
		}
		m_WFNearVisionRadio.EnableWindow(Chk);
	}
	//6.2.0

	if (i == 1 || i == 2 || i == 9) Invalidate(FALSE);//6.2.0 make the text become blue
}

//***********************************************************************************************

void CSettingsComboDlg::OnCTChkClicked(int i)
{
	BOOL Chk = m_CTSum_Check[i].GetCheck();

	int NumChk;

	if (Chk)
	{
		NumChk = 0;

		for (int j = 1; j < 8; j++)
		{
			if (m_CTSum_Check[j].GetCheck())
				NumChk++;
		}

		if (NumChk == 7)
		{
			m_CTSum_Check[i].SetCheck(FALSE);
			::Info("The maxmum CT displays is 7.");
			return;
		}
	}

	//
	NumChk = 0;

	for (int j = 1; j < 8; j++)
	{
		if (m_CTSum_Check[j].GetCheck())
			NumChk++;
	}

	CString s;
	s.Format(_T("%i"), NumChk + 1);
	m_SelNumText[1].SetWindowText(s);
	//


	if (i == 1)
	{
		if (!Chk && m_CTDblRadio.GetCheck())
		{
			m_CTDblRadio.SetCheck(FALSE);
			m_CTVrfRadio.SetCheck(TRUE);
		}
		m_CTDblRadio.EnableWindow(Chk);

	}
	else if (i == 2)
	{
		if (!Chk && m_CTCmpRadio.GetCheck())
		{
			m_CTCmpRadio.SetCheck(FALSE);
			m_CTVrfRadio.SetCheck(TRUE);
		}
		m_CTCmpRadio.EnableWindow(Chk);
	}
	else if (i == 3)
	{
		if (!Chk && m_CTSum1Radio.GetCheck())
		{
			m_CTSum1Radio.SetCheck(FALSE);
			m_CTVrfRadio.SetCheck(TRUE);
		}
		m_CTSum1Radio.EnableWindow(Chk);
		m_Button5Edit.EnableWindow(Chk);
	}
	else if (i == 4)
	{
		if (!Chk && m_CTSum2Radio.GetCheck())
		{
			m_CTSum2Radio.SetCheck(FALSE);
			m_CTVrfRadio.SetCheck(TRUE);
		}
		m_CTSum2Radio.EnableWindow(Chk);
		m_Button6Edit.EnableWindow(Chk);
	}
	else if (i == 5)
	{
		if (!Chk && m_CTSum3Radio.GetCheck())
		{
			m_CTSum3Radio.SetCheck(FALSE);
			m_CTVrfRadio.SetCheck(TRUE);
		}
		m_CTSum3Radio.EnableWindow(Chk);
		m_Button7Edit.EnableWindow(Chk);
	}
	else if (i == 6)
	{
		if (!Chk && m_CTSum4Radio.GetCheck())
		{
			m_CTSum4Radio.SetCheck(FALSE);
			m_CTVrfRadio.SetCheck(TRUE);
		}
		m_CTSum4Radio.EnableWindow(Chk);
		m_Button8Edit.EnableWindow(Chk);
	}
	else if (i == 7)
	{
		if (!Chk && m_CTSum5Radio.GetCheck())
		{
			m_CTSum5Radio.SetCheck(FALSE);
			m_CTVrfRadio.SetCheck(TRUE);
		}
		m_CTSum5Radio.EnableWindow(Chk);
		m_Button8Edit2.EnableWindow(Chk);
	}

	if (i == 1 || i == 2) Invalidate(FALSE);//
}

//***********************************************************************************************


void CSettingsComboDlg::OnINChkClicked(int i)
{
	//
	int NumChk = 0;
	int WhichChk = 0;
	for (int j = 10; j >= 0; j--)
	{
		if (m_INSum_Check[j].GetCheck())
		{
			NumChk++;
			if (i != j) WhichChk = j;
		}
	}

	//
	BOOL Chk = m_INSum_Check[i].GetCheck();

	if (!Chk && NumChk == 0)
	{
		m_INSum_Check[i].SetCheck(TRUE);
		::Info("There must be at lease one WF&CT summary display selected.");
		return;
	}

	if (Chk && NumChk == 8)
	{
		m_INSum_Check[i].SetCheck(FALSE);
		::Info("The maxmum WF&CT displays is 7.");
		return;
	}

	//
	CString s;
	s.Format(_T("%i"), NumChk);
	m_SelNumText[2].SetWindowText(s);
	//

	if (i == 0)
	{
		if (!Chk && m_INSum1Radio.GetCheck())
		{
			if (WhichChk == 0) m_INSum1Radio.SetCheck(TRUE);
			else if (WhichChk == 1) m_INSum2Radio.SetCheck(TRUE);
			else if (WhichChk == 2) m_INSum3Radio.SetCheck(TRUE);
			else if (WhichChk == 3) m_INSum4Radio.SetCheck(TRUE);
			else if (WhichChk == 4) m_INSum5Radio.SetCheck(TRUE);
			else if (WhichChk == 5) m_INSum6Radio.SetCheck(TRUE);
			else if (WhichChk == 6) m_INSum7Radio.SetCheck(TRUE);
			else if (WhichChk == 7) m_INSum8Radio.SetCheck(TRUE);
			else if (WhichChk == 8) m_INSum9Radio.SetCheck(TRUE);
			else if (WhichChk == 9) m_INSum10Radio.SetCheck(TRUE);// WFCT Angle ka
			else if (WhichChk == 10) m_INSum11Radio.SetCheck(TRUE);// Toric Alignment
			m_INSum1Radio.SetCheck(FALSE);
		}
		m_INSum1Radio.EnableWindow(Chk);
		m_Button9Edit.EnableWindow(Chk);
	}
	else if (i == 1)
	{
		if (!Chk && m_INSum2Radio.GetCheck())
		{
			if (WhichChk == 0) m_INSum1Radio.SetCheck(TRUE);
			else if (WhichChk == 1) m_INSum2Radio.SetCheck(TRUE);
			else if (WhichChk == 2) m_INSum3Radio.SetCheck(TRUE);
			else if (WhichChk == 3) m_INSum4Radio.SetCheck(TRUE);
			else if (WhichChk == 4) m_INSum5Radio.SetCheck(TRUE);
			else if (WhichChk == 5) m_INSum6Radio.SetCheck(TRUE);
			else if (WhichChk == 6) m_INSum7Radio.SetCheck(TRUE);
			else if (WhichChk == 7) m_INSum8Radio.SetCheck(TRUE);
			else if (WhichChk == 8) m_INSum9Radio.SetCheck(TRUE);
			else if (WhichChk == 9) m_INSum10Radio.SetCheck(TRUE);// WFCT Angle ka
			else if (WhichChk == 10) m_INSum11Radio.SetCheck(TRUE);// Toric Alignment
			m_INSum2Radio.SetCheck(FALSE);
		}
		m_INSum2Radio.EnableWindow(Chk);
		m_Button10Edit.EnableWindow(Chk);
	}
	else if (i == 2)
	{
		if (!Chk && m_INSum3Radio.GetCheck())
		{
			if (WhichChk == 0) m_INSum1Radio.SetCheck(TRUE);
			else if (WhichChk == 1) m_INSum2Radio.SetCheck(TRUE);
			else if (WhichChk == 2) m_INSum3Radio.SetCheck(TRUE);
			else if (WhichChk == 3) m_INSum4Radio.SetCheck(TRUE);
			else if (WhichChk == 4) m_INSum5Radio.SetCheck(TRUE);
			else if (WhichChk == 5) m_INSum6Radio.SetCheck(TRUE);
			else if (WhichChk == 6) m_INSum7Radio.SetCheck(TRUE);
			else if (WhichChk == 7) m_INSum8Radio.SetCheck(TRUE);
			else if (WhichChk == 8) m_INSum9Radio.SetCheck(TRUE);
			else if (WhichChk == 9) m_INSum10Radio.SetCheck(TRUE);// WFCT Angle ka
			else if (WhichChk == 10) m_INSum11Radio.SetCheck(TRUE);// Toric Alignment
			m_INSum3Radio.SetCheck(FALSE);
		}
		m_INSum3Radio.EnableWindow(Chk);
		m_Button11Edit.EnableWindow(Chk);
	}
	else if (i == 3)
	{
		if (!Chk && m_INSum4Radio.GetCheck())
		{
			if (WhichChk == 0) m_INSum1Radio.SetCheck(TRUE);
			else if (WhichChk == 1) m_INSum2Radio.SetCheck(TRUE);
			else if (WhichChk == 2) m_INSum3Radio.SetCheck(TRUE);
			else if (WhichChk == 3) m_INSum4Radio.SetCheck(TRUE);
			else if (WhichChk == 4) m_INSum5Radio.SetCheck(TRUE);
			else if (WhichChk == 5) m_INSum6Radio.SetCheck(TRUE);
			else if (WhichChk == 6) m_INSum7Radio.SetCheck(TRUE);
			else if (WhichChk == 7) m_INSum8Radio.SetCheck(TRUE);
			else if (WhichChk == 8) m_INSum9Radio.SetCheck(TRUE);
			else if (WhichChk == 9) m_INSum10Radio.SetCheck(TRUE);// WFCT Angle ka
			else if (WhichChk == 10) m_INSum11Radio.SetCheck(TRUE);// Toric Alignment
			m_INSum4Radio.SetCheck(FALSE);
		}
		m_INSum4Radio.EnableWindow(Chk);
	}
	else if (i == 4)
	{
		if (!Chk && m_INSum5Radio.GetCheck())
		{
			if (WhichChk == 0) m_INSum1Radio.SetCheck(TRUE);
			else if (WhichChk == 1) m_INSum2Radio.SetCheck(TRUE);
			else if (WhichChk == 2) m_INSum3Radio.SetCheck(TRUE);
			else if (WhichChk == 3) m_INSum4Radio.SetCheck(TRUE);
			else if (WhichChk == 4) m_INSum5Radio.SetCheck(TRUE);
			else if (WhichChk == 5) m_INSum6Radio.SetCheck(TRUE);
			else if (WhichChk == 6) m_INSum7Radio.SetCheck(TRUE);
			else if (WhichChk == 7) m_INSum8Radio.SetCheck(TRUE);
			else if (WhichChk == 8) m_INSum9Radio.SetCheck(TRUE);
			else if (WhichChk == 9) m_INSum10Radio.SetCheck(TRUE);// WFCT Angle ka
			else if (WhichChk == 10) m_INSum11Radio.SetCheck(TRUE);// Toric Alignment
			m_INSum5Radio.SetCheck(FALSE);
		}
		m_INSum5Radio.EnableWindow(Chk);
	}
	else if (i == 5)
	{
		if (!Chk && m_INSum6Radio.GetCheck())
		{
			if (WhichChk == 0) m_INSum1Radio.SetCheck(TRUE);
			else if (WhichChk == 1) m_INSum2Radio.SetCheck(TRUE);
			else if (WhichChk == 2) m_INSum3Radio.SetCheck(TRUE);
			else if (WhichChk == 3) m_INSum4Radio.SetCheck(TRUE);
			else if (WhichChk == 4) m_INSum5Radio.SetCheck(TRUE);
			else if (WhichChk == 5) m_INSum6Radio.SetCheck(TRUE);
			else if (WhichChk == 6) m_INSum7Radio.SetCheck(TRUE);
			else if (WhichChk == 7) m_INSum8Radio.SetCheck(TRUE);
			else if (WhichChk == 8) m_INSum9Radio.SetCheck(TRUE);
			else if (WhichChk == 9) m_INSum10Radio.SetCheck(TRUE);// WFCT Angle ka
			else if (WhichChk == 10) m_INSum11Radio.SetCheck(TRUE);// Toric Alignment
			m_INSum6Radio.SetCheck(FALSE);
		}
		m_INSum6Radio.EnableWindow(Chk);
	}
	else if (i == 6)
	{
		if (!Chk && m_INSum7Radio.GetCheck())
		{
			if (WhichChk == 0) m_INSum1Radio.SetCheck(TRUE);
			else if (WhichChk == 1) m_INSum2Radio.SetCheck(TRUE);
			else if (WhichChk == 2) m_INSum3Radio.SetCheck(TRUE);
			else if (WhichChk == 3) m_INSum4Radio.SetCheck(TRUE);
			else if (WhichChk == 4) m_INSum5Radio.SetCheck(TRUE);
			else if (WhichChk == 5) m_INSum6Radio.SetCheck(TRUE);
			else if (WhichChk == 6) m_INSum7Radio.SetCheck(TRUE);
			else if (WhichChk == 7) m_INSum8Radio.SetCheck(TRUE);
			else if (WhichChk == 8) m_INSum9Radio.SetCheck(TRUE);
			else if (WhichChk == 9) m_INSum10Radio.SetCheck(TRUE);// WFCT Angle ka
			else if (WhichChk == 10) m_INSum11Radio.SetCheck(TRUE);// Toric Alignment
			m_INSum7Radio.SetCheck(FALSE);
		}
		m_INSum7Radio.EnableWindow(Chk);
	}
	else if (i == 7)
	{
		if (!Chk && m_INSum8Radio.GetCheck())
		{
			if (WhichChk == 0) m_INSum1Radio.SetCheck(TRUE);
			else if (WhichChk == 1) m_INSum2Radio.SetCheck(TRUE);
			else if (WhichChk == 2) m_INSum3Radio.SetCheck(TRUE);
			else if (WhichChk == 3) m_INSum4Radio.SetCheck(TRUE);
			else if (WhichChk == 4) m_INSum5Radio.SetCheck(TRUE);
			else if (WhichChk == 5) m_INSum6Radio.SetCheck(TRUE);
			else if (WhichChk == 6) m_INSum7Radio.SetCheck(TRUE);
			else if (WhichChk == 7) m_INSum8Radio.SetCheck(TRUE);
			else if (WhichChk == 8) m_INSum9Radio.SetCheck(TRUE);
			else if (WhichChk == 9) m_INSum10Radio.SetCheck(TRUE);// WFCT Angle ka
			else if (WhichChk == 10) m_INSum11Radio.SetCheck(TRUE);// Toric Alignment
			m_INSum8Radio.SetCheck(FALSE);
		}
		m_INSum8Radio.EnableWindow(Chk);
		m_Button12Edit.EnableWindow(Chk);
	}
	else if (i == 8)
	{
		if (!Chk && m_INSum9Radio.GetCheck())
		{
			if (WhichChk == 0) m_INSum1Radio.SetCheck(TRUE);
			else if (WhichChk == 1) m_INSum2Radio.SetCheck(TRUE);
			else if (WhichChk == 2) m_INSum3Radio.SetCheck(TRUE);
			else if (WhichChk == 3) m_INSum4Radio.SetCheck(TRUE);
			else if (WhichChk == 4) m_INSum5Radio.SetCheck(TRUE);
			else if (WhichChk == 5) m_INSum6Radio.SetCheck(TRUE);
			else if (WhichChk == 6) m_INSum7Radio.SetCheck(TRUE);
			else if (WhichChk == 7) m_INSum8Radio.SetCheck(TRUE);
			else if (WhichChk == 8) m_INSum9Radio.SetCheck(TRUE);
			else if (WhichChk == 9) m_INSum10Radio.SetCheck(TRUE);// WFCT Angle ka
			else if (WhichChk == 10) m_INSum11Radio.SetCheck(TRUE);// Toric Alignment
			m_INSum9Radio.SetCheck(FALSE);
		}
		m_INSum9Radio.EnableWindow(Chk);
		m_Button13Edit.EnableWindow(Chk);
	}
	// WFCT Angle ka
	else if (i == 9)
	{
		if (!Chk && m_INSum10Radio.GetCheck())
		{
			if (WhichChk == 0) m_INSum1Radio.SetCheck(TRUE);
			else if (WhichChk == 1) m_INSum2Radio.SetCheck(TRUE);
			else if (WhichChk == 2) m_INSum3Radio.SetCheck(TRUE);
			else if (WhichChk == 3) m_INSum4Radio.SetCheck(TRUE);
			else if (WhichChk == 4) m_INSum5Radio.SetCheck(TRUE);
			else if (WhichChk == 5) m_INSum6Radio.SetCheck(TRUE);
			else if (WhichChk == 6) m_INSum7Radio.SetCheck(TRUE);
			else if (WhichChk == 7) m_INSum8Radio.SetCheck(TRUE);
			else if (WhichChk == 8) m_INSum9Radio.SetCheck(TRUE);
			else if (WhichChk == 9) m_INSum10Radio.SetCheck(TRUE);
			else if (WhichChk == 10) m_INSum11Radio.SetCheck(TRUE);// Toric Alignment
			m_INSum10Radio.SetCheck(FALSE);
		}
		m_INSum10Radio.EnableWindow(Chk);
		m_Button15Edit.EnableWindow(Chk);
	}
	// WFCT Angle ka Done
	// Toric Alignment
	else if (i == 10)
	{
		if (!Chk && m_INSum11Radio.GetCheck())
		{
			if (WhichChk == 0) m_INSum1Radio.SetCheck(TRUE);
			else if (WhichChk == 1) m_INSum2Radio.SetCheck(TRUE);
			else if (WhichChk == 2) m_INSum3Radio.SetCheck(TRUE);
			else if (WhichChk == 3) m_INSum4Radio.SetCheck(TRUE);
			else if (WhichChk == 4) m_INSum5Radio.SetCheck(TRUE);
			else if (WhichChk == 5) m_INSum6Radio.SetCheck(TRUE);
			else if (WhichChk == 6) m_INSum7Radio.SetCheck(TRUE);
			else if (WhichChk == 7) m_INSum8Radio.SetCheck(TRUE);
			else if (WhichChk == 8) m_INSum9Radio.SetCheck(TRUE);
			else if (WhichChk == 9) m_INSum10Radio.SetCheck(TRUE);
			else if (WhichChk == 10) m_INSum11Radio.SetCheck(TRUE);// Toric Alignment
			m_INSum11Radio.SetCheck(FALSE);
		}
		m_INSum11Radio.EnableWindow(Chk);
	}
	// Toric Alignment Done

	if (i == 3 || i == 4 || i == 5 || i == 6 || i == 10)
	{
		Invalidate(FALSE);
	}
}

//***********************************************************************************************
//
void  CSettingsComboDlg::SetSettingsDefault()
{
	if (m_ExcDef)
	{

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
	}

	if (m_GUIDef)
	{
		//530 The default simplified RMS GUI is TRUE
		for (int i = 0; i < 4; i++)
		{
			if (i == 0)
			{
				::Settings.m_TScmSettings.m_SimplifiedRMS = TRUE;
				::Settings.m_TScmSettings.m_SimplifiedWFPanel = TRUE;
				::Settings.m_TScmSettings.m_SimplifiedCTPanel = TRUE;

				::Settings.m_TMsmSettings.m_SimplifiedRMS = TRUE;
				::Settings.m_TMsmSettings.m_SimplifiedWFPanel = TRUE;
				::Settings.m_TMsmSettings.m_SimplifiedCTPanel = TRUE;

				::Settings.m_CSvrSettings.m_SimplifiedRMS = TRUE;
				::Settings.m_CSvrSettings.m_SimplifiedWFPanel = TRUE;
				::Settings.m_CSvrSettings.m_SimplifiedCTPanel = TRUE;

				::Settings.m_CScmSettings.m_SimplifiedRMS = TRUE;
				::Settings.m_CScmSettings.m_SimplifiedWFPanel = TRUE;
				::Settings.m_CScmSettings.m_SimplifiedCTPanel = TRUE;

				::Settings.m_PlannerSettings.m_SimplifiedRMS = TRUE;
				::Settings.m_PlannerSettings.m_SimplifiedWFPanel = TRUE;
				::Settings.m_PlannerSettings.m_SimplifiedCTPanel = TRUE;
			}

			if (i < 2)
			{
				::Settings.m_CDsmSettings[i].m_SimplifiedRMS = TRUE;
				::Settings.m_CDsmSettings[i].m_SimplifiedWFPanel = TRUE;
				::Settings.m_CDsmSettings[i].m_SimplifiedCTPanel = TRUE;
			}

			if (i < 3)
			{
				::Settings.m_WTsmSettings[i].m_SimplifiedRMS = TRUE;
				::Settings.m_WTsmSettings[i].m_SimplifiedWFPanel = TRUE;
				::Settings.m_WTsmSettings[i].m_SimplifiedCTPanel = TRUE;
			}

			::Settings.m_TSvrSettings[i].m_SimplifiedRMS = TRUE;
			::Settings.m_TSvrSettings[i].m_SimplifiedWFPanel = TRUE;
			::Settings.m_TSvrSettings[i].m_SimplifiedCTPanel = TRUE;

			::Settings.m_TDsmSettings[i].m_SimplifiedRMS = TRUE;
			::Settings.m_TDsmSettings[i].m_SimplifiedWFPanel = TRUE;
			::Settings.m_TDsmSettings[i].m_SimplifiedCTPanel = TRUE;
		}

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				::Settings.m_TSsmSettings[i][j].m_SimplifiedRMS = TRUE;
				::Settings.m_TSsmSettings[i][j].m_SimplifiedWFPanel = TRUE;
				::Settings.m_TSsmSettings[i][j].m_SimplifiedCTPanel = TRUE;

				::Settings.m_CSsmSettings[i][j].m_SimplifiedRMS = TRUE;
				::Settings.m_CSsmSettings[i][j].m_SimplifiedWFPanel = TRUE;
				::Settings.m_CSsmSettings[i][j].m_SimplifiedCTPanel = TRUE;

				::Settings.m_IDsmSettings[i][j].m_SimplifiedRMS = TRUE;
				::Settings.m_IDsmSettings[i][j].m_SimplifiedWFPanel = TRUE;
				::Settings.m_IDsmSettings[i][j].m_SimplifiedCTPanel = TRUE;
			}
		}
		//530 The default simplified RMS GUI is TRUE Done


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
		::Settings.m_TSsmSettings[0][0].m_LtrDistanceType = 0;//530

		::Settings.m_TSsmSettings[0][2].m_Mask.SetType(MASK_TOTAL);
		::Settings.m_TSsmSettings[0][2].m_Piston = TRUE;
		::Settings.m_TSsmSettings[0][2].m_Tilt = TRUE;
		::Settings.m_TSsmSettings[0][2].m_LtrDistanceType = 0;//530

		::Settings.m_TSsmSettings[0][1].m_Mask.SetType(MASK_HO_TOTAL);
		::Settings.m_TSsmSettings[0][1].m_Piston = FALSE;
		::Settings.m_TSsmSettings[0][1].m_Tilt = FALSE;
		::Settings.m_TSsmSettings[0][1].m_LtrDistanceType = 0;//530

		::Settings.m_TSsmSettings[0][3].m_Mask.SetType(MASK_HO_TOTAL);
		::Settings.m_TSsmSettings[0][3].m_Piston = FALSE;
		::Settings.m_TSsmSettings[0][3].m_Tilt = FALSE;
		::Settings.m_TSsmSettings[0][3].m_LtrDistanceType = 0;//530


		for (int i = 0; i < 2; i++)
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

		for (int i = 2; i < 4; i++)
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

		for (int i = 0; i < 3; i++)
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
			::Settings.m_CDsmSettings[i].m_MapShowKeratometry = 0;// 530 Keratometry

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
		::Settings.m_CScmSettings.m_MapShowKeratometry = 0;// 530 Keratometry

		CScale* pScale2 = ::Settings.m_CScmSettings.GetScale();
		pScale2->m_Step = 0.5;
		::Settings.m_CScmSettings.SetDefaultColors(1);
		//Done


		//CT Summary
		::Settings.m_CSsmSettings[0][0].m_Type = 31; //Axial map       TYPE_CAXM
		::Settings.m_CSsmSettings[0][1].m_Type = TYPE_TLTR;
		::Settings.m_CSsmSettings[0][2].m_Type = TYPE_CRMS; 
		::Settings.m_CSsmSettings[0][3].m_Type = TYPE_CEYE; 

		::Settings.m_CSsmSettings[0][1].m_ZoneRadiusUm = 2000;
		::Settings.m_CSsmSettings[0][2].m_ZoneRadiusUm = 2000;
	
		::Settings.m_CSsmSettings[0][0].m_Mask.SetType(MASK_HO_TOTAL);
		::Settings.m_CSsmSettings[0][1].m_Mask.SetType(MASK_HO_TOTAL);

		::Settings.m_CSsmSettings[0][0].m_MapShowKeratometry = TRUE;//6.2.0

		::Settings.m_CSsmSettings[0][0].m_MapUnit = DIOPTERS;
		::Settings.m_CSsmSettings[0][2].m_MapUnit = MILLIMETERS;

		::Settings.m_CSsmSettings[0][1].m_LtrLine = 100;

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
			::Settings.m_CSsmSettings[0][i].m_MapShowRings = FALSE;

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
			::Settings.m_CSsmSettings[3][i].m_MapShowKeratometry = 0;// 530 Keratometry

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


		//6.2.0  from WF&CT Visual acuity Display to MTF		
		::Settings.m_IDsmSettings[2][0].m_Method3D = FALSE;
		::Settings.m_IDsmSettings[2][0].m_MapShowSolidSurface = TRUE;
		::Settings.m_IDsmSettings[2][0].m_MapShowWireMesh = TRUE;
		::Settings.m_IDsmSettings[2][0].m_SizeSmall = TRUE;
		::Settings.m_IDsmSettings[2][0].m_ZoneRadiusUm = 5000;
		::Settings.m_IDsmSettings[2][0].m_Mask.SetType(MASK_HO_TOTAL);//6.2.0 MASK_TOTAL_NO_DEFOCUS
		::Settings.m_IDsmSettings[2][0].m_Piston = TRUE;
		::Settings.m_IDsmSettings[2][0].m_Tilt = TRUE;

		//6.2.0
		::Settings.m_IDsmSettings[2][0].m_Type = TYPE_IMTF; 
		::Settings.m_IDsmSettings[2][1].m_Type = TYPE_CMTF;
		::Settings.m_IDsmSettings[2][2].m_Type = TYPE_TMTF; 

		::Settings.m_IDsmSettings[2][0].m_Mask.SetType(MASK_HO_TOTAL);
		::Settings.m_IDsmSettings[2][1].m_Mask.SetType(MASK_HO_TOTAL); 
		::Settings.m_IDsmSettings[2][2].m_Mask.SetType(MASK_HO_TOTAL);
		 //6.2.0

		for (int i = 0; i < 3; i++) ::Settings.m_IDsmSettings[2][i].m_Ltr = 0;

		::Settings.m_IDsmSettings[2][0].m_LtrLine = 70;
		::Settings.m_IDsmSettings[2][0].m_LtrOrientation = 0;

		::Settings.m_IDsmSettings[2][3].m_Type = 31; //Axial map       TYPE_CAXM
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
		//Done

		
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

		//For Dysfunctional lens patient
		::NewSettings.m_DysPtSettings[0] = ::NewSettings.m_DysfuncSettings[1];
		::NewSettings.m_DysPtSettings[0] = ::NewSettings.m_DysfuncSettings[1];
		::NewSettings.m_DysPtSettings[0].m_SimplifiedWFPanel = 1;
		::NewSettings.m_DysPtSettings[0].m_Type = TYPE_CLTR;

		::NewSettings.m_DysPtSettings[1] = ::NewSettings.m_DysfuncSettings[1];

		::NewSettings.m_DysPtSettings[2] = ::NewSettings.m_DysfuncSettings[1];
		::NewSettings.m_DysPtSettings[2].m_Type = TYPE_TLTR;

		::NewSettings.m_DysPtSettings[3] = ::NewSettings.m_DysfuncSettings[3];
		::NewSettings.m_DysPtSettings[4] = ::NewSettings.m_DysfuncSettings[2];

		// For Depth focus display
		for (int i = 0; i < 4; i++)
		{
			::NewSettings.m_TSsm5Settings[i] = ::Settings.m_TSsmSettings[0][i];
			::NewSettings.m_TSsm5Settings[i].m_SizeSmall = FALSE;
			::NewSettings.m_TSsm5Settings[i].m_Type = TYPE_DOFF;
		}

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

		//Others in newsettings
		::NewSettings.m_IncLoC_OD = 0;
		::NewSettings.m_IncLoC_OS = 0;
		::NewSettings.m_SurInAst_OD = 0.0;
		::NewSettings.m_SurInAst_OS = 0.0;
		::NewSettings.m_OSHER_ALIG_CUS[3] = 1;

		SetDef_ExList_Width();//Set the default widths of the exam list	

							  //for screen shot saving
		::NewSettings.m_ScreenShotFormat = 0; //0: save 'jpg' format while the users save the screen	  
											  //Done

											  //For solo Aberration and Refraction analysis settings
		::NewSettings.m_DefaultWFSolo = 1;//0:Point; 1:Refraction; 2:Compare; 3:Aberration; 4:VFA
										  //Refraction Analysis Settings
		for (int i = 0; i < 4; i++)
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

		// For Showing wf or ct buttons setting
		for (int i = 0; i < 8; i++)
		{
			::NewSettings.m_WFSumShow[i] = TRUE;
			::NewSettings.m_CTSumShow[i] = TRUE;
			::NewSettings.m_WFCTSumShow[i] = TRUE;
		}
		// Add the "Depth of Focus comparison and remove the WF/RMS" and transformed 'Angle K/A'
		::NewSettings.m_WFNewSumShow[0] = TRUE;//
		::NewSettings.m_WFSumShow[5] = FALSE;// 

		::NewSettings.m_WFCTNewSumShow[0] = TRUE;//	transformed 'Angle K/A'			
		::NewSettings.m_WFCTNewSumShow[1] = TRUE;//	Toric Alignment

		::NewSettings.m_WFSumShow[3] = FALSE;//
		::NewSettings.m_WFSumShow[4] = FALSE;//
		::NewSettings.m_WFSumShow[7] = FALSE;
		::NewSettings.m_WFSumShow[8] = FALSE;//

		::NewSettings.m_CTSumShow[1] = FALSE;//
		::NewSettings.m_CTSumShow[5] = FALSE;//
		::NewSettings.m_CTSumShow[7] = FALSE;

		::NewSettings.m_WFCTSumShow[2] = FALSE;//
		::NewSettings.m_WFCTSumShow[4] = FALSE;//
		::NewSettings.m_WFCTSumShow[5] = FALSE;//
		::NewSettings.m_WFCTSumShow[7] = FALSE;
		::NewSettings.m_WFCTSumShow[8] = FALSE;
	}
}

//***************************************************************************************
//
HBRUSH CSettingsComboDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	int iID = pWnd->GetDlgCtrlID();

	//Note
	if (iID == IDC_NOTE_TEXT2 || iID == IDC_NOTE_TEXT3 || iID == IDC_NOTE_TEXT4
		|| iID == IDC_SELWFNUM_TEXT || iID == IDC_SELCTNUM_TEXT || iID == IDC_SELWFCTNUM_TEXT)
	{
		pDC->SetTextColor(RED);
	}
	//Note Done

	//Refraction
	if (iID == IDC_WF_POS_CYL_TEXT)
	{
		if (m_PosCylCheck.GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_ROUND_1_8_TEXT)
	{
		if (m_Round8Radio.GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_ROUND_1_100_TEXT)
	{
		if (m_Round100Radio.GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}
	//Refraction Done


	//WF Exam acuqsition	
	if (iID == IDC_WFSUPEREXAM_TEXT && !m_isComboViewer)
	{
		if (m_Super_ExamCheck.GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_WF_ENABLE_PROBING_TEXT && !m_isComboViewer)
	{
		if (m_ProbingEnabledCheck.GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_WF_ENABLE_FOGGING_TEXT && !m_isComboViewer)
	{
		if (m_FoggingEnabledCheck.GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_ACCOMM_TARGET_LIGHT_ON_TEXT && !m_isComboViewer)
	{
		if (m_AccommTargetLightOnCheck.GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}


	if (iID == IDC_WF_SCAN_AFTER_NEW_EXAM_TEXT  && !m_isComboViewer)
	{
		if (m_AnimateAfterNewExamCheck.GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_ALIGNMENT_PUPIL_TEXT && !m_isComboViewer)
	{
		if (m_AlignmentPupilRadio.GetCheck() && !m_Super_ExamCheck.GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_ALIGNMENT_4IRDOTS_TEXT && !m_isComboViewer)
	{
		if (m_Alignment4IrdotsRadio.GetCheck() && !m_Super_ExamCheck.GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}
	//WF Exam acuqsition Done

	//Data format
	if (iID == IDC_DATE_FORMAT_MMDDYYYY_TEXT)
	{
		if (m_MMDDYYYYRadio.GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_DATE_FORMAT_DDMMYYYY_TEXT)
	{
		if (m_DDMMYYYYRadio.GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_DATE_FORMAT_YYYYMMDD_TEXT)
	{
		if (m_YYYYMMDDRadio.GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}
	//Data format Done

	//CT Preference
	if (iID == IDC_SHOW_REF_TEXT)
	{
		if (m_RefkRadio.GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_SHOW_SIM_TEXT)
	{
		if (m_SimkRadio.GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}
	//CT Preference Done

	//Others
	if (iID == IDC_WAVETOUCH_TEXT)
	{
		if (m_WavetouchCheck.GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_SHOW_SEND_ARKR_TEXT)
	{
		if (m_ShowSendARKRCheck.GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_CAP_COLORIMG_TEXT  && !m_isComboViewer)
	{
		if (m_CapColorImgCheck.GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_COLOR_IMG_TEXT1)
	{
		if (m_CapColorImgAutoRadio.GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_COLOR_IMG_TEXT2)
	{
		if (m_CapColorImgManuRadio.GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}
	//Others Done

	//Export data
	if (iID == IDC_JPG_TEXT)
	{
		if (m_UseJPGFormatRadio.GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_PNG_TEXT)
	{
		if (m_UsePNGFormatRadio.GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_INCLUDE_LADS_TEXT)
	{
		if (m_IncludeLADsCheck.GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}
	//Export data Done

	//Export excel
	if (iID == IDC_EXCEL_EXPORT_TEXT)
	{
		if (m_ExcelExportCheck.GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}
	//Export excel Done

	//Task 	
	if (iID == IDC_TASKSHORTCUT_TEXT)
	{
		if (m_Task_ShortCutCheck.GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_TASK1_EDIT)
	{
		if (m_SubTask_Check[0].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_TASK2_EDIT)
	{
		if (m_SubTask_Check[1].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_TASK3_EDIT)
	{
		if (m_SubTask_Check[2].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_TASK4_EDIT)
	{
		if (m_SubTask_Check[3].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_TASK5_EDIT)
	{
		if (m_SubTask_Check[4].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}
	//Task Done

	//WF 	
	if (iID == IDC_STATIC_WF_POINTS)
	{
		pDC->SetTextColor(BLUE);
	}

	if (iID == IDC_STATIC_WF_VFA)
	{
		if (m_WFSum_Check[1].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_STATIC_WF_COMPARE)
	{
		if (m_WFSum_Check[2].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_NAME1_EDIT)
	{
		if (m_WFSum_Check[3].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_NAME4_EDIT2)
	{
		if (m_WFSum_Check[6].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_NAME4_EDIT3)
	{
		if (m_WFSum_Check[8].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_NAME2_EDIT)
	{
		if (m_WFSum_Check[4].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_NAME4_EDIT)
	{
		if (m_WFSum_Check[7].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	//6.2.0 'WF Near vision'
	if (iID == IDC_STATIC_WF_NEARVISION)
	{
		if (m_WFSum_Check[9].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}
	//6.2.0 'WF Near vision'	
	//WF Done	


	//CT 	
	if (iID == IDC_STATIC_CT_VRF)
	{
		pDC->SetTextColor(BLUE);
	}

	if (iID == IDC_STATIC_CT_ODOS)
	{
		if (m_CTSum_Check[1].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_STATIC_CT_COMPARE)
	{
		if (m_CTSum_Check[2].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_NAME5_EDIT)
	{
		if (m_CTSum_Check[3].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_NAME6_EDIT)
	{
		if (m_CTSum_Check[4].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_NAME7_EDIT)
	{
		if (m_CTSum_Check[5].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_NAME8_EDIT)
	{
		if (m_CTSum_Check[6].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_NAME8_EDIT2)
	{
		if (m_CTSum_Check[7].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}
	//CT Done

	//WF & CT 	
	if (iID == IDC_NAME9_EDIT)
	{
		if (m_INSum_Check[0].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_NAME10_EDIT)
	{
		if (m_INSum_Check[1].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_NAME11_EDIT)
	{
		if (m_INSum_Check[2].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_STATIC_WFCT_OU)
	{
		if (m_INSum_Check[3].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_STATIC_WFCT_IOL)
	{
		if (m_INSum_Check[4].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_NAME11_EDIT2)
	{
		if (m_INSum_Check[9].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_STATIC_WFCT_DYSMD)
	{
		if (m_INSum_Check[5].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_STATIC_WFCT_DYSPT)
	{
		if (m_INSum_Check[6].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_NAME12_EDIT)
	{
		if (m_INSum_Check[7].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_STATIC_WFCT_TORALI)
	{
		if (m_INSum_Check[10].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}

	if (iID == IDC_NAME12_EDIT3)
	{
		if (m_INSum_Check[8].GetCheck())
		{
			pDC->SetTextColor(BLUE);
		}
		else
		{
			pDC->SetTextColor(DARK_GRAY);
		}
	}
	//WF & CT Done

	return hbr;
}
//***************************************************************************************
//


//m_AutoScanDiameterEdit.EnableWindow(FALSE);
//m_CapColorImgAutoText.EnableWindow(FALSE);
//m_CapColorImgManuText.EnableWindow(FALSE);




void CSettingsComboDlg::OnLButtonDown(uint nFlags, CPoint Point)
{
	int whichCheck = -1;
	CRect rect;
	CPoint point, OriPoint;

	GetCursorPos(&point);

	//Plus cylinder
	OriPoint = point;
	m_PosCylText.ScreenToClient(&OriPoint);
	::GetClientRect(m_PosCylText.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_PosCylCheck.SetCheck(!m_PosCylCheck.GetCheck());
		OnInvalidClicked();
		return;
	}

	//Tracey refraction round 8		
	OriPoint = point;
	m_Round8Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_Round8Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		if (m_Round8Radio.GetCheck()) return;

		m_Round8Radio.SetCheck(!m_Round8Radio.GetCheck());
		m_Round100Radio.SetCheck(!m_Round100Radio.GetCheck());
		OnInvalidClicked();
		return;
	}

	//Tracey refraction round 100		
	OriPoint = point;
	m_Round100Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_Round100Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		if (m_Round100Radio.GetCheck()) return;

		m_Round8Radio.SetCheck(!m_Round8Radio.GetCheck());
		m_Round100Radio.SetCheck(!m_Round100Radio.GetCheck());
		OnInvalidClicked();
		return;
	}

	//Enable autofixation
	if (!m_isComboViewer)
	{
		OriPoint = point;
		m_ProbingEnabledText.ScreenToClient(&OriPoint);
		::GetClientRect(m_ProbingEnabledText.GetSafeHwnd(), &rect);
		rect.left -= 5;
		rect.right += 3;

		if (rect.PtInRect(OriPoint))
		{
			m_ProbingEnabledCheck.SetCheck(!m_ProbingEnabledCheck.GetCheck());
			OnProbingButtonClicked();
			return;
		}
	}

	//Enable fogging
	if (!m_isComboViewer)
	{
		OriPoint = point;
		m_FoggingEnabledText.ScreenToClient(&OriPoint);
		::GetClientRect(m_FoggingEnabledText.GetSafeHwnd(), &rect);
		rect.left -= 5;
		rect.right += 3;

		if (rect.PtInRect(OriPoint) && m_ProbingEnabledCheck.GetCheck())
		{
			m_FoggingEnabledCheck.SetCheck(!m_FoggingEnabledCheck.GetCheck());
			OnInvalidClicked();
			return;
		}
	}

	//Fixation light on...
	if (!m_isComboViewer)
	{
		OriPoint = point;
		m_AccommTargetLightOnText.ScreenToClient(&OriPoint);
		::GetClientRect(m_AccommTargetLightOnText.GetSafeHwnd(), &rect);
		rect.left -= 5;
		rect.right += 3;

		if (rect.PtInRect(OriPoint))
		{
			m_AccommTargetLightOnCheck.SetCheck(!m_AccommTargetLightOnCheck.GetCheck());
			OnTargetLightBtnClicked();
			return;
		}
	}

	//3X averaged Exam
	if (!m_isComboViewer)
	{
		OriPoint = point;
		m_Super_ExamText.ScreenToClient(&OriPoint);
		::GetClientRect(m_Super_ExamText.GetSafeHwnd(), &rect);
		rect.left -= 5;
		rect.right += 3;

		if (rect.PtInRect(OriPoint))
		{
			m_Super_ExamCheck.SetCheck(!m_Super_ExamCheck.GetCheck());
			OnSuperExamBtnClicked();
			return;
		}
	}

	//Pupil alignment
	if (!m_isComboViewer)
	{
		OriPoint = point;
		m_AlignmentPupilText.ScreenToClient(&OriPoint);
		::GetClientRect(m_AlignmentPupilText.GetSafeHwnd(), &rect);
		rect.left -= 5;
		rect.right += 3;

		if (rect.PtInRect(OriPoint) && !m_Super_ExamCheck.GetCheck())
		{
			if (m_AlignmentPupilRadio.GetCheck()) return;

			m_AlignmentPupilRadio.SetCheck(!m_AlignmentPupilRadio.GetCheck());
			m_Alignment4IrdotsRadio.SetCheck(!m_Alignment4IrdotsRadio.GetCheck());
			OnInvalidClicked();
			return;
		}
	}

	//4 IR dots alignment
	if (!m_isComboViewer)
	{
		OriPoint = point;
		m_Alignment4IrdotsText.ScreenToClient(&OriPoint);
		::GetClientRect(m_Alignment4IrdotsText.GetSafeHwnd(), &rect);
		rect.left -= 5;
		rect.right += 3;

		if (rect.PtInRect(OriPoint) && !m_Super_ExamCheck.GetCheck())
		{
			if (m_Alignment4IrdotsRadio.GetCheck()) return;

			m_AlignmentPupilRadio.SetCheck(!m_AlignmentPupilRadio.GetCheck());
			m_Alignment4IrdotsRadio.SetCheck(!m_Alignment4IrdotsRadio.GetCheck());
			OnInvalidClicked();
			return;
		}
	}

	//Show animated scanning...	
	if (!m_isComboViewer)
	{
		OriPoint = point;
		m_AnimateAfterNewExamText.ScreenToClient(&OriPoint);
		::GetClientRect(m_AnimateAfterNewExamText.GetSafeHwnd(), &rect);
		rect.left -= 5;
		rect.right += 3;

		if (rect.PtInRect(OriPoint))
		{
			m_AnimateAfterNewExamCheck.SetCheck(!m_AnimateAfterNewExamCheck.GetCheck());
			OnInvalidClicked();
			return;
		}
	}

	//Date fromat
	OriPoint = point;
	m_MMDDYYYYText.ScreenToClient(&OriPoint);
	::GetClientRect(m_MMDDYYYYText.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		if (m_MMDDYYYYRadio.GetCheck()) return;

		m_MMDDYYYYRadio.SetCheck(TRUE);
		m_DDMMYYYYRadio.SetCheck(FALSE);
		m_YYYYMMDDRadio.SetCheck(FALSE);
		OnInvalidClicked();
		return;
	}

	//Date fromat
	OriPoint = point;
	m_DDMMYYYYText.ScreenToClient(&OriPoint);
	::GetClientRect(m_DDMMYYYYText.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		if (m_DDMMYYYYRadio.GetCheck()) return;

		m_DDMMYYYYRadio.SetCheck(TRUE);
		m_MMDDYYYYRadio.SetCheck(FALSE);
		m_YYYYMMDDRadio.SetCheck(FALSE);
		OnInvalidClicked();
		return;
	}

	//Date fromat
	OriPoint = point;
	m_YYYYMMDDText.ScreenToClient(&OriPoint);
	::GetClientRect(m_DDMMYYYYText.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		if (m_YYYYMMDDRadio.GetCheck()) return;

		m_YYYYMMDDRadio.SetCheck(TRUE);
		m_MMDDYYYYRadio.SetCheck(FALSE);
		m_DDMMYYYYRadio.SetCheck(FALSE);
		OnInvalidClicked();
		return;
	}

	//Show Refractive K	
	OriPoint = point;
	m_SimkText.ScreenToClient(&OriPoint);
	::GetClientRect(m_SimkText.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		if (m_SimkRadio.GetCheck()) return;

		m_SimkRadio.SetCheck(TRUE);
		m_RefkRadio.SetCheck(FALSE);
		OnInvalidClicked();
		return;
	}

	//Show sim K
	OriPoint = point;
	m_RefkText.ScreenToClient(&OriPoint);
	::GetClientRect(m_RefkText.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		if (m_RefkRadio.GetCheck()) return;

		m_RefkRadio.SetCheck(TRUE);
		m_SimkRadio.SetCheck(FALSE);
		OnInvalidClicked();
		return;
	}

	//Enable Wavetouch functionality
	OriPoint = point;
	m_WavetouchText.ScreenToClient(&OriPoint);
	::GetClientRect(m_WavetouchText.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_WavetouchCheck.SetCheck(!m_WavetouchCheck.GetCheck());
		OnInvalidClicked();
		return;
	}

	//Show 'Send AR/KR' button
	OriPoint = point;
	m_ShowSendARKRText.ScreenToClient(&OriPoint);
	::GetClientRect(m_ShowSendARKRText.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_ShowSendARKRCheck.SetCheck(!m_ShowSendARKRCheck.GetCheck());
		OnInvalidClicked();
		return;
	}

	//Capture color image
	if (!m_isComboViewer)
	{
		OriPoint = point;
		m_CapColorImgText.ScreenToClient(&OriPoint);
		::GetClientRect(m_CapColorImgText.GetSafeHwnd(), &rect);
		rect.left -= 5;
		rect.right += 3;

		if (rect.PtInRect(OriPoint))
		{
			m_CapColorImgCheck.SetCheck(!m_CapColorImgCheck.GetCheck());
			OnColImgButtonClicked();
			return;
		}
	}

	//Capture color image Automatically
	if (!m_isComboViewer)
	{
		OriPoint = point;
		m_CapColorImgAutoText.ScreenToClient(&OriPoint);
		::GetClientRect(m_CapColorImgAutoText.GetSafeHwnd(), &rect);
		rect.left -= 5;
		rect.right += 3;

		if (rect.PtInRect(OriPoint))
		{
			if (m_CapColorImgAutoRadio.GetCheck()) return;

			m_CapColorImgAutoRadio.SetCheck(TRUE);
			m_CapColorImgManuRadio.SetCheck(FALSE);
			OnColImgAutoManRadio1Clicked();
			return;
		}

		//Capture color image Manually
		OriPoint = point;
		m_CapColorImgManuText.ScreenToClient(&OriPoint);
		::GetClientRect(m_CapColorImgManuText.GetSafeHwnd(), &rect);
		rect.left -= 5;
		rect.right += 3;

		if (rect.PtInRect(OriPoint))
		{
			if (m_CapColorImgManuRadio.GetCheck()) return;

			m_CapColorImgManuRadio.SetCheck(TRUE);
			m_CapColorImgAutoRadio.SetCheck(FALSE);
			OnColImgAutoManRadio2Clicked();
			return;
		}
	}

	//Use JPG format
	OriPoint = point;
	m_UseJPGFormatText.ScreenToClient(&OriPoint);
	::GetClientRect(m_UseJPGFormatText.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		if (m_UseJPGFormatRadio.GetCheck()) return;

		m_UseJPGFormatRadio.SetCheck(TRUE);
		m_UsePNGFormatRadio.SetCheck(FALSE);
		OnInvalidClicked();
		return;
	}

	//Use JPG format
	OriPoint = point;
	m_UsePNGFormatText.ScreenToClient(&OriPoint);
	::GetClientRect(m_UsePNGFormatText.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		if (m_UsePNGFormatRadio.GetCheck()) return;

		m_UsePNGFormatRadio.SetCheck(TRUE);
		m_UseJPGFormatRadio.SetCheck(FALSE);
		OnInvalidClicked();
		return;
	}

	//Include LADs data
	OriPoint = point;
	m_IncludeLADsText.ScreenToClient(&OriPoint);
	::GetClientRect(m_IncludeLADsText.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_IncludeLADsCheck.SetCheck(!m_IncludeLADsCheck.GetCheck());
		OnInvalidClicked();
		return;
	}

	//Automatic Excel record
	OriPoint = point;
	m_ExcelExportText.ScreenToClient(&OriPoint);
	::GetClientRect(m_ExcelExportText.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_ExcelExportCheck.SetCheck(!m_ExcelExportCheck.GetCheck());
		OnExcelCheckClicked();
		return;
	}

	//Task Shortcut
	OriPoint = point;
	m_Task_ShortCutText.ScreenToClient(&OriPoint);
	::GetClientRect(m_Task_ShortCutText.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_Task_ShortCutCheck.SetCheck(!m_Task_ShortCutCheck.GetCheck());
		OnTaskShowBtnClicked();
		return;
	}

	// WF summary screens --- Points
	OriPoint = point;
	m_WFVrfText.ScreenToClient(&OriPoint);
	::GetClientRect(m_WFVrfText.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		if (m_WFVrfRadio.GetCheck()) return;

		m_WFVrfRadio.SetCheck(TRUE);

		m_WFVfaRadio.SetCheck(FALSE);
		m_WFCmpRadio.SetCheck(FALSE);
		m_WFSum1Radio.SetCheck(FALSE);
		m_WFSum2Radio.SetCheck(FALSE);
		m_WFSum3Radio.SetCheck(FALSE);
		m_WFSum4Radio.SetCheck(FALSE);
		m_WFSum5Radio.SetCheck(FALSE);
		m_WFSum6Radio.SetCheck(FALSE);
		m_WFNearVisionRadio.SetCheck(FALSE);//6.2.0
		return;
	}

	// WF summary screens --- vfa
	OriPoint = point;
	m_WFVfaText.ScreenToClient(&OriPoint);
	::GetClientRect(m_WFVfaText.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint) && m_WFSum_Check[1].GetCheck())
	{
		if (m_WFVfaRadio.GetCheck()) return;

		m_WFVfaRadio.SetCheck(TRUE);

		m_WFVrfRadio.SetCheck(FALSE);
		m_WFCmpRadio.SetCheck(FALSE);
		m_WFSum1Radio.SetCheck(FALSE);
		m_WFSum2Radio.SetCheck(FALSE);
		m_WFSum3Radio.SetCheck(FALSE);
		m_WFSum4Radio.SetCheck(FALSE);
		m_WFSum5Radio.SetCheck(FALSE);
		m_WFSum6Radio.SetCheck(FALSE);
		m_WFNearVisionRadio.SetCheck(FALSE);//6.2.0
		return;
	}

	// WF summary screens --- Compare
	OriPoint = point;
	m_WFCmpText.ScreenToClient(&OriPoint);
	::GetClientRect(m_WFCmpText.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint) && m_WFSum_Check[2].GetCheck())
	{
		if (m_WFCmpRadio.GetCheck()) return;

		m_WFCmpRadio.SetCheck(TRUE);

		m_WFVrfRadio.SetCheck(FALSE);
		m_WFVfaRadio.SetCheck(FALSE);
		m_WFSum1Radio.SetCheck(FALSE);
		m_WFSum2Radio.SetCheck(FALSE);
		m_WFSum3Radio.SetCheck(FALSE);
		m_WFSum4Radio.SetCheck(FALSE);
		m_WFSum5Radio.SetCheck(FALSE);
		m_WFSum6Radio.SetCheck(FALSE);
		m_WFNearVisionRadio.SetCheck(FALSE);//6.2.0
		return;
	}

	//6.2.0 WF Near Vision screens --- Points
	OriPoint = point;
	m_WFNearVisonText.ScreenToClient(&OriPoint);
	::GetClientRect(m_WFNearVisonText.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint) && m_WFSum_Check[9].GetCheck())
	{
		if (m_WFNearVisionRadio.GetCheck()) return;

		m_WFNearVisionRadio.SetCheck(TRUE);//6.2.0

		m_WFVrfRadio.SetCheck(FALSE);
		m_WFVfaRadio.SetCheck(FALSE);
		m_WFCmpRadio.SetCheck(FALSE);
		m_WFSum1Radio.SetCheck(FALSE);
		m_WFSum2Radio.SetCheck(FALSE);
		m_WFSum3Radio.SetCheck(FALSE);
		m_WFSum4Radio.SetCheck(FALSE);
		m_WFSum5Radio.SetCheck(FALSE);
		m_WFSum6Radio.SetCheck(FALSE);

		return;
	}

	// CT summary screens --- Rings
	OriPoint = point;
	m_CTVrfText.ScreenToClient(&OriPoint);
	::GetClientRect(m_CTVrfText.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		if (m_CTVrfRadio.GetCheck()) return;

		m_CTVrfRadio.SetCheck(TRUE);

		m_CTDblRadio.SetCheck(FALSE);
		m_CTCmpRadio.SetCheck(FALSE);
		m_CTSum1Radio.SetCheck(FALSE);
		m_CTSum2Radio.SetCheck(FALSE);
		m_CTSum3Radio.SetCheck(FALSE);
		m_CTSum4Radio.SetCheck(FALSE);
		m_CTSum5Radio.SetCheck(FALSE);
		return;
	}

	// CT summary screens --- OD / OS
	OriPoint = point;
	m_CTDblText.ScreenToClient(&OriPoint);
	::GetClientRect(m_CTDblText.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint) && m_CTSum_Check[1].GetCheck())
	{
		if (m_CTDblRadio.GetCheck()) return;

		m_CTDblRadio.SetCheck(TRUE);

		m_CTVrfRadio.SetCheck(FALSE);
		m_CTCmpRadio.SetCheck(FALSE);
		m_CTSum1Radio.SetCheck(FALSE);
		m_CTSum2Radio.SetCheck(FALSE);
		m_CTSum3Radio.SetCheck(FALSE);
		m_CTSum4Radio.SetCheck(FALSE);
		m_CTSum5Radio.SetCheck(FALSE);
		return;
	}

	// CT summary screens --- Compare
	OriPoint = point;
	m_CTCmpText.ScreenToClient(&OriPoint);
	::GetClientRect(m_CTCmpText.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint) && m_CTSum_Check[2].GetCheck())
	{
		if (m_CTCmpRadio.GetCheck()) return;

		m_CTCmpRadio.SetCheck(TRUE);

		m_CTVrfRadio.SetCheck(FALSE);
		m_CTDblRadio.SetCheck(FALSE);
		m_CTSum1Radio.SetCheck(FALSE);
		m_CTSum2Radio.SetCheck(FALSE);
		m_CTSum3Radio.SetCheck(FALSE);
		m_CTSum4Radio.SetCheck(FALSE);
		m_CTSum5Radio.SetCheck(FALSE);
		return;
	}

	// WF & CT summary screens --- OU Overview
	OriPoint = point;
	m_INSum4Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_INSum4Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint) && m_INSum_Check[3].GetCheck())
	{
		if (m_INSum4Radio.GetCheck()) return;

		m_INSum4Radio.SetCheck(TRUE);

		m_INSum1Radio.SetCheck(FALSE);
		m_INSum2Radio.SetCheck(FALSE);
		m_INSum3Radio.SetCheck(FALSE);
		m_INSum5Radio.SetCheck(FALSE);
		m_INSum6Radio.SetCheck(FALSE);
		m_INSum7Radio.SetCheck(FALSE);
		m_INSum8Radio.SetCheck(FALSE);
		m_INSum9Radio.SetCheck(FALSE);
		m_INSum10Radio.SetCheck(FALSE);
		m_INSum11Radio.SetCheck(FALSE);

		return;
	}

	// WF & CT summary screens --- IOL Selection
	OriPoint = point;
	m_INSum5Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_INSum5Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint) && m_INSum_Check[4].GetCheck())
	{
		if (m_INSum5Radio.GetCheck()) return;

		m_INSum5Radio.SetCheck(TRUE);

		m_INSum1Radio.SetCheck(FALSE);
		m_INSum2Radio.SetCheck(FALSE);
		m_INSum3Radio.SetCheck(FALSE);
		m_INSum4Radio.SetCheck(FALSE);
		m_INSum6Radio.SetCheck(FALSE);
		m_INSum7Radio.SetCheck(FALSE);
		m_INSum8Radio.SetCheck(FALSE);
		m_INSum9Radio.SetCheck(FALSE);
		m_INSum10Radio.SetCheck(FALSE);
		m_INSum11Radio.SetCheck(FALSE);

		return;
	}

	// WF & CT summary screens --- Dysfunctional Lens - MD
	OriPoint = point;
	m_INSum6Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_INSum6Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint) && m_INSum_Check[5].GetCheck())
	{
		if (m_INSum6Radio.GetCheck()) return;

		m_INSum6Radio.SetCheck(TRUE);

		m_INSum1Radio.SetCheck(FALSE);
		m_INSum2Radio.SetCheck(FALSE);
		m_INSum3Radio.SetCheck(FALSE);
		m_INSum4Radio.SetCheck(FALSE);
		m_INSum5Radio.SetCheck(FALSE);
		m_INSum7Radio.SetCheck(FALSE);
		m_INSum8Radio.SetCheck(FALSE);
		m_INSum9Radio.SetCheck(FALSE);
		m_INSum10Radio.SetCheck(FALSE);
		m_INSum11Radio.SetCheck(FALSE);

		return;
	}

	// WF & CT summary screens --- Dysfunctional Lens - PT
	OriPoint = point;
	m_INSum7Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_INSum7Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint) && m_INSum_Check[6].GetCheck())
	{
		if (m_INSum7Radio.GetCheck()) return;

		m_INSum7Radio.SetCheck(TRUE);

		m_INSum1Radio.SetCheck(FALSE);
		m_INSum2Radio.SetCheck(FALSE);
		m_INSum3Radio.SetCheck(FALSE);
		m_INSum4Radio.SetCheck(FALSE);
		m_INSum5Radio.SetCheck(FALSE);
		m_INSum6Radio.SetCheck(FALSE);
		m_INSum8Radio.SetCheck(FALSE);
		m_INSum9Radio.SetCheck(FALSE);
		m_INSum10Radio.SetCheck(FALSE);
		m_INSum11Radio.SetCheck(FALSE);

		return;
	}

	// WF & CT summary screens --- Toric Alignment
	OriPoint = point;
	m_INSum11Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_INSum11Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint) && m_INSum_Check[10].GetCheck())
	{
		if (m_INSum11Radio.GetCheck()) return;

		m_INSum11Radio.SetCheck(TRUE);

		m_INSum1Radio.SetCheck(FALSE);
		m_INSum2Radio.SetCheck(FALSE);
		m_INSum3Radio.SetCheck(FALSE);
		m_INSum4Radio.SetCheck(FALSE);
		m_INSum5Radio.SetCheck(FALSE);
		m_INSum6Radio.SetCheck(FALSE);
		m_INSum8Radio.SetCheck(FALSE);
		m_INSum9Radio.SetCheck(FALSE);
		m_INSum10Radio.SetCheck(FALSE);
		m_INSum7Radio.SetCheck(FALSE);

		return;
	}
}

//***************************************************************************************