//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Image.h"

//***************************************************************************************

void WriteText(HDC hDC, CString Text, const RECT& Rect, const CFont& Font, COLORREF Color, int Centration, COLORREF BkColor);

//***************************************************************************************

class CMFont : public CFont
{
public:

	CMFont()
	{
		m_Height = 0;
	}

	CMFont(const int Height, const int Width, const char* FaceName)
	{
		Create(Height, Width, FaceName);
	}

	int m_Height;

	void Create(const int Height, const int Width, const char* FaceName);
};

//***************************************************************************************

class CMRgn : public CRgn
{
public:

	CMRgn() { CreateRectRgn(0, 0, 1, 1); }
	~CMRgn() { DeleteObject(); }
};

//***************************************************************************************

class CMDC : public CDC
{
public:

	int m_w;
	int m_h;
	uchar* m_RGBData;

	void CreateTrueColorDC(int w, int h);

	void SetPixel(int x, int y, int R, int G, int B, int A = 255);
	void DrawLine(int x1, int y1, int x2, int y2, int w, COLORREF c);
	void DrawDottedLine(int x1, int y1, int x2, int y2, COLORREF c);
	void DrawHorzDottedLine(int x1, int x2, int y, COLORREF c1, COLORREF c2);
	void DrawVertDottedLine(int x, int y1, int y2, COLORREF c1, COLORREF c2);
	void DrawRectangle(const RECT& Rect, COLORREF PenColor, COLORREF BrushColor);
	void DrawCircle(int x, int y, int r, int w, COLORREF PenColor, COLORREF BrushColor);
	void DrawEllipse(int x, int y, int rx, int ry, int w, COLORREF PenColor, COLORREF BrushColor);
	void DrawArc(int x, int y, int r, int w, int a1, int a2, const COLORREF PenColor);
	void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int w, const COLORREF PenColor, COLORREF BrushColor);//531
	void WriteText(const CString& Text, const RECT& Rect, const CFont& Font, COLORREF Color, int Centration, COLORREF BkColor = NOCOLOR);
	void WriteRotatedText(const CString& Text, int x, int y, int a, int FontSize, int FontWidth, const CString& FontFace, COLORREF Color, COLORREF BkColor);
	void MeasureRect(const CString& Text, RECT& Rect, const CFont& Font);

	void DrawImage(const CBasicImage& Image, int x, int y, int cx, int cy, BOOL Enabled);
};

//***************************************************************************************
