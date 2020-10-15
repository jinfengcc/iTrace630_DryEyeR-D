//***************************************************************************************
//libcmtd.lib; uafxcwd.lib
#pragma once

//***************************************************************************************

//6.2.0
#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif
//6.2.0
//***************************************************************************************

#define VC_EXTRALEAN
// #define _CRT_SECURE_NO_WARNINGS
// #define WINVER           0x0501
// #define _WIN32_WINNT     0x0501
// #define _WIN32_WINDOWS   0x0410
// #define _WIN32_IE        0x0600

//***************************************************************************************

#include <AfxWin.h>    // MFC core and standard components
#include <WinIoCtl.h>
#include <AfxExt.h>    // MFC extensions
#include <AfxDtCtl.h>  // MFC support for Internet Explorer 4 Common Controls
#include <AfxCmn.h>    // MFC support for Windows Common Controls
#include <Math.h>
#include <Float.h>
#include <Gl\Gl.h>     // OpenGL
//#include <Gl\GlAux.h>  // OpenGL
#include <Dbt.h>       // device arrival/removal
#include <Shlwapi.h>   // PathFileExists
#include <htmlhelp.h>  // 530 for help system

#include <algorithm>
namespace Gdiplus {
  using std::min;
  using std::max;
}

#include <Gdiplus.h>   // GDI+

//***************************************************************************************

typedef unsigned char    uchar;
typedef unsigned short   ushort;
typedef unsigned long    ulong;
typedef unsigned int     uint;
typedef __int64          int64;
typedef unsigned __int64 uint64;
typedef double           real_t;

//***************************************************************************************

#include "Structures.h"
#include "Utils.h"
#include "Messages.h"

//***************************************************************************************

#define SAFE_DELETE(p) if(p){delete p; p = NULL;}
#define SAFE_DELETE_ARRAY(p) if(p){delete [] p; p = NULL;}

#define LINE_SIZE(w) ( ((w) + (w) + (w)) & 3 ? (((w) + (w) + (w)) | 3) + 1 : (w) + (w) + (w) )
#define IMAGE_SIZE(w, h) ( (h) * LINE_SIZE(w) )

#define _E1      2.71828182845904523536028747135 //change from _E to _E1. 0528 for vs2013
#define _Pi      3.14159265358979323846264338328
#define _2_Pi    6.28318530717958647692528676656
#define _Pi_2    1.57079632679489661923132169164
#define _Pi_180  0.01745329251994329576923690768
#define _180_Pi 57.29577951308232087679815481411

#define INVALID_VALUE 1000000000

#define NOCOLOR         0xffffffff
#define RED             0x000000ff
#define GREEN           0x0000ff00
#define DARK_GREEN      0x0000CC00//
#define BLUE            0x00ff0000
#define LIGHT_BLUE      0x00888800
#define YELLOW          0x0000ffff
#define MAGENTA         0x00ff00ff
#define CYAN            0x00ffff00
#define ORANGE          0x00008fff
#define BROWN           0x00004fa0
#define WHITE           0x00ffffff
#define BLACK           0x00000000
#define GRAY            0x00808080
#define LIGHT_GRAY      0x00c0c0c0
#define DARK_GRAY       0x00404040
#define MEDIUM_GRAY     0x00a0a0a0//530

