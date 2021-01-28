/*************************************************************************
*
*       System: Merge DICOM Toolkit
*
*       Author: Merge Healthcare
*
*  Description: This contains general utilities for all the sample programs.
*
*************************************************************************
*
*                      COPYRIGHT (C) 2012
*                   Merge Healthcare Incorporated
*            900 Walnut Ridge Drive, Hartland, WI 53029
*
*                     -- ALL RIGHTS RESERVED --
*
*  This software is furnished under license and may be used and copied
*  only in accordance with the terms of such license and with the
*  inclusion of the above copyright notice.  This software or any other
*  copies thereof may not be provided or otherwise made available to any
*  other person.  No title to and ownership of the software is hereby
*  transferred.
*
************************************************************************/

#include "StdAfx.h"//6.1.0

#if defined(_WIN32)||defined(_WIN64)
#include <windows.h>
#endif

#if !defined(_MSDOS)         && !defined(_WIN32)  && !defined(_RMX3) && \
    !defined(INTEL_WCC)      && !defined(__OS2__) && !defined(UNIX) && \
    !defined(_ALPHA_OPENVMS) && !defined(OS9_68K) && !defined(OS9_PPC) && \
    !defined(VXWORKS) && !defined (_MACINTOSH)
#define UNIX            1
#endif

#if defined(MACH_NEXT)
#undef UNIX
#endif

#if defined(_MACINTOSH)
#include <Types.h>
#include <Events.h>
#include <Menus.h>
#include <Windows.h>
#include <time.h>
#include <console.h>
#include <SIOUX.h>
#endif

#if defined(INTEL_WCC)
#ifdef NO_EXT_KEYS
#undef NO_EXT_KEYS
#endif
#endif

#if defined(_MSDOS) || defined(_WIN32) || defined(INTEL_WCC)
#if !defined(_PHAR_LAP)
#include <conio.h>
#endif
#endif

#include <time.h>
#include <ctype.h>
#include <stdlib.h> /* malloc() */
#include <stdio.h>
#include <memory.h>
#ifdef UNIX
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h> 
#endif

#include "general_util.h"

#if defined(CLOCK_MONOTONIC)
/* Use clock_gettime, which returns struct timespec */
#elif defined(_WIN32) || defined(_WIN64)
/* Use QueryPerformanceCounter, which returns LARGE_INTEGER */
#else
/* No high-resolution timer available; use time_t */
#endif

void *GetIntervalStart()
{
#if defined(CLOCK_MONOTONIC)
	struct timespec *result = malloc(sizeof(struct timespec));
	if (clock_gettime(CLOCK_MONOTONIC, result) != 0)
	{
		free(result);
		result = NULL;
	}
	return result;
#elif defined(_WIN32) || defined(_WIN64)
	/* Use QueryPerformanceCounter, which returns LARGE_INTEGER */

	LARGE_INTEGER *result;
	/*LARGE_INTEGER *result = malloc(sizeof(LARGE_INTEGER));
	if (!QueryPerformanceCounter(result))
	{
		free(result);
		result = NULL;
	}*/
#else
	/* No high-resolution timer available; use time_t */
	time_t *result = malloc(sizeof(time_t));
	(void)time(result);
#endif
	return result;
}

double GetIntervalElapsed(void *start_generic)
{
	if (start_generic != NULL)
	{
#if defined(CLOCK_MONOTONIC)
		struct timespec now, start = *(struct timespec *)start_generic;
		static const double nanoseconds = 1E9;    /* nanoseconds per second */
		free(start_generic);
		if (clock_gettime(CLOCK_MONOTONIC, &now) == 0)
		{
			return (now.tv_sec + now.tv_nsec / nanoseconds - start.tv_sec - start.tv_nsec / nanoseconds);
		}
#elif defined(_WIN32)||defined(_WIN64)
		LARGE_INTEGER now;
		LARGE_INTEGER frequency;
		LARGE_INTEGER start = *(LARGE_INTEGER *)start_generic;
		free(start_generic);
		if (QueryPerformanceCounter(&now) && QueryPerformanceFrequency(&frequency))
		{
			return (now.QuadPart - start.QuadPart) / (double)frequency.QuadPart;
		}
#else
		time_t start = *(time_t *)start_generic;
		time_t now = time(NULL);
		free(start_generic);
		return difftime(now, start);
#endif
	}
	return 0.0;
}


