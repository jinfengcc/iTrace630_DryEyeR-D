//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "PatientDlg.h"
#include "GroupDlg.h"
#include "EyeDlg.h"
#include "Settings.h"
#include "GlobalFunctions.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CPatientDlg, CDialog)

	ON_WM_CTLCOLOR()//

	ON_BN_CLICKED(IDC_NEW_GROUP, OnNewGroupButtonClicked)
	ON_BN_CLICKED(IDC_OD_BUTTON, OnODButtonClicked)
	ON_BN_CLICKED(IDC_OS_BUTTON, OnOSButtonClicked)
	ON_BN_CLICKED(IDC_MALE_CHECK, OnMaleCheckClicked)
	ON_BN_CLICKED(IDC_FEMALE_CHECK, OnFemaleCheckClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CPatientDlg::CPatientDlg(CWnd* pParentWnd, CPatient* pPatient, CEye* pOD, CEye* pOS) :
	CDialog(IDD_PATIENT_DLG, pParentWnd)
{
	m_pPatient = pPatient;
	m_pOD = pOD;
	m_pOS = pOS;
	m_SaveOD = FALSE;
	m_SaveOS = FALSE;
}

//***************************************************************************************

void CPatientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LAST_NAME_EDIT, m_LastNameEdit);
	DDX_Control(pDX, IDC_FIRST_NAME_EDIT, m_FirstNameEdit);
	DDX_Control(pDX, IDC_MID_NAME_EDIT, m_MiddleNameEdit);
	//DDX_Control(pDX, IDC_YEAR_EDIT         , m_YearEdit      );//[cjf***032012]
	DDX_Control(pDX, IDC_MONTH_COMBOBOX, m_MonthComboBox);
	DDX_Control(pDX, IDC_YEAR_COMBOBOX, m_YearComboBox);//[cjf***032012]
	DDX_Control(pDX, IDC_DAYS_COMBOBOX, m_DaysComboBox);//[cjf***032012]
	DDX_Control(pDX, IDC_DATE_YEAR, m_YearsLabel);//530
	DDX_Control(pDX, IDC_DATE_MONTH, m_MonthLabel);//530
	DDX_Control(pDX, IDC_DATE_DAY, m_DaysLabel);//530
												//DDX_Control(pDX, IDC_DAY_EDIT          , m_DayEdit       );//[cjf***032012]
	DDX_Control(pDX, IDC_MALE_CHECK, m_MaleCheck);
	DDX_Control(pDX, IDC_FEMALE_CHECK, m_FemaleCheck);
	DDX_Control(pDX, IDC_CUSTOM_ID_EDIT, m_CustomIdEdit);
	DDX_Control(pDX, IDC_GROUPS_COMBOBOX, m_GroupsComboBox);
	DDX_Control(pDX, IDC_NOTE_EDIT, m_NoteEdit);
	DDX_Control(pDX, IDC_LINE1_EDIT, m_Line1Edit);
	DDX_Control(pDX, IDC_LINE2_EDIT, m_Line2Edit);
	DDX_Control(pDX, IDC_CITY_EDIT, m_CityEdit);
	DDX_Control(pDX, IDC_STATE_COMBOBOX, m_StateComboBox);
	DDX_Control(pDX, IDC_ZIP_EDIT, m_ZipEdit);
	DDX_Control(pDX, IDC_COUNTRY_EDIT, m_CountryEdit);
	DDX_Control(pDX, IDC_HOMEPHONE_EDIT, m_HomePhoneEdit);
	DDX_Control(pDX, IDC_WORKPHONE_EDIT, m_WorkPhoneEdit);
	DDX_Control(pDX, IDC_EMAIL_EDIT, m_EMailEdit);
}

//***************************************************************************************

