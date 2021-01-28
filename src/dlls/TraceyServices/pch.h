// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#include <libs/CommonLib/CommonIncl.h>
#include <libs/CommonLib/Implements.h>
#include <libs/CommonLib/Hasher.h>
#include <libs/CommonLib/Compression/Blob.h>
#include <libs/CommonLib/Compression/Compression.h>
#include <libs/LoggingLib/Logger.h>

#include <atlwin.h>
#include <wtl/atlapp.h>
#include <wtl/atlctrls.h>
#include <wtl/atlcrack.h>

#include <interfaces/ICalibration.h>
#include <interfaces/IDatabase.h>

#endif // PCH_H
