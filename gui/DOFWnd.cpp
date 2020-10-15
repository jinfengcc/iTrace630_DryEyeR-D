//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "DOFWnd.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CDOFWnd, CDispWnd)

	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()

	ON_BN_CLICKED(IDC_DOF_SWITCH, OnSwitchBtnClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CDOFWnd::CDOFWnd(RECT& Rect, Matrix<real_t>& X, Matrix<real_t>& F, CWnd* pWnd) : CDispWnd(Rect, pWnd)
{
	m_abs = TRUE;//6.2.1 01092019
	m_thr = 0.5;

	m_Spline.Create(X, F);

	m_w_px = Rect.right - Rect.left;
	m_h_px = Rect.bottom - Rect.top;

	m_l_px = intRound(0.2 * m_w_px);
	m_r_px = intRound(0.95 * m_w_px);

	if (m_SizeSmall) m_t_px = intRound(0.20 * m_h_px);
	else            m_t_px = intRound(0.15 * m_h_px);
	m_b_px = intRound(0.88 * m_h_px);

	m_pos = INVALID_VALUE;

	m_imax = 0;
	m_xmax = 0.0;
	m_fmax = -DBL_MAX;
	for (int i = -1500; i <= 1500; i++)
	{
		real_t SphEqDpt = i * 3.0 / 1500;
		m_Spline.GetAt(SphEqDpt, m_F[i + 1500], TRUE);
		if (m_fmax < m_F[i + 1500])
		{
			m_imax = i;
			m_xmax = SphEqDpt;
			m_fmax = m_F[i + 1500];
		}
	}
	m_fmin = 0.0;
}

//***************************************************************************************

void CDOFWnd::RepaintMemDC()
{
	if (m_SizeSmall) m_t_px = intRound(0.20 * m_h_px);
	else            m_t_px = intRound(0.15 * m_h_px);

	RECT Rect;

	::SetRect(&Rect, 0, 0, m_w_px, m_h_px);
	m_MemDC.FillSolidRect(&Rect, BLACK);

	if (m_abs) m_LTLabel[0] = "Through-focus VSOTFa  - Absolute";
	else      m_LTLabel[0] = "Through-focus VSOTFa  - Relative [%]";

	Labels();
	Border();

	//Draw the chart 
	CString s;
	if (m_SizeSmall)
	{
		int fs1 = intRound(0.04*m_h_px);
		int fs2 = intRound(0.05*m_h_px);
		int fs3 = intRound(0.045*m_h_px);

		CMFont Font1(fs1, 400, "Arial");
		CMFont Font2(fs2, 400, "Arial");
		CMFont Font3(fs3, 400, "Arial");

		// vertical lines
		for (int k = 0; k < 25; k = k + 2) {
			int x_px = m_l_px + intRound((m_r_px - m_l_px) * k / (25 - 1));
			m_MemDC.DrawLine(x_px, m_t_px, x_px, m_b_px, 1, k == 12 ? WHITE : DARK_GRAY);
			::SetRect(&Rect, x_px - 100, m_b_px + 3, x_px + 100, m_b_px + 3 + fs1);
			s.Format(_T("%.2f"), (k - 12) * 0.25);
			m_MemDC.WriteText(s, Rect, Font1, WHITE, 1);
		}

		// horizontal lines
		const int ny = m_abs ? 9 : 11;
		for (int k = 0; k < ny; k++) {
			int y_px = m_b_px - intRound(k * (m_b_px - m_t_px) / (ny - 1));
			m_MemDC.DrawLine(m_l_px, y_px, m_r_px, y_px, 1, k == 0 ? WHITE : k == 5 && !m_abs ? LIGHT_GRAY : DARK_GRAY);
			if (m_abs) {
				::SetRect(&Rect, m_r_px + 8, y_px - 100, m_w_px, y_px + 100);
				s.Format(_T("%.1f"), k * 0.1);
				m_MemDC.WriteText(s, Rect, Font1, WHITE, 0);
			}
			else {
				::SetRect(&Rect, 0, y_px - 100, m_l_px - 8, y_px + 100);
				s.Format(_T("%i"), k * 10);
				s += "%";
				m_MemDC.WriteText(s, Rect, Font1, WHITE, 2);
			}
		}

		// curve
		real_t qx = (m_r_px - m_l_px) / (m_Spline.m_x[m_Spline.m_n - 1] - m_Spline.m_x[0]);
		real_t qy = (m_b_px - m_t_px) / ((m_abs ? 0.8 : m_fmax) - m_fmin);

		{
			int x_px_p, y_px_p;
			for (int x_px = m_l_px; x_px <= m_r_px; x_px++) {
				real_t x = m_Spline.m_x[0] + (x_px - m_l_px) / qx;
				real_t f;
				m_Spline.GetAt(x, f, TRUE);
				int y_px = m_b_px - intRound(qy * (f - m_fmin));
				if (x_px > m_l_px) m_MemDC.DrawLine(x_px_p, y_px_p, x_px, y_px, 2, RED);
				x_px_p = x_px;
				y_px_p = y_px;
			}
		}

		//
		{
			int y_px = m_b_px - intRound(qy * (m_fmax - m_fmin));
			m_MemDC.DrawDottedLine(m_l_px, y_px, m_r_px, y_px, WHITE);
			::SetRect(&Rect, 0, y_px - 20, m_r_px - 8, y_px);
			s.Format(_T("VSOTF = %.3f"), m_fmax);
			m_MemDC.WriteText(s, Rect, Font1, WHITE, 2);
			::SetRect(&Rect, m_l_px + 8, y_px - 20, m_r_px, y_px);
			m_MemDC.WriteText("100%", Rect, Font1, WHITE, 0);
		}

		// nodes
		for (int k = 0; k < m_Spline.m_n; k++) {
			int x_px = m_l_px + intRound(qx * (m_Spline.m_x[k] - m_Spline.m_x[0]));
			int y_px = m_b_px - intRound(qy * (m_Spline.m_f[k] - m_fmin));
			m_MemDC.DrawCircle(x_px, y_px, 2, 1, WHITE, WHITE);
		}

		// max
		{
			int x_px = m_l_px + intRound(qx * (m_xmax - m_Spline.m_x[0]));
			m_MemDC.DrawDottedLine(x_px, m_t_px - 25, x_px, m_b_px, WHITE);
			/*::SetRect(&Rect, x_px-70, m_t_px-20, x_px+70, m_t_px);*/
			if (m_xmax >= 2.75)
				::SetRect(&Rect, x_px - 140, m_t_px - 30 - fs1, x_px, m_t_px - 25);
			else ::SetRect(&Rect, x_px - 70, m_t_px - 30 - fs1, x_px + 70, m_t_px - 25);
			s.Format(_T("Offset = %.2f D"), m_xmax);
			m_MemDC.WriteText(s, Rect, Font1, WHITE, 1);
		}

		// threshold

		if (m_thr != INVALID_VALUE) {

			real_t L = 0.0;
			real_t R = 0.0;
			BOOL bl = FALSE;
			BOOL br = FALSE;

			int m = 1500;
			if (m_fmax != 0.0) {
				for (int i = m_imax; i > -m; i--) {
					if (m_F[m + i - 1] / m_fmax <= m_thr && m_thr <= m_F[m + i] / m_fmax) {
						L = i * 3.0 / m;
						bl = TRUE;
						break;
					}
				}
				for (int i = m_imax; i < m; i++) {
					if (m_F[m + i] / m_fmax >= m_thr && m_thr >= m_F[m + i + 1] / m_fmax) {
						R = i * 3.0 / m;
						br = TRUE;
						break;
					}
				}
			}

			int y_px = m_b_px - intRound(qy * ((m_thr * m_fmax) - m_fmin));

			m_MemDC.DrawDottedLine(m_l_px, y_px, m_r_px, y_px, GREEN);
			if (m_abs) ::SetRect(&Rect, 0, y_px - 30, m_l_px - 5, y_px + 30);
			else      ::SetRect(&Rect, 0, y_px - 30, m_l_px - 30, y_px + 30);
			s.Format(_T("%.1f"), 100.0 * m_thr);
			s += "%";
			m_MemDC.WriteText(s, Rect, Font1, GREEN, 1);

			int xl_px, xr_px;
			if (bl) {
				xl_px = m_l_px + intRound(qx * (L - m_Spline.m_x[0]));
				m_MemDC.DrawDottedLine(xl_px, m_t_px, xl_px, m_b_px, YELLOW);
			}
			if (br) {
				xr_px = m_l_px + intRound(qx * (R - m_Spline.m_x[0]));
				m_MemDC.DrawDottedLine(xr_px, m_t_px, xr_px, m_b_px, YELLOW);
			}
			if (bl && br) {
				::SetRect(&Rect, (xl_px + xr_px) / 2 - 80, y_px + 10, (xl_px + xr_px) / 2 + 80, y_px + 40);
				s.Format(_T("DOF = %.2f D"), R - L);
				m_MemDC.WriteText(s, Rect, Font2, YELLOW, 1);
			}
		}

		//
		if (m_pos != INVALID_VALUE) {

			m_MemDC.DrawDottedLine(m_pos, m_t_px, m_pos, m_b_px, CYAN);

			::SetRect(&Rect, m_pos - 50, m_b_px + 20, m_pos + 50, m_b_px + 50);
			real_t x = m_Spline.m_x[0] + (m_pos - m_l_px) / qx;
			s.Format(_T("%.2f D"), x);
			m_MemDC.WriteText(s, Rect, Font2, CYAN, 1);
		}
	}
	else
	{
		int fs0 = 15;
		int fs1 = 20;
		int fs2 = 30;
		int fs3 = 40;
		CMFont Font0(fs0, 400, "Arial");
		CMFont Font1(fs1, 400, "Arial");
		CMFont Font2(fs2, 400, "Arial");
		CMFont Font3(fs3, 400, "Arial");

		// vertical lines
		for (int k = 0; k < 25; k++) {
			int x_px = m_l_px + intRound((m_r_px - m_l_px) * k / (25 - 1));
			m_MemDC.DrawLine(x_px, m_t_px, x_px, m_b_px, 1, k == 12 ? WHITE : DARK_GRAY);
			::SetRect(&Rect, x_px - 100, m_b_px + 3, x_px + 100, m_b_px + 3 + fs1);
			s.Format(_T("%.2f"), (k - 12) * 0.25);
			if (k % 2 == 0) m_MemDC.WriteText(s, Rect, Font0, WHITE, 1);
		}

		// horizontal lines
		const int ny = m_abs ? 9 : 11;
		for (int k = 0; k < ny; k++) {
			int y_px = m_b_px - intRound(k * (m_b_px - m_t_px) / (ny - 1));
			m_MemDC.DrawLine(m_l_px, y_px, m_r_px, y_px, 1, k == 0 ? WHITE : k == 5 && !m_abs ? LIGHT_GRAY : DARK_GRAY);
			if (m_abs) {
				::SetRect(&Rect, m_r_px + 8, y_px - 100, m_w_px, y_px + 100);
				s.Format(_T("%.1f"), k * 0.1);
				m_MemDC.WriteText(s, Rect, Font1, WHITE, 0);
			}
			else {
				::SetRect(&Rect, 0, y_px - 100, m_l_px - 8, y_px + 100);
				s.Format(_T("%i"), k * 10);
				s += "%";
				m_MemDC.WriteText(s, Rect, Font1, WHITE, 2);
			}
		}

		// curve
		real_t qx = (m_r_px - m_l_px) / (m_Spline.m_x[m_Spline.m_n - 1] - m_Spline.m_x[0]);
		real_t qy = (m_b_px - m_t_px) / ((m_abs ? 0.8 : m_fmax) - m_fmin);

		{
			int x_px_p, y_px_p;
			for (int x_px = m_l_px; x_px <= m_r_px; x_px++) {
				real_t x = m_Spline.m_x[0] + (x_px - m_l_px) / qx;
				real_t f;
				m_Spline.GetAt(x, f, TRUE);
				int y_px = m_b_px - intRound(qy * (f - m_fmin));
				if (x_px > m_l_px) m_MemDC.DrawLine(x_px_p, y_px_p, x_px, y_px, 2, RED);
				x_px_p = x_px;
				y_px_p = y_px;
			}
		}

		//
		{
			int y_px = m_b_px - intRound(qy * (m_fmax - m_fmin));
			m_MemDC.DrawDottedLine(m_l_px, y_px, m_r_px, y_px, WHITE);
			::SetRect(&Rect, 0, y_px - 20, m_r_px - 8, y_px);
			s.Format(_T("VSOTF = %.3f"), m_fmax);
			m_MemDC.WriteText(s, Rect, Font1, WHITE, 2);
			::SetRect(&Rect, m_l_px + 8, y_px - 20, m_r_px, y_px);
			m_MemDC.WriteText("100%", Rect, Font1, WHITE, 0);
		}

		// nodes
		for (int k = 0; k < m_Spline.m_n; k++) {
			int x_px = m_l_px + intRound(qx * (m_Spline.m_x[k] - m_Spline.m_x[0]));
			int y_px = m_b_px - intRound(qy * (m_Spline.m_f[k] - m_fmin));
			m_MemDC.DrawCircle(x_px, y_px, 2, 1, WHITE, WHITE);
		}

		// max
		{
			int x_px = m_l_px + intRound(qx * (m_xmax - m_Spline.m_x[0]));
			m_MemDC.DrawDottedLine(x_px, m_t_px - 25, x_px, m_b_px, WHITE);
			//::SetRect(&Rect, x_px-70, m_t_px-20, x_px+70, m_t_px);
			if (m_xmax >= 2.75)
				::SetRect(&Rect, x_px - 140, m_t_px - 30 - fs1, x_px, m_t_px - 25);
			else ::SetRect(&Rect, x_px - 70, m_t_px - 30 - fs1, x_px + 70, m_t_px - 25);
			s.Format(_T("Offset = %.2f D"), m_xmax);
			m_MemDC.WriteText(s, Rect, Font1, WHITE, 1);
		}

		// threshold

		if (m_thr != INVALID_VALUE) {

			real_t L = 0.0;
			real_t R = 0.0;
			BOOL bl = FALSE;
			BOOL br = FALSE;

			int m = 1500;
			if (m_fmax != 0.0) {
				for (int i = m_imax; i > -m; i--) {
					if (m_F[m + i - 1] / m_fmax <= m_thr && m_thr <= m_F[m + i] / m_fmax) {
						L = i * 3.0 / m;
						bl = TRUE;
						break;
					}
				}
				for (int i = m_imax; i < m; i++) {
					if (m_F[m + i] / m_fmax >= m_thr && m_thr >= m_F[m + i + 1] / m_fmax) {
						R = i * 3.0 / m;
						br = TRUE;
						break;
					}
				}
			}

			int y_px = m_b_px - intRound(qy * ((m_thr * m_fmax) - m_fmin));

			m_MemDC.DrawDottedLine(m_l_px, y_px, m_r_px, y_px, GREEN);
			if (m_abs) ::SetRect(&Rect, 10, y_px - 30, m_l_px - 10, y_px + 30);
			else      ::SetRect(&Rect, 0, y_px - 30, m_l_px - 30, y_px + 30);
			s.Format(_T("%.1f"), 100.0 * m_thr);
			s += "%";
			m_MemDC.WriteText(s, Rect, Font1, GREEN, 1);

			int xl_px, xr_px;
			if (bl) {
				xl_px = m_l_px + intRound(qx * (L - m_Spline.m_x[0]));
				m_MemDC.DrawDottedLine(xl_px, m_t_px, xl_px, m_b_px, YELLOW);
			}
			if (br) {
				xr_px = m_l_px + intRound(qx * (R - m_Spline.m_x[0]));
				m_MemDC.DrawDottedLine(xr_px, m_t_px, xr_px, m_b_px, YELLOW);
			}
			if (bl && br) {
				::SetRect(&Rect, (xl_px + xr_px) / 2 - 80, y_px + 10, (xl_px + xr_px) / 2 + 80, y_px + 40);
				s.Format(_T("DOF = %.2f D"), R - L);
				m_MemDC.WriteText(s, Rect, Font2, YELLOW, 1);
			}
		}

		//
		if (m_pos != INVALID_VALUE) {

			m_MemDC.DrawDottedLine(m_pos, m_t_px, m_pos, m_b_px, CYAN);

			::SetRect(&Rect, m_pos - 50, m_b_px + 20, m_pos + 50, m_b_px + 50);
			real_t x = m_Spline.m_x[0] + (m_pos - m_l_px) / qx;
			s.Format(_T("%.2f D"), x);
			m_MemDC.WriteText(s, Rect, Font2, CYAN, 1);
		}
	}
	//Done  
}

//***************************************************************************************

void CDOFWnd::OnMouseMove(uint nFlags, CPoint Point)
{
	CDispWnd::OnMouseMove(nFlags, Point);
	m_mx_px = Point.x;
	m_my_px = Point.y;

	if (nFlags & MK_LBUTTON) {
		m_thr = (real_t)(m_b_px - m_my_px) / (m_b_px - m_t_px);
		if (m_abs) m_thr *= 0.8 / m_fmax;
		if (m_thr < 0.0 || m_thr > 1.0) m_thr = INVALID_VALUE;
	}
	else {
		if (m_t_px <= m_my_px && m_my_px <= m_b_px && m_l_px <= m_mx_px && m_mx_px <= m_r_px) {
			m_pos = m_mx_px;
		}
		else {
			m_pos = INVALID_VALUE;
		}
	}

	Repaint();
}
//***************************************************************************************

void CDOFWnd::OnLButtonDown(uint nFlags, CPoint Point)
{
	CDispWnd::OnLButtonDown(nFlags, Point);

	OnMouseMove(nFlags, Point);
}

//***************************************************************************************

void CDOFWnd::CreateSwitchBtn()
{
	RECT Rect;
	CMRgn Rgn;
	int w = intRound(0.058 * m_w); if (w < 25) w = 25;

	int W = ::GetSystemMetrics(SM_CXSCREEN);

	int r = m_w - 2;
	int b = m_h - 2;
	SetRect(&Rect, r - w, 2, r, 2 + w);

	m_SwitchButton.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_DOF_SWITCH);

	if (!m_abs) m_SwitchButton.SetImage(m_SizeSmall ?
		(W == 1920 ? IDR_DOF_REL_3 : W == 1600 ? IDR_DOF_REL_2 : IDR_DOF_REL_1) :
		(W == 1920 ? IDR_DOF_REL_6 : W == 1600 ? IDR_DOF_REL_5 : IDR_DOF_REL_4));
	else       m_SwitchButton.SetImage(m_SizeSmall ?
		(W == 1920 ? IDR_DOF_ABS_3 : W == 1600 ? IDR_DOF_ABS_2 : IDR_DOF_ABS_1) :
		(W == 1920 ? IDR_DOF_ABS_6 : W == 1600 ? IDR_DOF_ABS_5 : IDR_DOF_ABS_4));

	m_SwitchButton.SetBk(FALSE);
	m_SwitchButton.SetBkColor(BLACK);
	m_SwitchButton.SetTip("Switch between Absolute and Relative");
	Rgn.SetRectRgn(&Rect);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	m_SwitchButton.ShowWindow(SW_SHOW);
	m_SwitchButton.ShowWindow(SW_SHOW);
}


