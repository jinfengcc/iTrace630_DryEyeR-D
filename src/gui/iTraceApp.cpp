//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "SplashScreen.h"
#include "MainWnd.h"
#include "Hardware.h"
#include "Registry.h"
#include "Settings.h"
#include "Licensing.h"
#include "GlobalFunctions.h"
#include "TraceyDicom.h" //6.2.0 For Dicom Toolkit

#include "INIParser.h" //6.2.0 ini registration
#include "libs/CommonLib/GdiplusInitializer.h"
#include "DbgMiniDump.h"
#include "iTraceApp.h"

#if _MSC_VER >= 1400
#  if defined _M_IX86
#    pragma comment(                                                                                                                                           \
      linker,                                                                                                                                                  \
      "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#  elif defined _M_IA64
#    pragma comment(                                                                                                                                           \
      linker,                                                                                                                                                  \
      "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#  elif defined _M_X64
#    pragma comment(                                                                                                                                           \
      linker,                                                                                                                                                  \
      "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#  else
#    pragma comment(                                                                                                                                           \
      linker,                                                                                                                                                  \
      "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#  endif
#endif

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

//***************************************************************************************
BOOL m_FirstSet;

CLicensing      Licensing;
CSettings       Settings;
CTempSettings   TempSettings;
CNewSettings    NewSettings;
C531NewSettings OldNewSettings;
C601NewSettings Old601NewSettings;
// CDB              DB;
// CHW              HW;
CiTraceApp App;
CBugTest   BugTest;

//***************************************************************************************
BOOL CiTraceApp::InitInstance()
{
  CWinApp::InitInstance();
  GdiplusInitializer gdipi;
  CoInitialize(nullptr);

  DbgMiniDumpInit();
  CSplashScreen::CreateInstance(IDB_SPLASH_SCREEN);

  INITCOMMONCONTROLSEX InitCtrls;
  InitCtrls.dwSize = sizeof(INITCOMMONCONTROLSEX);
  InitCtrls.dwICC  = ICC_LISTVIEW_CLASSES;

  Registration(); // 6.2.0 ini registration

  ControlSettings();

  CHW::CreateInstance();
  BOOL IsHardwareOK = ::HW.IsConnected();

  // Open camera
  if (IsHardwareOK) {
    if (!::HW.ISVideoCameraOn) {
      HW.TurnVideoCameraOn();
      ::HW.ISVideoCameraOn = true;
    }
  }
  else {
    ::HW.ISVideoCameraOn = false;
  }
  // Open camera Done

  // COM library is initialized with COINIT_APARTMENTTHREADED after the following call

  // Database connection
  CDB::CreateInstance();
  ::DB.LocalConnect(::Settings.m_OpenDBAsReadOnly); // 6.2.0  create "dicom" filed in patienset table
  ::DB.Disconnect();
  ::DB.Connect(::Settings.m_OpenDBAsReadOnly);
  // Database connection Done

  CalSinCos(); // Calculate cos and sin values

  // Software license
  ::Licensing.GetHardwareID();
  ::Licensing.LoadSoftwareAK();
  ::Licensing.DetermineLicenseType("");

  m_isCombo =
    (::Licensing.IsCombo() || ::Licensing.IsComboViewer() || ::Licensing.IsComboDICOM() || ::Licensing.IsPresbia() ? TRUE : FALSE);
  m_isHoya        = (::Licensing.IsWorkstation() || ::Licensing.IsViewer() || ::Licensing.IsDemo() ? TRUE : FALSE);
  m_isSolo        = (::Licensing.IsSolo() ? TRUE : FALSE);
  m_isDemo        = (::Licensing.IsDemo() ? TRUE : FALSE);
  m_isViewer      = (::Licensing.IsComboViewer() || ::Licensing.IsViewer() ? TRUE : FALSE);
  m_isComboDicom  = ::Licensing.IsComboDICOM();
  m_isValid       = (::Licensing.IsValid() ? TRUE : FALSE);
  m_isPresbia     = (::Licensing.IsPresbia() ? TRUE : FALSE);
  m_isComboViewer = (::Licensing.IsComboViewer() ? TRUE : FALSE);

  m_LicenseName = ::Licensing.LicenseName();

  if (m_isComboDicom) // If It is combo dicom, ask the activation of RZDCX
  {
    RzdcxActivation();
  }
  // Software license Done

  ControlSettings2();

  if (m_isDemo) {
    ::DB.Disconnect();
    ::DB.Connect(::Settings.m_OpenDBAsReadOnly);
  }

  if (m_FirstSet && m_isCombo) {
    ::NewSettings.m_OSHER_ALIG_CUS[3] = 1;
  }

  ::WndClassName = ::AfxRegisterWndClass(CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW), NULL, NULL);

  Gdiplus::GdiplusStartupInput GdiplusStartupInput;
  Gdiplus::GdiplusStartup(&m_GdiplusToken, &GdiplusStartupInput, NULL);

  ::NewSettings.m_WFSumShow[5] = FALSE; // WFCT Angle k/a (do not allow the wf angle k/a)

  m_pMainWnd = new CMainWnd;

  ::HW.SetNotificationWindow(m_pMainWnd);

  CSplashScreen::Instance()->HideSplash();

  return TRUE;
}

//***************************************************************************************

int CiTraceApp::ExitInstance()
{
  HW.TurnVideoCameraOff();

  Gdiplus::GdiplusShutdown(m_GdiplusToken);

  ::HW.ReleaseNotificationWindow();

  ::DB.Disconnect();

  ::Settings.SaveIntoFile(MAIN_DIR + "\\settings620");

  ::NewSettings.SaveIntoFile(MAIN_DIR + "\\newsettings620");

  CoUninitialize();
  return CWinApp::ExitInstance();
}

//***************************************************************************************

// The contents inside the 620 ini file
//[Tracey 6.2.0]
// Main Folder = C:\Tracey
// Export Excel Folder = C:\Tracey\Export Excel Data
// Database Type = 0
// Database Folder = C:\Data\AAO15 - Copy
// Database Filename =
// S1 = TOSHIBA DT01ACA300 Z4I2JNZGS
// S2 =
// S3 =
// S4 = Intel(R) Core(TM) i7 - 4790 CPU @ 3.60GHz Intel64 Family 6 Model 60 Stepping 3
// Software Activation Key 6.0.0 = X79G33MLTIH9PKKJ5GKX
// OMED =
// Export Folder = C:\Tracey\Export Data
// WaveTouch Folder = C:\Tracey\WaveTouch Lens Orders
// Settings Folder = C:\Tracey
// Import Format = zip
//  Screenshot Folder = C:\Users\iTrace\Desktop\
//Dicom MWL Local AE = RZDCX
// Dicom MWL Target AE = RZDCX
// Dicom MWL IP Address = localhost
// Dicom MWL IP Port = 104
// RZDCX ACT = 0
// Record Folder = C:\Tracey\Rec_Videos  //Only for record video branch

