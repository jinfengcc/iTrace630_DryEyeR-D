//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "SumWnd.h"

#include "GlobalFunctions.h"//6.2.0 For read Chinese name

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

int angular_distance_counterclockwise3(int a1, int a2)
{
	int da = (a2 - a1) % 360;
	if (da < 0) da += 360;
	return da;
}

// distance between a1 and a2 ?[0, 180)
int angular_distance3(int a1, int a2)
{
	int da = angular_distance_counterclockwise3(a1, a2);
	if (da > 180) da = 360 - da;
	return da;
}

//***************************************************************************************
//520 For show the info of OU overview Window

void CSumWnd::OUWFExamInfo(CWFExam* pExam, RECT Rect)
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);

	CString s, s1, s2, s3;

	int L = Rect.left + 3, T = Rect.top, R = Rect.right - 3, B = Rect.bottom;

	real_t t = T;
	real_t h = (Rect.bottom - Rect.top) / (real_t)6;

	CMFont Font(intRound(0.9 * h), 400, "Arial");

	// date / time,   
	::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
	s = "WF ";
	m_MemDC.WriteText(s, Rect, Font, pExam->m_Header.IsEyeManuallyOverriden() ? RED : white, 0);
	s.Format(_T(" %s  %02i:%02i:%02i"),
		::DateToStr(pExam->m_Header.m_Year, pExam->m_Header.m_Month, pExam->m_Header.m_Day), pExam->m_Header.m_Hour, pExam->m_Header.m_Minute, pExam->m_Header.m_Second);
	m_MemDC.WriteText(s, Rect, Font, white, 2);

	// empty line
	t += h;
	::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
	m_MemDC.WriteText(" ", Rect, Font, white, 2);

	// limbus / pupil / scan
	t += h;
	::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
	//m_MemDC.WriteText(" Limbus / Pupil / Scan", Rect, Font, white, 0);//
	m_MemDC.WriteText(" Pupil / Scan", Rect, Font, white, 0);//
															 //if (pExam->m_Image.m_li_ok) {
															 //  s1.Format(_T("%.2f"), pExam->m_Image.m_li_r_mean_um * 0.002);
															 //}
															 //else {
															 //  s1 = "---";//WFCT
															 //}
	if (pExam->m_Image.m_pu_ok) {
		s2.Format(_T("%.2f"), pExam->m_Image.m_pu_r_mean_um * 0.002);
	}
	else {
		s2 = "---";
	}
	s3.Format(_T("%.2f"), pExam->m_ScanDiameter * 0.001);
	/*s = s1 + " / " + s2 + " / " + s3 + " mm ";*///
	s = s2 + " / " + s3 + " mm ";//
	m_MemDC.WriteText(s, Rect, Font, white, 2);


	real_t SphEq, Sph, Cyl; int Axis;
	CZernikeSurface Surface;

	// Tracey refraction
	t += h;

	COLORREF Color;
	if (pExam->m_Complaints.m_NightMiopia == COMP_SEVERE ||
		pExam->m_Complaints.m_NightHyperopia == COMP_SEVERE ||
		pExam->m_Complaints.m_Astigmatism == COMP_SEVERE ||
		pExam->m_Complaints.m_BlurDoubleVision == COMP_SEVERE ||
		pExam->m_Complaints.m_GlareHalo == COMP_SEVERE ||
		pExam->m_Complaints.m_MixedFocus == COMP_SEVERE)
	{
		Color = RED;
	}
	else if (pExam->m_Complaints.m_NightMiopia == COMP_MODERATE ||
		pExam->m_Complaints.m_NightHyperopia == COMP_MODERATE ||
		pExam->m_Complaints.m_Astigmatism == COMP_MODERATE ||
		pExam->m_Complaints.m_BlurDoubleVision == COMP_MODERATE ||
		pExam->m_Complaints.m_GlareHalo == COMP_MODERATE ||
		pExam->m_Complaints.m_MixedFocus == COMP_MODERATE) {
		Color = m_Printing ? ORANGE : YELLOW;
	}
	else {
		Color = GREEN;
		//Color = m_Printing ? ORANGE : YELLOW;
	}

	::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
	m_MemDC.WriteText(" Tracey Refraction", Rect, Font, Color, 0);
	//
	Surface = pExam->m_WfSurface;
	Surface.ChangeRMaxUm(2000.0);
	CWFExam::GetSpheqSphCylAxis(Surface, ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
	//
	if (::Settings.m_RefractionRounding == 0) {
		Sph = round8(Sph);
		Cyl = round8(Cyl);
	}
	s.Format(_T("%+.2f D %+.2f D x %i° "), Sph, Cyl, Axis);
	m_MemDC.WriteText(s, Rect, Font, Color, 2);
	//Done;

	//Ho Total //AAA
	t += h;
	::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
	//  
	s = " HO Total";
	m_MemDC.WriteText(s, Rect, Font, white, 0);

	// microns
	int MaskType = MASK_HO_TOTAL;
	CZernikeMask Mask;
	Mask.SetType(MaskType);
	Surface = pExam->m_WfSurface;
	Surface &= Mask;
	real_t rms = Surface.ComputeRms();
	s.Format(_T("%6.3f µ "), rms);

	m_MemDC.WriteText(s, Rect, Font, white, 2);
	//Done

	// notes
	s = G_ReadChinese(pExam->m_Header.m_Note);
	int l = s.GetLength();
	int i;

	for (i = 1; i <= l; i++)
	{
		if (s[i - 1] == '\n') break;
		m_MemDC.MeasureRect(" " + s.Left(i) + " ", Rect, Font);
		if (Rect.right - Rect.left > R - L) break;
	}

	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" " + s.Left(i) + " ", Rect, Font, white, 0);
	}
}
//******************************************************************************************
//530 To show simplied wf info for Chang Analysis 
void CSumWnd::ChangWFExamInfo(CWFExam* pExam, RECT Rect, int type, BOOL Show_Alpha)
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);

	CString s, s1, s2, s3;

	int L = Rect.left + 3, T = Rect.top + 3, R = Rect.right - 3, B = Rect.bottom;

	real_t t = T;
	real_t h = (Rect.bottom - Rect.top) / 15;
	real_t smallh = 4 * h / 5.0;

	//CMFont Font(intRound(0.9 * h), 400, "Arial");
	CMFont Font(intRound(0.8 * 1.1 * h), 400, "Arial");
	CMFont SmallFont(intRound(0.7 * 1.1 * h), 400, "Arial");
	CMFont VerySmallFont(intRound(0.5 * 1.1 * h), 400, "Arial");
	CMFont BigFont(intRound(1.5 * 1.1 * h), 400, "Arial");
	//CMFont BoldFont(intRound(1.1 * h), FW_BOLD, "Arial");//[5.1.1]

	// date / time, od/os
	if (type == 1)
	{
		::SetRect(&Rect, L, intRound(t + 5), R - 25, intRound(t + h + 10));
		s = pExam->m_Header.Eye(); s += " ";
		m_MemDC.WriteText(s, Rect, BigFont, pExam->m_Header.IsEyeManuallyOverriden() ? RED : white, 2);
		s.Format(_T("  %s  %02i:%02i:%02i"),
			::DateToStr(pExam->m_Header.m_Year, pExam->m_Header.m_Month, pExam->m_Header.m_Day), pExam->m_Header.m_Hour, pExam->m_Header.m_Minute, pExam->m_Header.m_Second);
		m_MemDC.WriteText(s, Rect, Font, white, 0);
	}


	// clinic / physician / operator
	int r1 = intRound(real_t(R - L - 10) / 3.0);
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), L + r1, intRound(t + h + 1));
		CClinic* pClinic = ::DB.GetClinic(pExam->m_Header.m_ClinicID);
		if (pClinic)
		{
			//m_MemDC.WriteText(" Clinic", Rect, SmallFont, white, 0);
			m_MemDC.WriteText("  " + G_ReadChinese(pClinic->m_Name), Rect, VerySmallFont, white, 0);
		}

		::SetRect(&Rect, L + r1, intRound(t), L + r1 + r1, intRound(t + h + 1));
		CPhysician* pPhysician = ::DB.GetPhysician(pExam->m_Header.m_PhysicianID);
		if (pPhysician)
		{
			//m_MemDC.WriteText(" Physician", Rect, SmallFont, white, 0);
			m_MemDC.WriteText(G_ReadChinese(pPhysician->FullName()) + " ", Rect, VerySmallFont, white, 0);
		}

		::SetRect(&Rect, L + r1 + r1, intRound(t), L + r1 + r1 + r1, intRound(t + h + 1));

		COperator* pOperator = ::DB.GetOperator(pExam->m_Header.m_OperatorID);
		if (pOperator)
		{
			//m_MemDC.WriteText(" Operator", Rect, SmallFont, white, 0);
			m_MemDC.WriteText(G_ReadChinese(pOperator->FullName()) + " ", Rect, VerySmallFont, white, 0);
		}
	}


	// limbus / pupil / scan
	t += h;
	if (type == 1)
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		/*if (pExam->m_Image.m_li_ok) {
		s1.Format(_T("  Limbus %.2f mm  /  ", pExam->m_Image.m_li_r_mean_um * 0.002);
		}
		else {*/
		//s1 = "Limbus ---  /  ";//
		//}
		if (pExam->m_Image.m_pu_ok) {
			s2.Format(_T("Pupil %.2f mm  /  "), pExam->m_Image.m_pu_r_mean_um * 0.002);
		}
		else {
			s2 = "Pupil ---  /  ";
		}
		s3.Format(_T("Scan %.2f mm"), pExam->m_ScanDiameter * 0.001);
		//s = s1 + s2 +s3;//
		s = s2 + s3;//
		m_MemDC.WriteText(s, Rect, SmallFont, white, 0);
	}


	real_t SphEq, Sph, Cyl; int Axis;
	CZernikeSurface Surface;

	// Tracey refraction
	t += h;
	{
		COLORREF Color;
		if (pExam->m_Complaints.m_NightMiopia == COMP_SEVERE ||
			pExam->m_Complaints.m_NightHyperopia == COMP_SEVERE ||
			pExam->m_Complaints.m_Astigmatism == COMP_SEVERE ||
			pExam->m_Complaints.m_BlurDoubleVision == COMP_SEVERE ||
			pExam->m_Complaints.m_GlareHalo == COMP_SEVERE ||
			pExam->m_Complaints.m_MixedFocus == COMP_SEVERE) {
			Color = RED;
		}
		else if (pExam->m_Complaints.m_NightMiopia == COMP_MODERATE ||
			pExam->m_Complaints.m_NightHyperopia == COMP_MODERATE ||
			pExam->m_Complaints.m_Astigmatism == COMP_MODERATE ||
			pExam->m_Complaints.m_BlurDoubleVision == COMP_MODERATE ||
			pExam->m_Complaints.m_GlareHalo == COMP_MODERATE ||
			pExam->m_Complaints.m_MixedFocus == COMP_MODERATE) {
			Color = m_Printing ? ORANGE : YELLOW;
		}
		else {
			Color = GREEN;
		}

		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText("  Tracey Refraction", Rect, Font, Color, 0);
		//
		Surface = pExam->m_WfSurface;
		Surface.ChangeRMaxUm(2000.0);
		CWFExam::GetSpheqSphCylAxis(Surface, ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
		//
		if (::Settings.m_RefractionRounding == 0) {
			Sph = round8(Sph);
			Cyl = round8(Cyl);
		}
		s.Format(_T("%+.2f D %+.2f D x %i° "), Sph, Cyl, Axis);
		m_MemDC.WriteText(s, Rect, Font, Color, 2);
	}

	// refraction  
	m_MemDC.MeasureRect("          %.2f mm ", Rect, Font);
	int w1 = Rect.right - Rect.left;

	for (int i = 0; i < 4; i++)
	{
		t += smallh;
		{
			CString LineS = "";
			//
			Surface = pExam->m_WfSurface;
			//
			real_t r_max_um = i < 3 ? 0.5 * ::Settings.m_ZoneDiameterUm[i] : Surface.m_r_max_um;
			if (r_max_um <= Surface.m_r_max_um)
			{
				//
				Surface.ChangeRMaxUm(r_max_um);
				CWFExam::GetSpheqSphCylAxis(Surface, ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);

				// zone size
				::SetRect(&Rect, L, intRound(t), L + w1, intRound(t + h + 1));
				s.Format(_T("         %.2f mm "), 0.002 * Surface.m_r_max_um);
				m_MemDC.WriteText(s, Rect, SmallFont, white, 0);

				// sphere       
				s.Format(_T("%+.2f D "), Sph);
				LineS = s;

				// cylinder
				s.Format(_T("%+.2f D x "), Cyl);
				LineS += s;

				// axis
				::SetRect(&Rect, L + w1, intRound(t), R, intRound(t + h + 1));
				s.Format(_T("%i° "), Axis);
				LineS += s;
				m_MemDC.WriteText(LineS, Rect, SmallFont, white, 2);
			}
			else
			{
				::SetRect(&Rect, L, intRound(t), L + w1, intRound(t + h + 1));
				s.Format(_T("         %.2f mm "), 0.002 * r_max_um);
				m_MemDC.WriteText(s, Rect, SmallFont, gray, 0);
			}
		}
	}

	t += smallh;

	// rms
	m_MemDC.MeasureRect("+00.000 µ ", Rect, SmallFont);
	int w2 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("x 000° ", Rect, SmallFont);
	int w3 = Rect.right - Rect.left;
	int w0 = (R - L) - (w2 + w3);

	m_MemDC.MeasureRect("   HO Total @ D <= 0.00 mm", Rect, Font);
	w1 = Rect.right - Rect.left;

	CString an;
	int MaskType[4] = { MASK_HO_TOTAL, MASK_COMA, MASK_SPHERICAL, MASK_TREFOIL };
	for (int i = 0; i < 4; i++)
	{
		t += smallh;
		{
			//
			CZernikeMask Mask;
			Mask.SetType(MaskType[i]);
			Surface = pExam->m_WfSurface;
			Surface &= Mask;
			real_t rms = Surface.ComputeRms();
			//

			Mask.GetName(s);

			if (i == 0)
			{
				::SetRect(&Rect, L, intRound(t), L + w1, intRound(t + h + 1));
				s.Format(_T("  HO Total @ D <= %.2f mm"), 0.002 * Surface.m_r_max_um);
				m_MemDC.WriteText(s, Rect, Font, white, 0);

				::SetRect(&Rect, L + w1, intRound(t), R, intRound(t + h + 1));
				s.Format(_T("%6.3f µ "), rms);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
			else
			{
				::SetRect(&Rect, L, intRound(t), L + w1, intRound(t + h + 1));
				if (MaskType[i] == MASK_SPHERICAL)
				{
					s += " Aberration";
				}
				m_MemDC.WriteText("          " + s, Rect, SmallFont, white, 0);

				s.Format(_T("%6.3f µ "), rms);

				::SetRect(&Rect, L + w0, intRound(t), L + w0 + w2, intRound(t + h + 1));
				if (MaskType[i] == MASK_SPHERICAL)
				{
					if (Surface.m_c[12] > 0.0) s = "+" + s;
					else if (Surface.m_c[12] < 0.0) s = "-" + s;
				}

				m_MemDC.WriteText(s, Rect, SmallFont, white, 2);

				::SetRect(&Rect, L + w0 + w2, intRound(t), R, intRound(t + h + 1));

				if (MaskType[i] == MASK_COMA)
				{
					s = "x";
					m_MemDC.WriteText(s, Rect, SmallFont, white, 0);
					s.Format(_T("%i° "), intRound(angle(Surface.m_c[7], Surface.m_c[8]) * _180_Pi) % 360);
					m_MemDC.WriteText(s, Rect, SmallFont, white, 2);
				}

				if (MaskType[i] == MASK_TREFOIL)
				{
					s = "x ";
					m_MemDC.WriteText(s, Rect, SmallFont, white, 0);
					s.Format(_T("%i° "), intRound(angle(Surface.m_c[6], Surface.m_c[9]) * _180_Pi / 3.0) % 120);
					m_MemDC.WriteText(s, Rect, SmallFont, white, 2);
				}
			}
		}
	}
	t += smallh;
	t += smallh;

	//Angle Alpha 
	if (Show_Alpha)
		//if(pExam->m_AlignmentMethod == ALIGNMENT_4_IR_DOTS && Show_Alpha)
	{
		s = "  Angle Alpha  D = ";
		m_MemDC.MeasureRect(s + "      ", Rect, Font);
		w1 = Rect.right - Rect.left;

		::SetRect(&Rect, L, intRound(t), L + w1, intRound(t + h + 1));
		if (pExam->m_Header.m_AngleAlphaOK)
		{
			COLORREF yellow = m_Printing ? ORANGE : YELLOW;//601

			if (pExam->m_Header.m_AngleAlphaRUm < 300)     m_MemDC.WriteText(s, Rect, Font, GREEN, 0);
			else
			{
				if (pExam->m_Header.m_AngleAlphaRUm < 500) m_MemDC.WriteText(s, Rect, Font, yellow, 0);
				else                                       m_MemDC.WriteText(s, Rect, Font, RED, 0);
			}

			if (pExam->m_Header.m_AngleAlphaRUm == 0)  pExam->m_Header.m_AngleAlphaADg = 0;//600
			s.Format(_T("%.3f mm @ %i° "), pExam->m_Header.m_AngleAlphaRUm*0.001, pExam->m_Header.m_AngleAlphaADg);
			::SetRect(&Rect, L + w1, intRound(t), R, intRound(t + h + 1));

			//531
			if (pExam->m_Header.m_AngleAlphaRUm < 300)     m_MemDC.WriteText(s, Rect, Font, GREEN, 2);
			else
			{
				if (pExam->m_Header.m_AngleAlphaRUm < 500) m_MemDC.WriteText(s, Rect, Font, yellow, 2);
				else                                       m_MemDC.WriteText(s, Rect, Font, RED, 2);
			}
			//531
		}
		else
		{
			m_MemDC.WriteText(s, Rect, Font, gray, 0);
		}
	}
	/*else
	{
	s = "  Angle Alpha  D = ";
	m_MemDC.MeasureRect(s + "      ", Rect, Font);
	w1 = Rect.right - Rect.left;

	::SetRect(&Rect, L, intRound(t), L + w1, intRound(t + h + 1));

	m_MemDC.WriteText(s, Rect, Font, gray, 0);
	}*/
	//Done

	// notes
	int nn = 0;
	t += h;
	s = " " + G_ReadChinese(pExam->m_Header.m_Note);

	int l = s.GetLength();
	while (l > 0 && nn < 1)
	{
		int i;
		for (i = 1; i <= l; i++) {
			if (s[i - 1] == '\n') break;
			m_MemDC.MeasureRect(" " + s.Left(i) + " ", Rect, SmallFont);
			if (Rect.right - Rect.left > R - L) {
				for (; i >= 1; i--) {
					if (s[i - 1] == ' ') break;
				}
				break;
			}
		}

		t += h;
		nn++;
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText(" " + s.Left(i) + " ", Rect, SmallFont, white, 0);
		}

		s = s.Right(l - i);
		l -= i;
	}
}

//******************************************************************************************

//530 (for wfct iol selection assistant)
void CSumWnd::WFCTIolPan(RECT Rect, BOOL TraceySet, BOOL CorHoa, BOOL Inter, int Alpha_Dec, int TCHoa_Dec, int Cor_Ast_Dec, int Cor_SphAb_Dec, real_t vals[4])
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	//
	m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);

	RECT TempRect;
	CString s;

	int L = Rect.left + 5, T = Rect.top + 3, R = Rect.right - 3, B = Rect.bottom;

	int h = intRound((Rect.bottom - Rect.top - 5) / 30);
	int top = T;
	int btm = intRound(top + h);

	CMFont Font(h, 400, "Arial");
	CMFont TextFont(intRound(0.9 * h), 400, "Arial");
	//CMFont BoldFont(intRound(1.1 * h), FW_BOLD, "Arial");//[5.1.1]

	int n = 0;
	//Title 
	::SetRect(&TempRect, L, top, R, btm);
	s = "IOL Selection Checklist";

	m_MemDC.WriteText(s, TempRect, Font, white, 0);
	//Done

	top += h;
	btm = (top + h);
	s = "Yes";
	::SetRect(&TempRect, R - 65, top, R - 40, btm);
	m_MemDC.WriteText(s, TempRect, TextFont, white, 1);

	s = "No";
	::SetRect(&TempRect, R - 30, top, R - 5, btm);
	m_MemDC.WriteText(s, TempRect, TextFont, white, 1);

	top += h;
	btm = (top + h);
	s = "Is Tracey Refraction normal?";
	::SetRect(&TempRect, L, top, R, btm);
	m_MemDC.WriteText(s, TempRect, TextFont, white, 0);

	//BackGround of TraceySet CheckBox
	if (TraceySet) ::SetRect(&TempRect, R - 62, top - 2, R - 45, top + 15);
	else          ::SetRect(&TempRect, R - 27, top - 2, R - 10, top + 15);
	m_MemDC.FillSolidRect(&TempRect, BLUE);
	//Done

	top += h;
	top += h;
	btm = (top + h);
	s = "Are Corneal HOAs present?";
	::SetRect(&TempRect, L, top, R, btm);
	m_MemDC.WriteText(s, TempRect, TextFont, white, 0);

	//BackGround of TraceySet CheckBox
	if (CorHoa)    ::SetRect(&TempRect, R - 62, top - 2, R - 45, top + 15);
	else          ::SetRect(&TempRect, R - 27, top - 2, R - 10, top + 15);
	m_MemDC.FillSolidRect(&TempRect, BLUE);
	//Done

	top += h;
	top += h;
	btm = (top + h);
	//s = "Do Internal / Entire Eye /";
	s = "Are Internal HOAs present?";
	::SetRect(&TempRect, L, top, R, btm);
	m_MemDC.WriteText(s, TempRect, TextFont, white, 0);

	//BackGround of TraceySet CheckBox
	if (Inter)     ::SetRect(&TempRect, R - 62, top - 2, R - 45, top + 15);
	else          ::SetRect(&TempRect, R - 27, top - 2, R - 10, top + 15);
	m_MemDC.FillSolidRect(&TempRect, BLUE);
	//Done

	top += h;
	/*btm = (top + h);
	s = "Potential Visual Complaints";
	::SetRect(&TempRect, L, top, R, btm);
	m_MemDC.WriteText(s, TempRect, TextFont, white, 0);*/
	// 

	top += h;
	/* btm = (top + h);
	s = "Correlate?";
	::SetRect(&TempRect, L, top, R, btm);
	m_MemDC.WriteText(s, TempRect, TextFont, white, 0);*/
	// 

	//
	CString val, s1, s2, s3, s4;

	//Angle a
	if (vals[0] < 0) val = "No value";//531 done
	else            val.Format(_T("%.3fmm"), vals[0]);

	COLORREF thisColor = Alpha_Dec == 0 ? white : gray;
	top += h;
	top += h;
	btm = (top + h);
	s1.Format(_T("%.2fmm"), ::NewSettings.m_IOLSel_Alpha[0]);
	s2.Format(_T("%.2fmm"), ::NewSettings.m_IOLSel_Alpha[1]);
	s = "Angle Alpha >= " + s1;
	::SetRect(&TempRect, L, top, R, btm);
	m_MemDC.WriteText(s, TempRect, TextFont, thisColor, 0);

	if (thisColor == white)
	{
		TempRect.right -= 10;
		m_MemDC.WriteText(val, TempRect, TextFont, white, 2);
	}

	thisColor = Alpha_Dec == 1 ? white : gray;
	top += h;
	btm = (top + h);
	s = "Angle Alpha  " + s1 + " - " + s2;
	::SetRect(&TempRect, L, top, R, btm);
	m_MemDC.WriteText(s, TempRect, TextFont, thisColor, 0);

	if (thisColor == white)
	{
		TempRect.right -= 10;
		m_MemDC.WriteText(val, TempRect, TextFont, white, 2);
	}

	thisColor = Alpha_Dec == 2 ? white : gray;
	top += h;
	btm = (top + h);
	s = "Angle Alpha <= " + s2;
	::SetRect(&TempRect, L, top, R, btm);
	m_MemDC.WriteText(s, TempRect, TextFont, thisColor, 0);

	if (thisColor == white)
	{
		TempRect.right -= 10;
		m_MemDC.WriteText(val, TempRect, TextFont, white, 2);
	}
	//


	// Total Corn. HOA
	val.Format(_T("%.2fµ"), vals[1]);
	thisColor = TCHoa_Dec == 1 ? white : gray;
	top += h;
	top += h;
	btm = (top + h);
	s1.Format(_T("%.3fµ"), ::NewSettings.m_IOLSel_CHOA);
	s = "Total Corn. HOA >= " + s1;
	::SetRect(&TempRect, L, top, R, btm);
	m_MemDC.WriteText(s, TempRect, TextFont, thisColor, 0);

	if (thisColor == white)
	{
		TempRect.right -= 10;
		m_MemDC.WriteText(val, TempRect, TextFont, white, 2);
	}

	thisColor = TCHoa_Dec == 0 ? white : gray;
	top += h;
	btm = (top + h);
	s = "Total Corn. HOA < " + s1;
	::SetRect(&TempRect, L, top, R, btm);
	m_MemDC.WriteText(s, TempRect, TextFont, thisColor, 0);

	if (thisColor == white)
	{
		TempRect.right -= 10;
		m_MemDC.WriteText(val, TempRect, TextFont, white, 2);
	}
	//

	//Total Astig.
	val.Format(_T("%.2fD"), vals[2]);
	thisColor = Cor_Ast_Dec == 1 ? white : gray;
	top += h;
	top += h;
	btm = (top + h);
	s1.Format(_T("%.2fD"), ::NewSettings.m_IOLSel_Astig);
	s = "Total Astig. >= " + s1;
	::SetRect(&TempRect, L, top, R, btm);
	m_MemDC.WriteText(s, TempRect, TextFont, thisColor, 0);

	if (thisColor == white)
	{
		TempRect.right -= 10;
		m_MemDC.WriteText(val, TempRect, TextFont, white, 2);
	}

	thisColor = Cor_Ast_Dec == 0 ? white : gray;
	top += h;
	btm = (top + h);
	s = "Total Astig. < " + s1;
	::SetRect(&TempRect, L, top, R, btm);
	m_MemDC.WriteText(s, TempRect, TextFont, thisColor, 0);

	if (thisColor == white)
	{
		TempRect.right -= 10;
		m_MemDC.WriteText(val, TempRect, TextFont, white, 2);
	}

	//Spherical Aberration
	val.Format(_T("%.2f"), vals[3]);
	top += h;
	top += h;
	btm = (top + h);
	s = "Corneal Spherical Aberration(SA) at 6.00mm:";
	::SetRect(&TempRect, L, top, R, btm);
	m_MemDC.WriteText(s, TempRect, TextFont, white, 0);

	thisColor = Cor_SphAb_Dec == 0 ? white : gray;
	top += h;
	btm = (top + h);
	s1.Format(_T("%.2f"), ::NewSettings.m_IOLSel_Sph_Abbe[0]);
	s2.Format(_T("%.2f"), ::NewSettings.m_IOLSel_Sph_Abbe[1]);
	s3.Format(_T("%.2f"), ::NewSettings.m_IOLSel_Sph_Abbe[2]);
	s4.Format(_T("%.2f"), ::NewSettings.m_IOLSel_Sph_Abbe[3]);
	s = "<= " + s1;
	::SetRect(&TempRect, L, top, R, btm);
	m_MemDC.WriteText(s, TempRect, TextFont, thisColor, 0);

	if (thisColor == white)
	{
		TempRect.right -= 10;
		m_MemDC.WriteText(val, TempRect, TextFont, white, 2);
	}

	thisColor = Cor_SphAb_Dec == 1 ? white : gray;
	top += h;
	btm = (top + h);
	s = s1 + " - " + s2;
	::SetRect(&TempRect, L, top, R, btm);
	m_MemDC.WriteText(s, TempRect, TextFont, thisColor, 0);

	if (thisColor == white)
	{
		TempRect.right -= 10;
		m_MemDC.WriteText(val, TempRect, TextFont, white, 2);
	}

	thisColor = Cor_SphAb_Dec == 2 ? white : gray;
	top += h;
	btm = (top + h);
	s = s2 + " - " + s3;
	::SetRect(&TempRect, L, top, R, btm);
	m_MemDC.WriteText(s, TempRect, TextFont, thisColor, 0);

	if (thisColor == white)
	{
		TempRect.right -= 10;
		m_MemDC.WriteText(val, TempRect, TextFont, white, 2);
	}

	thisColor = Cor_SphAb_Dec == 3 ? white : gray;
	top += h;
	btm = (top + h);
	s = s3 + " - " + s4;
	::SetRect(&TempRect, L, top, R, btm);
	m_MemDC.WriteText(s, TempRect, TextFont, thisColor, 0);

	if (thisColor == white)
	{
		TempRect.right -= 10;
		m_MemDC.WriteText(val, TempRect, TextFont, white, 2);
	}

	thisColor = Cor_SphAb_Dec == 4 ? white : gray;
	top += h;
	btm = (top + h);
	s = "> " + s4;
	::SetRect(&TempRect, L, top, R, btm);
	m_MemDC.WriteText(s, TempRect, TextFont, thisColor, 0);

	if (thisColor == white)
	{
		TempRect.right -= 10;
		m_MemDC.WriteText(val, TempRect, TextFont, white, 2);
	}
	//
}
//530
//******************************************************************************************
//530 for wfct iol consideration box
void CSumWnd::WFCTIolCon(RECT Rect, CString Con_Res1, CString Con_Res2, CString Con_Res3, CString Con_Res4)
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);

	if (Con_Res1 == "") return;

	int L = Rect.left + 5, T = Rect.top + 5, R = Rect.right - 5, B = Rect.bottom;

	int w = Rect.right - Rect.left;
	int h = Rect.bottom - Rect.top;

	real_t line_h = real_t(B - T) / 8.0;
	real_t line_half_h = line_h / 2;

	int FontSize = intRound(line_h);
	CMFont Font1(FontSize, 400, "Arial");
	CMFont Font(FontSize, 400, "Arial");
	RECT TempRect;

	int len1 = Con_Res1.GetLength();
	int len2 = Con_Res2.GetLength();
	int len3 = Con_Res3.GetLength();
	int len4 = Con_Res4.GetLength();

	CString meaString;

	if (len1 > len2 && len1 > len3 && len1 > len4)  meaString = Con_Res1;
	if (len2 > len1 && len2 > len3 && len2 > len4)  meaString = Con_Res2;
	if (len3 > len1 && len3 > len2 && len3 > len4)  meaString = Con_Res3;
	if (len4 > len1 && len4 > len2 && len4 > len3)  meaString = Con_Res4;

	while (1)
	{
		m_MemDC.MeasureRect(meaString + "  ", TempRect, Font);

		int thisw = TempRect.right - TempRect.left;

		if (thisw <= w) break;

		else
		{
			FontSize = intRound(0.9 * real_t(FontSize));
			Font.Create(FontSize, 400, "Arial");
		}
	}

	int top = T;
	int btm = top + intRound(line_h);
	::SetRect(&TempRect, L, top, R, btm);
	m_MemDC.WriteText("Lenses to Consider :", TempRect, Font1, white, 0);

	top = btm + intRound(line_half_h);
	btm = top + intRound(line_h);
	::SetRect(&TempRect, L, top, R, btm);
	m_MemDC.WriteText(Con_Res1, TempRect, Font, white, 0);

	top = btm + intRound(line_half_h);
	btm = top + intRound(line_h);
	::SetRect(&TempRect, L, top, R, btm);
	m_MemDC.WriteText(Con_Res2, TempRect, Font, white, 0);

	top = btm + intRound(line_half_h);
	btm = top + intRound(line_h);
	::SetRect(&TempRect, L, top, R, btm);
	m_MemDC.WriteText(Con_Res3, TempRect, Font, white, 0);

	top = btm + intRound(line_half_h);
	btm = top + intRound(line_h);
	::SetRect(&TempRect, L, top, R, btm);
	m_MemDC.WriteText(Con_Res4, TempRect, Font, white, 0);

}

