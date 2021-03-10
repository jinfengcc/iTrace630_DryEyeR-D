/*******************************************************************
Autor:         Jinfeng Chen
E-mail:        jinfengcc@gmail.com
Version:       6.2.0
Date:          04-24-2017
Description:   Generate a new WF Display which shows the Near and Far vision comparison
*****************************************************************************************/


#include "StdAfx.h"
#include "Resource.h"
#include "BusyCursor.h"
#include "SelectExamDlg.h"
#include "LtrWnd.h"
#include "NearLtrWnd.h"
#include "MTFWnd.h"

#include "WFNearVisionSumWnd.h"


//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CWFNearVisionSumWnd, CSumWnd)

	ON_BN_CLICKED(IDC_NEAR_VISION_COMP_BUTTON, OnComBtnClicked)

	ON_MESSAGE(WM_SLIDER_LCLICK, OnSliderClick)
	ON_MESSAGE(WM_SLIDER_POS_CHANGED, OnSliderChangedPos)

END_MESSAGE_MAP()

//***************************************************************************************

CWFNearVisionSumWnd::CWFNearVisionSumWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CWFExam* pWFExamFar, CWFExam* pWFExamNear, int ExamAmount, int ExamOrder, int show) :
	CSumWnd(pWnd, WndRect, pPatient, NULL)
{
	CBusyCursor Cursor;

	m_ExamAmount = ExamAmount;
	m_ExamOrder = ExamOrder;

	//----------------------------------------------------
	RECT Rect;
	GetWindowRect(&Rect);
	real_t t = 3.0 * (0.03 * m_h) + m_g;
	real_t w = ((Rect.right - Rect.left) - 4.0 * m_g) / 3.0;
	real_t h = ((Rect.bottom - Rect.top - t) - 2.0 * m_g) / 2.0;
	::SetRect(&m_Rect[0], intRound(m_g), intRound(t), intRound(m_g + w), intRound(t + h)); // internal	wavefront Map
	::SetRect(&m_Rect[1], intRound(m_g + w + m_g), intRound(t), intRound(m_g + w + m_g + w), intRound(t + h)); // Cornea wavefront map
	::SetRect(&m_Rect[2], intRound(m_g + w + m_g + w + m_g), intRound(t), intRound(m_g + w + m_g + w + m_g + w), intRound(t + h)); // Cornea axial map

	::SetRect(&m_Rect[3], intRound(m_g), intRound(t + h + m_g), intRound(m_g + w), intRound(t + h + m_g + h));

	::SetRect(&m_OutlineRect[0], m_Rect[0].left, m_Rect[0].top, m_Rect[3].right, m_Rect[3].bottom); // internal	wavefront Map
	::SetRect(&m_OutlineRect[1], m_Rect[1].left, m_Rect[0].top, m_Rect[2].right, m_Rect[3].bottom); // internal	wavefront Map

	int h1 = intRound((real_t)(m_Rect[3].bottom - m_Rect[3].top) / 6.0);
	int sTop = m_Rect[3].top + h1 + m_g;

	int h2 = h - h1 - m_g;
	::SetRect(&m_Rect[4], intRound(m_g + w + m_g), intRound(t + h + m_g), intRound(m_g + w + m_g + w + m_g + w), intRound(t + h + m_g + h1));
	::SetRect(&m_Rect[5], intRound(m_g + w + m_g), intRound(t + h + m_g + h1 + m_g), intRound(m_g + w + m_g + w + m_g + w), intRound(t + h + m_g + h1 + m_g + h2));
	//----------------------------------------------------



	//----------------------------------------------------
	//The 'Compare button' For chosing another exam
	t = 0.02 * ::GetSystemMetrics(SM_CXSCREEN);
	int hBtn = intRound(t);
	int wBtn = intRound(3.5 * t);

	int fs = intRound(0.02 * ::GetSystemMetrics(SM_CYSCREEN));

	::SetRect(&Rect, m_Rect[3].right - 5 - wBtn, m_Rect[3].bottom - hBtn - 5, m_Rect[3].right - 5, m_Rect[3].bottom - 5);
	m_CompareBtn.SetBkColor(NOCOLOR);
	m_CompareBtn.SetText("Compare");
	m_CompareBtn.SetTip("Select another WF Exam for comparison");
	m_CompareBtn.SetFont(fs);
	m_CompareBtn.Create(_T(""), _T(""), WS_VISIBLE, Rect, this, IDC_NEAR_VISION_COMP_BUTTON);

	m_CompareBtn.ShowWindow(SW_SHOW);
	//----------------------------------------------------


	//----------------------------------------------------
	//Control Slider
	HSliderParameters SliderPars;

	SliderPars.Pos = 7.0 / 12.0;
	SliderPars.WideRatio = 0.2;
	SliderPars.scaleNum = 12;
	SliderPars.ScaleType = 2;

	int Lengths[13] = { { 1 },{ 0 },{ 1 },{ 0 },{ 1 },{ 0 },{ 1 },{ 0 },{ 1 },{ 0 },{ 1 },{ 0 },{ 1 } };

	CString UpLabels[13] = { { _T("∞") },{ "" },{ "" },{ "" },{ "100cm" },{ "" },{ "" },{ "" },{ "50cm" },{ "" },{ "" },{ "" },{ "33cm" } };

	CString DownLabels[13] = { { "0.00D" },{ "" },{ "" },{ "" },{ "1.00D" },{ "" },{ "" },{ "" },{ "2.00D" },{ "" },{ "" },{ "" },{ "3.00D" } };

	for (int i = 0; i <= 12; i++)
	{
		SliderPars.ScaleLength[i] = Lengths[i];
		SliderPars.UpLabels[i] = UpLabels[i];
		SliderPars.DownLabels[i] = DownLabels[i];
	}

	SliderPars.Title = "NEAR POINT   1.25D";

	SliderPars.OutlineHighlight[0] = 1;
	SliderPars.OutlineHighlight[1] = 0;
	SliderPars.OutlineHighlight[2] = 1;
	SliderPars.OutlineHighlight[3] = 1;

	m_Slider = new CHorSliderCtrl();

	m_Slider->Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, m_Rect[5], this, IDC_WFNEARVISION_SLIDER);

	m_Slider->m_Printing = m_Printing;

	m_Slider->SetParameters(SliderPars);


	//----------------------------------------------------


	m_pWFExamFar = pWFExamFar;
	m_pWFExamNear = pWFExamNear;

	real_t r_max_umFar = m_pWFExamFar->m_WfSurface.m_r_max_um;
	real_t r_max_umNear = m_pWFExamNear->m_WfSurface.m_r_max_um;
	m_ThisWndSetting.m_ZoneRadiusUm = r_max_umFar < r_max_umNear ? r_max_umFar : r_max_umNear;

	::TempSettings.Com_r_max_um = m_ThisWndSetting.m_ZoneRadiusUm;

	//Total eye Surfaces
	m_Surfaces[0] = m_pWFExamFar->m_WfSurface;
	m_Surfaces[1] = m_pWFExamNear->m_WfSurface;

	real_t r_max_um = 5000.0;
	r_max_um = __min(r_max_um, m_Surfaces[0].m_r_max_um);
	r_max_um = __min(r_max_um, m_ThisWndSetting.m_ZoneRadiusUm);


	m_ThisWndSetting.m_Mask.SetType(MASK_TOTAL);
	m_ThisWndSetting.m_Piston = TRUE;
	m_ThisWndSetting.m_Tilt = TRUE;
	m_ThisWndSetting.SetDefaultScale();
	m_ThisWndSetting.SetDefaultColors(1);
	m_ThisWndSetting.m_Method3D = FALSE;

	if (!(m_ThisWndSetting.m_Ltr == 0 || m_ThisWndSetting.m_Ltr == 1))
	{
		m_ThisWndSetting.m_Ltr = 0;
	}
	if (!(m_ThisWndSetting.m_LtrLine == 20 || m_ThisWndSetting.m_LtrLine == 25 || m_ThisWndSetting.m_LtrLine == 30 ||
		m_ThisWndSetting.m_LtrLine == 40 || m_ThisWndSetting.m_LtrLine == 50 || m_ThisWndSetting.m_LtrLine == 70 ||
		m_ThisWndSetting.m_LtrLine == 100 || m_ThisWndSetting.m_LtrLine == 200))
	{
		m_ThisWndSetting.m_LtrLine = 70;
	}
	if (!(m_ThisWndSetting.m_LtrOrientation == 0 || m_ThisWndSetting.m_LtrOrientation == 90 ||
		m_ThisWndSetting.m_LtrOrientation == 180 || m_ThisWndSetting.m_LtrOrientation == 270))
	{
		m_ThisWndSetting.m_LtrOrientation = 0;
	}

	m_ThisWndSetting.m_MTFAverage = TRUE;

	for (int i = 0; i < 6; i++)
	{
		m_InsideZone[i] = ::NewSettings.m_ShowPreZone_WF_VFAWnd;
	}

	m_ShiftVal = 1.25;

	m_OriMc4 = m_pWFExamNear->m_WfSurface.m_c[4];

	CreateChildWnd();

	Repaint();

	ShowWindow(show);
}

