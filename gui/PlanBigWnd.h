
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
class CPlanBigWnd : public CMemWnd
{
public:

	CPlanBigWnd(CWnd* pParentWnd, RECT& Rect, CDicomInfo  DicomInfo, CCTExam* pCTExam);

	void RealseMem();

	CCTExam*  m_pCTExam;

	virtual void  OnMouseWheel(uint nFlags, short zDelta, CPoint Point);

	int m_wheel_n;
	int m_wheel_n2;

private:

	virtual void RepaintMemDC() override;

	COLORREF m_LensColor;
	COLORREF m_CaliperColor2;//[cjf***05032012*005*]
	COLORREF m_CaliperColor;

	//BOOL    m_SurgeonView;

	CDicomInfo  m_DicomInfo;//520 for Dicom file

	int  m_last_wheel_n;
	real m_h_um;
	real m_w_um;
	real m_x_px_um;
	real m_y_px_um;
	real m_cx;
	real m_cy;
	real m_Adx;
	real m_Ady;

	int  m_Movex;
	int  m_Movey;

	//CDC * m_pDC;
	CEyeImage*  m_pImage;
	CBasicImage m_pZoomImage2;
	uchar* m_pZoomBackup[15];
	real   m_cxBackup[15];	//The x center of the eye image
	real   m_cyBackup[15];	//The y center of the eye image
	BOOL   m_BackupYes[15];

	int  m_MoveCalipar;
	int  m_MousePos;

	long m_ImgSize;

	int  m_a1;
	real m_Ali_Rum1;
	int  m_a2;
	real m_Ali_Rum2;
	int  m_a3;
	real m_Ali_Rum3;
	int  m_a4;
	real m_Ali_Rum4;

	int  m_EyeWndTop;
	int  m_EyeWndButtom;
	int  m_EyeWndLeft;
	int  m_EyeWndRight;
	int  m_EyeWndHeight;
	int  m_EyeWndWidth;

	real m_EyeWndRatioH;
	real m_EyeWndRatioW;

	BOOL   m_ShowDymImg;
	BOOL   m_ShowOsherRing;
	BOOL   m_AlignLine;
	BOOL   m_ShowPupil;
	BOOL   m_ShowLimbus;
	BOOL   m_ShowRRuler;//new
	BOOL   m_ShowLRuler;//new

	BOOL   m_ShowCaliper;
	BOOL   m_OverLap;

	BOOL   m_mouseRealCap;
	BOOL   m_mouseCap;
	CPoint m_CapturePoint;
	CPoint m_MovePoint;

	CBtn   m_ImgMouseCotrol;
	CBtn   m_RingMouseCotrol;
	CBtn   m_SaveScreenButton;
	CBtn   m_PrintScreenButton;
	CBtn   m_ZaldivarButton;
	CBtn   m_ExitButton;

	real Distance(real x0, real y0, real x1, real y1) { return sqrt((x0 - x1)*(x0 - x1) + (y0 - y1)*(y0 - y1)); }
	void FindClearLimbus(CEyeImage* OriImage, real LastLimbuX, real LastLimbuY, real LastLimbuR);
	BOOL FindColorPupil(CEyeImage* OriImage);
	void SafeInteRelease(int **ppT, int Row);
	double Gaussian(int x, int y, double de);
	HSI  ChangeRGBtoHSI(int R, int G, int B);

	RECT m_Ratio1Rect;
	RECT m_Ratio2Rect;
	RECT m_Ratio1TextRect;
	RECT m_Ratio2TextRect;

	RECT m_PNameTextRect;
	RECT m_pIDTextRect;
	RECT m_pODOSTextRect;//531

	RECT m_IOLPlaceTextRect;//531

	RECT m_LRulerText1Rect;
	RECT m_LRulerText2Rect;
	RECT m_RRulerText1Rect;
	RECT m_RRulerText2Rect;

	void CreateZoomImg();
	void Eye();
	void Grid();// The degree scale
	void Limbus();
	void Pupil();
	void RadialRuler();
	void LinearRuler();
	void AlignLineDeg(int a1, int a2, real Ali_Rum1, real Ali_Rum2, real Ali_Rum3, real Ali_Rum4, int Type);//531
	void AlignLine(int a, real Ali_Rum, int Type, CString whichLine);//531
	void AlignHoyaLine(int a, real Ali_Rum);//531
	void Caliper();//Caliper
	void Points();//vertex(light reflexes)
	void Labels();

	void OnShow(BOOL* pCheck);

	BOOL OverLapFun(int a0, int a1, int b0, int b1);
	int  SmallAng(int a0, int a1);// get the samller degree difference between a0 and a1

	int  CreateRefAxis(int a0, int a1, int a2);//531 Transform the three protractor's angles to one int according to some rule
	void GetProAngs(int inputV, int &a0, int &a1, int &a2);//531 Transform one int value to the three protractor's angles according to some rule

	CXPMenu* m_pMenu;
	virtual void CreateMenu();

	int angular_distance_counterclockwise(int a1, int a2);
	int angular_distance(int a1, int a2);

	afx_msg void OnImgMouseButtonClicked();
	afx_msg void OnRingMouseButtonClicked();
	afx_msg void OnSaveScreenButtonClicked();
	afx_msg void OnPrintScreenButtonClicked();
	afx_msg void OnZaldivarButtonClicked();
	afx_msg void OnExitButtonClicked();

	afx_msg void OnRButtonUp(uint nFlags, CPoint Point);
	afx_msg void OnLButtonDown(uint nFlags, CPoint Point);
	afx_msg void OnLButtonUp(uint nFlags, CPoint Point);
	afx_msg virtual void OnMouseMove(uint nFlags, CPoint Point);
	afx_msg HBRUSH OnCtlColor(CDC*   pDC, CWnd*   pWnd, UINT   nCtlColor);

	afx_msg void OnShowPupilItemClicked() { OnShow(&m_ShowPupil); Repaint(); }
	afx_msg void OnShowLimbusItemClicked() { OnShow(&m_ShowLimbus); Repaint(); }
	afx_msg void OnShowRRulerItemClicked() { OnShow(&m_ShowRRuler); Repaint(); }
	afx_msg void OnShowLRulerItemClicked() { OnShow(&m_ShowLRuler); Repaint(); }
	afx_msg void OnEditLimbusItemClicked();

	DECLARE_MESSAGE_MAP()
};
