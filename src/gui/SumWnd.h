//***************************************************************************************

#pragma once

//***************************************************************************************

#include "DispWnd.h"
#include "Data.h"
#include "Settings.h"
#include "Btn.h"
#include "XPMenu.h"

//***************************************************************************************

#define NUM_CHILDWND_MAX 6

//***************************************************************************************

class CSumWnd : public CMemWnd
{
public:

	CSumWnd(CWnd* pParentWnd, RECT& Rect, CPatient* pPatient, CWndSettings* pWndSettings);
	virtual ~CSumWnd();

	real_t m_g;
	int m_Axes[3];
	BOOL m_VFASumWnd;// for judge it is vfa summary window
	BOOL m_OUSumWnd; // for judge it is OU summary window
	BOOL m_Togther;// for judge the size zone is togther or not for the VFA and OU GUI
	BOOL m_TogtherEdge;// for judge the size zone is togther or not for the VFA and OU GUI
	BOOL m_WFSumWnd;
	BOOL m_WFIolWnd;
	BOOL m_WFCTDYSWnd;
	BOOL m_WFVrfTestSumWnd;
	BOOL m_WFCTTORREFSumWnd;
	BOOL m_InsideZone[6];// For show Presbia 2.4mm zone size

	CPatient* m_pPatient;
	CWndSettings* m_pWndSettings;
	CWndSettings m_ThisWndSetting;// WFCTTorAliWnd and WF NEAR Vision
protected:
  // std::vector<std::unique_ptr<CDispWnd>> m_pDispWnd;
  CDispWnd *m_pDispWnd[NUM_CHILDWND_MAX];

public:
	CZernikeSurface m_Surfaces[6];
	CZernikeSurface m_DysSuface[3];// surfaces for correct dysfunctional function;
	CZernikeSurface m_ReMTF_Surfaces;// surface for correct dysfunctional function;

	RECT m_Rect[12];// change from m_Rect[10] to m_Rect[12]
	RECT m_LargeRect;

	int m_i;
	int m_d;

	int m_SW;// Screen Width

	CBtn m_WFShowFullInfoButton;//530
	CBtn m_WFShowFullInfoButton2;//530
	CBtn m_WFShowFullInfoButton3;//530
	CBtn m_CTShowFullInfoButton;//530
	CBtn m_CTShowFullInfoButton2;//530

	virtual CWndSettings* GetWndSettings() { return NULL; }
	virtual void CreateChildWnd() {}


	virtual CString GetName() { return ""; }

	int GetClosePos(int inputVal, int maxVal);//521 Used for solo vertical splider

	CXPMenu* m_pMenu;
	afx_msg LRESULT OnChildWndLBtnDownMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChildWndRBtnDownMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChildWndRBtnUpMsg(WPARAM wParam, LPARAM lParam);
	virtual void CreateMenu() {}
	void CreateSubmenuMask();
	void CreateSubmenuZone(int Type);//0:Add WF Presbia 2.4 GUI; 1: Do not add; 2: WF&CT GUI
	void CreateSubmenuMapSteps();
	void CreateSubmenuMapColors();
	void CreateSubmenuLtr();

	virtual void RepaintMemDC() override;

	void CSADisplay(real_t psSize, CCTExam* pExam, RECT Rect);//(for wfct iol selection assistant) Corneal Spherical Aberration Function Display
	void WFCTIolPan(RECT Rect, BOOL TraceySet, BOOL CorHoa, BOOL Inter, int Alpha_Dec, int TCHoa_Dec, int Cor_Ast_Dec, int Cor_SphAb_Dec, real_t vals[4]);//530 (for wfct iol selection assistant)
	void WFCTIolCon(RECT Rect, CString Con_Res1, CString Con_Res2, CString Con_Res3, CString Con_Res4); //for wfct iol consideration box

