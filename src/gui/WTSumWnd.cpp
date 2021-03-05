//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "WTSumWnd.h"
#include "EyeWnd.h"
#include "BusyCursor.h"
#include "Export.h"
#include "LensDlg.h"
#include "ImageDlg.h"
#include "ExamDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CWTSumWnd, CSumWnd)

	ON_COMMAND(IDC_EDIT_LENS_DOTS_ITEM, OnEditLensDotsItemClicked)
	ON_COMMAND(IDC_EDIT_LIMBUS_ITEM, OnEditLimbusItemClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CWTSumWnd::CWTSumWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CWFExam* pWFExam, CCTExam* pCTExam, CWndSettings* pWndSettings) :
	CSumWnd(pWnd, WndRect, pPatient, pWndSettings)
{
	CBusyCursor Cursor;

	//----------------------------------------------------
	RECT Rect;
	GetWindowRect(&Rect);
	real_t t = 3.0 * (0.03 * m_h) + m_g;
	real_t w = ((Rect.right - Rect.left) - 4.0 * m_g) / 3.0;
	real_t h = ((Rect.bottom - Rect.top - t) - 2.0 * m_g) / 2.0;
	::SetRect(&m_Rect[0], intRound(m_g), intRound(t), intRound(m_g + w), intRound(t + h)); // eye image
	::SetRect(&m_Rect[1], intRound(m_g + w + m_g), intRound(t), intRound(m_g + w + m_g + w), intRound(t + h)); // wavefront map
	::SetRect(&m_Rect[2], intRound(m_g + w + m_g), intRound(t + h + m_g), intRound(m_g + w + m_g + w), intRound(t + h + m_g + h)); // axial map
	::SetRect(&m_Rect[3], intRound(m_g), intRound(t + h + m_g), intRound(m_g + w), intRound(t + h + m_g + h)); // lens info
	::SetRect(&m_Rect[4], intRound(m_g + w + m_g + w + m_g), intRound(t), intRound(m_g + w + m_g + w + m_g + w), intRound(t + h)); // wf exam info
	::SetRect(&m_Rect[5], intRound(m_g + w + m_g + w + m_g), intRound(t + h + m_g), intRound(m_g + w + m_g + w + m_g + w), intRound(t + h + m_g + h)); // ct exam info
																																					   //----------------------------------------------------

	m_pWFExam = pWFExam;
	m_pCTExam = pCTExam;

	//*007*[cjf***05052012],record the increase and decrease range
	if (m_pCTExam == NULL)
		::TempSettings.Com_r_max_um = m_pWFExam->m_WfSurface.m_r_max_um;
	else
		::TempSettings.Com_r_max_um = ((m_pWFExam->m_WfSurface.m_r_max_um) < (m_pCTExam->m_WfSurface.m_r_max_um) ? m_pWFExam->m_WfSurface.m_r_max_um : m_pCTExam->m_WfSurface.m_r_max_um);
	//*007*[cjf***05052012]

	for (m_d = 0; m_d < (m_pCTExam ? 3 : 2); m_d++) {
		CreateChildWnd();
	}

	Repaint();

	ShowWindow(SW_SHOW);

	//6.3.0 Popup the lens dot dialog 
	if (m_pWFExam->m_Image.m_le_ok == FALSE)
	{
		m_d = 0;
		OnEditLensDotsItemClicked();
	}
	//6.3.0
}

//***************************************************************************************

