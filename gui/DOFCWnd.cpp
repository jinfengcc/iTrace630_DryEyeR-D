/// The display for Depth of Focus Comparison

//迷雾汀芳，暗夜黄昏，浮动花影，适彼中秋

//寂静昭华，君子瑟琴，随云起舞，素手点星

//作者： 陈劲风

//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "DOFCWnd.h"


//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CDOFCWnd, CDispWnd)


END_MESSAGE_MAP()

//***************************************************************************************

CDOFCWnd::CDOFCWnd(RECT& Rect, Matrix<real>& X1, Matrix<real>& F1, Matrix<real>& X2, Matrix<real>& F2, real fraction1, real fraction2, CWnd* pWnd) : CDispWnd(Rect, pWnd)
{
	m_thr1 = 0.5;
	m_thr2 = 0.5;

	m_fraction1 = fraction1;
	m_fraction2 = fraction2;

	m_Spline1.Create(X1, F1);
	m_Spline2.Create(X2, F2);

	m_w_px = Rect.right - Rect.left;
	m_h_px = Rect.bottom - Rect.top;

	m_l_px = intRound(0.1 * m_w_px);
	m_r_px = intRound(0.9 * m_w_px);

	m_t_px = intRound(0.15 * m_h_px);
	m_b_px = intRound(0.88 * m_h_px);

	m_pos1 = INVALID_VALUE;

	m_imax1 = 0;
	m_xmax1 = 0.0;
	m_fmax1 = -DBL_MAX;
	for (int i = -1500; i <= 1500; i++)
	{
		real SphEqDpt = i * 3.0 / 1500;
		m_Spline1.GetAt(SphEqDpt, m_F1[i + 1500], TRUE);
		if (m_fmax1 < m_F1[i + 1500])
		{
			m_imax1 = i;
			m_xmax1 = SphEqDpt;
			m_fmax1 = m_F1[i + 1500];
		}
	}
	m_fmin1 = 0.0;

	m_pos2 = INVALID_VALUE;

	m_imax2 = 0;
	m_xmax2 = 0.0;
	m_fmax2 = -DBL_MAX;
	for (int i = -1500; i <= 1500; i++)
	{
		real SphEqDpt = i * 3.0 / 1500;
		m_Spline2.GetAt(SphEqDpt, m_F2[i + 1500], TRUE);
		if (m_fmax2 < m_F2[i + 1500])
		{
			m_imax2 = i;
			m_xmax2 = SphEqDpt;
			m_fmax2 = m_F2[i + 1500];
		}
	}
	m_fmin2 = 0.0;

	m_fmax = (m_fmax2 > m_fmax1 ? m_fmax2 : m_fmax1);
}

//***************************************************************************************

