//***************************************************************************************

#pragma once

//***************************************************************************************

class CTestResultDlg : public CDialog
{
public:

	CTestResultDlg(CWnd* pParentWnd, BOOL Res, real SphEq, real Sph, real Cyl, int Axis);

private:

	BOOL m_Res;
	real m_SphEq;
	real m_Sph;
	real m_Cyl;
	int m_Axis;

	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
