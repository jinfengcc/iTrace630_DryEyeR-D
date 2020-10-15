//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "SettingsIOLSelDlg.h"
#include "Data.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CSettingsIOLSelDlg, CDialog)

	ON_WM_CTLCOLOR()//

	ON_EN_CHANGE(IDC_ALPHA_EDIT1, UpdateAlphaEdit1)
	ON_EN_CHANGE(IDC_ALPHA_EDIT2, UpdateAlphaEdit2)
	ON_EN_CHANGE(IDC_ALPHA_EDIT3, UpdateAlphaEdit3)
	ON_EN_CHANGE(IDC_ALPHA_EDIT4, UpdateAlphaEdit4)

	ON_EN_CHANGE(IDC_CHOA_EDIT1, UpdateHoaCEdit1)
	ON_EN_CHANGE(IDC_CHOA_EDIT2, UpdateHoaCEdit2)

	ON_EN_CHANGE(IDC_IHOA_EDIT1, UpdateHoaIEdit1)
	ON_EN_CHANGE(IDC_IHOA_EDIT2, UpdateHoaIEdit2)

	ON_EN_CHANGE(IDC_Astig_Edit1, UpdateAstigEdit1)
	ON_EN_CHANGE(IDC_Astig_Edit2, UpdateAstigEdit2)

	ON_EN_CHANGE(IDC_SAVAL_EDIT1, UpdateSavalEdit1)
	ON_EN_CHANGE(IDC_SAVAL_EDIT2, UpdateSavalEdit2)
	ON_EN_CHANGE(IDC_SAVAL_EDIT3, UpdateSavalEdit3)
	ON_EN_CHANGE(IDC_SAVAL_EDIT4, UpdateSavalEdit4)
	ON_EN_CHANGE(IDC_SAVAL_EDIT5, UpdateSavalEdit5)
	ON_EN_CHANGE(IDC_SAVAL_EDIT6, UpdateSavalEdit6)
	ON_EN_CHANGE(IDC_SAVAL_EDIT7, UpdateSavalEdit7)
	ON_EN_CHANGE(IDC_SAVAL_EDIT8, UpdateSavalEdit8)

	ON_BN_CLICKED(ID_DEFAULT, OnDefault)

END_MESSAGE_MAP()
//***************************************************************************************


CSettingsIOLSelDlg::CSettingsIOLSelDlg(CWnd* pParentWnd, BOOL Default, real_t IOLSel_Alpha[2], real_t IOLSel_CHOA, real_t IOLSel_IHOA, real_t IOLSel_Astig, real_t IOLSel_Sph_Abbe[4]) :
	CDialog(IDD_SEL_THRE_SET_DLG, pParentWnd)
{
	m_Defult = Default;

	m_IOLSel_Alpha[0] = IOLSel_Alpha[0];//530 IOL SELECTION threshold
	m_IOLSel_Alpha[1] = IOLSel_Alpha[1];//530 IOL SELECTION threshold

	m_IOLSel_CHOA = IOLSel_CHOA;//530 IOL SELECTION threshold
	m_IOLSel_IHOA = IOLSel_IHOA;//530 IOL SELECTION threshold

	m_IOLSel_Astig = IOLSel_Astig;//530 IOL SELECTION threshold

	m_IOLSel_Sph_Abbe[0] = IOLSel_Sph_Abbe[0];//530 IOL SELECTION threshold
	m_IOLSel_Sph_Abbe[1] = IOLSel_Sph_Abbe[1];//530 IOL SELECTION threshold
	m_IOLSel_Sph_Abbe[2] = IOLSel_Sph_Abbe[2];//530 IOL SELECTION threshold
	m_IOLSel_Sph_Abbe[3] = IOLSel_Sph_Abbe[3];//530 IOL SELECTION threshold
}

//***************************************************************************************

void CSettingsIOLSelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ALPHA_EDIT1, m_Alpha_Edit1);
	DDX_Control(pDX, IDC_ALPHA_EDIT2, m_Alpha_Edit2);
	DDX_Control(pDX, IDC_ALPHA_EDIT3, m_Alpha_Edit3);
	DDX_Control(pDX, IDC_ALPHA_EDIT4, m_Alpha_Edit4);

	DDX_Control(pDX, IDC_CHOA_EDIT1, m_Hoa_CEdit1);
	DDX_Control(pDX, IDC_CHOA_EDIT2, m_Hoa_CEdit2);

	DDX_Control(pDX, IDC_IHOA_EDIT1, m_Hoa_IEdit1);
	DDX_Control(pDX, IDC_IHOA_EDIT2, m_Hoa_IEdit2);

	DDX_Control(pDX, IDC_Astig_Edit1, m_Astig_Edit1);
	DDX_Control(pDX, IDC_Astig_Edit2, m_Astig_Edit2);

	DDX_Control(pDX, IDC_SAVAL_EDIT1, m_Saval_Edit[0]);
	DDX_Control(pDX, IDC_SAVAL_EDIT2, m_Saval_Edit[1]);
	DDX_Control(pDX, IDC_SAVAL_EDIT3, m_Saval_Edit[2]);
	DDX_Control(pDX, IDC_SAVAL_EDIT4, m_Saval_Edit[3]);
	DDX_Control(pDX, IDC_SAVAL_EDIT5, m_Saval_Edit[4]);
	DDX_Control(pDX, IDC_SAVAL_EDIT6, m_Saval_Edit[5]);
	DDX_Control(pDX, IDC_SAVAL_EDIT7, m_Saval_Edit[6]);
	DDX_Control(pDX, IDC_SAVAL_EDIT8, m_Saval_Edit[7]);
}

//***************************************************************************************