//***************************************************************************************
void CDOFWnd::OnSwitchBtnClicked()
{
	m_abs = !m_abs;

	if (m_abs)
	{
		m_l_px = intRound(0.1 * m_w_px);
		m_r_px = intRound(0.90 * m_w_px);
	}
	else
	{
		m_l_px = intRound(0.2 * m_w_px);
		m_r_px = intRound(0.95 * m_w_px);
	}

	int W = ::GetSystemMetrics(SM_CXSCREEN);

	if (!m_abs) m_SwitchButton.SetImage(m_SizeSmall ?
		(W == 1920 ? IDR_DOF_REL_3 : W == 1600 ? IDR_DOF_REL_2 : IDR_DOF_REL_1) :
		(W == 1920 ? IDR_DOF_REL_6 : W == 1600 ? IDR_DOF_REL_5 : IDR_DOF_REL_4));
	else       m_SwitchButton.SetImage(m_SizeSmall ?
		(W == 1920 ? IDR_DOF_ABS_3 : W == 1600 ? IDR_DOF_ABS_2 : IDR_DOF_ABS_1) :
		(W == 1920 ? IDR_DOF_ABS_6 : W == 1600 ? IDR_DOF_ABS_5 : IDR_DOF_ABS_4));

	m_SwitchButton.Repaint();

	Repaint();
}
//***************************************************************************************