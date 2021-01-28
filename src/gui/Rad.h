//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MGdi.h"
#include "Image.h"

//***************************************************************************************

class CRad : public CWnd
{
public:

	CRad();
	~CRad();

	BOOL IsEnabled() { return m_Enabled; }
	void SetEnabled(const BOOL Enabled) { m_Enabled = Enabled; }

	BOOL GetCheck() { return m_Checked; }
	void SetCheck(const BOOL Checked) { m_Checked = Checked; }

	void SetHighlighted(const BOOL Highlighted) { m_Highlighted = Highlighted; }
	void SetText(const CString& Text) { m_Text = Text; }
	void SetTextColor(const COLORREF TextColor) { m_TextColor = TextColor; }
	void SetFontSize(const int FontSize) { m_FontSize = FontSize; }
	void SetBkColor(const COLORREF BkColor) { m_BkColor = BkColor; }
	void SetFont(const int Size, const int Width = 400, const CString& Face = "Arial") { m_FontSize = Size; m_FontWidth = Width; m_FontFace = Face; }

	void Repaint();

private:

	CMDC m_MemDC;

	BOOL m_Enabled;
	BOOL m_Highlighted;
	BOOL m_Checked;
	BOOL m_HasMouse;
	BOOL m_Pushed;
	CString m_Text;
	COLORREF m_TextColor;
	COLORREF m_BkColor;
	int m_FontSize;
	int m_FontWidth;
	CString m_FontFace;

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
