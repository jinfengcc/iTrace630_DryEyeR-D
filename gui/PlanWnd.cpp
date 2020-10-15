//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "PlanWnd.h"
#include "BusyCursor.h"
#include "Settings.h"

//***************************************************************************************

// distance from a1 to a2 counterclockwise Є [0, 360)
int angular_distance_counterclockwise(int a1, int a2)
{
	int da = (a2 - a1) % 360;
	if (da < 0) da += 360;
	return da;
}

//***************************************************************************************

// distance between a1 and a2 Є [0, 180)
int angular_distance(int a1, int a2)
{
	int da = angular_distance_counterclockwise(a1, a2);
	if (da > 180) da = 360 - da;
	return da;
}

//***************************************************************************************

int CPlanWnd::m_wheel_n = 0;

//***************************************************************************************

BEGIN_MESSAGE_MAP(CPlanWnd, CDispWnd)

	ON_WM_MOUSEMOVE()
	//ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_VIEW_BUTTON, OnViewButtonClicked)
	ON_BN_CLICKED(IDC_SHOW_MAP_ZALDIVAR_ITEM, OnZaldivarButtonClicked)
	ON_BN_CLICKED(IDC_OSHER_ALIGNMENT_ITEM, OnOSAliButtonClicked)
	ON_BN_CLICKED(IDC_REC_LENS_ITEM, OnRecalLenButtonClicked)
	ON_BN_CLICKED(IDC_RE_SCALE_ITEM, OnReScaleButtonClicked)
	ON_BN_CLICKED(IDC_NEXT, OnNextButtonClicked)
	ON_BN_CLICKED(IDC_PREV, OnPrevButtonClicked)

	//ON_WM_LBUTTONDBLCLK()//[520]

END_MESSAGE_MAP()

//***************************************************************************************

CPlanWnd::CPlanWnd(RECT& Rect, CWnd* pWnd) :
	CDispWnd(Rect, pWnd)
{
	m_pCTExam = NULL;

	m_SurgeonView = FALSE;
	m_ShowEye = TRUE;
	m_ShowMap = FALSE;
	m_ShowCaliper = (m_isCombo ? TRUE : FALSE);
	m_ShowOsherRing = FALSE;
	m_MoveCalipar = 0;

	m_MousePos = 0;

	m_l = intRound(0.12 * m_w);
	m_h_um = 16000.0;
	m_w_um = m_h_um * m_w / m_h;
	m_x_px_um = m_w / m_w_um;
	m_y_px_um = m_h / m_h_um;
	m_cx = m_l + 0.5 * (m_w - m_l);
	m_cy = 0.5 * m_h;

	RECT Rect1;
	CMRgn Rgn;

	int W = ::GetSystemMetrics(SM_CXSCREEN);
	real_t t = 0.02 * W;
	int h = intRound(t);
	int w = intRound(5.5 * t);

	int fs = intRound(0.02 * ::GetSystemMetrics(SM_CYSCREEN));

	::SetRect(&Rect1, m_w - 2 - w, m_h - 4 - 2 * h, m_w - 2, m_h - 4 - h);
	m_ViewButton.SetBkColor(NOCOLOR);
	m_ViewButton.SetText(m_SurgeonView ? "Standard View" : "Surgeon's View");
	m_ViewButton.SetFont(fs);
	m_ViewButton.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, Rect1, this, IDC_VIEW_BUTTON);
	Rgn.SetRectRgn(&Rect1);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	// [520]
	::SetRect(&Rect1, m_w - 2 - w, m_h - 2 - h, m_w - 2, m_h - 2);
	m_OSAlignmentButton.SetBkColor(NOCOLOR);
	m_OSAlignmentButton.SetText("Osher Alignment");
	m_OSAlignmentButton.SetFont(fs);
	m_OSAlignmentButton.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, Rect1, this, IDC_OSHER_ALIGNMENT_ITEM);
	Rgn.SetRectRgn(&Rect1);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	m_OverLap = 0;

	//show Zaldivar clipar
	::SetRect(&Rect1, m_w - 2 - 2 * w, m_h - 2 - h, m_w - 2 - w, m_h - 2);
	m_ZaldivarButton.SetBkColor(NOCOLOR);

	if (m_isCombo) m_ZaldivarButton.SetText("Protractor");
	else          m_ZaldivarButton.SetText("Zaldivar Caliper");

	m_ZaldivarButton.SetFont(fs);
	m_ZaldivarButton.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, Rect1, this, IDC_SHOW_MAP_ZALDIVAR_ITEM);
	Rgn.SetRectRgn(&Rect1);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	m_OverLap = 0;

	//move lens at 1 degree
	int s = intRound(0.035 * ::GetSystemMetrics(SM_CXSCREEN));

	::SetRect(&Rect1, m_w - intRound(w), m_l - 2 - intRound(2.0*h), m_w - intRound(w) + s, m_l - 10);
	m_PrevButton.Create(_T(""), _T(""), 0, Rect1, this, IDC_PREV);
	m_PrevButton.SetImage(W == 1920 ? IDR_LEFT_ARROW_3 : W == 1600 ? IDR_LEFT_ARROW_2 : IDR_LEFT_ARROW_1);
	m_PrevButton.SetBk(FALSE);
	m_PrevButton.SetBkColor(BLACK);
	m_PrevButton.SetTip("Lens rotation at -1°");
	Rgn.SetRectRgn(&Rect1);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);


	//::SetRect(&Rect1, m_w - 2 - intRound(0.5*w), m_h - 2 - intRound(2.4*h), m_w - 2 - intRound(0.5*w) + s, m_h - 2 - intRound(1.4*h));
	::SetRect(&Rect1, m_w - intRound(0.5*w), m_l - 2 - intRound(2.0*h), m_w - intRound(0.5*w) + s, m_l - 10);
	m_NextButton.Create(_T(""), _T(""), 0, Rect1, this, IDC_NEXT);
	m_NextButton.SetImage(W == 1920 ? IDR_RIGHT_ARROW_3 : W == 1600 ? IDR_RIGHT_ARROW_2 : IDR_RIGHT_ARROW_1);
	m_NextButton.SetBk(FALSE);
	m_NextButton.SetBkColor(BLACK);
	m_NextButton.SetTip("Lens roatation at +1°");
	Rgn.SetRectRgn(&Rect1);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	//Recalculate Lens placement Combox
	::SetRect(&Rect1, m_w - intRound(1.4*w), 3, m_w - intRound(1.4*w) + 13, 16);
	m_RecalLenButton.Create(_T(""), WS_CHILD | BS_CHECKBOX, Rect1, this, IDC_REC_LENS_ITEM);//Recalculate Lens placement
	m_MemDC.FillSolidRect(&Rect1, BLACK);
	m_RecalLenButton.ShowWindow(SW_HIDE);
	m_RecalLenButton.SetCheck(0);
	m_Recal_Lens = 0;
	Rgn.SetRectRgn(&Rect1);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	::SetRect(&Rect1, m_w - intRound(1.4*w), 20, m_w - intRound(1.4*w) + 13, 33);
	m_ReScale.Create(_T(""), WS_CHILD | BS_CHECKBOX, Rect1, this, IDC_RE_SCALE_ITEM);
	m_MemDC.FillSolidRect(&Rect1, BLACK);
	m_ReScale.ShowWindow(SW_SHOW);
	m_ReScale.SetCheck(0);
	Rgn.SetRectRgn(&Rect1);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);
}


////***************************************************************************************

void CPlanWnd::RepaintMemDC()
{
	//aaabbb
	switch (m_pCTExam->m_OpData.m_OpType) {
	case 0: m_LTLabel[0] = "IOL"; break;
	case 1: m_LTLabel[0] = "Toric IOL Enhancement"; break;
	case 2: m_LTLabel[0] = "ICL"; break;
	}

	//[//[cjf***05012012*005*]
	if (::TempSettings.NewPlannerSettings.m_NewPlaner == 1) {

		if (!m_isCombo)
		{
			m_pCTExam->m_OpData.m_RefAxis[0] = 0;
			m_pCTExam->m_OpData.m_RefAxis[1] = 180;
		}

		//[cjf***05022012*004*], set the GUI axes display
		m_ShowIncision = FALSE;
		m_ShowPreopCorneal = FALSE;
		m_ShowPreopInternal = FALSE;
		m_ShowPreopEntireEye = FALSE;
		m_PostopCorneal = FALSE;
		m_PostopInternal = FALSE;
		m_PostopEntireEye = TRUE;
		m_ShowICLIndSteep = FALSE;
		m_ShowICLIndFlat = FALSE;
		if (m_pCTExam->m_OpData.m_OpType == 0)//IOL
		{
			m_ShowIncision = 1;
		}
		else if (m_pCTExam->m_OpData.m_OpType == 2)//ICL
		{
			m_ShowPreopEntireEye = 1;
			m_ShowICLIndSteep = 1;
			m_PostopEntireEye = 0;
		}

		//[cjf***05022012*004*]
		if (!m_isCombo) m_ShowCaliper = FALSE;
		m_MoveCalipar = FALSE;//[cjf***05022012*004*]
	}
	::TempSettings.NewPlannerSettings.m_NewPlaner = 0;
	//[cjf***05012012*004*]

	if (m_pCTExam->m_OpData.m_OpType != 1)//IOL and ICL
	{
		m_PrevButton.ShowWindow(SW_SHOW);
		m_NextButton.ShowWindow(SW_SHOW);
		if (m_pCTExam->m_OpData.m_OpType == 2) m_PostopEntireEye = 0;
	}
	else {
		m_PrevButton.ShowWindow(SW_HIDE);
		m_NextButton.ShowWindow(SW_HIDE);
	}

	//531
	if (m_isCombo)
	{
		if (m_pCTExam->m_OpData.m_RefAxis[0] == INVALID_VALUE)
		{
			m_ZaldivarButton.ShowWindow(FALSE);
			m_ShowCaliper = FALSE;
		}
		else
		{
			if (m_pCTExam->m_OpData.m_RefAxis[0]< 0)
			{
				if (!m_ZaldivarButton.IsWindowVisible())
				{
					m_ShowCaliper = TRUE;
					m_ZaldivarButton.ShowWindow(TRUE);
				}
			}
			else
			{
				m_ZaldivarButton.ShowWindow(FALSE);
				m_ShowCaliper = FALSE;
			}
		}
	}
	//531 Done

	m_CorneaAxesColor = RED;
	m_InternalAxesColor = MAGENTA;
	m_EntireEyeAxesColor = 0x0000df00;
	m_IncisionColor = BLUE;
	m_ICLSteepAxisColor = ORANGE;
	m_ICLFlatAxisColor = 0x00afaf00;
	m_LensColor = 0x00007f9f;
	m_CaliperColor = RGB(230, 215, 30);//RGB(195,145,65);
	m_CaliperColor2 = RGB(255, 128, 0);//[cjf***05032012*004*]RGB(195,145,65);
	m_RotationColor = m_Printing ? ORANGE : YELLOW;

	ClearMemDC();

	Eye();
	Map();
	Lens1();

	BackupMemDC();

	Grid();

	if (m_ShowIncision)       Incision();//[cjf***05012012*004*]
	else  m_RecalLenButton.ShowWindow(SW_HIDE); //[cjf***05052012*008*]

	if (m_ShowPreopCorneal)   CorneaPreopAxes();//[cjf***05012012*004*]
	if (m_ShowPreopInternal)  InternalPreopAxes();//[cjf***05012012*004*]
	if (m_ShowPreopEntireEye) EyePreopAxes();//[cjf***05012012*004*]
	if (m_PostopCorneal)      CorneaPostopAxes();//[cjf***05012012*004*]
	if (m_PostopInternal)     InternalPostopAxes();//[cjf***05012012*004*]
	if (m_PostopEntireEye)    EyePostopAxes();//[cjf***05012012*004*]

	ICLPostopAxes();

	Lens2();

	if (!m_isCombo) Caliper();
	else           Protractor();

	Labels();

	Labels2();

	Border();

	Points();
}

