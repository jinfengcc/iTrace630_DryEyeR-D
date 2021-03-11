/*******************************************************************
Autor:         Jinfeng Chen
E-mail:        jinfengcc@gmail.com
Version:       6.2.0
Date:          04-25-2017
Description:   Generate a new Horizontal slider control component which used for the WF Near and Far vision control
*****************************************************************************************/

#include "StdAfx.h"

#include "HorSliderCtrl.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CHorSliderCtrl, CWnd)

	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()

END_MESSAGE_MAP()

//***************************************************************************************

CHorSliderCtrl::CHorSliderCtrl()
{
	m_Draw = FALSE;
	m_Mousemove = FALSE;
}

//***************************************************************************************

CHorSliderCtrl::~CHorSliderCtrl()
{
	delete m_MemBackup;
	m_MemBackup = NULL;
}

//***************************************************************************************

void CHorSliderCtrl::SetParameters(HSliderParameters Pars)
{
	m_Pars.Pos = Pars.Pos;
	m_Pars.scaleNum = Pars.scaleNum;
	m_Pars.WideRatio = Pars.WideRatio;
	m_Pars.ScaleType = Pars.ScaleType;

	m_halfScale = 0.5 / (real_t)m_Pars.scaleNum;

	for (int i = 0; i <= m_Pars.scaleNum; i++)
	{
		m_Pars.ScaleLength[i] = Pars.ScaleLength[i];
		m_Pars.UpLabels[i] = Pars.UpLabels[i];
		m_Pars.DownLabels[i] = Pars.DownLabels[i];
	}

	m_Pars.Title = Pars.Title;

	//m_Pars.SliderRect = Pars.SliderRect;

	m_SliderWidth = -1.0;

	for (int i = 0; i <= m_Pars.scaleNum; i++)
	{
		m_Pars.OutlineHighlight[i] = Pars.OutlineHighlight[i];
	}

	m_Draw = FALSE;

}

//***************************************************************************************

BOOL CHorSliderCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = ::AfxRegisterWndClass(0, ::LoadCursor(NULL, IDC_HAND), 0, 0);
	cs.style = cs.style | WS_CHILD;
	cs.dwExStyle = 0;
	return CWnd::PreCreateWindow(cs);
}

//***************************************************************************************

