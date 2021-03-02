//***************************************************************************************

#include "StdAfx.h"
#include "MGdi.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

void WriteText(HDC hDC, CString Text, const RECT& Rect, const CFont& Font, COLORREF Color, int Centration, COLORREF BkColor)
{
	::SetTextColor(hDC, Color);

	if (BkColor == NOCOLOR)
	{
		::SetBkMode(hDC, TRANSPARENT);
	}
	else
	{
		::SetBkMode(hDC, OPAQUE);
		::SetBkColor(hDC, BkColor);
	}

	uint Format = DT_SINGLELINE | DT_VCENTER;
	switch (Centration)
	{
	case 0: Format |= DT_LEFT;   break;
	case 1: Format |= DT_CENTER; break;
	case 2: Format |= DT_RIGHT;  break;
	}

	HFONT hFont = (HFONT)::SelectObject(hDC, Font.m_hObject);

	::DrawText(hDC, (LPCWSTR)Text, lstrlenW(Text), (RECT*)&Rect, Format);

	::SelectObject(hDC, hFont);
}

//***************************************************************************************

void CMFont::Create(const int Height, const int Width, const char* FaceName)
{
	DeleteObject();

	m_Height = Height;

	CreateFont(Height, 0, 0, 0, Width, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, (LPCTSTR)FaceName);
}

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

CMDC::~CMDC()
{
  if (m_bitmap) {
    ::SelectObject(m_hDC, m_oldBitmap);
    ::DeleteObject(m_bitmap);
  }
}

void CMDC::CreateTrueColorDC(const int w, const int h)
{
	m_w = w;
	m_h = h;

	CreateCompatibleDC(NULL);

	SetMapMode(MM_TEXT);
	SetROP2(R2_COPYPEN);
	SetStretchBltMode(COLORONCOLOR);
	SetBkMode(TRANSPARENT);

	BITMAPINFOHEADER bih;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = w;
	bih.biHeight = h;
	bih.biPlanes = 1;
	bih.biBitCount = 24;
	bih.biCompression = BI_RGB;
	bih.biSizeImage = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;

	if (m_bitmap) {
    ::SelectObject(m_hDC, m_oldBitmap);
    ::DeleteObject(m_bitmap);
  }

	m_bitmap = ::CreateDIBSection(NULL, (BITMAPINFO*)&bih, DIB_RGB_COLORS, reinterpret_cast<void **>(&m_RGBData), NULL, 0);
  m_oldBitmap = (HBITMAP)::SelectObject(m_hDC, m_bitmap);
}

//***************************************************************************************

void CMDC::SetPixel(int x, int y, int R, int G, int B, int A)
{
	if (A < 255)
	{
		COLORREF C = GetPixel(x, y);
		int bR = GetRValue(C);
		int bG = GetGValue(C);
		int bB = GetBValue(C);
		int bA = 255 - A;
		R = (A * R + bA * bR) / 255;
		G = (A * G + bA * bG) / 255;
		B = (A * B + bA * bB) / 255;
	}
	SetPixelV(x, y, RGB(R, G, B));
}

//***************************************************************************************

void CMDC::DrawLine(int x1, int y1, int x2, int y2, int w, COLORREF c)
{
	CPen Pen(PS_SOLID, w, c);

	CPen* pPen = SelectObject(&Pen);

	MoveTo(x1, y1);
	LineTo(x2, y2);

	SelectObject(pPen);
}

//***************************************************************************************

void CMDC::DrawDottedLine(int x1, int y1, int x2, int y2, COLORREF c)
{
	CPen Pen(PS_DOT, 1, c);

	CPen* pPen = SelectObject(&Pen);

	MoveTo(x1, y1);
	LineTo(x2, y2);

	SelectObject(pPen);
}

//***************************************************************************************

