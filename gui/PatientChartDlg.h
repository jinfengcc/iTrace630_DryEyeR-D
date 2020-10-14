//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MGDI.h"
#include "Data.h"

//***************************************************************************************
typedef struct _Patient_Reg {

	int Year;
	int Month;
	int Num;

} Patient_Reg;//The struct used to record how many patients in per month

class CPatientChartDlg : public CDialog
{
public:

	CPatientChartDlg(CWnd* pParentWnd);
	~CPatientChartDlg();

private:

	HACCEL m_hAccel;

	CMDC m_MemDC;
	CRgn m_Rgn;

	int  m_pic_w;
	int  m_pic_h;

	void Resize();

	void DrawChart(CMDC& MemDC);

	Patient_Reg m_PatienNumByTime[240];//20 years with 240 months
	int  m_maxNuminOneMonth;
	int  m_MonthRange;

	void CalChartValues();
	int  GetMonthRange(int minYear, int minMonth, int maxYear, int maxMonth);//Calculate month range

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;

	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
