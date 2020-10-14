//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "ToricSelDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CToricSelDlg, CDialog)

	ON_BN_CLICKED(IDC_PREOP_RADIO, OnPreRadioClicked)
	ON_BN_CLICKED(IDC_POSTOP_RADIO, OnPostRadioClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CToricSelDlg::CToricSelDlg(CWnd* pParentWnd) : CDialog(IDD_TORIC_DLG, pParentWnd)//
{
}

//***************************************************************************************

void CToricSelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PREOP_RADIO, m_Pre_Radio);
	DDX_Control(pDX, IDC_POSTOP_RADIO, m_Post_Radio);
	DDX_Control(pDX, IDOK, m_OKButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

//***************************************************************************************

BOOL CToricSelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	//int W = ::GetSystemMetrics(SM_CXSCREEN);
	//int H = ::GetSystemMetrics(SM_CYSCREEN);

	//int bw = intRound(0.4 * W);           // button width
	//int bh = intRound(0.4 * H);           // button height

	//int dw = intRound(0.25 * W);           // dialog width
	//int dh = intRound(0.25 * H);           // dialog height

	//SetWindowPos(NULL, bw, bh, dw, dh, SWP_NOZORDER);
	CenterWindow();


	m_Pre_Radio.SetCheck(1);
	m_pre = TRUE;

	return FALSE;
}

//***************************************************************************************
void CToricSelDlg::OnPreRadioClicked()
{
	m_Pre_Radio.SetCheck(1);
	m_pre = TRUE;
}

//***************************************************************************************
void CToricSelDlg::OnPostRadioClicked()
{
	m_Post_Radio.SetCheck(1);
	m_pre = FALSE;
}
//


