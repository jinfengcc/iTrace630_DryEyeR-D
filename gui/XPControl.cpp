//***************************************************************************************

#include "StdAfx.h"

#include "XPControl.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

typedef HTHEME(__stdcall* TYPE_OPEN_THEME_DATA) (HWND, const wchar_t*);
typedef HRESULT(__stdcall* TYPE_DRAW_THEME_BACKGROUND) (HTHEME, HDC, int, int, const RECT*, const RECT*);
typedef HRESULT(__stdcall* TYPE_CLOSE_THEME_DATA) (HTHEME);

//***************************************************************************************

BOOL DrawBackgintRound(int Type, HWND hWnd, HDC hDC, int Part, int State, RECT& Rect)
{
	HMODULE hDLL = ::LoadLibrary(_T("UxTheme.dll"));

	if (hDLL == NULL) return FALSE;

	TYPE_OPEN_THEME_DATA       OpenThemeData;
	TYPE_DRAW_THEME_BACKGROUND DrawThemeBackground;
	TYPE_CLOSE_THEME_DATA      CloseThemeData;

	OpenThemeData = (TYPE_OPEN_THEME_DATA)::GetProcAddress(hDLL, "OpenThemeData");
	DrawThemeBackground = (TYPE_DRAW_THEME_BACKGROUND)::GetProcAddress(hDLL, "DrawThemeBackground");
	CloseThemeData = (TYPE_CLOSE_THEME_DATA)::GetProcAddress(hDLL, "CloseThemeData");

	wchar_t s[16];
	switch (Type) {
	case XP_BUTTON: wcscpy(s, L"Button"); break;
	case XP_LIST_HEADER: wcscpy(s, L"HEADER"); break;
	case XP_EDIT: wcscpy(s, L"EDIT"); break;
	default: return FALSE;
	}

	HTHEME hTheme = OpenThemeData(hWnd, s);

	if (hTheme == NULL) return FALSE;

	DrawThemeBackground(hTheme, hDC, Part, State, &Rect, NULL);

	CloseThemeData(hTheme);

	::FreeLibrary(hDLL);

	return TRUE;
}

//***************************************************************************************
