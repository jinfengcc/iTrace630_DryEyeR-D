//***************************************************************************************

#pragma once

//***************************************************************************************

//class CRegKey1
//{
//public:
//
//  CRegKey1(HKEY ParentKey, const char* KeyName);
//  ~CRegKey1();
//
//  BOOL GetIntValue(const char* ValueName, int& Value);
//  BOOL SetIntValue(const char* ValueName, const int Value);
//  BOOL GetStrValue(const char* ValueName, CString& Value);
//  BOOL SetStrValue(const char* ValueName, const CString& Value);
//
//  BOOL GetBinValue(const char* ValueName, ulong BufferLength, uchar* Buffer); 
//
//private:
//
//  HKEY m_Key;
//};


class CTraceyRegKey
{
public:

	CTraceyRegKey(HKEY ParentKey, CString KeyName);
	~CTraceyRegKey();

	BOOL GetIntValue(CString ValueName, int& Value);
	BOOL GetStrValue(CString ValueName, CString& Value);

private:

	HKEY m_Key;
};

//***************************************************************************************

extern const char* MAIN_REG_KEY;
extern const char* MAIN_REG_KEY2;

//***************************************************************************************
