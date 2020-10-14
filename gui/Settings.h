//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Surface.h"
#include "Data.h"

//***************************************************************************************
//for color pupil detection

struct OnePoint
{
	int x;
	int y;
};

struct HSI
{
	real H;
	real S;
	real I;
};

//***************************************************************************************

class CScale
{
public:

	BOOL     m_CentAuto;
	real     m_Cent;
	BOOL     m_StepAuto;
	real     m_Step;
	int      m_NumColors;
	COLORREF m_Colors[21];

	void LoadColors(const CString& FileName);
	void SaveColors(const CString& FileName);

};

//***************************************************************************************

class CWndSettings
{
public:

	int          m_Type;
	BOOL         m_Method3D;
	BOOL         m_SizeSmall;
	int          m_ZoneRadiusUm;
	CZernikeMask m_Mask;
	BOOL         m_Piston;
	BOOL         m_Tilt;

	BOOL         m_MapShowEye;
	BOOL         m_MapShowPupil;
	BOOL         m_MapShowCornealVertex;
	BOOL         m_MapShowLimbus;
	BOOL         m_MapShowWavetouchLens;
	BOOL         m_MapShowPoints;
	BOOL         m_MapShowRings;
	BOOL         m_MapShowSolidSurface;
	BOOL         m_MapShowWireMesh;

	BOOL         m_ShowEEF;

	BOOL         m_MTFAverage;
	int          m_MTFAngle;

	int          m_LtrLine;

	BOOL         m_RMSIndividual;

	int          m_RefSurfaceType;
	real         m_RefSurfaceAx0Um;
	real         m_RefSurfaceQ;

	int          m_Ltr;
	int          m_LtrOrientation;
	int          m_EEFType;

	BOOL         m_MapTranslucent;
	int          m_MapUnit;
	BOOL         m_MapShowMap;
	BOOL         m_MapShowNumbers;
	BOOL         m_MapShowKeratometry;
	BOOL         m_MapShowSimKAxes;
	BOOL         m_MapShowAstigmatismAxes;
	BOOL         m_MapShowOsherRing;
	BOOL         m_MapShowZaldivarCaliper;

	//uchar        m_Reserved2[16];

	uchar        m_Reserved2[3];
	uchar        m_LtrDistanceType;
	BOOL         m_SimplifiedRMS;// 4 bytes, show simplified RMS or not
	BOOL         m_SimplifiedWFPanel;// 4 bytes, show simplified WF Info panel or not
	BOOL         m_SimplifiedCTPanel;// 4 bytes, show simplified CT Info panel or not    

	CScale* GetScale();
	real GetIncrement();
	real GetMaxStep();
	void SetDefaultScale();

	void SetDefaultColors(int ColorsIndex);
	BOOL AreDefaultColors(int ColorsIndex);
	BOOL AreDefaultColors();

private:

	CScale       m_Scale[8];
};

//***************************************************************************************

//task Setting
class CTaskSetting
{
public:

	BOOL     m_Enable;
	char     m_ButtonName[25];
	int      m_SubBtnNum;//How many sub buttons in this task
	int      m_DefNum;//Which display(button) is default GUI
	int      m_SubButton[8]; //Button type (which display)

	BOOL     m_SelfSettings[8];//It is Self setting

	char     m_ChButtonName[200];//6.2.0 Chinese

	CWndSettings m_WndSettings[8][4];

	void     SaveBtnName(CString BtnName);//6.2.0 Chinese
	CString  ReadBtnName();//6.2.0 Chinese
};

//***************************************************************************************

class CPlannerSettings
{
public:
	BOOL         m_MapShowIncision;
	BOOL         m_MapShowPreopCorneal;
	BOOL         m_MapShowPreopInternal;
	BOOL         m_MapShowPreopEntireEye;
	BOOL         m_MapPostopCorneal;
	BOOL         m_MapPostopInternal;
	BOOL         m_MapPostopEntireEye;
	BOOL         m_NewPlaner;
	BOOL         m_MapShowICLIndSteep;
	BOOL         m_MapShowICLIndFlat;
};

//***************************************************************************************

class CSettings
{
public:

	CSettings();