//***************************************************************************************

void CPlanWnd::Eye()
{
	//[520]
	if (m_pCTExam->m_ColorImage.m_RGBData.GetMem() != NULL)
	{
		if (m_ShowColorEye)
		{
			m_pImage = &m_pCTExam->m_ColorImage;
			m_pImage->SwapImageInfo(&m_pCTExam->m_Image);
			if (!m_pImage->m_ve_ok)	m_pImage->FindWFVertex();
			if (!m_pImage->m_li_ok)
			{
				FindClearLimbus(m_pImage, m_pCTExam->m_Image.m_li_x0_um, m_pCTExam->m_Image.m_li_y0_um, m_pCTExam->m_Image.m_li_r_mean_um);
			}
			m_pImage->m_pu_ok = FALSE;
		}
		else
		{
			m_pImage = &m_pCTExam->m_Image;
			m_pImage->m_pu_ok = FALSE;
			m_pImage->m_ve_ok = FALSE;
		}
	}
	else
	{
		m_pImage = &m_pCTExam->m_Image;
		m_pImage->m_pu_ok = FALSE;
		m_pImage->m_ve_ok = FALSE;
	}

	//[520]*/
	if (!m_ShowEye) return;
	if (m_pImage == NULL) return;
	if (m_pImage->m_RGBData.GetMem() == NULL) return;

	real_t x_px_um = m_SurgeonView ? -m_x_px_um : m_x_px_um;
	real_t y_px_um = m_SurgeonView ? -m_y_px_um : m_y_px_um;

	real_t ve_x_um = m_pImage->m_ve_x_um;
	real_t ve_y_um = m_pImage->m_ve_y_um;
	for (int y = 0; y < m_h; y++) {
		real_t y_um = ve_y_um + (m_cy - y) / y_px_um;
		for (int x = m_l; x < m_w; x++) {
			real_t x_um = ve_x_um + (x - m_cx) / x_px_um;
			real_t r, g, b;
			if (m_pImage->GetRGBAtUm(x_um, y_um, &r, &g, &b)) {
				m_MemDC.SetPixel(x, y, (int)r, (int)g, (int)b);
			}
		}
	}
}

//***************************************************************************************

void CPlanWnd::Points()
{
	int x0, y0;
	//real t1 = m_pImage->m_ve_x_um* m_x_px_um;
	//real t2 = m_pImage->m_ve_y_um* m_y_px_um;

	if (m_pImage->m_ve_ok)
	{
		x0 = intRound(m_cx);
		y0 = intRound(m_cy);
		m_MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 3, RED);
		m_MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 3, RED);
	}

	/* if(m_pImage->m_li_ok)
	{
	x0 = intRound(m_cx + ( m_pImage->m_li_x0_um * m_x_px_um));
	y0 = intRound(m_cy - ( m_pImage->m_li_y0_um * m_y_px_um));
	m_MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 3, BLUE);
	m_MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 3, BLUE);

	}

	if(m_pImage->m_pu_ok)
	{
	x0 = intRound(m_cx + ( m_pImage->m_pu_x0_um * m_x_px_um));
	y0 = intRound(m_cy - ( m_pImage->m_pu_y0_um * m_y_px_um));
	m_MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 3, GREEN);
	m_MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 3, GREEN);
	}*/

}
//***************************************************************************************

void CPlanWnd::Map()
{
	if (!m_ShowMap) return;

	int NumColors = 15;
	COLORREF Colors[15];
	Colors[14] = 0x000000aa;
	Colors[13] = 0x000000dc;
	Colors[12] = 0x000064e6;
	Colors[11] = 0x000096f0;
	Colors[10] = 0x0000d7ff;
	Colors[9] = 0x0000ffff;
	Colors[8] = 0x0000dcb4;
	Colors[7] = 0x0000be00;
	Colors[6] = 0x006ed200;
	Colors[5] = 0x00dcdc00;
	Colors[4] = 0x00c88000;
	Colors[3] = 0x00c80000;
	Colors[2] = 0x00c8004b;
	Colors[1] = 0x00c80082;
	Colors[0] = 0x007d0052;
	real_t Inc = 0.25;
	real_t Cent = m_pCTExam->m_ax0_dp;
	real_t Min = m_Map2D.m_min;
	real_t Max = m_Map2D.m_max;
	real_t Span = 2.0 * __max(fabs(Min - Cent), fabs(Max - Cent));
	real_t t = Span / NumColors;
	real_t Step = (int)(t / Inc) * Inc;
	if (t > Step + 0.001) Step += Inc;

	Min = Cent - Step * (NumColors >> 1);

	int cx = intRound(m_cx);
	int cy = intRound(m_cy);

	for (int y = -m_Map2D.m_nr; y <= m_Map2D.m_nr; y++) {
		for (int x = -m_Map2D.m_nr; x <= m_Map2D.m_nr; x++) {
			real_t v = m_Map2D.GetAt(y, x);
			if (v != INVALID_VALUE) {
				int i = intRound((v - Min) / Step);
				if (i < 0) i = 0;
				else if (i > NumColors - 1) i = NumColors - 1;
				COLORREF C = Colors[i];
				int xx = m_SurgeonView ? cx - x : cx + x;
				int yy = m_SurgeonView ? cy + y : cy - y;
				m_MemDC.SetPixel(xx, yy, GetRValue(C), GetGValue(C), GetBValue(C), 128);
			}
		}
	}
}

//***************************************************************************************

void CPlanWnd::Grid()
{
	real_t x_px_um = m_SurgeonView ? -m_x_px_um : m_x_px_um;
	real_t y_px_um = m_SurgeonView ? -m_y_px_um : m_y_px_um;

	COLORREF Color = m_Printing ? BLACK : WHITE;

	CString s;

	int FontSize = intRound(0.045 * m_h);

	real_t r_um = 5000.0;

	// circle
	int rx = intRound(r_um * m_x_px_um);
	int ry = intRound(r_um * m_y_px_um);
	m_MemDC.DrawEllipse(intRound(m_cx), intRound(m_cy), rx, ry, 3, Color, NOCOLOR);

	if (m_ShowOsherRing) r_um += m_wheel_n * 100.0;

	// circle
	if (m_wheel_n != 0) {
		rx = intRound(r_um * m_x_px_um);
		ry = intRound(r_um * m_y_px_um);
		m_MemDC.DrawEllipse(intRound(m_cx), intRound(m_cy), rx, ry, 3, Color, NOCOLOR);
	}

	// numbers
	int IncreaseDeg = 5;
	if (m_wheel_n >= 10) IncreaseDeg = 1;//[cjf***04202012]


	for (int a = 0; a < 360; a += IncreaseDeg)
	{

		if (a == 20) a = a;

		real_t r1_um = r_um + 100.0;

		int x1 = intRound(m_cx + x_px_um * r1_um * COS[a]);
		int y1 = intRound(m_cy - y_px_um * r1_um * SIN[a]);

		real_t r2_um = 0;

		if (IncreaseDeg == 1) r2_um = r_um + ((a % 15) == 0 ? 400.0 : 150.0);//[cjf***04202012]
		else r2_um = r_um + ((a % 15) == 0 ? 400.0 : 250.0);//[cjf***04202012]

		if (IncreaseDeg == 1 && (a % 5) == 0 && (a % 15) != 0)	r2_um = r_um + 250;//[cjf***04202012]

		int x2 = intRound(m_cx + x_px_um * r2_um * COS[a]);
		int y2 = intRound(m_cy - y_px_um * r2_um * SIN[a]);
		int w = intRound(((a % 15) == 0 ? 0.006 : 0.003) * m_h);
		m_MemDC.DrawLine(x1, y1, x2, y2, w, Color);

		int sa = 0;
		if (a % 30 == 0)
		{
			real_t r3_um = r_um + 900.0;
			int x = intRound(m_cx + x_px_um * r3_um * COS[a]);
			int y = intRound(m_cy - y_px_um * r3_um * SIN[a]);

			sa = a;
			if (::TempSettings.Temp_ScaleRang)
			{
				if (a > 180) sa = a - 180;
			}

			s.Format(_T(" %i°"), sa);
			m_MemDC.WriteRotatedText(s, x, y, m_SurgeonView ? a + 90 : a - 90, FontSize, 400, "Arial", Color, NOCOLOR);
		}
	}
}

//***************************************************************************************

void CPlanWnd::Lens1()
{
	if (m_ShowMap) return;

	int R = GetRValue(m_LensColor);
	int G = GetGValue(m_LensColor);
	int B = GetBValue(m_LensColor);

	int cx = intRound(m_cx);
	int cy = intRound(m_cy);

	int rx = intRound(2500.0 * m_x_px_um);
	int ry = intRound(2500.0 * m_y_px_um);

	for (int y = -ry; y <= ry; y++) {
		for (int x = -rx; x <= rx; x++) {
			if (sqr(ry) * sqr(x) + sqr(rx) * sqr(y) <= sqr(rx * ry)) {
				m_MemDC.SetPixel(cx + x, cy - y, R, G, B, 64);
			}
		}
	}
}

//***************************************************************************************

void CPlanWnd::CorneaPreopAxes()
{
	if (m_pCTExam->m_OpData.m_CorneaPreopAxis == INVALID_VALUE) return;
	if (m_pCTExam->m_OpData.m_CorneaPreopAxis < 0 || m_pCTExam->m_OpData.m_CorneaPreopAxis >= 180) return;

	real_t x_px_um = m_SurgeonView ? -m_x_px_um : m_x_px_um;
	real_t y_px_um = m_SurgeonView ? -m_y_px_um : m_y_px_um;

	int r = intRound(0.005 * m_h);

	int a[2];
	a[0] = m_pCTExam->m_OpData.m_CorneaPreopAxis;
	a[1] = a[0] + 180;

	for (int i = 0; i < 2; i++) {
		for (int k = 0; k < 6; k++) {
			real_t r_um = 2750.0 + k * 400.0;
			int x = intRound(m_cx + r_um * COS[a[i]] * x_px_um);
			int y = intRound(m_cy - r_um * SIN[a[i]] * y_px_um);
			m_MemDC.DrawCircle(x, y, r, 1, m_CorneaAxesColor, m_CorneaAxesColor);
		}
	}

	int b = a[0];
	int x = intRound(m_cx + 7200.0 * COS[b] * x_px_um);
	int y = intRound(m_cy - 7200.0 * SIN[b] * y_px_um);
	CString s;
	s.Format(_T("%i°"), b);
	int o = CheckAngle((m_SurgeonView ? b + 90 : b - 90) - 1);
	int fs = intRound(0.045 * m_h);
	m_MemDC.WriteRotatedText(s, x, y, o, fs, 400, "Arial", m_CorneaAxesColor, NOCOLOR);
}

