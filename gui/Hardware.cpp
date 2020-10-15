//***************************************************************************************

#include "StdAfx.h"
#include "Hardware.h"
#include "Compression.h"
#include "CubicSpline.h"

#include "WinUser.h"
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

const int CHW::m_VideoWidth = 624;
const int CHW::m_VideoHeight = 468;
const int CHW::m_VideoLineWidth = 3 * CHW::m_VideoWidth;
const int CHW::m_VideoSize = CHW::m_VideoHeight * CHW::m_VideoLineWidth;

//***************************************************************************************

void ResetCalibration(CALIBRATION* pCalibration)
{
	memset(pCalibration, 0, sizeof(CALIBRATION));

	pCalibration->CalibrationID = GUID_NULL;

	pCalibration->LaserWavelength = 785.0;

	pCalibration->NumPoints = 256;

	pCalibration->AODX.Freq0 = 60000000;
	pCalibration->AODX.HzUm = 2000;
	pCalibration->AODX.RMS = 10;
	pCalibration->AODY.Freq0 = 60000000;
	pCalibration->AODY.HzUm = 2000;
	pCalibration->AODY.RMS = 100;

	pCalibration->LADExposureTime = 1024;
	pCalibration->LADSwapXY = FALSE;
	pCalibration->LADXNoFlip = FALSE;
	pCalibration->LADXFocalDistance = 75000.0;
	pCalibration->LADYNoFlip = FALSE;
	pCalibration->LADYFocalDistance = 75000.0;

	pCalibration->AODXNoFlip = FALSE;
	pCalibration->AODYNoFlip = FALSE;

	pCalibration->VideoFlipX = FALSE;
	pCalibration->VideoFlipY = FALSE;
	pCalibration->VideoWidth = CHW::m_VideoWidth;
	pCalibration->VideoHeight = CHW::m_VideoHeight;
	pCalibration->VideoWidthMicrons = 16000.0;
	pCalibration->VideoHeightMicrons = 12000.0;

	//pCalibration->VideoSettings.Color                     = FALSE;
	//pCalibration->VideoSettings.InfraredLEDsPowerLevel    =  50;
	//pCalibration->VideoSettings.WhiteLEDsPowerLevel       =  50;
	//pCalibration->VideoSettings.Brightness                = 150;
	//pCalibration->VideoSettings.Contrast                  = 255;
	//pCalibration->VideoSettings.Hue                       =   0;
	//pCalibration->VideoSettings.Saturation                = 255;

	pCalibration->WFVideoSettings.Color = FALSE;
	pCalibration->WFVideoSettings.InfraredLEDsPowerLevel = 50;
	pCalibration->WFVideoSettings.WhiteLEDsPowerLevel = 0;
	pCalibration->WFVideoSettings.Brightness = 150;
	pCalibration->WFVideoSettings.Contrast = 255;
	pCalibration->WFVideoSettings.Hue = 0;
	pCalibration->WFVideoSettings.Saturation = 255;

	pCalibration->WFVideo2Settings.Color = TRUE;
	pCalibration->WFVideo2Settings.InfraredLEDsPowerLevel = 0;
	pCalibration->WFVideo2Settings.WhiteLEDsPowerLevel = 50;
	pCalibration->WFVideo2Settings.Brightness = 150;
	pCalibration->WFVideo2Settings.Contrast = 255;
	pCalibration->WFVideo2Settings.Hue = 0;
	pCalibration->WFVideo2Settings.Saturation = 255;

	pCalibration->CTVideoSettings.Color = FALSE;
	pCalibration->CTVideoSettings.InfraredLEDsPowerLevel = 0;
	pCalibration->CTVideoSettings.WhiteLEDsPowerLevel = 0;
	pCalibration->CTVideoSettings.Brightness = 150;
	pCalibration->CTVideoSettings.Contrast = 255;
	pCalibration->CTVideoSettings.Hue = 0;
	pCalibration->CTVideoSettings.Saturation = 255;

	pCalibration->OptometerEnabled = 1;

	for (int i = 0; i <= 20; i++)
	{
		pCalibration->OptometerTable[i] = 0;
	}

	pCalibration->OptometerGap = 0;

	pCalibration->Zernike.C3 = 0.0;
	pCalibration->Zernike.C4 = 0.0;
	pCalibration->Zernike.C5 = 0.0;

	pCalibration->ColorImageDelay = 280;

	pCalibration->AccommodationTargetPowerLevel = 50;
	pCalibration->FarTargetPowerLevel = 50;
	pCalibration->NearTargetPowerLevel = 50;

	pCalibration->NumRings = 24;

	pCalibration->LaserSpotRadiusX = 12;
	pCalibration->LaserSpotRadiusY = 7;
	pCalibration->IntensityThreshold1 = 180;
	pCalibration->LaserSpotRingRadius = 18;
	pCalibration->IntensityThreshold2 = 40;

	pCalibration->LaserIntensityThreshold = 160;
	pCalibration->LaserDuration = 30;

	pCalibration->BallsAx[0] = 6129.0;
	pCalibration->BallsAx[1] = 7099.0;
	pCalibration->BallsAx[2] = 7939.0;
	pCalibration->BallsAx[3] = 9000.0;
}

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

CHW::CHW()
{
	LoadDLL();

	m_FlashDataStartAddress = 0x00000000;

	m_FrameTransferThread = NULL;
	m_OptometerThread = NULL;

	::ResetCalibration(&m_LCalibration);

	m_Calibration = m_LCalibration;

	m_pTVideoFrame = &m_VideoFrames[0];
	m_pDVideoFrame = &m_VideoFrames[1];
	m_DFrameProcessed = FALSE;

	m_OptometerInitialized = FALSE;
	m_OptometerTargetRangeUm = 0.0;
	m_OptometerTargetPositionD = 0.0;
	m_OptometerTargetPositionUm = 0.0;

	m_pCurrentVideoSettings = &m_Calibration.WFVideoSettings;

	if (m_hDLL != NULL)
	{
		DllInitializeDeviceStatus();

		if (DllIsConnected())
		{
			LoadCalibrationFromFlash();
		}
	}
}

//***************************************************************************************

CHW::~CHW()
{
	TurnAODsOff();
	TurnLADsOff();
	TurnVideoCameraOff();
	TurnAccommodationMotorOff();
	TurnPlacidoOff();
	TurnTriLaserOff();
	TurnInfraredLEDsOff();
	TurnWhiteLEDsOff();
	TurnAccommodationTargetOff();

	UnloadDLL();

	if (m_OptometerThread != NULL) ::CloseHandle(m_OptometerThread);
}

//***************************************************************************************


