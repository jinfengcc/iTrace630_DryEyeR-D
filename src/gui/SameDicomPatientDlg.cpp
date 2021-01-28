//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "GroupDlg.h"
#include "EyeDlg.h"
#include "Settings.h"
#include "GlobalFunctions.h"

#include "SameDicomPatientDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CSameDicomPatientDlg, CDialog)

END_MESSAGE_MAP()

//***************************************************************************************

CSameDicomPatientDlg::CSameDicomPatientDlg(CWnd* pParentWnd, CPatient* NewPatient, CPatient* OldPatient) :
	CDialog(IDD_DICOMPATIENT_DLG, pParentWnd)
{
	m_NewPatient = NewPatient;
	m_OldPatient = OldPatient;
}

//***************************************************************************************

void CSameDicomPatientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LAST_NAME_COM, m_LastNameEdit);
	DDX_Control(pDX, IDC_FIRST_NAME_COM, m_FirstNameEdit);
	DDX_Control(pDX, IDC_MID_NAME_COM, m_MiddleNameEdit);
	DDX_Control(pDX, IDC_DOB_COM, m_DOBEdit);
	DDX_Control(pDX, IDC_SEX_COM, m_SexEdit);

	DDX_Control(pDX, IDC_STATIC1, m_LastNameText);
	DDX_Control(pDX, IDC_STATIC2, m_MiddleNameText);
	DDX_Control(pDX, IDC_STATIC3, m_FirstNameText);
	DDX_Control(pDX, IDC_STATIC5, m_DOBText);
	DDX_Control(pDX, IDC_STATIC6, m_SexText);

	DDX_Control(pDX, IDC_NOTE_EDIT, m_NoteEdit);
}

//***************************************************************************************

