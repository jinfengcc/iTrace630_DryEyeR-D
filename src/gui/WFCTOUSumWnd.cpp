//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "WFCTOUSumWnd.h"
#include "EyeWnd.h"
#include "NewEyeWnd.h"//521
#include "PSFWnd.h"
#include "MTFWnd.h"
#include "EEFWnd.h"
#include "LtrWnd.h"
#include "RMSWnd.h"
#include "ChartWnd.h"
#include "BusyCursor.h"
#include "GlobalFunctions.h"//

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CWFCTOUSumWnd, CSumWnd)

	ON_MESSAGE(WM_CHANGE_CENT, OnChangeCentMsg)
	ON_MESSAGE(WM_CHANGE_STEP, OnChangeStepMsg)

	ON_COMMAND(IDC_DEFAULT_GUI_ITEM, OnShowDefaultItemClicked)//521

	ON_COMMAND(IDC_TYPE_CORNEAL_ITEM, OnCornealItemClicked)//530  

END_MESSAGE_MAP()

//***************************************************************************************

CWFCTOUSumWnd::CWFCTOUSumWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CWFExam* pWFExam1, CWFExam* pWFExam2, CCTExam* pCTExam1, CCTExam* pCTExam2, CWndSettings* pWndSettings, int show) :
	CSumWnd(pWnd, WndRect, pPatient, pWndSettings)
{
	CBusyCursor Cursor;

	//----------------------------------------------------
	RECT Rect;
	GetWindowRect(&Rect);
	real_t t = 3.0 * (0.03 * m_h) + m_g;
	real_t w2 = ((Rect.right - Rect.left) - 5.0 * m_g) / 6.0;
	real_t h2 = ((Rect.bottom - Rect.top - t) - 4.0 * m_g) / 4.0;
	real_t moveH = 3.0*m_g;

	real_t t1 = t + moveH;
	real_t w = w2 + w2;
	real_t h = h2 + m_g + h2;

	real_t h3 = 0.4 * h;
	real_t h4 = h - h3;

	m_right = intRound(m_g + w + m_g + w2 + m_g + w + m_g + w2);
	m_cen = intRound(m_g + w + m_g + w2 + m_g / 2);
	m_btm = intRound(t1 + h + m_g + h - moveH);

	::SetRect(&m_TitleRect[0], intRound(m_g), intRound(t - 2 * m_g), intRound(m_g + w + m_g + w2), intRound(t1 - m_g)); // Title "OD"
	::SetRect(&m_TitleRect[1], intRound(m_g + w + m_g + w2 + m_g), intRound(t - 2 * m_g), intRound(m_g + w + m_g + w2 + m_g + w + m_g + w2), intRound(t1 - m_g)); // Title "OS"

	::SetRect(&m_Rect[0], intRound(m_g + w2 + m_g), intRound(t1), intRound(m_g + w + m_g + w2), intRound(t1 + h)); // exam 1 map
	::SetRect(&m_Rect[1], intRound(m_g), intRound(t1), intRound(m_g + w2), intRound(t1 + h2)); // exam 1 psf
	::SetRect(&m_Rect[2], intRound(m_g), intRound(t1 + h2 + m_g), intRound(m_g + w2), intRound(t1 + h)); // exam 1 letter

	::SetRect(&m_Rect[3], intRound(m_g + w + m_g + w2 + m_g + w2 + m_g), intRound(t1), intRound(m_g + w + m_g + w2 + m_g + w2 + m_g + w), intRound(t1 + h)); // exam 2 map
	::SetRect(&m_Rect[4], intRound(m_g + w + m_g + w2 + m_g), intRound(t1), intRound(m_g + w + m_g + w2 + m_g + w2), intRound(t1 + h2)); // exam 2 psf
	::SetRect(&m_Rect[5], intRound(m_g + w + m_g + w2 + m_g), intRound(t1 + h2 + m_g), intRound(m_g + w + m_g + w2 + m_g + w2), intRound(t1 + h)); // exam 2 letter

	::SetRect(&m_Rect[6], intRound(m_g), intRound(t1 + h + m_g), intRound(m_g + w), intRound(t1 + h + h3));// WF exam 1 info
	::SetRect(&m_InfoRect[0], intRound(m_g), intRound(t1 + h + h3 + m_g), intRound(m_g + w), intRound(t1 + h + m_g + h - moveH)); // CT exam 1 info

	::SetRect(&m_Rect[7], intRound(m_g + w + m_g + w2 + m_g), intRound(t1 + h + m_g), intRound(m_g + w + m_g + w2 + m_g + w), intRound(t1 + h + h3));       // WF exam 2 info
	::SetRect(&m_InfoRect[1], intRound(m_g + w + m_g + w2 + m_g), intRound(t1 + h + h3 + m_g), intRound(m_g + w + m_g + w2 + m_g + w), intRound(t1 + h + m_g + h - moveH)); // CT exam 2 info

	::SetRect(&m_Rect[8], intRound(m_g + w + m_g), intRound(t1 + h + m_g), intRound(m_g + w + m_g + w2), intRound(t1 + h + h3)); // exam 1 WF complaints
	::SetRect(&m_Rect[9], intRound(m_g + w + m_g + w2 + m_g + w + m_g), intRound(t1 + h + m_g), intRound(m_g + w + m_g + w2 + m_g + w + m_g + w2), intRound(t1 + h + h3)); // exam 2 WF complaints

	::SetRect(&m_ComplaintRect[0], intRound(m_g + w + m_g), intRound(t1 + h + h3 + m_g), intRound(m_g + w + m_g + w2), intRound(t1 + h + m_g + h - moveH)); // exam 1 CT complaints
	::SetRect(&m_ComplaintRect[1], intRound(m_g + w + m_g + w2 + m_g + w + m_g), intRound(t1 + h + h3 + m_g), intRound(m_g + w + m_g + w2 + m_g + w + m_g + w2), intRound(t1 + h + m_g + h - moveH)); // exam 2 CT complaints
																																																	  //----------------------------------------------------

	m_pWFExam1 = NULL;
	m_pWFExam2 = NULL;
	m_pCTExam1 = NULL;
	m_pCTExam2 = NULL;
	if (pWFExam1)
	{
		if (pWFExam1->m_Header.IsOD())
		{
			m_pWFExam1 = pWFExam1;
			m_pWFExam2 = pWFExam2;

			m_pCTExam1 = pCTExam1;
			m_pCTExam2 = pCTExam2;
		}
		else
		{
			m_pWFExam1 = pWFExam2;
			m_pWFExam2 = pWFExam1;

			m_pCTExam1 = pCTExam2;
			m_pCTExam2 = pCTExam1;
		}
	}

	m_OUSumWnd = TRUE;//520

					  //*007*[cjf***05052012],record the increase and decrease range
	int invalidValue = 0;
	real_t m_r1 = 0;
	real_t m_r2 = 0;
	if (m_pWFExam1 == NULL) invalidValue = 1;
	else m_r1 = m_pWFExam1->m_WfSurface.m_r_max_um;

	if (m_pWFExam2 == NULL) invalidValue = (invalidValue == 0 ? 2 : 3);
	else m_r2 = m_pWFExam2->m_WfSurface.m_r_max_um;

	switch (invalidValue)
	{
	case 0:
		::TempSettings.Com_r_max_um = (m_r1< m_r2 ? m_r1 : m_r2); break;
	case 1:
		::TempSettings.Com_r_max_um = m_r2; break;
	case 2:
		::TempSettings.Com_r_max_um = m_r1; break;
	case 3:
		::TempSettings.Com_r_max_um = 5000; break;
	}
	//*007*[cjf***05052012]  

	//520
	if (m_pWFExam1 != NULL) ::TempSettings.Com_r_max_um1 = m_r1;
	if (m_pWFExam2 != NULL) ::TempSettings.Com_r_max_um2 = m_r2;
	//520

	//521, to find the minimum border for refraction/wafefront border
	m_Scale = GetGridScale(m_r1, m_r2);
	//521

	// WFCT Angle k/a adjust wf eye limbus by ct info
	if (m_pWFExam1 && m_pCTExam1)
	{
		LimbusAdjust(m_pWFExam1, m_pCTExam1);
		m_pWFExam1->CalcAngles();
	}
	if (m_pWFExam2 && m_pCTExam2)
	{
		LimbusAdjust(m_pWFExam2, m_pCTExam2);
		m_pWFExam2->CalcAngles();
	}
	// WFCT Angle k/a adjust wf eye limbus by ct info Done

	CreateChildWnd();

	Repaint();

	ShowWindow(show);
}