//***************************************************************************************

void CPlanWnd::CorneaPostopAxes()
{
	if (m_pCTExam->m_OpData.m_CorneaPostopAxis == INVALID_VALUE) return;
	if (m_pCTExam->m_OpData.m_CorneaPostopAxis < 0 || m_pCTExam->m_OpData.m_CorneaPostopAxis >= 180) return;

	real_t x_px_um = m_SurgeonView ? -m_x_px_um : m_x_px_um;
	real_t y_px_um = m_SurgeonView ? -m_y_px_um : m_y_px_um;

	int w = intRound(0.010 * m_h);

	int a[2];
	a[0] = m_pCTExam->m_OpData.m_CorneaPostopAxis;
	a[1] = a[0] + 180;

	for (int i = 0; i < 2; i++) {
		real_t r1_um = 2750.0;
		real_t r2_um = 4750.0;
		int x1 = intRound(m_cx + r1_um * COS[a[i]] * x_px_um);
		int y1 = intRound(m_cy - r1_um * SIN[a[i]] * y_px_um);
		int x2 = intRound(m_cx + r2_um * COS[a[i]] * x_px_um);
		int y2 = intRound(m_cy - r2_um * SIN[a[i]] * y_px_um);
		m_MemDC.DrawLine(x1, y1, x2, y2, w, m_CorneaAxesColor);
	}

	//darw a arrow line point to the degree text[cjf***04202012]
	for (int i = 0; i<14; i++)
	{
		int re = i % 2;
		if (re == 0)
		{
			int x1 = intRound(m_cx + (5000 + i * 100) * COS[a[0]] * x_px_um);
			int y1 = intRound(m_cy - (5000 + i * 100) * SIN[a[0]] * y_px_um);
			int x2 = intRound(m_cx + (5000 + (i + 1) * 100) * COS[a[0]] * x_px_um);
			int y2 = intRound(m_cy - (5000 + (i + 1) * 100) * SIN[a[0]] * y_px_um);
			m_MemDC.DrawLine(x1, y1, x2, y2, 1, m_CorneaAxesColor);
		}
	}
	//[cjf***04202012]

	int b = a[0];
	int x = intRound(m_cx + 6600.0 * COS[b] * x_px_um);//[cjf***04202012]
	int y = intRound(m_cy - 6600.0 * SIN[b] * y_px_um);//[cjf***04202012]
	CString s;
	s.Format(_T("%i°"), b);
	int o = CheckAngle((m_SurgeonView ? b + 90 : b - 90) - 1);
	int fs = intRound(0.045 * m_h);
	m_MemDC.WriteRotatedText(s, x, y, o, fs, 400, "Arial", m_CorneaAxesColor, NOCOLOR);
}

//***************************************************************************************

void CPlanWnd::Incision()
{
	if (m_pCTExam->m_OpData.m_OpType != 0 && m_pCTExam->m_OpData.m_OpType != 2) return;
	if (m_pCTExam->m_OpData.m_CorneaIncisionAxis == INVALID_VALUE) return;
	if (m_pCTExam->m_OpData.m_CorneaIncisionAxis < 0 || m_pCTExam->m_OpData.m_CorneaIncisionAxis >= 360) return;

	//[cjf***05052012*008*]show the recalculate of lens combox
	{
		RECT Rect;
		int FontSize = intRound(0.025 * m_h);
		real_t t = 0.02 * ::GetSystemMetrics(SM_CXSCREEN);
		int w = intRound(5.5 * t);
		COLORREF white = m_Printing ? BLACK : WHITE;

		CMFont Font(FontSize, 400, "Arial");

		m_RecalLenButton.ShowWindow(SW_SHOW);

		::SetRect(&Rect, m_w - intRound(1.3*w), 0, m_w, FontSize);
		CString s("Recalculate Lens Placement");
		m_MemDC.WriteText(s, Rect, Font, white, 1);
	}
	//[cjf***05052012*008*]

	real_t x_px_um = m_SurgeonView ? -m_x_px_um : m_x_px_um;
	real_t y_px_um = m_SurgeonView ? -m_y_px_um : m_y_px_um;

	int w = intRound(0.015 * m_h);

	int a = m_pCTExam->m_OpData.m_CorneaIncisionAxis;

	//[cjf***04202012] *006*
	int ShortenDeg_txt = 0;
	if (!m_SurgeonView)
	{
		if ((a > 258 && a < 315) || (a > 78 && a < 102)) ShortenDeg_txt = 1;
		if ((a > 47 && a < 58)) ShortenDeg_txt = 1;


		if (m_StepLength >= 5.5)
		{

			if (m_StepLength == 5.5)
			{
				if (a > 220 && a < 246) ShortenDeg_txt = 1;//40
			}
			else if (m_StepLength == 6.5) {
				if (a > 216 && a < 246) ShortenDeg_txt = 2;//40
			}
			else {
				if (a > 213 && a < 246) ShortenDeg_txt = 2;
			}
		}
	}
	else
	{
		if ((a > 258 && a < 280) || (a > 78 && a < 135)) ShortenDeg_txt = 1;
		if ((a > 239 && a < 224)) ShortenDeg_txt = 1;

		if (m_StepLength >= 5.5)
		{

			if (m_StepLength == 5.5)
			{
				if (a > 40 && a < 66) ShortenDeg_txt = 1;//40
			}
			else if (m_StepLength == 6.5) {
				if (a > 36 && a < 66) ShortenDeg_txt = 2;//40
			}
			else {
				if (a > 33 && a < 66) ShortenDeg_txt = 2;
			}
		}
	}
	//[cjf***04202012] *006*

	int x = intRound(m_cx);
	int y = intRound(m_cy);
	int r = intRound(5000.0 * m_x_px_um);
	int a1 = a - 5; if (m_SurgeonView) a1 += 180;
	int a2 = a + 5; if (m_SurgeonView) a2 += 180;
	m_MemDC.DrawArc(x, y, r, w, a1, a2, m_IncisionColor);

	//darw a arrow line point to the degree text[cjf***04202012]
	for (int i = 0; i<25; i++)
	{
		int re = i % 2;
		if (re == 0)
		{
			if (ShortenDeg_txt == 1 && i == 20) break;
			if (ShortenDeg_txt == 2 && i == 14) break;

			int x1 = intRound(m_cx + (5000 + i * 100) * COS[a] * x_px_um);
			int y1 = intRound(m_cy - (5000 + i * 100) * SIN[a] * y_px_um);
			int x2 = intRound(m_cx + (5000 + (i + 1) * 100) * COS[a] * x_px_um);
			int y2 = intRound(m_cy - (5000 + (i + 1) * 100) * SIN[a] * y_px_um);
			m_MemDC.DrawLine(x1, y1, x2, y2, 1, m_IncisionColor);
		}
	}
	//[cjf***04202012]
	int Dis = 7900;
	if (ShortenDeg_txt == 1)  Dis = 7200;
	else if (ShortenDeg_txt == 2)  Dis = 6600;
	x = intRound(m_cx + Dis * COS[a] * x_px_um);//7200,[cjf***04202012]
	y = intRound(m_cy - Dis * SIN[a] * y_px_um);//7200,[cjf***04202012]
	CString s;
	s.Format(_T("%i°"), a);
	int o = CheckAngle((m_SurgeonView ? a + 90 : a - 90) - 1);
	int fs = intRound(0.045 * m_h);
	m_MemDC.WriteRotatedText(s, x, y, o, fs, 400, "Arial", m_IncisionColor, NOCOLOR);
}

//***************************************************************************************
void CPlanWnd::EyePreopAxes()
{
	if (m_pCTExam->m_OpData.m_EyePreopAxis == INVALID_VALUE) return;
	if (m_pCTExam->m_OpData.m_EyePreopAxis < 0 || m_pCTExam->m_OpData.m_EyePreopAxis >= 180) return;

	real_t x_px_um = m_SurgeonView ? -m_x_px_um : m_x_px_um;
	real_t y_px_um = m_SurgeonView ? -m_y_px_um : m_y_px_um;

	int r = intRound(0.005 * m_h);

	int a[2];
	a[0] = m_pCTExam->m_OpData.m_EyePreopAxis;
	a[1] = a[0] + 180;

	for (int i = 0; i < 2; i++) {
		for (int k = 0; k < 6; k++) {
			real_t r_um = 2750.0 + k * 400.0;
			int x = intRound(m_cx + r_um * COS[a[i]] * x_px_um);
			int y = intRound(m_cy - r_um * SIN[a[i]] * y_px_um);
			m_MemDC.DrawCircle(x, y, r, 1, m_EntireEyeAxesColor, m_EntireEyeAxesColor);
		}
	}

	int x = intRound(m_cx + 7200.0 * COS[a[0]] * x_px_um);
	int y = intRound(m_cy - 7200.0 * SIN[a[0]] * y_px_um);
	CString s;
	s.Format(_T("%i°"), a[0]);
	int o = CheckAngle((m_SurgeonView ? a[0] + 90 : a[0] - 90) - 1);
	int fs = intRound(0.045 * m_h);
	m_MemDC.WriteRotatedText(s, x, y, o, fs, 400, "Arial", m_EntireEyeAxesColor, NOCOLOR);
}

//***************************************************************************************

