//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Image.h"
#include "Surface.h"
#include "libs/CommonLib/Singleton.h"

//***************************************************************************************

#define DATABASE_TYPE_MSJET                0
#define DATABASE_TYPE_MYSQL                1
#define DATABASE_TYPE_MSJET_OLD            2

#define EXAM_TYPE_UNKNOWN                  0
#define EXAM_TYPE_WF                       1
#define EXAM_TYPE_CT                       2

#define EXAM_MODE_UNKNOWN                  0
#define EXAM_MODE_AUTO                     1
#define EXAM_MODE_MANUAL                   2
#define EXAM_MODE_MANUAL_PRESBIA64         3
#define EXAM_MODE_MANUAL_PRESBIA256        4

#define SEX_UNKNOWN                        0
#define SEX_MALE                           1
#define SEX_FEMALE                         2

#define EYE_OD                             1
#define EYE_OS                             2
#define EYE_OD_OS                          3
#define EYE_OS_OD                          4
#define EYE_BOTH                           5 //For Depth of Focus Comparison

#define PREOP_UNKNOWN                      0
#define PREOP_YES                          1
#define PREOP_NO                           2

#define WAVETOUCH_LENS_BASECURVE_UNKNOWN   0
#define WAVETOUCH_LENS_BASECURVE_MEDIAN    1
#define WAVETOUCH_LENS_BASECURVE_STEEP     2

#define DATE_FORMAT_MM_DD_YYYY             0
#define DATE_FORMAT_DD_MM_YYYY             1
#define DATE_FORMAT_YYYY_MM_DD             2

#define REF_SURFACE_BEST_SPHERE            0
#define REF_SURFACE_BEST_CONIC             1
#define REF_SURFACE_USER_CONIC             2

#define ALIGNMENT_PUPIL_CENTER             0
#define ALIGNMENT_4_IR_DOTS                1

#define ISOD(Eye) ((Eye) == EYE_OD || (Eye) == EYE_OS_OD )
#define ISOS(Eye) ((Eye) == EYE_OS || (Eye) == EYE_OD_OS )
#define ISBOTH(Eye) ((Eye) == EYE_BOTH )   //For Depth of Focus Comparison

//***************************************************************************************

#pragma pack(push)
#pragma pack(1)

//***************************************************************************************

typedef struct _TDB_ADDRESS
{
	char            Line1[51];
	char            Line2[51];
	char            City[51];
	char            State[51]; // Region/province for non-US addresses is stored here
	char            ZIP[21];
	char            Country[51];

} TDB_ADDRESS;

//***************************************************************************************

typedef struct _TDB_GROUP
{
	GUID            GroupID;
	char            Name[51];

} TDB_GROUP;

//***************************************************************************************

typedef struct _TDB_CLINIC
{
	GUID            ClinicID;
	char            Name[51];
	TDB_ADDRESS     Address;
} TDB_CLINIC;

//***************************************************************************************

typedef struct _TDB_PHYSICIAN
{
	GUID            PhysicianID;
	char            LastName[51];
	char            FirstName[51];
} TDB_PHYSICIAN;

//***************************************************************************************

typedef struct _TDB_OPERATOR
{
	GUID            OperatorID;
	char            LastName[51];
	char            FirstName[51];
} TDB_OPERATOR;

//***************************************************************************************

typedef struct _TDB_EYE
{
	GUID            EyeID;
	GUID            PatientID;
	int             Type;
	real_t            ManifestVertex;
	real_t            ManifestSphere;
	real_t            ManifestCylinder;
	int             ManifestAxis;
	real_t            K1;
	real_t            K2;
	int             K2Axis;
} TDB_EYE;

//***************************************************************************************

typedef struct _TDB_PATIENT
{
	GUID            PatientID;
	GUID            GroupID;
	char            CustomID[51];
	char            LastName[51];
	char            FirstName[51];
	char            MiddleName[51];
	int             BirthYear;
	int             BirthMonth;
	int             BirthDay;
	int             Sex;
	TDB_ADDRESS     Address;
	char            HomePhone[21];
	char            WorkPhone[21];
	char            EMail[51];
	char            Note[101];
	int             RegYear;
	int             RegMonth;
	int             RegDay;
	int             RegHour;
	int             RegMinute;
	int             IsDicom;//6.2.0 For Dicom

} TDB_PATIENT;

//***************************************************************************************

typedef struct _TDB_EXAM_HEADER
{
	GUID            ExamID;
	GUID            PatientID;
	GUID            CalibrationID;
	GUID            ClinicID;
	GUID            PhysicianID;
	GUID            OperatorID;
	int             Type;
	int             Year;
	int             Month;
	int             Day;
	int             Hour;
	int             Minute;
	int             Second;
	int             Eye;
	int             Preop;
	int             SeriesNumber;
	int             Mode;
	uint            SoftwareVersion;
  uint            legacy : 1, analogCamera : 1;
	int             Reserved4;
	char            Note[101];

} TDB_EXAM_HEADER;

//***************************************************************************************

typedef struct _TDB_WF_EXAM_DATA
{
	int             StructSize;

	real_t            ScanDiameter;
	real_t            AccommTargetPosition;
	real_t            PrecalcSphere;
	real_t            PrecalcCylinder;
	int             PrecalcAxis;
	real_t            WavetouchLensPower;
	int             WavetouchLensBaseCurve;
	int             DetectorType;
	int             NumPoints;
	int             PointSize;
	int             NumImages;                //     68 bytes

	uchar           WavetouchLensDotsOK;
	real_t            WavetouchLensDotX[3];
	real_t            WavetouchLensDotY[3];     //    117 bytes

	uchar           VertexOK;
	real_t            VertexX;
	real_t            VertexY;                  //    134 bytes

	uchar           PupilOK;
	real_t            PupilX0;
	real_t            PupilY0;
	real_t            PupilR[360];
	real_t            PupilRMin;
	real_t            PupilRMax;
	real_t            PupilRMean;               //  3,055 bytes

	uchar           AODsOK;                   //  3,056 bytes

	uchar           LimbusOK;
	real_t            LimbusX0;
	real_t            LimbusY0;
	real_t            LimbusR[360];
	real_t            LimbusRMin;
	real_t            LimbusRMax;
	real_t            LimbusRMean;              //  5,977 bytes


	int             SaveYear;
	int             SaveMonth;
	int             SaveDay;
	int             SaveHour;
	int             SaveMinute;
	int             SaveSecond;               //  6,001 bytes

	char            WavetouchOther[21];
	char            WavetouchNote[101];
	real_t            WavetouchVertex;
	real_t            WavetouchSph;
	real_t            WavetouchCyl;
	int             WavetouchAxis;            //  6,151 bytes

	uchar           AlignmentMethod;          //  6,152 bytes

	GUID            FellowExamID;             //  6,168 bytes

	real_t            ScanX0;
	real_t            ScanY0;                   //  6,184 bytes

											  //Radial Ruler, used in the Angle K/A
	uchar           RadialRulerOk;           //   1 byte
	real_t            RadialRulerX0;           //   8 bytes
	real_t            RadialRulerY0;           //   8 bytes
	real_t            RadialRulerR;            //   8 + 8 + 8 +1 + 6184 = 6209 bytes

											 //Inlay, used in the Angle K/A
	uchar           InlayOk;                 //   1 byte
	real_t            InlayX0;                 //   8 bytes
	real_t            InlayY0;                 //   8 bytes
	real_t            InlayR0;                 //   8 bytes + 8 + 8 + 1 + 6209 = 6234 bytes
	real_t            InlayInR0;               //   8 bytes + 6234 = 6242 bytes  ***cjf***

											 //Linear Ruler, used in the Angle K/A
	uchar           LinearRulerOk;           //   1 byte
	real_t            LinearRulerX0;           //   8 bytes
	real_t            LinearRulerY0;           //   8 bytes
	real_t            LinearRulerX1;           //   8 bytes
	real_t            LinearRulerY1;           //   8 + 8 + 8 + 8 + 1 + 6242 = 6275 bytes

} TDB_WF_EXAM_DATA;

