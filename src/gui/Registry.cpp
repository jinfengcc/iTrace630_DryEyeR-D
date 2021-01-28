//***************************************************************************************

#include "StdAfx.h"
#include "Registry.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

const char* MAIN_REG_KEY = "Software\\Tracey Technologies\\iTrace50";
//const char* MAIN_REG_KEY2 = "SOFTWARE\\Classes\\VirtualStore\\MACHINE\\SOFTWARE\\Wow6432Node\\Tracey Technologies\\iTrace50";//


//************************** key only can read ******************************************

CTraceyRegKey::CTraceyRegKey(HKEY ParentKey, CString KeyName)
{
	ulong disp;
	if (::RegCreateKeyEx(ParentKey, KeyName, 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_READ, NULL, &m_Key, &disp) != ERROR_SUCCESS)
	{
		m_Key = 0;
	}
}

//***************************************************************************************

CTraceyRegKey::~CTraceyRegKey()
{
	if (m_Key != 0)
	{
		::RegCloseKey(m_Key);
	}
}


//***************************************************************************************

BOOL CTraceyRegKey::GetIntValue(CString ValueName, int& Value)
{
	if (m_Key == 0) return FALSE;

	ulong len = 4;

	return (::RegQueryValueEx(m_Key, ValueName, 0, NULL, (uchar*)&Value, &len) == ERROR_SUCCESS);
}

//***************************************************************************************

BOOL CTraceyRegKey::GetStrValue(CString ValueName, CString& Value)
{
	if (m_Key == 0) return FALSE;

	wchar_t buf[256];
	ulong len = 255;
	memset(buf, 0, sizeof(buf));

	if (::RegQueryValueEx(m_Key, (LPCTSTR)ValueName, 0, NULL, (LPBYTE)buf, &len) == ERROR_SUCCESS)
	{
		Value = buf;
		return TRUE;
	}

	return FALSE;
}

//***************************************************************************************
