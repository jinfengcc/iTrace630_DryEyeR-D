// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

//#include <afx.h>
#include <atlbase.h>
#include <wtl/atlapp.h>
#include <wtl/atlframe.h>
#include <wtl/atlmisc.h>
#include <wtl/atlcrack.h>
#include <wtl/atldlgs.h>
#include <wtl/atlctrls.h>
#include <wtl/atlctrlx.h>
#include <wtl/atlddx.h>

#include <libs/CommonLib/CommonIncl.h>
#include <libs/CommonLib/Implements.h>
#include <libs/CommonLib/ClassFactoryImpl.h>
#include <libs/CommonLib/TraceyRegistry.h>
#include <libs/CommonLib/AppFiles.h>

#include <interfaces/ITraceyDicom.h>
#include <libs/DicomLib/DicomQuery.h>
#include <libs/DicomLib/DicomSend.h>
#include <libs/DicomLib/DicomSqlite.h>

namespace fmt {
  template <>
  struct formatter<CStringA> : public formatter<std::string>
  {
    template <typename FormatContext>
    auto format(const CStringA &s, FormatContext &ctx)
    {
      formatter<std::string>::format(s.GetString(), ctx);
    }
  };
  template <>
  struct formatter<CStringW> : public formatter<std::string>
  {
    template <typename FormatContext>
    auto format(const CStringW &s, FormatContext &ctx)
    {
      CStringA sa(s);
      formatter<std::string>::format(sa.GetString(), ctx);
    }
  };
} // namespace fmt

template <class... Args>
constexpr std::uint32_t CombineEnumFlags(Args... args)
{
  return (... | (1 << static_cast<std::uint32_t>(args)));
}

template<class DLG>
int DoModal( DLG &dlg, HWND hParentWnd)
{
  return dlg.DoModal(hParentWnd ? hParentWnd : GetActiveWindow());
}

#endif //PCH_H
