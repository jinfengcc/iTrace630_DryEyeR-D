//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "Compression.h"
#include "GlobalFunctions.h"//6.2.0 Chinese

#include "Settings.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

void CScale::LoadColors(const CString& FileName)
{
	G_As = FileName;
	FILE* pFile = fopen(G_As, "rt");
	if (pFile == NULL) return;

	int n = 0;
	fscanf(pFile, "%i", &n);
	if (n < 11 || n > 21) return;

	m_NumColors = n;

	for (int i = 0; i < m_NumColors; i++)
	{
		fscanf(pFile, "%x", &m_Colors[i]);
	}

	fclose(pFile);
}

//***************************************************************************************

void CScale::SaveColors(const CString& FileName)
{
	G_As = FileName;
	FILE* pFile = fopen(G_As, "wt");
	if (pFile == NULL) return;

	fprintf(pFile, "%i\n", m_NumColors);

	for (int i = 0; i < m_NumColors; i++)
	{
		fprintf(pFile, "%06x\n", m_Colors[i]);
	}

	fclose(pFile);
}

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

real_t CWndSettings::GetIncrement()
{
	switch (m_Type)
	{
	case TYPE_TOPM: return 0.01;
	case TYPE_TWFM: return 0.1;
	case TYPE_TRFM: return 0.25;
	case TYPE_CAXM:
	case TYPE_CTNM:
	case TYPE_CRFM: return m_MapUnit == DIOPTERS ? 0.25 : 0.01;
	case TYPE_CELM: return 0.10;
	case TYPE_CWFM: return 0.10;
	case TYPE_IWFM: return 0.10;
	case TYPE_IRFM: return 0.25;
	}

	return 0.0;
}

//***************************************************************************************

real_t CWndSettings::GetMaxStep()
{
	switch (m_Type)
	{
	case TYPE_TOPM: return  1.00;
	case TYPE_TWFM: return  1.00;
	case TYPE_TRFM: return  2.00;
	case TYPE_CAXM:
	case TYPE_CTNM:
	case TYPE_CRFM: return m_MapUnit == DIOPTERS ? 2.00 : 0.50;
	case TYPE_CELM: return 10.00;
	case TYPE_CWFM: return 10.00;
	case TYPE_IWFM: return 10.00;
	case TYPE_IRFM: return  2.00;
	}

	return 0.0;
}

//***************************************************************************************

void CWndSettings::SetDefaultScale()
{
	for (int i = 0; i < 8; i++)
	{
		m_Scale[i].m_CentAuto = TRUE;
		m_Scale[i].m_NumColors = 15;
		m_Scale[i].m_Step = 0.5;

		/*m_Scale[i].m_Colors[14] = 0x000000aa;
		m_Scale[i].m_Colors[13] = 0x000000dc;
		m_Scale[i].m_Colors[12] = 0x000064e6;
		m_Scale[i].m_Colors[11] = 0x000096f0;
		m_Scale[i].m_Colors[10] = 0x0000d7ff;
		m_Scale[i].m_Colors[ 9] = 0x0000ffff;
		m_Scale[i].m_Colors[ 8] = 0x0000dcb4;
		m_Scale[i].m_Colors[ 7] = 0x0000be00;
		m_Scale[i].m_Colors[ 6] = 0x006ed200;
		m_Scale[i].m_Colors[ 5] = 0x00dcdc00;
		m_Scale[i].m_Colors[ 4] = 0x00c88000;
		m_Scale[i].m_Colors[ 3] = 0x00c80000;
		m_Scale[i].m_Colors[ 2] = 0x00c8004b;
		m_Scale[i].m_Colors[ 1] = 0x00c80082;
		m_Scale[i].m_Colors[ 0] = 0x007d0052;*/
	}
}

//***************************************************************************************

CScale* CWndSettings::GetScale()
{
	CScale* pScale;

	switch (m_Type)
	{
	case TYPE_TOPM: pScale = &m_Scale[0]; break;
	case TYPE_TWFM: pScale = &m_Scale[1]; break;
	case TYPE_TRFM: pScale = &m_Scale[2]; break;

	case TYPE_CAXM: pScale = m_MapUnit == DIOPTERS ? pScale = &m_Scale[0] : &m_Scale[1]; break;
	case TYPE_CTNM: pScale = m_MapUnit == DIOPTERS ? pScale = &m_Scale[2] : &m_Scale[3]; break;
	case TYPE_CRFM: pScale = &m_Scale[4]; break;
	case TYPE_CELM: pScale = &m_Scale[6]; break;
	case TYPE_CWFM: pScale = &m_Scale[7]; break;

	case TYPE_IWFM: pScale = &m_Scale[0]; break;
	case TYPE_IRFM: pScale = &m_Scale[1]; break;

	default: pScale = &m_Scale[0];
	}

	if (pScale->m_NumColors < 10) pScale->m_NumColors = 10;
	else if (pScale->m_NumColors > 21) pScale->m_NumColors = 21;

	return pScale;
}

//***************************************************************************************

void CWndSettings::SetDefaultColors(int ColorsIndex)
{
	CScale* pScale = GetScale();

	if (m_Type == TYPE_TOPM)
	{
		pScale->m_NumColors = 11;
		real_t dr = 255.0 / (pScale->m_NumColors - 1);
		real_t dg = 255.0 / (pScale->m_NumColors - 1);
		real_t db = 155.0 / (pScale->m_NumColors - 1);

		for (int i = 0; i < pScale->m_NumColors; i++)
		{
			int r = 255 - intRound(i * dr);
			int g = 255 - intRound(i * dg);
			int b = 255 - intRound(i * db);
			pScale->m_Colors[i] = RGB(r, g, b);
		}
	}
	else if (m_Type == TYPE_TRFM || m_Type == TYPE_IRFM ||
		(m_Type == TYPE_CAXM && m_MapUnit == MILLIMETERS) ||
		(m_Type == TYPE_CTNM && m_MapUnit == MILLIMETERS) ||
		(m_Type == TYPE_CRFM && m_MapUnit == MILLIMETERS))
	{
		pScale->m_NumColors = 15;

		if (ColorsIndex == 0)
		{
			pScale->m_Colors[14] = BLUE;
			pScale->m_Colors[13] = 0x00ff6f00;
			pScale->m_Colors[12] = 0x00ff9f00;
			pScale->m_Colors[11] = 0x00ffcf00;
			pScale->m_Colors[10] = CYAN;
			pScale->m_Colors[9] = 0x00afff00;
			pScale->m_Colors[8] = 0x006fff00;
			pScale->m_Colors[7] = GREEN;
			pScale->m_Colors[6] = 0x0000ffaf;
			pScale->m_Colors[5] = 0x0000ffdf;
			pScale->m_Colors[4] = YELLOW;
			pScale->m_Colors[3] = 0x0000cfff;
			pScale->m_Colors[2] = 0x00009fff;
			pScale->m_Colors[1] = 0x00006fff;
			pScale->m_Colors[0] = RED;
		}
		else if (ColorsIndex == 1)
		{
			pScale->m_Colors[14] = 0x007d0052;
			pScale->m_Colors[13] = 0x00c80082;
			pScale->m_Colors[12] = 0x00c8004b;
			pScale->m_Colors[11] = 0x00c80000;
			pScale->m_Colors[10] = 0x00c88000;
			pScale->m_Colors[9] = 0x00dcdc00;
			pScale->m_Colors[8] = 0x006ed200;
			pScale->m_Colors[7] = 0x0000be00;
			pScale->m_Colors[6] = 0x0000dcb4;
			pScale->m_Colors[5] = 0x0000ffff;
			pScale->m_Colors[4] = 0x0000d7ff;
			pScale->m_Colors[3] = 0x000096f0;
			pScale->m_Colors[2] = 0x000064e6;
			pScale->m_Colors[1] = 0x000000dc;
			pScale->m_Colors[0] = 0x000000aa;
		}
	}
	else
	{
		pScale->m_NumColors = 15;
		if (ColorsIndex == 0)
		{
			pScale->m_Colors[14] = RED;
			pScale->m_Colors[13] = 0x00006fff;
			pScale->m_Colors[12] = 0x00009fff;
			pScale->m_Colors[11] = 0x0000cfff;
			pScale->m_Colors[10] = YELLOW;
			pScale->m_Colors[9] = 0x0000ffdf;
			pScale->m_Colors[8] = 0x0000ffaf;
			pScale->m_Colors[7] = GREEN;
			pScale->m_Colors[6] = 0x006fff00;
			pScale->m_Colors[5] = 0x00afff00;
			pScale->m_Colors[4] = CYAN;
			pScale->m_Colors[3] = 0x00ffcf00;
			pScale->m_Colors[2] = 0x00ff9f00;
			pScale->m_Colors[1] = 0x00ff6f00;
			pScale->m_Colors[0] = BLUE;
		}
		else if (ColorsIndex == 1)
		{
			pScale->m_Colors[14] = 0x000000aa;
			pScale->m_Colors[13] = 0x000000dc;
			pScale->m_Colors[12] = 0x000064e6;
			pScale->m_Colors[11] = 0x000096f0;
			pScale->m_Colors[10] = 0x0000d7ff;
			pScale->m_Colors[9] = 0x0000ffff;
			pScale->m_Colors[8] = 0x0000dcb4;
			pScale->m_Colors[7] = 0x0000be00;
			pScale->m_Colors[6] = 0x006ed200;
			pScale->m_Colors[5] = 0x00dcdc00;
			pScale->m_Colors[4] = 0x00c88000;
			pScale->m_Colors[3] = 0x00c80000;
			pScale->m_Colors[2] = 0x00c8004b;
			pScale->m_Colors[1] = 0x00c80082;
			pScale->m_Colors[0] = 0x007d0052;
		}
	}
}

//***************************************************************************************

BOOL CWndSettings::AreDefaultColors(int ColorsIndex)
{
	CScale* pScale = GetScale();
	CScale BackupScale;
	BackupScale = *pScale;
	SetDefaultColors(ColorsIndex);
	BOOL Result = (BackupScale.m_NumColors == pScale->m_NumColors) &&
		(memcmp(BackupScale.m_Colors, pScale->m_Colors, BackupScale.m_NumColors * sizeof(COLORREF)) == 0);
	*pScale = BackupScale;

	return Result;
}

BOOL CWndSettings::AreDefaultColors()
{
	return AreDefaultColors(0) || AreDefaultColors(1);
}

//***************************************************************************************

CSettings::CSettings()
{
	memset(this, 0, sizeof(CSettings));
}

//***************************************************************************************

//6.2.0 Chinese
void CTaskSetting::SaveBtnName(CString BtnName)
{
	BOOL Chinese = FALSE;

	int len = BtnName.GetLength();

	for (int k = 0; k < len; k++)
	{
		TCHAR ch = BtnName.GetAt(k);

		if (ch > 255)
		{
			Chinese = TRUE;
			break;
		}
	}

	if (Chinese)
	{
		CStringA AName, CName;
		AName = "Chinese";
		CName = "";

		CString s;
		CString Str = BtnName;
		size_t len = Str.GetLength();

		int NewLen = len;

		for (int k = 0; k < len; k++)
		{
			CString Namestr = Str.Left(1);

			TCHAR ch = Namestr.GetAt(0);

			if (ch > 255)
			{
				int ChineseID = G_GetChineseId(Namestr);
				s.Format(_T("C%i*"), ChineseID);
				CName += s;
			}
			else
			{
				CName += Namestr;
				CName += "*";
			}

			Str = Str.Right(NewLen - 1);
			NewLen = Str.GetLength();
		}


		strncpy(m_ButtonName, AName, 25);
		strncpy(m_ChButtonName, CName, 200);
	}
	else
	{
		CStringA ATaskName = BtnName;
		strncpy(m_ButtonName, ATaskName, 25);
	}
}
//6.2.0 Chinese

//***************************************************************************************

//6.2.0 Chinese
CString  CTaskSetting::ReadBtnName()
{
	char sa[26];
	memset(sa, 0, sizeof(sa));
	strncpy(sa, (const char*)m_ButtonName, 25);

	CString Name = sa;

	//6.2.0 Chinese
	int count = Name.Find(_T("Chinese"), 0);
	if (count != -1) //chinese
	{
		char s2[201];
		memset(s2, 0, sizeof(s2));
		strncpy(s2, (const char*)m_ChButtonName, 200);
		Name = s2;

		CString Str = Name;

		Name = "";
		{
			int Len = Str.GetLength();
			int NewLen = Len;
			CStringA StrId = "";

			for (int k = 0; k < Len; k++)
			{
				CString Namestr = Str.Left(1);

				if (Namestr == "*")
				{
					BOOL Chinese = FALSE;

					TCHAR ch = StrId.GetAt(0);
					int Len = StrId.GetLength();

					if (Len > 1)
					{
						Chinese = TRUE;
						StrId.Replace("C", "");
					}

					if (Chinese)
					{
						int id = atoi(StrId);
						Name += G_GetChinese(id);
					}
					else
					{
						Name += StrId;
					}
					StrId = "";
				}
				else
				{
					StrId += Namestr;
				}

				Str = Str.Right(NewLen - 1);
				NewLen = Str.GetLength();
			}
		}
	}

	return Name;
}
//6.2.0 For Chinese

