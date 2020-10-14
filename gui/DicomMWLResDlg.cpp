/***************************************************************************************

Autor:         Jinfeng Chen
E-mail:        jinfengcc@gmail.com
Date:          08-16-2017
Description:   Create a dialog for the source of import patient selection.
Since customer has their MWL system for DICOM import
read '//dicomiseasy.blogspot.com/2012/04/dicom-modality-worklist.html'
and '//dicomiseasy.blogspot.com/2011/12/introduction-to-dicom-chapter-5-solving.html'
verison:       6.2.0

***************************************************************************************/

//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "BusyCursor.h"

#include "DicomMWLResDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CDicomMWLResDlg, CDialog)

	ON_BN_CLICKED(IDC_SELALL_CHECK, OnSelAllChkClicked)
	ON_NOTIFY(NM_CLICK, IDC_PATIENTS_LIST, OnListClicked)
	ON_NOTIFY(NM_RCLICK, IDC_PATIENTS_LIST, OnListRClicked)	

END_MESSAGE_MAP()

//***************************************************************************************

void CDicomMWLResDlg::DoDataExchange(CDataExchange* pDX)
{	
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SELALL_CHECK, m_SelAllChk);
}

//***************************************************************************************

CDicomMWLResDlg::CDicomMWLResDlg(CWnd* pParentWnd) : CDialog(IDD_DICOMMWLRESDLG, pParentWnd)
{

}

//***************************************************************************************

BOOL CDicomMWLResDlg::OnInitDialog()
{
	RECT ClientRect;
	GetClientRect(&ClientRect);

	int W = ClientRect.right - ClientRect.left;
	int H = ClientRect.bottom - ClientRect.top;

	//Create the Record list
	int FontSize = intRound(0.1 * H);

	CMFont Font;
	Font.Create(FontSize, 400, "Arial");

	RECT Rect;
	::SetRect(&Rect, intRound(0.05 * W), intRound(0.05 * H), intRound(0.95 * W), intRound(0.88 * H));

	m_RecordList.m_HeaderCtrl.m_sortable = TRUE;
	m_RecordList.SetFont(&Font);
	m_RecordList.CreateWnd(0, Rect, this, IDC_PATIENTS_LIST);

	real w1[7] = { 0.13 * W, 0.10*W, 0.15 * W, 0.15 * W, 0.15 * W, 0.16 * W, 0.06 * W};

	m_RecordList.InsertColumn(0, _T("ID"), intRound(w1[0]));
	m_RecordList.InsertColumn(1, _T("Modality"), intRound(w1[1]));
	m_RecordList.InsertColumn(2, _T("Last name"), intRound(w1[2]));
	m_RecordList.InsertColumn(3, _T("First name"), intRound(w1[3]));
	m_RecordList.InsertColumn(4, _T("Middle name"), intRound(w1[4]));
	m_RecordList.InsertColumn(5, _T("DOB"), intRound(w1[5]));
	m_RecordList.InsertColumn(6, _T("Sex"), intRound(w1[6]));

	m_RecordList.ShowWindow(TRUE);
	//Create the Record list Done
	
	CDialog::OnInitDialog();
	CenterWindow();

	m_SelAll = TRUE;
	m_SelAllChk.SetCheck(TRUE);

	UpdatePatientsList();

	return FALSE;
}

//***************************************************************************************

BOOL CDicomMWLResDlg::PreTranslateMessage(MSG* pMsg)
{
	return CDialog::PreTranslateMessage(pMsg);
}

//***************************************************************************************

