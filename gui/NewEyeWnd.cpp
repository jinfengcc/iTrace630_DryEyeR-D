//521, Copy code from EyeWnd, but add the grid value for WFCTOUSumWnd to make the 
//wafefront/refraction map to be bigger
//a new variable 'scale' is added here
//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "EyeWnd.h"
#include "NewEyeWnd.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CNewEyeWnd, CDispWnd)

	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(WM_CHANGE_POS, OnChangePosMsg)
	ON_BN_CLICKED(IDC_INC_CENT, OnIncCentButtonClicked)
	ON_BN_CLICKED(IDC_DEC_CENT, OnDecCentButtonClicked)
	ON_BN_CLICKED(IDC_ZOOM_IN, OnZoomInButtonClicked)
	ON_BN_CLICKED(IDC_ZOOM_OUT, OnZoomOutButtonClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CNewEyeWnd::CNewEyeWnd(RECT& Rect, CWnd* pWnd, int Scale) : CDispWnd(Rect, pWnd)
{
	m_pWFExam = NULL;
	m_pCTExam = NULL;

	m_MapShowEye = FALSE;
	m_MapShowMap = FALSE;
	m_MapShowPupil = FALSE;
	m_MapShowCornealVertex = FALSE;
	m_MapShowWavetouchLens = FALSE;
	m_MapShowPoints = FALSE;
	m_MapShowRings = FALSE;
	m_MapShowLimbus = FALSE;
	m_MapShowSolidSurface = FALSE;
	m_MapShowWireMesh = FALSE;
	m_MapShowNumbers = FALSE;
	m_MapShowKeratometry = FALSE;
	m_MapShowSimKAxes = FALSE;
	m_MapShowAstigmatismAxes = FALSE;
	m_MapTranslucent = FALSE;
	m_ShowRadialRuler = FALSE;//520
	m_ShowLinearRuler = FALSE;//520
	m_ShowInlay = FALSE;//520

	m_Cent = 0.0;
	m_Step = 1.0;
	m_Inc = 1.0;
	m_NumColors = 15;
	for (int i = 0; i < 15; i++) {
		int c = i * 255 / 14;
		m_Colors[i] = RGB(c, c, c);
	}
	m_Unit = "";

	m_l = intRound(0.100 * m_w);
	m_h_um = 13000.0;
	m_w_um = m_h_um * (m_w - m_l) / m_h;
	m_x_px_um = (m_w - m_l) / m_w_um;
	m_y_px_um = m_h / m_h_um;

	m_Scale = Scale;//521
	m_ScaleRatio = real(5000) / (real)m_Scale;
	m_x_px_um = m_x_px_um*m_ScaleRatio;//521
	m_y_px_um = m_y_px_um*m_ScaleRatio;//521

	m_cx = m_l + 0.5 * (m_w - m_l);
	m_cy = 0.5 * m_h;

	m_cursor_r_um = 0.0;
	m_cursor_a_rd = 0.0;

	m_astigm_axis = INVALID_VALUE;
}

//***************************************************************************************

void CNewEyeWnd::CreateCentButtons()
{
	RECT Rect;
	CMRgn Rgn;

	int w = intRound(0.058 * m_w); if (w < 25) w = 25;

	int W = ::GetSystemMetrics(SM_CXSCREEN);

	int l = m_l + 1;
	int b = m_h - 2 - w;
	::SetRect(&Rect, l, b - w, l + w, b);
	m_IncCentButton.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_INC_CENT);
	m_IncCentButton.SetImage(m_SizeSmall ?
		(W == 1920 ? IDR_DOWN_ARROW_3 : W == 1600 ? IDR_DOWN_ARROW_2 : IDR_DOWN_ARROW_1) :
		(W == 1920 ? IDR_DOWN_ARROW_6 : W == 1600 ? IDR_DOWN_ARROW_5 : IDR_DOWN_ARROW_4));
	m_IncCentButton.SetBk(FALSE);
	m_IncCentButton.SetBkColor(BLACK);
	m_IncCentButton.SetTip("Increase middle value");
	Rgn.SetRectRgn(&Rect);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	l += w;
	::SetRect(&Rect, l, b - w, l + w, b);
	m_DecCentButton.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_DEC_CENT);
	m_DecCentButton.SetImage(m_SizeSmall ?
		(W == 1920 ? IDR_UP_ARROW_3 : W == 1600 ? IDR_UP_ARROW_2 : IDR_UP_ARROW_1) :
		(W == 1920 ? IDR_UP_ARROW_6 : W == 1600 ? IDR_UP_ARROW_5 : IDR_UP_ARROW_4));
	m_DecCentButton.SetBk(FALSE);
	m_DecCentButton.SetBkColor(BLACK);
	m_DecCentButton.SetTip("Decrease middle value");
	Rgn.SetRectRgn(&Rect);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	m_DecCentButton.ShowWindow(SW_SHOW);
	m_IncCentButton.ShowWindow(SW_SHOW);
}

//***************************************************************************************

void CNewEyeWnd::CreateZoomButtons()
{
	RECT Rect;
	CMRgn Rgn;

	int w = intRound(0.058 * m_w); if (w < 25) w = 25;

	int W = ::GetSystemMetrics(SM_CXSCREEN);

	int l = m_l + 1;
	int b = m_h - 2;
	SetRect(&Rect, l, b - w, l + w, b);
	m_ZoomOutButton.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_ZOOM_OUT);
	m_ZoomOutButton.SetImage(m_SizeSmall ?
		(W == 1920 ? IDR_ZOOM_OUT_3 : W == 1600 ? IDR_ZOOM_OUT_2 : IDR_ZOOM_OUT_1) :
		(W == 1920 ? IDR_ZOOM_OUT_6 : W == 1600 ? IDR_ZOOM_OUT_5 : IDR_ZOOM_OUT_4));
	m_ZoomOutButton.SetBk(FALSE);
	m_ZoomOutButton.SetBkColor(BLACK);
	m_ZoomOutButton.SetTip("Increase step size");
	Rgn.SetRectRgn(&Rect);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	l += w;
	::SetRect(&Rect, l, b - w, l + w, b);
	m_ZoomInButton.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_ZOOM_IN);
	m_ZoomInButton.SetImage(m_SizeSmall ?
		(W == 1920 ? IDR_ZOOM_IN_3 : W == 1600 ? IDR_ZOOM_IN_2 : IDR_ZOOM_IN_1) :
		(W == 1920 ? IDR_ZOOM_IN_6 : W == 1600 ? IDR_ZOOM_IN_5 : IDR_ZOOM_IN_4));
	m_ZoomInButton.SetBk(FALSE);
	m_ZoomInButton.SetBkColor(BLACK);
	m_ZoomInButton.SetTip("Decrease step size");
	Rgn.SetRectRgn(&Rect);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	m_ZoomInButton.ShowWindow(SW_SHOW);
	m_ZoomOutButton.ShowWindow(SW_SHOW);
}

//***************************************************************************************

void CNewEyeWnd::RepaintMemDC()
{
	ClearMemDC();

	if (m_Method3D) {
		Surface();
	}
	else {
		Eye();
		Map();
		Pupil();
		Rings();
		Numbers();
		Points();
		RadialRuler();//520	
		LinearRuler();//520	
		Inlay();//520
		Limbus();
		Grid();
		Vertex();
		Lens();
		Keratometry();
		SimKAxes();
		AstigmatismAxes();
		Circles();
		SideLabels();
	}

	Legend();
	Labels();

	if (!m_Method3D) {

		BackupMemDC();

		CurrentPoint();
		if (m_Type != TYPE_TEYE) Cursor();
	}

	Border();
}

//***************************************************************************************

void CNewEyeWnd::SideLabels()
{
	if (m_Type != TYPE_TEYE &&
		m_Type != TYPE_TEYC &&
		m_Type != TYPE_TOPM &&
		m_Type != TYPE_TWFM &&
		m_Type != TYPE_TRFM &&
		m_Type != TYPE_CEYE &&
		m_Type != TYPE_CAXM &&
		m_Type != TYPE_CTNM &&
		m_Type != TYPE_CRFM &&
		m_Type != TYPE_CELM &&
		m_Type != TYPE_CWFM &&
		m_Type != TYPE_IWFM &&
		m_Type != TYPE_IRFM) return;

	if (m_Method3D) return;

	BOOL OD;
	if (m_pWFExam) OD = m_pWFExam->m_Header.IsOD();
	else if (m_pCTExam) OD = m_pCTExam->m_Header.IsOD();
	else return;

	COLORREF white = m_Printing ? BLACK : WHITE;

	int FontSize = intRound(0.045 * m_h);

	m_MemDC.WriteRotatedText(OD ? "temporal" : "nasal", m_l + intRound(0.5 * FontSize), intRound(0.5 * m_h), 90, FontSize, 400, "Arial", white, NOCOLOR);
	m_MemDC.WriteRotatedText(OD ? "nasal" : "temporal", m_w - intRound(0.5 * FontSize), intRound(0.5 * m_h), 90, FontSize, 400, "Arial", white, NOCOLOR);
}

//***************************************************************************************

