//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "MTFWnd.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CMTFWnd, CDispWnd)

	ON_MESSAGE(WM_SLIDER_POS_CHANGED, OnSliderChangedPos)

END_MESSAGE_MAP()

//***************************************************************************************

CMTFWnd::CMTFWnd(RECT& Rect, CWnd* pWnd) : CDispWnd(Rect, pWnd)
{
}

//***************************************************************************************

void CMTFWnd::CreateSlider()
{
	RECT Rect = { intRound(0.100 * m_w), intRound(0.950 * m_h), intRound(0.767 * m_w), intRound(0.990 * m_h) };
	m_AngleSlider.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, Rect, this, IDC_MTF_ANGLE_SLIDER);
	m_AngleSlider.m_Pos = (real)m_Angle / 179.0;
	CMRgn Rgn;
	Rgn.SetRectRgn(&Rect);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);
}

//***************************************************************************************

LRESULT CMTFWnd::OnSliderChangedPos(WPARAM wParam, LPARAM lParam)
{
	m_Angle = intRound(179.0 * m_AngleSlider.m_Pos);

	Repaint();

	return 0;
}

//***************************************************************************************

void CMTFWnd::RepaintMemDC()
{
	ClearMemDC();

	if (m_Method3D)
	{
		Mtf3D();
	}
	else
	{
		if (m_ShowDys) Mtf2DDys();
		else          Mtf2D();
	}

	Labels();
	Border();
}

//***************************************************************************************

void CMTFWnd::Mtf2D()
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	real L = 0.100 * m_w;
	real T = 0.180 * m_h;
	real R = 0.900 * m_w;
	real B = 0.860 * m_h;
	real w_px = R - L;
	real h_px = B - T;
	int g_cd = 5;
	int w_cd = 30;
	real px_cd = w_px / w_cd;

	CString s;
	RECT Rect;

	int FontSize = intRound(0.045 * m_h);
	CMFont Font(FontSize, 400, "Arial");


	// angle
	if (!m_Average)
	{
		s.Format(_T("%i°"), m_Angle);
		real b = 0.2 * m_h;
		::SetRect(&Rect, intRound(0.012 * m_w), intRound(0.950 * m_h), m_w, intRound(0.990 * m_h));
		m_MemDC.WriteText(s, Rect, Font, white, 0);
	}

	for (int i_cd = 0; i_cd <= w_cd; i_cd += g_cd)
	{
		real x = L + px_cd * i_cd;
		m_MemDC.DrawLine(intRound(x), intRound(T), intRound(x), intRound(B + 1), 1, gray);

		s.Format(_T("%i"), i_cd);
		::SetRect(&Rect, intRound(x - 1000.0), intRound(B + 3.0), intRound(x + 1000.0), intRound(B + 3.0 + FontSize));
		m_MemDC.WriteText(s, Rect, Font, white, 1);
	}

	for (int i = 0; i <= 10; i++)
	{
		real y = B - 0.1 * i * h_px;
		m_MemDC.DrawLine(intRound(L), intRound(y), intRound(R + 1), intRound(y), 1, gray);

		s.Format(_T("%i"), i * 10);
		s += "% ";

		::SetRect(&Rect, intRound(L - 1000), intRound(y - 100.0), intRound(L), intRound(y + 100.0));
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	Matrix<real> x(w_cd + 1);
	Matrix<real> y(w_cd + 1);
	Matrix<real> f(w_cd + 1); f.Fill(INVALID_VALUE);
	real avg = 0.0;
	int k = 0;

	for (int i_cd = 0; i_cd <= w_cd; i_cd++)
	{
		x[i_cd] = L + i_cd * px_cd;
		if (m_Average)
		{
			if (!m_Mtf.GetAtCdR(i_cd, f[i_cd])) break;
		}
		else
		{
			if (!m_Mtf.GetAtCdRA(i_cd, m_Angle, f[i_cd])) break;
		}
		y[i_cd] = B - f[i_cd] * h_px;

		avg += f[i_cd];
		k++;
	}

	if (k > 0) avg /= k;

	for (int i_cd = 1; i_cd <= w_cd; i_cd++)
	{
		if (f[i_cd] == INVALID_VALUE) break;

		m_MemDC.DrawLine(intRound(x[i_cd - 1]), intRound(y[i_cd - 1]), intRound(x[i_cd]), intRound(y[i_cd]), 3, m_Printing ? ORANGE : YELLOW);

		if (i_cd % 5 == 0)
		{
			m_MemDC.DrawCircle(intRound(x[i_cd]), intRound(y[i_cd]), 3, 1, white, white);
			s.Format(_T("%.3f"), f[i_cd]);
			::SetRect(&Rect, intRound(x[i_cd] + 5), intRound(y[i_cd] - FontSize), intRound(x[i_cd] + 1000.0), intRound(y[i_cd]));
			m_MemDC.WriteText(s, Rect, Font, white, 0);
		}
	}

	//6.2.0 Indicate point on all MTF graphs at 10 cycles per degree & 40%.  
	int x0 = L;
	int x1 = L + px_cd * 10;
	int y0 = B;
	int y1 = B - 0.4 * h_px;

	m_MemDC.DrawLine(x0, y1, x1, y1, 1, RED);
	m_MemDC.DrawLine(x1, y0, x1, y1, 1, RED);

	for (int i = x0; i <= x1; i++)
	{
		for (int j = y1; j <= y0; j++)
		{
			COLORREF C = m_MemDC.GetPixel(i, j);
			int bR = GetRValue(C);
			int bG = GetGValue(C);
			int bB = GetBValue(C);
			int Brightness = intRound(0.3*bR + 0.59*bG + 0.11*bB);

			int R = 250;

			if (Brightness > 200)
			{
				R = 220;
			}

			int A = 60;
			int bA = 195;
			R = (A * R + bA * bR) / 255;


			m_MemDC.SetPixelV(i, j, RGB(R, bG, bB));
		}
	}

	//6.2.0 Indicate point on all MTF graphs at 10 cycles per degree & 40%.  Done

	// average height
	m_RTLabel[0].Format(_T("Avg Height = %.3f"), avg);

	// units
	::SetRect(&Rect, 0, intRound(B + 3.0 + FontSize), m_w, intRound(B + 3.0 + 2.0 * FontSize));
	m_MemDC.WriteText("cycles / degree", Rect, Font, white, 1);
}

