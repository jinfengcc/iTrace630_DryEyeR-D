//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "SumWnd.h"
#include "Surface.h"
#include "EyeWnd.h"
#include "PSFWnd.h"
#include "MTFWnd.h"
#include "LtrWnd.h"
#include "CustomColorsDlg.h"
#include "NumDlg.h"
#include "RefSurfaceDlg.h"
#include "BusyCursor.h"
#include "Licensing.h"
#include "AppVersion.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CSumWnd, CMemWnd)

	ON_MESSAGE(WM_NOTIFY_LBUTTON_DOWN, OnChildWndLBtnDownMsg)
	ON_MESSAGE(WM_NOTIFY_RBUTTON_DOWN, OnChildWndRBtnDownMsg)
	ON_MESSAGE(WM_NOTIFY_RBUTTON_UP, OnChildWndRBtnUpMsg)

	ON_MESSAGE(WM_CHANGE_POS, OnChangePosMsg)

	ON_MESSAGE(WM_DEC_ZONE, OnDecZoneMsg)
	ON_MESSAGE(WM_INC_ZONE, OnIncZoneMsg)

	ON_MESSAGE(WM_SOLO_MASK, OnSoloMaskMsg)

										   // display type
	ON_COMMAND(IDC_TYPE_TEYE_ITEM, OnTypeTEyeItemClicked)
	ON_COMMAND(IDC_TYPE_TCEY_ITEM, OnTypeTCeyItemClicked)
	ON_COMMAND(IDC_TYPE_TRSD_ITEM, OnTypeTRsdItemClicked)
	ON_COMMAND(IDC_TYPE_TOPM_ITEM, OnTypeTOpmItemClicked)
	ON_COMMAND(IDC_TYPE_TWFM_ITEM, OnTypeTWfmItemClicked)
	ON_COMMAND(IDC_TYPE_TRFM_ITEM, OnTypeTRfmItemClicked)
	ON_COMMAND(IDC_TYPE_TPSF_ITEM, OnTypeTPsfItemClicked)
	ON_COMMAND(IDC_TYPE_TEEF_ITEM, OnTypeTEefItemClicked)
	ON_COMMAND(IDC_TYPE_TLTR_ITEM, OnTypeTLtrItemClicked)

	ON_COMMAND(IDC_TYPE_TMTF_ITEM, OnTypeTMtfItemClicked)
	ON_COMMAND(IDC_TYPE_TRMS_ITEM, OnTypeTRmsItemClicked)
	ON_COMMAND(IDC_TYPE_TAZR_ITEM, OnTypeTAzrItemClicked)
	ON_COMMAND(IDC_TYPE_CEYE_ITEM, OnTypeCEyeItemClicked)
	ON_COMMAND(IDC_TYPE_CAXM_ITEM, OnTypeCAxmItemClicked)
	ON_COMMAND(IDC_TYPE_CTNM_ITEM, OnTypeCTnmItemClicked)
	ON_COMMAND(IDC_TYPE_CRFM_ITEM, OnTypeCRfmItemClicked)
	ON_COMMAND(IDC_TYPE_CELM_ITEM, OnTypeCElmItemClicked)
	ON_COMMAND(IDC_TYPE_CWFM_ITEM, OnTypeCWfmItemClicked)
	ON_COMMAND(IDC_TYPE_CPSF_ITEM, OnTypeCPsfItemClicked)
	ON_COMMAND(IDC_TYPE_CEEF_ITEM, OnTypeCEefItemClicked)
	ON_COMMAND(IDC_TYPE_CMTF_ITEM, OnTypeCMtfItemClicked)
	ON_COMMAND(IDC_TYPE_CRMS_ITEM, OnTypeCRmsItemClicked)
	ON_COMMAND(IDC_TYPE_CLTR_ITEM, OnTypeCLtrItemClicked)
	ON_COMMAND(IDC_TYPE_CSKM_ITEM, OnTypeCSkmItemClicked)
	ON_COMMAND(IDC_TYPE_IWFM_ITEM, OnTypeIWfmItemClicked)
	ON_COMMAND(IDC_TYPE_IRFM_ITEM, OnTypeIRfmItemClicked)
	ON_COMMAND(IDC_TYPE_IPSF_ITEM, OnTypeIPsfItemClicked)
	ON_COMMAND(IDC_TYPE_IEEF_ITEM, OnTypeIEefItemClicked)
	ON_COMMAND(IDC_TYPE_IMTF_ITEM, OnTypeIMtfItemClicked)
	ON_COMMAND(IDC_TYPE_IRMS_ITEM, OnTypeIRmsItemClicked)
	ON_COMMAND(IDC_TYPE_ILTR_ITEM, OnTypeILtrItemClicked)

	ON_COMMAND(IDC_TYPE_DOFF_ITEM, OnTypeDofItemClicked)//531

	ON_COMMAND(IDC_TYPE_OSHER_ALIGNMENT, OneOsherAlignmentClicked)//[520]
																  // display method
	ON_COMMAND(IDC_METHOD_3D_ITEM, OnMethod3DItemClicked)
	// display size
	ON_COMMAND(IDC_SIZE_LARGE_ITEM, OnSizeLargeItemClicked)
	// zone
	ON_COMMAND(IDC_ZONE_02MM_ITEM, OnZone02MMItemClicked)
	ON_COMMAND(IDC_ZONE_03MM_ITEM, OnZone03MMItemClicked)
	ON_COMMAND(IDC_ZONE_04MM_ITEM, OnZone04MMItemClicked)
	ON_COMMAND(IDC_ZONE_05MM_ITEM, OnZone05MMItemClicked)
	ON_COMMAND(IDC_ZONE_06MM_ITEM, OnZone06MMItemClicked)
	ON_COMMAND(IDC_ZONE_07MM_ITEM, OnZone07MMItemClicked)
	ON_COMMAND(IDC_ZONE_08MM_ITEM, OnZone08MMItemClicked)
	ON_COMMAND(IDC_ZONE_09MM_ITEM, OnZone09MMItemClicked)
	ON_COMMAND(IDC_ZONE_10MM_ITEM, OnZone10MMItemClicked)
	ON_COMMAND(IDC_ZONE_PRESBIA_ITEM, OnZonePresbiaMMItemClicked)//530 For Presbia
	ON_COMMAND(IDC_ZONE_OTHER_ITEM, OnZoneOtherItemClicked)
	// aberrations mask
	ON_COMMAND(IDC_MASK_PISTON_ITEM, OnMaskPistonItemClicked)
	ON_COMMAND(IDC_MASK_TILT_ITEM, OnMaskTiltItemClicked)
	ON_COMMAND(IDC_MASK_TOTAL_ITEM, OnMaskTotalItemClicked)
	ON_COMMAND(IDC_MASK_LO_TOTAL_ITEM, OnMaskLOTotalItemClicked)
	ON_COMMAND(IDC_MASK_DEFOCUS_ITEM, OnMaskDefocusItemClicked)
	ON_COMMAND(IDC_MASK_ASTIGMATISM_ITEM, OnMaskAstigmatismItemClicked)
	ON_COMMAND(IDC_MASK_HO_TOTAL_ITEM, OnMaskHOTotalItemClicked)
	ON_COMMAND(IDC_MASK_COMA_ITEM, OnMaskComaItemClicked)
	ON_COMMAND(IDC_MASK_TREFOIL_ITEM, OnMaskTrefoilItemClicked)
	ON_COMMAND(IDC_MASK_SPHERICAL_ITEM, OnMaskSphericalItemClicked)
	ON_COMMAND(IDC_MASK_SEC_ASTIGMATISM_ITEM, OnMaskSecAstigmatismItemClicked)
	ON_COMMAND(IDC_MASK_OTHER_ITEM, OnMaskOtherItemClicked)
	ON_COMMAND(IDC_MASK_TOTAL_NO_DEFOCUS_ITEM, OnMaskTotalNoDefocusItemClicked)
	// map unit
	ON_COMMAND(IDC_MAP_DIOPTERS_ITEM, OnMapDioptersItemClicked)
	ON_COMMAND(IDC_MAP_MILLIMETERS_ITEM, OnMapMillimetersItemClicked)
	// display elements
	ON_COMMAND(IDC_SHOW_MAP_EYE_ITEM, OnShowMapEyeItemClicked)
	ON_COMMAND(IDC_SHOW_MAP_SURFACE_ITEM, OnShowMapSurfaceItemClicked)
	ON_COMMAND(IDC_SHOW_MAP_MESH_ITEM, OnShowMapMeshItemClicked)
	ON_COMMAND(IDC_SHOW_MAP_PUPIL_ITEM, OnShowMapPupilItemClicked)
	ON_COMMAND(IDC_SHOW_MAP_VERTEX_ITEM, OnShowMapVertexItemClicked)
	ON_COMMAND(IDC_SHOW_MAP_LENS_ITEM, OnShowMapLensItemClicked)
	ON_COMMAND(IDC_SHOW_MAP_POINTS_ITEM, OnShowMapPointsItemClicked)
	ON_COMMAND(IDC_SHOW_MAP_RINGS_ITEM, OnShowMapRingsItemClicked)
	ON_COMMAND(IDC_SHOW_MAP_LIMBUS_ITEM, OnShowMapLimbusItemClicked)
	ON_COMMAND(IDC_SHOW_MAP_MAP_ITEM, OnShowMapMapItemClicked)
	ON_COMMAND(IDC_SHOW_MAP_NUMBERS_ITEM, OnShowMapNumbersItemClicked)
	ON_COMMAND(IDC_SHOW_MAP_KERATOMETRY_ITEM, OnShowMapKeratometryItemClicked)
	ON_COMMAND(IDC_SHOW_MAP_ASTIGM_AXES_ITEM, OnShowMapAstigmatismAxesItemClicked)
	ON_COMMAND(IDC_SHOW_MAP_OSHER_RING_ITEM, OnShowMapOsherRingItemClicked)
	//ON_COMMAND(IDC_SHOW_MAP_ZALDIVAR_ITEM    , OnShowMapZaldivarCaliperItemClicked)
	//[cjf***05022012*004*]
	ON_COMMAND(IDC_SHOW_MAP_INCISION_ITEM, OnShowMapIncisionItemClicked)
	ON_COMMAND(IDC_SHOW_MAP_PREOP_CORNEAL_ITEM, OnShowMapPreopCornealItemClicked)
	ON_COMMAND(IDC_SHOW_MAP_PREOP_INTERNAL_ITEM, OnShowMapPreopInternalItemClicked)
	ON_COMMAND(IDC_SHOW_MAP_PREOP_ENTIRE_ITEM, OnShowMapPreopEntireEyeItemClicked)
	ON_COMMAND(IDC_SHOW_MAP_POSTOP_CORNEAL_ITEM, OnShowMapPostopCornealItemClicked)
	ON_COMMAND(IDC_SHOW_MAP_POSTOP_INTERNAL_ITEM, OnShowMapPostopInternalItemClicked)
	ON_COMMAND(IDC_SHOW_MAP_POSTOP_ENTIRE_ITEM, OnShowMapPostopEntireEyeItemClicked)
	ON_COMMAND(IDC_SHOW_MAP_ICL_STEEP_ITEM, OnShowMapICLinducedSteepItemClicked)
	ON_COMMAND(IDC_SHOW_MAP_ICL_FLAT_ITEM, OnShowMapICLinducedFlatItemClicked)
	//[cjf***05022012*004*]
	ON_COMMAND(IDC_SHOW_PSF_EEF_ITEM, OnShowPsfEefItemClicked)

	// translucent map
	ON_COMMAND(IDC_MAP_TRANSLUCENT_ITEM, OnMapTranslucentItemClicked)
	// psf eef
	ON_COMMAND(IDC_EEF_TYPE1_ITEM, OnEefType1ItemClicked)
	ON_COMMAND(IDC_EEF_TYPE2_ITEM, OnEefType2ItemClicked)
	// cent
	ON_COMMAND(IDC_MAP_CENT_AUTO_ITEM, OnMapCentAutoItemClicked)
	ON_COMMAND(IDC_MAP_CENT_USER_ITEM, OnMapCentUserItemClicked)
	// step
	ON_COMMAND(IDC_MAP_STEP_0001_ITEM, OnMapStep0001ItemClicked)
	ON_COMMAND(IDC_MAP_STEP_0002_ITEM, OnMapStep0002ItemClicked)
	ON_COMMAND(IDC_MAP_STEP_0005_ITEM, OnMapStep0005ItemClicked)
	ON_COMMAND(IDC_MAP_STEP_0010_ITEM, OnMapStep0010ItemClicked)
	ON_COMMAND(IDC_MAP_STEP_0025_ITEM, OnMapStep0025ItemClicked)
	ON_COMMAND(IDC_MAP_STEP_0050_ITEM, OnMapStep0050ItemClicked)
	ON_COMMAND(IDC_MAP_STEP_0075_ITEM, OnMapStep0075ItemClicked)
	ON_COMMAND(IDC_MAP_STEP_0100_ITEM, OnMapStep0100ItemClicked)
	ON_COMMAND(IDC_MAP_STEP_0125_ITEM, OnMapStep0125ItemClicked)
	ON_COMMAND(IDC_MAP_STEP_0150_ITEM, OnMapStep0150ItemClicked)
	ON_COMMAND(IDC_MAP_STEP_0175_ITEM, OnMapStep0175ItemClicked)
	ON_COMMAND(IDC_MAP_STEP_0200_ITEM, OnMapStep0200ItemClicked)
	ON_COMMAND(IDC_MAP_STEP_USER_ITEM, OnMapStepUserItemClicked)
	ON_COMMAND(IDC_MAP_STEP_AUTO_ITEM, OnMapStepAutoItemClicked)
	// colors
	ON_COMMAND(IDC_MAP_COLORS1_ITEM, OnMapColors1ItemClicked)
	ON_COMMAND(IDC_MAP_COLORS2_ITEM, OnMapColors2ItemClicked)
	ON_COMMAND(IDC_MAP_COLORS_USER_ITEM, OnMapColorsUserItemClicked)
	// ltr
	ON_COMMAND(IDC_LTR_E_ITEM, OnLtrEItemClicked)
	ON_COMMAND(IDC_LTR_C_ITEM, OnLtrCItemClicked)
	ON_COMMAND(IDC_LTR_LINE_020_ITEM, OnLtrLine020ItemClicked)
	ON_COMMAND(IDC_LTR_LINE_025_ITEM, OnLtrLine025ItemClicked)
	ON_COMMAND(IDC_LTR_LINE_030_ITEM, OnLtrLine030ItemClicked)
	ON_COMMAND(IDC_LTR_LINE_040_ITEM, OnLtrLine040ItemClicked)
	ON_COMMAND(IDC_LTR_LINE_050_ITEM, OnLtrLine050ItemClicked)
	ON_COMMAND(IDC_LTR_LINE_070_ITEM, OnLtrLine070ItemClicked)
	ON_COMMAND(IDC_LTR_LINE_100_ITEM, OnLtrLine100ItemClicked)
	ON_COMMAND(IDC_LTR_LINE_200_ITEM, OnLtrLine200ItemClicked)
	ON_COMMAND(IDC_LTR_ORIENTATION_000_ITEM, OnLtrOrientation000ItemClicked)
	ON_COMMAND(IDC_LTR_ORIENTATION_090_ITEM, OnLtrOrientation090ItemClicked)
	ON_COMMAND(IDC_LTR_ORIENTATION_180_ITEM, OnLtrOrientation180ItemClicked)
	ON_COMMAND(IDC_LTR_ORIENTATION_270_ITEM, OnLtrOrientation270ItemClicked)

	//530
	ON_COMMAND(IDC_TYPE_NLTR_1ITEM, OnNLtrLine1ItemClicked)
	ON_COMMAND(IDC_TYPE_NLTR_2ITEM, OnNLtrLine2ItemClicked)
	ON_COMMAND(IDC_TYPE_NLTR_3ITEM, OnNLtrLine3ItemClicked)
	ON_COMMAND(IDC_TYPE_NLTR_4ITEM, OnNLtrLine4ItemClicked)
	ON_COMMAND(IDC_TYPE_NLTR_5ITEM, OnNLtrLine5ItemClicked)
	//530

	// mtf type
	ON_COMMAND(IDC_MTF_AVG_ITEM, OnMtfAvgItemClicked)
	// rms type
	ON_COMMAND(IDC_RMS_CMB_ITEM, OnRmsCmbItemClicked)
	// reference surface
	ON_COMMAND(IDC_REF_BEST_SPHERE_ITEM, OnElmRefBestSphereItemClicked)
	ON_COMMAND(IDC_REF_BEST_CONICOID_ITEM, OnElmRefBestConicoidItemClicked)
	ON_COMMAND(IDC_REF_USER_CONICOID_ITEM, OnElmRefUserConicoidItemClicked)
	// save
	ON_COMMAND(IDC_SAVE_TXT_ITEM, OnSaveTxtItemClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CSumWnd::CSumWnd(CWnd* pParentWnd, RECT& Rect, CPatient* pPatient, CWndSettings* pWndSettings)
{
	CreateWnd(Rect, pParentWnd, SW_HIDE);

	m_g = 0.01 * m_h;

	m_pPatient = pPatient;
	m_pWndSettings = pWndSettings;

	m_d = 0;

	m_pMenu = NULL;

	m_VFASumWnd = FALSE;//520
	m_OUSumWnd = FALSE;//520
	m_WFSumWnd = FALSE;
	m_WFIolWnd = FALSE;//530
	m_WFCTDYSWnd = FALSE;//600
	m_WFVrfTestSumWnd = FALSE;//
	m_WFCTTORREFSumWnd = FALSE;//

	for (int i = 0; i < 6; i++) m_InsideZone[i] = FALSE;//530

	m_SW = ::GetSystemMetrics(SM_CXSCREEN);//530

	for (int d = 0; d < NUM_CHILDWND_MAX; d++) m_pDispWnd[d] = NULL;
}

//***************************************************************************************

CSumWnd::~CSumWnd()
{

	SAFE_DELETE(m_pMenu);

	for (int d = 0; d < NUM_CHILDWND_MAX; d++) SAFE_DELETE(m_pDispWnd[d]);
}

//***************************************************************************************

void CSumWnd::RepaintMemDC()
{
	ClearMemDC();

	RECT r1, r2;
	GetWindowRect(&r1);
	for (int i = 0; i < NUM_CHILDWND_MAX; i++) {
		if (m_pDispWnd[i]) {
			m_pDispWnd[i]->RepaintMemDC();
			m_pDispWnd[i]->GetWindowRect(&r2);
			m_MemDC.BitBlt(r2.left - r1.left, r2.top - r1.top, r2.right - r2.left, r2.bottom - r2.top, &m_pDispWnd[i]->m_MemDC, 0, 0, SRCCOPY);
		}
	}

	COLORREF white = m_Printing ? BLACK : WHITE;

	real_t t = 0.0;
	real_t h = 3.0 * m_g;

	RECT Rect;
	CString s;

	CMFont Font1(intRound(1.1 * h), 400, "Arial");
	CMFont Font2(intRound(1.5 * m_g), 400, "Arial");

	// patient name
	if (!m_WFVrfTestSumWnd)//
	{
		::SetRect(&Rect, 0, intRound(t), intRound(0.990 * m_w), intRound(t + h + 1));
		m_MemDC.WriteText(m_pPatient->FullName(), Rect, Font1, white, 2);
	}

	// patient custom id
	if (!m_WFVrfTestSumWnd)//
	{
		t += h;
		::SetRect(&Rect, 0, intRound(t), intRound(0.990 * m_w), intRound(t + h + 1));
		if (m_pPatient->m_CustomID != "") {
			//530
			if (m_OUSumWnd)
			{
				CString s;
				CGroup* pGroup = ::DB.GetGroup(m_pPatient->m_GroupID);
				if (pGroup) {
					s = "ID:  " + m_pPatient->m_CustomID;
					s += "     Group:  " + pGroup->m_Name;
					m_MemDC.WriteText(s, Rect, Font1, white, 2);
				}
				else m_MemDC.WriteText("ID:  " + m_pPatient->m_CustomID, Rect, Font1, white, 2);
			}
			//530
			else m_MemDC.WriteText("ID:  " + m_pPatient->m_CustomID, Rect, Font1, white, 2);
		}
	}

	// caption
	if (m_isCombo) t -= h;

	::SetRect(&Rect, intRound(0.010 * m_w), intRound(t), m_w, intRound(t + h + 1));
	m_MemDC.WriteText(GetName(), Rect, Font1, white, 0);

	// if (::Licensing.m_LicenseType != LT_HOYA) t += h;
	if (m_isCombo) t += h;


	// group name
	if (!m_OUSumWnd && !m_WFVrfTestSumWnd)//530 add 'if(!m_OUSumWnd) '  add '&& !m_WFVrfTestSumWnd'
	{
		t += h;
		::SetRect(&Rect, 0, intRound(t), intRound(0.990 * m_w), intRound(t + h + 1));
		CGroup* pGroup = ::DB.GetGroup(m_pPatient->m_GroupID);
		if (pGroup) {
			m_MemDC.WriteText("Group:  " + pGroup->m_Name, Rect, Font1, white, 2);
		}
	}

	// Tracey logo
	if (m_isHoya) {
		CTraceyLogo TraceyLogo(m_Printing ? 1 : 0);
		m_MemDC.DrawImage(TraceyLogo, 3, 3, 0, 0, TRUE);
	}

	// product logo
	CBasicImage *pLogo;
	if (m_isHoya) {
		pLogo = new CProductLogo(m_Printing ? 1 : 0);
	}
	else {
		pLogo = new CiTraceLogo(m_Printing ? 1 : 0);
	}
	m_MemDC.DrawImage(*pLogo, intRound(0.500 * m_w), intRound(0.050 * m_h), 1, 1, TRUE);
	delete pLogo;

	// software version
	::SetRect(&Rect, intRound(0.005 * m_w), intRound(m_h - m_g), m_w, m_h);
	s = "Version " + ::FullSoftwareVersion() + "." + ::Licensing.LicenseName();
	if (m_isPresbia) s = s + "resbia";//530
									  //CString s1("Version 6.0.0 Beta 01-30-2015" + m_LicenseName);//531//531
	m_MemDC.WriteText(s, Rect, Font2, white, 0);
}

//***************************************************************************************
// Draw the glass icon for print out
void CSumWnd::DrawGlassIcon(int makeType, int SizeSmall, int i)
{
	//Get the pos of Glass Icon
	RECT r2;
	m_pDispWnd[i]->GetWindowRect(&r2);

	int w = r2.right - r2.left;
	int IconWidth = intRound(0.058 * w);

	if (IconWidth < 25) IconWidth = 25;

	int r = r2.right - 2 * IconWidth - 2;
	int b = r2.bottom - 2;
	//


	int W = ::GetSystemMetrics(SM_CXSCREEN);

	CBasicImage GlassIcon;
	int ResourceID = 0;

	if (makeType == MASK_TOTAL)
	{
		ResourceID = (SizeSmall ?
			(W >= 1920 ? IDR_SOLO_NOCOR3 : W >= 1600 ? IDR_SOLO_NOCOR2 : IDR_SOLO_NOCOR1) :
			(W >= 1920 ? IDR_SOLO_NOCOR6 : W >= 1600 ? IDR_SOLO_NOCOR5 : IDR_SOLO_NOCOR4));
	}
	else if (makeType == MASK_HO_TOTAL)
	{
		ResourceID = (SizeSmall ?
			(W >= 1920 ? IDR_SOLO_COR3 : W >= 1600 ? IDR_SOLO_COR2 : IDR_SOLO_COR1) :
			(W >= 1920 ? IDR_SOLO_COR6 : W >= 1600 ? IDR_SOLO_COR5 : IDR_SOLO_COR4));
	}
	else if (makeType == MASK_TOTAL_NO_DEFOCUS)
	{
		ResourceID = (SizeSmall ?
			(W >= 1920 ? IDR_SOLO_CORSP3 : W >= 1600 ? IDR_SOLO_CORSP2 : IDR_SOLO_CORSP1) :
			(W >= 1920 ? IDR_SOLO_CORSP6 : W >= 1600 ? IDR_SOLO_CORSP5 : IDR_SOLO_CORSP4));
	}

	GlassIcon.LoadFromRes(ResourceID);

	m_MemDC.DrawImage(GlassIcon, r, intRound(b), IconWidth, IconWidth, TRUE);
}

//***************************************************************************************

LRESULT CSumWnd::OnDecZoneMsg(WPARAM wParam, LPARAM lParam)
{
	for (int d = 0; d < NUM_CHILDWND_MAX; d++)
	{
		if (m_pDispWnd[d])
		{
			if ((WPARAM)m_pDispWnd[d] == wParam)
			{
				m_d = d;
				break;
			}
		}
	}

	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	m_pDispWnd[m_d]->m_IncZoneButton.SetEnabled(TRUE);
	m_pDispWnd[m_d]->m_IncZoneButton.Repaint();

	if (m_OUSumWnd)
	{
		if (m_d == 3) m_d = 4;
		if (m_d == 0)
		{
			m_d = 1;
			pWndSettings = m_pWndSettings + m_d;
		}
	}

	// vfa summary
	if (m_VFASumWnd || m_OUSumWnd)
	{
		if (m_d < 3)
		{
			::TempSettings.Com_r_max_um = ::TempSettings.Com_r_max_um1;
		}
		else
		{
			pWndSettings = m_pWndSettings + m_d;
			::TempSettings.Com_r_max_um = ::TempSettings.Com_r_max_um2;
		}
	}

	if (m_WFCTTORREFSumWnd)
	{
		pWndSettings = &m_ThisWndSetting;
	}

	if (pWndSettings->m_ZoneRadiusUm <= 500)
	{
		return 0;
	}

	if (m_VFASumWnd || m_OUSumWnd)
	{
		if (!m_Togther)
		{
			int CurZone = pWndSettings->m_ZoneRadiusUm;

			if (pWndSettings->m_ZoneRadiusUm > ::TempSettings.Com_r_max_um)
				pWndSettings->m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um);
			if (pWndSettings->m_ZoneRadiusUm <= 1000)
				pWndSettings->m_ZoneRadiusUm = 500;
			else
			{
				int rest = pWndSettings->m_ZoneRadiusUm % 500;
				if (rest != 0) pWndSettings->m_ZoneRadiusUm -= rest;
				else pWndSettings->m_ZoneRadiusUm -= 500;
			}

			if (m_d < 3 && ::TempSettings.Com_r_max_um1 > ::TempSettings.Com_r_max_um2)
			{
				if (pWndSettings->m_ZoneRadiusUm < ::TempSettings.Com_r_max_um2 &&
					CurZone > ::TempSettings.Com_r_max_um2)
					pWndSettings->m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um2);
			}

			else if (m_d >= 3 && ::TempSettings.Com_r_max_um2 > ::TempSettings.Com_r_max_um1)
			{
				if (pWndSettings->m_ZoneRadiusUm < ::TempSettings.Com_r_max_um1 &&
					CurZone > ::TempSettings.Com_r_max_um1)
					pWndSettings->m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um1);
			}
		}
		else
		{
			if (pWndSettings->m_ZoneRadiusUm > ::TempSettings.Com_r_max_um)
				pWndSettings->m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um);

			if (pWndSettings->m_ZoneRadiusUm <= 1000)
				pWndSettings->m_ZoneRadiusUm = 500;
			else
			{
				int rest = pWndSettings->m_ZoneRadiusUm % 500;
				if (rest != 0) pWndSettings->m_ZoneRadiusUm -= rest;
				else pWndSettings->m_ZoneRadiusUm -= 500;
			}
		}
	}
	else if (m_WFCTDYSWnd)
	{
		if (pWndSettings->m_ZoneRadiusUm > ::TempSettings.Com_r_max_um)
			pWndSettings->m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um);


		int rest = pWndSettings->m_ZoneRadiusUm % 500;

		if (rest != 0) pWndSettings->m_ZoneRadiusUm -= rest;
		else           pWndSettings->m_ZoneRadiusUm -= 500;

		if (pWndSettings->m_ZoneRadiusUm <= 1250)
			pWndSettings->m_ZoneRadiusUm = 1250;
	}
	else
	{
		if (pWndSettings->m_ZoneRadiusUm > ::TempSettings.Com_r_max_um)
			pWndSettings->m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um);

		if (pWndSettings->m_ZoneRadiusUm <= 1000)
			pWndSettings->m_ZoneRadiusUm = 500;
		else
		{
			int rest = pWndSettings->m_ZoneRadiusUm % 500;
			if (rest != 0) pWndSettings->m_ZoneRadiusUm -= rest;
			else pWndSettings->m_ZoneRadiusUm -= 500;
		}
	}

	CBusyCursor Cursor;

	if (m_WFSumWnd)
	{
		for (int i = 0; i < 4; i++)
		{
			if (m_InsideZone[m_d] != TRUE) // if it is not Presbia 2.4mm..
				m_pWndSettings[i].m_ZoneRadiusUm = pWndSettings->m_ZoneRadiusUm;
		}

		if (!m_pWndSettings[m_d].m_SizeSmall)
		{

			CreateChildWnd();
		}
		else
		{
			for (m_d = 0; m_d < 4; m_d++) {
				if (m_InsideZone[m_d] != TRUE) // if it is not Presbia 2.4mm..
					CreateChildWnd();
			}
		}
	}
	else if (m_WFCTDYSWnd)
	{
		for (int i = 0; i < 4; i++)
			m_pWndSettings[i].m_ZoneRadiusUm = pWndSettings->m_ZoneRadiusUm;

		CreateChildWnd();
	}
	else CreateChildWnd();

	Repaint();

	return 0;
}

