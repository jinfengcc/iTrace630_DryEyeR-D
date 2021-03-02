//***************************************************************************************

#include "StdAfx.h"
#include "GlobalFunctions.h"
#include "XPControl.h"

#include "PreBtn.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CPreBtn, CWnd)

	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)

END_MESSAGE_MAP()

//***************************************************************************************

CPreBtn::CPreBtn()
{
	m_Enabled = TRUE;
	m_Pushed = FALSE;
	m_HasMouse = FALSE;
	m_Highlighted = FALSE;

	m_DrawBackground = TRUE;

	m_BkColor = ::GetSysColor(COLOR_BTNFACE);

	m_EdgeColor = NOCOLOR;//530

	m_TextPos = 1;//530
	m_ImagePos = 0;

	m_Text = "";
	m_TextColor = BLACK;

	m_FontSize = 14;
	m_FontWidth = 400;
	m_FontFace = "Arial";

	m_pTipWnd = NULL;
}

//***************************************************************************************

CPreBtn::~CPreBtn()
{
	if (m_pTipWnd) delete m_pTipWnd;
}

//***************************************************************************************

BOOL CPreBtn::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = ::AfxRegisterWndClass(CS_DBLCLKS, ::LoadCursor(NULL, IDC_HAND), NULL, NULL);
	cs.style = cs.style | WS_CHILD;
	cs.dwExStyle = 0;

	m_MemDC.CreateTrueColorDC(cs.cx, cs.cy);

	return CWnd::PreCreateWindow(cs);
}

//***************************************************************************************

void CPreBtn::OnShowWindow(BOOL bShow, uint nStatus)
{
	if (bShow) Repaint();

	CWnd::OnShowWindow(bShow, nStatus);
}

//***************************************************************************************

void CPreBtn::OnPaint()
{
	CPaintDC PaintDC(this);

	RECT Rect;
	GetClientRect(&Rect);

	PaintDC.BitBlt(0, 0, Rect.right, Rect.bottom, &m_MemDC, 0, 0, SRCCOPY);
}

//***************************************************************************************

void CPreBtn::OnLButtonDown(uint nFlags, CPoint Point)
{
	if (m_Enabled)
	{
		HideTip();
		m_Pushed = TRUE;
		Repaint();
	}
}

//***************************************************************************************

void CPreBtn::OnLButtonUp(uint nFlags, CPoint Point)
{
	if (m_Pushed)
	{
		m_Pushed = FALSE;
		Repaint();

		ulong wParam = ((ulong)BN_CLICKED << 16) | (ulong)GetDlgCtrlID();
		ulong lParam = (ulong)m_hWnd;
		GetParent()->PostMessage(WM_COMMAND, (WPARAM)wParam, (LPARAM)lParam);
	}
}

//***************************************************************************************

void CPreBtn::OnMouseMove(uint nFlags, CPoint Point)
{
	if (!m_HasMouse)
	{
		m_HasMouse = TRUE;

		TRACKMOUSEEVENT Event = { sizeof(TRACKMOUSEEVENT), TME_LEAVE | TME_HOVER, m_hWnd, 500/*ms*/ };
		::TrackMouseEvent(&Event);

		Repaint();
	}
}

//***************************************************************************************

LRESULT CPreBtn::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_HasMouse = FALSE;

	m_Pushed = FALSE;

	HideTip();

	Repaint();

	return 0;
}

//***************************************************************************************

LRESULT CPreBtn::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	if (m_Enabled && !m_Pushed) ShowTip();
	return 0;
}

//***************************************************************************************

void CPreBtn::ShowTip()
{
	if (m_Tip.IsEmpty()) return;
	if (m_pTipWnd) return; // just in case

	RECT Rect;
	GetWindowRect(&Rect);

	m_pTipWnd = new CTipWnd(Rect, this, m_Tip);
}

//***************************************************************************************

void CPreBtn::HideTip()
{
	if (m_pTipWnd)
	{
		delete m_pTipWnd;
		m_pTipWnd = NULL;
	}
}

