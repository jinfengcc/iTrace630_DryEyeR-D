//***************************************************************************************

#include "StdAfx.h"
#include "Data.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

void CExam::CalcAngles()
{
	if (m_Image.m_ve_ok && m_Image.m_pu_ok)
	{
		m_Header.m_AngleKappaOK = TRUE;
		m_Header.m_AngleKappaXUm = m_Image.m_pu_x0_um - m_Image.m_ve_x_um;
		m_Header.m_AngleKappaYUm = m_Image.m_pu_y0_um - m_Image.m_ve_y_um;
		m_Header.m_AngleKappaRUm = hyp(m_Header.m_AngleKappaYUm, m_Header.m_AngleKappaXUm);
		m_Header.m_AngleKappaADg = intRound(_180_Pi * angle(m_Header.m_AngleKappaYUm, m_Header.m_AngleKappaXUm)) % 360;
		CString s;
		s.Format(_T("\nAngle Kappa Distance:   %.3f mm @ %i°"), m_Header.m_AngleKappaRUm * 0.001, m_Header.m_AngleKappaADg);
		m_Header.m_AutoNote += s;
	}
	else
	{
		m_Header.m_AngleKappaOK = FALSE;
	}

	if (m_Image.m_ve_ok && m_Image.m_li_ok)
	{
		m_Header.m_AngleAlphaOK = TRUE;
		m_Header.m_AngleAlphaXUm = m_Image.m_li_x0_um - m_Image.m_ve_x_um;
		m_Header.m_AngleAlphaYUm = m_Image.m_li_y0_um - m_Image.m_ve_y_um;
		m_Header.m_AngleAlphaRUm = hyp(m_Header.m_AngleAlphaYUm, m_Header.m_AngleAlphaXUm);
		m_Header.m_AngleAlphaADg = intRound(_180_Pi * angle(m_Header.m_AngleAlphaYUm, m_Header.m_AngleAlphaXUm)) % 360;
		CString s;
		s.Format(_T("\nAngle Alpha Distance:    %.3f mm @ %i°"), m_Header.m_AngleAlphaRUm * 0.001, m_Header.m_AngleAlphaADg);
		m_Header.m_AutoNote += s;
	}
	else
	{
		m_Header.m_AngleAlphaOK = FALSE;
	}


	if (m_Image.m_ve_ok && m_Image.m_Ra_ok == TRUE)
	{
		m_Header.m_AngleRulerOK = TRUE;
		m_Header.m_AngleRulerXUm = m_Image.m_Ra_x_um - m_Image.m_ve_x_um;
		m_Header.m_AngleRulerYUm = m_Image.m_Ra_y_um - m_Image.m_ve_y_um;
		m_Header.m_AngleRulerRUm = hyp(m_Header.m_AngleRulerYUm, m_Header.m_AngleRulerXUm);
		m_Header.m_AngleRulerADg = intRound(_180_Pi * angle(m_Header.m_AngleRulerYUm, m_Header.m_AngleRulerXUm)) % 360;
	}
	else
	{
		m_Header.m_AngleRulerOK = FALSE;
	}

	if (m_Image.m_ve_ok && m_Image.m_In_ok == TRUE)
	{
		m_Header.m_InlayOK = TRUE;
		m_Header.m_InlayXUm = m_Image.m_In_x_um - m_Image.m_ve_x_um;
		m_Header.m_InlayYUm = m_Image.m_In_y_um - m_Image.m_ve_y_um;
		m_Header.m_InlayAngleRUm = hyp(m_Header.m_InlayYUm, m_Header.m_InlayXUm);
		m_Header.m_InlayAngleADg = intRound(_180_Pi * angle(m_Header.m_InlayYUm, m_Header.m_InlayXUm)) % 360;
	}
	else
	{
		m_Header.m_InlayOK = FALSE;
	}

	if (m_Image.m_LRa_ok == TRUE)
	{
		m_Header.m_LinearRulerOK = TRUE;
		m_Header.m_LinearRulerX0Um = m_Image.m_LRa_x0_um;
		m_Header.m_LinearRulerY0Um = m_Image.m_LRa_y0_um;
		m_Header.m_LinearRulerX1Um = m_Image.m_LRa_x1_um;
		m_Header.m_LinearRulerY1Um = m_Image.m_LRa_y1_um;
	}
	else
	{
		m_Header.m_LinearRulerOK = FALSE;
	}
}

