#include "pch.h"
#include "Hasher.h"

#include <atlfile.h>
#include <atlenc.h>

#pragma comment(lib, "bcrypt.lib")

Hasher::Hasher(Type type)
  : m_hAlg(AlgHandle(type))
{
  auto nts = BCryptCreateHash(m_hAlg, &m_hHash, nullptr, 0, NULL, 0, BCRYPT_HASH_REUSABLE_FLAG);
  Check(nts, "BCryptCreateHash() failed.");

  ULONG hashSize{};
  nts = BCryptGetProperty(m_hAlg, BCRYPT_HASH_LENGTH, reinterpret_cast<BYTE *>(&hashSize), sizeof(hashSize), &hashSize, 0);
  Check(nts, "BCryptGetProperty(...BCRYPT_OBJECT_LENGTH...) failed.");

  m_hash.resize(hashSize);
}

Hasher::~Hasher()
{
  if (m_hHash)
    BCryptDestroyHash(m_hHash);
}

void Hasher::Reset()
{
  // Ignore errors
  (void)BCryptFinishHash(m_hHash, m_hash.data(), m_hash.size(), 0);
}

bool Hasher::Update(const void *d, unsigned size) const
{
  return Update({static_cast<const BYTE *>(d), gsl::narrow<std::size_t>(size)});
}

bool Hasher::Update(gsl::span<const BYTE> v) const
{
  auto pd = v.data();
  for (auto size = v.size(); size != 0;) {
    constexpr int64_t N = std::numeric_limits<ULONG>::max();
    const auto        n = size < N ? size : N;

    auto nts = BCryptHashData(m_hHash, const_cast<BYTE *>(pd), gsl::narrow<ULONG>(n), 0);
    Check(nts, "BCryptHashData() failed.");

    pd += n;
    size -= n;
  }
  return true;
}

bool Hasher::Update(const fs::path &p, uint64_t size) const
{
  if (fs::file_size(p) == 0)
    return false;

  CAtlFile file;
  if (file.Create(p.wstring().c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING) != S_OK)
    return false;

  CAtlFileMapping map;
  if (map.MapFile(file) != S_OK)
    return false;

  if (size > map.GetMappingSize())
    size = map.GetMappingSize();

  return Update(map.GetData(), size);
}

void Hasher::Finish()
{
  auto nts = BCryptFinishHash(m_hHash, &m_hash[0], m_hash.size(), 0);
  Check(nts, "BCryptFinishHash() failed.");
}

BCRYPT_ALG_HANDLE Hasher::AlgHandle(Type type)
{
  static std::array<const wchar_t *, 3>   typeNames = {L"SHA1", L"SHA256", L"MD5"};
  static std::array<BCRYPT_ALG_HANDLE, 3> handles{};

  auto n = static_cast<int>(type);
  if (!handles[n]) {
    BCRYPT_ALG_HANDLE hAlg{};

    auto nts = BCryptOpenAlgorithmProvider(&hAlg, typeNames[n], NULL, 0);
    Check(nts, "BCryptOpenAlgorithmProvider() failed.");

    handles[n] = hAlg;
  }

  return handles[n];
}

void Hasher::GetHashStr(std::string &str) const
{
  char sz[100];
  int  size = sizeof(sz);

  AtlHexEncode(m_hash.data(), gsl::narrow<int>(m_hash.size()), sz, &size);
  sz[size] = 0;

  str = sz;
}

void Hasher::GetHashStr(std::wstring &str) const
{
  std::string s;
  GetHashStr(s);
  str = utf8_convert(s);
}

void Hasher::Check(NTSTATUS s, const char *msg)
{
  // static struct
  //{
  //  NTSTATUS st;
  //  const char *msg;
  //} errs[] = {STATUS_BUFFER_TOO_SMALL,
  //            "The size of the hash object specified by the cbHashObject parameter is not large enough to hold the hash object.",
  //            STATUS_INVALID_HANDLE,
  //            "The algorithm handle in the hAlgorithm parameter is not valid.",
  //            STATUS_INVALID_PARAMETER,
  //            "One or more parameters are not valid.",
  //            STATUS_NOT_SUPPORTED,
  //            "The algorithm provider specified by the hAlgorithm parameter does not support the hash interface."};

  if (s < 0)
    TRACEY_THROW(msg);
}
