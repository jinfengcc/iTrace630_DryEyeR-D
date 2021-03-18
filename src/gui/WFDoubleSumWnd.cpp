//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "WFDoubleSumWnd.h"
#include "EyeWnd.h"
#include "PSFWnd.h"
#include "MTFWnd.h"
#include "EEFWnd.h"
#include "LtrWnd.h"
#include "RMSWnd.h"
#include "ChartWnd.h"
#include "BusyCursor.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CWFDoubleSumWnd, CSumWnd)

	ON_MESSAGE(WM_CHANGE_CENT, OnChangeCentMsg)
	ON_MESSAGE(WM_CHANGE_STEP, OnChangeStepMsg)

	ON_COMMAND(IDC_DEFAULT_GUI_ITEM, OnShowDefaultItemClicked)

	ON_BN_CLICKED(IDC_SHOW_FULL_INFO, OnShowWFTextInfoBtnClicked)
	ON_BN_CLICKED(IDC_SHOW_FULL_INFO2, OnShowWFTextInfoBtn2Clicked)

END_MESSAGE_MAP()

//***************************************************************************************

CWFDoubleSumWnd::CWFDoubleSumWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CWFExam* pWFExam1, CWFExam* pWFExam2, CWndSettings* pWndSettings) :
	CSumWnd(pWnd, WndRect, pPatient, pWndSettings)
{
	CBusyCursor Cursor;

	//----------------------------------------------------
	RECT Rect;
	GetWindowRect(&Rect);
	real_t t = 3.0 * (0.03 * m_h) + m_g;
	real_t w2 = ((Rect.right - Rect.left) - 5.0 * m_g) / 6.0;
	real_t h2 = ((Rect.bottom - Rect.top - t) - 4.0 * m_g) / 4.0;
	real_t w = w2 + w2;
	real_t h = h2 + m_g + h2;
	::SetRect(&m_Rect[0], intRound(m_g), intRound(t), intRound(m_g + w), intRound(t + h)); // exam 1 map
	::SetRect(&m_Rect[1], intRound(m_g + w + m_g), intRound(t), intRound(m_g + w + m_g + w2), intRound(t + h2)); // exam 1 psf
	::SetRect(&m_Rect[2], intRound(m_g + w + m_g), intRound(t + h2 + m_g), intRound(m_g + w + m_g + w2), intRound(t + h)); // exam 1 letter
	::SetRect(&m_Rect[3], intRound(m_g + w + m_g + w2 + m_g), intRound(t), intRound(m_g + w + m_g + w2 + m_g + w), intRound(t + h)); // exam 2 map
	::SetRect(&m_Rect[4], intRound(m_g + w + m_g + w2 + m_g + w + m_g), intRound(t), intRound(m_g + w + m_g + w2 + m_g + w + m_g + w2), intRound(t + h2)); // exam 2 psf
	::SetRect(&m_Rect[5], intRound(m_g + w + m_g + w2 + m_g + w + m_g), intRound(t + h2 + m_g), intRound(m_g + w + m_g + w2 + m_g + w + m_g + w2), intRound(t + h)); // exam 2 letter
	::SetRect(&m_Rect[6], intRound(m_g), intRound(t + h + m_g), intRound(m_g + w), intRound(t + h + m_g + h)); // exam 1 info
	::SetRect(&m_Rect[7], intRound(m_g + w + m_g + w2 + m_g), intRound(t + h + m_g), intRound(m_g + w + m_g + w2 + m_g + w), intRound(t + h + m_g + h)); // exam 2 info
	::SetRect(&m_Rect[8], intRound(m_g + w + m_g), intRound(t + h + m_g), intRound(m_g + w + m_g + w2), intRound(t + h + m_g + h)); // exam 1 complaints
	::SetRect(&m_Rect[9], intRound(m_g + w + m_g + w2 + m_g + w + m_g), intRound(t + h + m_g), intRound(m_g + w + m_g + w2 + m_g + w + m_g + w2), intRound(t + h + m_g + h)); // exam 2 complaints
																																											  //----------------------------------------------------

	for (int i = 0; i < 6; i++) m_InsideZone[i] = ::NewSettings.m_ShowPreZone_WF_VFAWnd;

	m_pWFExam1 = NULL;
	m_pWFExam2 = NULL;
	if (pWFExam1) {
		if (pWFExam1->m_Header.IsOD()) m_pWFExam1 = pWFExam1;
		else m_pWFExam2 = pWFExam1;
	}
	if (pWFExam2) {
		if (pWFExam2->m_Header.IsOD()) m_pWFExam1 = pWFExam2;
		else m_pWFExam2 = pWFExam2;
	}

	//record the increase and decrease range
	m_VFASumWnd = TRUE;

	int invalidValue = 0;
	real_t m_r1, m_r2;
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
	//record the increase and decrease range

	if (m_pWFExam1 != NULL) ::TempSettings.Com_r_max_um1 = m_r1;
	if (m_pWFExam2 != NULL) ::TempSettings.Com_r_max_um2 = m_r2;

	CreateChildWnd();

	Repaint();

	// Draw the WF Text Info button
	if (m_pWFExam1 != NULL)
	{
		CString s = (m_pWndSettings[0].m_SimplifiedWFPanel ? "Show Details" : "Hide Details");
		::SetRect(&Rect, m_Rect[6].right - 25, m_Rect[6].top + 5, m_Rect[6].right - 5, m_Rect[6].top + 20);
		m_WFShowFullInfoButton.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_SHOW_FULL_INFO);
		m_WFShowFullInfoButton.SetImage(m_pWndSettings[0].m_SimplifiedWFPanel ?
			(m_SW >= 1920 ? IDR_SHOW_DETAIL_3 : m_SW >= 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
			(m_SW >= 1920 ? IDR_HIDE_DETAIL_3 : m_SW >= 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
		m_WFShowFullInfoButton.SetBk(FALSE);
		m_WFShowFullInfoButton.SetBkColor(BLACK);
		m_WFShowFullInfoButton.SetTip(s);
		m_WFShowFullInfoButton.ShowWindow(SW_SHOW);
	}

	if (m_pWFExam2 != NULL)
	{
		CString s = (m_pWndSettings[0].m_SimplifiedWFPanel ? "Show Details" : "Hide Details");
		::SetRect(&Rect, m_Rect[7].right - 25, m_Rect[7].top + 5, m_Rect[7].right - 5, m_Rect[7].top + 20);
		m_WFShowFullInfoButton2.Create(_T(""), _T(""), WS_CHILD, Rect, this, IDC_SHOW_FULL_INFO2);
		m_WFShowFullInfoButton2.SetImage(m_pWndSettings[0].m_SimplifiedWFPanel ?
			(m_SW >= 1920 ? IDR_SHOW_DETAIL_3 : m_SW >= 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
			(m_SW >= 1920 ? IDR_HIDE_DETAIL_3 : m_SW >= 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
		m_WFShowFullInfoButton2.SetBk(FALSE);
		m_WFShowFullInfoButton2.SetBkColor(BLACK);
		m_WFShowFullInfoButton2.SetTip(s);
		m_WFShowFullInfoButton2.ShowWindow(SW_SHOW);
	}

	ShowWindow(SW_SHOW);
}

//***************************************************************************************
void CWFDoubleSumWnd::RepaintMemDC()
{
	CSumWnd::RepaintMemDC();

	BOOL Rows[22] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

	{
		if (m_pWFExam1)
		{
			if (m_InsideZone[m_d] == TRUE)
			{
				m_pWFExam1->Process();//530

									  //for average exam 
				CString ExamId, LoadFileName;
				GUIDToStr(m_pWFExam1->m_Header.m_ExamID, ExamId);
				LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

				if (::PathFileExists(LoadFileName))
				{
					CAveExamInfo AveExamInfo;

					AveExamInfo.LoadFromFile(LoadFileName);
					for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
					{
						m_pWFExam1->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
					}
				}
				// for average exam Done
			}

			if (m_pWndSettings[0].m_SimplifiedWFPanel == 1)
				ChangWFExamInfo(m_pWFExam1, m_Rect[6], 1, 0);
			else WFExamInfo(m_pWFExam1, m_Rect[6], 22, Rows);

			VisualComplaints(m_pWFExam1, m_Rect[8], 22);
		}

		if (m_pWFExam2)
		{
			if (m_InsideZone[m_d] == TRUE)
			{
				m_pWFExam2->Process();//530

									  // for average exam 
				CString ExamId, LoadFileName;
				GUIDToStr(m_pWFExam2->m_Header.m_ExamID, ExamId);
				LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

				if (::PathFileExists(LoadFileName))
				{
					CAveExamInfo AveExamInfo;

					AveExamInfo.LoadFromFile(LoadFileName);
					for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
					{
						m_pWFExam2->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
					}
				}
				// for average exam Done
			}

			if (m_pWndSettings[0].m_SimplifiedWFPanel == 1)
				ChangWFExamInfo(m_pWFExam2, m_Rect[7], 1, FALSE);
			else WFExamInfo(m_pWFExam2, m_Rect[7], 22, Rows);

			VisualComplaints(m_pWFExam2, m_Rect[9], 22);
		}
	}

	// Draw the glass icon for print out  
	if (m_Printing || m_Saving)
	{
		int GUIType = m_pWndSettings[0].m_Type;

		if (GUIType != TYPE_TEYE && GUIType != TYPE_DOFF && m_pWFExam1)
		{
			int MaskType = m_pWndSettings[0].m_Mask.GetType();
			DrawGlassIcon(MaskType, 1, 0);
		}

		GUIType = m_pWndSettings[3].m_Type;

		if (GUIType != TYPE_TEYE && GUIType != TYPE_DOFF && m_pWFExam2)
		{
			int MaskType = m_pWndSettings[3].m_Mask.GetType();
			DrawGlassIcon(MaskType, 1, 3);
		}
	}
	// Draw the glass icon for print out  Done
}

//***************************************************************************************

void CWFDoubleSumWnd::CreateChildWnd()
{
	for (int d = 0; d < 6; d++) SAFE_DELETE(m_pDispWnd[d]);

	if (m_InsideZone[m_d] == TRUE)
	{
		if (m_pWFExam1) m_pWFExam1->Presbia24Process();
		if (m_pWFExam2) m_pWFExam2->Presbia24Process();
	}
	else
	{
		if (m_pWFExam1)
		{
			m_pWFExam1->Process();

			// for average exam 
			CString ExamId, LoadFileName;
			GUIDToStr(m_pWFExam1->m_Header.m_ExamID, ExamId);
			LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

			if (::PathFileExists(LoadFileName))
			{
				CAveExamInfo AveExamInfo;

				AveExamInfo.LoadFromFile(LoadFileName);
				for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
				{
					m_pWFExam1->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
				}
			}
			// for average exam Done
		}
		if (m_pWFExam2)
		{
			m_pWFExam2->Process();

			// for average exam 
			CString ExamId, LoadFileName;
			GUIDToStr(m_pWFExam2->m_Header.m_ExamID, ExamId);
			LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

			if (::PathFileExists(LoadFileName))
			{
				CAveExamInfo AveExamInfo;

				AveExamInfo.LoadFromFile(LoadFileName);
				for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
				{
					m_pWFExam2->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
				}
			}
			// for average exam Done
		}
	}

	for (int i = 0; i < 6; i++)
	{
		m_InsideZone[i] = m_InsideZone[m_d];
		if (m_InsideZone[m_d] == TRUE) m_pWndSettings[i].m_ZoneRadiusUm = m_pWndSettings[m_d].m_ZoneRadiusUm;
	}
	::NewSettings.m_ShowPreZone_WF_VFAWnd = m_InsideZone[m_d];

	// type
	if (m_pWndSettings[0].m_Type != TYPE_TWFM) m_pWndSettings[0].m_Type = TYPE_TRFM;
	m_pWndSettings[1].m_Type = TYPE_TPSF;
	m_pWndSettings[2].m_Type = TYPE_TLTR;
	{

		CWndSettings *pWndSettings = &m_pWndSettings[2];

		if (!(pWndSettings->m_Ltr == 0 || pWndSettings->m_Ltr == 1))
		{
			pWndSettings->m_Ltr = 0;
		}

		if (!(pWndSettings->m_LtrLine == 20 || pWndSettings->m_LtrLine == 25 || pWndSettings->m_LtrLine == 30 ||
			pWndSettings->m_LtrLine == 40 || pWndSettings->m_LtrLine == 50 || pWndSettings->m_LtrLine == 70 ||
			pWndSettings->m_LtrLine == 100 || pWndSettings->m_LtrLine == 200))
		{
			pWndSettings->m_LtrLine = 40;
		}

		if (!(pWndSettings->m_LtrOrientation == 0 || pWndSettings->m_LtrOrientation == 90 ||
			pWndSettings->m_LtrOrientation == 180 || pWndSettings->m_LtrOrientation == 270))
		{
			pWndSettings->m_LtrOrientation = 0;
		}
	}

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
		if (Greater == 2) m_d = 3;
		else m_d = 0;
		for (int d = 0; d < 3; d++)
		{
			if (m_pWFExam1) m_pWndSettings[d].m_ZoneRadiusUm = intRound(m_pWFExam1->m_WfSurface.m_r_max_um);
		}
		for (int d = 3; d < 6; d++)
		{
			if (m_pWFExam2) m_pWndSettings[d].m_ZoneRadiusUm = intRound(m_pWFExam2->m_WfSurface.m_r_max_um);
		}
	}

	m_Togther = TRUE;// Increase or decrease togther

	if (Greater != 0)
	{
		if (m_pWndSettings[3].m_ZoneRadiusUm != 0)
		{
			if (m_pWndSettings[m_d].m_ZoneRadiusUm <= r_min_max)
				m_Togther = TRUE;
			else
			{
				m_Togther = FALSE;

				for (int d = 0; d < 3; d++)
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

				for (int d = 3; d < 6; d++)
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
			for (int d = 0; d < 6; d++)
			{
				m_pWndSettings[d].m_ZoneRadiusUm = r_min_max;

				// size
				m_pWndSettings[d].m_SizeSmall = TRUE;
				// method
				m_pWndSettings[d].m_Method3D = FALSE;

				if (d != 0)
				{
					m_pWndSettings[d].m_Mask = m_pWndSettings[0].m_Mask;
					m_pWndSettings[d].m_Piston = m_pWndSettings[0].m_Piston;
					m_pWndSettings[d].m_Tilt = m_pWndSettings[0].m_Tilt;
				}
			}
		}
	}


	if (m_Togther)
	{
		for (int d = 0; d < 6; d++)
		{
			// size
			m_pWndSettings[d].m_SizeSmall = TRUE;
			// method
			m_pWndSettings[d].m_Method3D = FALSE;
			if (d != m_d)
			{
				// zone
				m_pWndSettings[d].m_ZoneRadiusUm = m_pWndSettings[m_d].m_ZoneRadiusUm;

				// mask
				m_pWndSettings[d].m_Mask = m_pWndSettings[m_d].m_Mask;
				m_pWndSettings[d].m_Piston = m_pWndSettings[m_d].m_Piston;
				m_pWndSettings[d].m_Tilt = m_pWndSettings[m_d].m_Tilt;
			}
		}
	}


	real_t r_max_um = 5000;
	real_t r_max_um1, r_max_um2;

	if (m_pWFExam1) r_max_um1 = m_pWFExam1->m_WfSurface.m_r_max_um;
	if (m_pWFExam2) r_max_um2 = m_pWFExam2->m_WfSurface.m_r_max_um;

	if (m_pWFExam1) r_max_um1 = __min(r_max_um1, m_pWndSettings[0].m_ZoneRadiusUm);
	if (m_pWFExam2) r_max_um2 = __min(r_max_um2, m_pWndSettings[3].m_ZoneRadiusUm);

	if (m_pWFExam1) r_max_um = __min(r_max_um, m_pWFExam1->m_WfSurface.m_r_max_um);
	if (m_pWFExam2) r_max_um = __min(r_max_um, m_pWFExam2->m_WfSurface.m_r_max_um);
	r_max_um = __min(r_max_um, m_pWndSettings[0].m_ZoneRadiusUm);

	if (m_Togther)
	{
		if (r_max_um == r_min_max)
			m_TogtherEdge = TRUE;
		else m_TogtherEdge = FALSE;
	}
	else m_TogtherEdge = FALSE;


	CString PresbisRBs[2];
	if (m_InsideZone[m_d] == TRUE)
	{
		CZernikeSurface Surface;

		real_t SphEq, Sph, Cyl; int Axis;

		if (m_pWFExam1)
		{
			Surface = m_pWFExam1->m_WfSurface;
			Surface.ChangeRMaxUm(r_max_um);
			CWFExam::GetSpheqSphCylAxis(Surface, ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
			//
			if (::Settings.m_RefractionRounding == 0)
			{
				Sph = round8(Sph);
				Cyl = round8(Cyl);
			}
			PresbisRBs[0].Format(_T(" %+.2f D %+.2f D x %i° "), Sph, Cyl, Axis);
		}

		if (m_pWFExam2)
		{
			Surface = m_pWFExam2->m_WfSurface;
			Surface.ChangeRMaxUm(r_max_um);

			CWFExam::GetSpheqSphCylAxis(Surface, ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
			//
			if (::Settings.m_RefractionRounding == 0)
			{
				Sph = round8(Sph);
				Cyl = round8(Cyl);
			}
			PresbisRBs[1].Format(_T(" %+.2f D %+.2f D x %i° "), Sph, Cyl, Axis);
		}
	}

	for (int e = 0; e < 2; e++)
	{

		CWFExam* pWFExam = e == 0 ? m_pWFExam1 : m_pWFExam2;

		if (pWFExam == NULL) continue;

		CZernikeSurface Surface;
		Surface = pWFExam->m_WfSurface;

		if (m_Togther)
			Surface.ChangeRMaxUm(r_max_um);
		else
		{
			if (e == 0) Surface.ChangeRMaxUm(r_max_um1);
			else     Surface.ChangeRMaxUm(r_max_um2);
		}

		if (m_InsideZone[m_d] == TRUE) Surface.ChangeRMaxUm(1700);

		Surface &= m_pWndSettings[0].m_Mask;
		Surface.ComputePiston(m_pWndSettings[0].m_Piston);
		Surface.ComputeTilt(m_pWndSettings[0].m_Tilt);

		for (int d = e * 3; d < e * 3 + 3; d++)
		{
			m_Surfaces[d] = Surface;

			CWndSettings* pWndSettings = m_pWndSettings + (d % 3);

			switch (pWndSettings->m_Type)
			{

			case TYPE_TWFM:
			{
				CEyeWnd* pEyeWnd = new CEyeWnd(m_Rect[d], this);
				pEyeWnd->m_Type = TYPE_TWFM;
				pEyeWnd->m_SizeSmall = TRUE;
				pEyeWnd->m_Method3D = FALSE;
				pEyeWnd->m_LTLabel[0] = "Wavefront Map";
				pWndSettings->m_Mask.GetName(pEyeWnd->m_LTLabel[1]);

				if (m_InsideZone[d] == TRUE)
				{
					pEyeWnd->m_LTLabel[2] = "Presbia 2.4mm";
				}
				else
				{
					if (m_Togther)
						pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
					else
					{
						if (e == 0) pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um1 * 0.002);
						else     pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um2 * 0.002);
					}
				}

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
				for (int i = 0; i < pScale->m_NumColors; i++)
				{
					pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
				}
				pEyeWnd->m_pWFExam = pWFExam;

				if (m_InsideZone[d] != TRUE)
					CWFExam::CreateWfm2D(pEyeWnd->m_Map2D, Surface, 1.0 / pEyeWnd->m_y_px_um);
				else CWFExam::CreatePresbiaWfm2D(pEyeWnd->m_Map2D, Surface, 1.0 / pEyeWnd->m_y_px_um);

				pEyeWnd->CreateCentButtons();
				pEyeWnd->CreateZoomButtons();

				if (m_InsideZone[d] == TRUE) pEyeWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 2);
				else                      	pEyeWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 1);

				if (m_InsideZone[d] != TRUE) pEyeWnd->CreateZoneButtons();
				else
				{
					pEyeWnd->m_RBLabel[5] = PresbisRBs[e];
				}

				m_pDispWnd[d] = pEyeWnd;
			}
			break;

			case TYPE_TRFM:
			{
				CEyeWnd* pEyeWnd = new CEyeWnd(m_Rect[d], this);
				pEyeWnd->m_Type = TYPE_TRFM;
				pEyeWnd->m_SizeSmall = TRUE;
				pEyeWnd->m_Method3D = FALSE;
				pEyeWnd->m_LTLabel[0] = "Refraction Map";
				pWndSettings->m_Mask.GetName(pEyeWnd->m_LTLabel[1]);

				if (m_InsideZone[d] == TRUE)
				{
					pEyeWnd->m_LTLabel[2] = "Presbia 2.4mm";
				}
				else
				{
					if (m_Togther)
						pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
					else
					{
						if (e == 0) pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um1 * 0.002);
						else     pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um2 * 0.002);
					}
				}

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

				for (int i = 0; i < pScale->m_NumColors; i++)
				{
					pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
				}

				pEyeWnd->m_pWFExam = pWFExam;

				if (m_InsideZone[d] != TRUE)
					CWFExam::CreateRfm2D(pEyeWnd->m_Map2D, Surface, 1.0 / pEyeWnd->m_y_px_um);
				else CWFExam::CreatePresbiaRfm2D(pEyeWnd->m_Map2D, Surface, 1.0 / pEyeWnd->m_y_px_um);

				pEyeWnd->CreateCentButtons();
				pEyeWnd->CreateZoomButtons();

				if (m_InsideZone[d] == TRUE) pEyeWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 2);
				else                        pEyeWnd->CreateSoloMaskButtons(pWndSettings->m_Mask.GetType(), 1);

				if (m_InsideZone[d] != TRUE) pEyeWnd->CreateZoneButtons();
				else
				{
					pEyeWnd->m_RBLabel[5] = PresbisRBs[e];
				}

				m_pDispWnd[d] = pEyeWnd;
			}
			break;

			case TYPE_TPSF:
			{
				CPSFWnd* pPSFWnd = new CPSFWnd(m_Rect[d], this);
				CWFExam::CreatePsf(Surface, pPSFWnd->m_Psf, pWndSettings->m_EEFType);
				pPSFWnd->m_Type = TYPE_TPSF;
				pPSFWnd->m_SizeSmall = TRUE;
				pPSFWnd->m_Method3D = FALSE;
				pPSFWnd->m_LTLabel[0] = "Point Spread Function";
				pWndSettings->m_Mask.GetName(pPSFWnd->m_LTLabel[1]);

				if (m_InsideZone[d] == TRUE)
				{
					pPSFWnd->m_LTLabel[2] = "Presbia 2.4mm";
				}
				else
				{
					if (m_Togther)
						pPSFWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
					else
					{
						if (e == 0) pPSFWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um1 * 0.002);
						else     pPSFWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um2 * 0.002);
					}
				}

				pPSFWnd->m_ShowEEF = FALSE;
				pPSFWnd->CreateZoomButtons();

				if (m_InsideZone[d] != TRUE) pPSFWnd->CreateZoneButtons();
				else
				{
					pPSFWnd->m_RBLabel[5] = PresbisRBs[e];
				}

				m_pDispWnd[d] = pPSFWnd;
			}
			break;

			case TYPE_TLTR:
			{
				CLtrWnd* pLtrWnd = new CLtrWnd(m_Rect[d], this);
				pLtrWnd->m_DysVal = -1;
				CWFExam::CreateLtr(Surface, pLtrWnd->m_Ltr, pWndSettings->m_Ltr, pWndSettings->m_LtrLine, pWndSettings->m_LtrOrientation);
				pLtrWnd->m_Type = TYPE_TLTR;
				pLtrWnd->m_SizeSmall = TRUE;
				pLtrWnd->m_Method3D = FALSE;
				pLtrWnd->m_LTLabel[0] = "Letter";
				pWndSettings->m_Mask.GetName(pLtrWnd->m_LTLabel[1]);

				if (m_InsideZone[d] == TRUE)
				{
					pLtrWnd->m_LTLabel[2] = "Presbia 2.4mm";
				}
				else
				{
					if (m_Togther)
						pLtrWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
					else
					{
						if (e == 0) pLtrWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um1 * 0.002);
						else     pLtrWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um2 * 0.002);
					}
				}


				pLtrWnd->CreateZoomButtons();

				if (m_InsideZone[d] != TRUE) pLtrWnd->CreateZoneButtons();
				else
				{
					pLtrWnd->m_RBLabel[5] = PresbisRBs[e];
				}

				m_pDispWnd[d] = pLtrWnd;
			}
			break;
			}
		}
	}

	{
		CWndSettings* pWndSettings = m_pWndSettings;
		CScale* pScale = pWndSettings->GetScale();
		real_t Inc = pWndSettings->GetIncrement();
		real_t Cent = 0.0;
		real_t Step;

		if (pScale->m_StepAuto)
		{
			real_t Span = 0.0;
			for (int e = 0; e < 2; e++)
			{
				CEyeWnd* pEyeWnd = (CEyeWnd*)m_pDispWnd[e * 3];
				if (pEyeWnd)
				{
					real_t Min = pWndSettings->m_Method3D ? pEyeWnd->m_Map3D.m_min : pEyeWnd->m_Map2D.m_min;
					real_t Max = pWndSettings->m_Method3D ? pEyeWnd->m_Map3D.m_max : pEyeWnd->m_Map2D.m_max;
					Span = __max(Span, 2.0 * __max(fabs(Min - Cent), fabs(Max - Cent)));
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

		real_t StepMax = pWndSettings->GetMaxStep();

		if (Step > StepMax) Step = StepMax;

		for (int e = 0; e < 2; e++)
		{
			CEyeWnd* pEyeWnd = (CEyeWnd*)m_pDispWnd[e * 3];
			if (pEyeWnd) {
				pEyeWnd->m_Cent = Cent;
				pEyeWnd->m_Step = Step;
			}
		}
	}

	if (m_InsideZone[m_d] != TRUE)
	{
		for (int d = 0; d < 6; d++)
		{
			if (d < 3 && m_pWFExam1)
			{
				if (m_pWndSettings[d].m_ZoneRadiusUm >= m_pWFExam1->m_WfSurface.m_r_max_um)
				{
					m_pDispWnd[d]->m_IncZoneButton.SetEnabled(FALSE);
					m_pDispWnd[d]->m_IncZoneButton.Repaint();
				}
				if (m_pWndSettings[d].m_ZoneRadiusUm == 500)
				{
					m_pDispWnd[d]->m_DecZoneButton.SetEnabled(FALSE);
					m_pDispWnd[d]->m_DecZoneButton.Repaint();
				}
			}

			if (d >= 3 && m_pWFExam2)
			{
				if (m_pWndSettings[d].m_ZoneRadiusUm >= m_pWFExam2->m_WfSurface.m_r_max_um)
				{
					m_pDispWnd[d]->m_IncZoneButton.SetEnabled(FALSE);
					m_pDispWnd[d]->m_IncZoneButton.Repaint();
				}

				if (m_pWndSettings[d].m_ZoneRadiusUm == 500)
				{
					m_pDispWnd[d]->m_DecZoneButton.SetEnabled(FALSE);
					m_pDispWnd[d]->m_DecZoneButton.Repaint();
				}
			}
		}
	}
}

//***************************************************************************************

void CWFDoubleSumWnd::CreateMenu()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_d == 0 || m_d == 3) {
		m_pMenu->AppendItem(IDC_TYPE_TWFM_ITEM, pWndSettings->m_Type == TYPE_TWFM, FALSE, "Wavefront Map", "");
		m_pMenu->AppendItem(IDC_TYPE_TRFM_ITEM, pWndSettings->m_Type == TYPE_TRFM, FALSE, "Refraction Map", "");
		m_pMenu->AppendSeparator();
	}

	int type = 0;
	if (::TempSettings.Com_r_max_um <= 700) type = 1;
	CreateSubmenuZone(type);

	CreateSubmenuMask();

	if (m_d == 0 || m_d == 3)
	{
		m_pMenu->AppendSeparator();

		CreateSubmenuMapSteps();
		CreateSubmenuMapColors();
	}

	else if (m_d == 2 || m_d == 5)
	{
		m_pMenu->AppendSeparator();

		CreateSubmenuLtr();
	}

	if (pWndSettings->m_Type == TYPE_TWFM ||
		pWndSettings->m_Type == TYPE_TRFM ||
		pWndSettings->m_Type == TYPE_TLTR ||
		pWndSettings->m_Type == TYPE_TRMS)
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_SAVE_TXT_ITEM, FALSE, FALSE, "Save Zernike coefficients into file", "");
	}

	if (m_d == 0 && !SameVfa0())
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
	}
	if (m_d == 1 && !SameVfa1())
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
	}
	if (m_d == 2 && !SameVfa2())
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
	}
	if (m_d == 3 && !SameVfa3())
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
	}
	if (m_d == 4 && !SameVfa4())
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
	}
	if (m_d == 5 && !SameVfa5())
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
	}
	//521
}