void CPlanWnd::EyePostopAxes()
{
	if (m_pCTExam->m_OpData.m_EyePostopAxis == INVALID_VALUE) return;
	if (m_pCTExam->m_OpData.m_EyePostopAxis < 0 || m_pCTExam->m_OpData.m_EyePostopAxis >= 180) return;

	real_t x_px_um = m_SurgeonView ? -m_x_px_um : m_x_px_um;
	real_t y_px_um = m_SurgeonView ? -m_y_px_um : m_y_px_um;

	int w = intRound(0.010 * m_h);
	int halfw = intRound(0.005 * m_h);//[cjf***04202012]

	int a[2];
	a[0] = m_pCTExam->m_OpData.m_EyePostopAxis % 180; if (a[0] < 0) a[0] += 180;
	a[1] = a[0] + 180;

	//[cjf***04202012], wether the tow axies are overlaped
	BOOL m_pre_post_SamePos = 0;
	int c[1];
	c[0] = m_pCTExam->m_OpData.m_CorneaPostopAxis;

	if (a[0] == c[0]) m_pre_post_SamePos = 1;
	else m_pre_post_SamePos = 0;
	//[cjf***04202012]

	//[cjf***04202012]
	int ShortenDeg_txt = 0;
	if (a[0] > 78 && a[0] < 136) ShortenDeg_txt = 1;
	else if (m_SurgeonView && a[0] > 34 && a[0] < 66) ShortenDeg_txt = 2;
	else ShortenDeg_txt = 0;
	//[cjf***04202012]

	for (int i = 0; i < 2; i++) {
		real_t r1_um = 2750.0;
		real_t r2_um = 4750.0;
		int x1 = intRound(m_cx + r1_um * COS[a[i]] * x_px_um);
		int y1 = intRound(m_cy - r1_um * SIN[a[i]] * y_px_um);
		int x2 = intRound(m_cx + r2_um * COS[a[i]] * x_px_um);
		int y2 = intRound(m_cy - r2_um * SIN[a[i]] * y_px_um);
		//[cjf***04202012]
		if (m_pre_post_SamePos && m_PostopCorneal)//[cjf***05012012*004*]
												  //if( m_pre_post_SamePos)
		{
			x1 = x1 - 1;
			x2 = x2 - 1;
			y1 = y1 - 1;
			y2 = y2 - 1;
			m_MemDC.DrawLine(x1, y1, x2, y2, halfw, m_EntireEyeAxesColor);
		}
		//[cjf***04202012]
		else  m_MemDC.DrawLine(x1, y1, x2, y2, w, m_EntireEyeAxesColor);
	}

	//darw a arrow line point to the degree text[cjf***04202012]
	for (int i = 0; i<25; i++)
	{
		int re = i % 2;
		if (re == 0)
		{
			if (ShortenDeg_txt == 1 && i == 20) break;
			if (ShortenDeg_txt == 2 && i == 16) break;

			int x1 = intRound(m_cx + (5000 + i * 100) * COS[a[0]] * x_px_um);
			int y1 = intRound(m_cy - (5000 + i * 100) * SIN[a[0]] * y_px_um);
			int x2 = intRound(m_cx + (5000 + (i + 1) * 100) * COS[a[0]] * x_px_um);
			int y2 = intRound(m_cy - (5000 + (i + 1) * 100) * SIN[a[0]] * y_px_um);
			m_MemDC.DrawLine(x1, y1, x2, y2, 1, m_EntireEyeAxesColor);
		}
	}
	//[cjf***04202012]
	int Dis = 7900;
	if (ShortenDeg_txt == 1)  Dis = 7200;
	else if (ShortenDeg_txt == 2) Dis = 6800;
	int x = intRound(m_cx + Dis * COS[a[0]] * x_px_um);//7200,[cjf***04202012]
	int y = intRound(m_cy - Dis * SIN[a[0]] * y_px_um);//7200,[cjf***04202012]
	CString s;
	s.Format(_T("%i°"), a[0]);
	int o = CheckAngle((m_SurgeonView ? a[0] + 90 : a[0] - 90) - 1);
	int fs = intRound(0.045 * m_h);
	m_MemDC.WriteRotatedText(s, x, y, o, fs, 400, "Arial", m_EntireEyeAxesColor, NOCOLOR);
}

//***************************************************************************************

void CPlanWnd::InternalPreopAxes()
{
	if (m_pCTExam->m_OpData.m_InternalPreopAxis == INVALID_VALUE) return;
	if (m_pCTExam->m_OpData.m_InternalPreopAxis < 0 || m_pCTExam->m_OpData.m_InternalPreopAxis >= 180) return;

	real_t x_px_um = m_SurgeonView ? -m_x_px_um : m_x_px_um;
	real_t y_px_um = m_SurgeonView ? -m_y_px_um : m_y_px_um;

	int r = intRound(0.005 * m_h);

	int a[2];
	a[0] = m_pCTExam->m_OpData.m_InternalPreopAxis;
	a[1] = a[0] + 180;

	for (int i = 0; i < 2; i++) {
		for (int k = 0; k < 6; k++) {
			real_t r_um = 2250.0 - k * 400.0;
			int x = intRound(m_cx + r_um * COS[a[i]] * x_px_um);
			int y = intRound(m_cy - r_um * SIN[a[i]] * y_px_um);
			m_MemDC.DrawCircle(x, y, r, 1, m_InternalAxesColor, m_InternalAxesColor);
		}
	}

	int x = intRound(m_cx + 3500.0 * COS[a[0]] * x_px_um);//[cjf***04202012]
	int y = intRound(m_cy - 3500.0 * SIN[a[0]] * y_px_um);//[cjf***04202012]
	CString s;
	s.Format(_T("%i°"), a[0]);
	int o = CheckAngle((m_SurgeonView ? a[0] + 90 : a[0] - 90) - 1);
	int fs = intRound(0.045 * m_h);
	m_MemDC.WriteRotatedText(s, x, y, o, fs, 400, "Arial", m_InternalAxesColor, NOCOLOR);
}

//***************************************************************************************

void CPlanWnd::InternalPostopAxes()
{
	if (m_pCTExam->m_OpData.m_OpType != 0 && m_pCTExam->m_OpData.m_OpType != 1) return;
	if (m_pCTExam->m_OpData.m_InternalPostopAxis == INVALID_VALUE) return;
	if (m_pCTExam->m_OpData.m_InternalPostopAxis < 0 || m_pCTExam->m_OpData.m_InternalPostopAxis >= 180) return;

	real_t x_px_um = m_SurgeonView ? -m_x_px_um : m_x_px_um;
	real_t y_px_um = m_SurgeonView ? -m_y_px_um : m_y_px_um;

	int w = intRound(0.010 * m_h);

	int a[2];
	a[0] = m_pCTExam->m_OpData.m_InternalPostopAxis;
	a[1] = a[0] + 180;

	for (int i = 0; i < 2; i++) {
		real_t r1_um = 250.0;
		real_t r2_um = 2250.0;
		int x1 = intRound(m_cx + r1_um * COS[a[i]] * x_px_um);
		int y1 = intRound(m_cy - r1_um * SIN[a[i]] * y_px_um);
		int x2 = intRound(m_cx + r2_um * COS[a[i]] * x_px_um);
		int y2 = intRound(m_cy - r2_um * SIN[a[i]] * y_px_um);
		m_MemDC.DrawLine(x1, y1, x2, y2, w, m_InternalAxesColor);
	}

	int x = intRound(m_cx + 3000.0 * COS[a[0]] * x_px_um);
	int y = intRound(m_cy - 3000.0 * SIN[a[0]] * y_px_um);
	CString s;
	s.Format(_T("%i°"), a[0]);
	int o = CheckAngle((m_SurgeonView ? a[0] + 90 : a[0] - 90) - 1);
	int fs = intRound(0.045 * m_h);
	m_MemDC.WriteRotatedText(s, x, y, o, fs, 400, "Arial", m_InternalAxesColor, NOCOLOR);
}

//***************************************************************************************

void CPlanWnd::ICLPostopAxes()
{
	if (m_pCTExam->m_OpData.m_OpType != 2) return;
	if (m_pCTExam->m_OpData.m_ICL.m_InducedAxis == INVALID_VALUE) return;
	if (m_pCTExam->m_OpData.m_ICL.m_InducedAxis < 0 || m_pCTExam->m_OpData.m_ICL.m_InducedAxis >= 180) return;
	if (!m_ShowICLIndSteep && !m_ShowICLIndFlat) return;//[cjf***05072012]

	real_t x_px_um = m_SurgeonView ? -m_x_px_um : m_x_px_um;
	real_t y_px_um = m_SurgeonView ? -m_y_px_um : m_y_px_um;

	int w = intRound(0.010 * m_h);

	int a[4];
	a[0] = m_pCTExam->m_OpData.m_ICL.m_InducedAxis;
	a[1] = a[0] + 180;
	a[2] = (a[0] + 90) % 180;
	a[3] = a[2] + 180;

	//[cjf***05072012]
	int start = 0;
	int end = 4;
	if (m_ShowICLIndSteep && !m_ShowICLIndFlat) end = 2;//show steep and do not show flat
	if (!m_ShowICLIndSteep && m_ShowICLIndFlat) start = 2;//show steep and do not show flat
														  //[cjf***05072012]

	for (int i = start; i < end; i++) {
		real_t r1_um = 250.0;
		real_t r2_um = 2250.0;
		int x1 = intRound(m_cx + r1_um * COS[a[i]] * x_px_um);
		int y1 = intRound(m_cy - r1_um * SIN[a[i]] * y_px_um);
		int x2 = intRound(m_cx + r2_um * COS[a[i]] * x_px_um);
		int y2 = intRound(m_cy - r2_um * SIN[a[i]] * y_px_um);
		m_MemDC.DrawLine(x1, y1, x2, y2, w, i < 2 ? m_ICLSteepAxisColor : m_ICLFlatAxisColor);
	}

	if (m_ShowICLIndSteep)//[cjf***05072012]
	{
		int x = intRound(m_cx + 3000.0 * COS[a[0]] * x_px_um);
		int y = intRound(m_cy - 3000.0 * SIN[a[0]] * y_px_um);
		CString s;
		s.Format(_T("%i°"), a[0]);
		int o = CheckAngle((m_SurgeonView ? a[0] + 90 : a[0] - 90) - 1);
		int fs = intRound(0.045 * m_h);
		m_MemDC.WriteRotatedText(s, x, y, o, fs, 400, "Arial", m_ICLSteepAxisColor, NOCOLOR);
	}
}

//***************************************************************************************

