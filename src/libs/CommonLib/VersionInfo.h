#pragma once

#include <vector>
#include <filesystem>

class VersionInfo
{
public:
  VersionInfo();
  VersionInfo(const std::filesystem::path &path);

  operator const VS_FIXEDFILEINFO *() const
  {
    return get();
  }
  const VS_FIXEDFILEINFO *get() const
  {
    return m_ffi.dwSignature == VS_FFI_SIGNATURE ? &m_ffi : nullptr;
  }

private:
  std::vector<BYTE> m_buf;
  VS_FIXEDFILEINFO  m_ffi{};

  void GetFileVersionInfo(const std::filesystem::path &path);
};
