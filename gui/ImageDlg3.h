//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MGDI.h"
#include "Image.h"

//***************************************************************************************

class CImageDlg3 : public CDialog
{
public:

	CImageDlg3(CWnd* pParentWnd, CEyeImage* pImage, CEyeImage* pCTImage, BOOL WF, BOOL RADIAL_RULER);
	~CImageDlg3();

private:
	static int   m_wheel_n;//520
	int  m_last_wheel_n;//520

	BOOL m_ShowTechInfo;

	BOOL m_InalyDrawed;//whether the inaly has been drawed

					   //HACCEL m_hAccel;

	CMFont   ThisFont;//621;	  

	BOOL m_WF;
	BOOL m_RADIAL_RULER;//[520]
	CEyeImage*  m_pImage;
	CEyeImage   m_Image;
	CEyeImage*  m_pCtImage;//
	CEyeImage   m_Ct_Image;//

	int m_InlayPos[180000];

	real m_x_um[2];
	real m_y_um[2];

	int m_w;
	int m_h;
	int m_l;
	int m_t;
	real m_w_um;
	real m_h_um;
	real m_x_px_um;
	real m_y_px_um;
	real m_cx;
	real m_cy;

	CMDC m_MemDC;
	CRgn m_Rgn;

	int m_P;
	int m_L;
	int m_V;
	int m_R_R;//520 Radial Ruler Click note
	int m_L_R;//520 Linear Ruler Click note
	int m_I;//520  disk click note

	real m_Last_x;//520
	real m_Last_y;//520
	real m_Last_I_R;//520
	real m_Last_I_I_R;//520
	real m_InnerD_Ran;//520

					  // int  m_InsideEmptyWidth;//520
	int  m_InsideWidth;//520
	int  m_OutSideWidth;//520

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

	//520 Edit  disk Radios  
	CButton m_InlayRadio;
	CButton m_Inlay1Radio;
	CButton m_Inlay2Radio;
	CButton m_Inlay3Radio;
	CButton m_Inlay4Radio;
	//Done

	//520 
	RECT    m_InlayText1Rec;
	RECT    m_OtherText1Rec;

	CStatic m_DisText;
	CStatic m_DisText2;
	CStatic m_DisText3;
	CEdit   m_InlayEdit;
	CStatic m_DisText4;
	CStatic m_DisText5;
	CStatic m_DisText6;
	CEdit   m_InlayEdit2;
	CStatic m_Split;//IDC_STATIC1
	CButton m_SetInlayBtn;

	CStatic m_NOTEText;//6.2.1

	CButton m_PupilCheck;
	CButton m_LimbusCheck;
	CButton m_VertexCheck;
	CButton m_RRulerCheck;
	CButton m_LRulerCheck;
	CButton m_InlayCheck;
	//Done

	BOOL IsDigital(CString strItem);//520

	void Resize();

	void Repaint();

	void DrawEye(CMDC& MemDC);
	void DrawPupil(CMDC& MemDC);
	void DrawVertex(CMDC& MemDC);
	void DrawLimbus(CMDC& MemDC);
	void DrawRuler(CMDC& MemDC);//520
	void DrawLRuler(CMDC& MemDC);//520
	void DrawInlay(CMDC& MemDC);//520

	void DisablePupilRadios();
	void DisableLimbusRadios();
	void DisableVertexRadios();
	void DisableRadialRulerRadios();//520
	void DisableLinearRulerRadios();//520
	void DisableInlayRadios();//520

	void  HideInlayInfo();
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

	afx_msg void OnInlayClicked();
	afx_msg void OnInlay1Clicked();
	afx_msg void OnInlay2Clicked();
	afx_msg void OnInlay3Clicked();
	afx_msg void OnInlay4Clicked();
	//Done

	afx_msg void OnSetInalyDBtnClicked();

	virtual BOOL PreTranslateMessage(MSG* pMsg) override;
	afx_msg BOOL OnMouseWheel(uint nFlags, short zDelta, CPoint Point);//520

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