void CDOFCWnd::RepaintMemDC()
{
	if (m_SizeSmall) m_t_px = intRound(0.20 * m_h_px);
	else            m_t_px = intRound(0.15 * m_h_px);

	int xl_px1, xr_px1;
	int xl_px2, xr_px2;
	BOOL bl1 = FALSE;
	BOOL br1 = FALSE;
	BOOL bl2 = FALSE;
	BOOL br2 = FALSE;

	real L = 0.0;
	real R = 0.0;
	real L1 = 0.0;
	real R1 = 0.0;
	real L2 = 0.0;
	real R2 = 0.0;

	RECT Rect;

	::SetRect(&Rect, 0, 0, m_w_px, m_h_px);
	m_MemDC.FillSolidRect(&Rect, BLACK);

	m_LTLabel[0] = "Through-focus VSOTFa  - Absolute";

	Labels();
	Border();

	//Draw the chart 
	CString s;

	int fs0 = 15;
	int fs1 = 20;
	int fs2 = 30;
	int fs3 = 40;
	CMFont Font0(fs0, 400, "Arial");
	CMFont Font1(fs1, 400, "Arial");
	CMFont Font2(fs2, 400, "Arial");
	CMFont Font3(fs3, 400, "Arial");

	int LeftType = (m_fraction1 <= m_fraction2 ? 1 : 2);

	real leftVal = (m_fraction1 <= m_fraction2 ? m_fraction1 : m_fraction2);
	leftVal -= 3.0;

	real rightVal = (m_fraction1 > m_fraction2 ? m_fraction1 : m_fraction2);
	rightVal += 3.0;

	int interNum = intRound((rightVal - leftVal) / 0.25);

	if (interNum > 41) interNum = 40;

	int left = intRound(leftVal);

	// vertical lines
	for (int k = 0; k <= interNum; k++) {
		int x_px = m_l_px + intRound((m_r_px - m_l_px) * k / (interNum - 1));
		m_MemDC.DrawLine(x_px, m_t_px, x_px, m_b_px, 1, k == 12 ? WHITE : DARK_GRAY);
		::SetRect(&Rect, x_px - 100, m_b_px + 3, x_px + 100, m_b_px + 3 + fs1);
		s.Format(_T("%.2f"), left + k * 0.25);
		if (k % 2 == 0) m_MemDC.WriteText(s, Rect, Font0, WHITE, 1);
	}

	int rightR = m_l_px + intRound((m_r_px - m_l_px) * interNum / (interNum - 1));
	int right = interNum* 0.25 + left;

	int m_Exam1Highest_x = intRound(m_l_px + (rightR - m_l_px)*((m_fraction1 - left - 0.25) / real(right - left)));
	int m_Exam2Highest_x = intRound(m_l_px + (rightR - m_l_px)*((m_fraction2 - left - 0.25) / real(right - left)));

	real ratio = real(right - left) / real(rightR - m_l_px);

	// horizontal lines
	const int ny = GetMaxHorValue(m_fmax) + 1;
	for (int k = 0; k < ny; k++) {
		int y_px = m_b_px - intRound(k * (m_b_px - m_t_px) / (ny - 1));

		m_MemDC.DrawLine(m_l_px, y_px, m_r_px, y_px, 1, k == 0 ? WHITE : k == 5 ? LIGHT_GRAY : DARK_GRAY);
		::SetRect(&Rect, m_r_px + 8, y_px - 100, m_w_px, y_px + 100);
		s.Format(_T("%.1f"), k * 0.1);
		m_MemDC.WriteText(s, Rect, Font1, WHITE, 0);
	}


	int dis1, dis2;

	//Exam1
	// curve
	int x_HighestPx1 = 0;//new

	real qx = (m_r_px - m_l_px) / (m_Spline1.m_x[m_Spline1.m_n - 1] - m_Spline1.m_x[0]);
	real qy = (m_b_px - m_t_px) / (0.8 - m_fmin1);

	int y_HighestPx1 = m_b_px - intRound(qy * (m_fmax1 - m_fmin1));
	int y_ThrPx1 = m_b_px - intRound(qy * ((m_thr1 * m_fmax1) - m_fmin1));

	real difHighest = 10000;

	{
		int x_px_p, y_px_p;
		for (int x_px = m_l_px; x_px <= m_r_px; x_px++) {
			real x = m_Spline1.m_x[0] + (x_px - m_l_px) / qx;

			real f;
			m_Spline1.GetAt(x, f, TRUE);
			int y_px = m_b_px - intRound(qy * (f - m_fmin1));

			real thisDif = fabs(f - m_fmax1);
			if (thisDif < difHighest)
			{
				difHighest = thisDif;
				x_HighestPx1 = x_px;
			}

			/*	if (x_px > m_l_px)
			{
			m_MemDC.DrawLine(x_px_p, y_px_p, x_px, y_px, 2, BLUE);
			}
			*/
			x_px_p = x_px;
			y_px_p = y_px;
		}

		dis1 = m_Exam1Highest_x - x_HighestPx1;

		for (int x_px = m_l_px; x_px <= m_r_px; x_px++) {
			real x = m_Spline1.m_x[0] + (x_px - m_l_px) / qx;

			real f;
			m_Spline1.GetAt(x, f, TRUE);
			int y_px = m_b_px - intRound(qy * (f - m_fmin1));

			if (x_px > m_l_px)
			{
				m_MemDC.DrawLine(x_px_p + dis1, y_px_p, x_px + dis1, y_px, 2, BLUE);
			}

			x_px_p = x_px;
			y_px_p = y_px;
		}
	}


	// nodes
	/*for (int k = 0; k < m_Spline1.m_n; k++) {
	int x_px = m_l_px + intRound(qx * (m_Spline1.m_x[k] - m_Spline1.m_x[0]));
	int y_px = m_b_px - intRound(qy * (m_Spline1.m_f[k] - m_fmin1));
	m_MemDC.DrawCircle(x_px, y_px, 2, 1, WHITE, WHITE);
	}	*/

	// threshold

	if (m_thr1 != INVALID_VALUE) {

		int m = 1500;
		if (m_fmax1 != 0.0) {
			for (int i = m_imax1; i > -m; i--) {
				if (m_F1[m + i - 1] / m_fmax1 <= m_thr1 && m_thr1 <= m_F1[m + i] / m_fmax1) {
					L1 = i * 3.0 / m;
					bl1 = TRUE;
					break;
				}
			}
			for (int i = m_imax1; i < m; i++) {
				if (m_F1[m + i] / m_fmax1 >= m_thr1 && m_thr1 >= m_F1[m + i + 1] / m_fmax1) {
					R1 = i * 3.0 / m;
					br1 = TRUE;
					break;
				}
			}
		}


		if (bl1) {
			xl_px1 = m_l_px + intRound(qx * (L1 - m_Spline1.m_x[0])) + dis1;
		}
		if (br1) {
			xr_px1 = m_l_px + intRound(qx * (R1 - m_Spline1.m_x[0])) + dis1;
		}
		if (bl1 && br1) {
			::SetRect(&Rect, (xl_px1 + xr_px1) / 2 - 80, y_ThrPx1 + 10, (xl_px1 + xr_px1) / 2 + 80, y_ThrPx1 + 40);
			s.Format(_T("DOF = %.2f D"), R1 - L1);
			m_MemDC.WriteText(s, Rect, Font2, BLUE, 1);
		}
	}

	//
	if (m_pos1 != INVALID_VALUE) {

		m_MemDC.DrawDottedLine(m_pos1, m_t_px, m_pos1, m_b_px, CYAN);

		::SetRect(&Rect, m_pos1 - 50, m_b_px + 20, m_pos1 + 50, m_b_px + 50);
		real x = m_Spline1.m_x[0] + (m_pos1 - m_l_px) / qx;
		s.Format(_T("%.2f D"), x);
		m_MemDC.WriteText(s, Rect, Font2, CYAN, 1);
	}
	//Exam1 Done  









	//Exam2
	// curve
	qx = (m_r_px - m_l_px) / (m_Spline2.m_x[m_Spline2.m_n - 1] - m_Spline2.m_x[0]);
	qy = (m_b_px - m_t_px) / (0.8 - m_fmin2);

	int x_HighestPx2 = 0;//new

	int y_HighestPx2 = m_b_px - intRound(qy * (m_fmax2 - m_fmin2));
	int y_ThrPx2 = m_b_px - intRound(qy * ((m_thr1 * m_fmax2) - m_fmin2));

	difHighest = 10000;
	{
		int x_px_p, y_px_p;
		for (int x_px = m_l_px; x_px <= m_r_px; x_px++) {
			real x = m_Spline2.m_x[0] + (x_px - m_l_px) / qx;
			real f;
			m_Spline2.GetAt(x, f, TRUE);

			real thisDif = fabs(f - m_fmax2);
			if (thisDif < difHighest)
			{
				difHighest = thisDif;
				x_HighestPx2 = x_px;
			}

			int y_px = m_b_px - intRound(qy * (f - m_fmin2));
			//if (x_px > m_l_px) m_MemDC.DrawLine(x_px_p, y_px_p, x_px, y_px, 2, MAGENTA);
			x_px_p = x_px;
			y_px_p = y_px;
		}

		dis2 = m_Exam2Highest_x - x_HighestPx2;

		for (int x_px = m_l_px; x_px <= m_r_px; x_px++) {
			real x = m_Spline2.m_x[0] + (x_px - m_l_px) / qx;
			real f;
			m_Spline2.GetAt(x, f, TRUE);

			int y_px = m_b_px - intRound(qy * (f - m_fmin2));
			if (x_px > m_l_px) m_MemDC.DrawLine(x_px_p + dis2, y_px_p, x_px + dis2, y_px, 2, MAGENTA);
			x_px_p = x_px;
			y_px_p = y_px;
		}
	}


	// nodes
	/*for (int k = 0; k < m_Spline2.m_n; k++) {
	int x_px = m_l_px + intRound(qx * (m_Spline2.m_x[k] - m_Spline2.m_x[0]));
	int y_px = m_b_px - intRound(qy * (m_Spline2.m_f[k] - m_fmin2));
	m_MemDC.DrawCircle(x_px, y_px, 2, 1, WHITE, WHITE);
	}	*/

	// threshold

	if (m_thr2 != INVALID_VALUE) {

		int m = 1500;
		if (m_fmax2 != 0.0) {
			for (int i = m_imax2; i > -m; i--) {
				if (m_F2[m + i - 1] / m_fmax2 <= m_thr2 && m_thr2 <= m_F2[m + i] / m_fmax2) {
					L2 = i * 3.0 / m;
					bl2 = TRUE;
					break;
				}
			}
			for (int i = m_imax2; i < m; i++) {
				if (m_F2[m + i] / m_fmax2 >= m_thr2 && m_thr2 >= m_F2[m + i + 1] / m_fmax2) {
					R2 = i * 3.0 / m;
					br2 = TRUE;
					break;
				}
			}
		}

		if (bl2) {
			xl_px2 = m_l_px + intRound(qx * (L2 - m_Spline2.m_x[0])) + dis2;
		}
		if (br2) {
			xr_px2 = m_l_px + intRound(qx * (R2 - m_Spline2.m_x[0])) + dis2;
		}
		if (bl2 && br2) {
			::SetRect(&Rect, (xl_px1 + xr_px1) / 2 - 80, y_ThrPx1 + 10, (xl_px1 + xr_px1) / 2 + 80, y_ThrPx1 + 40);

			if (y_ThrPx1 > y_ThrPx2 && y_ThrPx1 < y_ThrPx2 + 30)
			{
				::SetRect(&Rect, (xl_px2 + xr_px2) / 2 - 80, y_ThrPx1 - 30, (xl_px2 + xr_px2) / 2 + 80, y_ThrPx1);
			}
			else if (y_ThrPx2 > y_ThrPx1 && y_ThrPx2 < y_ThrPx1 + 30)
			{
				::SetRect(&Rect, (xl_px2 + xr_px2) / 2 - 80, y_ThrPx1 + 50, (xl_px2 + xr_px2) / 2 + 80, y_ThrPx1 + 80);
			}
			else
			{
				::SetRect(&Rect, (xl_px2 + xr_px2) / 2 - 80, y_ThrPx2 + 10, (xl_px2 + xr_px2) / 2 + 80, y_ThrPx2 + 40);
			}

			s.Format(_T("DOF = %.2f D"), R2 - L2);
			m_MemDC.WriteText(s, Rect, Font2, MAGENTA, 1);
		}
	}

	//
	if (m_pos2 != INVALID_VALUE) {

		m_MemDC.DrawDottedLine(m_pos2, m_t_px, m_pos2, m_b_px, CYAN);

		::SetRect(&Rect, m_pos2 - 50, m_b_px + 20, m_pos2 + 50, m_b_px + 50);

		real x = m_Spline2.m_x[0] + (m_pos2 - m_l_px) / qx;
		s.Format(_T("%.2f D"), x);
		m_MemDC.WriteText(s, Rect, Font2, CYAN, 1);
	}
	//Exam2 Done  

	if (x_HighestPx1 <= x_HighestPx2)
	{
		m_MemDC.DrawDottedLine(m_l_px, y_HighestPx1, m_Exam1Highest_x, y_HighestPx1, WHITE);
		::SetRect(&Rect, m_l_px + 8, y_HighestPx1 - 20, m_r_px, y_HighestPx1);
		m_MemDC.WriteText("100%", Rect, Font1, WHITE, 0);

		m_MemDC.DrawDottedLine(m_Exam2Highest_x, y_HighestPx2, m_r_px, y_HighestPx2, WHITE);
		::SetRect(&Rect, m_l_px + 8, y_HighestPx2 - 20, m_r_px, y_HighestPx2);
		m_MemDC.WriteText("100%", Rect, Font1, WHITE, 2);
	}
	else
	{
		m_MemDC.DrawDottedLine(m_Exam1Highest_x, y_HighestPx1, m_r_px, y_HighestPx1, WHITE);
		::SetRect(&Rect, m_l_px + 8, y_HighestPx1 - 20, m_r_px, y_HighestPx1);
		m_MemDC.WriteText("100%", Rect, Font1, WHITE, 2);

		m_MemDC.DrawDottedLine(m_l_px, y_HighestPx2, m_Exam2Highest_x, y_HighestPx2, WHITE);
		::SetRect(&Rect, m_l_px + 8, y_HighestPx2 - 20, m_r_px, y_HighestPx2);
		m_MemDC.WriteText("100%", Rect, Font1, WHITE, 0);
	}

	//Position Lines
	if (bl1)
	{
		s.Format(_T("%.1f"), 100.0 * m_thr1);
		s += "%";
		if (m_Exam1Highest_x <= m_Exam2Highest_x)
		{
			m_MemDC.DrawDottedLine(m_l_px, y_ThrPx1, xl_px1, y_ThrPx1, GREEN);
			::SetRect(&Rect, 0, y_ThrPx1 - 30, m_l_px - 5, y_ThrPx1 + 30);
			m_MemDC.WriteText(s, Rect, Font1, GREEN, 2);
		}
		else
		{
			m_MemDC.DrawDottedLine(xr_px1, y_ThrPx1, m_r_px, y_ThrPx1, GREEN);
			::SetRect(&Rect, m_r_px + 10, y_ThrPx1 - 30, m_w_px, y_ThrPx1 + 30);
			m_MemDC.WriteText(s, Rect, Font1, GREEN, 0);
		}
	}

	if (br2)
	{
		s.Format(_T("%.1f"), 100.0 * m_thr2);
		s += "%";

		if (m_Exam1Highest_x <= m_Exam2Highest_x)
		{
			m_MemDC.DrawDottedLine(xr_px2, y_ThrPx2, m_r_px, y_ThrPx2, GREEN);
			::SetRect(&Rect, m_r_px + 10, y_ThrPx2 - 30, m_w_px, y_ThrPx2 + 30);

			m_MemDC.WriteText(s, Rect, Font1, GREEN, 0);
		}
		else
		{
			m_MemDC.DrawDottedLine(m_l_px, y_ThrPx2, xl_px2, y_ThrPx2, GREEN);
			::SetRect(&Rect, 0, y_ThrPx2 - 30, m_l_px - 5, y_ThrPx2 + 30);

			m_MemDC.WriteText(s, Rect, Font1, GREEN, 2);
		}
	}

	int  selExamNumber = 1;
	BOOL bl = FALSE;
	BOOL br = FALSE;
	int xl_px, xr_px;
	if (bl1 && bl2)
	{
		if (m_Exam1Highest_x <= m_Exam2Highest_x)
		{
			xl_px = xl_px1;
			L = L1;
		}
		else
		{
			xl_px = xl_px2;
			L = L2;
		}
		bl = TRUE;
	}
	if (br1 && br2)
	{
		if (m_Exam1Highest_x <= m_Exam2Highest_x)
		{
			xr_px = xr_px2;
			R = R2;
		}
		else
		{
			xr_px = xr_px1;
			R = R1;
		}
		br = TRUE;
	}

	if (bl && br)
	{
		m_MemDC.DrawLine(xl_px, m_t_px, xl_px, m_b_px, 1, YELLOW);
		m_MemDC.DrawLine(xr_px, m_t_px, xr_px, m_b_px, 1, YELLOW);

		//Effective Range of Focus
		real DisD = real(xr_px - xl_px) * ratio;

		int y_Eff_px = y_HighestPx1 > y_HighestPx2 ? y_HighestPx2 : y_HighestPx1;

		if (y_Eff_px - 30 <= 0) y_Eff_px = 90;

		int x0 = xr_px;
		int y0 = y_Eff_px - 30;

		int x1 = x0 - 8;
		int y1 = y0 - 6;

		int x2 = x1;
		int y2 = y0 + 6;

		m_MemDC.DrawLine(xl_px, y_Eff_px - 30, xr_px, y_Eff_px - 30, 1, YELLOW);
		m_MemDC.DrawTriangle(x0, y0, x1, y1, x2, y2, 1, YELLOW, YELLOW);

		x0 = xl_px;
		x2 = x1 = x0 + 8;
		m_MemDC.DrawTriangle(x0, y0, x1, y1, x2, y2, 1, YELLOW, YELLOW);

		m_MemDC.MeasureRect("Effective Range of Focus = 3.65D  ", Rect, Font1);

		int Width = Rect.right - Rect.left;

		if (Width >= (xr_px - xl_px))
		{
			int midWidth = intRound((real)Width / 2.0);
			int mid = xl_px + intRound((real)(xr_px - xl_px) / 2.0);

			::SetRect(&Rect, mid - midWidth, y_Eff_px - 55, mid + midWidth, y_Eff_px - 35);
		}
		else
		{
			::SetRect(&Rect, xl_px, y_Eff_px - 55, xr_px, y_Eff_px - 35);
		}

		s.Format(_T("Effective Range of Focus = %.2fD"), DisD);
		m_MemDC.WriteText(s, Rect, Font1, WHITE, 1);
		//Effective Range of Focus Done
	}
	//Done+

	//y_HighestPx1, x_HighestPx1
	m_MemDC.DrawDottedLine(m_Exam1Highest_x, y_HighestPx1, m_Exam1Highest_x, m_b_px + 30, BLUE);
	::SetRect(&Rect, m_Exam1Highest_x - 25, m_b_px + 33, m_Exam1Highest_x + 25, m_b_px + 30 + 3 + fs1);
	s.Format(_T("%.2fD"), m_fraction1);
	m_MemDC.WriteText(s, Rect, Font1, BLUE, 1);

	m_MemDC.DrawDottedLine(m_Exam2Highest_x, y_HighestPx2, m_Exam2Highest_x, m_b_px + 50, MAGENTA);
	::SetRect(&Rect, m_Exam2Highest_x - 25, m_b_px + 53, m_Exam2Highest_x + 25, m_b_px + 53 + fs1);
	s.Format(_T("%.2fD"), m_fraction2);
	m_MemDC.WriteText(s, Rect, Font1, MAGENTA, 1);

	//

}

//***************************************************************************************
int CDOFCWnd::GetMaxHorValue(real fmax)
{
	int val = intRound(fmax * 10 + 0.5);//fix by 0711 2018
	if (val == 1) val = 2;
	return val;
}