//***************************************************************************************

void CWFCTOUSumWnd::RepaintMemDC()
{
	CSumWnd::RepaintMemDC();

	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	CMFont Font(intRound(4.5 * m_g), 400, "Arial");
	m_MemDC.WriteText("OD", m_TitleRect[0], Font, white, 1);
	m_MemDC.WriteText("OS", m_TitleRect[1], Font, white, 1);

	if (m_pWFExam1) {
		OUWFExamInfo(m_pWFExam1, m_Rect[6]);
		OUCTExamInfo(m_pCTExam1, m_InfoRect[0]);
		VisualComplaints(m_pWFExam1, m_Rect[8], 8);
		VisualCTComplaints(m_pWFExam1, m_ComplaintRect[0], 10);
	}

	if (m_pWFExam2) {
		OUWFExamInfo(m_pWFExam2, m_Rect[7]);
		OUCTExamInfo(m_pCTExam2, m_InfoRect[1]);
		VisualComplaints(m_pWFExam2, m_Rect[9], 8);
		VisualCTComplaints(m_pWFExam2, m_ComplaintRect[1], 10);
	}

	//530 show the highlight outline 
	{
		COLORREF white = m_Printing ? BLACK : WHITE;

		m_MemDC.DrawLine(intRound(m_g), m_TitleRect[0].top, m_right, m_TitleRect[0].top, 2, white);

		m_MemDC.DrawLine(m_cen, m_TitleRect[0].top, m_cen, m_btm, 2, white);
	}
	//530 Done
}

//***************************************************************************************