//***************************************************************************************

COpData::COpData()
{
	m_OK = FALSE;

	m_OpType = 0;

	m_RefAxis[0] = 0;
	m_RefAxis[1] = 180;

	// Pre-op
	m_CorneaPreopCylSimK = INVALID_VALUE;
	m_CorneaPreopAxisSimK = INVALID_VALUE;
	m_CorneaPreopCylWF = INVALID_VALUE;
	m_CorneaPreopAxisWF = INVALID_VALUE;
	m_CorneaPreopCyl = INVALID_VALUE;
	m_CorneaPreopAxis = INVALID_VALUE;
	m_EyePreopCylWF = INVALID_VALUE;
	m_EyePreopAxisWF = INVALID_VALUE;
	m_EyePreopCyl = INVALID_VALUE;
	m_EyePreopAxis = INVALID_VALUE;

	// IOL
	m_CorneaIncisionAxis = INVALID_VALUE;

	m_CorneaInducedCyl = INVALID_VALUE;
	m_CorneaInducedAxis = INVALID_VALUE;

	m_IOLSph = INVALID_VALUE;
	m_IOLPlacementAxis = INVALID_VALUE;

	m_IOLs[0].m_Name = "Non-toric";
	m_IOLs[0].m_Cyl = 0.00;
	m_IOLs[0].m_InternalPostopCyl = 0.00;

	m_IOLs[1].m_Name = "T3";
	m_IOLs[1].m_Cyl = 1.50;
	m_IOLs[1].m_InternalPostopCyl = 1.03;

	m_IOLs[2].m_Name = "T4";
	m_IOLs[2].m_Cyl = 2.25;
	m_IOLs[2].m_InternalPostopCyl = 1.55;

	m_IOLs[3].m_Name = "T5";
	m_IOLs[3].m_Cyl = 3.00;
	m_IOLs[3].m_InternalPostopCyl = 2.06;

	m_IOLs[4].m_Name = "T6";
	m_IOLs[4].m_Cyl = 3.75;
	m_IOLs[4].m_InternalPostopCyl = 2.58;

	m_IOLs[5].m_Name = "T7";
	m_IOLs[5].m_Cyl = 4.50;
	m_IOLs[5].m_InternalPostopCyl = 3.09;

	m_IOLs[6].m_Name = "T8";
	m_IOLs[6].m_Cyl = 5.25;
	m_IOLs[6].m_InternalPostopCyl = 3.61;

	m_IOLs[7].m_Name = "T9";
	m_IOLs[7].m_Cyl = 6.00;
	m_IOLs[7].m_InternalPostopCyl = 4.12;

	m_IOLs[8].m_Name = "T6+";
	m_IOLs[8].m_Cyl = 4.00;
	m_IOLs[8].m_InternalPostopCyl = 2.74;

	m_IOLs[9].m_Name = "T2";
	m_IOLs[9].m_Cyl = 1.00;
	m_IOLs[9].m_InternalPostopCyl = 0.68;

	m_IOLs[10].m_Name = "T3+";
	m_IOLs[10].m_Cyl = 2.00;
	m_IOLs[10].m_InternalPostopCyl = 1.38;

	m_IOLs[11].m_Name = "T5+";
	m_IOLs[11].m_Cyl = 3.50;
	m_IOLs[11].m_InternalPostopCyl = 2.41;

	m_SelectedIOL = -1;

	// ICL
	m_ICL.m_Cyl = INVALID_VALUE;
	m_ICL.m_Axis = INVALID_VALUE;
	m_ICL.m_PlacementAxis = INVALID_VALUE;

	ResetCalculatedStuff();

	//6.2.0
	// Pre-op
	m_Pri_CorneaPreopCylSimK = INVALID_VALUE;
	m_Pri_CorneaPreopAxisSimK = INVALID_VALUE;
	m_Pri_CorneaPreopCylWF = INVALID_VALUE;
	m_Pri_CorneaPreopAxisWF = INVALID_VALUE;
	m_Pri_CorneaPreopCyl = INVALID_VALUE;
	m_Pri_CorneaPreopAxis = INVALID_VALUE;
	m_Pri_EyePreopCylWF = INVALID_VALUE;
	m_Pri_EyePreopAxisWF = INVALID_VALUE;
	m_Pri_EyePreopCyl = INVALID_VALUE;
	m_Pri_EyePreopAxis = INVALID_VALUE;

	// IOL
	m_Pri_CorneaIncisionAxis = INVALID_VALUE;

	m_Pri_CorneaInducedCyl = INVALID_VALUE;
	m_Pri_CorneaInducedAxis = INVALID_VALUE;

	m_Pri_IOLSph = INVALID_VALUE;
	m_Pri_IOLPlacementAxis = INVALID_VALUE;

	m_Pri_IOLs[0].m_Name = "Non-toric";
	m_Pri_IOLs[0].m_Cyl = 0.00;
	m_Pri_IOLs[0].m_InternalPostopCyl = 0.00;

	m_Pri_IOLs[1].m_Name = "T3";
	m_Pri_IOLs[1].m_Cyl = 1.50;
	m_Pri_IOLs[1].m_InternalPostopCyl = 1.03;

	m_Pri_IOLs[2].m_Name = "T4";
	m_Pri_IOLs[2].m_Cyl = 2.25;
	m_Pri_IOLs[2].m_InternalPostopCyl = 1.55;

	m_Pri_IOLs[3].m_Name = "T5";
	m_Pri_IOLs[3].m_Cyl = 3.00;
	m_Pri_IOLs[3].m_InternalPostopCyl = 2.06;

	m_Pri_IOLs[4].m_Name = "T6";
	m_Pri_IOLs[4].m_Cyl = 3.75;
	m_Pri_IOLs[4].m_InternalPostopCyl = 2.58;

	m_Pri_IOLs[5].m_Name = "T7";
	m_Pri_IOLs[5].m_Cyl = 4.50;
	m_Pri_IOLs[5].m_InternalPostopCyl = 3.09;

	m_Pri_IOLs[6].m_Name = "T8";
	m_Pri_IOLs[6].m_Cyl = 5.25;
	m_Pri_IOLs[6].m_InternalPostopCyl = 3.61;

	m_Pri_IOLs[7].m_Name = "T9";
	m_Pri_IOLs[7].m_Cyl = 6.00;
	m_Pri_IOLs[7].m_InternalPostopCyl = 4.12;

	m_Pri_IOLs[8].m_Name = "T6+";
	m_Pri_IOLs[8].m_Cyl = 4.00;
	m_Pri_IOLs[8].m_InternalPostopCyl = 2.74;

	m_Pri_IOLs[9].m_Name = "T2";
	m_Pri_IOLs[9].m_Cyl = 1.00;
	m_Pri_IOLs[9].m_InternalPostopCyl = 0.68;

	m_Pri_IOLs[10].m_Name = "T3+";
	m_Pri_IOLs[10].m_Cyl = 2.00;
	m_Pri_IOLs[10].m_InternalPostopCyl = 1.38;

	m_Pri_IOLs[11].m_Name = "T5+";
	m_Pri_IOLs[11].m_Cyl = 3.50;
	m_Pri_IOLs[11].m_InternalPostopCyl = 2.41;

	m_Pri_SelectedIOL = -1;

	// ICL
	m_Pri_ICL.m_Cyl = INVALID_VALUE;
	m_Pri_ICL.m_Axis = INVALID_VALUE;
	m_Pri_ICL.m_PlacementAxis = INVALID_VALUE;

	Reset_Pri_CalculatedStuff();
	//6.2.0
}