//***************************************************************************************

typedef struct _TDB_CT_EXAM_DATA
{
	int             StructSize;

	int             NumImages;                //      8 bytes

	uchar           LimbusOK;
	real_t            LimbusX0;
	real_t            LimbusY0;
	real_t            LimbusR[360];
	real_t            LimbusRMin;
	real_t            LimbusRMax;
	real_t            LimbusRMean;              //  2,929 bytes

	uchar           PupilOK;
	real_t            PupilX0;
	real_t            PupilY0;
	real_t            PupilR[360];
	real_t            PupilRMin;
	real_t            PupilRMax;
	real_t            PupilRMean;               //  5,850 bytes

	uchar           VertexOK;
	real_t            VertexX;
	real_t            VertexY;
	uchar           RingsOK;
	real_t            RingsR[26][360];          // 80,748 bytes

	int             SaveYear;
	int             SaveMonth;
	int             SaveDay;
	int             SaveHour;
	int             SaveMinute;
	int             SaveSecond;               // 80,772 bytes

	GUID            FellowExamID;             // 80,788 bytes

											  //for gray image disk and rulers
											  //Radial ruler
	uchar           RadialRulerOk;            //   1 bytes
	real_t            RadialRulerX0;            //   8 bytes
	real_t            RadialRulerY0;            //   8 bytes
	real_t            RadialRulerR;             //   8 + 8 + 8 +1 + 80,788  = 80,813 bytes

											  //Inlay, used in the Angle K/A
	uchar           InlayOk;                  //   1 byte
	real_t            InlayX0;                  //   8 bytes
	real_t            InlayY0;                  //   8 bytes
	real_t            InlayR0;                  //   8 + 8 + 8 + 1 + 80,813 = 80,838 bytes
	real_t            InlayInR0;                //   530 8 bytes + 80,838 = 80,846 bytes  ***cjf***

											  //Linear Ruler, used in the Angle K/A
	uchar           LinearRulerOk;            //   1 byte
	real_t            LinearRulerX0;            //   8 bytes
	real_t            LinearRulerY0;            //   8 bytes
	real_t            LinearRulerX1;            //   8 bytes
	real_t            LinearRulerY1;            //   8 + 8 + 8 + 8 + 1 + 80,846 = 80,879 bytes

											  //for color image info
	uchar           ColorLimbusOK;            // 1 bytes
	real_t            ColorLimbusX0;            // 8 bytes
	real_t            ColorLimbusY0;            // 8 bytes
	real_t            ColorLimbusR[360];        // 8 bytes * 360 = 2880 bytes
	real_t            ColorLimbusRMin;          // 8 bytes
	real_t            ColorLimbusRMax;          // 8 bytes
	real_t            ColorLimbusRMean;         // 80,879 bytes + 1 + 8 + 8 + 2880 + 8 + 8 + 8 = 83,800 bytes

	uchar           ColorPupilOK;             // 1 bytes
	real_t            ColorPupilX0;             // 8 bytes
	real_t            ColorPupilY0;             // 8 bytes
	real_t            ColorPupilR[360];         // 8 bytes * 360 = 2880 bytes
	real_t            ColorPupilRMin;           // 8 bytes
	real_t            ColorPupilRMax;           // 8 bytes
	real_t            ColorPupilRMean;          // 83,800 bytes + 2921 bytes = 86,721 bytes


	uchar           ColorVertexOK;            // 1 bytes
	real_t            ColorVertexX;             // 8 bytes
	real_t            ColorVertexY;             // 86,721 bytes + 1 + 8 + 8 = 86,738 bytes

											  //Radial Ruler, used in the Angle K/A
	uchar           ColorRadialRulerOk;       //   1 bytes
	real_t            ColorRadialRulerX0;       //   8 bytes
	real_t            ColorRadialRulerY0;       //   8 bytes
	real_t            ColorRadialRulerR;        //   8 + 8 + 8 +1 + 86,738  = 86,763 bytes

											  //Inlay, used in the Angle K/A
	uchar           ColorInlayOk;             //   1 byte
	real_t            ColorInlayX0;             //   8 bytes
	real_t            ColorInlayY0;             //   8 bytes
	real_t            ColorInlayR0;             //   8 + 8 + 8 + 1 + 86,763 = 86,788 bytes
	real_t            ColorInlayInR0;           //   530 8 bytes + 86,788 = 86,796 bytes  ***cjf***

											  //Linear Ruler, used in the Angle K/A
	uchar           ColorLinearRulerOk;       //   1 byte
	real_t            ColorLinearRulerX0;       //   8 bytes
	real_t            ColorLinearRulerY0;       //   8 bytes
	real_t            ColorLinearRulerX1;       //   8 bytes
	real_t            ColorLinearRulerY1;       //   8 + 8 + 8 + 8 + 1 + 86,796 = 86,829 bytes

} TDB_CT_EXAM_DATA;

//***************************************************************************************

typedef struct _TDB_POINT_0
{
	int             ShotNumber;
	int             Bad;
	real_t            Radius;
	real_t            Angle;
	real_t            DX;
	real_t            DY;

} TDB_POINT_0;

//***************************************************************************************

typedef struct _TDB_POINT_1
{
	int             ShotNumber;
	int             Bad;
	real_t          Radius;
	real_t          Angle;
	uchar           DataX[1024];
	uchar           DataY[1024];

} TDB_POINT_1;

//***************************************************************************************

typedef struct _VIDEO_SETTINGS
{
  BOOL Color;
  int  InfraredLEDsPowerLevel;
  int  WhiteLEDsPowerLevel;
  int  Brightness;
  int  Contrast;
  union {
    int      Hue;
    struct Gain
    {
		unsigned Red : 8, Green : 8, Blue : 8;
    } gain;
  };

  int Saturation;

  // high resolution camera image capture setting
  //int Red;
  //int Green;
  //int Blue;
  // high resolution camera image capture setting

} VIDEO_SETTINGS;

//***************************************************************************************

typedef struct _AOD_SETTINGS
{
	int                Freq0;
	int                HzUm;
	int                RMS;

} AOD_SETTINGS;

//***************************************************************************************

typedef struct _ZERNIKE_SETTINGS
{
	real_t               C3;
	real_t               C4;
	real_t               C5;

} ZERNIKE_SETTINGS;

