//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "DofChartDlg.h"
#include "CubicSpline.h"
#include "MGdi.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

class CDofChartDlg : public CDialog
{
public:

	CDofChartDlg(RECT rect, BOOL SmallMap, CWnd* pParentWnd);

	CCubicSpline m_Spline;

private:

	real m_F[3001];
	int  m_imax;
	real m_xmax;
	real m_fmax;
	real m_fmin;

	RECT m_rect;//cjf
	BOOL m_SmallMap;//cjf

	static BOOL m_abs;
	static real m_thr;

	int m_pos;

	CMDC m_MemDC;

	int m_w_px;
	int m_h_px;
	int m_l_px;
	int m_r_px;
	int m_t_px;
	int m_b_px;

	int m_mx_px;
	int m_my_px;

	afx_msg void OnPaint();
	virtual BOOL OnInitDialog() override;
	afx_msg void OnMouseMove(uint nFlags, CPoint Point);
	afx_msg void OnLButtonDown(uint nFlags, CPoint Point);
	afx_msg void OnRButtonDown(uint nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
};

BOOL CDofChartDlg::m_abs = FALSE;
real CDofChartDlg::m_thr = 0.5;

//***************************************************************************************

BEGIN_MESSAGE_MAP(CDofChartDlg, CDialog)

	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()

END_MESSAGE_MAP()

//***************************************************************************************

CDofChartDlg::CDofChartDlg(RECT rect, BOOL SmallMap, CWnd* pParentWnd) : CDialog(IDD_DOF_CHART_DLG, pParentWnd)
{
	m_rect = rect;//cjf
	m_SmallMap = SmallMap;//cjf
}

//***************************************************************************************

BOOL CDofChartDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	int W = m_rect.right - m_rect.left;
	int H = m_rect.bottom - m_rect.top;
	SetWindowPos(NULL, m_rect.left, m_rect.top, W, H, SWP_NOZORDER);//cjf

	m_w_px = W;
	m_h_px = H;
	m_l_px = intRound(0.1 * m_w_px);
	m_r_px = intRound(0.9 * m_w_px);
	m_t_px = intRound(0.1 * m_h_px);
	m_b_px = intRound(0.9 * m_h_px);

	m_pos = INVALID_VALUE;

	m_imax = 0;
	m_xmax = 0.0;
	m_fmax = -DBL_MAX;
	for (int i = -1500; i <= 1500; i++) {
		real SphEqDpt = i * 3.0 / 1500;
		m_Spline.GetAt(SphEqDpt, m_F[i + 1500], TRUE);
		if (m_fmax < m_F[i + 1500]) {
			m_imax = i;
			m_xmax = SphEqDpt;
			m_fmax = m_F[i + 1500];
		}
	}
	m_fmin = 0.0;

	m_MemDC.CreateTrueColorDC(m_w_px, m_h_px);

	return FALSE;
}

//***************************************************************************************

