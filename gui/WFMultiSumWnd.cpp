//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "WFMultiSumWnd.h"
#include "ChartWnd.h"
#include "EyeWnd.h"
#include "NumDlg.h"
#include "BusyCursor.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CWFMultiSumWnd, CSumWnd)

	ON_COMMAND(IDC_DIFF_ITEM, OnDiffItemClicked)
	ON_COMMAND(IDC_AVG_ZONE_ITEM, OnAvgZoneItemClicked)
	ON_COMMAND(IDC_SERIES_NUMBER_00_ITEM + 0, OnSeriesNumber00ItemClicked)
	ON_COMMAND(IDC_SERIES_NUMBER_00_ITEM + 1, OnSeriesNumber01ItemClicked)
	ON_COMMAND(IDC_SERIES_NUMBER_00_ITEM + 2, OnSeriesNumber02ItemClicked)
	ON_COMMAND(IDC_SERIES_NUMBER_00_ITEM + 3, OnSeriesNumber03ItemClicked)
	ON_COMMAND(IDC_SERIES_NUMBER_00_ITEM + 4, OnSeriesNumber04ItemClicked)
	ON_COMMAND(IDC_SERIES_NUMBER_00_ITEM + 5, OnSeriesNumber05ItemClicked)
	ON_COMMAND(IDC_SERIES_NUMBER_00_ITEM + 6, OnSeriesNumber06ItemClicked)
	ON_COMMAND(IDC_SERIES_NUMBER_00_ITEM + 7, OnSeriesNumber07ItemClicked)
	ON_COMMAND(IDC_SERIES_NUMBER_00_ITEM + 8, OnSeriesNumber08ItemClicked)
	ON_COMMAND(IDC_SERIES_NUMBER_00_ITEM + 9, OnSeriesNumber09ItemClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CWFMultiSumWnd::CWFMultiSumWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, int NumExams, CWFExam** ppWFExams, CWndSettings* pWndSettings) :
	CSumWnd(pWnd, WndRect, pPatient, pWndSettings)
{
	CBusyCursor Cursor;

	//----------------------------------------------------
	RECT Rect;
	GetWindowRect(&Rect);
	real_t t = 3.0 * (0.03 * m_h) + m_g;
	real_t w = ((Rect.right - Rect.left) - 4.0 * m_g) / 3.0;
	real_t h = ((Rect.bottom - Rect.top - t) - 2.0 * m_g) / 2.0;
	::SetRect(&m_Rect[0], intRound(m_g), intRound(t), intRound(m_g + w), intRound(t + h));
	::SetRect(&m_Rect[1], intRound(m_g), intRound(t + h + m_g), intRound(m_g + w), intRound(t + h + m_g + h));
	::SetRect(&m_Rect[2], intRound(m_g + w + m_g), intRound(t), intRound(m_g + w + m_g + w), intRound(t + h));
	::SetRect(&m_Rect[3], intRound(m_g + w + m_g), intRound(t + h + m_g), intRound(m_g + w + m_g + w), intRound(t + h + m_g + h));
	//----------------------------------------------------

	m_NumExams = NumExams;
	m_ppWFExams = ppWFExams;

	//*007*[cjf***05052012],record the increase and decrease range
	real_t min_r_max_um = 100000;
	for (int i = 0; i<m_NumExams; i++)
	{
		if (m_ppWFExams[i]->m_WfSurface.m_r_max_um <  min_r_max_um)
		{
			min_r_max_um = m_ppWFExams[i]->m_WfSurface.m_r_max_um;
		}
	}
	::TempSettings.Com_r_max_um = min_r_max_um;
	//*007*[cjf***05052012]

	m_diff = FALSE;
	m_r_um = 0.0;
	m_series = 0;

	CreateChildWnd();
	Repaint();

	ShowWindow(SW_SHOW);
}

//***************************************************************************************