void CNewEyeWnd::Legend()
{
	if (m_Type != TYPE_TEYE &&
		m_Type != TYPE_TEYC &&
		m_Type != TYPE_TOPM &&
		m_Type != TYPE_TWFM &&
		m_Type != TYPE_TRFM &&
		m_Type != TYPE_CEYE &&
		m_Type != TYPE_CAXM &&
		m_Type != TYPE_CTNM &&
		m_Type != TYPE_CRFM &&
		m_Type != TYPE_CELM &&
		m_Type != TYPE_CWFM &&
		m_Type != TYPE_IWFM &&
		m_Type != TYPE_IRFM) return;

	RECT Rect = { 0, 0, m_l, m_h };
	m_MemDC.DrawRectangle(Rect, m_Printing ? LIGHT_GRAY : DARK_GRAY, m_Printing ? WHITE : BLACK);

	CString f;
	switch (m_Type) {
	case TYPE_TOPM:
	case TYPE_TRFM:
	case TYPE_CAXM:
	case TYPE_CTNM:
	case TYPE_CRFM:
	case TYPE_IRFM: f = "%.2f "; break;
	case TYPE_TWFM:
	case TYPE_CELM:
	case TYPE_CWFM:
	case TYPE_IWFM: f = "%.1f "; break;
	default: return;
	}

	real max = m_Cent + m_Step * (m_NumColors >> 1);

	real dH = (real)(m_h - 4) / (m_NumColors + 1);

	CString s;

	int FontSize = intRound(0.045 * m_h);
	CMFont Font(FontSize, 400, "Arial");

	for (int i = 0; i < m_NumColors; i++) {
		SetRect(&Rect, 2, intRound(i * dH) + 2, m_l - 2, intRound((i + 1) * dH) + 1);
		m_MemDC.FillSolidRect(&Rect, m_Colors[m_NumColors - 1 - i]);

		real v = max - i * m_Step;
		if (m_Type == TYPE_TOPM || m_Type == TYPE_CAXM || m_Type == TYPE_CTNM || m_Type == TYPE_CRFM) {
			s.Format(f, v);
		}
		else {
			s.Format(f, fabs(v));
			if (v < 0.0) s = "-" + s; else if (v > 0.0) s = "+" + s;
		}
		m_MemDC.WriteText(s, Rect, Font, BLACK, 2);
	}

	Rect.top = intRound(m_NumColors * dH) + 2;
	Rect.bottom = intRound((m_NumColors + 1) * dH) + 1;
	Rect.right = m_l;
	m_MemDC.WriteText(m_Unit, Rect, Font, m_Printing ? BLACK : WHITE, 1);
}

//***************************************************************************************

void CNewEyeWnd::Map()
{
	if (m_Type != TYPE_TOPM &&
		m_Type != TYPE_TWFM &&
		m_Type != TYPE_TRFM &&
		m_Type != TYPE_CAXM &&
		m_Type != TYPE_CTNM &&
		m_Type != TYPE_CRFM &&
		m_Type != TYPE_CELM &&
		m_Type != TYPE_CWFM &&
		m_Type != TYPE_IWFM &&
		m_Type != TYPE_IRFM) return;

	if (m_Method3D) return;

	if (!m_MapShowMap) return;

	BOOL Translucent = FALSE;

	if (m_Type == TYPE_TOPM ||
		m_Type == TYPE_TWFM ||
		m_Type == TYPE_TRFM) {
		Translucent = m_pWFExam ? m_pWFExam->m_Image.m_RGBData.GetMem() != NULL : FALSE;
	}
	else if (m_Type == TYPE_CAXM ||
		m_Type == TYPE_CTNM ||
		m_Type == TYPE_CRFM ||
		m_Type == TYPE_CELM ||
		m_Type == TYPE_CWFM) {
		Translucent = m_pCTExam ? m_pCTExam->m_Image.m_RGBData.GetMem() != NULL : FALSE;
	}
	else {
		Translucent = FALSE;
	}

	Translucent = Translucent && m_MapShowEye && m_MapTranslucent;

	real min = m_Cent - m_Step * (m_NumColors >> 1);

	int cx = intRound(m_cx);
	int cy = intRound(m_cy);

	int A = Translucent ? 128 : 255;

	for (int y = -m_Map2D.m_nr; y <= m_Map2D.m_nr; y++) {
		for (int x = -m_Map2D.m_nr; x <= m_Map2D.m_nr; x++) {
			real v = m_Map2D.GetAt(y, x);
			if (v != INVALID_VALUE) {
				int i = intRound((v - min) / m_Step);
				if (i < 0) i = 0;
				else if (i > m_NumColors - 1) i = m_NumColors - 1;
				COLORREF C = m_Colors[i];
				m_MemDC.SetPixel(cx + x, cy - y, GetRValue(C), GetGValue(C), GetBValue(C), A);
			}
		}
	}
}

//***************************************************************************************

void CNewEyeWnd::Grid()
{
	if (m_Type != TYPE_TEYE &&
		m_Type != TYPE_TEYC &&
		m_Type != TYPE_TOPM &&
		m_Type != TYPE_TWFM &&
		m_Type != TYPE_TRFM &&
		m_Type != TYPE_CEYE &&
		m_Type != TYPE_CAXM &&
		m_Type != TYPE_CTNM &&
		m_Type != TYPE_CRFM &&
		m_Type != TYPE_CELM &&
		m_Type != TYPE_CWFM &&
		m_Type != TYPE_IWFM &&
		m_Type != TYPE_IRFM) return;

	if (m_Method3D) return;

	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	CString s;

	int FontSize = intRound(0.045 * m_h);

	{
		// circle
		int rx = intRound(m_Scale * m_x_px_um);
		int ry = intRound(m_Scale * m_y_px_um);
		m_MemDC.DrawEllipse(intRound(m_cx), intRound(m_cy), rx, ry, 1, gray, NOCOLOR);

		for (real um = -m_Scale; um < m_Scale + 1.0; um += 1000.0) {
			real sq = sqrt(sqr(m_Scale) - sqr(um));
			// horizontal lines
			real y = m_cy - um * m_y_px_um;
			real x = sq * m_x_px_um;
			m_MemDC.DrawHorzDottedLine(intRound(m_cx - x), intRound(m_cx + x), intRound(y), gray, NOCOLOR);
			// vertical lines
			x = m_cx - um * m_x_px_um;
			y = sq * m_y_px_um;
			m_MemDC.DrawVertDottedLine(intRound(x), intRound(m_cy - y), intRound(m_cy + y), gray, NOCOLOR);
		}

		// numbers
		for (int a = 0; a < 360; a += 5) {
			real r1_um = m_Scale + 100.0 / m_ScaleRatio;
			int x1 = intRound(m_cx + m_x_px_um * r1_um * COS[a]);
			int y1 = intRound(m_cy - m_y_px_um * r1_um * SIN[a]);
			real r2_um = (a % 15) == 0 ? m_Scale + 400.0 / m_ScaleRatio : m_Scale + 250.0 / m_ScaleRatio;
			int x2 = intRound(m_cx + m_x_px_um * r2_um * COS[a]);
			int y2 = intRound(m_cy - m_y_px_um * r2_um * SIN[a]);
			int w = intRound(((a % 15) == 0 ? 0.006 : 0.003) * m_h);
			m_MemDC.DrawLine(x1, y1, x2, y2, w, white);
			if (a % 30 == 0) {
				real r_um = m_Scale + 900.0 / m_ScaleRatio;
				int x = intRound(m_cx + m_x_px_um * r_um * COS[a]);
				int y = intRound(m_cy - m_y_px_um * r_um * SIN[a]);
				s.Format(_T(" %i°"), a);
				m_MemDC.WriteRotatedText(s, x, y, a - 90, FontSize, 400, "Arial", white, NOCOLOR);
			}
		}
	}
}

//***************************************************************************************

void CNewEyeWnd::Circles()
{
	if (m_Method3D) return;

	for (int i = 0; i < m_Circles.GetSize(); i++) {
		int x = intRound(m_cx + m_Circles[i].m_x_um * m_x_px_um);
		int y = intRound(m_cy - m_Circles[i].m_y_um * m_y_px_um);
		int rx = intRound(m_Circles[i].m_r_um * m_x_px_um);
		int ry = intRound(m_Circles[i].m_r_um * m_y_px_um);
		m_MemDC.DrawEllipse(x, y, rx, ry, 1, m_Circles[i].m_color, NOCOLOR);
	}
}

//***************************************************************************************

void CNewEyeWnd::Eye()
{
	if (m_Method3D) return;

	if (!m_MapShowEye) return;

	CEyeImage* pImage = NULL;
	real x0_um = 0.0;
	real y0_um = 0.0;

	if (m_Type == TYPE_TEYE ||
		m_Type == TYPE_TEYC ||
		m_Type == TYPE_TOPM ||
		m_Type == TYPE_TWFM ||
		m_Type == TYPE_TRFM) {
		if (m_pWFExam == NULL) return;
		if (m_Type == TYPE_TEYC) {
			pImage = &m_pWFExam->m_ColorImage;
			if (pImage->m_RGBData.GetMem() == NULL) pImage = &m_pWFExam->m_Image;
		}
		else {
			pImage = &m_pWFExam->m_Image;
		}
		x0_um = m_pWFExam->m_sc_x0_um;
		y0_um = m_pWFExam->m_sc_y0_um;
	}
	else if (m_Type == TYPE_CEYE ||
		m_Type == TYPE_CAXM ||
		m_Type == TYPE_CTNM ||
		m_Type == TYPE_CRFM ||
		m_Type == TYPE_CELM ||
		m_Type == TYPE_CWFM) {
		if (m_pCTExam == NULL) return;
		//   //[520]
		//   //pImage = &m_pCTExam->m_Image;
		//if(m_pCTExam->m_ColorImage.m_RGBData.GetMem() != NULL)
		//{
		//	pImage = &m_pCTExam->m_ColorImage;
		//	pImage->SwapImageInfo(&m_pCTExam->m_Image);
		//}
		//else   pImage = &m_pCTExam->m_Image;	
		////[520]

		pImage = &m_pCTExam->m_Image;

		x0_um = pImage->m_ve_x_um;
		y0_um = pImage->m_ve_y_um;
	}
	else return;

	if (pImage->m_RGBData.GetMem() == NULL) return;

	for (int y = 0; y < m_h; y++) {
		real y_um = y0_um + (m_cy - y) / m_y_px_um;
		for (int x = m_l; x < m_w; x++) {
			real x_um = x0_um + (x - m_cx) / m_x_px_um;
			real r, g, b;
			if (pImage->GetRGBAtUm(x_um, y_um, &r, &g, &b)) {
				m_MemDC.SetPixel(x, y, (int)r, (int)g, (int)b);
			}
		}
	}

}

//***************************************************************************************