//***************************************************************************************

LRESULT CWFDoubleSumWnd::OnChangeCentMsg(WPARAM wParam, LPARAM lParam)
{
	if (!m_pDispWnd[0] || !m_pDispWnd[3]) return 0;

	if (wParam == (WPARAM)m_pDispWnd[0])
	{
		((CEyeWnd*)m_pDispWnd[3])->m_Cent = ((CEyeWnd*)m_pDispWnd[0])->m_Cent;
		m_pDispWnd[3]->Repaint();
	}
	else if (wParam == (WPARAM)m_pDispWnd[3])
	{
		((CEyeWnd*)m_pDispWnd[0])->m_Cent = ((CEyeWnd*)m_pDispWnd[3])->m_Cent;
		m_pDispWnd[0]->Repaint();
	}

	return 0;
}

//***************************************************************************************

LRESULT CWFDoubleSumWnd::OnChangeStepMsg(WPARAM wParam, LPARAM lParam)
{
	if (!m_pDispWnd[0] || !m_pDispWnd[3]) return 0;

	if (wParam == (WPARAM)m_pDispWnd[0])
	{
		((CEyeWnd*)m_pDispWnd[3])->m_Step = ((CEyeWnd*)m_pDispWnd[0])->m_Step;
		m_pDispWnd[3]->Repaint();
	}
	else if (wParam == (WPARAM)m_pDispWnd[3])
	{
		((CEyeWnd*)m_pDispWnd[0])->m_Step = ((CEyeWnd*)m_pDispWnd[3])->m_Step;
		m_pDispWnd[0]->Repaint();
	}

	return 0;
}

