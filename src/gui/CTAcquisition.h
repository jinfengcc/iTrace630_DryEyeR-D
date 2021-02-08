//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Acquisition.h"
#include "Data.h"

//***************************************************************************************

class CCTAcquisition : public CAcquisition
{
public:

	CCTAcquisition(CHW* pHW, BOOL ShowRedDot);

	BOOL    m_ShowRedDot;//the red dot to be shown or not

	CCTExam m_CTExam;

	BOOL    m_TriLaserShutoffOn;

	BOOL    m_TriLaserOn;

	BOOL    m_ve0_ok;
	BOOL    m_la_ok;

	int     m_AlignmentStatus;
	int     m_TriangulationSensorReading;
	BOOL    m_ChangeAlMode;

	int     m_BackupWhiteLEDsPowerLevel;
	int     m_BackupBrightness;
	int     m_BackupContrast;
	int     m_BackupHue;
	int     m_BackupSaturation;

	int     m_WhiteLEDsPower;
	int     m_Brightness;
	int     m_Contrast;
	int     m_Hue;
	int     m_Saturation;

private:

	virtual void Main() override;

	void CheckAlignment();
};

//***************************************************************************************
