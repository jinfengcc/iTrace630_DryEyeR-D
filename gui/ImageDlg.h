//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MGDI.h"
#include "Image.h"

//***************************************************************************************

class CImageDlg : public CDialog
{
public:

	CImageDlg(CWnd* pParentWnd, CEyeImage* pImage, BOOL WF, BOOL RADIAL_RULER);
	~CImageDlg();

private:
	static int   m_wheel_n;//520
	int  m_last_wheel_n;//520

	BOOL m_ShowTechInfo;

	HACCEL m_hAccel;

	BOOL m_WF;
	BOOL m_RADIAL_RULER;//[520]
	CEyeImage*  m_pImage;
	CEyeImage   m_Image;


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
	int m_R;
	int m_V;

	real m_x_um[2];
	real m_y_um[2];

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

	CButton m_RingsRadio;
	CButton m_Rings1Radio;
	CButton m_Rings2Radio;
	CButton m_Rings3Radio;
	CButton m_Rings4Radio;

	CStatic m_DisText;//520

	void Resize();

	void Repaint();

	void DrawEye(CMDC& MemDC);
	void DrawPupil(CMDC& MemDC);
	void DrawVertex(CMDC& MemDC);
	void DrawLimbus(CMDC& MemDC);
	void DrawRings(CMDC& MemDC);


	void DisablePupilRadios();
	void DisableLimbusRadios();
	void DisableRingsRadios();
	void DisableVertexRadios();


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

	afx_msg void OnRingsRadioClicked();
	afx_msg void OnRings1RadioClicked();
	afx_msg void OnRings2RadioClicked();
	afx_msg void OnRings3RadioClicked();
	afx_msg void OnRings4RadioClicked();


	afx_msg BOOL OnMouseWheel(uint nFlags, short zDelta, CPoint Point);//520

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
