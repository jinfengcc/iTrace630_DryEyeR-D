/*The basic settings
1.
*/


#include "StdAfx.h"

#include "INIParser.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

int CINIParser::GetIntKey(CString Section, CString key, CString Path)
{
	int SValue = GetPrivateProfileInt(Section, key, -1000, Path);

	return SValue;
}

//***************************************************************************************

CString CINIParser::GetStrKey(CString Section, CString key, CString Path)
{
	CString SValue;
	GetPrivateProfileString(Section, key, _T(""), SValue.GetBuffer(MAX_LENGTH), MAX_LENGTH, Path);

	SValue.ReleaseBuffer();
	return SValue;
}

//***************************************************************************************

void CINIParser::SetStrKey(CString Section, CString key, CString value, CString Path)
{
	WritePrivateProfileString(Section, key, value, Path);
}

//***************************************************************************************

void CINIParser::SetIntKey(CString Section, CString key, int value, CString Path)
{
	CString s;
	s.Format(_T("%i"), value);
	WritePrivateProfileString(Section, key, s, Path);
}

//***************************************************************************************