//***************************************************************************************

LRESULT CSumWnd::OnSoloMaskMsg(WPARAM wParam, LPARAM lParam)
{
	CBusyCursor Cursor;

	for (int d = 0; d < NUM_CHILDWND_MAX; d++)
	{
		if (m_pDispWnd[d])
		{
			if ((WPARAM)m_pDispWnd[d] == wParam)
			{
				m_d = d;
				break;
			}
		}
	}

	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)  pWndSettings = &m_pWndSettings[m_d];

	if (m_VFASumWnd && m_d != 0)
	{
		m_d = 0;
		pWndSettings = &m_pWndSettings[0];
	}

	if (m_WFCTTORREFSumWnd)
	{
		int maskType = (m_ThisWndSetting.m_Mask.GetType() == MASK_HO_TOTAL ? MASK_TOTAL : m_ThisWndSetting.m_Mask.GetType() == MASK_TOTAL ? MASK_TOTAL_NO_DEFOCUS : MASK_HO_TOTAL);

		m_ThisWndSetting.m_Mask.SetType(maskType);
	}
	else
	{
		int maskType = (pWndSettings->m_Mask.GetType() == MASK_HO_TOTAL ? MASK_TOTAL : pWndSettings->m_Mask.GetType() == MASK_TOTAL ? MASK_TOTAL_NO_DEFOCUS : MASK_HO_TOTAL);

		pWndSettings->m_Mask.SetType(maskType);
	}

	CreateChildWnd();

	Repaint();

	return 0;
}

