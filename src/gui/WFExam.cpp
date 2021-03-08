//***************************************************************************************

#include "StdAfx.h"
#include "Data.h"
#include "FT.h"
#include "MGdi.h"
#include "Hardware.h"
#include "Settings.h"
#include "AppVersion.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

CComplaints::CComplaints()
{
	Reset();
}

//***************************************************************************************

void CComplaints::Reset()
{
	m_NightMiopia = COMP_NORMAL;
	m_NightHyperopia = COMP_NORMAL;
	m_BlurDoubleVision = COMP_NORMAL;
	m_GlareHalo = COMP_NORMAL;
	m_MixedFocus = COMP_NORMAL;
	m_Starburst = COMP_NORMAL;
}

//***************************************************************************************

const real_t CPSF::m_um_mn = 16600.0 * tan(_Pi_180 * (1.0 / 60.0));//4.8287444000472437
const real_t CPSF::m_mn_um = 1.0 / CPSF::m_um_mn;

const real_t CWFExam::m_LADPixelSize = 25.0;

const real_t CWFExam::m_d_eye_um = 22890.0;
const real_t CWFExam::m_vn = 1.3375;
const real_t CWFExam::m_f_eye_um = m_d_eye_um / m_vn;

//***************************************************************************************

CWFExam::CWFExam()
{
	m_CurShot = 0;
	m_CurPoint = -1;

	m_sc_x0_um = 0.0;
	m_sc_y0_um = 0.0;
}

//***************************************************************************************

//Showing Presbia 2.4mm
void CWFExam::ProcessPresbiaPoints()
{
	m_SignalMax = 0;
	m_NumGoodPoints = 0;
	real_t AverageDX = 0.0;
	real_t AverageDY = 0.0;
	real_t MaxOpacityX = 0.0;
	real_t MaxOpacityY = 0.0;
	real_t MaxOpacity = 0.0;

	CScanPoint* pPoint = m_Points.MoveFirst();

	while (pPoint)
	{
		if (pPoint->m_r_um >= 700 && pPoint->m_r_um <= 1700)//530
		{
			if (m_DetectorType == DT_LINEAR_ARRAYS)
			{
				pPoint->m_LADX.Process();
				pPoint->m_LADY.Process();
				m_SignalMax = __max(m_SignalMax, pPoint->m_LADX.m_Max);
				m_SignalMax = __max(m_SignalMax, pPoint->m_LADY.m_Max);
				pPoint->m_dx_um = pPoint->m_LADX.m_CentroidPos * m_LADPixelSize;
				pPoint->m_dy_um = pPoint->m_LADY.m_CentroidPos * m_LADPixelSize;
				pPoint->m_OpacityX = pPoint->m_LADX.m_Area;
				pPoint->m_OpacityY = pPoint->m_LADY.m_Area;
				pPoint->m_Opacity = pPoint->m_OpacityX + pPoint->m_OpacityY;

				if (MaxOpacityX < pPoint->m_OpacityX) MaxOpacityX = pPoint->m_OpacityX;
				if (MaxOpacityY < pPoint->m_OpacityY) MaxOpacityY = pPoint->m_OpacityY;
				if (MaxOpacity < pPoint->m_Opacity) MaxOpacity = pPoint->m_Opacity;

				pPoint->m_AutoBad = pPoint->m_LADX.m_Bad || pPoint->m_LADY.m_Bad;
				pPoint->m_Bad |= pPoint->m_AutoBad;
			}

			if (!pPoint->m_Bad)
			{
				AverageDX += pPoint->m_dx_um;
				AverageDY += pPoint->m_dy_um;
				m_NumGoodPoints++;
			}
		}

		pPoint = m_Points.MoveNext();
	}

	if (m_NumGoodPoints > 0)
	{
		AverageDX /= m_NumGoodPoints;
		AverageDY /= m_NumGoodPoints;
	}

	pPoint = m_Points.MoveFirst();

	while (pPoint)
	{
		if (pPoint->m_r_um >= 700 && pPoint->m_r_um <= 1700)
		{
			if (MaxOpacityX > 0.0) pPoint->m_OpacityX /= MaxOpacityX;
			if (MaxOpacityY > 0.0) pPoint->m_OpacityY /= MaxOpacityY;
			if (MaxOpacity > 0.0) pPoint->m_Opacity /= MaxOpacity;
			pPoint->m_dx_um -= AverageDX;
			pPoint->m_dy_um -= AverageDY;
		}
		pPoint = m_Points.MoveNext();
	}
}

//***************************************************************************************
void CWFExam::ProcessPoints()
{
	m_SignalMax = 0;
	m_NumGoodPoints = 0;
	real_t AverageDX = 0.0;
	real_t AverageDY = 0.0;
	real_t MaxOpacityX = 0.0;
	real_t MaxOpacityY = 0.0;
	real_t MaxOpacity = 0.0;

	CScanPoint* pPoint = m_Points.MoveFirst();

	//select 10th maxOpcity as real maximum, in the future judge the 10th maximum opacity to find the noise
	real_t Max10thOpacity[10];
	for (int i = 0; i < 10; i++) Max10thOpacity[i] = 0;
	//select 10th maxOpcity Done

	while (pPoint)
	{
		if (m_DetectorType == DT_LINEAR_ARRAYS)
		{
			pPoint->m_LADX.Process();
			pPoint->m_LADY.Process();

			m_SignalMax = __max(m_SignalMax, pPoint->m_LADX.m_Max);
			m_SignalMax = __max(m_SignalMax, pPoint->m_LADY.m_Max);
			pPoint->m_dx_um = pPoint->m_LADX.m_CentroidPos * m_LADPixelSize;
			pPoint->m_dy_um = pPoint->m_LADY.m_CentroidPos * m_LADPixelSize;
			pPoint->m_OpacityX = pPoint->m_LADX.m_Area;
			pPoint->m_OpacityY = pPoint->m_LADY.m_Area;
			pPoint->m_Opacity = pPoint->m_OpacityX + pPoint->m_OpacityY;

			if (MaxOpacityX < pPoint->m_OpacityX) MaxOpacityX = pPoint->m_OpacityX;
			if (MaxOpacityY < pPoint->m_OpacityY) MaxOpacityY = pPoint->m_OpacityY;
			if (MaxOpacity < pPoint->m_Opacity) MaxOpacity = pPoint->m_Opacity;

			Max10thOpacity[0] = MaxOpacity;
			if (pPoint->m_Opacity > Max10thOpacity[1] && pPoint->m_Opacity < Max10thOpacity[0])  Max10thOpacity[1] = pPoint->m_Opacity;
			else if (pPoint->m_Opacity > Max10thOpacity[2] && pPoint->m_Opacity < Max10thOpacity[1])  Max10thOpacity[2] = pPoint->m_Opacity;
			else if (pPoint->m_Opacity > Max10thOpacity[3] && pPoint->m_Opacity < Max10thOpacity[2])  Max10thOpacity[3] = pPoint->m_Opacity;
			else if (pPoint->m_Opacity > Max10thOpacity[4] && pPoint->m_Opacity < Max10thOpacity[3])  Max10thOpacity[4] = pPoint->m_Opacity;
			else if (pPoint->m_Opacity > Max10thOpacity[5] && pPoint->m_Opacity < Max10thOpacity[4])  Max10thOpacity[5] = pPoint->m_Opacity;
			else if (pPoint->m_Opacity > Max10thOpacity[6] && pPoint->m_Opacity < Max10thOpacity[5])  Max10thOpacity[6] = pPoint->m_Opacity;
			else if (pPoint->m_Opacity > Max10thOpacity[7] && pPoint->m_Opacity < Max10thOpacity[6])  Max10thOpacity[7] = pPoint->m_Opacity;
			else if (pPoint->m_Opacity > Max10thOpacity[8] && pPoint->m_Opacity < Max10thOpacity[7])  Max10thOpacity[8] = pPoint->m_Opacity;
			else if (pPoint->m_Opacity > Max10thOpacity[9] && pPoint->m_Opacity < Max10thOpacity[8])  Max10thOpacity[9] = pPoint->m_Opacity;

			pPoint->m_AutoBad = pPoint->m_LADX.m_Bad || pPoint->m_LADY.m_Bad;
			pPoint->m_Bad |= pPoint->m_AutoBad;
		}

		if (!pPoint->m_Bad)
		{
			AverageDX += pPoint->m_dx_um;
			AverageDY += pPoint->m_dy_um;
			m_NumGoodPoints++;
		}

		pPoint = m_Points.MoveNext();
	}

	if (m_NumGoodPoints > 0)
	{
		AverageDX /= m_NumGoodPoints;
		AverageDY /= m_NumGoodPoints;
	}

	pPoint = m_Points.MoveFirst();
	while (pPoint)
	{
		if (MaxOpacityX > 0.0) pPoint->m_OpacityX /= MaxOpacityX;
		if (MaxOpacityY > 0.0) pPoint->m_OpacityY /= MaxOpacityY;

		pPoint->m_Opacity /= Max10thOpacity[4];
		if (pPoint->m_Opacity > 1) pPoint->m_Opacity = 1;

		pPoint->m_dx_um -= AverageDX;
		pPoint->m_dy_um -= AverageDY;
		pPoint = m_Points.MoveNext();
	}
}