//***************************************************************************************

//6.2.0 Chinese
void CNewSettings::SaveTaskName(int i, CString& Name)
{
	BOOL Chinese = FALSE;

	int len = Name.GetLength();

	for (int k = 0; k < len; k++)
	{
		TCHAR ch = Name.GetAt(k);

		if (ch > 255)
		{
			Chinese = TRUE;
			break;
		}
	}

	if (Chinese) //chinese中文
	{
		CStringA AName, CName;
		AName = "Chinese";
		CName = "";

		CString s;
		CString Str = Name;
		size_t len = Str.GetLength();

		int NewLen = len;

		for (int k = 0; k < len; k++)
		{
			CString Namestr = Str.Left(1);

			TCHAR ch = Namestr.GetAt(0);

			if (ch > 255)
			{
				int ChineseID = G_GetChineseId(Namestr);
				s.Format(_T("C%i*"), ChineseID);
				CName += s;
			}
			else
			{
				CName += Namestr;
				CName += "*";
			}

			Str = Str.Right(NewLen - 1);
			NewLen = Str.GetLength();
		}


		strncpy(&m_Tasks[i].m_ButtonName[0], AName, 25);
		strncpy(&m_Tasks[i].m_ChButtonName[0], CName, 200);
	}
	else
	{
		CStringA ADef_Name(Name);
		strncpy(&m_Tasks[i].m_ButtonName[0], ADef_Name, 25);
	}
}
//6.2.0 Chinese

//***************************************************************************************

//6.2.0 Chinese
CString	 CNewSettings::ReadTaskName(int i)
{
	char sa[26];
	memset(sa, 0, sizeof(sa));
	strncpy(sa, (const char*)m_Tasks[i].m_ButtonName, 25);

	CString Name = sa;

	//6.2.0 Chinese
	int count = Name.Find(_T("Chinese"), 0);
	if (count != -1) //chinese
	{
		char s2[201];
		memset(s2, 0, sizeof(s2));
		strncpy(s2, (const char*)m_Tasks[i].m_ChButtonName, 200);//cjf
		Name = s2;

		CString Str = Name;

		Name = "";
		{
			int Len = Str.GetLength();
			int NewLen = Len;
			CStringA StrId = "";

			for (int k = 0; k < Len; k++)
			{
				CString Namestr = Str.Left(1);

				if (Namestr == "*")
				{
					BOOL Chinese = FALSE;

					TCHAR ch = StrId.GetAt(0);
					int Len = StrId.GetLength();

					if (Len > 1)
					{
						Chinese = TRUE;
						StrId.Replace("C", "");
					}

					if (Chinese)
					{
						int id = atoi(StrId);
						Name += G_GetChinese(id);
					}
					else
					{
						Name += StrId;
					}
					StrId = "";
				}
				else
				{
					StrId += Namestr;
				}

				Str = Str.Right(NewLen - 1);
				NewLen = Str.GetLength();
			}
		}
	}

	return Name;
}
//6.2.0 Chinese

//***************************************************************************************

CNewSettings::CNewSettings()
{
	memset(this, 0, sizeof(CNewSettings));
}

//***************************************************************************************