//***************************************************************************************

LRESULT CSumWnd::OnIncZoneMsg(WPARAM wParam, LPARAM lParam)
{
	for (int d = 0; d < NUM_CHILDWND_MAX; d++)
	{
		if (m_pDispWnd[d])
		{
			if ((WPARAM)m_pDispWnd[d] == wParam)
			{
				m_d = d;
				break;
			}
		}
	}

	if (m_OUSumWnd)
	{
		if (m_d == 3) m_d = 4;
		if (m_d == 0) m_d = 1;
	}

	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)  pWndSettings = &m_pWndSettings[m_d];

	m_pDispWnd[m_d]->m_DecZoneButton.SetEnabled(TRUE);
	m_pDispWnd[m_d]->m_DecZoneButton.Repaint();

	if (m_WFCTTORREFSumWnd)
	{
		pWndSettings = &m_ThisWndSetting;
	}

	// for vfa summary
	if (m_VFASumWnd || m_OUSumWnd)
	{
		if (m_d < 3)
		{
			::TempSettings.Com_r_max_um = ::TempSettings.Com_r_max_um1;
		}
		else
		{
			pWndSettings = m_pWndSettings + m_d;
			::TempSettings.Com_r_max_um = ::TempSettings.Com_r_max_um2;
		}

		if (!m_Togther)
		{
			if (pWndSettings->m_ZoneRadiusUm >= ::TempSettings.Com_r_max_um)
			{
				return 0;
			}

			int CurZone = pWndSettings->m_ZoneRadiusUm;
			int rest = pWndSettings->m_ZoneRadiusUm % 500;

			if (rest != 0)
			{
				pWndSettings->m_ZoneRadiusUm = pWndSettings->m_ZoneRadiusUm - rest + 500;
			}
			else
			{
				pWndSettings->m_ZoneRadiusUm += 500;
			}

			if (m_d < 3 && ::TempSettings.Com_r_max_um1 > ::TempSettings.Com_r_max_um2)
			{
				if (CurZone < ::TempSettings.Com_r_max_um2 &&
					pWndSettings->m_ZoneRadiusUm > ::TempSettings.Com_r_max_um2)
				{
					pWndSettings->m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um2);
				}
			}
			else if (m_d >= 3 && ::TempSettings.Com_r_max_um2 > ::TempSettings.Com_r_max_um1)
			{
				if (CurZone < ::TempSettings.Com_r_max_um1 &&
					pWndSettings->m_ZoneRadiusUm > ::TempSettings.Com_r_max_um1)
				{
					pWndSettings->m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um1);
				}
			}

			if (pWndSettings->m_ZoneRadiusUm > ::TempSettings.Com_r_max_um)
				pWndSettings->m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um);
		}
		else
		{
			if (m_TogtherEdge
				&&
				((m_d < 3 && ::TempSettings.Com_r_max_um1 > ::TempSettings.Com_r_max_um2)
					|| (m_d >= 3 && ::TempSettings.Com_r_max_um2 > ::TempSettings.Com_r_max_um1)

					)
				)

			{
				if (m_d < 3)
				{
					::TempSettings.Com_r_max_um = ::TempSettings.Com_r_max_um1;
				}
				else
				{
					pWndSettings = m_pWndSettings + m_d;
					::TempSettings.Com_r_max_um = ::TempSettings.Com_r_max_um2;
				}

				if (pWndSettings->m_ZoneRadiusUm >= ::TempSettings.Com_r_max_um)
				{
					return 0;
				}


				int CurZone = pWndSettings->m_ZoneRadiusUm;
				int rest = pWndSettings->m_ZoneRadiusUm % 500;
				if (rest != 0) pWndSettings->m_ZoneRadiusUm = pWndSettings->m_ZoneRadiusUm - rest + 500;
				else pWndSettings->m_ZoneRadiusUm += 500;

				if (m_d < 3 && ::TempSettings.Com_r_max_um1 > ::TempSettings.Com_r_max_um2)
				{
					if (CurZone < ::TempSettings.Com_r_max_um2 &&
						pWndSettings->m_ZoneRadiusUm > ::TempSettings.Com_r_max_um2)
					{
						pWndSettings->m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um2);
					}
				}
				else if (m_d >= 3 && ::TempSettings.Com_r_max_um2 > ::TempSettings.Com_r_max_um1)
				{
					if (CurZone < ::TempSettings.Com_r_max_um1 &&
						pWndSettings->m_ZoneRadiusUm > ::TempSettings.Com_r_max_um1)
					{
						pWndSettings->m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um1);
					}
				}
			}
			else
			{
				if (pWndSettings->m_ZoneRadiusUm >= ::TempSettings.Com_r_max_um)
				{
					return 0;
				}

				int CurZone = pWndSettings->m_ZoneRadiusUm;

				pWndSettings->m_ZoneRadiusUm += 500;

				if (m_d < 3 && ::TempSettings.Com_r_max_um1 > ::TempSettings.Com_r_max_um2)
				{
					if (CurZone < ::TempSettings.Com_r_max_um2 &&
						pWndSettings->m_ZoneRadiusUm > ::TempSettings.Com_r_max_um2)
					{
						pWndSettings->m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um2);
					}
				}
				else if (m_d >= 3 && ::TempSettings.Com_r_max_um2 > ::TempSettings.Com_r_max_um1)
				{
					if (CurZone < ::TempSettings.Com_r_max_um1 &&
						pWndSettings->m_ZoneRadiusUm > ::TempSettings.Com_r_max_um1)
					{
						pWndSettings->m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um1);
					}
				}

				if (pWndSettings->m_ZoneRadiusUm >= ::TempSettings.Com_r_max_um)
					pWndSettings->m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um);
			}
		}
	}
	else if (m_WFCTDYSWnd)
	{
		if (pWndSettings->m_ZoneRadiusUm >= ::TempSettings.Com_r_max_um)
		{
			return 0;
		}

		if (pWndSettings->m_ZoneRadiusUm == 1250)
		{
			pWndSettings->m_ZoneRadiusUm = 1500;
		}
		else pWndSettings->m_ZoneRadiusUm += 500;

		if (pWndSettings->m_ZoneRadiusUm >= ::TempSettings.Com_r_max_um)
			pWndSettings->m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um);
	}
	else
	{
		if (pWndSettings->m_ZoneRadiusUm >= ::TempSettings.Com_r_max_um)
		{
			return 0;
		}

		pWndSettings->m_ZoneRadiusUm += 500;

		if (pWndSettings->m_ZoneRadiusUm >= ::TempSettings.Com_r_max_um)
			pWndSettings->m_ZoneRadiusUm = intRound(::TempSettings.Com_r_max_um);
	}

	CBusyCursor Cursor;

	if (m_WFSumWnd)
	{
		for (int i = 0; i < 4; i++)
		{
			if (m_InsideZone[m_d] != TRUE) //if it is not Presbia 2.4mm..
				m_pWndSettings[i].m_ZoneRadiusUm = pWndSettings->m_ZoneRadiusUm;
		}

		if (!m_pWndSettings[m_d].m_SizeSmall)
		{
			CreateChildWnd();
		}
		else
		{
			for (m_d = 0; m_d < 4; m_d++) {
				if (m_InsideZone[m_d] != TRUE) // if it is not Presbia 2.4mm..
					CreateChildWnd();
			}
		}
	}
	else if (m_WFCTDYSWnd)
	{
		for (int i = 0; i < 4; i++)
			m_pWndSettings[i].m_ZoneRadiusUm = pWndSettings->m_ZoneRadiusUm;

		CreateChildWnd();
	}
	else CreateChildWnd();

	Repaint();

	return 0;
}

