//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Data.h"
#include <memory>
#include "HardwareHiresCamera.h"
#include <libs/CommonLib/Singleton.h>

//***************************************************************************************

#define DEVICE_IGNORE    0
#define DEVICE_ARRIVED   1
#define DEVICE_REMOVED   2

#define HW_ACCOMMODATION_MOTOR     1
#define HW_COMPENSATION_MOTOR      2
#define HW_AODS                    3
#define HW_VIDEO_CAMERA            4
#define HW_VISTA_HAND_UNIT         5
#define HW_TRIANGULATION_LASER     6
#define HW_PLACIDO                 7
#define HW_LADS                    8
#define HW_SCANNING_LASER          9

#define HW_INFRARED_LEDS           1
#define HW_WHITE_LEDS              2
#define HW_ACCOMMODATION_TARGET    3
#define HW_FAR_FIXATION_TARGET     4
#define HW_NEAR_FIXATION_TARGET    5

//***************************************************************************************

#pragma pack(push)
#pragma pack(8)

typedef struct _LIGHTSOURCES_POWERLEVELS
{
	int     InfraredLEDs;
	int     WhiteLEDs;
	int     AccommodationTarget;
	int     FarFixationTarget;
	int     NearFixationTarget;
	int     Reserved3;
	int     Reserved2;
	int     Reserved1;

} LIGHTSOURCES_POWERLEVELS;

typedef struct _AOD_CONFIG
{
	int     XHz;
	int     YHz;
	int     XPower;
	int     YPower;

} AOD_CONFIG;

typedef struct _VIDEO_FRAME
{

	int     StructSize;
	ulong   Timestamp;
	uchar   Data[487 * 720 * 2];

} VIDEO_FRAME;

typedef struct _DETECTOR_FRAME
{

	int     StructSize;
	ulong   Timestamp;
	int     XHz;
	int     YHz;
	short   XData[512];
	short   YData[512];

} DETECTOR_FRAME;

#pragma pack(pop)

//***************************************************************************************

typedef void(_cdecl* TDllSetupNotifications)(HWND);
typedef void(_cdecl* TDllClearNotifications)(void);
typedef void(_cdecl* TDllInitializeDeviceStatus)(void);
typedef BOOL(_cdecl* TDllDeviceChange)(uint, ulong);
typedef BOOL(_cdecl* TDllIsConnected)(void);
typedef BOOL(_cdecl* TDllGetFirmwareBuildNumber)(ushort*);
typedef BOOL(_cdecl* TDllCompareFirmware)(const wchar_t*, int*, int*, ushort*, ushort*);
typedef BOOL(_cdecl* TDllUpdateFirmware)(const wchar_t*, int*);
typedef BOOL(_cdecl* TDllReadFlash)(ulong, ulong, uchar*, BOOL, ulong*, ulong*);
typedef BOOL(_cdecl* TDllGetDataFlashSize)(ulong*);
typedef BOOL(_cdecl* TDllReadDataFlash)(uchar*, ulong, ulong, ulong*, ulong*);
typedef BOOL(_cdecl* TDllWriteDataFlash)(uchar*, ulong, ulong, ulong*);
typedef BOOL(_cdecl* TDllIsDataFlashBusy)(BOOL*);
typedef BOOL(_cdecl* TDllSetPowerState)(ulong, ulong);
typedef BOOL(_cdecl* TDllSetLightSourcesPowerLevels)(LIGHTSOURCES_POWERLEVELS*);
typedef BOOL(_cdecl* TDllGetODOSState)(BOOL*);
typedef BOOL(_cdecl* TDllGetButtonState)(int*, BOOL*);
typedef BOOL(_cdecl* TDllGetTriangulationSensorReading)(int*); // range 0 - 65535 (scaled)
typedef BOOL(_cdecl* TDllInitializeAccommodationMotor)(void);
typedef BOOL(_cdecl* TDllIsAccommodationMotorInitializationComplete)(BOOL*, BOOL*);
typedef BOOL(_cdecl* TDllGetAccommodationMotorRange)(double*);
typedef BOOL(_cdecl* TDllMoveAccommodationMotor)(double);
typedef BOOL(_cdecl* TDllGetAccommodationMotorStatus)(double*, BOOL*, BOOL*, BOOL*);
typedef BOOL(_cdecl* TDllInitializeCompensationMotor)(void);
typedef BOOL(_cdecl* TDllIsCompensationMotorInitializationComplete)(BOOL*, BOOL*);
typedef BOOL(_cdecl* TDllGetCompensationMotorRange)(double*);
typedef BOOL(_cdecl* TDllMoveCompensationMotor)(double);
typedef BOOL(_cdecl* TDllGetCompensationMotorStatus)(double*, BOOL*, BOOL*, BOOL*);
typedef BOOL(_cdecl* TDllSetVideoBrightness)(int);
typedef BOOL(_cdecl* TDllSetVideoContrast)(int);
typedef BOOL(_cdecl* TDllSetVideoHue)(int);
typedef BOOL(_cdecl* TDllSetVideoSaturation)(int);
typedef BOOL(_cdecl* TDllGetMostRecentVideoFrame)(BOOL, int, int, BOOL*, VIDEO_FRAME*);
typedef BOOL(_cdecl* TDllSenseHardwareStatus)(BOOL*, BOOL*, BOOL*, BOOL*, BOOL*, BOOL*, BOOL*, BOOL*, BOOL*);
typedef BOOL(_cdecl* TDllStartExamSequence)(int, int, int, int, AOD_CONFIG*, BOOL);
typedef BOOL(_cdecl* TDllIsExamFinished)(BOOL*, BOOL*);
typedef BOOL(_cdecl* TDllGetExamResultData)(int*, int*, int*, void*, void*);
typedef BOOL(_cdecl* TDllStartContinuousScanning)(int, int, AOD_CONFIG*);
typedef BOOL(_cdecl* TDllStopContinuousScanning)(void);
typedef BOOL(_cdecl* TDllSetAOD)(int, int, int, int);
typedef BOOL(_cdecl* TDllReadLAD)(int, short*, short*);
typedef BOOL(_cdecl* TDllIsLaserSafetyEnabled)(BOOL*);