void CDicomMWLResDlg::UpdatePatientsList()
{
	CBusyCursor Cursor;

	m_RecordList.DeleteAllItems();
	
	int k = 0;
	for (int p = 0; p < m_RecCount; p++)
	{
		CString ID, Modality, FirstName, LastName, MidName, Dob, Sex;

		GetFiLaMiName(m_DicomInfo[p].m_Patient_Name, FirstName, LastName, MidName);

		m_RecordList.InsertItem(k, m_DicomInfo[p].m_Patient_ID);
		m_RecordList.SetItemText(k, 1, m_DicomInfo[p].m_Study_Date);//wait for registration time
		m_RecordList.SetItemText(k, 2, LastName);//6.2.0 ReadC hinese name
		m_RecordList.SetItemText(k, 3, FirstName);//6.2.0 ReadC hinese name
		m_RecordList.SetItemText(k, 4, MidName);//6.2.0 ReadC hinese name
		m_RecordList.SetItemText(k, 5, m_DicomInfo[p].m_Patient_BirthDate);
		m_RecordList.SetItemText(k, 6, m_DicomInfo[p].m_Patient_Sex);

		k++;		
	}

	if (m_SelAll)
	{
		for (int Row = 0; Row < m_RecCount; Row++)
		{
			m_RecordList.SetItemState(Row, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}

	Invalidate(FALSE);
}


//***********************************************************************************************

void CDicomMWLResDlg::OnSelAllChkClicked()
{
	BOOL Chk = m_SelAllChk.GetCheck();

	if (!Chk)
	{
		m_SelAll = FALSE;

		for (int Row = 0; Row < m_RecCount; Row++)
		{
			m_RecordList.SetItemState(Row, 0, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
	else
	{
		m_SelAll = TRUE;

		for (int Row = 0; Row < m_RecCount; Row++)
		{
			m_RecordList.SetItemState(Row, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}

	//m_SelAllChk.SetCheck(Chk);
}

//***************************************************************************************

void CDicomMWLResDlg::OnListClicked(NMHDR* pNMHDR, LRESULT* pRes)
{
	*pRes = 0;

	int SelNum = 0;

	for (int Row = 0; Row < m_RecCount; Row++)
	{
		if (m_RecordList.GetItemState(Row, LVIS_SELECTED) == LVIS_SELECTED)
		{
			SelNum++;
		}
	}

	if (SelNum == m_RecCount)
	{
		m_SelAll = TRUE;
	}
	else
	{
		m_SelAll = FALSE;
	}

	m_SelAllChk.SetCheck(m_SelAll);
}


//***************************************************************************************

void CDicomMWLResDlg::OnListRClicked(NMHDR* pNMHDR, LRESULT* pRes)
{
	m_SelAll = FALSE;
	m_SelAllChk.SetCheck(m_SelAll);
}

//***************************************************************************************

void CDicomMWLResDlg::OnOK()
{
	m_SelCount = 0;

	for (int Row = 0; Row < m_RecCount; Row++)
	{
		if (m_RecordList.GetItemState(Row, LVIS_SELECTED) == LVIS_SELECTED)
		{
			m_SelCount++;

			m_SelDicom[Row] = TRUE;
		}
		else
		{
			m_SelDicom[Row] = FALSE;
		}
	}

	CDialog::OnOK();
}


//***************************************************************************************

void  CDicomMWLResDlg::GetFiLaMiName(CString FullName, CString & FirstName, CString & LastName, CString & MiddleName)
{
	CString s[4];
	int Len = FullName.GetLength();

	if (Len == 0)
	{
		FirstName = "";
		LastName = "";
		MiddleName = "";

		return;
	}


	TCHAR  *pValue = new TCHAR[Len];

	pValue = (LPTSTR)(LPCTSTR)FullName;

	int t = 0;

	for (int i = 0; i < 4; i++)
	{
		s[i] = "";
	}

	//0x41 A; 0x42 B; 0x43 C; 44 D; 45 E; 46 F; 47 G; 48 H; 49 I; 4A J; 4B K; 4C L;
	//0x4D M; 0x4E N; 0x4F O; 50 P; 51 Q; 52 R; 53 S; 54 T; 55 U; 56 V; 57 W; 58 X;
	//0x59 Y; 0x5A Z;
	for (int i = 0; i < Len; i++)
	{
		TCHAR c = *(pValue + i);

		if (c == '^')
		{
			t++;
		}
		else
		{		
			s[t] += c;
		}
	}



	for (int i = 0; i <= t; i++)
	{
		s[i].Trim();
	}

	LastName = s[0];
	FirstName = s[1];
	MiddleName = s[2];
}

//***************************************************************************************