//***************************************************************************************

real_t CWFExam::GetClosestPointOpacity(const real_t r_um, const real_t a_rd, const int shot, const int op_type)
{
	CScanPoint* pClosestPoint = NULL;
	real_t x_um = r_um * cos(a_rd);
	real_t y_um = r_um * sin(a_rd);
	real_t dmin = DBL_MAX;

	CScanPoint* pPoint = m_Points.MoveFirst();

	while (pPoint)
	{
		if (pPoint->m_shot == shot)
		{
			real_t d = sqr(x_um - pPoint->x_um()) + sqr(y_um - pPoint->y_um());
			if (d < dmin)
			{
				dmin = d;
				pClosestPoint = pPoint;
			}
		}

		pPoint = m_Points.MoveNext();
	}

	if (op_type == 0) return pClosestPoint->m_OpacityX;
	if (op_type == 1) return pClosestPoint->m_OpacityY;

	return pClosestPoint->m_Opacity;
}

//***************************************************************************************

int CWFExam::GetNumShots()
{
	int NumShots = -1;

	CScanPoint* pPoint = m_Points.MoveFirst();

	while (pPoint)
	{
		if (NumShots < pPoint->m_shot)
		{
			NumShots = pPoint->m_shot;
		}

		pPoint = m_Points.MoveNext();
	}

	return NumShots + 1;
}

//***************************************************************************************

void CWFExam::FstPoint()
{
	CScanPoint* pPoint = m_Points.MoveFirst();
	m_CurPoint = -1;
	while (pPoint)
	{
		m_CurPoint++;

		if (pPoint->m_shot == m_CurShot) break;

		pPoint = m_Points.MoveNext();
	}
}

//***************************************************************************************

void CWFExam::LstPoint()
{
	CScanPoint* pPoint = m_Points.MoveLast();

	m_CurPoint = m_Points.GetSize() - 1;

	while (pPoint)
	{
		if (pPoint->m_shot == m_CurShot) break;
		m_CurPoint--;
		pPoint = m_Points.MovePrev();
	}
}

//***************************************************************************************

void CWFExam::NxtPoint()
{
	CScanPoint* pPoint = m_Points.MoveFirst();
	int p = 0;

	while (pPoint)
	{
		if (pPoint->m_shot == m_CurShot && p > m_CurPoint)
		{
			m_CurPoint = p;
			return;
		}
		pPoint = m_Points.MoveNext();
		p++;
	}

	FstPoint();
}

//***************************************************************************************

void CWFExam::PrvPoint()
{
	CScanPoint* pPoint = m_Points.MoveLast();

	int p = m_Points.GetSize() - 1;

	while (pPoint)
	{
		if (pPoint->m_shot == m_CurShot && p < m_CurPoint)
		{
			m_CurPoint = p;
			return;
		}
		pPoint = m_Points.MovePrev();
		p--;
	}

	LstPoint();
}

//***************************************************************************************

int CWFExam::GetNumShotPoints()
{
	CScanPoint* pPoint = m_Points.MoveFirst();

	int NumShotPoints = 0;

	while (pPoint)
	{
		if (pPoint->m_shot == m_CurShot)
		{
			NumShotPoints++;
		}

		pPoint = m_Points.MoveNext();
	}

	return NumShotPoints;
}

//***************************************************************************************

void CWFExam::CreateScanPattern(real_t& r_max_um)
{
	m_Points.Destroy();

	if (r_max_um > 4000.0) r_max_um = 4000.0;

	if (::Normal64Mode)//::PresbiaMode == 1 && scanMode == 3
	{
		const int nc = 5;
		int np[nc] = { 1, 6, 13, 19, 25 };

		real_t r_min_um = 0.0;
		real_t dr_um = (r_max_um - r_min_um) / (nc - 1);

		for (int c = 0; c < nc; c++)
		{
			real_t r_um = r_min_um + c * dr_um;
			real_t da_rd = _2_Pi / np[c];

			for (int p = 0; p < np[c]; p++)
			{
				CScanPoint* pPoint = m_Points.Append();
				pPoint->m_shot = 0;
				pPoint->m_r_um = r_um;
				pPoint->m_a_rd = da_rd * p;
			}
		}
	}
	else
	{
		if (::PresbiaMode == 2)
		{
			r_max_um = 650.0;

			const int nc = 5;
			int np[nc] = { 1, 6, 13, 19, 25 };

			real_t r_min_um = 0.0;
			real_t dr_um = (r_max_um - r_min_um) / (nc - 1);

			for (int c = 0; c < nc; c++)
			{
				real_t r_um = r_min_um + c * dr_um;
				real_t da_rd = _2_Pi / np[c];

				for (int p = 0; p < np[c]; p++)
				{
					CScanPoint* pPoint = m_Points.Append();
					pPoint->m_shot = 0;
					pPoint->m_r_um = r_um;
					pPoint->m_a_rd = da_rd * p;
				}
			}
		}
		else
		{
			//Presbia64

			/*const int nc = 5;
			int np[nc] = {1, 9, 14, 19, 23};

			real r_min_um = 0.0;
			real dr_um = (r_max_um - r_min_um) / (nc - 1);
			for (int c = 0; c < nc; c++) {
			real r_um = r_min_um + c * dr_um;
			real da_rd = _2_Pi / np[c];
			real a0_rd = da_rd * 0.5;
			for (int p = 0; p < np[c]; p++) {
			CScanPoint* pPoint = m_Points.Append();
			pPoint->m_shot = 1;
			pPoint->m_r_um = r_um;
			pPoint->m_a_rd = a0_rd + da_rd * p;
			}*/
			const int nc = 7;
			int np[nc] = { 1, 6, 12, 18, 24, 30, 37 };

			real_t r_min_um = 0.0;
			real_t dr_um = (r_max_um - r_min_um) / (nc - 1);

			for (int s = 0; s < 2; s++)
			{
				for (int c = 0; c < nc; c++)
				{
					real_t r_um = r_min_um + c * dr_um;
					real_t da_rd = _2_Pi / np[c];
					real_t a0_rd = s * da_rd * 0.5;

					for (int p = 0; p < np[c]; p++)
					{
						CScanPoint* pPoint = m_Points.Append();
						pPoint->m_shot = s;
						pPoint->m_r_um = r_um;
						pPoint->m_a_rd = a0_rd + da_rd * p;
					}
				}
			}
		}
	}
	m_ScanDiameter = 2.0 * r_max_um;
}