//***************************************************************************************

void ResetCalibration(CALIBRATION* pCalibration);

//***************************************************************************************

class CHW : public Singleton<CHW>
{
  friend class Singleton<CHW>;

	CHW();

public:

	~CHW();

	const static int m_VideoWidth;
	const static int m_VideoHeight;
	const static int m_VideoLineWidth;
	const static int m_VideoSize;

	void LoadDLL();
	void UnloadDLL();

	int DeviceChange(uint EventType, DWORD_PTR Data);
	void SetNotificationWindow(CWnd* pWnd);
	void ReleaseNotificationWindow();
	BOOL IsConnected();
	ulong GetFirmwareBuild();

	BOOL ISVideoCameraOn;
	CALIBRATION m_Calibration;
	BOOL LoadCalibrationFromFlash();
	BOOL SaveCalibrationIntoFlash();
	BOOL LoadCalibrationFromFile(const CString& FileName);
	BOOL SaveCalibrationIntoFile(const CString& FileName);
	void CancelCalibrationChanges();
	BOOL HasCalibrationChanged();
	BOOL HasCalibrationLoaded() { return m_Calibration.CalibrationID != GUID_NULL; }

	VIDEO_SETTINGS* m_pCurrentVideoSettings;
	void ApplyCurrentVideoBrightness();
	void ApplyCurrentVideoContrast();
	void ApplyCurrentVideoHue();
	void ApplyCurrentVideoSaturation();
	void StartTransferringVideoFrame();
	void FinishTransferringVideoFrame();
	void ClearFrames();
	uchar* GetRGBData();

	int CheckLaserSafetyStatus();//6.2.0 Change from void to int

	void InitializeOptometer();
	void MoveOptometerTargetToPositionD(real_t PositionD, BOOL Wait);
	real_t GetOptometerTargetPositionD() { return m_OptometerTargetPositionD; }
	real_t GetOptometerTargetPositionUm() { return m_OptometerTargetPositionUm; }

	int GetEye();
	BOOL IsButtonPressed();
	int GetTriangulationSensorReading();

	BOOL DoWFExam(const int NumPoints, const real_t* pXUm, const real_t* pYUm);
	BOOL GetLADsData(const int NumPoints, ushort* pLADXData, ushort* pLADYData);

	int m_XHz;
	int m_YHz;
	void SetAODsFrequencies(const real_t XUm, const real_t YUm);
	BOOL ReadLADs(ushort* pLADXData, ushort* pLADYData);