//******************************************************************************************

//530 Corneal Spherical Aberration Function Display
void CSumWnd::CSADisplay(real_t psSize, CCTExam* pExam, RECT Rect)//psSize : Pupil or Scan Size
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);

	real_t r_max_um = pExam->m_r_max_um;

	RECT TempRect, textRect;
	CString s;

	int L = Rect.left + 3, T = Rect.top + 3, R = Rect.right - 3, B = Rect.bottom;

	int w = Rect.right - Rect.left;
	int h = Rect.bottom - Rect.top;

	real_t font_h = (B - T) / (10);

	CMFont Font(intRound(0.9 * font_h), 400, "Arial");
	CMFont SmallFont(intRound(0.7 * font_h), 400, "Arial");

	//Title 
	real_t t = T;
	::SetRect(&TempRect, L, intRound(t), R, intRound(t + font_h));
	s = "Corneal Spherical Aberration Function";

	m_MemDC.WriteText(s, TempRect, Font, white, 0);
	//Done

	if (r_max_um == 0)
	{
		CMFont NoFont(intRound(1.5 * font_h), 400, "Arial");
		s = "No scan data available.";
		m_MemDC.WriteText(s, Rect, NoFont, white, 1);
		return;
	}

	//Funtion chart range
	T = intRound(t + font_h + 0.08 * real_t(h));
	B = Rect.bottom - intRound(0.12 * real_t(h));
	L = Rect.left + intRound(0.1 * real_t(w));
	R = Rect.right - intRound(0.08 * real_t(w));

	::SetRect(&TempRect, L, T, R, B);
	m_MemDC.DrawRectangle(TempRect, white, NOCOLOR);

	w = TempRect.right - TempRect.left;
	h = TempRect.bottom - TempRect.top;

	real_t step_h = real_t(h) / 10.0;

	real_t   maxr = 0;
	real_t  mindis = 100000;

	for (int i = 0; i <= 12; i++)
	{
		real_t testmax = i * 500;

		real_t dis = (testmax - r_max_um);
		if (dis < 0) dis = 100000;

		if (dis < mindis)
		{
			mindis = dis;
			maxr = testmax;
		}
	}

	int  ewInt = (int)(maxr / 500) - 1;
	int  valPn = intRound((maxr - 500) / 100) + 1;
	int  existPn = valPn - (int)((maxr - r_max_um) / 100.0);

	int step_Width_w = intRound(real_t(w) / ewInt);//5
	int step_H_W_w = intRound(real_t(step_Width_w) / 2);
	int step_H_W_h = intRound(real_t(step_h) / 2);

	int  step_Cal_w = intRound(real_t(w) / real_t(valPn + 1));//26.0

														  //Cal Corneal Spherical Aberration  
														  /* CZernikeMask Mask;
														  Mask.SetType(MASK_SPHERICAL);*/

	real_t *sph = new real_t[valPn];// = new ;
	int n = 0;
	real_t min = 100, max = 0;
	for (int r = intRound(maxr); r >= 500; r -= 100)
	{
		CZernikeSurface WfSurface = pExam->m_WfSurface;

		//WfSurface &= Mask;

		if (r < pExam->m_WfSurface.m_r_max_um)
		{
			WfSurface.ChangeRMaxUm(r);
		}
		else WfSurface.ChangeRMaxUm(r_max_um);

		sph[n] = WfSurface.GetCUm(12);

		if (sph[n] < min) min = sph[n];
		if (sph[n] > max) max = sph[n];

		n++;
	}

	if (min > 0) min = 0;
	//Done

	//Draw assistant line
	int Left = L;
	int Right = Left + step_H_W_w;
	for (int i = 0; i <= ewInt; i++)
	{
		int x = TempRect.left + i*step_Width_w;

		if (i >= 1 && i <= ewInt - 1)
		{
			m_MemDC.DrawLine(x, TempRect.top, x, TempRect.bottom, 1, gray);
		}

		if (i == 0)
		{
			s = "1.00mm";
		}
		else
		{
			Left = Right;
			Right = Left + step_Width_w;

			if (i == 1)
				s = "2.00mm";
			else if (i == 2)
				s = "3.00mm";
			else if (i == 3)
				s = "4.00mm";
			else if (i == 4)
				s = "5.00mm";
			else if (i == 5)
				s = "6.00mm";
			else if (i == 6)
				s = "7.00mm";
			else if (i == 7)
				s = "8.00mm";
			else if (i == 8)
				s = "9.00mm";
			else if (i == 9)
				s = "10.00mm";
		}
		::SetRect(&textRect, Left, B, Right, Rect.bottom);

		if (i == 0) m_MemDC.WriteText(s, textRect, SmallFont, white, 0, NOCOLOR);
		else       m_MemDC.WriteText(s, textRect, SmallFont, white, 1, NOCOLOR);
	}

	//
	real_t step_h_v = (max - min) / 10;
	for (int i = 0; i <= 10; i++)
	{
		int y = TempRect.bottom - intRound(i*step_h);

		if (i != 0 && i != 10) m_MemDC.DrawLine(TempRect.left, y, TempRect.right, y, 1, gray);

		real_t val = min + i*step_h_v;

		s.Format(_T("%.2f"), val);

		::SetRect(&textRect, Rect.left + 3, y - step_H_W_h, L - 3, y + step_H_W_h);

		m_MemDC.WriteText(s, textRect, SmallFont, white, 2, NOCOLOR);
	}
	//Done

	//Draw one lines (Pupil/scan size)
	int psX = 0;
	{
		real_t ratio = (psSize - 1) / (real_t)ewInt;
		psX = TempRect.left + intRound(ratio*real_t(w));
		m_MemDC.DrawLine(psX, TempRect.top, psX, TempRect.bottom, 1, RED);
	}
	//Done

	//Draw function line
	COLORREF yellow = m_Printing ? ORANGE : YELLOW;//601

	int minPsX = 1000;
	int psY = 0;
	int LastX = -1, LastY = -1;
	for (int i = 0; i <= existPn; i++)
	{
		int x = TempRect.left + step_Cal_w*i;
		int y = TempRect.bottom - intRound(h*(sph[valPn - i] - min) / (max - min));

		int dis = abs(x - psX);

		if (dis < minPsX)
		{
			minPsX = dis;
			psY = y;
		}

		//if(i == valPn - 1) x = TempRect.right - 1;

		if (LastX != -1 && LastY != -1)
		{
			m_MemDC.DrawLine(LastX, LastY, x, y, 2, yellow);
		}

		//m_MemDC.DrawCircle(x,y,2,2,RED,RED);

		LastX = x;
		LastY = y;
	}
	//Done

	//Draw one lines (horizontal line for pupil/scan value)
	m_MemDC.DrawLine(TempRect.left, psY, TempRect.right, psY, 1, RED);
	//Done

	delete sph;
}
//530

//******************************************************************************************
// To show simplied wf info for Depth of Focus Comparison
void CSumWnd::WFExamSmallInfo(CWFExam* pExam, RECT Rect, int type, int ColorType)
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;
	COLORREF EysColor = (ColorType == 0 ? BLUE : ColorType == 1 ? MAGENTA : m_Printing ? BLACK : WHITE);

	m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);

	CString s, s1, s2, s3;

	int L = Rect.left + 3, T = Rect.top + 3, R = Rect.right - 3, B = Rect.bottom;

	real_t t = T;
	real_t h = (Rect.bottom - Rect.top) / 8;

	//CMFont Font(intRound(0.9 * h), 400, "Arial");
	CMFont Font(intRound(0.8 * 1.1 * h), 400, "Arial");
	CMFont SmallFont(intRound(0.7 * 1.1 * h), 400, "Arial");
	CMFont VerySmallFont(intRound(0.5 * 1.1 * h), 400, "Arial");
	CMFont BigFont(intRound(1.5 * 1.1 * h), 400, "Arial");
	//CMFont BoldFont(intRound(1.1 * h), FW_BOLD, "Arial");//[5.1.1]

	// date / time, od/os
	if (type == 1)
	{
		::SetRect(&Rect, L, intRound(t + 5), R - 25, intRound(t + h + 10));
		s = pExam->m_Header.Eye(); s += " ";
		m_MemDC.WriteText(s, Rect, BigFont, pExam->m_Header.IsEyeManuallyOverriden() ? BLUE : EysColor, 2);
		s.Format(_T("  %s  %02i:%02i:%02i"),
			::DateToStr(pExam->m_Header.m_Year, pExam->m_Header.m_Month, pExam->m_Header.m_Day), pExam->m_Header.m_Hour, pExam->m_Header.m_Minute, pExam->m_Header.m_Second);
		m_MemDC.WriteText(s, Rect, Font, white, 0);
	}


	// clinic / physician / operator
	int r1 = intRound(real_t(R - L - 10) / 3.0);
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), L + r1, intRound(t + h + 1));
		CClinic* pClinic = ::DB.GetClinic(pExam->m_Header.m_ClinicID);
		if (pClinic)
		{
			//m_MemDC.WriteText(" Clinic", Rect, SmallFont, white, 0);
			m_MemDC.WriteText("  " + G_ReadChinese(pClinic->m_Name), Rect, VerySmallFont, white, 0);
		}

		::SetRect(&Rect, L + r1, intRound(t), L + r1 + r1, intRound(t + h + 1));
		CPhysician* pPhysician = ::DB.GetPhysician(pExam->m_Header.m_PhysicianID);
		if (pPhysician)
		{
			//m_MemDC.WriteText(" Physician", Rect, SmallFont, white, 0);
			m_MemDC.WriteText(G_ReadChinese(pPhysician->FullName()) + " ", Rect, VerySmallFont, white, 0);
		}

		::SetRect(&Rect, L + r1 + r1, intRound(t), L + r1 + r1 + r1, intRound(t + h + 1));

		COperator* pOperator = ::DB.GetOperator(pExam->m_Header.m_OperatorID);
		if (pOperator)
		{
			//m_MemDC.WriteText(" Operator", Rect, SmallFont, white, 0);
			m_MemDC.WriteText(G_ReadChinese(pOperator->FullName()) + " ", Rect, VerySmallFont, white, 0);
		}
	}


	// limbus / pupil / scan
	t += h;
	if (type == 1)
	{
		::SetRect(&Rect, L, intRound(t + 5), R, intRound(t + h + 10));
		/*if (pExam->m_Image.m_li_ok) {
		s1.Format(_T("  Limbus %.2f mm  /  ", pExam->m_Image.m_li_r_mean_um * 0.002);
		}
		else {
		s1 = "Limbus ---  /  ";
		}*/
		if (pExam->m_Image.m_pu_ok) {
			s2.Format(_T("Pupil %.2f mm  /  "), pExam->m_Image.m_pu_r_mean_um * 0.002);
		}
		else {
			s2 = "Pupil ---  /  ";
		}
		s3.Format(_T("Scan %.2f mm"), pExam->m_ScanDiameter * 0.001);
		s = s2 + s3;
		m_MemDC.WriteText(s, Rect, SmallFont, white, 0);
	}


	real_t SphEq, Sph, Cyl; int Axis;
	CZernikeSurface Surface;

	// Tracey refraction
	t += h;
	{
		COLORREF Color;
		if (pExam->m_Complaints.m_NightMiopia == COMP_SEVERE ||
			pExam->m_Complaints.m_NightHyperopia == COMP_SEVERE ||
			pExam->m_Complaints.m_Astigmatism == COMP_SEVERE ||
			pExam->m_Complaints.m_BlurDoubleVision == COMP_SEVERE ||
			pExam->m_Complaints.m_GlareHalo == COMP_SEVERE ||
			pExam->m_Complaints.m_MixedFocus == COMP_SEVERE) {
			Color = RED;
		}
		else if (pExam->m_Complaints.m_NightMiopia == COMP_MODERATE ||
			pExam->m_Complaints.m_NightHyperopia == COMP_MODERATE ||
			pExam->m_Complaints.m_Astigmatism == COMP_MODERATE ||
			pExam->m_Complaints.m_BlurDoubleVision == COMP_MODERATE ||
			pExam->m_Complaints.m_GlareHalo == COMP_MODERATE ||
			pExam->m_Complaints.m_MixedFocus == COMP_MODERATE) {
			Color = m_Printing ? ORANGE : YELLOW;
		}
		else {
			Color = GREEN;
		}

		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText("  Tracey Refraction", Rect, Font, Color, 0);
		//
		Surface = pExam->m_WfSurface;
		Surface.ChangeRMaxUm(2000.0);
		CWFExam::GetSpheqSphCylAxis(Surface, ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
		//
		if (::Settings.m_RefractionRounding == 0) {
			Sph = round8(Sph);
			Cyl = round8(Cyl);
		}
		s.Format(_T("%+.2f D %+.2f D x %i° "), Sph, Cyl, Axis);
		m_MemDC.WriteText(s, Rect, Font, Color, 2);
	}



	t += h;

	// rms
	m_MemDC.MeasureRect("+00.000 µ ", Rect, SmallFont);
	int w2 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("x 000° ", Rect, SmallFont);
	int w3 = Rect.right - Rect.left;
	int w0 = (R - L) - (w2 + w3);

	m_MemDC.MeasureRect("   HO Total @ D <= 0.00 mm", Rect, Font);
	int w1 = Rect.right - Rect.left;

	//
	CZernikeMask Mask;
	Mask.SetType(MASK_HO_TOTAL);
	Surface = pExam->m_WfSurface;
	Surface &= Mask;
	real_t rms = Surface.ComputeRms();
	//

	Mask.GetName(s);

	::SetRect(&Rect, L, intRound(t), L + w1, intRound(t + h + 1));
	s.Format(_T("  HO Total @ D <= %.2f mm"), 0.002 * Surface.m_r_max_um);
	m_MemDC.WriteText(s, Rect, Font, white, 0);

	::SetRect(&Rect, L + w1, intRound(t), R, intRound(t + h + 1));
	s.Format(_T("%6.3f µ "), rms);
	m_MemDC.WriteText(s, Rect, Font, white, 2);


	t += h;

	// Comment for WFCT Angle K/A
	////Angle Alpha 
	//s = "  Angle Alpha D = ";
	//m_MemDC.MeasureRect(s + "      ", Rect, Font);
	//w1 = Rect.right - Rect.left;

	//::SetRect(&Rect, L, intRound(t), L + w1, intRound(t + h + 1));
	//if (pExam->m_Header.m_AngleAlphaOK)
	//{
	//	COLORREF yellow = m_Printing ? ORANGE : YELLOW;//601

	//	if (pExam->m_Header.m_AngleAlphaRUm < 300)     m_MemDC.WriteText(s, Rect, Font, GREEN, 0);
	//	else
	//	{
	//		if (pExam->m_Header.m_AngleAlphaRUm < 500) m_MemDC.WriteText(s, Rect, Font, yellow, 0);
	//		else                                       m_MemDC.WriteText(s, Rect, Font, RED, 0);
	//	}

	//	if (pExam->m_Header.m_AngleAlphaRUm == 0)  pExam->m_Header.m_AngleAlphaADg = 0;//600
	//	s.Format(_T("%.3f mm @ %i° ", pExam->m_Header.m_AngleAlphaRUm*0.001, pExam->m_Header.m_AngleAlphaADg);
	//	::SetRect(&Rect, L + w1, intRound(t), R, intRound(t + h + 1));

	//	//531
	//	if (pExam->m_Header.m_AngleAlphaRUm < 300)     m_MemDC.WriteText(s, Rect, Font, GREEN, 2);
	//	else
	//	{
	//		if (pExam->m_Header.m_AngleAlphaRUm < 500) m_MemDC.WriteText(s, Rect, Font, yellow, 2);
	//		else                                       m_MemDC.WriteText(s, Rect, Font, RED, 2);
	//	}
	//	//531
	//}
	//else m_MemDC.WriteText(s, Rect, Font, gray, 0);

	//Done

	// notes
	int nn = 0;
	t += h;
	s = " " + G_ReadChinese(pExam->m_Header.m_Note);

	int l = s.GetLength();
	while (l > 0 && nn < 1)
	{
		int i;
		for (i = 1; i <= l; i++) {
			if (s[i - 1] == '\n') break;
			m_MemDC.MeasureRect(" " + s.Left(i) + " ", Rect, SmallFont);
			if (Rect.right - Rect.left > R - L) {
				for (; i >= 1; i--) {
					if (s[i - 1] == ' ') break;
				}
				break;
			}
		}

		t += h;
		nn++;
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText(" " + s.Left(i) + " ", Rect, SmallFont, white, 0);
		}

		s = s.Right(l - i);
		l -= i;
	}
}

//******************************************************************************************

void CSumWnd::WFExamInfo(CWFExam* pExam, RECT Rect, int NumRows, BOOL* Rows)
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);

	CString s, s1, s2, s3;

	int L = Rect.left + 3, T = Rect.top + 3, R = Rect.right - 3, B = Rect.bottom;

	real_t t = T;
	real_t h = (Rect.bottom - Rect.top - 5) / ((real_t)NumRows + 0.5);

	CMFont Font(intRound(1.1 * h), 400, "Arial");
	CMFont BigFont(intRound(1.5 * 1.1 * h), 400, "Arial");
	//CMFont BoldFont(intRound(1.1 * h), FW_BOLD, "Arial");//[5.1.1]

	int n = 0;

	// date / time, od/os
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R - 25, intRound(t + h + 1));
		s = pExam->m_Header.Eye(); s += " ";
		m_MemDC.WriteText(s, Rect, BigFont, pExam->m_Header.IsEyeManuallyOverriden() ? RED : white, 2);
		s.Format(_T(" %s  %02i:%02i:%02i"),
			::DateToStr(pExam->m_Header.m_Year, pExam->m_Header.m_Month, pExam->m_Header.m_Day), pExam->m_Header.m_Hour, pExam->m_Header.m_Minute, pExam->m_Header.m_Second);
		m_MemDC.WriteText(s, Rect, Font, white, 0);
	}

	// clinic
	t += 5;
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));

		CClinic* pClinic = ::DB.GetClinic(pExam->m_Header.m_ClinicID);
		if (pClinic) {
			m_MemDC.WriteText(" Clinic", Rect, Font, white, 0);
			m_MemDC.WriteText(G_ReadChinese(pClinic->m_Name) + " ", Rect, Font, white, 2);
		}
		else m_MemDC.WriteText(" Clinic", Rect, Font, gray, 0);
	}

	// physician
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));

		CPhysician* pPhysician = ::DB.GetPhysician(pExam->m_Header.m_PhysicianID);
		if (pPhysician) {
			m_MemDC.WriteText(" Physician", Rect, Font, white, 0);
			m_MemDC.WriteText(G_ReadChinese(pPhysician->FullName()) + " ", Rect, Font, white, 2);
		}
		else m_MemDC.WriteText(" Physician", Rect, Font, gray, 0);
	}

	// operator
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));

		COperator* pOperator = ::DB.GetOperator(pExam->m_Header.m_OperatorID);
		if (pOperator) {
			m_MemDC.WriteText(" Operator", Rect, Font, white, 0);
			m_MemDC.WriteText(G_ReadChinese(pOperator->FullName()) + " ", Rect, Font, white, 2);
		}
		else m_MemDC.WriteText(" Operator", Rect, Font, gray, 0);
	}

	// limbus / pupil / scan
	t += h;
	if (Rows[n++]) 
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));

		m_MemDC.WriteText(" Pupil / Scan", Rect, Font, white, 0);//
																
		if (pExam->m_Image.m_pu_ok) 
		{
			s2.Format(_T("%.2f"), pExam->m_Image.m_pu_r_mean_um * 0.002);
		}
		else 
		{
			s2 = "---";
		}
		s3.Format(_T("%.2f"), pExam->m_ScanDiameter * 0.001);

		s = s2 + " / " + s3 + " mm ";
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	// target
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Fixation Target Position", Rect, Font, white, 0);
		real_t dpt = pExam->m_AccommTargetPosition;
		s.Format(_T("%.2f D "), fabs(dpt));
		if (dpt < -0.001) s = "- " + s; else if (dpt > 0.001) s = "+ " + s;
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	real_t SphEq, Sph, Cyl; int Axis;
	CZernikeSurface Surface;

	// Tracey refraction
	t += h;
	if (Rows[n++])
	{
		COLORREF Color;
		if (pExam->m_Complaints.m_NightMiopia == COMP_SEVERE ||
			pExam->m_Complaints.m_NightHyperopia == COMP_SEVERE ||
			pExam->m_Complaints.m_Astigmatism == COMP_SEVERE ||
			pExam->m_Complaints.m_BlurDoubleVision == COMP_SEVERE ||
			pExam->m_Complaints.m_GlareHalo == COMP_SEVERE ||
			pExam->m_Complaints.m_MixedFocus == COMP_SEVERE) {
			Color = RED;
		}
		else if (pExam->m_Complaints.m_NightMiopia == COMP_MODERATE ||
			pExam->m_Complaints.m_NightHyperopia == COMP_MODERATE ||
			pExam->m_Complaints.m_Astigmatism == COMP_MODERATE ||
			pExam->m_Complaints.m_BlurDoubleVision == COMP_MODERATE ||
			pExam->m_Complaints.m_GlareHalo == COMP_MODERATE ||
			pExam->m_Complaints.m_MixedFocus == COMP_MODERATE) {
			Color = m_Printing ? ORANGE : YELLOW;
		}
		else {
			Color = GREEN;
		}

		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Tracey Refraction", Rect, Font, Color, 0);
		//
		Surface = pExam->m_WfSurface;
		Surface.ChangeRMaxUm(2000.0);
		CWFExam::GetSpheqSphCylAxis(Surface, ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
		//
		if (::Settings.m_RefractionRounding == 0) {
			Sph = round8(Sph);
			Cyl = round8(Cyl);
		}
		s.Format(_T("%+.2f D %+.2f D x %i° "), Sph, Cyl, Axis);
		m_MemDC.WriteText(s, Rect, Font, Color, 2);
	}

	// refraction
	m_MemDC.MeasureRect("+00.00 D ", Rect, Font);
	int w1 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("+00.00 D x ", Rect, Font);
	int w2 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("000° ", Rect, Font);
	int w3 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("@ D <= %.2f mm ", Rect, Font);
	int w4 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("VD = %.2f mm ", Rect, Font);
	int w5 = Rect.right - Rect.left;
	for (int i = 0; i < 4; i++)
	{
		t += h;
		if (Rows[n++])
		{
			//
			Surface = pExam->m_WfSurface;
			//
			real_t r_max_um = i < 3 ? 0.5 * ::Settings.m_ZoneDiameterUm[i] : Surface.m_r_max_um;
			if (r_max_um <= Surface.m_r_max_um)
			{
				//
				Surface.ChangeRMaxUm(r_max_um);
				CWFExam::GetSpheqSphCylAxis(Surface, ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
				// sphere
				::SetRect(&Rect, L, intRound(t), L + w1, intRound(t + h + 1));
				s.Format(_T("%+.2f D "), Sph);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
				// cylinder
				::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
				s.Format(_T("%+.2f D x "), Cyl);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
				// axis
				::SetRect(&Rect, L + w1 + w2, intRound(t), L + w1 + w2 + w3, intRound(t + h + 1));
				s.Format(_T("%i° "), Axis);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
				// zone size
				::SetRect(&Rect, L + w1 + w2 + w3, intRound(t), L + w1 + w2 + w3 + w4, intRound(t + h + 1));
				s.Format(_T("@ D <= %.2f mm "), 0.002 * Surface.m_r_max_um);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
				// vertex
				::SetRect(&Rect, L + w1 + w2 + w3 + w4, intRound(t), R, intRound(t + h + 1));
				s.Format(_T("VD = %.2f mm "), 0.001 * ::Settings.m_VertexDistanceUm);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
			else
			{
				// zone size
				::SetRect(&Rect, L + w1 + w2 + w3, intRound(t), L + w1 + w2 + w3 + w4, intRound(t + h + 1));
				s.Format(_T("@ D <= %.2f mm "), 0.001 * ::Settings.m_ZoneDiameterUm[i]);
				m_MemDC.WriteText(s, Rect, Font, gray, 2);
				// vertex
				::SetRect(&Rect, L + w1 + w2 + w3 + w4, intRound(t), R, intRound(t + h + 1));
				s.Format(_T("VD = %.2f mm "), 0.001 * ::Settings.m_VertexDistanceUm);
				m_MemDC.WriteText(s, Rect, Font, gray, 2);
			}
		}
	}

	// rms
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		Surface = pExam->m_WfSurface;
		s.Format(_T(" Root Mean Square @ D <= %.2f mm"), 0.002 * Surface.m_r_max_um);
		m_MemDC.WriteText(s, Rect, Font, white, 0);
	}

	// rms
	m_MemDC.MeasureRect("+00.000 µ ", Rect, Font);
	w2 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("x 000° ", Rect, Font);
	w3 = Rect.right - Rect.left;
	w1 = (R - L) - (w2 + w3);
	int MaskType[9] = { MASK_TOTAL, MASK_LO_TOTAL, MASK_DEFOCUS, MASK_ASTIGMATISM, MASK_HO_TOTAL, MASK_COMA, MASK_SPHERICAL, MASK_4ORD_ASTIGMATISM, MASK_TREFOIL };
	for (int i = 0; i < 9; i++)
	{
		t += h;
		if (Rows[n++]) {
			//
			CZernikeMask Mask;
			Mask.SetType(MaskType[i]);
			Surface = pExam->m_WfSurface;
			Surface &= Mask;
			real_t rms = Surface.ComputeRms();
			//
			::SetRect(&Rect, L, intRound(t), L + w1, intRound(t + h + 1));
			Mask.GetName(s);

			m_MemDC.WriteText("     " + s, Rect, Font, white, 0);

			// microns
			::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
			s.Format(_T("%6.3f µ "), rms);
			if (MaskType[i] == MASK_DEFOCUS) {
				if (Surface.m_c[4] > 0.0) s = "+" + s;
				else if (Surface.m_c[4] < 0.0) s = "-" + s;
			}
			else if (MaskType[i] == MASK_SPHERICAL) {
				if (Surface.m_c[12] > 0.0) s = "+" + s;
				else if (Surface.m_c[12] < 0.0) s = "-" + s;
			}

			if (i != 4)//[5.1.1]
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			else
			{
				CMDC DC;
				DC.CreateCompatibleDC(NULL);
				DC.MeasureRect("     ", Rect, Font);
				int cw = Rect.right - Rect.left - 3;
				::SetRect(&Rect, L + cw, intRound(t), L + w1 + w2, intRound(t + h + 1));//[5.1.1]
				m_MemDC.DrawRectangle(Rect, white, NOCOLOR);//[5.1.1]
				m_MemDC.WriteText(s, Rect, Font, white, 2);//[5.1.1]
			}


			// angle	  
			::SetRect(&Rect, L + w1 + w2, intRound(t), R, intRound(t + h + 1));

			if (MaskType[i] == MASK_ASTIGMATISM) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[3], Surface.m_c[5]) * _180_Pi / 2.0) % 180);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
			else if (MaskType[i] == MASK_COMA) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[7], Surface.m_c[8]) * _180_Pi) % 360);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
			else if (MaskType[i] == MASK_TREFOIL) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[6], Surface.m_c[9]) * _180_Pi / 3.0) % 120);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
			else if (MaskType[i] == MASK_4ORD_ASTIGMATISM) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[11], Surface.m_c[13]) * _180_Pi / 2.0) % 180);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
		}
	}

	// notes
	s = G_ReadChinese(pExam->m_Header.m_Note) + G_ReadChinese(pExam->m_Header.m_AutoNote);

	int l = s.GetLength();

	while (l > 0 && n < NumRows)
	{
		int i;
		for (i = 1; i <= l; i++) {
			if (s[i - 1] == '\n') break;
			m_MemDC.MeasureRect(" " + s.Left(i) + " ", Rect, Font);
			if (Rect.right - Rect.left > R - L) {
				for (; i >= 1; i--) {
					if (s[i - 1] == ' ') break;
				}
				break;
			}
		}

		t += h;
		if (Rows[n++]) {
			// WFCT Angle k/a 
			int count = s.Left(i).Find(_T("Alpha"), 0);
			int count2 = s.Left(i).Find(_T("Kappa"), 0);

			if (count != -1 || count2 != -1)
			{
			}
			else // WFCT Angle k/a 
			{
				::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
				m_MemDC.WriteText(" " + s.Left(i) + " ", Rect, Font, white, 0);
			}
		}

		s = s.Right(l - i);
		l -= i;
	}
}


