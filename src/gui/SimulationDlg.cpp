//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "SimulationDlg.h"
#include "MGdi.h"
#include "Data.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CSimulationDlg, CDialog)

	ON_BN_CLICKED(IDC_LOAD, OnLoadButtonClicked)
	ON_WM_PAINT()

END_MESSAGE_MAP()

//***************************************************************************************

CSimulationDlg::CSimulationDlg(CWnd* pParentWnd) : CDialog(IDD_SIMULATION_DLG, pParentWnd)
{
	m_Eye = EYE_OD;
}

//***************************************************************************************

void CSimulationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_OD_RADIO, m_ODRadio);
	DDX_Control(pDX, IDC_OS_RADIO, m_OSRadio);
}

//***************************************************************************************

BOOL CSimulationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	int L = 280;
	int t = 20;
	int w = 40;
	int h = 20;
	int g = 20;

	L += w;
	t -= h + g;
	int l = L;

	CString s;
	for (int i = 3; i < 45; i++) {
		if (CZernikeSurface::m_ord[i] != CZernikeSurface::m_ord[i - 1]) {
			L -= w;
			t += h + g;
			l = L;
		}
		s.Format(_T("%.3f"), m_Surface.GetCUm(i));
		GetDlgItem(IDC_00 + i)->SetWindowText(s);
		GetDlgItem(IDC_00 + i)->SetWindowPos(NULL, l, t, w, h, SWP_NOZORDER);
		l += 2 * w;
	}
	s.Format(_T("%.2f"), m_Surface.m_r_max_um * 0.002);
	GetDlgItem(IDC_D_MM_EDIT)->SetWindowText(s);
	GetDlgItem(IDC_D_MM_EDIT)->SetWindowPos(NULL, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER);

	if (m_Eye == EYE_OD) m_ODRadio.SetCheck(1);
	else m_OSRadio.SetCheck(1);

	return TRUE;
}

//***************************************************************************************

void CSimulationDlg::OnLoadButtonClicked()
{
	CFileDialog* pDlg = new CFileDialog(TRUE, _T("txt"), _T(""), OFN_HIDEREADONLY, _T("Text files (*.txt)|*.txt||"), this);
	pDlg->m_ofn.lpstrInitialDir = MAIN_DIR;

	if (pDlg->DoModal() == IDOK)
	{
		if (m_Surface.LoadTxtFile(pDlg->GetPathName()))
		{
			CString s;

			for (int i = 3; i < 45; i++)
			{
				s.Format(_T("%.10f"), m_Surface.GetCUm(i));
				GetDlgItem(IDC_00 + i)->SetWindowText(s);
			}

			s.Format(_T("%.10f"), 0.002 * m_Surface.m_r_max_um);
			GetDlgItem(IDC_D_MM_EDIT)->SetWindowText(s);
		}
	}
	delete pDlg;
}

//***************************************************************************************

void CSimulationDlg::OnOK()
{
	m_Eye = m_ODRadio.GetCheck() ? EYE_OD : EYE_OS;

	CDialog::OnOK();

	CString s;
	GetDlgItem(IDC_D_MM_EDIT)->GetWindowText(s);
	G_As = s;
	m_Surface.m_r_max_um = 500.0 * atof(G_As);

	for (int i = 3; i < 45; i++)
	{
		GetDlgItem(IDC_00 + i)->GetWindowText(s);
		G_As = s;
		m_Surface.SetCUm(i, atof(G_As));
	}
}

//***************************************************************************************

void CSimulationDlg::OnPaint()
{
	CPaintDC PaintDC(this);

	int L = 280;
	int t = 20;
	int w = 40;
	int h = 20;
	int g = 20;

	L += w; t -= h + g;
	int l = L;

	CString Text;
	RECT Rect;
	CMFont Font(14, 400, "Arial");

	for (int z = 3; z < 45; z++)
	{
		if (CZernikeSurface::m_ord[z] != CZernikeSurface::m_ord[z - 1])
		{
			L -= w;
			t += h + g;
			l = L;
		}
		::SetRect(&Rect, 0, t, l - 5, t + h);

		Text.Format(_T("%i"), z);

		//CStringA TextA(Text);
		::WriteText(PaintDC.m_hDC, Text, Rect, Font, BLACK, 2, NOCOLOR);
		l += 2 * w;
	}
}
//6.2.0 BUG

//***************************************************************************************