//***************************************************************************************

void CWFDoubleSumWnd::OnShowDefaultItemClicked()
{
	CBusyCursor Cursor;

	m_pWndSettings[m_d].m_LtrDistanceType = 0;

	//WF Vfa display
	if (m_d == 0 || m_d == 3)
	{
		m_pWndSettings[0].m_Type = 11;
		m_pWndSettings[3].m_Type = 11;
	}

	for (int i = 0; i < 6; i++)
	{
		m_pWndSettings[i].m_Mask.SetType(MASK_HO_TOTAL);
		m_pWndSettings[i].m_Piston = FALSE;
		m_pWndSettings[i].m_Tilt = FALSE;

		if (m_d == 2 || m_d == 5)
		{
			if (i != 0 && i != 3)
			{
				m_pWndSettings[i].m_Ltr = 0;
				m_pWndSettings[i].m_LtrLine = 70;
				m_pWndSettings[i].m_LtrOrientation = 0;
			}
		}

		if (m_d == 0 || m_d == 3)
		{
			if (i == 0 || i == 3)
			{
				CScale* pScale = m_pWndSettings[i].GetScale();
				pScale->m_Step = 0.5;

				m_pWndSettings[i].SetDefaultColors(1);
			}
		}
	}

	CreateChildWnd();

	Repaint();
}