//******************************************************************************************
//6.3.0 For WTT adjusted GUI
//test degree error ° 2
void CSumWnd::WFWTTExamInfo(CWFExam* pExam, int ExamOrder, RECT Rect, int NumRows, BOOL* Rows)
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);

	CString s, s1, s2, s3;

	int L = Rect.left + 3, T = Rect.top + 3, R = Rect.right - 3, B = Rect.bottom;

	real_t t = T;
	real_t h = (Rect.bottom - Rect.top - 5) / ((real_t)NumRows + 0.5);

	CMFont Font(intRound(1.1 * h), 400, "Arial");
	CMFont BigFont(intRound(1.5 * 1.1 * h), 400, "Arial");
	//CMFont BoldFont(intRound(1.1 * h), FW_BOLD, "Arial");//[5.1.1]

	int n = 0;

	// date / time, od/os
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R - 25, intRound(t + h + 1));
		s = pExam->m_Header.Eye(); s += " ";
		m_MemDC.WriteText(s, Rect, BigFont, pExam->m_Header.IsEyeManuallyOverriden() ? RED : white, 2);
		//03292019 add exam order
		s.Format(_T(" %s  %02i:%02i:%02i    Exam Order: %04i"),
			::DateToStr(pExam->m_Header.m_Year, pExam->m_Header.m_Month, pExam->m_Header.m_Day), pExam->m_Header.m_Hour, pExam->m_Header.m_Minute, pExam->m_Header.m_Second, ExamOrder);
		//03292019 add exam order
		m_MemDC.WriteText(s, Rect, Font, white, 0);
	}

	// clinic
	t += 5;
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));

		CClinic* pClinic = ::DB.GetClinic(pExam->m_Header.m_ClinicID);
		if (pClinic) {
			m_MemDC.WriteText(" Clinic", Rect, Font, white, 0);
			m_MemDC.WriteText(G_ReadChinese(pClinic->m_Name) + " ", Rect, Font, white, 2);
		}
		else m_MemDC.WriteText(" Clinic", Rect, Font, gray, 0);
	}

	// physician
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));

		CPhysician* pPhysician = ::DB.GetPhysician(pExam->m_Header.m_PhysicianID);
		if (pPhysician) {
			m_MemDC.WriteText(" Physician", Rect, Font, white, 0);
			m_MemDC.WriteText(G_ReadChinese(pPhysician->FullName()) + " ", Rect, Font, white, 2);
		}
		else m_MemDC.WriteText(" Physician", Rect, Font, gray, 0);
	}

	// operator
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));

		COperator* pOperator = ::DB.GetOperator(pExam->m_Header.m_OperatorID);
		if (pOperator) {
			m_MemDC.WriteText(" Operator", Rect, Font, white, 0);
			m_MemDC.WriteText(G_ReadChinese(pOperator->FullName()) + " ", Rect, Font, white, 2);
		}
		else m_MemDC.WriteText(" Operator", Rect, Font, gray, 0);
	}

	// limbus / pupil / scan
	t += h;
	if (Rows[n++])
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));

		m_MemDC.WriteText(" Pupil / Scan", Rect, Font, white, 0);//

		if (pExam->m_Image.m_pu_ok)
		{
			s2.Format(_T("%.2f"), pExam->m_Image.m_pu_r_mean_um * 0.002);
		}
		else
		{
			s2 = "---";
		}
		s3.Format(_T("%.2f"), pExam->m_ScanDiameter * 0.001);

		s = s2 + " / " + s3 + " mm ";
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	// target
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Fixation Target Position", Rect, Font, white, 0);
		real_t dpt = pExam->m_AccommTargetPosition;
		s.Format(_T("%.2f D "), fabs(dpt));
		if (dpt < -0.001) s = "- " + s; else if (dpt > 0.001) s = "+ " + s;
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	real_t SphEq, Sph, Cyl; int Axis;
	CZernikeSurface Surface;

	// Tracey refraction
	t += h;
	if (Rows[n++])
	{
		COLORREF Color;
		if (pExam->m_Complaints.m_NightMiopia == COMP_SEVERE ||
			pExam->m_Complaints.m_NightHyperopia == COMP_SEVERE ||
			pExam->m_Complaints.m_Astigmatism == COMP_SEVERE ||
			pExam->m_Complaints.m_BlurDoubleVision == COMP_SEVERE ||
			pExam->m_Complaints.m_GlareHalo == COMP_SEVERE ||
			pExam->m_Complaints.m_MixedFocus == COMP_SEVERE) {
			Color = RED;
		}
		else if (pExam->m_Complaints.m_NightMiopia == COMP_MODERATE ||
			pExam->m_Complaints.m_NightHyperopia == COMP_MODERATE ||
			pExam->m_Complaints.m_Astigmatism == COMP_MODERATE ||
			pExam->m_Complaints.m_BlurDoubleVision == COMP_MODERATE ||
			pExam->m_Complaints.m_GlareHalo == COMP_MODERATE ||
			pExam->m_Complaints.m_MixedFocus == COMP_MODERATE) {
			Color = m_Printing ? ORANGE : YELLOW;
		}
		else {
			Color = GREEN;
		}

		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Tracey Refraction", Rect, Font, Color, 0);
		//
		Surface = pExam->m_WfSurface;
		Surface.ChangeRMaxUm(2000.0);
		CWFExam::GetSpheqSphCylAxis(Surface, ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
		//
		if (::Settings.m_RefractionRounding == 0) {
			Sph = round8(Sph);
			Cyl = round8(Cyl);
		}
		s.Format(_T("%+.2f D %+.2f D x %i° "), Sph, Cyl, Axis);
		m_MemDC.WriteText(s, Rect, Font, Color, 2);
	}

	// refraction
	m_MemDC.MeasureRect("+00.00 D ", Rect, Font);
	int w1 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("+00.00 D x ", Rect, Font);
	int w2 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("000° ", Rect, Font);
	int w3 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("@ D <= %.2f mm ", Rect, Font);
	int w4 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("VD = %.2f mm ", Rect, Font);
	int w5 = Rect.right - Rect.left;
	for (int i = 0; i < 4; i++)
	{
		t += h;
		if (Rows[n++])
		{
			//
			Surface = pExam->m_WfSurface;
			//
			real_t r_max_um = i < 3 ? 0.5 * ::Settings.m_ZoneDiameterUm[i] : Surface.m_r_max_um;
			if (r_max_um <= Surface.m_r_max_um)
			{
				//
				Surface.ChangeRMaxUm(r_max_um);
				CWFExam::GetSpheqSphCylAxis(Surface, ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
				// sphere
				::SetRect(&Rect, L, intRound(t), L + w1, intRound(t + h + 1));
				s.Format(_T("%+.2f D "), Sph);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
				// cylinder
				::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
				s.Format(_T("%+.2f D x "), Cyl);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
				// axis
				::SetRect(&Rect, L + w1 + w2, intRound(t), L + w1 + w2 + w3, intRound(t + h + 1));
				s.Format(_T("%i° "), Axis);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
				// zone size
				::SetRect(&Rect, L + w1 + w2 + w3, intRound(t), L + w1 + w2 + w3 + w4, intRound(t + h + 1));
				s.Format(_T("@ D <= %.2f mm "), 0.002 * Surface.m_r_max_um);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
				// vertex
				::SetRect(&Rect, L + w1 + w2 + w3 + w4, intRound(t), R, intRound(t + h + 1));
				s.Format(_T("VD = %.2f mm "), 0.001 * ::Settings.m_VertexDistanceUm);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
			else
			{
				// zone size
				::SetRect(&Rect, L + w1 + w2 + w3, intRound(t), L + w1 + w2 + w3 + w4, intRound(t + h + 1));
				s.Format(_T("@ D <= %.2f mm "), 0.001 * ::Settings.m_ZoneDiameterUm[i]);
				m_MemDC.WriteText(s, Rect, Font, gray, 2);
				// vertex
				::SetRect(&Rect, L + w1 + w2 + w3 + w4, intRound(t), R, intRound(t + h + 1));
				s.Format(_T("VD = %.2f mm "), 0.001 * ::Settings.m_VertexDistanceUm);
				m_MemDC.WriteText(s, Rect, Font, gray, 2);
			}
		}
	}

	// rms
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		Surface = pExam->m_WfSurface;
		s.Format(_T(" Root Mean Square @ D <= %.2f mm"), 0.002 * Surface.m_r_max_um);
		m_MemDC.WriteText(s, Rect, Font, white, 0);
	}

	// rms
	m_MemDC.MeasureRect("+00.000 µ ", Rect, Font);
	w2 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("x 000° ", Rect, Font);
	w3 = Rect.right - Rect.left;
	w1 = (R - L) - (w2 + w3);
	int MaskType[9] = { MASK_TOTAL, MASK_LO_TOTAL, MASK_DEFOCUS, MASK_ASTIGMATISM, MASK_HO_TOTAL, MASK_COMA, MASK_SPHERICAL, MASK_4ORD_ASTIGMATISM, MASK_TREFOIL };
	for (int i = 0; i < 9; i++)
	{
		t += h;
		if (Rows[n++]) {
			//
			CZernikeMask Mask;
			Mask.SetType(MaskType[i]);
			Surface = pExam->m_WfSurface;
			Surface &= Mask;
			real_t rms = Surface.ComputeRms();
			//
			::SetRect(&Rect, L, intRound(t), L + w1, intRound(t + h + 1));
			Mask.GetName(s);

			m_MemDC.WriteText("     " + s, Rect, Font, white, 0);

			// microns
			::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
			s.Format(_T("%6.3f µ "), rms);
			if (MaskType[i] == MASK_DEFOCUS) {
				if (Surface.m_c[4] > 0.0) s = "+" + s;
				else if (Surface.m_c[4] < 0.0) s = "-" + s;
			}
			else if (MaskType[i] == MASK_SPHERICAL) {
				if (Surface.m_c[12] > 0.0) s = "+" + s;
				else if (Surface.m_c[12] < 0.0) s = "-" + s;
			}

			if (i != 4)//[5.1.1]
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			else
			{
				CMDC DC;
				DC.CreateCompatibleDC(NULL);
				DC.MeasureRect("     ", Rect, Font);
				int cw = Rect.right - Rect.left - 3;
				::SetRect(&Rect, L + cw, intRound(t), L + w1 + w2, intRound(t + h + 1));//[5.1.1]
				m_MemDC.DrawRectangle(Rect, white, NOCOLOR);//[5.1.1]
				m_MemDC.WriteText(s, Rect, Font, white, 2);//[5.1.1]
			}


			// angle	  
			::SetRect(&Rect, L + w1 + w2, intRound(t), R, intRound(t + h + 1));

			if (MaskType[i] == MASK_ASTIGMATISM) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[3], Surface.m_c[5]) * _180_Pi / 2.0) % 180);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
			else if (MaskType[i] == MASK_COMA) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[7], Surface.m_c[8]) * _180_Pi) % 360);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
			else if (MaskType[i] == MASK_TREFOIL) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[6], Surface.m_c[9]) * _180_Pi / 3.0) % 120);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
			else if (MaskType[i] == MASK_4ORD_ASTIGMATISM) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[11], Surface.m_c[13]) * _180_Pi / 2.0) % 180);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
		}
	}

	// notes
	s = G_ReadChinese(pExam->m_Header.m_Note) + G_ReadChinese(pExam->m_Header.m_AutoNote);

	int l = s.GetLength();

	while (l > 0 && n < NumRows)
	{
		int i;
		for (i = 1; i <= l; i++) {
			if (s[i - 1] == '\n') break;
			m_MemDC.MeasureRect(" " + s.Left(i) + " ", Rect, Font);
			if (Rect.right - Rect.left > R - L) {
				for (; i >= 1; i--) {
					if (s[i - 1] == ' ') break;
				}
				break;
			}
		}

		t += h;
		if (Rows[n++]) {
			// WFCT Angle k/a 
			int count = s.Left(i).Find(_T("Alpha"), 0);
			int count2 = s.Left(i).Find(_T("Kappa"), 0);

			if (count != -1 || count2 != -1)
			{
			}
			else // WFCT Angle k/a 
			{
				::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
				m_MemDC.WriteText(" " + s.Left(i) + " ", Rect, Font, white, 0);
			}
		}

		s = s.Right(l - i);
		l -= i;
	}
}

//******************************************************************************************

void CSumWnd::WFCTTorDataInfo(CWFExam* pWFExam, CCTExam* pCTExam, RECT Rect)
{
	RECT textRect;
	RECT testRect;
	CString s;

	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	real_t h = (Rect.bottom - Rect.top) / 17.0;

	CMFont Font(intRound(0.8 * 1.1 * h), 400, "Arial");
	CMFont BigFont(intRound(1.5 * 1.1 * h), 400, "Arial");

	int h1 = intRound((real_t)(Rect.bottom - Rect.top) / 6.0);
	int sBottom = Rect.top + h1;

	int lTop = sBottom + intRound(m_g);

	//Small Rect
	RECT smallRect;
	::SetRect(&smallRect, Rect.left, Rect.top, Rect.right, sBottom);

	m_MemDC.DrawRectangle(smallRect, gray, NOCOLOR);

	m_MemDC.MeasureRect("OS ", testRect, BigFont);

	int L = smallRect.left + 10, T = smallRect.top + 15, R = smallRect.left + 3 + testRect.right - testRect.left;
	int textH = intRound((real_t)(smallRect.bottom - smallRect.top) / 1.5);

	s = pWFExam->m_Header.Eye();
	::SetRect(&textRect, L, T, R, T + textH);
	m_MemDC.WriteText(s, textRect, BigFont, pWFExam->m_Header.IsEyeManuallyOverriden() ? RED : white, 0);

	L = R + 3;
	R = smallRect.right - 10;
	::SetRect(&textRect, L, T, R, T + textH);
	m_MemDC.WriteText("WF", textRect, Font, white, 0);

	s.Format(_T("  %s  %02i:%02i:%02i"),
		::DateToStr(pWFExam->m_Header.m_Year, pWFExam->m_Header.m_Month, pWFExam->m_Header.m_Day), pWFExam->m_Header.m_Hour, pWFExam->m_Header.m_Minute, pWFExam->m_Header.m_Second);
	m_MemDC.WriteText(s, textRect, Font, white, 2);
	//Small Rect Done


	//Large Rect
	//Calculate values
	/*
	1. Current Corneal Power & Axis
	2. Estimated TORIC Lens Power & Axis
	3. Current Entire Eye Astigmatism
	4. Est. POST ROTATION Entire Eye Astig.
	*/
	CString s1, s2, s3, s4, s5;
	COpData OpData = pCTExam->m_OpData;

	//1. Current Corneal Power & Axis
	OpData.m_OpType = 1;
	OpData.Recalculate();

	if (OpData.m_CorneaPreopCyl != INVALID_VALUE) {
		s1.Format(_T("%.2f D x "), OpData.m_CorneaPreopCyl);
	}
	if (OpData.m_CorneaPreopAxis != INVALID_VALUE) {
		s2.Format(_T("%i° "), OpData.m_CorneaPreopAxis);
	}
	s1 += s2;
	//1. Current Corneal Power & Axis Done

	//2. Estimated TORIC Lens Power & Axis
	if (OpData.m_InternalPreopCyl != INVALID_VALUE) {
		s2.Format(_T("%.2f D x "), OpData.m_InternalPreopCyl);
	}
	if (OpData.m_InternalPreopAxis != INVALID_VALUE) {
		int axis = (OpData.m_InternalPreopAxis + 90);
		axis = axis % 180;
		s3.Format(_T("%i° "), axis);
	}
	s2 += s3;
	//2. Estimated TORIC Lens Power & Axis Done

	//3. Current Entire Eye Astigmatism
	if (OpData.m_EyePreopCyl != INVALID_VALUE) {
		s3.Format(_T("%.2f D x "), OpData.m_EyePreopCyl);
	}
	if (OpData.m_EyePreopAxis != INVALID_VALUE) {
		s4.Format(_T("%i° "), OpData.m_EyePreopAxis);
	}
	s3 += s4;
	//3. Current Entire Eye Astigmatism Done

	//4. Est.POST ROTATION Entire Eye Astig.
	if (OpData.m_EyePostopCyl != INVALID_VALUE) {
		s4.Format(_T("%.2f D x "), OpData.m_EyePostopCyl);
	}
	if (OpData.m_EyePostopAxis != INVALID_VALUE) {
		s5.Format(_T("%i° "), OpData.m_EyePostopAxis);
	}
	s4 += s5;
	//4. Est.POST ROTATION Entire Eye Astig. Done


	//Calculate values Done
	RECT LargeRect;
	::SetRect(&LargeRect, Rect.left, lTop, Rect.right, Rect.bottom);
	m_MemDC.DrawRectangle(LargeRect, white, NOCOLOR);

	textH = intRound(1.5 * 1.2 * h);
	s = "Toric Data";
	L = LargeRect.left + 10,
		T = LargeRect.top + 5;
	int B = T + textH;
	::SetRect(&textRect, L, T, R, B);
	m_MemDC.WriteText(s, textRect, BigFont, white, 0);

	textH = intRound((real_t)(LargeRect.bottom - LargeRect.top) / 10.0);

	T = B + textH;
	B = T + textH;

	::SetRect(&textRect, L, T, R, B);
	m_MemDC.WriteText("Current Corneal Power && Axis", textRect, Font, white, 0);
	m_MemDC.WriteText(s1, textRect, Font, white, 2);


	T = B;
	B = T + textH;

	::SetRect(&textRect, L, T, R, B);
	m_MemDC.WriteText("TORIC lens Power && Axis*", textRect, Font, white, 0);
	m_MemDC.WriteText(s2, textRect, Font, white, 2);

	T = B;
	B = T + textH;

	::SetRect(&textRect, L, T, R, B);
	m_MemDC.WriteText("Current Entire Eye Astigmatism", textRect, Font, white, 0);
	m_MemDC.WriteText(s3, textRect, Font, white, 2);

	T = B + textH;
	B = T + textH;

	::SetRect(&textRect, L, T, R, B);
	m_MemDC.WriteText("POST ROTATION Entire Eye Astigmatism*", textRect, Font, white, 0);
	m_MemDC.WriteText(s4, textRect, Font, white, 2);

	T = B + textH;
	B = T + textH;

	::SetRect(&textRect, L, T, R, B);
	m_MemDC.WriteText("*estimated", textRect, Font, white, 0);
	//Large Rect Done
}

//******************************************************************************************

void CSumWnd::WFCTTorRefInfo(CWFExam* pWFExam, CCTExam* pCTExam, RECT Rect)
{
	RECT textRect;
	RECT testRect;
	CString s;

	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;
	COLORREF yellow = m_Printing ? ORANGE : YELLOW;

	real_t h = (Rect.bottom - Rect.top) / 17.0;

	CMFont Font(intRound(0.8 * 1.1 * h), 400, "Arial");
	CMFont BigFont(intRound(1.5 * 1.1 * h), 400, "Arial");
	CMFont Font2(intRound(1.1 * h), 400, "Arial");
	CMFont LargeFont(intRound(2.8 * h), 400, "Arial");

	int h1 = intRound((real_t)(Rect.bottom - Rect.top) / 6.0);
	int sBottom = Rect.top + h1;

	int lTop = sBottom + intRound(m_g);
	int lRight = Rect.left + intRound((real_t)(Rect.right - Rect.left - m_g) / 2.0);

	//Small Rect
	RECT smallRect;
	::SetRect(&smallRect, Rect.left, Rect.top, lRight, sBottom);

	m_MemDC.MeasureRect("OS ", testRect, BigFont);

	int L = smallRect.left + 10, T = smallRect.top + 15, R = smallRect.left + 3 + testRect.right - testRect.left;
	int textH = intRound((real_t)(smallRect.bottom - smallRect.top) / 1.5);

	s = pCTExam->m_Header.Eye();
	::SetRect(&textRect, L, T, R, T + textH - 10);
	m_MemDC.WriteText(s, textRect, BigFont, pCTExam->m_Header.IsEyeManuallyOverriden() ? RED : white, 0);

	L = R + 3;
	R = smallRect.right - 10;
	::SetRect(&textRect, L, T, R, T + textH);
	m_MemDC.WriteText("CT", textRect, Font, white, 0);

	s.Format(_T("  %s  %02i:%02i:%02i"),
		::DateToStr(pCTExam->m_Header.m_Year, pCTExam->m_Header.m_Month, pCTExam->m_Header.m_Day), pCTExam->m_Header.m_Hour, pCTExam->m_Header.m_Minute, pCTExam->m_Header.m_Second);
	m_MemDC.WriteText(s, textRect, Font, white, 2);
	m_MemDC.DrawRectangle(smallRect, gray, NOCOLOR);
	//Small Rect Done


	//Large Rect
	COLORREF RefColor;
	RECT LargeRect;
	::SetRect(&LargeRect, Rect.left, lTop, Rect.right, Rect.bottom);
	m_MemDC.DrawRectangle(LargeRect, white, NOCOLOR);

	CString s1, s2;
	real_t Spre, Apre;
	CalTraceyRef(pWFExam, RefColor, Spre, Apre, s);//Calculate the Tracey Refraction
	CalIolEnh(pCTExam, s1, s2);//Calculate the IOL enhancement

	textH = intRound(1.1 * h);
	L = LargeRect.left;
	T = LargeRect.top + 10;
	R = LargeRect.right;
	int B = T + textH;
	::SetRect(&textRect, L, T, R, B);
	m_MemDC.WriteText(s, textRect, Font2, RefColor, 1);


	T = B + textH;
	textH = intRound(3.0 * h);
	B = T + textH;
	::SetRect(&textRect, L, T, R, B);
	m_MemDC.WriteText(s1, textRect, LargeFont, white, 1);

	T = B;
	B = T + textH;
	::SetRect(&textRect, L, T, R, B);
	m_MemDC.WriteText(s2, textRect, LargeFont, white, 1);

	//Predicted POST ROTATION Refraction
	T = Rect.bottom - 15 - textH;
	B = Rect.bottom - 15;
	::SetRect(&textRect, L, T, R, B);

	//Calculation
	//1. If Tracey Refraction in negative cylinder(as set in Settings, plus cylinder is uncheck), then convert 
	//"POST ROTATION Entire Eye Astigmatism" to negative format
	//(negate cyl and add 90 degrees to axis and subtract 180 if over 180).
	//Call this Apost for astigmatism and Xpost for angle.

	//2. Call the Tracey Refraction Spre D Apre D x Xpre.
	//	Spost = Spre + (Apre - Apost) / 2
	//	Predicted Refraction = Spost D Apost D x Xpost.

	real_t Apost, Spost;
	int Xpost;
	COpData OpData = pCTExam->m_OpData;
	OpData.m_OpType = 1;
	OpData.Recalculate();

	if (OpData.m_EyePostopCyl != INVALID_VALUE && OpData.m_EyePostopAxis != INVALID_VALUE)
	{
		Apost = OpData.m_EyePostopCyl;
		if (!::Settings.m_PositiveCylinder)
		{
			Apost = -1 * Apost;
		}

		Xpost = OpData.m_EyePostopAxis;
		if (!::Settings.m_PositiveCylinder)
		{
			Xpost = Xpost + 90;
			if (Xpost > 180) Xpost -= 180;
		}

		Spost = Spre + (Apre - Apost) / 2.0;

		if (Apost >= 0) s.Format(_T("Predicted POST ROTATION Refraction*     %.2f D +%.2f D x %i°"), Spost, Apost, Xpost);
		else s.Format(_T("Predicted POST ROTATION Refraction*     %.2f D  %.2f D x %i°"), Spost, Apost, Xpost);
		m_MemDC.WriteText(s, textRect, Font2, RefColor, 1);
	}
	//Large Rect Done
}

//******************************************************************************************

