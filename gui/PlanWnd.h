//***************************************************************************************

#pragma once

//***************************************************************************************

#include "DispWnd.h"
#include "Data.h"

//***************************************************************************************

class CPlanWnd : public CDispWnd
{
public:

	CPlanWnd(RECT& Rect, CWnd* pWnd);

	CCTExam* m_pCTExam;

	CMap2D m_Map2D;
	real m_y_px_um;

	BOOL     m_ShowGrayEye;//521
	BOOL     m_ShowColorEye;//521

	BOOL m_ShowEye;
	BOOL m_ShowMap;
	BOOL m_ShowCaliper;
	BOOL m_ShowOsherRing;

	//[cjf***05022012*004*]
	BOOL m_ShowIncision;
	BOOL m_ShowPreopCorneal;
	BOOL m_ShowPreopInternal;
	BOOL m_ShowPreopEntireEye;
	BOOL m_PostopCorneal;
	BOOL m_PostopInternal;
	BOOL m_PostopEntireEye;
	BOOL m_ShowICLIndSteep;
	BOOL m_ShowICLIndFlat;
	//[cjf***05022012*004*]

private:

	//CMDC m_MemDC;  

	static int m_wheel_n;

	COLORREF m_CorneaAxesColor;
	COLORREF m_InternalAxesColor;
	COLORREF m_EntireEyeAxesColor;
	COLORREF m_IncisionColor;
	COLORREF m_ICLSteepAxisColor;
	COLORREF m_ICLFlatAxisColor;
	COLORREF m_LensColor;
	COLORREF m_CaliperColor;
	COLORREF m_CaliperColor2;//[cjf***05032012*005*]
	COLORREF m_RotationColor;

	int m_MousePos;

	BOOL    m_SurgeonView;
	real    m_StepLength;//[cjf***05022012*004*],used to adjust postioion of axes notes
	CString m_Angular_distance;//[cjf***05022012*004*],amount of rotation of IOL enhancement
	BOOL    m_Clock_wise;//[cjf***05022012*004*],direction of rotation of IOL enhancement
	int     m_MoveCalipar;//[cjf***05022012*004*],moveing which caliper point
	BOOL    m_OverLap;//cjf***]
	BOOL    m_Recal_Lens;//[cjf***05052012*008*],whether recalculate the len placemtn

	real m_w_um;
	real m_h_um;
	real m_x_px_um;
	real m_cx;
	real m_cy;

	CEyeImage*  m_pImage;

	virtual void RepaintMemDC() override;
	void Eye();
	void Map();
	void Grid();
	void Lens1();
	void Labels2();
	void CorneaPreopAxes();
	void InternalPreopAxes();
	void EyePreopAxes();
	void Points();//Pupil, limbus centers and vertex(light reflexes)

	void Incision();
	void CorneaPostopAxes();
	void InternalPostopAxes();
	void ICLPostopAxes();
	void EyePostopAxes();
	void Lens2();
	void Caliper();
	void Protractor();//531

	void GetProAngs(int inputV, int &a0, int &a1, int &a2);//531

	BOOL OverLapFun(int a0, int a1, int b0, int b1);//[cjf***05032012*005*] wether two arcs are overlap;
	int  SmallAng(int a0, int a1);// get the samller degree difference between a0 and a1

	void FindClearLimbus(CEyeImage* OriImage, real LastLimbuX, real LastLimbuY, real LastLimbuR);

	CBtn m_ViewButton;
	CBtn m_ZaldivarButton;// [cjf***05022012*004*] 531 from private to public for combo gray it or not
	CBtn m_OSAlignmentButton;// [520]

	CButton m_RecalLenButton;// [cjf***05052012*008*],the combox for the recalculation of lens placement;
	CButton m_ReScale;// [520];
	CBtn m_PrevButton; //[cjf***05072012] 
	CBtn m_NextButton; //[cjf***05072012] 
	afx_msg void OnPrevButtonClicked();//[cjf***05072012] 
	afx_msg void OnNextButtonClicked();//[cjf***05072012] 

	afx_msg void OnViewButtonClicked();
	afx_msg void OnZaldivarButtonClicked();//[cjf***04192012]
	afx_msg void OnRecalLenButtonClicked();//[cjf***05052012*008*]
	afx_msg void OnReScaleButtonClicked();//[520]  
	afx_msg void OnOSAliButtonClicked();//[520]

	afx_msg virtual void OnMouseMove(uint nFlags, CPoint Point) override;
	afx_msg void OnLButtonDown(uint nFlags, CPoint Point);
	afx_msg void OnLButtonUp(uint nFlags, CPoint Point);

	//afx_msg void OnLButtonDblClk(uint nFlags, CPoint Point);//[520]

	virtual void OnMouseWheel(uint nFlags, short zDelta, CPoint Point) override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
