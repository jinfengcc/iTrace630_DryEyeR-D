// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

//#define _USE_STD_COMPLEX

#include "CommonIncl.h"
#include <sstream>

#define ASSERT  ATLASSERT

static_assert(_ASSERT_LEVEL > 0);

#pragma warning(disable: 5033) // 'register' is no longer a supported storage class
#pragma warning(disable: 4456) // declaration of 'res' hides previous local declaration
#pragma warning(disable: 4457) //	declaration of 'buf' hides function parameter
#pragma warning(disable: 4458) // declaration of 'buf' hides class member
#pragma warning(disable: 4302) // 'type cast': truncation from 'HWND' to 'unsigned long'
#pragma warning(disable: 4311) // 'type cast': pointer truncation from 'HWND' to 'unsigned long'
#pragma warning(disable: 4244) // 'argument' : conversion from 'uint64_t' to 'unsigned int', possible loss of data

#endif //PCH_H
