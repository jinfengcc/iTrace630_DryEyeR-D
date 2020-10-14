//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "ChartDlg.h"
#include "MGdi.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

class CChartDlg : public CDialog
{
public:

	CChartDlg(CWnd* pParentWnd);

	Matrix<real> m_x;
	Matrix<real> m_f;

private:

	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************

BEGIN_MESSAGE_MAP(CChartDlg, CDialog)

	ON_WM_PAINT()

END_MESSAGE_MAP()

//***************************************************************************************

CChartDlg::CChartDlg(CWnd* pParentWnd) : CDialog(IDD_CHART_DLG, pParentWnd)
{
}

//***************************************************************************************

void CChartDlg::OnPaint()
{
	CPaintDC PaintDC(this);

	CMDC DC;
	DC.Attach(PaintDC.Detach());

	RECT Rect;
	GetClientRect(&Rect);

	DC.DrawRectangle(Rect, WHITE, BLACK);

	int fs = 14;
	CMFont Font(fs, 400, "Arial");

	CString s;

	real min;
	m_f.GetMin(min);
	min = floor(min);

	real max;
	m_f.GetMax(max);
	max = ceil(max);

	int nx = m_f.GetSize();
	int ny = (int)(max - min) + 1;
	if (ny <= 1) {
		max = min + 1;
		ny = 2;
	}

	int w = Rect.right - Rect.left;
	int h = Rect.bottom - Rect.top;

	real dw_px = 0.8 * w / (nx - 1.0);
	real dh_px = 0.8 * h / (ny - 1.0);

	int l_px = intRound(0.1 * w);
	int r_px = intRound(0.9 * w);
	int t_px = intRound(0.1 * h);
	int b_px = intRound(0.9 * h);

	int n = m_x.GetSize();
	if (n == 0) n = 11;

	// vertical lines
	real d = (nx - 1.0) / (n - 1.0);
	for (int k = 0; k < n; k++) {
		real i = 0 + k * d;
		int x_px = l_px + intRound(i * dw_px);
		DC.DrawLine(x_px, b_px, x_px, t_px, 1, DARK_GRAY);
		::SetRect(&Rect, x_px - 100, b_px + 3, x_px + 100, b_px + 3 + fs);
		s.Format(_T("%i"), intRound(m_x.GetSize() == 0 ? i : m_x[k]));
		DC.WriteText(s, Rect, Font, WHITE, 1);
	}

	// horizontal lines
	d = (ny - 1.0) / 10.0;
	for (int k = 0; k <= 10; k++) {
		real i = min + k * d;
		int y_px = b_px - intRound((i - min) * dh_px);
		DC.DrawLine(l_px, y_px, r_px, y_px, 1, DARK_GRAY);
		::SetRect(&Rect, 0, y_px - 100, l_px - 5, y_px + 100);
		s.Format(_T("%i"), intRound(i));
		DC.WriteText(s, Rect, Font, WHITE, 2);
	}

	// curve
	int x_px_p, y_px_p;
	for (int i = 0; i < nx; i++) {
		int x_px = l_px + intRound(i * dw_px);
		int y_px = b_px - intRound((m_f[i] - min) * dh_px);
		if (i > 0) DC.DrawLine(x_px_p, y_px_p, x_px, y_px, 2, ORANGE);
		x_px_p = x_px;
		y_px_p = y_px;
	}

	PaintDC.Attach(DC.Detach());
}

//***************************************************************************************

void ShowFunction(const real* x, const real* f, const int n)
{
	CChartDlg* pDlg = new CChartDlg(NULL);
	pDlg->m_x.Create(n, 1, (real*)x);
	pDlg->m_f.Create(n, 1, (real*)f);
	pDlg->DoModal();
	delete pDlg;
}

//***************************************************************************************