void CHorSliderCtrl::OnPaint()
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF black = m_Printing ? WHITE : BLACK;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	CMDC DC;

	RECT Rect, TitleRect, UpRect, DownRect;//UpRect and DownRect are used to label the scale
	GetClientRect(&Rect);

	int m_w = Rect.right - Rect.left;
	int m_h = Rect.bottom - Rect.top;

	DC.CreateTrueColorDC(m_w, m_h);
	m_RGBSize = IMAGE_SIZE(m_w, m_h);

	int h = (Rect.bottom - Rect.top) * m_Pars.WideRatio;
	int y0 = Rect.bottom - (Rect.bottom - Rect.top) / 3;//The position of the splider center line

	if (!m_Draw)
	{
		m_MemBackup = new uchar[m_RGBSize];

		DC.DrawRectangle(Rect, gray, NOCOLOR);


		//Highlight the outline
		if (m_Pars.OutlineHighlight[0] == 1)
		{
			DC.DrawLine(Rect.left, Rect.top, Rect.left, Rect.bottom, 1, white);
		}
		if (m_Pars.OutlineHighlight[1] == 1)
		{
			DC.DrawLine(Rect.left, Rect.top, Rect.right, Rect.top, 1, white);
		}
		if (m_Pars.OutlineHighlight[2] == 1)
		{
			DC.DrawLine(Rect.right - 1, Rect.top, Rect.right - 1, Rect.bottom, 1, white);
		}
		if (m_Pars.OutlineHighlight[3] == 1)
		{
			DC.DrawLine(Rect.left, Rect.bottom, Rect.right, Rect.bottom, 1, white);
		}
		//Highlight the outline

		int d = h / 2;

		int SmallFontSize = intRound(0.25* h);
		CMFont SmallFont(SmallFontSize, 400, "Arial");
		CMFont MediumFont(intRound(0.5* h), 400, "Arial");

		m_Left = Rect.left + m_w*0.15;
		m_Right = Rect.right - m_w*0.15;

		int w = m_Right - m_Left;

		int x0 = m_Left + intRound((1 - m_Pars.Pos) * w);

		if (x0 > m_Right) x0 = m_Right;

		//Draw the scales
		int xPos;
		int yPos1, yPos2;
		int ScaleFontSize = 1;
		for (int i = 0; i <= m_Pars.scaleNum; i++)
		{
			int  xPos = intRound(m_Left + real_t(i) * real_t(m_Right - m_Left) / m_Pars.scaleNum);

			if (i == m_Pars.scaleNum)
			{
				xPos = m_Right;
			}

			if (m_Pars.ScaleType == 0)
			{
				if (m_Pars.ScaleLength[i] == 0)
				{
					yPos1 = y0 - 0.7*h;
					yPos2 = y0;
					ScaleFontSize = 1;
				}
				else if (m_Pars.ScaleLength[i] == 1)
				{
					yPos1 = y0 - h;
					yPos2 = y0;
					ScaleFontSize = 2;
				}
			}
			else if (m_Pars.ScaleType == 1)
			{
				if (m_Pars.ScaleLength[i] == 0)
				{
					yPos1 = y0;
					yPos2 = y0 + 0.7*h;
					ScaleFontSize = 1;
				}
				else if (m_Pars.ScaleLength[i] == 1)
				{
					yPos1 = y0;
					yPos2 = y0 + h;
					ScaleFontSize = 2;
				}
			}
			else if (m_Pars.ScaleType == 2)
			{
				if (m_Pars.ScaleLength[i] == 0)
				{
					yPos1 = y0 - 0.35*h;
					yPos2 = y0 + 0.35*h;
					ScaleFontSize = 1;
				}
				else if (m_Pars.ScaleLength[i] == 1)
				{
					yPos1 = y0 - 0.5 * h;
					yPos2 = y0 + 0.5 * h;
					ScaleFontSize = 2;
				}
			}

			DC.DrawLine(xPos, yPos1, xPos, yPos2, ScaleFontSize, white);

			yPos1 = y0 - 0.5 * h;
			yPos2 = y0 + 0.5 * h;

			if (i == 0)
			{
				::SetRect(&UpRect, xPos - 40, yPos1 - 5 - SmallFontSize, xPos + 40, yPos1 - 5);
				DC.WriteText(m_Pars.UpLabels[i], UpRect, MediumFont, white, 1, NOCOLOR);
			}
			else
			{
				::SetRect(&UpRect, xPos - 40, yPos1 - 5 - SmallFontSize, xPos + 40, yPos1 - 5);
				DC.WriteText(m_Pars.UpLabels[i], UpRect, SmallFont, white, 1, NOCOLOR);
			}

			::SetRect(&DownRect, xPos - 40, yPos2 + 5, xPos + 40, yPos2 + 5 + SmallFontSize);
			DC.WriteText(m_Pars.DownLabels[i], DownRect, SmallFont, white, 1, NOCOLOR);
		}

		m_HalfWidth = m_halfScale*w;

		m_Draw = TRUE;

		memcpy(m_MemBackup, DC.m_RGBData, m_RGBSize);
	}
	//Done
	else
	{
		memcpy(DC.m_RGBData, m_MemBackup, m_RGBSize);
	}

	{
		//----------------------------------------------------
		//Show the title
		int TitleY0 = Rect.top + (Rect.bottom - Rect.top) / 4;
		int TitleY1 = y0 - h - 15;
		::SetRect(&TitleRect, m_Left, TitleY0, m_Right, TitleY1);

		CMFont BigFont(TitleY1 - TitleY0, 400, "Arial");

		DC.WriteText(m_Pars.Title, TitleRect, BigFont, white, 1, NOCOLOR);
		//----------------------------------------------------


		//----------------------------------------------------
		//Draw the selected point
		int h = (Rect.bottom - Rect.top) * m_Pars.WideRatio;
		int d = h / 2;

		int y0 = Rect.bottom - (Rect.bottom - Rect.top) / 3;//The position of the splider center line

		int x1 = Rect.left + m_w * 0.15;
		int x2 = Rect.right - m_w * 0.15;

		int w = x2 - x1;

		int x0 = x1 + intRound((1 - m_Pars.Pos) * w);

		if (m_Pars.SliderRect.right < 0 || m_Pars.SliderRect.left  < 0 || m_Pars.SliderRect.right > 10000 || m_Pars.SliderRect.left > 10000)
		{
			::SetRect(&m_Pars.SliderRect, x1, y0 - 0.5 * h, x2, y0 + 0.5 * h);

			m_SliderWidth = m_Pars.SliderRect.right - m_Pars.SliderRect.left;
		}

		DC.DrawLine(x1, y0, x2, y0, 2, white);

		DC.DrawCircle(x0, y0, intRound(0.2*d), 1, YELLOW, YELLOW);

		m_LastDownX = x0;
		//----------------------------------------------------
	}

	CPaintDC PaintDC(this);
	PaintDC.BitBlt(0, 0, m_w, m_h, &DC, 0, 0, SRCCOPY);
}