void CWFCTOUSumWnd::CreateChildWnd()
{
	for (int d = 0; d < 6; d++) SAFE_DELETE(m_pDispWnd[d]);

	//Set the setting of first big ct dispwnd: m_pDispWnd[0] and m_pDispWnd[3]
	if (m_pWndSettings[0].m_Type != TYPE_CEYE &&
		m_pWndSettings[0].m_Type != TYPE_CAXM &&
		m_pWndSettings[0].m_Type != TYPE_CTNM &&
		m_pWndSettings[0].m_Type != TYPE_CRFM &&
		m_pWndSettings[0].m_Type != TYPE_CELM &&
		m_pWndSettings[0].m_Type != TYPE_CWFM &&
		m_pWndSettings[0].m_Type != TYPE_CRMS &&
		m_pWndSettings[0].m_Type != TYPE_CSKM)
	{
		m_pWndSettings[0].m_Type = TYPE_CAXM;//Axial Map
	}

	if (m_pWndSettings[0].m_Type == TYPE_CRMS)
	{
		if (m_pWndSettings[0].m_Method3D)
		{
			m_pWndSettings[0].m_RMSIndividual = TRUE;
		}
	}

	if (m_pWndSettings[0].m_Type == TYPE_CSKM)
	{
		m_pWndSettings[0].m_Method3D = FALSE;
	}



	//Set the setting of left four small wf dispwnd: m_pDispWnd[1] m_pDispWnd[2] and m_pDispWnd[4] m_pDispWnd[5]
	if (m_pWndSettings[1].m_Type != TYPE_TWFM) m_pWndSettings[1].m_Type = TYPE_TRFM;

	//530
	if (m_pWndSettings[2].m_Type != TYPE_TLTR && m_pWndSettings[2].m_Type != TYPE_TPSF
		&& m_pWndSettings[2].m_Type != TYPE_CLTR && m_pWndSettings[2].m_Type != TYPE_CPSF)
	{
		m_pWndSettings[2].m_Type = TYPE_CLTR;
	}
	//530

	{
		CWndSettings *pWndSettings = &m_pWndSettings[2];
		if (!(pWndSettings->m_Ltr == 0 || pWndSettings->m_Ltr == 1)) {
			pWndSettings->m_Ltr = 0;
		}
		if (!(pWndSettings->m_LtrLine == 20 || pWndSettings->m_LtrLine == 25 || pWndSettings->m_LtrLine == 30 ||
			pWndSettings->m_LtrLine == 40 || pWndSettings->m_LtrLine == 50 || pWndSettings->m_LtrLine == 70 ||
			pWndSettings->m_LtrLine == 100 || pWndSettings->m_LtrLine == 200)) {
			pWndSettings->m_LtrLine = 70;
		}
		if (!(pWndSettings->m_LtrOrientation == 0 || pWndSettings->m_LtrOrientation == 90 ||
			pWndSettings->m_LtrOrientation == 180 || pWndSettings->m_LtrOrientation == 270)) {
			pWndSettings->m_LtrOrientation = 0;
		}
	}


	//new
	int  Greater;
	int  r_min_max;

	if (m_pWFExam1 && m_pWFExam2)
	{
		if (m_pWFExam1->m_WfSurface.m_r_max_um > m_pWFExam2->m_WfSurface.m_r_max_um)
		{
			Greater = 1;
			r_min_max = intRound(m_pWFExam2->m_WfSurface.m_r_max_um);
		}
		else if (m_pWFExam1->m_WfSurface.m_r_max_um < m_pWFExam2->m_WfSurface.m_r_max_um)
		{
			Greater = 2;
			r_min_max = intRound(m_pWFExam1->m_WfSurface.m_r_max_um);
		}
		else
		{
			Greater = 0;
			r_min_max = intRound(m_pWFExam1->m_WfSurface.m_r_max_um);
		}
	}
	else if (m_pWFExam1 && !m_pWFExam2)
	{
		Greater = 0;
		r_min_max = intRound(m_pWFExam1->m_WfSurface.m_r_max_um);
	}
	else if (m_pWFExam2 && !m_pWFExam1)
	{
		Greater = 0;
		r_min_max = intRound(m_pWFExam2->m_WfSurface.m_r_max_um);
	}


	if (m_Togther != TRUE && m_Togther != FALSE)
	{
		for (int d = 1; d < 3; d++)
		{
			if (m_pWFExam1) m_pWndSettings[d].m_ZoneRadiusUm = intRound(m_pWFExam1->m_WfSurface.m_r_max_um);
		}
		for (int d = 4; d < 6; d++)
		{
			if (m_pWFExam2) m_pWndSettings[d].m_ZoneRadiusUm = intRound(m_pWFExam2->m_WfSurface.m_r_max_um);
		}
	}

	m_Togther = TRUE;// Increase or decrease togther  

	if (Greater != 0)
	{
		if (m_pWndSettings[4].m_ZoneRadiusUm != 0)
		{
			if (m_d == 0 || m_d == 3)
			{
				for (int d = 1; d < 6; d++)
				{
					m_pWndSettings[d].m_Mask = m_pWndSettings[0].m_Mask;
					m_pWndSettings[d].m_Piston = m_pWndSettings[0].m_Piston;
					m_pWndSettings[d].m_Tilt = m_pWndSettings[0].m_Tilt;
				}

				if (Greater == 1) m_d = 1;
				else if (Greater == 2) m_d = 4;
			}

			if (m_pWndSettings[m_d].m_ZoneRadiusUm <= r_min_max)
				m_Togther = TRUE;
			else
			{
				m_Togther = FALSE;

				for (int d = 1; d < 3; d++)
				{
					// size
					m_pWndSettings[d].m_SizeSmall = TRUE;
					// method
					m_pWndSettings[d].m_Method3D = FALSE;
					if (d != m_d)
					{
						// zone
						if (Greater == 1)
						{
							m_pWndSettings[d].m_ZoneRadiusUm = m_pWndSettings[m_d].m_ZoneRadiusUm;
						}

						// mask
						m_pWndSettings[d].m_Mask = m_pWndSettings[m_d].m_Mask;
						m_pWndSettings[d].m_Piston = m_pWndSettings[m_d].m_Piston;
						m_pWndSettings[d].m_Tilt = m_pWndSettings[m_d].m_Tilt;
					}
				}

				for (int d = 4; d < 6; d++)
				{
					// size
					m_pWndSettings[d].m_SizeSmall = TRUE;
					// method
					m_pWndSettings[d].m_Method3D = FALSE;
					if (d != m_d)
					{
						// zone
						if (Greater == 2)
						{
							m_pWndSettings[d].m_ZoneRadiusUm = m_pWndSettings[m_d].m_ZoneRadiusUm;
						}

						// mask
						m_pWndSettings[d].m_Mask = m_pWndSettings[m_d].m_Mask;
						m_pWndSettings[d].m_Piston = m_pWndSettings[m_d].m_Piston;
						m_pWndSettings[d].m_Tilt = m_pWndSettings[m_d].m_Tilt;
					}
				}
			}
		}

		else
		{
			for (int d = 1; d < 3; d++)
			{
				if (d != 1)
				{
					m_pWndSettings[d].m_Mask = m_pWndSettings[1].m_Mask;
					m_pWndSettings[d].m_Piston = m_pWndSettings[1].m_Piston;
					m_pWndSettings[d].m_Tilt = m_pWndSettings[1].m_Tilt;
				}
			}
			for (int d = 4; d < 6; d++)
			{
				if (d != 1)
				{
					m_pWndSettings[d].m_Mask = m_pWndSettings[1].m_Mask;
					m_pWndSettings[d].m_Piston = m_pWndSettings[1].m_Piston;
					m_pWndSettings[d].m_Tilt = m_pWndSettings[1].m_Tilt;
				}
			}
		}
	}

	if (m_Togther)
	{
		for (int d = 1; d < 3; d++)
		{
			// size
			m_pWndSettings[d].m_SizeSmall = TRUE;
			// method
			m_pWndSettings[d].m_Method3D = FALSE;
			if (d != m_d) {
				// zone
				m_pWndSettings[d].m_ZoneRadiusUm = m_pWndSettings[m_d].m_ZoneRadiusUm;

				// mask
				m_pWndSettings[d].m_Mask = m_pWndSettings[m_d].m_Mask;
				m_pWndSettings[d].m_Piston = m_pWndSettings[m_d].m_Piston;
				m_pWndSettings[d].m_Tilt = m_pWndSettings[m_d].m_Tilt;
			}
		}

		for (int d = 4; d < 6; d++)
		{
			// size
			m_pWndSettings[d].m_SizeSmall = TRUE;
			// method
			m_pWndSettings[d].m_Method3D = FALSE;
			if (d != m_d) {
				// zone
				m_pWndSettings[d].m_ZoneRadiusUm = m_pWndSettings[m_d].m_ZoneRadiusUm;

				// mask
				m_pWndSettings[d].m_Mask = m_pWndSettings[m_d].m_Mask;
				m_pWndSettings[d].m_Piston = m_pWndSettings[m_d].m_Piston;
				m_pWndSettings[d].m_Tilt = m_pWndSettings[m_d].m_Tilt;
			}
		}
	}

	real_t ar_max_um = 5000;
	real_t ar_max_um1, ar_max_um2;

	if (m_pWFExam1) ar_max_um1 = m_pWFExam1->m_WfSurface.m_r_max_um;
	if (m_pWFExam2) ar_max_um2 = m_pWFExam2->m_WfSurface.m_r_max_um;

	if (m_pWFExam1) ar_max_um1 = __min(ar_max_um1, m_pWndSettings[1].m_ZoneRadiusUm);
	if (m_pWFExam2) ar_max_um2 = __min(ar_max_um2, m_pWndSettings[4].m_ZoneRadiusUm);

	if (m_pWFExam1) ar_max_um = __min(ar_max_um, m_pWFExam1->m_WfSurface.m_r_max_um);
	if (m_pWFExam2) ar_max_um = __min(ar_max_um, m_pWFExam2->m_WfSurface.m_r_max_um);
	ar_max_um = __min(ar_max_um, m_pWndSettings[1].m_ZoneRadiusUm);


	//new
	if (m_Togther)
	{
		if (ar_max_um == r_min_max)
			m_TogtherEdge = TRUE;
		else m_TogtherEdge = FALSE;
	}
	else m_TogtherEdge = FALSE;
	//Done
	//New Done

	for (int e = 0; e < 2; e++)
	{

		CWFExam* pWFExam = e == 0 ? m_pWFExam1 : m_pWFExam2;

		CCTExam* pCTExam = e == 0 ? m_pCTExam1 : m_pCTExam2;//530

		if (pWFExam == NULL) continue;


		CZernikeSurface Surface, CornealSurface;

		Surface = pWFExam->m_WfSurface;
		CornealSurface = pCTExam->m_WfSurface;//530

											  //New
		if (m_Togther)
		{
			Surface.ChangeRMaxUm(ar_max_um);
			CornealSurface.ChangeRMaxUm(ar_max_um);//530
		}
		else
		{
			if (e == 0)
			{
				Surface.ChangeRMaxUm(ar_max_um1);
				CornealSurface.ChangeRMaxUm(ar_max_um1);//530
			}
			else
			{
				Surface.ChangeRMaxUm(ar_max_um2);
				CornealSurface.ChangeRMaxUm(ar_max_um2);//530
			}
		}
		//Done
		//Surface.ChangeRMaxUm(r_max_um);

		Surface &= m_pWndSettings[1].m_Mask;
		Surface.ComputePiston(m_pWndSettings[1].m_Piston);
		Surface.ComputeTilt(m_pWndSettings[1].m_Tilt);

		//530
		CornealSurface &= m_pWndSettings[1].m_Mask;
		CornealSurface.ComputePiston(m_pWndSettings[1].m_Piston);
		CornealSurface.ComputeTilt(m_pWndSettings[1].m_Tilt);
		//530

		for (int d = 0; d < 2; d++)
		{
			int t = (e * 3 + 1) + d;

			m_Surfaces[t] = Surface;

			CWndSettings* pWndSettings = m_pWndSettings + (d + 1);

			switch (pWndSettings->m_Type)
			{
			case TYPE_TWFM:
			{
				//521
				//CEyeWnd* pEyeWnd = new CEyeWnd(m_Rect[t], this);
				CNewEyeWnd* pEyeWnd = new CNewEyeWnd(m_Rect[t], this, m_Scale);
				//521

				pEyeWnd->m_LTBigFont = TRUE;//530
				pEyeWnd->m_Type = TYPE_TWFM;
				pEyeWnd->m_SizeSmall = TRUE;
				pEyeWnd->m_Method3D = FALSE;
				pEyeWnd->m_LTLabel[0] = "Wavefront Map";
				pWndSettings->m_Mask.GetName(pEyeWnd->m_LTLabel[1]);

				//New
				if (m_Togther)
					pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), ar_max_um * 0.002);
				else
				{
					if (e == 0) pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), ar_max_um1 * 0.002);
					else     pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), ar_max_um2 * 0.002);
				}
				//Done
				//pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);

				pEyeWnd->m_MapShowMap = TRUE;
				pEyeWnd->m_MapShowNumbers = FALSE;
				pEyeWnd->m_MapShowEye = FALSE;
				pEyeWnd->m_MapShowPupil = FALSE;
				pEyeWnd->m_MapShowLimbus = FALSE;
				pEyeWnd->m_MapShowPoints = FALSE;
				pEyeWnd->m_MapTranslucent = FALSE;
				pEyeWnd->m_Unit = Unit::micron;
				pEyeWnd->m_Inc = pWndSettings->GetIncrement();
				CScale* pScale = pWndSettings->GetScale();
				pEyeWnd->m_NumColors = pScale->m_NumColors;
				for (int i = 0; i < pScale->m_NumColors; i++) {
					pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
				}
				pEyeWnd->m_pWFExam = pWFExam;
				CWFExam::CreateWfm2D(pEyeWnd->m_Map2D, Surface, 1.0 / pEyeWnd->m_y_px_um);
				pEyeWnd->CreateCentButtons();
				pEyeWnd->CreateZoomButtons();
				pEyeWnd->CreateZoneButtons();
				m_pDispWnd[t] = pEyeWnd;
			}
			break;

			case TYPE_TRFM:
			{
				//521
				//CEyeWnd* pEyeWnd = new CEyeWnd(m_Rect[t], this);
				CNewEyeWnd* pEyeWnd = new CNewEyeWnd(m_Rect[t], this, m_Scale);
				//521

				pEyeWnd->m_LTBigFont = TRUE;//530

				pEyeWnd->m_Type = TYPE_TRFM;
				pEyeWnd->m_SizeSmall = TRUE;
				pEyeWnd->m_Method3D = FALSE;
				pEyeWnd->m_LTLabel[0] = "Refraction Map";
				pWndSettings->m_Mask.GetName(pEyeWnd->m_LTLabel[1]);

				//New
				if (m_Togther)
					pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), ar_max_um * 0.002);
				else
				{
					if (e == 0) pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), ar_max_um1 * 0.002);
					else     pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), ar_max_um2 * 0.002);
				}
				//Done
				//pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);

				pEyeWnd->m_MapShowMap = TRUE;
				pEyeWnd->m_MapShowNumbers = FALSE;
				pEyeWnd->m_MapShowEye = FALSE;
				pEyeWnd->m_MapShowPupil = FALSE;
				pEyeWnd->m_MapShowLimbus = FALSE;
				pEyeWnd->m_MapShowPoints = FALSE;
				pEyeWnd->m_MapTranslucent = FALSE;
				pEyeWnd->m_Unit = Unit::D;
				pEyeWnd->m_Inc = pWndSettings->GetIncrement();
				CScale* pScale = pWndSettings->GetScale();
				pEyeWnd->m_NumColors = pScale->m_NumColors;
				for (int i = 0; i < pScale->m_NumColors; i++) {
					pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
				}
				pEyeWnd->m_pWFExam = pWFExam;
				CWFExam::CreateRfm2D(pEyeWnd->m_Map2D, Surface, 1.0 / pEyeWnd->m_y_px_um);
				pEyeWnd->CreateCentButtons();
				pEyeWnd->CreateZoomButtons();
				pEyeWnd->CreateZoneButtons();
				m_pDispWnd[t] = pEyeWnd;
			}
			break;

			case TYPE_TLTR:
			{
				CLtrWnd* pLtrWnd = new CLtrWnd(m_Rect[t], this);
				pLtrWnd->m_DysVal = -1;//531

				pLtrWnd->m_LTBigFont = TRUE;//530

				CWFExam::CreateLtr(Surface, pLtrWnd->m_Ltr, pWndSettings->m_Ltr, pWndSettings->m_LtrLine, pWndSettings->m_LtrOrientation);
				pLtrWnd->m_Type = TYPE_TLTR;
				pLtrWnd->m_SizeSmall = TRUE;
				pLtrWnd->m_Method3D = FALSE;
				//pLtrWnd->m_LTLabel[0].Format(_T("Letter 20 / %i", pWndSettings->m_LtrLine);
				pLtrWnd->m_LTLabel[0] = "Letter";//531
				pWndSettings->m_Mask.GetName(pLtrWnd->m_LTLabel[1]);

				//New
				if (m_Togther)
					pLtrWnd->m_LTLabel[2].Format(_T("%.2f mm"), ar_max_um * 0.002);
				else
				{
					if (e == 0) pLtrWnd->m_LTLabel[2].Format(_T("%.2f mm"), ar_max_um1 * 0.002);
					else     pLtrWnd->m_LTLabel[2].Format(_T("%.2f mm"), ar_max_um2 * 0.002);
				}
				//Done
				//pLtrWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);

				pLtrWnd->CreateZoomButtons();
				pLtrWnd->CreateZoneButtons();
				m_pDispWnd[t] = pLtrWnd;
			}
			break;

			//530
			case TYPE_CLTR:
			{
				CLtrWnd* pLtrWnd = new CLtrWnd(m_Rect[t], this);
				pLtrWnd->m_DysVal = -1;//531

				pLtrWnd->m_LTBigFont = TRUE;//530

				CWFExam::CreateLtr(CornealSurface, pLtrWnd->m_Ltr, pWndSettings->m_Ltr, pWndSettings->m_LtrLine, pWndSettings->m_LtrOrientation);
				pLtrWnd->m_Type = TYPE_CLTR;
				pLtrWnd->m_SizeSmall = TRUE;
				pLtrWnd->m_Method3D = FALSE;
				//pLtrWnd->m_LTLabel[0].Format(_T("Letter 20 / %i - Cornea", pWndSettings->m_LtrLine);
				pLtrWnd->m_LTLabel[0] = "Letter";//531
				pWndSettings->m_Mask.GetName(pLtrWnd->m_LTLabel[1]);

				//New
				if (m_Togther)
					pLtrWnd->m_LTLabel[2].Format(_T("%.2f mm"), ar_max_um * 0.002);
				else
				{
					if (e == 0) pLtrWnd->m_LTLabel[2].Format(_T("%.2f mm"), ar_max_um1 * 0.002);
					else     pLtrWnd->m_LTLabel[2].Format(_T("%.2f mm"), ar_max_um2 * 0.002);
				}
				//Done
				//pLtrWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);

				pLtrWnd->CreateZoomButtons();
				pLtrWnd->CreateZoneButtons();
				m_pDispWnd[t] = pLtrWnd;
			}
			break;
			//530

			//530 
			case TYPE_TPSF:
			{
				CPSFWnd* pPSFWnd = new CPSFWnd(m_Rect[t], this);

				pPSFWnd->m_LTBigFont = TRUE;//530

				CWFExam::CreatePsf(Surface, pPSFWnd->m_Psf, pWndSettings->m_EEFType);
				pPSFWnd->m_Type = pWndSettings->m_Type;
				pPSFWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
				pPSFWnd->m_Method3D = pWndSettings->m_Method3D;
				pPSFWnd->m_LTLabel[0] = "Point Spread Function";
				pWndSettings->m_Mask.GetName(pPSFWnd->m_LTLabel[1]);
				//New
				if (m_Togther)
					pPSFWnd->m_LTLabel[2].Format(_T("%.2f mm"), ar_max_um * 0.002);
				else
				{
					if (e == 0) pPSFWnd->m_LTLabel[2].Format(_T("%.2f mm"), ar_max_um1 * 0.002);
					else     pPSFWnd->m_LTLabel[2].Format(_T("%.2f mm"), ar_max_um2 * 0.002);
				}
				//Done
				pPSFWnd->m_ShowEEF = pWndSettings->m_ShowEEF;
				pPSFWnd->CreateZoomButtons();
				pPSFWnd->CreateZoneButtons();
				m_pDispWnd[t] = pPSFWnd;
			}
			break;

			//corneal PSF
			case TYPE_CPSF:
			{
				CPSFWnd* pPSFWnd = new CPSFWnd(m_Rect[t], this);

				pPSFWnd->m_LTBigFont = TRUE;//530

				CWFExam::CreatePsf(CornealSurface, pPSFWnd->m_Psf, pWndSettings->m_EEFType);
				pPSFWnd->m_Type = pWndSettings->m_Type;
				pPSFWnd->m_SizeSmall = pWndSettings->m_SizeSmall;
				pPSFWnd->m_Method3D = pWndSettings->m_Method3D;
				pPSFWnd->m_LTLabel[0] = "Point Spread Function - Cornea";
				pWndSettings->m_Mask.GetName(pPSFWnd->m_LTLabel[1]);
				//New
				if (m_Togther)
					pPSFWnd->m_LTLabel[2].Format(_T("%.2f mm"), ar_max_um * 0.002);
				else
				{
					if (e == 0) pPSFWnd->m_LTLabel[2].Format(_T("%.2f mm"), ar_max_um1 * 0.002);
					else     pPSFWnd->m_LTLabel[2].Format(_T("%.2f mm"), ar_max_um2 * 0.002);
				}
				//Done
				pPSFWnd->m_ShowEEF = pWndSettings->m_ShowEEF;
				pPSFWnd->CreateZoomButtons();
				pPSFWnd->CreateZoneButtons();
				m_pDispWnd[t] = pPSFWnd;
			}
			break;
			//530
			}
		}
	}


	//Show the m_pDispWnd[0] and m_pDispWnd[3]  
	real_t r_max_um1 = 5000;
	real_t r_max_um2 = 5000;

	CZernikeSurface WFSurface;
	if (m_pCTExam1)
	{
		m_Surfaces[0] = m_pCTExam1->m_WfSurface;
		WFSurface = m_pWFExam1->m_WfSurface;

		r_max_um1 = __min(r_max_um1, m_Surfaces[0].m_r_max_um);
		r_max_um1 = __min(r_max_um1, WFSurface.m_r_max_um);
	}

	if (m_pWndSettings[1].m_ZoneRadiusUm != 0)
		r_max_um1 = __min(r_max_um1, m_pWndSettings[1].m_ZoneRadiusUm);

	if (m_pCTExam2)
	{
		m_Surfaces[1] = m_pCTExam2->m_WfSurface;
		WFSurface = m_pWFExam2->m_WfSurface;

		r_max_um2 = __min(r_max_um2, m_Surfaces[1].m_r_max_um);
		r_max_um2 = __min(r_max_um2, WFSurface.m_r_max_um);
	}

	if (m_pWndSettings[4].m_ZoneRadiusUm != 0)
		r_max_um2 = __min(r_max_um2, m_pWndSettings[4].m_ZoneRadiusUm);


	if (m_d == 1 || m_d == 2)
	{
		r_max_um1 = __min(r_max_um1, m_pWndSettings[m_d].m_ZoneRadiusUm);
	}
	else if (m_d == 4 || m_d == 5)
	{
		r_max_um2 = __min(r_max_um2, m_pWndSettings[m_d].m_ZoneRadiusUm);
	}

	if (m_pCTExam1)
	{
		m_Surfaces[0].ChangeRMaxUm(r_max_um1);
		m_Surfaces[0] &= m_pWndSettings[0].m_Mask;
		m_Surfaces[0].ComputePiston(m_pWndSettings[0].m_Piston);
		m_Surfaces[0].ComputeTilt(m_pWndSettings[0].m_Tilt);
	}

	if (m_pCTExam2)
	{
		m_Surfaces[1].ChangeRMaxUm(r_max_um2);
		m_Surfaces[1] &= m_pWndSettings[0].m_Mask;
		m_Surfaces[1].ComputePiston(m_pWndSettings[0].m_Piston);
		m_Surfaces[1].ComputeTilt(m_pWndSettings[0].m_Tilt);
	}

	int down = 1;
	int up = 1;
	if (m_pCTExam1) down = 0;
	if (m_pCTExam2) up = 2;
	for (int i = down; i < up; i++)
	{
		CCTExam* pCTExam = i == 0 ? m_pCTExam1 : m_pCTExam2;
		int t = i * 3;

		real_t r_max_um = i == 0 ? r_max_um1 : r_max_um2;

		switch (m_pWndSettings[0].m_Type)
		{
		case TYPE_CEYE:
		{
			CNewEyeWnd* pEyeWnd = new CNewEyeWnd(m_Rect[t], this, 5000);

			pEyeWnd->m_LTBigFont = TRUE;//530

			pEyeWnd->m_Type = m_pWndSettings[0].m_Type;
			pEyeWnd->m_SizeSmall = m_pWndSettings[0].m_SizeSmall;
			pEyeWnd->m_Method3D = FALSE;
			pEyeWnd->m_MapShowEye = TRUE;
			pEyeWnd->m_MapShowRings = m_pWndSettings[0].m_MapShowRings;
			pEyeWnd->m_MapShowLimbus = m_pWndSettings[0].m_MapShowLimbus;
			pEyeWnd->m_MapShowPupil = m_pWndSettings[0].m_MapShowPupil;
			pEyeWnd->m_pCTExam = pCTExam;
			m_pDispWnd[t] = pEyeWnd;
		}
		break;

		case TYPE_CAXM:
		case TYPE_CTNM:
		case TYPE_CRFM:
		{
			int MapUnit = (m_pWndSettings[0].m_Type == TYPE_CRFM) || (m_pWndSettings[0].m_MapUnit == DIOPTERS) ? DIOPTERS : MILLIMETERS;
			CNewEyeWnd* pEyeWnd = new CNewEyeWnd(m_Rect[t], this, 5000);

			pEyeWnd->m_LTBigFont = TRUE;//530

			pEyeWnd->m_Type = m_pWndSettings[0].m_Type;
			pEyeWnd->m_SizeSmall = m_pWndSettings[0].m_SizeSmall;
			pEyeWnd->m_Method3D = m_pWndSettings[0].m_Method3D;

			if (m_pWndSettings[0].m_Type == TYPE_CAXM) pEyeWnd->m_LTLabel[0] = "Axial Map";
			else if (m_pWndSettings[0].m_Type == TYPE_CTNM) pEyeWnd->m_LTLabel[0] = "Local ROC Map";
			else pEyeWnd->m_LTLabel[0] = "Refractive Map";

			pEyeWnd->m_MapShowEye = m_pWndSettings[0].m_MapShowEye;
			pEyeWnd->m_MapShowMap = TRUE;//m_pWndSettings[0].m_MapShowMap;
			pEyeWnd->m_MapShowNumbers = m_pWndSettings[0].m_MapShowNumbers;
			pEyeWnd->m_MapShowPupil = m_pWndSettings[0].m_MapShowPupil;
			pEyeWnd->m_MapShowLimbus = m_pWndSettings[0].m_MapShowLimbus;
			pEyeWnd->m_MapShowSolidSurface = m_pWndSettings[0].m_MapShowSolidSurface;
			pEyeWnd->m_MapShowWireMesh = m_pWndSettings[0].m_MapShowWireMesh;
			pEyeWnd->m_MapTranslucent = m_pWndSettings[0].m_MapTranslucent;
			pEyeWnd->m_MapShowKeratometry = m_pWndSettings[0].m_MapShowKeratometry;
			pEyeWnd->m_MapShowSimKAxes = m_pWndSettings[0].m_MapShowSimKAxes;
			pEyeWnd->m_Unit = MapUnit == DIOPTERS ? Unit::D : Unit::mm;
			pEyeWnd->m_Inc = m_pWndSettings[0].GetIncrement();
			CScale* pScale = m_pWndSettings[0].GetScale();
			pEyeWnd->m_NumColors = pScale->m_NumColors;
			for (int i = 0; i < pScale->m_NumColors; i++) {
				pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
			}
			pEyeWnd->m_pCTExam = pCTExam;
			if (m_pWndSettings[0].m_Method3D)
			{
				pCTExam->CreateDpm3D(pEyeWnd->m_Map3D, m_pWndSettings[0].m_Type, MapUnit);
			}
			else
			{
				pCTExam->CreateDpm2D(pEyeWnd->m_Map2D, m_pWndSettings[0].m_Type, MapUnit, 1.0 / pEyeWnd->m_y_px_um);
				pEyeWnd->m_Circles.Create(1);
				pEyeWnd->m_Circles[0].m_x_um = 0.0;
				pEyeWnd->m_Circles[0].m_y_um = 0.0;
				pEyeWnd->m_Circles[0].m_r_um = r_max_um;
				pEyeWnd->m_Circles[0].m_color = BLACK;
			}
			pEyeWnd->CreateCentButtons();
			pEyeWnd->CreateZoomButtons();
			m_pDispWnd[t] = pEyeWnd;
		}
		break;

		case TYPE_CELM:
		{
			real_t ax0_um, q;
			if (m_pWndSettings[0].m_RefSurfaceType == REF_SURFACE_BEST_SPHERE) {
				ax0_um = pCTExam->m_HtSpSurface.m_ax0_um;
				q = 0.0;
			}
			else if (m_pWndSettings[0].m_RefSurfaceType == REF_SURFACE_BEST_CONIC) {
				ax0_um = pCTExam->m_HtCnSurface.m_ax0_um;
				q = pCTExam->m_HtCnSurface.m_q;
			}
			else {
				ax0_um = m_pWndSettings[0].m_RefSurfaceAx0Um;
				q = m_pWndSettings[0].m_RefSurfaceQ;
			}
			CConicSurface RefSurface;
			RefSurface.Create(CT_R_MAX_UM, ax0_um, q);

			CNewEyeWnd* pEyeWnd = new CNewEyeWnd(m_Rect[t], this, 5000);

			pEyeWnd->m_LTBigFont = TRUE;//530

			pEyeWnd->m_Type = m_pWndSettings[0].m_Type;
			pEyeWnd->m_SizeSmall = m_pWndSettings[0].m_SizeSmall;
			pEyeWnd->m_Method3D = m_pWndSettings[0].m_Method3D;
			pEyeWnd->m_LTLabel[0] = "Z Elevation Map";
			pEyeWnd->m_RBLabel[4] = "Ref Surface";
			pEyeWnd->m_RBLabel[5].Format(_T("R0 = %.2f mm, Q = %.2f"), 0.001 * RefSurface.m_ax0_um, RefSurface.m_q);
			pEyeWnd->m_RBLabelColor[0] = m_Printing ? BLACK : WHITE;
			pEyeWnd->m_RBLabelColor[1] = pEyeWnd->m_RBLabelColor[0];
			pEyeWnd->m_RBLabelColor[2] = pEyeWnd->m_RBLabelColor[0];
			pEyeWnd->m_RBLabelColor[3] = pEyeWnd->m_RBLabelColor[0];
			pEyeWnd->m_RBLabelColor[4] = pEyeWnd->m_RBLabelColor[0];
			pEyeWnd->m_RBLabelColor[5] = pEyeWnd->m_RBLabelColor[0];
			pEyeWnd->m_RBLabelBkColor = NOCOLOR;
			pEyeWnd->m_MapShowEye = m_pWndSettings[0].m_MapShowEye;
			pEyeWnd->m_MapShowMap = TRUE;//m_pWndSettings[0].m_MapShowMap;
			pEyeWnd->m_MapShowNumbers = m_pWndSettings[0].m_MapShowNumbers;
			pEyeWnd->m_MapShowPupil = m_pWndSettings[0].m_MapShowPupil;
			pEyeWnd->m_MapShowLimbus = m_pWndSettings[0].m_MapShowLimbus;
			pEyeWnd->m_MapShowSolidSurface = m_pWndSettings[0].m_MapShowSolidSurface;
			pEyeWnd->m_MapShowWireMesh = m_pWndSettings[0].m_MapShowWireMesh;
			pEyeWnd->m_MapTranslucent = m_pWndSettings[0].m_MapTranslucent;
			pEyeWnd->m_Unit = Unit::micron;
			pEyeWnd->m_Inc = m_pWndSettings[0].GetIncrement();
			CScale* pScale = m_pWndSettings[0].GetScale();
			pEyeWnd->m_NumColors = pScale->m_NumColors;
			for (int i = 0; i < pScale->m_NumColors; i++) {
				pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
			}
			pEyeWnd->m_pCTExam = pCTExam;
			if (m_pWndSettings[0].m_Method3D) {
				pCTExam->CreateElm3D(pEyeWnd->m_Map3D, RefSurface);
			}
			else {
				pCTExam->CreateElm2D(pEyeWnd->m_Map2D, RefSurface, 1.0 / pEyeWnd->m_y_px_um);
			}
			pEyeWnd->CreateCentButtons();
			pEyeWnd->CreateZoomButtons();
			m_pDispWnd[t] = pEyeWnd;
		}
		break;

		case TYPE_CWFM://35
		{
			CNewEyeWnd* pEyeWnd = new CNewEyeWnd(m_Rect[t], this, 5000);

			pEyeWnd->m_LTBigFont = TRUE;//530

			pEyeWnd->m_Type = m_pWndSettings[0].m_Type;
			pEyeWnd->m_SizeSmall = m_pWndSettings[0].m_SizeSmall;
			pEyeWnd->m_Method3D = m_pWndSettings[0].m_Method3D;
			pEyeWnd->m_LTLabel[0] = "Wavefront Map";
			m_pWndSettings[0].m_Mask.GetName(pEyeWnd->m_LTLabel[1]);

			pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);

			pEyeWnd->m_MapShowEye = m_pWndSettings[0].m_MapShowEye;
			pEyeWnd->m_MapShowMap = TRUE;//m_pWndSettings[0].m_MapShowMap;
			pEyeWnd->m_MapShowNumbers = m_pWndSettings[0].m_MapShowNumbers;
			pEyeWnd->m_MapShowLimbus = m_pWndSettings[0].m_MapShowLimbus;
			pEyeWnd->m_MapShowPupil = m_pWndSettings[0].m_MapShowPupil;
			pEyeWnd->m_MapShowSolidSurface = m_pWndSettings[0].m_MapShowSolidSurface;
			pEyeWnd->m_MapShowWireMesh = m_pWndSettings[0].m_MapShowWireMesh;
			pEyeWnd->m_MapTranslucent = m_pWndSettings[0].m_MapTranslucent;
			pEyeWnd->m_Unit = Unit::micron;
			pEyeWnd->m_Inc = m_pWndSettings[0].GetIncrement();
			CScale* pScale = m_pWndSettings[0].GetScale();
			pEyeWnd->m_NumColors = pScale->m_NumColors;
			for (int is = 0; is < pScale->m_NumColors; is++) {
				pEyeWnd->m_Colors[is] = pScale->m_Colors[is];
			}
			pEyeWnd->m_pCTExam = pCTExam;
			if (m_pWndSettings[0].m_Method3D) {
				pCTExam->CreateWfm3D(pEyeWnd->m_Map3D, m_Surfaces[i]);
			}
			else {
				pCTExam->CreateWfm2D(pEyeWnd->m_Map2D, m_Surfaces[i], 1.0 / pEyeWnd->m_y_px_um);
			}
			pEyeWnd->CreateCentButtons();
			pEyeWnd->CreateZoomButtons();
			pEyeWnd->CreateZoneButtons();
			m_pDispWnd[t] = pEyeWnd;
		}
		break;

		case TYPE_CRMS:
		{
			CRMSWnd* pRMSWnd = new CRMSWnd(m_Rect[t], this);

			pRMSWnd->m_LTBigFont = TRUE;//530

			pRMSWnd->m_SimpliedRMS = m_pWndSettings[0].m_SimplifiedRMS;//530
			pRMSWnd->m_Type = m_pWndSettings[0].m_Type;
			pRMSWnd->m_SizeSmall = m_pWndSettings[0].m_SizeSmall;
			pRMSWnd->m_Method3D = m_pWndSettings[0].m_Method3D;
			pRMSWnd->m_LTLabel[0] = "Root Mean Square";
			m_pWndSettings[0].m_Mask.GetName(pRMSWnd->m_LTLabel[1]);
			pRMSWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
			pRMSWnd->m_Surface = m_Surfaces[i];
			pRMSWnd->m_Individual = m_pWndSettings[0].m_RMSIndividual;
			if (!pRMSWnd->m_Method3D && pRMSWnd->m_Individual) pRMSWnd->CreateSimpButtons(m_pWndSettings[0].m_SimplifiedRMS);//530
			pRMSWnd->CreateZoomButtons();
			pRMSWnd->CreateZoneButtons();
			m_pDispWnd[t] = pRMSWnd;
		}
		break;

		case TYPE_CSKM:
		{
			CNewEyeWnd* pEyeWnd = new CNewEyeWnd(m_Rect[t], this, 5000);

			pEyeWnd->m_LTBigFont = TRUE;//530

			pEyeWnd->m_Type = m_pWndSettings[0].m_Type;
			pEyeWnd->m_SizeSmall = m_pWndSettings[0].m_SizeSmall;
			pEyeWnd->m_Method3D = FALSE;//m_pWndSettings[0].m_Method3D;
			pEyeWnd->m_LTLabel[0] = "Keratometry Map";
			pEyeWnd->m_MapShowEye = m_pWndSettings[0].m_MapShowEye;
			pEyeWnd->m_MapShowRings = FALSE;
			pEyeWnd->m_MapShowLimbus = m_pWndSettings[0].m_MapShowLimbus;
			pEyeWnd->m_MapShowPupil = m_pWndSettings[0].m_MapShowPupil;
			pEyeWnd->m_pCTExam = pCTExam;
			m_pDispWnd[t] = pEyeWnd;
		}
		break;
		}
	}
	//Done


	//Scale range of windows 0 and 3
	{
		CWndSettings* pWndSettings = m_pWndSettings;
		CScale* pScale = pWndSettings->GetScale();
		real_t Inc = pWndSettings->GetIncrement();
		real_t Cent = 0.0;
		real_t Step;

		// middle value
		if (m_pWndSettings[0].m_Type == TYPE_CAXM || m_pWndSettings[0].m_Type == TYPE_CTNM || m_pWndSettings[0].m_Type == TYPE_CRFM)
		{
			int MapUnit = (m_pWndSettings[0].m_Type == TYPE_CRFM) || (m_pWndSettings[0].m_MapUnit == DIOPTERS) ? DIOPTERS : MILLIMETERS;
			if (pScale->m_CentAuto)
			{
				if (MapUnit == DIOPTERS)
				{
					if (m_pCTExam1) Cent = m_pCTExam1->m_ax0_dp;
					else Cent = m_pCTExam2->m_ax0_dp;
				}
				else
				{
					if (m_pCTExam1)  Cent = 0.001 * m_pCTExam1->m_ax0_um;
					else Cent = 0.001 * m_pCTExam2->m_ax0_um;
				}
			}
			else
			{
				Cent = pScale->m_Cent;
			}
		}
		else
		{
			Cent = 0.0;
		}
		Cent = RealRound(Cent, Inc);

		// step value
		if (pScale->m_StepAuto)
		{
			real_t Span = 0.0;
			for (int e = 0; e < 2; e++)
			{
				CNewEyeWnd* pEyeWnd = (CNewEyeWnd*)m_pDispWnd[e * 3];

				if (pEyeWnd)
				{
					real_t Min = m_pWndSettings[0].m_Method3D ? pEyeWnd->m_Map3D.m_min : pEyeWnd->m_Map2D.m_min;
					real_t Max = m_pWndSettings[0].m_Method3D ? pEyeWnd->m_Map3D.m_max : pEyeWnd->m_Map2D.m_max;
					real_t Span = 2.0 * __max(fabs(Min - Cent), fabs(Max - Cent));
				}
			}
			real_t t = Span / pScale->m_NumColors;
			Step = (int)(t / Inc) * Inc;
			if (t > Step + 0.001) Step += Inc;
		}
		else
		{
			Step = RealRound(pScale->m_Step, Inc);
		}
		if (Step < Inc) Step = Inc;
		real_t StepMax = m_pWndSettings[0].GetMaxStep();
		if (Step > StepMax) Step = StepMax;

		for (int e = 0; e < 2; e++)
		{
			CNewEyeWnd* pEyeWnd = (CNewEyeWnd*)m_pDispWnd[e * 3];
			if (pEyeWnd)
			{
				pEyeWnd->m_Cent = Cent;
				pEyeWnd->m_Step = Step;
			}
		}
	}
	//Done



	//Scale range of windows 1 and 4
	{
		CWndSettings* pWndSettings = m_pWndSettings + 1;
		CScale* pScale = pWndSettings->GetScale();
		real_t Inc = pWndSettings->GetIncrement();
		real_t Cent = 0.0;
		real_t Step;
		if (pScale->m_StepAuto)
		{
			real_t Span = 0.0;
			for (int e = down; e < up; e++)
			{
				CNewEyeWnd* pEyeWnd = (CNewEyeWnd*)m_pDispWnd[e * 3 + 1];
				if (pEyeWnd) {
					real_t Min = pWndSettings->m_Method3D ? pEyeWnd->m_Map3D.m_min : pEyeWnd->m_Map2D.m_min;
					real_t Max = pWndSettings->m_Method3D ? pEyeWnd->m_Map3D.m_max : pEyeWnd->m_Map2D.m_max;
					Span = __max(Span, 2.0 * __max(fabs(Min - Cent), fabs(Max - Cent)));
				}
			}
			real_t t = Span / pScale->m_NumColors;
			Step = (int)(t / Inc) * Inc;
			if (t > Step + 0.001) Step += Inc;
		}
		else {
			Step = RealRound(pScale->m_Step, Inc);
		}
		if (Step < Inc) Step = Inc;
		real_t StepMax = pWndSettings->GetMaxStep();
		if (Step > StepMax) Step = StepMax;
		//
		for (int e = 0; e < up; e++) {
			CNewEyeWnd* pEyeWnd = (CNewEyeWnd*)m_pDispWnd[e * 3 + 1];
			if (pEyeWnd) {
				pEyeWnd->m_Cent = Cent;
				pEyeWnd->m_Step = Step;
			}
		}
	}
	//Done

	//521 Let the zoom in/out button to be gray if minimum or maximum scan size

	for (int i = down * 3; i < up * 2; i++)
	{
		real_t maxR = (i < 3 ? ::TempSettings.Com_r_max_um1 : ::TempSettings.Com_r_max_um2);
		if (i != 0 && i != 3)
		{
			m_pDispWnd[i]->m_IncZoneButton.SetEnabled(TRUE);
			m_pDispWnd[i]->m_IncZoneButton.Repaint();

			m_pDispWnd[i]->m_DecZoneButton.SetEnabled(TRUE);
			m_pDispWnd[i]->m_DecZoneButton.Repaint();

			if (m_pWndSettings[i].m_ZoneRadiusUm >= (int)maxR)
			{
				m_pDispWnd[i]->m_IncZoneButton.SetEnabled(FALSE);
				m_pDispWnd[i]->m_IncZoneButton.Repaint();
			}

			if (m_pWndSettings[i].m_ZoneRadiusUm == 500)
			{
				m_pDispWnd[i]->m_DecZoneButton.SetEnabled(FALSE);
				m_pDispWnd[i]->m_DecZoneButton.Repaint();
			}
		}
	}
	//521  
}

