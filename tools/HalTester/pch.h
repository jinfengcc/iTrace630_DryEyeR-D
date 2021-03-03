// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#define _WTL_NO_WTYPES
#define _WTL_NO_CSTRING
#define _WTYPES_NS
#define _ATL_USE_DDX_FLOAT

#include "targetver.h"

#include <functional>
#include <memory>
#include <stdexcept>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

namespace Gdiplus {
  using std::max;
  using std::min;
} // namespace Gdiplus

#include <fmt/format.h>
#include <gsl/gsl>
#include <wil/com.h>

#include <atlbase.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlstr.h>
#include <atlcom.h>
#include <atlcomcli.h>
#include <ATLComTime.h>

#include <wtl/atlapp.h>
#include <wtl/atlmisc.h>
#include <wtl/atlframe.h>
#include <wtl/atltheme.h>
#include <wtl/atlctrls.h>
#include <wtl/atldlgs.h>
#include <wtl/atlctrlw.h>
#include <wtl/atlctrlx.h>
#include <wtl/atlddx.h>
#include <wtl/atlcrack.h>
#include <wtl/atltheme.h>

#define CHAIN_MSG_MAP_MEMBER_PTR(chainMemberPtr)                                                                                           \
  if (chainMemberPtr && chainMemberPtr->ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult))                                         \
    return TRUE;

#include <gdiplus.h>
#include <shlwapi.h>

#include "Globals.h"

extern CAppModule _Module;

#include <wil/com.h>

#include <libs/CommonLib/VersionInfo.h>
#include <libs/CommonLib/CreateObj.h>
#include <libs/CommonLib/thread/jthread.h>

#include <interfaces/hal/IDevice.h>
#include <interfaces/hal/ICamera.h>

#include <WinHelpers.h>
#include <IView.h>

CRect GetSameAspectRect(CRect rc, const CRect &refRect);

#endif // PCH_H