BOOL CNewSettings::LoadFromFile(const CString& FileName)
{
	BOOL Res = FALSE;
	HANDLE hFile = ::CreateFile(FileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (hFile != INVALID_HANDLE_VALUE) {

		int ZipSize = ::GetFileSize(hFile, NULL);
		void* pZipDataNew = malloc(ZipSize);

		ulong bt;
		::ReadFile(hFile, pZipDataNew, ZipSize, &bt, NULL);

		int aSize = ::GetDecompressedSize(pZipDataNew, ZipSize);
		int bSize = sizeof(CNewSettings);
		if (::GetDecompressedSize(pZipDataNew, ZipSize) == sizeof(CNewSettings)) {
			if (::Decompress(pZipDataNew, ZipSize, this)) {
				Res = TRUE;
			}
		}

		free(pZipDataNew);

		::CloseHandle(hFile);

	}

	return Res;
}

//***************************************************************************************

void CNewSettings::SaveIntoFile(const CString& FileName)
{
	HANDLE hFile = ::CreateFile(FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);

	if (hFile != INVALID_HANDLE_VALUE) {

		void* pZipDataNew = malloc(sizeof(CNewSettings));
		int ZipSize;

		if (::Compress(this, sizeof(CNewSettings), pZipDataNew, &ZipSize)) {
			ulong bt;
			::WriteFile(hFile, pZipDataNew, ZipSize, &bt, NULL);
		}

		free(pZipDataNew);

		::CloseHandle(hFile);
	}
}

//***************************************************************************************

C531NewSettings::C531NewSettings()
{
	memset(this, 0, sizeof(C531NewSettings));
}

//***************************************************************************************

BOOL C531NewSettings::LoadFromFile(const CString& FileName)
{
	BOOL Res = FALSE;

	HANDLE hFile = ::CreateFile(FileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (hFile != INVALID_HANDLE_VALUE) {

		int ZipSize = ::GetFileSize(hFile, NULL);
		void* pZipDataNew = malloc(ZipSize);

		ulong bt;
		::ReadFile(hFile, pZipDataNew, ZipSize, &bt, NULL);

		int aSize = ::GetDecompressedSize(pZipDataNew, ZipSize);
		int bSize = sizeof(C531NewSettings);
		if (::GetDecompressedSize(pZipDataNew, ZipSize) == sizeof(C531NewSettings)) {
			if (::Decompress(pZipDataNew, ZipSize, this)) {
				Res = TRUE;
			}
		}

		free(pZipDataNew);

		::CloseHandle(hFile);

	}

	return Res;
}

//***************************************************************************************

void C531NewSettings::SaveIntoFile(const CString& FileName)
{
	HANDLE hFile = ::CreateFile(FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);

	if (hFile != INVALID_HANDLE_VALUE) {

		void* pZipDataNew = malloc(sizeof(C531NewSettings));
		int ZipSize;

		if (::Compress(this, sizeof(C531NewSettings), pZipDataNew, &ZipSize)) {
			ulong bt;
			::WriteFile(hFile, pZipDataNew, ZipSize, &bt, NULL);
		}

		free(pZipDataNew);

		::CloseHandle(hFile);
	}
}

//*************************************************************************************************

C601NewSettings::C601NewSettings()
{
	memset(this, 0, sizeof(C601NewSettings));
}

//*************************************************************************************************

BOOL C601NewSettings::LoadFromFile(const CString& FileName)
{
	BOOL Res = FALSE;

	HANDLE hFile = ::CreateFile(FileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		int ZipSize = ::GetFileSize(hFile, NULL);
		void* pZipDataNew = malloc(ZipSize);

		ulong bt;
		::ReadFile(hFile, pZipDataNew, ZipSize, &bt, NULL);

		int aSize = ::GetDecompressedSize(pZipDataNew, ZipSize);
		int bSize = sizeof(C601NewSettings);
		if (::GetDecompressedSize(pZipDataNew, ZipSize) == sizeof(C601NewSettings))
		{
			if (::Decompress(pZipDataNew, ZipSize, this))
			{
				Res = TRUE;
			}
		}

		free(pZipDataNew);

		::CloseHandle(hFile);

	}

	return Res;
}

//*************************************************************************************************

void C601NewSettings::SaveIntoFile(const CString& FileName)
{
	HANDLE hFile = ::CreateFile(FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{

		void* pZipDataNew = malloc(sizeof(C601NewSettings));
		int ZipSize;

		if (::Compress(this, sizeof(C601NewSettings), pZipDataNew, &ZipSize)) {
			ulong bt;
			::WriteFile(hFile, pZipDataNew, ZipSize, &bt, NULL);
		}

		free(pZipDataNew);

		::CloseHandle(hFile);
	}
}

//*************************************************************************************************

CString CNewSettings::GetTaskBtnName(int i, int j)
{
	static CString DefaultNames[20] =
	{
		""                      , "User defined", "OU Overview"  , "Refraction Analysis", "Aberration Analysis",
		"M T F"   , "CT Summary"  , "VFA"          , "Keratometry"        , "Chang Analysis",
		"IOL Selection Analysis", "Angle K/A"   , "Toric Planner", "WF Compare",
		"M T F"      , "WF/RMS"      , "CT OD/OS"     ,  "CT 3D Z-Elevation",
		"WF/CT Summary"         , "CT Compare"  ,
	};

	int type = m_Tasks[i].m_SubButton[j];

	if (type <= 19) return DefaultNames[type];
	else //User Defination
	{
		if (type == 28) return "Dysfunctional Lens - MD";
		else if (type == 29) return "Astigmatism Source";
		else if (type == TYPE_DYSP) return "Dysfunctional Lens - PT";
		else if (type == TYPE_DEPF) return "Depth of Focus";
		else if (type == TYPE_TOAL) return "Toric Check";
		else if (type == TYPE_NEVI) return "Near Vision"; //6.2.0 NEAR VISION
		else return "No Done Yet";
	}
}

//***************************************************************************************

void CNewSettings::GetNewWFBtnName(int i, int j, CString& Name)
{
	if (strncmp((const char*)m_NewButtonNames[i][j], "OK", 2) == 0)
	{
		char s[31];
		memset(s, 0, sizeof(s));
		strncpy(s, (const char*)m_NewButtonNames[i][j] + 2, 30);
		Name = s;

		//6.2.0 Chinese
		int count = Name.Find(_T("Chinese"), 0);
		if (count != -1) //chinese
		{
			char s2[201];
			memset(s2, 0, sizeof(s2));
			strncpy(s2, (const char*)m_NewChButtonNames[i][j], 200);//cjf
			Name = s2;

			CString Str = Name;

			Name = "";

			int Len = Str.GetLength();
			int NewLen = Len;
			CStringA StrId = "";

			for (int k = 0; k < Len; k++)
			{
				CString Namestr = Str.Left(1);

				if (Namestr == "*")
				{
					BOOL Chinese = FALSE;

					TCHAR ch = StrId.GetAt(0);
					int Len = StrId.GetLength();

					if (Len > 1)
					{
						Chinese = TRUE;
						StrId.Replace("C", "");
					}

					if (Chinese)
					{
						int id = atoi(StrId);
						Name += G_GetChinese(id);
					}
					else
					{
						Name += StrId;
					}
					StrId = "";
				}
				else
				{
					StrId += Namestr;
				}

				Str = Str.Right(NewLen - 1);
				NewLen = Str.GetLength();
			}

		}
		//6.2.0 Chinese		
	}
	else
	{
		static CString DefaultNames[3][7] = {
			{ "Depth of Focus Comparison", "", "", "", "", "", "" },//[cjf***06282012]
			{ "", "", "", "", "", "", "" },
			{ "", "", "", "", "", "", "" }
		};

		Name = DefaultNames[i][j];
	}
}

//***************************************************************************************

void CNewSettings::SetNewWFBtnName(int i, int j, const CString& Name)
{
	//6.2.0 Chinese
	BOOL Chinese = FALSE;
	int len = Name.GetLength();

	for (int k = 0; k < len; k++)
	{
		TCHAR ch = Name.GetAt(k);

		if (ch > 255)
		{
			Chinese = TRUE;
			break;
		}
	}

	if (Chinese) //chinese
	{
		CStringA AName, CName;
		AName = "Chinese";
		CName = "";

		CString s;
		CString Str = Name;
		size_t len = Str.GetLength();

		int NewLen = len;

		for (int k = 0; k < len; k++)
		{
			CString Namestr = Str.Left(1);

			TCHAR ch = Namestr.GetAt(0);

			if (ch > 255)
			{
				int ChineseID = G_GetChineseId(Namestr);
				s.Format(_T("C%i*"), ChineseID);
				CName += s;
			}
			else
			{
				CName += Namestr;
				CName += "*";
			}

			Str = Str.Right(NewLen - 1);
			NewLen = Str.GetLength();
		}


		m_NewButtonNames[i][j][0] = 'O';
		m_NewButtonNames[i][j][1] = 'K';

		strncpy(&m_NewButtonNames[i][j][2], AName, 18);
		strncpy(&m_NewChButtonNames[i][j][0], CName, 200);
	}//6.2.0 Chinese
	else
	{
		m_NewButtonNames[i][j][0] = 'O';
		m_NewButtonNames[i][j][1] = 'K';
		CStringA AName(Name);
		strncpy(&m_NewButtonNames[i][j][2], AName, 30);
	}
}

//***************************************************************************************

BOOL CSettings::LoadFromFile(const CString& FileName)
{
	BOOL Res = FALSE;

	HANDLE hFile = ::CreateFile(FileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (hFile != INVALID_HANDLE_VALUE) {

		int ZipSize = ::GetFileSize(hFile, NULL);
		void* pZipData = malloc(ZipSize);

		ulong bt;
		::ReadFile(hFile, pZipData, ZipSize, &bt, NULL);

		int size = sizeof(CSettings);

		//if (::GetDecompressedSize(pZipData, ZipSize) == sizeof(CSettings)) 
		{
			if (::Decompress(pZipData, ZipSize, this)) {
				Check();
				Res = TRUE;
			}
		}

		free(pZipData);

		::CloseHandle(hFile);

	}

	return Res;
}

//***************************************************************************************

void CSettings::Check()
{
}

//***************************************************************************************

void CSettings::SaveIntoFile(const CString& FileName)
{
	HANDLE hFile = ::CreateFile(FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{

		void* pZipData = malloc(sizeof(CSettings));
		int ZipSize;

		if (::Compress(this, sizeof(CSettings), pZipData, &ZipSize))
		{
			ulong bt;
			::WriteFile(hFile, pZipData, ZipSize, &bt, NULL);
		}

		free(pZipData);

		::CloseHandle(hFile);
	}
}

//***************************************************************************************

void CSettings::LoadFromResource()
{
	static int ResSize = 0;
	static void* pResData = NULL;

	if (pResData == NULL)
	{
		HRSRC hResInfo = ::FindResource(NULL, MAKEINTRESOURCE(IDR_SETTINGS), _T("Settings"));
		if (hResInfo)
		{
			HGLOBAL hRes = ::LoadResource(NULL, hResInfo);
			if (hRes != NULL)
			{
				ResSize = ::SizeofResource(NULL, hResInfo);
				pResData = ::LockResource(hRes);
			}
		}
	}

	if (pResData != NULL)
	{
		if (::Decompress(pResData, ResSize, this))
		{
			Check();
		}
	}

	::Settings.m_AutoScanDiameterUm = 6500;

	::Settings.m_AccommTargetLightTime = 60;

	::Settings.m_ShowSimK = FALSE;// Set the CT Panel(etc. in OU overview) to show the simk(not ref.k)

	::Settings.m_AlignmentMethod = ALIGNMENT_4_IR_DOTS;//

	::Settings.SetButtonName(1, 0, "CT Summary");
	::Settings.SetButtonName(1, 2, "3D Z-Elevation");//CT 3D Z-Elevation
	::Settings.SetButtonName(1, 4, "Osher Iris Imaging");//CT Osher alignment
	::Settings.SetButtonName(1, 3, "Custom");//CT Custom
	::Settings.SetButtonName(2, 0, "Chang Analysis");//Chang Analysis
	::Settings.SetButtonName(2, 1, "Summary");//WFCT Summary
	::Settings.SetButtonName(2, 2, "M T F");//WFCT Visual Acuity
	::Settings.SetButtonName(2, 7, "Angle K/A");//WFCT Angle K/A
	::Settings.SetButtonName(2, 4, "Astigmatism Source");//Astigmatism source

														 // IOL Selection data set
	::NewSettings.m_IOLSel_Alpha[0] = 0.65;
	::NewSettings.m_IOLSel_Alpha[1] = 0.5;

	::NewSettings.m_IOLSel_CHOA = 0.2;
	::NewSettings.m_IOLSel_IHOA = 0.2;

	::NewSettings.m_IOLSel_Astig = 1.0;

	::NewSettings.m_IOLSel_Sph_Abbe[0] = 0.14;
	::NewSettings.m_IOLSel_Sph_Abbe[1] = 0.20;
	::NewSettings.m_IOLSel_Sph_Abbe[2] = 0.25;
	::NewSettings.m_IOLSel_Sph_Abbe[3] = 0.30;

	// The default simplified RMS GUI is TRUE
	for (int i = 0; i < 4; i++)
	{
		if (i == 0)
		{
			::Settings.m_TScmSettings.m_SimplifiedRMS = TRUE;
			::Settings.m_TScmSettings.m_SimplifiedWFPanel = TRUE;
			::Settings.m_TScmSettings.m_SimplifiedCTPanel = TRUE;

			::Settings.m_TMsmSettings.m_SimplifiedRMS = TRUE;
			::Settings.m_TMsmSettings.m_SimplifiedWFPanel = TRUE;
			::Settings.m_TMsmSettings.m_SimplifiedCTPanel = TRUE;

			::Settings.m_CSvrSettings.m_SimplifiedRMS = TRUE;
			::Settings.m_CSvrSettings.m_SimplifiedWFPanel = TRUE;
			::Settings.m_CSvrSettings.m_SimplifiedCTPanel = TRUE;

			::Settings.m_CScmSettings.m_SimplifiedRMS = TRUE;
			::Settings.m_CScmSettings.m_SimplifiedWFPanel = TRUE;
			::Settings.m_CScmSettings.m_SimplifiedCTPanel = TRUE;

			::Settings.m_PlannerSettings.m_SimplifiedRMS = TRUE;
			::Settings.m_PlannerSettings.m_SimplifiedWFPanel = TRUE;
			::Settings.m_PlannerSettings.m_SimplifiedCTPanel = TRUE;
		}

		if (i < 2)
		{
			::Settings.m_CDsmSettings[i].m_SimplifiedRMS = TRUE;
			::Settings.m_CDsmSettings[i].m_SimplifiedWFPanel = TRUE;
			::Settings.m_CDsmSettings[i].m_SimplifiedCTPanel = TRUE;
		}

		if (i < 3)
		{
			::Settings.m_WTsmSettings[i].m_SimplifiedRMS = TRUE;
			::Settings.m_WTsmSettings[i].m_SimplifiedWFPanel = TRUE;
			::Settings.m_WTsmSettings[i].m_SimplifiedCTPanel = TRUE;
		}

		::Settings.m_TSvrSettings[i].m_SimplifiedRMS = TRUE;
		::Settings.m_TSvrSettings[i].m_SimplifiedWFPanel = TRUE;
		::Settings.m_TSvrSettings[i].m_SimplifiedCTPanel = TRUE;

		::Settings.m_TDsmSettings[i].m_SimplifiedRMS = TRUE;
		::Settings.m_TDsmSettings[i].m_SimplifiedWFPanel = TRUE;
		::Settings.m_TDsmSettings[i].m_SimplifiedCTPanel = TRUE;
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			::Settings.m_TSsmSettings[i][j].m_SimplifiedRMS = TRUE;
			::Settings.m_TSsmSettings[i][j].m_SimplifiedWFPanel = TRUE;
			::Settings.m_TSsmSettings[i][j].m_SimplifiedCTPanel = TRUE;

			::Settings.m_CSsmSettings[i][j].m_SimplifiedRMS = TRUE;
			::Settings.m_CSsmSettings[i][j].m_SimplifiedWFPanel = TRUE;
			::Settings.m_CSsmSettings[i][j].m_SimplifiedCTPanel = TRUE;

			::Settings.m_IDsmSettings[i][j].m_SimplifiedRMS = TRUE;
			::Settings.m_IDsmSettings[i][j].m_SimplifiedWFPanel = TRUE;
			::Settings.m_IDsmSettings[i][j].m_SimplifiedCTPanel = TRUE;
		}
	}
	//The default simplified RMS GUI is TRUE Done


	//Following Set  display default settings  
	::Settings.m_CSsmSettings[1][0].m_Type = 51;

	//WF Point display
	::Settings.m_TSvrSettings[0].m_MapShowCornealVertex = 1;
	::Settings.m_TSvrSettings[0].m_MapShowPoints = 1;
	::Settings.m_TSvrSettings[0].m_MapShowPupil = 1;
	::Settings.m_TSvrSettings[0].m_MapShowLimbus = 1;
	::Settings.m_TSvrSettings[0].m_SizeSmall = TRUE;

	//WF Vfa display
	::Settings.m_TDsmSettings[0].m_Type = 11;
	::Settings.m_TDsmSettings[3].m_Type = 11;

	for (int i = 0; i < 6; i++)
	{
		::Settings.m_TDsmSettings[i].m_Mask.SetType(MASK_HO_TOTAL);
		::Settings.m_TDsmSettings[i].m_Piston = FALSE;
		::Settings.m_TDsmSettings[i].m_Tilt = FALSE;

		if (i != 0 && i != 3)
		{
			::Settings.m_TDsmSettings[i].m_Ltr = 0;
			::Settings.m_TDsmSettings[i].m_LtrLine = 70;
			::Settings.m_TDsmSettings[i].m_LtrOrientation = 0;
		}

		if (i == 0 || i == 3)
		{
			CScale* pScale = ::Settings.m_TDsmSettings[i].GetScale();
			pScale->m_Step = 0.5;

			::Settings.m_TDsmSettings[i].SetDefaultColors(1);
		}
	}

	// WF Compare Display
	::Settings.m_TScmSettings.m_Type = 11;

	::Settings.m_TScmSettings.m_Method3D = 0;
	::Settings.m_TScmSettings.m_MapShowSolidSurface = TRUE;
	::Settings.m_TScmSettings.m_MapShowWireMesh = TRUE;

	::Settings.m_TScmSettings.m_Mask.SetType(MASK_TOTAL);
	::Settings.m_TScmSettings.m_Piston = FALSE;
	::Settings.m_TScmSettings.m_Tilt = FALSE;

	::Settings.m_TScmSettings.m_MapShowMap = 1;
	::Settings.m_TScmSettings.m_MapShowNumbers = 0;
	::Settings.m_TScmSettings.m_MapShowEye = 0;
	::Settings.m_TScmSettings.m_MapShowPupil = 0;
	::Settings.m_TScmSettings.m_MapShowLimbus = 0;
	::Settings.m_TScmSettings.m_MapShowCornealVertex = 0;
	::Settings.m_TScmSettings.m_MapTranslucent = 0;

	::Settings.m_TScmSettings.m_ZoneRadiusUm = 5000;

	::Settings.m_TScmSettings.m_Ltr = 0;
	::Settings.m_TScmSettings.m_LtrLine = 70;
	::Settings.m_TScmSettings.m_LtrOrientation = 0;

	CScale* pScale = ::Settings.m_TScmSettings.GetScale();
	pScale->m_Step = 0.5;
	::Settings.m_TScmSettings.SetDefaultColors(1);

	//WF visual acuity display	
	::Settings.m_TSsmSettings[0][0].m_Mask.SetType(MASK_TOTAL);
	::Settings.m_TSsmSettings[0][0].m_Piston = TRUE;
	::Settings.m_TSsmSettings[0][0].m_Tilt = TRUE;

	::Settings.m_TSsmSettings[0][2].m_Mask.SetType(MASK_TOTAL);
	::Settings.m_TSsmSettings[0][2].m_Piston = TRUE;
	::Settings.m_TSsmSettings[0][2].m_Tilt = TRUE;

	::Settings.m_TSsmSettings[0][1].m_Mask.SetType(MASK_HO_TOTAL);
	::Settings.m_TSsmSettings[0][1].m_Piston = FALSE;
	::Settings.m_TSsmSettings[0][1].m_Tilt = FALSE;

	::Settings.m_TSsmSettings[0][3].m_Mask.SetType(MASK_HO_TOTAL);
	::Settings.m_TSsmSettings[0][3].m_Piston = FALSE;
	::Settings.m_TSsmSettings[0][3].m_Tilt = FALSE;

	for (int i = 0; i< 2; i++)
	{
		::Settings.m_TSsmSettings[0][i].m_Type = 11;

		::Settings.m_TSsmSettings[0][i].m_MapShowMap = 1;
		::Settings.m_TSsmSettings[0][i].m_MapShowEye = 0;
		::Settings.m_TSsmSettings[0][i].m_MapShowNumbers = 0;
		::Settings.m_TSsmSettings[0][i].m_MapShowPupil = 0;
		::Settings.m_TSsmSettings[0][i].m_MapShowLimbus = 0;
		::Settings.m_TSsmSettings[0][i].m_MapShowCornealVertex = 0;
		::Settings.m_TSsmSettings[0][i].m_MapTranslucent = 0;

		::Settings.m_TSsmSettings[0][i].m_Method3D = 0;
		::Settings.m_TSsmSettings[0][i].m_MapShowSolidSurface = TRUE;
		::Settings.m_TSsmSettings[0][i].m_MapShowWireMesh = TRUE;
		::Settings.m_TSsmSettings[0][i].m_ZoneRadiusUm = 5000;
		::Settings.m_TSsmSettings[0][i].m_SizeSmall = TRUE;

		CScale* pScale = ::Settings.m_TSsmSettings[0][i].GetScale();
		pScale->m_Step = 0.5;
		::Settings.m_TSsmSettings[0][i].SetDefaultColors(1);
	}

	for (int i = 2; i< 4; i++)
	{
		::Settings.m_TSsmSettings[0][i].m_Type = 14;
		::Settings.m_TSsmSettings[0][i].m_Ltr = 0;
		::Settings.m_TSsmSettings[0][i].m_LtrLine = 70;
		::Settings.m_TSsmSettings[0][i].m_LtrOrientation = 0;
		::Settings.m_TSsmSettings[0][i].m_Method3D = 0;
		::Settings.m_TSsmSettings[0][i].m_MapShowSolidSurface = TRUE;
		::Settings.m_TSsmSettings[0][i].m_MapShowWireMesh = TRUE;
		::Settings.m_TSsmSettings[0][i].m_ZoneRadiusUm = 5000;
		::Settings.m_TSsmSettings[0][i].m_SizeSmall = TRUE;
	}

	//WF/RMS
	::Settings.m_TSsmSettings[1][0].m_Mask.SetType(MASK_TOTAL);
	::Settings.m_TSsmSettings[1][0].m_Piston = TRUE;
	::Settings.m_TSsmSettings[1][0].m_Tilt = TRUE;

	::Settings.m_TSsmSettings[1][1].m_Mask.SetType(MASK_HO_TOTAL);
	::Settings.m_TSsmSettings[1][1].m_Piston = FALSE;
	::Settings.m_TSsmSettings[1][1].m_Tilt = FALSE;

	::Settings.m_TSsmSettings[1][2].m_Mask.SetType(MASK_LO_TOTAL);
	::Settings.m_TSsmSettings[1][2].m_Piston = FALSE;
	::Settings.m_TSsmSettings[1][2].m_Tilt = FALSE;

	::Settings.m_TSsmSettings[1][3].m_Mask.SetType(MASK_TOTAL);
	::Settings.m_TSsmSettings[1][3].m_Piston = TRUE;
	::Settings.m_TSsmSettings[1][3].m_Tilt = TRUE;

	for (int i = 0; i< 3; i++)
	{
		::Settings.m_TSsmSettings[1][i].m_Type = 10;

		::Settings.m_TSsmSettings[1][i].m_Method3D = 0;
		::Settings.m_TSsmSettings[1][i].m_MapShowSolidSurface = TRUE;
		::Settings.m_TSsmSettings[1][i].m_MapShowWireMesh = TRUE;
		::Settings.m_TSsmSettings[1][i].m_ZoneRadiusUm = 5000;

		::Settings.m_TSsmSettings[1][i].m_SizeSmall = TRUE;

		::Settings.m_TSsmSettings[1][i].m_MapShowMap = 1;
		::Settings.m_TSsmSettings[1][i].m_MapShowEye = 0;
		::Settings.m_TSsmSettings[1][i].m_MapShowNumbers = 0;
		::Settings.m_TSsmSettings[1][i].m_MapShowPupil = 0;
		::Settings.m_TSsmSettings[1][i].m_MapShowLimbus = 0;
		::Settings.m_TSsmSettings[1][i].m_MapShowCornealVertex = 0;
		::Settings.m_TSsmSettings[1][i].m_MapTranslucent = 0;

		CScale* pScale = ::Settings.m_TSsmSettings[1][i].GetScale();
		pScale->m_Step = 0.5;
		::Settings.m_TSsmSettings[1][i].SetDefaultColors(1);
	}

	::Settings.m_TSsmSettings[1][3].m_Type = 16;
	::Settings.m_TSsmSettings[1][3].m_ZoneRadiusUm = 5000;
	::Settings.m_TSsmSettings[1][3].m_RMSIndividual = FALSE;
	::Settings.m_TSsmSettings[1][3].m_SizeSmall = TRUE;

	//WF Angle K/A Display
	//Problem the showruler and show inlay can not be set here
	//Can not be remmber in the display too
	::Settings.m_TSsmSettings[2][0].m_Type = 1;
	::Settings.m_TSsmSettings[2][0].m_MapShowPupil = 1;
	::Settings.m_TSsmSettings[2][0].m_MapShowLimbus = 1;
	::Settings.m_TSsmSettings[2][0].m_MapShowCornealVertex = 1;
	::Settings.m_TSsmSettings[2][0].m_SizeSmall = FALSE;

	//WF Custom
	::Settings.m_TSsmSettings[3][0].m_Mask.SetType(MASK_TOTAL);
	::Settings.m_TSsmSettings[3][0].m_Piston = TRUE;
	::Settings.m_TSsmSettings[3][0].m_Tilt = TRUE;

	::Settings.m_TSsmSettings[3][1].m_Mask.SetType(MASK_HO_TOTAL);
	::Settings.m_TSsmSettings[3][1].m_Piston = FALSE;
	::Settings.m_TSsmSettings[3][1].m_Tilt = FALSE;

	::Settings.m_TSsmSettings[3][2].m_Mask.SetType(MASK_TOTAL);
	::Settings.m_TSsmSettings[3][2].m_Piston = TRUE;
	::Settings.m_TSsmSettings[3][2].m_Tilt = TRUE;

	::Settings.m_TSsmSettings[3][3].m_Mask.SetType(MASK_HO_TOTAL);
	::Settings.m_TSsmSettings[3][3].m_Piston = FALSE;
	::Settings.m_TSsmSettings[3][3].m_Tilt = FALSE;

	for (int i = 0; i < 4; i++)
	{
		::Settings.m_TSsmSettings[3][i].m_ZoneRadiusUm = 5000;
		::Settings.m_TSsmSettings[3][i].m_Method3D = 0;
		::Settings.m_TSsmSettings[3][i].m_MapShowSolidSurface = TRUE;
		::Settings.m_TSsmSettings[3][i].m_MapShowWireMesh = TRUE;
		::Settings.m_TSsmSettings[3][i].m_SizeSmall = TRUE;

		if (i == 0 || i == 1)
		{
			::Settings.m_TSsmSettings[3][i].m_Type = 15;//TYPE_TMTF
			::Settings.m_TSsmSettings[3][i].m_MTFAverage = 1;
			::Settings.m_TSsmSettings[3][i].m_MTFAngle = 0;
		}

		if (i == 2 || i == 3)
		{
			::Settings.m_TSsmSettings[3][i].m_Type = 12;//TYPE_TPSF
			::Settings.m_TSsmSettings[3][i].m_ShowEEF = 0;
		}
	}

	//WF Toric planner
	::Settings.m_PlannerSettings.m_MapShowEye = 1;

	//CT Rings
	::Settings.m_CSvrSettings.m_MapShowRings = 1;
	::Settings.m_CSvrSettings.m_MapShowPupil = 0;
	::Settings.m_CSvrSettings.m_MapShowLimbus = 0;

	//CT OD/OS	
	::Settings.m_CDsmSettings[0].m_Type = 31;//Axial map       TYPE_CAXM	 
	::Settings.m_CDsmSettings[1].m_Type = 32;//local Roc  map  TYPE_CTNM

	for (int i = 0; i < 2; i++)
	{
		::Settings.m_CDsmSettings[i].m_Method3D = 0;
		::Settings.m_CDsmSettings[i].m_MapShowSolidSurface = TRUE;
		::Settings.m_CDsmSettings[i].m_MapShowWireMesh = TRUE;
		::Settings.m_CDsmSettings[i].m_MapUnit = DIOPTERS;

		::Settings.m_CDsmSettings[i].m_MapShowMap = 1;
		::Settings.m_CDsmSettings[i].m_MapShowEye = 1;
		::Settings.m_CDsmSettings[i].m_MapShowNumbers = 0;
		::Settings.m_CDsmSettings[i].m_MapShowPupil = 0;
		::Settings.m_CDsmSettings[i].m_MapShowLimbus = 0;
		::Settings.m_CDsmSettings[i].m_MapShowCornealVertex = 0;
		::Settings.m_CDsmSettings[i].m_MapTranslucent = 0;

		CScale* pScale = ::Settings.m_CDsmSettings[i].GetScale();
		pScale->m_Step = 0.5;
		::Settings.m_CDsmSettings[i].SetDefaultColors(1);
	}

	//CT Compare
	::Settings.m_CScmSettings.m_Type = 31;//Axial map       TYPE_CAXM
	::Settings.m_CScmSettings.m_Method3D = 0;
	::Settings.m_CScmSettings.m_MapShowSolidSurface = TRUE;
	::Settings.m_CScmSettings.m_MapShowWireMesh = TRUE;
	::Settings.m_CScmSettings.m_MapUnit = DIOPTERS;

	::Settings.m_CScmSettings.m_MapShowMap = 1;
	::Settings.m_CScmSettings.m_MapShowEye = 1;
	::Settings.m_CScmSettings.m_MapShowNumbers = 0;
	::Settings.m_CScmSettings.m_MapShowPupil = 0;
	::Settings.m_CScmSettings.m_MapShowLimbus = 0;
	::Settings.m_CScmSettings.m_MapShowCornealVertex = 0;
	::Settings.m_CScmSettings.m_MapTranslucent = 0;

	CScale* pScale2 = ::Settings.m_CScmSettings.GetScale();
	pScale2->m_Step = 0.5;
	::Settings.m_CScmSettings.SetDefaultColors(1);

	//CT Summary
	::Settings.m_CSsmSettings[0][0].m_Type = 31; //Axial map       TYPE_CAXM
	::Settings.m_CSsmSettings[0][1].m_Type = 34; //z elevation     TYPE_CELM 
	::Settings.m_CSsmSettings[0][2].m_Type = 32; //local Roc  map  TYPE_CTNM
	::Settings.m_CSsmSettings[0][3].m_Type = 33; //refrective map  TYPE_CRFM

	::Settings.m_CSsmSettings[0][0].m_MapUnit = DIOPTERS;
	::Settings.m_CSsmSettings[0][2].m_MapUnit = MILLIMETERS;

	::Settings.m_CSsmSettings[0][1].m_LtrLine = 100;

	::Settings.m_CSsmSettings[0][1].m_RefSurfaceType = REF_SURFACE_BEST_SPHERE;

	for (int i = 0; i < 4; i++)
	{
		::Settings.m_CSsmSettings[0][i].m_Method3D = 0;
		::Settings.m_CSsmSettings[0][i].m_MapShowSolidSurface = TRUE;
		::Settings.m_CSsmSettings[0][i].m_MapShowWireMesh = TRUE;
		::Settings.m_CSsmSettings[0][i].m_SizeSmall = TRUE;

		::Settings.m_CSsmSettings[0][i].m_MapShowMap = 1;
		::Settings.m_CSsmSettings[0][i].m_MapShowEye = 1;
		::Settings.m_CSsmSettings[0][i].m_MapShowNumbers = 0;
		::Settings.m_CSsmSettings[0][i].m_MapShowPupil = 0;
		::Settings.m_CSsmSettings[0][i].m_MapShowLimbus = 0;
		::Settings.m_CSsmSettings[0][i].m_MapShowCornealVertex = 0;
		::Settings.m_CSsmSettings[0][i].m_MapTranslucent = 0;
		::Settings.m_CSsmSettings[0][i].m_MapShowRings = FALSE;

		CScale* pScale = ::Settings.m_CSsmSettings[0][i].GetScale();
		pScale->m_Step = 0.5;
		::Settings.m_CSsmSettings[0][i].SetDefaultColors(1);
	}

	//CT Kerotametry Display
	::Settings.m_CSsmSettings[1][0].m_Type = 51; //Kerotametry map       TYPE_CSKM
	::Settings.m_CSsmSettings[1][0].m_SizeSmall = FALSE;

	//CT 3D Z Elevation Display
	::Settings.m_CSsmSettings[2][0].m_Type = 34; //z elevation     TYPE_CELM 
	::Settings.m_CSsmSettings[2][0].m_SizeSmall = FALSE;
	::Settings.m_CSsmSettings[2][0].m_Method3D = 1;
	::Settings.m_CSsmSettings[2][0].m_MapShowSolidSurface = TRUE;
	::Settings.m_CSsmSettings[2][0].m_MapShowWireMesh = TRUE;
	CScale* pScale3 = ::Settings.m_CSsmSettings[2][0].GetScale();
	pScale3->m_Step = 0.5;
	::Settings.m_CSsmSettings[2][0].SetDefaultColors(1);
	::Settings.m_CSsmSettings[2][0].m_RefSurfaceType = REF_SURFACE_BEST_SPHERE;

	//CT Custom Display
	::Settings.m_CSsmSettings[3][0].m_Type = 31; //Axial map       TYPE_CAXM
	::Settings.m_CSsmSettings[3][1].m_Type = 34; //z elevation     TYPE_CELM 
	::Settings.m_CSsmSettings[3][2].m_Type = 32; //local Roc  map  TYPE_CTNM
	::Settings.m_CSsmSettings[3][3].m_Type = 33; //refrective map  TYPE_CRFM

	::Settings.m_CSsmSettings[3][0].m_MapUnit = DIOPTERS;
	::Settings.m_CSsmSettings[3][2].m_MapUnit = MILLIMETERS;

	::Settings.m_CSsmSettings[3][1].m_RefSurfaceType = REF_SURFACE_BEST_SPHERE;

	for (int i = 0; i < 4; i++)
	{
		::Settings.m_CSsmSettings[3][i].m_Method3D = 0;
		::Settings.m_CSsmSettings[3][i].m_MapShowSolidSurface = TRUE;
		::Settings.m_CSsmSettings[3][i].m_MapShowWireMesh = TRUE;
		::Settings.m_CSsmSettings[3][i].m_SizeSmall = TRUE;

		::Settings.m_CSsmSettings[3][i].m_MapShowMap = 1;
		::Settings.m_CSsmSettings[3][i].m_MapShowEye = 1;
		::Settings.m_CSsmSettings[3][i].m_MapShowNumbers = 0;
		::Settings.m_CSsmSettings[3][i].m_MapShowPupil = 0;
		::Settings.m_CSsmSettings[3][i].m_MapShowLimbus = 0;
		::Settings.m_CSsmSettings[3][i].m_MapShowCornealVertex = 0;
		::Settings.m_CSsmSettings[3][i].m_MapTranslucent = 0;

		CScale* pScale = ::Settings.m_CSsmSettings[3][i].GetScale();
		pScale->m_Step = 0.5;
		::Settings.m_CSsmSettings[3][i].SetDefaultColors(1);
	}

	//WF&CT Change Analysis Display
	::Settings.m_IDsmSettings[0][0].m_Type = 41; //RMS map        TYPE_IRMS
	::Settings.m_IDsmSettings[0][0].m_Method3D = 0;
	::Settings.m_IDsmSettings[0][0].m_MapShowSolidSurface = TRUE;
	::Settings.m_IDsmSettings[0][0].m_MapShowWireMesh = TRUE;
	::Settings.m_IDsmSettings[0][0].m_SizeSmall = TRUE;
	::Settings.m_IDsmSettings[0][0].m_ZoneRadiusUm = 5000;
	::Settings.m_IDsmSettings[0][0].m_Mask.SetType(MASK_TOTAL_NO_DEFOCUS);
	::Settings.m_IDsmSettings[0][0].m_Piston = TRUE;
	::Settings.m_IDsmSettings[0][0].m_Tilt = TRUE;
	::Settings.m_IDsmSettings[0][0].m_RMSIndividual = TRUE;

	::Settings.m_IDsmSettings[0][3].m_Type = 31; //Axial map       TYPE_CAXM
	::Settings.m_IDsmSettings[0][3].m_Method3D = 0;
	::Settings.m_IDsmSettings[0][3].m_MapShowSolidSurface = TRUE;
	::Settings.m_IDsmSettings[0][3].m_MapShowWireMesh = TRUE;
	::Settings.m_IDsmSettings[0][3].m_SizeSmall = TRUE;
	::Settings.m_IDsmSettings[0][3].m_MapUnit = DIOPTERS;
	::Settings.m_IDsmSettings[0][3].m_MapShowMap = 1;
	::Settings.m_IDsmSettings[0][3].m_MapShowEye = 1;
	::Settings.m_IDsmSettings[0][3].m_MapShowNumbers = 0;
	::Settings.m_IDsmSettings[0][3].m_MapShowPupil = 0;
	::Settings.m_IDsmSettings[0][3].m_MapShowLimbus = 0;
	::Settings.m_IDsmSettings[0][3].m_MapShowCornealVertex = 0;
	::Settings.m_IDsmSettings[0][3].m_MapTranslucent = 0;
	::Settings.m_IDsmSettings[0][3].m_MapShowKeratometry = 1;

	CScale* pScale4 = ::Settings.m_IDsmSettings[0][3].GetScale();
	pScale4->m_Step = 0.5;
	::Settings.m_IDsmSettings[0][3].SetDefaultColors(1);

	//WF&CT Summary and Custom Displays
	for (int i = 1; i < 4; i++)
	{
		if (i != 2)
		{
			::Settings.m_IDsmSettings[i][0].m_Type = 40; //Wavefront map     
			::Settings.m_IDsmSettings[i][1].m_Type = 36; //Wavefront map     
			::Settings.m_IDsmSettings[i][2].m_Type = 16; //Wavefront map 

			::Settings.m_IDsmSettings[i][0].m_Method3D = 0;
			::Settings.m_IDsmSettings[i][0].m_MapShowSolidSurface = TRUE;
			::Settings.m_IDsmSettings[i][0].m_MapShowWireMesh = TRUE;
			::Settings.m_IDsmSettings[i][0].m_SizeSmall = TRUE;
			::Settings.m_IDsmSettings[i][0].m_Mask.SetType(MASK_TOTAL_NO_DEFOCUS);
			::Settings.m_IDsmSettings[i][0].m_Piston = TRUE;
			::Settings.m_IDsmSettings[i][0].m_Tilt = TRUE;
			::Settings.m_IDsmSettings[i][0].m_MapShowMap = 1;
			::Settings.m_IDsmSettings[i][0].m_ZoneRadiusUm = 5000;
			::Settings.m_IDsmSettings[i][0].m_MapShowEye = 0;
			::Settings.m_IDsmSettings[i][0].m_MapShowNumbers = 0;
			::Settings.m_IDsmSettings[i][0].m_MapShowPupil = 0;
			::Settings.m_IDsmSettings[i][0].m_MapShowLimbus = 0;
			::Settings.m_IDsmSettings[i][0].m_MapShowCornealVertex = 0;
			::Settings.m_IDsmSettings[i][0].m_MapTranslucent = 0;
			::Settings.m_IDsmSettings[i][0].m_MapShowKeratometry = 0;
			::Settings.m_IDsmSettings[i][0].m_MapShowKeratometry = 0;
			::Settings.m_IDsmSettings[i][0].m_MapShowAstigmatismAxes = 0;
			::Settings.m_IDsmSettings[i][0].SetDefaultColors(1);


			::Settings.m_IDsmSettings[i][3].m_Type = 31; //Axial map       TYPE_CAXM
			::Settings.m_IDsmSettings[i][3].m_Method3D = 0;
			::Settings.m_IDsmSettings[i][3].m_MapShowSolidSurface = TRUE;
			::Settings.m_IDsmSettings[i][3].m_MapShowWireMesh = TRUE;
			::Settings.m_IDsmSettings[i][3].m_SizeSmall = TRUE;
			::Settings.m_IDsmSettings[i][3].m_MapUnit = DIOPTERS;
			::Settings.m_IDsmSettings[i][3].m_MapShowMap = 1;
			::Settings.m_IDsmSettings[i][3].m_MapShowEye = 1;
			::Settings.m_IDsmSettings[i][3].m_MapShowNumbers = 0;
			::Settings.m_IDsmSettings[i][3].m_MapShowPupil = 0;
			::Settings.m_IDsmSettings[i][3].m_MapShowLimbus = 0;
			::Settings.m_IDsmSettings[i][3].m_MapShowCornealVertex = 0;
			::Settings.m_IDsmSettings[i][3].m_MapTranslucent = 0;
			::Settings.m_IDsmSettings[i][3].m_MapShowKeratometry = 1;

			CScale* pScale = ::Settings.m_IDsmSettings[i][3].GetScale();
			pScale->m_Step = 0.5;
			::Settings.m_IDsmSettings[i][3].SetDefaultColors(1);
		}
	}

	//WF&CT Visual acuity Display
	::Settings.m_IDsmSettings[2][0].m_Method3D = FALSE;
	::Settings.m_IDsmSettings[2][0].m_MapShowSolidSurface = TRUE;
	::Settings.m_IDsmSettings[2][0].m_MapShowWireMesh = TRUE;
	::Settings.m_IDsmSettings[2][0].m_SizeSmall = TRUE;
	::Settings.m_IDsmSettings[2][0].m_ZoneRadiusUm = 5000;
	::Settings.m_IDsmSettings[2][0].m_Mask.SetType(MASK_TOTAL_NO_DEFOCUS);
	::Settings.m_IDsmSettings[2][0].m_Piston = TRUE;
	::Settings.m_IDsmSettings[2][0].m_Tilt = TRUE;
	for (int i = 0; i < 3; i++) ::Settings.m_IDsmSettings[2][i].m_Ltr = 0;
	::Settings.m_IDsmSettings[2][0].m_LtrLine = 70;
	::Settings.m_IDsmSettings[2][0].m_LtrOrientation = 0;

	//6.2.0
	::Settings.m_IDsmSettings[2][0].m_Type = TYPE_IMTF;
	::Settings.m_IDsmSettings[2][1].m_Type = TYPE_CMTF;
	::Settings.m_IDsmSettings[2][2].m_Type = TYPE_TMTF;

	::Settings.m_IDsmSettings[2][0].m_Mask.SetType(MASK_HO_TOTAL);
	::Settings.m_IDsmSettings[2][1].m_Mask.SetType(MASK_HO_TOTAL);
	::Settings.m_IDsmSettings[2][2].m_Mask.SetType(MASK_HO_TOTAL);
	//6.2.0

	::Settings.m_IDsmSettings[2][3].m_Type = 31; //Axial map       TYPE_CAXM
	::Settings.m_IDsmSettings[2][3].m_Method3D = 0;
	::Settings.m_IDsmSettings[2][3].m_MapShowSolidSurface = TRUE;
	::Settings.m_IDsmSettings[2][3].m_MapShowWireMesh = TRUE;
	::Settings.m_IDsmSettings[2][3].m_SizeSmall = TRUE;
	::Settings.m_IDsmSettings[2][3].m_MapUnit = DIOPTERS;
	::Settings.m_IDsmSettings[2][3].m_MapShowMap = 1;
	::Settings.m_IDsmSettings[2][3].m_MapShowEye = 0;
	::Settings.m_IDsmSettings[2][3].m_MapShowNumbers = 0;
	::Settings.m_IDsmSettings[2][3].m_MapShowPupil = 0;
	::Settings.m_IDsmSettings[2][3].m_MapShowLimbus = 0;
	::Settings.m_IDsmSettings[2][3].m_MapShowCornealVertex = 0;
	::Settings.m_IDsmSettings[2][3].m_MapTranslucent = 0;
	::Settings.m_IDsmSettings[2][3].m_MapShowKeratometry = 1;

	CScale* pScale5 = ::Settings.m_IDsmSettings[2][3].GetScale();
	pScale5->m_Step = 0.5;
	::Settings.m_IDsmSettings[2][3].SetDefaultColors(1);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			::Settings.m_IDsmSettings[i][j].m_MapShowEye = 0;
		}
	}

	//WF&CT  OU Overview
	::NewSettings.m_OWFCTSettings[0].m_Type = 31;//Axial map       TYPE_CAXM
	::NewSettings.m_OWFCTSettings[0].m_Mask.SetType(MASK_HO_TOTAL);
	::NewSettings.m_OWFCTSettings[0].m_Piston = FALSE;
	::NewSettings.m_OWFCTSettings[0].m_Tilt = FALSE;
	::NewSettings.m_OWFCTSettings[0].m_Method3D = 0;
	::NewSettings.m_OWFCTSettings[0].m_MapShowSolidSurface = TRUE;
	::NewSettings.m_OWFCTSettings[0].m_MapShowWireMesh = TRUE;
	::NewSettings.m_OWFCTSettings[0].m_MapUnit = DIOPTERS;
	::NewSettings.m_OWFCTSettings[0].m_MapShowMap = 1;
	::NewSettings.m_OWFCTSettings[0].m_MapShowEye = 1;
	::NewSettings.m_OWFCTSettings[0].m_MapShowNumbers = 0;
	::NewSettings.m_OWFCTSettings[0].m_MapShowPupil = 0;
	::NewSettings.m_OWFCTSettings[0].m_MapShowLimbus = 0;
	::NewSettings.m_OWFCTSettings[0].m_MapShowCornealVertex = 0;
	::NewSettings.m_OWFCTSettings[0].m_MapTranslucent = 0;
	::NewSettings.m_OWFCTSettings[0].m_MapShowKeratometry = 0;

	CScale* pScale6 = ::NewSettings.m_OWFCTSettings[0].GetScale();
	pScale6->m_Step = 0.5;
	::NewSettings.m_OWFCTSettings[0].SetDefaultColors(1);


	::NewSettings.m_OWFCTSettings[1].m_Type = 11;//Refraction Map  TYPE_TRFM
	::NewSettings.m_OWFCTSettings[1].m_ZoneRadiusUm = 5000;
	::NewSettings.m_OWFCTSettings[1].m_Mask.SetType(MASK_HO_TOTAL);
	::NewSettings.m_OWFCTSettings[1].m_Piston = FALSE;
	::NewSettings.m_OWFCTSettings[1].m_Tilt = FALSE;
	::NewSettings.m_OWFCTSettings[1].SetDefaultColors(1);

	::NewSettings.m_OWFCTSettings[2].m_Ltr = 0;
	::NewSettings.m_OWFCTSettings[2].m_LtrLine = 70;
	::NewSettings.m_OWFCTSettings[2].m_LtrOrientation = 0;
	//Done

	//IOL Selection Overview 530
	::NewSettings.m_IOLSelSettings[0].m_Type = 31;//Axial map       TYPE_CAXM
	::NewSettings.m_IOLSelSettings[0].m_Mask.SetType(MASK_HO_TOTAL);//601
	::NewSettings.m_IOLSelSettings[0].m_Piston = FALSE;
	::NewSettings.m_IOLSelSettings[0].m_Tilt = FALSE;
	::NewSettings.m_IOLSelSettings[0].m_Method3D = 0;
	::NewSettings.m_IOLSelSettings[0].m_MapShowSolidSurface = TRUE;
	::NewSettings.m_IOLSelSettings[0].m_MapShowWireMesh = TRUE;
	::NewSettings.m_IOLSelSettings[0].m_MapUnit = DIOPTERS;
	::NewSettings.m_IOLSelSettings[0].m_MapShowMap = 1;
	::NewSettings.m_IOLSelSettings[0].m_MapShowEye = 1;
	::NewSettings.m_IOLSelSettings[0].m_MapShowNumbers = 0;
	::NewSettings.m_IOLSelSettings[0].m_MapShowPupil = 0;
	::NewSettings.m_IOLSelSettings[0].m_MapShowLimbus = 0;
	::NewSettings.m_IOLSelSettings[0].m_MapShowCornealVertex = 0;
	::NewSettings.m_IOLSelSettings[0].m_MapTranslucent = 0;
	::NewSettings.m_IOLSelSettings[0].m_MapShowKeratometry = 0;

	pScale6 = ::NewSettings.m_IOLSelSettings[0].GetScale();
	pScale6->m_Step = 0.5;
	::NewSettings.m_IOLSelSettings[0].SetDefaultColors(1);


	::NewSettings.m_IOLSelSettings[1].m_Type = 11;//Refraction Map  TYPE_TRFM
	::NewSettings.m_IOLSelSettings[1].m_ZoneRadiusUm = 5000;
	::NewSettings.m_IOLSelSettings[1].m_Mask.SetType(MASK_HO_TOTAL);//601
	::NewSettings.m_IOLSelSettings[1].m_Piston = FALSE;
	::NewSettings.m_IOLSelSettings[1].m_Tilt = FALSE;
	::NewSettings.m_IOLSelSettings[1].SetDefaultColors(1);

	::NewSettings.m_IOLSelSettings[2].m_Ltr = 0;
	::NewSettings.m_IOLSelSettings[2].m_LtrLine = 70;
	::NewSettings.m_IOLSelSettings[2].m_LtrOrientation = 0;

	for (int i = 3; i < 6; i++)
	{
		::NewSettings.m_IOLSelSettings[i].m_Type = 41; //RMS map        TYPE_IRMS
		::NewSettings.m_IOLSelSettings[i].m_Method3D = 0;
		::NewSettings.m_IOLSelSettings[i].m_MapShowSolidSurface = TRUE;
		::NewSettings.m_IOLSelSettings[i].m_MapShowWireMesh = TRUE;
		::NewSettings.m_IOLSelSettings[i].m_SizeSmall = TRUE;
		::NewSettings.m_IOLSelSettings[i].m_ZoneRadiusUm = 5000;
		::NewSettings.m_IOLSelSettings[i].m_Mask.SetType(MASK_HO_TOTAL);//601
		::NewSettings.m_IOLSelSettings[i].m_Piston = TRUE;
		::NewSettings.m_IOLSelSettings[i].m_Tilt = TRUE;
		::NewSettings.m_IOLSelSettings[i].m_RMSIndividual = TRUE;
	}

	//Astiagmatism source  
	::NewSettings.m_InAstigSettings[0] = ::Settings.m_IDsmSettings[2][0];
	::NewSettings.m_InAstigSettings[0] = ::Settings.m_IDsmSettings[2][1];
	::NewSettings.m_InAstigSettings[0] = ::Settings.m_IDsmSettings[2][2];
	::NewSettings.m_InAstigSettings[3] = ::Settings.m_IDsmSettings[2][3];

	for (int i = 0; i < 4; i++)
	{
		::NewSettings.m_InAstigSettings[i].m_SizeSmall = TRUE;
		::NewSettings.m_InAstigSettings[i].m_SimplifiedRMS = TRUE;

		if (i != 3)::NewSettings.m_InAstigSettings[i].m_Mask.SetType(MASK_ASTIGMATISM);
	}
	//Astiagmatism source done

	//Dysfunctional Lens Analysis 531 
	::NewSettings.m_DysfuncSettings[0].m_Type = TYPE_IMTF;
	::NewSettings.m_DysfuncSettings[0].m_MTFAverage = TRUE;
	::NewSettings.m_DysfuncSettings[0].m_SimplifiedWFPanel = TRUE;
	::NewSettings.m_DysfuncSettings[0].m_SimplifiedCTPanel = TRUE;
	::NewSettings.m_DysfuncSettings[1].m_Type = TYPE_ILTR;
	::NewSettings.m_DysfuncSettings[1].m_RMSIndividual = FALSE;
	::NewSettings.m_DysfuncSettings[1].m_SimplifiedRMS = TRUE;
	::NewSettings.m_DysfuncSettings[2].m_Type = TYPE_TOPM;
	::NewSettings.m_DysfuncSettings[3].m_Type = TYPE_CAXM;

	for (int i = 0; i < 4; i++)
	{
		::NewSettings.m_DysfuncSettings[i].m_Method3D = 0;
		::NewSettings.m_DysfuncSettings[i].m_MapShowSolidSurface = TRUE;
		::NewSettings.m_DysfuncSettings[i].m_MapShowWireMesh = TRUE;
		::NewSettings.m_DysfuncSettings[i].m_SizeSmall = TRUE;

		if (i == 0 || i == 1)
		{
			::NewSettings.m_DysfuncSettings[i].m_Mask.SetType(MASK_HO_TOTAL);
		}
		else
		{
			::NewSettings.m_DysfuncSettings[i].m_Mask.SetType(MASK_TOTAL_NO_DEFOCUS);
		}

		::NewSettings.m_DysfuncSettings[i].m_Piston = TRUE;
		::NewSettings.m_DysfuncSettings[i].m_Tilt = TRUE;
		::NewSettings.m_DysfuncSettings[i].m_MapShowMap = 1;
		::NewSettings.m_DysfuncSettings[i].m_ZoneRadiusUm = 5000;
		::NewSettings.m_DysfuncSettings[i].m_MapShowEye = 0;
		::NewSettings.m_DysfuncSettings[i].m_MapShowNumbers = 0;
		::NewSettings.m_DysfuncSettings[i].m_MapShowPupil = 0;
		::NewSettings.m_DysfuncSettings[i].m_MapShowLimbus = 0;
		::NewSettings.m_DysfuncSettings[i].m_MapShowCornealVertex = 0;
		::NewSettings.m_DysfuncSettings[i].m_MapTranslucent = 0;
		::NewSettings.m_DysfuncSettings[i].m_MapShowKeratometry = 0;
		::NewSettings.m_DysfuncSettings[i].m_MapShowAstigmatismAxes = 0;

		::NewSettings.m_DysfuncSettings[i].SetDefaultScale();
		CScale* pScale = ::NewSettings.m_DysfuncSettings[i].GetScale();
		pScale->m_Step = 0.5;
		::NewSettings.m_DysfuncSettings[i].SetDefaultColors(1);

		if (i == 3)
		{
			::NewSettings.m_DysfuncSettings[3].m_Type = TYPE_CTNM;
			CScale* pScale1 = ::NewSettings.m_DysfuncSettings[i].GetScale();
			pScale1->m_Step = 0.5;
			::NewSettings.m_DysfuncSettings[i].SetDefaultColors(1);

			::NewSettings.m_DysfuncSettings[3].m_Type = TYPE_CRFM;
			CScale* pScale2 = ::NewSettings.m_DysfuncSettings[i].GetScale();
			pScale2->m_Step = 0.5;
			::NewSettings.m_DysfuncSettings[i].SetDefaultColors(1);

			::NewSettings.m_DysfuncSettings[3].m_Type = TYPE_CELM;
			::NewSettings.m_DysfuncSettings[i].SetDefaultColors(1);

			::NewSettings.m_DysfuncSettings[3].m_Type = TYPE_CAXM;
		}

		::NewSettings.m_DysfuncSettings[i].m_Ltr = 0;
		::NewSettings.m_DysfuncSettings[i].m_LtrLine = 70;
		::NewSettings.m_DysfuncSettings[i].m_LtrOrientation = 0;
	}

	::NewSettings.m_DysfuncSettings[3].m_MapShowAstigmatismAxes = 1;
	::NewSettings.m_DysfuncSettings[3].m_MapShowKeratometry = 1;
	::NewSettings.m_DysfuncSettings[3].m_MapShowEye = 1;
	::NewSettings.m_DysfuncSettings[3].m_MapShowSimKAxes = 1;
	//Dysfunctional Lens Analysis 531  done

	//For Dysfunctional lens patient
	::NewSettings.m_DysPtSettings[0] = ::NewSettings.m_DysfuncSettings[1];
	::NewSettings.m_DysPtSettings[0] = ::NewSettings.m_DysfuncSettings[1];
	::NewSettings.m_DysPtSettings[0].m_SimplifiedWFPanel = 1;
	::NewSettings.m_DysPtSettings[0].m_Type = TYPE_CLTR;
	::NewSettings.m_DysPtSettings[1] = ::NewSettings.m_DysfuncSettings[1];
	::NewSettings.m_DysPtSettings[2] = ::NewSettings.m_DysfuncSettings[1];
	::NewSettings.m_DysPtSettings[2].m_Type = TYPE_TLTR;
	::NewSettings.m_DysPtSettings[3] = ::NewSettings.m_DysfuncSettings[3];
	::NewSettings.m_DysPtSettings[4] = ::NewSettings.m_DysfuncSettings[2];

	::NewSettings.m_DysPtSettings[4].m_MapShowEye = 1;//6.2.0
	::NewSettings.m_DysPtSettings[4].m_MapShowLimbus = 1;//6.2.0
	::NewSettings.m_DysPtSettings[4].m_MapShowCornealVertex = 1;//6.2.0

	//For Depth focus display
	for (int i = 0; i < 4; i++)
	{
		::NewSettings.m_TSsm5Settings[i] = ::Settings.m_TSsmSettings[0][i];
		::NewSettings.m_TSsm5Settings[i].m_SizeSmall = FALSE;
		::NewSettings.m_TSsm5Settings[i].m_Type = TYPE_DOFF;
	}
	// Done

	// Depth focus Comparison display
	for (int i = 0; i < 4; i++)
	{
		::NewSettings.m_TSsm6Settings[i] = ::Settings.m_TSsmSettings[0][i];
		::NewSettings.m_TSsm6Settings[i].m_SizeSmall = FALSE;
		::NewSettings.m_TSsm6Settings[i].m_Type = TYPE_DOFC;
	}
	// Depth focus Comparison display Done

	for (int i = 0; i < 4; i++)
	{
		::NewSettings.m_OSHER_ALIG_CUS[i] = 0;
	}

	// Presbia scan size zone
	::NewSettings.m_ShowPreZone_WF_VFAWnd = FALSE;
	::NewSettings.m_ShowPreZone_WF_CMPWnd = FALSE;

	for (int i = 0; i < 4; i++)
	{
		::NewSettings.m_ShowPreZone_WF_VA_Wnd[i] = FALSE;
		::NewSettings.m_ShowPreZone_WF_AKA_Wnd[i] = FALSE;
	}

	//Excel Export	 
	for (int i = 0; i < 89; i++)
	{
		if (i < 53) ::Settings.m_EXCEL_WF_SETS[i] = '0';
		if (i < 68) ::Settings.m_EXCEL_CT_SETS[i] = '0';
		if (i < 89) ::Settings.m_EXCEL_WFCT_SETS[i] = '0';
	}
	::Settings.m_EXCEL_WFCT_SETS[0] = '3';

	::NewSettings.m_NEW_EXCEL_WF_SETS[0] = '0';//WFOpacity Grade
	::NewSettings.m_NEW_EXCEL_WFCT_SETS[0] = '0';//WFCT DLI Value
	::NewSettings.m_NEW_EXCEL_WFCT_SETS[1] = '0';//WFCT Opacity Grade

												 //Others in newsettings
	::NewSettings.m_IncLoC_OD = 0;
	::NewSettings.m_IncLoC_OS = 0;
	::NewSettings.m_SurInAst_OD = 0.0;
	::NewSettings.m_SurInAst_OS = 0.0;


	if (m_isSolo)
	{
		::NewSettings.m_OSHER_ALIG_CUS[2] = 1;
	}

	//Set the default widths of the exam list	
	for (int i = 0; i< 11; i++)
	{
		switch (i)
		{
		case 0:
			::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] = ::NewSettings.CTOS_COL_Width[i] = 0.030;
			break;
		case 1:
			::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] = ::NewSettings.CTOS_COL_Width[i] = 0.058;
			break;
		case 2:
			::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] = ::NewSettings.CTOS_COL_Width[i] = 0.046;
			break;
		case 4:
			::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = 0.05;
			::NewSettings.CTOD_COL_Width[i] = ::NewSettings.CTOS_COL_Width[i] = 0.002;
			break;
		case 3:
			::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] = ::NewSettings.CTOS_COL_Width[i] = 0.002;
			break;
		case 5:
			::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] = ::NewSettings.CTOS_COL_Width[i] = 0.002;
			break;
		case 6:
			::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] = ::NewSettings.CTOS_COL_Width[i] = 0.002;
			break;
		case 7:
			::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] = ::NewSettings.CTOS_COL_Width[i] = 0.002;
			break;
		case 8:
			::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] = ::NewSettings.CTOS_COL_Width[i] = 0.002;
			break;
		case 9:
			::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = ::NewSettings.CTOD_COL_Width[i] = ::NewSettings.CTOS_COL_Width[i] = 0.002;
			break;
		case 10:
			::NewSettings.WFOD_COL_Width[i] = ::NewSettings.WFOS_COL_Width[i] = 0.25;
			::NewSettings.CTOD_COL_Width[i] = ::NewSettings.CTOS_COL_Width[i] = 0.3;
			break;
		}
	}

	::NewSettings.m_ScreenShotFormat = 0;

	// For solo Aberration and Refraction analysis settings	
	::NewSettings.m_DefaultWFSolo = 1;//0:Point; 1:Refraction; 2:Aberration; 3:VFA
									  // Refraction Analysis Settings

	for (int i = 0; i<4; i++)
	{
		::NewSettings.m_WFSoloSettings[0][i].m_Type = TYPE_TLTR;//visual acuity

		switch (i)
		{
		case 0: ::NewSettings.m_WFSoloSettings[0][i].m_Mask.SetType(MASK_TOTAL);
			break;
		case 1: ::NewSettings.m_WFSoloSettings[0][i].m_Mask.SetType(MASK_HO_TOTAL);
			break;
		case 2: ::NewSettings.m_WFSoloSettings[0][i].m_Mask.SetType(MASK_TOTAL);
			break;
		case 3: ::NewSettings.m_WFSoloSettings[0][i].m_Mask.SetType(MASK_HO_TOTAL);
			break;
		}

		::NewSettings.m_WFSoloSettings[0][i].m_SizeSmall = TRUE;
		::NewSettings.m_WFSoloSettings[0][i].m_Piston = FALSE;
		::NewSettings.m_WFSoloSettings[0][i].m_Tilt = FALSE;
		::NewSettings.m_WFSoloSettings[0][i].m_Method3D = FALSE;
		::NewSettings.m_WFSoloSettings[0][i].m_MapShowSolidSurface = TRUE;
		::NewSettings.m_WFSoloSettings[0][i].m_MapShowWireMesh = TRUE;
		::NewSettings.m_WFSoloSettings[0][i].m_MapUnit = DIOPTERS;
		::NewSettings.m_WFSoloSettings[0][i].m_MapShowMap = 1;
		::NewSettings.m_WFSoloSettings[0][i].m_MapShowEye = 0;
		::NewSettings.m_WFSoloSettings[0][i].m_MapShowNumbers = 0;
		::NewSettings.m_WFSoloSettings[0][i].m_MapShowPupil = 0;
		::NewSettings.m_WFSoloSettings[0][i].m_MapShowLimbus = 0;
		::NewSettings.m_WFSoloSettings[0][i].m_MapShowCornealVertex = 0;
		::NewSettings.m_WFSoloSettings[0][i].m_MapTranslucent = 0;
		::NewSettings.m_WFSoloSettings[0][i].m_MapShowKeratometry = 0;
		::NewSettings.m_WFSoloSettings[0][i].m_ZoneRadiusUm = 5000;

		CScale* pScale = ::NewSettings.m_WFSoloSettings[0][i].GetScale();

		pScale->m_Step = 0.5;

		::NewSettings.m_WFSoloSettings[0][i].SetDefaultColors(1);
		::NewSettings.m_WFSoloSettings[0][i].m_Ltr = 0;
		::NewSettings.m_WFSoloSettings[0][i].m_LtrLine = 70;
		::NewSettings.m_WFSoloSettings[0][i].m_LtrOrientation = 0;
	}

	//Aberration Analysis Settings
	::NewSettings.m_WFSoloSettings[1][0].m_Type = 10;//Wavefront map 
	::NewSettings.m_WFSoloSettings[1][0].m_Mask.SetType(MASK_TOTAL);
	::NewSettings.m_WFSoloSettings[1][0].m_SizeSmall = TRUE;
	::NewSettings.m_WFSoloSettings[1][0].m_Piston = FALSE;
	::NewSettings.m_WFSoloSettings[1][0].m_Tilt = FALSE;
	::NewSettings.m_WFSoloSettings[1][0].m_Method3D = 0;
	::NewSettings.m_WFSoloSettings[1][0].m_MapShowSolidSurface = TRUE;
	::NewSettings.m_WFSoloSettings[1][0].m_MapShowWireMesh = TRUE;
	::NewSettings.m_WFSoloSettings[1][0].m_MapUnit = DIOPTERS;
	::NewSettings.m_WFSoloSettings[1][0].m_MapShowMap = 1;
	::NewSettings.m_WFSoloSettings[1][0].m_MapShowEye = 0;
	::NewSettings.m_WFSoloSettings[1][0].m_MapShowNumbers = 0;
	::NewSettings.m_WFSoloSettings[1][0].m_MapShowPupil = 0;
	::NewSettings.m_WFSoloSettings[1][0].m_MapShowLimbus = 0;
	::NewSettings.m_WFSoloSettings[1][0].m_MapShowCornealVertex = 0;
	::NewSettings.m_WFSoloSettings[1][0].m_MapTranslucent = 0;
	::NewSettings.m_WFSoloSettings[1][0].m_MapShowKeratometry = 0;
	::NewSettings.m_WFSoloSettings[1][0].m_ZoneRadiusUm = 5000;

	pScale = ::NewSettings.m_WFSoloSettings[1][0].GetScale();

	pScale->m_Step = 0.5;

	::NewSettings.m_WFSoloSettings[1][0].SetDefaultColors(1);
	::NewSettings.m_WFSoloSettings[1][0].m_Ltr = 0;
	::NewSettings.m_WFSoloSettings[1][0].m_LtrLine = 70;
	::NewSettings.m_WFSoloSettings[1][0].m_LtrOrientation = 0;



	::NewSettings.m_WFSoloSettings[1][1].m_Type = 10;//Wavefront map 
	::NewSettings.m_WFSoloSettings[1][1].m_Mask.SetType(MASK_HO_TOTAL);
	::NewSettings.m_WFSoloSettings[1][1].m_SizeSmall = TRUE;
	::NewSettings.m_WFSoloSettings[1][1].m_Piston = FALSE;
	::NewSettings.m_WFSoloSettings[1][1].m_Tilt = FALSE;
	::NewSettings.m_WFSoloSettings[1][1].m_Method3D = 0;
	::NewSettings.m_WFSoloSettings[1][1].m_MapShowSolidSurface = TRUE;
	::NewSettings.m_WFSoloSettings[1][1].m_MapShowWireMesh = TRUE;
	::NewSettings.m_WFSoloSettings[1][1].m_MapUnit = DIOPTERS;
	::NewSettings.m_WFSoloSettings[1][1].m_MapShowMap = 1;
	::NewSettings.m_WFSoloSettings[1][1].m_MapShowEye = 0;
	::NewSettings.m_WFSoloSettings[1][1].m_MapShowNumbers = 0;
	::NewSettings.m_WFSoloSettings[1][1].m_MapShowPupil = 0;
	::NewSettings.m_WFSoloSettings[1][1].m_MapShowLimbus = 0;
	::NewSettings.m_WFSoloSettings[1][1].m_MapShowCornealVertex = 0;
	::NewSettings.m_WFSoloSettings[1][1].m_MapTranslucent = 0;
	::NewSettings.m_WFSoloSettings[1][1].m_MapShowKeratometry = 0;
	::NewSettings.m_WFSoloSettings[1][1].m_ZoneRadiusUm = 5000;

	pScale = ::NewSettings.m_WFSoloSettings[1][1].GetScale();

	pScale->m_Step = 0.5;

	::NewSettings.m_WFSoloSettings[1][1].SetDefaultColors(1);
	::NewSettings.m_WFSoloSettings[1][1].m_Ltr = 0;
	::NewSettings.m_WFSoloSettings[1][1].m_LtrLine = 70;
	::NewSettings.m_WFSoloSettings[1][1].m_LtrOrientation = 0;

	::NewSettings.m_WFSoloSettings[1][2].m_Type = TYPE_TRMS;//TYPE_TMTF;
	::NewSettings.m_WFSoloSettings[1][2].m_Mask.SetType(MASK_TOTAL);
	::NewSettings.m_WFSoloSettings[1][2].m_RMSIndividual = FALSE;//Combined 
	::NewSettings.m_WFSoloSettings[1][2].m_SizeSmall = TRUE;
	::NewSettings.m_WFSoloSettings[1][2].m_Piston = FALSE;
	::NewSettings.m_WFSoloSettings[1][2].m_Tilt = FALSE;
	::NewSettings.m_WFSoloSettings[1][2].m_Method3D = 0;
	::NewSettings.m_WFSoloSettings[1][2].m_MapShowSolidSurface = TRUE;
	::NewSettings.m_WFSoloSettings[1][2].m_MapShowWireMesh = TRUE;
	::NewSettings.m_WFSoloSettings[1][2].m_MapUnit = DIOPTERS;
	::NewSettings.m_WFSoloSettings[1][2].m_MapShowMap = 1;
	::NewSettings.m_WFSoloSettings[1][2].m_MapShowEye = 0;
	::NewSettings.m_WFSoloSettings[1][2].m_MapShowNumbers = 0;
	::NewSettings.m_WFSoloSettings[1][2].m_MapShowPupil = 0;
	::NewSettings.m_WFSoloSettings[1][2].m_MapShowLimbus = 0;
	::NewSettings.m_WFSoloSettings[1][2].m_MapShowCornealVertex = 0;
	::NewSettings.m_WFSoloSettings[1][2].m_MapTranslucent = 0;
	::NewSettings.m_WFSoloSettings[1][2].m_MapShowKeratometry = 0;
	::NewSettings.m_WFSoloSettings[1][2].m_ZoneRadiusUm = 5000;


	::NewSettings.m_WFSoloSettings[1][3].m_Type = TYPE_TRMS;
	::NewSettings.m_WFSoloSettings[1][3].m_Mask.SetType(MASK_HO_TOTAL);
	::NewSettings.m_WFSoloSettings[1][3].m_RMSIndividual = TRUE;
	::NewSettings.m_WFSoloSettings[1][3].m_SizeSmall = TRUE;
	::NewSettings.m_WFSoloSettings[1][3].m_Piston = FALSE;
	::NewSettings.m_WFSoloSettings[1][3].m_Tilt = FALSE;
	::NewSettings.m_WFSoloSettings[1][3].m_Method3D = 0;
	::NewSettings.m_WFSoloSettings[1][3].m_MapShowSolidSurface = TRUE;
	::NewSettings.m_WFSoloSettings[1][3].m_MapShowWireMesh = TRUE;
	::NewSettings.m_WFSoloSettings[1][3].m_MapUnit = DIOPTERS;
	::NewSettings.m_WFSoloSettings[1][3].m_MapShowMap = 1;
	::NewSettings.m_WFSoloSettings[1][3].m_MapShowEye = 0;
	::NewSettings.m_WFSoloSettings[1][3].m_MapShowNumbers = 0;
	::NewSettings.m_WFSoloSettings[1][3].m_MapShowPupil = 0;
	::NewSettings.m_WFSoloSettings[1][3].m_MapShowLimbus = 0;
	::NewSettings.m_WFSoloSettings[1][3].m_MapShowCornealVertex = 0;
	::NewSettings.m_WFSoloSettings[1][3].m_MapTranslucent = 0;
	::NewSettings.m_WFSoloSettings[1][3].m_MapShowKeratometry = 0;
	::NewSettings.m_WFSoloSettings[1][3].m_ZoneRadiusUm = 5000;

	//Color image capture
	::Settings.m_Cap_ColorImg = 0;
	::Settings.m_Cap_ColorAutoOrManu = 1;

	//For Showing wf or ct buttons setting
	for (int i = 0; i < 8; i++)
	{
		::NewSettings.m_WFSumShow[i] = FALSE;
		::NewSettings.m_CTSumShow[i] = FALSE;
		::NewSettings.m_WFCTSumShow[i] = FALSE;
	}
}