BOOL CPatientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//Adjust the year/month/day position according to the setting
	if ((::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY) || (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD))
	{
		RECT TempRect;
		this->GetWindowRect(&TempRect);
		int win_Left = TempRect.left;
		int win_top = TempRect.top + 27;

		int w, left, right, Label_top, Label_btm, Combox_top, Combox_btm;

		w = 0;

		m_MonthLabel.GetWindowRect(&TempRect);
		int MonthLabel_w = TempRect.right - TempRect.left;

		left = TempRect.left - win_Left;
		Label_top = TempRect.top - win_top;
		Label_btm = TempRect.bottom - win_top;

		m_MonthComboBox.GetWindowRect(&TempRect);
		int MonthCom_w = TempRect.right - TempRect.left;

		Combox_top = TempRect.top - win_top;
		Combox_btm = TempRect.bottom - win_top;

		w += MonthLabel_w;
		w += MonthCom_w;

		//
		m_DaysLabel.GetWindowRect(&TempRect);
		int DaysLabel_w = TempRect.right - TempRect.left;

		m_DaysComboBox.GetWindowRect(&TempRect);
		int DaysCom_w = TempRect.right - TempRect.left;

		w += DaysLabel_w;
		w += DaysCom_w;

		//
		m_YearsLabel.GetWindowRect(&TempRect);
		int YearLabel_w = TempRect.right - TempRect.left;

		m_YearComboBox.GetWindowRect(&TempRect);
		int YearCom_w = TempRect.right - TempRect.left;

		w += YearLabel_w;
		w += YearCom_w;
		//

		int Total_w = TempRect.right - left - win_Left;
		int in_w = intRound(real_t((Total_w - (w)) / 2.0)) - 2;

		if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
		{
			//day
			right = left + DaysLabel_w;
			::SetRect(&TempRect, left, Label_top, right, Label_btm);
			m_DaysLabel.MoveWindow(&TempRect, 1);

			left = right;
			right += DaysCom_w;
			::SetRect(&TempRect, left, Combox_top, right, Combox_btm);
			//day

			//month
			right += in_w;
			left = right;
			right += MonthLabel_w;
			::SetRect(&TempRect, left, Label_top, right, Label_btm);
			m_MonthLabel.MoveWindow(&TempRect, 1);

			left = right;
			right += MonthCom_w;
			::SetRect(&TempRect, left, Combox_top, right, Combox_btm);
			//month

			//year
			right += in_w;
			left = right;
			right += YearLabel_w;
			::SetRect(&TempRect, left, Label_top, right, Label_btm);
			m_YearsLabel.MoveWindow(&TempRect, 1);

			left = right;
			right += YearCom_w;
			::SetRect(&TempRect, left, Combox_top, right, Combox_btm);
			//year
		}
		else
		{
			//year
			right = left + YearLabel_w;
			::SetRect(&TempRect, left, Label_top, right, Label_btm);
			m_YearsLabel.MoveWindow(&TempRect, 1);


			left = right;
			right += YearCom_w;
			::SetRect(&TempRect, left, Combox_top, right, Combox_btm);
			m_MonthComboBox.MoveWindow(&TempRect, 1);
			//year

			//month
			right += in_w;
			left = right;
			right += MonthLabel_w;
			::SetRect(&TempRect, left, Label_top, right, Label_btm);
			m_MonthLabel.MoveWindow(&TempRect, 1);

			left = right;
			right += MonthCom_w;
			::SetRect(&TempRect, left, Combox_top, right, Combox_btm);
			m_DaysComboBox.MoveWindow(&TempRect, 1);
			//month

			//day
			right += in_w;
			left = right;
			int right = left + DaysLabel_w;
			::SetRect(&TempRect, left, Label_top, right, Label_btm);
			m_DaysLabel.MoveWindow(&TempRect, 1);

			left = right;
			right += DaysCom_w;
			::SetRect(&TempRect, left, Combox_top, right, Combox_btm);
			m_YearComboBox.MoveWindow(&TempRect, 1);
			//day
		}
	}
	//Done

	CString MONTH[12] = {
		{ "Jan" },{ "Feb" },{ "Mar" },{ "Apr" },
		{ "May" },{ "Jun" },{ "Jul" },{ "Aug" },
		{ "Sep" },{ "Oct" },{ "Nov" },{ "Dec" }
	};

	for (int i = 0; i < 12; i++)
	{
		if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
			m_MonthComboBox.AddString(LPCTSTR(MONTH[i]));
		else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
			m_DaysComboBox.AddString(LPCTSTR(MONTH[i]));
		else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
			m_DaysComboBox.AddString(LPCTSTR(MONTH[i]));
	}

	CString DAY[31] =
	{
		{ "01" },{ "02" },{ "03" },{ "04" },{ "05" },{ "06" },{ "07" },{ "08" },{ "09" },{ "10" },
		{ "11" },{ "12" },{ "13" },{ "14" },{ "15" },{ "16" },{ "17" },{ "18" },{ "19" },{ "20" },
		{ "21" },{ "22" },{ "23" },{ "24" },{ "25" },{ "26" },{ "27" },{ "28" },{ "29" },{ "30" },
		{ "31" }
	};

	for (int i = 0; i < 31; i++)
	{
		if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
			m_DaysComboBox.AddString(LPCTSTR(DAY[i]));
		else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
			m_MonthComboBox.AddString(LPCTSTR(DAY[i]));
		else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
			m_YearComboBox.AddString(LPCTSTR(DAY[i]));
	}

	CString YEAR[130] = 
	{
		{ "2029" },{ "2028" },{ "2027" },{ "2026" },{ "2025" },{ "2024" },{ "2023" },{ "2022" },{ "2021" },{ "2020" },
		{ "2019" },{ "2018" },{ "2017" },{ "2016" },{ "2015" },{ "2014" },{ "2013" },{ "2012" },{ "2011" },{ "2010" },
		{ "2009" },{ "2008" },{ "2007" },{ "2006" },{ "2005" },{ "2004" },{ "2003" },{ "2002" },{ "2001" },{ "2000" },
		{ "1999" },{ "1998" },{ "1997" },{ "1996" },{ "1995" },{ "1994" },{ "1993" },{ "1992" },{ "1991" },{ "1990" },
		{ "1989" },{ "1988" },{ "1987" },{ "1986" },{ "1985" },{ "1984" },{ "1983" },{ "1982" },{ "1981" },{ "1980" },
		{ "1979" },{ "1978" },{ "1977" },{ "1976" },{ "1975" },{ "1974" },{ "1973" },{ "1972" },{ "1971" },{ "1970" },
		{ "1969" },{ "1968" },{ "1967" },{ "1966" },{ "1965" },{ "1964" },{ "1963" },{ "1962" },{ "1961" },{ "1960" },
		{ "1959" },{ "1958" },{ "1957" },{ "1956" },{ "1955" },{ "1954" },{ "1953" },{ "1952" },{ "1951" },{ "1950" },
		{ "1949" },{ "1948" },{ "1947" },{ "1946" },{ "1945" },{ "1944" },{ "1943" },{ "1942" },{ "1941" },{ "1940" },
		{ "1939" },{ "1938" },{ "1937" },{ "1936" },{ "1935" },{ "1934" },{ "1933" },{ "1932" },{ "1931" },{ "1930" },
		{ "1929" },{ "1928" },{ "1927" },{ "1926" },{ "1925" },{ "1924" },{ "1923" },{ "1922" },{ "1921" },{ "1920" },
		{ "1919" },{ "1918" },{ "1917" },{ "1916" },{ "1915" },{ "1914" },{ "1913" },{ "1912" },{ "1911" },{ "1910" },
		{ "1909" },{ "1908" },{ "1907" },{ "1906" },{ "1905" },{ "1904" },{ "1903" },{ "1902" },{ "1901" },{ "1900" },
	};

	int StartYear;

	m_CurYear = GetCurYear();

	for (int i = 2018; i < 2030; i++)
	{
		if (m_CurYear == i)
		{
			StartYear = 11 - (i - 2018);

			break;
		}
	}	

	for (int i = StartYear; i < 130; i++)
	{
		if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
			m_YearComboBox.AddString(LPCTSTR(YEAR[i]));
		else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
			m_YearComboBox.AddString(LPCTSTR(YEAR[i]));
		else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
			m_MonthComboBox.AddString(LPCTSTR(YEAR[i]));
	}

	CString STATE[57] = {
		{ "AA" },{ "AE" },{ "AK" },{ "AL" },{ "AP" },{ "AR" },{ "AZ" },{ "CA" },{ "CO" },{ "CT" },
		{ "DC" },{ "DE" },{ "FL" },{ "GA" },{ "GU" },{ "HI" },{ "IA" },{ "ID" },{ "IL" },{ "IN" },
		{ "KS" },{ "KY" },{ "LA" },{ "MA" },{ "MD" },{ "ME" },{ "MI" },{ "MN" },{ "MO" },{ "MS" },
		{ "MT" },{ "NC" },{ "ND" },{ "NE" },{ "NH" },{ "NJ" },{ "NM" },{ "NV" },{ "NY" },{ "OH" },
		{ "OK" },{ "OR" },{ "PA" },{ "PR" },{ "RI" },{ "SC" },{ "SD" },{ "TN" },{ "TX" },{ "UT" },
		{ "VA" },{ "VI" },{ "VT" },{ "WA" },{ "WI" },{ "WV" },{ "WY" }
	};

	for (int i = 0; i < 57; i++) m_StateComboBox.AddString(LPCTSTR(STATE[i]));

	CString s;

	if (m_pPatient->m_PatientID != GUID_NULL)
	{
		m_LastNameEdit.SetWindowText(G_ReadChinese(m_pPatient->m_LastName));
		m_FirstNameEdit.SetWindowText(G_ReadChinese(m_pPatient->m_FirstName));
		m_MiddleNameEdit.SetWindowText(G_ReadChinese(m_pPatient->m_MiddleName));

		if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
		{
			m_YearComboBox.SetCurSel(m_CurYear - m_pPatient->m_BirthYear);
			m_MonthComboBox.SetCurSel(m_pPatient->m_BirthMonth - 1);
			m_DaysComboBox.SetCurSel(m_pPatient->m_BirthDay - 1);
		}
		else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
		{
			m_YearComboBox.SetCurSel(m_CurYear - m_pPatient->m_BirthYear);
			m_DaysComboBox.SetCurSel(m_pPatient->m_BirthMonth - 1);
			m_MonthComboBox.SetCurSel(m_pPatient->m_BirthDay - 1);
		}
		else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
		{
			m_MonthComboBox.SetCurSel(m_CurYear - m_pPatient->m_BirthYear);
			m_DaysComboBox.SetCurSel(m_pPatient->m_BirthMonth - 1);
			m_YearComboBox.SetCurSel(m_pPatient->m_BirthDay - 1);
		}
	}

	switch (m_pPatient->m_Sex)
	{
	case SEX_MALE:
		m_MaleCheck.SetCheck(1);
		break;
	case SEX_FEMALE:
		m_FemaleCheck.SetCheck(1);
		break;
	}

	m_CustomIdEdit.SetWindowText(m_pPatient->m_CustomID);
	CreateGroupsList(m_pPatient->m_GroupID);
	m_NoteEdit.SetWindowText(G_ReadChinese(m_pPatient->m_Note));//6.2.0 For Chinese

	m_Line1Edit.SetWindowText(G_ReadChinese(m_pPatient->m_Address.m_Line1));//6.2.0 For Chinese
	m_Line2Edit.SetWindowText(G_ReadChinese(m_pPatient->m_Address.m_Line2));//6.2.0 For Chinese
	m_CityEdit.SetWindowText(G_ReadChinese(m_pPatient->m_Address.m_City));//6.2.0 For Chinese
	m_StateComboBox.SetWindowText(G_ReadChinese(m_pPatient->m_Address.m_State));//6.2.0 For Chinese
	m_ZipEdit.SetWindowText(G_ReadChinese(m_pPatient->m_Address.m_ZIP));//6.2.0 For Chinese
	m_CountryEdit.SetWindowText(G_ReadChinese(m_pPatient->m_Address.m_Country));//6.2.0 For Chinese
	m_HomePhoneEdit.SetWindowText(m_pPatient->m_HomePhone);
	m_WorkPhoneEdit.SetWindowText(m_pPatient->m_WorkPhone);
	m_EMailEdit.SetWindowText(m_pPatient->m_EMail);

	if (::DB.IsReadOnly())
	{
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDC_NEW_GROUP)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDC_NEW_GROUP)->EnableWindow(TRUE);
	}

	if (m_pPatient->m_PatientID == GUID_NULL)
	{
		s = "";
	}
	else
	{
		::GUIDToStr(m_pPatient->m_PatientID, s);
	}

	SetWindowText(_T("Patient  ") + s);

	return TRUE;
}

