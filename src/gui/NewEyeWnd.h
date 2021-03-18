//***************************************************************************************

#pragma once

//***************************************************************************************

#include "DispWnd.h"
#include "Data.h"
#include "Units.h"

//***************************************************************************************

class CNewEyeWnd : public CDispWnd
{
public:

	CNewEyeWnd(RECT& Rect, CWnd* pWnd, int Scale);

	CWFExam*        m_pWFExam;
	CCTExam*        m_pCTExam;

	Matrix<CCircle> m_Circles;
	CMap2D          m_Map2D;
	CMap3D          m_Map3D;
	BOOL            m_MapShowEye;
	BOOL            m_MapShowMap;
	BOOL            m_MapShowPupil;
	BOOL            m_MapShowCornealVertex;
	BOOL            m_MapShowWavetouchLens;
	BOOL            m_MapShowPoints;
	BOOL            m_MapShowRings;
	BOOL            m_MapShowLimbus;
	BOOL            m_MapShowSolidSurface;
	BOOL            m_MapShowWireMesh;
	BOOL            m_MapShowNumbers;
	BOOL            m_MapShowKeratometry;
	BOOL            m_MapShowSimKAxes;
	BOOL            m_MapShowAstigmatismAxes;
	BOOL            m_MapTranslucent;
	BOOL            m_ShowRadialRuler;//520
	BOOL            m_ShowLinearRuler;//520
	BOOL            m_ShowInlay;//520
	real_t          m_Cent;
	real_t          m_Step;
	real_t          m_Inc;
	int             m_NumColors;
	COLORREF        m_Colors[21];
	Unit            m_Unit;

	void CreateCentButtons();
	void CreateZoomButtons();

	void ChangePoint();

	real_t m_x_px_um;
	real_t m_y_px_um;

	int m_astigm_axis;

private:

	real_t m_w_um;
	real_t m_h_um;
	real_t m_cx;
	real_t m_cy;

	real_t m_cursor_r_um;
	real_t m_cursor_a_rd;

	int  m_Scale;//521
	real_t m_ScaleRatio;//521

	virtual void RepaintMemDC() override;
	void Eye();
	void Legend();
	void Map();
	void Numbers();
	void Surface();
	void Grid();
	void Pupil();
	void RadialRuler();//520
	void LinearRuler();//520
	void Inlay();//520
	void Lens();
	void Vertex();
	void Points();
	void CurrentPoint();
	void Rings();
	void Limbus();
	void Circles();
	void Keratometry();
	void SimKAxes();
	void AstigmatismAxes();
	void Cursor();
	void SideLabels();

	CBtn m_IncCentButton;
	CBtn m_DecCentButton;
	afx_msg void OnIncCentButtonClicked();
	afx_msg void OnDecCentButtonClicked();

	CBtn m_ZoomInButton;
	CBtn m_ZoomOutButton;
	afx_msg void OnZoomInButtonClicked();
	afx_msg void OnZoomOutButtonClicked();

	afx_msg virtual void OnMouseMove(uint nFlags, CPoint Point) override;
	afx_msg void OnLButtonDown(uint nFlags, CPoint Point);
	afx_msg void OnLButtonDblClk(uint nFlags, CPoint Point);
	afx_msg LRESULT OnChangePosMsg(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
