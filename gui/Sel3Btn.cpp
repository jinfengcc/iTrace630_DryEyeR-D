//***************************************************************************************
//(3 Buttons selection  Button)

//***************************************************************************************

#include "StdAfx.h"

#include "Sel3Btn.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CSel3Btn, CWnd)

	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()

	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)

END_MESSAGE_MAP()

//***************************************************************************************

CSel3Btn::CSel3Btn()
{
	m_HasMouse1 = FALSE;
	m_HasMouse2 = FALSE;
	m_HasMouse3 = FALSE;

	m_pTipWnd = NULL;
}

//***************************************************************************************
CSel3Btn::~CSel3Btn()
{
	if (m_pTipWnd) delete m_pTipWnd;
}
//***************************************************************************************

BOOL CSel3Btn::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = ::AfxRegisterWndClass(CS_DBLCLKS, ::LoadCursor(NULL, IDC_HAND), NULL, NULL);
	cs.style = cs.style | WS_CHILD;
	cs.dwExStyle = 0;

	m_MemDC.CreateTrueColorDC(cs.cx, cs.cy);

	return CWnd::PreCreateWindow(cs);
}

//***************************************************************************************

void CSel3Btn::OnShowWindow(BOOL bShow, uint nStatus)
{
	if (bShow) Repaint();

	CWnd::OnShowWindow(bShow, nStatus);
}

//***************************************************************************************