	void WFExamSmallInfo(CWFExam* pExam, RECT Rect, int type, int ColorType);
	void WFExamInfo(CWFExam* pExam, RECT Rect, int NumRows, BOOL* Rows);
	void WFExamLargeInfo(CWFExam* pExam, RECT Rect, int type);
	void WFCTExamLargeInfo(CWFExam* pExam, RECT Rect);// WFCT Angle K/A
	void ChangWFExamInfo(CWFExam* pExam, RECT Rect, int type, BOOL Show_Alpha);// To show simplied wf info for Chang Analysis
	void SoloCmpWFExamInfo(CWFExam* pExam, RECT Rect, int NumRows, BOOL* Rows, BOOL HighLight);
	void SoloVfaExamInfo(CWFExam* pExam, RECT Rect, int NumRows, BOOL* Rows, BOOL HighLight);
	void WFSoloExamInfo(CWFExam* pExam, RECT Rect, int NumRows, BOOL* Rows, BOOL HighLight);
	void WFSoloDoubleExamInfo(CWFExam* pExam, RECT Rect1, RECT Rect2, RECT Rect3, RECT Rect4, BOOL HighLight);

	void WFCTTorDataInfo(CWFExam* pWFExam, CCTExam* pCTExam, RECT Rect);

	void WFNearVisionInfo(CWFExam* pWFExam, RECT Rect);//6.2.0 WF Near Vision GUI
	void WFNearVisionInfo2(CWFExam* pWFExam, RECT Rect);//6.2.0 WF Near Vision GUI


	void DysInfo(CWFExam* pExam, RECT Rect, real_t dysVal, real_t *rms, int type);// To show Dysfunctional lens info

	void CTExamInfo(CCTExam* pExam, RECT Rect, int NumRows, BOOL* Rows);
	void CTExamLargeInfo(CCTExam* pExam, RECT Rect, int NumRows, BOOL* Rows);
	void ChangCTExamInfo(CCTExam* pExam, RECT Rect);// To show simplied ct info for Chang Analysis
	void OUWFExamInfo(CWFExam* pExam, RECT Rect);// For show the WF info of OU overview Window
	void OUCTExamInfo(CCTExam* pExam, RECT Rect);// For show the CT info of OU overview Window
	void VisualComplaints(CWFExam* pExam, RECT Rect, int NumRows);
	void SoloVisualComplaints(CWFExam* pExam, RECT Rect, int NumRows, BOOL HighLight);//521

	void VisualCTComplaints(CWFExam* pExam, RECT Rect, int NumRows);//show the complain

	int  GetFontSize(int w, CString inputStr, int OriFontSize);//Get the biggest fontsize of a string in a specified Rect

	void DrawGlassIcon(int makeType, int SizeSmall, int i);
	void WFCTTorRefInfo(CWFExam* pWFExam, CCTExam* pCTExam, RECT Rect);
	void CalIolEnh(CCTExam* pCTExam, CString &s1, CString &s2); // Calculate the IOL enhancement
	void CalTraceyRef(CWFExam* pWFExam, COLORREF &Color, real_t &Spre, real_t &Apre, CString &s);// Calculate the Tracey Refraction

	void OnType(int Type);
	afx_msg void OnTypeTEyeItemClicked() { OnType(TYPE_TEYE); }
	afx_msg void OnTypeTCeyItemClicked() { OnType(TYPE_TEYC); }
	afx_msg void OnTypeTRsdItemClicked() { OnType(TYPE_TRSD); }
	afx_msg void OnTypeTOpmItemClicked() { OnType(TYPE_TOPM); }
	afx_msg void OnTypeTWfmItemClicked() { OnType(TYPE_TWFM); }
	afx_msg void OnTypeTRfmItemClicked() { OnType(TYPE_TRFM); }
	afx_msg void OnTypeTPsfItemClicked() { OnType(TYPE_TPSF); }
	afx_msg void OnTypeTEefItemClicked() { OnType(TYPE_TEEF); }
	afx_msg void OnTypeTLtrItemClicked() { OnType(TYPE_TLTR); }