//***************************************************************************************

void CWFExam::CreateProbePattern(real_t& r_max_um)
{
	m_Points.Destroy();

	if (r_max_um > 1000.0) r_max_um = 1000.0;

	const int np[3] = { 1, 6, 10 };
	real_t dr_um = r_max_um / 2.0;

	for (int c = 0; c < 3; c++)
	{
		real_t r_um = c * dr_um;
		real_t da_rd = _2_Pi / np[c];

		for (int p = 0; p < np[c]; p++)
		{
			CScanPoint* pPoint = m_Points.Append();
			pPoint->m_shot = 0;
			pPoint->m_r_um = r_um;
			pPoint->m_a_rd = da_rd * p;
		}
	}

	m_ScanDiameter = 2.0 * r_max_um;
}

//***************************************************************************************

void CWFExam::CreateCalSurface(CZernikeSurface& CalSurface)
{
	CalSurface.Reset();

	CalSurface.m_r_max_um = 0.5 * m_ScanDiameter;
	CalSurface.m_c[3] = -m_Calibration.Zernike.C3 * CalSurface.m_r_max_um / 4000000.0;
	CalSurface.m_c[4] = -m_Calibration.Zernike.C4 * CalSurface.m_r_max_um / 4000000.0;
	CalSurface.m_c[5] = -m_Calibration.Zernike.C5 * CalSurface.m_r_max_um / 4000000.0;
}

//***************************************************************************************

//For showing the presbia 2.4mm zone
BOOL CWFExam::Presbia24Process()
{
	m_OK = FALSE;

	m_Image.m_w_um = m_Calibration.VideoWidthMicrons;
	m_Image.m_h_um = m_Calibration.VideoHeightMicrons;

	m_ColorImage.m_w_um = m_Calibration.VideoWidthMicrons;
	m_ColorImage.m_h_um = m_Calibration.VideoHeightMicrons;

	if (!m_Header.m_Saved)
	{
		if (!m_Image.m_pu_ok)
		{
			m_Image.FindPupil();
		}

		if (!m_Image.m_ve_ok)
		{
			m_Image.FindWFVertex();
		}

		if (!m_Image.m_li_ok)
		{
			//improve the comprison and then find limbus( improve the accuracy)
			//Create the array for the IOL eye image info

			//calculate the new limbus position
			CEyeImage* TestImage = new CEyeImage();
			TestImage->m_h = m_Image.m_h;
			TestImage->m_w = m_Image.m_w;
			TestImage->m_h_um = m_Image.m_h_um;
			TestImage->m_w_um = m_Image.m_w_um;

			TestImage->m_RGBData.Create(m_Image.m_h, (int)m_Image.m_w * 3, m_Image.m_RGBData.GetMem());

			//CheckLightEye(TestImage);
			TestImage->ImproveComprison();
			TestImage->FindLimbus();

			m_Image.m_li_ok = TestImage->m_li_ok;
			m_Image.m_li_r_max_um = TestImage->m_li_r_max_um;
			m_Image.m_li_r_mean_um = TestImage->m_li_r_mean_um;
			m_Image.m_li_r_min_um = TestImage->m_li_r_min_um;
			for (int i = 0; i < 360; i++)	m_Image.m_li_r_um[i] = TestImage->m_li_r_um[i];
			m_Image.m_li_x0_um = TestImage->m_li_x0_um;
			m_Image.m_li_y0_um = TestImage->m_li_y0_um;

			delete TestImage;
		}

		if (!m_Image.m_le_ok)
		{
			m_Image.FindLensDots();
		}
	}

	m_Header.m_AutoNote.Empty();

	if (m_Image.m_pu_ok)
	{
		if (m_Image.m_pu_r_mean_um > 3000.0)
		{
			m_Header.m_AutoNote += "\nPupil size is larger than 6.00 mm.";
		}
		else if (m_Image.m_pu_r_mean_um < 1250.0)
		{
			m_Header.m_AutoNote += "\nPupil size is smaller than 2.50 mm.";
		}
	}

	if (m_AlignmentMethod == ALIGNMENT_4_IR_DOTS)
	{
		m_Header.m_AutoNote += "\nAligned on IR dots.";
	}

	CalcAngles();

	ProcessPresbiaPoints();

	real_t* r_um = new real_t[m_NumGoodPoints];
	real_t* a_rd = new real_t[m_NumGoodPoints];
	real_t* dw_dx = new real_t[m_NumGoodPoints];
	real_t* dw_dy = new real_t[m_NumGoodPoints];

	int k = 0;
	CScanPoint* pPoint = m_Points.MoveFirst();
	real_t maxum = 0;

	while (pPoint)
	{
		if (pPoint->m_r_um >= 700 && pPoint->m_r_um <= 1700)
		{
			if (!pPoint->m_Bad)
			{
				r_um[k] = pPoint->m_r_um;
				a_rd[k] = pPoint->m_a_rd;
				dw_dx[k] = -pPoint->m_dx_um / m_Calibration.LADXFocalDistance;
				dw_dy[k] = pPoint->m_dy_um / m_Calibration.LADYFocalDistance;
				k++;
			}
		}
		pPoint = m_Points.MoveNext();
	}

	real_t rmse_min = DBL_MAX;
	int order_max;
	for (order_max = 2; order_max <= ZERNIKE_ORDER_MAX; order_max++)
	{
		CZernikeSurface Surface;
		real_t rmse;
		if (Surface.CreateXY(order_max, 0.5 * m_ScanDiameter, m_NumGoodPoints, r_um, a_rd, dw_dx, dw_dy, &rmse))
		{
			if (rmse < rmse_min)
			{
				rmse_min = rmse;
				m_WfSurface = Surface;
				m_OK = TRUE;
			}
		}
	}

	delete[] r_um;
	delete[] a_rd;
	delete[] dw_dx;
	delete[] dw_dy;

	if (!m_OK) return FALSE;

	CZernikeSurface CalSurface;
	CreateCalSurface(CalSurface);
	m_WfSurface -= CalSurface;
	m_WfSurface.ComputePiston(TRUE);
	m_WfSurface.ComputeTilt(TRUE);

	CreateComplaints();

	return TRUE;
}

//***************************************************************************************

