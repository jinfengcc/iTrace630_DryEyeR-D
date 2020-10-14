//***************************************************************************************

#pragma once

//***************************************************************************************

#include <UxTheme.h>
//#include <TmSchema.h>
#include <vssym32.h>//vs2013 05282015 'use <vssym32.h> as substitude  of <TmSchema.h>

//***************************************************************************************

#define XP_BUTTON        0
#define XP_CHECK_BOX     1
#define XP_RADIO         2
#define XP_GROUP_BOX     3
#define XP_LIST_HEADER   4
#define XP_EDIT          5

//***************************************************************************************

BOOL DrawBackgintRound(int Type, HWND hWnd, HDC hDC, int Part, int State, RECT& Rect);

//***************************************************************************************
