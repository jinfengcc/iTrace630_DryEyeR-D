//***************************************************************************************

#include "StdAfx.h"

#if defined _M_IX86
#pragma comment(                                                                                                                           \
  linker,                                                                                                                                  \
  "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.1.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(                                                                                                                           \
  linker,                                                                                                                                  \
  "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.1.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(                                                                                                                           \
  linker,                                                                                                                                  \
  "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.1.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif


//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

const   uint SOFTWARE_VERSION_3 = 0x03000000;
const   uint SOFTWARE_VERSION_4 = 0x04000000;
// <summary>
// The software version
// </summary>
const   uint SOFTWARE_VERSION = 0x06214116;// V V V V Y M D D

real_t    COS[360];
real_t    SIN[360];
int     MutiCOS[800][360];
int     MutiSIN[800][360];

int     OD_AverageExam;// the number of OD exams for this averaged Exam(multiple selection)
GUID    OD_SelectExamsID[1024];// if it is averaged exam, maintain the GUID of OD exams in here; 1024 is NUM_EXAMS_MAX
int     OD_AverageExam2;//For compare display
GUID    OD_SelectExamsID2[1024];//For compare display

int     OS_AverageExam;//  the number of OS exams for this averaged Exam(multiple selection)
GUID    OS_SelectExamsID[1024];// if it is averaged exam, maintain the GUID of OS exams in here; 1024 is NUM_EXAMS_MAX
int     OS_AverageExam2;//For compare display
GUID    OS_SelectExamsID2[1024];//For compare display

int     PresbiaMode;//Presbia 1:Normal; 2: Presbia64; 3:Presbia256
int     PresbiaModeOrder;//Presbia Order  12 : Normal - Presbia64; 13: Normal - Presbia256 etc.

BOOL    Normal64Mode;//
BOOL    WFScanModeChanged;//

BOOL    m_isCombo;
BOOL    m_isHoya;
BOOL    m_isSolo;
BOOL    m_isDemo;
BOOL    m_isViewer;
BOOL    m_isValid;
BOOL    m_isPresbia;
BOOL    m_isComboViewer;
BOOL    m_isComboDicom;
CString m_LicenseName;

CString MAIN_DIR;

CStringA G_As;//6.2.0 For change to unicode

CString IniPath = "Tracey620sys.ini";//6.2.0 ini registration
CString IniSection = "Tracey 6.2.0";//6.2.0 ini registration

CString WndClassName;

//***************************************************************************************

CString FullSoftwareVersion(const uint SoftwareVersion)
{
	uint v1 = (SoftwareVersion & 0x0f000000) >> 24;
	uint v2 = (SoftwareVersion & 0x00f00000) >> 20;
	uint v3 = (SoftwareVersion & 0x000f0000) >> 16;
	uint y = 2015 + ((SoftwareVersion & 0x0000f000) >> 12);//
	uint m = (SoftwareVersion & 0x00000f00) >> 8;
	uint d = SoftwareVersion & 0x000000ff;
	CString s;
	s.Format(_T("%u.%u.%u %04u-%02u-%02u"), v1, v2, v3, y, m, d);
	return s;
}

//***************************************************************************************

// check the last input letter is number(0~9 and .) or not
BOOL InputIsIpADD(CString inputStr)
{
	BOOL res = TRUE;

	int len = inputStr.GetLength();

	CString inputChar = inputStr.Right(1);

	if (inputChar != "0" && inputChar != "1" && inputChar != "2"
		&& inputChar != "3" && inputChar != "4" && inputChar != "5"
		&& inputChar != "6" && inputChar != "7" && inputChar != "8"
		&& inputChar != "9" && inputChar != ".")
	{
		res = FALSE;
	}

	return res;
}

//***************************************************************************************

// check the last input letter is number(0~9 and .) or not
BOOL InputIsRealNum(CString inputStr)
{
	BOOL res = TRUE;

	int len = inputStr.GetLength();

	CString inputChar = inputStr.Right(1);

	if (inputChar != "0" && inputChar != "1" && inputChar != "2"
		&& inputChar != "3" && inputChar != "4" && inputChar != "5"
		&& inputChar != "6" && inputChar != "7" && inputChar != "8"
		&& inputChar != "9" && inputChar != ".")
	{
		if (len == 1)
		{
			if (inputChar != "-") res = FALSE;
		}
		else 	res = FALSE;
	}
	else
	{
		if (inputChar == ".")
		{
			char *pValue = new char[len];
			CStringA Astr(inputStr);
			strncpy(pValue, Astr, len);

			for (int i = 0; i < len - 1; i++)
			{
				char c = *(pValue + i);

				if (c == '.')
				{
					res = FALSE;
					break;
				}
			}
			delete pValue;
		}
	}

	return res;
}


//***************************************************************************************

// check the last input letter is number(0~9 and .) or not
BOOL InputIsIntNum(CString inputStr)
{
	BOOL res = TRUE;

	int len = inputStr.GetLength();

	CString inputChar = inputStr.Right(1);

	if (inputChar != "0" && inputChar != "1" && inputChar != "2"
		&& inputChar != "3" && inputChar != "4" && inputChar != "5"
		&& inputChar != "6" && inputChar != "7" && inputChar != "8"
		&& inputChar != "9")
	{
		res = FALSE;
	}

	return res;
}

//***************************************************************************************

// check the str is real number or not
BOOL NotRealNum(CString inputStr)
{
	BOOL res = FALSE;

	int len = inputStr.GetLength();

	char *pValue = new char[len];
	CStringA Astr(inputStr);
	strncpy(pValue, Astr, len);

	int t = 0;
	for (int i = 0; i < len; i++)
	{
		char c = *(pValue + i);

		if (c == '.')
		{
			t++;
		}

		if (i == 0)
		{
			if ((c != '.' && c != '0' && c != '1' && c != '2'
				&& c != '3' && c != '4' && c != '5' && c != '6'
				&& c != '7' && c != '8' && c != '9' && c != '-') || t > 1)
			{
				res = TRUE;
				break;
			}
		}
		else
		{
			if ((c != '.' && c != '0' && c != '1' && c != '2'
				&& c != '3' && c != '4' && c != '5' && c != '6'
				&& c != '7' && c != '8' && c != '9') || t > 1)
			{
				res = TRUE;
				break;
			}
		}


	}
	delete pValue;

	return res;
}

//***************************************************************************************

// check the str is Integer number or not
BOOL NotIntNum(CString inputStr)
{
	BOOL res = FALSE;

	int len = inputStr.GetLength();

	char *pValue = new char[len];
	CStringA Astr(inputStr);
	strncpy(pValue, Astr, len);

	for (int i = 0; i < len; i++)
	{
		char c = *(pValue + i);

		if (i == 0)
		{
			if (c != '0' && c != '1' && c != '2'
				&& c != '3' && c != '4' && c != '5' && c != '6'
				&& c != '7' && c != '8' && c != '9' && c != '-')
			{
				res = TRUE;
				break;
			}
		}
		else
		{
			if (c != '0' && c != '1' && c != '2'
				&& c != '3' && c != '4' && c != '5' && c != '6'
				&& c != '7' && c != '8' && c != '9')
			{
				res = TRUE;
				break;
			}
		}
	}
	delete pValue;

	return res;
}

//***************************************************************************************

//for DLI Branch
int GetDegree(real_t x, real_t y)
{
	real_t ang = atan(y / x);

	if (x < 0.0)
	{
		ang = ang + _Pi;
	}
	else
	{
		if (y < 0.0) ang = ang + _2_Pi;
	}

	return intRound(_180_Pi * ang);
}
// DLI Branch Done

//***************************************************************************************

//folders copy
void  CopyFolder(CString  OriFolder, CString  DestFolder)
{
	OriFolder += '\0';
	DestFolder += '\0';

	SHFILEOPSTRUCT fop;
	ZeroMemory(&fop, sizeof fop);
	fop.wFunc = FO_COPY;
	fop.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR;
	fop.pFrom = OriFolder;
	fop.pTo = DestFolder;
	SHFileOperation(&fop);
}

//***************************************************************************************

//finding a DstStr form SearchStr
BOOL  FindSubString(CString DstStr, CString SearchStr, int &pos)
{
	if (DstStr == SearchStr)
	{
		pos = 0;
		return TRUE;
	}

	int DstStrLen = DstStr.GetLength();
	int SearchStrLen = SearchStr.GetLength();

	if (DstStrLen > SearchStrLen) return FALSE;

	char *pValue = new char[SearchStrLen];
	CStringA Astr(SearchStr);
	strncpy(pValue, Astr, SearchStrLen);

	for (int i = 0; i<SearchStrLen - DstStrLen + 1; i++)
	{
		CString compareStr = "";
		for (int j = 0; j < DstStrLen; j++)
		{
			char c = *(pValue + i + j);
			compareStr = compareStr + c;
		}
		if (compareStr == DstStr)
		{
			pos = i;
			return TRUE;
		}
	}

	return FALSE;
}

//***************************************************************************************