	void StartContinuousScanning(const int NumPoints, const real_t* pXUm, const real_t* pYUm);
	void StopContinuousScanning();

	void TurnAccommodationMotorOn() { SetPowerState(HW_ACCOMMODATION_MOTOR, TRUE); }
	void TurnAccommodationMotorOff() { SetPowerState(HW_ACCOMMODATION_MOTOR, FALSE); }
	//void TurnCompensationMotorOn   () {SetPowerState(HW_COMPENSATION_MOTOR , TRUE );}
	//void TurnCompensationMotorOff  () {SetPowerState(HW_COMPENSATION_MOTOR , FALSE);}
	void TurnAODsOn() { SetPowerState(HW_AODS, TRUE); }
	void TurnAODsOff() { SetPowerState(HW_AODS, FALSE); }
	void TurnVideoCameraOn() { SetPowerState(HW_VIDEO_CAMERA, TRUE); }
	void TurnVideoCameraOff() { SetPowerState(HW_VIDEO_CAMERA, FALSE); }
	//void TurnVistaOn               () {SetPowerState(HW_VISTA_HAND_UNIT    , TRUE );}
	//void TurnVistaOff              () {SetPowerState(HW_VISTA_HAND_UNIT    , FALSE);}
	void TurnTriLaserOn() { SetPowerState(HW_TRIANGULATION_LASER, TRUE); }
	void TurnTriLaserOff() { SetPowerState(HW_TRIANGULATION_LASER, FALSE); }
	void TurnPlacidoOn() { SetPowerState(HW_PLACIDO, TRUE); }
	void TurnPlacidoOff() { SetPowerState(HW_PLACIDO, FALSE); }
	void TurnLADsOn() { SetPowerState(HW_LADS, TRUE); }
	void TurnLADsOff() { SetPowerState(HW_LADS, FALSE); }
	void TurnScanningLaserOn() { SetPowerState(HW_SCANNING_LASER, TRUE); }
	void TurnScanningLaserOff() { SetPowerState(HW_SCANNING_LASER, FALSE); }

	void TurnInfraredLEDsOn() { SetLightSourcePowerLevel(HW_INFRARED_LEDS, m_pCurrentVideoSettings->InfraredLEDsPowerLevel); }
	void TurnInfraredLEDsOff() { SetLightSourcePowerLevel(HW_INFRARED_LEDS, 0); }
	void TurnWhiteLEDsOn() { SetLightSourcePowerLevel(HW_WHITE_LEDS, m_pCurrentVideoSettings->WhiteLEDsPowerLevel); }
	void TurnWhiteLEDsOff() { SetLightSourcePowerLevel(HW_WHITE_LEDS, 0); }
	void TurnAccommodationTargetOn() { SetLightSourcePowerLevel(HW_ACCOMMODATION_TARGET, m_Calibration.AccommodationTargetPowerLevel); }
	void TurnAccommodationTargetOff() { SetLightSourcePowerLevel(HW_ACCOMMODATION_TARGET, 0); }
	//void TurnFarTargetOn           () {SetLightSourcePowerLevel(HW_FAR_FIXATION_TARGET , m_Calibration.FarTargetPowerLevel);}
	//void TurnFarTargetOff          () {SetLightSourcePowerLevel(HW_FAR_FIXATION_TARGET , 0);}
	//void TurnNearTargetOn          () {SetLightSourcePowerLevel(HW_NEAR_FIXATION_TARGET, m_Calibration.NearTargetPowerLevel);}
	//void TurnNearTargetOff         () {SetLightSourcePowerLevel(HW_NEAR_FIXATION_TARGET, 0);}

	BOOL CompareFirmware(const CString& FileName);
	BOOL UpdateFirmware(const CString& FileName);

private:
	std::unique_ptr<HardwareHiresCamera> m_hiresCamera;

	int UmToHzX(const real_t XUm);
	int UmToHzY(const real_t YUm);

	ulong m_FlashDataStartAddress;
	CALIBRATION m_LCalibration;
	BOOL LoadCalibrationFromFlash(CALIBRATION* pCalibration);
	BOOL SaveCalibrationIntoFlash(CALIBRATION* pCalibration);