#define WM_THREAD_UPDATE       0x8101
#define WM_THREAD_FINISH       0x8102
#define WM_NOTIFY_LBUTTON_DOWN 0x8103
#define WM_NOTIFY_RBUTTON_DOWN 0x8104
#define WM_NOTIFY_RBUTTON_UP   0x8105
#define WM_CHANGE_POS          0x8106
#define WM_ROTATE              0x8107
#define WM_CHANGE_CENT         0x8108
#define WM_CHANGE_STEP         0x8109
#define WM_CHANGE_MAX          0x8110
#define WM_SLIDER_POS_CHANGED  0x8111
#define WM_UPDATE_INFO         0x8112
#define WM_DEC_ZONE            0x8113
#define WM_INC_ZONE            0x8114
#define WM_NOTIFY_LBTN_DBCLK   0x8115
#define WM_NOTIFY_OSHER_ALG    0x8201
#define WM_NOTIFY_OSHER_ALG_BACK 0x8202
#define WM_SOLO_MASK           0x8203
#define WM_SOLO_HYPELINK       0x8204
#define WM_SLIDER_LCLICK       0x8205
#define WM_OKULIX_LCLICK       0x8206
#define WM_SHOW_SIMPRMS        0x8207// Show Simplified Rms GUI
#define WM_ONOFF_POS_CHANGED   0x8208// Move OnOff Btn
#define WM_SELBTN_POS_CHANGED   0x8209// Move OnOff Btn

//T: Entire eye   C:Corneal   I:internal
#define TYPE_TEYE  1//CWFSingleSumWnd(Angle K/A) default
#define TYPE_TEYC  2
#define TYPE_TSGN  3
#define TYPE_TRSD  4
#define TYPE_TOPM  5
#define TYPE_TWFM 10 //Wafvfront map
#define TYPE_TRFM 11 //Refraction Map
#define TYPE_TPSF 12
#define TYPE_TEEF 13
#define TYPE_TLTR 14 //Visual Acuity Chart
#define TYPE_TMTF 15
#define TYPE_TRMS 16
#define TYPE_TAZR 17
#define TYPE_TANG 18

//#define TYPE_NLTR 19 // Near Snellen E

#define TYPE_CEYE 30
#define TYPE_CAXM 31 //CT: Axial map
#define TYPE_CTNM 32 // local Roc Map
#define TYPE_CRFM 33 // refrective map
#define TYPE_CELM 34 //ct z elevation
#define TYPE_CWFM 35
#define TYPE_CRMS 36
#define TYPE_CPSF 37 //axial map
#define TYPE_CEEF 38
#define TYPE_CMTF 39
#define TYPE_CLTR 50
#define TYPE_CSKM 51
#define TYPE_CPLN 52

#define TYPE_IWFM 40
#define TYPE_IRMS 41
#define TYPE_IPSF 42
#define TYPE_IEEF 43
#define TYPE_IMTF 44
#define TYPE_ILTR 45
#define TYPE_IRFM 46

#define TYPE_DOFF 61 // Deepth of Focus E
#define TYPE_DOFC 62 // Deepth of Focus Comparison

//530 for task buttons
#define TYPE_NONA   0  //No GUI

#define TYPE_DEFI1  20  //User defined 1
#define TYPE_DEFI2  21  //User defined 2
#define TYPE_DEFI3  22  //User defined 3
#define TYPE_DEFI4  23  //User defined 4
#define TYPE_DEFI5  24  //User defined 5
#define TYPE_DEFI6  25  //User defined 6
#define TYPE_DEFI7  26  //User defined 7
#define TYPE_DEFI8  27  //User defined 8

#define TYPE_WCOU   2  //WF/CT OU Overview  A
#define TYPE_REFA   3  //Refraction Analysis A
#define TYPE_ABEA   4  //Aberration Analysis A
#define TYPE_WCVA   5  //WF/CT Visual Acuity  A
#define TYPE_CTSU   6  //CT Summary A
#define TYPE_VFA    7  //VFA  A

#define TYPE_KERA   8  //Keratometry  //A
#define TYPE_CHAN   9  //Chang Analysis  A

#define TYPE_IOLS  10  //IOL Selection Analysis A
#define TYPE_ANGK  11  //Angle K/A A
#define TYPE_TORI  12  //Toric Planner
#define TYPE_ACCO  13  //Accomm and DOF Analysis

#define TYPE_WFVA  14  //WF Visual Acuity  //A
#define TYPE_WFRM  15  //WF/RMS   y
#define TYPE_CTDS  16  //CT OD/OS
#define TYPE_CT3D  17  //CT 3D Z-Elevation  y
#define TYPE_WCSU  18  //WF/CT Summary      y
#define TYPE_CTCM  19  //CT Compare      n