	BOOL         m_PositiveCylinder;
	int          m_VertexDistanceUm;
	int          m_ZoneDiameterUm[3];
	BOOL         m_ProbingEnabled;
	BOOL         m_FoggingEnabled;
	int          m_ManualScanDiameterUm;
	BOOL         m_AnimateAfterNewExam;
	int          m_PatientsListColumn;
	BOOL         m_PatientsListAscending;
	uchar        m_DateFormat;
	int          m_AlignmentMethod;
	BOOL         m_ShowWavetouch;
	BOOL         m_AccommTargetLightOff;
	BOOL         m_ShowSendARKRButton;
	BOOL         m_UseJPGFormat;
	BOOL         m_IncludeLADs;
	BOOL         m_ShowHoyaLenses;
	uchar        m_DefaultWFSum;
	uchar        m_DefaultCTSum;
	uchar        m_DefaultINSum;
	BOOL         m_IsCustomvis;
	BOOL         m_OpenDBAsReadOnly;
	uchar        m_Reserved1[88];

	CWndSettings m_TSvrSettings[4];       //CWFSingleVrfWnd       
	CWndSettings m_TSsmSettings[4][4];    //CWFSingleSumWnd       
	CWndSettings m_TScmSettings;          //CWFSingleCmpWnd       
	CWndSettings m_TDsmSettings[4];       //CWFDoubleSumWnd       
	CWndSettings m_TMsmSettings;          //CWFMultiSumWnd        

	CWndSettings m_CSvrSettings;          //CCTColorImgWnd        
	CWndSettings m_CSsmSettings[4][4];    //CCTSingleSumWnd       
	CWndSettings m_CScmSettings;          //                     
	CWndSettings m_CDsmSettings[2];       //                      

	CWndSettings m_IDsmSettings[4][4];    //                      

	CWndSettings m_WTsmSettings[3];       //                                 

	CWndSettings m_PlannerSettings;       //                          

										  // sizeof(CWndSettings) = 1304
										  //uchar        m_Reserved2[760];
										  //uchar        m_Reserved2[752];//, (760 - 8) for m_Cap_ColorImg and m_Cap_ColorAutoOrManu, Two BOOL Used 8 Bytes

	int          m_AccommTargetLightTime;//  4 BYTES
	BOOL         m_ShowSimK;// 4 BYTES 
	char         m_ButtonNames2[3][3][20];// 3*3*20 = 180
	uchar        m_Reserved2[350];// (752 - 4 - 210 - 4 - 4) // new 530-180 = 350

	int          m_RefractionRounding;

	char         m_ButtonNames[3][5][20];
	GUID         m_OperatorID;
	GUID         m_PhysicianID;
	GUID         m_ClinicID;

	BOOL         m_Cap_ColorImg;// Capture the color image or not;
	BOOL         m_Cap_ColorAutoOrManu;// Capture the color image Automatically or Manually;

	uchar        m_EXCEL_WF_SETS[53];
	uchar        m_EXCEL_CT_SETS[68];
	uchar        m_EXCEL_WFCT_SETS[89];//53 + 68 + 89 = 210 Bytes;  
	BOOL         m_Export_Excel;//Export excel or not;

	int          m_AutoScanDiameterUm;

	char         m_ChButtonNames[3][5][200];//6.2.0 Chinese ButtonName 3*5*200 = 3000
	char         m_ChButtonNames2[3][3][200];//6.2.0 Chinese ButtonName new 3*3*200 = 1800

	BOOL LoadFromFile(const CString& FileName);
	void SaveIntoFile(const CString& FileName);
	void LoadFromResource();
	void Check();
	void SetSomeDefault();
	void GetButtonName(int i, int j, CString& Name);
	void SetButtonName(int i, int j, const CString& Name);
};

//***************************************************************************************

//define some global paratmeters here
class CTempSettings
{
public:
	real Com_r_max_um;//used to check the increase and decrease range
	CPlannerSettings NewPlannerSettings;//[cjf***05042012*005*],used to maintain the more planner setting 
	BOOL Temp_m_Edit;//whether it is edit or new of group/clinic/physician/operator
	BOOL Temp_Deleted;//whether the clinic/physician/operator are deleted
	BOOL Temp_ScaleRang; // True means the toric planner scale range is from 0-180
	BOOL Temp_WFAngel;//The current display is WF Angle K/A display