//***************************************************************************************

void CWFCTOUSumWnd::CreateMenu()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_d == 0 || m_d == 3)
	{
		CScale* pScale = pWndSettings->GetScale();
		CXPMenu* pSubMenu;

		//m_pMenu->AppendItem(IDC_TYPE_CEYE_ITEM, pWndSettings->m_Type == TYPE_CEYE, FALSE, "Eye Image", "");
		m_pMenu->AppendItem(IDC_TYPE_CAXM_ITEM, pWndSettings->m_Type == TYPE_CAXM, FALSE, "Axial Map", "");
		m_pMenu->AppendItem(IDC_TYPE_CTNM_ITEM, pWndSettings->m_Type == TYPE_CTNM, FALSE, "Local ROC Map", "");

		m_pMenu->AppendSeparator();

		m_pMenu->AppendItem(IDC_SHOW_MAP_EYE_ITEM, FALSE, pWndSettings->m_MapShowEye, "Show eye image", "");
		m_pMenu->AppendItem(IDC_SHOW_MAP_PUPIL_ITEM, FALSE, pWndSettings->m_MapShowPupil, "Show pupil", "");
		m_pMenu->AppendItem(IDC_MAP_TRANSLUCENT_ITEM, FALSE, pWndSettings->m_MapTranslucent, "Translucent map", "");

		m_pMenu->AppendSeparator();

		pSubMenu = m_pMenu->AppendSubmenu("Map middle value", "");
		pSubMenu->AppendItem(IDC_MAP_CENT_USER_ITEM, !pScale->m_CentAuto, FALSE, "Fixed...", "");
		pSubMenu->AppendItem(IDC_MAP_CENT_AUTO_ITEM, pScale->m_CentAuto, FALSE, "Auto", "");

		CreateSubmenuMapSteps();
		CreateSubmenuMapColors();
	}
	else
	{
		if (m_d == 1 || m_d == 4) {
			m_pMenu->AppendItem(IDC_TYPE_TWFM_ITEM, pWndSettings->m_Type == TYPE_TWFM, FALSE, "Wavefront Map", "");
			m_pMenu->AppendItem(IDC_TYPE_TRFM_ITEM, pWndSettings->m_Type == TYPE_TRFM, FALSE, "Refraction Map", "");
			m_pMenu->AppendSeparator();
		}

		//530
		if (m_d == 2 || m_d == 5) {
			m_pMenu->AppendItem(IDC_TYPE_TLTR_ITEM, pWndSettings->m_Type == TYPE_TLTR || pWndSettings->m_Type == TYPE_CLTR, FALSE, "Visual Acuity Chart", "");
			m_pMenu->AppendItem(IDC_TYPE_TPSF_ITEM, pWndSettings->m_Type == TYPE_TPSF || pWndSettings->m_Type == TYPE_CPSF, FALSE, "Point Spread Function", "");

			m_pMenu->AppendSeparator();
			m_pMenu->AppendItem(IDC_TYPE_CORNEAL_ITEM, FALSE, pWndSettings->m_Type == TYPE_CLTR || pWndSettings->m_Type == TYPE_CPSF, "Corneal Only", "");
			m_pMenu->AppendSeparator();
		}
		//530

		CreateSubmenuZone(2);
		CreateSubmenuMask();

		if (m_d == 1 || m_d == 4) {

			m_pMenu->AppendSeparator();

			CreateSubmenuMapSteps();
			CreateSubmenuMapColors();
		}

		else if (m_d == 2 || m_d == 5) {

			m_pMenu->AppendSeparator();

			CreateSubmenuLtr();
		}
	}

	//521
	if (!SameOU())
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
	}
	//521
}