//***************************************************************************************

typedef struct _CALIBRATION
{
	GUID               CalibrationID;

	char               HardwareSerial[28];
	SYSTEMTIME         Time;

	real_t               LaserWavelength;

	int                NumPoints;

	int                OptometerEnabled;

	AOD_SETTINGS       AODX;
	AOD_SETTINGS       AODY;

	int                LADExposureTime; // µs
	BOOL               LADSwapXY;
	BOOL               LADXNoFlip;
	real_t               LADXFocalDistance;
	BOOL               LADYNoFlip;
	real_t               LADYFocalDistance;

	int                OptometerTable[21]; // D -> µ
	int                OptometerGap;

	int                LaserSpotRadiusX;
	int                LaserSpotRadiusY;
	int                IntensityThreshold1;
	int                LaserSpotRingRadius;
	int                IntensityThreshold2;

	BOOL               AODXNoFlip;
	BOOL               AODYNoFlip;

	int                LaserDuration;
	int                LaserIntensityThreshold;
	int                ZAlignmentMethod;
	int                SensorThreshold;

	int                NumConsecFrames;
	int                AllowedDecentError;
	int                AllowedMovement;

	uchar              Reserved1[340];

	BOOL               VideoFlipX;
	BOOL               VideoFlipY;
	int                VideoWidth;
	int                VideoHeight;
	real_t               VideoWidthMicrons;
	real_t               VideoHeightMicrons;

	uchar              StepLeft;//for camers frame move left (step 1pixel)
	uchar              StepRight;//for camers frame move right (step 1pixel)
	uchar              StepUp;//for camers frame move up (step 1pixel)
	uchar              StepDown;//for camers frame move down (step 1pixel)
	uchar              Reserved2[24];

	VIDEO_SETTINGS     WFVideoSettings;
	VIDEO_SETTINGS     WFVideo2Settings;
	VIDEO_SETTINGS     CTVideoSettings;

	ZERNIKE_SETTINGS   Zernike;

	int                ColorImageDelay;
	int                AccommodationTargetPowerLevel;
	int                FarTargetPowerLevel;
	int                NearTargetPowerLevel;

	uchar              Reserved3[4];

	int                NumRings;
	real_t               BallsAx[4];
	real_t               BallsRi[4][26][360];

} CALIBRATION;

//***************************************************************************************

////Big Database Research since 03062019
//typedef struct _ExamHeader_STR
//{
//	CStringA     ExamID_Str;
//	CStringA     CalibrationID_Str;
//	CStringA     ClinicIDStr;
//	CStringA     PhysicianID_Str;
//	CStringA     OperatorID_Str;
//	CStringA     Type_Str;
//	CStringA     Mode_Str;
//	CStringA     Year_Str;
//	CStringA     Month_Str;
//	CStringA     Day_Str;
//	CStringA     Hour_Str;
//	CStringA     Minute_Str;
//	CStringA     Second_Str;
//	CStringA     Eye_Str;
//	CStringA     Preop_Str;
//	CStringA     SeriesNumber_Str;
//	CStringA     Note_Str;
//	CStringA     SoftwareVersion_Str;
//	CStringA     Reserved3_Str;
//	CStringA     Reserved4_Str;
//} ExamHeader_STR;
////Big Database Research since 03062019

#pragma pack(pop)

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

class CAddress
{
public:

	CAddress();

	CString     m_Line1;
	CString     m_Line2;
	CString     m_City;
	CString     m_State;
	CString     m_ZIP;
	CString     m_Country;

	void Reset();
};

//*************************************************************************************

class CGroup
{
public:

	CGroup();

	GUID        m_GroupID;
	CString     m_Name;

	void Reset();
};

//***************************************************************************************

class CClinic
{
public:

	CClinic();

	GUID        m_ClinicID;
	CString     m_Name;
	CAddress    m_Address;

	void Reset();
};

//***************************************************************************************

class CPhysician
{
public:

	CPhysician();

	GUID        m_PhysicianID;
	CString     m_LastName;
	CString     m_FirstName;

	void Reset();
	CString FullName();
};

//***************************************************************************************

class COperator
{
public:

	COperator();

	GUID        m_OperatorID;
	CString     m_LastName;
	CString     m_FirstName;

	void Reset();
	CString FullName();
};

//***************************************************************************************

class CEye
{
public:

	CEye();

	GUID        m_EyeID;
	GUID        m_PatientID;
	int         m_Type;
	real_t        m_ManifestVertex;
	real_t        m_ManifestSphere;
	real_t        m_ManifestCylinder;
	int         m_ManifestAxis;
	real_t        m_K1;
	real_t        m_K2;
	int         m_K2Axis;

	void Reset();
};

//***************************************************************************************

class CPatient
{
public:

	CPatient();

	GUID        m_PatientID;
	GUID        m_GroupID;
	CString     m_CustomID;
	CString     m_LastName;
	CString     m_FirstName;
	CString     m_MiddleName;
	int         m_BirthYear;
	int         m_BirthMonth;
	int         m_BirthDay;
	int         m_Sex;
	CAddress    m_Address;
	CString     m_HomePhone;
	CString     m_WorkPhone;
	CString     m_EMail;
	CString     m_Note;
	int         m_RegYear;
	int         m_RegMonth;
	int         m_RegDay;
	int         m_RegHour;
	int         m_RegMinute;
	int         m_IsDicom; //6.2.0 For Dicom

	void Reset();

	CString FullName();
};

//***************************************************************************************
//The basic info for Dicom file
//Some info about DICOM,
//1. The DICOM used the open source library 'DCMTK'
//2. The debug and release version of the program used different DCMTK library
//3. The debug DCMTK library is under the 'c:/dcmtk/lib/debug'
//4. The relaese DCMTK library is under the 'c:/dcmtk/lib/release'
//5. Set the library link in the 'Tools->options->vc++ directory -> library files'

class CDicomInfo
{
public:
	CString     m_FileName;

	//Patient
	CString     m_Patient_Name;
	CString     m_Patient_ID;
	CString     m_Patient_Other_ID;//OtherPatientIDs
	CString     m_Patient_BirthDate;
	CString     m_Patient_Sex;
	//Done

	//Add new item here
	//m_Physician_Name
	//m_Study_ID
	//m_Series_Instance_UID//can not be empty. This could also be the EXAM GUID field.
	//m_Series_Num
	//m_Conversion_Type// can not be empty.
	CString     m_Acess_Num;
	CString     m_Laterality;
	CString     m_Instance_Num;
	CString     m_Patient_Orientation;
	//new item

	//General study
	CString     m_Study_Date;//Exam date
	CString     m_Study_Time;
	CString     m_Study_ID;
	CString     m_Study_Instance_ID;
	CString     m_Physician_Name;
	CString     m_StudyDes; //Stduy Description =""
	//General study Done

	//Series
	CString     m_Series_Date;
	CString     m_Series_Time;
	CString     m_Series_Instance_UID;
	CString     m_Series_Num;
	CString     m_Series_Laterality;
	//Series Done

