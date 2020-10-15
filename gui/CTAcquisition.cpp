//***************************************************************************************

#include "StdAfx.h"
#include "Settings.h"
#include "Messages.h"
#include "Video3ToolDlg.h"

#include "CTAcquisition.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

CCTAcquisition::CCTAcquisition(CHW* pHW, BOOL ShowRedDot) : CAcquisition(pHW)
{
	m_AlignmentStatus = 0;
	m_TriLaserShutoffOn = TRUE;
	m_TriLaserOn = FALSE;
	m_ve0_ok = FALSE;
	m_la_ok = FALSE;
	m_TriangulationSensorReading = 0;
	m_ShowRedDot = ShowRedDot;

	m_ChangeAlMode = FALSE;
}

//***************************************************************************************

void CCTAcquisition::CheckAlignment()
{
	m_CTExam.m_Image.m_RGBData.Attach(m_CTExam.m_Image.m_h, LINE_SIZE(m_CTExam.m_Image.m_w), m_pHW->GetRGBData());
	m_CTExam.m_Image.ClearVertex0();
	m_CTExam.m_Image.FindVertex0(m_TriLaserOn, m_pHW->m_Calibration.LaserIntensityThreshold);

	m_ve0_ok = m_CTExam.m_Image.m_ve0_ok && (sqr(m_CTExam.m_Image.m_ve0_x) + sqr(m_CTExam.m_Image.m_ve0_y) <= sqr(5));

	if (m_pHW->m_Calibration.ZAlignmentMethod == 0)
	{
		m_la_ok = m_CTExam.m_Image.m_la_ok && abs(m_CTExam.m_Image.m_la_x) <= 5 && m_CTExam.m_Image.m_la_RtoC_OK;
		m_AlignmentStatus = m_ve0_ok && m_la_ok ? 1 : 0;
	}
	else
	{
		m_AlignmentStatus = m_ve0_ok && (m_TriangulationSensorReading > m_pHW->m_Calibration.SensorThreshold) ? 1 : 0;
	}
}

//***************************************************************************************

