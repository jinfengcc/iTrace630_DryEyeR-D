// HalTester.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "resource.h"
#include "MainDlg.h"

#include "AppInitializer.h"

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

    CMainDlg dlgMain;
    if (dlgMain.Create(NULL) == NULL) {
      ATLTRACE(_T("Main dialog creation failed!\n"));
      return 0;
    }

    dlgMain.ShowWindow(nCmdShow);

    nRet = theLoop.Run();

    //wil::exe_terminating();
    _Module.RemoveMessageLoop();
  }
  catch (std::exception &e) {
    //wil::exe_terminating();
    MessageBoxA(nullptr, e.what(), "Error", MB_OK | MB_ICONSTOP);
  }

  _Module.Term();

  return nRet;
}