	int  CTSumNum;

	BOOL Temp_ColorImgCpted;// the color Image has been capture;

	real Com_r_max_um1;//used to check the increase and decrease range for VFA screen
	real Com_r_max_um2;//used to check the increase and decrease range for VFA screen  
};

//***************************************************************************************

//new Settings for some new adding attributes
class C531NewSettings
{
public:

	C531NewSettings();

	int          m_IncLoC_OD;
	int          m_IncLoC_OS;
	real         m_SurInAst_OD;
	real         m_SurInAst_OS;
	BOOL         m_Adjust_CT;
	BOOL         m_Adjusting_CT;
	real         WFOD_COL_Width[11];//the width of WFODLIST columns
	real         WFOS_COL_Width[11];//the width of WFOSLIST columns
	real         CTOD_COL_Width[11];//the width of CTODLIST columns
	real         CTOS_COL_Width[11];//the width of CTOSLIST columns

	int          m_OSHER_ALIG_CUS[4];//for the CT OSHER alignment custom

	CWndSettings m_OWFCTSettings[4];//CWFCTOUSumWnd

	int          m_ScreenShotFormat;// 0: jpg(default) ; 1: png;  2: bmp; 3: dcm;

	CWndSettings m_WFSoloSettings[2][4];    // SOLO Aberration and Refraction analysis settings

	uchar        m_DefaultWFSolo;

	CWndSettings m_IOLSelSettings[6];// IOL SELECTION wnd

	real         m_IOLSel_Alpha[2];// IOL SELECTION threshold
	real         m_IOLSel_CHOA;// IOL SELECTION Corneal hoa threshold
	real         m_IOLSel_Astig;// IOL SELECTION threshold
	real         m_IOLSel_Sph_Abbe[4];// IOL SELECTION threshold

	CWndSettings m_InAstigSettings[4]; // Astigmatism Source Display

	CTaskSetting m_Tasks[5];// for tasks settings

	BOOL         m_ShowPreZone_WF_VFAWnd; // for showing presbia zone in wf VFA window
	BOOL         m_ShowPreZone_WF_CMPWnd; // for showing presbia zone in wf Compare window
	BOOL         m_ShowPreZone_WF_VA_Wnd[4]; // for showing presbia zone in wf Visual Acuity window
	BOOL         m_ShowPreZone_WF_AKA_Wnd[4]; // for showing presbia zone in wf Angle A/K window

	BOOL         m_ShowTask;// for task settings

	real         m_IOLSel_IHOA;// IOL SELECTION  Internal hoa threshold

	uchar        m_Reserved[10000];

	BOOL         LoadFromFile(const CString& FileName);
	void         SaveIntoFile(const CString& FileName);

	CString GetTaskBtnName(int i, int j);
	int     GetTaskBtnType(int i, int j) { return m_Tasks[i].m_SubButton[j]; };
};

//***************************************************************************************

class C601NewSettings
{
public:

	C601NewSettings();

	int          m_IncLoC_OD;
	int          m_IncLoC_OS;
	real         m_SurInAst_OD;
	real         m_SurInAst_OS;
	BOOL         m_Adjust_CT;
	BOOL         m_Adjusting_CT;
	real         WFOD_COL_Width[11];//the width of WFODLIST columns
	real         WFOS_COL_Width[11];//the width of WFOSLIST columns
	real         CTOD_COL_Width[11];//the width of CTODLIST columns
	real         CTOS_COL_Width[11];//the width of CTOSLIST columns

	int          m_OSHER_ALIG_CUS[4];//for the CT OSHER alignment custom

	CWndSettings m_OWFCTSettings[4];// CWFCTOUSumWnd

	int          m_ScreenShotFormat;// 0: jpg(default) ; 1: png;  2: bmp; 3: dcm;

	CWndSettings m_WFSoloSettings[2][4];    //SOLO Aberration and Refraction analysis settings

	uchar        m_DefaultWFSolo;

