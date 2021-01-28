#pragma once

#include <Unknwn.h>

#ifdef TRACEYOBJ
#undef TRACEYOBJ
#endif

#define TRACEYOBJ(guid) struct __declspec(uuid(guid))

