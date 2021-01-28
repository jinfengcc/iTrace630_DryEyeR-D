//***************************************************************************************

#include "StdAfx.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

int intRound(const real_t v)
{
	return (int)floor(v + 0.5);
}

real_t RealRound(const real_t v, const real_t d)
{
	return intRound(v / d) * d;
}

real_t round8(const real_t v)
{
	return (int)(100.0 * RealRound(v, 0.125)) * 0.01;
}

//***************************************************************************************

real_t hyp(const real_t x, const real_t y) { return sqrt(x * x + y * y); }

//***************************************************************************************

real_t angle(const real_t y, const real_t x)
{
	if (x == 0.0)
	{
		if (y == 0.0) return 0.0;
		if (y < 0.0) return 3.0 * _Pi_2;
		return _Pi_2;
	}
	real_t a = atan(y / x);

	if (x < 0.0) return a + _Pi;

	if (y < 0.0) return a + _2_Pi;

	return a;
}

//***************************************************************************************

int log2(const uint n)
{
	uint a = 1;

	for (int i = 0; i < 32; i++)
	{
		if (a == n) return i;
		a <<= 1;
	}

	return -1;
}

//***************************************************************************************

real_t factorial(const int n)
{
	real_t res = 1.0;

	for (int i = 2; i <= n; i++) res *= i;

	return res;
}

//***************************************************************************************
/*
void frw_byte_memcpy(
void* dst,
const void* src,
uint num_bytes);
{
uint n4 = num_bytes >> 2;
uint* di = (uint*)dst;
uint* si = (uint*)src;
while (n4--) *di++ = *si++;
uint rm = n & 0x00000003;
uchar* dc = (uchar*)di;
uchar* sc = (uchar*)si;
while (rm--) *dc++ = *sc++;
}
*/
void rev_byte_memcpy(void* dst_bytes, const void* src_bytes, uint num_bytes)
{
	uchar* d = (uchar*)dst_bytes;
	uchar* s = (uchar*)src_bytes + num_bytes - 1;

	while (num_bytes--) *d++ = *s--;
}

void rev_word_memcpy(void* dst_words, const void* src_words, uint num_words)
{
	ushort* d = (ushort*)dst_words;
	ushort* s = (ushort*)src_words + num_words - 1;

	while (num_words--) *d++ = *s--;
}

//***************************************************************************************

int CheckAngle(int a)
{
	return (a %= 360) >= 0 ? a : a + 360;
}

//***************************************************************************************

void OpenTxtFile(const char* FileName)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	memset(&pi, 0, sizeof(pi));

	char prm[1024];
	strcpy(prm, "\"");
	strcat(prm, "C:\\Windows\\notepad.exe");
	strcat(prm, "\" \"");
	strcat(prm, FileName);
	strcat(prm, "\"");

	CreateProcess(NULL, (LPWSTR)prm, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
}

//***************************************************************************************

void GUIDToStr(const GUID& guid, CString& s)
{
	s.Format(_T("{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}"),
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

	s.MakeUpper();
}

//***************************************************************************************

void StrToGUID(const char* s, GUID& guid)
{
	char t[17];

	uint u;

	memset(t, 0, sizeof(t));
	strncpy(t, s + 1, 8);
	sscanf(t, "%08x", &guid.Data1);

	memset(t, 0, sizeof(t));
	strncpy(t, s + 10, 4);
	sscanf(t, "%04x", &u);
	guid.Data2 = (ushort)u;

	memset(t, 0, sizeof(t));
	strncpy(t, s + 15, 4);
	sscanf(t, "%04x", &u);
	guid.Data3 = (ushort)u;

	memset(t, 0, sizeof(t));
	strncpy(t, s + 20, 4);
	strncat(t, s + 25, 12);
	for (int i = 0; i < 8; i++)
	{
		sscanf(t + (i << 1), "%02x", &u);
		guid.Data4[i] = (uchar)u;
	}
}

//***************************************************************************************
