//***************************************************************************************

#pragma once

#include <gsl/gsl>
#include <vector>

//***************************************************************************************

BOOL Compress(const void *pData, const int DataSize, void *pZip, int *pZipSize);
int  GetDecompressedSize(const void *pZip, const int ZipSize);
BOOL Decompress(const void *pZip, const int ZipSize, void *pData);

//***************************************************************************************

std::vector<std::uint8_t> Compress(const void *data, unsigned dataSize);
std::vector<std::uint8_t> Decompress(const void *data, unsigned dataSize);

inline int DecompressedSize(gsl::span<std::uint8_t> data)
{
  return GetDecompressedSize(data.data(), gsl::narrow<int>(data.size()));
}

inline std::vector<std::uint8_t> Decompress(gsl::span<std::uint8_t> data)
{
  return Decompress(data.data(), data.size());
}