//***************************************************************************************

void CSettings::SetSomeDefault()
{
	m_TSvrSettings[0].m_Type = TYPE_TEYE;
	m_TSvrSettings[0].m_Method3D = FALSE;
	m_TSvrSettings[0].m_SizeSmall = TRUE;

	m_TSvrSettings[1].m_Type = TYPE_TRSD;
	m_TSvrSettings[1].m_Method3D = FALSE;
	m_TSvrSettings[1].m_SizeSmall = TRUE;

	m_TSvrSettings[2].m_Type = TYPE_TSGN;
	m_TSvrSettings[2].m_Method3D = FALSE;
	m_TSvrSettings[2].m_SizeSmall = TRUE;

	m_TSvrSettings[3].m_Type = TYPE_TSGN;
	m_TSvrSettings[3].m_Method3D = FALSE;
	m_TSvrSettings[3].m_SizeSmall = TRUE;

	m_TMsmSettings.m_Type = TYPE_TRFM;
	m_TMsmSettings.m_Method3D = FALSE;
	m_TMsmSettings.m_SizeSmall = TRUE;
	m_TMsmSettings.m_ZoneRadiusUm = 5000;
	m_TMsmSettings.m_Mask.SetType(MASK_TOTAL);
	m_TMsmSettings.m_Piston = TRUE;
	m_TMsmSettings.m_Tilt = TRUE;

	// Chang Analysis
	m_IDsmSettings[0][0].m_Type = TYPE_IRMS;
	m_IDsmSettings[0][0].m_Method3D = FALSE;
	m_IDsmSettings[0][0].m_SizeSmall = TRUE;
	m_IDsmSettings[0][0].m_ZoneRadiusUm = 5000;
	m_IDsmSettings[0][0].m_Mask.SetType(MASK_TOTAL_NO_DEFOCUS);

	m_IDsmSettings[0][1].m_Type = TYPE_CRMS;
	m_IDsmSettings[0][1].m_Method3D = FALSE;
	m_IDsmSettings[0][1].m_SizeSmall = TRUE;
	m_IDsmSettings[0][1].m_ZoneRadiusUm = 5000;
	m_IDsmSettings[0][1].m_Mask.SetType(MASK_TOTAL_NO_DEFOCUS);

	m_IDsmSettings[0][2].m_Type = TYPE_TRMS;
	m_IDsmSettings[0][2].m_Method3D = FALSE;
	m_IDsmSettings[0][2].m_SizeSmall = TRUE;
	m_IDsmSettings[0][2].m_ZoneRadiusUm = 5000;
	m_IDsmSettings[0][2].m_Mask.SetType(MASK_TOTAL_NO_DEFOCUS);

	m_IDsmSettings[0][3].m_Type = TYPE_CAXM;
	m_IDsmSettings[0][3].m_Method3D = FALSE;
	m_IDsmSettings[0][3].m_SizeSmall = TRUE;
	m_IDsmSettings[0][3].m_MapShowEye = TRUE;
	m_IDsmSettings[0][3].m_MapShowMap = TRUE;
	m_IDsmSettings[0][3].m_MapShowKeratometry = TRUE;
	m_IDsmSettings[0][3].m_MapShowSimKAxes = TRUE;

	// statndard WF / CT summary
	m_WTsmSettings[0].m_Type = TYPE_TEYE;
	m_WTsmSettings[0].m_Method3D = FALSE;
	m_WTsmSettings[0].m_SizeSmall = TRUE;

	m_WTsmSettings[1].m_Type = TYPE_TWFM;
	m_WTsmSettings[1].m_Method3D = FALSE;
	m_WTsmSettings[1].m_SizeSmall = TRUE;
	m_WTsmSettings[1].m_ZoneRadiusUm = 5000;
	m_WTsmSettings[1].m_Mask.SetType(MASK_TOTAL);

	m_WTsmSettings[2].m_Type = TYPE_CAXM;
	m_WTsmSettings[2].m_Method3D = FALSE;
	m_WTsmSettings[2].m_SizeSmall = TRUE;
}

