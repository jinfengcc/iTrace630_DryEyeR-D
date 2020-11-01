// HalTester.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "resource.h"
#include "MainDlg.h"

#include "AppInitializer.h"
#include "libs/CommonLib/AppSettings.h"

CAppModule _Module;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
  AppInitializer gdipInit;

  AtlInitCommonControls(ICC_BAR_CLASSES); // add flags to support other controls

  auto hRes = _Module.Init(nullptr, hInstance);
  ATLASSERT(SUCCEEDED(hRes));

  int nRet = -1;
  try {
    CMessageLoop theLoop;
    _Module.AddMessageLoop(&theLoop);

    if (__argc == 2) {
      AppSettings::CreateInstance(__wargv[1]);
      // clang-format off
      auto x1  = AppSettings::Instance()->Get<bool       >("HalTester", "x1");
      auto x2a = AppSettings::Instance()->Get<int        >("HalTester", "x2");
      auto x2b = AppSettings::Instance()->Get<double     >("HalTester", "x2");
      auto x3  = AppSettings::Instance()->Get<double     >("HalTester", "x3");
      auto x4  = AppSettings::Instance()->Get<std::string>("HalTester", "x4");
      // clang-format on
    }
    else {
      AppSettings::CreateInstance();
    }

    CMainDlg dlgMain;
    if (dlgMain.Create(nullptr) == nullptr) {
      ATLTRACE(_T("Main dialog creation failed!\n"));
      return 0;
    }

    dlgMain.ShowWindow(nCmdShow);

    nRet = theLoop.Run();

    wil::exe_terminating();
    _Module.RemoveMessageLoop();
  }
  catch (std::exception &e) {
    wil::exe_terminating();
    MessageBoxA(nullptr, e.what(), "Error", MB_OK | MB_ICONSTOP);
  }

  _Module.Term();

  return nRet;
}