//***************************************************************************************

LRESULT CWFCTOUSumWnd::OnChangeCentMsg(WPARAM wParam, LPARAM lParam)
{
	//if (!m_pDispWnd[1] || !m_pDispWnd[4]) return 0;

	if (m_pCTExam1 && m_pCTExam2)
	{
		if (wParam == (WPARAM)m_pDispWnd[0]) {
			((CNewEyeWnd*)m_pDispWnd[3])->m_Cent = ((CNewEyeWnd*)m_pDispWnd[0])->m_Cent;
			m_pDispWnd[3]->Repaint();
		}
		else if (wParam == (WPARAM)m_pDispWnd[3]) {
			((CNewEyeWnd*)m_pDispWnd[0])->m_Cent = ((CNewEyeWnd*)m_pDispWnd[3])->m_Cent;
			m_pDispWnd[0]->Repaint();
		}

		if (wParam == (WPARAM)m_pDispWnd[1]) {
			((CNewEyeWnd*)m_pDispWnd[4])->m_Cent = ((CNewEyeWnd*)m_pDispWnd[1])->m_Cent;
			m_pDispWnd[4]->Repaint();
		}
		else if (wParam == (WPARAM)m_pDispWnd[4]) {
			((CNewEyeWnd*)m_pDispWnd[1])->m_Cent = ((CNewEyeWnd*)m_pDispWnd[4])->m_Cent;
			m_pDispWnd[1]->Repaint();
		}
	}

	return 0;
}

