//***************************************************************************************

#include "StdAfx.h"
#include "WFAcquisition.h"
#include "SimulationDlg.h"
#include "AppVersion.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

#define SCAN_R_MIN_UM 1000
#define SCAN_R_MAX_UM 4000

//***************************************************************************************

CWFAcquisition::CWFAcquisition(CHW* pHW, CSettings* pSettings, BOOL Combo) : CAcquisition(pHW)
{
	m_pSettings = pSettings;
	m_Combo = Combo;

	m_ss = "-1";

	m_ChangeScanSize = FALSE;
	m_ChangeAlMode = FALSE;
	m_ChangeAlMethod = FALSE;
	m_ChangeIllumi = FALSE;

	m_NumConsecFrames = m_pHW->m_Calibration.NumConsecFrames;
	if (m_NumConsecFrames <= 0 || m_NumConsecFrames >= 20) m_NumConsecFrames = 3;

	m_AllowedDecentError = m_pHW->m_Calibration.AllowedDecentError;
	if (m_AllowedDecentError <= 0 || m_AllowedDecentError >= 2000) m_AllowedDecentError = 300;

	m_AllowedMovement = m_pHW->m_Calibration.AllowedMovement;
	if (m_AllowedMovement <= 0 || m_AllowedMovement >= 1000) m_AllowedMovement = 30;

	m_WFExam.m_Calibration = m_pHW->m_Calibration;

	m_WFExam.m_AlignmentMethod = m_Combo ? ALIGNMENT_4_IR_DOTS : m_pSettings->m_AlignmentMethod;

	m_WFExam.m_PrecalcSphere = 0.0;
	m_WFExam.m_PrecalcCylinder = 0.0;
	m_WFExam.m_PrecalcAxis = 0;

	m_user_r_max_um = RealRound(0.5 * m_pSettings->m_ManualScanDiameterUm, 50.0);
	if (m_user_r_max_um < SCAN_R_MIN_UM)
	{
		m_user_r_max_um = SCAN_R_MIN_UM;
	}
	else if (m_user_r_max_um > SCAN_R_MAX_UM)
	{
		m_user_r_max_um = SCAN_R_MAX_UM;
	}

	m_auto_r_max_um = RealRound(0.5 * m_pSettings->m_AutoScanDiameterUm, 50.0);
	if (m_auto_r_max_um < SCAN_R_MIN_UM)
	{
		m_auto_r_max_um = SCAN_R_MIN_UM;
	}
	else if (m_auto_r_max_um > SCAN_R_MAX_UM)
	{
		m_auto_r_max_um = SCAN_R_MAX_UM;
	}

	m_AlignmentCounter = 0;
}
//***************************************************************************************