//***************************************************************************************

void CHorSliderCtrl::OnLButtonDown(uint nFlags, CPoint Point)
{
	m_DownX = Point.x;
	m_DownY = Point.y;

	//CString s;
	//s.Format(_T("%i"), m_DownX);
	//::Info(s);

	OnMouseMove(nFlags, Point);
}

//***************************************************************************************

void CHorSliderCtrl::OnLButtonUp(uint nFlags, CPoint Point)
{
	//::Info("1");

	if (m_Mousemove)
	{
		m_Pars.Pos = 1 - (real_t)(Point.x - m_Pars.SliderRect.left) / m_SliderWidth;

		real_t D = m_Pars.Pos*0.25;

		m_Pars.Title.Format(_T("NEAR POINT   %.2fD"), (1 - m_Pars.Pos) * 3);

		Invalidate(FALSE);

		GetParent()->PostMessage(WM_SLIDER_LCLICK, (WPARAM)this, 0);

		m_Mousemove = FALSE;

		return;
	}

	//Following is the mouse click
	real_t xPower = (real_t)Point.x - (real_t)m_DownX;
	xPower *= xPower;

	real_t yPower = (real_t)Point.y - (real_t)m_DownY;
	yPower *= yPower;
	real_t dis = sqrt(xPower + yPower);

	if (dis >= 0.001) return;

	/*CString s;
	s.Format(_T("dis %i"), dis);
	::Info(s);

	s.Format(_T("left %i"), m_Pars.SliderRect.left);
	::Info(s);

	s.Format(_T("right %i"), m_Pars.SliderRect.right);
	::Info(s);*/

	//If it is click , Selection will snap to closet scale point
	Point.y -= 10;

	if ((m_Pars.SliderRect.left <= Point.x && Point.x <= m_Pars.SliderRect.right) &&
		(m_Pars.SliderRect.top <= Point.y && Point.y <= m_Pars.SliderRect.bottom)
		)
	{
		real_t SelxPos = 1 - (real_t)(Point.x - m_Pars.SliderRect.left) / m_SliderWidth;

		for (int i = 0; i <= m_Pars.scaleNum; i++)
		{
			real_t  ScalePos = (real_t)(i) / (real_t)m_Pars.scaleNum;

			if (SelxPos - ScalePos <= m_halfScale)
			{
				m_Pars.Pos = ScalePos;
				break;
			}
		}

		m_Pars.Title.Format(_T("NEAR POINT   %.2fD"), (1 - m_Pars.Pos) * 3);

		//Invalidate(FALSE);

		GetParent()->PostMessage(WM_SLIDER_LCLICK, (WPARAM)this, 0);
	}
}

//***************************************************************************************

void CHorSliderCtrl::OnMouseMove(uint nFlags, CPoint Point)
{
	if (nFlags & MK_LBUTTON)
	{
		int xPower = abs(Point.x - m_LastDownX);

		/*CString s;
		s.Format(_T("xPower %i"), xPower);
		::Info(s);

		s.Format(_T("right %i"), m_Pars.SliderRect.right);
		::Info(s);

		s.Format(_T("left %i"), m_Pars.SliderRect.left);
		::Info(s);*/

		if (xPower >= m_HalfWidth)
		{
			m_Mousemove = FALSE;
			return;
		}

		int w = m_Pars.SliderRect.right - m_Pars.SliderRect.left;

		Point.y -= 10;

		if ((m_Pars.SliderRect.left <= Point.x && Point.x <= m_Pars.SliderRect.right) &&
			(m_Pars.SliderRect.top <= Point.y && Point.y <= m_Pars.SliderRect.bottom)
			)
		{
			m_Mousemove = TRUE;

			m_Pars.Pos = 1 - (real_t)(Point.x - m_Pars.SliderRect.left) / w;


			real_t D = m_Pars.Pos*0.25;

			m_Pars.Title.Format(_T("NEAR POINT   %.2fD"), (1 - m_Pars.Pos) * 3);

			Invalidate(FALSE);

			GetParent()->PostMessage(WM_SLIDER_POS_CHANGED, (WPARAM)this, 0);
		}
	}
}

//***************************************************************************************