void CHW::LoadDLL()
{
#ifdef _DEBUG
	m_hDLL = ::LoadLibrary(_T("C:\\Tracey\\thw.dll"));
#else
	m_hDLL = ::LoadLibrary(_T("thw.dll"));
#endif

	if (m_hDLL == NULL) goto _ERROR_;

	DllSetupNotifications = (TDllSetupNotifications)::GetProcAddress(m_hDLL, "Lynx_SetupNotifications");
	if (DllSetupNotifications == NULL) goto _ERROR_;

	DllClearNotifications = (TDllClearNotifications)::GetProcAddress(m_hDLL, "Lynx_ClearNotifications");
	if (DllClearNotifications == NULL) goto _ERROR_;

	DllInitializeDeviceStatus = (TDllInitializeDeviceStatus)::GetProcAddress(m_hDLL, "Lynx_InitializeDeviceStatus");
	if (DllInitializeDeviceStatus == NULL) goto _ERROR_;

	DllDeviceChange = (TDllDeviceChange)::GetProcAddress(m_hDLL, "Lynx_DeviceChange");
	if (DllDeviceChange == NULL) goto _ERROR_;

	DllIsConnected = (TDllIsConnected)::GetProcAddress(m_hDLL, "Lynx_IsConnected");
	if (DllIsConnected == NULL) goto _ERROR_;

	DllGetFirmwareBuildNumber = (TDllGetFirmwareBuildNumber)::GetProcAddress(m_hDLL, "Lynx_GetFirmwareBuildNumber");
	if (DllGetFirmwareBuildNumber == NULL) goto _ERROR_;

	DllCompareFirmware = (TDllCompareFirmware)::GetProcAddress(m_hDLL, "Lynx_CompareFirmware");
	if (DllCompareFirmware == NULL) goto _ERROR_;

	DllUpdateFirmware = (TDllUpdateFirmware)::GetProcAddress(m_hDLL, "Lynx_UpdateFirmware");
	if (DllUpdateFirmware == NULL) goto _ERROR_;

	DllReadFlash = (TDllReadFlash)::GetProcAddress(m_hDLL, "Lynx_ReadFlash");
	if (DllReadFlash == NULL) goto _ERROR_;

	DllGetDataFlashSize = (TDllGetDataFlashSize)::GetProcAddress(m_hDLL, "Lynx_GetDataFlashSize");
	if (DllGetDataFlashSize == NULL) goto _ERROR_;

	DllReadDataFlash = (TDllReadDataFlash)::GetProcAddress(m_hDLL, "Lynx_ReadDataFlash");
	if (DllReadDataFlash == NULL) goto _ERROR_;

	DllWriteDataFlash = (TDllWriteDataFlash)::GetProcAddress(m_hDLL, "Lynx_WriteDataFlash");
	if (DllWriteDataFlash == NULL) goto _ERROR_;

	DllIsDataFlashBusy = (TDllIsDataFlashBusy)::GetProcAddress(m_hDLL, "Lynx_IsDataFlashBusy");
	if (DllIsDataFlashBusy == NULL) goto _ERROR_;

	DllSetPowerState = (TDllSetPowerState)::GetProcAddress(m_hDLL, "Lynx_SetPowerState");
	if (DllSetPowerState == NULL) goto _ERROR_;

	DllSetLightSourcesPowerLevels = (TDllSetLightSourcesPowerLevels)::GetProcAddress(m_hDLL, "Lynx_SetIlluminationLevels");
	if (DllSetLightSourcesPowerLevels == NULL) goto _ERROR_;

	DllGetODOSState = (TDllGetODOSState)::GetProcAddress(m_hDLL, "Lynx_GetODOSState");
	if (DllGetODOSState == NULL) goto _ERROR_;

	DllGetButtonState = (TDllGetButtonState)::GetProcAddress(m_hDLL, "Lynx_GetButtonState");
	if (DllGetButtonState == NULL) goto _ERROR_;

	DllGetTriangulationSensorReading = (TDllGetTriangulationSensorReading)::GetProcAddress(m_hDLL, "Lynx_GetTriangulationSensorReading");
	if (DllGetTriangulationSensorReading == NULL) goto _ERROR_;

	DllInitializeAccommodationMotor = (TDllInitializeAccommodationMotor)::GetProcAddress(m_hDLL, "Lynx_InitializeAccommodationMotor");
	if (DllInitializeAccommodationMotor == NULL) goto _ERROR_;

	DllIsAccommodationMotorInitializationComplete = (TDllIsAccommodationMotorInitializationComplete)::GetProcAddress(m_hDLL, "Lynx_IsAccommodationMotorInitializationComplete");
	if (DllIsAccommodationMotorInitializationComplete == NULL) goto _ERROR_;

	DllGetAccommodationMotorRange = (TDllGetAccommodationMotorRange)::GetProcAddress(m_hDLL, "Lynx_GetAccommodationMotorRange");
	if (DllGetAccommodationMotorRange == NULL) goto _ERROR_;

	DllMoveAccommodationMotor = (TDllMoveAccommodationMotor)::GetProcAddress(m_hDLL, "Lynx_MoveAccommodationMotor");
	if (DllMoveAccommodationMotor == NULL) goto _ERROR_;

	DllGetAccommodationMotorStatus = (TDllGetAccommodationMotorStatus)::GetProcAddress(m_hDLL, "Lynx_GetAccommodationMotorStatus");
	if (DllGetAccommodationMotorStatus == NULL) goto _ERROR_;

	DllInitializeCompensationMotor = (TDllInitializeCompensationMotor)::GetProcAddress(m_hDLL, "Lynx_InitializeCompensationMotor");
	if (DllInitializeCompensationMotor == NULL) goto _ERROR_;

	DllIsCompensationMotorInitializationComplete = (TDllIsCompensationMotorInitializationComplete)::GetProcAddress(m_hDLL, "Lynx_IsCompensationMotorInitializationComplete");
	if (DllIsCompensationMotorInitializationComplete == NULL) goto _ERROR_;

	DllGetCompensationMotorRange = (TDllGetCompensationMotorRange)::GetProcAddress(m_hDLL, "Lynx_GetCompensationMotorRange");
	if (DllGetCompensationMotorRange == NULL) goto _ERROR_;

	DllMoveCompensationMotor = (TDllMoveCompensationMotor)::GetProcAddress(m_hDLL, "Lynx_MoveCompensationMotor");
	if (DllMoveCompensationMotor == NULL) goto _ERROR_;

	DllGetCompensationMotorStatus = (TDllGetCompensationMotorStatus)::GetProcAddress(m_hDLL, "Lynx_GetCompensationMotorStatus");
	if (DllGetCompensationMotorStatus == NULL) goto _ERROR_;

	DllSetVideoBrightness = (TDllSetVideoBrightness)::GetProcAddress(m_hDLL, "Lynx_SetVideoBrightness");
	if (DllSetVideoBrightness == NULL) goto _ERROR_;

	DllSetVideoContrast = (TDllSetVideoContrast)::GetProcAddress(m_hDLL, "Lynx_SetVideoContrast");
	if (DllSetVideoContrast == NULL) goto _ERROR_;

	DllSetVideoHue = (TDllSetVideoHue)::GetProcAddress(m_hDLL, "Lynx_SetVideoHue");
	if (DllSetVideoHue == NULL) goto _ERROR_;

	DllSetVideoSaturation = (TDllSetVideoSaturation)::GetProcAddress(m_hDLL, "Lynx_SetVideoSaturation");
	if (DllSetVideoSaturation == NULL) goto _ERROR_;

	DllGetMostRecentVideoFrame = (TDllGetMostRecentVideoFrame)::GetProcAddress(m_hDLL, "Lynx_GetMostRecentVideoFrame");
	if (DllGetMostRecentVideoFrame == NULL) goto _ERROR_;

	DllSenseHardwareStatus = (TDllSenseHardwareStatus)::GetProcAddress(m_hDLL, "Lynx_SenseHardwareStatus");
	if (DllSenseHardwareStatus == NULL) goto _ERROR_;

	DllStartExamSequence = (TDllStartExamSequence)::GetProcAddress(m_hDLL, "Lynx_StartExamSequence");
	if (DllStartExamSequence == NULL) goto _ERROR_;

	DllIsExamFinished = (TDllIsExamFinished)::GetProcAddress(m_hDLL, "Lynx_IsExamFinished");
	if (DllIsExamFinished == NULL) goto _ERROR_;

	DllGetExamResultData = (TDllGetExamResultData)::GetProcAddress(m_hDLL, "Lynx_GetExamResultData");
	if (DllGetExamResultData == NULL) goto _ERROR_;

	DllStartContinuousScanning = (TDllStartContinuousScanning)::GetProcAddress(m_hDLL, "Lynx_StartContinuousScanning");
	if (DllStartContinuousScanning == NULL) goto _ERROR_;

	DllStopContinuousScanning = (TDllStopContinuousScanning)::GetProcAddress(m_hDLL, "Lynx_StopContinuousScanning");
	if (DllStopContinuousScanning == NULL) goto _ERROR_;

	DllSetAOD = (TDllSetAOD)::GetProcAddress(m_hDLL, "Lynx_SetAOD");
	if (DllSetAOD == NULL) goto _ERROR_;

	DllReadLAD = (TDllReadLAD)::GetProcAddress(m_hDLL, "Lynx_ReadLAD");
	if (DllReadLAD == NULL) goto _ERROR_;

	DllIsLaserSafetyEnabled = (TDllIsLaserSafetyEnabled)::GetProcAddress(m_hDLL, "Lynx_IsLaserSafetyEnabled");
	if (DllIsLaserSafetyEnabled == NULL) goto _ERROR_;

	return;

_ERROR_:

	UnloadDLL();
}