void CiTraceApp::Registration()
{
  CINIParser IniPar;
  BOOL       IniFileExist = TRUE;

  // Get System folders
  CTraceyRegKey Key_Read(HKEY_LOCAL_MACHINE, MAIN_REG_KEY);
  CTraceyRegKey Key_Read1(HKEY_CURRENT_USER, MAIN_REG_KEY);

  if (!Key_Read.GetStrValue("Main Folder", MAIN_DIR)) {
    if (!Key_Read1.GetStrValue("Main Folder", MAIN_DIR)) {
      MAIN_DIR = "C:\\Tracey";
    }
  }

  if (!::PathFileExists(MAIN_DIR)) {
    ::CreateDirectory(MAIN_DIR, NULL);
    ::CreateDirectory(MAIN_DIR + "\\Data", NULL);
  }

  if (!::PathFileExists(MAIN_DIR + "\\Data")) {
    ::CreateDirectory(MAIN_DIR + "\\Data", NULL);
  }
  // Get System folders Done

  IniPath = MAIN_DIR + "\\" + IniPath;
  if (!::PathFileExists(IniPath)) {
    CString MainkeyStr = "Main Folder";
    IniPar.SetStrKey(IniSection, MainkeyStr, MAIN_DIR, IniPath);
    IniFileExist = FALSE;
  }

  // if 611 is First running, Get all other ini info from 610 ini(Traceysys.ini) if it exist,
  // or from win reigstration table
  // the new 611 ini file is Tracey611sys.ini
  if (!IniFileExist) {
    CString ExcelFolderName  = "";
    int     DatabaseType     = DATABASE_TYPE_MSJET;
    CString DatabaseFolder   = "";
    CString DatabaseFilename = "";
    CString InstDate         = "";
    CString S1               = "";
    CString S2               = "";
    CString S3               = "";
    CString S4               = "";
    CString Act              = "";
    CString OMED             = "";
    CString ExportFolder     = "";
    CString WaveTouchFolder  = "";
    CString SettingsFolder   = "";
    CString ImportFormat     = "";
    CString ScreenshotFolder = "";

    CString keyStr[22];
    keyStr[0]  = "Export Excel Folder";
    keyStr[1]  = "Database Type";
    keyStr[2]  = "Database Folder";
    keyStr[3]  = "Database Filename";
    keyStr[4]  = "S1";
    keyStr[5]  = "S2";
    keyStr[6]  = "S3";
    keyStr[7]  = "S4";
    keyStr[8]  = "Software Activation Key 6.0.0";
    keyStr[9]  = "OMED";
    keyStr[10] = "Export Folder";
    keyStr[11] = "WaveTouch Folder";
    keyStr[12] = "Settings Folder";
    keyStr[13] = "Import Format";
    keyStr[14] = "Screenshot Folder";

    keyStr[15] = "Dicom MWL Local AE";
    keyStr[16] = "Dicom MWL Target AE";
    keyStr[17] = "Dicom MWL IP Address";
    keyStr[18] = "Dicom MWL IP Port";
    keyStr[19] = "Dicom Modality";
    keyStr[20] = "RZDCX ACT";
    keyStr[21] = "Inst Data";

    // Check ini 610
    BOOL    Ini610Exist = TRUE;
    CString Ini610      = MAIN_DIR + "\\Traceysys.ini";

    if (!::PathFileExists(Ini610)) {
      Ini610Exist = FALSE;
    }
    // Check ini 610 Done

    if (Ini610Exist) {
      CString IniSec = "Tracey ";

      // Read from 610 ini
      ExcelFolderName = IniPar.GetStrKey(IniSec, keyStr[0], Ini610);

      // 6.2.0 New Bug
      if (ExcelFolderName == "") {
        IniSec = "Tracey 6.1.0";

        // Read from 610 ini
        ExcelFolderName = IniPar.GetStrKey(IniSec, keyStr[0], Ini610); // 6.2.0 New Bug

        CString Type = IniPar.GetStrKey(IniSec, keyStr[1], Ini610);

        // Tracey 6.1.0

        G_As         = Type;
        DatabaseType = atoi(G_As);

        DatabaseFolder = IniPar.GetStrKey(IniSec, keyStr[2], Ini610);

        S1  = IniPar.GetStrKey(IniSec, keyStr[4], Ini610);
        S2  = IniPar.GetStrKey(IniSec, keyStr[5], Ini610);
        S3  = IniPar.GetStrKey(IniSec, keyStr[6], Ini610);
        S4  = IniPar.GetStrKey(IniSec, keyStr[7], Ini610);
        Act = IniPar.GetStrKey(IniSec, keyStr[8], Ini610);
        // Read from 610 ini done

        // Read DatabaseFilename,OMED, Export Folder, WaveTouch Folder,
        // Settings folder, Screenshot Folder and Import Format from registration table,
        // because they are not in 610 ini
        Key_Read.GetStrValue(keyStr[3], DatabaseFilename);
        Key_Read.GetStrValue(keyStr[9], OMED);
        Key_Read.GetStrValue(keyStr[10], ExportFolder);
        Key_Read.GetStrValue(keyStr[11], WaveTouchFolder);
        Key_Read.GetStrValue(keyStr[12], SettingsFolder);
        Key_Read.GetStrValue(keyStr[13], ImportFormat);
        Key_Read.GetStrValue(keyStr[14], ScreenshotFolder);
        // Done
      }
    }
    else {
      // 6.2.0 New Bug
      Ini610 = MAIN_DIR + "\\Tracey610Sys.ini";

      if (::PathFileExists(Ini610)) {
        CString IniSec = "Tracey ";

        // Read from 610 ini
        ExcelFolderName = IniPar.GetStrKey(IniSec, keyStr[0], Ini610);

        // 6.2.0 New Bug
        if (ExcelFolderName == "") {
          IniSec = "Tracey 6.1.0";

          // Read from 610 ini
          ExcelFolderName = IniPar.GetStrKey(IniSec, keyStr[0], Ini610); // 6.2.0 New Bug

          CString Type = IniPar.GetStrKey(IniSec, keyStr[1], Ini610);

          // Tracey 6.1.0

          G_As         = Type;
          DatabaseType = atoi(G_As);

          DatabaseFolder = IniPar.GetStrKey(IniSec, keyStr[2], Ini610);

          S1  = IniPar.GetStrKey(IniSec, keyStr[4], Ini610);
          S2  = IniPar.GetStrKey(IniSec, keyStr[5], Ini610);
          S3  = IniPar.GetStrKey(IniSec, keyStr[6], Ini610);
          S4  = IniPar.GetStrKey(IniSec, keyStr[7], Ini610);
          Act = IniPar.GetStrKey(IniSec, keyStr[8], Ini610);
          // Read from 610 ini done

          // Read DatabaseFilename,OMED, Export Folder, WaveTouch Folder,
          // Settings folder, Screenshot Folder and Import Format from registration table,
          // because they are not in 610 ini
          Key_Read.GetStrValue(keyStr[3], DatabaseFilename);
          Key_Read.GetStrValue(keyStr[9], OMED);
          Key_Read.GetStrValue(keyStr[10], ExportFolder);
          Key_Read.GetStrValue(keyStr[11], WaveTouchFolder);
          Key_Read.GetStrValue(keyStr[12], SettingsFolder);
          Key_Read.GetStrValue(keyStr[13], ImportFormat);
          Key_Read.GetStrValue(keyStr[14], ScreenshotFolder);
          // Done
        }
        // 6.2.0 New Bug
      }
      else // 6.2.0 New Bug
      {
        // Done
        CString ExcelFolderName2  = "";
        CString DatabaseFolder2   = "";
        CString DatabaseFilename2 = "";
        CString Act2              = "";
        CString OMED2             = "";
        CString ExportFolder2     = "";
        CString WaveTouchFolder2  = "";
        CString SettingsFolder2   = "";
        CString ScreenshotFolder2 = "";

        Key_Read.GetStrValue(keyStr[0], ExcelFolderName);
        Key_Read.GetIntValue(keyStr[1], DatabaseType);
        Key_Read.GetStrValue(keyStr[2], DatabaseFolder);
        Key_Read.GetStrValue(keyStr[3], DatabaseFilename);
        Key_Read.GetStrValue(keyStr[4], S1);
        Key_Read.GetStrValue(keyStr[5], S2);
        Key_Read.GetStrValue(keyStr[6], S3);
        Key_Read.GetStrValue(keyStr[7], S4);
        Key_Read.GetStrValue(keyStr[8], Act);
        Key_Read.GetStrValue(keyStr[9], OMED);
        Key_Read.GetStrValue(keyStr[10], ExportFolder);
        Key_Read.GetStrValue(keyStr[11], WaveTouchFolder);
        Key_Read.GetStrValue(keyStr[12], SettingsFolder);
        Key_Read.GetStrValue(keyStr[13], ImportFormat);
        Key_Read.GetStrValue(keyStr[14], ScreenshotFolder);

        Key_Read1.GetStrValue(keyStr[0], ExcelFolderName2);
        Key_Read1.GetStrValue(keyStr[2], DatabaseFolder2);
        Key_Read1.GetStrValue(keyStr[3], DatabaseFilename2);
        Key_Read1.GetStrValue(keyStr[8], Act2);
        Key_Read1.GetStrValue(keyStr[9], OMED2);
        Key_Read1.GetStrValue(keyStr[10], ExportFolder2);
        Key_Read1.GetStrValue(keyStr[11], WaveTouchFolder2);
        Key_Read1.GetStrValue(keyStr[12], SettingsFolder2);
        Key_Read1.GetStrValue(keyStr[14], ScreenshotFolder2);

        if (ExcelFolderName2 != "" && ExcelFolderName != ExcelFolderName2) {
          ExcelFolderName = ExcelFolderName2;
        }

        if (DatabaseFolder2 != "" && DatabaseFolder != DatabaseFolder2) {
          DatabaseFolder = DatabaseFolder2;
        }

        if (DatabaseFilename2 != "" && DatabaseFilename != DatabaseFilename2) {
          DatabaseFilename = DatabaseFilename2;
        }

        if (Act2 != "" && Act != Act2) {
          Act = Act2;
        }

        if (OMED2 != "" && OMED != OMED2) {
          OMED = OMED2;
        }

        if (ExportFolder2 != "" && ExportFolder != ExportFolder2) {
          ExportFolder = ExportFolder2;
        }

        if (WaveTouchFolder2 != "" && WaveTouchFolder != WaveTouchFolder2) {
          WaveTouchFolder = WaveTouchFolder2;
        }

        if (SettingsFolder2 != "" && SettingsFolder != SettingsFolder2) {
          SettingsFolder = SettingsFolder2;
        }

        if (ScreenshotFolder2 != "" && ScreenshotFolder != ScreenshotFolder2) {
          ScreenshotFolder = ScreenshotFolder2;
        }
      }
    }

    // Check registration values
    if (!::PathFileExists(ExcelFolderName)) {
      ExcelFolderName = MAIN_DIR + "\\Export Excel Data";
      if (!::PathFileExists(ExcelFolderName))
        ::CreateDirectory(ExcelFolderName, NULL);
    }

    if (!::PathFileExists(DatabaseFolder)) {
      DatabaseFolder = MAIN_DIR + "\\Data\\MSJet";
      if (!::PathFileExists(DatabaseFolder))
        ::CreateDirectory(DatabaseFolder, NULL);
    }

    if (!::PathFileExists(ExportFolder)) // export dicom/jpg/bmp/png screenshot
    {
      ExportFolder = MAIN_DIR + "\\Export Data";
      if (!::PathFileExists(ExportFolder))
        ::CreateDirectory(ExportFolder, NULL);
    }

    if (!::PathFileExists(WaveTouchFolder)) {
      WaveTouchFolder = MAIN_DIR + "\\WaveTouch Lens Orders";
      if (!::PathFileExists(WaveTouchFolder))
        ::CreateDirectory(WaveTouchFolder, NULL);
    }

    if (!::PathFileExists(SettingsFolder)) {
      SettingsFolder = MAIN_DIR;
    }

    if (ImportFormat == "") {
      ImportFormat = "zip";
    }

    if (!::PathFileExists(ScreenshotFolder)) {
      ScreenshotFolder = MAIN_DIR;
    }

    if (ImportFormat == "") {
      ImportFormat = "zip";
    }

    if (InstDate == "") {
      SYSTEMTIME CurTime;
      ::GetLocalTime(&CurTime);
      CString Def_FileName;
      InstDate.Format(_T("%02i%02i%04i"), CurTime.wMonth, CurTime.wDay, CurTime.wYear);
    }
    // Check registration values Done

    // Write registration values to 611 ini
    IniPar.SetStrKey(IniSection, keyStr[0], ExcelFolderName, IniPath);
    IniPar.SetIntKey(IniSection, keyStr[1], DatabaseType, IniPath);
    IniPar.SetStrKey(IniSection, keyStr[2], DatabaseFolder, IniPath);
    IniPar.SetStrKey(IniSection, keyStr[3], DatabaseFilename, IniPath);
    IniPar.SetStrKey(IniSection, keyStr[4], S1, IniPath);
    IniPar.SetStrKey(IniSection, keyStr[5], S2, IniPath);
    IniPar.SetStrKey(IniSection, keyStr[6], S3, IniPath);
    IniPar.SetStrKey(IniSection, keyStr[7], S4, IniPath);
    IniPar.SetStrKey(IniSection, keyStr[8], Act, IniPath);
    IniPar.SetStrKey(IniSection, keyStr[9], OMED, IniPath);
    IniPar.SetStrKey(IniSection, keyStr[10], ExportFolder, IniPath);
    IniPar.SetStrKey(IniSection, keyStr[11], WaveTouchFolder, IniPath);
    IniPar.SetStrKey(IniSection, keyStr[12], SettingsFolder, IniPath);
    IniPar.SetStrKey(IniSection, keyStr[13], ImportFormat, IniPath);
    IniPar.SetStrKey(IniSection, keyStr[14], ScreenshotFolder, IniPath);

    IniPar.SetStrKey(IniSection, keyStr[15], "", IniPath);
    IniPar.SetStrKey(IniSection, keyStr[16], "", IniPath);
    IniPar.SetStrKey(IniSection, keyStr[17], "", IniPath);
    IniPar.SetStrKey(IniSection, keyStr[18], "", IniPath);

    IniPar.SetStrKey(IniSection, keyStr[20], "0", IniPath);

    IniPar.SetStrKey(IniSection, keyStr[19], "OPM � Ophthalmic Mapping", IniPath);

    IniPar.SetStrKey(IniSection, keyStr[21], InstDate, IniPath);
    // Write registration values to 611 ini Done
  }
  else {
    CString RzdcxAct = IniPar.GetStrKey(IniSection, "RZDCX ACT", IniPath); // 6.2.0 ini registration

    if (RzdcxAct == "") {
      IniPar.SetStrKey(IniSection, "RZDCX ACT", "0", IniPath);
    }

    CString InstDate = IniPar.GetStrKey(IniSection, "Inst Data", IniPath);
    if (InstDate == "") {
      SYSTEMTIME CurTime;
      ::GetLocalTime(&CurTime);
      CString Def_FileName;
      InstDate.Format(_T("%02i%02i%04i"), CurTime.wMonth, CurTime.wDay, CurTime.wYear);

      IniPar.SetStrKey(IniSection, "Inst Data", InstDate, IniPath);
    }
  }
  // Done
}

//*********************************************************************************************************

void CiTraceApp::CalSinCos()
{
  for (int a = 0; a < 360; a++) {
    ::COS[a] = cos(a * _Pi_180);
    ::SIN[a] = sin(a * _Pi_180);
  }

  for (int i = 0; i < 800; i++) {
    for (int a = 0; a < 360; a++) {
      ::MutiCOS[i][a] = intRound(i * ::COS[a]);
      ::MutiSIN[i][a] = intRound(i * ::SIN[a]);
    }
  }
}

//*********************************************************************************************************