	//Equipment
	CString     m_Equi_Manufacturer;
	CString     m_Equi_Institution_Name; //clinic
	CString     m_Equi_Manufacture_Model;
	CString     m_Equi_Serial_Num;
	CString     m_Software_Versions;
	//Equipment Done

	//SC Equipment
	CString     m_Modality;
	CString     m_Conversion_Type;
	//SC Equipment Done

	//sop and scu
	CString     m_SopInstanceUID;
	CString     m_SopClassUid;
	//sop and scuDone

	////6.2.0 New Items here
	//CString     m_Acess_Num;
	//CString     m_Laterality;
	//CString     m_Instance_Num;
	//CString     m_Patient_Orientation;
	////6.2.0 New Items here
};

//***************************************************************************************

class CExamHeader
{
public:

	CExamHeader();

  GUID    m_ExamID;
  GUID    m_FellowExamID;
  GUID    m_PatientID;
  GUID    m_CalibrationID;
  GUID    m_ClinicID;
  GUID    m_PhysicianID;
  GUID    m_OperatorID;
  int     m_Type;
  int     m_Mode;
  int     m_Year;
  int     m_Month;
  int     m_Day;
  int     m_Hour;
  int     m_Minute;
  int     m_Second;
  int     m_Eye;
  int     m_Preop;
  int     m_SeriesNumber;
  CString m_Note;
  uint    m_SoftwareVersion;
  int     m_SaveYear;
  int     m_SaveMonth;
  int     m_SaveDay;
  int     m_SaveHour;
  int     m_SaveMinute;
  int     m_SaveSecond;
  int     m_Number;
  BOOL    m_Saved;

  CString m_AutoNote;
  BOOL    m_AngleKappaOK;
  real_t  m_AngleKappaRUm;
  int     m_AngleKappaADg;
  real_t  m_AngleKappaXUm;
  real_t  m_AngleKappaYUm;
  BOOL    m_AngleAlphaOK;
  real_t  m_AngleAlphaRUm;
  int     m_AngleAlphaADg;
  real_t  m_AngleAlphaXUm;
  real_t  m_AngleAlphaYUm;

  // Remmber add this into the itrace TPS
  BOOL   m_LinearRulerOK;
  real_t m_LinearRulerX0Um;
  real_t m_LinearRulerY0Um;
  real_t m_LinearRulerX1Um;
  real_t m_LinearRulerY1Um;

  BOOL   m_AngleRulerOK;
  real_t m_AngleRulerRUm;
  int    m_AngleRulerADg;
  real_t m_AngleRulerXUm;
  real_t m_AngleRulerYUm;

	BOOL   m_InlayOK;
  real_t m_InlayAngleRUm;
  int    m_InlayAngleADg;
  real_t m_InlayXUm;
  real_t m_InlayYUm;

	void Reset();

	BOOL IsWF() const { return m_Type == EXAM_TYPE_WF; }
	BOOL IsCT() const { return m_Type == EXAM_TYPE_CT; }

	BOOL IsOD() const { return ISOD(m_Eye); }
	BOOL IsOS() const { return ISOS(m_Eye); }
	const char* Eye() { if (IsOD()) return "OD"; if (IsOS()) return "OS"; return ""; }
	BOOL IsEyeManuallyOverriden() { return m_Eye == EYE_OD_OS || m_Eye == EYE_OS_OD; }

	const char* Preop();
	const char* SoftwareVersion();
	const char* Type() { return m_Type == EXAM_TYPE_WF ? "Wavefront" : (m_Type == EXAM_TYPE_CT ? "Corneal Topography" : ""); }
	const char* Mode() { return m_Mode == EXAM_MODE_AUTO ? "Auto" : (m_Mode == EXAM_MODE_MANUAL ? "Manual" : (m_Mode == EXAM_MODE_MANUAL_PRESBIA64 ? "Presbia64" : (m_Mode == EXAM_MODE_MANUAL_PRESBIA256 ? "Presbia256" : ""))); }
};

//***************************************************************************************

class CIOL
{
public:

	CString m_Name;
	real_t m_Cyl;
	real_t m_InternalPostopCyl;
	int  m_InternalPostopAxis;
	real_t m_EyePostopCyl;
	int  m_EyePostopAxis;
};

//***************************************************************************************

class CICL
{
public:

	real_t m_Cyl;
	int  m_Axis;
	int  m_PlacementAxis;
	real_t m_InducedCyl;
	int  m_InducedAxis;
};

//***************************************************************************************

void CalculateCrossedAstigmatism(const real_t Cyl1, const int Axis1, const real_t Cyl2, const int Axis2, real_t& Cyl, int& Axis);

//***************************************************************************************

class COpData
{
public:

	COpData();

	BOOL m_OK;

	BOOL m_Pri_OK;// Cyclinder adjustment Branch

	int  m_OpType;

	int  m_RefAxis[2];
	real_t m_RefLength[4];// Add the protractor length

	// Pre-op
	real_t m_CorneaPreopCylSimK;
	int  m_CorneaPreopAxisSimK;
	real_t m_CorneaPreopCylWF;
	int  m_CorneaPreopAxisWF;
	real_t m_CorneaPreopCyl;
	int  m_CorneaPreopAxis;
	real_t m_InternalPreopCyl;
	int  m_InternalPreopAxis;
	real_t m_EyePreopCylWF;
	int  m_EyePreopAxisWF;
	real_t m_EyePreopCyl;
	int  m_EyePreopAxis;

	//
	real_t m_Pri_CorneaPreopCylSimK;
	int  m_Pri_CorneaPreopAxisSimK;
	real_t m_Pri_CorneaPreopCylWF;
	int  m_Pri_CorneaPreopAxisWF;
	real_t m_Pri_CorneaPreopCyl;
	int  m_Pri_CorneaPreopAxis;
	real_t m_Pri_InternalPreopCyl;
	int  m_Pri_InternalPreopAxis;
	real_t m_Pri_EyePreopCylWF;
	int  m_Pri_EyePreopAxisWF;
	real_t m_Pri_EyePreopCyl;
	int  m_Pri_EyePreopAxis;

	void CalNewCorneaPreop();
	// Cyclinder adjustment Branch

	// IOL
	BOOL m_CorneaCycliderAdj;// Cyclinder adjustment Branch
	BOOL m_Ori_CorneaCycliderAdj;// Cyclinder adjustment Branch
	BOOL m_CorneaCycliderAdjCaled;// Cyclinder adjustment Branch

	int  m_CorneaIncisionAxis;
	real_t m_CorneaInducedCyl;
	int  m_CorneaInducedAxis;
	real_t m_IOLSph;
	int  m_IOLPlacementAxis;
	CIOL m_NewIOLs[31];//7.0.0
	int  m_SelectedIOL;
	int  m_NewBestIOL;//7.0.0
	int  m_NewBestIOLPos;//7.0.0
	int  m_FirstIOLInGui;//7.0.0

	// Cyclinder adjustment Branch
	int  m_Pri_CorneaIncisionAxis;
	real_t m_Pri_CorneaInducedCyl;
	int  m_Pri_CorneaInducedAxis;
	real_t m_Pri_IOLSph;
	int  m_Pri_IOLPlacementAxis;
	//CIOL m_Pri_IOLs[12];
	CIOL m_NewPri_IOLs[31];//7.0.0
	int  m_Pri_SelectedIOL;
	//int  m_Pri_BestIOL;
	int  m_NewPri_BestIOL;//7.0.0
	// Cyclinder adjustment Branch