BOOL CSameDicomPatientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString s = "";

	if (m_OldPatient->m_LastName == m_NewPatient->m_LastName)
	{
		m_LastNameEdit.ShowWindow(SW_HIDE);
		m_LastNameText.ShowWindow(SW_HIDE);
		m_SelLN = FALSE;
	}
	else
	{
		m_LastNameEdit.SetWindowText(G_ReadChinese(m_OldPatient->m_LastName));
		m_LastNameEdit.AddString(G_ReadChinese(m_NewPatient->m_LastName));
		m_SelLN = TRUE;
	}
	//

	if (m_OldPatient->m_FirstName == m_NewPatient->m_FirstName)
	{
		m_FirstNameEdit.ShowWindow(SW_HIDE);
		m_FirstNameText.ShowWindow(SW_HIDE);
		m_SelFN = FALSE;
	}
	else
	{
		m_FirstNameEdit.SetWindowText(G_ReadChinese(m_OldPatient->m_FirstName));
		m_FirstNameEdit.AddString(G_ReadChinese(m_NewPatient->m_FirstName));
		m_SelFN = TRUE;
	}
	//

	if (m_OldPatient->m_MiddleName == m_NewPatient->m_MiddleName)
	{
		m_MiddleNameEdit.ShowWindow(SW_HIDE);
		m_MiddleNameText.ShowWindow(SW_HIDE);
		m_SelMN = FALSE;
	}
	else
	{
		m_MiddleNameEdit.SetWindowText(G_ReadChinese(m_OldPatient->m_MiddleName));
		m_MiddleNameEdit.AddString(G_ReadChinese(m_NewPatient->m_MiddleName));
		m_SelMN = TRUE;
	}

	//DOB
	if ((m_OldPatient->m_BirthYear == m_NewPatient->m_BirthYear) &&
		(m_OldPatient->m_BirthMonth == m_NewPatient->m_BirthMonth) &&
		(m_OldPatient->m_BirthDay == m_NewPatient->m_BirthDay)
		)
	{
		m_DOBEdit.ShowWindow(SW_HIDE);
		m_DOBText.ShowWindow(SW_HIDE);
		m_SelDOB = FALSE;
	}
	else
	{
		CString OldDob = DateToStr(m_OldPatient->m_BirthYear, m_OldPatient->m_BirthMonth, m_OldPatient->m_BirthDay);
		CString NewDob = DateToStr(m_NewPatient->m_BirthYear, m_NewPatient->m_BirthMonth, m_NewPatient->m_BirthDay);

		m_DOBEdit.SetWindowText(OldDob);
		m_DOBEdit.AddString(NewDob);
		m_SelDOB = TRUE;
	}
	//DOB Done

	//Sex
	if (m_OldPatient->m_Sex == m_NewPatient->m_Sex)
	{
		m_SexEdit.ShowWindow(SW_HIDE);
		m_SexText.ShowWindow(SW_HIDE);
		m_SelSex = FALSE;
	}
	else
	{
		CString OldSex;
		CString NewSex;

		OldSex = m_OldPatient->m_Sex == SEX_FEMALE ? "Female" : "Male";
		NewSex = m_NewPatient->m_Sex == SEX_FEMALE ? "Female" : "Male";

		m_SexEdit.SetWindowText(OldSex);
		m_SexEdit.AddString(NewSex);
		m_SelSex = TRUE;
	}

	//Note
	CString NewNote = "[Dicom:" + m_NewPatient->m_Note + "]";

	CString OldNote = m_OldPatient->m_Note;
	OldNote.Replace(_T("*"), _T(""));
	int count = OldNote.Find(NewNote, 0);

	s = m_OldPatient->m_Note;

	if (count == -1)
	{
		s += NewNote;
	}

	m_NoteEdit.SetWindowText(G_ReadChinese(s));

	//Title
	::GUIDToStr(m_OldPatient->m_PatientID, s);

	SetWindowText(_T("Patient  ") + s);

	//Registration time
	m_NewPatient->m_RegYear = m_OldPatient->m_RegYear;
	m_NewPatient->m_RegMonth = m_OldPatient->m_RegMonth;
	m_NewPatient->m_RegDay = m_OldPatient->m_RegDay;
	m_NewPatient->m_RegHour = m_OldPatient->m_RegHour;
	m_NewPatient->m_RegMinute = m_OldPatient->m_RegMinute;

	return TRUE;
}

//***************************************************************************************

void CSameDicomPatientDlg::OnOK()
{
	CString LastName;
	if (m_SelLN) m_LastNameEdit.GetWindowText(LastName);

	CString FirstName;
	if (m_SelFN) m_FirstNameEdit.GetWindowText(FirstName);

	CString MiddleName;
	if (m_SelMN) m_MiddleNameEdit.GetWindowText(MiddleName);

	int DOBIndex;
	if (m_SelDOB) DOBIndex = m_DOBEdit.GetCurSel();

	int SexIndex;
	if (m_SelSex) SexIndex = m_SexEdit.GetCurSel();

	CString Note;
	m_NoteEdit.GetWindowText(Note);

	if (m_SelLN)
	{
		m_NewPatient->m_LastName = G_ConvertChineseStr(LastName);
	}

	if (m_SelFN)
	{
		m_NewPatient->m_FirstName = G_ConvertChineseStr(FirstName);
	}

	if (m_SelMN)
	{
		m_NewPatient->m_MiddleName = G_ConvertChineseStr(MiddleName);
	}

	if (m_SelDOB && DOBIndex == 1)
	{
		m_NewPatient->m_BirthYear = m_OldPatient->m_BirthYear;
		m_NewPatient->m_BirthMonth = m_OldPatient->m_BirthMonth;
		m_NewPatient->m_BirthDay = m_OldPatient->m_BirthDay;
	}

	if (m_SelSex && SexIndex == 1)
	{
		m_NewPatient->m_Sex = m_OldPatient->m_Sex;
	}

	m_NewPatient->m_Note = G_ConvertChineseStr(Note);

	CDialog::OnOK();
}

//***************************************************************************************