//***************************************************************************************

void COpData::ResetCalculatedStuff()
{
	m_OK = FALSE;

	m_CorneaInducedAxis = INVALID_VALUE;

	m_InternalPreopCyl = INVALID_VALUE;
	m_InternalPreopAxis = INVALID_VALUE;

	for (int i = 0; i < 12; i++)
	{
		m_IOLs[i].m_InternalPostopAxis = INVALID_VALUE;
		m_IOLs[i].m_EyePostopCyl = INVALID_VALUE;
		m_IOLs[i].m_EyePostopAxis = INVALID_VALUE;
	}

	m_BestIOL = -1;

	m_ICL.m_InducedCyl = INVALID_VALUE;
	m_ICL.m_InducedAxis = INVALID_VALUE;

	m_CorneaPostopCyl = INVALID_VALUE;
	m_CorneaPostopAxis = INVALID_VALUE;
	m_InternalPostopCyl = INVALID_VALUE;
	m_InternalPostopAxis = INVALID_VALUE;
	m_EyePostopCyl = INVALID_VALUE;
	m_EyePostopAxis = INVALID_VALUE;
}


//***************************************************************************************
//6.2.0 For cyclinderadjustment
void COpData::Reset_Pri_CalculatedStuff()
{
	m_Pri_OK = FALSE;

	m_Pri_CorneaInducedAxis = INVALID_VALUE;

	m_Pri_InternalPreopCyl = INVALID_VALUE;
	m_Pri_InternalPreopAxis = INVALID_VALUE;

	for (int i = 0; i < 12; i++)
	{
		m_Pri_IOLs[i].m_InternalPostopAxis = INVALID_VALUE;
		m_Pri_IOLs[i].m_EyePostopCyl = INVALID_VALUE;
		m_Pri_IOLs[i].m_EyePostopAxis = INVALID_VALUE;
	}

	m_Pri_BestIOL = -1;

	m_Pri_ICL.m_InducedCyl = INVALID_VALUE;
	m_Pri_ICL.m_InducedAxis = INVALID_VALUE;

	m_Pri_CorneaPostopCyl = INVALID_VALUE;
	m_Pri_CorneaPostopAxis = INVALID_VALUE;
	m_Pri_InternalPostopCyl = INVALID_VALUE;
	m_Pri_InternalPostopAxis = INVALID_VALUE;
	m_Pri_EyePostopCyl = INVALID_VALUE;
	m_Pri_EyePostopAxis = INVALID_VALUE;
}
//***************************************************************************************

