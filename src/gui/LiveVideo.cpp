//***************************************************************************************

#include "StdAfx.h"
#include "LiveVideo.h"
#include "libs/LoggingLib/TkTraceWin.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

CLiveVideo::CLiveVideo(CHW* pHW)
{
	m_pHW = pHW;

	m_pVideoWnd = NULL;

	m_NumFramesReceived = 0;
	m_AverageCycleTime = 0.0;

	m_Thread = NULL;

	m_ToDo = DO_NOTHING;
}

//***************************************************************************************

ulong __stdcall CLiveVideo::ThreadFunction(void* pParam)
{
	((CLiveVideo*)pParam)->Main();
	return 0;
}

//***************************************************************************************

BOOL CLiveVideo::Start()
{
	if (m_pVideoWnd == NULL) return FALSE;

	ulong id;
	m_Thread = ::CreateThread(NULL, 0, ThreadFunction, this, CREATE_SUSPENDED, &id);

	if (m_Thread == NULL) return FALSE;

	::SetThreadPriority(m_Thread, THREAD_PRIORITY_NORMAL);
	::ResumeThread(m_Thread);

	return TRUE;
}

//***************************************************************************************

void CLiveVideo::Finish()
{
	if (m_Thread == NULL) return;

	::WaitForSingleObject(m_Thread, INFINITE);

	::CloseHandle(m_Thread);
	m_Thread = NULL;
}

//***************************************************************************************

void CLiveVideo::Main()
{
	m_pHW->ClearFrames();

	m_pHW->ApplyCurrentVideoSettings();
	m_pHW->TurnInfraredLEDsOn();
	m_pHW->TurnWhiteLEDsOn();

	if (!::HW.ISVideoCameraOn)
	{
		m_pHW->TurnVideoCameraOn();
		::HW.ISVideoCameraOn = true;
	}

	::Sleep(600);

	BOOL Done = FALSE;

	int StartTime = clock();
	m_NumFramesReceived = 0;
	m_AverageCycleTime = 0.0;
			
	while (!Done)
	{
		m_pHW->StartTransferringVideoFrame();

		m_pVideoWnd->PostMessage(WM_THREAD_UPDATE, 0, 0);

	// #fix
    //Sleep(1000);
		m_pHW->FinishTransferringVideoFrame();

		switch (m_ToDo)
		{
		case ABORT:
			Done = TRUE;
			break;
		case CHANGE_VIDEO_SETTINGS:
			m_pHW->ApplyCurrentVideoSettings();
			break;

		case CHANGE_BRIGHTNESS:
			m_pHW->ApplyCurrentVideoBrightness();
			break;
		case CHANGE_CONTRAST:
			m_pHW->ApplyCurrentVideoContrast();
			break;
		case CHANGE_HUE:
			//m_pHW->ApplyCurrentVideoHue();
			break;
		case CHANGE_SATURATION:
			//m_pHW->ApplyCurrentVideoSaturation();
			break;
		case CHANGE_INFRARED_LEDS_POWER_LEVEL:
			m_pHW->TurnInfraredLEDsOn();
			break;
		case CHANGE_WHITE_LEDS_POWER_LEVEL:
			m_pHW->TurnWhiteLEDsOn();
			break;
		}
		m_ToDo = DO_NOTHING;

		m_AverageCycleTime = (real_t)(clock() - StartTime) / (++m_NumFramesReceived);

		::Sleep(0);
	}

	char sz[80];
  sprintf_s(sz, "Avg Cycle time = %.1f ms\n", 1000 * m_AverageCycleTime / CLOCKS_PER_SEC);
  DILASCIA_TRACE_EX("FPS", sz);

	m_pHW->TurnInfraredLEDsOff();
	m_pHW->TurnWhiteLEDsOff();

	m_pVideoWnd->PostMessage(WM_THREAD_FINISH, 0, 0);
}

//***************************************************************************************
