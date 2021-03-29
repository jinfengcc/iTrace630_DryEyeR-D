#pragma once

#include <libs/CommonLib/SignalUI.h>
#include "libs/CommonLib/SingleInstance.h"

class CSettings;
class CWndSettings;

class CiTraceApp : public CWinApp
{
public:
  BOOL InitInstance() override;
  int  ExitInstance() override;

  sig::Signal<const CSettings *, CSettings *> m_sigSettingsChanged;
  sig::Signal<>                               m_sigShuttingDown;
  sig::Signal<>                               m_sigDatabaseOpened;
  sig::Signal<CPatient *>                     m_siPatientAdded;

private:
  SingleInstance m_singleInstance{_T("7db4bddd-4124-4e53-abfc-39d75da4bd1e")};
  ULONG_PTR      m_GdiplusToken;

  void Registration(); // 6.2.0 ini registration
  void ControlSettings();
  void CalSinCos(); // 6.2.0
  void ControlSettings2();
  void SetDef_ExList_Width(); // set the default exam list width
  void SetDef_OWFCT();
  void SetDef_IOLSEL();
  void SetDef_AstigSou(); // set the default of Asitgmatism source display
  void SetDef_DysFun();   // set the default of Dysfunctional Lens Analysis
  void SetNew620GUI();    // 6.2.0
  void SetDef_Pre_SowhZone();
  void SetDef_TaskSettings();
  void SetDef_SoloSettings();
  void addNewSetting(int type);

  void SetDefTask_WCVA(CWndSettings InWndSetting[4]);
  void SetDefTask_CTSU(CWndSettings InWndSetting[4]);
  void SetDefTask_CTKE(CWndSettings InWndSetting[4]);
  void SetDefTask_CHAN(CWndSettings InWndSetting[4]);
  void SetDefTask_WFAN(CWndSettings InWndSetting[4]);

  void TransformNewSettings(); // 600

  void Transform601NewSettings();

  void RzdcxActivation(); // 6.2.0
};

extern CiTraceApp App;