BOOL CWFAcquisition::CheckAlignment()
{
	m_WFExam.m_sc_x0_um = 0.0;
	m_WFExam.m_sc_y0_um = 0.0;

	// save previous pupil center coordinates for later comparison
	real_t prv_pu_x0_um;
	real_t prv_pu_y0_um;
	if (m_AlignmentCounter >= 1)
	{
		prv_pu_x0_um = m_WFExam.m_Image.m_pu_x0_um;
		prv_pu_y0_um = m_WFExam.m_Image.m_pu_y0_um;
	}

	m_WFExam.m_Image.m_w = CHW::m_VideoWidth;
	m_WFExam.m_Image.m_h = CHW::m_VideoHeight;
	m_WFExam.m_Image.m_w_um = m_pHW->m_Calibration.VideoWidthMicrons;
	m_WFExam.m_Image.m_h_um = m_pHW->m_Calibration.VideoHeightMicrons;
	m_WFExam.m_Image.m_RGBData.Attach(CHW::m_VideoHeight, CHW::m_VideoLineWidth, m_pHW->GetRGBData());

	m_WFExam.m_Image.FindPupil();

	if (!m_WFExam.m_Image.m_pu_ok)
	{
		m_auto_r_max_um = RealRound(0.5 * m_pSettings->m_AutoScanDiameterUm, 50.0);
		if (m_auto_r_max_um < SCAN_R_MIN_UM)
		{
			m_auto_r_max_um = SCAN_R_MIN_UM;
		}
		else if (m_auto_r_max_um > SCAN_R_MAX_UM)
		{
			m_auto_r_max_um = SCAN_R_MAX_UM;
		}
		m_AlignmentCounter = 0;
		return FALSE;
	}

	m_WFExam.m_Image.FindWFVertex();

	if (m_WFExam.m_AlignmentMethod == ALIGNMENT_4_IR_DOTS)
	{
		if (!m_WFExam.m_Image.m_ve_ok)
		{
			m_auto_r_max_um = RealRound(0.5 * m_pSettings->m_AutoScanDiameterUm, 50.0);
			if (m_auto_r_max_um < SCAN_R_MIN_UM)
			{
				m_auto_r_max_um = SCAN_R_MIN_UM;
			}
			else if (m_auto_r_max_um > SCAN_R_MAX_UM)
			{
				m_auto_r_max_um = SCAN_R_MAX_UM;
			}

			m_AlignmentCounter = 0;
			return FALSE;
		}
	}

	// check pupil centration
	real_t mid_auto_r_max_um;

	if (m_WFExam.m_AlignmentMethod == ALIGNMENT_PUPIL_CENTER)
	{
		if (sqr(m_WFExam.m_Image.m_pu_x0_um) + sqr(m_WFExam.m_Image.m_pu_y0_um) > sqr(m_AllowedDecentError))
		{
			m_auto_r_max_um = RealRound(0.5 * m_pSettings->m_AutoScanDiameterUm, 50.0);

			if (m_auto_r_max_um < SCAN_R_MIN_UM)
			{
				m_auto_r_max_um = SCAN_R_MIN_UM;
			}
			else if (m_auto_r_max_um > SCAN_R_MAX_UM)
			{
				m_auto_r_max_um = SCAN_R_MAX_UM;
			}

			m_AlignmentCounter = 0;
			return FALSE;
		}

		mid_auto_r_max_um = m_WFExam.m_Image.m_pu_r_min_um;
	}
	else // check vertex centration
	{
		if (sqr(m_WFExam.m_Image.m_ve_x_um) + sqr(m_WFExam.m_Image.m_ve_y_um) > sqr(m_AllowedDecentError))
		{
			m_auto_r_max_um = RealRound(0.5 * m_pSettings->m_AutoScanDiameterUm, 50.0);
			if (m_auto_r_max_um < SCAN_R_MIN_UM)
			{
				m_auto_r_max_um = SCAN_R_MIN_UM;
			}
			else if (m_auto_r_max_um > SCAN_R_MAX_UM)
			{
				m_auto_r_max_um = SCAN_R_MAX_UM;
			}

			m_AlignmentCounter = 0;
			return FALSE;
		}

		mid_auto_r_max_um = m_WFExam.m_Image.m_pu_r_min_um -
			hyp(m_WFExam.m_Image.m_ve_x_um - m_WFExam.m_Image.m_pu_x0_um, m_WFExam.m_Image.m_ve_y_um - m_WFExam.m_Image.m_pu_y0_um);
	}

	m_auto_r_max_um = RealRound(0.5 * m_pSettings->m_AutoScanDiameterUm, 50.0);
	if (m_auto_r_max_um < SCAN_R_MIN_UM)
	{
		m_auto_r_max_um = SCAN_R_MIN_UM;
	}
	else if (m_auto_r_max_um > SCAN_R_MAX_UM)
	{
		m_auto_r_max_um = SCAN_R_MAX_UM;
	}

	mid_auto_r_max_um = 50.0 * (int)(0.02 * mid_auto_r_max_um) - 300.0;
	if (mid_auto_r_max_um < m_auto_r_max_um) m_auto_r_max_um = mid_auto_r_max_um;

	if (m_auto_r_max_um < SCAN_R_MIN_UM || m_auto_r_max_um > SCAN_R_MAX_UM)
	{
		m_auto_r_max_um = RealRound(0.5 * m_pSettings->m_AutoScanDiameterUm, 50.0);
		if (m_auto_r_max_um < SCAN_R_MIN_UM)
		{
			m_auto_r_max_um = SCAN_R_MIN_UM;
		}
		else if (m_auto_r_max_um > SCAN_R_MAX_UM)
		{
			m_auto_r_max_um = SCAN_R_MAX_UM;
		}

		m_AlignmentCounter = 0;
		return FALSE;
	}

	if (m_WFAutoMode != 1)
	{
		if (m_auto_r_max_um < m_user_r_max_um)
		{
			m_auto_r_max_um = RealRound(0.5 * m_pSettings->m_AutoScanDiameterUm, 50.0);
			if (m_auto_r_max_um < SCAN_R_MIN_UM)
			{
				m_auto_r_max_um = SCAN_R_MIN_UM;
			}
			else if (m_auto_r_max_um > SCAN_R_MAX_UM)
			{
				m_auto_r_max_um = SCAN_R_MAX_UM;
			}

			m_AlignmentCounter = 0;
			return FALSE;
		}
	}

	// check pupil movement
	if (m_AlignmentCounter >= 1)
	{
		real_t moveMoment = sqr(m_WFExam.m_Image.m_pu_x0_um - prv_pu_x0_um) + sqr(m_WFExam.m_Image.m_pu_y0_um - prv_pu_y0_um);

		if (moveMoment > sqr(m_AllowedMovement))
		{
			m_auto_r_max_um = RealRound(0.5 * m_pSettings->m_AutoScanDiameterUm, 50.0);

			if (m_auto_r_max_um < SCAN_R_MIN_UM)
			{
				m_auto_r_max_um = SCAN_R_MIN_UM;
			}
			else if (m_auto_r_max_um > SCAN_R_MAX_UM)
			{
				m_auto_r_max_um = SCAN_R_MAX_UM;
			}

			m_AlignmentCounter = 0;
			return FALSE;
		}
	}

	if (++m_AlignmentCounter != m_NumConsecFrames) return FALSE;

	if (m_WFExam.m_AlignmentMethod == ALIGNMENT_PUPIL_CENTER)
	{
		m_WFExam.m_sc_x0_um = m_WFExam.m_Image.m_pu_x0_um;
		m_WFExam.m_sc_y0_um = m_WFExam.m_Image.m_pu_y0_um;
	}
	else
	{
		m_WFExam.m_sc_x0_um = m_WFExam.m_Image.m_ve_x_um;
		m_WFExam.m_sc_y0_um = m_WFExam.m_Image.m_ve_y_um;
	}

	return TRUE;
}