void CNewEyeWnd::Points()
{
	if (m_Type != TYPE_TEYE) return;

	if (m_Method3D) return;

	if (!m_MapShowPoints) return;

	if (m_pWFExam == NULL) return;

	COLORREF Colors[7] = { MAGENTA, RED, ORANGE, YELLOW, GREEN, CYAN, BLUE };

	int Circle = -1;
	real prev_r_um = -1.0;
	CScanPoint* pPoint = m_pWFExam->m_Points.MoveFirst();
	while (pPoint) {
		if (pPoint->m_shot == m_pWFExam->m_CurShot) {
			if (fabs(pPoint->m_r_um - prev_r_um) > 0.001) Circle++;
			int x = intRound(m_cx + pPoint->x_um() * m_x_px_um);
			int y = intRound(m_cy - pPoint->y_um() * m_y_px_um);
			COLORREF Color = pPoint->m_Bad ? (m_Printing ? BLACK : WHITE) : Colors[Circle % 7];
			m_MemDC.DrawCircle(x, y, 2, 1, Color, Color);
			prev_r_um = pPoint->m_r_um;
		}
		pPoint = m_pWFExam->m_Points.MoveNext();
	}
}

//***************************************************************************************

void CNewEyeWnd::CurrentPoint()
{
	if (m_Type != TYPE_TEYE) return;

	if (m_Method3D) return;

	if (!m_MapShowPoints) return;

	if (m_pWFExam == NULL) return;

	if (m_pWFExam->m_CurPoint < 0 || m_pWFExam->m_CurPoint >= m_pWFExam->m_Points.GetSize()) return;

	CScanPoint* pPoint = m_pWFExam->m_Points.Get(m_pWFExam->m_CurPoint);
	int x = intRound(m_cx + pPoint->x_um() * m_x_px_um);
	int y = intRound(m_cy - pPoint->y_um() * m_y_px_um);
	m_MemDC.DrawCircle(x, y, 3, 1, m_Printing ? BLACK : WHITE, NOCOLOR);
	m_MemDC.DrawCircle(x, y, 4, 1, m_Printing ? WHITE : BLACK, NOCOLOR);
	m_MemDC.DrawCircle(x, y, 5, 1, m_Printing ? BLACK : WHITE, NOCOLOR);
}

//***************************************************************************************

void CNewEyeWnd::Pupil()
{
	if (m_Method3D) return;

	if (!m_MapShowPupil) return;

	CEyeImage* pImage = NULL;
	real x0_um = 0.0;
	real y0_um = 0.0;

	if (m_Type == TYPE_TEYE ||
		m_Type == TYPE_TOPM ||
		m_Type == TYPE_TWFM ||
		m_Type == TYPE_TRFM)
	{
		if (m_pWFExam == NULL) return;
		pImage = &m_pWFExam->m_Image;
		x0_um = m_pWFExam->m_sc_x0_um;
		y0_um = m_pWFExam->m_sc_y0_um;
	}
	else if (m_Type == TYPE_CEYE ||
		m_Type == TYPE_CAXM ||
		m_Type == TYPE_CTNM ||
		m_Type == TYPE_CRFM ||
		m_Type == TYPE_CELM ||
		m_Type == TYPE_CWFM)
	{
		if (m_pCTExam == NULL) return;

		////[520]
		//   //pImage = &m_pCTExam->m_Image;
		//if(m_pCTExam->m_ColorImage.m_RGBData.GetMem() != NULL)
		//{
		//	pImage = &m_pCTExam->m_ColorImage;
		//	pImage->SwapImageInfo(&m_pCTExam->m_Image);		
		//}
		//else   
		pImage = &m_pCTExam->m_Image;
		//[520]

		x0_um = pImage->m_ve_x_um;
		y0_um = pImage->m_ve_y_um;
	}
	else return;

	if (pImage == NULL) return;

	if (!pImage->m_pu_ok) return;

	// центр зрачка
	int x0 = intRound(m_cx + (-x0_um + pImage->m_pu_x0_um) * m_x_px_um);
	int y0 = intRound(m_cy - (-y0_um + pImage->m_pu_y0_um) * m_y_px_um);
	m_MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 3, GREEN);
	m_MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 3, GREEN);

	// граница зрачка
	for (int a = 0; a < 360; a++) {
		int b = a == 359 ? 0 : a + 1;
		if (pImage->m_pu_r_um[a] == INVALID_VALUE) continue;
		if (pImage->m_pu_r_um[b] == INVALID_VALUE) continue;
		int xa = intRound(m_cx + (-x0_um + pImage->m_pu_x0_um + pImage->m_pu_r_um[a] * COS[a]) * m_x_px_um);
		int ya = intRound(m_cy - (-y0_um + pImage->m_pu_y0_um + pImage->m_pu_r_um[a] * SIN[a]) * m_y_px_um);
		int xb = intRound(m_cx + (-x0_um + pImage->m_pu_x0_um + pImage->m_pu_r_um[b] * COS[b]) * m_x_px_um);
		int yb = intRound(m_cy - (-y0_um + pImage->m_pu_y0_um + pImage->m_pu_r_um[b] * SIN[b]) * m_y_px_um);
		m_MemDC.DrawLine(xa, ya, xb, yb, 3, GREEN);
	}
}

//***************************************************************************************

//520
void CNewEyeWnd::RadialRuler()
{
	if (m_Method3D) return;

	if (!m_ShowRadialRuler) return;

	CEyeImage* pImage = NULL;
	real x0_um = 0.0;
	real y0_um = 0.0;

	if (m_Type == TYPE_TEYE ||
		m_Type == TYPE_TOPM ||
		m_Type == TYPE_TWFM ||
		m_Type == TYPE_TRFM) {
		if (m_pWFExam == NULL) return;
		pImage = &m_pWFExam->m_Image;
		x0_um = m_pWFExam->m_sc_x0_um;
		y0_um = m_pWFExam->m_sc_y0_um;
	}
	else if (m_Type == TYPE_CEYE ||
		m_Type == TYPE_CAXM ||
		m_Type == TYPE_CTNM ||
		m_Type == TYPE_CRFM ||
		m_Type == TYPE_CELM ||
		m_Type == TYPE_CWFM) {
		if (m_pCTExam == NULL) return;

		//[520]
		//pImage = &m_pCTExam->m_Image;
		if (m_pCTExam->m_ColorImage.m_RGBData.GetMem() != NULL)
		{
			pImage = &m_pCTExam->m_ColorImage;
			pImage->SwapImageInfo(&m_pCTExam->m_Image);
			//pImage->FindVertex0
		}
		else   pImage = &m_pCTExam->m_Image;
		//[520]

		x0_um = pImage->m_ve_x_um;
		y0_um = pImage->m_ve_y_um;
	}
	else return;

	if (pImage == NULL) return;

	if (pImage->m_Ra_ok != TRUE) return;

	// центр зрачка
	int x0 = intRound(m_cx + (-x0_um + pImage->m_Ra_x_um) * m_x_px_um);
	int y0 = intRound(m_cy - (-y0_um + pImage->m_Ra_y_um) * m_y_px_um);
	m_MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 3, YELLOW);
	m_MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 3, YELLOW);

	// граница зрачка
	for (int a = 0; a < 360; a++) {
		int b = a == 359 ? 0 : a + 1;
		int xa = intRound(m_cx + (-x0_um + pImage->m_Ra_x_um + pImage->m_Ra_r_um * COS[a]) * m_x_px_um);
		int ya = intRound(m_cy - (-y0_um + pImage->m_Ra_y_um + pImage->m_Ra_r_um * SIN[a]) * m_y_px_um);
		int xb = intRound(m_cx + (-x0_um + pImage->m_Ra_x_um + pImage->m_Ra_r_um * COS[b]) * m_x_px_um);
		int yb = intRound(m_cy - (-y0_um + pImage->m_Ra_y_um + pImage->m_Ra_r_um * SIN[b]) * m_y_px_um);
		m_MemDC.DrawLine(xa, ya, xb, yb, 3, YELLOW);
	}
}
//520

//***************************************************************************************

//520
void CNewEyeWnd::LinearRuler()
{
	if (m_Method3D) return;

	if (!m_ShowLinearRuler) return;

	CEyeImage* pImage = NULL;
	real x0_um = 0.0;
	real y0_um = 0.0;

	if (m_Type == TYPE_TEYE ||
		m_Type == TYPE_TOPM ||
		m_Type == TYPE_TWFM ||
		m_Type == TYPE_TRFM) {
		if (m_pWFExam == NULL) return;
		pImage = &m_pWFExam->m_Image;
		x0_um = m_pWFExam->m_sc_x0_um;
		y0_um = m_pWFExam->m_sc_y0_um;
	}
	else if (m_Type == TYPE_CEYE ||
		m_Type == TYPE_CAXM ||
		m_Type == TYPE_CTNM ||
		m_Type == TYPE_CRFM ||
		m_Type == TYPE_CELM ||
		m_Type == TYPE_CWFM) {
		if (m_pCTExam == NULL) return;

		//[520]
		//pImage = &m_pCTExam->m_Image;
		if (m_pCTExam->m_ColorImage.m_RGBData.GetMem() != NULL)
		{
			pImage = &m_pCTExam->m_ColorImage;
			pImage->SwapImageInfo(&m_pCTExam->m_Image);
			//pImage->FindVertex0
		}
		else   pImage = &m_pCTExam->m_Image;
		//[520]

		x0_um = pImage->m_ve_x_um;
		y0_um = pImage->m_ve_y_um;
	}
	else return;

	if (pImage == NULL) return;

	if (pImage->m_LRa_ok != TRUE) return;

	//
	int x0 = intRound(m_cx + (-x0_um + pImage->m_LRa_x0_um) * m_x_px_um);
	int y0 = intRound(m_cy - (-y0_um + pImage->m_LRa_y0_um) * m_y_px_um);
	int x1 = intRound(m_cx + (-x0_um + pImage->m_LRa_x1_um) * m_x_px_um);
	int y1 = intRound(m_cy - (-y0_um + pImage->m_LRa_y1_um) * m_y_px_um);

	COLORREF LRulerColor = RGB(255, 128, 0);
	m_MemDC.DrawCircle(x0, y0, 2, 0, LRulerColor, LRulerColor);
	m_MemDC.DrawCircle(x1, y1, 2, 0, LRulerColor, LRulerColor);
	m_MemDC.DrawLine(x0, y0, x1, y1, 2, LRulerColor);
}
//520