void CiTraceApp::ControlSettings()
{
  m_FirstSet = FALSE;

  if (!::Settings.LoadFromFile(MAIN_DIR + "\\settings620")) {
    m_FirstSet = TRUE;

    if (!::Settings.LoadFromFile(MAIN_DIR + "\\settings610")) {
      if (!::Settings.LoadFromFile(MAIN_DIR + "\\settings600")) {
        if (!::Settings.LoadFromFile(MAIN_DIR + "\\settings531")) {
          if (!::Settings.LoadFromFile(MAIN_DIR + "\\settings530")) {
            if (!::Settings.LoadFromFile(MAIN_DIR + "\\settings521")) {
              if (!::Settings.LoadFromFile(MAIN_DIR + "\\settings52")) {
                if (!::Settings.LoadFromFile(MAIN_DIR + "\\settings50")) {
                  if (!::Settings.LoadFromFile(MAIN_DIR + "\\settings")) {
                    ::Settings.LoadFromResource();
                  }
                  else
                    addNewSetting(0);
                }
                else
                  addNewSetting(1);
              }
              else
                addNewSetting(2);
            }
            else {
              addNewSetting(2);
            }

            ::Settings.m_ManualScanDiameterUm  = 3000;
            ::Settings.m_AccommTargetLightTime = 60;    // set the light target to be 60 seconds
            ::Settings.m_ShowSimK              = FALSE; // Set the CT Panel(etc. in OU overview) to show the simk(not ref.k)
          }
        }
      }
    }

    for (int i = 0; i < 4; i++) {
      ::Settings.m_IDsmSettings[2][i].m_MapShowEye = TRUE; // 6.2.0
    }
  }

  if (::Settings.m_AutoScanDiameterUm == 0) {
    ::Settings.m_AutoScanDiameterUm = 6500;
  }

  if (::Settings.m_AccommTargetLightTime <= 0) {
    ::Settings.m_AccommTargetLightTime = 60; // set the light target to be 60 seconds
  }

  if (::Settings.m_ShowSimK != TRUE && ::Settings.m_ShowSimK != FALSE) {
    ::Settings.m_ShowSimK = FALSE; // Set the CT Panel(etc. in OU overview) to show the simk(not ref.k)
  }

  CString Def_Name = ""; // At first Set the task[0] name be empty

  if (!::NewSettings.LoadFromFile(MAIN_DIR + "\\newsettings620")) {
    if (!::NewSettings.LoadFromFile(MAIN_DIR + "\\newsettings610")) {
      if (!::PathFileExists(MAIN_DIR + "\\newsettings600")) {
        if (!::PathFileExists(MAIN_DIR + "\\newsettings531")) {
          if (!::PathFileExists(MAIN_DIR + "\\newsettings530")) {
            SetDef_ExList_Width(); // Set the default widths of the exam list

            ::NewSettings.m_IncLoC_OD   = 0;
            ::NewSettings.m_IncLoC_OS   = 0;
            ::NewSettings.m_SurInAst_OD = 0.0;
            ::NewSettings.m_SurInAst_OS = 0.0;

            SetDef_OWFCT();

            SetDef_IOLSEL();

            SetDef_AstigSou(); // 530

            SetDef_Pre_SowhZone(); // 530

            SetDef_SoloSettings();

            G_As = Def_Name;
            strncpy(&::NewSettings.m_Tasks[0].m_ButtonName[0], G_As, 25);
          }
          else {
            OldNewSettings.LoadFromFile(MAIN_DIR + "\\newsettings531");
            G_As = Def_Name;
            strncpy(&::NewSettings.m_Tasks[0].m_ButtonName[0], G_As, 25);

            TransformNewSettings();
          }
        }
        else {
          OldNewSettings.LoadFromFile(MAIN_DIR + "\\newsettings531");
          G_As = Def_Name;
          strncpy(&::NewSettings.m_Tasks[0].m_ButtonName[0], G_As, 25);

          TransformNewSettings();
        }
      }
      else {
        CString Def_Name = "";
        Old601NewSettings.LoadFromFile(MAIN_DIR + "\\newsettings600");
        G_As = Def_Name;
        strncpy(&::NewSettings.m_Tasks[0].m_ButtonName[0], G_As, 25);

        ::NewSettings.m_Super_Exam = ::Old601NewSettings.m_Super_Exam;
        Transform601NewSettings();
      }
    }
  }

  if (::NewSettings.WFOD_COL_Width[0] == 0.0) {
    SetDef_ExList_Width();
  }

  if (::NewSettings.m_OWFCTSettings[0].m_Type <= 0 || ::NewSettings.m_OWFCTSettings[0].m_Type > 52) // First m_OWFCTSettings setting loading
  {
    SetDef_OWFCT();
  }

  if (::NewSettings.m_IOLSelSettings[0].m_Type <= 0 ||
      ::NewSettings.m_IOLSelSettings[0].m_Type > 52) // First m_OWFCTSettings setting loading
  {
    SetDef_IOLSEL();
  }

  if (::NewSettings.m_InAstigSettings[0].m_Type <= 0 ||
      ::NewSettings.m_InAstigSettings[0].m_Type > 52) // First m_OWFCTSettings setting loading
  {
    SetDef_AstigSou();
  }

  if (::NewSettings.m_WFSoloSettings[1][0].m_Type <= 0 || ::NewSettings.m_OWFCTSettings[0].m_Type > 52) {
    SetDef_SoloSettings();
  }

  if (!::PathFileExists(MAIN_DIR + "\\newsettings620") && !::PathFileExists(MAIN_DIR + "\\newsettings610") &&
      !::PathFileExists(MAIN_DIR + "\\newsettings600")) // New
  {
    ::NewSettings.m_Super_Exam = FALSE;

    SetDef_DysFun();
  }

  if (!::PathFileExists(MAIN_DIR + "\\newsettings620")) // New
  {

    SetDef_DysFun();
  }

  if (!::PathFileExists(MAIN_DIR + "\\settings620")) // 6.2.0
  {
    SetNew620GUI();
  }

  if (::NewSettings.m_DysfuncSettings[0].m_Type <= 0 ||
      ::NewSettings.m_DysfuncSettings[0].m_Type > 52) // First m_OWFCTSettings setting loading
  {
    SetDef_DysFun();
  }

  ::NewSettings.m_Adjust_CT = FALSE;

  if (::NewSettings.m_ScreenShotFormat != 0 && ::NewSettings.m_ScreenShotFormat != 1 && ::NewSettings.m_ScreenShotFormat != 2 &&
      ::NewSettings.m_ScreenShotFormat != 3)
    ::NewSettings.m_ScreenShotFormat = 0;

  ::TempSettings.NewPlannerSettings.m_MapShowIncision       = 1;
  ::TempSettings.NewPlannerSettings.m_MapShowPreopCorneal   = 0;
  ::TempSettings.NewPlannerSettings.m_MapShowPreopInternal  = 0;
  ::TempSettings.NewPlannerSettings.m_MapShowPreopEntireEye = 0;
  ::TempSettings.NewPlannerSettings.m_MapPostopCorneal      = 0;
  ::TempSettings.NewPlannerSettings.m_MapPostopInternal     = 0;
  ::TempSettings.NewPlannerSettings.m_MapPostopEntireEye    = 1;
  ::TempSettings.NewPlannerSettings.m_MapShowICLIndSteep    = 0;
  ::TempSettings.NewPlannerSettings.m_MapShowICLIndFlat     = 0;
  ::TempSettings.Temp_ScaleRang                             = FALSE;

  ::TempSettings.CTSumNum = -1;
}

//***************************************************************************************

// Some settings must be set after licesing was determined
void CiTraceApp::ControlSettings2()
{
  // Detect the m_WFSumShow/m_CTSumShow/m_WFCTSumShow have been set or not
  BOOL SetWFShow = FALSE;
  for (int i = 0; i < 8; i++) {
    SetWFShow += ::NewSettings.m_WFSumShow[i];
  }

  if (SetWFShow > 7) {
    ::NewSettings.m_WFSumShow[7] = FALSE;
  }

  if (!SetWFShow) {
    for (int i = 0; i < 8; i++) {
      ::NewSettings.m_WFSumShow[i] = TRUE;
    }

    if (m_isCombo) {
      // Add the "Depth of Focus comparison and remove the WF/RMS" and transformed 'Angle K/A'
      ::NewSettings.m_WFNewSumShow[0] = FALSE; //
      ::NewSettings.m_WFNewSumShow[1] = FALSE; // 6.2.0 'WF Near vision'

      ::NewSettings.m_WFSumShow[1] = FALSE; // 6.2.0
      ::NewSettings.m_WFSumShow[3] = FALSE; //
      ::NewSettings.m_WFSumShow[4] = FALSE; //
      ::NewSettings.m_WFSumShow[5] = FALSE; //
      ::NewSettings.m_WFSumShow[6] = FALSE; //
      ::NewSettings.m_WFSumShow[7] = FALSE;
      ::NewSettings.m_WFSumShow[8] = FALSE; //
    }

    ::Settings.m_DefaultWFSum = 11;
  }

  // CT
  BOOL SetCTShow = FALSE;
  for (int i = 0; i < 8; i++) {
    SetCTShow += ::NewSettings.m_CTSumShow[i];
  }

  if (SetCTShow > 7) {
    ::NewSettings.m_CTSumShow[7] = FALSE;
  }

  if (!SetCTShow) {
    for (int i = 0; i < 8; i++) {
      ::NewSettings.m_CTSumShow[i] = TRUE;
    }

    if (m_isCombo) {
      ::NewSettings.m_CTSumShow[5] = FALSE; //
      ::NewSettings.m_CTSumShow[6] = FALSE; // 6.2.0
      ::NewSettings.m_CTSumShow[7] = FALSE;
    }

    ::Settings.m_DefaultCTSum = 11;
  }

  // WFCT
  BOOL SetWFCTShow = FALSE;
  for (int i = 0; i < 9; i++) {
    SetWFCTShow += ::NewSettings.m_WFCTSumShow[i];
  }

  if (SetWFCTShow > 7) {
    ::NewSettings.m_WFCTSumShow[7] = FALSE;
    ::NewSettings.m_WFCTSumShow[8] = FALSE;
  }

  if (!SetCTShow) {
    for (int i = 0; i < 8; i++) {
      ::NewSettings.m_WFCTSumShow[i] = TRUE;
    }

    if (m_isCombo) {
      ::NewSettings.m_WFCTNewSumShow[0] = TRUE; //	transformed 'Angle K/A'
      ::NewSettings.m_WFCTNewSumShow[1] = TRUE; //	Toric Alignment

      ::NewSettings.m_WFCTSumShow[3] = FALSE; //
      ::NewSettings.m_WFCTSumShow[4] = FALSE; //
      ::NewSettings.m_WFCTSumShow[5] = FALSE; //
      ::NewSettings.m_WFCTSumShow[7] = FALSE;
      ::NewSettings.m_WFCTSumShow[8] = FALSE;
    }

    ::Settings.m_DefaultINSum = 6;
  }

  // Check task settings
  char s[26];
  memset(s, 0, sizeof(s));
  strncpy(s, (const char *)::NewSettings.m_Tasks[0].m_ButtonName, 25);
  CString taskName = s;

  if (taskName == "")
    SetDef_TaskSettings();
  // Check task settings Done
}

//***************************************************************************************

// set the default exam list width
void CiTraceApp::SetDef_ExList_Width()
{
  for (int i = 0; i < 11; i++) {
    switch (i) {
    case 0:
      ::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] =
        ::NewSettings.CTOS_COL_Width[i]                                 = 0.030;
      break;
    case 1:
      ::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] =
        ::NewSettings.CTOS_COL_Width[i]                                 = 0.058;
      break;
    case 2:
      ::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] =
        ::NewSettings.CTOS_COL_Width[i]                                 = 0.046;
      break;
    case 4:
      ::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = 0.05;
      ::NewSettings.CTOD_COL_Width[i] = ::NewSettings.CTOS_COL_Width[i] = 0.002;
      break;
    case 3:
      ::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] =
        ::NewSettings.CTOS_COL_Width[i]                                 = 0.002;
      break;
    case 5:
      ::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] =
        ::NewSettings.CTOS_COL_Width[i]                                 = 0.002;
      break;
    case 6:
      ::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] =
        ::NewSettings.CTOS_COL_Width[i]                                 = 0.002;
      break;
    case 7:
      ::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] =
        ::NewSettings.CTOS_COL_Width[i]                                 = 0.002;
      break;
    case 8:
      ::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] =
        ::NewSettings.CTOS_COL_Width[i]                                 = 0.002;
      break;
    case 9:
      ::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] =
        ::NewSettings.CTOS_COL_Width[i]                                 = 0.002;
      break;
    case 10:
      ::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = 0.25;
      ::NewSettings.CTOD_COL_Width[i] = ::NewSettings.CTOS_COL_Width[i] = 0.3;
      break;
    }
  }
}

//***************************************************************************************

// Set the default ou overview setting
void CiTraceApp::SetDef_OWFCT()
{
  ::NewSettings.m_OWFCTSettings[0] = ::Settings.m_CSsmSettings[0][0];
  ::NewSettings.m_OWFCTSettings[1] = ::Settings.m_TSsmSettings[0][0];

  ::NewSettings.m_OWFCTSettings[2]           = ::Settings.m_TSsmSettings[0][2];
  ::NewSettings.m_OWFCTSettings[2].m_LtrLine = 70;
}

//***************************************************************************************

// Set the default IOL Selection setting
void CiTraceApp::SetDef_IOLSEL()
{
  ::NewSettings.m_IOLSelSettings[0] = ::Settings.m_CSsmSettings[0][0];
  ::NewSettings.m_IOLSelSettings[1] = ::Settings.m_TSsmSettings[0][0];

  ::NewSettings.m_IOLSelSettings[2]           = ::Settings.m_TSsmSettings[0][2];
  ::NewSettings.m_IOLSelSettings[2].m_LtrLine = 70;

  for (int i = 3; i < 6; i++) {
    ::NewSettings.m_IOLSelSettings[i]            = ::Settings.m_IDsmSettings[0][i - 3];
    ::NewSettings.m_IOLSelSettings[i].m_LtrLine  = 70;
    ::NewSettings.m_IOLSelSettings[i].m_Method3D = FALSE;
  }

  // Thresholds
  ::NewSettings.m_IOLSel_Alpha[0] = 0.65;
  ::NewSettings.m_IOLSel_Alpha[1] = 0.5;

  ::NewSettings.m_IOLSel_CHOA = 0.2;
  ::NewSettings.m_IOLSel_IHOA = 0.2;

  ::NewSettings.m_IOLSel_Astig = 1.0;

  ::NewSettings.m_IOLSel_Sph_Abbe[0] = 0.14;
  ::NewSettings.m_IOLSel_Sph_Abbe[1] = 0.20;
  ::NewSettings.m_IOLSel_Sph_Abbe[2] = 0.25;
  ::NewSettings.m_IOLSel_Sph_Abbe[3] = 0.30;
}