//***************************************************************************************

BOOL CWFDoubleSumWnd::SameVfa0()
{
	if (m_pWndSettings[0].m_Type != 11)            return FALSE;
	if (m_pWndSettings[0].m_Mask.GetType() != MASK_HO_TOTAL) return FALSE;
	if (m_pWndSettings[0].m_Piston != FALSE)         return FALSE;
	if (m_pWndSettings[0].m_Tilt != FALSE)         return FALSE;

	CScale* pScale = m_pWndSettings[0].GetScale();
	if (pScale->m_Step != 0.5)           return FALSE;
	if (m_pWndSettings[0].AreDefaultColors(0))                  return FALSE;

	return TRUE;
}

//***************************************************************************************

BOOL CWFDoubleSumWnd::SameVfa1()
{
	if (m_pWndSettings[1].m_Mask.GetType() != MASK_HO_TOTAL) return FALSE;
	if (m_pWndSettings[1].m_Piston != FALSE)         return FALSE;
	if (m_pWndSettings[1].m_Tilt != FALSE)         return FALSE;

	return TRUE;
}

//***************************************************************************************

BOOL CWFDoubleSumWnd::SameVfa2()
{
	if (m_pWndSettings[2].m_Mask.GetType() != MASK_HO_TOTAL) return FALSE;
	if (m_pWndSettings[2].m_Piston != FALSE)         return FALSE;
	if (m_pWndSettings[2].m_Tilt != FALSE)         return FALSE;

	if (m_pWndSettings[2].m_Ltr != 0)             return FALSE;
	if (m_pWndSettings[2].m_LtrLine != 70)            return FALSE;
	if (m_pWndSettings[2].m_LtrOrientation != 0)             return FALSE;

	return TRUE;
}