// Calculate the Tracey Refraction
void CSumWnd::CalTraceyRef(CWFExam* pWFExam, COLORREF &Color, real_t &Spre, real_t &Apre, CString &s)
{
	if (pWFExam->m_Complaints.m_NightMiopia == COMP_SEVERE ||
		pWFExam->m_Complaints.m_NightHyperopia == COMP_SEVERE ||
		pWFExam->m_Complaints.m_Astigmatism == COMP_SEVERE ||
		pWFExam->m_Complaints.m_BlurDoubleVision == COMP_SEVERE ||
		pWFExam->m_Complaints.m_GlareHalo == COMP_SEVERE ||
		pWFExam->m_Complaints.m_MixedFocus == COMP_SEVERE) {
		Color = RED;
	}
	else if (pWFExam->m_Complaints.m_NightMiopia == COMP_MODERATE ||
		pWFExam->m_Complaints.m_NightHyperopia == COMP_MODERATE ||
		pWFExam->m_Complaints.m_Astigmatism == COMP_MODERATE ||
		pWFExam->m_Complaints.m_BlurDoubleVision == COMP_MODERATE ||
		pWFExam->m_Complaints.m_GlareHalo == COMP_MODERATE ||
		pWFExam->m_Complaints.m_MixedFocus == COMP_MODERATE) {
		Color = m_Printing ? ORANGE : YELLOW;
	}
	else {
		Color = GREEN;
	}

	real_t SphEq, Sph, Cyl; int Axis;
	CZernikeSurface Surface;
	//
	Surface = pWFExam->m_WfSurface;
	Surface.ChangeRMaxUm(2000.0);
	CWFExam::GetSpheqSphCylAxis(Surface, ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
	//
	if (::Settings.m_RefractionRounding == 0) {
		Sph = round8(Sph);
		Cyl = round8(Cyl);
	}
	s.Format(_T("Tracey Refraction         %+.2f D %+.2f D x %i° "), Sph, Cyl, Axis);

	Spre = Sph;
	Apre = Cyl;
}

//******************************************************************************************
// Calculate the IOL enhancement
void CSumWnd::CalIolEnh(CCTExam* pCTExam, CString &s1, CString &s2)
{
	COpData OpData = pCTExam->m_OpData;

	OpData.m_OpType = 1;
	OpData.Recalculate();

	if ((OpData.m_InternalPreopAxis != INVALID_VALUE) && (OpData.m_IOLPlacementAxis != INVALID_VALUE))
	{
		int a[2][2];
		a[0][0] = (OpData.m_InternalPreopAxis + 90) % 180;
		a[0][1] = a[0][0] + 180;
		a[1][0] = OpData.m_IOLPlacementAxis;
		a[1][1] = a[1][0] + 180;

		if (angular_distance3(a[0][0], a[1][0]) > angular_distance3(a[0][0], a[1][1])) Swap(a[1][0], a[1][1]);

		int a1, a2;
		BOOL m_Clock_wise;
		if (angular_distance_counterclockwise3(a[0][0], a[1][0]) < 180)
		{
			a1 = a[0][0];
			a2 = a[1][0];
			m_Clock_wise = 1;
		}
		else
		{
			a1 = a[1][0];
			a2 = a[0][0];
			m_Clock_wise = 0;
		}

		CString angelStr;
		int Angel = angular_distance3(a1, a2);

		if (Angel > 90)
		{
			Angel = 180 - Angel;
		}

		angelStr.Format(_T("%i°"), Angel);

		s1 = angelStr + " rotation ";
	}

	if ((OpData.m_EyePreopCyl != INVALID_VALUE) && (OpData.m_EyePostopCyl != INVALID_VALUE))
	{
		CString DisStr;
		real_t disCylPower = (OpData.m_EyePreopCyl - OpData.m_EyePostopCyl);
		DisStr.Format(_T("%.2f"), disCylPower);

		s2 = DisStr + " D change in cylinder power.";
	}
}

//******************************************************************************************

//6.2.0 WF Near Vision
void CSumWnd::WFNearVisionInfo(CWFExam* pWFExam, RECT Rect)
{
	RECT textRect;
	RECT testRect;
	CString s;

	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	real_t h = (Rect.bottom - Rect.top) / 17.0;

	CMFont Font(intRound(0.8 * 1.1 * h), 400, "Arial");
	CMFont BigFont(intRound(1.25 * 1.1 * h), 400, "Arial");

	int h1 = intRound((real_t)(Rect.bottom - Rect.top) / 6.0);
	int sBottom = Rect.top + h1;

	int lTop = sBottom + intRound(m_g);

	//Small Rect
	RECT smallRect;
	::SetRect(&smallRect, Rect.left, Rect.top, Rect.right, sBottom);

	m_MemDC.DrawRectangle(smallRect, gray, NOCOLOR);

	m_MemDC.MeasureRect("OS ", testRect, BigFont);

	int L = smallRect.left + 10, T = smallRect.top + 15, R = smallRect.left + 3 + testRect.right - testRect.left;
	int textH = intRound((real_t)(smallRect.bottom - smallRect.top) / 1.5);

	s = pWFExam->m_Header.Eye();
	::SetRect(&textRect, L, T, R, T + textH);
	m_MemDC.WriteText(s, textRect, BigFont, pWFExam->m_Header.IsEyeManuallyOverriden() ? RED : white, 0);

	L = R + 3;
	R = smallRect.right - 10;
	::SetRect(&textRect, L, T, R, T + textH);
	m_MemDC.WriteText(" FAR", textRect, Font, white, 0);

	s.Format(_T("  %s  %02i:%02i:%02i"),
		::DateToStr(pWFExam->m_Header.m_Year, pWFExam->m_Header.m_Month, pWFExam->m_Header.m_Day), pWFExam->m_Header.m_Hour, pWFExam->m_Header.m_Minute, pWFExam->m_Header.m_Second);
	m_MemDC.WriteText(s, textRect, Font, white, 2);
	//Small Rect Done

	//Large Rect
	COLORREF Color;
	if (pWFExam->m_Complaints.m_NightMiopia == COMP_SEVERE ||
		pWFExam->m_Complaints.m_NightHyperopia == COMP_SEVERE ||
		pWFExam->m_Complaints.m_Astigmatism == COMP_SEVERE ||
		pWFExam->m_Complaints.m_BlurDoubleVision == COMP_SEVERE ||
		pWFExam->m_Complaints.m_GlareHalo == COMP_SEVERE ||
		pWFExam->m_Complaints.m_MixedFocus == COMP_SEVERE)
	{
		Color = RED;
	}
	else if (pWFExam->m_Complaints.m_NightMiopia == COMP_MODERATE ||
		pWFExam->m_Complaints.m_NightHyperopia == COMP_MODERATE ||
		pWFExam->m_Complaints.m_Astigmatism == COMP_MODERATE ||
		pWFExam->m_Complaints.m_BlurDoubleVision == COMP_MODERATE ||
		pWFExam->m_Complaints.m_GlareHalo == COMP_MODERATE ||
		pWFExam->m_Complaints.m_MixedFocus == COMP_MODERATE)
	{
		Color = m_Printing ? ORANGE : YELLOW;
	}
	else
	{
		Color = GREEN;
	}

	real_t SphEq, Sph, Cyl; int Axis;
	CZernikeSurface Surface;

	RECT LargeRect;
	::SetRect(&LargeRect, Rect.left, lTop, Rect.right, Rect.bottom);
	m_MemDC.DrawRectangle(LargeRect, gray, NOCOLOR);

	textH = intRound(1.35 * 1.2 * h);
	s = " Tracey Refraction FAR";
	L = LargeRect.left + 10;

	T = LargeRect.top - 5 + intRound(real_t(Rect.bottom - lTop) / 2.3);
	int B = T + textH;

	::SetRect(&textRect, L, T, R, B);
	m_MemDC.WriteText(s, textRect, BigFont, white, 1);

	//
	Surface = pWFExam->m_WfSurface;
	Surface.ChangeRMaxUm(2000.0);
	CWFExam::GetSpheqSphCylAxis(Surface, ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
	//
	if (::Settings.m_RefractionRounding == 0)
	{
		Sph = round8(Sph);
		Cyl = round8(Cyl);
	}
	s.Format(_T("%+.2f D %+.2f D x %i° "), Sph, Cyl, Axis);

	T = B + 5;
	B = T + textH;
	::SetRect(&textRect, L, T, R, B);

	m_MemDC.WriteText(s, textRect, BigFont, Color, 1);
}


//******************************************************************************************

//6.2.0 WF Near Vision GUI
void CSumWnd::WFNearVisionInfo2(CWFExam* pWFExam, RECT Rect)
{
	CString s;
	RECT textRect;
	RECT testRect;

	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	real_t h = (Rect.bottom - Rect.top) / 3.0;

	CMFont Font(intRound(0.8 * 1.1 * h), 400, "Arial");
	CMFont BigFont(intRound(1.25 * 1.1 * h), 400, "Arial");

	RECT smallRect;
	::SetRect(&smallRect, Rect.left, Rect.top, Rect.right, Rect.bottom);

	m_MemDC.DrawRectangle(smallRect, gray, NOCOLOR);

	m_MemDC.MeasureRect("OS ", testRect, BigFont);

	int L = smallRect.left + 10, T = smallRect.top + 15, R = smallRect.left + 3 + testRect.right - testRect.left;
	int textH = intRound((real_t)(smallRect.bottom - smallRect.top) / 1.5);

	s = pWFExam->m_Header.Eye();
	::SetRect(&textRect, L, T, R, T + textH);
	m_MemDC.WriteText(s, textRect, BigFont, pWFExam->m_Header.IsEyeManuallyOverriden() ? RED : white, 0);

	L = R + 3;
	R = smallRect.right - 10;
	::SetRect(&textRect, L, T, R, T + textH);
	m_MemDC.WriteText(" SIMULATED NEAR", textRect, Font, white, 0);

	s.Format(_T("  %s  %02i:%02i:%02i"),
		::DateToStr(pWFExam->m_Header.m_Year, pWFExam->m_Header.m_Month, pWFExam->m_Header.m_Day), pWFExam->m_Header.m_Hour, pWFExam->m_Header.m_Minute, pWFExam->m_Header.m_Second);
	m_MemDC.WriteText(s, textRect, Font, white, 2);
}

//******************************************************************************************

// To show Dysfunctional lens info
void CSumWnd::DysInfo(CWFExam* pExam, RECT Rect, real_t dysVal, real_t *rms, int type)// ADD limbusOK
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);

	CString s, s1, s2, s3;

	int L = Rect.left + 3, T = Rect.top + 3, R = Rect.right - 3, B = Rect.bottom;

	real_t t = T;
	real_t h = (Rect.bottom - Rect.top) / 17;
	real_t smallh = 4 * h / 5.0;

	//CMFont Font(intRound(0.9 * h), 400, "Arial");
	CMFont Font(intRound(0.8 * 1.1 * h), 400, "Arial");
	CMFont SmallFont(intRound(0.7 * 1.1 * h), 400, "Arial");
	CMFont VerySmallFont(intRound(0.5 * 1.1 * h), 400, "Arial");
	CMFont BigFont(intRound(1.5 * 1.1 * h), 400, "Arial");
	CMFont LFont(intRound(1.05 * 1.1 * h), 400, "Arial");//600
														 //CMFont BoldFont(intRound(1.1 * h), FW_BOLD, "Arial");//[5.1.1]

														 // date / time, od/os
	if (type == 1)
	{
		::SetRect(&Rect, L, intRound(t + 5), R - 25, intRound(t + h + 10));
		s = pExam->m_Header.Eye(); s += " ";
		m_MemDC.WriteText(s, Rect, BigFont, pExam->m_Header.IsEyeManuallyOverriden() ? RED : white, 2);
		s.Format(_T("  %s  %02i:%02i:%02i"),
			::DateToStr(pExam->m_Header.m_Year, pExam->m_Header.m_Month, pExam->m_Header.m_Day), pExam->m_Header.m_Hour, pExam->m_Header.m_Minute, pExam->m_Header.m_Second);
		m_MemDC.WriteText(s, Rect, Font, white, 0);
	}


	// clinic / physician / operator
	int r1 = intRound(real_t(R - L - 10) / 3.0);
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), L + r1, intRound(t + h + 1));
		CClinic* pClinic = ::DB.GetClinic(pExam->m_Header.m_ClinicID);
		if (pClinic)
		{
			//m_MemDC.WriteText(" Clinic", Rect, SmallFont, white, 0);
			m_MemDC.WriteText("  " + G_ReadChinese(pClinic->m_Name), Rect, VerySmallFont, white, 0);
		}

		::SetRect(&Rect, L + r1, intRound(t), L + r1 + r1, intRound(t + h + 1));
		CPhysician* pPhysician = ::DB.GetPhysician(pExam->m_Header.m_PhysicianID);
		if (pPhysician)
		{
			//m_MemDC.WriteText(" Physician", Rect, SmallFont, white, 0);
			m_MemDC.WriteText(G_ReadChinese(pPhysician->FullName()) + " ", Rect, VerySmallFont, white, 0);
		}

		::SetRect(&Rect, L + r1 + r1, intRound(t), L + r1 + r1 + r1, intRound(t + h + 1));

		COperator* pOperator = ::DB.GetOperator(pExam->m_Header.m_OperatorID);
		if (pOperator)
		{
			//m_MemDC.WriteText(" Operator", Rect, SmallFont, white, 0);
			m_MemDC.WriteText(G_ReadChinese(pOperator->FullName()) + " ", Rect, VerySmallFont, white, 0);
		}
	}


	// limbus / pupil / scan
	t += h;
	if (type == 1)
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		if (pExam->m_Image.m_li_ok) {
			s1.Format(_T("  Limbus %.2f mm  /  "), pExam->m_Image.m_li_r_mean_um * 0.002);
			//
			if (s1 == "  Limbus 0.00 mm  /  ")
			{
				pExam->m_Image.m_li_ok = FALSE;
				s1 = "Limbus ---  /  ";
			}
			//
		}
		else {
			s1 = "Limbus ---  /  ";//WFCT
		}
		if (pExam->m_Image.m_pu_ok) {
			s2.Format(_T("Pupil %.2f mm  /  "), pExam->m_Image.m_pu_r_mean_um * 0.002);
		}
		else {
			s2 = "Pupil ---  /  ";
		}
		s3.Format(_T("Scan %.2f mm"), pExam->m_ScanDiameter * 0.001);
		s = s1 + s2 + s3;
		m_MemDC.WriteText(s, Rect, SmallFont, white, 0);
	}


	real_t SphEq, Sph, Cyl; int Axis;
	CZernikeSurface Surface;

	// Tracey refraction
	COLORREF Color;
	t += h;
	t += h;
	{
		if (pExam->m_Complaints.m_NightMiopia == COMP_SEVERE ||
			pExam->m_Complaints.m_NightHyperopia == COMP_SEVERE ||
			pExam->m_Complaints.m_Astigmatism == COMP_SEVERE ||
			pExam->m_Complaints.m_BlurDoubleVision == COMP_SEVERE ||
			pExam->m_Complaints.m_GlareHalo == COMP_SEVERE ||
			pExam->m_Complaints.m_MixedFocus == COMP_SEVERE) {
			Color = RED;
		}
		else if (pExam->m_Complaints.m_NightMiopia == COMP_MODERATE ||
			pExam->m_Complaints.m_NightHyperopia == COMP_MODERATE ||
			pExam->m_Complaints.m_Astigmatism == COMP_MODERATE ||
			pExam->m_Complaints.m_BlurDoubleVision == COMP_MODERATE ||
			pExam->m_Complaints.m_GlareHalo == COMP_MODERATE ||
			pExam->m_Complaints.m_MixedFocus == COMP_MODERATE) {
			Color = m_Printing ? ORANGE : YELLOW;
		}
		else {
			Color = GREEN;
		}

		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText("  Tracey Refraction", Rect, Font, Color, 0);
		//
		Surface = pExam->m_WfSurface;
		Surface.ChangeRMaxUm(2000.0);
		CWFExam::GetSpheqSphCylAxis(Surface, ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
		//
		if (::Settings.m_RefractionRounding == 0) {
			Sph = round8(Sph);
			Cyl = round8(Cyl);
		}
		s.Format(_T("%+.2f D %+.2f D x %i° "), Sph, Cyl, Axis);
		m_MemDC.WriteText(s, Rect, LFont, Color, 2);
	}
	//Tracey Refraction Done

	//DLI 
	COLORREF CLColor[10];
	CLColor[9] = RGB(250, 0, 0);
	CLColor[8] = RGB(220, 46, 0);
	CLColor[7] = RGB(227, 97, 0);
	CLColor[6] = RGB(227, 137, 0);
	CLColor[5] = RGB(227, 176, 0);
	CLColor[4] = RGB(200, 200, 0);
	CLColor[3] = RGB(176, 227, 0);
	CLColor[2] = RGB(137, 227, 0);
	CLColor[1] = RGB(66, 207, 0);
	CLColor[0] = RGB(17, 247, 100);

	int cn = intRound(10 - dysVal);
	if (cn == 10) cn = 9;
	Color = CLColor[cn];

	t += h;
	t += h;

	//600
	m_MemDC.MeasureRect("  DLI", Rect, Font);

	int Width = Rect.right - Rect.left;
	::SetRect(&Rect, L, intRound(t), L + Width, intRound(t + h + 1));

	s = "  DLI";
	m_MemDC.WriteText(s, Rect, Font, Color, 2);


	m_MemDC.MeasureRect("TM", Rect, VerySmallFont);
	int smallWidth = Rect.right - Rect.left;
	::SetRect(&Rect, L + Width, intRound(t), L + Width + smallWidth, intRound(t + real_t(h) / 2 + 1));
	s = "TM";
	m_MemDC.WriteText(s, Rect, VerySmallFont, Color, 2);
	//600

	::SetRect(&Rect, L + Width + smallWidth, intRound(t), R, intRound(t + h + 1));

	//06032015 for DLI Branch
	if (pExam->m_WfSurface.m_r_max_um < 2500)
		s.Format(_T(" @ D <= %.2f mm"), 0.002 * pExam->m_WfSurface.m_r_max_um);
	else
		s.Format(_T(" @ D <= 5.00 mm"));
	//06032015 for DLI Branch

	m_MemDC.WriteText(s, Rect, Font, Color, 0);

	s.Format(_T("%.2f "), dysVal);
	m_MemDC.WriteText(s, Rect, LFont, Color, 2);
	//DLI Done

	// rms
	t += h;
	t += h;

	m_MemDC.MeasureRect("   HO Total @ D <= 0.00 mm", Rect, Font);
	int w1 = Rect.right - Rect.left;

	::SetRect(&Rect, L, intRound(t), L + w1, intRound(t + h + 1));
	s.Format(_T("  HO Total @ D <= %.2f mm  ="), 0.002 * pExam->m_WfSurface.m_r_max_um);
	m_MemDC.WriteText(s, Rect, Font, white, 0);

	for (int i = 0; i < 3; i++)
	{
		t += h;
		{
			::SetRect(&Rect, L, intRound(t), L + w1, intRound(t + h + 1));

			if (i == 0) s = "  TOTAL EYE";
			else if (i == 1) s = "  CORNEA";
			else if (i == 2) s = "  INTERNAL";

			m_MemDC.WriteText(s, Rect, Font, white, 0);

			s.Format(_T("%6.3f µ "), rms[i]);

			::SetRect(&Rect, L + w1, intRound(t), R, intRound(t + h + 1));

			m_MemDC.WriteText(s, Rect, Font, white, 2);
		}
	}
	t += smallh;
	t += smallh;

	//Angle Alpha 
	//if(pExam->m_AlignmentMethod == ALIGNMENT_4_IR_DOTS && Show_Alpha)
	{
		s = "  Angle Alpha  D = ";
		m_MemDC.MeasureRect(s + "      ", Rect, Font);
		w1 = Rect.right - Rect.left;

		::SetRect(&Rect, L, intRound(t), L + w1, intRound(t + h + 1));
		if (pExam->m_Header.m_AngleAlphaOK)//
		{
			COLORREF yellow = m_Printing ? ORANGE : YELLOW;//601

			if (pExam->m_Header.m_AngleAlphaRUm < 300)     m_MemDC.WriteText(s, Rect, Font, GREEN, 0);
			else
			{
				if (pExam->m_Header.m_AngleAlphaRUm < 500) m_MemDC.WriteText(s, Rect, Font, yellow, 0);
				else                                       m_MemDC.WriteText(s, Rect, Font, RED, 0);
			}

			if (pExam->m_Header.m_AngleAlphaRUm == 0)  pExam->m_Header.m_AngleAlphaADg = 0;//600
			s.Format(_T("%.3f mm @ %i° "), pExam->m_Header.m_AngleAlphaRUm*0.001, pExam->m_Header.m_AngleAlphaADg);
			::SetRect(&Rect, L + w1, intRound(t), R, intRound(t + h + 1));

			//531
			if (pExam->m_Header.m_AngleAlphaRUm < 300)     m_MemDC.WriteText(s, Rect, Font, GREEN, 2);
			else
			{
				if (pExam->m_Header.m_AngleAlphaRUm < 500) m_MemDC.WriteText(s, Rect, Font, yellow, 2);
				else                                       m_MemDC.WriteText(s, Rect, Font, RED, 2);
			}
			//531
		}
		else m_MemDC.WriteText(s, Rect, Font, gray, 0);
	}
	/* else
	{
	s = "  Angle Alpha  D = ";
	m_MemDC.MeasureRect(s + "      ", Rect, Font);
	w1 = Rect.right - Rect.left;

	::SetRect(&Rect, L, intRound(t), L + w1, intRound(t + h + 1));

	m_MemDC.WriteText(s, Rect, Font, gray, 0);
	}*/
	//Done

	// notes
	int nn = 0;
	t += h;
	s = " " + G_ReadChinese(pExam->m_Header.m_Note);

	int l = s.GetLength();
	while (l > 0 && nn < 1)
	{
		int i;
		for (i = 1; i <= l; i++) {
			if (s[i - 1] == '\n') break;
			m_MemDC.MeasureRect(" " + s.Left(i) + " ", Rect, SmallFont);
			if (Rect.right - Rect.left > R - L) {
				for (; i >= 1; i--) {
					if (s[i - 1] == ' ') break;
				}
				break;
			}
		}

		t += h;
		nn++;
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText(" " + s.Left(i) + " ", Rect, SmallFont, white, 0);
		}

		s = s.Right(l - i);
		l -= i;
	}
}
//******************************************************************************************

void CSumWnd::SoloCmpWFExamInfo(CWFExam* pExam, RECT Rect, int NumRows, BOOL* Rows, BOOL HighLight)
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	if (HighLight)
		m_MemDC.DrawRectangle(Rect, white, NOCOLOR);
	else m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);

	CString s, s1, s2, s3;

	int L = Rect.left + 3, T = Rect.top + 3, R = Rect.right - 3, B = Rect.bottom;

	real_t t = T;
	real_t h = (Rect.bottom - Rect.top) / ((real_t)NumRows + 0.5);

	CMFont Font(intRound(1.1 * h), 400, "Arial");
	CMFont BigFont(intRound(1.5 * 1.1 * h), 400, "Arial");
	//CMFont BoldFont(intRound(1.1 * h), FW_BOLD, "Arial");//[5.1.1]

	int n = 0;

	// date / time, od/os
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R - 25, intRound(t + h + 1));
		s = pExam->m_Header.Eye(); s += " ";
		m_MemDC.WriteText(s, Rect, BigFont, pExam->m_Header.IsEyeManuallyOverriden() ? RED : white, 2);
		s.Format(_T(" %s  %02i:%02i:%02i"),
			::DateToStr(pExam->m_Header.m_Year, pExam->m_Header.m_Month, pExam->m_Header.m_Day), pExam->m_Header.m_Hour, pExam->m_Header.m_Minute, pExam->m_Header.m_Second);
		m_MemDC.WriteText(s, Rect, Font, white, 0);
	}

	// clinic
	t += 5;
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));

		CClinic* pClinic = ::DB.GetClinic(pExam->m_Header.m_ClinicID);
		if (pClinic) {
			m_MemDC.WriteText(" Clinic", Rect, Font, white, 0);
			m_MemDC.WriteText(G_ReadChinese(pClinic->m_Name) + " ", Rect, Font, white, 2);
		}
		else m_MemDC.WriteText(" Clinic", Rect, Font, gray, 0);
	}

	// physician
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));

		CPhysician* pPhysician = ::DB.GetPhysician(pExam->m_Header.m_PhysicianID);
		if (pPhysician) {
			m_MemDC.WriteText(" Physician", Rect, Font, white, 0);
			m_MemDC.WriteText(G_ReadChinese(pPhysician->FullName()) + " ", Rect, Font, white, 2);
		}
		else m_MemDC.WriteText(" Physician", Rect, Font, gray, 0);
	}

	// operator
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));

		COperator* pOperator = ::DB.GetOperator(pExam->m_Header.m_OperatorID);
		if (pOperator) {
			m_MemDC.WriteText(" Operator", Rect, Font, white, 0);
			m_MemDC.WriteText(G_ReadChinese(pOperator->FullName()) + " ", Rect, Font, white, 2);
		}
		else m_MemDC.WriteText(" Operator", Rect, Font, gray, 0);
	}

	// limbus / pupil / scan
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Limbus / Pupil / Scan", Rect, Font, white, 0);
		/* if (pExam->m_Image.m_li_ok) {
		s1.Format(_T("%.2f"), pExam->m_Image.m_li_r_mean_um * 0.002);
		}
		else {*/
		s1 = "---";
		//}
		if (pExam->m_Image.m_pu_ok) {
			s2.Format(_T("%.2f"), pExam->m_Image.m_pu_r_mean_um * 0.002);
		}
		else {
			s2 = "---";
		}
		s3.Format(_T("%.2f"), pExam->m_ScanDiameter * 0.001);
		s = s1 + " / " + s2 + " / " + s3 + " mm ";
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	// target
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Fixation Target Position", Rect, Font, white, 0);
		real_t dpt = pExam->m_AccommTargetPosition;
		s.Format(_T("%.2f D "), fabs(dpt));
		if (dpt < -0.001) s = "- " + s; else if (dpt > 0.001) s = "+ " + s;
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	real_t SphEq, Sph, Cyl; int Axis;
	CZernikeSurface Surface;

	// Tracey refraction
	t += h;
	if (Rows[n++]) {
		COLORREF Color;
		if (pExam->m_Complaints.m_NightMiopia == COMP_SEVERE ||
			pExam->m_Complaints.m_NightHyperopia == COMP_SEVERE ||
			pExam->m_Complaints.m_Astigmatism == COMP_SEVERE ||
			pExam->m_Complaints.m_BlurDoubleVision == COMP_SEVERE ||
			pExam->m_Complaints.m_GlareHalo == COMP_SEVERE ||
			pExam->m_Complaints.m_MixedFocus == COMP_SEVERE) {
			Color = RED;
		}
		else if (pExam->m_Complaints.m_NightMiopia == COMP_MODERATE ||
			pExam->m_Complaints.m_NightHyperopia == COMP_MODERATE ||
			pExam->m_Complaints.m_Astigmatism == COMP_MODERATE ||
			pExam->m_Complaints.m_BlurDoubleVision == COMP_MODERATE ||
			pExam->m_Complaints.m_GlareHalo == COMP_MODERATE ||
			pExam->m_Complaints.m_MixedFocus == COMP_MODERATE) {
			Color = m_Printing ? ORANGE : YELLOW;
		}
		else {
			Color = GREEN;
		}

		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Tracey Refraction", Rect, Font, Color, 0);
		//
		Surface = pExam->m_WfSurface;
		Surface.ChangeRMaxUm(2000.0);
		CWFExam::GetSpheqSphCylAxis(Surface, ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
		//
		if (::Settings.m_RefractionRounding == 0) {
			Sph = round8(Sph);
			Cyl = round8(Cyl);
		}
		s.Format(_T("%+.2f D %+.2f D x %i° "), Sph, Cyl, Axis);
		m_MemDC.WriteText(s, Rect, Font, Color, 2);
	}

	// refraction
	m_MemDC.MeasureRect("+00.00 D ", Rect, Font);
	int w1 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("+00.00 D x ", Rect, Font);
	int w2 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("000° ", Rect, Font);
	int w3 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("@ D <= %.2f mm ", Rect, Font);
	int w4 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("VD = %.2f mm ", Rect, Font);
	int w5 = Rect.right - Rect.left;
	for (int i = 0; i < 4; i++)
	{
		t += h;
		if (Rows[n++])
		{
			//
			Surface = pExam->m_WfSurface;
			//
			real_t r_max_um = i < 3 ? 0.5 * ::Settings.m_ZoneDiameterUm[i] : Surface.m_r_max_um;
			if (r_max_um <= Surface.m_r_max_um)
			{
				//
				Surface.ChangeRMaxUm(r_max_um);
				CWFExam::GetSpheqSphCylAxis(Surface, ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
				// sphere
				::SetRect(&Rect, L, intRound(t), L + w1, intRound(t + h + 1));
				s.Format(_T("%+.2f D "), Sph);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
				// cylinder
				::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
				s.Format(_T("%+.2f D x "), Cyl);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
				// axis
				::SetRect(&Rect, L + w1 + w2, intRound(t), L + w1 + w2 + w3, intRound(t + h + 1));
				s.Format(_T("%i° "), Axis);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
				// zone size
				::SetRect(&Rect, L + w1 + w2 + w3, intRound(t), L + w1 + w2 + w3 + w4, intRound(t + h + 1));
				s.Format(_T("@ D <= %.2f mm "), 0.002 * Surface.m_r_max_um);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
				// vertex
				::SetRect(&Rect, L + w1 + w2 + w3 + w4, intRound(t), R, intRound(t + h + 1));
				s.Format(_T("VD = %.2f mm "), 0.001 * ::Settings.m_VertexDistanceUm);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
		}
	}

	// rms
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		Surface = pExam->m_WfSurface;
		s.Format(_T(" Root Mean Square @ D <= %.2f mm"), 0.002 * Surface.m_r_max_um);
		m_MemDC.WriteText(s, Rect, Font, white, 0);
	}

	// rms
	m_MemDC.MeasureRect("+00.000 µ ", Rect, Font);
	w2 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("x 000° ", Rect, Font);
	w3 = Rect.right - Rect.left;
	w1 = (R - L) - (w2 + w3);
	int MaskType[9] = { MASK_TOTAL, MASK_LO_TOTAL, MASK_DEFOCUS, MASK_ASTIGMATISM, MASK_HO_TOTAL, MASK_COMA, MASK_SPHERICAL, MASK_4ORD_ASTIGMATISM, MASK_TREFOIL };
	for (int i = 0; i < 9; i++)
	{
		t += h;
		if (Rows[n++]) {
			//
			CZernikeMask Mask;
			Mask.SetType(MaskType[i]);
			Surface = pExam->m_WfSurface;
			Surface &= Mask;
			real_t rms = Surface.ComputeRms();
			//
			::SetRect(&Rect, L, intRound(t), L + w1, intRound(t + h + 1));
			Mask.GetName(s);

			m_MemDC.WriteText("     " + s, Rect, Font, white, 0);

			// microns
			::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
			s.Format(_T("%6.3f µ "), rms);
			if (MaskType[i] == MASK_DEFOCUS) {
				if (Surface.m_c[4] > 0.0) s = "+" + s;
				else if (Surface.m_c[4] < 0.0) s = "-" + s;
			}
			else if (MaskType[i] == MASK_SPHERICAL) {
				if (Surface.m_c[12] > 0.0) s = "+" + s;
				else if (Surface.m_c[12] < 0.0) s = "-" + s;
			}

			if (i != 4)//[5.1.1]
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			else
			{
				CMDC DC;
				DC.CreateCompatibleDC(NULL);
				DC.MeasureRect("     ", Rect, Font);
				int cw = Rect.right - Rect.left - 3;
				::SetRect(&Rect, L + cw, intRound(t), L + w1 + w2, intRound(t + h + 1));//[5.1.1]
				m_MemDC.DrawRectangle(Rect, white, NOCOLOR);//[5.1.1]
				m_MemDC.WriteText(s, Rect, Font, white, 2);//[5.1.1]
			}


			// angle	  
			::SetRect(&Rect, L + w1 + w2, intRound(t), R, intRound(t + h + 1));

			if (MaskType[i] == MASK_ASTIGMATISM) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[3], Surface.m_c[5]) * _180_Pi / 2.0) % 180);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
			else if (MaskType[i] == MASK_COMA) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[7], Surface.m_c[8]) * _180_Pi) % 360);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
			else if (MaskType[i] == MASK_TREFOIL) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[6], Surface.m_c[9]) * _180_Pi / 3.0) % 120);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
			else if (MaskType[i] == MASK_4ORD_ASTIGMATISM) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[11], Surface.m_c[13]) * _180_Pi / 2.0) % 180);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
		}
	}

	// notes
	s = G_ReadChinese(pExam->m_Header.m_Note) + G_ReadChinese(pExam->m_Header.m_AutoNote);

	int l = s.GetLength();

	while (l > 0 && n < NumRows)
	{
		int i;
		for (i = 1; i <= l; i++) 
		{
			if (s[i - 1] == '\n') break;
			m_MemDC.MeasureRect(" " + s.Left(i) + " ", Rect, Font);
			if (Rect.right - Rect.left > R - L)
			{
				for (; i >= 1; i--)
				{
					if (s[i - 1] == ' ') break;
				}
				break;
			}
		}

		t += h;
		if (Rows[n++]) {
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText(" " + s.Left(i) + " ", Rect, Font, white, 0);
		}

		s = s.Right(l - i);
		l -= i;
	}

}



