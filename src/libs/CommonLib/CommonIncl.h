#pragma once

#include <Unknwn.h>
#include <shlobj.h>

#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <numeric>
#include <optional>
#include <filesystem>
#include <span>
#include <math.h>

namespace fs = std::filesystem;

namespace Gdiplus {
  using std::max;
  using std::min;
} // namespace Gdiplus

#include <atlbase.h>
#include <atlstr.h>
#include <atlfile.h>

#include <limits>

#include <fmt/format.h>
#include <gsl/gsl>

#pragma warning(push)
#pragma warning(disable : 5054)
#include <opencv2/opencv.hpp>
#pragma warning(pop)

#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include "types.h"
#include "constants.h"
#include "Result.h"
#include "utf8.h"
#include "TraceyException.h"
#include "NonCopyable.h"
#include "CreateObj.h"
#include "Implements.h"

static_assert(CV_MAJOR_VERSION == 4, "We only tested with OpenCV 4.x");
static_assert(CV_MINOR_VERSION >= 3, "OpenCV 4.3.x or newer is required");

//#include "Refraction.h"
