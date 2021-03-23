#include "pch.h"
#include "VersionInfo.h"

#pragma comment(lib, "version.lib")

VersionInfo::VersionInfo()
{
  TCHAR filename[_MAX_PATH];
  DWORD len = GetModuleFileName(nullptr, filename, _MAX_PATH);
  if (len > 0)
    GetFileVersionInfo(fs::path(filename));
}

VersionInfo::VersionInfo(const fs::path &path)
{
  GetFileVersionInfo(path);
}

void VersionInfo::GetFileVersionInfo(const fs::path &path)
{
  // read file version info
  DWORD dwDummyHandle;
  auto  len = GetFileVersionInfoSizeA(path.string().c_str(), &dwDummyHandle);
  if (len == 0)
    return;

  m_buf.resize(len);
  if (!::GetFileVersionInfoA(path.string().c_str(), 0, len, &m_buf[0]))
    return;

  LPVOID lpvi;
  UINT   iLen;
  if (VerQueryValue(&m_buf[0], _T("\\"), &lpvi, &iLen))
    m_ffi = *reinterpret_cast<VS_FIXEDFILEINFO *>(lpvi);
}