//******************************************************************************************
//521
void CSumWnd::SoloVfaExamInfo(CWFExam* pExam, RECT Rect, int NumRows, BOOL* Rows, BOOL HighLight)
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	if (HighLight)
		m_MemDC.DrawRectangle(Rect, white, NOCOLOR);
	else m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);

	CString s, s1, s2, s3;

	int L = Rect.left + 3, T = Rect.top + 3, R = Rect.right - 3, B = Rect.bottom;

	real_t t = T;
	real_t h = (Rect.bottom - Rect.top) / ((real_t)NumRows + 0.5);

	CMFont Font(intRound(1.1 * h), 400, "Arial");
	CMFont BigFont(intRound(1.5 * 1.1 * h), 400, "Arial");
	//CMFont BoldFont(intRound(1.1 * h), FW_BOLD, "Arial");//[5.1.1]

	int n = 0;

	// date / time, od/os
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R - 25, intRound(t + h + 1));
		s = pExam->m_Header.Eye(); s += " ";
		m_MemDC.WriteText(s, Rect, BigFont, pExam->m_Header.IsEyeManuallyOverriden() ? RED : white, 2);
		s.Format(_T(" %s  %02i:%02i:%02i"),
			::DateToStr(pExam->m_Header.m_Year, pExam->m_Header.m_Month, pExam->m_Header.m_Day), pExam->m_Header.m_Hour, pExam->m_Header.m_Minute, pExam->m_Header.m_Second);
		m_MemDC.WriteText(s, Rect, Font, white, 0);
	}

	// clinic
	t += 5;
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Clinic", Rect, Font, white, 0);
		CClinic* pClinic = ::DB.GetClinic(pExam->m_Header.m_ClinicID);
		if (pClinic) {
			m_MemDC.WriteText(G_ReadChinese(pClinic->m_Name) + " ", Rect, Font, white, 2);
		}
	}

	// physician
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Physician", Rect, Font, white, 0);
		CPhysician* pPhysician = ::DB.GetPhysician(pExam->m_Header.m_PhysicianID);
		if (pPhysician) {
			m_MemDC.WriteText(G_ReadChinese(pPhysician->FullName()) + " ", Rect, Font, white, 2);
		}
	}

	// operator
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Operator", Rect, Font, white, 0);
		COperator* pOperator = ::DB.GetOperator(pExam->m_Header.m_OperatorID);
		if (pOperator) {
			m_MemDC.WriteText(G_ReadChinese(pOperator->FullName()) + " ", Rect, Font, white, 2);
		}
	}

	// limbus / pupil / scan
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Limbus / Pupil / Scan", Rect, Font, white, 0);
		/* if (pExam->m_Image.m_li_ok) {
		s1.Format(_T("%.2f"), pExam->m_Image.m_li_r_mean_um * 0.002);
		}
		else {*/
		s1 = "---";
		//}
		if (pExam->m_Image.m_pu_ok) {
			s2.Format(_T("%.2f"), pExam->m_Image.m_pu_r_mean_um * 0.002);
		}
		else {
			s2 = "---";
		}
		s3.Format(_T("%.2f"), pExam->m_ScanDiameter * 0.001);
		s = s1 + " / " + s2 + " / " + s3 + " mm ";
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	// target
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Fixation Target Position", Rect, Font, white, 0);
		real_t dpt = pExam->m_AccommTargetPosition;
		s.Format(_T("%.2f D "), fabs(dpt));
		if (dpt < -0.001) s = "- " + s; else if (dpt > 0.001) s = "+ " + s;
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	real_t SphEq, Sph, Cyl; int Axis;
	CZernikeSurface Surface;

	// Tracey refraction
	t += h;
	if (Rows[n++]) {
		COLORREF Color;
		if (pExam->m_Complaints.m_NightMiopia == COMP_SEVERE ||
			pExam->m_Complaints.m_NightHyperopia == COMP_SEVERE ||
			pExam->m_Complaints.m_Astigmatism == COMP_SEVERE ||
			pExam->m_Complaints.m_BlurDoubleVision == COMP_SEVERE ||
			pExam->m_Complaints.m_GlareHalo == COMP_SEVERE ||
			pExam->m_Complaints.m_MixedFocus == COMP_SEVERE) {
			Color = RED;
		}
		else if (pExam->m_Complaints.m_NightMiopia == COMP_MODERATE ||
			pExam->m_Complaints.m_NightHyperopia == COMP_MODERATE ||
			pExam->m_Complaints.m_Astigmatism == COMP_MODERATE ||
			pExam->m_Complaints.m_BlurDoubleVision == COMP_MODERATE ||
			pExam->m_Complaints.m_GlareHalo == COMP_MODERATE ||
			pExam->m_Complaints.m_MixedFocus == COMP_MODERATE) {
			Color = m_Printing ? ORANGE : YELLOW;
		}
		else {
			Color = GREEN;
		}

		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Tracey Refraction", Rect, Font, Color, 0);
		//
		Surface = pExam->m_WfSurface;
		Surface.ChangeRMaxUm(2000.0);
		CWFExam::GetSpheqSphCylAxis(Surface, ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
		//
		if (::Settings.m_RefractionRounding == 0) {
			Sph = round8(Sph);
			Cyl = round8(Cyl);
		}
		s.Format(_T("%+.2f D %+.2f D x %i° "), Sph, Cyl, Axis);
		m_MemDC.WriteText(s, Rect, Font, Color, 2);
	}

	// refraction
	m_MemDC.MeasureRect("+00.00 D ", Rect, Font);
	int w1 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("+00.00 D x ", Rect, Font);
	int w2 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("000° ", Rect, Font);
	int w3 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("@ D <= %.2f mm ", Rect, Font);
	int w4 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("VD = %.2f mm ", Rect, Font);
	int w5 = Rect.right - Rect.left;
	for (int i = 0; i < 4; i++)
	{
		t += h;
		if (Rows[n++])
		{
			//
			Surface = pExam->m_WfSurface;
			//
			real_t r_max_um = i < 3 ? 0.5 * ::Settings.m_ZoneDiameterUm[i] : Surface.m_r_max_um;
			if (r_max_um <= Surface.m_r_max_um)
			{
				//
				Surface.ChangeRMaxUm(r_max_um);
				CWFExam::GetSpheqSphCylAxis(Surface, ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
				// sphere
				::SetRect(&Rect, L, intRound(t), L + w1, intRound(t + h + 1));
				s.Format(_T("%+.2f D "), Sph);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
				// cylinder
				::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
				s.Format(_T("%+.2f D x "), Cyl);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
				// axis
				::SetRect(&Rect, L + w1 + w2, intRound(t), L + w1 + w2 + w3, intRound(t + h + 1));
				s.Format(_T("%i° "), Axis);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
				// zone size
				::SetRect(&Rect, L + w1 + w2 + w3, intRound(t), L + w1 + w2 + w3 + w4, intRound(t + h + 1));
				s.Format(_T("@ D <= %.2f mm "), 0.002 * Surface.m_r_max_um);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
				// vertex
				::SetRect(&Rect, L + w1 + w2 + w3 + w4, intRound(t), R, intRound(t + h + 1));
				s.Format(_T("VD = %.2f mm "), 0.001 * ::Settings.m_VertexDistanceUm);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
		}
	}

	// rms
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		Surface = pExam->m_WfSurface;
		s.Format(_T(" Root Mean Square @ D <= %.2f mm"), 0.002 * Surface.m_r_max_um);
		m_MemDC.WriteText(s, Rect, Font, white, 0);
	}

	// rms
	m_MemDC.MeasureRect("+00.000 µ ", Rect, Font);
	w2 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("x 000° ", Rect, Font);
	w3 = Rect.right - Rect.left;
	w1 = (R - L) - (w2 + w3);
	int MaskType[9] = { MASK_TOTAL, MASK_LO_TOTAL, MASK_DEFOCUS, MASK_ASTIGMATISM, MASK_HO_TOTAL, MASK_COMA, MASK_SPHERICAL, MASK_4ORD_ASTIGMATISM, MASK_TREFOIL };
	for (int i = 0; i < 9; i++)
	{
		t += h;
		if (Rows[n++]) {
			//
			CZernikeMask Mask;
			Mask.SetType(MaskType[i]);
			Surface = pExam->m_WfSurface;
			Surface &= Mask;
			real_t rms = Surface.ComputeRms();
			//
			::SetRect(&Rect, L, intRound(t), L + w1, intRound(t + h + 1));
			Mask.GetName(s);

			m_MemDC.WriteText("     " + s, Rect, Font, white, 0);

			// microns
			::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
			s.Format(_T("%6.3f µ "), rms);
			if (MaskType[i] == MASK_DEFOCUS) {
				if (Surface.m_c[4] > 0.0) s = "+" + s;
				else if (Surface.m_c[4] < 0.0) s = "-" + s;
			}
			else if (MaskType[i] == MASK_SPHERICAL) {
				if (Surface.m_c[12] > 0.0) s = "+" + s;
				else if (Surface.m_c[12] < 0.0) s = "-" + s;
			}

			if (i != 4)//[5.1.1]
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			else
			{
				CMDC DC;
				DC.CreateCompatibleDC(NULL);
				DC.MeasureRect("     ", Rect, Font);
				int cw = Rect.right - Rect.left - 3;
				::SetRect(&Rect, L + cw, intRound(t), L + w1 + w2, intRound(t + h + 1));//[5.1.1]
				m_MemDC.DrawRectangle(Rect, white, NOCOLOR);//[5.1.1]
				m_MemDC.WriteText(s, Rect, Font, white, 2);//[5.1.1]
			}


			// angle	  
			::SetRect(&Rect, L + w1 + w2, intRound(t), R, intRound(t + h + 1));

			if (MaskType[i] == MASK_ASTIGMATISM) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[3], Surface.m_c[5]) * _180_Pi / 2.0) % 180);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
			else if (MaskType[i] == MASK_COMA) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[7], Surface.m_c[8]) * _180_Pi) % 360);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
			else if (MaskType[i] == MASK_TREFOIL) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[6], Surface.m_c[9]) * _180_Pi / 3.0) % 120);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
			else if (MaskType[i] == MASK_4ORD_ASTIGMATISM) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[11], Surface.m_c[13]) * _180_Pi / 2.0) % 180);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
		}
	}

	// notes
	s = G_ReadChinese(pExam->m_Header.m_Note) + G_ReadChinese(pExam->m_Header.m_AutoNote);

	int l = s.GetLength();

	while (l > 0 && n < NumRows)
	{
		int i;
		for (i = 1; i <= l; i++)
		{
			if (s[i - 1] == '\n') break;
			m_MemDC.MeasureRect(" " + s.Left(i) + " ", Rect, Font);
			if (Rect.right - Rect.left > R - L) 
			{
				for (; i >= 1; i--) 
				{
					if (s[i - 1] == ' ') break;
				}
				break;
			}
		}

		t += h;
		if (Rows[n++]) 
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText(" " + s.Left(i) + " ", Rect, Font, white, 0);
		}

		s = s.Right(l - i);
		l -= i;
	}

}
//******************************************************************************************
//530 WF Aberration Text Info (Large)
void CSumWnd::WFExamLargeInfo(CWFExam* pExam, RECT Rect, int type)
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);

	CString s, s1, s2, s3;

	int L = Rect.left + 5, T = Rect.top + 5, R = Rect.right - 5, B = Rect.bottom;

	real_t ns = (type == 0 ? 33 : 35);
	real_t t = T;
	real_t h = (Rect.bottom - Rect.top - 5) / ns;

	CMFont Font(intRound(0.85 * h), 400, "Arial");
	CMFont BigFont(intRound(1.2 * h), 400, "Arial");
	//CMFont BoldFont(intRound(1.1 * h), FW_BOLD, "Arial");//[5.1.1]


	// date / time, od/os
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		s = pExam->m_Header.Eye(); s += " ";
		m_MemDC.WriteText(s, Rect, BigFont, pExam->m_Header.IsEyeManuallyOverriden() ? RED : white, 2);
		s.Format(_T(" %s  %02i:%02i:%02i"),
			::DateToStr(pExam->m_Header.m_Year, pExam->m_Header.m_Month, pExam->m_Header.m_Day), pExam->m_Header.m_Hour, pExam->m_Header.m_Minute, pExam->m_Header.m_Second);
		m_MemDC.WriteText(s, Rect, Font, white, 0);
	}

	// clinic
	t += h;
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		CClinic* pClinic = ::DB.GetClinic(pExam->m_Header.m_ClinicID);
		if (pClinic) {
			m_MemDC.WriteText(" Clinic", Rect, Font, white, 0);
			m_MemDC.WriteText(" " + G_ReadChinese(pClinic->m_Name) + " ", Rect, Font, white, 2);
		}
		else m_MemDC.WriteText(" Clinic", Rect, Font, gray, 0);
	}

	// physician
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		CPhysician* pPhysician = ::DB.GetPhysician(pExam->m_Header.m_PhysicianID);
		if (pPhysician) {
			m_MemDC.WriteText(" Physician", Rect, Font, white, 0);
			m_MemDC.WriteText(" " + G_ReadChinese(pPhysician->FullName()) + " ", Rect, Font, white, 2);
		}
		else m_MemDC.WriteText(" Physician", Rect, Font, gray, 0);
	}

	// operator
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		COperator* pOperator = ::DB.GetOperator(pExam->m_Header.m_OperatorID);
		if (pOperator) {
			m_MemDC.WriteText(" Operator", Rect, Font, white, 0);
			m_MemDC.WriteText(" " + G_ReadChinese(pOperator->FullName()) + " ", Rect, Font, white, 2);
		}
		else m_MemDC.WriteText(" Operator", Rect, Font, gray, 0);
	}

	// limbus / pupil / scan
	t += h;
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		//m_MemDC.WriteText(" Limbus / Pupil / Scan", Rect, Font, white, 0);
		m_MemDC.WriteText(" Pupil / Scan", Rect, Font, white, 0);//
																 /* if (pExam->m_Image.m_li_ok) {
																 s1.Format(_T("%.2f"), pExam->m_Image.m_li_r_mean_um * 0.002);
																 }
																 else {*/
																 //s1 = "---";//
																 //}
		if (pExam->m_Image.m_pu_ok) {
			s2.Format(_T("%.2f"), pExam->m_Image.m_pu_r_mean_um * 0.002);
		}
		else {
			s2 = "---";
		}
		s3.Format(_T("%.2f"), pExam->m_ScanDiameter * 0.001);
		//s = s1 + " / " + s2 + " / " + s3 + " mm ";//
		s = s2 + " / " + s3 + " mm ";//
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	// target
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Fixation Target Position", Rect, Font, white, 0);
		real_t dpt = pExam->m_AccommTargetPosition;
		s.Format(_T("%.2f D "), fabs(dpt));
		if (dpt < -0.001) s = "- " + s; else if (dpt > 0.001) s = "+ " + s;
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	real_t SphEq, Sph, Cyl; int Axis;
	CZernikeSurface Surface;

	// Tracey refraction
	t += h;
	t += h;
	{
		COLORREF Color;
		if (pExam->m_Complaints.m_NightMiopia == COMP_SEVERE ||
			pExam->m_Complaints.m_NightHyperopia == COMP_SEVERE ||
			pExam->m_Complaints.m_Astigmatism == COMP_SEVERE ||
			pExam->m_Complaints.m_BlurDoubleVision == COMP_SEVERE ||
			pExam->m_Complaints.m_GlareHalo == COMP_SEVERE ||
			pExam->m_Complaints.m_MixedFocus == COMP_SEVERE) {
			Color = RED;
		}
		else if (pExam->m_Complaints.m_NightMiopia == COMP_MODERATE ||
			pExam->m_Complaints.m_NightHyperopia == COMP_MODERATE ||
			pExam->m_Complaints.m_Astigmatism == COMP_MODERATE ||
			pExam->m_Complaints.m_BlurDoubleVision == COMP_MODERATE ||
			pExam->m_Complaints.m_GlareHalo == COMP_MODERATE ||
			pExam->m_Complaints.m_MixedFocus == COMP_MODERATE) {
			Color = m_Printing ? ORANGE : YELLOW;
		}
		else {
			Color = GREEN;
		}

		::SetRect(&Rect, L + 4, intRound(t), R - 4, intRound(t + h + 1));	//
		m_MemDC.WriteText(" Tracey Refraction", Rect, Font, Color, 0);
		//
		Surface = pExam->m_WfSurface;
		Surface.ChangeRMaxUm(2000.0);
		CWFExam::GetSpheqSphCylAxis(Surface, ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
		//
		if (::Settings.m_RefractionRounding == 0) {
			Sph = round8(Sph);
			Cyl = round8(Cyl);
		}
		s.Format(_T("%+.2f D %+.2f D x %i° "), Sph, Cyl, Axis);
		m_MemDC.WriteText(s, Rect, Font, Color, 2);
	}

	// refraction
	m_MemDC.MeasureRect("+00.00 D ", Rect, Font);
	int w1 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("+00.00 D x ", Rect, Font);
	int w2 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("000° ", Rect, Font);
	int w3 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("@ D <= %.2f mm ", Rect, Font);
	int w4 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("VD = %.2f mm ", Rect, Font);
	int w5 = Rect.right - Rect.left;
	for (int i = 0; i < 4; i++)
	{
		t += h;

		{
			//
			Surface = pExam->m_WfSurface;
			//
			real_t r_max_um = i < 3 ? 0.5 * ::Settings.m_ZoneDiameterUm[i] : Surface.m_r_max_um;
			if (r_max_um <= Surface.m_r_max_um)
			{
				//
				Surface.ChangeRMaxUm(r_max_um);
				CWFExam::GetSpheqSphCylAxis(Surface, ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
				// sphere
				::SetRect(&Rect, L, intRound(t), L + w1, intRound(t + h + 1));
				s.Format(_T("%+.2f D "), Sph);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
				// cylinder
				::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
				s.Format(_T("%+.2f D x "), Cyl);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
				// axis
				::SetRect(&Rect, L + w1 + w2, intRound(t), L + w1 + w2 + w3, intRound(t + h + 1));
				s.Format(_T("%i° "), Axis);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
				// zone size
				::SetRect(&Rect, L + w1 + w2 + w3, intRound(t), L + w1 + w2 + w3 + w4, intRound(t + h + 1));
				s.Format(_T("@ D <= %.2f mm "), 0.002 * Surface.m_r_max_um);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
				// vertex
				::SetRect(&Rect, L + w1 + w2 + w3 + w4, intRound(t), R, intRound(t + h + 1));
				s.Format(_T("VD = %.2f mm "), 0.001 * ::Settings.m_VertexDistanceUm);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
			else
			{
				// zone size
				::SetRect(&Rect, L + w1 + w2 + w3, intRound(t), L + w1 + w2 + w3 + w4, intRound(t + h + 1));
				s.Format(_T("@ D <= %.2f mm "), 0.001 * ::Settings.m_ZoneDiameterUm[i]);
				m_MemDC.WriteText(s, Rect, Font, gray, 2);

				// vertex
				::SetRect(&Rect, L + w1 + w2 + w3 + w4, intRound(t), R, intRound(t + h + 1));
				s.Format(_T("VD = %.2f mm "), 0.001 * ::Settings.m_VertexDistanceUm);
				m_MemDC.WriteText(s, Rect, Font, gray, 2);
			}
		}
	}

	// rms
	t += h;
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		Surface = pExam->m_WfSurface;
		s.Format(_T(" Root Mean Square @ D <= %.2f mm"), 0.002 * Surface.m_r_max_um);
		m_MemDC.WriteText(s, Rect, Font, white, 0);
	}

	// rms
	m_MemDC.MeasureRect("+00.000 µ ", Rect, Font);
	w2 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("x 000° ", Rect, Font);
	w3 = Rect.right - Rect.left;
	w1 = (R - L) - (w2 + w3);
	int MaskType[9] = { MASK_TOTAL, MASK_LO_TOTAL, MASK_DEFOCUS, MASK_ASTIGMATISM, MASK_HO_TOTAL, MASK_COMA, MASK_SPHERICAL, MASK_4ORD_ASTIGMATISM, MASK_TREFOIL };
	for (int i = 0; i < 9; i++)
	{
		t += h;
		{
			//
			CZernikeMask Mask;
			Mask.SetType(MaskType[i]);
			Surface = pExam->m_WfSurface;
			Surface &= Mask;
			real_t rms = Surface.ComputeRms();
			//
			::SetRect(&Rect, L, intRound(t), L + w1, intRound(t + h + 1));
			Mask.GetName(s);

			m_MemDC.WriteText("     " + s, Rect, Font, white, 0);

			// microns
			::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
			s.Format(_T("%6.3f µ "), rms);
			if (MaskType[i] == MASK_DEFOCUS) {
				if (Surface.m_c[4] > 0.0) s = "+" + s;
				else if (Surface.m_c[4] < 0.0) s = "-" + s;
			}
			else if (MaskType[i] == MASK_SPHERICAL) {
				if (Surface.m_c[12] > 0.0) s = "+" + s;
				else if (Surface.m_c[12] < 0.0) s = "-" + s;
			}

			if (i != 4)//[5.1.1]
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			else
			{
				CMDC DC;
				DC.CreateCompatibleDC(NULL);
				DC.MeasureRect("     ", Rect, Font);
				int cw = Rect.right - Rect.left - 3;
				::SetRect(&Rect, L + cw, intRound(t), L + w1 + w2, intRound(t + h + 1));//[5.1.1]
				m_MemDC.DrawRectangle(Rect, white, NOCOLOR);//[5.1.1]
				m_MemDC.WriteText(s, Rect, Font, white, 2);//[5.1.1]
			}


			// angle	  
			::SetRect(&Rect, L + w1 + w2, intRound(t), R, intRound(t + h + 1));

			if (MaskType[i] == MASK_ASTIGMATISM) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[3], Surface.m_c[5]) * _180_Pi / 2.0) % 180);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
			else if (MaskType[i] == MASK_COMA) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[7], Surface.m_c[8]) * _180_Pi) % 360);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
			else if (MaskType[i] == MASK_TREFOIL) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[6], Surface.m_c[9]) * _180_Pi / 3.0) % 120);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
			else if (MaskType[i] == MASK_4ORD_ASTIGMATISM) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[11], Surface.m_c[13]) * _180_Pi / 2.0) % 180);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
		}
	}

	// notes
	int startN = 0;

	int nn = 0;
	t += h;
	s = G_ReadChinese(pExam->m_Header.m_Note) + G_ReadChinese(pExam->m_Header.m_AutoNote);

	int l = s.GetLength();

	while (l > 0 && nn <= 7)
	{
		int i;
		for (i = 1; i <= l; i++)
		{
			if (s[i - 1] == '\n') break;
			m_MemDC.MeasureRect(" " + s.Left(i) + " ", Rect, Font);
			if (Rect.right - Rect.left > R - L)
			{
				for (; i >= 1; i--)
				{
					if (s[i - 1] == ' ') break;
				}
				break;
			}
		}

		t += h;
		nn++;
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));

			int count = s.Left(i).Find(_T("Alpha"), 0);
			int count2 = s.Left(i).Find(_T("Kappa"), 0);

			if (count != -1 || count2 != -1)
			{
				// COLORREF color = RED;

				// int count0 = s.Left(i).Find(_T("0.0", 0);//531
				// int count1 = s.Left(i).Find(_T("0.1", 0);
				// int count2 = s.Left(i).Find(_T("0.2", 0);		  
				// //int count3 = s.Left(i).Find(_T("0.300", 0);
				// int count4 = s.Left(i).Find(_T("0.3", 0);
				// int count5 = s.Left(i).Find(_T("0.4", 0);
				//// int count6 = s.Left(i).Find(_T("0.500", 0);

				// if(count0 != -1 || count1 != -1 || count2 != -1) 
				//  color = GREEN;
				// else if(count4 != -1 || count5 != -1) 
				//  color = m_Printing ? ORANGE : YELLOW;//601;

				// m_MemDC.WriteText(" " + s.Left(i) + " ", Rect, Font, color, 0);
			}
			else m_MemDC.WriteText(" " + s.Left(i) + " ", Rect, Font, white, 0);
		}

		s = s.Right(l - i);
		l -= i;
	}

}