//***************************************************************************************

// 6.3.0 for fix leak memory pWFExamFar and pWFExamNear
~CWFNearVisionSumWnd()
{
  delete m_pWFExamFar;
  delete m_pWFExamNear;
}

//***************************************************************************************

void CWFNearVisionSumWnd::RepaintMemDC()
{
	CSumWnd::RepaintMemDC();


	WFNearVisionInfo(m_pWFExamFar, m_Rect[3]);
	WFNearVisionInfo2(m_pWFExamNear, m_Rect[4]);

	COLORREF white = m_Printing ? BLACK : WHITE;
	m_MemDC.DrawRectangle(m_OutlineRect[0], white, NOCOLOR);
	m_MemDC.DrawRectangle(m_OutlineRect[1], white, NOCOLOR);

	//6.2.1
	if (m_Printing)
	{
		PaintSlider();
	}
	//6.2.1
}

//***************************************************************************************

void CWFNearVisionSumWnd::PaintControls()
{

	PaintSlider();

	InvalidateRgn(&m_Rgn, FALSE);

	UpdateWindow();
}

//***************************************************************************************

//6.2.1 20181218
void CWFNearVisionSumWnd::PaintSlider()
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF black = m_Printing ? WHITE : BLACK;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	m_MemDC.DrawLine(m_Rect[5].left, m_Rect[5].top, m_Rect[5].left, m_Rect[5].bottom, 1, white);

	int m_w = m_Rect[5].right - m_Rect[5].left;
	int m_h = m_Rect[5].bottom - m_Rect[5].top;

	int h = (m_Rect[5].bottom - m_Rect[5].top) * m_Slider->m_Pars.WideRatio;
	int y0 = m_Rect[5].bottom - (m_Rect[5].bottom - m_Rect[5].top) / 3;//The position of the splider center line

	RECT UpRect, DownRect, TitleRect;

		//Highlight the outline
		if (m_Slider->m_Pars.OutlineHighlight[0] == 1)
		{
			m_MemDC.DrawLine(m_Rect[5].left, m_Rect[5].top, m_Rect[5].left, m_Rect[5].bottom, 1, white);
		}
		if (m_Slider->m_Pars.OutlineHighlight[1] == 1)
		{
			m_MemDC.DrawLine(m_Rect[5].left, m_Rect[5].top, m_Rect[5].right, m_Rect[5].top, 1, white);
		}
		if (m_Slider->m_Pars.OutlineHighlight[2] == 1)
		{
			m_MemDC.DrawLine(m_Rect[5].right - 1, m_Rect[5].top, m_Rect[5].right - 1, m_Rect[5].bottom, 1, white);
		}
		if (m_Slider->m_Pars.OutlineHighlight[3] == 1)
		{
			m_MemDC.DrawLine(m_Rect[5].left, m_Rect[5].bottom, m_Rect[5].right, m_Rect[5].bottom, 1, white);
		}
		//Highlight the outline

		int d = h / 2;

		int SmallFontSize = intRound(0.25* h);
		CMFont SmallFont(SmallFontSize, 400, "Arial");
		CMFont MediumFont(intRound(0.5* h), 400, "Arial");

		int m_Left = m_Rect[5].left + m_w*0.15;
		int m_Right = m_Rect[5].right - m_w*0.15;

		int w = m_Right - m_Left;

		int x0 = m_Left + intRound((1 - m_Slider->m_Pars.Pos) * w);

		if (x0 > m_Right) x0 = m_Right;

		//Draw the scales
		int xPos;
		int yPos1, yPos2;
		int ScaleFontSize = 1;
		for (int i = 0; i <= m_Slider->m_Pars.scaleNum; i++)
		{
			int  xPos = intRound(m_Left + real_t(i) * real_t(m_Right - m_Left) / m_Slider->m_Pars.scaleNum);

			if (i == m_Slider->m_Pars.scaleNum)
			{
				xPos = m_Right;
			}

			if (m_Slider->m_Pars.ScaleType == 0)
			{
				if (m_Slider->m_Pars.ScaleLength[i] == 0)
				{
					yPos1 = y0 - 0.7*h;
					yPos2 = y0;
					ScaleFontSize = 1;
				}
				else if (m_Slider->m_Pars.ScaleLength[i] == 1)
				{
					yPos1 = y0 - h;
					yPos2 = y0;
					ScaleFontSize = 2;
				}
			}
			else if (m_Slider->m_Pars.ScaleType == 1)
			{
				if (m_Slider->m_Pars.ScaleLength[i] == 0)
				{
					yPos1 = y0;
					yPos2 = y0 + 0.7*h;
					ScaleFontSize = 1;
				}
				else if (m_Slider->m_Pars.ScaleLength[i] == 1)
				{
					yPos1 = y0;
					yPos2 = y0 + h;
					ScaleFontSize = 2;
				}
			}
			else if (m_Slider->m_Pars.ScaleType == 2)
			{
				if (m_Slider->m_Pars.ScaleLength[i] == 0)
				{
					yPos1 = y0 - 0.35*h;
					yPos2 = y0 + 0.35*h;
					ScaleFontSize = 1;
				}
				else if (m_Slider->m_Pars.ScaleLength[i] == 1)
				{
					yPos1 = y0 - 0.5 * h;
					yPos2 = y0 + 0.5 * h;
					ScaleFontSize = 2;
				}
			}

			m_MemDC.DrawLine(xPos, yPos1, xPos, yPos2, ScaleFontSize, white);

			yPos1 = y0 - 0.5 * h;
			yPos2 = y0 + 0.5 * h;

			if (i == 0)
			{
				::SetRect(&UpRect, xPos - 40, yPos1 - 5 - SmallFontSize, xPos + 40, yPos1 - 5);
				m_MemDC.WriteText(m_Slider->m_Pars.UpLabels[i], UpRect, MediumFont, white, 1, NOCOLOR);
			}
			else
			{
				::SetRect(&UpRect, xPos - 40, yPos1 - 5 - SmallFontSize, xPos + 40, yPos1 - 5);
				m_MemDC.WriteText(m_Slider->m_Pars.UpLabels[i], UpRect, SmallFont, white, 1, NOCOLOR);
			}

			::SetRect(&DownRect, xPos - 40, yPos2 + 5, xPos + 40, yPos2 + 5 + SmallFontSize);
			m_MemDC.WriteText(m_Slider->m_Pars.DownLabels[i], DownRect, SmallFont, white, 1, NOCOLOR);
		}


		//----------------------------------------------------
		//Show the title
		int TitleY0 = m_Rect[5].top + (m_Rect[5].bottom - m_Rect[5].top) / 4;
		int TitleY1 = y0 - h - 15;
		::SetRect(&TitleRect, m_Left, TitleY0, m_Right, TitleY1);

		CMFont BigFont(TitleY1 - TitleY0, 400, "Arial");

		m_MemDC.WriteText(m_Slider->m_Pars.Title, TitleRect, BigFont, white, 1, NOCOLOR);
		//----------------------------------------------------


		//----------------------------------------------------
		//Draw the selected point
		int x1 = m_Rect[5].left + m_w * 0.15;
		int x2 = m_Rect[5].right - m_w * 0.15;

		w = x2 - x1;

		x0 = x1 + intRound((1 - m_Slider->m_Pars.Pos) * w);

		if (m_Slider->m_Pars.SliderRect.right < 0 || m_Slider->m_Pars.SliderRect.left  < 0 || m_Slider->m_Pars.SliderRect.right > 10000 || m_Slider->m_Pars.SliderRect.left > 10000)
		{
			::SetRect(&m_Slider->m_Pars.SliderRect, x1, y0 - 0.5 * h, x2, y0 + 0.5 * h);
		}

		m_MemDC.DrawLine(x1, y0, x2, y0, 2, white);

		m_MemDC.DrawCircle(x0, y0, intRound(0.2*d), 1, YELLOW, YELLOW);
		//----------------------------------------------------
}
//6.2.1 20181218