BOOL CSettingsIOLSelDlg::PreTranslateMessage(MSG* pMsg)
{
	//
	if (pMsg->hwnd == m_Alpha_Edit1.m_hWnd && pMsg->message == WM_LBUTTONDOWN)
	{
		m_Alpha_Edit1Updata = TRUE;
		m_Alpha_Edit3Updata = FALSE;
	}

	if (pMsg->hwnd == m_Alpha_Edit2.m_hWnd && pMsg->message == WM_LBUTTONDOWN)
	{
		m_Alpha_Edit2Updata = TRUE;
		m_Alpha_Edit4Updata = FALSE;
	}

	if (pMsg->hwnd == m_Alpha_Edit3.m_hWnd && pMsg->message == WM_LBUTTONDOWN)
	{
		m_Alpha_Edit3Updata = TRUE;
		m_Alpha_Edit1Updata = FALSE;
	}

	if (pMsg->hwnd == m_Alpha_Edit4.m_hWnd && pMsg->message == WM_LBUTTONDOWN)
	{
		m_Alpha_Edit4Updata = TRUE;
		m_Alpha_Edit2Updata = FALSE;
	}
	//

	//
	if (pMsg->hwnd == m_Hoa_CEdit1.m_hWnd && pMsg->message == WM_LBUTTONDOWN)
	{
		m_HOA_CEdit1Updata = TRUE;
		m_HOA_CEdit2Updata = FALSE;
	}

	if (pMsg->hwnd == m_Hoa_CEdit2.m_hWnd && pMsg->message == WM_LBUTTONDOWN)
	{
		m_HOA_CEdit2Updata = TRUE;
		m_HOA_CEdit1Updata = FALSE;
	}
	//

	//
	if (pMsg->hwnd == m_Hoa_IEdit1.m_hWnd && pMsg->message == WM_LBUTTONDOWN)
	{
		m_HOA_IEdit1Updata = TRUE;
		m_HOA_IEdit2Updata = FALSE;
	}

	if (pMsg->hwnd == m_Hoa_IEdit2.m_hWnd && pMsg->message == WM_LBUTTONDOWN)
	{
		m_HOA_IEdit2Updata = TRUE;
		m_HOA_IEdit1Updata = FALSE;
	}
	//

	//
	if (pMsg->hwnd == m_Astig_Edit1.m_hWnd && pMsg->message == WM_LBUTTONDOWN)
	{
		m_Astig_Edit1Updata = TRUE;
		m_Astig_Edit2Updata = FALSE;
	}

	if (pMsg->hwnd == m_Astig_Edit2.m_hWnd && pMsg->message == WM_LBUTTONDOWN)
	{
		m_Astig_Edit2Updata = TRUE;
		m_Astig_Edit1Updata = FALSE;
	}
	//

	//
	for (int i = 0; i < 8; i++)
	{
		if (pMsg->hwnd == m_Saval_Edit[i].m_hWnd && pMsg->message == WM_LBUTTONDOWN)
		{
			switch (i)
			{
			case 0:
				m_Saval_EditUpdata[0] = TRUE;
				m_Saval_EditUpdata[1] = FALSE;
				break;
			case 1:
				m_Saval_EditUpdata[1] = TRUE;
				m_Saval_EditUpdata[0] = FALSE;
				break;
			case 2:
				m_Saval_EditUpdata[2] = TRUE;
				m_Saval_EditUpdata[3] = FALSE;
				break;
			case 3:
				m_Saval_EditUpdata[3] = TRUE;
				m_Saval_EditUpdata[2] = FALSE;
				break;
			case 4:
				m_Saval_EditUpdata[4] = TRUE;
				m_Saval_EditUpdata[5] = FALSE;
				break;
			case 5:
				m_Saval_EditUpdata[5] = TRUE;
				m_Saval_EditUpdata[4] = FALSE;
				break;
			case 6:
				m_Saval_EditUpdata[6] = TRUE;
				m_Saval_EditUpdata[7] = FALSE;
				break;
			case 7:
				m_Saval_EditUpdata[7] = TRUE;
				m_Saval_EditUpdata[6] = FALSE;
				break;
			}
			break;
		}
	}


	//Number check
	if (pMsg->message == WM_KEYUP)
	{
		int Edit = 0;

		if (GetFocus() == &m_Alpha_Edit1)   Edit = 1;
		else if (GetFocus() == &m_Alpha_Edit2)   Edit = 2;
		else if (GetFocus() == &m_Alpha_Edit3)   Edit = 3;
		else if (GetFocus() == &m_Alpha_Edit4)   Edit = 4;

		else if (GetFocus() == &m_Hoa_CEdit1)    Edit = 7;
		else if (GetFocus() == &m_Hoa_CEdit2)    Edit = 8;

		else if (GetFocus() == &m_Hoa_IEdit1)    Edit = 17;
		else if (GetFocus() == &m_Hoa_IEdit2)    Edit = 18;

		else if (GetFocus() == &m_Astig_Edit1)   Edit = 5;
		else if (GetFocus() == &m_Astig_Edit2)   Edit = 6;

		else if (GetFocus() == &m_Saval_Edit[0]) Edit = 9;
		else if (GetFocus() == &m_Saval_Edit[1]) Edit = 10;
		else if (GetFocus() == &m_Saval_Edit[2]) Edit = 11;
		else if (GetFocus() == &m_Saval_Edit[3]) Edit = 12;
		else if (GetFocus() == &m_Saval_Edit[4]) Edit = 13;
		else if (GetFocus() == &m_Saval_Edit[5]) Edit = 14;
		else if (GetFocus() == &m_Saval_Edit[6]) Edit = 15;
		else if (GetFocus() == &m_Saval_Edit[7]) Edit = 16;

		if (Edit != 0)
		{
			CString DOB;
			GetFocus()->GetWindowText(DOB);

			BOOL IsNum = ::InputIsRealNum(DOB);

			if (!IsNum)
			{
				int len = DOB.GetLength();
				DOB = DOB.Left(len - 1);
				GetFocus()->SetWindowText(DOB);

				if (Edit == 1)  m_Alpha_Edit1.SetSel(-1);
				if (Edit == 2)  m_Alpha_Edit2.SetSel(-1);
				if (Edit == 3)  m_Alpha_Edit3.SetSel(-1);
				if (Edit == 4)  m_Alpha_Edit4.SetSel(-1);

				if (Edit == 7)  m_Hoa_CEdit1.SetSel(-1);
				if (Edit == 8)  m_Hoa_CEdit2.SetSel(-1);

				if (Edit == 17)  m_Hoa_IEdit1.SetSel(-1);
				if (Edit == 18)  m_Hoa_IEdit2.SetSel(-1);

				if (Edit == 5)  m_Astig_Edit1.SetSel(-1);
				if (Edit == 6)  m_Astig_Edit2.SetSel(-1);

				if (Edit == 9)  m_Saval_Edit[0].SetSel(-1);
				if (Edit == 10) m_Saval_Edit[1].SetSel(-1);
				if (Edit == 11) m_Saval_Edit[2].SetSel(-1);
				if (Edit == 12) m_Saval_Edit[3].SetSel(-1);
				if (Edit == 13) m_Saval_Edit[4].SetSel(-1);
				if (Edit == 14) m_Saval_Edit[5].SetSel(-1);
				if (Edit == 15) m_Saval_Edit[6].SetSel(-1);
				if (Edit == 16) m_Saval_Edit[7].SetSel(-1);
			}
		}
	}
	//Done

	return CDialog::PreTranslateMessage(pMsg);
}