//***************************************************************************************

BOOL CWFDoubleSumWnd::SameVfa3()
{
	if (m_pWndSettings[3].m_Type != 11)            return FALSE;
	if (m_pWndSettings[3].m_Mask.GetType() != MASK_HO_TOTAL) return FALSE;
	if (m_pWndSettings[3].m_Piston != FALSE)         return FALSE;
	if (m_pWndSettings[3].m_Tilt != FALSE)         return FALSE;

	CScale* pScale = m_pWndSettings[3].GetScale();
	if (pScale->m_Step != 0.5)           return FALSE;
	if (!m_pWndSettings[3].AreDefaultColors(1))                 return FALSE;

	return TRUE;
}

//***************************************************************************************

BOOL CWFDoubleSumWnd::SameVfa4()
{
	if (m_pWndSettings[4].m_Mask.GetType() != MASK_HO_TOTAL) return FALSE;
	if (m_pWndSettings[4].m_Piston != FALSE)         return FALSE;
	if (m_pWndSettings[4].m_Tilt != FALSE)         return FALSE;

	return TRUE;
}

//***************************************************************************************

BOOL CWFDoubleSumWnd::SameVfa5()
{
	if (m_pWndSettings[5].m_Mask.GetType() != MASK_HO_TOTAL) return FALSE;
	if (m_pWndSettings[5].m_Piston != FALSE)         return FALSE;
	if (m_pWndSettings[5].m_Tilt != FALSE)         return FALSE;

	if (m_pWndSettings[5].m_Ltr != 0)             return FALSE;
	if (m_pWndSettings[5].m_LtrLine != 70)            return FALSE;
	if (m_pWndSettings[5].m_LtrOrientation != 0)             return FALSE;

	return TRUE;
}

