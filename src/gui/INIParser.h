//How about checking connected exist tracey machine, if it has, it is combo, or it is viewer

//***************************************************************************************

#pragma once

//***************************************************************************************

#define MAX_LENGTH 100

#include <windows.h>

//***************************************************************************************

class CINIParser
{
public:
	CINIParser() { ; }
	~CINIParser() { ; }

	int GetIntKey(CString Section, CString key, CString Path);
	CString GetStrKey(CString Section, CString key, CString Path);

	void SetIntKey(CString Section, CString key, int value, CString Path);
	void SetStrKey(CString Section, CString key, CString value, CString Path);

private:

};

//***************************************************************************************
