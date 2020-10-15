#include "pch.h"
#include "Compressor.h"
#include <libs/CommonLib/Compression/Compression.h>
#include <libs/CommonLib/Compression/Blob.h>

namespace {
  using namespace util::compress;

  void CheckCompressionParams(Version version, const void *data, unsigned size)
  {
    TRACEY_THROW_IF(version != 0, "Only version=0 is supported for Compression");
    TRACEY_THROW_IF(data == nullptr, "Invalid Compress arg (data)");
    TRACEY_THROW_IF(size < MIN_COMPRESSION_SIZE, "Invalid Compress arg (size)");
  }
}

namespace util::compress {
  VersionedData Decompress(const path &path)
  {
    CAtlFile file;
    if (file.Create(path.wstring().c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING) == S_OK) {
      CAtlFileMapping<std::uint8_t> mmf;
      if (mmf.MapFile(file) == S_OK) {
        return Decompress(mmf, mmf.GetMappingSize());
      }
    }
    return {};
  }

  VersionedData Decompress(const void *data, unsigned size)
  {
    return {0u, ::Decompress(data, size)};
  }

  Data Compress(Version version, const void *data, unsigned size)
  {
    CheckCompressionParams(version, data, size);
    return ::Compress(data, size);
  }

  void Compress(const path &path, Version version, const void *data, unsigned size)
  {
    CheckCompressionParams(version, data, size);

    auto compressed = Compress(version, data, size);

    CAtlTemporaryFile file;
    if (FAILED(file.Create()))
      TRACEY_THROW("Unable to create compressed file");

    if (file.Write(compressed.data(), compressed.size()) == S_OK) {
      if (file.Close(path.wstring().c_str()) == S_OK)
        return;
    }

    TRACEY_THROW("Unable to create compressed file");
  }
} // namespace compressor
