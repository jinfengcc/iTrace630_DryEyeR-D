//***************************************************************************************

#pragma once

//***************************************************************************************

#include "LiveVideo.h"

//***************************************************************************************

#define ACQUIRE       10
#define CHANGE_MODE   11

//***************************************************************************************

class CAcquisition : public CLiveVideo
{
public:

	CAcquisition(CHW* pHW) : CLiveVideo(pHW)
	{
		m_Eye = EYE_OD;	
		m_CTAutoMode = TRUE;
		m_WFAutoMode = 1;
	}

	int m_Eye;

	BOOL m_CTAutoMode;
	int  m_WFAutoMode; //1: WF Automode, 2: WF Manual full scan ; 3: WF Manual 64 scan

	int m_TimeLeft;

	void Acquire() { m_ToDo = ACQUIRE; }
	void ChangeMode() { m_ToDo = CHANGE_MODE; }
};

//***************************************************************************************