void CMDC::DrawHorzDottedLine(int x1, int x2, int y, COLORREF c1, COLORREF c2)
{
	int xmin = __min(x1, x2);
	int xmax = __max(x1, x2);

	if (c1 != NOCOLOR)
	{
		for (int x = xmin; x <= xmax; x += 2)
		{
			SetPixel(x, y, GetRValue(c1), GetGValue(c1), GetBValue(c1));
		}
	}

	if (c2 != NOCOLOR)
	{
		for (int x = xmin + 1; x <= xmax; x += 2)
		{
			SetPixel(x, y, GetRValue(c2), GetGValue(c2), GetBValue(c2));
		}
	}
}

//***************************************************************************************

void CMDC::DrawVertDottedLine(int x, int y1, int y2, COLORREF c1, COLORREF c2)
{
	int ymin = __min(y1, y2);
	int ymax = __max(y1, y2);

	if (c1 != NOCOLOR)
	{
		for (int y = ymin; y <= ymax; y += 2)
		{
			SetPixel(x, y, GetRValue(c1), GetGValue(c1), GetBValue(c1));
		}
	}

	if (c2 != NOCOLOR)
	{
		for (int y = ymin + 1; y <= ymax; y += 2)
		{
			SetPixel(x, y, GetRValue(c2), GetGValue(c2), GetBValue(c2));
		}
	}
}

//***************************************************************************************

void CMDC::DrawRectangle(const RECT& Rect, COLORREF PenColor, COLORREF BrushColor)
{
	CPen Pen(PS_SOLID, 1, PenColor);
	CBrush Brush(BrushColor);

	CPen* pPen = PenColor == NOCOLOR ? (CPen*)SelectStockObject(NULL_PEN) : SelectObject(&Pen);
	CBrush* pBrush = BrushColor == NOCOLOR ? (CBrush*)SelectStockObject(NULL_BRUSH) : SelectObject(&Brush);

	Rectangle(&Rect);

	SelectObject(pPen);
	SelectObject(pBrush);
}

//***************************************************************************************

void CMDC::DrawCircle(int x, int y, int r, int w, COLORREF PenColor, COLORREF BrushColor)
{
	CPen Pen(PS_SOLID, w, PenColor);
	CBrush Brush(BrushColor);

	CPen* pPen = SelectObject(&Pen);
	CBrush* pBrush = BrushColor == NOCOLOR ? (CBrush*)SelectStockObject(NULL_BRUSH) : SelectObject(&Brush);

	Ellipse(x - r, y - r, x + r + 1, y + r + 1);

	SelectObject(pPen);
	SelectObject(pBrush);
}

//***************************************************************************************

void CMDC::DrawEllipse(int x, int y, int rx, int ry, int w, COLORREF PenColor, COLORREF BrushColor)
{
	CPen Pen(PS_SOLID, w, PenColor);
	CBrush Brush(BrushColor);

	CPen* pPen = PenColor == NOCOLOR ? (CPen*)SelectStockObject(NULL_PEN) : SelectObject(&Pen);
	CBrush* pBrush = BrushColor == NOCOLOR ? (CBrush*)SelectStockObject(NULL_BRUSH) : SelectObject(&Brush);

	Ellipse(x - rx, y - ry, x + rx + 1, y + ry + 1);

	SelectObject(pPen);
	SelectObject(pBrush);
}

//***************************************************************************************

void CMDC::DrawArc(int x, int y, int r, int w, int a1, int a2, COLORREF PenColor)
{
	a1 = CheckAngle(a1);
	a2 = CheckAngle(a2);
	if (a1 == a2) return;

	CPen Pen(PS_SOLID, w, PenColor);
	CPen* pPen = SelectObject(&Pen);

	Arc(x - r, y - r, x + r + 1, y + r + 1, intRound(x + r * COS[a1]), intRound(y - r * SIN[a1]), intRound(x + r * COS[a2]), intRound(y - r * SIN[a2]));

	SelectObject(pPen);
}

//***************************************************************************************