void CSel3Btn::OnPaint()
{
	CPaintDC PaintDC(this);

	CMDC DC;
	DC.Attach(PaintDC.Detach());

	RECT Rect, LeftOutLineRect, MidOutLineRect, RightOutLineRect;
	GetClientRect(&Rect);

	m_w = Rect.right - Rect.left;
	m_h = Rect.bottom - Rect.top;

	m_Third_w = intRound(real_t(m_w) / 3);

	::SetRect(&m_LeftRect, 0, Rect.top, m_Third_w, Rect.bottom);
	::SetRect(&m_MidRect, m_Third_w, Rect.top, 2 * m_Third_w, Rect.bottom);
	::SetRect(&m_RightRect, 2 * m_Third_w, Rect.top, 3 * m_Third_w, Rect.bottom);

	if (m_HasMouse1 || m_HasMouse2 || m_HasMouse3)
	{
		if (m_HasMouse1)
		{
			DC.DrawRectangle(m_RightRect, BLACK, BLACK);
			DC.DrawRectangle(m_MidRect, BLACK, BLACK);
			DC.DrawRectangle(m_LeftRect, YELLOW, YELLOW);
		}
		else if (m_HasMouse2)
		{
			DC.DrawRectangle(m_LeftRect, BLACK, BLACK);
			DC.DrawRectangle(m_RightRect, BLACK, BLACK);
			DC.DrawRectangle(m_MidRect, YELLOW, YELLOW);
		}
		else
		{
			DC.DrawRectangle(m_LeftRect, BLACK, BLACK);
			DC.DrawRectangle(m_MidRect, BLACK, BLACK);
			DC.DrawRectangle(m_RightRect, YELLOW, YELLOW);
		}
	}
	else
	{
		DC.DrawRectangle(Rect, BLACK, BLACK);
		DC.DrawLine(m_Third_w, Rect.top, m_Third_w, Rect.bottom, 1, BLACK);
	}

	int w = Rect.right - Rect.left + 1;
	int h = Rect.bottom - Rect.top + 1;
	int e = (w - 8) / 2;
	int d = h / 2;

	//out  
	::SetRect(&LeftOutLineRect, m_LeftRect.left + 1, m_LeftRect.top + 1, m_LeftRect.right - 1, m_LeftRect.bottom - 1);
	DC.DrawRectangle(LeftOutLineRect, DARK_GRAY, DARK_GRAY);

	::SetRect(&MidOutLineRect, m_MidRect.left + 1, m_MidRect.top + 1, m_MidRect.right - 1, m_MidRect.bottom - 1);
	DC.DrawRectangle(MidOutLineRect, DARK_GRAY, DARK_GRAY);

	::SetRect(&RightOutLineRect, m_RightRect.left + 1, m_RightRect.top + 1, m_RightRect.right - 1, m_RightRect.bottom - 1);
	DC.DrawRectangle(RightOutLineRect, DARK_GRAY, DARK_GRAY);

	CMFont Font(intRound(0.9*(LeftOutLineRect.bottom - LeftOutLineRect.top - 10)), 400, "Arial");

	CMFont SmallFont(intRound(0.6*(LeftOutLineRect.bottom - LeftOutLineRect.top - 10)), 400, "Arial");

	//In Button 
	if (m_OnOff == 1)
	{
		DC.DrawRectangle(LeftOutLineRect, WHITE, CYAN);
		DC.DrawLine(LeftOutLineRect.left + 1, LeftOutLineRect.top + 1, LeftOutLineRect.right - 2, LeftOutLineRect.top + 1, 3, WHITE);
		DC.DrawLine(LeftOutLineRect.right - 3, LeftOutLineRect.top + 3, LeftOutLineRect.right - 3, LeftOutLineRect.bottom - 4, 3, BLACK);
		DC.DrawLine(LeftOutLineRect.left + 1, LeftOutLineRect.bottom - 4, LeftOutLineRect.right - 3, LeftOutLineRect.bottom - 4, 3, BLACK);
		DC.DrawLine(LeftOutLineRect.left + 1, LeftOutLineRect.top + 1, LeftOutLineRect.left + 1, LeftOutLineRect.bottom - 2, 3, WHITE);

		DC.DrawLine(LeftOutLineRect.left + 2, LeftOutLineRect.top + 2, LeftOutLineRect.right - 3, LeftOutLineRect.top + 2, 1, DARK_GRAY);
		DC.DrawLine(LeftOutLineRect.left + 2, LeftOutLineRect.top + 3, LeftOutLineRect.left + 2, LeftOutLineRect.bottom - 3, 1, DARK_GRAY);

		if (m_SmallFont[0])
		{
			CString Lines[8];
			int n = 0;
			int RectW = LeftOutLineRect.right - LeftOutLineRect.left - 10;

			CString s(m_Labels[0]);
			int l;
			while ((l = s.GetLength()) > 0 && n < 8)
			{
				int l1;
				for (l1 = 1; l1 <= l; l1++)
				{
					RECT Rect1;
					m_MemDC.MeasureRect(s.Left(l1), Rect1, SmallFont);
					int CurW = Rect1.right - Rect1.left;
					if (CurW > RectW)
					{
						l1--;
						for (int l2 = l1; l2 >= 1; l2--)
						{
							if (s[l2 - 1] == ' ')
							{
								l1 = l2;
								break;
							}
						}
						break;
					}
				}

				if (l1 == 0) break;

				if (s.GetAt(l1 - 1) == ' ')
				{
					Lines[n] = s.Left(l1 - 1);
				}
				else
				{
					Lines[n] = s.Left(l1);
				}
				s = s.Right(l - l1);
				n++;
			}

			int dh = (LeftOutLineRect.bottom - LeftOutLineRect.top) / (n + 1);

			RECT Rect2;
			Rect2.left = LeftOutLineRect.left;
			Rect2.right = LeftOutLineRect.right;
			Rect2.top = LeftOutLineRect.top;
			Rect2.bottom = Rect2.top + 2 * dh;

			for (int i = 0; i < n; i++)
			{
				DC.WriteText(Lines[i], Rect2, SmallFont, BLACK, 1, CYAN);
				Rect2.top += dh; Rect2.bottom += dh;
			}
		}
		else
		{
			DC.WriteText(m_Labels[0], LeftOutLineRect, Font, BLACK, 1, CYAN);
		}

		if (m_SmallFont[1])
		{
			CString Lines[8];
			int n = 0;
			int RectW = MidOutLineRect.right - MidOutLineRect.left - 10;

			CString s(m_Labels[1]);
			int l;

			while ((l = s.GetLength()) > 0 && n < 8)
			{
				int l1;
				for (l1 = 1; l1 <= l; l1++)
				{
					RECT Rect1;
					m_MemDC.MeasureRect(s.Left(l1), Rect1, SmallFont);
					int CurW = Rect1.right - Rect1.left;
					if (CurW > RectW)
					{
						l1--;
						for (int l2 = l1; l2 >= 1; l2--)
						{
							if (s[l2 - 1] == ' ')
							{
								l1 = l2;
								break;
							}
						}
						break;
					}
				}

				if (l1 == 0) break;

				if (s.GetAt(l1 - 1) == ' ')
				{
					Lines[n] = s.Left(l1 - 1);
				}
				else
				{
					Lines[n] = s.Left(l1);
				}

				s = s.Right(l - l1);
				n++;
			}

			int dh = (MidOutLineRect.bottom - MidOutLineRect.top) / (n + 1);

			RECT Rect2;
			Rect2.left = MidOutLineRect.left;
			Rect2.right = MidOutLineRect.right;
			Rect2.top = MidOutLineRect.top;
			Rect2.bottom = Rect2.top + 2 * dh;

			for (int i = 0; i < n; i++)
			{
				DC.WriteText(Lines[i], Rect2, SmallFont, GRAY, 1, DARK_GRAY);
				Rect2.top += dh; Rect2.bottom += dh;
			}
		}
		else
		{
			DC.WriteText(m_Labels[1], MidOutLineRect, Font, GRAY, 1, DARK_GRAY);
		}

		if (m_SmallFont[2])
		{
			CString Lines[8];
			int n = 0;
			int RectW = RightOutLineRect.right - RightOutLineRect.left - 10;

			CString s(m_Labels[2]);
			int l;

			while ((l = s.GetLength()) > 0 && n < 8)
			{
				int l1;
				for (l1 = 1; l1 <= l; l1++)
				{
					RECT Rect1;
					m_MemDC.MeasureRect(s.Left(l1), Rect1, SmallFont);
					int CurW = Rect1.right - Rect1.left;
					if (CurW > RectW)
					{
						l1--;
						for (int l2 = l1; l2 >= 1; l2--)
						{
							if (s[l2 - 1] == ' ')
							{
								l1 = l2;
								break;
							}
						}
						break;
					}
				}

				if (l1 == 0) break;

				if (s.GetAt(l1 - 1) == ' ')
				{
					Lines[n] = s.Left(l1 - 1);
				}
				else
				{
					Lines[n] = s.Left(l1);
				}

				s = s.Right(l - l1);
				n++;
			}

			int dh = (RightOutLineRect.bottom - RightOutLineRect.top) / (n + 1);

			RECT Rect2;
			Rect2.left = RightOutLineRect.left;
			Rect2.right = RightOutLineRect.right;
			Rect2.top = RightOutLineRect.top;
			Rect2.bottom = Rect2.top + 2 * dh;
			for (int i = 0; i < n; i++)
			{
				DC.WriteText(Lines[i], Rect2, SmallFont, GRAY, 1, DARK_GRAY);
				Rect2.top += dh; Rect2.bottom += dh;
			}
		}
		else
		{
			DC.WriteText(m_Labels[2], RightOutLineRect, Font, GRAY, 1, DARK_GRAY);
		}
	}
	else if (m_OnOff == 2)
	{
		DC.DrawRectangle(MidOutLineRect, WHITE, CYAN);
		DC.DrawLine(MidOutLineRect.left + 1, MidOutLineRect.top + 1, MidOutLineRect.right - 2, MidOutLineRect.top + 1, 3, WHITE);
		DC.DrawLine(MidOutLineRect.right - 3, MidOutLineRect.top + 3, MidOutLineRect.right - 3, MidOutLineRect.bottom - 4, 3, BLACK);
		DC.DrawLine(MidOutLineRect.left + 1, MidOutLineRect.bottom - 4, MidOutLineRect.right - 3, MidOutLineRect.bottom - 4, 3, BLACK);
		DC.DrawLine(MidOutLineRect.left + 1, MidOutLineRect.top + 1, MidOutLineRect.left + 1, MidOutLineRect.bottom - 2, 3, WHITE);

		DC.DrawLine(MidOutLineRect.left + 2, MidOutLineRect.top + 2, MidOutLineRect.right - 3, MidOutLineRect.top + 2, 1, DARK_GRAY);
		DC.DrawLine(MidOutLineRect.left + 2, MidOutLineRect.top + 3, MidOutLineRect.left + 2, MidOutLineRect.bottom - 3, 1, DARK_GRAY);

		if (m_SmallFont[0])
		{
			CString Lines[8];
			int n = 0;
			int RectW = LeftOutLineRect.right - LeftOutLineRect.left - 10;

			CString s(m_Labels[0]);
			int l;

			while ((l = s.GetLength()) > 0 && n < 8)
			{
				int l1;
				for (l1 = 1; l1 <= l; l1++)
				{
					RECT Rect1;
					m_MemDC.MeasureRect(s.Left(l1), Rect1, SmallFont);
					int CurW = Rect1.right - Rect1.left;
					if (CurW > RectW)
					{
						l1--;
						for (int l2 = l1; l2 >= 1; l2--)
						{
							if (s[l2 - 1] == ' ')
							{
								l1 = l2;
								break;
							}
						}
						break;
					}
				}

				if (l1 == 0) break;

				if (s.GetAt(l1 - 1) == ' ')
				{
					Lines[n] = s.Left(l1 - 1);
				}
				else
				{
					Lines[n] = s.Left(l1);
				}

				s = s.Right(l - l1);
				n++;
			}

			int dh = (LeftOutLineRect.bottom - LeftOutLineRect.top) / (n + 1);

			RECT Rect2;
			Rect2.left = LeftOutLineRect.left;
			Rect2.right = LeftOutLineRect.right;
			Rect2.top = LeftOutLineRect.top;
			Rect2.bottom = Rect2.top + 2 * dh;
			for (int i = 0; i < n; i++)
			{
				DC.WriteText(Lines[i], Rect2, SmallFont, GRAY, 1, DARK_GRAY);
				Rect2.top += dh; Rect2.bottom += dh;
			}
		}
		else
		{
			DC.WriteText(m_Labels[0], LeftOutLineRect, Font, GRAY, 1, DARK_GRAY);
		}

		if (m_SmallFont[1])
		{
			CString Lines[8];
			int n = 0;
			int RectW = MidOutLineRect.right - MidOutLineRect.left - 10;

			CString s(m_Labels[1]);
			int l;

			while ((l = s.GetLength()) > 0 && n < 8)
			{
				int l1;
				for (l1 = 1; l1 <= l; l1++)
				{
					RECT Rect1;
					m_MemDC.MeasureRect(s.Left(l1), Rect1, SmallFont);
					int CurW = Rect1.right - Rect1.left;

					if (CurW > RectW)
					{
						l1--;
						for (int l2 = l1; l2 >= 1; l2--)
						{
							if (s[l2 - 1] == ' ')
							{
								l1 = l2;
								break;
							}
						}
						break;
					}
				}

				if (l1 == 0) break;

				if (s.GetAt(l1 - 1) == ' ')
				{
					Lines[n] = s.Left(l1 - 1);
				}
				else
				{
					Lines[n] = s.Left(l1);
				}

				s = s.Right(l - l1);
				n++;
			}

			int dh = (MidOutLineRect.bottom - MidOutLineRect.top) / (n + 1);

			RECT Rect2;
			Rect2.left = MidOutLineRect.left;
			Rect2.right = MidOutLineRect.right;
			Rect2.top = MidOutLineRect.top;
			Rect2.bottom = Rect2.top + 2 * dh;
			for (int i = 0; i < n; i++) {
				DC.WriteText(Lines[i], Rect2, SmallFont, BLACK, 1, CYAN);
				Rect2.top += dh; Rect2.bottom += dh;
			}
		}
		else
		{
			DC.WriteText(m_Labels[1], MidOutLineRect, Font, BLACK, 1, CYAN);
		}

		if (m_SmallFont[2])
		{
			CString Lines[8];
			int n = 0;
			int RectW = RightOutLineRect.right - RightOutLineRect.left - 10;

			CString s(m_Labels[2]);
			int l;

			while ((l = s.GetLength()) > 0 && n < 8)
			{
				int l1;
				for (l1 = 1; l1 <= l; l1++)
				{
					RECT Rect1;
					m_MemDC.MeasureRect(s.Left(l1), Rect1, SmallFont);
					int CurW = Rect1.right - Rect1.left;
					if (CurW > RectW)
					{
						l1--;
						for (int l2 = l1; l2 >= 1; l2--)
						{
							if (s[l2 - 1] == ' ')
							{
								l1 = l2;
								break;
							}
						}
						break;
					}
				}

				if (l1 == 0) break;

				if (s.GetAt(l1 - 1) == ' ')
				{
					Lines[n] = s.Left(l1 - 1);
				}
				else
				{
					Lines[n] = s.Left(l1);
				}

				s = s.Right(l - l1);
				n++;
			}

			int dh = (RightOutLineRect.bottom - RightOutLineRect.top) / (n + 1);

			RECT Rect2;
			Rect2.left = RightOutLineRect.left;
			Rect2.right = RightOutLineRect.right;
			Rect2.top = RightOutLineRect.top;
			Rect2.bottom = Rect2.top + 2 * dh;

			for (int i = 0; i < n; i++)
			{
				DC.WriteText(Lines[i], Rect2, SmallFont, GRAY, 1, DARK_GRAY);
				Rect2.top += dh; Rect2.bottom += dh;
			}
		}
		else
		{
			DC.WriteText(m_Labels[2], RightOutLineRect, Font, GRAY, 1, DARK_GRAY);
		}
	}
	else
	{
		DC.DrawRectangle(RightOutLineRect, WHITE, CYAN);
		DC.DrawLine(RightOutLineRect.left + 1, RightOutLineRect.top + 1, RightOutLineRect.right - 2, RightOutLineRect.top + 1, 3, WHITE);
		DC.DrawLine(RightOutLineRect.right - 3, RightOutLineRect.top + 3, RightOutLineRect.right - 3, RightOutLineRect.bottom - 4, 3, BLACK);
		DC.DrawLine(RightOutLineRect.left + 1, RightOutLineRect.bottom - 4, RightOutLineRect.right - 3, RightOutLineRect.bottom - 4, 3, BLACK);
		DC.DrawLine(RightOutLineRect.left + 1, LeftOutLineRect.top + 1, RightOutLineRect.left + 1, RightOutLineRect.bottom - 2, 3, WHITE);

		DC.DrawLine(RightOutLineRect.left + 2, RightOutLineRect.top + 2, RightOutLineRect.right - 3, RightOutLineRect.top + 2, 1, DARK_GRAY);
		DC.DrawLine(RightOutLineRect.left + 2, RightOutLineRect.top + 3, RightOutLineRect.left + 2, RightOutLineRect.bottom - 3, 1, DARK_GRAY);

		if (m_SmallFont[0])
		{
			CString Lines[8];
			int n = 0;
			int RectW = LeftOutLineRect.right - LeftOutLineRect.left - 10;

			CString s(m_Labels[0]);
			int l;

			while ((l = s.GetLength()) > 0 && n < 8)
			{
				int l1;
				for (l1 = 1; l1 <= l; l1++)
				{
					RECT Rect1;
					m_MemDC.MeasureRect(s.Left(l1), Rect1, SmallFont);

					int CurW = Rect1.right - Rect1.left;
					if (CurW > RectW)
					{
						l1--;
						for (int l2 = l1; l2 >= 1; l2--)
						{
							if (s[l2 - 1] == ' ')
							{
								l1 = l2;
								break;
							}
						}
						break;
					}
				}

				if (l1 == 0) break;
				if (s.GetAt(l1 - 1) == ' ')
				{
					Lines[n] = s.Left(l1 - 1);
				}
				else
				{
					Lines[n] = s.Left(l1);
				}

				s = s.Right(l - l1);
				n++;
			}

			int dh = (LeftOutLineRect.bottom - LeftOutLineRect.top) / (n + 1);

			RECT Rect2;
			Rect2.left = LeftOutLineRect.left;
			Rect2.right = LeftOutLineRect.right;
			Rect2.top = LeftOutLineRect.top;
			Rect2.bottom = Rect2.top + 2 * dh;

			for (int i = 0; i < n; i++)
			{
				DC.WriteText(Lines[i], Rect2, SmallFont, GRAY, 1, DARK_GRAY);
				Rect2.top += dh; Rect2.bottom += dh;
			}
		}
		else
		{
			DC.WriteText(m_Labels[0], LeftOutLineRect, Font, GRAY, 1, DARK_GRAY);
		}

		if (m_SmallFont[1])
		{
			CString Lines[8];
			int n = 0;
			int RectW = MidOutLineRect.right - MidOutLineRect.left - 10;

			CString s(m_Labels[1]);
			int l;

			while ((l = s.GetLength()) > 0 && n < 8)
			{
				int l1;
				for (l1 = 1; l1 <= l; l1++)
				{
					RECT Rect1;
					m_MemDC.MeasureRect(s.Left(l1), Rect1, SmallFont);
					int CurW = Rect1.right - Rect1.left;
					if (CurW > RectW)
					{
						l1--;
						for (int l2 = l1; l2 >= 1; l2--)
						{
							if (s[l2 - 1] == ' ')
							{
								l1 = l2;
								break;
							}
						}
						break;
					}
				}

				if (l1 == 0) break;

				if (s.GetAt(l1 - 1) == ' ')
				{
					Lines[n] = s.Left(l1 - 1);
				}
				else
				{
					Lines[n] = s.Left(l1);
				}

				s = s.Right(l - l1);
				n++;
			}

			int dh = (MidOutLineRect.bottom - MidOutLineRect.top) / (n + 1);

			RECT Rect2;
			Rect2.left = MidOutLineRect.left;
			Rect2.right = MidOutLineRect.right;
			Rect2.top = MidOutLineRect.top;
			Rect2.bottom = Rect2.top + 2 * dh;
			for (int i = 0; i < n; i++)
			{
				DC.WriteText(Lines[i], Rect2, SmallFont, GRAY, 1, DARK_GRAY);
				Rect2.top += dh; Rect2.bottom += dh;
			}
		}
		else
		{
			DC.WriteText(m_Labels[1], MidOutLineRect, Font, GRAY, 1, DARK_GRAY);
		}

		if (m_SmallFont[2])
		{
			CString Lines[8];
			int n = 0;
			int RectW = RightOutLineRect.right - RightOutLineRect.left - 10;

			CString s(m_Labels[2]);
			int l;

			while ((l = s.GetLength()) > 0 && n < 8)
			{
				int l1;
				for (l1 = 1; l1 <= l; l1++)
				{
					RECT Rect1;
					m_MemDC.MeasureRect(s.Left(l1), Rect1, SmallFont);
					int CurW = Rect1.right - Rect1.left;
					if (CurW > RectW)
					{
						l1--;
						for (int l2 = l1; l2 >= 1; l2--)
						{
							if (s[l2 - 1] == ' ')
							{
								l1 = l2;
								break;
							}
						}
						break;
					}
				}

				if (l1 == 0) break;

				if (s.GetAt(l1 - 1) == ' ')
				{
					Lines[n] = s.Left(l1 - 1);
				}
				else
				{
					Lines[n] = s.Left(l1);
				}

				s = s.Right(l - l1);
				n++;
			}

			int dh = (RightOutLineRect.bottom - RightOutLineRect.top) / (n + 1);

			RECT Rect2;
			Rect2.left = RightOutLineRect.left;
			Rect2.right = RightOutLineRect.right;
			Rect2.top = RightOutLineRect.top;
			Rect2.bottom = Rect2.top + 2 * dh;
			for (int i = 0; i < n; i++)
			{
				DC.WriteText(Lines[i], Rect2, SmallFont, BLACK, 1, CYAN);
				Rect2.top += dh; Rect2.bottom += dh;
			}
		}
		else
		{
			DC.WriteText(m_Labels[2], RightOutLineRect, Font, BLACK, 1, CYAN);
		}
	}

	PaintDC.Attach(DC.Detach());
}

