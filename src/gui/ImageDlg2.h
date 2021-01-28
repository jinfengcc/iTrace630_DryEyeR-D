//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MGDI.h"
#include "Image.h"
#include "Settings.h"

//***************************************************************************************

class CImageDlg2 : public CDialog
{
public:

	CImageDlg2(CWnd* pParentWnd, CEyeImage* pImage, BOOL Gray, BOOL RADIAL_RULER);
	~CImageDlg2();

private:
	static int   m_wheel_n;//520
	int  m_last_wheel_n;//520

	BOOL m_ShowTechInfo;

	HACCEL m_hAccel;

	BOOL m_Gray;
	BOOL m_RADIAL_RULER;//[520]
	CEyeImage*  m_pImage;
	CEyeImage   m_Image;

	real_t m_x_um[2];
	real_t m_y_um[2];

	int  m_w;
	int  m_h;
	int  m_l;
	int  m_t;
	real_t m_w_um;
	real_t m_h_um;
	real_t m_x_px_um;
	real_t m_y_px_um;
	real_t m_cx;
	real_t m_cy;

	CMDC m_MemDC;
	CRgn m_Rgn;

	int m_P;
	int m_L;
	int m_V;
	int m_R_R;//520 Radial Ruler Click note
	int m_L_R;//520 Linear Ruler Click note

	CButton m_OKButton;
	CButton m_CancelButton;

	CButton m_PupilRadio;
	CButton m_Pupil1Radio;
	CButton m_Pupil2Radio;
	CButton m_Pupil3Radio;
	CButton m_Pupil4Radio;

	CButton m_LimbusRadio;
	CButton m_Limbus1Radio;
	CButton m_Limbus2Radio;
	CButton m_Limbus3Radio;
	CButton m_Limbus4Radio;

	CButton m_VertexRadio;
	CButton m_Vertex1Radio;
	CButton m_Vertex2Radio;
	CButton m_Vertex3Radio;
	CButton m_Vertex4Radio;

	//520 Edit Radial Ruler Radios
	CButton m_RadialRulerRadio;
	CButton m_RadialRuler1Radio;
	CButton m_RadialRuler2Radio;
	CButton m_RadialRuler3Radio;
	//Done

	//520 Edit Linear Ruler Radios
	CButton m_LinearRulerRadio;
	CButton m_LinearRuler1Radio;
	CButton m_LinearRuler2Radio;
	CButton m_LinearRuler3Radio;
	//Done

	//530 Edit  disk Radios  
	CButton m_InlayRadio;
	CButton m_Inlay1Radio;
	CButton m_Inlay2Radio;
	CButton m_Inlay3Radio;
	CButton m_Inlay4Radio;
	//Done

	//520 
	CStatic m_DisText;
	CStatic m_Split;//IDC_STATIC1

	CButton m_PupilCheck;
	CButton m_LimbusCheck;
	CButton m_VertexCheck;
	CButton m_RRulerCheck;
	CButton m_LRulerCheck;
	//Done

	void Resize();

	void Repaint();

	void DrawEye(CMDC& MemDC);
	void DrawPupil(CMDC& MemDC);
	void DrawVertex(CMDC& MemDC);
	void DrawLimbus(CMDC& MemDC);
	void DrawRuler(CMDC& MemDC);//520
	void DrawLRuler(CMDC& MemDC);//520

	void DisablePupilRadios();
	void DisableLimbusRadios();
	void DisableVertexRadios();
	void DisableRadialRulerRadios();//520
	void DisableLinearRulerRadios();//520

	virtual BOOL PreTranslateMessage(MSG* pMsg) override;

	afx_msg void OnCtrlD();

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;

	afx_msg void OnPaint();

	afx_msg void OnMouseMove(uint nFlags, CPoint Point);
	afx_msg void OnLButtonDown(uint nFlags, CPoint Point);
	afx_msg void OnLButtonUp(uint nFlags, CPoint Point);//520

	afx_msg void OnPupilRadioClicked();
	afx_msg void OnPupil1RadioClicked();
	afx_msg void OnPupil2RadioClicked();
	afx_msg void OnPupil3RadioClicked();
	afx_msg void OnPupil4RadioClicked();

	afx_msg void OnLimbusRadioClicked();
	afx_msg void OnLimbus1RadioClicked();
	afx_msg void OnLimbus2RadioClicked();
	afx_msg void OnLimbus3RadioClicked();
	afx_msg void OnLimbus4RadioClicked();

	afx_msg void OnVertexRadioClicked();
	afx_msg void OnVertex1RadioClicked();
	afx_msg void OnVertex2RadioClicked();
	afx_msg void OnVertex3RadioClicked();
	afx_msg void OnVertex4RadioClicked();


	//[520]
	afx_msg void OnRadialRulerClicked();
	afx_msg void OnRadialRuler1Clicked();
	afx_msg void OnRadialRuler2Clicked();
	afx_msg void OnRadialRuler3Clicked();

	afx_msg void OnLinearRulerClicked();
	afx_msg void OnLinearRuler1Clicked();
	afx_msg void OnLinearRuler2Clicked();
	afx_msg void OnLinearRuler3Clicked();
	//Done

	real_t Distance(real_t x0, real_t y0, real_t x1, real_t y1) { return sqrt((x0 - x1)*(x0 - x1) + (y0 - y1)*(y0 - y1)); }
	BOOL FindColorPupil(CEyeImage* OriImage);
	void SafeInteRelease(int **ppT, int Row);
	double Gaussian(int x, int y, double de);
	HSI  ChangeRGBtoHSI(int R, int G, int B);

	afx_msg BOOL OnMouseWheel(uint nFlags, short zDelta, CPoint Point);//520

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