BOOL CWFExam::Process()
{
	m_OK = FALSE;

	m_Image.m_w_um = m_Calibration.VideoWidthMicrons;
	m_Image.m_h_um = m_Calibration.VideoHeightMicrons;

	m_ColorImage.m_w_um = m_Calibration.VideoWidthMicrons;
	m_ColorImage.m_h_um = m_Calibration.VideoHeightMicrons;

	if (!m_Header.m_Saved)
	{
		if (!m_Image.m_pu_ok)
		{
			m_Image.FindPupil();
		}

		if (!m_Image.m_ve_ok)
		{
			m_Image.FindWFVertex();
		}

		if (!m_Image.m_li_ok)
		{
			//improve the comprison and then find limbus( improve the accuracy)
			//Create the array for the IOL eye image info

			//calculate the new limbus position
			CEyeImage* TestImage = new CEyeImage();
			TestImage->m_h = m_Image.m_h;
			TestImage->m_w = m_Image.m_w;
			TestImage->m_h_um = m_Image.m_h_um;
			TestImage->m_w_um = m_Image.m_w_um;

			TestImage->m_RGBData.Create(m_Image.m_h, (int)m_Image.m_w * 3, m_Image.m_RGBData.GetMem());

			TestImage->ImproveComprison();
			TestImage->FindLimbus();

			m_Image.m_li_ok = TestImage->m_li_ok;
			m_Image.m_li_r_max_um = TestImage->m_li_r_max_um;
			m_Image.m_li_r_mean_um = TestImage->m_li_r_mean_um;
			m_Image.m_li_r_min_um = TestImage->m_li_r_min_um;
			for (int i = 0; i < 360; i++)	m_Image.m_li_r_um[i] = TestImage->m_li_r_um[i];
			m_Image.m_li_x0_um = TestImage->m_li_x0_um;
			m_Image.m_li_y0_um = TestImage->m_li_y0_um;

			delete TestImage;
		}

		if (!m_Image.m_le_ok)
		{
			m_Image.FindLensDots();
		}
	}

	m_Header.m_AutoNote.Empty();

	if (m_Image.m_pu_ok)
	{
		if (m_Image.m_pu_r_mean_um > 3000.0)
		{
			m_Header.m_AutoNote += "\nPupil size is larger than 6.00 mm.";
		}
		else if (m_Image.m_pu_r_mean_um < 1250.0)
		{
			m_Header.m_AutoNote += "\nPupil size is smaller than 2.50 mm.";
		}
	}

	if (m_AlignmentMethod == ALIGNMENT_4_IR_DOTS)
	{
		m_Header.m_AutoNote += "\nAligned on IR dots.";
	}

	CalcAngles();

	ProcessPoints();

	real_t* r_um = new real_t[m_NumGoodPoints];
	real_t* a_rd = new real_t[m_NumGoodPoints];
	real_t* dw_dx = new real_t[m_NumGoodPoints];
	real_t* dw_dy = new real_t[m_NumGoodPoints];

	int k = 0;

	CScanPoint* pPoint = m_Points.MoveFirst();

	while (pPoint)
	{
		if (!pPoint->m_Bad)
		{
			r_um[k] = pPoint->m_r_um;
			a_rd[k] = pPoint->m_a_rd;
			dw_dx[k] = -pPoint->m_dx_um / m_Calibration.LADXFocalDistance;
			dw_dy[k] = pPoint->m_dy_um / m_Calibration.LADYFocalDistance;
			k++;
		}
		pPoint = m_Points.MoveNext();
	}

	real_t rmse_min = DBL_MAX;
	int order_max;
	for (order_max = 2; order_max <= ZERNIKE_ORDER_MAX; order_max++)
	{
		CZernikeSurface Surface;
		real_t rmse;

		if (Surface.CreateXY(order_max, 0.5 * m_ScanDiameter, m_NumGoodPoints, r_um, a_rd, dw_dx, dw_dy, &rmse))
		{
			if (rmse < rmse_min)
			{
				rmse_min = rmse;
				m_WfSurface = Surface;
				m_OK = TRUE;
			}
		}
	}

	delete[] r_um;
	delete[] a_rd;
	delete[] dw_dx;
	delete[] dw_dy;

	if (!m_OK) return FALSE;

	CZernikeSurface CalSurface;
	CreateCalSurface(CalSurface);
	m_WfSurface -= CalSurface;
	m_WfSurface.ComputePiston(TRUE);
	m_WfSurface.ComputeTilt(TRUE);

	CreateComplaints();

	return TRUE;
}

//***************************************************************************************

void CWFExam::CreateOpm2D(CMap2D& Map, const real_t dr_um, const int shot, const int op_type)
{
	if (!m_OK) return;

	real_t r_max_um = 0.5 * m_ScanDiameter;

	Map.Create(r_max_um, dr_um);

	for (int y = -Map.m_nr; y <= Map.m_nr; y++)
	{
		real_t y_um = y * dr_um;
		for (int x = -Map.m_nr; x <= Map.m_nr; x++)
		{
			real_t x_um = x * dr_um;
			real_t r_um = hyp(y_um, x_um);
			if (r_um <= r_max_um)
			{
				real_t a_rd = angle(y_um, x_um);
				real_t v = GetClosestPointOpacity(r_um, a_rd, shot, op_type);
				Map.SetAt(y, x, v);
			}
		}
	}

	return;
}

//***************************************************************************************
void CWFExam::CreateWfm2D(CMap2D& Map, const CZernikeSurface& Surface, const real_t dr_um)
{
	real_t r_max_um = Surface.m_r_max_um;

	Map.Create(r_max_um, dr_um);

	for (int y = -Map.m_nr; y <= Map.m_nr; y++)
	{
		real_t y_um = y * dr_um;
		for (int x = -Map.m_nr; x <= Map.m_nr; x++)
		{
			real_t x_um = x * dr_um;
			real_t r_um = hyp(y_um, x_um);
			if (r_um <= r_max_um)
			{
				real_t a_rd = angle(y_um, x_um);
				real_t w_um;
				Surface.GetAt(r_um, a_rd, &w_um, NULL, NULL, NULL, NULL, NULL);
				Map.SetAt(y, x, w_um);
			}
		}
	}
}
//***************************************************************************************

//For showing Presbia 2.4mm...
void CWFExam::CreatePresbiaWfm2D(CMap2D& Map, const CZernikeSurface& Surface, const real_t dr_um)
{
	real_t r_max_um = Surface.m_r_max_um;

	Map.Create(r_max_um, dr_um);

	for (int y = -Map.m_nr; y <= Map.m_nr; y++)
	{
		real_t y_um = y * dr_um;

		for (int x = -Map.m_nr; x <= Map.m_nr; x++)
		{
			real_t x_um = x * dr_um;
			real_t r_um = hyp(y_um, x_um);

			if (r_um <= r_max_um && r_um >= 700)
			{
				real_t a_rd = angle(y_um, x_um);
				real_t w_um;
				Surface.GetAt(r_um, a_rd, &w_um, NULL, NULL, NULL, NULL, NULL);
				Map.SetAt(y, x, w_um);
			}
		}
	}
}

//***************************************************************************************

void CWFExam::CreateWfm3D(CMap3D& Map, const CZernikeSurface& Surface)
{
	real_t r_max_um = Surface.m_r_max_um;

	Map.Create(r_max_um);

	for (int r = 0; r <= Map.m_nr; r++)
	{
		real_t r_um = r * Map.m_dr_um;
		for (int a = 0; a < 360; a++)
		{
			real_t a_rd = _Pi_180 * a;
			real_t w_um;
			Surface.GetAt(r_um, a_rd, &w_um, NULL, NULL, NULL, NULL, NULL);
			Map.SetAt(r, a, w_um);
		}
	}
}

//***************************************************************************************