void CMDC::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int w, const COLORREF PenColor, COLORREF BrushColor)
{
	CPen Pen(PS_SOLID, w, PenColor);
	CBrush Brush(BrushColor);

	CPen* pPen = PenColor == NOCOLOR ? (CPen*)SelectStockObject(NULL_PEN) : SelectObject(&Pen);
	CBrush* pBrush = BrushColor == NOCOLOR ? (CBrush*)SelectStockObject(NULL_BRUSH) : SelectObject(&Brush);

	POINT lpPoints[3];

	lpPoints[0].x = x1;
	lpPoints[0].y = y1;

	lpPoints[1].x = x2;
	lpPoints[1].y = y2;

	lpPoints[2].x = x3;
	lpPoints[2].y = y3;

	Polygon(lpPoints, 3);

	SelectObject(pPen);
	SelectObject(pBrush);
}

//***************************************************************************************

void CMDC::WriteText(const CString& Text, const RECT& Rect, const CFont& Font, COLORREF Color, int Centration, COLORREF BkColor)
{
	SetTextColor(Color);

	if (BkColor == NOCOLOR)
	{
		SetBkMode(TRANSPARENT);
	}
	else
	{
		SetBkMode(OPAQUE);
		SetBkColor(BkColor);
	}

	uint Format = DT_SINGLELINE | DT_VCENTER;
	switch (Centration)
	{
	case 0: Format |= DT_LEFT;   break;
	case 1: Format |= DT_CENTER; break;
	case 2: Format |= DT_RIGHT;  break;
	}

	CFont *pFont = SelectObject((CFont*)&Font);
	
	int len = Text.GetLength();
	DrawText(Text, len, (RECT*)&Rect, Format);

	SelectObject(pFont);
}

//***************************************************************************************

void CMDC::WriteRotatedText(const CString& Text, int x, int y, int a, int FontSize, int FontWidth, const CString& FontFace, COLORREF Color, COLORREF BkColor)
{
	a = CheckAngle(a);

	RECT Rect;

	G_As = FontFace;
	CMFont Font1(FontSize, FontWidth, G_As);

	MeasureRect(Text, Rect, Font1);

	real_t dx = 0.5 * (Rect.right - Rect.left);
	real_t dy = 0.5 * (Rect.bottom - Rect.top);
	real_t c = angle(dy, dx);
	real_t b = a * _Pi_180 - c;
	real_t d = hyp(dx, dy);
	x -= intRound(d * cos(b));
	y += intRound(d * sin(b));

	CFont Font2;

	Font2.CreateFont(FontSize, 0, a * 10, a * 10, FontWidth, 0, 0, 0,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, FontFace);

	CFont* pFont = SelectObject(&Font2);

	SetTextAlign(TA_LEFT | TA_TOP | TA_NOUPDATECP);

	SetTextColor(Color);

	if (BkColor == NOCOLOR)
	{
		SetBkMode(TRANSPARENT);
	}
	else
	{
		SetBkMode(OPAQUE);
		SetBkColor(BkColor);
	}

	CStringA AText(Text);
	TextOut(x, y, Text, strlen(AText));

	SelectObject(pFont);
}

//***************************************************************************************

void CMDC::MeasureRect(const CString& Text, RECT& Rect, const CFont& Font)
{
	CFont* pFont = SelectObject((CFont*)&Font);

	DrawText(Text, &Rect, DT_CALCRECT);

	SelectObject(pFont);
}

//***************************************************************************************

void CMDC::DrawImage(const CBasicImage& Image, int x, int y, int cx, int cy, BOOL Enabled)
{
	int l;

	if (cx == 0) l = x;
	else if (cx == 1) l = x - (Image.m_w >> 1);
	else l = x - Image.m_w;

	int t;
	if (cy == 0) t = y;
	else if (cy == 1) t = y - (Image.m_h >> 1);
	else t = y - Image.m_h;

	for (int j = 0; j < Image.m_h; j++)
	{
		for (int i = 0; i < Image.m_w; i++)
		{
			int R = Image.GetRAt(i, j);
			int G = Image.GetGAt(i, j);
			int B = Image.GetBAt(i, j);

			if (!Enabled)
			{
				R = G = B = (R + G + B + 255 + 255 + 255) / 6;
			}

			int A = 255;
			if (Image.m_AData.GetMem())
			{
				A = Image.GetAAt(i, j);
			}

			SetPixel(l + i, t + (Image.m_h - 1 - j), R, G, B, A);
		}
	}
}

//***************************************************************************************
