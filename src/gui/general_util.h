/*************************************************************************
*
*       System: Merge DICOM Toolkit Sample Application - General Utilities Include File
*
*       Author: Merge Healthcare
*
*  Description: This module contains defintions used by applications
*               which use the Merge DICOM C/C++ Toolkit HIS/RIS Service
*	             functions.
*
*************************************************************************
*
*                         COPYRIGHT (C) 2012
*                  Merge Healthcare Incorporated
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
#ifndef MTI_GENERAL_UTIL_H
#define MTI_GENERAL_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif 

	extern void *GetIntervalStart(void);
	extern double GetIntervalElapsed(void *start);  /* also releases start */
	extern int PollInputQuitKey(void);  /* Poll stdin/console for quit key on appropriate platforms */

#ifdef __cplusplus
}
#endif 

#endif