void CCTAcquisition::Main()
{
	::NewSettings.m_Adjusting_CT = FALSE;

	if (m_pHW->m_Calibration.LaserIntensityThreshold <= 0 || m_pHW->m_Calibration.LaserIntensityThreshold > 255)
	{
		m_pHW->m_Calibration.LaserIntensityThreshold = 160;
	}

	if (m_pHW->m_Calibration.SensorThreshold < 0 || m_pHW->m_Calibration.SensorThreshold > 65535)
	{
		m_pHW->m_Calibration.SensorThreshold = 30000;
	}

	if (m_pHW->m_Calibration.LaserDuration <= 0 || m_pHW->m_Calibration.LaserDuration > 600)
	{
		m_pHW->m_Calibration.LaserDuration = 30;
	}

	m_CTExam.m_Calibration = m_pHW->m_Calibration;

	m_CTExam.m_Image.Destroy();
	m_CTExam.m_ColorImage.Destroy();

	m_CTExam.m_Image.m_w = m_CTExam.m_Calibration.VideoWidth;
	m_CTExam.m_Image.m_h = m_CTExam.m_Calibration.VideoHeight;
	m_CTExam.m_Image.m_w_um = m_CTExam.m_Calibration.VideoWidthMicrons;
	m_CTExam.m_Image.m_h_um = m_CTExam.m_Calibration.VideoHeightMicrons;
	m_CTExam.m_Image.m_NumRings = m_CTExam.m_Calibration.NumRings;

	m_pHW->ClearFrames();

	m_pHW->m_pCurrentVideoSettings = &m_pHW->m_Calibration.CTVideoSettings;
	m_pHW->ApplyCurrentVideoBrightness();
	m_pHW->ApplyCurrentVideoContrast();
	m_pHW->ApplyCurrentVideoHue();
	m_pHW->ApplyCurrentVideoSaturation();

	m_pHW->TurnPlacidoOn();
	m_pHW->TurnAccommodationTargetOn();

	if (!::HW.ISVideoCameraOn)
	{
		m_pHW->TurnVideoCameraOn();
		::HW.ISVideoCameraOn = true;
	}

	m_TriLaserOn = FALSE;
	m_AlignmentStatus = 0;
	m_ve0_ok = FALSE;
	m_la_ok = FALSE;
	m_TriangulationSensorReading = 0;

	m_CTExam.m_Image.ClearVertex0();

	m_CTAutoMode = TRUE;
	uint Done = 0;
	uint ODOSCounter = 0;
	m_Eye = m_pHW->GetEye();

	int TimeLimit = 1050 * m_pHW->m_Calibration.LaserDuration;
	int StartTime = (int)clock();
	m_TimeLeft = TimeLimit;
	m_NumFramesReceived = 0;
	m_AverageCycleTime = 0.0;

	if (m_pHW->m_Calibration.ZAlignmentMethod == 1)
	{
		m_pHW->TurnTriLaserOn();
		m_TriLaserOn = TRUE;
	}

	m_CTExam.m_Image.m_la_Ok_Times = 0;
	m_CTExam.m_Image.m_la_RtoC_OK = 0;

	//Acquisition loop
	while (Done == 0)
	{
		if (m_pHW->m_Calibration.ZAlignmentMethod == 1)
		{
			m_TriangulationSensorReading = m_pHW->GetTriangulationSensorReading();
		}

		if (++ODOSCounter == 30)
		{
			m_Eye = m_pHW->GetEye();
			ODOSCounter = 0;
		}

		m_pHW->StartTransferringVideoFrame();


		if (m_NumFramesReceived > 20)
		{
			if (m_NumFramesReceived > 30)
			{
				CheckAlignment();
			}
			m_pVideoWnd->SendMessage(WM_THREAD_UPDATE, 0, 0);
		}

		m_ChangeAlMode = FALSE;

		m_pHW->FinishTransferringVideoFrame();

		m_TimeLeft = TimeLimit - ((int)clock() - StartTime);

		if ((m_TriLaserShutoffOn && (m_TimeLeft <= 0)) || (m_ToDo == ABORT))
		{
			Done = 1;
		}
		else if (m_CTAutoMode && m_AlignmentStatus == 1)
		{
			Done = 2;
		}
		else if (!m_CTAutoMode && (m_ToDo == ACQUIRE || m_pHW->IsButtonPressed()))
		{
			Done = 3;
		}
		else if (m_ToDo == CHANGE_MODE)
		{
			m_CTAutoMode = !m_CTAutoMode;
			m_ChangeAlMode = TRUE;
		}
		else if (m_ToDo == CHANGE_BRIGHTNESS)
		{
			m_pHW->ApplyCurrentVideoBrightness();
		}
		else if (m_ToDo == CHANGE_CONTRAST)
		{
			m_pHW->ApplyCurrentVideoContrast();
		}

		m_ToDo = DO_NOTHING;

		if (m_pHW->m_Calibration.ZAlignmentMethod == 0)
		{
			if (m_ve0_ok)
			{
				if (!m_TriLaserOn)
				{
					m_pHW->TurnTriLaserOn();
					m_TriLaserOn = TRUE;
				}
			}
		}

		m_AverageCycleTime = (real_t)(clock() - StartTime) / (++m_NumFramesReceived);

		::Sleep(0);
	}
	//Acquisition loop Done

	if (m_TriLaserOn)
	{
		m_pHW->TurnTriLaserOff();
		m_TriLaserOn = FALSE;
	}

	if (Done == 2 || Done == 3)
	{
		for (int i = 0; i < 4; i++)
		{
			m_pHW->StartTransferringVideoFrame();
			m_pHW->FinishTransferringVideoFrame();
		}
	}


	if (Done == 1)
	{
		m_pHW->TurnPlacidoOff();
		m_pHW->TurnAccommodationTargetOff();
		m_pVideoWnd->PostMessage(WM_THREAD_FINISH, 0, 0);
		return;
	}

	::CoCreateGuid(&m_CTExam.m_Header.m_ExamID);
	m_CTExam.m_Header.m_PatientID = GUID_NULL; // later
	m_CTExam.m_Header.m_CalibrationID = m_CTExam.m_Calibration.CalibrationID;
	m_CTExam.m_Header.m_ClinicID = GUID_NULL; // later
	m_CTExam.m_Header.m_PhysicianID = GUID_NULL; // later
	m_CTExam.m_Header.m_OperatorID = GUID_NULL; // later
	m_CTExam.m_Header.m_FellowExamID = GUID_NULL; // later
	m_CTExam.m_Header.m_Type = EXAM_TYPE_CT;
	m_CTExam.m_Header.m_Mode = Done == 2 ? EXAM_MODE_AUTO : EXAM_MODE_MANUAL;

	SYSTEMTIME Time;
	::GetLocalTime(&Time);
	m_CTExam.m_Header.m_Year = Time.wYear;
	m_CTExam.m_Header.m_Month = Time.wMonth;
	m_CTExam.m_Header.m_Day = Time.wDay;
	m_CTExam.m_Header.m_Hour = Time.wHour;
	m_CTExam.m_Header.m_Minute = Time.wMinute;
	m_CTExam.m_Header.m_Second = Time.wSecond;
	m_CTExam.m_Header.m_Eye = m_Eye;
	m_CTExam.m_Header.m_Preop = PREOP_UNKNOWN;
	m_CTExam.m_Header.m_SeriesNumber = 0;
	m_CTExam.m_Header.m_SoftwareVersion = SOFTWARE_VERSION;
	m_CTExam.m_Header.m_Note.Empty();
	m_CTExam.m_Header.m_AutoNote.Empty();
	m_CTExam.m_Header.m_Saved = FALSE;

	m_CTExam.m_Image.m_RGBData.Create(m_CTExam.m_Image.m_h, LINE_SIZE(m_CTExam.m_Image.m_w), m_pHW->GetRGBData());

	//color image
	::TempSettings.Temp_ColorImgCpted = FALSE;
	//::NewSettings.m_Adjust_CT is used to rings tool setting   
	if (::Settings.m_Cap_ColorImg && !::NewSettings.m_Adjust_CT)
	{
		TimeLimit = 1050 * m_pHW->m_Calibration.LaserDuration;
		StartTime = (int)clock();

		if (::Settings.m_Cap_ColorAutoOrManu)//automatic
		{
			m_pHW->TurnPlacidoOff();

			m_pHW->m_pCurrentVideoSettings = &m_pHW->m_Calibration.WFVideo2Settings;
			m_pHW->ClearFrames();
			m_pHW->ApplyCurrentVideoBrightness();
			m_pHW->ApplyCurrentVideoContrast();
			m_pHW->ApplyCurrentVideoHue();
			m_pHW->ApplyCurrentVideoSaturation();
			m_pHW->TurnInfraredLEDsOn();
			m_pHW->TurnWhiteLEDsOn();

			ulong StartTime = clock();

			while (clock() - StartTime < (ulong)m_pHW->m_Calibration.ColorImageDelay)
			{
				m_pHW->StartTransferringVideoFrame();
				m_pVideoWnd->SendMessage(WM_THREAD_UPDATE, 0, 2);
				m_pHW->FinishTransferringVideoFrame();
			}

			//adjust the image quality		  		  
			int ST = (int)clock();

			m_BackupBrightness = m_pHW->m_Calibration.WFVideo2Settings.Brightness;

			m_pHW->StartTransferringVideoFrame();
			m_pHW->FinishTransferringVideoFrame();

			CEyeImage TestImage;
			TestImage.m_w = m_CTExam.m_Calibration.VideoWidth;
			TestImage.m_h = m_CTExam.m_Calibration.VideoHeight;
			TestImage.m_w_um = m_CTExam.m_Calibration.VideoWidthMicrons;
			TestImage.m_h_um = m_CTExam.m_Calibration.VideoHeightMicrons;
			TestImage.m_RGBData.Create(m_CTExam.m_ColorImage.m_h, LINE_SIZE(m_CTExam.m_ColorImage.m_w), m_pHW->GetRGBData());
			TestImage.m_RGBData.Attach(m_CTExam.m_Image.m_h, LINE_SIZE(m_CTExam.m_Image.m_w), m_pHW->GetRGBData());

			real_t Total = 0;
			real_t count = (real_t)TestImage.m_w*TestImage.m_h;
			for (int i = 0; i < TestImage.m_w; i++)
			{
				for (int j = 0; j < TestImage.m_h; j++)
				{
					int R = TestImage.GetRAt(i, j);
					int G = TestImage.GetGAt(i, j);
					int B = TestImage.GetBAt(i, j);

					int Gray = intRound(0.3*R + 0.59*G + 0.11*B);
					Total += Gray;
				}
			}

			real_t Brightness = Total / count;

			if (Brightness < 75 || Brightness > 130)
			{
				VIDEO_SETTINGS* pVideoSettings = m_pHW->m_pCurrentVideoSettings;

				real_t ratio = 75 / Brightness;
				if (Brightness < 75)
				{
					int newBright = int(75 * ratio);

					if (newBright > 130) pVideoSettings->Brightness = 130;
					else pVideoSettings->Brightness = newBright;

					/*s.Format(_T("%i"),pVideoSettings->Brightness);
					::Info("Less than 75 : " + s);*/
				}
				else
				{
					int newBright = int(65 / ratio);

					if (newBright < 40) pVideoSettings->Brightness = 40;
					else pVideoSettings->Brightness = newBright;

					/*s.Format(_T("%i"), pVideoSettings->Brightness);
					::Info("Greater than 75 : " + s);*/
				}

				m_pHW->ApplyCurrentVideoBrightness();

				StartTime = (int)clock();

				while (clock() - StartTime < (ulong)m_pHW->m_Calibration.ColorImageDelay)
				{
					m_pHW->StartTransferringVideoFrame();
					m_pVideoWnd->SendMessage(WM_THREAD_UPDATE, 0, 0);
					m_pHW->FinishTransferringVideoFrame();
				}
			}

			int UT = (int)clock() - ST;
			//adjust the image quality	Done

			m_pHW->TurnWhiteLEDsOff();
			m_pHW->TurnInfraredLEDsOff();
			m_pHW->TurnAccommodationTargetOff();

			m_CTExam.m_ColorImage.m_w = m_CTExam.m_Calibration.VideoWidth;
			m_CTExam.m_ColorImage.m_h = m_CTExam.m_Calibration.VideoHeight;
			m_CTExam.m_ColorImage.m_w_um = m_CTExam.m_Calibration.VideoWidthMicrons;
			m_CTExam.m_ColorImage.m_h_um = m_CTExam.m_Calibration.VideoHeightMicrons;
			m_CTExam.m_ColorImage.m_RGBData.Create(m_CTExam.m_ColorImage.m_h, LINE_SIZE(m_CTExam.m_ColorImage.m_w), m_pHW->GetRGBData());
			m_CTExam.m_ColorImage.m_RGBData.Attach(m_CTExam.m_Image.m_h, LINE_SIZE(m_CTExam.m_Image.m_w), m_pHW->GetRGBData());

			::TempSettings.Temp_ColorImgCpted = TRUE;

			if (Brightness < 75 || Brightness > 130)
				m_pHW->m_Calibration.WFVideo2Settings.Brightness = m_BackupBrightness;

			//m_CTExam.m_ColorImage.SaveIntoFile("C://Tracey//0.jpg");
		}
		else //manual
		{
			::NewSettings.m_Adjusting_CT = TRUE;

			m_pHW->TurnPlacidoOff();

			m_BackupWhiteLEDsPowerLevel = m_pHW->m_Calibration.WFVideo2Settings.WhiteLEDsPowerLevel;
			m_BackupBrightness = m_pHW->m_Calibration.WFVideo2Settings.Brightness;
			m_BackupContrast = m_pHW->m_Calibration.WFVideo2Settings.Contrast;
			m_BackupHue = m_pHW->m_Calibration.WFVideo2Settings.Hue;
			m_BackupSaturation = m_pHW->m_Calibration.WFVideo2Settings.Saturation;

			m_pHW->m_pCurrentVideoSettings = &m_pHW->m_Calibration.WFVideo2Settings;
			m_pHW->ClearFrames();
			m_pHW->TurnInfraredLEDsOn();
			m_pHW->TurnWhiteLEDsOn();

			//530
			while (1)
			{
				m_pHW->StartTransferringVideoFrame();
				if (m_NumFramesReceived > 20)
				{
					m_pVideoWnd->SendMessage(WM_THREAD_UPDATE, 0, 1);//Transform the lParam = 1
				}
				m_pHW->FinishTransferringVideoFrame();

				m_pHW->m_pCurrentVideoSettings->WhiteLEDsPowerLevel = m_WhiteLEDsPower;
				m_pHW->m_pCurrentVideoSettings->Brightness = m_Brightness;
				m_pHW->m_pCurrentVideoSettings->Contrast = m_Contrast;
				m_pHW->m_pCurrentVideoSettings->Hue = m_Hue;
				m_pHW->m_pCurrentVideoSettings->Saturation = m_Saturation;

				m_pHW->ApplyCurrentVideoBrightness();
				m_pHW->ApplyCurrentVideoContrast();
				m_pHW->ApplyCurrentVideoHue();
				m_pHW->ApplyCurrentVideoSaturation();

				//Abort
				m_TimeLeft = TimeLimit - ((int)clock() - StartTime);
				if ((m_TriLaserShutoffOn && m_TimeLeft <= 0) || (m_ToDo == ABORT))//Time is finish
				{
					m_pHW->TurnWhiteLEDsOff();
					m_pHW->TurnInfraredLEDsOff();
					m_pHW->TurnPlacidoOff();
					m_pHW->TurnAccommodationTargetOff();

					m_pHW->m_Calibration.WFVideo2Settings.WhiteLEDsPowerLevel = m_BackupWhiteLEDsPowerLevel;
					m_pHW->m_Calibration.WFVideo2Settings.Brightness = m_BackupBrightness;
					m_pHW->m_Calibration.WFVideo2Settings.Contrast = m_BackupContrast;
					m_pHW->m_Calibration.WFVideo2Settings.Hue = m_BackupHue;
					m_pHW->m_Calibration.WFVideo2Settings.Saturation = m_BackupSaturation;

					break;
				}
				//Abort Done

				if ((m_ToDo == ACQUIRE || m_pHW->IsButtonPressed()))
				{
					ulong StartTime = clock();
					while (clock() - StartTime < (ulong)m_pHW->m_Calibration.ColorImageDelay)
					{
						m_pHW->StartTransferringVideoFrame();
						m_pVideoWnd->SendMessage(WM_THREAD_UPDATE, 0, 1);//Transform the lParam = 1
						m_pHW->FinishTransferringVideoFrame();
					}

					m_pHW->TurnWhiteLEDsOff();
					m_pHW->TurnInfraredLEDsOff();


					m_CTExam.m_ColorImage.m_w = m_CTExam.m_Calibration.VideoWidth;
					m_CTExam.m_ColorImage.m_h = m_CTExam.m_Calibration.VideoHeight;
					m_CTExam.m_ColorImage.m_w_um = m_CTExam.m_Calibration.VideoWidthMicrons;
					m_CTExam.m_ColorImage.m_h_um = m_CTExam.m_Calibration.VideoHeightMicrons;
					m_CTExam.m_ColorImage.m_RGBData.Create(m_CTExam.m_ColorImage.m_h, LINE_SIZE(m_CTExam.m_ColorImage.m_w), m_pHW->GetRGBData());
					m_CTExam.m_ColorImage.m_RGBData.Attach(m_CTExam.m_Image.m_h, LINE_SIZE(m_CTExam.m_Image.m_w), m_pHW->GetRGBData());

					::TempSettings.Temp_ColorImgCpted = TRUE;


					m_pHW->m_Calibration.WFVideo2Settings.WhiteLEDsPowerLevel = m_BackupWhiteLEDsPowerLevel;
					m_pHW->m_Calibration.WFVideo2Settings.Brightness = m_BackupBrightness;
					m_pHW->m_Calibration.WFVideo2Settings.Contrast = m_BackupContrast;
					m_pHW->m_Calibration.WFVideo2Settings.Hue = m_BackupHue;
					m_pHW->m_Calibration.WFVideo2Settings.Saturation = m_BackupSaturation;

					break;
				}
			}
		}
		m_pHW->TurnAccommodationTargetOff();
	}
	else
	{
		m_pHW->TurnPlacidoOff();
		m_pHW->TurnAccommodationTargetOff();
	}


	::NewSettings.m_Adjusting_CT = FALSE;
	m_pVideoWnd->PostMessage(WM_THREAD_FINISH, 1, 0);
}

//***************************************************************************************