//***************************************************************************************

void CWFAcquisition::ChangeIllumination()
{
	m_TargetLightOn = !m_TargetLightOn;

	if (m_TargetLightOn)
	{
		m_pHW->TurnAccommodationTargetOn();
	}
	else
	{
		m_pHW->TurnAccommodationTargetOff();
	}
}

//***************************************************************************************

void CWFAcquisition::ChangeAlignmentMethod()
{
	if (m_Combo) return;

	if (m_WFExam.m_AlignmentMethod == ALIGNMENT_PUPIL_CENTER)
	{
		m_WFExam.m_AlignmentMethod = ALIGNMENT_4_IR_DOTS;
	}
	else
	{
		m_WFExam.m_AlignmentMethod = ALIGNMENT_PUPIL_CENTER;
	}

	m_WFExam.m_Image.ClearPupil();
	m_WFExam.m_Image.ClearVertex();

	m_auto_r_max_um = RealRound(0.5 * m_pSettings->m_AutoScanDiameterUm, 50.0);

	if (m_auto_r_max_um < SCAN_R_MIN_UM)
	{
		m_auto_r_max_um = SCAN_R_MIN_UM;
	}
	else if (m_auto_r_max_um > SCAN_R_MAX_UM)
	{
		m_auto_r_max_um = SCAN_R_MAX_UM;
	}

	m_AlignmentCounter = 0;
}

//***************************************************************************************

void CWFAcquisition::ChangeAlignmentMode()
{
	if (::PresbiaMode == 1)
	{
		m_WFAutoMode = (m_WFAutoMode == 1 ? 2 : 1);

		m_WFExam.m_Image.ClearPupil();
		m_WFExam.m_Image.ClearVertex();

		m_auto_r_max_um = RealRound(0.5 * m_pSettings->m_AutoScanDiameterUm, 50.0);
		if (m_auto_r_max_um < SCAN_R_MIN_UM)
		{
			m_auto_r_max_um = SCAN_R_MIN_UM;
		}
		else if (m_auto_r_max_um > SCAN_R_MAX_UM)
		{
			m_auto_r_max_um = SCAN_R_MAX_UM;
		}

		m_AlignmentCounter = 0;
	}
	else if (::PresbiaMode == 2 || ::PresbiaMode == 3)
	{
		m_WFAutoMode = ::PresbiaMode; // Two Presbia modes
	}
}

//***************************************************************************************