void CWTSumWnd::LensInfo(RECT Rect, int NumRows)
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);

	CString s, s1;

	int L = Rect.left, T = Rect.top, R = Rect.right, B = Rect.bottom;

	real_t t = T;
	real_t h = (Rect.bottom - Rect.top) / (real_t)NumRows;

	CMFont Font(intRound(1.1 * h), 400, "Arial");

	//
	::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
	m_MemDC.WriteText(" Acquisition Lens", Rect, Font, white, 0);

	//
	t += 1.5 * h;
	::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
	if (m_pWFExam->m_WavetouchLensPower == INVALID_VALUE) s = "   Power  --.-- D";
	else s.Format(_T("   Power  %.2f D"), m_pWFExam->m_WavetouchLensPower);
	m_MemDC.WriteText(s, Rect, Font, white, 0);

	//
	t += h;
	::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
	switch (m_pWFExam->m_WavetouchLensBaseCurve) {
	case WAVETOUCH_LENS_BASECURVE_MEDIAN: s = "   Base Curve  Median"; break;
	case WAVETOUCH_LENS_BASECURVE_STEEP: s = "   Base Curve  Steep"; break;
	default: s = "   Base Curve  Unknown"; break;
	}
	m_MemDC.WriteText(s, Rect, Font, white, 0);

	//
	t += h;
	::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
	s = "   Other  " + m_pWFExam->m_WavetouchOther;
	m_MemDC.WriteText(s, Rect, Font, white, 0);

	//
	t += 1.5 * h;
	::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
	m_MemDC.WriteText(" Manifest Refraction", Rect, Font, white, 0);

	//
	t += 1.5 * h;
	::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
	if (m_pWFExam->m_WavetouchSph == INVALID_VALUE) {
		s = "   Sph  --.-- D";
	}
	else {
		s.Format(_T("   Sph  %.2f D"), m_pWFExam->m_WavetouchSph);
	}
	if (m_pWFExam->m_WavetouchCyl == INVALID_VALUE) {
		s += "   Cyl  --.-- D";
	}
	else {
		s1.Format(_T("   Cyl  %.2f D"), m_pWFExam->m_WavetouchCyl);
		s += s1;
	}
	if (m_pWFExam->m_WavetouchAxis == INVALID_VALUE) {
		s += "   Cyl  --.-- D";
	}
	else {
		s1.Format(_T(" x %i�"), m_pWFExam->m_WavetouchAxis);
		s += s1;
	}
	m_MemDC.WriteText(s, Rect, Font, white, 0);

	//
	t += 1.5 * h;
	::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
	m_MemDC.WriteText(" WaveTouch Notes", Rect, Font, white, 0);

	//
	t += 0.5 * h;
	s = m_pWFExam->m_WavetouchNote;
	int m = 10;
	int l = s.GetLength();
	while (l > 0 && m) {
		int i;
		for (i = 1; i <= l; i++) {
			if (s[i - 1] == '\n') break;
			m_MemDC.MeasureRect("   " + s.Left(i) + " ", Rect, Font);
			if (Rect.right - Rect.left > R - L) {
				for (; i >= 1; i--) {
					if (s[i - 1] == ' ') break;
				}
				break;
			}
		}

		t += h;
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText("   " + s.Left(i) + " ", Rect, Font, white, 0);

		s = s.Right(l - i);
		l -= i;
		m--;
	}
}

//***************************************************************************************

void CWTSumWnd::RepaintMemDC()
{
	CSumWnd::RepaintMemDC();

	BOOL Rows[22] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

	if (m_pWFExam) {
		WFExamInfo(m_pWFExam, m_Rect[4], 22, Rows);
		LensInfo(m_Rect[3], 22);
	}

	if (m_pCTExam) {
		CTExamInfo(m_pCTExam, m_Rect[5], 22, Rows);
	}

	CWavetouchLogo Logo(m_Printing ? 1 : 0);
	m_MemDC.DrawImage(Logo, intRound(2.0 * m_g), m_h - intRound(2.0 * m_g), 0, 2, TRUE);
}

//***************************************************************************************

