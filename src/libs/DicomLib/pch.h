// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#include <atlbase.h>
#include <atlstr.h>
#include <ATLComTime.h>

#include <string>
#include <list>
#include <fstream>
#include <limits>
#include <algorithm>
#include <ranges>
#include <filesystem>

namespace fs = std::filesystem;

#include <wil/com.h>
#include <fmt/format.h>
#include <opencv2/opencv.hpp>

#include <libs/CommonLib/StringSplit.h>
#include <libs/CommonLib/Guid.h>
#include <libs/CommonLib/AppFiles.h>
#include <libs/CommonLib/TraceyException.h>

#include <interfaces/data/Dicom.h>

extern const char SOP_CLASS_UID[];
extern const char MWL_SOP_CLASS_UID[];

#endif //PCH_H