//*************************************************************************************

//Draw Dys MTF Line
void CMTFWnd::Mtf2DDys()
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	real L = 0.150 * m_w;//From 0.10 to 0.15
	real T = 0.180 * m_h;
	real R = 0.900 * m_w;
	real B = 0.860 * m_h;
	real w_px = R - L;
	real h_px = B - T;
	int g_cd = 5;
	int w_cd = 30;
	real px_cd = w_px / w_cd;

	CString s;
	RECT Rect;

	int FontSize = intRound(0.045 * m_h);
	CMFont Font(FontSize, 400, "Arial");

	// angle
	if (!m_Average)
	{
		s.Format(_T("%i°"), m_Angle);
		real b = 0.2 * m_h;
		::SetRect(&Rect, intRound(0.012 * m_w), intRound(0.950 * m_h), m_w, intRound(0.990 * m_h));
		m_MemDC.WriteText(s, Rect, Font, white, 0);
	}

	for (int i_cd = 0; i_cd <= w_cd; i_cd += g_cd)
	{
		real x = L + px_cd * i_cd;
		m_MemDC.DrawLine(intRound(x), intRound(T), intRound(x), intRound(B + 1), 1, gray);

		s.Format(_T("%i"), i_cd);
		::SetRect(&Rect, intRound(x - 1000.0), intRound(B + 3.0), intRound(x + 1000.0), intRound(B + 3.0 + FontSize));
		m_MemDC.WriteText(s, Rect, Font, white, 1);
	}

	for (int i = 0; i <= 10; i++)
	{
		real y = B - 0.1 * i * h_px;
		m_MemDC.DrawLine(intRound(L), intRound(y), intRound(R + 1), intRound(y), 1, gray);

		s.Format(_T("%i"), i * 10);
		s += "% ";
		::SetRect(&Rect, intRound(L - 1000), intRound(y - 100.0), intRound(L), intRound(y + 100.0));
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	Matrix<real> x(w_cd + 1);
	Matrix<real> y(w_cd + 1);
	Matrix<real> f(w_cd + 1); f.Fill(INVALID_VALUE);
	real avg = 0.0;
	int k = 0;

	for (int i_cd = 0; i_cd <= w_cd; i_cd++)
	{
		x[i_cd] = L + i_cd * px_cd;

		if (m_Average)
		{
			if (!m_Mtf.GetAtCdR(i_cd, f[i_cd])) break;
		}
		else
		{
			if (!m_Mtf.GetAtCdRA(i_cd, m_Angle, f[i_cd])) break;
		}

		y[i_cd] = B - f[i_cd] * h_px;

		avg += f[i_cd];
		k++;
	}

	if (k > 0) avg /= k;

	for (int i_cd = 1; i_cd <= w_cd; i_cd++)
	{
		if (f[i_cd] == INVALID_VALUE) break;

		real a = f[i_cd];

		m_MemDC.DrawLine(intRound(x[i_cd - 1]), intRound(y[i_cd - 1]), intRound(x[i_cd]), intRound(y[i_cd]), 3, m_Printing ? ORANGE : YELLOW);

		if (i_cd % 5 == 0)
		{
			m_MemDC.DrawCircle(intRound(x[i_cd]), intRound(y[i_cd]), 3, 1, white, white);
			s.Format(_T("%.3f"), f[i_cd]);
			::SetRect(&Rect, intRound(x[i_cd] + 5), intRound(y[i_cd] - FontSize), intRound(x[i_cd] + 1000.0), intRound(y[i_cd]));
			m_MemDC.WriteText(s, Rect, Font, white, 0);
		}
	}

	// average height
	m_RTLabel[0].Format(_T("Avg Height = %.3f"), avg);

	// units
	::SetRect(&Rect, 0, intRound(B + 3.0 + FontSize), m_w, intRound(B + 3.0 + 2.0 * FontSize));
	m_MemDC.WriteText("cycles / degree", Rect, Font, white, 1);



	//MTF Line
	for (int i_cd = 1; i_cd <= w_cd; i_cd++)
	{
		if (f[i_cd] == INVALID_VALUE) break;

		m_MemDC.DrawLine(intRound(x[i_cd - 1]), intRound(y[i_cd - 1]), intRound(x[i_cd]), intRound(y[i_cd]), 3, m_Printing ? ORANGE : YELLOW);

		if (i_cd % 5 == 0)
		{
			m_MemDC.DrawCircle(intRound(x[i_cd]), intRound(y[i_cd]), 3, 1, white, white);
			s.Format(_T("%.3f"), f[i_cd]);
			::SetRect(&Rect, intRound(x[i_cd] + 5), intRound(y[i_cd] - FontSize), intRound(x[i_cd] + 1000.0), intRound(y[i_cd]));
			m_MemDC.WriteText(s, Rect, Font, white, 0);
		}
	}
	//MTF Line

	//
	int xs = intRound(m_h / 2.0) + 10;
	s = "contrast";  m_MemDC.WriteRotatedText(s, 10, xs, 90, FontSize, 400, "Arial", white, NOCOLOR);
	//

	//real Right = L + intRound(real(R - L) * 1 / 3);
	//int sw     = intRound((R - Right)/10.0);
	//int sT     = intRound(T) + intRound(real(B-T)/10.0);

	//int SL    = intRound(Right);
	//int Left  = intRound(Right);

	//::SetRect(&Rect , Left, intRound(T) - 2 - FontSize, intRound(R), intRound(T) - 2);
	//			
	//if(m_LTLabel[0].Find(_T("INTERNAL",0) != -1) s = "Dysfunctional Lens Index";
	//else if(m_LTLabel[0].Find(_T("CORNEA",0) != -1) s = "Cornea Index";
	//else if(m_LTLabel[0].Find(_T("TOTAL EYE",0) != -1) s = "TOTAL Eye Index";
	//m_MemDC.WriteText(s, Rect,Font, white, 0);

	//for(int i = 1; i <= 10; i++)
	//{
	//   Right = Left + sw;
	//if(i == 10) Right = R;
	//	 
	//if(i == 1) 
	//{
	// ::SetRect(&Rect , Left - FontSize, sT + 2, Left + FontSize, sT + 2 + FontSize);
	// m_MemDC.WriteText("0", Rect,Font, CLColor[i - 1], 1);
	//}

	//::SetRect(&Rect , Left, intRound(T), intRound(Right), sT);
	//m_MemDC.DrawRectangle(Rect,CLColor[i-1], CLColor[i - 1]);

	//Left = intRound(Right);

	//if(i == 5 || i == 10) 
	//{
	// s.Format(_T("%i"), i);

	// ::SetRect(&Rect , Left - FontSize, sT + 2, Left + FontSize, sT + 2 + FontSize);
	// m_MemDC.WriteText(s, Rect,Font, CLColor[i - 1], 1);
	//}
	//}

	//Arrow and value
	//val    = 10 - val;

	//int x0 = intRound(SL + val*(real)sw);

	//int cn = intRound(val);//color index

	//int x1 = x0 - 6;
	//int x2 = x0 + 6;


	//int y0 =  sT + 2 + FontSize + 2;
	//int y1 =  y0 + 8;
	//int y2 =  y1;
	//m_MemDC.DrawTriangle(x0, y0, x1, y1, x2, y2, 1, CLColor[cn], CLColor[cn]);
	//
	//x1 = x0 - 2;
	//x2 = x0 + 2;

	//y2 = y0 + 25;

	//::SetRect(&Rect ,x1, y1, x2, y2);
	//m_MemDC.DrawRectangle(Rect, CLColor[cn], CLColor[cn]);
	////Arrow Done

	////Text
	//x1 = x0 - 3*FontSize;
	//x2 = x0 + 3*FontSize;

	//y1 = y2 + 3;
	//y2 = y1 + FontSize;
	//::SetRect(&Rect ,x1, y1, x2, y2);
	//s.Format(_T("%.2f"), val);
	//m_MemDC.WriteText(s, Rect,Font, CLColor[cn], 1);
	////Text Done

	//m_MemDC.DrawCircle(intRound(valNumX[0]), intRound(valNumY[0]), 4, 1, GREEN, GREEN);		
	//m_MemDC.DrawCircle(intRound(valNumX[2]), intRound(valNumY[2]), 4, 1, ORANGE, ORANGE);
	//m_MemDC.DrawCircle(intRound(valNumX[3]), intRound(valNumY[3]), 4, 1, RED, RED);	
	//m_MemDC.DrawCircle(intRound(valNumX[4]), intRound(valNumY[4]), 4, 1, CYAN, CYAN);	
	//m_MemDC.DrawCircle(intRound(valNumX[5]), intRound(valNumY[5]), 4, 1, YELLOW, YELLOW);

	//531 done 
}