	// ICL
	CICL m_ICL;

	void InitIOLs();//7.0.0

	// Post-op
	real_t m_CorneaPostopCyl;
	int  m_CorneaPostopAxis;

	real_t m_InternalPostopCyl;
	int  m_InternalPostopAxis;
	real_t m_EyePostopCyl;
	int  m_EyePostopAxis;

	void ResetCalculatedStuff();
	void Recalculate();

	// Cyclinder adjustment Branch
	CICL m_Pri_ICL;

	real_t m_Pri_CorneaPostopCyl;
	int  m_Pri_CorneaPostopAxis;

	real_t m_Pri_InternalPostopCyl;
	int  m_Pri_InternalPostopAxis;
	real_t m_Pri_EyePostopCyl;
	int  m_Pri_EyePostopAxis;

	void Reset_Pri_CalculatedStuff();
	void Pri_Recalculate();
	// Cyclinder adjustment Branch
};

//***************************************************************************************

class CExam
{
public:

	CExam() { m_OK = FALSE; }
	virtual ~CExam() {}

	BOOL m_OK;
	CExamHeader m_Header;
	CALIBRATION m_Calibration;

	CEyeImage m_Image;

	COpData m_OpData;

	void CalcAngles();
};

//***************************************************************************************
//     Linear Array Detector
class CLAD
{
public:

	CLAD();

	ushort m_Signal[512];
	real_t   m_Baseline;
	int    m_Max;
	int    m_MaxPos;
	real_t   m_CentroidPos;
	real_t   m_Area;
	BOOL   m_Bad;

	void Process();
};

//***************************************************************************************

#define DT_QUADRANT        0
#define DT_LINEAR_ARRAYS   1

class CScanPoint
{
public:

	CScanPoint();

	int m_shot;
	real_t m_r_um;
	real_t m_a_rd;
	real_t m_dx_um;
	real_t m_dy_um;
	CLAD m_LADX;
	CLAD m_LADY;
	real_t m_OpacityX;
	real_t m_OpacityY;
	real_t m_Opacity;
	BOOL m_AutoBad;
	BOOL m_Bad;
	real_t x_um() { return m_r_um * cos(m_a_rd); }
	real_t y_um() { return m_r_um * sin(m_a_rd); }
};

//***************************************************************************************

#define EEF_MAX   0
#define EEF_CNT   1

// PSF is Point Spread Function.  This is one of the display methods.
class CPSF
{
public:

	const static real_t m_um_mn;
	const static real_t m_mn_um;

	real_t m_um_px;
	real_t m_px_um;

	real_t m_mn_px;
	real_t m_px_mn;

	Matrix<real_t> m_Array;

	int  m_xeef;
	int  m_yeef;

	int  m_xcnt;
	int  m_ycnt;

	int  m_xmax;
	int  m_ymax;
	real_t m_vmax; // maximum in non-normalized table for Strehl ratio

	real_t m_StrehlRatio;
	real_t m_EncircledEnergy[101];
	int  m_R50mn;
};

//***************************************************************************************
//     PSF is Point Spread Function.  This is one of the display methods.
class CPSF1
{
public:

	const static real_t m_um_mn;
	const static real_t m_mn_um;

	real_t m_um_px;
	real_t m_px_um;

	real_t m_mn_px;
	real_t m_px_mn;

	Matrix<real_t> m_Array;

	int  m_xeef;
	int  m_yeef;

	int  m_xcnt;
	int  m_ycnt;

	int  m_xmax;
	int  m_ymax;
	real_t m_vmax; // maximum in non-normalized table for Strehl ratio

	real_t m_StrehlRatio;
	real_t m_EncircledEnergy[101];
	int  m_R50mn;
};

//***************************************************************************************
//     PSF is Point Spread Function.  This is one of the display methods.
class CPSF2
{
public:

	const static real_t m_um_mn;
	const static real_t m_mn_um;

	real_t m_um_px;
	real_t m_px_um;

	real_t m_mn_px;
	real_t m_px_mn;

	Matrix<real_t> m_Array;

	int  m_xeef;
	int  m_yeef;

	int  m_xcnt;
	int  m_ycnt;

	int  m_xmax;
	int  m_ymax;
	real_t m_vmax; // maximum in non-normalized table for Strehl ratio

	real_t m_StrehlRatio;
	real_t m_EncircledEnergy[101];
	int  m_R50mn;
};

//***************************************************************************************
//     PSF is Point Spread Function.  This is one of the display methods.
class CPSF3
{
public:

	const static real_t m_um_mn;
	const static real_t m_mn_um;

	real_t m_um_px;
	real_t m_px_um;

	real_t m_mn_px;
	real_t m_px_mn;

	Matrix<real_t> m_Array;

	int  m_xeef;
	int  m_yeef;

	int  m_xcnt;
	int  m_ycnt;

	int  m_xmax;
	int  m_ymax;
	real_t m_vmax; // maximum in non-normalized table for Strehl ratio

	real_t m_StrehlRatio;
	real_t m_EncircledEnergy[101];
	int  m_R50mn;
};

//***************************************************************************************
//     PSF is Point Spread Function.  This is one of the display methods.
class CPSF4
{
public:

	const static real_t m_um_mn;
	const static real_t m_mn_um;

	real_t m_um_px;
	real_t m_px_um;

	real_t m_mn_px;
	real_t m_px_mn;

	Matrix<real_t> m_Array;

	int  m_xeef;
	int  m_yeef;

	int  m_xcnt;
	int  m_ycnt;

	int  m_xmax;
	int  m_ymax;
	real_t m_vmax; // maximum in non-normalized table for Strehl ratio

	real_t m_StrehlRatio;
	real_t m_EncircledEnergy[101];
	int  m_R50mn;
};

//***************************************************************************************

//  PSF is Point Spread Function.  This is one of the display methods.
class CPSF5
{
public:

	const static real_t m_um_mn;
	const static real_t m_mn_um;

	real_t m_um_px;
	real_t m_px_um;

	real_t m_mn_px;
	real_t m_px_mn;

	Matrix<real_t> m_Array;

	int  m_xeef;
	int  m_yeef;

	int  m_xcnt;
	int  m_ycnt;

	int  m_xmax;
	int  m_ymax;
	real_t m_vmax; // maximum in non-normalized table for Strehl ratio

	real_t m_StrehlRatio;
	real_t m_EncircledEnergy[101];
	int  m_R50mn;
};

//***************************************************************************************
//     MTF is Modulation Transfer Function.
//	   This is one of the display methods.
//	   It is related to the PSF in that the MTF is the rotationally averaged 2D Fourier Transform
//	   of the PSF
class CMTF
{
public:

	BOOL GetAtCdXY(const real_t x_cd, const real_t y_cd, real_t& v);
	BOOL GetAtCdRA(const real_t r_cd, int a_dg, real_t& v);
	BOOL GetAtCdR(const real_t r_cd, real_t& v);

	real_t m_cd_px;
	real_t m_px_cd;