	VIDEO_FRAME m_VideoFrames[2];
	VIDEO_FRAME* m_pTVideoFrame;
	VIDEO_FRAME* m_pDVideoFrame;
	BOOL m_DFrameProcessed;
	void* m_FrameTransferThread;
	void FrameTransferMain();
	static ulong __stdcall FrameTransferThreadFunction(void* pParam) { ((CHW*)pParam)->FrameTransferMain(); return 0; }

	BOOL m_OptometerInitialized;
	real_t m_OptometerTargetRangeUm;
	real_t m_OptometerTargetMoveToPositionUm;
	real_t m_OptometerTargetMoveToPositionD;
	real_t m_OptometerTargetPositionUm;
	real_t m_OptometerTargetPositionD;
	void* m_OptometerThread;
	BOOL OptometerStartThread();
	void OptometerMain();
	static ulong __stdcall OptometerThreadFunction(void* pParam) { ((CHW*)pParam)->OptometerMain(); return 0; }

	void SetPowerState(const int PowerUser, BOOL On);
	void SetLightSourcePowerLevel(const int LightSource, int PowerLevel);

	HMODULE m_hDLL;

	TDllSetupNotifications                         DllSetupNotifications;
	TDllClearNotifications                         DllClearNotifications;
	TDllInitializeDeviceStatus                     DllInitializeDeviceStatus;
	TDllDeviceChange                               DllDeviceChange;
	TDllIsConnected                                DllIsConnected;
	TDllGetFirmwareBuildNumber                     DllGetFirmwareBuildNumber;
	TDllCompareFirmware                            DllCompareFirmware;
	TDllUpdateFirmware                             DllUpdateFirmware;
	TDllReadFlash                                  DllReadFlash;
	TDllGetDataFlashSize                           DllGetDataFlashSize;
	TDllReadDataFlash                              DllReadDataFlash;
	TDllWriteDataFlash                             DllWriteDataFlash;
	TDllIsDataFlashBusy                            DllIsDataFlashBusy;
	TDllSetPowerState                              DllSetPowerState;
	TDllSetLightSourcesPowerLevels                 DllSetLightSourcesPowerLevels;
	TDllGetODOSState                               DllGetODOSState;
	TDllGetButtonState                             DllGetButtonState;
	TDllGetTriangulationSensorReading              DllGetTriangulationSensorReading;
	TDllInitializeAccommodationMotor               DllInitializeAccommodationMotor;
	TDllIsAccommodationMotorInitializationComplete DllIsAccommodationMotorInitializationComplete;
	TDllGetAccommodationMotorRange                 DllGetAccommodationMotorRange;
	TDllMoveAccommodationMotor                     DllMoveAccommodationMotor;
	TDllGetAccommodationMotorStatus                DllGetAccommodationMotorStatus;
	TDllInitializeCompensationMotor                DllInitializeCompensationMotor;
	TDllIsCompensationMotorInitializationComplete  DllIsCompensationMotorInitializationComplete;
	TDllGetCompensationMotorRange                  DllGetCompensationMotorRange;
	TDllMoveCompensationMotor                      DllMoveCompensationMotor;
	TDllGetCompensationMotorStatus                 DllGetCompensationMotorStatus;
	TDllSetVideoBrightness                         DllSetVideoBrightness;
	TDllSetVideoContrast                           DllSetVideoContrast;
	TDllSetVideoHue                                DllSetVideoHue;
	TDllSetVideoSaturation                         DllSetVideoSaturation;
	TDllGetMostRecentVideoFrame                    DllGetMostRecentVideoFrame;
	TDllSenseHardwareStatus                        DllSenseHardwareStatus;
	TDllStartExamSequence                          DllStartExamSequence;
	TDllIsExamFinished                             DllIsExamFinished;
	TDllGetExamResultData                          DllGetExamResultData;
	TDllStartContinuousScanning                    DllStartContinuousScanning;
	TDllStopContinuousScanning                     DllStopContinuousScanning;
	TDllSetAOD                                     DllSetAOD;
	TDllReadLAD                                    DllReadLAD;
	TDllIsLaserSafetyEnabled                       DllIsLaserSafetyEnabled;

public:
  static CHW &TheOneAndOnly()
  {
    return *Instance();
  }
};

//***************************************************************************************

// extern CHW HW;

#define HW CHW::TheOneAndOnly()

//***************************************************************************************
