//WF capture controller window

//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"

#include "WFGUICtrl0Wnd.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CWFGUICtrl0Wnd, CMemWnd)

	ON_MESSAGE(WM_SLIDER_LCLICK, OnSliderClick)
	ON_MESSAGE(WM_SLIDER_POS_CHANGED, OnSliderChangedPos)

END_MESSAGE_MAP()

//***************************************************************************************'

CWFGUICtrl0Wnd::CWFGUICtrl0Wnd() :CMemWnd()
{
}

void CWFGUICtrl0Wnd::CreateThisWnd(RECT& Rect, CWnd* pWnd)
{
	CreateWnd(Rect, pWnd);

	m_w = Rect.right - Rect.left;
	m_h = Rect.bottom - Rect.top;

	int Slider_w = intRound(real_t(m_w) / 10);

	int Left_w = intRound(real_t(m_w) / 3);
	int Right_w = Left_w + Slider_w;
	int top_h = intRound(real_t(m_h) / 10);
	int btm_h = m_h - top_h;

	int d = Slider_w / 2;
	int y1 = top_h + d;
	int y2 = btm_h - d;

	m_FontSize = intRound(real_t(y2 - y1) / 20);
	int h_fontSize = intRound(real_t(m_FontSize) / 2.0);

	RECT tmpRect;
	CMFont Font(m_FontSize, 400, "Arial");
	m_MemDC.MeasureRect("   0.00mm", tmpRect, Font);
	int Label_w = tmpRect.right - tmpRect.left;

	for (int i = 0; i <= 6; i++)
	{
		int xL = Right_w + 8;
		int xR = xL + Label_w;

		int  yPos = intRound(y1 + real_t(i) * real_t(y2 - y1) / 6);
		if (i == 6)  yPos = y2;

		int yT = yPos - h_fontSize;
		int yB = yPos + h_fontSize;

		::SetRect(&m_SliderLabelRect[i], xL, yT, xR, yB);
	}

	for (int i = 0; i <= 7; i++)
	{
		int xL = Right_w + 8;
		int xR = xL + Label_w;

		int  yPos = intRound(y1 + real_t(i) * real_t(y2 - y1) / 7);
		if (i == 7)  yPos = y2;

		int yT = yPos - h_fontSize;
		int yB = yPos + h_fontSize;

		::SetRect(&m_Slider64LabelRect[i], xL, yT, xR, yB);
	}

	::SetRect(&m_SliderRect, Left_w, top_h, Right_w, btm_h);
	m_Slider.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_SliderRect, this, IDC_WFGUI_SLIDER);
}

//***************************************************************************************

//Create the slider for Presbia 256
void CWFGUICtrl0Wnd::CrSliderInfo()
{
	if (::Normal64Mode)
	{
		m_Slider.m_scaleNum = 7;
		m_Slider.LowerstPos = 0;
	}
	else
	{
		if (::PresbiaMode == 3)
		{
			m_Slider.m_scaleNum = 3;
			m_Slider.LowerstPos = (1.3 - 1.0) / (4.0 - 1.0);
		}

		else
		{
			m_Slider.m_scaleNum = 6;
			m_Slider.LowerstPos = 0;
		}
	}
	m_Slider.m_Pos = 1;
	m_Slider.OnPaint();
}

//***************************************************************************************

CWFGUICtrl0Wnd::~CWFGUICtrl0Wnd()
{
}

//***************************************************************************************

void CWFGUICtrl0Wnd::ShowGUI()
{
	m_ShowWindow = TRUE;

	m_Slider.ShowWindow(SW_SHOW);

	Label();
}

//***************************************************************************************

void CWFGUICtrl0Wnd::HideGUI()
{
	m_ShowWindow = FALSE;

	m_Slider.ShowWindow(SW_HIDE);

	Label();
}

//***************************************************************************************

void CWFGUICtrl0Wnd::RepaintMemDC()
{
}

//***************************************************************************************

