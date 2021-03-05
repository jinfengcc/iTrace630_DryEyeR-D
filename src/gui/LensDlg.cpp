//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "LensDlg.h"
#include "Data.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CLensDlg, CDialog)

	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DOT1_RADIO, OnDotRadioClicked)
	ON_BN_CLICKED(IDC_DOT2_RADIO, OnDotRadioClicked)
	ON_BN_CLICKED(IDC_DOT3_RADIO, OnDotRadioClicked)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()

END_MESSAGE_MAP()

//***************************************************************************************

CLensDlg::CLensDlg(CWnd* pParentWnd, CEyeImage* pImage) : CDialog(IDD_LENS_DLG, pParentWnd)
{
	m_pImage = pImage;

	for (int i = 0; i < 3; i++) {
		m_le_x_um[i] = m_pImage->m_le_x_um[i];
		m_le_y_um[i] = m_pImage->m_le_y_um[i];
	}

	m_d = -1;
}

//***************************************************************************************

void CLensDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDOK, m_OKButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_DOT1_RADIO, m_Dot1Radio);
	DDX_Control(pDX, IDC_DOT2_RADIO, m_Dot2Radio);
	DDX_Control(pDX, IDC_DOT3_RADIO, m_Dot3Radio);
}

//***************************************************************************************

BOOL CLensDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_w = m_pImage->m_w;
	m_h = m_pImage->m_h;
	m_w_um = m_pImage->m_w_um;
	m_h_um = m_pImage->m_h_um;
	m_x_px_um = m_w / m_w_um;
	m_y_px_um = m_h / m_h_um;
	m_cx = 0.5 * m_w;
	m_cy = 0.5 * m_h - 1;

	RECT Rect;
	GetWindowRect(&Rect);
	int ww = Rect.right - Rect.left;
	int wh = Rect.bottom - Rect.top;
	GetClientRect(&Rect);
	int cw = Rect.right - Rect.left;
	int ch = Rect.bottom - Rect.top;
	int tw = ww - cw;
	int th = wh - ch;

	int W = ::GetSystemMetrics(SM_CXSCREEN);
	int H = ::GetSystemMetrics(SM_CYSCREEN);

	int d = intRound(0.005 * W);

	m_l = m_t = d;

	int iw = __max(640, m_w);
	int ih = __max(480, m_h);

	int bw = intRound(0.065 * W);
	int bh = intRound(0.033 * H);

	ww = tw + d + iw + d + bw + d;
	wh = th + d + ih + d + bh + d;

	m_MemDC.CreateTrueColorDC(m_w, m_h);

	m_Rgn.CreateRectRgn(d, d, d + m_w, d + m_h);

	SetWindowPos(NULL, 0, 0, ww, wh, SWP_NOZORDER);
	CenterWindow();

	int t = intRound(0.005 * W);
	int dt1 = intRound(0.011 * W);
	int dt2 = intRound(0.018 * W);

	m_Dot1Radio.SetWindowPos(NULL, iw + d + d, t, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_Dot2Radio.SetWindowPos(NULL, iw + d + d, t += dt2, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_Dot3Radio.SetWindowPos(NULL, iw + d + d, t += dt2, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	GetClientRect(&Rect);
	cw = Rect.right - Rect.left;
	ch = Rect.bottom - Rect.top;

	int bl = intRound(0.5 * cw - 0.5 * d) - bw;
	int bt = ch - d - bh;
	m_CancelButton.SetWindowPos(NULL, bl, bt, bw, bh, SWP_NOZORDER);
	bl = intRound(0.5 * cw + 0.5 * d);
	m_OKButton.SetWindowPos(NULL, bl, bt, bw, bh, SWP_NOZORDER);

	DrawDots();

	BOOL IsDBReadOnly = ::DB.IsReadOnly();
	m_OKButton.EnableWindow(!IsDBReadOnly);

	return FALSE;
}

//***************************************************************************************

void CLensDlg::DrawDots()
{
	memcpy(m_MemDC.m_RGBData, m_pImage->m_RGBData.GetMem(), m_pImage->m_RGBData.GetSize());

	int x[3], y[3];
	for (int i = 0; i < 3; i++) {
		x[i] = intRound(m_cx + m_le_x_um[i] * m_x_px_um);
		y[i] = intRound(m_cy - m_le_y_um[i] * m_y_px_um);
	}
	int x0 = intRound(m_cx + 0.5 * (m_le_x_um[0] + m_le_x_um[2]) * m_x_px_um);
	int y0 = intRound(m_cy - 0.5 * (m_le_y_um[0] + m_le_y_um[2]) * m_y_px_um);

	for (int i = 0; i < 3; i++) {
		COLORREF Color = i == m_d ? RED : YELLOW;
		m_MemDC.DrawLine(x[i], y[i], x0, y0, 1, Color);
		m_MemDC.DrawCircle(x[i], y[i], 4, 1, Color, Color);
	}
}

//***************************************************************************************

void CLensDlg::OnDotRadioClicked()
{
	if (m_Dot1Radio.GetCheck()) m_d = 0;
	else if (m_Dot2Radio.GetCheck()) m_d = 1;
	else if (m_Dot3Radio.GetCheck()) m_d = 2;

	DrawDots();

	InvalidateRgn(&m_Rgn, FALSE);
}

//***************************************************************************************

void CLensDlg::OnOK()
{
	CDialog::OnOK();

	for (int i = 0; i < 3; i++) {
		m_pImage->m_le_x_um[i] = m_le_x_um[i];
		m_pImage->m_le_y_um[i] = m_le_y_um[i];
	}

	m_pImage->m_le_ok = TRUE;
}

//***************************************************************************************

void CLensDlg::OnPaint()
{
	CPaintDC PaintDC(this);
	PaintDC.BitBlt(m_l, m_t, m_w, m_h, &m_MemDC, 0, 0, SRCCOPY);
}

//***************************************************************************************

void CLensDlg::OnLButtonDown(uint nFlags, CPoint Point)
{
	OnMouseMove(nFlags, Point);
}

//***************************************************************************************

void CLensDlg::OnMouseMove(uint nFlags, CPoint Point)
{
	if (!(nFlags & MK_LBUTTON)) return;
	if (m_d == -1) return;

	if (Point.x < m_l || Point.x >= m_l + m_w || Point.y < m_t || Point.y >= m_t + m_h) return;

	real_t x = (Point.x - m_l) - m_cx;
	real_t y = m_cy - (Point.y - m_t);

	m_le_x_um[m_d] = x / m_x_px_um;
	m_le_y_um[m_d] = y / m_y_px_um;

	DrawDots();

	InvalidateRgn(&m_Rgn, FALSE);
}

//***************************************************************************************
