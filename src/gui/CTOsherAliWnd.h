
//[520]
#pragma once

//***************************************************************************************
#include "MemWnd.h"
#include "Btn.h"
#include "XPMenu.h"
#include "Settings.h"
#include "Data.h"
#include "XPMenu.h"

//***************************************************************************************
class CCTOsherAliWnd : public CMemWnd
{
public:

	CCTOsherAliWnd(CWnd* pParentWnd, RECT& Rect, CCTExam* pCTExam, int ThisMain, int show = SW_SHOW, bool colorImg = false);
   ~CCTOsherAliWnd();

	void RealseMem();

	CCTExam* m_pCTExam;

	virtual void  OnMouseWheel(uint nFlags, short zDelta, CPoint Point);

	int m_wheel_n;
	int m_wheel_n2;

	int m_Screen;//521
	int m_d;//Which window of CT Summary to this 

	int m_ThisMain;//6.2.0

	virtual void CreateMenu();

private:

	virtual void RepaintMemDC() override;

	COLORREF m_LensColor;
	COLORREF m_CaliperColor2;
	COLORREF m_CaliperColor;

	//BOOL    m_SurgeonView;

	int    m_last_wheel_n;
	real_t   m_h_um;
	real_t   m_w_um;
	real_t   m_x_px_um;
	real_t   m_y_px_um;
	real_t   m_cx;
	real_t   m_cy;
	real_t   m_Adx;
	real_t   m_Ady;
	int    m_Movex;
	int    m_Movey;

	CEyeImage*  m_pImage;
	CBasicImage m_pZoomImage2;
	uchar* m_pZoomBackup[15];
	real_t   m_cxBackup[15];	//The x center of the eye image
	real_t   m_cyBackup[15];	//The y center of the eye image
	BOOL   m_BackupYes[15];

	int    m_MoveCalipar;
	int    m_MousePos;

	long   m_ImgSize;

	int    m_a1;
	real_t   m_Ali_Rum1;
	int    m_a2;
	real_t   m_Ali_Rum2;

	int    m_EyeWndTop;
	int    m_EyeWndButtom;
	int    m_EyeWndLeft;
	int    m_EyeWndRight;
	int    m_EyeWndHeight;
	int    m_EyeWndWidth;

	real_t   m_EyeWndRatioH;
	real_t   m_EyeWndRatioW;

	BOOL   m_ShowDymImg;
	BOOL   m_ShowOsherRing;
	BOOL   m_AlignLine;
	BOOL   m_ShowPupil;
	BOOL   m_ShowLimbus;
	BOOL   m_ShowRRuler;
	BOOL   m_ShowLRuler;
	BOOL   m_ShowInlay;//530
	BOOL   m_ShowSnellen;//

	BOOL   m_mouseCap;
	BOOL   m_mouseRealCap;
	CPoint m_CapturePoint;
	CPoint m_MovePoint;

	RECT   m_Ratio1Rect;
	RECT   m_Ratio2Rect;
	RECT   m_Ratio1TextRect;
	RECT   m_Ratio2TextRect;

	RECT   m_PNameTextRect;
	RECT   m_pIDTextRect;
	RECT   m_pODOSTextRect;//531

	RECT   m_LRulerText1Rect;
	RECT   m_LRulerText2Rect;
	RECT   m_RRulerText1Rect;
	RECT   m_RRulerText2Rect;
	RECT   m_InlayText1Rect;//530
	RECT   m_InlayText2Rect;//530
	RECT   m_InlayText3Rect;//530

	CBtn   m_ImgMouseCotrol;
	CBtn   m_RingMouseCotrol;
	//	CBtn m_ExitButton;

	real_t   Distance(real_t x0, real_t y0, real_t x1, real_t y1) { return sqrt((x0 - x1)*(x0 - x1) + (y0 - y1)*(y0 - y1)); }
	void   FindClearLimbus(CEyeImage* OriImage, real_t LastLimbuX, real_t LastLimbuY, real_t LastLimbuR);
	BOOL   FindColorPupil(CEyeImage* OriImage);
	void   SafeInteRelease(int **ppT, int Row);
	double Gaussian(int x, int y, double de);
	HSI    ChangeRGBtoHSI(int R, int G, int B);

	void   CreateZoomImg();
	void   Eye();
	void   Limbus();
	void   Pupil();
	void   RadialRuler();
	void   LinearRuler();
	void   Inlay();//530
	void   Grid();// The degree scale
	void   AlignLine(int a, real_t Ali_Rum, int Type);
	void   AlignLineDeg(int a1, int a2, real_t Ali_Rum1, real_t Ali_Rum2);//The degree between two AlignLine
	void   Points();//vertex(light reflexes)
	void   Labels();

	void OnShow(BOOL* pCheck);

	BOOL OverLapFun(int a0, int a1, int b0, int b1);
	int  SmallAng(int a0, int a1);// get the samller degree difference between a0 and a1

	CXPMenu* m_pMenu;

	int angular_distance_counterclockwise(int a1, int a2);
	int angular_distance(int a1, int a2);

	afx_msg void OnShowDefaultItemClicked();//521

	afx_msg void OnImgMouseButtonClicked();
	afx_msg void OnRingMouseButtonClicked();

	afx_msg void OnRButtonUp(uint nFlags, CPoint Point);
	//afx_msg void OnLButtonDblClk(uint nFlags, CPoint Point);	
	afx_msg void OnLButtonDown(uint nFlags, CPoint Point);
	afx_msg void OnLButtonUp(uint nFlags, CPoint Point);
	afx_msg virtual void OnMouseMove(uint nFlags, CPoint Point);

	afx_msg void OnShowPupilItemClicked() { OnShow(&m_ShowPupil); Repaint(); }
	afx_msg void OnShowLimbusItemClicked() { OnShow(&m_ShowLimbus); Repaint(); }
	afx_msg void OnShowRRulerItemClicked() { OnShow(&m_ShowRRuler); Repaint(); }
	afx_msg void OnShowLRulerItemClicked() { OnShow(&m_ShowLRuler); Repaint(); }
	afx_msg void OnShowInlayItemClicked() { OnShow(&m_ShowInlay); Repaint(); }//530


	afx_msg void OnEditLimbusItemClicked();


	void OnType(int Type);
	afx_msg void OnTypeCEyeItemClicked() { OnType(TYPE_CEYE); }
	afx_msg void OnTypeCAxmItemClicked() { OnType(TYPE_CAXM); }
	afx_msg void OnTypeCTnmItemClicked() { OnType(TYPE_CTNM); }
	afx_msg void OnTypeCRfmItemClicked() { OnType(TYPE_CRFM); }
	afx_msg void OnTypeCElmItemClicked() { OnType(TYPE_CELM); }
	afx_msg void OnTypeCWfmItemClicked() { OnType(TYPE_CWFM); }
	afx_msg void OnTypeCRmsItemClicked() { OnType(TYPE_CRMS); }
	afx_msg void OnTypeCSkmItemClicked() { OnType(TYPE_CSKM); }
	afx_msg void OnTypeSenllenItemClicked() { OnType(TYPE_TLTR); }//

	DECLARE_MESSAGE_MAP()
};