//***************************************************************************************

// Set the default IOL Selection setting
void CiTraceApp::SetDef_AstigSou()
{
  // Astiagmatism source 530
  ::NewSettings.m_InAstigSettings[0] = ::Settings.m_IDsmSettings[2][0];
  ::NewSettings.m_InAstigSettings[1] = ::Settings.m_IDsmSettings[2][1];
  ::NewSettings.m_InAstigSettings[2] = ::Settings.m_IDsmSettings[2][2];
  ::NewSettings.m_InAstigSettings[3] = ::Settings.m_IDsmSettings[2][3];
  for (int i = 0; i < 4; i++) {
    ::NewSettings.m_InAstigSettings[i].m_SizeSmall     = TRUE;
    ::NewSettings.m_InAstigSettings[i].m_SimplifiedRMS = TRUE;

    if (i != 3)
      ::NewSettings.m_InAstigSettings[i].m_Mask.SetType(MASK_ASTIGMATISM);
  }
  // Astiagmatism source 530  done
}

//***************************************************************************************

// Set the default Dysfunctional Lens Analysis
void CiTraceApp::SetDef_DysFun()
{
  ::NewSettings.m_DysfuncSettings[0].m_Type              = TYPE_IMTF;
  ::NewSettings.m_DysfuncSettings[0].m_MTFAverage        = TRUE;
  ::NewSettings.m_DysfuncSettings[0].m_SimplifiedWFPanel = TRUE;
  ::NewSettings.m_DysfuncSettings[0].m_SimplifiedCTPanel = TRUE;
  ::NewSettings.m_DysfuncSettings[1].m_Type              = TYPE_ILTR;
  ::NewSettings.m_DysfuncSettings[1].m_RMSIndividual     = FALSE;
  ::NewSettings.m_DysfuncSettings[1].m_SimplifiedRMS     = TRUE;
  ::NewSettings.m_DysfuncSettings[2].m_Type              = TYPE_TOPM;
  ::NewSettings.m_DysfuncSettings[3].m_Type              = TYPE_CAXM;

  ::NewSettings.m_DysfuncSettings[3] = ::Settings.m_IDsmSettings[0][3];

  for (int i = 0; i < 4; i++) {
    if (i != 3) {
      ::NewSettings.m_DysfuncSettings[i].m_Method3D            = 0;
      ::NewSettings.m_DysfuncSettings[i].m_MapShowSolidSurface = TRUE;
      ::NewSettings.m_DysfuncSettings[i].m_MapShowWireMesh     = TRUE;
      ::NewSettings.m_DysfuncSettings[i].m_SizeSmall           = TRUE;

      if (i == 0 || i == 1) {
        ::NewSettings.m_DysfuncSettings[i].m_Mask.SetType(MASK_HO_TOTAL);
      }
      else {
        ::NewSettings.m_DysfuncSettings[i].m_Mask.SetType(MASK_TOTAL_NO_DEFOCUS);
      }

      ::NewSettings.m_DysfuncSettings[i].m_Piston                 = TRUE;
      ::NewSettings.m_DysfuncSettings[i].m_Tilt                   = TRUE;
      ::NewSettings.m_DysfuncSettings[i].m_MapShowMap             = 1;
      ::NewSettings.m_DysfuncSettings[i].m_ZoneRadiusUm           = 5000;
      ::NewSettings.m_DysfuncSettings[i].m_MapShowEye             = 0;
      ::NewSettings.m_DysfuncSettings[i].m_MapShowNumbers         = 0;
      ::NewSettings.m_DysfuncSettings[i].m_MapShowPupil           = 0;
      ::NewSettings.m_DysfuncSettings[i].m_MapShowLimbus          = 0;
      ::NewSettings.m_DysfuncSettings[i].m_MapShowCornealVertex   = 0;
      ::NewSettings.m_DysfuncSettings[i].m_MapTranslucent         = 0;
      ::NewSettings.m_DysfuncSettings[i].m_MapShowKeratometry     = 0;
      ::NewSettings.m_DysfuncSettings[i].m_MapShowAstigmatismAxes = 0;

      ::NewSettings.m_DysfuncSettings[i].SetDefaultScale();
      CScale *pScale = ::NewSettings.m_DysfuncSettings[i].GetScale();
      pScale->m_Step = 0.5;
      ::NewSettings.m_DysfuncSettings[i].SetDefaultColors(1);
    }

    ::NewSettings.m_DysfuncSettings[i].m_Ltr            = 0;
    ::NewSettings.m_DysfuncSettings[i].m_LtrLine        = 70;
    ::NewSettings.m_DysfuncSettings[i].m_LtrOrientation = 0;
  }

  ::NewSettings.m_DysfuncSettings[3].m_MapShowAstigmatismAxes = 1;
  ::NewSettings.m_DysfuncSettings[3].m_MapShowKeratometry     = 1;
  ::NewSettings.m_DysfuncSettings[3].m_MapShowEye             = 1;
  ::NewSettings.m_DysfuncSettings[3].m_MapShowSimKAxes        = 1;

  // Dysfunctional lens patient
  ::NewSettings.m_DysPtSettings[0]                     = ::NewSettings.m_DysfuncSettings[1];
  ::NewSettings.m_DysPtSettings[0].m_SimplifiedWFPanel = 1;
  ::NewSettings.m_DysPtSettings[0].m_Type              = TYPE_CLTR;

  ::NewSettings.m_DysPtSettings[1] = ::NewSettings.m_DysfuncSettings[1];

  ::NewSettings.m_DysPtSettings[2]        = ::NewSettings.m_DysfuncSettings[1];
  ::NewSettings.m_DysPtSettings[2].m_Type = TYPE_TLTR;

  ::NewSettings.m_DysPtSettings[3] = ::NewSettings.m_DysfuncSettings[3];
  ::NewSettings.m_DysPtSettings[4] = ::NewSettings.m_DysfuncSettings[2];

  ::NewSettings.m_DysPtSettings[4].m_MapShowEye           = 1; // 6.2.0
  ::NewSettings.m_DysPtSettings[4].m_MapShowLimbus        = 1; // 6.2.0
  ::NewSettings.m_DysPtSettings[4].m_MapShowCornealVertex = 1; // 6.2.0

  // Depth focus display
  for (int i = 0; i < 4; i++) {
    ::NewSettings.m_TSsm5Settings[i]             = ::Settings.m_TSsmSettings[0][i];
    ::NewSettings.m_TSsm5Settings[i].m_SizeSmall = FALSE;
    ::NewSettings.m_TSsm5Settings[i].m_Type      = TYPE_DOFF;
  }

  // Depth focus Comparison display
  for (int i = 0; i < 4; i++) {
    ::NewSettings.m_TSsm6Settings[i]             = ::Settings.m_TSsmSettings[0][i];
    ::NewSettings.m_TSsm6Settings[i].m_SizeSmall = FALSE;
    ::NewSettings.m_TSsm6Settings[i].m_Type      = TYPE_DOFC;
  }
}

//***************************************************************************************

// 6.2.0 Set the default Dysfunctional Lens Analysis
void CiTraceApp::SetNew620GUI()
{
  // 6.2.0  from WF&CT Visual acuity Display to MTF
  ::Settings.m_IDsmSettings[2][0].m_Method3D            = FALSE;
  ::Settings.m_IDsmSettings[2][0].m_MapShowSolidSurface = TRUE;
  ::Settings.m_IDsmSettings[2][0].m_MapShowWireMesh     = TRUE;
  ::Settings.m_IDsmSettings[2][0].m_SizeSmall           = TRUE;
  ::Settings.m_IDsmSettings[2][0].m_ZoneRadiusUm        = 5000;
  ::Settings.m_IDsmSettings[2][0].m_Piston              = TRUE;
  ::Settings.m_IDsmSettings[2][0].m_Tilt                = TRUE;

  ::Settings.m_IDsmSettings[2][0].m_Type = TYPE_IMTF;
  ::Settings.m_IDsmSettings[2][1].m_Type = TYPE_CMTF;
  ::Settings.m_IDsmSettings[2][2].m_Type = TYPE_TMTF;

  ::Settings.m_IDsmSettings[2][0].m_Mask.SetType(MASK_HO_TOTAL);
  ::Settings.m_IDsmSettings[2][1].m_Mask.SetType(MASK_HO_TOTAL);
  ::Settings.m_IDsmSettings[2][2].m_Mask.SetType(MASK_HO_TOTAL);

  for (int i = 0; i < 3; i++)
    ::Settings.m_IDsmSettings[2][i].m_Ltr = 0;

  ::Settings.m_IDsmSettings[2][0].m_LtrLine        = 70;
  ::Settings.m_IDsmSettings[2][0].m_LtrOrientation = 0;

  ::Settings.m_IDsmSettings[2][3].m_Type                 = 31; // Axial map       TYPE_CAXM
  ::Settings.m_IDsmSettings[2][3].m_Method3D             = 0;
  ::Settings.m_IDsmSettings[2][3].m_MapShowSolidSurface  = TRUE;
  ::Settings.m_IDsmSettings[2][3].m_MapShowWireMesh      = TRUE;
  ::Settings.m_IDsmSettings[2][3].m_SizeSmall            = TRUE;
  ::Settings.m_IDsmSettings[2][3].m_MapUnit              = DIOPTERS;
  ::Settings.m_IDsmSettings[2][3].m_MapShowMap           = 1;
  ::Settings.m_IDsmSettings[2][3].m_MapShowEye           = 1;
  ::Settings.m_IDsmSettings[2][3].m_MapShowNumbers       = 0;
  ::Settings.m_IDsmSettings[2][3].m_MapShowPupil         = 0;
  ::Settings.m_IDsmSettings[2][3].m_MapShowLimbus        = 0;
  ::Settings.m_IDsmSettings[2][3].m_MapShowCornealVertex = 0;
  ::Settings.m_IDsmSettings[2][3].m_MapTranslucent       = 0;
  ::Settings.m_IDsmSettings[2][3].m_MapShowKeratometry   = 1;

  CScale *pScale5 = ::Settings.m_IDsmSettings[2][3].GetScale();
  pScale5->m_Step = 0.5;
  ::Settings.m_IDsmSettings[2][3].SetDefaultColors(1);
  // 6.2.0  from WF&CT Visual acuity Display to MTF Done

  // Set new CT Summary display
  ::Settings.m_CSsmSettings[0][0].m_Type = 31;        // Axial map       TYPE_CAXM
  ::Settings.m_CSsmSettings[0][1].m_Type = TYPE_TLTR; //
  ::Settings.m_CSsmSettings[0][2].m_Type = TYPE_CRMS; // local Roc  map  TYPE_CTNM
  ::Settings.m_CSsmSettings[0][3].m_Type = TYPE_CEYE;

  ::Settings.m_CSsmSettings[0][1].m_ZoneRadiusUm = 2000;
  ::Settings.m_CSsmSettings[0][2].m_ZoneRadiusUm = 2000;

  ::Settings.m_CSsmSettings[0][0].m_Mask.SetType(MASK_HO_TOTAL);
  ::Settings.m_CSsmSettings[0][1].m_Mask.SetType(MASK_HO_TOTAL);

  ::Settings.m_CSsmSettings[0][0].m_MapShowKeratometry = TRUE;

  ::Settings.m_CSsmSettings[0][0].m_MapUnit = DIOPTERS;
  ::Settings.m_CSsmSettings[0][2].m_MapUnit = MILLIMETERS;

  ::Settings.m_CSsmSettings[0][1].m_LtrLine = 100;

  ::Settings.m_CSsmSettings[0][1].m_RefSurfaceType = REF_SURFACE_BEST_SPHERE;
  for (int i = 0; i < 4; i++) {
    ::Settings.m_CSsmSettings[0][i].m_Method3D            = 0;
    ::Settings.m_CSsmSettings[0][i].m_MapShowSolidSurface = TRUE;
    ::Settings.m_CSsmSettings[0][i].m_MapShowWireMesh     = TRUE;
    ::Settings.m_CSsmSettings[0][i].m_SizeSmall           = TRUE;

    ::Settings.m_CSsmSettings[0][i].m_MapShowMap           = 1;
    ::Settings.m_CSsmSettings[0][i].m_MapShowEye           = 1;
    ::Settings.m_CSsmSettings[0][i].m_MapShowNumbers       = 0;
    ::Settings.m_CSsmSettings[0][i].m_MapShowPupil         = 0;
    ::Settings.m_CSsmSettings[0][i].m_MapShowLimbus        = 0;
    ::Settings.m_CSsmSettings[0][i].m_MapShowCornealVertex = 0;
    ::Settings.m_CSsmSettings[0][i].m_MapTranslucent       = 0;
    ::Settings.m_CSsmSettings[0][i].m_MapShowRings         = FALSE;

    CScale *pScale = ::Settings.m_CSsmSettings[0][i].GetScale();
    pScale->m_Step = 0.5;
    ::Settings.m_CSsmSettings[0][i].SetDefaultColors(1);
  }
  // Set new CT Summary display Done

  // Set Default buttom name
  ::Settings.SetButtonName(0, 0, "Visual Acuity");
  ::Settings.SetButtonName(0, 1, "WF/RMS");
  ::Settings.SetButtonName(0, 2, "Angle K/A");
  ::Settings.SetButtonName(0, 3, "Custom");
  ::Settings.SetButtonName(0, 4, "Depth of Focus");
  ::NewSettings.SetNewWFBtnName(0, 0, "Depth of Focus Comparison");

  ::Settings.SetButtonName(1, 0, "CT Summary");
  ::Settings.SetButtonName(1, 1, "Keratometry");
  ::Settings.SetButtonName(1, 2, "3D Z-Elevation");
  ::Settings.SetButtonName(1, 4, "Osher Iris Imaging");
  ::Settings.SetButtonName(1, 3, "Custom");
  // CT disply names Done

  ::Settings.SetButtonName(2, 0, "Chang Analysis");

  ::Settings.SetButtonName(2, 1, "Summary");

  ::Settings.SetButtonName(2, 2, "M T F");

  ::Settings.SetButtonName(2, 4, "Astigmatism Source");
  ::Settings.SetButtonName(2, 3, "Custom");

  // WFCT Angle ka
  ::Settings.SetButtonName(2, 7, "Angle K/A");
  // WFCT Angle ka

  ::Settings.m_DefaultWFSum = 11;
  ::Settings.m_DefaultCTSum = 11;
  ::Settings.m_DefaultINSum = 6;
  // Set Default buttom name Done
}

