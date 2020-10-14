//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "TestResultDlg.h"
#include "MGDI.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CTestResultDlg, CDialog)

	ON_WM_PAINT()

END_MESSAGE_MAP()

//***************************************************************************************

CTestResultDlg::CTestResultDlg(CWnd* pParentWnd, BOOL Res, real SphEq, real Sph, real Cyl, int Axis) :
	CDialog(IDD_TEST_RESULT_DLG, pParentWnd)
{
	m_Res = Res;
	m_SphEq = SphEq;
	m_Sph = Sph;
	m_Cyl = Cyl;
	m_Axis = Axis;
}

//***************************************************************************************

void CTestResultDlg::OnPaint()
{
	CPaintDC PaintDC(this);

	CString s;
	s.Format(_T("SphEq = %.2f   Sph = %.2f   Cyl = %.2f   Axis = %i"), m_SphEq, m_Sph, m_Cyl, m_Axis);

	CMFont Font1(16, 400, "Arial");
	CMFont Font2(18, 800, "Arial");

	RECT Rect;
	GetClientRect(&Rect);

	Rect.top = 10;
	Rect.bottom = 35;
	G_As = s;
	::WriteText(PaintDC.m_hDC, s, Rect, Font1, BLACK, 1, NOCOLOR);//cjf

	Rect.top = Rect.bottom;
	Rect.bottom += 25;
	::WriteText(PaintDC.m_hDC, m_Res ? "PASSED" : "FAILED", Rect, Font2, m_Res ? 0x0000af00 : RED, 1, NOCOLOR);
}

//***************************************************************************************
