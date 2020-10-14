//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MemWnd.h"
#include "Btn.h"

//***************************************************************************************

class CDispWnd : public CMemWnd
{
public:

	CDispWnd(RECT& Rect, CWnd* pWnd);

	int      m_Type;
	BOOL     m_Method3D;
	BOOL     m_SizeSmall;

	BOOL     m_LTBigFont;//530, Lefttop lable use bigger font

	real     m_LTLabelFontSizePrc;
	CString  m_SIMLTLabel[4];//530 To show big font title of Simplified RMS
	CString  m_LTLabel[5];
	COLORREF m_LTLabelColor;
	COLORREF m_LTLabelBkColor;

	real     m_RTLabelFontSizePrc;
	CString  m_RTLabel[2];
	COLORREF m_RTLabelColor;
	COLORREF m_RTLabelBkColor;

	real     m_RBLabelFontSizePrc;
	CString  m_RBLabel[6];
	COLORREF m_RBLabelColor[6];
	COLORREF m_RBLabelBkColor;

	//601 05202015
	//Add a warning to the DLI panel that indicates if the corneal total HOAs are over a
	//threshold vale to alert the user that the DLI could be influenced by high corneal HOAs.
	//"HIGH CORNEAL ABERRATIONS" in yellow ( be sure to use the printable yellow for the print screen view)
	//when corneal total HOAs >= 0.280 < 0.450
	//in red when corneal total HOAs >= 0.450  
	real     m_TMLabelFontSizePrc;//TM means top midle
	CString  m_TMLabel;
	COLORREF m_TMLabelColor;
	COLORREF m_TMLabelBkColor;
	//601 05202015 Done    

	//520
	CString  m_LBLabel[11];
	COLORREF m_LBLabelColor[11];
	//520

	BOOL     m_CaptionVertical;

	int      m_ax;
	int      m_ay;

	CBtn m_IncZoneButton;
	CBtn m_DecZoneButton;

	CBtn m_ShowMoreButton;

	void CreateSimpButtons(BOOL ShowDetails);

	void CreateZoneButtons();

	void CreateZoneButtons2();

	void CreateSoloMaskButtons(int makeType, int pos);

	virtual void OnMouseWheel(uint nFlags, short zDelta, CPoint Point) {} // not a message handler

protected:

	static const PIXELFORMATDESCRIPTOR m_PixelFormatDescriptor;

	int      m_l;

	void Labels();

	afx_msg virtual void OnMouseMove(uint nFlags, CPoint Point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	CBtn m_SoloMaskButton;

	afx_msg void OnIncZoneButtonClicked();
	afx_msg void OnDecZoneButtonClicked();
	afx_msg void OnSoloMaskButtonClicked();

private:

	int      m_mx;
	int      m_my;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