//***************************************************************************************

void CPatientDlg::OnNewGroupButtonClicked()
{
	CGroup NewGroup;
	CGroupDlg* pDlg = new CGroupDlg(this, &NewGroup);

	if (pDlg->DoModal() == IDOK)
	{
		::DB.SaveGroup(NewGroup);
		CreateGroupsList(NewGroup.m_GroupID);
	}
	delete pDlg;
}

//***************************************************************************************

void CPatientDlg::OnODButtonClicked()
{
	CEyeDlg* pDlg = new CEyeDlg(this, m_pOD);

	if (pDlg->DoModal() == IDOK)
	{
		m_SaveOD = TRUE;
	}
	delete pDlg;
}

//***************************************************************************************

void CPatientDlg::OnOSButtonClicked()
{
	CEyeDlg* pDlg = new CEyeDlg(this, m_pOS);
	if (pDlg->DoModal() == IDOK)
	{
		m_SaveOS = TRUE;
	}
	delete pDlg;
}

//***************************************************************************************

void CPatientDlg::OnOK()
{
	CString CustomID;
	m_CustomIdEdit.GetWindowText(CustomID);

	if (CustomID.FindOneOf(_T("\\/:*?\"<>|,")) != -1)
	{
		::Error("An ID can't contain any of the following characters: \\/:*?\"<>|,");
		m_CustomIdEdit.SetFocus();
		return;
	}

	CString LastName;
	m_LastNameEdit.GetWindowText(LastName);

	if (LastName.IsEmpty())
	{
		::Error("Please enter last name.");
		m_LastNameEdit.SetFocus();
		return;
	}

	if (LastName.FindOneOf(_T("\\/:*?\"<>|,")) != -1)
	{
		::Error("Last name can't contain any of the following characters: \\/:*?\"<>|,");
		m_LastNameEdit.SetFocus();
		return;
	}

	CString FirstName;
	m_FirstNameEdit.GetWindowText(FirstName);

	if (FirstName.IsEmpty())
	{
		::Error("Please enter first name.");
		m_FirstNameEdit.SetFocus();
		return;
	}

	if (FirstName.FindOneOf(_T("\\/:*?\"<>|,")) != -1)
	{
		::Error("First name can't contain any of the following characters: \\/:*?\"<>|,");
		m_FirstNameEdit.SetFocus();
		return;
	}

	CString MiddleName;
	m_MiddleNameEdit.GetWindowText(MiddleName);

	if (MiddleName.FindOneOf(_T("\\/:*?\"<>|,")) != -1)
	{
		::Error("Middle name can't contain any of the following characters: \\/:*?\"<>|,");
		m_MiddleNameEdit.SetFocus();
		return;
	}

	CString s;
	CTime tm;
	tm = tm.GetCurrentTime();
	int tmInt_Year = tm.GetYear();

	int BirthYear;

	if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
	{
		BirthYear = m_YearComboBox.GetCurSel();
	}
	else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
	{
		BirthYear = m_YearComboBox.GetCurSel();
	}
	else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
	{
		BirthYear = m_MonthComboBox.GetCurSel();
	}

	if (BirthYear == -1)
	{
		if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
		{
			m_YearComboBox.GetWindowText(s);
		}
		else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
		{
			m_YearComboBox.GetWindowText(s);
		}
		else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
		{
			m_MonthComboBox.GetWindowText(s);
		}

		if (s.GetLength() > 4)
		{
			s.Format(_T("%i"), tm.GetYear());
			s += ").";
			::Error("Please enter correct year of birth (1900-" + s);

			if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
			{
				m_YearComboBox.SetFocus();
			}
			else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
			{
				m_YearComboBox.SetFocus();
			}
			else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
			{
				m_MonthComboBox.SetFocus();
			}

			return;
		}

		G_As = s;
		BirthYear = atoi(G_As);

		if (BirthYear > tmInt_Year || BirthYear < 1900)
		{
			s.Format(_T("%i"), tm.GetYear());
			s += ").";
			::Error("Please enter correct year of birth (1900-" + s);
			m_YearComboBox.SetFocus();

			if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
			{
				m_YearComboBox.SetFocus();
			}
			else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
			{
				m_YearComboBox.SetFocus();
			}
			else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
			{
				m_MonthComboBox.SetFocus();
			}

			return;
		}
	}
	else
	{
		if (BirthYear < 0 || BirthYear > 115)
		{
			::Error("Please enter year of birth.");

			if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
			{
				m_YearComboBox.SetFocus();
			}
			else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
			{
				m_YearComboBox.SetFocus();
			}
			else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
			{
				m_MonthComboBox.SetFocus();
			}

			return;
		}

		BirthYear = m_CurYear - BirthYear;
	}

	int BirthMonth;

	if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
	{
		BirthMonth = m_MonthComboBox.GetCurSel();
	}
	else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
	{
		BirthMonth = m_DaysComboBox.GetCurSel();
	}
	else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
	{
		BirthMonth = m_DaysComboBox.GetCurSel();
	}

	if (BirthMonth == -1)
	{
		if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
		{
			m_MonthComboBox.GetWindowText(s);
		}
		else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
		{
			m_DaysComboBox.GetWindowText(s);
		}
		else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
		{
			m_DaysComboBox.GetWindowText(s);
		}

		if (s.GetLength()>3)
		{
			::Error("Please enter correct month of birth (1-12 or Jan-Dec).");

			if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
			{
				m_MonthComboBox.SetFocus();
			}
			else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
			{
				m_DaysComboBox.SetFocus();
			}
			else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
			{
				m_DaysComboBox.SetFocus();
			}

			return;
		}

		s.MakeLower();
		if (s == "jan") s = "1";
		else if (s == "feb") s = "2";
		else if (s == "mar") s = "3";
		else if (s == "apr") s = "4";
		else if (s == "may") s = "5";
		else if (s == "jun") s = "6";
		else if (s == "jul") s = "7";
		else if (s == "aug") s = "8";
		else if (s == "sep") s = "9";
		else if (s == "oct") s = "10";
		else if (s == "nov") s = "11";
		else if (s == "dec") s = "12";

		G_As = s;
		BirthMonth = atoi(G_As);
		if (BirthMonth > 12 || BirthMonth < 1)
		{
			::Error("Please enter correct month of birth (1-12 or Jan-Dec).");

			if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
			{
				m_MonthComboBox.SetFocus();
			}
			else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
			{
				m_DaysComboBox.SetFocus();
			}
			else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
			{
				m_DaysComboBox.SetFocus();
			}

			return;
		}
	}
	else
	{
		if (BirthMonth < 0 || BirthMonth > 11)
		{
			::Error("Please enter month of birth.");

			if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
			{
				m_MonthComboBox.SetFocus();
			}
			else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
			{
				m_DaysComboBox.SetFocus();
			}
			else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
			{
				m_DaysComboBox.SetFocus();
			}

			return;
		}
		BirthMonth++;
	}


	int BirthDay;

	if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
	{
		BirthDay = m_DaysComboBox.GetCurSel();
	}
	else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
	{
		BirthDay = m_MonthComboBox.GetCurSel();
	}
	else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
	{
		BirthDay = m_YearComboBox.GetCurSel();
	}

	if (BirthDay == -1)
	{
		if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
		{
			m_DaysComboBox.GetWindowText(s);
		}
		else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
		{
			m_MonthComboBox.GetWindowText(s);
		}
		else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
		{
			m_YearComboBox.GetWindowText(s);
		}

		if (s.GetLength() > 2)
		{
			::Error("Please enter correct day of birth (1-31).");

			if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
			{
				m_DaysComboBox.SetFocus();
			}
			else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
			{
				m_MonthComboBox.SetFocus();
			}
			else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
			{
				m_YearComboBox.SetFocus();
			}

			return;
		}

		G_As = s;
		BirthDay = atoi(G_As);
		if (BirthDay > 31 || BirthDay < 1)
		{
			::Error("Please enter correct day of birth (1-31).");

			if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
			{
				m_DaysComboBox.SetFocus();
			}
			else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
			{
				m_MonthComboBox.SetFocus();
			}
			else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
			{
				m_YearComboBox.SetFocus();
			}

			return;
		}
	}
	else
	{
		if (BirthDay < 0 || BirthDay > 31)
		{
			::Error("Please enter day of birth.");

			if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
			{
				m_DaysComboBox.SetFocus();
			}
			else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
			{
				m_MonthComboBox.SetFocus();
			}
			else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
			{
				m_YearComboBox.SetFocus();
			}

			return;
		}
		BirthDay++;
	}

	int DayMax[12] = { 31, 28, 31, 30,  31, 30, 31, 31,  30, 31, 30, 31 };
	if (!((BirthYear % 4 == 0) && BirthMonth == 2 && BirthDay == 29))
	{
		if (BirthDay < 1 || BirthDay > DayMax[BirthMonth - 1])
		{
			::Error("Please enter correct day of birth.");

			if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
			{
				m_DaysComboBox.SetFocus();
			}
			else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
			{
				m_MonthComboBox.SetFocus();
			}
			else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
			{
				m_YearComboBox.SetFocus();
			}

			return;
		}
	}


	//make sure the DOB can not be greater than current date
	if (BirthYear == tmInt_Year)
	{
		G_As = tm.Format(_T("%m"));
		int tmInt_Month = atoi(G_As);
		if (BirthMonth > tmInt_Month)
		{
			::Error("The DOB is later than today.");
			return;
		}
		else if (BirthMonth == tmInt_Month)
		{
			G_As = tm.Format(_T("%d"));
			int tmInt_Day = atoi(G_As);
			if (BirthDay > tmInt_Day)
			{
				::Error("The DOB is later than today.");
				return;
			}
		}
	}
	//

	int Sex;
	if (m_MaleCheck.GetCheck()) Sex = SEX_MALE;
	else if (m_FemaleCheck.GetCheck()) Sex = SEX_FEMALE;
	else Sex = SEX_UNKNOWN;


	//check wether the edited or added patient  already has been maintained in the database  
	for (int i = 0; i<::DB.m_Patients.GetSize(); i++)
	{
		CPatient* EachPatient = &::DB.m_Patients[i];

		// if the Patient in the DB is not the current added or edited patient 
		if (EachPatient->m_PatientID != m_pPatient->m_PatientID)
		{
			CString InDataLastName = G_ReadChinese(EachPatient->m_LastName);//6.2.0 for Chinese
			CString InDataMidName = G_ReadChinese(EachPatient->m_MiddleName);//6.2.0 for Chinese
			CString InDataFirstName = G_ReadChinese(EachPatient->m_FirstName);//6.2.0 for Chinese

			CString ThisLastName = LastName;
			CString ThisMidName = MiddleName;
			CString ThisFirstName = FirstName;

			InDataLastName.MakeLower();
			InDataMidName.MakeLower();
			InDataFirstName.MakeLower();

			ThisLastName.MakeLower();
			ThisMidName.MakeLower();
			ThisFirstName.MakeLower();

			if ((EachPatient->m_LastName == LastName) && (EachPatient->m_FirstName == FirstName) && (EachPatient->m_MiddleName == MiddleName))
			{
				if ((EachPatient->m_BirthYear == BirthYear) && (EachPatient->m_BirthMonth == BirthMonth) && (EachPatient->m_BirthDay == BirthDay))
				{
					if (EachPatient->m_Sex == Sex)
					{
						::Error("The Same Patient already exists.");
						return;
					}
				}
			}

			if ((InDataLastName == ThisLastName) && (InDataMidName == ThisMidName) && (InDataFirstName == ThisFirstName))
			{
				s = EachPatient->m_LastName + "." + EachPatient->m_FirstName;

				if (InDataMidName != "")
					s = EachPatient->m_LastName + "." + EachPatient->m_MiddleName + "." + EachPatient->m_FirstName;

				CString s1 = LastName + "." + MiddleName + "." + FirstName;

				s = "A patient named " + s + " already in Database, do you still want to save this patient " + s1 + " ?";
				if (!::Question(s, 0, 1)) return;
			}
		}
	}

	if (m_pPatient->m_PatientID != GUID_NULL)
	{
		if (G_ReadChinese(m_pPatient->m_LastName) != LastName)
		{
			if (!Question("Are sure you want to change patient's last name?", 0, 1))
			{
				LastName = m_pPatient->m_LastName;
				m_LastNameEdit.SetWindowText(LastName);
				return;
			}
		}
		if (G_ReadChinese(m_pPatient->m_FirstName) != FirstName)
		{
			if (!Question("Are sure you want to change patient's first name?", 0, 1))
			{
				FirstName = m_pPatient->m_FirstName;
				m_FirstNameEdit.SetWindowText(FirstName);
				return;
			}
		}
		if (G_ReadChinese(m_pPatient->m_MiddleName) != MiddleName)
		{
			if (!Question("Are sure you want to change patient's middle name?", 0, 1))
			{
				MiddleName = m_pPatient->m_MiddleName;
				m_MiddleNameEdit.SetWindowText(MiddleName);
				return;
			}
		}

		if (m_pPatient->m_BirthYear != BirthYear ||
			m_pPatient->m_BirthMonth != BirthMonth ||
			m_pPatient->m_BirthDay != BirthDay)
		{
			if (!Question("Are sure you want to change patient's date of birth?", 0, 1))
			{
				BirthYear = m_pPatient->m_BirthYear;
				BirthMonth = m_pPatient->m_BirthMonth;
				BirthDay = m_pPatient->m_BirthDay;

				m_YearComboBox.SetCurSel(m_CurYear - BirthYear);
				m_MonthComboBox.SetCurSel(BirthMonth - 1);
				m_DaysComboBox.SetCurSel(BirthDay - 1);

				if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
				{
					m_YearComboBox.SetCurSel(m_CurYear - BirthYear);
					m_MonthComboBox.SetCurSel(BirthMonth - 1);
					m_DaysComboBox.SetCurSel(BirthDay - 1);
				}
				else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
				{
					m_YearComboBox.SetCurSel(m_CurYear - BirthYear);
					m_DaysComboBox.SetCurSel(BirthMonth - 1);
					m_MonthComboBox.SetCurSel(BirthDay - 1);
				}
				else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
				{
					m_MonthComboBox.SetCurSel(m_CurYear - BirthYear);
					m_DaysComboBox.SetCurSel(BirthMonth - 1);
					m_YearComboBox.SetCurSel(BirthDay - 1);
				}

				return;
			}
		}
	}

	m_pPatient->m_CustomID = CustomID;
	m_pPatient->m_LastName = G_ConvertChineseStr(LastName);//6.2.0 Chinese
	m_pPatient->m_FirstName = G_ConvertChineseStr(FirstName);//6.2.0 Chinese
	m_pPatient->m_MiddleName = G_ConvertChineseStr(MiddleName);//6.2.0 Chinese
	m_pPatient->m_BirthYear = BirthYear;
	m_pPatient->m_BirthMonth = BirthMonth;
	m_pPatient->m_BirthDay = BirthDay;
	m_pPatient->m_Sex = Sex;

	//Save the registration date and time to the database
	if (m_pPatient->m_PatientID == GUID_NULL)
	{
		SYSTEMTIME Time;
		::GetLocalTime(&Time);
		m_pPatient->m_RegYear = Time.wYear;
		m_pPatient->m_RegMonth = Time.wMonth;
		m_pPatient->m_RegDay = Time.wDay;
		m_pPatient->m_RegHour = Time.wHour;
		m_pPatient->m_RegMinute = Time.wMinute;
	}
	//[cjf***04302012*003*], Save date & time

	int g = m_GroupsComboBox.GetCurSel();

	if (g < 1 || g > ::DB.m_Groups.GetSize())
	{
		m_pPatient->m_GroupID = GUID_NULL;
	}
	else
	{
		m_pPatient->m_GroupID = ::DB.m_Groups[g - 1].m_GroupID;
	}

	CString Text;

	m_NoteEdit.GetWindowText(Text);
	m_pPatient->m_Note = G_ConvertChineseStr(Text);

	m_Line1Edit.GetWindowText(Text);
	m_pPatient->m_Address.m_Line1 = G_ConvertChineseStr(Text);

	m_Line2Edit.GetWindowText(Text);
	m_pPatient->m_Address.m_Line2 = G_ConvertChineseStr(Text);

	m_CityEdit.GetWindowText(Text);
	m_pPatient->m_Address.m_City = G_ConvertChineseStr(Text);

	m_StateComboBox.GetWindowText(Text);
	m_pPatient->m_Address.m_State = G_ConvertChineseStr(Text);

	m_ZipEdit.GetWindowText(m_pPatient->m_Address.m_ZIP);

	m_CountryEdit.GetWindowText(Text);
	m_pPatient->m_Address.m_Country = G_ConvertChineseStr(Text);

	m_HomePhoneEdit.GetWindowText(m_pPatient->m_HomePhone);

	m_WorkPhoneEdit.GetWindowText(m_pPatient->m_WorkPhone);

	m_EMailEdit.GetWindowText(m_pPatient->m_EMail);

	CDialog::OnOK();
}