//***************************************************************************************

LRESULT CWFCTOUSumWnd::OnChangeStepMsg(WPARAM wParam, LPARAM lParam)
{
	if (!m_pDispWnd[0] || !m_pDispWnd[3]) return 0;

	if (wParam == (WPARAM)m_pDispWnd[0]) {
		((CNewEyeWnd*)m_pDispWnd[3])->m_Step = ((CNewEyeWnd*)m_pDispWnd[0])->m_Step;
		m_pDispWnd[3]->Repaint();
	}
	else if (wParam == (WPARAM)m_pDispWnd[3]) {
		((CNewEyeWnd*)m_pDispWnd[0])->m_Step = ((CNewEyeWnd*)m_pDispWnd[3])->m_Step;
		m_pDispWnd[0]->Repaint();
	}

	return 0;
}

//***************************************************************************************

//521
int CWFCTOUSumWnd::GetGridScale(real_t r_um1, real_t r_um2)
{
	real_t r_um = (r_um1 > r_um2 ? r_um1 : r_um2) + 1000;

	return int(r_um / 1000) * 1000;
}
//521

//***************************************************************************************
//521
void CWFCTOUSumWnd::OnShowDefaultItemClicked()
{
	CBusyCursor Cursor;

	m_pWndSettings[m_d].m_LtrDistanceType = 0;//530

	if (m_d == 0 || m_d == 3)
	{
		m_pWndSettings[0].m_Type = 31;//Axial map       TYPE_CAXM
		m_pWndSettings[0].m_Method3D = 0;
		m_pWndSettings[0].m_MapUnit = DIOPTERS;
		m_pWndSettings[0].m_MapShowMap = 1;
		m_pWndSettings[0].m_MapShowEye = 1;
		m_pWndSettings[0].m_MapShowNumbers = 0;
		m_pWndSettings[0].m_MapShowPupil = 0;
		m_pWndSettings[0].m_MapShowLimbus = 0;
		m_pWndSettings[0].m_MapShowCornealVertex = 0;
		m_pWndSettings[0].m_MapTranslucent = 0;
		m_pWndSettings[0].m_MapShowKeratometry = 0;

		CScale* pScale6 = m_pWndSettings[0].GetScale();
		pScale6->m_Step = 0.5;
		m_pWndSettings[0].SetDefaultColors(1);

		m_d = 0;
	}

	else if (m_d == 1 || m_d == 4)
	{
		m_pWndSettings[1].m_Type = 11;//Refraction Map  TYPE_TRFM
		m_pWndSettings[1].m_ZoneRadiusUm = 5000;
		m_pWndSettings[1].m_Mask.SetType(MASK_HO_TOTAL);
		m_pWndSettings[1].m_Piston = FALSE;
		m_pWndSettings[1].m_Tilt = FALSE;
		m_pWndSettings[1].SetDefaultColors(1);

		m_d = 1;
	}

	else if (m_d == 2 || m_d == 5)
	{
		m_pWndSettings[2].m_Mask.SetType(MASK_HO_TOTAL);
		m_pWndSettings[2].m_Ltr = 0;
		m_pWndSettings[2].m_LtrLine = 70;
		m_pWndSettings[2].m_LtrOrientation = 0;

		m_d = 2;
	}


	CreateChildWnd();

	Repaint();
}
//521
//***************************************************************************************
//521
BOOL CWFCTOUSumWnd::SameOU()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_d == 0 || m_d == 3)
	{
		if (pWndSettings->m_Type != 31)    return FALSE;
		if (pWndSettings->m_Method3D != 0)     return FALSE;
		if (pWndSettings->m_MapUnit != DIOPTERS) return FALSE;

		if (pWndSettings->m_MapShowMap != 1)     return FALSE;
		if (pWndSettings->m_MapShowEye != 1)     return FALSE;
		if (pWndSettings->m_MapShowPupil != 0)     return FALSE;
		if (pWndSettings->m_MapTranslucent != 0)     return FALSE;

		CScale* pScale = pWndSettings->GetScale();
		if (pScale->m_Step != 0.5)                         return FALSE;
		if (!pWndSettings->AreDefaultColors(1))            return FALSE;
	}

	else if (m_d == 1 || m_d == 4)
	{
		if (pWndSettings->m_Type != 11)        return FALSE;
		if (pWndSettings->m_Mask.GetType() != MASK_HO_TOTAL)   return FALSE;
		if (pWndSettings->m_Piston != FALSE)     return FALSE;
		if (pWndSettings->m_Tilt != FALSE)     return FALSE;

		if (!pWndSettings->AreDefaultColors(1))            return FALSE;
	}

	else if (m_d == 2 || m_d == 5)
	{
		if (pWndSettings->m_Mask.GetType() != MASK_HO_TOTAL)   return FALSE;
		if (pWndSettings->m_Ltr != 0)         return FALSE;
		if (pWndSettings->m_LtrLine != 70)        return FALSE;
		if (pWndSettings->m_LtrOrientation != 0)         return FALSE;
	}

	return TRUE;
}
//***************************************************************************************
//530
void CWFCTOUSumWnd::OnCornealItemClicked()
{
	CBusyCursor Cursor;

	CWndSettings* pWndSettings = GetWndSettings();

	if (pWndSettings->m_Type == TYPE_TLTR)  pWndSettings->m_Type = TYPE_CLTR;
	else if (pWndSettings->m_Type == TYPE_TPSF)  pWndSettings->m_Type = TYPE_CPSF;
	else if (pWndSettings->m_Type == TYPE_CLTR)  pWndSettings->m_Type = TYPE_TLTR;
	else if (pWndSettings->m_Type == TYPE_CPSF)  pWndSettings->m_Type = TYPE_TPSF;

	CreateChildWnd();

	Repaint();
}
//530
//***************************************************************************************