	CWndSettings m_IOLSelSettings[6];// IOL SELECTION wnd

	real         m_IOLSel_Alpha[2];// IOL SELECTION threshold
	real         m_IOLSel_CHOA;// IOL SELECTION Corneal hoa threshold
	real         m_IOLSel_Astig;// IOL SELECTION threshold
	real         m_IOLSel_Sph_Abbe[4];// IOL SELECTION threshold

	CWndSettings m_InAstigSettings[4]; // Astigmatism Source Display

	CTaskSetting m_Tasks[5];// for tasks settings

	BOOL         m_ShowPreZone_WF_VFAWnd; // for showing presbia zone in wf VFA window
	BOOL         m_ShowPreZone_WF_CMPWnd; // for showing presbia zone in wf Compare window
	BOOL         m_ShowPreZone_WF_VA_Wnd[4]; // for showing presbia zone in wf Visual Acuity window
	BOOL         m_ShowPreZone_WF_AKA_Wnd[4]; // for showing presbia zone in wf Angle A/K window

	BOOL         m_ShowTask;// for task settings

	real         m_IOLSel_IHOA;// IOL SELECTION  Internal hoa threshold

	BOOL         m_Super_Exam;// 4 BYTES 

	CWndSettings m_DysfuncSettings[4]; // Dysfunctional Lens Analysis

	BOOL         m_WFSumShow[8];// 4 * 8 = 32 bytes , used for showing or not of WF buttons
	BOOL         m_CTSumShow[8];// 4 * 8 = 32 bytes , used for showing or not of CT buttons
	BOOL         m_WFCTSumShow[9];// 4 * 9 = 36 bytes , used for showing or not of WF/CT buttons

	CWndSettings m_DysPtSettings[5];// sizeof(CWndSettings) = 1304 * 5 = 6520 bytes

	CWndSettings m_TSsm5Settings[4];   // CWFSingleSumWnd  for Depth of focus Display

	uchar         m_Reserved[200000];

	BOOL         LoadFromFile(const CString& FileName);
	void         SaveIntoFile(const CString& FileName);


	CString GetTaskBtnName(int i, int j);
	int     GetTaskBtnType(int i, int j) { return m_Tasks[i].m_SubButton[j]; };
};

//***************************************************************************************

//new Settings for some new adding attributes later than version 600
class CNewSettings
{
public:

	CNewSettings();

	int          m_IncLoC_OD;
	int          m_IncLoC_OS;
	real         m_SurInAst_OD;
	real         m_SurInAst_OS;
	BOOL         m_Adjust_CT;
	BOOL         m_Adjusting_CT;
	real         WFOD_COL_Width[11];//the width of WFODLIST columns
	real         WFOS_COL_Width[11];//the width of WFOSLIST columns
	real         CTOD_COL_Width[11];//the width of CTODLIST columns
	real         CTOS_COL_Width[11];//the width of CTOSLIST columns

	int          m_OSHER_ALIG_CUS[4];//for the CT OSHER alignment custom

	CWndSettings m_OWFCTSettings[4];// CWFCTOUSumWnd

	int          m_ScreenShotFormat;// 0: jpg(default) ; 1: png;  2: bmp; 3: dcm;

	CWndSettings m_WFSoloSettings[2][4];    // SOLO Aberration and Refraction analysis settings

	uchar        m_DefaultWFSolo;

	CWndSettings m_IOLSelSettings[6];// IOL SELECTION wnd

	real         m_IOLSel_Alpha[2];// IOL SELECTION threshold
	real         m_IOLSel_CHOA;// IOL SELECTION Corneal hoa threshold
	real         m_IOLSel_Astig;// IOL SELECTION threshold
	real         m_IOLSel_Sph_Abbe[4];// IOL SELECTION threshold

	CWndSettings m_InAstigSettings[4]; // Astigmatism Source Display

	CTaskSetting m_Tasks[5];// for tasks settings

	BOOL         m_ShowPreZone_WF_VFAWnd; // for showing presbia zone in wf VFA window
	BOOL         m_ShowPreZone_WF_CMPWnd; // for showing presbia zone in wf Compare window
	BOOL         m_ShowPreZone_WF_VA_Wnd[4]; // for showing presbia zone in wf Visual Acuity window
	BOOL         m_ShowPreZone_WF_AKA_Wnd[4]; // for showing presbia zone in wf Angle A/K window