void CWTSumWnd::CreateChildWnd()
{
	SAFE_DELETE(m_pDispWnd[m_d]);

	CWndSettings* pWndSettings = m_pWndSettings + m_d;

	switch (m_d) {

	case 0:
	{
		CEyeWnd* pEyeWnd = new CEyeWnd(m_Rect[0], this);
		pEyeWnd->m_Type = TYPE_TEYE;
		pEyeWnd->m_SizeSmall = TRUE;
		pEyeWnd->m_Method3D = FALSE;
		pEyeWnd->m_MapShowEye = TRUE;
		pEyeWnd->m_MapShowPupil = pWndSettings->m_MapShowPupil;
		pEyeWnd->m_MapShowCornealVertex = pWndSettings->m_MapShowCornealVertex;
		pEyeWnd->m_MapShowPoints = pWndSettings->m_MapShowPoints;
		pEyeWnd->m_MapShowLimbus = pWndSettings->m_MapShowLimbus;
		pEyeWnd->m_MapShowWavetouchLens = pWndSettings->m_MapShowWavetouchLens;
		pEyeWnd->m_pWFExam = m_pWFExam;
		m_pDispWnd[0] = pEyeWnd;
	}
	break;

	case 1:
	{
		CEyeWnd* pEyeWnd = new CEyeWnd(m_Rect[1], this);
		pEyeWnd->m_Type = TYPE_TWFM;
		pEyeWnd->m_SizeSmall = TRUE;
		pEyeWnd->m_Method3D = FALSE;
		pEyeWnd->m_LTLabel[0] = "Wavefront Map";

		pWndSettings->m_Mask.SetType(MASK_TOTAL);
		pWndSettings->m_Mask.GetName(pEyeWnd->m_LTLabel[1]);

		pWndSettings->m_ZoneRadiusUm = 5000;
		real_t r_max_um = 5000.0;
		r_max_um = __min(r_max_um, m_pWFExam->m_WfSurface.m_r_max_um);
		r_max_um = __min(r_max_um, pWndSettings->m_ZoneRadiusUm);

		pEyeWnd->m_LTLabel[2].Format(_T("%.2f mm"), 0.002 * r_max_um);

		pEyeWnd->m_MapShowEye = pWndSettings->m_MapShowEye;
		pEyeWnd->m_MapShowMap = TRUE;
		pEyeWnd->m_MapShowNumbers = FALSE;
		pEyeWnd->m_MapShowPupil = FALSE;
		pEyeWnd->m_MapShowCornealVertex = FALSE;
		pEyeWnd->m_MapShowLimbus = FALSE;
		pEyeWnd->m_MapShowSolidSurface = FALSE;
		pEyeWnd->m_MapShowWireMesh = FALSE;
		pEyeWnd->m_MapTranslucent = FALSE;
		pEyeWnd->m_Unit = "�";
		pEyeWnd->m_Inc = pWndSettings->GetIncrement();
		CScale* pScale = pWndSettings->GetScale();
		pEyeWnd->m_NumColors = pScale->m_NumColors;
		for (int i = 0; i < pScale->m_NumColors; i++) {
			pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
		}
		pEyeWnd->m_pWFExam = m_pWFExam;

		CWFExam::CreateWfm2D(pEyeWnd->m_Map2D, m_pWFExam->m_WfSurface, 1.0 / pEyeWnd->m_y_px_um);

		pEyeWnd->CreateCentButtons();
		pEyeWnd->CreateZoomButtons();

		m_pDispWnd[1] = pEyeWnd;

		real_t Inc = pWndSettings->GetIncrement();

		real_t Cent = 0.0;

		real_t Step;
		if (pScale->m_StepAuto) {
			real_t Min = pEyeWnd->m_Map2D.m_min;
			real_t Max = pEyeWnd->m_Map2D.m_max;
			real_t Span = 2.0 * __max(fabs(Min - Cent), fabs(Max - Cent));
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

		pEyeWnd->m_Cent = Cent;
		pEyeWnd->m_Step = Step;
	}
	break;

	case 2:
	{
		CEyeWnd* pEyeWnd = new CEyeWnd(m_Rect[2], this);
		pEyeWnd->m_Type = TYPE_CAXM;
		pEyeWnd->m_SizeSmall = TRUE;
		pEyeWnd->m_Method3D = FALSE;
		pEyeWnd->m_LTLabel[0] = "Axial Map";
		pEyeWnd->m_MapShowEye = pWndSettings->m_MapShowEye;
		pEyeWnd->m_MapShowMap = TRUE;
		pEyeWnd->m_MapShowNumbers = FALSE;
		pEyeWnd->m_MapShowPupil = FALSE;
		pEyeWnd->m_MapShowCornealVertex = FALSE;
		pEyeWnd->m_MapShowLimbus = FALSE;
		pEyeWnd->m_MapShowSolidSurface = FALSE;
		pEyeWnd->m_MapShowWireMesh = FALSE;
		pEyeWnd->m_MapTranslucent = FALSE;
		pEyeWnd->m_MapShowKeratometry = FALSE;
		pEyeWnd->m_MapTranslucent = FALSE;
		pEyeWnd->m_Unit = pWndSettings->m_MapUnit == DIOPTERS ? "D" : "mm";
		pEyeWnd->m_Inc = pWndSettings->GetIncrement();
		CScale* pScale = pWndSettings->GetScale();
		pEyeWnd->m_NumColors = pScale->m_NumColors;
		for (int i = 0; i < pScale->m_NumColors; i++) {
			pEyeWnd->m_Colors[i] = pScale->m_Colors[i];
		}
		pEyeWnd->m_pCTExam = m_pCTExam;

		m_pCTExam->CreateDpm2D(pEyeWnd->m_Map2D, TYPE_CAXM, pWndSettings->m_MapUnit, 1.0 / pEyeWnd->m_y_px_um);

		pEyeWnd->CreateCentButtons();
		pEyeWnd->CreateZoomButtons();

		m_pDispWnd[2] = pEyeWnd;

		real_t Inc = pWndSettings->GetIncrement();

		real_t Cent;
		if (pScale->m_CentAuto) {
			if (pWndSettings->m_MapUnit == DIOPTERS) {
				Cent = m_pCTExam->m_ax0_dp;
			}
			else {
				Cent = 0.001 * m_pCTExam->m_ax0_um;
			}
		}
		else {
			Cent = pScale->m_Cent;
		}
		Cent = RealRound(Cent, Inc);

		real_t Step;
		if (pScale->m_StepAuto) {
			real_t Min = pEyeWnd->m_Map2D.m_min;
			real_t Max = pEyeWnd->m_Map2D.m_max;
			real_t Span = 2.0 * __max(fabs(Min - Cent), fabs(Max - Cent));
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

		pEyeWnd->m_Cent = Cent;
		pEyeWnd->m_Step = Step;
	}
	break;

	}

}

//***************************************************************************************

void CWTSumWnd::CreateMenu()
{
	CWndSettings* pWndSettings = m_pWndSettings + m_d;
	CScale* pScale = pWndSettings->GetScale();
	CXPMenu* pSubMenu;

	switch (m_d) {

	case 0:
		// elements
		m_pMenu->AppendItem(IDC_SHOW_MAP_PUPIL_ITEM, FALSE, pWndSettings->m_MapShowPupil, "Show pupil", "");
		m_pMenu->AppendItem(IDC_SHOW_MAP_VERTEX_ITEM, FALSE, pWndSettings->m_MapShowCornealVertex, "Show corneal vertex", "");
		m_pMenu->AppendItem(IDC_SHOW_MAP_LIMBUS_ITEM, FALSE, pWndSettings->m_MapShowLimbus, "Show limbus", "");
		m_pMenu->AppendItem(IDC_SHOW_MAP_POINTS_ITEM, FALSE, pWndSettings->m_MapShowPoints, "Show scan points", "");
		m_pMenu->AppendItem(IDC_SHOW_MAP_LENS_ITEM, FALSE, pWndSettings->m_MapShowWavetouchLens, "Show lens dots", "");
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_EDIT_LIMBUS_ITEM, FALSE, FALSE, "Edit pupil, limbus and vertex", "");
		m_pMenu->AppendItem(IDC_EDIT_LENS_DOTS_ITEM, FALSE, FALSE, "Edit lens dots", "");
		break;

	case 1:
		// elements
		m_pMenu->AppendItem(IDC_SHOW_MAP_EYE_ITEM, FALSE, pWndSettings->m_MapShowEye, "Show eye image", "");
		m_pMenu->AppendSeparator();
		// step
		CreateSubmenuMapSteps();
		// colors
		CreateSubmenuMapColors();
		break;

	case 2:
		// elements
		m_pMenu->AppendItem(IDC_SHOW_MAP_EYE_ITEM, FALSE, pWndSettings->m_MapShowEye, "Show eye image", "");
		m_pMenu->AppendSeparator();
		// units
		m_pMenu->AppendItem(IDC_MAP_DIOPTERS_ITEM, pWndSettings->m_MapUnit == DIOPTERS, FALSE, "Diopters", "");
		m_pMenu->AppendItem(IDC_MAP_MILLIMETERS_ITEM, pWndSettings->m_MapUnit == MILLIMETERS, FALSE, "Millimeters", "");
		m_pMenu->AppendSeparator();
		// center
		pSubMenu = m_pMenu->AppendSubmenu("Map middle value", "");
		pSubMenu->AppendItem(IDC_MAP_CENT_USER_ITEM, !pScale->m_CentAuto, FALSE, "Fixed...", "");
		pSubMenu->AppendItem(IDC_MAP_CENT_AUTO_ITEM, pScale->m_CentAuto, FALSE, "Auto", "");
		// step
		CreateSubmenuMapSteps();
		// colors
		CreateSubmenuMapColors();
		break;
	}

}

//***************************************************************************************

void CWTSumWnd::OnEditLimbusItemClicked()
{
	CEyeImage* pImage = &m_pWFExam->m_Image;

	if (pImage->m_RGBData.GetMem() == NULL) return;

	CImageDlg* pDlg = new CImageDlg(this, pImage, TRUE, FALSE);

	if (pDlg->DoModal() == IDOK) {

		CBusyCursor Cursor;

		m_pWFExam->Process();

		// for average exam 
		CString ExamId, LoadFileName;
		GUIDToStr(m_pWFExam->m_Header.m_ExamID, ExamId);
		LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

		if (::PathFileExists(LoadFileName))
		{
			CAveExamInfo AveExamInfo;

			AveExamInfo.LoadFromFile(LoadFileName);
			for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
			{
				m_pWFExam->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
			}
		}
		// for average exam Done

		Repaint();

		::DB.SaveWFExam(m_pWFExam);
	}

	delete pDlg;
}

//***************************************************************************************

void CWTSumWnd::OnEditLensDotsItemClicked()
{
	CEyeImage* pImage = &m_pWFExam->m_Image;

	if (pImage->m_RGBData.GetMem() == NULL) return;

	CLensDlg* pDlg = new CLensDlg(this, pImage);

	if (pDlg->DoModal() == IDOK) {

		CBusyCursor Cursor;

		Repaint();

		::DB.SaveWFExam(m_pWFExam);
	}

	delete pDlg;
}

//***************************************************************************************
