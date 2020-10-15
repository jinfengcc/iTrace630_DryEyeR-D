#include "pch.h"
#include "VersionInfo.h"

#pragma comment(lib, "version.lib")

VersionInfo::VersionInfo()
{
   (void)GetFileVersionInfo();
}

bool VersionInfo::GetFileVersionInfo()
{
   TCHAR filename[_MAX_PATH];
   DWORD len = GetModuleFileName(NULL, filename, _MAX_PATH);
   if (len == 0)
      return false;

   // read file version info
   DWORD dwDummyHandle; // will always be set to zero
   len = GetFileVersionInfoSize(filename, &dwDummyHandle);
   if (len == 0)
      return false;

   m_buf.resize(len);
   if (!::GetFileVersionInfo(filename, 0, len, &m_buf[0]))
      return false;

   LPVOID lpvi;
   UINT iLen;
   if (!VerQueryValue(&m_buf[0], _T("\\"), &lpvi, &iLen))
      return false;

   // copy fixed info to myself, which am derived from VS_FIXEDFILEINFO
   m_ffi = *reinterpret_cast<VS_FIXEDFILEINFO *>(lpvi);

   return m_ffi.dwSignature == VS_FFI_SIGNATURE;
}