//***************************************************************************************

// Set the default Presbia showing zone setting
void CiTraceApp::SetDef_Pre_SowhZone()
{
  ::NewSettings.m_ShowPreZone_WF_VFAWnd = FALSE;
  ::NewSettings.m_ShowPreZone_WF_CMPWnd = FALSE;

  for (int i = 0; i < 4; i++) {
    ::NewSettings.m_ShowPreZone_WF_VA_Wnd[i]  = FALSE;
    ::NewSettings.m_ShowPreZone_WF_AKA_Wnd[i] = FALSE;
  }
}

//***************************************************************************************

void CiTraceApp::SetDef_SoloSettings()
{
  ::NewSettings.m_DefaultWFSolo = 1; // 0:Point; 1:Refraction; 2:Aberration; 3:VFA

  // Refraction Analysis Settings
  for (int i = 0; i < 4; i++) {
    ::NewSettings.m_WFSoloSettings[0][i].m_Type = TYPE_TLTR; // visual acuity
    switch (i) {
    case 0:
      ::NewSettings.m_WFSoloSettings[0][i].m_Mask.SetType(MASK_TOTAL);
      break;
    case 1:
      ::NewSettings.m_WFSoloSettings[0][i].m_Mask.SetType(MASK_HO_TOTAL);
      break;
    case 2:
      ::NewSettings.m_WFSoloSettings[0][i].m_Mask.SetType(MASK_TOTAL);
      break;
    case 3:
      ::NewSettings.m_WFSoloSettings[0][i].m_Mask.SetType(MASK_HO_TOTAL);
      break;
    }
    ::NewSettings.m_WFSoloSettings[0][i].m_SizeSmall            = TRUE;
    ::NewSettings.m_WFSoloSettings[0][i].m_Piston               = FALSE;
    ::NewSettings.m_WFSoloSettings[0][i].m_Tilt                 = FALSE;
    ::NewSettings.m_WFSoloSettings[0][i].m_Method3D             = FALSE;
    ::NewSettings.m_WFSoloSettings[0][i].m_MapShowSolidSurface  = TRUE;
    ::NewSettings.m_WFSoloSettings[0][i].m_MapShowWireMesh      = TRUE;
    ::NewSettings.m_WFSoloSettings[0][i].m_MapUnit              = DIOPTERS;
    ::NewSettings.m_WFSoloSettings[0][i].m_MapShowMap           = 1;
    ::NewSettings.m_WFSoloSettings[0][i].m_MapShowEye           = 0;
    ::NewSettings.m_WFSoloSettings[0][i].m_MapShowNumbers       = 0;
    ::NewSettings.m_WFSoloSettings[0][i].m_MapShowPupil         = 0;
    ::NewSettings.m_WFSoloSettings[0][i].m_MapShowLimbus        = 0;
    ::NewSettings.m_WFSoloSettings[0][i].m_MapShowCornealVertex = 0;
    ::NewSettings.m_WFSoloSettings[0][i].m_MapTranslucent       = 0;
    ::NewSettings.m_WFSoloSettings[0][i].m_MapShowKeratometry   = 0;
    ::NewSettings.m_WFSoloSettings[0][i].m_ZoneRadiusUm         = 5000;

    CScale *pScale = ::NewSettings.m_WFSoloSettings[0][i].GetScale();

    pScale->m_Step = 0.5;
    ::NewSettings.m_WFSoloSettings[0][i].SetDefaultColors(1);
    ::NewSettings.m_WFSoloSettings[0][i].m_Ltr            = 0;
    ::NewSettings.m_WFSoloSettings[0][i].m_LtrLine        = 70;
    ::NewSettings.m_WFSoloSettings[0][i].m_LtrOrientation = 0;
  }
  // Done

  // Aberration Analysis Settings
  ::NewSettings.m_WFSoloSettings[1][0].m_Type = 10; // Wavefront map
  ::NewSettings.m_WFSoloSettings[1][0].m_Mask.SetType(MASK_TOTAL);
  ::NewSettings.m_WFSoloSettings[1][0].m_SizeSmall            = TRUE;
  ::NewSettings.m_WFSoloSettings[1][0].m_Piston               = FALSE;
  ::NewSettings.m_WFSoloSettings[1][0].m_Tilt                 = FALSE;
  ::NewSettings.m_WFSoloSettings[1][0].m_Method3D             = 0;
  ::NewSettings.m_WFSoloSettings[1][0].m_MapShowSolidSurface  = TRUE;
  ::NewSettings.m_WFSoloSettings[1][0].m_MapShowWireMesh      = TRUE;
  ::NewSettings.m_WFSoloSettings[1][0].m_MapUnit              = DIOPTERS;
  ::NewSettings.m_WFSoloSettings[1][0].m_MapShowMap           = 1;
  ::NewSettings.m_WFSoloSettings[1][0].m_MapShowEye           = 0;
  ::NewSettings.m_WFSoloSettings[1][0].m_MapShowNumbers       = 0;
  ::NewSettings.m_WFSoloSettings[1][0].m_MapShowPupil         = 0;
  ::NewSettings.m_WFSoloSettings[1][0].m_MapShowLimbus        = 0;
  ::NewSettings.m_WFSoloSettings[1][0].m_MapShowCornealVertex = 0;
  ::NewSettings.m_WFSoloSettings[1][0].m_MapTranslucent       = 0;
  ::NewSettings.m_WFSoloSettings[1][0].m_MapShowKeratometry   = 0;
  ::NewSettings.m_WFSoloSettings[1][0].m_ZoneRadiusUm         = 5000;

  CScale *pScale = ::NewSettings.m_WFSoloSettings[1][0].GetScale();

  pScale->m_Step = 0.5;
  ::NewSettings.m_WFSoloSettings[1][0].SetDefaultColors(1);
  ::NewSettings.m_WFSoloSettings[1][0].m_Ltr            = 0;
  ::NewSettings.m_WFSoloSettings[1][0].m_LtrLine        = 70;
  ::NewSettings.m_WFSoloSettings[1][0].m_LtrOrientation = 0;

  ::NewSettings.m_WFSoloSettings[1][1].m_Type = 10; // Wavefront map
  ::NewSettings.m_WFSoloSettings[1][1].m_Mask.SetType(MASK_HO_TOTAL);
  ::NewSettings.m_WFSoloSettings[1][1].m_SizeSmall            = TRUE;
  ::NewSettings.m_WFSoloSettings[1][1].m_Piston               = FALSE;
  ::NewSettings.m_WFSoloSettings[1][1].m_Tilt                 = FALSE;
  ::NewSettings.m_WFSoloSettings[1][1].m_Method3D             = 0;
  ::NewSettings.m_WFSoloSettings[1][1].m_MapShowSolidSurface  = TRUE;
  ::NewSettings.m_WFSoloSettings[1][1].m_MapShowWireMesh      = TRUE;
  ::NewSettings.m_WFSoloSettings[1][1].m_MapUnit              = DIOPTERS;
  ::NewSettings.m_WFSoloSettings[1][1].m_MapShowMap           = 1;
  ::NewSettings.m_WFSoloSettings[1][1].m_MapShowEye           = 0;
  ::NewSettings.m_WFSoloSettings[1][1].m_MapShowNumbers       = 0;
  ::NewSettings.m_WFSoloSettings[1][1].m_MapShowPupil         = 0;
  ::NewSettings.m_WFSoloSettings[1][1].m_MapShowLimbus        = 0;
  ::NewSettings.m_WFSoloSettings[1][1].m_MapShowCornealVertex = 0;
  ::NewSettings.m_WFSoloSettings[1][1].m_MapTranslucent       = 0;
  ::NewSettings.m_WFSoloSettings[1][1].m_MapShowKeratometry   = 0;
  ::NewSettings.m_WFSoloSettings[1][1].m_ZoneRadiusUm         = 5000;

  pScale = ::NewSettings.m_WFSoloSettings[1][1].GetScale();

  pScale->m_Step = 0.5;
  ::NewSettings.m_WFSoloSettings[1][1].SetDefaultColors(1);
  ::NewSettings.m_WFSoloSettings[1][1].m_Ltr            = 0;
  ::NewSettings.m_WFSoloSettings[1][1].m_LtrLine        = 70;
  ::NewSettings.m_WFSoloSettings[1][1].m_LtrOrientation = 0;

  ::NewSettings.m_WFSoloSettings[1][2].m_Type = TYPE_TRMS; // TYPE_TMTF;
  ::NewSettings.m_WFSoloSettings[1][2].m_Mask.SetType(MASK_TOTAL);
  ::NewSettings.m_WFSoloSettings[1][2].m_RMSIndividual        = FALSE; // Combined
  ::NewSettings.m_WFSoloSettings[1][2].m_SizeSmall            = TRUE;
  ::NewSettings.m_WFSoloSettings[1][2].m_Piston               = FALSE;
  ::NewSettings.m_WFSoloSettings[1][2].m_Tilt                 = FALSE;
  ::NewSettings.m_WFSoloSettings[1][2].m_Method3D             = 0;
  ::NewSettings.m_WFSoloSettings[1][2].m_MapShowSolidSurface  = TRUE;
  ::NewSettings.m_WFSoloSettings[1][2].m_MapShowWireMesh      = TRUE;
  ::NewSettings.m_WFSoloSettings[1][2].m_MapUnit              = DIOPTERS;
  ::NewSettings.m_WFSoloSettings[1][2].m_MapShowMap           = 1;
  ::NewSettings.m_WFSoloSettings[1][2].m_MapShowEye           = 0;
  ::NewSettings.m_WFSoloSettings[1][2].m_MapShowNumbers       = 0;
  ::NewSettings.m_WFSoloSettings[1][2].m_MapShowPupil         = 0;
  ::NewSettings.m_WFSoloSettings[1][2].m_MapShowLimbus        = 0;
  ::NewSettings.m_WFSoloSettings[1][2].m_MapShowCornealVertex = 0;
  ::NewSettings.m_WFSoloSettings[1][2].m_MapTranslucent       = 0;
  ::NewSettings.m_WFSoloSettings[1][2].m_MapShowKeratometry   = 0;
  ::NewSettings.m_WFSoloSettings[1][2].m_ZoneRadiusUm         = 5000;

  ::NewSettings.m_WFSoloSettings[1][3].m_Type = TYPE_TRMS;
  ::NewSettings.m_WFSoloSettings[1][3].m_Mask.SetType(MASK_HO_TOTAL);
  ::NewSettings.m_WFSoloSettings[1][3].m_RMSIndividual        = TRUE;
  ::NewSettings.m_WFSoloSettings[1][3].m_SizeSmall            = TRUE;
  ::NewSettings.m_WFSoloSettings[1][3].m_Piston               = FALSE;
  ::NewSettings.m_WFSoloSettings[1][3].m_Tilt                 = FALSE;
  ::NewSettings.m_WFSoloSettings[1][3].m_Method3D             = 0;
  ::NewSettings.m_WFSoloSettings[1][3].m_MapShowSolidSurface  = TRUE;
  ::NewSettings.m_WFSoloSettings[1][3].m_MapShowWireMesh      = TRUE;
  ::NewSettings.m_WFSoloSettings[1][3].m_MapUnit              = DIOPTERS;
  ::NewSettings.m_WFSoloSettings[1][3].m_MapShowMap           = 1;
  ::NewSettings.m_WFSoloSettings[1][3].m_MapShowEye           = 0;
  ::NewSettings.m_WFSoloSettings[1][3].m_MapShowNumbers       = 0;
  ::NewSettings.m_WFSoloSettings[1][3].m_MapShowPupil         = 0;
  ::NewSettings.m_WFSoloSettings[1][3].m_MapShowLimbus        = 0;
  ::NewSettings.m_WFSoloSettings[1][3].m_MapShowCornealVertex = 0;
  ::NewSettings.m_WFSoloSettings[1][3].m_MapTranslucent       = 0;
  ::NewSettings.m_WFSoloSettings[1][3].m_MapShowKeratometry   = 0;
  ::NewSettings.m_WFSoloSettings[1][3].m_ZoneRadiusUm         = 5000;
}