void CPlanWnd::Lens2()
{
	real_t x_px_um = m_SurgeonView ? -m_x_px_um : m_x_px_um;
	real_t y_px_um = m_SurgeonView ? -m_y_px_um : m_y_px_um;

	int w = intRound(0.010 * m_h);
	int r = intRound(0.005 * m_h);

	int a[2][2];
	if (m_pCTExam->m_OpData.m_OpType == 0) {
		a[0][0] = INVALID_VALUE;
		a[0][1] = INVALID_VALUE;
		a[1][0] = m_pCTExam->m_OpData.m_IOLPlacementAxis;
	}
	else if (m_pCTExam->m_OpData.m_OpType == 1) {
		a[0][0] = (m_pCTExam->m_OpData.m_InternalPreopAxis + 90) % 180;
		a[0][1] = a[0][0] + 180;
		a[1][0] = m_pCTExam->m_OpData.m_IOLPlacementAxis;
	}
	else if (m_pCTExam->m_OpData.m_OpType == 2) {
		a[0][0] = INVALID_VALUE;
		a[0][1] = INVALID_VALUE;
		a[1][0] = m_pCTExam->m_OpData.m_ICL.m_PlacementAxis;
	}
	else return;
	a[1][1] = a[1][0] + 180;

	// кольцо
	m_MemDC.DrawEllipse(intRound(m_cx), intRound(m_cy), intRound(2500.0 * m_x_px_um), intRound(2500.0 * m_y_px_um), w, m_LensColor, NOCOLOR);

	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 2; i++)
		{
			int b = a[j][i];
			if (b == INVALID_VALUE) continue;
			if (m_pCTExam->m_OpData.m_OpType == 1)
			{
				if (j == 1)
				{
					COLORREF Color = m_Printing ? ORANGE : YELLOW;
					real_t r1_um = 1500.0;
					real_t r2_um = 2500.0;
					int x1 = intRound(m_cx + r1_um * COS[b] * x_px_um);
					int y1 = intRound(m_cy - r1_um * SIN[b] * y_px_um);
					int x2 = intRound(m_cx + r2_um * COS[b] * x_px_um);
					int y2 = intRound(m_cy - r2_um * SIN[b] * y_px_um);
					m_MemDC.DrawLine(x1, y1, x2, y2, w, Color);
				}
			}

			for (int k = 0; k < 3; k++)
			{
				real_t r_um = 2250.0 - k * 400.0;
				int x = intRound(m_cx + r_um * COS[b] * x_px_um);
				int y = intRound(m_cy - r_um * SIN[b] * y_px_um);
				m_MemDC.DrawCircle(x, y, r, 1, m_LensColor, m_LensColor);
			}
			//
			if (m_pCTExam->m_OpData.m_OpType == 0 || m_pCTExam->m_OpData.m_OpType == 1)
			{
				real_t x1_um = 2500.0 * COS[b];
				real_t y1_um = 2500.0 * SIN[b];

				int a0 = CheckAngle(b + 60);
				real_t x0_um = 1500.0 * COS[a0];
				real_t y0_um = 1500.0 * SIN[a0];
				int x0 = intRound(m_cx + x0_um * x_px_um);
				int y0 = intRound(m_cy - y0_um * y_px_um);

				real_t r_um = hyp((x1_um - x0_um), (y1_um - y0_um));

				int o = intRound(_180_Pi * (angle(y1_um - y0_um, x1_um - x0_um)));

				if (j == 0)
				{
					for (int k = 0; k < 10; k++)
					{
						int e = CheckAngle(o);
						int x = intRound(m_cx + (x0_um + r_um * COS[e]) * x_px_um);
						int y = intRound(m_cy - (y0_um + r_um * SIN[e]) * y_px_um);
						m_MemDC.DrawCircle(x, y, r, 1, m_LensColor, m_LensColor);
						o += 10;
					}
				}
				else
				{
					int rt = abs(intRound(r_um * x_px_um));
					m_MemDC.DrawArc(x0, y0, rt, w, m_SurgeonView ? o + 180 : o, m_SurgeonView ? o + 270 : o + 90, m_LensColor);
				}
			}
		}
	}

	if (m_pCTExam->m_OpData.m_OpType != 1) return;

	int FontSize = intRound(0.045 * m_h);

	if (angular_distance(a[0][0], a[1][0]) > angular_distance(a[0][0], a[1][1])) Swap(a[1][0], a[1][1]);

	for (int i = 0; i < 2; i++)
	{
		int a1, a2;
		if (angular_distance_counterclockwise(a[0][i], a[1][i]) < 180)
		{
			a1 = a[0][i];
			a2 = a[1][i];
			m_Clock_wise = 1;
		}
		else
		{
			a1 = a[1][i];
			a2 = a[0][i];
			m_Clock_wise = 0;
		}

		if (m_SurgeonView)
		{
			a1 += 180;
			a2 += 180;
		}

		int x = intRound(m_cx);
		int y = intRound(m_cy);
		int ra = intRound(2500.0 * m_x_px_um);

		if (m_Clock_wise)
		{
			a2 += 180;
			if (a2 > 360) a2 %= 360;
			int ADis = abs(a2 - a1);
			if (ADis != 180)
			{
				m_MemDC.DrawArc(x, y, ra, w, a2, a1, m_RotationColor);
			}
		}
		else
		{
			int ADis = abs(a2 - a1);
			if (ADis != 180)
				m_MemDC.DrawArc(x, y, ra, w, a1, a2, m_RotationColor);
		}

		// текст
		int amin = std::min(a[0][i], a[1][i]);
		int amax = std::max(a[0][i], a[1][i]);
		int a0 = (amin + amax) / 2;

		if (amax - amin > 180) { // лежат по разные стороны нуля
			a0 = (a0 + 180) % 360;
		}

		if (m_Clock_wise && amin != amax)
		{
			a0 += 90;
		}

		if (a0 > 360) a0 %= 360;
		x = intRound(m_cx + 3000.0 * COS[a0] * x_px_um);
		y = intRound(m_cy - 3000.0 * SIN[a0] * y_px_um);
		CString s;
		int rotatedA = angular_distance(a[0][i], a[1][i]);
		if (m_Clock_wise) rotatedA = 180 - rotatedA;

		if (rotatedA == 180) rotatedA = 0;

		s.Format(_T("%i°"), rotatedA);
		m_Angular_distance = s;

		int o = CheckAngle((m_SurgeonView ? a0 + 90 : a0 - 90) - 1);

		m_MemDC.WriteRotatedText(s, x, y, o, FontSize, 400, "Arial", m_RotationColor, NOCOLOR);
	}
}

//***************************************************************************************

void CPlanWnd::Protractor()
{
	if (!m_ShowCaliper) return;

	real_t x_px_um = m_SurgeonView ? -m_x_px_um : m_x_px_um;
	real_t y_px_um = m_SurgeonView ? -m_y_px_um : m_y_px_um;


	int FontSize = intRound(0.045 * m_h);

	int aw[3], stopPos;
	stopPos = 3;

	GetProAngs(m_pCTExam->m_OpData.m_RefAxis[0], aw[0], aw[1], aw[2]);

	aw[0] %= 360; if (aw[0] < 0) aw[0] += 360;
	aw[1] %= 360; if (aw[1] < 0) aw[1] += 360;
	if (aw[2] != -1)
	{
		aw[2] %= 360;
		if (aw[2] < 0) aw[2] += 360;
	}
	else            stopPos = 2;

	int x1 = intRound(m_cx);
	int y1 = intRound(m_cy);
	int w = intRound(0.005 * m_h);

	int fs = 20;
	for (int i = 0; i < stopPos; i++)
	{
		/*int x2 = intRound(m_cx + 6400.0 * COS[aw[i]] * x_px_um);
		int y2 = intRound(m_cy - 6400.0 * SIN[aw[i]] * y_px_um);*/

		int x2 = intRound(m_cx + m_pCTExam->m_OpData.m_RefLength[i+1]* COS[aw[i]] * x_px_um);//6.2.0
		int y2 = intRound(m_cy - m_pCTExam->m_OpData.m_RefLength[i + 1] * SIN[aw[i]] * y_px_um);//6.2.0

		m_MemDC.DrawLine(x1, y1, x2, y2, w, CYAN);
		m_MemDC.DrawCircle(x2, y2, w, w, CYAN, CYAN);

		CString s;
		if (i == 0) s = "1";
		else if (i == 1) s = "2";
		else if (i == 2) s = "3";

		int angle = aw[i];

		int angle2 = angle;

		if (angle2 < 0) angle2 = 360 + angle2;

		if (x2 < x1) angle += 180;

		int x = intRound(x2 + 20 * SIN[angle2]);
		int y = intRound(y2 + 20 * COS[angle2]);

		m_MemDC.WriteRotatedText(s, x, y, angle, fs, 400, "Arial", RED, NOCOLOR);
	}

	int a = m_pCTExam->m_OpData.m_IOLPlacementAxis;
	x1 = intRound(m_cx + m_pCTExam->m_OpData.m_RefLength[0] * COS[a] * x_px_um);//6.2.0
	y1 = intRound(m_cy - m_pCTExam->m_OpData.m_RefLength[0] * SIN[a] * y_px_um);//6.2.0

	int a2 = (a + 180) % 360;
	int x2 = intRound(m_cx + m_pCTExam->m_OpData.m_RefLength[0] * COS[a2] * x_px_um);
	int y2 = intRound(m_cy - m_pCTExam->m_OpData.m_RefLength[0] * SIN[a2] * y_px_um);

	m_MemDC.DrawLine(x1, y1, x2, y2, w, MAGENTA);
	m_MemDC.DrawCircle(x1, y1, w, w, MAGENTA, MAGENTA);
	m_MemDC.DrawCircle(x2, y2, w, w, MAGENTA, MAGENTA);

	//Degree
	int aUsed[3], aDis, a01, a02, a03, o, x, y, r;
	FontSize = intRound(0.03 * m_h);
	for (int i = 0; i < stopPos; i++)
	{
		a = m_pCTExam->m_OpData.m_IOLPlacementAxis;

		aDis = abs(a - aw[i]);

		BOOL Switch = FALSE;
		if (aDis > 90)
		{
			int aTest = (a + 180) % 360;

			if (aTest > 270 && aw[i] < 90 && (aw[i] + 360 - aTest) <= 90)
			{
				a = aTest;
			}
			else
			{
				if (abs(aTest - aw[i]) > 90)
				{
					Switch = TRUE;
				}
				else a = aTest;
			}
		}

		if (Switch)
		{
			a01 = aw[i];
			a02 = a;
		}
		else
		{
			a01 = a;
			a02 = aw[i];
		}

		aUsed[i] = a;

		if (!(angular_distance_counterclockwise(a, aw[i]) < 180))
		{
			a01 = aw[i];
			a02 = a;
		}

		w = intRound(0.002 * m_h);

		if (i == 0) r = intRound(4500 * x_px_um);
		else if (i == 1) r = intRound(3200 * x_px_um);
		else if (i == 2) r = intRound(1900 * x_px_um);
		m_MemDC.DrawArc(intRound(m_cx), intRound(m_cy), r, w, a01, a02 - 1, MAGENTA);

		//Text
		int  ADis = SmallAng(a, aw[i]);
		CString s;
		s.Format(_T("%i°"), ADis);

		int amin = std::min(a01, a02);
		int amax = std::max(a01, a02);
		a03 = (amin + amax) / 2;

		if (amax - amin > 180) { // лежат по разные стороны нуля
			a03 = (a03 + 180) % 360;
		}
		x = intRound(m_cx + (r + 15) * COS[a03]);
		y = intRound(m_cy - (r + 15) * SIN[a03]);

		o = CheckAngle((a03 - 90) - 1);

		m_MemDC.WriteRotatedText(s, x, y, o, FontSize, 400, "Arial", RED, NOCOLOR);
	}
	//Done


	//Another degree line and Degree
	if (aw[2] == -1)
	{
		for (int i = 0; i < 2; i++)
		{
			if (i == 0) r = intRound(4500 * x_px_um);
			else if (i == 1) r = intRound(3200 * x_px_um);

			a = (aUsed[i] + 180) % 360;;

			a01 = a;
			a02 = aw[i];

			if (!(angular_distance_counterclockwise(a, aw[i]) < 180))
			{
				a01 = aw[i];
				a02 = a;
			}

			if (a01 > a02)
			{
				if (a01 > 180) a01 -= 360;
				else
				{
					a02 = aw[i];
					a01 = a;
				}
			}

			int t = 0;
			for (int j = a01; j <= a02; j = j + 5)
			{
				if (t == 0)
				{
					int stAngle = j + 5;
					if (stAngle > a02) stAngle = a02;
					m_MemDC.DrawArc(intRound(m_cx), intRound(m_cy), r, w, j, stAngle, BROWN);
				}

				if (t == 0) t = 1;
				else t = 0;
			}

			//Degree text
			int ADis = SmallAng(a, aw[i]);
			CString s;
			s.Format(_T("%i°"), ADis);

			int amin = std::min(a01, a02);
			int amax = std::max(a01, a02);
			a03 = (amin + amax) / 2;

			if (amax - amin > 180) { // лежат по разные стороны нуля
				a03 = (a03 + 180) % 360;
			}
			if (a03 < 0) a03 += 360;
			x = intRound(m_cx + (r + 15) * COS[a03]);
			y = intRound(m_cy - (r + 15) * SIN[a03]);

			o = CheckAngle((a03 - 90) - 1);

			m_MemDC.WriteRotatedText(s, x, y, o, FontSize, 400, "Arial", RED, NOCOLOR);
		}
	}
	//Done
}

