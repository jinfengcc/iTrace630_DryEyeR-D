// The display for Depth of Focus Comparison

//迷雾汀芳，暗夜黄昏，浮动花影，适彼中秋

//寂静昭华，君子瑟琴，随云起舞，素手点星

//作者： 陈劲风

//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "DOFWnd.h"
#include "BusyCursor.h"
#include "DOFCWnd.h"

#include "WFDOFCWnd.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CWFDOFCWnd, CSumWnd)
	ON_COMMAND(IDC_DEFAULT_GUI_ITEM, OnShowDefaultItemClicked)//521	
END_MESSAGE_MAP()

//***************************************************************************************

CWFDOFCWnd::CWFDOFCWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CWFExam* pWFExam1, CWFExam* pWFExam2, CWndSettings* pWndSettings) :
	CSumWnd(pWnd, WndRect, pPatient, pWndSettings)
{
	CBusyCursor Cursor;

	//----------------------------------------------------
	RECT Rect;
	GetWindowRect(&Rect);

	real_t t = 3.0 * (0.03 * m_h) + m_g;
	real_t w = ((Rect.right - Rect.left) - 4.0 * m_g) / 3.0;
	real_t h = ((Rect.bottom - Rect.top - t) - 2.0 * m_g) / 4.0;
	::SetRect(&m_Rect[0], intRound(m_g), intRound(t), intRound(m_g + w + m_g + w + m_g + w), intRound(t + 3 * h)); // depth of focus comparison map
	::SetRect(&m_Rect[1], intRound(m_g), intRound(t + 3 * h + m_g), intRound(m_g + w), intRound(t + 4 * h + m_g)); // info 1
	::SetRect(&m_Rect[2], intRound(m_g + w + m_g), intRound(t + 3 * h + m_g), intRound(m_g + w + m_g + w), intRound(t + 4 * h + m_g)); // info 2
	::SetRect(&m_Rect[3], intRound(m_g + w + m_g + w + m_g), intRound(t + 3 * h + m_g), intRound(m_g + w + m_g + w + m_g + w), intRound(t + 4 * h + m_g));// info 3
																																						  //----------------------------------------------------

	m_pWFExam1 = pWFExam1;
	m_pWFExam2 = pWFExam2;

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
	//for average exam Done

	//
	m_pWFExam1->Process();
	m_pWFExam2->Process();
	//

	CZernikeSurface Surfaces[2];
	Surfaces[0] = m_pWFExam1->m_WfSurface;
	Surfaces[1] = m_pWFExam2->m_WfSurface;
	// change diameter
	//530
	real_t n_r_max_um = 5000.0;
	n_r_max_um = __min(n_r_max_um, Surfaces[0].m_r_max_um);
	n_r_max_um = __min(n_r_max_um, Surfaces[1].m_r_max_um);
	//530

	Surfaces[0].ChangeRMaxUm(n_r_max_um);
	Surfaces[1].ChangeRMaxUm(n_r_max_um);
	// mask

	CZernikeMask Mask;
	Mask.SetType(MASK_TOTAL);

	Surfaces[0] &= Mask;//530
	Surfaces[0].ComputePiston(pWndSettings->m_Piston);
	Surfaces[0].ComputeTilt(pWndSettings->m_Tilt);
	Surfaces[1] &= Mask;//530
	Surfaces[1].ComputePiston(pWndSettings->m_Piston);
	Surfaces[1].ComputeTilt(pWndSettings->m_Tilt);

	// subtract
	m_Surfaces[3] = Surfaces[0];
	m_Surfaces[3] -= Surfaces[1];
	//521

	//Calculate the Refraction
	real_t SphEq, Sph, Cyl;
	int Axis;

	CZernikeSurface Surface1 = m_pWFExam1->m_WfSurface;
	Surface1.ChangeRMaxUm(2000.0);
	CWFExam::GetSpheqSphCylAxis(Surface1, ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
	//
	if (::Settings.m_RefractionRounding == 0) {
		Sph = round8(Sph);
		Cyl = round8(Cyl);
	}

	m_fraction1 = Sph + Cyl / 2.0;

	CZernikeSurface Surface2 = m_pWFExam2->m_WfSurface;
	Surface2.ChangeRMaxUm(2000.0);
	CWFExam::GetSpheqSphCylAxis(Surface2, ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
	//
	if (::Settings.m_RefractionRounding == 0) {
		Sph = round8(Sph);
		Cyl = round8(Cyl);
	}

	m_fraction2 = Sph + Cyl / 2.0;
	//Calculate the Refraction

	CreateChildWnd();

	Repaint();

	ShowWindow(SW_SHOW);
}

//***************************************************************************************

void CWFDOFCWnd::RepaintMemDC()
{
	CSumWnd::RepaintMemDC();

	if (m_pWFExam1) {
		WFExamSmallInfo(m_pWFExam1, m_Rect[1], 1, 0);
	}

	if (m_pWFExam2) {
		WFExamSmallInfo(m_pWFExam2, m_Rect[2], 1, 1);
	}

	if (m_pWFExam1 && m_pWFExam2) {
		CWFExam* pWFExam3 = new CWFExam;
		pWFExam3->m_WfSurface = m_Surfaces[3];

		WFExamSmallInfo(pWFExam3, m_Rect[3], 0, 2);
	}
}

//***************************************************************************************

void CWFDOFCWnd::CreateChildWnd()
{
	CBusyCursor Cursor;



	SAFE_DELETE(m_pDispWnd[0]);

	CWndSettings* pWndSettings = GetWndSettings();

	if (pWndSettings->m_Type != TYPE_DOFC)
	{
		pWndSettings->m_Type = TYPE_DOFC;
	}

	if (m_X1.GetSize() == 0)
	{
		::CreateThroughFocusVSOTFA(m_pWFExam1->m_WfSurface, m_X1, m_F1);
	}
	if (m_X2.GetSize() == 0)
	{
		::CreateThroughFocusVSOTFA(m_pWFExam2->m_WfSurface, m_X2, m_F2);
	}

	CDOFCWnd* pDOFCWnd = new CDOFCWnd(m_Rect[0], m_X1, m_F1, m_X2, m_F2, m_fraction1, m_fraction2, this);
	pDOFCWnd->m_Type = pWndSettings->m_Type;
	pDOFCWnd->m_LTLabel[0] = "Through-focus VSOTFa Comparison";

	m_pDispWnd[0] = pDOFCWnd;
}

//***************************************************************************************

void CWFDOFCWnd::CreateMenu()
{
	m_d = m_d;
	CWndSettings* pWndSettings = GetWndSettings();

	//521
	m_pMenu->AppendItem(IDC_TYPE_TEYE_ITEM, pWndSettings->m_Type == TYPE_TEYE, FALSE, "Eye Image", "");

	m_pMenu->AppendItem(IDC_TYPE_TWFM_ITEM, pWndSettings->m_Type == TYPE_TWFM, FALSE, "Wavefront Map", "");
	m_pMenu->AppendItem(IDC_TYPE_TRFM_ITEM, pWndSettings->m_Type == TYPE_TRFM, FALSE, "Refraction Map", "");
	m_pMenu->AppendItem(IDC_TYPE_TPSF_ITEM, pWndSettings->m_Type == TYPE_TPSF, FALSE, "Point Spread Function", "");
	m_pMenu->AppendItem(IDC_TYPE_TMTF_ITEM, pWndSettings->m_Type == TYPE_TMTF, FALSE, "Modulation Transfer Function", "");
	m_pMenu->AppendItem(IDC_TYPE_TLTR_ITEM, pWndSettings->m_Type == TYPE_TLTR, FALSE, "Visual Acuity Chart", "");
	
	//CreateSubmenuNLtr();
	
	m_pMenu->AppendItem(IDC_TYPE_TEEF_ITEM, pWndSettings->m_Type == TYPE_TEEF, FALSE, "Encircled Energy Function", "");
	m_pMenu->AppendItem(IDC_TYPE_TRMS_ITEM, pWndSettings->m_Type == TYPE_TRMS, FALSE, "Root Mean Square", "");
	m_pMenu->AppendItem(IDC_TYPE_TAZR_ITEM, pWndSettings->m_Type == TYPE_TAZR, FALSE, "Annular / Zonal Refraction", "");

	if (m_isCombo) m_pMenu->AppendItem(IDC_TYPE_DOFF_ITEM, pWndSettings->m_Type == TYPE_DOFF, FALSE, "Depth of Focus Function", "");//531

	m_pMenu->AppendSeparator();
	m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
}

//***************************************************************************************
//521
void CWFDOFCWnd::OnShowDefaultItemClicked()
{
	CBusyCursor Cursor;

	CWndSettings* pWndSettings = GetWndSettings();

	pWndSettings->m_LtrDistanceType = 0;//530

	CreateChildWnd();

	Repaint();
}

//***************************************************************************************
//521
BOOL CWFDOFCWnd::SameDepthFocus()
{
	if (m_pWndSettings[m_d].m_Type != TYPE_DOFF) return FALSE;
	if (m_pWndSettings[m_d].m_SizeSmall == TRUE)      return FALSE;

	return TRUE;
}
//521