//***************************************************************************************

LRESULT CSumWnd::OnChildWndLBtnDownMsg(WPARAM wParam, LPARAM lParam)
{
	for (int d = 0; d < NUM_CHILDWND_MAX; d++) {
		if (m_pDispWnd[d] && ((WPARAM)m_pDispWnd[d] == wParam)) {
			m_d = d;
		}
	}

	return 0;
}

//***************************************************************************************

LRESULT CSumWnd::OnChildWndRBtnDownMsg(WPARAM wParam, LPARAM lParam)
{
	for (int d = 0; d < NUM_CHILDWND_MAX; d++) {
		if (m_pDispWnd[d] && ((WPARAM)m_pDispWnd[d] == wParam)) {
			m_d = d;
		}
	}

	return 0;
}

//***************************************************************************************

LRESULT CSumWnd::OnChildWndRBtnUpMsg(WPARAM wParam, LPARAM lParam)
{
	SAFE_DELETE(m_pMenu);

	m_pMenu = new CXPMenu();

	CreateMenu();

	int l = (int)(((uint)lParam & 0xffff0000) >> 16);
	int t = (int)((uint)lParam & 0x0000ffff);

	m_pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, l, t, this);

	return 0;
}

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

void CSumWnd::CreateSubmenuMask()
{

	CString MaskName;

	CWndSettings* pWndSettings = GetWndSettings();

	//530
	if (m_WFIolWnd)  pWndSettings = &m_pWndSettings[m_d];
	//530

	int MaskType = pWndSettings->m_Mask.GetType();
	CXPMenu* pSubMenu = m_pMenu->AppendSubmenu("Aberrations", "");

	if (!m_isSolo)
	{
		// piston & tilt
		pSubMenu->AppendItem(IDC_MASK_PISTON_ITEM, FALSE, pWndSettings->m_Piston, "Piston", "Z0");
		pSubMenu->AppendItem(IDC_MASK_TILT_ITEM, FALSE, pWndSettings->m_Tilt, "Tilt", "Z1, Z2");
		// total
		pSubMenu->AppendSeparator();

		CZernikeMask::GetName(MASK_TOTAL, MaskName);
		pSubMenu->AppendItem(IDC_MASK_TOTAL_ITEM, MaskType == MASK_TOTAL, FALSE, MaskName, "Z3 - Z44");

		CZernikeMask::GetName(MASK_TOTAL_NO_DEFOCUS, MaskName);
		pSubMenu->AppendItem(IDC_MASK_TOTAL_NO_DEFOCUS_ITEM, MaskType == MASK_TOTAL_NO_DEFOCUS, FALSE, MaskName, "Z3, Z5, Z6 - Z44");

		// low orders
		pSubMenu->AppendSeparator();
		CZernikeMask::GetName(MASK_LO_TOTAL, MaskName);
		pSubMenu->AppendItem(IDC_MASK_LO_TOTAL_ITEM, MaskType == MASK_LO_TOTAL, FALSE, MaskName, "Z3, Z4, Z5");
		//if (IsWorkstation || IsCombo)
		{
			CZernikeMask::GetName(MASK_DEFOCUS, MaskName);
			pSubMenu->AppendItem(IDC_MASK_DEFOCUS_ITEM, MaskType == MASK_DEFOCUS, FALSE, MaskName, "Z4");

			CZernikeMask::GetName(MASK_ASTIGMATISM, MaskName);
			pSubMenu->AppendItem(IDC_MASK_ASTIGMATISM_ITEM, MaskType == MASK_ASTIGMATISM, FALSE, MaskName, "Z3, Z5");
		}
		// high orders
		pSubMenu->AppendSeparator();
		CZernikeMask::GetName(MASK_HO_TOTAL, MaskName);
		pSubMenu->AppendItem(IDC_MASK_HO_TOTAL_ITEM, MaskType == MASK_HO_TOTAL, FALSE, MaskName, "Z6 - Z44");
		//if (IsWorkstation || IsCombo)
		{
			CZernikeMask::GetName(MASK_COMA, MaskName);
			pSubMenu->AppendItem(IDC_MASK_COMA_ITEM, MaskType == MASK_COMA, FALSE, MaskName, "Z7, Z8");

			CZernikeMask::GetName(MASK_TREFOIL, MaskName);
			pSubMenu->AppendItem(IDC_MASK_TREFOIL_ITEM, MaskType == MASK_TREFOIL, FALSE, MaskName, "Z6, Z9");

			CZernikeMask::GetName(MASK_SPHERICAL, MaskName);
			pSubMenu->AppendItem(IDC_MASK_SPHERICAL_ITEM, MaskType == MASK_SPHERICAL, FALSE, MaskName, "Z12");

			CZernikeMask::GetName(MASK_4ORD_ASTIGMATISM, MaskName);
			pSubMenu->AppendItem(IDC_MASK_SEC_ASTIGMATISM_ITEM, MaskType == MASK_4ORD_ASTIGMATISM, FALSE, MaskName, "Z11, Z13");
		}
		// custom
		//if (IsWorkstation || IsCombo)
		{
			pSubMenu->AppendSeparator();
			pSubMenu->AppendItem(IDC_MASK_OTHER_ITEM, MaskType == MASK_OTHER, FALSE, "Other...", "");
		}
	}
	else
	{
		CZernikeMask::GetName(MASK_TOTAL, MaskName);
		pSubMenu->AppendItem(IDC_MASK_TOTAL_ITEM, MaskType == MASK_TOTAL, FALSE, MaskName, "Z3 - Z44");

		CZernikeMask::GetName(MASK_HO_TOTAL, MaskName);
		pSubMenu->AppendItem(IDC_MASK_HO_TOTAL_ITEM, MaskType == MASK_HO_TOTAL, FALSE, MaskName, "Z6 - Z44");
	}
}

