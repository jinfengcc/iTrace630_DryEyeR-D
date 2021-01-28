//***************************************************************************************

#include "StdAfx.h"
#include "WFAcquisition.h"
#include "SimulationDlg.h"
#include "Licensing.h"

//***************************************************************************************

#define SCAN_R_MIN_UM 1000
#define SCAN_R_MAX_UM 4000

//***************************************************************************************

CWFAcquisition::CWFAcquisition(CHW* pHW, CSettings* pSettings) : CAcquisition(pHW)
{
  m_pSettings = pSettings;
}

//***************************************************************************************

void CWFAcquisition::CheckAlignment()
{
  CImage* pImage = &m_WFExam.m_Image[0];

  pImage->m_w = CHW::m_VideoWidth;
  pImage->m_h = CHW::m_VideoHeight;
  pImage->m_w_um = m_pHW->m_Calibration.VideoWidthMicrons;
  pImage->m_h_um = m_pHW->m_Calibration.VideoHeightMicrons;

  pImage->m_RGBData.Attach(CHW::m_VideoHeight, CHW::m_VideoLineWidth, m_pHW->GetRGBData());

  pImage->ClearPupil();
  pImage->FindPupil();

  if (pImage->m_pu_ok && (sqr(pImage->m_pu_x0_um) + sqr(pImage->m_pu_y0_um) < sqr(75.0))) {
    m_auto_r_max_um = 50.0 * (int)(pImage->m_pu_r_min_um * 0.02) - 300.0;
    if (m_auto_r_max_um < SCAN_R_MIN_UM || m_auto_r_max_um > SCAN_R_MAX_UM) {
      m_auto_r_max_um = 0.0;
    }
  }
  else {
    m_auto_r_max_um = 0.0;
  }

  m_AlignmentStatus = m_auto_r_max_um > 0.001 ? 1 : 0;
}

//***************************************************************************************