void CWFAcquisition::Main()
{
	int LaserStatu = m_pHW->CheckLaserSafetyStatus();

	//6.2.0 Change the reponse of laser safty fail ( Close software)
	if (LaserStatu == LASERFAIL)
	{
		int a = 0;
		m_pVideoWnd->PostMessage(WM_THREAD_FINISH, 0, 1);
		return;
	}
	//6.2.0 Change the reponse of laser safty fail ( Close software)

	m_pHW->ClearFrames();

	m_pHW->m_pCurrentVideoSettings = &m_pHW->m_Calibration.WFVideoSettings;
	m_pHW->ApplyCurrentVideoSettings();

	m_pHW->TurnInfraredLEDsOn();

	if (!::HW.ISVideoCameraOn)
	{
		m_pHW->TurnVideoCameraOn();
		::HW.ISVideoCameraOn = true;
	}

	if (!m_pSettings->m_AccommTargetLightOff)
	{
		m_TargetLightOn = TRUE;
		m_pHW->TurnAccommodationTargetOn();
	}
	else
	{
		m_TargetLightOn = FALSE;
	}

	::Sleep(600);

	m_pHW->InitializeOptometer();
	m_pHW->MoveOptometerTargetToPositionD(10.0, TRUE);


	//int Ut = (int)clock() - St;

	m_Eye = m_pHW->GetEye();

	int  Delay = 1000;//1 second delay between 1st and 2nd capture
	real_t r_max_um;
	int  ODOSCounter;
	int  Done;
	int  StartTime;
	const int TimeLimit = 60000;
	m_TimeLeft = TimeLimit;

	m_TargetTime = ::Settings.m_AccommTargetLightTime;

	StartTime = (int)clock();//presbia

	m_ChangeToPresbia256 = FALSE;
	m_user_r_max_um = RealRound(0.5 * m_pSettings->m_ManualScanDiameterUm, 50.0);
	if (m_user_r_max_um < SCAN_R_MIN_UM)
	{
		m_user_r_max_um = SCAN_R_MIN_UM;
	}
	else if (m_user_r_max_um > SCAN_R_MAX_UM)
	{
		m_user_r_max_um = SCAN_R_MAX_UM;
	}

	r_max_um = 1000.0;
	m_user_r_max_um2 = 500;

	BOOL Aligned = FALSE;
	m_Probing = TRUE;
	m_WFAutoMode = 1;
	m_AlignmentCounter = 0;
	ODOSCounter = 0;
	Done = 0;

	if (m_pSettings->m_ProbingEnabled)
	{
		BOOL ProbingResult = FALSE;

		int	FindTime = 0;
		while (Done == 0)
		{
			if (m_TargetTime == 0)
			{
				m_ToDo = SWITCH_TARGET_LIGHT;
				m_TargetTime = -1;
			}

			if (::PresbiaMode == 2 || ::PresbiaMode == 3)
			{
				ProbingResult = TRUE;
				Done = 2;

				if (::PresbiaMode == 3) r_max_um = 2000;
				m_WFExam.CreateScanPattern(r_max_um);
				m_user_r_max_um = r_max_um;

				break;
			}

			if (++ODOSCounter == 30)
			{
				m_Eye = m_pHW->GetEye();
				ODOSCounter = 0;
			}

			m_pHW->StartTransferringVideoFrame();

			Aligned = CheckAlignment();

			m_pVideoWnd->SendMessage(WM_THREAD_UPDATE, 0, 0);

			m_pHW->FinishTransferringVideoFrame();

			m_TimeLeft = TimeLimit - ((int)clock() - StartTime);

			m_ChangeAlMode = FALSE;//530
			m_ChangeAlMethod = FALSE;//530
			m_ChangeIllumi = FALSE;//530

			if ((m_TimeLeft <= 0) || (m_ToDo == ABORT))
			{
				Done = 1;
			}

			else if (Aligned && m_WFAutoMode == 1) //only allow the auto mode to get capture here
			{
				if (FindTime == 0)
				{
					FindTime = m_TimeLeft;
				}
				else
				{
					if ((FindTime - m_TimeLeft)>1000)	Done = 2;
				}
			}

			else if (m_WFAutoMode != 1 && (m_ToDo == ACQUIRE || m_pHW->IsButtonPressed()))
			{
				Done = 3;
			}

			else if (m_ToDo == CHANGE_MODE)
			{
				ChangeAlignmentMode();
				m_ChangeAlMode = TRUE;
			}

			else if (m_ToDo == SWITCH_TARGET_LIGHT)
			{
				ChangeIllumination();
				m_ChangeIllumi = TRUE;
			}

			else if (m_ToDo == CHANGE_ALIGNMENT_METHOD)
			{
				ChangeAlignmentMethod();
				m_ChangeAlMethod = TRUE;
			}

			m_ToDo = DO_NOTHING;

			::Sleep(0);
		} //The loop of 'while (Done == 0)' Done

		m_pHW->TurnInfraredLEDsOff();

		if (Done == 2 || Done == 3)
		{
			if (m_pHW->IsConnected())
			{
				// create probing pattern
				r_max_um = 1000.0;

				m_WFExam.CreateProbePattern(r_max_um);

				// setup scan points and scan
				if (SetupScanPointsAndScan())
				{
					// download scan data into PC
					if (DownloadScanResults())
					{
						m_WFExam.ProcessPoints();
						int NumPoints = m_WFExam.m_Points.GetSize();
						real_t* r_um = new real_t[NumPoints];
						real_t* a_rd = new real_t[NumPoints];
						real_t* dw_dx = new real_t[NumPoints];
						real_t* dw_dy = new real_t[NumPoints];
						int k = 0;
						CScanPoint* pPoint = m_WFExam.m_Points.MoveFirst();
						while (pPoint)
						{
							if (!pPoint->m_AutoBad)
							{
								r_um[k] = pPoint->m_r_um;
								a_rd[k] = pPoint->m_a_rd;
								dw_dx[k] = -pPoint->m_dx_um / m_WFExam.m_Calibration.LADXFocalDistance;
								dw_dy[k] = pPoint->m_dy_um / m_WFExam.m_Calibration.LADYFocalDistance;
								k++;
							}
							pPoint = m_WFExam.m_Points.MoveNext();
						}

						// create Zernike surface
						CZernikeSurface Surface;
						BOOL SurfaceCreated = Surface.CreateXY(2, r_max_um, k, r_um, a_rd, dw_dx, dw_dy, NULL);
						delete[] r_um;
						delete[] a_rd;
						delete[] dw_dx;
						delete[] dw_dy;
						if (SurfaceCreated)
						{
							// subtract cal surface
							CZernikeSurface CalSurface;
							m_WFExam.CreateCalSurface(CalSurface);
							Surface -= CalSurface;

							// calculate sph/cyl
							real_t SphEq, Sph, Cyl;
							int Axis;
							CWFExam::GetSpheqSphCylAxis(Surface, 0, TRUE, SphEq, Sph, Cyl, Axis);

							m_WFExam.m_PrecalcSphere = SphEq - 0.5 * Cyl;
							m_WFExam.m_PrecalcCylinder = Cyl;
							m_WFExam.m_PrecalcAxis = Axis;

							// move target to calculate sph/cyl
							m_pHW->MoveOptometerTargetToPositionD(SphEq + 0.5 * Cyl, TRUE);

							// fog if necessary
							if (m_pSettings->m_FoggingEnabled)
							{
								::Sleep(1000);
								m_pHW->MoveOptometerTargetToPositionD(SphEq + 0.5 * Cyl + 1.0, TRUE);
							}

							ProbingResult = TRUE;// probing completed successfully
						} // if (SurfaceCreated)
					} // if (DownloadScanResults())
				} // if (SetupScanPointsAndScan())
			} // if (m_pHW->IsConnected())

			else // simulation mode when equipment is not connected
			{
				m_WFExam.m_PrecalcSphere = 0.0;
				m_WFExam.m_PrecalcCylinder = 0.0;
				m_WFExam.m_PrecalcAxis = 0;

				ProbingResult = TRUE;// probing completed successfully
			}
		} // if (Done == 2 || Done == 3)

		if (!ProbingResult)
		{
			m_pHW->TurnAccommodationTargetOff();
			m_pVideoWnd->PostMessage(WM_THREAD_FINISH, 0, 0);
			return;
		}

		m_pHW->TurnInfraredLEDsOn();

		if (!::HW.ISVideoCameraOn)
		{
			m_pHW->TurnVideoCameraOn();
			::HW.ISVideoCameraOn = true;
		}

		Delay = 1000;//capture Delay here 6.2.0 Change to 2 second
	}
	else
	{
		Delay = 2000;// 3 second delay before capture without auto fixation
	}
	// The loop of 'if (m_pSettings->m_ProbingEnabled)' Done


 	int t = 0;
	if (::NewSettings.m_Super_Exam) t = 2; //Setting of the super Exams code(3 times)

	BOOL Result;

	m_ss = "-1";

	for (int i = 0; i <= t; i++)//Setting of the super Exams code(3 times)
	{
		BOOL Repeate = TRUE;//Remove goto line 11162020

		while (Repeate)//Remove goto line 11162020
		{
			Repeate = FALSE;//Remove goto line 11162020

			Aligned = FALSE;
			m_Probing = FALSE;
			m_WFAutoMode = 1;//

							 //Presbia
			if (::PresbiaMode == 2 || ::PresbiaMode == 3)
			{
				m_WFAutoMode = ::PresbiaMode;
			}
			//Presbia

			m_AlignmentCounter = 0;
			ODOSCounter = 0;
			Done = 0;
			m_TimeLeft = TimeLimit;
			int SeStime = (int)clock();
			StartTime = (int)clock();

			m_NumFramesReceived = 0;
			m_AverageCycleTime = 0.0;

			m_LastNumStr = "";


			while (Done == 0)
			{
				if (m_TargetTime == 0)
				{
					m_ToDo = SWITCH_TARGET_LIGHT;
					m_TargetTime = -1;
				}

				//presbia
				m_ChangeToPresbia256 = FALSE;

				if (::PresbiaMode == 1)
				{
					if (m_WFAutoMode == 3 && r_max_um != m_user_r_max_um2)
					{
						m_WFExam.CreateScanPattern(m_user_r_max_um2);
						r_max_um = m_user_r_max_um2;

						m_ChangeScanSize = true;
					}

					if (m_WFAutoMode == 2 && r_max_um != m_user_r_max_um)
					{
						m_WFExam.CreateScanPattern(m_user_r_max_um);
						r_max_um = m_user_r_max_um;

						m_ChangeScanSize = true;
					}
				}
				//presbia

				//Presbia
				if ((::PresbiaMode == 1 && ::PresbiaModeOrder == 2) ||
					(::PresbiaMode == 1 && ::PresbiaModeOrder == 3))
				{
					r_max_um = 0.0;
					m_WFExam.CreateProbePattern(r_max_um);

					::PresbiaModeOrder = 1;

					//Remove goto line 11162020
					m_ChangeToPresbia256 = FALSE;
					m_user_r_max_um = RealRound(0.5 * m_pSettings->m_ManualScanDiameterUm, 50.0);
					if (m_user_r_max_um < SCAN_R_MIN_UM)
					{
						m_user_r_max_um = SCAN_R_MIN_UM;
					}
					else if (m_user_r_max_um > SCAN_R_MAX_UM)
					{
						m_user_r_max_um = SCAN_R_MAX_UM;
					}

					r_max_um = 1000.0;
					m_user_r_max_um2 = 500;

					BOOL Aligned = FALSE;
					m_Probing = TRUE;
					m_WFAutoMode = 1;
					m_AlignmentCounter = 0;
					ODOSCounter = 0;
					Done = 0;

					Repeate = TRUE;
					break;
					//Remove goto line 11162020
				}

				if (::PresbiaMode == 2 && ::PresbiaModeOrder != 2)
				{
					m_WFExam.CreateScanPattern(r_max_um);

					m_user_r_max_um = r_max_um;
					::PresbiaModeOrder = 2;
				}

				if (::PresbiaMode == 3 && ::PresbiaModeOrder != 3)
				{
					r_max_um = 2000;
					m_WFExam.CreateScanPattern(r_max_um);

					m_user_r_max_um = r_max_um;
					::PresbiaModeOrder = 3;
					m_ChangeToPresbia256 = TRUE;
				}
				//Presbia

				if (++ODOSCounter == 30)
				{
					m_Eye = m_pHW->GetEye();
					ODOSCounter = 0;
				}

				m_pHW->StartTransferringVideoFrame();

				Aligned = CheckAlignment();

				m_pVideoWnd->SendMessage(WM_THREAD_UPDATE, 0, 0);

				m_pHW->FinishTransferringVideoFrame();

				m_TimeLeft = TimeLimit - ((int)clock() - StartTime);

				m_ChangeScanSize = FALSE;
				m_ChangeAlMode = FALSE;
				m_ChangeAlMethod = FALSE;
				m_ChangeIllumi = FALSE;

				if ((m_TimeLeft <= 0) || (m_ToDo == ABORT))
				{
					Done = 1;
				}
				else if (Aligned && ((int)clock() - SeStime) >= Delay && m_WFAutoMode == 1)
				{
					Done = 2;
				}
				else if (m_WFAutoMode != 1 && (m_ToDo == ACQUIRE || m_pHW->IsButtonPressed()))
				{
					Done = 3;
				}
				else if (m_ToDo == CHANGE_MODE)
				{
					ChangeAlignmentMode();
					m_ChangeAlMode = TRUE;//530

					if (m_WFAutoMode != 1)
					{
						m_WFExam.CreateScanPattern(m_user_r_max_um);
					}
					else m_WFExam.CreateScanPattern(m_user_r_max_um);
				}
				else if (m_ToDo == INC_SCAN_SIZE)
				{
					//
					if (::Normal64Mode && m_user_r_max_um2 < 4000 - 50.0 + 0.001)
					{
						m_ChangeScanSize = TRUE;//530
						m_user_r_max_um2 += 50.0;
						m_WFExam.CreateScanPattern(m_user_r_max_um2);
					}
					else
					{
						//Presbia
						if (::PresbiaMode == 2)
						{
						}
						else if (::PresbiaMode == 3)
						{
							if (m_WFAutoMode != 1 && m_user_r_max_um < 2000 - 50.0 + 0.001)
							{
								m_ChangeScanSize = TRUE;
								m_user_r_max_um += 50.0;
								m_WFExam.CreateScanPattern(m_user_r_max_um);
							}
						}
						else
						{
							//Presbia
							if (m_WFAutoMode != 1 && m_user_r_max_um < SCAN_R_MAX_UM - 50.0 + 0.001)
							{
								m_ChangeScanSize = TRUE;
								m_user_r_max_um += 50.0;
								m_WFExam.CreateScanPattern(m_user_r_max_um);
							}
						}
					}
				}
				else if (m_ToDo == DEC_SCAN_SIZE)
				{
					if (::Normal64Mode && m_user_r_max_um2 > 500 + 50.0 - 0.001)
					{
						m_ChangeScanSize = TRUE;
						m_user_r_max_um2 -= 50.0;
						m_WFExam.CreateScanPattern(m_user_r_max_um2);
					}
					else
					{
						//Presbia
						if (::PresbiaMode == 2)
						{
						}
						else if (::PresbiaMode == 3)
						{
							if (m_WFAutoMode != 1 && m_user_r_max_um > 650 + 50.0 - 0.001)
							{
								m_ChangeScanSize = TRUE;
								m_user_r_max_um -= 50.0;
								m_WFExam.CreateScanPattern(m_user_r_max_um);
							}
						}
						else
						{   //Presbia
							if (m_WFAutoMode != 1 && m_user_r_max_um > SCAN_R_MIN_UM + 50.0 - 0.001)
							{
								m_ChangeScanSize = TRUE;
								m_user_r_max_um -= 50.0;
								m_WFExam.CreateScanPattern(m_user_r_max_um);
							}
						}
					}
				}
				else if (m_ToDo == CHANGE_SCAN_SIZE)
				{
					m_ChangeScanSize = TRUE;
					m_user_r_max_um = m_NewScanSize;
					m_WFExam.CreateScanPattern(m_user_r_max_um);
				}
				else if (m_ToDo == MOVE_TARGET_UP)
				{
					m_pHW->MoveOptometerTargetToPositionD(m_pHW->GetOptometerTargetPositionD() + 0.25, FALSE);
				}
				else if (m_ToDo == MOVE_TARGET_DN)
				{
					m_pHW->MoveOptometerTargetToPositionD(m_pHW->GetOptometerTargetPositionD() - 0.25, FALSE);
				}
				else if (m_ToDo == MOVE_TARGET_HOME)
				{
					m_pHW->MoveOptometerTargetToPositionD(0.0, FALSE);
				}
				else if (m_ToDo == SWITCH_TARGET_LIGHT)
				{
					ChangeIllumination();
					m_ChangeIllumi = TRUE;
				}
				else if (m_ToDo == CHANGE_ALIGNMENT_METHOD)
				{
					ChangeAlignmentMethod();
					m_ChangeAlMethod = TRUE;
				}

				m_ToDo = DO_NOTHING;

				m_AverageCycleTime = (real_t)(clock() - StartTime) / (++m_NumFramesReceived);

				::Sleep(0);
			}
		}

		//if (!::NewSettings.m_Super_Exam) //531//cjf1020
		//{
		//	m_pHW->TurnInfraredLEDsOff();
		//	m_pHW->TurnAccommodationTargetOff();
		//}

		if (Done == 1)
		{
			if (::NewSettings.m_Super_Exam)
			{
				m_pHW->TurnInfraredLEDsOff();
				m_pHW->TurnAccommodationTargetOff();
			}

			m_pVideoWnd->PostMessage(WM_THREAD_FINISH, 0, 0);
			r_max_um = 0;
			return;
		}

		if (::Normal64Mode)
		{
			r_max_um = m_user_r_max_um2;
		}
		else
		{
			if (m_WFAutoMode == 1)
			{
				r_max_um = m_auto_r_max_um;
				m_WFExam.CreateScanPattern(r_max_um);
			}
			else
			{
				r_max_um = m_user_r_max_um;
			}
		}

		::CoCreateGuid(&m_WFExam.m_Header.m_ExamID);
		m_WFExam.m_Header.m_PatientID = GUID_NULL; // later
		m_WFExam.m_Header.m_CalibrationID = m_WFExam.m_Calibration.CalibrationID;
		m_WFExam.m_Header.m_ClinicID = GUID_NULL; // later
		m_WFExam.m_Header.m_PhysicianID = GUID_NULL; // later
		m_WFExam.m_Header.m_OperatorID = GUID_NULL; // later
		m_WFExam.m_Header.m_FellowExamID = GUID_NULL; // later
		m_WFExam.m_Header.m_Type = EXAM_TYPE_WF;
		m_WFExam.m_Header.m_Mode = Done == 2 ? EXAM_MODE_AUTO : EXAM_MODE_MANUAL;

		//530 For Presbia
		if (::PresbiaMode == 2) m_WFExam.m_Header.m_Mode = EXAM_MODE_MANUAL_PRESBIA64;
		if (::PresbiaMode == 3) m_WFExam.m_Header.m_Mode = EXAM_MODE_MANUAL_PRESBIA256;
		//530 For Presbia

		SYSTEMTIME Time;
		::GetLocalTime(&Time);
		m_WFExam.m_Header.m_Year = Time.wYear;
		m_WFExam.m_Header.m_Month = Time.wMonth;
		m_WFExam.m_Header.m_Day = Time.wDay;
		m_WFExam.m_Header.m_Hour = Time.wHour;
		m_WFExam.m_Header.m_Minute = Time.wMinute;
		m_WFExam.m_Header.m_Second = Time.wSecond;
		m_WFExam.m_Header.m_Eye = m_Eye;
		m_WFExam.m_Header.m_Preop = PREOP_UNKNOWN;
		m_WFExam.m_Header.m_SeriesNumber = 0;
		m_WFExam.m_Header.m_SoftwareVersion = SOFTWARE_VERSION;
		m_WFExam.m_Header.m_Note.Empty();
		m_WFExam.m_Header.m_AutoNote.Empty();
		m_WFExam.m_Header.m_Saved = FALSE;

		m_WFExam.m_ScanDiameter = 2.0 * r_max_um;
		m_WFExam.m_AccommTargetPosition = m_pHW->GetOptometerTargetPositionD();
		//m_WFExam.m_PrecalcSphere          = ��?
		//m_WFExam.m_PrecalcCylinder        = ��?
		//m_WFExam.m_PrecalcAxis            = ��?
		m_WFExam.m_WavetouchLensPower = INVALID_VALUE;
		m_WFExam.m_WavetouchLensBaseCurve = WAVETOUCH_LENS_BASECURVE_UNKNOWN;
		m_WFExam.m_WavetouchSph = INVALID_VALUE;
		m_WFExam.m_WavetouchCyl = INVALID_VALUE;
		m_WFExam.m_WavetouchAxis = INVALID_VALUE;
		m_WFExam.m_WavetouchVertex = 0.0;
		m_WFExam.m_WavetouchNote = "";
		m_WFExam.m_WavetouchOther = "";
		m_WFExam.m_AODsOK = TRUE;

		m_WFExam.m_Image.Destroy();
		m_WFExam.m_ColorImage.Destroy();



		Result = FALSE;
		//int St = (int)clock();
		if (m_pHW->IsConnected())
		{
			Result = SetupScanPointsAndScan();

			if (Result)
			{
				m_WFExam.m_Image.m_w = m_WFExam.m_Calibration.VideoWidth;
				m_WFExam.m_Image.m_h = m_WFExam.m_Calibration.VideoHeight;
				m_WFExam.m_Image.m_w_um = m_WFExam.m_Calibration.VideoWidthMicrons;
				m_WFExam.m_Image.m_h_um = m_WFExam.m_Calibration.VideoHeightMicrons;
				m_WFExam.m_Image.m_RGBData.Create(m_WFExam.m_Image.m_h, LINE_SIZE(m_WFExam.m_Image.m_w), m_pHW->GetRGBData());

				Result = DownloadScanResults();
			}

			//int Ut = (int)clock() - St;
		}
		else
		{
			// create a Zernike surface with specified coefficients
			CSimulationDlg* pDlg = new CSimulationDlg(m_pVideoWnd);
			pDlg->m_Surface.m_r_max_um = r_max_um;
			pDlg->m_Eye = m_Eye;
			if (Result = (pDlg->DoModal() == IDOK))
			{
				r_max_um = pDlg->m_Surface.m_r_max_um;
				m_WFExam.m_ScanDiameter = 2.0 * r_max_um;
				m_WFExam.CreateScanPattern(r_max_um);
				m_WFExam.m_Header.m_Eye = pDlg->m_Eye;

				// create a set of deltas
				m_WFExam.m_DetectorType = DT_QUADRANT;
				CScanPoint* pPoint = m_WFExam.m_Points.MoveFirst();
				while (pPoint)
				{
					real_t dw_dx, dw_dy;

					pDlg->m_Surface.GetAt(pPoint->m_r_um, pPoint->m_a_rd, NULL, &dw_dx, &dw_dy, NULL, NULL, NULL);
					pPoint->m_dx_um = -dw_dx * m_WFExam.m_Calibration.LADXFocalDistance;
					pPoint->m_dy_um = dw_dy * m_WFExam.m_Calibration.LADYFocalDistance;
					pPoint = m_WFExam.m_Points.MoveNext();
				}
			}
			delete pDlg;
		}

		if (::NewSettings.m_Super_Exam)
		{
			m_mWFExams[i] = m_WFExam;
			if (i != 2)
			{
				m_ss = "3";
				m_pVideoWnd->SendMessage(WM_THREAD_UPDATE, 0, 0);
				Sleep(1000);
				m_ss = "2";
				m_pVideoWnd->SendMessage(WM_THREAD_UPDATE, 0, 0);
				Sleep(1000);
				m_ss = "1";
				m_pVideoWnd->SendMessage(WM_THREAD_UPDATE, 0, 0);
				Sleep(1000);
				m_ss = "0";
				m_pVideoWnd->SendMessage(WM_THREAD_UPDATE, 0, 0);
				Sleep(1000);
				m_ss = "-1";
			}
		}
	}// The loop 'for(int i = 0; i <= t; i++)' is done

	 //Alert user when the scan size is <2.5mm at the time of acquisition
	if (r_max_um < 1250 && !::Normal64Mode)
	{
		::Info("The scan size is less than 2.5mm");
	}
	//Alert user when the scan size is <2.5mm at the time of acquisition Done

	//if (::NewSettings.m_Super_Exam)//cjf1020
	{
		m_pHW->TurnInfraredLEDsOff();
		m_pHW->TurnAccommodationTargetOff();
	}

	m_pVideoWnd->PostMessage(WM_THREAD_FINISH, Result ? 1 : 0, 0);
}