	afx_msg void OnTypeTMtfItemClicked() { OnType(TYPE_TMTF); }
	afx_msg void OnTypeTRmsItemClicked() { OnType(TYPE_TRMS); }
	afx_msg void OnTypeTAzrItemClicked() { OnType(TYPE_TAZR); }
	afx_msg void OnTypeCEyeItemClicked() { OnType(TYPE_CEYE); }
	afx_msg void OnTypeCAxmItemClicked() { OnType(TYPE_CAXM); }
	afx_msg void OnTypeCTnmItemClicked() { OnType(TYPE_CTNM); }
	afx_msg void OnTypeCRfmItemClicked() { OnType(TYPE_CRFM); }
	afx_msg void OnTypeCElmItemClicked() { OnType(TYPE_CELM); }
	afx_msg void OnTypeCWfmItemClicked() { OnType(TYPE_CWFM); }
	afx_msg void OnTypeCPsfItemClicked() { OnType(TYPE_CPSF); }
	afx_msg void OnTypeCEefItemClicked() { OnType(TYPE_CEEF); }
	afx_msg void OnTypeCMtfItemClicked() { OnType(TYPE_CMTF); }
	afx_msg void OnTypeCRmsItemClicked() { OnType(TYPE_CRMS); }
	afx_msg void OnTypeCLtrItemClicked() { OnType(TYPE_CLTR); }
	afx_msg void OnTypeCSkmItemClicked() { OnType(TYPE_CSKM); }
	afx_msg void OnTypeIWfmItemClicked() { OnType(TYPE_IWFM); }
	afx_msg void OnTypeIRfmItemClicked() { OnType(TYPE_IRFM); }
	afx_msg void OnTypeIPsfItemClicked() { OnType(TYPE_IPSF); }
	afx_msg void OnTypeIEefItemClicked() { OnType(TYPE_IEEF); }
	afx_msg void OnTypeIMtfItemClicked() { OnType(TYPE_IMTF); }
	afx_msg void OnTypeIRmsItemClicked() { OnType(TYPE_IRMS); }
	afx_msg void OnTypeILtrItemClicked() { OnType(TYPE_ILTR); }
	afx_msg void OnTypeDofItemClicked() { OnType(TYPE_DOFF); }

	afx_msg void OneOsherAlignmentClicked();

	afx_msg void OnMethod3DItemClicked();
	afx_msg virtual void OnSizeLargeItemClicked() {}

	afx_msg void OnMaskPistonItemClicked();
	afx_msg void OnMaskTiltItemClicked();
	void OnMask(int MaskType);
	afx_msg void OnMaskTotalItemClicked() { OnMask(MASK_TOTAL); }
	afx_msg void OnMaskLOTotalItemClicked() { OnMask(MASK_LO_TOTAL); }
	afx_msg void OnMaskDefocusItemClicked() { OnMask(MASK_DEFOCUS); }
	afx_msg void OnMaskAstigmatismItemClicked() { OnMask(MASK_ASTIGMATISM); }
	afx_msg void OnMaskHOTotalItemClicked() { OnMask(MASK_HO_TOTAL); }
	afx_msg void OnMaskComaItemClicked() { OnMask(MASK_COMA); }
	afx_msg void OnMaskTrefoilItemClicked() { OnMask(MASK_TREFOIL); }
	afx_msg void OnMaskSphericalItemClicked() { OnMask(MASK_SPHERICAL); }
	afx_msg void OnMaskSecAstigmatismItemClicked() { OnMask(MASK_4ORD_ASTIGMATISM); }
	afx_msg void OnMaskOtherItemClicked() { OnMask(MASK_OTHER); }
	afx_msg void OnMaskTotalNoDefocusItemClicked() { OnMask(MASK_TOTAL_NO_DEFOCUS); }

	void OnZone(int ZoneRadiusUm);

	afx_msg void OnZone02MMItemClicked()
	{
		if (m_WFCTDYSWnd) OnZone(1250);
		else OnZone(1000);
	}

	afx_msg void OnZone03MMItemClicked() { OnZone(1500); }
	afx_msg void OnZone04MMItemClicked() { OnZone(2000); }
	afx_msg void OnZone05MMItemClicked() { OnZone(2500); }
	afx_msg void OnZone06MMItemClicked() { OnZone(3000); }
	afx_msg void OnZone07MMItemClicked() { OnZone(3500); }
	afx_msg void OnZone08MMItemClicked() { OnZone(4000); }
	afx_msg void OnZone09MMItemClicked() { OnZone(4500); }
	afx_msg void OnZone10MMItemClicked() { OnZone(5000); }
	afx_msg void OnZonePresbiaMMItemClicked();
	afx_msg void OnZoneOtherItemClicked() { OnZone(0); }