//***************************************************************************************

void CSettings::GetButtonName(int i, int j, CString& Name)
{
	if (strncmp((const char*)m_ButtonNames[i][j], "OK", 2) == 0 && j < 5)
	{
		char s[19];
		memset(s, 0, sizeof(s));
		strncpy(s, (const char*)m_ButtonNames[i][j] + 2, 18);
		Name = s;

		//6.2.0 Chinese
		int count = Name.Find(_T("Chinese"), 0);
		if (count != -1) //chinese
		{
			char s2[201];
			memset(s2, 0, sizeof(s2));
			strncpy(s2, (const char*)m_ChButtonNames[i][j] + 2, 200);
			Name = s2;

			CString Str = Name;

			Name = "";
			{
				int Len = Str.GetLength();
				int NewLen = Len;
				CStringA StrId = "";

				for (int k = 0; k < Len; k++)
				{
					CString Namestr = Str.Left(1);

					if (Namestr == "*")
					{
						BOOL Chinese = FALSE;

						TCHAR ch = StrId.GetAt(0);
						int Len = StrId.GetLength();

						if (Len > 1)
						{
							Chinese = TRUE;
							StrId.Replace("C", "");
						}

						if (Chinese)
						{
							int id = atoi(StrId);
							Name += G_GetChinese(id);
						}
						else
						{
							Name += StrId;
						}
						StrId = "";
					}
					else
					{
						StrId += Namestr;
					}

					Str = Str.Right(NewLen - 1);
					NewLen = Str.GetLength();
				}
			}
		}
		//6.2.0 Chinese

		

		if (i == 2 && j == 2 && Name == "Custom")
		{
			Name = "Visual Acuity"; 
		}

		if (Name == "UCVA/BCVA" || Name == "VA With/Without")
		{
			Name = "M T F"; //6.2.0 "Visual Acuity";
		}


		//6.2.0 
		if (i == 2 && j == 2 && Name == "Visual Acuity")
		{
			Name = "M T F";
		}
		//6.2.0 

		if (Name == "Local ROC") Name = "Keratometry";

		if (i == 2 && j == 4 && Name == "Custom")
		{
			Name = "Astigmatism Source";
		}
	}
	else if (strncmp((const char*)m_ButtonNames2[i][j - 5], "OK", 2) == 0 && j > 5)
	{
		char s[19];
		memset(s, 0, sizeof(s));
		strncpy(s, (const char*)m_ButtonNames2[i][j - 5] + 2, 18);
		Name = s;

		//6.2.0 Chinese
		int count = Name.Find(_T("Chinese"), 0);
		if (count != -1) //chinese
		{
			char s2[201];
			memset(s2, 0, sizeof(s2));
			strncpy(s2, (const char*)m_ChButtonNames2[i][j - 5] + 2, 200);
			Name = s2;

			CString Str = Name;

			Name = "";
			{
				int Len = Str.GetLength();
				int NewLen = Len;
				CStringA StrId = "";

				for (int k = 0; k < Len; k++)
				{
					CString Namestr = Str.Left(1);

					if (Namestr == "*")
					{
						BOOL Chinese = FALSE;

						TCHAR ch = StrId.GetAt(0);
						int Len = StrId.GetLength();

						if (Len > 1)
						{
							Chinese = TRUE;
							StrId.Replace("C", "");
						}

						if (Chinese)
						{
							int id = atoi(StrId);
							Name += G_GetChinese(id);
						}
						else
						{
							Name += StrId;
						}
						StrId = "";
					}
					else
					{
						StrId += Namestr;
					}

					Str = Str.Right(NewLen - 1);
					NewLen = Str.GetLength();
				}
			}
		}
		//6.2.0 Chinese

		if (Name == "UCVA/BCVA" || Name == "VA With/Without")
		{
			Name = "M T F"; //6.2.0 "Visual Acuity";
		}

		if (Name == "Local ROC")
		{
			Name = "Keratometry";
		}
	}
	else
	{
		static CString DefaultNames[3][8] =
		{
			{ "Visual Acuity"     , "WF/RMS"   , "Ref/WF"   , "Custom", "Depth of Focus" },
			{ "CT Sum"        , "Keratometry", "3D Z-Elev", "Custom", "Osher Iris Imaging" },
			{ "Chang Analysis", "Summary"  , "M T F"   , "Custom", "OU Overview", "IOL Selection Analysis", "Astigmatism Source", "Angle K/A" }
		};

		Name = DefaultNames[i][j];
	}
}