//***************************************************************************************
//6.2.0
int CPatientDlg::GetCurYear()
{
	SYSTEMTIME CurTime;
	::GetLocalTime(&CurTime);
	return CurTime.wYear;
}

//***************************************************************************************
void CPatientDlg::CreateGroupsList(const GUID& GroupID)
{
	::DB.LoadGroups();
	m_GroupsComboBox.ResetContent();
	m_GroupsComboBox.AddString(_T(""));
	int k = 0;
	for (int i = 0; i < ::DB.m_Groups.GetSize(); i++) {
		m_GroupsComboBox.AddString(::DB.m_Groups[i].m_Name);
		if (::DB.m_Groups[i].m_GroupID == GroupID) k = i + 1;
	}
	m_GroupsComboBox.SetCurSel(k);
}

//***************************************************************************************

void CPatientDlg::OnMaleCheckClicked()
{
	if (m_MaleCheck.GetCheck()) m_FemaleCheck.SetCheck(0);
}

//***************************************************************************************

void CPatientDlg::OnFemaleCheckClicked()
{
	if (m_FemaleCheck.GetCheck()) m_MaleCheck.SetCheck(0);
}

//***************************************************************************************
//
HBRUSH CPatientDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_EDIT)
	{
		//pDC->SetTextColor(DARK_GREEN);
	}

	return hbr;
}
//

//***************************************************************************************