//***************************************************************************************
//520
void CNewEyeWnd::Inlay()
{
	if (m_Method3D) return;

	if (!m_ShowInlay) return;

	CEyeImage* pImage = NULL;
	real x0_um = 0.0;
	real y0_um = 0.0;

	if (m_Type == TYPE_TEYE ||
		m_Type == TYPE_TOPM ||
		m_Type == TYPE_TWFM ||
		m_Type == TYPE_TRFM) {
		if (m_pWFExam == NULL) return;
		pImage = &m_pWFExam->m_Image;
		x0_um = m_pWFExam->m_sc_x0_um;
		y0_um = m_pWFExam->m_sc_y0_um;
	}
	else if (m_Type == TYPE_CEYE ||
		m_Type == TYPE_CAXM ||
		m_Type == TYPE_CTNM ||
		m_Type == TYPE_CRFM ||
		m_Type == TYPE_CELM ||
		m_Type == TYPE_CWFM) {
		if (m_pCTExam == NULL) return;

		//[520]
		//pImage = &m_pCTExam->m_Image;
		if (m_pCTExam->m_ColorImage.m_RGBData.GetMem() != NULL)
		{
			pImage = &m_pCTExam->m_ColorImage;
			pImage->SwapImageInfo(&m_pCTExam->m_Image);
			//pImage->FindVertex0
		}
		else   pImage = &m_pCTExam->m_Image;
		//[520]

		x0_um = pImage->m_ve_x_um;
		y0_um = pImage->m_ve_y_um;
	}
	else return;

	if (pImage == NULL) return;

	if (pImage->m_In_ok == TRUE && pImage->m_In_r_um == 0)
		pImage->m_In_ok = FALSE;

	if (!pImage->m_In_ok) return;

	//a filled circle with a center cut-out, 50% transparent white, no outline
	//an annulus design 3.8 mm outer diameter and 1.6 mm inner diamet
	int  x0 = intRound(m_cx + (-x0_um + pImage->m_In_x_um) * m_x_px_um);
	int  y0 = intRound(m_cy - (-y0_um + pImage->m_In_y_um) * m_y_px_um);

	m_MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 1, WHITE);
	m_MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 1, WHITE);

	int OutSideWidth = intRound(pImage->m_In_r_um * m_x_px_um);
	if (pImage->m_In_In_r_um  < 0) pImage->m_In_In_r_um = 0;
	int InsideWidth = intRound(pImage->m_In_In_r_um * m_x_px_um);

	int OutSideLeftX = x0 - OutSideWidth - 1;
	int OutSideRightX = x0 + OutSideWidth + 1;
	int OutSideTopY = y0 - OutSideWidth - 1;
	int OutSideBotmY = y0 + OutSideWidth + 1;


	for (int i = OutSideLeftX; i <= OutSideRightX; i++)
	{
		for (int j = OutSideTopY; j <= OutSideBotmY; j++)
		{

			real disX = (real)(i - x0);
			real disY = (real)(j - y0);

			real dis = sqrt(disX*disX + disY*disY);

			if (dis <= OutSideWidth && dis >= InsideWidth)
			{
				COLORREF C = m_MemDC.GetPixel(i, j);
				int bR = GetRValue(C);
				int bG = GetGValue(C);
				int bB = GetBValue(C);
				int Brightness = intRound(0.3*bR + 0.59*bG + 0.11*bB);

				int R = 250;
				int G = 250;
				int B = 200;

				if (Brightness > 200)
				{
					R = 220; G = 220; B = 170;
				}


				int A = 128;
				int bA = 127;
				R = (A * R + bA * bR) / 255;
				G = (A * G + bA * bG) / 255;
				B = (A * B + bA * bB) / 255;

				m_MemDC.SetPixelV(i, j, RGB(R, G, B));
			}

			//if(dis <= OutSideWidth && dis >= InsideWidth)
			//	m_MemDC.SetPixel(i, j, 250, 250, 200, 128);

		}
	}

	//Draw the dashed edge
	int tE = 0;
	for (int a = 0; a < 360; a++)
	{
		int b = a == 359 ? 0 : a + 1;

		int xa = x0 + intRound(OutSideWidth * COS[a]);
		int ya = y0 - intRound(OutSideWidth * SIN[a]);
		int xb = x0 + intRound(OutSideWidth * COS[b]);
		int yb = y0 - intRound(OutSideWidth * SIN[b]);

		int x2a = x0 + intRound(InsideWidth * COS[a]);
		int y2a = y0 - intRound(InsideWidth * SIN[a]);
		int x2b = x0 + intRound(InsideWidth * COS[b]);
		int y2b = y0 - intRound(InsideWidth * SIN[b]);

		tE++;
		if (tE == 10)
			tE = 0;
		if (tE <= 5)
		{
			m_MemDC.DrawLine(xa, ya, xb, yb, 1, BLACK);
			m_MemDC.DrawLine(x2a, y2a, x2b, y2b, 1, BLACK);
		}
		else
		{
			m_MemDC.DrawLine(xa, ya, xb, yb, 1, WHITE);
			m_MemDC.DrawLine(x2a, y2a, x2b, y2b, 1, WHITE);
		}
	}
	//Done


	//Draw four lines and txt length
	//  1
	//2   0
	//  3
	//x1,y1 are 4 points in the  disk out-edge;
	//x2,y2 are 4 points in the limbua edge
	//
	if (pImage->m_li_ok)
	{
		int x1[4], y1[4], x2[4], y2[4];

		x1[0] = OutSideRightX;
		y1[0] = y0;

		x1[1] = x0;
		y1[1] = OutSideBotmY;

		x1[2] = OutSideLeftX;
		y1[2] = y0;

		x1[3] = x0;
		y1[3] = OutSideTopY;

		//
		for (int i = 0; i < 4; i++) { x2[i] = -1000; y2[i] = -1000; }

		int minXT = 10000;
		int minXB = 10000;
		int minYR = 10000;
		int minYL = 10000;
		for (int a = 0; a < 360; a++)
		{
			int b = a == 359 ? 0 : a + 1;

			int xa = intRound(m_cx + (-x0_um + pImage->m_li_x0_um + pImage->m_li_r_mean_um * COS[a]) * m_x_px_um);
			int ya = intRound(m_cy - (-y0_um + pImage->m_li_y0_um + pImage->m_li_r_mean_um * SIN[a]) * m_y_px_um);
			int xb = intRound(m_cx + (-x0_um + pImage->m_li_x0_um + pImage->m_li_r_mean_um * COS[b]) * m_x_px_um);
			int yb = intRound(m_cy - (-y0_um + pImage->m_li_y0_um + pImage->m_li_r_mean_um * SIN[b]) * m_y_px_um);

			m_MemDC.DrawLine(xa, ya, xb, yb, 3, GRAY);

			int yDis = abs(ya - y0);
			int xDis = abs(xa - x0);

			if (yDis < minYR && ((a > 270 && a < 360) || (a >= 0 && a < 90)))
			{
				minYR = yDis;
				if (xa > x1[0])
				{
					x2[0] = xa;
					y2[0] = y0;
				}
			}

			if (yDis < minYL && a > 90 && a < 270)
			{
				minYL = yDis;
				if (xa < x1[2])
				{
					x2[2] = xa;
					y2[2] = y0;
				}
			}

			if (xDis < minXB && a > 180 && a < 360)
			{
				minXB = xDis;
				if (ya > y1[1])
				{
					x2[1] = x0;
					y2[1] = ya;
				}

			}

			if (xDis < minXT && a > 0 && a < 180)
			{
				minXT = xDis;
				if (ya < y1[3])
				{
					x2[3] = x0;
					y2[3] = ya;
				}
			}
		}

		//RECT rect1,rect2,rect3,rect4;
		RECT rect1 = { x1[0], y1[0] - 4, x2[0], y1[0] + 4 };
		RECT rect2 = { x1[1] - 4, y1[1], x1[1] + 4, y2[1] };
		RECT rect3 = { x1[2], y1[2] - 4, x2[2], y1[2] + 4 };
		RECT rect4 = { x1[3] - 4, y1[3], x1[3] + 4, y2[3] };

		if (y2[0] != -1000 && x2[0] != -1000)
			m_MemDC.DrawRectangle(rect1, 0x00505050, 0x00505050);
		if (y2[1] != -1000 && x2[1] != -1000)
			m_MemDC.DrawRectangle(rect2, 0x00505050, 0x00505050);
		if (y2[2] != -1000 && x2[2] != -1000)
			m_MemDC.DrawRectangle(rect3, 0x00505050, 0x00505050);
		if (y2[3] != -1000 && x2[3] != -1000)
			m_MemDC.DrawRectangle(rect4, 0x00505050, 0x00505050);


		//Show the length of four lines
		CString sDis1, sDis2, sDis3, sDis4;

		real xD = (real)x1[0] - (real)x2[0];
		real yD = (real)y1[0] - (real)y2[0];
		real dis1 = (sqrt(xD*xD + yD*yD) / m_x_px_um)*0.001;
		sDis1.Format(_T("%.3fmm"), dis1);

		xD = (real)x1[1] - (real)x2[1];
		yD = (real)y1[1] - (real)y2[1];
		real dis2 = (sqrt(xD*xD + yD*yD) / m_x_px_um)*0.001;
		sDis2.Format(_T("%.3fmm"), dis2);

		xD = (real)x1[2] - (real)x2[2];
		yD = (real)y1[2] - (real)y2[2];
		real dis3 = (sqrt(xD*xD + yD*yD) / m_x_px_um)*0.001;
		sDis3.Format(_T("%.3fmm"), dis3);

		xD = (real)x1[3] - (real)x2[3];
		yD = (real)y1[3] - (real)y2[3];
		real dis4 = (sqrt(xD*xD + yD*yD) / m_x_px_um)*0.001;
		sDis4.Format(_T("%.3fmm"), dis4);

		int FontSize = intRound(0.02 * m_h);
		CMFont Font(FontSize, 400, "Arial");
		RECT rect1Txt = { x1[0], y1[0] + 20, x2[0], y1[0] + 10 };
		m_MemDC.WriteText(sDis1, rect1Txt, Font, BLACK, 1, NOCOLOR);

		int yStart = y1[1] + (int)((real)(y2[1] - y1[1]) / 2.0);
		m_MemDC.WriteRotatedText(sDis2, x1[1] + 15, yStart, 90, FontSize, 300, "Arial", BLACK, NOCOLOR);

		RECT rect3Txt = { x1[2], y1[2] + 20, x2[2], y1[2] + 10 };
		m_MemDC.WriteText(sDis3, rect3Txt, Font, BLACK, 1, NOCOLOR);

		yStart = y1[3] + (int)((real)(y2[3] - y1[3]) / 2.0);
		m_MemDC.WriteRotatedText(sDis4, x1[3] + 15, yStart, 90, FontSize, 300, "Arial", BLACK, NOCOLOR);
	}
	//Done
}