//***************************************************************************************

void CHW::UnloadDLL()
{
	if (m_hDLL != NULL)
	{
		::FreeLibrary(m_hDLL);
		m_hDLL = NULL;
	}
}

//***************************************************************************************

int CHW::DeviceChange(uint EventType, DWORD_PTR Data)
{
	const GUID GUID_CLASS_NCBULK = { 0x244300fa, 0x127d, 0x4ad8, 0x92, 0xf6, 0x38, 0x66, 0x8f, 0xfd, 0xb1, 0x9f };

	if (m_hDLL == NULL) return DEVICE_IGNORE;

	DEV_BROADCAST_DEVICEINTERFACE* pDeviceInterface = (DEV_BROADCAST_DEVICEINTERFACE*)Data;
	if (pDeviceInterface != NULL)
	{
		if (pDeviceInterface->dbcc_classguid == GUID_CLASS_NCBULK)
		{
			if (DllDeviceChange(EventType, Data))
			{
				if (EventType == DBT_DEVICEARRIVAL)
				{
					m_OptometerInitialized = FALSE;
					if (DllIsConnected())
					{
						LoadCalibrationFromFlash();
						return DEVICE_ARRIVED;
					}
				}
				else if (EventType == DBT_DEVICEREMOVECOMPLETE)
				{
					m_OptometerInitialized = FALSE;
					return DEVICE_REMOVED;
				}
			}
		}
	}

	return DEVICE_IGNORE;
}

//***************************************************************************************

void CHW::SetNotificationWindow(CWnd* pWnd)
{
	if (m_hDLL == NULL) return;

	DllSetupNotifications(pWnd->m_hWnd);
}

void CHW::ReleaseNotificationWindow()
{
	if (m_hDLL == NULL) return;

	DllClearNotifications();
}

//***************************************************************************************

BOOL CHW::IsConnected()
{
	if (m_hDLL == NULL) return FALSE;

	return DllIsConnected();
}

//***************************************************************************************

ulong CHW::GetFirmwareBuild()
{
	ushort Version = 0;

	if (m_hDLL) DllGetFirmwareBuildNumber(&Version);

	return Version;
}

//***************************************************************************************

BOOL CHW::HasCalibrationChanged()
{
	return memcmp(&m_LCalibration, &m_Calibration, sizeof(CALIBRATION)) != 0;
}

//***************************************************************************************

BOOL CHW::LoadCalibrationFromFlash(CALIBRATION* pCalibration)
{
	BOOL Result = FALSE;
	CString s;
	ulong SizeRead = 0;
	ulong ErrorCode = 0xffffffff;
	int ZipSize = 0;
	if /*1*/ (DllReadDataFlash((uchar*)&ZipSize, m_FlashDataStartAddress, 4, &SizeRead, &ErrorCode))
	{
		if /*2*/ (ErrorCode == 0)
		{
			if /*3*/ (SizeRead == 4)
			{
				if /*4*/ (ZipSize > 0 || ZipSize <= sizeof(CALIBRATION))
				{
					void* pZipData = malloc(ZipSize);
					if /*5*/ (DllReadDataFlash((uchar*)pZipData, m_FlashDataStartAddress + 4, ZipSize, &SizeRead, &ErrorCode))
					{
						if /*6*/ (ErrorCode == 0)
						{
							if /*7*/ (SizeRead == ZipSize)
							{
								int CalSize = ::GetDecompressedSize(pZipData, ZipSize);
								if /*8*/ (CalSize == sizeof(CALIBRATION))
								{
									if /*9*/ (::Decompress(pZipData, ZipSize, pCalibration))
									{
										Result = TRUE;
									}
									else /*9*/
									{
										::Error("Failed to decompress data.");
									}
								}
								else /*8*/
								{
									::Error("Incorrect size of decompressed data.");
								}
							}
							else /*7*/
							{
								::Error("Improper data size read from EEPROM.");
							}
						}
						else /*6*/
						{
							s.Format(_T("Error reading from EEPROM.\nError code: %i."), ErrorCode);
							::Error(s);
						}
					}
					else /*5*/
					{
						::Error("Failed to read from EEPROM.");
					}
					free(pZipData);
				}
				else /*4*/
				{
					::Error("Inappropriate zip size read from EEPROM.");
				}
			}
			else /*3*/
			{
				::Error("Improper amount of data read from EEPROM.");
			}
		}
		else /*2*/
		{
			s.Format(_T("Error reading from EEPROM.\nError code: %i."), ErrorCode);
			::Error(s);
		}
	}
	else /*1*/
	{
		::Error("Failed to read from EEPROM.");
	}

	if (!Result) {
		::Error("Failed to load configuration settings from EEPROM.");
	}

	return Result;
}

//***************************************************************************************

BOOL CHW::LoadCalibrationFromFlash()
{
	if (m_hDLL == NULL)
	{
		::Error("Hardware DLL is not loaded.");
		return FALSE;
	}

	CALIBRATION* pCalibration = new CALIBRATION;

	BOOL Result = LoadCalibrationFromFlash(pCalibration);

	if (Result)
	{
		m_LCalibration = *pCalibration;
		m_Calibration = *pCalibration;
	}

	delete pCalibration;

	return Result;
}

//***************************************************************************************

