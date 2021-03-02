//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MGdi.h"
#include "Image.h"
#include "TipWnd.h"

//***************************************************************************************

class CPreBtn : public CWnd
{
public:

	CPreBtn();
	~CPreBtn();

	BOOL IsEnabled() { return m_Enabled; }
	void SetEnabled(const BOOL Enabled) { m_Enabled = Enabled; }
	void SetHighlighted(const BOOL Highlighted) { m_Highlighted = Highlighted; }
	void SetText(const CString& Text) { m_Text = Text; }
	void SetTip(const CString& Tip) { m_Tip = Tip; }
	void SetTextColor(const COLORREF TextColor) { m_TextColor = TextColor; }
	void SetBk(const BOOL DrawBackground) { m_DrawBackground = DrawBackground; }
	void SetBkColor(const COLORREF BkColor) { m_BkColor = BkColor; }
	void SetFont(const int Size, const int Width = 400, const CString& Face = "Arial") { m_FontSize = Size; m_FontWidth = Width; m_FontFace = Face; }
	void SetImage(int ResourceID) { m_Image.LoadFromRes(ResourceID); }
	void SetEdgeColor(const COLORREF EdgeColor) { m_EdgeColor = EdgeColor; }
	void SetTextWidth(const real_t pos) { m_TextPos = pos; }
	void SetImagePos(const real_t pos) { m_ImagePos = pos; }

	void Repaint();

private:

	CMDC m_MemDC;

	real_t m_ImagePos;
	real_t m_TextPos;
	BOOL m_Enabled;
	BOOL m_Highlighted;
	BOOL m_Pushed;
	BOOL m_HasMouse;
	//CString m_Text;
	CString m_Text;
	CString m_Tip;
	COLORREF m_TextColor;
	BOOL m_DrawBackground;
	COLORREF m_BkColor;
	COLORREF m_EdgeColor;
	int m_FontSize;
	int m_FontWidth;
	CString m_FontFace;
	CBasicImage m_Image;

	CTipWnd* m_pTipWnd;
	void ShowTip();
	void HideTip();

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs) override;

	afx_msg void OnPaint();
	afx_msg void OnMouseMove(uint nFlags, CPoint Point);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDown(uint nFlags, CPoint Point);
	afx_msg void OnLButtonUp(uint nFlags, CPoint Point);
	afx_msg void OnShowWindow(BOOL bShow, uint nStatus);

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************

// Windows XP ships with both version 5 and version 6.
// ComCtl32.dll version 6 contains both the user controls and the common controls.
// By default, applications use the user controls defined in User32.dll
// and the common controls defined in ComCtl32.dll version 5.

// If you want your application to use visual styles, you must add an application
// manifest that indicates that ComCtl32.dll version 6 should be used if it is available.
// Version 6 includes some new controls and new options for other controls,
// but the biggest change is support for changing the appearance of controls in a window.

// USER or CLASSIC CONTROLS
// User.exe (16-bit module) or User32.dll

// Buttons          "BUTTON"     CButton 
// List boxes       "LISTBOX"    CListBox 
// Edit controls    "EDIT"       CEdit 
// Combo boxes      "COMBOBOX"   CComboBox 
// Scroll bars      "SCROLLBAR"  CScrollBar 
// Static controls  "STATIC"     CStatic 

// COMMON CONTROLS
// ComCtl32.dll

//***************************************************************************************
