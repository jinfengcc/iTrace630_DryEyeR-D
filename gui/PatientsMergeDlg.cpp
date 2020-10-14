/*******************************************************************
Autor:          Jinfeng Chen
E-mail:         jinfengcc@gmail.com
Date:           10-07-2015
Software Ver:   
Description:    This dialog is used to merge two or more patients to be one patient.

*****************************************************************************************/


//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "PatientsMergeDlg.h"
#include "GroupDlg.h"
#include "EyeDlg.h"
#include "Settings.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CPatientsMergeDlg, CDialog)

	ON_BN_CLICKED(IDC_NEW_GROUP, OnNewGroupButtonClicked)
	ON_BN_CLICKED(IDC_OD_BUTTON, OnODButtonClicked)
	ON_BN_CLICKED(IDC_OS_BUTTON, OnOSButtonClicked)
	ON_BN_CLICKED(IDC_MALE_CHECK, OnMaleCheckClicked)
	ON_BN_CLICKED(IDC_FEMALE_CHECK, OnFemaleCheckClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CPatientsMergeDlg::CPatientsMergeDlg(CWnd* pParentWnd, CPatient pPatient[100], int nPatients, CEye* pOD, CEye* pOS, CPatient* resPatient) :
	CDialog(IDD_MERGE_DLG, pParentWnd)
{
	m_nPatients = nPatients;
	for (int i = 0; i < nPatients; i++)
	{
		m_pPatient[i] = pPatient[i];
	}
	m_resPatient = resPatient;
	m_pOD = pOD;
	m_pOS = pOS;
	m_SaveOD = FALSE;
	m_SaveOS = FALSE;
}

//***************************************************************************************

void CPatientsMergeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LAST_NAME_COMBOBOX, m_LastNameComboBox);

	DDX_Control(pDX, IDC_FIRST_NAME_COMBOBOX, m_FirstNameComboBox);
	DDX_Control(pDX, IDC_MID_NAME_COMBOBOX, m_MiddleComboBox);

	DDX_Control(pDX, IDC_MONTH_COMBOBOX, m_MonthComboBox);
	DDX_Control(pDX, IDC_YEAR_COMBOBOX, m_YearComboBox);//[cjf***032012]
	DDX_Control(pDX, IDC_DAYS_COMBOBOX, m_DaysComboBox);//[cjf***032012]
	DDX_Control(pDX, IDC_DATE_YEAR, m_YearsLabel);//530
	DDX_Control(pDX, IDC_DATE_MONTH, m_MonthLabel);//530
	DDX_Control(pDX, IDC_DATE_DAY, m_DaysLabel);//530

	DDX_Control(pDX, IDC_MALE_CHECK, m_MaleCheck);
	DDX_Control(pDX, IDC_FEMALE_CHECK, m_FemaleCheck);
	DDX_Control(pDX, IDC_CUSTOM_ID_COMBOBOX, m_CustomIdComboBox);
	DDX_Control(pDX, IDC_GROUPS_COMBOBOX, m_GroupsComboBox);
	DDX_Control(pDX, IDC_NOTE_EDIT, m_NoteEdit);

	DDX_Control(pDX, IDC_LINE1_COMBOBOX, m_Line1ComboBox);
	DDX_Control(pDX, IDC_LINE2_COMBOBOX, m_Line2ComboBox);
	DDX_Control(pDX, IDC_CITY_COMBOBOX, m_CityComboBox);
	DDX_Control(pDX, IDC_STATE_COMBOBOX, m_StateComboBox);
	DDX_Control(pDX, IDC_ZIP_COMBOBOX, m_ZipComboBox);
	DDX_Control(pDX, IDC_COUNTRY_COMBOBOX, m_CountryComboBox);
	DDX_Control(pDX, IDC_HOMEPHONE_COMBOBOX, m_HomePhoneComboBox);
	DDX_Control(pDX, IDC_WORKPHONE_COMBOBOX, m_WorkPhoneComboBox);
	DDX_Control(pDX, IDC_EMAIL_COMBOBOX, m_EMailComboBox);
}

//***************************************************************************************