//******************************************************************************************
// WFCT Angle K/A
void CSumWnd::WFCTExamLargeInfo(CWFExam* pExam, RECT Rect)
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);

	CString s, s1, s2, s3;

	int L = Rect.left + 5, T = Rect.top + 5, R = Rect.right - 5, B = Rect.bottom;

	real_t ns = 33;
	real_t t = T;
	real_t h = (Rect.bottom - Rect.top - 5) / ns;

	CMFont Font(intRound(0.85 * h), 400, "Arial");
	CMFont BigFont(intRound(1.2 * h), 400, "Arial");
	//CMFont BoldFont(intRound(1.1 * h), FW_BOLD, "Arial");//[5.1.1]


	// date / time, od/os
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		s = pExam->m_Header.Eye(); s += " ";
		m_MemDC.WriteText(s, Rect, BigFont, pExam->m_Header.IsEyeManuallyOverriden() ? RED : white, 2);
		s.Format(_T(" %s  %02i:%02i:%02i"),
			::DateToStr(pExam->m_Header.m_Year, pExam->m_Header.m_Month, pExam->m_Header.m_Day), pExam->m_Header.m_Hour, pExam->m_Header.m_Minute, pExam->m_Header.m_Second);
		m_MemDC.WriteText(s, Rect, Font, white, 0);
	}

	// clinic
	t += h;
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		CClinic* pClinic = ::DB.GetClinic(pExam->m_Header.m_ClinicID);
		if (pClinic) {
			m_MemDC.WriteText(" Clinic", Rect, Font, white, 0);
			m_MemDC.WriteText(" " + G_ReadChinese(pClinic->m_Name) + " ", Rect, Font, white, 2);
		}
		else m_MemDC.WriteText(" Clinic", Rect, Font, gray, 0);
	}

	// physician
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		CPhysician* pPhysician = ::DB.GetPhysician(pExam->m_Header.m_PhysicianID);
		if (pPhysician) {
			m_MemDC.WriteText(" Physician", Rect, Font, white, 0);
			m_MemDC.WriteText(" " + G_ReadChinese(pPhysician->FullName()) + " ", Rect, Font, white, 2);
		}
		else m_MemDC.WriteText(" Physician", Rect, Font, gray, 0);
	}

	// operator
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		COperator* pOperator = ::DB.GetOperator(pExam->m_Header.m_OperatorID);
		if (pOperator) {
			m_MemDC.WriteText(" Operator", Rect, Font, white, 0);
			m_MemDC.WriteText(" " + G_ReadChinese(pOperator->FullName()) + " ", Rect, Font, white, 2);
		}
		else m_MemDC.WriteText(" Operator", Rect, Font, gray, 0);
	}

	// limbus / pupil / scan
	t += h;
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Limbus / Pupil / Scan", Rect, Font, white, 0);
		if (pExam->m_Image.m_li_ok) {
			s1.Format(_T("%.2f"), pExam->m_Image.m_li_r_mean_um * 0.002);
		}
		else {
			s1 = "---";
		}
		if (pExam->m_Image.m_pu_ok) {
			s2.Format(_T("%.2f"), pExam->m_Image.m_pu_r_mean_um * 0.002);
		}
		else {
			s2 = "---";
		}
		s3.Format(_T("%.2f"), pExam->m_ScanDiameter * 0.001);
		s = s1 + " / " + s2 + " / " + s3 + " mm ";
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	// target
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Fixation Target Position", Rect, Font, white, 0);
		real_t dpt = pExam->m_AccommTargetPosition;
		s.Format(_T("%.2f D "), fabs(dpt));
		if (dpt < -0.001) s = "- " + s; else if (dpt > 0.001) s = "+ " + s;
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	real_t SphEq, Sph, Cyl; int Axis;
	CZernikeSurface Surface;

	// Tracey refraction
	t += h;
	t += h;
	{
		COLORREF Color;
		if (pExam->m_Complaints.m_NightMiopia == COMP_SEVERE ||
			pExam->m_Complaints.m_NightHyperopia == COMP_SEVERE ||
			pExam->m_Complaints.m_Astigmatism == COMP_SEVERE ||
			pExam->m_Complaints.m_BlurDoubleVision == COMP_SEVERE ||
			pExam->m_Complaints.m_GlareHalo == COMP_SEVERE ||
			pExam->m_Complaints.m_MixedFocus == COMP_SEVERE) {
			Color = RED;
		}
		else if (pExam->m_Complaints.m_NightMiopia == COMP_MODERATE ||
			pExam->m_Complaints.m_NightHyperopia == COMP_MODERATE ||
			pExam->m_Complaints.m_Astigmatism == COMP_MODERATE ||
			pExam->m_Complaints.m_BlurDoubleVision == COMP_MODERATE ||
			pExam->m_Complaints.m_GlareHalo == COMP_MODERATE ||
			pExam->m_Complaints.m_MixedFocus == COMP_MODERATE) {
			Color = m_Printing ? ORANGE : YELLOW;
		}
		else {
			Color = GREEN;
		}

		::SetRect(&Rect, L + 4, intRound(t), R - 4, intRound(t + h + 1));	//
		m_MemDC.WriteText(" Tracey Refraction", Rect, Font, Color, 0);
		//
		Surface = pExam->m_WfSurface;
		Surface.ChangeRMaxUm(2000.0);
		CWFExam::GetSpheqSphCylAxis(Surface, ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
		//
		if (::Settings.m_RefractionRounding == 0) {
			Sph = round8(Sph);
			Cyl = round8(Cyl);
		}
		s.Format(_T("%+.2f D %+.2f D x %i° "), Sph, Cyl, Axis);
		m_MemDC.WriteText(s, Rect, Font, Color, 2);
	}

	// refraction
	m_MemDC.MeasureRect("+00.00 D ", Rect, Font);
	int w1 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("+00.00 D x ", Rect, Font);
	int w2 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("000° ", Rect, Font);
	int w3 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("@ D <= %.2f mm ", Rect, Font);
	int w4 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("VD = %.2f mm ", Rect, Font);
	int w5 = Rect.right - Rect.left;
	for (int i = 0; i < 4; i++)
	{
		t += h;

		{
			//
			Surface = pExam->m_WfSurface;
			//
			real_t r_max_um = i < 3 ? 0.5 * ::Settings.m_ZoneDiameterUm[i] : Surface.m_r_max_um;
			if (r_max_um <= Surface.m_r_max_um)
			{
				//
				Surface.ChangeRMaxUm(r_max_um);
				CWFExam::GetSpheqSphCylAxis(Surface, ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
				// sphere
				::SetRect(&Rect, L, intRound(t), L + w1, intRound(t + h + 1));
				s.Format(_T("%+.2f D "), Sph);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
				// cylinder
				::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
				s.Format(_T("%+.2f D x "), Cyl);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
				// axis
				::SetRect(&Rect, L + w1 + w2, intRound(t), L + w1 + w2 + w3, intRound(t + h + 1));
				s.Format(_T("%i° "), Axis);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
				// zone size
				::SetRect(&Rect, L + w1 + w2 + w3, intRound(t), L + w1 + w2 + w3 + w4, intRound(t + h + 1));
				s.Format(_T("@ D <= %.2f mm "), 0.002 * Surface.m_r_max_um);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
				// vertex
				::SetRect(&Rect, L + w1 + w2 + w3 + w4, intRound(t), R, intRound(t + h + 1));
				s.Format(_T("VD = %.2f mm "), 0.001 * ::Settings.m_VertexDistanceUm);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
			else
			{
				// zone size
				::SetRect(&Rect, L + w1 + w2 + w3, intRound(t), L + w1 + w2 + w3 + w4, intRound(t + h + 1));
				s.Format(_T("@ D <= %.2f mm "), 0.001 * ::Settings.m_ZoneDiameterUm[i]);
				m_MemDC.WriteText(s, Rect, Font, gray, 2);

				// vertex
				::SetRect(&Rect, L + w1 + w2 + w3 + w4, intRound(t), R, intRound(t + h + 1));
				s.Format(_T("VD = %.2f mm "), 0.001 * ::Settings.m_VertexDistanceUm);
				m_MemDC.WriteText(s, Rect, Font, gray, 2);
			}
		}
	}

	// rms
	t += h;
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		Surface = pExam->m_WfSurface;
		s.Format(_T(" Root Mean Square @ D <= %.2f mm"), 0.002 * Surface.m_r_max_um);
		m_MemDC.WriteText(s, Rect, Font, white, 0);
	}

	// rms
	m_MemDC.MeasureRect("+00.000 µ ", Rect, Font);
	w2 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("x 000° ", Rect, Font);
	w3 = Rect.right - Rect.left;
	w1 = (R - L) - (w2 + w3);
	int MaskType[9] = { MASK_TOTAL, MASK_LO_TOTAL, MASK_DEFOCUS, MASK_ASTIGMATISM, MASK_HO_TOTAL, MASK_COMA, MASK_SPHERICAL, MASK_4ORD_ASTIGMATISM, MASK_TREFOIL };
	for (int i = 0; i < 9; i++)
	{
		t += h;
		{
			//
			CZernikeMask Mask;
			Mask.SetType(MaskType[i]);
			Surface = pExam->m_WfSurface;
			Surface &= Mask;
			real_t rms = Surface.ComputeRms();
			//
			::SetRect(&Rect, L, intRound(t), L + w1, intRound(t + h + 1));
			Mask.GetName(s);

			m_MemDC.WriteText("     " + s, Rect, Font, white, 0);

			// microns
			::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
			s.Format(_T("%6.3f µ "), rms);
			if (MaskType[i] == MASK_DEFOCUS) {
				if (Surface.m_c[4] > 0.0) s = "+" + s;
				else if (Surface.m_c[4] < 0.0) s = "-" + s;
			}
			else if (MaskType[i] == MASK_SPHERICAL) {
				if (Surface.m_c[12] > 0.0) s = "+" + s;
				else if (Surface.m_c[12] < 0.0) s = "-" + s;
			}

			if (i != 4)//[5.1.1]
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			else
			{
				CMDC DC;
				DC.CreateCompatibleDC(NULL);
				DC.MeasureRect("     ", Rect, Font);
				int cw = Rect.right - Rect.left - 3;
				::SetRect(&Rect, L + cw, intRound(t), L + w1 + w2, intRound(t + h + 1));//[5.1.1]
				m_MemDC.DrawRectangle(Rect, white, NOCOLOR);//[5.1.1]
				m_MemDC.WriteText(s, Rect, Font, white, 2);//[5.1.1]
			}


			// angle	  
			::SetRect(&Rect, L + w1 + w2, intRound(t), R, intRound(t + h + 1));

			if (MaskType[i] == MASK_ASTIGMATISM) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[3], Surface.m_c[5]) * _180_Pi / 2.0) % 180);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
			else if (MaskType[i] == MASK_COMA) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[7], Surface.m_c[8]) * _180_Pi) % 360);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
			else if (MaskType[i] == MASK_TREFOIL) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[6], Surface.m_c[9]) * _180_Pi / 3.0) % 120);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
			else if (MaskType[i] == MASK_4ORD_ASTIGMATISM) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[11], Surface.m_c[13]) * _180_Pi / 2.0) % 180);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
		}
	}

	// notes
	int startN = 0;

	int nn = 0;
	t += h;
	s = G_ReadChinese(pExam->m_Header.m_Note) + G_ReadChinese(pExam->m_Header.m_AutoNote);

	int l = s.GetLength();
	while (l > 0 && nn <= 7)
	{
		int i;
		for (i = 1; i <= l; i++)
		{
			if (s[i - 1] == '\n') break;
			m_MemDC.MeasureRect(" " + s.Left(i) + " ", Rect, Font);
			if (Rect.right - Rect.left > R - L) {
				for (; i >= 1; i--) {
					if (s[i - 1] == ' ') break;
				}
				break;
			}
		}

		t += h;
		nn++;
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));

			int count = s.Left(i).Find(_T("Alpha"), 0);
			int count2 = s.Left(i).Find(_T("Kappa"), 0);

			if (count != -1 || count2 != -1)
			{
				COLORREF color = RED;

				int count0 = s.Left(i).Find(_T("0.0"), 0);//531
				int count1 = s.Left(i).Find(_T("0.1"), 0);
				int count2 = s.Left(i).Find(_T("0.2"), 0);
				//int count3 = s.Left(i).Find(_T("0.300", 0);
				int count4 = s.Left(i).Find(_T("0.3"), 0);
				int count5 = s.Left(i).Find(_T("0.4"), 0);
				// int count6 = s.Left(i).Find(_T("0.500", 0);

				if (count0 != -1 || count1 != -1 || count2 != -1)
					color = GREEN;
				else if (count4 != -1 || count5 != -1)
					color = m_Printing ? ORANGE : YELLOW;//601;

				m_MemDC.WriteText(" " + s.Left(i) + " ", Rect, Font, color, 0);
			}
			else m_MemDC.WriteText(" " + s.Left(i) + " ", Rect, Font, white, 0);
		}

		s = s.Right(l - i);
		l -= i;
	}

}
//******************************************************************************************
//521 Solo Aberration Text Info
void CSumWnd::WFSoloExamInfo(CWFExam* pExam, RECT Rect, int NumRows, BOOL* Rows, BOOL HighLight)
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);

	CString s, s1, s2, s3;

	int L = Rect.left + 5, T = Rect.top + 5, R = Rect.right - 5, B = Rect.bottom;

	real_t t = T;
	real_t h = (Rect.bottom - Rect.top - 5) / (real_t)NumRows;

	CMFont Font(intRound(0.85 * h), 400, "Arial");
	CMFont BigFont(intRound(1.2 * h), 400, "Arial");
	//CMFont BoldFont(intRound(1.1 * h), FW_BOLD, "Arial");//[5.1.1]

	int n = 0;

	// date / time, od/os
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		s = pExam->m_Header.Eye(); s += " ";
		m_MemDC.WriteText(s, Rect, BigFont, pExam->m_Header.IsEyeManuallyOverriden() ? RED : white, 2);
		s.Format(_T(" %s  %02i:%02i:%02i"),
			::DateToStr(pExam->m_Header.m_Year, pExam->m_Header.m_Month, pExam->m_Header.m_Day), pExam->m_Header.m_Hour, pExam->m_Header.m_Minute, pExam->m_Header.m_Second);
		m_MemDC.WriteText(s, Rect, Font, white, 0);
	}

	// clinic
	t += h;
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		CClinic* pClinic = ::DB.GetClinic(pExam->m_Header.m_ClinicID);
		if (pClinic) {
			m_MemDC.WriteText(" " + pClinic->m_Name + " ", Rect, Font, white, 0);
		}
	}

	// physician
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		CPhysician* pPhysician = ::DB.GetPhysician(pExam->m_Header.m_PhysicianID);
		if (pPhysician) {
			m_MemDC.WriteText(" " + pPhysician->FullName() + " ", Rect, Font, white, 0);
		}
	}

	// operator
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		COperator* pOperator = ::DB.GetOperator(pExam->m_Header.m_OperatorID);
		if (pOperator) {
			m_MemDC.WriteText(" " + pOperator->FullName() + " ", Rect, Font, white, 0);
		}
	}

	// limbus / pupil / scan
	t += h;
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Limbus / Pupil / Scan", Rect, Font, white, 0);
		/* if (pExam->m_Image.m_li_ok) {
		s1.Format(_T("%.2f"), pExam->m_Image.m_li_r_mean_um * 0.002);
		}
		else {*/
		s1 = "---";
		//}
		if (pExam->m_Image.m_pu_ok) {
			s2.Format(_T("%.2f"), pExam->m_Image.m_pu_r_mean_um * 0.002);
		}
		else {
			s2 = "---";
		}
		s3.Format(_T("%.2f"), pExam->m_ScanDiameter * 0.001);
		s = s1 + " / " + s2 + " / " + s3 + " mm ";
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	// target
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Fixation Target Position", Rect, Font, white, 0);
		real_t dpt = pExam->m_AccommTargetPosition;
		s.Format(_T("%.2f D "), fabs(dpt));
		if (dpt < -0.001) s = "- " + s; else if (dpt > 0.001) s = "+ " + s;
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	real_t SphEq, Sph, Cyl; int Axis;
	CZernikeSurface Surface;

	// Tracey refraction
	t += h;
	t += h;
	if (Rows[n++]) {
		COLORREF Color;
		if (pExam->m_Complaints.m_NightMiopia == COMP_SEVERE ||
			pExam->m_Complaints.m_NightHyperopia == COMP_SEVERE ||
			pExam->m_Complaints.m_Astigmatism == COMP_SEVERE ||
			pExam->m_Complaints.m_BlurDoubleVision == COMP_SEVERE ||
			pExam->m_Complaints.m_GlareHalo == COMP_SEVERE ||
			pExam->m_Complaints.m_MixedFocus == COMP_SEVERE) {
			Color = RED;
		}
		else if (pExam->m_Complaints.m_NightMiopia == COMP_MODERATE ||
			pExam->m_Complaints.m_NightHyperopia == COMP_MODERATE ||
			pExam->m_Complaints.m_Astigmatism == COMP_MODERATE ||
			pExam->m_Complaints.m_BlurDoubleVision == COMP_MODERATE ||
			pExam->m_Complaints.m_GlareHalo == COMP_MODERATE ||
			pExam->m_Complaints.m_MixedFocus == COMP_MODERATE) {
			Color = m_Printing ? ORANGE : YELLOW;
		}
		else {
			Color = GREEN;
		}


		::SetRect(&Rect, L + 2, intRound(t) - 3, R - 2, intRound(t + h + 3));	//
		if (HighLight)
			m_MemDC.DrawRectangle(Rect, white, NOCOLOR);
		else m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);

		::SetRect(&Rect, L + 4, intRound(t), R - 4, intRound(t + h + 1));	//
		m_MemDC.WriteText(" Tracey Refraction", Rect, Font, Color, 0);
		//
		Surface = pExam->m_WfSurface;
		Surface.ChangeRMaxUm(2000.0);
		CWFExam::GetSpheqSphCylAxis(Surface, ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
		//
		if (::Settings.m_RefractionRounding == 0) {
			Sph = round8(Sph);
			Cyl = round8(Cyl);
		}
		s.Format(_T("%+.2f D %+.2f D x %i° "), Sph, Cyl, Axis);
		m_MemDC.WriteText(s, Rect, Font, Color, 2);
	}

	// rms
	t += h;
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		Surface = pExam->m_WfSurface;
		s.Format(_T(" Root Mean Square @ D <= %.2f mm"), 0.002 * Surface.m_r_max_um);
		m_MemDC.WriteText(s, Rect, Font, white, 0);
	}

	// rms
	m_MemDC.MeasureRect("+00.000 µ ", Rect, Font);
	int w2 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("x 000° ", Rect, Font);
	int w3 = Rect.right - Rect.left;
	int w1 = (R - L) - (w2 + w3);
	int MaskType[9] = { MASK_TOTAL, MASK_LO_TOTAL, MASK_DEFOCUS, MASK_ASTIGMATISM, MASK_HO_TOTAL, MASK_COMA, MASK_SPHERICAL, MASK_4ORD_ASTIGMATISM, MASK_TREFOIL };
	for (int i = 0; i < 9; i++)
	{
		t += h;
		if (Rows[n++]) {
			//
			CZernikeMask Mask;
			Mask.SetType(MaskType[i]);
			Surface = pExam->m_WfSurface;
			Surface &= Mask;
			real_t rms = Surface.ComputeRms();
			//
			::SetRect(&Rect, L, intRound(t), L + w1, intRound(t + h + 1));
			Mask.GetName(s);

			m_MemDC.WriteText("     " + s, Rect, Font, white, 0);

			// microns
			::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
			s.Format(_T("%6.3f µ "), rms);
			if (MaskType[i] == MASK_DEFOCUS) {
				if (Surface.m_c[4] > 0.0) s = "+" + s;
				else if (Surface.m_c[4] < 0.0) s = "-" + s;
			}
			else if (MaskType[i] == MASK_SPHERICAL) {
				if (Surface.m_c[12] > 0.0) s = "+" + s;
				else if (Surface.m_c[12] < 0.0) s = "-" + s;
			}

			if (i != 4)//[5.1.1]
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			else
			{
				CMDC DC;
				DC.CreateCompatibleDC(NULL);
				DC.MeasureRect("     ", Rect, Font);
				int cw = Rect.right - Rect.left - 3;
				::SetRect(&Rect, L + cw, intRound(t), L + w1 + w2, intRound(t + h + 1));//[5.1.1]
				m_MemDC.DrawRectangle(Rect, white, NOCOLOR);//[5.1.1]
				m_MemDC.WriteText(s, Rect, Font, white, 2);//[5.1.1]
			}


			// angle	  
			::SetRect(&Rect, L + w1 + w2, intRound(t), R, intRound(t + h + 1));

			if (MaskType[i] == MASK_ASTIGMATISM) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[3], Surface.m_c[5]) * _180_Pi / 2.0) % 180);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
			else if (MaskType[i] == MASK_COMA) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[7], Surface.m_c[8]) * _180_Pi) % 360);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
			else if (MaskType[i] == MASK_TREFOIL) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[6], Surface.m_c[9]) * _180_Pi / 3.0) % 120);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
			else if (MaskType[i] == MASK_4ORD_ASTIGMATISM) {
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), intRound(angle(Surface.m_c[11], Surface.m_c[13]) * _180_Pi / 2.0) % 180);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
		}
	}

	// notes
	t += h;
	s = G_ReadChinese(pExam->m_Header.m_Note) + G_ReadChinese(pExam->m_Header.m_AutoNote);

	int l = s.GetLength();
	while (l > 0 && n < NumRows) 
	{
		int i;
		for (i = 1; i <= l; i++) 
		{
			if (s[i - 1] == '\n') break;
			m_MemDC.MeasureRect(" " + s.Left(i) + " ", Rect, Font);
			if (Rect.right - Rect.left > R - L)
			{
				for (; i >= 1; i--)
				{
					if (s[i - 1] == ' ') break;
				}
				break;
			}
		}

		t += h;
		if (Rows[n++])
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText(" " + s.Left(i) + " ", Rect, Font, white, 0);
		}

		s = s.Right(l - i);
		l -= i;
	}

}

//******************************************************************************************
//521 Solo Refraction Text Info
void CSumWnd::WFSoloDoubleExamInfo(CWFExam* pExam, RECT Rect1, RECT Rect2, RECT Rect3, RECT Rect4, BOOL HighLight)
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;
	COLORREF red = m_Printing ? RED : 0x002f2fff;
	COLORREF blue = m_Printing ? BLUE : 0x00ff2f2f;
	COLORREF yellow = m_Printing ? ORANGE : YELLOW;

	CString s;
	real_t SphEq, Sph, Cyl; int Axis;
	CZernikeSurface Surface;

	int W1 = Rect1.right - Rect1.left;
	int H1 = Rect1.bottom - Rect1.top;

	CMFont Font1(intRound(0.8*H1), 400, "Arial");

	COLORREF Color;
	if (pExam->m_Complaints.m_NightMiopia == COMP_SEVERE ||
		pExam->m_Complaints.m_NightHyperopia == COMP_SEVERE ||
		pExam->m_Complaints.m_Astigmatism == COMP_SEVERE ||
		pExam->m_Complaints.m_BlurDoubleVision == COMP_SEVERE ||
		pExam->m_Complaints.m_GlareHalo == COMP_SEVERE ||
		pExam->m_Complaints.m_MixedFocus == COMP_SEVERE)
	{
		Color = RED;
	}
	else if (pExam->m_Complaints.m_NightMiopia == COMP_MODERATE ||
		pExam->m_Complaints.m_NightHyperopia == COMP_MODERATE ||
		pExam->m_Complaints.m_Astigmatism == COMP_MODERATE ||
		pExam->m_Complaints.m_BlurDoubleVision == COMP_MODERATE ||
		pExam->m_Complaints.m_GlareHalo == COMP_MODERATE ||
		pExam->m_Complaints.m_MixedFocus == COMP_MODERATE)
	{
		Color = m_Printing ? ORANGE : YELLOW;
	}
	else
	{
		Color = GREEN;
	}

	Surface = pExam->m_WfSurface;
	Surface.ChangeRMaxUm(2000.0);
	CWFExam::GetSpheqSphCylAxis(Surface, ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
	//
	if (::Settings.m_RefractionRounding == 0)
	{
		Sph = round8(Sph);
		Cyl = round8(Cyl);
	}
	s.Format(_T("%+.2f D %+.2f D x %i° "), Sph, Cyl, Axis);

	m_MemDC.WriteText(" Tracey Refraction        " + s, Rect1, Font1, Color, 2);


	//Info text in the Rect2 CMFont SmallFont(intRound(0.8 * h), 400, "Arial");
	CString s1, s2, s3;

	int L = Rect2.left, T = Rect2.top, R = Rect2.right, B = Rect2.bottom;

	real_t t = T;
	real_t h = (Rect2.bottom - Rect2.top) / (real_t)13.33;

	CMFont Font(intRound(0.92*h), 400, "Arial");
	CMFont BigFont(intRound(1.1 * h), 400, "Arial");
	//CMFont BoldFont(intRound(1.1 * h), FW_BOLD, "Arial");//[5.1.1]

	int n = 0;

	// date / time
	t += h;
	t += h;
	::SetRect(&Rect2, L, intRound(t), R, intRound(t + h + 1));
	s.Format(_T("%s  %02i:%02i:%02i"),
		::DateToStr(pExam->m_Header.m_Year, pExam->m_Header.m_Month, pExam->m_Header.m_Day), pExam->m_Header.m_Hour, pExam->m_Header.m_Minute, pExam->m_Header.m_Second);
	m_MemDC.WriteText(s, Rect2, BigFont, white, 0);

	t += h;
	t += h;
	::SetRect(&Rect2, L, intRound(t), R, intRound(t + h + 1));
	m_MemDC.WriteText(" Limbus ", Rect2, BigFont, white, 0);
	if (pExam->m_Image.m_li_ok) {
		s1.Format(_T("%.2f mm"), pExam->m_Image.m_li_r_mean_um * 0.002);
	}
	else {
		s1 = "---";
	}
	m_MemDC.WriteText(s1, Rect2, BigFont, white, 2);

	t += h;
	::SetRect(&Rect2, L, intRound(t), R, intRound(t + h + 1));
	m_MemDC.WriteText(" Pupil ", Rect2, BigFont, white, 0);
	if (pExam->m_Image.m_li_ok) {
		s2.Format(_T("%.2f mm"), pExam->m_Image.m_pu_r_mean_um * 0.002);
	}
	else {
		s2 = "---";
	}
	m_MemDC.WriteText(s2, Rect2, BigFont, white, 2);

	t += h;
	::SetRect(&Rect2, L, intRound(t), R, intRound(t + h + 1));
	m_MemDC.WriteText(" Scan ", Rect2, BigFont, white, 0);
	s3.Format(_T("%.2f mm"), pExam->m_ScanDiameter * 0.001);
	m_MemDC.WriteText(s3, Rect2, BigFont, white, 2);


	//Add the note here
	t += h;
	// notes
	s = G_ReadChinese(pExam->m_Header.m_Note) + G_ReadChinese(pExam->m_Header.m_AutoNote);

	int l = s.GetLength();

	while (l > 0)
	{
		int i;
		for (i = 1; i <= l; i++)
		{
			if (s[i - 1] == '\n') break;
			m_MemDC.MeasureRect(" " + s.Left(i) + " ", Rect2, Font);
			if (Rect2.right - Rect2.left > R - L)
			{
				break;
			}
		}

		t += h;
		{
			::SetRect(&Rect2, L, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText(" " + s.Left(i) + " ", Rect2, Font, white, 0);
		}

		s = s.Right(l - i);
		l -= i;
	}
	//


	//
	if (HighLight)
		m_MemDC.DrawRectangle(Rect3, white, NOCOLOR);
	else m_MemDC.DrawRectangle(Rect3, gray, NOCOLOR);

	//Ho Total //AAA
	L = Rect3.left, T = Rect3.top, R = Rect3.right, B = Rect3.bottom;
	t = T + intRound((real_t)h / (real_t)2); ;
	::SetRect(&Rect3, L, intRound(t), R, intRound(t + h + 1));
	//  
	s = " Higher-Order Total";
	m_MemDC.WriteText(s, Rect3, BigFont, white, 0);

	// microns
	int MaskType = MASK_HO_TOTAL;
	CZernikeMask Mask;
	Mask.SetType(MaskType);
	Surface = pExam->m_WfSurface;
	Surface &= Mask;
	real_t rms = Surface.ComputeRms();
	s.Format(_T("%6.3f µ "), rms);
	m_MemDC.WriteText(s, Rect3, BigFont, white, 2);

	//Complaints	
	t += h;
	t += h;
	t += h;

	::SetRect(&Rect3, L, intRound(t), R, intRound(t + h + 1));
	m_MemDC.WriteText(" Potential Visual Complaints", Rect3, BigFont, white, 0);

	CString Name[6] =
	{
		"     Night Myopia",
		"     Night Hyperopia",
		"     Blur / Double Vision",
		"     Glare / Halo",
		"     Mixed Focus",
		"     Starburst"
	};
	int *A[6] =
	{
		&pExam->m_Complaints.m_NightMiopia,
		&pExam->m_Complaints.m_NightHyperopia,
		&pExam->m_Complaints.m_BlurDoubleVision,
		&pExam->m_Complaints.m_GlareHalo,
		&pExam->m_Complaints.m_MixedFocus,
		&pExam->m_Complaints.m_Starburst
	};

	for (int i = 0; i < 6; i++)
	{
		if (*A[i] != COMP_NORMAL)
		{
			t += h;
			::SetRect(&Rect3, L, intRound(t), R, intRound(t + h + 1));
			if (*A[i] == COMP_SEVERE)
			{
				Color = RED;
				s = "+++ ";
			}
			else if (*A[i] == COMP_MODERATE)
			{
				Color = m_Printing ? ORANGE : YELLOW;
				s = "++ ";
			}
			else if (*A[i] == COMP_LIGHT)
			{
				Color = white;
				s = "+ ";
			}
			m_MemDC.WriteText(Name[i], Rect3, BigFont, Color, 0);
			m_MemDC.WriteText(s, Rect3, BigFont, Color, 2);
		}
	}

	if (pExam->m_Image.m_pu_r_mean_um < 1250.0)
	{
		t += h;
		::SetRect(&Rect3, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText("     Small pupil", Rect3, BigFont, white, 0);
		t += h;
		::SetRect(&Rect3, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText("     Consider pinhole effect", Rect3, BigFont, white, 0);
	}
	//

	//
	// refraction
	CMFont Font2(intRound(0.8*h), 400, "Arial");

	L = Rect4.left, T = Rect4.top, R = Rect4.right, B = Rect4.bottom;

	t = T + h;

	::SetRect(&Rect4, L, intRound(t), R, intRound(t + h + 1));
	//  
	s = "Multi-zone refraction";
	m_MemDC.WriteText(s, Rect4, BigFont, white, 0);

	s.Format(_T(" (VD = %.2f mm)"), 0.001 * ::Settings.m_VertexDistanceUm);
	m_MemDC.WriteText(s, Rect4, Font2, white, 2);

	t += h;
	m_MemDC.MeasureRect("+0.00 D", Rect4, BigFont);
	int w1 = Rect4.right - Rect4.left;

	m_MemDC.MeasureRect("+00.00 D x ", Rect4, BigFont);
	int w2 = Rect4.right - Rect4.left;

	m_MemDC.MeasureRect("000° ", Rect4, BigFont);
	int w3 = Rect4.right - Rect4.left;

	m_MemDC.MeasureRect("@%.2f mm", Rect4, BigFont);
	int w4 = Rect4.right - Rect4.left;

	for (int i = 0; i < 4; i++)
	{
		t += h;
		//
		Surface = pExam->m_WfSurface;
		//
		real_t r_max_um = i < 3 ? 0.5 * ::Settings.m_ZoneDiameterUm[i] : Surface.m_r_max_um;
		if (r_max_um <= Surface.m_r_max_um)
		{
			//
			Surface.ChangeRMaxUm(r_max_um);
			CWFExam::GetSpheqSphCylAxis(Surface, ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
			// sphere
			::SetRect(&Rect4, L, intRound(t), L + w1, intRound(t + h + 1));
			s.Format(_T("%+.2f D "), Sph);
			m_MemDC.WriteText(s, Rect4, BigFont, white, 1);
			// cylinder
			::SetRect(&Rect4, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
			s.Format(_T("%+.2f D x "), Cyl);
			m_MemDC.WriteText(s, Rect4, BigFont, white, 2);
			// axis
			::SetRect(&Rect4, L + w1 + w2, intRound(t), L + w1 + w2 + w3, intRound(t + h + 1));
			s.Format(_T("%i° "), Axis);
			m_MemDC.WriteText(s, Rect4, BigFont, white, 2);
			// zone size
			::SetRect(&Rect4, L + w1 + w2 + w3, intRound(t), L + w1 + w2 + w3 + w4, intRound(t + h + 1));
			s.Format(_T("@%.2f mm "), 0.002 * Surface.m_r_max_um);
			m_MemDC.WriteText(s, Rect4, BigFont, white, 2);
		}
	}

	//Done
}

//***************************************************************************************
//530 To show simplied ct info for Chang Analysis
void CSumWnd::ChangCTExamInfo(CCTExam* pExam, RECT Rect)
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;
	COLORREF red = m_Printing ? RED : 0x002f2fff;
	COLORREF blue = m_Printing ? BLUE : 0x00ff2f2f;
	COLORREF yellow = m_Printing ? ORANGE : YELLOW;

	m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);

	CString s, s1, s2;

	int L = Rect.left + 3, T = Rect.top + 3, R = Rect.right - 3, B = Rect.bottom;

	real_t t = T;
	int  divInt = 15;
	if (pExam->m_OK && pExam->Keratoconus()) divInt = 16;
	real_t h = (Rect.bottom - Rect.top) / divInt;
	real_t smallh = 4 * h / 5.0;

	CMFont Font(intRound(0.8 * 1.1 * h), 400, "Arial");
	CMFont SmallFont(intRound(0.7 * 1.1 * h), 400, "Arial");
	CMFont VerySmallFont(intRound(0.5 * 1.1 * h), 400, "Arial");
	CMFont BigFont(intRound(1.5 * 1.1 * h), 400, "Arial");

	{
		::SetRect(&Rect, L, intRound(t + 5), R - 25, intRound(t + h + 10));
		s = pExam->m_Header.Eye(); s += " ";
		m_MemDC.WriteText(s, Rect, BigFont, pExam->m_Header.IsEyeManuallyOverriden() ? RED : white, 2);
		s.Format(_T("  %s  %02i:%02i:%02i"),
			::DateToStr(pExam->m_Header.m_Year, pExam->m_Header.m_Month, pExam->m_Header.m_Day), pExam->m_Header.m_Hour, pExam->m_Header.m_Minute, pExam->m_Header.m_Second);

		if (pExam->m_Header.m_Mode == EXAM_MODE_MANUAL)
		{
			int w;
			m_MemDC.MeasureRect("     %s  %02i:%02i:%02i", Rect, Font);
			w = Rect.right - Rect.left;

			::SetRect(&Rect, L, intRound(t), L + w, intRound(t + h + 1));
			m_MemDC.WriteText(s, Rect, Font, white, 0);

			::SetRect(&Rect, L + w, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("Manual", Rect, Font, RED, 0);
		}
		else  m_MemDC.WriteText(s, Rect, Font, white, 0);
	}


	// clinic / physician / operator
	int r1 = intRound(real_t(R - L - 10) / 3.0);
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), L + r1, intRound(t + h + 1));
		CClinic* pClinic = ::DB.GetClinic(pExam->m_Header.m_ClinicID);
		if (pClinic)
		{
			//m_MemDC.WriteText(" Clinic", Rect, SmallFont, white, 0);
			m_MemDC.WriteText("    " + G_ReadChinese(pClinic->m_Name), Rect, VerySmallFont, white, 0);
		}

		::SetRect(&Rect, L + r1, intRound(t), L + r1 + r1, intRound(t + h + 1));
		CPhysician* pPhysician = ::DB.GetPhysician(pExam->m_Header.m_PhysicianID);
		if (pPhysician)
		{
			//m_MemDC.WriteText(" Physician", Rect, SmallFont, white, 0);
			m_MemDC.WriteText(G_ReadChinese(pPhysician->FullName()) + " ", Rect, VerySmallFont, white, 0);
		}

		::SetRect(&Rect, L + r1 + r1, intRound(t), L + r1 + r1 + r1, intRound(t + h + 1));

		COperator* pOperator = ::DB.GetOperator(pExam->m_Header.m_OperatorID);
		if (pOperator)
		{
			//m_MemDC.WriteText(" Operator", Rect, SmallFont, white, 0);
			m_MemDC.WriteText(G_ReadChinese(pOperator->FullName()) + " ", Rect, VerySmallFont, white, 0);
		}
	}


	// refractive power or Sim k
	if (::Settings.m_ShowSimK)
	{
		t += h;
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("  Sim K @ 3.00 mm", Rect, Font, white, 0);
		}

		int w1, w2, w3, w4;
		m_MemDC.MeasureRect("0.00 mm / ", Rect, Font);
		w2 = Rect.right - Rect.left;
		m_MemDC.MeasureRect("00.00 D ", Rect, Font);
		w3 = Rect.right - Rect.left;
		m_MemDC.MeasureRect("x 000° ", Rect, Font);
		w4 = Rect.right - Rect.left;
		w1 = (R - L) - (w2 + w3 + w4);

		// steep
		t += h;
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("          Steep", Rect, SmallFont, red, 0);
			if (pExam->m_OK && pExam->m_sk_ok) {
				//
				::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
				s.Format(_T("%.2f mm / "), 0.001 * pExam->m_sk_st_um);
				m_MemDC.WriteText(s, Rect, SmallFont, red, 2);
				//
				::SetRect(&Rect, L + w1 + w2, intRound(t), L + w1 + w2 + w3, intRound(t + h + 1));
				s.Format(_T("%.2f D "), pExam->m_sk_st_dp);
				m_MemDC.WriteText(s, Rect, SmallFont, red, 2);
				//
				::SetRect(&Rect, L + w1 + w2 + w3, intRound(t), R, intRound(t + h + 1));
				m_MemDC.WriteText("x", Rect, SmallFont, red, 0);
				s.Format(_T("%i° "), pExam->m_sk_st_dg);
				m_MemDC.WriteText(s, Rect, SmallFont, red, 2);
			}
		}

		// flat
		t += h;
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("          Flat", Rect, SmallFont, blue, 0);
			if (pExam->m_OK && pExam->m_sk_ok) {
				//
				::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
				s.Format(_T("%.2f mm / "), 0.001 * pExam->m_sk_fl_um);
				m_MemDC.WriteText(s, Rect, SmallFont, blue, 2);
				//
				::SetRect(&Rect, L + w1 + w2, intRound(t), L + w1 + w2 + w3, intRound(t + h + 1));
				s.Format(_T("%.2f D "), pExam->m_sk_fl_dp);
				m_MemDC.WriteText(s, Rect, SmallFont, blue, 2);
				//
				::SetRect(&Rect, L + w1 + w2 + w3, intRound(t), R, intRound(t + h + 1));
				m_MemDC.WriteText("x", Rect, SmallFont, blue, 0);
				s.Format(_T("%i° "), pExam->m_sk_fl_dg);
				m_MemDC.WriteText(s, Rect, SmallFont, blue, 2);
			}
		}

		// delta
		t += h;
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("          Delta", Rect, SmallFont, white, 0);

			if (pExam->m_OK && pExam->m_sk_ok)
			{
				//
				::SetRect(&Rect, L + w1 + w2, intRound(t), L + w1 + w2 + w3, intRound(t + h + 1));
				s.Format(_T("%.2f D "), pExam->m_sk_df_dp);
				m_MemDC.WriteText(s, Rect, SmallFont, white, 2);
				//
				::SetRect(&Rect, L + w1 + w2 + w3, intRound(t), R, intRound(t + h + 1));
				m_MemDC.WriteText("x", Rect, SmallFont, white, 0);
				s.Format(_T("%i° "), pExam->m_sk_st_dg);
				m_MemDC.WriteText(s, Rect, SmallFont, white, 2);
			}
		}
	}
	else
	{
		t += h;
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("  Refractive Power @ D <= 3.00 mm", Rect, Font, white, 0);
		}

		int w1, w2, w3;
		m_MemDC.MeasureRect("00.00 D ", Rect, SmallFont);
		w2 = Rect.right - Rect.left;
		m_MemDC.MeasureRect("x 000° ", Rect, SmallFont);
		w3 = Rect.right - Rect.left;
		w1 = (R - L) - (w2 + w3);

		// steep
		t += smallh;
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("          Steep", Rect, SmallFont, red, 0);
			if (pExam->m_OK && pExam->m_rf_ok) {
				//
				::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
				s.Format(_T("%.2f D "), pExam->m_rf_st_dp);
				m_MemDC.WriteText(s, Rect, SmallFont, red, 2);
				//
				::SetRect(&Rect, L + w1 + w2, intRound(t), R, intRound(t + h + 1));
				m_MemDC.WriteText("x", Rect, Font, red, 0);
				s.Format(_T("%i° "), pExam->m_rf_st_dg);
				m_MemDC.WriteText(s, Rect, SmallFont, red, 2);
			}
		}
		// flat
		t += smallh;
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("          Flat", Rect, SmallFont, blue, 0);
			if (pExam->m_OK && pExam->m_rf_ok) {
				//
				::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
				s.Format(_T("%.2f D "), pExam->m_rf_fl_dp);
				m_MemDC.WriteText(s, Rect, SmallFont, blue, 2);
				//
				::SetRect(&Rect, L + w1 + w2, intRound(t), R, intRound(t + h + 1));
				m_MemDC.WriteText("x", Rect, Font, blue, 0);
				s.Format(_T("%i° "), pExam->m_rf_fl_dg);
				m_MemDC.WriteText(s, Rect, SmallFont, blue, 2);
			}
		}
		// Astigmatism
		t += smallh;
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("          Astigmatism", Rect, SmallFont, white, 0);
			if (pExam->m_OK && pExam->m_rf_ok) {
				//
				::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
				s.Format(_T("%.2f D "), pExam->m_rf_df_dp);
				m_MemDC.WriteText(s, Rect, SmallFont, white, 2);
				//
				::SetRect(&Rect, L + w1 + w2, intRound(t), R, intRound(t + h + 1));
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), pExam->m_rf_st_dg);
				m_MemDC.WriteText(s, Rect, SmallFont, white, 2);
			}
		}
	}

	t += smallh;
	t += smallh;

	//effective
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		//m_MemDC.WriteText("     Effective", Rect, Font, yellow, 0); 	
		m_MemDC.WriteText("  Effective RP", Rect, Font, white, 0); //[5.1.1]

		if (pExam->m_OK && pExam->m_rf_ok) {
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			s.Format(_T("%.2f D "), pExam->m_rf_av_dp);

			//m_MemDC.WriteText(s, Rect, Font, yellow, 2);
			m_MemDC.WriteText(s, Rect, Font, white, 2);//[5.1.1]
		}
	}

	// central radius / power
	t += h;
	t += (int)(0.8*h);
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));

		if (pExam->m_OK)
		{
			COLORREF thisColor = white;
			if (pExam->Keratoconus()) thisColor = red;
			else
			{
				if (pExam->m_ax0_dp > 47.2)  thisColor = yellow;
				else                         thisColor = white;
			}

			m_MemDC.WriteText("  Central Radius / Power", Rect, Font, thisColor, 0);

			s.Format(_T("%.2f mm / %.2f D "), 0.001 * pExam->m_ax0_um, pExam->m_ax0_dp);
			m_MemDC.WriteText(s, Rect, Font, thisColor, 2);
		}
	}

	// spherical aberration
	t += h;
	t += (int)(0.8*h);
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText("  Corneal SphAb @ D = 6.00 mm", Rect, Font, white, 0);
		if (pExam->m_OK) {
			//s.Format(_T("%+.3f µ "), fabs(pExam->m_sph_6mm));
			s.Format(_T("%.3f µ "), pExam->m_sph_6mm);//6.2.0
			m_MemDC.WriteText(s, Rect, Font, white, 2);
		}
	}

	// I-S
	t += h;
	t += (int)(0.8*h);
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));

		if (pExam->m_OK && pExam->m_is_ok)
		{
			COLORREF thisColor = white;
			if (pExam->Keratoconus()) thisColor = red;
			else
			{
				if (pExam->m_is_df_dp > 1.4)  thisColor = yellow;
				else                          thisColor = white;
			}

			m_MemDC.WriteText("  I-S Axial Power @ D = 6.00 mm", Rect, Font, thisColor, 0);
			if (pExam->m_OK && pExam->m_is_ok)
			{
				s.Format(_T("%.2f D "), pExam->m_is_df_dp);
				if (s == "-0.00 D " || s == "0.00 D ")
				{
					s = "0.00 D ";
					m_MemDC.WriteText(s, Rect, Font, thisColor, 2);
				}
				else
				{
					m_MemDC.WriteText(s, Rect, Font, thisColor, 2);
				}
			}
		}
	}

	// keratoconus 
	if (pExam->m_OK && pExam->Keratoconus())
	{
		t += h;
		t += (int)(0.5*h);
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText("  Suspect Keratoconus?", Rect, Font, red, 0);
	}

	// // notes
	t += (int)(0.8*h);
	int nn = 0;
	s = " " + G_ReadChinese(pExam->m_Header.m_Note);
	
	int l = s.GetLength();
	while (l > 0 && nn < 1)
	{
		int i;
		for (i = 1; i <= l; i++)
		{
			if (s[i - 1] == '\n') break;
			m_MemDC.MeasureRect(" " + s.Left(i) + " ", Rect, SmallFont);
			if (Rect.right - Rect.left > R - L)
			{
				for (; i >= 1; i--)
				{
					if (s[i - 1] == ' ') break;
				}
				break;
			}
		}

		t += h;
		nn++;
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText(" " + s.Left(i) + " ", Rect, SmallFont, white, 0);
		}

		s = s.Right(l - i);
		l -= i;
	}
}