//***************************************************************************************

void CSumWnd::CreateSubmenuZone(int Type)
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)  pWndSettings = &m_pWndSettings[m_d];

	if (m_InsideZone[m_d] == TRUE)  pWndSettings->m_ZoneRadiusUm = 1700;

	CXPMenu* pSubMenu = m_pMenu->AppendSubmenu("Zone size", "");

	if (m_WFCTDYSWnd)
	{
		pSubMenu->AppendItem(IDC_ZONE_02MM_ITEM, pWndSettings->m_ZoneRadiusUm == 1250, FALSE, " 2.5 mm", "");
	}
	else
	{
		pSubMenu->AppendItem(IDC_ZONE_02MM_ITEM, pWndSettings->m_ZoneRadiusUm == 1000, FALSE, " 2 mm", "");
	}

	pSubMenu->AppendItem(IDC_ZONE_03MM_ITEM, pWndSettings->m_ZoneRadiusUm == 1500, FALSE, " 3 mm", "");
	pSubMenu->AppendItem(IDC_ZONE_04MM_ITEM, pWndSettings->m_ZoneRadiusUm == 2000, FALSE, " 4 mm", "");
	pSubMenu->AppendItem(IDC_ZONE_05MM_ITEM, pWndSettings->m_ZoneRadiusUm == 2500, FALSE, " 5 mm", "");
	pSubMenu->AppendItem(IDC_ZONE_06MM_ITEM, pWndSettings->m_ZoneRadiusUm == 3000, FALSE, " 6 mm", "");
	pSubMenu->AppendItem(IDC_ZONE_07MM_ITEM, pWndSettings->m_ZoneRadiusUm == 3500, FALSE, " 7 mm", "");
	pSubMenu->AppendItem(IDC_ZONE_08MM_ITEM, pWndSettings->m_ZoneRadiusUm == 4000, FALSE, " 8 mm", "");
	pSubMenu->AppendItem(IDC_ZONE_09MM_ITEM, pWndSettings->m_ZoneRadiusUm == 4500, FALSE, " 9 mm", "");
	pSubMenu->AppendItem(IDC_ZONE_10MM_ITEM, pWndSettings->m_ZoneRadiusUm == 5000, FALSE, "10 mm", "");

	BOOL Other = TRUE;

	for (int i = 1000; i <= 5000; i += 500)
	{
		if (pWndSettings->m_ZoneRadiusUm == i)
		{
			Other = FALSE;
			m_InsideZone[m_d] = FALSE;
			break;
		}
	}

	if (m_isPresbia && Type == 0)
	{
		pSubMenu->AppendItem(IDC_ZONE_PRESBIA_ITEM, m_InsideZone[m_d], FALSE, "Presbia 2.4mm Zone", "");
	}
	if (m_InsideZone[m_d] == TRUE)  Other = FALSE;

	pSubMenu->AppendSeparator();
	pSubMenu->AppendItem(IDC_ZONE_OTHER_ITEM, Other, FALSE, "Other...", "");
}

