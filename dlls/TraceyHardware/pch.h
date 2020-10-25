// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#include <libs/CommonLib/CommonIncl.h>
#include <libs/CommonLib/Implements.h>
#include <libs/CommonLib/thread/jthread.hpp>
#include <libs/LoggingLib/Logger.h>
#include <libs/CommonLib/Signal.h>

#include <chrono>

#include <wil/result_macros.h>

#include <interfaces/hal/IDevice.h>
#include <interfaces/hal/IFlash.h>
#include <interfaces/hal/ICamera.h>

using namespace hal;

#include "thw/thw_defines.h"
#include "thw/thw_data.h"
#include "thw/thw.h"

#include <libs/LegacyLib/Structures.h>
#include <libs/LegacyLib/Utils.h>

extern sig::Signal<> ShutdownSignal;

#endif // PCH_H