//***************************************************************************************

void CSumWnd::CTExamInfo(CCTExam* pExam, RECT Rect, int NumRows, BOOL* Rows)
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;
	COLORREF red = m_Printing ? RED : 0x002f2fff;
	COLORREF blue = m_Printing ? BLUE : 0x00ff2f2f;
	COLORREF yellow = m_Printing ? ORANGE : YELLOW;

	m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);

	CString s, s1, s2;

	int L = Rect.left + 3, T = Rect.top + 3, R = Rect.right - 3, B = Rect.bottom;

	real_t t = T;
	real_t h = (Rect.bottom - Rect.top - 5) / ((real_t)NumRows + 0.5);

	CMFont Font(intRound(1.1 * h), 400, "Arial");
	CMFont BigFont(intRound(1.5 * 1.1 * h), 400, "Arial");
	//CMFont BoldFont(intRound(1.1 * h), FW_BOLD, "Arial");//[5.1.1]

	int n = 0;

	// date / time, od/os
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R - 25, intRound(t + h + 1));
		s = pExam->m_Header.Eye(); s += " ";
		m_MemDC.WriteText(s, Rect, BigFont, pExam->m_Header.IsEyeManuallyOverriden() ? RED : white, 2);
		s.Format(_T(" %s  %02i:%02i:%02i"),
			::DateToStr(pExam->m_Header.m_Year, pExam->m_Header.m_Month, pExam->m_Header.m_Day), pExam->m_Header.m_Hour, pExam->m_Header.m_Minute, pExam->m_Header.m_Second);

		if (pExam->m_Header.m_Mode == EXAM_MODE_MANUAL)
		{
			int w;
			m_MemDC.MeasureRect("     %s  %02i:%02i:%02i", Rect, Font);
			w = Rect.right - Rect.left;

			::SetRect(&Rect, L, intRound(t), L + w, intRound(t + h + 1));
			m_MemDC.WriteText(s, Rect, Font, white, 0);

			::SetRect(&Rect, L + w, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("Manual", Rect, Font, RED, 0);
		}
		else  m_MemDC.WriteText(s, Rect, Font, white, 0);
	}

	// clinic
	t += 5;
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));

		CClinic* pClinic = ::DB.GetClinic(pExam->m_Header.m_ClinicID);
		if (pClinic) {
			m_MemDC.WriteText(" Clinic", Rect, Font, white, 0);
			m_MemDC.WriteText(G_ReadChinese(pClinic->m_Name) + " ", Rect, Font, white, 2);
		}
		else
			m_MemDC.WriteText(" Clinic", Rect, Font, gray, 0);
	}

	// physician
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));

		CPhysician* pPhysician = ::DB.GetPhysician(pExam->m_Header.m_PhysicianID);
		if (pPhysician) {
			m_MemDC.WriteText(" Physician", Rect, Font, white, 0);
			m_MemDC.WriteText(G_ReadChinese(pPhysician->FullName()) + " ", Rect, Font, white, 2);
		}
		else
			m_MemDC.WriteText(" Physician", Rect, Font, gray, 0);
	}

	// operator
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));

		COperator* pOperator = ::DB.GetOperator(pExam->m_Header.m_OperatorID);
		if (pOperator) {
			m_MemDC.WriteText(" Operator", Rect, Font, white, 0);
			m_MemDC.WriteText(G_ReadChinese(pOperator->FullName()) + " ", Rect, Font, white, 2);
		}
		else  m_MemDC.WriteText(" Operator", Rect, Font, gray, 0);
	}

	// limbus / pupil
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Limbus / Pupil", Rect, Font, white, 0);
		if (pExam->m_Image.m_li_ok) {
			s1.Format(_T("%.2f"), pExam->m_Image.m_li_r_mean_um * 0.002);
		}
		else {
			s1 = "---";//CT
		}
		if (pExam->m_Image.m_pu_ok) {
			s2.Format(_T("%.2f"), pExam->m_Image.m_pu_r_mean_um * 0.002);
		}
		else {
			s2 = "---";
		}
		s = s1 + " / " + s2 + " mm ";
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	// refractive power
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Refractive Power @ D <= 3.00 mm", Rect, Font, white, 0);
	}

	int w1, w2, w3, w4;
	m_MemDC.MeasureRect("00.00 D ", Rect, Font);
	w2 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("x 000° ", Rect, Font);
	w3 = Rect.right - Rect.left;
	w1 = (R - L) - (w2 + w3);

	// steep
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText("     Steep", Rect, Font, red, 0);
		if (pExam->m_OK && pExam->m_rf_ok) {
			//
			::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
			s.Format(_T("%.2f D "), pExam->m_rf_st_dp);
			m_MemDC.WriteText(s, Rect, Font, red, 2);
			//
			::SetRect(&Rect, L + w1 + w2, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("x", Rect, Font, red, 0);
			s.Format(_T("%i° "), pExam->m_rf_st_dg);
			m_MemDC.WriteText(s, Rect, Font, red, 2);
		}
	}
	// flat
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText("     Flat", Rect, Font, blue, 0);
		if (pExam->m_OK && pExam->m_rf_ok) {
			//
			::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
			s.Format(_T("%.2f D "), pExam->m_rf_fl_dp);
			m_MemDC.WriteText(s, Rect, Font, blue, 2);
			//
			::SetRect(&Rect, L + w1 + w2, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("x", Rect, Font, blue, 0);
			s.Format(_T("%i° "), pExam->m_rf_fl_dg);
			m_MemDC.WriteText(s, Rect, Font, blue, 2);
		}
	}
	// delta
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText("     Astigmatism", Rect, Font, white, 0);
		if (pExam->m_OK && pExam->m_rf_ok) {
			//
			::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
			s.Format(_T("%.2f D "), pExam->m_rf_df_dp);
			m_MemDC.WriteText(s, Rect, Font, white, 2);
			//
			::SetRect(&Rect, L + w1 + w2, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("x", Rect, Font, white, 0);
			s.Format(_T("%i° "), pExam->m_rf_st_dg);
			m_MemDC.WriteText(s, Rect, Font, white, 2);
		}
	}

	// average

	//effective
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		//m_MemDC.WriteText("     Effective", Rect, Font, yellow, 0); 	
		m_MemDC.WriteText("     Effective", Rect, Font, white, 0); //[5.1.1]

		if (pExam->m_OK && pExam->m_rf_ok) {
			::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
			s.Format(_T("%.2f D "), pExam->m_rf_av_dp);

			//m_MemDC.WriteText(s, Rect, Font, yellow, 2);
			m_MemDC.WriteText(s, Rect, Font, white, 2);//[5.1.1]

			CMDC DC;
			DC.CreateCompatibleDC(NULL);
			DC.MeasureRect("     ", Rect, Font);
			int cw = Rect.right - Rect.left - 3;

			::SetRect(&Rect, L + cw, intRound(t), L + w1 + w2, intRound(t + h + 1));//[5.1.1]
			m_MemDC.DrawRectangle(Rect, white, NOCOLOR);//[5.1.1]
		}
	}

	// SimK
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Sim K @ D = 3.00 mm", Rect, Font, white, 0);
	}

	m_MemDC.MeasureRect("0.00 mm / ", Rect, Font);
	w2 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("00.00 D ", Rect, Font);
	w3 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("x 000° ", Rect, Font);
	w4 = Rect.right - Rect.left;
	w1 = (R - L) - (w2 + w3 + w4);

	// steep
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText("     Steep", Rect, Font, red, 0);
		if (pExam->m_OK && pExam->m_sk_ok) {
			//
			::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
			s.Format(_T("%.2f mm / "), 0.001 * pExam->m_sk_st_um);
			m_MemDC.WriteText(s, Rect, Font, red, 2);
			//
			::SetRect(&Rect, L + w1 + w2, intRound(t), L + w1 + w2 + w3, intRound(t + h + 1));
			s.Format(_T("%.2f D "), pExam->m_sk_st_dp);
			m_MemDC.WriteText(s, Rect, Font, red, 2);
			//
			::SetRect(&Rect, L + w1 + w2 + w3, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("x", Rect, Font, red, 0);
			s.Format(_T("%i° "), pExam->m_sk_st_dg);
			m_MemDC.WriteText(s, Rect, Font, red, 2);
		}
	}

	// flat
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText("     Flat", Rect, Font, blue, 0);
		if (pExam->m_OK && pExam->m_sk_ok) {
			//
			::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
			s.Format(_T("%.2f mm / "), 0.001 * pExam->m_sk_fl_um);
			m_MemDC.WriteText(s, Rect, Font, blue, 2);
			//
			::SetRect(&Rect, L + w1 + w2, intRound(t), L + w1 + w2 + w3, intRound(t + h + 1));
			s.Format(_T("%.2f D "), pExam->m_sk_fl_dp);
			m_MemDC.WriteText(s, Rect, Font, blue, 2);
			//
			::SetRect(&Rect, L + w1 + w2 + w3, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("x", Rect, Font, blue, 0);
			s.Format(_T("%i° "), pExam->m_sk_fl_dg);
			m_MemDC.WriteText(s, Rect, Font, blue, 2);
		}
	}

	// delta
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText("     Delta", Rect, Font, white, 0);
		if (pExam->m_OK && pExam->m_sk_ok) {
			//
			::SetRect(&Rect, L + w1 + w2, intRound(t), L + w1 + w2 + w3, intRound(t + h + 1));
			s.Format(_T("%.2f D "), pExam->m_sk_df_dp);
			m_MemDC.WriteText(s, Rect, Font, white, 2);
			//
			::SetRect(&Rect, L + w1 + w2 + w3, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("x", Rect, Font, white, 0);
			s.Format(_T("%i° "), pExam->m_sk_st_dg);
			m_MemDC.WriteText(s, Rect, Font, white, 2);
		}
	}

	// average
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		//m_MemDC.WriteText("     Average", Rect, Font, yellow, 0);
		m_MemDC.WriteText("     Average", Rect, Font, white, 0);

		if (pExam->m_OK && pExam->m_sk_ok) {
			//
			::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
			s.Format(_T("%.2f mm / "), 0.001 * pExam->m_sk_av_um);
			//m_MemDC.WriteText(s, Rect, Font, yellow, 2);
			m_MemDC.WriteText(s, Rect, Font, white, 2);//[5.1.1]

													   //
			::SetRect(&Rect, L + w1 + w2, intRound(t), L + w1 + w2 + w3, intRound(t + h + 1));
			s.Format(_T("%.2f D "), pExam->m_sk_av_dp);
			//m_MemDC.WriteText(s, Rect, Font, yellow, 2);
			m_MemDC.WriteText(s, Rect, Font, white, 2);//[5.1.1]

			CMDC DC;
			DC.CreateCompatibleDC(NULL);
			DC.MeasureRect("     ", Rect, Font);
			int cw = Rect.right - Rect.left - 3;
			::SetRect(&Rect, L + cw, intRound(t), L + w1 + w2 + w3, intRound(t + h + 1));//[5.1.1]
			m_MemDC.DrawRectangle(Rect, white, NOCOLOR);//[5.1.1]
		}
	}

	// central radius / power
	t += h;
	if (Rows[n++]) {
		if (pExam->m_OK)
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));

			COLORREF thisColor = white;
			if (pExam->Keratoconus()) thisColor = red;
			else
			{
				if (pExam->m_ax0_dp > 47.2)  thisColor = yellow;
				else                         thisColor = white;
			}

			m_MemDC.WriteText(" Central Radius / Power", Rect, Font, thisColor, 0);

			s.Format(_T("%.2f mm / %.2f D "), 0.001 * pExam->m_ax0_um, pExam->m_ax0_dp);
			m_MemDC.WriteText(s, Rect, Font, thisColor, 2);
		}
	}

	// best fit sphere
	t += h;
	if (Rows[n++])
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Best Fit Sphere", Rect, Font, white, 0);

		s.Format(_T("R0 = %.2f mm "), pExam->m_HtSpSurface.m_ax0_um * 0.001);
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	// best fit conicoid
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Best Fit Conicoid", Rect, Font, white, 0);
		if (pExam->m_OK) {
			s.Format(_T("R0 = %.2f mm Q = %.2f e = %.2f "), pExam->m_HtCnSurface.m_ax0_um * 0.001, pExam->m_HtCnSurface.m_q, pExam->m_HtCnSurface.GetEccentricity());
			m_MemDC.WriteText(s, Rect, Font, white, 2);
		}
	}

	// spherical aberration
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Corneal SphAb @ D = 6.00 mm", Rect, Font, white, 0);
		if (pExam->m_OK) {
			//s.Format(_T("%+.3f µ "), fabs(pExam->m_sph_6mm));
			s.Format(_T("%.3f µ "), pExam->m_sph_6mm);//6.2.0
			m_MemDC.WriteText(s, Rect, Font, white, 2);
		}
	}

	// I-S
	t += h;
	if (Rows[n++]) {
		if (pExam->m_OK && pExam->m_is_ok)
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			COLORREF thisColor = white;
			if (pExam->Keratoconus()) thisColor = red;
			else
			{
				if (pExam->m_is_df_dp > 1.4)  thisColor = yellow;
				else                          thisColor = white;
			}

			m_MemDC.WriteText(" I-S Axial Power @ D = 6.00 mm", Rect, Font, thisColor, 0);
			if (pExam->m_OK && pExam->m_is_ok)
			{
				s.Format(_T("%.2f D "), pExam->m_is_df_dp);
				if (s == "-0.00 D " || s == "0.00 D ")
				{
					s = "0.00 D ";
					m_MemDC.WriteText(s, Rect, Font, thisColor, 2);
				}
				else
				{
					m_MemDC.WriteText(s, Rect, Font, thisColor, 2);
				}
			}
		}
	}

	// keratoconus
	t += h;
	if (Rows[n++]) {
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		if (pExam->m_OK) {
			if (pExam->Keratoconus()) {
				m_MemDC.WriteText(" Suspect Keratoconus?", Rect, Font, RED, 0);
			}
		}
	}

	// notes
	s = G_ReadChinese(pExam->m_Header.m_Note);
	
	int l = s.GetLength();

	while (l > 0 && n < NumRows)
	{
		int i;
		for (i = 1; i <= l; i++) 
		{
			if (s[i - 1] == '\n') break;
			m_MemDC.MeasureRect(" " + s.Left(i) + " ", Rect, Font);
			if (Rect.right - Rect.left > R - L)
			{
				for (; i >= 1; i--)
				{
					if (s[i - 1] == ' ') break;
				}
				break;
			}
		}

		t += h;
		if (Rows[n++]) 
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText(" " + s.Left(i) + " ", Rect, Font, white, 0);
		}

		s = s.Right(l - i);
		l -= i;
	}

}