BOOL CPatientsMergeDlg::OnInitDialog()
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
		//	
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

		//
		m_DaysLabel.GetWindowRect(&TempRect);
		int DaysLabel_w = TempRect.right - TempRect.left;

		m_DaysComboBox.GetWindowRect(&TempRect);
		int DaysCom_w = TempRect.right - TempRect.left;

		w += DaysLabel_w;
		w += DaysCom_w;
		//

		//
		m_YearsLabel.GetWindowRect(&TempRect);
		int YearLabel_w = TempRect.right - TempRect.left;

		m_YearComboBox.GetWindowRect(&TempRect);
		int YearCom_w = TempRect.right - TempRect.left;

		w += YearLabel_w;
		w += YearCom_w;
		//

		int Total_w = TempRect.right - left - win_Left;

		int in_w = intRound(real((Total_w - (w)) / 2.0)) - 2;
		//

		if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
		{
			//day
			right = left + DaysLabel_w;
			::SetRect(&TempRect, left, Label_top, right, Label_btm);
			m_DaysLabel.MoveWindow(&TempRect, 1);

			left = right;
			right += DaysCom_w;
			::SetRect(&TempRect, left, Combox_top, right, Combox_btm);
			//m_DaysComboBox.MoveWindow(&TempRect, 1);
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
			//m_MonthComboBox.MoveWindow(&TempRect, 1);
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
			//m_YearComboBox.MoveWindow(&TempRect, 1);
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
			//m_YearComboBox.MoveWindow(&TempRect, 1);
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
			//m_MonthComboBox.MoveWindow(&TempRect, 1);
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
			//m_DaysComboBox.MoveWindow(&TempRect, 1);
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

	CString YEAR[118] = {
		{ "2017" },{ "2016" },{ "2015" },{ "2014" },{ "2013" },{ "2012" },{ "2011" },{ "2010" },
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
	for (int i = 0; i < 118; i++)
	{
		if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
			m_YearComboBox.AddString(LPCTSTR(YEAR[i]));
		else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
			m_YearComboBox.AddString(LPCTSTR(YEAR[i]));
		else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
			m_MonthComboBox.AddString(LPCTSTR(YEAR[i]));
	}
	//[cjf***032012] Done

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

	for (int i = 0; i < m_nPatients; i++)
	{
		if (m_LastNameComboBox.FindString(0, m_pPatient[i].m_LastName) == -1)
		{
			m_LastNameComboBox.AddString(m_pPatient[i].m_LastName);
		}

		if (m_FirstNameComboBox.FindString(0, m_pPatient[i].m_FirstName) == -1)
		{
			m_FirstNameComboBox.AddString(m_pPatient[i].m_FirstName);
		}

		if (m_MiddleComboBox.FindString(0, m_pPatient[i].m_MiddleName) == -1)
		{
			m_MiddleComboBox.AddString(m_pPatient[i].m_MiddleName);
		}
	}
	m_LastNameComboBox.SetCurSel(0);
	m_FirstNameComboBox.SetCurSel(0);
	m_MiddleComboBox.SetCurSel(0);

	if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
	{
		m_YearComboBox.SetCurSel(2015 - m_pPatient[0].m_BirthYear);//[cjf***03272012]
		m_MonthComboBox.SetCurSel(m_pPatient[0].m_BirthMonth - 1);//[cjf***03272012]  
		m_DaysComboBox.SetCurSel(m_pPatient[0].m_BirthDay - 1);    //[cjf***03272012]  
	}
	else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
	{
		m_YearComboBox.SetCurSel(2015 - m_pPatient[0].m_BirthYear);//[cjf***03272012]
		m_DaysComboBox.SetCurSel(m_pPatient[0].m_BirthMonth - 1);//[cjf***03272012]  
		m_MonthComboBox.SetCurSel(m_pPatient[0].m_BirthDay - 1);    //[cjf***03272012]  
	}
	else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
	{
		m_MonthComboBox.SetCurSel(2015 - m_pPatient[0].m_BirthYear);//[cjf***03272012]
		m_DaysComboBox.SetCurSel(m_pPatient[0].m_BirthMonth - 1);//[cjf***03272012]  
		m_YearComboBox.SetCurSel(m_pPatient[0].m_BirthDay - 1);    //[cjf***03272012]  
	}

	switch (m_pPatient[0].m_Sex) {
	case SEX_MALE:
		m_MaleCheck.SetCheck(1);
		break;
	case SEX_FEMALE:
		m_FemaleCheck.SetCheck(1);
		break;
	}

	//Add some values to the comboBox
	CString Note = "";
	BOOL KeepSearchID = TRUE;
	BOOL KeepSearchGroup = TRUE;
	BOOL KeepSearchAdLine1 = TRUE;
	BOOL KeepSearchAdLine2 = TRUE;

	BOOL KeepSearchCity = TRUE;
	BOOL KeepSearchZip = TRUE;
	BOOL KeepSearchCountry = TRUE;
	BOOL KeepSearchHomePhone = TRUE;
	BOOL KeepSearchWorkPhone = TRUE;
	BOOL KeepSearchEMail = TRUE;

	for (int i = 0; i < m_nPatients; i++)
	{
		if (Note.Find(m_pPatient[i].m_Note, 0) == -1)
		{
			Note += m_pPatient[i].m_Note;
			Note += "\r\n";
		}

		if (m_pPatient[i].m_GroupID != GUID_NULL && KeepSearchGroup)
		{
			CreateGroupsList(m_pPatient[i].m_GroupID);
			KeepSearchGroup = FALSE;
		}

		if (m_pPatient[i].m_CustomID != "" && KeepSearchID)
		{
			CreateIDList(m_pPatient[i].m_CustomID);
			KeepSearchID = FALSE;
		}

		if (m_pPatient[i].m_Address.m_Line1 != "" && KeepSearchAdLine1)
		{
			CreateAdLine1List(m_pPatient[i].m_Address.m_Line1);
			KeepSearchAdLine1 = FALSE;
		}

		if (m_pPatient[i].m_Address.m_Line2 != "" && KeepSearchAdLine2)
		{
			CreateAdLine2List(m_pPatient[i].m_Address.m_Line2);
			KeepSearchAdLine2 = FALSE;
		}

		if (m_pPatient[i].m_Address.m_City != "" && KeepSearchCity)
		{
			CreateCityList(m_pPatient[i].m_Address.m_City);
			KeepSearchCity = FALSE;
		}

		if (m_pPatient[i].m_Address.m_ZIP != "" && KeepSearchZip)
		{
			CreateZipList(m_pPatient[i].m_Address.m_ZIP);
			KeepSearchZip = FALSE;
		}

		if (m_pPatient[i].m_Address.m_Country != "" && KeepSearchCountry)
		{
			CreateCountryList(m_pPatient[i].m_Address.m_Country);
			KeepSearchCountry = FALSE;
		}

		if (m_pPatient[i].m_HomePhone != "" && KeepSearchHomePhone)
		{
			CreateHomePhoneList(m_pPatient[i].m_HomePhone);
			KeepSearchHomePhone = FALSE;
		}

		if (m_pPatient[i].m_WorkPhone != "" && KeepSearchWorkPhone)
		{
			CreateWorkPhoneList(m_pPatient[i].m_WorkPhone);
			KeepSearchWorkPhone = FALSE;
		}

		if (m_pPatient[i].m_EMail != "" && KeepSearchEMail)
		{
			CreateEmailList(m_pPatient[i].m_EMail);
			KeepSearchEMail = FALSE;
		}
	}

	if (KeepSearchGroup) CreateGroupsList(m_pPatient[0].m_GroupID);

	m_NoteEdit.SetWindowText(Note);
	//Done

	if (::DB.IsReadOnly()) {
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDC_NEW_GROUP)->EnableWindow(FALSE);
	}
	else {
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDC_NEW_GROUP)->EnableWindow(TRUE);
	}

	return TRUE;
}