//***************************************************************************************

void CNewEyeWnd::Limbus()
{
	if (m_Method3D) return;

	if (!m_MapShowLimbus) return;

	CEyeImage* pImage = NULL;
	real x0_um = 0.0;
	real y0_um = 0.0;

	if (m_Type == TYPE_TEYE ||
		m_Type == TYPE_TOPM ||
		m_Type == TYPE_TWFM ||
		m_Type == TYPE_TRFM) {
		if (m_pWFExam == NULL) return;
		pImage = &m_pWFExam->m_Image;
		x0_um = m_pWFExam->m_sc_x0_um;
		y0_um = m_pWFExam->m_sc_y0_um;
	}
	else if (m_Type == TYPE_CEYE ||
		m_Type == TYPE_CAXM ||
		m_Type == TYPE_CTNM ||
		m_Type == TYPE_CRFM ||
		m_Type == TYPE_CELM ||
		m_Type == TYPE_CWFM) {
		if (m_pCTExam == NULL) return;
		pImage = &m_pCTExam->m_Image;
		x0_um = pImage->m_ve_x_um;
		y0_um = pImage->m_ve_y_um;
	}
	else return;

	if (pImage == NULL) return;

	if (!pImage->m_li_ok) return;

	// центр лимбуса
	int x0 = intRound(m_cx + (-x0_um + pImage->m_li_x0_um) * m_x_px_um);
	int y0 = intRound(m_cy - (-y0_um + pImage->m_li_y0_um) * m_y_px_um);
	m_MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 3, BLUE);
	m_MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 3, BLUE);

	// граница лимбуса
	for (int a = 0; a < 360; a++) {
		int b = a == 359 ? 0 : a + 1;
		if (pImage->m_li_r_um[a] == INVALID_VALUE) continue;
		if (pImage->m_li_r_um[b] == INVALID_VALUE) continue;
		int xa = intRound(m_cx + (-x0_um + pImage->m_li_x0_um + pImage->m_li_r_um[a] * COS[a]) * m_x_px_um);
		int ya = intRound(m_cy - (-y0_um + pImage->m_li_y0_um + pImage->m_li_r_um[a] * SIN[a]) * m_y_px_um);
		int xb = intRound(m_cx + (-x0_um + pImage->m_li_x0_um + pImage->m_li_r_um[b] * COS[b]) * m_x_px_um);
		int yb = intRound(m_cy - (-y0_um + pImage->m_li_y0_um + pImage->m_li_r_um[b] * SIN[b]) * m_y_px_um);
		m_MemDC.DrawLine(xa, ya, xb, yb, 3, BLUE);
	}
}

//***************************************************************************************

void CNewEyeWnd::Vertex()
{
	if (m_Type != TYPE_TEYE &&
		m_Type != TYPE_TOPM &&
		m_Type != TYPE_TWFM &&
		m_Type != TYPE_TRFM) return;

	if (m_Method3D) return;

	if (!m_MapShowCornealVertex) return;

	if (m_pWFExam == NULL) return;

	CEyeImage* pImage = &m_pWFExam->m_Image;
	if (!pImage->m_ve_ok) return;

	int x0 = intRound(m_cx + (-m_pWFExam->m_sc_x0_um + pImage->m_ve_x_um) * m_x_px_um);
	int y0 = intRound(m_cy - (-m_pWFExam->m_sc_y0_um + pImage->m_ve_y_um) * m_y_px_um);
	m_MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 3, RED);
	m_MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 3, RED);
}

//***************************************************************************************

void CNewEyeWnd::Lens()
{
	if (m_Type != TYPE_TEYE) return;

	if (m_Method3D) return;

	if (!m_MapShowWavetouchLens) return;

	if (m_pWFExam == NULL) return;

	CEyeImage* pImage = &m_pWFExam->m_Image;
	if (!pImage->m_le_ok) return;

	int x[3], y[3];
	for (int i = 0; i < 3; i++) {
		x[i] = intRound(m_cx + (-m_pWFExam->m_sc_x0_um + pImage->m_le_x_um[i]) * m_x_px_um);
		y[i] = intRound(m_cy - (-m_pWFExam->m_sc_y0_um + pImage->m_le_y_um[i]) * m_y_px_um);
		m_MemDC.DrawCircle(x[i], y[i], 3, 1, YELLOW, YELLOW);
	}
	m_MemDC.DrawLine(x[0], y[0], x[2], y[2], 1, YELLOW);
	m_MemDC.DrawLine((x[0] + x[2]) >> 1, (y[0] + y[2]) >> 1, x[1], y[1], 1, YELLOW);
}

//***************************************************************************************

void CNewEyeWnd::Rings()
{
	if (m_Type != TYPE_CEYE) return;

	if (m_Method3D) return;

	if (!m_MapShowRings) return;

	if (m_pCTExam == NULL) return;

	CEyeImage* pImage = &m_pCTExam->m_Image;

	if (pImage->m_ri_ok) {

		int x0 = intRound(m_cx);
		int y0 = intRound(m_cy);
		m_MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 1, RED);
		m_MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 1, RED);

		for (int r = 0; r < pImage->m_NumRings; r++) {
			for (int a = 0; a < 360; a++) {
				int b = a == 359 ? 0 : a + 1;
				real ra_um = pImage->m_ri_r_um[r][a]; if (ra_um == INVALID_VALUE) continue;
				real rb_um = pImage->m_ri_r_um[r][b]; if (rb_um == INVALID_VALUE) continue;
				int xa = intRound(m_cx + ra_um * COS[a] * m_x_px_um);
				int ya = intRound(m_cy - ra_um * SIN[a] * m_y_px_um);
				int xb = intRound(m_cx + rb_um * COS[b] * m_x_px_um);
				int yb = intRound(m_cy - rb_um * SIN[b] * m_y_px_um);
				m_MemDC.DrawLine(xa, ya, xb, yb, 1, r % 2 ? YELLOW : RED);
			}
		}

	}
}

//***************************************************************************************

void CNewEyeWnd::Numbers()
{
	if (m_Type != TYPE_TWFM &&
		m_Type != TYPE_TRFM &&
		m_Type != TYPE_CAXM &&
		m_Type != TYPE_CTNM &&
		m_Type != TYPE_CRFM &&
		m_Type != TYPE_CELM &&
		m_Type != TYPE_CWFM &&
		m_Type != TYPE_IWFM &&
		m_Type != TYPE_IRFM) return;

	if (m_Method3D) return;

	if (!m_MapShowNumbers) return;

	real min = m_Cent - m_Step * (m_NumColors >> 1);

	int A[6];
	if (m_SizeSmall) { A[0] = 360; A[1] = 90; A[2] = 30; A[3] = 30; A[4] = 15; A[5] = 15; }
	else { A[0] = 360; A[1] = 45; A[2] = 30; A[3] = 15; A[4] = 15; A[5] = 15; }

	int FontSize = intRound(0.033 * m_h);
	CMFont Font(FontSize, 400, "Arial");

	int x0 = intRound(m_cx);
	int y0 = intRound(m_cy);

	for (int r = 0; r <= 5; r++) {
		for (int a = 0; a < 360; a += A[r]) {
			real x_um = r * 1000.0 * COS[a];
			real y_um = r * 1000.0 * SIN[a];
			int x = intRound(x_um * m_x_px_um);
			int y = intRound(y_um * m_y_px_um);
			real v = m_Map2D.GetAt(y, x);
			if (v != INVALID_VALUE) {
				CString s;
				s.Format(_T("%.2f"), v);
				x = x0 + x;
				y = y0 - y;
				RECT Rect = { x - 100, y - 100, x + 100, y + 100 };
				COLORREF Color;
				if (m_MapShowMap) {
					Color = BLACK;
				}
				else {
					int i = intRound((v - min) / m_Step);
					if (i < 0) i = 0;
					else if (i > m_NumColors - 1) i = m_NumColors - 1;
					Color = m_Colors[i];
				}
				m_MemDC.WriteText(s, Rect, Font, Color, 1);
				//m_MemDC.DrawCircle(x, y, 2, 1, BLACK, WHITE);
			}
		}
	}
}

//***************************************************************************************

