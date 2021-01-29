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

#if defined(_DEBUG) && 0
    AppSettings aps;
    // clang-format off
    auto x1  = aps.Get<bool       >("HalTester.x1");
    auto x2a = aps.Get<int        >("HalTester.x2");
    auto x2b = aps.Get<double     >("HalTester.x2");
    auto x3  = aps.Get<double     >("HalTester.x3");
    auto x4  = aps.Get<std::string>("HalTester.x4");

    auto xa = aps.Get<std::string>("HalTester.camera.enhance");

    auto y4 = aps.Get("HalTester.x2", 12);
    auto y5 = aps.Get("HalTester.x5", 16);

    AppSettings aps2(AppSettings(), "HalTester.camera");

    auto xb = aps2.Get<std::string>("enhance");

    // clang-format on
#endif // _DEBUG

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