void CWFAcquisition::Main()
{
  m_WFExam.m_Calibration = m_pHW->m_Calibration;

  m_pHW->CheckLaserSafetyStatus();

  m_pHW->ClearFrames();

  m_WFExam.m_PrecalcSphere = 0.0;
  m_WFExam.m_PrecalcCylinder = 0.0;
  m_WFExam.m_PrecalcAxis = 0;

  m_pHW->m_pCurrentVideoSettings = &m_pHW->m_Calibration.WFVideoSettings;
  m_pHW->ApplyCurrentVideoBrightness();
  m_pHW->ApplyCurrentVideoContrast();
  m_pHW->ApplyCurrentVideoHue();
  m_pHW->ApplyCurrentVideoSaturation();
  m_pHW->TurnInfraredLEDsOn();
  m_pHW->TurnWhiteLEDsOn();
  m_TargetLightOn = TRUE;
  m_pHW->TurnAccommodationTargetOn();
  m_pHW->TurnVideoCameraOn();
  ::Sleep(600);

  m_pHW->InitializeOptometer(); // this is done once
  m_pHW->MoveOptometerTargetToPositionD(10.0, TRUE);

  m_Eye = m_pHW->GetEye();

  int Delay;
  real r_max_um;
  uint AlignmentCounter;
  uint ODOSCounter;
  uint Done;
  long StartTime;
  const long TimeOut = 60000;

  //------------------------------------------------------------------------
  m_Probing = TRUE;
  AlignmentCounter = 0;
  ODOSCounter = 0;
  Done = 0;
  //m_auto_r_max_um = 0.0;

  if (m_pSettings->m_ProbingEnabled) {
    BOOL ProbingResult = FALSE;
    StartTime = clock();
    while (Done == 0) {
      // check OD/OS once a second
      if (++ODOSCounter == 30) {
        m_Eye = m_pHW->GetEye();
        ODOSCounter = 0;
      }
      // start transferring video frame
      m_pHW->StartTransferringVideoFrame();
      // check eye alignment on already received video frame
      CheckAlignment();
      // update live video window
      m_pVideoWnd->SendMessage(WM_THREAD_UPDATE, 0, 0);
      // finalize transfer
      m_pHW->FinishTransferringVideoFrame();
      //
      long Duration = clock() - StartTime;
      //
      if (m_AlignmentStatus == 1) {
        AlignmentCounter++;
      }
      else {
        AlignmentCounter = 0;
      }
      // timed out
      if (Duration >= TimeOut) {
        Done = 1;
      }
      // automatic capture
      else if (AlignmentCounter == 3) {
        Done = 2;
      }
      // manual capture
      else if (m_ToDo == ACQUIRE) {
        Done = 3;
      }
      // interrupted by user
      else if (m_ToDo == ABORT) {
        Done = 1;
      }
      // turn on/off target illumination
      else if (m_ToDo == TARGET_LIGHT) {
        if (m_TargetLightOn) {
          m_pHW->TurnAccommodationTargetOn();
        }
        else {
          m_pHW->TurnAccommodationTargetOff();
        }
      }
      // flush todo
      m_ToDo = DO_NOTHING;
      //
      ::Sleep(0);
    } // while (done == 0)
    //
    m_pHW->TurnInfraredLEDsOff();
    m_pHW->TurnWhiteLEDsOff();
    //
    if (Done == 2 || Done == 3) {
      //
      if (m_pHW->IsConnected()) { // это нужно только для симуляции если не подсоединено железо
      // create probing pattern
        r_max_um = 1000.0;
        m_WFExam.CreateProbePattern(r_max_um);
        // setup scan points and scan
        if (SetupScanPointsAndScan()) {
          // download scan data into PC
          if (DownloadScanResults()) {
            m_WFExam.ProcessPoints(TRUE);
            int NumPoints = m_WFExam.m_Points.GetSize();
            real* r_um = (real*)malloc(NumPoints * sizeof(real));
            real* a_rd = (real*)malloc(NumPoints * sizeof(real));
            real* dw_dx = (real*)malloc(NumPoints * sizeof(real));
            real* dw_dy = (real*)malloc(NumPoints * sizeof(real));
            int k = 0;
            CScanPoint* pPoint = m_WFExam.m_Points.GetFirst();
            while (pPoint) {
              if (!pPoint->m_AutoBad) {
                r_um[k] = pPoint->m_r_um;
                a_rd[k] = pPoint->m_a_rd;
                dw_dx[k] = - pPoint->m_dx_um / m_WFExam.m_Calibration.LADXFocalDistance;
                dw_dy[k] =   pPoint->m_dy_um / m_WFExam.m_Calibration.LADYFocalDistance;
                k++;
              }
              pPoint = m_WFExam.m_Points.GetNext();
            }
            // create Zernike surface
            CZernikeSurface Surface;
            BOOL SurfaceCreated = Surface.Create(r_max_um, 6, k, r_um, a_rd, dw_dx, dw_dy);
            free(r_um);
            free(a_rd);
            free(dw_dx);
            free(dw_dy);
            if (SurfaceCreated) {
              // subtract cal surface
              CZernikeSurface CalSurface;
              m_WFExam.CreateCalSurface(CalSurface);
              Surface -= CalSurface;
              // calculate sph/cyl
              real SphEq, Sph, Cyl;
              int Axis;
              CWFExam::GetSpheqSphCylAxis(Surface, 0, TRUE, SphEq, Sph, Cyl, Axis);
              //
              m_WFExam.m_PrecalcSphere = SphEq - 0.5 * Cyl;
              m_WFExam.m_PrecalcCylinder = Cyl;
              m_WFExam.m_PrecalcAxis = Axis;
              // move target to calculate sph/cyl
              m_pHW->MoveOptometerTargetToPositionD(SphEq + 0.5 * Cyl, TRUE);
              // fog if necessary
              if (m_pSettings->m_FoggingEnabled) {
                ::Sleep(1000);
                m_pHW->MoveOptometerTargetToPositionD(SphEq + 0.5 * Cyl + 1.0, TRUE);
              }
              // probing completed successfully
              ProbingResult = TRUE;
            } // if (SurfaceCreated)
          } // if (DownloadScanResults())
        } // if (SetupScanPointsAndScan())
      } // if (m_pHW->IsConnected())
      // simulation mode when equipment is not connected
      else {
        m_WFExam.m_PrecalcSphere = 0.0;
        m_WFExam.m_PrecalcCylinder = 0.0;
        m_WFExam.m_PrecalcAxis = 0;
        // probing completed successfully
        ProbingResult = TRUE;
      }
    } // if (Done == 2 || Done == 3)
    if (!ProbingResult) {
      m_pHW->TurnAccommodationTargetOff();
      m_pVideoWnd->PostMessage(WM_THREAD_FINISH, 0, 0);
      return;
    } // if (!ProbingResult)
    Delay = 5000;
    //
    m_pHW->TurnInfraredLEDsOn();
    m_pHW->TurnWhiteLEDsOn();
  } // if (m_pSettings->m_ProbingEnabled)
  else {
    Delay = 0;
  }
  //------------------------------------------------------------------------

  m_Probing = FALSE;
  m_AutoScanSize = TRUE;
  AlignmentCounter = 0;
  ODOSCounter = 0;
  Done = 0;
  //m_auto_r_max_um = 0.0;
  real man_r_max_um = round(0.5 * m_pSettings->m_ManualScanDiameterUm, 50.0);
  if (man_r_max_um < SCAN_R_MIN_UM) {
    man_r_max_um = SCAN_R_MIN_UM;
  }
  else if (man_r_max_um > SCAN_R_MAX_UM) {
    man_r_max_um = SCAN_R_MAX_UM;
  }
  //
  m_NumFramesReceived = 0;
  m_AverageCycleTime = 0.0;
  StartTime = clock();

  while (Done == 0) {
    // check OD/OS once a second
    if (++ODOSCounter == 30) {
      m_Eye = m_pHW->GetEye();
      ODOSCounter = 0;
    }
    // start transferring video frame
    m_pHW->StartTransferringVideoFrame();
    // check eye alignment on already received video frame
    CheckAlignment();
    // update live video window
    m_pVideoWnd->SendMessage(WM_THREAD_UPDATE, 0, 0);
    // finalize transfer
    m_pHW->FinishTransferringVideoFrame();
    //
    long Duration = clock() - StartTime;
    //
    if (m_AlignmentStatus == 1) {
      if (Duration > Delay) {
        if (m_AutoScanSize) {
          AlignmentCounter++;
        }
        else {
          if (man_r_max_um <= m_auto_r_max_um) {
            AlignmentCounter++;
          }
          else {
            AlignmentCounter = 0;
          }
        }
      }
    }
    else {
      AlignmentCounter = 0;
    }
    // timed out
    if (Duration >= TimeOut) {
      Done = 1;
    }
    // automatic capture
    else if (AlignmentCounter == 3) {
      Done = 2;
    }
    // manual capture with spacebar
    else if (!m_AutoScanSize && m_ToDo == ACQUIRE) {
      Done = 3;
    }
    // manual capture with pushbutton
    else if (!m_AutoScanSize && m_pHW->IsButtonPressed()) {
      Done = 3;
    }
    // interrupted by user
    else if (m_ToDo == ABORT) {
      Done = 1;
    }
    // change auto/manual mode
    else if (m_ToDo == CHANGE_MODE) {
      m_AutoScanSize = !m_AutoScanSize;
      if (!m_AutoScanSize) {
        m_WFExam.CreateScanPattern(2, man_r_max_um);
      }
    }
    // increase manual scan size
    else if (m_ToDo == INC_SCAN_SIZE) {
      if (!m_AutoScanSize && man_r_max_um < SCAN_R_MAX_UM - 50.0 + 0.001) {
        man_r_max_um += 50.0;
        m_WFExam.CreateScanPattern(2, man_r_max_um);
      }
    }
    // decrease manual scan size
    else if (m_ToDo == DEC_SCAN_SIZE) {
      if (!m_AutoScanSize && man_r_max_um > SCAN_R_MIN_UM + 50.0 - 0.001) {
        man_r_max_um -= 50.0;
        m_WFExam.CreateScanPattern(2, man_r_max_um);
      }
    }
    // move target upwards
    else if (m_ToDo == TARGET_UP) {
      m_pHW->MoveOptometerTargetToPositionD(m_pHW->GetOptometerTargetPositionD() + 0.25, FALSE);
    }
    // move target downwards
    else if (m_ToDo == TARGET_DN) {
      m_pHW->MoveOptometerTargetToPositionD(m_pHW->GetOptometerTargetPositionD() - 0.25, FALSE);
    }
    // move target to zero
    else if (m_ToDo == TARGET_HOME) {
      m_pHW->MoveOptometerTargetToPositionD(0.0, FALSE);
    }
    // turn on/off target illumination
    else if (m_ToDo == TARGET_LIGHT) {
      if (m_TargetLightOn) {
        m_pHW->TurnAccommodationTargetOn();
      }
      else {
        m_pHW->TurnAccommodationTargetOff();
      }
    }
    // flush todo
    m_ToDo = DO_NOTHING;
    //
    m_AverageCycleTime = (real)(clock() - StartTime) / (++m_NumFramesReceived);
    //
    ::Sleep(0);
  }
  //
  m_pHW->TurnAccommodationTargetOff();
  m_pHW->TurnInfraredLEDsOff();
  m_pHW->TurnWhiteLEDsOff();
  //
  if (Done == 1) {
    m_pHW->TurnVideoCameraOff();
    m_pVideoWnd->PostMessage(WM_THREAD_FINISH, 0, 0);
    return;
  }

  r_max_um = m_AutoScanSize ? m_auto_r_max_um : man_r_max_um;

  //m_WFExam.m_Header.m_PatientID     = позже
  m_WFExam.m_Header.m_CalibrationID   = m_WFExam.m_Calibration.CalibrationID;
  m_WFExam.m_Header.m_ClinicID        = GUID_NULL;
  m_WFExam.m_Header.m_PhysicianID     = GUID_NULL;
  m_WFExam.m_Header.m_OperatorID      = GUID_NULL;
  m_WFExam.m_Header.m_Type            = EXAM_TYPE_WF;
  m_WFExam.m_Header.m_Mode            = Done == 2 ? EXAM_MODE_AUTO : EXAM_MODE_MANUAL;
  SYSTEMTIME Time;
  ::GetLocalTime(&Time);
  m_WFExam.m_Header.m_Year            = Time.wYear;
  m_WFExam.m_Header.m_Month           = Time.wMonth;
  m_WFExam.m_Header.m_Day             = Time.wDay;
  m_WFExam.m_Header.m_Hour            = Time.wHour;
  m_WFExam.m_Header.m_Minute          = Time.wMinute;
  m_WFExam.m_Header.m_Second          = Time.wSecond;
  m_WFExam.m_Header.m_Eye             = m_Eye;
  m_WFExam.m_Header.m_Preop           = PREOP_UNKNOWN;
  m_WFExam.m_Header.m_SeriesNumber    = 0;
  m_WFExam.m_Header.m_SoftwareVersion = SOFTWARE_VERSION;
  m_WFExam.m_Header.m_Note            = "";
  m_WFExam.m_Header.m_Saved           = FALSE;

  m_WFExam.m_ScanDiameter             = 2.0 * r_max_um;
  m_WFExam.m_AccommTargetPosition     = m_pHW->GetOptometerTargetPositionD();
  //m_WFExam.m_PrecalcSphere          = уже
  //m_WFExam.m_PrecalcCylinder        = уже
  //m_WFExam.m_PrecalcAxis            = уже
  m_WFExam.m_WavetouchLensPower       = 0.0;
  m_WFExam.m_WavetouchLensBaseCurve   = WAVETOUCH_LENS_BASECURVE_UNKNOWN;
  m_WFExam.m_AODsOK                   = TRUE;

  m_WFExam.m_Image[0].Destroy();
  m_WFExam.m_Image[1].Destroy();

  BOOL Result;

  if (m_pHW->IsConnected()) { // это нужно только для симуляции если не подсоединено железо
    //
    m_WFExam.CreateScanPattern(2, r_max_um);
    //
    Result = SetupScanPointsAndScan();
    if (Result) {
      //m_WFExam.m_Image[0].m_pu_ok = FALSE;
      m_WFExam.m_Image[0].m_w = CHW::m_VideoWidth;
      m_WFExam.m_Image[0].m_h = CHW::m_VideoHeight;
      m_WFExam.m_Image[0].m_RGBData.Create(CHW::m_VideoHeight, CHW::m_VideoLineWidth, m_pHW->GetRGBData());
      //
      m_WFExam.m_Image[0].m_JPGData.Destroy();
      m_WFExam.m_Image[0].CreateJPGFromRGB();
      m_WFExam.m_Image[0].m_RGBData.Destroy();
      m_WFExam.m_Image[0].CreateRGBFromJPG();
      //
      if (::Licensing.IsCustomvis()) {
        //
        m_pHW->m_pCurrentVideoSettings = &m_pHW->m_Calibration.WFVideo2Settings;
        m_pHW->ApplyCurrentVideoBrightness();
        m_pHW->ApplyCurrentVideoContrast();
        m_pHW->ApplyCurrentVideoHue();
        m_pHW->ApplyCurrentVideoSaturation();
        m_pHW->TurnInfraredLEDsOn();
        m_pHW->TurnWhiteLEDsOn();
        //
        ulong StartTime = clock();
        while (clock() - StartTime < (ulong)m_pHW->m_Calibration.ColorImageDelay) {
          m_pHW->StartTransferringVideoFrame();
          m_pVideoWnd->SendMessage(WM_THREAD_UPDATE, 0, 0);
          m_pHW->FinishTransferringVideoFrame();
        }
        //
        //m_WFExam.m_Image[1].m_pu_ok = FALSE;
        m_WFExam.m_Image[1].m_w = CHW::m_VideoWidth;
        m_WFExam.m_Image[1].m_h = CHW::m_VideoHeight;
        m_WFExam.m_Image[1].m_RGBData.Create(CHW::m_VideoHeight, CHW::m_VideoLineWidth, m_pHW->GetRGBData());
        //
        m_WFExam.m_Image[1].m_JPGData.Destroy();
        m_WFExam.m_Image[1].CreateJPGFromRGB();
        m_WFExam.m_Image[1].m_RGBData.Destroy();
        m_WFExam.m_Image[1].CreateRGBFromJPG();
        //
        m_pHW->TurnInfraredLEDsOff();
        m_pHW->TurnWhiteLEDsOff();
      }
    }

    m_pHW->TurnVideoCameraOff();

    if (Result) {
      Result = DownloadScanResults();
    }
  }

  else {
    // create a Zernike surface with specified coefficients
    CSimulationDlg* pDlg = new CSimulationDlg(m_pVideoWnd);
    pDlg->m_Surface.m_r_max_um = r_max_um;
    pDlg->m_Surface.Zero();
    pDlg->m_Eye = m_Eye;
    if (pDlg->DoModal() == IDOK) {
      //r_max_um = pDlg->m_Surface.m_r_max_um - 500.0;
      r_max_um = pDlg->m_Surface.m_r_max_um;
      m_WFExam.m_ScanDiameter = 2.0 * r_max_um;
      m_WFExam.CreateScanPattern(2, r_max_um);
      m_WFExam.m_Header.m_Eye = pDlg->m_Eye;
      // create a set of deltas
      m_WFExam.m_DetectorType = DT_QUADRANT;
      CScanPoint* pPoint = m_WFExam.m_Points.GetFirst();
      while (pPoint) {
        real dw_dx, dw_dy;
        /*
        real x_um = pPoint->m_r_um * cos(pPoint->m_a_rd);
        real y_um = pPoint->m_r_um * sin(pPoint->m_a_rd) + 500.0;
        real r_um = hyp(x_um, y_um);
        real a_rd = angle(y_um, x_um);
        pDlg->m_Surface.GetAt(r_um, a_rd, NULL, &dw_dx, &dw_dy);
        */
        pDlg->m_Surface.GetAt(pPoint->m_r_um, pPoint->m_a_rd, NULL, &dw_dx, &dw_dy);
        pPoint->m_dx_um = - dw_dx * m_WFExam.m_Calibration.LADXFocalDistance;
        pPoint->m_dy_um =   dw_dy * m_WFExam.m_Calibration.LADYFocalDistance;
        pPoint = m_WFExam.m_Points.GetNext();
      }
      Result = TRUE;

    }
    else {
      Result = FALSE;
    }
    delete pDlg;
  }

  m_pVideoWnd->PostMessage(WM_THREAD_FINISH, Result ? 1 : 0, 0);
}