void CWFMultiSumWnd::CreateChildWnd()
{
	for (int i = 0; i < 4; i++) SAFE_DELETE(m_pDispWnd[i]);

	CWndSettings* pWndSettings = GetWndSettings();

	real_t r_max_um = 5000.0;
	for (int j = 0; j < m_NumExams; j++) {
		r_max_um = __min(r_max_um, m_ppWFExams[j]->m_WfSurface.m_r_max_um);
	}
	r_max_um = __min(r_max_um, pWndSettings->m_ZoneRadiusUm);

	CZernikeSurface WfSurface1;
	WfSurface1 = m_ppWFExams[0]->m_WfSurface;
	WfSurface1.ChangeRMaxUm(r_max_um);
	WfSurface1 &= pWndSettings->m_Mask;

	CChartWnd* pChartWnd[3];
	for (int i = 0; i < 3; i++) {
		pChartWnd[i] = new CChartWnd(m_Rect[i], this);
		pChartWnd[i]->m_Type = 0; // !!!!!!!!!!!!!!!!!
		pChartWnd[i]->m_SizeSmall = TRUE;
		pChartWnd[i]->m_Method3D = FALSE;
		pChartWnd[i]->m_LTLabel[0] = "";
		pWndSettings->m_Mask.GetName(pChartWnd[i]->m_LTLabel[1]);
		pChartWnd[i]->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
		pChartWnd[i]->CreateZoneButtons();
		m_pDispWnd[i] = pChartWnd[i];
	}

	pChartWnd[0]->m_MainLabels.Create(3);
	pChartWnd[0]->m_MainLabels[0] = "Sph Eq";
	pChartWnd[0]->m_MainLabels[1] = "Sph";
	pChartWnd[0]->m_MainLabels[2] = "Cyl";
	pChartWnd[0]->m_Colors.Create(3);
	pChartWnd[0]->m_Colors[0] = MAGENTA;
	pChartWnd[0]->m_Colors[1] = m_Printing ? ORANGE : YELLOW;
	pChartWnd[0]->m_Colors[2] = CYAN;
	pChartWnd[0]->m_YUnitLabel = "diopters";
	pChartWnd[0]->m_XUnitLabel = "series #";
	pChartWnd[0]->m_BLabels.Create(m_NumExams);
	for (int j = 0; j < m_NumExams; j++) {
		pChartWnd[0]->m_BLabels[j].Format(_T("%i"), m_ppWFExams[j]->m_Header.m_SeriesNumber);
	}
	pChartWnd[0]->m_Values.Create(m_NumExams, 3);

	pChartWnd[1]->m_MainLabels.Create(4);
	pChartWnd[1]->m_MainLabels[0] = "Total";
	pChartWnd[1]->m_MainLabels[1] = "LO Total";
	pChartWnd[1]->m_MainLabels[2] = "HO Total";
	pChartWnd[1]->m_MainLabels[3] = "Root Mean Square";
	pChartWnd[1]->m_Colors.Create(4);
	pChartWnd[1]->m_Colors[0] = MAGENTA;
	pChartWnd[1]->m_Colors[1] = m_Printing ? ORANGE : YELLOW;
	pChartWnd[1]->m_Colors[2] = CYAN;
	pChartWnd[1]->m_Colors[3] = m_Printing ? BLACK : WHITE;
	pChartWnd[1]->m_YUnitLabel = "micrometers";
	pChartWnd[1]->m_XUnitLabel = "series #";
	pChartWnd[1]->m_BLabels.Create(m_NumExams);
	for (int j = 0; j < m_NumExams; j++) {
		pChartWnd[1]->m_BLabels[j].Format(_T("%i"), m_ppWFExams[j]->m_Header.m_SeriesNumber);
	}
	pChartWnd[1]->m_Values.Create(m_NumExams, 3);

	pChartWnd[2]->m_MainLabels.Create(4);
	pChartWnd[2]->m_MainLabels[0] = "Min";
	pChartWnd[2]->m_MainLabels[1] = "Max";
	pChartWnd[2]->m_MainLabels[2] = "Center";
	pChartWnd[2]->m_MainLabels[3] = "Refraction";
	pChartWnd[2]->m_Colors.Create(4);
	pChartWnd[2]->m_Colors[0] = RED;
	pChartWnd[2]->m_Colors[1] = BLUE;
	pChartWnd[2]->m_Colors[2] = GREEN;
	pChartWnd[2]->m_Colors[3] = m_Printing ? BLACK : WHITE;
	pChartWnd[2]->m_YUnitLabel = "diopters";
	pChartWnd[2]->m_XUnitLabel = "series #";
	pChartWnd[2]->m_BLabels.Create(m_NumExams);
	pChartWnd[2]->m_TLabels.Create(m_NumExams);
	for (int j = 0; j < m_NumExams; j++) {
		pChartWnd[2]->m_BLabels[j].Format(_T("%i"), m_ppWFExams[j]->m_Header.m_SeriesNumber);
	}
	pChartWnd[2]->m_Values.Create(m_NumExams, 3);

	CZernikeMask MaskLo;
	MaskLo.SetType(MASK_LO_TOTAL);

	CZernikeMask MaskHo;
	MaskHo.SetType(MASK_HO_TOTAL);

	Matrix<real_t> rf_min(m_NumExams);
	Matrix<real_t> rf_max(m_NumExams);
	Matrix<real_t> x_min_um(m_NumExams);
	Matrix<real_t> y_min_um(m_NumExams);
	Matrix<real_t> x_max_um(m_NumExams);
	Matrix<real_t> y_max_um(m_NumExams);
	for (int j = 0; j < m_NumExams; j++) {

		CWFExam* pWFExam = m_ppWFExams[j];

		CZernikeSurface WfSurface;
		WfSurface = pWFExam->m_WfSurface;
		WfSurface.ChangeRMaxUm(r_max_um);
		WfSurface &= pWndSettings->m_Mask;
		if (m_diff) WfSurface -= WfSurface1;

		real_t SphEq, Sph, Cyl;
		int Axis;
		CWFExam::GetSpheqSphCylAxis(WfSurface, ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
		pChartWnd[0]->m_Values(j, 0) = SphEq;
		pChartWnd[0]->m_Values(j, 1) = Sph;
		pChartWnd[0]->m_Values(j, 2) = Cyl;

		pChartWnd[1]->m_Values(j, 0) = WfSurface.ComputeRms();
		CZernikeSurface SurfaceLo;
		SurfaceLo = WfSurface;
		SurfaceLo &= MaskLo;
		pChartWnd[1]->m_Values(j, 1) = SurfaceLo.ComputeRms();
		CZernikeSurface SurfaceHo;
		SurfaceHo = WfSurface;
		SurfaceHo &= MaskHo;
		pChartWnd[1]->m_Values(j, 2) = SurfaceHo.ComputeRms();

		CWFExam::GetMinMaxRf(WfSurface, rf_min[j], x_min_um[j], y_min_um[j], rf_max[j], x_max_um[j], y_max_um[j]);
		rf_min[j] = CWFExam::GetAverageRfAt(WfSurface, m_r_um, x_min_um[j], y_min_um[j]);
		rf_max[j] = CWFExam::GetAverageRfAt(WfSurface, m_r_um, x_max_um[j], y_max_um[j]);
		pChartWnd[2]->m_Values(j, 0) = rf_min[j];
		pChartWnd[2]->m_Values(j, 1) = rf_max[j];
		pChartWnd[2]->m_Values(j, 2) = CWFExam::GetAverageRfAt(WfSurface, m_r_um);

		real_t mx = rf_max[j];
		real_t mn = rf_min[j];
		pChartWnd[2]->m_TLabels[j].Format(_T("%.1f"), rf_max[j] - rf_min[j]);
	}

	real_t min, max;

	pChartWnd[0]->m_Values.GetMin(min);
	pChartWnd[0]->m_Values.GetMax(max);
	real_t t1 = __max(fabs(min), fabs(max));
	int t2 = (int)t1;
	if (t1 > t2 + 0.001) t2++;
	else if (t2 == 0) t2 = 1;
	pChartWnd[0]->m_max = t2;
	pChartWnd[0]->m_min = -pChartWnd[0]->m_max;

	pChartWnd[1]->m_Values.GetMax(max);
	t2 = (int)max;
	if (max > t2 + 0.001) t2++;
	else if (t2 == 0) t2 = 1;
	pChartWnd[1]->m_max = t2;
	pChartWnd[1]->m_min = 0;

	pChartWnd[2]->m_Values.GetMin(min);
	pChartWnd[2]->m_Values.GetMax(max);
	t1 = __max(fabs(min), fabs(max));
	t2 = (int)t1;
	if (t1 > t2 + 0.001) t2++;
	else if (t2 == 0) t2 = 1;
	pChartWnd[2]->m_max = t2;
	pChartWnd[2]->m_min = -pChartWnd[2]->m_max;

	for (int i = 0; i < 3; i++) {
		int m = pChartWnd[i]->m_max - pChartWnd[i]->m_min + 1;
		pChartWnd[i]->m_LLabels.Create(m);
		for (int j = 0; j < m; j++) {
			pChartWnd[i]->m_LLabels[j].Format(_T("%i"), pChartWnd[i]->m_min + j);
		}
	}

	{
		CWFExam* pWFExam = m_ppWFExams[m_series];

		CZernikeSurface WfSurface;
		WfSurface = pWFExam->m_WfSurface;
		WfSurface.ChangeRMaxUm(r_max_um);
		WfSurface &= pWndSettings->m_Mask;
		if (m_diff) WfSurface -= WfSurface1;

		CEyeWnd* pEyeWnd = new CEyeWnd(m_Rect[3], this);
		pEyeWnd->m_Type = TYPE_TRFM;
		pEyeWnd->m_SizeSmall = TRUE;
		pEyeWnd->m_Method3D = FALSE;
		pEyeWnd->m_LTLabel[0] = "Refraction Map";
		pWndSettings->m_Mask.GetName(pEyeWnd->m_LTLabel[1]);
		pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), WfSurface.m_r_max_um * 0.002);
		pEyeWnd->m_MapShowMap = TRUE;
		pEyeWnd->m_MapShowNumbers = FALSE;
		pEyeWnd->m_Unit = "D";
		pEyeWnd->m_Inc = pWndSettings->GetIncrement();
		CScale* pScale = pWndSettings->GetScale();
		pEyeWnd->m_NumColors = pScale->m_NumColors;
		for (int i = 0; i < pScale->m_NumColors; i++) {
			pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
		}
		pEyeWnd->m_pWFExam = pWFExam;
		CWFExam::CreateRfm2D(pEyeWnd->m_Map2D, WfSurface, 1.0 / pEyeWnd->m_y_px_um);
		pEyeWnd->m_Circles.Create(2);
		pEyeWnd->m_Circles[0].m_x_um = x_min_um[m_series];
		pEyeWnd->m_Circles[0].m_y_um = y_min_um[m_series];
		pEyeWnd->m_Circles[0].m_r_um = m_r_um;
		pEyeWnd->m_Circles[0].m_color = WHITE;
		pEyeWnd->m_Circles[1].m_x_um = x_max_um[m_series];
		pEyeWnd->m_Circles[1].m_y_um = y_max_um[m_series];
		pEyeWnd->m_Circles[1].m_r_um = m_r_um;
		pEyeWnd->m_Circles[1].m_color = WHITE;
		pEyeWnd->CreateCentButtons();
		pEyeWnd->CreateZoomButtons();
		pEyeWnd->CreateZoneButtons();
		m_pDispWnd[3] = pEyeWnd;

		real_t Inc = pWndSettings->GetIncrement();
		real_t Cent = 0.0;
		real_t Step;
		if (pScale->m_StepAuto) {
			real_t Min = pWndSettings->m_Method3D ? pEyeWnd->m_Map3D.m_min : pEyeWnd->m_Map2D.m_min;
			real_t Max = pWndSettings->m_Method3D ? pEyeWnd->m_Map3D.m_max : pEyeWnd->m_Map2D.m_max;
			real_t Span = 2.0 * __max(fabs(Min - Cent), fabs(Max - Cent));
			real_t t = Span / pScale->m_NumColors;
			Step = (int)(t / Inc) * Inc;
			if (t > Step + 0.001) Step += Inc;
		}
		else {
			Step = RealRound(pScale->m_Step, Inc);
		}
		if (Step < Inc) Step = Inc;
		real_t StepMax = pWndSettings->GetMaxStep();
		if (Step > StepMax) Step = StepMax;
		//
		pEyeWnd->m_Cent = Cent;
		pEyeWnd->m_Step = Step;
	}

}