//For showing Presbia 2.4mm...
void CWFExam::CreatePresbiaWfm3D(CMap3D& Map, const CZernikeSurface& Surface)
{
	real_t r_max_um = Surface.m_r_max_um;

	Map.Create(r_max_um);

	for (int r = 0; r <= Map.m_nr; r++)
	{
		real_t r_um = r * Map.m_dr_um;
		if (r_um >= 700)
		{
			for (int a = 0; a < 360; a++)
			{
				real_t a_rd = _Pi_180 * a;
				real_t w_um;
				Surface.GetAt(r_um, a_rd, &w_um, NULL, NULL, NULL, NULL, NULL);
				Map.SetAt(r, a, w_um);
			}
		}
	}
}

//***************************************************************************************

void CWFExam::CreateRfm2D(CMap2D& Map, const CZernikeSurface& Surface, const real_t dr_um)
{
	real_t r_max_um = Surface.m_r_max_um;

	Map.Create(r_max_um, dr_um);

	for (int y = -Map.m_nr; y <= Map.m_nr; y++)
	{
		real_t y_um = y * dr_um;
		for (int x = -Map.m_nr; x <= Map.m_nr; x++)
		{
			real_t x_um = x * dr_um;
			real_t r_um = hyp(y_um, x_um);
			if (r_um <= r_max_um)
			{
				real_t a_rd = angle(y_um, x_um);
				real_t v = CWFExam::GetRfAt(Surface, r_um, a_rd);
				Map.SetAt(y, x, v);
			}
		}
	}
}

//***************************************************************************************

//For showing Presbia 2.4mm.
void CWFExam::CreatePresbiaRfm2D(CMap2D& Map, const CZernikeSurface& Surface, const real_t dr_um)
{
	real_t r_max_um = Surface.m_r_max_um;

	Map.Create(r_max_um, dr_um);

	for (int y = -Map.m_nr; y <= Map.m_nr; y++)
	{
		real_t y_um = y * dr_um;
		for (int x = -Map.m_nr; x <= Map.m_nr; x++)
		{
			real_t x_um = x * dr_um;
			real_t r_um = hyp(y_um, x_um);
			if (r_um <= r_max_um && r_um >= 700)
			{
				real_t a_rd = angle(y_um, x_um);
				real_t v = CWFExam::GetRfAt(Surface, r_um, a_rd);
				Map.SetAt(y, x, v);
			}
		}
	}
}

//***************************************************************************************

void CWFExam::CreateRfm3D(CMap3D& Map, const CZernikeSurface& Surface)
{
	real_t r_max_um = Surface.m_r_max_um;

	Map.Create(r_max_um);

	for (int r = 0; r <= Map.m_nr; r++)
	{
		real_t r_um = r * Map.m_dr_um;

		if (r_um == 0.0) r_um = 1.0;

		for (int a = 0; a < 360; a++)
		{
			real_t a_rd = _Pi_180 * a;
			real_t v = CWFExam::GetRfAt(Surface, r_um, a_rd);
			Map.SetAt(r, a, v);
		}
	}
}
//***************************************************************************************

//For showing Presbia 2.4mm.
void CWFExam::CreatePresbiaRfm3D(CMap3D& Map, const CZernikeSurface& Surface)
{
	real_t r_max_um = Surface.m_r_max_um;

	Map.Create(r_max_um);

	for (int r = 0; r <= Map.m_nr; r++)
	{
		real_t r_um = r * Map.m_dr_um;
		if (r_um == 0.0) r_um = 1.0;

		if (r_um >= 700)
		{
			for (int a = 0; a < 360; a++)
			{
				real_t a_rd = _Pi_180 * a;
				real_t v = CWFExam::GetRfAt(Surface, r_um, a_rd);
				Map.SetAt(r, a, v);
			}
		}
	}
}

//***************************************************************************************

real_t CWFExam::GetRfAt(const CZernikeSurface& Surface, const real_t r_um, const real_t a_rd)
{
	if (r_um == 0.0) return INVALID_VALUE;

	real_t dw_dr, tilt;
	Surface.GetAt(r_um, a_rd, NULL, NULL, NULL, &dw_dr, NULL, NULL);

	Surface.GetTiltAt(a_rd, &tilt, NULL, NULL);
	dw_dr -= tilt;

	return -1000000.0 * dw_dr / r_um;
}

//***************************************************************************************

real_t CWFExam::GetAnnularRfAt(const CZernikeSurface& Surface, real_t r_um)
{
	if (r_um < 1.0) r_um = 1.0;
	if (r_um > Surface.m_r_max_um) r_um = Surface.m_r_max_um;

	CZernikeSurface SurfaceSphericals;
	SurfaceSphericals = Surface;

	for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
	{
		if (SurfaceSphericals.m_frq[z] != 0)
		{
			SurfaceSphericals.m_c[z] = 0.0;
		}
	}

	return GetRfAt(SurfaceSphericals, r_um, 0.0);
}

//***************************************************************************************

real_t CWFExam::GetAverageRfAt(const CZernikeSurface& Surface, real_t r_um)
{
	if (r_um < 1.0)
	{
		r_um = 1.0;
	}

	if (r_um > Surface.m_r_max_um)
	{
		r_um = Surface.m_r_max_um;
	}

	CZernikeSurface SubSurface;
	SubSurface = Surface;
	SubSurface.ChangeRMaxUm(r_um);

	real_t c = 0.0;
	for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
	{
		int a = SubSurface.m_ord[z];

		if (SubSurface.m_frq[z] == 0 && ((SubSurface.m_ord[z] - 2) % 4) == 0)
		{
			c += SubSurface.m_c[z];
		}
	}

	//real c2 = SubSurface.m_c[3] + SubSurface.m_c[4] + SubSurface.m_c[5] + SubSurface.m_c[12] + SubSurface.m_c[24];

	return -4.0 * c / (0.000001 * r_um);
}

//***************************************************************************************

real_t CWFExam::GetAverageRfAt(const CZernikeSurface& Surface, const real_t r_um, const real_t x0_um, const real_t y0_um)
{
	real_t d_um = 50.0;
	int n = (int)(r_um / d_um);
	real_t rf_avg = 0.0;
	int k = 0;

	for (int j = -n; j <= n; j++)
	{
		real_t dy_um = j * d_um;
		real_t y_um = y0_um + dy_um;

		for (int i = -n; i <= n; i++)
		{
			real_t dx_um = i * d_um;

			if (sqr(dy_um) + sqr(dx_um) > sqr(r_um)) continue;

			real_t x_um = x0_um + dx_um;
			real_t r1_um = hyp(y_um, x_um);

			if (r1_um > Surface.m_r_max_um) continue;

			real_t rf = CWFExam::GetRfAt(Surface, r1_um, angle(y_um, x_um));

			if (rf == INVALID_VALUE) continue;

			rf_avg += rf;
			k++;
		}
	}

	rf_avg /= k;

	return rf_avg;
}

//***************************************************************************************