//***************************************************************************************

void CiTraceApp::addNewSetting(int type)
{
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      ::Settings.m_IDsmSettings[i][j].m_MapShowEye = 0;
    }
  }

  if (type <= 1) {
    // Excel Export
    for (int i = 0; i < 89; i++) {
      if (i < 53)
        ::Settings.m_EXCEL_WF_SETS[i] = '0';
      if (i < 68)
        ::Settings.m_EXCEL_CT_SETS[i] = '0';
      if (i < 89)
        ::Settings.m_EXCEL_WFCT_SETS[i] = '0';
    }

    ::Settings.m_EXCEL_WFCT_SETS[0] = '3';
    // Excel Export	Done

    ::NewSettings.m_NEW_EXCEL_WF_SETS[0]   = '0'; // WFOpacity Grade
    ::NewSettings.m_NEW_EXCEL_WFCT_SETS[0] = '0'; // WFCT DLI Value
    ::NewSettings.m_NEW_EXCEL_WFCT_SETS[1] = '0'; // WFCT Opacity Grade

    ::Settings.m_Cap_ColorImg        = 0;
    ::Settings.m_Cap_ColorAutoOrManu = 1; //???????
  }

  ::Settings.m_AutoScanDiameterUm = 6500;

  // The default simplified RMS GUI is TRUE
  for (int i = 0; i < 4; i++) {
    if (i == 0) {
      ::Settings.m_TScmSettings.m_SimplifiedRMS     = TRUE;
      ::Settings.m_TScmSettings.m_SimplifiedWFPanel = TRUE;
      ::Settings.m_TScmSettings.m_SimplifiedCTPanel = TRUE;

      ::Settings.m_TMsmSettings.m_SimplifiedRMS     = TRUE;
      ::Settings.m_TMsmSettings.m_SimplifiedWFPanel = TRUE;
      ::Settings.m_TMsmSettings.m_SimplifiedCTPanel = TRUE;

      ::Settings.m_CSvrSettings.m_SimplifiedRMS     = TRUE;
      ::Settings.m_CSvrSettings.m_SimplifiedWFPanel = TRUE;
      ::Settings.m_CSvrSettings.m_SimplifiedCTPanel = TRUE;

      ::Settings.m_CScmSettings.m_SimplifiedRMS     = TRUE;
      ::Settings.m_CScmSettings.m_SimplifiedWFPanel = TRUE;
      ::Settings.m_CScmSettings.m_SimplifiedCTPanel = TRUE;

      ::Settings.m_PlannerSettings.m_SimplifiedRMS     = TRUE;
      ::Settings.m_PlannerSettings.m_SimplifiedWFPanel = TRUE;
      ::Settings.m_PlannerSettings.m_SimplifiedCTPanel = TRUE;
    }

    if (i < 2) {
      ::Settings.m_CDsmSettings[i].m_SimplifiedRMS     = TRUE;
      ::Settings.m_CDsmSettings[i].m_SimplifiedWFPanel = TRUE;
      ::Settings.m_CDsmSettings[i].m_SimplifiedCTPanel = TRUE;
    }

    if (i < 3) {
      ::Settings.m_WTsmSettings[i].m_SimplifiedRMS     = TRUE;
      ::Settings.m_WTsmSettings[i].m_SimplifiedWFPanel = TRUE;
      ::Settings.m_WTsmSettings[i].m_SimplifiedCTPanel = TRUE;
    }

    ::Settings.m_TSvrSettings[i].m_SimplifiedRMS     = TRUE;
    ::Settings.m_TSvrSettings[i].m_SimplifiedWFPanel = TRUE;
    ::Settings.m_TSvrSettings[i].m_SimplifiedCTPanel = TRUE;

    ::Settings.m_TDsmSettings[i].m_SimplifiedRMS     = TRUE;
    ::Settings.m_TDsmSettings[i].m_SimplifiedWFPanel = TRUE;
    ::Settings.m_TDsmSettings[i].m_SimplifiedCTPanel = TRUE;
  }

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ::Settings.m_TSsmSettings[i][j].m_SimplifiedRMS     = TRUE;
      ::Settings.m_TSsmSettings[i][j].m_SimplifiedWFPanel = TRUE;
      ::Settings.m_TSsmSettings[i][j].m_SimplifiedCTPanel = TRUE;

      ::Settings.m_CSsmSettings[i][j].m_SimplifiedRMS     = TRUE;
      ::Settings.m_CSsmSettings[i][j].m_SimplifiedWFPanel = TRUE;
      ::Settings.m_CSsmSettings[i][j].m_SimplifiedCTPanel = TRUE;

      ::Settings.m_IDsmSettings[i][j].m_SimplifiedRMS     = TRUE;
      ::Settings.m_IDsmSettings[i][j].m_SimplifiedWFPanel = TRUE;
      ::Settings.m_IDsmSettings[i][j].m_SimplifiedCTPanel = TRUE;
    }
  }
  // The default simplified RMS GUI is TRUE Done
}

//***************************************************************************************

// Set Default Task settings
void CiTraceApp::SetDef_TaskSettings()
{
  ::NewSettings.m_ShowTask = TRUE;
  CString Def_Name;
  int     Def_Num;

  for (int i = 0; i < 5; i++) {
    if (i == 0)
      ::NewSettings.m_Tasks[i].m_Enable = TRUE;
    else
      ::NewSettings.m_Tasks[i].m_Enable = FALSE;

    ::NewSettings.m_Tasks[i].m_DefNum = 0;

    for (int j = 0; j < 10; j++)
      ::NewSettings.m_Tasks[i].m_SelfSettings[j] = FALSE;

    if (i == 0) {
      Def_Name = "Cataract Evaluation";

      if (m_isCombo) {
        Def_Num = 5;

        ::NewSettings.m_Tasks[i].m_SubButton[0] = TYPE_DYSP;
        ::NewSettings.m_Tasks[i].m_SubButton[1] = TYPE_CHAN;
        ::NewSettings.m_Tasks[i].m_SubButton[2] = TYPE_ANGK;
        ::NewSettings.m_Tasks[i].m_SubButton[3] = TYPE_TOAL; // 6.2.0
        ::NewSettings.m_Tasks[i].m_SubButton[4] = TYPE_TORI;
        ::NewSettings.m_Tasks[i].m_SubButton[5] = TYPE_NONA;
        ::NewSettings.m_Tasks[i].m_SubButton[6] = TYPE_NONA;
        ::NewSettings.m_Tasks[i].m_SubButton[7] = TYPE_NONA;
        ::NewSettings.m_Tasks[i].m_SubButton[8] = TYPE_NONA;
        ::NewSettings.m_Tasks[i].m_SubButton[9] = TYPE_NONA;

        CString s;

        // Check WF/CT Visual Acuity
        ::Settings.GetButtonName(2, 0, s);
        if (s != "Chang Analysis") {
          SetDefTask_CHAN(::NewSettings.m_Tasks[i].m_WndSettings[1]);
          ::NewSettings.m_Tasks[i].m_SelfSettings[2] = TRUE;
        }
        // Check WF/CT Visual Acuity Done
      }
      else {
        Def_Num = 3;

        ::NewSettings.m_Tasks[i].m_SubButton[0] = TYPE_IOLS;
        ::NewSettings.m_Tasks[i].m_SubButton[1] = TYPE_CHAN;
        ::NewSettings.m_Tasks[i].m_SubButton[2] = TYPE_TORI;
        ::NewSettings.m_Tasks[i].m_SubButton[3] = TYPE_NONA;
        ::NewSettings.m_Tasks[i].m_SubButton[4] = TYPE_NONA;
        ::NewSettings.m_Tasks[i].m_SubButton[5] = TYPE_NONA;
        ::NewSettings.m_Tasks[i].m_SubButton[6] = TYPE_NONA;
        ::NewSettings.m_Tasks[i].m_SubButton[7] = TYPE_NONA;
        ::NewSettings.m_Tasks[i].m_SubButton[8] = TYPE_NONA;
        ::NewSettings.m_Tasks[i].m_SubButton[9] = TYPE_NONA;

        CString s;

        // Check WF/CT Visual Acuity
        ::Settings.GetButtonName(2, 0, s);
        if (s != "Chang Analysis") {
          SetDefTask_CHAN(::NewSettings.m_Tasks[i].m_WndSettings[1]);
          ::NewSettings.m_Tasks[i].m_SelfSettings[1] = TRUE;
        }
        // Check WF/CT Visual Acuity Done
      }

      // Still need set the TYPE_DYSF Setting?
    }
    else if (i == 1) {
      Def_Name = "Vision Diagnostics";
      Def_Num  = 6;

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

      // Check whether WF/CT Visual Acuity is exist ( Did not changed(customed) by the user)
      CString s;
      ::Settings.GetButtonName(2, 2, s);
      if (s != "Visual Acuity") {
        SetDefTask_WCVA(::NewSettings.m_Tasks[i].m_WndSettings[3]);
        ::NewSettings.m_Tasks[i].m_SelfSettings[3] = TRUE;
      }

      // Check CT Summary
      ::Settings.GetButtonName(1, 0, s);
      if (s != "CT Summary") {
        SetDefTask_CTSU(::NewSettings.m_Tasks[i].m_WndSettings[4]);
        ::NewSettings.m_Tasks[i].m_SelfSettings[4] = TRUE;
      }
    }
    else if (i == 2) {
      Def_Name = "Corneal Diagnostics";
      Def_Num  = 4;

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

      // Check CT Summary
      CString s;
      ::Settings.GetButtonName(1, 0, s);
      if (s != "CT Summary") {
        SetDefTask_CTSU(::NewSettings.m_Tasks[i].m_WndSettings[0]);
        ::NewSettings.m_Tasks[i].m_SelfSettings[0] = TRUE;
      }

      // Check Keratometry
      ::Settings.GetButtonName(1, 1, s);
      if (s != "Keratometry") {
        SetDefTask_CTKE(::NewSettings.m_Tasks[i].m_WndSettings[1]);
        ::NewSettings.m_Tasks[i].m_SelfSettings[1] = TRUE;
      }

      // Check WF/CT Visual Acuity
      ::Settings.GetButtonName(2, 2, s);
      if (s != "Visual Acuity") {
        SetDefTask_WCVA(::NewSettings.m_Tasks[i].m_WndSettings[2]);
        ::NewSettings.m_Tasks[i].m_SelfSettings[2] = TRUE;
      }

      // Check Chang
      ::Settings.GetButtonName(2, 0, s);
      if (s != "Chang Analysis") {
        SetDefTask_CHAN(::NewSettings.m_Tasks[i].m_WndSettings[3]);
        ::NewSettings.m_Tasks[i].m_SelfSettings[3] = TRUE;
      }
    }
    else if (i == 3) {
      Def_Name = "Toric Planner";
      Def_Num  = 1;

      ::NewSettings.m_Tasks[i].m_SubButton[0] = TYPE_TORI;
      for (int j = 1; j < 10; j++)
        ::NewSettings.m_Tasks[i].m_SubButton[j] = TYPE_NONA;
    }
    else if (i == 4) {
      if (m_isCombo) {
        Def_Name = "Accomm and DOF Analysis";

        Def_Num = 2;

        ::NewSettings.m_Tasks[i].m_SubButton[0] = TYPE_ACCO;
        ::NewSettings.m_Tasks[i].m_SubButton[1] = TYPE_DEPF;
        for (int j = 2; j < 10; j++)
          ::NewSettings.m_Tasks[i].m_SubButton[j] = TYPE_NONA;
      }
      else {
        Def_Name = "Accommdation Analysis";

        Def_Num = 1;

        ::NewSettings.m_Tasks[i].m_SubButton[0] = TYPE_ACCO;
        for (int j = 1; j < 10; j++)
          ::NewSettings.m_Tasks[i].m_SubButton[j] = TYPE_NONA;
      }
    }

    G_As = Def_Name;
    strncpy(&::NewSettings.m_Tasks[i].m_ButtonName[0], G_As, 25);

    ::NewSettings.m_Tasks[i].m_SubBtnNum = Def_Num;
  }
}