	Matrix<real_t> m_Array;
};

//***************************************************************************************

class CMap2D
{
public:

	CMap2D();

	real_t m_r_max_um;
	real_t m_min;
	real_t m_max;

	int m_nr;

	real_t m_dr_um;

	void Create(const real_t r_max_um, const real_t dr_um);

	void SetAt(const int y, const int x, const real_t v);
	real_t GetAt(const int y, const int x) const;

	void Subtract(const CMap2D& Map1, const CMap2D& Map2);

	void ComputeMeanStdDev(real_t& mean, real_t& std_dev) const;

private:

	Matrix<real_t> m_data;
};

//***************************************************************************************

class CMap3D
{
public:

	CMap3D();

	real_t m_r_max_um;
	real_t m_min;
	real_t m_max;

	int m_nr;

	real_t m_dr_um;

	void Create(const real_t r_max_um, const real_t dr_um = 100.0);

	void SetAt(const int r, const int a, const real_t v);
	real_t GetAt(const int r, const int a) const;

	void Subtract(const CMap3D& Map1, const CMap3D& Map2);

private:

	Matrix<real_t> m_data;
};

//***************************************************************************************

class CComplaints
{
public:

	CComplaints();

	void Reset();

	int m_NightMiopia;
	int m_NightHyperopia;
	int m_Astigmatism;
	int m_BlurDoubleVision;//coma
	int m_GlareHalo;//spherical
	int m_MixedFocus;//secondary astigmatism
	int m_Starburst;//trefoil
};

//***************************************************************************************

class CWFExam : public CExam
{
public:

	CWFExam();

	const static real_t m_LADPixelSize;

	int m_DetectorType;

	List<CScanPoint> m_Points;
	int m_SignalMax;
	int m_NumGoodPoints;

	void ProcessPoints();
	void ProcessPresbiaPoints();//For showing Presbia 2.4mm
	int  GetNumShots();

	int  m_CurShot;
	int  m_CurPoint;
	void FstPoint();
	void LstPoint();
	void NxtPoint();
	void PrvPoint();
	int  GetNumShotPoints();

	void CreateProbePattern(real_t& r_max_um);
	void CreateScanPattern(real_t& r_max_um);

	CEyeImage m_ColorImage;

	CZernikeSurface m_WfSurface;

	real_t m_ScanDiameter;
	real_t m_AccommTargetPosition;
	real_t m_PrecalcSphere;
	real_t m_PrecalcCylinder;
	int  m_PrecalcAxis;
	real_t m_WavetouchLensPower;
	int  m_WavetouchLensBaseCurve;
	CString m_WavetouchOther;
	CString m_WavetouchNote;
	real_t m_WavetouchVertex;
	real_t m_WavetouchSph;
	real_t m_WavetouchCyl;
	int m_WavetouchAxis;
	BOOL m_AODsOK;
	int m_AlignmentMethod;

	real_t m_sc_x0_um;
	real_t m_sc_y0_um;

	const static real_t m_d_eye_um;
	const static real_t m_vn;
	const static real_t m_f_eye_um;

	void CreateCalSurface(CZernikeSurface& CalSurface);

	BOOL Process();

	BOOL Presbia24Process();// For showing the presbia 2.4mm zone

	static void GetSpheqSphCylAxis(const CZernikeSurface& Surface, const int VertexDistanceUm, const BOOL PositiveCylinder,
		real_t& sph_eq, real_t& sph, real_t& cyl, int& axis);
	void GetSpheqSphCylAxis(const int VertexDistanceUm, const BOOL PositiveCylinder,
		real_t& sph_eq, real_t& sph, real_t& cyl, int& axis) const;

	void CreateOpm2D(CMap2D& Map, const real_t dr_um, const int shot, const int op_type);
	static void CreateWfm2D(CMap2D& Map, const CZernikeSurface& Surface, const real_t dr_um);
	static void CreatePresbiaWfm2D(CMap2D& Map, const CZernikeSurface& Surface, const real_t dr_um);// For showing Presbia 2.4mm...
	static void CreateWfm3D(CMap3D& Map, const CZernikeSurface& Surface);
	static void CreatePresbiaWfm3D(CMap3D& Map, const CZernikeSurface& Surface);// For showing Presbia 2.4mm...
	static void CreateRfm2D(CMap2D& Map, const CZernikeSurface& Surface, const real_t dr_um);
	static void CreatePresbiaRfm2D(CMap2D& Map, const CZernikeSurface& Surface, const real_t dr_um);//530 For showing Presbia 2.4mm...
	static void CreateRfm3D(CMap3D& Map, const CZernikeSurface& Surface);
	static void CreatePresbiaRfm3D(CMap3D& Map, const CZernikeSurface& Surface);// For showing Presbia 2.4mm...

	static void CreatePsf(const CZernikeSurface& Surface, CPSF& Psf, const int EEFType);
	static void CreateMtf(const CZernikeSurface& Surface, CMTF& Mtf);
	static void CreateLtr(const CZernikeSurface& Surface, CPSF& Ltr, const int LtrType, const int LtrLine, const int LtrOrientation);

	static real_t GetAnnularRfAt(const CZernikeSurface& Surface, real_t r_um);
	static real_t GetAverageRfAt(const CZernikeSurface& Surface, real_t r_um);
	static real_t GetAverageRfAt(const CZernikeSurface& Surface, const real_t r_um, const real_t x0_um, const real_t y0_um);
	static void GetMinMaxRf(const CZernikeSurface& Surface, real_t& rf_min, real_t& x_min_um, real_t& y_min_um, real_t& rf_max, real_t& x_max_um, real_t& y_max_um);

	void Average(const Matrix<CWFExam*>& pWFExams);

	CComplaints m_Complaints;
	void CreateComplaints();

private:

	static void CreatePsfN(const CZernikeSurface& Surface, CPSF& Psf, const int n);
	static void CreateLtrN(const CZernikeSurface& Surface, CPSF& Ltr, const int LtrType, const int LtrLine,
		const int LtrOrientation, const int n);

	static real_t GetRfAt(const CZernikeSurface& Surface, const real_t r_um, const real_t a_rd);

	real_t GetClosestPointOpacity(const real_t r_um, const real_t a_rd, const int shot, const int op_type);

	BOOL m_OK;
};

//***************************************************************************************

#define RFI_UM 1337500.0
#define SKI_UM  337500.0

#define CT_R_MAX_UM 5000.0

#define CT_MAP_UNIT_DIOPTERS    0
#define CT_MAP_UNIT_MILLIMETERS 1

//***************************************************************************************

class CCTExam : public CExam
{
public:

	real_t              m_r_max_um;
	real_t              m_ra_max_um[360];

	real_t              m_ax_um[MAX_NUM_RINGS][360];

	CZernikeSurface   m_HtZrSurface;
	CConicSurface     m_HtCnSurface;
	CConicSurface     m_HtSpSurface;

	real_t              m_dh_dr[MAX_NUM_RINGS][360]; // CustomVis

	real_t              m_ax0_um;
	real_t              m_ax0_dp;
	real_t              m_rf0_um;

