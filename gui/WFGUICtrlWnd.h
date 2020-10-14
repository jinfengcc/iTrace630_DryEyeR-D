//WF controller window

//***************************************************************************************

#pragma once

//***************************************************************************************
#include "MemWnd.h"
#include "Onoff.h"
#include "Rad.h"
#include "Btn.h"
#include "SelBtn.h"
#include "Sel3Btn.h"//Presbia

//***************************************************************************************

class CWFGUICtrlWnd : public CMemWnd
{
public:
	CWFGUICtrlWnd();
	~CWFGUICtrlWnd();

	void  CreateThisWnd(RECT& Rect, CWnd* pWnd, int CapWidth);

	void  ShowGUI();
	void  HideGUI();
	void  HideAliMethod();

	BOOL  m_ShowWindow;

	BOOL  m_WFORCT;//0:WF; 1:CT;

	BOOL  m_TargetOn;
	int   m_AutoMode;//Change from BOOL to int; 1: Automode, 2: Manual full scan ; 3: Manual 64 scan
	BOOL  m_PupilCen;

	BOOL  m_ShowScanRing;

	void  RepaintAuto();
	void  RepaintPupilCen();
	void  RepaintTarget();

	BOOL  m_ShowAcqBtn;
	BOOL  m_Acquire;

	void  ShowAcqBtnGUI();
	void  HideAcqBtnGUI();

	void SetScanRing(BOOL ScanRing);

private:

	int     m_w;
	int     m_h;
	int     m_FontSize;

	BOOL    m_Type;

	CBtn     m_CapBtn; //Acqurie btn

	CBtn     m_ShowScanBtn;
	CSel3Btn m_PresbiaOnOff;//Presbia

	CSelBtn  m_OnOff[2];
	CSel3Btn m_AutoManual;

	RECT    m_Presbia_Onoff_LabelRect;//Presbia
	RECT    m_Presbia_OnoffRect;//Presbia
	RECT    m_OnoffRect[3];
	RECT    m_LabelRect[3];

	RECT    m_Bk_OnoffRect[2];
	RECT    m_Bk_BtnRect;
	RECT    m_Bk_LabelRect[2];

	RECT    m_CT_OnoffRect;
	RECT    m_CT_LabelRect;

	void    Label();
	void    CTLabel();

	afx_msg LRESULT OnOnOffChangedPos(WPARAM wParam, LPARAM lParam);

	afx_msg void OnShowScanBtnClicked();

	afx_msg void OnCaptureBtnClicked();

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
