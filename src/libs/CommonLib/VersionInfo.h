#pragma once

#include <vector>

class VersionInfo
{
public:
   VersionInfo();

   operator const VS_FIXEDFILEINFO *() const
   {
      return m_ffi.dwSignature == VS_FFI_SIGNATURE ? &m_ffi : nullptr;
   }
private:
   std::vector<BYTE> m_buf;
   VS_FIXEDFILEINFO  m_ffi{};

   bool GetFileVersionInfo();
};