//***************************************************************************************

void CWFDoubleSumWnd::OnShowWFTextInfoBtnClicked()
{
	m_pWndSettings[0].m_SimplifiedWFPanel = !m_pWndSettings[0].m_SimplifiedWFPanel;
	Repaint();

	CString s = (m_pWndSettings[0].m_SimplifiedWFPanel ? "Show Details" : "Hide Details");

	m_WFShowFullInfoButton.SetImage(m_pWndSettings[0].m_SimplifiedWFPanel ?
		(m_SW >= 1920 ? IDR_SHOW_DETAIL_3 : m_SW >= 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
		(m_SW >= 1920 ? IDR_HIDE_DETAIL_3 : m_SW >= 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));

	m_WFShowFullInfoButton.SetTip(s);
	m_WFShowFullInfoButton.Repaint();

	if (m_pWFExam2 != NULL)
	{
		m_WFShowFullInfoButton2.SetImage(m_pWndSettings[0].m_SimplifiedWFPanel ?
			(m_SW >= 1920 ? IDR_SHOW_DETAIL_3 : m_SW >= 1600 ? IDR_SHOW_DETAIL_2 : IDR_SHOW_DETAIL_1) :
			(m_SW >= 1920 ? IDR_HIDE_DETAIL_3 : m_SW >= 1600 ? IDR_HIDE_DETAIL_2 : IDR_HIDE_DETAIL_1));
		m_WFShowFullInfoButton2.SetTip(s);
		m_WFShowFullInfoButton2.Repaint();
	}
}

//***************************************************************************************

void CWFDoubleSumWnd::OnShowWFTextInfoBtn2Clicked()
{
	OnShowWFTextInfoBtnClicked();
}

//***************************************************************************************
