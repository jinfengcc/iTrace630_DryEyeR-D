//***************************************************************************************

#include "StdAfx.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

void Warning(const CString& s)
{
	::MessageBox(
		NULL,
		s,
		_T("Warning"),
		MB_TOPMOST | MB_TASKMODAL | MB_ICONWARNING | MB_OK);
}

//***************************************************************************************

void Error(const CString& s)
{
	::MessageBox(
		NULL,
		s,
		_T("Error"),
		MB_TOPMOST | MB_TASKMODAL | MB_ICONERROR | MB_OK);
}

//***************************************************************************************

void Info(const CString& s)
{
	::MessageBox(
		NULL,
		s,
		_T("Information"),
		MB_TOPMOST | MB_TASKMODAL | MB_ICONINFORMATION | MB_OK);
}

//***************************************************************************************

void Info(const int v)
{
	CString s;
	s.Format(_T("%i"), v);
	::Info(s);
}

//***************************************************************************************

BOOL Question(const CString& s, int type, int button)
{
	uint t = MB_TOPMOST | MB_TASKMODAL | MB_YESNO | (type == 0 ? MB_ICONWARNING : MB_ICONINFORMATION) | (button == 0 ? MB_DEFBUTTON1 : MB_DEFBUTTON2);
	CString caption = type == 0 ? "Warning" : "Information";
	return ::MessageBox(NULL, s, caption, t) == IDYES;
}

//***************************************************************************************