	BOOL              m_sk_ok;
	real_t              m_sk_st_um;
	real_t              m_sk_st_dp;
	int               m_sk_st_dg;
	real_t              m_sk_fl_um;
	real_t              m_sk_fl_dp;
	int               m_sk_fl_dg;
	real_t              m_sk_av_um;
	real_t              m_sk_av_dp;
	real_t              m_sk_df_dp;

	BOOL              m_kr_ok[3];
	real_t              m_kr_st_um[3][2];
	real_t              m_kr_st_dp[3][2];
	int               m_kr_st_dg[3][2];
	real_t              m_kr_fl_um[3][2];
	real_t              m_kr_fl_dp[3][2];
	int               m_kr_fl_dg[3][2];

	BOOL              m_zk_ok[11];
	real_t              m_zk_av_dp[11];

	BOOL              m_rf_ok;
	real_t              m_rf_st_dp;
	int               m_rf_st_dg;
	real_t              m_rf_fl_dp;
	int               m_rf_fl_dg;
	real_t              m_rf_av_dp;
	real_t              m_rf_df_dp;

	BOOL              m_is_ok;
	real_t              m_is_df_dp;

	real_t              m_sph_6mm;

	CZernikeSurface   m_WfSurface;

	CEyeImage         m_ColorImage;// for getting the color image

	BOOL Process();

	void CreateDpm2D(CMap2D& Map, const int Type, const int Unit, const real_t dr_um);
	void CreateElm2D(CMap2D& Map, const CSurface& RefSurface, const real_t dr_um);
	void CreateWfm2D(CMap2D& Map, const CSurface& WfSurface, const real_t dr_um);

	void CreateDpm3D(CMap3D& Map, const int Type, const int Unit);
	void CreateElm3D(CMap3D& Map, const CSurface& RefSurface);
	void CreateWfm3D(CMap3D& Map, const CSurface& WfSurface);

	BOOL Keratoconus() const { return m_OK && (m_ax0_dp > 47.2 && m_is_ok && m_is_df_dp > 1.4); }

	real_t GetTnUmAt(const real_t r_um, const real_t a_rd);// move from private to here

private:

	void Clear();

	void CopyRiFromImage();
	void FindAx();

	BOOL CreateHeightSurface();
	BOOL FindCentralPower();
	void FindKeratometry();
	void FindRefractivePower();
	void FindIS();
	void CreateBestFitConicoidHeightSurfaces();
	void CreateWavefrontSurface();

	BOOL CheckAt(const real_t r_um, const real_t a_rd) const;
	real_t GetAxUmAt(const real_t r_um, const real_t a_rd);
	real_t GetAxDpAt(const real_t r_um, const real_t a_rd);

	real_t GetTnDpAt(const real_t r_um, const real_t a_rd);
	real_t GetRfUmAt(const real_t r_um, const real_t a_rd);
	real_t GetRfDpAt(const real_t r_um, const real_t a_rd);
	real_t GetElUmAt(const real_t r_um, const real_t a_rd, const CSurface& RefSurface);
	real_t GetWfUmAt(const real_t r_um, const real_t a_rd, const CSurface& WfSurface) const;
};

//***************************************************************************************

typedef BOOL(__cdecl* TDllInitDatabase)(const char* RootPassword);

typedef BOOL(__cdecl* TDllConnect)(const char* Filename, const char* Password, const char* LocalFolderName, const BOOL ReadOnly);// add LocalFolderName amd Merge
typedef void(__cdecl* TDllDisconnect)(void);
typedef BOOL(__cdecl* TDllIsReadOnly)(void);
typedef BOOL(__cdecl* TDllChangeLinker)(const char* FolderName);//For change of front-end table linker
typedef BOOL(__cdecl* TDllCreateLinker)(const char* FileName, const char* FolderName);// For create of front-end table linker

typedef int(__cdecl* TDllGetGroupsCount)(void);
typedef BOOL(__cdecl* TDllLoadGroups)(TDB_GROUP*);
typedef BOOL(__cdecl* TDllSaveGroup)(TDB_GROUP*);

typedef int(__cdecl* TDllGetClinicsCount)(void);
typedef BOOL(__cdecl* TDllLoadClinics)(TDB_CLINIC*);
typedef BOOL(__cdecl* TDllSaveClinic)(TDB_CLINIC*);
typedef BOOL(__cdecl* TDllDeleteClinic)(const GUID& ClinicID);

typedef int(__cdecl* TDllGetPhysiciansCount)(void);
typedef BOOL(__cdecl* TDllLoadPhysicians)(TDB_PHYSICIAN*);
typedef BOOL(__cdecl* TDllSavePhysician)(TDB_PHYSICIAN*);

typedef int(__cdecl* TDllGetOperatorsCount)(void);
typedef BOOL(__cdecl* TDllLoadOperators)(TDB_OPERATOR*);
typedef BOOL(__cdecl* TDllSaveOperator)(TDB_OPERATOR*);

typedef int(__cdecl* TDllGetPatientsCount)(void);
typedef BOOL(__cdecl* TDllLoadPatients)(TDB_PATIENT*);
typedef BOOL(__cdecl* TDllSavePatient)(TDB_PATIENT*);
typedef BOOL(__cdecl* TDllDeletePatient)(const GUID& PatientID);

typedef int(__cdecl* TDllGetEyesCount)(const GUID& PatientID);
typedef BOOL(__cdecl* TDllLoadEyes)(const GUID& PatientID, TDB_EYE*);
typedef BOOL(__cdecl* TDllSaveEye)(TDB_EYE*);

typedef int(__cdecl* TDllGetExamsCount)(const GUID& PatientID);
typedef BOOL(__cdecl* TDllLoadExamHeaders)(const GUID& PatientID, TDB_EXAM_HEADER* pExamHeaders);
typedef BOOL(__cdecl* TDllSaveExamHeader)(TDB_EXAM_HEADER* pExamHeader);
typedef int(__cdecl* TDllGetExamSize)(const GUID& ExamID);
typedef BOOL(__cdecl* TDllLoadExam)(const GUID& ExamID, void* pData, const int DataSize);
typedef BOOL(__cdecl* TDllSaveExam)(const GUID& ExamID, const void* pData, const int DataSize);
typedef BOOL(__cdecl* TDllDeleteExam)(const GUID& ExamID);

typedef int(__cdecl* TDllGetCalibrationSize)(const GUID& CalibrationID);
typedef BOOL(__cdecl* TDllLoadCalibration)(const GUID& CalibrationID, void* pData, const int DataSize);
typedef BOOL(__cdecl* TDllSaveCalibration)(const GUID& CalibrationID, const void* pData, const int DataSize);

//***************************************************************************************

class CDEMOID
{
public:

	GUID            PatientID[50];

	BOOL LoadFromDemoFile(const CString& FileName);
	void SaveIntoDemoFile(const CString& FileName);
};

//***************************************************************************************
class CDB : public Singleton<CDB>
{
public:
	CDB();
	~CDB();

	Matrix<CGroup> m_Groups;
	Matrix<CClinic> m_Clinics;
	Matrix<CPhysician> m_Physicians;
	Matrix<COperator> m_Operators;
	Matrix<CPatient> m_Patients;
	Matrix<CEye> m_Eyes;
	Matrix<CExamHeader> m_ExamHeaders;