void CWFExam::GetMinMaxRf(const CZernikeSurface& Surface,
	real_t& rf_min, real_t& x_min_um, real_t& y_min_um, real_t& rf_max, real_t& x_max_um, real_t& y_max_um)
{
	rf_min = DBL_MAX;
	x_min_um = 0.0;
	y_min_um = 0.0;
	rf_max = -DBL_MAX;
	x_max_um = 0.0;
	y_max_um = 0.0;

	real_t d_um = 50.0;
	int n = (int)(Surface.m_r_max_um / d_um);
	int k = 0;

	for (int j = -n; j <= n; j++)
	{
		real_t y_um = j * d_um;
		for (int i = -n; i <= n; i++)
		{
			real_t x_um = i * d_um;
			real_t r_um = hyp(y_um, x_um);

			if (r_um > Surface.m_r_max_um) continue;

			real_t rf = CWFExam::GetRfAt(Surface, r_um, angle(y_um, x_um));

			if (rf == INVALID_VALUE) continue;

			if (rf < rf_min)
			{
				rf_min = rf;
				x_min_um = x_um;
				y_min_um = y_um;
			}

			if (rf > rf_max)
			{
				rf_max = rf;
				x_max_um = x_um;
				y_max_um = y_um;
			}
		}
	}
}

//***************************************************************************************

void CWFExam::CreatePsf(const CZernikeSurface& Surface, CPSF& Psf, const int EEFType)
{
	const int n1 = 128;
	const int n2 = 1024;

	BOOL NeedToIncreasePsfSize = FALSE;
	int n;

	for (n = n1; n <= n2; n <<= 1)
	{
		CreatePsfN(Surface, Psf, n);
		real_t thr = 0.01;
		NeedToIncreasePsfSize = FALSE;

		for (int k = 0; k < n; k++)
		{
			if (Psf.m_Array(k, 0) >= thr || Psf.m_Array(k, n - 1) >= thr ||
				Psf.m_Array(0, k) >= thr || Psf.m_Array(n - 1, k) >= thr)
			{
				NeedToIncreasePsfSize = TRUE;
				break;
			}
		}

		if (NeedToIncreasePsfSize == FALSE) break;
	}

	if (n > n2) n = n2;

	// Ideal PSF
	CZernikeSurface IdealSurface;
	IdealSurface.m_r_max_um = Surface.m_r_max_um;
	CPSF IdealPsf;
	CreatePsfN(IdealSurface, IdealPsf, n);

	// Strehl ratio
	Psf.m_StrehlRatio = Psf.m_vmax / IdealPsf.m_vmax;

	// Encircled energy
	Psf.m_xeef = EEFType == EEF_MAX ? Psf.m_xmax : Psf.m_xcnt;
	Psf.m_yeef = EEFType == EEF_MAX ? Psf.m_ymax : Psf.m_ycnt;

	for (int i = 0; i <= 100; i++)
	{
		Psf.m_EncircledEnergy[i] = 0.0;
	}

	for (int y = 0; y < n; y++)
	{
		real_t y_mn = (y - Psf.m_yeef) * Psf.m_mn_px;

		for (int x = 0; x < n; x++)
		{
			real_t x_mn = (x - Psf.m_xeef) * Psf.m_mn_px;
			real_t r_mn = hyp(y_mn, x_mn);

			int r = (int)r_mn;

			if (fabs(r_mn - r) > 0.000001) r++;

			if (r > 100) r = 100;

			Psf.m_EncircledEnergy[r] += Psf.m_Array(y, x);
		}
	}

	for (int i = 1; i <= 100; i++)
	{
		Psf.m_EncircledEnergy[i] += Psf.m_EncircledEnergy[i - 1];
	}

	real_t k = 100.0 / Psf.m_EncircledEnergy[100];

	if (Psf.m_EncircledEnergy[100] != 0.0)
	{
		for (int i = 0; i <= 100; i++)
		{
			Psf.m_EncircledEnergy[i] *= k;
		}
	}

	// 50%
	for (int i = 1; i <= 100; i++)
	{
		if (Psf.m_EncircledEnergy[i - 1] < 50.0 && 50.0 <= Psf.m_EncircledEnergy[i])
		{
			Psf.m_R50mn = i;
			break;
		}
	}
}

//***************************************************************************************

void CWFExam::CreatePsfN(const CZernikeSurface& Surface, CPSF& Psf, const int n)
{
	const real_t t_um = 8000.0;
	const real_t l_um = 0.555;

	Psf.m_um_px = l_um * CWFExam::m_f_eye_um / t_um;
	Psf.m_px_um = 1.0 / Psf.m_um_px;
	Psf.m_mn_px = Psf.m_mn_um * Psf.m_um_px;
	Psf.m_px_mn = 1.0 / Psf.m_mn_px;

	Psf.m_Array.Create(n, n);

	int o = n >> 1;

	// FOURIER
	Matrix<complex> Array(n, n);

	real_t um_px = t_um / n;

	for (int y = 0; y < n; y++)
	{
		real_t y_um = (y - o) * um_px;

		for (int x = 0; x < n; x++)
		{
			real_t x_um = (x - o) * um_px;
			real_t r_um = hyp(y_um, x_um);

			if (r_um <= Surface.m_r_max_um)
			{
				real_t a_rd = angle(y_um, x_um);
				real_t w_um = 0.0;
				Surface.GetAt(r_um, a_rd, &w_um, NULL, NULL, NULL, NULL, NULL);
				real_t w = _2_Pi * w_um / l_um;
				real_t a = exp(-0.120 * sqr(0.001 * r_um)); // 0.120 - Applegate & Lakshminarayanan, 0.105 - Schweigerling
				Array(y, x) = complex(a * cos(w), -a * sin(w));
			}
		}
	}

	FourierTransform(Array, TRUE);

	for (int y = 0; y < n; y++)
	{
		for (int x = 0; x < n; x++)
		{
			Psf.m_Array(y, x) = sqr_mod(Array(y, x));
		}
	}

	int n2 = n >> 1;

	for (int y = 0; y < n2; y++)
	{
		for (int x = 0; x < n2; x++)
		{
			Swap(Psf.m_Array(y, x), Psf.m_Array(n2 + y, n2 + x));
			Swap(Psf.m_Array(y, n2 + x), Psf.m_Array(n2 + y, x));
		}
	}

	real_t sx = 0.0;
	real_t sy = 0.0;
	real_t s = 0.0;

	Psf.m_vmax = -DBL_MAX;

	for (int y = 0; y < n; y++)
	{
		for (int x = 0; x < n; x++)
		{
			real_t v = Psf.m_Array(y, x);

			if (Psf.m_vmax < v)
			{
				Psf.m_xmax = x;
				Psf.m_ymax = y;
				Psf.m_vmax = v;
			}

			sx += x * v;
			sy += y * v;
			s += v;
		}
	}

	if (s > 0.0)
	{
		Psf.m_xcnt = intRound(sx / s);
		Psf.m_ycnt = intRound(sy / s);
	}
	else
	{
		Psf.m_xcnt = n >> 1;
		Psf.m_ycnt = n >> 1;
	}

	if (Psf.m_vmax != 0.0)
	{
		for (int y = 0; y < n; y++)
		{
			for (int x = 0; x < n; x++)
			{
				Psf.m_Array(y, x) /= Psf.m_vmax;
			}
		}
	}
}

//***************************************************************************************

