//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Acquisition.h"
#include "Settings.h"
#include "Data.h"

//***************************************************************************************
#define INC_SCAN_SIZE           21
#define DEC_SCAN_SIZE           22
#define MOVE_TARGET_UP          23
#define MOVE_TARGET_DN          24
#define MOVE_TARGET_HOME        25
#define SWITCH_TARGET_LIGHT     26
#define CHANGE_ALIGNMENT_METHOD 27
#define CHANGE_SCAN_SIZE        28

//***************************************************************************************

class CWFAcquisition : public CAcquisition
{
public:

	CWFAcquisition(CHW* pHW, CSettings* pSettings, BOOL Combo);

	BOOL       m_Combo;

	int        m_TargetTime;
	BOOL       m_ChangeScanSize;
	BOOL       m_ChangeAlMode;
	BOOL       m_ChangeAlMethod;
	BOOL       m_ChangeIllumi;
	BOOL       m_ChangeToPresbia256;//Presbia
	BOOL       m_FromPreToNat;//Presbia

	real       m_NewScanSize;
	CString    m_LastNumStr;

	CString    m_ss;

	CWFExam    m_WFExam;

	CWFExam    m_mWFExams[3];

	CSettings* m_pSettings;

	BOOL       m_Probing;
	BOOL       m_TargetLightOn;

	int        m_NumConsecFrames;
	int        m_AllowedDecentError;
	int        m_AllowedMovement;

	real        m_auto_r_max_um;
	real        m_user_r_max_um;
	real        m_user_r_max_um2;

	void        NotifyIncScanSize() { m_ToDo = INC_SCAN_SIZE; }
	void        NotifyDecScanSize() { m_ToDo = DEC_SCAN_SIZE; }
	void        NotifyMoveTargetUp() { m_ToDo = MOVE_TARGET_UP; }
	void        NotifyMoveTargetDn() { m_ToDo = MOVE_TARGET_DN; }
	void        NotifyMoveTargetHome() { m_ToDo = MOVE_TARGET_HOME; }
	void        NotifySwitchTargetLight() { m_ToDo = SWITCH_TARGET_LIGHT; }
	void        NotifyChangeAlignmentMethod() { m_ToDo = CHANGE_ALIGNMENT_METHOD; }
	void        NotifyChangeScanSize() { m_ToDo = CHANGE_SCAN_SIZE; }

private:

	int m_AlignmentCounter;

	virtual void Main() override;

	BOOL CheckAlignment();

	void ChangeIllumination();
	void ChangeAlignmentMethod();
	void ChangeAlignmentMode();

	BOOL SetupScanPointsAndScan();
	BOOL DownloadScanResults();
};

//***************************************************************************************
