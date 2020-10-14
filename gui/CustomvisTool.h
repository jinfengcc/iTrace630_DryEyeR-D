//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Acquisition.h"
#include "WFExam.h"
#include "Settings.h"

//***************************************************************************************

#define CHANGE_MODE   20
#define INC_SCAN_SIZE 21
#define DEC_SCAN_SIZE 22
#define TARGET_UP     23
#define TARGET_DN     24
#define TARGET_HOME   25
#define TARGET_LIGHT  26

//***************************************************************************************

class CWFAcquisition : public CAcquisition
{
public:

  CWFAcquisition(CHW* pHW, CSettings* pSettings);

  CWFExam m_WFExam;

  CSettings* m_pSettings;

  BOOL m_AutoScanSize;
  BOOL m_Probing;
  BOOL m_TargetLightOn;

  void ChangeMode () {m_ToDo = CHANGE_MODE;}
  void IncScanSize() {m_ToDo = INC_SCAN_SIZE;}
  void DecScanSize() {m_ToDo = DEC_SCAN_SIZE;}
  void TargetUp   () {m_ToDo = TARGET_UP;}
  void TargetDn   () {m_ToDo = TARGET_DN;}
  void TargetHome () {m_ToDo = TARGET_HOME;}
  void TargetLight() {m_ToDo = TARGET_LIGHT; m_TargetLightOn = !m_TargetLightOn;}

private:

  virtual void Main() override;

  real m_auto_r_max_um;

  virtual void CheckAlignment() override;

  BOOL SetupScanPointsAndScan();
  BOOL DownloadScanResults();
};

//***************************************************************************************