void CWFGUICtrl0Wnd::Label()
{
	//Presbia
	for (int i = 0; i <= 7; i++)
	{
		if (i <= 6)
		{
			m_MemDC.FillSolidRect(&m_SliderLabelRect[i], BLACK);
			InvalidateRect(&m_SliderLabelRect[i], FALSE);
		}

		m_MemDC.FillSolidRect(&m_Slider64LabelRect[i], BLACK);
		InvalidateRect(&m_Slider64LabelRect[i], FALSE);
	}
	//Presbia

	if (m_ShowWindow)
	{
		CMFont Font(intRound(0.9 * m_FontSize), 400, "Arial");

		CString s;

		if (::Normal64Mode)
		{
			for (int i = 0; i <= 7; i++)
			{
				if (i == 0)      s = "8.00mm";
				else if (i == 1) s = "7.00mm";
				else if (i == 2) s = "6.00mm";
				else if (i == 3) s = "5.00mm";
				else if (i == 4) s = "4.00mm";
				else if (i == 5) s = "3.00mm";
				else if (i == 6) s = "2.00mm";
				else if (i == 7) s = "1.00mm";

				m_MemDC.WriteText(s, m_Slider64LabelRect[i], Font, RED, 0, BLACK);
				InvalidateRect(&m_Slider64LabelRect[i], FALSE);
			}
		}
		else
		{
			if (::PresbiaMode == 3)
			{
				for (int i = 0; i <= 6; i++)
				{
					if (i == 0)      s = "4.00mm";
					else if (i == 1) s = "";
					else if (i == 2) s = "3.00mm";
					else if (i == 3) s = "";
					else if (i == 4) s = "2.00mm";
					else if (i == 5) s = "";
					else if (i == 6) s = "1.00mm";

					m_MemDC.WriteText(s, m_SliderLabelRect[i], Font, RED, 0, BLACK);
					InvalidateRect(&m_SliderLabelRect[i], FALSE);
				}
			}
			else
			{
				for (int i = 0; i <= 6; i++)
				{
					if (i == 0)      s = "8.00mm";
					else if (i == 1) s = "7.00mm";
					else if (i == 2) s = "6.00mm";
					else if (i == 3) s = "5.00mm";
					else if (i == 4) s = "4.00mm";
					else if (i == 5) s = "3.00mm";
					else if (i == 6) s = "2.00mm";

					m_MemDC.WriteText(s, m_SliderLabelRect[i], Font, RED, 0, BLACK);
					InvalidateRect(&m_SliderLabelRect[i], FALSE);
				}
			}
		}
	}
}


//***************************************************************************************

void CWFGUICtrl0Wnd::SetPos(real_t Pos_Val)
{
	if (::Normal64Mode)
	{
		m_Slider.m_Pos = (Pos_Val - 1.0) / (8.0 - 1.0);
	}
	else
	{
		if (::PresbiaMode != 3) m_Slider.m_Pos = (Pos_Val - 2.0) / (8.0 - 2.0);//Presbia
		else m_Slider.m_Pos = (Pos_Val - 1.0) / (4.0 - 1.0);//Presbia
	}
	m_Slider.OnPaint();
	m_Slider.Refresh();

	if (::Normal64Mode)
	{
		m_ScanDiameter = 1000 * (1 + 7 * m_Slider.m_Pos);
	}
	else
	{
		if (::PresbiaMode != 3) m_ScanDiameter = 1000 * (2 + 6 * m_Slider.m_Pos);//Presbia
		else m_ScanDiameter = 1000 * (1 + 3 * m_Slider.m_Pos);//Presbia
	}
}

//***************************************************************************************

LRESULT CWFGUICtrl0Wnd::OnSliderChangedPos(WPARAM wParam, LPARAM lParam)
{
	real_t TemPDiameter;

	if (::Normal64Mode)
	{
		TemPDiameter = 1000 * (1 + 7 * m_Slider.m_Pos);
	}
	else
	{
		if (::PresbiaMode != 3) TemPDiameter = 1000 * (2 + 6 * m_Slider.m_Pos);//Presbia
		else  TemPDiameter = 1000 * (1 + 3 * m_Slider.m_Pos);//Presbia
	}

	TemPDiameter = real_t(intRound(TemPDiameter / 100)) + 0.0000000001;

	if (::PresbiaMode == 3 && TemPDiameter < 13) TemPDiameter = 13;

	m_ScanDiameter = 100 * TemPDiameter;

	if (::Normal64Mode)
	{
		m_Slider.m_Pos = (TemPDiameter / 10 - 1) / 7;
	}
	else
	{
		if (::PresbiaMode != 3)
			m_Slider.m_Pos = (TemPDiameter / 10 - 2) / 6;
		else
			m_Slider.m_Pos = (TemPDiameter / 10 - 1) / 3;
	}

	return 0;
}

//***************************************************************************************

LRESULT CWFGUICtrl0Wnd::OnSliderClick(WPARAM wParam, LPARAM lParam)
{
	CPoint point;
	GetCursorPos(&point);

	real_t TemPDiameter;

	if (::Normal64Mode)
	{
		TemPDiameter = 1000 * (1 + 7 * m_Slider.m_Pos);
	}
	else
	{
		if (::PresbiaMode != 3) TemPDiameter = 1000 * (2 + 6 * m_Slider.m_Pos);//Presbia
		else TemPDiameter = 1000 * (1 + 3 * m_Slider.m_Pos);//Presbia
	}

	TemPDiameter = real_t(intRound(TemPDiameter / 1000)) + 0.0000000001;

	if (!::Normal64Mode)
	{
		if (TemPDiameter <= 1.00001) TemPDiameter = 1.3;
	}

	m_ScanDiameter = 1000 * TemPDiameter;

	if (::Normal64Mode)
	{
		m_Slider.m_Pos = (TemPDiameter - 1) / 7;
	}
	else
	{
		if (::PresbiaMode != 3)
			m_Slider.m_Pos = (TemPDiameter - 2) / 6;
		else
			m_Slider.m_Pos = (TemPDiameter - 1) / 3;
	}

	RECT TempRect;
	GetWindowRect(&TempRect);

	int y = m_SliderRect.top + m_Slider.returnY();
	SetCursorPos(point.x + 7, TempRect.top + m_SliderRect.top + m_Slider.returnY());

	return 0;
}

//***************************************************************************************