//***************************************************************************************

void CSettings::SetButtonName(int i, int j, const CString& Name)
{
	//6.2.0 Chinese
	BOOL Chinese = FALSE;

	int len = Name.GetLength();

	for (int k = 0; k < len; k++)
	{
		TCHAR ch = Name.GetAt(k);

		if (ch > 255)
		{
			Chinese = TRUE;
			break;
		}
	}

	if (Chinese) //chinese
	{
		CStringA AName, CName;
		AName = "Chinese";
		CName = "";

		CString s;
		CString Str = Name;
		size_t len = Str.GetLength();

		int NewLen = len;

		for (int k = 0; k < len; k++)
		{
			CString Namestr = Str.Left(1);

			TCHAR ch = Namestr.GetAt(0);

			if (ch > 255)
			{
				int ChineseID = G_GetChineseId(Namestr);
				s.Format(_T("C%i*"), ChineseID);
				CName += s;
			}
			else
			{
				CName += Namestr;
				CName += "*";
			}

			Str = Str.Right(NewLen - 1);
			NewLen = Str.GetLength();
		}


		if (j < 5)// new
		{
			m_ButtonNames[i][j][0] = 'O';
			m_ButtonNames[i][j][1] = 'K';

			strncpy(&m_ButtonNames[i][j][2], AName, 18);
			strncpy(&m_ChButtonNames[i][j][2], CName, 200);
		}
		else
		{
			m_ButtonNames2[i][j - 5][0] = 'O';
			m_ButtonNames2[i][j - 5][1] = 'K';
			strncpy(&m_ButtonNames2[i][j - 5][2], AName, 18);
			strncpy(&m_ChButtonNames2[i][j - 5][2], CName, 200);
		}
	}
	else
	{
		CStringA AName(Name);
		if (j < 5)// new
		{
			m_ButtonNames[i][j][0] = 'O';
			m_ButtonNames[i][j][1] = 'K';

			strncpy(&m_ButtonNames[i][j][2], AName, 18);
		}
		else
		{
			m_ButtonNames2[i][j - 5][0] = 'O';
			m_ButtonNames2[i][j - 5][1] = 'K';
			strncpy(&m_ButtonNames2[i][j - 5][2], AName, 18); //(const char*)(LPCTSTR)Name
		}
	}
	//6.2.0 Chinese
}

