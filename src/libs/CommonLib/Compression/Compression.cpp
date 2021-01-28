//***************************************************************************************

#include "pch.h"
//#include "types.h"
#include "Compression.h"
#include "Zip.h"
#include "UnZip.h"
#include "Hasher.h"

//***************************************************************************************

#ifdef _DEBUG
uint RotateLeft(const uint x, const uint c)
{
  return (x << c) | (x >> (32 - c));
}

//***************************************************************************************

void Transform(const uchar *pBlock, uint *pState)
{
  const uint Shift[64] = {7,  12, 17, 22, 7,  12, 17, 22, 7,  12, 17, 22, 7,  12, 17, 22, 5,  9,  14, 20, 5,  9,
                          14, 20, 5,  9,  14, 20, 5,  9,  14, 20, 4,  11, 16, 23, 4,  11, 16, 23, 4,  11, 16, 23,
                          4,  11, 16, 23, 6,  10, 15, 21, 6,  10, 15, 21, 6,  10, 15, 21, 6,  10, 15, 21};

  static uint k[64];
  static BOOL Initialized = FALSE;
  if (!Initialized) {
    for (uint i = 0; i < 64; i++) {
      k[i] = (uint)floor(fabs(sin(i + 1.0)) * 4294967296.0);
    }
    Initialized = TRUE;
  }

  uint a = pState[0];
  uint b = pState[1];
  uint c = pState[2];
  uint d = pState[3];

  for (uint i = 0; i < 64; i++) {
    uint f, g;
    if (i < 16) {
      f = (b & c) | (~b & d);
      g = i;
    }
    else if (16 <= i && i < 32) {
      f = (b & d) | (c & ~d);
      g = (5 * i + 1) % 16;
    }
    else if (32 <= i && i < 48) {
      f = b ^ c ^ d;
      g = (3 * i + 5) % 16;
    }
    else {
      f = c ^ (b | ~d);
      g = (7 * i) % 16;
    }
    uint t = d;
    d      = c;
    c      = b;
    b += RotateLeft(a + f + k[i] + ((uint *)pBlock)[g], Shift[i]);
    a = t;
  }

  pState[0] += a;
  pState[1] += b;
  pState[2] += c;
  pState[3] += d;
}

//***************************************************************************************

void ComputeMD5ChecksumOrig(const uchar *pMessage, const int MessageSize, uchar *pDigest)
{
  const uint ChunkSize = 64; // 64 bytes == 512 bits

  ((uint *)pDigest)[0] = 0x67452301;
  ((uint *)pDigest)[1] = 0xefcdab89;
  ((uint *)pDigest)[2] = 0x98badcfe;
  ((uint *)pDigest)[3] = 0x10325476;

  uint NumFullChunks = MessageSize / ChunkSize;

  for (uint j = 0; j < NumFullChunks; j++) {
    Transform(pMessage + j * ChunkSize, (uint *)pDigest);
  }

  uint LastChunkSize = MessageSize % ChunkSize;

  uchar LastChunk[ChunkSize];

  for (uint i = 0; i < LastChunkSize; i++) {
    LastChunk[i] = pMessage[NumFullChunks * ChunkSize + i];
  }

  LastChunk[LastChunkSize] = 0x80;

  if (LastChunkSize <= ChunkSize - 9) {
    for (uint i = LastChunkSize + 1; i < ChunkSize - 8; i++) {
      LastChunk[i] = 0x00;
    }
    *((uint64 *)&LastChunk[ChunkSize - 8]) = (uint64)MessageSize * (uint64)8;
    Transform(LastChunk, (uint *)pDigest);
  }
  else {
    for (uint i = LastChunkSize + 1; i < ChunkSize; i++) {
      LastChunk[i] = 0x00;
    }

    Transform(LastChunk, (uint *)pDigest);

    for (uint i = 0; i < ChunkSize - 8; i++) {
      LastChunk[i] = 0x00;
    }

    *((uint64 *)&LastChunk[ChunkSize - 8]) = (uint64)MessageSize * (uint64)8;
    Transform(LastChunk, (uint *)pDigest);
  }
}
#endif
static void ComputeMD5Checksum(const void *pMessage, int messageSize, void *dest, unsigned destSize)
{
  Hasher hasher(Hasher::Type::MD5);
  hasher.Update(pMessage, messageSize);
  hasher.Finish();

  auto hash = hasher.GetHash();

  Expects(hash.size() <= destSize);
  memcpy(dest, hash.data(), destSize);

#ifdef _DEBUG
  uchar temp[16];
  ComputeMD5ChecksumOrig((const uchar *)pMessage, messageSize, temp);
  ATLASSERT(memcmp(temp, hash.data(), 16) == 0);
#endif // _DEBUG
}