//***************************************************************************************
//***************************************************************************************

void CMTFWnd::Re_Mtf2D()
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	real  valNumX[6];//531
	real  valNumY[6];//531

	real L = 0.100 * m_w;
	real T = 0.180 * m_h;
	real R = 0.900 * m_w;
	real B = 0.860 * m_h;
	real w_px = R - L;
	real h_px = B - T;
	int g_cd = 5;
	int w_cd = 30;
	real px_cd = w_px / w_cd;

	CString s;
	RECT Rect;

	int FontSize = intRound(0.045 * m_h);
	CMFont Font(FontSize, 400, "Arial");

	//531
	valNumX[0] = L;
	valNumY[0] = B;

	valNumX[3] = L;
	valNumY[3] = T + (B - T) * 3 / 5;
	//531

	// angle
	if (!m_Average) {
		s.Format(_T("%i°"), m_Angle);
		real b = 0.2 * m_h;
		::SetRect(&Rect, intRound(0.012 * m_w), intRound(0.950 * m_h), m_w, intRound(0.990 * m_h));
		m_MemDC.WriteText(s, Rect, Font, white, 0);
	}

	// вертикальные линии
	for (int i_cd = 0; i_cd <= w_cd; i_cd += g_cd) {
		real x = L + px_cd * i_cd;
		m_MemDC.DrawLine(intRound(x), intRound(T), intRound(x), intRound(B + 1), 1, gray);

		s.Format(_T("%i"), i_cd);
		::SetRect(&Rect, intRound(x - 1000.0), intRound(B + 3.0), intRound(x + 1000.0), intRound(B + 3.0 + FontSize));
		m_MemDC.WriteText(s, Rect, Font, white, 1);
	}

	// горизонтальные линии
	for (int i = 0; i <= 10; i++) {
		real y = B - 0.1 * i * h_px;
		m_MemDC.DrawLine(intRound(L), intRound(y), intRound(R + 1), intRound(y), 1, gray);

		s.Format(_T("%i"), i * 10);//610
		s += "% ";//610
		::SetRect(&Rect, intRound(L - 1000), intRound(y - 100.0), intRound(L), intRound(y + 100.0));
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	Matrix<real> x(w_cd + 1);
	Matrix<real> y(w_cd + 1);
	Matrix<real> f(w_cd + 1); f.Fill(INVALID_VALUE);
	real avg = 0.0;
	int k = 0;

	for (int i_cd = 0; i_cd <= w_cd; i_cd++) {
		x[i_cd] = L + i_cd * px_cd;
		if (m_Average) {
			if (!m_ReMtf.GetAtCdR(i_cd, f[i_cd])) break;
		}
		else {
			if (!m_ReMtf.GetAtCdRA(i_cd, m_Angle, f[i_cd])) break;
		}
		y[i_cd] = B - f[i_cd] * h_px;

		avg += f[i_cd];
		k++;
	}
	if (k > 0) avg /= k;
	for (int i_cd = 1; i_cd <= w_cd; i_cd++) {
		if (f[i_cd] == INVALID_VALUE) break;
		// участок кривой
		m_MemDC.DrawLine(intRound(x[i_cd - 1]), intRound(y[i_cd - 1]), intRound(x[i_cd]), intRound(y[i_cd]), 1, GREEN);
	}

	//531
	valNumX[1] = x[5];
	valNumY[1] = y[5];

	valNumX[4] = L + (R - L) / 6;
	valNumY[4] = valNumY[3];
	valNumX[5] = L;

	real fa;
	real minpos = 1000;//531
	for (real i = 0; i <= 30; i = i + 0.1) {
		real xa = L + i * px_cd;
		if (m_Average) {
			if (!m_ReMtf.GetAtCdR(i, fa)) break;
		}
		else {
			if (!m_ReMtf.GetAtCdRA(i, m_Angle, fa)) break;
		}
		real ya = B - fa * h_px;

		real disPos = fabs(ya - valNumY[3]);
		if (disPos < minpos)
		{
			valNumX[5] = xa;
			minpos = disPos;
		}
	}
	valNumY[5] = valNumY[3];
	//531 Done

	// average height
	m_RTLabel[0].Format(_T("Avg Height = %.3f"), avg);

	// units
	::SetRect(&Rect, 0, intRound(B + 3.0 + FontSize), m_w, intRound(B + 3.0 + 2.0 * FontSize));
	m_MemDC.WriteText("cycles / degree", Rect, Font, white, 1);

	//531
	real tx = L + px_cd * 10;
	m_MemDC.DrawLine(intRound(tx), intRound(T + h_px*0.6), intRound(tx), intRound(B + 1), 1, RED);

	real ty = B - 0.1 * 4 * h_px;
	m_MemDC.DrawLine(intRound(L), intRound(ty), intRound(L + w_px / 3.0), intRound(ty), 1, RED);

	valNumX[2] = tx;
	valNumY[2] = ty;

	for (int y = intRound(ty + 1); y < B; y++)
	{
		for (int x = intRound(L); x < intRound(tx); x++)
		{
			COLORREF C = m_MemDC.GetPixel(x, y);
			int bR = GetRValue(C);
			int bG = GetGValue(C);
			int bB = GetBValue(C);

			int R = 250;
			int G = 20;
			int B = 20;

			int A = 38;
			int bA = 217;
			R = (A * R + bA * bR) / 255;
			G = (A * G + bA * bG) / 255;
			B = (A * B + bA * bB) / 255;

			m_MemDC.SetPixelV(x, y, RGB(R, G, B));
		}
	}

	//The DysFunctional scale

	//calculate the value
	real val = 0;
	if (valNumX[5] < valNumX[2])
	{
		real Dis = sqrt((valNumX[3] - valNumX[4])*(valNumX[3] - valNumX[4]) + (valNumY[3] - valNumY[4])*(valNumY[3] - valNumY[4]));
		real Dis0 = sqrt((valNumX[5] - valNumX[3])*(valNumX[5] - valNumX[3]) + (valNumY[5] - valNumY[3])*(valNumY[5] - valNumY[3]));

		val = 10.0 - ((Dis0*2.50) / Dis);
	}
	else
	{
		if (valNumX[5] < (L + 5 * (R - L) / 6))
		{
			real Dis = sqrt((valNumX[3] - valNumX[4])*(valNumX[3] - valNumX[4]) + (valNumY[3] - valNumY[4])*(valNumY[3] - valNumY[4]));
			real Dis0 = sqrt((valNumX[5] - valNumX[2])*(valNumX[5] - valNumX[2]) + (valNumY[5] - valNumY[2])*(valNumY[5] - valNumY[2]));

			val = 5 - ((Dis0*(5.0 / 3.0)) / Dis);
		}
	}
	//Done

	COLORREF CLColor[10];
	CLColor[9] = RGB(250, 0, 0);
	CLColor[8] = RGB(220, 46, 0);
	CLColor[7] = RGB(227, 97, 0);
	CLColor[6] = RGB(227, 137, 0);
	CLColor[5] = RGB(227, 176, 0);
	CLColor[4] = RGB(200, 200, 0);
	CLColor[3] = RGB(176, 227, 0);
	CLColor[2] = RGB(137, 227, 0);
	CLColor[1] = RGB(66, 207, 0);
	CLColor[0] = RGB(17, 247, 100);

	real Right = L + intRound(real(R - L) * 1 / 3);
	int sw = intRound((R - Right) / 10.0);
	int sT = intRound(T) + intRound(real(B - T) / 10.0);

	int SL = intRound(Right);
	int Left = intRound(Right);

	::SetRect(&Rect, Left, intRound(T) - 2 - FontSize, intRound(R), intRound(T) - 2);

	if (m_LTLabel[0].Find(_T("INTERNAL"), 0) != -1) s = "Dysfunctional Lens Index";
	else if (m_LTLabel[0].Find(_T("CORNEA"), 0) != -1) s = "Cornea Index";
	else if (m_LTLabel[0].Find(_T("TOTAL EYE"), 0) != -1) s = "TOTAL Eye Index";
	m_MemDC.WriteText(s, Rect, Font, white, 0);

	for (int i = 1; i <= 10; i++)
	{
		Right = Left + sw;
		if (i == 10) Right = R;

		if (i == 1)
		{
			::SetRect(&Rect, Left - FontSize, sT + 2, Left + FontSize, sT + 2 + FontSize);
			m_MemDC.WriteText("0", Rect, Font, CLColor[i - 1], 1);
		}

		::SetRect(&Rect, Left, intRound(T), intRound(Right), sT);
		m_MemDC.DrawRectangle(Rect, CLColor[i - 1], CLColor[i - 1]);

		Left = intRound(Right);

		if (i == 5 || i == 10)
		{
			s.Format(_T("%i"), i);

			::SetRect(&Rect, Left - FontSize, sT + 2, Left + FontSize, sT + 2 + FontSize);
			m_MemDC.WriteText(s, Rect, Font, CLColor[i - 1], 1);
		}
	}

	//Arrow and value
	int x0 = intRound(SL + val*(real)sw);

	int cn = intRound(val);//color index

	int x1 = x0 - 6;
	int x2 = x0 + 6;


	int y0 = sT + 2 + FontSize + 2;
	int y1 = y0 + 8;
	int y2 = y1;
	m_MemDC.DrawTriangle(x0, y0, x1, y1, x2, y2, 1, CLColor[cn], CLColor[cn]);

	x1 = x0 - 2;
	x2 = x0 + 2;

	y2 = y0 + 25;

	::SetRect(&Rect, x1, y1, x2, y2);
	m_MemDC.DrawRectangle(Rect, CLColor[cn], CLColor[cn]);
	//Arrow Done

	//Text
	x1 = x0 - 3 * FontSize;
	x2 = x0 + 3 * FontSize;

	y1 = y2 + 3;
	y2 = y1 + FontSize;
	::SetRect(&Rect, x1, y1, x2, y2);
	s.Format(_T("%.2f"), val);
	m_MemDC.WriteText(s, Rect, Font, CLColor[cn], 1);
	//Text Done

	m_MemDC.DrawCircle(intRound(valNumX[0]), intRound(valNumY[0]), 4, 1, GREEN, GREEN);
	m_MemDC.DrawCircle(intRound(valNumX[1]), intRound(valNumY[1]), 4, 1, BLUE, BLUE);
	m_MemDC.DrawCircle(intRound(valNumX[2]), intRound(valNumY[2]), 4, 1, ORANGE, ORANGE);

	//531 done

}