//***************************************************************************************

CString DateToStr(const int Year, const int Month, const int Day)
{
	CString s;

	switch (::Settings.m_DateFormat)
	{
	case DATE_FORMAT_DD_MM_YYYY:
		s.Format(_T("%02i-%02i-%04i"), Day, Month, Year);
		break;
	case DATE_FORMAT_YYYY_MM_DD:
		s.Format(_T("%04i-%02i-%02i"), Year, Month, Day);
		break;
	default:
		s.Format(_T("%02i-%02i-%04i"), Month, Day, Year);
	}

	return s;
}

//***************************************************************************************

//Load The IOL Data from a file
BOOL CIOLData::LoadFromFile(const CString& FileName)
{
	BOOL Res = FALSE;

	HANDLE hFile = ::CreateFile(FileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		int ZipSize = ::GetFileSize(hFile, NULL);
		void* pZipDataNew = malloc(ZipSize);

		ulong bt;
		::ReadFile(hFile, pZipDataNew, ZipSize, &bt, NULL);

		if (::GetDecompressedSize(pZipDataNew, ZipSize) == sizeof(CIOLData))
		{
			if (::Decompress(pZipDataNew, ZipSize, this))
			{
				Res = TRUE;
			}
		}

		free(pZipDataNew);

		::CloseHandle(hFile);
	}

	return Res;
}