void CNewEyeWnd::Keratometry()
{
	if (m_Type != TYPE_CAXM &&
		m_Type != TYPE_CSKM) return;

	if (m_Type == TYPE_CAXM && !m_MapShowKeratometry) return;

	if (m_Method3D) return;

	if (m_pCTExam == NULL) return;

	int FontSize = intRound(0.033 * m_h);

	real x_px_um = m_Type == TYPE_CAXM ? m_x_px_um : m_x_px_um * 1.2;
	real y_px_um = m_Type == TYPE_CAXM ? m_y_px_um : m_y_px_um * 1.2;
	real cx = m_Type == TYPE_CAXM ? m_cx : m_w - intRound(4000.0 * x_px_um);
	real cy = m_Type == TYPE_CAXM ? m_cy : intRound(4000.0 * y_px_um);

	COLORREF c[4] = { BLUE, BLUE, RED, RED };
	int dd = 5 * FontSize / 8;
	int r1_um[3] = { 0, 1500, 2500 };
	int r2_um[3] = { 1500, 2500, 3500 };
	for (int z = 0; z < 3; z++) {
		m_MemDC.DrawCircle(intRound(cx), intRound(cy), intRound(r2_um[z] * x_px_um), 1, GRAY, NOCOLOR);
		if (m_pCTExam->m_kr_ok[z]) {
			int a[4] = { m_pCTExam->m_kr_fl_dg[z][0], m_pCTExam->m_kr_fl_dg[z][1], m_pCTExam->m_kr_st_dg[z][0], m_pCTExam->m_kr_st_dg[z][1] };
			real d[4] = { m_pCTExam->m_kr_fl_dp[z][0], m_pCTExam->m_kr_fl_dp[z][1], m_pCTExam->m_kr_st_dp[z][0], m_pCTExam->m_kr_st_dp[z][1] };
			int x1, y1, x2, y2;
			for (int i = 0; i < 4; i++) {
				int aa = a[i];
				x1 = intRound(cx + r1_um[z] * COS[aa] * x_px_um);
				y1 = intRound(cy - r1_um[z] * SIN[aa] * y_px_um);
				x2 = intRound(cx + r2_um[z] * COS[aa] * x_px_um);
				y2 = intRound(cy - r2_um[z] * SIN[aa] * y_px_um);
				m_MemDC.DrawLine(x1, y1, x2, y2, 2, c[i]);
				CString s; s.Format(_T("%.2f"), d[i]);
				int x0 = intRound(((x1 + x2) >> 1) + dd * SIN[a[i]]);
				int y0 = intRound(((y1 + y2) >> 1) + dd * COS[a[i]]);
				if (90 < aa && aa < 180) aa += 180;
				else if (180 <= aa && aa <= 270) aa -= 180;
				m_MemDC.WriteRotatedText(s, x0, y0, aa, FontSize, 400, "Arial", c[i], NOCOLOR);
			}
		}
	}

	if (m_Type != TYPE_CSKM) return;

	FontSize = intRound(0.040 * m_h);
	CMFont Font(FontSize, 400, "Arial");

	const real dh = m_h / 24.0;

	RECT Rect;
	COLORREF white = m_Printing ? BLACK : WHITE;
	CString s;

	//---------------------------------------------------------

	int l = intRound(m_w * 0.015);
	real t = m_h - 22.0 * dh;

	SetRect(&Rect, l, intRound(t), m_w, intRound(t + dh));
	m_MemDC.WriteText("Sim K (D = 3 mm)", Rect, Font, white, 0);

	t += dh;
	if (m_pCTExam->m_sk_ok) {
		SetRect(&Rect, l, intRound(t), m_w, intRound(t + dh));
		s.Format(_T("%.2f D (%.2f mm) @ %i°"), m_pCTExam->m_sk_st_dp, 0.001 * m_pCTExam->m_sk_st_um, m_pCTExam->m_sk_st_dg);
		m_MemDC.WriteText(s, Rect, Font, RED, 0);
	}

	t += dh;
	if (m_pCTExam->m_sk_ok) {
		SetRect(&Rect, l, intRound(t), m_w, intRound(t + dh));
		s.Format(_T("%.2f D (%.2f mm) @ %i°"), m_pCTExam->m_sk_fl_dp, 0.001 * m_pCTExam->m_sk_fl_um, m_pCTExam->m_sk_fl_dg);
		m_MemDC.WriteText(s, Rect, Font, BLUE, 0);
	}

	t += dh;
	if (m_pCTExam->m_sk_ok) {
		SetRect(&Rect, l, intRound(t), m_w, intRound(t + dh));
		s.Format(_T("Delta %.2f D @ %i°"), m_pCTExam->m_sk_df_dp, m_pCTExam->m_sk_st_dg);
		m_MemDC.WriteText(s, Rect, Font, white, 0);
	}

	t += dh;
	if (m_pCTExam->m_sk_ok) {
		SetRect(&Rect, l, intRound(t), m_w, intRound(t + dh));
		s.Format(_T("Average %.2f D"), m_pCTExam->m_sk_av_dp);
		m_MemDC.WriteText(s, Rect, Font, white, 0);
	}

	//---------------------------------------------------------

	t += 1.5 * dh;
	SetRect(&Rect, l, intRound(t), m_w, intRound(t + dh));
	m_MemDC.WriteText("Central (0 <= D <= 3 mm)", Rect, Font, white, 0);

	t += dh;
	if (m_pCTExam->m_kr_ok[0]) {
		SetRect(&Rect, l, intRound(t), m_w, intRound(t + dh));
		s.Format(_T("%.2f D (%.2f mm) @ %i°"), m_pCTExam->m_kr_st_dp[0][0], 0.001 * m_pCTExam->m_kr_st_um[0][0], m_pCTExam->m_kr_st_dg[0][0]);
		m_MemDC.WriteText(s, Rect, Font, RED, 0);
	}

	t += dh;
	if (m_pCTExam->m_kr_ok[0]) {
		SetRect(&Rect, l, intRound(t), m_w, intRound(t + dh));
		s.Format(_T("%.2f D (%.2f mm) @ %i°"), m_pCTExam->m_kr_st_dp[0][1], 0.001 * m_pCTExam->m_kr_st_um[0][1], m_pCTExam->m_kr_st_dg[0][1]);
		m_MemDC.WriteText(s, Rect, Font, RED, 0);
	}

	t += dh;
	if (m_pCTExam->m_kr_ok[0]) {
		SetRect(&Rect, l, intRound(t), m_w, intRound(t + dh));
		s.Format(_T("%.2f D (%.2f mm) @ %i°"), m_pCTExam->m_kr_fl_dp[0][0], 0.001 * m_pCTExam->m_kr_fl_um[0][0], m_pCTExam->m_kr_fl_dg[0][0]);
		m_MemDC.WriteText(s, Rect, Font, BLUE, 0);
	}

	t += dh;
	if (m_pCTExam->m_kr_ok[0]) {
		SetRect(&Rect, l, intRound(t), m_w, intRound(t + dh));
		s.Format(_T("%.2f D (%.2f mm) @ %i°"), m_pCTExam->m_kr_fl_dp[0][1], 0.001 * m_pCTExam->m_kr_fl_um[0][1], m_pCTExam->m_kr_fl_dg[0][1]);
		m_MemDC.WriteText(s, Rect, Font, BLUE, 0);
	}

	//---------------------------------------------------------

	t += 1.5 * dh;
	SetRect(&Rect, l, intRound(t), m_w, intRound(t + dh));
	m_MemDC.WriteText("Mid-Periphery (3 <= D <= 5 mm)", Rect, Font, white, 0);

	t += dh;
	if (m_pCTExam->m_kr_ok[1]) {
		SetRect(&Rect, l, intRound(t), m_w, intRound(t + dh));
		s.Format(_T("%.2f D (%.2f mm) @ %i°"), m_pCTExam->m_kr_st_dp[1][0], 0.001 * m_pCTExam->m_kr_st_um[1][0], m_pCTExam->m_kr_st_dg[1][0]);
		m_MemDC.WriteText(s, Rect, Font, RED, 0);
	}

	t += dh;
	if (m_pCTExam->m_kr_ok[1]) {
		SetRect(&Rect, l, intRound(t), m_w, intRound(t + dh));
		s.Format(_T("%.2f D (%.2f mm) @ %i°"), m_pCTExam->m_kr_st_dp[1][1], 0.001 * m_pCTExam->m_kr_st_um[1][1], m_pCTExam->m_kr_st_dg[1][1]);
		m_MemDC.WriteText(s, Rect, Font, RED, 0);
	}

	t += dh;
	if (m_pCTExam->m_kr_ok[1]) {
		SetRect(&Rect, l, intRound(t), m_w, intRound(t + dh));
		s.Format(_T("%.2f D (%.2f mm) @ %i°"), m_pCTExam->m_kr_fl_dp[1][0], 0.001 * m_pCTExam->m_kr_fl_um[1][0], m_pCTExam->m_kr_fl_dg[1][0]);
		m_MemDC.WriteText(s, Rect, Font, BLUE, 0);
	}

	t += dh;
	if (m_pCTExam->m_kr_ok[1]) {
		SetRect(&Rect, l, intRound(t), m_w, intRound(t + dh));
		s.Format(_T("%.2f D (%.2f mm) @ %i°"), m_pCTExam->m_kr_fl_dp[1][1], 0.001 * m_pCTExam->m_kr_fl_um[1][1], m_pCTExam->m_kr_fl_dg[1][1]);
		m_MemDC.WriteText(s, Rect, Font, BLUE, 0);
	}

	//---------------------------------------------------------

	t += 1.5 * dh;
	SetRect(&Rect, l, intRound(t), m_w, intRound(t + dh));
	m_MemDC.WriteText("Periphery (5 <= D <= 7 mm)", Rect, Font, white, 0);

	t += dh;
	if (m_pCTExam->m_kr_ok[2]) {
		SetRect(&Rect, l, intRound(t), m_w, intRound(t + dh));
		s.Format(_T("%.2f D (%.2f mm) @ %i°"), m_pCTExam->m_kr_st_dp[2][0], 0.001 * m_pCTExam->m_kr_st_um[2][0], m_pCTExam->m_kr_st_dg[2][0]);
		m_MemDC.WriteText(s, Rect, Font, RED, 0);
	}

	t += dh;
	if (m_pCTExam->m_kr_ok[2]) {
		SetRect(&Rect, l, intRound(t), m_w, intRound(t + dh));
		s.Format(_T("%.2f D (%.2f mm) @ %i°"), m_pCTExam->m_kr_st_dp[2][1], 0.001 * m_pCTExam->m_kr_st_um[2][1], m_pCTExam->m_kr_st_dg[2][1]);
		m_MemDC.WriteText(s, Rect, Font, RED, 0);
	}

	t += dh;
	if (m_pCTExam->m_kr_ok[2]) {
		SetRect(&Rect, l, intRound(t), m_w, intRound(t + dh));
		s.Format(_T("%.2f D (%.2f mm) @ %i°"), m_pCTExam->m_kr_fl_dp[2][0], 0.001 * m_pCTExam->m_kr_fl_um[2][0], m_pCTExam->m_kr_fl_dg[2][0]);
		m_MemDC.WriteText(s, Rect, Font, BLUE, 0);
	}

	t += dh;
	if (m_pCTExam->m_kr_ok[2]) {
		SetRect(&Rect, l, intRound(t), m_w, intRound(t + dh));
		s.Format(_T("%.2f D (%.2f mm) @ %i°"), m_pCTExam->m_kr_fl_dp[2][1], 0.001 * m_pCTExam->m_kr_fl_um[2][1], m_pCTExam->m_kr_fl_dg[2][1]);
		m_MemDC.WriteText(s, Rect, Font, BLUE, 0);
	}

	//---------------------------------------------------------

	l = intRound(0.400 * m_w);
	t = m_h - 7.5 * dh;

	SetRect(&Rect, l, intRound(t), m_w, intRound(t + dh));
	m_MemDC.WriteText("Avg K", Rect, Font, white, 0);

	for (int i = 0; i <= 10; i++) {

		if (i == 6) {
			l = intRound(0.700 * m_w);
			t = m_h - 6.5 * dh;
		}
		else {
			t += dh;
		}
		if (m_pCTExam->m_zk_ok[i]) {
			SetRect(&Rect, l, intRound(t), m_w, intRound(t + dh));
			s.Format(_T("%.2f D @ D = %i mm"), m_pCTExam->m_zk_av_dp[i], i);
			m_MemDC.WriteText(s, Rect, Font, white, 0);
		}
	}

	//---------------------------------------------------------

}