//********************************************************************************************

void CiTraceApp::SetDefTask_WCVA(CWndSettings InWndSetting[4])
{
  InWndSetting[0] = ::Settings.m_IDsmSettings[2][0];
  InWndSetting[1] = ::Settings.m_IDsmSettings[2][1];
  InWndSetting[2] = ::Settings.m_IDsmSettings[2][2];
  InWndSetting[3] = ::Settings.m_IDsmSettings[2][3];

  InWndSetting[0].m_Type                = TYPE_ILTR;
  InWndSetting[0].m_Method3D            = FALSE;
  InWndSetting[0].m_MapShowSolidSurface = TRUE;
  InWndSetting[0].m_MapShowWireMesh     = TRUE;
  InWndSetting[0].m_SizeSmall           = TRUE;
  InWndSetting[0].m_ZoneRadiusUm        = 5000;
  InWndSetting[0].m_Mask.SetType(MASK_TOTAL_NO_DEFOCUS);
  InWndSetting[0].m_Piston = TRUE;
  InWndSetting[0].m_Tilt   = TRUE;

  for (int i = 0; i < 3; i++) {
    InWndSetting[i].m_Ltr               = 0;
    InWndSetting[i].m_SimplifiedRMS     = TRUE;
    InWndSetting[i].m_SimplifiedWFPanel = TRUE;
    InWndSetting[i].m_SimplifiedCTPanel = TRUE;

    InWndSetting[i].m_SizeSmall = TRUE;
  }

  InWndSetting[0].m_LtrLine        = 70;
  InWndSetting[0].m_LtrOrientation = 0;

  InWndSetting[3].m_Type                 = 31; // Axial map       TYPE_CAXM
  InWndSetting[3].m_Method3D             = 0;
  InWndSetting[3].m_MapShowSolidSurface  = TRUE;
  InWndSetting[3].m_MapShowWireMesh      = TRUE;
  InWndSetting[3].m_SizeSmall            = TRUE;
  InWndSetting[3].m_MapUnit              = DIOPTERS;
  InWndSetting[3].m_MapShowMap           = 1;
  InWndSetting[3].m_MapShowEye           = 1;
  InWndSetting[3].m_MapShowNumbers       = 0;
  InWndSetting[3].m_MapShowPupil         = 0;
  InWndSetting[3].m_MapShowLimbus        = 0;
  InWndSetting[3].m_MapShowCornealVertex = 0;
  InWndSetting[3].m_MapTranslucent       = 0;
  InWndSetting[3].m_MapShowKeratometry   = 1;

  InWndSetting[3].SetDefaultScale();

  for (int i = 0; i < 3; i++) {
    InWndSetting[i].m_MapShowEye = 0;
    InWndSetting[i].SetDefaultColors(1);
  }
}

//********************************************************************************************

void CiTraceApp::SetDefTask_CTSU(CWndSettings InWndSetting[4])
{
  InWndSetting[0] = ::Settings.m_CSsmSettings[0][0];
  InWndSetting[1] = ::Settings.m_CSsmSettings[0][1];
  InWndSetting[2] = ::Settings.m_CSsmSettings[0][2];
  InWndSetting[3] = ::Settings.m_CSsmSettings[0][3];

  InWndSetting[0].m_Type = 31; // Axial map       TYPE_CAXM
  InWndSetting[1].m_Type = 34; // z elevation     TYPE_CELM
  InWndSetting[2].m_Type = 32; // local Roc  map  TYPE_CTNM
  InWndSetting[3].m_Type = 33; // refrective map  TYPE_CRFM

  InWndSetting[0].m_MapUnit = DIOPTERS;
  InWndSetting[2].m_MapUnit = MILLIMETERS;

  InWndSetting[1].m_RefSurfaceType = REF_SURFACE_BEST_SPHERE;

  for (int i = 0; i < 4; i++) {
    InWndSetting[i].m_Method3D            = 0;
    InWndSetting[i].m_MapShowSolidSurface = TRUE;
    InWndSetting[i].m_MapShowWireMesh     = TRUE;
    InWndSetting[i].m_SizeSmall           = TRUE;

    InWndSetting[i].m_MapShowMap           = 1;
    InWndSetting[i].m_MapShowEye           = 1;
    InWndSetting[i].m_MapShowNumbers       = 0;
    InWndSetting[i].m_MapShowPupil         = 0;
    InWndSetting[i].m_MapShowLimbus        = 0;
    InWndSetting[i].m_MapShowCornealVertex = 0;
    InWndSetting[i].m_MapTranslucent       = 0;

    InWndSetting[i].SetDefaultScale();
    InWndSetting[i].SetDefaultColors(1);
  }
}

//********************************************************************************************

void CiTraceApp::SetDefTask_CTKE(CWndSettings InWndSetting[4])
{
  InWndSetting[0] = ::Settings.m_CSsmSettings[1][0];
  InWndSetting[1] = ::Settings.m_CSsmSettings[1][1];
  InWndSetting[2] = ::Settings.m_CSsmSettings[1][2];
  InWndSetting[3] = ::Settings.m_CSsmSettings[1][3];

  InWndSetting[0].m_Type = 51; // Axial map       TYPE_CAXM
  InWndSetting[1].m_Type = 34; // z elevation     TYPE_CELM
  InWndSetting[2].m_Type = 32; // local Roc  map  TYPE_CTNM
  InWndSetting[3].m_Type = 33; // refrective map  TYPE_CRFM

  InWndSetting[0].m_MapUnit = DIOPTERS;
  InWndSetting[2].m_MapUnit = MILLIMETERS;

  InWndSetting[1].m_RefSurfaceType = REF_SURFACE_BEST_SPHERE;

  for (int i = 0; i < 4; i++) {
    InWndSetting[i].m_Method3D            = 0;
    InWndSetting[i].m_MapShowSolidSurface = TRUE;
    InWndSetting[i].m_MapShowWireMesh     = TRUE;
    InWndSetting[i].m_SizeSmall           = TRUE;

    InWndSetting[i].m_MapShowMap           = 1;
    InWndSetting[i].m_MapShowEye           = 1;
    InWndSetting[i].m_MapShowNumbers       = 0;
    InWndSetting[i].m_MapShowPupil         = 0;
    InWndSetting[i].m_MapShowLimbus        = 0;
    InWndSetting[i].m_MapShowCornealVertex = 0;
    InWndSetting[i].m_MapTranslucent       = 0;

    InWndSetting[i].SetDefaultScale();
    InWndSetting[i].SetDefaultColors(1);
  }

  InWndSetting[0].m_SizeSmall = FALSE;
}

//********************************************************************************************

void CiTraceApp::SetDefTask_CHAN(CWndSettings InWndSetting[4])
{
  InWndSetting[0] = ::Settings.m_IDsmSettings[0][0];
  InWndSetting[1] = ::Settings.m_IDsmSettings[0][1];
  InWndSetting[2] = ::Settings.m_IDsmSettings[0][2];
  InWndSetting[3] = ::Settings.m_IDsmSettings[0][3];

  InWndSetting[0].m_Type                = 41; // RMS map        TYPE_IRMS
  InWndSetting[0].m_Method3D            = 0;
  InWndSetting[0].m_MapShowSolidSurface = TRUE;
  InWndSetting[0].m_MapShowWireMesh     = TRUE;
  InWndSetting[0].m_SizeSmall           = TRUE;
  InWndSetting[0].m_ZoneRadiusUm        = 5000;
  InWndSetting[0].m_Mask.SetType(MASK_TOTAL_NO_DEFOCUS);
  InWndSetting[0].m_Piston        = TRUE;
  InWndSetting[0].m_Tilt          = TRUE;
  InWndSetting[0].m_RMSIndividual = TRUE;

  InWndSetting[3].m_Type                 = 31; // Axial map       TYPE_CAXM
  InWndSetting[3].m_Method3D             = 0;
  InWndSetting[3].m_MapShowSolidSurface  = TRUE;
  InWndSetting[3].m_MapShowWireMesh      = TRUE;
  InWndSetting[3].m_SizeSmall            = TRUE;
  InWndSetting[3].m_MapUnit              = DIOPTERS;
  InWndSetting[3].m_MapShowMap           = 1;
  InWndSetting[3].m_MapShowEye           = 1;
  InWndSetting[3].m_MapShowNumbers       = 0;
  InWndSetting[3].m_MapShowPupil         = 0;
  InWndSetting[3].m_MapShowLimbus        = 0;
  InWndSetting[3].m_MapShowCornealVertex = 0;
  InWndSetting[3].m_MapTranslucent       = 0;
  InWndSetting[3].m_MapShowKeratometry   = 1;

  for (int i = 0; i < 4; i++) {
    InWndSetting[i].m_SizeSmall         = TRUE;
    InWndSetting[i].m_SimplifiedRMS     = TRUE;
    InWndSetting[i].m_SimplifiedWFPanel = TRUE;
    InWndSetting[i].m_SimplifiedCTPanel = TRUE;
  }

  InWndSetting[0].SetDefaultScale();
  InWndSetting[0].SetDefaultColors(1);

  InWndSetting[3].SetDefaultScale();
  InWndSetting[3].SetDefaultColors(1);
}

//********************************************************************************************

void CiTraceApp::SetDefTask_WFAN(CWndSettings InWndSetting[4])
{
  InWndSetting[0] = ::Settings.m_TSsmSettings[2][0];
  InWndSetting[1] = ::Settings.m_TSsmSettings[2][1];
  InWndSetting[2] = ::Settings.m_TSsmSettings[2][2];
  InWndSetting[3] = ::Settings.m_TSsmSettings[2][3];

  for (int i = 0; i < 4; i++) {
    InWndSetting[i].m_Type                 = 1;
    InWndSetting[i].m_MapShowPupil         = 1;
    InWndSetting[i].m_MapShowLimbus        = 1;
    InWndSetting[i].m_MapShowCornealVertex = 1;
    InWndSetting[i].m_SizeSmall            = 1;
  }

  InWndSetting[0].m_SizeSmall = FALSE;
}

//********************************************************************************************

