//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "PatientChartDlg.h"
#include "BusyCursor.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CPatientChartDlg, CDialog)

	ON_WM_PAINT()

END_MESSAGE_MAP()

//***************************************************************************************

CPatientChartDlg::CPatientChartDlg(CWnd* pParentWnd) : CDialog(IDD_PATIENT_CHART_DLG, pParentWnd)
{
	m_hAccel = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR));
}

//***************************************************************************************

CPatientChartDlg::~CPatientChartDlg()
{
	DestroyAcceleratorTable(m_hAccel);
}

//***************************************************************************************

void CPatientChartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

//***************************************************************************************

BOOL CPatientChartDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	Resize();

	OnPaint();

	return FALSE;
}

//***************************************************************************************

void CPatientChartDlg::Resize()
{
	int W = ::GetSystemMetrics(SM_CXSCREEN);
	int H = ::GetSystemMetrics(SM_CYSCREEN);

	int ww = intRound(0.85 * W);
	int wh = intRound(0.7 * H);

	m_pic_w = ww - 30;
	m_pic_h = wh - 30;

	m_MemDC.CreateTrueColorDC(m_pic_w, m_pic_h);

	m_Rgn.CreateRectRgn(10, 10, m_pic_w, m_pic_h);

	SetWindowPos(NULL, 0, 0, ww, wh, SWP_NOZORDER);
	CenterWindow();

	CalChartValues();
}

//***************************************************************************************

void CPatientChartDlg::CalChartValues()
{
	int minYear = 3000;
	int minMonth = 13;

	int maxYear = 0;
	int maxMonth = 0;

	int TotPatCount = ::DB.m_Patients.GetSize();

	for (int e = 0; e < TotPatCount; e++)
	{
		CPatient* pPatient = &::DB.m_Patients[e];

		if (pPatient->m_RegYear < 3010 && pPatient->m_RegYear > 0)
		{
			if (pPatient->m_RegYear < minYear)
			{
				minYear = pPatient->m_RegYear;
			}

			if (pPatient->m_RegYear > maxYear)
			{
				maxYear = pPatient->m_RegYear;
			}
		}
	}

	for (int e = 0; e < TotPatCount; e++)
	{
		CPatient* pPatient = &::DB.m_Patients[e];

		if (pPatient->m_RegYear == minYear)
		{
			if (pPatient->m_RegMonth < minMonth)
			{
				minMonth = pPatient->m_RegMonth;
			}
		}

		if (pPatient->m_RegYear == maxYear)
		{

			if (pPatient->m_RegMonth > maxMonth)
			{
				maxMonth = pPatient->m_RegMonth;
			}
		}
	}

	m_MonthRange = GetMonthRange(minYear, minMonth, maxYear, maxMonth);

	m_PatienNumByTime[0].Year = minYear;
	m_PatienNumByTime[0].Month = minMonth;
	m_PatienNumByTime[0].Num = 0;

	for (int i = 1; i < m_MonthRange; i++)
	{
		int ThisYear = m_PatienNumByTime[i - 1].Year;
		int ThisMonth = m_PatienNumByTime[i - 1].Month + 1;

		if (ThisMonth == 13)
		{
			ThisMonth = 1;
			ThisYear = m_PatienNumByTime[i - 1].Year + 1;
		}

		m_PatienNumByTime[i].Month = ThisMonth;
		m_PatienNumByTime[i].Year = ThisYear;
		m_PatienNumByTime[i].Num = 0;
	}

	m_maxNuminOneMonth = 0;
	for (int e = 0; e < TotPatCount; e++)
	{
		CPatient* pPatient = &::DB.m_Patients[e];

		int MonDif = GetMonthRange(minYear, minMonth, pPatient->m_RegYear, pPatient->m_RegMonth);

		m_PatienNumByTime[MonDif - 1].Num++;

		if (m_PatienNumByTime[MonDif - 1].Num > m_maxNuminOneMonth) m_maxNuminOneMonth = m_PatienNumByTime[MonDif - 1].Num;
	}
}

//***************************************************************************************

void CPatientChartDlg::OnPaint()
{
	DrawChart(m_MemDC);
	InvalidateRgn(&m_Rgn, FALSE);
	CPaintDC PaintDC(this);
	PaintDC.BitBlt(10, 10, m_pic_w, m_pic_h, &m_MemDC, 0, 0, SRCCOPY);
}