	BOOL         m_ShowTask;// for task settings

	real         m_IOLSel_IHOA;// IOL SELECTION  Internal hoa threshold

	BOOL         m_Super_Exam;// 4 BYTES 

	CWndSettings m_DysfuncSettings[4]; // Dysfunctional Lens Analysis

	BOOL         m_WFSumShow[8];// 4 * 8 = 32 bytes , used for showing or not of WF buttons
	BOOL         m_CTSumShow[8];// 4 * 8 = 32 bytes , used for showing or not of CT buttons
	BOOL         m_WFCTSumShow[9];// 4 * 9 = 36 bytes , used for showing or not of WF/CT buttons

	CWndSettings m_DysPtSettings[5];// sizeof(CWndSettings) = 1304 * 5 = 6520 bytes

	CWndSettings m_TSsm5Settings[4];   // CWFSingleSumWnd  for Depth of focus Display

	char         m_NewButtonNames[3][5][20];// size =    for new functionality button "Depth of focus comparison" and other possible names in the funture

	BOOL         m_WFNewSumShow[8];//  for new WF functionality button "Depth of focus comparison" and other possible names in the funture

	CWndSettings m_TSsm6Settings[4];   // CWFSingleSumWnd  for Depth of focus Comparison Display

	uchar        m_NEW_EXCEL_WF_SETS[30];
	uchar        m_NEW_EXCEL_WFCT_SETS[30];

	BOOL         m_WFCTNewSumShow[8];//  for transformed functionality button "Angle k/a" , Toric Alignment and other possible WF/CT names in the funture

	char         m_NewChButtonNames[3][5][200];//6.2.0 Chinese 3*5*200 = 3000

	uchar        m_Reserved[200000];

	BOOL         LoadFromFile(const CString& FileName);
	void         SaveIntoFile(const CString& FileName);

	void		 SaveTaskName(int i, CString& Name);//6.2.0 Chinese
	CString	     ReadTaskName(int i);//6.2.0 Chinese


	CString      GetTaskBtnName(int i, int j);
	int          GetTaskBtnType(int i, int j) { return m_Tasks[i].m_SubButton[j]; };

	void         GetNewWFBtnName(int i, int j, CString& Name);
	void         SetNewWFBtnName(int i, int j, const CString& Name);
};

//**************************************************************

//IOL Data for program iTraceTPS
class CIOLData
{
public:

	GUID            PatientID;
	char            GroupName[51];
	char            CustomID[51];
	char            LastName[51];
	char            FirstName[51];
	char            MiddleName[51];
	int             BirthYear;
	int             BirthMonth;
	int             BirthDay;
	int             Sex;
	TDB_ADDRESS     Address;
	char            HomePhone[21];
	char            WorkPhone[21];
	char            EMail[51];
	char            Note[101];

	int             RegYear;
	int             RegMonth;
	int             RegDay;
	int             RegHour;
	int             RegMinute;

	char            FileName[301];

	int             ExportYear;
	int             ExportMonth;
	int             ExportDay;
	int             ExportHour;
	int             ExportMinute;

	CCTExam         pCTExam;

	BOOL LoadFromFile(const CString& FileName);
	void SaveIntoFile(const CString& FileName);
};

//*********************************************************************************
// saving and loding average exam m_c infomation
class CAveExamInfo
{
public:
	real  m_c[45];

	uchar m_resevered[490];  //The size must be greater than ? bytes for zip file works

	BOOL LoadFromFile(const CString& FileName);
	void SaveIntoFile(const CString& FileName);
};
//saving and loding average exam m_c infomation Done

//*************************************************************************************

class CBugTest
{
public:
	CString BugCode;
};

//***************************************************************************************

CString DateToStr(const int Year, const int Month, const int Day);

extern CTempSettings TempSettings;

extern CNewSettings  NewSettings;

extern C531NewSettings  OldNewSettings;

extern C601NewSettings  Old601NewSettings;

extern CSettings Settings;

//***************************************************************************************