/**
* Poll the input (stdin or console) for a 'quit' key. Platform dependant.
*  On Windows and related, check console for Q, q or Esc key, and try to check stdin as a pipe.
*  On Mac, check for Q, q, or Cmd-.
*  On Unix/Linux, check for a line starting with Q or q.
*  On other platforms, do nothing (always return false).
*/
int PollInputQuitKey()
{
	int quit = 0;
#if defined(_MACINTOSH)
	EventRecord    asEvent;
	Boolean        aqQuit = false;
#endif

#if defined(UNIX)
	/* Solaris 8/64 bit Intel seems to have trouble with fd_set if
	* it is FD_SETSIZE and on the stack.
	*/
	fd_set fds[4];
	struct timeval timeout;
	FD_ZERO(fds);
	FD_SET(fileno(stdin), fds);
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	if (select(4, fds, NULL, NULL, &timeout) > 0)
	{
		/* Note that this does not set the input to raw; i.e. user must enter Q <return>. */
		char line[512];
		fgets(line, sizeof line, stdin);
		quit = tolower(line[0]) == 'q';
	}
#endif

#if defined(_WIN32)
	/* Attempt to peek in stdin as if it were a pipe. If this succeeds, quit
	* if a 'Q' was read.
	*/
	DWORD bytesRead, totalBytesAvail, bytesLeftThisMessage;
	if (PeekNamedPipe(GetStdHandle(STD_INPUT_HANDLE), NULL, 0, NULL, &totalBytesAvail, &bytesLeftThisMessage) && totalBytesAvail > 0)
	{
		char buffer[1];
		if (ReadFile(GetStdHandle(STD_INPUT_HANDLE), buffer, 1, &bytesRead, NULL))
		{
			quit = buffer[0] == 'q' || buffer[0] == 'Q';
		}
	}
#endif

#if defined(_MSDOS) || defined(_WIN32)
#if defined(INTEL_BCC)
	if (kbhit())
#else
	if (_kbhit())
#endif
	{
		switch (_getch())
		{
		case    0:      /* Ignore function keys */
			_getch();
			break;

		case    'Q':    /* Quit */
		case    'q':
		case    27:
			quit = 1;
		}
	}

#elif defined(INTEL_WCC)
	if (kbhit())
	{
		switch (getch())
		{
		case    0:      /* Ignore function keys */
			getch();
			break;

		case    'Q':    /* Quit */
		case    'q':
		case    27:
			quit = 1;
		}
	}

#elif defined(_RMX3)
	switch (_getch())
	{
	case    0:      /* Ignore function keys */
		_getch();
		break;

	case    'Q':    /* Quit */
	case    'q':
	case    27:
		quit = 1;
	}
#elif defined(_MACINTOSH)
	if (WaitNextEvent(mDownMask | mUpMask | keyDownMask | keyUpMask | autoKeyMask,
		&asEvent, 10, NULL))
	{
		if (asEvent.what == keyDown) {
			unsigned char    abChar = asEvent.message & charCodeMask;
			Boolean            aqCmnd = ((asEvent.modifiers & cmdKey) != 0);

			if ((abChar == 'Q') || (abChar == 'q') || (aqCmnd && (abChar == '.')))
				aqQuit = true;
		}
		else if (asEvent.what == mouseDown) {
			WindowPtr    apWindow;
			short            awWhere;

			awWhere = FindWindow(asEvent.where, &apWindow);
			if (awWhere == inMenuBar) {
				MenuSelect(asEvent.where);
				HiliteMenu(0);
			}
			else if (awWhere == inSysWindow)
				SystemClick(&asEvent, apWindow);
		}
	}
#if defined(__MWERKS__)
	else
		SIOUXHandleOneEvent(&asEvent);
#endif
	if (aqQuit)
		quit = 1;
#endif
	return quit;
}