//***************************************************************************************
void CSumWnd::CTExamLargeInfo(CCTExam* pExam, RECT Rect, int NumRows, BOOL* Rows)
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;
	COLORREF red = m_Printing ? RED : 0x002f2fff;
	COLORREF blue = m_Printing ? BLUE : 0x00ff2f2f;
	COLORREF yellow = m_Printing ? ORANGE : YELLOW;

	m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);

	CString s, s1, s2;

	int L = Rect.left + 5, T = Rect.top + 5, R = Rect.right - 5, B = Rect.bottom;

	real_t t = T;
	real_t h = (Rect.bottom - Rect.top) / (real_t)33;

	CMFont Font(intRound(0.85 * h), 400, "Arial");
	CMFont BigFont(intRound(1.2 * 1.1 * h), 400, "Arial");

	//CMFont BoldFont(intRound(1.1 * h), FW_BOLD, "Arial");//[5.1.1]

	// date / time, od/os
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		s = pExam->m_Header.Eye(); s += " ";
		m_MemDC.WriteText(s, Rect, BigFont, pExam->m_Header.IsEyeManuallyOverriden() ? RED : white, 2);
		s.Format(_T(" %s  %02i:%02i:%02i"),
			::DateToStr(pExam->m_Header.m_Year, pExam->m_Header.m_Month, pExam->m_Header.m_Day), pExam->m_Header.m_Hour, pExam->m_Header.m_Minute, pExam->m_Header.m_Second);

		if (pExam->m_Header.m_Mode == EXAM_MODE_MANUAL)
		{
			int w;
			m_MemDC.MeasureRect("     %s  %02i:%02i:%02i", Rect, Font);
			w = Rect.right - Rect.left;

			::SetRect(&Rect, L, intRound(t), L + w, intRound(t + h + 1));
			m_MemDC.WriteText(s, Rect, Font, white, 0);

			::SetRect(&Rect, L + w, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("Manual", Rect, Font, RED, 0);
		}
		else  m_MemDC.WriteText(s, Rect, Font, white, 0);
	}

	// clinic
	t += h;
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		CClinic* pClinic = ::DB.GetClinic(pExam->m_Header.m_ClinicID);
		if (pClinic) {
			m_MemDC.WriteText(" Clinic", Rect, Font, white, 0);
			m_MemDC.WriteText(" " + G_ReadChinese(pClinic->m_Name) + " ", Rect, Font, white, 2);
		}
		else m_MemDC.WriteText(" Clinic", Rect, Font, gray, 0);
	}

	// physician
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		CPhysician* pPhysician = ::DB.GetPhysician(pExam->m_Header.m_PhysicianID);
		if (pPhysician) {
			m_MemDC.WriteText(" Physician", Rect, Font, white, 0);
			m_MemDC.WriteText(" " + G_ReadChinese(pPhysician->FullName()) + " ", Rect, Font, white, 2);
		}
		else m_MemDC.WriteText(" Physician", Rect, Font, gray, 0);
	}

	// operator
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		COperator* pOperator = ::DB.GetOperator(pExam->m_Header.m_OperatorID);
		if (pOperator) {
			m_MemDC.WriteText(" Operator", Rect, Font, white, 0);
			m_MemDC.WriteText(" " + G_ReadChinese(pOperator->FullName()) + " ", Rect, Font, white, 2);
		}
		else m_MemDC.WriteText(" Operator", Rect, Font, gray, 0);
	}

	// limbus / pupil
	t += h;
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Limbus / Pupil", Rect, Font, white, 0);
		if (pExam->m_Image.m_li_ok) {
			s1.Format(_T("%.2f"), pExam->m_Image.m_li_r_mean_um * 0.002);
		}
		else {
			s1 = "---";//CT
		}
		if (pExam->m_Image.m_pu_ok) {
			s2.Format(_T("%.2f"), pExam->m_Image.m_pu_r_mean_um * 0.002);
		}
		else {
			s2 = "---";
		}
		s = s1 + " / " + s2 + " mm ";
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	// refractive power
	t += h;
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Refractive Power @ D <= 3.00 mm", Rect, Font, white, 0);
	}

	int w1, w2, w3, w4;
	m_MemDC.MeasureRect("00.00 D ", Rect, Font);
	w2 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("x 000° ", Rect, Font);
	w3 = Rect.right - Rect.left;
	w1 = (R - L) - (w2 + w3);

	// steep
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText("     Steep", Rect, Font, red, 0);
		if (pExam->m_OK && pExam->m_rf_ok) {
			//
			::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
			s.Format(_T("%.2f D "), pExam->m_rf_st_dp);
			m_MemDC.WriteText(s, Rect, Font, red, 2);
			//
			::SetRect(&Rect, L + w1 + w2, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("x", Rect, Font, red, 0);
			s.Format(_T("%i° "), pExam->m_rf_st_dg);
			m_MemDC.WriteText(s, Rect, Font, red, 2);
		}
	}
	// flat
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText("     Flat", Rect, Font, blue, 0);
		if (pExam->m_OK && pExam->m_rf_ok) {
			//
			::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
			s.Format(_T("%.2f D "), pExam->m_rf_fl_dp);
			m_MemDC.WriteText(s, Rect, Font, blue, 2);
			//
			::SetRect(&Rect, L + w1 + w2, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("x", Rect, Font, blue, 0);
			s.Format(_T("%i° "), pExam->m_rf_fl_dg);
			m_MemDC.WriteText(s, Rect, Font, blue, 2);
		}
	}
	// delta
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText("     Astigmatism", Rect, Font, white, 0);
		if (pExam->m_OK && pExam->m_rf_ok) {
			//
			::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
			s.Format(_T("%.2f D "), pExam->m_rf_df_dp);
			m_MemDC.WriteText(s, Rect, Font, white, 2);
			//
			::SetRect(&Rect, L + w1 + w2, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("x", Rect, Font, white, 0);
			s.Format(_T("%i° "), pExam->m_rf_st_dg);
			m_MemDC.WriteText(s, Rect, Font, white, 2);
		}
	}

	// average

	//effective
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		//m_MemDC.WriteText("     Effective", Rect, Font, yellow, 0); 	
		m_MemDC.WriteText("     Effective", Rect, Font, white, 0); //[5.1.1]

		if (pExam->m_OK && pExam->m_rf_ok) {
			::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
			s.Format(_T("%.2f D "), pExam->m_rf_av_dp);

			//m_MemDC.WriteText(s, Rect, Font, yellow, 2);
			m_MemDC.WriteText(s, Rect, Font, white, 2);//[5.1.1]

			CMDC DC;
			DC.CreateCompatibleDC(NULL);
			DC.MeasureRect("     ", Rect, Font);
			int cw = Rect.right - Rect.left - 3;

			::SetRect(&Rect, L + cw, intRound(t), L + w1 + w2, intRound(t + h + 1));//[5.1.1]
			m_MemDC.DrawRectangle(Rect, white, NOCOLOR);//[5.1.1]
		}
	}

	// SimK
	t += h;
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Sim K @ D = 3.00 mm", Rect, Font, white, 0);
	}

	m_MemDC.MeasureRect("0.00 mm / ", Rect, Font);
	w2 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("00.00 D ", Rect, Font);
	w3 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("x 000° ", Rect, Font);
	w4 = Rect.right - Rect.left;
	w1 = (R - L) - (w2 + w3 + w4);

	// steep
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText("     Steep", Rect, Font, red, 0);
		if (pExam->m_OK && pExam->m_sk_ok) {
			//
			::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
			s.Format(_T("%.2f mm / "), 0.001 * pExam->m_sk_st_um);
			m_MemDC.WriteText(s, Rect, Font, red, 2);
			//
			::SetRect(&Rect, L + w1 + w2, intRound(t), L + w1 + w2 + w3, intRound(t + h + 1));
			s.Format(_T("%.2f D "), pExam->m_sk_st_dp);
			m_MemDC.WriteText(s, Rect, Font, red, 2);
			//
			::SetRect(&Rect, L + w1 + w2 + w3, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("x", Rect, Font, red, 0);
			s.Format(_T("%i° "), pExam->m_sk_st_dg);
			m_MemDC.WriteText(s, Rect, Font, red, 2);
		}
	}

	// flat
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText("     Flat", Rect, Font, blue, 0);
		if (pExam->m_OK && pExam->m_sk_ok) {
			//
			::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
			s.Format(_T("%.2f mm / "), 0.001 * pExam->m_sk_fl_um);
			m_MemDC.WriteText(s, Rect, Font, blue, 2);
			//
			::SetRect(&Rect, L + w1 + w2, intRound(t), L + w1 + w2 + w3, intRound(t + h + 1));
			s.Format(_T("%.2f D "), pExam->m_sk_fl_dp);
			m_MemDC.WriteText(s, Rect, Font, blue, 2);
			//
			::SetRect(&Rect, L + w1 + w2 + w3, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("x", Rect, Font, blue, 0);
			s.Format(_T("%i° "), pExam->m_sk_fl_dg);
			m_MemDC.WriteText(s, Rect, Font, blue, 2);
		}
	}

	// delta
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText("     Delta", Rect, Font, white, 0);
		if (pExam->m_OK && pExam->m_sk_ok) {
			//
			::SetRect(&Rect, L + w1 + w2, intRound(t), L + w1 + w2 + w3, intRound(t + h + 1));
			s.Format(_T("%.2f D "), pExam->m_sk_df_dp);
			m_MemDC.WriteText(s, Rect, Font, white, 2);
			//
			::SetRect(&Rect, L + w1 + w2 + w3, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("x", Rect, Font, white, 0);
			s.Format(_T("%i° "), pExam->m_sk_st_dg);
			m_MemDC.WriteText(s, Rect, Font, white, 2);
		}
	}

	// average
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		//m_MemDC.WriteText("     Average", Rect, Font, yellow, 0);
		m_MemDC.WriteText("     Average", Rect, Font, white, 0);

		if (pExam->m_OK && pExam->m_sk_ok) {
			//
			::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
			s.Format(_T("%.2f mm / "), 0.001 * pExam->m_sk_av_um);
			//m_MemDC.WriteText(s, Rect, Font, yellow, 2);
			m_MemDC.WriteText(s, Rect, Font, white, 2);//[5.1.1]

													   //
			::SetRect(&Rect, L + w1 + w2, intRound(t), L + w1 + w2 + w3, intRound(t + h + 1));
			s.Format(_T("%.2f D "), pExam->m_sk_av_dp);
			//m_MemDC.WriteText(s, Rect, Font, yellow, 2);
			m_MemDC.WriteText(s, Rect, Font, white, 2);//[5.1.1]

			CMDC DC;
			DC.CreateCompatibleDC(NULL);
			DC.MeasureRect("     ", Rect, Font);
			int cw = Rect.right - Rect.left - 3;
			::SetRect(&Rect, L + cw, intRound(t), L + w1 + w2 + w3, intRound(t + h + 1));//[5.1.1]
			m_MemDC.DrawRectangle(Rect, white, NOCOLOR);//[5.1.1]
		}
	}

	// central radius / power
	t += h;
	t += h;
	{
		if (pExam->m_OK)
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));

			COLORREF thisColor = white;
			if (pExam->Keratoconus()) thisColor = red;
			else
			{
				if (pExam->m_ax0_dp > 47.2)  thisColor = yellow;
				else                         thisColor = white;
			}

			m_MemDC.WriteText(" Central Radius / Power", Rect, Font, thisColor, 0);

			s.Format(_T("%.2f mm / %.2f D "), 0.001 * pExam->m_ax0_um, pExam->m_ax0_dp);
			m_MemDC.WriteText(s, Rect, Font, thisColor, 2);
		}
	}

	// best fit sphere
	t += h;
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Best Fit Sphere", Rect, Font, white, 0);
		if (pExam->m_OK) {
			s.Format(_T("R0 = %.2f mm "), pExam->m_HtSpSurface.m_ax0_um * 0.001);
			m_MemDC.WriteText(s, Rect, Font, white, 2);
		}
	}

	// best fit conicoid
	t += h;
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Best Fit Conicoid", Rect, Font, white, 0);
		if (pExam->m_OK) {
			s.Format(_T("R0 = %.2f mm Q = %.2f e = %.2f "), pExam->m_HtCnSurface.m_ax0_um * 0.001, pExam->m_HtCnSurface.m_q, pExam->m_HtCnSurface.GetEccentricity());
			m_MemDC.WriteText(s, Rect, Font, white, 2);
		}
	}

	// spherical aberration
	t += h;
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Corneal SphAb @ D = 6.00 mm", Rect, Font, white, 0);
		if (pExam->m_OK) {
			//s.Format(_T("%+.3f µ "), fabs(pExam->m_sph_6mm));
			s.Format(_T("%.3f µ "), pExam->m_sph_6mm);//6.2.0
			m_MemDC.WriteText(s, Rect, Font, white, 2);
		}
	}

	// I-S
	t += h;
	t += h;
	{
		if (pExam->m_OK && pExam->m_is_ok)
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));

			COLORREF thisColor = white;
			if (pExam->Keratoconus()) thisColor = red;
			else
			{
				if (pExam->m_is_df_dp > 1.4)  thisColor = yellow;
				else                          thisColor = white;
			}

			m_MemDC.WriteText(" I-S Axial Power @ D = 6.00 mm", Rect, Font, thisColor, 0);
			if (pExam->m_OK && pExam->m_is_ok)
			{
				s.Format(_T("%.2f D "), pExam->m_is_df_dp);
				if (s == "-0.00 D " || s == "0.00 D ")
				{
					s = "0.00 D ";
					m_MemDC.WriteText(s, Rect, Font, thisColor, 2);
				}
				else
				{
					m_MemDC.WriteText(s, Rect, Font, thisColor, 2);
				}
			}
		}
	}


	// keratoconus 
	if (pExam->m_OK && pExam->Keratoconus())
	{
		t += h;
		t += (int)(0.5*h);
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Suspect Keratoconus?", Rect, Font, red, 0);
	}

	// notes
	int nn = 0;
	t += h;
	s = G_ReadChinese(pExam->m_Header.m_Note);
	int l = s.GetLength();

	while (l > 0 && nn <= 2) 
	{
		int i;
		for (i = 1; i <= l; i++) 
		{
			if (s[i - 1] == '\n') break;
			m_MemDC.MeasureRect(" " + s.Left(i) + " ", Rect, Font);
			if (Rect.right - Rect.left > R - L) 
			{
				for (; i >= 1; i--)
				{
					if (s[i - 1] == ' ') break;
				}
				break;
			}
		}

		t += h;
		nn++;
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			CString testS = " " + s.Left(i) + " ";
			m_MemDC.WriteText(" " + s.Left(i) + " ", Rect, Font, white, 0);
		}

		s = s.Right(l - i);
		l -= i;
	}
}

//***************************************************************************************
//520 For show the CT info of OU overview Window  
void CSumWnd::OUCTExamInfo(CCTExam* pExam, RECT Rect)
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;
	COLORREF red = m_Printing ? RED : 0x002f2fff;
	COLORREF blue = m_Printing ? BLUE : 0x00ff2f2f;
	COLORREF yellow = m_Printing ? ORANGE : YELLOW;

	m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);

	CString s, s1, s2;

	int L = Rect.left + 3, T = Rect.top, R = Rect.right - 3, B = Rect.bottom;

	real_t t = T;
	real_t h = (Rect.bottom - Rect.top) / (real_t)9;

	CMFont Font(intRound(0.9 * h), 400, "Arial");
	//CMFont BoldFont(intRound(1.1 * h), FW_BOLD, "Arial");//[5.1.1]

	int n = 0;

	int w1, w2, w3, w4;
	m_MemDC.MeasureRect("00.00 D ", Rect, Font);
	w2 = Rect.right - Rect.left;
	m_MemDC.MeasureRect("x 000° ", Rect, Font);
	w3 = Rect.right - Rect.left;
	w1 = (R - L) - (w2 + w3);

	// date / time, od/os
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		s += " CT";

		//531
		if (pExam->m_Header.m_Mode == EXAM_MODE_MANUAL)
		{
			int w;
			m_MemDC.MeasureRect("  CT ", Rect, Font);
			w = Rect.right - Rect.left;

			::SetRect(&Rect, L, intRound(t), L + w, intRound(t + h + 1));
			m_MemDC.WriteText(s, Rect, Font, pExam->m_Header.IsEyeManuallyOverriden() ? RED : white, 0);

			::SetRect(&Rect, L + w, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("Manual", Rect, Font, RED, 0);
		}
		else   m_MemDC.WriteText(s, Rect, Font, pExam->m_Header.IsEyeManuallyOverriden() ? RED : white, 0);
		//531 Done

		s.Format(_T(" %s  %02i:%02i:%02i"),
			::DateToStr(pExam->m_Header.m_Year, pExam->m_Header.m_Month, pExam->m_Header.m_Day), pExam->m_Header.m_Hour, pExam->m_Header.m_Minute, pExam->m_Header.m_Second);
		m_MemDC.WriteText(s, Rect, Font, white, 2);
	}

	// SimK or Ref.K

	if (::Settings.m_ShowSimK)
	{
		t += h;
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText(" Sim K @ 3.00 mm", Rect, Font, white, 0);
		}

		m_MemDC.MeasureRect("0.00 mm / ", Rect, Font);
		w2 = Rect.right - Rect.left;
		m_MemDC.MeasureRect("00.00 D ", Rect, Font);
		w3 = Rect.right - Rect.left;
		m_MemDC.MeasureRect("x 000° ", Rect, Font);
		w4 = Rect.right - Rect.left;
		w1 = (R - L) - (w2 + w3 + w4);

		// steep
		t += h;
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("     Steep", Rect, Font, red, 0);
			if (pExam->m_OK && pExam->m_sk_ok) {
				//
				::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
				s.Format(_T("%.2f mm / "), 0.001 * pExam->m_sk_st_um);
				m_MemDC.WriteText(s, Rect, Font, red, 2);
				//
				::SetRect(&Rect, L + w1 + w2, intRound(t), L + w1 + w2 + w3, intRound(t + h + 1));
				s.Format(_T("%.2f D "), pExam->m_sk_st_dp);
				m_MemDC.WriteText(s, Rect, Font, red, 2);
				//
				::SetRect(&Rect, L + w1 + w2 + w3, intRound(t), R, intRound(t + h + 1));
				m_MemDC.WriteText("x", Rect, Font, red, 0);
				s.Format(_T("%i° "), pExam->m_sk_st_dg);
				m_MemDC.WriteText(s, Rect, Font, red, 2);
			}
		}

		// flat
		t += h;
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("     Flat", Rect, Font, blue, 0);
			if (pExam->m_OK && pExam->m_sk_ok) {
				//
				::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
				s.Format(_T("%.2f mm / "), 0.001 * pExam->m_sk_fl_um);
				m_MemDC.WriteText(s, Rect, Font, blue, 2);
				//
				::SetRect(&Rect, L + w1 + w2, intRound(t), L + w1 + w2 + w3, intRound(t + h + 1));
				s.Format(_T("%.2f D "), pExam->m_sk_fl_dp);
				m_MemDC.WriteText(s, Rect, Font, blue, 2);
				//
				::SetRect(&Rect, L + w1 + w2 + w3, intRound(t), R, intRound(t + h + 1));
				m_MemDC.WriteText("x", Rect, Font, blue, 0);
				s.Format(_T("%i° "), pExam->m_sk_fl_dg);
				m_MemDC.WriteText(s, Rect, Font, blue, 2);
			}
		}

		// delta
		t += h;
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("     Delta", Rect, Font, white, 0);
			if (pExam->m_OK && pExam->m_sk_ok) {
				//
				::SetRect(&Rect, L + w1 + w2, intRound(t), L + w1 + w2 + w3, intRound(t + h + 1));
				s.Format(_T("%.2f D "), pExam->m_sk_df_dp);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
				//
				::SetRect(&Rect, L + w1 + w2 + w3, intRound(t), R, intRound(t + h + 1));
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), pExam->m_sk_st_dg);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
		}
	}
	else
	{
		t += h;
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText(" Refractive Power @ D <= 3.00 mm", Rect, Font, white, 0);
		}

		int w1, w2, w3;
		m_MemDC.MeasureRect("00.00 D ", Rect, Font);
		w2 = Rect.right - Rect.left;
		m_MemDC.MeasureRect("x 000° ", Rect, Font);
		w3 = Rect.right - Rect.left;
		w1 = (R - L) - (w2 + w3);

		// steep
		t += h;
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("     Steep", Rect, Font, red, 0);
			if (pExam->m_OK && pExam->m_rf_ok) {
				//
				::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
				s.Format(_T("%.2f D "), pExam->m_rf_st_dp);
				m_MemDC.WriteText(s, Rect, Font, red, 2);
				//
				::SetRect(&Rect, L + w1 + w2, intRound(t), R, intRound(t + h + 1));
				m_MemDC.WriteText("x", Rect, Font, red, 0);
				s.Format(_T("%i° "), pExam->m_rf_st_dg);
				m_MemDC.WriteText(s, Rect, Font, red, 2);
			}
		}
		// flat
		t += h;
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("     Flat", Rect, Font, blue, 0);
			if (pExam->m_OK && pExam->m_rf_ok) {
				//
				::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
				s.Format(_T("%.2f D "), pExam->m_rf_fl_dp);
				m_MemDC.WriteText(s, Rect, Font, blue, 2);
				//
				::SetRect(&Rect, L + w1 + w2, intRound(t), R, intRound(t + h + 1));
				m_MemDC.WriteText("x", Rect, Font, blue, 0);
				s.Format(_T("%i° "), pExam->m_rf_fl_dg);
				m_MemDC.WriteText(s, Rect, Font, blue, 2);
			}
		}
		// Astigmatism
		t += h;
		{
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			m_MemDC.WriteText("     Astigmatism", Rect, Font, white, 0);
			if (pExam->m_OK && pExam->m_rf_ok) {
				//
				::SetRect(&Rect, L + w1, intRound(t), L + w1 + w2, intRound(t + h + 1));
				s.Format(_T("%.2f D "), pExam->m_rf_df_dp);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
				//
				::SetRect(&Rect, L + w1 + w2, intRound(t), R, intRound(t + h + 1));
				m_MemDC.WriteText("x", Rect, Font, white, 0);
				s.Format(_T("%i° "), pExam->m_rf_st_dg);
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
		}
	}
	// 530 Done

	//effective
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Effective Ref Pwr @ 3.00m", Rect, Font, white, 0);

		if (pExam->m_OK && pExam->m_rf_ok) {
			//::SetRect(&Rect, L + w1 + w2, intRound(t), L + w1 + w2 + w3, intRound(t + h + 1));
			s.Format(_T("%.2f D "), pExam->m_rf_av_dp);

			m_MemDC.WriteText(s, Rect, Font, white, 2);//[5.1.1]

		}
	}

	// spherical aberration
	//cjf 0515
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" Corneal SphAb @ D = 6.00 mm", Rect, Font, white, 0);
		if (pExam->m_OK) {
			//s.Format(_T("%+.3f µ "), fabs(pExam->m_sph_6mm));
			s.Format(_T("%.3f µ "), pExam->m_sph_6mm);//6.2.0
			m_MemDC.WriteText(s, Rect, Font, white, 2);
		}
	}

	// I-S 
	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" I-S Axial Power @ D = 6.00 mm", Rect, Font, white, 0);
		if (pExam->m_OK && pExam->m_is_ok)
		{
			s.Format(_T("%.2f D "), pExam->m_is_df_dp);
			if (s == "-0.00 D " || s == "0.00 D ")
			{
				s = "0.00 D ";
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
			else
			{
				m_MemDC.WriteText(s, Rect, Font, white, 2);
			}
		}
	}

	// notes
	s = G_ReadChinese(pExam->m_Header.m_Note);

	int l = s.GetLength();
	int i;

	for (i = 1; i <= l; i++)
	{
		if (s[i - 1] == '\n') break;
		m_MemDC.MeasureRect(" " + s.Left(i) + " ", Rect, Font);
		if (Rect.right - Rect.left > R - L) break;
	}

	t += h;
	{
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText(" " + s.Left(i) + " ", Rect, Font, white, 0);
	}
}

//***************************************************************************************
void CSumWnd::VisualComplaints(CWFExam* pExam, RECT Rect, int NumRows)
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);

	CString s;
	COLORREF Color;

	int L = Rect.left, T = Rect.top, R = Rect.right;

	real_t t = T;
	real_t h = (Rect.bottom - Rect.top) / (real_t)NumRows;

	CMFont Font(intRound(1.1 * h), 400, "Arial");

	::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
	m_MemDC.WriteText(" Potential Visual Complaints", Rect, Font, white, 0);

	CString Name[6] = {
		"     Night Myopia",
		"     Night Hyperopia",
		"     Blur / Double Vision",
		"     Glare / Halo",
		"     Mixed Focus",
		"     Starburst"
	};
	int *A[6] = {
		&pExam->m_Complaints.m_NightMiopia,
		&pExam->m_Complaints.m_NightHyperopia,
		&pExam->m_Complaints.m_BlurDoubleVision,
		&pExam->m_Complaints.m_GlareHalo,
		&pExam->m_Complaints.m_MixedFocus,
		&pExam->m_Complaints.m_Starburst
	};

	for (int i = 0; i < 6; i++) {
		if (*A[i] != COMP_NORMAL) {
			t += h;
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			if (*A[i] == COMP_SEVERE) {
				Color = RED;
				s = "+++ ";
			}
			else if (*A[i] == COMP_MODERATE) {
				Color = m_Printing ? ORANGE : YELLOW;
				s = "++ ";
			}
			else if (*A[i] == COMP_LIGHT) {
				Color = white;
				s = "+ ";
			}
			m_MemDC.WriteText(Name[i], Rect, Font, Color, 0);
			m_MemDC.WriteText(s, Rect, Font, Color, 2);
		}
	}

	if (pExam->m_Image.m_pu_r_mean_um < 1250.0) {
		t += h;
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText("     Small pupil", Rect, Font, white, 0);
		t += h;
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText("     Consider pinhole effect", Rect, Font, white, 0);
	}

}


//***************************************************************************************
void CSumWnd::SoloVisualComplaints(CWFExam* pExam, RECT Rect, int NumRows, BOOL HighLight)
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	if (HighLight) m_MemDC.DrawRectangle(Rect, white, NOCOLOR);
	else m_MemDC.DrawRectangle(Rect, gray, NOCOLOR);

	CString s;
	COLORREF Color;

	int L = Rect.left, T = Rect.top, R = Rect.right;

	real_t t = T;
	real_t h = (Rect.bottom - Rect.top) / (real_t)NumRows;

	CMFont Font(intRound(1.1 * h), 400, "Arial");

	::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
	m_MemDC.WriteText(" Potential Visual Complaints", Rect, Font, white, 0);

	CString Name[6] = {
		"     Night Myopia",
		"     Night Hyperopia",
		"     Blur / Double Vision",
		"     Glare / Halo",
		"     Mixed Focus",
		"     Starburst"
	};
	int *A[6] = {
		&pExam->m_Complaints.m_NightMiopia,
		&pExam->m_Complaints.m_NightHyperopia,
		&pExam->m_Complaints.m_BlurDoubleVision,
		&pExam->m_Complaints.m_GlareHalo,
		&pExam->m_Complaints.m_MixedFocus,
		&pExam->m_Complaints.m_Starburst
	};

	for (int i = 0; i < 6; i++) {
		if (*A[i] != COMP_NORMAL) {
			t += h;
			::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
			if (*A[i] == COMP_SEVERE) {
				Color = RED;
				s = "+++ ";
			}
			else if (*A[i] == COMP_MODERATE) {
				Color = m_Printing ? ORANGE : YELLOW;
				s = "++ ";
			}
			else if (*A[i] == COMP_LIGHT) {
				Color = white;
				s = "+ ";
			}
			m_MemDC.WriteText(Name[i], Rect, Font, Color, 0);
			m_MemDC.WriteText(s, Rect, Font, Color, 2);
		}
	}

	if (pExam->m_Image.m_pu_r_mean_um < 1250.0) {
		t += h;
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText("     Small pupil", Rect, Font, white, 0);
		t += h;
		::SetRect(&Rect, L, intRound(t), R, intRound(t + h + 1));
		m_MemDC.WriteText("     Consider pinhole effect", Rect, Font, white, 0);
	}

}

//***************************************************************************************

//520 For show the complain 
void CSumWnd::VisualCTComplaints(CWFExam* pExam, RECT Rect, int NumRows)
{
	COLORREF white = m_Printing ? BLACK : WHITE;
	COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

	CString s, s1, s2;

	RECT UpperRect, BtmRect;//530

	int L = Rect.left, T = Rect.top, R = Rect.right;

	real_t h = (Rect.bottom - Rect.top) / (real_t)NumRows;

	CMFont Font(intRound(0.8*h), 400, "Arial");

	::SetRect(&UpperRect, L, T, R, T + intRound(3 * h));//530

	m_MemDC.DrawRectangle(UpperRect, gray, NOCOLOR);//530

	::SetRect(&BtmRect, L, T + intRound(3 * h) + 10, R, Rect.bottom);//530

	m_MemDC.DrawRectangle(BtmRect, gray, NOCOLOR);//530
												  //

	s = pExam->m_Header.m_AutoNote;
	//s.Replace(" Distance","");

	//530 Remove Alpha and Kappa from AutoNote
	real_t t;
	if (pExam->m_AlignmentMethod == ALIGNMENT_4_IR_DOTS)// New
	{
		if (pExam->m_Header.m_AngleAlphaOK)//
		{
			if (pExam->m_Header.m_AngleAlphaRUm == 0)  pExam->m_Header.m_AngleAlphaADg = 0;//600
			s1.Format(_T("Angle Alpha Distance:    %.3f mm @ %i°"), pExam->m_Header.m_AngleAlphaRUm * 0.001, pExam->m_Header.m_AngleAlphaADg);
			s.Replace(s1, _T(""));
		}
		else s1 = "";

		if (pExam->m_Header.m_AngleKappaOK)//
		{
			if (pExam->m_Header.m_AngleKappaRUm == 0)  pExam->m_Header.m_AngleKappaADg = 0;//600
			s2.Format(_T("Angle Kappa Distance:   %.3f mm @ %i°"), pExam->m_Header.m_AngleKappaRUm * 0.001, pExam->m_Header.m_AngleKappaADg);
			s.Replace(s2, _T(""));
		}
		else s2 = "";

		s1.Replace(_T("lpha Distance"), _T(""));
		s2.Replace(_T("appa Distance"), _T(""));
		//530 Done

		//530
		COLORREF yellow = m_Printing ? ORANGE : YELLOW;//601 

		t = UpperRect.top;
		real_t h1 = (UpperRect.bottom - UpperRect.top) / 2;

		int len1 = s1.GetLength();
		int len2 = s2.GetLength();

		CString Ms = (len1 > len2 ? s1 : s2);

		int w = R - L - 3;
		int FontSize = GetFontSize(w, Ms, intRound(0.8*h1));
		CMFont Font1(FontSize, 400, "Arial");

		::SetRect(&Rect, L + 3, intRound(t), R, intRound(t + h1 + 1));
		//531
		if (pExam->m_Header.m_AngleAlphaRUm < 300) m_MemDC.WriteText(" " + s1 + " ", Rect, Font1, GREEN, 0);
		else
		{
			if (pExam->m_Header.m_AngleAlphaRUm < 500) m_MemDC.WriteText(" " + s1 + " ", Rect, Font1, yellow, 0);
			else m_MemDC.WriteText(" " + s1 + " ", Rect, Font1, RED, 0);
		}
		//531 Done

		t += h1;
		::SetRect(&Rect, L + 3, intRound(t), R, intRound(t + h1 + 1));
		m_MemDC.WriteText(" " + s2 + " ", Rect, Font1, white, 0);
		//530
	}// New

	t = BtmRect.top;

	int l = s.GetLength();
	while (l > 0)
	{
		int i;
		for (i = 1; i <= l; i++)
		{
			if (s[i - 1] == '\n') break;
		}

		if (s.Left(i) != "" && s.Left(i) != "\n")
		{
			CString tts = s.Left(i);
			t += h;
			{
				// New
				int count = s.Left(i).Find(_T("Alpha"), 0);
				int count2 = s.Left(i).Find(_T("Kappa"), 0);

				if (count != -1 || count2 != -1)
				{
					if (pExam->m_AlignmentMethod == ALIGNMENT_4_IR_DOTS)
					{
						::SetRect(&Rect, L + 3, intRound(t), R, intRound(t + h + 1));
						m_MemDC.WriteText(" " + s.Left(i) + " ", Rect, Font, white, 0);
					}
				}
				else// New
				{
					::SetRect(&Rect, L + 3, intRound(t), R, intRound(t + h + 1));
					m_MemDC.WriteText(" " + s.Left(i) + " ", Rect, Font, white, 0);
				}
			}
		}
		s = s.Right(l - i);
		l -= i;
	}
}
//520
//***************************************************************************************
//530 Get the biggest fontsize of a string in a specified InputRect
int CSumWnd::GetFontSize(int w, CString inputStr, int OriFontSize)
{
	RECT TempRect;
	int FontSize = OriFontSize;
	CMFont Font(FontSize, 400, "Arial");

	while (1)
	{
		m_MemDC.MeasureRect(inputStr + "  ", TempRect, Font);

		int thisw = TempRect.right - TempRect.left;

		if (thisw <= w) break;

		else
		{
			FontSize = intRound(0.9 * real_t(FontSize));
			Font.Create(FontSize, 400, "Arial");
		}
	}

	return FontSize;
}