	void ConnectOtherMsjet(CString FolderName);//Load Other msjet database for database merge
	void Connect(const BOOL ReadOnly);
	void LocalConnect(const BOOL ReadOnly);//6.2.0 For Dicom
	void Disconnect();

	void Reinit();

	BOOL IsReadOnly();

	void DeleteDirectory(char* sDirName);// delete all files inside a specified folder

	void GetType(int& Type);
	void GetFolderName(CString& FolderName);
	void GetLocalFolderName(CString& LocalFolderName, CString ServerFolderName);

	CString GetSelfFolderName(CString FolderName);
	void GetFileName(CString& FileName);//For old msjet file

	void Change(const int NewType, const CString& NewFolderName, const CString& NewFileName, const BOOL ReadOnly);

	void LoadGroups();
	BOOL SaveGroup(CGroup& Group);
	CGroup* GetGroup(const GUID& GroupID);

	void LoadClinics();
	BOOL SaveClinic(CClinic& Clinic);
	CClinic* GetClinic(const GUID& ClinicID);
	void DeleteClinic(const GUID& ClinicID);

	GUID LoadFellowID(const GUID& ExamID, real_t& scanSize, int& NumImages, int exam_type);
	BOOL ExamExist(const GUID& ExamID);

	void LoadPhysicians();
	BOOL SavePhysician(CPhysician& Physician);
	CPhysician* GetPhysician(const GUID& PhysicianID);

	void LoadOperators();
	BOOL SaveOperator(COperator& Operator);
	COperator* GetOperator(const GUID& OperatorID);

	void LoadPatients();
	BOOL SavePatient(CPatient& Patient);
	void DeletePatient(const GUID& PatientID);
	CPatient* GetPatient(const GUID& PatientID);

	int  m_EyeNum;
	void LoadEyes(const GUID& PatientID);
	BOOL SaveEye(CEye& Eye);

	int  m_NumExams;
	void LoadExamHeaders(const GUID& PatientID);
	BOOL SaveExamHeaders(CExamHeader& ExamHeader);

	//BOOL CheckExamsNum(CString DataFolderName);//Big Database Research since 03062019
	//void CheckPreNewExams();//Big Database Research since 03062019
	//void LoadTxtExamHeaders(const GUID& PatientID);//Big Database Research since 03062019
	//BOOL SaveTxTExamHeader(const GUID& PatientID, ExamHeader_STR ThisExamStr);//Big Database Research since 03062019
	//BOOL SaveTxTExamHeaders(CExamHeader* In_ExamHeader);//Big Database Research since 03062019
	////BOOL DelTxTExamHeader(const GUID& ExamID);//Big Database Research since 03062019
	//BOOL DelTxTExamHeader(const GUID& PatientID, const GUID& ExamID);//Big Database Research since 03062019

	//void GenerateTxtExamHeader(const GUID& PatientID, ExamHeader_STR ThisExamStr);//Big Database Research since 03062019

  //**** Better loading functions
  auto LoadWFExam(const GUID &ExamID)
  {
    auto e = std::make_unique<CWFExam>();
    if (!LoadWFExam(ExamID, e.get()))
      e.reset();
    return e;
  }
  auto LoadCTExam(const GUID &ExamID)
  {
    auto e = std::make_unique<CCTExam>();
    if (!LoadCTExam(ExamID, e.get()))
      e.reset();
    return e;
  }

  //*****************************
  BOOL LoadWFExam(const GUID &ExamID, CWFExam *pWFExam);
	BOOL LoadCTExam(const GUID& ExamID, CCTExam* pCTExam);
	BOOL SaveExam(CExam* pExam);
	BOOL SaveWFExam(CWFExam* pWFExam);
	BOOL SaveCTExam(CCTExam* pCTExam);
	void DeleteExam(const GUID& ExamID);
	CExamHeader* GetExamHeader(const GUID& ExamID);

	//BOOL SaveExamHeader(CExamHeader m_Header, int Type);// Recover the exam patients;05202016 for a customer lost tdb data but has exam files.

	BOOL Import(const CString& FileName);
	BOOL ExportPatient(const GUID& PatientID, const CString& FolderName);

private:

	void SetType(const int Type);
	void SetFolderName(const CString& FolderName);
	void SetFileName(const CString& FileName);//For old msjet file

	BOOL LoadCalibration(const GUID& CalibrationID, CALIBRATION* pCalibration);
	BOOL SaveCalibration(const GUID& CalibrationID, CALIBRATION* pCalibration);

	void PatientClassToStruct(const CPatient& Patient, TDB_PATIENT* pPatient);
	void PatientStructToClass(const TDB_PATIENT* pPatient, CPatient& Patient);

	CString SplitDatabaseProcess(CString FolderName, BOOL& NeedChangeLinker, BOOL& FirstRun);//Get the linker folder and filename for linked database

	HMODULE m_hDLL;

	HMODULE LoadLibrary(const int Type);

	TDllConnect            DllConnect;
	TDllDisconnect         DllDisconnect;
	TDllInitDatabase       DllInitDatabase;
	TDllIsReadOnly         DllIsReadOnly;

	TDllChangeLinker       DllChangeLinker;
	TDllCreateLinker       DllCreateLinker;

	TDllGetGroupsCount     DllGetGroupsCount;
	TDllLoadGroups         DllLoadGroups;
	TDllSaveGroup          DllSaveGroup;

	TDllGetClinicsCount    DllGetClinicsCount;
	TDllLoadClinics        DllLoadClinics;
	TDllSaveClinic         DllSaveClinic;
	TDllDeleteClinic       DllDeleteClinic;

	TDllGetPhysiciansCount DllGetPhysiciansCount;
	TDllLoadPhysicians     DllLoadPhysicians;
	TDllSavePhysician      DllSavePhysician;

	TDllGetOperatorsCount  DllGetOperatorsCount;
	TDllLoadOperators      DllLoadOperators;
	TDllSaveOperator       DllSaveOperator;

	TDllGetPatientsCount   DllGetPatientsCount;
	TDllLoadPatients       DllLoadPatients;
	TDllSavePatient        DllSavePatient;
	TDllDeletePatient      DllDeletePatient;

	TDllGetEyesCount       DllGetEyesCount;
	TDllLoadEyes           DllLoadEyes;
	TDllSaveEye            DllSaveEye;

	TDllGetExamsCount      DllGetExamsCount;
	TDllLoadExamHeaders    DllLoadExamHeaders;
	TDllSaveExamHeader     DllSaveExamHeader;
	TDllGetExamSize        DllGetExamSize;
	TDllLoadExam           DllLoadExam;
	TDllSaveExam           DllSaveExam;
	TDllDeleteExam         DllDeleteExam;

	TDllGetCalibrationSize DllGetCalibrationSize;
	TDllLoadCalibration    DllLoadCalibration;
	TDllSaveCalibration    DllSaveCalibration;

public:
  static CDB &TheOneAndOnly()
  {
    return *Instance();
  }
};

//***************************************************************************************

// extern CDB DB;

#define DB CDB::TheOneAndOnly()

//***************************************************************************************