//6.2.0 For cyclinderadjustment
void COpData::CalNewCorneaPreop()
{
	real PostopCyl;

	if (
		(m_CorneaPreopAxis >= 60 && m_CorneaPreopAxis <= 120)
		)
	{
		 PostopCyl = 0.5;
	}

	else if (
		(m_CorneaPreopAxis >= 150 && m_CorneaPreopAxis < 180)
		||
		(m_CorneaPreopAxis >= 0 && m_CorneaPreopAxis <= 30)
		)
	{
		PostopCyl = 0.3;
	}
	else
	{
		m_Pri_CorneaPreopCyl = m_CorneaPreopCyl;
		m_Pri_CorneaPreopAxis = m_CorneaPreopAxis;

		return;
	}

	real Cyl1X = PostopCyl * ::COS[0];
	real Cyl1Y = PostopCyl * ::SIN[0];

	real Cyl2X = m_CorneaPreopCyl * ::COS[2 * m_CorneaPreopAxis];
	real Cyl2Y = m_CorneaPreopCyl * ::SIN[2 * m_CorneaPreopAxis];

	real CylX = Cyl1X + Cyl2X;
	real CylY = Cyl1Y + Cyl2Y;

	m_Pri_CorneaPreopCyl = hyp(CylX, CylY);
	m_Pri_CorneaPreopAxis = intRound(_180_Pi * 0.5 * angle(CylY, CylX)) % 180;
}
//6.2.0 For cyclinderadjustment

//***************************************************************************************