//***************************************************************************************

void CWFNearVisionSumWnd::CreateChildWnd()
{
	CBusyCursor Cursor;

	for (int i = 0; i < 3; i++)
	{
		SAFE_DELETE(m_pDispWnd[i]);
	}

	m_pWFExamNear->m_WfSurface.m_c[4] = m_OriMc4;

	m_pWFExamNear->m_WfSurface.AddSphEq(m_ShiftVal);

	m_Surfaces[0] = m_pWFExamFar->m_WfSurface;
	m_Surfaces[1] = m_pWFExamNear->m_WfSurface;

	real_t r_max_um = m_ThisWndSetting.m_ZoneRadiusUm;

	m_Surfaces[0].ChangeRMaxUm(r_max_um);
	m_Surfaces[0] &= m_ThisWndSetting.m_Mask;
	m_Surfaces[0].ComputePiston(m_ThisWndSetting.m_Piston);
	m_Surfaces[0].ComputeTilt(m_ThisWndSetting.m_Tilt);

	m_Surfaces[1].ChangeRMaxUm(r_max_um);
	m_Surfaces[1] &= m_ThisWndSetting.m_Mask;
	m_Surfaces[1].ComputePiston(m_ThisWndSetting.m_Piston);
	m_Surfaces[1].ComputeTilt(m_ThisWndSetting.m_Tilt);


	//-------------------------------------------------------
	//Window 1 Far vision LTR
	CLtrWnd* FarLtrWnd = new CLtrWnd(m_Rect[0], this);

	FarLtrWnd->SpecialBorder = TRUE;

	FarLtrWnd->HighLightOutline[0] = 1;
	FarLtrWnd->HighLightOutline[1] = 1;
	FarLtrWnd->HighLightOutline[2] = 1;
	FarLtrWnd->HighLightOutline[3] = 0;

	FarLtrWnd->m_DysVal = -1;
	FarLtrWnd->m_RTLabel[0] = "FAR";

	CWFExam::CreateLtr(m_Surfaces[0], FarLtrWnd->m_Ltr, m_ThisWndSetting.m_Ltr, m_ThisWndSetting.m_LtrLine, m_ThisWndSetting.m_LtrOrientation);
	FarLtrWnd->m_Type = m_ThisWndSetting.m_Type;
	FarLtrWnd->m_SizeSmall = TRUE;
	FarLtrWnd->m_Method3D = m_ThisWndSetting.m_Method3D;

	FarLtrWnd->m_LTLabel[0] = "TOTAL EYE - Letter";
	m_ThisWndSetting.m_Mask.GetName(FarLtrWnd->m_LTLabel[1]);
	FarLtrWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);

	FarLtrWnd->CreateZoomButtons();

	if (m_InsideZone[0] != TRUE)
	{
		FarLtrWnd->CreateZoneButtons();
	}
	else
	{
		FarLtrWnd->m_LTLabel[2] = "Presbia 2.4mm";
	}

	m_pDispWnd[0] = FarLtrWnd;


	//-------------------------------------------------------
	//Window 2 Near vision LTR
	CLtrWnd* pNearLtrWnd = new CLtrWnd(m_Rect[1], this);

	pNearLtrWnd->SpecialBorder = TRUE;

	pNearLtrWnd->HighLightOutline[0] = 1;
	pNearLtrWnd->HighLightOutline[1] = 1;
	pNearLtrWnd->HighLightOutline[2] = 0;
	pNearLtrWnd->HighLightOutline[3] = 0;

	pNearLtrWnd->m_DysVal = -1;
	pNearLtrWnd->m_RTLabel[0] = "NEAR";

	CWFExam::CreateLtr(m_Surfaces[1], pNearLtrWnd->m_Ltr, m_ThisWndSetting.m_Ltr, m_ThisWndSetting.m_LtrLine, m_ThisWndSetting.m_LtrOrientation);

	pNearLtrWnd->m_Type = m_ThisWndSetting.m_Type;
	pNearLtrWnd->m_SizeSmall = TRUE;
	pNearLtrWnd->m_Method3D = m_ThisWndSetting.m_Method3D;
	pNearLtrWnd->m_LTLabel[0] = "TOTAL EYE - Letter";
	m_ThisWndSetting.m_Mask.GetName(pNearLtrWnd->m_LTLabel[1]);
	pNearLtrWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);

	pNearLtrWnd->CreateZoomButtons();

	if (m_InsideZone[0] != TRUE)
	{
		pNearLtrWnd->CreateZoneButtons();
	}
	else
	{
		pNearLtrWnd->m_LTLabel[2] = "Presbia 2.4mm";
	}

	m_pDispWnd[1] = pNearLtrWnd;


	//-------------------------------------------------------
	//Window 3 Near MTF

	CMTFWnd* pMTFWnd = new CMTFWnd(m_Rect[2], this);
	CWFExam::CreateMtf(m_Surfaces[1], pMTFWnd->m_Mtf);
	pMTFWnd->m_Type = m_ThisWndSetting.m_Type;
	pMTFWnd->m_SizeSmall = TRUE;
	pMTFWnd->m_Method3D = m_ThisWndSetting.m_Method3D;

	pMTFWnd->SpecialBorder = TRUE;

	pMTFWnd->HighLightOutline[0] = 0;
	pMTFWnd->HighLightOutline[1] = 1;
	pMTFWnd->HighLightOutline[2] = 1;
	pMTFWnd->HighLightOutline[3] = 0;

	pMTFWnd->m_ShowDys = FALSE;

	pMTFWnd->m_LTLabel[0] = "TOTAL EYE - MTF";
	m_ThisWndSetting.m_Mask.GetName(pMTFWnd->m_LTLabel[1]);
	pMTFWnd->m_LTLabel[2].Format(_T("%.2f mm"), r_max_um * 0.002);
	pMTFWnd->m_RTLabel[0] = "NEAR";

	pMTFWnd->m_Average = m_ThisWndSetting.m_MTFAverage;
	pMTFWnd->m_Angle = m_ThisWndSetting.m_MTFAngle;

	if (!m_ThisWndSetting.m_Method3D && !m_ThisWndSetting.m_MTFAverage)
	{
		pMTFWnd->CreateSlider();
	}

	if (m_InsideZone[0] != TRUE)
	{
		pMTFWnd->CreateZoneButtons();
	}
	else
	{
		pMTFWnd->m_LTLabel[2] = "Presbia 2.4mm";
	}

	m_pDispWnd[2] = pMTFWnd;


	//----------------------------------------------

	if (m_ThisWndSetting.m_ZoneRadiusUm >= ::TempSettings.Com_r_max_um)
	{
		m_pDispWnd[0]->m_IncZoneButton.SetEnabled(FALSE);
		m_pDispWnd[0]->m_IncZoneButton.Repaint();

		m_pDispWnd[1]->m_IncZoneButton.SetEnabled(FALSE);
		m_pDispWnd[1]->m_IncZoneButton.Repaint();

		m_pDispWnd[2]->m_IncZoneButton.SetEnabled(FALSE);
		m_pDispWnd[2]->m_IncZoneButton.Repaint();
	}

	if (m_ThisWndSetting.m_ZoneRadiusUm == 500)
	{
		m_pDispWnd[0]->m_DecZoneButton.SetEnabled(FALSE);
		m_pDispWnd[0]->m_DecZoneButton.Repaint();

		m_pDispWnd[1]->m_DecZoneButton.SetEnabled(FALSE);
		m_pDispWnd[1]->m_DecZoneButton.Repaint();

		m_pDispWnd[2]->m_DecZoneButton.SetEnabled(FALSE);
		m_pDispWnd[2]->m_DecZoneButton.Repaint();
	}

	Invalidate(FALSE);
}