//***************************************************************************************

BOOL CWFAcquisition::SetupScanPointsAndScan()
{
	int NumPoints = m_WFExam.m_Points.GetSize();

	real_t* scan_x_um = new real_t[NumPoints];
	real_t* scan_y_um = new real_t[NumPoints];

	CScanPoint* pPoint = m_WFExam.m_Points.MoveFirst();
	for (int p = 0; p < NumPoints; p++)
	{
		scan_x_um[p] = m_WFExam.m_sc_x0_um + pPoint->x_um();
		scan_y_um[p] = m_WFExam.m_sc_y0_um + pPoint->y_um();
		pPoint = m_WFExam.m_Points.MoveNext();
	}

	BOOL Result = m_pHW->DoWFExam(NumPoints, scan_x_um, scan_y_um);

	delete[] scan_x_um;
	delete[] scan_y_um;

	return Result;
}

//***************************************************************************************

BOOL CWFAcquisition::DownloadScanResults()
{
	int NumPoints = m_WFExam.m_Points.GetSize();

	ushort* pArrayX = new ushort[NumPoints * 512];
	ushort* pArrayY = new ushort[NumPoints * 512];

	BOOL Result = m_pHW->GetLADsData(NumPoints, pArrayX, pArrayY);

	m_WFExam.m_DetectorType = DT_LINEAR_ARRAYS;
	CScanPoint* pPoint = m_WFExam.m_Points.MoveFirst();
	for (int p = 0; p < NumPoints; p++)
	{
		memcpy(pPoint->m_LADX.m_Signal, pArrayX + p * 512, 1024);
		memcpy(pPoint->m_LADY.m_Signal, pArrayY + p * 512, 1024);
		pPoint = m_WFExam.m_Points.MoveNext();
	}

	delete[] pArrayX;
	delete[] pArrayY;

	return Result;
}

//***************************************************************************************
