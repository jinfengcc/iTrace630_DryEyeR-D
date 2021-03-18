//***************************************************************************************

#pragma once

//***************************************************************************************

#include "DispWnd.h"
#include "Data.h"
#include "Units.h"


//***************************************************************************************

class CCircle
{
public:
	real_t m_x_um;
	real_t m_y_um;
	real_t m_r_um;
	COLORREF m_color;
};

//***************************************************************************************

class CEyeWnd : public CDispWnd
{
public:

	CEyeWnd(RECT& Rect, CWnd* pWnd);

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
	BOOL            m_ShowRadialRuler;
	BOOL            m_ShowLinearRuler;
	BOOL            m_ShowInlay;
	BOOL            m_MapShowBLUEAstigmatismAxes;
	BOOL            m_MapShowREDAstigmatismAxes;
	BOOL            m_DonotShow90Degree;
	real_t          m_Cent;
	real_t          m_Step;
	real_t          m_Inc;
	int             m_NumColors;
	COLORREF        m_Colors[21];
	Unit            m_Unit;

	void CreateCentButtons();
	void CreateZoomButtons();

	void CreateSwitchButtons();//6.3.0 For WTT adjusted GUI

	void CreateOKulixButtons();

	void ChangePoint();

	real_t m_y_px_um;

	int m_astigm_axis;

private:

	real_t m_w_um;
	real_t m_h_um;
	real_t m_x_px_um;
	real_t m_cx;
	real_t m_cy;

	real_t m_cursor_r_um;
	real_t m_cursor_a_rd;

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
	void BlueAstigmatismAxes();//
	void RedAstigmatismAxes();//
	void Cursor();
	void SideLabels();

	CBtn m_SwitchButton;//6.3.0 For WTT adjusted GUI

	CBtn m_IncCentButton;
	CBtn m_DecCentButton;

	CBtn m_OkulixButton;//521

	afx_msg void OnIncCentButtonClicked();
	afx_msg void OnDecCentButtonClicked();

	CBtn m_ZoomInButton;
	CBtn m_ZoomOutButton;
	afx_msg void OnZoomInButtonClicked();
	afx_msg void OnZoomOutButtonClicked();
	afx_msg void OnOkulixButtonClicked();//521

	afx_msg virtual void OnMouseMove(uint nFlags, CPoint Point) override;
	afx_msg void OnLButtonDown(uint nFlags, CPoint Point);
	afx_msg void OnLButtonDblClk(uint nFlags, CPoint Point);
	afx_msg LRESULT OnChangePosMsg(WPARAM wParam, LPARAM lParam);

	afx_msg void OnWTTSButtonClicked();//6.3.0 For WTT adjusted GUI

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
