/*******************************************************************
Autor:          Jinfeng Chen
E-mail:         jinfengcc@gmail.com
Date:           10-13-2015
Software Ver:   
Description:    This dialog is used to transferr exam

*****************************************************************************************/


//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "Data.h"
#include "Settings.h"

#include "TranExamDlg.h"

//***************************************************************************************

BEGIN_MESSAGE_MAP(CTranExamDlg, CDialog)

	ON_BN_CLICKED(IDC_ODOS_RADIO, OnOdOsRadioClicked)
	ON_BN_CLICKED(IDC_TRANEXAM_RADIO, OnTransRadioClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CTranExamDlg::CTranExamDlg(CWnd* pParentWnd) : CDialog(IDD_TRANEXAM_DLG, pParentWnd)
{
}

//***************************************************************************************

void CTranExamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ODOS_RADIO, m_OdOsBtn);
	DDX_Control(pDX, IDC_TRANEXAM_RADIO, m_TransBtn);
}

//***************************************************************************************

BOOL CTranExamDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_OdOsBtn.SetCheck(TRUE);
	m_OdOsIsSelected = TRUE;

	// patients list
	RECT Rect, ListRect;

	this->GetWindowRect(&Rect);
	this->ScreenToClient(&Rect);

	int W = Rect.right - Rect.left;
	int H = Rect.bottom - Rect.top;

	int Top = intRound(0.25*H);
	int Buttom = H - intRound(0.2*H);
	int Left = 10;
	int Right = intRound(0.95*W);
	int ListW = Right - Left;

	int FontSize = intRound(0.05 * H);
	CMFont Font;
	Font.Create(FontSize, 400, "Arial");

	::SetRect(&ListRect, Left, Top, Right, Buttom);

	m_PatientsList.m_HeaderCtrl.m_sortable = TRUE;
	m_PatientsList.SetFont(&Font);
	m_PatientsList.CreateWnd(0, ListRect, this, IDC_PATIENTS_LIST);

	real w1[5] = { 0.10 * ListW, 0.20*ListW, 0.23 * ListW, 0.23 * ListW, 0.23 * ListW };

	m_PatientsList.InsertColumn(0, _T("ID"), intRound(w1[0]));
	m_PatientsList.InsertColumn(1, _T("Reg. Date"), intRound(w1[1]));
	m_PatientsList.InsertColumn(2, _T("Last name"), intRound(w1[2]));
	m_PatientsList.InsertColumn(3, _T("First name"), intRound(w1[3]));
	m_PatientsList.InsertColumn(4, _T("Middle name"), intRound(w1[4]));

	UpdatePatientsList();

	m_PatientsList.ShowWindow(FALSE);
	// patients list Done

	return TRUE;
}
//***************************************************************************************

void CTranExamDlg::OnOdOsRadioClicked()
{
	if (m_OdOsIsSelected) return;
	m_PatientsList.ShowWindow(FALSE);
	m_OdOsIsSelected = TRUE;
}

//***************************************************************************************

void CTranExamDlg::OnTransRadioClicked()
{
	if (!m_OdOsIsSelected) return;
	m_PatientsList.ShowWindow(TRUE);
	m_OdOsIsSelected = FALSE;
}

//***************************************************************************************

void CTranExamDlg::OnOK()
{
	if (m_OdOsBtn.GetCheck())
	{
		m_Res = 0;
	}
	else
	{
		int m = m_PatientsList.GetSelectionMark();

		if (m == -1)
		{
			if (::Question("You did not select any patient. Do you want to give up this transform?", 1, 1))
			{
				m_Res = -1;
			}
			else return;
		}
		else
		{
			m_Res = 1;
			m_P = (int)m_PatientsList.GetItemData(m);
		}
	}

	CDialog::OnOK();
}

//***************************************************************************************

void CTranExamDlg::UpdatePatientsList()
{
	m_PatientsList.DeleteAllItems();

	::DB.LoadPatients();

	int TotPatCount = ::DB.m_Patients.GetSize();//[5.1.1]

	CString Note;
	CString RegDate;
	int k = 0;
	for (int p = 0; p < TotPatCount; p++)
	{
		CPatient* pPatient = &::DB.m_Patients[p];

		CString RegDate(::DateToStr(pPatient->m_RegYear, pPatient->m_RegMonth, pPatient->m_RegDay));

		m_PatientsList.InsertItem(k, pPatient->m_CustomID);
		m_PatientsList.SetItemText(k, 1, RegDate);//[cjf***04262012*001*],wait for registration time
		m_PatientsList.SetItemText(k, 2, pPatient->m_LastName);//[cjf***04262012*001*] 1->2
		m_PatientsList.SetItemText(k, 3, pPatient->m_FirstName);//[cjf***04262012*001*] 2->3
		m_PatientsList.SetItemText(k, 4, pPatient->m_MiddleName);//[cjf***04262012*001*] 3->4

		m_PatientsList.SetItemData(k, p);
		k++;
	}

	//m_PatientsList.SortItems(CompareProc, (LPARAM)this);

	Invalidate(FALSE);//[cjf***04242012] 
}