//***************************************************************************************

void CNewEyeWnd::SimKAxes()
{
	if (m_Type != TYPE_CAXM) return;
	if (m_Method3D) return;
	if (!m_MapShowSimKAxes) return;
	if (!m_pCTExam) return;
	if (!m_pCTExam->m_sk_ok) return;

	int a[4];
	a[0] = m_pCTExam->m_sk_st_dg % 180; if (a[0] < 0) a[0] += 180;
	a[1] = a[0] + 180;
	a[2] = a[0] + 90; if (a[2] >= 180) a[2] -= 180;
	a[3] = a[2] + 180;

	for (int i = 0; i < 4; i++) {
		COLORREF Color = i < 2 ? RED : BLUE;
		int x1 = intRound(m_cx + 5000.0 * COS[a[i]] * m_x_px_um);
		int y1 = intRound(m_cy - 5000.0 * SIN[a[i]] * m_y_px_um);
		int x2 = intRound(m_cx - 5000.0 * COS[a[i]] * m_x_px_um);
		int y2 = intRound(m_cy + 5000.0 * SIN[a[i]] * m_y_px_um);
		m_MemDC.DrawLine(x1, y1, x2, y2, 1, Color);
	}
}

//***************************************************************************************

void CNewEyeWnd::AstigmatismAxes()
{
	if (m_Type != TYPE_TWFM &&
		m_Type != TYPE_CWFM &&
		m_Type != TYPE_IWFM &&
		m_Type != TYPE_TRFM) return;
	if (m_Method3D) return;
	if (!m_MapShowMap) return;
	if (!m_MapShowAstigmatismAxes) return;
	if (m_astigm_axis == INVALID_VALUE) return;

	int a[4];
	a[0] = m_astigm_axis % 180; if (a[0] < 0) a[0] += 180;
	a[1] = a[0] + 180;
	a[2] = a[0] + 90; if (a[2] >= 180) a[2] -= 180;
	a[3] = a[2] + 180;

	for (int i = 0; i < 4; i++) {
		COLORREF Color = i < 2 ? RED : BLUE;
		real r_um = m_Map2D.m_r_max_um;
		int x1 = intRound(m_cx + r_um * COS[a[i]] * m_x_px_um);
		int y1 = intRound(m_cy - r_um * SIN[a[i]] * m_y_px_um);
		int x2 = intRound(m_cx - r_um * COS[a[i]] * m_x_px_um);
		int y2 = intRound(m_cy + r_um * SIN[a[i]] * m_y_px_um);
		m_MemDC.DrawLine(x1, y1, x2, y2, 3, Color);
		if (i == 0 || i == 2) {
			r_um += 500.0;
			int x = intRound(m_cx + r_um * COS[a[i]] * m_x_px_um);
			int y = intRound(m_cy - r_um * SIN[a[i]] * m_y_px_um);
			CString s;
			s.Format(_T("%i°"), a[i]);
			int o = CheckAngle(a[i] - 91);
			int fs = intRound(0.045 * m_h);
			m_MemDC.WriteRotatedText(s, x, y, o, fs, 400, "Arial", Color, NOCOLOR);
		}
	}
}

//***************************************************************************************

void CNewEyeWnd::Surface()
{
	if (m_Type != TYPE_TWFM &&
		m_Type != TYPE_TRFM &&
		m_Type != TYPE_CAXM &&
		m_Type != TYPE_CTNM &&
		m_Type != TYPE_CRFM &&
		m_Type != TYPE_CELM &&
		m_Type != TYPE_CWFM &&
		m_Type != TYPE_IWFM &&
		m_Type != TYPE_IRFM) return;

	if (!m_Method3D) return;

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
	real p = (real)(m_w - m_l) / m_h;
	::glOrtho(-p, p, -1.0, 1.0, -5.0, 5.0);

	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();
	::glRotated(-45 - m_ay, 1.0, 0.0, 0.0);
	::glRotated(-45 - m_ax, 0.0, 0.0, 1.0);
	::glScaled(0.85, 0.85, 0.7);

	::glViewport(m_l, 0, m_w - m_l, m_h);

	::glClearColor((GLclampf)_0, (GLclampf)_0, (GLclampf)_0, (GLclampf)1.0);
	::glClearDepth(1.0);
	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	::glDepthFunc(GL_LEQUAL);
	::glEnable(GL_DEPTH_TEST);
	::glBlendFunc(GL_ONE, GL_ZERO);
	::glEnable(GL_BLEND);
	::glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	const real R_max_um = 5000.0;

	real z_min = m_Cent - m_Step * (m_NumColors >> 1);
	real z_max = m_Cent + m_Step * (m_NumColors >> 1);

	Matrix<real> R(m_NumColors);
	Matrix<real> G(m_NumColors);
	Matrix<real> B(m_NumColors);
	for (int i = 0; i < m_NumColors; i++) {
		R[i] = GetRValue(m_Colors[i]) / 255.0;
		G[i] = GetGValue(m_Colors[i]) / 255.0;
		B[i] = GetBValue(m_Colors[i]) / 255.0;
	}

	real k = m_MapShowSolidSurface ? 0.75 : 1.0;

	::glEnable(GL_POLYGON_OFFSET_FILL);
	::glPolygonOffset(0.0, 10000.0);

	for (int ri = 0; ri < m_Map3D.m_nr; ri++) {

		int r[4] = { ri, ri, ri + 1, ri + 1 };

		real r_um[4];
		for (int i = 0; i < 4; i++) r_um[i] = r[i] * m_Map3D.m_dr_um;

		for (int ai = 0; ai < 360; ai++) {

			int a[4];
			a[0] = ai;
			a[1] = ai + 1; if (a[1] >= 360) a[1] = 0;
			a[2] = a[1];
			a[3] = ai;

			// рассчитать координаты x[i], y[i], z[i] и цвета cr[i], cg[i], cb[i]
			real x[4], y[4], z[4], cr[4], cg[4], cb[4];
			{
				int k;
				for (k = 0; k < 4; k++) {
					real v = m_Map3D.GetAt(r[k], a[k]); if (v == INVALID_VALUE) break;
					x[k] = r_um[k] * COS[a[k]] / R_max_um;
					y[k] = r_um[k] * SIN[a[k]] / R_max_um;
					z[k] = (v - m_Cent) / (z_max - m_Cent);

					real s = (v - z_min) / m_Step;
					int j1 = (int)floor(s);
					int j2 = j1 + 1;
					if (j1 < 0) {
						cr[k] = R[0];
						cg[k] = G[0];
						cb[k] = B[0];
					}
					else if (j2 >= m_NumColors) {
						cr[k] = R[m_NumColors - 1];
						cg[k] = G[m_NumColors - 1];
						cb[k] = B[m_NumColors - 1];
					}
					else {
						real t = s - j1;
						cr[k] = R[j1] + t * (R[j2] - R[j1]);
						cg[k] = G[j1] + t * (G[j2] - G[j1]);
						cb[k] = B[j1] + t * (B[j2] - B[j1]);
					}
				}
				if (k < 4) continue;
			}

			if (m_MapShowSolidSurface) {
				::glBegin(GL_POLYGON);
				for (int i = 0; i < 4; i++) {
					::glColor3d(cr[i], cg[i], cb[i]);
					::glVertex3d(x[i], y[i], z[i]);
				}
				::glEnd();
			}

			if (m_MapShowWireMesh) {
				if (a[0] % 10 == 0) { // радиальная линия через каждые 10 градусов
					::glBegin(GL_LINES);
					int o[2] = { 0, 3 };
					for (int i = 0; i < 2; i++) {
						int j = o[i];
						::glColor3d(k * cr[j], k * cg[j], k * cb[j]);
						::glVertex3d(x[j], y[j], z[j]);
					}
					::glEnd();
				}
				if (r[3] % 5 == 0) { // азимутальная линия через каждые 250 микрометров
					::glBegin(GL_LINES);
					int o[2] = { 2, 3 };
					for (int i = 0; i < 2; i++) {
						int j = o[i];
						::glColor3d(k * cr[j], k * cg[j], k * cb[j]);
						::glVertex3d(x[j], y[j], z[j]);
					}
					::glEnd();
				}
			}

		}
	}

	::glDisable(GL_POLYGON_OFFSET_FILL);

	// X, Y, Z arrows
	::glBlendFunc(GL_ONE, GL_ZERO);
	::glColor3d(_1, _1, _1);
	::glBegin(GL_TRIANGLES);
	::glVertex3d(1.15, 0.0, 0.0); ::glVertex3d(1.13, -0.02, 0.0); ::glVertex3d(1.21, 0.0, 0.0); // X
	::glVertex3d(1.15, 0.0, 0.0); ::glVertex3d(1.13, 0.02, 0.0); ::glVertex3d(1.21, 0.0, 0.0); // X
	::glVertex3d(0.0, 1.15, 0.0); ::glVertex3d(-0.02, 1.13, 0.0); ::glVertex3d(0.0, 1.21, 0.0); // Y
	::glVertex3d(0.0, 1.15, 0.0); ::glVertex3d(0.02, 1.13, 0.0); ::glVertex3d(0.0, 1.21, 0.0); // Y
	::glVertex3d(0.0, 0.0, 1.02); ::glVertex3d(-0.02, 0.0, 1.0); ::glVertex3d(0.0, 0.0, 1.08); // Z
	::glVertex3d(0.0, 0.0, 1.02); ::glVertex3d(0.02, 0.0, 1.0); ::glVertex3d(0.0, 0.0, 1.08); // Z
	::glVertex3d(0.0, 0.0, 1.02); ::glVertex3d(0.0, -0.02, 1.0); ::glVertex3d(0.0, 0.0, 1.08); // Z
	::glVertex3d(0.0, 0.0, 1.02); ::glVertex3d(0.0, 0.02, 1.0); ::glVertex3d(0.0, 0.0, 1.08); // Z
	::glEnd();

	// Z axis
	::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	::glColor4d(0.5, 0.5, 0.5, 0.5);
	::glBegin(GL_LINES);
	::glVertex3d(0.0, 0.0, -1.0); ::glVertex3d(0.0, 0.0, 1.0);
	::glEnd();

	// circle
	::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	::glColor4d(0.5, 0.5, 0.5, 0.5);
	::glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i++) ::glVertex3d(COS[i], SIN[i], 0.0);
	::glEnd();

	// dots
	::glBlendFunc(GL_ONE, GL_ZERO);
	::glColor3d(_1, _1, _1);
	::glLineWidth(2.0);
	::glBegin(GL_LINES);
	for (int i = 0; i < 360; i += 5) {
		real x1 = 1.02 * COS[i]; real y1 = 1.02 * SIN[i];
		real x2 = 1.03 * COS[i]; real y2 = 1.03 * SIN[i];
		::glVertex3d(x1, y1, 0.0);
		::glVertex3d(x2, y2, 0.0);
	}
	::glEnd();
	::glLineWidth(1.0);

	// grid
	::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	::glColor4d(0.5, 0.5, 0.5, 0.5);
	::glBegin(GL_LINES);
	for (real um = -R_max_um; um < R_max_um + 0.001; um += 1000.0) {
		real a = um / R_max_um;
		real b = sqrt(sqr(1.0) - sqr(a));
		::glVertex3d(a, -b, 0.0); ::glVertex3d(a, b, 0.0);
		::glVertex3d(-b, a, 0.0); ::glVertex3d(b, a, 0.0);
	}
	::glEnd();

	// numbers
	int FontSize = intRound(0.045 * m_h);
	CMFont Font(FontSize, 400, "Arial");
	CFont* pFont = m_MemDC.SelectObject(&Font);

	GLYPHMETRICSFLOAT gm[10];
	::wglUseFontOutlines(m_MemDC.m_hDC, 48, 10, 1, 0.0f, 0.0f, WGL_FONT_POLYGONS, gm);
	::glListBase(1);

	real f = m_SizeSmall ? 0.09 : 0.08;
	for (int i = 0; i < 360; i += 30) {
		::glBlendFunc(GL_ONE, GL_ZERO);
		::glColor3d(_1, _1, _1);
		int a;
		if (i == 0) a = i + 1; else if (i < 100) a = i + 2; else a = i + 3;
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
		for (int j = 0; j < 2; j++) if (offs[0] == 0) { offs[0] = offs[1]; offs[1] = offs[2]; n--; }
		::glCallLists(n, GL_UNSIGNED_BYTE, offs);
		::glPopMatrix();
	}

	m_MemDC.SelectObject(pFont);


	::glFlush();

	::wglMakeCurrent(NULL, NULL);

	::wglDeleteContext(hRenderingContext);
}