//***************************************************************************************

LRESULT CSel3Btn::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_HasMouse1 = FALSE;
	m_HasMouse2 = FALSE;
	m_HasMouse3 = FALSE;

	HideTip();//

	Invalidate(FALSE);

	return 0;
}

//***************************************************************************************

void CSel3Btn::Repaint()
{
	Invalidate(FALSE);
}

//***************************************************************************************

void CSel3Btn::OnLButtonDown(uint nFlags, CPoint Point)
{
	HideTip();
}

//***************************************************************************************

void CSel3Btn::OnLButtonUp(uint nFlags, CPoint Point)
{
	if (MK_LBUTTON)
	{
		if (Point.x < m_Third_w)
		{
			m_OnOff = 1;
		}
		else if (Point.x < 2 * m_Third_w && Point.x >= m_Third_w)
		{
			m_OnOff = 2;
		}
		else if (Point.x <= 3 * m_Third_w && Point.x >= 2 * m_Third_w)
		{
			m_OnOff = 3;
		}

		Repaint();

		GetParent()->PostMessage(WM_SELBTN_POS_CHANGED, (WPARAM)this, m_id);
	}
}

//***************************************************************************************

void CSel3Btn::OnMouseMove(uint nFlags, CPoint Point)
{
	if (Point.x < m_Third_w)
	{
		if (m_CurrentTip != 0 && m_pTipWnd)
		{
			delete m_pTipWnd;
			m_pTipWnd = NULL;
			ShowTip(0);
		}

		m_HasMouse1 = TRUE;
		m_HasMouse2 = FALSE;
		m_HasMouse3 = FALSE;
	}
	else if (Point.x < 2 * m_Third_w && Point.x >= m_Third_w)
	{
		if (m_CurrentTip != 1 && m_pTipWnd)
		{
			delete m_pTipWnd;
			m_pTipWnd = NULL;
			ShowTip(1);
		}

		m_HasMouse2 = TRUE;
		m_HasMouse1 = FALSE;
		m_HasMouse3 = FALSE;
	}
	else if (Point.x <= 3 * m_Third_w && Point.x >= 2 * m_Third_w)
	{
		if (m_CurrentTip != 2 && m_pTipWnd)
		{
			delete m_pTipWnd;
			m_pTipWnd = NULL;
			ShowTip(2);
		}

		m_HasMouse3 = TRUE;
		m_HasMouse1 = FALSE;
		m_HasMouse2 = FALSE;
	}

	TRACKMOUSEEVENT Event = { sizeof(TRACKMOUSEEVENT), TME_LEAVE | TME_HOVER, m_hWnd, 500/*ms*/ };
	::TrackMouseEvent(&Event);

	Repaint();
}