//***************************************************************************************

// Save The IOL Data to a file
void CIOLData::SaveIntoFile(const CString& FileName)
{
	HANDLE hFile = ::CreateFile(FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		void* pZipDataNew = malloc(sizeof(CIOLData));
		int ZipSize;

		if (::Compress(this, sizeof(CIOLData), pZipDataNew, &ZipSize))
		{
			int asize = ZipSize;
			ulong bt;
			::WriteFile(hFile, pZipDataNew, ZipSize, &bt, NULL);
		}

		free(pZipDataNew);

		::CloseHandle(hFile);
	}
}

//*********************************************************************************

// save average exam m_c infomation
void CAveExamInfo::SaveIntoFile(const CString& FileName)
{
	HANDLE hFile = ::CreateFile(FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		void* pZipDataNew = malloc(sizeof(CAveExamInfo));
		int ZipSize;

		if (::Compress(this, sizeof(CAveExamInfo), pZipDataNew, &ZipSize))
		{
			int asize = ZipSize;
			ulong bt;
			::WriteFile(hFile, pZipDataNew, ZipSize, &bt, NULL);
		}

		free(pZipDataNew);

		::CloseHandle(hFile);
	}
}

//***************************************************************************************

// Loding average exam m_c infomation
BOOL CAveExamInfo::LoadFromFile(const CString& FileName)
{
	BOOL Res = FALSE;

	HANDLE hFile = ::CreateFile(FileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		int ZipSize = ::GetFileSize(hFile, NULL);
		void* pZipDataNew = malloc(ZipSize);

		ulong bt;
		::ReadFile(hFile, pZipDataNew, ZipSize, &bt, NULL);

		if (::GetDecompressedSize(pZipDataNew, ZipSize) == sizeof(CAveExamInfo))
		{
			if (::Decompress(pZipDataNew, ZipSize, this))
			{
				Res = TRUE;
			}
		}

		free(pZipDataNew);

		::CloseHandle(hFile);
	}

	return Res;
}

//***************************************************************************************