//***************************************************************************************

void CWFNearVisionSumWnd::OnComBtnClicked()
{
	if (m_ExamAmount == 1)
	{
		::Info("There is only one wavefront exam under wavefront list.");

		return;
	}

	GUID PatientID = m_pWFExamNear->m_Header.m_PatientID;

	int H = ::GetSystemMetrics(SM_CYSCREEN);

	int FontSize = intRound(0.02 * H);

	CMFont Font;
	Font.Create(FontSize, 400, "Arial");

	int           NumImages[NUM_EXAMS_MAX];//image number of exam (if color imgage has been taken, this values is 2)

	CSelectExamDlg* pDlg = new CSelectExamDlg(this, PatientID, m_ExamOrder, EXAM_TYPE_WF, EYE_BOTH, &Font, NumImages);

	if (pDlg->DoModal() == IDOK)
	{
		if (m_pWFExamFar != NULL)
		{
			delete m_pWFExamFar;
			m_pWFExamFar = NULL;
		}

		m_pWFExamFar = new CWFExam;

		::DB.LoadWFExam(pDlg->m_ExamID, m_pWFExamFar);
		m_pWFExamFar->Process();

		real_t r_max_umFar = m_pWFExamFar->m_WfSurface.m_r_max_um;

		real_t r_max_umNear = m_pWFExamNear->m_WfSurface.m_r_max_um;

		m_ThisWndSetting.m_ZoneRadiusUm = r_max_umFar < r_max_umNear ? r_max_umFar : r_max_umNear;

		::TempSettings.Com_r_max_um = m_ThisWndSetting.m_ZoneRadiusUm;

		//Total eye Surfaces
		m_Surfaces[0] = m_pWFExamFar->m_WfSurface;
		m_Surfaces[1] = m_pWFExamNear->m_WfSurface;

		real_t r_max_um = 5000.0;
		r_max_um = __min(r_max_um, m_Surfaces[0].m_r_max_um);
		r_max_um = __min(r_max_um, m_ThisWndSetting.m_ZoneRadiusUm);

		m_Surfaces[0].ChangeRMaxUm(r_max_um);
		m_Surfaces[1].ChangeRMaxUm(r_max_um);

		CreateChildWnd();

		Repaint();
	}
}

//***************************************************************************************

LRESULT CWFNearVisionSumWnd::OnSliderChangedPos(WPARAM wParam, LPARAM lParam)
{
	real_t D = 3 - 3 * m_Slider->m_Pars.Pos;

	real_t dis = fabs(m_ShiftVal - D);

	if (dis < 0.1) return 0;

	m_ShiftVal = D;

	CreateChildWnd();

	Repaint();

	return 0;
}

//***************************************************************************************

LRESULT CWFNearVisionSumWnd::OnSliderClick(WPARAM wParam, LPARAM lParam)
{
	real_t D = 3 - 3 * m_Slider->m_Pars.Pos;

	m_ShiftVal = D;

	m_Slider->OnPaint();

	CreateChildWnd();

	Repaint();

	return 0;
}

//***************************************************************************************