void CiTraceApp::TransformNewSettings()
{
  ::NewSettings.m_IncLoC_OD    = ::OldNewSettings.m_IncLoC_OD;
  ::NewSettings.m_IncLoC_OS    = ::OldNewSettings.m_IncLoC_OS;
  ::NewSettings.m_SurInAst_OD  = ::OldNewSettings.m_SurInAst_OD;
  ::NewSettings.m_SurInAst_OS  = ::OldNewSettings.m_SurInAst_OS;
  ::NewSettings.m_Adjust_CT    = ::OldNewSettings.m_Adjust_CT;
  ::NewSettings.m_Adjusting_CT = ::OldNewSettings.m_Adjusting_CT;

  for (int i = 0; i < 11; i++) {
    ::NewSettings.WFOD_COL_Width[i] = ::OldNewSettings.WFOD_COL_Width[i]; // the width of WFODLIST columns
    ::NewSettings.WFOS_COL_Width[i] = ::OldNewSettings.WFOS_COL_Width[i]; // the width of WFOSLIST columns
    ::NewSettings.CTOD_COL_Width[i] = ::OldNewSettings.CTOD_COL_Width[i]; // the width of CTODLIST columns
    ::NewSettings.CTOS_COL_Width[i] = ::OldNewSettings.CTOS_COL_Width[i]; // the width of CTOSLIST columns
  }

  for (int i = 0; i < 4; i++)
    ::NewSettings.m_OSHER_ALIG_CUS[i] = ::OldNewSettings.m_OSHER_ALIG_CUS[i]; // for the CT OSHER alignment custom

  for (int i = 0; i < 4; i++)
    ::NewSettings.m_OWFCTSettings[i] = ::OldNewSettings.m_OWFCTSettings[i]; // CWFCTOUSumWnd

  ::NewSettings.m_ScreenShotFormat = ::OldNewSettings.m_ScreenShotFormat; // 0: jpg(default) ; 1: png;  2: bmp; 3: dcm;

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 4; j++) {
      ::NewSettings.m_WFSoloSettings[i][j] = ::OldNewSettings.m_WFSoloSettings[i][j]; // SOLO Aberration and Refraction analysis settings
    }
  }

  ::NewSettings.m_DefaultWFSolo = ::OldNewSettings.m_DefaultWFSolo;

  for (int i = 0; i < 6; i++)
    ::NewSettings.m_IOLSelSettings[i] = ::OldNewSettings.m_IOLSelSettings[i]; // IOL SELECTION wnd

  for (int i = 0; i < 2; i++)
    ::NewSettings.m_IOLSel_Alpha[i] = ::OldNewSettings.m_IOLSel_Alpha[i]; // IOL SELECTION threshold

  ::NewSettings.m_IOLSel_CHOA = ::OldNewSettings.m_IOLSel_CHOA; // IOL SELECTION Corneal hoa threshold

  ::NewSettings.m_IOLSel_Astig = ::OldNewSettings.m_IOLSel_Astig; // IOL SELECTION threshold

  for (int i = 0; i < 4; i++)
    ::NewSettings.m_IOLSel_Sph_Abbe[i] = ::OldNewSettings.m_IOLSel_Sph_Abbe[i]; // IOL SELECTION threshold

  for (int i = 0; i < 4; i++)
    ::NewSettings.m_InAstigSettings[i] = ::OldNewSettings.m_InAstigSettings[i]; // Astigmatism Source Display

  for (int i = 0; i < 4; i++)
    ::NewSettings.m_Tasks[i] = ::OldNewSettings.m_Tasks[i]; // for tasks settings

  ::NewSettings.m_ShowPreZone_WF_VFAWnd = ::OldNewSettings.m_ShowPreZone_WF_VFAWnd; // for showing presbia zone in wf VFA window
  ::NewSettings.m_ShowPreZone_WF_CMPWnd = ::OldNewSettings.m_ShowPreZone_WF_CMPWnd; // for showing presbia zone in wf Compare window

  for (int i = 0; i < 4; i++)
    ::NewSettings.m_ShowPreZone_WF_VA_Wnd[i] =
      ::OldNewSettings.m_ShowPreZone_WF_VA_Wnd[i]; // for showing presbia zone in wf Visual Acuity window
  for (int i = 0; i < 4; i++)
    ::NewSettings.m_ShowPreZone_WF_AKA_Wnd[i] =
      ::OldNewSettings.m_ShowPreZone_WF_AKA_Wnd[i]; // for showing presbia zone in wf Angle A/K window

  ::NewSettings.m_ShowTask = ::OldNewSettings.m_ShowTask; // for task settings

  ::NewSettings.m_IOLSel_IHOA = ::OldNewSettings.m_IOLSel_IHOA; // IOL SELECTION  Internal hoa threshold
}

//********************************************************************************************

void CiTraceApp::Transform601NewSettings()
{
  ::NewSettings.m_IncLoC_OD    = ::Old601NewSettings.m_IncLoC_OD;
  ::NewSettings.m_IncLoC_OS    = ::Old601NewSettings.m_IncLoC_OS;
  ::NewSettings.m_SurInAst_OD  = ::Old601NewSettings.m_SurInAst_OD;
  ::NewSettings.m_SurInAst_OS  = ::Old601NewSettings.m_SurInAst_OS;
  ::NewSettings.m_Adjust_CT    = ::Old601NewSettings.m_Adjust_CT;
  ::NewSettings.m_Adjusting_CT = ::Old601NewSettings.m_Adjusting_CT;

  for (int i = 0; i < 11; i++) {
    ::NewSettings.WFOD_COL_Width[i] = ::Old601NewSettings.WFOD_COL_Width[i]; // the width of WFODLIST columns
    ::NewSettings.WFOS_COL_Width[i] = ::Old601NewSettings.WFOS_COL_Width[i]; // the width of WFOSLIST columns
    ::NewSettings.CTOD_COL_Width[i] = ::Old601NewSettings.CTOD_COL_Width[i]; // the width of CTODLIST columns
    ::NewSettings.CTOS_COL_Width[i] = ::Old601NewSettings.CTOS_COL_Width[i]; // the width of CTOSLIST columns
  }

  for (int i = 0; i < 4; i++)
    ::NewSettings.m_OSHER_ALIG_CUS[i] = ::Old601NewSettings.m_OSHER_ALIG_CUS[i]; // for the CT OSHER alignment custom

  for (int i = 0; i < 4; i++)
    ::NewSettings.m_OWFCTSettings[i] = ::Old601NewSettings.m_OWFCTSettings[i]; // CWFCTOUSumWnd

  ::NewSettings.m_ScreenShotFormat = ::Old601NewSettings.m_ScreenShotFormat; // 0: jpg(default) ; 1: png;  2: bmp; 3: dcm;

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 4; j++) {
      ::NewSettings.m_WFSoloSettings[i][j] = ::Old601NewSettings.m_WFSoloSettings[i][j]; // SOLO Aberration and Refraction analysis settings
    }
  }

  ::NewSettings.m_DefaultWFSolo = ::Old601NewSettings.m_DefaultWFSolo;

  for (int i = 0; i < 6; i++)
    ::NewSettings.m_IOLSelSettings[i] = ::Old601NewSettings.m_IOLSelSettings[i]; // IOL SELECTION wnd

  for (int i = 0; i < 2; i++)
    ::NewSettings.m_IOLSel_Alpha[i] = ::Old601NewSettings.m_IOLSel_Alpha[i]; // IOL SELECTION threshold

  ::NewSettings.m_IOLSel_CHOA = ::Old601NewSettings.m_IOLSel_CHOA; // IOL SELECTION Corneal hoa threshold

  ::NewSettings.m_IOLSel_Astig = ::Old601NewSettings.m_IOLSel_Astig; // IOL SELECTION threshold

  for (int i = 0; i < 4; i++)
    ::NewSettings.m_IOLSel_Sph_Abbe[i] = ::Old601NewSettings.m_IOLSel_Sph_Abbe[i]; // IOL SELECTION threshold

  for (int i = 0; i < 4; i++)
    ::NewSettings.m_InAstigSettings[i] = ::Old601NewSettings.m_InAstigSettings[i]; // Astigmatism Source Display

  for (int i = 0; i < 5; i++)
    ::NewSettings.m_Tasks[i] = ::Old601NewSettings.m_Tasks[i]; // for tasks settings

  ::NewSettings.m_ShowPreZone_WF_VFAWnd = ::Old601NewSettings.m_ShowPreZone_WF_VFAWnd; // for showing presbia zone in wf VFA window
  ::NewSettings.m_ShowPreZone_WF_CMPWnd = ::Old601NewSettings.m_ShowPreZone_WF_CMPWnd; // for showing presbia zone in wf Compare window

  for (int i = 0; i < 4; i++)
    ::NewSettings.m_ShowPreZone_WF_VA_Wnd[i] =
      ::Old601NewSettings.m_ShowPreZone_WF_VA_Wnd[i]; // for showing presbia zone in wf Visual Acuity window
  for (int i = 0; i < 4; i++)
    ::NewSettings.m_ShowPreZone_WF_AKA_Wnd[i] =
      ::Old601NewSettings.m_ShowPreZone_WF_AKA_Wnd[i]; // for showing presbia zone in wf Angle A/K window

  ::NewSettings.m_ShowTask = ::Old601NewSettings.m_ShowTask; // for task settings

  ::NewSettings.m_IOLSel_IHOA = ::Old601NewSettings.m_IOLSel_IHOA; // IOL SELECTION  Internal hoa threshold

  ::NewSettings.m_Super_Exam = ::Old601NewSettings.m_Super_Exam;

  for (int i = 0; i < 4; i++) {
    ::NewSettings.m_DysfuncSettings[i] = ::Old601NewSettings.m_DysfuncSettings[i];
    ::NewSettings.m_TSsm5Settings[i]   = ::Old601NewSettings.m_TSsm5Settings[i];
  }

  for (int i = 0; i < 5; i++)
    ::NewSettings.m_DysPtSettings[i] = ::Old601NewSettings.m_DysPtSettings[i];

  for (int i = 0; i < 9; i++) {
    if (i < 8) {
      ::NewSettings.m_WFSumShow[i] = ::Old601NewSettings.m_WFSumShow[i];
      ::NewSettings.m_CTSumShow[i] = ::Old601NewSettings.m_CTSumShow[i];
    }
    ::NewSettings.m_WFCTSumShow[i] = ::Old601NewSettings.m_WFCTSumShow[i];
  }
}

//********************************************************************************************

void CiTraceApp::RzdcxActivation()
{
  // License details :
  // Serial Number : 33Y67339H6518913W - T
  //	E - Mail : agatz@traceytech.com
  //	Number of units : 1

  CINIParser IniPar;

  CString RzdcxAct = IniPar.GetStrKey(IniSection, "RZDCX ACT", IniPath); // 6.2.0 ini registration

  if (RzdcxAct == "0") {
  // A:
    ::Info("The Dicom Toolkit needs to be activated for COMBO DICOM Version.");

    ////-----------------------------------------Registration
    // SHELLEXECUTEINFO ShExecInfo1 = { 0 };
    // ShExecInfo1.cbSize = sizeof(SHELLEXECUTEINFO);
    // ShExecInfo1.fMask = SEE_MASK_NOCLOSEPROCESS;
    // ShExecInfo1.hwnd = NULL;
    // ShExecInfo1.lpVerb = _T("open");
    // ShExecInfo1.lpFile = _T("C:\\RZDCX\\Activation\\RegistrationTool.exe");
    // ShExecInfo1.lpParameters = _T("");
    // ShExecInfo1.lpDirectory = NULL;
    // ShExecInfo1.nShow = SW_SHOW;
    // ShExecInfo1.hInstApp = NULL;

    // ShellExecuteEx(&ShExecInfo1);

    // WaitForSingleObject(ShExecInfo1.hProcess, INFINITE);

    //-----------------------------------------------------------
    SHELLEXECUTEINFO ShExecInfo = {0};
    ShExecInfo.cbSize           = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask            = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd             = NULL;
    ShExecInfo.lpVerb           = _T("open");
    ShExecInfo.lpFile           = _T("C:\\RZDCX\\Activation\\ActivationTool.exe");
    ShExecInfo.lpParameters     = _T("");
    ShExecInfo.lpDirectory      = NULL;
    ShExecInfo.nShow            = SW_SHOW;
    ShExecInfo.hInstApp         = NULL;

    ShellExecuteEx(&ShExecInfo);

    WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

    // Check the RZDXC.License file and copy it
    if (G_Is64BitOS()) {
      CString File64Name = "C:\\RZDCX\\Win64\\RZDCX.license";

      if (::PathFileExists(File64Name)) {
        CString DesFileName = "C:\\RZDCX\\RZDCX.license";

        CopyFile(File64Name, DesFileName, TRUE);
      }
    }
    else {
      CString File32Name = "C:\\RZDCX\\Win32\\RZDCX.license";

      if (::PathFileExists(File32Name)) {
        CString DesFileName = "C:\\RZDCX\\RZDCX.license";

        CopyFile(File32Name, DesFileName, TRUE);
      }
    }
    // Done

    // Check the activation infomation
    ::Info("Program will run the Dicom activation test. "); // If the activation is not done, the program will be terminated.

    CTraceyDicom TraceyDicom;
    if (TraceyDicom.TestActivation()) {
      IniPar.SetStrKey(IniSection, "RZDCX ACT", "1", IniPath);
      ::Info("Dicom test passed. The Dicom toolkit has been activated in your computer.");
    }
    else {
      ::Info("Dicom test failed. The Dicom toolkit has not been activated in your computer. Please cotact producer.");
    }
    // Done
  }
  else {
    /*CTraceyDicom TraceyDicom;

    if (!TraceyDicom.TestActivation())
    {
      IniPar.SetStrKey(IniSection, "RZDCX ACT", "0", IniPath);
      ::Info("Dicom test failed.");
      goto A;
    }*/
  }
}

//********************************************************************************************
