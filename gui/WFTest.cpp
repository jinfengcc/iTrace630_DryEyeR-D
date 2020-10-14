//***************************************************************************************

#include "StdAfx.h"
#include "WFTest.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

CWFTest::CWFTest(CHW* pHW)
{
	m_pHW = pHW;

	m_SphEq = 0.0;
	m_Sph = 0.0;
	m_Cyl = 0.0;
	m_Axis = 0;
	m_Res = FALSE;
}

//***************************************************************************************

void CWFTest::Test()
{
	m_Res = FALSE;

	m_WFExam.m_Calibration = m_pHW->m_Calibration;

	m_WFExam.m_sc_x0_um = 0.0;
	m_WFExam.m_sc_y0_um = 0.0;

	real r_max_um = 3000.0;
	m_WFExam.CreateScanPattern(r_max_um);

	int NumPoints = m_WFExam.m_Points.GetSize();
	real* x_um = new real[NumPoints];
	real* y_um = new real[NumPoints];
	CScanPoint* pPoint = m_WFExam.m_Points.MoveFirst();

	for (int p = 0; p < NumPoints; p++)
	{
		x_um[p] = pPoint->x_um();
		y_um[p] = pPoint->y_um();
		pPoint = m_WFExam.m_Points.MoveNext();
	}

	m_Res = m_pHW->DoWFExam(NumPoints, x_um, y_um);
	delete[] x_um;
	delete[] y_um;

	if (m_Res)
	{
		ushort* pArrayX = new ushort[NumPoints * 512];
		ushort* pArrayY = new ushort[NumPoints * 512];

		m_Res = m_pHW->GetLADsData(NumPoints, pArrayX, pArrayY);
		m_WFExam.m_DetectorType = DT_LINEAR_ARRAYS;
		pPoint = m_WFExam.m_Points.MoveFirst();

		for (int p = 0; p < NumPoints; p++)
		{
			memcpy(pPoint->m_LADX.m_Signal, pArrayX + p * 512, 1024);
			memcpy(pPoint->m_LADY.m_Signal, pArrayY + p * 512, 1024);
			pPoint = m_WFExam.m_Points.MoveNext();
		}
		delete[] pArrayX;
		delete[] pArrayY;

		if (m_Res)
		{
			m_WFExam.ProcessPoints();
			real* r_um = new real[NumPoints];
			real* a_rd = new real[NumPoints];
			real* dw_dx = new real[NumPoints];
			real* dw_dy = new real[NumPoints];
			int k = 0;
			pPoint = m_WFExam.m_Points.MoveFirst();
			while (pPoint)
			{
				if (!pPoint->m_AutoBad)
				{
					r_um[k] = pPoint->m_r_um;
					a_rd[k] = pPoint->m_a_rd;
					dw_dx[k] = -pPoint->m_dx_um / m_WFExam.m_Calibration.LADXFocalDistance;
					dw_dy[k] = pPoint->m_dy_um / m_WFExam.m_Calibration.LADYFocalDistance;
					k++;
				}
				pPoint = m_WFExam.m_Points.MoveNext();
			}

			CZernikeSurface Surface;
			if (Surface.CreateXY(ZERNIKE_ORDER_MAX, 3000.0, k, r_um, a_rd, dw_dx, dw_dy, NULL))
			{
				CZernikeSurface CalSurface;
				m_WFExam.CreateCalSurface(CalSurface);
				Surface -= CalSurface;
				Surface.ChangeRMaxUm(2000.0);
				CWFExam::GetSpheqSphCylAxis(Surface, 0, TRUE, m_SphEq, m_Sph, m_Cyl, m_Axis);
				m_Res = fabs(-5.0 - m_SphEq) <= 0.125 && fabs(m_Cyl) <= 0.25;
			}
			else
			{
				m_Res = FALSE;
			}

			delete[] r_um;
			delete[] a_rd;
			delete[] dw_dx;
			delete[] dw_dy;
		}
	}
}

//***************************************************************************************
