// stdafx.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef STDAFX_H
#define STDAFX_H

#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <numeric>
#include <optional>
#include <filesystem>
#include <limits>

namespace fs = std::filesystem;

namespace Gdiplus {
  using std::max;
  using std::min;
} // namespace Gdiplus

#include <atlbase.h>
#include <atlstr.h>
#include <atlfile.h>

#include <gsl/gsl>
#include <fmt/format.h>
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include <libs/CommonLib/constants.h>
#include <libs/CommonLib/utf8.h>
#include <libs/CommonLib/TraceyException.h>
#include <libs/LoggingLib/TkTraceWin.h>

#include "Legacy.h"
#include "LegacyDbg.h"

constexpr BOOL m_isCombo = FALSE;
constexpr BOOL m_isValid = FALSE;

#ifndef _D
#ifdef _DEBUG
#define _D(x) x
#else
#define _D(x)
#endif
#endif


#endif //STDAFX_H
