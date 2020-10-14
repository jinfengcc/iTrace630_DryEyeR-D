//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Btn.h"
#include "XPMenu.h"
#include "XPListCtrl.h"
#include "XPListCtrla.h"
#include "Data.h"
#include "WFSingleVrfWnd.h"
#include "WFSingleSumWnd.h"
#include "WFSingleCmpWnd.h"
#include "WFDoubleSumWnd.h"
#include "CTSingleVrfWnd.h"
#include "CTSingleSumWnd.h"
#include "CTSingleCmpWnd.h"
#include "CTDoubleSumWnd.h"
#include "WFCTSumWnd.h"
#include "WFMultiSumWnd.h"
#include "WTSumWnd.h"
#include "WFSingleSoloWnd.h"
#include "WFDoubleSoloWnd.h"
#include "WFSoloVfaWnd.h"
#include "WFSoloCmpWnd.h"
#include "PlannerWnd.h"
#include "PlanBigWnd.h"
#include "PlanBigWnd.h"
#include "CTOsherAliWnd.h"
#include "WFCTOUSumWnd.h"
#include "ImgInfoWnd.h"
#include "PatientChartDlg.h"
#include "WFCTIolWnd.h"
#include "WFCTDysWnd.h"
#include "WFCTDysPtWnd.h"

//***************************************************************************************

#define NUM_EXAMS_MAX 1024

//***************************************************************************************

class CMainWnd : public CWnd
{
public:

	CMainWnd();
	virtual ~CMainWnd();

private:

#define TIMER1  1 
#define TIMER2  2 //for tip of patient note delay

	int m_q;

	int m_ExamsType;//[cjf***04022012], used to check the mutiple Exams type: 
					//0: Unsure; 1: Muti-WFOD; 2: Muti-WFOS; 

					//3: one WF-OD first and then one WF-OS /12
					//4: one WF-OS first and then one WF-OD /21

					//5: one WF-OD First and then one CT-OD /13
					//6: one CT-OD First and then one WF-OD /31

					//7: one WF-OS First and then one CT-OS /24
					//8: one CT-OS First and then one WF-OS /42

					//9: one CT-OD First and then one CT-OS /34
					//10:one CT-OS First and then one CT-OD /43

	CStringArray m_IndexSelectedExams;
	int m_TotPatCount;// Total patient Counts
	int m_Shift;
	int m_OD;
	int m_OpenTime;// time to allow the user select another exam 

	CPatient    m_Patient;

	CWFExam    *m_pWFExams[NUM_EXAMS_MAX];
	CCTExam    *m_pCTExams[NUM_EXAMS_MAX];

	int         m_FontSize;
	CMFont      m_Font;

	enum
	{
		LEVEL_MAIN,
		LEVEL_PATIENT,
		LEVEL_VIDEO,
		LEVEL_SUMMARY
	} m_Level;

	enum
	{
		LAST_LEVEL_MAIN,
		LAST_LEVEL_PATIENT,
		LAST_LEVEL_VIDEO,
		LAST_LEVEL_SUMMARY,
	} m_Last_evel;

	HACCEL      m_hAccel;

	GUID        m_ImportID;

	RECT        m_BR[3][16];

	int         m_selK;//531

	CMFont      m_LabelsFont;
	CMFont      m_LabelsFont2;
	CMFont      m_VersionFont;

	virtual BOOL PreTranslateMessage(MSG* pMsg) override;
	virtual void PostNcDestroy() override;