//***************************************************************************************

void CNewEyeWnd::Cursor()
{
	if (m_Type != TYPE_TEYE &&
		m_Type != TYPE_TEYC &&
		m_Type != TYPE_TOPM &&
		m_Type != TYPE_TWFM &&
		m_Type != TYPE_TRFM &&
		m_Type != TYPE_CEYE &&
		m_Type != TYPE_CAXM &&
		m_Type != TYPE_CTNM &&
		m_Type != TYPE_CRFM &&
		m_Type != TYPE_CELM &&
		m_Type != TYPE_CWFM &&
		m_Type != TYPE_IWFM &&
		m_Type != TYPE_IRFM) return;

	if (m_Method3D) return;

	if (m_cursor_r_um > 5000.0) return;

	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF black = m_Printing ? WHITE : BLACK;

	CString s;
	RECT Rect;

	int FontSize = intRound(0.045 * m_h);
	CMFont Font(FontSize, 400, "Arial");

	int T = intRound(0.012 * m_h);
	int R = intRound(0.988 * m_w);
	int h = intRound(0.900 * FontSize);
	::SetRect(&Rect, 0, T, R, T + h);
	s.Format(_T("%.2f mm"), 0.001 * m_cursor_r_um);
	m_MemDC.WriteText(s, Rect, Font, white, 2);

	::SetRect(&Rect, 0, T + h, R, T + h + h);
	int a = intRound(_180_Pi * m_cursor_a_rd); if (a > 359) a -= 360;
	s.Format(_T("%i°"), a);
	m_MemDC.WriteText(s, Rect, Font, white, 2);

	real cursor_x_um = m_cursor_r_um * cos(m_cursor_a_rd);
	real cursor_y_um = m_cursor_r_um * sin(m_cursor_a_rd);

	if (m_cursor_r_um <= m_Map2D.m_r_max_um) {
		if (m_MapShowMap || m_MapShowNumbers) {
			int x = intRound(cursor_x_um * m_x_px_um);
			int y = intRound(cursor_y_um * m_y_px_um);
			real v = m_Map2D.GetAt(y, x);
			if (v != INVALID_VALUE) {
				real min = m_Cent - m_Step * (m_NumColors >> 1);
				int i = intRound((v - min) / m_Step);
				if (i < 0) i = 0;
				else if (i > m_NumColors - 1) i = m_NumColors - 1;
				::SetRect(&Rect, 0, T + h + h, R, T + h + h + h);
				s.Format(_T("%.2f %s"), v, m_Unit);
				m_MemDC.WriteText(s, Rect, Font, m_Colors[i], 2);
			}
		}
	}

	int x = intRound(m_cx + cursor_x_um * m_x_px_um);
	int y = intRound(m_cy - cursor_y_um * m_y_px_um);
	m_MemDC.DrawLine(x - 6, y - 1, x + 7, y - 1, 1, white);
	m_MemDC.DrawLine(x - 6, y, x + 7, y, 1, white);
	m_MemDC.DrawLine(x - 6, y + 1, x + 7, y + 1, 1, white);
	m_MemDC.DrawLine(x - 1, y - 6, x - 1, y + 7, 1, white);
	m_MemDC.DrawLine(x, y - 6, x, y + 7, 1, white);
	m_MemDC.DrawLine(x + 1, y - 6, x + 1, y + 7, 1, white);
	m_MemDC.DrawLine(x - 5, y, x + 6, y, 1, black);
	m_MemDC.DrawLine(x, y - 5, x, y + 6, 1, black);
}

//***************************************************************************************

void CNewEyeWnd::OnMouseMove(uint nFlags, CPoint Point)
{
	CDispWnd::OnMouseMove(nFlags, Point);

	if ((nFlags & MK_LBUTTON) && !m_Method3D) {

		real x_um = (Point.x - m_cx) / m_x_px_um;
		real y_um = (m_cy - Point.y) / m_y_px_um;

		m_cursor_r_um = hyp(x_um, y_um);
		m_cursor_a_rd = angle(y_um, x_um);

		RestoreMemDC();

		CurrentPoint();
		Cursor();
		Border();

		InvalidateRgn(&m_Rgn, FALSE);

		static real m[2];
		m[0] = m_cursor_r_um;
		m[1] = m_cursor_a_rd;
		GetParent()->PostMessage(WM_CHANGE_POS, (WPARAM)this, (LPARAM)m);
	}
}

//***************************************************************************************

void CNewEyeWnd::OnLButtonDown(uint nFlags, CPoint Point)
{
	CDispWnd::OnLButtonDown(nFlags, Point);

	OnMouseMove(nFlags, Point);
}

//***************************************************************************************

void CNewEyeWnd::OnLButtonDblClk(uint nFlags, CPoint Point)
{
	if (m_MapShowMap && !m_Method3D) {
		real mean, std_dev;
		m_Map2D.ComputeMeanStdDev(mean, std_dev);
		CString s;
		s.Format(_T("Mean = %.2f\nStandard deviation = %.2f"), mean, std_dev);
		Info(s);
	}

	CDispWnd::OnLButtonDblClk(nFlags, Point);
}

//***************************************************************************************

LRESULT CNewEyeWnd::OnChangePosMsg(WPARAM wParam, LPARAM lParam)
{
	if (!m_Method3D) {

		m_cursor_r_um = ((real*)lParam)[0];
		m_cursor_a_rd = ((real*)lParam)[1];

		RestoreMemDC();

		CurrentPoint();
		Cursor();
		Border();

		InvalidateRgn(&m_Rgn, FALSE);
	}

	return 0;
}

//***************************************************************************************

void CNewEyeWnd::ChangePoint()
{
	RestoreMemDC();

	CurrentPoint();
	Cursor();
	Border();

	InvalidateRgn(&m_Rgn, FALSE);
}

//***************************************************************************************

void CNewEyeWnd::OnIncCentButtonClicked()
{
	m_Cent += m_Inc;

	RepaintMemDC();

	InvalidateRgn(&m_Rgn, FALSE);

	GetParent()->PostMessage(WM_CHANGE_CENT, (WPARAM)this, (LPARAM)((float)m_Cent));
}

//***************************************************************************************

void CNewEyeWnd::OnDecCentButtonClicked()
{
	m_Cent -= m_Inc;

	RepaintMemDC();

	InvalidateRgn(&m_Rgn, FALSE);

	GetParent()->PostMessage(WM_CHANGE_CENT, (WPARAM)this, (LPARAM)((float)m_Cent));
}

//***************************************************************************************

void CNewEyeWnd::OnZoomOutButtonClicked()
{
	m_Step += m_Inc;

	RepaintMemDC();

	InvalidateRgn(&m_Rgn, FALSE);

	GetParent()->PostMessage(WM_CHANGE_STEP, (WPARAM)this, (LPARAM)((float)m_Step));
}

//***************************************************************************************

void CNewEyeWnd::OnZoomInButtonClicked()
{
	if (fabs(m_Step - m_Inc) < 0.000001) return;

	m_Step -= m_Inc;

	RepaintMemDC();

	InvalidateRgn(&m_Rgn, FALSE);

	GetParent()->PostMessage(WM_CHANGE_STEP, (WPARAM)this, (LPARAM)((float)m_Step));
}

//***************************************************************************************