
//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "MainWnd.h"
#include "PatientDlg.h"
#include "PatientsMergeDlg.h"
#include "SettingsDlg.h"
#include "SettingsComboDlg.h"
#include "SettingsSoloDlg.h"
#include "StatusDlg.h"
#include "CalDlg.h"
#include "DatabaseDlg.h"
#include "ExamDlg.h"
#include "SelectExamDlg.h"
#include "SelectTwoExamDlg.h"
#include "WFAcquisitionDlg.h"
#include "CTAcquisitionDlg.h"
#include "TestResultDlg.h"
#include "PasswordDlg.h"
#include "FolderDlg.h"
#include "PreopDataDlg.h"
#include "PreopComboDataDlg.h"
#include "WFTest.h"
#include "Data.h"
#include "Hardware.h"
#include "Settings.h"
#include "BusyCursor.h"
#include "Licensing.h"
#include "Export.h"
#include "Registry.h"
#include "XPControl.h"
#include "RegtimeDlg.h"
#include "CTColorImgWnd.h"
#include "ExcelSettingsDlg2.h"
#include "Excel9.h"                //For Excel
#include "ExamAveDlg.h"
#include "WFDOFCWnd.h"             //Depth of Focus Comparison
#include "AlignmentDlg.h"          //A simple ctrl+w window use for adjust hardware (Tracey technician tool)
#include "WFCTTorAliWnd.h"         //WF/CT toric alignment window
#include "WFCTAngleSumWnd.h"       //WF/CT Angle k/a window (transformed from WF display)
#include "GlobalFunctions.h"
#include "TraceyDicom.h"           //Dicom Toolkit
#include "SameDicomPatientDlg.h"   //Dicom imported patient comparison
#include "DicomSelTypeDlg.h"
#include "DicomMWLRecDlg.h"
#include "DicomMWLResDlg.h"
#include "INIParser.h"             //Ini registration
#include "WFNearVisionSumWnd.h"    //6.2.0 One of WF Display to show the near and far vision comparison

#include <fstream>
#include "AppVersion.h"
#include "SplashScreen.h"
#include "iTraceApp.h"
#include "Cmd.Target.h"
#include "CmdLineArgs.h"

using namespace std;

static CRect g_summaryRect;

CRect GetSummaryRect()
{
  // #todo: Move to 'Layout'
  auto rc  = g_summaryRect;
  rc.right = rc.left + rc.Width() & ~3;
  return rc;
}


//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CMainWnd, CWnd)

	ON_WM_TIMER()

	ON_WM_PAINT()
	ON_WM_DEVICECHANGE()

	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()

	ON_BN_CLICKED(IDC_REGI_TIME_BUTTON, OnRegButtonClicked)
	ON_BN_CLICKED(IDC_SEARCH_DEFAULT_BTN, OnEraseSearchButtonClicked)//Erase search
	ON_BN_CLICKED(IDC_REFRESH_DATA_BTN, OnRefreshDataButtonClicked)//Refresh patients lit
	ON_BN_CLICKED(IDC_DATABASE_BTN, OnDatabaseButtonClicked)//Refresh patients lit
	//ON_BN_CLICKED(IDC_DATABASE_TEST_BTN, OnDatabaseTestButtonClicked)//Database test
	ON_BN_CLICKED(IDC_NEW_PATIENT_BUTTON, OnNewPatientButtonClicked)
	ON_BN_CLICKED(IDC_SELECT_PATIENT_BUTTON, OnSelectPatientButtonClicked)//Patient selection
	ON_COMMAND(IDK_ENTER, OnSelectPatientButtonClicked)
	ON_BN_CLICKED(IDC_EDIT_PATIENT_BUTTON, OnEditPatientButtonClicked)
	ON_BN_CLICKED(IDC_DELETE_PATIENT_BUTTON, OnDeletePatientButtonClicked)
	ON_BN_CLICKED(IDC_IMPORT_PATIENT_BUTTON, OnImportPatientButtonClicked)
	ON_BN_CLICKED(IDC_EXPORT_PATIENT_BUTTON, OnExportPatientButtonClicked)
	ON_BN_CLICKED(IDC_EXPORT_EXCEL_BUTTON, OnExcelButtonClicked)//Excel export
	ON_BN_CLICKED(IDC_TEST_BUTTON, OnTestButtonClicked)
	ON_BN_CLICKED(IDC_SETTINGS_BUTTON, OnSettingsButtonClicked)//Setting Dlg
	ON_BN_CLICKED(IDC_LICENSE_BUTTON, OnLicenseButtonClicked)
	ON_BN_CLICKED(IDC_BACK_BUTTON, OnBackButtonClicked)// Back button
	ON_COMMAND(IDK_ESCAPE, OnBackButtonClicked)
	ON_COMMAND(IDK_CTRL_D, OnCtrlD)
	ON_COMMAND(IDK_CTRL_W, OnCtrlW)
	ON_COMMAND(IDK_CTRL_A, OnCtrlA)//Select all patients
	ON_COMMAND(IDK_CTRL_Z, OnCtrlZ)//Last Implement (No functionality yet)
	ON_COMMAND(IDK_CTRL_Y, OnCtrlY)//Next Implement (No functionality yet)

	ON_EN_CHANGE(IDC_CUSTOM_ID_EDIT, UpdatePatientsList)
	ON_EN_CHANGE(IDC_LAST_NAME_EDIT, UpdatePatientsList)
	ON_EN_CHANGE(IDC_FIRST_NAME_EDIT, UpdatePatientsList)
	ON_EN_CHANGE(IDC_MID_NAME_EDIT, UpdatePatientsList)
	ON_CBN_SELCHANGE(IDC_GROUPS_COMBOBOX, UpdatePatientsList)
	ON_EN_CHANGE(IDC_DOB_EDIT, UpdatePatientsList)
	ON_CBN_SELCHANGE(IDC_SEX_COMBOBOX, UpdatePatientsList)
	ON_NOTIFY(NM_DBLCLK, IDC_PATIENTS_LIST, OnPatientsListDblClicked)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_PATIENTS_LIST, OnPatientsListColumnClicked)
	ON_NOTIFY(LVN_HOTTRACK, IDC_PATIENTS_LIST, OnPatientListHOVER)//when mouse hover on the patient list

	//Exam
	ON_BN_CLICKED(IDC_NEW_WF_EXAM_BUTTON, OnNewWFExamButtonClicked)
	ON_BN_CLICKED(IDC_NEW_CT_EXAM_BUTTON, OnNewCTExamButtonClicked)
	ON_BN_CLICKED(IDC_NEW_WFCT_EXAM_BUTTON, OnNewWFCTExamButtonClicked)
	ON_COMMAND(IDK_CTRL_L, OnCtrlL)
	ON_COMMAND(IDK_CTRL_M, OnCtrlM)
	ON_COMMAND(IDK_CTRL_N, OnCtrlN)
	ON_BN_CLICKED(IDC_VIEW_EXAM_BUTTON, OnViewExamButtonClicked)//Select the exam
	ON_BN_CLICKED(IDC_EDIT_EXAM_BUTTON, OnEditExamButtonClicked)
	ON_BN_CLICKED(IDC_DELETE_EXAM_BUTTON, OnDeleteExamButtonClicked)
	ON_BN_CLICKED(IDC_TOPCON_BUTTON, OnTopconButtonClicked)
	//Exam Done

	//Task buttons click
	ON_BN_CLICKED(IDC_VIEW_EXAM_0_BUTTON, OnViewExam0BtnClicked)
	ON_BN_CLICKED(IDC_VIEW_EXAM_1_BUTTON, OnViewExam1BtnClicked)
	ON_BN_CLICKED(IDC_VIEW_EXAM_2_BUTTON, OnViewExam2BtnClicked)
	ON_BN_CLICKED(IDC_VIEW_EXAM_3_BUTTON, OnViewExam3BtnClicked)
	ON_BN_CLICKED(IDC_VIEW_EXAM_4_BUTTON, OnViewExam4BtnClicked)
	//Task buttons click Done

	ON_NOTIFY(NM_DBLCLK, IDC_WF_OD_EXAMS_LIST, OnExamsListDblClicked)
	ON_NOTIFY(NM_DBLCLK, IDC_WF_OS_EXAMS_LIST, OnExamsListDblClicked)
	ON_NOTIFY(NM_DBLCLK, IDC_CT_OD_EXAMS_LIST, OnExamsListDblClicked)
	ON_NOTIFY(NM_DBLCLK, IDC_CT_OS_EXAMS_LIST, OnExamsListDblClicked)
	ON_NOTIFY(NM_CLICK, IDC_WF_OD_EXAMS_LIST, OnExamsListClicked)
	ON_NOTIFY(NM_CLICK, IDC_WF_OS_EXAMS_LIST, OnExamsListClicked)
	ON_NOTIFY(NM_CLICK, IDC_CT_OD_EXAMS_LIST, OnExamsListClicked)
	ON_NOTIFY(NM_CLICK, IDC_CT_OS_EXAMS_LIST, OnExamsListClicked)

	ON_NOTIFY(NM_RCLICK, IDC_WF_OD_EXAMS_LIST, OnExamsListRClicked)
	ON_NOTIFY(NM_RCLICK, IDC_WF_OS_EXAMS_LIST, OnExamsListRClicked)
	ON_NOTIFY(NM_RCLICK, IDC_CT_OD_EXAMS_LIST, OnExamsListRClicked)
	ON_NOTIFY(NM_RCLICK, IDC_CT_OS_EXAMS_LIST, OnExamsListRClicked)

	ON_NOTIFY(LVN_HOTTRACK, IDC_WF_OS_EXAMS_LIST, OnExamsListHOVER)
	ON_NOTIFY(LVN_HOTTRACK, IDC_WF_OD_EXAMS_LIST, OnExamsListHOVER)
	ON_NOTIFY(LVN_HOTTRACK, IDC_CT_OD_EXAMS_LIST, OnExamsListHOVER)
	ON_NOTIFY(LVN_HOTTRACK, IDC_CT_OS_EXAMS_LIST, OnExamsListHOVER)

	ON_NOTIFY(LVN_COLUMNCLICK, IDC_WF_OD_EXAMS_LIST, OnWFODExamListColumnClicked)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_WF_OS_EXAMS_LIST, OnWFOSExamListColumnClicked)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_CT_OD_EXAMS_LIST, OnCTODExamListColumnClicked)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_CT_OS_EXAMS_LIST, OnCTOSExamListColumnClicked)

	ON_BN_CLICKED(IDC_ACCEPT_EXAM_BUTTON, OnAcceptExamButtonClicked)
	ON_BN_CLICKED(IDC_REJECT_EXAM_BUTTON, OnRejectExamButtonClicked)

	//Summary buttons click
	ON_BN_CLICKED(IDC_TOGGLE_BUTTON, OnToggleButtonClicked)//Switch between the wf,ct, and wf/ct displays
	ON_BN_CLICKED(IDC_WF_VRF_BUTTON, OnWFVrfButtonClicked)
	ON_BN_CLICKED(IDC_WF_VFA_BUTTON, OnWFVfaButtonClicked)
	ON_BN_CLICKED(IDC_WF_CMP_BUTTON, OnWFCmpButtonClicked)
	ON_BN_CLICKED(IDC_WF_SUM_1_BUTTON, OnWFSum1ButtonClicked)
	ON_BN_CLICKED(IDC_WF_SUM_2_BUTTON, OnWFSum2ButtonClicked)
	ON_BN_CLICKED(IDC_WF_SUM_3_BUTTON, OnWFSum3ButtonClicked)
	ON_BN_CLICKED(IDC_WF_SUM_4_BUTTON, OnWFSum4ButtonClicked)
	ON_BN_CLICKED(IDC_WF_SUM_5_BUTTON, OnWFSum5ButtonClicked)
	ON_BN_CLICKED(IDC_WF_SUM_6_BUTTON, OnWFSum6ButtonClicked)
	ON_BN_CLICKED(IDC_WF_SUM_7_BUTTON, OnWFSum7ButtonClicked)//6.2.0 WF Near Vision
	ON_BN_CLICKED(IDC_CT_VRF_BUTTON, OnCTVrfButtonClicked)
	ON_BN_CLICKED(IDC_CT_DBL_BUTTON, OnCTDblButtonClicked)
	ON_BN_CLICKED(IDC_CT_CMP_BUTTON, OnCTCmpButtonClicked)
	ON_BN_CLICKED(IDC_CT_SUM_1_BUTTON, OnCTSum1ButtonClicked)
	ON_BN_CLICKED(IDC_CT_SUM_2_BUTTON, OnCTSum2ButtonClicked)
	ON_BN_CLICKED(IDC_CT_SUM_3_BUTTON, OnCTSum3ButtonClicked)
	ON_BN_CLICKED(IDC_CT_SUM_4_BUTTON, OnCTSum4ButtonClicked)
	ON_BN_CLICKED(IDC_CT_SUM_5_BUTTON, OnCTSum5ButtonClicked)
	ON_BN_CLICKED(IDC_IN_SUM_1_BUTTON, OnINSum1ButtonClicked)
	ON_BN_CLICKED(IDC_IN_SUM_2_BUTTON, OnINSum2ButtonClicked)
	ON_BN_CLICKED(IDC_IN_SUM_3_BUTTON, OnINSum3ButtonClicked)
	ON_BN_CLICKED(IDC_IN_SUM_4_BUTTON, OnINSum4ButtonClicked)
	ON_BN_CLICKED(IDC_IN_SUM_5_BUTTON, OnINSum5ButtonClicked)//ou overview
	ON_BN_CLICKED(IDC_IN_SUM_6_BUTTON, OnINSum6ButtonClicked)//IOL Selection
	ON_BN_CLICKED(IDC_IN_SUM_7_BUTTON, OnINSum7ButtonClicked)
	ON_BN_CLICKED(IDC_IN_SUM_8_BUTTON, OnINSum8ButtonClicked)
	ON_BN_CLICKED(IDC_IN_SUM_9_BUTTON, OnINSum9ButtonClicked)
	ON_BN_CLICKED(IDC_IN_SUM_10_BUTTON, OnINSum10ButtonClicked)//WFCT Angle k/a
	ON_BN_CLICKED(IDC_IN_SUM_11_BUTTON, OnINSum11ButtonClicked)//WFCT Angle k/a
	//Summary buttons click Done

	ON_BN_CLICKED(IDC_TASK_SUB0_BUTTON, OntTaskSubBtn0Clicked)
	ON_BN_CLICKED(IDC_TASK_SUB1_BUTTON, OntTaskSubBtn1Clicked)
	ON_BN_CLICKED(IDC_TASK_SUB2_BUTTON, OntTaskSubBtn2Clicked)
	ON_BN_CLICKED(IDC_TASK_SUB3_BUTTON, OntTaskSubBtn3Clicked)
	ON_BN_CLICKED(IDC_TASK_SUB4_BUTTON, OntTaskSubBtn4Clicked)
	ON_BN_CLICKED(IDC_TASK_SUB5_BUTTON, OntTaskSubBtn5Clicked)
	ON_BN_CLICKED(IDC_TASK_SUB6_BUTTON, OntTaskSubBtn6Clicked)
	ON_BN_CLICKED(IDC_TASK_SUB7_BUTTON, OntTaskSubBtn7Clicked)

	ON_BN_CLICKED(IDC_TORIC_PLANNER_BUTTON, OnToricPlannerButtonClicked)
	ON_BN_CLICKED(IDC_WAVETOUCH_BUTTON, OnWavetouchButtonClicked)
	ON_BN_CLICKED(IDC_EXPORT_ORDER_BUTTON, OnExportOrderButtonClicked)
	ON_BN_CLICKED(IDC_EXPORT_SCREEN_BUTTON, OnExportScreenButtonClicked)
	ON_BN_CLICKED(IDC_SAVE_SCREEN_BUTTON, OnSaveScreenButtonClicked)
	ON_BN_CLICKED(IDC_PRINT_SCREEN_BUTTON, OnPrintScreenButtonClicked)

	ON_COMMAND(IDK_F9, OnF9)

	ON_MESSAGE(WM_NOTIFY_LBTN_DBCLK, OnBigPlanWndShown)    //for showing a big planer wnd
	ON_MESSAGE(WM_NOTIFY_OSHER_ALG, OnCTOSHERALGWndShown) //for showing a big CT OSHER alignment wnd
	ON_MESSAGE(WM_NOTIFY_OSHER_ALG_BACK, OnCTOSHERALGWndHide)  //for showing a big planer wnd

	ON_MESSAGE(WM_SOLO_HYPELINK, OnSoloHypeLiner)      //for solo hypelinker

	ON_COMMAND(IDK_F1, OnF1)

	ON_COMMAND(IDK_CTRL_Q, OnCtrlQ)   //A simple ctrl+w window use for adjust hardware (Tracey technician tool)

END_MESSAGE_MAP()

//***************************************************************************************

Matrix<int>   m_RegYear;
Matrix<int>   m_RegMonth;
Matrix<int>   m_RegDay;
Matrix<int>   m_RegHour;
Matrix<int>   m_RegMinute;
CString       m_RegFromTime = "";//use to set the registration time range
CString       m_RegToTime = "";// use to set the registration time range

real_t          m_ScanSize[NUM_EXAMS_MAX];// Scan size of exam
int           m_NumImages[NUM_EXAMS_MAX];//image number of exam (if color imgage has been taken, this values is 2)

//***************************************************************************************

CMainWnd::CMainWnd()
{
	m_OpenTime = 0;
	m_Last_evel = LAST_LEVEL_MAIN;
	m_ImportID = GUID_NULL;

    RegisterCallbacks();

	for (int i = 0; i < NUM_EXAMS_MAX; i++)
	{
		m_pWFExams[i] = NULL;
		m_pCTExams[i] = NULL;
	}

	m_NumLoadedWFODExams = 0;
	m_NumLoadedWFOSExams = 0;
	m_NumLoadedCTODExams = 0;
	m_NumLoadedCTOSExams = 0;
	m_NumLoadedWFExams = 0;
	m_NumLoadedCTExams = 0;

	DeleteSelectedIndicesLists();

	// determine width and height
	int W = ::GetSystemMetrics(SM_CXSCREEN);
	int H = ::GetSystemMetrics(SM_CYSCREEN);
	m_q = W >= 1920 ? 2 : (W >= 1600 ? 1 : 0);
	RECT Rect;
	CWnd* pTasbarWnd = FindWindow(_T("Shell_traywnd"), NULL);
	if (pTasbarWnd)
	{
		pTasbarWnd->GetWindowRect(&Rect);
		int tw = Rect.right - Rect.left;
		int th = Rect.bottom - Rect.top;
		if (tw > th) H -= th;
		else W -= tw;
	}

  // Substract the minmum maximum menu bar height
  int MenuBarHeight = GetSystemMetrics(SM_CYMENU);
  H -= MenuBarHeight;
  H -= 10;
  // Substract the minmum maximum menu bar height

	CString ClassName = ::AfxRegisterWndClass(0, ::LoadCursor(NULL, IDC_ARROW), NULL, ::AfxGetApp()->LoadIcon(IDI_ITRACE));

  CmdLineArgs cla;
  if (cla.FullScreen()) {
	  CreateEx(0, ClassName, _T("iTrace"), WS_POPUP | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN, 0, 0, W, H, NULL, 0);
  }
  else {
    CRect rc{0, 0, W, H};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, TRUE);
    CreateEx(0, ClassName, _T("iTrace"), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, rc.left, rc.top, rc.Width(), rc.Height(), NULL, 0);
    CenterWindow();
  }


	m_hAccel = ::LoadAccelerators(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR));

	real_t d = 0.005 * H;// gap

	real_t SW = 0.915 * W;// summary window width

						//buttons
	real_t BH, BW[3], BL[3];
	BH = (H - 17 * d) / 16;
	BL[0] = 0.920 * W;
	BW[0] = 0.075 * W;
	BW[1] = BW[2] = (BW[0] - d) / 2 - 1;
	BL[1] = BL[0];
	BL[2] = BL[0] + BW[0] - BW[2];

	for (int c = 0; c < 3; c++)
	{
		for (int r = 0; r < 16; r++)
		{
			::SetRect(&m_BR[c][r], intRound(BL[c]), intRound(d + r * (BH + d)), intRound(BL[c] + BW[c]), intRound((r + 1) * (BH + d)));
		}
	}

	// buttons
	m_FontSize = intRound(0.02 * H);

	m_Font.Create(m_FontSize, 400, "Arial");

	// 1
	m_NewPatientButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][0], this, IDC_NEW_PATIENT_BUTTON);
	m_NewPatientButton.SetTip("New Patient");
	m_NewPatientButton.SetImage(m_q == 2 ? IDR_NEW_PATIENT_3 : m_q == 1 ? IDR_NEW_PATIENT_2 : IDR_NEW_PATIENT_1);

	m_NewWFCTExamButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][0], this, IDC_NEW_WFCT_EXAM_BUTTON);
	m_NewWFCTExamButton.SetTip("New WF/CT Exam");
	m_NewWFCTExamButton.SetImage(m_q == 2 ? IDR_NEW_IN_EXAM_3 : m_q == 1 ? IDR_NEW_IN_EXAM_2 : IDR_NEW_IN_EXAM_1);

	m_AcceptExamButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][0], this, IDC_ACCEPT_EXAM_BUTTON);
	m_AcceptExamButton.SetTip("Accept Exam");
	m_AcceptExamButton.SetImage(m_q == 2 ? IDR_ACCEPT_3 : m_q == 1 ? IDR_ACCEPT_2 : IDR_ACCEPT_1);

	// 2
	m_SelectPatientButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][1], this, IDC_SELECT_PATIENT_BUTTON);
	m_SelectPatientButton.SetTip("Select Patient");
	m_SelectPatientButton.SetImage(m_q == 2 ? IDR_RIGHT_ARROW_3 : m_q == 1 ? IDR_RIGHT_ARROW_2 : IDR_RIGHT_ARROW_1);

	m_NewWFExamButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][1], this, IDC_NEW_WF_EXAM_BUTTON);
	m_NewWFExamButton.SetTip("New WF Exam");
	m_NewWFExamButton.SetImage(m_q == 2 ? IDR_NEW_WF_EXAM_3 : m_q == 1 ? IDR_NEW_WF_EXAM_2 : IDR_NEW_WF_EXAM_1);

	m_RejectExamButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][1], this, IDC_REJECT_EXAM_BUTTON);
	m_RejectExamButton.SetTip("Reject Exam");
	m_RejectExamButton.SetImage(m_q == 2 ? IDR_REJECT_3 : m_q == 1 ? IDR_REJECT_2 : IDR_REJECT_1);

	// 3
	m_EditPatientButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][2], this, IDC_EDIT_PATIENT_BUTTON);
	m_EditPatientButton.SetTip("Edit Patient");
	m_EditPatientButton.SetImage(m_q == 2 ? IDR_EDIT_PATIENT_3 : m_q == 1 ? IDR_EDIT_PATIENT_2 : IDR_EDIT_PATIENT_1);

	m_NewCTExamButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][2], this, IDC_NEW_CT_EXAM_BUTTON);
	m_NewCTExamButton.SetTip("New CT Exam");
	m_NewCTExamButton.SetImage(m_q == 2 ? IDR_NEW_CT_EXAM_3 : m_q == 1 ? IDR_NEW_CT_EXAM_2 : IDR_NEW_CT_EXAM_1);

	// 4
	m_DeletePatientButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][3], this, IDC_DELETE_PATIENT_BUTTON);
	m_DeletePatientButton.SetTip("Delete Patients");
	m_DeletePatientButton.SetImage(m_q == 2 ? IDR_DELETE_PATIENT_3 : m_q == 1 ? IDR_DELETE_PATIENT_2 : IDR_DELETE_PATIENT_1);

	m_ViewExamButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][3], this, IDC_VIEW_EXAM_BUTTON);
	m_ViewExamButton.SetTip("View Exams");
	m_ViewExamButton.SetImage(m_q == 2 ? IDR_RIGHT_ARROW_3 : m_q == 1 ? IDR_RIGHT_ARROW_2 : IDR_RIGHT_ARROW_1);

	CString Task_Btn_Name = ::NewSettings.m_Tasks[0].ReadBtnName();//6.2.0 For Chinese

	int StartPos = 4;
	{
		m_ViewCustExamButton[0].Create(_T(""), _T(""), WS_CHILD, m_BR[0][StartPos], this, IDC_VIEW_EXAM_0_BUTTON);
		m_ViewCustExamButton[0].SetTip("View " + Task_Btn_Name);
		m_ViewCustExamButton[0].SetTextWidth(0.8);
		m_ViewCustExamButton[0].SetText(Task_Btn_Name);
		m_ViewCustExamButton[0].SetImagePos(0.1);
		m_ViewCustExamButton[0].SetImage(m_q == 2 ? IDR_RIGHT_S_ARROW_3 : m_q == 1 ? IDR_RIGHT_S_ARROW_2 : IDR_RIGHT_S_ARROW_1);
		StartPos++;
		m_CustExamCreated[0] = TRUE;
	}

	Task_Btn_Name = ::NewSettings.m_Tasks[1].ReadBtnName();//6.2.0 For Chinese
	{
		m_ViewCustExamButton[1].Create(_T(""), _T(""), WS_CHILD, m_BR[0][StartPos], this, IDC_VIEW_EXAM_1_BUTTON);
		m_ViewCustExamButton[1].SetTip("View " + Task_Btn_Name);
		m_ViewCustExamButton[1].SetTextWidth(0.8);
		m_ViewCustExamButton[1].SetText(Task_Btn_Name);
		m_ViewCustExamButton[1].SetImagePos(0.1);
		m_ViewCustExamButton[1].SetImage(m_q == 2 ? IDR_RIGHT_S_ARROW_3 : m_q == 1 ? IDR_RIGHT_S_ARROW_2 : IDR_RIGHT_S_ARROW_1);
		StartPos++;
		m_CustExamCreated[1] = TRUE;
	}

	Task_Btn_Name = ::NewSettings.m_Tasks[2].ReadBtnName();//6.2.0 For Chinese
	{
		m_ViewCustExamButton[2].Create(_T(""), _T(""), WS_CHILD, m_BR[0][StartPos], this, IDC_VIEW_EXAM_2_BUTTON);
		m_ViewCustExamButton[2].SetTip("View " + Task_Btn_Name);
		m_ViewCustExamButton[2].SetTextWidth(0.8);
		m_ViewCustExamButton[2].SetText(Task_Btn_Name);
		m_ViewCustExamButton[2].SetImagePos(0.1);
		m_ViewCustExamButton[2].SetImage(m_q == 2 ? IDR_RIGHT_S_ARROW_3 : m_q == 1 ? IDR_RIGHT_S_ARROW_2 : IDR_RIGHT_S_ARROW_1);
		StartPos++;
		m_CustExamCreated[2] = TRUE;
	}

	Task_Btn_Name = ::NewSettings.m_Tasks[3].ReadBtnName();//6.2.0 For Chinese
	{
		m_ViewCustExamButton[3].Create(_T(""), _T(""), WS_CHILD, m_BR[0][StartPos], this, IDC_VIEW_EXAM_3_BUTTON);
		m_ViewCustExamButton[3].SetTip("View " + Task_Btn_Name);
		m_ViewCustExamButton[3].SetTextWidth(0.8);

		m_ViewCustExamButton[3].SetText(Task_Btn_Name);
		m_ViewCustExamButton[3].SetImagePos(0.1);
		m_ViewCustExamButton[3].SetImage(m_q == 2 ? IDR_RIGHT_S_ARROW_3 : m_q == 1 ? IDR_RIGHT_S_ARROW_2 : IDR_RIGHT_S_ARROW_1);
		StartPos++;
		m_CustExamCreated[3] = TRUE;
	}

	Task_Btn_Name = ::NewSettings.m_Tasks[4].ReadBtnName();//6.2.0 For Chinese
	{
		m_ViewCustExamButton[4].Create(_T(""), _T(""), WS_CHILD, m_BR[0][StartPos], this, IDC_VIEW_EXAM_4_BUTTON);
		m_ViewCustExamButton[4].SetTip("View " + Task_Btn_Name);
		m_ViewCustExamButton[4].SetTextWidth(0.8);
		m_ViewCustExamButton[4].SetText(Task_Btn_Name);
		m_ViewCustExamButton[4].SetImagePos(0.1);
		m_ViewCustExamButton[4].SetImage(m_q == 2 ? IDR_RIGHT_S_ARROW_3 : m_q == 1 ? IDR_RIGHT_S_ARROW_2 : IDR_RIGHT_S_ARROW_1);
		StartPos++;
		m_CustExamCreated[4] = TRUE;
	}

	m_ToggleButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][3], this, IDC_TOGGLE_BUTTON);
	m_ToggleButton.SetTip("Toggle");
	//m_ToggleButton.SetImage(m_q == 2 ? IDR_TOGGLE_DISPLAYS_3 : m_q == 1 ? IDR_TOGGLE_DISPLAYS_2 : IDR_TOGGLE_DISPLAYS_1);

	// 5
	m_ImportPatientButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][4], this, IDC_IMPORT_PATIENT_BUTTON);
	m_ImportPatientButton.SetTip("Import Patients");
	m_ImportPatientButton.SetImage(m_q == 2 ? IDR_IMPORT_PATIENTS_3 : m_q == 1 ? IDR_IMPORT_PATIENTS_2 : IDR_IMPORT_PATIENTS_1);

	//0129 Dicom Export
	if (m_isComboDicom)
	{
		m_DicomExportButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][9], this, IDC_DICOMEXPORT_BUTTON);
		m_DicomExportButton.SetTip("Dicom Export");
    m_DicomExportButton.SetImage(m_q == 2 ? IDR_DICOM_EXPORT_3 : m_q == 1 ? IDR_DICOM_EXPORT_2 : IDR_DICOM_EXPORT_1);
	}
	//0129 Dicom Export

	if (m_isSolo || !::NewSettings.m_ShowTask) m_EditExamButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][4], this, IDC_EDIT_EXAM_BUTTON);
	else m_EditExamButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][9], this, IDC_EDIT_EXAM_BUTTON);//530 from m_BR[0][4] to m_BR[0][9]

	m_EditExamButton.SetTip("Edit Exam");
	m_EditExamButton.SetImage(m_q == 2 ? IDR_EDIT_EXAM_3 : m_q == 1 ? IDR_EDIT_EXAM_2 : IDR_EDIT_EXAM_1);

	m_WFVrfButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][4], this, IDC_WF_VRF_BUTTON);//530
	m_WFVrfButton.SetFont(m_FontSize);
	m_WFVrfButton.SetText("Points");

	m_CTVrfButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][4], this, IDC_CT_VRF_BUTTON);
	m_CTVrfButton.SetFont(m_FontSize);
	m_CTVrfButton.SetText("Rings");

	// 6
	m_ExportPatientButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][5], this, IDC_EXPORT_PATIENT_BUTTON);
	m_ExportPatientButton.SetTip("Export Patients");
	m_ExportPatientButton.SetImage(m_q == 2 ? IDR_EXPORT_PATIENTS_3 : m_q == 1 ? IDR_EXPORT_PATIENTS_2 : IDR_EXPORT_PATIENTS_1);

	if (m_isSolo || !::NewSettings.m_ShowTask) m_DeleteExamButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][5], this, IDC_DELETE_EXAM_BUTTON);
	else m_DeleteExamButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][10], this, IDC_DELETE_EXAM_BUTTON);

	m_DeleteExamButton.SetTip("Delete Exams");
	m_DeleteExamButton.SetImage(m_q == 2 ? IDR_DELETE_EXAM_3 : m_q == 1 ? IDR_DELETE_EXAM_2 : IDR_DELETE_EXAM_1);

	m_WFVfaButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][5], this, IDC_WF_VFA_BUTTON);
	m_WFVfaButton.SetFont(m_FontSize);
	m_WFVfaButton.SetText("VFA");

	m_CTDblButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][5], this, IDC_CT_DBL_BUTTON);
	m_CTDblButton.SetFont(m_FontSize);
	m_CTDblButton.SetText("OD / OS");

	// 7
	//export excel button
	m_ExportExcelButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][6], this, IDC_EXPORT_EXCEL_BUTTON);
	m_ExportExcelButton.SetTip("Excel Export");
	m_ExportExcelButton.SetImage(m_q == 2 ? IDR_EXCEL_EXPORT3 : m_q == 1 ? IDR_EXCEL_EXPORT2 : IDR_EXCEL_EXPORT1);

	if (m_isSolo) m_SettingsButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][6], this, IDC_SETTINGS_BUTTON);
	else m_SettingsButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][7], this, IDC_SETTINGS_BUTTON);
	m_SettingsButton.SetTip("Settings");
	m_SettingsButton.SetImage(m_q == 2 ? IDR_SETTINGS_3 : m_q == 1 ? IDR_SETTINGS_2 : IDR_SETTINGS_1);

	if (m_isSolo || !::NewSettings.m_ShowTask) m_TopconButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][6], this, IDC_TOPCON_BUTTON);
	else m_TopconButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][11], this, IDC_TOPCON_BUTTON);

	m_TopconButton.SetFont(m_FontSize);
	m_TopconButton.SetText("Send AR/KR");

	m_WFCmpButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][6], this, IDC_WF_CMP_BUTTON);
	m_WFCmpButton.SetFont(m_FontSize);
	m_WFCmpButton.SetText("Compare");

	m_CTCmpButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][6], this, IDC_CT_CMP_BUTTON);
	m_CTCmpButton.SetFont(m_FontSize);
	m_CTCmpButton.SetText("Compare");

	// 8
	if (m_isSolo) m_LicenseButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][7], this, IDC_LICENSE_BUTTON);
	else  m_LicenseButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][8], this, IDC_LICENSE_BUTTON);
	m_LicenseButton.SetTip("Software Activation Key");
	m_LicenseButton.SetImage(m_q == 2 ? IDR_LICENSE_3 : m_q == 1 ? IDR_LICENSE_2 : IDR_LICENSE_1);

	m_WFSumButton[0].Create(_T(""), _T(""), WS_CHILD, m_BR[0][7], this, IDC_WF_SUM_1_BUTTON);
	m_WFSumButton[0].SetFont(m_FontSize);

	m_CTSumButton[0].Create(_T(""), _T(""), WS_CHILD, m_BR[0][7], this, IDC_CT_SUM_1_BUTTON);
	m_CTSumButton[0].SetFont(m_FontSize);

	m_INSumButton[0].Create(_T(""), _T(""), WS_CHILD, m_BR[0][4], this, IDC_IN_SUM_1_BUTTON);
	m_INSumButton[0].SetFont(m_FontSize);

	// 9
	if (m_isSolo) m_TestButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][8], this, IDC_TEST_BUTTON);
	else m_TestButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][9], this, IDC_TEST_BUTTON);
	m_TestButton.SetTip("Test");
	m_TestButton.SetImage(m_q == 2 ? IDR_TEST_3 : m_q == 1 ? IDR_TEST_2 : IDR_TEST_1);

	m_WFSumButton[1].Create(_T(""), _T(""), WS_CHILD, m_BR[0][8], this, IDC_WF_SUM_2_BUTTON);
	m_WFSumButton[1].SetFont(m_FontSize);

	m_CTSumButton[1].Create(_T(""), _T(""), WS_CHILD, m_BR[0][8], this, IDC_CT_SUM_2_BUTTON);
	m_CTSumButton[1].SetFont(m_FontSize);

	m_INSumButton[1].Create(_T(""), _T(""), WS_CHILD, m_BR[0][5], this, IDC_IN_SUM_2_BUTTON);
	m_INSumButton[1].SetFont(m_FontSize);

	// 10
	m_WFSumButton[2].Create(_T(""), _T(""), WS_CHILD, m_BR[0][9], this, IDC_WF_SUM_3_BUTTON);
	m_WFSumButton[2].SetFont(m_FontSize);

	m_CTSumButton[2].Create(_T(""), _T(""), WS_CHILD, m_BR[0][9], this, IDC_CT_SUM_3_BUTTON);
	m_CTSumButton[2].SetFont(m_FontSize);

	if (m_isCombo)  m_INSumButton[2].Create(_T(""), _T(""), WS_CHILD, m_BR[0][5], this, IDC_IN_SUM_3_BUTTON);
	else         m_INSumButton[2].Create(_T(""), _T(""), WS_CHILD, m_BR[0][6], this, IDC_IN_SUM_3_BUTTON);
	m_INSumButton[2].SetFont(m_FontSize);

	// 11
	m_WFSumButton[3].Create(_T(""), _T(""), WS_CHILD, m_BR[0][10], this, IDC_WF_SUM_4_BUTTON);
	m_WFSumButton[3].SetFont(m_FontSize);

	m_CTSumButton[3].Create(_T(""), _T(""), WS_CHILD, m_BR[0][10], this, IDC_CT_SUM_4_BUTTON);
	m_CTSumButton[3].SetFont(m_FontSize);

	if (m_isCombo)
	{
		m_WFSumButton[4].Create(_T(""), _T(""), WS_CHILD, m_BR[0][10], this, IDC_WF_SUM_5_BUTTON);
		m_WFSumButton[4].SetFont(m_FontSize);

		m_WFSumButton[5].Create(_T(""), _T(""), WS_CHILD, m_BR[0][11], this, IDC_WF_SUM_6_BUTTON);
		m_WFSumButton[5].SetFont(m_FontSize);

		m_WFSumButton[6].Create(_T(""), _T(""), WS_CHILD, m_BR[0][11], this, IDC_WF_SUM_7_BUTTON);//6.2.0
		m_WFSumButton[6].SetFont(m_FontSize);

		m_CTSumButton[4].Create(_T(""), _T(""), WS_CHILD, m_BR[0][10], this, IDC_CT_SUM_5_BUTTON);
		m_CTSumButton[4].SetFont(m_FontSize);
	}

	if (m_isCombo)  m_INSumButton[3].Create(_T(""), _T(""), WS_CHILD, m_BR[0][6], this, IDC_IN_SUM_4_BUTTON);
	else         m_INSumButton[3].Create(_T(""), _T(""), WS_CHILD, m_BR[0][7], this, IDC_IN_SUM_4_BUTTON);
	m_INSumButton[3].SetFont(m_FontSize);

	if (m_isCombo)  m_INSumButton[4].Create(_T(""), _T(""), WS_CHILD, m_BR[0][7], this, IDC_IN_SUM_5_BUTTON);
	else         m_INSumButton[4].Create(_T(""), _T(""), WS_CHILD, m_BR[0][8], this, IDC_IN_SUM_5_BUTTON);
	m_INSumButton[4].SetFont(m_FontSize);

	if (m_isCombo)  m_INSumButton[5].Create(_T(""), _T(""), WS_CHILD, m_BR[0][8], this, IDC_IN_SUM_6_BUTTON);
	else         m_INSumButton[5].Create(_T(""), _T(""), WS_CHILD, m_BR[0][9], this, IDC_IN_SUM_6_BUTTON);
	m_INSumButton[5].SetFont(m_FontSize);

	if (m_isCombo)  m_INSumButton[6].Create(_T(""), _T(""), WS_CHILD, m_BR[0][9], this, IDC_IN_SUM_7_BUTTON);
	else         m_INSumButton[6].Create(_T(""), _T(""), WS_CHILD, m_BR[0][10], this, IDC_IN_SUM_7_BUTTON);
	m_INSumButton[6].SetFont(m_FontSize);

	if (m_isCombo)
	{
		m_INSumButton[7].Create(_T(""), _T(""), WS_CHILD, m_BR[0][10], this, IDC_IN_SUM_8_BUTTON);
		m_INSumButton[7].SetFont(m_FontSize);

		m_INSumButton[8].Create(_T(""), _T(""), WS_CHILD, m_BR[0][11], this, IDC_IN_SUM_9_BUTTON);
		m_INSumButton[8].SetFont(m_FontSize);

		//WFCT Angle K/A
		m_INSumButton[9].Create(_T(""), _T(""), WS_CHILD, m_BR[0][12], this, IDC_IN_SUM_10_BUTTON);
		m_INSumButton[9].SetFont(m_FontSize);
		//WFCT Angle K/A


		//WFCT Toric Alignment
		m_INSumButton[10].Create(_T(""), _T(""), WS_CHILD, m_BR[0][13], this, IDC_IN_SUM_11_BUTTON);
		m_INSumButton[10].SetFont(m_FontSize);
		//WFCT Toric Alignment
	}

	// 12
	m_ToricPlannerButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][11], this, IDC_TORIC_PLANNER_BUTTON);
	m_ToricPlannerButton.SetFont(m_FontSize);
	m_ToricPlannerButton.SetText("Toric Planner");
	m_ToricPlannerButton.SetEdgeColor(RED);
	// 13

	//task sub-button
	m_TaskButton[0].Create(_T(""), _T(""), WS_CHILD, m_BR[0][4], this, IDC_TASK_SUB0_BUTTON);
	m_TaskButton[0].SetFont(m_FontSize);

	m_TaskButton[1].Create(_T(""), _T(""), WS_CHILD, m_BR[0][5], this, IDC_TASK_SUB1_BUTTON);
	m_TaskButton[1].SetFont(m_FontSize);

	m_TaskButton[2].Create(_T(""), _T(""), WS_CHILD, m_BR[0][6], this, IDC_TASK_SUB2_BUTTON);
	m_TaskButton[2].SetFont(m_FontSize);

	m_TaskButton[3].Create(_T(""), _T(""), WS_CHILD, m_BR[0][7], this, IDC_TASK_SUB3_BUTTON);
	m_TaskButton[3].SetFont(m_FontSize);

	m_TaskButton[4].Create(_T(""), _T(""), WS_CHILD, m_BR[0][8], this, IDC_TASK_SUB4_BUTTON);
	m_TaskButton[4].SetFont(m_FontSize);

	m_TaskButton[5].Create(_T(""), _T(""), WS_CHILD, m_BR[0][9], this, IDC_TASK_SUB5_BUTTON);
	m_TaskButton[5].SetFont(m_FontSize);

	m_TaskButton[6].Create(_T(""), _T(""), WS_CHILD, m_BR[0][10], this, IDC_TASK_SUB6_BUTTON);
	m_TaskButton[6].SetFont(m_FontSize);

	m_TaskButton[7].Create(_T(""), _T(""), WS_CHILD, m_BR[0][11], this, IDC_TASK_SUB7_BUTTON);
	m_TaskButton[7].SetFont(m_FontSize);
	//task sub-button Done

	m_WavetouchButton.Create(_T(""), _T(""), WS_CHILD, m_BR[1][12], this, IDC_WAVETOUCH_BUTTON);
	m_WavetouchButton.SetTip("WaveTouch Display");
	m_WavetouchButton.SetImage(m_q == 2 ? IDR_WAVETOUCH_3 : m_q == 1 ? IDR_WAVETOUCH_2 : IDR_WAVETOUCH_1);

	m_ExportOrderButton.Create(_T(""), _T(""), WS_CHILD, m_BR[2][12], this, IDC_EXPORT_ORDER_BUTTON);
	m_ExportOrderButton.SetTip("Export Order");
	m_ExportOrderButton.SetImage(m_q == 2 ? IDR_EXPORT_ORDER_3 : m_q == 1 ? IDR_EXPORT_ORDER_2 : IDR_EXPORT_ORDER_1);

	// 14
	m_ExportScreenButton.Create(_T(""), _T(""), WS_CHILD, m_BR[2][13], this, IDC_EXPORT_SCREEN_BUTTON);
	m_ExportScreenButton.SetTip("Export Data");
	m_ExportScreenButton.SetImage(m_q == 2 ? IDR_EXPORT_DATA_3 : m_q == 1 ? IDR_EXPORT_DATA_2 : IDR_EXPORT_DATA_1);

	// 15
	m_SaveScreenButton.Create(_T(""), _T(""), WS_CHILD, m_BR[1][14], this, IDC_SAVE_SCREEN_BUTTON);
	m_SaveScreenButton.SetTip("Save Screen");
	m_SaveScreenButton.SetImage(m_q == 2 ? IDR_SAVE_SCREEN_3 : m_q == 1 ? IDR_SAVE_SCREEN_2 : IDR_SAVE_SCREEN_1);

	m_PrintScreenButton.Create(_T(""), _T(""), WS_CHILD, m_BR[2][14], this, IDC_PRINT_SCREEN_BUTTON);
	m_PrintScreenButton.SetTip("Print Screen");
	m_PrintScreenButton.SetImage(m_q == 2 ? IDR_PRINT_SCREEN_3 : m_q == 1 ? IDR_PRINT_SCREEN_2 : IDR_PRINT_SCREEN_1);

	// 16
	m_BackButton.Create(_T(""), _T(""), WS_CHILD, m_BR[0][15], this, IDC_BACK_BUTTON);

	// patients list
	::SetRect(&m_PatientsListRect, intRound(0.005 * W), intRound(0.100 * H), intRound(0.915 * W), intRound(0.964 * H));

	m_PatientsList.m_HeaderCtrl.m_sortable = TRUE;
	m_PatientsList.SetFont(&m_Font);
	m_PatientsList.CreateWnd(0, m_PatientsListRect, this, IDC_PATIENTS_LIST);

	real_t w1[10] = { 0.06 * W, 0.10*W, 0.13 * W, 0.13 * W, 0.07 * W, 0.06 * W, 0.03 * W, 0.08 * W, 0.05*W, 0.185 * W };

	m_PatientsList.InsertColumn(0, _T("ID"), intRound(w1[0]));
	m_PatientsList.InsertColumn(1, _T("Registration Date"), intRound(w1[1]));
	m_PatientsList.InsertColumn(2, _T("Last name"), intRound(w1[2]));
	m_PatientsList.InsertColumn(3, _T("First name"), intRound(w1[3]));
	m_PatientsList.InsertColumn(4, _T("Middle name"), intRound(w1[4]));
	m_PatientsList.InsertColumn(5, _T("DOB"), intRound(w1[5]));
	m_PatientsList.InsertColumn(6, _T("Sex"), intRound(w1[6]));
	m_PatientsList.InsertColumn(7, _T("Group"), intRound(w1[7]));
	m_PatientsList.InsertColumn(8, _T("Source"), intRound(w1[8]));//Dicom
	m_PatientsList.InsertColumn(9, _T("Note"), intRound(w1[9]));

	m_PatientsList.m_HeaderCtrl.m_sort_col = ::Settings.m_PatientsListColumn;
	if (m_PatientsList.m_HeaderCtrl.m_sort_col < 0 || m_PatientsList.m_HeaderCtrl.m_sort_col >= m_PatientsList.m_HeaderCtrl.m_num_cols)
	{
		m_PatientsList.m_HeaderCtrl.m_sort_col = 0;
	}
	m_PatientsList.m_HeaderCtrl.m_sort_asc = ::Settings.m_PatientsListAscending;

	m_PatientsList.SetExtendedStyle(m_PatientsList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_TRACKSELECT);//6.2.0 For unicode

	real_t l = 0.005 * W;
	real_t t = 0.964 * H - 1.0;
	real_t r = l + w1[0] + 3.0;
	real_t b = 0.991 * H;
	::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b));
	m_CustomIDEdit.Create(WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, Rect, this, IDC_CUSTOM_ID_EDIT);
	m_CustomIDEdit.SetLimitText(49);
	m_CustomIDEdit.SetFont(&m_Font, FALSE);

	//registration time check button here
	l = r - 1.0;
	r = l + w1[1] + 1.0;
	::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b));
	m_RegTimeButton.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_REGI_TIME_BUTTON);
	m_RegTimeButton.SetTip("Registration Time");

	l = r - 1.0;
	r = l + w1[2] + 1.0;
	::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b));
	m_LastNameEdit.Create(WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, Rect, this, IDC_LAST_NAME_EDIT);
	m_LastNameEdit.SetLimitText(49);
	m_LastNameEdit.SetFont(&m_Font, FALSE);

	l = r - 1.0;
	r = l + w1[3] + 1.0;
	::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b));
	m_FirstNameEdit.Create(WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, Rect, this, IDC_FIRST_NAME_EDIT);
	m_FirstNameEdit.SetLimitText(49);
	m_FirstNameEdit.SetFont(&m_Font, FALSE);

	l = r - 1.0;
	r = l + w1[4] + 1.0;
	::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b));
	m_MiddleNameEdit.Create(WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, Rect, this, IDC_MID_NAME_EDIT);
	m_MiddleNameEdit.SetLimitText(49);
	m_MiddleNameEdit.SetFont(&m_Font, FALSE);

	l = r - 1.0;
	r = l + w1[5] + 1.0;
	::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b));
	m_DOBEdit.Create(WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER, Rect, this, IDC_DOB_EDIT);
	m_DOBEdit.SetLimitText(10);
	m_DOBEdit.SetFont(&m_Font, FALSE);
	m_InputDOBLen = 0;

	l = r - 1.0;
	r = l + w1[6] + 2.0;
	::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b) + 100);
	m_SexComboBox.Create(WS_CHILD | WS_BORDER | WS_VSCROLL | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST, Rect, this, IDC_SEX_COMBOBOX);
	m_SexComboBox.SetFont(&m_Font, FALSE);
	m_SexComboBox.AddString(_T(""));
	m_SexComboBox.AddString(_T("M"));
	m_SexComboBox.AddString(_T("F"));
	m_SexComboBox.SetCurSel(0);

	l = r - 1.0;
	r = l + w1[7] + 1.0;
	::SetRect(&Rect, intRound(l), intRound(t), intRound(r), intRound(b) + 100);
	m_GroupsComboBox.Create(WS_CHILD | WS_BORDER | WS_VSCROLL | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST, Rect, this, IDC_GROUPS_COMBOBOX);
	m_GroupsComboBox.SetFont(&m_Font, FALSE);

	l = r + 3.0;
	r = l + intRound(w1[6] * 0.8);
	::SetRect(&Rect, intRound(l), intRound(t + 1), intRound(r), intRound(b + 1));
	m_SearchDefButton.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_SEARCH_DEFAULT_BTN);
	m_SearchDefButton.SetImage(m_q == 2 ? IDR_CANCEL_2 : m_q == 1 ? IDR_CANCEL_2 : IDR_CANCEL_1);//
	m_SearchDefButton.SetTip("Erase search");
	m_SearchDefButton.SetBk(FALSE);

	//fresh button
	//so that the viewer stations can update their database with recently added patients/exams.
	l = intRound(0.89 * W);
	r = l + intRound(w1[6] * 0.8);
	::SetRect(&Rect, intRound(l), intRound(t + 1), intRound(r), intRound(b + 1));
	m_RefrshButton.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_REFRESH_DATA_BTN);
	m_RefrshButton.SetImage(m_q == 2 ? IDB_DATA_REFREASH2 : m_q == 1 ? IDB_DATA_REFREASH1 : IDB_DATA_REFREASH1);
	m_RefrshButton.SetTip("Refresh database");
	m_RefrshButton.SetBk(FALSE);

	//database button to replace the CTRLD functional
	l = intRound(0.005 * W);
	r = intRound(0.02 * W);
	::SetRect(&Rect, intRound(l), intRound(0.07 * H), intRound(r), intRound(0.10 * H));
	m_DatabaseButton.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_DATABASE_BTN);
	m_DatabaseButton.SetImage(m_q == 2 ? IDB_DATABASE_BTN2 : m_q == 1 ? IDB_DATABASE_BTN1 : IDB_DATABASE_BTN1);
	m_DatabaseButton.SetTip("Database setting");
	m_DatabaseButton.SetBk(FALSE);

	// Database test
	//l = intRound(0.3 * W);
	//r = intRound(0.32 * W);
	//::SetRect(&Rect, intRound(l), intRound(0.07 * H), intRound(r), intRound(0.10 * H));
	//m_DataTestButton.Create(_T(""), _T(""),  WS_CHILD, Rect, this, IDC_DATABASE_TEST_BTN);
	//m_DataTestButton.SetText("D Test");
	//m_DataTestButton.SetTip("Database test");
	///

	//[the rect of patient count showing position
	::SetRect(&m_PatientCountRect, intRound(0.75 * W), intRound(t), intRound(0.89*W), intRound(b));
	m_PatientCountFont.Create(intRound(0.8* (m_PatientCountRect.bottom - m_PatientCountRect.top)), 400, "Arial");
	//the rect of patient count showing position

	m_DateBaseFolderFont.Create(intRound(0.6* (m_PatientCountRect.bottom - m_PatientCountRect.top)), 400, "Arial");

	//The excel export note
	::SetRect(&m_ExcelExportRect, intRound(0.75 * W), intRound(0.07 * H), intRound(0.91*W), intRound(0.10 * H));
	//Done

	//The database folder note
	::SetRect(&m_DataBaseFolderRect, intRound(0.02 * W), intRound(0.07 * H), intRound(0.457*W), intRound(0.10 * H));
	//Done

	// exams lists
	// WF OD
	::SetRect(&m_LabelsRect[0], intRound(0.005 * W), intRound(0.100 * H), intRound(0.457 * W), intRound(0.130 * H) + 1);
	::SetRect(&m_Labels2Rect[0], intRound(0.355 * W), intRound(0.100 * H), intRound(0.447 * W), intRound(0.130 * H) + 1);//[cjf***05092012]
	::SetRect(&m_ExamsListRect[0], intRound(0.005 * W), intRound(0.130 * H), intRound(0.457 * W), intRound(0.540 * H));
	// CT OD
	::SetRect(&m_LabelsRect[1], intRound(0.005 * W), intRound(0.550 * H), intRound(0.457 * W), intRound(0.580 * H) + 1);
	::SetRect(&m_Labels2Rect[1], intRound(0.355 * W), intRound(0.550 * H), intRound(0.447 * W), intRound(0.580 * H) + 1);//[cjf***05092012]
	::SetRect(&m_ExamsListRect[1], intRound(0.005 * W), intRound(0.580 * H), intRound(0.457 * W), intRound(0.990 * H));
	// WF OS
	::SetRect(&m_LabelsRect[2], intRound(0.463 * W), intRound(0.100 * H), intRound(0.915 * W), intRound(0.130 * H) + 1);
	::SetRect(&m_Labels2Rect[2], intRound(0.813 * W), intRound(0.100 * H), intRound(0.905 * W), intRound(0.130 * H) + 1);//[cjf***05092012]
	::SetRect(&m_ExamsListRect[2], intRound(0.463 * W), intRound(0.130 * H), intRound(0.915 * W), intRound(0.540 * H));
	// CT OS
	::SetRect(&m_LabelsRect[3], intRound(0.463 * W), intRound(0.550 * H), intRound(0.915 * W), intRound(0.580 * H) + 1);
	::SetRect(&m_Labels2Rect[3], intRound(0.813 * W), intRound(0.550 * H), intRound(0.905 * W), intRound(0.580 * H) + 1);//[cjf***05092012]
	::SetRect(&m_ExamsListRect[3], intRound(0.463 * W), intRound(0.580 * H), intRound(0.915 * W), intRound(0.990 * H));
	// WF OD large
	::SetRect(&m_ExamsListRect[4], intRound(0.005 * W), intRound(0.130 * H), intRound(0.457 * W), intRound(0.990 * H));
	// WF OS large
	::SetRect(&m_ExamsListRect[5], intRound(0.463 * W), intRound(0.130 * H), intRound(0.915 * W), intRound(0.990 * H));

	real_t w2[11] = { ::NewSettings.WFOD_COL_Width[0] * W, ::NewSettings.WFOD_COL_Width[1] * W,
		::NewSettings.WFOD_COL_Width[2] * W, ::NewSettings.WFOD_COL_Width[3] * W,
		::NewSettings.WFOD_COL_Width[4] * W, ::NewSettings.WFOD_COL_Width[5] * W,
		::NewSettings.WFOD_COL_Width[6] * W, ::NewSettings.WFOD_COL_Width[7] * W,
		::NewSettings.WFOD_COL_Width[8] * W, ::NewSettings.WFOD_COL_Width[9] * W,
		::NewSettings.WFOD_COL_Width[10] * W };

	m_WFODList.SetFont(&m_Font);
	m_WFODList.m_HeaderCtrl.m_sortable = TRUE;
	m_WFODList.CreateWnd(0, m_ExamsListRect[0], this, IDC_WF_OD_EXAMS_LIST);

	m_WFODList.InsertColumn(0, _T("#"), intRound(w2[0]));
	m_WFODList.InsertColumn(1, _T("Date"), intRound(w2[1]));
	m_WFODList.InsertColumn(2, _T("Time"), intRound(w2[2]));
	m_WFODList.InsertColumn(3, _T("Ver"), intRound(w2[3]));
	m_WFODList.InsertColumn(4, _T("Mode"), intRound(w2[4]));
	m_WFODList.InsertColumn(5, _T("Clinic"), intRound(w2[5]));
	m_WFODList.InsertColumn(6, _T("Physician"), intRound(w2[6]));
	m_WFODList.InsertColumn(7, _T("Operator"), intRound(w2[7]));
	m_WFODList.InsertColumn(8, _T("Preop"), intRound(w2[8]));
	m_WFODList.InsertColumn(9, _T("Srs"), intRound(w2[9]));
	m_WFODList.InsertColumn(10, _T("Scan"), intRound(w2[2] + 10));
	m_WFODList.InsertColumn(11, _T("Note"), intRound(w2[10] - w2[2] - 15));

	real_t w3[11] = { ::NewSettings.CTOD_COL_Width[0] * W, ::NewSettings.CTOD_COL_Width[1] * W,
		::NewSettings.CTOD_COL_Width[2] * W, ::NewSettings.CTOD_COL_Width[3] * W,
		::NewSettings.CTOD_COL_Width[4] * W, ::NewSettings.CTOD_COL_Width[5] * W,
		::NewSettings.CTOD_COL_Width[6] * W, ::NewSettings.CTOD_COL_Width[7] * W,
		::NewSettings.CTOD_COL_Width[8] * W, ::NewSettings.CTOD_COL_Width[9] * W,
		::NewSettings.CTOD_COL_Width[10] * W };

	m_CTODList.SetFont(&m_Font);
	m_CTODList.m_HeaderCtrl.m_sortable = TRUE;
	m_CTODList.CreateWnd(0, m_ExamsListRect[1], this, IDC_CT_OD_EXAMS_LIST);

	m_CTODList.InsertColumn(0, _T("#"), intRound(w3[0]));
	m_CTODList.InsertColumn(1, _T("Date"), intRound(w3[1]));
	m_CTODList.InsertColumn(2, _T("Time"), intRound(w3[2]));
	m_CTODList.InsertColumn(3, _T("Ver"), intRound(w3[3]));
	m_CTODList.InsertColumn(4, _T("Mode"), intRound(w3[4]));
	m_CTODList.InsertColumn(5, _T("Clinic"), intRound(w3[5]));
	m_CTODList.InsertColumn(6, _T("Physician"), intRound(w3[6]));
	m_CTODList.InsertColumn(7, _T("Operator"), intRound(w3[7]));
	m_CTODList.InsertColumn(8, _T("Preop"), intRound(w3[8]));
	m_CTODList.InsertColumn(9, _T("Srs"), intRound(w3[9]));
	m_CTODList.InsertColumn(10, _T("Note"), intRound(w3[10]));

	real_t w4[11] = { ::NewSettings.WFOS_COL_Width[0] * W, ::NewSettings.WFOS_COL_Width[1] * W,
		::NewSettings.WFOS_COL_Width[2] * W, ::NewSettings.WFOS_COL_Width[3] * W,
		::NewSettings.WFOS_COL_Width[4] * W, ::NewSettings.WFOS_COL_Width[5] * W,
		::NewSettings.WFOS_COL_Width[6] * W, ::NewSettings.WFOS_COL_Width[7] * W,
		::NewSettings.WFOS_COL_Width[8] * W, ::NewSettings.WFOS_COL_Width[9] * W,
		::NewSettings.WFOS_COL_Width[10] * W };

	m_WFOSList.SetFont(&m_Font);

	m_WFOSList.m_HeaderCtrl.m_sortable = TRUE;
	m_WFOSList.CreateWnd(0, m_ExamsListRect[2], this, IDC_WF_OS_EXAMS_LIST);
	m_WFOSList.InsertColumn(0, _T("#"), intRound(w4[0]));
	m_WFOSList.InsertColumn(1, _T("Date"), intRound(w4[1]));
	m_WFOSList.InsertColumn(2, _T("Time"), intRound(w4[2]));
	m_WFOSList.InsertColumn(3, _T("Ver"), intRound(w4[3]));
	m_WFOSList.InsertColumn(4, _T("Mode"), intRound(w4[4]));
	m_WFOSList.InsertColumn(5, _T("Clinic"), intRound(w4[5]));
	m_WFOSList.InsertColumn(6, _T("Physician"), intRound(w4[6]));
	m_WFOSList.InsertColumn(7, _T("Operator"), intRound(w4[7]));
	m_WFOSList.InsertColumn(8, _T("Preop"), intRound(w4[8]));
	m_WFOSList.InsertColumn(9, _T("Srs"), intRound(w4[9]));
	m_WFOSList.InsertColumn(10, _T("Scan"), intRound(w4[2] + 10));
	m_WFOSList.InsertColumn(11, _T("Note"), intRound(w4[10] - w2[2] - 15));

	real_t w5[11] = { ::NewSettings.CTOS_COL_Width[0] * W, ::NewSettings.CTOS_COL_Width[1] * W,
		::NewSettings.CTOS_COL_Width[2] * W, ::NewSettings.CTOS_COL_Width[3] * W,
		::NewSettings.CTOS_COL_Width[4] * W, ::NewSettings.CTOS_COL_Width[5] * W,
		::NewSettings.CTOS_COL_Width[6] * W, ::NewSettings.CTOS_COL_Width[7] * W,
		::NewSettings.CTOS_COL_Width[8] * W, ::NewSettings.CTOS_COL_Width[9] * W,
		::NewSettings.CTOS_COL_Width[10] * W };
	m_CTOSList.SetFont(&m_Font);

	m_CTOSList.m_HeaderCtrl.m_sortable = TRUE;
	m_CTOSList.CreateWnd(0, m_ExamsListRect[3], this, IDC_CT_OS_EXAMS_LIST);
	m_CTOSList.InsertColumn(0, _T("#"), intRound(w5[0]));
	m_CTOSList.InsertColumn(1, _T("Date"), intRound(w5[1]));
	m_CTOSList.InsertColumn(2, _T("Time"), intRound(w5[2]));
	m_CTOSList.InsertColumn(3, _T("Ver"), intRound(w5[3]));
	m_CTOSList.InsertColumn(4, _T("Mode"), intRound(w5[4]));
	m_CTOSList.InsertColumn(5, _T("Clinic"), intRound(w5[5]));
	m_CTOSList.InsertColumn(6, _T("Physician"), intRound(w5[6]));
	m_CTOSList.InsertColumn(7, _T("Operator"), intRound(w5[7]));
	m_CTOSList.InsertColumn(8, _T("Preop"), intRound(w5[8]));
	m_CTOSList.InsertColumn(9, _T("Srs"), intRound(w5[9]));
	m_CTOSList.InsertColumn(10, _T("Note"), intRound(w5[10]));

	m_WFODList.SetExtendedStyle(m_WFODList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_TRACKSELECT);
	m_WFOSList.SetExtendedStyle(m_WFOSList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_TRACKSELECT);//6.2.0 For unicode
	m_CTODList.SetExtendedStyle(m_CTODList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_TRACKSELECT);//6.2.0 For unicode
	m_CTOSList.SetExtendedStyle(m_CTOSList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_TRACKSELECT);//6.2.0 For unicode

	::SetRect(&m_ImgInfoRect, 0, 0, 312, 234);
	m_ExamImgInfo.CreateWnd(m_ImgInfoRect, this);
	m_ExamImgInfo.ShowWindow(FALSE);

	::SetRect(&m_ImgInfoRect, 0, 0, 640, 60);
	m_NoteImgInfo.CreateWnd(m_ImgInfoRect, this);
	m_NoteImgInfo.ClearMemDC();
	m_NoteImgInfo.ShowWindow(FALSE);

	// summary window
	::SetRect(&m_SumRect, 0, 0, intRound(SW), H);
  g_summaryRect = m_SumRect;

	::SetRect(&m_BigRect, 0, 0, W, H);

	m_pSumWnd = NULL;

	m_BigPlanWnd = NULL;
	m_BigCTSumWnd = NULL;

	m_Screen = SCREEN_NONE;

	// labels
	::SetRect(&m_NameRect, 0, 0, intRound(0.905 * W), intRound(0.3 * m_PatientsListRect.top));
	::SetRect(&m_CustomIDRect, 0, intRound(0.3 * m_PatientsListRect.top), intRound(0.905 * W), intRound(0.6 * m_PatientsListRect.top));
	::SetRect(&m_VersionRect, intRound(0.005 * W), intRound(0.99 * H), W, H);

	m_LabelsFont.Create(intRound(1.1 * (m_NameRect.bottom - m_NameRect.top)), 400, "Arial");
	m_LabelsFont2.Create(intRound(0.9 * (m_NameRect.bottom - m_NameRect.top)), 400, "Arial");
	m_VersionFont.Create(intRound(1.5 * (m_VersionRect.bottom - m_VersionRect.top)), 400, "Arial");

	m_Level = LEVEL_MAIN;

	ShowMainLevelControls();

	m_FellowIDCaled = FALSE;
	m_Update = TRUE;

	UpdatePatientsList();

	if (!m_isValid)
	{
    CSplashScreen::Instance()->HideSplash();

    CStatusDlg *pDlg = new CStatusDlg(this);
		pDlg->DoModal();
		delete pDlg;
		ChangeLicese();
		Invalidate(FALSE);
	}


	// 7.0.0 inilization of optometer
  ::HW.ISVideoCameraOn = false;
  if (!m_isViewer && ::HW.IsConnected()) {
	HW.TurnVideoCameraOn();
	::HW.ISVideoCameraOn = true;
	::HW.InitializeOptometer();
	::HW.MoveOptometerTargetToPositionD(10.0, TRUE);
  }
  // 7.0.0 inilization of optometer

	////If it is first run of 6.2.1 Show the exporting info
	////get the local db folder
	//CString FolderName;

	//::DB.GetFolderName(FolderName);

	//FolderName = FolderName + "\\ExamHeader";
	//
	//if (!::PathFileExists(FolderName))
	//{
	//	::CreateDirectory(FolderName, NULL);

	//	::Info("An update to the database is required. \n This process may take serveral minutes.\n Please click OK to continue.");
	//	//Sleep(1000);
	//	SplitExamHeaderToText();
	//	//::DB.GenerateExamAmount();
	//}
	//else
	//{
	//	CBusyCursor Cursor;
	//	::DB.CheckPreNewExams();
	//	m_PatientsList.SetFocus();
	//}

	ShowWindow(SW_SHOW);
  CSplashScreen::Instance()->HideSplash();
}

//***************************************************************************************

CMainWnd::~CMainWnd()
{
	::Settings.m_PatientsListColumn = m_PatientsList.m_HeaderCtrl.m_sort_col;
	::Settings.m_PatientsListAscending = m_PatientsList.m_HeaderCtrl.m_sort_asc;

	SAFE_DELETE(m_pSumWnd);

	DeleteLoadedExams();

	::DestroyAcceleratorTable(m_hAccel);
}

//***************************************************************************************

BOOL CMainWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
  UINT nID   = LOWORD(wParam);
  int  nCode = HIWORD(wParam);

  if (CmdTargetBase::ForwardCommand(nID, nCode, nullptr, nullptr))
    return TRUE;

  return CWnd::OnCommand(wParam, lParam);
}

//***************************************************************************************

void CMainWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case TIMER1:
	{

	}

	case TIMER2:
	{
		if (m_ShowInfo)
		{
			if (!m_NoteImgInfo.IsWindowVisible())
			{
				m_NoteImgInfo.ShowWindow(TRUE);
			}
			m_NoteImgInfo.Invalidate(TRUE);
		}

		KillTimer(TIMER2);
		break;
	}
	}
}

//***************************************************************************************

BOOL CMainWnd::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		CWnd* pWnd = NULL;

		if (pMsg->wParam == 0x59 || pMsg->wParam == 0x5A) //Ctrl + Y, Ctrl + Z;
		{
			if (GetKeyState(VK_CONTROL))
			{
				pWnd = this;
			}
		}
		else
		{
			switch (m_Level)
			{
				//Main GUI
			case LEVEL_MAIN:

				if (pMsg->wParam == VK_ESCAPE)
				{
					pWnd = this;
				}
				//0x41 A; 0x42 B; 0x43 C; 44 D; 45 E; 46 F; 47 G; 48 H; 49 I; 4A J; 4B K; 4C L;
				//0x4D M; 0x4E N; 0x4F O; 50 P; 51 Q; 52 R; 53 S; 54 T; 55 U; 56 V; 57 W; 58 X;
				//0x59 Y; 0x5A Z;
				else if (pMsg->wParam == 0x41 || pMsg->wParam == 0x44 || pMsg->wParam == 0x57
					|| pMsg->wParam == 0x59 || pMsg->wParam == 0x5A || pMsg->wParam == 0x51)
				{
					if (GetKeyState(VK_CONTROL))
					{
						pWnd = this;
					}
				}
				else if (pMsg->wParam == VK_F1)
					pWnd = this;

				break;

			case LEVEL_PATIENT:

				m_Shift = 0;

				if (pMsg->wParam == VK_ESCAPE)
				{
					pWnd = this;
				}
				//0x4c :L ; 0x4d: M; 0x4e : N
				else if (pMsg->wParam == 0x4c || pMsg->wParam == 0x4d || pMsg->wParam == 0x4e)
				{
					if (GetKeyState(VK_CONTROL))
					{
						pWnd = this;
					}
				}

				else if (pMsg->wParam == VK_SHIFT) m_Shift = 1;

				else if (pMsg->wParam == VK_F1)  pWnd = this;

				break;
				//Main GUI Done

				//Summary GUI
			case LEVEL_SUMMARY:

				if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_F9)
				{
					pWnd = this;
				}

				else if (pMsg->wParam == 0x4c || pMsg->wParam == 0x4d || pMsg->wParam == 0x4e)
				{
					if (GetKeyState(VK_CONTROL))
					{
						pWnd = this;
					}
				}

				else if (m_Screen == SCREEN_WFVRF)
				{
					if (pMsg->wParam == VK_F1 || pMsg->wParam == VK_F2 || pMsg->wParam == VK_F3 || pMsg->wParam == VK_F4 ||
						pMsg->wParam == VK_F7 ||
						pMsg->wParam == VK_LEFT || pMsg->wParam == VK_RIGHT)
					{
						pWnd = m_pSumWnd;
					}
				}
				else if (pMsg->wParam == VK_F1)
					pWnd = this;

				break;
				//Summary GUI Done
			}
		}

		if (pWnd)
		{
			if (TranslateAccelerator(pWnd->m_hWnd, m_hAccel, pMsg)) return TRUE;
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}

//***************************************************************************************

void CMainWnd::PostNcDestroy()
{
	delete this;
}

//***************************************************************************************

////Big Database Research since 03062019
//void CMainWnd::SplitExamHeaderToText()
//{
//	CBusyCursor Cursor;
//
//	CString  s1, s2;
//	CString  sECount;
//	CMFont   Info1Font, Info2Font;
//	COLORREF InfoBkColor = 0x00efdfcf;
//	COLORREF ProBlockBkColor = 0x00efffdf;
//	COLORREF ProcessingBkColor = 0x00d0d0d0;
//	RECT     ExportingRect, Info1Rect, Info2Rect, RestRect, ProcessRect,
//		ProcessingRect, OutLineRect, Shadow1Rect, Shadow2Rect, Shadow3Rect, Shadow4Rect;
//	real div = (real)m_TotPatCount / 100.0;
//	int Step = intRound(div);
//
//	::SetRect(&ExportingRect, 3000, 2000, 3400, 2200);
//	::SetRect(&Info1Rect, 5, 0, 400, 40);
//	::SetRect(&Info2Rect, 50, 60, 400, 100);
//	::SetRect(&RestRect, 5, 50, 400, 195);
//	::SetRect(&ProcessRect, 50, 120, 350, 140);
//	::SetRect(&OutLineRect, 5, 0, 400, 195);
//	::SetRect(&Shadow1Rect, 0, 5, 5, 200);
//	::SetRect(&Shadow2Rect, 0, 195, 395, 200);
//	::SetRect(&Shadow3Rect, 0, 0, 5, 5);
//	::SetRect(&Shadow4Rect, 395, 195, 400, 200);
//
//	CImgInfoWnd m_ExportingInfo;//Big Database Research since 03062019
//
//	m_ExportingInfo.CreateWnd(ExportingRect, this);
//
//	sECount.Format(_T("%i"), m_TotPatCount);
//
//	s1 = "6.2.1 Database is updateing (Total " + sECount + "  records)";
//	s2 = "";
//
//	Info1Font.Create(intRound(0.8* (20)), 400, "Arial");
//	Info2Font.Create(intRound(0.9* (20)), 400, "Arial");
//
//	m_ExportingInfo.m_MemDC.FillSolidRect(&Shadow1Rect, 0x00cccccc);
//	m_ExportingInfo.m_MemDC.FillSolidRect(&Shadow2Rect, 0x00cccccc);
//	m_ExportingInfo.m_MemDC.FillSolidRect(&Shadow3Rect, WHITE);
//	m_ExportingInfo.m_MemDC.FillSolidRect(&Shadow4Rect, WHITE);
//	m_ExportingInfo.m_MemDC.FillSolidRect(&Info1Rect, InfoBkColor);
//	m_ExportingInfo.m_MemDC.FillSolidRect(&RestRect, ProBlockBkColor);
//	m_ExportingInfo.m_MemDC.FillSolidRect(&ProcessRect, ProcessingBkColor);
//	m_ExportingInfo.m_MemDC.WriteText(s1, Info1Rect, Info1Font, BLACK, 1, NOCOLOR);
//	m_ExportingInfo.m_MemDC.WriteText(s2, Info2Rect, Info2Font, BLACK, 0, NOCOLOR);
//	m_ExportingInfo.m_MemDC.DrawRectangle(ProcessRect, BLUE, NOCOLOR);
//	m_ExportingInfo.m_MemDC.DrawRectangle(OutLineRect, BLACK, NOCOLOR);
//	m_ExportingInfo.CenterWindow(0);
//	m_ExportingInfo.ShowWindow(SW_SHOW);
//	m_ExportingInfo.Repaint();
//	m_ExportingInfo.Invalidate(FALSE);
//
//	//get the local db folder
//	CString FolderName;
//
//	::DB.GetFolderName(m_DataFolderName);
//
//	FolderName = m_DataFolderName + "\\ExamHeader";
//
//	ExamHeader_STR ThisExamStr;
//	CString IDStr;
//	int StepVal = 0;
//	int ProVal = 0;
//	int restECount = m_TotPatCount;
//
//	for (int i = 0; i < m_TotPatCount; i++)
//	{
//		CPatient* pPatient = &::DB.m_Patients[i];
//
//		::DB.LoadExamHeaders(pPatient->m_PatientID);//Big Database Research since 03062019
//
//		int ExamSize = ::DB.m_ExamHeaders.GetSize();
//
//		//Refresh the process box
//		m_ExportingInfo.ClearMemDC();
//		m_ExportingInfo.m_MemDC.FillSolidRect(&Shadow1Rect, 0x00cccccc);
//		m_ExportingInfo.m_MemDC.FillSolidRect(&Shadow2Rect, 0x00cccccc);
//		m_ExportingInfo.m_MemDC.FillSolidRect(&Shadow3Rect, WHITE);
//		m_ExportingInfo.m_MemDC.FillSolidRect(&Shadow4Rect, WHITE);
//		m_ExportingInfo.m_MemDC.FillSolidRect(&Info1Rect, InfoBkColor);
//		m_ExportingInfo.m_MemDC.FillSolidRect(&RestRect, ProBlockBkColor);
//		m_ExportingInfo.m_MemDC.FillSolidRect(&ProcessRect, ProcessingBkColor);
//		m_ExportingInfo.m_MemDC.WriteText(s1, Info1Rect, Info1Font, BLACK, 1, NOCOLOR);
//		m_ExportingInfo.m_MemDC.DrawRectangle(ProcessRect, BLUE, NOCOLOR);
//		m_ExportingInfo.m_MemDC.DrawRectangle(OutLineRect, BLACK, NOCOLOR);
//
//		StepVal++;
//		restECount--;
//
//		if (StepVal == Step)
//		{
//			StepVal = 0;
//			ProVal++;
//			if (ProVal == 100 && restECount > 0)
//			{
//				ProVal = 99;
//			}
//		}
//
//		::SetRect(&ProcessingRect, 50, 120, 50 + ProVal * 3, 140);
//		m_ExportingInfo.m_MemDC.DrawRectangle(ProcessingRect, BLUE, BLUE);
//
//		sECount.Format(_T("%i"), restECount);
//		s2.Format(_T("%i records remaining"), restECount);
//		m_ExportingInfo.m_MemDC.WriteText(s2, Info2Rect, Info2Font, BLACK, 0, NOCOLOR);
//
//		m_ExportingInfo.Repaint();
//		m_ExportingInfo.Invalidate(FALSE);
//		//Refresh the process box
//
//
//		for (int e = 0; e < ExamSize; e++)
//		{
//			CExamHeader* pExamHeader = &::DB.m_ExamHeaders[e];
//
//			GUIDToStr(pExamHeader->m_ExamID, IDStr);
//			ThisExamStr.ExamID_Str = IDStr;
//
//			GUIDToStr(pExamHeader->m_CalibrationID, IDStr);
//			ThisExamStr.CalibrationID_Str = IDStr;
//
//			GUIDToStr(pExamHeader->m_ClinicID, IDStr);
//			ThisExamStr.ClinicIDStr = IDStr;
//
//			GUIDToStr(pExamHeader->m_PhysicianID, IDStr);
//			ThisExamStr.PhysicianID_Str = IDStr;
//
//			GUIDToStr(pExamHeader->m_OperatorID, IDStr);
//			ThisExamStr.OperatorID_Str = IDStr;
//
//			ThisExamStr.Type_Str.Format("%i", pExamHeader->m_Type);
//			ThisExamStr.Mode_Str.Format("%i", pExamHeader->m_Mode);
//			ThisExamStr.Year_Str.Format("%i", pExamHeader->m_Year);
//			ThisExamStr.Month_Str.Format("%i", pExamHeader->m_Month);
//			ThisExamStr.Day_Str.Format("%i", pExamHeader->m_Day);
//			ThisExamStr.Hour_Str.Format("%i", pExamHeader->m_Hour);
//			ThisExamStr.Minute_Str.Format("%i", pExamHeader->m_Minute);
//			ThisExamStr.Second_Str.Format("%i", pExamHeader->m_Second);
//			ThisExamStr.Eye_Str.Format("%i", pExamHeader->m_Eye);
//			ThisExamStr.Preop_Str.Format("%i", pExamHeader->m_Preop);
//			ThisExamStr.SeriesNumber_Str.Format("%i", pExamHeader->m_SeriesNumber);
//
//			ThisExamStr.Note_Str = pExamHeader->m_Note;
//			ThisExamStr.SoftwareVersion_Str.Format("%i", pExamHeader->m_SoftwareVersion);
//
//			ThisExamStr.Reserved3_Str = "-1";
//			ThisExamStr.Reserved4_Str = "-1";
//
//			::DB.SaveTxTExamHeader(pPatient->m_PatientID, ThisExamStr);
//		}
//	}
//
//	m_ExportingInfo.ShowWindow(FALSE);
//	::Info("Database has been updated sucessfully.");
//}
////Big Database Research since 03062019

//***************************************************************************************
//If The software detected that conncect tracey machine is connected or rmoved
BOOL CMainWnd::OnDeviceChange(UINT nEventType, DWORD_PTR dwData)
{
	if (m_isDemo || m_isViewer) return TRUE;

	int Result = ::HW.DeviceChange(nEventType, dwData);

	if (Result == DEVICE_ARRIVED || Result == DEVICE_REMOVED)
	{
		BOOL IsDBReadOnly = ::DB.IsReadOnly();
		BOOL IsHardwareOK = ::HW.IsConnected() && ::HW.HasCalibrationLoaded();

		//Alwasy Turn on the camera even their is not acquisition
		if (IsHardwareOK)
		{
			if (!::HW.ISVideoCameraOn)
			{
				HW.TurnVideoCameraOn();
				::HW.ISVideoCameraOn = true;
			}

			::HW.InitializeOptometer();//7.0.0
			::HW.MoveOptometerTargetToPositionD(10.0, TRUE);//7.0.0
		}
		else ::HW.ISVideoCameraOn = false;
		//Turn on the camera Done

		if (m_Level == LEVEL_MAIN)
		{
			m_TestButton.SetEnabled(IsHardwareOK);
		}
		else if (m_Level == LEVEL_PATIENT || m_Level == LEVEL_SUMMARY)
		{
			m_NewWFExamButton.SetEnabled(!IsDBReadOnly && IsHardwareOK);
			m_NewWFExamButton.Repaint();
			m_NewCTExamButton.SetEnabled(!IsDBReadOnly && IsHardwareOK);
			m_NewCTExamButton.Repaint();
			m_NewWFCTExamButton.SetEnabled(!IsDBReadOnly && IsHardwareOK);
			m_NewWFCTExamButton.Repaint();
		}
	}

	return TRUE;
}

//***************************************************************************************

void CMainWnd::OnPaint()
{
	CPaintDC PaintDC(this);

	CMDC DC;
	DC.Attach(PaintDC.Detach());

	RECT ClientRect;
	GetClientRect(&ClientRect);

	COLORREF BkColor = m_Level == LEVEL_VIDEO ? BLACK : ::GetSysColor(COLOR_BTNFACE);

	DC.FillSolidRect(&ClientRect, BkColor);

	if (m_WhichTaskBtn != -1 && m_Level == LEVEL_SUMMARY
		&& ::NewSettings.m_Tasks[m_WhichTaskBtn].m_SubBtnNum >= 1)
	{
		CString s = ::NewSettings.m_Tasks[m_WhichTaskBtn].ReadBtnName();//6.2.0 For Chinese

		CString Lines[8];
		RECT Rect = m_BR[0][3];
		int l;
		int n = 0;
		while ((l = s.GetLength()) > 0)
		{
			int l1;
			for (l1 = 1; l1 <= l; l1++)
			{
				RECT Rect1;
				DC.MeasureRect(s.Left(l1), Rect1, m_DateBaseFolderFont);
				if (Rect1.right - Rect1.left > Rect.right - Rect.left)
				{
					l1--;
					for (int l2 = l1; l2 >= 1; l2--)
					{
						if (s[l2 - 1] == ' ')
						{
							l1 = l2;
							break;
						}
					}
					break;
				}
			}

			if (l1 == 0) break;

			if (s.GetAt(l1 - 1) == ' ')
			{
				Lines[n] = s.Left(l1 - 1);
			}
			else
			{
				Lines[n] = s.Left(l1);
			}

			s = s.Right(l - l1);
			n++;
		}

		int dh = (Rect.bottom - Rect.top) / (n + 1);

		Rect.bottom = Rect.top + 2 * dh;
		for (int i = 0; i < n; i++)
		{
			DC.WriteText(Lines[i], Rect, m_DateBaseFolderFont, BROWN, 1);
			Rect.top += dh; Rect.bottom += dh;
		}
	}

	if (m_Level == LEVEL_MAIN || m_Level == LEVEL_PATIENT || m_Level == LEVEL_VIDEO)
	{
		COLORREF TextColor = m_Level == LEVEL_VIDEO ? WHITE : BLACK;

		//Show the patient count
		if (m_Level == LEVEL_MAIN)
		{
			CString s;
			int count = m_PatientsList.GetItemCount();
			s.Format(_T("%d"), count);
			if (m_TotPatCount != count) s = "Found " + s;
			else s = "Total " + s;
			if (count == 0 || count == 1) s = s + " Patient";
			else 	  s = s + " Patients";
			DC.WriteText(s, m_PatientCountRect, m_PatientCountFont, TextColor, 2, NOCOLOR);

			//For Excel Export note
			if (!m_isSolo)
			{
				if (::Settings.m_Export_Excel == 1)
					DC.WriteText("Automatic Excel Record On", m_ExcelExportRect, m_PatientCountFont, BLUE, 2, NOCOLOR);
				else DC.WriteText("Automatic Excel Record Off", m_ExcelExportRect, m_PatientCountFont, BROWN, 2, NOCOLOR);
			}

			CString DataBaseFolder;
			::DB.GetFolderName(DataBaseFolder);
			DC.WriteText("Database Folder:  " + DataBaseFolder, m_DataBaseFolderRect, m_DateBaseFolderFont, BLUE, 0, NOCOLOR);
		}
		//Show the patient count done

		if (m_Level == LEVEL_PATIENT || m_Level == LEVEL_VIDEO)
		{
			// first & last names
			DC.WriteText(m_Patient.FullName(), m_NameRect, m_LabelsFont, TextColor, 2, NOCOLOR);

			// custom id
			DC.WriteText(m_Patient.m_CustomID, m_CustomIDRect, m_LabelsFont, TextColor, 2, NOCOLOR);

			// OD / OS
			if (m_Level == LEVEL_PATIENT)
			{
				::DrawBackgintRound(XP_LIST_HEADER, m_hWnd, DC.m_hDC, HP_HEADERITEM, HIS_NORMAL, m_LabelsRect[0]);
				::DrawBackgintRound(XP_LIST_HEADER, m_hWnd, DC.m_hDC, HP_HEADERITEM, HIS_NORMAL, m_LabelsRect[1]);
				::DrawBackgintRound(XP_LIST_HEADER, m_hWnd, DC.m_hDC, HP_HEADERITEM, HIS_NORMAL, m_LabelsRect[2]);
				::DrawBackgintRound(XP_LIST_HEADER, m_hWnd, DC.m_hDC, HP_HEADERITEM, HIS_NORMAL, m_LabelsRect[3]);
				DC.DrawRectangle(m_LabelsRect[0], 0x00000000, NOCOLOR);
				DC.DrawRectangle(m_LabelsRect[1], 0x00000000, NOCOLOR);
				DC.DrawRectangle(m_LabelsRect[2], 0x00000000, NOCOLOR);
				DC.DrawRectangle(m_LabelsRect[3], 0x00000000, NOCOLOR);
				DC.WriteText("OD WF", m_LabelsRect[0], m_LabelsFont2, TextColor, 1, NOCOLOR);
				DC.WriteText("OD CT", m_LabelsRect[1], m_LabelsFont2, TextColor, 1, NOCOLOR);
				DC.WriteText("OS WF", m_LabelsRect[2], m_LabelsFont2, TextColor, 1, NOCOLOR);
				DC.WriteText("OS CT", m_LabelsRect[3], m_LabelsFont2, TextColor, 1, NOCOLOR);

				CString s;
				int Exam_Num = m_WFODList.GetItemCount();
				s.Format(_T("%d"), Exam_Num);
				if (Exam_Num == 0 || Exam_Num == 1) s += " Exam";
				else s += " Exams";
				DC.WriteText(s, m_Labels2Rect[0], m_Font, TextColor, 2, NOCOLOR);

				Exam_Num = m_CTODList.GetItemCount();
				s.Format(_T("%d"), Exam_Num);
				if (Exam_Num == 0 || Exam_Num == 1) s += " Exam";
				else s += " Exams";
				DC.WriteText(s, m_Labels2Rect[1], m_Font, TextColor, 2, NOCOLOR);

				Exam_Num = m_WFOSList.GetItemCount();
				s.Format(_T("%d"), Exam_Num);
				if (Exam_Num == 0 || Exam_Num == 1) s += " Exam";
				else s += " Exams";
				DC.WriteText(s, m_Labels2Rect[2], m_Font, TextColor, 2, NOCOLOR);

				Exam_Num = m_CTOSList.GetItemCount();
				s.Format(_T("%d"), Exam_Num);
				if (Exam_Num == 0 || Exam_Num == 1) s += " Exam";
				else s += " Exams";
				DC.WriteText(s, m_Labels2Rect[3], m_Font, TextColor, 2, NOCOLOR);
			}
		}

		RECT Rect;
		// Tracey logo
		if (m_isHoya)
		{
			CTraceyLogo TraceyLogo(m_Level == LEVEL_VIDEO ? 0 : 1);
			CMDC TraceyDC;
			TraceyDC.CreateTrueColorDC(TraceyLogo.m_w, TraceyLogo.m_h);
			::SetRect(&Rect, 0, 0, TraceyLogo.m_w, TraceyLogo.m_h);
			TraceyDC.FillSolidRect(&Rect, BkColor);
			TraceyDC.DrawImage(TraceyLogo, 0, 0, 0, 0, TRUE);
			DC.BitBlt(3, 3, TraceyLogo.m_w, TraceyLogo.m_h, &TraceyDC, 0, 0, SRCCOPY);
		}

		// product logo
		CBasicImage *pLogo;
		if (m_isHoya)
		{
			pLogo = new CProductLogo(m_Level == LEVEL_VIDEO ? 0 : 1);
		}
		else
		{
			pLogo = new CiTraceLogo(m_Level == LEVEL_VIDEO ? 0 : 1);
		}

		CMDC LogoDC;
		LogoDC.CreateTrueColorDC(pLogo->m_w, pLogo->m_h);
		::SetRect(&Rect, 0, 0, pLogo->m_w, pLogo->m_h);
		LogoDC.FillSolidRect(&Rect, BkColor);
		LogoDC.DrawImage(*pLogo, 0, 0, 0, 0, TRUE);
		DC.BitBlt(intRound(0.5 * ((m_LabelsRect[0].right + m_LabelsRect[2].left) - pLogo->m_w)),
			intRound(0.050 * ClientRect.bottom - 0.5 * pLogo->m_h),
			pLogo->m_w, pLogo->m_h, &LogoDC, 0, 0, SRCCOPY);
		delete pLogo;

		// version
		CString s("Version " + ::FullSoftwareVersion() + m_LicenseName);
		if (m_isPresbia) s = s + "resbia";//530
		DC.WriteText(s, m_VersionRect, m_VersionFont, TextColor, 0, NOCOLOR);
	}

	PaintDC.Attach(DC.Detach());
}

//***************************************************************************************

void CMainWnd::OnEraseSearchButtonClicked()
{
	m_Update = FALSE;

	m_RegFromTime = "";
	m_RegToTime = "";
	m_RegTimeButton.SetText("");
	m_RegTimeButton.Repaint();

	m_CustomIDEdit.SetWindowText(_T(""));
	m_LastNameEdit.SetWindowText(_T(""));
	m_FirstNameEdit.SetWindowText(_T(""));
	m_MiddleNameEdit.SetWindowText(_T(""));

	m_GroupsComboBox.SetCurSel(0);
	m_DOBEdit.SetWindowText(_T(""));
	m_SexComboBox.SetCurSel(0);
	m_Update = TRUE;

	UpdatePatientsList();

	m_SearchDefButton.SetEnabled(FALSE);
	m_SearchDefButton.Repaint();
}

//***************************************************************************************

//update their database with recently added patients/exams.
void CMainWnd::OnRefreshDataButtonClicked()
{
	m_Update = TRUE;

	UpdatePatientsList();
}

//***************************************************************************************

//update their database with recently added patients/exams.
void CMainWnd::OnDatabaseButtonClicked()
{
	OnCtrlD();
}

//void CMainWnd::OnDatabaseTestButtonClicked()
//{
//	CBusyCursor Cursor;
//
//	CPatient TestPatient;
//	BOOL Found1 = FALSE;
//	BOOL Found2 = FALSE;
//	CString FullName;
//
//	int count = m_PatientsList.GetItemCount();
//
//	const int TimeLimit = 60000 * 60 * 24;
//	int TimeLeft = TimeLimit;
//
//	int StartTime = (int)clock();
//
//	int LastTimeLeft = TimeLeft;
//
//	while (true)
//	{
//		TimeLeft = TimeLimit - ((int)clock() - StartTime);
//
//		if (LastTimeLeft - TimeLeft >= 600000)
//		{
//			LastTimeLeft = TimeLeft;
//
//			::DB.Disconnect();
//			::DB.Connect(::Settings.m_OpenDBAsReadOnly);
//
//			m_Update = TRUE;
//			UpdatePatientsList();
//		}
//
//		if (TimeLeft <= 0) break;
//
//		for (int i = 0; i < count; i++)
//		{
//			TestPatient = ::DB.m_Patients[i];
//
//			FullName = TestPatient.FullName();
//
//			if (FullName == "DB TEST")
//			{
//				Found1 = TRUE;
//				break;
//			}
//
//			if (FullName == "TEST DB")
//			{
//				Found2 = TRUE;
//				break;
//			}
//		}
//
//		if (Found1)
//		{
//			TestPatient.m_LastName = "DB";
//			TestPatient.m_FirstName = "TEST";
//
//			::DB.SavePatient(TestPatient);
//		}
//
//		if (Found2)
//		{
//			TestPatient.m_LastName = "TEST";
//			TestPatient.m_FirstName = "DB";
//
//			::DB.SavePatient(TestPatient);
//		}
//	}
//}

//***************************************************************************************

//show the reg time range dlg
void CMainWnd::OnRegButtonClicked()
{
	CString FromTime;
	CString ToTime;
	CRegtimeDlg* pDlg = new CRegtimeDlg(this, &FromTime, &ToTime);
	if (pDlg->DoModal() == IDOK)
	{
		m_RegFromTime = FromTime;
		m_RegToTime = ToTime;
		m_RegTimeButton.SetText(FromTime + " ~ " + ToTime);
		m_RegTimeButton.Repaint();
	}
	else
	{
		m_RegFromTime = "";
		m_RegToTime = "";
		m_RegTimeButton.SetText("");
		m_RegTimeButton.Repaint();
	}

	UpdatePatientsList();
	delete pDlg;
}

//***************************************************************************************

void CMainWnd::OnNewPatientButtonClicked()
{
	CPatient NewPatient;
	CEye OD, OS;

	if (!m_isValid)
	{
		::Error("Enter software activation key first.");
    m_PatientsList.SetFocus();
    return;
  }

	OD.m_Type = EYE_OD;
	OS.m_Type = EYE_OS;

	auto pDlg = std::make_unique<CPatientDlg>(this, &NewPatient, &OD, &OS);
	if (pDlg->DoModal() == IDOK)
	{
		::DB.SavePatient(NewPatient);

		if (pDlg->m_SaveOD)
		{
			OD.m_PatientID = NewPatient.m_PatientID;
			OD.m_Type = EYE_OD;

			::DB.SaveEye(OD);
		}

		if (pDlg->m_SaveOS)
		{
			OS.m_PatientID = NewPatient.m_PatientID;
			OS.m_Type = EYE_OS;

			::DB.SaveEye(OS);
		}

		m_Patient = NewPatient;

		UpdatePatientsList();
	}
}

//***************************************************************************************

void CMainWnd::OnSelectPatientButtonClicked()
{
	CString UniqueID = "";//PatientID for Dicom

	m_FellowIDCaled = FALSE;

	m_NoteImgInfo.ShowWindow(FALSE);
	m_ShowInfo = FALSE;

	CString Prefix = (m_Patient.m_Sex == 0 ? " ^ " : " ^ ");

	if (m_Level != LEVEL_MAIN) return;

	if (!m_isValid)
	{
		::Error("Enter software activation key first.");
    m_PatientsList.SetFocus();
    return;
  }

	int n = m_PatientsList.GetSelectedCount();
	if (n < 1)
	{
		::Error("No patient is selected.");
    m_PatientsList.SetFocus();
    return;
  }
	if (n > 1)
	{
		::Error("Please select one patient at a time.");
    m_PatientsList.SetFocus();
    return;
  }

	int m = m_PatientsList.GetSelectionMark();

	int p = (int)m_PatientsList.GetItemData(m);

	m_Patient = ::DB.m_Patients[p];

	// Save the used patient info to the Dicom info class from patient info
	if (m_Patient.m_MiddleName == "") m_Patient.m_MiddleName = " ";


	GUIDToStr(m_Patient.m_PatientID, UniqueID);//PatientID

	m_DicomInfo.m_Patient_Other_ID = UniqueID;

	m_DicomInfo.m_Patient_ID = ( m_Patient.m_CustomID == "" ? "unknown" : m_Patient.m_CustomID);

	m_DicomInfo.m_FileName = G_ReadChinese(m_Patient.m_FirstName) + "." + G_ReadChinese(m_Patient.m_LastName);

	m_DicomInfo.m_Patient_Name = m_Patient.m_LastName + "^" + m_Patient.m_FirstName;
	if (m_Patient.m_MiddleName != "")
	{

		m_DicomInfo.m_FileName = G_ReadChinese(m_Patient.m_FirstName) + "." + G_ReadChinese(m_Patient.m_MiddleName) + "." + G_ReadChinese(m_Patient.m_LastName);

		m_DicomInfo.m_Patient_Name += "^" + m_Patient.m_MiddleName;
	}

	m_DicomInfo.m_Patient_Sex = (m_Patient.m_Sex == SEX_FEMALE ? "F" : m_Patient.m_Sex == SEX_MALE ? "M" : "");
	m_DicomInfo.m_Patient_BirthDate.Format(_T("%04i%02i%02i"), m_Patient.m_BirthYear, m_Patient.m_BirthMonth, m_Patient.m_BirthDay);

	m_DicomInfo.m_StudyDes = "";

	m_DicomInfo.m_Modality = "OPM";
	m_DicomInfo.m_Conversion_Type = "SYN";

	m_DicomInfo.m_Equi_Manufacture_Model = "iTrace VFA";
	m_DicomInfo.m_Equi_Manufacturer = "Tracey Technologies";
	m_DicomInfo.m_Software_Versions = "6.2.0";

	//6.2.0 New Items here
	m_DicomInfo.m_Physician_Name = "";
	m_DicomInfo.m_Study_ID = "";
	m_DicomInfo.m_Series_Instance_UID = "";//can not be empty.
	m_DicomInfo.m_Series_Num = "";
	m_DicomInfo.m_Acess_Num = "";
	m_DicomInfo.m_Laterality = "";
	m_DicomInfo.m_Conversion_Type = "SYN"; // can not be empty.
	m_DicomInfo.m_Instance_Num = "";
	m_DicomInfo.m_Patient_Orientation = "";
	m_DicomInfo.m_Acess_Num = "";
	//New Items Done

	//Dicom info Done

	HideMainLevelControls();

	m_Last_evel = LAST_LEVEL_MAIN;
	m_Level = LEVEL_PATIENT;
	Invalidate(FALSE);

	ShowPatientLevelControls();

	UpdateExamsLists();
}

//***************************************************************************************

void CMainWnd::OnEditPatientButtonClicked()
{
	CPatient EditPatient;
	CEye OD, OS;

	if (!m_isValid)
	{
		::Error("Enter software activation key first.");
    m_PatientsList.SetFocus();
    return;
	}

	int n = m_PatientsList.GetSelectedCount();
	if (n < 1)
	{
		::Error("No patient is selected.");
    m_PatientsList.SetFocus();
    return;
  }

	if (n > 1)
	{
		//For merge patients
		CPasswordDlg* pDlg = new CPasswordDlg(this);
		int Res = pDlg->DoModal();
		CString Password = pDlg->m_Password;
		delete pDlg;

		CString s = "tracey";
		if (Res != IDOK || Password != s)
		{
      m_PatientsList.SetFocus();
      return;
    }

		if (::Question("Merge selected patients?", 1, 1))
		{
			CString s;
			s.Format(_T("%i"), n);
			s = " You are about to merge selected " + s;
			s += " patients and all of related exams from the database.";
			s += " After combination all the original patients will be removed from database.";
			s += " Are you sure?";

			if (::Question(s, 1, 1))
			{
				int m = m_PatientsList.GetSelectionMark();

				int p = (int)m_PatientsList.GetItemData(m);

				CPatient FirstPatient = ::DB.m_Patients[p];
				OD.m_Type = EYE_OD;
				OS.m_Type = EYE_OS;

				::DB.LoadEyes(FirstPatient.m_PatientID);

				for (int i = 0; i < ::DB.m_Eyes.GetSize(); i++)
				{
					switch (::DB.m_Eyes[i].m_Type)
					{
					case EYE_OD:
						OD = ::DB.m_Eyes[i];
						break;

					case EYE_OS:
						OS = ::DB.m_Eyes[i];
						break;
					}

				}

				CPatient EditPatients[100];

				int t = 0;
				POSITION Pos = m_PatientsList.GetFirstSelectedItemPosition();
				while (Pos)
				{
					int p = (int)m_PatientsList.GetItemData(m_PatientsList.GetNextSelectedItem(Pos));

					EditPatients[t] = ::DB.m_Patients[p];

					t++;
				}

				CPatient CombinedPatient;

				CPatientsMergeDlg* pDlg = new CPatientsMergeDlg(this, EditPatients, n, &OD, &OS, &CombinedPatient);

				if (pDlg->DoModal() == IDOK)
				{
					CBusyCursor Cursor;

					::DB.SavePatient(CombinedPatient);

					if (pDlg->m_SaveOD)
					{
						OD.m_PatientID = CombinedPatient.m_PatientID;
						OD.m_Type = EYE_OD;

						::DB.SaveEye(OD);
					}

					if (pDlg->m_SaveOS)
					{
						OS.m_PatientID = CombinedPatient.m_PatientID;
						OS.m_Type = EYE_OS;

						::DB.SaveEye(OS);
					}

					//Get the exams and point it to merged patient
					for (int i = 0; i < n; i++)
					{
						::DB.LoadExamHeaders(EditPatients[i].m_PatientID);//Big Database Research since 03062019
						//::DB.LoadTxtExamHeaders(EditPatients[i].m_PatientID);//Big Database Research since 03062019

						for (int e = 0; e < ::DB.m_ExamHeaders.GetSize(); e++)
						{
							CExamHeader* pExamHeader = &::DB.m_ExamHeaders[e];

							if (pExamHeader->m_Type == EXAM_TYPE_WF)
							{
								CWFExam *WFExam = new CWFExam;
								if (::DB.LoadWFExam(pExamHeader->m_ExamID, WFExam))
								{
									WFExam->m_Header.m_PatientID = CombinedPatient.m_PatientID;
									::DB.SaveWFExam(WFExam);
								}
							}
							else if (pExamHeader->m_Type == EXAM_TYPE_CT)
							{
								CCTExam* CTExam = new CCTExam;
								if (::DB.LoadCTExam(pExamHeader->m_ExamID, CTExam))
								{
									CTExam->m_Header.m_PatientID = CombinedPatient.m_PatientID;
									::DB.SaveCTExam(CTExam);
								}
							}
						}
					}
					//Get the exams and point it to merged patient Done

					m_Patient = CombinedPatient;

					//Delete old patients
					for (int i = 0; i < n; i++)
					{
						::DB.DeletePatient(EditPatients[i].m_PatientID);
					}
					//Delete old patients Done

					UpdatePatientsList();
				}

				delete pDlg;
			}
			else
			{
				::Error("Please select one patient at a time.");
			}
      m_PatientsList.SetFocus();
      return;
    }
		//For merge patients Done
		else
		{
			::Error("Please select one patient at a time.");
		}
    m_PatientsList.SetFocus();
    return;
  }

	int m = m_PatientsList.GetSelectionMark();

	int p = (int)m_PatientsList.GetItemData(m);

	EditPatient = ::DB.m_Patients[p];
	OD.m_Type = EYE_OD;
	OS.m_Type = EYE_OS;

	::DB.LoadEyes(EditPatient.m_PatientID);

	for (int i = 0; i < ::DB.m_Eyes.GetSize(); i++)
	{
		switch (::DB.m_Eyes[i].m_Type)
		{
		case EYE_OD:
			OD = ::DB.m_Eyes[i];
			break;

		case EYE_OS:
			OS = ::DB.m_Eyes[i];
			break;
		}
	}

	CPatientDlg* pDlg = new CPatientDlg(this, &EditPatient, &OD, &OS);

	if (pDlg->DoModal() == IDOK)
	{
		::DB.SavePatient(EditPatient);

		if (pDlg->m_SaveOD)
		{
			OD.m_PatientID = EditPatient.m_PatientID;
			OD.m_Type = EYE_OD;

			::DB.SaveEye(OD);
		}

		if (pDlg->m_SaveOS)
		{
			OS.m_PatientID = EditPatient.m_PatientID;
			OS.m_Type = EYE_OS;

			::DB.SaveEye(OS);
		}

		m_Patient = EditPatient;

		UpdatePatientsList();
	}

	delete pDlg;
}

//***************************************************************************************

void CMainWnd::OnDeletePatientButtonClicked()
{
	if (!m_isValid)
	{
		::Error("Enter software activation key first.");
    m_PatientsList.SetFocus();
    return;
  }

	int n = m_PatientsList.GetSelectedCount();
	if (n < 1)
	{
		::Error("No patient is selected.");
    m_PatientsList.SetFocus();
    return;
  }

	if (!::Question("Delete selected patients?", 1, 1))
	{
    m_PatientsList.SetFocus();
    return;
  }

	if (!::Question("You are about to remove selected patients\nand all of the related exams from the database.\nAre you sure?", 0, 1))
	{
    m_PatientsList.SetFocus();
    return;
  }

	POSITION Pos = m_PatientsList.GetFirstSelectedItemPosition();
	while (Pos)
	{
		int p = (int)m_PatientsList.GetItemData(m_PatientsList.GetNextSelectedItem(Pos));

		//601 06092015 for average exam deletation
		::DB.LoadExamHeaders(::DB.m_Patients[p].m_PatientID);	//Big Database Research since 03062019
		//::DB.LoadTxtExamHeaders(::DB.m_Patients[p].m_PatientID);//Big Database Research since 03062019

		for (int e = 0; e < ::DB.m_ExamHeaders.GetSize(); e++)
		{
			CExamHeader* pExamHeader = &::DB.m_ExamHeaders[e];

			if (pExamHeader->m_Type == EXAM_TYPE_WF)
			{
				CString ExamId, LoadFileName;
				GUIDToStr(pExamHeader->m_ExamID, ExamId);
				LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

				if (::PathFileExists(LoadFileName))
				{
					DeleteFile(LoadFileName);
				}
			}
		}
		//601 06092015 for average exam deletation Done

		////Big Database Research since 03062019
		//CString PatientIDStr;

		//GUIDToStr(::DB.m_Patients[p].m_PatientID, PatientIDStr);

		//CString FolderName;
		//::DB.GetFolderName(FolderName);

		//CString HeadFileName = FolderName + "\\ExamHeader\\" + PatientIDStr + ".csv";
		//if (::PathFileExists(HeadFileName))
		//{
		//	DeleteFile(HeadFileName);
		//}
		////Big Database Research since 03062019

		::DB.DeletePatient(::DB.m_Patients[p].m_PatientID);
	}

	m_Patient.Reset();

	UpdatePatientsList();
}

//***************************************************************************************

//New Dicom
//read dicom from MWL (Modality worklist) see '//dicomiseasy.blogspot.com/2012/04/dicom-modality-worklist.html'
// and '//dicomiseasy.blogspot.com/2011/12/introduction-to-dicom-chapter-5-solving.html'
BOOL  CMainWnd::ReadMWLInfo()
{
	CDicomMWLRecDlg* pMWLRecDlg = new CDicomMWLRecDlg(this);

	if (pMWLRecDlg->DoModal() == IDOK)
	{
		BOOL OK = FALSE;

		int n = pMWLRecDlg->m_RecCount;

		if (n == 0) return FALSE;



		//Test
		if (n > 1)
		{
			CDicomMWLResDlg* pMWLResDlg = new CDicomMWLResDlg(this);

			pMWLResDlg->m_RecCount = n;

			for (int i = 0; i < n; i++)
			{
				pMWLResDlg->m_DicomInfo[i] = pMWLRecDlg->m_DicomInfo[i];
			}

			if (pMWLResDlg->DoModal() == IDOK)
			{
				for (int k = 0; k < n; k++)
				{
					pMWLRecDlg->m_SelDicom[k] = pMWLResDlg->m_SelDicom[k];
				}
			}
			else
			{
				delete pMWLResDlg;
				delete pMWLRecDlg;
				return FALSE;
			}

			delete pMWLResDlg;
		}
		//Test

		for (int i = 0; i < n; i++)
		{
			if (pMWLRecDlg->m_SelDicom[i])
			{
				CString nStr;
				nStr.Format(_T("%i"), i + 1);

				//Check duplicate(Same GUID) patient
				CPatient NewPatient = ReadFromDicomInfo(pMWLRecDlg->m_DicomInfo[i]);

				int pos;
				BOOL Exist = CheckSameMWLPatient(NewPatient, pos);
				//Check duplicate(Same GUID) patient Done

				if (Exist)
				{
					m_ImportID = NewPatient.m_PatientID;

					::Info(" Patient '" + NewPatient.FullName() + "' is already in the current iTrace patient database.");
				}
				else
				{
					CPatient NewPatient;
					NewPatient = ReadFromDicomInfo(pMWLRecDlg->m_DicomInfo[i]);
					::DB.SavePatient(NewPatient);

					m_ImportID = NewPatient.m_PatientID;

					::Info(" Patient '" + NewPatient.FullName() + "' has been merged to the current patient database.");
				}
			}
		}

		UpdatePatientsList();

		delete pMWLRecDlg;
		return TRUE;
	}
	else
	{
		delete pMWLRecDlg;
		return FALSE;
	}
}
//New Dicom

//***************************************************************************************

void CMainWnd::OnImportPatientButtonClicked()
{
	//6.2.0
	int formatType = 0;//0:zip; 1:Dicom
	CString ImportFormat = "zip";

	CINIParser IniPar;

	if (m_isComboDicom)
	{
		ImportFormat = IniPar.GetStrKey(IniSection, "Import Format", IniPath);	//6.2.0 ini registration

		if (ImportFormat == "zip") formatType = 0;//The tracey zip file
		else formatType = 1;//Dicom
	}
	//6.2.0

	if (!m_isValid)
	{
		::Error("Enter software activation key first.");
    IniPar.SetStrKey(IniSection, "Import Format", ImportFormat, IniPath); // 6.2.0 ini registration
    m_PatientsList.SetFocus();
    return;
  }

	int TotPatCount = ::DB.m_Patients.GetSize();
	if (m_isDemo)
	{
		if (TotPatCount >= 30)
		{
			::Info("The Tracey Demo data is limited to 30 patients.");
			{
				m_ImportPatientButton.SetEnabled(FALSE);
				m_ImportPatientButton.Repaint();

				IniPar.SetStrKey(IniSection, "OMED", "1", IniPath);	//6.2.0 ini registration
			}
			return;
		}
	}

	char* pFileNameListBuffer = new char[5000 * MAX_PATH];//change from 1024 to 5000 * MAX_PATH, allow import maxmium 5000 patients
	memset(pFileNameListBuffer, 0, 5000 * MAX_PATH);//change from 1024 to 5000 * MAX_PATH, allow import maxmium 5000 patients

	m_ImportID = GUID_NULL;

	CFileDialog* pDlg;

	//6.2.0
	if (m_isComboDicom)
	{
		switch (formatType)
		{
		case 0:

			pDlg = new CFileDialog(TRUE, _T("zip"), _T(""), OFN_HIDEREADONLY, _T("Zip files (*.zip)|*.zip|DICOM (*.dcm)|*.dcm||"), this);//6.2.0 add dicom file
			break;
		case 1:
			pDlg = new CFileDialog(TRUE, _T("dcm"), _T(""), OFN_HIDEREADONLY, _T("DICOM (*.dcm)|*.dcm|Zip files (*.zip)|*.zip||"), this);//6.2.0 add dicom file
			break;
		}
	}
	else
	{
		pDlg = new CFileDialog(TRUE, _T("zip"), _T(""), OFN_HIDEREADONLY, _T("Zip files (*.zip)|*.zip||"), this);//6.2.0 add dicom file
	}
	//6.2.0

	//
	pDlg->m_ofn.Flags |= OFN_ALLOWMULTISELECT;
	pDlg->m_ofn.lpstrTitle = _T("Import patients");
	pDlg->m_ofn.lpstrInitialDir = MAIN_DIR + _T("\\Export");
	pDlg->m_ofn.lpstrFile = (LPWSTR)pFileNameListBuffer;
	pDlg->m_ofn.nMaxFile = 2000 * MAX_PATH;//change from 1024 to 5000 * MAX_PATH, allow import maxmium 5000 patients
	if (pDlg->DoModal() == IDOK)
	{
		CBusyCursor Cursor;
		CString s, fileFormatStr;
		int ns = 0;
		int nf = 0;

		POSITION Pos = pDlg->GetStartPosition();
		while (Pos)
		{
			s = pDlg->GetNextPathName(Pos);

			//read dicom file
			fileFormatStr = s.Right(3);
			ImportFormat = fileFormatStr;//6.2.0

			if (fileFormatStr == "dcm")
			{
				CTraceyDicom TraceyDicom;
				CDicomInfo DicomInfo;

				int Len = s.GetLength();

				if (G_IsChinese(s))
				{
					wchar_t *pValue = new wchar_t[Len];
					wcscpy(pValue, s);

					TraceyDicom.InputChinseeDicom(pValue, DicomInfo);
				}
				else
				{
					char *pValue = new char[Len];

					G_As = s;

					pValue = G_As.GetBuffer(Len);

					TraceyDicom.InputDicom(pValue, DicomInfo);
				}

				//Check duplicate(Same GUID) patient
				CPatient NewPatient = ReadFromDicomInfo(DicomInfo);

				int pos;
				BOOL Exist = CheckSameDCMPatient(NewPatient, DicomInfo.m_Patient_Other_ID, pos);
				//Check duplicate(Same GUID) patient Done

				if (Exist)
				{
					if (pos != -1)
					{
						//Show
						CPatient OldPatient = ::DB.m_Patients[pos];

						BOOL Same = SameInfo(NewPatient, OldPatient);

						if (Same)
						{
							OldPatient.m_IsDicom = TRUE;

							if (NewPatient.m_Note != "")
							{
								CString NewNote = "[Dicom:" + NewPatient.m_Note + "]";

								CString OldNote = OldPatient.m_Note;
								OldNote.Replace(_T("*"), _T(""));
								int count = OldNote.Find(NewNote, 0);

								if (count == -1)
								{
									OldPatient.m_Note += NewNote;
								}
							}
							::DB.SavePatient(OldPatient);
							::Info(" Patient '" + NewPatient.FullName() + "' has been merged to the current patient data.");
						}
						else
						{
							CString	s = G_ReadChinese(OldPatient.m_LastName) + "." + G_ReadChinese(OldPatient.m_FirstName);

							if (OldPatient.m_MiddleName != "")
							{
								s = G_ReadChinese(OldPatient.m_LastName) + "." + G_ReadChinese(OldPatient.m_MiddleName) + "." + G_ReadChinese(OldPatient.m_FirstName);
							}

							s = "Patient '" + G_ReadChinese(OldPatient.FullName()) + "' has same unique identification as imported patient '";
							s += G_ReadChinese(NewPatient.FullName());
							s += "'. Do you wish to update the patient's demographic information from the imported file?";
							s += " If you select no, the iTrace database will not be updated with the import file information.";

							if (::Question(s, 0, 1))
							{
								CSameDicomPatientDlg* pinDlg = new CSameDicomPatientDlg(this, &NewPatient, &OldPatient);

								if (pinDlg->DoModal() == IDOK)
								{
									::DB.SavePatient(NewPatient);
									::Info(" Patient '" + NewPatient.FullName() + "' has been merged to the current patient data.");
								}

								delete pinDlg;
							}
						}

						m_ImportID = NewPatient.m_PatientID;
					}
				}
				else
				{
					CPatient NewPatient;
					NewPatient = ReadFromDicomInfo(DicomInfo);
					::DB.SavePatient(NewPatient);

					m_ImportID = NewPatient.m_PatientID;
				}
			}
			//read dicom file done
			else
			{
				if (::DB.Import(s))
				{
					if (ns == 0) m_ImportID = GetIDFromS(s);

					ns++;

					if (m_isDemo)
					{
						TotPatCount++;
						if (TotPatCount >= 30)
						{
							::Info("The Tracey Demo data is limited to 30 patients.");
							break;
						}
					}
				}
				else
				{
					nf++;
				}
			}
		}

		UpdatePatientsList();

		m_ImportID = GUID_NULL;

		//Save the patient id to demo date exc.zip file
		if (m_isDemo)
		{
			CDEMOID DemoData;
			CString DemoFileName = "C:\\Tracey\\Data\\MSJetEx\\exc.zip";

			int count = ::DB.m_Patients.GetSize();

			for (int i = 0; i < 30; i++)
			{
				if (i < count) DemoData.PatientID[i] = ::DB.m_Patients[i].m_PatientID;
				else DemoData.PatientID[i] = GUID_NULL;
			}

			DemoData.SaveIntoDemoFile(DemoFileName);

			if (count == 30)
			{
				m_ImportPatientButton.SetEnabled(FALSE);
				m_ImportPatientButton.Repaint();

				CINIParser IniPar;
				IniPar.SetStrKey(IniSection, "OMED", "1", IniPath);	//6.2.0 ini registration
			}
		}
		//Save the patient id to demo date exc.zip file Done

		if (ns >= 1)
		{
			s.Format(_T("Successully imported %i patient%s."), ns, ns == 1 ? "" : "s");
			::Info(s);
		}
		if (nf >= 1)
		{
			s.Format(_T("Failed to import %i patient%s."), nf, nf == 1 ? "" : "s");
			::Error(s);
		}
	}

	delete pDlg;

	delete[] pFileNameListBuffer;
}

//***************************************************************************************

void CMainWnd::OnExportPatientButtonClicked()
{
	CBusyCursor Cursor;
	CString ExportFolderName, s;

	if (!m_isValid)
	{
		::Error("Enter software activation key first.");
    m_PatientsList.SetFocus();
    return;
  }

	int n = m_PatientsList.GetSelectedCount();
	if (n < 1)
	{
		::Error("No patient is selected.");
    m_PatientsList.SetFocus();
    return;
  }

	ExportFolderName = MAIN_DIR + "\\Export";

	::CreateDirectory(ExportFolderName, NULL);

	CFolderDlg::m_InitialFolderName = ExportFolderName;

	CFolderDlg* pDlg = new CFolderDlg(this, "");

	if (pDlg->Show())
	{
		ExportFolderName = pDlg->m_FolderName;
	}
	else
	{
		ExportFolderName.Empty();
	}
	delete pDlg;

  if (!::PathFileExists(ExportFolderName)) {
    m_PatientsList.SetFocus();
    return;
  }


	POSITION Pos = m_PatientsList.GetFirstSelectedItemPosition();
	int ns = 0;
	int nf = 0;
	while (Pos)
	{
		int p = (int)m_PatientsList.GetItemData(m_PatientsList.GetNextSelectedItem(Pos));
		if (::DB.ExportPatient(::DB.m_Patients[p].m_PatientID, ExportFolderName))
		{
			ns++;
		}
		else
		{
			nf++;
		}
	}

	if (ns >= 1)
	{
		s.Format(_T("Successully exported %i patient%s."), ns, ns == 1 ? "" : "s");
		::Info(s);
	}
	if (nf >= 1)
	{
		s.Format(_T("Failed to export %i patient%s."), nf, nf == 1 ? "" : "s");
		::Error(s);
	}
}

//***************************************************************************************

void CMainWnd::OnSettingsButtonClicked()
{
	if (!m_isValid)
	{
		::Error("Enter software activation key first.");
    m_PatientsList.SetFocus();
    return;
  }

	uchar DateFormat = ::Settings.m_DateFormat;

	CDialog* pDlg;
	if (m_isSolo)
		pDlg = new CSettingsSoloDlg(this);
	else
	{
		if (m_isCombo)//531
			pDlg = new CSettingsComboDlg(this);
		else
			pDlg = new CSettingsDlg(this);
	}

	if (pDlg->DoModal() == IDOK)
	{
		//to make sure the default will change the setting of exam list collum width
		int W = ::GetSystemMetrics(SM_CXSCREEN);
		real_t w2[11] = { ::NewSettings.WFOD_COL_Width[0] * W, ::NewSettings.WFOD_COL_Width[1] * W,
			::NewSettings.WFOD_COL_Width[2] * W, ::NewSettings.WFOD_COL_Width[3] * W,
			::NewSettings.WFOD_COL_Width[4] * W, ::NewSettings.WFOD_COL_Width[5] * W,
			::NewSettings.WFOD_COL_Width[6] * W, ::NewSettings.WFOD_COL_Width[7] * W,
			::NewSettings.WFOD_COL_Width[8] * W, ::NewSettings.WFOD_COL_Width[9] * W,
			::NewSettings.WFOD_COL_Width[10] * W };

		for (int i = 0; i < 11; i++)
		{
			if (i < 10)
			{
				m_WFODList.SetColumnWidth(i, intRound(w2[i]));
				m_WFOSList.SetColumnWidth(i, intRound(w2[i]));
			}
			m_CTODList.SetColumnWidth(i, intRound(w2[i]));
			m_CTOSList.SetColumnWidth(i, intRound(w2[i]));
		}
		m_WFODList.SetColumnWidth(10, intRound(w2[2] + 10));
		m_WFODList.SetColumnWidth(11, intRound(w2[10] - w2[2] - 15));

		m_WFOSList.SetColumnWidth(10, intRound(w2[2] + 10));
		m_WFOSList.SetColumnWidth(11, intRound(w2[10] - w2[2] - 15));


		if (DateFormat != ::Settings.m_DateFormat)
		{
			UpdatePatientsList();
		}
		Invalidate(FALSE);
	}
	delete pDlg;
}

//***************************************************************************************

//First activation, used to change the licensing
void  CMainWnd::ChangeLicese()
{
	//Version type
	m_isCombo = (::Licensing.IsCombo() || ::Licensing.IsComboViewer() || ::Licensing.IsComboDICOM() || ::Licensing.IsPresbia() ? TRUE : FALSE);
	m_isHoya = (::Licensing.IsWorkstation() || ::Licensing.IsViewer() || ::Licensing.IsDemo() ? TRUE : FALSE);
	m_isSolo = (::Licensing.IsSolo() ? TRUE : FALSE);
	m_isDemo = (::Licensing.IsDemo() ? TRUE : FALSE);;//531
	m_isViewer = (::Licensing.IsComboViewer() || ::Licensing.IsViewer() ? TRUE : FALSE);;//531
	m_isComboDicom = ::Licensing.IsComboDICOM();//620
	m_isValid = (::Licensing.IsValid() ? TRUE : FALSE);;//531
	m_isPresbia = (::Licensing.IsPresbia() ? TRUE : FALSE);;//531
	m_isComboViewer = (::Licensing.IsComboViewer() ? TRUE : FALSE);;//531
	m_LicenseName = ::Licensing.LicenseName();
	//Version type Done

	//adjust button location
	if (!m_isValid) return;

	LPCRECT prct1, prct2, prct3, prct4, prct5, prct6, prct7, prct1a, prct4a, prct5a;

	prct1a = &m_BR[0][9];
	prct4a = &m_BR[0][7];
	prct5a = &m_BR[0][8];

	if (m_isHoya)
	{
		prct1 = &m_BR[0][9];
		prct2 = &m_BR[0][5];
		prct3 = &m_BR[0][6];
		prct4 = &m_BR[0][7];
		prct5 = &m_BR[0][8];
		prct6 = &m_BR[0][9];
		prct7 = &m_BR[0][10];
	}

	if (m_isCombo)
	{
		prct1 = &m_BR[0][8];
		prct2 = &m_BR[0][4];
		prct3 = &m_BR[0][5];
		prct4 = &m_BR[0][6];
		prct5 = &m_BR[0][7];
		prct6 = &m_BR[0][8];
		prct7 = &m_BR[0][9];
	}

	if (!m_isSolo)
	{
		m_ExportExcelButton.ShowWindow(SW_SHOW);
		m_SettingsButton.MoveWindow(prct4a, 1);
		m_LicenseButton.MoveWindow(prct5a, 1);
		m_TestButton.MoveWindow(prct1a, 1);

		m_WFSumButton[2].MoveWindow(prct1, 1);
		m_INSumButton[1].MoveWindow(prct2, 1);
		m_INSumButton[2].MoveWindow(prct3, 1);
		m_INSumButton[3].MoveWindow(prct4, 1);
		m_INSumButton[4].MoveWindow(prct5, 1);
		m_INSumButton[5].MoveWindow(prct6, 1);
		m_INSumButton[6].MoveWindow(prct7, 1);
	}
	else
	{
		m_SettingsButton.MoveWindow(&m_BR[0][6], 1);
		m_LicenseButton.MoveWindow(&m_BR[0][7], 1);
		m_TestButton.MoveWindow(&m_BR[0][8], 1);

		m_SettingsButton.ShowWindow(SW_SHOW);
		m_LicenseButton.ShowWindow(SW_SHOW);
		m_TestButton.ShowWindow(SW_SHOW);

		m_ExportExcelButton.ShowWindow(SW_HIDE);
	}

	//Reset the setting
	for (int i = 0; i < 8; i++)
	{
		::NewSettings.m_WFSumShow[i] = TRUE;
		::NewSettings.m_CTSumShow[i] = TRUE;
		::NewSettings.m_WFCTSumShow[i] = TRUE;
	}
	if (m_isCombo)
	{
		::NewSettings.m_WFSumShow[7] = FALSE;
		::NewSettings.m_CTSumShow[7] = FALSE;
		::NewSettings.m_WFCTSumShow[7] = FALSE;
		::NewSettings.m_WFCTSumShow[8] = FALSE;
	}

	::Settings.m_DefaultWFSum = 11;
	::Settings.m_DefaultCTSum = 11;
	::Settings.m_DefaultINSum = 6;

	SetDef_TaskSettings();
	//Reset the setting Done


	::DB.Disconnect();
	::DB.Connect(::Settings.m_OpenDBAsReadOnly);
	UpdatePatientsList();

	if (m_isDemo)
	{
		m_NewPatientButton.SetEnabled(FALSE);
		m_EditPatientButton.SetEnabled(FALSE);
		m_DeletePatientButton.SetEnabled(FALSE);
		m_NewPatientButton.Repaint();
		m_EditPatientButton.Repaint();
		m_DeletePatientButton.Repaint();

		//Import button
		CINIParser IniPar;
		int Type2 = IniPar.GetIntKey(IniSection, "OMED", IniPath);	//6.2.0 ini registration

		if (Type2 == 1)
		{
			m_ImportPatientButton.SetEnabled(FALSE);
			m_ImportPatientButton.Repaint();
			return;
		}
		//Import button Done
	}
	else
	{
		m_NewPatientButton.SetEnabled(TRUE);
		m_EditPatientButton.SetEnabled(TRUE);
		m_DeletePatientButton.SetEnabled(TRUE);
		m_ImportPatientButton.SetEnabled(TRUE);
		m_ImportPatientButton.Repaint();
		m_NewPatientButton.Repaint();
		m_EditPatientButton.Repaint();
		m_DeletePatientButton.Repaint();
	}
}

//***************************************************************************************

void CMainWnd::OnLicenseButtonClicked()
{
	int OriType = ::Licensing.m_LicenseType;
	BOOL OriIsCombo = m_isCombo;

	CStatusDlg* pDlg = new CStatusDlg(this);
	pDlg->DoModal();
	delete pDlg;


	int CurType = ::Licensing.m_LicenseType;

	if (CurType == OriType)
	{
		return;
	}

	m_isCombo = (::Licensing.IsCombo() || ::Licensing.IsComboViewer() || ::Licensing.IsComboDICOM() || ::Licensing.IsPresbia() ? TRUE : FALSE);
	m_isHoya = (::Licensing.IsWorkstation() || ::Licensing.IsViewer() || ::Licensing.IsDemo() ? TRUE : FALSE);
	m_isSolo = (::Licensing.IsSolo() ? TRUE : FALSE);
	m_isDemo = (::Licensing.IsDemo() ? TRUE : FALSE);;//531
	m_isViewer = (::Licensing.IsComboViewer() || ::Licensing.IsViewer() ? TRUE : FALSE);;//531
	m_isComboDicom = ::Licensing.IsComboDICOM();
	m_isValid = (::Licensing.IsValid() ? TRUE : FALSE);;//531
	m_isPresbia = (::Licensing.IsPresbia() ? TRUE : FALSE);;//531
	m_isComboViewer = (::Licensing.IsComboViewer() ? TRUE : FALSE);;//531
	m_LicenseName = ::Licensing.LicenseName();

	//[adjust button
	if (!m_isValid) return;

	LPCRECT prct1, prct2, prct3, prct4, prct5, prct6, prct7, prct1a, prct4a, prct5a;

	prct1a = &m_BR[0][9];
	prct4a = &m_BR[0][7];
	prct5a = &m_BR[0][8];

	if (m_isHoya)
	{
		prct1 = &m_BR[0][9];
		prct2 = &m_BR[0][5];
		prct3 = &m_BR[0][6];
		prct4 = &m_BR[0][7];
		prct5 = &m_BR[0][8];
		prct6 = &m_BR[0][9];
		prct7 = &m_BR[0][10];
	}

	if (m_isCombo)
	{
		prct1 = &m_BR[0][8];
		prct2 = &m_BR[0][4];
		prct3 = &m_BR[0][5];
		prct4 = &m_BR[0][6];
		prct5 = &m_BR[0][7];
		prct6 = &m_BR[0][8];
		prct7 = &m_BR[0][9];
	}

	if (!m_isSolo)
	{
		m_ExportExcelButton.ShowWindow(SW_SHOW);
		m_SettingsButton.MoveWindow(prct4a, 1);
		m_LicenseButton.MoveWindow(prct5a, 1);
		m_TestButton.MoveWindow(prct1a, 1);

		m_WFSumButton[2].MoveWindow(prct1, 1);
		m_INSumButton[1].MoveWindow(prct2, 1);
		m_INSumButton[2].MoveWindow(prct3, 1);
		m_INSumButton[3].MoveWindow(prct4, 1);
		m_INSumButton[4].MoveWindow(prct5, 1);
		m_INSumButton[5].MoveWindow(prct6, 1);
		m_INSumButton[6].MoveWindow(prct7, 1);
	}
	else
	{
		m_SettingsButton.MoveWindow(&m_BR[0][6], 1);
		m_LicenseButton.MoveWindow(&m_BR[0][7], 1);
		m_TestButton.MoveWindow(&m_BR[0][8], 1);

		m_SettingsButton.ShowWindow(SW_SHOW);
		m_LicenseButton.ShowWindow(SW_SHOW);
		m_TestButton.ShowWindow(SW_SHOW);

		m_ExportExcelButton.ShowWindow(SW_HIDE);
	}

	//Reset the setting
	if ((!OriIsCombo && m_isCombo) || (OriIsCombo && !m_isCombo))
	{
		for (int i = 0; i < 8; i++)
		{
			::NewSettings.m_WFSumShow[i] = TRUE;
			::NewSettings.m_CTSumShow[i] = TRUE;
			::NewSettings.m_WFCTSumShow[i] = TRUE;
		}
		if (m_isCombo)
		{
			::NewSettings.m_WFSumShow[7] = FALSE;
			::NewSettings.m_CTSumShow[7] = FALSE;
			::NewSettings.m_WFCTSumShow[7] = FALSE;
			::NewSettings.m_WFCTSumShow[8] = FALSE;
		}

		::Settings.m_DefaultWFSum = 11;
		::Settings.m_DefaultCTSum = 11;
		::Settings.m_DefaultINSum = 6;

		SetDef_TaskSettings();
	}
	//Reset the setting Done

	if ((OriType != LT_DEMO && m_isDemo) || (OriType == LT_DEMO && !m_isDemo))
	{
		::DB.Disconnect();
		::DB.Connect(::Settings.m_OpenDBAsReadOnly);
		UpdatePatientsList();

		if (m_isDemo)
		{
			m_NewPatientButton.SetEnabled(FALSE);
			m_EditPatientButton.SetEnabled(FALSE);
			m_DeletePatientButton.SetEnabled(FALSE);
			m_NewPatientButton.Repaint();
			m_EditPatientButton.Repaint();
			m_DeletePatientButton.Repaint();

			//Import button
			CINIParser IniPar;
			int Type2 = IniPar.GetIntKey(IniSection, "OMED", IniPath);	//6.2.0 ini registration

			if (Type2 == 1)
			{
				m_ImportPatientButton.SetEnabled(FALSE);
				m_ImportPatientButton.Repaint();
				return;
			}
			//Import button Done
		}
		else
		{
			m_NewPatientButton.SetEnabled(TRUE);
			m_EditPatientButton.SetEnabled(TRUE);
			m_DeletePatientButton.SetEnabled(TRUE);
			m_ImportPatientButton.SetEnabled(TRUE);
			m_ImportPatientButton.Repaint();
			m_NewPatientButton.Repaint();
			m_EditPatientButton.Repaint();
			m_DeletePatientButton.Repaint();
		}
	}

	//6.2.0 Check RZDCX Activation
	if (m_isComboDicom)
	{
		CINIParser IniPar;

		CString RzdcxAct = IniPar.GetStrKey(IniSection, "RZDCX ACT", IniPath);	//6.2.0 ini registration

		if (RzdcxAct == "0")
		{
		A:	::Info("The Dicom Toolkit needs to be activated for COMBO DICOM Version.");

			////-----------------------------------------Registration
			//SHELLEXECUTEINFO ShExecInfo1 = { 0 };
			//ShExecInfo1.cbSize = sizeof(SHELLEXECUTEINFO);
			//ShExecInfo1.fMask = SEE_MASK_NOCLOSEPROCESS;
			//ShExecInfo1.hwnd = NULL;
			//ShExecInfo1.lpVerb = _T("open");
			//ShExecInfo1.lpFile = _T("C:\\RZDCX\\Activation\\RegistrationTool.exe");
			//ShExecInfo1.lpParameters = _T("");
			//ShExecInfo1.lpDirectory = NULL;
			//ShExecInfo1.nShow = SW_SHOW;
			//ShExecInfo1.hInstApp = NULL;

			//ShellExecuteEx(&ShExecInfo1);

			//WaitForSingleObject(ShExecInfo1.hProcess, INFINITE);

			//-----------------------------------------------------------
			SHELLEXECUTEINFO ShExecInfo = { 0 };
			ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
			ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
			ShExecInfo.hwnd = NULL;
			ShExecInfo.lpVerb = _T("open");
			ShExecInfo.lpFile = _T("C:\\RZDCX\\Activation\\ActivationTool.exe");
			ShExecInfo.lpParameters = _T("");
			ShExecInfo.lpDirectory = NULL;
			ShExecInfo.nShow = SW_SHOW;
			ShExecInfo.hInstApp = NULL;

			ShellExecuteEx(&ShExecInfo);

			WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

			//Check the RZDXC.License file and copy it
			if (G_Is64BitOS())
			{
				CString File64Name = "C:\\RZDCX\\Win64\\RZDCX.license";

				if (::PathFileExists(File64Name))
				{
					CString DesFileName = "C:\\RZDCX\\RZDCX.license";

					CopyFile(File64Name, DesFileName, TRUE);
				}
			}
			else
			{
				CString File32Name = "C:\\RZDCX\\Win32\\RZDCX.license";

				if (::PathFileExists(File32Name))
				{
					CString DesFileName = "C:\\RZDCX\\RZDCX.license";

					CopyFile(File32Name, DesFileName, TRUE);
				}
			}
			//Done

			//Check the activation infomation
			::Info("Prgoram will run the Dicom activation test. ");//If the activation is not done, the program will be terminated.

			CTraceyDicom TraceyDicom;
			if (TraceyDicom.TestActivation())
			{
				IniPar.SetStrKey(IniSection, "RZDCX ACT", "1", IniPath);
				::Info("Dicom test passed. The Dicom toolkit has been activated in your computer.");
			}
			else
			{
				::Info("Dicom test failed. The Dicom toolkit has not been activated in your computer. Please cotact producer.");
			}
			//Done
		}
		else
		{
			CTraceyDicom TraceyDicom;

			if (!TraceyDicom.TestActivation())
			{
				IniPar.SetStrKey(IniSection, "RZDCX ACT", "0", IniPath);
				::Info("Dicom test failed.");
				goto A;
			}
		}
	}
	//6.2.0

	Invalidate(FALSE);
	m_PatientsList.SetFocus();
}

//***************************************************************************************

void CMainWnd::OnTestButtonClicked()
{
	if (!::Question("Do you want to check the instrument?\nIf so setup the test object and click Yes.", 0, 1))
	{
    m_PatientsList.SetFocus();
    return;
  }

	CWFTest* pTest = new CWFTest(&::HW);
	pTest->Test();

	CTestResultDlg* pDlg = new CTestResultDlg(this, pTest->m_Res, pTest->m_SphEq, pTest->m_Sph, pTest->m_Cyl, pTest->m_Axis);
	pDlg->DoModal();
	delete pDlg;

	delete pTest;
}

//***************************************************************************************
//520 export all patients exam to excel file
void CMainWnd::OnExcelButtonClicked()
{
	if (!m_isValid)
	{
		::Error("Enter software activation key first.");
		return;
	}

	int n = m_PatientsList.GetSelectedCount();
	if (n < 1)
	{
		::Error("No patient is selected.");
		return;
	}

	CBusyCursor Cursor;

	if (::Settings.m_EXCEL_WFCT_SETS[0] != '1' && ::Settings.m_EXCEL_WFCT_SETS[0] != '0')
	{
		CString s;
		s = "The spreadsheet settings is not set yet.  \nSet the spreadsheet now?  \n\n(You can also set the spreadsheet settings in the Settings dialog.)";
		if (::Question(s, 1, 0))
		{
			CExcelSettingsDlg2* pDlg = new CExcelSettingsDlg2(this, FALSE);

			if (pDlg->DoModal() == IDOK)
			{
			}
			delete pDlg;
		}
		else  return;
	}

	//Save the exam info into excel file
	CString FolderName, Name1, Name2, Name3;

	CINIParser IniPar;
	FolderName = IniPar.GetStrKey(IniSection, "Export Excel Folder", IniPath);	//6.2.0 ini registration

	Name1 = FolderName + "\\620PatientsWFExams.xlsx";
	Name2 = FolderName + "\\620PatientsCTExams.xlsx";
	Name3 = FolderName + "\\620PatientsWFCTExams.xlsx";

	//Show the exporting info
	POSITION Pos = m_PatientsList.GetFirstSelectedItemPosition();
	int ECount = 0;
	while (Pos)
	{
		int p = (int)m_PatientsList.GetItemData(m_PatientsList.GetNextSelectedItem(Pos));

		CPatient* pPatient = &::DB.m_Patients[p];

		::DB.LoadExamHeaders(pPatient->m_PatientID);//Big Database Research since 03062019
		//::DB.LoadTxtExamHeaders(pPatient->m_PatientID);//Big Database Research since 03062019

		ECount += ::DB.m_ExamHeaders.GetSize();
	}

	BOOL ShowExpInfo = TRUE;

	CString  s1, s2;
	CString  sPCount, sECount;
	CMFont   Info1Font, Info2Font;
	COLORREF InfoBkColor = 0x00efdfcf;
	COLORREF ProBlockBkColor = 0x00efffdf;
	COLORREF ProcessingBkColor = 0x00d0d0d0;
	RECT     ExportingRect, Info1Rect, Info2Rect, RestRect, ProcessRect,
			 ProcessingRect, OutLineRect, Shadow1Rect, Shadow2Rect, Shadow3Rect, Shadow4Rect;

	if (ECount < 5)
	{
		ShowExpInfo = FALSE;
	}

	int Step, ProVal, StepVal;

	CImgInfoWnd ExportingInfo;

	if (ShowExpInfo)
	{
		if (ECount > 100)
		{
			real_t div = (real_t)ECount / 100.0;
			Step = intRound(div);
		}
		else
		{
			real_t div = 100.0 / (real_t)ECount;
			Step = intRound(div);
		}

		::SetRect(&ExportingRect, 3000, 2000, 3400, 2200);
		::SetRect(&Info1Rect, 5, 0, 400, 40);
		::SetRect(&Info2Rect, 50, 60, 400, 100);
		::SetRect(&RestRect, 5, 50, 400, 195);
		::SetRect(&ProcessRect, 50, 120, 350, 140);
		::SetRect(&OutLineRect, 5, 0, 400, 195);
		::SetRect(&Shadow1Rect, 0, 5, 5, 200);
		::SetRect(&Shadow2Rect, 0, 195, 395, 200);
		::SetRect(&Shadow3Rect, 0, 0, 5, 5);
		::SetRect(&Shadow4Rect, 395, 195, 400, 200);

		ExportingInfo.CreateWnd(ExportingRect, this);

		sPCount.Format(_T("%i"), n);
		sECount.Format(_T("%i"), ECount);

		CString ps = "Patients";
		if (n <= 1) ps = "Patient";
		CString es = "Exams";
		if (ECount <= 1) es = "Exam";

		s1 = "Exporting " + sPCount + " " + ps + " with " + sECount + " " + es + " to Excel spreadsheet";
		s2 = sPCount + " " + ps + " with " + sECount + " " + es + " remaining";

		Info1Font.Create(intRound(0.8* (20)), 400, "Arial");
		Info2Font.Create(intRound(0.9* (20)), 400, "Arial");

		ExportingInfo.m_MemDC.FillSolidRect(&Shadow1Rect, 0x00cccccc);
		ExportingInfo.m_MemDC.FillSolidRect(&Shadow2Rect, 0x00cccccc);
		ExportingInfo.m_MemDC.FillSolidRect(&Shadow3Rect, WHITE);
		ExportingInfo.m_MemDC.FillSolidRect(&Shadow4Rect, WHITE);
		ExportingInfo.m_MemDC.FillSolidRect(&Info1Rect, InfoBkColor);
		ExportingInfo.m_MemDC.FillSolidRect(&RestRect, ProBlockBkColor);
		ExportingInfo.m_MemDC.FillSolidRect(&ProcessRect, ProcessingBkColor);
		ExportingInfo.m_MemDC.WriteText(s1, Info1Rect, Info1Font, BLACK, 1, NOCOLOR);
		ExportingInfo.m_MemDC.WriteText(s2, Info2Rect, Info2Font, BLACK, 0, NOCOLOR);
		ExportingInfo.m_MemDC.DrawRectangle(ProcessRect, BLUE, NOCOLOR);
		ExportingInfo.m_MemDC.DrawRectangle(OutLineRect, BLACK, NOCOLOR);
		ExportingInfo.CenterWindow(0);
		ExportingInfo.ShowWindow(TRUE);

		StepVal = 0;
		ProVal = 0;
	}
	//Show the exporting info Done

	int restPCount = n;
	int restECount = ECount;
	Pos = m_PatientsList.GetFirstSelectedItemPosition();

	while (Pos)
	{
		int p = (int)m_PatientsList.GetItemData(m_PatientsList.GetNextSelectedItem(Pos));

		CPatient* pPatient = &::DB.m_Patients[p];

		::DB.LoadExamHeaders(pPatient->m_PatientID);//Big Database Research since 03062019
		//::DB.LoadTxtExamHeaders(pPatient->m_PatientID);//Big Database Research since 03062019

		for (int e = 0; e < ::DB.m_ExamHeaders.GetSize(); e++)
		{
			CExamHeader* pExamHeader = &::DB.m_ExamHeaders[e];

			if (pExamHeader->m_Type == EXAM_TYPE_WF)
			{
				CWFExam *WFExam = new CWFExam;
				if (::DB.LoadWFExam(pExamHeader->m_ExamID, WFExam))
				{
					WFExam->Process();

					//for average exam
					CString ExamId, LoadFileName;
					GUIDToStr(WFExam->m_Header.m_ExamID, ExamId);
					LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

					if (::PathFileExists(LoadFileName))
					{
						CAveExamInfo AveExamInfo;

						AveExamInfo.LoadFromFile(LoadFileName);
						for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
						{
							WFExam->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
						}
					}
					//for average exam Done

					int res = SaveWFExamToExcel(WFExam, Name1);

					if (res == -2)
					{
						delete WFExam;
						return;
					}

					//WF & CT exporting
					{
						BOOL FellowExamExist = FALSE;
						GUID FellowExamID = GUID_NULL;

						real_t scanSize = 0;
						int  NumImg = 0;
						FellowExamID = ::DB.LoadFellowID(pExamHeader->m_ExamID, scanSize, NumImg, EXAM_TYPE_WF);
						if (FellowExamID != GUID_NULL && ::DB.ExamExist(FellowExamID))
						{
							CCTExam* CTExam = new CCTExam;
							if (::DB.LoadCTExam(FellowExamID, CTExam))
							{
								CTExam->Process();
								int res = SaveWFCTExamToExcel(WFExam, CTExam, Name3);//OnExcelButtonClicked
								delete CTExam;
								if (res == -2) return;
							}
						}
					}

					delete WFExam;

					restECount--;

					if (ShowExpInfo)
					{
						ExportingInfo.ClearMemDC();
						ExportingInfo.m_MemDC.FillSolidRect(&Shadow1Rect, 0x00cccccc);
						ExportingInfo.m_MemDC.FillSolidRect(&Shadow2Rect, 0x00cccccc);
						ExportingInfo.m_MemDC.FillSolidRect(&Shadow3Rect, WHITE);
						ExportingInfo.m_MemDC.FillSolidRect(&Shadow4Rect, WHITE);
						ExportingInfo.m_MemDC.FillSolidRect(&Info1Rect, InfoBkColor);
						ExportingInfo.m_MemDC.FillSolidRect(&RestRect, ProBlockBkColor);
						ExportingInfo.m_MemDC.FillSolidRect(&ProcessRect, ProcessingBkColor);
						ExportingInfo.m_MemDC.WriteText(s1, Info1Rect, Info1Font, BLACK, 1, NOCOLOR);
						ExportingInfo.m_MemDC.DrawRectangle(ProcessRect, BLUE, NOCOLOR);
						ExportingInfo.m_MemDC.DrawRectangle(OutLineRect, BLACK, NOCOLOR);

						StepVal++;

						if (ECount > 100)
						{
							if (StepVal == Step)
							{
								StepVal = 0;
								ProVal++;
								if (ProVal == 100 && restECount > 0)
								{
									ProVal = 99;
								}
							}
						}
						else
						{
							ProVal += Step;
							if (ProVal >= 100) ProVal -= Step;
						}
						::SetRect(&ProcessingRect, 50, 120, 50 + ProVal * 3, 140);
						ExportingInfo.m_MemDC.DrawRectangle(ProcessingRect, BLUE, BLUE);

						CString ps = "Patients";
						if (restPCount <= 1) ps = "Patient";
						CString es = "Exams";
						if (restECount <= 1) es = "Exam";

						sECount.Format(_T("%i"), restECount);
						s2 = sPCount + " " + ps + " with " + sECount + " " + es + " remaining";
						ExportingInfo.m_MemDC.WriteText(s2, Info2Rect, Info2Font, BLACK, 0, NOCOLOR);

						ExportingInfo.Invalidate(FALSE);
					}
				}
			}
			else if (pExamHeader->m_Type == EXAM_TYPE_CT)
			{
				CCTExam* CTExam = new CCTExam;
				if (::DB.LoadCTExam(pExamHeader->m_ExamID, CTExam))
				{
					CTExam->Process();

					int res = SaveCTExamToExcel(CTExam, Name2);

					if (res == -2)
					{
						delete CTExam;
						return;
					}

					restECount--;

					if (ShowExpInfo)
					{
						ExportingInfo.ClearMemDC();
						ExportingInfo.m_MemDC.FillSolidRect(&Shadow1Rect, 0x00cccccc);
						ExportingInfo.m_MemDC.FillSolidRect(&Shadow2Rect, 0x00cccccc);
						ExportingInfo.m_MemDC.FillSolidRect(&Shadow3Rect, WHITE);
						ExportingInfo.m_MemDC.FillSolidRect(&Shadow4Rect, WHITE);
						ExportingInfo.m_MemDC.FillSolidRect(&Info1Rect, InfoBkColor);
						ExportingInfo.m_MemDC.FillSolidRect(&RestRect, ProBlockBkColor);
						ExportingInfo.m_MemDC.FillSolidRect(&ProcessRect, ProcessingBkColor);
						ExportingInfo.m_MemDC.WriteText(s1, Info1Rect, Info1Font, BLACK, 1, NOCOLOR);
						ExportingInfo.m_MemDC.DrawRectangle(ProcessRect, BLUE, NOCOLOR);
						ExportingInfo.m_MemDC.DrawRectangle(OutLineRect, BLACK, NOCOLOR);

						StepVal++;

						if (ECount > 100)
						{
							if (StepVal == Step)
							{
								StepVal = 0;
								ProVal++;
								if (ProVal == 100 && restECount > 0)
								{
									ProVal = 99;
								}
							}
						}
						else
						{
							ProVal += Step;
							if (ProVal >= 100) ProVal -= Step;
						}

						::SetRect(&ProcessingRect, 50, 120, 50 + ProVal * 3, 140);
						ExportingInfo.m_MemDC.DrawRectangle(ProcessingRect, BLUE, BLUE);

						CString ps = "Patients";
						if (restPCount <= 1) ps = "Patient";
						CString es = "Exams";
						if (restECount <= 1) es = "Exam";

						sECount.Format(_T("%i"), restECount);
						s2 = sPCount + " " + ps + " with " + sECount + " " + es + " remaining";
						ExportingInfo.m_MemDC.WriteText(s2, Info2Rect, Info2Font, BLACK, 0, NOCOLOR);

						ExportingInfo.Invalidate(FALSE);
					}
				}
				delete CTExam;
			}
			else continue;
		}

		restPCount--;
		sPCount.Format(_T("%i"), restPCount);
	}

	if (ShowExpInfo) ExportingInfo.ShowWindow(FALSE);

	sPCount.Format(_T("%i"), n);
	CString sSuc;
	if (n == 1)
		sSuc = "Successfully exported " + sPCount + " patient to '" + FolderName + "\\610Patients*.xls(x)'";
	else
		sSuc = "Successfully exported " + sPCount + " patients to '" + FolderName + "\\610Patients*.xls(x)'";
	::Info(sSuc);
}

//***************************************************************************************

void CMainWnd::OnBackButtonClicked()
{
	if (m_Screen == SCREEN_BIG_TORIC_PLANNER)
	{
		m_Screen = SCREEN_TORIC_PLANNER;
		m_BigPlanWnd->ShowWindow(SW_HIDE);
		m_pSumWnd->ShowWindow(SW_SHOW);

		ShowCommonSummaryLevelButtons();
		if (m_SummaryType == 1) {
			ShowCTSummaryLevelButtons();
		}
		else if (m_SummaryType == 2) {
			ShowINSummaryLevelButtons();
		}
		else if (m_SummaryType == 0) {
			ShowWFSummaryLevelButtons();
		}
		return;
	}

	if (m_Level == LEVEL_MAIN)
	{
		if (::Question("Exit the application?", 1, 0))
		{
			if (m_BigPlanWnd != NULL) m_BigPlanWnd->RealseMem();
			SAFE_DELETE(m_BigPlanWnd);

			if (m_BigCTSumWnd != NULL) m_BigCTSumWnd->RealseMem();
			SAFE_DELETE(m_BigCTSumWnd);

			int W = ::GetSystemMetrics(SM_CXSCREEN);
			for (int i = 0; i < 11; i++)
			{
				real_t ratioWFOD = 0.0;
				real_t ratioWFOS = 0.0;
				real_t ratioCTOD = m_CTODList.GetColumnWidth(i) / real_t(W);
				real_t ratioCTOS = m_CTOSList.GetColumnWidth(i) / real_t(W);
				if (i != 10)
				{
					ratioWFOD = m_WFODList.GetColumnWidth(i) / real_t(W);
					ratioWFOS = m_WFOSList.GetColumnWidth(i) / real_t(W);
				}
				else
				{
					ratioWFOD = (m_WFODList.GetColumnWidth(11) + 15 + m_WFODList.GetColumnWidth(2)) / real_t(W);
					ratioWFOS = (m_WFOSList.GetColumnWidth(11) + 15 + m_WFOSList.GetColumnWidth(2)) / real_t(W);
				}

				::NewSettings.WFOD_COL_Width[i] = ratioWFOD;
				::NewSettings.WFOS_COL_Width[i] = ratioWFOS;
				::NewSettings.CTOD_COL_Width[i] = ratioCTOD;
				::NewSettings.CTOS_COL_Width[i] = ratioCTOS;
			}

			PostMessage(WM_CLOSE, 0, 0);
		}
	}

	else if (m_Level == LEVEL_PATIENT)
	{
		DeleteSelectedIndicesLists();

		HidePatientLevelControls();

		m_Last_evel = LAST_LEVEL_PATIENT;

		m_Level = LEVEL_MAIN;

		Invalidate(FALSE);

		ShowMainLevelControls();

		m_PatientsList.SetFocus();
	}

	else if (m_Level == LEVEL_SUMMARY)
	{
		SAFE_DELETE(m_pSumWnd);

		if (m_BigPlanWnd != NULL)
		{
			m_BigPlanWnd->RealseMem();
			SAFE_DELETE(m_BigPlanWnd);
		}

		if (m_BigCTSumWnd != NULL) m_BigCTSumWnd->RealseMem();
		SAFE_DELETE(m_BigCTSumWnd);

		m_Screen = SCREEN_NONE;

		HideAllSummaryLevelButtons();

		DeleteLoadedExams();

		m_Last_evel = LAST_LEVEL_SUMMARY;
		m_Level = LEVEL_PATIENT;

		Sleep(100);

		Invalidate(FALSE);

		ShowPatientLevelControls();

		UpdateExamsLists();
	}
}

//***************************************************************************************

void CMainWnd::OnCtrlD()
{
	if (m_isDemo) return;

	CDatabaseDlg* pDlg = new CDatabaseDlg(this);

	if (pDlg->DoModal() == IDOK)
	{
		::DB.Change(pDlg->m_DatabaseType, pDlg->m_FolderName, pDlg->m_FileName, ::Settings.m_OpenDBAsReadOnly);

		UpdatePatientsList();

		////Big Database Research since 03062019
		//CString FolderName;

		//::DB.GetFolderName(FolderName);

		//FolderName = FolderName + "\\ExamHeader";

		//if (!::PathFileExists(FolderName))
		//{
		//	::CreateDirectory(FolderName, NULL);
		//	::Info("An update to the database is required. \n This process may take serveral minutes.\n Please click OK to continue.");
		//	//Sleep(1000);
		//	SplitExamHeaderToText();
		//}
		////Big Database Research since 03062019
	}

	delete pDlg;

	BOOL IsDBReadOnly = ::DB.IsReadOnly();

	m_NewPatientButton.SetEnabled(!IsDBReadOnly);
	m_NewPatientButton.Repaint();
	m_RegTimeButton.Repaint();

	m_DeletePatientButton.SetEnabled(!IsDBReadOnly);
	m_DeletePatientButton.Repaint();

	m_ImportPatientButton.SetEnabled(!IsDBReadOnly);
	m_ImportPatientButton.Repaint();

	m_PatientsList.SetFocus();
}

//***************************************************************************************

void CMainWnd::OnCtrlW()
{
	if (!m_isDemo && !m_isViewer)
	{
		CPasswordDlg* pDlg = new CPasswordDlg(this);
		int Res = pDlg->DoModal();
		CString Password = pDlg->m_Password;
		delete pDlg;

		CString s;
		if (m_isPresbia) s = "techservice";
		else s = "tracey";

		if (Res == IDOK && Password == s) {
			CCalDlg* pDlg = new CCalDlg(&::HW, NULL, this);
			pDlg->DoModal();
			delete pDlg;
		}

		m_PatientsList.SetFocus();
	}
}

//***************************************************************************************

//a siplified ctrl+w window
void CMainWnd::OnCtrlQ()
{
	CPasswordDlg* pDlg = new CPasswordDlg(this);
	int Res = pDlg->DoModal();
	CString Password = pDlg->m_Password;
	delete pDlg;

	CString s = "tracey";

	if (Res == IDOK && Password == s) {
		CAlignmentDlg* pDlg = new CAlignmentDlg(&::HW, NULL, this);
		pDlg->DoModal();
		delete pDlg;
	}

	m_PatientsList.SetFocus();
}

//***************************************************************************************

void CMainWnd::OnCtrlA()
{
	if (m_Level == LEVEL_MAIN)
	{
		for (int Row = 0; Row < m_PatientsList.GetItemCount(); Row++)
		{
			m_PatientsList.SetItemState(Row, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
	Invalidate(FALSE);
}

//***************************************************************************************

void CMainWnd::OnCtrlZ()
{
	//reserved
}

//***************************************************************************************

void CMainWnd::OnCtrlY()
{
	//reserved
}


//***************************************************************************************

void CMainWnd::ShowMainLevelControls()
{
	BOOL IsHardwareOK = ::HW.IsConnected() && ::HW.HasCalibrationLoaded();
	BOOL IsDBReadOnly = ::DB.IsReadOnly();

	if (IsHardwareOK)
	{
		if (!::HW.ISVideoCameraOn)
		{
			HW.TurnVideoCameraOn();
			::HW.ISVideoCameraOn = true;
		}
	}
	else ::HW.ISVideoCameraOn = false;

	m_NewPatientButton.SetEnabled(!IsDBReadOnly);
	m_NewPatientButton.ShowWindow(SW_SHOW);
	m_RegTimeButton.ShowWindow(SW_SHOW);
	m_SearchDefButton.ShowWindow(SW_SHOW);
	m_RefrshButton.ShowWindow(SW_SHOW);
	m_DatabaseButton.ShowWindow(SW_SHOW);
	//m_DataTestButton.ShowWindow(SW_SHOW);

	CString CustomID;
	m_CustomIDEdit.GetWindowText(CustomID);

	CString LastName;
	m_LastNameEdit.GetWindowText(LastName);

	CString FirstName;
	m_FirstNameEdit.GetWindowText(FirstName);

	CString MiddleName;
	m_MiddleNameEdit.GetWindowText(MiddleName);

	int k = m_GroupsComboBox.GetCurSel();

	CString DOB;
	m_DOBEdit.GetWindowText(DOB);

	CString Sex;
	m_SexComboBox.GetWindowText(Sex);

	if (CustomID != "" || LastName != "" || FirstName != "" || MiddleName != "" || (k != -1 && k != 0) || DOB != "" || Sex != "" || m_RegFromTime != "")
		m_SearchDefButton.SetEnabled(TRUE);
	else	  m_SearchDefButton.SetEnabled(FALSE);

	m_SearchDefButton.Repaint();

	m_SelectPatientButton.ShowWindow(SW_SHOW);

	m_EditPatientButton.ShowWindow(SW_SHOW);

	m_DeletePatientButton.SetEnabled(!IsDBReadOnly);
	m_DeletePatientButton.ShowWindow(SW_SHOW);

	m_ImportPatientButton.SetEnabled(!IsDBReadOnly);
	m_ImportPatientButton.ShowWindow(SW_SHOW);

	m_ExportPatientButton.ShowWindow(SW_SHOW);

	if (!m_isSolo) m_ExportExcelButton.ShowWindow(SW_SHOW);

	m_SettingsButton.ShowWindow(SW_SHOW);

	m_LicenseButton.ShowWindow(SW_SHOW);

	m_TestButton.SetEnabled(IsHardwareOK);
	m_TestButton.ShowWindow(SW_SHOW);

	m_BackButton.SetImage(m_q == 2 ? IDR_EXIT_3 : m_q == 1 ? IDR_EXIT_2 : IDR_EXIT_1);
	m_BackButton.ShowWindow(SW_SHOW);

	m_PatientsList.ShowWindow(SW_SHOW);
	m_CustomIDEdit.ShowWindow(SW_SHOW);
	m_LastNameEdit.ShowWindow(SW_SHOW);
	m_FirstNameEdit.ShowWindow(SW_SHOW);
	m_MiddleNameEdit.ShowWindow(SW_SHOW);
	m_GroupsComboBox.ShowWindow(SW_SHOW);
	m_DOBEdit.ShowWindow(SW_SHOW);
	m_SexComboBox.ShowWindow(SW_SHOW);

	if (m_isDemo)
	{
		m_NewPatientButton.SetEnabled(FALSE);
		m_EditPatientButton.SetEnabled(FALSE);
		m_DeletePatientButton.SetEnabled(FALSE);
		m_NewPatientButton.Repaint();
		m_EditPatientButton.Repaint();
		m_DeletePatientButton.Repaint();

		//Import button
		CINIParser IniPar;
		int Type2 = IniPar.GetIntKey(IniSection, "OMED", IniPath); // 6.2.0 ini registration

		if (Type2 == 1)
		{
			int n = m_PatientsList.GetSelectedCount();
			if (n > 30)
			{
				m_ImportPatientButton.SetEnabled(FALSE);
				m_ImportPatientButton.Repaint();
			}

			return;
		}
		//Import button Done
	}
	//530
}

//***************************************************************************************

void CMainWnd::HideMainLevelControls()
{
	m_NewPatientButton.ShowWindow(SW_HIDE);
	m_RegTimeButton.ShowWindow(SW_HIDE);
	m_SearchDefButton.ShowWindow(SW_HIDE);
	m_RefrshButton.ShowWindow(SW_HIDE);
	m_DatabaseButton.ShowWindow(SW_HIDE);
	m_SelectPatientButton.ShowWindow(SW_HIDE);
	m_EditPatientButton.ShowWindow(SW_HIDE);
	m_DeletePatientButton.ShowWindow(SW_HIDE);
	m_ImportPatientButton.ShowWindow(SW_HIDE);
	m_ExportPatientButton.ShowWindow(SW_HIDE);
	m_ExportExcelButton.ShowWindow(SW_HIDE);
	m_SettingsButton.ShowWindow(SW_HIDE);
	m_LicenseButton.ShowWindow(SW_HIDE);
	m_TestButton.ShowWindow(SW_HIDE);
	m_BackButton.ShowWindow(SW_HIDE);

	m_PatientsList.ShowWindow(SW_HIDE);
	m_CustomIDEdit.ShowWindow(SW_HIDE);
	m_LastNameEdit.ShowWindow(SW_HIDE);
	m_FirstNameEdit.ShowWindow(SW_HIDE);
	m_MiddleNameEdit.ShowWindow(SW_HIDE);
	m_GroupsComboBox.ShowWindow(SW_HIDE);
	m_DOBEdit.ShowWindow(SW_HIDE);
	m_SexComboBox.ShowWindow(SW_HIDE);
}

//***************************************************************************************

// lParam1 and lParam2 parameters specify the item data for the two items being compared
int CALLBACK CMainWnd::CompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CMainWnd* pWnd = (CMainWnd*)lParamSort;
	int i1 = (int)lParam1;
	int i2 = (int)lParam2;
	CPatient* pPatient1 = &::DB.m_Patients[i1];
	CPatient* pPatient2 = &::DB.m_Patients[i2];
	int res = -1;

	switch (pWnd->m_PatientsList.m_HeaderCtrl.m_sort_col)
	{
	case 0:
		res = pPatient1->m_CustomID.CompareNoCase(pPatient2->m_CustomID);
		break;

	case 1:
		if (m_RegYear[i1]  < m_RegYear[i2])  res = -1;
		else if (m_RegYear[i1]  > m_RegYear[i2])  res = 1;
		else if (m_RegMonth[i1] < m_RegMonth[i2]) res = -1;
		else if (m_RegMonth[i1] > m_RegMonth[i2]) res = 1;
		else if (m_RegDay[i1]   < m_RegDay[i2])   res = -1;
		else if (m_RegDay[i1]   > m_RegDay[i2])   res = 1;
		else if (m_RegHour[i1]  < m_RegHour[i2])  res = -1;
		else if (m_RegHour[i1]  > m_RegHour[i2])  res = 1;
		else if (m_RegMinute[i1]  > m_RegMinute[i2])  res = 1;
		else if (m_RegMinute[i1]  > m_RegMinute[i2])  res = 1;
		else                                      res = 0;
		break;

	case 2:
		res = pPatient1->m_LastName.CompareNoCase(pPatient2->m_LastName);
		break;

	case 3:
		res = pPatient1->m_FirstName.CompareNoCase(pPatient2->m_FirstName);
		break;

	case 4:
		res = pPatient1->m_MiddleName.CompareNoCase(pPatient2->m_MiddleName);
		break;

	case 5:
		if (pPatient1->m_BirthYear  < pPatient2->m_BirthYear)       res = -1;
		else if (pPatient1->m_BirthYear  > pPatient2->m_BirthYear)  res = 1;
		else if (pPatient1->m_BirthMonth < pPatient2->m_BirthMonth) res = -1;
		else if (pPatient1->m_BirthMonth > pPatient2->m_BirthMonth) res = 1;
		else if (pPatient1->m_BirthDay   < pPatient2->m_BirthDay)   res = -1;
		else if (pPatient1->m_BirthDay   > pPatient2->m_BirthDay)   res = 1;
		else                                                        res = 0;
		break;

	case 6:
		if (pPatient1->m_Sex < pPatient2->m_Sex)      res = -1;
		else if (pPatient1->m_Sex > pPatient2->m_Sex) res = 1;
		else                                          res = 0;
		break;

	case 7:
		res = pWnd->m_GroupNames[i1].CompareNoCase(pWnd->m_GroupNames[i2]);
		break;

	case 8: //6.2.0 Sort the source column of patient list
		if (pPatient1->m_IsDicom < pPatient2->m_IsDicom)      res = -1;
		else if (pPatient1->m_IsDicom > pPatient2->m_IsDicom) res = 1;
		else                                                  res = 0;
		break;

	case 9:
		res = pPatient1->m_Note.CompareNoCase(pPatient2->m_Note);
		break;
	}

	return pWnd->m_PatientsList.m_HeaderCtrl.m_sort_asc ? res : -res;
}

//***************************************************************************************

int CALLBACK CMainWnd::CompareWFODExamProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CMainWnd* pWnd = (CMainWnd*)lParamSort;
	int res = -1;

	int i1 = (int)lParam1;
	int i2 = (int)lParam2;

	CExamHeader* pExamHeader1 = &::DB.m_ExamHeaders[i1];
	CExamHeader* pExamHeader2 = &::DB.m_ExamHeaders[i2];

	CClinic* pClinic;
	COperator* pOperator;
	CPhysician* pPhysician;

	CString s1 = "";
	CString s2 = "";
	switch (pWnd->m_WFODList.m_HeaderCtrl.m_sort_col)
	{
	case 0:
		if (i1  < i2)  res = -1;
		else if (i1  > i2)  res = 1;
		else                res = 0;
		break;

	case 1:
		if (pExamHeader1->m_Year   < pExamHeader2->m_Year)  res = -1;
		else if (pExamHeader1->m_Year   > pExamHeader2->m_Year)  res = 1;
		else if (pExamHeader1->m_Month  < pExamHeader2->m_Month)  res = -1;
		else if (pExamHeader1->m_Month  > pExamHeader2->m_Month)  res = 1;
		else if (pExamHeader1->m_Day    < pExamHeader2->m_Day)  res = -1;
		else if (pExamHeader1->m_Day    > pExamHeader2->m_Day)  res = 1;
		else if (pExamHeader1->m_Hour   < pExamHeader2->m_Hour)  res = -1;
		else if (pExamHeader1->m_Hour   > pExamHeader2->m_Hour)  res = 1;
		else if (pExamHeader1->m_Minute < pExamHeader2->m_Minute)  res = -1;
		else if (pExamHeader1->m_Minute > pExamHeader2->m_Minute)  res = 1;
		else if (pExamHeader1->m_Second < pExamHeader2->m_Second)  res = -1;
		else if (pExamHeader1->m_Second > pExamHeader2->m_Second)  res = 1;
		else                                                       res = 0;
		break;

	case 2:
		if (pExamHeader1->m_Hour   < pExamHeader2->m_Hour)  res = -1;
		else if (pExamHeader1->m_Hour   > pExamHeader2->m_Hour)  res = 1;
		else if (pExamHeader1->m_Minute < pExamHeader2->m_Minute)  res = -1;
		else if (pExamHeader1->m_Minute > pExamHeader2->m_Minute)  res = 1;
		else if (pExamHeader1->m_Second < pExamHeader2->m_Second)  res = -1;
		else if (pExamHeader1->m_Second > pExamHeader2->m_Second)  res = 1;
		else                                                       res = 0;
		break;

	case 3:
		s1 = pExamHeader1->SoftwareVersion();
		s2 = pExamHeader2->SoftwareVersion();
		res = s1.CompareNoCase(s2);
		break;

	case 4:
		s1 = pExamHeader1->Mode();
		s2 = pExamHeader2->Mode();
		res = s1.CompareNoCase(s2);
		break;

	case 5:
		pClinic = ::DB.GetClinic(pExamHeader1->m_ClinicID);
		if (pClinic) s1 = pClinic->m_Name;
		pClinic = ::DB.GetClinic(pExamHeader2->m_ClinicID);
		if (pClinic) s2 = pClinic->m_Name;
		res = s1.CompareNoCase(s2);
		break;

	case 6:
		pPhysician = ::DB.GetPhysician(pExamHeader1->m_PhysicianID);
		if (pPhysician) s1 = pPhysician->FullName();
		pPhysician = ::DB.GetPhysician(pExamHeader2->m_PhysicianID);
		if (pPhysician) s2 = pPhysician->FullName();
		res = s1.CompareNoCase(s2);
		break;

	case 7:
		pOperator = ::DB.GetOperator(pExamHeader1->m_OperatorID);
		if (pOperator) s1 = pOperator->FullName();
		pOperator = ::DB.GetOperator(pExamHeader2->m_OperatorID);
		if (pOperator) s2 = pOperator->FullName();
		res = s1.CompareNoCase(s2);
		break;

	case 8:
		s1 = pExamHeader1->Preop();
		s2 = pExamHeader2->Preop();
		res = s1.CompareNoCase(s2);
		break;

	case 9:
		if (pExamHeader1->m_SeriesNumber < pExamHeader2->m_SeriesNumber)  res = -1;
		else if (pExamHeader1->m_SeriesNumber > pExamHeader2->m_SeriesNumber)  res = 1;
		else                                                                   res = 0;
		break;

	case 10:
		if (m_ScanSize[i1] < m_ScanSize[i2])  res = -1;
		else if (m_ScanSize[i1] > m_ScanSize[i2])  res = 1;
		else             res = 0;
		break;

	case 11:
		res = pExamHeader1->m_Note.CompareNoCase(pExamHeader2->m_Note);
		break;
	}

	return pWnd->m_WFODList.m_HeaderCtrl.m_sort_asc ? res : -res;
}


//***************************************************************************************

int CALLBACK CMainWnd::CompareWFOSExamProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CMainWnd* pWnd = (CMainWnd*)lParamSort;
	int res = -1;

	int i1 = (int)lParam1;
	int i2 = (int)lParam2;

	//int aa = CMainWnd::WDArray[0];

	CExamHeader* pExamHeader1 = &::DB.m_ExamHeaders[i1];
	CExamHeader* pExamHeader2 = &::DB.m_ExamHeaders[i2];

	CClinic* pClinic;
	COperator* pOperator;
	CPhysician* pPhysician;

	CString s1 = "";
	CString s2 = "";
	switch (pWnd->m_WFOSList.m_HeaderCtrl.m_sort_col)
	{
	case 0:
		if (i1  < i2)  res = -1;
		else if (i1  > i2)  res = 1;
		else                res = 0;
		break;

	case 1:
		if (pExamHeader1->m_Year   < pExamHeader2->m_Year)  res = -1;
		else if (pExamHeader1->m_Year   > pExamHeader2->m_Year)  res = 1;
		else if (pExamHeader1->m_Month  < pExamHeader2->m_Month)  res = -1;
		else if (pExamHeader1->m_Month  > pExamHeader2->m_Month)  res = 1;
		else if (pExamHeader1->m_Day    < pExamHeader2->m_Day)  res = -1;
		else if (pExamHeader1->m_Day    > pExamHeader2->m_Day)  res = 1;
		else if (pExamHeader1->m_Hour   < pExamHeader2->m_Hour)  res = -1;
		else if (pExamHeader1->m_Hour   > pExamHeader2->m_Hour)  res = 1;
		else if (pExamHeader1->m_Minute < pExamHeader2->m_Minute)  res = -1;
		else if (pExamHeader1->m_Minute > pExamHeader2->m_Minute)  res = 1;
		else if (pExamHeader1->m_Second < pExamHeader2->m_Second)  res = -1;
		else if (pExamHeader1->m_Second > pExamHeader2->m_Second)  res = 1;
		else                                                       res = 0;
		break;

	case 2:
		if (pExamHeader1->m_Hour   < pExamHeader2->m_Hour)  res = -1;
		else if (pExamHeader1->m_Hour   > pExamHeader2->m_Hour)  res = 1;
		else if (pExamHeader1->m_Minute < pExamHeader2->m_Minute)  res = -1;
		else if (pExamHeader1->m_Minute > pExamHeader2->m_Minute)  res = 1;
		else if (pExamHeader1->m_Second < pExamHeader2->m_Second)  res = -1;
		else if (pExamHeader1->m_Second > pExamHeader2->m_Second)  res = 1;
		else                                                       res = 0;
		break;

	case 3:
		s1 = pExamHeader1->SoftwareVersion();
		s2 = pExamHeader2->SoftwareVersion();
		res = s1.CompareNoCase(s2);
		break;

	case 4:
		s1 = pExamHeader1->Mode();
		s2 = pExamHeader2->Mode();
		res = s1.CompareNoCase(s2);
		break;

	case 5:
		pClinic = ::DB.GetClinic(pExamHeader1->m_ClinicID);
		if (pClinic) s1 = pClinic->m_Name;
		pClinic = ::DB.GetClinic(pExamHeader2->m_ClinicID);
		if (pClinic) s2 = pClinic->m_Name;
		res = s1.CompareNoCase(s2);
		break;

	case 6:
		pPhysician = ::DB.GetPhysician(pExamHeader1->m_PhysicianID);
		if (pPhysician) s1 = pPhysician->FullName();
		pPhysician = ::DB.GetPhysician(pExamHeader2->m_PhysicianID);
		if (pPhysician) s2 = pPhysician->FullName();
		res = s1.CompareNoCase(s2);
		break;

	case 7:
		pOperator = ::DB.GetOperator(pExamHeader1->m_OperatorID);
		if (pOperator) s1 = pOperator->FullName();
		pOperator = ::DB.GetOperator(pExamHeader2->m_OperatorID);
		if (pOperator) s2 = pOperator->FullName();
		res = s1.CompareNoCase(s2);
		break;

	case 8:
		s1 = pExamHeader1->Preop();
		s2 = pExamHeader2->Preop();
		res = s1.CompareNoCase(s2);
		break;

	case 9:
		if (pExamHeader1->m_SeriesNumber < pExamHeader2->m_SeriesNumber)  res = -1;
		else if (pExamHeader1->m_SeriesNumber > pExamHeader2->m_SeriesNumber)  res = 1;
		else                                                                   res = 0;
		break;

	case 10:
		if (m_ScanSize[i1] < m_ScanSize[i2])  res = -1;
		else if (m_ScanSize[i1] > m_ScanSize[i2])  res = 1;
		else                                        res = 0;
		break;

	case 11:
		res = pExamHeader1->m_Note.CompareNoCase(pExamHeader2->m_Note);
		break;
	}

	return pWnd->m_WFOSList.m_HeaderCtrl.m_sort_asc ? res : -res;
}

//***************************************************************************************

int CALLBACK CMainWnd::CompareCTODExamProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CMainWnd* pWnd = (CMainWnd*)lParamSort;
	int res = -1;

	int i1 = (int)lParam1;
	int i2 = (int)lParam2;

	//int aa = CMainWnd::WDArray[0];

	CExamHeader* pExamHeader1 = &::DB.m_ExamHeaders[i1];
	CExamHeader* pExamHeader2 = &::DB.m_ExamHeaders[i2];

	CClinic* pClinic;
	COperator* pOperator;
	CPhysician* pPhysician;

	CString s1 = "";
	CString s2 = "";
	switch (pWnd->m_CTODList.m_HeaderCtrl.m_sort_col)
	{
	case 0:
		if (i1  < i2)  res = -1;
		else if (i1  > i2)  res = 1;
		else                res = 0;
		break;

	case 1:
		if (pExamHeader1->m_Year   < pExamHeader2->m_Year)  res = -1;
		else if (pExamHeader1->m_Year   > pExamHeader2->m_Year)  res = 1;
		else if (pExamHeader1->m_Month  < pExamHeader2->m_Month)  res = -1;
		else if (pExamHeader1->m_Month  > pExamHeader2->m_Month)  res = 1;
		else if (pExamHeader1->m_Day    < pExamHeader2->m_Day)  res = -1;
		else if (pExamHeader1->m_Day    > pExamHeader2->m_Day)  res = 1;
		else if (pExamHeader1->m_Hour   < pExamHeader2->m_Hour)  res = -1;
		else if (pExamHeader1->m_Hour   > pExamHeader2->m_Hour)  res = 1;
		else if (pExamHeader1->m_Minute < pExamHeader2->m_Minute)  res = -1;
		else if (pExamHeader1->m_Minute > pExamHeader2->m_Minute)  res = 1;
		else if (pExamHeader1->m_Second < pExamHeader2->m_Second)  res = -1;
		else if (pExamHeader1->m_Second > pExamHeader2->m_Second)  res = 1;
		else                                                       res = 0;
		break;

	case 2:
		if (pExamHeader1->m_Hour   < pExamHeader2->m_Hour)  res = -1;
		else if (pExamHeader1->m_Hour   > pExamHeader2->m_Hour)  res = 1;
		else if (pExamHeader1->m_Minute < pExamHeader2->m_Minute)  res = -1;
		else if (pExamHeader1->m_Minute > pExamHeader2->m_Minute)  res = 1;
		else if (pExamHeader1->m_Second < pExamHeader2->m_Second)  res = -1;
		else if (pExamHeader1->m_Second > pExamHeader2->m_Second)  res = 1;
		else                                                       res = 0;
		break;

	case 3:
		s1 = pExamHeader1->SoftwareVersion();
		s2 = pExamHeader2->SoftwareVersion();
		res = s1.CompareNoCase(s2);
		break;

	case 4:
		s1 = pExamHeader1->Mode();
		s2 = pExamHeader2->Mode();
		res = s1.CompareNoCase(s2);
		break;

	case 5:
		pClinic = ::DB.GetClinic(pExamHeader1->m_ClinicID);
		if (pClinic) s1 = pClinic->m_Name;
		pClinic = ::DB.GetClinic(pExamHeader2->m_ClinicID);
		if (pClinic) s2 = pClinic->m_Name;
		res = s1.CompareNoCase(s2);
		break;

	case 6:
		pPhysician = ::DB.GetPhysician(pExamHeader1->m_PhysicianID);
		if (pPhysician) s1 = pPhysician->FullName();
		pPhysician = ::DB.GetPhysician(pExamHeader2->m_PhysicianID);
		if (pPhysician) s2 = pPhysician->FullName();
		res = s1.CompareNoCase(s2);
		break;

	case 7:
		pOperator = ::DB.GetOperator(pExamHeader1->m_OperatorID);
		if (pOperator) s1 = pOperator->FullName();
		pOperator = ::DB.GetOperator(pExamHeader2->m_OperatorID);
		if (pOperator) s2 = pOperator->FullName();
		res = s1.CompareNoCase(s2);
		break;

	case 8:
		s1 = pExamHeader1->Preop();
		s2 = pExamHeader2->Preop();
		res = s1.CompareNoCase(s2);
		break;

	case 9:
		if (pExamHeader1->m_SeriesNumber < pExamHeader2->m_SeriesNumber)  res = -1;
		else if (pExamHeader1->m_SeriesNumber > pExamHeader2->m_SeriesNumber)  res = 1;
		else                                                                   res = 0;
		break;

	case 10:
		res = pExamHeader1->m_Note.CompareNoCase(pExamHeader2->m_Note);
		break;
	}

	return pWnd->m_CTODList.m_HeaderCtrl.m_sort_asc ? res : -res;
}

//***************************************************************************************

int CALLBACK CMainWnd::CompareCTOSExamProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CMainWnd* pWnd = (CMainWnd*)lParamSort;
	int res = -1;

	int i1 = (int)lParam1;
	int i2 = (int)lParam2;

	CExamHeader* pExamHeader1 = &::DB.m_ExamHeaders[i1];
	CExamHeader* pExamHeader2 = &::DB.m_ExamHeaders[i2];

	CClinic* pClinic;
	COperator* pOperator;
	CPhysician* pPhysician;

	CString s1 = "";
	CString s2 = "";
	switch (pWnd->m_CTOSList.m_HeaderCtrl.m_sort_col)
	{
	case 0:
		if (i1  < i2)  res = -1;
		else if (i1  > i2)  res = 1;
		else                res = 0;
		break;

	case 1:
		if (pExamHeader1->m_Year   < pExamHeader2->m_Year)  res = -1;
		else if (pExamHeader1->m_Year   > pExamHeader2->m_Year)  res = 1;
		else if (pExamHeader1->m_Month  < pExamHeader2->m_Month)  res = -1;
		else if (pExamHeader1->m_Month  > pExamHeader2->m_Month)  res = 1;
		else if (pExamHeader1->m_Day    < pExamHeader2->m_Day)  res = -1;
		else if (pExamHeader1->m_Day    > pExamHeader2->m_Day)  res = 1;
		else if (pExamHeader1->m_Hour   < pExamHeader2->m_Hour)  res = -1;
		else if (pExamHeader1->m_Hour   > pExamHeader2->m_Hour)  res = 1;
		else if (pExamHeader1->m_Minute < pExamHeader2->m_Minute)  res = -1;
		else if (pExamHeader1->m_Minute > pExamHeader2->m_Minute)  res = 1;
		else if (pExamHeader1->m_Second < pExamHeader2->m_Second)  res = -1;
		else if (pExamHeader1->m_Second > pExamHeader2->m_Second)  res = 1;
		else                                                       res = 0;
		break;

	case 2:
		if (pExamHeader1->m_Hour   < pExamHeader2->m_Hour)  res = -1;
		else if (pExamHeader1->m_Hour   > pExamHeader2->m_Hour)  res = 1;
		else if (pExamHeader1->m_Minute < pExamHeader2->m_Minute)  res = -1;
		else if (pExamHeader1->m_Minute > pExamHeader2->m_Minute)  res = 1;
		else if (pExamHeader1->m_Second < pExamHeader2->m_Second)  res = -1;
		else if (pExamHeader1->m_Second > pExamHeader2->m_Second)  res = 1;
		else                                                       res = 0;
		break;

	case 3:
		s1 = pExamHeader1->SoftwareVersion();
		s2 = pExamHeader2->SoftwareVersion();
		res = s1.CompareNoCase(s2);
		break;

	case 4:
		s1 = pExamHeader1->Mode();
		s2 = pExamHeader2->Mode();
		res = s1.CompareNoCase(s2);
		break;

	case 5:
		pClinic = ::DB.GetClinic(pExamHeader1->m_ClinicID);
		if (pClinic) s1 = pClinic->m_Name;
		pClinic = ::DB.GetClinic(pExamHeader2->m_ClinicID);
		if (pClinic) s2 = pClinic->m_Name;
		res = s1.CompareNoCase(s2);
		break;

	case 6:
		pPhysician = ::DB.GetPhysician(pExamHeader1->m_PhysicianID);
		if (pPhysician) s1 = pPhysician->FullName();
		pPhysician = ::DB.GetPhysician(pExamHeader2->m_PhysicianID);
		if (pPhysician) s2 = pPhysician->FullName();
		res = s1.CompareNoCase(s2);
		break;

	case 7:
		pOperator = ::DB.GetOperator(pExamHeader1->m_OperatorID);
		if (pOperator) s1 = pOperator->FullName();
		pOperator = ::DB.GetOperator(pExamHeader2->m_OperatorID);
		if (pOperator) s2 = pOperator->FullName();
		res = s1.CompareNoCase(s2);
		break;

	case 8:
		s1 = pExamHeader1->Preop();
		s2 = pExamHeader2->Preop();
		res = s1.CompareNoCase(s2);
		break;

	case 9:
		if (pExamHeader1->m_SeriesNumber < pExamHeader2->m_SeriesNumber)  res = -1;
		else if (pExamHeader1->m_SeriesNumber > pExamHeader2->m_SeriesNumber)  res = 1;
		else                                                                   res = 0;
		break;

	case 10:
		res = pExamHeader1->m_Note.CompareNoCase(pExamHeader2->m_Note);
		break;

	}

	return pWnd->m_CTOSList.m_HeaderCtrl.m_sort_asc ? res : -res;
}

//***************************************************************************************

void CMainWnd::OnPatientsListColumnClicked(NMHDR* pNMHDR, LRESULT* pRes)
{
	m_PatientsList.SetSortItem(((NMLISTVIEW*)pNMHDR)->iSubItem);

	BOOL Sort = m_PatientsList.SortItems(CompareProc, (LPARAM)this);

	int mark = m_PatientsList.GetSelectionMark();
	if (mark != -1) m_PatientsList.EnsureVisible(mark, FALSE);

	*pRes = 0;
}

//***************************************************************************************

void CMainWnd::OnPatientsListDblClicked(NMHDR* pNMHDR, LRESULT* pRes)
{
	*pRes = 0;

	OnSelectPatientButtonClicked();
}

//***************************************************************************************

void CMainWnd::UpdatePatientsList()
{
	if (!m_Update)
	{
		return;
	}

	CBusyCursor Cursor;

	m_PatientsList.DeleteAllItems();

	::DB.LoadPatients();

	m_TotPatCount = ::DB.m_Patients.GetSize();

	CString CustomID;
	m_CustomIDEdit.GetWindowText(CustomID);

	CString LastName;
	m_LastNameEdit.GetWindowText(LastName);

	CString FirstName;
	m_FirstNameEdit.GetWindowText(FirstName);

	CString MiddleName;
	m_MiddleNameEdit.GetWindowText(MiddleName);

	GUID GroupID = GUID_NULL;

	int k = m_GroupsComboBox.GetCurSel();
	if (k >= 1 && k <= ::DB.m_Groups.GetSize()) GroupID = ::DB.m_Groups[k - 1].m_GroupID;

	::DB.LoadGroups();
	m_GroupsComboBox.ResetContent();
	m_GroupsComboBox.AddString(_T(""));
	k = 0;
	for (int i = 0; i < ::DB.m_Groups.GetSize(); i++)
	{
		m_GroupsComboBox.AddString(::DB.m_Groups[i].m_Name);
		if (::DB.m_Groups[i].m_GroupID == GroupID) k = i + 1;
	}
	m_GroupsComboBox.SetCurSel(k);

	CString DOB;
	m_DOBEdit.GetWindowText(DOB);

	//520
	int FisrtDOB_POS = 2;
	int SecondDOB_POS = 5;
	if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
	{
		FisrtDOB_POS = 4;
		SecondDOB_POS = 7;
	}
	if (DOB.GetLength() == FisrtDOB_POS && DOB.GetLength() > m_InputDOBLen)
	{
		DOB += "-";
		m_DOBEdit.SetWindowText(DOB);
		m_DOBEdit.SetSel(-1);
	}
	if (DOB.GetLength() == SecondDOB_POS  && DOB.GetLength() > m_InputDOBLen)
	{
		DOB += "-";
		m_DOBEdit.SetWindowText(DOB);
		m_DOBEdit.SetSel(-1);
	}
	m_InputDOBLen = DOB.GetLength();

	CString Sex;
	m_SexComboBox.GetWindowText(Sex);

	if (CustomID != "" || LastName != "" || FirstName != "" || MiddleName != "" || k != 0 || DOB != "" || Sex != "" || m_RegFromTime != "")
		m_SearchDefButton.SetEnabled(TRUE);
	else  m_SearchDefButton.SetEnabled(FALSE);

	m_SearchDefButton.Repaint();

	m_GroupNames.Create(m_TotPatCount);
	m_RegYear.Create(m_TotPatCount);
	m_RegMonth.Create(m_TotPatCount);
	m_RegDay.Create(m_TotPatCount);
	m_RegHour.Create(m_TotPatCount);
	m_RegMinute.Create(m_TotPatCount);

	CString Note;
	CString RegTime;
	CString RegDate;
	k = 0;


	//int St = (int)clock();

	for (int p = 0; p < m_TotPatCount; p++)
	{
		if (p == 7)
		{
			int a = 0;
		}
		CPatient* pPatient = &::DB.m_Patients[p];
		CString dob(::DateToStr(pPatient->m_BirthYear, pPatient->m_BirthMonth, pPatient->m_BirthDay));
		CString sex(pPatient->m_Sex == SEX_MALE ? "M" : (pPatient->m_Sex == SEX_FEMALE ? "F" : ""));

		//6.2.0 For Dicom
		CString Resource = "";

		if (pPatient->m_LastName == "Planner")
		{
			int a = 0;
		}

		if (pPatient->m_IsDicom != TRUE &&  pPatient->m_IsDicom != FALSE)
		{
			pPatient->m_IsDicom = FALSE;
			::DB.SavePatient(*pPatient);

			Resource = "";
		}
		else
		{
			Resource = (pPatient->m_IsDicom ? "Dicom" : "");
		}
		//6.2.0 For Dicom

		if (pPatient->m_RegYear < 3010 && pPatient->m_RegYear > 0)
		{
			m_RegYear[p] = pPatient->m_RegYear;
			m_RegMonth[p] = pPatient->m_RegMonth;
			m_RegDay[p] = pPatient->m_RegDay;
			m_RegHour[p] = pPatient->m_RegHour;
			m_RegMinute[p] = pPatient->m_RegMinute;
			CString regtime(::DateToStr(pPatient->m_RegYear, pPatient->m_RegMonth, pPatient->m_RegDay));
			RegDate = regtime;
			CString s;
			s.Format(_T("%02i"), pPatient->m_RegHour);
			RegTime = regtime + " " + s + ":";
			s.Format(_T("%02i"), pPatient->m_RegMinute);
			RegTime = RegTime + s;
		}
		else
		{
			//get the registration time from exams of the patient
			::DB.LoadExamHeaders(pPatient->m_PatientID);//Big Database Research since 03062019
			//::DB.LoadTxtExamHeaders(pPatient->m_PatientID);//Big Database Research since 03062019

			if (::DB.m_ExamHeaders.GetSize() > 0)
			{
				CExamHeader* pExamHeader = &::DB.m_ExamHeaders[0];
				CString regtime(::DateToStr(pExamHeader->m_Year, pExamHeader->m_Month, pExamHeader->m_Day));
				CString s;
				RegDate = regtime;
				s.Format(_T("%02i"), pExamHeader->m_Hour);
				RegTime = regtime + " " + s + ":";
				s.Format(_T("%02i"), pExamHeader->m_Minute);
				RegTime = RegTime + s;
				pPatient->m_RegYear = m_RegYear[p] = pExamHeader->m_Year;
				pPatient->m_RegMonth = m_RegMonth[p] = pExamHeader->m_Month;
				pPatient->m_RegDay = m_RegDay[p] = pExamHeader->m_Day;
				pPatient->m_RegHour = m_RegHour[p] = pExamHeader->m_Hour;
				pPatient->m_RegMinute = m_RegMinute[p] = pExamHeader->m_Minute;
			}
			else
			{
				RegTime = "";
				RegDate = "";
				m_RegYear[p] = 0;
				m_RegMonth[p] = 0;
				m_RegDay[p] = 0;
				m_RegHour[p] = 0;
				m_RegMinute[p] = 0;
			}
			//::DB.SavePatient(*pPatient);
		}

		//add: && (m_RegFromTime == "" || (GreatTime(m_RegYear[p],m_RegMonth[p],m_RegDay[p],m_RegFromTime) && LessTime(m_RegYear[p],m_RegMonth[p],m_RegDay[p],m_RegToTime)))
		if (
			(CustomID == "" || CustomID.CompareNoCase(pPatient->m_CustomID.Left(CustomID.GetLength())) == 0) &&
			(LastName == "" || LastName.CompareNoCase(pPatient->m_LastName.Left(LastName.GetLength())) == 0) &&
			(FirstName == "" || FirstName.CompareNoCase(pPatient->m_FirstName.Left(FirstName.GetLength())) == 0) &&
			(MiddleName == "" || MiddleName.CompareNoCase(pPatient->m_MiddleName.Left(MiddleName.GetLength())) == 0) &&
			(GroupID == GUID_NULL || GroupID == pPatient->m_GroupID) &&
			(DOB == "" || DOB == dob.Left(DOB.GetLength())) &&
			(
				Sex == "" || Sex == sex) && (m_RegFromTime == "" ||
					(
						GreatTime(m_RegYear[p], m_RegMonth[p], m_RegDay[p], m_RegFromTime) &&
						LessTime(m_RegYear[p], m_RegMonth[p], m_RegDay[p], m_RegToTime)
						)
					)
			)
		{
			m_PatientsList.InsertItem(k, pPatient->m_CustomID);
			m_PatientsList.SetItemText(k, 1, RegDate);//wait for registration time
			m_PatientsList.SetItemText(k, 2, G_ReadChinese(pPatient->m_LastName));//6.2.0 ReadC hinese name
			m_PatientsList.SetItemText(k, 3, G_ReadChinese(pPatient->m_FirstName));//6.2.0 ReadC hinese name
			m_PatientsList.SetItemText(k, 4, G_ReadChinese(pPatient->m_MiddleName));//6.2.0 ReadC hinese name
			m_PatientsList.SetItemText(k, 5, dob);
			m_PatientsList.SetItemText(k, 6, sex);

			CGroup* pGroup = ::DB.GetGroup(pPatient->m_GroupID);
			if (pGroup)
			{
				m_PatientsList.SetItemText(k, 7, pGroup->m_Name);
				m_GroupNames[p] = pGroup->m_Name;
			}
			else
			{
				m_GroupNames[p] = "";
			}

			m_PatientsList.SetItemText(k, 8, Resource);//'Resource' for Dicom

			Note = pPatient->m_Note;
			for (int i = 0; i < Note.GetLength() - 1; i++)
			{
				if (Note.GetAt(i) == (char)13 && Note.GetAt(i + 1) == (char)10)
				{
					Note.SetAt(i, ' ');
					Note.SetAt(i + 1, ' ');
				}
			}
			m_PatientsList.SetItemText(k, 9, G_ReadChinese(Note));//6.2.0 From 8 to 9 For Dicom

			m_PatientsList.SetItemData(k, p);
			k++;
		}
	}

	//int Ut = (int)clock() - St;
	m_PatientsList.SortItems(CompareProc, (LPARAM)this);

	// highlighting must be done after sorting
	GUID ID = m_Patient.m_PatientID;

	if (m_ImportID != GUID_NULL)
	{
		ID = m_ImportID;
	}


	for (int i = 0; i < k; i++)
	{
		int p = (int)m_PatientsList.GetItemData(i);
		if (::DB.m_Patients[p].m_PatientID == ID)
		{
			m_PatientsList.SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			m_PatientsList.SetSelectionMark(i);
			m_PatientsList.EnsureVisible(i, FALSE);
			break;
		}
	}



	Invalidate(FALSE);
}

//***************************************************************************************

//wether the regtime is less than or eaual to the compared time
BOOL CMainWnd::LessTime(int Year, int Month, int Day, CString ComTime)
{
	CString ComMonthS = "0";
	CString ComDayS = "0";
	CString ComYearS = "0";
	int whichOne = 0;
	int Len = ComTime.GetLength();

	for (int i = 0; i < Len; i++)
	{
		CString a = ComTime.GetAt(i);

		if (a != "/")
		{
			if (whichOne == 0) 		 ComMonthS = ComMonthS + a;
			else if (whichOne == 1) ComDayS = ComDayS + a;
			else if (whichOne == 2) ComYearS = ComYearS + a;
		}
		else whichOne++;
	}

	G_As = ComYearS;
	int ComYear = atoi(G_As);
	G_As = ComMonthS;
	int ComMonth = atoi(G_As);
	G_As = ComDayS;
	int ComDay = atoi(G_As);

	if (Year < ComYear) return 1;
	else if (Year == ComYear)
	{
		if (Month < ComMonth) return 1;
		else if (Month == ComMonth)
		{
			if (Day <= ComDay) return 1;
			else return 0;
		}
		else return 0;
	}
	else return 0;
}

//***************************************************************************************

//wether the regtime is great than or eaual to the compared time
BOOL CMainWnd::GreatTime(int Year, int Month, int Day, CString ComTime)
{
	CString ComMonthS = "0";
	CString ComDayS = "0";
	CString ComYearS = "0";
	int whichOne = 0;
	int Len = ComTime.GetLength();
	for (int i = 0; i < Len; i++)
	{
		CString a = ComTime.GetAt(i);

		if (a != "/")
		{
			if (whichOne == 0) 		 ComMonthS = ComMonthS + a;
			else if (whichOne == 1) ComDayS = ComDayS + a;
			else if (whichOne == 2) ComYearS = ComYearS + a;
		}
		else whichOne++;
	}

	G_As = ComYearS;
	int ComYear = atoi(G_As);
	G_As = ComMonthS;
	int ComMonth = atoi(G_As);
	G_As = ComDayS;
	int ComDay = atoi(G_As);

	if (Year > ComYear) return 1;
	else if (Year == ComYear)
	{
		if (Month > ComMonth) return 1;
		else if (Month == ComMonth)
		{
			if (Day >= ComDay) return 1;
			else return 0;
		}
		else return 0;
	}
	else return 0;
}
//***************************************************************************************

void CMainWnd::DeleteLoadedExams()
{
	for (int i = 0; i < NUM_EXAMS_MAX; i++)
	{
		SAFE_DELETE(m_pWFExams[i]);
		SAFE_DELETE(m_pCTExams[i]);
	}
	m_NumLoadedWFODExams = 0;
	m_NumLoadedWFOSExams = 0;
	m_NumLoadedCTODExams = 0;
	m_NumLoadedCTOSExams = 0;
	m_NumLoadedWFExams = 0;
	m_NumLoadedCTExams = 0;
}

//***************************************************************************************

BOOL CMainWnd::LoadSelectedExams()
{
	DeleteLoadedExams();

	for (int i = 0; i < m_NumSelectedWFODExams; i++)
	{
		m_pWFExams[m_NumLoadedWFExams] = new CWFExam;
		if (!::DB.LoadWFExam(m_WFODID[i], m_pWFExams[m_NumLoadedWFExams]))
		{
			DeleteLoadedExams();
			::Error("Failed to load exams.");
			return FALSE;
		}
		m_NumLoadedWFODExams++;
		m_NumLoadedWFExams++;
	}

	for (int i = 0; i < m_NumSelectedWFOSExams; i++)
	{
		m_pWFExams[m_NumLoadedWFExams] = new CWFExam;
		if (!::DB.LoadWFExam(m_WFOSID[i], m_pWFExams[m_NumLoadedWFExams]))
		{
			DeleteLoadedExams();
			::Error("Failed to load exams.");
			return FALSE;
		}
		m_NumLoadedWFOSExams++;
		m_NumLoadedWFExams++;
	}

	for (int i = 0; i < m_NumSelectedCTODExams; i++)
	{
		m_pCTExams[m_NumLoadedCTExams] = new CCTExam;
		if (!::DB.LoadCTExam(m_CTODID[i], m_pCTExams[m_NumLoadedCTExams]))
		{
			DeleteLoadedExams();
			::Error("Failed to load exams.");
			return FALSE;
		}
		m_NumLoadedCTODExams++;
		m_NumLoadedCTExams++;
	}

	for (int i = 0; i < m_NumSelectedCTOSExams; i++)
	{
		m_pCTExams[m_NumLoadedCTExams] = new CCTExam;
		if (!::DB.LoadCTExam(m_CTOSID[i], m_pCTExams[m_NumLoadedCTExams]))
		{
			DeleteLoadedExams();
			::Error("Failed to load exams.");
			return FALSE;
		}
		m_NumLoadedCTOSExams++;
		m_NumLoadedCTExams++;
	}

	return TRUE;
}

//***************************************************************************************

BOOL CMainWnd::LoadFellowExam()
{
	if (m_NumLoadedWFExams == 1 && m_NumLoadedCTExams == 0)
	{
		if (m_pWFExams[0]->m_Header.m_FellowExamID == GUID_NULL) return TRUE;
		m_pCTExams[0] = new CCTExam;
		if (!::DB.LoadCTExam(m_pWFExams[0]->m_Header.m_FellowExamID, m_pCTExams[0]))
		{
			::Error("Failed to load exams.");
			DeleteLoadedExams();
			return FALSE;
		}
		if (m_NumLoadedWFODExams == 1) m_NumLoadedCTODExams = 1;
		else m_NumLoadedCTOSExams = 1;
		m_NumLoadedCTExams = 1;
	}

	else if (m_NumLoadedWFExams == 0 && m_NumLoadedCTExams == 1)
	{
		if (m_pCTExams[0]->m_Header.m_FellowExamID == GUID_NULL) return TRUE;
		m_pWFExams[0] = new CWFExam;
		if (!::DB.LoadWFExam(m_pCTExams[0]->m_Header.m_FellowExamID, m_pWFExams[0]))
		{
			::Error("Failed to load exams.");
			DeleteLoadedExams();
			return FALSE;
		}
		if (m_NumLoadedCTODExams == 1) m_NumLoadedWFODExams = 1;
		else m_NumLoadedWFOSExams = 1;
		m_NumLoadedWFExams = 1;
	}

	return TRUE;
}

//***************************************************************************************

void CMainWnd::OnNewWFExamButtonClicked()
{
  //::HW.AdjustHirCamer(WFEXAM);



	m_ExamImgInfo.ShowWindow(SW_HIDE);
	m_NoteImgInfo.ShowWindow(SW_HIDE);

	m_FellowIDCaled = FALSE;

	SAFE_DELETE(m_pSumWnd);
	SAFE_DELETE(m_BigCTSumWnd);

	m_WhichTaskBtn = -1;

	m_Screen = SCREEN_NONE;

	HidePatientLevelControls();

	HideAllSummaryLevelButtons();

	DeleteSelectedIndicesLists();

	DeleteLoadedExams();

	m_Level = LEVEL_VIDEO;
	Invalidate(FALSE);

	CWFAcquisition* pWFAcquisition = new CWFAcquisition(&::HW, &::Settings, FALSE);

	CWFAcquisitionDlg* pWFAcquisitionDlg = new CWFAcquisitionDlg(this, pWFAcquisition, TRUE);

	BOOL Res = pWFAcquisitionDlg->DoModal() == IDOK;

	//6.2.0 For laser safty check
	if (!pWFAcquisitionDlg->m_LaserSafe)
	{
		CString s;
		s = "There was a problem verifying the laser safety mechanism.Please restart the iTrace device and software.\n\nIf this message appears again after restarting, please call Tracey Technical Service at + 1 281 - 445 - 1666.\n\nClicking OK below will close the iTrace software.";
		::Info(s);

		delete pWFAcquisition;
		delete pWFAcquisitionDlg;

		PostMessage(WM_CLOSE, 0, 0);

		return;
	}
	//6.2.0 For laser safty check

	if (Res)
	{
		if (::NewSettings.m_Super_Exam)
		{
			for (int i = 0; i < 3; i++)
			{
				m_pWFExams[i] = new CWFExam;
				*m_pWFExams[i] = pWFAcquisition->m_mWFExams[i];
				m_ExcelSaved = FALSE;
			}
		}
		else
		{
			m_pWFExams[0] = new CWFExam;
			*m_pWFExams[0] = pWFAcquisition->m_WFExam;
			m_ExcelSaved = FALSE;
		}
	}
	delete pWFAcquisition;
	delete pWFAcquisitionDlg;

	if (!Res)
	{
		m_Level = LEVEL_PATIENT;
		Invalidate(FALSE);

		ShowPatientLevelControls();

		UpdateExamsLists();

		return;
	}

	CBusyCursor Cursor;

	m_Level = LEVEL_SUMMARY;
	Invalidate(FALSE);

	m_pWFExams[0]->m_Header.m_PatientID = m_Patient.m_PatientID;
	m_pWFExams[0]->Process();

	//for average exam
	CString ExamId, LoadFileName;
	GUIDToStr(m_pWFExams[0]->m_Header.m_ExamID, ExamId);
	LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

	if (::PathFileExists(LoadFileName))
	{
		CAveExamInfo AveExamInfo;

		AveExamInfo.LoadFromFile(LoadFileName);
		for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
		{
			m_pWFExams[0]->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
		}
	}
	//average exam Done

	//Super exam (3 times)
	if (::NewSettings.m_Super_Exam)
	{
		m_pWFExams[1]->m_Header.m_PatientID = m_Patient.m_PatientID;
		m_pWFExams[1]->Process();

		//average exam
		CString ExamId, LoadFileName;
		GUIDToStr(m_pWFExams[1]->m_Header.m_ExamID, ExamId);
		LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

		if (::PathFileExists(LoadFileName))
		{
			CAveExamInfo AveExamInfo;

			AveExamInfo.LoadFromFile(LoadFileName);
			for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
			{
				m_pWFExams[1]->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
			}
		}
		//average exam Done

		m_pWFExams[2]->m_Header.m_PatientID = m_Patient.m_PatientID;
		m_pWFExams[2]->Process();

		//average exam
		GUIDToStr(m_pWFExams[2]->m_Header.m_ExamID, ExamId);
		LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

		if (::PathFileExists(LoadFileName))
		{
			CAveExamInfo AveExamInfo;

			AveExamInfo.LoadFromFile(LoadFileName);
			for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
			{
				m_pWFExams[2]->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
			}
		}
		//average exam Done
	}
	//Super exam (3 times) Done

	::TempSettings.CTSumNum = -1;

	m_whichWFExam = 0;

	m_pSumWnd = new CWFSingleVrfWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], &::Settings.m_TSvrSettings[0], m_whichWFExam, ::NewSettings.m_Super_Exam);

	m_Screen = SCREEN_WFVRF;

	Invalidate(FALSE);

	m_AcceptExamButton.ShowWindow(SW_SHOW);
	m_RejectExamButton.ShowWindow(SW_SHOW);

	if (::Settings.m_AnimateAfterNewExam) ((CWFSingleVrfWnd*)m_pSumWnd)->OnF7();

	HideTaskButtons();
}

//***************************************************************************************

void CMainWnd::OnWFODExamListColumnClicked(NMHDR* pNMHDR, LRESULT* pRes)
{
	GUID FelRowId;

	//remmber the WFOD exam ID which has been selected as a fellow exam
	if (m_WFODList.m_FelRow != -1)
	{
		int e = m_WFODList.GetItemData(m_WFODList.m_FelRow);
		FelRowId = ::DB.m_ExamHeaders[e].m_ExamID;
	}

	m_WFODList.SetSortItem(((NMLISTVIEW*)pNMHDR)->iSubItem);

	m_WFODList.SortItems(CompareWFODExamProc, (LPARAM)this);

	int mark = m_WFODList.GetSelectionMark();
	if (mark != -1) m_WFODList.EnsureVisible(mark, FALSE);

	*pRes = 0;

	if (m_WFODList.m_FelRow != -1)
	{
		int n = m_WFODList.GetItemCount();
		for (int i = 0; i < n; i++)
		{
			int e = m_WFODList.GetItemData(i);
			GUID TestId = ::DB.m_ExamHeaders[e].m_ExamID;
			if (::DB.m_ExamHeaders[e].m_ExamID == FelRowId)
			{
				m_WFODList.m_FelRow = i;
				break;
			}
		}
		m_WFODList.Invalidate(false);
	}
}

//***************************************************************************************

void CMainWnd::OnWFOSExamListColumnClicked(NMHDR* pNMHDR, LRESULT* pRes)
{
	GUID FelRowId;

	//remmber the WFOD exam ID which has been selected as a fellow exam
	if (m_WFOSList.m_FelRow != -1)
	{
		int e = m_WFOSList.GetItemData(m_WFOSList.m_FelRow);
		FelRowId = ::DB.m_ExamHeaders[e].m_ExamID;
	}

	m_WFOSList.SetSortItem(((NMLISTVIEW*)pNMHDR)->iSubItem);

	m_WFOSList.SortItems(CompareWFOSExamProc, (LPARAM)this);

	int mark = m_WFOSList.GetSelectionMark();
	if (mark != -1) m_WFOSList.EnsureVisible(mark, FALSE);

	*pRes = 0;

	if (m_WFOSList.m_FelRow != -1)
	{
		int n = m_WFOSList.GetItemCount();
		for (int i = 0; i < n; i++)
		{
			int e = m_WFOSList.GetItemData(i);
			GUID TestId = ::DB.m_ExamHeaders[e].m_ExamID;
			if (::DB.m_ExamHeaders[e].m_ExamID == FelRowId)
			{
				m_WFOSList.m_FelRow = i;
				break;
			}
		}
		m_WFOSList.Invalidate(false);
	}
}

//***************************************************************************************

void CMainWnd::OnCTODExamListColumnClicked(NMHDR* pNMHDR, LRESULT* pRes)
{
	GUID FelRowId;

	//remmber the WFOD exam ID which has been selected as a fellow exam
	if (m_CTODList.m_FelRow != -1)
	{
		int e = m_CTODList.GetItemData(m_CTODList.m_FelRow);
		FelRowId = ::DB.m_ExamHeaders[e].m_ExamID;
	}

	m_CTODList.SetSortItem(((NMLISTVIEW*)pNMHDR)->iSubItem);

	m_CTODList.SortItems(CompareCTODExamProc, (LPARAM)this);

	int mark = m_CTODList.GetSelectionMark();
	if (mark != -1) m_CTODList.EnsureVisible(mark, FALSE);

	*pRes = 0;

	if (m_CTODList.m_FelRow != -1)
	{
		int n = m_CTODList.GetItemCount();
		for (int i = 0; i < n; i++)
		{
			int e = m_CTODList.GetItemData(i);
			GUID TestId = ::DB.m_ExamHeaders[e].m_ExamID;
			if (::DB.m_ExamHeaders[e].m_ExamID == FelRowId)
			{
				m_CTODList.m_FelRow = i;
				break;
			}
		}
		m_CTODList.Invalidate(false);
	}
}

//***************************************************************************************

void CMainWnd::OnCTOSExamListColumnClicked(NMHDR* pNMHDR, LRESULT* pRes)
{
	GUID FelRowId;

	//remmber the WFOD exam ID which has been selected as a fellow exam
	if (m_CTOSList.m_FelRow != -1)
	{
		int e = m_CTOSList.GetItemData(m_CTOSList.m_FelRow);
		FelRowId = ::DB.m_ExamHeaders[e].m_ExamID;
	}

	m_CTOSList.SetSortItem(((NMLISTVIEW*)pNMHDR)->iSubItem);

	m_CTOSList.SortItems(CompareCTOSExamProc, (LPARAM)this);

	int mark = m_CTOSList.GetSelectionMark();
	if (mark != -1) m_CTOSList.EnsureVisible(mark, FALSE);

	*pRes = 0;

	if (m_CTOSList.m_FelRow != -1)
	{
		int n = m_CTOSList.GetItemCount();
		for (int i = 0; i < n; i++)
		{
			int e = m_CTOSList.GetItemData(i);
			GUID TestId = ::DB.m_ExamHeaders[e].m_ExamID;
			if (::DB.m_ExamHeaders[e].m_ExamID == FelRowId)
			{
				m_CTOSList.m_FelRow = i;
				break;
			}
		}
		m_CTOSList.Invalidate(false);
	}
}

//***************************************************************************************

void CMainWnd::OnNewCTExamButtonClicked()
{
	m_ExamImgInfo.ShowWindow(SW_HIDE);
	m_NoteImgInfo.ShowWindow(SW_HIDE);

	m_FellowIDCaled = FALSE;

	::NewSettings.m_Adjust_CT = FALSE;

	SAFE_DELETE(m_pSumWnd);
	SAFE_DELETE(m_BigCTSumWnd);

	m_WhichTaskBtn = -1;

	m_Screen = SCREEN_NONE;

	HidePatientLevelControls();

	HideAllSummaryLevelButtons();

	DeleteSelectedIndicesLists();

	DeleteLoadedExams();

	m_Level = LEVEL_VIDEO;
	Invalidate(FALSE);

	CCTAcquisition* pCTAcquisition = new CCTAcquisition(&::HW, 0);

	CCTAcquisitionDlg* pCTAcquisitionDlg = new CCTAcquisitionDlg(this, pCTAcquisition);

	BOOL Res = pCTAcquisitionDlg->DoModal() == IDOK;

	if (Res)
	{
		m_pCTExams[0] = new CCTExam;
		*m_pCTExams[0] = pCTAcquisition->m_CTExam;
		m_ExcelSaved = FALSE;
	}

	delete pCTAcquisition;
	delete pCTAcquisitionDlg;

	if (!Res)
	{
		m_Level = LEVEL_PATIENT;
		Invalidate(FALSE);

		ShowPatientLevelControls();

		UpdateExamsLists();

		return;
	}

	CBusyCursor Cursor;

	m_Level = LEVEL_SUMMARY;
	Invalidate(FALSE);

	m_pCTExams[0]->m_Header.m_PatientID = m_Patient.m_PatientID;
	m_pCTExams[0]->Process();

	::TempSettings.CTSumNum = -1;

	m_whichWFExam = 0;

	m_pSumWnd = new CCTSingleVrfWnd(this, m_SumRect, &m_Patient, m_pCTExams[0], &::Settings.m_CSvrSettings);
	m_Screen = SCREEN_CTVRF;

	m_AcceptExamButton.ShowWindow(SW_SHOW);
	m_RejectExamButton.ShowWindow(SW_SHOW);
}

//***************************************************************************************

void CMainWnd::OnNewWFCTExamButtonClicked()
{
	m_ExamImgInfo.ShowWindow(SW_HIDE);
	m_NoteImgInfo.ShowWindow(SW_HIDE);

	m_FellowIDCaled = FALSE;

	::NewSettings.m_Adjust_CT = FALSE;

	SAFE_DELETE(m_pSumWnd);

	SAFE_DELETE(m_BigCTSumWnd);

	m_Screen = SCREEN_NONE;

	m_WhichTaskBtn = -1;

	HidePatientLevelControls();

	HideAllSummaryLevelButtons();

	DeleteSelectedIndicesLists();

	DeleteLoadedExams();

	m_Level = LEVEL_VIDEO;
	Invalidate(FALSE);

	//WF and CT dual exam will always keep the WF exam use 4 IR Dot alignment
	int oldAliMethod = ::Settings.m_AlignmentMethod;
	::Settings.m_AlignmentMethod = ALIGNMENT_4_IR_DOTS;
	//WF and CT dual exam will always keep the WF exam use 4 IR Dot alignment Done

	CWFAcquisition* pWFAcquisition = new CWFAcquisition(&::HW, &::Settings, FALSE);

	CWFAcquisitionDlg* pWFAcquisitionDlg = new CWFAcquisitionDlg(this, pWFAcquisition, TRUE);

	BOOL Res = pWFAcquisitionDlg->DoModal() == IDOK;

	//6.2.0 For laser safty check
	if (!pWFAcquisitionDlg->m_LaserSafe)
	{
		CString s;
		s = "There was a problem verifying the laser safety mechanism.Please restart the iTrace device and software.\n\nIf this message appears again after restarting, please call Tracey Technical Service at + 1 281 - 445 - 1666.\n\nClicking OK below will close the iTrace software.";
		::Info(s);

		delete pWFAcquisition;
		delete pWFAcquisitionDlg;

		PostMessage(WM_CLOSE, 0, 0);
		return;
	}
	//6.2.0 For laser safty check

	if (Res)
	{
		if (::NewSettings.m_Super_Exam)
		{
			for (int i = 0; i < 3; i++)
			{
				m_pWFExams[i] = new CWFExam;
				*m_pWFExams[i] = pWFAcquisition->m_mWFExams[i];
				m_ExcelSaved = FALSE;
			}
		}
		else
		{
			m_pWFExams[0] = new CWFExam;
			*m_pWFExams[0] = pWFAcquisition->m_WFExam;
			m_ExcelSaved = FALSE;
		}
	}
	delete pWFAcquisition;

	if (!Res)
	{
		m_Level = LEVEL_PATIENT;
		Invalidate(FALSE);

		ShowPatientLevelControls();

		UpdateExamsLists();

		//WF and CT dual exam will always keep the WF exam use 4 IR Dot alignment
		::Settings.m_AlignmentMethod = oldAliMethod;
		//WF and CT dual exam will always keep the WF exam use 4 IR Dot alignment Done

		return;
	}


	CCTAcquisition* pCTAcquisition = new CCTAcquisition(&::HW, 0);

	CCTAcquisitionDlg* pCTAcquisitionDlg = new CCTAcquisitionDlg(this, pCTAcquisition);

	Res = pCTAcquisitionDlg->DoModal() == IDOK;

	if (Res) {
		m_pCTExams[0] = new CCTExam;
		*m_pCTExams[0] = pCTAcquisition->m_CTExam;
	}

	delete pCTAcquisition;
	delete pCTAcquisitionDlg;

	::Settings.m_AlignmentMethod = oldAliMethod;//WF and CT dual exam will always keep the WF exam use 4 IR Dot alignment

	if (!Res)
	{
		m_Level = LEVEL_PATIENT;
		Invalidate(FALSE);

		ShowPatientLevelControls();

		UpdateExamsLists();

		return;
	}

	CBusyCursor Cursor;

	m_Level = LEVEL_SUMMARY;
	Invalidate(FALSE);

	m_pWFExams[0]->m_Header.m_PatientID = m_Patient.m_PatientID;
	//m_pWFExams[0]->m_Header.m_FellowExamID = m_pCTExams[0]->m_Header.m_ExamID;
	m_pWFExams[0]->Process();

	//average exam
	CString ExamId, LoadFileName;
	GUIDToStr(m_pWFExams[0]->m_Header.m_ExamID, ExamId);
	LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

	if (::PathFileExists(LoadFileName))
	{
		CAveExamInfo AveExamInfo;

		AveExamInfo.LoadFromFile(LoadFileName);
		for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
		{
			m_pWFExams[0]->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
		}
	}
	//average exam Done

	if (::NewSettings.m_Super_Exam)
	{
		m_pWFExams[1]->m_Header.m_PatientID = m_Patient.m_PatientID;
		m_pWFExams[1]->Process();

		//average exam
		CString ExamId, LoadFileName;
		GUIDToStr(m_pWFExams[1]->m_Header.m_ExamID, ExamId);
		LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

		if (::PathFileExists(LoadFileName))
		{
			CAveExamInfo AveExamInfo;

			AveExamInfo.LoadFromFile(LoadFileName);
			for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
			{
				m_pWFExams[1]->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
			}
		}
		//average exam Done

		m_pWFExams[2]->m_Header.m_PatientID = m_Patient.m_PatientID;
		m_pWFExams[2]->Process();

		//average exam
		GUIDToStr(m_pWFExams[2]->m_Header.m_ExamID, ExamId);
		LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

		if (::PathFileExists(LoadFileName))
		{
			CAveExamInfo AveExamInfo;

			AveExamInfo.LoadFromFile(LoadFileName);
			for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
			{
				m_pWFExams[2]->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
			}
		}
		//average exam Done
	}
	//Done

	m_pCTExams[0]->m_Header.m_PatientID = m_Patient.m_PatientID;
	//m_pCTExams[0]->m_Header.m_FellowExamID = m_pWFExams[0]->m_Header.m_ExamID;
	m_pCTExams[0]->Process();

	::TempSettings.CTSumNum = -1;//[520]

	m_whichWFExam = 0;//531

	m_pSumWnd = new CWFSingleVrfWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], &::Settings.m_TSvrSettings[0], m_whichWFExam, ::NewSettings.m_Super_Exam);
	m_Screen = SCREEN_WFVRF;

	m_AcceptExamButton.ShowWindow(SW_SHOW);
	m_RejectExamButton.ShowWindow(SW_SHOW);
}

//***************************************************************************************

void CMainWnd::OnViewExamButtonClicked()
{
	m_WhichTaskBtn = -1;

	m_ExamImgInfo.ShowWindow(SW_HIDE);
	m_NoteImgInfo.ShowWindow(SW_HIDE);
	CreateSelectedIndicesLists();

	::OD_AverageExam = 0;
	::OS_AverageExam = 0;

	if (m_NumSelectedWFExams == 0 && m_NumSelectedCTExams == 0)
	{
		::Error("No exam selected.");
		return;
	}

	if (m_NumSelectedWFODExams > 1 && m_NumSelectedWFOSExams > 0 ||
		m_NumSelectedWFODExams > 1 && m_NumSelectedCTODExams > 0 ||
		m_NumSelectedWFODExams > 0 && m_NumSelectedCTOSExams > 0 ||
		m_NumSelectedWFOSExams > 1 && m_NumSelectedWFODExams > 0 ||
		m_NumSelectedWFOSExams > 1 && m_NumSelectedCTOSExams > 0 ||
		m_NumSelectedWFOSExams > 0 && m_NumSelectedCTODExams > 0 ||
		m_NumSelectedCTODExams > 1 ||
		m_NumSelectedCTOSExams > 1)
	{
		::Error("Invalid exam selection.");
		return;
	}

	if (m_NumSelectedWFExams > NUM_EXAMS_MAX)
	{
		::Error("Too many exams selected.");
		return;
	}

	CBusyCursor Cursor;

	if (!LoadSelectedExams()) return;

	if (m_NumLoadedWFExams == 1 && m_NumLoadedCTExams == 0 ||
		m_NumLoadedWFExams == 0 && m_NumLoadedCTExams == 1)
	{
		if (!LoadFellowExam()) return;
	}

	::Settings.SetSomeDefault();

	if (::NewSettings.m_OWFCTSettings[0].m_Type <= 0 || ::NewSettings.m_OWFCTSettings[0].m_Type > 52)
	{
		::NewSettings.m_OWFCTSettings[0] = ::Settings.m_CSsmSettings[0][0];
		::NewSettings.m_OWFCTSettings[1] = ::Settings.m_TSsmSettings[0][0];
		::NewSettings.m_OWFCTSettings[2] = ::Settings.m_TSsmSettings[0][2];
		::NewSettings.m_OWFCTSettings[2].m_LtrLine = 100;
	}

	if (::NewSettings.m_IOLSelSettings[0].m_Type <= 0 || ::NewSettings.m_IOLSelSettings[0].m_Type > 52)
	{
		::NewSettings.m_IOLSelSettings[0] = ::Settings.m_CSsmSettings[0][0];
		::NewSettings.m_IOLSelSettings[1] = ::Settings.m_TSsmSettings[0][0];
		::NewSettings.m_IOLSelSettings[2] = ::Settings.m_TSsmSettings[0][2];
		::NewSettings.m_IOLSelSettings[2].m_LtrLine = 100;
	}

	HidePatientLevelControls();

	m_Level = LEVEL_SUMMARY;
	Invalidate(FALSE);

	//extract the (physician Name/Study date and time) for DICOM file
	CString StudyDateStr1;
	CString StudyDateStr2;
	CString StudyTimeStr1;
	CString StudyTimeStr2;
	int StudyDate1 = 30000000;
	int StudyDate2 = 30000000;
	int StudyTime1 = 10000;
	int StudyTime2 = 10000;

	if (m_NumLoadedWFExams > 0)
	{
		StudyDate1 = m_pWFExams[0]->m_Header.m_Year * 10000 + m_pWFExams[0]->m_Header.m_Month * 100 + m_pWFExams[0]->m_Header.m_Day;
		StudyTime1 = m_pWFExams[0]->m_Header.m_Hour * 10000 + m_pWFExams[0]->m_Header.m_Minute * 100 + m_pWFExams[0]->m_Header.m_Second;
		StudyDateStr1.Format(_T("%04i%02i%02i"), m_pWFExams[0]->m_Header.m_Year, m_pWFExams[0]->m_Header.m_Month, m_pWFExams[0]->m_Header.m_Day);
		StudyTimeStr1.Format(_T("%02i%02i%02i"), m_pWFExams[0]->m_Header.m_Hour, m_pWFExams[0]->m_Header.m_Minute, m_pWFExams[0]->m_Header.m_Second);

		GUID  PhysicianID = m_pWFExams[0]->m_Header.m_PhysicianID;
		if (PhysicianID != GUID_NULL)
		{
			CPhysician* pPhysician = ::DB.GetPhysician(PhysicianID);
			if (pPhysician)
			{
				m_DicomInfo.m_Physician_Name = pPhysician->m_LastName + ". " + pPhysician->m_FirstName;
			}
			else
			{
				m_DicomInfo.m_Physician_Name = "";
			}
		}
		else
		{
			m_DicomInfo.m_Physician_Name = "";
		}

		GUID  ClinicID = m_pWFExams[0]->m_Header.m_ClinicID;
		if (ClinicID != GUID_NULL)
		{
			CClinic* Clinic = ::DB.GetClinic(ClinicID);
			if (Clinic)
			{
				m_DicomInfo.m_Equi_Institution_Name = Clinic->m_Name;
			}
			else
			{
				m_DicomInfo.m_Equi_Institution_Name = "";
			}
		}
		else
		{
			m_DicomInfo.m_Equi_Institution_Name = "";
		}
	}

	if (m_NumLoadedCTExams > 0)
	{
		StudyDate2 = m_pCTExams[0]->m_Header.m_Year * 10000 + m_pCTExams[0]->m_Header.m_Month * 100 + m_pCTExams[0]->m_Header.m_Day;
		StudyTime2 = m_pCTExams[0]->m_Header.m_Hour * 10000 + m_pCTExams[0]->m_Header.m_Minute * 100 + m_pCTExams[0]->m_Header.m_Second;
		StudyDateStr2.Format(_T("%04i%02i%02i"), m_pCTExams[0]->m_Header.m_Year, m_pCTExams[0]->m_Header.m_Month, m_pCTExams[0]->m_Header.m_Day);
		StudyTimeStr2.Format(_T("%02i%02i%02i"), m_pCTExams[0]->m_Header.m_Hour, m_pCTExams[0]->m_Header.m_Minute, m_pCTExams[0]->m_Header.m_Second);

		if (m_DicomInfo.m_Physician_Name == "")
		{
			GUID  PhysicianID = m_pCTExams[0]->m_Header.m_PhysicianID;

			if (PhysicianID != GUID_NULL)
			{
				CPhysician* pPhysician = ::DB.GetPhysician(PhysicianID);
				if (pPhysician)
				{
					m_DicomInfo.m_Physician_Name = pPhysician->m_LastName + ". " + pPhysician->m_FirstName;
				}
				else
				{
					m_DicomInfo.m_Physician_Name = "";
				}
			}
			else
			{
				m_DicomInfo.m_Physician_Name = "";
			}
		}

		if (m_DicomInfo.m_Equi_Institution_Name == "")
		{
			GUID  ClinicID = m_pCTExams[0]->m_Header.m_ClinicID;
			if (ClinicID != GUID_NULL)
			{
				CClinic* Clinic = ::DB.GetClinic(ClinicID);
				if (Clinic)
				{
					m_DicomInfo.m_Equi_Institution_Name = Clinic->m_Name;
				}
				else
				{
					m_DicomInfo.m_Equi_Institution_Name = "";
				}
			}
			else
			{
				m_DicomInfo.m_Equi_Institution_Name = "";
			}
		}
	}

	if (StudyDate1 < StudyDate2)
	{
		m_DicomInfo.m_Study_Date = StudyDateStr1;
		m_DicomInfo.m_Study_Time = StudyTimeStr1;
	}
	else if (StudyDate1 > StudyDate2)
	{
		m_DicomInfo.m_Study_Date = StudyDateStr2;
		m_DicomInfo.m_Study_Time = StudyTimeStr2;
	}
	else
	{
		m_DicomInfo.m_Study_Date = StudyDateStr1;
		m_DicomInfo.m_Study_Time = StudyTimeStr1;

		if (StudyTimeStr1 > StudyTimeStr2)
		{
			m_DicomInfo.m_Study_Date = StudyDateStr2;
			m_DicomInfo.m_Study_Time = StudyTimeStr2;
		}
	}
	// Dicom Done

	for (int i = 0; i < m_NumLoadedWFExams; i++)
	{
		m_pWFExams[i]->Process();

		//average exam
		CString ExamId, LoadFileName;
		GUIDToStr(m_pWFExams[i]->m_Header.m_ExamID, ExamId);
		LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

		if (::PathFileExists(LoadFileName))
		{
			CAveExamInfo AveExamInfo;

			AveExamInfo.LoadFromFile(LoadFileName);
			for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
			{
				m_pWFExams[i]->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
			}
		}
		//average exam Done
	}

	if (!m_isSolo)
	{
		for (int i = 0; i < m_NumLoadedCTExams; i++)
		{
			//m_pCTExams[i]->m_Image.m_ri_ok = FALSE;//6.3.0 test
            m_pCTExams[i]->Process();
		}

		//6.2.0 0601 for bug of some ct exam inlay_ok but r is wrong (less than 0)
		for (int i = 0; i < m_NumSelectedCTExams; i++)
		{
			BOOL Adjusted = FALSE;

			if (m_pCTExams[i]->m_Image.m_In_ok && m_pCTExams[i]->m_Image.m_In_r_um <= 0)
			{
				m_pCTExams[i]->m_Image.m_In_ok = FALSE;
				m_pCTExams[i]->m_ColorImage.m_In_ok = FALSE;

				Adjusted = TRUE;
			}

			if (m_pCTExams[i]->m_ColorImage.m_In_ok && m_pCTExams[i]->m_ColorImage.m_In_r_um <= 0)
			{
				m_pCTExams[i]->m_Image.m_In_ok = FALSE;
				m_pCTExams[i]->m_ColorImage.m_In_ok = FALSE;
				Adjusted = TRUE;
			}

			if (m_pCTExams[i]->m_Image.m_Ra_ok && m_pCTExams[i]->m_Image.m_Ra_r_um <= 0)
			{
				m_pCTExams[i]->m_Image.m_Ra_ok = FALSE;
				m_pCTExams[i]->m_ColorImage.m_Ra_ok = FALSE;
				Adjusted = TRUE;
			}

			if (m_pCTExams[i]->m_ColorImage.m_Ra_ok && m_pCTExams[i]->m_ColorImage.m_In_r_um <= 0)
			{
				m_pCTExams[i]->m_Image.m_Ra_ok = FALSE;
				m_pCTExams[i]->m_ColorImage.m_Ra_ok = FALSE;
				Adjusted = TRUE;
			}

			if (m_pCTExams[i]->m_Image.m_LRa_ok && (m_pCTExams[i]->m_Image.m_LRa_x0_um <= 0 || m_pCTExams[i]->m_Image.m_LRa_y0_um <= 0))
			{
				m_pCTExams[i]->m_Image.m_LRa_ok = FALSE;
				m_pCTExams[i]->m_ColorImage.m_LRa_ok = FALSE;
				Adjusted = TRUE;
			}

			if (m_pCTExams[i]->m_ColorImage.m_LRa_ok && (m_pCTExams[i]->m_ColorImage.m_LRa_x0_um <= 0 || m_pCTExams[i]->m_ColorImage.m_LRa_y0_um <= 0))
			{
				m_pCTExams[i]->m_Image.m_LRa_ok = FALSE;
				m_pCTExams[i]->m_ColorImage.m_LRa_ok = FALSE;
				Adjusted = TRUE;
			}

			if (Adjusted)
			{
				::DB.SaveCTExam(m_pCTExams[i]);
			}
		}
		//6.2.0
	}

	if (::Settings.m_Export_Excel == 1)
	{
		BOOL SaveExcel = TRUE;
		if (::Settings.m_EXCEL_WFCT_SETS[0] != '1' && ::Settings.m_EXCEL_WFCT_SETS[0] != '0')
		{
			CString s;
			s = "The spreadsheet settings is not set yet.  \nSet the spreadsheet now?  \n\n(You can also set the spreadsheet settings in the Settings dialog.)";
			if (::Question(s, 1, 0))
			{
				CExcelSettingsDlg2* pDlg = new CExcelSettingsDlg2(this, FALSE);

				if (pDlg->DoModal() == IDOK)
				{
				}
				delete pDlg;
			}
			else
			{
				SaveExcel = FALSE;
				::Info("The exam will not be saved to excel file.");
			}
		}

		if (SaveExcel)
		{
			if (m_NumSelectedWFExams > 0 && m_NumSelectedCTExams == 0)
			{
				CString FolderName;

				CINIParser IniPar;
				FolderName = IniPar.GetStrKey(IniSection, "Export Excel Folder", IniPath);	//6.2.0 ini registration

				FolderName += "\\610WFSpreadsheetRec.xlsx";

				if (::PathFileExists(FolderName))
				{
					while (1)
					{
						fstream iofile(FolderName, ios::in | ios::out | ios::binary);
						if (!iofile)
						{
							::Info("Please close " + FolderName);
						}
						else
						{
							break;
						}
					}
				}


				for (int i = 0; i < m_NumSelectedWFExams; i++)
				{
					int res = SaveWFExamToExcel(m_pWFExams[i], FolderName);
				}
			}
			else if (m_NumSelectedWFExams == 0 && m_NumSelectedCTExams > 0)
			{
				CString FolderName;

				CINIParser IniPar;
				FolderName = IniPar.GetStrKey(IniSection, "Export Excel Folder", IniPath);	//6.2.0 ini registration

				FolderName += "\\610CTSpreadsheetRec.xlsx";

				if (::PathFileExists(FolderName))
				{
					while (1)
					{
						fstream iofile(FolderName, ios::in | ios::out | ios::binary);
						if (!iofile)
						{
							::Info("Please close " + FolderName);
						}
						else
						{
							break;
						}
					}
				}

				for (int i = 0; i < m_NumSelectedCTExams; i++)
				{
					int res = SaveCTExamToExcel(m_pCTExams[i], FolderName);
				}
			}
			else if (m_NumSelectedWFExams == 1 && m_NumSelectedCTExams == 1)
			{
				CString FolderName;

				CINIParser IniPar;
				FolderName = IniPar.GetStrKey(IniSection, "Export Excel Folder", IniPath);	//6.2.0 ini registration
				FolderName += "\\610WFCTSpreadsheetRec.xlsx";

				if (::PathFileExists(FolderName))
				{
					while (1)
					{
						fstream iofile(FolderName, ios::in | ios::out | ios::binary);
						if (!iofile)
						{
							::Info("Please close " + FolderName);
						}
						else
						{
							break;
						}
					}
				}

				int res = SaveWFCTExamToExcel(m_pWFExams[0], m_pCTExams[0], FolderName);//OnViewExamButtonClicked
			}
			else
			{
			}
		}
	}


	if (((m_NumLoadedWFODExams > 1 && m_NumLoadedWFOSExams == 0) || (m_NumLoadedWFODExams == 0 && m_NumLoadedWFOSExams > 1)) && m_NumLoadedCTExams == 0)
	{
		for (int i = 0; i < m_NumLoadedWFExams - 1; i++)
		{
			for (int j = m_NumLoadedWFExams - 1; j > i; j--)
			{
				if (m_pWFExams[j]->m_Header.m_SeriesNumber < m_pWFExams[j - 1]->m_Header.m_SeriesNumber)
				{
					Swap(m_pWFExams[j - 1], m_pWFExams[j]);
				}
			}
		}

		//multiple selection of WF exams
		if (m_NumLoadedWFODExams > 1)
		{
			::OD_AverageExam = m_NumLoadedWFExams;

			for (int i = 0; i < m_NumLoadedWFExams; i++)
			{
				::OD_SelectExamsID[i] = m_pWFExams[i]->m_Header.m_ExamID;
			}
		}
		else
		{
			::OS_AverageExam = m_NumLoadedWFExams;

			for (int i = 0; i < m_NumLoadedWFExams; i++)
			{
				::OS_SelectExamsID[i] = m_pWFExams[i]->m_Header.m_ExamID;
			}
		}

		int NumWFSeries = 1;
		for (int i = 1; i < m_NumLoadedWFExams; i++)
		{
			if (m_pWFExams[i - 1]->m_Header.m_SeriesNumber != m_pWFExams[i]->m_Header.m_SeriesNumber) NumWFSeries++;
		}

		Matrix<int> NumExamsPerSeries(NumWFSeries);
		NumExamsPerSeries.Fill(1);
		int s = 0;
		for (int i = 1; i < m_NumLoadedWFExams; i++)
		{
			if (m_pWFExams[i]->m_Header.m_SeriesNumber != m_pWFExams[i - 1]->m_Header.m_SeriesNumber) s++;
			else NumExamsPerSeries[s]++;
		}

		CWFExam* pWFAvgExams[NUM_EXAMS_MAX];
		for (int i = 0; i < NUM_EXAMS_MAX; i++) pWFAvgExams[i] = NULL;
		int i = 0;
		for (int s = 0; s < NumWFSeries; s++)
		{
			pWFAvgExams[s] = new CWFExam;
			if (NumExamsPerSeries[s] == 1)
			{
				*pWFAvgExams[s] = *m_pWFExams[i++];
			}
			else
			{
				Matrix<CWFExam*> pWFSeriesExams(NumExamsPerSeries[s]);
				for (int j = 0; j < NumExamsPerSeries[s]; j++)
				{
					pWFSeriesExams[j] = m_pWFExams[i++];
				}
				pWFAvgExams[s]->Average(pWFSeriesExams);//view exam averaging, do not need save
			}
		}

		DeleteLoadedExams();

		for (int i = 0; i < NumWFSeries; i++)
		{
			m_pWFExams[i] = pWFAvgExams[i];
		}

		if (m_NumSelectedWFODExams > 0) m_NumLoadedWFODExams = NumWFSeries;
		else m_NumLoadedWFOSExams = NumWFSeries;
		m_NumLoadedWFExams = NumWFSeries;
	}

	//Study Des value for Dicom file
	if (m_NumLoadedWFExams == 1 && m_NumSelectedCTExams == 0)
	{
		if (m_NumLoadedWFODExams == 1) m_DicomInfo.m_StudyDes = "OD Wavefront";
		else m_DicomInfo.m_StudyDes = "OS Wavefront";
	}
	else if (m_NumLoadedWFODExams == 1 && m_NumLoadedWFOSExams == 1 && m_NumLoadedCTExams == 0)
	{
		m_DicomInfo.m_StudyDes = "OU Wavefront";
	}
	else if (m_NumLoadedWFODExams > 1 && m_NumLoadedWFOSExams == 0)
	{
		m_DicomInfo.m_StudyDes = "Multi OD Wavefront";
	}
	else if (m_NumLoadedWFODExams == 0 && m_NumLoadedWFOSExams > 1)
	{
		m_DicomInfo.m_StudyDes = "Multi OS Wavefront";
	}
	else if (m_NumSelectedWFExams == 0 && m_NumLoadedCTExams == 1)
	{
		if (m_NumLoadedCTODExams == 1) m_DicomInfo.m_StudyDes = "OD Corneal Topography";
		else m_DicomInfo.m_StudyDes = "OS Corneal Topography";
	}
	else if (m_NumLoadedCTODExams == 1 && m_NumLoadedCTOSExams == 1 && m_NumLoadedWFExams == 0)
	{
		m_DicomInfo.m_StudyDes = "OU Corneal Topography";
	}
	else if (m_NumLoadedWFExams == 1 && m_NumLoadedCTExams == 1)
	{
		if (m_NumLoadedCTODExams == 1) m_DicomInfo.m_StudyDes = "OD Wavefront and Corneal Topography";
		else m_DicomInfo.m_StudyDes = "OS Wavefront and Corneal Topography";
	}
	else m_DicomInfo.m_StudyDes = "";
	//Study Des value for Dicom file Done

	// WF single
	if (m_NumLoadedWFExams == 1 && m_NumSelectedCTExams == 0)
	{
		m_EnableToggleButton = m_NumLoadedCTExams == 1;
		m_EnablePlannerButton = m_NumLoadedCTExams == 1;
		m_EnableWavetouchButtons = TRUE;
		m_EnableEditExamButton = TRUE;
		m_EnableExportDataButton = TRUE;

		ShowCommonSummaryLevelButtons();
		ShowWFSummaryLevelButtons();

		if (!m_isSolo)//521
		{
			if (::Settings.m_DefaultWFSum >= 0 && ::Settings.m_DefaultWFSum <= 3)
			{
				if (::Settings.m_DefaultWFSum != 0) m_ReselectExam = TRUE;
				int num = ::Settings.m_DefaultWFSum;
				if (num == 3) num = 4;

				if (m_isCombo)
				{
					if (num == 1) num = 4;
					else if (num == 2) num = 1;
					else if (num == 4) num = 5;
				}

				OnWFSumButtonClicked(num);
			}
			else if (::Settings.m_DefaultWFSum == 4)
			{
				//depth of focus
				m_Screen = SCREEN_WFSUM3;
				m_pSumWnd = new CWFSingleSumWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], &::NewSettings.m_TSsm5Settings[0], 4);
				HighlightSummaryLevelButtons();
			}
			else if (::Settings.m_DefaultWFSum == 10)
			{
				m_ReselectExam = TRUE;
				OnWFVfaButtonClicked();
			}
			else if (::Settings.m_DefaultWFSum == 12)
			{
				m_ReselectExam = TRUE;
				OnWFCmpButtonClicked();
			}
			else if (::Settings.m_DefaultWFSum == 13)
			{
				OnWFSumButtonClicked(3);
			}
			else if (::Settings.m_DefaultWFSum == 14)//6.2.0 WF Near Vision
			{
				OnWFSumButtonClicked(6);
			}
			else
			{
				m_ReselectExam = TRUE;
				::Settings.m_DefaultWFSum = 11;
				OnWFVrfButtonClicked();
			}
		}
		else
		{
			m_SoloControl = FALSE;
			m_WFVrfButton.SetEnabled(TRUE);
			m_WFCmpButton.SetEnabled(TRUE);
			m_WFSumButton[0].SetEnabled(TRUE);
			m_ReselectExam = TRUE;

			switch (::NewSettings.m_DefaultWFSolo)
			{
			case 0: OnWFVrfButtonClicked();//Point
				break;
			case 1: //Refraction
				OnWFSumButtonClicked(1);
				break;
			case 2: //Aberration
				OnWFSumButtonClicked(0);
				break;
			case 3: //VFA
				OnWFVfaButtonClicked();
				break;
			}
		}
	}

	// WF double
	else if (m_NumLoadedWFODExams == 1 && m_NumLoadedWFOSExams == 1 && m_NumLoadedCTExams == 0)
	{
		m_EnableToggleButton = FALSE;
		m_EnablePlannerButton = FALSE;
		m_EnableWavetouchButtons = FALSE;
		m_EnableEditExamButton = FALSE;
		m_EnableExportDataButton = TRUE;

		ShowCommonSummaryLevelButtons();

		m_ReselectExam = FALSE;

		if (!m_isSolo)
			OnWFVfaButtonClicked();
		else
		{
			m_SoloControl = TRUE;
			m_WFVrfButton.SetEnabled(FALSE);
			m_WFCmpButton.SetEnabled(FALSE);
			m_WFSumButton[0].SetEnabled(FALSE);

			ShowWFSummaryLevelButtons();

			if (::NewSettings.m_DefaultWFSolo == 3)
				OnWFVfaButtonClicked();
			else
				OnWFSum2ButtonClicked();
		}
	}

	// WF multi
	else if (((m_NumLoadedWFODExams > 1 && m_NumLoadedWFOSExams == 0) || (m_NumLoadedWFODExams == 0 && m_NumLoadedWFOSExams > 1)) && (m_NumLoadedCTExams == 0))
	{
		m_EnableToggleButton = FALSE;
		m_EnablePlannerButton = FALSE;
		m_EnableWavetouchButtons = FALSE;
		m_EnableEditExamButton = FALSE;
		m_EnableExportDataButton = FALSE;

		ShowCommonSummaryLevelButtons();

		::TempSettings.CTSumNum = -1;

		if (m_BigCTSumWnd != NULL)	m_BigCTSumWnd->ShowWindow(SW_HIDE);

		m_pSumWnd = new CWFMultiSumWnd(this, m_SumRect, &m_Patient, m_NumLoadedWFExams, m_pWFExams, &::Settings.m_TMsmSettings);
		m_Screen = SCREEN_WFMLT;

		HighlightSummaryLevelButtons();
	}

	// CT single
	else if (m_NumSelectedWFExams == 0 && m_NumLoadedCTExams == 1)
	{
		m_EnableToggleButton = m_NumLoadedWFExams == 1;

		m_EnablePlannerButton = m_NumLoadedWFExams == 1;
		m_EnableWavetouchButtons = m_NumLoadedWFExams == 1;
		m_EnableEditExamButton = TRUE;
		m_EnableExportDataButton = TRUE;

		ShowCommonSummaryLevelButtons();
		ShowCTSummaryLevelButtons();

		if (::Settings.m_DefaultCTSum >= 0 && ::Settings.m_DefaultCTSum <= 4)
		{
			OnCTSumButtonClicked(::Settings.m_DefaultCTSum);
		}
		else if (::Settings.m_DefaultCTSum == 10)
		{
			m_ReselectExam = TRUE;
			OnCTDblButtonClicked();
		}
		else if (::Settings.m_DefaultCTSum == 12)
		{
			m_ReselectExam = TRUE;
			OnCTCmpButtonClicked();
		}
		else
		{
			m_ReselectExam = TRUE;
			::Settings.m_DefaultCTSum = 11;
			OnCTVrfButtonClicked();
		}
	}

	// CT double
	else if (m_NumLoadedWFExams == 0 && m_NumLoadedCTODExams == 1 && m_NumLoadedCTOSExams == 1)
	{
		m_EnableToggleButton = FALSE;
		m_EnablePlannerButton = FALSE;
		m_EnableWavetouchButtons = FALSE;
		m_EnableEditExamButton = FALSE;
		m_EnableExportDataButton = TRUE;

		ShowCommonSummaryLevelButtons();

		m_ReselectExam = FALSE;
		OnCTDblButtonClicked();
	}

	// WF & CT
	else if (m_NumLoadedWFExams == 1 && m_NumLoadedCTExams == 1)
	{
		m_EnableToggleButton = TRUE;
		m_EnablePlannerButton = TRUE;
		m_EnableWavetouchButtons = TRUE;
		m_EnableEditExamButton = TRUE;
		m_EnableExportDataButton = TRUE;

		ShowCommonSummaryLevelButtons();
		ShowINSummaryLevelButtons();

		if (m_isCombo)
		{
			if (::Settings.m_DefaultINSum < 0 || ::Settings.m_DefaultINSum > 11)
			{
				::Settings.m_DefaultINSum = 6;
			}

			if (::Settings.m_DefaultINSum == 4)
			{
				OnINSum10ButtonClicked();//Angle K/A
			}
			else if (::Settings.m_DefaultINSum == 7)
			{
				OnINSum5ButtonClicked();//OU OverView
			}
			else
			{
				if (::Settings.m_DefaultINSum == 8)
				{
					OnINSumButtonClicked(7);
				}
				else if (::Settings.m_DefaultINSum == 9)
				{
					OnINSumButtonClicked(8);
				}
				else if (::Settings.m_DefaultINSum == 10 || ::Settings.m_DefaultINSum == 6)
				{
					if (m_pWFExams[0]->m_ScanDiameter < 2500)
					{
						::Info("The scan size is less than 2.50mm and cannot be used to generate the Dysfunctional Lens Index.\n Please re-acquire the exam if a DLI is desired.");

						BOOL FoundOther = FALSE;
						int pos, whichFun;
						CString s;
						for (int i = 0; i <= 8; i++)
						{
							if (::NewSettings.m_WFCTSumShow[i] && i != 5 && i != 6)
							{
								whichFun = i;
								pos = i;

								if (i == 8) pos = 3;
								if (i == 7) pos = 4;

								::Settings.GetButtonName(2, pos, s);

								if (i == 3) s = "OU Overview";
								if (i == 4) s = "IOL Selection Analysis";

								::Info("System will transform to " + s);

								FoundOther = TRUE;
								break;
							}
						}

						if (!FoundOther)
						{
							m_Level = LEVEL_SUMMARY;
							OnBackButtonClicked();
							return;
						}
						else
						{

							if (whichFun == 4)
							{
								OnINSum5ButtonClicked();
							}
							else OnINSumButtonClicked(whichFun);
						}
					}
					else OnINSumButtonClicked(::Settings.m_DefaultINSum);
				}
				else if (::Settings.m_DefaultINSum == 11)
				{
					OnINSum11ButtonClicked();
				}
				else OnINSumButtonClicked(::Settings.m_DefaultINSum);
			}
		}
		else
		{
			if (::Settings.m_DefaultINSum < 0 || ::Settings.m_DefaultINSum > 7)
			{
				::Settings.m_DefaultINSum = 6;
			}

			if (::Settings.m_DefaultINSum == 4)
			{
				OnINSum6ButtonClicked();//Go to IOL Selection
			}
			else if (::Settings.m_DefaultINSum == 7)
			{
				OnINSum5ButtonClicked();//OU OverView
			}
			else
			{
				OnINSumButtonClicked(::Settings.m_DefaultINSum);
			}
		}
	}
	else
	{
		m_pSumWnd = NULL;
		m_Screen = SCREEN_NONE;
	}

	m_TransType = 0;

	//Set the OD or OS for the toric planner
	if (m_NumLoadedWFODExams == 1 && m_NumLoadedCTODExams == 1)
		m_OD = 1;
	else m_OD = 0;
}

//***************************************************************************************

BOOL CMainWnd::OnViewExamBtnAClicked(int TaskNum)
{
	m_ExamImgInfo.ShowWindow(SW_HIDE);
	m_NoteImgInfo.ShowWindow(SW_HIDE);
	CreateSelectedIndicesLists();

	::OD_AverageExam = 0;
	::OS_AverageExam = 0;

	if (m_NumSelectedWFExams == 0 && m_NumSelectedCTExams == 0)
	{
		::Error("No exam selected.");
		return FALSE;
	}
	if (m_NumSelectedWFODExams > 1 && m_NumSelectedWFOSExams > 0 ||
		m_NumSelectedWFODExams > 1 && m_NumSelectedCTODExams > 0 ||
		m_NumSelectedWFODExams > 0 && m_NumSelectedCTOSExams > 0 ||
		m_NumSelectedWFOSExams > 1 && m_NumSelectedWFODExams > 0 ||
		m_NumSelectedWFOSExams > 1 && m_NumSelectedCTOSExams > 0 ||
		m_NumSelectedWFOSExams > 0 && m_NumSelectedCTODExams > 0 ||
		m_NumSelectedCTODExams > 1 ||
		m_NumSelectedCTOSExams > 1)
	{
		::Error("Invalid exam selection.");
		return FALSE;
	}

	if (m_NumSelectedWFExams > NUM_EXAMS_MAX)
	{
		::Error("Too many exams selected.");
		return FALSE;
	}
	if (!LoadSelectedExams()) return FALSE;

	if (m_NumLoadedWFExams == 1 && m_NumLoadedCTExams == 0 ||
		m_NumLoadedWFExams == 0 && m_NumLoadedCTExams == 1)
	{
		if (!LoadFellowExam())   return FALSE;
	}

	//Check the task  type to decide the exam selection
	//0: None; 1: one wf exam; 2: one CT exam; 3: one WF and one CT exam, or one wf exam(has fellow), or one ct exam(has fellow)
	int selType = 0;

	for (int i = 0; i < 8; i++)
	{
		switch (::NewSettings.m_Tasks[TaskNum].m_SubButton[i])
		{
		case TYPE_NONA:
			break;

		case TYPE_WCOU:
			selType = 3;
			break;

		case TYPE_REFA:
			if (selType == 0)  selType = 1;
			else if (selType == 2)  selType = 3;
			else                   selType = selType;
			break;

		case TYPE_ABEA:
			if (selType == 0)  selType = 1;
			else if (selType == 2)  selType = 3;
			else                   selType = selType;
			break;

			//6.2.0  Near Vision
		case TYPE_NEVI:
			if (selType == 0)  selType = 1;
			else if (selType == 2)  selType = 3;
			else                   selType = selType;
			break;

		case TYPE_WCVA:
			selType = 3;
			break;

		case TYPE_CTSU:
			if (selType == 0)  selType = 2;
			else if (selType == 1)  selType = 3;
			else                   selType = selType;
			break;

		case TYPE_VFA:
			if (selType == 0)  selType = 1;
			else if (selType == 2)  selType = 3;
			else                   selType = selType;
			break;

		case TYPE_KERA:
			if (selType == 0)  selType = 2;
			else if (selType == 1)  selType = 3;
			else                   selType = selType;
			break;

		case TYPE_CHAN:
			selType = 3;
			break;

		case TYPE_IOLS:
			selType = 3;
			break;

		case TYPE_ANGK:
			selType = 3;//WFCT Angle k/a
			break;

		case TYPE_TORI:
			selType = 3;
			break;

		case TYPE_ACCO:
			if (selType == 0)  selType = 1;
			else if (selType == 2)  selType = 3;
			else                   selType = selType;
			break;

		case TYPE_WFVA:
			if (selType == 0)  selType = 1;
			else if (selType == 2)  selType = 3;
			else                   selType = selType;
			break;

		case TYPE_WFRM:
			if (selType == 0)  selType = 1;
			else if (selType == 2)  selType = 3;
			else                   selType = selType;
			break;

		case TYPE_CTDS:
			if (selType == 0)  selType = 2;
			else if (selType == 1)  selType = 3;
			else                   selType = selType;
			break;

		case TYPE_CT3D:
			if (selType == 0)  selType = 2;
			else if (selType == 1)  selType = 3;
			else                   selType = selType;
			break;

		case TYPE_WCSU:
			selType = 3;
			break;

		case TYPE_CTCM:
			if (selType == 0)  selType = 2;
			else if (selType == 1)  selType = 3;
			else                   selType = selType;
			break;

		case TYPE_DYSF: //Dysfunctional lens Surgeon
			selType = 3;
			break;

		case TYPE_ASTI: //Astigmatism source
			selType = 3;
			break;

		case TYPE_DYSP: //Dysfunctional lens Patient
			selType = 3;
			break;

		case TYPE_DEPF://Depth of Focus
			if (selType == 0)  selType = 1;
			else if (selType == 2)  selType = 3;
			else                   selType = selType;
			break;

		case TYPE_TOAL:
			selType = 3;
			break;
		}
	}
	//Task Buttons Done

	if (selType == 0) return FALSE;
	else if (selType == 1)
	{
		if (m_NumLoadedWFExams != 1)
		{
			::Error("Please selct one WF Exam.");
			return FALSE;
		}
	}
	else if (selType == 2)
	{
		if (m_NumLoadedCTExams != 1)
		{
			::Error("Please selct one CT Exam.");
			return FALSE;
		}
	}
	else if (selType == 3)
	{
		if (!(m_NumLoadedWFExams == 1 && m_NumLoadedCTExams == 1))
		{
			::Error("Please select one WF Exam and one CT Exam, or one exam which is one exam of dual exam.");
			return FALSE;
		}
	}

	return TRUE;
}

//***************************************************************************************

void CMainWnd::OnViewExamBtnBClicked()
{
	::Settings.SetSomeDefault();

	if (::NewSettings.m_OWFCTSettings[0].m_Type <= 0 || ::NewSettings.m_OWFCTSettings[0].m_Type > 52)
	{
		::NewSettings.m_OWFCTSettings[0] = ::Settings.m_CSsmSettings[0][0];
		::NewSettings.m_OWFCTSettings[1] = ::Settings.m_TSsmSettings[0][0];
		::NewSettings.m_OWFCTSettings[2] = ::Settings.m_TSsmSettings[0][2];
		::NewSettings.m_OWFCTSettings[2].m_LtrLine = 100;
	}

	if (::NewSettings.m_IOLSelSettings[0].m_Type <= 0 || ::NewSettings.m_IOLSelSettings[0].m_Type > 52)
	{
		::NewSettings.m_IOLSelSettings[0] = ::Settings.m_CSsmSettings[0][0];
		::NewSettings.m_IOLSelSettings[1] = ::Settings.m_TSsmSettings[0][0];
		::NewSettings.m_IOLSelSettings[2] = ::Settings.m_TSsmSettings[0][2];
		::NewSettings.m_IOLSelSettings[2].m_LtrLine = 100;
	}

	HidePatientLevelControls();

	m_Level = LEVEL_SUMMARY;
	Invalidate(FALSE);

	//extract the (physician Name/Study date and time) for DICOM file
	CString StudyDateStr1;
	CString StudyDateStr2;
	CString StudyTimeStr1;
	CString StudyTimeStr2;
	int StudyDate1 = 30000000;
	int StudyDate2 = 30000000;
	int StudyTime1 = 10000;
	int StudyTime2 = 10000;

	if (m_NumLoadedWFExams > 0)
	{
		StudyDate1 = m_pWFExams[0]->m_Header.m_Year * 10000 + m_pWFExams[0]->m_Header.m_Month * 100 + m_pWFExams[0]->m_Header.m_Day;
		StudyTime1 = m_pWFExams[0]->m_Header.m_Hour * 10000 + m_pWFExams[0]->m_Header.m_Minute * 100 + m_pWFExams[0]->m_Header.m_Second;
		StudyDateStr1.Format(_T("%02i%02i%04i"), m_pWFExams[0]->m_Header.m_Month, m_pWFExams[0]->m_Header.m_Day, m_pWFExams[0]->m_Header.m_Year);
		StudyTimeStr1.Format(_T("%02i%02i%02i"), m_pWFExams[0]->m_Header.m_Hour, m_pWFExams[0]->m_Header.m_Minute, m_pWFExams[0]->m_Header.m_Second);

		GUID  PhysicianID = m_pWFExams[0]->m_Header.m_PhysicianID;
		if (PhysicianID != GUID_NULL)
		{
			CPhysician* pPhysician = ::DB.GetPhysician(PhysicianID);
			if (pPhysician) m_DicomInfo.m_Physician_Name = pPhysician->m_LastName + ". " + pPhysician->m_FirstName;
			else  m_DicomInfo.m_Physician_Name = "";
		}
		else  m_DicomInfo.m_Physician_Name = "";

		GUID  ClinicID = m_pWFExams[0]->m_Header.m_ClinicID;
		if (ClinicID != GUID_NULL)
		{
			CClinic* Clinic = ::DB.GetClinic(ClinicID);
			if (Clinic) m_DicomInfo.m_Equi_Institution_Name = Clinic->m_Name;
			else m_DicomInfo.m_Equi_Institution_Name = "";
		}
		else  m_DicomInfo.m_Equi_Institution_Name = "";
	}

	if (m_NumLoadedCTExams > 0)
	{
		StudyDate2 = m_pCTExams[0]->m_Header.m_Year * 10000 + m_pCTExams[0]->m_Header.m_Month * 100 + m_pCTExams[0]->m_Header.m_Day;
		StudyTime2 = m_pCTExams[0]->m_Header.m_Hour * 10000 + m_pCTExams[0]->m_Header.m_Minute * 100 + m_pCTExams[0]->m_Header.m_Second;
		StudyDateStr2.Format(_T("%02i%02i%04i"), m_pCTExams[0]->m_Header.m_Month, m_pCTExams[0]->m_Header.m_Day, m_pCTExams[0]->m_Header.m_Year);
		StudyTimeStr2.Format(_T("%02i%02i%02i"), m_pCTExams[0]->m_Header.m_Hour, m_pCTExams[0]->m_Header.m_Minute, m_pCTExams[0]->m_Header.m_Second);

		if (m_DicomInfo.m_Physician_Name == "")
		{
			GUID  PhysicianID = m_pCTExams[0]->m_Header.m_PhysicianID;

			if (PhysicianID != GUID_NULL)
			{
				CPhysician* pPhysician = ::DB.GetPhysician(PhysicianID);
				if (pPhysician) m_DicomInfo.m_Physician_Name = pPhysician->m_LastName + ". " + pPhysician->m_FirstName;
				else m_DicomInfo.m_Physician_Name = "";
			}
			else  m_DicomInfo.m_Physician_Name = "";
		}

		if (m_DicomInfo.m_Equi_Institution_Name == "")
		{
			GUID  ClinicID = m_pCTExams[0]->m_Header.m_ClinicID;
			if (ClinicID != GUID_NULL)
			{
				CClinic* Clinic = ::DB.GetClinic(ClinicID);
				if (Clinic) m_DicomInfo.m_Equi_Institution_Name = Clinic->m_Name;
				else  	    m_DicomInfo.m_Equi_Institution_Name = "";
			}
			else  m_DicomInfo.m_Equi_Institution_Name = "";
		}
	}

	if (StudyDate1 < StudyDate2)
	{
		m_DicomInfo.m_Study_Date = StudyDateStr1;
		m_DicomInfo.m_Study_Time = StudyTimeStr1;
	}
	else if (StudyDate1 > StudyDate2)
	{
		m_DicomInfo.m_Study_Date = StudyDateStr2;
		m_DicomInfo.m_Study_Time = StudyTimeStr2;
	}
	else
	{
		m_DicomInfo.m_Study_Date = StudyDateStr1;
		m_DicomInfo.m_Study_Time = StudyTimeStr1;

		if (StudyTimeStr1 > StudyTimeStr2)
		{
			m_DicomInfo.m_Study_Date = StudyDateStr2;
			m_DicomInfo.m_Study_Time = StudyTimeStr2;
		}
	}
	//extract the (physician Name/Study date and time) for DICOM file Done

	for (int i = 0; i < m_NumLoadedWFExams; i++)
	{
		m_pWFExams[i]->Process();

		//average exam
		CString ExamId, LoadFileName;
		GUIDToStr(m_pWFExams[i]->m_Header.m_ExamID, ExamId);
		LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

		if (::PathFileExists(LoadFileName))
		{
			CAveExamInfo AveExamInfo;

			AveExamInfo.LoadFromFile(LoadFileName);
			for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
			{
				m_pWFExams[i]->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
			}
		}
		//average exam Done
	}

	if (!m_isSolo)
	{
		for (int i = 0; i < m_NumLoadedCTExams; i++)
		{
			m_pCTExams[i]->Process();
		}
	}

	if (::Settings.m_Export_Excel == 1)
	{
		BOOL SaveExcel = TRUE;
		if (::Settings.m_EXCEL_WFCT_SETS[0] != '1' && ::Settings.m_EXCEL_WFCT_SETS[0] != '0')
		{
			CString s;
			s = "The spreadsheet settings is not set yet.  \nSet the spreadsheet now?  \n\n(You can also set the spreadsheet settings in the Settings dialog.)";
			if (::Question(s, 1, 0))
			{
				CExcelSettingsDlg2* pDlg = new CExcelSettingsDlg2(this, FALSE);

				if (pDlg->DoModal() == IDOK)
				{
				}
				delete pDlg;
			}
			else
			{
				SaveExcel = FALSE;
				::Info("The exam will not be saved to excel file.");
			}
		}

		if (SaveExcel)
		{
			if (m_NumSelectedWFExams > 0 && m_NumSelectedCTExams == 0)
			{
				CString FolderName;
				CINIParser IniPar;
				FolderName = IniPar.GetStrKey(IniSection, "Export Excel Folder", IniPath);	//6.2.0 ini registration

				FolderName += "\\610WFSpreadsheetRec.xlsx";

				if (::PathFileExists(FolderName))
				{
					while (1)
					{
						fstream iofile(FolderName, ios::in | ios::out | ios::binary);
						if (!iofile)
						{
							::Info("Please close " + FolderName);
						}
						else
						{
							break;
						}
					}
				}


				for (int i = 0; i < m_NumSelectedWFExams; i++)
				{
					int res = SaveWFExamToExcel(m_pWFExams[i], FolderName);
				}
			}
			else if (m_NumSelectedWFExams == 0 && m_NumSelectedCTExams > 0)
			{
				CString FolderName;
				CINIParser IniPar;
				FolderName = IniPar.GetStrKey(IniSection, "Export Excel Folder", IniPath);	//6.2.0 ini registration

				FolderName += "\\610CTSpreadsheetRec.xlsx";

				if (::PathFileExists(FolderName))
				{
					while (1)
					{
						fstream iofile(FolderName, ios::in | ios::out | ios::binary);
						if (!iofile)
						{
							::Info("Please close " + FolderName);
						}
						else
						{
							break;
						}
					}
				}

				for (int i = 0; i < m_NumSelectedCTExams; i++)
				{
					int res = SaveCTExamToExcel(m_pCTExams[i], FolderName);
				}
			}
			else if (m_NumSelectedWFExams == 1 && m_NumSelectedCTExams == 1)
			{
				CString FolderName;

				CINIParser IniPar;
				FolderName = IniPar.GetStrKey(IniSection, "Export Excel Folder", IniPath);	//6.2.0 ini registration
				FolderName += "\\610WFCTSpreadsheetRec.xlsx";

				if (::PathFileExists(FolderName))
				{
					while (1)
					{
						fstream iofile(FolderName, ios::in | ios::out | ios::binary);
						if (!iofile)
						{
							::Info("Please close " + FolderName);
						}
						else
						{
							break;
						}
					}
				}

				int res = SaveWFCTExamToExcel(m_pWFExams[0], m_pCTExams[0], FolderName);//OnViewExamBtnBClicked
			}
			else
			{
			}
		}
	}

	//Study Des value for Dicom file
	if (m_NumLoadedCTODExams == 1)
		m_DicomInfo.m_StudyDes = "OD Wavefront and Corneal Topography";
	else m_DicomInfo.m_StudyDes = "OS Wavefront and Corneal Topography";
	//Study Des value for Dicom file  Done
}

//***************************************************************************************

//Click task btn 1 Default is caracter
void CMainWnd::OnViewExam0BtnClicked()
{
	CBusyCursor Cursor;
	if (!OnViewExamBtnAClicked(0)) return;

	OnViewExamBtnBClicked();

	if (m_NumLoadedWFODExams == 1 && m_NumLoadedCTODExams == 1)  m_OD = 1;
	else m_OD = 0;

	m_WhichTaskBtn = 0;

	m_EnableWavetouchButtons = TRUE;
	m_EnableEditExamButton = TRUE;
	m_EnableExportDataButton = TRUE;

	ShowCommonSummaryLevelButtons();
	ShowTaskButtons(0);

	m_FirstTask = 0;

	int DefSubTask = ::NewSettings.m_Tasks[0].m_DefNum;
	OnTaskSubBtnClicked(DefSubTask);

	m_TransType = 0;//For DICOM
}

//***************************************************************************************

//Click task btn 2 Default is corneal diagnostics
void CMainWnd::OnViewExam1BtnClicked()
{
	CBusyCursor Cursor;
	if (!OnViewExamBtnAClicked(1)) return;

	OnViewExamBtnBClicked();

	if (m_NumLoadedWFODExams == 1 && m_NumLoadedCTODExams == 1)  m_OD = 1;
	else m_OD = 0;

	m_WhichTaskBtn = 1;

	m_EnableWavetouchButtons = TRUE;
	m_EnableEditExamButton = TRUE;
	m_EnableExportDataButton = TRUE;

	ShowCommonSummaryLevelButtons();
	ShowTaskButtons(1);

	m_FirstTask = 0;

	int DefSubTask = ::NewSettings.m_Tasks[1].m_DefNum;
	OnTaskSubBtnClicked(DefSubTask);

	m_TransType = 0;//For DICOM
}

//***************************************************************************************

//Click task btn 3 Default is cataract elevation
void CMainWnd::OnViewExam2BtnClicked()
{
	CBusyCursor Cursor;
	if (!OnViewExamBtnAClicked(2)) return;

	OnViewExamBtnBClicked();

	if (m_NumLoadedWFODExams == 1 && m_NumLoadedCTODExams == 1)  m_OD = 1;
	else m_OD = 0;

	m_WhichTaskBtn = 2;

	m_EnableWavetouchButtons = TRUE;
	m_EnableEditExamButton = TRUE;
	m_EnableExportDataButton = TRUE;

	ShowCommonSummaryLevelButtons();
	ShowTaskButtons(2);

	m_FirstTask = 0;

	int DefSubTask = ::NewSettings.m_Tasks[2].m_DefNum;
	OnTaskSubBtnClicked(DefSubTask);

	m_TransType = 0;//For DICOM
}

//***************************************************************************************

//Click task btn 4 Default is troic planner
void CMainWnd::OnViewExam3BtnClicked()
{
	CBusyCursor Cursor;
	if (!OnViewExamBtnAClicked(3)) return;

	OnViewExamBtnBClicked();

	if (m_NumLoadedWFODExams == 1 && m_NumLoadedCTODExams == 1)  m_OD = 1;
	else m_OD = 0;

	m_WhichTaskBtn = 3;

	m_EnableWavetouchButtons = TRUE;
	m_EnableEditExamButton = TRUE;
	m_EnableExportDataButton = TRUE;

	ShowCommonSummaryLevelButtons();
	ShowTaskButtons(3);

	m_FirstTask = 0;

	int DefSubTask = ::NewSettings.m_Tasks[3].m_DefNum;
	OnTaskSubBtnClicked(DefSubTask);

	m_TransType = 0;//For DICOM
}

//***************************************************************************************

//Click task btn 5 Default is accommdation analysis
void CMainWnd::OnViewExam4BtnClicked()
{
	CBusyCursor Cursor;
	if (!OnViewExamBtnAClicked(4)) return;

	OnViewExamBtnBClicked();

	if (m_NumLoadedWFODExams == 1 && m_NumLoadedCTODExams == 1)  m_OD = 1;
	else m_OD = 0;

	m_WhichTaskBtn = 4;

	m_EnableWavetouchButtons = TRUE;
	m_EnableEditExamButton = TRUE;
	m_EnableExportDataButton = TRUE;

	ShowCommonSummaryLevelButtons();
	ShowTaskButtons(4);

	m_FirstTask = 0;

	int DefSubTask = ::NewSettings.m_Tasks[4].m_DefNum;
	OnTaskSubBtnClicked(DefSubTask);

	m_TransType = 0;//For DICOM
}

//***************************************************************************************

void CMainWnd::OnEditExamButtonClicked()
{
	m_ExamImgInfo.ShowWindow(SW_HIDE);
	m_NoteImgInfo.ShowWindow(SW_HIDE);

	if (m_Level == LEVEL_PATIENT)
	{
		::TempSettings.Temp_Deleted = 0;

		CreateSelectedIndicesLists();

		if (m_NumSelectedWFExams + m_NumSelectedCTExams < 1)
		{
			::Error("No exam is selected.");
			return;
		}

		if (m_NumSelectedWFExams + m_NumSelectedCTExams > 1)
		{
			//averaged wf exam save
			if (m_NumSelectedCTExams == 0)
			{
				if (!LoadSelectedExams())
				{
					return;
				}

				BOOL OD = TRUE;
				BOOL OS = TRUE;
				//is all selected exams belong to OD or OS?
				for (int i = 0; i < m_NumSelectedWFExams; i++)
				{
					if (m_pWFExams[i]->m_Header.IsOD())
					{
						if (OD) OD = TRUE;
						OS = FALSE;
					}
					else
					{
						if (OS) OS = TRUE;
						OD = FALSE;
					}
				}

				if (OD || OS)
				{
					CExamAveDlg* pDlg = new CExamAveDlg(this, m_pWFExams, m_NumSelectedWFExams);

					if (pDlg->DoModal() == IDOK)
					{
						m_FellowIDCaled = FALSE;
						UpdateExamsLists();
					}

					delete pDlg;
					return;
				}
			}

			::Error("Please select only one exam, or select mutiple OD or OS WF exams.");//521
			return;
		}

		if (!LoadSelectedExams())
		{
			return;
		}

		CExam* pExams[2] = { m_pWFExams[0], m_pCTExams[0] };

		for (int i = 0; i < 2; i++)
		{
			if (pExams[i])
			{
				int Eye = pExams[i]->m_Header.m_Eye;

				CExamDlg* pDlg = new CExamDlg(this, pExams[i]);

				if (pDlg->DoModal() == IDOK)
				{
					::DB.SaveExam(pExams[i]);
					if (Eye != pExams[i]->m_Header.m_Eye)
					{
						DeleteSelectedIndicesLists();
					}

					m_WFODList.m_FelRow = -1;
					m_WFOSList.m_FelRow = -1;
					m_CTODList.m_FelRow = -1;
					m_CTOSList.m_FelRow = -1;

					UpdateExamsLists();
				}

				if (::TempSettings.Temp_Deleted) UpdateExamsLists();

				delete pDlg;
			}
		}

		DeleteLoadedExams();
	}

	else if (m_Level == LEVEL_SUMMARY)
	{
		CExam* pExams[2] = { m_pWFExams[0], m_pCTExams[0] };

		for (int i = 0; i < 2; i++)
		{
			if (pExams[i])
			{
				CExamDlg* pDlg = new CExamDlg(this, pExams[i]);
				if (pDlg->DoModal() == IDOK)
				{
					::DB.SaveExam(pExams[i]);
				}
				delete pDlg;
			}
		}

		m_pSumWnd->Repaint();
	}
}

//***************************************************************************************

void CMainWnd::OnDeleteExamButtonClicked()
{
	m_ExamImgInfo.ShowWindow(SW_HIDE);
	m_NoteImgInfo.ShowWindow(SW_HIDE);

	CreateSelectedIndicesLists();

	if (m_NumSelectedWFExams + m_NumSelectedCTExams < 1)
	{
		::Error("No exam is selected.");
		return;
	}

	if (!::Question("Delete selected exams?", 1, 1)) return;
	if (!::Question("You are about to remove selected exams\nfrom the database. Are you sure?", 0, 1)) return;

	real_t scanSize = 0;
	for (int i = 0; i < m_NumSelectedWFODExams; i++)
	{
		int  NumImages = 1;
		GUID FellowExamID = ::DB.LoadFellowID(m_WFODID[i], scanSize, NumImages, EXAM_TYPE_WF);

		if (FellowExamID != GUID_NULL)
		{
			for (int e = 0; e < ::DB.m_ExamHeaders.GetSize(); e++)
			{
				CExamHeader* pExamHeader = &::DB.m_ExamHeaders[e];

				if (pExamHeader->m_Type == EXAM_TYPE_CT && pExamHeader->IsOD())
				{
					GUID WF_FellowExamID = ::DB.LoadFellowID(pExamHeader->m_ExamID, scanSize, NumImages, EXAM_TYPE_CT);

					if (WF_FellowExamID == m_WFODID[i])
					{
						CCTExam* pCTExam = new CCTExam;

						::DB.LoadCTExam(pExamHeader->m_ExamID, pCTExam);
						pCTExam->m_Header.m_FellowExamID = GUID_NULL;
						::DB.SaveCTExam(pCTExam);
						break;
					}
				}
			}
		}

		::DB.DeleteExam(m_WFODID[i]);

		//::DB.DelTxTExamHeader(m_Patient.m_PatientID, m_WFODID[i]);//Big Database Research since 03062019

		//average exam deletation
		CString ExamId, LoadFileName;
		GUIDToStr(m_WFODID[i], ExamId);
		LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

		if (::PathFileExists(LoadFileName))
		{
			DeleteFile(LoadFileName);
		}
		// average exam deletation Done
	}

	for (int i = 0; i < m_NumSelectedWFOSExams; i++)
	{
		int  NumImages = 1;
		GUID FellowExamID = ::DB.LoadFellowID(m_WFOSID[i], scanSize, NumImages, EXAM_TYPE_WF);

		if (FellowExamID != GUID_NULL)
		{
			for (int e = 0; e < ::DB.m_ExamHeaders.GetSize(); e++)
			{
				CExamHeader* pExamHeader = &::DB.m_ExamHeaders[e];

				if (pExamHeader->m_Type == EXAM_TYPE_CT && pExamHeader->IsOS())
				{
					GUID WF_FellowExamID = ::DB.LoadFellowID(pExamHeader->m_ExamID, scanSize, NumImages, EXAM_TYPE_CT);

					if (WF_FellowExamID == m_WFOSID[i])
					{
						CCTExam* pCTExam = new CCTExam;

						::DB.LoadCTExam(pExamHeader->m_ExamID, pCTExam);
						pCTExam->m_Header.m_FellowExamID = GUID_NULL;
						::DB.SaveCTExam(pCTExam);
						break;
					}
				}
			}
		}

		::DB.DeleteExam(m_WFOSID[i]);
		//::DB.DelTxTExamHeader(m_Patient.m_PatientID, m_WFOSID[i]);//Big Database Research since 03062019

		//average exam deletation
		CString ExamId, LoadFileName;
		GUIDToStr(m_WFOSID[i], ExamId);
		LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

		if (::PathFileExists(LoadFileName))
		{
			DeleteFile(LoadFileName);
		}
		//average exam deletation Done
	}

	for (int i = 0; i < m_NumSelectedCTODExams; i++)
	{
		int  NumImages = 1;
		GUID FellowExamID = ::DB.LoadFellowID(m_CTODID[i], scanSize, NumImages, EXAM_TYPE_WF);

		if (FellowExamID != GUID_NULL)
		{
			for (int e = 0; e < ::DB.m_ExamHeaders.GetSize(); e++)
			{
				CExamHeader* pExamHeader = &::DB.m_ExamHeaders[e];

				if (pExamHeader->m_Type == EXAM_TYPE_WF && pExamHeader->IsOD())
				{
					CWFExam* pWFExam = new CWFExam;
					::DB.LoadWFExam(pExamHeader->m_ExamID, pWFExam);

					if (pWFExam->m_Header.m_FellowExamID == m_CTODID[i])
					{
						pWFExam->m_Header.m_FellowExamID = GUID_NULL;
						::DB.SaveWFExam(pWFExam);
						break;
					}
				}
			}
		}

		::DB.DeleteExam(m_CTODID[i]);

		//::DB.DelTxTExamHeader(m_Patient.m_PatientID, m_CTODID[i]);//Big Database Research since 03062019
	}

	for (int i = 0; i < m_NumSelectedCTOSExams; i++)
	{
		int  NumImages = 1;
		GUID FellowExamID = ::DB.LoadFellowID(m_CTOSID[i], scanSize, NumImages, EXAM_TYPE_WF);

		if (FellowExamID != GUID_NULL)
		{
			for (int e = 0; e < ::DB.m_ExamHeaders.GetSize(); e++)
			{
				CExamHeader* pExamHeader = &::DB.m_ExamHeaders[e];

				if (pExamHeader->m_Type == EXAM_TYPE_WF && pExamHeader->IsOS())
				{
					CWFExam* pWFExam = new CWFExam;
					::DB.LoadWFExam(pExamHeader->m_ExamID, pWFExam);

					if (pWFExam->m_Header.m_FellowExamID == m_CTOSID[i])
					{
						pWFExam->m_Header.m_FellowExamID = GUID_NULL;
						::DB.SaveWFExam(pWFExam);
						break;
					}
				}
			}
		}

		::DB.DeleteExam(m_CTOSID[i]);
		//::DB.DelTxTExamHeader(m_Patient.m_PatientID, m_CTOSID[i]);//Big Database Research since 03062019
	}

	DeleteSelectedIndicesLists();

	m_FellowIDCaled = FALSE;

	UpdateExamsLists();
}

//***************************************************************************************

void CMainWnd::OnTopconButtonClicked()
{
	CreateSelectedIndicesLists();

	if (m_NumSelectedWFExams + m_NumSelectedCTExams < 1)
	{
		::Error("No exam is selected.");
		return;
	}
	if (m_NumSelectedWFODExams > 1 || m_NumSelectedWFOSExams > 1 || m_NumSelectedCTODExams > 1 || m_NumSelectedCTOSExams > 1)
	{
		::Error("Invalid exam selection.");
		return;
	}

	if (!LoadSelectedExams())
	{
		return;
	}

	CBusyCursor Cursor;

	CWFExam* pWFODExam = NULL;
	CWFExam* pWFOSExam = NULL;
	CCTExam* pCTODExam = NULL;
	CCTExam* pCTOSExam = NULL;

	if (m_NumLoadedWFODExams == 1)
	{
		pWFODExam = m_pWFExams[0]->m_Header.IsOD() ? m_pWFExams[0] : m_pWFExams[1];
		pWFODExam->Process();

		//for average exam
		CString ExamId, LoadFileName;
		GUIDToStr(pWFODExam->m_Header.m_ExamID, ExamId);
		LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

		if (::PathFileExists(LoadFileName))
		{
			CAveExamInfo AveExamInfo;

			AveExamInfo.LoadFromFile(LoadFileName);
			for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
			{
				pWFODExam->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
			}
		}
		//for average exam Done
	}

	if (m_NumLoadedWFOSExams == 1)
	{
		pWFOSExam = m_pWFExams[0]->m_Header.IsOS() ? m_pWFExams[0] : m_pWFExams[1];
		pWFOSExam->Process();

		//for average exam
		CString ExamId, LoadFileName;
		GUIDToStr(pWFOSExam->m_Header.m_ExamID, ExamId);
		LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

		if (::PathFileExists(LoadFileName))
		{
			CAveExamInfo AveExamInfo;

			AveExamInfo.LoadFromFile(LoadFileName);
			for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
			{
				pWFOSExam->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
			}
		}
		//for average exam Done
	}

	if (m_NumLoadedCTODExams == 1)
	{
		pCTODExam = m_pCTExams[0]->m_Header.IsOD() ? m_pCTExams[0] : m_pCTExams[1];
		pCTODExam->Process();
	}

	if (m_NumLoadedCTOSExams == 1)
	{
		pCTOSExam = m_pCTExams[0]->m_Header.IsOS() ? m_pCTExams[0] : m_pCTExams[1];
		pCTOSExam->Process();
	}

	::ExportTopconCV5000(pWFODExam, pCTODExam, pWFOSExam, pCTOSExam);

	DeleteLoadedExams();
}

//***************************************************************************************

void CMainWnd::OnCtrlL()
{
	if (m_isViewer || m_isDemo)	return;

	if (m_Level == LEVEL_PATIENT || m_Level == LEVEL_SUMMARY)
	{
		OnNewWFCTExamButtonClicked();
		//OnNewCTExamButtonClicked();
		//OnNewWFExamButtonClicked();
	}
}

//***************************************************************************************

void CMainWnd::OnCtrlM()
{
	if (m_isViewer || m_isDemo)	return;
	if (m_Level == LEVEL_PATIENT || m_Level == LEVEL_SUMMARY)
	{
		OnNewCTExamButtonClicked();
	}
}

//***************************************************************************************

void CMainWnd::OnCtrlN()
{
	if (m_isViewer || m_isDemo)	return;
	if (m_Level == LEVEL_PATIENT || m_Level == LEVEL_SUMMARY)
	{
		OnNewWFExamButtonClicked();
	}
}

//***************************************************************************************

void CMainWnd::OnExamsListDblClicked(NMHDR* pNotifyStruct, LRESULT* pRes)
{
	m_ExamImgInfo.ShowWindow(FALSE);
	m_NoteImgInfo.ShowWindow(FALSE);
	m_ShowInfo = FALSE;

	if (pNotifyStruct->hwndFrom != m_WFODList.m_hWnd)
	{
		int n = m_WFODList.GetItemCount();

		for (int i = 0; i < n; i++)
			m_WFODList.SetItemState(i, 0, LVIS_SELECTED | LVIS_FOCUSED);

		m_WFODList.SetSelectionMark(-1);
	}

	if (pNotifyStruct->hwndFrom != m_WFOSList.m_hWnd)
	{
		int n = m_WFOSList.GetItemCount();

		for (int i = 0; i < n; i++)
			m_WFOSList.SetItemState(i, 0, LVIS_SELECTED | LVIS_FOCUSED);

		m_WFOSList.SetSelectionMark(-1);
	}

	if (pNotifyStruct->hwndFrom != m_CTODList.m_hWnd)
	{
		int n = m_CTODList.GetItemCount();

		for (int i = 0; i < n; i++)
			m_CTODList.SetItemState(i, 0, LVIS_SELECTED | LVIS_FOCUSED);

		m_CTODList.SetSelectionMark(-1);
	}

	if (pNotifyStruct->hwndFrom != m_CTOSList.m_hWnd)
	{
		int n = m_CTOSList.GetItemCount();

		for (int i = 0; i < n; i++)
			m_CTOSList.SetItemState(i, 0, LVIS_SELECTED | LVIS_FOCUSED);

		m_CTOSList.SetSelectionMark(-1);
	}

	m_WFODList.UpdateWindow();
	m_WFOSList.UpdateWindow();
	m_CTODList.UpdateWindow();
	m_CTOSList.UpdateWindow();
	UpdateWindow();

	OnViewExamButtonClicked();

	*pRes = 0;
}

//***************************************************************************************

void CMainWnd::OnPatientListHOVER(NMHDR* pNotifyStruct, LRESULT* pRes)
{
	CPoint point;
	GetCursorPos(&point);

	int Row = -1;

	if (pNotifyStruct->hwndFrom == m_PatientsList.m_hWnd)
	{
		int left = point.x - m_PatientsListRect.left;

		int PListNotePos = 0;

		for (int i = 0; i < 8; i++)
		{
			PListNotePos += m_PatientsList.GetColumnWidth(i);
		}
		int PListNoteWidth = m_PatientsList.GetColumnWidth(8);

		if (left < PListNotePos)
		{
			m_NoteImgInfo.ShowWindow(FALSE);
			m_ShowInfo = FALSE;
			return;
		}
		m_PatientsList.ScreenToClient(&point);
		Row = m_PatientsList.HitTest(point);

		if (Row == -1)
		{
			m_NoteImgInfo.ShowWindow(FALSE);
			m_ShowInfo = FALSE;
			return;
		}
		else
		{
			int p = (int)m_PatientsList.GetItemData(Row);
			m_Patient = ::DB.m_Patients[p];

			CString Note = m_Patient.m_Note;

			if (Note == "")
			{
				m_NoteImgInfo.ShowWindow(FALSE);
				m_ShowInfo = FALSE;
				return;
			}

			Note.Remove(_T('\r'));
			Note.Remove(_T('\n'));


			//Check how long the Note is
			RECT Rect;
			CMDC DC;
			DC.CreateCompatibleDC(NULL);
			DC.MeasureRect(Note, Rect, m_PatientCountFont);
			int CheckLen = Rect.right - Rect.left - 25;
			//Done

			if (CheckLen > PListNoteWidth)
			{
				if (m_NoteImgInfo.IsWindowVisible())
				{
					if (m_LastRow == Row) return;
				}

				m_NoteImgInfo.ShowWindow(FALSE);
				m_ShowInfo = FALSE;
				m_LastRow = Row;

				RECT rect1, rect2;
				::SetRect(&rect1, 5, 3, 635, 30);
				::SetRect(&rect2, 5, 33, 635, 60);

				m_NoteImgInfo.ClearMemDC();

				CString NoteSeg = "";
				int whichRect = 1;
				int InfoWinWidth = 600;
				int len = Note.GetLength();

				BOOL Chinese = FALSE;
				for (int i = 0; i < len; i++)
				{
					NoteSeg += Note.GetAt(i);

					//Check current Note segment length
					DC.MeasureRect(NoteSeg, Rect, m_PatientCountFont);
					int NoteSegLen = Rect.right - Rect.left;

					if (NoteSegLen >= InfoWinWidth)
					{
						int count = NoteSeg.Find(_T("Chinese*"), 0);//6.2.0 Chinese
						if (count == -1) //No chinese
						{
							if (IsLetter(Note.GetAt(i)) && IsLetter(Note.GetAt(i - 1)) && IsLetter(Note.GetAt(i + 1)))
							{
								NoteSeg += "-";
							}

							if (whichRect == 1)
								m_NoteImgInfo.m_MemDC.WriteText(NoteSeg, rect1, m_PatientCountFont, BLACK, 0, NOCOLOR);
							else if (whichRect == 2)
								m_NoteImgInfo.m_MemDC.WriteText(NoteSeg, rect2, m_PatientCountFont, BLACK, 0, NOCOLOR);

							NoteSeg = "";
							whichRect++;
						}
						else
						{
							Chinese = TRUE;		//6.2.0 Chinese
							whichRect = 1;
						}
					}
				}

				if (whichRect == 1)
				{
					m_NoteImgInfo.m_MemDC.WriteText(G_ReadChinese(NoteSeg), rect1, m_PatientCountFont, BLACK, 0, NOCOLOR);//6.2.0 Chinese
				}
				else if (whichRect == 2)
				{
					m_NoteImgInfo.m_MemDC.WriteText(G_ReadChinese(NoteSeg), rect2, m_PatientCountFont, BLACK, 0, NOCOLOR);//6.2.0 Chinese
				}

				int WinHeight = 60;
				int WinWidth = 640;
				if (whichRect == 1)
				{
					WinHeight = 30;

					if (Chinese)
					{
						CheckLen = (intRound((real_t)CheckLen / 3.0));//6.2.0 Chinese
					}

					WinWidth = CheckLen + 50;
				}

				int Infox, Infoy;
				Infox = point.x - WinWidth + m_PatientsListRect.left;

				if (Row < 15)
					Infoy = point.y + m_PatientsListRect.top + 20;
				else
					Infoy = point.y + m_PatientsListRect.top - WinHeight - 15;


				RECT Rect = { 0, 0, WinWidth, WinHeight };
				m_NoteImgInfo.m_MemDC.DrawRectangle(Rect, DARK_GRAY, NOCOLOR);
				m_NoteImgInfo.MoveWindow(Infox, Infoy, WinWidth, WinHeight, 1);

				m_ShowInfo = TRUE;
				SetTimer(TIMER2, 700, 0);
			}
			else
			{
				m_NoteImgInfo.ShowWindow(FALSE);
				m_ShowInfo = FALSE;
				return;
			}
		}
	}
}

//***************************************************************************************

BOOL CMainWnd::IsLetter(CString s)
{
	if (s == "a" || s == "b" || s == "c" || s == "d" || s == "e"
		|| s == "f" || s == "g" || s == "h" || s == "i" || s == "j"
		|| s == "k" || s == "l" || s == "m" || s == "n" || s == "o"
		|| s == "p" || s == "q" || s == "r" || s == "s" || s == "t"
		|| s == "u" || s == "v" || s == "w" || s == "x" || s == "y"
		|| s == "z")
		return TRUE;
	else return FALSE;
}

//***************************************************************************************
//Show the Tip of note and the eye image
void CMainWnd::OnExamsListHOVER(NMHDR* pNotifyStruct, LRESULT* pRes)
{
	CPoint point;
	GetCursorPos(&point);

	int EXAM_List_NColS[4];
	EXAM_List_NColS[0] = 0;//The Note col Start pos of WFOD LIST
	EXAM_List_NColS[1] = 0;//The Note col Start pos of WFOS LIST
	EXAM_List_NColS[2] = 0;//The Note col Start pos of CTOD LIST
	EXAM_List_NColS[3] = 0;//The Note col Start pos of CTOS LIST

	for (int i = 0; i < 10; i++)
	{
		EXAM_List_NColS[0] += m_WFODList.GetColumnWidth(i);
		EXAM_List_NColS[1] += m_WFOSList.GetColumnWidth(i);
		EXAM_List_NColS[2] += m_CTODList.GetColumnWidth(i);
		EXAM_List_NColS[3] += m_CTOSList.GetColumnWidth(i);
	}

	EXAM_List_NColS[0] += m_WFODList.GetColumnWidth(10);
	EXAM_List_NColS[1] += m_WFOSList.GetColumnWidth(10);

	int TipOrImage = 2;//0:Tip 1:Image; 2: Other
	int Row = -1;
	int WhichExam = -1;

	if (pNotifyStruct->hwndFrom == m_WFODList.m_hWnd)
	{
		int left = point.x - m_ExamsListRect[0].left;
		if (left <= m_WFODList.GetColumnWidth(0))
		{
			TipOrImage = 1;
			m_NoteImgInfo.ShowWindow(FALSE);
			m_ShowInfo = FALSE;
		}
		else if (left >= EXAM_List_NColS[0])
		{
			TipOrImage = 0;
			m_ExamImgInfo.ShowWindow(FALSE);
		}

		if (TipOrImage == 2)
		{
			m_ExamImgInfo.ShowWindow(FALSE);
			m_NoteImgInfo.ShowWindow(FALSE);
			m_ShowInfo = FALSE;
			return;
		}

		WhichExam = 1;
		m_WFODList.ScreenToClient(&point);
		Row = m_WFODList.HitTest(point);
	}
	else if (pNotifyStruct->hwndFrom == m_WFOSList.m_hWnd)
	{
		int left = point.x - m_ExamsListRect[2].left;
		if (left <= m_WFOSList.GetColumnWidth(0))
		{
			TipOrImage = 1;
			m_NoteImgInfo.ShowWindow(FALSE);
			m_ShowInfo = FALSE;
		}
		else if (left >= EXAM_List_NColS[1])
		{
			TipOrImage = 0;
			m_ExamImgInfo.ShowWindow(FALSE);
		}

		if (TipOrImage == 2)
		{
			m_ExamImgInfo.ShowWindow(FALSE);
			m_NoteImgInfo.ShowWindow(FALSE);
			m_ShowInfo = FALSE;
			return;
		}

		WhichExam = 2;
		m_WFOSList.ScreenToClient(&point);
		Row = m_WFOSList.HitTest(point);
	}
	else if (pNotifyStruct->hwndFrom == m_CTODList.m_hWnd)
	{
		int left = point.x - m_ExamsListRect[1].left;
		if (left <= m_CTODList.GetColumnWidth(0))
		{
			TipOrImage = 1;
			m_NoteImgInfo.ShowWindow(FALSE);
			m_ShowInfo = FALSE;
		}
		else if (left >= EXAM_List_NColS[2])
		{
			TipOrImage = 0;
			m_ExamImgInfo.ShowWindow(FALSE);
		}

		if (TipOrImage == 2)
		{
			m_ExamImgInfo.ShowWindow(FALSE);
			m_NoteImgInfo.ShowWindow(FALSE);
			m_ShowInfo = FALSE;
			return;
		}

		WhichExam = 3;
		m_CTODList.ScreenToClient(&point);
		Row = m_CTODList.HitTest(point);
	}
	else if (pNotifyStruct->hwndFrom == m_CTOSList.m_hWnd)
	{
		int left = point.x - m_ExamsListRect[3].left;
		if (left <= m_CTOSList.GetColumnWidth(0))
		{
			TipOrImage = 1;
			m_NoteImgInfo.ShowWindow(FALSE);
			m_ShowInfo = FALSE;
		}
		else if (left >= EXAM_List_NColS[3])
		{
			TipOrImage = 0;
			m_ExamImgInfo.ShowWindow(FALSE);
		}

		if (TipOrImage == 2)
		{
			m_ExamImgInfo.ShowWindow(FALSE);
			m_NoteImgInfo.ShowWindow(FALSE);
			m_ShowInfo = FALSE;
			return;
		}

		WhichExam = 4;
		m_CTOSList.ScreenToClient(&point);
		Row = m_CTOSList.HitTest(point);
	}

	if (Row == -1)
	{
		m_ExamImgInfo.ShowWindow(FALSE);
		m_NoteImgInfo.ShowWindow(FALSE);
		m_ShowInfo = FALSE;
		return;
	}

	int e = -1;
	if (WhichExam == 1)
		e = m_WFODList.GetItemData(Row);
	else if (WhichExam == 2)
		e = m_WFOSList.GetItemData(Row);
	else if (WhichExam == 3)
		e = m_CTODList.GetItemData(Row);
	else if (WhichExam == 4)
		e = m_CTOSList.GetItemData(Row);
	else
	{
		m_ExamImgInfo.ShowWindow(FALSE); m_NoteImgInfo.ShowWindow(FALSE);	return;
	}


	if (TipOrImage == 0)//Show Note Tip
	{
		int NoteWidth;

		if (WhichExam == 1)
			NoteWidth = m_WFODList.GetColumnWidth(11);
		else if (WhichExam == 2)
			NoteWidth = m_WFOSList.GetColumnWidth(11);
		else if (WhichExam == 3)
			NoteWidth = m_CTODList.GetColumnWidth(11);
		else if (WhichExam == 4)
			NoteWidth = m_CTOSList.GetColumnWidth(11);

		CString Note = ::DB.m_ExamHeaders[e].m_Note;

		if (Note == "")
		{
			m_NoteImgInfo.ShowWindow(FALSE);
			m_ShowInfo = FALSE;
			return;
		}

		Note.Remove(_T('\r'));
		Note.Remove(_T('\n'));

		//Check how long the Note is
		RECT Rect;
		CMDC DC;
		DC.CreateCompatibleDC(NULL);
		DC.MeasureRect(Note, Rect, m_PatientCountFont);
		int CheckLen = Rect.right - Rect.left - 25;
		//Done

		if (CheckLen > NoteWidth)
		{
			if (m_NoteImgInfo.IsWindowVisible())
			{
				if (m_LastRow == Row) return;
			}

			m_NoteImgInfo.ShowWindow(FALSE);
			m_ShowInfo = FALSE;
			m_LastRow = Row;

			m_NoteImgInfo.ClearMemDC();

			RECT rect1, rect2;
			::SetRect(&rect1, 5, 3, 635, 30);
			::SetRect(&rect2, 5, 33, 635, 60);

			CString NoteSeg = "";
			int whichRect = 1;
			int InfoWinWidth = 600;
			int len = Note.GetLength();

			for (int i = 0; i < len; i++)
			{
				NoteSeg += Note.GetAt(i);

				//Check current Note segment length
				DC.MeasureRect(NoteSeg, Rect, m_PatientCountFont);
				int NoteSegLen = Rect.right - Rect.left;

				if (NoteSegLen >= InfoWinWidth)
				{
					if (IsLetter(Note.GetAt(i)) && IsLetter(Note.GetAt(i - 1)) && IsLetter(Note.GetAt(i + 1)))
					{
						NoteSeg += "-";
					}

					if (whichRect == 1)
						m_NoteImgInfo.m_MemDC.WriteText(NoteSeg, rect1, m_PatientCountFont, BLACK, 0, NOCOLOR);
					else if (whichRect == 2)
						m_NoteImgInfo.m_MemDC.WriteText(NoteSeg, rect2, m_PatientCountFont, BLACK, 0, NOCOLOR);

					whichRect++;
					NoteSeg = "";
				}
				//

				if (i == len - 1)
				{
					if (whichRect == 1)
						m_NoteImgInfo.m_MemDC.WriteText(NoteSeg, rect1, m_PatientCountFont, BLACK, 0, NOCOLOR);
					else if (whichRect == 2)
						m_NoteImgInfo.m_MemDC.WriteText(NoteSeg, rect2, m_PatientCountFont, BLACK, 0, NOCOLOR);
				}
			}

			int WinHeight = 60;
			int WinWidth = 640;
			if (whichRect == 1)
			{
				WinHeight = 30;
				WinWidth = CheckLen + 50;
			}

			int Infox, Infoy;
			if (WhichExam == 1)
			{
				Infox = point.x + m_ExamsListRect[0].left;
				Infoy = point.y + 137;
			}
			else if (WhichExam == 2)
			{
				Infox = point.x - WinWidth + m_ExamsListRect[2].left;
				Infoy = point.y + 137;
			}
			else if (WhichExam == 3)
			{
				Infox = point.x + m_ExamsListRect[1].left;
				Infoy = point.y - WinHeight - 10 + m_ExamsListRect[1].top;//
			}
			else if (WhichExam == 4)
			{
				Infox = point.x - WinWidth + m_ExamsListRect[3].left;
				Infoy = point.y - WinHeight - 10 + m_ExamsListRect[3].top;
			}

			RECT Rect = { 0, 0, WinWidth, WinHeight };
			m_NoteImgInfo.m_MemDC.DrawRectangle(Rect, DARK_GRAY, NOCOLOR);
			m_NoteImgInfo.MoveWindow(Infox, Infoy, WinWidth, WinHeight, 1);

			m_ShowInfo = TRUE;
			SetTimer(TIMER2, 700, 0);
		}
		else
		{
			m_NoteImgInfo.ShowWindow(FALSE);
			m_ShowInfo = FALSE;
			return;
		}
	}
	else if (TipOrImage == 1)//Show Eye Image
	{
		if (m_ExamImgInfo.IsWindowVisible())
		{
			if (m_LastExam == WhichExam && m_LastRow == Row) return;
			else
			{
				m_LastExam = WhichExam;
				m_LastRow = Row;
			}
		}
		else
		{
			m_LastExam = WhichExam;
			m_LastRow = Row;
			m_ExamImgInfo.ShowWindow(FALSE);
		}
		//

		int Infox, Infoy;

		if (WhichExam == 1)
		{
			e = m_WFODList.GetItemData(Row);
			Infox = point.x + 20;
			Infoy = point.y + 20 + 117;
		}
		else if (WhichExam == 2)
		{
			e = m_WFOSList.GetItemData(Row);
			Infox = point.x + 20 + m_ExamsListRect[2].left;
			Infoy = point.y + 137;
		}
		else if (WhichExam == 3)
		{
			e = m_CTODList.GetItemData(Row);
			Infox = point.x + 20;
			Infoy = point.y - 254 + m_ExamsListRect[1].top;//
		}
		else if (WhichExam == 4)
		{
			e = m_CTOSList.GetItemData(Row);
			Infox = point.x + 20 + m_ExamsListRect[3].left;
			Infoy = point.y - 254 + m_ExamsListRect[3].top;
		}
		else { m_ExamImgInfo.ShowWindow(FALSE); return; }


		if (WhichExam == 1 || WhichExam == 2)
		{
			CWFExam* pWFExam = new CWFExam;
			if (!::DB.LoadWFExam(::DB.m_ExamHeaders[e].m_ExamID, pWFExam))
			{
				m_ExamImgInfo.ShowWindow(FALSE);
				delete pWFExam;
				return;
			}
			else
			{
				m_ExamImgInfo.MoveWindow(Infox, Infoy, 312, 234, 1);

				if (pWFExam->m_Image.m_RGBData.GetMem() == NULL)
				{
					m_ExamImgInfo.ShowWindow(FALSE);
					delete pWFExam;
					return;
				}

				real_t ratioW = real_t(pWFExam->m_Image.m_w) / real_t(m_ExamImgInfo.m_w);

				for (int y = 0; y < m_ExamImgInfo.m_h; y++)
				{
					for (int x = 0; x < m_ExamImgInfo.m_w; x++)
					{
						int InputI = intRound(x*ratioW);
						int InputJ = intRound(y*ratioW);

						if (InputI >= pWFExam->m_Image.m_w) InputI = pWFExam->m_Image.m_w - 1;
						if (InputJ >= pWFExam->m_Image.m_h) InputJ = pWFExam->m_Image.m_h - 1;


						int R = pWFExam->m_Image.GetRAt(InputI, InputJ);
						/*int G = pWFExam->m_Image.GetGAt(InputI,InputJ);
						int B = pWFExam->m_Image.GetBAt(InputI,InputJ);*/

						m_ExamImgInfo.m_MemDC.SetPixel(x, m_ExamImgInfo.m_h - y - 1, R, R, R);
					}
				}

				m_ExamImgInfo.ShowWindow(TRUE);
				m_ExamImgInfo.Invalidate(FALSE);
			}
			delete pWFExam;
		}
		else
		{
			CCTExam* pCTExam = new CCTExam;
			if (!::DB.LoadCTExam(::DB.m_ExamHeaders[e].m_ExamID, pCTExam))
			{
				m_ExamImgInfo.ShowWindow(FALSE);
				delete pCTExam;
				return;
			}
			else
			{
				m_ExamImgInfo.MoveWindow(Infox, Infoy, 312, 234, 1);

				if (pCTExam->m_Image.m_RGBData.GetMem() == NULL)
				{
					m_ExamImgInfo.ShowWindow(FALSE);
					delete pCTExam;
					return;
				}

				//m_ExamImgInfo.ClearMemDC();
				real_t ratioW = real_t(pCTExam->m_Image.m_w) / real_t(m_ExamImgInfo.m_w);

				for (int y = 0; y < m_ExamImgInfo.m_h; y++)
				{
					for (int x = 0; x < m_ExamImgInfo.m_w; x++)
					{
						int InputI = intRound(x*ratioW);
						int InputJ = intRound(y*ratioW);

						if (InputI >= pCTExam->m_Image.m_w) InputI = pCTExam->m_Image.m_w - 1;
						if (InputJ >= pCTExam->m_Image.m_h) InputJ = pCTExam->m_Image.m_h - 1;


						int R = pCTExam->m_Image.GetRAt(InputI, InputJ);
						/*int G = pCTExam->m_Image.GetGAt(InputI,InputJ);
						int B = pCTExam->m_Image.GetBAt(InputI,InputJ);*/

						m_ExamImgInfo.m_MemDC.SetPixel(x, m_ExamImgInfo.m_h - y - 1, R, R, R);
					}
				}

				m_ExamImgInfo.ShowWindow(TRUE);
				m_ExamImgInfo.Invalidate(FALSE);
			}
			delete pCTExam;
		}
	}
}

//***************************************************************************************

void CMainWnd::OnExamsListRClicked(NMHDR* pNotifyStruct, LRESULT* pRes)
{
	m_ExamImgInfo.ShowWindow(FALSE);
	m_NoteImgInfo.ShowWindow(FALSE);
	m_ShowInfo = FALSE;
	m_RightClick = 1;
	OnExamsListClicked(pNotifyStruct, pRes);
	m_RightClick = 0;
}

//***************************************************************************************
void CMainWnd::OnExamsListClicked(NMHDR* pNotifyStruct, LRESULT* pRes)
{
	m_ExamImgInfo.ShowWindow(FALSE);
	m_NoteImgInfo.ShowWindow(FALSE);
	m_ShowInfo = FALSE;

	int KeepTime = 200;// only the next click is greater than 400ms, allow the selection
	if (m_RightClick) KeepTime = 0;// if the keeptime is 400, sometimes it will not remove corresponding paired exam highlight

	int cur_Time = (int)clock();

	if (cur_Time <= m_OpenTime + KeepTime)
	{
		int WhichExam = 0;
		int Row = -1;

		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		//Check the exem types and which row is selected
		// 1: WFOD; 2: WFOS; 3:CTOD; 4:CTOS
		if (pNotifyStruct->hwndFrom == m_WFODList.m_hWnd)
		{
			WhichExam = 1;
			m_WFODList.ScreenToClient(&point);
			Row = m_WFODList.HitTest(point);
		}
		else if (pNotifyStruct->hwndFrom == m_WFOSList.m_hWnd)
		{
			WhichExam = 2;
			m_WFOSList.ScreenToClient(&point);
			Row = m_WFOSList.HitTest(point);
		}
		else if (pNotifyStruct->hwndFrom == m_CTODList.m_hWnd)
		{
			WhichExam = 3;
			m_CTODList.ScreenToClient(&point);
			Row = m_CTODList.HitTest(point);
		}
		else if (pNotifyStruct->hwndFrom == m_CTOSList.m_hWnd)
		{
			WhichExam = 4;
			m_CTOSList.ScreenToClient(&point);
			Row = m_CTOSList.HitTest(point);
		}

		if (WhichExam == 1)      m_WFODList.SetItemState(Row, 0, LVIS_SELECTED | LVIS_FOCUSED);
		else if (WhichExam == 2) m_WFOSList.SetItemState(Row, 0, LVIS_SELECTED | LVIS_FOCUSED);
		else if (WhichExam == 3) m_CTODList.SetItemState(Row, 0, LVIS_SELECTED | LVIS_FOCUSED);
		else if (WhichExam == 4) m_CTOSList.SetItemState(Row, 0, LVIS_SELECTED | LVIS_FOCUSED);
		return;
	}


	*pRes = 0;

	NMITEMACTIVATE* pStruct = (NMITEMACTIVATE*)pNotifyStruct;

	int WhichExam = 0;
	int Row = -1;

	DWORD dwPos = GetMessagePos();
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));

	//Check the exem types and which row is selected
	// 1: WFOD; 2: WFOS; 3:CTOD; 4:CTOS
	if (pNotifyStruct->hwndFrom == m_WFODList.m_hWnd)
	{
		WhichExam = 1;
		m_WFODList.ScreenToClient(&point);
		Row = m_WFODList.HitTest(point);
	}
	else if (pNotifyStruct->hwndFrom == m_WFOSList.m_hWnd)
	{
		WhichExam = 2;
		m_WFOSList.ScreenToClient(&point);
		Row = m_WFOSList.HitTest(point);
	}
	else if (pNotifyStruct->hwndFrom == m_CTODList.m_hWnd)
	{
		WhichExam = 3;
		m_CTODList.ScreenToClient(&point);
		Row = m_CTODList.HitTest(point);
	}
	else if (pNotifyStruct->hwndFrom == m_CTOSList.m_hWnd)
	{
		WhichExam = 4;
		m_CTOSList.ScreenToClient(&point);
		Row = m_CTOSList.HitTest(point);
	}


	AdjustSelectedItems(WhichExam, Row);
	Sleep(30);

	CreateSelectedIndicesLists();

	//sometimes mistake happen while click too fast
	BOOL mistake = 0;
	if (m_ExamsType == 0)
	{
		if ((m_NumSelectedWFExams + m_NumSelectedCTExams)>1)
		{
			int count = m_IndexSelectedExams.GetCount() / 2;
			mistake = 1;
			FixSelectionMistake();
			//::Error("Please select the exams slower.");
		}
	}
	else
	{
		if (m_ExamsType != 1 && m_ExamsType != 2)
		{
			if ((m_NumSelectedWFExams + m_NumSelectedCTExams)>2)
			{
				int count = m_IndexSelectedExams.GetCount() / 2;
				mistake = 1;
				FixSelectionMistake();
				//::Error("Please select the exams slower.");
			}
		}
	}

	if (mistake)
	{
		CreateSelectedIndicesLists();
	}

	if (m_NumSelectedWFOSExams == 0 && m_NumSelectedCTOSExams == 0)
	{
		m_WFOSList.m_FelRow = -1;
		m_CTOSList.m_FelRow = -1;
		m_WFOSList.Invalidate(false);
		m_CTOSList.Invalidate(false);
	}

	if (m_NumSelectedWFODExams == 0 && m_NumSelectedCTODExams == 0)
	{
		m_WFODList.m_FelRow = -1;
		m_CTODList.m_FelRow = -1;
		m_WFODList.Invalidate(false);
		m_CTODList.Invalidate(false);
	}

	CXPListCtrl* pList1;
	CXPListCtrl* pList2;
	int NumSelected1;
	int NumSelected2;
	int* pSelected1;
	int* pSelected2;

	if (pNotifyStruct->hwndFrom == m_WFODList.m_hWnd || pNotifyStruct->hwndFrom == m_CTODList.m_hWnd)
	{
		pList1 = &m_WFODList;
		pList2 = &m_CTODList;

		NumSelected1 = m_NumSelectedWFODExams;
		NumSelected2 = m_NumSelectedCTODExams;
		pSelected1 = &m_WFOD[0];
		pSelected2 = &m_CTOD[0];
	}
	else
	{
		pList1 = &m_WFOSList;
		pList2 = &m_CTOSList;
		NumSelected1 = m_NumSelectedWFOSExams;
		NumSelected2 = m_NumSelectedCTOSExams;
		pSelected1 = &m_WFOS[0];
		pSelected2 = &m_CTOS[0];
	}

	if (NumSelected1 == 0 && NumSelected2 == 1)
	{
		Swap(pList1, pList2);
		Swap(NumSelected1, NumSelected2);
		Swap(pSelected1, pSelected2);
	}

	int FelRow1 = pList1->m_FelRow;
	pList1->m_FelRow = -1;

	int FelRow2 = -1;

	if (!(NumSelected1 == 1 && NumSelected2 == 0))
	{
		FelRow2 = pList2->m_FelRow;
		pList2->m_FelRow = -1;
	}
	else
	{
		GUID FellowExamID;

		int i = pSelected1[0];
		int e = pList1->GetItemData(i);

		if (::DB.m_ExamHeaders[e].m_Type == EXAM_TYPE_WF)
		{
			CWFExam* pWFExam = new CWFExam;
			if (!::DB.LoadWFExam(::DB.m_ExamHeaders[e].m_ExamID, pWFExam))
			{
				delete pWFExam;
				::Error("Failed to load exam.");
				return;
			}
			FellowExamID = pWFExam->m_Header.m_FellowExamID;
			delete pWFExam;
		}
		else
		{
			CCTExam* pCTExam = new CCTExam;
			if (!::DB.LoadCTExam(::DB.m_ExamHeaders[e].m_ExamID, pCTExam))
			{
				delete pCTExam;
				::Error("Failed to load exam.");
				return;
			}
			FellowExamID = pCTExam->m_Header.m_FellowExamID;
			delete pCTExam;
		}

		if (FellowExamID == GUID_NULL)
		{
			FelRow2 = pList2->m_FelRow;
			pList2->m_FelRow = -1;
		}
		else
		{
			int n = pList2->GetItemCount();
			for (int i = 0; i < n; i++)
			{
				int e = pList2->GetItemData(i);
				if (::DB.m_ExamHeaders[e].m_ExamID == FellowExamID)
				{
					FelRow2 = pList2->m_FelRow;
					pList2->m_FelRow = i;
					break;
				}
			}
		}
	}

	if (FelRow1 != pList1->m_FelRow)
	{
		if (pList1->m_FelRow != -1)
		{
			int nSel = pList1->m_FelRow;
			int nItem = pList1->GetTopIndex();
			CRect rc;
			pList1->GetItemRect(nItem, rc, LVIR_BOUNDS);
			CSize sz(0, (nSel - nItem)*rc.Height());
			pList1->Scroll(sz);
		}

		pList1->Invalidate(FALSE);
	}

	if (FelRow2 != pList2->m_FelRow)
	{
		if (pList2->m_FelRow != -1)
		{
			int nSel = pList2->m_FelRow;
			int nItem = pList2->GetTopIndex();
			CRect rc;
			pList2->GetItemRect(nItem, rc, LVIR_BOUNDS);
			CSize sz(0, (nSel - nItem)*rc.Height());
			pList2->Scroll(sz);
		}

		pList2->Invalidate(FALSE);
	}

	m_OpenTime = (int)clock();
}

//***************************************************************************************

//sometimes mistake happen while click exams lists too fast
void CMainWnd::FixSelectionMistake()
{
	if (m_ExamsType != 0)
	{
		G_As = m_IndexSelectedExams.ElementAt(0);
		int firstType = atoi(G_As);
		G_As = m_IndexSelectedExams.ElementAt(2);
		int secondType = atoi(G_As);
		//a mistake is that the operator select muti WFOD or WFOS too fast
		if (m_NumSelectedWFODExams>1)
		{
			int Row = -1;
			if (firstType == 1)
			{
				G_As = m_IndexSelectedExams.ElementAt(1);
				Row = atoi(G_As);
			}
			else if (secondType == 1)
			{
				G_As = m_IndexSelectedExams.ElementAt(3);
				Row = atoi(G_As);
			}

			for (int i = 0; i<m_WFODList.GetItemCount(); i++)
			{
				if (i != Row)
				{
					if (m_WFODList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
						m_WFODList.SetItemState(i, 0, LVIS_SELECTED | LVIS_FOCUSED);
				}
			}
			return;
		}
		else if (m_NumSelectedWFOSExams>1)
		{
			int Row = -1;

			if (firstType == 2)
			{
				G_As = m_IndexSelectedExams.ElementAt(1);
				Row = atoi(G_As);
			}
			else if (secondType == 2)
			{
				G_As = m_IndexSelectedExams.ElementAt(3);
				Row = atoi(G_As);
			}

			for (int i = 0; i<m_WFOSList.GetItemCount(); i++)
			{
				if (i != Row)
				{
					if (m_WFOSList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
					{
						m_WFOSList.SetItemState(i, 0, LVIS_SELECTED | LVIS_FOCUSED);
					}
				}
			}
			return;
		}

		int delType1 = 0;
		int delType2 = 0;
		if ((firstType == 1 && secondType == 2) || (firstType == 2 && secondType == 1))
		{
			delType1 = 3;
			delType2 = 4;
		}
		else if ((firstType == 1 && secondType == 3) || (firstType == 3 && secondType == 1))
		{
			delType1 = 2;
			delType2 = 4;
		}
		else if ((firstType == 2 && secondType == 4) || (firstType == 4 && secondType == 2))
		{
			delType1 = 1;
			delType2 = 3;
		}
		else if ((firstType == 3 && secondType == 4) || (firstType == 4 && secondType == 3))
		{
			delType1 = 1;
			delType2 = 2;
		}

		if (delType1 == 1)
		{
			for (int i = 0; i<m_WFODList.GetItemCount(); i++)
				if (m_WFODList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
					m_WFODList.SetItemState(i, 0, LVIS_SELECTED | LVIS_FOCUSED);
		}
		if (delType1 == 2 || delType2 == 2)
		{
			for (int i = 0; i<m_WFOSList.GetItemCount(); i++)
				if (m_WFOSList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
					m_WFOSList.SetItemState(i, 0, LVIS_SELECTED | LVIS_FOCUSED);
		}
		if (delType1 == 3 || delType2 == 3)
		{
			for (int i = 0; i<m_CTODList.GetItemCount(); i++)
				if (m_CTODList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
					m_CTODList.SetItemState(i, 0, LVIS_SELECTED | LVIS_FOCUSED);
		}
		if (delType2 == 4)
		{
			for (int i = 0; i<m_CTOSList.GetItemCount(); i++)
				if (m_CTOSList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
					m_CTOSList.SetItemState(i, 0, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
	else
	{
		G_As = m_IndexSelectedExams.ElementAt(0);
		int firstType = atoi(G_As);
		int delType1 = 0;
		int delType2 = 0;
		int delType3 = 0;

		if (firstType == 1)
		{
			delType1 = 2;
			delType2 = 3;
			delType3 = 4;
		}
		else if (firstType == 2)
		{
			delType1 = 1;
			delType2 = 3;
			delType3 = 4;
		}
		else if (firstType == 3)
		{
			delType1 = 1;
			delType2 = 2;
			delType3 = 4;
		}
		else if (firstType == 4)
		{
			delType1 = 1;
			delType2 = 2;
			delType3 = 3;
		}

		if (delType1 == 1)
		{
			for (int i = 0; i<m_WFODList.GetItemCount(); i++)
				if (m_WFODList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
					m_WFODList.SetItemState(i, 0, LVIS_SELECTED | LVIS_FOCUSED);
		}

		if (delType1 == 2 || delType2 == 2)
		{
			for (int i = 0; i<m_WFOSList.GetItemCount(); i++)
				if (m_WFOSList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
					m_WFOSList.SetItemState(i, 0, LVIS_SELECTED | LVIS_FOCUSED);
		}

		if (delType2 == 3 || delType3 == 3)
		{
			for (int i = 0; i<m_CTODList.GetItemCount(); i++)
				if (m_CTODList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
					m_CTODList.SetItemState(i, 0, LVIS_SELECTED | LVIS_FOCUSED);
		}

		if (delType3 == 4)
		{
			for (int i = 0; i<m_CTOSList.GetItemCount(); i++)
				if (m_CTOSList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
					m_CTOSList.SetItemState(i, 0, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
}

//***************************************************************************************

//deleted some mis - slected exams from the Exam Lists
void CMainWnd::DelSomeSelExams(int StatrPos, int Num, CStringArray* Index)
{
	//delete the selected exams from the exam list
	for (int i = StatrPos; i<StatrPos + Num; i++)
	{
		G_As = Index->ElementAt(i * 2);
		int ExamType = atoi(G_As);
		G_As = Index->ElementAt(i * 2 + 1);
		int RowPos = atoi(G_As);

		if (ExamType == 1) 	m_WFODList.SetItemState(RowPos, 0, LVIS_SELECTED | LVIS_FOCUSED);
		else if (ExamType == 2) 	m_WFOSList.SetItemState(RowPos, 0, LVIS_SELECTED | LVIS_FOCUSED);
		else if (ExamType == 3) 	m_CTODList.SetItemState(RowPos, 0, LVIS_SELECTED | LVIS_FOCUSED);
		else if (ExamType == 4) 	m_CTOSList.SetItemState(RowPos, 0, LVIS_SELECTED | LVIS_FOCUSED);
	}

	//Remove the records in the m_IndexSelectedExams
	for (int i = StatrPos; i<StatrPos + Num; i++)
	{
		Index->RemoveAt(StatrPos * 2);
		Index->RemoveAt(StatrPos * 2);
	}
}

//***************************************************************************************

//arrange the selected exams by program logic
//1. WFOD or WFOS can select mutiple exams on their self filed;
//2. Can not select more than 3 Exams if they belong to different exam types,
//   for instance: 2 WFOD + 1 WFOS, 2 WFOD + 1CTOD
//3. Paired selection : WFOD and CTOS, or WFOS and CTOD can not be selected.

void CMainWnd::AdjustSelectedItems(int WhichExam, int Row)
{
	CString WhichExam_Str, Row_Str;
	WhichExam_Str.Format(_T("%d"), WhichExam);
	Row_Str.Format(_T("%d"), Row);

	int count = m_IndexSelectedExams.GetCount() / 2;

	//while shift+left click, the mutiple selection occurs, handle such situation first
	if (WhichExam == 1)
	{
		if (m_Shift == 1)
		{
			POSITION pos = m_WFODList.GetFirstSelectedItemPosition();
			while (pos)
			{
				int i = m_WFODList.GetNextSelectedItem(pos);
				if (i != Row) m_WFODList.SetItemState(i, 0, LVIS_SELECTED | LVIS_FOCUSED);
			}
		}
	}
	else if (WhichExam == 2)
	{
		if (m_Shift == 1)
		{
			POSITION pos = m_WFOSList.GetFirstSelectedItemPosition();
			while (pos)
			{
				int i = m_WFOSList.GetNextSelectedItem(pos);
				if (i != Row) m_WFOSList.SetItemState(i, 0, LVIS_SELECTED | LVIS_FOCUSED);
			}
		}
	}
	else if (WhichExam == 3)
	{
		POSITION pos = m_CTODList.GetFirstSelectedItemPosition();
		while (pos)
		{
			int i = m_CTODList.GetNextSelectedItem(pos);

			if (i != Row)
				m_CTODList.SetItemState(i, 0, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
	else if (WhichExam == 4)
	{
		POSITION pos = m_CTOSList.GetFirstSelectedItemPosition();
		while (pos)
		{
			int i = m_CTOSList.GetNextSelectedItem(pos);

			if (i != Row)
				m_CTOSList.SetItemState(i, 0, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
	//shift+left prcessing done

	//If user click on a selected exam, delete this exam from m_IndexSelectedExams,and change the m_ExamsType
	BOOL SameClick = 0;
	BOOL Del_Ok = 0;

	int* Del_Arr = new int[count];
	for (int i = 0; i < count; i++) Del_Arr[i] = 0;

	int n = 0;
	{
		for (int i = 0; i<count; i++)
		{
			G_As = m_IndexSelectedExams.ElementAt(i * 2);
			int ExamType = atoi(G_As);
			G_As = m_IndexSelectedExams.ElementAt(i * 2 + 1);
			int RowPos = atoi(G_As);

			if (WhichExam == ExamType)
			{
				if (Row == RowPos) SameClick = 1;
				if (WhichExam == 1)
				{
					if (!(m_WFODList.GetItemState(RowPos, LVIS_SELECTED) == LVIS_SELECTED))
					{
						Del_Ok = 1;
						Del_Arr[n] = i;
						n++;
					}
				}
				else if (WhichExam == 2)
				{
					if (!(m_WFOSList.GetItemState(RowPos, LVIS_SELECTED) == LVIS_SELECTED))
					{
						Del_Arr[n] = i;
						n++;
						Del_Ok = 1;
					}
				}
				else if (WhichExam == 3)
				{
					if (!(m_CTODList.GetItemState(RowPos, LVIS_SELECTED) == LVIS_SELECTED))
					{
						Del_Ok = 1;
						Del_Arr[n] = i;
						n++;
					}
				}
				else if (WhichExam == 4)
				{
					if (!(m_CTOSList.GetItemState(RowPos, LVIS_SELECTED) == LVIS_SELECTED))
					{
						Del_Ok = 1;
						Del_Arr[n] = i;
						n++;
					}
				}
			}
		}
	}

	if (Del_Ok)
	{
		for (int i = n - 1; i >= 0; i--)
		{
			int pos = Del_Arr[i];
			DelSomeSelExams(pos, 1, &m_IndexSelectedExams);
		}
	}
	delete[] Del_Arr;

	if (SameClick) return;

	count = m_IndexSelectedExams.GetCount() / 2;

	if (count == 0)
	{
		m_IndexSelectedExams.Add(WhichExam_Str);
		m_IndexSelectedExams.Add(Row_Str);
		m_ExamsType = 0;
	}
	else if (count == 1)
	{
		// WhichExam and ExamType---1: WFOD; 2: WFOS; 3:CTOD; 4:CTOS

		// m_ExamsType ---
		//0: Unsure; 1: Muti-WFOD; 2: Muti-WFOS;

		//3: one WF-OD first and then one WF-OS (12)
		//4: one WF-OS first and then one WF-OD (21)

		//5: one WF-OD First and then one CT-OD (13)
		//6: one CT-OD First and then one WF-OD (31)

		//7: one WF-OS First and then one CT-OS (24)
		//8: one CT-OS First and then one WF-OS (42)

		//9: one CT-OD First and then one CT-OS (34)
		//10:one CT-OS First and then one CT-OD (43)
		G_As = m_IndexSelectedExams.ElementAt(0);
		int ExamType = atoi(G_As);
		G_As = m_IndexSelectedExams.ElementAt(1);
		int RowPos = atoi(G_As);

		if (ExamType == 1)//WFOD
		{
			if (WhichExam == 1) m_ExamsType = 1; //11
			else if (WhichExam == 2) m_ExamsType = 3; //12
			else if (WhichExam == 3) m_ExamsType = 5; //13
			else if (WhichExam == 4) //DELETE THE first WFOD selection because they can not be seleted at the same time
			{
				DelSomeSelExams(0, 1, &m_IndexSelectedExams);
				m_ExamsType = 0;
				Invalidate(FALSE);
			}
		}
		else if (ExamType == 2)//WFOS
		{
			if (WhichExam == 1) m_ExamsType = 4; //21
			else if (WhichExam == 2) m_ExamsType = 2; //22
			else if (WhichExam == 3) //23
			{
				DelSomeSelExams(0, 1, &m_IndexSelectedExams);
				m_ExamsType = 0;
			}
			else if (WhichExam == 4) m_ExamsType = 7; //24
		}
		else if (ExamType == 3)//CTOD
		{
			if (WhichExam == 1) m_ExamsType = 6; //31
			else if (WhichExam == 2) //32
			{
				DelSomeSelExams(0, 1, &m_IndexSelectedExams);
				m_ExamsType = 0;
			}
			else if (WhichExam == 3) //33
			{
				DelSomeSelExams(0, 1, &m_IndexSelectedExams);
				m_ExamsType = 0;
			}
			else if (WhichExam == 4) m_ExamsType = 9; //34
		}
		else if (ExamType == 4) //CTOS
		{
			if (WhichExam == 1) //41
			{
				DelSomeSelExams(0, 1, &m_IndexSelectedExams);
				m_ExamsType = 0;
			}
			else if (WhichExam == 2) m_ExamsType = 8; //42
			else if (WhichExam == 3) m_ExamsType = 10; //43
			else if (WhichExam == 4)
			{
				DelSomeSelExams(0, 1, &m_IndexSelectedExams);
				m_ExamsType = 0;
			}
		}

		m_IndexSelectedExams.Add(WhichExam_Str);
		m_IndexSelectedExams.Add(Row_Str);
	}
	else //count > 1
	{
		if (m_ExamsType == 1) //1: Muti-WFOD;
		{
			if (WhichExam == 1)
			{
			}
			else if (WhichExam == 2)
			{
				DelSomeSelExams(0, count - 1, &m_IndexSelectedExams);
				m_ExamsType = 3; //12
			}
			else if (WhichExam == 3)
			{
				DelSomeSelExams(0, count - 1, &m_IndexSelectedExams);
				m_ExamsType = 5; //13
			}
			else if (WhichExam == 4)
			{
				DelSomeSelExams(0, count, &m_IndexSelectedExams);
				m_ExamsType = 0;
			}
		}


		else if (m_ExamsType == 2) //2: Muti-WFOS;
		{
			if (WhichExam == 1) {
				DelSomeSelExams(0, count - 1, &m_IndexSelectedExams);
				m_ExamsType = 4; //21
			}
			else if (WhichExam == 2) {}
			else if (WhichExam == 3)
			{
				DelSomeSelExams(0, count, &m_IndexSelectedExams);
				m_ExamsType = 0; //3
			}
			else if (WhichExam == 4)
			{
				DelSomeSelExams(0, count - 1, &m_IndexSelectedExams);
				m_ExamsType = 7; //24
			}
		}

		else if (m_ExamsType == 3) //3: one WF-OD first and then one WF-OS (12)
		{
			if (WhichExam == 1)
			{
				DelSomeSelExams(0, 1, &m_IndexSelectedExams);
				m_ExamsType = 4; //21
			}
			else if (WhichExam == 2)
			{
				DelSomeSelExams(0, 1, &m_IndexSelectedExams);
				m_ExamsType = 2; //22
			}
			else if (WhichExam == 3)
			{
				DelSomeSelExams(0, count, &m_IndexSelectedExams);
				m_ExamsType = 0; // 3
			}
			else if (WhichExam == 4)
			{
				DelSomeSelExams(0, count - 1, &m_IndexSelectedExams);
				m_ExamsType = 7; //24	5: one WF-OS First and then one CT-OS (24)
			}
		}

		else if (m_ExamsType == 4) //4: one WF-OS first and then one WF-OD (21)
		{
			if (WhichExam == 1)
			{
				DelSomeSelExams(0, 1, &m_IndexSelectedExams);
				m_ExamsType = 1; //1: Muti-WFOD; 2: Muti-WFOS;
			}
			else if (WhichExam == 2)
			{
				DelSomeSelExams(0, 1, &m_IndexSelectedExams);
				m_ExamsType = 3; //3: one WF-OD first and then one WF-OS (12)
			}
			else if (WhichExam == 3)
			{
				DelSomeSelExams(0, count - 1, &m_IndexSelectedExams);
				m_ExamsType = 5; // 	5: one WF-OD First and then one CT-OD (13)
			}
			else if (WhichExam == 4)
			{
				DelSomeSelExams(0, count, &m_IndexSelectedExams);
				m_ExamsType = 0; //
			}
		}

		else if (m_ExamsType == 5) //5:  one WF-OD First and then one CT-OD (13)
		{
			if (WhichExam == 1)
			{
				DelSomeSelExams(0, 1, &m_IndexSelectedExams);
				m_ExamsType = 6; //6:  one CT-OD First and then one WF-OD (31)
			}
			else if (WhichExam == 2)
			{
				DelSomeSelExams(0, count, &m_IndexSelectedExams);
				m_ExamsType = 0;
			}
			else if (WhichExam == 3)
			{
				DelSomeSelExams(1, 1, &m_IndexSelectedExams);
				m_ExamsType = 5;
			}
			else if (WhichExam == 4)
			{
				DelSomeSelExams(0, 1, &m_IndexSelectedExams);
				m_ExamsType = 9; //	//9: one CT-OD First and then one CT-OS (34)
			}
		}

		else if (m_ExamsType == 6) //6:  one CT-OD First and then one WF-OD (31)
		{
			if (WhichExam == 1)
			{
				DelSomeSelExams(0, 1, &m_IndexSelectedExams);
				m_ExamsType = 1; //1: Muti-WFOD;
			}
			else if (WhichExam == 2)
			{
				DelSomeSelExams(0, 1, &m_IndexSelectedExams);
				m_ExamsType = 3; //3: one WF-OD first and then one WF-OS (12)
			}
			else if (WhichExam == 3)
			{
				DelSomeSelExams(0, 1, &m_IndexSelectedExams);
				m_ExamsType = 5;  //5:  one WF-OD First and then one CT-OD (13)
			}
			else if (WhichExam == 4)
			{
				DelSomeSelExams(0, count, &m_IndexSelectedExams);
				m_ExamsType = 0; //
			}
		}

		else if (m_ExamsType == 7) //7: one WF-OS First and then one CT-OS (24)
		{
			if (WhichExam == 1)
			{
				DelSomeSelExams(0, count, &m_IndexSelectedExams);
				m_ExamsType = 0;
			}
			else if (WhichExam == 2)
			{
				DelSomeSelExams(0, 1, &m_IndexSelectedExams);
				m_ExamsType = 8;  //8: one CT-OS First and then one WF-OS (42)
			}
			else if (WhichExam == 3)
			{
				DelSomeSelExams(0, 1, &m_IndexSelectedExams);
				m_ExamsType = 10;  //10:one CT-OS First and then one CT-OD (43)
			}
			else if (WhichExam == 4)
			{
				DelSomeSelExams(1, 1, &m_IndexSelectedExams);
				m_ExamsType = 7; //7: one WF-OS First and then one CT-OS (24)
			}
		}

		else if (m_ExamsType == 8) //8: one CT-OS First and then one WF-OS (42)
		{
			if (WhichExam == 1)
			{
				DelSomeSelExams(0, 1, &m_IndexSelectedExams);
				m_ExamsType = 4;   //4: one WF-OS first and then one WF-OD (21)
			}
			else if (WhichExam == 2)
			{
				DelSomeSelExams(0, 1, &m_IndexSelectedExams);
				m_ExamsType = 2;  	//2: Muti-WFOS;
			}
			else if (WhichExam == 3)
			{
				DelSomeSelExams(0, count, &m_IndexSelectedExams);
				m_ExamsType = 0;
			}
			else if (WhichExam == 4)
			{
				DelSomeSelExams(0, 1, &m_IndexSelectedExams);
				m_ExamsType = 7; //
			}
		}

		//
		else if (m_ExamsType == 9) //9: one CT-OD First and then one CT-OS (34)
		{
			if (WhichExam == 1)
			{
				DelSomeSelExams(1, 1, &m_IndexSelectedExams);
				m_ExamsType = 6;   //6: one CT-OD First and then one WF-OD (31)
			}
			else if (WhichExam == 2)
			{
				DelSomeSelExams(0, 1, &m_IndexSelectedExams);
				m_ExamsType = 8;   //8: one CT-OS First and then one WF-OS (42)
			}
			else if (WhichExam == 3)
			{
				DelSomeSelExams(0, 1, &m_IndexSelectedExams);
				m_ExamsType = 10;  //10:one CT-OS First and then one CT-OD (43)
			}
			else if (WhichExam == 4)
			{
				DelSomeSelExams(1, 1, &m_IndexSelectedExams);
				m_ExamsType = 9; //
			}
		}

		//
		else if (m_ExamsType == 10) //10:one CT-OS First and then one CT-OD (43)
		{
			if (WhichExam == 1)
			{
				DelSomeSelExams(0, 1, &m_IndexSelectedExams);
				m_ExamsType = 6;   //6: one CT-OD First and then one WF-OD (31)
			}
			else if (WhichExam == 2)
			{
				DelSomeSelExams(1, 1, &m_IndexSelectedExams);
				m_ExamsType = 8;   //8: one CT-OS First and then one WF-OS (42)
			}
			else if (WhichExam == 3)
			{
				DelSomeSelExams(1, 1, &m_IndexSelectedExams);
				m_ExamsType = 10;  //10:one CT-OS First and then one CT-OD (43)
			}
			else if (WhichExam == 4)
			{
				DelSomeSelExams(0, 1, &m_IndexSelectedExams);
				m_ExamsType = 9; //
			}
		}

		m_IndexSelectedExams.Add(WhichExam_Str);
		m_IndexSelectedExams.Add(Row_Str);
		//
	}
}

//***************************************************************************************

void CMainWnd::CreateSelectedIndicesLists()
{
	DeleteSelectedIndicesLists();

	POSITION pos = m_WFODList.GetFirstSelectedItemPosition();
	while (pos && m_NumSelectedWFODExams < NUM_EXAMS_MAX)
	{
		int i = m_WFODList.GetNextSelectedItem(pos);
		int e = m_WFODList.GetItemData(i);
		m_WFOD[m_NumSelectedWFODExams] = i;
		m_WFODID[m_NumSelectedWFODExams] = ::DB.m_ExamHeaders[e].m_ExamID;
		m_NumSelectedWFODExams++;
	}

	pos = m_WFOSList.GetFirstSelectedItemPosition();
	while (pos && m_NumSelectedWFOSExams < NUM_EXAMS_MAX)
	{
		int i = m_WFOSList.GetNextSelectedItem(pos);
		int e = m_WFOSList.GetItemData(i);
		m_WFOS[m_NumSelectedWFOSExams] = i;
		m_WFOSID[m_NumSelectedWFOSExams] = ::DB.m_ExamHeaders[e].m_ExamID;
		m_NumSelectedWFOSExams++;
	}

	pos = m_CTODList.GetFirstSelectedItemPosition();
	while (pos && m_NumSelectedCTODExams < NUM_EXAMS_MAX)
	{
		int i = m_CTODList.GetNextSelectedItem(pos);
		int e = m_CTODList.GetItemData(i);
		m_CTOD[m_NumSelectedCTODExams] = i;
		m_CTODID[m_NumSelectedCTODExams] = ::DB.m_ExamHeaders[e].m_ExamID;
		m_NumSelectedCTODExams++;
	}

	pos = m_CTOSList.GetFirstSelectedItemPosition();
	while (pos && m_NumSelectedCTOSExams < NUM_EXAMS_MAX)
	{
		int i = m_CTOSList.GetNextSelectedItem(pos);
		int e = m_CTOSList.GetItemData(i);
		m_CTOS[m_NumSelectedCTOSExams] = i;
		m_CTOSID[m_NumSelectedCTOSExams] = ::DB.m_ExamHeaders[e].m_ExamID;
		m_NumSelectedCTOSExams++;
	}

	m_NumSelectedWFExams = m_NumSelectedWFODExams + m_NumSelectedWFOSExams;
	m_NumSelectedCTExams = m_NumSelectedCTODExams + m_NumSelectedCTOSExams;
}

//***************************************************************************************

void CMainWnd::DeleteSelectedIndicesLists()
{
	m_NumSelectedWFODExams = 0;
	m_NumSelectedWFOSExams = 0;
	m_NumSelectedCTODExams = 0;
	m_NumSelectedCTOSExams = 0;
	m_NumSelectedWFExams = 0;
	m_NumSelectedCTExams = 0;
}

//***************************************************************************************

void CMainWnd::ShowPatientLevelControls()
{
  BOOL IsHardwareOK = ::HW.IsConnected() && ::HW.HasCalibrationLoaded();

  BOOL IsDBReadOnly = ::DB.IsReadOnly();

  if (IsHardwareOK) {
    if (!::HW.ISVideoCameraOn) {
      HW.TurnVideoCameraOn();
      ::HW.ISVideoCameraOn = true;
    }
  }
  else
    ::HW.ISVideoCameraOn = false;

  if (m_isSolo) {
    m_WFODList.SetWindowPos(NULL, m_ExamsListRect[4].left, m_ExamsListRect[4].top, m_ExamsListRect[4].right - m_ExamsListRect[4].left,
                            m_ExamsListRect[4].bottom - m_ExamsListRect[4].top, SWP_NOMOVE | SWP_NOZORDER);
    m_WFOSList.SetWindowPos(NULL, m_ExamsListRect[5].left, m_ExamsListRect[5].top, m_ExamsListRect[5].right - m_ExamsListRect[5].left,
                            m_ExamsListRect[5].bottom - m_ExamsListRect[5].top, SWP_NOMOVE | SWP_NOZORDER);
  }
  else {
    m_WFODList.SetWindowPos(NULL, m_ExamsListRect[0].left, m_ExamsListRect[0].top, m_ExamsListRect[0].right - m_ExamsListRect[0].left,
                            m_ExamsListRect[0].bottom - m_ExamsListRect[0].top, SWP_NOMOVE | SWP_NOZORDER);
    m_WFOSList.SetWindowPos(NULL, m_ExamsListRect[2].left, m_ExamsListRect[2].top, m_ExamsListRect[2].right - m_ExamsListRect[2].left,
                            m_ExamsListRect[2].bottom - m_ExamsListRect[2].top, SWP_NOMOVE | SWP_NOZORDER);
  }

  m_WFODList.ShowWindow(SW_SHOW);
  m_WFOSList.ShowWindow(SW_SHOW);

  if (!m_isSolo) {
    m_CTODList.ShowWindow(SW_SHOW);
    m_CTOSList.ShowWindow(SW_SHOW);
  }

  m_NewWFExamButton.SetEnabled(!IsDBReadOnly && IsHardwareOK);
  m_NewWFExamButton.ShowWindow(SW_SHOW);

  if (::NewSettings.m_Super_Exam) {
    m_NewWFExamButton.SetImage(m_q == 2 ? IDR_NEW_SUPERWF_EXAM_3 : m_q == 1 ? IDR_NEW_SUPERWF_EXAM_2 : IDR_NEW_SUPERWF_EXAM_1);
    m_NewWFCTExamButton.SetImage(m_q == 2 ? IDR_NEW_SUPERIN_EXAM_3 : m_q == 1 ? IDR_NEW_SUPERIN_EXAM_2 : IDR_NEW_SUPERIN_EXAM_1);
  }
  else {
    m_NewWFExamButton.SetImage(m_q == 2 ? IDR_NEW_WF_EXAM_3 : m_q == 1 ? IDR_NEW_WF_EXAM_2 : IDR_NEW_WF_EXAM_1);
    m_NewWFCTExamButton.SetImage(m_q == 2 ? IDR_NEW_IN_EXAM_3 : m_q == 1 ? IDR_NEW_IN_EXAM_2 : IDR_NEW_IN_EXAM_1);
  }

  if (!m_isSolo) {
    m_NewCTExamButton.SetEnabled(!IsDBReadOnly && IsHardwareOK);
    m_NewCTExamButton.ShowWindow(SW_SHOW);
    m_NewWFCTExamButton.SetEnabled(!IsDBReadOnly && IsHardwareOK);
    m_NewWFCTExamButton.ShowWindow(SW_SHOW);

    // Change the NewWFExamButton position back from solo version if it was happened
    LPCRECT prct;
    prct = &m_BR[0][1];

    m_NewWFExamButton.MoveWindow(prct, 1);
    m_NewWFExamButton.Repaint();
  }
  // Change the NewWFExamButton position of solo
  else {
    LPCRECT prct;
    prct = &m_BR[0][0];

    m_NewWFExamButton.MoveWindow(prct, 1);
    m_NewWFExamButton.Repaint();
  }

  if (m_isDemo) {
    m_NewWFExamButton.SetEnabled(FALSE);
    m_NewCTExamButton.Repaint();
    m_NewCTExamButton.SetEnabled(FALSE);
    m_NewCTExamButton.Repaint();
    m_NewWFCTExamButton.SetEnabled(FALSE);
    m_NewWFCTExamButton.Repaint();
  }
  if (m_isViewer) {
    m_NewWFExamButton.ShowWindow(FALSE);
    m_NewCTExamButton.ShowWindow(FALSE);
    m_NewWFCTExamButton.ShowWindow(FALSE);
  }

  m_ViewExamButton.ShowWindow(SW_SHOW);

  if (m_isComboDicom)
    m_DicomExportButton.ShowWindow(SW_SHOW); // 0129 Dicom Export

  if (!m_isSolo) {
    if (::NewSettings.m_ShowTask) {
      CString Task_Btn_Name;

      int StartPos = 4;
      for (int i = 0; i < 5; i++) {
        Task_Btn_Name = ::NewSettings.m_Tasks[i].ReadBtnName(); // 6.2.0 For Chinese

        if (m_CustExamCreated[i]) {
          m_ViewCustExamButton[i].ShowWindow(SW_SHOW);
          m_ViewCustExamButton[i].SetTip("View " + Task_Btn_Name);

          if (Task_Btn_Name == "Accomm and DOF Analysis" && !m_isCombo)
            Task_Btn_Name = "Accomm. Analysis";

          if (Task_Btn_Name == "Accommdation Analysis" && m_isCombo)
            Task_Btn_Name = "Accomm and DOF Analysis";

          m_ViewCustExamButton[i].SetText(Task_Btn_Name);

          if (!::NewSettings.m_Tasks[i].m_Enable) {
            m_ViewCustExamButton[i].ShowWindow(FALSE); // 531
          }
          else {
            m_ViewCustExamButton[i].ShowWindow(TRUE);

            m_ViewCustExamButton[i].MoveWindow(&m_BR[0][StartPos], 1);
            StartPos++;
          }
          m_ViewCustExamButton[i].Repaint();
        }
      }

      // 0129 Dicom Export button
      if (m_isComboDicom) {
        m_DicomExportButton.MoveWindow(&m_BR[0][StartPos], 1);
        StartPos++;
      }
      // 0129 Dicom Export button

      m_EditExamButton.MoveWindow(&m_BR[0][StartPos], 1);
      StartPos++;
      m_DeleteExamButton.MoveWindow(&m_BR[0][StartPos], 1);
      if (::Settings.m_ShowSendARKRButton) {
        StartPos++;
        m_TopconButton.MoveWindow(&m_BR[0][StartPos], 1);
      }
    }
    else {
      // 0129 Dicom Export button
      int EditBtnMovPos = m_isComboDicom ? 5 : 4;

      if (m_isComboDicom) {
        m_DicomExportButton.MoveWindow(&m_BR[0][4], 1);
      }
      // 0129 Dicom Export button

      m_EditExamButton.MoveWindow(&m_BR[0][EditBtnMovPos], 1);
      m_DeleteExamButton.MoveWindow(&m_BR[0][EditBtnMovPos + 1], 1);
      if (::Settings.m_ShowSendARKRButton) {
        m_TopconButton.MoveWindow(&m_BR[0][EditBtnMovPos + 2], 1);
      }
      for (int i = 0; i < 5; i++) {
        if (m_CustExamCreated[i])
          m_ViewCustExamButton[i].ShowWindow(FALSE);
      }
    }
  }

  RECT R;
  if (m_isSolo) {
    R = m_BR[0][4];
    m_EditExamButton.SetWindowPos(NULL, R.left, R.top, R.right - R.left, R.bottom - R.top, SWP_NOZORDER);

    R = m_BR[0][5];
    m_DeleteExamButton.SetWindowPos(NULL, R.left, R.top, R.right - R.left, R.bottom - R.top, SWP_NOZORDER);

    if (::Settings.m_ShowSendARKRButton) {
      R = m_BR[0][6];
      m_TopconButton.SetWindowPos(NULL, R.left, R.top, R.right - R.left, R.bottom - R.top, SWP_NOZORDER);
    }
  }

  m_EditExamButton.SetEnabled(TRUE);
  m_EditExamButton.ShowWindow(SW_SHOW);

  m_DeleteExamButton.SetEnabled(!IsDBReadOnly);
  m_DeleteExamButton.ShowWindow(SW_SHOW);

  if (::Settings.m_ShowSendARKRButton)
    m_TopconButton.ShowWindow(SW_SHOW);

  m_BackButton.SetImage(m_q == 2 ? IDR_LEFT_ARROW_3 : m_q == 1 ? IDR_LEFT_ARROW_2 : IDR_LEFT_ARROW_1);
  m_BackButton.ShowWindow(SW_SHOW);

  // set the empty selections of exams if it is come from patient's GUI
  if (m_Last_evel == LAST_LEVEL_MAIN) {
    DeleteSelectedIndicesLists();
    m_IndexSelectedExams.RemoveAll();
    m_WFOSList.m_FelRow = -1;
    m_CTOSList.m_FelRow = -1;
    m_WFOSList.Invalidate(false);
    m_CTOSList.Invalidate(false);
    m_WFODList.m_FelRow = -1;
    m_CTODList.m_FelRow = -1;
    m_WFODList.Invalidate(false);
    m_CTODList.Invalidate(false);
  }
}

//***************************************************************************************

void CMainWnd::HidePatientLevelControls()
{
	m_NewWFExamButton.ShowWindow(SW_HIDE);
	m_NewCTExamButton.ShowWindow(SW_HIDE);
	m_NewWFCTExamButton.ShowWindow(SW_HIDE);
	m_ViewExamButton.ShowWindow(SW_HIDE);

	for (int i = 0; i < 5; i++) if (m_CustExamCreated[i]) m_ViewCustExamButton[i].ShowWindow(SW_HIDE);

	if(m_isComboDicom) m_DicomExportButton.ShowWindow(SW_HIDE);//0129 Dicom Export button

	m_EditExamButton.ShowWindow(SW_HIDE);
	m_DeleteExamButton.ShowWindow(SW_HIDE);
	m_TopconButton.ShowWindow(SW_HIDE);
	m_BackButton.ShowWindow(SW_HIDE);

	m_WFODList.ShowWindow(SW_HIDE);
	m_WFOSList.ShowWindow(SW_HIDE);
	m_CTODList.ShowWindow(SW_HIDE);
	m_CTOSList.ShowWindow(SW_HIDE);
}

//***************************************************************************************

void CMainWnd::UpdateExamsLists()
{
	CBusyCursor Cursor;

	m_WFODList.DeleteAllItems();
	m_WFOSList.DeleteAllItems();
	m_CTODList.DeleteAllItems();
	m_CTOSList.DeleteAllItems();

	::DB.LoadClinics();
	::DB.LoadPhysicians();
	::DB.LoadOperators();

	::DB.LoadExamHeaders(m_Patient.m_PatientID);//Big Database Research since 03062019

	//::DB.LoadTxtExamHeaders(m_Patient.m_PatientID);//Big Database Research since 03062019

	CString s;
	CXPListCtrl* pList;

	for (int e = 0; e < ::DB.m_ExamHeaders.GetSize(); e++)
	{
		CExamHeader* pExamHeader = &::DB.m_ExamHeaders[e];

		if (pExamHeader->m_Type == EXAM_TYPE_WF)
		{
			pList = pExamHeader->IsOD() ? &m_WFODList : &m_WFOSList;
		}
		else if (pExamHeader->m_Type == EXAM_TYPE_CT) {
			pList = pExamHeader->IsOD() ? &m_CTODList : &m_CTOSList;
		}
		else continue;

		int k = pList->GetItemCount();

		// #
		s.Format(_T("%i"), pExamHeader->m_Number);

		//[5.1.1], if the exam has the fellow exam add a note in here
		real_t scanSize = 0;
		int  NumImages = 1;//[5.2]

		if (m_FellowIDCaled)
		{
			s = s + m_FellowIDStr[e];
			scanSize = m_ScanSize[e];
			NumImages = m_NumImages[e];//[5.2]
		}
		else
		{
			BOOL FellowExamExist = FALSE;
			GUID FellowExamID = GUID_NULL;
			if (pExamHeader->m_Type == EXAM_TYPE_WF)
			{
				FellowExamID = ::DB.LoadFellowID(pExamHeader->m_ExamID, scanSize, NumImages, EXAM_TYPE_WF);

				//make sure the fellowexmaID is still exist
				if (FellowExamID != GUID_NULL && ::DB.ExamExist(FellowExamID)) FellowExamExist = TRUE;
			}
			else if (pExamHeader->m_Type == EXAM_TYPE_CT)
			{
				FellowExamID = ::DB.LoadFellowID(pExamHeader->m_ExamID, scanSize, NumImages, EXAM_TYPE_CT);

				if (FellowExamID != GUID_NULL && ::DB.ExamExist(FellowExamID)) FellowExamExist = TRUE;
			}
			else continue;

			m_ScanSize[e] = scanSize;
			m_NumImages[e] = NumImages;

			if (FellowExamExist && !m_isSolo)
			{
				m_FellowIDStr[e] = " .";
				s = s + " .";
			}
			else m_FellowIDStr[e] = "";
		}

		//Check the color Image
		if (NumImages == 2) pList->m_ColorNum[k] = 1;
		else pList->m_ColorNum[k] = 0;

		pList->InsertItem(k, s);

		// date
		pList->SetItemText(k, 1, ::DateToStr(pExamHeader->m_Year, pExamHeader->m_Month, pExamHeader->m_Day));

		// time
		s.Format(_T("%02i:%02i:%02i"), pExamHeader->m_Hour, pExamHeader->m_Minute, pExamHeader->m_Second);
		pList->SetItemText(k, 2, s);

		// software version
		s = pExamHeader->SoftwareVersion();
		pList->SetItemText(k, 3, s);

		// mode
		s = pExamHeader->Mode();
		pList->SetItemText(k, 4, s);

		// clinic
		CClinic* pClinic = ::DB.GetClinic(pExamHeader->m_ClinicID);
		if (pClinic) pList->SetItemText(k, 5, G_ReadChinese(pClinic->m_Name));

		// physician
		CPhysician* pPhysician = ::DB.GetPhysician(pExamHeader->m_PhysicianID);
		if (pPhysician) pList->SetItemText(k, 6, G_ReadChinese(pPhysician->FullName()));

		// operator
		COperator* pOperator = ::DB.GetOperator(pExamHeader->m_OperatorID);
		if (pOperator) pList->SetItemText(k, 7, G_ReadChinese(pOperator->FullName()));

		// preop
		s = pExamHeader->Preop();
		pList->SetItemText(k, 8, s);

		// series number
		s.Format(_T("%i"), pExamHeader->m_SeriesNumber);
		pList->SetItemText(k, 9, s);

		//[5.1.1]
		if (pExamHeader->m_Type == EXAM_TYPE_WF)
		{
			s.Format(_T("%.2f"), scanSize * 0.001);

			pList->SetItemText(k, 10, s);
			pList->SetItemText(k, 11, G_ReadChinese(pExamHeader->m_Note));// note
		}
		else if (pExamHeader->m_Type == EXAM_TYPE_CT)
		{
			pList->SetItemText(k, 10, G_ReadChinese(pExamHeader->m_Note));// note
		}
		else continue;

		pList->SetItemData(k, e);
	}

	m_FellowIDCaled = TRUE;

	m_WFODList.SortItems(CompareWFODExamProc, (LPARAM)this);
	m_WFOSList.SortItems(CompareWFOSExamProc, (LPARAM)this);
	m_CTODList.SortItems(CompareCTODExamProc, (LPARAM)this);
	m_CTOSList.SortItems(CompareCTOSExamProc, (LPARAM)this);

	for (int i = 0; i < m_NumSelectedWFODExams; i++)
	{
		m_WFODList.SetItemState(m_WFOD[i], LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_WFODList.SetSelectionMark(m_WFOD[i]);
		m_WFODList.EnsureVisible(m_WFOD[i], FALSE);
	}

	for (int i = 0; i < m_NumSelectedWFOSExams; i++)
	{
		m_WFOSList.SetItemState(m_WFOS[i], LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_WFOSList.SetSelectionMark(m_WFOS[i]);
		m_WFOSList.EnsureVisible(m_WFOS[i], FALSE);
	}

	for (int i = 0; i < m_NumSelectedCTODExams; i++)
	{
		m_CTODList.SetItemState(m_CTOD[i], LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_CTODList.SetSelectionMark(m_CTOD[i]);
		m_CTODList.EnsureVisible(m_CTOD[i], FALSE);
	}

	for (int i = 0; i < m_NumSelectedCTOSExams; i++)
	{
		m_CTOSList.SetItemState(m_CTOS[i], LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_CTOSList.SetSelectionMark(m_CTOS[i]);
		m_CTOSList.EnsureVisible(m_CTOS[i], FALSE);
	}
	m_RightClick = 0;

	Invalidate(FALSE);
}

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

void CMainWnd::OnAcceptExamButtonClicked()
{
	if (m_pWFExams[m_whichWFExam] && !m_pCTExams[0])
	{
		//export the wf exam to excel
		if (::Settings.m_Export_Excel == 1)
		{
			BOOL SaveExcel = TRUE;
			if (::Settings.m_EXCEL_WFCT_SETS[0] != '1' && ::Settings.m_EXCEL_WFCT_SETS[0] != '0')
			{
				CString s;
				s = "The spreadsheet settings is not set yet.  \nSet the spreadsheet now?  \n\n(You can also set the spreadsheet settings in the Settings dialog.)";
				if (::Question(s, 1, 0))
				{
					CExcelSettingsDlg2* pDlg = new CExcelSettingsDlg2(this, FALSE);

					if (pDlg->DoModal() == IDOK)
					{
					}
					delete pDlg;
				}
				else
				{
					SaveExcel = FALSE;
					::Info("The exam will not be saved to excel file.");
				}
			}

			if (SaveExcel)
			{
				CString FolderName;

				CINIParser IniPar;
				FolderName = IniPar.GetStrKey(IniSection, "Export Excel Folder", IniPath);//6.2.0 ini registration
				FolderName += "\\610WFSpreadsheetRec.xlsx";

				if (::PathFileExists(FolderName))
				{
					while (1)
					{
						fstream iofile(FolderName, ios::in | ios::out | ios::binary);
						if (!iofile)
						{
							::Info("Please close " + FolderName);
						}
						else
						{
							break;
						}
					}
				}

				int res = SaveWFExamToExcel(m_pWFExams[m_whichWFExam], FolderName);
			}
		}
		//Done

		if (!::NewSettings.m_Super_Exam || m_whichWFExam == 2)
		{
			m_AcceptExamButton.ShowWindow(SW_HIDE);
			m_RejectExamButton.ShowWindow(SW_HIDE);
			m_WFExamAcc[m_whichWFExam] = TRUE;
		}
		else
		{
			::DB.SaveWFExam(m_pWFExams[m_whichWFExam]);

			m_WFExamAcc[m_whichWFExam] = TRUE;
			m_whichWFExam++;

			SAFE_DELETE(m_pSumWnd);
			m_pSumWnd = new CWFSingleVrfWnd(this, m_SumRect, &m_Patient, m_pWFExams[m_whichWFExam], &::Settings.m_TSvrSettings[0], m_whichWFExam, ::NewSettings.m_Super_Exam);

			return;
		}

		//Create averaged exam
		int accNum;
		CWFExam* Ave_Exam = new CWFExam;
		int lastNum = 0;
		if (::NewSettings.m_Super_Exam && m_whichWFExam == 2)
		{
			accNum = 0;

			for (int i = 0; i < 3; i++)
			{
				if (m_WFExamAcc[i])
				{
					accNum++;
					lastNum = i;
				}
			}

			if (accNum > 1)
			{
				Matrix<CWFExam*> pWFSeriesExams(accNum);

				int t = 0;
				int lastNum = 0;
				for (int i = 0; i <= 2; i++)
				{
					if (m_WFExamAcc[i])
					{
						pWFSeriesExams[t] = m_pWFExams[i];
						lastNum = i;
						t++;
					}
				}

				CAveExamInfo AveExamInfo;//for average exam

				Ave_Exam->Average(pWFSeriesExams);

				//for average exam
				for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
				{
					AveExamInfo.m_c[z] = Ave_Exam->m_WfSurface.m_c[z];
				}
				//for average exam

				Ave_Exam->m_Header.m_Eye = (m_pWFExams[0]->m_Header.IsOD() ? EYE_OD : EYE_OS);

				::CoCreateGuid(&Ave_Exam->m_Header.m_ExamID);

				Ave_Exam->m_Header.m_Mode = EXAM_MODE_MANUAL;

				Ave_Exam->m_Header.m_Note = "Averaged Exam";

				Ave_Exam->m_Image = m_pWFExams[lastNum]->m_Image;

				::DB.SaveWFExam(Ave_Exam);

				//for average exam
				CString s, SavedFileName;
				GUIDToStr(Ave_Exam->m_Header.m_ExamID, s);
				SavedFileName = MAIN_DIR + "\\Data\\" + s;
				AveExamInfo.SaveIntoFile(SavedFileName);
				//for average exam
			}
		}

		::DB.SaveWFExam(m_pWFExams[m_whichWFExam]);

		m_EnableToggleButton = FALSE;
		m_EnablePlannerButton = FALSE;
		m_EnableWavetouchButtons = TRUE;
		m_EnableEditExamButton = TRUE;
		m_EnableExportDataButton = TRUE;

		ShowCommonSummaryLevelButtons();
		ShowWFSummaryLevelButtons();

		if (::Settings.m_DefaultWFSum >= 0 && ::Settings.m_DefaultWFSum <= 3)
		{
			int num = ::Settings.m_DefaultWFSum;

			if (m_isCombo)
			{
				if (num == 1) num = 4;
				else if (num == 2) num = 1;
				else if (num == 3) num = 5;
			}

			OnWFSumButtonClicked(num);
		}
		else if (::Settings.m_DefaultWFSum == 10)
		{
			m_ReselectExam = TRUE;
			OnWFVfaButtonClicked();
		}
		else if (::Settings.m_DefaultWFSum == 12)
		{
			m_ReselectExam = TRUE;
			OnWFSumButtonClicked(12);
		}
		else
		{
			::Settings.m_DefaultWFSum = 11;

			if (m_whichWFExam == 2)
			{
				SetDicomStduyDesForToggle();//For the 'study des' item in the Dicom file while the summary GUI has been 'OnToggleButtonClicked'

				SAFE_DELETE(m_pSumWnd);

				::TempSettings.CTSumNum = -1;

				if (accNum > 1)
				{
					SAFE_DELETE(m_pWFExams[0]);
					m_pWFExams[0] = Ave_Exam;
					m_pWFExams[0]->m_Image = m_pWFExams[lastNum]->m_Image;
					m_pWFExams[0]->Process();

					//for average exam
					CString ExamId, LoadFileName;
					GUIDToStr(m_pWFExams[0]->m_Header.m_ExamID, ExamId);
					LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

					if (::PathFileExists(LoadFileName))
					{
						CAveExamInfo AveExamInfo;

						AveExamInfo.LoadFromFile(LoadFileName);
						for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
						{
							m_pWFExams[0]->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
						}
					}
					//for average exam Done

					m_pSumWnd = new CWFSingleVrfWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], &::Settings.m_TSvrSettings[0], 0, FALSE);
				}
				else
				{
					for (int i = 0; i <= 2; i++)
					{
						if (m_WFExamAcc[i])
						{
							SAFE_DELETE(m_pWFExams[0]);
							m_pWFExams[0] = new CWFExam;
							::DB.LoadWFExam(m_pWFExams[i]->m_Header.m_ExamID, m_pWFExams[0]);
							m_pWFExams[0]->Process();

							//for average exam
							CString ExamId, LoadFileName;
							GUIDToStr(m_pWFExams[0]->m_Header.m_ExamID, ExamId);
							LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

							if (::PathFileExists(LoadFileName))
							{
								CAveExamInfo AveExamInfo;

								AveExamInfo.LoadFromFile(LoadFileName);
								for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
								{
									m_pWFExams[0]->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
								}
							}
							//for average exam Done

							m_pSumWnd = new CWFSingleVrfWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], &::Settings.m_TSvrSettings[0], 0, FALSE);
							break;
						}
					}
				}

				if (m_BigCTSumWnd != NULL)	m_BigCTSumWnd->ShowWindow(SW_HIDE);

				m_Screen = SCREEN_WFVRF;

				HighlightSummaryLevelButtons();
			}
			else
			{
				OnWFVrfButtonClicked();
			}
		}
	}
	else if (!m_pWFExams[0] && m_pCTExams[0])
	{
		//export the ct exam to excel
		if (::Settings.m_Export_Excel == 1 && !m_ExcelSaved)
		{
			BOOL SaveExcel = TRUE;
			if (::Settings.m_EXCEL_WFCT_SETS[0] != '1' && ::Settings.m_EXCEL_WFCT_SETS[0] != '0')
			{
				CString s;
				s = "The spreadsheet settings is not set yet.  \nSet the spreadsheet now?  \n\n(You can also set the spreadsheet settings in the Settings dialog.)";
				if (::Question(s, 1, 0))
				{
					CExcelSettingsDlg2* pDlg = new CExcelSettingsDlg2(this, FALSE);

					if (pDlg->DoModal() == IDOK)
					{
					}
					delete pDlg;
				}
				else
				{
					SaveExcel = FALSE;
					::Info("The exam will not be saved to excel file.");
				}
			}

			if (SaveExcel)
			{
				CString FolderName;

				CINIParser IniPar;
				FolderName = IniPar.GetStrKey(IniSection, "Export Excel Folder", IniPath);	//6.2.0 ini registration
				FolderName += "\\610WFSpreadsheetRec.xlsx";

				if (::PathFileExists(FolderName))
				{
					while (1)
					{
						fstream iofile(FolderName, ios::in | ios::out | ios::binary);
						if (!iofile)
						{
							::Info("Please close " + FolderName);
						}
						else
						{
							break;
						}
					}
				}

				int res = SaveCTExamToExcel(m_pCTExams[0], FolderName);
			}
			m_ExcelSaved = TRUE;
		}
		//export the ct exam to excel Done

		if (::TempSettings.Temp_ColorImgCpted)
		{
			if (m_Screen == SCREEN_CTVRF)
			{
				m_Screen = SCREEN_SAVE_COLOR_IMG0;
				SAFE_DELETE(m_pSumWnd);
				::TempSettings.CTSumNum = -1;
				m_pSumWnd = new CCTColorImgWnd(this, m_SumRect, &m_Patient, m_pCTExams[0], &::Settings.m_CSvrSettings);
			}
			else
			{
				m_AcceptExamButton.ShowWindow(SW_HIDE);
				m_RejectExamButton.ShowWindow(SW_HIDE);

				::DB.SaveCTExam(m_pCTExams[0]);

				m_EnableToggleButton = FALSE;
				m_EnablePlannerButton = FALSE;
				m_EnableWavetouchButtons = FALSE;
				m_EnableEditExamButton = TRUE;
				m_EnableExportDataButton = TRUE;

				ShowCommonSummaryLevelButtons();
				ShowCTSummaryLevelButtons();

				if (::Settings.m_DefaultCTSum >= 0 && ::Settings.m_DefaultCTSum <= 3)\
				{
					OnCTSumButtonClicked(::Settings.m_DefaultCTSum);
				}
				else if (::Settings.m_DefaultCTSum == 10)
				{
					m_ReselectExam = TRUE;
					OnCTDblButtonClicked();
				}
				else
				{
					::Settings.m_DefaultCTSum = 11;
					OnCTVrfButtonClicked();
				}
			}
		}
		else
		{
			m_AcceptExamButton.ShowWindow(SW_HIDE);
			m_RejectExamButton.ShowWindow(SW_HIDE);

			::DB.SaveCTExam(m_pCTExams[0]);

			m_EnableToggleButton = FALSE;
			m_EnablePlannerButton = FALSE;
			m_EnableWavetouchButtons = FALSE;
			m_EnableEditExamButton = TRUE;
			m_EnableExportDataButton = TRUE;

			ShowCommonSummaryLevelButtons();
			ShowCTSummaryLevelButtons();

			if (::Settings.m_DefaultCTSum >= 0 && ::Settings.m_DefaultCTSum <= 3)
			{
				OnCTSumButtonClicked(::Settings.m_DefaultCTSum);
			}
			else if (::Settings.m_DefaultCTSum == 10)
			{
				m_ReselectExam = TRUE;
				OnCTDblButtonClicked();
			}
			else
			{
				::Settings.m_DefaultCTSum = 11;
				OnCTVrfButtonClicked();
			}
		}
	}
	else if (m_pWFExams[0] && m_pCTExams[0]) // wf and ct
	{
		if (!::NewSettings.m_Super_Exam)
		{
			if (::Settings.m_Export_Excel == 1)
			{
				m_AcceptTime++;
				//export wf and ct exams to excel
				if (!m_ExcelSaved && m_AcceptTime == 2)
				{
					BOOL SaveExcel = TRUE;
					if (::Settings.m_EXCEL_WFCT_SETS[0] != '1' && ::Settings.m_EXCEL_WFCT_SETS[0] != '0')
					{
						CString s;
						s = "The spreadsheet settings is not set yet.  \nSet the spreadsheet now?  \n\n(You can also set the spreadsheet settings in the Settings dialog.)";
						if (::Question(s, 1, 0))
						{
							CExcelSettingsDlg2* pDlg = new CExcelSettingsDlg2(this, FALSE);

							if (pDlg->DoModal() == IDOK)
							{
							}
							delete pDlg;
						}
						else
						{
							SaveExcel = FALSE;
							::Info("The exam will not be saved to excel file.");
						}
					}

					if (SaveExcel)
					{
						CString FolderName;

						CINIParser IniPar;
						FolderName = IniPar.GetStrKey(IniSection, "Export Excel Folder", IniPath);	//6.2.0 ini registration
						FolderName += "\\610WFCTSpreadsheetRec.xlsx";

						if (::PathFileExists(FolderName))
						{
							while (1)
							{
								fstream iofile(FolderName, ios::in | ios::out | ios::binary);
								if (!iofile)
								{
									::Info("Please close " + FolderName);
								}
								else
								{
									break;
								}
							}
						}

						int res = SaveWFCTExamToExcel(m_pWFExams[0], m_pCTExams[0], FolderName);//OnAcceptExamButtonClicked
					}

					m_ExcelSaved = TRUE;
				}
			}
			//export wf and ct exams to excel Done

			if (::TempSettings.Temp_ColorImgCpted)
			{
				if (m_Screen == SCREEN_WFVRF)
				{

					OnCTVrfButtonClicked();
				}
				else if (m_Screen == SCREEN_CTVRF)
				{
					m_Screen = SCREEN_SAVE_COLOR_IMG1;
					SAFE_DELETE(m_pSumWnd);
					::TempSettings.CTSumNum = -1;//[520]
					m_pSumWnd = new CCTColorImgWnd(this, m_SumRect, &m_Patient, m_pCTExams[0], &::Settings.m_CSvrSettings);
				}
				else
				{
					m_AcceptExamButton.ShowWindow(SW_HIDE);
					m_RejectExamButton.ShowWindow(SW_HIDE);

					m_pWFExams[0]->m_Header.m_FellowExamID = m_pCTExams[0]->m_Header.m_ExamID;
					m_pCTExams[0]->m_Header.m_FellowExamID = m_pWFExams[0]->m_Header.m_ExamID;

					::DB.SaveWFExam(m_pWFExams[0]);

					::DB.SaveCTExam(m_pCTExams[0]);

					m_EnableToggleButton = TRUE;
					m_EnablePlannerButton = TRUE;
					m_EnableWavetouchButtons = TRUE;
					m_EnableEditExamButton = TRUE;
					m_EnableExportDataButton = TRUE;

					ShowCommonSummaryLevelButtons();
					ShowINSummaryLevelButtons();

					if (::Settings.m_DefaultINSum < 0 || ::Settings.m_DefaultINSum > 3)
					{
						::Settings.m_DefaultINSum = 0;
					}
					OnINSumButtonClicked(::Settings.m_DefaultINSum);
				}
			}
			else
			{
				if (m_Screen == SCREEN_WFVRF)
				{
					OnCTVrfButtonClicked();
				}
				else
				{
					m_AcceptExamButton.ShowWindow(SW_HIDE);
					m_RejectExamButton.ShowWindow(SW_HIDE);

					m_pWFExams[0]->m_Header.m_FellowExamID = m_pCTExams[0]->m_Header.m_ExamID;
					m_pCTExams[0]->m_Header.m_FellowExamID = m_pWFExams[0]->m_Header.m_ExamID;

					::DB.SaveWFExam(m_pWFExams[0]);
					::DB.SaveCTExam(m_pCTExams[0]);

					m_EnableToggleButton = TRUE;
					m_EnablePlannerButton = TRUE;
					m_EnableWavetouchButtons = TRUE;
					m_EnableEditExamButton = TRUE;
					m_EnableExportDataButton = TRUE;

					ShowCommonSummaryLevelButtons();
					ShowINSummaryLevelButtons();

					if (::Settings.m_DefaultINSum < 0 || ::Settings.m_DefaultINSum > 3)
					{
						::Settings.m_DefaultINSum = 0;
					}
					OnINSumButtonClicked(::Settings.m_DefaultINSum);
				}
			}
		}
		else if (::NewSettings.m_Super_Exam)
		{
			if (m_Screen == SCREEN_WFVRF && m_whichWFExam < 2)
			{
				m_WFExamAcc[m_whichWFExam] = TRUE;
				m_whichWFExam++;

				SAFE_DELETE(m_pSumWnd);
				m_pSumWnd = new CWFSingleVrfWnd(this, m_SumRect, &m_Patient, m_pWFExams[m_whichWFExam], &::Settings.m_TSvrSettings[0], m_whichWFExam, ::NewSettings.m_Super_Exam);

				return;
			}
			else if (m_Screen == SCREEN_WFVRF && m_whichWFExam == 2)
			{
				m_WFExamAcc[m_whichWFExam] = TRUE;
				OnCTVrfButtonClicked();
				return;
			}

			if (::TempSettings.Temp_ColorImgCpted && m_Screen == SCREEN_CTVRF)
			{
				m_Screen = SCREEN_SAVE_COLOR_IMG1;
				SAFE_DELETE(m_pSumWnd);
				::TempSettings.CTSumNum = -1;
				m_pSumWnd = new CCTColorImgWnd(this, m_SumRect, &m_Patient, m_pCTExams[0], &::Settings.m_CSvrSettings);

				return;
			}

			if ((!::TempSettings.Temp_ColorImgCpted && m_Screen == SCREEN_CTVRF)
				|| m_Screen == SCREEN_SAVE_COLOR_IMG1)
			{
				m_AcceptExamButton.ShowWindow(SW_HIDE);
				m_RejectExamButton.ShowWindow(SW_HIDE);

				SaveMultWFCTExams();
			}
		}
	}
}

//***************************************************************************************

//Save multi- wf ct exams
void CMainWnd::SaveMultWFCTExams()
{
	//AveExam
	int accNum = 0;
	CWFExam* Ave_Exam = new CWFExam;

	int lastNum = 0;
	for (int i = 0; i < 3; i++)
	{
		if (m_WFExamAcc[i])
		{
			accNum++;
			lastNum = i;
		}
	}

	if (accNum > 1)
	{
		Matrix<CWFExam*> pWFSeriesExams(accNum);

		int t = 0;
		int lasNum = 0;
		for (int i = 0; i <= 2; i++)
		{
			if (m_WFExamAcc[i])
			{
				pWFSeriesExams[t] = m_pWFExams[i];
				lasNum = i;
				t++;
			}
		}

		Ave_Exam->Average(pWFSeriesExams);

		Ave_Exam->m_Header.m_Eye = (m_pWFExams[0]->m_Header.IsOD() ? EYE_OD : EYE_OS);

		::CoCreateGuid(&Ave_Exam->m_Header.m_ExamID);

		Ave_Exam->m_Header.m_Mode = EXAM_MODE_MANUAL;

		Ave_Exam->m_Header.m_Note = "Averaged Exam";

		Ave_Exam->m_Image = m_pWFExams[lasNum]->m_Image;
	}
	//Ave Exam Done

	//save the exam
	if (accNum > 1)
	{
		//Set the fellow index to ave_exam
		Ave_Exam->m_Header.m_FellowExamID = m_pCTExams[0]->m_Header.m_ExamID;
		m_pCTExams[0]->m_Header.m_FellowExamID = Ave_Exam->m_Header.m_ExamID;
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			if (m_WFExamAcc[i])
			{
				m_pWFExams[i]->m_Header.m_FellowExamID = m_pCTExams[0]->m_Header.m_ExamID;
				m_pCTExams[0]->m_Header.m_FellowExamID = m_pWFExams[i]->m_Header.m_ExamID;
				break;
			}
		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (m_WFExamAcc[i]) ::DB.SaveWFExam(m_pWFExams[i]);
	}

	if (accNum > 1)
	{
		::DB.SaveWFExam(Ave_Exam);

		//for average exam
		CAveExamInfo AveExamInfo;
		for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
		{
			AveExamInfo.m_c[z] = Ave_Exam->m_WfSurface.m_c[z];
		}
		CString s, SavedFileName;
		GUIDToStr(Ave_Exam->m_Header.m_ExamID, s);
		SavedFileName = MAIN_DIR + "\\Data\\" + s;
		AveExamInfo.SaveIntoFile(SavedFileName);
		//for average exam Done
	}

	::DB.SaveCTExam(m_pCTExams[0]);

	//Save Excel
	if (::Settings.m_Export_Excel == 1)
	{
		if (!m_ExcelSaved)
		{
			BOOL SaveExcel = TRUE;
			if (::Settings.m_EXCEL_WFCT_SETS[0] != '1' && ::Settings.m_EXCEL_WFCT_SETS[0] != '0')
			{
				CString s;
				s = "The spreadsheet settings is not set yet.  \nSet the spreadsheet now?  \n\n(You can also set the spreadsheet settings in the Settings dialog.)";
				if (::Question(s, 1, 0))
				{
					CExcelSettingsDlg2* pDlg = new CExcelSettingsDlg2(this, FALSE);

					if (pDlg->DoModal() == IDOK)
					{
					}
					delete pDlg;
				}
				else
				{
					SaveExcel = FALSE;
					::Info("The exam will not be saved to excel file.");
				}
			}

			if (SaveExcel)
			{
				CString FolderName;

				CINIParser IniPar;
				FolderName = IniPar.GetStrKey(IniSection, "Export Excel Folder", IniPath);//6.2.0 ini registration
				FolderName += "\\610WFCTSpreadsheetRec.xlsx";

				if (::PathFileExists(FolderName))
				{
					while (1)
					{
						fstream iofile(FolderName, ios::in | ios::out | ios::binary);
						if (!iofile)
						{
							::Info("Please close " + FolderName);
						}
						else
						{
							break;
						}
					}
				}

				if (accNum > 1)  int res = SaveWFCTExamToExcel(Ave_Exam, m_pCTExams[0], FolderName);
				else
				{
					for (int i = 0; i < 3; i++)
					{
						if (m_WFExamAcc[i])
						{
							int res = SaveWFCTExamToExcel(m_pWFExams[i], m_pCTExams[0], FolderName);
						}
					}
				}
			}

			m_ExcelSaved = TRUE;
		}
	}

	m_EnableToggleButton = TRUE;
	m_EnablePlannerButton = TRUE;
	m_EnableWavetouchButtons = TRUE;
	m_EnableEditExamButton = TRUE;
	m_EnableExportDataButton = TRUE;

	ShowCommonSummaryLevelButtons();
	ShowINSummaryLevelButtons();

	if (::Settings.m_DefaultINSum < 0 || ::Settings.m_DefaultINSum > 3)
	{
		::Settings.m_DefaultINSum = 0;
	}

	if (accNum > 1)
	{
		SAFE_DELETE(m_pWFExams[0]);
		m_pWFExams[0] = Ave_Exam;
		m_pWFExams[0]->m_Points = m_pWFExams[lastNum]->m_Points;
		m_pWFExams[0]->m_DetectorType = DT_LINEAR_ARRAYS;
		m_pWFExams[0]->m_Image = m_pWFExams[lastNum]->m_Image;
		m_pWFExams[0]->Process();

		//for average exam
		CString ExamId, LoadFileName;
		GUIDToStr(m_pWFExams[0]->m_Header.m_ExamID, ExamId);
		LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

		if (::PathFileExists(LoadFileName))
		{
			CAveExamInfo AveExamInfo;

			AveExamInfo.LoadFromFile(LoadFileName);
			for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
			{
				m_pWFExams[0]->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
			}
		}
		//for average exam Done
	}
	else if (accNum == 1)
	{
		for (int i = 0; i <= 2; i++)
		{
			if (m_WFExamAcc[i])
			{
				if (i != 0)
				{
					SAFE_DELETE(m_pWFExams[0]);
					m_pWFExams[0] = new CWFExam;
					::DB.LoadWFExam(m_pWFExams[i]->m_Header.m_ExamID, m_pWFExams[0]);
					m_pWFExams[0]->Process();

					//for average exam
					CString ExamId, LoadFileName;
					GUIDToStr(m_pWFExams[0]->m_Header.m_ExamID, ExamId);
					LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

					if (::PathFileExists(LoadFileName))
					{
						CAveExamInfo AveExamInfo;

						AveExamInfo.LoadFromFile(LoadFileName);
						for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
						{
							m_pWFExams[0]->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
						}
					}
					//for average exam Done
				}
				break;
			}
		}
	}

	OnINSumButtonClicked(::Settings.m_DefaultINSum);
}

//***************************************************************************************

void CMainWnd::OnRejectExamButtonClicked()
{
	if (m_Screen == SCREEN_SAVE_COLOR_IMG0)
	{
		m_pCTExams[0]->m_ColorImage.m_RGBData.Destroy();

		m_AcceptExamButton.ShowWindow(SW_HIDE);
		m_RejectExamButton.ShowWindow(SW_HIDE);

		::DB.SaveCTExam(m_pCTExams[0]);

		m_EnableToggleButton = FALSE;
		m_EnablePlannerButton = FALSE;
		m_EnableWavetouchButtons = FALSE;
		m_EnableEditExamButton = TRUE;
		m_EnableExportDataButton = TRUE;

		ShowCommonSummaryLevelButtons();
		ShowCTSummaryLevelButtons();

		if (::Settings.m_DefaultCTSum >= 0 && ::Settings.m_DefaultCTSum <= 3)
		{
			OnCTSumButtonClicked(::Settings.m_DefaultCTSum);
		}
		else if (::Settings.m_DefaultCTSum == 10)
		{
			m_ReselectExam = TRUE;
			OnCTDblButtonClicked();
		}
		else
		{
			::Settings.m_DefaultCTSum = 11;
			OnCTVrfButtonClicked();
		}
	}
	else if (m_Screen == SCREEN_SAVE_COLOR_IMG1)
	{
		m_pCTExams[0]->m_ColorImage.m_RGBData.Destroy();

		m_AcceptExamButton.ShowWindow(SW_HIDE);
		m_RejectExamButton.ShowWindow(SW_HIDE);

		::DB.SaveWFExam(m_pWFExams[0]);
		::DB.SaveCTExam(m_pCTExams[0]);

		m_EnableToggleButton = TRUE;
		m_EnablePlannerButton = TRUE;
		m_EnableWavetouchButtons = TRUE;
		m_EnableEditExamButton = TRUE;
		m_EnableExportDataButton = TRUE;

		ShowCommonSummaryLevelButtons();
		ShowINSummaryLevelButtons();

		if (::Settings.m_DefaultINSum < 0 || ::Settings.m_DefaultINSum > 3)
		{
			::Settings.m_DefaultINSum = 0;
		}
		OnINSumButtonClicked(::Settings.m_DefaultINSum);
	}
	else
	{
		if (::NewSettings.m_Super_Exam && m_whichWFExam < 2 && m_Screen == SCREEN_WFVRF)
		{
			m_WFExamAcc[m_whichWFExam] = FALSE;//6.2.0

			m_whichWFExam++;

			SAFE_DELETE(m_pSumWnd);
			m_pSumWnd = new CWFSingleVrfWnd(this, m_SumRect, &m_Patient, m_pWFExams[m_whichWFExam], &::Settings.m_TSvrSettings[0], m_whichWFExam, ::NewSettings.m_Super_Exam);

			return;
		}

		//Create averaged exam
		if (!(m_pWFExams[0] && m_pCTExams[0]))
		{
			if (::NewSettings.m_Super_Exam && m_whichWFExam == 2 && m_Screen == SCREEN_WFVRF)
			{
				m_WFExamAcc[m_whichWFExam] = FALSE;//6.2.0

				int accNum = 0;
				CWFExam* Ave_Exam = new CWFExam;

				for (int i = 0; i < 3; i++)
				{
					if (m_WFExamAcc[i]) accNum++;
				}

				if (accNum > 1)
				{
					Matrix<CWFExam*> pWFSeriesExams(accNum);

					int t = 0;
					int lastNum = 0;
					for (int i = 0; i <= 2; i++)
					{
						if (m_WFExamAcc[i])
						{
							pWFSeriesExams[t] = m_pWFExams[i];
							lastNum = i;
							t++;
						}
					}

					Ave_Exam->Average(pWFSeriesExams);

					Ave_Exam->m_Header.m_Eye = (m_pWFExams[0]->m_Header.IsOD() ? EYE_OD : EYE_OS);

					::CoCreateGuid(&Ave_Exam->m_Header.m_ExamID);

					Ave_Exam->m_Header.m_Mode = EXAM_MODE_MANUAL;

					Ave_Exam->m_Header.m_Note = "Averaged Exam";

					Ave_Exam->m_Image = m_pWFExams[lastNum]->m_Image;

					::DB.SaveWFExam(Ave_Exam);

					//for average exam
					CAveExamInfo AveExamInfo;
					for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
					{
						AveExamInfo.m_c[z] = Ave_Exam->m_WfSurface.m_c[z];
					}
					CString s, SavedFileName;
					GUIDToStr(Ave_Exam->m_Header.m_ExamID, s);
					SavedFileName = MAIN_DIR + "\\Data\\" + s;
					AveExamInfo.SaveIntoFile(SavedFileName);
					//for average exam Done

					//show the result on window
					SAFE_DELETE(m_pWFExams[0]);
					m_pWFExams[0] = Ave_Exam;
					m_pWFExams[0]->m_Image = m_pWFExams[lastNum]->m_Image;
					m_pWFExams[0]->Process();

					//for average exam
					CString ExamId, LoadFileName;
					GUIDToStr(m_pWFExams[0]->m_Header.m_ExamID, ExamId);
					LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

					if (::PathFileExists(LoadFileName))
					{
						CAveExamInfo AveExamInfo;

						AveExamInfo.LoadFromFile(LoadFileName);
						for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
						{
							m_pWFExams[0]->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
						}
					}
					//for average exam Done

					HideAllSummaryLevelButtons();
					m_EnableToggleButton = FALSE;
					m_EnablePlannerButton = FALSE;
					m_EnableWavetouchButtons = TRUE;
					m_EnableEditExamButton = TRUE;
					m_EnableExportDataButton = TRUE;

					ShowCommonSummaryLevelButtons();
					ShowWFSummaryLevelButtons();

					SetDicomStduyDesForToggle();//the 'study des' item in the Dicom file while the summary GUI has been 'OnToggleButtonClicked'

					SAFE_DELETE(m_pSumWnd);

					::TempSettings.CTSumNum = -1;

					m_pSumWnd = new CWFSingleVrfWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], &::Settings.m_TSvrSettings[0], 0, FALSE);

					if (m_BigCTSumWnd != NULL)	m_BigCTSumWnd->ShowWindow(SW_HIDE);

					m_Screen = SCREEN_WFVRF;

					HighlightSummaryLevelButtons();

					return;
				}

				else if (accNum == 1)
				{
					HideAllSummaryLevelButtons();
					m_EnableToggleButton = FALSE;
					m_EnablePlannerButton = FALSE;
					m_EnableWavetouchButtons = TRUE;
					m_EnableEditExamButton = TRUE;
					m_EnableExportDataButton = TRUE;

					ShowCommonSummaryLevelButtons();
					ShowWFSummaryLevelButtons();

					SetDicomStduyDesForToggle();//For the 'study des' item in the Dicom file while the summary GUI has been 'OnToggleButtonClicked'

					SAFE_DELETE(m_pSumWnd);

					::TempSettings.CTSumNum = -1;

					for (int i = 0; i <= 2; i++)
					{
						if (m_WFExamAcc[i])
						{
							if (i != 0)
							{
								SAFE_DELETE(m_pWFExams[0]);
								m_pWFExams[0] = new CWFExam;
								::DB.LoadWFExam(m_pWFExams[i]->m_Header.m_ExamID, m_pWFExams[0]);
								m_pWFExams[0]->Process();

								//for average exam
								CString ExamId, LoadFileName;
								GUIDToStr(m_pWFExams[0]->m_Header.m_ExamID, ExamId);
								LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

								if (::PathFileExists(LoadFileName))
								{
									CAveExamInfo AveExamInfo;

									AveExamInfo.LoadFromFile(LoadFileName);
									for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
									{
										m_pWFExams[0]->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
									}
								}
								//for average exam Done
							}
							m_pSumWnd = new CWFSingleVrfWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], &::Settings.m_TSvrSettings[0], 0, FALSE);
							break;
						}
					}

					if (m_BigCTSumWnd != NULL)	m_BigCTSumWnd->ShowWindow(SW_HIDE);	//[520]

					m_Screen = SCREEN_WFVRF;

					HighlightSummaryLevelButtons();
					return;
				}
			}
		}
		else
		{
			if (::NewSettings.m_Super_Exam && m_whichWFExam == 2 && m_Screen == SCREEN_WFVRF)
			{
				int accNum = 0;

				for (int i = 0; i < 3; i++)
				{
					if (m_WFExamAcc[i]) accNum++;
				}

				if (accNum != 0)
				{
					OnCTVrfButtonClicked();
					return;
				}
			}
		}

		SAFE_DELETE(m_pSumWnd);

		m_Screen = SCREEN_NONE;

		HideAllSummaryLevelButtons();

		DeleteLoadedExams();

		m_Level = LEVEL_PATIENT;
		Invalidate(FALSE);

		ShowPatientLevelControls();

		UpdateExamsLists();
	}
}

//***************************************************************************************

void CMainWnd::OnToggleButtonClicked()
{
	if (m_DisScreen == 0)
	{
		HideWFSummaryLevelButtons();
		ShowCTSummaryLevelButtons();

		m_TransType = 12;//Dicom File m_StudyDes
	}
	else if (m_DisScreen == 1)
	{
		HideCTSummaryLevelButtons();
		ShowINSummaryLevelButtons();

		m_TransType = 23;//Dicom File m_StudyDes
	}
	else if (m_DisScreen == 2)
	{
		HideINSummaryLevelButtons();
		ShowWFSummaryLevelButtons();

		m_TransType = 31;//Dicom File m_StudyDes
	}
}

//***************************************************************************************

void CMainWnd::OnWFVrfButtonClicked()
{
	if (m_Screen == SCREEN_WFVRF) return;

	SetDicomStduyDesForToggle();//the 'study des' item in the Dicom file while the summary GUI has been 'OnToggleButtonClicked'

	SAFE_DELETE(m_pSumWnd);

	::TempSettings.CTSumNum = -1;

	m_pSumWnd = new CWFSingleVrfWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], &::Settings.m_TSvrSettings[0], 0, FALSE);

	if (m_BigCTSumWnd != NULL)	m_BigCTSumWnd->ShowWindow(SW_HIDE);

	m_Screen = SCREEN_WFVRF;

	HighlightSummaryLevelButtons();
}

//***************************************************************************************
int GetMaxHorValue(real_t fmax)
{
	int val = intRound(fmax * 10 + 0.5);//fix by 0711 2018
	if (val == 1) val = 2;
	return val;
}

void CMainWnd::OnWFSumButtonClicked(int k)
{
	CBusyCursor Cursor;//6.2.0 New Bug

	BOOL DelSumWnd = TRUE;

	if (k == 0 && m_Screen == SCREEN_WFSUM1) return;
	if (k == 1 && m_Screen == SCREEN_WFSUM2  && !m_isSolo) return;
	if (k == 2 && m_Screen == SCREEN_WFSUM3) return;
	if (k == 3)
	{
		if (!m_isCombo)
		{
			if (m_Screen == SCREEN_WFSUM4) return;
		}
		else
		{
			if (m_WhichTaskBtn == -1) DelSumWnd = FALSE;
		}
	}
	if (k == 4 && m_Screen == SCREEN_WFSUM5) return;

	if (k == 5 && m_Screen == SCREEN_WFSUM6) return;

	if (k == 6 && m_Screen == SCREEN_WFSUM7) return;//6.2.0 WF Near vision

	if (m_isSolo && k == 1 && m_Screen == SCREEN_WFSUM2 && m_SoloControl) return;

	if (DelSumWnd)
	{
		SAFE_DELETE(m_pSumWnd);
	}

	SetDicomStduyDesForToggle();//the 'study des' item in the Dicom file while the summary GUI has been 'OnToggleButtonClicked'

	::TempSettings.CTSumNum = -1;

	if (k == 2) ::TempSettings.Temp_WFAngel = TRUE;
	else       ::TempSettings.Temp_WFAngel = FALSE;

	BOOL TaskReforAbe = FALSE; // it is Refraction Analysis or Aberration Analysis under task functionality
	if (m_WhichTaskBtn != -1)
	{
		int Type = ::NewSettings.m_Tasks[m_WhichTaskBtn].m_SubButton[m_TaskSubK];

		if (Type == TYPE_REFA || Type == TYPE_ABEA)
		{
			if (Type == TYPE_REFA && m_Screen == SCREEN_WFSUM2)
			{
				for (int i = 0; i < 8; i++)
				{
					if (::NewSettings.m_Tasks[m_WhichTaskBtn].m_SubButton[i] == TYPE_ABEA)
					{
						m_TaskSubK = i;
						TaskReforAbe = TRUE;

						if (m_TaskSubK == 0)  m_Screen = SCREEN_TASKSUB0;
						if (m_TaskSubK == 1)  m_Screen = SCREEN_TASKSUB1;
						if (m_TaskSubK == 2)  m_Screen = SCREEN_TASKSUB2;
						if (m_TaskSubK == 3)  m_Screen = SCREEN_TASKSUB3;
						if (m_TaskSubK == 4)  m_Screen = SCREEN_TASKSUB4;
						if (m_TaskSubK == 5)  m_Screen = SCREEN_TASKSUB5;
						if (m_TaskSubK == 6)  m_Screen = SCREEN_TASKSUB6;
						if (m_TaskSubK == 7)  m_Screen = SCREEN_TASKSUB7;
						if (m_TaskSubK == 8)  m_Screen = SCREEN_TASKSUB8;

						HighlightTaskSubButtons();
						break;
					}
				}
			}
			else if (Type == TYPE_ABEA && m_Screen == SCREEN_WFSUM1)
			{
				for (int i = 0; i < 8; i++)
				{
					if (::NewSettings.m_Tasks[m_WhichTaskBtn].m_SubButton[i] == TYPE_REFA)
					{
						m_TaskSubK = i;
						TaskReforAbe = TRUE;

						if (m_TaskSubK == 0)  m_Screen = SCREEN_TASKSUB0;
						if (m_TaskSubK == 1)  m_Screen = SCREEN_TASKSUB1;
						if (m_TaskSubK == 2)  m_Screen = SCREEN_TASKSUB2;
						if (m_TaskSubK == 3)  m_Screen = SCREEN_TASKSUB3;
						if (m_TaskSubK == 4)  m_Screen = SCREEN_TASKSUB4;
						if (m_TaskSubK == 5)  m_Screen = SCREEN_TASKSUB5;
						if (m_TaskSubK == 6)  m_Screen = SCREEN_TASKSUB6;
						if (m_TaskSubK == 7)  m_Screen = SCREEN_TASKSUB7;
						if (m_TaskSubK == 8)  m_Screen = SCREEN_TASKSUB8;

						HighlightTaskSubButtons();
						break;
					}
				}
			}
			else TaskReforAbe = TRUE;
		}
	}

	if (m_isSolo || TaskReforAbe)
	{
		if (k == 1) //Refraction Analysis
		{
			if (m_pWFExams[1] == NULL || m_ReselectExam)
			{
				GUID SecondExamID = GUID_NULL;

				BOOL IsFirstOD = m_pWFExams[0]->m_Header.IsOD();
				BOOL IsFirstOS = m_pWFExams[0]->m_Header.IsOS();
				int n = 0;
				for (int e = 0; e < ::DB.m_ExamHeaders.GetSize(); e++)
				{
					CExamHeader* pExamHeader = &::DB.m_ExamHeaders[e];
					if (pExamHeader->m_Type == EXAM_TYPE_WF && ((pExamHeader->IsOD() && IsFirstOS) || (pExamHeader->IsOS() && IsFirstOD))) {
						SecondExamID = pExamHeader->m_ExamID;
						n++;
					}
				}

				if (n > 1)
				{
					m_SelNum = 0;

					int number = 0;
					POSITION position;
					if (m_pWFExams[0]->m_Header.IsOD())
						position = m_WFODList.GetFirstSelectedItemPosition();
					else position = m_WFOSList.GetFirstSelectedItemPosition();

					if (position)
					{
						int i, e;
						if (m_pWFExams[0]->m_Header.IsOD())
						{
							i = m_WFODList.GetNextSelectedItem(position);
							e = m_WFODList.GetItemData(i);
						}
						else
						{
							i = m_WFOSList.GetNextSelectedItem(position);
							e = m_WFOSList.GetItemData(i);
						}
						number = ::DB.m_ExamHeaders[e].m_Number;
					}

					CSelectExamDlg* pDlg = new CSelectExamDlg(this, m_Patient.m_PatientID, number, EXAM_TYPE_WF, IsFirstOD ? EYE_OS : EYE_OD, &m_Font, m_NumImages);
					if (pDlg->DoModal() == IDOK)
					{
						SecondExamID = pDlg->m_ExamID;
						m_SelNum = pDlg->m_SelNum;
					}
					else
					{
						SecondExamID = GUID_NULL;
					}

					for (int i = 0; i < m_SelNum; i++)
					{
						m_ExamIDs[i] = pDlg->m_ExamIDs[i];
					}

					delete pDlg;
				}

				if (n == 1) m_SelNum = 1;

				SAFE_DELETE(m_pWFExams[1]);

				if (SecondExamID != GUID_NULL)
				{
					m_pWFExams[1] = new CWFExam;

					if (m_SelNum == 1)
					{
						if (IsFirstOD)
							::OS_AverageExam = 0;
						else
							::OD_AverageExam = 0;

						if (::DB.LoadWFExam(SecondExamID, m_pWFExams[1]))
						{
							m_pWFExams[1]->Process();

							//for average exam
							CString ExamId, LoadFileName;
							GUIDToStr(m_pWFExams[1]->m_Header.m_ExamID, ExamId);
							LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

							if (::PathFileExists(LoadFileName))
							{
								CAveExamInfo AveExamInfo;

								AveExamInfo.LoadFromFile(LoadFileName);
								for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
								{
									m_pWFExams[1]->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
								}
							}
							//for average exam Done
						}

						else SAFE_DELETE(m_pWFExams[1]);
					}
					else //means it is multiple exams have been selected(average them)
					{
						if (IsFirstOD)
							::OS_AverageExam = m_SelNum;
						else
							::OD_AverageExam = m_SelNum;

						BOOL SOK = TRUE;
						Matrix<CWFExam*> pWFSeriesExams(m_SelNum);

						for (int s = 0; s < m_SelNum; s++)
						{
							CWFExam* loadExam = new CWFExam;
							if (::DB.LoadWFExam(m_ExamIDs[s], loadExam))
							{
								loadExam->Process();

								//for average exam
								CString ExamId, LoadFileName;
								GUIDToStr(loadExam->m_Header.m_ExamID, ExamId);
								LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

								if (::PathFileExists(LoadFileName))
								{
									CAveExamInfo AveExamInfo;

									AveExamInfo.LoadFromFile(LoadFileName);
									for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
									{
										loadExam->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
									}
								}
								//for average exam Done

								pWFSeriesExams[s] = loadExam;

								if (IsFirstOD)
									::OS_SelectExamsID[s] = m_ExamIDs[s];
								else  ::OD_SelectExamsID[s] = m_ExamIDs[s];
							}
							else
							{
								SAFE_DELETE(m_pWFExams[1]);
								SOK = FALSE;
								break;
							}
						}

						if (SOK)
							m_pWFExams[1]->Average(pWFSeriesExams);
					}
				}
			}

			for (int i = 0; i < 4; i++)
			{
				if (::NewSettings.m_WFSoloSettings[0][i].m_Type != TYPE_TWFM
					&& ::NewSettings.m_WFSoloSettings[0][i].m_Type != TYPE_TRFM
					&& ::NewSettings.m_WFSoloSettings[0][i].m_Type != TYPE_TLTR
					)
				{
					::NewSettings.m_WFSoloSettings[0][i].m_Type = TYPE_TLTR;
				}

				int MaskType = ::NewSettings.m_WFSoloSettings[0][i].m_Mask.GetType();
				if (MaskType != MASK_TOTAL &&
					MaskType != MASK_HO_TOTAL)
				{
					if (i == 0 || i == 2) ::NewSettings.m_WFSoloSettings[0][i].m_Mask.SetType(MASK_TOTAL);
					if (i == 1 || i == 2) ::NewSettings.m_WFSoloSettings[0][i].m_Mask.SetType(MASK_HO_TOTAL);
				}
			}

			m_pSumWnd = new CWFDoubleSoloWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], m_pWFExams[1], &::NewSettings.m_WFSoloSettings[0][0]);//***

			if (!m_SoloControl) m_ReselectExam = TRUE;
		}

		if (k == 0) //Aberration Analysis
		{
			m_pSumWnd = new CWFSingleSoloWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], &::NewSettings.m_WFSoloSettings[1][0]);
		}

	}
	else
	{
		if (m_WhichTaskBtn != -1 && ::NewSettings.m_Tasks[m_WhichTaskBtn].m_SelfSettings[m_TaskSubK] == TRUE)
		{
			int pos = k;
			if (k == 3 && m_isCombo)  pos = 4;

			if (pos == 2)//WFCT Angle k / a
				m_pSumWnd = new CWFCTAngleSumWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], m_pCTExams[0], &::Settings.m_TSsmSettings[2][0], 2);
			else
				m_pSumWnd = new CWFSingleSumWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], &::NewSettings.m_Tasks[m_WhichTaskBtn].m_WndSettings[m_TaskSubK][0], pos);
		}
		else
		{
			if (m_isCombo)
			{
				//After add new 'depth of focus comparison' the order of k has been changed,
				//Thus change the order back according to original oreder.

				// = 0 : Visual Acuity; = 1 :Angle K/A  ; 2 :depth of focus;   3:  depth of focus comparison;
				// 4: WF/RMS; // 5 : Custom

				int pos = k;

				if (m_WhichTaskBtn == -1)
				{
					if (k == 1) pos = 2; //Angle K / A
					if (k == 2) pos = 3; //Depth of focus
					if (k == 3) pos = 5; //Depth of focus comparison
					if (k == 4) pos = 1; //WF/RMS
					if (k == 5) pos = 4; //Custom
				}

				if (pos == 3)//depth of focus
				{
					m_pSumWnd = new CWFSingleSumWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], &::NewSettings.m_TSsm5Settings[0], 4);
				}
				else if (pos <= 4)
				{
					if (pos == 4) pos = 3;

					if (pos == 2)//WFCT Angle k/a
						m_pSumWnd = new CWFCTAngleSumWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], m_pCTExams[0], &::Settings.m_TSsmSettings[2][0], 2);
					else
						m_pSumWnd = new CWFSingleSumWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], &::Settings.m_TSsmSettings[pos][0], pos);
				}

				//6.2.0 WF Near Vision
				else if (pos == 6)
				{
					int WFExamOrder;
					int WFExamAmount = 0;

					//Get WFExamAmount
					for (int e = 0; e < ::DB.m_ExamHeaders.GetSize(); e++)
					{
						CExamHeader* pExamHeader = &::DB.m_ExamHeaders[e];

						if (pExamHeader->m_Type == EXAM_TYPE_WF)
						{
							WFExamAmount++;
						}
					}
					//Get WFExamAmount Done

					//Get WFExamOrder
					POSITION position;
					if (m_pWFExams[0]->m_Header.IsOD())
						position = m_WFODList.GetFirstSelectedItemPosition();
					else position = m_WFOSList.GetFirstSelectedItemPosition();

					if (position)
					{
						int i, e;
						if (m_pWFExams[0]->m_Header.IsOD())
						{
							i = m_WFODList.GetNextSelectedItem(position);
							e = m_WFODList.GetItemData(i);
						}
						else
						{
							i = m_WFOSList.GetNextSelectedItem(position);
							e = m_WFOSList.GetItemData(i);
						}
						WFExamOrder = ::DB.m_ExamHeaders[e].m_Number;
					}
					//Get WFExamOrder Done

					CWFExam    *pWFExamFar = new CWFExam;

					::DB.LoadWFExam(m_pWFExams[0]->m_Header.m_ExamID, pWFExamFar);

					pWFExamFar->Process();

					CWFExam    *pWFExamNear = new CWFExam;

					::DB.LoadWFExam(m_pWFExams[0]->m_Header.m_ExamID, pWFExamNear);

					pWFExamNear->Process();

					m_pSumWnd = new CWFNearVisionSumWnd(this, m_SumRect, &m_Patient, pWFExamFar, pWFExamNear, WFExamAmount, WFExamOrder);
				}
				//6.2.0 WF Near Vision

				//pos = 5 Depth of focus comparison
				else
				{
					int number = 0;
					POSITION position;
					if (m_pWFExams[0]->m_Header.IsOD())
						position = m_WFODList.GetFirstSelectedItemPosition();
					else position = m_WFOSList.GetFirstSelectedItemPosition();

					if (position)
					{
						int i, e;
						if (m_pWFExams[0]->m_Header.IsOD())
						{
							i = m_WFODList.GetNextSelectedItem(position);
							e = m_WFODList.GetItemData(i);
						}
						else
						{
							i = m_WFOSList.GetNextSelectedItem(position);
							e = m_WFOSList.GetItemData(i);
						}
						number = ::DB.m_ExamHeaders[e].m_Number;
					}

					CSelectExamDlg* pDlg = new CSelectExamDlg(this, m_Patient.m_PatientID, number, EXAM_TYPE_WF, EYE_BOTH, &m_Font, m_NumImages);

					GUID ExamID = GUID_NULL;
					if (pDlg->DoModal() == IDOK)
					{
						ExamID = pDlg->m_ExamID;
					}
					else
					{
						delete pDlg;
						if (m_pSumWnd == NULL)
						{
							OnWFVrfButtonClicked();
						}
						return;
					}

					delete pDlg;

					CWFExam* SelWFExam = new CWFExam;
					if (ExamID != GUID_NULL)
					{
						SAFE_DELETE(m_pSumWnd);
						::DB.LoadWFExam(ExamID, SelWFExam);
						SelWFExam->Process();
						m_pSumWnd = new CWFDOFCWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], SelWFExam, &::NewSettings.m_TSsm6Settings[0]);
					}
				}
				//pos = 5 Depth of focus comparison
			}
			else
				m_pSumWnd = new CWFSingleSumWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], &::Settings.m_TSsmSettings[k][0], k);
		}
	}

	switch (k)
	{
	case 0: m_Screen = SCREEN_WFSUM1; break;
	case 1: m_Screen = SCREEN_WFSUM2; break;
	case 2: m_Screen = SCREEN_WFSUM3; break;
	case 3: m_Screen = SCREEN_WFSUM4; break;
	case 4: m_Screen = SCREEN_WFSUM5; break;
	case 5: m_Screen = SCREEN_WFSUM6; break;
	case 6: m_Screen = SCREEN_WFSUM7; break;//6.2.0 WF Near vision
	}

	if (m_BigCTSumWnd != NULL)	m_BigCTSumWnd->ShowWindow(SW_HIDE);

	HighlightSummaryLevelButtons();
}

//***************************************************************************************

void CMainWnd::OnWFCmpButtonClicked()
{
	int number = 0;
	POSITION position;
	if (m_pWFExams[0]->m_Header.IsOD())
		position = m_WFODList.GetFirstSelectedItemPosition();
	else position = m_WFOSList.GetFirstSelectedItemPosition();

	if (position)
	{
		int i, e;
		if (m_pWFExams[0]->m_Header.IsOD())
		{
			i = m_WFODList.GetNextSelectedItem(position);
			e = m_WFODList.GetItemData(i);
		}
		else
		{
			i = m_WFOSList.GetNextSelectedItem(position);
			e = m_WFOSList.GetItemData(i);
		}
		number = ::DB.m_ExamHeaders[e].m_Number;
	}

	CSelectExamDlg* pDlg = new CSelectExamDlg(this, m_Patient.m_PatientID, number, EXAM_TYPE_WF, m_pWFExams[0]->m_Header.IsOD() ? EYE_OD : EYE_OS, &m_Font, m_NumImages);

	GUID ExamID = GUID_NULL;

	m_SelNum = 0;//For multiple selection
	if (pDlg->DoModal() == IDOK)
	{
		ExamID = pDlg->m_ExamID;
		m_SelNum = pDlg->m_SelNum;//521
		m_WFCompareDone = TRUE;//530
	}
	else m_WFCompareDone = FALSE;

	for (int i = 0; i < m_SelNum; i++)
	{
		m_ExamIDs[i] = pDlg->m_ExamIDs[i];
	}

	delete pDlg;

	if (ExamID != GUID_NULL)
	{
		CBusyCursor Cursor;

		if (m_SelNum == 1)
		{
			CWFExam* pWFExam = new CWFExam;
			if (::DB.LoadWFExam(ExamID, pWFExam))
			{
				pWFExam->Process();

				// for average exam
				CString ExamId, LoadFileName;
				GUIDToStr(pWFExam->m_Header.m_ExamID, ExamId);
				LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

				if (::PathFileExists(LoadFileName))
				{
					CAveExamInfo AveExamInfo;

					AveExamInfo.LoadFromFile(LoadFileName);
					for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
					{
						pWFExam->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
					}
				}
				//for average exam Done

				SAFE_DELETE(m_pSumWnd);
				SAFE_DELETE(m_pWFExams[1]);
				m_pWFExams[1] = pWFExam;

				if (m_pWFExams[0]->m_Header.IsOD())
					::OD_AverageExam2 = 0;
				else
					::OS_AverageExam2 = 0;

				SetDicomStduyDesForToggle();//For the 'study des' item in the Dicom file while the summary GUI has been 'OnToggleButtonClicked'
				::TempSettings.CTSumNum = -1;
				if (m_isSolo)
					m_pSumWnd = new CWFSoloCmpWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], m_pWFExams[1], &::Settings.m_TScmSettings);
				else m_pSumWnd = new CWFSingleCmpWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], m_pWFExams[1], &::Settings.m_TScmSettings);

				m_Screen = SCREEN_WFCMP;

				HighlightSummaryLevelButtons();
			}
			else delete pWFExam;
		}
		else if (m_SelNum > 1)
		{
			BOOL SOK = TRUE;
			Matrix<CWFExam*> pWFSeriesExams(m_SelNum);

			if (m_pWFExams[0]->m_Header.IsOD())
				::OD_AverageExam2 = m_SelNum;
			else
				::OS_AverageExam2 = m_SelNum;

			for (int s = 0; s < m_SelNum; s++)
			{
				CWFExam* loadExam = new CWFExam;
				if (::DB.LoadWFExam(m_ExamIDs[s], loadExam))
				{
					loadExam->Process();

					// for average exam
					CString ExamId, LoadFileName;
					GUIDToStr(loadExam->m_Header.m_ExamID, ExamId);
					LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

					if (::PathFileExists(LoadFileName))
					{
						CAveExamInfo AveExamInfo;

						AveExamInfo.LoadFromFile(LoadFileName);
						for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
						{
							loadExam->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
						}
					}
					//for average exam Done

					pWFSeriesExams[s] = loadExam;

					if (m_pWFExams[0]->m_Header.IsOD())
						::OD_SelectExamsID2[s] = m_ExamIDs[s];
					else
						::OS_SelectExamsID2[s] = m_ExamIDs[s];
				}
				else
				{
					SAFE_DELETE(m_pWFExams[1]);
					SOK = FALSE;
					break;
				}
			}

			if (SOK)
			{
				SAFE_DELETE(m_pSumWnd);
				SAFE_DELETE(m_pWFExams[1]);
				m_pWFExams[1] = new CWFExam;
				m_pWFExams[1]->Average(pWFSeriesExams);
				SetDicomStduyDesForToggle();//the 'study des' item in the Dicom file while the summary GUI has been 'OnToggleButtonClicked'
				::TempSettings.CTSumNum = -1;

				if (m_isSolo)
					m_pSumWnd = new CWFSoloCmpWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], m_pWFExams[1], &::Settings.m_TScmSettings);
				else m_pSumWnd = new CWFSingleCmpWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], m_pWFExams[1], &::Settings.m_TScmSettings);

				m_Screen = SCREEN_WFCMP;

				HighlightSummaryLevelButtons();
			}
		}

		if (m_BigCTSumWnd != NULL)
		{
			m_BigCTSumWnd->ShowWindow(SW_HIDE);
		}
	}
	else
	{
		if (m_WhichTaskBtn != -1)
		{
			if (m_FirstTask == 0) OnBackButtonClicked();//Default task
		}
	}
	m_ReselectExam = TRUE;
}

//***************************************************************************************

void CMainWnd::OnWFVfaButtonClicked()
{
	if (m_isSolo && m_Screen == SCREEN_WFVFA && m_SoloControl) return;

	if (m_pWFExams[1] == NULL || m_ReselectExam)
	{
		GUID SecondExamID = GUID_NULL;

		BOOL IsFirstOD = m_pWFExams[0]->m_Header.IsOD();
		BOOL IsFirstOS = m_pWFExams[0]->m_Header.IsOS();
		int n = 0;
		for (int e = 0; e < ::DB.m_ExamHeaders.GetSize(); e++)
		{
			CExamHeader* pExamHeader = &::DB.m_ExamHeaders[e];
			if (pExamHeader->m_Type == EXAM_TYPE_WF && ((pExamHeader->IsOD() && IsFirstOS) || (pExamHeader->IsOS() && IsFirstOD)))
			{
				SecondExamID = pExamHeader->m_ExamID;
				n++;
			}
		}

		if (n > 1)
		{
			m_SelNum = 0;

			int number = 0;
			POSITION position;
			if (m_pWFExams[0]->m_Header.IsOD())
				position = m_WFODList.GetFirstSelectedItemPosition();
			else position = m_WFOSList.GetFirstSelectedItemPosition();

			if (position)
			{
				int i, e;
				if (m_pWFExams[0]->m_Header.IsOD())
				{
					i = m_WFODList.GetNextSelectedItem(position);
					e = m_WFODList.GetItemData(i);
				}
				else
				{
					i = m_WFOSList.GetNextSelectedItem(position);
					e = m_WFOSList.GetItemData(i);
				}
				number = ::DB.m_ExamHeaders[e].m_Number;
			}

			CSelectExamDlg* pDlg = new CSelectExamDlg(this, m_Patient.m_PatientID, number, EXAM_TYPE_WF, IsFirstOD ? EYE_OS : EYE_OD, &m_Font, m_NumImages);
			if (pDlg->DoModal() == IDOK)
			{
				SecondExamID = pDlg->m_ExamID;
				m_SelNum = pDlg->m_SelNum;
			}
			else
			{
				SecondExamID = GUID_NULL;
			}

			for (int i = 0; i < m_SelNum; i++)
			{
				m_ExamIDs[i] = pDlg->m_ExamIDs[i];
			}

			delete pDlg;
		}

		if (n == 1) m_SelNum = 1;//521

		SAFE_DELETE(m_pWFExams[1]);

		if (SecondExamID != GUID_NULL)
		{
			m_pWFExams[1] = new CWFExam;

			if (m_SelNum == 1)
			{
				if (IsFirstOD)
					::OS_AverageExam = 0;
				else
					::OD_AverageExam = 0;

				if (::DB.LoadWFExam(SecondExamID, m_pWFExams[1]))
				{
					m_pWFExams[1]->Process();

					//for average exam
					CString ExamId, LoadFileName;
					GUIDToStr(m_pWFExams[1]->m_Header.m_ExamID, ExamId);
					LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

					if (::PathFileExists(LoadFileName))
					{
						CAveExamInfo AveExamInfo;

						AveExamInfo.LoadFromFile(LoadFileName);
						for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
						{
							m_pWFExams[1]->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
						}
					}
					//for average exam Done
				}

				else SAFE_DELETE(m_pWFExams[1]);
			}
			else //means it is multiple exams have been selected(average them)
			{
				if (IsFirstOD)
					::OS_AverageExam = m_SelNum;
				else
					::OD_AverageExam = m_SelNum;

				BOOL SOK = TRUE;
				Matrix<CWFExam*> pWFSeriesExams(m_SelNum);

				for (int s = 0; s < m_SelNum; s++)
				{
					CWFExam* loadExam = new CWFExam;
					if (::DB.LoadWFExam(m_ExamIDs[s], loadExam))
					{
						loadExam->Process();

						//for average exam
						CString ExamId, LoadFileName;
						GUIDToStr(loadExam->m_Header.m_ExamID, ExamId);
						LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

						if (::PathFileExists(LoadFileName))
						{
							CAveExamInfo AveExamInfo;

							AveExamInfo.LoadFromFile(LoadFileName);
							for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
							{
								loadExam->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
							}
						}
						//for average exam Done

						pWFSeriesExams[s] = loadExam;

						if (IsFirstOD)
							::OS_SelectExamsID[s] = m_ExamIDs[s];
						else  ::OD_SelectExamsID[s] = m_ExamIDs[s];
					}
					else
					{
						SAFE_DELETE(m_pWFExams[1]);
						SOK = FALSE;
						break;
					}
				}

				if (SOK)
					m_pWFExams[1]->Average(pWFSeriesExams);
			}
		}
	}

	SAFE_DELETE(m_pSumWnd);

	SetDicomStduyDesForToggle();//the 'study des' item in the Dicom file while the summary GUI has been 'OnToggleButtonClicked'

	::TempSettings.CTSumNum = -1;

	if (m_isSolo)
		m_pSumWnd = new CWFSoloVfaWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], m_pWFExams[1], &::Settings.m_TDsmSettings[0]);
	else
		m_pSumWnd = new CWFDoubleSumWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], m_pWFExams[1], &::Settings.m_TDsmSettings[0]);

	if (!m_SoloControl) m_ReselectExam = TRUE;

	if (m_BigCTSumWnd != NULL)	m_BigCTSumWnd->ShowWindow(SW_HIDE);

	m_Screen = SCREEN_WFVFA;

	HighlightSummaryLevelButtons();
}

//***************************************************************************************

void CMainWnd::OnCTVrfButtonClicked()
{
	m_pCTExams[0]->m_OpData.m_OK = FALSE;//6.2.0
	m_pCTExams[0]->m_OpData.m_Ori_CorneaCycliderAdj = m_pCTExams[0]->m_OpData.m_CorneaCycliderAdj;
	m_pCTExams[0]->m_OpData.m_CorneaCycliderAdj = FALSE;//6.2.0

	if (m_Screen == SCREEN_CTVRF) return;

	SAFE_DELETE(m_pSumWnd);

	SetDicomStduyDesForToggle();//the 'study des' item in the Dicom file while the summary GUI has been 'OnToggleButtonClicked'

	::TempSettings.CTSumNum = -1;

	m_pSumWnd = new CCTSingleVrfWnd(this, m_SumRect, &m_Patient, m_pCTExams[0], &::Settings.m_CSvrSettings);

	m_Screen = SCREEN_CTVRF;

	if (m_BigCTSumWnd != NULL)	m_BigCTSumWnd->ShowWindow(SW_HIDE);

	HighlightSummaryLevelButtons();
}

//***************************************************************************************

void CMainWnd::OnCTSumButtonClicked(int k)
{
	CBusyCursor Cursor;//6.2.0 New Bug

	m_pCTExams[0]->m_OpData.m_OK = FALSE;//6.2.0
	m_pCTExams[0]->m_OpData.m_Ori_CorneaCycliderAdj = m_pCTExams[0]->m_OpData.m_CorneaCycliderAdj;
	m_pCTExams[0]->m_OpData.m_CorneaCycliderAdj = FALSE;//6.2.0

	::TempSettings.CTSumNum = k;

	if (k == 0 && m_Screen == SCREEN_CTSUM1) return;
	if (k == 1 && m_Screen == SCREEN_CTSUM2) return;
	if (k == 2 && m_Screen == SCREEN_CTSUM3) return;
	if (k == 3 && m_Screen == SCREEN_CTSUM4) return;
	if (k == 4 && m_Screen == SCREEN_CTSUM5) return;

	SetDicomStduyDesForToggle();//For the 'study des' item in the Dicom file while the summary GUI has been 'OnToggleButtonClicked'

								//for Osher alignment button in the combo version
	switch (k)
	{
	case 0: m_Screen = SCREEN_CTSUM1; break;
	case 1: m_Screen = SCREEN_CTSUM2; break;
	case 2: m_Screen = SCREEN_CTSUM3; break;
	case 3: m_Screen = SCREEN_CTSUM4; break;
	case 4: m_Screen = SCREEN_CTSUM5; break;
	}

	//[520] Show the OSHER ALIGNMENT WINDOW, Bug
	if (::NewSettings.m_OSHER_ALIG_CUS[k] == 1)
	{
		if (m_pSumWnd != NULL) m_pSumWnd->ShowWindow(SW_HIDE);
		SAFE_DELETE(m_BigCTSumWnd);

		if (k == 3)//6.2.0
		{
			m_BigCTSumWnd = new CCTOsherAliWnd(this, m_SumRect, m_pCTExams[0], TRUE);
		}
		else
		{
			m_BigCTSumWnd = new CCTOsherAliWnd(this, m_SumRect, m_pCTExams[0], FALSE);
		}

		m_BigCTSumWnd->m_Screen = m_Screen;

		m_BigCTSumWnd->ShowWindow(SW_SHOW);
		HighlightSummaryLevelButtons();

		switch (k)
		{
		case 0: m_Screen = SCREEN_BIG_CT_OSHER1; break;
		case 1: m_Screen = SCREEN_BIG_CT_OSHER2; break;
		case 2: m_Screen = SCREEN_BIG_CT_OSHER3; break;
		case 3: m_Screen = SCREEN_BIG_CT_OSHER4; break;
		}
	}
	else
	{
		SAFE_DELETE(m_pSumWnd);

		if (m_WhichTaskBtn != -1 && ::NewSettings.m_Tasks[m_WhichTaskBtn].m_SelfSettings[m_TaskSubK] == TRUE)
		{
			m_pSumWnd = new CCTSingleSumWnd(this, m_SumRect, &m_Patient, m_pCTExams[0], &::NewSettings.m_Tasks[m_WhichTaskBtn].m_WndSettings[m_TaskSubK][0], k);
		}
		else
		{
			if (k == 4) k = 3;

			m_pSumWnd = new CCTSingleSumWnd(this, m_SumRect, &m_Patient, m_pCTExams[0], &::Settings.m_CSsmSettings[k][0], k);
		}

		if (m_BigCTSumWnd != NULL)	m_BigCTSumWnd->ShowWindow(SW_HIDE);
		HighlightSummaryLevelButtons();
	}
}

//***************************************************************************************

void CMainWnd::OnCTCmpButtonClicked()
{
	m_pCTExams[0]->m_OpData.m_OK = FALSE;//6.2.0
	m_pCTExams[0]->m_OpData.m_Ori_CorneaCycliderAdj = m_pCTExams[0]->m_OpData.m_CorneaCycliderAdj;
	m_pCTExams[0]->m_OpData.m_CorneaCycliderAdj = FALSE;//6.2.0

	int number = 0;
	POSITION position;
	if (m_pCTExams[0]->m_Header.IsOD())
		position = m_CTODList.GetFirstSelectedItemPosition();
	else position = m_CTOSList.GetFirstSelectedItemPosition();

	if (position)
	{
		int i, e;
		if (m_pCTExams[0]->m_Header.IsOD())
		{
			i = m_CTODList.GetNextSelectedItem(position);
			e = m_CTODList.GetItemData(i);
		}
		else
		{
			i = m_CTOSList.GetNextSelectedItem(position);
			e = m_CTOSList.GetItemData(i);
		}
		number = ::DB.m_ExamHeaders[e].m_Number;
	}

	CSelectExamDlg* pDlg = new CSelectExamDlg(this, m_Patient.m_PatientID, number, EXAM_TYPE_CT, m_pCTExams[0]->m_Header.IsOD() ? EYE_OD : EYE_OS, &m_Font, m_NumImages);

	GUID ExamID = GUID_NULL;

	if (pDlg->DoModal() == IDOK)
	{
		ExamID = pDlg->m_ExamID;
		m_CTCompareDone = TRUE;
	}
	else
	{
		m_CTCompareDone = FALSE;
	}

	delete pDlg;

	if (ExamID != GUID_NULL)
	{
		CBusyCursor Cursor;

		CCTExam* pCTExam = new CCTExam;

		if (::DB.LoadCTExam(ExamID, pCTExam))
		{

			pCTExam->Process();

			SAFE_DELETE(m_pSumWnd);

			SAFE_DELETE(m_pCTExams[1]);

			m_pCTExams[1] = pCTExam;

			SetDicomStduyDesForToggle();//the 'study des' item in the Dicom file while the summary GUI has been 'OnToggleButtonClicked'

			::TempSettings.CTSumNum = -1;

			m_pSumWnd = new CCTSingleCmpWnd(this, m_SumRect, &m_Patient, m_pCTExams[0], m_pCTExams[1], &::Settings.m_CScmSettings);

			m_Screen = SCREEN_CTCMP;

			HighlightSummaryLevelButtons();
		}

		else delete pCTExam;

		if (m_BigCTSumWnd != NULL)	m_BigCTSumWnd->ShowWindow(SW_HIDE);
	}
	else
	{
		if (m_WhichTaskBtn != -1)
		{
			if (m_FirstTask == 0) OnBackButtonClicked();//Default task
		}
	}
	m_ReselectExam = TRUE;
}

//***************************************************************************************

void CMainWnd::OnINSum5ButtonClicked()
{
	if (m_isCombo && m_selK != 3 && m_WhichTaskBtn == -1)
	{
		m_selK = 4;
		OnINSum6ButtonClicked(); //IOL Selection
		return;
	}

	CBusyCursor Cursor;

	BOOL IsFirstOD = m_pWFExams[0]->m_Header.IsOD();
	BOOL IsFirstOS = m_pWFExams[0]->m_Header.IsOS();

	int WFn = 0;
	int CTn = 0;
	for (int e = 0; e < ::DB.m_ExamHeaders.GetSize(); e++)
	{
		CExamHeader* pExamHeader = &::DB.m_ExamHeaders[e];
		if (pExamHeader->m_Type == EXAM_TYPE_WF && ((pExamHeader->IsOD() && IsFirstOS) || (pExamHeader->IsOS() && IsFirstOD)))
		{
			WFn++;
		}
		if (pExamHeader->m_Type == EXAM_TYPE_CT && ((pExamHeader->IsOD() && IsFirstOS) || (pExamHeader->IsOS() && IsFirstOD)))
		{
			CTn++;
		}
	}

	GUID SecondWFExamID = GUID_NULL;
	GUID SecondCTExamID = GUID_NULL;

	if (WFn == 1 && CTn == 1)
	{
		int i1, i2;//the position of fellow WF and CT exams
		if (IsFirstOD)
		{
			i1 = (int)m_WFOSList.GetItemData(0);
			i2 = (int)m_CTOSList.GetItemData(0);
		}
		else
		{
			i1 = (int)m_WFODList.GetItemData(0);
			i2 = (int)m_CTODList.GetItemData(0);
		}

		SecondWFExamID = ::DB.m_ExamHeaders[i1].m_ExamID;
		SecondCTExamID = ::DB.m_ExamHeaders[i2].m_ExamID;
	}
	else if (WFn == 0 || CTn == 0)
	{
	}
	else
	{
		CSelectTwoExamDlg* pDlg = new CSelectTwoExamDlg(this, m_Patient.m_PatientID, IsFirstOD ? EYE_OS : EYE_OD, &m_Font, WFn, CTn, m_ScanSize, m_NumImages, m_FellowIDStr);
		if (pDlg->DoModal() == IDOK)
		{
			SecondWFExamID = pDlg->m_WFExamID;
			SecondCTExamID = pDlg->m_CTExamID;
			delete pDlg;
		}
		else
		{
			delete pDlg;
		}
	}

	Cursor;
	//Load selected fellow WF and CT exams
	//WF
	SAFE_DELETE(m_pWFExams[1]);

	if (SecondWFExamID != GUID_NULL)
	{
		m_pWFExams[1] = new CWFExam;

		if (::DB.LoadWFExam(SecondWFExamID, m_pWFExams[1]))
		{
			m_pWFExams[1]->Process();

			//for average exam
			CString ExamId, LoadFileName;
			GUIDToStr(m_pWFExams[1]->m_Header.m_ExamID, ExamId);
			LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

			if (::PathFileExists(LoadFileName))
			{
				CAveExamInfo AveExamInfo;

				AveExamInfo.LoadFromFile(LoadFileName);
				for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
				{
					m_pWFExams[1]->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
				}
			}
			//for average exam Done
		}
		else
		{
			::Error("Failed to load exams.");
			SAFE_DELETE(m_pWFExams[1]);

			if (m_WhichTaskBtn != -1)
			{
				if (m_FirstTask == 0) OnBackButtonClicked();//530 Default task
				else
				{
					m_TaskSubK = m_Ori_TaskSubK;

					if (m_Ori_TaskSubK == 0)  m_Screen = SCREEN_TASKSUB0;
					if (m_Ori_TaskSubK == 1)  m_Screen = SCREEN_TASKSUB1;
					if (m_Ori_TaskSubK == 2)  m_Screen = SCREEN_TASKSUB2;
					if (m_Ori_TaskSubK == 3)  m_Screen = SCREEN_TASKSUB3;
					if (m_Ori_TaskSubK == 4)  m_Screen = SCREEN_TASKSUB4;
					if (m_Ori_TaskSubK == 5)  m_Screen = SCREEN_TASKSUB5;
					if (m_Ori_TaskSubK == 6)  m_Screen = SCREEN_TASKSUB6;
					if (m_Ori_TaskSubK == 7)  m_Screen = SCREEN_TASKSUB7;
					if (m_Ori_TaskSubK == 8)  m_Screen = SCREEN_TASKSUB8;

					HighlightTaskSubButtons();
				}
				return;
			}
			return;
		}
	}

	//CT
	SAFE_DELETE(m_pCTExams[1]);

	if (SecondCTExamID != GUID_NULL)
	{
		m_pCTExams[1] = new CCTExam;

		if (::DB.LoadCTExam(SecondCTExamID, m_pCTExams[1]))
		{
			m_pCTExams[1]->Process();
		}
		else
		{
			::Error("Failed to load exams.");
			SAFE_DELETE(m_pCTExams[1]);

			if (m_WhichTaskBtn != -1)
			{
				if (m_FirstTask == 0) OnBackButtonClicked();//Default task
				else
				{
					m_TaskSubK = m_Ori_TaskSubK;

					if (m_Ori_TaskSubK == 0)  m_Screen = SCREEN_TASKSUB0;
					if (m_Ori_TaskSubK == 1)  m_Screen = SCREEN_TASKSUB1;
					if (m_Ori_TaskSubK == 2)  m_Screen = SCREEN_TASKSUB2;
					if (m_Ori_TaskSubK == 3)  m_Screen = SCREEN_TASKSUB3;
					if (m_Ori_TaskSubK == 4)  m_Screen = SCREEN_TASKSUB4;
					if (m_Ori_TaskSubK == 5)  m_Screen = SCREEN_TASKSUB5;
					if (m_Ori_TaskSubK == 6)  m_Screen = SCREEN_TASKSUB6;
					if (m_Ori_TaskSubK == 7)  m_Screen = SCREEN_TASKSUB7;
					if (m_Ori_TaskSubK == 8)  m_Screen = SCREEN_TASKSUB8;

					HighlightTaskSubButtons();
				}
				return;
			}
			return;
		}
	}
	//Done


	if (m_BigCTSumWnd != NULL)
	{
		m_BigCTSumWnd->ShowWindow(SW_HIDE);
	}

	m_Screen = SCREEN_INSUM5;

	SAFE_DELETE(m_pSumWnd);

	m_DicomInfo.m_StudyDes = "OU Wavefront and Corneal Topography";//For the 'study des' item in the Dicom file while the summary GUI has been 'OnToggleButtonClicked'

	::TempSettings.CTSumNum = -1;

	m_pSumWnd = new CWFCTOUSumWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], m_pWFExams[1], m_pCTExams[0], m_pCTExams[1], &::NewSettings.m_OWFCTSettings[0]);

	HighlightSummaryLevelButtons();

	m_selK = 0;
}


//***************************************************************************************

void CMainWnd::OnINSum6ButtonClicked()
{
	if (m_isCombo && m_selK != 4 && m_WhichTaskBtn == -1)
	{
		m_selK = 10;
		OnINSumButtonClicked(m_selK); //IOL Selection
		return;
	}

	CBusyCursor Cursor;

	if (m_BigCTSumWnd != NULL)	m_BigCTSumWnd->ShowWindow(SW_HIDE);

	m_Screen = SCREEN_INSUM6;

	SAFE_DELETE(m_pSumWnd);

	m_DicomInfo.m_StudyDes = "IOL Selection Analysis";//For the 'study des' item in the Dicom file while the summary GUI has been 'OnToggleButtonClicked'

	::TempSettings.CTSumNum = -1;

	m_pSumWnd = new CWFCTIolWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], m_pCTExams[0], &::NewSettings.m_IOLSelSettings[0]);

	HighlightSummaryLevelButtons();

	m_selK = 0;
}

//***************************************************************************************

void CMainWnd::OnCTDblButtonClicked()
{
	m_pCTExams[0]->m_OpData.m_OK = FALSE;//6.2.0
	m_pCTExams[0]->m_OpData.m_Ori_CorneaCycliderAdj = m_pCTExams[0]->m_OpData.m_CorneaCycliderAdj;
	m_pCTExams[0]->m_OpData.m_CorneaCycliderAdj = FALSE;//6.2.0

	if (m_pCTExams[1] == NULL || m_ReselectExam)
	{
		GUID SecondExamID = GUID_NULL;

		BOOL IsFirstOD = m_pCTExams[0]->m_Header.IsOD();
		BOOL IsFirstOS = m_pCTExams[0]->m_Header.IsOS();
		int n = 0;
		for (int e = 0; e < ::DB.m_ExamHeaders.GetSize(); e++)
		{
			CExamHeader* pExamHeader = &::DB.m_ExamHeaders[e];
			if (pExamHeader->m_Type == EXAM_TYPE_CT && ((pExamHeader->IsOD() && IsFirstOS) || (pExamHeader->IsOS() && IsFirstOD)))
			{
				SecondExamID = pExamHeader->m_ExamID;
				n++;
			}
		}
		if (n > 1)
		{
			int number = 0;
			POSITION position;
			if (m_pCTExams[0]->m_Header.IsOD())
				position = m_CTODList.GetFirstSelectedItemPosition();
			else position = m_CTOSList.GetFirstSelectedItemPosition();

			if (position)
			{
				int i, e;
				if (m_pCTExams[0]->m_Header.IsOD())
				{
					i = m_CTODList.GetNextSelectedItem(position);
					e = m_CTODList.GetItemData(i);
				}
				else
				{
					i = m_CTOSList.GetNextSelectedItem(position);
					e = m_CTOSList.GetItemData(i);
				}
				number = ::DB.m_ExamHeaders[e].m_Number;
			}

			CSelectExamDlg* pDlg = new CSelectExamDlg(this, m_Patient.m_PatientID, number, EXAM_TYPE_CT, IsFirstOD ? EYE_OS : EYE_OD, &m_Font, m_NumImages);
			if (pDlg->DoModal() == IDOK)
			{
				SecondExamID = pDlg->m_ExamID;
			}
			else
			{
				SecondExamID = GUID_NULL;
			}
			delete pDlg;
		}

		SAFE_DELETE(m_pCTExams[1]);

		if (SecondExamID != GUID_NULL)
		{
			m_pCTExams[1] = new CCTExam;

			if (::DB.LoadCTExam(SecondExamID, m_pCTExams[1]))
			{
				m_pCTExams[1]->Process();
			}

			else SAFE_DELETE(m_pCTExams[1]);
		}
	}

	SAFE_DELETE(m_pSumWnd);

	SetDicomStduyDesForToggle();//the 'study des' item in the Dicom file while the summary GUI has been 'OnToggleButtonClicked'

	::TempSettings.CTSumNum = -1;

	m_pSumWnd = new CCTDoubleSumWnd(this, m_SumRect, &m_Patient, m_pCTExams[0], m_pCTExams[1], &::Settings.m_CDsmSettings[0]);

	m_Screen = SCREEN_CTDBL;

	if (m_BigCTSumWnd != NULL)	m_BigCTSumWnd->ShowWindow(SW_HIDE);

	HighlightSummaryLevelButtons();
}

//***************************************************************************************

void CMainWnd::OnINSumButtonClicked(int k)
{
	CBusyCursor Cursor;//6.2.0 New Bug

	m_pCTExams[0]->m_OpData.m_OK = FALSE;//6.2.0
	m_pCTExams[0]->m_OpData.m_Ori_CorneaCycliderAdj = m_pCTExams[0]->m_OpData.m_CorneaCycliderAdj;
	m_pCTExams[0]->m_OpData.m_CorneaCycliderAdj = FALSE;//6.2.0

	if (m_isCombo)
	{
		if (k == 10)
		{
			k = 6;
			if (m_pWFExams[0]->m_ScanDiameter < 2500)
			{
				::Info("The scan size is less than 2.50mm and cannot be used to generate the Dysfunctional Lens Index.\n Please re-acquire the exam if a DLI is desired.");
				return;
			}
		}
		else if (k == 6)
		{
			k = 7;// Dys patient
			if (m_pWFExams[0]->m_ScanDiameter < 2500)
			{
				::Info("The scan size is less than 2.50mm and cannot be used to generate the Dysfunctional Lens Index.\nPlease re-acquire the exam if a DLI is desired.");
				return;
			}
		}
		else if (k == 7)   k = 8;
		else if (k == 8)   k = 9;

		if (k == 11)
		{
			if (m_pWFExams[0]->m_AlignmentMethod == ALIGNMENT_PUPIL_CENTER)
			{
				::Info("The Angle K/A display is only available when viewing a wavefront exam acquired on visual axis.\nThe wavefront exam chosen was acquired with pupil centration.");
				return;
			}

			if (m_pCTExams[0]->m_Image.m_li_ok == FALSE)
			{
				::Info("The Angle K/A can not be shown if the limbus was not detected on the CT display.(Limbus detection can be obtained by selecting right click menu 'Edit pupil, limbus and rings' under CT->Rings.)");
				return;
			}
		}
	}

	m_selK = k;
	if (m_isCombo && m_selK == 3)
	{
		OnINSum5ButtonClicked(); //OU OverView
		return;
	}

	::TempSettings.Temp_WFAngel = FALSE;

	if (k == 0 && m_Screen == SCREEN_INSUM1) return;
	if (k == 1 && m_Screen == SCREEN_INSUM2) return;
	if (k == 2 && m_Screen == SCREEN_INSUM3) return;
	if (k == 3 && m_Screen == SCREEN_INSUM4) return;
	if (k == 6 && m_Screen == SCREEN_INSUM7) return;
	if (k == 7 && m_Screen == SCREEN_INSUM8) return;
	if (k == 8 && m_Screen == SCREEN_INSUM9) return;
	if (k == 9 && m_Screen == SCREEN_INSUM10) return;
	if (k == 11 && m_Screen == SCREEN_INSUM11) return;//WFCT Angle k/a
	if (k == 12 && m_Screen == SCREEN_INSUM12) return;//Toric Alignment

	switch (k)
	{
	case 0: m_Screen = SCREEN_INSUM1; break;
	case 1: m_Screen = SCREEN_INSUM2; break;
	case 2: m_Screen = SCREEN_INSUM3; break;
	case 3: m_Screen = SCREEN_INSUM4; break;
	case 6: m_Screen = SCREEN_INSUM7; break;
	case 7: m_Screen = SCREEN_INSUM8; break;
	case 8: m_Screen = SCREEN_INSUM9; break;
	case 9: m_Screen = SCREEN_INSUM10; break;
	case 11: m_Screen = SCREEN_INSUM11; break;//WFCT Angle k/a
	case 12: m_Screen = SCREEN_INSUM12; break;//Toric Alignment
	}

	SAFE_DELETE(m_pSumWnd);

	SetDicomStduyDesForToggle();//the 'study des' item in the Dicom file while the summary GUI has been 'OnToggleButtonClicked'

	::TempSettings.CTSumNum = -1;

	if (k < 6)
	{
		if (m_WhichTaskBtn != -1 && ::NewSettings.m_Tasks[m_WhichTaskBtn].m_SelfSettings[m_TaskSubK] == TRUE)
		{
			m_pSumWnd = new CWFCTSumWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], m_pCTExams[0], &::NewSettings.m_Tasks[m_WhichTaskBtn].m_WndSettings[m_TaskSubK][0], k);
		}
		else
		{
			m_pSumWnd = new CWFCTSumWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], m_pCTExams[0], &::Settings.m_IDsmSettings[k][0], k);
		}
	}
	else if (k == 6)
	{
		if (m_WhichTaskBtn != -1)
		{
			if (::NewSettings.m_Tasks[m_WhichTaskBtn].m_SubButton[m_TaskSubK] == TYPE_DYSF)
			{
				m_pSumWnd = new CWFCTDysWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], m_pCTExams[0], &::NewSettings.m_DysfuncSettings[0], k);
			}
			else if (::NewSettings.m_Tasks[m_WhichTaskBtn].m_SubButton[m_TaskSubK] == TYPE_ASTI)
			{
				if (::NewSettings.m_Tasks[m_WhichTaskBtn].m_SelfSettings[m_TaskSubK] == TRUE)
					m_pSumWnd = new CWFCTSumWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], m_pCTExams[0], &::NewSettings.m_Tasks[m_WhichTaskBtn].m_WndSettings[m_TaskSubK][0], k);
				else
					m_pSumWnd = new CWFCTSumWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], m_pCTExams[0], &::NewSettings.m_InAstigSettings[0], k);
			}
		}
		else
		{
			//k == 6 is Dysfunctional Lens Analysis display
			if (m_isCombo)
				m_pSumWnd = new CWFCTDysWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], m_pCTExams[0], &::NewSettings.m_DysfuncSettings[0], k);
			else
				m_pSumWnd = new CWFCTSumWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], m_pCTExams[0], &::NewSettings.m_InAstigSettings[0], k);
		}
	}
	else if (k == 7)
	{
		m_pSumWnd = new CWFCTDysPtWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], m_pCTExams[0], &::NewSettings.m_DysPtSettings[0], 0);
	}
	else if (k == 8)
	{
		if (m_WhichTaskBtn != -1)
		{
			if (::NewSettings.m_Tasks[m_WhichTaskBtn].m_SelfSettings[m_TaskSubK] == TRUE)
				m_pSumWnd = new CWFCTSumWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], m_pCTExams[0], &::NewSettings.m_Tasks[m_WhichTaskBtn].m_WndSettings[m_TaskSubK][0], 6);
			else   m_pSumWnd = new CWFCTSumWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], m_pCTExams[0], &::NewSettings.m_InAstigSettings[0], 6);
		}
		else       m_pSumWnd = new CWFCTSumWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], m_pCTExams[0], &::NewSettings.m_InAstigSettings[0], 6);
	}
	else if (k == 9)
	{
		m_pSumWnd = new CWFCTSumWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], m_pCTExams[0], &::Settings.m_IDsmSettings[3][0], 3);//custom
	}
	else if (k == 11)//WFCT Angle k/a
	{
		m_pSumWnd = new CWFCTAngleSumWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], m_pCTExams[0], &::Settings.m_TSsmSettings[2][0], 2);
	}
	else if (k == 12)// WFCT Toric Alignment
	{
		CWFExam* pWFExam = m_pWFExams[0];
		CCTExam* pCTExam = m_pCTExams[0];

		if (!pCTExam->m_OpData.m_OK)
		{
			if (pCTExam->m_sk_ok) {
				pCTExam->m_OpData.m_CorneaPreopCylSimK = pCTExam->m_sk_df_dp;
				pCTExam->m_OpData.m_CorneaPreopAxisSimK = pCTExam->m_sk_st_dg;
			}

			CZernikeSurface Surface = pCTExam->m_WfSurface;

			Surface.ChangeRMaxUm(2000.0);

			real_t SphEq, Sph, Cyl; int Axis;
			CWFExam::GetSpheqSphCylAxis(Surface, 0, TRUE, SphEq, Sph, Cyl, Axis);
			Cyl = 0.01 * intRound(100.0 * Cyl);

			pCTExam->m_OpData.m_CorneaPreopCyl = pCTExam->m_OpData.m_CorneaPreopCylWF = Cyl;
			pCTExam->m_OpData.m_CorneaPreopAxis = pCTExam->m_OpData.m_CorneaPreopAxisWF = Axis;

			if (pWFExam)
			{
				Surface = pWFExam->m_WfSurface;
				Surface.ChangeRMaxUm(__min(2000.0, Surface.m_r_max_um));
				CWFExam::GetSpheqSphCylAxis(Surface, 0, TRUE, SphEq, Sph, Cyl, Axis);
				Cyl = 0.01 * intRound(100.0 * Cyl);
				pCTExam->m_OpData.m_EyePreopCyl = pCTExam->m_OpData.m_EyePreopCylWF = Cyl;
				pCTExam->m_OpData.m_EyePreopAxis = pCTExam->m_OpData.m_EyePreopAxisWF = Axis;
			}
		}

		m_pSumWnd = new CWFCTTorAliWnd(this, m_SumRect, &m_Patient, pWFExam, pCTExam);//new test window for Toric Alighment Check
	}
	//WFCT Toric Alignment Done

	if (m_BigCTSumWnd != NULL)
	{
		m_BigCTSumWnd->ShowWindow(SW_HIDE);
	}

	HighlightSummaryLevelButtons();
}


//***************************************************************************************

void CMainWnd::OnTaskSubBtnClicked(int k)
{
	CBusyCursor Cursor;//6.2.0 New Bug

	::TempSettings.Temp_WFAngel = FALSE;

	int Type = ::NewSettings.m_Tasks[m_WhichTaskBtn].m_SubButton[k];
	if ((Type == TYPE_DYSF || Type == TYPE_DYSP) && m_pWFExams[0]->m_ScanDiameter < 2500)
	{
		::Info("The scan size is less than 2.50mm and cannot be used to generate the Dysfunctional Lens Index.\n Please re-acquire the exam if a DLI is desired.");

		if (m_FirstTask == 0)
		{
			BOOL FoundOther = FALSE;
			for (int i = 0; i < ::NewSettings.m_Tasks[m_WhichTaskBtn].m_SubBtnNum; i++)
			{
				if (i != k && ::NewSettings.m_Tasks[m_WhichTaskBtn].m_SubButton[i] != TYPE_DYSF && ::NewSettings.m_Tasks[m_WhichTaskBtn].m_SubButton[i] != TYPE_DYSP)
				{
					k = i;
					CString TaskName = ::NewSettings.GetTaskBtnName(m_WhichTaskBtn, i);
					::Info("System will transform to " + TaskName);
					FoundOther = TRUE;
					break;
				}
			}
			if (!FoundOther)
			{
				m_Level = LEVEL_SUMMARY;
				OnBackButtonClicked();
				return;
			}
		}
		else return;
	}


	if (k == 0 && m_Screen == SCREEN_TASKSUB0) return;
	if (k == 1 && m_Screen == SCREEN_TASKSUB1) return;
	if (k == 2 && m_Screen == SCREEN_TASKSUB2) return;
	if (k == 3 && m_Screen == SCREEN_TASKSUB3) return;
	if (k == 4 && m_Screen == SCREEN_TASKSUB4) return;
	if (k == 5 && m_Screen == SCREEN_TASKSUB5) return;
	if (k == 6 && m_Screen == SCREEN_TASKSUB6) return;
	if (k == 7 && m_Screen == SCREEN_TASKSUB7) return;
	if (k == 8 && m_Screen == SCREEN_TASKSUB8) return;

	switch (k)
	{
	case 0: m_Screen = SCREEN_TASKSUB0; break;
	case 1: m_Screen = SCREEN_TASKSUB1; break;
	case 2: m_Screen = SCREEN_TASKSUB2; break;
	case 3: m_Screen = SCREEN_TASKSUB3; break;
	case 4: m_Screen = SCREEN_TASKSUB4; break;
	case 5: m_Screen = SCREEN_TASKSUB5; break;
	case 6: m_Screen = SCREEN_TASKSUB6; break;
	case 7: m_Screen = SCREEN_TASKSUB7; break;
	case 8: m_Screen = SCREEN_TASKSUB8; break;
	}

	// SAFE_DELETE(m_pSumWnd);

	SetDicomStduyDesForToggle();//the 'study des' item in the Dicom file while the summary GUI has been 'OnToggleButtonClicked'

	::TempSettings.CTSumNum = -1;

	HighlightTaskSubButtons();

	Type = ::NewSettings.m_Tasks[m_WhichTaskBtn].m_SubButton[k];
	m_TaskSubK = k;

	BOOL isWFCOM = FALSE;
	BOOL isCTCOM = FALSE;
	m_WFCompareDone = FALSE;
	m_CTCompareDone = FALSE;
	switch (Type)
	{
	case TYPE_NONA:
		OnBackButtonClicked();
		break;

	case TYPE_WCOU:
		OnINSum5ButtonClicked();
		break;

	case TYPE_REFA:
		OnWFSumButtonClicked(1);
		break;
	case TYPE_ABEA:
		OnWFSumButtonClicked(0);
		break;

	case TYPE_WCVA:
		OnINSum3ButtonClicked();
		break;

		//6.2.0 Near Vision
	case TYPE_NEVI:
		OnWFSumButtonClicked(6);
		break;
		//6.2.0 Near Vision

	case TYPE_CTSU:
		OnCTSum1ButtonClicked();
		break;

	case TYPE_VFA:
		OnWFVfaButtonClicked();
		break;

	case TYPE_KERA:
		OnCTSum2ButtonClicked();
		break;

	case TYPE_CHAN:
		OnINSumButtonClicked(0);
		break;

	case TYPE_IOLS:
		OnINSum6ButtonClicked();
		break;

	case TYPE_ANGK:
		OnWFSum3ButtonClicked();
		break;

	case TYPE_TORI:
		OnToricPlannerButtonClicked();
		break;

	case TYPE_ACCO:
		isWFCOM = TRUE;
		OnWFCmpButtonClicked();
		break;

	case TYPE_WFVA: //WF Visual Acuity
		OnWFSumButtonClicked(0);
		break;
	case TYPE_WFRM: //WF/RMS
		OnWFSumButtonClicked(1);
		break;

	case TYPE_CTDS: //CT OD/OS
		OnCTDblButtonClicked();
		break;

	case TYPE_CT3D: //CT 3D Z-Elevation
		OnCTSumButtonClicked(2);
		break;

	case TYPE_WCSU: //WF/CT Summary
		OnINSumButtonClicked(1);
		break;

	case TYPE_CTCM: //CT Compare
		isCTCOM = TRUE;
		OnCTCmpButtonClicked();
		break;

	case TYPE_DYSF: //WF/CT Dysfunctional lens analysis Surgeon
		OnINSumButtonClicked(10);
		break;

	case TYPE_DYSP: //WF/CT Dysfunctional lens analysis Surgeon
		OnINSum7ButtonClicked();
		break;

	case TYPE_ASTI: //WF/CT Astigmastim Source
		if (m_isCombo) OnINSumButtonClicked(7);
		else OnINSum7ButtonClicked();
		break;

	case TYPE_DEPF:
		OnWFSumButtonClicked(3);
		break;

	case TYPE_TOAL:
		OnINSumButtonClicked(12);
		break;
	}


	if ((!m_WFCompareDone && isWFCOM) || (!m_CTCompareDone && isCTCOM))
	{
		switch (m_Ori_TaskSubK)
		{
		case 0: m_Screen = SCREEN_TASKSUB0; break;
		case 1: m_Screen = SCREEN_TASKSUB1; break;
		case 2: m_Screen = SCREEN_TASKSUB2; break;
		case 3: m_Screen = SCREEN_TASKSUB3; break;
		case 4: m_Screen = SCREEN_TASKSUB4; break;
		case 5: m_Screen = SCREEN_TASKSUB5; break;
		case 6: m_Screen = SCREEN_TASKSUB6; break;
		case 7: m_Screen = SCREEN_TASKSUB7; break;
		case 8: m_Screen = SCREEN_TASKSUB8; break;
		}
		HighlightTaskSubButtons();
	}
	else m_Ori_TaskSubK = m_TaskSubK;

	m_FirstTask++;
}

//***************************************************************************************

//the 'study des' item in the Dicom file while the summary GUI has been 'OnToggleButtonClicked'
void CMainWnd::SetDicomStduyDesForToggle()
{
	if (m_TransType != 0)
	{
		if (m_TransType == 12)
		{
			if (m_DicomInfo.m_StudyDes == "OD Wavefront") m_DicomInfo.m_StudyDes = "OD Corneal Topography";
			else m_DicomInfo.m_StudyDes = "OS Corneal Topography";
		}
		else if (m_TransType == 23)
		{
			if (m_DicomInfo.m_StudyDes == "OD Corneal Topography") m_DicomInfo.m_StudyDes = "OD Wavefront and Corneal Topography";
			else m_DicomInfo.m_StudyDes = "OS Wavefront and Corneal Topography";
		}
		else if (m_TransType == 31)
		{
			if (m_DicomInfo.m_StudyDes == "OD Wavefront and Corneal Topography") m_DicomInfo.m_StudyDes = "OD Wavefront";
			else m_DicomInfo.m_StudyDes = "OS Wavefront";
		}
		m_TransType = 0;
	}
}

//***************************************************************************************

void CMainWnd::OnToricPlannerButtonClicked()
{
	CBusyCursor Cursor;

	if (m_Screen == SCREEN_TORIC_PLANNER)
		return;

	m_pCTExams[0]->m_OpData.m_CorneaCycliderAdj = m_pCTExams[0]->m_OpData.m_Ori_CorneaCycliderAdj; // 6.2.0
	m_pCTExams[0]->m_OpData.m_CorneaCycliderAdjCaled = FALSE;

	CWFExam* pWFExam = m_pWFExams[0];
	CCTExam* pCTExam = m_pCTExams[0];

	// if (!pCTExam->m_OpData.m_OK)
	{
		if (pCTExam->m_sk_ok) {
			pCTExam->m_OpData.m_CorneaPreopCylSimK = pCTExam->m_sk_df_dp;
			pCTExam->m_OpData.m_CorneaPreopAxisSimK = pCTExam->m_sk_st_dg;
		}

		CZernikeSurface Surface = pCTExam->m_WfSurface;

		Surface.ChangeRMaxUm(2000.0);

		real_t SphEq, Sph, Cyl;
		int    Axis;
		CWFExam::GetSpheqSphCylAxis(Surface, 0, TRUE, SphEq, Sph, Cyl, Axis);
		Cyl = 0.01 * intRound(100.0 * Cyl);
		pCTExam->m_OpData.m_CorneaPreopCyl = pCTExam->m_OpData.m_CorneaPreopCylWF = Cyl;
		pCTExam->m_OpData.m_CorneaPreopAxis = pCTExam->m_OpData.m_CorneaPreopAxisWF = Axis;

		if (pWFExam) {
			Surface = pWFExam->m_WfSurface;
			Surface.ChangeRMaxUm(__min(2000.0, Surface.m_r_max_um));
			CWFExam::GetSpheqSphCylAxis(Surface, 0, TRUE, SphEq, Sph, Cyl, Axis);
			Cyl = 0.01 * intRound(100.0 * Cyl);
			pCTExam->m_OpData.m_EyePreopCyl = pCTExam->m_OpData.m_EyePreopCylWF = Cyl;
			pCTExam->m_OpData.m_EyePreopAxis = pCTExam->m_OpData.m_EyePreopAxisWF = Axis;
		}

		BOOL OK;

		if (m_isCombo) {
			CString PatientName = G_ReadChinese(m_Patient.m_FirstName) + "." + G_ReadChinese(m_Patient.m_LastName);
			CString PatientAge = GetPatientAge(m_Patient);

			CPreopComboDataDlg* pDlg = new CPreopComboDataDlg(this, &pCTExam->m_OpData, m_OD, PatientName, PatientAge);
			OK = pDlg->DoModal() == IDOK;
			delete pDlg;

			m_pCTExams[0]->m_OpData.m_Ori_CorneaCycliderAdj = m_pCTExams[0]->m_OpData.m_CorneaCycliderAdj; // 6.2.0
		}
		else {
			CPreopDataDlg* pDlg = new CPreopDataDlg(this, &pCTExam->m_OpData, m_OD);
			OK = pDlg->DoModal() == IDOK;
			delete pDlg;
		}

		if (!OK) {
			if (m_WhichTaskBtn != -1) {
				if (m_FirstTask == 0)
					OnBackButtonClicked();
				else {
					m_TaskSubK = m_Ori_TaskSubK;

					if (m_Ori_TaskSubK == 0)
						m_Screen = SCREEN_TASKSUB0;
					if (m_Ori_TaskSubK == 1)
						m_Screen = SCREEN_TASKSUB1;
					if (m_Ori_TaskSubK == 2)
						m_Screen = SCREEN_TASKSUB2;
					if (m_Ori_TaskSubK == 3)
						m_Screen = SCREEN_TASKSUB3;
					if (m_Ori_TaskSubK == 4)
						m_Screen = SCREEN_TASKSUB4;
					if (m_Ori_TaskSubK == 5)
						m_Screen = SCREEN_TASKSUB5;
					if (m_Ori_TaskSubK == 6)
						m_Screen = SCREEN_TASKSUB6;
					if (m_Ori_TaskSubK == 7)
						m_Screen = SCREEN_TASKSUB7;
					if (m_Ori_TaskSubK == 8)
						m_Screen = SCREEN_TASKSUB8;

					HighlightTaskSubButtons();
				}
				return;
			}
			else
				return;
		}
	}

	SAFE_DELETE(m_pSumWnd);

	::TempSettings.CTSumNum = -1;

	m_pSumWnd = new CPlannerWnd(this, m_SumRect, &m_Patient, m_pCTExams[0], &::Settings.m_PlannerSettings, m_OD);

	if (m_BigCTSumWnd != NULL)
		m_BigCTSumWnd->ShowWindow(SW_HIDE);

	if (m_BigPlanWnd != NULL) {
		m_BigPlanWnd->RealseMem();
		SAFE_DELETE(m_BigPlanWnd);
	}
	//[520]

	m_Screen = SCREEN_TORIC_PLANNER;

	HighlightSummaryLevelButtons();
}

//***************************************************************************************

//6.3.0
CString CMainWnd::GetPatientAge(CPatient m_Patient)
{
	CString res;

	SYSTEMTIME CurTime;
	::GetLocalTime(&CurTime);
	int age = CurTime.wYear - m_Patient.m_BirthYear;

	if (CurTime.wMonth < m_Patient.m_BirthMonth)
	{
		age--;
	}
	else if (CurTime.wMonth == m_Patient.m_BirthMonth)
	{
		if (CurTime.wDay < m_Patient.m_BirthDay)
		{
			age--;
		}
	}

	res.Format(_T("%i"), age);

	return res;
}

//***************************************************************************************

void CMainWnd::OnWavetouchButtonClicked()
{
	if (m_Screen == SCREEN_WAVETOUCH) return;

	SAFE_DELETE(m_pSumWnd);

	::TempSettings.CTSumNum = -1;//[520]

	if (m_BigCTSumWnd != NULL)	m_BigCTSumWnd->ShowWindow(SW_HIDE);	//[520]

	m_pSumWnd = new CWTSumWnd(this, m_SumRect, &m_Patient, m_pWFExams[0], m_pCTExams[0], &::Settings.m_WTsmSettings[0]);

	m_Screen = SCREEN_WAVETOUCH;

	HighlightSummaryLevelButtons();
}

//***************************************************************************************

void CMainWnd::OnExportOrderButtonClicked()
{
	//6.3.0 For WTT adjusted GUI
	if (!::DB.IsReadOnly())
	{
		::DB.SaveWFExam(m_pWFExams[0]);
		m_pSumWnd->Repaint();
	}
	else {
		::Error("You can not make changes to the exam because database is opened as read-only.");
		return;
	}

	m_pSumWnd->Repaint();

	Matrix<uchar> Mem;
	m_pSumWnd->SaveIntoMem(Mem, COMPRESSION_PNG);

	::ExportWavetouch(m_pWFExams[0], Mem);
}

//***************************************************************************************

void CMainWnd::OnSaveScreenButtonClicked()
{
	if (m_pWFExams[0])
	{
		//6.2.0
		CString ExamId;
		GUIDToStr(m_pWFExams[0]->m_Header.m_ExamID, ExamId);
		m_DicomInfo.m_Series_Instance_UID = ExamId;
		//6.2.0
	}

	if (!m_pWFExams[0] && m_pCTExams[0])
	{
		//6.2.0
		CString ExamId;
		GUIDToStr(m_pCTExams[0]->m_Header.m_ExamID, ExamId);
		m_DicomInfo.m_Series_Instance_UID = ExamId;
		//6.2.0
	}

	//Study Des value for Dicom file
	if (m_DicomInfo.m_StudyDes == "" && m_NumLoadedWFExams == 0 && m_NumSelectedCTExams == 0)
	{
		CString StudyDateStr, StudyTimeStr;

		if (m_pWFExams[0] && !m_pCTExams[0])
		{
			if (m_pWFExams[0] && m_pWFExams[1])
			{
				if (m_pWFExams[0]->m_Header.IsOD())
				{
					m_DicomInfo.m_StudyDes = "Multi OD Wavefront";
				}
				else
				{
					m_DicomInfo.m_StudyDes = "Multi OS Wavefront";
				}
			}
			else
			{
				if (m_pWFExams[0]->m_Header.IsOD())
				{
					m_DicomInfo.m_StudyDes = "OD Wavefront";
				}
				else
				{
					m_DicomInfo.m_StudyDes = "OS Wavefront";
				}
			}

			int StudyDate = m_pWFExams[0]->m_Header.m_Year * 10000 + m_pWFExams[0]->m_Header.m_Month * 100 + m_pWFExams[0]->m_Header.m_Day;
			int StudyTime = m_pWFExams[0]->m_Header.m_Hour * 10000 + m_pWFExams[0]->m_Header.m_Minute * 100 + m_pWFExams[0]->m_Header.m_Second;

			StudyDateStr.Format(_T("%04i%02i%02i"), m_pWFExams[0]->m_Header.m_Year, m_pWFExams[0]->m_Header.m_Month, m_pWFExams[0]->m_Header.m_Day);
			StudyTimeStr.Format(_T("%02i%02i%02i"), m_pWFExams[0]->m_Header.m_Hour, m_pWFExams[0]->m_Header.m_Minute, m_pWFExams[0]->m_Header.m_Second);

			m_DicomInfo.m_Study_Date = StudyDateStr;
			m_DicomInfo.m_Study_Time = StudyTimeStr;
		}

		else if (!m_pWFExams[0] && m_pCTExams[0])
		{
			if (m_pCTExams[0] && m_pCTExams[1])
			{
				if (m_pCTExams[0]->m_Header.IsOD())
				{
					m_DicomInfo.m_StudyDes = "Multi OD Corneal Topography";
				}
				else
				{
					m_DicomInfo.m_StudyDes = "Multi OS Corneal Topography";
				}
			}
			else
			{
				if (m_pCTExams[0]->m_Header.IsOD())
				{
					m_DicomInfo.m_StudyDes = "OD Corneal Topography";
				}
				else
				{
					m_DicomInfo.m_StudyDes = "OS Corneal Topography";
				}
			}

			int StudyDate = m_pCTExams[0]->m_Header.m_Year * 10000 + m_pCTExams[0]->m_Header.m_Month * 100 + m_pCTExams[0]->m_Header.m_Day;
			int StudyTime = m_pCTExams[0]->m_Header.m_Hour * 10000 + m_pCTExams[0]->m_Header.m_Minute * 100 + m_pCTExams[0]->m_Header.m_Second;

			StudyDateStr.Format(_T("%04i%02i%02i"), m_pWFExams[0]->m_Header.m_Year, m_pWFExams[0]->m_Header.m_Month, m_pWFExams[0]->m_Header.m_Day);
			StudyTimeStr.Format(_T("%02i%02i%02i"), m_pWFExams[0]->m_Header.m_Hour, m_pWFExams[0]->m_Header.m_Minute, m_pWFExams[0]->m_Header.m_Second);

			m_DicomInfo.m_Study_Date = StudyDateStr;
			m_DicomInfo.m_Study_Time = StudyTimeStr;
		}
		else if (m_pWFExams[0] && m_pCTExams[0])
		{

			if (m_pWFExams[0]->m_Header.IsOD())
			{
				m_DicomInfo.m_StudyDes = "OD Wavefront and Corneal Topography";
			}
			else
			{
				m_DicomInfo.m_StudyDes = "OS Wavefront and Corneal Topography";
			}

			int StudyDate = m_pWFExams[0]->m_Header.m_Year * 10000 + m_pWFExams[0]->m_Header.m_Month * 100 + m_pWFExams[0]->m_Header.m_Day;
			int StudyTime = m_pWFExams[0]->m_Header.m_Hour * 10000 + m_pWFExams[0]->m_Header.m_Minute * 100 + m_pWFExams[0]->m_Header.m_Second;

			StudyDateStr.Format(_T("%04i%02i%02i"), m_pWFExams[0]->m_Header.m_Year, m_pWFExams[0]->m_Header.m_Month, m_pWFExams[0]->m_Header.m_Day);
			StudyTimeStr.Format(_T("%02i%02i%02i"), m_pWFExams[0]->m_Header.m_Hour, m_pWFExams[0]->m_Header.m_Minute, m_pWFExams[0]->m_Header.m_Second);

			m_DicomInfo.m_Study_Date = StudyDateStr;
			m_DicomInfo.m_Study_Time = StudyTimeStr;
		}
	}

	if (m_Screen == SCREEN_BIG_TORIC_PLANNER)
		m_BigPlanWnd->SaveIntoFile(m_DicomInfo);
	else if (m_Screen == SCREEN_BIG_CT_OSHER1 || m_Screen == SCREEN_BIG_CT_OSHER2
		|| m_Screen == SCREEN_BIG_CT_OSHER3 || m_Screen == SCREEN_BIG_CT_OSHER4
		|| m_Screen == SCREEN_BIG_CT_OSHER5)
		m_BigCTSumWnd->SaveIntoFile(m_DicomInfo);
	else
	{
		//6.2.1
		if (m_Screen == SCREEN_WFSUM7)
		{
			int a = 0;
			m_pSumWnd->PaintControls();
		}
		//6.2.1
		m_pSumWnd->SaveIntoFile(m_DicomInfo);
	}

	m_DicomInfo.m_StudyDes = "";
}

//***************************************************************************************

void CMainWnd::OnPrintScreenButtonClicked()
{
	if (m_Screen == SCREEN_BIG_TORIC_PLANNER)
	{
		m_BigPlanWnd->Print();
	}
	else if (m_Screen == SCREEN_BIG_CT_OSHER1 || m_Screen == SCREEN_BIG_CT_OSHER2
		|| m_Screen == SCREEN_BIG_CT_OSHER3 || m_Screen == SCREEN_BIG_CT_OSHER4)
	{
		m_BigCTSumWnd->Print();
	}
	else
	{
		m_pSumWnd->Print();
	}
}

//***************************************************************************************

void CMainWnd::OnExportScreenButtonClicked()
{
	CBusyCursor Cursor;

	m_pSumWnd->RepaintMemDC();

	Matrix<uchar> Mem;

	//520
	int w, h;
	uchar* RGBData;
	if (m_Screen == SCREEN_BIG_TORIC_PLANNER)
	{
		m_BigPlanWnd->SaveIntoMem(Mem, ::Settings.m_UseJPGFormat ? COMPRESSION_JPG : COMPRESSION_PNG);
		w = m_BigPlanWnd->m_w;
		h = m_BigPlanWnd->m_h;
		RGBData = m_BigPlanWnd->m_MemDC.m_RGBData;
	}
	else if (m_Screen == SCREEN_BIG_CT_OSHER1 || m_Screen == SCREEN_BIG_CT_OSHER2
		|| m_Screen == SCREEN_BIG_CT_OSHER3 || m_Screen == SCREEN_BIG_CT_OSHER4
		|| m_Screen == SCREEN_BIG_CT_OSHER5)
	{
		m_BigCTSumWnd->SaveIntoMem(Mem, ::Settings.m_UseJPGFormat ? COMPRESSION_JPG : COMPRESSION_PNG);
		w = m_BigCTSumWnd->m_w;
		h = m_BigCTSumWnd->m_h;
		RGBData = m_BigCTSumWnd->m_MemDC.m_RGBData;
	}
	else
	{
		m_pSumWnd->SaveIntoMem(Mem, ::Settings.m_UseJPGFormat ? COMPRESSION_JPG : COMPRESSION_PNG);
		w = m_pSumWnd->m_w;
		h = m_pSumWnd->m_h;
		RGBData = m_pSumWnd->m_MemDC.m_RGBData;
	}
	//520

	//m_pSumWnd->SaveIntoMem(Mem, ::Settings.m_UseJPGFormat ? COMPRESSION_JPG : COMPRESSION_PNG);

	if (m_pWFExams[0] && !m_pCTExams[0])
	{
		//6.2.0
		CString ExamId;
		GUIDToStr(m_pWFExams[0]->m_Header.m_ExamID, ExamId);
		m_DicomInfo.m_Series_Instance_UID = ExamId;
		//6.2.0

		::ExportScreen(m_pWFExams[0], m_pWFExams[1], Mem, m_DicomInfo, RGBData, w, h);//520 Add ', m_DicomInfo'
	}

	else if (!m_pWFExams[0] && m_pCTExams[0])
	{
		//6.2.0
		CString ExamId;
		GUIDToStr(m_pCTExams[0]->m_Header.m_ExamID, ExamId);
		m_DicomInfo.m_Series_Instance_UID = ExamId;
		//6.2.0

		::ExportScreen(m_pCTExams[0], m_pCTExams[1], Mem, m_DicomInfo, RGBData, w, h);//520 Add ', m_DicomInfo'
	}

	else if (m_pWFExams[0] && m_pCTExams[0])
	{
		//6.2.0
		CString ExamId;
		GUIDToStr(m_pWFExams[0]->m_Header.m_ExamID, ExamId);
		m_DicomInfo.m_Series_Instance_UID = ExamId;
		//6.2.0

		::ExportScreen(m_pWFExams[0], m_pCTExams[0], Mem, m_DicomInfo, RGBData, w, h);//520 Add ', m_DicomInfo'
	}


	// if(m_Screen == SCREEN_BIG_TORIC_PLANNER)
	//	m_BigPlanWnd->SaveIntoFile(m_DicomInfo);
	//else if(m_Screen == SCREEN_BIG_CT_OSHER1 || m_Screen ==SCREEN_BIG_CT_OSHER2
	//	 || m_Screen == SCREEN_BIG_CT_OSHER3 || m_Screen ==SCREEN_BIG_CT_OSHER4)
	//       m_BigCTSumWnd->SaveIntoFile(m_DicomInfo);
	//else
	//	m_pSumWnd->SaveIntoFile(m_DicomInfo);

}

//***************************************************************************************

void CMainWnd::OnF9()
{
	if (m_Level != LEVEL_SUMMARY) return;

	CALIBRATION* pCalibration;

	if (m_Screen == SCREEN_WFVRF) {
		pCalibration = &m_pWFExams[0]->m_Calibration;
	}
	else if (m_Screen == SCREEN_CTVRF) {
		pCalibration = &m_pCTExams[0]->m_Calibration;
	}
	else return;

	CCalDlg* pDlg = new CCalDlg(&::HW, pCalibration, this);
	pDlg->DoModal();
	delete pDlg;
}

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

void CMainWnd::ShowWFSummaryLevelButtons()
{
	m_DisScreen = 0;

	CString s, sButton[6];

	m_ToggleButton.SetEdgeColor(CYAN);
	m_ToggleButton.SetImage(m_q == 2 ? IDR_TOGGLE_WFDISPLAYS_3 : m_q == 1 ? IDR_TOGGLE_WFDISPLAYS_2 : IDR_TOGGLE_WFDISPLAYS_1);
	m_ToggleButton.Repaint();

	m_WFVrfButton.SetEdgeColor(CYAN);
	m_WFVfaButton.SetEdgeColor(CYAN);
	m_WFCmpButton.SetEdgeColor(CYAN);

	m_WFVrfButton.ShowWindow(SW_SHOW);
	m_WFVfaButton.ShowWindow(SW_SHOW);
	m_WFCmpButton.ShowWindow(SW_SHOW);

	if (!m_isSolo)
	{
		int Pos = 3;
		if (m_isCombo) Pos = 4;
		for (int i = 0; i <= Pos; i++)
		{
			int pos = i;
			if (i == 4) pos = 3;
			else if (i == 3) pos = 4;

			::Settings.GetButtonName(0, pos, s);

			if (i == 1 && s == "Angle K/A") s = "WF/RMS";

			if (!m_isCombo && s == "Depth of Focus") s = "Custom";

			// m_WFSumButton[i].SetText(s);
			sButton[i] = s;
			m_WFSumButton[i].SetEdgeColor(CYAN);
			m_WFSumButton[i].ShowWindow(SW_SHOW);
		}

		// Change the order and add 'Depth of Focus Comparison'
		if (m_isCombo)
		{
			setlocale(LC_ALL, "chs");

			m_WFSumButton[0].SetText(sButton[0]);//6.2.0 'Visual Acuity'
			m_WFSumButton[1].SetText(sButton[2]);
			m_WFSumButton[2].SetText(sButton[3]);

		   /* m_WFSumButton[0].SetText(sButton[1]);
			m_WFSumButton[1].SetText(sButton[2]);
			m_WFSumButton[2].SetText(sButton[3]);*/

			::NewSettings.GetNewWFBtnName(0, 0, s);
			m_WFSumButton[3].SetText(s);

			m_WFSumButton[4].SetText(sButton[1]);
			m_WFSumButton[5].SetText(sButton[4]);
			m_WFSumButton[6].SetText("Near Vision");//6.2.0 'WF Near Vison'

			m_WFSumButton[5].SetEdgeColor(CYAN);
			m_WFSumButton[5].ShowWindow(SW_SHOW);

			m_WFSumButton[6].SetEdgeColor(CYAN);//6.2.0 'WF Near Vison'
			m_WFSumButton[6].ShowWindow(SW_SHOW);
		}
	}
	// Show the new solo buttons
	else
	{
		LPCRECT prct1, prct2;
		prct1 = &m_BR[0][5];
		prct2 = &m_BR[0][8];

		m_WFSumButton[0].SetText("Aberration Analysis");
		m_WFSumButton[0].ShowWindow(SW_SHOW);

		m_WFSumButton[1].SetText("Refraction Analysis");
		m_WFSumButton[1].ShowWindow(SW_SHOW);

		m_WFSumButton[1].MoveWindow(prct1, 1);
		m_WFVfaButton.MoveWindow(prct2, 1);
	}
	//521 Show the new solo buttons

	//531 Relocate/show/no show the buttons
	if (!m_isSolo)
	{
		int StartPos = 5;
		if (::NewSettings.m_WFSumShow[1])
		{
			m_WFVfaButton.ShowWindow(TRUE);
			StartPos++;
		}
		else m_WFVfaButton.ShowWindow(FALSE);

		if (::NewSettings.m_WFSumShow[2])
		{
			m_WFCmpButton.ShowWindow(TRUE);
			m_WFCmpButton.MoveWindow(&m_BR[0][StartPos], 1);
			StartPos++;
		}
		else m_WFCmpButton.ShowWindow(FALSE);

		//For WF Buttons,
		//m_WFSumButtons[0]:  Visual Acuity
		//m_WFSumButtons[1]: Angle K/A
		//m_WFSumButtons[2]: Depth of Focus
		//m_WFSumButtons[3]: Depth of Focus Comparison
		//m_WFSumButtons[4]: WF/RMS
		//m_WFSumButtons[5]: Custom
		//
		//For ::NewSettings.m_WFSumShow and ::NewSettings.m_WFNewSumShow
		//::NewSettings.m_WFSumShow[0]: Ponit(always be true)
		//::NewSettings.m_WFSumShow[1]: VFA
		//::NewSettings.m_WFSumShow[2]: Comparison
		//::NewSettings.m_WFSumShow[3]: visual/Acuity
		//::NewSettings.m_WFSumShow[4]: WF/RMS
		//::NewSettings.m_WFSumShow[5]: Angle K/A
		//::NewSettings.m_WFSumShow[6]: Depth of Focus
		//::NewSettings.m_WFSumShow[7]: Custom
		//::NewSettings.m_WFNewSumShow[0]: Depth of Focus Comparison
		//::NewSettings.m_WFNewSumShow[1]: WF Near vison

		int Pos = 6;
		if (!m_isCombo)
		{
			for (int i = 3; i <= Pos; i++)
			{
				if (::NewSettings.m_WFSumShow[i])
				{
					m_WFSumButton[i - 3].ShowWindow(TRUE);
					m_WFSumButton[i - 3].MoveWindow(&m_BR[0][StartPos], 1);
					StartPos++;
				}
				else m_WFSumButton[i - 3].ShowWindow(FALSE);
			}
		}
		else
		{
			//6.2.0 WF Near vision
			if (::NewSettings.m_WFNewSumShow[1])
			{
				m_WFSumButton[6].ShowWindow(TRUE);
				m_WFSumButton[6].MoveWindow(&m_BR[0][StartPos], 1);
				StartPos++;
			}
			else m_WFSumButton[6].ShowWindow(FALSE);
			//6.2.0 WF Near vision

			//visual/Acuity
			if (::NewSettings.m_WFSumShow[3])
			{
				m_WFSumButton[0].ShowWindow(TRUE);
				m_WFSumButton[0].MoveWindow(&m_BR[0][StartPos], 1);
				StartPos++;
			}
			else m_WFSumButton[0].ShowWindow(FALSE);
			//visual / Acuity Done

			//Angle K/A
			if (::NewSettings.m_WFSumShow[5])
			{
				m_WFSumButton[1].ShowWindow(TRUE);
				m_WFSumButton[1].MoveWindow(&m_BR[0][StartPos], 1);
				StartPos++;
			}
			else m_WFSumButton[1].ShowWindow(FALSE);
			//Angle K/A Done



			//Depth of Focus
			if (::NewSettings.m_WFSumShow[6])
			{
				m_WFSumButton[2].ShowWindow(TRUE);
				m_WFSumButton[2].MoveWindow(&m_BR[0][StartPos], 1);
				StartPos++;
			}
			else m_WFSumButton[2].ShowWindow(FALSE);
			//Depth of Focus Done

			//Depth of Focus Comparison
			if (::NewSettings.m_WFNewSumShow[0])
			{
				m_WFSumButton[3].ShowWindow(TRUE);
				m_WFSumButton[3].MoveWindow(&m_BR[0][StartPos], 1);
				StartPos++;
			}
			else m_WFSumButton[3].ShowWindow(FALSE);
			//Depth of Focus Done Comparison




			//WF/RMS
			if (::NewSettings.m_WFSumShow[4])
			{
				m_WFSumButton[4].ShowWindow(TRUE);
				m_WFSumButton[4].MoveWindow(&m_BR[0][StartPos], 1);
				StartPos++;
			}
			else m_WFSumButton[4].ShowWindow(FALSE);
			//WF/RMS Done

			//Custom
			if (::NewSettings.m_WFSumShow[7])
			{
				m_WFSumButton[5].ShowWindow(TRUE);
				m_WFSumButton[5].MoveWindow(&m_BR[0][StartPos], 1);
				StartPos++;
			}
			else m_WFSumButton[5].ShowWindow(FALSE);
			//Custom Done
		}

		for (int i = 0; i <= 5; i++)
		{
			m_WFSumButton[i].Repaint();
		}
	}
}

//---------------------------------------------------------------------------------------

void CMainWnd::HideWFSummaryLevelButtons()
{
	m_WFVrfButton.ShowWindow(SW_HIDE);
	m_WFVfaButton.ShowWindow(SW_HIDE);
	m_WFCmpButton.ShowWindow(SW_HIDE);

	m_WFSumButton[0].ShowWindow(SW_HIDE);
	m_WFSumButton[1].ShowWindow(SW_HIDE);
	m_WFSumButton[2].ShowWindow(SW_HIDE);
	m_WFSumButton[3].ShowWindow(SW_HIDE);

	if (m_isCombo)
	{
		m_WFSumButton[4].ShowWindow(SW_HIDE);
		m_WFSumButton[5].ShowWindow(SW_HIDE);
		m_WFSumButton[6].ShowWindow(SW_HIDE);//6.2.0 WF Near vision
	}
}

//***************************************************************************************

void CMainWnd::ShowCTSummaryLevelButtons()
{
	m_DisScreen = 1;//531

	CString s;

	//530
	m_ToggleButton.SetEdgeColor(GREEN);
	m_ToggleButton.SetImage(m_q == 2 ? IDR_TOGGLE_CTDISPLAYS_3 : m_q == 1 ? IDR_TOGGLE_CTDISPLAYS_2 : IDR_TOGGLE_CTDISPLAYS_1);
	m_ToggleButton.Repaint();
	//530

	//530
	m_CTVrfButton.SetEdgeColor(GREEN);
	m_CTDblButton.SetEdgeColor(GREEN);
	m_CTCmpButton.SetEdgeColor(GREEN);
	//Done

	m_CTVrfButton.ShowWindow(SW_SHOW);
	m_CTDblButton.ShowWindow(SW_SHOW);
	m_CTCmpButton.ShowWindow(SW_SHOW);

	for (int i = 0; i < 4; i++)
	{
		//[520]for Osher alignment button in the combo version
		int pos = i;

		if (m_isCombo && i == 3) pos = 4;

		//[520]

		::Settings.GetButtonName(1, pos, s);
		m_CTSumButton[i].SetText(s);

		m_CTSumButton[i].SetEdgeColor(GREEN);

		m_CTSumButton[i].ShowWindow(SW_SHOW);
	}

	//531
	if (m_isCombo)
	{
		::Settings.GetButtonName(1, 3, s);
		m_CTSumButton[4].SetText(s);

		m_CTSumButton[4].SetEdgeColor(GREEN);

		m_CTSumButton[4].ShowWindow(SW_SHOW);
	}
	//531 Done

	//531 Relocate/show/no show the buttons
	if (!m_isSolo)
	{
		int StartPos = 5;
		if (::NewSettings.m_CTSumShow[1])
		{
			m_CTDblButton.ShowWindow(TRUE);
			StartPos++;
		}
		else m_CTDblButton.ShowWindow(FALSE);

		if (::NewSettings.m_CTSumShow[2])
		{
			m_CTCmpButton.ShowWindow(TRUE);
			m_CTCmpButton.MoveWindow(&m_BR[0][StartPos], 1);
			StartPos++;
		}
		else m_CTCmpButton.ShowWindow(FALSE);

		int Num = 7;

		if (m_isCombo) Num = 8;

		for (int i = 3; i < Num; i++)
		{
			if (::NewSettings.m_CTSumShow[i])
			{
				m_CTSumButton[i - 3].ShowWindow(TRUE);
				m_CTSumButton[i - 3].MoveWindow(&m_BR[0][StartPos], 1);
				StartPos++;
			}
			else m_CTSumButton[i - 3].ShowWindow(FALSE);
		}
	}
	//531 Done
}

//---------------------------------------------------------------------------------------

void CMainWnd::HideCTSummaryLevelButtons()
{
	m_CTVrfButton.ShowWindow(SW_HIDE);
	m_CTDblButton.ShowWindow(SW_HIDE);
	m_CTCmpButton.ShowWindow(SW_HIDE);

	m_CTSumButton[0].ShowWindow(SW_HIDE);
	m_CTSumButton[1].ShowWindow(SW_HIDE);
	m_CTSumButton[2].ShowWindow(SW_HIDE);
	m_CTSumButton[3].ShowWindow(SW_HIDE);
	if (m_isCombo) m_CTSumButton[4].ShowWindow(SW_HIDE);
}

//***************************************************************************************

void CMainWnd::ShowINSummaryLevelButtons()
{
	m_DisScreen = 2;//531

					//530
	m_ToggleButton.SetEdgeColor(YELLOW);
	m_ToggleButton.SetImage(m_q == 2 ? IDR_TOGGLE_WFCTDIS_3 : m_q == 1 ? IDR_TOGGLE_WFCTDIS_2 : IDR_TOGGLE_WFCTDIS_1);
	m_ToggleButton.Repaint();
	//530

	CString s;

	if (!m_isCombo)
	{
		for (int i = 0; i <= 6; i++)
		{
			::Settings.GetButtonName(2, i, s);
			m_INSumButton[i].SetText(s);

			m_INSumButton[i].SetEdgeColor(YELLOW);//530
			m_INSumButton[i].ShowWindow(SW_SHOW);
		}

		m_INSumButton[4].SetEdgeColor(YELLOW);//530
		m_INSumButton[4].SetText("OU Overview");
		m_INSumButton[4].Repaint();

		m_INSumButton[5].SetEdgeColor(YELLOW);//530
		m_INSumButton[5].SetText("IOL Selection Analysis");
		m_INSumButton[5].Repaint();

		//530
		::Settings.GetButtonName(2, 4, s);
		m_INSumButton[6].SetEdgeColor(YELLOW);
		m_INSumButton[6].SetText(s);
		m_INSumButton[6].Repaint();
		//530
	}
	else
	{
		int pos;
		for (int i = 0; i <= 10; i++)
		{
			pos = i;

			if (i == 8) pos = 3;
			if (i == 7) pos = 4;
			if (i == 9) pos = 7;// WFCT Angle K/A

			if (i != 10) ::Settings.GetButtonName(2, pos, s);

			if (i == 3) s = "OU Overview";
			if (i == 4) s = "IOL Selection Analysis";
			if (i == 5) s = "Dysfunctional Lens - MD";//shao astigmatism source in setting abc
			if (i == 6) s = "Dysfunctional Lens - PT";
			if (i == 10) s = "Toric Check";;//WFCT Toric Alignment

			m_INSumButton[i].SetText(s);

			m_INSumButton[i].SetEdgeColor(YELLOW);
			m_INSumButton[i].ShowWindow(SW_SHOW);
		}
	}

	m_ToricPlannerButton.SetText("Toric Planner");


	// Relocate/show/no show the buttons
	int StartPos = 4;
	int num = 6;
	if (m_isCombo) num = 8;
	for (int i = 0; i <= num; i++)
	{
		// WFCT Angle K/A
		if (i == 5)// the pos of WFCT Angle K/A
		{
			if (::NewSettings.m_WFCTNewSumShow[0])
			{
				m_INSumButton[9].MoveWindow(&m_BR[0][StartPos], 1);
				m_INSumButton[9].ShowWindow(TRUE);
				//m_INSumButton[9].EnableWindow(FALSE);
				StartPos++;
			}
			else
			{
				m_INSumButton[9].ShowWindow(FALSE);
			}
		}
		// WFCT Angle K/A

		// WFCT Toric Alignement
		if (i == 8 && ::NewSettings.m_WFCTNewSumShow[1])
		{
			m_INSumButton[10].MoveWindow(&m_BR[0][StartPos], 1);
			m_INSumButton[10].ShowWindow(TRUE);
			StartPos++;
		}
		else
		{
			m_INSumButton[10].ShowWindow(FALSE);
		}
		// WFCT Angle K/A


		if (::NewSettings.m_WFCTSumShow[i])
		{


			m_INSumButton[i].MoveWindow(&m_BR[0][StartPos], 1);
			m_INSumButton[i].ShowWindow(TRUE);
			StartPos++;
		}
		else m_INSumButton[i].ShowWindow(FALSE);
	}
	//531 Done
}

//---------------------------------------------------------------------------------------

void CMainWnd::HideINSummaryLevelButtons()
{
	m_INSumButton[0].ShowWindow(SW_HIDE);
	m_INSumButton[1].ShowWindow(SW_HIDE);
	m_INSumButton[2].ShowWindow(SW_HIDE);
	m_INSumButton[3].ShowWindow(SW_HIDE);
	m_INSumButton[4].ShowWindow(SW_HIDE);//[520]
	m_INSumButton[5].ShowWindow(SW_HIDE);//[530]
	m_INSumButton[6].ShowWindow(SW_HIDE);//[530]

	if (m_isCombo)
	{
		m_INSumButton[7].ShowWindow(SW_HIDE);//[531]
		m_INSumButton[8].ShowWindow(SW_HIDE);//[531]
		m_INSumButton[9].ShowWindow(SW_HIDE);// WFCT Angle K/A
		m_INSumButton[10].ShowWindow(SW_HIDE);//WFCT Toric Alignment
	}
}

//***************************************************************************************

void CMainWnd::ShowCommonSummaryLevelButtons()
{
	BOOL IsHardwareOK = ::HW.IsConnected() && ::HW.HasCalibrationLoaded();
	BOOL IsDBReadOnly = ::DB.IsReadOnly();

	if (IsHardwareOK)
	{
		if (!::HW.ISVideoCameraOn)
		{
			HW.TurnVideoCameraOn();
			::HW.ISVideoCameraOn = true;
		}
	}
	else
	{
		::HW.ISVideoCameraOn = false;
	}

	CString s;

	m_NewWFExamButton.SetEnabled(!IsDBReadOnly && IsHardwareOK);
	m_NewWFExamButton.ShowWindow(SW_SHOW);

	if (!m_isSolo)
	{
		m_NewCTExamButton.SetEnabled(!IsDBReadOnly && IsHardwareOK);
		m_NewCTExamButton.ShowWindow(SW_SHOW);
		m_NewWFCTExamButton.SetEnabled(!IsDBReadOnly && IsHardwareOK);
		m_NewWFCTExamButton.ShowWindow(SW_SHOW);
	}

	if (!m_isSolo)
	{
		m_ToggleButton.SetEnabled(m_EnableToggleButton);
		m_ToggleButton.ShowWindow(SW_SHOW);

		m_ToricPlannerButton.SetEnabled(m_EnablePlannerButton);
		m_ToricPlannerButton.ShowWindow(SW_SHOW);
	}

	if (m_isDemo)
	{
		m_NewWFExamButton.SetEnabled(FALSE);
		m_NewCTExamButton.Repaint();
		m_NewCTExamButton.SetEnabled(FALSE);
		m_NewCTExamButton.Repaint();
		m_NewWFCTExamButton.SetEnabled(FALSE);
		m_NewWFCTExamButton.Repaint();
	}
	if (m_isViewer)
	{
		m_NewWFExamButton.ShowWindow(FALSE);
		m_NewCTExamButton.ShowWindow(FALSE);
		m_NewWFCTExamButton.ShowWindow(FALSE);
	}

	if (::Settings.m_ShowWavetouch)
	{
		m_WavetouchButton.SetEnabled(m_EnableWavetouchButtons);
		m_WavetouchButton.ShowWindow(SW_SHOW);

		m_ExportOrderButton.SetEnabled(m_EnableWavetouchButtons && m_Screen == SCREEN_WAVETOUCH);
		m_ExportOrderButton.ShowWindow(SW_SHOW);
	}

	RECT R = m_BR[1][13];
	m_EditExamButton.SetWindowPos(NULL, R.left, R.top, R.right - R.left, R.bottom - R.top, SWP_NOZORDER);
	m_EditExamButton.SetEnabled(m_EnableEditExamButton);
	m_EditExamButton.ShowWindow(SW_SHOW);

	if (!m_isSolo)
	{
		m_ExportScreenButton.SetEnabled(m_EnableExportDataButton);
		m_ExportScreenButton.ShowWindow(SW_SHOW);
	}

	m_SaveScreenButton.ShowWindow(SW_SHOW);
	m_PrintScreenButton.ShowWindow(SW_SHOW);

	m_BackButton.ShowWindow(SW_SHOW);
}


//***************************************************************************************

void CMainWnd::HideTaskButtons()
{
	for (int i = 0; i < 8; i++) m_TaskButton[i].ShowWindow(SW_HIDE);
}

//***************************************************************************************

void CMainWnd::ShowTaskButtons(int TaskNum)
{
	BOOL IsHardwareOK = ::HW.IsConnected() && ::HW.HasCalibrationLoaded();
	BOOL IsDBReadOnly = ::DB.IsReadOnly();


	if (IsHardwareOK)
	{
		if (!::HW.ISVideoCameraOn)
		{
			HW.TurnVideoCameraOn();
			::HW.ISVideoCameraOn = true;
		}
	}
	else ::HW.ISVideoCameraOn = false;

	CString s;

	m_NewWFExamButton.SetEnabled(!IsDBReadOnly && IsHardwareOK);
	m_NewWFExamButton.ShowWindow(SW_SHOW);

	if (!m_isSolo)
	{
		m_NewCTExamButton.SetEnabled(!IsDBReadOnly && IsHardwareOK);
		m_NewCTExamButton.ShowWindow(SW_SHOW);
		m_NewWFCTExamButton.SetEnabled(!IsDBReadOnly && IsHardwareOK);
		m_NewWFCTExamButton.ShowWindow(SW_SHOW);
	}

	if (m_isDemo)
	{
		m_NewWFExamButton.SetEnabled(FALSE);
		m_NewCTExamButton.Repaint();
		m_NewCTExamButton.SetEnabled(FALSE);
		m_NewCTExamButton.Repaint();
		m_NewWFCTExamButton.SetEnabled(FALSE);
		m_NewWFCTExamButton.Repaint();
	}
	if (m_isViewer)
	{
		m_NewWFExamButton.ShowWindow(FALSE);
		m_NewCTExamButton.ShowWindow(FALSE);
		m_NewWFCTExamButton.ShowWindow(FALSE);
	}

	m_ToggleButton.ShowWindow(SW_HIDE);
	m_ToricPlannerButton.ShowWindow(SW_HIDE);


	//Task buttons
	for (int i = 0; i < 8; i++)   m_TaskButton[i].MoveWindow(&m_BR[0][i + 4], 1);

	int n = ::NewSettings.m_Tasks[TaskNum].m_SubBtnNum;
	for (int i = 0; i < 8; i++)
	{
		if (i < n)
		{
			CString BtnName = ::NewSettings.GetTaskBtnName(TaskNum, i);

			{
				m_TaskButton[i].ShowWindow(SW_SHOW);
				m_TaskButton[i].SetText(BtnName);

				//531
				int BtnType = ::NewSettings.GetTaskBtnType(TaskNum, i);

				COLORREF Color;
				if (BtnType == TYPE_WCOU || BtnType == TYPE_WCVA
					|| BtnType == TYPE_CHAN || BtnType == TYPE_IOLS
					|| BtnType == TYPE_WCSU || BtnType == TYPE_DYSF
					|| BtnType == TYPE_ASTI || BtnType == TYPE_DYSP
					|| BtnType == TYPE_DEPF || BtnType == TYPE_TOAL
					|| BtnType == TYPE_ANGK)
					Color = YELLOW;//WFCT

				else if (BtnType == TYPE_REFA || BtnType == TYPE_ABEA
					|| BtnType == TYPE_VFA || BtnType == TYPE_WFVA
					|| BtnType == TYPE_WFRM || BtnType == TYPE_ACCO)
					Color = CYAN; //WF

				else if (BtnType == TYPE_CTSU || BtnType == TYPE_CTDS
					|| BtnType == TYPE_CT3D || BtnType == TYPE_CTCM
					|| BtnType == TYPE_KERA)
					Color = GREEN;//ct

				else if (BtnType == TYPE_TORI)
					Color = RED;

				else Color = BLACK;

				m_TaskButton[i].SetEdgeColor(Color);

				m_TaskButton[i].Repaint();
			}
		}
		else m_TaskButton[i].ShowWindow(SW_HIDE);
	}
	Invalidate(FALSE);
	//Task Buttons Done

	if (::Settings.m_ShowWavetouch)
	{
		m_WavetouchButton.SetEnabled(m_EnableWavetouchButtons);
		m_WavetouchButton.ShowWindow(SW_SHOW);

		m_ExportOrderButton.SetEnabled(m_EnableWavetouchButtons && m_Screen == SCREEN_WAVETOUCH);
		m_ExportOrderButton.ShowWindow(SW_SHOW);
	}

	RECT R = m_BR[1][13];
	m_EditExamButton.SetWindowPos(NULL, R.left, R.top, R.right - R.left, R.bottom - R.top, SWP_NOZORDER);
	m_EditExamButton.SetEnabled(m_EnableEditExamButton);
	m_EditExamButton.ShowWindow(SW_SHOW);

	if (!m_isSolo)
	{
		m_ExportScreenButton.SetEnabled(m_EnableExportDataButton);
		m_ExportScreenButton.ShowWindow(SW_SHOW);
	}

	m_SaveScreenButton.ShowWindow(SW_SHOW);
	m_PrintScreenButton.ShowWindow(SW_SHOW);

	m_BackButton.ShowWindow(SW_SHOW);
}

//---------------------------------------------------------------------------------------

void CMainWnd::HideCommonSummaryLevelButtons()
{
	m_AcceptExamButton.ShowWindow(SW_HIDE);
	m_RejectExamButton.ShowWindow(SW_HIDE);

	m_NewWFExamButton.ShowWindow(SW_HIDE);
	m_NewCTExamButton.ShowWindow(SW_HIDE);
	m_NewWFCTExamButton.ShowWindow(SW_HIDE);

	m_ToggleButton.ShowWindow(SW_HIDE);

	m_ToricPlannerButton.ShowWindow(SW_HIDE);

	m_WavetouchButton.ShowWindow(SW_HIDE);
	m_ExportOrderButton.ShowWindow(SW_HIDE);

	m_EditExamButton.ShowWindow(SW_HIDE);
	m_ExportScreenButton.ShowWindow(SW_HIDE);

	m_SaveScreenButton.ShowWindow(SW_HIDE);
	m_PrintScreenButton.ShowWindow(SW_HIDE);

	m_BackButton.ShowWindow(SW_HIDE);
}

//***************************************************************************************

void CMainWnd::HideAllSummaryLevelButtons()
{
	HideCommonSummaryLevelButtons();
	HideWFSummaryLevelButtons();
	HideCTSummaryLevelButtons();
	HideINSummaryLevelButtons();
	HideTaskButtons();//530
}

//***************************************************************************************

void CMainWnd::HighlightSummaryLevelButtons()
{
	m_WFVrfButton.SetHighlighted(m_Screen == SCREEN_WFVRF);
	m_WFVfaButton.SetHighlighted(m_Screen == SCREEN_WFVFA);
	m_WFCmpButton.SetHighlighted(m_Screen == SCREEN_WFCMP);

	m_CTVrfButton.SetHighlighted(m_Screen == SCREEN_CTVRF);
	m_CTDblButton.SetHighlighted(m_Screen == SCREEN_CTDBL);
	m_CTCmpButton.SetHighlighted(m_Screen == SCREEN_CTCMP);

	m_WFSumButton[0].SetHighlighted(m_Screen == SCREEN_WFSUM1);
	m_WFSumButton[1].SetHighlighted(m_Screen == SCREEN_WFSUM2);
	m_WFSumButton[2].SetHighlighted(m_Screen == SCREEN_WFSUM3);
	m_WFSumButton[3].SetHighlighted(m_Screen == SCREEN_WFSUM4);
	m_WFSumButton[4].SetHighlighted(m_Screen == SCREEN_WFSUM5);
	m_WFSumButton[5].SetHighlighted(m_Screen == SCREEN_WFSUM6);
	m_WFSumButton[6].SetHighlighted(m_Screen == SCREEN_WFSUM7);//6.2.0 WF Near vision

	m_CTSumButton[0].SetHighlighted(m_Screen == SCREEN_CTSUM1);
	m_CTSumButton[1].SetHighlighted(m_Screen == SCREEN_CTSUM2);
	m_CTSumButton[2].SetHighlighted(m_Screen == SCREEN_CTSUM3);

	m_CTSumButton[3].SetHighlighted(m_Screen == SCREEN_CTSUM4);
	m_CTSumButton[4].SetHighlighted(m_Screen == SCREEN_CTSUM5);//531

	if (!m_isCombo)
	{
		m_INSumButton[0].SetHighlighted(m_Screen == SCREEN_INSUM1);
		m_INSumButton[1].SetHighlighted(m_Screen == SCREEN_INSUM2);
		m_INSumButton[2].SetHighlighted(m_Screen == SCREEN_INSUM3);
		m_INSumButton[3].SetHighlighted(m_Screen == SCREEN_INSUM4);
		m_INSumButton[4].SetHighlighted(m_Screen == SCREEN_INSUM5);//520 OU overview
		m_INSumButton[5].SetHighlighted(m_Screen == SCREEN_INSUM6);//530
		m_INSumButton[6].SetHighlighted(m_Screen == SCREEN_INSUM7);//530
		m_INSumButton[7].SetHighlighted(m_Screen == SCREEN_INSUM8);//531
		m_INSumButton[8].SetHighlighted(m_Screen == SCREEN_INSUM9);//531
	}
	else
	{
		m_INSumButton[0].SetHighlighted(m_Screen == SCREEN_INSUM1);
		m_INSumButton[1].SetHighlighted(m_Screen == SCREEN_INSUM2);
		m_INSumButton[2].SetHighlighted(m_Screen == SCREEN_INSUM3);

		m_INSumButton[3].SetHighlighted(m_Screen == SCREEN_INSUM5);//OU overview
		m_INSumButton[4].SetHighlighted(m_Screen == SCREEN_INSUM6);//IOL Selection

		m_INSumButton[5].SetHighlighted(m_Screen == SCREEN_INSUM7);
		m_INSumButton[6].SetHighlighted(m_Screen == SCREEN_INSUM8);
		m_INSumButton[7].SetHighlighted(m_Screen == SCREEN_INSUM9);

		m_INSumButton[8].SetHighlighted(m_Screen == SCREEN_INSUM10);// Custom

		m_INSumButton[9].SetHighlighted(m_Screen == SCREEN_INSUM11);//WFCT Angle k/a

		m_INSumButton[10].SetHighlighted(m_Screen == SCREEN_INSUM12);// WFCT Toric Alignment
	}

	m_ToricPlannerButton.SetHighlighted(m_Screen == SCREEN_TORIC_PLANNER);

	m_ExportOrderButton.SetEnabled(m_Screen == SCREEN_WAVETOUCH && m_pWFExams[0] != NULL);
	m_WavetouchButton.SetHighlighted(m_Screen == SCREEN_WAVETOUCH);

	m_WFVrfButton.Repaint();
	m_WFVfaButton.Repaint();
	m_WFCmpButton.Repaint();

	m_CTVrfButton.Repaint();
	m_CTDblButton.Repaint();
	m_CTCmpButton.Repaint();

	for (int i = 0; i < 4; i++)
	{
		m_WFSumButton[i].Repaint();
		m_CTSumButton[i].Repaint();
		m_INSumButton[i].Repaint();
	}

	if (m_isCombo)
	{
		m_WFSumButton[4].Repaint();
		m_WFSumButton[5].Repaint();
		m_WFSumButton[6].Repaint();//6.2.0
		m_CTSumButton[4].Repaint();
		m_INSumButton[7].Repaint();
		m_INSumButton[8].Repaint();
		m_INSumButton[9].Repaint();// WFCT Angle k/a
		m_INSumButton[10].Repaint();// WFCT Toric Alignment
	}

	m_INSumButton[4].Repaint();
	m_INSumButton[5].Repaint();
	m_INSumButton[6].Repaint();


	m_ToricPlannerButton.Repaint();

	m_ExportOrderButton.Repaint();
	m_WavetouchButton.Repaint();
}

//***************************************************************************************

void CMainWnd::HighlightTaskSubButtons()
{
	m_TaskButton[0].SetHighlighted(m_Screen == SCREEN_TASKSUB0);
	m_TaskButton[1].SetHighlighted(m_Screen == SCREEN_TASKSUB1);
	m_TaskButton[2].SetHighlighted(m_Screen == SCREEN_TASKSUB2);
	m_TaskButton[3].SetHighlighted(m_Screen == SCREEN_TASKSUB3);
	m_TaskButton[4].SetHighlighted(m_Screen == SCREEN_TASKSUB4);
	m_TaskButton[5].SetHighlighted(m_Screen == SCREEN_TASKSUB5);
	m_TaskButton[6].SetHighlighted(m_Screen == SCREEN_TASKSUB6);
	m_TaskButton[7].SetHighlighted(m_Screen == SCREEN_TASKSUB7);
	m_TaskButton[8].SetHighlighted(m_Screen == SCREEN_TASKSUB8);

	for (int i = 0; i < 9; i++)
	{
		m_TaskButton[i].Repaint();
	}
}

//***************************************************************************************

LRESULT CMainWnd::OnBigPlanWndShown(WPARAM wParam, LPARAM lParam)
{
	if (m_Screen == SCREEN_BIG_TORIC_PLANNER)
	{
		m_Screen = SCREEN_TORIC_PLANNER;
		m_BigPlanWnd->ShowWindow(SW_HIDE);
		m_pSumWnd->Repaint();
		m_pSumWnd->ShowWindow(SW_SHOW);

		ShowCommonSummaryLevelButtons();

		if (m_SummaryType == 1)
		{
			ShowCTSummaryLevelButtons();
		}

		else if (m_SummaryType == 2)
		{
			ShowINSummaryLevelButtons();
		}

		else if (m_SummaryType == 0)
		{
			ShowWFSummaryLevelButtons();
		}

		else if (m_SummaryType == 3)
		{
			ShowTaskButtons(m_WhichTaskBtn);
		}
	}
	else if (m_Screen == SCREEN_TORIC_PLANNER)
	{
		if (m_BigPlanWnd != NULL)
		{
			m_BigPlanWnd->RealseMem();
			SAFE_DELETE(m_BigPlanWnd);
		}

		m_BigPlanWnd = new CPlanBigWnd(this, m_BigRect, m_DicomInfo, m_pCTExams[0]);
		m_Screen = SCREEN_BIG_TORIC_PLANNER;

		if (m_WhichTaskBtn != -1)
		{
			m_SummaryType = 3;
		}
		else
		{
			if (m_WFVfaButton.IsWindowVisible())
			{
				m_SummaryType = 0;
			}

			else if (m_CTDblButton.IsWindowVisible())
			{
				m_SummaryType = 1;
			}

			else if (m_INSumButton[1].IsWindowVisible())
			{
				m_SummaryType = 2;
			}
		}

		HideAllSummaryLevelButtons();
		m_pSumWnd->ShowWindow(SW_HIDE);
		m_BigPlanWnd->Repaint();
		m_BigPlanWnd->ShowWindow(SW_SHOW);
	}

	//6.2.0 message from toric check
	else if (m_Screen == SCREEN_INSUM12)
	{
		if (m_BigPlanWnd != NULL)
		{
			m_BigPlanWnd->RealseMem();
			SAFE_DELETE(m_BigPlanWnd);
		}

		m_BigPlanWnd = new CPlanBigWnd(this, m_BigRect, m_DicomInfo, m_pCTExams[0]);
		m_Screen = SCREEN_BIG_TORIC_ICL_PLANNER;

		if (m_WhichTaskBtn != -1)
		{
			m_SummaryType = 3;
		}
		else
		{
			m_SummaryType = 2;
		}

		HideAllSummaryLevelButtons();
		m_pSumWnd->ShowWindow(SW_HIDE);
		m_BigPlanWnd->Repaint();
		m_BigPlanWnd->ShowWindow(SW_SHOW);
	}

	else if (m_Screen == SCREEN_BIG_TORIC_ICL_PLANNER)
	{
		m_Screen = SCREEN_INSUM12;
		m_BigPlanWnd->ShowWindow(SW_HIDE);
		m_pSumWnd->Repaint();
		m_pSumWnd->ShowWindow(SW_SHOW);

		ShowCommonSummaryLevelButtons();

		if (m_SummaryType == 2)
		{
			ShowINSummaryLevelButtons();
		}

		else if (m_SummaryType == 3)
		{
			ShowTaskButtons(m_WhichTaskBtn);
		}
	}

	//6.2.0 message from toric check

	return 0;
}

//***************************************************************************************

LRESULT CMainWnd::OnCTOSHERALGWndShown(WPARAM wParam, LPARAM lParam)
{
	m_pSumWnd->ShowWindow(SW_HIDE);

	if (m_Screen == SCREEN_CTSUM1)      m_Screen = SCREEN_BIG_CT_OSHER1;
	else if (m_Screen == SCREEN_CTSUM2) m_Screen = SCREEN_BIG_CT_OSHER2;
	else if (m_Screen == SCREEN_CTSUM3) m_Screen = SCREEN_BIG_CT_OSHER3;
	else if (m_Screen == SCREEN_CTSUM4) m_Screen = SCREEN_BIG_CT_OSHER4;
	else if (m_Screen == SCREEN_CTSUM5) m_Screen = SCREEN_BIG_CT_OSHER5;

	::TempSettings.CTSumNum = ::TempSettings.CTSumNum;

	//if (m_BigCTSumWnd == NULL)
	{
		SAFE_DELETE(m_BigCTSumWnd);

		if (::TempSettings.CTSumNum == 3)
		{
			m_BigCTSumWnd = new CCTOsherAliWnd(this, m_SumRect, m_pCTExams[0], TRUE);//6.2.0
		}
		else
		{
			m_BigCTSumWnd = new CCTOsherAliWnd(this, m_SumRect, m_pCTExams[0], FALSE);
		}
	}


	/*if (::NewSettings.m_OSHER_ALIG_CUS[::TempSettings.CTSumNum] == 1)
	{
		m_BigCTSumWnd->m_ThisMain = TRUE;
	}
	else
	{
		m_BigCTSumWnd->m_ThisMain = FALSE;
	}*/

	//m_BigCTSumWnd->m_d = wParam;
	m_BigCTSumWnd->m_d = wParam;
	m_BigCTSumWnd->m_Screen = m_Screen;	//521
	m_BigCTSumWnd->CreateMenu();//521
	m_BigCTSumWnd->ShowWindow(SW_SHOW);

	return 0;
}

//***************************************************************************************

LRESULT CMainWnd::OnCTOSHERALGWndHide(WPARAM wParam, LPARAM lParam)
{
	m_BigCTSumWnd->ShowWindow(SW_HIDE);

	int k = 0;
	if (m_Screen == SCREEN_BIG_CT_OSHER1)
	{
		m_Screen = SCREEN_CTSUM1;
		::NewSettings.m_OSHER_ALIG_CUS[0] = 0;
	}
	else if (m_Screen == SCREEN_BIG_CT_OSHER2)
	{
		m_Screen = SCREEN_CTSUM2;
		::NewSettings.m_OSHER_ALIG_CUS[1] = 0;
		k = 1;
	}
	else if (m_Screen == SCREEN_BIG_CT_OSHER3)
	{
		::NewSettings.m_OSHER_ALIG_CUS[2] = 0;
		m_Screen = SCREEN_CTSUM3;
		k = 2;
	}
	else if (m_Screen == SCREEN_BIG_CT_OSHER4)
	{
		m_Screen = SCREEN_CTSUM4;
		::NewSettings.m_OSHER_ALIG_CUS[3] = 0;
		k = 3;//k= 3
	}
	else if (m_Screen == SCREEN_BIG_CT_OSHER5)
	{
		m_Screen = SCREEN_CTSUM5;
		//::NewSettings.m_OSHER_ALIG_CUS[3] = 0;//Bug
		k = 4;
	}

	SAFE_DELETE(m_pSumWnd);

	// return to default
	if (wParam == -1)
	{
		if (m_Screen == SCREEN_CTSUM1)//CT Summary
		{
			::Settings.m_CSsmSettings[0][0].m_Type = 31; //Axial map       TYPE_CAXM
			::Settings.m_CSsmSettings[0][1].m_Type = TYPE_CLTR;
			::Settings.m_CSsmSettings[0][2].m_Type = TYPE_CRMS;
			::Settings.m_CSsmSettings[0][3].m_Type = TYPE_CEYE;

			::Settings.m_CSsmSettings[0][0].m_MapShowKeratometry = TRUE;

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
		}
		else if (m_Screen == SCREEN_CTSUM2)//CT Keratometry
		{
			::Settings.m_CSsmSettings[1][0].m_Type = 51; //Kerotametry map       TYPE_CSKM
			::Settings.m_CSsmSettings[1][0].m_SizeSmall = FALSE;
		}
		else if (m_Screen == SCREEN_CTSUM3)//CT 3D-Z Elevation
		{
			::Settings.m_CSsmSettings[2][0].m_Type = 34; //z elevation     TYPE_CELM
			::Settings.m_CSsmSettings[2][0].m_SizeSmall = FALSE;
			::Settings.m_CSsmSettings[2][0].m_Method3D = 1;
			::Settings.m_CSsmSettings[2][0].m_MapShowSolidSurface = TRUE;
			::Settings.m_CSsmSettings[2][0].m_MapShowWireMesh = TRUE;
			CScale* pScale3 = ::Settings.m_CSsmSettings[2][0].GetScale();
			pScale3->m_Step = 0.5;
			::Settings.m_CSsmSettings[2][0].SetDefaultColors(1);
			::Settings.m_CSsmSettings[2][0].m_RefSurfaceType = REF_SURFACE_BEST_SPHERE;
		}
		else if (m_Screen == SCREEN_CTSUM4)//CT Custom
		{
			if (lParam == 0)      wParam = 31;
			else if (lParam == 1) wParam = 34;
			else if (lParam == 2) wParam = 32;
			else if (lParam == 3) wParam = 33;
		}
	}

	if (m_BigCTSumWnd->m_d < 0)
	{
		m_BigCTSumWnd->m_d = 0;

		if (wParam != -1)
		{
			::Settings.m_CSsmSettings[k][m_BigCTSumWnd->m_d].m_SizeSmall = FALSE;

			for (int i = 0; i< 4; i++)
				::Settings.m_CSsmSettings[k][i].m_Method3D = FALSE;
		}
	}

	if (wParam != -1) ::Settings.m_CSsmSettings[k][m_BigCTSumWnd->m_d].m_Type = wParam;


	m_pSumWnd = new CCTSingleSumWnd(this, m_SumRect, &m_Patient, m_pCTExams[0], &::Settings.m_CSsmSettings[k][0], k);

	m_pSumWnd->ShowWindow(SW_SHOW);

	return 0;
}

//***************************************************************************************
/*
void CMainWnd::OnCustomvisButtonClicked()
{
CreateSelectedIndicesLists();

if (m_NumSelectedWFExams + m_NumSelectedCTExams < 1) {
::Error("No exam is selected.");
return;
}

if (m_NumSelectedWFExams + m_NumSelectedCTExams > 1) {
::Error("Please select only one exam.");
return;
}

CString CustomVisFolderName;

CRegKey1 Key(HKEY_CURRENT_USER, MAIN_REG_KEY);
Key.GetStrValue("CustomVis Folder", CustomVisFolderName);

if (!::PathFileExists(CustomVisFolderName)) {

CFolderDlg::m_InitialFolderName = MAIN_DIR;

CString s("Please selectan existing or create a new folder for iTrace >> ZCAD export data");
CFolderDlg* pDlg = new CFolderDlg(this, s);
if (pDlg->Show()) {
CustomVisFolderName = pDlg->m_FolderName;
}
delete pDlg;

}

if (!::PathFileExists(CustomVisFolderName)) return;

Key.SetStrValue("CustomVis Folder", CustomVisFolderName);

::CreateDirectory(CustomVisFolderName, NULL);

?????????? ?????? ???????? ????? ???????? ?? ????????????? ???????

CString SubFolderName = CustomVisFolderName + "\\" + m_Patient.m_LastName;
CString MiddleName = m_Patient.m_MiddleName;
if (MiddleName != "") {
if (MiddleName.Right(1) == ".") MiddleName = MiddleName.Left(MiddleName.GetLength() - 1);
if (MiddleName != "") SubFolderName += "_" + MiddleName;
}
if (m_Patient.m_FirstName != "") SubFolderName += "_" + m_Patient.m_FirstName;
if (m_Patient.m_CustomID != "") SubFolderName += "_" + m_Patient.m_CustomID;
CString s;
s.Format(_T("_%04i-%02i-%02i", m_Patient.m_BirthYear, m_Patient.m_BirthMonth, m_Patient.m_BirthDay);
SubFolderName += s;
::CreateDirectory(SubFolderName, NULL);

if (!LoadSelectedExams()) {
return;
}

CBusyCursor Cursor;

if (m_pWFExams[0]) {

m_pWFExams[0]->Process();

SubFolderName += m_pWFExams[0]->m_Header.IsOD() ? "\\OD" : "\\OS";
s.Format(_T("_WF_%04i-%02i-%02i_%02i-%02i-%02i",
m_pWFExams[0]->m_Header.m_Year, m_pWFExams[0]->m_Header.m_Month, m_pWFExams[0]->m_Header.m_Day,
m_pWFExams[0]->m_Header.m_Hour, m_pWFExams[0]->m_Header.m_Minute, m_pWFExams[0]->m_Header.m_Second);
SubFolderName += s;

BOOL res = ::PathFileExists(SubFolderName);
int i = 1;
s = "";
while (res) {
s.Format(_T("_%i", i);
res = ::PathFileExists(SubFolderName + s);
i++;
}
SubFolderName += s;
::CreateDirectory(SubFolderName, NULL);

::ExportCustomvis(SubFolderName, m_pWFExams[0]);
}

if (m_pCTExams[0]) {

m_pCTExams[0]->Process();

SubFolderName += m_pCTExams[0]->m_Header.IsOD() ? "\\OD" : "\\OS";
s.Format(_T("_CT_%04i-%02i-%02i_%02i-%02i-%02i",
m_pCTExams[0]->m_Header.m_Year, m_pCTExams[0]->m_Header.m_Month, m_pCTExams[0]->m_Header.m_Day,
m_pCTExams[0]->m_Header.m_Hour, m_pCTExams[0]->m_Header.m_Minute, m_pCTExams[0]->m_Header.m_Second);
SubFolderName += s;

BOOL res = ::PathFileExists(SubFolderName);
int i = 1;
s = "";
while (res) {
s.Format(_T("_%i", i);
res = ::PathFileExists(SubFolderName + s);
i++;
}
SubFolderName += s;
::CreateDirectory(SubFolderName, NULL);

::ExportCustomvis(SubFolderName, m_pCTExams[0]);
}

DeleteLoadedExams();
}
*/

//***************************************************************************************

void CMainWnd::OnMouseMove(uint nFlags, CPoint Point)
{
	m_ExamImgInfo.ShowWindow(FALSE);
	m_NoteImgInfo.ShowWindow(FALSE);
	m_ShowInfo = FALSE;

	//Test the chart of patient
	//Another idea, set the appoint message system???
	CPoint point;
	GetCursorPos(&point);

	if (m_Level == LEVEL_MAIN && point.x < 1 && point.y < 1)
	{
		////Recover the exam patients;05202016 for a customer lost tdb data but has exam files.
		//RecoverFromExams();
		////Done

		/*CPatientChartDlg* pDlg = new CPatientChartDlg(this);
		pDlg->DoModal();
		delete pDlg;*/
	}
	//Done
}

//***************************************************************************************

BOOL CMainWnd::OnMouseWheel(uint nFlags, short zDelta, CPoint Point)
{
	if (m_pSumWnd)
	{
		if (m_Screen == SCREEN_BIG_TORIC_PLANNER || m_Screen == SCREEN_BIG_TORIC_ICL_PLANNER)//6.2.0 add SCREEN_BIG_TORIC_ICL_PLANNER
		{
			m_BigPlanWnd->OnMouseWheel(nFlags, zDelta, Point);
		}
		else if (m_Screen == SCREEN_BIG_CT_OSHER1 || m_Screen == SCREEN_BIG_CT_OSHER2
			|| m_Screen == SCREEN_BIG_CT_OSHER3 || m_Screen == SCREEN_BIG_CT_OSHER4
			|| m_Screen == SCREEN_BIG_CT_OSHER5)
		{
			m_BigCTSumWnd->OnMouseWheel(nFlags, zDelta, Point);
		}
		else
		{
			m_pSumWnd->OnMouseWheel(nFlags, zDelta, Point);
		}
	}
	else
	{
		if (m_Screen == SCREEN_BIG_CT_OSHER1 || m_Screen == SCREEN_BIG_CT_OSHER2
			|| m_Screen == SCREEN_BIG_CT_OSHER3 || m_Screen == SCREEN_BIG_CT_OSHER4
			|| m_Screen == SCREEN_BIG_CT_OSHER5)
		{
			m_BigCTSumWnd->OnMouseWheel(nFlags, zDelta, Point);
		}
	}

	return CWnd::OnMouseWheel(nFlags, zDelta, Point);
}

//***************************************************************************************

// For exporting the  exam to spreadsheet
int CMainWnd::SaveWFExamToExcel(CWFExam *WFExam, CString FileName)
{
	CPatient* pPatient = ::DB.GetPatient(WFExam->m_Header.m_PatientID);
	if (!pPatient) {
		::Error("No patient associated with the exam.");
		return -1;
	}

	HRESULT hr;

	hr = ::CoInitialize(NULL);

	if (FAILED(hr))
	{
		::Info("WF co error");
	}

	_Application excelApp;

	if (!excelApp.CreateDispatch(_T("Excel.Application"), NULL))
	{
		::Info("Excel application failed.");
		::Info("Please install Excel on your computer for Excel export.");
		::CoUninitialize();
		::Settings.m_Export_Excel = FALSE;
		return -2;
	}


	/*CString excelVer = excelApp.GetVersion();
	int   excelV = atoi(excelVer);

	if(excelV <= 20)
	{
	FileName.Replace(".xlsx", ".xls");
	}*/


	CString RecordTime;
	CString PaUniqueID, ExUniqueID, ExamType;

	CString UserID;  //1
	CString FName;   //2
	CString MName;   //3
	CString LName;   //4
	CString DOB;     //5
	CString Gender;  //6
	CString AddLine1;//7
	CString AddLine2;//8
	CString City;    //9
	CString State;   //10

	CString Zip;      //11
	CString Country;  //12
	CString WPhone;   //13
	CString HPhone;   //14
	CString Email;    //15
	CString Note;     //16
	CString GroupGUID;//17
	CString GroupName;//18
	CString CalID;    //19
	CString Mode;     //20

	CString Date;       //21
	CString Time;       //22
	CString Eye;        //23
	CString PrePost;    //24
	CString SW;         //25

	CString ClinicUniID; //53
	CString ClinicName;  //53
	CString ClinicAdd1; //53
	CString ClinicAdd2; //53
	CString ClinicCity; //53
	CString ClinicState; //53
	CString ClinicZip;  //53
	CString ClinicCountry;  //53

	CString PhysicianID;//26
	CString PhysFName;  //27
	CString PhysLName;  //28
	CString OperatorID; //29
	CString OperFName;  //30

	CString OperLName;  //31
	CString ExamNote;   //32
	CString AngelKDist; //33
	CString AngelKDeg;  //34
						//CString AngelADist; //35
						//CString AngelADeg;  //36
						//CString LimbalX;    //37
						//CString LimbalY;    //38
						//CString LimbalD;    //39
	CString PupilX;     //40

	CString PupilY;     //41
	CString PupilD;     //42
	CString ScanD;      //43

	CString TraceRefSph;//44
	CString TraceRefCyl;//44
	CString TraceRefAxis;//44
	CString TraceRefVd; //44

	CString Ref1ZoneSph;  //45
	CString Ref1ZoneCyl;  //45
	CString Ref1ZoneAxis; //45
	CString Ref1ZoneVd;   //45

	CString Ref2ZoneSph;  //46
	CString Ref2ZoneCyl;  //46
	CString Ref2ZoneAxis; //46
	CString Ref2ZoneVd;   //46

	CString Ref3ZoneSph;  //47
	CString Ref3ZoneCyl;  //47
	CString Ref3ZoneAxis; //47
	CString Ref3ZoneVd;   //47

	CString Ref4ZoneSph;  //48
	CString Ref4ZoneCyl;  //48
	CString Ref4ZoneAxis; //48
	CString Ref4ZoneVd;   //48

	CString EntireEyeZC1[45]; //49
	CString EntireEyeZC2[45]; //50
	CString EntireEyeZC3[45]; //51
	CString EntireEyeZCF[45]; //52

	CString WFOpacityVal;// wf

						 //ExportScreen

						 //Time
	SYSTEMTIME CurTime;
	::GetLocalTime(&CurTime);
	RecordTime.Format(_T("%02i-%02i-%04i %02i:%02i"), CurTime.wMonth, CurTime.wDay, CurTime.wYear, CurTime.wHour, CurTime.wMinute);
	//

	//PaUniqueID
	GUIDToStr(WFExam->m_Header.m_PatientID, PaUniqueID);
	//Done

	//ExUniqueID
	GUIDToStr(WFExam->m_Header.m_ExamID, ExUniqueID);
	//Done

	//ExamType
	ExamType = WFExam->m_Header.Type();
	//Done

	//UserID;  //1
	if (::Settings.m_EXCEL_WF_SETS[0] == '1')
	{
		UserID = pPatient->m_CustomID;
	}
	else UserID = "";
	//Done

	//FName;   //2
	if (::Settings.m_EXCEL_WF_SETS[1] == '1')
	{
		FName = pPatient->m_FirstName;
	}
	else FName = "";
	//Done

	//MName;   //3
	if (::Settings.m_EXCEL_WF_SETS[2] == '1')
	{
		MName = pPatient->m_MiddleName;
	}
	else MName = "";
	//Done

	//LName;   //4
	if (::Settings.m_EXCEL_WF_SETS[3] == '1')
	{
		LName = pPatient->m_LastName;
	}
	else LName = "";
	//Done

	//DOB;     //5
	if (::Settings.m_EXCEL_WF_SETS[4] == '1')
	{
		DOB.Format(_T("%02i-%02i-%04i"), pPatient->m_BirthMonth, pPatient->m_BirthDay, pPatient->m_BirthYear);
	}
	else DOB = "";
	//Done

	//Gender;  //6
	if (::Settings.m_EXCEL_WF_SETS[5] == '1')
	{
		if (pPatient->m_Sex == 1)
			Gender = "M";
		else Gender = "F";
	}
	else Gender = "";
	//Done

	//AddLine1;//7
	if (::Settings.m_EXCEL_WF_SETS[6] == '1')
	{
		AddLine1 = pPatient->m_Address.m_Line1;
	}
	else AddLine1 = "";
	//Done

	//AddLine2;//8
	if (::Settings.m_EXCEL_WF_SETS[7] == '1')
	{
		AddLine2 = pPatient->m_Address.m_Line2;
	}
	else AddLine2 = "";
	//Done

	//City;    //9
	if (::Settings.m_EXCEL_WF_SETS[8] == '1')
	{
		City = pPatient->m_Address.m_City;
	}
	else City = "";
	//Done

	//State;   //10
	if (::Settings.m_EXCEL_WF_SETS[9] == '1')
	{
		State = pPatient->m_Address.m_State;
	}
	else State = "";
	//Done

	//Zip;      //11
	if (::Settings.m_EXCEL_WF_SETS[10] == '1')
	{
		Zip = pPatient->m_Address.m_ZIP;
	}
	else Zip = "";
	//Done

	//Country;  //12
	if (::Settings.m_EXCEL_WF_SETS[11] == '1')
	{
		Country = pPatient->m_Address.m_Country;
	}
	else Country = "";
	//Done

	//WPhone;   //13
	if (::Settings.m_EXCEL_WF_SETS[12] == '1')
	{
		WPhone = pPatient->m_WorkPhone;
	}
	else WPhone = "";
	//Done

	//HPhone;   //14
	if (::Settings.m_EXCEL_WF_SETS[13] == '1')
	{
		HPhone = pPatient->m_HomePhone;
	}
	else HPhone = "";
	//Done

	//Email;    //15
	if (::Settings.m_EXCEL_WF_SETS[14] == '1')
	{
		Email = pPatient->m_EMail;
	}
	else Email = "";
	//Done

	//Note;     //16
	if (::Settings.m_EXCEL_WF_SETS[15] == '1')
	{
		Note = pPatient->m_Note;
	}
	else Note = "";
	//Done

	//GroupGUID;//17
	if (::Settings.m_EXCEL_WF_SETS[16] == '1')
	{
		GUIDToStr(pPatient->m_GroupID, GroupGUID);
	}
	else GroupGUID = "";
	//Done

	//GroupName;//18
	if (::Settings.m_EXCEL_WF_SETS[17] == '1')
	{
		CGroup* Group = ::DB.GetGroup(pPatient->m_GroupID);
		if (Group) GroupName = Group->m_Name;
		else GroupName = "";
	}
	else GroupName = "";
	//Done

	//CalID;    //19
	if (::Settings.m_EXCEL_WF_SETS[18] == '1')
	{
		GUIDToStr(::HW.m_Calibration.CalibrationID, CalID);
	}
	else CalID = "";
	//Done

	//Mode;     //20
	if (::Settings.m_EXCEL_WF_SETS[19] == '1')
	{
		Mode = WFExam->m_Header.Mode();
	}
	else Mode = "";
	//Done

	//Date;       //21
	if (::Settings.m_EXCEL_WF_SETS[20] == '1')
	{
		Date.Format(_T("%02i-%02i-%04i"), WFExam->m_Header.m_Month, WFExam->m_Header.m_Day, WFExam->m_Header.m_Year);
	}
	else Date = "";
	//Done

	//Time;       //22
	if (::Settings.m_EXCEL_WF_SETS[21] == '1')
	{
		Time.Format(_T("%02i:%02i:%02i"), WFExam->m_Header.m_Hour, WFExam->m_Header.m_Minute, WFExam->m_Header.m_Second);
	}
	else Time = "";
	//Done

	//Eye;        //23
	if (::Settings.m_EXCEL_WF_SETS[22] == '1')
	{
		Eye = WFExam->m_Header.Eye();
	}
	else Eye = "";
	//Done

	//PrePost;    //24
	if (::Settings.m_EXCEL_WF_SETS[23] == '1')
	{
		PrePost = WFExam->m_Header.Preop();
	}
	else PrePost = "";
	//Done

	//SW;         //25
	if (::Settings.m_EXCEL_WF_SETS[24] == '1')
	{
		SW = WFExam->m_Header.SoftwareVersion();
	}
	else SW = "";
	//Done

	//Clinic Info;         //53
	if (::Settings.m_EXCEL_WF_SETS[52] == '1')
	{
		CClinic* pClinic = ::DB.GetClinic(WFExam->m_Header.m_ClinicID);
		if (pClinic)
		{
			GUIDToStr(WFExam->m_Header.m_ClinicID, ClinicUniID);
			ClinicName = pClinic->m_Name;
			ClinicAdd1 = pClinic->m_Address.m_Line1;
			ClinicAdd2 = pClinic->m_Address.m_Line2;
			ClinicCity = pClinic->m_Address.m_City;
			ClinicState = pClinic->m_Address.m_State;
			ClinicZip = pClinic->m_Address.m_ZIP;
			ClinicCountry = pClinic->m_Address.m_Country;
		}
		else
		{
			ClinicUniID = "";
			ClinicName = "";
			ClinicAdd1 = "";
			ClinicAdd2 = "";
			ClinicCity = "";
			ClinicState = "";
			ClinicZip = "";
			ClinicCountry = "";
		}
	}
	else
	{
		ClinicUniID = "";
		ClinicName = "";
		ClinicAdd1 = "";
		ClinicAdd2 = "";
		ClinicCity = "";
		ClinicState = "";
		ClinicZip = "";
		ClinicCountry = "";
	}
	//Done

	CPhysician* pPhysician = ::DB.GetPhysician(WFExam->m_Header.m_PhysicianID);
	COperator*  pOperator = ::DB.GetOperator(WFExam->m_Header.m_OperatorID);

	//PhysicianID;//26
	if (::Settings.m_EXCEL_WF_SETS[25] == '1')
	{
		if (pPhysician) GUIDToStr(WFExam->m_Header.m_PhysicianID, PhysicianID);
		else PhysicianID = "";
	}
	else PhysicianID = "";
	//Done

	//PhysFName;  //27
	if (::Settings.m_EXCEL_WF_SETS[26] == '1')
	{
		if (pPhysician) PhysFName = pPhysician->m_FirstName;
		else PhysFName = "";
	}
	else PhysFName = "";
	//Done

	//PhysLName;  //28
	if (::Settings.m_EXCEL_WF_SETS[27] == '1')
	{
		if (pPhysician) PhysLName = pPhysician->m_LastName;
		else PhysLName = "";
	}
	else PhysLName = "";
	//Done

	//OperatorID; //29
	if (::Settings.m_EXCEL_WF_SETS[28] == '1')
	{
		if (pOperator) GUIDToStr(WFExam->m_Header.m_OperatorID, OperatorID);
		else OperatorID = "";
	}
	else OperatorID = "";
	//Done

	//OperFName;  //30
	if (::Settings.m_EXCEL_WF_SETS[29] == '1')
	{
		if (pOperator) OperFName = pOperator->m_FirstName;
		else OperFName = "";
	}
	else OperFName = "";
	//Done

	//OperLName;  //31
	if (::Settings.m_EXCEL_WF_SETS[30] == '1')
	{
		if (pOperator) OperLName = pOperator->m_LastName;
		else OperLName = "";
	}
	else OperLName = "";
	//Done

	//ExamNote;   //32
	if (::Settings.m_EXCEL_WF_SETS[31] == '1')
	{
		ExamNote = WFExam->m_Header.m_Note;
	}
	else ExamNote = "";
	//Done

	//AngelKDist; //33
	if (::Settings.m_EXCEL_WF_SETS[32] == '1')
	{
		if (WFExam->m_Header.m_AngleKappaOK)
		{
			AngelKDist.Format(_T("%.3f"), WFExam->m_Header.m_AngleKappaRUm / 1000.0);
		}
		else AngelKDist = "";
	}
	else AngelKDist = "";
	//Done

	//AngelKDeg;  //34
	if (::Settings.m_EXCEL_WF_SETS[33] == '1')
	{
		if (WFExam->m_Header.m_AngleKappaOK)
		{
			AngelKDeg.Format(_T("%i"), WFExam->m_Header.m_AngleKappaADg);
		}
		else AngelKDeg = "";
	}
	else AngelKDeg = "";
	//Done

	//AngelADist; //35
	/*if(::Settings.m_EXCEL_WF_SETS[34] == '1')
	{
	if(WFExam->m_Header.m_AngleAlphaOK)
	{
	AngelADist.Format(_T("%.3f"), WFExam->m_Header.m_AngleAlphaRUm/1000.0);
	}
	else AngelADist = "";
	}
	else AngelADist = "";	*/
	// Remove the Angle A and Limbus Data from WF Exam
	//Done

	//AngelADeg;  //36
	/*if(::Settings.m_EXCEL_WF_SETS[35] == '1')
	{
	if(WFExam->m_Header.m_AngleAlphaOK)
	{
	AngelADeg.Format(_T("%i"), WFExam->m_Header.m_AngleAlphaADg);
	}
	else AngelADeg = "";
	}
	else AngelADeg = "";*/
	// Remove the Angle A and Limbus Data from WF Exam
	//Done

	//LimbalX;    //37
	/*if(::Settings.m_EXCEL_WF_SETS[36] == '1')
	{
	if(WFExam->m_Image.m_li_ok)
	{
	LimbalX.Format(_T("%.3f"), WFExam->m_Image.m_li_x0_um/1000.0);
	}
	else LimbalX = "";
	}
	else LimbalX = "";*/
	// Remove the Angle A and Limbus Data from WF Exam
	//Done

	//LimbalY;    //38
	/*if(::Settings.m_EXCEL_WF_SETS[37] == '1')
	{
	if(WFExam->m_Image.m_li_ok)
	{
	LimbalY.Format(_T("%.3f"), WFExam->m_Image.m_li_y0_um/1000.0);
	}
	else LimbalY = "";
	}
	else LimbalY = "";*/
	// Remove the Angle A and Limbus Data from WF Exam
	//Done

	//LimbalD;    //39
	/*if(::Settings.m_EXCEL_WF_SETS[38] == '1')
	{
	if(WFExam->m_Image.m_li_ok)
	{
	LimbalD.Format(_T("%.3f"), (WFExam->m_Image.m_li_r_mean_um*2.0)/1000.0);
	}
	else LimbalD = "";
	}
	else LimbalD = "";*/
	// Remove the Angle A and Limbus Data from WF Exam
	//Done

	//PupilX;     //40
	if (::Settings.m_EXCEL_WF_SETS[39] == '1')
	{
		if (WFExam->m_Image.m_pu_ok)
		{
			PupilX.Format(_T("%.3f"), WFExam->m_Image.m_pu_x0_um / 1000.0);
		}
		else PupilX = "";
	}
	else PupilX = "";
	//Done

	//PupilY;     //41
	if (::Settings.m_EXCEL_WF_SETS[40] == '1')
	{
		if (WFExam->m_Image.m_pu_ok)
		{
			PupilY.Format(_T("%.3f"), WFExam->m_Image.m_pu_y0_um / 1000.0);
		}
		else PupilY = "";
	}
	else PupilY = "";
	//Done

	//PupilD;     //42
	if (::Settings.m_EXCEL_WF_SETS[41] == '1')
	{
		if (WFExam->m_Image.m_pu_ok)
		{
			PupilD.Format(_T("%.3f"), (WFExam->m_Image.m_pu_r_mean_um * 2) / 1000.0);
		}
		else PupilD = "";
	}
	else PupilD = "";
	//Done

	//ScanD;      //43
	if (::Settings.m_EXCEL_WF_SETS[42] == '1')
	{
		ScanD.Format(_T("%.3f"), WFExam->m_ScanDiameter / 1000.0);
	}
	else ScanD = "";
	//Done

	CZernikeSurface Surface[5];
	for (int i = 0; i < 5; i++) Surface[i] = WFExam->m_WfSurface;

	int vd = int(::Settings.m_VertexDistanceUm / 1000.0);
	CString vdStr;
	vdStr.Format(_T("%i"), vd);

	//TraceRef Sph, Cyl, Axis, VD;   //44
	if (::Settings.m_EXCEL_WF_SETS[43] == '1')
	{
		real_t SphEq, Sph, Cyl; int Axis;
		Surface[0].ChangeRMaxUm(2000.0);
		//WFExam->GetSpheqSphCylAxis(Surface, ::Settings.m_VertexDistanceUm, TRUE, SphEq, Sph, Cyl, Axis);
		CWFExam::GetSpheqSphCylAxis(Surface[0], ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder,
			SphEq, Sph, Cyl, Axis);

		if (::Settings.m_RefractionRounding == 0)
		{
			Sph = round8(Sph);
			Cyl = round8(Cyl);
		}

		TraceRefSph.Format(_T("%.2f"), Sph);
		TraceRefCyl.Format(_T("%.2f"), Cyl);
		TraceRefAxis.Format(_T("%i"), Axis);
		TraceRefVd = vdStr;
	}
	else
	{
		TraceRefSph = "";
		TraceRefCyl = "";
		TraceRefAxis = "";
		TraceRefVd = "";
	}
	//Done

	real_t ZoneRum[4];
	//Ref1Zone;   //45
	if (::Settings.m_EXCEL_WF_SETS[44] == '1')
	{
		ZoneRum[0] = 0.5 * ::Settings.m_ZoneDiameterUm[0];

		if (ZoneRum[0] <= WFExam->m_WfSurface.m_r_max_um)
		{
			real_t SphEq, Sph, Cyl; int Axis;
			Surface[1].ChangeRMaxUm(ZoneRum[0]);
			CWFExam::GetSpheqSphCylAxis(Surface[1], ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder,
				SphEq, Sph, Cyl, Axis);

			Ref1ZoneSph.Format(_T("%.2f"), Sph);
			Ref1ZoneCyl.Format(_T("%.2f"), Cyl);
			Ref1ZoneAxis.Format(_T("%i"), Axis);
			Ref1ZoneVd = vdStr;
		}
		else
		{
			Ref1ZoneSph = ""; //45
			Ref1ZoneCyl = ""; //45
			Ref1ZoneAxis = ""; //45
			Ref1ZoneVd = ""; //45
		}
	}
	else
	{
		Ref1ZoneSph = ""; //45
		Ref1ZoneCyl = ""; //45
		Ref1ZoneAxis = ""; //45
		Ref1ZoneVd = ""; //45
	}
	//Done

	//Ref2Zone;   //46
	if (::Settings.m_EXCEL_WF_SETS[45] == '1')
	{
		ZoneRum[1] = 0.5 * ::Settings.m_ZoneDiameterUm[1];

		if (ZoneRum[1] <= WFExam->m_WfSurface.m_r_max_um)
		{
			real_t SphEq, Sph, Cyl; int Axis;
			Surface[2].ChangeRMaxUm(ZoneRum[1]);
			CWFExam::GetSpheqSphCylAxis(Surface[2], ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder,
				SphEq, Sph, Cyl, Axis);

			Ref2ZoneSph.Format(_T("%.2f"), Sph);
			Ref2ZoneCyl.Format(_T("%.2f"), Cyl);
			Ref2ZoneAxis.Format(_T("%i"), Axis);
			Ref2ZoneVd = vdStr;
		}
		else
		{
			Ref2ZoneSph = ""; //45
			Ref2ZoneCyl = ""; //45
			Ref2ZoneAxis = ""; //45
			Ref2ZoneVd = ""; //45
		}
	}
	else
	{
		Ref2ZoneSph = ""; //46
		Ref2ZoneCyl = ""; //46
		Ref2ZoneAxis = ""; //46
		Ref2ZoneVd = ""; //46
	}
	//Done

	//Ref3Zone;   //47
	if (::Settings.m_EXCEL_WF_SETS[46] == '1')
	{
		ZoneRum[2] = 0.5 * ::Settings.m_ZoneDiameterUm[2];

		if (ZoneRum[2] <= WFExam->m_WfSurface.m_r_max_um)
		{
			real_t SphEq, Sph, Cyl; int Axis;
			Surface[3].ChangeRMaxUm(ZoneRum[2]);
			CWFExam::GetSpheqSphCylAxis(Surface[3], ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder,
				SphEq, Sph, Cyl, Axis);

			Ref3ZoneSph.Format(_T("%.2f"), Sph);
			Ref3ZoneCyl.Format(_T("%.2f"), Cyl);
			Ref3ZoneAxis.Format(_T("%i"), Axis);
			Ref3ZoneVd = vdStr;
		}
		else
		{
			Ref3ZoneSph = ""; //45
			Ref3ZoneCyl = ""; //45
			Ref3ZoneAxis = ""; //45
			Ref3ZoneVd = ""; //45
		}
	}
	else
	{
		Ref3ZoneSph = ""; //47
		Ref3ZoneCyl = ""; //47
		Ref3ZoneAxis = ""; //47
		Ref3ZoneVd = ""; //47
	}
	//Done

	//Ref4Zone;   //48
	if (::Settings.m_EXCEL_WF_SETS[47] == '1')
	{
		ZoneRum[3] = WFExam->m_WfSurface.m_r_max_um;

		if (ZoneRum[3] <= WFExam->m_WfSurface.m_r_max_um)
		{
			real_t SphEq, Sph, Cyl; int Axis;
			Surface[4].ChangeRMaxUm(ZoneRum[3]);
			CWFExam::GetSpheqSphCylAxis(Surface[4], ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder,
				SphEq, Sph, Cyl, Axis);

			Ref4ZoneSph.Format(_T("%.2f"), Sph);
			Ref4ZoneCyl.Format(_T("%.2f"), Cyl);
			Ref4ZoneAxis.Format(_T("%i"), Axis);
			Ref4ZoneVd = vdStr;
		}
		else
		{
			Ref4ZoneSph = ""; //45
			Ref4ZoneCyl = ""; //45
			Ref4ZoneAxis = ""; //45
			Ref4ZoneVd = ""; //45
		}
	}
	else
	{
		Ref4ZoneSph = ""; //48
		Ref4ZoneCyl = ""; //48
		Ref4ZoneAxis = ""; //48
		Ref4ZoneVd = ""; //48
	}
	//Done


	//EntireEyeZC1; //49
	if (::Settings.m_EXCEL_WF_SETS[48] == '1')
	{
		if (ZoneRum[0] <= WFExam->m_WfSurface.m_r_max_um)//6.2.0
		{
			for (int i = 0; i < 45; i++)
			{
				real_t um = Surface[1].GetCUm(i);
				//um = fabs(um);
				EntireEyeZC1[i].Format(_T("%6.10f"), um);
			}
		}
		else
		{
			for (int i = 0; i < 45; i++)
			{
				EntireEyeZC1[i] = "";
			}
		}
	}
	else
	{
		for (int i = 0; i < 45; i++)
		{
			EntireEyeZC1[i] = "";
		}
	}
	//Done

	//EntireEyeZC2; //50
	if (::Settings.m_EXCEL_WF_SETS[49] == '1')
	{
		if (ZoneRum[1] <= WFExam->m_WfSurface.m_r_max_um)//6.2.0
		{
			for (int i = 0; i < 45; i++)
			{
				real_t um = Surface[2].GetCUm(i);
				//um = fabs(um);
				EntireEyeZC2[i].Format(_T("%6.10f"), um);
			}
		}
		else
		{
			for (int i = 0; i < 45; i++)
			{
				EntireEyeZC2[i] = "";
			}
		}
	}
	else
	{
		for (int i = 0; i < 45; i++)
		{
			EntireEyeZC2[i] = "";
		}
	}
	//Done

	//EntireEyeZC3; //51
	if (::Settings.m_EXCEL_WF_SETS[50] == '1')
	{
		if (ZoneRum[2] <= WFExam->m_WfSurface.m_r_max_um)//6.2.0
		{
			for (int i = 0; i < 45; i++)
			{
				real_t um = Surface[3].GetCUm(i);
				//um = fabs(um);
				EntireEyeZC3[i].Format(_T("%6.10f"), um);
			}
		}
		else
		{
			for (int i = 0; i < 45; i++)
			{
				EntireEyeZC3[i] = "";
			}
		}
	}
	else
	{
		for (int i = 0; i < 45; i++)
		{
			EntireEyeZC3[i] = "";
		}
	}
	//Done

	//EntireEyeZCF; //52
	if (::Settings.m_EXCEL_WF_SETS[51] == '1')
	{
		for (int i = 0; i < 45; i++)
		{
			real_t um = Surface[4].GetCUm(i);
			//um = fabs(um);
			EntireEyeZCF[i].Format(_T("%6.10f"), um);
		}
	}
	else
	{
		for (int i = 0; i < 45; i++)
		{
			EntireEyeZCF[i] = "";//save wf
		}
	}
	//Done

	//
	if (::NewSettings.m_NEW_EXCEL_WF_SETS[0] == '1')
	{
		WFOpacityVal = GetOpacityGrade(WFExam);
	}
	else
	{
		WFOpacityVal = "";
	}
	//


	//Save the exam info into excel file

	//Following add the exam info into the excel file
	Range range;

	Workbooks books;
	_Workbook book;

	Worksheets sheets;
	_Worksheet sheet;

	LPDISPATCH lpDisp;
	//COleVariant vResult;

	COleVariant  covTrue((short)TRUE), covFalse((short)FALSE), covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	excelApp.SetVisible(FALSE);
	excelApp.SetUserControl(TRUE);

	books.AttachDispatch(excelApp.GetWorkbooks());

	long iStartRow = 1;
	BOOL NewExcel = FALSE;

	if (!::PathFileExists(FileName))
	{
		NewExcel = TRUE;
		books = excelApp.GetWorkbooks();
		book = books.Add(covOptional);
		sheets = book.GetSheets();
		sheet = sheets.GetItem(COleVariant((short)1));
		sheet.SetName(_T("WF Exam Report"));
	}
	else
	{
		try
		{
			lpDisp = books.Open(FileName,
				covOptional, covOptional, covOptional, covOptional, covOptional,
				covOptional, covOptional, covOptional, covOptional, covOptional,
				covOptional, covOptional);


			//Get work book
			book.AttachDispatch(lpDisp);
			//Get work sheets
			sheets.AttachDispatch(book.GetWorksheets());

			lpDisp = book.GetActiveSheet();
			sheet.AttachDispatch(lpDisp);
		}
		catch (...)
		{
			books.Close();
			excelApp.Quit();
			::CoUninitialize();
			::Info("Excel file open failed");
			return -3;
		}
	}

	//Used info
	if (!NewExcel)
	{
		Range usedRange;
		usedRange.AttachDispatch(sheet.GetUsedRange());
		range.AttachDispatch(usedRange.GetRows());
		long iRowNum = range.GetCount();   //Used Row
		iStartRow = iRowNum + 1;     //Started row
		range.AttachDispatch(sheet.GetCells(), TRUE);
	}
	else
	{
		//add Title
		CString StrPos[259] = { // from 263 to 264; and then from 264 to 259
			"A1", "B1", "C1", "D1", "E1", "F1", "G1",
			"H1", "I1", "J1", "K1", "L1", "M1", "N1",
			"O1", "P1", "Q1", "R1", "S1", "T1", "U1",
			"V1", "W1", "X1", "Y1", "Z1",
			"AA1", "AB1", "AC1", "AD1", "AE1", "AF1", "AG1",
			"AH1", "AI1", "AJ1", "AK1", "AL1", "AM1", "AN1",
			"AO1", "AP1", "AQ1", "AR1", "AS1", "AT1", "AU1",
			"AV1", "AW1", "AX1", "AY1", "AZ1",
			"BA1", "BB1", "BC1", "BD1", "BE1", "BF1", "BG1",
			"BH1", "BI1", "BJ1", "BK1", "BL1", "BM1", "BN1",
			"BO1", "BP1", "BQ1", "BR1", "BS1", "BT1", "BU1",
			"BV1", "BW1", "BX1", "BY1", "BZ1",
			"CA1", "CB1", "CC1", "CD1", "CE1", "CF1", "CG1",
			"CH1", "CI1", "CJ1", "CK1", "CL1", "CM1", "CN1",
			"CO1", "CP1", "CQ1", "CR1", "CS1", "CT1", "CU1",
			"CV1", "CW1", "CX1", "CY1", "CZ1",
			"DA1", "DB1", "DC1", "DD1", "DE1", "DF1", "DG1",
			"DH1", "DI1", "DJ1", "DK1", "DL1", "DM1", "DN1",
			"DO1", "DP1", "DQ1", "DR1", "DS1", "DT1", "DU1",
			"DV1", "DW1", "DX1", "DY1", "DZ1",
			"EA1", "EB1", "EC1", "ED1", "EE1", "EF1", "EG1",
			"EH1", "EI1", "EJ1", "EK1", "EL1", "EM1", "EN1",
			"EO1", "EP1", "EQ1", "ER1", "ES1", "ET1", "EU1",
			"EV1", "EW1", "EX1", "EY1", "EZ1",
			"FA1", "FB1", "FC1", "FD1", "FE1", "FF1", "FG1",
			"FH1", "FI1", "FJ1", "FK1", "FL1", "FM1", "FN1",
			"FO1", "FP1", "FQ1", "FR1", "FS1", "FT1", "FU1",
			"FV1", "FW1", "FX1", "FY1", "FZ1",
			"GA1", "GB1", "GC1", "GD1", "GE1", "GF1", "GG1",
			"GH1", "GI1", "GJ1", "GK1", "GL1", "GM1", "GN1",
			"GO1", "GP1", "GQ1", "GR1", "GS1", "GT1", "GU1",
			"GV1", "GW1", "GX1", "GY1", "GZ1",
			"HA1", "HB1", "HC1", "HD1", "HE1", "HF1", "HG1",
			"HH1", "HI1", "HJ1", "HK1", "HL1", "HM1", "HN1",
			"HO1", "HP1", "HQ1", "HR1", "HS1", "HT1", "HU1",
			"HV1", "HW1", "HX1", "HY1", "HZ1",
			"IA1", "IB1", "IC1", "ID1", "IE1", "IF1", "IG1",
			"IH1", "II1", "IJ1", "IK1", "IL1", "IM1", "IN1",
			"IO1", "IP1", "IQ1", "IR1", "IS1", "IT1", "IU1",
			"IV1", "IW1", "IX1", "IY1",
			// Add "JD1", and then remove  "IZ1","JA1", "JB1", "JC1", "JD1"
		};
		int WidthPos[259] = { // from 263 to 264; and then from 264 to 259
			16, 40, 12, 12, 12, 12, 12, // a, b, c, d, e, f, g
			12, 12, 12, 12, 12, 12, 12, // h, i, j, k, l, m, n
			12, 12, 12, 40, 40, 12, 40, // o, p, q, r, s, t, u
			40, 20, 12, 12, 12,         // v, w, x, y, z
			12, 12, 12, 40, 12, 12, 12,
			12, 12, 12, 12, 40, 20, 20,
			40, 20, 20, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 16,
			12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 16,
			12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 16,
			12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 16,
			12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 16,
			12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 16,
			12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 16,
			12, 12, 12, 12, 12,
			12, 12, 12, 12// add one more 12 value
		};

		Font font;
		for (int i = 0; i < 259; i++)// from 263 to 264; and then from 264 to 259
		{
			range = sheet.GetRange(COleVariant(StrPos[i]), COleVariant(StrPos[i]));

			range.SetColumnWidth(COleVariant((long)WidthPos[i]));

			//Cell Font
			Font font;
			font.AttachDispatch(range.GetFont());
			font.SetBold(covTrue);
			font.SetColor(COleVariant((long)0xFF0000));
			range.SetHorizontalAlignment(COleVariant((short)-4108)); //Center
			range.SetVerticalAlignment(COleVariant((short)-4108)); //Center
																   //Done

																   //Background Color
			Interior it;
			it.AttachDispatch(range.GetInterior());
			if (i < 20)   it.SetColorIndex(COleVariant((long)20));
			else         it.SetColorIndex(COleVariant((long)40));

			if (i == 74 || i == 120 || i == 166 || i == 212)
				it.SetColorIndex(COleVariant((long)33));

			if (i == 258) it.SetColorIndex(COleVariant((long)15));//
																  //Done

																  //Border
			Borders border;
			border = range.GetBorders();
			border.SetColorIndex(COleVariant((long)1));
			border.SetLineStyle(COleVariant((long)1));
			border.SetWeight(COleVariant((long)4));
			//Done
		}

		range.AttachDispatch(sheet.GetCells(), TRUE);

		//range.AutoOutline();

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)1), COleVariant(_T("Export Time")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)2), COleVariant(_T("Patient Unique ID")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)3), COleVariant(_T("ID")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)4), COleVariant(_T("First Name")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)5), COleVariant(_T("Middle Name")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)6), COleVariant(_T("Last Name")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)7), COleVariant(_T("DOB")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)8), COleVariant(_T("Gender")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)9), COleVariant(_T("Add Line1")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)10), COleVariant(_T("Add Line2")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)11), COleVariant(_T("City")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)12), COleVariant(_T("State")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)13), COleVariant(_T("Zip")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)14), COleVariant(_T("Country")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)15), COleVariant(_T("Work Phone")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)16), COleVariant(_T("Home Phone")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)17), COleVariant(_T("Email")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)18), COleVariant(_T("Note")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)19), COleVariant(_T("Group GUID")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)20), COleVariant(_T("Group Name")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)21), COleVariant(_T("Exam Unique ID")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)22), COleVariant(_T("Calibration Unique ID")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)23), COleVariant(_T("Exam Type")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)24), COleVariant(_T("Mode")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)25), COleVariant(_T("Exam Date")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)26), COleVariant(_T("Exam Time")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)27), COleVariant(_T("Eye")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)28), COleVariant(_T("Pre/Post")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)29), COleVariant(_T("SW")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)30), COleVariant(_T("Clinic Unique ID")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)31), COleVariant(_T("Clinic Name")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)32), COleVariant(_T("Clinic Add1")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)33), COleVariant(_T("Clinic Add2")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)34), COleVariant(_T("Clinic City")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)35), COleVariant(_T("Clinic State")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)36), COleVariant(_T("Clinic Zip")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)37), COleVariant(_T("Clinic Country")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)38), COleVariant(_T("Physician ID")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)39), COleVariant(_T("Physician First Name")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)40), COleVariant(_T("Physician Last Name")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)41), COleVariant(_T("Operator ID")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)42), COleVariant(_T("Operator First Name")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)43), COleVariant(_T("Operator Last Name")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)44), COleVariant(_T("Exam Note")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)45), COleVariant(_T("Angel K Dist")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)46), COleVariant(_T("Angel K Degree")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)47), COleVariant(_T("Pupil Center X")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)48), COleVariant(_T("Pupil Center Y")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)49), COleVariant(_T("Pupil Diameter")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)50), COleVariant(_T("Scan Diameter")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)51), COleVariant(_T("TraceRef Sph")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)52), COleVariant(_T("TraceRef Cyl")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)53), COleVariant(_T("TraceRef Axis")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)54), COleVariant(_T("TraceRef VD")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)55), COleVariant(_T("Ref1 Zone")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)56), COleVariant(_T("Ref1 Sph")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)57), COleVariant(_T("Ref1 Cyl")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)58), COleVariant(_T("Ref1 Axis")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)59), COleVariant(_T("Ref1 VD")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)60), COleVariant(_T("Ref2 Zone")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)61), COleVariant(_T("Ref2 Sph")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)62), COleVariant(_T("Ref2 Cyl")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)63), COleVariant(_T("Ref2 Axis")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)64), COleVariant(_T("Ref2 VD")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)65), COleVariant(_T("Ref3 Zone")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)66), COleVariant(_T("Ref3 Sph")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)67), COleVariant(_T("Ref3 Cyl")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)68), COleVariant(_T("Ref3 Axis")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)69), COleVariant(_T("Ref3 VD")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)70), COleVariant(_T("Ref4 Zone")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)71), COleVariant(_T("Ref4 Sph")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)72), COleVariant(_T("Ref4 Cyl")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)73), COleVariant(_T("Ref4 Axis")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)74), COleVariant(_T("Ref4 VD")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)75), COleVariant(_T("Z1 Zone")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)121), COleVariant(_T("Z2 Zone")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)167), COleVariant(_T("Z3 Zone")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)213), COleVariant(_T("Z4 Zone")));

		CString s, s1, s2, s3, s4;
		for (int i = 0; i < 45; i++)
		{
			s.Format(_T("%i"), i);
			s1 = "Z1 C" + s;
			s2 = "Z2 C" + s;
			s3 = "Z3 C" + s;
			s4 = "Z4 C" + s;
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)76 + i), COleVariant(s1));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)122 + i), COleVariant(s2));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)168 + i), COleVariant(s3));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)214 + i), COleVariant(s4));
		}


		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)259), COleVariant(_T("Opacity Grade")));//

		iStartRow = 2;
	}

	//Save Data
	{
		CString s1, s2, s3, s4;

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)1), COleVariant(RecordTime));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)2), COleVariant(PaUniqueID));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)3), COleVariant(UserID));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)4), COleVariant(FName));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)5), COleVariant(MName));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)6), COleVariant(LName));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)7), COleVariant(DOB));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)8), COleVariant(Gender));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)9), COleVariant(AddLine1));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)10), COleVariant(AddLine2));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)11), COleVariant(City));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)12), COleVariant(State));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)13), COleVariant(Zip));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)14), COleVariant(Country));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)15), COleVariant(WPhone));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)16), COleVariant(HPhone));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)17), COleVariant(Email));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)18), COleVariant(Note));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)19), COleVariant(GroupGUID));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)20), COleVariant(GroupName));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)21), COleVariant(ExUniqueID));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)22), COleVariant(CalID));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)23), COleVariant(ExamType));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)24), COleVariant(Mode));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)25), COleVariant(Date));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)26), COleVariant(Time));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)27), COleVariant(Eye));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)28), COleVariant(PrePost));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)29), COleVariant(SW));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)30), COleVariant(ClinicUniID));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)31), COleVariant(ClinicName));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)32), COleVariant(ClinicAdd1));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)33), COleVariant(ClinicAdd2));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)34), COleVariant(ClinicCity));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)35), COleVariant(ClinicState));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)36), COleVariant(ClinicZip));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)37), COleVariant(ClinicCountry));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)38), COleVariant(PhysicianID));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)39), COleVariant(PhysFName));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)40), COleVariant(PhysLName));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)41), COleVariant(OperatorID));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)42), COleVariant(OperFName));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)43), COleVariant(OperLName));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)44), COleVariant(ExamNote));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)45), COleVariant(AngelKDist));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)46), COleVariant(AngelKDeg));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)47), COleVariant(PupilX));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)48), COleVariant(PupilY));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)49), COleVariant(PupilD));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)50), COleVariant(ScanD));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)51), COleVariant(TraceRefSph));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)52), COleVariant(TraceRefCyl));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)53), COleVariant(TraceRefAxis));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)54), COleVariant(TraceRefVd));

		s1.Format(_T("%.1f"), (real_t)::Settings.m_ZoneDiameterUm[0] / 1000.0);//6.2.0
		if (::Settings.m_EXCEL_WF_SETS[44] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)55), COleVariant(s1));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)56), COleVariant(Ref1ZoneSph));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)57), COleVariant(Ref1ZoneCyl));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)58), COleVariant(Ref1ZoneAxis));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)59), COleVariant(Ref1ZoneVd));

		s2.Format(_T("%.1f"), (real_t)::Settings.m_ZoneDiameterUm[1] / 1000.0);//6.2.0
		if (::Settings.m_EXCEL_WF_SETS[45] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)60), COleVariant(s2));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)61), COleVariant(Ref2ZoneSph));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)62), COleVariant(Ref2ZoneCyl));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)63), COleVariant(Ref2ZoneAxis));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)64), COleVariant(Ref2ZoneVd));

		s3.Format(_T("%.1f"), (real_t)::Settings.m_ZoneDiameterUm[2] / 1000.0);//6.2.0
		if (::Settings.m_EXCEL_WF_SETS[46] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)65), COleVariant(s3));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)66), COleVariant(Ref3ZoneSph));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)67), COleVariant(Ref3ZoneCyl));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)68), COleVariant(Ref3ZoneAxis));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)69), COleVariant(Ref3ZoneVd));

		s4.Format(_T("%.1f"), (real_t)WFExam->m_WfSurface.m_r_max_um * 2 / 1000.0);//6.2.0
		if (::Settings.m_EXCEL_WF_SETS[47] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)70), COleVariant(s4));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)71), COleVariant(Ref4ZoneSph));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)72), COleVariant(Ref4ZoneCyl));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)73), COleVariant(Ref4ZoneAxis));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)74), COleVariant(Ref4ZoneVd));

		if (::Settings.m_EXCEL_WF_SETS[48] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)75), COleVariant(s1));

		if (::Settings.m_EXCEL_WF_SETS[49] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)121), COleVariant(s2));

		if (::Settings.m_EXCEL_WF_SETS[50] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)167), COleVariant(s3));

		if (::Settings.m_EXCEL_WF_SETS[51] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)213), COleVariant(s4));

		for (int i = 0; i < 45; i++)
		{
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)76 + i), COleVariant(EntireEyeZC1[i]));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)122 + i), COleVariant(EntireEyeZC2[i]));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)168 + i), COleVariant(EntireEyeZC3[i]));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)214 + i), COleVariant(EntireEyeZCF[i]));
		}

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)259), COleVariant(WFOpacityVal));//
	}

	if (!NewExcel)
	{
		book.Save();
		book.SetSaved(TRUE);
	}
	else
	{
		book.SaveAs(COleVariant(FileName),
			covOptional, covOptional, covOptional, covOptional,
			covOptional, 0, covOptional, covOptional,
			covOptional, covOptional);
	}

	//release
	range.ReleaseDispatch();
	sheet.ReleaseDispatch();
	sheets.ReleaseDispatch();
	book.ReleaseDispatch();
	books.ReleaseDispatch();
	excelApp.Quit();
	excelApp.ReleaseDispatch();
	::CoUninitialize();
	return 0;
	//Done
}
//

int CMainWnd::SaveCTExamToExcel(CCTExam *CTExam, CString FileName)
{
	CPatient* pPatient = ::DB.GetPatient(CTExam->m_Header.m_PatientID);
	if (!pPatient) {
		::Error("No patient associated with the exam.");
		return -1;
	}

	HRESULT hr;

	hr = ::CoInitialize(NULL);

	if (FAILED(hr))
	{
		::Info("CT co error");
	}

	_Application excelApp;

	if (!excelApp.CreateDispatch(_T("Excel.Application"), NULL))
	{
		::Info("Excel application failed.");
		::Info("Please install Excel on your computer for Excel export.");
		::CoUninitialize();
		::Settings.m_Export_Excel = FALSE;
		return -2;
	}

	CString RecordTime;
	CString PaUniqueID, ExUniqueID, ExamType;

	CString UserID;  //1
	CString FName;   //2
	CString MName;   //3
	CString LName;   //4
	CString DOB;     //5
	CString Gender;  //6
	CString AddLine1;//7
	CString AddLine2;//8
	CString City;    //9
	CString State;   //10

	CString Zip;      //11
	CString Country;  //12
	CString WPhone;   //13
	CString HPhone;   //14
	CString Email;    //15
	CString Note;     //16
	CString GroupGUID;//17
	CString GroupName;//18
	CString CalID;    //19
	CString Mode;     //20

	CString Date;       //21
	CString Time;       //22
	CString Eye;        //23
	CString PrePost;    //24
	CString SW;         //25

	CString ClinicUniID; //68
	CString ClinicName;  //68
	CString ClinicAdd1; //68
	CString ClinicAdd2; //68
	CString ClinicCity; //68
	CString ClinicState; //68
	CString ClinicZip;  //68
	CString ClinicCountry;  //68

	CString PhysicianID;//26
	CString PhysFName;  //27
	CString PhysLName;  //28
	CString OperatorID; //29
	CString OperFName;  //30

	CString OperLName;  //31
	CString ExamNote;   //32

	CString LimbalX;    //33
	CString LimbalY;    //34
	CString LimbalD;    //35

	CString CentralPwrD; //36
	CString CentralPwrMM; //36

	CString BestFitSph; //37
	CString BestConCen; //38
	CString BestConEcc; //39
	CString BestConAsp; //40

	CString RefPwrStpD;     //41
	CString RefPwrStpAxis;  //41

	CString RefPwrFltD;     //42
	CString RefPwrFltAxis;  //42

	CString EffRef;     //43

	CString RefPwrAstD;     //44
	CString RefPwrAstAxis;  //44

	CString SimKStpD;    //45
	CString SimKStpMM;   //45
	CString SimKStpDeg;  //45

	CString SimKFltD;    //46
	CString SimKFltMM;   //46
	CString SimKFltDeg;  //46

	CString SimKAvgD;    //47
	CString SimKAvgMM;   //47

	CString SimKDelD;    //48
	CString SimKDelDGE;  //48

	CString IS;         //49

	CString AvePwr[11]; //50

	CString StpSemi1D;   //51
	CString StpSemi1MM;  //51
	CString StpSemi1Deg; //51

	CString StpSemi2D;   //52
	CString StpSemi2MM;  //52
	CString StpSemi2Deg; //52

	CString FltSemi1D;   //53
	CString FltSemi1MM;  //53
	CString FltSemi1Deg; //53

	CString FltSemi2D;   //54
	CString FltSemi2MM;  //54
	CString FltSemi2Deg; //54

	CString StpSemi12D;   //55
	CString StpSemi12MM;  //55
	CString StpSemi12Deg; //55

	CString StpSemi22D;   //56
	CString StpSemi22MM;  //56
	CString StpSemi22Deg; //56

	CString FltSemi12D;   //57
	CString FltSemi12MM;  //57
	CString FltSemi12Deg; //57

	CString FltSemi22D;   //58
	CString FltSemi22MM;  //58
	CString FltSemi22Deg; //58

	CString StpSemi13D;   //59
	CString StpSemi13MM;  //59
	CString StpSemi13Deg; //59

	CString StpSemi23D;   //60
	CString StpSemi23MM;  //60
	CString StpSemi23Deg; //60

	CString FltSemi13D;   //61
	CString FltSemi13MM;  //61
	CString FltSemi13Deg; //61

	CString FltSemi23D;   //62
	CString FltSemi23MM;  //62
	CString FltSemi23Deg; //62

	CString CornealSph; //63

	CString CornealZer1[45]; //64
	CString CornealZer2[45]; //65
	CString CornealZer3[45]; //66
	CString CornealZerF[45]; //67

							 //ExportScreen

							 //Time
	SYSTEMTIME CurTime;
	::GetLocalTime(&CurTime);
	RecordTime.Format(_T("%02i-%02i-%04i %02i:%02i"), CurTime.wMonth, CurTime.wDay, CurTime.wYear, CurTime.wHour, CurTime.wMinute);
	//

	//PaUniqueID
	GUIDToStr(CTExam->m_Header.m_PatientID, PaUniqueID);
	//Done

	//ExUniqueID
	GUIDToStr(CTExam->m_Header.m_ExamID, ExUniqueID);
	//Done

	//ExamType
	ExamType = CTExam->m_Header.Type();
	//Done

	//UserID;  //1
	if (::Settings.m_EXCEL_CT_SETS[0] == '1')
	{
		UserID = pPatient->m_CustomID;
	}
	else UserID = "";
	//Done

	//FName;   //2
	if (::Settings.m_EXCEL_CT_SETS[1] == '1')
	{
		FName = pPatient->m_FirstName;
	}
	else FName = "";
	//Done

	//MName;   //3
	if (::Settings.m_EXCEL_CT_SETS[2] == '1')
	{
		MName = pPatient->m_MiddleName;
	}
	else MName = "";
	//Done

	//LName;   //4
	if (::Settings.m_EXCEL_CT_SETS[3] == '1')
	{
		LName = pPatient->m_LastName;
	}
	else LName = "";
	//Done

	//DOB;     //5
	if (::Settings.m_EXCEL_CT_SETS[4] == '1')
	{
		DOB.Format(_T("%02i-%02i-%04i"), pPatient->m_BirthMonth, pPatient->m_BirthDay, pPatient->m_BirthYear);
	}
	else DOB = "";
	//Done

	//Gender;  //6
	if (::Settings.m_EXCEL_CT_SETS[5] == '1')
	{
		if (pPatient->m_Sex == 1)
			Gender = "M";
		else Gender = "F";
	}
	else Gender = "";
	//Done

	//AddLine1;//7
	if (::Settings.m_EXCEL_CT_SETS[6] == '1')
	{
		AddLine1 = pPatient->m_Address.m_Line1;
	}
	else AddLine1 = "";
	//Done

	//AddLine2;//8
	if (::Settings.m_EXCEL_CT_SETS[7] == '1')
	{
		AddLine2 = pPatient->m_Address.m_Line2;
	}
	else AddLine2 = "";
	//Done

	//City;    //9
	if (::Settings.m_EXCEL_CT_SETS[8] == '1')
	{
		City = pPatient->m_Address.m_City;
	}
	else City = "";
	//Done

	//State;   //10
	if (::Settings.m_EXCEL_CT_SETS[9] == '1')
	{
		State = pPatient->m_Address.m_State;
	}
	else State = "";
	//Done

	//Zip;      //11
	if (::Settings.m_EXCEL_CT_SETS[10] == '1')
	{
		Zip = pPatient->m_Address.m_ZIP;
	}
	else Zip = "";
	//Done

	//Country;  //12
	if (::Settings.m_EXCEL_CT_SETS[11] == '1')
	{
		Country = pPatient->m_Address.m_Country;
	}
	else Country = "";
	//Done

	//WPhone;   //13
	if (::Settings.m_EXCEL_CT_SETS[12] == '1')
	{
		WPhone = pPatient->m_WorkPhone;
	}
	else WPhone = "";
	//Done

	//HPhone;   //14
	if (::Settings.m_EXCEL_CT_SETS[13] == '1')
	{
		HPhone = pPatient->m_HomePhone;
	}
	else HPhone = "";
	//Done

	//Email;    //15
	if (::Settings.m_EXCEL_CT_SETS[14] == '1')
	{
		Email = pPatient->m_EMail;
	}
	else Email = "";
	//Done

	//Note;     //16
	if (::Settings.m_EXCEL_CT_SETS[15] == '1')
	{
		Note = pPatient->m_Note;
	}
	else Note = "";
	//Done

	//GroupGUID;//17
	if (::Settings.m_EXCEL_CT_SETS[16] == '1')
	{
		GUIDToStr(pPatient->m_GroupID, GroupGUID);
	}
	else GroupGUID = "";
	//Done

	//GroupName;//18
	if (::Settings.m_EXCEL_CT_SETS[17] == '1')
	{
		CGroup* Group = ::DB.GetGroup(pPatient->m_GroupID);
		if (Group) GroupName = Group->m_Name;
		else GroupName = "";
	}
	else GroupName = "";
	//Done

	//CalID;    //19
	if (::Settings.m_EXCEL_CT_SETS[18] == '1')
	{
		GUIDToStr(::HW.m_Calibration.CalibrationID, CalID);
	}
	else CalID = "";
	//Done

	//Mode;     //20
	if (::Settings.m_EXCEL_CT_SETS[19] == '1')
	{
		Mode = CTExam->m_Header.Mode();
	}
	else Mode = "";
	//Done

	//Date;       //21
	if (::Settings.m_EXCEL_CT_SETS[20] == '1')
	{
		Date.Format(_T("%02i-%02i-%04i"), CTExam->m_Header.m_Month, CTExam->m_Header.m_Day, CTExam->m_Header.m_Year);
	}
	else Date = "";
	//Done

	//Time;       //22
	if (::Settings.m_EXCEL_CT_SETS[21] == '1')
	{
		Time.Format(_T("%02i:%02i:%02i"), CTExam->m_Header.m_Hour, CTExam->m_Header.m_Minute, CTExam->m_Header.m_Second);
	}
	else Time = "";
	//Done

	//Eye;        //23
	if (::Settings.m_EXCEL_CT_SETS[22] == '1')
	{
		Eye = CTExam->m_Header.Eye();
	}
	else Eye = "";
	//Done

	//PrePost;    //24
	if (::Settings.m_EXCEL_CT_SETS[23] == '1')
	{
		PrePost = CTExam->m_Header.Preop();
	}
	else PrePost = "";
	//Done

	//SW;         //25
	if (::Settings.m_EXCEL_CT_SETS[24] == '1')
	{
		SW = CTExam->m_Header.SoftwareVersion();
	}
	else SW = "";
	//Done

	//Clinic Info;         //68
	if (::Settings.m_EXCEL_CT_SETS[67] == '1')
	{
		CClinic* pClinic = ::DB.GetClinic(CTExam->m_Header.m_ClinicID);
		if (pClinic)
		{
			GUIDToStr(CTExam->m_Header.m_ClinicID, ClinicUniID);
			ClinicName = pClinic->m_Name;
			ClinicAdd1 = pClinic->m_Address.m_Line1;
			ClinicAdd2 = pClinic->m_Address.m_Line2;
			ClinicCity = pClinic->m_Address.m_City;
			ClinicState = pClinic->m_Address.m_State;
			ClinicZip = pClinic->m_Address.m_ZIP;
			ClinicCountry = pClinic->m_Address.m_Country;
		}
		else
		{
			ClinicUniID = "";
			ClinicName = "";
			ClinicAdd1 = "";
			ClinicAdd2 = "";
			ClinicCity = "";
			ClinicState = "";
			ClinicZip = "";
			ClinicCountry = "";
		}
	}
	else
	{
		ClinicUniID = "";
		ClinicName = "";
		ClinicAdd1 = "";
		ClinicAdd2 = "";
		ClinicCity = "";
		ClinicState = "";
		ClinicZip = "";
		ClinicCountry = "";
	}
	//Done

	CPhysician* pPhysician = ::DB.GetPhysician(CTExam->m_Header.m_PhysicianID);
	COperator*  pOperator = ::DB.GetOperator(CTExam->m_Header.m_OperatorID);

	//PhysicianID;//26
	if (::Settings.m_EXCEL_CT_SETS[25] == '1')
	{
		if (pPhysician) GUIDToStr(CTExam->m_Header.m_PhysicianID, PhysicianID);
		else PhysicianID = "";
	}
	else PhysicianID = "";
	//Done

	//PhysFName;  //27
	if (::Settings.m_EXCEL_CT_SETS[26] == '1')
	{
		if (pPhysician) PhysFName = pPhysician->m_FirstName;
		else PhysFName = "";
	}
	else PhysFName = "";
	//Done

	//PhysLName;  //28
	if (::Settings.m_EXCEL_CT_SETS[27] == '1')
	{
		if (pPhysician) PhysLName = pPhysician->m_LastName;
		else PhysLName = "";
	}
	else PhysLName = "";
	//Done

	//OperatorID; //29
	if (::Settings.m_EXCEL_CT_SETS[28] == '1')
	{
		if (pOperator) GUIDToStr(CTExam->m_Header.m_OperatorID, OperatorID);
		else OperatorID = "";
	}
	else OperatorID = "";
	//Done

	//OperFName;  //30
	if (::Settings.m_EXCEL_CT_SETS[29] == '1')
	{
		if (pOperator) OperFName = pOperator->m_FirstName;
		else OperFName = "";
	}
	else OperFName = "";
	//Done

	//OperLName;  //31
	if (::Settings.m_EXCEL_CT_SETS[30] == '1')
	{
		if (pOperator) OperLName = pOperator->m_LastName;
		else OperLName = "";
	}
	else OperLName = "";
	//Done

	//ExamNote;   //32
	if (::Settings.m_EXCEL_CT_SETS[31] == '1')
	{
		ExamNote = CTExam->m_Header.m_Note;
	}
	else ExamNote = "";
	//Done

	//LimbalX;    //33
	if (::Settings.m_EXCEL_CT_SETS[32] == '1')
	{
		if (CTExam->m_Image.m_li_ok)
		{
			LimbalX.Format(_T("%.3f"), CTExam->m_Image.m_li_x0_um / 1000.0);
		}
		else LimbalX = "";
	}
	else LimbalX = "";
	//Done

	//LimbalY;    //34
	if (::Settings.m_EXCEL_CT_SETS[33] == '1')
	{
		if (CTExam->m_Image.m_li_ok)
		{
			LimbalY.Format(_T("%.3f"), CTExam->m_Image.m_li_y0_um / 1000.0);
		}
		else LimbalY = "";
	}
	else LimbalY = "";
	//Done

	//LimbalD;    //35
	if (::Settings.m_EXCEL_CT_SETS[34] == '1')
	{
		if (CTExam->m_Image.m_li_ok)
		{
			LimbalD.Format(_T("%.3f"), (CTExam->m_Image.m_li_r_mean_um*2.0) / 1000.0);
		}
		else LimbalD = "";
	}
	else LimbalD = "";
	//Done



	//CentralPwr;     //36
	if (::Settings.m_EXCEL_CT_SETS[35] == '1')
	{
		CentralPwrD.Format(_T("%.10f"), CTExam->m_ax0_dp);
		CentralPwrMM.Format(_T("%.10f"), 0.001 * CTExam->m_ax0_um);
	}
	else
	{
		CentralPwrD = "";
		CentralPwrMM = "";
	}
	//Done

	//BestFitSph; //37
	if (::Settings.m_EXCEL_CT_SETS[36] == '1')
	{
		BestFitSph.Format(_T("%.10f"), 0.001 * CTExam->m_HtSpSurface.m_ax0_um);
	}
	else  BestFitSph = "";
	//Done

	//BestConCen; //38
	if (::Settings.m_EXCEL_CT_SETS[37] == '1')
	{
		BestConCen.Format(_T("%.10f"), 0.001 * CTExam->m_HtCnSurface.m_ax0_um);
	}
	else  BestConCen = "";
	//Done

	//BestConEcc; //39
	if (::Settings.m_EXCEL_CT_SETS[38] == '1')
	{
		BestConEcc.Format(_T("%.10f"), CTExam->m_HtCnSurface.GetEccentricity());
	}
	else  BestConEcc = "";
	//Done

	//BestConAsp; //40
	if (::Settings.m_EXCEL_CT_SETS[39] == '1')
	{
		BestConAsp.Format(_T("%.10f"), CTExam->m_HtCnSurface.m_q);
	}
	else  BestConAsp = "";
	//Done

	//RefPwrStp;  //41
	if (::Settings.m_EXCEL_CT_SETS[40] == '1')
	{
		if (CTExam->m_rf_ok)
		{
			RefPwrStpD.Format(_T("%.10f"), CTExam->m_rf_st_dp);
			RefPwrStpAxis.Format(_T("%i"), CTExam->m_rf_st_dg);
		}
		else
		{
			RefPwrStpD = "";
			RefPwrStpAxis = "";
		}
	}
	else
	{
		RefPwrStpD = "";
		RefPwrStpAxis = "";
	}
	//Done

	//RefPwrFlt;  //42
	if (::Settings.m_EXCEL_CT_SETS[41] == '1')
	{
		if (CTExam->m_rf_ok)
		{
			RefPwrFltD.Format(_T("%.10f"), CTExam->m_rf_fl_dp);
			RefPwrFltAxis.Format(_T("%i"), CTExam->m_rf_fl_dg);
		}
		else
		{
			RefPwrFltD = "";
			RefPwrFltAxis = "";
		}

	}
	else
	{
		RefPwrFltD = "";
		RefPwrFltAxis = "";
	}
	//Done

	//EffRef;     //43
	if (::Settings.m_EXCEL_CT_SETS[42] == '1')
	{
		if (CTExam->m_rf_ok) EffRef.Format(_T("%.10f"), CTExam->m_rf_av_dp);
		else EffRef.Empty();
	}
	else  EffRef = "";
	//Done

	//RefPwrAst;  //44
	if (::Settings.m_EXCEL_CT_SETS[43] == '1')
	{
		if (CTExam->m_rf_ok)
		{
			RefPwrAstD.Format(_T("%.10f"), CTExam->m_rf_df_dp);
			RefPwrAstAxis.Format(_T("%i"), CTExam->m_rf_st_dg);
		}
		else
		{
			RefPwrAstD = "";
			RefPwrAstAxis = "";
		}
	}
	else
	{
		RefPwrAstD = "";
		RefPwrAstAxis = "";
	}
	//Done

	//SimKStp;    //45
	if (::Settings.m_EXCEL_CT_SETS[44] == '1')
	{
		if (CTExam->m_sk_ok)
		{
			SimKStpD.Format(_T("%.10f"), CTExam->m_sk_st_dp);
			SimKStpMM.Format(_T("%.10f"), 0.001 * CTExam->m_sk_st_um);
			SimKStpDeg.Format(_T("%i"), CTExam->m_sk_st_dg);
		}
		else
		{
			SimKStpD = "";
			SimKStpMM = "";
			SimKStpDeg = "";
		}
	}
	else
	{
		SimKStpD = "";
		SimKStpMM = "";
		SimKStpDeg = "";
	}
	//Done

	//SimKFlt;    //46
	if (::Settings.m_EXCEL_CT_SETS[45] == '1')
	{
		if (CTExam->m_sk_ok)
		{
			SimKFltD.Format(_T("%.10f"), CTExam->m_sk_fl_dp);
			SimKFltMM.Format(_T("%.10f"), 0.001 * CTExam->m_sk_fl_um);
			SimKFltDeg.Format(_T("%i"), CTExam->m_sk_fl_dg);
		}
		else
		{
			SimKFltD = "";
			SimKFltMM = "";
			SimKFltDeg = "";
		}
	}
	else
	{
		SimKFltD = "";
		SimKFltMM = "";
		SimKFltDeg = "";
	}
	//Done

	//SimKAvg;    //47
	if (::Settings.m_EXCEL_CT_SETS[46] == '1')
	{
		if (CTExam->m_sk_ok)
		{
			SimKAvgD.Format(_T("%.10f"), CTExam->m_sk_av_dp);
			SimKAvgMM.Format(_T("%.10f"), 0.001 * CTExam->m_sk_av_um);
		}
		else
		{
			SimKAvgD = "";
			SimKAvgMM = "";
		}
	}
	else
	{
		SimKAvgD = "";
		SimKAvgMM = "";
	}
	//Done

	//SimKDel;    //48
	if (::Settings.m_EXCEL_CT_SETS[47] == '1')
	{
		if (CTExam->m_sk_ok)
		{
			SimKDelD.Format(_T("%.10f"), CTExam->m_sk_df_dp);
			SimKDelDGE.Format(_T("%.10f"), 0.001 * CTExam->m_sk_st_dg);
		}
		else
		{
			SimKDelD = "";
			SimKDelDGE = "";
		}
	}
	else
	{
		SimKDelD = "";
		SimKDelDGE = "";
	}
	//Done

	//IS;         //49
	if (::Settings.m_EXCEL_CT_SETS[48] == '1')
	{
		if (CTExam->m_is_ok) IS.Format(_T("%.10f"), CTExam->m_is_df_dp);
		else IS.Empty();
	}
	else  IS = "";
	//Done

	//AvePwr;     //50
	if (::Settings.m_EXCEL_CT_SETS[49] == '1')
	{
		for (int i = 0; i< 11; i++)
		{
			if (CTExam->m_zk_ok[i])
				AvePwr[i].Format(_T("%.10f"), CTExam->m_zk_av_dp[i]);
			else
				AvePwr[i].Empty();
		}
	}
	else
	{
		for (int i = 0; i< 11; i++) AvePwr[i] = "";
	}
	//Done

	//StpSemi1; //51
	if (::Settings.m_EXCEL_CT_SETS[50] == '1')
	{
		if (CTExam->m_kr_ok[0])
		{
			StpSemi1D.Format(_T("%.10f"), CTExam->m_kr_st_dp[0][0]);
			StpSemi1MM.Format(_T("%.10f"), 0.001 * CTExam->m_kr_st_um[0][0]);
			StpSemi1Deg.Format(_T("%i"), CTExam->m_kr_st_dg[0][0]);
		}
		else
		{
			StpSemi1D = "";
			StpSemi1MM = "";
			StpSemi1Deg = "";
		}
	}
	else
	{
		StpSemi1D = "";
		StpSemi1MM = "";
		StpSemi1Deg = "";
	}
	//Done

	//StpSemi2; //52
	if (::Settings.m_EXCEL_CT_SETS[51] == '1')
	{
		if (CTExam->m_kr_ok[0])
		{
			StpSemi2D.Format(_T("%.10f"), CTExam->m_kr_st_dp[0][1]);
			StpSemi2MM.Format(_T("%.10f"), 0.001 * CTExam->m_kr_st_um[0][1]);
			StpSemi2Deg.Format(_T("%i"), CTExam->m_kr_st_dg[0][1]);
		}
		else
		{
			StpSemi2D = "";
			StpSemi2MM = "";
			StpSemi2Deg = "";
		}
	}
	else
	{
		StpSemi2D = "";
		StpSemi2MM = "";
		StpSemi2Deg = "";
	}
	//Done

	//FltSemi1; //53
	if (::Settings.m_EXCEL_CT_SETS[52] == '1')
	{
		if (CTExam->m_kr_ok[0])
		{
			FltSemi1D.Format(_T("%.10f"), CTExam->m_kr_fl_dp[0][0]);
			FltSemi1MM.Format(_T("%.10f"), 0.001 * CTExam->m_kr_fl_um[0][0]);
			FltSemi1Deg.Format(_T("%i"), CTExam->m_kr_fl_dg[0][0]);
		}
		else
		{
			FltSemi1D = "";
			FltSemi1MM = "";
			FltSemi1Deg = "";
		}
	}
	else
	{
		FltSemi1D = "";
		FltSemi1MM = "";
		FltSemi1Deg = "";
	}
	//Done

	//FltSemi2; //54
	if (::Settings.m_EXCEL_CT_SETS[53] == '1')
	{
		if (CTExam->m_kr_ok[0])
		{
			FltSemi2D.Format(_T("%.10f"), CTExam->m_kr_fl_dp[0][1]);
			FltSemi2MM.Format(_T("%.10f"), 0.001 * CTExam->m_kr_fl_um[0][1]);
			FltSemi2Deg.Format(_T("%i"), CTExam->m_kr_fl_dg[0][1]);
		}
		else
		{
			FltSemi2D = "";
			FltSemi2MM = "";
			FltSemi2Deg = "";
		}
	}
	else
	{
		FltSemi2D = "";
		FltSemi2MM = "";
		FltSemi2Deg = "";
	}
	//Done

	//StpSemi12; //55
	if (::Settings.m_EXCEL_CT_SETS[54] == '1')
	{
		if (CTExam->m_kr_ok[1])
		{
			StpSemi12D.Format(_T("%.10f"), CTExam->m_kr_st_dp[1][0]);
			StpSemi12MM.Format(_T("%.10f"), 0.001 * CTExam->m_kr_st_um[1][0]);
			StpSemi12Deg.Format(_T("%i"), CTExam->m_kr_st_dg[1][0]);
		}
		else
		{
			StpSemi12D = "";
			StpSemi12MM = "";
			StpSemi12Deg = "";
		}
	}
	else
	{
		StpSemi12D = "";
		StpSemi12MM = "";
		StpSemi12Deg = "";
	}
	//Done

	//StpSemi22; //56
	if (::Settings.m_EXCEL_CT_SETS[55] == '1')
	{
		if (CTExam->m_kr_ok[1])
		{
			StpSemi22D.Format(_T("%.10f"), CTExam->m_kr_st_dp[1][1]);
			StpSemi22MM.Format(_T("%.10f"), 0.001 * CTExam->m_kr_st_um[1][1]);
			StpSemi22Deg.Format(_T("%i"), CTExam->m_kr_st_dg[1][1]);
		}
		else
		{
			StpSemi22D = "";
			StpSemi22MM = "";
			StpSemi22Deg = "";
		}
	}
	else
	{
		StpSemi22D = "";
		StpSemi22MM = "";
		StpSemi22Deg = "";
	}
	//Done

	//FltSemi12; //57
	if (::Settings.m_EXCEL_CT_SETS[56] == '1')
	{
		if (CTExam->m_kr_ok[1])
		{
			FltSemi12D.Format(_T("%.10f"), CTExam->m_kr_fl_dp[1][0]);
			FltSemi12MM.Format(_T("%.10f"), 0.001 * CTExam->m_kr_fl_um[1][0]);
			FltSemi12Deg.Format(_T("%i"), CTExam->m_kr_fl_dg[1][0]);
		}
		else
		{
			FltSemi12D = "";
			FltSemi12MM = "";
			FltSemi12Deg = "";
		}
	}
	else
	{
		FltSemi12D = "";
		FltSemi12MM = "";
		FltSemi12Deg = "";
	}
	//Done

	//FltSemi22; //58
	if (::Settings.m_EXCEL_CT_SETS[57] == '1')
	{
		if (CTExam->m_kr_ok[1])
		{
			FltSemi22D.Format(_T("%.10f"), CTExam->m_kr_fl_dp[1][1]);
			FltSemi22MM.Format(_T("%.10f"), 0.001 * CTExam->m_kr_fl_um[1][1]);
			FltSemi22Deg.Format(_T("%i"), CTExam->m_kr_fl_dg[1][1]);
		}
		else
		{
			FltSemi22D = "";
			FltSemi22MM = "";
			FltSemi22Deg = "";
		}
	}
	else
	{
		FltSemi22D = "";
		FltSemi22MM = "";
		FltSemi22Deg = "";
	}
	//Done

	//StpSemi13; //59
	if (::Settings.m_EXCEL_CT_SETS[58] == '1')
	{
		if (CTExam->m_kr_ok[2])
		{
			StpSemi13D.Format(_T("%.10f"), CTExam->m_kr_st_dp[2][0]);
			StpSemi13MM.Format(_T("%.10f"), 0.001 * CTExam->m_kr_st_um[2][0]);
			StpSemi13Deg.Format(_T("%i"), CTExam->m_kr_st_dg[2][0]);
		}
		else
		{
			StpSemi13D = "";
			StpSemi13MM = "";
			StpSemi13Deg = "";
		}
	}
	else
	{
		StpSemi13D = "";
		StpSemi13MM = "";
		StpSemi13Deg = "";
	}
	//Done

	//StpSemi23; //60
	if (::Settings.m_EXCEL_CT_SETS[59] == '1')
	{
		if (CTExam->m_kr_ok[2])
		{
			StpSemi23D.Format(_T("%.10f"), CTExam->m_kr_st_dp[2][1]);
			StpSemi23MM.Format(_T("%.10f"), 0.001 * CTExam->m_kr_st_um[2][1]);
			StpSemi23Deg.Format(_T("%i"), CTExam->m_kr_st_dg[2][1]);
		}
		else
		{
			StpSemi23D = "";
			StpSemi23MM = "";
			StpSemi23Deg = "";
		}
	}
	else
	{
		StpSemi23D = "";
		StpSemi23MM = "";
		StpSemi23Deg = "";
	}
	//Done

	//FltSemi13; //61
	if (::Settings.m_EXCEL_CT_SETS[60] == '1')
	{
		if (CTExam->m_kr_ok[2])
		{
			FltSemi13D.Format(_T("%.10f"), CTExam->m_kr_fl_dp[2][0]);
			FltSemi13MM.Format(_T("%.10f"), 0.001 * CTExam->m_kr_fl_um[2][0]);
			FltSemi13Deg.Format(_T("%i"), CTExam->m_kr_fl_dg[2][0]);
		}
		else
		{
			FltSemi13D = "";
			FltSemi13MM = "";
			FltSemi13Deg = "";
		}
	}
	else
	{
		FltSemi13D = "";
		FltSemi13MM = "";
		FltSemi13Deg = "";
	}
	//Done

	//FltSemi23; //62
	if (::Settings.m_EXCEL_CT_SETS[61] == '1')
	{
		if (CTExam->m_kr_ok[2])
		{
			FltSemi23D.Format(_T("%.10f"), CTExam->m_kr_fl_dp[2][1]);
			FltSemi23MM.Format(_T("%.10f"), 0.001 * CTExam->m_kr_fl_um[2][1]);
			FltSemi23Deg.Format(_T("%i"), CTExam->m_kr_fl_dg[2][1]);
		}
		else
		{
			FltSemi23D = "";
			FltSemi23MM = "";
			FltSemi23Deg = "";
		}
	}
	else
	{
		FltSemi23D = "";
		FltSemi23MM = "";
		FltSemi23Deg = "";
	}
	//Done

	//CornealSph; //63
	if (::Settings.m_EXCEL_CT_SETS[62] == '1')
	{
		CornealSph.Format(_T("%.10f"), CTExam->m_sph_6mm);
	}
	else  CornealSph = "";
	//Done

	CZernikeSurface Surface[5];
	for (int i = 0; i < 4; i++) Surface[i] = CTExam->m_WfSurface;  //
	real_t ZoneRum[4];

	//CornealZer1; //64
	if (::Settings.m_EXCEL_CT_SETS[63] == '1')
	{
		ZoneRum[0] = 0.5 * ::Settings.m_ZoneDiameterUm[0];

		if (ZoneRum[0] <= CTExam->m_WfSurface.m_r_max_um)
		{
			Surface[0].ChangeRMaxUm(ZoneRum[0]);
			for (int i = 0; i < 45; i++)
			{
				real_t um = Surface[0].GetCUm(i);
				//um = fabs(um);
				CornealZer1[i].Format(_T("%6.10f"), um);
			}
		}
	}
	else
	{
		{ for (int i = 0; i < 45; i++)	CornealZer1[i] = ""; }
	}
	//Done

	//CornealZer2; //65
	if (::Settings.m_EXCEL_CT_SETS[64] == '1')
	{
		ZoneRum[1] = 0.5 * ::Settings.m_ZoneDiameterUm[1];
		if (ZoneRum[1] <= CTExam->m_WfSurface.m_r_max_um)
		{
			Surface[1].ChangeRMaxUm(ZoneRum[1]);
			for (int i = 0; i < 45; i++)
			{
				real_t um = Surface[1].GetCUm(i);
				//um = fabs(um);
				CornealZer2[i].Format(_T("%6.10f"), um);
			}
		}
	}
	else { for (int i = 0; i < 45; i++)	CornealZer2[i] = ""; }
	//Done

	//CornealZer3; //66
	if (::Settings.m_EXCEL_CT_SETS[65] == '1')
	{
		ZoneRum[2] = 0.5 * ::Settings.m_ZoneDiameterUm[2];

		if (ZoneRum[2] <= CTExam->m_WfSurface.m_r_max_um)
		{
			Surface[2].ChangeRMaxUm(ZoneRum[2]);
			for (int i = 0; i < 45; i++)
			{
				real_t um = Surface[2].GetCUm(i);
				//um = fabs(um);
				CornealZer3[i].Format(_T("%6.10f"), um);
			}
		}
	}
	else { for (int i = 0; i < 45; i++)	CornealZer3[i] = ""; }
	//Done

	//CornealZerF; //67
	if (::Settings.m_EXCEL_CT_SETS[66] == '1')
	{
		ZoneRum[3] = CTExam->m_WfSurface.m_r_max_um;

		if (ZoneRum[3] <= CTExam->m_WfSurface.m_r_max_um)
		{
			Surface[3].ChangeRMaxUm(ZoneRum[3]);
			for (int i = 0; i < 45; i++)
			{
				real_t um = Surface[3].GetCUm(i);
				//um = fabs(um);
				CornealZerF[i].Format(_T("%6.10f"), um);
			}
		}
	}
	else { for (int i = 0; i < 45; i++)	CornealZerF[i] = ""; }
	//Done

	//Done



	//Save the exam info into excel file

	//Following add the exam info into the excel file
	Range range;

	Workbooks books;
	_Workbook book;

	Worksheets sheets;
	_Worksheet sheet;

	LPDISPATCH lpDisp;
	//COleVariant vResult;

	COleVariant  covTrue((short)TRUE), covFalse((short)FALSE), covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	excelApp.SetVisible(FALSE);
	excelApp.SetUserControl(TRUE);

	books.AttachDispatch(excelApp.GetWorkbooks());

	long iStartRow = 1;
	BOOL NewExcel = FALSE;

	if (!::PathFileExists(FileName))
	{
		NewExcel = TRUE;
		books = excelApp.GetWorkbooks();
		book = books.Add(covOptional);
		sheets = book.GetSheets();
		sheet = sheets.GetItem(COleVariant((short)1));
		sheet.SetName(_T("CT Exam Report"));
	}
	else
	{
		try
		{
			lpDisp = books.Open(FileName,
				covOptional, covOptional, covOptional, covOptional, covOptional,
				covOptional, covOptional, covOptional, covOptional, covOptional,
				covOptional, covOptional);


			//Get work book
			book.AttachDispatch(lpDisp);
			//Get work sheets
			sheets.AttachDispatch(book.GetWorksheets());

			lpDisp = book.GetActiveSheet();
			sheet.AttachDispatch(lpDisp);
		}
		catch (...)
		{
			books.Close();
			excelApp.Quit();
			::CoUninitialize();

			::Info("Excel file open failed");
			return -3;
		}
	}

	//Used info
	if (!NewExcel)
	{
		Range usedRange;
		usedRange.AttachDispatch(sheet.GetUsedRange());
		range.AttachDispatch(usedRange.GetRows());
		long iRowNum = range.GetCount();   //Used Row
		iStartRow = iRowNum + 1;     //Started row
		range.AttachDispatch(sheet.GetCells(), TRUE);
	}
	else
	{
		//add Title
		CString StrPos[303] = {
			"A1", "B1", "C1", "D1", "E1", "F1", "G1",
			"H1", "I1", "J1", "K1", "L1", "M1", "N1",
			"O1", "P1", "Q1", "R1", "S1", "T1", "U1",
			"V1", "W1", "X1", "Y1", "Z1",
			"AA1", "AB1", "AC1", "AD1", "AE1", "AF1", "AG1",
			"AH1", "AI1", "AJ1", "AK1", "AL1", "AM1", "AN1",
			"AO1", "AP1", "AQ1", "AR1", "AS1", "AT1", "AU1",
			"AV1", "AW1", "AX1", "AY1", "AZ1",
			"BA1", "BB1", "BC1", "BD1", "BE1", "BF1", "BG1",
			"BH1", "BI1", "BJ1", "BK1", "BL1", "BM1", "BN1",
			"BO1", "BP1", "BQ1", "BR1", "BS1", "BT1", "BU1",
			"BV1", "BW1", "BX1", "BY1", "BZ1",
			"CA1", "CB1", "CC1", "CD1", "CE1", "CF1", "CG1",
			"CH1", "CI1", "CJ1", "CK1", "CL1", "CM1", "CN1",
			"CO1", "CP1", "CQ1", "CR1", "CS1", "CT1", "CU1",
			"CV1", "CW1", "CX1", "CY1", "CZ1",
			"DA1", "DB1", "DC1", "DD1", "DE1", "DF1", "DG1",
			"DH1", "DI1", "DJ1", "DK1", "DL1", "DM1", "DN1",
			"DO1", "DP1", "DQ1", "DR1", "DS1", "DT1", "DU1",//DO1
			"DV1", "DW1", "DX1", "DY1", "DZ1",
			"EA1", "EB1", "EC1", "ED1", "EE1", "EF1", "EG1",
			"EH1", "EI1", "EJ1", "EK1", "EL1", "EM1", "EN1",
			"EO1", "EP1", "EQ1", "ER1", "ES1", "ET1", "EU1",
			"EV1", "EW1", "EX1", "EY1", "EZ1",
			"FA1", "FB1", "FC1", "FD1", "FE1", "FF1", "FG1",
			"FH1", "FI1", "FJ1", "FK1", "FL1", "FM1", "FN1",
			"FO1", "FP1", "FQ1", "FR1", "FS1", "FT1", "FU1",
			"FV1", "FW1", "FX1", "FY1", "FZ1",
			"GA1", "GB1", "GC1", "GD1", "GE1", "GF1", "GG1",
			"GH1", "GI1", "GJ1", "GK1", "GL1", "GM1", "GN1",
			"GO1", "GP1", "GQ1", "GR1", "GS1", "GT1", "GU1",
			"GV1", "GW1", "GX1", "GY1", "GZ1",
			"HA1", "HB1", "HC1", "HD1", "HE1", "HF1", "HG1",
			"HH1", "HI1", "HJ1", "HK1", "HL1", "HM1", "HN1",
			"HO1", "HP1", "HQ1", "HR1", "HS1", "HT1", "HU1",
			"HV1", "HW1", "HX1", "HY1", "HZ1",
			"IA1", "IB1", "IC1", "ID1", "IE1", "IF1", "IG1",
			"IH1", "II1", "IJ1", "IK1", "IL1", "IM1", "IN1",
			"IO1", "IP1", "IQ1", "IR1", "IS1", "IT1", "IU1",
			"IV1", "IW1", "IX1", "IY1", "IZ1",
			"JA1", "JB1", "JC1", "JD1", "JE1", "JF1", "JG1",
			"JH1", "JI1", "JJ1", "JK1", "JL1", "JM1", "JN1",
			"JO1", "JP1", "JQ1", "JR1", "JS1", "JT1", "JU1",
			"JV1", "JW1", "JX1", "JY1", "JZ1",
			"KA1", "KB1", "KC1", "KD1", "KE1", "KF1", "KG1",
			"KH1", "KI1", "KJ1", "KK1", "KL1", "KM1", "KN1",
			"KO1", "KP1", "KQ1"
		};
		int WidthPos[303] = {
			16, 40, 12, 12, 12, 12, 12, // a, b, c, d, e, f, g
			12, 12, 12, 12, 12, 12, 12, // h, i, j, k, l, m, n
			12, 12, 12, 40, 40, 12, 40, // o, p, q, r, s, t, u
			40, 20, 12, 12, 12,         // v, w, x, y, z

			12, 12, 12, 40, 12, 12, 12,//A
			16, 16, 16, 16, 40, 20, 20,
			40, 20, 20, 16, 16, 16, 16,
			16, 18, 18, 20, 20,

			20, 20, 22, 18, 22, 14, 20,//B
			24, 12, 12, 12, 12, 12, 12,
			12, 12, 18, 14, 18, 18, 18,
			18, 18, 18, 18, 18,

			18, 18, 18, 18, 18, 18, 18,//C
			18, 18, 18, 18, 18, 18, 18,
			18, 18, 18, 18, 18, 18, 18,
			19, 18, 18, 18, 18,

			18, 18, 18, 18, 18, 18, 18,//D
			18, 18, 18, 18, 18, 18, 18,
			20, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12,//E
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12,//F
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12,//G
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12,//H
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12,//I
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12,//J
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12,//K
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12
		};
		for (int i = 0; i < 303; i++)
		{
			range = sheet.GetRange(COleVariant(StrPos[i]), COleVariant(StrPos[i]));
			range.SetColumnWidth(COleVariant((long)WidthPos[i]));

			//Cell Font
			Font font;
			font.AttachDispatch(range.GetFont());
			font.SetBold(covTrue);
			font.SetColor(COleVariant((long)0xFF0000));
			range.SetHorizontalAlignment(COleVariant((short)-4108)); //Center
			range.SetVerticalAlignment(COleVariant((short)-4108)); //Center
																   //Done

																   //Background Color
			Interior it;
			it.AttachDispatch(range.GetInterior());
			if (i < 20)   it.SetColorIndex(COleVariant((long)20));
			else         it.SetColorIndex(COleVariant((long)40));

			if (i == 119 || i == 165 || i == 211 || i == 257)
				it.SetColorIndex(COleVariant((long)33));
			//Done

			//Border
			Borders border;
			border = range.GetBorders();
			border.SetColorIndex(COleVariant((long)1));
			border.SetLineStyle(COleVariant((long)1));
			border.SetWeight(COleVariant((long)4));
			//Done
		}

		//Font font;
		range.AttachDispatch(sheet.GetCells(), TRUE);
		//font.AttachDispatch (range.GetFont ());
		//font.SetColor(COleVariant((long)0xFF0000));

		//Font ft;
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)1), COleVariant(_T("Export Time")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)2), COleVariant(_T("Patient Unique ID")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)3), COleVariant(_T("ID")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)4), COleVariant(_T("First Name")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)5), COleVariant(_T("Middle Name")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)6), COleVariant(_T("Last Name")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)7), COleVariant(_T("DOB")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)8), COleVariant(_T("Gender")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)9), COleVariant(_T("Add Line1")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)10), COleVariant(_T("Add Line2")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)11), COleVariant(_T("City")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)12), COleVariant(_T("State")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)13), COleVariant(_T("Zip")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)14), COleVariant(_T("Country")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)15), COleVariant(_T("Work Phone")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)16), COleVariant(_T("Home Phone")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)17), COleVariant(_T("Email")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)18), COleVariant(_T("Note")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)19), COleVariant(_T("Group GUID")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)20), COleVariant(_T("Group Name")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)21), COleVariant(_T("Exam Unique ID")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)22), COleVariant(_T("Calibration Unique ID")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)23), COleVariant(_T("Exam Type")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)24), COleVariant(_T("Mode")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)25), COleVariant(_T("Exam Date")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)26), COleVariant(_T("Exam Time")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)27), COleVariant(_T("Eye")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)28), COleVariant(_T("Pre/Post")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)29), COleVariant(_T("SW")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)30), COleVariant(_T("Clinic Unique ID")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)31), COleVariant(_T("Clinic Name")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)32), COleVariant(_T("Clinic Add1")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)33), COleVariant(_T("Clinic Add2")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)34), COleVariant(_T("Clinic City")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)35), COleVariant(_T("Clinic State")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)36), COleVariant(_T("Clinic Zip")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)37), COleVariant(_T("Clinic Country")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)38), COleVariant(_T("Physician ID")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)39), COleVariant(_T("Physician First Name")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)40), COleVariant(_T("Physician Last Name")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)41), COleVariant(_T("Operator ID")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)42), COleVariant(_T("Operator First Name")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)43), COleVariant(_T("Operator Last Name")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)44), COleVariant(_T("Exam Note")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)45), COleVariant(_T("Limbal Center X")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)46), COleVariant(_T("Limbal Center Y")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)47), COleVariant(_T("Limbal Diameter")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)48), COleVariant(_T("CentralPwr D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)49), COleVariant(_T("CentralPwr mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)50), COleVariant(_T("BestFit Sph Rad")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)51), COleVariant(_T("BestFit Con-CentralRad")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)52), COleVariant(_T("BestFit Con-Eccentricity")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)53), COleVariant(_T("BestFit Con-Asph")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)54), COleVariant(_T("RefPwr at <3mm Stp D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)55), COleVariant(_T("RefPwr at <3mm Stp Axis")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)56), COleVariant(_T("RefPwr at <3mm Flt D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)57), COleVariant(_T("RefPwr at <3mm Flt Axis")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)58), COleVariant(_T("Eff Ref Pwr")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)59), COleVariant(_T("RefPwr at <3mm Astig D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)60), COleVariant(_T("RefPwr at <3mm Astig Axis1")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)61), COleVariant(_T("SimK Stp D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)62), COleVariant(_T("SimK Stp mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)63), COleVariant(_T("SimK Stp Deg")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)64), COleVariant(_T("SimK Flt D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)65), COleVariant(_T("SimK Flt mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)66), COleVariant(_T("SimK Flt Deg")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)67), COleVariant(_T("SimK Avg D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)68), COleVariant(_T("SimK Avg mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)69), COleVariant(_T("SimK Delta D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)70), COleVariant(_T("SimK Delta Deg")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)71), COleVariant(_T("I-S ")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)72), COleVariant(_T("AvgPwer at 0mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)73), COleVariant(_T("AvgPwer at 1mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)74), COleVariant(_T("AvgPwer at 2mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)75), COleVariant(_T("AvgPwer at 2mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)76), COleVariant(_T("AvgPwer at 3mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)77), COleVariant(_T("AvgPwer at 4mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)78), COleVariant(_T("AvgPwer at 5mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)79), COleVariant(_T("AvgPwer at 6mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)80), COleVariant(_T("AvgPwer at 7mm")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)81), COleVariant(_T("AvgPwer at 8mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)82), COleVariant(_T("AvgPwer at 10mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)83), COleVariant(_T("Stp Semi1 at <3 D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)84), COleVariant(_T("Stp Semi1 at <3 mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)85), COleVariant(_T("Stp Semi1 at <3 Deg")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)86), COleVariant(_T("Stp Semi2 at <3 D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)87), COleVariant(_T("Stp Semi2 at <3 mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)88), COleVariant(_T("Stp Semi2 at <3 Deg")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)89), COleVariant(_T("Flt Semi1 at <3 D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)90), COleVariant(_T("Flt Semi1 at <3 mm")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)91), COleVariant(_T("Flt Semi1 at <3 Deg")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)92), COleVariant(_T("Flt Semi2 at <3 D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)93), COleVariant(_T("Flt Semi2 at <3 mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)94), COleVariant(_T("Flt Semi2 at <3 Deg")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)95), COleVariant(_T("Stp Semi1 @ 3 - 5 D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)96), COleVariant(_T("Stp Semi1 @ 3 - 5 mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)97), COleVariant(_T("Stp Semi1 @ 3 - 5 Deg")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)98), COleVariant(_T("Stp Semi2 @ 3 - 5 D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)99), COleVariant(_T("Stp Semi2 @ 3 - 5 mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)100), COleVariant(_T("Stp Semi2 @ 3 - 5 Deg")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)101), COleVariant(_T("Flt Semi1 @ 3 - 5 D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)102), COleVariant(_T("Flt Semi1 @ 3 - 5 mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)103), COleVariant(_T("Flt Semi1 @ 3 - 5 Deg")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)104), COleVariant(_T("Flt Semi2 @ 3 - 5 D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)105), COleVariant(_T("Flt Semi2 @ 3 - 5 mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)106), COleVariant(_T("Flt Semi2 @ 3 - 5 Deg")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)107), COleVariant(_T("Stp Semi1 @ 5-7 D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)108), COleVariant(_T("Stp Semi1 @ 5-7 mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)109), COleVariant(_T("Stp Semi1 @ 5-7 Deg")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)110), COleVariant(_T("Stp Semi2 @ 5-7 D")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)111), COleVariant(_T("Stp Semi2 @ 5-7 mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)112), COleVariant(_T("Stp Semi2 @5-7 Deg")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)113), COleVariant(_T("Flt Semi1 @ 5-7 D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)114), COleVariant(_T("Flt Semi1 @ 5-7 mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)115), COleVariant(_T("Flt Semi1 @ 5-7 Deg")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)116), COleVariant(_T("Flt Semi2 @ 5-7 D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)117), COleVariant(_T("Flt Semi2 @ 5-7 mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)118), COleVariant(_T("Flt Semi2 @ 5-7 Deg")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)119), COleVariant(_T("Corneal SphAberr@6mm")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)120), COleVariant(_T("CZ1 Zone")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)166), COleVariant(_T("CZ2 Zone")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)212), COleVariant(_T("CZ3 Zone")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)258), COleVariant(_T("CZ4 Zone")));

		CString s, s1, s2, s3, s4;
		for (int i = 0; i < 45; i++)
		{
			s.Format(_T("%i"), i);
			s1 = "CZ1 C" + s;
			s2 = "CZ2 C" + s;
			s3 = "CZ3 C" + s;
			s4 = "CZ4 C" + s;
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)121 + i), COleVariant(s1));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)167 + i), COleVariant(s2));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)213 + i), COleVariant(s3));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)259 + i), COleVariant(s4));
		}

		iStartRow = 2;
	}

	//Save Data
	{
		CString s1, s2, s3, s4;

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)1), COleVariant(RecordTime));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)2), COleVariant(PaUniqueID));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)3), COleVariant(UserID));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)4), COleVariant(FName));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)5), COleVariant(MName));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)6), COleVariant(LName));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)7), COleVariant(DOB));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)8), COleVariant(Gender));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)9), COleVariant(AddLine1));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)10), COleVariant(AddLine2));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)11), COleVariant(City));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)12), COleVariant(State));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)13), COleVariant(Zip));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)14), COleVariant(Country));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)15), COleVariant(WPhone));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)16), COleVariant(HPhone));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)17), COleVariant(Email));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)18), COleVariant(Note));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)19), COleVariant(GroupGUID));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)20), COleVariant(GroupName));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)21), COleVariant(ExUniqueID));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)22), COleVariant(CalID));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)23), COleVariant(ExamType));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)24), COleVariant(Mode));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)25), COleVariant(Date));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)26), COleVariant(Time));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)27), COleVariant(Eye));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)28), COleVariant(PrePost));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)29), COleVariant(SW));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)30), COleVariant(ClinicUniID));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)31), COleVariant(ClinicName));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)32), COleVariant(ClinicAdd1));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)33), COleVariant(ClinicAdd2));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)34), COleVariant(ClinicCity));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)35), COleVariant(ClinicState));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)36), COleVariant(ClinicZip));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)37), COleVariant(ClinicCountry));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)38), COleVariant(PhysicianID));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)39), COleVariant(PhysFName));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)40), COleVariant(PhysLName));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)41), COleVariant(OperatorID));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)42), COleVariant(OperFName));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)43), COleVariant(OperLName));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)44), COleVariant(ExamNote));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)45), COleVariant(LimbalX));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)46), COleVariant(LimbalY));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)47), COleVariant(LimbalD));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)48), COleVariant(CentralPwrD));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)49), COleVariant(CentralPwrMM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)50), COleVariant(BestFitSph));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)51), COleVariant(BestConCen));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)52), COleVariant(BestConEcc));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)53), COleVariant(BestConAsp));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)54), COleVariant(RefPwrStpD));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)55), COleVariant(RefPwrStpAxis));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)56), COleVariant(RefPwrFltD));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)57), COleVariant(RefPwrFltAxis));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)58), COleVariant(EffRef));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)59), COleVariant(RefPwrAstD));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)60), COleVariant(RefPwrAstAxis));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)61), COleVariant(SimKStpD));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)62), COleVariant(SimKStpMM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)63), COleVariant(SimKStpDeg));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)64), COleVariant(SimKFltD));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)65), COleVariant(SimKFltMM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)66), COleVariant(SimKFltDeg));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)67), COleVariant(SimKAvgD));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)68), COleVariant(SimKAvgMM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)69), COleVariant(SimKDelD));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)70), COleVariant(SimKDelDGE));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)71), COleVariant(IS));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)72), COleVariant(AvePwr[0]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)73), COleVariant(AvePwr[1]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)74), COleVariant(AvePwr[2]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)75), COleVariant(AvePwr[3]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)76), COleVariant(AvePwr[4]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)77), COleVariant(AvePwr[5]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)78), COleVariant(AvePwr[6]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)79), COleVariant(AvePwr[7]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)80), COleVariant(AvePwr[8]));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)81), COleVariant(AvePwr[9]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)82), COleVariant(AvePwr[10]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)83), COleVariant(StpSemi1D));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)84), COleVariant(StpSemi1MM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)85), COleVariant(StpSemi1Deg));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)86), COleVariant(StpSemi2D));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)87), COleVariant(StpSemi2MM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)88), COleVariant(StpSemi2Deg));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)89), COleVariant(FltSemi1D));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)90), COleVariant(FltSemi1MM));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)91), COleVariant(FltSemi1Deg));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)92), COleVariant(FltSemi2D));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)93), COleVariant(FltSemi2MM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)94), COleVariant(FltSemi2Deg));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)95), COleVariant(StpSemi12D));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)96), COleVariant(StpSemi12MM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)97), COleVariant(StpSemi12Deg));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)98), COleVariant(StpSemi22D));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)99), COleVariant(StpSemi22MM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)100), COleVariant(StpSemi22Deg));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)101), COleVariant(FltSemi12D));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)102), COleVariant(FltSemi12MM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)103), COleVariant(FltSemi12Deg));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)104), COleVariant(FltSemi22D));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)105), COleVariant(FltSemi22MM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)106), COleVariant(FltSemi22Deg));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)107), COleVariant(StpSemi13D));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)108), COleVariant(StpSemi13MM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)109), COleVariant(StpSemi13Deg));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)110), COleVariant(StpSemi23D));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)111), COleVariant(StpSemi23MM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)112), COleVariant(StpSemi23Deg));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)113), COleVariant(FltSemi13D));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)114), COleVariant(FltSemi13MM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)115), COleVariant(FltSemi13Deg));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)116), COleVariant(FltSemi23D));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)117), COleVariant(FltSemi23MM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)118), COleVariant(FltSemi23Deg));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)119), COleVariant(CornealSph));

		s1.Format(_T("%.1f"), (real_t)::Settings.m_ZoneDiameterUm[0] / 1000);
		s2.Format(_T("%.1f"), (real_t)::Settings.m_ZoneDiameterUm[1] / 1000);
		s3.Format(_T("%.1f"), (real_t)::Settings.m_ZoneDiameterUm[2] / 1000);
		s4.Format(_T("%.1f"), (real_t)CTExam->m_WfSurface.m_r_max_um * 2 / 1000);

		if (::Settings.m_EXCEL_CT_SETS[63] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)120), COleVariant(s1));

		if (::Settings.m_EXCEL_CT_SETS[64] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)166), COleVariant(s2));

		if (::Settings.m_EXCEL_CT_SETS[65] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)212), COleVariant(s3));

		if (::Settings.m_EXCEL_CT_SETS[66] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)258), COleVariant(s4));

		for (int i = 0; i < 45; i++)
		{
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)121 + i), COleVariant(CornealZer1[i]));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)167 + i), COleVariant(CornealZer2[i]));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)213 + i), COleVariant(CornealZer3[i]));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)259 + i), COleVariant(CornealZerF[i]));
		}
	}

	if (!NewExcel)
	{
		book.Save();
		book.SetSaved(TRUE);
	}
	else
	{
		book.SaveAs(COleVariant(FileName),
			covOptional, covOptional, covOptional, covOptional,
			covOptional, 0, covOptional, covOptional,
			covOptional, covOptional);
	}

	//release
	range.ReleaseDispatch();
	sheet.ReleaseDispatch();
	sheets.ReleaseDispatch();
	book.ReleaseDispatch();
	books.ReleaseDispatch();
	excelApp.Quit();
	excelApp.ReleaseDispatch();
	::CoUninitialize();
	return 0;
	//Done
}

//********************************************************************************************************

int CMainWnd::SaveWFCTExamToExcel(CWFExam *WFExam, CCTExam *CTExam, CString FileName)
{
	CPatient* pPatient = ::DB.GetPatient(WFExam->m_Header.m_PatientID);
	if (!pPatient) {
		::Error("No patient associated with the exam.");
		return -1;
	}

	HRESULT hr;

	hr = ::CoInitialize(NULL);

	if (FAILED(hr))
	{
		::Info("WF/CT co error");
	}

	_Application excelApp;

	if (!excelApp.CreateDispatch(_T("Excel.Application"), NULL))
	{
		::Info("Excel application failed.");
		::Info("Please install Excel on your computer for Excel export.");
		::CoUninitialize();
		::Settings.m_Export_Excel = FALSE;
		return -2;
	}

	CString RecordTime;
	CString PaUniqueID, ExUniqueID[2], ExamType[2];

	CString UserID;  //1
	CString FName;   //2
	CString MName;   //3
	CString LName;   //4
	CString DOB;     //5
	CString Gender;  //6
	CString AddLine1;//7
	CString AddLine2;//8
	CString City;    //9
	CString State;   //10

	CString Zip;      //11
	CString Country;  //12
	CString WPhone;   //13
	CString HPhone;   //14
	CString Email;    //15
	CString Note;     //16
	CString GroupGUID;//17
	CString GroupName;//18
	CString CalID[2];    //19
	CString Mode[2];     //20(Exam Type)

	CString Date[2];       //21
	CString Time[2];       //22
	CString Eye[2];        //23
	CString PrePost[2];    //24
	CString SW[2];         //25

	CString ClinicUniID[2]; //89
	CString ClinicName[2];  //89
	CString ClinicAdd1[2]; //89
	CString ClinicAdd2[2]; //89
	CString ClinicCity[2]; //89
	CString ClinicState[2]; //89
	CString ClinicZip[2];  //89
	CString ClinicCountry[2];  //89

	CString PhysicianID[2];//26
	CString PhysFName[2];  //27
	CString PhysLName[2];  //28
	CString OperatorID[2]; //29
	CString OperFName[2];  //30

	CString OperLName[2];  //31
	CString ExamNote[2];   //32

						   //WF Exam and Some CT Exam '[2]'
	CString AngelKDist; //33
	CString AngelKDeg;  //34
	CString AngelADist; //35
	CString AngelADeg;  //36
	CString LimbalX[2]; //37
	CString LimbalY[2]; //38
	CString LimbalD[2]; //39
	CString PupilX;     //40

	CString PupilY;     //41
	CString PupilD;     //42
	CString ScanD;      //43

	CString TraceRefSph; //44
	CString TraceRefCyl; //44
	CString TraceRefAxis;//44
	CString TraceRefVd;  //44

	CString Ref1ZoneSph;  //45
	CString Ref1ZoneCyl;  //45
	CString Ref1ZoneAxis; //45
	CString Ref1ZoneVd;   //45

	CString Ref2ZoneSph;  //46
	CString Ref2ZoneCyl;  //46
	CString Ref2ZoneAxis; //46
	CString Ref2ZoneVd;   //46

	CString Ref3ZoneSph;  //47
	CString Ref3ZoneCyl;  //47
	CString Ref3ZoneAxis; //47
	CString Ref3ZoneVd;   //47

	CString Ref4ZoneSph;  //48
	CString Ref4ZoneCyl;  //48
	CString Ref4ZoneAxis; //48
	CString Ref4ZoneVd;   //48

	CString EntireEyeZC1[45]; //49
	CString EntireEyeZC2[45]; //50
	CString EntireEyeZC3[45]; //51
	CString EntireEyeZCF[45]; //52

	CString WFCTOpacityVal; //53
	//Done


	//CTEXAM
	CString CentralPwrD;  //53
	CString CentralPwrMM; //54

	CString BestFitSph; //55
	CString BestConCen; //56
	CString BestConEcc; //57
	CString BestConAsp; //58

	CString RefPwrStpD;     //59
	CString RefPwrStpAxis;  //60

	CString RefPwrFltD;     //61
	CString RefPwrFltAxis;  //62

	CString EffRef;     //63

	CString RefPwrAstD;     //64
	CString RefPwrAstAxis;  //64

	CString SimKStpD;    //65
	CString SimKStpMM;   //65
	CString SimKStpDeg;  //65

	CString SimKFltD;    //66
	CString SimKFltMM;   //66
	CString SimKFltDeg;  //66

	CString SimKAvgD;    //67
	CString SimKAvgMM;   //67

	CString SimKDelD;    //68
	CString SimKDelDGE;  //68

	CString IS;         //69

	CString AvePwr[11]; //70

	CString StpSemi1D;   //71
	CString StpSemi1MM;  //71
	CString StpSemi1Deg; //71

	CString StpSemi2D;   //72
	CString StpSemi2MM;  //72
	CString StpSemi2Deg; //72

	CString FltSemi1D;   //73
	CString FltSemi1MM;  //73
	CString FltSemi1Deg; //79

	CString FltSemi2D;   //84
	CString FltSemi2MM;  //84
	CString FltSemi2Deg; //84

	CString StpSemi12D;   //85
	CString StpSemi12MM;  //85
	CString StpSemi12Deg; //85

	CString StpSemi22D;   //86
	CString StpSemi22MM;  //86
	CString StpSemi22Deg; //86

	CString FltSemi12D;   //87
	CString FltSemi12MM;  //87
	CString FltSemi12Deg; //87

	CString FltSemi22D;   //88
	CString FltSemi22MM;  //88
	CString FltSemi22Deg; //88

	CString StpSemi13D;   //89
	CString StpSemi13MM;  //89
	CString StpSemi13Deg; //89

	CString StpSemi23D;   //90
	CString StpSemi23MM;  //90
	CString StpSemi23Deg; //90

	CString FltSemi13D;   //91
	CString FltSemi13MM;  //91
	CString FltSemi13Deg; //91

	CString FltSemi23D;   //92
	CString FltSemi23MM;  //92
	CString FltSemi23Deg; //92

	CString CornealSph; //93

	CString CornealZer1[45]; //94
	CString CornealZer2[45]; //95
	CString CornealZer3[45]; //96
	CString CornealZerF[45]; //97
							 //

							 //Comom attributes
	CString InternalZer1[45]; //98
	CString InternalZer2[45]; //98
	CString InternalZer3[45]; //100
	CString InternalZerF[45]; //101
							  //Done

							  //ExportScreen

							  //Time
	SYSTEMTIME CurTime;
	::GetLocalTime(&CurTime);
	RecordTime.Format(_T("%02i-%02i-%04i %02i:%02i"), CurTime.wMonth, CurTime.wDay, CurTime.wYear, CurTime.wHour, CurTime.wMinute);
	//

	//PaUniqueID
	GUIDToStr(WFExam->m_Header.m_PatientID, PaUniqueID);
	//Done

	//ExUniqueID
	GUIDToStr(WFExam->m_Header.m_ExamID, ExUniqueID[0]);
	GUIDToStr(CTExam->m_Header.m_ExamID, ExUniqueID[1]);
	//Done

	//ExamType
	ExamType[0] = WFExam->m_Header.Type();
	ExamType[1] = CTExam->m_Header.Type();
	//Done

	//UserID;  //1
	if (::Settings.m_EXCEL_WFCT_SETS[0] == '1')
	{
		UserID = pPatient->m_CustomID;
	}
	else UserID = "";
	//Done

	//FName;   //2
	if (::Settings.m_EXCEL_WFCT_SETS[1] == '1')
	{
		FName = pPatient->m_FirstName;
	}
	else FName = "";
	//Done

	//MName;   //3
	if (::Settings.m_EXCEL_WFCT_SETS[2] == '1')
	{
		MName = pPatient->m_MiddleName;
	}
	else MName = "";
	//Done

	//LName;   //4
	if (::Settings.m_EXCEL_WFCT_SETS[3] == '1')
	{
		LName = pPatient->m_LastName;
	}
	else LName = "";
	//Done

	//DOB;     //5
	if (::Settings.m_EXCEL_WFCT_SETS[4] == '1')
	{
		DOB.Format(_T("%02i-%02i-%04i"), pPatient->m_BirthMonth, pPatient->m_BirthDay, pPatient->m_BirthYear);
	}
	else DOB = "";
	//Done

	//Gender;  //6
	if (::Settings.m_EXCEL_WFCT_SETS[5] == '1')
	{
		if (pPatient->m_Sex == 1)
			Gender = "M";
		else Gender = "F";
	}
	else Gender = "";
	//Done

	//AddLine1;//7
	if (::Settings.m_EXCEL_WFCT_SETS[6] == '1')
	{
		AddLine1 = pPatient->m_Address.m_Line1;
	}
	else AddLine1 = "";
	//Done

	//AddLine2;//8
	if (::Settings.m_EXCEL_WFCT_SETS[7] == '1')
	{
		AddLine2 = pPatient->m_Address.m_Line2;
	}
	else AddLine2 = "";
	//Done

	//City;    //9
	if (::Settings.m_EXCEL_WFCT_SETS[8] == '1')
	{
		City = pPatient->m_Address.m_City;
	}
	else City = "";
	//Done

	//State;   //10
	if (::Settings.m_EXCEL_WFCT_SETS[9] == '1')
	{
		State = pPatient->m_Address.m_State;
	}
	else State = "";
	//Done

	//Zip;      //11
	if (::Settings.m_EXCEL_WFCT_SETS[10] == '1')
	{
		Zip = pPatient->m_Address.m_ZIP;
	}
	else Zip = "";
	//Done

	//Country;  //12
	if (::Settings.m_EXCEL_WFCT_SETS[11] == '1')
	{
		Country = pPatient->m_Address.m_Country;
	}
	else Country = "";
	//Done

	//WPhone;   //13
	if (::Settings.m_EXCEL_WFCT_SETS[12] == '1')
	{
		WPhone = pPatient->m_WorkPhone;
	}
	else WPhone = "";
	//Done

	//HPhone;   //14
	if (::Settings.m_EXCEL_WFCT_SETS[13] == '1')
	{
		HPhone = pPatient->m_HomePhone;
	}
	else HPhone = "";
	//Done

	//Email;    //15
	if (::Settings.m_EXCEL_WFCT_SETS[14] == '1')
	{
		Email = pPatient->m_EMail;
	}
	else Email = "";
	//Done

	//Note;     //16
	if (::Settings.m_EXCEL_WFCT_SETS[15] == '1')
	{
		Note = pPatient->m_Note;
	}
	else Note = "";
	//Done

	//GroupGUID;//17
	if (::Settings.m_EXCEL_WFCT_SETS[16] == '1')
	{
		GUIDToStr(pPatient->m_GroupID, GroupGUID);
	}
	else GroupGUID = "";
	//Done

	//GroupName;//18
	if (::Settings.m_EXCEL_WFCT_SETS[17] == '1')
	{
		CGroup* Group = ::DB.GetGroup(pPatient->m_GroupID);
		if (Group) GroupName = Group->m_Name;
		else GroupName = "";
	}
	else GroupName = "";
	//Done

	//CalID;    //19
	if (::Settings.m_EXCEL_WFCT_SETS[18] == '1')
	{
		GUIDToStr(::HW.m_Calibration.CalibrationID, CalID[0]);
		GUIDToStr(::HW.m_Calibration.CalibrationID, CalID[1]);
	}
	else
	{
		CalID[0] = "";
		CalID[1] = "";
	}
	//Done

	//Mode;     //20
	if (::Settings.m_EXCEL_WFCT_SETS[19] == '1')
	{
		Mode[0] = WFExam->m_Header.Mode();
		Mode[1] = CTExam->m_Header.Mode();
	}
	else
	{
		Mode[0] = "";
		Mode[1] = "";
	}
	//Done

	//Date;       //21
	if (::Settings.m_EXCEL_WFCT_SETS[20] == '1')
	{
		Date[0].Format(_T("%02i-%02i-%04i"), WFExam->m_Header.m_Month, WFExam->m_Header.m_Day, WFExam->m_Header.m_Year);
		Date[1].Format(_T("%02i-%02i-%04i"), CTExam->m_Header.m_Month, CTExam->m_Header.m_Day, CTExam->m_Header.m_Year);
	}
	else
	{
		Date[0] = "";
		Date[1] = "";
	}
	//Done

	//Time;       //22
	if (::Settings.m_EXCEL_WFCT_SETS[21] == '1')
	{
		Time[0].Format(_T("%02i:%02i:%02i"), WFExam->m_Header.m_Hour, WFExam->m_Header.m_Minute, WFExam->m_Header.m_Second);
		Time[1].Format(_T("%02i:%02i:%02i"), CTExam->m_Header.m_Hour, CTExam->m_Header.m_Minute, CTExam->m_Header.m_Second);
	}
	else
	{
		Time[0] = "";
		Time[1] = "";
	}
	//Done

	//Eye;        //23
	if (::Settings.m_EXCEL_WFCT_SETS[22] == '1')
	{
		Eye[0] = WFExam->m_Header.Eye();
		Eye[1] = CTExam->m_Header.Eye();
	}
	else
	{
		Eye[0] = "";
		Eye[1] = "";
	}
	//Done

	//PrePost;    //24
	if (::Settings.m_EXCEL_WFCT_SETS[23] == '1')
	{
		PrePost[0] = WFExam->m_Header.Preop();
		PrePost[1] = CTExam->m_Header.Preop();
	}
	else
	{
		PrePost[0] = "";
		PrePost[1] = "";
	}
	//Done

	//SW;         //25
	if (::Settings.m_EXCEL_WFCT_SETS[24] == '1')
	{
		SW[0] = WFExam->m_Header.SoftwareVersion();
		SW[1] = CTExam->m_Header.SoftwareVersion();
	}
	else
	{
		SW[0] = "";
		SW[1] = "";
	}
	//Done

	//Clinic Info;         //89
	if (::Settings.m_EXCEL_WFCT_SETS[88] == '1')
	{
		CClinic* pClinic1 = ::DB.GetClinic(WFExam->m_Header.m_ClinicID);
		if (pClinic1)
		{
			GUIDToStr(WFExam->m_Header.m_ClinicID, ClinicUniID[0]);
			ClinicName[0] = pClinic1->m_Name;
			ClinicAdd1[0] = pClinic1->m_Address.m_Line1;
			ClinicAdd2[0] = pClinic1->m_Address.m_Line2;
			ClinicCity[0] = pClinic1->m_Address.m_City;
			ClinicState[0] = pClinic1->m_Address.m_State;
			ClinicZip[0] = pClinic1->m_Address.m_ZIP;
			ClinicCountry[0] = pClinic1->m_Address.m_Country;
		}
		else
		{
			ClinicUniID[0] = "";
			ClinicName[0] = "";
			ClinicAdd1[0] = "";
			ClinicAdd2[0] = "";
			ClinicCity[0] = "";
			ClinicState[0] = "";
			ClinicZip[0] = "";
			ClinicCountry[0] = "";
		}
		CClinic* pClinic2 = ::DB.GetClinic(CTExam->m_Header.m_ClinicID);
		if (pClinic2)
		{
			GUIDToStr(CTExam->m_Header.m_ClinicID, ClinicUniID[1]);
			ClinicName[1] = pClinic2->m_Name;
			ClinicAdd1[1] = pClinic2->m_Address.m_Line1;
			ClinicAdd2[1] = pClinic2->m_Address.m_Line2;
			ClinicCity[1] = pClinic2->m_Address.m_City;
			ClinicState[1] = pClinic2->m_Address.m_State;
			ClinicZip[1] = pClinic2->m_Address.m_ZIP;
			ClinicCountry[1] = pClinic2->m_Address.m_Country;
		}
		else
		{
			ClinicUniID[1] = "";
			ClinicName[1] = "";
			ClinicAdd1[1] = "";
			ClinicAdd2[1] = "";
			ClinicCity[1] = "";
			ClinicState[1] = "";
			ClinicZip[1] = "";
			ClinicCountry[1] = "";
		}
	}
	else
	{
		ClinicUniID[0] = "";
		ClinicName[0] = "";
		ClinicAdd1[0] = "";
		ClinicAdd2[0] = "";
		ClinicCity[0] = "";
		ClinicState[0] = "";
		ClinicZip[0] = "";
		ClinicCountry[0] = "";

		ClinicUniID[1] = "";
		ClinicName[1] = "";
		ClinicAdd1[1] = "";
		ClinicAdd2[1] = "";
		ClinicCity[1] = "";
		ClinicState[1] = "";
		ClinicZip[1] = "";
		ClinicCountry[1] = "";
	}
	//Done

	CPhysician* pPhysician1 = ::DB.GetPhysician(WFExam->m_Header.m_PhysicianID);
	CPhysician* pPhysician2 = ::DB.GetPhysician(CTExam->m_Header.m_PhysicianID);

	COperator*  pOperator1 = ::DB.GetOperator(WFExam->m_Header.m_OperatorID);
	COperator*  pOperator2 = ::DB.GetOperator(CTExam->m_Header.m_OperatorID);

	//PhysicianID;//26
	if (::Settings.m_EXCEL_WFCT_SETS[25] == '1')
	{
		if (pPhysician1) GUIDToStr(WFExam->m_Header.m_PhysicianID, PhysicianID[0]);
		else PhysicianID[0] = "";

		if (pPhysician2) GUIDToStr(CTExam->m_Header.m_PhysicianID, PhysicianID[1]);
		else PhysicianID[1] = "";
	}
	else
	{
		PhysicianID[0] = "";
		PhysicianID[1] = "";
	}
	//Done

	//PhysFName;  //27
	if (::Settings.m_EXCEL_WFCT_SETS[26] == '1')
	{
		if (pPhysician1) PhysFName[0] = pPhysician1->m_FirstName;
		else PhysFName[0] = "";

		if (pPhysician2) PhysFName[1] = pPhysician2->m_FirstName;
		else PhysFName[1] = "";
	}
	else
	{
		PhysFName[0] = "";
		PhysFName[1] = "";
	}
	//Done

	//PhysLName;  //28
	if (::Settings.m_EXCEL_WFCT_SETS[27] == '1')
	{
		if (pPhysician1) PhysLName[0] = pPhysician1->m_LastName;
		else PhysLName[0] = "";

		if (pPhysician2) PhysLName[1] = pPhysician2->m_LastName;
		else PhysLName[1] = "";
	}
	else
	{
		PhysLName[0] = "";
		PhysLName[1] = "";
	}
	//Done

	//OperatorID; //29
	if (::Settings.m_EXCEL_WFCT_SETS[28] == '1')
	{
		if (pOperator1) GUIDToStr(WFExam->m_Header.m_OperatorID, OperatorID[0]);
		else OperatorID[0] = "";

		if (pOperator2) GUIDToStr(CTExam->m_Header.m_OperatorID, OperatorID[1]);
		else OperatorID[1] = "";
	}
	else
	{
		OperatorID[0] = "";
		OperatorID[1] = "";
	}
	//Done

	//OperFName;  //30
	if (::Settings.m_EXCEL_WFCT_SETS[29] == '1')
	{
		if (pOperator1) OperFName[0] = pOperator1->m_FirstName;
		else OperFName[0] = "";

		if (pOperator2) OperFName[1] = pOperator2->m_FirstName;
		else OperFName[1] = "";
	}
	else
	{
		OperFName[0] = "";
		OperFName[1] = "";
	}
	//Done

	//OperLName;  //31
	if (::Settings.m_EXCEL_WFCT_SETS[30] == '1')
	{
		if (pOperator1) OperLName[0] = pOperator1->m_LastName;
		else OperLName[0] = "";

		if (pOperator2) OperLName[1] = pOperator2->m_LastName;
		else OperLName[1] = "";
	}
	else
	{
		OperLName[0] = "";
		OperLName[1] = "";
	}
	//Done

	//ExamNote;   //32
	if (::Settings.m_EXCEL_WFCT_SETS[31] == '1')
	{
		ExamNote[0] = WFExam->m_Header.m_Note;
		ExamNote[1] = CTExam->m_Header.m_Note;
	}
	else
	{
		ExamNote[0] = "";
		ExamNote[1] = "";
	}
	//Done

	//AngelKDist; //33
	if (::Settings.m_EXCEL_WFCT_SETS[32] == '1')
	{
		if (WFExam->m_Header.m_AngleKappaOK)
		{
			AngelKDist.Format(_T("%.3f"), WFExam->m_Header.m_AngleKappaRUm / 1000.0);
		}
		else AngelKDist = "";
	}
	else AngelKDist = "";
	//Done

	//AngelKDeg;  //34
	if (::Settings.m_EXCEL_WFCT_SETS[33] == '1')
	{
		if (WFExam->m_Header.m_AngleKappaOK)
		{
			AngelKDeg.Format(_T("%i"), WFExam->m_Header.m_AngleKappaADg);
		}
		else AngelKDeg = "";
	}
	else AngelKDeg = "";
	//Done

	//AngelADist; //35
	if (::Settings.m_EXCEL_WFCT_SETS[34] == '1')
	{
		//
		LimbusAdjust(WFExam, CTExam);
		WFExam->CalcAngles();
		//
		if (WFExam->m_Header.m_AngleAlphaOK)
		{
			AngelADist.Format(_T("%.3f"), WFExam->m_Header.m_AngleAlphaRUm / 1000.0);
		}
		else AngelADist = "";
	}
	else AngelADist = "";
	//Done

	//AngelADeg;  //36
	if (::Settings.m_EXCEL_WFCT_SETS[35] == '1')
	{
		//
		LimbusAdjust(WFExam, CTExam);
		WFExam->CalcAngles();
		//

		if (WFExam->m_Header.m_AngleAlphaOK)
		{
			AngelADeg.Format(_T("%i"), WFExam->m_Header.m_AngleAlphaADg);
		}
		else AngelADeg = "";
	}
	else AngelADeg = "";
	//Done

	//LimbalX;    //37
	if (::Settings.m_EXCEL_WFCT_SETS[36] == '1')
	{
		/*if(WFExam->m_Image.m_li_ok)
		{
		LimbalX[0].Format(_T("%.3f"), WFExam->m_Image.m_li_x0_um/1000.0);
		}
		else LimbalX[0] = "";*/

		if (CTExam->m_Image.m_li_ok)
		{
			LimbalX[1].Format(_T("%.3f"), CTExam->m_Image.m_li_x0_um / 1000.0);
		}
		else LimbalX[1] = "";
	}
	else
	{
		//LimbalX[0] = "";
		LimbalX[1] = "";
	}
	//Done

	//LimbalY;    //38
	if (::Settings.m_EXCEL_WFCT_SETS[37] == '1')
	{
		/*if(WFExam->m_Image.m_li_ok)
		{
		LimbalY[0].Format(_T("%.3f"), WFExam->m_Image.m_li_y0_um/1000.0);
		}
		else LimbalY[0] = "";*/

		if (CTExam->m_Image.m_li_ok)
		{
			LimbalY[1].Format(_T("%.3f"), CTExam->m_Image.m_li_y0_um / 1000.0);
		}
		else LimbalY[1] = "";
	}
	else
	{
		//LimbalY[0] = "";
		LimbalY[1] = "";
	}
	//Done

	//LimbalD;    //39
	if (::Settings.m_EXCEL_WFCT_SETS[38] == '1')
	{
		/*if(WFExam->m_Image.m_li_ok)
		{
		LimbalD[0].Format(_T("%.3f"), (WFExam->m_Image.m_li_r_mean_um*2.0)/1000.0);
		}
		else LimbalD[0] = "";*/

		if (CTExam->m_Image.m_li_ok)
		{
			LimbalD[1].Format(_T("%.3f"), (CTExam->m_Image.m_li_r_mean_um*2.0) / 1000.0);
		}
		else LimbalD[1] = "";
	}
	else
	{
		//LimbalD[0] = "";
		LimbalD[1] = "";
	}
	//Done

	//PupilX;     //40
	if (::Settings.m_EXCEL_WFCT_SETS[39] == '1')
	{
		if (WFExam->m_Image.m_pu_ok)
		{
			PupilX.Format(_T("%.3f"), WFExam->m_Image.m_pu_x0_um / 1000.0);
		}
		else PupilX = "";
	}
	else PupilX = "";
	//Done

	//PupilY;     //41
	if (::Settings.m_EXCEL_WFCT_SETS[40] == '1')
	{
		if (WFExam->m_Image.m_pu_ok)
		{
			PupilY.Format(_T("%.3f"), WFExam->m_Image.m_pu_y0_um / 1000.0);
		}
		else PupilY = "";
	}
	else PupilY = "";
	//Done

	//PupilD;     //42
	if (::Settings.m_EXCEL_WFCT_SETS[41] == '1')
	{
		if (WFExam->m_Image.m_pu_ok)
		{
			PupilD.Format(_T("%.3f"), (WFExam->m_Image.m_pu_r_mean_um * 2) / 1000.0);
		}
		else PupilD = "";
	}
	else PupilD = "";
	//Done

	//ScanD;      //43
	if (::Settings.m_EXCEL_WFCT_SETS[42] == '1')
	{
		ScanD.Format(_T("%.3f"), WFExam->m_ScanDiameter / 1000.0);
	}
	else ScanD = "";
	//Done

	CZernikeSurface WFSurface[5];

	real_t r_max_um = 5000.0;
	r_max_um = __min(r_max_um, WFExam->m_WfSurface.m_r_max_um);
	r_max_um = __min(r_max_um, CTExam->m_WfSurface.m_r_max_um);

	for (int i = 0; i < 5; i++)
	{
		WFSurface[i] = WFExam->m_WfSurface;
		WFSurface[i].ChangeRMaxUm(r_max_um);
	}

	int vd = int(::Settings.m_VertexDistanceUm / 1000.0);
	CString vdStr;
	vdStr.Format(_T("%i"), vd);

	//TraceRef Sph, Cyl, Axis, VD;   //44
	if (::Settings.m_EXCEL_WFCT_SETS[43] == '1')
	{
		real_t SphEq, Sph, Cyl; int Axis;
		WFSurface[0].ChangeRMaxUm(2000.0);
		//WFExam->GetSpheqSphCylAxis(Surface, ::Settings.m_VertexDistanceUm, TRUE, SphEq, Sph, Cyl, Axis);
		CWFExam::GetSpheqSphCylAxis(WFSurface[0], ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder,
			SphEq, Sph, Cyl, Axis);

		if (::Settings.m_RefractionRounding == 0)
		{
			Sph = round8(Sph);
			Cyl = round8(Cyl);
		}

		TraceRefSph.Format(_T("%.2f"), Sph);
		TraceRefCyl.Format(_T("%.2f"), Cyl);
		TraceRefAxis.Format(_T("%i"), Axis);
		TraceRefVd = vdStr;
	}
	else
	{
		TraceRefSph = "";
		TraceRefCyl = "";
		TraceRefAxis = "";
		TraceRefVd = "";
	}
	//Done

	real_t WFZoneRum[4];
	//Ref1Zone;   //45
	if (::Settings.m_EXCEL_WFCT_SETS[44] == '1')
	{
		WFZoneRum[0] = 0.5 * ::Settings.m_ZoneDiameterUm[0];

		if (WFZoneRum[0] <= WFSurface[1].m_r_max_um)
		{
			real_t SphEq, Sph, Cyl; int Axis;
			WFSurface[1].ChangeRMaxUm(WFZoneRum[0]);
			CWFExam::GetSpheqSphCylAxis(WFSurface[1], ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder,
				SphEq, Sph, Cyl, Axis);

			Ref1ZoneSph.Format(_T("%.2f"), Sph);
			Ref1ZoneCyl.Format(_T("%.2f"), Cyl);
			Ref1ZoneAxis.Format(_T("%i"), Axis);
			Ref1ZoneVd = vdStr;
		}
		else
		{
			Ref1ZoneSph = ""; //45
			Ref1ZoneCyl = ""; //45
			Ref1ZoneAxis = ""; //45
			Ref1ZoneVd = ""; //45
		}
	}
	else
	{
		Ref1ZoneSph = ""; //45
		Ref1ZoneCyl = ""; //45
		Ref1ZoneAxis = ""; //45
		Ref1ZoneVd = ""; //45
	}
	//Done

	//Ref2Zone;   //46
	if (::Settings.m_EXCEL_WFCT_SETS[45] == '1')
	{
		WFZoneRum[1] = 0.5 * ::Settings.m_ZoneDiameterUm[1];

		if (WFZoneRum[1] <= WFSurface[2].m_r_max_um)
		{
			real_t SphEq, Sph, Cyl; int Axis;
			WFSurface[2].ChangeRMaxUm(WFZoneRum[1]);
			CWFExam::GetSpheqSphCylAxis(WFSurface[2], ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder,
				SphEq, Sph, Cyl, Axis);

			Ref2ZoneSph.Format(_T("%.2f"), Sph);
			Ref2ZoneCyl.Format(_T("%.2f"), Cyl);
			Ref2ZoneAxis.Format(_T("%i"), Axis);
			Ref2ZoneVd = vdStr;
		}
		else
		{
			Ref2ZoneSph = ""; //45
			Ref2ZoneCyl = ""; //45
			Ref2ZoneAxis = ""; //45
			Ref2ZoneVd = ""; //45
		}
	}
	else
	{
		Ref2ZoneSph = ""; //46
		Ref2ZoneCyl = ""; //46
		Ref2ZoneAxis = ""; //46
		Ref2ZoneVd = ""; //46
	}
	//Done

	//Ref3Zone;   //47
	if (::Settings.m_EXCEL_WFCT_SETS[46] == '1')
	{
		WFZoneRum[2] = 0.5 * ::Settings.m_ZoneDiameterUm[2];

		if (WFZoneRum[2] <= WFSurface[3].m_r_max_um)
		{
			real_t SphEq, Sph, Cyl; int Axis;
			WFSurface[3].ChangeRMaxUm(WFZoneRum[2]);
			CWFExam::GetSpheqSphCylAxis(WFSurface[3], ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder,
				SphEq, Sph, Cyl, Axis);

			Ref3ZoneSph.Format(_T("%.2f"), Sph);
			Ref3ZoneCyl.Format(_T("%.2f"), Cyl);
			Ref3ZoneAxis.Format(_T("%i"), Axis);
			Ref3ZoneVd = vdStr;
		}
		else
		{
			Ref3ZoneSph = ""; //45
			Ref3ZoneCyl = ""; //45
			Ref3ZoneAxis = ""; //45
			Ref3ZoneVd = ""; //45
		}
	}
	else
	{
		Ref3ZoneSph = ""; //47
		Ref3ZoneCyl = ""; //47
		Ref3ZoneAxis = ""; //47
		Ref3ZoneVd = ""; //47
	}
	//Done

	//Ref4Zone;   //48
	if (::Settings.m_EXCEL_WFCT_SETS[47] == '1')
	{
		WFZoneRum[3] = WFSurface[4].m_r_max_um;

		if (WFZoneRum[3] <= WFSurface[4].m_r_max_um)
		{
			real_t SphEq, Sph, Cyl; int Axis;
			WFSurface[4].ChangeRMaxUm(WFZoneRum[3]);
			CWFExam::GetSpheqSphCylAxis(WFSurface[4], ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder,
				SphEq, Sph, Cyl, Axis);

			Ref4ZoneSph.Format(_T("%.2f"), Sph);
			Ref4ZoneCyl.Format(_T("%.2f"), Cyl);
			Ref4ZoneAxis.Format(_T("%i"), Axis);
			Ref4ZoneVd = vdStr;
		}
		else
		{
			Ref4ZoneSph = ""; //45
			Ref4ZoneCyl = ""; //45
			Ref4ZoneAxis = ""; //45
			Ref4ZoneVd = ""; //45
		}
	}
	else
	{
		Ref4ZoneSph = ""; //48
		Ref4ZoneCyl = ""; //48
		Ref4ZoneAxis = ""; //48
		Ref4ZoneVd = ""; //48
	}
	//Done


	//EntireEyeZC1; //49
	if (::Settings.m_EXCEL_WFCT_SETS[48] == '1')
	{
		if (WFZoneRum[0] <= WFExam->m_WfSurface.m_r_max_um)//6.2.0
		{
			for (int i = 0; i < 45; i++)
			{
				real_t um = WFSurface[1].GetCUm(i);
				//um = fabs(um);
				EntireEyeZC1[i].Format(_T("%6.10f"), um);
			}
		}
		else
		{
			for (int i = 0; i < 45; i++)
			{
				EntireEyeZC1[i] = "";
			}
		}
	}
	else
	{
		for (int i = 0; i < 45; i++)
		{
			EntireEyeZC1[i] = "";
		}
	}
	//Done

	//EntireEyeZC2; //50
	if (::Settings.m_EXCEL_WFCT_SETS[49] == '1')
	{
		if (WFZoneRum[1] <= WFExam->m_WfSurface.m_r_max_um)//6.2.0
		{
			for (int i = 0; i < 45; i++)
			{
				real_t um = WFSurface[2].GetCUm(i);
				//um = fabs(um);
				EntireEyeZC2[i].Format(_T("%6.10f"), um);
			}
		}
		else
		{
			for (int i = 0; i < 45; i++)
			{
				EntireEyeZC2[i] = "";
			}
		}
	}
	else
	{
		for (int i = 0; i < 45; i++)
		{
			EntireEyeZC2[i] = "";
		}
	}
	//Done

	//EntireEyeZC3; //51
	if (::Settings.m_EXCEL_WFCT_SETS[50] == '1')
	{
		if (WFZoneRum[2] <= WFExam->m_WfSurface.m_r_max_um)//6.2.0
		{
			for (int i = 0; i < 45; i++)
			{
				real_t um = WFSurface[3].GetCUm(i);
				//um = fabs(um);
				EntireEyeZC3[i].Format(_T("%6.10f"), um);
			}
		}
		else
		{
			for (int i = 0; i < 45; i++)
			{
				EntireEyeZC3[i] = "";
			}
		}
	}
	else
	{
		for (int i = 0; i < 45; i++)
		{
			EntireEyeZC3[i] = "";
		}
	}
	//Done

	//EntireEyeZCF; //52
	if (::Settings.m_EXCEL_WFCT_SETS[51] == '1')
	{
		for (int i = 0; i < 45; i++)
		{
			real_t um = WFSurface[4].GetCUm(i);
			//um = fabs(um);
			EntireEyeZCF[i].Format(_T("%6.10f"), um);
		}
	}
	else
	{
		for (int i = 0; i < 45; i++)
		{
			EntireEyeZCF[i] = "";
		}
	}

	// //save wf-ct
	if (::NewSettings.m_NEW_EXCEL_WFCT_SETS[0] == '1')
	{
		WFCTOpacityVal = GetOpacityGrade(WFExam);
	}
	else
	{
		WFCTOpacityVal = "";
	}
	//
	//Done


	//****************************************************
	//Attributes from CT Exam

	//CentralPwr;     //53
	if (::Settings.m_EXCEL_WFCT_SETS[52] == '1')
	{
		CentralPwrD.Format(_T("%.10f"), CTExam->m_ax0_dp);
		CentralPwrMM.Format(_T("%.10f"), 0.001 * CTExam->m_ax0_um);
	}
	else
	{
		CentralPwrD = "";
		CentralPwrMM = "";
	}
	//Done

	//BestFitSph; //54
	if (::Settings.m_EXCEL_WFCT_SETS[53] == '1')
	{
		BestFitSph.Format(_T("%.10f"), 0.001 * CTExam->m_HtSpSurface.m_ax0_um);
	}
	else  BestFitSph = "";
	//Done

	//BestConCen; //55
	if (::Settings.m_EXCEL_WFCT_SETS[54] == '1')
	{
		BestConCen.Format(_T("%.10f"), 0.001 * CTExam->m_HtCnSurface.m_ax0_um);
	}
	else  BestConCen = "";
	//Done

	//BestConEcc; //56
	if (::Settings.m_EXCEL_WFCT_SETS[55] == '1')
	{
		BestConEcc.Format(_T("%.10f"), CTExam->m_HtCnSurface.GetEccentricity());
	}
	else  BestConEcc = "";
	//Done

	//BestConAsp; //57
	if (::Settings.m_EXCEL_WFCT_SETS[56] == '1')
	{
		BestConAsp.Format(_T("%.10f"), CTExam->m_HtCnSurface.m_q);
	}
	else  BestConAsp = "";
	//Done

	//RefPwrStp;  //58
	if (::Settings.m_EXCEL_WFCT_SETS[57] == '1')
	{
		if (CTExam->m_rf_ok)
		{
			RefPwrStpD.Format(_T("%.10f"), CTExam->m_rf_st_dp);
			RefPwrStpAxis.Format(_T("%i"), CTExam->m_rf_st_dg);
		}
		else
		{
			RefPwrStpD = "";
			RefPwrStpAxis = "";
		}
	}
	else
	{
		RefPwrStpD = "";
		RefPwrStpAxis = "";
	}
	//Done

	//RefPwrFlt;  //59
	if (::Settings.m_EXCEL_WFCT_SETS[58] == '1')
	{
		if (CTExam->m_rf_ok)
		{
			RefPwrFltD.Format(_T("%.10f"), CTExam->m_rf_fl_dp);
			RefPwrFltAxis.Format(_T("%i"), CTExam->m_rf_fl_dg);
		}
		else
		{
			RefPwrFltD = "";
			RefPwrFltAxis = "";
		}

	}
	else
	{
		RefPwrFltD = "";
		RefPwrFltAxis = "";
	}
	//Done

	//EffRef;     //60
	if (::Settings.m_EXCEL_WFCT_SETS[59] == '1')
	{
		if (CTExam->m_rf_ok) EffRef.Format(_T("%.10f"), CTExam->m_rf_av_dp);
		else EffRef.Empty();
	}
	else  EffRef = "";
	//Done

	//RefPwrAst;  //61
	if (::Settings.m_EXCEL_WFCT_SETS[60] == '1')
	{
		if (CTExam->m_rf_ok)
		{
			RefPwrAstD.Format(_T("%.10f"), CTExam->m_rf_df_dp);
			RefPwrAstAxis.Format(_T("%i"), CTExam->m_rf_st_dg);
		}
		else
		{
			RefPwrAstD = "";
			RefPwrAstAxis = "";
		}
	}
	else
	{
		RefPwrAstD = "";
		RefPwrAstAxis = "";
	}
	//Done

	//SimKStp;    //62
	if (::Settings.m_EXCEL_WFCT_SETS[61] == '1')
	{
		if (CTExam->m_sk_ok)
		{
			SimKStpD.Format(_T("%.10f"), CTExam->m_sk_st_dp);
			SimKStpMM.Format(_T("%.10f"), 0.001 * CTExam->m_sk_st_um);
			SimKStpDeg.Format(_T("%i"), CTExam->m_sk_st_dg);
		}
		else
		{
			SimKStpD = "";
			SimKStpMM = "";
			SimKStpDeg = "";
		}
	}
	else
	{
		SimKStpD = "";
		SimKStpMM = "";
		SimKStpDeg = "";
	}
	//Done

	//SimKFlt;    //63
	if (::Settings.m_EXCEL_WFCT_SETS[62] == '1')
	{
		if (CTExam->m_sk_ok)
		{
			SimKFltD.Format(_T("%.10f"), CTExam->m_sk_fl_dp);
			SimKFltMM.Format(_T("%.10f"), 0.001 * CTExam->m_sk_fl_um);
			SimKFltDeg.Format(_T("%i"), CTExam->m_sk_fl_dg);
		}
		else
		{
			SimKFltD = "";
			SimKFltMM = "";
			SimKFltDeg = "";
		}
	}
	else
	{
		SimKFltD = "";
		SimKFltMM = "";
		SimKFltDeg = "";
	}
	//Done

	//SimKAvg;    //64
	if (::Settings.m_EXCEL_WFCT_SETS[63] == '1')
	{
		if (CTExam->m_sk_ok)
		{
			SimKAvgD.Format(_T("%.10f"), CTExam->m_sk_av_dp);
			SimKAvgMM.Format(_T("%.10f"), 0.001 * CTExam->m_sk_av_um);
		}
		else
		{
			SimKAvgD = "";
			SimKAvgMM = "";
		}
	}
	else
	{
		SimKAvgD = "";
		SimKAvgMM = "";
	}
	//Done

	//SimKDel;    //65
	if (::Settings.m_EXCEL_WFCT_SETS[64] == '1')
	{
		if (CTExam->m_sk_ok)
		{
			SimKDelD.Format(_T("%.10f"), CTExam->m_sk_df_dp);
			SimKDelDGE.Format(_T("%.10f"), 0.001 * CTExam->m_sk_st_dg);
		}
		else
		{
			SimKDelD = "";
			SimKDelDGE = "";
		}
	}
	else
	{
		SimKDelD = "";
		SimKDelDGE = "";
	}
	//Done

	//IS;         //66
	if (::Settings.m_EXCEL_WFCT_SETS[65] == '1')
	{
		if (CTExam->m_is_ok) IS.Format(_T("%.10f"), CTExam->m_is_df_dp);
		else IS.Empty();
	}
	else  IS = "";
	//Done

	//AvePwr;     //67
	if (::Settings.m_EXCEL_WFCT_SETS[66] == '1')
	{
		for (int i = 0; i< 11; i++)
		{
			if (CTExam->m_zk_ok[i])
				AvePwr[i].Format(_T("%.10f"), CTExam->m_zk_av_dp[i]);
			else
				AvePwr[i].Empty();
		}
	}
	else
	{
		for (int i = 0; i< 11; i++) AvePwr[i] = "";
	}
	//Done

	//StpSemi1; //68
	if (::Settings.m_EXCEL_WFCT_SETS[67] == '1')
	{
		if (CTExam->m_kr_ok[0])
		{
			StpSemi1D.Format(_T("%.10f"), CTExam->m_kr_st_dp[0][0]);
			StpSemi1MM.Format(_T("%.10f"), 0.001 * CTExam->m_kr_st_um[0][0]);
			StpSemi1Deg.Format(_T("%i"), CTExam->m_kr_st_dg[0][0]);
		}
		else
		{
			StpSemi1D = "";
			StpSemi1MM = "";
			StpSemi1Deg = "";
		}
	}
	else
	{
		StpSemi1D = "";
		StpSemi1MM = "";
		StpSemi1Deg = "";
	}
	//Done

	//StpSemi2; //69
	if (::Settings.m_EXCEL_WFCT_SETS[68] == '1')
	{
		if (CTExam->m_kr_ok[0])
		{
			StpSemi2D.Format(_T("%.10f"), CTExam->m_kr_st_dp[0][1]);
			StpSemi2MM.Format(_T("%.10f"), 0.001 * CTExam->m_kr_st_um[0][1]);
			StpSemi2Deg.Format(_T("%i"), CTExam->m_kr_st_dg[0][1]);
		}
		else
		{
			StpSemi2D = "";
			StpSemi2MM = "";
			StpSemi2Deg = "";
		}
	}
	else
	{
		StpSemi2D = "";
		StpSemi2MM = "";
		StpSemi2Deg = "";
	}
	//Done

	//FltSemi1; //70
	if (::Settings.m_EXCEL_WFCT_SETS[69] == '1')
	{
		if (CTExam->m_kr_ok[0])
		{
			FltSemi1D.Format(_T("%.10f"), CTExam->m_kr_fl_dp[0][0]);
			FltSemi1MM.Format(_T("%.10f"), 0.001 * CTExam->m_kr_fl_um[0][0]);
			FltSemi1Deg.Format(_T("%i"), CTExam->m_kr_fl_dg[0][0]);
		}
		else
		{
			FltSemi1D = "";
			FltSemi1MM = "";
			FltSemi1Deg = "";
		}
	}
	else
	{
		FltSemi1D = "";
		FltSemi1MM = "";
		FltSemi1Deg = "";
	}
	//Done

	//FltSemi2; //71
	if (::Settings.m_EXCEL_WFCT_SETS[70] == '1')
	{
		if (CTExam->m_kr_ok[0])
		{
			FltSemi2D.Format(_T("%.10f"), CTExam->m_kr_fl_dp[0][1]);
			FltSemi2MM.Format(_T("%.10f"), 0.001 * CTExam->m_kr_fl_um[0][1]);
			FltSemi2Deg.Format(_T("%i"), CTExam->m_kr_fl_dg[0][1]);
		}
		else
		{
			FltSemi2D = "";
			FltSemi2MM = "";
			FltSemi2Deg = "";
		}
	}
	else
	{
		FltSemi2D = "";
		FltSemi2MM = "";
		FltSemi2Deg = "";
	}
	//Done

	//StpSemi12; //72
	if (::Settings.m_EXCEL_WFCT_SETS[71] == '1')
	{
		if (CTExam->m_kr_ok[1])
		{
			StpSemi12D.Format(_T("%.10f"), CTExam->m_kr_st_dp[1][0]);
			StpSemi12MM.Format(_T("%.10f"), 0.001 * CTExam->m_kr_st_um[1][0]);
			StpSemi12Deg.Format(_T("%i"), CTExam->m_kr_st_dg[1][0]);
		}
		else
		{
			StpSemi12D = "";
			StpSemi12MM = "";
			StpSemi12Deg = "";
		}
	}
	else
	{
		StpSemi12D = "";
		StpSemi12MM = "";
		StpSemi12Deg = "";
	}
	//Done

	//StpSemi22; //73
	if (::Settings.m_EXCEL_WFCT_SETS[72] == '1')
	{
		if (CTExam->m_kr_ok[1])
		{
			StpSemi22D.Format(_T("%.10f"), CTExam->m_kr_st_dp[1][1]);
			StpSemi22MM.Format(_T("%.10f"), 0.001 * CTExam->m_kr_st_um[1][1]);
			StpSemi22Deg.Format(_T("%i"), CTExam->m_kr_st_dg[1][1]);
		}
		else
		{
			StpSemi22D = "";
			StpSemi22MM = "";
			StpSemi22Deg = "";
		}
	}
	else
	{
		StpSemi22D = "";
		StpSemi22MM = "";
		StpSemi22Deg = "";
	}
	//Done

	//FltSemi12; //74
	if (::Settings.m_EXCEL_WFCT_SETS[73] == '1')
	{
		if (CTExam->m_kr_ok[1])
		{
			FltSemi12D.Format(_T("%.10f"), CTExam->m_kr_fl_dp[1][0]);
			FltSemi12MM.Format(_T("%.10f"), 0.001 * CTExam->m_kr_fl_um[1][0]);
			FltSemi12Deg.Format(_T("%i"), CTExam->m_kr_fl_dg[1][0]);
		}
		else
		{
			FltSemi12D = "";
			FltSemi12MM = "";
			FltSemi12Deg = "";
		}
	}
	else
	{
		FltSemi12D = "";
		FltSemi12MM = "";
		FltSemi12Deg = "";
	}
	//Done

	//FltSemi22; //75
	if (::Settings.m_EXCEL_WFCT_SETS[74] == '1')
	{
		if (CTExam->m_kr_ok[1])
		{
			FltSemi22D.Format(_T("%.10f"), CTExam->m_kr_fl_dp[1][1]);
			FltSemi22MM.Format(_T("%.10f"), 0.001 * CTExam->m_kr_fl_um[1][1]);
			FltSemi22Deg.Format(_T("%i"), CTExam->m_kr_fl_dg[1][1]);
		}
		else
		{
			FltSemi22D = "";
			FltSemi22MM = "";
			FltSemi22Deg = "";
		}
	}
	else
	{
		FltSemi22D = "";
		FltSemi22MM = "";
		FltSemi22Deg = "";
	}
	//Done

	//StpSemi13; //76
	if (::Settings.m_EXCEL_WFCT_SETS[75] == '1')
	{
		if (CTExam->m_kr_ok[2])
		{
			StpSemi13D.Format(_T("%.10f"), CTExam->m_kr_st_dp[2][0]);
			StpSemi13MM.Format(_T("%.10f"), 0.001 * CTExam->m_kr_st_um[2][0]);
			StpSemi13Deg.Format(_T("%i"), CTExam->m_kr_st_dg[2][0]);
		}
		else
		{
			StpSemi13D = "";
			StpSemi13MM = "";
			StpSemi13Deg = "";
		}
	}
	else
	{
		StpSemi13D = "";
		StpSemi13MM = "";
		StpSemi13Deg = "";
	}
	//Done

	//StpSemi23; //77
	if (::Settings.m_EXCEL_WFCT_SETS[76] == '1')
	{
		if (CTExam->m_kr_ok[2])
		{
			StpSemi23D.Format(_T("%.10f"), CTExam->m_kr_st_dp[2][1]);
			StpSemi23MM.Format(_T("%.10f"), 0.001 * CTExam->m_kr_st_um[2][1]);
			StpSemi23Deg.Format(_T("%i"), CTExam->m_kr_st_dg[2][1]);
		}
		else
		{
			StpSemi23D = "";
			StpSemi23MM = "";
			StpSemi23Deg = "";
		}
	}
	else
	{
		StpSemi23D = "";
		StpSemi23MM = "";
		StpSemi23Deg = "";
	}
	//Done

	//FltSemi13; //78
	if (::Settings.m_EXCEL_WFCT_SETS[77] == '1')
	{
		if (CTExam->m_kr_ok[2])
		{
			FltSemi13D.Format(_T("%.10f"), CTExam->m_kr_fl_dp[2][0]);
			FltSemi13MM.Format(_T("%.10f"), 0.001 * CTExam->m_kr_fl_um[2][0]);
			FltSemi13Deg.Format(_T("%i"), CTExam->m_kr_fl_dg[2][0]);
		}
		else
		{
			FltSemi13D = "";
			FltSemi13MM = "";
			FltSemi13Deg = "";
		}
	}
	else
	{
		FltSemi13D = "";
		FltSemi13MM = "";
		FltSemi13Deg = "";
	}
	//Done

	//FltSemi23; //79
	if (::Settings.m_EXCEL_WFCT_SETS[78] == '1')
	{
		if (CTExam->m_kr_ok[2])
		{
			FltSemi23D.Format(_T("%.10f"), CTExam->m_kr_fl_dp[2][1]);
			FltSemi23MM.Format(_T("%.10f"), 0.001 * CTExam->m_kr_fl_um[2][1]);
			FltSemi23Deg.Format(_T("%i"), CTExam->m_kr_fl_dg[2][1]);
		}
		else
		{
			FltSemi23D = "";
			FltSemi23MM = "";
			FltSemi23Deg = "";
		}
	}
	else
	{
		FltSemi23D = "";
		FltSemi23MM = "";
		FltSemi23Deg = "";
	}
	//Done

	//CornealSph; //80
	if (::Settings.m_EXCEL_WFCT_SETS[79] == '1')
	{
		CornealSph.Format(_T("%.10f"), CTExam->m_sph_6mm);
	}
	else  CornealSph = "";
	//Done

	CZernikeSurface CTSurface[5];
	for (int i = 0; i < 5; i++)
	{
		CTSurface[i] = CTExam->m_WfSurface;
		CTSurface[i].ChangeRMaxUm(r_max_um);
	}
	real_t CTZoneRum[4];

	//CornealZer1; //81
	if (::Settings.m_EXCEL_WFCT_SETS[80] == '1')
	{
		CTZoneRum[0] = 0.5 * ::Settings.m_ZoneDiameterUm[0];

		if (CTZoneRum[0] <= CTSurface[1].m_r_max_um)
			CTSurface[1].ChangeRMaxUm(CTZoneRum[0]);

		for (int i = 0; i < 45; i++)
		{
			real_t um = CTSurface[1].GetCUm(i);
			//um = fabs(um);
			CornealZer1[i].Format(_T("%6.10f"), um);
		}
	}
	else { for (int i = 0; i < 45; i++)	CornealZer1[i] = ""; }
	//Done

	//CornealZer2; //82
	if (::Settings.m_EXCEL_WFCT_SETS[81] == '1')
	{
		CTZoneRum[1] = 0.5 * ::Settings.m_ZoneDiameterUm[1];
		if (CTZoneRum[1] <= CTSurface[2].m_r_max_um)
			CTSurface[2].ChangeRMaxUm(CTZoneRum[1]);

		for (int i = 0; i < 45; i++)
		{
			real_t um = CTSurface[2].GetCUm(i);
			//um = fabs(um);
			CornealZer2[i].Format(_T("%6.10f"), um);
		}
	}
	else { for (int i = 0; i < 45; i++)	CornealZer2[i] = ""; }
	//Done

	//CornealZer3; //83
	if (::Settings.m_EXCEL_WFCT_SETS[82] == '1')
	{
		CTZoneRum[2] = 0.5 * ::Settings.m_ZoneDiameterUm[2];

		if (CTZoneRum[2] <= CTSurface[3].m_r_max_um)
			CTSurface[3].ChangeRMaxUm(CTZoneRum[2]);

		for (int i = 0; i < 45; i++)
		{
			real_t um = CTSurface[3].GetCUm(i);
			//um = fabs(um);
			CornealZer3[i].Format(_T("%6.10f"), um);
		}
	}
	else { for (int i = 0; i < 45; i++)	CornealZer3[i] = ""; }
	//Done

	//CornealZerF; //84
	if (::Settings.m_EXCEL_WFCT_SETS[83] == '1')
	{
		CTZoneRum[3] = CTSurface[4].m_r_max_um;

		if (CTZoneRum[3] <= CTSurface[4].m_r_max_um)
			CTSurface[4].ChangeRMaxUm(CTZoneRum[3]);

		for (int i = 0; i < 45; i++)
		{
			real_t um = CTSurface[4].GetCUm(i);
			//um = fabs(um);
			CornealZerF[i].Format(_T("%6.10f"), um);
		}
	}
	else { for (int i = 0; i < 45; i++)	CornealZerF[i] = ""; }
	//Done

	//****************************************************
	//Attributes from WF and CT Exam
	CZernikeSurface WFCTSurface[4];
	CZernikeSurface WFSur = WFExam->m_WfSurface;
	CZernikeSurface CTSur = CTExam->m_WfSurface;
	WFSur.ChangeRMaxUm(r_max_um);
	CTSur.ChangeRMaxUm(r_max_um);
	CZernikeSurface WFCTSur;
	WFCTSur = WFSur;
	WFCTSur -= CTSur;

	for (int i = 0; i < 4; i++)		WFCTSurface[i] = WFCTSur;

	//CString InternalZer1[45]; //85
	if (::Settings.m_EXCEL_WFCT_SETS[84] == '1')
	{
		if (CTZoneRum[0] <= WFCTSurface[0].m_r_max_um)
			WFCTSurface[0].ChangeRMaxUm(CTZoneRum[0]);

		for (int i = 0; i < 45; i++)
		{
			real_t um = WFCTSurface[0].GetCUm(i);
			//um = fabs(um);
			InternalZer1[i].Format(_T("%6.10f"), um);
		}
	}
	else { for (int i = 0; i < 45; i++)	InternalZer1[i] = ""; }
	//Done

	//CString InternalZer2[45]; //86
	if (::Settings.m_EXCEL_WFCT_SETS[85] == '1')
	{
		if (CTZoneRum[1] <= WFCTSurface[1].m_r_max_um)
			WFCTSurface[1].ChangeRMaxUm(CTZoneRum[1]);

		for (int i = 0; i < 45; i++)
		{
			real_t um = WFCTSurface[1].GetCUm(i);
			//um = fabs(um);
			InternalZer2[i].Format(_T("%6.10f"), um);
		}
	}
	else { for (int i = 0; i < 45; i++)	InternalZer2[i] = ""; }
	//Done

	//CString InternalZer3[45]; //87
	if (::Settings.m_EXCEL_WFCT_SETS[86] == '1')
	{
		if (CTZoneRum[2] <= WFCTSurface[2].m_r_max_um)
			WFCTSurface[2].ChangeRMaxUm(CTZoneRum[2]);

		for (int i = 0; i < 45; i++)
		{
			real_t um = WFCTSurface[2].GetCUm(i);
			//um = fabs(um);
			InternalZer3[i].Format(_T("%6.10f"), um);
		}
	}
	else { for (int i = 0; i < 45; i++)	InternalZer3[i] = ""; }
	//Done

	//CString InternalZerF[45]; //88
	if (::Settings.m_EXCEL_WFCT_SETS[87] == '1')
	{
		for (int i = 0; i < 45; i++)
		{
			real_t um = WFCTSurface[3].GetCUm(i);
			//um = fabs(um);
			InternalZerF[i].Format(_T("%6.10f"), um);
		}
	}
	else { for (int i = 0; i < 45; i++)	InternalZerF[i] = ""; }
	//Done

	// DLI Value
	CString DLIVal;

	if (::NewSettings.m_NEW_EXCEL_WFCT_SETS[0] == '1')
	{
		DLIVal = GetDliValue(WFExam, CTExam);
	}
	else
	{
		DLIVal = "";
	}
	//
	//****************************************************



	//Save the exam info into excel file

	//Following add the exam info into the excel file
	Range range;

	Workbooks books;
	_Workbook book;

	Worksheets sheets;
	_Worksheet sheet;

	LPDISPATCH lpDisp;
	//COleVariant vResult;

	COleVariant  covTrue((short)TRUE), covFalse((short)FALSE), covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	excelApp.SetVisible(FALSE);
	excelApp.SetUserControl(TRUE);

	books.AttachDispatch(excelApp.GetWorkbooks());

	long iStartRow = 1;
	BOOL NewExcel = FALSE;

	if (!::PathFileExists(FileName))
	{
		NewExcel = TRUE;
		books = excelApp.GetWorkbooks();
		book = books.Add(covOptional);
		sheets = book.GetSheets();
		sheet = sheets.GetItem(COleVariant((short)1));
		sheet.SetName(_T("WF and CT Exams Report"));
	}
	else
	{
		try
		{
			lpDisp = books.Open(FileName,
				covOptional, covOptional, covOptional, covOptional, covOptional,
				covOptional, covOptional, covOptional, covOptional, covOptional,
				covOptional, covOptional);


			//Get work book
			book.AttachDispatch(lpDisp);
			//Get work sheets
			sheets.AttachDispatch(book.GetWorksheets());

			lpDisp = book.GetActiveSheet();
			sheet.AttachDispatch(lpDisp);
		}
		catch (...)
		{
			books.Close();
			excelApp.Quit();
			::CoUninitialize();

			::Info("Excel file open failed");
			return -3;
		}
	}

	//Used info
	if (!NewExcel)
	{
		Range usedRange;
		usedRange.AttachDispatch(sheet.GetUsedRange());
		range.AttachDispatch(usedRange.GetRows());
		long iRowNum = range.GetCount();   //Used Row
		iStartRow = iRowNum + 1;     //Started row
		range.AttachDispatch(sheet.GetCells(), TRUE);
	}
	else
	{
		//add Title
		CString StrPos[734] = { // From 735 to 737, Then change to 734
			"A1", "B1", "C1", "D1", "E1", "F1", "G1",
			"H1", "I1", "J1", "K1", "L1", "M1", "N1",
			"O1", "P1", "Q1", "R1", "S1", "T1", "U1",
			"V1", "W1", "X1", "Y1", "Z1",

			"AA1", "AB1", "AC1", "AD1", "AE1", "AF1", "AG1",
			"AH1", "AI1", "AJ1", "AK1", "AL1", "AM1", "AN1",
			"AO1", "AP1", "AQ1", "AR1", "AS1", "AT1", "AU1",
			"AV1", "AW1", "AX1", "AY1", "AZ1",

			"BA1", "BB1", "BC1", "BD1", "BE1", "BF1", "BG1",
			"BH1", "BI1", "BJ1", "BK1", "BL1", "BM1", "BN1",
			"BO1", "BP1", "BQ1", "BR1", "BS1", "BT1", "BU1",
			"BV1", "BW1", "BX1", "BY1", "BZ1",

			"CA1", "CB1", "CC1", "CD1", "CE1", "CF1", "CG1",
			"CH1", "CI1", "CJ1", "CK1", "CL1", "CM1", "CN1",
			"CO1", "CP1", "CQ1", "CR1", "CS1", "CT1", "CU1",
			"CV1", "CW1", "CX1", "CY1", "CZ1",

			"DA1", "DB1", "DC1", "DD1", "DE1", "DF1", "DG1",
			"DH1", "DI1", "DJ1", "DK1", "DL1", "DM1", "DN1",
			"DO1", "DP1", "DQ1", "DR1", "DS1", "DT1", "DU1",
			"DV1", "DW1", "DX1", "DY1", "DZ1",

			"EA1", "EB1", "EC1", "ED1", "EE1", "EF1", "EG1",
			"EH1", "EI1", "EJ1", "EK1", "EL1", "EM1", "EN1",
			"EO1", "EP1", "EQ1", "ER1", "ES1", "ET1", "EU1",
			"EV1", "EW1", "EX1", "EY1", "EZ1",

			"FA1", "FB1", "FC1", "FD1", "FE1", "FF1", "FG1",
			"FH1", "FI1", "FJ1", "FK1", "FL1", "FM1", "FN1",
			"FO1", "FP1", "FQ1", "FR1", "FS1", "FT1", "FU1",
			"FV1", "FW1", "FX1", "FY1", "FZ1",

			"GA1", "GB1", "GC1", "GD1", "GE1", "GF1", "GG1",
			"GH1", "GI1", "GJ1", "GK1", "GL1", "GM1", "GN1",
			"GO1", "GP1", "GQ1", "GR1", "GS1", "GT1", "GU1",
			"GV1", "GW1", "GX1", "GY1", "GZ1",

			"HA1", "HB1", "HC1", "HD1", "HE1", "HF1", "HG1",
			"HH1", "HI1", "HJ1", "HK1", "HL1", "HM1", "HN1",
			"HO1", "HP1", "HQ1", "HR1", "HS1", "HT1", "HU1",
			"HV1", "HW1", "HX1", "HY1", "HZ1",

			"IA1", "IB1", "IC1", "ID1", "IE1", "IF1", "IG1",
			"IH1", "II1", "IJ1", "IK1", "IL1", "IM1", "IN1",
			"IO1", "IP1", "IQ1", "IR1", "IS1", "IT1", "IU1",
			"IV1", "IW1", "IX1", "IY1", "IZ1",

			"JA1", "JB1", "JC1", "JD1", "JE1", "JF1", "JG1",//JC1
			"JH1", "JI1", "JJ1", "JK1", "JL1", "JM1", "JN1",
			"JO1", "JP1", "JQ1", "JR1", "JS1", "JT1", "JU1",
			"JV1", "JW1", "JX1", "JY1", "JZ1",

			"KA1", "KB1", "KC1", "KD1", "KE1", "KF1", "KG1",
			"KH1", "KI1", "KJ1", "KK1", "KL1", "KM1", "KN1",
			"KO1", "KP1", "KQ1", "KR1", "KS1", "KT1", "KU1",
			"KV1", "KW1", "KX1", "KY1", "KZ1",

			"LA1", "LB1", "LC1", "LD1", "LE1", "LF1", "LG1",
			"LH1", "LI1", "LJ1", "LK1", "LL1", "LM1", "LN1",
			"LO1", "LP1", "LQ1", "LR1", "LS1", "LT1", "LU1",
			"LV1", "LW1", "LX1", "LY1", "LZ1",

			"MA1", "MB1", "MC1", "MD1", "ME1", "MF1", "MG1",
			"MH1", "MI1", "MJ1", "MK1", "ML1", "MM1", "MN1",
			"MO1", "MP1", "MQ1", "MR1", "MS1", "MT1", "MU1",
			"MV1", "MW1", "MX1", "MY1", "MZ1",

			"NA1", "NB1", "NC1", "ND1", "NE1", "NF1", "NG1",
			"NH1", "NI1", "NJ1", "NK1", "NL1", "NM1", "NN1",
			"NO1", "NP1", "NQ1", "NR1", "NS1", "NT1", "NU1",
			"NV1", "NW1", "NX1", "NY1", "NZ1",

			"OA1", "OB1", "OC1", "OD1", "OE1", "OF1", "OG1",
			"OH1", "OI1", "OJ1", "OK1", "OL1", "OM1", "ON1",
			"OO1", "OP1", "OQ1", "OR1", "OS1", "OT1", "OU1",
			"OV1", "OW1", "OX1", "OY1", "OZ1",

			"PA1", "PB1", "PC1", "PD1", "PE1", "PF1", "PG1",
			"PH1", "PI1", "PJ1", "PK1", "PL1", "PM1", "PN1",
			"PO1", "PP1", "PQ1", "PR1", "PS1", "PT1", "PU1",
			"PV1", "PW1", "PX1", "PY1", "PZ1",

			"QA1", "QB1", "QC1", "QD1", "QE1", "QF1", "QG1",
			"QH1", "QI1", "QJ1", "QK1", "QL1", "QM1", "QN1",
			"QO1", "QP1", "QQ1", "QR1", "QS1", "QT1", "QU1",
			"QV1", "QW1", "QX1", "QY1", "QZ1",

			"RA1", "RB1", "RC1", "RD1", "RE1", "RF1", "RG1",
			"RH1", "RI1", "RJ1", "RK1", "RL1", "RM1", "RN1",
			"RO1", "RP1", "RQ1", "RR1", "RS1", "RT1", "RU1",
			"RV1", "RW1", "RX1", "RY1", "RZ1",

			"SA1", "SB1", "SC1", "SD1", "SE1", "SF1", "SG1",
			"SH1", "SI1", "SJ1", "SK1", "SL1", "SM1", "SN1",
			"SO1", "SP1", "SQ1", "SR1", "SS1", "ST1", "SU1",
			"SV1", "SW1", "SX1", "SY1", "SZ1",

			"TA1", "TB1", "TC1", "TD1", "TE1", "TF1", "TG1",
			"TH1", "TI1", "TJ1", "TK1", "TL1", "TM1", "TN1",
			"TO1", "TP1", "TQ1", "TR1", "TS1", "TT1", "TU1",
			"TV1", "TW1", "TX1", "TY1", "TZ1",

			"UA1", "UB1", "UC1", "UD1", "UE1", "UF1", "UG1",
			"UH1", "UI1", "UJ1", "UK1", "UL1", "UM1", "UN1",
			"UO1", "UP1", "UQ1", "UR1", "US1", "UT1", "UU1",
			"UV1", "UW1", "UX1", "UY1", "UZ1",

			"VA1", "VB1", "VC1", "VD1", "VE1", "VF1", "VG1",
			"VH1", "VI1", "VJ1", "VK1", "VL1", "VM1", "VN1",
			"VO1", "VP1", "VQ1", "VR1", "VS1", "VT1", "VU1",
			"VV1", "VW1", "VX1", "VY1", "VZ1",

			"WA1", "WB1", "WC1", "WD1", "WE1", "WF1", "WG1",
			"WH1", "WI1", "WJ1", "WK1", "WL1", "WM1", "WN1",
			"WO1", "WP1", "WQ1", "WR1", "WS1", "WT1", "WU1",
			"WV1", "WW1", "WX1", "WY1", "WZ1",

			"XA1", "XB1", "XC1", "XD1", "XE1", "XF1", "XG1",
			"XH1", "XI1", "XJ1", "XK1", "XL1", "XM1", "XN1",
			"XO1", "XP1", "XQ1", "XR1", "XS1", "XT1", "XU1",
			"XV1", "XW1", "XX1", "XY1", "XZ1",

			"YA1", "YB1", "YC1", "YD1", "YE1", "YF1", "YG1",
			"YH1", "YI1", "YJ1", "YK1", "YL1", "YM1", "YN1",
			"YO1", "YP1", "YQ1", "YR1", "YS1", "YT1", "YU1",
			"YV1", "YW1", "YX1", "YY1", "YZ1",

			"ZA1", "ZB1", "ZC1", "ZD1", "ZE1", "ZF1", "ZG1",
			"ZH1", "ZI1", "ZJ1", "ZK1", "ZL1", "ZM1", "ZN1",
			"ZO1", "ZP1", "ZQ1", "ZR1", "ZS1", "ZT1", "ZU1",
			"ZV1", "ZW1", "ZX1", "ZY1", "ZZ1",

			"AAA1", "AAB1", "AAC1", "AAD1", "AAE1", "AAF1", "AAG1",
			"AAH1", "AAI1", "AAJ1", "AAK1", "AAL1", "AAM1", "AAN1",
			"AAO1", "AAP1", "AAQ1", "AAR1", "AAS1", "AAT1", "AAU1",
			"AAV1", "AAW1", "AAX1", "AAY1", "AAZ1",

			"ABA1", "ABB1", "ABC1", "ABD1", "ABE1", "ABF1"
			// Add "ABH1" adn "ABI1", and then remove "ABE1", "ABF1", "ABG1", "ABH1", "ABI1"
		};
		int WidthPos[734] = {
			16, 40, 12, 12, 12, 12, 12, // a, b, c, d, e, f, g
			12, 12, 12, 12, 12, 12, 12, // h, i, j, k, l, m, n
			12, 12, 12, 40, 40, 12, 12, // o, p, q, r, s, t, u
			40, 40, 20, 12, 12,         // v, w, x, y, z

			12, 12, 12, 12, 40, 12, 12, //A
			12, 12, 12, 12, 12, 40, 20,
			20, 40, 20, 20, 12, 12, 14,
			14,

			14, 14, 14, 12, 12, 12, //B
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12, //C
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12, //D
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12, //E
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12, //F
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12, //G
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12, //H
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12, //I
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12, //J
			40, 40, 20, 12, 12, 12, 12,
			12, 12, 40,   14, 14, 14, 14,
			14, 14, 14, 40, 20,

			20, 40, 20, 20, 16, 16, 16, //K
			16, 16, 18, 18, 20, 20, 20,
			20, 22, 18, 22, 14, 20, 24,
			12, 12, 12, 12, 12,

			12, 12, 12, 18, 14, 18, 18, //L
			18, 18, 18, 18, 18, 18, 18,
			18, 18, 18, 18, 18, 18, 18,
			18, 18, 18, 18, 18,

			18, 18, 18, 18, 18, 18, 18, //M
			18, 18, 18, 18, 18, 18, 18,
			18, 18, 18, 18, 18, 18, 18,
			18, 18, 18, 18, 18,

			18, 20, 12, 12, 12, 12, 12, //N
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12, //O
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12, //P
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12, //Q
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12, //R
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 16,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12, //S
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12, //T
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 20, 12, 12, //U
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12, //V
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12, //W
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12, //X
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12, //Y
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12, //Z
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12, //AA
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12, 12, 12,
			12, 12, 12, 12, 12,

			12, 12, 12, 12, 12, 12, 12, //AB
			12, 30 // add to '12'
		};

		for (int i = 0; i < 734; i++)
		{
			range = sheet.GetRange(COleVariant(StrPos[i]), COleVariant(StrPos[i]));
			range.SetColumnWidth(COleVariant((long)WidthPos[i]));
			//Cell Font
			Font font;
			font.AttachDispatch(range.GetFont());
			font.SetBold(covTrue);
			font.SetColor(COleVariant((long)0xFF0000));
			range.SetHorizontalAlignment(COleVariant((short)-4108)); //Center
			range.SetVerticalAlignment(COleVariant((short)-4108)); //Center
																   //Done

																   //Background Color
			Interior it;
			it.AttachDispatch(range.GetInterior());

			if (i < 20)       it.SetColorIndex(COleVariant((long)20));
			else if (i < 261) it.SetColorIndex(COleVariant((long)38));
			else if (i < 546) it.SetColorIndex(COleVariant((long)40));
			else             it.SetColorIndex(COleVariant((long)50));


			if (i == 75 || i == 121 || i == 167 || i == 213
				|| i == 361 || i == 407 || i == 453 || i == 499
				|| i == 549 || i == 595 || i == 641 || i == 687
				)
				it.SetColorIndex(COleVariant((long)33));

			//
			if (i == 547) it.SetColorIndex(COleVariant((long)15));
			if (i == 548) it.SetColorIndex(COleVariant((long)15));

			if (i == 259) it.SetColorIndex(COleVariant((long)15));
			if (i == 733) it.SetColorIndex(COleVariant((long)15));
			//
			//Done

			//Border
			Borders border;
			border = range.GetBorders();
			border.SetColorIndex(COleVariant((long)1));
			border.SetLineStyle(COleVariant((long)1));
			border.SetWeight(COleVariant((long)4));
			//Done
		}

		//Font font;
		range.AttachDispatch(sheet.GetCells(), TRUE);
		//font.AttachDispatch (range.GetFont ());
		//font.SetColor(COleVariant((long)0xFF0000));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)1), COleVariant(_T("Export Time")));   //A
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)2), COleVariant(_T("Patient Unique ID")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)3), COleVariant(_T("ID")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)4), COleVariant(_T("First Name")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)5), COleVariant(_T("Middle Name")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)6), COleVariant(_T("Last Name")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)7), COleVariant(_T("DOB")));   //G
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)8), COleVariant(_T("Gender")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)9), COleVariant(_T("Add Line1")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)10), COleVariant(_T("Add Line2")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)11), COleVariant(_T("City")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)12), COleVariant(_T("State")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)13), COleVariant(_T("Zip")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)14), COleVariant(_T("Country")));  //N
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)15), COleVariant(_T("Work Phone")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)16), COleVariant(_T("Home Phone")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)17), COleVariant(_T("Email")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)18), COleVariant(_T("Note")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)19), COleVariant(_T("Group GUID")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)20), COleVariant(_T("Group Name")));  //T

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)21), COleVariant(_T("WF Data")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)22), COleVariant(_T("Exam Unique ID")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)23), COleVariant(_T("Calibration Unique ID")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)24), COleVariant(_T("Exam Type")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)25), COleVariant(_T("Mode")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)26), COleVariant(_T("Exam Date")));  //Z
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)27), COleVariant(_T("Exam Time")));   //AA
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)28), COleVariant(_T("Eye")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)29), COleVariant(_T("Pre/Post")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)30), COleVariant(_T("SW")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)31), COleVariant(_T("Clinic Unique ID")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)32), COleVariant(_T("Clinic Name")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)33), COleVariant(_T("Clinic Add1")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)34), COleVariant(_T("Clinic Add2")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)35), COleVariant(_T("Clinic City")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)36), COleVariant(_T("Clinic State")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)37), COleVariant(_T("Clinic Zip")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)38), COleVariant(_T("Clinic Country")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)39), COleVariant(_T("Physician ID")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)40), COleVariant(_T("Physician First Name")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)41), COleVariant(_T("Physician Last Name")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)42), COleVariant(_T("Operator ID")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)43), COleVariant(_T("Operator First Name")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)44), COleVariant(_T("Operator Last Name")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)45), COleVariant(_T("Exam Note")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)46), COleVariant(_T("Angel K Dist")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)47), COleVariant(_T("Angel K Degree")));
		/*range.SetItem(COleVariant((long)iStartRow),COleVariant((long)48),COleVariant(_T("Angel A Dist")));
		range.SetItem(COleVariant((long)iStartRow),COleVariant((long)49),COleVariant(_T("Angel A Degree")));
		range.SetItem(COleVariant((long)iStartRow),COleVariant((long)50),COleVariant(_T("Limbal Center X")));
		range.SetItem(COleVariant((long)iStartRow),COleVariant((long)51),COleVariant(_T("Limbal Center Y")));

		range.SetItem(COleVariant((long)iStartRow),COleVariant((long)52),COleVariant(_T("Limbal Diameter")));     */
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)48), COleVariant(_T("Pupil Center X")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)49), COleVariant(_T("Pupil Center Y")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)50), COleVariant(_T("Pupil Diameter")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)51), COleVariant(_T("Scan Diameter")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)52), COleVariant(_T("TraceRef Sph")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)53), COleVariant(_T("TraceRef Cyl")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)54), COleVariant(_T("TraceRef Axis")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)55), COleVariant(_T("TraceRef VD")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)56), COleVariant(_T("Ref1 Zone")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)57), COleVariant(_T("Ref1 Sph")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)58), COleVariant(_T("Ref1 Cyl")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)59), COleVariant(_T("Ref1 Axis")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)60), COleVariant(_T("Ref1 VD")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)61), COleVariant(_T("Ref2 Zone")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)62), COleVariant(_T("Ref2 Sph")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)63), COleVariant(_T("Ref2 Cyl")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)64), COleVariant(_T("Ref2 Axis")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)65), COleVariant(_T("Ref2 VD")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)66), COleVariant(_T("Ref3 Zone")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)67), COleVariant(_T("Ref3 Sph")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)68), COleVariant(_T("Ref3 Cyl")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)69), COleVariant(_T("Ref3 Axis")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)70), COleVariant(_T("Ref3 VD")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)71), COleVariant(_T("Ref4 Zone")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)72), COleVariant(_T("Ref4 Sph")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)73), COleVariant(_T("Ref4 Cyl")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)74), COleVariant(_T("Ref4 Axis")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)75), COleVariant(_T("Ref4 VD")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)76), COleVariant(_T("Z1 Zone")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)122), COleVariant(_T("Z2 Zone")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)168), COleVariant(_T("Z3 Zone")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)214), COleVariant(_T("Z4 Zone")));

		CString s, s1, s2, s3, s4;
		for (int i = 0; i < 45; i++)
		{
			s.Format(_T("%i"), i);
			s1 = "Z1 C" + s;
			s2 = "Z2 C" + s;
			s3 = "Z3 C" + s;
			s4 = "Z4 C" + s;
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)77 + i), COleVariant(s1));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)123 + i), COleVariant(s2));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)169 + i), COleVariant(s3));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)215 + i), COleVariant(s4));
		}

		//
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)260), COleVariant(_T("Opacity Grade")));
		//

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)261), COleVariant(_T("WF Data End")));//JE

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)262), COleVariant(_T("CT Data")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)263), COleVariant(_T("Exam Unique ID")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)264), COleVariant(_T("Calibration Unique ID")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)265), COleVariant(_T("Exam Type")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)266), COleVariant(_T("Mode")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)267), COleVariant(_T("Exam Date")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)268), COleVariant(_T("Exam Time")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)269), COleVariant(_T("Eye")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)270), COleVariant(_T("Pre/Post")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)271), COleVariant(_T("SW")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)272), COleVariant(_T("Clinic Unique ID")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)273), COleVariant(_T("Clinic Name")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)274), COleVariant(_T("Clinic Add1")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)275), COleVariant(_T("Clinic Add2")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)276), COleVariant(_T("Clinic City")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)277), COleVariant(_T("Clinic State")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)278), COleVariant(_T("Clinic Zip")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)279), COleVariant(_T("Clinic Country")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)280), COleVariant(_T("Physician ID")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)281), COleVariant(_T("Physician First Name")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)282), COleVariant(_T("Physician Last Name")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)283), COleVariant(_T("Operator ID")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)284), COleVariant(_T("Operator First Name")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)285), COleVariant(_T("Operator Last Name")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)286), COleVariant(_T("Exam Note")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)287), COleVariant(_T("Limbal Center X")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)288), COleVariant(_T("Limbal Center Y")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)289), COleVariant(_T("Limbal Diameter")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)290), COleVariant(_T("CentralPwr D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)291), COleVariant(_T("CentralPwr mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)292), COleVariant(_T("BestFit Sph Rad")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)293), COleVariant(_T("BestFit Con-CentralRad")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)294), COleVariant(_T("BestFit Con-Eccentricity")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)295), COleVariant(_T("BestFit Con-Asph")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)296), COleVariant(_T("RefPwr at <3mm Stp D")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)297), COleVariant(_T("RefPwr at <3mm Stp Axis")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)298), COleVariant(_T("RefPwr at <3mm Flt D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)299), COleVariant(_T("RefPwr at <3mm Flt Axis")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)300), COleVariant(_T("Eff Ref Pwr")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)301), COleVariant(_T("RefPwr at <3mm Astig D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)302), COleVariant(_T("RefPwr at <3mm Astig Axis1")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)303), COleVariant(_T("SimK Stp D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)304), COleVariant(_T("SimK Stp mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)305), COleVariant(_T("SimK Stp Deg")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)306), COleVariant(_T("SimK Flt D")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)307), COleVariant(_T("SimK Flt mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)308), COleVariant(_T("SimK Flt Deg")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)309), COleVariant(_T("SimK Avg D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)310), COleVariant(_T("SimK Avg mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)311), COleVariant(_T("SimK Delta D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)312), COleVariant(_T("SimK Delta Deg")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)313), COleVariant(_T("I-S ")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)314), COleVariant(_T("AvgPwer at 0mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)315), COleVariant(_T("AvgPwer at 1mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)316), COleVariant(_T("AvgPwer at 2mm")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)317), COleVariant(_T("AvgPwer at 2mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)318), COleVariant(_T("AvgPwer at 3mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)319), COleVariant(_T("AvgPwer at 4mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)320), COleVariant(_T("AvgPwer at 5mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)321), COleVariant(_T("AvgPwer at 6mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)322), COleVariant(_T("AvgPwer at 7mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)323), COleVariant(_T("AvgPwer at 8mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)324), COleVariant(_T("AvgPwer at 10mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)325), COleVariant(_T("Stp Semi1 at <3 D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)326), COleVariant(_T("Stp Semi1 at <3 mm")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)327), COleVariant(_T("Stp Semi1 at <3 Deg")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)328), COleVariant(_T("Stp Semi2 at <3 D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)329), COleVariant(_T("Stp Semi2 at <3 mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)330), COleVariant(_T("Stp Semi2 at <3 Deg")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)331), COleVariant(_T("Flt Semi1 at <3 D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)332), COleVariant(_T("Flt Semi1 at <3 mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)333), COleVariant(_T("Flt Semi1 at <3 Deg")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)334), COleVariant(_T("Flt Semi2 at <3 D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)335), COleVariant(_T("Flt Semi2 at <3 mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)336), COleVariant(_T("Flt Semi2 at <3 Deg")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)337), COleVariant(_T("Stp Semi1 @ 3 - 5 D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)338), COleVariant(_T("Stp Semi1 @ 3 - 5 mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)339), COleVariant(_T("Stp Semi1 @ 3 - 5 Deg")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)340), COleVariant(_T("Stp Semi2 @ 3 - 5 D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)341), COleVariant(_T("Stp Semi2 @ 3 - 5 mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)342), COleVariant(_T("Stp Semi2 @ 3 - 5 Deg")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)343), COleVariant(_T("Flt Semi1 @ 3 - 5 D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)344), COleVariant(_T("Flt Semi1 @ 3 - 5 mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)345), COleVariant(_T("Flt Semi1 @ 3 - 5 Deg")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)346), COleVariant(_T("Flt Semi2 @ 3 - 5 D")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)347), COleVariant(_T("Flt Semi2 @ 3 - 5 mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)348), COleVariant(_T("Flt Semi2 @ 3 - 5 Deg")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)349), COleVariant(_T("Stp Semi1 @ 5-7 D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)350), COleVariant(_T("Stp Semi1 @ 5-7 mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)351), COleVariant(_T("Stp Semi1 @ 5-7 Deg")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)352), COleVariant(_T("Stp Semi2 @ 5-7 D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)353), COleVariant(_T("Stp Semi2 @ 5-7 mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)354), COleVariant(_T("Stp Semi2 @5-7 Deg")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)355), COleVariant(_T("Flt Semi1 @ 5-7 D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)356), COleVariant(_T("Flt Semi1 @ 5-7 mm")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)357), COleVariant(_T("Flt Semi1 @ 5-7 Deg")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)358), COleVariant(_T("Flt Semi2 @ 5-7 D")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)359), COleVariant(_T("Flt Semi2 @ 5-7 mm")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)360), COleVariant(_T("Flt Semi2 @ 5-7 Deg")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)361), COleVariant(_T("Corneal SphAberr@6mm")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)362), COleVariant(_T("CZ1 Zone")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)408), COleVariant(_T("CZ2 Zone")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)454), COleVariant(_T("CZ3 Zone")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)500), COleVariant(_T("CZ4 Zone")));

		for (int i = 0; i < 45; i++)
		{
			s.Format(_T("%i"), i);
			s1 = "CZ1 C" + s;
			s2 = "CZ2 C" + s;
			s3 = "CZ3 C" + s;
			s4 = "CZ4 C" + s;
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)363 + i), COleVariant(s1));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)409 + i), COleVariant(s2));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)455 + i), COleVariant(s3));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)501 + i), COleVariant(s4));
		}

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)546), COleVariant(_T("CT Data End")));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)547), COleVariant(_T("WF & CT Data")));

		// Move Angle a to here
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)548), COleVariant(_T("Angel A Dist")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)549), COleVariant(_T("Angel A Degree")));
		// Move Angle a to here Done

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)550), COleVariant(_T("IZ1 Zone")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)596), COleVariant(_T("IZ2 Zone")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)642), COleVariant(_T("IZ3 Zone")));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)688), COleVariant(_T("IZ4 Zone")));

		for (int i = 0; i < 45; i++)
		{
			s.Format(_T("%i"), i);
			s1 = "IZ1 C" + s;
			s2 = "IZ2 C" + s;
			s3 = "IZ3 C" + s;
			s4 = "IZ4 C" + s;
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)551 + i), COleVariant(s1));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)597 + i), COleVariant(s2));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)643 + i), COleVariant(s3));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)689 + i), COleVariant(s4));
		}

		//
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)734), COleVariant(_T("Dysfunctional Lens Index")));
		//
		//End pos ''  with 736 cols

		iStartRow = 2;
	}

	//Save Data
	{
		CString s1, s2, s3, s4;

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)1), COleVariant(RecordTime));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)2), COleVariant(PaUniqueID));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)3), COleVariant(UserID));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)4), COleVariant(FName));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)5), COleVariant(MName));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)6), COleVariant(LName));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)7), COleVariant(DOB));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)8), COleVariant(Gender));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)9), COleVariant(AddLine1));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)10), COleVariant(AddLine2));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)11), COleVariant(City));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)12), COleVariant(State));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)13), COleVariant(Zip));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)14), COleVariant(Country));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)15), COleVariant(WPhone));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)16), COleVariant(HPhone));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)17), COleVariant(Email));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)18), COleVariant(Note));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)19), COleVariant(GroupGUID));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)20), COleVariant(GroupName));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)22), COleVariant(ExUniqueID[0]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)23), COleVariant(CalID[0]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)24), COleVariant(ExamType[0]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)25), COleVariant(Mode[0]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)26), COleVariant(Date[0]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)27), COleVariant(Time[0]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)28), COleVariant(Eye[0]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)29), COleVariant(PrePost[0]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)30), COleVariant(SW[0]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)31), COleVariant(ClinicUniID[0]));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)32), COleVariant(ClinicName[0]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)33), COleVariant(ClinicAdd1[0]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)34), COleVariant(ClinicAdd2[0]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)35), COleVariant(ClinicCity[0]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)36), COleVariant(ClinicState[0]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)37), COleVariant(ClinicZip[0]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)38), COleVariant(ClinicCountry[0]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)39), COleVariant(PhysicianID[0]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)40), COleVariant(PhysFName[0]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)41), COleVariant(PhysLName[0]));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)42), COleVariant(OperatorID[0]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)43), COleVariant(OperFName[0]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)44), COleVariant(OperLName[0]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)45), COleVariant(ExamNote[0]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)46), COleVariant(AngelKDist));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)47), COleVariant(AngelKDeg));
		/*range.SetItem(COleVariant((long)iStartRow),COleVariant((long)48),COleVariant(AngelADist)));
		range.SetItem(COleVariant((long)iStartRow),COleVariant((long)49),COleVariant(AngelADeg)));
		range.SetItem(COleVariant((long)iStartRow),COleVariant((long)50),COleVariant(LimbalX[0])));
		range.SetItem(COleVariant((long)iStartRow),COleVariant((long)51),COleVariant(LimbalY[0])));

		range.SetItem(COleVariant((long)iStartRow),COleVariant((long)52),COleVariant(LimbalD[0])));   */
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)48), COleVariant(PupilX));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)49), COleVariant(PupilY));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)50), COleVariant(PupilD));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)51), COleVariant(ScanD));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)52), COleVariant(TraceRefSph));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)53), COleVariant(TraceRefCyl));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)54), COleVariant(TraceRefAxis));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)55), COleVariant(TraceRefVd));

		s1.Format(_T("%.1f"), (real_t)::Settings.m_ZoneDiameterUm[0] / 1000);
		if (::Settings.m_EXCEL_WFCT_SETS[44] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)56), COleVariant(s1));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)57), COleVariant(Ref1ZoneSph));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)58), COleVariant(Ref1ZoneCyl));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)59), COleVariant(Ref1ZoneAxis));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)60), COleVariant(Ref1ZoneVd));

		s2.Format(_T("%.1f"), (real_t)::Settings.m_ZoneDiameterUm[1] / 1000);
		if (::Settings.m_EXCEL_WFCT_SETS[45] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)61), COleVariant(s2));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)62), COleVariant(Ref2ZoneSph));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)63), COleVariant(Ref2ZoneCyl));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)64), COleVariant(Ref2ZoneAxis));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)65), COleVariant(Ref2ZoneVd));

		s3.Format(_T("%.1f"), (real_t)::Settings.m_ZoneDiameterUm[2] / 1000);
		if (::Settings.m_EXCEL_WFCT_SETS[46] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)66), COleVariant(s3));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)67), COleVariant(Ref3ZoneSph));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)68), COleVariant(Ref3ZoneCyl));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)69), COleVariant(Ref3ZoneAxis));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)70), COleVariant(Ref3ZoneVd));

		s4.Format(_T("%.1f"), (real_t)WFSurface[4].m_r_max_um * 2 / 1000);
		if (::Settings.m_EXCEL_WFCT_SETS[47] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)71), COleVariant(s4));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)72), COleVariant(Ref4ZoneSph));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)73), COleVariant(Ref4ZoneCyl));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)74), COleVariant(Ref4ZoneAxis));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)75), COleVariant(Ref4ZoneVd));

		if (::Settings.m_EXCEL_WFCT_SETS[48] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)76), COleVariant(s1));

		if (::Settings.m_EXCEL_WFCT_SETS[49] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)122), COleVariant(s2));

		if (::Settings.m_EXCEL_WFCT_SETS[50] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)168), COleVariant(s3));

		if (::Settings.m_EXCEL_WFCT_SETS[51] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)214), COleVariant(s4));
		for (int i = 0; i < 45; i++)
		{
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)77 + i), COleVariant(EntireEyeZC1[i]));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)123 + i), COleVariant(EntireEyeZC2[i]));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)169 + i), COleVariant(EntireEyeZC3[i]));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)215 + i), COleVariant(EntireEyeZCF[i]));
		}

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)260), COleVariant(WFCTOpacityVal));//
																										 //***************************************************
																										 //CT Exam data
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)263), COleVariant(ExUniqueID[1]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)264), COleVariant(CalID[1]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)265), COleVariant(ExamType[1]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)266), COleVariant(Mode[1]));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)267), COleVariant(Date[1]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)268), COleVariant(Time[1]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)269), COleVariant(Eye[1]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)270), COleVariant(PrePost[1]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)271), COleVariant(SW[1]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)272), COleVariant(ClinicUniID[1]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)273), COleVariant(ClinicName[1]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)274), COleVariant(ClinicAdd1[1]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)275), COleVariant(ClinicAdd2[1]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)276), COleVariant(ClinicCity[1]));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)277), COleVariant(ClinicState[1]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)278), COleVariant(ClinicZip[1]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)279), COleVariant(ClinicCountry[1]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)280), COleVariant(PhysicianID[1]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)281), COleVariant(PhysFName[1]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)282), COleVariant(PhysLName[1]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)283), COleVariant(OperatorID[1]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)284), COleVariant(OperFName[1]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)285), COleVariant(OperLName[1]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)286), COleVariant(ExamNote[1]));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)287), COleVariant(LimbalX[1]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)288), COleVariant(LimbalY[1]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)289), COleVariant(LimbalD[1]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)290), COleVariant(CentralPwrD));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)291), COleVariant(CentralPwrMM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)292), COleVariant(BestFitSph));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)293), COleVariant(BestConCen));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)294), COleVariant(BestConEcc));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)295), COleVariant(BestConAsp));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)296), COleVariant(RefPwrStpD));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)297), COleVariant(RefPwrStpAxis));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)298), COleVariant(RefPwrFltD));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)299), COleVariant(RefPwrFltAxis));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)300), COleVariant(EffRef));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)301), COleVariant(RefPwrAstD));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)302), COleVariant(RefPwrAstAxis));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)303), COleVariant(SimKStpD));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)304), COleVariant(SimKStpMM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)305), COleVariant(SimKStpDeg));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)306), COleVariant(SimKFltD));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)307), COleVariant(SimKFltMM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)308), COleVariant(SimKFltDeg));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)309), COleVariant(SimKAvgD));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)310), COleVariant(SimKAvgMM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)311), COleVariant(SimKDelD));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)312), COleVariant(SimKDelDGE));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)313), COleVariant(IS));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)314), COleVariant(AvePwr[0]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)315), COleVariant(AvePwr[1]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)316), COleVariant(AvePwr[2]));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)317), COleVariant(AvePwr[3]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)318), COleVariant(AvePwr[4]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)319), COleVariant(AvePwr[5]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)320), COleVariant(AvePwr[6]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)321), COleVariant(AvePwr[7]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)322), COleVariant(AvePwr[8]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)323), COleVariant(AvePwr[9]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)324), COleVariant(AvePwr[10]));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)325), COleVariant(StpSemi1D));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)326), COleVariant(StpSemi1MM));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)327), COleVariant(StpSemi1Deg));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)328), COleVariant(StpSemi2D));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)329), COleVariant(StpSemi2MM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)330), COleVariant(StpSemi2Deg));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)331), COleVariant(FltSemi1D));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)332), COleVariant(FltSemi1MM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)333), COleVariant(FltSemi1Deg));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)334), COleVariant(FltSemi2D));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)335), COleVariant(FltSemi2MM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)336), COleVariant(FltSemi2Deg));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)337), COleVariant(StpSemi12D));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)338), COleVariant(StpSemi12MM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)339), COleVariant(StpSemi12Deg));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)340), COleVariant(StpSemi22D));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)341), COleVariant(StpSemi22MM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)342), COleVariant(StpSemi22Deg));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)343), COleVariant(FltSemi12D));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)344), COleVariant(FltSemi12MM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)345), COleVariant(FltSemi12Deg));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)346), COleVariant(FltSemi22D));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)347), COleVariant(FltSemi22MM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)348), COleVariant(FltSemi22Deg));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)349), COleVariant(StpSemi13D));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)350), COleVariant(StpSemi13MM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)351), COleVariant(StpSemi13Deg));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)352), COleVariant(StpSemi23D));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)353), COleVariant(StpSemi23MM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)354), COleVariant(StpSemi23Deg));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)355), COleVariant(FltSemi13D));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)356), COleVariant(FltSemi13MM));

		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)357), COleVariant(FltSemi13Deg));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)358), COleVariant(FltSemi23D));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)359), COleVariant(FltSemi23MM));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)360), COleVariant(FltSemi23Deg));
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)361), COleVariant(CornealSph));

		s1.Format(_T("%.1f"), (real_t)::Settings.m_ZoneDiameterUm[0] / 1000);
		s2.Format(_T("%.1f"), (real_t)::Settings.m_ZoneDiameterUm[1] / 1000);
		s3.Format(_T("%.1f"), (real_t)::Settings.m_ZoneDiameterUm[2] / 1000);
		s4.Format(_T("%.1f"), (real_t)CTSurface[4].m_r_max_um * 2 / 1000);

		if (::Settings.m_EXCEL_WFCT_SETS[80] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)362), COleVariant(s1));

		if (::Settings.m_EXCEL_WFCT_SETS[81] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)408), COleVariant(s2));

		if (::Settings.m_EXCEL_WFCT_SETS[82] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)454), COleVariant(s3));

		if (::Settings.m_EXCEL_WFCT_SETS[83] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)500), COleVariant(s4));

		// Move Angle a to here
		if (::Settings.m_EXCEL_WFCT_SETS[34] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)548), COleVariant(AngelADist));
		if (::Settings.m_EXCEL_WFCT_SETS[35] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)549), COleVariant(AngelADeg));
		// Move Angle a to here Done

		for (int i = 0; i < 45; i++)
		{
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)363 + i), COleVariant(CornealZer1[i]));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)409 + i), COleVariant(CornealZer2[i]));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)455 + i), COleVariant(CornealZer3[i]));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)501 + i), COleVariant(CornealZerF[i]));

			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)551 + i), COleVariant(InternalZer1[i]));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)597 + i), COleVariant(InternalZer2[i]));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)643 + i), COleVariant(InternalZer3[i]));
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)689 + i), COleVariant(InternalZerF[i]));
		}

		//
		s1.Format(_T("%.1f"), (real_t)::Settings.m_ZoneDiameterUm[0] / 1000);
		s2.Format(_T("%.1f"), (real_t)::Settings.m_ZoneDiameterUm[1] / 1000);
		s3.Format(_T("%.1f"), (real_t)::Settings.m_ZoneDiameterUm[2] / 1000);
		s4.Format(_T("%.1f"), (real_t)WFCTSurface[3].m_r_max_um * 2 / 1000);

		if (::Settings.m_EXCEL_WFCT_SETS[83] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)550), COleVariant(s1));

		if (::Settings.m_EXCEL_WFCT_SETS[84] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)594), COleVariant(s2));

		if (::Settings.m_EXCEL_WFCT_SETS[85] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)642), COleVariant(s3));

		if (::Settings.m_EXCEL_WFCT_SETS[86] == '1')
			range.SetItem(COleVariant((long)iStartRow), COleVariant((long)686), COleVariant(s4));

		//
		range.SetItem(COleVariant((long)iStartRow), COleVariant((long)734), COleVariant(DLIVal));
		//
	}

	if (!NewExcel)
	{
		book.Save();
		book.SetSaved(TRUE);
	}
	else
	{
		book.SaveAs(COleVariant(FileName),
			covOptional, covOptional, covOptional, covOptional,
			covOptional, 0, covOptional, covOptional,
			covOptional, covOptional);
	}
	//release
	range.ReleaseDispatch();
	sheet.ReleaseDispatch();
	sheets.ReleaseDispatch();
	book.ReleaseDispatch();
	books.ReleaseDispatch();
	excelApp.Quit();
	excelApp.ReleaseDispatch();
	::CoUninitialize();
	return 0;
	//Done
}
//Done

//***************************************************************************************
//521
LRESULT CMainWnd::OnSoloHypeLiner(WPARAM wParam, LPARAM lParam)
{
	m_SoloControl = FALSE;
	m_ReselectExam = TRUE;
	m_WFVrfButton.SetEnabled(TRUE);
	m_WFCmpButton.SetEnabled(TRUE);
	m_WFSumButton[0].SetEnabled(TRUE);

	if (wParam == 0)//From Refraction or VFA to aberration
	{
		if ((lParam == 1 && m_pWFExams[0]->m_Header.IsOD()) ||
			(lParam == 0 && m_pWFExams[0]->m_Header.IsOS()))
		{
			SAFE_DELETE(m_pWFExams[0]);

			m_pWFExams[0] = new CWFExam;

			if (m_pWFExams[1]->m_Header.m_ExamID == GUID_NULL)//Multiple selection
			{
				BOOL SOK = TRUE;
				Matrix<CWFExam*> pWFSeriesExams(m_SelNum);

				for (int s = 0; s < m_SelNum; s++)
				{
					CWFExam* loadExam = new CWFExam;
					if (::DB.LoadWFExam(m_ExamIDs[s], loadExam))
					{
						loadExam->Process();

						// for average exam
						CString ExamId, LoadFileName;
						GUIDToStr(loadExam->m_Header.m_ExamID, ExamId);
						LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

						if (::PathFileExists(LoadFileName))
						{
							CAveExamInfo AveExamInfo;

							AveExamInfo.LoadFromFile(LoadFileName);
							for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
							{
								loadExam->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
							}
						}
						// for average exam Done

						pWFSeriesExams[s] = loadExam;
					}
					else
					{
						SAFE_DELETE(m_pWFExams[0]);
						SOK = FALSE;
						break;
					}
				}

				if (SOK)
					m_pWFExams[0]->Average(pWFSeriesExams);
			}
			else
			{
				::DB.LoadWFExam(m_pWFExams[1]->m_Header.m_ExamID, m_pWFExams[0]);
				m_pWFExams[0]->Process();

				// for average exam
				CString ExamId, LoadFileName;
				GUIDToStr(m_pWFExams[0]->m_Header.m_ExamID, ExamId);
				LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

				if (::PathFileExists(LoadFileName))
				{
					CAveExamInfo AveExamInfo;

					AveExamInfo.LoadFromFile(LoadFileName);
					for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
					{
						m_pWFExams[0]->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
					}
				}
				// for average exam Done
			}

			SAFE_DELETE(m_pWFExams[1]);
		}

		OnWFSum1ButtonClicked();
	}
	else if (wParam == 1)//From the VFA to Refraction analysis
	{
		if ((lParam == 1 && m_pWFExams[0]->m_Header.IsOD()) ||
			(lParam == 0 && m_pWFExams[0]->m_Header.IsOS()))
		{
			SAFE_DELETE(m_pWFExams[0]);

			m_pWFExams[0] = new CWFExam;

			if (m_pWFExams[1]->m_Header.m_ExamID == GUID_NULL)//Multiple selection
			{
				BOOL SOK = TRUE;
				Matrix<CWFExam*> pWFSeriesExams(m_SelNum);

				for (int s = 0; s < m_SelNum; s++)
				{
					CWFExam* loadExam = new CWFExam;
					if (::DB.LoadWFExam(m_ExamIDs[s], loadExam))
					{
						loadExam->Process();

						// for average exam
						CString ExamId, LoadFileName;
						GUIDToStr(loadExam->m_Header.m_ExamID, ExamId);
						LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

						if (::PathFileExists(LoadFileName))
						{
							CAveExamInfo AveExamInfo;

							AveExamInfo.LoadFromFile(LoadFileName);
							for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
							{
								loadExam->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
							}
						}
						// for average exam Done

						pWFSeriesExams[s] = loadExam;
					}
					else
					{
						SAFE_DELETE(m_pWFExams[0]);
						SOK = FALSE;
						break;
					}
				}

				if (SOK)
					m_pWFExams[0]->Average(pWFSeriesExams);
			}
			else
			{
				::DB.LoadWFExam(m_pWFExams[1]->m_Header.m_ExamID, m_pWFExams[0]);
				m_pWFExams[0]->Process();

				// for average exam
				CString ExamId, LoadFileName;
				GUIDToStr(m_pWFExams[0]->m_Header.m_ExamID, ExamId);
				LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

				if (::PathFileExists(LoadFileName))
				{
					CAveExamInfo AveExamInfo;

					AveExamInfo.LoadFromFile(LoadFileName);
					for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
					{
						m_pWFExams[0]->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
					}
				}
				// for average exam Done
			}

			SAFE_DELETE(m_pWFExams[1]);
		}

		OnWFSum2ButtonClicked();
	}
	else if (wParam == 2)//From the Aberration to Refraction analysis
	{
		OnWFSum2ButtonClicked();
	}
	else if (wParam == 3)//From the solo comparison to Refraction analysis
	{
		if (lParam == 1)
		{
			SAFE_DELETE(m_pWFExams[0]);
			m_pWFExams[0] = new CWFExam;

			if (m_pWFExams[1]->m_Header.m_ExamID == GUID_NULL)//Multiple selection
			{
				BOOL SOK = TRUE;
				Matrix<CWFExam*> pWFSeriesExams(m_SelNum);

				for (int s = 0; s < m_SelNum; s++)
				{
					CWFExam* loadExam = new CWFExam;
					if (::DB.LoadWFExam(m_ExamIDs[s], loadExam))
					{
						loadExam->Process();

						// for average exam
						CString ExamId, LoadFileName;
						GUIDToStr(loadExam->m_Header.m_ExamID, ExamId);
						LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

						if (::PathFileExists(LoadFileName))
						{
							CAveExamInfo AveExamInfo;

							AveExamInfo.LoadFromFile(LoadFileName);
							for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
							{
								loadExam->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
							}
						}
						// for average exam Done

						pWFSeriesExams[s] = loadExam;
					}
					else
					{
						SAFE_DELETE(m_pWFExams[0]);
						SOK = FALSE;
						break;
					}
				}

				if (SOK)
				{
					m_pWFExams[0]->Average(pWFSeriesExams);

					if (m_pWFExams[1]->m_Header.IsOD())
					{
						::OD_AverageExam = ::OD_AverageExam2;
						for (int i = 0; i < ::OD_AverageExam; i++)
						{
							::OD_SelectExamsID[i] = ::OD_SelectExamsID2[i];
						}
					}
					else
					{
						::OS_AverageExam = ::OS_AverageExam2;
						for (int i = 0; i < ::OS_AverageExam; i++)
						{
							::OS_SelectExamsID[i] = ::OS_SelectExamsID2[i];
						}
					}
				}
			}
			else
			{
				::DB.LoadWFExam(m_pWFExams[1]->m_Header.m_ExamID, m_pWFExams[0]);
				m_pWFExams[0]->Process();

				// for average exam
				CString ExamId, LoadFileName;
				GUIDToStr(m_pWFExams[0]->m_Header.m_ExamID, ExamId);
				LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

				if (::PathFileExists(LoadFileName))
				{
					CAveExamInfo AveExamInfo;

					AveExamInfo.LoadFromFile(LoadFileName);
					for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
					{
						m_pWFExams[0]->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
					}
				}
				// for average exam Done
			}
		}
		SAFE_DELETE(m_pWFExams[1]);
		OnWFSum2ButtonClicked();
	}

	return 0;
}
//Done

//***************************************************************************************

//530
void CMainWnd::OnF1()
{
	::HtmlHelp(GetSafeHwnd(), _T("iTraceDocs.chm"), HH_DISPLAY_TOC, 0);
}
//530

//***************************************************************************************

//530
GUID CMainWnd::GetIDFromS(CString s)
{
	GUID id;
	CString str_id;
	str_id = s.Left(s.GetLength() - 4);

	int strLength = str_id.GetLength();
	char *pValue = new char[strLength];

	G_As = str_id;
	strncpy(pValue, G_As, strLength);

	int newStrLength;
	for (int j = 0; j<strLength - 1; j++)
	{
		char c = *(pValue + j);
		if (c == '{')
		{
			newStrLength = j;
			break;
		}
	}
	str_id = str_id.Right(strLength - newStrLength);

	G_As = str_id;
	::StrToGUID(G_As, id);

	return id;
}
//530

//***************************************************************************************
//530 Set Default Task settings
void  CMainWnd::SetDef_TaskSettings()
{
	::NewSettings.m_ShowTask = TRUE;
	CString Def_Name;
	int     Def_Num;

	for (int i = 0; i < 5; i++)
	{
		if (i == 0) ::NewSettings.m_Tasks[i].m_Enable = TRUE;
		else ::NewSettings.m_Tasks[i].m_Enable = FALSE;//
		::NewSettings.m_Tasks[i].m_DefNum = 0;

		for (int j = 0; j < 10; j++) ::NewSettings.m_Tasks[i].m_SelfSettings[j] = FALSE;

		if (i == 0)
		{
			Def_Name = "Cataract Evaluation";

			if (m_isCombo) //531
			{
				Def_Num = 5;

				::NewSettings.m_Tasks[i].m_SubButton[0] = TYPE_DYSP;
				::NewSettings.m_Tasks[i].m_SubButton[1] = TYPE_CHAN;
				::NewSettings.m_Tasks[i].m_SubButton[2] = TYPE_ANGK;
				::NewSettings.m_Tasks[i].m_SubButton[3] = TYPE_TOAL;
				::NewSettings.m_Tasks[i].m_SubButton[4] = TYPE_TORI;//531
				::NewSettings.m_Tasks[i].m_SubButton[5] = TYPE_NONA;//531
				::NewSettings.m_Tasks[i].m_SubButton[6] = TYPE_NONA;
				::NewSettings.m_Tasks[i].m_SubButton[7] = TYPE_NONA;
				::NewSettings.m_Tasks[i].m_SubButton[8] = TYPE_NONA;
				::NewSettings.m_Tasks[i].m_SubButton[9] = TYPE_NONA;

				CString s;
				//Check WF/CT Visual Acuity
				::Settings.GetButtonName(2, 0, s);
				if (s != "Chang Analysis")
				{
					SetDefTask_CHAN(::NewSettings.m_Tasks[i].m_WndSettings[1]);
					::NewSettings.m_Tasks[i].m_SelfSettings[2] = TRUE;
				}
				//Done
			}
			else
			{
				Def_Num = 3;

				::NewSettings.m_Tasks[i].m_SubButton[0] = TYPE_IOLS;
				::NewSettings.m_Tasks[i].m_SubButton[1] = TYPE_CHAN;
				::NewSettings.m_Tasks[i].m_SubButton[2] = TYPE_TORI;
				::NewSettings.m_Tasks[i].m_SubButton[3] = TYPE_NONA;
				::NewSettings.m_Tasks[i].m_SubButton[4] = TYPE_NONA;//531
				::NewSettings.m_Tasks[i].m_SubButton[5] = TYPE_NONA;//531
				::NewSettings.m_Tasks[i].m_SubButton[6] = TYPE_NONA;
				::NewSettings.m_Tasks[i].m_SubButton[7] = TYPE_NONA;
				::NewSettings.m_Tasks[i].m_SubButton[8] = TYPE_NONA;
				::NewSettings.m_Tasks[i].m_SubButton[9] = TYPE_NONA;

				CString s;
				//Check WF/CT Visual Acuity
				::Settings.GetButtonName(2, 0, s);
				if (s != "Chang Analysis")
				{
					SetDefTask_CHAN(::NewSettings.m_Tasks[i].m_WndSettings[1]);
					::NewSettings.m_Tasks[i].m_SelfSettings[1] = TRUE;
				}
				//Done
			}

			//531 Still need set the TYPE_DYSF Setting?
			//531 Done
		}
		else  if (i == 1)
		{
			Def_Name = "Vision Diagnostics";
			Def_Num = 6;

			::NewSettings.m_Tasks[i].m_SubButton[0] = TYPE_WCOU;
			::NewSettings.m_Tasks[i].m_SubButton[1] = TYPE_REFA;
			::NewSettings.m_Tasks[i].m_SubButton[2] = TYPE_ABEA;
			::NewSettings.m_Tasks[i].m_SubButton[3] = TYPE_WCVA;
			::NewSettings.m_Tasks[i].m_SubButton[4] = TYPE_CTSU;
			::NewSettings.m_Tasks[i].m_SubButton[5] = TYPE_VFA;
			::NewSettings.m_Tasks[i].m_SubButton[6] = TYPE_NONA;
			::NewSettings.m_Tasks[i].m_SubButton[7] = TYPE_NONA;
			::NewSettings.m_Tasks[i].m_SubButton[8] = TYPE_NONA;
			::NewSettings.m_Tasks[i].m_SubButton[9] = TYPE_NONA;

			//Check whether WF/CT Visual Acuity is exist ( Did not changed(customed) by the user)
			CString s;
			::Settings.GetButtonName(2, 2, s);
			if (s != "Visual Acuity")
			{
				SetDefTask_WCVA(::NewSettings.m_Tasks[i].m_WndSettings[3]);
				::NewSettings.m_Tasks[i].m_SelfSettings[3] = TRUE;
			}

			//Check CT Summary
			::Settings.GetButtonName(1, 0, s);
			if (s != "CT Summary")
			{
				SetDefTask_CTSU(::NewSettings.m_Tasks[i].m_WndSettings[4]);
				::NewSettings.m_Tasks[i].m_SelfSettings[4] = TRUE;
			}
			//Done

		}
		else if (i == 2)
		{
			Def_Name = "Corneal Diagnostics";
			Def_Num = 4;

			::NewSettings.m_Tasks[i].m_SubButton[0] = TYPE_CTSU;
			::NewSettings.m_Tasks[i].m_SubButton[1] = TYPE_KERA;
			::NewSettings.m_Tasks[i].m_SubButton[2] = TYPE_WCVA;
			::NewSettings.m_Tasks[i].m_SubButton[3] = TYPE_CHAN;
			::NewSettings.m_Tasks[i].m_SubButton[4] = TYPE_NONA;
			::NewSettings.m_Tasks[i].m_SubButton[5] = TYPE_NONA;
			::NewSettings.m_Tasks[i].m_SubButton[6] = TYPE_NONA;
			::NewSettings.m_Tasks[i].m_SubButton[7] = TYPE_NONA;
			::NewSettings.m_Tasks[i].m_SubButton[8] = TYPE_NONA;
			::NewSettings.m_Tasks[i].m_SubButton[9] = TYPE_NONA;

			//Check CT Summary
			CString s;
			::Settings.GetButtonName(1, 0, s);
			if (s != "CT Summary")
			{
				SetDefTask_CTSU(::NewSettings.m_Tasks[i].m_WndSettings[0]);
				::NewSettings.m_Tasks[i].m_SelfSettings[0] = TRUE;
			}

			//Check Keratometry
			::Settings.GetButtonName(1, 1, s);
			if (s != "Keratometry")
			{
				SetDefTask_CTKE(::NewSettings.m_Tasks[i].m_WndSettings[1]);
				::NewSettings.m_Tasks[i].m_SelfSettings[1] = TRUE;
			}
			//Done

			//Check WF/CT Visual Acuity
			::Settings.GetButtonName(2, 2, s);
			if (s != "Visual Acuity")
			{
				SetDefTask_WCVA(::NewSettings.m_Tasks[i].m_WndSettings[2]);
				::NewSettings.m_Tasks[i].m_SelfSettings[2] = TRUE;
			}
			//Done

			//Check Chang
			::Settings.GetButtonName(2, 0, s);
			if (s != "Chang Analysis")
			{
				SetDefTask_CHAN(::NewSettings.m_Tasks[i].m_WndSettings[3]);
				::NewSettings.m_Tasks[i].m_SelfSettings[3] = TRUE;
			}
			//Done
		}
		else if (i == 3)
		{
			Def_Name = "Toric Planner";
			Def_Num = 1;

			::NewSettings.m_Tasks[i].m_SubButton[0] = TYPE_TORI;
			for (int j = 1; j < 10; j++) ::NewSettings.m_Tasks[i].m_SubButton[j] = TYPE_NONA;
		}
		else if (i == 4)
		{
			if (m_isCombo) //531
			{
				Def_Name = "Accomm and DOF Analysis";

				Def_Num = 2;

				::NewSettings.m_Tasks[i].m_SubButton[0] = TYPE_ACCO;
				::NewSettings.m_Tasks[i].m_SubButton[1] = TYPE_DEPF;
				for (int j = 2; j < 10; j++) ::NewSettings.m_Tasks[i].m_SubButton[j] = TYPE_NONA;
			}
			else
			{
				Def_Name = "Accommdation Analysis";

				Def_Num = 1;

				::NewSettings.m_Tasks[i].m_SubButton[0] = TYPE_ACCO;
				for (int j = 1; j < 10; j++) ::NewSettings.m_Tasks[i].m_SubButton[j] = TYPE_NONA;
			}
		}

		G_As = Def_Name;
		strncpy(&::NewSettings.m_Tasks[i].m_ButtonName[0], G_As, 25);

		::NewSettings.m_Tasks[i].m_SubBtnNum = Def_Num;
	}
}
//Done

//********************************************************************************************
//530
void CMainWnd::SetDefTask_WCVA(CWndSettings InWndSetting[4])
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
void CMainWnd::SetDefTask_CTSU(CWndSettings InWndSetting[4])
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
		InWndSetting[i].m_MapShowEye = 1;//521 change from 0 to 1
		InWndSetting[i].m_MapShowNumbers = 0;
		InWndSetting[i].m_MapShowPupil = 0;
		InWndSetting[i].m_MapShowLimbus = 0;
		InWndSetting[i].m_MapShowCornealVertex = 0;
		InWndSetting[i].m_MapTranslucent = 0;

		InWndSetting[i].SetDefaultScale();
		InWndSetting[i].SetDefaultColors(1);
	}
}
//530
//********************************************************************************************
//530
void CMainWnd::SetDefTask_CTKE(CWndSettings InWndSetting[4])
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
		InWndSetting[i].m_MapShowEye = 1;//521 change from 0 to 1
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
void CMainWnd::SetDefTask_CHAN(CWndSettings InWndSetting[4])
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
void CMainWnd::SetDefTask_WFAN(CWndSettings InWndSetting[4])
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
// Calculate the Opacity grade for excel export
CString  CMainWnd::GetOpacityGrade(CWFExam *WFExam)
{
	CString GradeStr = "";

	CMap2D ThisMap2D;

	WFExam->CreateOpm2D(ThisMap2D, 23.63436, WFExam->m_CurShot, 2);//m_d==1?2:m_d==2?0:1);
																   //531
	real_t mean, std_dev;
	ThisMap2D.ComputeMeanStdDev(mean, std_dev);

	std_dev = fabs(std_dev);

	int  GradeMean, GradeSd;
	real_t Grade;

	if (mean <  0.69)               GradeMean = 4;
	else if (mean >= 0.69 && mean < 0.76) GradeMean = 3;
	else if (mean >= 0.76 && mean < 0.85) GradeMean = 2;
	else if (mean >= 0.85 && mean < 0.91) GradeMean = 1;
	else                                  GradeMean = 0;

	if (std_dev <= 0.03)                  GradeSd = 0;
	else if (std_dev > 0.03 && std_dev <= 0.06) GradeSd = 1;
	else if (std_dev > 0.06 && std_dev <= 0.09) GradeSd = 2;
	else if (std_dev > 0.09 && std_dev <= 0.12) GradeSd = 3;
	else                                        GradeSd = 4;

	Grade = real_t(GradeMean + GradeSd) / 2.0;

	GradeStr.Format(_T("%6.2f"), Grade);

	return GradeStr;
}
//********************************************************************************************
// Calculate the Dli Value for excel export
CString  CMainWnd::GetDliValue(CWFExam *WFExam, CCTExam *CTExam)
{
	CString DliVal = "";

	if (WFExam->m_ScanDiameter < 2500)
	{
		DliVal = "Scan size smaller than 2.5mm";
		return DliVal;
	}

	//531 Calculate ReMTF_Surfaces
	CZernikeSurface DysSuface[3];
	CZernikeSurface ReMTF_Surfaces;

	CZernikeMask DysMask;
	DysMask.SetType(MASK_HO_TOTAL);

	// corneal wavefront surface
	DysSuface[1] = CTExam->m_WfSurface;
	// total wavefront surface
	DysSuface[2] = WFExam->m_WfSurface;

	real_t r_max_um = 2500.0;//06022015 for DLI Branch
	r_max_um = __min(r_max_um, DysSuface[1].m_r_max_um);
	r_max_um = __min(r_max_um, DysSuface[2].m_r_max_um);
	//r_max_um = __min(r_max_um, m_pWndSettings[m_d].m_ZoneRadiusUm);

	DysSuface[1].ChangeRMaxUm(r_max_um);
	DysSuface[1] &= DysMask;

	DysSuface[2].ChangeRMaxUm(r_max_um);
	DysSuface[2] &= DysMask;

	// internal optics wavefront surface
	DysSuface[0] = DysSuface[2];
	DysSuface[0] -= DysSuface[1];


	//surface for correct dysfunctional function;
	real_t um1, um2, um;

	ReMTF_Surfaces = DysSuface[0];
	for (int mode = 6; mode <= 27; mode++)
	{
		if (mode != 7 && mode != 8)
		{
			um1 = DysSuface[0].GetCUm(mode);//Total
			um2 = DysSuface[2].GetCUm(mode);//Internal

			um = (fabs(um1) <= fabs(um2) ? um1 : um2);

			if (mode == 24)
			{
				um = (um > 0.2 ? um - 0.2 : 0);
			}

			ReMTF_Surfaces.SetCUm(mode, um);
		}
	}

	//05292015 for DLI Branch
	real_t newum1 = 0;
	real_t newum2 = 0;
	um1 = DysSuface[1].GetCUm(7);//Corneal
	um2 = DysSuface[0].GetCUm(7);//Internal (Lens)
	real_t um3 = DysSuface[1].GetCUm(8);//Corneal
	real_t um4 = DysSuface[0].GetCUm(8);//Internal (Lens)
	real_t um5 = DysSuface[2].GetCUm(7);//Total
	real_t um6 = DysSuface[2].GetCUm(8);//Total
	CreateAdjustedUm(um1, um2, um3, um4, um5, um6, newum1, newum2);
	ReMTF_Surfaces.SetCUm(7, newum1);
	ReMTF_Surfaces.SetCUm(8, newum2);
	//05292015 for DLI Branch
	//531 Calculate m_ReMTF_Surfaces Done

	real_t StaticDysVal = CalDysVal(r_max_um, ReMTF_Surfaces);

	DliVal.Format(_T("%2.2f"), StaticDysVal);

	return DliVal;
}
// Calculate the Dli Value for excel export
//********************************************************************************************
// Calculate the Dli Value for excel export
void  CMainWnd::CreateAdjustedUm(real_t um1, real_t um2, real_t um3, real_t um4, real_t um5, real_t um6, real_t& newum1, real_t& newum2)
{
	real_t ZC = sqrt(um1*um1 + um3*um3);
	real_t ZL = sqrt(um2*um2 + um4*um4);

	if ((ZL <= ZC) && (ZL > 0.1*ZC))
	{
		real_t CornealDeg = ::GetDegree(um3, um1);
		real_t LensDeg = ::GetDegree(um4, um2);

		real_t adj_CornealDeg = CornealDeg;
		real_t adj_LensDeg = LensDeg;

		if (CornealDeg > LensDeg)
			adj_LensDeg += 180;
		else
			adj_CornealDeg += 180;

		real_t DegreeDis = fabs(adj_CornealDeg - adj_LensDeg);


		real_t ratio = 1.0;

		//Joe's coefficents
		if (DegreeDis <= 5) ratio = 1.0;
		else if (DegreeDis <= 15) ratio = 0.97;
		else if (DegreeDis <= 25) ratio = 0.91;
		else if (DegreeDis <= 35) ratio = 0.82;
		else if (DegreeDis <= 45) ratio = 0.71;

		if (DegreeDis <= 45)
		{
			newum1 = ZC * (1 - ratio) * cos(CornealDeg * _Pi_180);
			newum2 = ZC * (1 - ratio) * sin(CornealDeg * _Pi_180);
		}
		else
		{
			newum1 = (fabs(um2) <= fabs(um5) ? um2 : um5);
			newum2 = (fabs(um4) <= fabs(um6) ? um4 : um6);
		}
	}
	else
	{
		newum1 = (fabs(um2) <= fabs(um5) ? um2 : um5);
		newum2 = (fabs(um4) <= fabs(um6) ? um4 : um6);
	}
}
// Calculate the Dli Value for excel export
//********************************************************************************************
// Calculate the Dli Value for excel export
real_t  CMainWnd::CalDysVal(real_t pupilSize, CZernikeSurface ReMTF_Surfaces)
{
	real_t  index, valNumX;

	pupilSize = pupilSize * 2 / 1000;

	CMTF Mtf;

	CWFExam::CreateMtf(ReMTF_Surfaces, Mtf);

	int w = 427;//Rect.right - Rect.left;
	int h = 375;//Rect.bottom - Rect.top;

	real_t L = 0.100 * w;
	real_t T = 0.180 * h;
	real_t R = 0.900 * w;
	real_t B = 0.860 * h;

	real_t w_px = R - L;
	real_t h_px = B - T;
	int  w_cd = 30;
	real_t px_cd = w_px / w_cd;
	real_t valNumY = T + (B - T) * 3 / 5;

	real_t fa;
	real_t minpos = 1000;//531
	int  k = 0;
	int  t = 0;

	real_t areaW = 0;//601
	real_t areaB = 0;//601
	real_t areaC = 0;//601
	real_t lastxa = L;//601

	for (real_t i = 0; i <= 30; i = i + 0.1)
	{
		real_t xa = L + i * px_cd;
		if (!Mtf.GetAtCdR(i, fa)) break;

		real_t ya = B - fa * h_px;

		areaB += (B - ya)*(xa - lastxa);//601
		areaW += h_px*(xa - lastxa);//601

		real_t disPos = fabs(ya - valNumY);
		if (disPos < minpos)
		{
			valNumX = xa;
			minpos = disPos;
		}

		if (t == 0 || t == 10)
		{
			//avgH += fa;
			k++;
			t = 0;

			if (k == 5) areaC = areaB;//601
		}
		t++;

		lastxa = xa;//601
	}

	areaC = areaB - areaC;//601
	areaC /= areaW;//601

	valNumX = (valNumX - L) / px_cd;

	//calculate the value 601
	index = (sqrt(valNumX) * 2 + (areaC - 0.05) * 10 / 0.35) / (2 * ((5 - (pupilSize + 0.6))*0.03 + 1));
	index = (index > 10 ? 10 : index);
	//Done 601

	return index;
}
// Calculate the Dli Value for excel export
//********************************************************************************************

// Recover the exam patients;05202016 for a customer lost tdb data but has exam files.
void CMainWnd::RecoverFromExams()
{
	CString FolderName = "C:\\Tracey\\Data\\MSJet\\Exams";

	CString FileName, GUIDStr, Str;
	CFileFind finder;

	BOOL working = finder.FindFile(FolderName + "\\*.exam");

	CWFExam* pWFExam = new CWFExam;
	CCTExam* pCTExam = new CCTExam;

	GUID ExamId, PaId;
	CString PatientID[5000];
	CString PatientIDStr;

	BOOL Exist;
	int  IsWFExam;// 0: ct; 1: wf; -1: can not be load

	CExamHeader SavedHeader;

	int n = 0;

	while (working)
	{
		working = finder.FindNextFile();
		if (finder.IsDots())    continue;
		if (finder.IsDirectory())
		{
			//FindAllFile(finder.GetFilePath(), filenames, count);
		}
		else
		{
			IsWFExam = FALSE;

			FileName = finder.GetFileName();

			GUIDStr = FileName;
			GUIDStr.Replace(_T(".exam"), _T(""));

			G_As = GUIDStr;
			::StrToGUID(G_As, ExamId);

			//Load the patient Id
			if (::DB.LoadWFExam(ExamId, pWFExam))
			{
				IsWFExam = TRUE;
				PaId = pWFExam->m_Header.m_PatientID;

				GUIDToStr(PaId, PatientIDStr);

				SavedHeader = pWFExam->m_Header;
			}
			else
			{
				if (::DB.LoadCTExam(ExamId, pCTExam))
				{
					PaId = pCTExam->m_Header.m_PatientID;

					GUIDToStr(PaId, PatientIDStr);

					SavedHeader = pWFExam->m_Header;
				}
				else
					IsWFExam = -1;
			}


			//Load the patient Id Done

			//Create the patient data
			if (IsWFExam != -1)
			{
				Exist = FALSE;
				for (int i = 0; i < n; i++)
				{
					if (PatientIDStr == PatientID[i])
					{
						Exist = TRUE;
						break;
					}
				}

				if (!Exist)
				{
					PatientID[n] = PatientIDStr;

					//Create new patient
					Str.Format(_T("%3.0i"), n + 1);

					CPatient RecoverPatient;

					RecoverPatient.m_LastName = "Recovered Patient" + Str;
					RecoverPatient.m_FirstName = "Tracey Tec.";

					RecoverPatient.m_PatientID = PaId;

					RecoverPatient.m_BirthYear = 2016;
					RecoverPatient.m_BirthMonth = 5;
					RecoverPatient.m_BirthDay = 20;

					//RecoverPatient.m_Note =

					::DB.SavePatient(RecoverPatient);
					//Create new patient Done

					n++;
				}
				//Create the patient data Done

				//::DB.SaveExamHeader(SavedHeader, IsWFExam);
			}
		}
	}

	::DB.Disconnect();
	::DB.Connect(::Settings.m_OpenDBAsReadOnly);

	UpdatePatientsList();
}
//********************************************************************************************

//6.2.0 For Dicom
BOOL CMainWnd::CheckSameMWLPatient(CPatient Import_Patient, int & pos)
{
	pos = -1;

	int size = ::DB.m_Patients.GetSize();
	for (int i = 0; i < size; i++)
	{
		CPatient* EachPatient = &::DB.m_Patients[i];

		if (EachPatient->FullName() == Import_Patient.FullName()
			&& EachPatient->m_BirthYear == Import_Patient.m_BirthYear
			&& EachPatient->m_BirthMonth == Import_Patient.m_BirthMonth
			&& EachPatient->m_BirthDay == Import_Patient.m_BirthDay
			&& EachPatient->m_Sex == Import_Patient.m_Sex)
		{
			pos = i;

			return TRUE;
		}
	}

	return FALSE;
}

//********************************************************************************************

//6.2.0 For Dicom
BOOL CMainWnd::CheckSameDCMPatient(CPatient Import_Patient, CString Import_Patient_ID, int & pos)
{
	pos = -1;

	for (int i = 0; i<::DB.m_Patients.GetSize(); i++)
	{
		CPatient* EachPatient = &::DB.m_Patients[i];

		CString OriID;
		GUIDToStr(EachPatient->m_PatientID, OriID);

		// if the Patient in the DB is not the current added or edited patient
		if (OriID == Import_Patient_ID)
		{
			pos = i;

			return TRUE;
		}
	}

	return FALSE;
}

//6.2.0 For Dicom
//********************************************************************************************
//6.2.0 For Dicom
BOOL  CMainWnd::SameInfo(CPatient NewPatient, CPatient OldPatient)
{
	if ((NewPatient.m_FirstName != OldPatient.m_FirstName) ||
		(NewPatient.m_LastName != OldPatient.m_LastName) ||
		(NewPatient.m_MiddleName != OldPatient.m_MiddleName) ||

		(NewPatient.m_BirthYear != OldPatient.m_BirthYear) ||
		(NewPatient.m_BirthMonth != OldPatient.m_BirthMonth) ||
		(NewPatient.m_BirthDay != OldPatient.m_BirthDay) ||
		(NewPatient.m_Sex != OldPatient.m_Sex)
		)
		return FALSE;

	return TRUE;
}
//***************************************************************************************

//6.2.0 For Dicom
CPatient CMainWnd::ReadFromDicomInfo(CDicomInfo DicomInfo)
{
	CPatient NewPatient;

	GetFiLaMiName(DicomInfo.m_Patient_Name, NewPatient.m_FirstName, NewPatient.m_LastName, NewPatient.m_MiddleName);

	SYSTEMTIME Time;
	::GetLocalTime(&Time);
	NewPatient.m_RegYear = Time.wYear;
	NewPatient.m_RegMonth = Time.wMonth;
	NewPatient.m_RegDay = Time.wDay;
	NewPatient.m_RegHour = Time.wHour;
	NewPatient.m_RegMinute = Time.wMinute;

	//6.2.0 For Dicom Check the Dicom file from Tracey or not
	if (DicomInfo.m_Equi_Manufacture_Model == "iTrace VFA" && DicomInfo.m_Equi_Manufacturer == "Tracey Technologies")
	{
		GUID PatientGuid;
		G_As = DicomInfo.m_Patient_Other_ID;
		StrToGUID(G_As, PatientGuid);;

		NewPatient.m_PatientID = PatientGuid;
	}

	NewPatient.m_CustomID = DicomInfo.m_Patient_ID;
	// For Dicom Check the Dicom file from Tracey or not

	GetBirthday(DicomInfo.m_Patient_BirthDate, NewPatient.m_BirthYear, NewPatient.m_BirthMonth, NewPatient.m_BirthDay);

	NewPatient.m_Sex = (DicomInfo.m_Patient_Sex == "F" ? SEX_FEMALE : DicomInfo.m_Patient_Sex == "M" ? SEX_MALE : SEX_UNKNOWN);

	NewPatient.m_IsDicom = TRUE;

	NewPatient.m_Note = DicomInfo.m_StudyDes;

	return NewPatient;
}
//6.2.0 For Dicom

//********************************************************************************************

//6.2.0 For Dicom
/*
Rev. John Robert Quincy Adams, B.A. M.Div.

"Adams^John Robert Quincy^^Rev.^B.A. M.Div."

[One family name; three given names; no middle name; one prefix; two suffixes.]

Susan Morrison-Jones, Ph.D., Chief Executive Officer

"Morrison-Jones^Susan^^^Ph.D., Chief Executive Officer"

[Two family names; one given name; no middle name; no prefix; two suffixes.]

John Doe

"Doe^John"

[One family name; one given name; no middle name, prefix, or suffix. Delimiters have been omitted for the three trailing null components.]

(for examples of the encoding of Person Names using multi-byte character sets see Annex H)

"Smith^Fluffy"

[A cat, rather than a human, whose responsible party family name is Smith, and whose own name is Fluffy]

"ABC Farms^Running on Water"

[A horse whose responsible organization is named ABC Farms, and whose name is "Running On Water"]
*/
void  CMainWnd::GetFiLaMiName(CString FullName, CString & FirstName, CString & LastName, CString & MiddleName)
{
	CString s[4];
	int Len = FullName.GetLength();

	if (Len == 0)
	{
		FirstName = "";
		LastName = "";
		MiddleName = "";

		return;
	}


	TCHAR *pValue = new TCHAR[Len];

	pValue = (LPTSTR)(LPCTSTR)FullName;

	int t = 0;

	for (int i = 0; i < 4; i++)
	{
		s[i] = "";
	}

	//0x41 A; 0x42 B; 0x43 C; 44 D; 45 E; 46 F; 47 G; 48 H; 49 I; 4A J; 4B K; 4C L;
	//0x4D M; 0x4E N; 0x4F O; 50 P; 51 Q; 52 R; 53 S; 54 T; 55 U; 56 V; 57 W; 58 X;
	//0x59 Y; 0x5A Z;
	for (int i = 0; i < Len; i++)
	{
		TCHAR c = *(pValue + i);

		if (c == '^')
		{
			t++;
		}
		else
		{
			s[t] += c;
		}
	}



	for (int i = 0; i <= t; i++)
	{
		s[i].Trim();
	}

	LastName = s[0];
	FirstName = s[1];
	MiddleName = s[2];
}
//6.2.0 For Dicom

//***************************************************************************************

//6.2.0 For Dicom
void  CMainWnd::GetBirthday(CString BirthDate, int & BirthYear, int & BirthMonth, int & BirthDay)
{
	//"%02i/%02i/%04i", BirthMonth,BirthDay, BirthYear);
	CString s[3], s2[3];

	int Len = BirthDate.GetLength();

	char *pValue = new char[Len];

	G_As = BirthDate;
	strncpy(pValue, G_As, Len);

	int t = 0;
	for (int i = 0; i < Len; i++)
	{
		char c = *(pValue + i);

		if (i < 4)
		{
			s[0] += c;
		}
		else if (i >= 4 && i < 6)
		{
			s[1] += c;
		}
		else if (i >= 6 && i < 8)
		{
			s[2] += c;
		}
	}

	G_As = s[0];
	BirthYear = atoi(G_As);
	G_As = s[1];
	BirthMonth = atoi(G_As);
	G_As = s[2];
	BirthDay = atoi(G_As);
}
//6.2.0 For Dicom

//***************************************************************************************

///////////////////////// #temp

#include "PatientServices.h"

void CMainWnd::RegisterCallbacks()
{
  // Start with the signals
  App.m_siPatientAdded.Connect([this](const CPatient *p) {
    m_Patient = *p;
    UpdatePatientsList();
  });

  auto pat = [this]() -> std::vector<GUID> {
    std::vector<GUID> data;

    POSITION Pos = m_PatientsList.GetFirstSelectedItemPosition();
    while (Pos) {
      int p = (int)m_PatientsList.GetItemData(m_PatientsList.GetNextSelectedItem(Pos));
      data.push_back(CDB::Instance()->m_Patients[p].m_PatientID);
    }

    return data;
  };
  PatientServices::Instance()->RegisterPatientCallback(pat);

  auto exam = [this](EYE eye, PatientServices::Type type) {
    CXPListCtrl *list{};
    switch (type) {
    case PatientServices::Type::WF:
      list = eye == EYE::OD ? &m_WFODList : &m_WFOSList;
      break;

    case PatientServices::Type::CT:
      list = eye == EYE::OD ? &m_CTODList : &m_CTOSList;
      break;

    default:
      break;
    }

    std::vector<GUID> data;
    if (list) {
      POSITION pos = list->GetFirstSelectedItemPosition();
      while (pos) {
        int i = list->GetNextSelectedItem(pos);
        int e = list->GetItemData(i);

        data.push_back(CDB::Instance()->m_ExamHeaders[e].m_ExamID);
      }
    }

    return data;
  };

  PatientServices::Instance()->RegisterExamCallback(exam);
}