//***************************************************************************************

BOOL CWFAcquisition::SetupScanPointsAndScan()
{
  int NumPoints = m_WFExam.m_Points.GetSize();

  real* x_um = (real*)malloc(NumPoints * sizeof(real));
  real* y_um = (real*)malloc(NumPoints * sizeof(real));

  CScanPoint* pPoint = m_WFExam.m_Points.GetFirst();
  for (int p = 0; p < NumPoints; p++) {
    x_um[p] = pPoint->x_um();
    y_um[p] = pPoint->y_um();
    pPoint = m_WFExam.m_Points.GetNext();
  }

  BOOL Result = m_pHW->DoWFExam(NumPoints, x_um, y_um);

  free(x_um);
  free(y_um);

  return Result;
}

//***************************************************************************************

BOOL CWFAcquisition::DownloadScanResults()
{
  int NumPoints = m_WFExam.m_Points.GetSize();

  ushort* pArrayX = (ushort*)malloc(NumPoints * 512 * sizeof(ushort));
  ushort* pArrayY = (ushort*)malloc(NumPoints * 512 * sizeof(ushort));

  BOOL Result = m_pHW->GetLADsData(NumPoints, pArrayX, pArrayY);

  m_WFExam.m_DetectorType = DT_LINEAR_ARRAYS;
  CScanPoint* pPoint = m_WFExam.m_Points.GetFirst();
  for (int p = 0; p < NumPoints; p++) {
    memcpy(pPoint->m_LADX.m_Signal, pArrayX + p * 512, 1024);
    memcpy(pPoint->m_LADY.m_Signal, pArrayY + p * 512, 1024);
    pPoint = m_WFExam.m_Points.GetNext();
  }

  free(pArrayX);
  free(pArrayY);

  return Result;
}

//***************************************************************************************
