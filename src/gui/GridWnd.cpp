//***************************************************************************************

#include "StdAfx.h"
#include "GridWnd.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

CGridWnd::CGridWnd()
{
	m_PointsColor = GREEN;
	m_CurPoint = -1;
}

//***************************************************************************************

void CGridWnd::RepaintMemDC()
{
	ClearMemDC();

	const real_t w_um = 8500.0;
	const real_t h_um = 8500.0;
	real_t x_px_um = m_w / w_um;
	real_t y_px_um = m_h / h_um;
	real_t cx = m_w * 0.5;
	real_t cy = m_h * 0.5;

	// Grid
	for (int i = -4; i <= 4; i++) {
		real_t um = i * 1000.0;
		int x = intRound(cx + um * x_px_um);
		int y = intRound(cy - um * y_px_um);
		m_MemDC.DrawHorzDottedLine(0, m_w, y, 0x00606060, NOCOLOR);
		m_MemDC.DrawVertDottedLine(x, 0, m_h, 0x00606060, NOCOLOR);
	}

	int n = m_XUm.GetSize();

	// Points
	for (int i = 0; i < n; i++) {
		int x = intRound(cx + m_XUm[i] * x_px_um);
		int y = intRound(cy - m_YUm[i] * y_px_um);
		m_MemDC.DrawCircle(x, y, 2, 1, m_PointsColor, m_PointsColor);
	}

	// Current point
	if (m_CurPoint >= 0 && m_CurPoint < n) {
		int x = intRound(cx + m_XUm[m_CurPoint] * x_px_um);
		int y = intRound(cy - m_YUm[m_CurPoint] * y_px_um);
		m_MemDC.DrawCircle(x, y, 2, 1, RED, RED);

		RECT Rect = { 5, 2, m_w, 17 };
		CString s; s.Format(_T("%i"), m_CurPoint + 1);
		CMFont Font(14, 400, "Arial");
		m_MemDC.WriteText(s, Rect, Font, WHITE, 0);
	}
}

//***************************************************************************************

void CGridWnd::CreateScanPattern(real_t RMax)
{
	m_XUm.Create(1 + 6 + 12 + 18 + 24 + 30 + 37);
	m_YUm.Create(1 + 6 + 12 + 18 + 24 + 30 + 37);
	int n[7] = { 1, 6, 12, 18, 24, 30, 37 };
	real_t dR = RMax / 6.0;
	int i = 0;
	for (int c = 0; c < 7; c++) {
		real_t R = c * dR;
		real_t dA = _2_Pi / n[c];
		for (int p = 0; p < n[c]; p++) {
			real_t A = p * dA;
			m_XUm[i] = R * cos(A);
			m_YUm[i] = R * sin(A);
			i++;
		}
	}
}

//***************************************************************************************

void CGridWnd::CreateProbePattern(real_t RMax)
{
	m_XUm.Create(1 + 6 + 10);
	m_YUm.Create(1 + 6 + 10);
	int n[3] = { 1, 6, 10 };
	real_t dR = RMax / 2.0;
	int i = 0;
	for (int c = 0; c < 3; c++) {
		real_t R = c * dR;
		real_t dA = _2_Pi / n[c];
		for (int p = 0; p < n[c]; p++) {
			real_t A = p * dA;
			m_XUm[i] = R * cos(A);
			m_YUm[i] = R * sin(A);
			i++;
		}
	}
}

//***************************************************************************************

void CGridWnd::CreateCirclePattern(real_t RMax)
{
	m_XUm.Create(1 + 40);
	m_YUm.Create(1 + 40);
	m_XUm[0] = 0.0;
	m_YUm[0] = 0.0;
	int n = 40;
	real_t dA = _2_Pi / n;
	for (int p = 0; p < n; p++) {
		real_t A = p * dA;
		m_XUm[1 + p] = RMax * cos(A);
		m_YUm[1 + p] = RMax * sin(A);
	}
}

//***************************************************************************************

void CGridWnd::CreateCrossPattern(real_t RMax)
{
	int n = 6;
	m_XUm.Create((n + 1 + n) + (n + 1 + n));
	m_YUm.Create((n + 1 + n) + (n + 1 + n));
	real_t D = RMax / n;
	int i = 0;
	for (int k = -n; k <= n; k++) {
		m_XUm[i] = k * D;
		m_YUm[i] = 0.0;
		i++;
	}
	for (int k = -n; k <= n; k++) {
		m_XUm[i] = 0.0;
		m_YUm[i] = k * D;
		i++;
	}
}

//***************************************************************************************

void CGridWnd::CreateLetterPattern(real_t RMax)
{
	int n = 6;

	int np = 0;
	for (int y = n; y >= -n; y--) {
		for (int x = -n; x <= n; x++) {
			if (abs(y) == n || abs(x) == n || abs(x) == abs(y)) {
				np++;
			}
		}
	}
	m_XUm.Create(np);
	m_YUm.Create(np);

	real_t D = RMax / n;
	int i = 0;
	real_t Y = RMax;
	for (int y = n; y >= -n; y--) {
		real_t X = -RMax;
		for (int x = -n; x <= n; x++) {
			if (abs(y) == n || abs(x) == n || abs(x) == abs(y)) {
				m_XUm[i] = X;
				m_YUm[i] = Y;
				i++;
			}
			X += D;
		}
		Y -= D;
	}
}

//***************************************************************************************

void CGridWnd::CreateSquarePattern(real_t RMax)
{
	int n = 6;
	m_XUm.Create((n + 1 + n) * (n + 1 + n));
	m_YUm.Create((n + 1 + n) * (n + 1 + n));
	real_t D = RMax / n;
	int i = 0;
	real_t Y = RMax;
	for (int y = n; y >= -n; y--) {
		real_t X = -RMax;
		for (int x = -n; x <= n; x++) {
			m_XUm[i] = X;
			m_YUm[i] = Y;
			i++;
			X += D;
		}
		Y -= D;
	}
}

//***************************************************************************************

void CGridWnd::CreateVLinePattern(real_t RMax)
{
	int n = 6;
	m_XUm.Create(n + 1 + n);
	m_YUm.Create(n + 1 + n);
	real_t D = RMax / n;
	int i = 0;
	for (int k = -n; k <= n; k++) {
		m_XUm[i] = 0.0;
		m_YUm[i] = k * D;
		i++;
	}
}

//***************************************************************************************

void CGridWnd::CreateHLinePattern(real_t RMax)
{
	int n = 6;
	m_XUm.Create(n + 1 + n);
	m_YUm.Create(n + 1 + n);
	real_t D = RMax / n;
	int i = 0;
	for (int k = -n; k <= n; k++) {
		m_XUm[i] = k * D;
		m_YUm[i] = 0.0;
		i++;
	}
}

//***************************************************************************************

void CGridWnd::CreateLTRBLinePattern(real_t RMax)
{
	int n = 6;
	m_XUm.Create(n + 1 + n);
	m_YUm.Create(n + 1 + n);
	real_t D = RMax / n;
	int i = 0;
	for (int k = -n; k <= n; k++) {
		m_XUm[i] = k * D;
		m_YUm[i] = -k * D;
		i++;
	}
}

//***************************************************************************************

void CGridWnd::CreateLBRTLinePattern(real_t RMax)
{
	int n = 6;
	m_XUm.Create(n + 1 + n);
	m_YUm.Create(n + 1 + n);
	real_t D = RMax / n;
	int i = 0;
	for (int k = -n; k <= n; k++) {
		m_XUm[i] = k * D;
		m_YUm[i] = k * D;
		i++;
	}
}

//***************************************************************************************