	afx_msg LRESULT OnDecZoneMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnIncZoneMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSoloMaskMsg(WPARAM wParam, LPARAM lParam);

	void OnMapUnit(int MapUnit);
	afx_msg void OnMapDioptersItemClicked() { OnMapUnit(DIOPTERS); }
	afx_msg void OnMapMillimetersItemClicked() { OnMapUnit(MILLIMETERS); }

	void OnShow(BOOL* pCheck);
	afx_msg void OnShowMapEyeItemClicked() { OnShow(&GetWndSettings()->m_MapShowEye); }
	afx_msg void OnShowMapPupilItemClicked() { OnShow(&GetWndSettings()->m_MapShowPupil); }
	afx_msg void OnShowMapVertexItemClicked() { OnShow(&GetWndSettings()->m_MapShowCornealVertex); }
	afx_msg void OnShowMapLensItemClicked() { OnShow(&GetWndSettings()->m_MapShowWavetouchLens); }
	afx_msg void OnShowMapPointsItemClicked() { OnShow(&GetWndSettings()->m_MapShowPoints); }
	afx_msg void OnShowMapRingsItemClicked() { OnShow(&GetWndSettings()->m_MapShowRings); }
	afx_msg void OnShowMapLimbusItemClicked() { OnShow(&GetWndSettings()->m_MapShowLimbus); }
	afx_msg void OnShowMapMapItemClicked() { OnShow(&GetWndSettings()->m_MapShowMap); }
	afx_msg void OnShowMapNumbersItemClicked() { OnShow(&GetWndSettings()->m_MapShowNumbers); }
	afx_msg void OnShowMapKeratometryItemClicked() { OnShow(&GetWndSettings()->m_MapShowKeratometry); }
	afx_msg void OnShowMapAstigmatismAxesItemClicked() { OnShow(&GetWndSettings()->m_MapShowAstigmatismAxes); }
	afx_msg void OnShowMapSurfaceItemClicked() { OnShow(&GetWndSettings()->m_MapShowSolidSurface); }
	afx_msg void OnShowMapMeshItemClicked() { OnShow(&GetWndSettings()->m_MapShowWireMesh); }
	afx_msg void OnShowMapOsherRingItemClicked() { OnShow(&GetWndSettings()->m_MapShowOsherRing); }

	afx_msg void OnShowMapIncisionItemClicked() { OnShow(&::TempSettings.NewPlannerSettings.m_MapShowIncision); }
	afx_msg void OnShowMapPreopCornealItemClicked() { OnShow(&::TempSettings.NewPlannerSettings.m_MapShowPreopCorneal); }
	afx_msg void OnShowMapPreopInternalItemClicked() { OnShow(&::TempSettings.NewPlannerSettings.m_MapShowPreopInternal); }
	afx_msg void OnShowMapPreopEntireEyeItemClicked() { OnShow(&::TempSettings.NewPlannerSettings.m_MapShowPreopEntireEye); }
	afx_msg void OnShowMapPostopCornealItemClicked() { OnShow(&::TempSettings.NewPlannerSettings.m_MapPostopCorneal); }
	afx_msg void OnShowMapPostopInternalItemClicked() { OnShow(&::TempSettings.NewPlannerSettings.m_MapPostopInternal); }
	afx_msg void OnShowMapPostopEntireEyeItemClicked() { OnShow(&::TempSettings.NewPlannerSettings.m_MapPostopEntireEye); }
	afx_msg void OnShowMapICLinducedSteepItemClicked() { OnShow(&::TempSettings.NewPlannerSettings.m_MapShowICLIndSteep); }
	afx_msg void OnShowMapICLinducedFlatItemClicked() { OnShow(&::TempSettings.NewPlannerSettings.m_MapShowICLIndFlat); }

	afx_msg void OnShowPsfEefItemClicked() { OnShow(&GetWndSettings()->m_ShowEEF); }

	void OnEefType(int EefType);
	afx_msg void OnEefType1ItemClicked() { OnEefType(EEF_MAX); }
	afx_msg void OnEefType2ItemClicked() { OnEefType(EEF_CNT); }