void CDofChartDlg::OnPaint()
{
	CPaintDC PaintDC(this);

	RECT Rect;

	::SetRect(&Rect, 0, 0, m_w_px, m_h_px);
	m_MemDC.FillSolidRect(&Rect, BLACK);

	if (m_SmallMap)
	{
		int L = intRound(0.012 * m_w_px);
		int T = intRound(0.012 * m_h_px);

		int fs1 = intRound(0.04*m_h_px);
		int fs2 = intRound(0.06*m_h_px);
		int fs3 = intRound(0.045*m_h_px);

		CMFont Font1(fs1, 400, "Arial");
		CMFont Font2(fs2, 400, "Arial");
		CMFont Font3(fs3, 400, "Arial");
		CString s;

		// background
		m_MemDC.DrawRectangle(m_rect, BLACK, BLACK);

		// caption
		::SetRect(&Rect, L, T, m_w_px, T + fs2);
		m_MemDC.WriteText("Through-focus VSOTFa", Rect, Font3, WHITE, 0);

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
			m_MemDC.DrawLine(m_l_px, y_px, m_r_px, y_px, 1, k == 0 ? WHITE : DARK_GRAY);
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
		real qx = (m_r_px - m_l_px) / (m_Spline.m_x[m_Spline.m_n - 1] - m_Spline.m_x[0]);
		real qy = (m_b_px - m_t_px) / ((m_abs ? 0.8 : m_fmax) - m_fmin);

		{
			int x_px_p, y_px_p;
			for (int x_px = m_l_px; x_px <= m_r_px; x_px++) {
				real x = m_Spline.m_x[0] + (x_px - m_l_px) / qx;
				real f;
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
			s.Format(_T("%.3f"), m_fmax);
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
			m_MemDC.DrawDottedLine(x_px, m_t_px, x_px, m_b_px, WHITE);
			::SetRect(&Rect, x_px - 50, m_t_px - 20, x_px + 50, m_t_px);
			s.Format(_T("%.2f D"), m_xmax);
			m_MemDC.WriteText(s, Rect, Font1, WHITE, 1);
		}

		// threshold

		if (m_thr != INVALID_VALUE) {

			real L = 0.0;
			real R = 0.0;
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
			::SetRect(&Rect, 0, y_px - 30, m_l_px - 50, y_px + 30);
			s.Format(_T("%.1f"), 100.0 * m_thr);
			s += "%";
			m_MemDC.WriteText(s, Rect, Font2, GREEN, 1);

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
				::SetRect(&Rect, (xl_px + xr_px) / 2 - 50, y_px + 10, (xl_px + xr_px) / 2 + 50, y_px + 40);
				s.Format(_T("%.2f D"), R - L);
				m_MemDC.WriteText(s, Rect, Font2, YELLOW, 1);
			}
		}

		//
		if (m_pos != INVALID_VALUE) {

			m_MemDC.DrawDottedLine(m_pos, m_t_px, m_pos, m_b_px, BLUE);

			::SetRect(&Rect, m_pos - 50, m_b_px + 20, m_pos + 50, m_b_px + 50);
			real x = m_Spline.m_x[0] + (m_pos - m_l_px) / qx;
			s.Format(_T("%.2f D"), x);
			m_MemDC.WriteText(s, Rect, Font2, BLUE, 1);
		}

		PaintDC.BitBlt(0, 0, m_w_px, m_h_px, &m_MemDC, 0, 0, SRCCOPY);
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
		CString s;

		m_MemDC.DrawRectangle(m_rect, BLACK, BLACK);

		// caption
		::SetRect(&Rect, 20, 10, m_w_px, 60);
		m_MemDC.WriteText("Through-focus VSOTFa", Rect, Font3, WHITE, 0);

		// vertical lines
		for (int k = 0; k < 25; k++) {
			int x_px = m_l_px + intRound((m_r_px - m_l_px) * k / (25 - 1));
			m_MemDC.DrawLine(x_px, m_t_px, x_px, m_b_px, 1, k == 12 ? WHITE : DARK_GRAY);
			::SetRect(&Rect, x_px - 100, m_b_px + 3, x_px + 100, m_b_px + 3 + fs1);
			s.Format(_T("%.2f"), (k - 12) * 0.25);
			m_MemDC.WriteText(s, Rect, Font0, WHITE, 1);
		}

		// horizontal lines
		const int ny = m_abs ? 9 : 11;
		for (int k = 0; k < ny; k++) {
			int y_px = m_b_px - intRound(k * (m_b_px - m_t_px) / (ny - 1));
			m_MemDC.DrawLine(m_l_px, y_px, m_r_px, y_px, 1, k == 0 ? WHITE : DARK_GRAY);
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
		real qx = (m_r_px - m_l_px) / (m_Spline.m_x[m_Spline.m_n - 1] - m_Spline.m_x[0]);
		real qy = (m_b_px - m_t_px) / ((m_abs ? 0.8 : m_fmax) - m_fmin);

		{
			int x_px_p, y_px_p;
			for (int x_px = m_l_px; x_px <= m_r_px; x_px++) {
				real x = m_Spline.m_x[0] + (x_px - m_l_px) / qx;
				real f;
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
			s.Format(_T("%.3f"), m_fmax);
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
			m_MemDC.DrawDottedLine(x_px, m_t_px, x_px, m_b_px, WHITE);
			::SetRect(&Rect, x_px - 50, m_t_px - 20, x_px + 50, m_t_px);
			s.Format(_T("%.2f D"), m_xmax);
			m_MemDC.WriteText(s, Rect, Font1, WHITE, 1);
		}

		// threshold

		if (m_thr != INVALID_VALUE) {

			real L = 0.0;
			real R = 0.0;
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
			::SetRect(&Rect, 0, y_px - 30, m_l_px - 50, y_px + 30);
			//m_MemDC.FillSolidRect(&Rect,BLACK);
			//::SetRect(&Rect, 20, y_px-30, m_l_px - 50, y_px+30);
			s.Format(_T("%.1f"), 100.0 * m_thr);
			s += "%";
			m_MemDC.WriteText(s, Rect, Font0, GREEN, 1);

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
				::SetRect(&Rect, (xl_px + xr_px) / 2 - 50, y_px + 10, (xl_px + xr_px) / 2 + 50, y_px + 40);
				s.Format(_T("%.2f D"), R - L);
				m_MemDC.WriteText(s, Rect, Font2, YELLOW, 1);
			}
		}

		//
		if (m_pos != INVALID_VALUE) {

			m_MemDC.DrawDottedLine(m_pos, m_t_px, m_pos, m_b_px, BLUE);

			::SetRect(&Rect, m_pos - 50, m_b_px + 20, m_pos + 50, m_b_px + 50);
			real x = m_Spline.m_x[0] + (m_pos - m_l_px) / qx;
			s.Format(_T("%.2f D"), x);
			m_MemDC.WriteText(s, Rect, Font2, BLUE, 1);
		}

		PaintDC.BitBlt(0, 0, m_w_px, m_h_px, &m_MemDC, 0, 0, SRCCOPY);

	}
}

//***************************************************************************************

void CDofChartDlg::OnLButtonDown(uint nFlags, CPoint Point)
{
	OnMouseMove(nFlags, Point);
}

//***************************************************************************************

void CDofChartDlg::OnRButtonDown(uint nFlags, CPoint Point)
{
	m_abs = !m_abs;

	Invalidate(FALSE);
}

//***************************************************************************************

void CDofChartDlg::OnMouseMove(uint nFlags, CPoint Point)
{
	m_mx_px = Point.x;
	m_my_px = Point.y;

	if (nFlags & MK_LBUTTON) {
		m_thr = (real)(m_b_px - m_my_px) / (m_b_px - m_t_px);
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

	Invalidate(FALSE);
}

//***************************************************************************************

void ShowFunction(RECT rect, BOOL SmallMap, Matrix<real> &X, Matrix<real> &F)
{
	CDofChartDlg* pDlg = new CDofChartDlg(rect, SmallMap, NULL);

	pDlg->m_Spline.Create(X, F);

	pDlg->DoModal();

	delete pDlg;
}

//***************************************************************************************
//***************************************************************************************

//void CDofChartDlg::OnPaint()
//{
//  CPaintDC PaintDC(this);
//
//  RECT Rect;
//
//  
//	  int fs1 = 20;
//	  int fs2 = 30;
//	  int fs3 = 40;
//	  CMFont Font1(fs1, 400, "Arial");
//	  CMFont Font2(fs2, 400, "Arial");
//	  CMFont Font3(fs3, 400, "Arial");
//	  CString s;
//
//	  // background
//	  //RECT Rect;
//	  //GetClientRect(&Rect);  
//	  m_MemDC.DrawRectangle(m_rect, BLACK, BLACK);
//
//	  // caption
//	  ::SetRect(&Rect, 20, 10, m_w_px, 60);
//	  m_MemDC.WriteText("Through-focus VSOTFa", Rect, Font3, WHITE, 0);
//
//	  // vertical lines
//	  for (int k = 0; k < 25; k++) {
//		int x_px = m_l_px + intRound((m_r_px - m_l_px) * k / (25 - 1));
//		m_MemDC.DrawLine(x_px, m_t_px, x_px, m_b_px, 1, k == 12 ? WHITE : DARK_GRAY);
//		::SetRect(&Rect, x_px-100, m_b_px+3, x_px+100, m_b_px+3+fs1);
//		s.Format(_T("%.2f"), (k - 12) * 0.25);
//		m_MemDC.WriteText(s, Rect, Font1, WHITE, 1);
//	  }
//
//	  // horizontal lines
//	  const int ny = m_abs ? 9 : 11;
//	  for (int k = 0; k < ny; k++) {
//		int y_px = m_b_px - intRound(k * (m_b_px - m_t_px) / (ny - 1));
//		m_MemDC.DrawLine(m_l_px, y_px, m_r_px, y_px, 1, k == 0 ? WHITE : DARK_GRAY);
//		if (m_abs) {
//		  ::SetRect(&Rect, m_r_px+8, y_px-100, m_w_px, y_px+100);
//		  s.Format(_T("%.1f"), k * 0.1);
//		  m_MemDC.WriteText(s, Rect, Font1, WHITE, 0);
//		}
//		else {
//		  ::SetRect(&Rect, 0, y_px-100, m_l_px-8, y_px+100);
//		  s.Format(_T("%i"), k * 10);
//		  s += "%";
//		  m_MemDC.WriteText(s, Rect, Font1, WHITE, 2);
//		}
//	  }
//
//	  // curve
//	  real qx = (m_r_px - m_l_px) / (m_Spline.m_x[m_Spline.m_n - 1] - m_Spline.m_x[0]);
//	  real qy = (m_b_px - m_t_px) / ((m_abs ? 0.8 : m_fmax) - m_fmin);
//
//	  {
//		int x_px_p, y_px_p;
//		for (int x_px = m_l_px; x_px <= m_r_px; x_px++) {
//		  real x = m_Spline.m_x[0] + (x_px - m_l_px) / qx;
//		  real f;
//		  m_Spline.GetAt(x, f, TRUE);
//		  int y_px = m_b_px - intRound(qy * (f - m_fmin));
//		  if (x_px > m_l_px) m_MemDC.DrawLine(x_px_p, y_px_p, x_px, y_px, 2, RED);
//		  x_px_p = x_px;
//		  y_px_p = y_px;
//		}
//	  }
//
//	  //
//	  {
//		int y_px = m_b_px - intRound(qy * (m_fmax - m_fmin));
//		m_MemDC.DrawDottedLine(m_l_px, y_px, m_r_px, y_px, WHITE);
//		::SetRect(&Rect, 0, y_px-20, m_r_px-8, y_px);
//		s.Format(_T("%.3f"), m_fmax);
//		m_MemDC.WriteText(s, Rect, Font1, WHITE, 2);
//		::SetRect(&Rect, m_l_px+8, y_px-20, m_r_px, y_px);
//		m_MemDC.WriteText("100%", Rect, Font1, WHITE, 0);
//	  }
//
//	  // nodes
//	  for (int k = 0; k < m_Spline.m_n; k++) {
//		int x_px = m_l_px + intRound(qx * (m_Spline.m_x[k] - m_Spline.m_x[0]));
//		int y_px = m_b_px - intRound(qy * (m_Spline.m_f[k] - m_fmin));
//		m_MemDC.DrawCircle(x_px, y_px, 2, 1, WHITE, WHITE);
//	  }
//
//	  // max
//	  {
//		int x_px = m_l_px + intRound(qx * (m_xmax - m_Spline.m_x[0]));
//		m_MemDC.DrawDottedLine(x_px, m_t_px, x_px, m_b_px, WHITE);
//		::SetRect(&Rect, x_px-50, m_t_px-20, x_px+50, m_t_px);
//		s.Format(_T("%.2f D"), m_xmax);
//		m_MemDC.WriteText(s, Rect, Font1, WHITE, 1);
//	  }
//
//	  // threshold
//
//	  if (m_thr != INVALID_VALUE) {
//
//		real L = 0.0;
//		real R = 0.0;
//		BOOL bl = FALSE;
//		BOOL br = FALSE;
//
//		int m = 1500;
//		if (m_fmax != 0.0) {
//		  for (int i = m_imax; i > -m; i--) {
//			if (m_F[m + i - 1]/m_fmax <= m_thr && m_thr <= m_F[m + i]/m_fmax) {
//			  L = i * 3.0 / m;
//			  bl = TRUE;
//			  break;
//			}
//		  }
//		  for (int i = m_imax; i < m; i++) {
//			if (m_F[m + i]/m_fmax >= m_thr && m_thr >= m_F[m + i + 1]/m_fmax) {
//			  R = i * 3.0 / m;
//			  br = TRUE;
//			  break;
//			}
//		  }
//		}
//
//		int y_px = m_b_px - intRound(qy * ((m_thr * m_fmax) - m_fmin));
//
//		m_MemDC.DrawDottedLine(m_l_px, y_px, m_r_px, y_px, GREEN);
//		::SetRect(&Rect, 0, y_px-30, m_l_px-50, y_px+30);
//		s.Format(_T("%.1f"), 100.0 * m_thr);
//		s += "%";
//		m_MemDC.WriteText(s, Rect, Font2, GREEN, 1);
//
//		int xl_px, xr_px;
//		if (bl) {
//		  xl_px = m_l_px + intRound(qx * (L - m_Spline.m_x[0]));
//		  m_MemDC.DrawDottedLine(xl_px, m_t_px, xl_px, m_b_px, YELLOW);
//		}
//		if (br) {
//		  xr_px = m_l_px + intRound(qx * (R - m_Spline.m_x[0]));
//		  m_MemDC.DrawDottedLine(xr_px, m_t_px, xr_px, m_b_px, YELLOW);
//		}
//		if (bl && br) {
//		  ::SetRect(&Rect, (xl_px + xr_px)/2-50, y_px+10, (xl_px+xr_px)/2+50, y_px+40);
//		  s.Format(_T("%.2f D"), R - L);
//		  m_MemDC.WriteText(s, Rect, Font2, YELLOW, 1);
//		}
//	  }
//
//	  //
//	  if (m_pos != INVALID_VALUE) {
//
//		m_MemDC.DrawDottedLine(m_pos, m_t_px, m_pos, m_b_px, BLUE);
//
//		::SetRect(&Rect, m_pos-50, m_b_px+20, m_pos+50, m_b_px+50);
//		real x = m_Spline.m_x[0] + (m_pos - m_l_px) / qx;
//		s.Format(_T("%.2f D"), x);
//		m_MemDC.WriteText(s, Rect, Font2, BLUE, 1);
//	  }
//
//	  PaintDC.BitBlt(0, 0, m_w_px, m_h_px, &m_MemDC, 0, 0, SRCCOPY);  
//}