//***************************************************************************************

void CSumWnd::CreateSubmenuMapSteps()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	int Type = pWndSettings->m_Type;
	CScale* pScale = pWndSettings->GetScale();
	BOOL Auto = pScale->m_StepAuto;
	real_t Step = pScale->m_Step;

	CXPMenu* pSubMenu = m_pMenu->AppendSubmenu("Map step size", "");

	if (Type == TYPE_TRFM ||
		Type == TYPE_IRFM ||
		(Type == TYPE_CAXM && pWndSettings->m_MapUnit == DIOPTERS) ||
		(Type == TYPE_CTNM && pWndSettings->m_MapUnit == DIOPTERS) ||
		(Type == TYPE_CRFM && pWndSettings->m_MapUnit == DIOPTERS))
	{
		pSubMenu->AppendItem(IDC_MAP_STEP_0025_ITEM, !Auto && Step == 0.25, FALSE, "0.25 D", "");
		pSubMenu->AppendItem(IDC_MAP_STEP_0050_ITEM, !Auto && Step == 0.50, FALSE, "0.50 D", "");
		pSubMenu->AppendItem(IDC_MAP_STEP_0075_ITEM, !Auto && Step == 0.75, FALSE, "0.75 D", "");
		pSubMenu->AppendItem(IDC_MAP_STEP_0100_ITEM, !Auto && Step == 1.00, FALSE, "1.00 D", "");
		pSubMenu->AppendItem(IDC_MAP_STEP_0125_ITEM, !Auto && Step == 1.25, FALSE, "1.25 D", "");
		pSubMenu->AppendItem(IDC_MAP_STEP_0150_ITEM, !Auto && Step == 1.50, FALSE, "1.50 D", "");
		pSubMenu->AppendItem(IDC_MAP_STEP_0175_ITEM, !Auto && Step == 1.75, FALSE, "1.75 D", "");
		pSubMenu->AppendItem(IDC_MAP_STEP_0200_ITEM, !Auto && Step == 2.00, FALSE, "2.00 D", "");
		pSubMenu->AppendSeparator();
		pSubMenu->AppendItem(IDC_MAP_STEP_AUTO_ITEM, Auto, FALSE, "Auto", "");
	}
	else
	{
		pSubMenu->AppendItem(IDC_MAP_STEP_USER_ITEM, !Auto, FALSE, "Fixed...", "");
		pSubMenu->AppendItem(IDC_MAP_STEP_AUTO_ITEM, Auto, FALSE, "Auto", "");
	}
}

//***************************************************************************************