//531 Done
//***************************************************************************************

void CMTFWnd::Mtf3D()
{
	real _0 = m_Printing ? 1.0 : 0.0;
	real _1 = m_Printing ? 0.0 : 1.0;

	HGLRC hRenderingContext = NULL;
	int PixelFormat = ::ChoosePixelFormat(m_MemDC.m_hDC, &m_PixelFormatDescriptor);
	if (PixelFormat != 0) {
		if (::SetPixelFormat(m_MemDC.m_hDC, PixelFormat, &m_PixelFormatDescriptor)) {
			hRenderingContext = ::wglCreateContext(m_MemDC.m_hDC);
		}
	}
	if (hRenderingContext == NULL) return;

	::wglMakeCurrent(m_MemDC.m_hDC, hRenderingContext);

	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();
	real p = (real)m_w / m_h;
	::glOrtho(-p, p, -1.0, 1.0, -5.0, 5.0);

	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();
	::glRotated(-45 - m_ay, 1.0, 0.0, 0.0);
	::glRotated(-45 - m_ax, 0.0, 0.0, 1.0);
	::glTranslated(0.0, 0.0, -0.2);
	::glScaled(0.8, 0.8, 1.0);

	::glViewport(0, 0, m_w, m_h);

	::glClearColor((GLclampf)_0, (GLclampf)_0, (GLclampf)_0, (GLclampf)1.0);
	::glClearDepth(1.0);
	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	::glDepthFunc(GL_LEQUAL);
	::glEnable(GL_DEPTH_TEST);
	::glBlendFunc(GL_ONE, GL_ZERO);
	::glEnable(GL_BLEND);
	::glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	real g_cd = 5.0;
	real t_cd = 30.0;
	real cd_un = t_cd / 1.0;

	::glEnable(GL_POLYGON_OFFSET_FILL);
	::glPolygonOffset(0.0, 10000.0);

	int n = 30;
	real kc = 0.75;

	Matrix<real> mtf(n + 1, 180);
	for (int r = 0; r <= n; r++) {
		for (int a = 0; a < 180; a++) {
			real v;
			if (m_Mtf.GetAtCdRA(cd_un * r / n, a, v)) mtf(r, a) = v;
			else mtf(r, a) = INVALID_VALUE;
		}
	}

	for (int r = 0; r < n; r++) {

		int rr[4] = { r, r, r + 1, r + 1 };

		for (int a = 0; a < 360; a++) {

			int a1 = a == 359 ? 0 : a + 1;
			int aa[4] = { a, a1, a1, a };

			real x[4], y[4], z[4];

			int k;
			for (k = 0; k < 4; k++) {
				z[k] = mtf(rr[k], aa[k] % 180);
				if (z[k] == INVALID_VALUE) break;
				x[k] = COS[aa[k]] * rr[k] / n;
				y[k] = SIN[aa[k]] * rr[k] / n;
			}
			if (k < 4) continue;

			::glBegin(GL_POLYGON);
			for (k = 0; k < 4; k++) {
				real c = 0.25 + 0.75 * z[k];
				::glColor3d(c, 0.8*c, 0.0);
				::glVertex3d(x[k], y[k], z[k]);
			}
			::glEnd();

			if (aa[0] % 10 == 0) { // радиальные линии
				::glBegin(GL_LINES);
				int o[2] = { 0, 3 };
				for (int i = 0; i < 2; i++) {
					int j = o[i];
					real c = 0.15 + 0.75 * z[j];
					::glColor3d(c, 0.8*c, 0.0);
					::glVertex3d(x[j], y[j], z[j]);
				}
				::glEnd();
			}

			real r_cd = cd_un * rr[0] / n;
			if (fabs(r_cd - g_cd * (int)(r_cd / g_cd)) < 0.001) { // азимутальные линии
				::glBegin(GL_LINES);
				int o[2] = { 0, 1 };
				for (int i = 0; i < 2; i++) {
					int j = o[i];
					real c = 0.15 + 0.75 * z[j];
					::glColor3d(c, 0.8*c, 0.0);
					::glVertex3d(x[j], y[j], z[j]);
				}
				::glEnd();
			}

		}
	}

	::glDisable(GL_POLYGON_OFFSET_FILL);

	// X arrow
	::glBlendFunc(GL_ONE, GL_ZERO);
	::glColor3d(_1, _1, _1);
	::glBegin(GL_TRIANGLES);
	::glVertex3d(1.15, 0.0, 0.0); ::glVertex3d(1.13, -0.02, 0.0); ::glVertex3d(1.21, 0.0, 0.0);
	::glVertex3d(1.15, 0.0, 0.0); ::glVertex3d(1.13, 0.02, 0.0); ::glVertex3d(1.21, 0.0, 0.0);
	::glEnd();

	// Y arrow
	::glBlendFunc(GL_ONE, GL_ZERO);
	::glBegin(GL_TRIANGLES);
	::glVertex3d(0.0, 1.15, 0.0); ::glVertex3d(-0.02, 1.13, 0.0); ::glVertex3d(0.0, 1.21, 0.0);
	::glVertex3d(0.0, 1.15, 0.0); ::glVertex3d(0.02, 1.13, 0.0); ::glVertex3d(0.0, 1.21, 0.0);
	::glEnd();

	// circle
	::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	::glColor4d(0.5, 0.5, 0.5, 0.5);
	::glBegin(GL_LINE_LOOP);
	for (int a = 0; a < 360; a++) ::glVertex3d(SIN[a], COS[a], 0.0);
	::glEnd();

	// dots
	::glBlendFunc(GL_ONE, GL_ZERO);
	::glColor3d(_1, _1, _1);
	::glLineWidth(2.0);
	::glBegin(GL_LINES);
	for (int a = 0; a < 360; a += 10) {
		real x1 = 1.02 * COS[a]; real y1 = 1.02 * SIN[a];
		real x2 = 1.03 * COS[a]; real y2 = 1.03 * SIN[a];
		::glVertex3d(x1, y1, 0.0);
		::glVertex3d(x2, y2, 0.0);
	}
	::glEnd();
	::glLineWidth(1.0);

	// numbers
	int FontSize = intRound(0.045 * m_h);
	CMFont Font(FontSize, 400, "Arial");

	CFont* pFont = m_MemDC.SelectObject(&Font);

	GLYPHMETRICSFLOAT gm[10];
	wglUseFontOutlines(m_MemDC.m_hDC, 48, 10, 1, 0.0f, 0.0f, WGL_FONT_POLYGONS, gm);
	::glListBase(1);

	real f = m_SizeSmall ? 0.09 : 0.08;
	for (int i = 0; i < 360; i += 30) {
		::glBlendFunc(GL_ONE, GL_ZERO);
		::glColor3d(_1, _1, _1);
		int a; if (i == 0) a = i + 1; else if (i < 100) a = i + 2; else a = i + 3;
		if (a > 359) a -= 360;
		::glPushMatrix();
		::glTranslated(1.05 * COS[a], 1.05 * SIN[a], 0.0);
		::glRotated(i - 90, 0.0, 0.0, 1.0);
		::glScaled(f, f, f);
		uchar offs[3];
		uint n = 3;
		offs[0] = i / 100;
		offs[1] = (i % 100) / 10;
		offs[2] = i % 10;
		for (int j = 0; j < 2; j++) if (offs[0] == 0) { offs[0] = offs[1]; offs[1] = offs[2]; --n; }
		::glCallLists(n, GL_UNSIGNED_BYTE, offs);
		::glPopMatrix();
	}

	m_MemDC.SelectObject(pFont);

	::glFlush();

	::wglMakeCurrent(NULL, NULL);

	::wglDeleteContext(hRenderingContext);

	//---------------------------------------------------

	COLORREF white = m_Printing ? BLACK : WHITE;
	CString s;
	RECT Rect;

	// units
	s.Format(_T(" Radial spacing:  %i cycles / degree"), (int)g_cd);
	::SetRect(&Rect, 0, m_h - 3, m_w, m_h - 3 - FontSize);
	m_MemDC.WriteText(s, Rect, Font, white, 0);
}

//***************************************************************************************
