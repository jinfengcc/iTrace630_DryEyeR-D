#pragma once

#define VERSION_MAJOR     6
#define VERSION_MAJOR_STR "6"

#define VERSION_MINOR     30
#define VERSION_MINOR_STR "30"

#define VERSION_YEAR     2020
#define VERSION_YEAR_STR "2020"

#define VERSION_DAY     1022
#define VERSION_DAY_STR "1022"

#ifndef RC_INVOKED
extern const unsigned SOFTWARE_VERSION_3;
extern const unsigned SOFTWARE_VERSION_4;
extern const unsigned SOFTWARE_VERSION;
#endif

CString FullSoftwareVersion();