void COpData::Recalculate()
{
 	ResetCalculatedStuff();

	BOOL CorneaPostopCalculated = FALSE;

	if (m_OpType == 1)
	{
		m_CorneaIncisionAxis = 0;
		m_CorneaInducedCyl = 0.0;
	}

	if (m_CorneaPreopCyl != INVALID_VALUE && m_CorneaPreopCyl >= 0.0 && m_CorneaPreopCyl <= 10.0 &&
		m_CorneaPreopAxis != INVALID_VALUE && m_CorneaPreopAxis >= 0 && m_CorneaPreopAxis < 180 &&
		m_CorneaIncisionAxis != INVALID_VALUE && m_CorneaIncisionAxis >= 0 && m_CorneaIncisionAxis < 360 &&
		m_CorneaInducedCyl != INVALID_VALUE && m_CorneaInducedCyl >= 0.0 && m_CorneaInducedCyl <= 2.0)
	{
		m_CorneaInducedAxis = (m_CorneaIncisionAxis + 90) % 180;

		::CalculateCrossedAstigmatism(
			m_CorneaPreopCyl, m_CorneaPreopAxis,
			m_CorneaInducedCyl, m_CorneaInducedAxis,
			m_CorneaPostopCyl, m_CorneaPostopAxis);

		CorneaPostopCalculated = TRUE;
	}

	BOOL InternalPreopCalculated = FALSE;

	if (m_CorneaPreopCyl != INVALID_VALUE && m_CorneaPreopCyl >= 0.0 && m_CorneaPreopCyl <= 10.0 &&
		m_CorneaPreopAxis != INVALID_VALUE && m_CorneaPreopAxis >= 0 && m_CorneaPreopAxis < 180 &&
		m_EyePreopCyl != INVALID_VALUE && m_EyePreopCyl >= 0.0 && m_EyePreopCyl <= 10.0 &&
		m_EyePreopAxis != INVALID_VALUE && m_EyePreopAxis >= 0 && m_EyePreopAxis < 180)
	{
		::CalculateCrossedAstigmatism(
			m_EyePreopCyl, m_EyePreopAxis,
			-m_CorneaPreopCyl, m_CorneaPreopAxis,
			m_InternalPreopCyl, m_InternalPreopAxis);

		InternalPreopCalculated = TRUE;
	}

	if (m_OpType == 0)
	{
		if (!CorneaPostopCalculated) return;

		if (m_IOLPlacementAxis == INVALID_VALUE)
		{
			m_IOLPlacementAxis = m_CorneaPostopAxis;
		}
		else
		{
			if (m_IOLPlacementAxis < 0 || m_IOLPlacementAxis >= 180) return;
		}

		real BestCyl = DBL_MAX;

		for (int i = 0; i < 12; i++)
		{
			m_IOLs[i].m_InternalPostopAxis = (m_IOLPlacementAxis + 90) % 180;

			::CalculateCrossedAstigmatism(
				m_CorneaPostopCyl, m_CorneaPostopAxis,
				m_IOLs[i].m_InternalPostopCyl, m_IOLs[i].m_InternalPostopAxis,
				m_IOLs[i].m_EyePostopCyl, m_IOLs[i].m_EyePostopAxis);

			if (BestCyl > m_IOLs[i].m_EyePostopCyl)
			{
				BestCyl = m_IOLs[i].m_EyePostopCyl;
				m_BestIOL = i;
			}
		}

		if (m_SelectedIOL >= 0 && m_SelectedIOL < 12)
		{
			m_InternalPostopCyl = m_IOLs[m_SelectedIOL].m_InternalPostopCyl;
			m_InternalPostopAxis = m_IOLs[m_SelectedIOL].m_InternalPostopAxis;
			m_EyePostopCyl = m_IOLs[m_SelectedIOL].m_EyePostopCyl;
			m_EyePostopAxis = m_IOLs[m_SelectedIOL].m_EyePostopAxis;
		}
	}
	else if (m_OpType == 1)
	{
		if (!CorneaPostopCalculated) return;
		if (!InternalPreopCalculated) return;

		m_IOLPlacementAxis = m_CorneaPostopAxis;
		m_InternalPostopCyl = m_InternalPreopCyl;
		m_InternalPostopAxis = (m_IOLPlacementAxis + 90) % 180;

		::CalculateCrossedAstigmatism(
			m_CorneaPostopCyl, m_CorneaPostopAxis,
			m_InternalPostopCyl, m_InternalPostopAxis,
			m_EyePostopCyl, m_EyePostopAxis);
	}
	else if (m_OpType == 2)
	{
		if (m_EyePreopAxis == INVALID_VALUE || m_EyePreopAxis < 0 || m_EyePreopAxis >= 180) return;

		if (m_ICL.m_Axis == INVALID_VALUE || m_ICL.m_Axis < 0 || m_ICL.m_Axis >= 180) return;

		//m_ICL.m_InducedCyl = some formula or look-up table to calculate it from m_ICL.m_Cyl
		if (m_ICL.m_PlacementAxis == INVALID_VALUE)
		{
			m_ICL.m_InducedAxis = (m_EyePreopAxis + 90) % 180;
			m_ICL.m_PlacementAxis = (m_ICL.m_InducedAxis - m_ICL.m_Axis) % 180;
			if (m_ICL.m_PlacementAxis < 0) m_ICL.m_PlacementAxis += 180;
		}
		else
		{
			if (m_ICL.m_PlacementAxis < 0 || m_ICL.m_PlacementAxis >= 180) return;
			m_ICL.m_InducedAxis = (m_ICL.m_Axis + m_ICL.m_PlacementAxis) % 180;
		}
	}

	m_OK = TRUE;
}


