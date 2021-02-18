//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Hardware.h"

//***************************************************************************************

#define DO_NOTHING                         0
#define ABORT                              1
#define CHANGE_BRIGHTNESS                  2
#define CHANGE_CONTRAST                    3
#define CHANGE_HUE                         4
#define CHANGE_SATURATION                  5
#define CHANGE_INFRARED_LEDS_POWER_LEVEL   6
#define CHANGE_WHITE_LEDS_POWER_LEVEL      7
#define CHANGE_VIDEO_SETTINGS              8

//***************************************************************************************

class CLiveVideo
{
public:

	CLiveVideo(CHW* pHW);
	virtual ~CLiveVideo() {}

	CHW*  m_pHW;

	CWnd* m_pVideoWnd;

	uint  m_NumFramesReceived;
	real_t  m_AverageCycleTime;

	BOOL  Start();
	void  Finish();

	void  Abort() { m_ToDo = ABORT; }
	void  ChangeInfraredLEDsPowerLevel() { m_ToDo = CHANGE_INFRARED_LEDS_POWER_LEVEL; }
	void  ChangeWhiteLEDsPowerLevel() { m_ToDo = CHANGE_WHITE_LEDS_POWER_LEVEL; }
	void  ChangeBrightness() { m_ToDo = CHANGE_BRIGHTNESS; }
	void  ChangeContrast() { m_ToDo = CHANGE_CONTRAST; }
  void ChangeVideoSettings()
  {
    m_ToDo = CHANGE_VIDEO_SETTINGS;
  }

protected:

	uint m_ToDo;

	virtual void Main();

private:
	void  ChangeHue() { m_ToDo = CHANGE_HUE; }
	void  ChangeSaturation() { m_ToDo = CHANGE_SATURATION; }

	void* m_Thread;

	static ulong __stdcall ThreadFunction(void* pParam);
};

//***************************************************************************************