//***************************************************************************************

void CWFMultiSumWnd::CreateMenu()
{
	CWndSettings* pWndSettings = m_pWndSettings;
	CXPMenu* pSubMenu;

	if (m_d == 3) {
		pSubMenu = m_pMenu->AppendSubmenu("Series #", "");
		CString s;
		for (int i = 0; i < m_NumExams; i++) {
			if (m_diff) s.Format(_T("%i - %i"), m_ppWFExams[i]->m_Header.m_SeriesNumber, m_ppWFExams[0]->m_Header.m_SeriesNumber);
			else s.Format(_T("%i"), m_ppWFExams[i]->m_Header.m_SeriesNumber);
			pSubMenu->AppendItem(IDC_SERIES_NUMBER_00_ITEM + i, m_series == i, FALSE, s, "");
		}
	}

	//CreateSubmenuZone(0);//530
	CreateSubmenuZone(1);//530
	CreateSubmenuMask();

	m_pMenu->AppendItem(IDC_DIFF_ITEM, FALSE, m_diff, "Difference", "");

	if (m_d == 2) {
		CString s;
		s.Format(_T("%i µ"), intRound(2.0 * m_r_um));
		m_pMenu->AppendItem(IDC_AVG_ZONE_ITEM, FALSE, FALSE, "Spot size", s);
	}

	if (m_d == 3) {
		m_pMenu->AppendSeparator();//521
		CreateSubmenuMapSteps();
		CreateSubmenuMapColors();
	}
}

//***************************************************************************************

void CWFMultiSumWnd::OnSeriesNumber(int series)
{
	if (m_series == series) return;

	m_series = series;

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
}

//***************************************************************************************

void CWFMultiSumWnd::OnDiffItemClicked()
{
	m_diff = !m_diff;

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
}

//***************************************************************************************

void CWFMultiSumWnd::OnAvgZoneItemClicked()
{
	real_t d_um;
	if (!::EnterNumber(d_um, this, "Enter a number in microns")) return;
	if (d_um < 0.0) d_um = 0.0; else if (d_um > 10000.0) d_um = 10000.0;
	m_r_um = 0.5 * d_um;

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
}

//***************************************************************************************