BOOL CHW::SaveCalibrationIntoFlash(CALIBRATION* pCalibration)
{
	// GUID
	::CoCreateGuid(&pCalibration->CalibrationID);
	// Time stamp
	::GetLocalTime(&pCalibration->Time);

	// Compress
	uchar* pDataToSave = (uchar*)malloc(4 + sizeof(CALIBRATION));
	int ZipSize;

	if (!::Compress(pCalibration, sizeof(CALIBRATION), pDataToSave + 4, &ZipSize))
	{
		free(pDataToSave);
		::Error("Failed to compress data.");
		return FALSE;
	}

	int SizeToSave = 4 + ZipSize;

	// put the size of the archive in the beginning
	*((int*)pDataToSave) = (int)ZipSize;

	BOOL Result = FALSE;
	CString s;
	ulong ErrorCode = 0xffffffff;
	if /*1*/ (DllWriteDataFlash(pDataToSave, m_FlashDataStartAddress, SizeToSave, &ErrorCode))
	{
		if /*2*/ (ErrorCode == 0)
		{
			for (int i = 0; i < 100; i++)
			{
				BOOL DataFlashBusy = TRUE;
				if /*3*/ (DllIsDataFlashBusy(&DataFlashBusy))
				{
					if /*4*/ (DataFlashBusy)
					{
						Sleep(100);
					}
					else /*4*/
					{
						uchar* pDataReadBack = (uchar*)malloc(SizeToSave);
						ulong SizeReadBack = 0;
						ulong ErrorCode = 0xffffffff;

						if /*5*/ (DllReadDataFlash(pDataReadBack, m_FlashDataStartAddress, SizeToSave, &SizeReadBack, &ErrorCode))
						{
							if /*6*/ (ErrorCode == 0)
							{
								if /*7*/ (SizeReadBack == SizeToSave)
								{
									if /*8*/ (memcmp(pDataReadBack, pDataToSave, SizeToSave) == 0)
									{
										Result = TRUE;
									}
									else /*8*/
									{
										::Error("Data saved into EEPROM and data read back from it are not the same.");
									}
								}
								else /*7*/
								{
									::Error("Size of the data saved into EEPROM and read back from it is not the same.");
								}
							}
							else /*6*/
							{
								s.Format(_T("Error reading back from EEPROM.\nError code: %i."), ErrorCode);
								::Error(s);
							}
						}
						else /*5*/
						{
							::Error("Failed to read back from EEPROM.");
						}
						free(pDataReadBack);

						break;
					} /*4*/
				}
				else /*3*/
				{
					::Error("Failed to check if data flash was busy.");
					break;
				}
			}
		}
		else /*2*/
		{
			s.Format(_T("Error writing to EEPROM.\nError code: %i."), ErrorCode);
			::Error(s);
		}
	}
	else /*1*/
	{
		::Error("Failed to write to EEPROM.");
	}

	free(pDataToSave);

	return Result;
}

//***************************************************************************************

BOOL CHW::SaveCalibrationIntoFlash()
{
	if (m_hDLL == NULL)
	{
		::Error("Hardware DLL is not loaded.");
		return FALSE;
	}

	if (m_LCalibration.CalibrationID != GUID_NULL)
	{
		if (!HasCalibrationChanged())
		{
			::Warning("Configuration settings have not been changed.");
			return FALSE;
		}
	}

	if (!SaveCalibrationIntoFlash(&m_Calibration))
	{
		return FALSE;
	}

	m_LCalibration = m_Calibration;

	return TRUE;
}

//***************************************************************************************