//***************************************************************************************

LRESULT CSel3Btn::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	CPoint point;
	GetCursorPos(&point);

	int WhichSubBtn = 0;

	RECT Rect;
	GetWindowRect(&Rect);

	int pos = point.x - Rect.left;

	if (m_LeftRect.left < pos  && pos <= m_LeftRect.right)
	{
		WhichSubBtn = 0;
	}
	else if (m_MidRect.left < pos  && pos <= m_MidRect.right)
	{
		WhichSubBtn = 1;
	}
	else if (m_RightRect.left< pos  && pos <= m_RightRect.right)
	{
		WhichSubBtn = 2;
	}

	ShowTip(WhichSubBtn);
	return 0;
}

//***************************************************************************************

void CSel3Btn::ShowTip(int num)
{
	m_CurrentTip = num;

	if (m_Tip[num].IsEmpty()) return;
	if (m_pTipWnd) return;

	RECT Rect;
	GetWindowRect(&Rect);

	if (num == 0)
	{
		Rect.right = Rect.left + m_LeftRect.right;
	}
	else if (num == 1)
	{
		Rect.left += m_MidRect.left;
		Rect.right = Rect.left + m_Third_w;
	}
	else if (num == 2)
	{
		Rect.left += m_RightRect.left;
		Rect.right = Rect.left + m_Third_w;
	}

	m_pTipWnd = new CTipWnd(Rect, this, m_Tip[num]);
}

//***************************************************************************************

void CSel3Btn::HideTip()
{
	if (m_pTipWnd)
	{
		delete m_pTipWnd;
		m_pTipWnd = NULL;
	}
}

//***************************************************************************************