void CWFExam::CreateMtf(const CZernikeSurface& Surface, CMTF& Mtf)
{
	CPSF Psf;
	CreatePsf(Surface, Psf, 0);

	int n = Psf.m_Array.GetNumRows();

	Matrix<complex> Array(n, n);

	for (int y = 0; y < n; y++)
	{
		for (int x = 0; x < n; x++)
		{
			Array(y, x).re = Psf.m_Array(y, x);
		}
	}

	FourierTransform(Array, TRUE);

	Mtf.m_Array.Create(n, n);
	// if 1 PSF pixel is N minutes then sampling frequency is 1 cycle per N minutes
	// max frequency = 0.5 * sampling frequency = 1 cycle / 2N minutes =
	// = 1 cycle / 2N 1/60th degree = 60 cycles / 2N degrees =
	// = 30 cycles / N degrees = (30 / N) cycles per degree
	Mtf.m_cd_px = (2 * 30.0 * Psf.m_px_mn) / n;
	Mtf.m_px_cd = 1.0 / Mtf.m_cd_px;
	real_t max = -DBL_MAX;

	for (int y = 0; y < n; y++)
	{
		for (int x = 0; x < n; x++)
		{
			real_t v = Mtf.m_Array(y, x) = mod(Array(y, x));
			if (max < v)
			{
				max = v;
			}
		}
	}

	int n2 = n >> 1;
	for (int y = 0; y < n2; y++)
	{
		for (int x = 0; x < n2; x++)
		{
			Swap(Mtf.m_Array(y, x), Mtf.m_Array(n2 + y, n2 + x));
			Swap(Mtf.m_Array(y, n2 + x), Mtf.m_Array(n2 + y, x));
		}
	}

	if (max != 0.0)
	{
		for (int y = 0; y < n; y++)
		{
			for (int x = 0; x < n; x++)
			{
				Mtf.m_Array(y, x) /= max;
			}
		}
	}
}

//***************************************************************************************

void CWFExam::CreateLtr(const CZernikeSurface& Surface, CPSF& Ltr, const int LtrType, const int LtrLine, const int LtrOrientation)
{
	for (int n = 256; n <= 1024; n <<= 1)
	{
		CreateLtrN(Surface, Ltr, LtrType, LtrLine, LtrOrientation, n);
		BOOL b = FALSE;
		real_t thr = 0.01;

		for (int k = 0; k < n; k++)
		{
			if (Ltr.m_Array(k, 0) >= thr || Ltr.m_Array(k, n - 1) >= thr ||
				Ltr.m_Array(0, k) >= thr || Ltr.m_Array(n - 1, k) >= thr)
			{
				b = TRUE;
				break;
			}
		}
		if (b == FALSE) break;
	}
}

//***************************************************************************************

void CWFExam::CreateLtrN(const CZernikeSurface& Surface, CPSF& Ltr, const int LtrType, const int LtrLine,
	const int LtrOrientation, const int n)
{
	CPSF Psf;
	CreatePsfN(Surface, Psf, n);

	Ltr.m_mn_px = Psf.m_mn_px;
	Ltr.m_px_mn = Psf.m_px_mn;
	Ltr.m_um_px = Psf.m_um_px;
	Ltr.m_px_um = Psf.m_px_um;

	Matrix<real_t> ReArray1(n, n);
	ReArray1.Fill(0.0);
	int px_mn = 5;
	real_t mn_px = 1.0 / px_mn;
	int w1 = px_mn * LtrLine / 20;
	int w5 = 5 * w1;

	int c = (n - w5) >> 1;

	if (LtrType == 0)
	{
		for (int i = 0; i < 5; i++)
		{
			int y1 = i * w1;
			int y2 = (i + 1) * w1;

			for (int y = y1; y < y2; y++)
			{
				int m = i & 1 ? w1 : w5;

				for (int x = 0; x < m; x++)
				{
					ReArray1(c + y, c + x) = 1.0;
				}
			}
		}
	}
	else
	{
		CMDC DC;
		DC.CreateTrueColorDC(n, n);
		DC.FillSolidRect(0, 0, n, n, 0x00000000);
		DC.DrawCircle(n >> 1, n >> 1, w1 << 1, w1, 0x00ffffff, NOCOLOR);
		DC.FillSolidRect(n >> 1, (n >> 1) - (w1 >> 1), n >> 1, w1, 0x00000000);

		for (int y = 0; y < n; y++)
		{
			for (int x = 0; x < n; x++)
			{
				ReArray1(y, x) = (real_t)GetRValue(DC.GetPixel(x, y));
			}
		}
	}

	Matrix<real_t> ReArray2(n, n);

	if (LtrOrientation == 0)
	{
		for (int y = 0; y < n; y++)
		{
			for (int x = 0; x < n; x++)
			{
				ReArray2(y, x) = ReArray1(y, x);
			}
		}
	}
	else if (LtrOrientation == 90)
	{
		for (int y = 0; y < n; y++)
		{
			for (int x = 0; x < n; x++)
			{
				ReArray2(y, x) = ReArray1(n - 1 - x, y);
			}
		}
	}
	else if (LtrOrientation == 180)
	{
		for (int y = 0; y < n; y++)
		{
			for (int x = 0; x < n; x++)
			{
				ReArray2(y, x) = ReArray1(n - 1 - y, n - 1 - x);
			}
		}
	}
	else if (LtrOrientation == 270)
	{
		for (int y = 0; y < n; y++)
		{
			for (int x = 0; x < n; x++)
			{
				ReArray2(y, x) = ReArray1(x, n - 1 - y);
			}
		}
	}

	Zoom(ReArray2, ReArray1, Psf.m_px_mn / (real_t)px_mn);

	Matrix<complex> CoArray1(n, n);
	Matrix<complex> CoArray2(n, n);

	for (int y = 0; y < n; y++)
	{
		for (int x = 0; x < n; x++)
		{
			CoArray1(y, x).re = ReArray1(y, x);
			CoArray2(y, x).re = Psf.m_Array(n - 1 - y, x);
		}
	}

	FourierTransform(CoArray1, TRUE);
	FourierTransform(CoArray2, TRUE);

	for (int y = 0; y < n; y++)
	{
		for (int x = 0; x < n; x++)
		{
			CoArray1(y, x) *= CoArray2(y, x);
		}
	}

	FourierTransform(CoArray1, FALSE);

	Ltr.m_Array.Create(n, n);

	real_t max = 0.0;
	for (int y = 0; y < n; y++)
	{
		for (int x = 0; x < n; x++)
		{
			real_t v = Ltr.m_Array(y, x) = CoArray1(y, x).re;
			if (max < v) max = v;
		}
	}

	if (max > 0.0)
	{
		for (int y = 0; y < n; y++)
		{
			for (int x = 0; x < n; x++)
			{
				Ltr.m_Array(y, x) /= max;
			}
		}

		int n2 = n >> 1;

		for (int y = 0; y < n2; y++)
		{
			for (int x = 0; x < n2; x++)
			{
				Swap(Ltr.m_Array(y, x), Ltr.m_Array(n2 + y, n2 + x));
				Swap(Ltr.m_Array(y, n2 + x), Ltr.m_Array(n2 + y, x));
			}
		}
	}
}

//***************************************************************************************