void CSumWnd::CreateSubmenuMapColors()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	BOOL Colors1 = pWndSettings->AreDefaultColors(0);
	BOOL Colors2 = pWndSettings->AreDefaultColors(1);
	CXPMenu* pSubMenu = m_pMenu->AppendSubmenu("Map colors", "");
	pSubMenu->AppendItem(IDC_MAP_COLORS1_ITEM, Colors1, FALSE, "Colors 1", "");
	pSubMenu->AppendItem(IDC_MAP_COLORS2_ITEM, Colors2, FALSE, "Colors 2", "");

	if (!m_isSolo)
	{
		pSubMenu->AppendItem(IDC_MAP_COLORS_USER_ITEM, !(Colors1 || Colors2), FALSE, "Custom...", "");
	}
}

//***************************************************************************************

void CSumWnd::CreateSubmenuLtr()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	CXPMenu* pSubMenu = m_pMenu->AppendSubmenu("Letter", "");
	pSubMenu->AppendItem(IDC_LTR_E_ITEM, pWndSettings->m_Ltr == 0, FALSE, "E", "");
	pSubMenu->AppendItem(IDC_LTR_C_ITEM, pWndSettings->m_Ltr == 1, FALSE, "C", "");

	pSubMenu = m_pMenu->AppendSubmenu("Letter size", "");
	pSubMenu->AppendItem(IDC_LTR_LINE_200_ITEM, pWndSettings->m_LtrLine == 200, FALSE, "20 / 200", "");
	pSubMenu->AppendItem(IDC_LTR_LINE_100_ITEM, pWndSettings->m_LtrLine == 100, FALSE, "20 / 100", "");
	pSubMenu->AppendItem(IDC_LTR_LINE_070_ITEM, pWndSettings->m_LtrLine == 70, FALSE, "20 /  70", "");
	pSubMenu->AppendItem(IDC_LTR_LINE_050_ITEM, pWndSettings->m_LtrLine == 50, FALSE, "20 /  50", "");
	pSubMenu->AppendItem(IDC_LTR_LINE_040_ITEM, pWndSettings->m_LtrLine == 40, FALSE, "20 /  40", "");
	pSubMenu->AppendItem(IDC_LTR_LINE_030_ITEM, pWndSettings->m_LtrLine == 30, FALSE, "20 /  30", "");
	pSubMenu->AppendItem(IDC_LTR_LINE_025_ITEM, pWndSettings->m_LtrLine == 25, FALSE, "20 /  25", "");
	pSubMenu->AppendItem(IDC_LTR_LINE_020_ITEM, pWndSettings->m_LtrLine == 20, FALSE, "20 /  20", "");

	pSubMenu = m_pMenu->AppendSubmenu("Letter orientation", "");
	pSubMenu->AppendItem(IDC_LTR_ORIENTATION_000_ITEM, pWndSettings->m_LtrOrientation == 0, FALSE, "0°", "");
	pSubMenu->AppendItem(IDC_LTR_ORIENTATION_090_ITEM, pWndSettings->m_LtrOrientation == 90, FALSE, "90°", "");
	pSubMenu->AppendItem(IDC_LTR_ORIENTATION_180_ITEM, pWndSettings->m_LtrOrientation == 180, FALSE, "180°", "");
	pSubMenu->AppendItem(IDC_LTR_ORIENTATION_270_ITEM, pWndSettings->m_LtrOrientation == 270, FALSE, "270°", "");
}

//***************************************************************************************

LRESULT CSumWnd::OnChangePosMsg(WPARAM wParam, LPARAM lParam)
{
	for (int d = 0; d < NUM_CHILDWND_MAX; d++)
	{
		if (m_pDispWnd[d] && (WPARAM)m_pDispWnd[d] != wParam)
		{
			m_pDispWnd[d]->PostMessage(WM_CHANGE_POS, 0, lParam);
		}
	}
	return 0;
}

//***************************************************************************************

void CSumWnd::OnType(int Type)
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	if (pWndSettings->m_Type == Type)
	{
		return;
	}

	if (m_OUSumWnd)
	{
		if (pWndSettings->m_Type == TYPE_CLTR || pWndSettings->m_Type == TYPE_CPSF)
		{
			if (Type == TYPE_TLTR) pWndSettings->m_Type = TYPE_CLTR;
			if (Type == TYPE_TPSF) pWndSettings->m_Type = TYPE_CPSF;
		}
		else pWndSettings->m_Type = Type;
	}
	else
	{
		pWndSettings->m_Type = Type;
	}

	// vfa summary
	if (m_VFASumWnd)
	{
		for (int d = 3; d < 6; d++)
		{
			m_pWndSettings[d].m_Type = Type;
		}
	}

	if (::TempSettings.CTSumNum != -1)
	{
		::NewSettings.m_OSHER_ALIG_CUS[::TempSettings.CTSumNum] = 0;
	}

	CBusyCursor Cursor;

	CreateChildWnd();
	Repaint();
}

//***************************************************************************************

void CSumWnd::OnMethod3DItemClicked()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	pWndSettings->m_Method3D = !pWndSettings->m_Method3D;

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
}

//***************************************************************************************

void CSumWnd::OnZone(int ZoneRadiusUm)
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	m_InsideZone[m_d] = FALSE;//if it is not select Presbia 2.4...

	if (pWndSettings->m_ZoneRadiusUm == ZoneRadiusUm) return;

	if (ZoneRadiusUm == 0) {
		real_t ZoneDiameterMm;
		if (!::EnterNumber(ZoneDiameterMm, this, "Enter a number in millimeters")) return;
		ZoneRadiusUm = intRound(ZoneDiameterMm * 500.0);
	}
	if (ZoneRadiusUm < 1000) ZoneRadiusUm = 1000;
	else if (ZoneRadiusUm > 5000) ZoneRadiusUm = 5000;

	pWndSettings->m_ZoneRadiusUm = ZoneRadiusUm;

	// vfa summary
	if (m_VFASumWnd)//&& !m_Togther)
	{
		for (int d = 3; d < 6; d++)
		{
			m_pWndSettings[d].m_ZoneRadiusUm = ZoneRadiusUm;
		}
	}

	// ou summary
	if (m_OUSumWnd)
	{
		for (int d = 0; d < 6; d++)
		{
			m_pWndSettings[d].m_ZoneRadiusUm = ZoneRadiusUm;
		}
	}

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
}

//***************************************************************************************

void CSumWnd::OnMaskPistonItemClicked()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	pWndSettings->m_Piston = !pWndSettings->m_Piston;

	// vfa summary
	if (m_VFASumWnd)//&& !m_Togther)
	{
		for (int d = 3; d < 6; d++)
		{
			m_pWndSettings[d].m_Piston = pWndSettings->m_Piston;
		}
	}

	// ou summary
	if (m_OUSumWnd)
	{
		for (int d = 0; d < 6; d++)
		{
			m_pWndSettings[d].m_Piston = pWndSettings->m_Piston;
		}
	}

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
}

//***************************************************************************************

void CSumWnd::OnMaskTiltItemClicked()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	pWndSettings->m_Tilt = !pWndSettings->m_Tilt;

	// vfa summary
	if (m_VFASumWnd)
	{
		for (int d = 3; d < 6; d++)
		{
			m_pWndSettings[d].m_Tilt = pWndSettings->m_Tilt;
		}
	}

	// ou summary
	if (m_OUSumWnd)
	{
		for (int d = 4; d < 6; d++)
		{
			m_pWndSettings[d].m_Tilt = pWndSettings->m_Tilt;
		}
	}

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
}

//***************************************************************************************

void CSumWnd::OnMask(int MaskType)
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	if (MaskType != MASK_OTHER && pWndSettings->m_Mask.GetType() == MaskType)
	{
		return;
	}

	if (!pWndSettings->m_Mask.SetType(MaskType)) return;

	MaskType = pWndSettings->m_Mask.GetType();

	if (MaskType == MASK_TOTAL || MaskType == MASK_TOTAL_NO_DEFOCUS)
	{
		pWndSettings->m_Piston = TRUE;
		pWndSettings->m_Tilt = TRUE;
	}
	else {
		pWndSettings->m_Piston = FALSE;
		pWndSettings->m_Tilt = FALSE;
	}

	// for vfa summary
	if (m_VFASumWnd)
	{
		for (int d = 3; d < 6; d++)
		{
			m_pWndSettings[d].m_Mask = pWndSettings->m_Mask;
			m_pWndSettings[d].m_Piston = pWndSettings->m_Piston;
			m_pWndSettings[d].m_Tilt = pWndSettings->m_Tilt;
		}
	}

	// ou summary
	if (m_OUSumWnd)
	{
		for (int d = 0; d < 6; d++)
		{
			m_pWndSettings[d].m_Mask = pWndSettings->m_Mask;
			m_pWndSettings[d].m_Piston = pWndSettings->m_Piston;
			m_pWndSettings[d].m_Tilt = pWndSettings->m_Tilt;
		}
	}

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
}