//***************************************************************************************

void CPlanWnd::GetProAngs(int inputV, int &a0, int &a1, int &a2)
{
	CString inputS;
	inputS.Format(_T("%i"), inputV);

	if (inputS.Right(3) == "370")
	{
		a2 = -1;
	}
	else
	{
		G_As = inputS.Right(3);
		a2 = (atoi(G_As)) % 360;
	}

	inputS.Truncate(8);
	G_As = inputS.Right(3);
	a1 = (atoi(G_As)) % 360;

	inputS.Truncate(5);
	G_As = inputS.Right(3);
	a0 = (atoi(G_As)) % 360;
}

//***************************************************************************************

void CPlanWnd::Caliper()
{
	if (!m_ShowCaliper) return;

	real_t x_px_um = m_SurgeonView ? -m_x_px_um : m_x_px_um;
	real_t y_px_um = m_SurgeonView ? -m_y_px_um : m_y_px_um;

	CString s;

	int FontSize = intRound(0.045 * m_h);

	int aw[2] = { m_pCTExam->m_OpData.m_RefAxis[0], m_pCTExam->m_OpData.m_RefAxis[1] };

	if (aw[0] == INVALID_VALUE)
	{
		if (aw[1] == INVALID_VALUE) return;
		aw[0] = aw[1];
	}
	else if (aw[1] == INVALID_VALUE)
	{
		aw[1] = aw[0];
	}

	aw[0] %= 360; if (aw[0] < 0) aw[0] += 360;
	aw[1] %= 360; if (aw[1] < 0) aw[1] += 360;

	for (int i = 0; i < 2; i++)
	{
		int x1 = intRound(m_cx + 5100.0 * COS[aw[i]] * x_px_um);
		int y1 = intRound(m_cy - 5100.0 * SIN[aw[i]] * y_px_um);
		int x2 = intRound(m_cx + 6400.0 * COS[aw[i]] * x_px_um);
		int y2 = intRound(m_cy - 6400.0 * SIN[aw[i]] * y_px_um);
		int w = intRound(0.01 * m_h);

		if (m_OverLap) m_MemDC.DrawLine(x1, y1, x2, y2, w, m_CaliperColor2);
		else m_MemDC.DrawLine(x1, y1, x2, y2, w, m_CaliperColor);
	}

	int ar[2];

	if (m_pCTExam->m_OpData.m_OpType == 0)
	{
		ar[0] = m_pCTExam->m_OpData.m_IOLPlacementAxis;
	}
	else if (m_pCTExam->m_OpData.m_OpType == 1)
	{
		ar[0] = (m_pCTExam->m_OpData.m_InternalPostopAxis + 90) % 180;
	}
	else if (m_pCTExam->m_OpData.m_OpType == 2)
	{
		ar[0] = m_pCTExam->m_OpData.m_ICL.m_PlacementAxis;
	}
	else return;

	ar[0] %= 180; if (ar[0] < 0) ar[0] += 180;
	ar[1] = ar[0] + 180;

	for (int i = 0; i < 2; i++)
	{
		int x1 = intRound(m_cx + 5100.0 * COS[ar[i]] * x_px_um);
		int y1 = intRound(m_cy - 5100.0 * SIN[ar[i]] * y_px_um);
		int x2 = intRound(m_cx + 6400.0 * COS[ar[i]] * x_px_um);
		int y2 = intRound(m_cy - 6400.0 * SIN[ar[i]] * y_px_um);
		int w = intRound(0.01 * m_h);
		m_MemDC.DrawLine(x1, y1, x2, y2, w, m_LensColor);
	}

	for (int i = 0; i < 2; i++)
	{
		int a1, a2;

		if (angular_distance_counterclockwise(aw[i], ar[i]) < 180)
		{
			a1 = aw[i];
			a2 = ar[i];
		}
		else
		{
			a1 = ar[i];
			a2 = aw[i];
		}
		if (m_SurgeonView)
		{
			a1 += 180;
			a2 += 180;
		}

		int x = intRound(m_cx);
		int y = intRound(m_cy);
		int r = intRound(6500.0 * m_x_px_um);
		int w = intRound(0.01 * m_h);
		m_MemDC.DrawArc(x, y, r, w, a1, a2, m_CaliperColor);

		int amin = std::min(aw[i], ar[i]);
		int amax = std::max(aw[i], ar[i]);
		int a0 = (amin + amax) / 2;

		if (amax - amin > 180)
		{
			a0 = (a0 + 180) % 360;
		}

		x = intRound(m_cx + 7200.0 * COS[a0] * x_px_um);
		y = intRound(m_cy - 7200.0 * SIN[a0] * y_px_um);
		s.Format(_T("%i°"), angular_distance(aw[i], ar[i]));

		int o = CheckAngle((m_SurgeonView ? a0 + 90 : a0 - 90) - 1);
		m_MemDC.WriteRotatedText(s, x, y, o, FontSize, 400, "Arial", m_CaliperColor, NOCOLOR);
	}
}

//*****************************************************************************************************
void CPlanWnd::Labels2()
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF black = m_Printing ? WHITE : BLACK;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	RECT Rect;

	CString s;

	BOOL OD = m_pCTExam->m_Header.IsOD();

	int FontSize1 = intRound(0.045 * m_h);
	int FontSize2 = intRound(0.070 * m_h);
	int FontSize3 = intRound(0.025 * m_h);

	CMFont Font1(FontSize1, 400, "Arial");
	CMFont Font3(FontSize3, 400, "Arial");

	::SetRect(&Rect, 0, 20, m_w, 20 + FontSize3);
	s = "Scale Range : 0° - 180°";
	m_MemDC.WriteText(s, Rect, Font3, white, 2);

	// left label
	s = ((OD && !m_SurgeonView) || (!OD && m_SurgeonView)) ? "temporal" : "nasal";
	m_MemDC.WriteRotatedText(s, m_l + intRound(0.5 * FontSize1), intRound(0.5 * m_h), 90, FontSize1, 400, "Arial", white, NOCOLOR);

	// right label
	s = ((OD && !m_SurgeonView) || (!OD && m_SurgeonView)) ? "nasal" : "temporal";
	m_MemDC.WriteRotatedText(s, m_w - intRound(0.5 * FontSize1), intRound(0.5 * m_h), 90, FontSize1, 400, "Arial", white, NOCOLOR);

	// top label
	::SetRect(&Rect, m_l, 0, m_w, FontSize1);
	s = m_SurgeonView ? "inferior" : "superior";
	m_MemDC.WriteText(s, Rect, Font1, white, 1);

	// bottom label
	::SetRect(&Rect, m_l, m_h - FontSize1, m_w, m_h);
	s = m_SurgeonView ? "superior" : "inferior";
	m_MemDC.WriteText(s, Rect, Font1, white, 1);

	// surgeon's view label
	::SetRect(&Rect, 0, 0, m_l, m_h);
	m_MemDC.DrawRectangle(Rect, gray, black);

	if (m_SurgeonView)
	{
		m_MemDC.WriteRotatedText("SURGEON'S VIEW", intRound(0.5 * m_l), intRound(0.5 * m_h), 90, FontSize2, 400, "Arial", white, NOCOLOR);
	}

	// legend
	real_t dx_um = 400.0;
	int l1 = m_l + intRound(0.010 * m_w);
	int l2 = l1 + intRound(2 * dx_um * m_x_px_um);
	int l3 = l2 + intRound(0.010 * m_w);
	int w = intRound(0.010 * m_h);
	int r = intRound(0.005 * m_h);

	m_StepLength = 0.5;

	if (m_pCTExam->m_OpData.m_OpType == 2 && m_ShowICLIndFlat)
	{
		m_StepLength += 1;

		int y1 = intRound(m_h - m_StepLength * FontSize3);
		int y2 = m_h - intRound(m_StepLength + 0.5) * FontSize3;

		m_MemDC.DrawLine(l1, y1, l2, y1, w, m_ICLFlatAxisColor);
		::SetRect(&Rect, l3, y2, m_w, y2 + FontSize3);
		m_MemDC.WriteText("ICL Induced Flat", Rect, Font3, white, 0);
	}

	if (m_pCTExam->m_OpData.m_OpType == 2 && m_ShowICLIndSteep)
	{
		m_StepLength += 1;

		int y1 = intRound(m_h - m_StepLength * FontSize3);
		int y2 = m_h - intRound(m_StepLength + 0.5) * FontSize3;

		m_MemDC.DrawLine(l1, y1, l2, y1, w, m_ICLSteepAxisColor);
		::SetRect(&Rect, l3, y2, m_w, y2 + FontSize3);
		m_MemDC.WriteText("ICL Induced Steep", Rect, Font3, white, 0);
	}
	else
	{
		if (m_PostopInternal)
		{
			m_StepLength += 1;

			int y1 = intRound(m_h - m_StepLength * FontSize3);
			int y2 = m_h - intRound(m_StepLength + 0.5) * FontSize3;

			m_MemDC.DrawLine(l1, y1, l2, y1, w, m_InternalAxesColor);
			::SetRect(&Rect, l3, y2, m_w, y2 + FontSize3);
			m_MemDC.WriteText("Post-Op Internal Optics", Rect, Font3, white, 0);
		}
	}

	if (m_PostopEntireEye)
	{
		m_StepLength += 1;

		int y1 = intRound(m_h - m_StepLength * FontSize3);
		int y2 = m_h - intRound(m_StepLength + 0.5) * FontSize3;

		m_MemDC.DrawLine(l1, y1, l2, y1, w, m_EntireEyeAxesColor);
		::SetRect(&Rect, l3, y2, m_w, y2 + FontSize3);
		m_MemDC.WriteText("Post-Op Entire Eye", Rect, Font3, white, 0);//cjfaaa do we need change here?
	}

	if (m_PostopCorneal)
	{
		m_StepLength += 1;

		int y1 = intRound(m_h - m_StepLength * FontSize3);
		int y2 = m_h - intRound(m_StepLength + 0.5) * FontSize3;

		m_MemDC.DrawLine(l1, y1, l2, y1, w, m_CorneaAxesColor);
		::SetRect(&Rect, l3, y2, m_w, y2 + FontSize3);
		m_MemDC.WriteText("Post-Op Corneal", Rect, Font3, white, 0);
	}

	if (m_ShowPreopEntireEye)
	{
		m_StepLength += 1;
		int y1 = intRound(m_h - m_StepLength * FontSize3);
		int y2 = m_h - intRound(m_StepLength + 0.5) * FontSize3;

		for (int k = 0; k < 3; k++)
		{
			int x = intRound(l1 + k * dx_um * m_x_px_um);
			m_MemDC.DrawCircle(x, y1, r, 1, m_EntireEyeAxesColor, m_EntireEyeAxesColor);
		}

		::SetRect(&Rect, l3, y2, m_w, y2 + FontSize3);
		m_MemDC.WriteText("Pre-Op Entire Eye", Rect, Font3, white, 0);//cjfaaa do we need change here?
	}

	if (m_ShowPreopInternal)
	{
		m_StepLength += 1;
		int y1 = intRound(m_h - m_StepLength * FontSize3);
		int y2 = m_h - intRound(m_StepLength + 0.5) * FontSize3;

		for (int k = 0; k < 3; k++)
		{
			int x = intRound(l1 + k * dx_um * m_x_px_um);
			m_MemDC.DrawCircle(x, y1, r, 1, m_InternalAxesColor, m_InternalAxesColor);
		}

		::SetRect(&Rect, l3, y2, m_w, y2 + FontSize3);
		m_MemDC.WriteText("Pre-Op Internal Optics", Rect, Font3, white, 0);
	}

	if (m_ShowPreopCorneal)
	{
		m_StepLength += 1;

		int y1 = intRound(m_h - m_StepLength * FontSize3);
		int y2 = m_h - intRound(m_StepLength + 0.5) * FontSize3;

		for (int k = 0; k < 3; k++)
		{
			int x = intRound(l1 + k * dx_um * m_x_px_um);
			m_MemDC.DrawCircle(x, y1, r, 1, m_CorneaAxesColor, m_CorneaAxesColor);
		}

		::SetRect(&Rect, l3, y2, m_w, y2 + FontSize3);
		m_MemDC.WriteText("Pre-Op Corneal", Rect, Font3, white, 0);
	}

	if (m_ShowIncision)
	{
		m_StepLength += 1;
		int y1 = intRound(m_h - m_StepLength * FontSize3);
		int y2 = m_h - intRound(m_StepLength + 0.5) * FontSize3;
		if (m_pCTExam->m_OpData.m_OpType == 0 || m_pCTExam->m_OpData.m_OpType == 2) {
			m_MemDC.DrawLine(l1, y1, l2, y1, w, m_IncisionColor);
			::SetRect(&Rect, l3, y2, m_w, y2 + FontSize3);
			m_MemDC.WriteText("Surgical Incision", Rect, Font3, white, 0);
		}
	}

	if (!m_isCombo)
	{
		int y1 = intRound(m_h - 0.5 * FontSize3);
		int y2 = m_h - FontSize3;
		m_MemDC.DrawLine(l1, y1, l2, y1, w, m_CaliperColor);
		::SetRect(&Rect, l3, y2, m_w, y2 + FontSize3);
		m_MemDC.WriteText("Zaldivar Caliper", Rect, Font3, white, 0);
	}

	//Show the amount and direction of rotation of IOC enhancement
	if (m_pCTExam->m_OpData.m_OpType == 1)//IOC enhancement
	{
		int FontSize = intRound(m_LTLabelFontSizePrc * m_h);
		int y1 = intRound(FontSize);
		int y2 = y1 + FontSize3;
		::SetRect(&Rect, l1 + 3, y1, m_w, y2);
		CString s;
		s = "Rotate " + m_Angular_distance + " ";

		if (m_Angular_distance != "0°")
			s += "clock-wise";
		else s += "degree";

		m_MemDC.WriteText(s, Rect, Font3, white, 0);

		//draw a arrow circle in the
		int x = intRound(m_cx);
		int y = intRound(m_cy);
		int w = intRound(0.002 * m_w);
		int l1 = intRound(0.005 * m_w);
		int r = intRound(300.0 * m_x_px_um);
		int a1 = 0;
		int a2 = 300;

		m_MemDC.DrawArc(x, y, r, w, a1, a2, m_CaliperColor);
		m_MemDC.DrawLine(x + r, y, x + r - l1, y - l1, w, m_CaliperColor);
		m_MemDC.DrawLine(x + r, y, x + r + l1, y - l1, w, m_CaliperColor);
	}
}

