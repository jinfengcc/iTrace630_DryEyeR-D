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
	Matrix<real> m_XUm;
	Matrix<real> m_YUm;

	void CreateScanPattern(real RMax);
	void CreateProbePattern(real RMax);
	void CreateCirclePattern(real RMax);
	void CreateCrossPattern(real RMax);
	void CreateLetterPattern(real RMax);
	void CreateSquarePattern(real RMax);
	void CreateVLinePattern(real RMax);
	void CreateHLinePattern(real RMax);
	void CreateLTRBLinePattern(real RMax);
	void CreateLBRTLinePattern(real RMax);

private:

	virtual void RepaintMemDC() override;

};

//***************************************************************************************
