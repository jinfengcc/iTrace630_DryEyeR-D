//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MemWnd.h"

//***************************************************************************************

class CGridWnd : public CMemWnd
{
public:

	CGridWnd();

	COLORREF m_PointsColor;
	int m_CurPoint;
	Matrix<real_t> m_XUm;
	Matrix<real_t> m_YUm;

	void CreateScanPattern(real_t RMax);
	void CreateProbePattern(real_t RMax);
	void CreateCirclePattern(real_t RMax);
	void CreateCrossPattern(real_t RMax);
	void CreateLetterPattern(real_t RMax);
	void CreateSquarePattern(real_t RMax);
	void CreateVLinePattern(real_t RMax);
	void CreateHLinePattern(real_t RMax);
	void CreateLTRBLinePattern(real_t RMax);
	void CreateLBRTLinePattern(real_t RMax);

private:

	virtual void RepaintMemDC() override;

};

//***************************************************************************************