//***************************************************************************************

void COpData::Pri_Recalculate()
{
	//6.2.0 Cyclinder adjustment Branch
	if (m_CorneaCycliderAdj)
	{
		if (m_CorneaCycliderAdjCaled != TRUE)
		{
			CalNewCorneaPreop();
			m_CorneaCycliderAdjCaled = TRUE;
		}
	}
	else return;
	//6.2.0 Cyclinder adjustment Branch

	Reset_Pri_CalculatedStuff();

	m_Pri_IOLPlacementAxis = m_Pri_IOLPlacementAxis;

	BOOL Pri_CorneaPostopCalculated = FALSE;

	m_Pri_CorneaIncisionAxis = 0;
	m_Pri_CorneaInducedCyl = 0.0;
	m_Pri_EyePreopCyl = 0.0;
	m_Pri_EyePreopAxis = 0;

	
	if (m_Pri_CorneaPreopCyl != INVALID_VALUE && m_Pri_CorneaPreopCyl >= 0.0 && m_Pri_CorneaPreopCyl <= 10.0 &&
		m_Pri_CorneaPreopAxis != INVALID_VALUE && m_Pri_CorneaPreopAxis >= 0 && m_Pri_CorneaPreopAxis < 180 &&
		m_Pri_CorneaIncisionAxis != INVALID_VALUE && m_Pri_CorneaIncisionAxis >= 0 && m_Pri_CorneaIncisionAxis < 360 &&
		m_Pri_CorneaInducedCyl != INVALID_VALUE && m_Pri_CorneaInducedCyl >= 0.0 && m_Pri_CorneaInducedCyl <= 2.0)
	{
		m_Pri_CorneaInducedAxis = (m_Pri_CorneaIncisionAxis + 90) % 180;

		::CalculateCrossedAstigmatism(
			m_Pri_CorneaPreopCyl, m_Pri_CorneaPreopAxis,
			m_Pri_CorneaInducedCyl, m_Pri_CorneaInducedAxis,
			m_Pri_CorneaPostopCyl, m_Pri_CorneaPostopAxis);

		Pri_CorneaPostopCalculated = TRUE;
	}

	BOOL Pri_InternalPreopCalculated = FALSE;

	if (m_Pri_CorneaPreopCyl != INVALID_VALUE && m_Pri_CorneaPreopCyl >= 0.0 && m_Pri_CorneaPreopCyl <= 10.0 &&
		m_Pri_CorneaPreopAxis != INVALID_VALUE && m_Pri_CorneaPreopAxis >= 0 && m_Pri_CorneaPreopAxis < 180 &&
		m_Pri_EyePreopCyl != INVALID_VALUE && m_Pri_EyePreopCyl >= 0.0 && m_Pri_EyePreopCyl <= 10.0 &&
		m_Pri_EyePreopAxis != INVALID_VALUE && m_Pri_EyePreopAxis >= 0 && m_Pri_EyePreopAxis < 180)
	{
		::CalculateCrossedAstigmatism(
			m_Pri_EyePreopCyl, m_Pri_EyePreopAxis,
			-m_Pri_CorneaPreopCyl, m_Pri_CorneaPreopAxis,
			m_Pri_InternalPreopCyl, m_Pri_InternalPreopAxis);

		Pri_InternalPreopCalculated = TRUE;
	}

	if (m_OpType == 0)
	{
		if (!Pri_CorneaPostopCalculated) return;

		if (m_Pri_IOLPlacementAxis == INVALID_VALUE)
		{
			m_Pri_IOLPlacementAxis = m_Pri_CorneaPostopAxis;
		}
		else
		{
			if (m_Pri_IOLPlacementAxis < 0 || m_Pri_IOLPlacementAxis >= 180) return;
		}

		real Pri_BestCyl = DBL_MAX;

		for (int i = 0; i < 12; i++)
		{
			m_Pri_IOLs[i].m_InternalPostopAxis = (m_Pri_IOLPlacementAxis + 90) % 180;

			::CalculateCrossedAstigmatism(
				m_Pri_CorneaPostopCyl, m_Pri_CorneaPostopAxis,
				m_Pri_IOLs[i].m_InternalPostopCyl, m_Pri_IOLs[i].m_InternalPostopAxis,
				m_Pri_IOLs[i].m_EyePostopCyl, m_Pri_IOLs[i].m_EyePostopAxis);

			if (Pri_BestCyl > m_Pri_IOLs[i].m_EyePostopCyl)
			{
				Pri_BestCyl = m_Pri_IOLs[i].m_EyePostopCyl;
				m_Pri_BestIOL = i;
			}
		}

		if (m_Pri_SelectedIOL >= 0 && m_Pri_SelectedIOL < 12)
		{
			m_Pri_InternalPostopCyl = m_Pri_IOLs[m_SelectedIOL].m_InternalPostopCyl;
			m_Pri_InternalPostopAxis = m_Pri_IOLs[m_SelectedIOL].m_InternalPostopAxis;
			m_Pri_EyePostopCyl = m_Pri_IOLs[m_SelectedIOL].m_EyePostopCyl;
			m_Pri_EyePostopAxis = m_Pri_IOLs[m_SelectedIOL].m_EyePostopAxis;
		}
	}
	else if (m_OpType == 1)
	{
		if (!Pri_CorneaPostopCalculated) return;
		if (!Pri_InternalPreopCalculated) return;

		m_Pri_IOLPlacementAxis = m_Pri_CorneaPostopAxis;
		m_Pri_InternalPostopCyl = m_Pri_InternalPreopCyl;
		m_Pri_InternalPostopAxis = (m_Pri_IOLPlacementAxis + 90) % 180;

		::CalculateCrossedAstigmatism(
			m_Pri_CorneaPostopCyl, m_Pri_CorneaPostopAxis,
			m_Pri_InternalPostopCyl, m_Pri_InternalPostopAxis,
			m_Pri_EyePostopCyl, m_Pri_EyePostopAxis);
	}
	else if (m_OpType == 2)
	{
		if (m_Pri_EyePreopAxis == INVALID_VALUE || m_Pri_EyePreopAxis < 0 || m_Pri_EyePreopAxis >= 180) return;

		if (m_Pri_ICL.m_Axis == INVALID_VALUE || m_Pri_ICL.m_Axis < 0 || m_Pri_ICL.m_Axis >= 180) return;

		//m_ICL.m_InducedCyl = some formula or look-up table to calculate it from m_ICL.m_Cyl
		if (m_Pri_ICL.m_PlacementAxis == INVALID_VALUE)
		{
			m_Pri_ICL.m_InducedAxis = (m_Pri_EyePreopAxis + 90) % 180;
			m_Pri_ICL.m_PlacementAxis = (m_Pri_ICL.m_InducedAxis - m_Pri_ICL.m_Axis) % 180;
			if (m_Pri_ICL.m_PlacementAxis < 0) m_Pri_ICL.m_PlacementAxis += 180;
		}
		else
		{
			if (m_Pri_ICL.m_PlacementAxis < 0 || m_Pri_ICL.m_PlacementAxis >= 180) return;
			m_Pri_ICL.m_InducedAxis = (m_Pri_ICL.m_Axis + m_Pri_ICL.m_PlacementAxis) % 180;
		}
	}

	m_Pri_OK = TRUE;
}

//***************************************************************************************

void CalculateCrossedAstigmatism(const real Cyl1, const int Axis1, const real Cyl2, const int Axis2, real& Cyl, int& Axis)
{	
	real Cyl1X = Cyl1 * ::COS[2 * Axis1];
	real Cyl1Y = Cyl1 * ::SIN[2 * Axis1];

	real Cyl2X = Cyl2 * ::COS[2 * Axis2];
	real Cyl2Y = Cyl2 * ::SIN[2 * Axis2];

	real CylX = Cyl1X + Cyl2X;
	real CylY = Cyl1Y + Cyl2Y;

	Cyl = hyp(CylX, CylY);
	Axis = intRound(_180_Pi * 0.5 * angle(CylY, CylX)) % 180;
}

//***************************************************************************************