//***************************************************************************************

BOOL CSettingsIOLSelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Alpha_Edit1Updata = FALSE;
	m_Alpha_Edit2Updata = FALSE;
	m_Alpha_Edit3Updata = FALSE;
	m_Alpha_Edit4Updata = FALSE;

	m_HOA_CEdit1Updata = FALSE;
	m_HOA_CEdit2Updata = FALSE;

	m_HOA_IEdit1Updata = FALSE;
	m_HOA_IEdit2Updata = FALSE;

	m_Astig_Edit1Updata = FALSE;
	m_Astig_Edit2Updata = FALSE;

	for (int i = 0; i < 8; i++)
		m_Saval_EditUpdata[i] = FALSE;
	//
	CString s;

	//530
	if (m_Defult == TRUE)
	{
		OnDefault();
	}
	else
	{
		//530
		s.Format(_T("%.2f"), m_IOLSel_Alpha[0]);
		m_Alpha_Edit1.SetWindowText(s);
		m_Alpha_Edit3.SetWindowText(s);

		s.Format(_T("%.2f"), m_IOLSel_Alpha[1]);
		m_Alpha_Edit2.SetWindowText(s);
		m_Alpha_Edit4.SetWindowText(s);
		//

		//
		s.Format(_T("%.3f"), m_IOLSel_CHOA);
		m_Hoa_CEdit1.SetWindowText(s);
		m_Hoa_CEdit2.SetWindowText(s);
		//

		//
		s.Format(_T("%.3f"), m_IOLSel_IHOA);
		m_Hoa_IEdit1.SetWindowText(s);
		m_Hoa_IEdit2.SetWindowText(s);
		//

		//
		s.Format(_T("%.2f"), m_IOLSel_Astig);
		m_Astig_Edit1.SetWindowText(s);
		m_Astig_Edit2.SetWindowText(s);
		//

		//
		s.Format(_T("%.2f"), m_IOLSel_Sph_Abbe[0]);
		m_Saval_Edit[0].SetWindowText(s);
		m_Saval_Edit[1].SetWindowText(s);

		s.Format(_T("%.2f"), m_IOLSel_Sph_Abbe[1]);
		m_Saval_Edit[2].SetWindowText(s);
		m_Saval_Edit[3].SetWindowText(s);

		s.Format(_T("%.2f"), m_IOLSel_Sph_Abbe[2]);
		m_Saval_Edit[4].SetWindowText(s);
		m_Saval_Edit[5].SetWindowText(s);

		s.Format(_T("%.2f"), m_IOLSel_Sph_Abbe[3]);
		m_Saval_Edit[6].SetWindowText(s);
		m_Saval_Edit[7].SetWindowText(s);
		//
	}

	return FALSE;
}

//***************************************************************************************