//***************************************************************************************

BOOL Compress(const void *pData, const int DataSize, void *pZip, int *pZipSize)
{
  HZIP hZip = ::CreateZip(nullptr, DataSize, "");

  ::ZipAdd(hZip, _T("Data"), (void *)pData, (uint)DataSize);

  void *pZip1 = nullptr;

  static_assert(sizeof(int) == sizeof(unsigned long));
  using ulong = unsigned long;
  ::ZipGetMemory(hZip, &pZip1, (ulong *)pZipSize);

  memcpy(pZip, pZip1, *pZipSize);

  ::ComputeMD5Checksum(pData, DataSize, (uchar *)pZip + *pZipSize, 16);

  *pZipSize += 16;

  ::CloseZipZ(hZip);

  return TRUE;
}

std::vector<std::uint8_t> Compress(const void *pData, unsigned DataSize)
{
  HZIP hZip = ::CreateZip(nullptr, DataSize, "");

  ::ZipAdd(hZip, _T("Data"), (void *)pData, (uint)DataSize);

  void *pZip1   = nullptr;
  ulong zipSize = 0;

  ::ZipGetMemory(hZip, &pZip1, &zipSize);

  std::vector<std::uint8_t> cd(zipSize + 16);
  memcpy(&cd[0], pZip1, zipSize);

  ::ComputeMD5Checksum(pData, DataSize, &cd[zipSize], 16);

  ::CloseZipZ(hZip);

  return cd;
}

std::vector<std::uint8_t> Decompress(const void *data, unsigned comp_size)
{
  auto size = GetDecompressedSize(data, comp_size);
  if (size > 0) {
    auto dd = std::vector<std::uint8_t>(size);
    if (Decompress(data, comp_size, &dd[0]))
      return dd;
    else
      return {};
  }
  else {
    auto ud = static_cast<const std::uint8_t *>(data);
    return {ud, ud + comp_size};
  }
}

//***************************************************************************************

int GetDecompressedSize(const void *pZip, const int ZipSize)
{
  int DataSize = 0;

  HZIP hZip = ::OpenZip((void *)pZip, (uint)ZipSize - 16, "");

  int      i;
  ZIPENTRY ZipEntry;

  if (::FindZipItem(hZip, _T("Data"), true, &i, &ZipEntry) == ZR_OK) {
    DataSize = ZipEntry.unc_size;
  }

  ::CloseZipU(hZip);

  return DataSize;
}

//***************************************************************************************

BOOL Decompress(const void *pZip, const int ZipSize, void *pData)
{
  BOOL Result = FALSE;

  HZIP hZip = ::OpenZip((void *)pZip, (uint)ZipSize - 16, "");

  int      i;
  ZIPENTRY ZipEntry;

  if (::FindZipItem(hZip, _T("Data"), true, &i, &ZipEntry) == ZR_OK) {
    ::UnzipItem(hZip, i, pData, ZipEntry.unc_size);

    uchar MD5Checksum[16];
    ::ComputeMD5Checksum(pData, ZipEntry.unc_size, MD5Checksum, sizeof(MD5Checksum));

    if (memcmp((uchar *)pZip + (ZipSize - 16), MD5Checksum, 16) == 0) {
      Result = TRUE;
    }
    else {
      static_assert(_ASSERT_LEVEL > 0);
      // ::Error("Checksum verification failed.");
    }
  }

  ::CloseZipU(hZip);

  return Result;
}

//***************************************************************************************