//***************************************************************************************

void CPatientsMergeDlg::OnNewGroupButtonClicked()
{
	CGroup NewGroup;
	CGroupDlg* pDlg = new CGroupDlg(this, &NewGroup);
	if (pDlg->DoModal() == IDOK) {
		::DB.SaveGroup(NewGroup);
		CreateGroupsList(NewGroup.m_GroupID);
	}
	delete pDlg;
}

//***************************************************************************************

void CPatientsMergeDlg::OnODButtonClicked()
{
	CEyeDlg* pDlg = new CEyeDlg(this, m_pOD);
	if (pDlg->DoModal() == IDOK) {
		m_SaveOD = TRUE;
	}
	delete pDlg;
}

//***************************************************************************************

void CPatientsMergeDlg::OnOSButtonClicked()
{
	CEyeDlg* pDlg = new CEyeDlg(this, m_pOS);
	if (pDlg->DoModal() == IDOK) {
		m_SaveOS = TRUE;
	}
	delete pDlg;
}

//***************************************************************************************

void CPatientsMergeDlg::OnOK()
{
	CString CustomID;
	m_CustomIdComboBox.GetWindowText(CustomID);
	if (CustomID.FindOneOf(_T("\\/:*?\"<>|,")) != -1) {
		::Error("An ID can't contain any of the following characters: \\/:*?\"<>|,");
		m_CustomIdComboBox.SetFocus();
		return;
	}

	CString LastName;
	m_LastNameComboBox.GetWindowText(LastName);
	if (LastName.IsEmpty()) {
		::Error("Please enter last name.");
		m_LastNameComboBox.SetFocus();
		return;
	}
	if (LastName.FindOneOf(_T("\\/:*?\"<>|,")) != -1) {
		::Error("Last name can't contain any of the following characters: \\/:*?\"<>|,");
		m_LastNameComboBox.SetFocus();
		return;
	}

	CString FirstName;
	m_FirstNameComboBox.GetWindowText(FirstName);
	if (FirstName.IsEmpty()) {
		::Error("Please enter first name.");
		m_FirstNameComboBox.SetFocus();
		return;
	}
	if (FirstName.FindOneOf(_T("\\/:*?\"<>|,")) != -1)
	{
		::Error("First name can't contain any of the following characters: \\/:*?\"<>|,");
		m_FirstNameComboBox.SetFocus();
		return;
	}

	CString MiddleName;
	m_MiddleComboBox.GetWindowText(MiddleName);
	if (MiddleName.FindOneOf(_T("\\/:*?\"<>|,")) != -1) {
		::Error("Middle name can't contain any of the following characters: \\/:*?\"<>|,");
		m_MiddleComboBox.SetFocus();
		return;
	}

	CString s;
	CTime tm;
	tm = tm.GetCurrentTime();
	int tmInt_Year = tm.GetYear();

	int BirthYear;

	if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
		BirthYear = m_YearComboBox.GetCurSel();
	else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
		BirthYear = m_YearComboBox.GetCurSel();
	else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
		BirthYear = m_MonthComboBox.GetCurSel();

	if (BirthYear == -1)
	{

		if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
			m_YearComboBox.GetWindowText(s);
		else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
			m_YearComboBox.GetWindowText(s);
		else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
			m_MonthComboBox.GetWindowText(s);


		if (s.GetLength()>4)
		{
			s.Format(_T("%i"), tm.GetYear());
			s += ").";
			::Error("Please enter correct year of birth (1900-" + s);

			if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
				m_YearComboBox.SetFocus();
			else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
				m_YearComboBox.SetFocus();
			else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
				m_MonthComboBox.SetFocus();

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
				m_YearComboBox.SetFocus();
			else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
				m_YearComboBox.SetFocus();
			else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
				m_MonthComboBox.SetFocus();

			return;
		}
	}
	else
	{
		if (BirthYear < 0 || BirthYear > 115) {
			::Error("Please enter year of birth.");

			if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
				m_YearComboBox.SetFocus();
			else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
				m_YearComboBox.SetFocus();
			else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
				m_MonthComboBox.SetFocus();

			return;
		}
		BirthYear = 2015 - BirthYear;
	}

	int BirthMonth;

	if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
		BirthMonth = m_MonthComboBox.GetCurSel();
	else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
		BirthMonth = m_DaysComboBox.GetCurSel();
	else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
		BirthMonth = m_DaysComboBox.GetCurSel();

	if (BirthMonth == -1)
	{
		if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
			m_MonthComboBox.GetWindowText(s);
		else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
			m_DaysComboBox.GetWindowText(s);
		else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
			m_DaysComboBox.GetWindowText(s);

		if (s.GetLength()>3)
		{
			::Error("Please enter correct month of birth (1-12 or Jan-Dec).");

			if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
				m_MonthComboBox.SetFocus();
			else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
				m_DaysComboBox.SetFocus();
			else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
				m_DaysComboBox.SetFocus();

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
				m_MonthComboBox.SetFocus();
			else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
				m_DaysComboBox.SetFocus();
			else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
				m_DaysComboBox.SetFocus();

			return;
		}
	}
	else
	{
		if (BirthMonth < 0 || BirthMonth > 11) {
			::Error("Please enter month of birth.");

			if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
				m_MonthComboBox.SetFocus();
			else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
				m_DaysComboBox.SetFocus();
			else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
				m_DaysComboBox.SetFocus();

			return;
		}
		BirthMonth++;
	}

	int BirthDay;

	if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
		BirthDay = m_DaysComboBox.GetCurSel();
	else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
		BirthDay = m_MonthComboBox.GetCurSel();
	else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
		BirthDay = m_YearComboBox.GetCurSel();

	if (BirthDay == -1)
	{
		if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
			m_DaysComboBox.GetWindowText(s);
		else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
			m_MonthComboBox.GetWindowText(s);
		else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
			m_YearComboBox.GetWindowText(s);

		if (s.GetLength()>2)
		{
			::Error("Please enter correct day of birth (1-31).");

			if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
				m_DaysComboBox.SetFocus();
			else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
				m_MonthComboBox.SetFocus();
			else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
				m_YearComboBox.SetFocus();

			return;
		}

		G_As = s;
		BirthDay = atoi(G_As);
		if (BirthDay > 31 || BirthDay < 1)
		{
			::Error("Please enter correct day of birth (1-31).");

			if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
				m_DaysComboBox.SetFocus();
			else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
				m_MonthComboBox.SetFocus();
			else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
				m_YearComboBox.SetFocus();

			return;
		}
	}
	else
	{
		if (BirthDay<0 || BirthDay > 31) {
			::Error("Please enter day of birth.");

			if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
				m_DaysComboBox.SetFocus();
			else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
				m_MonthComboBox.SetFocus();
			else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
				m_YearComboBox.SetFocus();

			return;
		}
		BirthDay++;
	}

	int DayMax[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (!((BirthYear % 4 == 0) && BirthMonth == 2 && BirthDay == 29)) {
		if (BirthDay < 1 || BirthDay > DayMax[BirthMonth - 1]) {
			::Error("Please enter correct day of birth.");

			if (::Settings.m_DateFormat == DATE_FORMAT_MM_DD_YYYY)
				m_DaysComboBox.SetFocus();
			else if (::Settings.m_DateFormat == DATE_FORMAT_DD_MM_YYYY)
				m_MonthComboBox.SetFocus();
			else if (::Settings.m_DateFormat == DATE_FORMAT_YYYY_MM_DD)
				m_YearComboBox.SetFocus();

			return;
		}
	}

	//[cjf***032712]
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

	m_resPatient->m_CustomID = CustomID;
	m_resPatient->m_LastName = LastName;
	m_resPatient->m_FirstName = FirstName;
	m_resPatient->m_MiddleName = MiddleName;
	m_resPatient->m_BirthYear = BirthYear;
	m_resPatient->m_BirthMonth = BirthMonth;
	m_resPatient->m_BirthDay = BirthDay;
	m_resPatient->m_Sex = Sex;

	//Patient ID, registration date and time 
	::CoCreateGuid(&m_resPatient->m_PatientID);
	SYSTEMTIME Time;
	::GetLocalTime(&Time);
	m_resPatient->m_RegYear = Time.wYear;
	m_resPatient->m_RegMonth = Time.wMonth;
	m_resPatient->m_RegDay = Time.wDay;
	m_resPatient->m_RegHour = Time.wHour;
	m_resPatient->m_RegMinute = Time.wMinute;
	//Patient ID, registration date & time Done

	int g = m_GroupsComboBox.GetCurSel();
	if (g < 1 || g > ::DB.m_Groups.GetSize()) m_pPatient[0].m_GroupID = GUID_NULL;
	else m_pPatient[0].m_GroupID = ::DB.m_Groups[g - 1].m_GroupID;
	m_NoteEdit.GetWindowText(m_pPatient[0].m_Note);

	m_Line1ComboBox.GetWindowText(m_resPatient->m_Address.m_Line1);
	m_Line2ComboBox.GetWindowText(m_resPatient->m_Address.m_Line2);
	m_CityComboBox.GetWindowText(m_resPatient->m_Address.m_City);
	m_StateComboBox.GetWindowText(m_resPatient->m_Address.m_State);
	m_ZipComboBox.GetWindowText(m_resPatient->m_Address.m_ZIP);
	m_CountryComboBox.GetWindowText(m_resPatient->m_Address.m_Country);
	m_HomePhoneComboBox.GetWindowText(m_resPatient->m_HomePhone);
	m_WorkPhoneComboBox.GetWindowText(m_resPatient->m_WorkPhone);
	m_EMailComboBox.GetWindowText(m_resPatient->m_EMail);

	CDialog::OnOK();
}

//***************************************************************************************

void CPatientsMergeDlg::CreateGroupsList(const GUID& GroupID)
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

void CPatientsMergeDlg::CreateIDList(const CString& GroupID)
{
	m_CustomIdComboBox.ResetContent();
	m_CustomIdComboBox.AddString(_T(""));
	BOOL KeepSearchID = TRUE;
	int k = 0;
	int t = 0;
	for (int i = 0; i < m_nPatients; i++) {
		if (m_pPatient[i].m_CustomID != "" && m_CustomIdComboBox.FindString(0, m_pPatient[i].m_CustomID) == -1)
		{
			m_CustomIdComboBox.AddString(m_pPatient[i].m_CustomID);
			t++;
		}
		if (m_pPatient[i].m_CustomID == GroupID && KeepSearchID)
		{
			k = t;
			KeepSearchID = FALSE;
		}
	}
	m_CustomIdComboBox.SetCurSel(k);
}

//***************************************************************************************

void CPatientsMergeDlg::CreateAdLine1List(const CString& AdLine1)
{
	m_Line1ComboBox.ResetContent();
	m_Line1ComboBox.AddString(_T(""));
	BOOL KeepSearch = TRUE;
	int k = 0;
	int t = 0;
	for (int i = 0; i < m_nPatients; i++) {
		if (m_pPatient[i].m_Address.m_Line1 != "" && m_Line1ComboBox.FindString(0, m_pPatient[i].m_Address.m_Line1) == -1)
		{
			m_Line1ComboBox.AddString(m_pPatient[i].m_Address.m_Line1);
			t++;
		}
		if (m_pPatient[i].m_Address.m_Line1 == AdLine1 && KeepSearch)
		{
			k = t;
			KeepSearch = FALSE;
		}
	}
	m_Line1ComboBox.SetCurSel(k);
}

//***************************************************************************************

void CPatientsMergeDlg::CreateAdLine2List(const CString& AdLine2)
{
	m_Line2ComboBox.ResetContent();
	m_Line2ComboBox.AddString(_T(""));
	BOOL KeepSearch = TRUE;
	int k = 0;
	int t = 0;
	for (int i = 0; i < m_nPatients; i++) {
		if (m_pPatient[i].m_Address.m_Line2 != "" && m_Line2ComboBox.FindString(0, m_pPatient[i].m_Address.m_Line2) == -1)
		{
			m_Line2ComboBox.AddString(m_pPatient[i].m_Address.m_Line2);
			t++;
		}
		if (m_pPatient[i].m_Address.m_Line2 == AdLine2 && KeepSearch)
		{
			k = t;
			KeepSearch = FALSE;
		}
	}
	m_Line2ComboBox.SetCurSel(k);
}

//***************************************************************************************

void CPatientsMergeDlg::CreateCityList(const CString& City)
{
	m_CityComboBox.ResetContent();
	m_CityComboBox.AddString(_T(""));
	BOOL KeepSearch = TRUE;
	int k = 0;
	int t = 0;
	for (int i = 0; i < m_nPatients; i++) {
		if (m_pPatient[i].m_Address.m_City != "" && m_CityComboBox.FindString(0, m_pPatient[i].m_Address.m_City) == -1)
		{
			m_CityComboBox.AddString(m_pPatient[i].m_Address.m_City);
			t++;
		}
		if (m_pPatient[i].m_Address.m_City == City && KeepSearch)
		{
			k = t;
			KeepSearch = FALSE;
		}
	}
	m_CityComboBox.SetCurSel(k);
}

//***************************************************************************************

void CPatientsMergeDlg::CreateZipList(const CString& Zip)
{
	m_ZipComboBox.ResetContent();
	m_ZipComboBox.AddString(_T(""));
	BOOL KeepSearch = TRUE;
	int k = 0;
	int t = 0;
	for (int i = 0; i < m_nPatients; i++) {
		if (m_pPatient[i].m_Address.m_ZIP != "" && m_ZipComboBox.FindString(0, m_pPatient[i].m_Address.m_ZIP) == -1)
		{
			m_ZipComboBox.AddString(m_pPatient[i].m_Address.m_ZIP);
			t++;
		}
		if (m_pPatient[i].m_Address.m_ZIP == Zip && KeepSearch)
		{
			k = t;
			KeepSearch = FALSE;
		}
	}
	m_ZipComboBox.SetCurSel(k);
}

//***************************************************************************************

void CPatientsMergeDlg::CreateCountryList(const CString& Country)
{
	m_CountryComboBox.ResetContent();
	m_CountryComboBox.AddString(_T(""));
	BOOL KeepSearch = TRUE;
	int k = 0;
	int t = 0;
	for (int i = 0; i < m_nPatients; i++) {
		if (m_pPatient[i].m_Address.m_Country != "" && m_CountryComboBox.FindString(0, m_pPatient[i].m_Address.m_Country) == -1)
		{
			m_CountryComboBox.AddString(m_pPatient[i].m_Address.m_Country);
			t++;
		}
		if (m_pPatient[i].m_Address.m_Country == Country && KeepSearch)
		{
			k = t;
			KeepSearch = FALSE;
		}
	}
	m_CountryComboBox.SetCurSel(k);
}

//***************************************************************************************

void CPatientsMergeDlg::CreateHomePhoneList(const CString& HomePhone)
{
	m_HomePhoneComboBox.ResetContent();
	m_HomePhoneComboBox.AddString(_T(""));
	BOOL KeepSearch = TRUE;
	int k = 0;
	int t = 0;
	for (int i = 0; i < m_nPatients; i++) {
		if (m_pPatient[i].m_HomePhone != "" && m_HomePhoneComboBox.FindString(0, m_pPatient[i].m_HomePhone) == -1)
		{
			m_HomePhoneComboBox.AddString(m_pPatient[i].m_HomePhone);
			t++;
		}
		if (m_pPatient[i].m_HomePhone == HomePhone && KeepSearch)
		{
			k = t;
			KeepSearch = FALSE;
		}
	}
	m_HomePhoneComboBox.SetCurSel(k);
}

//***************************************************************************************

void CPatientsMergeDlg::CreateWorkPhoneList(const CString& WorkPhone)
{
	m_WorkPhoneComboBox.ResetContent();
	m_WorkPhoneComboBox.AddString(_T(""));
	BOOL KeepSearch = TRUE;
	int k = 0;
	int t = 0;
	for (int i = 0; i < m_nPatients; i++) {
		if (m_pPatient[i].m_WorkPhone != "" && m_WorkPhoneComboBox.FindString(0, m_pPatient[i].m_WorkPhone) == -1)
		{
			m_WorkPhoneComboBox.AddString(m_pPatient[i].m_WorkPhone);
			t++;
		}
		if (m_pPatient[i].m_WorkPhone == WorkPhone && KeepSearch)
		{
			k = t;
			KeepSearch = FALSE;
		}
	}
	m_WorkPhoneComboBox.SetCurSel(k);
}

//***************************************************************************************

void CPatientsMergeDlg::CreateEmailList(const CString& Email)
{
	m_EMailComboBox.ResetContent();
	m_EMailComboBox.AddString(_T(""));
	BOOL KeepSearch = TRUE;
	int k = 0;
	int t = 0;
	for (int i = 0; i < m_nPatients; i++) {
		if (m_pPatient[i].m_EMail != "" && m_EMailComboBox.FindString(0, m_pPatient[i].m_EMail) == -1)
		{
			m_EMailComboBox.AddString(m_pPatient[i].m_EMail);
			t++;
		}
		if (m_pPatient[i].m_EMail == Email && KeepSearch)
		{
			k = t;
			KeepSearch = FALSE;
		}
	}
	m_EMailComboBox.SetCurSel(k);
}

//***************************************************************************************

void CPatientsMergeDlg::OnMaleCheckClicked()
{
	if (m_MaleCheck.GetCheck()) m_FemaleCheck.SetCheck(0);
}

//***************************************************************************************

void CPatientsMergeDlg::OnFemaleCheckClicked()
{
	if (m_FemaleCheck.GetCheck()) m_MaleCheck.SetCheck(0);
}

//***************************************************************************************