void CSettingsIOLSelDlg::OnOK()
{
	CString s_Alpha1, s_Alpha2, s_CHOA, s_IHOA, s_Astig, s_Sph_Abbe[4];

	//
	m_Alpha_Edit1.GetWindowText(s_Alpha1);
	if (s_Alpha1 == "") goto A;

	m_Alpha_Edit2.GetWindowText(s_Alpha2);
	if (s_Alpha2 == "") goto B;
	//

	//
	m_Hoa_CEdit1.GetWindowText(s_CHOA);
	if (s_CHOA == "") goto C;
	//

	//
	m_Hoa_IEdit1.GetWindowText(s_IHOA);
	if (s_IHOA == "") goto CC;
	//

	//
	m_Astig_Edit1.GetWindowText(s_Astig);
	if (s_Astig == "") goto D;
	//

	//
	m_Saval_Edit[0].GetWindowText(s_Sph_Abbe[0]);
	if (s_Sph_Abbe[0] == "") goto E;

	m_Saval_Edit[2].GetWindowText(s_Sph_Abbe[1]);
	if (s_Sph_Abbe[1] == "") goto F;

	m_Saval_Edit[4].GetWindowText(s_Sph_Abbe[2]);
	if (s_Sph_Abbe[2] == "") goto G;

	m_Saval_Edit[6].GetWindowText(s_Sph_Abbe[3]);
	if (s_Sph_Abbe[3] == "") goto H;
	//

	if (::NotRealNum(s_Alpha1))      goto A1;
	if (::NotRealNum(s_Alpha2))      goto B1;
	if (::NotRealNum(s_CHOA))        goto C1;
	if (::NotRealNum(s_IHOA))        goto CC1;
	if (::NotRealNum(s_Astig))       goto D1;
	if (::NotRealNum(s_Sph_Abbe[0])) goto E1;
	if (::NotRealNum(s_Sph_Abbe[1])) goto F1;
	if (::NotRealNum(s_Sph_Abbe[2])) goto G1;
	if (::NotRealNum(s_Sph_Abbe[3])) goto H1;

	real_t real_Alpha1, real_Alpha2, real_CHOA, real_IHOA, real_Astig, real_Sph_Abbe[4];

	G_As = s_Alpha1;
	real_Alpha1 = atof(G_As);
	G_As = s_Alpha2;
	real_Alpha2 = atof(G_As);

	G_As = s_CHOA;
	real_CHOA = atof(G_As);
	G_As = s_IHOA;
	real_IHOA = atof(G_As);
	G_As = s_Astig;
	real_Astig = atof(G_As);

	G_As = s_Sph_Abbe[0];
	real_Sph_Abbe[0] = atof(G_As);
	G_As = s_Sph_Abbe[1];
	real_Sph_Abbe[1] = atof(G_As);
	G_As = s_Sph_Abbe[2];
	real_Sph_Abbe[2] = atof(G_As);
	G_As = s_Sph_Abbe[3];
	real_Sph_Abbe[3] = atof(G_As);

	if (real_Alpha1 <= real_Alpha2) goto A2;

	if (real_Sph_Abbe[0] >= real_Sph_Abbe[1]) goto E2;
	if (real_Sph_Abbe[1] >= real_Sph_Abbe[2]) goto F2;
	if (real_Sph_Abbe[2] >= real_Sph_Abbe[3]) goto G2;

	//Save the setting res
	m_IOLSel_Alpha[0] = real_Alpha1;
	m_IOLSel_Alpha[1] = real_Alpha2;

	m_IOLSel_CHOA = real_CHOA;
	m_IOLSel_IHOA = real_IHOA;

	m_IOLSel_Astig = real_Astig;

	m_IOLSel_Sph_Abbe[0] = real_Sph_Abbe[0];
	m_IOLSel_Sph_Abbe[1] = real_Sph_Abbe[1];
	m_IOLSel_Sph_Abbe[2] = real_Sph_Abbe[2];
	m_IOLSel_Sph_Abbe[3] = real_Sph_Abbe[3];
	//Done

	CDialog::OnOK();
	return;

A: ::Info("Alpha values can not be empty.");
	m_Alpha_Edit1.SetFocus();
	m_Alpha_Edit1Updata = TRUE;
	m_Alpha_Edit3Updata = FALSE;
	return;

B: ::Info("Alpha values can not be empty.");
	m_Alpha_Edit2.SetFocus();
	m_Alpha_Edit2Updata = TRUE;
	m_Alpha_Edit4Updata = FALSE;
	return;

C: ::Info("Total Corn. HOA values can not be empty.");
	m_Hoa_CEdit1.SetFocus();
	m_HOA_CEdit1Updata = TRUE;
	m_HOA_CEdit2Updata = FALSE;
	return;

CC: ::Info("Internal Corn. HOA values can not be empty.");
	m_Hoa_IEdit1.SetFocus();
	m_HOA_IEdit1Updata = TRUE;
	m_HOA_IEdit2Updata = FALSE;
	return;

D: ::Info("Total Astig. values can not be empty.");
	m_Astig_Edit1.SetFocus();
	m_Astig_Edit1Updata = TRUE;
	m_Astig_Edit2Updata = FALSE;
	return;

E: ::Info("Spherical Aberration values can not be empty.");
	m_Saval_Edit[0].SetFocus();
	m_Saval_EditUpdata[0] = TRUE;
	m_Saval_EditUpdata[1] = FALSE;
	return;

F: ::Info("Spherical Aberration values can not be empty.");
	m_Saval_Edit[2].SetFocus();
	m_Saval_EditUpdata[2] = TRUE;
	m_Saval_EditUpdata[3] = FALSE;
	return;

G: ::Info("Spherical Aberration values can not be empty.");
	m_Saval_Edit[4].SetFocus();
	m_Saval_EditUpdata[4] = TRUE;
	m_Saval_EditUpdata[5] = FALSE;
	return;

H: ::Info("Spherical Aberration values can not be empty.");
	m_Saval_Edit[6].SetFocus();
	m_Saval_EditUpdata[6] = TRUE;
	m_Saval_EditUpdata[7] = FALSE;
	return;

A1: ::Info("Alpha values must be a positive number.");
	m_Alpha_Edit1.SetFocus();
	m_Alpha_Edit1Updata = TRUE;
	m_Alpha_Edit3Updata = FALSE;
	return;

B1: ::Info("Alpha values must be a positive number.");
	m_Alpha_Edit2.SetFocus();
	m_Alpha_Edit2Updata = TRUE;
	m_Alpha_Edit4Updata = FALSE;
	return;

C1: ::Info("Total Corn. HOA values must be a positive number.");
	m_Hoa_CEdit1.SetFocus();
	m_HOA_CEdit1Updata = TRUE;
	m_HOA_CEdit2Updata = FALSE;
	return;

CC1: ::Info("Internal HOA values must be a positive number.");
	m_Hoa_IEdit1.SetFocus();
	m_HOA_IEdit1Updata = TRUE;
	m_HOA_IEdit2Updata = FALSE;
	return;

D1: ::Info("Total Astig. values must be a positive number.");
	m_Astig_Edit1.SetFocus();
	m_Astig_Edit1Updata = TRUE;
	m_Astig_Edit2Updata = FALSE;
	return;

E1: ::Info("Spherical Aberration values must be a positive number.");
	m_Saval_Edit[0].SetFocus();
	m_Saval_EditUpdata[0] = TRUE;
	m_Saval_EditUpdata[1] = FALSE;
	return;

F1: ::Info("Spherical Aberration values must be a positive number.");
	m_Saval_Edit[2].SetFocus();
	m_Saval_EditUpdata[2] = TRUE;
	m_Saval_EditUpdata[3] = FALSE;
	return;

G1: ::Info("Spherical Aberration values must be a positive number.");
	m_Saval_Edit[4].SetFocus();
	m_Saval_EditUpdata[4] = TRUE;
	m_Saval_EditUpdata[5] = FALSE;
	return;

H1: ::Info("Spherical Aberration values must be a positive number.");
	m_Saval_Edit[6].SetFocus();
	m_Saval_EditUpdata[6] = TRUE;
	m_Saval_EditUpdata[7] = FALSE;
	return;

A2: ::Info("The upper limit of Alpha (" + s_Alpha1 + ") must be greater than the lower limit(" + s_Alpha2 + ").");
	m_Alpha_Edit1.SetFocus();
	m_Alpha_Edit1Updata = TRUE;
	m_Alpha_Edit3Updata = FALSE;
	return;

E2: ::Info("The lower limit of Spherical Aberration (" + s_Sph_Abbe[0] + ") must be less than the upper limit(" + s_Sph_Abbe[1] + ").");
	m_Saval_Edit[0].SetFocus();
	m_Saval_EditUpdata[0] = TRUE;
	m_Saval_EditUpdata[1] = FALSE;
	return;

F2: ::Info("The lower limit of Spherical Aberration (" + s_Sph_Abbe[1] + ") must be less than the upper limit(" + s_Sph_Abbe[2] + ").");
	m_Saval_Edit[2].SetFocus();
	m_Saval_EditUpdata[2] = TRUE;
	m_Saval_EditUpdata[3] = FALSE;
	return;

G2: ::Info("The lower limit of Spherical Aberration (" + s_Sph_Abbe[2] + ") must be less than the upper limit(" + s_Sph_Abbe[3] + ").");
	m_Saval_Edit[4].SetFocus();
	m_Saval_EditUpdata[4] = TRUE;
	m_Saval_EditUpdata[5] = FALSE;
	return;
}

