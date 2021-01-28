//***************************************************************************************

#pragma once

//***************************************************************************************

class CTestResultDlg : public CDialog
{
public:

	CTestResultDlg(CWnd* pParentWnd, BOOL Res, real_t SphEq, real_t Sph, real_t Cyl, int Axis);

private:

	BOOL m_Res;
	real_t m_SphEq;
	real_t m_Sph;
	real_t m_Cyl;
	int m_Axis;

	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