	afx_msg void OnMapCentAutoItemClicked();
	afx_msg void OnMapCentUserItemClicked();

	void OnMapStep(real_t Step);
	afx_msg void OnMapStep0001ItemClicked() { OnMapStep(0.01); }
	afx_msg void OnMapStep0002ItemClicked() { OnMapStep(0.02); }
	afx_msg void OnMapStep0005ItemClicked() { OnMapStep(0.05); }
	afx_msg void OnMapStep0010ItemClicked() { OnMapStep(0.10); }
	afx_msg void OnMapStep0025ItemClicked() { OnMapStep(0.25); }
	afx_msg void OnMapStep0050ItemClicked() { OnMapStep(0.50); }
	afx_msg void OnMapStep0075ItemClicked() { OnMapStep(0.75); }
	afx_msg void OnMapStep0100ItemClicked() { OnMapStep(1.00); }
	afx_msg void OnMapStep0125ItemClicked() { OnMapStep(1.25); }
	afx_msg void OnMapStep0150ItemClicked() { OnMapStep(1.50); }
	afx_msg void OnMapStep0175ItemClicked() { OnMapStep(1.75); }
	afx_msg void OnMapStep0200ItemClicked() { OnMapStep(2.00); }
	afx_msg void OnMapStepUserItemClicked() { OnMapStep(-1.00); }
	afx_msg void OnMapStepAutoItemClicked() { OnMapStep(0.00); }

	void OnMapColors(int ColorsIndex);
	afx_msg void OnMapColors1ItemClicked() { OnMapColors(0); }
	afx_msg void OnMapColors2ItemClicked() { OnMapColors(1); }
	afx_msg void OnMapColorsUserItemClicked();

	afx_msg void OnMapTranslucentItemClicked();

	void OnLtr(int Ltr);
	afx_msg void OnLtrEItemClicked() { OnLtr(0); }
	afx_msg void OnLtrCItemClicked() { OnLtr(1); }

	void OnLtrLine(int LtrLine);
	afx_msg void OnLtrLine020ItemClicked() { OnLtrLine(20); }
	afx_msg void OnLtrLine025ItemClicked() { OnLtrLine(25); }
	afx_msg void OnLtrLine030ItemClicked() { OnLtrLine(30); }
	afx_msg void OnLtrLine040ItemClicked() { OnLtrLine(40); }
	afx_msg void OnLtrLine050ItemClicked() { OnLtrLine(50); }
	afx_msg void OnLtrLine070ItemClicked() { OnLtrLine(70); }
	afx_msg void OnLtrLine100ItemClicked() { OnLtrLine(100); }
	afx_msg void OnLtrLine200ItemClicked() { OnLtrLine(200); }

	void OnLtrOrientation(int LtrOrientation);
	afx_msg void OnLtrOrientation000ItemClicked() { OnLtrOrientation(0); }
	afx_msg void OnLtrOrientation090ItemClicked() { OnLtrOrientation(90); }
	afx_msg void OnLtrOrientation180ItemClicked() { OnLtrOrientation(180); }
	afx_msg void OnLtrOrientation270ItemClicked() { OnLtrOrientation(270); }

	void OnNLtr(int distanceType);
	afx_msg void OnNLtrLine1ItemClicked() { OnNLtr(1); }
	afx_msg void OnNLtrLine2ItemClicked() { OnNLtr(2); }
	afx_msg void OnNLtrLine3ItemClicked() { OnNLtr(3); }
	afx_msg void OnNLtrLine4ItemClicked() { OnNLtr(4); }
	afx_msg void OnNLtrLine5ItemClicked() { OnNLtr(5); }

	afx_msg void OnMtfAvgItemClicked();

	afx_msg void OnRmsCmbItemClicked();

	afx_msg void OnElmRefBestSphereItemClicked();
	afx_msg void OnElmRefBestConicoidItemClicked();
	afx_msg void OnElmRefUserConicoidItemClicked();

	afx_msg void OnSaveTxtItemClicked();

	afx_msg LRESULT OnChangePosMsg(WPARAM wParam, LPARAM lParam);

	void OnMouseWheel(uint nFlags, short zDelta, CPoint Point); // not a message handler

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