void CWFExam::GetSpheqSphCylAxis(const CZernikeSurface& Surface, const int VertexDistanceUm, const BOOL PositiveCylinder,
	real_t& sph_eq, real_t& sph, real_t& cyl, int& axis)
{
	// Die Differenz zwischen dem neuen Hornhaut-Scheitelabstand und dem alten.
	// Sie ist positiv bei Verlaengerung des Hornhaut-Scheitelabstandes und negativ bei Verkuerzung.

	real_t VertexDistanceM = VertexDistanceUm * 0.000001;

	// SPHERICAL EQUIVALENT
	real_t r_max_m = Surface.m_r_max_um * 0.000001;
	sph_eq = -4.0 * Surface.m_c[4] / r_max_m;

	// CYLINDER
	cyl = 4.0 * hyp(Surface.m_c[3], Surface.m_c[5]) / r_max_m;
	if (!PositiveCylinder) cyl = -cyl;

	// AXIS
	axis = intRound(0.5 * angle(Surface.m_c[3], Surface.m_c[5]) * _180_Pi); // this yields axis � [0, 180]
	if (!PositiveCylinder) axis += 90; // rotate by 90 degrees
	if (axis > 179) axis -= 180;

	// SPHERE
	sph = sph_eq - 0.5 * cyl;

	// wird bei Brillenglaesern mit astigmatischer Wirkung der Hornhaut-Scheitelabstand
	// veraendert, so ist jeder Hauptschnitt einzeln umzurechnen.

	// the strongest and weakest meridional power at corneal plane
	real_t mer1_cornea_dpt = sph;
	real_t mer2_cornea_dpt = sph + cyl;

	// the strongest and weakest meridional power at spectacles plane
	real_t mer1_spects_dpt = mer1_cornea_dpt / (1.0 + VertexDistanceM * mer1_cornea_dpt);
	real_t mer2_spects_dpt = mer2_cornea_dpt / (1.0 + VertexDistanceM * mer2_cornea_dpt);

	sph = mer1_spects_dpt;
	cyl = mer2_spects_dpt - mer1_spects_dpt;
	sph_eq = sph + cyl * 0.5;
}

//***************************************************************************************

void CWFExam::GetSpheqSphCylAxis(const int VertexDistanceUm, const BOOL PositiveCylinder,
	real_t& sph_eq, real_t& sph, real_t& cyl, int& axis) const
{
	CWFExam::GetSpheqSphCylAxis(m_WfSurface, VertexDistanceUm, PositiveCylinder, sph_eq, sph, cyl, axis);
}

//***************************************************************************************

void CWFExam::Average(const Matrix<CWFExam*>& pWFExams)
{
	::ResetCalibration(&m_Calibration);

	m_WfSurface.Reset();
	m_WfSurface.m_r_max_um = 5000.0;
	int n = pWFExams.GetSize();

	for (int i = 0; i < n; i++)
	{
		m_WfSurface += pWFExams[i]->m_WfSurface;
	}

	for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
	{
		m_WfSurface.m_c[z] /= n;
	}

	m_Header.m_ExamID = GUID_NULL;
	m_Header.m_PatientID = pWFExams[0]->m_Header.m_PatientID;
	m_Header.m_CalibrationID = m_Calibration.CalibrationID;
	m_Header.m_ClinicID = GUID_NULL;
	m_Header.m_PhysicianID = GUID_NULL;
	m_Header.m_OperatorID = GUID_NULL;
	m_Header.m_Type = EXAM_TYPE_WF;
	SYSTEMTIME Time;
	::GetLocalTime(&Time);
	m_Header.m_Year = Time.wYear;
	m_Header.m_Month = Time.wMonth;
	m_Header.m_Day = Time.wDay;
	m_Header.m_Hour = Time.wHour;
	m_Header.m_Minute = Time.wMinute;
	m_Header.m_Second = Time.wSecond;
	m_Header.m_Eye = pWFExams[0]->m_Header.m_Eye;
	m_Header.m_Preop = pWFExams[0]->m_Header.m_Preop;
	m_Header.m_SeriesNumber = pWFExams[0]->m_Header.m_SeriesNumber;
	m_Header.m_Note.Empty();
	m_Header.m_AutoNote = "Averaged Exam";
	m_Header.m_SoftwareVersion = SOFTWARE_VERSION;
	m_Header.m_Saved = FALSE;

	m_ScanDiameter = 2.0 * m_WfSurface.m_r_max_um;
	m_AccommTargetPosition = 0.0;
	m_PrecalcSphere = 0.0;
	m_PrecalcCylinder = 0.0;
	m_PrecalcAxis = 0;
	m_WavetouchLensPower = 0.0;
	m_WavetouchLensBaseCurve = WAVETOUCH_LENS_BASECURVE_UNKNOWN;

	CreateScanPattern(m_WfSurface.m_r_max_um);
	m_DetectorType = DT_QUADRANT;
	CScanPoint* pPoint = m_Points.MoveFirst();

	m_NumGoodPoints = 0;//6.2.0 0511

	while (pPoint)
	{
		real_t dw_dx, dw_dy;
		m_WfSurface.GetAt(pPoint->m_r_um, pPoint->m_a_rd, NULL, &dw_dx, &dw_dy, NULL, NULL, NULL);
		pPoint->m_dx_um = -dw_dx * m_Calibration.LADXFocalDistance;
		pPoint->m_dy_um = dw_dy * m_Calibration.LADYFocalDistance;
		pPoint = m_Points.MoveNext();
	}

	m_Image.Destroy();
	m_ColorImage.Destroy();
}

//***************************************************************************************

void CWFExam::CreateComplaints()
{
	real_t SphEq1, Sph1, Cyl1;
	int Axis1;
	CWFExam::GetSpheqSphCylAxis(m_WfSurface, 0, TRUE, SphEq1, Sph1, Cyl1, Axis1);

	CZernikeSurface Surface2;
	Surface2 = m_WfSurface;
	Surface2.ChangeRMaxUm(1000.0);
	real_t SphEq2, Sph2, Cyl2;
	int Axis2;
	CWFExam::GetSpheqSphCylAxis(Surface2, 0, TRUE, SphEq2, Sph2, Cyl2, Axis2);

	// Night miopia
	real_t d = SphEq1 - SphEq2;

	if (d <= -1.75)
	{
		m_Complaints.m_NightMiopia = COMP_SEVERE;
	}
	else if (d <= -1.25)
	{
		m_Complaints.m_NightMiopia = COMP_MODERATE;
	}
	else if (d <= -0.75)
	{
		m_Complaints.m_NightMiopia = COMP_LIGHT;
	}
	else
	{
		m_Complaints.m_NightMiopia = COMP_NORMAL;
	}

	// Night hyperopia
	if (d >= 1.75)
	{
		m_Complaints.m_NightHyperopia = COMP_SEVERE;
	}
	else if (d >= 1.25)
	{
		m_Complaints.m_NightHyperopia = COMP_MODERATE;
	}
	else if (d >= 0.75)
	{
		m_Complaints.m_NightHyperopia = COMP_LIGHT;
	}
	else
	{
		m_Complaints.m_NightHyperopia = COMP_NORMAL;
	}

	// Astigmatism
	real_t Cyl;
	int Axis;
	::CalculateCrossedAstigmatism(Cyl1, Axis1, -Cyl2, Axis2, Cyl, Axis);

	if (Cyl >= 1.50)
	{
		m_Complaints.m_Astigmatism = COMP_SEVERE;
	}
	else if (Cyl >= 0.75)
	{
		m_Complaints.m_Astigmatism = COMP_MODERATE;
	}
	else
	{
		m_Complaints.m_Astigmatism = COMP_NORMAL;
	}

	//
	m_Complaints.m_BlurDoubleVision = m_WfSurface.EvaluateMode(7, 8);
	m_Complaints.m_GlareHalo = m_WfSurface.EvaluateMode(12, -1);
	m_Complaints.m_MixedFocus = m_WfSurface.EvaluateMode(11, 13);
	m_Complaints.m_Starburst = m_WfSurface.EvaluateMode(6, 9);
}

//***************************************************************************************