BOOL CHW::LoadCalibrationFromFile(const CString& FileName)
{
	BOOL Result = FALSE;

	CALIBRATION* pCalibration = new CALIBRATION;

	HANDLE hFile = ::CreateFile(FileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if /*1*/ (hFile != INVALID_HANDLE_VALUE)
	{
		int ZipSize = ::GetFileSize(hFile, NULL);
		void* pZipData = malloc(ZipSize);
		ulong SizeRead = 0;
		if /*2*/ (::ReadFile(hFile, pZipData, ZipSize, &SizeRead, NULL))
		{
			if /*3*/ (SizeRead == ZipSize)
			{
				int CalSize = ::GetDecompressedSize(pZipData, ZipSize);
				if /*4*/ (CalSize == sizeof(CALIBRATION))
				{
					if /*5*/ (::Decompress(pZipData, ZipSize, pCalibration))
					{
						m_Calibration = *pCalibration;
						Result = TRUE;
					}
					else /*5*/
					{
						::Error("Failed to decompress data.");
					}
				}
				else /*4*/
				{
					::Error("Incorrect size of decompressed data.");
				}
			}
			else /*3*/
			{
				::Error("Improper data size read from file.");
			}
		}
		else /*2*/
		{
			::Error("Failed to read from file.");
		}
		free(pZipData);
		::CloseHandle(hFile);
	}
	else /*1*/
	{
		::Error("Failed to open file.");
	}

	if (Result)
	{
		::Info("Configuration settings loaded successfully.");
	}
	else
	{
		::Error("Failed to load configuration settings from file.");
	}

	delete pCalibration;

	return Result;
}

//***************************************************************************************

BOOL CHW::SaveCalibrationIntoFile(const CString& FileName)
{
	if (m_LCalibration.CalibrationID == GUID_NULL)
	{
		::Error("Save configuration settings into EEPROM first.");
		return FALSE;
	}
	if (HasCalibrationChanged())
	{
		::Error("Configuration settings have been changed. Save into EEPROM first.");
		return FALSE;
	}

	// Compress
	void* pZipData = malloc(sizeof(CALIBRATION));
	int ZipSize;

	if (!::Compress(&m_Calibration, sizeof(CALIBRATION), pZipData, &ZipSize))
	{
		free(pZipData);
		::Error("Failed to compress data.");
		return FALSE;
	}

	BOOL Result = FALSE;

	HANDLE hFile = ::CreateFile(FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		ulong bt;
		if (::WriteFile(hFile, pZipData, ZipSize, &bt, NULL))
		{
			if (bt == ZipSize)
			{
				Result = TRUE;
			}
			else
			{
				::Error("Improper data size written to file.");
			}
		}
		else
		{
			::Error("Failed to write to file.");
		}
	}
	else
	{
		::Error("Failed to create file.");
	}

	::CloseHandle(hFile);

	if (Result)
	{
		::Info("Configuration settings saved successfully into file.");
	}
	else
	{
		::Error("Failed to save configuration settings into file.");
	}

	free(pZipData);

	return Result;
}

//***************************************************************************************

void CHW::CancelCalibrationChanges()
{
	m_Calibration = m_LCalibration;
}

//***************************************************************************************
//6.2.0 Add new info s and return type
int CHW::CheckLaserSafetyStatus()
{
	if (m_hDLL == NULL) return NOHWCON;

	if (!DllIsConnected()) return NOHWCON;

	BOOL IsEnabled = FALSE;
	if (!DllIsLaserSafetyEnabled(&IsEnabled))
	{
		return LASERFAIL;
	}

	if (!IsEnabled)
	{
		//::Warning("Laser safety is disabled.");
		return LASERFAIL;
	}

	return LASERSAFE;
}

//***************************************************************************************

void CHW::ApplyCurrentVideoBrightness()
{
	if (m_hDLL == NULL) return;

	DllSetVideoBrightness(m_pCurrentVideoSettings->Brightness);
}

//***************************************************************************************

void CHW::ApplyCurrentVideoContrast()
{
	if (m_hDLL == NULL) return;

	DllSetVideoContrast(m_pCurrentVideoSettings->Contrast);
}

//***************************************************************************************

void CHW::ApplyCurrentVideoHue()
{
	if (m_hDLL == NULL) return;

	DllSetVideoHue(m_pCurrentVideoSettings->Hue);
}

//***************************************************************************************

void CHW::ApplyCurrentVideoSaturation()
{
	if (m_hDLL == NULL) return;

	DllSetVideoSaturation(m_pCurrentVideoSettings->Saturation);
}

//***************************************************************************************

void CHW::ClearFrames()
{
	memset(m_VideoFrames, 0, sizeof(m_VideoFrames));
}

//***************************************************************************************

void CHW::FrameTransferMain()
{
	BOOL Available = FALSE;

	if (m_hDLL != NULL)
	{
		ulong StartTime = clock();
		while (!Available && clock() - StartTime < 50)
		{
			if (!DllGetMostRecentVideoFrame(m_pCurrentVideoSettings->Color, 0, 0, &Available, m_pTVideoFrame)) break;
		}
	}

	if (!Available)
	{
		uchar* pSrcData = m_pTVideoFrame->Data;

		if (m_pCurrentVideoSettings->Color)
		{
			uchar* pSrcOdd = pSrcData;
			uchar* pSrcEvn = pSrcData + 243 * (720 + 720);

			for (int y = 0; y < 243; y++)
			{
				for (int x = 0; x < 1440; x += 4)
				{
					pSrcOdd[x] = 16 + (240 - 16) * (2 * y) / 485; // Cb
					pSrcOdd[x + 1] = 16; // Y1
					pSrcOdd[x + 2] = 16 + (240 - 16) * x / 1439;// Cr
					pSrcOdd[x + 3] = 16; // Y2

					pSrcEvn[x] = 16 + (240 - 16) * (2 * y + 1) / 485; // Cb
					pSrcEvn[x + 1] = 16; // Y1
					pSrcEvn[x + 2] = 16 + (240 - 16) * x / 1439; // Cr
					pSrcEvn[x + 3] = 16; // Y2
				}

				pSrcOdd += 1440;
				pSrcEvn += 1440;
			}
		}
		else
		{
			uchar* pSrcOdd = pSrcData;
			uchar* pSrcEvn = pSrcData + 243 * 720;

			for (int y = 0; y < 243; y++)
			{
				for (int x = 0; x < 720; x++)
				{
					pSrcOdd[x] = 16;
					pSrcEvn[x] = 16;
				}
				pSrcOdd += 720;
				pSrcEvn += 720;
			}
		}
	}
}

//***************************************************************************************

void CHW::StartTransferringVideoFrame()
{
	ulong id;
	m_FrameTransferThread = ::CreateThread(NULL, 0, FrameTransferThreadFunction, this, CREATE_SUSPENDED, &id);

	if (m_FrameTransferThread == NULL) return;

	::SetThreadPriority(m_FrameTransferThread, THREAD_PRIORITY_BELOW_NORMAL);
	::ResumeThread(m_FrameTransferThread);
	::Sleep(1);
}

//***************************************************************************************

void CHW::FinishTransferringVideoFrame()
{
	if (m_FrameTransferThread == NULL) return;

	::WaitForSingleObject(m_FrameTransferThread, INFINITE);

	::CloseHandle(m_FrameTransferThread);
	m_FrameTransferThread = NULL;

	Swap(m_pTVideoFrame, m_pDVideoFrame);
	m_DFrameProcessed = FALSE;
}

//***************************************************************************************

uchar* CHW::GetRGBData()
{
	static uchar Data1[3 * 468 * 624];
	static uchar Data2[3 * 468 * 624];

	if (m_DFrameProcessed) return Data1;

	uchar* pSrcData = m_pDVideoFrame->Data;
	uchar* pDstData = Data1;

	//Control the image position in calibration 
	int moveUpDown = ::HW.m_Calibration.StepDown;
	int moveLeftRight = ::HW.m_Calibration.StepRight;

	if (moveUpDown < 0 || moveUpDown > 10) moveUpDown = 5;
	if (moveLeftRight < 0 || moveLeftRight > 16) moveUpDown = 8;

	if ((moveUpDown == 0) && (moveLeftRight == 0) && (::HW.m_Calibration.StepUp == 0) && (::HW.m_Calibration.StepLeft == 0))
	{
		moveUpDown = 5;
		moveLeftRight = 8;
	}
	//Control the image position in calibration Done


	// color
	if (m_pCurrentVideoSettings->Color)
	{
		uchar* pSrcOdd = pSrcData + moveUpDown * 1440; // [5.1.1]
		uchar* pSrcEvn = pSrcData + 243 * 1440 + moveUpDown * 1440; // [5.1.1]

		uchar* pDstLum = pDstData;
		uchar* pDstChr = pDstData + 468 * 624;

		for (int dy = 0; dy < 234; dy++)
		{
			for (int dx = 0; dx < 624; dx += 2)
			{
				// luminance
				for (int i = 0; i < 2; i++)
				{
					int u = (dx + i) * 701;
					int v = u % 623;
					//int sx_y1 = 16 + ((u / 623) << 1) + 1; // домножить на 2 и сдвинуть на 1 вправо, потому что вперемежку с цветностью
					int sx_y1 = moveLeftRight * 2 + ((u / 623) << 1) + 1; // [5.1.1]
					int sx_y2 = sx_y1 + 2;
					// even
					int Y1 = pSrcEvn[sx_y1];
					int Y2 = pSrcEvn[sx_y2];
					pDstLum[dx + i] = (uchar)(Y1 + (Y2 - Y1) * v / 623);
					// odd
					Y1 = pSrcOdd[sx_y1];
					Y2 = pSrcOdd[sx_y2];
					pDstLum[dx + i + 624] = (uchar)(Y1 + (Y2 - Y1) * v / 623);
				}

				// chrominance
				int u = (dx >> 1) * 350;
				int v = u % 311;
				//int sx_cb1 = 16 + ((u / 311) << 2);
				int sx_cb1 = moveLeftRight * 2 + ((u / 311) << 2);// [5.1.1]
				int sx_cr1 = sx_cb1 + 2;
				int sx_cb2 = sx_cb1 + 4;
				int sx_cr2 = sx_cb1 + 6;
				// even
				int Cb1 = pSrcEvn[sx_cb1];
				int Cb2 = pSrcEvn[sx_cb2];
				int Cr1 = pSrcEvn[sx_cr1];
				int Cr2 = pSrcEvn[sx_cr2];
				pDstChr[dx] = (uchar)(Cb1 + (Cb2 - Cb1) * v / 311);
				pDstChr[dx + 1] = (uchar)(Cr1 + (Cr2 - Cr1) * v / 311);
				// odd
				Cb1 = pSrcOdd[sx_cb1];
				Cb2 = pSrcOdd[sx_cb2];
				Cr1 = pSrcOdd[sx_cr1];
				Cr2 = pSrcOdd[sx_cr2];
				pDstChr[dx + 624] = (uchar)(Cb1 + (Cb2 - Cb1) * v / 311);
				pDstChr[dx + 1 + 624] = (uchar)(Cr1 + (Cr2 - Cr1) * v / 311);

			}
			pSrcOdd += (720 + 720);
			pSrcEvn += (720 + 720);
			pDstLum += (624 + 624);
			pDstChr += (624 + 624);
		}
	}
	// color

	// monochrome
	else
	{
		//uchar* pSrcOdd = pSrcData +             5 * 720;
		//uchar* pSrcEvn = pSrcData + 243 * 720 + 5 * 720;

		uchar* pSrcOdd = pSrcData + moveUpDown * 720;// [5.1.1]
		uchar* pSrcEvn = pSrcData + 243 * 720 + moveUpDown * 720;// [5.1.1]

		uchar* pDstLum = pDstData;

		for (int dy = 0; dy < 234; dy++)
		{
			for (int dx = 0; dx < 624; dx++)
			{
				int u = dx * 701;
				int v = u % 623;
				//int sx_y1 = 8 + u / 623;
				int sx_y1 = moveLeftRight + u / 623;
				int sx_y2 = sx_y1 + 1;
				// even
				int Y1 = pSrcEvn[sx_y1];
				int Y2 = pSrcEvn[sx_y2];
				pDstLum[dx] = (uchar)(Y1 + (Y2 - Y1) * v / 623);
				// odd
				Y1 = pSrcOdd[sx_y1];
				Y2 = pSrcOdd[sx_y2];
				pDstLum[dx + 624] = (uchar)(Y1 + (Y2 - Y1) * v / 623);
			}

			pSrcOdd += 720;
			pSrcEvn += 720;
			pDstLum += (624 + 624);
		}
	}
	// monochrome

	pSrcData = Data1;
	pDstData = Data2;

	// flipping
	{
		uchar* pSrcLum = pSrcData + (m_Calibration.VideoFlipY ? (468 - 1) * 624 : 0);
		uchar* pSrcChr = pSrcLum + 468 * 624;
		uchar* pDstLum = pDstData;
		uchar* pDstChr = pDstLum + 468 * 624;
		int sd = m_Calibration.VideoFlipY ? -624 : 624;
		if (m_Calibration.VideoFlipX)
		{
			for (int y = 0; y < 468; y++)
			{
				rev_byte_memcpy(pDstLum, pSrcLum, 624);
				pSrcLum += sd;
				pDstLum += 624;
			}
			if (m_pCurrentVideoSettings->Color)
			{
				for (int y = 0; y < 468; y++)
				{
					rev_word_memcpy(pDstChr, pSrcChr, 312);
					pSrcChr += sd;
					pDstChr += 624;
				}
			}
		}
		else
		{
			for (int y = 0; y < 468; y++)
			{
				memcpy(pDstLum, pSrcLum, 624);
				pSrcLum += sd;
				pDstLum += 624;
			}
			if (m_pCurrentVideoSettings->Color)
			{
				for (int y = 0; y < 468; y++)
				{
					memcpy(pDstChr, pSrcChr, 624);
					pSrcChr += sd;
					pDstChr += 624;
				}
			}
		}
	}
	// flipping

	pSrcData = Data2;
	pDstData = Data1;

	// YCbCr -> RGB
	{
		int w = 624;
		int h = 468;
		uchar* pLData = pSrcData;
		uchar* pCData = m_pCurrentVideoSettings->Color ? pSrcData + 468 * 624 : NULL;
		uchar* pRGBData = pDstData;

		if (pCData)
		{
			int s = h * (w >> 1);

			while (s--)
			{
				int Y1 = *pLData++;
				int Y2 = *pLData++;
				int Cb = *pCData++;
				int Cr = *pCData++;
				Y1 = Y1 < 16 ? 16 : (Y1 > 235 ? 235 : Y1); // 16 <=  Y <= 235   220 values
				Y2 = Y2 < 16 ? 16 : (Y2 > 235 ? 235 : Y2);
				Cb = Cb < 16 ? 16 : (Cb > 240 ? 240 : Cb); // 16 <= Cb <= 240   225 values
				Cr = Cr < 16 ? 16 : (Cr > 240 ? 240 : Cr); // 16 <= Cr <= 240   225 values

				Y1 = 1193/*1165*/ * (Y1 - 16);
				Y2 = 1193/*1165*/ * (Y2 - 16);
				Cb -= 128;
				Cr -= 128;
				int Crr = 1635/*1597*/ * Cr;
				int Cbg = -401/* 392*/ * Cb;
				int Crg = -833/* 813*/ * Cr;
				int Cbb = 2066/*2018*/ * Cb;

				int R1 = (Y1 + Crr) >> 10; // заменил деление на 1000 на деление на 1024
				int G1 = (Y1 + Crg + Cbg) >> 10; // соответственно чуть чуть поменялись коэффициенты
				int B1 = (Y1 + Cbb) >> 10;
				int R2 = (Y2 + Crr) >> 10;
				int G2 = (Y2 + Crg + Cbg) >> 10;
				int B2 = (Y2 + Cbb) >> 10;

				R1 = R1 < 0 ? 0 : (R1 > 255 ? 255 : R1);
				G1 = G1 < 0 ? 0 : (G1 > 255 ? 255 : G1);
				B1 = B1 < 0 ? 0 : (B1 > 255 ? 255 : B1);
				R2 = R2 < 0 ? 0 : (R2 > 255 ? 255 : R2);
				G2 = G2 < 0 ? 0 : (G2 > 255 ? 255 : G2);
				B2 = B2 < 0 ? 0 : (B2 > 255 ? 255 : B2);

				*pRGBData++ = (uchar)B1;
				*pRGBData++ = (uchar)G1;
				*pRGBData++ = (uchar)R1;
				*pRGBData++ = (uchar)B2;
				*pRGBData++ = (uchar)G2;
				*pRGBData++ = (uchar)R2;
			}
		}
		else
		{
			int s = h * w;

			while (s--)
			{
				int Y = *pLData++;
				Y = Y < 16 ? 16 : (Y > 235 ? 235 : Y);
				Y = (1193 * (Y - 16)) >> 10;

				*pRGBData++ = (uchar)Y;
				*pRGBData++ = (uchar)Y;
				*pRGBData++ = (uchar)Y;
			}
		}
	}
	// YCbCr -> RGB

	m_DFrameProcessed = TRUE;

	return Data1;
}

//***************************************************************************************

int CHW::GetEye()
{
	if (m_hDLL == NULL) return EYE_OD;

	BOOL OD;

	if (!DllGetODOSState(&OD)) return EYE_OD;

	return OD ? EYE_OD : EYE_OS;
}

//***************************************************************************************

BOOL CHW::IsButtonPressed()
{
	if (m_hDLL == NULL) return FALSE;

	int Count;
	BOOL State;

	if (!DllGetButtonState(&Count, &State)) return FALSE;

	return State;
}

//***************************************************************************************

int CHW::GetTriangulationSensorReading()
{
	if (m_hDLL == NULL) return 0;

	int Reading = 0;

	if (!DllGetTriangulationSensorReading(&Reading)) return 0;

	return 65535 - Reading;
}

//***************************************************************************************

int CHW::UmToHzX(const real_t XUm)
{
	real_t d = XUm * m_Calibration.AODX.HzUm;

	if (!m_Calibration.AODXNoFlip) d = -d;

	return intRound(m_Calibration.AODX.Freq0 - d);
}

//***************************************************************************************

int CHW::UmToHzY(const real_t YUm)
{
	real_t d = YUm * m_Calibration.AODY.HzUm;

	if (!m_Calibration.AODYNoFlip) d = -d;

	return intRound(m_Calibration.AODY.Freq0 + d);
}

//***************************************************************************************

BOOL CHW::DoWFExam(const int NumPoints, const real_t* pXUm, const real_t* pYUm)
{
	if (m_hDLL == NULL) return FALSE;

	AOD_CONFIG* pAODConfig = new AOD_CONFIG[NumPoints];

	for (int p = 0; p < NumPoints; p++)
	{
		pAODConfig[p].XHz = UmToHzX(pXUm[p]);
		pAODConfig[p].YHz = UmToHzY(pYUm[p]);
		pAODConfig[p].XPower = m_Calibration.AODX.RMS;
		pAODConfig[p].YPower = m_Calibration.AODY.RMS;
	}

	//int sT = (int)clock();//531 Only For wf time test As Victor ask in 11/11/2014

	BOOL Res = DllStartExamSequence(NumPoints, m_Calibration.LADExposureTime, 1, 0, pAODConfig, TRUE);

	delete[] pAODConfig;

	if (Res)
	{
		for (int i = 0; i < 100; i++)
		{
			::Sleep(100);

			BOOL ExamFinished = FALSE;
			BOOL DDSErrorOccured = FALSE;
			if (DllIsExamFinished(&ExamFinished, &DDSErrorOccured))
			{
				if (ExamFinished)
				{
					if (DDSErrorOccured)
					{
						::Error("DDS error.");
						return FALSE;
					}

					return TRUE;
				}
			}
			else
			{
				return FALSE;
			}
		}
	}

	return FALSE;
}

//***************************************************************************************

BOOL CHW::GetLADsData(const int NumPoints, ushort* pLADXData, ushort* pLADYData)
{
	if (m_hDLL == NULL) return FALSE;

	int NumPointsRead;
	int RetinaSensorType;
	int CorneaSensorType;
	DETECTOR_FRAME* pRetinaData = new DETECTOR_FRAME[NumPoints];

	BOOL Res = DllGetExamResultData(&NumPointsRead, &RetinaSensorType, &CorneaSensorType, pRetinaData, NULL);

	Res &= NumPoints == NumPointsRead;

	if (Res)
	{
		for (int p = 0; p < NumPoints; p++)
		{
			int pp = p * 512;
			for (int i = 0; i < 512; i++)
			{
				pLADXData[pp + i] = (ushort)pRetinaData[p].XData[i];
				pLADYData[pp + i] = (ushort)pRetinaData[p].YData[i];
			}
		}
		for (int p = 0; p < NumPoints; p++)
		{
			int pp = p * 512;
			if (m_Calibration.LADSwapXY)
			{
				for (int i = 0; i < 512; i++) Swap(pLADXData[pp + i], pLADYData[pp + i]);
			}
			if (!m_Calibration.LADXNoFlip)
			{
				for (int i = 0; i < 256; i++) Swap(pLADXData[pp + i], pLADXData[pp + 511 - i]);
			}
			if (!m_Calibration.LADYNoFlip)
			{
				for (int i = 0; i < 256; i++) Swap(pLADYData[pp + i], pLADYData[pp + 511 - i]);
			}
		}
	}
	else
	{
		for (int p = 0; p < NumPoints; p++)
		{
			int pp = p * 512;

			for (int i = 0; i < 512; i++)
			{
				pLADXData[pp + i] = 0;
				pLADYData[pp + i] = 0;
			}
		}
	}

	delete[] pRetinaData;

	return Res;
}

//***************************************************************************************

void CHW::StartContinuousScanning(const int NumPoints, const real_t* pXUm, const real_t* pYUm)
{
	if (m_hDLL == NULL) return;

	AOD_CONFIG* pAODConfig = new AOD_CONFIG[NumPoints];

	for (int p = 0; p < NumPoints; p++)
	{
		pAODConfig[p].XHz = UmToHzX(pXUm[p]);
		pAODConfig[p].YHz = UmToHzY(pYUm[p]);
		pAODConfig[p].XPower = m_Calibration.AODX.RMS;
		pAODConfig[p].YPower = m_Calibration.AODY.RMS;
	}

	DllStartContinuousScanning(NumPoints, m_Calibration.LADExposureTime, pAODConfig);

	delete[] pAODConfig;
}

//***************************************************************************************

void CHW::StopContinuousScanning()
{
	if (m_hDLL == NULL) return;

	DllStopContinuousScanning();
}

//***************************************************************************************

void CHW::SetAODsFrequencies(const real_t XUm, const real_t YUm)
{
	if (m_hDLL == NULL) return;

	m_XHz = UmToHzX(XUm);
	m_YHz = UmToHzY(YUm);
	int XPower = m_Calibration.AODX.RMS;
	int YPower = m_Calibration.AODY.RMS;

	DllSetAOD(m_XHz, m_YHz, XPower, YPower);
}

//***************************************************************************************

BOOL CHW::ReadLADs(ushort* pLADXData, ushort* pLADYData)
{
	if (m_hDLL == NULL) return FALSE;

	BOOL Res = DllReadLAD(m_Calibration.LADExposureTime, (short*)pLADXData, (short*)pLADYData);

	if (Res)
	{
		if (m_Calibration.LADSwapXY)
		{
			for (int i = 0; i < 512; i++) Swap(pLADXData[i], pLADYData[i]);
		}
		if (!m_Calibration.LADXNoFlip)
		{
			for (int i = 0; i < 256; i++) Swap(pLADXData[i], pLADXData[511 - i]);
		}
		if (!m_Calibration.LADYNoFlip)
		{
			for (int i = 0; i < 256; i++) Swap(pLADYData[i], pLADYData[511 - i]);
		}
	}
	else
	{
		for (int i = 0; i < 512; i++)
		{
			pLADXData[i] = 0;
			pLADYData[i] = 0;
		}
	}

	return Res;
}

//***************************************************************************************

void CHW::InitializeOptometer()
{
	if (m_hDLL == NULL) return;
	if (m_Calibration.OptometerEnabled != 1) return;
	if (!IsConnected()) return;
	if (m_OptometerInitialized) return;

	BOOL Complete = FALSE;
	BOOL WorkingCorrectly = FALSE;

	TurnAccommodationMotorOn();

	if (DllInitializeAccommodationMotor())
	{
		for (int i = 0; i < 120; i++)
		{
			if (!DllIsAccommodationMotorInitializationComplete(&Complete, &WorkingCorrectly)) break;
			if (Complete) break;
			::Sleep(500);
		}
	}

	TurnAccommodationMotorOff();

	if (Complete && WorkingCorrectly)
	{
		real_t RangeMm = 0.0;
		if (DllGetAccommodationMotorRange(&RangeMm))
		{
			m_OptometerInitialized = TRUE;
			m_OptometerTargetRangeUm = 1000.0 * RangeMm;
		}
	}

	if (!m_OptometerInitialized)
	{
		::Error("Optometer failed to initialize.");
	}
}

//***************************************************************************************

void CHW::MoveOptometerTargetToPositionD(real_t PositionD, BOOL Wait)
{
	if (m_hDLL == NULL) return;
	if (m_Calibration.OptometerEnabled != 1) return;
	if (!IsConnected()) return;
	if (!m_OptometerInitialized) return;

	real_t LowerLimitUm = 0.0;
	real_t UpperLimitUm = m_OptometerTargetRangeUm;
	if (LowerLimitUm > UpperLimitUm) return;

	PositionD = RealRound(PositionD, 0.25);

	Matrix<real_t> X(21);
	Matrix<real_t> Y(21);

	for (int i = 0; i <= 20; i++)
	{
		X[i] = -10 + i;
		Y[i] = m_Calibration.OptometerTable[i] - m_Calibration.OptometerGap;
	}

	CCubicSpline Spline;
	Spline.Create(X, Y);

	real_t PositionUm;
	Spline.GetAt(PositionD, PositionUm, TRUE);

	while (PositionUm > UpperLimitUm)
	{
		PositionD -= 0.25;
		Spline.GetAt(PositionD, PositionUm, TRUE);
	}

	while (PositionUm < LowerLimitUm)
	{
		PositionD += 0.25;
		Spline.GetAt(PositionD, PositionUm, TRUE);
	}

	if (PositionUm > UpperLimitUm) return;

	if (fabs(PositionD - m_OptometerTargetPositionD) < 0.249) return;

	m_OptometerTargetMoveToPositionD = PositionD;
	m_OptometerTargetMoveToPositionUm = PositionUm;

	if (OptometerStartThread())
	{
		if (Wait)
		{
			::WaitForSingleObject(m_OptometerThread, INFINITE);
			::CloseHandle(m_OptometerThread);
			m_OptometerThread = NULL;
		}
	}
}

//***************************************************************************************

BOOL CHW::OptometerStartThread()
{
	if (m_OptometerThread != NULL)
	{
		if (::WaitForSingleObject(m_OptometerThread, 0) == WAIT_OBJECT_0)
		{
			::CloseHandle(m_OptometerThread);
			m_OptometerThread = NULL;
		}
		else return FALSE;
	}

	ulong OptometerThreadID;
	m_OptometerThread = ::CreateThread(NULL, 0, OptometerThreadFunction, this, CREATE_SUSPENDED, &OptometerThreadID);

	if (m_OptometerThread != NULL)
	{
		::SetThreadPriority(m_OptometerThread, THREAD_PRIORITY_BELOW_NORMAL);
		::ResumeThread(m_OptometerThread);
		return TRUE;
	}

	return FALSE;
}

//***************************************************************************************

void CHW::OptometerMain()
{
	TurnAccommodationMotorOn();

	real_t PositionMm = 0.001 * m_OptometerTargetMoveToPositionUm;

	DllMoveAccommodationMotor(PositionMm);

	BOOL Moving = FALSE;
	BOOL LowerLimitEngaged = FALSE;
	BOOL UpperLimitEngaged = FALSE;

	for (int i = 0; i < 100; i++)
	{
		if (!DllGetAccommodationMotorStatus(&PositionMm, &Moving, &LowerLimitEngaged, &UpperLimitEngaged)) break;
		if (Moving == FALSE) break;
		::Sleep(100);
	}

	TurnAccommodationMotorOff();

	m_OptometerTargetPositionUm = m_OptometerTargetMoveToPositionUm;
	m_OptometerTargetPositionD = m_OptometerTargetMoveToPositionD;
}

//***************************************************************************************

void CHW::SetPowerState(const int PowerUser, BOOL On)
{
	if (m_hDLL == NULL) return;
	ulong BitMask;

	switch (PowerUser)
	{
	case HW_ACCOMMODATION_MOTOR: BitMask = 0x00000001; break;
	case HW_COMPENSATION_MOTOR: BitMask = 0x00000002; break;
	case HW_AODS: BitMask = 0x00000004; break;
	case HW_VIDEO_CAMERA: BitMask = 0x00000008; break;
	case HW_VISTA_HAND_UNIT: BitMask = 0x00000010; break;
	case HW_TRIANGULATION_LASER: BitMask = 0x00000020; break;
	case HW_PLACIDO: BitMask = 0x00000040; break;
	case HW_LADS: BitMask = 0x00000080; break;
	case HW_SCANNING_LASER: BitMask = 0x00000100; break;
	}

	DllSetPowerState(On ? 0xffffffff : 0x00000000, BitMask);
}

//***************************************************************************************

void CHW::SetLightSourcePowerLevel(const int LightSource, int PowerLevel)
{
	if (m_hDLL == NULL) return;

	if (PowerLevel < 0) PowerLevel = 0;
	else if (PowerLevel > 100) PowerLevel = 100;

	LIGHTSOURCES_POWERLEVELS PowerLevels = { -1, -1, -1, -1, -1, -1, -1, -1 };
	switch (LightSource)
	{
	case HW_INFRARED_LEDS: PowerLevels.InfraredLEDs = PowerLevel; break;
	case HW_WHITE_LEDS: PowerLevels.WhiteLEDs = PowerLevel; break;
	case HW_ACCOMMODATION_TARGET: PowerLevels.AccommodationTarget = PowerLevel; break;
	case HW_FAR_FIXATION_TARGET: PowerLevels.FarFixationTarget = PowerLevel; break;
	case HW_NEAR_FIXATION_TARGET: PowerLevels.NearFixationTarget = PowerLevel; break;
	default: return;
	}

	DllSetLightSourcesPowerLevels(&PowerLevels);
}

//***************************************************************************************

BOOL CHW::CompareFirmware(const CString& FileName)
{
	if (m_hDLL == NULL) return FALSE;

	wchar_t _FileName[1024];
	memset(_FileName, 0, sizeof(_FileName));

	for (int i = 0; i < FileName.GetLength(); i++)
	{
		_FileName[i] = wchar_t(FileName[i]);
	}

	int Result;
	int ErrorCode = 0;
	ushort FileVersion;
	ushort FlashVersion;
	CString s;

	if (DllCompareFirmware(_FileName, &Result, &ErrorCode, &FileVersion, &FlashVersion))
	{
		if (ErrorCode == 0)
		{
			s.Format(_T("File version: %i.\nFlash version: %i."), FileVersion, FlashVersion);
			::Info(s);
			return TRUE;
		}
		else
		{
			s.Format(_T("Failed to compare firmware.\nError code: %i."), ErrorCode);
			::Error(s);
			return FALSE;
		}
	}
	else
	{
		::Error("Failed to compare firmware.");
		return FALSE;
	}
}

//***************************************************************************************

BOOL CHW::UpdateFirmware(const CString& FileName)
{
	if (m_hDLL == NULL) return FALSE;

	wchar_t _FileName[1024];
	memset(_FileName, 0, sizeof(_FileName));

	for (int i = 0; i < FileName.GetLength(); i++)
	{
		_FileName[i] = wchar_t(FileName[i]);
	}

	int ErrorCode = 0;
	CString s;

	if (DllUpdateFirmware(_FileName, &ErrorCode))
	{
		if (ErrorCode == 0)
		{
			::Info("Firmware successfully updated.");
			return TRUE;
		}
		else
		{
			CString s;
			s.Format(_T("Failed to update firmware.\nError code: %i."), ErrorCode);
			return FALSE;
		}
	}
	else
	{
		s.Format(_T("Failed to update firmware."));
		return FALSE;
	}
}

//***************************************************************************************