//***************************************************************************************

void CSumWnd::OnMapUnit(int MapUnit)
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	if (pWndSettings->m_MapUnit == MapUnit) return;

	pWndSettings->m_MapUnit = MapUnit;

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
}

//***************************************************************************************

void CSumWnd::OnShow(BOOL* check)
{
	if (!check) return;

	*check = !*check;

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
}

//***************************************************************************************

void CSumWnd::OnEefType(int EefType)
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	if (pWndSettings->m_EEFType == EefType)
	{
		return;
	}

	pWndSettings->m_EEFType = EefType;

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
}

//***************************************************************************************

void CSumWnd::OnMapCentAutoItemClicked()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	CScale* pScale = pWndSettings->GetScale();

	pScale->m_CentAuto = TRUE;
	pScale->m_Cent = 0.0;

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
}

void CSumWnd::OnMapCentUserItemClicked()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	CScale* pScale = pWndSettings->GetScale();
	real_t inc = pWndSettings->GetIncrement();

	real_t cent;
	if (!EnterNumber(cent, this, "Enter a number")) return;

	pScale->m_CentAuto = FALSE;
	pScale->m_Cent = intRound(cent / inc) * inc;

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
}

//***************************************************************************************

void CSumWnd::OnMapStep(real_t step)
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	CScale* pScale = pWndSettings->GetScale();
	real_t inc = pWndSettings->GetIncrement();

	if (step == 0.0)
	{
		pScale->m_StepAuto = TRUE;
	}
	else
	{
		if (step == -1.0)
		{
			if (!EnterNumber(step, this, "Enter a number")) return;
			step = RealRound(step, inc);
			if (step < inc) step = inc;
			pScale->m_StepAuto = FALSE;
		}
		else
		{
			pScale->m_StepAuto = FALSE;
		}
	}

	pScale->m_Step = step;

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
}

//***************************************************************************************

void CSumWnd::OnMapColors(int ColorsIndex)
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	if (pWndSettings->AreDefaultColors(ColorsIndex))
	{
		return;
	}

	pWndSettings->SetDefaultColors(ColorsIndex);

	// vfa summary
	if (m_VFASumWnd)
	{
		for (int d = 3; d < 6; d++)
		{
			m_pWndSettings[d].SetDefaultColors(ColorsIndex);
		}
	}

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
}

//***************************************************************************************

void CSumWnd::OnMapColorsUserItemClicked()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	CScale* pScale = pWndSettings->GetScale();

	CCustomColorsDlg* pDlg = new CCustomColorsDlg(pScale, NULL);

	if (pDlg->DoModal() == IDOK)
	{
		CBusyCursor Cursor;
		CreateChildWnd();
		Repaint();
	}

	delete pDlg;
}

//***************************************************************************************

void CSumWnd::OnMapTranslucentItemClicked()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	pWndSettings->m_MapTranslucent = !pWndSettings->m_MapTranslucent;

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
}

//***************************************************************************************

void CSumWnd::OnLtr(int Ltr)
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	if (pWndSettings->m_Ltr == Ltr)
	{
		return;
	}

	pWndSettings->m_Ltr = Ltr;

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
}

//---------------------------------------------------------------------------------------

void CSumWnd::OnLtrLine(int LtrLine)
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	if (pWndSettings->m_LtrLine == LtrLine) return;

	pWndSettings->m_LtrLine = LtrLine;

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
}

//***************************************************************************************

void CSumWnd::OnLtrOrientation(int LtrOrientation)
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	if (pWndSettings->m_LtrOrientation == LtrOrientation)
	{
		return;
	}

	pWndSettings->m_LtrOrientation = LtrOrientation;

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
}

//***************************************************************************************

void CSumWnd::OnMtfAvgItemClicked()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	pWndSettings->m_MTFAverage = !pWndSettings->m_MTFAverage;

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
}

//***************************************************************************************

void CSumWnd::OnRmsCmbItemClicked()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	pWndSettings->m_RMSIndividual = !pWndSettings->m_RMSIndividual;

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
}

//***************************************************************************************

void CSumWnd::OnElmRefBestSphereItemClicked()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	if (pWndSettings->m_RefSurfaceType == REF_SURFACE_BEST_SPHERE)
	{
		return;
	}

	pWndSettings->m_RefSurfaceType = REF_SURFACE_BEST_SPHERE;

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
}

//***************************************************************************************

void CSumWnd::OnElmRefBestConicoidItemClicked()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	if (pWndSettings->m_RefSurfaceType == REF_SURFACE_BEST_CONIC)
	{
		return;
	}

	pWndSettings->m_RefSurfaceType = REF_SURFACE_BEST_CONIC;

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
}

//***************************************************************************************

void CSumWnd::OnElmRefUserConicoidItemClicked()
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	CRefSurfaceDlg* pDlg = new CRefSurfaceDlg(this, pWndSettings->m_RefSurfaceAx0Um, pWndSettings->m_RefSurfaceQ);

	if (pDlg->DoModal() == IDOK)
	{

		pWndSettings->m_RefSurfaceType = REF_SURFACE_USER_CONIC;
		pWndSettings->m_RefSurfaceAx0Um = pDlg->m_ax0_um;
		pWndSettings->m_RefSurfaceQ = pDlg->m_q;

		CBusyCursor Cursor;
		CreateChildWnd();
		Repaint();
	}

	delete pDlg;
}

//***************************************************************************************

void CSumWnd::OnSaveTxtItemClicked()
{
	CFileDialog* pDlg = new CFileDialog(FALSE, _T("txt"), _T(""), OFN_OVERWRITEPROMPT, _T("Text files (*.txt)|*.txt||"), this);
	pDlg->m_ofn.lpstrInitialDir = MAIN_DIR;

	if (pDlg->DoModal() == IDOK)
	{

		m_Surfaces[m_d].SaveTxtFile(pDlg->GetPathName());
	}

	delete pDlg;
}

//***************************************************************************************

void CSumWnd::OnMouseWheel(uint nFlags, short zDelta, CPoint Point)
{
	for (int d = 0; d < NUM_CHILDWND_MAX; d++)
	{
		if (m_pDispWnd[d])
		{
			m_pDispWnd[d]->OnMouseWheel(nFlags, zDelta, Point);
		}
	}
}

//***************************************************************************************

void CSumWnd::OneOsherAlignmentClicked()
{
	::NewSettings.m_OSHER_ALIG_CUS[::TempSettings.CTSumNum] = 1;

	GetParent()->PostMessage(WM_NOTIFY_OSHER_ALG, (WPARAM)m_d, (LPARAM)0);
}

//***************************************************************************************

// Used for solo vertical splider
int CSumWnd::GetClosePos(int inputVal, int maxVal)
{
	int restMax = maxVal % 500;

	int dif = maxVal - inputVal;

	if (dif < restMax)
	{
		int closeVal = maxVal - restMax;
		int difTocloseVal = inputVal - closeVal;

		if (difTocloseVal < dif) inputVal = closeVal;
		else inputVal = maxVal;
	}
	else
	{
		int rest = inputVal % 500;
		if (rest != 0)
		{
			if (rest < 250)
				inputVal = inputVal - rest;
			else inputVal = inputVal - rest + 500;
		}
	}

	return inputVal;
}

//*******************************************************************************************************

void CSumWnd::OnNLtr(int distanceType)
{
	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	//pWndSettings->m_Type = TYPE_NLTR;

	if (pWndSettings->m_LtrDistanceType == distanceType)
	{
		return;
	}

	pWndSettings->m_LtrDistanceType = distanceType;

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
}

//***************************************************************************************

void CSumWnd::OnZonePresbiaMMItemClicked()
{
	int OutZoneRadiusUm = 1700;

	CWndSettings* pWndSettings = GetWndSettings();

	if (m_WFIolWnd)
	{
		pWndSettings = &m_pWndSettings[m_d];
	}

	if (pWndSettings->m_ZoneRadiusUm == OutZoneRadiusUm && m_InsideZone[m_d] == TRUE)
	{
		return;
	}

	m_InsideZone[m_d] = TRUE;

	pWndSettings->m_ZoneRadiusUm = OutZoneRadiusUm;

	CBusyCursor Cursor;
	CreateChildWnd();
	Repaint();
}

//***************************************************************************************