//***************************************************************************************

void CSettingsIOLSelDlg::UpdateAlphaEdit1()
{
	if (!m_Alpha_Edit1Updata) return;

	CString s;

	m_Alpha_Edit1.GetWindowText(s);
	m_Alpha_Edit3.SetWindowText(s);
}

//***************************************************************************************

void CSettingsIOLSelDlg::UpdateAlphaEdit2()
{
	if (!m_Alpha_Edit2Updata) return;

	CString s;

	m_Alpha_Edit2.GetWindowText(s);
	m_Alpha_Edit4.SetWindowText(s);
}

//***************************************************************************************

void CSettingsIOLSelDlg::UpdateAlphaEdit3()
{
	if (!m_Alpha_Edit3Updata) return;

	CString s;

	m_Alpha_Edit3.GetWindowText(s);
	m_Alpha_Edit1.SetWindowText(s);
}

//***************************************************************************************

void CSettingsIOLSelDlg::UpdateAlphaEdit4()
{
	if (!m_Alpha_Edit4Updata) return;

	CString s;

	m_Alpha_Edit4.GetWindowText(s);
	m_Alpha_Edit2.SetWindowText(s);
}

//***************************************************************************************

void CSettingsIOLSelDlg::UpdateHoaCEdit1()
{
	if (!m_HOA_CEdit1Updata) return;

	CString s;

	m_Hoa_CEdit1.GetWindowText(s);
	m_Hoa_CEdit2.SetWindowText(s);
}