#define TYPE_DYSF  28   //WF/CT Dysfunctional lens analysis 531
#define TYPE_ASTI  29   //WF/CT Astigmastim source
#define TYPE_DYSP  30   //WF/CT Dysfunctional lens analysis 531

#define TYPE_DEPF  31   //WF Depth of focus 531

#define TYPE_TOAL  32   //WF/CT Toric alignment

#define TYPE_NEVI  33  //6.2.0 WF Near Vision

#define DIOPTERS    0
#define MILLIMETERS 1

//6.2.0 For laser safty detection
#define NOHWCON     0 //No hardware conncetion
#define LASERFAIL   1 // Laser safty detection failed
#define LASERSAFE   2 // Laser safty detection sucessful
//6.2.0 For laser safty detection
//***************************************************************************************

// For merge dicom
typedef enum
{
	SAMP_SUCCESS = 1,
	SAMP_FAILURE = 0
} SAMP_STATUS;

//***************************************************************************************

// For merge dicom
typedef enum
{
	SAMP_TRUE = 1,
	SAMP_FALSE = 0
} SAMP_BOOLEAN;

//***************************************************************************************

extern const uint SOFTWARE_VERSION_3;
extern const uint SOFTWARE_VERSION_4;
extern const uint SOFTWARE_VERSION;

extern real_t COS[360];
extern real_t SIN[360];

extern int  MutiCOS[800][360];
extern int  MutiSIN[800][360];

extern CString MAIN_DIR;

extern CString IniPath;// ini registration
extern CString IniSection;// ini registration

extern CString WndClassName;

extern int   OD_AverageExam;// the number of OD exams for this averaged Exam(multiple selection)
extern GUID  OD_SelectExamsID[1024];// if it is averaged exam, maintain the GUID of OD exams in here; 1024 is NUM_EXAMS_MAX
extern int   OD_AverageExam2;//For compare display
extern GUID  OD_SelectExamsID2[1024];//For compare display

extern int   OS_AverageExam;//  the number of OS exams for this averaged Exam(multiple selection)
extern GUID  OS_SelectExamsID[1024];// if it is averaged exam, maintain the GUID of OS exams in here; 1024 is NUM_EXAMS_MAX
extern int   OS_AverageExam2;//For compare display
extern GUID  OS_SelectExamsID2[1024];//For compare display

extern int   PresbiaMode;//Presbia 1:Normal; 2: Presbia64; 3:Presbia256;
extern int   PresbiaModeOrder;//Presbia Order  12 : Normal - Presbia64; 13: Normal - Presbia256 etc.

extern BOOL  Normal64Mode;
extern BOOL  WFScanModeChanged;

extern BOOL  m_isCombo;
extern BOOL  m_isHoya;
extern BOOL  m_isSolo;
extern BOOL  m_isDemo;
extern BOOL  m_isViewer;
extern BOOL  m_isValid;
extern BOOL  m_isPresbia;
extern BOOL  m_isComboViewer;
extern BOOL  m_isComboDicom;//620
extern CString m_LicenseName;

extern CStringA G_As;//6.2.0 For change to unicode

//***************************************************************************************

CString FullSoftwareVersion(const uint SoftwareVersion);

BOOL  InputIsRealNum(CString inputStr);// check the last input letter is real number(0~9 and .) or not

BOOL  InputIsIntNum(CString inputStr);// check the last input letter is int number(0~9) or not

BOOL  InputIsIpADD(CString inputStr);// For new Dicom

BOOL  NotRealNum(CString inputStr);// check the str is real number or not

BOOL  NotIntNum(CString inputStr);// check the str is int number or not

int   GetDegree(real_t x, real_t y);// for DLI

void  CopyFolder(CString OriFolder, CString DestFolder);// For folders copy

BOOL  FindSubString(CString DstStr, CString SearchStr, int &pos);//For finding a DstStr form SearchStr

//***************************************************************************************