//***************************************************************************************

void CPlanWnd::OnMouseMove(uint nFlags, CPoint Point)
{
	CDispWnd::OnMouseMove(nFlags, Point);

	if (nFlags & MK_LBUTTON) {

		BOOL Redraw = FALSE;

		real_t x_px_um = m_SurgeonView ? -m_x_px_um : m_x_px_um;
		real_t y_px_um = m_SurgeonView ? -m_y_px_um : m_y_px_um;

		real_t x_um = (Point.x - m_cx) / x_px_um;
		real_t y_um = (m_cy - Point.y) / y_px_um;
		real_t a_rd = angle(y_um, x_um);
		int a = intRound(_180_Pi * a_rd) % 360; if (a < 0) a += 360;
		real_t d2 = sqr(x_um) + sqr(y_um);

		//cancel by [cjf***05032012*004*]
		/* if (sqr(6400.0) < d2) {
		if (m_MousePos == 0 || m_MousePos == 3) {
		if (m_ShowCaliper) {
		m_MousePos = 3;
		m_pCTExam->m_OpData.m_RefAxis[x_um >= 0.0 ? 0 : 1] = INVALID_VALUE;
		Redraw = TRUE;
		}
		}
		}*/

		if (sqr(5100.0) <= d2 && d2 <= sqr(6400.0))
		{ //else
			if (m_MousePos == 0 || m_MousePos == 3) {
				if (m_ShowCaliper && !m_isCombo) {
					m_MousePos = 3;

					////[cjf***05032012*005*]
					if (m_MoveCalipar == 0)
					{
						int disA1 = SmallAng(a, m_pCTExam->m_OpData.m_RefAxis[0]);
						int disA2 = SmallAng(a, m_pCTExam->m_OpData.m_RefAxis[1]);
						int disA = disA1 - disA2;
						m_MoveCalipar = (disA < 0 ? 1 : 2);
						//m_MoveCalipar = ( abs(a - m_pCTExam->m_OpData.m_RefAxis[0]) < abs(a - m_pCTExam->m_OpData.m_RefAxis[1]) ? 1 : 2 );
					}

					int ar[2];
					if (m_pCTExam->m_OpData.m_OpType == 0) {
						ar[0] = m_pCTExam->m_OpData.m_IOLPlacementAxis;
					}
					else if (m_pCTExam->m_OpData.m_OpType == 1) {
						ar[0] = (m_pCTExam->m_OpData.m_InternalPostopAxis + 90) % 180;
					}
					else if (m_pCTExam->m_OpData.m_OpType == 2) {
						ar[0] = m_pCTExam->m_OpData.m_ICL.m_PlacementAxis;
					}
					else return;
					ar[0] %= 180; if (ar[0] < 0) ar[0] += 180;
					ar[1] = ar[0] + 180;

					//judge there is overlap or not
					int br[2];
					if (m_MoveCalipar == 1)
					{
						br[0] = a;
						br[1] = m_pCTExam->m_OpData.m_RefAxis[1];
					}
					else
					{
						br[0] = m_pCTExam->m_OpData.m_RefAxis[0];
						br[1] = a;
					}

					m_OverLap = OverLapFun(ar[0], ar[1], br[0], br[1]);

					if (m_OverLap)// If it is overlap
					{
						if (m_MoveCalipar == 1) m_pCTExam->m_OpData.m_RefAxis[1] = a;
						else m_pCTExam->m_OpData.m_RefAxis[0] = a;
					}

					m_pCTExam->m_OpData.m_RefAxis[m_MoveCalipar - 1] = a;
					//[cjf***05032012*005*]

					// m_pCTExam->m_OpData.m_RefAxis[x_um >= 0.0 ? 0 : 1] = a;
					Redraw = TRUE;
				}
			}
		}

		else if (sqr(2600.0) <= d2 && d2 <= sqr(4900.0)) {
			if (m_MousePos == 0 || m_MousePos == 2) {
				if (m_pCTExam->m_OpData.m_OpType == 0 || m_pCTExam->m_OpData.m_OpType == 2) {
					m_MousePos = 2;
					m_pCTExam->m_OpData.m_CorneaIncisionAxis = a;

					//[cjf***05052012*008*]
					if (m_Recal_Lens)
					{
						m_pCTExam->m_OpData.m_IOLPlacementAxis = INVALID_VALUE;
					}
					//[cjf***05052012*008*]

					m_pCTExam->m_OpData.Recalculate();

					//[cjf***05082012], check the overlap of caliper
					if (m_ShowCaliper && m_Recal_Lens)
					{
						////[cjf***05032012*005*]
						int ar[2];
						if (m_pCTExam->m_OpData.m_OpType == 0) {
							ar[0] = m_pCTExam->m_OpData.m_IOLPlacementAxis;
						}
						else if (m_pCTExam->m_OpData.m_OpType == 1) {
							ar[0] = (m_pCTExam->m_OpData.m_InternalPostopAxis + 90) % 180;
						}
						else if (m_pCTExam->m_OpData.m_OpType == 2) {
							ar[0] = m_pCTExam->m_OpData.m_ICL.m_PlacementAxis;
						}
						else return;
						ar[0] %= 180; if (ar[0] < 0) ar[0] += 180;
						ar[1] = ar[0] + 180;

						//judge there is overlap or not
						int br[2];
						if (m_MoveCalipar == 1)
						{
							br[0] = a;
							br[1] = m_pCTExam->m_OpData.m_RefAxis[1];
						}
						else
						{
							br[0] = m_pCTExam->m_OpData.m_RefAxis[0];
							br[1] = a;
						}

						m_OverLap = OverLapFun(ar[0], ar[1], br[0], br[1]);

						if (m_OverLap)// If it is overlap
						{
							if (m_MoveCalipar == 1) m_pCTExam->m_OpData.m_RefAxis[1] = a;
							else m_pCTExam->m_OpData.m_RefAxis[0] = a;
						}
						//[cjf***05032012*005*]
					}
					//[cjf***05082012]

					GetParent()->PostMessage(WM_UPDATE_INFO, (WPARAM)0, (LPARAM)0);
					Redraw = TRUE;
				}
			}
		}

		else if (d2 <= sqr(2400.0)) {
			if (m_MousePos == 0 || m_MousePos == 1) {
				if (m_pCTExam->m_OpData.m_OpType == 0 || m_pCTExam->m_OpData.m_OpType == 2) {
					m_MousePos = 1;
					if (m_pCTExam->m_OpData.m_OpType == 0) {
						m_pCTExam->m_OpData.m_IOLPlacementAxis = a % 180;
					}
					else {
						m_pCTExam->m_OpData.m_ICL.m_PlacementAxis = a % 180;
					}
					m_pCTExam->m_OpData.Recalculate();

					//[cjf***05082012], check the overlap of caliper
					if (m_ShowCaliper)
					{
						////[cjf***05032012*005*]
						if (m_MoveCalipar == 0) m_MoveCalipar = (abs(a - m_pCTExam->m_OpData.m_RefAxis[0]) < abs(a - m_pCTExam->m_OpData.m_RefAxis[1]) ? 1 : 2);

						int ar[2];
						if (m_pCTExam->m_OpData.m_OpType == 0) {
							ar[0] = m_pCTExam->m_OpData.m_IOLPlacementAxis;
						}
						else if (m_pCTExam->m_OpData.m_OpType == 1) {
							ar[0] = (m_pCTExam->m_OpData.m_InternalPostopAxis + 90) % 180;
						}
						else if (m_pCTExam->m_OpData.m_OpType == 2) {
							ar[0] = m_pCTExam->m_OpData.m_ICL.m_PlacementAxis;
						}
						else return;
						ar[0] %= 180; if (ar[0] < 0) ar[0] += 180;
						ar[1] = ar[0] + 180;

						//judge there is overlap or not
						int br[2];
						if (m_MoveCalipar == 1)
						{
							br[0] = a;
							br[1] = m_pCTExam->m_OpData.m_RefAxis[1];
						}
						else
						{
							br[0] = m_pCTExam->m_OpData.m_RefAxis[0];
							br[1] = a;
						}

						m_OverLap = OverLapFun(ar[0], ar[1], br[0], br[1]);

						if (m_OverLap)// If it is overlap
						{
							if (m_MoveCalipar == 1) m_pCTExam->m_OpData.m_RefAxis[1] = a;
							else m_pCTExam->m_OpData.m_RefAxis[0] = a;
						}
						//[cjf***05032012*005*]
					}
					//[cjf***05082012]

					GetParent()->PostMessage(WM_UPDATE_INFO, (WPARAM)0, (LPARAM)0);
					Redraw = TRUE;
				}
			}
		}

		if (Redraw) {

			RestoreMemDC();

			Grid();
			if (m_ShowIncision)       Incision();//[cjf***05012012*004*]
			else  m_RecalLenButton.ShowWindow(SW_HIDE); //[cjf***05052012*008*]

			if (m_ShowPreopCorneal)   CorneaPreopAxes();//[cjf***05012012*004*]
			if (m_ShowPreopInternal)  InternalPreopAxes();//[cjf***05012012*004*]
			if (m_ShowPreopEntireEye) EyePreopAxes();//[cjf***05012012*004*]
			if (m_PostopCorneal)      CorneaPostopAxes();//[cjf***05012012*004*]
			if (m_PostopInternal)     InternalPostopAxes();//[cjf***05012012*004*]
			if (m_PostopEntireEye)    EyePostopAxes();//[cjf***05012012*004*]

			Lens2();
			ICLPostopAxes();
			//Caliper();
			if (!m_isCombo) Caliper();
			else          Protractor();

			Labels();
			Labels2();
			Border();
			Points();

			InvalidateRgn(&m_Rgn, FALSE);
			UpdateWindow();
		}

	}
}

