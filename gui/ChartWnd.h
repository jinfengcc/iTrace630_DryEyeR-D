//***************************************************************************************

#pragma once

//***************************************************************************************

#include "DispWnd.h"

//***************************************************************************************

class CChartWnd : public CDispWnd
{
public:

	CChartWnd(RECT& Rect, CWnd* pWnd);

	Matrix<CString> m_MainLabels;
	Matrix<COLORREF> m_Colors;
	CString m_XUnitLabel;
	CString m_YUnitLabel;
	Matrix<CString> m_TLabels;
	Matrix<CString> m_BLabels;
	Matrix<CString> m_LLabels;
	Matrix<real_t> m_Values;
	int m_min;
	int m_max;

private:

	virtual void RepaintMemDC() override;
	void Chart();

};

//***************************************************************************************