//***************************************************************************************
//Draw the patient number chart
void CPatientChartDlg::DrawChart(CMDC& MemDC)
{
	int x0 = intRound(m_pic_w*0.05);
	int y0 = intRound(m_pic_h*0.9);

	int x1 = x0;
	int y1 = intRound(m_pic_h*0.1);

	int x2 = intRound(m_pic_w*0.95);
	int y2 = y0;

	MemDC.DrawLine(x0, y0, x1, y1, 1, WHITE);
	MemDC.DrawLine(x1, y1, x1 - 5, y1 + 5, 1, WHITE);
	MemDC.DrawLine(x1, y1, x1 + 5, y1 + 5, 1, WHITE);

	MemDC.DrawLine(x0, y0, x2, y2, 1, WHITE);
	MemDC.DrawLine(x2, y2, x2 - 5, y2 - 5, 1, WHITE);
	MemDC.DrawLine(x2, y2, x2 - 5, y2 + 5, 1, WHITE);

	CMFont  Font;
	RECT    rect;
	Font.Create(intRound(m_pic_h*0.03), 400, "Arial");
	::SetRect(&rect, intRound(m_pic_w*0.95 - 40), intRound(m_pic_h*0.92), m_pic_w, intRound(m_pic_h*0.95));

	MemDC.WriteText("Time (month)", rect, Font, WHITE, 1, NOCOLOR);

	::SetRect(&rect, intRound(m_pic_w*0.005), intRound(m_pic_h*0.02), intRound(m_pic_w*0.15), intRound(m_pic_h*0.07));

	MemDC.WriteText("Patient amount", rect, Font, WHITE, 0, NOCOLOR);


	//Draw the patien's number and time
	//Patient_Reg m_PatienNumByTime[240];//20 years with 240 months
	CString s;
	int XCoordStep = intRound((x2 - x0 - 40) / m_MonthRange);
	int YCoordStep = intRound((y0 - y1 - 40) / m_maxNuminOneMonth);

	int halfXCoordStep = intRound(XCoordStep / 2);
	int halfYCoordStep = intRound(YCoordStep / 2);

	Font.Create(intRound(0.8*XCoordStep), 400, "Arial");

	int newY0 = y0;
	for (int i = 0; i < m_maxNuminOneMonth; i++)
	{
		newY0 -= YCoordStep;

		MemDC.DrawLine(x0, newY0, x0 + 5, newY0, 2, WHITE);

		s.Format(_T("%i"), i + 1);
		::SetRect(&rect, intRound(x0 - XCoordStep), newY0 - 5, intRound(x0), newY0 + 5);
		MemDC.WriteText(s, rect, Font, WHITE, 1, NOCOLOR);
	}

	int DisY = y0 - newY0;

	for (int i = 0; i < m_MonthRange; i++)
	{
		x0 += XCoordStep;

		s.Format(_T("%i"), m_PatienNumByTime[i].Month);
		::SetRect(&rect, intRound(x0 - halfXCoordStep), y0 + 5, intRound(x0 + halfXCoordStep), y0 + 15);

		if (m_PatienNumByTime[i].Month == 1)
		{
			MemDC.DrawLine(x0, y0 - 5, x0, y0, 2, GREEN);
			MemDC.WriteText(s, rect, Font, GREEN, 1, NOCOLOR);

			::SetRect(&rect, intRound(x0 - XCoordStep), y0 + 15, intRound(x0 + XCoordStep), y0 + 30);
			s.Format(_T("%i"), m_PatienNumByTime[i].Year);
			s = "(" + s + ")";
			MemDC.WriteText(s, rect, Font, GREEN, 1, NOCOLOR);
		}
		else
		{
			MemDC.DrawLine(x0, y0 - 5, x0, y0, 2, WHITE);
			MemDC.WriteText(s, rect, Font, WHITE, 1, NOCOLOR);
		}

		int Num = m_PatienNumByTime[i].Num;
		real ratio = (real)Num / (real)m_maxNuminOneMonth;
		int PosY = intRound(ratio*DisY);

		int y = intRound((m_PatienNumByTime[i].Num / m_maxNuminOneMonth)*newY0);
		MemDC.DrawDottedLine(x0, y0, x0, y0 - PosY, YELLOW);
	}
}

//********************************************************************************************

//Calculate month range
int CPatientChartDlg::GetMonthRange(int minYear, int minMonth, int maxYear, int maxMonth)
{
	int difYears = maxYear - minYear - 1;
	if (difYears == -1) return maxMonth - minMonth + 1;

	int startYearMons = 13 - minMonth;

	return difYears * 12 + startYearMons + maxMonth;
}