	afx_msg void OnPaint();
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD_PTR dwData);

	// main level  
	RECT        m_PatientsListRect;
	RECT        m_PatientCountRect; // the rect of patient count showing position
	CMFont      m_PatientCountFont;
	CMFont      m_DateBaseFolderFont;
	RECT        m_ExcelExportRect; 
	RECT        m_DataBaseFolderRect; 

	Matrix<CString> m_GroupNames;

	CBtn        m_NewPatientButton;
	CBtn        m_SelectPatientButton;
	CBtn        m_EditPatientButton;
	CBtn        m_DeletePatientButton;
	CBtn        m_ImportPatientButton;
	CBtn        m_ExportPatientButton;
	CBtn        m_SettingsButton;
	CBtn        m_LicenseButton;
	CBtn        m_ExportExcelButton;
	CBtn        m_TestButton;
	CBtn        m_BackButton;
	CXPListCtrla m_PatientsList;
	CEdit       m_CustomIDEdit;
	CEdit       m_LastNameEdit;
	CEdit       m_FirstNameEdit;
	CEdit       m_MiddleNameEdit;
	CComboBox   m_GroupsComboBox;
	CEdit       m_DOBEdit;
	CComboBox   m_SexComboBox;
	CBtn        m_RegTimeButton;//this button is used to let the user to check the patients in a time range 
	CBtn        m_SearchDefButton;
	CBtn        m_RefrshButton;
	CBtn        m_DatabaseButton;
	//CBtn        m_DataTestButton;// temporary Database Test

	int         m_InputDOBLen;

	void RecoverFromExams();// Recover the exam patients;05202016 for a customer lost tdb data but has exam files.

	GUID GetIDFromS(CString s);
	void ShowMainLevelControls();
	void HideMainLevelControls();
	BOOL LessTime(int Year, int Month, int Day, CString ComTime);//wether the regtime is less than or equal to the compared time
	BOOL GreatTime(int Year, int Month, int Day, CString ComTime);//wether the regtime is greater  than or equal to the compared time
	BOOL IsLetter(CString s);

	void  SetDef_TaskSettings();
	void  SetDefTask_WCVA(CWndSettings InWndSetting[4]);
	void  SetDefTask_CTSU(CWndSettings InWndSetting[4]);
	void  SetDefTask_CTKE(CWndSettings InWndSetting[4]);
	void  SetDefTask_CHAN(CWndSettings InWndSetting[4]);
	void  SetDefTask_WFAN(CWndSettings InWndSetting[4]);

	void  ChangeLicese();// First activation, used to change the licensing

	BOOL  ReadMWLInfo();//6.2.0 New Dicom

	CString  GetOpacityGrade(CWFExam *WFExam);// Calculate the Opacity grade for excel export
	CString  GetDliValue(CWFExam *WFExam, CCTExam *CTExam);// Calculate the Dli Value for excel export
	void     CreateAdjustedUm(real um1, real um2, real um3, real um4, real um5, real um6, real& newum1, real& newum2);// Calculate the Dli Value for excel export
	real     CalDysVal(real pupilSize, CZernikeSurface ReMTF_Surfaces);// Calculate the Dli Value for excel export

	CPatient ReadFromDicomInfo(CDicomInfo DicomInfo);//6.2.0 For Dicom
	void     GetFiLaMiName(CString FullName, CString & FirstName, CString & LastName, CString & MiddleName);//6.2.0 For Dicom	
	void     GetBirthday(CString BirthDate, int & BirthYear, int & BirthMonth, int & BirthDay);//6.2.0 For Dicom

	afx_msg void UpdatePatientsList();

	static int CALLBACK CompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);//6.2.0 Sort the column of patient list

	afx_msg void OnPatientListHOVER(NMHDR* pNotifyStruct, LRESULT* pRes);

	afx_msg void OnNewPatientButtonClicked();
	afx_msg void OnRegButtonClicked();//map to m_RegTime
	afx_msg void OnEraseSearchButtonClicked();
	afx_msg void OnRefreshDataButtonClicked();
	afx_msg void OnDatabaseButtonClicked();
	// afx_msg void OnDatabaseTestButtonClicked();// Database test  
	afx_msg void OnSelectPatientButtonClicked();
	afx_msg void OnEditPatientButtonClicked();
	afx_msg void OnDeletePatientButtonClicked();
	afx_msg void OnImportPatientButtonClicked();
	afx_msg void OnExportPatientButtonClicked();
	afx_msg void OnSettingsButtonClicked();
	afx_msg void OnLicenseButtonClicked();
	afx_msg void OnTestButtonClicked();
	afx_msg void OnExcelButtonClicked();
	afx_msg void OnBackButtonClicked();
	afx_msg void OnPatientsListDblClicked(NMHDR* pNMHDR, LRESULT* pRes);
	afx_msg void OnPatientsListColumnClicked(NMHDR* pNMHDR, LRESULT* pRes);
	afx_msg void OnCtrlD();
	afx_msg void OnCtrlW();
	afx_msg void OnCtrlA();
	afx_msg void OnCtrlY();
	afx_msg void OnCtrlZ();

	// patient level 
	BOOL        m_Update;
	BOOL        m_RightClick;//is the right mouse clicking
	RECT        m_ExamsListRect[6];
	RECT        m_LabelsRect[4];// Show the title of four exam lists
	RECT        m_Labels2Rect[4];//Show the exam numbers of four exam lists
	RECT        m_NameRect;
	RECT        m_CustomIDRect;
	RECT        m_VersionRect;

	RECT        m_ImgInfoRect;

	int         m_WhichTaskBtn;
	int         m_TaskSubK;
	int         m_Ori_TaskSubK;
	int         m_FirstTask;

	int         m_NumSelectedWFODExams;
	int         m_NumSelectedWFOSExams;
	int         m_NumSelectedCTODExams;
	int         m_NumSelectedCTOSExams;
	int         m_NumSelectedWFExams;
	int         m_NumSelectedCTExams;
	int         m_NumLoadedWFODExams;
	int         m_NumLoadedWFOSExams;
	int         m_NumLoadedCTODExams;
	int         m_NumLoadedCTOSExams;
	int         m_NumLoadedWFExams;
	int         m_NumLoadedCTExams;
	int         m_WFOD[NUM_EXAMS_MAX];
	int         m_WFOS[NUM_EXAMS_MAX];
	int         m_CTOD[NUM_EXAMS_MAX];
	int         m_CTOS[NUM_EXAMS_MAX];
	GUID        m_WFODID[NUM_EXAMS_MAX];
	GUID        m_WFOSID[NUM_EXAMS_MAX];
	GUID        m_CTODID[NUM_EXAMS_MAX];
	GUID        m_CTOSID[NUM_EXAMS_MAX];

	int         m_LastExam;
	int         m_LastRow;
	BOOL        m_ShowInfo;
	CImgInfoWnd m_ExamImgInfo;
	CImgInfoWnd m_NoteImgInfo;

	CBtn        m_NewWFExamButton;
	CBtn        m_NewCTExamButton;
	CBtn        m_NewWFCTExamButton;
	CBtn        m_ViewExamButton;
	BOOL        m_CustExamCreated[5];
	CBtn        m_ViewCustExamButton[5];// to view customed exam
	CBtn        m_EditExamButton;
	CBtn        m_DeleteExamButton;
	CBtn        m_TopconButton;
	CXPListCtrl m_WFODList;
	CXPListCtrl m_WFOSList;
	CXPListCtrl m_CTODList;
	CXPListCtrl m_CTOSList;

	BOOL CheckSameMWLPatient(CPatient Import_Patient, int &pos);//6.2.0 For Dicom
	BOOL CheckSameDCMPatient(CPatient Import_Patient, CString Import_Patient_ID, int &pos);//6.2.0 For Dicom
	BOOL SameInfo(CPatient NewPatient, CPatient OldPatient);//6.2.0 For Dicom

	void ShowPatientLevelControls();
	void HidePatientLevelControls();

	void DeleteSelectedIndicesLists();
	void CreateSelectedIndicesLists();

	BOOL LoadSelectedExams();
	BOOL LoadFellowExam();
	void DeleteLoadedExams();

	void AdjustSelectedItems(int WhichExam, int Row);//select or un-select the items according to the logic
	void DelSomeSelExams(int StatrPos, int Num, CStringArray* Index);// deleted some mis - slected exams from the Exam Lists
	void FixSelectionMistake();//sometimes mistake happen while click exams lists too fast	

	afx_msg void UpdateExamsLists();

	afx_msg void OnExamsListDblClicked(NMHDR* pNotifyStruct, LRESULT* pRes);
	afx_msg void OnExamsListClicked(NMHDR* pNotifyStruct, LRESULT* pRes);
	afx_msg void OnExamsListRClicked(NMHDR* pNotifyStruct, LRESULT* pRes);

	afx_msg void OnExamsListHOVER(NMHDR* pNotifyStruct, LRESULT* pRes);

	afx_msg void OnCtrlL();
	afx_msg void OnCtrlM();
	afx_msg void OnCtrlN();

	afx_msg void OnCtrlQ();// for a new similar ctrl+w window

	afx_msg void OnNewWFExamButtonClicked();
	afx_msg void OnNewCTExamButtonClicked();
	afx_msg void OnNewWFCTExamButtonClicked();
	afx_msg void OnViewExamButtonClicked();
	afx_msg void OnEditExamButtonClicked();
	afx_msg void OnDeleteExamButtonClicked();
	afx_msg void OnTopconButtonClicked();

	// Task btns
	BOOL OnViewExamBtnAClicked(int TaskNum);
	void OnViewExamBtnBClicked();
	afx_msg void OnViewExam0BtnClicked();
	afx_msg void OnViewExam1BtnClicked();
	afx_msg void OnViewExam2BtnClicked();
	afx_msg void OnViewExam3BtnClicked();
	afx_msg void OnViewExam4BtnClicked();
	// Task btns Done

	// used to sort the exam list
	static int CALLBACK CompareWFODExamProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK CompareWFOSExamProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK CompareCTODExamProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK CompareCTOSExamProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	afx_msg void OnWFODExamListColumnClicked(NMHDR* pNMHDR, LRESULT* pRes);
	afx_msg void OnWFOSExamListColumnClicked(NMHDR* pNMHDR, LRESULT* pRes);
	afx_msg void OnCTODExamListColumnClicked(NMHDR* pNMHDR, LRESULT* pRes);
	afx_msg void OnCTOSExamListColumnClicked(NMHDR* pNMHDR, LRESULT* pRes);
	// sort the exam list 

	// summary level
	int         m_AcceptTime;// While accepting the exam, use this parameter to control the excel export 
	BOOL        m_ExcelSaved;// While accepting the exam, use this parameter to control the excel export 
	int         m_TransType;// For Dicom file, judege the summary GUI has been 'OnToggleButtonClicked',initialize to 0 in the 'OnViewExamButtonClicked'
	BOOL        m_SoloControl;// For solo control

	BOOL        m_FellowIDCaled;//  the fellow ID have been checked or not.
	CString     m_FellowIDStr[NUM_EXAMS_MAX];// fellow id arrat
	BOOL        m_ReselectExam;

	RECT        m_SumRect;
	RECT        m_BigRect;

	CDicomInfo  m_DicomInfo;// Dicom file Information set 

	BOOL        m_WFCompareDone; 
	BOOL        m_CTCompareDone; 

	CBtn        m_AcceptExamButton;
	CBtn        m_RejectExamButton;
	CBtn        m_ToggleButton;
	CBtn        m_WFVrfButton;
	CBtn        m_WFVfaButton;
	CBtn        m_WFCmpButton;
	CBtn        m_WFSumButton[7];//6.2.0 From [6] to [7] add Button for 'WF Near vision'
	CBtn        m_CTVrfButton;
	CBtn        m_CTDblButton;
	CBtn        m_CTCmpButton;
	CBtn        m_CTSumButton[5];
	CBtn        m_INSumButton[11];
	CBtn        m_TaskButton[8];//Task sub-buttons
	CBtn        m_ToricPlannerButton;
	CBtn        m_WavetouchButton;
	CBtn        m_ExportOrderButton;
	CBtn        m_ExportScreenButton;
	CBtn        m_SaveScreenButton;
	CBtn        m_PrintScreenButton;

	BOOL        m_EnableToggleButton;
	BOOL        m_EnablePlannerButton;
	BOOL        m_EnableWavetouchButtons;
	BOOL        m_EnableEditExamButton;
	BOOL        m_EnableExportDataButton;

	enum
	{
		SCREEN_NONE,//0
		SCREEN_WFVRF,
		SCREEN_WFSUM1,
		SCREEN_WFSUM2,
		SCREEN_WFSUM3,
		SCREEN_WFSUM4,
		SCREEN_WFSUM5,
		SCREEN_WFSUM6,
		SCREEN_WFSUM7,//6.2.0
		SCREEN_WFCMP,
		SCREEN_WFVFA,//10
		SCREEN_WFMLT,
		SCREEN_CTVRF,
		SCREEN_CTSUM1,
		SCREEN_CTSUM2,
		SCREEN_CTSUM3,
		SCREEN_CTSUM4,//16
		SCREEN_CTSUM5,//531
		SCREEN_CTCMP,
		SCREEN_CTDBL,
		SCREEN_INSUM1,
		SCREEN_INSUM2,
		SCREEN_INSUM3,
		SCREEN_INSUM4,
		SCREEN_INSUM5,//OU Overview
		SCREEN_INSUM6,// IOL Selection
		SCREEN_INSUM7,// IOL Selection
		SCREEN_INSUM8,
		SCREEN_INSUM9,
		SCREEN_INSUM10,
		SCREEN_INSUM11,// WFCT Angle K/A
		SCREEN_INSUM12,// Toric Alignment
		SCREEN_TASKSUB0,// Task sub-button
		SCREEN_TASKSUB1,// Task sub-button
		SCREEN_TASKSUB2,// Task sub-button
		SCREEN_TASKSUB3,// Task sub-button
		SCREEN_TASKSUB4,// Task sub-button
		SCREEN_TASKSUB5,// Task sub-button
		SCREEN_TASKSUB6,// Task sub-button
		SCREEN_TASKSUB7,// Task sub-button
		SCREEN_TASKSUB8,// Task sub-button
		SCREEN_WAVETOUCH,
		SCREEN_TORIC_PLANNER,
		SCREEN_BIG_TORIC_PLANNER,
		SCREEN_BIG_TORIC_ICL_PLANNER,//6.2.0
		SCREEN_BIG_CT_OSHER1,
		SCREEN_BIG_CT_OSHER2,
		SCREEN_BIG_CT_OSHER3,
		SCREEN_BIG_CT_OSHER4,
		SCREEN_BIG_CT_OSHER5,
		SCREEN_SAVE_COLOR_IMG0,
		SCREEN_SAVE_COLOR_IMG1
	} m_Screen;

	BOOL     m_DisScreen;//0: wf screen 1: ct screen ; 2:wf/ct screen

	int      m_whichWFExam;
	BOOL     m_WFExamAcc[3];
	CSumWnd* m_pSumWnd;
	CPlanBigWnd*    m_BigPlanWnd;
	CCTOsherAliWnd* m_BigCTSumWnd;
	int  m_SummaryType;// 0:WF 1:CT 2 SUMMARY; 3 From Task Button

	GUID m_ExamIDs[NUM_EXAMS_MAX];// solo multiple selection
	int  m_SelNum;// solo multiple selection

	//CString m_DataFolderName;//Big Database Research since 03062019	
	//void SplitExamHeaderToText();//Big Database Research since 03062019

	void ShowCommonSummaryLevelButtons();
	void HideCommonSummaryLevelButtons();
	void ShowWFSummaryLevelButtons();
	void HideWFSummaryLevelButtons();
	void ShowCTSummaryLevelButtons();
	void HideCTSummaryLevelButtons();
	void ShowINSummaryLevelButtons();
	void HideINSummaryLevelButtons();

	void ShowTaskButtons(int TaskNum);
	void HideTaskButtons();

	void HideAllSummaryLevelButtons();

	void HighlightSummaryLevelButtons();

	void HighlightTaskSubButtons();

	void OnWFSumButtonClicked(int k);
	void OnCTSumButtonClicked(int k);
	void OnINSumButtonClicked(int k);

	void OnTaskSubBtnClicked(int k);

	void SetDicomStduyDesForToggle();//the 'study des' item in the Dicom file while the summary GUI has been 'OnToggleButtonClicked'

	int  SaveWFExamToExcel(CWFExam *WFExam, CString FileName);// For exporting the WF exam to spreadsheet
	int  SaveCTExamToExcel(CCTExam *CTExam, CString FileName);// For exporting the CT exam to spreadsheet
	int  SaveWFCTExamToExcel(CWFExam *WFExam, CCTExam *CTExam, CString FileName);// For exporting the WF and CT exams to spreadsheet

	void OnTimer(UINT_PTR nIDEvent);
	void SaveMultWFCTExams();

	afx_msg void OnAcceptExamButtonClicked();
	afx_msg void OnRejectExamButtonClicked();
	afx_msg void OnToggleButtonClicked();
	afx_msg void OnWFVrfButtonClicked();
	afx_msg void OnWFVfaButtonClicked();
	afx_msg void OnWFCmpButtonClicked();
	afx_msg void OnWFSum1ButtonClicked() { OnWFSumButtonClicked(0); }
	afx_msg void OnWFSum2ButtonClicked() { OnWFSumButtonClicked(1); }
	afx_msg void OnWFSum3ButtonClicked() { OnWFSumButtonClicked(2); }
	afx_msg void OnWFSum4ButtonClicked() { OnWFSumButtonClicked(3); }
	afx_msg void OnWFSum5ButtonClicked() { OnWFSumButtonClicked(4); }
	afx_msg void OnWFSum6ButtonClicked() { OnWFSumButtonClicked(5); }
	afx_msg void OnWFSum7ButtonClicked() { OnWFSumButtonClicked(6); }//6.2.0 WF Near Vision

	afx_msg void OnCTVrfButtonClicked();
	afx_msg void OnCTDblButtonClicked();
	afx_msg void OnCTCmpButtonClicked();
	afx_msg void OnCTSum1ButtonClicked() { OnCTSumButtonClicked(0); }
	afx_msg void OnCTSum2ButtonClicked() { OnCTSumButtonClicked(1); }
	afx_msg void OnCTSum3ButtonClicked() { OnCTSumButtonClicked(2); }
	afx_msg void OnCTSum4ButtonClicked() { OnCTSumButtonClicked(3); }
	afx_msg void OnCTSum5ButtonClicked() { OnCTSumButtonClicked(4); }
	afx_msg void OnINSum1ButtonClicked() { OnINSumButtonClicked(0); }
	afx_msg void OnINSum2ButtonClicked() { OnINSumButtonClicked(1); }
	afx_msg void OnINSum3ButtonClicked() { OnINSumButtonClicked(2); }
	afx_msg void OnINSum4ButtonClicked() { OnINSumButtonClicked(3); }
	afx_msg void OnINSum5ButtonClicked();// ou overview
	afx_msg void OnINSum6ButtonClicked();// IOL Selection
	afx_msg void OnINSum7ButtonClicked() { OnINSumButtonClicked(6); }// Astigmatism Source Display
	afx_msg void OnINSum8ButtonClicked() { OnINSumButtonClicked(7); }
	afx_msg void OnINSum9ButtonClicked() { OnINSumButtonClicked(8); }
	afx_msg void OnINSum10ButtonClicked() { OnINSumButtonClicked(11); }// WFCT Angle k/a
	afx_msg void OnINSum11ButtonClicked() { OnINSumButtonClicked(12); }// Toric Alignment

	afx_msg void OntTaskSubBtn0Clicked() { OnTaskSubBtnClicked(0); }
	afx_msg void OntTaskSubBtn1Clicked() { OnTaskSubBtnClicked(1); }
	afx_msg void OntTaskSubBtn2Clicked() { OnTaskSubBtnClicked(2); }
	afx_msg void OntTaskSubBtn3Clicked() { OnTaskSubBtnClicked(3); }
	afx_msg void OntTaskSubBtn4Clicked() { OnTaskSubBtnClicked(4); }
	afx_msg void OntTaskSubBtn5Clicked() { OnTaskSubBtnClicked(5); }
	afx_msg void OntTaskSubBtn6Clicked() { OnTaskSubBtnClicked(6); }
	afx_msg void OntTaskSubBtn7Clicked() { OnTaskSubBtnClicked(7); }

	afx_msg void OnToricPlannerButtonClicked();
	afx_msg void OnWavetouchButtonClicked();
	afx_msg void OnExportScreenButtonClicked();
	afx_msg void OnExportOrderButtonClicked();
	afx_msg void OnSaveScreenButtonClicked();
	afx_msg void OnPrintScreenButtonClicked();
	afx_msg void OnF9();

	afx_msg BOOL OnMouseWheel(uint nFlags, short zDelta, CPoint Point);
	afx_msg void OnMouseMove(uint nFlags, CPoint Point);

	afx_msg LRESULT OnBigPlanWndShown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCTOSHERALGWndShown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCTOSHERALGWndHide(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSoloHypeLiner(WPARAM wParam, LPARAM lParam);

	afx_msg void OnF1();

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************