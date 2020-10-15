#pragma once

// http://msdn.microsoft.com/en-us/library/aa376217(VS.85).aspx

#include <atlbase.h>
#include <bcrypt.h>
#include <gsl/gsl>
#include "NonCopyable.h"

class Hasher : public NonCopyable
{
public:
  enum class Type { SHA1, SHA256, MD5 };
  Hasher(Type type);
  ~Hasher();

  void Reset();
  bool Update(const void *d, unsigned size) const;
  bool Update(gsl::span<const BYTE>) const;
  bool Update(const fs::path &p, uint64_t size = std::numeric_limits<uint64_t>::max()) const;
  void Finish();

  gsl::span<const BYTE> GetHash() const
  {
    return {m_hash};
  }

  void GetHashStr(std::string &str) const;
  void GetHashStr(std::wstring &str) const;

private:
  Type               m_type{};
  BCRYPT_ALG_HANDLE  m_hAlg{};
  BCRYPT_HASH_HANDLE m_hHash{};
  std::vector<BYTE>  m_hash;

  static BCRYPT_ALG_HANDLE AlgHandle(Type type);
  static void              Check(NTSTATUS s, const char *msg);
};