//***************************************************************************************

void CSettingsIOLSelDlg::UpdateHoaCEdit2()
{
	if (!m_HOA_CEdit2Updata) return;

	CString s;

	m_Hoa_CEdit2.GetWindowText(s);
	m_Hoa_CEdit1.SetWindowText(s);
}

//***************************************************************************************

void CSettingsIOLSelDlg::UpdateHoaIEdit1()
{
	if (!m_HOA_IEdit1Updata) return;

	CString s;

	m_Hoa_IEdit1.GetWindowText(s);
	m_Hoa_IEdit2.SetWindowText(s);
}

//***************************************************************************************

void CSettingsIOLSelDlg::UpdateHoaIEdit2()
{
	if (!m_HOA_IEdit2Updata) return;

	CString s;

	m_Hoa_IEdit2.GetWindowText(s);
	m_Hoa_IEdit1.SetWindowText(s);
}

//***************************************************************************************



void CSettingsIOLSelDlg::UpdateAstigEdit1()
{
	if (!m_Astig_Edit1Updata) return;

	CString s;

	m_Astig_Edit1.GetWindowText(s);
	m_Astig_Edit2.SetWindowText(s);
}

//***************************************************************************************

void CSettingsIOLSelDlg::UpdateAstigEdit2()
{
	if (!m_Astig_Edit2Updata) return;

	CString s;

	m_Astig_Edit2.GetWindowText(s);
	m_Astig_Edit1.SetWindowText(s);
}

//***************************************************************************************

void CSettingsIOLSelDlg::UpdateSavalEdit1()
{
	if (!m_Saval_EditUpdata[0]) return;

	CString s;

	m_Saval_Edit[0].GetWindowText(s);
	m_Saval_Edit[1].SetWindowText(s);
}

//***************************************************************************************

void CSettingsIOLSelDlg::UpdateSavalEdit2()
{
	if (!m_Saval_EditUpdata[1]) return;

	CString s;

	m_Saval_Edit[1].GetWindowText(s);
	m_Saval_Edit[0].SetWindowText(s);
}

//***************************************************************************************

void CSettingsIOLSelDlg::UpdateSavalEdit3()
{
	if (!m_Saval_EditUpdata[2]) return;

	CString s;

	m_Saval_Edit[2].GetWindowText(s);
	m_Saval_Edit[3].SetWindowText(s);
}