//***************************************************************************************

void CPreBtn::Repaint()
{
	RECT Rect;
	GetClientRect(&Rect);

	int State;
	if (!m_Enabled)
	{
		State = PBS_DISABLED;
	}
	else if (m_Pushed || m_Highlighted)
	{
		State = PBS_PRESSED;
	}
	else if (m_HasMouse)
	{
		State = PBS_HOT;
	}
	else
	{
		State = PBS_NORMAL;
	}

	if (m_BkColor != NOCOLOR)
	{
		if (m_EdgeColor != NOCOLOR)
		{
			RECT insideRect, outsideRect;
			::SetRect(&insideRect, Rect.left + 1, Rect.top + 1, Rect.right - 1, Rect.bottom - 1);
			::SetRect(&outsideRect, Rect.left - 2, Rect.top - 2, Rect.right + 2, Rect.bottom + 2);

			m_MemDC.FillSolidRect(&outsideRect, m_EdgeColor);
			m_MemDC.FillSolidRect(&insideRect, m_BkColor);
		}
		else m_MemDC.FillSolidRect(&Rect, m_BkColor);
	}

	if (m_DrawBackground)
	{
		if (!::DrawBackgintRound(XP_BUTTON, m_hWnd, m_MemDC.m_hDC, BP_PUSHBUTTON, State, Rect))
		{
			if (m_EdgeColor == NOCOLOR) m_MemDC.DrawRectangle(Rect, BLACK, ::GetSysColor(COLOR_BTNFACE));
			else m_MemDC.DrawRectangle(Rect, m_EdgeColor, ::GetSysColor(COLOR_BTNFACE));
		}
	}


	if (m_Image.m_RGBData.GetMem())
	{
		int x = (Rect.right + Rect.left) / 2 + intRound(m_ImagePos * real_t(Rect.right - Rect.left));

		if (m_ImagePos != 0) x += 3;

		int y = (Rect.bottom + Rect.top) / 2;
		if (m_Pushed) { x++; y++; }

		m_MemDC.DrawImage(m_Image, x, y, 1, 1, m_Enabled);
	}

	if (!m_Text.IsEmpty())
	{
		COLORREF TextColor = m_Enabled ? m_TextColor : 0x00a0a0a0;

		if (m_ImagePos != 0) m_FontSize = intRound(14 * 1.1);

		// For some computer chinese luan ma
		if (G_IsChinese(m_Text))
		{			
			m_FontFace = _T("宋体");
		}
		// For some computer chinese luan ma

		CStringA AFontFace(m_FontFace);
		CMFont Font(m_FontSize, m_FontWidth, AFontFace);
		Rect.right = Rect.left + intRound(real_t(Rect.right - Rect.left)*m_TextPos);

		Rect.left += 4;
		Rect.right -= 4;

		if (m_Pushed || m_Highlighted)
		{
			if (m_TextPos == 1)
			{
				Rect.left += 3;
				Rect.top += 2;
			}
			else
			{
				Rect.left += 1;
				Rect.top += 1;
				Rect.right += 1;
				Rect.bottom += 1;
			}
		}

		CString Lines[8];
		int n = 0;

		CString s(m_Text);
		int l;
		while ((l = s.GetLength()) > 0 && n < 8)
		{
			int l1;
			for (l1 = 1; l1 <= l; l1++)
			{
				RECT Rect1;
				m_MemDC.MeasureRect(s.Left(l1), Rect1, Font);
				if (Rect1.right - Rect1.left > Rect.right - Rect.left)
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

		int dh = (Rect.bottom - Rect.top) / (n + 1);

		Rect.bottom = Rect.top + 2 * dh;
		for (int i = 0; i < n; i++)
		{
			m_MemDC.WriteText(Lines[i], Rect, Font, TextColor, 1);
			Rect.top += dh; Rect.bottom += dh;
		}
	}

	Invalidate(FALSE);
}

//***************************************************************************************