//***************************************************************************************
//[cjf***05032012*005*] wether two arcs are overlap;
BOOL CPlanWnd::OverLapFun(int a0, int a1, int b0, int b1)
{
	int rangMin = 0;
	int rangMax = 0;

	b1 = ((b1 - a0)<0 ? 360 + (b1 - a0) : (b1 - a0));
	b0 = ((b0 - a0)<0 ? 360 + (b0 - a0) : (b0 - a0));
	a1 = ((a1 - a0)<0 ? 360 + (a1 - a0) : (a1 - a0));


	if (b0 > 180) {
		rangMin = b0;
		rangMax = 360;
	}
	else rangMax = b0;

	if (b1 <= rangMax && b1 >= rangMin) return TRUE;


	return FALSE;
}
//***************************************************************************************
int  CPlanWnd::SmallAng(int a0, int a1)
{
	int dis = abs(a0 - a1);
	if (dis > 180) dis = 360 - dis;

	return dis;
}

//***************************************************************************************

////[520]
//void CPlanWnd::OnLButtonDblClk(uint nFlags, CPoint Point)
//{
//  GetParent()->PostMessage(WM_NOTIFY_LBTN_DBCLK, (WPARAM)0, (LPARAM)0);
//}
////[520]

//***************************************************************************************

void CPlanWnd::OnLButtonDown(uint nFlags, CPoint Point)
{
	CDispWnd::OnLButtonDown(nFlags, Point);

	//OnMouseMove(nFlags, Point);
	if (m_ShowCaliper && m_ShowOsherRing)
	{
		m_MousePos = 0;
		m_MoveCalipar = 0;
	}

	OnMouseMove(nFlags, Point);
}

//***************************************************************************************

void CPlanWnd::OnLButtonUp(uint nFlags, CPoint Point)
{
	CDispWnd::OnLButtonUp(nFlags, Point);

	m_MousePos = 0;
	m_MoveCalipar = 0;
}

//***************************************************************************************

void CPlanWnd::OnViewButtonClicked()
{
	CBusyCursor Cursor;

	m_SurgeonView = !m_SurgeonView;

	Repaint();

	m_ViewButton.SetText(m_SurgeonView ? "Standard View" : "Surgeon's View");
	m_ViewButton.Repaint();
}

//***************************************************************************************
//[cjf***05022012*004*]
void CPlanWnd::OnZaldivarButtonClicked()
{
	m_ShowCaliper = !m_ShowCaliper;
	::Settings.m_PlannerSettings.m_MapShowZaldivarCaliper = m_ShowCaliper;
	Repaint();
}
//[cjf***05022012*004*]

//***************************************************************************************

//[520]
void CPlanWnd::OnOSAliButtonClicked()
{
	GetParent()->PostMessage(WM_NOTIFY_LBTN_DBCLK, (WPARAM)0, (LPARAM)0);
}
//[520]

//***************************************************************************************
//[cjf***05052012*008*]
void CPlanWnd::OnRecalLenButtonClicked()
{
	m_RecalLenButton.SetCheck(!m_RecalLenButton.GetCheck());
	m_Recal_Lens = (m_RecalLenButton.GetCheck() ? 1 : 0);
	Repaint();
}
//[cjf***05052012*008*]

//***************************************************************************************
//[cjf***05052012*008*]
void CPlanWnd::OnReScaleButtonClicked()
{
	m_ReScale.SetCheck(!m_ReScale.GetCheck());
	::TempSettings.Temp_ScaleRang = m_ReScale.GetCheck();
	Repaint();
}
//[cjf***05052012*008*]

//***************************************************************************************
void CPlanWnd::OnMouseWheel(uint nFlags, short zDelta, CPoint Point)
{
	if (!m_ShowOsherRing) return;

	m_wheel_n += zDelta / WHEEL_DELTA;
	if (m_wheel_n < -40) {
		m_wheel_n = -40;
		return;
	}
	else if (m_wheel_n > 30) {
		m_wheel_n = 30;
		return;
	}

	RestoreMemDC();

	Grid();
	if (m_ShowIncision)       Incision();//[cjf***05012012*004*]
	else  m_RecalLenButton.ShowWindow(SW_HIDE); //[cjf***05052012*008*]

	if (m_ShowPreopCorneal)   CorneaPreopAxes();//[cjf***05012012*004*]
	if (m_ShowPreopInternal)  InternalPreopAxes();//[cjf***05012012*004*]
	if (m_ShowPreopEntireEye) EyePreopAxes();//[cjf***05012012*004*]
	if (m_PostopCorneal)      CorneaPostopAxes();//[cjf***05012012*004*]
	if (m_PostopInternal)     InternalPostopAxes();//[cjf***05012012*004*]
	if (m_PostopEntireEye)    EyePostopAxes();//[cjf***05012012*004*]

	Lens2();
	ICLPostopAxes();

	if (!m_isCombo) Caliper();
	else          Protractor();

	Labels();
	Labels2();
	Border();
	Points();

	InvalidateRgn(&m_Rgn, FALSE);
	UpdateWindow();
}

//***************************************************************************************
//[cjf***05072012] used for rotating lens at 1 degree in Toric IOL planner
void CPlanWnd::OnPrevButtonClicked()
{
	if (m_pCTExam->m_OpData.m_OpType == 0) {
		int a = m_pCTExam->m_OpData.m_IOLPlacementAxis - 1;
		if (a < 0) a += 360;
		m_pCTExam->m_OpData.m_IOLPlacementAxis = a % 180;
	}
	else if (m_pCTExam->m_OpData.m_OpType == 2) {
		int a = m_pCTExam->m_OpData.m_ICL.m_PlacementAxis - 1;
		if (a < 0) a += 360;
		m_pCTExam->m_OpData.m_ICL.m_PlacementAxis = a % 180;
	}
	m_pCTExam->m_OpData.Recalculate();
	GetParent()->PostMessage(WM_UPDATE_INFO, (WPARAM)0, (LPARAM)0);
	Repaint();
}

void CPlanWnd::OnNextButtonClicked()
{
	if (m_pCTExam->m_OpData.m_OpType == 0) {
		int a = m_pCTExam->m_OpData.m_IOLPlacementAxis + 1;
		if (a < 0) a += 360;
		m_pCTExam->m_OpData.m_IOLPlacementAxis = a % 180;
	}
	else if (m_pCTExam->m_OpData.m_OpType == 2) {
		int a = m_pCTExam->m_OpData.m_ICL.m_PlacementAxis + 1;
		if (a < 0) a += 360;
		m_pCTExam->m_OpData.m_ICL.m_PlacementAxis = a % 180;
	}
	m_pCTExam->m_OpData.Recalculate();

	////531 Keep the protractor position
	//int aw[3];

	//GetProAngs(m_pCTExam->m_OpData.m_RefAxis[0], aw[0], aw[1], aw[2]);

	////531

	GetParent()->PostMessage(WM_UPDATE_INFO, (WPARAM)0, (LPARAM)0);
	Repaint();
}
//[cjf***05072012]

//**********************************************************************************
void CPlanWnd::FindClearLimbus(CEyeImage* OriImage, real_t LastLimbuX, real_t LastLimbuY, real_t LastLimbuR)
{
	CEyeImage* TestImage = new CEyeImage();
	int h = TestImage->m_h = OriImage->m_h;
	int w = TestImage->m_w = OriImage->m_w;
	TestImage->m_h_um = OriImage->m_h_um;
	TestImage->m_w_um = OriImage->m_w_um;

	TestImage->m_RGBData.Create(OriImage->m_h, (int)OriImage->m_w * 3, OriImage->m_RGBData.GetMem());

	if (TestImage->m_RGBData.GetMem() == NULL) return;

	int num = w*h;
	int AveG = 0;

	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			int R0 = TestImage->GetRAt(i, j);
			int G0 = TestImage->GetGAt(i, j);
			int B0 = TestImage->GetBAt(i, j);
			int Gray0 = intRound(0.3*R0 + 0.59*G0 + 0.11*B0);
			AveG += Gray0;
		}
	}

	AveG = intRound(AveG / num);

	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			int R0 = TestImage->GetRAt(i, j);
			int G0 = TestImage->GetGAt(i, j);
			int B0 = TestImage->GetBAt(i, j);
			int Gray0 = intRound(0.3*R0 + 0.59*G0 + 0.11*B0);

			int NewGray = AveG + 2 * (Gray0 - AveG);

			if (NewGray > 250) NewGray = 250;
			if (NewGray < 0)  NewGray = 0;

			TestImage->SetRAt(i, j, NewGray);
			TestImage->SetGAt(i, j, NewGray);
			TestImage->SetBAt(i, j, NewGray);
		}
	}

	TestImage->FindLimbus2(LastLimbuX, LastLimbuY, LastLimbuR);
	//TestImage->FindLimbus();

	OriImage->m_li_ok = TestImage->m_li_ok;
	OriImage->m_li_r_max_um = TestImage->m_li_r_max_um;
	OriImage->m_li_r_mean_um = TestImage->m_li_r_mean_um;
	OriImage->m_li_r_min_um = TestImage->m_li_r_min_um;
	for (int i = 0; i < 360; i++)	OriImage->m_li_r_um[i] = TestImage->m_li_r_um[i];
	OriImage->m_li_x0_um = TestImage->m_li_x0_um;
	OriImage->m_li_y0_um = TestImage->m_li_y0_um;

	delete TestImage;
}

//***************************************************************************************