//***************************************************************************************

void CSettingsIOLSelDlg::UpdateSavalEdit4()
{
	if (!m_Saval_EditUpdata[3]) return;

	CString s;

	m_Saval_Edit[3].GetWindowText(s);
	m_Saval_Edit[2].SetWindowText(s);
}

//***************************************************************************************

void CSettingsIOLSelDlg::UpdateSavalEdit5()
{
	if (!m_Saval_EditUpdata[4]) return;

	CString s;

	m_Saval_Edit[4].GetWindowText(s);
	m_Saval_Edit[5].SetWindowText(s);
}

//***************************************************************************************

void CSettingsIOLSelDlg::UpdateSavalEdit6()
{
	if (!m_Saval_EditUpdata[5]) return;

	CString s;

	m_Saval_Edit[5].GetWindowText(s);
	m_Saval_Edit[4].SetWindowText(s);
}

//***************************************************************************************

void CSettingsIOLSelDlg::UpdateSavalEdit7()
{
	if (!m_Saval_EditUpdata[6]) return;

	CString s;

	m_Saval_Edit[6].GetWindowText(s);
	m_Saval_Edit[7].SetWindowText(s);
}

//***************************************************************************************

void CSettingsIOLSelDlg::UpdateSavalEdit8()
{
	if (!m_Saval_EditUpdata[7]) return;

	CString s;

	m_Saval_Edit[7].GetWindowText(s);
	m_Saval_Edit[6].SetWindowText(s);
}

//***************************************************************************************

void CSettingsIOLSelDlg::OnDefault()
{
	m_Alpha_Edit1.SetWindowText(_T("0.65"));
	m_Alpha_Edit3.SetWindowText(_T("0.65"));
	m_Alpha_Edit2.SetWindowText(_T("0.5"));
	m_Alpha_Edit4.SetWindowText(_T("0.5"));
	//

	//
	m_Hoa_CEdit1.SetWindowText(_T("0.2"));
	m_Hoa_CEdit2.SetWindowText(_T("0.2"));
	//

	//
	m_Hoa_IEdit1.SetWindowText(_T("0.2"));
	m_Hoa_IEdit2.SetWindowText(_T("0.2"));
	//

	//
	m_Astig_Edit1.SetWindowText(_T("1.0"));
	m_Astig_Edit2.SetWindowText(_T("1.0"));
	//

	//
	m_Saval_Edit[0].SetWindowText(_T("0.14"));
	m_Saval_Edit[1].SetWindowText(_T("0.14"));

	m_Saval_Edit[2].SetWindowText(_T("0.20"));
	m_Saval_Edit[3].SetWindowText(_T("0.20"));

	m_Saval_Edit[4].SetWindowText(_T("0.25"));
	m_Saval_Edit[5].SetWindowText(_T("0.25"));

	m_Saval_Edit[6].SetWindowText(_T("0.30"));
	m_Saval_Edit[7].SetWindowText(_T("0.30"));
	//
}

//***************************************************************************************
//
HBRUSH CSettingsIOLSelDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	int iID = pWnd->GetDlgCtrlID();

	if (iID == IDC_ALPHA_EDIT1 || iID == IDC_ALPHA_EDIT2 || iID == IDC_ALPHA_EDIT3 || iID == IDC_ALPHA_EDIT4 ||
		iID == IDC_Astig_Edit2 || iID == IDC_Astig_Edit1 || iID == IDC_IHOA_EDIT1 || iID == IDC_IHOA_EDIT2 ||
		iID == IDC_CHOA_EDIT1 || iID == IDC_CHOA_EDIT2 || iID == IDC_SAVAL_EDIT1 || iID == IDC_SAVAL_EDIT2 ||
		iID == IDC_SAVAL_EDIT3 || iID == IDC_SAVAL_EDIT4 || iID == IDC_SAVAL_EDIT5 || iID == IDC_SAVAL_EDIT6 ||
		iID == IDC_SAVAL_EDIT7 || iID == IDC_SAVAL_EDIT8
		)
	{
		//pDC->SetTextColor(DARK_GREEN);
	}

	return hbr;
}
//***************************************************************************************

